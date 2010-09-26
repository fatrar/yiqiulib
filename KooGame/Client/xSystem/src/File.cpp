/**CPP*************************************************************************
 File            : File.cpp
 Subsystem       : 
 Function Name(s): CFile
 Author          : YiQiu
 Date            : 2010-9-25  
 Time            : 10:48
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd. Ltd.
**************************************************************************cpp**/



#ifdef _WIN32
    #include "Windows_File.cpp"
#elif defined(__APPLE__)
    #include "IOS_File.cpp"
#elif defined(__SYMBIAN32__)

#else
    #error FileSystem unsupport system
#endif






// End of file