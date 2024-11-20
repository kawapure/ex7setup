#pragma once

#include "framework.h"
#include "resource.h"
#include "shareddata.h"

// Enable to test if initialisation finishes during runtime.
#define PAGE_BASE_VERIFY_FINISH_INIT

struct IPage
{
	virtual LRESULT DlgProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	virtual HRESULT Initialize() = 0;
};

class CPageBase : public IPage
{
public:
	CPageBase(HINSTANCE hInst);

	virtual HRESULT Initialize() override;

protected:
	static LRESULT CALLBACK DlgProcInternal(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HRESULT FinishInit();

	PROPSHEETPAGEW *m_pPropSheetPageBuilder = nullptr;

private:
	HINSTANCE m_hInst = nullptr;
	HPROPSHEETPAGE m_hPropSheetPage = nullptr;

#ifdef PAGE_BASE_VERIFY_FINISH_INIT
	bool m_fFinishedInit = false;
#endif

	int m_iPageIndex = 0;
};