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

    // IResPacker
private:
     virtual void SetDefaultEncryptParam(
        EncryptAlgo eAlgo,
        void* peParam = NULL );

    virtual void SetDefaultCompressParam(
        CompressAlgo cAlgo,
        void* pcParam = NULL);

    virtual void SetCurrentPath(const char* pPath);

    virtual void AddFile(
        const char* pFileName );

    virtual void AddFile(
        const char* pFileName,
        CompressAlgo cAlgo,
        void* pcParam,
        EncryptAlgo eAlgo,    
        void* peParam );

    virtual bool MakeFile(
        const char* pPackFilePath,
        FileNamePos eFileNamePos);
};

}




#endif  // _RESPACKER_H_2010_


// End of file

