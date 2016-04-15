//
//	sf_hmac.h
//

#ifndef	__SF_HMAC_H__
#define	__SF_HMAC_H__

#include "sfdefine.h"

//#include "sfmd5.h"
#include "sfsha.h"

#define	HMAC_HASHID_MD5					1
#define	HMAC_HASHID_SHA					2

#define	HMAC_MAX_DIGEST_LENGTH			20
#define	HMAC_MAX_CBLOCK					64

typedef struct {
	int			hashId;
	SF_BYTE		key[HMAC_MAX_CBLOCK];
	int			keyLength;
	int			macSize;
	union {
//		MD5_CTX		md5;
		SHA_CTX		sha;
	} ctx;
} HMAC_CTX;


#ifdef  __cplusplus
extern "C" {
#endif

void far	HMAC_Init		(HMAC_CTX far *ctx, int hashId, SF_BYTE far *key, int keyLength);
void far	HMAC_Update		(HMAC_CTX far *ctx, SF_BYTE far *data, int dataLength);
void far	HMAC_Final		(HMAC_CTX far *ctx, SF_BYTE far *md, int *mdLength);

void far	HMAC			(SF_BYTE far *md, int *mdLength, int hashId, SF_BYTE far *key, int keyLength, 
							 SF_BYTE far *data, int dataLength);

//void		HMAC_MD5		(SF_BYTE *md, SF_BYTE *key, int keyLength, SF_BYTE *data, int dataLength);
void far	HMAC_SHA		(SF_BYTE far *md, SF_BYTE far *key, int keyLength, SF_BYTE far *data, int dataLength);

#ifdef  __cplusplus
}
#endif

#endif
