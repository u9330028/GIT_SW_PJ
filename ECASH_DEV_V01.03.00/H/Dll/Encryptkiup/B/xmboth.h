//
//	xm_both.h : Token Encoding and Decoding
//

#ifndef	__XM_BOTH_H__
#define	__XM_BOTH_H__

#include "xmdefine.h"

#include "sfpkcs1.h"

#include "xmp.h"


#ifdef  __cplusplus
extern "C" {
#endif


int far 
Comp_Server_Profile (uint8 *profile);

int far 
Uint32ToByte (uint8 far *buf, uint32 a);

uint32 far 
ByteToUint32 (uint8 far *buf);

#ifdef  __cplusplus
}
#endif

#endif
