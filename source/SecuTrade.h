#ifndef T2SDK_SECUTRADE_H
#define T2SDK_SECUTRADE_H
#include "Common/tool.h"

class SecuRequestMode;
// 自定义类CCallback，通过继承（实现）CCallbackInterface，来自定义各种事件（包括连接成功、
// 连接断开、发送完成、收到数据等）发生时的回调方法
class CTradeCallback : public CCallbackInterface
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
public:
	void SetRequestMode(SecuRequestMode* lpMode);
	//331100 登入
	void OnResponse_331100(IF2UnPacker *lpUnPacker);
	
private:
	SecuRequestMode* lpReqMode;
};

class SecuRequestMode
{
public:
	SecuRequestMode();
	~SecuRequestMode()
	{
		lpConnection->Release();
		lpConfig->Release();
	};

	int InitConn(char *ini_file, char *fund_account, char *password, char *&error);
	unsigned long Release();
public:
	string m_opUserToken;
	int m_BranchNo;
	char m_client_id[18];
	int iSystemNo;
	int m_op_branch_no;

	int Login(IF2UnPacker * &lpUnPacker);

	int SendRequest(IBizMessage* lpBizMessage, int iSystemNo);
	//331100 登入
	int ReqFunction331100();
	//400 证券行情查询
	int ReqFunction400(char* exchange_type, char* stock_code);
	//330300 证券代码信息查询
	int ReqFunction330300();
	//333000 证券代码输入确认
	int ReqFunction333000(char* stock_code);
	//333001 大约可买获取
	int ReqFunction333001(char* exchange_type, char* stock_code,double entrust_price);
	//333002 普通委托 
	int ReqFunction333002(char* exchange_type,char* stock_code, double entrust_amount,double entrust_price,char entrust_bs);
	
	char* GetAccountName()
	{
		return m_AccountName;
	}
	char* GetPassword()
	{
		return m_Password;
	}
	char GetEntrustWay()
	{
		return m_EntrustWay;
	}
	string GetFuturesAccount()
	{
		return m_FuturesAccount;
	}
	string GetOpStation()
	{
		return m_opStation;
	}

private:
	CConfigInterface* lpConfig;
	CConnectionInterface *lpConnection;
	CTradeCallback callback;

	char m_AccountName[12];
	char m_Password[8];
	char m_EntrustWay;
	string m_FuturesAccount;
	string m_opStation;
};

#endif /* T2SDK_SECUTRADE_H */