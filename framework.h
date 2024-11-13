#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600    // Default value is 0x0400
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500       // Default value is 0x0400
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <prsht.h>
#include <CommCtrl.h>
#include <stdlib.h>
#include <atlbase.h>
#include <ShlObj.h>
#include <string>
#include <vector>
