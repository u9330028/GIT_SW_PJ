#if !defined(AFX_SEEDDWS_H__EBA28BDE_F79E_417E_874C_D6FF0621125A__INCLUDED_)
#define AFX_SEEDDWS_H__EBA28BDE_F79E_417E_874C_D6FF0621125A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeedDws.h : header file
//

#include "seedx.tab"

/******************* Constant Definitions *********************/

#define NoRounds         16
#define NoRoundKeys      (NoRounds*2)
#define SeedBlockSize    16    /* in bytes */
#define SeedBlockLen     128   /* in bits */


/********************** Common Macros ************************/

#if defined(_MSC_VER)
    #define ROTL(x, n)     (_lrotl((x), (n)))
    #define ROTR(x, n)     (_lrotr((x), (n)))
#else
    #define ROTL(x, n)     (((x) << (n)) | ((x) >> (32-(n))))
    #define ROTR(x, n)     (((x) >> (n)) | ((x) << (32-(n))))
#endif


/**************** Function Prototype Declarations **************/

#ifndef PROTOTYPES
#define PROTOTYPES  1
#endif

#if PROTOTYPES
#define PROTO_LIST(list)    list
#else
#define PROTO_LIST(list)    ()
#endif

#define	DIVISOR	16
#define	DATA_MAX_SIZE	10240+32

/////////////////////////////////////////////////////////////////////////////
// CSeedDws window

class CSeedDws
{
public:
	CSeedDws();           // protected constructor used by dynamic creation

	char pbSeedUserKey[32];
	DWORD prSeedRoundKey[32];

// Operations
public:
	int base64encode_buf (char *buf, int buflen, char *result, int *resultlen);
	int base64decode_buf (char *buf, int buflen, char *result, int *resultlen);
	void SeedEncryptCBC(char *bpData, int inlen, BYTE *bpIV);
	void SeedDecryptCBC(BYTE *bpData, int ilen, BYTE *bpIV);
	void SeedEncCBCPKCS5(BYTE *bpData, int *olen, int ilen, BYTE *bpIV);
	void SeedDecCBCPKCS5(BYTE *bpData, int *olen, int ilen, BYTE *bpIV);
	int SeedEncryptData(char *indata, int inlen, char *outdata, int *olen);
	int SeedDecryptData(char *indata, int inlen, char *outdata, int *olen);

	void SeedEncrypt 
	    PROTO_LIST((BYTE *pbData, DWORD *pdwRoundKey));
	void SeedDecrypt
		PROTO_LIST((BYTE *pbData, DWORD *pdwRoundKey));
	void SeedEncRoundKey
		PROTO_LIST((DWORD *pdwRoundKey, BYTE *pbUserKey));

	int base64DecodeLen(const char *);
	int base64Decode(char *, const char *);
	int base64DecodeBinary(unsigned char *, const char *);

	int base64EncodeLen(int);
	int base64EncodeBinary(char *, const unsigned char *, int);
	int base64Encode(char *, const char *, int);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEEDDWS_H__EBA28BDE_F79E_417E_874C_D6FF0621125A__INCLUDED_)
