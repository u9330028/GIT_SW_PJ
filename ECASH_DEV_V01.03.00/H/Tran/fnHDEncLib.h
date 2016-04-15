#ifdef WIN32
#include <windows.h>
#endif

#define HD_ERR_MST_KEY_INDEX		1	/* ������Ű Index�� 00 ~ 9 �̾�� �� */
#define HD_ERR_SVR_HASH				2	/* Client �� Server Hash���� ����    */	
#define HD_ERR_BLOCK_LEN			3	/* ��ȣȭ ���� ���� ����             */
#define HD_ERR_ENC_DATA_KIND		4	/* ��ȣȭ ���� ���� ����             */
#define HD_ERR_ETC_PROC				5	/* ��ȣȭ ����                       */

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