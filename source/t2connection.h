#ifndef T2SDK_T2CONNECTION_H
#define T2SDK_T2CONNECTION_H

#include <Include/t2sdk_interface.h>
#include "source/CCallback.h"
#include "php_t2sdk.h"
#include "source/t2sdk_struck.h"
#include <dlfcn.h> 

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