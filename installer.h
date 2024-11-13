#pragma once
#include "framework.h"
#include "resource.h"
#include "shareddata.h"

std::wstring GetFileVersion(LPCWSTR path);
void PatchExplorerBinary(HWND status, SHAREDWIZDATA* pdata);
