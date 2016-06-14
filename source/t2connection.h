#ifndef T2SDK_T2CONNECTION_H
#define T2SDK_T2CONNECTION_H

#include <Include/t2sdk_interface.h>
#include "source/CCallback.h"
#include "source/t2sdk_struck.h"
#include <dlfcn.h> 

typedef CConfigInterface* (*config)(); 
typedef CConnectionInterface* (*connection)(CConfigInterface*); 
typedef IF2Packer* (*packer)(int); 
typedef IF2UnPacker* (*unpacker)(void *, unsigned int);
typedef IBizMessage* (*biz_message)();

class T2Connection {
public:
    T2Connection(char *lib_t2sdk_file, char *ini_file);
    //void connect();
    bool connect(char * &error);
    void disconnect();
    void login();

private:
    char *ini_file;
    char *lib_t2sdk_file;
    CConfigInterface * lpConfig;
    int iSystemNo;
    CBusiness g_szBusiness;
};

#endif /* T2SDK_T2CONNECTION_H */