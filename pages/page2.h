#pragma once

#include "framework.h"
#include "resource.h"
#include "page_base.h"
#include "page2_rsrc.h"
#include "shareddata.h"

class CPage2 : CPageBase
{
public:
	CPage2(HINSTANCE hInst, HPROPSHEETPAGE *pPsPages, SHAREDWIZDATA *pWizData);
	~CPage2();

private:
	LPCWSTR OpenFolderPicker(HWND hWnd);
	LRESULT DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	int pageIndex = 1;
};
