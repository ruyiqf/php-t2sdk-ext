#include "tool.h"

class COptionRequestMode;
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
	void SetRequestMode(COptionRequestMode* lpMode);
	//331100 登入
	void OnResponse_331100(IF2UnPacker *lpUnPacker);
	
private:
	COptionRequestMode* lpReqMode;
};

class COptionRequestMode
{
public:
	COptionRequestMode()
	{
		lpConfig = NULL;
		lpConnection = NULL;
		callback.SetRequestMode(this);

		lpConfig = NewConfig();
		lpConfig->AddRef();
	};

	~COptionRequestMode()
	{
		lpConnection->Release();
		lpConfig->Release();
	};

	int InitConn(const char* clientName, const char* licFile, const char* serverAddr);
	unsigned long Release();
public:
	string m_opUserToken;
	int m_BranchNo;
	char m_client_id[18];
	int m_op_branch_no;
	int l_entrust_reference;

	//331100 登入
	int ReqFunction331100(const char* AccountName, const char* Password, const char EntrustWay);
	//338000期权代码信息查询 
	int ReqFunction338000();
	//338010期权可交易数量获取
	int ReqFunction338010(char* exchange_type,char* option_code, double opt_entrust_price,char entrust_bs,char entrust_oc);
	//期权代码输入确认 
	int ReqFunction338002(char* exchange_type, char* option_code);
	//338020期权委托查询
	int ReqFunction338020();
	//338021期权成交查询
	int ReqFunction338021();
	//338022期权资产查询
	int ReqFunction338022();
	//338023期权持仓查询
	int ReqFunction338023();
	//338011期权委托 
	int ReqFunciton338011( char* exchange_type,char* option_code,int entrust_amount,double entrust_price,char entrust_bs,char entrust_oc);
	//338012期权撤单
	int ReqFunction338012(char* exchange_type, int entrust_no);
	//339800历史期权委托查询
	int ReqFunction339800(int startDate, int endDate);
	//339801历史期权成交查询
	int ReqFunction339801(int startDate, int endDate);
	//338001期权标的信息查询
	int ReqFunction338001();
	//338024期权行权指派信息查询
	int ReqFunction338024();
	//338025期权行权交割信息查询
	int ReqFunction338025();
	//338026期权行权指派欠资欠券查询
	int ReqFunction338026();
	//338027期权标的证券数量提示
	int ReqFunction338027();
	//338028客户期权持仓合约信息提示
	int ReqFunction338028();
	//338029客户备兑证券不足查询
	int ReqFunction338029();
	//338030备兑证券可划转数量获取
	int ReqFunction338030();
	//338031备兑证券划转
	int ReqFunction338031(char* exchange_type, char* stock_code, int entrust_amount);
	//
private:
	CConfigInterface* lpConfig;
	CConnectionInterface *lpConnection;
	CTradeCallback callback;

	string m_opAccountName;
	string m_opPassword;
	char m_opEntrustWay;
	string m_opFuturesAccount;
	string m_opStation;
	
	int m_SubSystemNo;
};