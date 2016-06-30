#include "t2connection.h"
#include "php_t2sdk.h"

// 全局连接对象
// extern CConnectionInterface *T2SDK_G(g_pConnection);
// extern CConnectionInterface *T2SDK_G(g_pConnection)Hq;

// extern CBusiness g_szBusiness;
// extern CBusiness g_szBusinessHq;
extern config T2NewConfig;
extern connection T2NewConnection;
extern packer T2NewPacker;
extern unpacker T2NewUnPacker;
extern biz_message T2NewBizMessage;

zval * errorToZval(int errorNo, char *errorMsg)
{
    zval *result;
    ALLOC_INIT_ZVAL(result);
    array_init(result);

    add_assoc_long(result, "errorNo", errorNo);
    add_assoc_string(result, "errorMsg", errorMsg, 1);

    return result;
}

zval * packToZval(IF2UnPacker *pUnPacker)
{
    zval *result;
    ALLOC_INIT_ZVAL(result);
    array_init(result);

    if(NULL == pUnPacker)
    {
        return result;
    }

    puts("pUnPacker not null");
    
    int i = 0, t = 0, j = 0, k = 0;
    int ivalue;
    char cvalue;
    char *ccvalue = new char[10];
    char *svalue = new char[100];
    const char * csvalue;
    float fvalue;
    char index[8];
    char *col = new char[100];

    for (i = 0; i < pUnPacker->GetDatasetCount(); ++i)
    {
        // 设置当前结果集
        pUnPacker->SetCurrentDatasetByIndex(i);
        
        // 打印所有记录
        for (j = 0; j < (int)pUnPacker->GetRowCount(); ++j)
        {
            zval *arr;
            ALLOC_INIT_ZVAL(arr);
            array_init(arr);
            for (k = 0; k < pUnPacker->GetColCount(); ++k)
            {
                const char *col_name = pUnPacker->GetColName(k);
                  //strcpy(col, col_name);
                  //puts(col);
                //printf("%20c", pUnPacker->GetColType(k));
                switch (pUnPacker->GetColType(k))
                {
                    case 'I':
                    //printf("%20d", pUnPacker->GetIntByIndex(k));
                    ivalue = pUnPacker->GetIntByIndex(k);
                    add_assoc_long(arr, col_name, ivalue);
                    break;
                    
                    case 'C':
                    //printf("%20c", pUnPacker->GetCharByIndex(k));
                    cvalue = pUnPacker->GetCharByIndex(k);
                    sprintf(ccvalue, "%20c", cvalue);
                    add_assoc_string(arr, col_name, ccvalue, 1);
                    break;
                    
                    case 'S':
                    //printf("%20s", pUnPacker->GetStrByIndex(k));
                    csvalue = pUnPacker->GetStrByIndex(k);
                    strcpy(svalue, csvalue);
                    add_assoc_string(arr, col_name, svalue, 1);
                    break;
                    
                    case 'F':
                    //printf("%20f", pUnPacker->GetDoubleByIndex(k));
                    fvalue = pUnPacker->GetDoubleByIndex(k);
                    add_assoc_double(arr, col_name, fvalue);
                    break;
                    
                    case 'R':
                    {
                        break;
                    }               
                    default:
                    // 未知数据类型
                    printf("未知数据类型。\n");
                    break;
                }
            }
            printf("index:%d", j) ;  
            sprintf(index, "%d", j);  
            add_assoc_zval(result, index, arr);    
            pUnPacker->Next();
        }
    }
    return result;
}

T2Connection::T2Connection(char *lib_t2sdk_file, char *ini_file)
{
    this->lib_t2sdk_file = lib_t2sdk_file;
    this->ini_file = ini_file;
    // this->fund_account = fund_account;
    // this->password = password;
}


T2Connection::~T2Connection()
{
    puts("delete lp_SecuRequestMode;");
    delete lp_SecuRequestMode;
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

 lp_SecuRequestMode = new SecuRequestMode();
    //lp_SecuRequestMode->InitConn("demo", "license.dat", "218.108.19.190:18002");
 int ret = lp_SecuRequestMode->InitConn(this->ini_file, error);


 return ret;
}

void T2Connection::disconnect()
{
    puts("disconnect");
    delete lp_SecuRequestMode;
    puts("after disconnect");
}

zval* T2Connection::login(char *fund_account, char *password)
{
    IF2UnPacker *pUnPacker = NULL;
    int errorNo = 0;
    char *errorMsg = (char *)malloc(256);
    int iSystemNo = lp_SecuRequestMode->Login(fund_account, password, pUnPacker, errorNo, errorMsg);
    if(iSystemNo == -1)
    {
        return errorToZval(errorNo, errorMsg);
    }

    return packToZval(pUnPacker);
}

zval* T2Connection::req330300(char *stock_id, char *exchange_type)
{
    puts("begin req330300");
    IBizMessage* lpBizMessage = T2NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

        //功能号
    lpBizMessage->SetFunction(330300);
        //请求类型
    lpBizMessage->SetPacketType(REQUEST_PACKET);
        //设置营业部号
    lpBizMessage->SetBranchNo(1);
        //设置company_id
    lpBizMessage->SetCompanyID(91000);
       //设置SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
        //设置系统号
    lpBizMessage->SetSystemNo(lp_SecuRequestMode->iSystemNo);

        ///获取版本为2类型的pack打包器
    IF2Packer *pPacker = T2NewPacker(2);
    if(!pPacker)
    {
        return errorToZval(-1, "取打包器失败!");
    }
    pPacker->AddRef();

        ///开始打包
    pPacker->BeginPack();
    //加入字段名
    //pPacker->AddField("op_branch_no", 'I', 5);//操作分支机构
    pPacker->AddField("op_branch_no", 'I', 5);//操作分支机构
    pPacker->AddField("op_entrust_way", 'C', 1);//委托方式 
    pPacker->AddField("op_station", 'S', 255);//站点地址
    pPacker->AddField("query_type",'C');
    pPacker->AddField("exchange_type",'S');
    pPacker->AddField("stock_type",'S');
    pPacker->AddField("stock_code",'S');
    pPacker->AddField("position_str",'S');
    pPacker->AddField("request_num",'I', 5);
    
    
    ///加入对应的字段值
    pPacker->AddInt(lp_SecuRequestMode->m_op_branch_no);
    char entrust_way = lp_SecuRequestMode->GetEntrustWay();                        
    pPacker->AddChar(entrust_way); 
    string opStation = lp_SecuRequestMode->GetOpStation();            
    pPacker->AddStr(opStation.c_str());               
    pPacker->AddChar('0');
    pPacker->AddStr(exchange_type);
    pPacker->AddStr("");
    pPacker->AddStr(stock_id);
    pPacker->AddStr(" ");  
    pPacker->AddInt(1);
    ///加入对应的字段值
    
    //char output[100];
    // sprintf(output, "op_branch_no:%d entrust_way:%20c, op_station:%s", lp_SecuRequestMode->m_op_branch_no, entrust_way, opStation.c_str());
    // puts(output);
    ///结束打包
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());

    IF2UnPacker *pUnPacker = NULL;
    int errorNo = 0;
    char *errorMsg = "";

    int send = lp_SecuRequestMode->SendRequest(lpBizMessage, pPacker, iSystemNo, pUnPacker, errorNo, errorMsg);
    if(send != 0)
    {
        return errorToZval(errorNo, errorMsg);
    }

    return packToZval(pUnPacker);
}

zval* T2Connection::req400(char *stock_id, char *exchange_type)
{
    IBizMessage* lpBizMessage = T2NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

        //功能号
    lpBizMessage->SetFunction(400);
        //请求类型
    lpBizMessage->SetPacketType(REQUEST_PACKET);
        //设置营业部号
    lpBizMessage->SetBranchNo(1);
        //设置company_id
    lpBizMessage->SetCompanyID(91000);
       //设置SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
        //设置系统号
    lpBizMessage->SetSystemNo(lp_SecuRequestMode->iSystemNo);

        ///获取版本为2类型的pack打包器
    IF2Packer *pPacker = T2NewPacker(2);
    if(!pPacker)
    {
        return errorToZval(-1, "取打包器失败!");
    }
    pPacker->AddRef();

        ///开始打包
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("exchange_type",'S',4);
    pPacker->AddField("stock_code",'S',6);

    //加入字段值
    pPacker->AddStr(exchange_type);
    pPacker->AddStr(stock_id);
    ///加入对应的字段值
    
    //char output[100];
    // sprintf(output, "op_branch_no:%d entrust_way:%20c, op_station:%s", lp_SecuRequestMode->m_op_branch_no, entrust_way, opStation.c_str());
    // puts(output);
    ///结束打包
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());

    IF2UnPacker *pUnPacker = NULL;
    int errorNo = 0;
    char *errorMsg = (char *)malloc(256);

    int send = lp_SecuRequestMode->SendRequest(lpBizMessage, pPacker, iSystemNo, pUnPacker, errorNo, errorMsg);
    if(send != 0)
    {
        return errorToZval(errorNo, errorMsg);
    }

    return packToZval(pUnPacker);
}

zval* T2Connection::req333001(char *stock_id, char *exchange_type, double entrust_price)
{
    IBizMessage* lpBizMessage = T2NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

        //功能号
    lpBizMessage->SetFunction(333001);
        //请求类型
    lpBizMessage->SetPacketType(REQUEST_PACKET);
        //设置营业部号
    lpBizMessage->SetBranchNo(1);
        //设置company_id
    lpBizMessage->SetCompanyID(91000);
       //设置SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
        //设置系统号
    lpBizMessage->SetSystemNo(lp_SecuRequestMode->iSystemNo);

        ///获取版本为2类型的pack打包器
    IF2Packer *pPacker = T2NewPacker(2);
    if(!pPacker)
    {
        return errorToZval(-1, "取打包器失败!");
    }
    pPacker->AddRef();

        ///开始打包
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5); 
    pPacker->AddField("client_id", 'S', 18);//客户ID
    pPacker->AddField("fund_account", 'S', 18);//资金账号
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("password_type", 'C', 1); 
    pPacker->AddField("user_token", 'S', 40);
    pPacker->AddField("exchange_type", 'S', 4);
    pPacker->AddField("stock_account", 'S', 15);
    pPacker->AddField("stock_code", 'S', 6);
    pPacker->AddField("entrust_price", 'F', 18, 3);

    //加入字段值
    char entrust_way = lp_SecuRequestMode->GetEntrustWay();                        
    string opStation = lp_SecuRequestMode->GetOpStation(); 
    
    pPacker->AddInt(lp_SecuRequestMode->m_op_branch_no);                        
    pPacker->AddChar(entrust_way); 
    pPacker->AddStr(opStation.c_str());                  
    pPacker->AddInt(lp_SecuRequestMode->m_BranchNo);                
    pPacker->AddStr(lp_SecuRequestMode->m_client_id);            
    pPacker->AddStr(lp_SecuRequestMode->GetAccountName());         
    pPacker->AddStr(lp_SecuRequestMode->GetPassword());                
    pPacker->AddChar('2');                  
    pPacker->AddStr(lp_SecuRequestMode->m_opUserToken.c_str());         
    pPacker->AddStr(exchange_type);                 
    pPacker->AddStr("");
    pPacker->AddStr(stock_id);                
    pPacker->AddDouble(entrust_price);  
    
    // char output[1000];
    // sprintf(output, "op_branch_no:%d entrust_way:%20c, op_station:%s client_id:%s account_name:%s password:%s token:%s", lp_SecuRequestMode->m_op_branch_no, entrust_way, opStation.c_str(), lp_SecuRequestMode->m_client_id, lp_SecuRequestMode->GetAccountName(), lp_SecuRequestMode->GetPassword(),lp_SecuRequestMode->m_opUserToken.c_str());
    // puts(output);
    ///结束打包
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());

    IF2UnPacker *pUnPacker = NULL;
    int errorNo = 0;
    char *errorMsg = (char *)malloc(256);

    int send = lp_SecuRequestMode->SendRequest(lpBizMessage, pPacker, iSystemNo, pUnPacker, errorNo, errorMsg);
    if(send != 0)
    {
        return errorToZval(errorNo, errorMsg);
    }

    return packToZval(pUnPacker);
}

zval* T2Connection::req333002(char *stock_id, char *exchange_type, double entrust_amount, double entrust_price, char entrust_bs, char * entrust_prop)
{
    puts("begin 333002");

    IBizMessage* lpBizMessage = T2NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

        //功能号
    lpBizMessage->SetFunction(333002);
        //请求类型
    lpBizMessage->SetPacketType(REQUEST_PACKET);
        //设置营业部号
    lpBizMessage->SetBranchNo(1);
        //设置company_id
    lpBizMessage->SetCompanyID(91000);
       //设置SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
        //设置系统号
    lpBizMessage->SetSystemNo(lp_SecuRequestMode->iSystemNo);

        ///获取版本为2类型的pack打包器
    IF2Packer *pPacker = T2NewPacker(2);
    if(!pPacker)
    {
        return errorToZval(-1, "取打包器失败!");
    }
    pPacker->AddRef();

    puts("before pack");

        ///开始打包
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5); 
    pPacker->AddField("client_id", 'S', 18);//客户ID
    pPacker->AddField("fund_account", 'S', 18);//资金账号
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("password_type", 'C', 1); 
    pPacker->AddField("user_token", 'S', 40);
    pPacker->AddField("exchange_type", 'S', 4);
    pPacker->AddField("stock_account", 'S', 15);
    pPacker->AddField("stock_code", 'S', 6);
    pPacker->AddField("entrust_amount", 'F', 19, 2);
    pPacker->AddField("entrust_price", 'F', 18, 3);
    pPacker->AddField("entrust_bs", 'C', 1);
    pPacker->AddField("entrust_prop", 'S', 3);  
    //pPacker->AddField("batch_no", 'I', 8);

    //加入字段值
    char entrust_way = lp_SecuRequestMode->GetEntrustWay();                        
    string opStation = lp_SecuRequestMode->GetOpStation(); 
    
    pPacker->AddInt(lp_SecuRequestMode->m_op_branch_no);                        
    pPacker->AddChar(entrust_way); 
    pPacker->AddStr(opStation.c_str());                  
    pPacker->AddInt(lp_SecuRequestMode->m_BranchNo);
    pPacker->AddStr(lp_SecuRequestMode->m_client_id);            
    pPacker->AddStr(lp_SecuRequestMode->GetAccountName());         
    pPacker->AddStr(lp_SecuRequestMode->GetPassword());                
    pPacker->AddChar('2');  
    pPacker->AddStr(lp_SecuRequestMode->m_opUserToken.c_str());         
    pPacker->AddStr(exchange_type);                 
    pPacker->AddStr("");
    pPacker->AddStr(stock_id);                
    pPacker->AddDouble(entrust_amount);  
    pPacker->AddDouble(entrust_price);  
    pPacker->AddChar(entrust_bs);  
    pPacker->AddStr(entrust_prop);  
    //pPacker->AddInt(13);  
    
    //char output[1000];
    //printf("op_branch_no:%d entrust_way:%20c, op_station:%s client_id:%s account_name:%s password:%s token:%s entrust_bs:%20c entrust_prop:%s", lp_SecuRequestMode->m_op_branch_no, entrust_way, opStation.c_str(), lp_SecuRequestMode->m_client_id, lp_SecuRequestMode->GetAccountName(), lp_SecuRequestMode->GetPassword(),lp_SecuRequestMode->m_opUserToken.c_str(), entrust_bs, entrust_prop);
    ///结束打包
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());

    IF2UnPacker *pUnPacker = NULL;
    int errorNo = 0;
    char *errorMsg = (char *)malloc(256);

    int send = lp_SecuRequestMode->SendRequest(lpBizMessage, pPacker, iSystemNo, pUnPacker, errorNo, errorMsg);
    if(send != 0)
    {
        return errorToZval(errorNo, errorMsg);
    }

    return packToZval(pUnPacker);
}

zval* T2Connection::req333104(char *stock_id, char *exchange_type, char query_mode, char *position_str, int request_num)
{
    IBizMessage* lpBizMessage = T2NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

        //功能号
    lpBizMessage->SetFunction(333104);
        //请求类型
    lpBizMessage->SetPacketType(REQUEST_PACKET);
        //设置营业部号
    lpBizMessage->SetBranchNo(1);
        //设置company_id
    lpBizMessage->SetCompanyID(91000);
       //设置SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
        //设置系统号
    lpBizMessage->SetSystemNo(lp_SecuRequestMode->iSystemNo);

        ///获取版本为2类型的pack打包器
    IF2Packer *pPacker = T2NewPacker(2);
    if(!pPacker)
    {
        return errorToZval(-1, "取打包器失败!");
    }
    pPacker->AddRef();

        ///开始打包
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5); 
    pPacker->AddField("client_id", 'S', 18);//客户ID
    pPacker->AddField("fund_account", 'S', 18);//资金账号
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("password_type", 'C', 1); 
    pPacker->AddField("user_token", 'S', 40);
    pPacker->AddField("exchange_type", 'S', 4);
    pPacker->AddField("stock_account", 'S', 15);
    pPacker->AddField("stock_code", 'S', 6);
    pPacker->AddField("query_mode", 'C', 1);
    pPacker->AddField("position_str", 'S');
    pPacker->AddField("request_num", 'I', 5);

    //加入字段值
    char entrust_way = lp_SecuRequestMode->GetEntrustWay();                        
    string opStation = lp_SecuRequestMode->GetOpStation(); 
    
    pPacker->AddInt(lp_SecuRequestMode->m_op_branch_no);                        
    pPacker->AddChar(entrust_way); 
    pPacker->AddStr(opStation.c_str());                  
    pPacker->AddInt(lp_SecuRequestMode->m_BranchNo);                
    pPacker->AddStr(lp_SecuRequestMode->m_client_id);            
    pPacker->AddStr(lp_SecuRequestMode->GetAccountName());         
    pPacker->AddStr(lp_SecuRequestMode->GetPassword());                
    pPacker->AddChar('2');                  
    pPacker->AddStr(lp_SecuRequestMode->m_opUserToken.c_str());         
    pPacker->AddStr(exchange_type);                 
    pPacker->AddStr("");
    pPacker->AddStr(stock_id);                
    pPacker->AddChar(query_mode);  
    pPacker->AddStr(position_str);  
    pPacker->AddInt(request_num);  
    
    // char output[1000];
    // sprintf(output, "op_branch_no:%d entrust_way:%20c, op_station:%s client_id:%s account_name:%s password:%s token:%s", lp_SecuRequestMode->m_op_branch_no, entrust_way, opStation.c_str(), lp_SecuRequestMode->m_client_id, lp_SecuRequestMode->GetAccountName(), lp_SecuRequestMode->GetPassword(),lp_SecuRequestMode->m_opUserToken.c_str());
    // puts(output);
    ///结束打包
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());

    IF2UnPacker *pUnPacker = NULL;
    int errorNo = 0;
    char *errorMsg = (char *)malloc(256);

    int send = lp_SecuRequestMode->SendRequest(lpBizMessage, pPacker, iSystemNo, pUnPacker, errorNo, errorMsg);
    if(send != 0)
    {
        return errorToZval(errorNo, errorMsg);
    }

    return packToZval(pUnPacker);
}

zval* T2Connection::req333101(char sort_direction, int action_in, char query_type, char query_mode, char *position_str, int request_num)
{
    IBizMessage* lpBizMessage = T2NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

        //功能号
    lpBizMessage->SetFunction(333101);
        //请求类型
    lpBizMessage->SetPacketType(REQUEST_PACKET);
        //设置营业部号
    lpBizMessage->SetBranchNo(1);
        //设置company_id
    lpBizMessage->SetCompanyID(91000);
       //设置SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
        //设置系统号
    lpBizMessage->SetSystemNo(lp_SecuRequestMode->iSystemNo);

        ///获取版本为2类型的pack打包器
    IF2Packer *pPacker = T2NewPacker(2);
    if(!pPacker)
    {
        return errorToZval(-1, "取打包器失败!");
    }
    pPacker->AddRef();

        ///开始打包
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5); 
    pPacker->AddField("client_id", 'S', 18);//客户ID
    pPacker->AddField("fund_account", 'S', 18);//资金账号
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("password_type", 'C', 1); 
    pPacker->AddField("user_token", 'S', 40);
    //pPacker->AddField("exchange_type", 'S', 4);
    pPacker->AddField("stock_account", 'S', 15);
    pPacker->AddField("stock_code", 'S', 6);
    pPacker->AddField("sort_direction", 'C', 1);
    pPacker->AddField("report_no", 'I', 8);
    pPacker->AddField("action_in", 'I', 5);
    pPacker->AddField("locate_entrust_no", 'I', 8);
    pPacker->AddField("query_type", 'C', 1);
    pPacker->AddField("query_mode", 'C', 1);
    pPacker->AddField("position_str", 'S');
    pPacker->AddField("request_num", 'I', 5);

    //加入字段值
    char entrust_way = lp_SecuRequestMode->GetEntrustWay();                        
    string opStation = lp_SecuRequestMode->GetOpStation(); 
    
    pPacker->AddInt(lp_SecuRequestMode->m_op_branch_no);                        
    pPacker->AddChar(entrust_way); 
    pPacker->AddStr(opStation.c_str());                  
    pPacker->AddInt(lp_SecuRequestMode->m_BranchNo);                
    pPacker->AddStr(lp_SecuRequestMode->m_client_id);            
    pPacker->AddStr(lp_SecuRequestMode->GetAccountName());         
    pPacker->AddStr(lp_SecuRequestMode->GetPassword());                
    pPacker->AddChar('2');                  
    pPacker->AddStr(lp_SecuRequestMode->m_opUserToken.c_str());

    //pPacker->AddStr(exchange_type);                 
    pPacker->AddStr("");
    pPacker->AddStr(""); 
    pPacker->AddChar(sort_direction);               
    pPacker->AddInt(0);  
    pPacker->AddInt(action_in);  
    pPacker->AddInt(0);
    pPacker->AddChar(query_type);  
    pPacker->AddChar(query_mode);  
    pPacker->AddStr(position_str);  
    pPacker->AddInt(request_num);  
    
    // char output[1000];
    // sprintf(output, "op_branch_no:%d entrust_way:%20c, op_station:%s client_id:%s account_name:%s password:%s token:%s", lp_SecuRequestMode->m_op_branch_no, entrust_way, opStation.c_str(), lp_SecuRequestMode->m_client_id, lp_SecuRequestMode->GetAccountName(), lp_SecuRequestMode->GetPassword(),lp_SecuRequestMode->m_opUserToken.c_str());
    // puts(output);
    ///结束打包
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());

    IF2UnPacker *pUnPacker = NULL;
    int errorNo = 0;
    char *errorMsg = (char *)malloc(256);

    int send = lp_SecuRequestMode->SendRequest(lpBizMessage, pPacker, iSystemNo, pUnPacker, errorNo, errorMsg);
    if(send != 0)
    {
        return errorToZval(errorNo, errorMsg);
    }

    return packToZval(pUnPacker);
}

zval* T2Connection::req333102(char sort_direction, char query_type, char query_mode, char *position_str, int request_num)
{
    IBizMessage* lpBizMessage = T2NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

        //功能号
    lpBizMessage->SetFunction(333102);
        //请求类型
    lpBizMessage->SetPacketType(REQUEST_PACKET);
        //设置营业部号
    lpBizMessage->SetBranchNo(1);
        //设置company_id
    lpBizMessage->SetCompanyID(91000);
       //设置SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
        //设置系统号
    lpBizMessage->SetSystemNo(lp_SecuRequestMode->iSystemNo);

        ///获取版本为2类型的pack打包器
    IF2Packer *pPacker = T2NewPacker(2);
    if(!pPacker)
    {
        return errorToZval(-1, "取打包器失败!");
    }
    pPacker->AddRef();

        ///开始打包
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5); 
    pPacker->AddField("client_id", 'S', 18);//客户ID
    pPacker->AddField("fund_account", 'S', 18);//资金账号
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("password_type", 'C', 1); 
    pPacker->AddField("user_token", 'S', 40);
    //pPacker->AddField("exchange_type", 'S', 4);
    pPacker->AddField("stock_account", 'S', 15);
    pPacker->AddField("stock_code", 'S', 6);
    pPacker->AddField("serial_no", 'I', 8);
    pPacker->AddField("query_direction", 'C', 1);
    pPacker->AddField("report_no", 'I', 8);
    pPacker->AddField("query_type", 'C', 1);
    pPacker->AddField("query_mode", 'C', 1);
    pPacker->AddField("position_str", 'S');
    pPacker->AddField("request_num", 'I', 5);

    //加入字段值
    char entrust_way = lp_SecuRequestMode->GetEntrustWay();                        
    string opStation = lp_SecuRequestMode->GetOpStation(); 
    
    pPacker->AddInt(lp_SecuRequestMode->m_op_branch_no);                        
    pPacker->AddChar(entrust_way); 
    pPacker->AddStr(opStation.c_str());                  
    pPacker->AddInt(lp_SecuRequestMode->m_BranchNo);                
    pPacker->AddStr(lp_SecuRequestMode->m_client_id);            
    pPacker->AddStr(lp_SecuRequestMode->GetAccountName());         
    pPacker->AddStr(lp_SecuRequestMode->GetPassword());                
    pPacker->AddChar('2');                  
    pPacker->AddStr(lp_SecuRequestMode->m_opUserToken.c_str());

    //pPacker->AddStr(exchange_type);                 
    pPacker->AddStr("");
    pPacker->AddStr(""); 
    pPacker->AddInt(0);  
    pPacker->AddChar(sort_direction);               
    pPacker->AddInt(0);
    pPacker->AddChar(query_type);  
    pPacker->AddChar(query_mode);  
    pPacker->AddStr(position_str);  
    pPacker->AddInt(request_num);  
    
    // char output[1000];
    // sprintf(output, "op_branch_no:%d entrust_way:%20c, op_station:%s client_id:%s account_name:%s password:%s token:%s", lp_SecuRequestMode->m_op_branch_no, entrust_way, opStation.c_str(), lp_SecuRequestMode->m_client_id, lp_SecuRequestMode->GetAccountName(), lp_SecuRequestMode->GetPassword(),lp_SecuRequestMode->m_opUserToken.c_str());
    // puts(output);
    ///结束打包
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());

    IF2UnPacker *pUnPacker = NULL;
    int errorNo = 0;
    char *errorMsg = (char *)malloc(256);

    int send = lp_SecuRequestMode->SendRequest(lpBizMessage, pPacker, iSystemNo, pUnPacker, errorNo, errorMsg);
    if(send != 0)
    {
        return errorToZval(errorNo, errorMsg);
    }

    return packToZval(pUnPacker);
}

zval* T2Connection::req333103(char *position_str, int request_num)
{
    IBizMessage* lpBizMessage = T2NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

        //功能号
    lpBizMessage->SetFunction(333103);
        //请求类型
    lpBizMessage->SetPacketType(REQUEST_PACKET);
        //设置营业部号
    // lpBizMessage->SetBranchNo(1);
    //     //设置company_id
    // lpBizMessage->SetCompanyID(91000);
    //    //设置SenderCompanyID
    // lpBizMessage->SetSenderCompanyID(91000);
        //设置系统号
    lpBizMessage->SetSystemNo(lp_SecuRequestMode->iSystemNo);

        ///获取版本为2类型的pack打包器
    IF2Packer *pPacker = T2NewPacker(2);
    if(!pPacker)
    {
        return errorToZval(-1, "取打包器失败!");
    }
    pPacker->AddRef();

        ///开始打包
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5); 
    pPacker->AddField("client_id", 'S', 18);//客户ID
    pPacker->AddField("fund_account", 'S', 18);//资金账号
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("password_type", 'C', 1); 
    pPacker->AddField("user_token", 'S', 40);
    //pPacker->AddField("exchange_type", 'S', 4);
    pPacker->AddField("stock_account", 'S', 15);
    pPacker->AddField("stock_code", 'S', 6);
    pPacker->AddField("position_str", 'S');
    pPacker->AddField("request_num", 'I', 5);

    //加入字段值
    char entrust_way = lp_SecuRequestMode->GetEntrustWay();                        
    string opStation = lp_SecuRequestMode->GetOpStation(); 
    
    pPacker->AddInt(lp_SecuRequestMode->m_op_branch_no);                        
    pPacker->AddChar(entrust_way); 
    pPacker->AddStr(opStation.c_str());                  
    pPacker->AddInt(lp_SecuRequestMode->m_BranchNo);                
    pPacker->AddStr(lp_SecuRequestMode->m_client_id);            
    pPacker->AddStr(lp_SecuRequestMode->GetAccountName());         
    pPacker->AddStr(lp_SecuRequestMode->GetPassword());                
    pPacker->AddChar('2');                  
    pPacker->AddStr(lp_SecuRequestMode->m_opUserToken.c_str());

    //pPacker->AddStr(exchange_type);                 
    pPacker->AddStr("");
    pPacker->AddStr(""); 
    pPacker->AddStr(position_str);  
    pPacker->AddInt(request_num);  
    
    // char output[1000];
    // sprintf(output, "op_branch_no:%d entrust_way:%20c, op_station:%s client_id:%s account_name:%s password:%s token:%s", lp_SecuRequestMode->m_op_branch_no, entrust_way, opStation.c_str(), lp_SecuRequestMode->m_client_id, lp_SecuRequestMode->GetAccountName(), lp_SecuRequestMode->GetPassword(),lp_SecuRequestMode->m_opUserToken.c_str());
    // puts(output);
    ///结束打包
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());

    IF2UnPacker *pUnPacker = NULL;
    int errorNo = 0;
    char *errorMsg = (char *)malloc(256);

    int send = lp_SecuRequestMode->SendRequest(lpBizMessage, pPacker, iSystemNo, pUnPacker, errorNo, errorMsg);
    if(send != 0)
    {
        return errorToZval(errorNo, errorMsg);
    }

    return packToZval(pUnPacker);
}

zval* T2Connection::req333017(int entrust_no)
{
    IBizMessage* lpBizMessage = T2NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

        //功能号
    lpBizMessage->SetFunction(333103);
        //请求类型
    lpBizMessage->SetPacketType(REQUEST_PACKET);
        //设置营业部号
    // lpBizMessage->SetBranchNo(1);
    //     //设置company_id
    // lpBizMessage->SetCompanyID(91000);
    //    //设置SenderCompanyID
    // lpBizMessage->SetSenderCompanyID(91000);
        //设置系统号
    lpBizMessage->SetSystemNo(lp_SecuRequestMode->iSystemNo);

        ///获取版本为2类型的pack打包器
    IF2Packer *pPacker = T2NewPacker(2);
    if(!pPacker)
    {
        return errorToZval(-1, "取打包器失败!");
    }
    pPacker->AddRef();

        ///开始打包
    pPacker->BeginPack();
    //加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5); 
    pPacker->AddField("client_id", 'S', 18);//客户ID
    pPacker->AddField("fund_account", 'S', 18);//资金账号
    pPacker->AddField("password", 'S', 10);
    pPacker->AddField("password_type", 'C', 1); 
    pPacker->AddField("user_token", 'S', 40);
    //pPacker->AddField("exchange_type", 'S', 4);
    pPacker->AddField("batch_flag", 'C', 1);
    pPacker->AddField("exchange_type", 'S', 4);
    pPacker->AddField("entrust_no", 'I', 8);

    //加入字段值
    char entrust_way = lp_SecuRequestMode->GetEntrustWay();                        
    string opStation = lp_SecuRequestMode->GetOpStation(); 
    
    pPacker->AddInt(lp_SecuRequestMode->m_op_branch_no);                        
    pPacker->AddChar(entrust_way); 
    pPacker->AddStr(opStation.c_str());                  
    pPacker->AddInt(lp_SecuRequestMode->m_BranchNo);                
    pPacker->AddStr(lp_SecuRequestMode->m_client_id);            
    pPacker->AddStr(lp_SecuRequestMode->GetAccountName());         
    pPacker->AddStr(lp_SecuRequestMode->GetPassword());                
    pPacker->AddChar('2');                  
    pPacker->AddStr(lp_SecuRequestMode->m_opUserToken.c_str());

    //pPacker->AddStr(exchange_type);                 
    pPacker->AddChar('0');
    pPacker->AddStr(" "); 
    pPacker->AddInt(entrust_no);  
    
    // char output[1000];
    // sprintf(output, "op_branch_no:%d entrust_way:%20c, op_station:%s client_id:%s account_name:%s password:%s token:%s", lp_SecuRequestMode->m_op_branch_no, entrust_way, opStation.c_str(), lp_SecuRequestMode->m_client_id, lp_SecuRequestMode->GetAccountName(), lp_SecuRequestMode->GetPassword(),lp_SecuRequestMode->m_opUserToken.c_str());
    // puts(output);
    ///结束打包
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());

    IF2UnPacker *pUnPacker = NULL;
    int errorNo = 0;
    char *errorMsg = (char *)malloc(256);

    int send = lp_SecuRequestMode->SendRequest(lpBizMessage, pPacker, iSystemNo, pUnPacker, errorNo, errorMsg);
    if(send != 0)
    {
        return errorToZval(errorNo, errorMsg);
    }

    return packToZval(pUnPacker);
}