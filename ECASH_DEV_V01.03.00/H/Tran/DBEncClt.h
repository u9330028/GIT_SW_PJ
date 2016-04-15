/*******************************************************************
*
* FILE:         DBEncClt.h
* VER :         09.10.07
*
*******************************************************************/

#ifndef _DBENCCLT_H
#define _DBENCCLT_H

/*********************** Type Definitions *************************/

// typedef unsigned __int32	DWORD;	/* 32 bit Type */
// typedef unsigned __int16	WORD;	/* 16 bit Type */
// typedef unsigned char		BYTE;	/*  8 bit Type */

/******************* Constant Definitions *************************/

#define ERR_CLI_BLOCK_LEN			1	/* 전송된 암호화Block 필드의 Length 오류			*/
#define ERR_CLI_KEY_VER				2	/* 재인증요청(key version 상이)						*/	
#define ERR_CLI_ENC_DATA_KIND		3	/* 복호화된 정보의 종류 오류						*/
#define ERR_CLI_KEY_INDEX			4	/* Client가 정의되지 않은 Key Index 발송함			*/
#define ERR_SVR_KEY_INDEX			5	/* Server가 정의되지 않은 Key Index 발송함			*/
#define ERR_SVR_KEY_VER				6	/* Server의 세션키Block 필드의 key version 상이		*/	
#define ERR_SVR_HASH				7	/* Client 및 Server Hash값이 상이					*/	
#define ERR_SVR_ATM_NULL			8	/* 단말정보에 등록되지 않은 단말기					*/	
#define ERR_SVR_FETCH				9   /* 기관용 마스터키 오류								*/

/**************** Function Prototype Declarations *****************/

extern "C" _declspec(dllimport) int DB_CKeyInit(BYTE *s1Key, int *s1Len, BYTE *mk_table, BYTE *wKey, int *wLen);
extern "C" _declspec(dllimport) int DB_SKeyFinal(BYTE *s2Key, int *s2Len, BYTE *mk_table, BYTE *wKey, int *wLen);
extern "C" _declspec(dllimport) int DB_CKeyFinal(BYTE *s2Key, BYTE *mk_table, BYTE *wKey, int *wLen);
extern "C" _declspec(dllimport) int DB_GetHashCode(BYTE *datetime, long amnt, BYTE *dData);
extern "C" _declspec(dllimport) int DB_EncSeed(BYTE *wKey, BYTE *sData, int sLen, BYTE *dData, int *dLen);
extern "C" _declspec(dllimport) int DB_DecSeed(BYTE *wKey, BYTE *sData, int sLen, BYTE *pwd, BYTE *hData);

#endif
