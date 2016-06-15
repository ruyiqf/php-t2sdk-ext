#include "SecuTrade.h"
#include "SecuMD.h"
int TestSecuTrade()
{ 
	char exchange_type[4];
	char stock_code[16];
	double entrust_price=0.0;
	memset(exchange_type,0,sizeof(exchange_type));
	memset(stock_code,0,sizeof(stock_code));
	double entrust_amount=0.0;
	char entrust_bs='\0';
   int chose=0;
   SecuRequestMode* lp_SecuRequestMode = new SecuRequestMode();
   //lp_SecuRequestMode->InitConn("demo", "license.dat", "218.108.19.190:18002");
   lp_SecuRequestMode->InitConn();
   //lp_SecuRequestMode->InitConn("demo", "license.dat", "10.139.103.211:9339");
   cerr<<"------------------------------------------------"<<endl;
   cerr<<" [1]  331100 请求登录                           "<<endl;
   cerr<<" [2]  400 证券行情查询                          "<<endl;
   cerr<<" [3]  330300 获取证券代码信息                   "<<endl;
   cerr<<" [4]  333000 证券代码输入确认                   "<<endl;
   cerr<<" [5]  333001 大约可买获取                       "<<endl;
   cerr<<" [6]  333002 证券普通委托                       "<<endl;
   cerr<<" [0]  EXIT                                      "<<endl;
   cerr<<"------------------------------------------------"<<endl;
   while(1)
   {
   cin>>chose;
   switch(chose)
   {
   case 1:
	   // lp_SecuRequestMode->ReqFunction331100("100100003","123456",'7');
	   //lp_SecuRequestMode->ReqFunction331100("70001558","111111",'7');
	  lp_SecuRequestMode->ReqFunction331100();
		//lp_SecuRequestMode->ReqFunction331100("70960060","111111",'7');
		break;
   case 2:
	   cout<<"---------证券行情查询----------"<<endl;
	   cout<<"Enter exchange_type:";cin>>exchange_type;
	   cout<<"Enter stock_code:";cin>>stock_code;
	    lp_SecuRequestMode->ReqFunction400(exchange_type,stock_code);
		break;
   case 3:
	    lp_SecuRequestMode->ReqFunction330300();
		break;
   case 4:
	   cout<<"---------证券代码输入确认----------"<<endl;
	   cout<<"Enter stock_code:";cin>>stock_code;
	   lp_SecuRequestMode->ReqFunction333000(stock_code);
	   break;
   case 5:
	   cout<<"---------大约可买获取----------"<<endl;
	   cout<<"Enter exchange_type:";cin>>exchange_type;
	   cout<<"Enter stock_code:"; cin>>stock_code;
	   cout<<"Enter entrust_price:";cin>>entrust_price;
	   lp_SecuRequestMode->ReqFunction333001(exchange_type,stock_code,entrust_price);
	   break;
   case 6:
	   cout<<"---------证券普通委托----------"<<endl;
	   cout<<"exchange_type:"; cin>>exchange_type;
	   cout<<"stock_code:"; cin>>stock_code;
	   cout<<"entrust_amount:"; cin>>entrust_amount;
	   cout<<"entrust_price:";  cin>>entrust_price;
	   cout<<"entrust_bs:";cin>>entrust_bs;
	   lp_SecuRequestMode->ReqFunction333002(exchange_type, stock_code, entrust_amount, entrust_price,entrust_bs);
	   break;
   case 0:
	    exit(0); 
  }
 }
   delete lp_SecuRequestMode;
   return 0;
}

int TestSecutMd()
{   
	char exchange_type[4];
	char stock_code[16];
	double entrust_price=0.0;
	memset(exchange_type,0,sizeof(exchange_type));
	memset(stock_code,0,sizeof(stock_code));
	double entrust_amount=0.0;
	char entrust_bs='\0';
	int chose=0;
   CMdRequestMode* lp_CMdRequestMode = new CMdRequestMode();
   lp_CMdRequestMode->InitConn();
   cerr<<"------------------------------------------------"<<endl;
   cerr<<" [1]  331100 请求登录                           "<<endl;
   cerr<<" [2]  62001_12订阅证券成交回报                  "<<endl;
   cerr<<" [3] 62001_23订阅证券委托回报                   "<<endl;
   cerr<<" [4] 333002 普通委托                            "<<endl;
   cerr<<" [0]  EXIT                                      "<<endl;
   cerr<<"------------------------------------------------"<<endl;
   while(1)
   {
   cin>>chose;
   switch(chose)
   {
   case 1:
	   lp_CMdRequestMode->ReqFunction331100();
   break;
   case 2: 
	   lp_CMdRequestMode->SubFunction12(ISSUE_TYPE_REALTIME_SECU);
	   break;
   case 3:
	   lp_CMdRequestMode->SunFunction23(ISSUE_TYPE_ENTR_BACK);
	   break;
   case 4:
	   cout<<"---------证券普通委托----------"<<endl;
	    lp_CMdRequestMode->ReqFunction333002();
	   break;
   case 0:
	   exit(0);
   }
  }
    delete lp_CMdRequestMode;
   return 0;
}
int main()
{

    cerr<<"-----------------------------------------------"<<endl;
	cerr<<" [1] TestSecuTrade                          "<<endl;
	cerr<<" [2] TestSecuMd                             "<<endl;
	cerr<<" [0] Exit                                  "<<endl;
	cerr<<"-----------------------------------------------"<<endl;
	
	while(1)
	{
		int cmd;  
		cin>>cmd;
		switch(cmd){
			case 1: 
				TestSecuTrade();
				break;
			case 2:
				TestSecutMd();
				break;
			case 0: exit(0);
		}
	}

	//通过getchar阻塞线程，等待服务端应答包到达
    getchar();

	return 0;
}
