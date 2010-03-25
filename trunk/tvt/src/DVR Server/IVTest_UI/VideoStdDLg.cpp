/**CPP*************************************************************************
 File            : VideoStdDLg.cpp
 Subsystem       : 
 Function Name(s): CVideoStdDLg
 Author          : YiQiu
 Date            : 2010-3-25  
 Time            : 18:31
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "resource.h"
#include "VideoStdDLg.h"


CVideoStdDLg::CVideoStdDLg(void)
    : m_bSignal(0)
{
}

LRESULT CVideoStdDLg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow();
    ((CButton*)&GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
    ((CButton*)&GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
	return TRUE;
}

LRESULT CVideoStdDLg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DoDataExchange (TRUE, IDC_RADIO1); 
	EndDialog(wID);
	return 0;
}

LRESULT CVideoStdDLg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DoDataExchange (TRUE, IDC_RADIO1);
	EndDialog(wID);
	return 0;
}





// End of file
