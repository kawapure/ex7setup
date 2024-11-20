#include "page_base.h"

CPageBase::CPageBase(HINSTANCE hInst)
	: m_hInst(hInst)
{
	m_pPropSheetPageBuilder = new PROPSHEETPAGEW;
	m_pPropSheetPageBuilder->dwSize = sizeof(PROPSHEETPAGEW);
	m_pPropSheetPageBuilder->hInstance = hInst;
	m_pPropSheetPageBuilder->pfnDlgProc = (DLGPROC)DlgProcInternal;
	m_pPropSheetPageBuilder->lParam = (LONG_PTR)this;
}

HRESULT CPageBase::Initialize()
{
	return FinishInit();
}

HRESULT CPageBase::FinishInit()
{
#ifdef PAGE_BASE_VERIFY_FINISH_INIT
	m_fFinishedInit = true;
#endif

	m_hPropSheetPage = CreatePropertySheetPageW(m_pPropSheetPageBuilder);

	delete m_pPropSheetPageBuilder;
	m_pPropSheetPageBuilder = nullptr;

	return m_hPropSheetPage ? S_OK : E_FAIL;
}

LRESULT CALLBACK CPageBase::DlgProcInternal(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_INITDIALOG)
	{
		PROPSHEETPAGE *pPsPage = (PROPSHEETPAGE *)lParam;
		IPage *pPage = (IPage *)pPsPage->lParam;

		if (pPage)
		{
			SetWindowLongPtrW(hwndDlg, GWLP_USERDATA, (LONG_PTR)pPage);
			return pPage->DlgProc(hwndDlg, uMsg, wParam, lParam);
		}
	}

	IPage *pPage = (IPage *)GetWindowLongPtrW(hwndDlg, GWLP_USERDATA);
	if (pPage)
	{
		return pPage->DlgProc(hwndDlg, uMsg, wParam, lParam);
	}

	return FALSE;
}