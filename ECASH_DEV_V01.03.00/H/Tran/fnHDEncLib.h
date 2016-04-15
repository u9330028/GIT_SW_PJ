#ifdef WIN32
#include <windows.h>
#endif

#define HD_ERR_MST_KEY_INDEX		1	/* 마스터키 Index가 00 ~ 9 이어야 함 */
#define HD_ERR_SVR_HASH				2	/* Client 및 Server Hash값이 상이    */	
#define HD_ERR_BLOCK_LEN			3	/* 암호화 영역 길이 에러             */
#define HD_ERR_ENC_DATA_KIND		4	/* 암호화 영역 길이 에러             */
#define HD_ERR_ETC_PROC				5	/* 복호화 에러                       */

#define KEY_LEN				16
#define SSKEY_BLOCK_LEN			46
#define ENC_BLOCK_LEN			32

/**************** Function Prototype Declarations **************************************************************************************************/
// Encrpt Function
XDllExport int  CALLBACK fnHD_CKeyInit(BYTE *s1Key, int *s1Len, BYTE *s1Idx, BYTE *mk_table, BYTE *wKey, int wLen);
XDllExport int  CALLBACK fnHD_GetHashCode(BYTE *datetime, long amnt, BYTE *dData);
XDllExport int  CALLBACK fnHD_EncSeed(BYTE *wKey, BYTE *wIdx, BYTE *sData, int sLen, BYTE *dData, int *dLen);
XDllExport int  CALLBACK fnHD_DecSeed(BYTE *wKey, int wIdx, BYTE *mk_tabale, BYTE *sData, int sLen, BYTE *pwd, BYTE *acnt, BYTE *hData);
/**************************************************************************************************************************************************/