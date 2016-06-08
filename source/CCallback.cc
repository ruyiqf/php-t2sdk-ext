/** @file
* 演示T2_SDK进行异步发包、收包
* 并且实现了T2_SDK断开重连的一个方法（运行平台Win32）
* @author  T2小组
* @author  恒生电子股份有限公司
* @version 1.0
* @date    20090325
*/

#include <Include/t2sdk_interface.h>
#include "source/CCallback.h"

// 全局事件

void ShowPacket(IF2UnPacker *lpUnPacker)
{
    int i = 0, t = 0, j = 0, k = 0;

    for (i = 0; i < lpUnPacker->GetDatasetCount(); ++i)
    {
        // 设置当前结果集
        lpUnPacker->SetCurrentDatasetByIndex(i);

        // 打印字段
        for (t = 0; t < lpUnPacker->GetColCount(); ++t)
        {
            printf("%20s", lpUnPacker->GetColName(t));
        }

        putchar('\n');

        // 打印所有记录
        for (j = 0; j < (int)lpUnPacker->GetRowCount(); ++j)
        {
            // 打印每条记录
            for (k = 0; k < lpUnPacker->GetColCount(); ++k)
            {
                switch (lpUnPacker->GetColType(k))
                {
                case 'I':
                    printf("%20d", lpUnPacker->GetIntByIndex(k));
                    break;

                case 'C':
                    printf("%20c", lpUnPacker->GetCharByIndex(k));
                    break;

                case 'S':
                    printf("%20s", lpUnPacker->GetStrByIndex(k));
                    break;

                case 'F':
                    printf("%20f", lpUnPacker->GetDoubleByIndex(k));
                    break;

                case 'R':
                    {
                        int nLength = 0;
                        void *lpData = lpUnPacker->GetRawByIndex(k, &nLength);

                        // 对2进制数据进行处理
                        break;
                    }

                default:
                    // 未知数据类型
                    printf("未知数据类型。\n");
                    break;
                }
            }

            putchar('\n');

            lpUnPacker->Next();
        }

        putchar('\n');
    }
}

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
    puts("CCallback::OnClose");
}

void CCallback::OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
}

void CCallback::Reserved1(void *a, void *b, void *c, void *d)
{
}

void CCallback::Reserved2(void *a, void *b, void *c, void *d)
{
}

void CCallback::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
	
}

void CCallback::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{
    switch (nResult)
    {
    case 0:
        {
            puts("业务操作成功。");
            ShowPacket((IF2UnPacker *)lpUnPackerOrStr);
            break;
        }

    case 1:
        {
            puts("业务操作失败。");
            ShowPacket((IF2UnPacker *)lpUnPackerOrStr);
            break;
        }

    case 2:
        {
            puts((char *)lpUnPackerOrStr);
            break;
        }

    default:
        {
            puts("业务包解包失败。");
            break;
        }
    }
}

int  CCallback::Reserved3()
{
    return 0;
}

void CCallback::Reserved4()
{
}

void CCallback::Reserved5()
{
}

void CCallback::Reserved6()
{
}

void CCallback::Reserved7()
{
}
void CCallback::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{
	
}

