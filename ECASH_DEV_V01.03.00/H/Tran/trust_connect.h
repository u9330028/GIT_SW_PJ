
#ifndef __TRUSTCONNECT_H__
#define __TRUSTCONNECT_H__

#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
#ifdef TNATMCSTK_EXPORTS
#define TRUSTCONNECT_API __declspec(dllexport)
#else
#define TRUSTCONNECT_API __declspec(dllimport)
#endif
#else
#define TRUSTCONNECT_API
#endif

#ifndef _ETRI
#define _ETRI
typedef struct _UString 
{
    int				length;
    unsigned char	*value;
} UString;
#endif // _ETRI

// Transfer Buffer Setting
#define TC_TRANS_BUF_TYPE_POINT                     1
#define TC_TRANS_BUF_LENGTH_POINT                   2
#define TC_TRANS_BUF_LENGTH_SIZE                    3

// Transfer Buffer Type
#define TN_TCREQUEST_ENCPUBKEY                      0x01
#define TN_TCRESPONSE_ENCPUBKEY                     0x02
#define TN_TCSEND_ENCTNKEY                          0x03
#define TN_TCSEND_ENCTNNONCE                        0x04
#define TN_TCENCDATA                                0x05

// 성공여부.. 1st byte
#define S_SUCCESS                                   0x00000000
#define S_FAILED                                    0xF0000000

// 외부 함수.. 2-3nd byte
#define SL_TCSERVERKEYSHAREINIT                     0x01100000
#define SL_TCSERVERKEYSHAREFINAL                    0x01200000
#define SL_TCSERVERENCIPHER                         0x01300000
#define SL_TCSERVERDECIPHER                         0x01400000

#define SL_TCCLIENTKEYSHAREINIT                     0x02100000
#define SL_TCCLIENTKEYSHAREFINAL                    0x02200000
#define SL_TCCLIENTENCIPHER                         0x02300000
#define SL_TCCLIENTDECIPHER                         0x02400000
#define SL_TCCLIENTSESSIONDATASTATUS                0x02500000

#define SL_TCCONVERTUSTRING2TRANSBUFFER             0x03100000
#define SL_TCCONVERTTRANSBUFFER2USTRING             0x03200000

// 발발지점 (함수단위) 4-5th byte
#define SF_TN_TCGENPUBLICKEYPAIR                    0x00001000
#define SF_TN_ENCIPHERB                             0x00002000
#define SF_TN_DECIPHERB                             0x00003000
#define SF_TCSETPUBKEYPAIRGENTIME                   0x00004000
#define SF_TCSETPUBKEY                              0x00005000
#define SF_TCSETPRIKEY                              0x00006000
#define SF_TCSETENCPUBKEY                           0x00007000
#define SF_TCGETPUBKEYPAIRGENTIME                   0x00008000
#define SF_TCGETPUBKEY                              0x00009000
#define SF_TCGETPRIKEY                              0x0000a000
#define SF_TCGETENCPUBKEY                           0x0000b000
#define SF_TCPUBKEYENCIPHER                         0x0000c000
#define SF_TCPRIKEYDECIPHER                         0x0000d000
#define SF_TCGENTNNONCE                             0x0000e000
#define SF_TCENCIPHERTNNONCE                        0x0000f000
#define SF_TCSETSESSIONDATA                         0x00010000
#define SF_TCGETSESSIONDATA                         0x00011000
#define SF_TCGENHMAC                                0x00012000
#define SF_TCVERIFYHMAC                             0x00013000
#define SF_TCGENTDATA                               0x00014000
#define SF_TCENCIPHER                               0x00015000
#define SF_TCDECIPHER                               0x00016000
#define SF_TCPARSINGTDATA                           0x00017000
#define SF_TCGENTNKEY                               0x00018000
#define SF_TCSERVERVERIFYTNNONCE                    0x00018000
#define SF_TCCLIENTVERIFYTNNONCE                    0x00019000
#define SF_TCDECIPHERMASTERKEY                      0x0001a000
#define SF_TCGENENCIPHERMASTERKEY                   0x0001b000
#define SF_TCCLIENTSESSIONDATASTATUS                0x0001c000

// 기존 TN30CSTK 함수를 사용 하므로, 기존 오류 코드를 포함하기 위하여, 0x500(1280)부터 시작함.
// 이유 6-8th byte
#define SR_NULL_DATA                                0x00000500
#define SR_NOT_FOUND_DEVICEID                       0x00000501
#define SR_FAIL_CSP_GENERATEPKEY                    0x00000502
#define SR_FAIL_CSP_PUBLICENCRYPTION                0x00000503
#define SR_FAIL_CSP_PRIVATEDECRYPTION               0x00000504
#define SR_FAIL_CSP_GENHMAC                         0x00000505
#define SR_FAIL_CSP_VERIFYHMAC                      0x00000506
#define SR_FAIL_CSP_RANDOMSTRING                    0x00000507
#define SR_FAIL_CSP_GETBLOCKIDBYCHAR                0x00000508
#define SR_FAIL_LIB_INITIALIZE                      0x00000509
#define SR_FAIL_LIB_TERMINATE                       0x0000050a
#define SR_FAIL_TCGETDEVICEID                       0x0000050b
#define SR_FAIL_TCREADFILEB                         0x0000050c
#define SR_FAIL_TCWRITEFILEB                        0x0000050d
#define SR_FAIL_TCPARSINGSESSIONDATA                0x0000050e
#define SR_FAIL_TCVERIFYTNNONCE                     0x0000050f
#define SR_FAIL_TCVERIFYTNNONCE_INVALID_MODE        0x00000510
#define SR_NULL_SESSION_TNKEY                       0x00000511
#define SR_NULL_SESSION_TNNONCE                     0x00000512
#define SR_OVER_TRANS_BUF_SIZE                      0x00000513

#define SR_ERROR_UNKNOWN_ALGO						0x00000514
#define SR_ERROR_WRONG_KEYIV_LEN					0x00000515
#define SR_ERROR_BLOCK_ENCRYPT						0x00000516
#define SR_ERROR_BLOCK_DECRYPT						0x00000517
#define SR_ERROR_WRONG_INPUT						0x00000518
#define SR_ERROR_HASH_FAIL							0x00000519
#define SR_ERROR_BASE64_ENCODE_FAIL					0x0000051a
#define SR_ERROR_BASE64_DECODE_FAIL					0x0000051b

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

TRUSTCONNECT_API int WINAPI TN_TCServerKeyShareInit( char *_pszInFolderPath, char *_pszInMasterKey, UString *_pusInOutEncPubKey );
TRUSTCONNECT_API int WINAPI TN_TCServerKeyShareFinal( char *_pszInClientIndex, char *_pszInFolderPath, UString *_pusInEncTNKey, UString *_pusOutEncTNNonce );
TRUSTCONNECT_API int WINAPI TN_TCServerEncipher( char *_pszInClientIndex, char *_pszInFolderPath, UString *_pusInContentData, UString *_pusOutEncTData );
TRUSTCONNECT_API int WINAPI TN_TCServerDecipher( char *_pszInClientIndex, char *_pszInFolderPath, int _nInStatusCode, UString *_pusInEncTData, UString *_pusOutContentData );

TRUSTCONNECT_API int WINAPI TN_TCClientKeyShareInit( char *_pszInMasterKey, UString *_pusInEncPubKey, UString *_pusOutEncTNKey );
TRUSTCONNECT_API int WINAPI TN_TCClientKeyShareFinal( UString *_pusInEncTNNonce );
TRUSTCONNECT_API int WINAPI TN_TCClientEncipher( UString *_pusInContentData, UString *_pusOutEncTData );
TRUSTCONNECT_API int WINAPI TN_TCClientDecipher( UString *_pusInEncTData, UString *_pusOutContentData );
TRUSTCONNECT_API int WINAPI TN_TCClientSessionDataStatus( );
TRUSTCONNECT_API int WINAPI TN_TCClientDeleteSessionData( );

TRUSTCONNECT_API void WINAPI TN_TCSetUString( UString *_pusData, unsigned char *_pszValue, int _nLength );
TRUSTCONNECT_API void WINAPI TN_TCInitUString( UString *_pusData );
TRUSTCONNECT_API void WINAPI TN_TCDeleteMemoryUC( UString *_pusData );

TRUSTCONNECT_API int WINAPI TN_TCConvertUString2TransBuffer( int nInTransType, UString *_pusInTransBuffer, int nInBufLength, char *_pszOutTransBuffer );
TRUSTCONNECT_API int WINAPI TN_TCConvertTransBuffer2UString( char *_pszInTransBuffer, int nInBufLength, UString *_pusOutTransBuffer );

TRUSTCONNECT_API void WINAPI TN_LogHexaDump( char *_pszInCaption, unsigned char *_pszInMsgBuf, int _nInMsgLen );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __TRUSTCONNECT_H__

