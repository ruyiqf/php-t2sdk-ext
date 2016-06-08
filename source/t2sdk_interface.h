#include <stdio.h>
#include <stdlib.h>

#ifndef _T2SDK_INTERFACE_H
# define _T2SDK_INTERFACE_H

#ifdef _WIN32
	#if !defined( FUNCTION_CALL_MODE )
	#define FUNCTION_CALL_MODE		__stdcall
	#endif
#else
	#define FUNCTION_CALL_MODE
#endif

#ifndef HS_IKNOWN_SDK
#define HS_IKNOWN_SDK
struct IKnown
{
    virtual unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv) = 0;

    virtual unsigned long  FUNCTION_CALL_MODE AddRef() = 0;

    virtual unsigned long  FUNCTION_CALL_MODE Release() =  0;
};
#endif

#include <string.h>

#ifdef _WIN32
typedef unsigned int		uint32;
#else
#include <sys/types.h>
#if defined(__linux__)
#include <stdint.h>
#endif
typedef uint32_t			uint32;
#endif

///ESB×éÃû³¤¶È£¬Ãû×ÖÎª¿É¼û×Ö·û£¬²»ÄÜ°üº¬ÊµÀý·Ö¸ô·û¡¢¿Õ¸ñ¡¢·ÖºÅ;
#define IDENTITY_NAME_LENGTH	32
///ÊµÀý±àºÅ×î´óÕ¼Î»³¤¶È
#define ID_LENGTH               4 
///½ÚµãÃûÈ«³¤,¶¨ÒåÊ±Ê¹ÓÃchar sName[ID_STR_LEN+1]
#define ID_STR_LEN		       (IDENTITY_NAME_LENGTH + ID_LENGTH + 1)


//	²å¼þ½Ó¿ÚÃûµÄ×î´ó³¤¶È,¶¨ÒåÊ±Ê¹ÓÃchar sName[PLUGINID_LENGTH+1]
#define PLUGINID_LENGTH	256
//	²å¼þÊµÀýÃûµÄ×î´ó³¤¶È,¶¨ÒåÊ±Ê¹ÓÃchar sName[PLUGIN_NAME_LENGTH+1]
#define PLUGIN_NAME_LENGTH	(PLUGINID_LENGTH+ID_LENGTH+1)
//	½ø³ÌÃû×î´ó³¤¶È.¶¨ÒåÊ±Ê¹ÓÃchar sName[SVR_NAME_LENGTH+1]
#define SVR_NAME_LENGTH	256
//	½ø³ÌÊµÀýÃû×î´ó³¤¶È.¶¨ÒåÊ±Ê¹ÓÃchar sName[PLUGINID_NAME_LENGTH+1]
#define SVRINSTANCE_NAME_LENGTH	(SVR_NAME_LENGTH+ID_LENGTH+1)
//ÒµÎñÏûÏ¢ÀàÐÍ


//ÇëÇó
#define REQUEST_PACKET 0 
//Ó¦´ð
#define ANSWER_PACKET  1 
//20110302 xuxp Ôö¼ÓÂ·ÓÉÐÅÏ¢µÄ½á¹¹Ìå¶¨Òå
typedef struct tagRouteInfo
{
	char ospfName[ID_STR_LEN+1];
	char nbrName[ID_STR_LEN+1];
	char svrName[SVRINSTANCE_NAME_LENGTH+1];
	char pluginID[PLUGIN_NAME_LENGTH+1];
	int connectID;
	int memberNO;

	tagRouteInfo()
	{
		memset(this,0,sizeof(tagRouteInfo));
	}
}Route_Info;


//20101228 xuxp ÎªÁË·¢ËÍºÍ·µ»Ø¶©ÔÄÍÆËÍÐÅÏ¢¶øÔö¼ÓµÄ½á¹¹ÌåµÄ¶¨Òå
typedef struct tagRequestData
{	
	int sequeceNo;
	int issueType;
	void* lpKeyInfo;
	int keyInfoLen;
	void* lpFileHead;
	int fileHeadLen;
	int packetType;//20100111 xuxp ÐÂ¼ÓµÄ°üÀàÐÍ
	Route_Info routeInfo;//20110302 xuxp ÇëÇóÀïÃæÔö¼ÓÂ·ÓÉÐÅÏ¢
	int iSubSystemNo;//20130508 xuxp ²ÎÊýÖÐÔö¼Ó×ÓÏµÍ³ºÅ´«Èë
}REQ_DATA, *LPREQ_DATA;
typedef struct tagRespondData
{
	int functionID;
	int returnCode;
	int errorNo;
	char* errorInfo;
	int issueType;
	void* lpKeyInfo;
	int keyInfoLen;
	Route_Info sendInfo;//20110302 xuxp Ó¦´ðÀïÃæÔö¼Ó·¢ËÍÕßÐÅÏ¢
}RET_DATA, *LPRET_DATA;


///½â°üÆ÷½Ó¿Ú
struct IF2ResultSet : public IKnown
{

	///È¡×Ö¶ÎÊý
    /**@return ·µ»Ø×Ö¶ÎÊý.
      */
    virtual int FUNCTION_CALL_MODE GetColCount()=0;

    ///È¡×Ö¶ÎÃû
    /** @param column:×Ö¶ÎÐòºÅ(ÒÔ0Îª»ùÊý)
      * @return ·µ»Ø×Ö¶ÎÃû  ÏÂ±êÔ½½ç ·µ»ØNULL
      */
    virtual const char * FUNCTION_CALL_MODE GetColName(int column)=0;

    //È¡×Ö¶ÎÊý¾ÝÀàÐÍ
    /** @param column:×Ö¶ÎÐòºÅ(ÒÔ0Îª»ùÊý)
      * @return:¼ûÉÏÃæ³£Á¿¶¨Òå;ÏÂ±êÔ½½ç ·µ»Ø-1
      */
    virtual char FUNCTION_CALL_MODE GetColType(int column)=0;


   ///È¡Êý×ÖÐÍ×Ö¶ÎÐ¡ÊýÎ»Êý
   /** @param column:×Ö¶ÎÐòºÅ(ÒÔ0Îª»ùÊý)
     * @return int ·µ»Ø×Ö¶ÎÊý¾ÝÐ¡ÊýÎ»Êý ÏÂ±êÔ½½ç ·µ»Ø-1
     */
   virtual int FUNCTION_CALL_MODE GetColScale(int column)=0;
  
   //È¡×Ö¶ÎÔÊÐí´æ·ÅÊý¾ÝµÄ×î´ó¿í¶È.
   /** @param column:×Ö¶ÎÐòºÅ(ÒÔ0Îª»ùÊý)
     * @return int ·µ»Ø×Ö¶Î¿í¶È ÏÂ±êÔ½½ç ·µ»Ø-1
     */
   virtual int FUNCTION_CALL_MODE GetColWidth(int column) = 0;

   ///È¡×Ö¶ÎÃû¶ÔÓ¦µÄ×Ö¶ÎÐòºÅ
   /**@param columnName: ×Ö¶ÎÃû
     *@return ·µ»Ø×Ö¶ÎÐòºÅ. ²»´æÔÚ·µ»Ø-1
     */
   virtual int  FUNCTION_CALL_MODE FindColIndex(const char * columnName)=0;

    //
    //°´×Ö¶ÎÐòºÅ(ÒÔ0Îª»ùÊý)£¬È¡×Ö¶ÎÖµ(×Ö·û´®)
    /**@param column:×Ö¶ÎÐòºÅ(ÒÔ0Îª»ùÊý)
      *@return ×Ö·û´®ÐÍ×Ö¶ÎÖµ,ÏÂ±êÔ½½ç·µ»ØNULL
      */
    virtual const char *  FUNCTION_CALL_MODE GetStrByIndex(int column)=0;

    //°´×Ö¶ÎÃû£¬È¡×Ö¶ÎÖµ(×Ö·û´®)
    /**@param columnName: ×Ö¶ÎÃû
      *@return ×Ö·û´®ÐÍ×Ö¶ÎÖµ,²»´æÔÚ·µ»ØNULL
      */
    virtual const char *  FUNCTION_CALL_MODE GetStr(const char * columnName)=0;

    //°´×Ö¶ÎÐòºÅ(ÒÔ0Îª»ùÊý)£¬È¡×Ö¶ÎÖµ
    /**@param column:×Ö¶ÎÐòºÅ(ÒÔ0Îª»ùÊý)
      *@return ×Ö·ûÐÍ×Ö¶ÎÖµ,ÏÂ±êÔ½½ç·µ»Ø'\0'.
      */
     virtual char  FUNCTION_CALL_MODE  GetCharByIndex(int column)=0;

    //°´×Ö¶ÎÃû£¬È¡×Ö¶ÎÖµ
    /**@param columnName: ×Ö¶ÎÃû
     *@return ×Ö·ûÐÍ×Ö¶ÎÖµ,²»´æÔÚ·µ»Ø'\0'
     */
    virtual char   FUNCTION_CALL_MODE GetChar(const char * columnName)=0;

    //°´×Ö¶ÎÐòºÅ£¬È¡×Ö¶ÎÖµ
    /**@param column:×Ö¶ÎÐòºÅ(ÒÔ0Îª»ùÊý)
      *@return doubleÐÍ×Ö¶ÎÖµ,ÏÂ±êÔ½½ç·µ»Ø0
      */
    virtual double  FUNCTION_CALL_MODE GetDoubleByIndex( int column)=0;
    
	///°´×Ö¶ÎÃû£¬È¡×Ö¶ÎÖµ
    /**@param columnName:×Ö¶ÎÃû
      *@return doubleÐÍ×Ö¶ÎÖµ,²»´æÔÚ·µ»Ø0
      */
    virtual double  FUNCTION_CALL_MODE GetDouble(const char * columnName)=0;

    ///°´×Ö¶ÎÐòºÅ£¬È¡×Ö¶ÎÖµ
    /**@param column:×Ö¶ÎÐòºÅ(ÒÔ0Îª»ùÊý)
      *@return intÐÍ×Ö¶ÎÖµ,ÏÂ±êÔ½½ç·µ»Ø0
      */
    virtual int  FUNCTION_CALL_MODE GetIntByIndex(int column)=0;

    ///°´×Ö¶ÎÃû£¬È¡×Ö¶ÎÖµ
    /**@param columnName:×Ö¶ÎÃû
      *@return intÐÍ×Ö¶ÎÖµ,²»´æÔÚ·µ»Ø0
      */
    virtual int FUNCTION_CALL_MODE GetInt(const char * columnName)=0;

	///°´×Ö¶ÎÐòºÅ»ñµÃ×Ö¶ÎÖµ,¶þ½øÖÆÊý¾Ý
	/**@param column: ×Ö¶ÎÐòºÅ(ÒÔ0Îª»ùÊý)
	*@param   lpRawLen: [out]Êý¾Ý³¤¶È
	*@return : Êý¾ÝÊ×µØÖ·
	*/
	virtual void *  FUNCTION_CALL_MODE GetRawByIndex(int column,int * lpRawLen) = 0;

	///°´×Ö¶ÎÃû£¬È¡×Ö¶ÎÖµ
	/**@param columnName:×Ö¶ÎÃû
	*@param   lpRawLen: [out]Êý¾Ý³¤¶È
	*@return : Êý¾ÝÊ×µØÖ·
	*/
	virtual void *  FUNCTION_CALL_MODE GetRaw(const char * columnName,int * lpRawLen) = 0;


	///×îºóÒ»´ÎÈ¡µÄ×Ö¶ÎÖµÊÇ·ñÎªNULL
   /**@return 0 ÊÇ£¬ 1²»ÊÇ
   */
    virtual int  FUNCTION_CALL_MODE WasNull()=0;

    ///È¡ÏÂÒ»Ìõ¼ÇÂ¼
    virtual void  FUNCTION_CALL_MODE Next()=0;

    ///ÅÐ¶ÏÊÇ·ñÎª½áÎ²
    /**@return 1 ÊÇ£¬0 ²»ÊÇ;
      */
    virtual int  FUNCTION_CALL_MODE IsEOF()=0;
   
	///ÅÐ¶ÏÊÇ·ñÎª¿Õ
    /**@return 1 ÊÇ£¬0 ²»ÊÇ;
      */
    virtual int  FUNCTION_CALL_MODE IsEmpty()=0;
    
    virtual void * FUNCTION_CALL_MODE Destroy() = 0;
};

struct IF2UnPacker;

///¶à½á¹û¼¯´ò°ü½Ó¿Ú(Ò»¸ö°ü¿ÉÓÐ¶à¸öÒì¹¹½á¹û¼¯)
/**Ö´ÐÐÐòÁÐ:
 *
 * 0¡¢×¼±¸(¿ÉÑ¡, Èô²»½øÐÐ¸Ã²Ù×÷, ÔòÓÉIF2PackerÄÚ²¿ÉêÇëÄÚ´æ»º³å)£º SetBuffer(),Èç¹û´ò°ü»º´æÇøÓÉµ÷ÓÃÕßÌá¹©,Ôò±ØÐëÔÚBeginPack()Ö®Ç°×¼±¸;
 *
 * 1¡¢¿ªÊ¼:  BeginPack(),´ò°üÆ÷¸´Î»;
 *
 * 2¡¢µÚÒ»¸ö½á¹û¼¯´ò°ü£º
 *
 *(a)Ìí¼Ó×Ö¶ÎÃûÁÐ±íÓò£ºAddField()
 *
 *(b)°´ÕÕ½á¹û¼¯¶þÎ¬±íË³Ðò£¬Öð×Ö¶Î£¬ÖðÌõ¼ÇÂ¼°´×Ö¶ÎÀàÐÍÌí¼ÓÄÚÈÝ£ºAddStr() {AddInt(), AddDouble(), AddRaw()}
 *
 * 3¡¢ÉèÖÃ½á¹û¼¯µÄ·µ»ØÂë(¿ÉÑ¡£¬Èô²»ÉèÖÃ, Ôò·µ»ØÂëÎª0) SetReturnCode()
 *
 * 4¡¢´òÏÂÒ»¸ö½á¹û¼¯(¿ÉÑ¡) NewDataSet()£¬´Ë´¦Í¬Ê±ÉèÖÃÁË¸Ã½á¹û¼¯µÄ·µ»ØÂë£»
 * 
 * 5¡¢²Î¿¼µÚ2²½ÊµÏÖÏÂÒ»¸ö½á¹û¼¯´ò°ü(¿ÉÑ¡)£»
 *
 * 6¡¢½áÊø£ºEndPack(),ÖØ¸´µ÷ÓÃ»áµ¼ÖÂ¼ÓÈë¿Õ½á¹û¼¯;
 *
 * 7¡¢È¡´ò°ü½á¹û(»º´æÇø£¬»º´æÇø´óÐ¡£¬Êý¾Ý³¤¶È)
 *    ´ò°ü½á¹ûÒ²¿ÉÒÔÖ±½Ó½â°üUnPack()·µ»Ø½â°ü½Ó¿Ú
 *
 *Ê¹ÓÃ×¢ÒâÊÂÏî:IF2PackerËùÊ¹ÓÃµÄÄÚ´æ»º´æÇø£¬ÓÉµ÷ÓÃÕß¸ºÔð»ØÊÕ£»
 *             ½á¹û¼¯¸½´øµÄ·µ»ØÂë£¬Ö»ÓÐÔÚ°ü¸ñÊ½°æ±¾0x21¼°ÒÔÉÏÊ±ÓÐÐ§£»
 */
struct IF2Packer : public IKnown
{
    ///´ò°üÆ÷³õÊ¼»¯(Ê¹ÓÃµ÷ÓÃÕßµÄ»º´æÇø)
	/** µÚÒ»´ÎÊ¹ÓÃ´ò°üÆ÷Ê±£¬¿ÉÏÈÊ¹ÓÃ±¾·½·¨ÉèÖÃºÃ»º³åÇø(Êý¾Ý³¤¶È±»ÖÃÎªiDataLen)
	 *@param  char * pBuf  »º³åÇøµØÖ·
 	 *@param  int iBufSize  »º³åÇø¿Õ¼ä
 	 *@param  int iDataLen  ÒÑÓÐÊý¾Ý³¤¶È£¬ÐÂÔöÊý¾Ý¼ÓÔÚÒÑÓÐÊý¾ÝÖ®ºó£¨Ö»¶ÔV1.0¸ñÊ½µÄ°üÓÐÐ§£© 	 
 	 */
	virtual void FUNCTION_CALL_MODE SetBuffer(void * pBuf,int iBufSize,int iDataLen=0 )=0;

	///¸´Î»£¬ÖØÐÂ¿ªÊ¼´òÁíÒ»¸ö°ü(×Ö¶ÎÊýÓë¼ÇÂ¼ÊýÖÃÎª0ÐÐ0Àý)
	/**
	 * ¹¦ÄÜ£º¿ªÊ¼´ò°ü£¬°Ñ°ü³¤¶ÈÇåÁã(ÖØ¸´Ê¹ÓÃÒÑÓÐµÄ»º´æÇø¿Õ¼ä)
	 *@return ÎÞ
	 */
	virtual void FUNCTION_CALL_MODE BeginPack(void)=0;

	///¿ªÊ¼´òÒ»¸ö½á¹û¼¯
	/**ÔÚ´òµ¥½á¹û¼¯µÄ°üÊ±£¬¿ÉÒÔ²»µ÷ÓÃ±¾·½·¨,¾ùÈ¡Ä¬ÈÏÖµ
	 *@param const char *szDatasetName 0x20°æ´ò°üÐèÒªÖ¸Ã÷½á¹û¼¯Ãû×Ö
	 *@param int iReturnCode           0x20°æ´ò°üÐèÒªÎªÃ¿¸ö½á¹û¼¯Ö¸Ã÷·µ»ØÖµ
	 */
	virtual int FUNCTION_CALL_MODE NewDataset(const char *szDatasetName, int iReturnCode = 0)=0;

	/**
	 * ¹¦ÄÜ£ºÏò°üÌí¼Ó×Ö¶Î
	 *
	 *ÓÐÖ´ÐÐ´ÎÐòÒªÇó:ÔÚ NewDataset()»òReset(),SetBuffer()Ö®ºó,Öð¸ö×Ö¶Î°´Ë³ÐòÌí¼Ó;
	 *
	 *@param szFieldName£º×Ö¶ÎÃû
	 *@param cFieldType £º×Ö¶ÎÀàÐÍ:IÕûÊý£¬F¸¡µãÊý£¬C×Ö·û£¬S×Ö·û´®£¬RÈÎÒâ¶þ½øÖÆÊý¾Ý
	 *@param iFieldWidth £º×Ö¶Î¿í¶È£¨ËùÕ¼×î´ó×Ö½ÚÊý£©
	 *@param iFieldScale £º×Ö¶Î¾«¶È,¼´cFieldType='F'Ê±µÄÐ¡ÊýÎ»Êý(È±Ê¡Îª4Î»Ð¡Êý)
	 *@return ¸ºÊý±íÊ¾Ê§°Ü£¬·ñÔòÎªÄ¿Ç°°üµÄ³¤¶È
	 */
	virtual int FUNCTION_CALL_MODE AddField(const char *szFieldName,char cFieldType ='S',int iFieldWidth=255,int iFieldScale=4)=0;

	/**
	 * ¹¦ÄÜ£ºÏò°üÌí¼Ó×Ö·û´®Êý¾Ý
     * ÓÐÖ´ÐÐ´ÎÐòÒªÇó:±ØÐëÔÚËùÓÐ×Ö¶ÎÔö¼ÓÍêÖ®ºó,Öð¸ö×Ö¶Î°´Ë³ÐòÌí¼Ó;
	 *@param       szValue£º×Ö·û´®Êý¾Ý
	 *@return ¸ºÊý±íÊ¾Ê§°Ü£¬·ñÔòÎªÄ¿Ç°°üµÄ³¤¶È
	 */
	virtual int FUNCTION_CALL_MODE AddStr(const char *szValue)=0;

	/**
     * ¹¦ÄÜ£ºÏò°üÌí¼ÓÕûÊýÊý¾Ý
 	 *@param       iValue£ºÕûÊýÊý¾Ý
	 *@return ¸ºÊý±íÊ¾Ê§°Ü£¬·ñÔòÎªÄ¿Ç°°üµÄ³¤¶È
	 */
	virtual int FUNCTION_CALL_MODE AddInt(int iValue)=0;
	
    /**
	 * ¹¦ÄÜ£ºÏò°üÌí¼Ó¸¡µãÊý¾Ý
	 *@param       fValue£º¸¡µãÊý¾Ý
	 *@return ¸ºÊý±íÊ¾Ê§°Ü£¬·ñÔòÎªÄ¿Ç°°üµÄ³¤¶È
	 */
	virtual int FUNCTION_CALL_MODE AddDouble(double fValue)=0;

	/**
	 * ¹¦ÄÜ£ºÏò°üÌí¼ÓÒ»¸ö×Ö·û
	 *@param		 cValue£º×Ö·û
	 *@return ¸ºÊý±íÊ¾Ê§°Ü£¬·ñÔòÎªÄ¿Ç°°üµÄ³¤¶È
	 */
	virtual int FUNCTION_CALL_MODE AddChar(char cValue)=0;

	/**
	 * ¹¦ÄÜ£ºÏò°üÌí¼ÓÒ»¸ö´ó¶ÔÏó
	 *@param	void * lpBuff Êý¾ÝÇø
	 *@param	int iLen  Êý¾Ý³¤¶È	 
	 *@return ¸ºÊý±íÊ¾Ê§°Ü£¬·ñÔòÎªÄ¿Ç°°üµÄ³¤¶È
	 */
	virtual int FUNCTION_CALL_MODE AddRaw(void * lpBuff,int iLen)=0;

    ///½áÊø´ò°ü
	virtual void FUNCTION_CALL_MODE EndPack()=0;
 
	/**
     * ¹¦ÄÜ£ºÈ¡´ò°ü½á¹ûÖ¸Õë
	 *@return ´ò°ü½á¹ûÖ¸Õë
     */
	virtual void * FUNCTION_CALL_MODE GetPackBuf(void) = 0;
	
	/**
     * ¹¦ÄÜ£ºÈ¡´ò°ü½á¹û³¤¶È
     *@return ´ò°ü½á¹û³¤¶È
	 */
	virtual int FUNCTION_CALL_MODE GetPackLen(void) = 0;
	
	/**
	 * ¹¦ÄÜ£ºÈ¡´ò°ü½á¹û»º³åÇø´óÐ¡
     *@return ´ò°ü½á¹û»º³åÇø´óÐ¡
	 */
	virtual int FUNCTION_CALL_MODE GetPackBufSize(void) = 0;
	
	/**
	 * ¹¦ÄÜ£ºÈ¡´ò°ü¸ñÊ½°æ±¾
     *@return °æ±¾
	 */
	virtual int FUNCTION_CALL_MODE GetVersion(void) = 0;
	
	///ÉèÖÃ½á¹û¼¯µÄ·µ»ØÂë(0x20°æÒÔÉÏÒªÇó)£¬´íÎó½á¹û¼¯ÐèÒªÉèÖÃ
	/**·µ»ØÂëÈ¡È±Ê¡Öµ0£¬Ôò²»ÉèÖÃ£¬Èç¹ûÉèÖÃ£¬Ôò±ØÐëÔÚEndPack()Ö®Ç°µ÷ÓÃ
     *@return °æ±¾
	 */
	virtual void FUNCTION_CALL_MODE SetReturnCode(unsigned long dwRetCode) = 0;

	/**Ö±½Ó·µ»Øµ±Ç°´ò°ü½á¹ûµÄ½â°ü½Ó¿Ú,±ØÐëÔÚEndPack()Ö®ºó²ÅÄÜµ÷ÓÃ,ÔÚ´ò°üÆ÷ÊÍ·ÅÊ±ÏàÓ¦µÄ½â°üÆ÷ÊµÀýÒ²ÊÍ·Å
     *@return ½â°üÆ÷½Ó¿Ú£¬´Ë½â°ü½Ó¿Ú²»ÄÜµ÷ÓÃ destroy()À´ÊÍ·Å
	 */
	virtual IF2UnPacker * FUNCTION_CALL_MODE UnPack(void) = 0;

	/**
	 *
	 */
	virtual void FUNCTION_CALL_MODE FreeMem(void* lpBuf) = 0;

	/**
	 *
	 */
	virtual void FUNCTION_CALL_MODE ClearValue() = 0;
	
	
	//20110302 xuxp Ôö¼ÓÒ»¸ö½Ó¿Úº¯Êý£¬ÓÃÀ´´«µÝµÚÒ»¸ö½á¹û¼¯µÄÃû×Ö
	///¸´Î»£¬ÖØÐÂ¿ªÊ¼´òÁíÒ»¸ö°ü(×Ö¶ÎÊýÓë¼ÇÂ¼ÊýÖÃÎª0ÐÐ0Àý)
	/**
	 * ¹¦ÄÜ£º¿ªÊ¼´ò°ü£¬°Ñ°ü³¤¶ÈÇåÁã(ÖØ¸´Ê¹ÓÃÒÑÓÐµÄ»º´æÇø¿Õ¼ä)
	 *@return ÎÞ
	 */
	virtual void FUNCTION_CALL_MODE BeginPackEx(char* szName = NULL) = 0;

	//20110324 dongpf Ôö¼ÓÒ»¸ö½Ó¿Úº¯Êý£¬ÓÃÀ´¸´Î»µ±Ç°½á¹û¼¯
	///¸´Î»µ±Ç°½á¹û¼¯(×Ö¶ÎÊýÓë¼ÇÂ¼ÊýÖÃÎª0ÐÐ0Àý)£¬²»Ó°ÏìÆäËû½á¹û¼¯
	/**
	 * ¹¦ÄÜ£º¸´Î»µ±Ç°½á¹û¼¯
	 *@return ÎÞ
	 */
	virtual void FUNCTION_CALL_MODE ClearDataSet() = 0;
};

///½â°üÆ÷½Ó¿Ú
struct IF2UnPacker : public IF2ResultSet
{
	/**È¡´ò°ü¸ñÊ½°æ±¾
     *@return °æ±¾
	 */
	virtual int FUNCTION_CALL_MODE GetVersion(void) = 0;

	/**È¡½â°üÊý¾Ý³¤¶È
     *@return 							0 ±íÊ¾³É¹¦£¬ ÆäËüÎªÊ§°Ü
	 */
	virtual int FUNCTION_CALL_MODE Open(void * lpBuffer,unsigned int iLen) = 0;

    ///È¡½á¹û¼¯¸öÊý(0x20ÒÔÉÏ°æ±¾Ö§³Ö)
    virtual int FUNCTION_CALL_MODE GetDatasetCount()=0;

    ///ÉèÖÃµ±Ç°½á¹û¼¯(0x20ÒÔÉÏ°æ±¾Ö§³Ö)
    /**
	 *@param  int nIndex				½á¹û¼¯±àºÅ
	 *@return int						0±íÊ¾³É¹¦£¬·ñÔòÎªÊ§°Ü
	 */
    virtual int FUNCTION_CALL_MODE SetCurrentDatasetByIndex(int nIndex)=0;

    ///ÉèÖÃµ±Ç°½á¹û¼¯ (0x20ÒÔÉÏ°æ±¾Ö§³Ö)
    /**
	 *@param  const char *szDatasetName	½á¹û¼¯Ãû³Æ
	 *@return int						0 ±íÊ¾³É¹¦£¬·ñÔòÎªÊ§°Ü
	 */
    virtual int FUNCTION_CALL_MODE SetCurrentDataset(const char *szDatasetName)=0;

	/** È¡½â°üÊý¾ÝÇøÖ¸Õë
	 *@return Êý¾ÝÇøÖ¸Õë
     */
	virtual void * FUNCTION_CALL_MODE GetPackBuf(void) = 0;

	/** È¡½â°üÊý¾Ý³¤¶È
     *@return ½â°üÊý¾Ý³¤¶È
	 */
	virtual unsigned int FUNCTION_CALL_MODE GetPackLen(void) = 0;

	/**È¡½â°üÊý¾Ý¼ÇÂ¼ÌõÊý,20051207ÒÔºó°æ±¾Ö§³Ö
     *@return ¼ÇÂ¼ÌõÊý
	 */
	virtual unsigned int FUNCTION_CALL_MODE GetRowCount(void) = 0;
	
	///½á¹û¼¯ÐÐ¼ÇÂ¼ÓÎ±ê½Ó¿Ú£ºÈ¡½á¹û¼¯µÄÊ×Ìõ¼ÇÂ¼
    virtual void FUNCTION_CALL_MODE First() = 0;

    ///½á¹û¼¯ÐÐ¼ÇÂ¼ÓÎ±ê½Ó¿Ú£ºÈ¡½á¹û¼¯µÄ×îºóÒ»Ìõ¼ÇÂ¼
    virtual void FUNCTION_CALL_MODE Last() = 0;

    ///½á¹û¼¯ÐÐ¼ÇÂ¼ÓÎ±ê½Ó¿Ú£ºÈ¡½á¹û¼¯µÄµÚnÌõ¼ÇÂ¼£¬È¡Öµ·¶Î§[1, GetRowCount()]
    virtual void FUNCTION_CALL_MODE Go(int nRow) = 0;
	
	///»ñÈ¡µ±Ç°½á¹û¼¯Ãû×ÖµÄ½Ó¿Ú,Ã»ÓÐÃû×Ö·µ»Ø""
	virtual const char* FUNCTION_CALL_MODE GetDatasetName() = 0;
	
	virtual int FUNCTION_CALL_MODE OpenAndCopy(void * lpBuffer,unsigned int iLen) = 0;
};

///Á¬½Ó¶ÔÏó CConnectionInterface µÄ²ÎÊýÅäÖÃ¶ÔÏóCConfigInterface
/**
* °üÀ¨´ÓÎÄ¼þ¼ÓÔØ¡¢±£´æµ½ÎÄ¼þ£¬¶ÁÐ´²Ù×÷
*/
class CConfigInterface: public IKnown
{
public:
	/**
    * ´ÓÎÄ¼þ¼ÓÔØ
    * @param szFileName ÎÄ¼þÃû£¬¸ñÊ½ÀàËÆini£¬¾ßÌå²Î¿¼¿ª·¢°üÊ¾Àý
    * @return ·µ»Ø0±íÊ¾³É¹¦£¬·ñÔòÊ§°Ü
    */
    virtual int FUNCTION_CALL_MODE Load(const char *szFileName) = 0;

    /**
    * ±£´æµ½ÎÄ¼þ
    * @param szFileName ÎÄ¼þÃû
    * @return ·µ»Ø0±íÊ¾³É¹¦£¬·ñÔòÊ§°Ü
    */
    virtual int FUNCTION_CALL_MODE Save(const char *szFileName) = 0;

    /**
    * È¡×Ö·û´®Öµ
    * @param szSection ½ÚÃû
    * @param szEntry   ±äÁ¿Ãû
    * @param szDefault È±Ê¡Öµ
    * @return ×Ö·û´®Öµ£¬Ã»ÓÐÕÒµ½Ê±·µ»ØszDefault
    */
    virtual const char * FUNCTION_CALL_MODE GetString(const char *szSection, const char *szEntry, const char *szDefault) = 0;

    /**
    * È¡ÕûÊýÖµ
    * @param szSection ½ÚÃû
    * @param szEntry   ±äÁ¿Ãû
    * @param iDefault  È±Ê¡Öµ
    * @return ÕûÊýÖµ£¬Ã»ÓÐÕÒµ½Ê±·µ»ØiDefault
    */
    virtual int FUNCTION_CALL_MODE GetInt(const char *szSection, const char *szEntry, int iDefault) = 0;

    /**
    * ÉèÖÃ×Ö·û´®Öµ
    * @param szSection ½ÚÃû
    * @param szEntry   ±äÁ¿Ãû
    * @param szValue   Öµ
    * @return 0±íÊ¾³É¹¦£¬·ñÔòÊ§°Ü
    */
    virtual int FUNCTION_CALL_MODE SetString(const char *szSection, const char *szEntry, const char *szValue) = 0;

    /**
    * ÉèÖÃÕûÊýÖµ
    * @param szSection ½ÚÃû
    * @param szEntry   ±äÁ¿Ãû
    * @param iValue    Öµ
    * @return 0±íÊ¾³É¹¦£¬·ñÔòÊ§°Ü
    */
    virtual int FUNCTION_CALL_MODE SetInt(const char *szSection, const char *szEntry, int iValue) = 0;
};


typedef struct tagBizRouteInfo
{
	char ospfName[ID_STR_LEN+1];//Â·ÓÉÄ¿±ê½ÚµãÖÐ¼ä¼þÃû×Ö
	char nbrName[ID_STR_LEN+1];//ÖÐ¼ä¼þ½ÚµãµÄÁÚ¾ÓÃû×Ö
	char svrName[SVRINSTANCE_NAME_LENGTH+1];//ÖÐ¼ä¼þµÄ½ø³ÌÃû×Ö
	char pluginID[PLUGIN_NAME_LENGTH+1];//ÖÐ¼ä¼þ²å¼þÃû
	int connectID;//Á¬½ÓºÅ
	int memberNO;//³ÉÔ±±àºÅ
	
	tagBizRouteInfo()
	{
		memset(this,0,sizeof(tagBizRouteInfo));
	}
}BIZROUTE_INFO;


struct IBizMessage : public IKnown
{
	//ÉèÖÃ¹¦ÄÜºÅ
	virtual void FUNCTION_CALL_MODE SetFunction(const int nFUnctionNo) = 0;
	//»ñÈ¡¹¦ÄÜºÅ
	virtual int FUNCTION_CALL_MODE GetFunction() = 0;

	//ÉèÖÃ°üÀàÐÍ
	virtual void FUNCTION_CALL_MODE SetPacketType(const int nPacketType) = 0;
	//»ñÈ¡°üÀàÐÍ
	virtual int FUNCTION_CALL_MODE GetPacketType() = 0;

	//ÉèÖÃÓªÒµ²¿ºÅ
	virtual void FUNCTION_CALL_MODE SetBranchNo(const int nBranchNo) = 0;
	//»ñÈ¡ÓªÒµ²¿ºÅ
	virtual int FUNCTION_CALL_MODE GetBranchNo() = 0;

	//ÉèÖÃÏµÍ³ºÅ
	virtual void FUNCTION_CALL_MODE SetSystemNo(const int nSystemNo) = 0;
	//»ñÈ¡ÏµÍ³ºÅ
	virtual int FUNCTION_CALL_MODE GetSystemNo() = 0;

	//ÉèÖÃ×ÓÏµÍ³ºÅ
	virtual void FUNCTION_CALL_MODE SetSubSystemNo(const int nSubSystemNo) = 0;
	//»ñÈ¡×ÓÏµÍ³ºÅ
	virtual int FUNCTION_CALL_MODE GetSubSystemNo() = 0;

	//ÉèÖÃ·¢ËÍÕß±àºÅ
	virtual void FUNCTION_CALL_MODE SetSenderId(const int nSenderId) = 0;
	//»ñÈ¡·¢ËÍÕß±àºÅ
	virtual int FUNCTION_CALL_MODE GetSenderId() = 0;

	//ÉèÖÃ°üÐòºÅ
	virtual void FUNCTION_CALL_MODE SetPacketId(const int nPacketId) = 0;
	//»ñÈ¡°üÐòºÅ
	virtual int FUNCTION_CALL_MODE GetPacketId() = 0;

	//ÉèÖÃÄ¿µÄµØÂ·ÓÉ
	virtual void FUNCTION_CALL_MODE SetTargetInfo(const BIZROUTE_INFO targetInfo) = 0;
	//»ñÈ¡Ä¿µÄµØÂ·ÓÉ
	virtual void FUNCTION_CALL_MODE GetTargetInfo(BIZROUTE_INFO& targetInfo) = 0;
	
	//ÉèÖÃ·¢ËÍÕßÂ·ÓÉ
	virtual void FUNCTION_CALL_MODE SetSendInfo(const BIZROUTE_INFO sendInfo) = 0;
	//»ñÈ¡·¢ËÍÕßÂ·ÓÉ
	virtual void FUNCTION_CALL_MODE GetSendInfo(BIZROUTE_INFO& sendInfo) = 0;

	//ÉèÖÃ´íÎóºÅ
	virtual void FUNCTION_CALL_MODE SetErrorNo(const int nErrorNo) = 0;
	//»ñÈ¡´íÎóºÅ
	virtual int FUNCTION_CALL_MODE GetErrorNo() = 0;
	
	//ÉèÖÃ´íÎóÐÅÏ¢
	virtual void FUNCTION_CALL_MODE SetErrorInfo(const char* strErrorInfo) = 0;
	//»ñÈ¡´íÎóÐÅÏ¢
	virtual const char* FUNCTION_CALL_MODE GetErrorInfo() = 0;
	
	//ÉèÖÃ·µ»ØÂë
	virtual void FUNCTION_CALL_MODE SetReturnCode(const int nReturnCode) = 0;
	//»ñÈ¡·µ»ØÂë
	virtual int FUNCTION_CALL_MODE GetReturnCode() = 0;

	//ÉèÖÃÒµÎñÄÚÈÝ
	virtual void FUNCTION_CALL_MODE SetContent(void* lpContent,int iLen) = 0;
	//»ñÈ¡ÒµÎñÄÚÈÝ
	virtual const void* FUNCTION_CALL_MODE GetContent(int& iLen) = 0;

	//ÒÔÏÂ½Ó¿ÚÓÃÓÚÏûÏ¢ÖÐÐÄ1.0µÄ¶©ÔÄ
	//ÉèÖÃ¶©ÔÄÀàÐÍ
	virtual void FUNCTION_CALL_MODE SetIssueType(const int nIssueType) = 0;
	//»ñÈ¡¶©ÔÄÀàÐÍ
	virtual int FUNCTION_CALL_MODE GetIssueType() = 0;

	//ÉèÖÃÐòºÅ
	virtual void FUNCTION_CALL_MODE SetSequeceNo(const int nSequeceNo) = 0;
	//»ñÈ¡ÐòºÅ
	virtual int FUNCTION_CALL_MODE GetSequeceNo() = 0;

	//ÉèÖÃ¹Ø¼ü×Ö¶ÎÐÅÏ¢
	virtual void FUNCTION_CALL_MODE SetKeyInfo(void* lpKeyData,int iLen) = 0;
	//»ñÈ¡¹Ø¼ü×Ö¶ÎÐÅÏ¢
	virtual const void* FUNCTION_CALL_MODE GetKeyInfo(int& iLen) = 0;

	//ÉèÖÃ¸½¼ÓÊý¾Ý£¬¶©ÔÄÍÆËÍÊ±Ô­Ñù·µ»Ø
	virtual void FUNCTION_CALL_MODE SetAppData(const void* lpAppdata,int nAppLen) = 0;
	//»ñÈ¡¸½¼ÓÊý¾Ý£¬¶©ÔÄÍÆËÍÊ±Ô­Ñù·µ»Ø
	virtual const void* FUNCTION_CALL_MODE GetAppData(int& nAppLen) = 0;

	//ÇëÇó×ªÓ¦´ð
	virtual int	FUNCTION_CALL_MODE ChangeReq2AnsMessage() = 0;

	//»ñÈ¡¶þ½øÖÆ
	virtual void* FUNCTION_CALL_MODE GetBuff(int& nBuffLen) = 0;
	//½âÎö¶þ½øÖÆ
	virtual int	FUNCTION_CALL_MODE SetBuff(const void* lpBuff,int nBuffLen) = 0;

	//Çå³ýÏûÏ¢ÄÚµÄ×Ö¶Î£¬¿ÉÒÔÏÂ´Î¸´ÓÃ¡£
	virtual void FUNCTION_CALL_MODE ReSet() = 0;
};

#define IDENTITY_NAME_LENGTH    32  /**< ¿Í»§¶ËÃû×Ö³¤¶È */
#define MAX_MACADDRESS_LEN	    18  /**< MAC µØÖ·³¤¶È */
#define MAX_RAND_LEN	        4   /**< Ëæ»úÊý³¤¶È */

/** ¿Í»§±êÊ¶³¤¶È */
#define MAX_BIZNAME_LEN \
	IDENTITY_NAME_LENGTH+1+MAX_MACADDRESS_LEN+1+MAX_RAND_LEN+2

#define INIT_RECVQ_LEN 256          /**< ½ÓÊÕ¶ÓÁÐ³õÊ¼³¤¶È */
#define STEP_RECVQ_LEN 512          /**< ½ÓÊÕ¶ÓÁÐÀ©Õ¹²½³¤ */
#define SIMPLIFIED_CHINESE      0   /**< ´íÎóÐÅÏ¢ÓïÑÔ:¼òÌåÖÐÎÄ */
#define ENGLISH                 1   /**< ´íÎóÐÅÏ¢ÓïÑÔ:Ó¢ÎÄ */
#define MAX_FILTERFIELD_LEN 63      /**< ¹ýÂË×Ö¶Î³¤¶È */

/** Ö÷Ìâ¿É¿¿µÈ¼¶ */
enum ReliableLevel
{
    LEVEL_DOBEST            = 0,    /**< ¾¡Á¦¶øÎª */
    LEVEL_DOBEST_BYSEQ      = 1,    /**< ¾¡Á¦ÓÐÐò */
    LEVEL_MEM               = 2,    /**< ÄÚ´æ */
    LEVEL_FILE              = 3,    /**< ÎÄ¼þ */
    LEVEL_SYSTEM            = 4     /**< ÏµÍ³ */
};

/** 
 * ¹ýÂËÆ÷½Ó¿Ú
 */
class CFilterInterface:public IKnown
{
public:
   /**
    * ¸ù¾ÝÏÂ±ê»ñÈ¡¹ýÂËÌõ¼þµÄÃû×Ö
    * @param index ¶ÔÓ¦µÄ¹ýÂËÌõ¼þÏÂ±ê
    * @return ·µ»Ø¶ÔÓ¦µÄÏÂ±ê¹ýÂËÌõ¼þµÄÃû×Ö£¬·ñÔò·µ»ØNULL.
    */
    virtual char* FUNCTION_CALL_MODE GetFilterNameByIndex(int index) = 0;

   /**
    * ¸ù¾ÝÏÂ±ê»ñÈ¡¹ýÂËÌõ¼þµÄÖµ
    * @param index ¶ÔÓ¦µÄ¹ýÂËÌõ¼þÏÂ±ê
    * @return ·µ»Ø¶ÔÓ¦µÄÏÂ±ê¹ýÂËÌõ¼þµÄÖµ£¬·ñÔò·µ»ØNULL.
    */
    virtual char* FUNCTION_CALL_MODE GetFilterValueByIndex(int index)= 0;

   /**
    * ¸ù¾Ý¹ýÂËÌõ¼þµÄÃû×Ö»ñÈ¡¹ýÂËÌõ¼þµÄÖµ
    * @param fileName ¶ÔÓ¦µÄ¹ýÂËÌõ¼þÃû×Ö
    * @return ·µ»Ø¶ÔÓ¦µÄ¹ýÂËÌõ¼þÃû×ÖµÄÌõ¼þÖµ£¬·ñÔò·µ»ØNULL.
    */
    virtual char* FUNCTION_CALL_MODE GetFilterValue(char*  fileName)= 0;

   /**
    * »ñÈ¡¹ýÂËÌõ¼þµÄ¸öÊý
    * @return ·µ»Ø¶ÔÓ¦¹ýÂËÌõ¼þµÄ¸öÊý£¬Ã»ÓÐ·µ»Ø0
    */
    virtual int   FUNCTION_CALL_MODE GetCount() = 0;

   /**
    * ÉèÖÃ¹ýÂËÌõ¼þ£¬¸ù¾Ý¹ýÂËÌõ¼þÃû×ÖºÍÖµ
    * @param filterName ¶ÔÓ¦µÄ¹ýÂËÌõ¼þÃû×Ö
    * @param filterValue ¶ÔÓ¦µÄ¹ýÂËÌõ¼þÃû×ÖµÄÖµ
    */
    virtual void FUNCTION_CALL_MODE  SetFilter(char* filterName,char* filterValue) =0;
};

/**
 * ¶©ÔÄ²ÎÊýÀà½Ó¿Ú
 */
class CSubscribeParamInterface:public IKnown
{
public:

   /**
    * ÉèÖÃÖ÷ÌâÃû×Ö
    * @param szName ¶ÔÓ¦µÄÖ÷ÌâÃû×Ö
    */
    virtual void FUNCTION_CALL_MODE  SetTopicName(char* szName) =0;

   /**
    * ÉèÖÃ¸½¼ÓÊý¾Ý
    * @param lpData ¸½¼ÓÊý¾ÝµÄÊ×µØÖ·
    * @param iLen ¸½¼ÓÊý¾ÝµÄ³¤¶È
    */
    virtual void FUNCTION_CALL_MODE  SetAppData(void* lpData,int iLen)=0;
    
   /**
    * Ìí¼Ó¹ýÂËÌõ¼þ
    * @param filterName ¹ýÂËÌõ¼þµÄÃû×Ö
    * @param filterValue ¹ýÂËÌõ¼þµÄÖµ
    */
    virtual void FUNCTION_CALL_MODE  SetFilter(char* filterName,char* filterValue)=0;

   /**
    * Ìí¼Ó·µ»Ø×Ö¶Î
    * @param filedName ÐèÒªÌí¼ÓµÄ·µ»Ø×Ö¶Î
    */
    virtual void FUNCTION_CALL_MODE  SetReturnFiled(char* filedName)=0;

   /**
    * ÉèÖÃÊÇ·ñ²¹È±±êÖ¾
    * @param bFromNow true±íÊ¾ÐèÒªÖ®Ç°µÄÊý¾Ý£¬Ò²¾ÍÊÇ²¹È±£¬false±íÊ¾²»ÐèÒª²¹È±
    */
    virtual void FUNCTION_CALL_MODE  SetFromNow(bool bFromNow)=0;

   /**
    * ÉèÖÃ¸²¸Ç¶©ÔÄ±êÖ¾
    * @param bReplace true±íÊ¾¸²¸Ç¶©ÔÄ£¬È¡ÏûÖ®Ç°µÄËùÓÐ¶©ÔÄ£¬Ö»±£Áôµ±Ç°µÄ¶©ÔÄ£¬false±íÊ¾×·¼Ó¶©ÔÄ
    */
    virtual void FUNCTION_CALL_MODE  SetReplace(bool bReplace)=0;

   /**
    * ÉèÖÃ·¢ËÍ¼ä¸ô
    * @param nSendInterval µ¥Î»ÊÇÃë
    */
    virtual void FUNCTION_CALL_MODE  SetSendInterval(int nSendInterval)=0;

   /**
    * »ñÈ¡Ö÷ÌâÃû×Ö
    * @return ·µ»ØÖ÷ÌâÃû×ÖÐÅÏ¢
    */
    virtual char* FUNCTION_CALL_MODE  GetTopicName() =0;

   /**
    * »ñÈ¡¸½¼ÓÊý¾Ý
    * @param iLen ³ö²Î£¬±íÊ¾¸½¼ÓÊý¾ÝµÄ³¤¶È
    * @return ·µ»Ø¸½¼ÓÊý¾ÝÊ×µØÖ·£¬Ã»ÓÐ·µ»ØNULL
    */
    virtual void* FUNCTION_CALL_MODE  GetAppData(int *iLen) =0;

   /**
    * »ñÈ¡¶ÔÓ¦µÄ¹ýÂË×Ö¶ÎµÄÃû×Ö
    * @param index ¶ÔÓ¦µÄ¹ýÂËÌõ¼þÏÂ±ê
    * @return ·µ»Ø¶ÔÓ¦µÄÏÂ±ê¹ýÂËÌõ¼þµÄÃû×Ö£¬·ñÔò·µ»ØNULL.
    */
    virtual char* FUNCTION_CALL_MODE  GetFilterNameByIndex(int index) = 0;

   /**
    * ¸ù¾ÝÏÂ±ê»ñÈ¡¹ýÂËÌõ¼þµÄÖµ
    * @param index ¶ÔÓ¦µÄ¹ýÂËÌõ¼þÏÂ±ê
    * @return ·µ»Ø¶ÔÓ¦µÄÏÂ±ê¹ýÂËÌõ¼þµÄÖµ£¬·ñÔò·µ»ØNULL.
    */
    virtual char* FUNCTION_CALL_MODE  GetFilterValueByIndex(int index)= 0;

   /**
    * ¸ù¾Ý¹ýÂËÌõ¼þµÄÃû×Ö»ñÈ¡¹ýÂËÌõ¼þµÄÖµ
    * @param fileName ¶ÔÓ¦µÄ¹ýÂËÌõ¼þÃû×Ö
    * @return ·µ»Ø¶ÔÓ¦µÄ¹ýÂËÌõ¼þÃû×ÖµÄÌõ¼þÖµ£¬·ñÔò·µ»ØNULL.
    */
    virtual char* FUNCTION_CALL_MODE  GetFilterValue(char*  fileName)= 0;

   /**
    * »ñÈ¡¹ýÂËÌõ¼þµÄ¸öÊý
    * @return ·µ»Ø¶ÔÓ¦¹ýÂËÌõ¼þµÄ¸öÊý£¬Ã»ÓÐ·µ»Ø0
    */
    virtual int   FUNCTION_CALL_MODE  GetFilterCount() = 0;

   /**
    * »ñÈ¡·µ»Ø×Ö¶Î
    * @return ·µ»Ø¶ÔÓ¦µÄ·µ»Ø×Ö¶ÎÐÅÏ¢
    */
    virtual char* FUNCTION_CALL_MODE  GetReturnFiled()=0;

   /**
    * »ñÈ¡ÊÇ·ñ²¹È±µÄ±êÖ¾
    * @return ·µ»Ø¶ÔÓ¦µÄ²¹È±±êÖ¾
    */
    virtual bool  FUNCTION_CALL_MODE  GetFromNow()=0 ;

   /**
    * »ñÈ¡ÊÇ·ñ¸²¸Ç¶©ÔÄµÄ±êÖ¾
    * @return ·µ»Ø¶ÔÓ¦µÄ¸²¸Ç¶©ÔÄ±êÖ¾
    */
    virtual bool  FUNCTION_CALL_MODE  GetReplace() =0;

   /**
    * »ñÈ¡¶ÔÓ¦µÄ·¢ËÍÆµÂÊ
    * @return ·µ»Ø¶ÔÓ¦µÄ·¢ËÍ¼ä¸ô
    */
    virtual int   FUNCTION_CALL_MODE  GetSendInterval()=0;
};

class CSubscribeInterface;

/**
 * @brief ¶©ÔÄ»Øµ÷½Ó¿Ú·µ»ØµÄÊý¾Ý¶¨Òå£¬³ýÁË¶©ÔÄÐèÒªµÄÒµÎñÌåÖ®Íâ£¬»¹ÐèÒª·µ»ØµÄÊý¾Ý
 */
typedef struct tagSubscribeRecvData
{
	char* lpFilterData;     /**< ¹ýÂË×Ö¶ÎµÄÊý¾ÝÍ·Ö¸Õë£¬ÓÃ½â°üÆ÷½â°ü */
	int iFilterDataLen;     /**< ¹ýÂË×Ö¶ÎµÄÊý¾Ý³¤¶È */
	char* lpAppData;        /**< ¸½¼ÓÊý¾ÝµÄÊý¾ÝÍ·Ö¸Õë */
	int iAppDataLen;        /**< ¸½¼ÓÊý¾ÝµÄ³¤¶È */
	char szTopicName[260];  /**< Ö÷ÌâÃû×Ö */
	
   /**
    * tagSubscribeRecvData ¹¹Ôìº¯Êý
    */
	tagSubscribeRecvData()
	{
		memset(this,0,sizeof(tagSubscribeRecvData));
	}
}SUBSCRIBE_RECVDATA, *LPSUBSCRIBE_RECVDATA;

/**
 * ¶©ÔÄ»Øµ÷½Ó¿Ú£¬ÉÏ²ãÓ¦ÓÃÍ¨¹ýÕâ¸ö½Ó¿Ú£¬½ÓÊÕÖ÷ÍÆ¹ýÀ´µÄÏûÏ¢
 */
class CSubCallbackInterface: public IKnown
{
public:

   /**
    * ÊÕµ½·¢²¼ÏûÏ¢µÄ»Øµ÷
    * @param lpSub »Øµ÷µÄ¶©ÔÄÖ¸Õë
    * @param subscribeIndex ÏûÏ¢¶ÔÓ¦µÄ¶©ÔÄ±êÊ¶£¬Õâ¸ö±êÊ¶À´×ÔÓÚSubscribeTopicº¯ÊýµÄ·µ»Ø
    * @param lpData ·µ»ØÏûÏ¢µÄ¶þ½øÖÆÖ¸Õë£¬Ò»°ãÊÇÏûÏ¢µÄÒµÎñÌå´ò°üÄÚÈÝ
    * @param nLength ¶þ½øÖÆÊý¾ÝµÄ³¤¶È
	* @param lpRecvData Ö÷ÍÆÏûÏ¢µÄÆäËû×Ö¶Î·µ»Ø£¬Ö÷Òª°üº¬ÁË¸½¼ÓÊý¾Ý£¬¹ýÂËÐÅÏ¢£¬Ö÷ÌâÃû×Ö£¬ÏêÏ¸²Î¿´Ç°Ãæ½á¹¹Ìå¶¨Òå
    * @return ÎÞ
    */
    virtual void FUNCTION_CALL_MODE OnReceived(CSubscribeInterface *lpSub,int subscribeIndex, const void *lpData, int nLength,LPSUBSCRIBE_RECVDATA lpRecvData) = 0;

   /**
    * ÊÕµ½ÌÞ³ý¶©ÔÄÏîµÄÏûÏ¢»Øµ÷£¬Ò»°ãÔÚÓµÓÐÌßÈË²ßÂÔµÄÖ÷ÌâÏÂ»á»Øµ÷Õâ¸ö½Ó¿Ú,Õâ¸ö»Øµ÷ÀïÃæ²»ÐèÒªÈ¡Ïû¶©ÔÄ£¬µ×²ãÒÑ¾­È¡ÏûÕâ¸ö¶©ÔÄ£¬Ö»ÊÇÒ»¸öÍ¨Öª½Ó¿Ú
    * @param lpSub »Øµ÷µÄ¶©ÔÄÖ¸Õë
    * @param subscribeIndex ÏûÏ¢¶ÔÓ¦µÄ¶©ÔÄ±êÊ¶£¬Õâ¸ö±êÊ¶À´×ÔÓÚSubscribeTopicº¯ÊýµÄ·µ»Ø
    * @param TickMsgInfo ÌßÈËµÄ´íÎóÐÅÏ¢£¬Ö÷ÒªÊÇ°üº¬¾ßÌåÖØ¸´µÄ¶©ÔÄÏîÎ»ÖÃÐÅÏ¢
    * @return ÎÞ
    */
    virtual void FUNCTION_CALL_MODE OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo) = 0;
};


/**
 * ¶©ÔÄ½Ó¿ÚµÄ¶¨Òå
 */
class CSubscribeInterface: public IKnown
{
public:

   /**
    * ¶©ÔÄÖ÷Ìâ
    * @param lpSubscribeParamInter ÉÏÃæ¶¨ÒåµÄ¶©ÔÄ²ÎÊý½á¹¹
    * @param uiTimeout ³¬Ê±Ê±¼ä
    * @param lppBizUnPack ÒµÎñÐ£ÑéÊ±£¬Ê§°Ü·µ»ØµÄÒµÎñ´íÎóÐÅÏ¢£¬Èç¹û¶©ÔÄ³É¹¦Ã»ÓÐ·µ»Ø£¬Êä³ö²ÎÊý£¬ÐèÒªÍâÃæµ÷ÓÃReleaseÊÍ·Å
                          Èç¹û½ÓÊÜÒµÎñÐ£ÑéµÄ´íÎóÐÅÏ¢£¬Ð´·¨ÈçÏÂ£º
                          IF2UnPacker* lpBizUnPack =NULL;
                          SubscribeTopic(...,&lpBizUnPack);
                          ×îºó¸ù¾Ý·µ»ØÖµ£¬Èç¹ûÊÇÊ§°ÜµÄ¾ÍÅÐ¶Ï lpBizUnPack ÊÇ²»ÊÇNULL.
                          ×îºó´íÎóÐÅÏ¢»ñÈ¡ÍêÖ®ºó,ÊÍ·Å
                          lpBizUnPack->Release();
    * @param lpBizPack ÒµÎñÐ£ÑéÐèÒªÔö¼ÓµÄÒµÎñ×Ö¶ÎÒÔ¼°Öµ£¬Ã»ÓÐ¾Í¸ù¾Ý¹ýÂËÊôÐÔ×÷ÎªÒµÎñÐ£Ñé×Ö¶Î
    * @return ·µ»ØÖµ´óÓÚ0£¬±íÊ¾µ±Ç°¶©ÔÄ³É¹¦µÄ¶©ÔÄ±êÊ¶£¬ÍâÃæÒª¼Ç×¡Õâ¸ö±êÊ¶ºÍ¶©ÔÄÏîÖ®¼äµÄÓ³Éä¹ØÏµ£¬Õâ¸ö±êÊ¶ÐèÒªÓÃÓÚÈ¡Ïû¶©ÔÄºÍ½ÓÊÕÏûÏ¢µÄ»Øµ÷ÀïÃæ.
	*		  ·µ»ØÆäËûÖµ£¬¸ù¾Ý´íÎóºÅ»ñÈ¡´íÎóÐÅÏ¢.
    */
    virtual int FUNCTION_CALL_MODE SubscribeTopic(CSubscribeParamInterface* lpSubscribeParamInter ,unsigned int uiTimeout,IF2UnPacker** lppBizUnPack=NULL,IF2Packer* lpBizPack=NULL) = 0;

   /**
    * È¡Ïû¶©ÔÄÖ÷Ìâ
    * @param subscribeIndex ÏûÏ¢¶ÔÓ¦µÄ¶©ÔÄ±êÊ¶£¬Õâ¸ö±êÊ¶À´×ÔÓÚSubscribeTopicº¯ÊýµÄ·µ»Ø
    * @return ·µ»Ø0±íÊ¾È¡Ïû¶©ÔÄ³É¹¦£¬·µ»ØÆäËûÖµ£¬¸ù¾Ý´íÎóºÅ»ñÈ¡´íÎóÐÅÏ¢.
    */
    virtual int FUNCTION_CALL_MODE CancelSubscribeTopic(int subscribeIndex) = 0;


	/**
    * È¡Ïû¶©ÔÄÖ÷Ìâ
    * @param topicName Ö÷ÌâÃû×Ö
    * @param lpFilterInterface ¶ÔÓ¦µÄ¹ýÂËÌõ¼þ
    * @return ·µ»Ø0±íÊ¾È¡Ïû¶©ÔÄ³É¹¦£¬·µ»ØÆäËûÖµ£¬¸ù¾Ý´íÎóºÅ»ñÈ¡´íÎóÐÅÏ¢.
    */
    virtual int FUNCTION_CALL_MODE CancelSubscribeTopicEx(char* topicName,CFilterInterface* lpFilterInterface) = 0;



   /**
    * »ñÈ¡µ±Ç°¶©ÔÄ½Ó¿ÚÒÑ¾­¶©ÔÄµÄËùÓÐÖ÷ÌâÒÔ¼°¹ýÂËÌõ¼þÐÅÏ¢
    * @param lpPack ÍâÃæ´«ÈëµÄ´ò°üÆ÷
    * @note packer±¨ÎÄ×Ö¶Î
        - SubcribeIndex
        - IsBornTopic
        - TopicName
        - TopicNo
        - FilterRaw
        - Appdata
        - SendInterval
        - ReturnFileds
        - isReplace
        - isFromNow
    */
    virtual void FUNCTION_CALL_MODE GetSubcribeTopic(IF2Packer* lpPack)=0;
};

/**
 * ·¢²¼½Ó¿Ú
 */
class CPublishInterface: public IKnown
{
public:

   /**
    * ÒµÎñ´ò°ü¸ñÊ½µÄÄÚÈÝ·¢²¼½Ó¿Ú
    * @param topicName Ö÷ÌâÃû×Ö£¬²»ÖªµÀÃû×Ö¾Í´«NULL
    * @param lpUnPacker ¾ßÌåµÄÄÚÈÝ
    * @param iTimeOut ³¬Ê±Ê±¼ä
    * @param lppBizUnPack ÒµÎñÐ£ÑéÊ±£¬Ê§°Ü·µ»ØµÄÒµÎñ´íÎóÐÅÏ¢£¬Èç¹û·¢²¼³É¹¦Ã»ÓÐ·µ»Ø£¬Êä³ö²ÎÊý£¬ÐèÒªÍâÃæµ÷ÓÃReleaseÊÍ·Å
                            Èç¹û½ÓÊÜÒµÎñÐ£ÑéµÄ´íÎóÐÅÏ¢£¬Ð´·¨ÈçÏÂ£º
                            IF2UnPacker* lpBizUnPack =NULL;
                            PubMsgByPacker(...,&lpBizUnPack);
                            ×îºó¸ù¾Ý·µ»ØÖµ£¬Èç¹ûÊÇÊ§°ÜµÄ¾ÍÅÐ¶Ï lpBizUnPack ÊÇ²»ÊÇNULL.
                            ×îºó´íÎóÐÅÏ¢»ñÈ¡ÍêÖ®ºó,ÊÍ·Å
                            lpBizUnPack->Release();
    * @param bAddTimeStamp ÊÇ·ñÌí¼ÓÊ±¼ä´Á£¬ÅäºÏµ¥±ÊÐÔÄÜ²éÕÒ
    * @return ·µ»Ø0±íÊ¾³É¹¦£¬·µ»ØÆäËûÖµ£¬¸ù¾Ý´íÎóºÅ»ñÈ¡´íÎóÐÅÏ¢.
    */
    virtual int FUNCTION_CALL_MODE PubMsgByPacker(char* topicName ,IF2UnPacker* lpUnPacker,int iTimeOut=-1,
        IF2UnPacker** lppBizUnPack=NULL,bool bAddTimeStamp=false) = 0;
    
   /**
    * ·ÇÒµÎñ´ò°ü¸ñÊ½µÄÄÚÈÝ·¢²¼½Ó¿Ú£¬Ò»°ã¶þ½øÖÆ¸ñÊ½±¨ÎÄ·¢²¼
    * @param topicName Ö÷ÌâÃû×Ö£¬²»ÖªµÀÃû×Ö¾Í´«NULL
    * @param lpFilterInterface ¹ýÂËÌõ¼þ£¬ÐèÒªÉÏ²ã×Ô¼ºÖ¸¶¨£¬·ñÔòÄ¬ÈÏÃ»ÓÐ¹ýÂËÌõ¼þ
    * @param lpData ¾ßÌåµÄÄÚÈÝ
    * @param nLength ÄÚÈÝ³¤¶È
    * @param iTimeOut ³¬Ê±Ê±¼ä
    * @param lppBizUnPack ÒµÎñÐ£ÑéÊ±£¬Ê§°Ü·µ»ØµÄÒµÎñ´íÎóÐÅÏ¢£¬Èç¹û·¢²¼³É¹¦Ã»ÓÐ·µ»Ø£¬Êä³ö²ÎÊý£¬ÐèÒªÍâÃæµ÷ÓÃReleaseÊÍ·Å
                            Èç¹û½ÓÊÜÒµÎñÐ£ÑéµÄ´íÎóÐÅÏ¢£¬Ð´·¨ÈçÏÂ£º
                            IF2UnPacker* lpBizUnPack =NULL;
                            PubMsgByPacker(...,&lpBizUnPack);
                            ×îºó¸ù¾Ý·µ»ØÖµ£¬Èç¹ûÊÇÊ§°ÜµÄ¾ÍÅÐ¶Ï lpBizUnPack ÊÇ²»ÊÇNULL.
                            ×îºó´íÎóÐÅÏ¢»ñÈ¡ÍêÖ®ºó,ÊÍ·Å
                            lpBizUnPack->Release();
    * @param bAddTimeStamp ÊÇ·ñÌí¼ÓÊ±¼ä´Á£¬ÅäºÏµ¥±ÊÐÔÄÜ²éÕÒ
    * @return ·µ»Ø0±íÊ¾³É¹¦£¬·µ»ØÆäËûÖµ£¬¸ù¾Ý´íÎóºÅ»ñÈ¡´íÎóÐÅÏ¢.
    */
    virtual int FUNCTION_CALL_MODE PubMsg(char* topicName, CFilterInterface* lpFilterInterface, const void *lpData, int nLength,int iTimeOut=-1,
        IF2UnPacker** lppBizUnPack=NULL,bool bAddTimeStamp=false) = 0;

   /**
    * ·µ»Øµ±Ç°Ö÷ÌâµÄ·¢²¼ÐòºÅ
    * @param topicName Ö÷ÌâÃû×Ö
    * @return ·µ»Ø0±íÊ¾Ã»ÓÐ¶ÔÓ¦µÄÖ÷Ìâ£¬·µ»ØÆäËûÖµ±íÊ¾³É¹¦
    */
    virtual uint32 FUNCTION_CALL_MODE GetMsgNoByTopicName(char* topicName)=0;

   /**
    * È¡·þÎñÆ÷µØÖ·
    * @param lpPort Êä³öµÄ·þÎñÆ÷¶Ë¿Ú£¬¿ÉÒÔÎªNULL
    * @return ·µ»Ø·þÎñÆ÷µØÖ·
    */
    virtual const char * FUNCTION_CALL_MODE GetServerAddress(int *lpPort) = 0;
};

class CConnectionInterface;

///Á¬½Ó¶ÔÏó CConnectionInterface ÐèÒªµÄ»Øµ÷¶ÔÏó½Ó¿Ú¶¨Òå
/**
* °üÀ¨Á¬½Ó³É¹¦¡¢Á¬½Ó¶Ï¿ª¡¢·¢ËÍÍê³É¡¢ÊÕµ½Êý¾ÝµÈÊÂ¼þ    
*/
class CCallbackInterface: public IKnown
{
public:
    /**
    * Ì×½Ó×ÖÁ¬½Ó³É¹¦
    * @param lpConnection ·¢Éú¸ÃÊÂ¼þµÄÁ¬½Ó¶ÔÏó
    */
    virtual void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection) = 0;

    /**
    * Íê³É°²È«Á¬½Ó
    * @param lpConnection ·¢Éú¸ÃÊÂ¼þµÄÁ¬½Ó¶ÔÏó
    */
    virtual void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection) = 0;

    /**
    * Íê³É×¢²á
    * @param lpConnection ·¢Éú¸ÃÊÂ¼þµÄÁ¬½Ó¶ÔÏó
    */
    virtual void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection) = 0;

    /**
    * Á¬½Ó±»¶Ï¿ª
    * @param lpConnection ·¢Éú¸ÃÊÂ¼þµÄÁ¬½Ó¶ÔÏó
    */
    virtual void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection) = 0;

    /**
    * ·¢ËÍÍê³É
    * @param lpConnection ·¢Éú¸ÃÊÂ¼þµÄÁ¬½Ó¶ÔÏó
    * @param hSend        ·¢ËÍ¾ä±ú
    * @param reserved1    ±£Áô×Ö¶Î
    * @param reserved2    ±£Áô×Ö¶Î
    * @param nQueuingData ·¢ËÍ¶ÓÁÐÖÐÊ£Óà¸öÊý£¬Ê¹ÓÃÕß¿ÉÒÔÓÃ´ËÊýÖµ¿ØÖÆ·¢ËÍµÄËÙ¶È£¬¼´Ð¡ÓÚÄ³ÖµÊ±½øÐÐ·¢ËÍ
    * @see Send()
    */
    virtual void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData) = 0;

    virtual void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d) = 0;

    virtual void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d) = 0;

    virtual int  FUNCTION_CALL_MODE Reserved3() = 0;

    virtual void FUNCTION_CALL_MODE Reserved4() = 0;

    virtual void FUNCTION_CALL_MODE Reserved5() = 0;

    virtual void FUNCTION_CALL_MODE Reserved6() = 0;

    virtual void FUNCTION_CALL_MODE Reserved7() = 0;

    /**
    * ÊÕµ½SendBizÒì²½·¢ËÍµÄÇëÇóµÄÓ¦´ð
    * @param lpConnection    ·¢Éú¸ÃÊÂ¼þµÄÁ¬½Ó¶ÔÏó
    * @param hSend           ·¢ËÍ¾ä±ú
    * @param lpUnPackerOrStr Ö¸Ïò½â°üÆ÷Ö¸Õë»òÕß´íÎóÐÅÏ¢
    * @param nResult         ÊÕ°ü½á¹û
    * Èç¹ûnResultµÈÓÚ0£¬±íÊ¾ÒµÎñÊý¾Ý½ÓÊÕ³É¹¦£¬²¢ÇÒÒµÎñ²Ù×÷³É¹¦£¬lpUnPackerOrStrÖ¸ÏòÒ»¸ö½â°üÆ÷£¬´ËÊ±Ó¦Ê×ÏÈ½«¸ÃÖ¸Õë×ª»»ÎªIF2UnPacker *¡£
    * Èç¹ûnResultµÈÓÚ1£¬±íÊ¾ÒµÎñÊý¾Ý½ÓÊÕ³É¹¦£¬µ«ÒµÎñ²Ù×÷Ê§°ÜÁË£¬lpUnPackerOrStrÖ¸ÏòÒ»¸ö½â°üÆ÷£¬´ËÊ±Ó¦Ê×ÏÈ½«¸ÃÖ¸Õë×ª»»ÎªIF2UnPacker *¡£
    * Èç¹ûnResultµÈÓÚ2£¬±íÊ¾ÊÕµ½·ÇÒµÎñ´íÎóÐÅÏ¢£¬lpUnPackerOrStrÖ¸ÏòÒ»¸ö¿É¶ÁµÄ×Ö·û´®´íÎóÐÅÏ¢¡£
    * Èç¹ûnResultµÈÓÚ3£¬±íÊ¾ÒµÎñ°ü½â°üÊ§°Ü¡£lpUnPackerOrStrÖ¸ÏòNULL¡£
    */
    virtual void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult) = 0;

		/**
    * ÊÕµ½SendBizÒì²½·¢ËÍµÄÇëÇóµÄÓ¦´ð
    * @param lpConnection    ·¢Éú¸ÃÊÂ¼þµÄÁ¬½Ó¶ÔÏó
    * @param hSend           ·¢ËÍ¾ä±ú
    * @param lpRetData ÆäËûÐèÒª·µ»ØµÄÓ¦´ðÄÚÈÝ£¬¸ù¾ÝÐèÒª»ñÈ¡
    * @param lpUnPackerOrStr Ö¸Ïò½â°üÆ÷Ö¸Õë»òÕß´íÎóÐÅÏ¢
    * @param nResult         ÊÕ°ü½á¹û
    * Èç¹ûnResultµÈÓÚ0£¬±íÊ¾ÒµÎñÊý¾Ý½ÓÊÕ³É¹¦£¬²¢ÇÒÒµÎñ²Ù×÷³É¹¦£¬lpUnpackerOrStrÖ¸ÏòÒ»¸ö½â°üÆ÷£¬´ËÊ±Ó¦Ê×ÏÈ½«¸ÃÖ¸Õë×ª»»ÎªIF2UnPacker *¡£
    * Èç¹ûnResultµÈÓÚ1£¬±íÊ¾ÒµÎñÊý¾Ý½ÓÊÕ³É¹¦£¬µ«ÒµÎñ²Ù×÷Ê§°ÜÁË£¬lpUnpackerOrStrÖ¸ÏòÒ»¸ö½â°üÆ÷£¬´ËÊ±Ó¦Ê×ÏÈ½«¸ÃÖ¸Õë×ª»»ÎªIF2UnPacker *¡£
    * Èç¹ûnResultµÈÓÚ2£¬±íÊ¾ÊÕµ½·ÇÒµÎñ´íÎóÐÅÏ¢£¬lpUnpackerOrStrÖ¸ÏòÒ»¸ö¿É¶ÁµÄ×Ö·û´®´íÎóÐÅÏ¢¡£
    * Èç¹ûnResultµÈÓÚ3£¬±íÊ¾ÒµÎñ°ü½â°üÊ§°Ü¡£lpUnpackerOrStrÖ¸ÏòNULL¡£
    */
    virtual void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult) = 0;
	//20130624 xuxp »Øµ÷Ôö¼ÓBizMessage½Ó¿Ú
		/**
		* ÊÕµ½·¢ËÍÊ±Ö¸¶¨ÁËReplyCallbackÑ¡ÏîµÄÇëÇóµÄÓ¦´ð»òÕßÊÇÃ»ÓÐ¶ÔÓ¦ÇëÇóµÄÊý¾Ý
		* @param lpConnection ·¢Éú¸ÃÊÂ¼þµÄÁ¬½Ó¶ÔÏó
		* @param hSend        ·¢ËÍ¾ä±ú
		* @param lpMsg        ÒµÎñÏûÏ¢Ö¸Õë
		*/
	virtual void FUNCTION_CALL_MODE OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg) = 0;



};

///T2_SDKÁ¬½Ó¶ÔÏó½Ó¿Ú
/**
* Á¬½ÓµÄ¶ÔÏóÊÇÏß³Ì²»°²È«£¬Í¬Ê±Ò»¸öÁ¬½Ó×îºÃÊÇ¹éÓÚÒ»¸öÏß³ÌËùÓÃ£¬²»Òª¶àÏß³ÌÊ¹ÓÃ£¡£¡£¡
* °üÀ¨Á¬½Ó¡¢¶Ï¿ª¡¢·¢ËÍ¡¢½ÓÊÕµÈ
*/
class CConnectionInterface: public IKnown
{
public:
    ///Á¬½Ó×´Ì¬£¬¿É×éºÏ
    enum ConnectionStatus
    {
        Disconnected	= 0x0000, /**< Î´Á¬½Ó */
        Connecting		= 0x0001, /**< socketÕýÔÚÁ¬½Ó */
        Connected		= 0x0002, /**< socketÒÑÁ¬½Ó */
        SafeConnecting	= 0x0004, /**< ÕýÔÚ½¨Á¢°²È«Á¬½Ó */
        SafeConnected	= 0x0008, /**< ÒÑ½¨Á¢°²È«Á¬½Ó */
        Registering		= 0x0010, /**< Õý×¢²á */
        Registered		= 0x0020, /**< ÒÑ×¢²á */
        Rejected		= 0x0040  /**< ±»¾Ü¾ø£¬½«±»¹Ø±Õ */
    };

    ///½ÓÊÕÑ¡Ïî£¨¿É×éºÏ£¬0±íÊ¾½ÓÊÕ³¬Ê±Ê±£¬²»É¾³ý°üID£¬ÈÔ¿ÉÔÙ´Îµ÷ÓÃRecvBiz·½·¨À´³¢ÊÔ½ÓÊÕ£©
    enum RecvFlags
    {
        JustRemoveHandle = 0x0001   /**< µ±½ÓÊÕ³¬Ê±Ê±£¬°Ñpacket_idÉ¾³ý */
    };

    /**
    * ³õÊ¼»¯Á¬½Ó¶ÔÏó
    * @param lpCallback »Øµ÷¶ÔÏó
    * @return ·µ»Ø0±íÊ¾³É¹¦£¬·ñÔò±íÊ¾Ê§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
    * Èç¹ûÓ¦ÓÃ²»ÐèÒªÈÎºÎ»Øµ÷·½·¨£¬Ôò¿ÉÏò¸Ã·½·¨´«µÝNULL£¬¶ø²»±Ø×Ô¶¨Òå»Øµ÷ÀàºÍ¶ÔÏó
    */
    virtual int FUNCTION_CALL_MODE Create(CCallbackInterface *lpCallback) = 0;

    /**
    * ¿ªÊ¼Á¬½Ó/×¢²á
    * @param uiTimeout ³¬Ê±Ê±¼ä£¬µ¥Î»ºÁÃë£¬0±íÊ¾²»µÈ´ý£¨Ê¹ÓÃ´úÀíÁ¬½Ó·þÎñÆ÷Ê±£¬¸Ã²ÎÊý²»Æð×÷ÓÃ£©
    * @return ·µ»Ø0±íÊ¾³É¹¦£¬·ñÔò±íÊ¾Ê§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
    */
    virtual int FUNCTION_CALL_MODE Connect(unsigned int uiTimeout) = 0;

    /**
    * ¶Ï¿ªÁ¬½Ó
    * @return ·µ»Ø0±íÊ¾³É¹¦£¬·ñÔò±íÊ¾Ê§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
    */
    virtual int FUNCTION_CALL_MODE Close() = 0;

    virtual int FUNCTION_CALL_MODE Reserved1() = 0;

    virtual int FUNCTION_CALL_MODE Reserved2() = 0;

    virtual int FUNCTION_CALL_MODE Reserved3() = 0;

    virtual int FUNCTION_CALL_MODE Reserved4() = 0;

    virtual int FUNCTION_CALL_MODE Reserved5() = 0;

    virtual int FUNCTION_CALL_MODE Reserved6() = 0;

    /**
    * È¡·þÎñÆ÷µØÖ·
    * @param lpPort Êä³öµÄ·þÎñÆ÷¶Ë¿Ú£¬¿ÉÒÔÎªNULL
    * @return ·µ»Ø·þÎñÆ÷µØÖ·
    */
    virtual const char * FUNCTION_CALL_MODE GetServerAddress(int *lpPort) = 0;

    /**
    * È¡Á¬½Ó×´Ì¬
    * @return ·µ»ØÁ¬½Ó×´Ì¬
    */
    virtual int FUNCTION_CALL_MODE GetStatus() = 0;

    /**
    * È¡·þÎñÆ÷¸ºÔØ£¬Ê¹ÓÃÕß¿ÉÒÔÍ¬Ê±´´½¨¶à¸öÁ¬½ÓÊµÀýÍ¬Ê±Á¬½Ó²»Í¬µÄ·þÎñÆ÷£¬¸ù¾ÝÍê³ÉÁ¬½ÓµÄÊ±¼äÒÔ¼°¸ºÔØ¾ö¶¨Ê¹ÓÃÄÄ¸ö·þÎñÆ÷
    * @return ·µ»Ø·þÎñÆ÷¸ºÔØ£¨·Ç¸ºÊý£©£¬Ô½´ó±íÊ¾Ô½·±Ã¦£¬·ñÔò±íÊ¾Ê§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
    */
    virtual int FUNCTION_CALL_MODE GetServerLoad() = 0;

    /**
    * È¡´íÎóÂë¶ÔÓ¦µÄ´íÎóÐÅÏ¢£¬Ä¿Ç°Ö§³Ö¼òÌåÖÐÎÄºÍÓ¢ÎÄ£¬Ö§³ÖÆäËûÓïÑÔ¿ÉÒÔÍ¨¹ýÁ¬½Ó¶ÔÏóÅäÖÃerrormsg
    * @param nErrorCode ´íÎóÂë
    * @return ·µ»Ø´íÎóÐÅÏ¢
    */
    virtual const char * FUNCTION_CALL_MODE GetErrorMsg(int nErrorCode) = 0;

    /**
    * È¡Á¬½Ó´íÎóºÅ£¬µ±Á¬½ÓÎÞ·¨Óë·þÎñ¶ËÍê³É×¢²áÊ±£¬¼È¿ÉÍ¨¹ýConnectµÄ·µ»ØÖµ»ñÈ¡´íÎóºÅ£¬
    * Ò²¿ÉÍ¨¹ýµ÷ÓÃ±¾·½·¨À´»ñÈ¡´íÎóºÅ£¬È»ºóÓÃ¸Ã´íÎóºÅµ÷ÓÃGetErrorMsg¿É»ñÈ¡¿É¶ÁµÄ´íÎóÐÅÏ¢
    * @return ·µ»ØÁ¬½Ó´íÎóºÅ
    */
    virtual int FUNCTION_CALL_MODE GetConnectError() = 0;

    /**
    * ·¢ËÍÒµÎñÊý¾Ý
    * @param iFunID      ¹¦ÄÜºÅ
    * @param lpPacker    ´ò°üÆ÷Ö¸Õë
    * @param nAsy        0±íÊ¾Í¬²½£¬·ñÔò±íÊ¾Òì²½
    * @param iSystemNo   Èç¹ûiSystemNo > 0ÔòÉèÖÃÏµÍ³ºÅ
    * @param nCompressID ¶ÔÒµÎñ°üÌå½øÐÐÑ¹ËõµÄÑ¹ËõËã·¨ID£¬Ä¿Ç°Ö»Ö§³ÖID = 1µÄÑ¹ËõËã·¨¡£
    * ID = 0±íÊ¾²»Ñ¹Ëõ¡£×¢Òâ£¬Ñ¹ËõÖ»ÊÇÏòSDKÌá³ö½¨Òé£¬ÊÇ·ñÕæÕýÑ¹Ëõ»¹È¡¾öÓÚ°üµÄÊµ¼Ê´óÐ¡¡£
    * Í¬²½·¢ËÍµÄ°ü£¬Í¨¹ýµ÷ÓÃRecvBizÀ´½ÓÊÕ£¬Òì²½·¢ËÍµÄ°ü£¬µ±ÊÕµ½Ó¦´ð°üºó£¬×Ô¶¯´¥·¢»Øµ÷º¯ÊýOnReceivedBiz¡£
    * @return ·µ»Ø·¢ËÍ¾ä±ú£¨ÕýÊý£©£¬·ñÔò±íÊ¾Ê§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
    */
    virtual int FUNCTION_CALL_MODE SendBiz(int iFunID, IF2Packer *lpPacker, int nAsy = 0, int iSystemNo = 0, int nCompressID = 1) = 0;

    /**
    * ½ÓÊÕÒµÎñÊý¾Ý
    * @param hSend            ·¢ËÍ¾ä±ú£¨SendBizµÄ³É¹¦·µ»ØÖµ£©
    * @param lppUnPackerOrStr Èç¹û·µ»ØÖµµÈÓÚ0£¬±íÊ¾ÒµÎñÊý¾Ý½ÓÊÕ³É¹¦£¬²¢ÇÒÒµÎñ²Ù×÷³É¹¦£¬*lppUnPackerOrStrÖ¸ÏòÒ»¸ö½â°üÆ÷£¬´ËÊ±Ó¦Ê×ÏÈ½«¸ÃÖ¸Õë×ª»»ÎªIF2UnPacker *¡£
    *                         Èç¹û·µ»ØÖµµÈÓÚ1£¬±íÊ¾ÒµÎñÊý¾Ý½ÓÊÕ³É¹¦£¬µ«ÒµÎñ²Ù×÷Ê§°ÜÁË£¬*lppUnPackerOrStrÖ¸ÏòÒ»¸ö½â°üÆ÷£¬´ËÊ±Ó¦Ê×ÏÈ½«¸ÃÖ¸Õë×ª»»ÎªIF2UnPacker *¡£
    *                         Èç¹û·µ»ØÖµµÈÓÚ2£¬±íÊ¾ÊÕµ½·ÇÒµÎñ´íÎóÐÅÏ¢£¬*lppUnPackerOrStrÖ¸ÏòÒ»¸ö¿É¶ÁµÄ×Ö·û´®´íÎóÐÅÏ¢¡£
    *                         Èç¹û·µ»ØÖµµÈÓÚ3£¬±íÊ¾ÒµÎñ°ü½â°üÊ§°Ü¡£*lppUnPackerOrStrÔ­ÏÈËùÖ¸ÏòµÄÄÚÈÝ²»»á±»¸Ä±ä¡£
    * @param uiTimeout        ³¬Ê±Ê±¼ä£¬µ¥Î»ºÁÃë¡£
    * @param uiFlag           ½ÓÊÕÑ¡Ïî£¬0±íÊ¾½ÓÊÕ³¬Ê±ºóÈÔ¿É¼ÌÐøµ÷ÓÃRecvBizÀ´½ÓÊÕ£¬
    *                         JustRemoveHandle±íÊ¾µ±½ÓÊÕ³¬Ê±ºó£¬°ÑhSendÏà¹ØÊý¾ÝÉ¾³ý
    * @return Ð¡ÓÚ0±íÊ¾RecvBiz²Ù×÷±¾ÉíÊ§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
    * ×¢Òâ£¡Íâ²¿Ö¸ÕëËùÖ¸ÏòµÄ½â°üÆ÷µÄÄÚ´æÓÉSDKÄÚ²¿¹ÜÀí£¬Íâ²¿ÇÐÎðÊÍ·Å£¡
	* ×¢Òâ£¡lppUnPackerOrStr¶ÔÓ¦µÄ½â°üÆ÷ÊÇÁÙÊ±µÄ£¬ÉÏ²ã²»¿ÉÒÔ»º´æÖ¸Õë£¬ÔÙ´Îµ÷ÓÃÕâ¸öÁ¬½ÓµÄRecvBiz£¬Ö¸ÕëÖ¸ÏòµÄÄÚÈÝ¾Í»á¸Ä±ä
	* Èç¹ûÒª¿½±´£¬ÐèÒªµ÷ÓÃ½â°üÆ÷µÄGetPackBuf·½·¨£¬¶þ½øÖÆ¿½±´³öÈ¥£¬ÆäËûÏß³ÌÐèÒªÔÙ½â°ü
    */
    virtual int FUNCTION_CALL_MODE RecvBiz(int hSend, void **lppUnPackerOrStr, unsigned uiTimeout = 1000, unsigned uiFlag = 0) = 0;
    
   //20101228 xuxp ÐÂÔö·¢ËÍºÍ½ÓÊÜµÄÁ½¸ö½Ó¿Úº¯Êý£¬ÓÃÓÚ¶©ÔÄºÍÍÆËÍ
	/**
    * ·¢ËÍÒµÎñÊý¾Ý
    * @param iFunID      ¹¦ÄÜºÅ
    * @param lpPacker    ´ò°üÆ÷Ö¸Õë
	* @param svrName     Ö¸¶¨ÖÐ¼ä¼þµÄ½Úµã
    * @param nAsy        0±íÊ¾Í¬²½£¬·ñÔò±íÊ¾Òì²½¡£
    * @param iSystemNo   Èç¹ûiSystemNo > 0ÔòÉèÖÃÏµÍ³ºÅ
    * @param nCompressID ¶ÔÒµÎñ°üÌå½øÐÐÑ¹ËõµÄÑ¹ËõËã·¨ID£¬Ä¿Ç°Ö»Ö§³ÖID = 1µÄÑ¹ËõËã·¨¡£
    * ID = 0±íÊ¾²»Ñ¹Ëõ¡£×¢Òâ£¬Ñ¹ËõÖ»ÊÇÏòSDKÌá³ö½¨Òé£¬ÊÇ·ñÕæÕýÑ¹Ëõ»¹È¡¾öÓÚ°üµÄÊµ¼Ê´óÐ¡¡£
    * Í¬²½·¢ËÍµÄ°ü£¬Í¨¹ýµ÷ÓÃRecvBizExÀ´½ÓÊÕ£¬Òì²½·¢ËÍµÄ°ü£¬µ±ÊÕµ½Ó¦´ð°üºó£¬×Ô¶¯´¥·¢»Øµ÷º¯ÊýOnReceivedBizEx¡£
    * @param branchNo  ÓªÒµ²¿ºÅ¡£
    * @param lpRequest  ÇëÇóÀïÃæµÄÆäËûÄÚÈÝ£¬¸ù¾Ý½á¹¹Ìå¶¨Òå¸³Öµ¡£
    * @return ·µ»Ø·¢ËÍ¾ä±ú£¨ÕýÊý£©£¬·ñÔò±íÊ¾Ê§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
    */
    virtual int FUNCTION_CALL_MODE SendBizEx(int iFunID, IF2Packer *lpPacker,char* svrName, int nAsy = 0, int iSystemNo = 0, int nCompressID = 1,int branchNo=0,LPREQ_DATA lpRequest=NULL) = 0;
    
    /**
    * ½ÓÊÕÒµÎñÊý¾Ý
    * @param hSend            ·¢ËÍ¾ä±ú£¨SendBizµÄ³É¹¦·µ»ØÖµ£©
    * @param lppUnPackerOrStr Èç¹û·µ»ØÖµµÈÓÚ0£¬±íÊ¾ÒµÎñÊý¾Ý½ÓÊÕ³É¹¦£¬²¢ÇÒÒµÎñ²Ù×÷³É¹¦£¬*lppUnPackerOrStrÖ¸ÏòÒ»¸ö½â°üÆ÷£¬´ËÊ±Ó¦Ê×ÏÈ½«¸ÃÖ¸Õë×ª»»ÎªIF2UnPacker *¡£
    *                         Èç¹û·µ»ØÖµµÈÓÚ1£¬±íÊ¾ÒµÎñÊý¾Ý½ÓÊÕ³É¹¦£¬µ«ÒµÎñ²Ù×÷Ê§°ÜÁË£¬*lppUnPackerOrStrÖ¸ÏòÒ»¸ö½â°üÆ÷£¬´ËÊ±Ó¦Ê×ÏÈ½«¸ÃÖ¸Õë×ª»»ÎªIF2UnPacker *¡£
    *                         Èç¹û·µ»ØÖµµÈÓÚ2£¬±íÊ¾ÊÕµ½·ÇÒµÎñ´íÎóÐÅÏ¢£¬*lppUnPackerOrStrÖ¸ÏòÒ»¸ö¿É¶ÁµÄ×Ö·û´®´íÎóÐÅÏ¢¡£
    *                         Èç¹û·µ»ØÖµµÈÓÚ3£¬±íÊ¾ÒµÎñ°ü½â°üÊ§°Ü¡£*lppUnPackerOrStrÔ­ÏÈËùÖ¸ÏòµÄÄÚÈÝ²»»á±»¸Ä±ä¡£
    * @param lpRetData ÆäËûÐèÒª·µ»ØµÄÓ¦´ðÄÚÈÝ£¬¸ù¾ÝÐèÒª»ñÈ¡
    * @param uiTimeout        ³¬Ê±Ê±¼ä£¬µ¥Î»ºÁÃë£¬0±íÊ¾²»µÈ´ý¡£
    * @param uiFlag           ½ÓÊÕÑ¡Ïî£¬0±íÊ¾½ÓÊÕ³¬Ê±ºóÈÔ¿É¼ÌÐøµ÷ÓÃRecvBizÀ´½ÓÊÕ£¬
    *                         JustRemoveHandle±íÊ¾µ±½ÓÊÕ³¬Ê±ºó£¬°ÑhSendÏà¹ØÊý¾ÝÉ¾³ý
    * @return Ð¡ÓÚ0±íÊ¾RecvBizEx²Ù×÷±¾ÉíÊ§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
    * ×¢Òâ£¡Íâ²¿Ö¸ÕëËùÖ¸ÏòµÄ½â°üÆ÷µÄÄÚ´æÓÉSDKÄÚ²¿¹ÜÀí£¬Íâ²¿ÇÐÎðÊÍ·Å£¡
	* ×¢Òâ£¡Íâ²¿Ö¸ÕëËùÖ¸ÏòµÄLPRET_DATAµÄÄÚ´æÓÉSDKÄÚ²¿¹ÜÀí£¬Íâ²¿ÇÐÎðÊÍ·Å£¡
	* ×¢Òâ£¡lppUnPackerOrStr¶ÔÓ¦µÄ½â°üÆ÷ºÍLPRET_DATA¶¼ÊÇÁÙÊ±µÄ£¬ÉÏ²ã²»¿ÉÒÔ»º´æÖ¸Õë£¬ÔÙ´Îµ÷ÓÃÕâ¸öÁ¬½ÓµÄRecvBizEx£¬ÕâÁ½¸öÖ¸ÕëÖ¸ÏòµÄÄÚÈÝ¾Í»á¸Ä±ä
	* Èç¹ûÒª¿½±´£¬ÐèÒªµ÷ÓÃ½â°üÆ÷µÄGetPackBuf·½·¨£¬¶þ½øÖÆ¿½±´³öÈ¥£¬ÆäËûÏß³ÌÐèÒªÔÙ½â°ü£»
	* LPRET_DATA¿½±´£¬¾ÍÐèÒª×ö½á¹¹Ìå¸´ÖÆ
    */
    virtual int FUNCTION_CALL_MODE RecvBizEx(int hSend, void **lppUnpackerOrStr, LPRET_DATA* lpRetData, unsigned uiTimeout = 1000, unsigned uiFlag = 0) = 0;


	//20101228 xuxp ÐÂÔöCreateº¯Êý£¬À´±£Ö¤»Øµ÷ÓÃÐÂµÄOnReceivedBizExµÄ½Ó¿Ú
	/**
	* ³õÊ¼»¯Á¬½Ó¶ÔÏó
	* @param lpCallback »Øµ÷¶ÔÏó
	* @return ·µ»Ø0±íÊ¾³É¹¦£¬·ñÔò±íÊ¾Ê§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
    * Èç¹ûÓ¦ÓÃ²»ÐèÒªÈÎºÎ»Øµ÷·½·¨£¬Ôò¿ÉÏò¸Ã·½·¨´«µÝNULL£¬¶ø²»±Ø×Ô¶¨Òå»Øµ÷ÀàºÍ¶ÔÏó
	*/
	virtual int FUNCTION_CALL_MODE CreateEx(CCallbackInterface *lpCallback) = 0;
	
	
	//20120111 dongpf ÐÂÔöGetRealAddressº¯Êý£¬À´»ñÈ¡·þÎñ¶ËÉÏ×Ô¼ºµÄipµØÖ·ºÍ¶Ë¿Ú
	/**
	* »ñÈ¡ipµØÖ·ºÍ¶Ë¿Ú
	* @return »ñÈ¡ipµØÖ·ºÍ¶Ë¿Ú£¬¸ñÊ½£ºipµØÖ·+¶Ë¿Ú
	*/
	virtual const char* FUNCTION_CALL_MODE GetRealAddress() = 0;
	
	
	virtual int FUNCTION_CALL_MODE Reserved8() = 0;
	virtual int FUNCTION_CALL_MODE Reserved9() = 0;
	
	//20130527 xuxp ÐÂÔöGetSelfAddressº¯Êý£¬À´»ñÈ¡×Ô¼º±¾µØµÄIPºÍ¶Ë¿Ú
	/**
	* »ñÈ¡ipµØÖ·ºÍ¶Ë¿Ú
	* @return »ñÈ¡ipµØÖ·ºÍ¶Ë¿Ú£¬¸ñÊ½£ºipµØÖ·+¶Ë¿Ú
	*/
	virtual const char* FUNCTION_CALL_MODE GetSelfAddress() = 0;

	//20130529 xuxp ÐÂÔöGetSelfMacº¯Êý£¬À´»ñÈ¡×Ô¼º±¾µØÊ¹ÓÃµÄÍø¿¨MAC
	/**
	* »ñÈ¡MACµØÖ·
	* @return MACµÄµØÖ·×Ö·û´®¸ñÊ½£¬ÀàËÆ¡°D067E5556D83¡±,ÖÐ¼äÃ»ÓÐ·Ö¸ô·û
	*/
	virtual const char* FUNCTION_CALL_MODE GetSelfMac() = 0;
	
	//20130609 xuxp ÐÂÔö¶©ÔÄ·¢²¼½Ó¿Ú
	///////////////////////////////////ÏÂÃæÔö¼Ó¶©ÔÄ·¢²¼µÄ½Ó¿Ú///////////////////////////////////////

	/**
    * ´´½¨Ò»¸ö¶©ÔÄÕß
    * @param lpCallback »Øµ÷½Ó¿Ú
    * @param SubScribeName ¶©ÔÄÕßÃû×Ö£¬¶à¶©ÔÄÕßµÄÃû×Ö±ØÐë²»Ò»Ñù£¬²»¿ÉÒÔÏàÍ¬.×î´ó³¤¶È32¸ö×Ö½Ú
    * @param iTimeOut ³¬Ê±Ê±¼ä
    * @param iInitRecvQLen ³õÊ¼»¯½ÓÊÕ¶ÓÁÐµÄ³¤¶È
    * @param iStepRecvQLen ½ÓÊÜ¶ÓÁÐµÄÀ©Õ¹²½³¤
    * @return ·µ»Ø¶©ÔÄ½Ó¿ÚÊµÀý£¬Ò»¸ö»á»°½Ó¿Ú¶ÔÓ¦Ò»¸ö»Øµ÷.
    */
    virtual CSubscribeInterface* FUNCTION_CALL_MODE NewSubscriber(CSubCallbackInterface *lpCallback,char* SubScribeName,int iTimeOut,
        int iInitRecvQLen=INIT_RECVQ_LEN,int iStepRecvQLen=STEP_RECVQ_LEN) = 0;
    
   /**
    * »ñÈ¡·¢²¼Õß
    * @param PublishName ·¢²¼ÕßÒµÎñÃû
    * @param msgCount ±¾µØ»º´æÏûÏ¢µÄ¸öÊý
	  * @param iTimeOut ³õÊ¼»¯µÄÊ±ºòµÄ³¬Ê±Ê±¼ä
    * @param bResetNo ÊÇ·ñÖØÖÃÐò
    * @return ·µ»Ø·¢ËÍ½Ó¿ÚÊµÀý£¬·µ»Ø¶ÔÓ¦µÄÖ¸Õë
    */
    //virtual CPublishInterface* FUNCTION_CALL_MODE GetPublisher(int msgCount,int iTimeOut,bool bResetNo = false) = 0;
    virtual CPublishInterface* FUNCTION_CALL_MODE NewPublisher(char* PublishName,int msgCount,int iTimeOut,bool bResetNo = false) = 0;
    
  


   /**
    * »ñÈ¡·þÎñ¶ËµÄËùÓÐÖ÷ÌâÐÅÏ¢
    * @param byForce ÊÇ·ñÇ¿ÖÆ´ÓºóÌ¨»ñÈ¡
    * @param iTimeOut ³¬Ê±Ê±¼ä
    * @return ³É¹¦¾Í·µ»ØËùÓÐÖ÷ÌâÐÅÏ¢
    * @note ½â°üÆ÷ÍâÃæÐèÒªµ÷ÓÃrelease½Ó¿Ú½øÐÐÊÍ·Å.
    * @note packer·µ»Ø×Ö¶Î
    * - TopicName
    * - TopicNo
    * - ReliableLevel
    * - IssuePriority
    * - MsgLifetime
    * - Stutas 
    * - TickStrategy 
    * - BusinessVerify
    * - Local
    * - FilterField1 
    * - FilterField2 
    * - FilterField3 
    * - FilterField4 
    * - FilterField5 
    * - FilterField6
	* - FilterField7 
	* - FilterField8 
	* - FilterField9 
	* - FilterField10 
	* - FilterField11 
	* - FilterField12
	* - FilterField13 
	* - FilterField14 
	* - FilterField15 
	* - FilterField16
    * - SubscribeStr 
    * - PublishStr
	
    */
    virtual IF2UnPacker* FUNCTION_CALL_MODE GetTopic(bool byForce,int iTimeOut) = 0;
	
	/**
	* »ñÈ¡¶©ÔÄ·¢²¼µÄ×îºó´íÎó
	*/
	virtual const char* FUNCTION_CALL_MODE GetMCLastError() = 0;
	////////////////////////////////////////////////////////////////////////////////

	//20130624 xuxp Á¬½Ó½Ó¿ÚÔö¼ÓÏÂÃæÈý¸ö½Ó¿Ú£¬ÓÃÀ´×÷Îª·þÎñ¶ËµÄ½Ó¿Ú£¬¿Í»§¶Ë¿ª·¢Ò²ÍÆ¼öÊ¹ÓÃ
	///////////////////////////////////ÐÂµÄÒ»Ì×²Ù×÷½Ó¿Ú///////////////////////////////////////
	/**
	* ³õÊ¼»¯Á¬½Ó¶ÔÏó
	* @param lpCallback »Øµ÷¶ÔÏó
	* @return ·µ»Ø0±íÊ¾³É¹¦£¬·ñÔò±íÊ¾Ê§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
    * Èç¹ûÓ¦ÓÃ²»ÐèÒªÈÎºÎ»Øµ÷·½·¨£¬Ôò¿ÉÏò¸Ã·½·¨´«µÝNULL£¬¶ø²»±Ø×Ô¶¨Òå»Øµ÷ÀàºÍ¶ÔÏó
	*/
	virtual int FUNCTION_CALL_MODE Create2BizMsg(CCallbackInterface *lpCallback) = 0;

	/**
    * ·¢ËÍÒµÎñÊý¾Ý
    * @param lpMsg       ÒµÎñÏûÏ¢½Ó¿ÚÖ¸Õë
    * @param nAsy        0±íÊ¾Í¬²½£¬·ñÔò±íÊ¾Òì²½¡£
    * Í¬²½·¢ËÍµÄ°ü£¬Í¨¹ýµ÷ÓÃRecvBizMsgÀ´½ÓÊÕ£¬Òì²½·¢ËÍµÄ°ü£¬µ±ÊÕµ½Ó¦´ð°üºó£¬×Ô¶¯´¥·¢»Øµ÷º¯ÊýOnReceivedBizMsg¡£
    * @return ·µ»Ø·¢ËÍ¾ä±ú£¨ÕýÊý£©£¬·ñÔò±íÊ¾Ê§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
    */
	virtual int FUNCTION_CALL_MODE SendBizMsg(IBizMessage* lpMsg,int nAsy = 0) = 0;
	
	/**
	* ½ÓÊÕÊý¾Ý
	* @param hSend     ·¢ËÍ¾ä±ú
	* @param lpMsg	   ÊÕµ½ÒµÎñÏûÏ¢Ö¸ÕëµÄµØÖ·
	* @param uiTimeout ³¬Ê±Ê±¼ä£¬µ¥Î»ºÁÃë£¬0±íÊ¾²»µÈ´ý
	* @param uiFlag    ½ÓÊÕÑ¡Ïî£¬0±íÊ¾½ÓÊÕ³¬Ê±ºóÈÔ¿É¼ÌÐøµ÷ÓÃReceiveÀ´½ÓÊÕ£¬
    *                  JustRemoveHandle±íÊ¾µ±½ÓÊÕ³¬Ê±Ê±£¬°Ñpacket_idÉ¾³ý£¨ÒÔºóÔÙÊÕµ½£¬Ôò»áÒÔÒì²½µÄ·½Ê½ÊÕµ½£©
	* @return ·µ»Ø0±íÊ¾³É¹¦£¬·ñÔò±íÊ¾Ê§°Ü£¬Í¨¹ýµ÷ÓÃGetErrorMsg¿ÉÒÔ»ñÈ¡ÏêÏ¸´íÎóÐÅÏ¢
	* ×¢Òâ£¡Íâ²¿Ö¸ÕëËùÖ¸ÏòµÄIBizMessageµÄÄÚ´æÓÉSDKÄÚ²¿¹ÜÀí£¬Íâ²¿ÇÐÎðÊÍ·Å£¡
	* ×¢Òâ£¡lpMsg¶ÔÓ¦µÄÏûÏ¢Ö¸ÕëÊÇÁÙÊ±µÄ£¬ÉÏ²ã²»¿ÉÒÔ»º´æÖ¸Õë£¬ÔÙ´Îµ÷ÓÃÕâ¸öÁ¬½ÓµÄRecvBizMsg£¬Õâ¸öÖ¸ÕëÖ¸ÏòµÄÄÚÈÝ¾Í»á¸Ä±ä
	* Èç¹ûÒª¿½±´£¬ÐèÒªµ÷ÓÃIBizMessageµÄGetBuff·½·¨£¬¶þ½øÖÆ¿½±´³öÈ¥£¬ÆäËûÏß³ÌÐèÒªÔÙµ÷ÓÃSetBuff£»
	*/
	virtual int FUNCTION_CALL_MODE RecvBizMsg(int hSend, IBizMessage** lpMsg, unsigned uiTimeout = 1000, unsigned uiFlag = 0) = 0;
	////////////////////////////////////////////////////////////////////////////////
};

extern "C"
{
    /**
	* »ñÈ¡T2_SDKµÄ°æ±¾ºÅ
	* @return µ±Ç°T2_SDKµÄ°æ±¾ºÅ
	* Æ©Èç£º°æ±¾Îª0x10000002±íÊ¾1.0.0.2
	*/
    int FUNCTION_CALL_MODE GetVersionInfo();

    /**
	* »ñÈ¡Ò»¸öÊµÏÖCConfigInterface½Ó¿ÚµÄÀàµÄÖ¸Õë
	* @return ÊµÏÖCConfigInterface½Ó¿ÚµÄÀàµÄÖ¸Õë
	*/
CConfigInterface* FUNCTION_CALL_MODE NewConfig();

    /**
	* »ñÈ¡Ò»¸öÊµÏÖCConnectionInterface½Ó¿ÚµÄÀàµÄÖ¸Õë
	* @param CConfigInterface ÊµÏÖCConfigInterface½Ó¿ÚµÄÀàµÄÖ¸Õë
	* @returnÊµÏÖCConnectionInterface½Ó¿ÚµÄÀàµÄÖ¸Õë
	*/
CConnectionInterface* FUNCTION_CALL_MODE NewConnection(CConfigInterface *lpConfig);

/**
* @param int iVersion ÒµÎñ°ü¸ñÊ½°æ±¾(È¡Öµ:1 ×Ö´®°æ,ÆäËûÖµ 0x20°æ)
* ÍÆ¼öÊ¹ÓÃ0x20°æ
* @return IPacker * ´ò°üÆ÷½Ó¿ÚÖ¸Õë
*/
IF2Packer * FUNCTION_CALL_MODE NewPacker(int iVersion);

/**
* @param void * lpBuffer Òª½â°üµÄÊý¾Ý£¨²»º¬ARÍ¨ÐÅ°üÍ·£©
* @param unsigned int iLen Êý¾Ý³¤¶È
* @return IUnPacker * °æ±¾2½á¹û¼¯²Ù×÷½Ó¿ÚÖ¸Õë
*/
IF2UnPacker * FUNCTION_CALL_MODE NewUnPacker(void * lpBuffer, unsigned int iLen);

/**
* @param void * lpBuffer Òª½â°üµÄÊý¾Ý£¨²»º¬ARÍ¨ÐÅ°üÍ·£©
* @param unsigned int iLen Êý¾Ý³¤¶È
* @return IUnPacker * °æ±¾1½á¹û¼¯²Ù×÷½Ó¿ÚÖ¸Õë
*/
IF2UnPacker * FUNCTION_CALL_MODE NewUnPackerV1(void * lpBuffer, unsigned int iLen);

/**
* @param void * lpBuffer Òª½â°üµÄÊý¾Ý£¨²»º¬ARÍ¨ÐÅ°üÍ·£©
* @return 1±íÊ¾°æ±¾1µÄ½á¹û¼¯Êý¾Ý£¬0x21~0x2F °æ±¾2µÄ½á¹û¼¯Êý¾Ý
*/
int FUNCTION_CALL_MODE GetPackVersion(const void *lpBuffer);
/**
* @param char *EncodePass ´«³öµÄÉ¢ÁÐ½á¹û£¬×Ö·û´®£¬³¤¶È²»³¬¹ý16£¨°üÀ¨'\0'£©
* @param const char* Password ´«ÈëµÄ´ýÉ¢ÁÐµÄÃÜÂë
* @param int nReserve ±£Áô²ÎÊý£¬²ÉÓÃÄ¬ÈÏÖµ
* @return char * ´«³öµÄÉ¢ÁÐ½á¹ûµØÖ·£¬Í¬EncodePass
*/
char * FUNCTION_CALL_MODE Encode(char *EncodePass, const char* Password, int nReserve = 0 );


/**
*ÀûÓÃÒ»²¿Ìá¹©µÄ¼ÓÃÜº¯ÊýÀ´¶ÔÃÜÂë½øÐÐ¼ÓÃÜ
* @param const char* pIn ´«ÈëµÄ´ý¼ÓÃÜµÄ×Ö·û´®
* @param const char* pOut Êä³ö²ÎÊý£¬³¤¶ÈºÍ´«ÈëµÄ×Ö·û´®´óÐ¡Ò»Ñù£¬ÓÉÍâ²¿ÉêÇë
* @return 
*/
int FUNCTION_CALL_MODE EncodeEx(const char *pIn, char *pOut);


/**
* ¹¹ÔìÒ»¸ö¹ýÂËÆ÷½Ó¿ÚÖ¸Õë
* @return ·µ»ØNULL±íÊ¾Ê§°Ü.
*/
CFilterInterface* FUNCTION_CALL_MODE NewFilter();
    
/**
* ¹¹ÔìÒ»¸ö¶©ÔÄÊôÐÔ½Ó¿ÚÖ¸Õë
* @return ·µ»ØNULL±íÊ¾Ê§°Ü.
*/
CSubscribeParamInterface* FUNCTION_CALL_MODE NewSubscribeParam();

//20130625 xuxp ¹¹ÔìÒµÎñÏûÏ¢
IBizMessage* FUNCTION_CALL_MODE NewBizMessage();

}

#endif
