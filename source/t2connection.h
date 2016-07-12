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

    //用户登录
    zval* login(char *fund_account, char *password);

    //股票代码查询
    zval* req330300(char *stock_id, char *exchange_type);

    //代码行情查询
    zval* req400(char* stock_id, char* exchange_type);

    //大约可买获取
    zval* req333001(char* stock_id, char* exchange_type, double entrust_price);

    //普通委托
    zval* req333002(char *stock_id, char *exchange_type, double entrust_amount, double entrust_price, char entrust_bs, char * entrust_prop);
    
    //证券持仓查询
    zval* req333104(char *stock_id, char *exchange_type, char query_mode, char *position_str, int request_num);
    
    //证券委托查询
    zval* req333101(char sort_direction, int action_in, char query_type, char query_mode, char *position_str, int request_num);
    
    //证券成交查询
    zval* req333102(char sort_direction, char query_type, char query_mode, char *position_str, int request_num);
    
    //证券持仓快速查询
    zval* req333103(char *position_str, int request_num);

    //委托撤单
    zval* req333017(int entrust_no);

    //银行转账
    zval* req332200(char *bank_no, char transfer_direction, double occur_balance, char *fund_password, char *bank_password);

    //存款资金账户转账日志查询
    zval* req332250(char *bank_no, int entrust_no, int action_in, char *position_str, int request_num);

    //银行余额查询
    zval* req332253(char *bank_no, char *fund_password, char *bank_password);

    //客户资金快速查询
    zval* req332254();

    //客户银行账户查询
    zval* req331157();

    //历史资金流水查询
    zval* req339200(int start_date, int end_date, char *position_str, int request_num);

    //历史转账流水查询
    zval* req339204(int start_date, int end_date, char *bank_no, int action_in, char *position_str, int request_num);



private:
    char *ini_file;
    char *lib_t2sdk_file;
    int iSystemNo;
    SecuRequestMode* lp_SecuRequestMode;
};

#endif /* T2SDK_T2CONNECTION_H */