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
    T2Connection(char *lib_t2sdk_file, char *ini_file);
    ~T2Connection();
    //void connect();
    int connect(char * &error);
    void disconnect();
    zval* login(char *fund_account, char *password);

    //股票代码查询
    zval* req330300(char *stock_id);

    //代码行情查询
    zval* req400(char* stock_id, char* exchange_type);

    //大约可买获取
    zval* req333001(char* stock_id, char* exchange_type, double entrust_price);

    //普通委托
    zval* req333002(char *stock_id, char *exchange_type, double entrust_amount, double entrust_price, char entrust_bs, char * entrust_prop);
    
private:
    char *ini_file;
    char *lib_t2sdk_file;
    int iSystemNo;
    SecuRequestMode* lp_SecuRequestMode;
};

#endif /* T2SDK_T2CONNECTION_H */