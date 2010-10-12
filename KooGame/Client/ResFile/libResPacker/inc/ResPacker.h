/**H**************************************************************************
 File            : ResPacker.h
 Subsystem       : 
 Function Name(s): CResPacker
 Author          : YiQiu
 Date            : 2010-10-12  
 Time            : 16:06
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RESPACKER_H_2010_10
#define _RESPACKER_H_2010_10


#include "IResPacker.h"


namespace ResFile
{

class CResPacker :
    public IResPacker
{
public:
    CResPacker(void);
    ~CResPacker(void);
};

}




#endif  // _RESPACKER_H_2010_


// End of file

