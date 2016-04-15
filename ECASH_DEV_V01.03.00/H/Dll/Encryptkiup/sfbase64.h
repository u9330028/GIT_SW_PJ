//
//	sf_base64.h
//

#ifndef	__SF_BASE64_H__
#define	__SF_BASE64_H__

#include "sfdefine.h"

#ifdef  __cplusplus
extern "C" {
#endif

int far		BASE64_Encode		(uint8 far *output, uint8 far *input, int length);

int far		BASE64_Decode		(uint8 far *output, uint8 far *input, int length);

#ifdef	__cplusplus
}
#endif

#endif
