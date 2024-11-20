#pragma once

#include "framework.h"
#include "resource.h"
#include "page2_rsrc.h"
#include "shareddata.h"

class CPage2
{
public:
	CPage2(HINSTANCE hInst, HPROPSHEETPAGE *pPsPages, SHAREDWIZDATA *pWizData);
	~CPage2();

private:
	static LPCWSTR OpenFolderPicker(HWND hWnd);
	static LRESULT CALLBACK DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	int pageIndex = 1;
};
