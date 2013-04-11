// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning(disable: 4996 4786)
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <process.h>
using namespace std;

#include "ResFileDef.h"
#include "ResFileUtil.h"
#include "Common.h"

//using namespace FileSystem;
#include "..\..\Common\lzma\LzmaUtil.h"
#include "..\..\Common\inc\HashHelper.h"
#include "..\..\Common\inc\FileSystem.h"


// TODO: reference additional headers your program requires here