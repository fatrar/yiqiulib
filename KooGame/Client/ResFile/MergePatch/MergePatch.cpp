// MergePatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define TestFile "F:\\yiqiulib\\KooGame\\Client\\ResFile\\release\\New.pak"

int _tmain(int argc, _TCHAR* argv[])
{
    //ResFile::UnpackFile(TestFile);
    //return 0;
    //__asm {int 3}
    if (argc != 3)
    {
        cout << "argc Invaild!" << endl;
        return 0;
    }
    //const char* pOldFile = "F:\\yiqiulib\\KooGame\\Client\\ResFile\\debug\\anim.pak";
    //const char* pPatchFile = "F:\\yiqiulib\\KooGame\\Client\\ResFile\\debug\\anim.pat";
    const char* pOldFile = argv[1];
    const char* pPatchFile = argv[2];

    ResFile::IResUpdater* p = ResFile::CreateResUpdater(
        pPatchFile);
    p->Update(pOldFile);
	return 0;
}

