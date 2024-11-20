#include "page1.h"

CPage1::CPage1(HINSTANCE hInst, HPROPSHEETPAGE *pPsPages, SHAREDWIZDATA* pWizData)
	: CPageBase(hInst)
{
	m_pPropSheetPageBuilder->dwFlags = PSP_DEFAULT | PSP_HIDEHEADER;
	m_pPropSheetPageBuilder->pszHeaderTitle = MAKEINTRESOURCE(IDS_PAGE1_TITLE);
	m_pPropSheetPageBuilder->pszTemplate = MAKEINTRESOURCE(IDD_PAGE1);

	HPROPSHEETPAGE hPsp = nullptr;
	pPsPages[pageIndex] = hPsp = CreatePropertySheetPage(m_pPropSheetPageBuilder);

	Initialize();
}

CPage1::~CPage1()
{
	//DestroyPropertySheetPage(psPtr[pageIndex]);
}

LRESULT CPage1::DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_INITDIALOG)
	{
		HWND wlcmTitle = GetDlgItem(hwndDlg, IDC_WELC);
		LOGFONT logFont = { 0 };
		logFont.lfHeight = 20;
		logFont.lfWeight = FW_BOLD;
		lstrcpy(logFont.lfFaceName, TEXT("MS Shell Dlg"));

		HFONT hf = CreateFontIndirect(&logFont);
		SendMessage(wlcmTitle, WM_SETFONT, (WPARAM)hf, TRUE);

		// Center the dialog
		RECT rect;
		GetWindowRect(GetParent(hwndDlg), &rect);

		int winWidth = rect.right - rect.left;
		int winHeight = rect.bottom - rect.top;

		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		int xPos = (screenWidth - winWidth) / 2;
		int yPos = (screenHeight - winHeight) / 2;

		SetWindowPos(GetParent(hwndDlg), NULL, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
	else if (uMsg == WM_NOTIFY)
	{
		LPNMHDR lpnm = (LPNMHDR)lParam;
		if (lpnm->code == PSN_SETACTIVE)
		{
			PropSheet_SetWizButtons(GetParent(hwndDlg), PSWIZB_NEXT);
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
