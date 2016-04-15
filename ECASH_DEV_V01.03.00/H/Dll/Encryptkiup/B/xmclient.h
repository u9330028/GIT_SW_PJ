//
//	xm_client.h
//

#ifndef	__XM_CLIENT_H__
#define	__XM_CLIENT_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include "xmdevice.h"
#include "xmdefine.h"
#include "sfdefine.h"
#include "sfsha.h"
#include "sfseed.h"
#include "sfpkcs1.h"
#include "sfbase64.h"
#include "xmp.h"

#ifdef	NO_GLOBAL
#define	XMC_SEED_FILENAME	"seed.dat"
#define	XMC_CADB_FILENAME	"ca.db"
#endif

/*	XM client config */
typedef struct {
#ifdef ENABLE_FILE
	char				server_cert_file[MAX_PATH_LEN];
#else
	uint8				server_x509_der[CERT_SIZE_MAX];
	uint16				cert_len;
#endif
	PKCS1_PublicKey		server_rsa;
	uint8				server_certhash[SHA_DIGEST_LENGTH];
#ifdef NO_GLOBAL
	SEED_CONFIG			seed_config;
#endif
} XM_CLIENT_CONFIG;


/*	XM client context */
typedef struct {
	XM_CLIENT_CONFIG	config;
    uint8               secret[SECRET_SIZE];
	uint8				compress;
	SHA_CTX				sha_ctx;
	uint8				client_MAC_secret[SHA_DIGEST_LENGTH];
	uint8				server_MAC_secret[SHA_DIGEST_LENGTH];
	uint8				client_write_key[SEED_KEY_SIZE];
	uint8				server_write_key[SEED_KEY_SIZE];
	uint8				client_IV[SEED_IV_SIZE];
	uint8				server_IV[SEED_IV_SIZE];

	uint8				sid[20];
	uint8				sid_len;
    
	uint32				client_seq;
	uint32				server_seq;
} XM_CLIENT_CTX;

/**
 *	XMC2_ClientLib_Init :
 *	
 *	output : rand_seed			:
 *	input  : ctx				: pointer to Client Context
 *			 xmc_path			: SEED data file & ca.db directory (optional for NO_GLOBAL)
 *			 cert_filename		: server cert file name (optional for ENABLE_FILE)
 *			 server_cert		: pointer to DER encoded server_cert (optional for !ENABLE_FILE)
 *			 server_cert_length	: length of server_cert (optional for !ENABLE_FILE)
 *
 *	return : 1					: success
 *			 error <= 0			: fail
 */
int far		
XMC2_ClientLib_Init     (XM_CLIENT_CTX far *ctx
#if		defined (ENABLE_FILE)
						 , char *cert_filename		// full path
#else
						 , uint8 far *cert, uint16 cert_length
#endif
						 );

int	far	
XMC2_Encode_KeyInit_Token	(XM_CLIENT_CTX far *ctx, 
							 uint8 far *output, uint16 *output_length, uint16 output_max_length);

int	far	
XMC2_Decode_KeyFinal_Token  (XM_CLIENT_CTX far *ctx, 
							 uint8 far *input, uint16 input_length);

int	far	
XMC2_Encrypt_Message        (XM_CLIENT_CTX far *ctx, 
							 uint8 far *output, uint16 *output_length, uint16 output_max_length,  
                             uint8 far *message, uint16 message_length);

int far
XMC2_Decrypt_Message        (XM_CLIENT_CTX far *ctx, 
							 uint8 far *output, uint16 *output_length, uint16 output_max_length,
						     uint8 far *input, uint16 input_length);

int
XMC2_Encode_KeyInit_Token_Base64 (XM_CLIENT_CTX *ctx, uint8 *b64out, uint16 *b64out_len, uint16 out_max_len);

int		
XMC2_Decode_KeyFinal_Token_Base64  (XM_CLIENT_CTX *ctx, uint8 *b64in, uint16 b64in_len);

int		
XMC2_Encrypt_Message_Base64 (XM_CLIENT_CTX *ctx, uint8 *b64out, uint16 *b64out_len, uint16 out_max_len, uint8 *in, uint16 in_len);

int 
XMC2_Decrypt_Message_Base64 (XM_CLIENT_CTX *ctx, uint8 *out, uint16 *out_len, uint16 out_max_len, uint8 *b64in, uint16 b64in_len);


#ifdef ENABLE_FILE
int
File_Write					(char *file_name, uint8 *bin, uint16 bin_len);

int
File_Read					(char *file_name, uint8 *bin, uint16 max_len);

#else
int far
XMC2_Get_Certificate		(XM_CLIENT_CTX far *ctx, 
							 uint8 far *output, uint16 *output_length, uint16 output_max_length);
#endif

#ifdef  __cplusplus
}
#endif

#endif
