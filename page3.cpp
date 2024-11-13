#include "page3.h"

CPage3::CPage3(HINSTANCE hInst, HPROPSHEETPAGE* psppage, SHAREDWIZDATA* wizdata)
{
	PROPSHEETPAGE psp;
	ZeroMemory(&psp, sizeof(psp));

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_DEFAULT | PSP_USEHEADERTITLE | PSP_USEHEADERSUBTITLE;
	psp.hInstance = hInst;
	psp.lParam = (LPARAM)&wizdata;
	psp.pszHeaderTitle = MAKEINTRESOURCE(IDS_PAGE3_TITLE);
	psp.pszHeaderSubTitle = MAKEINTRESOURCE(IDS_PAGE3_SUBTITLE);
	psp.pszTemplate = MAKEINTRESOURCE(IDD_PAGE3);
	psp.pfnDlgProc = (DLGPROC)DlgProc;
	psppage[pageIndex] = CreatePropertySheetPage(&psp);
}

CPage3::~CPage3()
{

}


LRESULT CPage3::DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SHAREDWIZDATA* pdata = (SHAREDWIZDATA*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);
	if (uMsg == WM_INITDIALOG)
	{
		// load data
		PROPSHEETPAGE* psp = (PROPSHEETPAGE*)lParam;
		pdata = (SHAREDWIZDATA*)(psp->lParam);
		SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (DWORD_PTR)pdata);
	}
	else if (uMsg == WM_NOTIFY)
	{
		LPNMHDR lpnm = (LPNMHDR)lParam;
		if (lpnm->code == PSN_SETACTIVE)
		{
			PropSheet_SetWizButtons(GetParent(hwndDlg), PSWIZB_DISABLEDFINISH);
			HWND status = GetDlgItem(hwndDlg, IDC_STATUS);
			PatchExplorerBinary(status, pdata);
		}
		else if (lpnm->code == PSN_QUERYCANCEL)
		{
			SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, TRUE);
			return TRUE;
		}
	}
	return 0;
}
