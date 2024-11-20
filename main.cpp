#include "main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR, int nCmdShow)
{
	UNREFERENCED_PARAMETER(nCmdShow);
	UNREFERENCED_PARAMETER(hPrevInstance);

	ZeroMemory(&psppage, sizeof(HPROPSHEETPAGE) * NUM_PAGES);
	ZeroMemory(&wizdata, sizeof(wizdata));
	hInst = hInstance;

	try
	{
		CPage1 pg1 = CPage1(hInst, psppage, &wizdata);
		CPage2 pg2 = CPage2(hInst, psppage, &wizdata);
		CPage3 pg3 = CPage3(hInst, psppage, &wizdata);
	}
	catch (std::exception &e)
	{
		LPCSTR szExceptionMessage = e.what();

		MessageBoxA(NULL, szExceptionMessage, "Error", MB_OK);
	}

	PROPSHEETHEADER psh;
	ZeroMemory(&psh, sizeof(psh));
	psh.dwSize = sizeof(psh);
	psh.hInstance = hInstance;
	psh.hwndParent = NULL;
	psh.phpage = psppage;
	psh.dwFlags = PSH_WIZARD97 | PSH_WATERMARK | PSH_HEADER;
	psh.pszCaption = MAKEINTRESOURCE(IDS_WIZTITLE);
	psh.pszbmWatermark = MAKEINTRESOURCE(IDB_WATERMARK);
	psh.pszbmHeader = MAKEINTRESOURCE(IDB_HEADER);
	psh.nStartPage = 0;
	psh.nPages = NUM_PAGES;
	PropertySheet(&psh);

	return FALSE;
}