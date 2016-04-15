/*****************************************************************************
*                                                                            *
*  ------------------------------- des.h ----------------------------------  *
*                                                                            *
*****************************************************************************/

#ifndef DES_H
#define DES_H

#define DES_LEN						8
/*****************************************************************************
*                                                                            *
*  --------------------------- Public Interface ---------------------------  *
*                                                                            *
*****************************************************************************/

void des_encipher(const unsigned char *plaintext, unsigned char *ciphertext,
   const unsigned char *key);

void des_decipher(const unsigned char *ciphertext, unsigned char *plaintext,
   const unsigned char *key);

#endif
