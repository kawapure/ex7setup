#include "installer.h"

#include "pages/page3_rsrc.h"

std::wstring GetFileVersion(LPCWSTR path)
{
	DWORD verHandle = 0;
	DWORD verSize = GetFileVersionInfoSize(path, &verHandle);
	LPVOID verData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, verSize);
	if (verData != nullptr)
	{
		if (GetFileVersionInfo(path, verHandle, verSize, verData))
		{
			UINT size = 0;
			VS_FIXEDFILEINFO* verInfo = nullptr;
			VerQueryValue(verData, L"\\", reinterpret_cast<LPVOID*>(&verInfo), &size);
			if (verInfo != nullptr)
			{
				DWORD verMS = verInfo->dwFileVersionMS;
				DWORD verLS = verInfo->dwFileVersionLS;
				std::wstring version = std::to_wstring(HIWORD(verMS)) + L"." +
					std::to_wstring(LOWORD(verMS)) + L"." +
					std::to_wstring(HIWORD(verLS)) + L"." +
					std::to_wstring(LOWORD(verLS));

				HeapFree(GetProcessHeap(), 0, verData);
				return version;
			}
		}
	}
	HeapFree(GetProcessHeap(), 0, verData);
}

void AppendLog(HWND status, LPCWSTR str, bool newline)
{
	int length = GetWindowTextLength(status) + 1;
	std::wstring path(length + 1, L'\0');
	int copiedTextLen = GetWindowText(status, &path[0], length);
	path.resize(copiedTextLen);
	path += str;
	if (newline) path += L"\r\n";
	SetWindowText(status, path.c_str());

	// scroll to bottom
	int lineCount = SendMessage(status, EM_GETLINECOUNT, 0, 0);
	SendMessage(status, EM_LINESCROLL, 0, lineCount);

	// remove selection
	int textLength = GetWindowTextLength(status);
	PostMessage(status, EM_SETSEL, textLength, textLength);
}


PIMAGE_SECTION_HEADER GetEnclosingSectionHeader(DWORD rva, IMAGE_NT_HEADERS *ntHeaders)
{
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);
	for (unsigned int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++, section++)
	{
		DWORD size = section->Misc.VirtualSize;
		if (rva >= section->VirtualAddress && rva < (section->VirtualAddress + size))
		{
			return section;
		}
	}
	return nullptr;
}

LPVOID GetPtrFromRVA(DWORD rva, IMAGE_NT_HEADERS *ntHeaders, void *imageBase)
{
	PIMAGE_SECTION_HEADER sectionHeader = GetEnclosingSectionHeader(rva, ntHeaders);
	if (!sectionHeader) return nullptr;
	DWORD delta = sectionHeader->VirtualAddress - sectionHeader->PointerToRawData;
	return (LPVOID)((DWORD_PTR)imageBase + rva - delta);
}

void PrintSectionHeaders(LPCWSTR filePath, HWND status)
{
	HANDLE hFile = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	LPVOID lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)lpFileBase;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)lpFileBase + dosHeader->e_lfanew);

	// get the section headers
	PIMAGE_SECTION_HEADER sectionHeader = IMAGE_FIRST_SECTION(ntHeaders);
	int numberOfSections = ntHeaders->FileHeader.NumberOfSections;

	PIMAGE_SECTION_HEADER pApiSetSection = NULL;

	for (int i = 0; i < numberOfSections; ++i)
	{
		std::string name = std::string(reinterpret_cast<char*>(sectionHeader[i].Name), 8);
		AppendLog(status, std::wstring(name.begin(), name.end()).c_str(), true);
	}

	AppendLog(status, L"\r\nDLL Imports: ", true);

	//locate import rva
	DWORD importDirectoryRVA = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	PIMAGE_IMPORT_DESCRIPTOR importDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)GetPtrFromRVA(importDirectoryRVA, ntHeaders, lpFileBase);
	std::vector<LPCSTR> dlls = {"shlwapi.dll" ,"ole32.dll","explorerframe.dll"};

	while (importDescriptor->Name != NULL)
	{
		LPCSTR dllName = (LPCSTR)GetPtrFromRVA(importDescriptor->Name, ntHeaders, lpFileBase);

		AppendLog(status, std::wstring(dllName, dllName + strlen(dllName)).c_str(), true);

		//slow but ehhhh
		for (int i=0; i < dlls.size(); i++)
		{
			if (_stricmp(dllName, dlls[i]) == 0)
			{
				std::wstring oldname = std::wstring(dllName, dllName + strlen(dllName));
				int len = strlen("WRP64.dll");
				memcpy((LPVOID)dllName, "WRP64.dll", len);
				memset((LPVOID)(dllName + len), 0, strlen(dlls[i]) - len);

				oldname += L" => WRP64.dll";
				AppendLog(status, oldname.c_str(), true);
			}
		}
		++importDescriptor;
	}
}

void PatchExplorerBinary(HWND status, SHAREDWIZDATA *pdata)
{
	SetWindowText(status, L"");
	if (pdata->path != NULL)
	{
		std::wstring str = pdata->path;
		str += L"\\explorer.exe";
		if (PathFileExists(str.c_str()))
		{
			AppendLog(status, L"File: ", false);
			AppendLog(status, str.c_str(), true);
			AppendLog(status, L"Version: ", false);
			AppendLog(status, GetFileVersion(str.c_str()).c_str(), true);
			AppendLog(status, L"\r\nSection headers: ", true);
			PrintSectionHeaders(str.c_str(), status);

			SendMessage(GetDlgItem(GetParent(status), IDC_PROGRESS), PBM_SETPOS, (WPARAM)100, 0);
			PropSheet_SetWizButtons(GetParent(GetParent(status)), PSWIZB_FINISH);
		}
		else
		{
			AppendLog(status, L"File doesn't exist. Please check again.", true);
			PropSheet_SetWizButtons(GetParent(GetParent(status)), PSWIZB_DISABLEDFINISH | PSWIZB_BACK);
		}
	}
}