#pragma once

#include "framework.h"
#include "resource.h"
#include "page1_rsrc.h"
#include "shareddata.h"

class CPage1
{
public:
	CPage1(HINSTANCE hInst, HPROPSHEETPAGE *pPsPages, SHAREDWIZDATA *pWizData);
	~CPage1();

private:
	static LRESULT CALLBACK DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	int pageIndex = 0;
};