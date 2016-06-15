#ifndef T2SDK_T2CONNECTION_H
#define T2SDK_T2CONNECTION_H

#include <Include/t2sdk_interface.h>
#include "source/t2sdk_struck.h"
#include <dlfcn.h> 
#include "SecuTrade.h"
#include "SecuMD.h"

typedef CConfigInterface* (*config)(); 
typedef CConnectionInterface* (*connection)(CConfigInterface*); 
typedef IF2Packer* (*packer)(int); 
typedef IF2UnPacker* (*unpacker)(void *, unsigned int);
typedef IBizMessage* (*biz_message)();

class T2Connection {
public:
    T2Connection(char *lib_t2sdk_file, char *ini_file);
    //void connect();
    int connect(char * &error);
    void disconnect();
    void login();

private:
    char *ini_file;
    char *lib_t2sdk_file;
    char *fund_account;
    char *password;
    SecuRequestMode* lp_SecuRequestMode;
};

#endif /* T2SDK_T2CONNECTION_H */