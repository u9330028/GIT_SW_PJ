/* ********************************************* */
/*  Definition of Crypt                           */
/* ********************************************* */
// DES Functions
//-------------------------------------------------------------------
void Crypt_DesEncrypt(unsigned char *keyPtr, unsigned char *dataInPtr, unsigned char *dataOutPtr);
void Crypt_DesDecrypt(unsigned char *keyPtr, unsigned char *dataInPtr, unsigned char *dataOutPtr);
void Crypt_3DesEncrypt(unsigned char *keyPtr, unsigned char *dataInPtr, unsigned char *dataOutPtr);
void Crypt_3DesDecrypt(unsigned char *keyPtr, unsigned char *dataInPtr, unsigned char *dataOutPtr);

// RSA Function
//-------------------------------------------------------------------
void Crypt_Rsa(unsigned char *msg, unsigned char *key, int kl, unsigned char *exp, int el, unsigned char *out, int *ol);

// SHA1
//-------------------------------------------------------------------
void Crypt_Sha1(unsigned char *dataInPtr, unsigned int dataInLength, unsigned char *dataOutPtr);
