#pragma once
#include "framework.h"
#include "resource.h"
#include "shareddata.h"

class CPage1
{
public:
	CPage1(HINSTANCE hInst, HPROPSHEETPAGE* psppage, SHAREDWIZDATA* wizdata);
	~CPage1();

private:
	static LRESULT CALLBACK DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	int pageIndex = 0;
};