#ifndef T2SDK_T2CONNECTION_H
#define T2SDK_T2CONNECTION_H

#include <Include/t2sdk_interface.h>
#include "source/t2sdk_struck.h"
#include "php_t2sdk.h"
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
    T2Connection(char *lib_t2sdk_file, char *ini_file, char *fund_account, char *password);
    //void connect();
    int connect(char * &error);
    void disconnect();
    zval* login();
    zval* req330300(char *stockId);

private:
    char *ini_file;
    char *lib_t2sdk_file;
    char *fund_account;
    char *password;
    int iSystemNo;
    SecuRequestMode* lp_SecuRequestMode;
};

#endif /* T2SDK_T2CONNECTION_H */