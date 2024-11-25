#include "page2.h"

CPage2::CPage2(HINSTANCE hInst, HPROPSHEETPAGE *pPsPages, SHAREDWIZDATA *pWizData)
	: CPageBase(hInst)
{
	m_pPropSheetPageBuilder->dwFlags = PSP_DEFAULT | PSP_USEHEADERTITLE | PSP_USEHEADERSUBTITLE;
	m_pPropSheetPageBuilder->pszHeaderTitle = MAKEINTRESOURCE(IDS_PAGE2_TITLE);
	m_pPropSheetPageBuilder->pszHeaderSubTitle = MAKEINTRESOURCE(IDS_PAGE2_SUBTITLE);
	m_pPropSheetPageBuilder->pszTemplate = MAKEINTRESOURCE(IDD_PAGE2);

	pPsPages[pageIndex] = CreatePropertySheetPage(m_pPropSheetPageBuilder);

	Initialize();
}

CPage2::~CPage2()
{
}

LPCWSTR CPage2::OpenFolderPicker(HWND hWnd)
{
	CComPtr<IFileDialog> pfd;
	HRESULT hr = pfd.CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER);
	DWORD dwFlags;
	pfd->GetOptions(&dwFlags);
	pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM | FOS_PICKFOLDERS);
	pfd->SetTitle(L"Browse");
	pfd->Show(hWnd);
	CComPtr<IShellItem> psiResult;
	hr = pfd->GetResult(&psiResult);

	if (SUCCEEDED(hr))
	{
		PWSTR pszFilePath = NULL;
		hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
		return pszFilePath;
	}

	return NULL;
}

LRESULT CPage2::DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_COMMAND)
	{
		if (LOWORD(wParam) == IDC_BROWSE)
		{
			HWND ctrl = GetDlgItem(hwndDlg, IDC_LOCATION);
			SetWindowText(ctrl, OpenFolderPicker(hwndDlg));
			return TRUE;
		}
	}
	else if (uMsg == WM_NOTIFY)
	{
		LPNMHDR lpnm = (LPNMHDR)lParam;
		if (lpnm->code == PSN_SETACTIVE)
		{
			PropSheet_SetWizButtons(GetParent(hwndDlg),  PSWIZB_BACK | PSWIZB_NEXT);
		}
		else if (lpnm->code == PSN_WIZNEXT)
		{
			HWND ctrl = GetDlgItem(hwndDlg, IDC_LOCATION);
			int textLen = GetWindowTextLength(ctrl);
			if (textLen > 0)
			{
				std::wstring path(textLen + 1, L'\0');
				int copiedTextLen = GetWindowText(ctrl, &path[0], textLen + 1);
				path.resize(copiedTextLen);
				//pData->path = path.c_str(); // TODO(kawapure): Use class member
			}
			return TRUE;
		}
		else if (lpnm->code == PSN_QUERYCANCEL)
		{
			int result = MessageBox(hwndDlg, TEXT("Are you sure you want to cancel?"), TEXT("explorer7 Setup"), MB_YESNO | MB_ICONQUESTION);
			if (result == IDYES)
			{
				SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, FALSE);
			}
			else
			{
				SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, TRUE);
			}
			return TRUE;
		}
	}
	return 0;
}
