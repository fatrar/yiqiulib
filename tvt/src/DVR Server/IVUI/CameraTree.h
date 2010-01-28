/*H***************************************************************************
 File            : CameraTree.h
 Subsystem       : 
 Function Name(s): CCameraTree
 Author          : YiQiu
 Date            : 2010-1-22  
 Time            : 16:11
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _CAMERATREE_H_2010_
#define _CAMERATREE_H_2010_



class CCameraTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CCameraTree)

public:
	CCameraTree();
	virtual ~CCameraTree();

public:
    BOOL Init(int nNum);
protected:
	DECLARE_MESSAGE_MAP()
};










#endif  // End of file




