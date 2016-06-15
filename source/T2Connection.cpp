#include "t2connection.h"
#include "php_t2sdk.h"
#include "SecuTrade.h"
#include "SecuMD.h"

// 全局连接对象
// extern CConnectionInterface *T2SDK_G(g_pConnection);
// extern CConnectionInterface *T2SDK_G(g_pConnection)Hq;

// extern CBusiness g_szBusiness;
// extern CBusiness g_szBusinessHq;


T2Connection::T2Connection(char* lib_t2sdk_file, char *ini_file)
{
    this->lib_t2sdk_file = lib_t2sdk_file;
    this->ini_file = ini_file;
}


int T2Connection::connect(char * &error)
{
    void *handle = dlopen(this->lib_t2sdk_file, RTLD_LAZY | RTLD_GLOBAL);
    if (!handle)  
    {  
       error = dlerror();
       puts(error);
    }  
    
    T2NewConfig = (config) dlsym(handle, "NewConfig");
    T2NewConnection = (connection) dlsym(handle, "NewConnection");
    T2NewPacker = (packer) dlsym(handle, "NewPacker");
    T2NewUnPacker = (unpacker) dlsym(handle, "NewUnPacker");
    T2NewBizMessage = (biz_message) dlsym(handle, "NewBizMessage");

    SecuRequestMode* lp_SecuRequestMode = new SecuRequestMode();
    //lp_SecuRequestMode->InitConn("demo", "license.dat", "218.108.19.190:18002");
    int ret = lp_SecuRequestMode->InitConn(this->ini_file, "70000172", "111111", error);
    
    return ret;
}

void T2Connection::disconnect()
{
    delete lp_SecuRequestMode;
}

void T2Connection::login()
{
    lp_SecuRequestMode->ReqFunction331100();
}
