/**CPP*************************************************************************
 File            : IVCfgDlg.cpp
 Subsystem       : 
 Function Name(s): CIVCfgDlg
 Author          : YiQiu
 Date            : 2010-3-25  
 Time            : 18:08
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "resource.h"
#include "IVCfgDlg.h"




LRESULT CIVCfgDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	return TRUE;
}

LRESULT CIVCfgDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CIVCfgDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}






// End of file