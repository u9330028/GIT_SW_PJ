//
//	xm_client.h
//

#ifndef	__XM_CLIENT_H__KB
#define	__XM_CLIENT_H__KB

#ifdef  __cplusplus
extern "C" {
#endif

#include "xmdevice.h"
#include "xmdefine.h"
#include "sfdefine.h"
#include "sfsha.h"
#include "sfpkcs1.h"

#define	SECRET_SIZE			48
#define	SEED_KEY_SIZE		16
#define	SEED_IV_SIZE		16

/*	XM client config */
typedef struct {
	char				server_cert_file[MAX_PATH_LEN];
	PKCS1_PublicKey		server_rsa;
	uint8				server_certhash[SHA_DIGEST_LENGTH];
} KB_XM_CONFIG;


/*	XM client context */
typedef struct {
	KB_XM_CONFIG		config;
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
} KB_XM_CTX;

int 		
KB_ClientLib_Init	(KB_XM_CTX *ctx, char *cert_filename /* full path */);

int		
KB_Encode_KeyInit_Token	(KB_XM_CTX *ctx, 
						 uint8 *output, uint16 *output_length, uint16 output_max_length);

int	
KB_Decode_KeyFinal_Token	(KB_XM_CTX *ctx, 
							 uint8 *input, uint16 input_length);

int		
KB_Encrypt_Message	(KB_XM_CTX *ctx,
					 uint8 *output, uint16 *output_length, uint16 output_max_length,
					 uint8 *message, uint16 message_length);

int 
KB_Decrypt_Message	(KB_XM_CTX *ctx, 
					 uint8 *output, uint16 *output_length, uint16 output_max_length,
					 uint8 *input, uint16 input_length);

#ifdef  __cplusplus
}
#endif

#endif
