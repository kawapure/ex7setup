#pragma once

#include "framework.h"
#include "resource.h"
#include "page_base.h"
#include "page3_rsrc.h"
#include "shareddata.h"
#include "installer.h"

class CPage3 : CPageBase
{
public:
	CPage3(HINSTANCE hInst, HPROPSHEETPAGE *pPsPages, SHAREDWIZDATA *pWizData);
	~CPage3();

private:
	LRESULT DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	int pageIndex = 2;
};
