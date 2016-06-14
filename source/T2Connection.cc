#include "t2connection.h"
#include <Include/t2sdk_interface.h>
#include "source/CCallback.h"
#include <dlfcn.h> 

// 全局连接对象
CConnectionInterface *g_pConnection = NULL;
CConnectionInterface *g_pConnectionHq = NULL;

CBusiness g_szBusiness;
CBusiness g_szBusinessHq;

config T2NewConfig;
connection T2NewConnection;
packer T2NewPacker;
unpacker T2NewUnPacker;
biz_message T2NewBizMessage;



T2Connection::T2Connection(char* lib_t2sdk_file, char *ini_file)
{
    this->lib_t2sdk_file = lib_t2sdk_file;
    this->ini_file = ini_file;
}

void T2Connection::connect()
{
    void *handle = dlopen(this->lib_t2sdk_file, RTLD_LAZY | RTLD_GLOBAL);
    char *error;
    if (!handle)  
    {  
       error = dlerror();
       puts(error);
    }  
    
    T2NewConfig = (config) dlsym(handle, "NewConfig");
    T2NewConnection = (connection) dlsym(handle, "NewConnection");
    T2NewPacker = (packer) dlsym(handle, "NewPacker");

    //通过T2SDK的引出函数，来获取一个新的CConfig对象
    //此对象在创建连接对象时被传递，用于配置所创建的连接对象的各种属性（比如服务器IP地址、安全模式等）
    //值得注意的是，在向配置对象设置配置信息时，配置信息既可以从ini文件中载入，
    //也可以在程序代码中设定，或者是2者的混合，如果对同一个配置项设不同的值，则以最近一次设置为准
    CConfigInterface * lpConfig = T2NewConfig();

    //通过T2SDK的引出函数NewXXXX返回的对象，需要调用对象的Release方法释放，而不能直接用delete
    //因为t2sdk.dll和调用程序可能是由不同的编译器、编译模式生成，delete可能会导致异常
    //为了适合Delphi等使用（Delphi对接口自动调用AddRef方法），用C/C++开发的代码，需要在NewXXXX之后调用一下AddRef
    //以保证引用计数正确
    lpConfig->AddRef();

    ///从配置文件中加载配置
    lpConfig->Load(this->ini_file);

    ///可以在代码中对lpConfig对像中的具体值进行设置

    //[t2sdk] servers指定需要连接的IP地址及端口，可配置多个，中间以“;”间隔//111.207.179.6:8066  18.8.1.171:8066
    /*
    lpConfig->SetString("t2sdk", "servers", "192.168.51.162:3335");
    
    //[t2sdk] license_file指定许可证文件路径
    lpConfig->SetString("t2sdk", "license_file", "(20130306)tyjrnb-TYJRTYJR-ALL-0001_3rd.dat");
    
    //[t2sdk] lang指定错误信息的语言号（缺省为简体中文2052），1033为英文
    lpConfig->SetString("t2sdk", "lang", "1033");
    
    //[t2sdk] send_queue_size指定T2_SDK的发送队列大小
    lpConfig->SetString("t2sdk", "send_queue_size", "100");
    
    //[t2sdk] send_queue_size指定T2_SDK的发送队列大小
    lpConfig->SetString("t2sdk", "license_pwd", "888888");
    
    //[safe] safe_level指定连接的安全模式，需要和T2通道的安全模式一致，否则连接失败
    lpConfig->SetString("safe", "safe_level", "ssl");
    
    //[safe] 
    lpConfig->SetString("safe", "cert_file", "c20121011.pfx");
    
    //[safe] 
    lpConfig->SetString("safe", "cert_pwd", "111111");
    */

    g_szBusiness.SetConfig(lpConfig);
  //通过T2SDK的引出函数，来获取一个新的CConnection对象指针
    g_pConnection = T2NewConnection(lpConfig);
    
    g_pConnection->AddRef();
    
    // 创建自定义类CCallback的对象（在初始化连接对象时需传递此对象，请看下面代码）
    CCallback szCallback;
    
    int iRet = 0, iSystemNo = 0;
    
    //初始化连接对象，返回0表示初始化成功，注意此时并没开始连接服务器
    if (/*(0 == (iRet = g_pConnection->CreateEx(&szCallback)))*/(0 == (iRet = g_pConnection->Create2BizMsg(&szCallback))))
    {
            // 开启断开重连线程，由于在非断开情况下，该线程处于Wait状态，故对应用性能影响甚微
        //正式开始连接，参数5000为超时参数，单位毫秒
        if (iRet = g_pConnection->Connect(5000))
        {
                printf("连接服务器失败, 错误号: %d, 原因: %s!\r\n", iRet, g_pConnection->GetErrorMsg(iRet));
        }   
        else
        {
            printf("连接服务器成功!\r\n");
            ////////////////////////////证券测试//////////////////////////////////////
            //Subscribe(12, 8113, "811300000008");      //发送订阅请求:成交主推 12
            iSystemNo = g_szBusiness.Login();
            if(iSystemNo >= 0)
            {
                int chose = 0;
                printf("------------------------------欢迎使用demo------------------------------\n");
                //printf("1.订阅-证券成交回报. 2.证券委托 3.证券委托查询 4.委托撤单 5.成交查询 6.持仓查询 7.资金查询 0.退出该系统!\n请输入你的操作:");
                printf("1.证券委托 2.持仓查询 3.资金查询 4:证券股东信息查询 5.委托订阅 0.退出该系统!\n请输入你的操作:");
                while(scanf("%d",&chose),0 != chose)
                {
                    getchar();
                    switch (chose)
                    {

                        case 1: g_szBusiness.SecuEntrust(iSystemNo);
                                printf("请按任意键显示主菜单!:\n");
                            break;
                        case 2: g_szBusiness.SecuStockQry(iSystemNo);
                                printf("请按任意键显示主菜单!:\n");
                            break;
                        case 3: g_szBusiness.ClientFundAllQry(iSystemNo);
                                printf("请按任意键显示主菜单!:\n");
                            break;
                        case 4: g_szBusiness.ClientStkacctQry(iSystemNo);
                                printf("请按任意键显示主菜单!:\n");
                            break;
                        case 5:g_szBusiness.funcSubscribePush(620001, 23);
                                printf("请按任意键显示主菜单!:\n");
                            break;
                        default:printf("您输入的操作不正确,请输入正确的操作!\n");
                                printf("请按任意键显示主菜单!:\n");
                            break;
                    }
                    getchar();
                    //printf("1.订阅-证券成交回报. 2.证券委托 3.证券委托查询 4.委托撤单 5.成交查询 6.持仓查询 7.资金查询  8:证券股东信息查询 0.退出该系统!\n请输入你的操作(目前实现2，6，7，8):");
                    printf("1.证券委托 2.持仓查询 3.资金查询 4:证券股东信息查询 5.委托订阅 0.退出该系统!\n请输入你的操作:");

                }
            } 
            else
                printf("取得系统节点号失败, 返回结果: %d!\r\n", iSystemNo);
        }
    }
    else
    {
        if(0 != iRet)
          puts(g_pConnection->GetErrorMsg(iRet));
    }
    
    //通过getchar阻塞线程，等待服务端应答包到达
    printf("按任意键出退!\r\n");
    getchar();
    
    g_pConnection->Close();
    g_pConnection->Release();
    lpConfig->Release();
}

// void T2Connection::connect()
// {
//     //CConnectionInterface *g_Connection = NULL;

//     void *handle = dlopen(this->lib_t2sdk_file, RTLD_LAZY | RTLD_GLOBAL);
//     char *error;
//     if (!handle)  
//     {  
//        error = dlerror();
//        puts(error);
//     }  

//     typedef CConfigInterface* (*config)(); 
//     typedef CConnectionInterface* (*connection)(CConfigInterface*); 
//     typedef IF2Packer* (*packer)(int); 
//     config NewConfig = (config) dlsym(handle, "NewConfig");
//     connection NewConnection = (connection) dlsym(handle, "NewConnection");
//     packer NewPacker = (packer) dlsym(handle, "NewPacker");
  
//     CConfigInterface* lpConfig = NewConfig(); 
//     error = dlerror();
//     if(error)
//     {
//         dlclose(handle);
//         puts(error);
//     }

//     lpConfig->AddRef();

//     // [t2sdk] servers指定需要连接的IP地址及端口
//     lpConfig->SetString("t2sdk", "servers", this->domain);

//     // [t2sdk] license_file指定许可证文件
//     lpConfig->SetString("t2sdk", "license_file", this->license_file);

//     // [t2sdk] send_queue_size指定T2_SDK的发送队列大小
//     lpConfig->SetString("t2sdk", "send_queue_size", this->send_queue_size);
//     //在此设置一下就可以支持自动重连
//     lpConfig->SetString("t2sdk", "auto_reconnect", this->auto_reconnect);

//     g_Connection = NewConnection(lpConfig);

//     g_Connection->AddRef();

//     // 创建自定义类CCallback的对象（在初始化连接对象时需传递此对象，请看下面代码）
//     CCallback callback;

//     int ret = 0;

//     // 初始化连接对象，返回0表示初始化成功，注意此时并没开始连接服务器
//     if (0 == (ret = g_Connection->Create(&callback)))
//     {
//         // 正式开始连接注册，参数1000为超时参数，单位是ms
//         if (ret = g_Connection->Connect(1000))
//         {
//             // 若连接/注册失败，打印失败原因
//             puts(g_Connection->GetErrorMsg(ret));
//         }
//         else
//         {
//             // 演示如何使用打包器
//             IF2Packer *lpPacker = NewPacker(2);

//             lpPacker->AddRef();
//             lpPacker->BeginPack();
//             lpPacker->AddField("Field");
//             lpPacker->AddStr("Data");

//             lpPacker->EndPack();

//             // 异步模式收发包，第三个参数填1
//             g_Connection->SendBiz(100, lpPacker, 1);

//             // 释放打包器，不释放会引起内存泄露，后果严重
//             lpPacker->FreeMem(lpPacker->GetPackBuf());
//             lpPacker->Release();
//         }
//     }
//     else
//     {
//         puts(g_Connection->GetErrorMsg(ret));
//     }

//     // 通过getchar阻塞线程，等待服务端应答包到达
//     // 演示断开重连时，可在此时关闭服务器，然后再恢复
//     getchar();

//     // 释放资源
//     g_Connection->Release();
//     lpConfig->Release();
// }