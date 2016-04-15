#ifndef _ISATM_H_
#define _ISATM_H_

#if WIN32
#ifdef ISATM_EXPORTS
#define ISA_API __declspec(dllexport)
#else
#define ISA_API __declspec(dllimport)
#endif
#else
#define ISA_API
#endif

#ifdef  __cplusplus
extern "C" {
#endif /* #ifdef  __cplusplus */

#define SL_DEFAULT			0x00
#define SL_FORWARE_SECURITY		0x01
#define SL_MAC_USE			0x02
#define SL_FULL_SECURITY		SL_FORWARE_SECURITY
	
#define FL_NO_KEYUPDATE			0x00
#define FL_KEYUPDATE			0x01

#define ALG_SEED_CBC			0x00
#define ALG_DES_EDE3_CBC		0x01
#define ALG_AES128_CBC			0x02

#define ISA_MAX_CHELLO_LEN		200
#define ISA_MAX_ENC_LEN(len) 		(len+60)
#define ISA_MAX_KEYREQ_LEN		30
#define ISA_MAX_SHELLO_LEN		250
#define ISA_MAX_KEYRES_LEN		1024
	
#define ISA_BUFFER_SIZE         2048
#define ISA_SID_SIZE              48

#define ISA_KEY_UPDATE			100	

/**************************************************************
 * INISAFE ATM CLIENT API
**************************************************************/
typedef void* ISA_HANDLE; 
 
ISA_API void ISA_Init(); 
ISA_API void ISA_Cleanup(); 
 
ISA_API int ISA_HANDLE_acquire(ISA_HANDLE *phandle, unsigned char flag, char *authfile);
ISA_API void ISA_HANDLE_release(ISA_HANDLE handle);  
 
ISA_API int ISA_chello(ISA_HANDLE handle, char *outbuf, int bufsize, int *outl, char* sidstr, int sidstrl);
ISA_API int ISA_cfinish(ISA_HANDLE handle, char* instr, int inl); 

ISA_API int ISA_ckeyupdate_init(ISA_HANDLE handle, char *outbuf, int bufsize, int *outl);
ISA_API int ISA_ckeyupdate_final(ISA_HANDLE handle, char *instr, int instrl);

ISA_API int ISA_cencrypt(ISA_HANDLE handle, char *outbuf, int bufsize, int *outl, char *instr, int inl, unsigned char alg); 
ISA_API int ISA_cdecrypt(ISA_HANDLE handle, char *outbuf, int bufsize, int *outl, char *instr, int inl); 

ISA_API char *ISA_getErrorCode(int errorcode);

ISA_API int ISA_HANDLE_store_encfile(ISA_HANDLE handle, char* ctxpath, char *atmid, int atmidl);
ISA_API int ISA_HANDLE_load_encfile(ISA_HANDLE* handle, char* ctxpath, char *atmid, int atmidl);

#ifdef  __cplusplus
}
#endif /* #ifdef  __cplusplus */

#endif /* #ifndef _ISATM_H_*/
