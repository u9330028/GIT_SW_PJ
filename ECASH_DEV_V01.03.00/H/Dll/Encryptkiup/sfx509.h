//
//	sf_x509.h
//

#ifndef __SF_X509_H__
#define __SF_X509_H__

#include "sfpkcs1.h"


#define	X509_ERR_FILE_MANAGER			-51
#define	X509_ERR_FILE_OPEN_FAILED		-52
#define	X509_ERR_MALLOC_FAIL			-53
#define	X509_ERR_INVALID_INPUT			-54
#define	X509_ERR_RSAKEY_TOO_LARGE		-55
#define	X509_ERR_UNSUPPORTED_CERT		-56
#define	X509_ERR_DECODE_ERROR			-57
#define	X509_ERR_BUFFER_OVERFLOW		-58
#define	X509_ERR_CA_CERT_NOT_FOUND		-59

#ifdef  __cplusplus
extern "C" {
#endif



int far	Cert_Get_PKCS1PublicKey				(uint8 far *issuerDNHash,
											 uint8 far *signatureHash, 
											 uint8 *validity,
											 PKCS1_PublicKey far *publicKey, 
											 uint8 far *cert, uint16 cert_len);
int far	Cert_Verify_n_Get_PKCS1PublicKey	(XM_CLIENT_CONFIG far *config, 
											 PKCS1_PublicKey far *publicKey, 
											 uint8 far *cert, uint16 cert_len);

#ifdef  __cplusplus
}
#endif

#endif


