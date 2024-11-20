#include "page3.h"

CPage3::CPage3(HINSTANCE hInst, HPROPSHEETPAGE *pPsPages, SHAREDWIZDATA *pWizData)
	: CPageBase(hInst)
{
	m_pPropSheetPageBuilder->dwFlags = PSP_DEFAULT | PSP_USEHEADERTITLE | PSP_USEHEADERSUBTITLE;
	m_pPropSheetPageBuilder->pszHeaderTitle = MAKEINTRESOURCE(IDS_PAGE3_TITLE);
	m_pPropSheetPageBuilder->pszHeaderSubTitle = MAKEINTRESOURCE(IDS_PAGE3_SUBTITLE);
	m_pPropSheetPageBuilder->pszTemplate = MAKEINTRESOURCE(IDD_PAGE3);

	pPsPages[pageIndex] = CreatePropertySheetPage(m_pPropSheetPageBuilder);

	Initialize();
}

CPage3::~CPage3()
{

}


LRESULT CPage3::DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NOTIFY)
	{
		LPNMHDR lpnm = (LPNMHDR)lParam;
		if (lpnm->code == PSN_SETACTIVE)
		{
			PropSheet_SetWizButtons(GetParent(hwndDlg), PSWIZB_DISABLEDFINISH);
			HWND status = GetDlgItem(hwndDlg, IDC_STATUS);
			//PatchExplorerBinary(status, pData); // TODO(kawapure): Use class member.
		}
		else if (lpnm->code == PSN_QUERYCANCEL)
		{
			SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, TRUE);
			return TRUE;
		}
	}
	return 0;
}
