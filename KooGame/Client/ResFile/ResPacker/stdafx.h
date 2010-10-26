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


#include "ICommand.h"
#include "IResPacker.h"
#include "Common.h"

#include <iostream>
#include <string>
#include <fstream>
using namespace std;
//using namespace ResFile;

#pragma warning(disable: 4996)
// TODO: reference additional headers your program requires here


#ifdef _DEBUG
#pragma comment(lib, "..\\debug\\libCmdParser.lib")
#pragma comment(lib, "..\\debug\\libResPacker.lib")
#else
#pragma comment(lib, "..\\release\\libCmdParser.lib")
#pragma comment(lib, "..\\release\\libResPacker.lib")
#endif // _DEBUG

