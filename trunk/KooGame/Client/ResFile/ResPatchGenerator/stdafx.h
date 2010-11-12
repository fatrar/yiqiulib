// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <tchar.h>

#include <iostream>
using namespace std;
//#include <ifstream>
//#include <ofstream>

#include "Common.h"
#include "FileSystem.h"

#include "..\libCmdParser\inc\ICommand.h"

#include "ResFileUtil.h"
#include "StlHelper.h"
#include "..\..\Common\lzma\LzmaUtil.h"


// TODO: reference additional headers your program requires here

#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\libCmdParser.lib")
#else
#pragma comment(lib, "..\\release\\libCmdParser.lib")
#endif // _DEBUG
