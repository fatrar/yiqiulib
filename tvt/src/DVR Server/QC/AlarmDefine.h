/*typedef  BOOL(__stdcall IniCard)();
typedef  BOOL(__stdcall AlarmOut)(DWORD);
typedef  BOOL(__stdcall AlarmIn)(DWORD *);
typedef  void (__stdcall QuitCard)();
*/


typedef BOOL (FAR __cdecl *IniCard)();
typedef void (FAR __cdecl *QuitCard)();
typedef BOOL (FAR __cdecl *AlarmIn)(DWORD * pAlarmIn);
typedef BOOL (FAR __cdecl *AlarmOut)(DWORD dwAlarmOut);

typedef BOOL (FAR __cdecl *CommOut)(char *pOut, int OutLen);//djx
typedef BOOL (FAR __cdecl *GetCardVer)();