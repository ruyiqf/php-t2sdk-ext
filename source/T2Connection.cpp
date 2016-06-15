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

T2Connection::T2Connection(char *lib_t2sdk_file, char *ini_file, char *fund_account, char *password)
{
    this->lib_t2sdk_file = lib_t2sdk_file;
    this->ini_file = ini_file;
    this->fund_account = fund_account;
    this->password = password;
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
    int ret = lp_SecuRequestMode->InitConn(this->ini_file, this->fund_account, this->password, error);
    

    return ret;
}

void T2Connection::disconnect()
{
    delete lp_SecuRequestMode;
}

zval* T2Connection::login()
{
    IF2UnPacker *pUnPacker;
    iSystemNo = lp_SecuRequestMode->Login(pUnPacker);

    puts("iSystemNo");
    zval *result;
    ALLOC_INIT_ZVAL(result);
    array_init(result);
    
    int i = 0, t = 0, j = 0, k = 0;

    for (i = 0; i < pUnPacker->GetDatasetCount(); ++i)
    {
        puts("nihao");
        // 设置当前结果集
        pUnPacker->SetCurrentDatasetByIndex(i);
        
        // 打印所有记录
        for (j = 0; j < (int)pUnPacker->GetRowCount(); ++j)
        {
            puts("111");
            zval* arr;
            ALLOC_INIT_ZVAL(arr);
            array_init(arr);
            for (k = 0; k < pUnPacker->GetColCount(); ++k)
            {
                const char *col_name = pUnPacker->GetColName(k);
                
                    char *col = new char[20];
                    strcpy(col, col_name);
                    puts(col);

                int ivalue;
                char cvalue;
                char *svalue;
                const char * csvalue;
                float fvalue;

                switch (pUnPacker->GetColType(k))
                {
                case 'I':
                    //printf("%20d", pUnPacker->GetIntByIndex(k););
                    ivalue = pUnPacker->GetIntByIndex(k);
                    add_assoc_long(arr, col_name, ivalue);
                    break;
                    
                case 'C':
                    //printf("%20c", pUnPacker->GetCharByIndex(k));
                    cvalue = pUnPacker->GetCharByIndex(k);
                    add_assoc_string(arr, col_name, (char *)cvalue, 1);
                    break;
                    
                case 'S':
                    //printf("%20s", pUnPacker->GetStrByIndex(k));
                    csvalue = pUnPacker->GetStrByIndex(k);
                    svalue = new char[20];
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
            char index[8];
            sprintf(index, "%d", j);  
            add_assoc_zval(result, index, arr);    
            pUnPacker->Next();
        }
        return result;
    }
}
