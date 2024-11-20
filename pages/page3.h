#pragma once

#include "framework.h"
#include "resource.h"
#include "page3_rsrc.h"
#include "shareddata.h"
#include "installer.h"

class CPage3
{
public:
	CPage3(HINSTANCE hInst, HPROPSHEETPAGE *pPsPages, SHAREDWIZDATA *pWizData);
	~CPage3();

private:
	static LRESULT CALLBACK DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	int pageIndex = 2;
};
