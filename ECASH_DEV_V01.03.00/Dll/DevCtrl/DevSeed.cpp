/////////////////////////////////////////////////////////////////////////////
//	DevSeed.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevCmn.h"

/*
#include "..\..\H\Seed\qsl.h"
#include "..\..\H\Seed\qslkey.h"
#include "..\..\H\Seed\Base64.h"
#include "..\..\H\Seed\Base64.c"
#include "..\..\H\Seed\jbcm.c"
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NMSBUFFSIZE				32768

/////////////////////////////////////////////////////////////////////////////
//	SEED함수(SED) : SEED INIT
/////////////////////////////////////////////////////////////////////////////
// SeedKey Intiialize & Get SeedKey
int	CDevCmn::fnSED_SeedKeyInitialize(BYTE* SeedKeyBuff)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSED_SeedKeyInitialize()");
	int		nReturn = T_OK;
	char	SeedKey[263];										// 암호화 키

/////////////////////////////////////////////////////////////////////////////
// 버퍼/변수 초기화
/////////////////////////////////////////////////////////////////////////////
	SeedFailCode = 0;											// FilaCode 초기화
	memset(SeedKey,		0, sizeof(SeedKey));					// SeedKey

/////////////////////////////////////////////////////////////////////////////
// key Initialize
/////////////////////////////////////////////////////////////////////////////
	if (cl_sess != NULL)
	{
		QSL_Del(cl_sess);
	}
	memset(SeedKey, 0x00, sizeof(SeedKey));
	memcpy(SeedKey, SeedKeyBuff, 262);
	pubKey = QSLPubKey_NewFromBuf(SeedKey, strlen(SeedKey));
	if (pubKey == NULL)
	{
		return T_ERROR;
	}

	cl_sess = QSL_Client_New(pubKey, QSL_SEED);
	if(cl_sess == NULL)
	{
		return T_ERROR;
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSED_SeedKeyInitialize():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SEED함수(SED) : SEED ENCRYPT
/////////////////////////////////////////////////////////////////////////////
// SeedFianlKey Encrypt
int CDevCmn::fnSED_SeedKeyEncryption(char* KeyData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSED_SeedKeyEncryption()");
	int		nReturn = T_OK;
	BYTE	SeedPlain[NMSBUFFSIZE];
	int		SeedPlainLen;
	char	SeedCipher[NMSBUFFSIZE];
/////////////////////////////////////////////////////////////////////////////
// 버퍼/변수 초기화
/////////////////////////////////////////////////////////////////////////////
	SeedFailCode = 0;											// FilaCode 초기화

	if (QSL_HEADER_Write(SeedPlain, &SeedPlainLen, 180, cl_sess) != SUCCESS)
	{
		SeedFailCode = nReturn;
		return T_ERROR;
	}

	Base64_Encode(SeedCipher, 300, SeedPlain, SeedPlainLen);

	nReturn = strlen(SeedCipher);
	memcpy(KeyData, SeedCipher, nReturn);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSED_SeedKeyEncryption():return(%d)", nReturn);
	return nReturn;
}

// Encryption
int CDevCmn::fnSED_SeedEncryption(BYTE* PlainData, BYTE* CipherData, int PlainDataLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSED_SeedEncryption()");
	
	int		nReturn = FALSE;
	int		SeedCipherLen;
	int		SeedPlainLen;

	BYTE	SeedPlain[NMSBUFFSIZE];
	BYTE	SeedCipher[NMSBUFFSIZE];

/////////////////////////////////////////////////////////////////////////////
// 버퍼/변수 초기화
/////////////////////////////////////////////////////////////////////////////
	SeedFailCode = 0;											// FilaCode 초기화
	SeedPlainLen		= 0;									// SeedPlain Length
	memset(SeedPlain,	0, sizeof(SeedPlain));					// SeedPlain
	SeedCipherLen		= 0;									// SeedCipher Length
	memset(SeedCipher,	0, sizeof(SeedCipher));					// SeedCipher
//	memset(&SeedContext, 0, sizeof(SeedContext));				// Seed Context영역

/////////////////////////////////////////////////////////////////////////////
// key Encryption
/////////////////////////////////////////////////////////////////////////////
	if ((SeedPlainLen = JBB_Compress(SeedPlain, PlainData, PlainDataLen)) < 0)
	{
		SeedFailCode = nReturn;
		return T_ERROR;
	}
	
	SeedCipherLen = NMSBUFFSIZE;

	if (nReturn = QSL_DATA_Encrypt(SeedCipher, &SeedCipherLen, SeedPlain, SeedPlainLen, cl_sess) != SUCCESS)
	{
		SeedFailCode = nReturn;
		return T_ERROR;
	}

	nReturn = Base64_Encode((char *)CipherData, 1024, SeedCipher, SeedCipherLen);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSED_SeedEncryption():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SEED함수(SED) : SEED DECRYPT
/////////////////////////////////////////////////////////////////////////////
// Host SeedKey Decryption
int CDevCmn::fnSED_SeedKeyDecryption(BYTE* KeyData, unsigned int KeyLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSED_SeedKeyDecryption()");
	int		nReturn = T_OK;
	BYTE	SeedPlain[NMSBUFFSIZE];
	unsigned int		SeedPlainLen;										// PlainText Length

/////////////////////////////////////////////////////////////////////////////
// 버퍼/변수 초기화
/////////////////////////////////////////////////////////////////////////////
	SeedFailCode = 0;											// FilaCode 초기화

	Base64_Decode2(SeedPlain, 150, &SeedPlainLen, (const char *)KeyData, KeyLen);

	if (QSL_HEADER_Read(cl_sess, SeedPlain, SeedPlainLen) != SUCCESS)
	{
		SeedFailCode = nReturn;
		return T_ERROR;
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSED_SeedKeyDecryption():return(%d)", nReturn);
	return nReturn;
}

// Host SeedData Decryption
int CDevCmn::fnSED_SeedDataDecryption(BYTE* CipherData, BYTE* PlainData, int CipherDataLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSED_SeedDataDecryption()");
	
	int		nReturn = FALSE;

	int		SeedPlainLen;										// PlainText Length
	BYTE	SeedPlain[NMSBUFFSIZE];								// PlainText

	unsigned int	SeedCipherLen;								// CipherText Length
	BYTE	SeedCipher[NMSBUFFSIZE];							// CipherText

/////////////////////////////////////////////////////////////////////////////
// 버퍼/변수 초기화
/////////////////////////////////////////////////////////////////////////////
	SeedFailCode = 0;											// FilaCode 초기화
	SeedPlainLen		= 0;									// SeedPlain Length
	memset(SeedPlain,	0, sizeof(SeedPlain));					// SeedPlain
	SeedCipherLen		= 0;									// SeedCipher Length
	memset(SeedCipher,	0, sizeof(SeedCipher));					// SeedCipher
//	memset(&SeedContext, 0, sizeof(SeedContext));				// Seed Context영역

/////////////////////////////////////////////////////////////////////////////
// Host SeedData Decryption
/////////////////////////////////////////////////////////////////////////////
	nReturn = Base64_Decode(SeedCipher, 1024, &SeedCipherLen, (char *)CipherData);
	SeedPlainLen = NMSBUFFSIZE;

	nReturn = QSL_DATA_Decrypt(SeedPlain, &SeedPlainLen, SeedCipher, SeedCipherLen, cl_sess);
	if (nReturn < 0)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSED_SeedKeyDecryption():return(%d)", nReturn);
		SeedFailCode = nReturn;
		return T_ERROR;
	}

	SeedPlainLen = JBB_Decompress(PlainData, SeedPlain, SeedPlainLen);
	nReturn = SeedPlainLen;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSED_SeedKeyDecryption():return(%d)", nReturn);
	return nReturn;
}

// Get SEED ErrorCode 2004.02.10
CString CDevCmn::fstrSED_SeedGetErrorCode(int NGCode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSED_SeedGetErrorCode(%d)", NGCode);
	
	CString strReturn("");
	char	szSeedErrorCode[] = "0000000";

	strReturn.Format("%7.7s", szSeedErrorCode);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSED_SeedGetErrorCode():return(%s)", strReturn);
	return strReturn;
}

