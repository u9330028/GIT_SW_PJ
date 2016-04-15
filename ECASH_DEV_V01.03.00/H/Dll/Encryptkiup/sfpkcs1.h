//
//	sf_pkcs1.h
//

#ifndef __SF_PKCS1_H__
#define __SF_PKCS1_H__

#include "sfbigint.h"

//
//
//
#define	RSA_INT_SIZE		BIGINT_MAX_SIZE		// 32 word = 1024 bit

//
//
//
typedef	struct {
	SF_WORD		n[RSA_INT_SIZE +1];
	int			nWordSize;
	SF_WORD		e[RSA_INT_SIZE/2 +1];
	int			eWordSize;
} PKCS1_PublicKey;

#if	defined (ENABLE_RSA_PRI)
typedef struct {
	SF_WORD		n[RSA_INT_SIZE +1];
	int			nWordSize;
	SF_WORD		e[RSA_INT_SIZE/2 +1];
	int			eWordSize;
	SF_WORD		d[RSA_INT_SIZE +1];
	int			dWordSize;
	SF_WORD		p[RSA_INT_SIZE/2 +1];
	int			pWordSize;
	SF_WORD		q[RSA_INT_SIZE/2 +1];
	int			qWordSize;
	SF_WORD		dP[RSA_INT_SIZE/2 +1];
	int			dPWordSize;
	SF_WORD		dQ[RSA_INT_SIZE/2 +1];
	int			dQWordSize;
	SF_WORD		qInv[RSA_INT_SIZE/2 +1];
	int			qInvWordSize;
} PKCS1_PrivateKey;
#endif	// defined (ENABLE_RSA_PRI)

#define		ASN1_INTEGER				2
#define		ASN1_NULL					5
#define		ASN1_SEQUENCE				0x30
#define		RSA_PRI_DEF_VERSION			0

/****
 ****
 ****		PKCS1 Error Message
 ****
 ****/

#define		PKCS1_ERR_MESSAGE_TOO_LONG				-1
#define		PKCS1_ERR_DECODING_ERROR				-2
#define		PKCS1_ERR_CODE_LENGTH_TOO_SHORT			-3
#define		PKCS1_ERR_MODULUS_TOO_SHORT				-4
#define		PKCS1_ERR_DECRYPTION_ERROR				-5
#define		PKCS1_ERR_MALLOC_FAIL					-10

//by utpark 2002-02-07
#define		PKCS1_ERR_INVALID_INPUT					-11
#define		PKCS1_INTENDED_ENCODED_MESSAGE_LENGTH_TOO_SHORT		-12
#define		PKCS1_ERR_INVALID_SIGNATURE				-13
#define		PKCS1_ERR_WRONG_KEY_TYPE				-14
#define		PKCS1_ERR_RSAKEY_TOO_LARGE				-15

#ifdef  __cplusplus
extern "C" {
#endif

uint16 far 
DER_Get_Length(uint8 far *der, uint16 *length);

///////////////////////////////////////////////////////////////////////////////
//
//	PKCS1_I2OSP : convert a nonnegative integer to an octet string of a 
//				  specified length.
//
//	output : result			: pointer to result buffer
//	input  : input			: pointer to input data
//			 resultLength	: length of result data
//
void far 	PKCS1_I2OSP			(SF_BYTE far *result, SF_WORD far *input, int resultLength);

///////////////////////////////////////////////////////////////////////////////
//
//	PKCS1_OS2IP : converts an octet string to a nonnegative integer.
//
//	output : output			: pointer to output buffer
//	input  : string			: pointer to string data
//			 stringLength	: length of string (byte length)
//
void far 	PKCS1_OS2IP			(SF_WORD far *integer, SF_BYTE far *string, int string_len);

///////////////////////////////////////////////////////////////////////////////
//	
//	PKCS1_EME_Encode :
//
//	output : result			: encoded message, an octet string of length result_len.
//	input  : result_len		: intended length in octets of the encoded message.
//			 input			: message to be encoded, an octet string of length
//							  at most result_len -10.
//			 input_len		: string length of input
//
//	return : TRUE = 1		: encoding ok
//			 error_code < 0	: encoding fail
//	
int far 	PKCS1_EME_Encode	(SF_BYTE far *result, int result_len, 
								 SF_BYTE far *input, int inputLength);

///////////////////////////////////////////////////////////////////////////////
//
//	PKCS1_EME_Decode : 
//
//	output : result			: recovered message, an octet string of length at
//							  most |input|-10.
//	input  : input			: encoded message, an octet string of length
//							  at least 10.
//			 input_len		: octet string length of input.
//
//	return : result_len > 0	: decoding ok
//			 error_code	< 0	: decoding fail
//	
int far 	PKCS1_EME_Decode	(SF_BYTE far *result, SF_BYTE far *input, int inputLength);

///////////////////////////////////////////////////////////////////////////////
//
//	PKCS1_EMSA_Decode : 
//
//	output : result			: recovered message, an octet string of length at
//							  most |input|-10.
//	input  : input			: encoded message, an octet string of length
//							  at least 10.
//			 input_len		: octet string length of input.
//
//	return : result_len > 0	: decoding ok
//			 error_code	< 0	: decoding fail
//	
int far 	PKCS1_EMSA_Decode	(SF_BYTE far *result, SF_BYTE far *input, int inputLength);

///////////////////////////////////////////////////////////////////////////////
//
//	PKCS1_PublicKeyExp : 
//
//	output : output			: pointer to output buffer
//	input  : input			: pointer to input data
//			 inputSize		: length of input
//			 publicKey		: pointer to public key
//
//	return : outputSize		: length of output
//
int far 		PKCS1_PublicKeyExp		(SF_WORD far *output, SF_WORD far *input, int inputSize, 
								 PKCS1_PublicKey far *publicKey);

#if	defined (ENABLE_RSA_PRI)
///////////////////////////////////////////////////////////////////////////////
//
//	PKCS1_PrivateKeyExp : 
//
//	output : output			: pointer to output buffer
//	input  : input			: pointer to input data
//			 inputSize		: length of input
//			 privateKey		: pointer to private key
//
//	return : outputSize		: length of output
//
int		PKCS1_PrivateKeyExp		(SF_WORD *output, SF_WORD *input, int inputSize, 
								 PKCS1_PrivateKey *privateKey);
#endif	// defined (ENABLE_RSA_PRI)

///////////////////////////////////////////////////////////////////////////////
//	
//	PKCS1_RSAES_Encrypt :
//
//	output : cipher			: encrypted message
//	input  : plain			: message to be encrypted, an octet string of
//							  length at most k-11 octets, where k is the length
//							  of the modulus n.
//	         plain_len		: byte length of plain text.
//	         public_key		: RSA public key
//
//	return : cipher_len > 0	: encrypt ok
//			 error_code < 0	: encrypt fail
//
int far		PKCS1_RSAES_Encrypt		(SF_BYTE far *cipher, SF_BYTE far *plain, int plain_len, 
									 PKCS1_PublicKey far *public_key);

#if	defined (ENABLE_RSA_PRI)
///////////////////////////////////////////////////////////////////////////////
//	
//	PKCS1_RSAES_Decrypt :
//
//	output :
//	input  :
//
//	return :
//
int		PKCS1_RSAES_Decrypt		(SF_BYTE *plain, SF_BYTE *cipher, int cipherLength, 
								 PKCS1_PrivateKey *privateKey);

int		PKCS1_EMSA_Encode		(SF_BYTE *output, SF_BYTE *input, int outputLength, 
								 int inputLength, int hashID );
#endif	// defined (ENABLE_RSA_PRI)

///////////////////////////////////////////////////////////////////////////////
//
//	SF_RSASSA_PKCS1_V1_5_UnSign : recover signature message using public key.
//
//	output : recovered_msg	: recovered message
//	input  : public_key		: RSA public key
//			 signature		: signature message
//			 signature_len	: byte length of signature message
//
//	return : msg_len > 0	: recovered message length
//			 error_code < 0	: unsign error
//
int far 	PKCS1_RSASSA_UnSign		(SF_BYTE far *unsign_msg, PKCS1_PublicKey far *public_key, 
									 SF_BYTE far *signature, int signature_len);

//SF_WORD PKCS1_Verify(SF_BYTE *signature, SF_BYTE *hashValue, PKCS1_PublicKey *key, int hashID, int signLength );


#ifdef  __cplusplus
}
#endif

#endif
