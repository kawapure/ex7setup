#pragma once

#include "framework.h"
#include "resource.h"
#include "page_base.h"
#include "page1_rsrc.h"
#include "shareddata.h"

class CPage1 : CPageBase
{
public:
	CPage1(HINSTANCE hInst, HPROPSHEETPAGE *pPsPages, SHAREDWIZDATA *pWizData);
	~CPage1();

private:
	LRESULT DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	int pageIndex = 0;
};