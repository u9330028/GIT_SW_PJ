#ifndef _API_H_
#define _API_H_

#if WIN32
#ifdef L_SEED_EXPORTS
#define SEED_API __declspec(dllexport)
#else
#define SEED_API __declspec(dllimport)
#endif
#else
#define SEED_API
#endif

#ifdef  __cplusplus
extern "C" {
#endif /* #ifdef  __cplusplus */

#define ERR_WRONG_PARAM		-101
#define ERR_MALLOC_FAIL		-102
#define ERR_BUFSIZE_SMALL	-103
#define ERR_FOPEN_FAIL		-104
#define ERR_FREAD_FAIL		-105
#define ERR_FWRITE_FAIL		-106

#define ERR_SEED_ENCRYPT	-201
#define ERR_SEED_DECRYPT	-202


SEED_API int ISEED_encrypt(char *szKeyFilePath, unsigned char *inbuf, int inbufl,
		unsigned char *outbuf, int outbufl, int *outl);
SEED_API int ISEED_decrypt(char *szKeyFilePath, unsigned char *inbuf, int inbufl,
		unsigned char *outbuf, int outbufl, int *outl);

SEED_API int ISEED_encrypt_buf(unsigned char *keybuf, int keybufl, unsigned char *inbuf, int inbufl,
		unsigned char *outbuf, int outbufl, int *outl);
SEED_API int ISEED_decrypt_buf(unsigned char *keybuf, int keybufl, unsigned char *inbuf, int inbufl,
		unsigned char *outbuf, int outbufl, int *outl);


#ifdef  __cplusplus
}
#endif /* #ifdef  __cplusplus */

#endif /* _API_H_ */
