//
//	sf_seed.h
//

#ifndef __SF_SEED_H__
#define __SF_SEED_H__

#include "sfdefine.h"

/**
 *	Constants
 */

#define	SEED_ENCRYPT				0
#define	SEED_DECRYPT				1

#define	SEED_ROUNDKEY_SIZE			32		// WORD
#define	SEED_BLOCK_SIZE				16		// BYTE
#define	SEED_KEY_SIZE				16		// BYTE
#define	SEED_IV_SIZE				16		// BYTE

/**
 *	Error codes
 */

#define	SEED_ERROR_INVALID_KEY		-100
#define	SEED_ERROR_INVALID_INPUT	-101

/**
 *	Prototypes
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO_GLOBAL
/*	seed config */
typedef struct {
	SF_WORD		SS0[256];
	SF_WORD		SS1[256];
	SF_WORD		SS2[256];
	SF_WORD		SS3[256];
} SEED_CONFIG;

void	SEED_MakeKey			(SEED_CONFIG *seed_config, 
								 SF_WORD *pdwRoundKey, SF_BYTE *pbUserKey, int dir);
void	SEED_Main				(SEED_CONFIG *seed_config, 
								 SF_BYTE *output, SF_BYTE *input, SF_WORD *roundKey);
int		SEED_CBC_Encrypt		(SEED_CONFIG *seed_config, 
								 SF_BYTE *psOutput, SF_BYTE *psInput, SF_WORD nInputLength,
								 SF_BYTE *psIV, SF_BYTE *psKey);
int		SEED_CBC_Decrypt		(SEED_CONFIG *seed_config, 
								 SF_BYTE *psOutput, SF_BYTE *psInput, SF_WORD nInputLength,
								 SF_BYTE *psIV, SF_BYTE *psKey);
#else
void far	SEED_MakeKey			(SF_WORD far *pdwRoundKey, SF_BYTE far *pbUserKey, int dir);
void far	SEED_Main				(SF_BYTE far *output, SF_BYTE far *input, SF_WORD far *roundKey);
int far		SEED_CBC_Encrypt		(SF_BYTE far *psOutput, SF_BYTE far *psInput, SF_WORD nInputLength,
									 SF_BYTE far *psIV, SF_BYTE far *psKey);
int far		SEED_CBC_Decrypt		(SF_BYTE far *psOutput, SF_BYTE far *psInput, SF_WORD nInputLength,
									 SF_BYTE far *psIV, SF_BYTE far *psKey);
#endif

#ifdef __cplusplus
}
#endif

#endif
