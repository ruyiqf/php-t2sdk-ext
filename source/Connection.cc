#include "connection.h"
#include <Include/t2sdk_interface.h>
#include "CCallback.h"
#include <dlfcn.h> 

// 全局连接对象
CConnectionInterface *g_Connection = NULL;

Connection::Connection(char *domain, char* lib_t2sdk_file, char *license_file, char *send_queue_size, char *auto_reconnect)
{
    this->domain = domain;
    this->lib_t2sdk_file = lib_t2sdk_file;
    this->license_file = license_file;
    this->send_queue_size = send_queue_size;
    this->auto_reconnect = auto_reconnect;
}

void Connection::connect()
{
    //CConnectionInterface *g_Connection = NULL;

    void *handle = dlopen(this->lib_t2sdk_file, RTLD_LAZY | RTLD_GLOBAL);
    char *error;
    if (!handle)  
    {  
       error = dlerror();
       puts(error);
    }  

    typedef CConfigInterface* (*config)(); 
    typedef CConnectionInterface* (*connection)(CConfigInterface*); 
    typedef IF2Packer* (*packer)(int); 
    config NewConfig = (config) dlsym(handle, "NewConfig");
    connection NewConnection = (connection) dlsym(handle, "NewConnection");
    packer NewPacker = (packer) dlsym(handle, "NewPacker");
  
    CConfigInterface* lpConfig = NewConfig(); 
    error = dlerror();
    if(error)
    {
        dlclose(handle);
        puts(error);
    }

    lpConfig->AddRef();

    // [t2sdk] servers指定需要连接的IP地址及端口
    lpConfig->SetString("t2sdk", "servers", this->domain);

    // [t2sdk] license_file指定许可证文件
    lpConfig->SetString("t2sdk", "license_file", this->license_file);

    // [t2sdk] send_queue_size指定T2_SDK的发送队列大小
    lpConfig->SetString("t2sdk", "send_queue_size", this->send_queue_size);
    //在此设置一下就可以支持自动重连
    lpConfig->SetString("t2sdk", "auto_reconnect", this->auto_reconnect);

    g_Connection = NewConnection(lpConfig);

    g_Connection->AddRef();

    // 创建自定义类CCallback的对象（在初始化连接对象时需传递此对象，请看下面代码）
    CCallback callback;

    int ret = 0;

    // 初始化连接对象，返回0表示初始化成功，注意此时并没开始连接服务器
    if (0 == (ret = g_Connection->Create(&callback)))
    {
        // 正式开始连接注册，参数1000为超时参数，单位是ms
        if (ret = g_Connection->Connect(1000))
        {
            // 若连接/注册失败，打印失败原因
            puts(g_Connection->GetErrorMsg(ret));
        }
        else
        {
            // 演示如何使用打包器
            IF2Packer *lpPacker = NewPacker(2);

            lpPacker->AddRef();
            lpPacker->BeginPack();
            lpPacker->AddField("Field");
            lpPacker->AddStr("Data");

            lpPacker->EndPack();

            // 异步模式收发包，第三个参数填1
            g_Connection->SendBiz(100, lpPacker, 1);

            // 释放打包器，不释放会引起内存泄露，后果严重
            lpPacker->FreeMem(lpPacker->GetPackBuf());
            lpPacker->Release();
        }
    }
    else
    {
        puts(g_Connection->GetErrorMsg(ret));
    }

    // 通过getchar阻塞线程，等待服务端应答包到达
    // 演示断开重连时，可在此时关闭服务器，然后再恢复
    getchar();

    // 释放资源
    g_Connection->Release();
    lpConfig->Release();
}