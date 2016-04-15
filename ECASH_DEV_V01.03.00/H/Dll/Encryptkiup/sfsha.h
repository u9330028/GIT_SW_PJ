//
//	sf_sha.h
//

#ifndef __SF_SHA_H__
#define __SF_SHA_H__

#include "sfdefine.h"

#define SHA_CBLOCK				64
#define	SHA_DIGEST_LENGTH		20

typedef struct {
	SF_WORD		state[5];
	SF_WORD		count[2];
	SF_BYTE		buffer[64];
} SHA_CTX;

#ifdef __cplusplus
extern "C" {
#endif

void far	SHA_Init		(SHA_CTX far *ctx);
void far	SHA_Update		(SHA_CTX far *ctx, SF_BYTE far *data, SF_WORD len);
void far	SHA_Final		(SHA_CTX far *ctx, SF_BYTE far *md);

void far	SF_SHA			(SF_BYTE far *md, SF_BYTE far *data, SF_WORD len);

#ifdef __cplusplus
}
#endif


#endif
