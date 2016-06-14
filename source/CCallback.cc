#include <stdio.h>
#include "Callback.h"
////////////////////////////////

//int g_iSystemNo = -1;
//bool g_bLogin = false;
//char g_cUserToken[USERTOKENLEN] = {0};
//打印应答包

void ShowPacket(int iIssueType, IF2UnPacker *pUnPacker)
{
    int i = 0, t = 0, j = 0, k = 0;
    
    for (i = 0; i < pUnPacker->GetDatasetCount(); ++i)
    {
        // 设置当前结果集
        pUnPacker->SetCurrentDatasetByIndex(i);
        
        // 打印字段
        for (t = 0; t < pUnPacker->GetColCount(); ++t)
        {
            printf("%20s", pUnPacker->GetColName(t));
        }
        
        putchar('\n');
        
        // 打印所有记录
        for (j = 0; j < (int)pUnPacker->GetRowCount(); ++j)
        {
            // 打印每条记录
            for (k = 0; k < pUnPacker->GetColCount(); ++k)
            {
                switch (pUnPacker->GetColType(k))
                {
                case 'I':
                    printf("%20d", pUnPacker->GetIntByIndex(k));
                    break;
                    
                case 'C':
                    printf("%20c", pUnPacker->GetCharByIndex(k));
                    break;
                    
                case 'S':
                    printf("%20s", pUnPacker->GetStrByIndex(k));
                    break;
                    
                case 'F':
                    printf("%20f", pUnPacker->GetDoubleByIndex(k));
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
            putchar('\n');      
            pUnPacker->Next();
        }
        putchar('\n');
    }
}

/////////////////////////////////////////////////////////////
//CBusiness 实现
CBusiness::CBusiness()
{
    memset(m_cUserToken, 0, sizeof(m_cUserToken));
}

CBusiness::~CBusiness()
{
    
}

//登录
int CBusiness::Login()
{
    int hSend = 0, iSystemNo = -1;


    IBizMessage* lpBizMessage = NewBizMessage();
    lpBizMessage->AddRef();


    IBizMessage* lpBizMessageRecv = NULL;

    //功能号
    lpBizMessage->SetFunction(331100);
    //请求类型
    lpBizMessage->SetPacketType(REQUEST_PACKET);
    //设置营业部号
    lpBizMessage->SetBranchNo(1);
    //设置company_id
    lpBizMessage->SetCompanyID(91000);
    //设置SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
    
    ///获取版本为2类型的pack打包器
    IF2Packer *pPacker = NewPacker(2);
    if(!pPacker)
    {
        printf("取打包器失败!\r\n");
        return -1;
    }
    pPacker->AddRef();

    ///定义解包器
    //IF2UnPacker *pUnPacker = NULL;

    ///其他的应答信息
    //LPRET_DATA pRetData = NULL;
    ///开始打包
    pPacker->BeginPack();
    
    ///加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);//操作分支机构
    pPacker->AddField("op_entrust_way", 'C', 1);//委托方式 
    pPacker->AddField("op_station", 'S', 255);//站点地址
    pPacker->AddField("branch_no", 'I', 5);     
    pPacker->AddField("input_content", 'C'); 
    pPacker->AddField("account_content", 'S', 30);
    pPacker->AddField("content_type", 'S', 6);  
    pPacker->AddField("password", 'S', 10);      
    pPacker->AddField("password_type", 'C');   
   
    ///加入对应的字段值
    pPacker->AddInt(0);         //op_branch_no          
    pPacker->AddChar('7');      //  op_entrust_way  
    pPacker->AddStr("hs");  //      op_station  
    pPacker->AddInt(1);     //  branch_no
    pPacker->AddChar('1');      //input_content         
    pPacker->AddStr("70001172");    //account_content       
    pPacker->AddStr("0");   //content_type
    pPacker->AddStr("111111");      //password  
    pPacker->AddChar('1');  //password_type
    
    ///结束打包
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    
    ///同步发送登录请求，应答在RecvBizEx中处理。
    /*(hSend = g_pConnection->SendBizEx(331100,pPacker,NULL,SYNCSEND,0,0,1,NULL)*/
    if((hSend = g_pConnection->SendBizMsg(lpBizMessage,0)) < 0)
    {
        printf("发送功能331100失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        goto EXIT;
    }

    printf("发送功能331100成功, 返回接收句柄: %d!\r\n", hSend);
    


    //iRet = g_pConnection->RecvBizEx(hSend, (void **)&pUnPacker, &pRetData, 1000);
    //int a;  int &ra=a;  //定义引用ra,它是变量a的引用，即别名

    hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,1000);
    if(hSend != 0)
    {
        printf("接收功能331100失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        goto EXIT;
    }else{

        int iReturnCode = lpBizMessageRecv->GetReturnCode();
        if(iReturnCode!=0) //错误
        {
            printf("接收功能331100失败,errorNo:%d,errorInfo:%s\n",lpBizMessageRecv->GetErrorNo(),lpBizMessageRecv->GetErrorInfo());
                            
        }
        else if(iReturnCode==0) // 正确
        {
                        //如果要把消息放到其他线程处理，必须自行拷贝，操作如下：
                        //int iMsgLen = 0;
                        //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
                        //将lpMsgBuffer拷贝走，然后在其他线程中恢复成消息可进行如下操作：
                        //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
                        //没有错误信息
            puts("业务操作成功");
            int iLen = 0;
            //接收包体       "包头|功能码|签名|包体";   例如“12|331100|XyIxtt..|username=sundsun,password=123456,”;
            const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
            //
            IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
            //设置系统号
            iSystemNo = lpUnPacker->GetInt("sysnode_id");
            printf("分支机构号："+iSystemNo);
            //分支机构
            m_branch_no = lpUnPacker->GetInt("branch_no");
            const char *pUserToken = lpUnPacker->GetStr("user_token");
            if(pUserToken){
                strcpy(m_cUserToken, pUserToken);
                //printf("\r\nuser_token[%s]\r\n",m_cUserToken);
            }
            //客户编号
            pUserToken = lpUnPacker->GetStr("client_id");
            if(pUserToken){
                strcpy(m_client_id, pUserToken);
                //printf("\r\nclient_id[%s]\r\n",m_client_id);
            }
            //资产账户
            pUserToken = lpUnPacker->GetStr("fund_account");
            if(pUserToken){
                strcpy(m_fund_account, pUserToken);
                //printf("\r\nfund_account[%s]\r\n",m_fund_account);
            }
            ShowPacket(0,lpUnPacker);
        }
    }

EXIT:
    ///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
    if(pPacker)
    {
        pPacker->FreeMem(pPacker->GetPackBuf());
        ///释放申请的pack
        pPacker->Release();
    }
    if(lpBizMessage){
        lpBizMessage->Release();
    }
    return iSystemNo;
}

//证券委托 333002
int CBusiness::SecuEntrust(int iSystemNo)
{

        int hSend = 0;
    
        IBizMessage* lpBizMessage = NewBizMessage();
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
        lpBizMessage->SetSystemNo(iSystemNo);
    
        
        
        ///获取版本为2类型的pack打包器
        IF2Packer *pPacker = NewPacker(2);
        if(!pPacker)
        {
            printf("取打包器失败!\r\n");
            return -1;
        }
        pPacker->AddRef();
        
        ///开始打包
        pPacker->BeginPack();
        
        ///加入字段名
        pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
        pPacker->AddField("op_entrust_way", 'C', 1);
        pPacker->AddField("op_station", 'S', 255);
        pPacker->AddField("branch_no", 'I', 5); 
        pPacker->AddField("client_id", 'S', 18);
        pPacker->AddField("fund_account", 'S', 18);
        pPacker->AddField("password", 'S', 50);
        pPacker->AddField("password_type", 'C', 1); 
        pPacker->AddField("user_token", 'S', 512);
        pPacker->AddField("exchange_type", 'S', 4);
        pPacker->AddField("stock_account", 'S', 11);
        pPacker->AddField("stock_code", 'S', 16);
        pPacker->AddField("entrust_amount", 'F', 19, 2);
        pPacker->AddField("entrust_price", 'F', 18, 3);
        pPacker->AddField("entrust_bs", 'C', 1);
        pPacker->AddField("entrust_prop", 'S', 3);  
        pPacker->AddField("batch_no", 'I', 8);
        
        
        ///加入对应的字段值
        pPacker->AddInt(0);     //op_branch_no              
        pPacker->AddChar('7');      //op_entrust_way            
        pPacker->AddStr("1");   //  op_station          
        pPacker->AddInt(m_branch_no);//branch_no
        pPacker->AddStr(m_client_id);   //client_id     
        pPacker->AddStr(m_fund_account);    //fund_account      
        pPacker->AddStr("111111");      //password      
        pPacker->AddChar('2');          //password_type     
        pPacker->AddStr(m_cUserToken); //user_token
        pPacker->AddStr("1");       //exchange_type         
        pPacker->AddStr("A070000172");  //stock_account     
        pPacker->AddStr("600570");      //stock_code        
        pPacker->AddDouble(100);        //entrust_amount        
        pPacker->AddDouble(39.07);  //entrust_price     
        pPacker->AddChar('1');      //  entrust_bs      
        pPacker->AddStr("0");       //  entrust_prop        
        pPacker->AddInt(13);//batch_no
        

    
        ///结束打包
        pPacker->EndPack();
    
        lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    
    
        
        ///同步发送资金查询请求，应答在RecvBizEx中处理。
        if(/*(hSend = g_pConnection->SendBizEx(332255,pPacker,NULL,SYNCSEND,iSystemNo,0,1,NULL)) < 0*/(hSend = g_pConnection->SendBizMsg(lpBizMessage,0)) < 0)
        {
            printf("发送功能333002失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
            hSend=-2;
            goto EXIT;
        }
        
        printf("发送功能333002成功, 返回接收句柄: %d!\r\n", hSend);
        
        //iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
        hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,1000);
        if(hSend != 0)
        {
            printf("接收功能333002失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
            goto EXIT;
        }else{
            int iReturnCode = lpBizMessageRecv->GetReturnCode();
            if(iReturnCode!= 0) //错误
            {
                printf("接收功能333002失败,errorNo:%d,errorInfo:%s\n",lpBizMessageRecv->GetReturnCode(),lpBizMessageRecv->GetErrorInfo());            
            }
            else if(iReturnCode==0) // 正确
            {
                            //如果要把消息放到其他线程处理，必须自行拷贝，操作如下：
                            //int iMsgLen = 0;
                            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
                            //将lpMsgBuffer拷贝走，然后在其他线程中恢复成消息可进行如下操作：
                            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
                            //没有错误信息
                puts("业务操作成功");
                int iLen = 0;
                const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
                IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
                ShowPacket(0,lpUnPacker);
            }
    
        }
        
    EXIT:
        ///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
        if(pPacker)
        {
            pPacker->FreeMem(pPacker->GetPackBuf());
            ///释放申请的pack
            pPacker->Release();
        }
        if(lpBizMessage){
            lpBizMessage->Release();
        }
        
        return hSend;
}









//证券股东信息查询 331300
int CBusiness::ClientStkacctQry(int iSystemNo)
{

        int hSend = 0;
    
        IBizMessage* lpBizMessage = NewBizMessage();
        lpBizMessage->AddRef();
        
        
        IBizMessage* lpBizMessageRecv = NULL;
        
        //功能号
        lpBizMessage->SetFunction(331300);
        //请求类型
        lpBizMessage->SetPacketType(REQUEST_PACKET);
        //设置营业部号
        lpBizMessage->SetBranchNo(1);
         lpBizMessage->SetCompanyID(91000);
    //设置SenderCompanyID
       lpBizMessage->SetSenderCompanyID(91000);
        //设置系统号
        lpBizMessage->SetSystemNo(iSystemNo);
    
        
        
        ///获取版本为2类型的pack打包器
        IF2Packer *pPacker = NewPacker(2);
        if(!pPacker)
        {
            printf("取打包器失败!\r\n");
            return -1;
        }
        pPacker->AddRef();
        
        ///开始打包
        pPacker->BeginPack();
        
        ///加入字段名
        pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
        pPacker->AddField("op_entrust_way", 'C', 1);
        pPacker->AddField("op_station", 'S', 255);
        pPacker->AddField("branch_no", 'I', 5); 
        pPacker->AddField("client_id", 'S', 18);
        pPacker->AddField("fund_account", 'S', 18);
        pPacker->AddField("password", 'S', 10);
        pPacker->AddField("password_type", 'C', 1); 
        pPacker->AddField("user_token", 'S', 512);
        pPacker->AddField("exchange_type", 'S', 4);

    
        
        ///加入对应的字段值
        pPacker->AddInt(0);         //op_branch_no          
        pPacker->AddChar('7');          //  op_entrust_way  
        pPacker->AddStr("hs");  //op_station                
        pPacker->AddInt(m_branch_no);       //  branch_no   
        pPacker->AddStr(m_client_id);   //      client_id
        pPacker->AddStr(m_fund_account);    //  fund_account    
        pPacker->AddStr("111111");      //password      
        pPacker->AddChar('2');          //  password_type   
        pPacker->AddStr(m_cUserToken);  //  user_token  
        pPacker->AddStr("1");           //exchange_type     


    
        ///结束打包
        pPacker->EndPack();
    
        lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    
    
        
        ///同步发送资金查询请求，应答在RecvBizEx中处理。
        if(/*(hSend = g_pConnection->SendBizEx(332255,pPacker,NULL,SYNCSEND,iSystemNo,0,1,NULL)) < 0*/(hSend = g_pConnection->SendBizMsg(lpBizMessage,0)) < 0)
        {
            printf("发送功能331300失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
            hSend=-2;
            goto EXIT;
        }
        
        printf("发送功能333002成功, 返回接收句柄: %d!\r\n", hSend);
        
        //iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
        hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,1000);
        if(hSend != 0)
        {
            printf("接收功能331300失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
            goto EXIT;
        }else{
            int iReturnCode = lpBizMessageRecv->GetReturnCode();
            if(iReturnCode!= 0) //错误
            {
                printf("接收功能331300失败,errorNo:%d,errorInfo:%s\n",lpBizMessageRecv->GetErrorNo(),lpBizMessageRecv->GetErrorInfo());           
            }
            else if(iReturnCode==0) // 正确
            {
                            //如果要把消息放到其他线程处理，必须自行拷贝，操作如下：
                            //int iMsgLen = 0;
                            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
                            //将lpMsgBuffer拷贝走，然后在其他线程中恢复成消息可进行如下操作：
                            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
                            //没有错误信息
                puts("业务操作成功");
                int iLen = 0;
                const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
                IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
                ShowPacket(0,lpUnPacker);
            }
    
        }
        
    EXIT:
        ///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
        if(pPacker)
        {
            pPacker->FreeMem(pPacker->GetPackBuf());
            ///释放申请的pack
            pPacker->Release();
        }
        if(lpBizMessage){
            lpBizMessage->Release();
        }
        
        return hSend;


}


//持仓查询 333104
int CBusiness::SecuStockQry(int iSystemNo /* = 2 */)
{

        printf("--------------------------->m_cUserToken[%s]\n",m_cUserToken);
        int hSend = 0;
    
        IBizMessage* lpBizMessage = NewBizMessage();
        lpBizMessage->AddRef();
        
        
        IBizMessage* lpBizMessageRecv = NULL;
        
        //功能号
        lpBizMessage->SetFunction(333104);
        //请求类型
        lpBizMessage->SetPacketType(REQUEST_PACKET);
        //设置营业部号
        lpBizMessage->SetBranchNo(1);
         lpBizMessage->SetCompanyID(91000);
    //设置SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
        //设置系统号
        lpBizMessage->SetSystemNo(iSystemNo);
    
        
        
        ///获取版本为2类型的pack打包器
        IF2Packer *pPacker = T2NewPacker(2);
        if(!pPacker)
        {
            printf("取打包器失败!\r\n");
            return -1;
        }
        pPacker->AddRef();
        
        ///开始打包
        pPacker->BeginPack();
        
        ///加入字段名
        pPacker->AddField("op_branch_no", 'I', 5);
        pPacker->AddField("op_entrust_way", 'C', 1);
        pPacker->AddField("op_station", 'S', 255);
        pPacker->AddField("branch_no", 'I', 5);         
        pPacker->AddField("client_id", 'S', 18);            
        pPacker->AddField("fund_account", 'S', 18);     
        pPacker->AddField("password", 'S', 10);                     
        pPacker->AddField("password_type", 'C', 1);     
        pPacker->AddField("user_token", 'S', 512);      
        
        ///加入对应的字段值
        pPacker->AddInt(0);             //  op_branch_no    
        pPacker->AddChar('7');          //      op_entrust_way
        pPacker->AddStr("hs");      //op_station
        pPacker->AddInt(m_branch_no);   //  branch_no       
        pPacker->AddStr(m_client_id);   //      client_id
        pPacker->AddStr(m_fund_account);    //  fund_account    
        pPacker->AddStr("111111");      //  password    
        pPacker->AddChar('1');              //  password_type
        pPacker->AddStr(m_cUserToken);  //user_token

    
        ///结束打包
        pPacker->EndPack();
    
        lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    
    
        
        ///同步发送资金查询请求，应答在RecvBizEx中处理。
        if(/*(hSend = g_pConnection->SendBizEx(332255,pPacker,NULL,SYNCSEND,iSystemNo,0,1,NULL)) < 0*/(hSend = g_pConnection->SendBizMsg(lpBizMessage,0)) < 0)
        {
            printf("发送功能333104失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
            hSend=-2;
            goto EXIT;
        }
        
        printf("发送功能333104成功, 返回接收句柄: %d!\r\n", hSend);
        
        //iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
        hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,1000);
        if(hSend != 0)
        {
            printf("接收功能333104失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
            goto EXIT;
        }else{
            int iReturnCode = lpBizMessageRecv->GetReturnCode();
            if(iReturnCode!= 0) //错误
            {
                printf("接收功能333104失败,errorNo:%d,errorInfo:%s\n",lpBizMessageRecv->GetErrorNo(),lpBizMessageRecv->GetErrorInfo());           
            }
            else if(iReturnCode==0) // 正确
            {
                            //如果要把消息放到其他线程处理，必须自行拷贝，操作如下：
                            //int iMsgLen = 0;
                            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
                            //将lpMsgBuffer拷贝走，然后在其他线程中恢复成消息可进行如下操作：
                            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
                            //没有错误信息
                puts("业务操作成功");
                int iLen = 0;
                const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
                IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
                ShowPacket(0,lpUnPacker);
            }
    
        }
        
    EXIT:
        ///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
        if(pPacker)
        {
            pPacker->FreeMem(pPacker->GetPackBuf());
            ///释放申请的pack
            pPacker->Release();
        }
        if(lpBizMessage){
            lpBizMessage->Release();
        }
        
        return hSend;
        
}

//订阅
int CBusiness::funcSubscribePush(int nFUnctionNo, int nIssueType){
        printf("--------------------------->m_cUserToken[%s]\n",m_cUserToken);
        int hSend = 0;
    
        IBizMessage* lpBizMessage = NewBizMessage();
        lpBizMessage->AddRef();
        
        
        IBizMessage* lpBizMessageRecv = NULL;
        
    
        //功能号
        lpBizMessage->SetFunction(nFUnctionNo);
        //请求类型
        lpBizMessage->SetPacketType(REQUEST_PACKET);

         lpBizMessage->SetCompanyID(91000);
        //设置SenderCompanyID
        lpBizMessage->SetSenderCompanyID(91000);
        //设置消息类型
        lpBizMessage->SetIssueType(nIssueType);
    
        ///获取版本为2类型的pack打包器
        IF2Packer *pPacker = NewPacker(2);
        if(!pPacker)
        {
            printf("取打包器失败!\r\n");
            return -1;
        }
        pPacker->AddRef();
    
        ///开始打包
        pPacker->BeginPack();
    
        ///加入字段名
/*      pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
        pPacker->AddField("op_entrust_way", 'C', 1);
        pPacker->AddField("op_station", 'S', 255);
        pPacker->AddField("password", 'S', 50);
        pPacker->AddField("client_id", 'S', 18);
        pPacker->AddField("user_token", 'S', 512);


        pPacker->AddField("fund_account", 'S', 255);
        pPacker->AddField("branch_no", 'I', 5);
        pPacker->AddField("issue_type", 'I', 5);
*/
    /*  pPacker->AddField("op_branch_no");//名字 类型 长度
        pPacker->AddField("op_entrust_way");
        pPacker->AddField("op_station");
        pPacker->AddField("password");
        pPacker->AddField("client_id");
        pPacker->AddField("user_token");*/


        pPacker->AddField("fund_account", 'S', 255);
        pPacker->AddField("branch_no", 'I', 5);
    //  pPacker->AddField("issue_type", 'I', 5);
        

        ///加入对应的字段值
    /*  pPacker->AddStr("");        //op_branch_no              
        pPacker->AddChar('7');      //op_entrust_way            
        pPacker->AddStr("station1");    //  op_station  
        pPacker->AddStr("111111");      //password
        pPacker->AddStr("");
        pPacker->AddStr(""); //user_token
*/

        
        pPacker->AddStr("70000172");            //  fund_account    
        pPacker->AddInt(m_branch_no);               //  branch_no   
        //pPacker->AddInt(nIssueType); //issue_type

        ///结束打包
        pPacker->EndPack();

    
        //lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpBizMessage->SetKeyInfo(pPacker->GetPackBuf(),pPacker->GetPackLen());
    
        
        ///同步发送资金查询请求，应答在RecvBizEx中处理。
        if(/*(hSend = g_pConnection->SendBizEx(332255,pPacker,NULL,SYNCSEND,iSystemNo,0,1,NULL)) < 0*/(hSend = g_pConnection->SendBizMsg(lpBizMessage,1)) < 0)
        {
            printf("发送功能%d 消息类型%d 失败, 错误号: %d, 原因: %s!\r\n", nFUnctionNo, nIssueType, hSend, g_pConnection->GetErrorMsg(hSend));
            hSend=-2;
            goto EXIT;

        }
        
        printf("发送功能%d 消息类型%d 成功, 返回接收句柄: %d!\r\n", nFUnctionNo, nIssueType, hSend);
        
        //iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
/*      hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,5000);
        if(hSend != 0)
        {
            printf("接收功能%d 消息类型%d 失败, 错误号: %d, 原因: %s!\r\n", nFUnctionNo, nIssueType,hSend, g_pConnection->GetErrorMsg(hSend));
            goto EXIT;
        }else{
            int iReturnCode = lpBizMessageRecv->GetReturnCode();
            if(iReturnCode!= 0) //错误
            {
                printf("接收功能%d 消息类型%d 失败,errorNo:%d,errorInfo:%s\n",nFUnctionNo, nIssueType,lpBizMessageRecv->GetErrorNo(),lpBizMessageRecv->GetErrorInfo());           
            }
            else if(iReturnCode==0) // 正确
            {
                            //如果要把消息放到其他线程处理，必须自行拷贝，操作如下：
                            //int iMsgLen = 0;
                            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
                            //将lpMsgBuffer拷贝走，然后在其他线程中恢复成消息可进行如下操作：
                            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
                            //没有错误信息
                puts("业务操作成功");
                int iLen = 0;
                const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
                IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
                ShowPacket(0,lpUnPacker);
            }
    
        }
    */  
    EXIT:
        ///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
        if(pPacker)
        {
            pPacker->FreeMem(pPacker->GetPackBuf());
            ///释放申请的pack
            pPacker->Release();
        }
        if(lpBizMessage){
            lpBizMessage->Release();
        }
        
        return hSend;


}

// 资金查询 332255 //异步处理方式 
int CBusiness::ClientFundAllQry(int iSystemNo /* = 2 */)
{
    printf("--------------------------->m_cUserToken[%s]\n",m_cUserToken);
    int hSend = 0;

    IBizMessage* lpBizMessage = NewBizMessage();
    lpBizMessage->AddRef();
    
    
    IBizMessage* lpBizMessageRecv = NULL;
    
    //功能号
    lpBizMessage->SetFunction(332255);
    //请求类型
    lpBizMessage->SetPacketType(REQUEST_PACKET);
    //设置营业部号
    lpBizMessage->SetBranchNo(1);
    //lpBizMessage->SetBranchNo(m_branch_no);
     lpBizMessage->SetCompanyID(91000);
    //设置SenderCompanyID
    lpBizMessage->SetSenderCompanyID(91000);
    //设置系统号
    lpBizMessage->SetSystemNo(iSystemNo);

    
    
    ///获取版本为2类型的pack打包器
    IF2Packer *pPacker = NewPacker(2);
    if(!pPacker)
    {
        printf("取打包器失败!\r\n");
        return -1;
    }
    pPacker->AddRef();
    
    ///开始打包
    pPacker->BeginPack();
    
    ///加入字段名
    pPacker->AddField("op_branch_no", 'I', 5);
    pPacker->AddField("op_entrust_way", 'C', 1);
    pPacker->AddField("op_station", 'S', 255);
    pPacker->AddField("branch_no", 'I', 5);         
    pPacker->AddField("client_id", 'S', 18);            
    pPacker->AddField("fund_account", 'S', 18);     
    pPacker->AddField("password", 'S', 10);                     
    pPacker->AddField("password_type", 'C', 1);     
    pPacker->AddField("user_token", 'S', 512);

    ///加入对应的字段值
    pPacker->AddInt(0);             //  op_branch_no    
    pPacker->AddChar('7');          //  op_entrust_way  
    pPacker->AddStr("hs");  //op_station
    pPacker->AddInt(m_branch_no);   //branch_no
    pPacker->AddStr(m_client_id);   //      client_id
    pPacker->AddStr(m_fund_account);    //  fund_account
    pPacker->AddStr("111111");      //  password    
    pPacker->AddChar('1');          //  password_type   
    pPacker->AddStr(m_cUserToken);//user_token

    ///结束打包
    pPacker->EndPack();

    lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());








///同步发送资金查询请求，应答在RecvBizEx中处理。
        if(/*(hSend = g_pConnection->SendBizEx(332255,pPacker,NULL,SYNCSEND,iSystemNo,0,1,NULL)) < 0*/(hSend = g_pConnection->SendBizMsg(lpBizMessage,0)) < 0)
        {
            printf("发送功能332255失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
            hSend=-2;
            goto EXIT;
        }
        
        printf("发送功能332255成功, 返回接收句柄: %d!\r\n", hSend);
        
        //iRet = g_pConnection->RecvBizEx(hSend,(void **)&pUnPacker,&pRetData,1000);
        hSend = g_pConnection->RecvBizMsg(hSend,&lpBizMessageRecv,1000);
        if(hSend != 0)
        {
            printf("接收功能332255失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
            goto EXIT;
        }else{
            int iReturnCode = lpBizMessageRecv->GetReturnCode();
            if(iReturnCode!= 0) //错误
            {
                printf("接收功能332255失败,errorNo:%d,errorInfo:%s\n",lpBizMessageRecv->GetErrorNo(),lpBizMessageRecv->GetErrorInfo());           
            }
            else if(iReturnCode==0) // 正确
            {
                            //如果要把消息放到其他线程处理，必须自行拷贝，操作如下：
                            //int iMsgLen = 0;
                            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
                            //将lpMsgBuffer拷贝走，然后在其他线程中恢复成消息可进行如下操作：
                            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
                            //没有错误信息
                puts("业务操作成功");
                int iLen = 0;
                const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
                IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
                ShowPacket(0,lpUnPacker);
            }
    
        }
        
    EXIT:
        ///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
        if(pPacker)
        {
            pPacker->FreeMem(pPacker->GetPackBuf());
            ///释放申请的pack
            pPacker->Release();
        }
        if(lpBizMessage){
            lpBizMessage->Release();
        }
        
        return hSend;
    
}





    
/*  原始代码
///同步发送资金查询请求，应答在RecvBizEx中处理。
    if((hSend = g_pConnection->SendBizMsg(lpBizMessage,1)) < 0)
    {
        printf("发送功能332255失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
        hSend=-2;
        goto EXIT;
    }
    
    printf("发送功能332255成功, 返回接收句柄: %d!\r\n", hSend);
    
EXIT:
    ///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
    if(pPacker)
    {
        pPacker->FreeMem(pPacker->GetPackBuf());
        ///释放申请的pack
        pPacker->Release();
    }
    if(lpBizMessage){
        lpBizMessage->Release();
    }
    
    return hSend;
}*/



////////////////////////////////////////////////////////////
//Callback 实现
unsigned long CCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    return 0;
}

unsigned long CCallback::AddRef()
{
    return 0;
}

unsigned long CCallback::Release()
{
    return 0;
}

// 以下各回调方法的实现仅仅为演示使用
void CCallback::OnConnect(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnConnect");
}

void CCallback::OnSafeConnect(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnSafeConnect");
}

void CCallback::OnRegister(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnRegister");
}

void CCallback::OnClose(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnClose");;
}

void CCallback::OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
    puts("CCallback::OnSent");
}

void CCallback::Reserved1(void *a, void *b, void *c, void *d)
{
    puts("CCallback::Reserved1");
}

void CCallback::Reserved2(void *a, void *b, void *c, void *d)
{
    puts("CCallback::Reserved2");
}

void CCallback::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
    puts("CCallback::OnReceivedBizEx");
}

void CCallback::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{
    puts("CCallback::OnReceivedBiz");
}

int  CCallback::Reserved3()
{
    puts("CCallback::Reserved3");
    return 0;
}

void CCallback::Reserved4()
{
    puts("CCallback::Reserved4");
}

void CCallback::Reserved5()
{
    puts("CCallback::Reserved5");
}

void CCallback::Reserved6()
{
    puts("CCallback::Reserved6");
}

void CCallback::Reserved7()
{
    puts("CCallback::Reserved7");
}

void CCallback::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{
    puts("CCallback::OnReceivedBizMsg");
     if (lpMsg!=NULL)
     {
         //成功,应用程序不能释放lpBizMessageRecv消息
         int iReturnCode = lpMsg->GetReturnCode();
         if(iReturnCode!= 0) //错误
         {
             printf("接收功能332255失败,errorNo:%d,errorInfo:%s\n",lpMsg->GetErrorNo(),lpMsg->GetErrorInfo());             
         }
         else if(iReturnCode==0) // 正确


         {

              //如果要把消息放到其他线程处理，必须自行拷贝，操作如下：
                         //int iMsgLen = 0;
                         //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
                         //将lpMsgBuffer拷贝走，然后在其他线程中恢复成消息可进行如下操作：
                         //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
                         //没有错误信息
            int Func = lpMsg->GetFunction();
            printf("-----func----[%d]\n",Func);
             if(620000 == Func){
                printf("------------心跳包----------\n");
                lpMsg->ChangeReq2AnsMessage();
                lpConnection->SendBizMsg(lpMsg,1);
                return ;
             }
             //puts("业务操作成功");
             int iLen = 0;
             const void * lpBuffer = lpMsg->GetKeyInfo(iLen);
             IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
            int iIssueType =lpMsg->GetIssueType();
             if(23 == iIssueType && 620003 == Func){
                printf("------------委托回报----------\n");
            }else if(12 == iIssueType && 620003 == Func){
                printf("------------成交回报----------\n");
            }else if(23 == iIssueType && 620001 == Func){
                printf("------------订阅委托回报返回----------\n");
            }else if(12 == iIssueType && 620001 == Func){
                printf("------------订阅成交回报返回----------\n");
            }else{
                printf("------------未知----------func [%d]iIssueType[%d]\n",Func,iIssueType);
            }
             ShowPacket(0,lpUnPacker);

 /*原始
                         //如果要把消息放到其他线程处理，必须自行拷贝，操作如下：
                         //int iMsgLen = 0;
                         //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
                         //将lpMsgBuffer拷贝走，然后在其他线程中恢复成消息可进行如下操作：
                         //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
                         //没有错误信息
             puts("业务操作成功");
             int iLen = 0;
             const void * lpBuffer = lpMsg->GetContent(iLen);
             IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
             ShowPacket(0,lpUnPacker);
            
             if (null != lpUnPacker)
                ShowPacket(0,lpUnPacker);
             else {
                //error happen within NewUnPacker;
                 printf("error happen within NewUnpacker\n");
             }
             */
            
         }

     }
}

////////////////////////////////////////////////////////////////
//main
// 全局连接对象
CConnectionInterface *g_pConnection = NULL;
CConnectionInterface *g_pConnectionHq = NULL;

CBusiness g_szBusiness;
CBusiness g_szBusinessHq;

int main()
{

    //通过T2SDK的引出函数，来获取一个新的CConfig对象
    //此对象在创建连接对象时被传递，用于配置所创建的连接对象的各种属性（比如服务器IP地址、安全模式等）
    //值得注意的是，在向配置对象设置配置信息时，配置信息既可以从ini文件中载入，
    //也可以在程序代码中设定，或者是2者的混合，如果对同一个配置项设不同的值，则以最近一次设置为准
    CConfigInterface * lpConfig = NewConfig();

    //通过T2SDK的引出函数NewXXXX返回的对象，需要调用对象的Release方法释放，而不能直接用delete
    //因为t2sdk.dll和调用程序可能是由不同的编译器、编译模式生成，delete可能会导致异常
    //为了适合Delphi等使用（Delphi对接口自动调用AddRef方法），用C/C++开发的代码，需要在NewXXXX之后调用一下AddRef
    //以保证引用计数正确
    lpConfig->AddRef();

    ///从配置文件中加载配置
    lpConfig->Load("t2sdk.ini");

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
    g_pConnection = NewConnection(lpConfig);
    
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
    
    return 0;
}
