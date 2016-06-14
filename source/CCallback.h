#ifndef T2SDK_CCALBACK_H
#define T2SDK_CCALBACK_H
#include <Include/t2sdk_interface.h>
#include <stdio.h>
#include "source/t2connection.h"
#include <typeinfo>

//宏
#define SYNCSEND        0   ///同步发送
#define ASYNCSEND       1   ///异步发送
#define USERTOKENLEN    256     ///令牌长度
#define USERSECTION     "user"
#define T2SDK           "t2sdk"
#define ISSUE_TYPE_REALTIME_SECU    12          ///证券成交类型

//////////////////////////////////////////////////////////////
// 全局变量
extern CConnectionInterface *g_pConnection;
extern CConnectionInterface *g_pConnectionHq;

void ShowPacket(int iIssueType, IF2UnPacker *pUnPacker);

///////////////////////////////////////////////////////////////
//CBusiness
class CBusiness
{
public:
    CBusiness();
    virtual ~CBusiness();

    void SetConfig(CConfigInterface *pConfig)
    {
        m_pConfig = pConfig;
        memset(m_cUserToken, 0, USERTOKENLEN);
        m_branch_no = 0;
        memset(m_client_id, 0, USERTOKENLEN);
        memset(m_fund_account, 0, USERTOKENLEN);
    }

    CConfigInterface* getConfig()
    {
        return m_pConfig;
    }

    int DealReceive();
    int Subscribe(int issue_type); ///发布类型
    int CancelSub(int issueType);

    /** 
    * 登录，调用331100功能登录
    * @return >=0 返回系统结点号，其他失败
    */
    int Login();

    /** 
    * 证券委托
    * 先获取pack打包器进行打包，再发送委托，得到应答以后再解包。
    * @param iSystemNo 系统节点号，
    *               对于UF2.0交易系统，需要根据登录功能331100返回的字段sysnode_id来设置iSystemNo。
    * @return 0: 成功, 其他失败!
    */
    int SecuEntrust(int iSystemNo = 2);

    /** 
    * 证券委托查询
    * 先获取pack打包器进行打包，再发送委托，得到应答以后再解包。
    * @param iSystemNo 系统节点号，
    *               对于UF2.0交易系统，需要根据登录功能331100返回的字段sysnode_id来设置iSystemNo。
    * @return 0: 成功, 其他失败!
    */
    //int SearchSecuEntrust(int iSystemNo = 2);

        /** 
    * 证券委托撤单
    * 先获取pack打包器进行打包，再发送委托，得到应答以后再解包。
    * @param iSystemNo 系统节点号，
    *               对于UF2.0交易系统，需要根据登录功能331100返回的字段sysnode_id来设置iSystemNo。
    * @return 0: 成功, 其他失败!
    */
    //int SecuEntrustWithdraw(int iSystemNo = 2);

    /** 
    * 证券成交查询
    * 先获取pack打包器进行打包，再发送委托，得到应答以后再解包。
    * @param iSystemNo 系统节点号，
    *               对于UF2.0交易系统，需要根据登录功能331100返回的字段sysnode_id来设置iSystemNo。
    * @return 0: 成功, 其他失败!
    */

    //int SecuRealtimeQry(int iSystemNo  = 2 );


    int SecuStockQry(int iSystemNo  = 2 );

    int ClientFundAllQry(int iSystemNo = 2 );


    int ClientStkacctQry(int iSystemNo = 2 );

    int funcSubscribePush(int nFUnctionNo, int nIssueType);
    
private:
    CConfigInterface *m_pConfig;
    char m_cUserToken[USERTOKENLEN];
    int m_branch_no;
    char m_client_id[USERTOKENLEN];
    char m_fund_account[USERTOKENLEN];
    
};

// 自定义类CCallback，通过继承（实现）CCallbackInterface，来自定义各种事件（包括连接成功、
// 连接断开、发送完成、收到数据等）发生时的回调方法
// //Callback
class CCallback : public CCallbackInterface
{
public:
    // 因为CCallbackInterface的最终纯虚基类是IKnown，所以需要实现一下这3个方法
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release();
    
    // 各种事件发生时的回调方法，实际使用时可以根据需要来选择实现，对于不需要的事件回调方法，可直接return
    // Reserved?为保留方法，为以后扩展做准备，实现时可直接return或return 0。
    void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData);
    void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d);
    void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d);
    int  FUNCTION_CALL_MODE Reserved3();
    void FUNCTION_CALL_MODE Reserved4();
    void FUNCTION_CALL_MODE Reserved5();
    void FUNCTION_CALL_MODE Reserved6();
    void FUNCTION_CALL_MODE Reserved7();
    void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult);
        void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult);
        void FUNCTION_CALL_MODE OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg);
};
#endif
