//
//	sf_rand.h
//

#ifndef __SF_RAND_H__
#define __SF_RAND_H__

#include "sfdefine.h"

#ifdef  __cplusplus
extern "C" {
#endif

void far	RAND_Init			(void);
void far	RAND_GetByte		(SF_BYTE far *random, int byte_len);

#ifdef	__cplusplus
}
#endif

#endif
