//
//	xm_phash.h
//

#ifndef	__XM_PHASH_H__
#define	__XM_PHASH_H__

#ifdef  __cplusplus
extern "C" {
#endif

/**
 *	XM_PHASH :
 *
 *	output : output			: pointer to output buffer
 *	input  : output_len		: length of output
 *			 secret			: pointer to secret
 *			 secret_len		: length of secret
 *			 seed			: pointer to secret
 *			 seed_len		: length of secret
 *
 */
void far	XM_PHASH			(uint8 far *output, int output_len, uint8 far *secret, 
							 int secret_len, uint8 far *seed, int seed_len);

#ifdef  __cplusplus
}
#endif

#endif
