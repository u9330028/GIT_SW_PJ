// SeedDws.cpp : implementation file
//

#include "stdafx.h"
#include "SeedDws.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeedDws

/******************* Encryption/Decryption *******************/
#define BIG_ENDIAN
#define GetB0(A)  ( (BYTE)((A)    ) )
#define GetB1(A)  ( (BYTE)((A)>> 8) )
#define GetB2(A)  ( (BYTE)((A)>>16) )
#define GetB3(A)  ( (BYTE)((A)>>24) )

#define SeedRound(L0, L1, R0, R1, K) {             \
    T0 = R0 ^ (K)[0];                              \
    T1 = R1 ^ (K)[1];                              \
    T1 ^= T0;                                      \
    T1 = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^         \
         SS2[GetB2(T1)] ^ SS3[GetB3(T1)];          \
    T0 += T1;                                      \
    T0 = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^         \
         SS2[GetB2(T0)] ^ SS3[GetB3(T0)];          \
    T1 += T0;                                      \
    T1 = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^         \
         SS2[GetB2(T1)] ^ SS3[GetB3(T1)];          \
    T0 += T1;                                      \
    L0 ^= T0; L1 ^= T1;                            \
}


#define EndianChange(dwS)                       \
    ( (ROTL((dwS),  8) & (DWORD)0x00ff00ff) |   \
      (ROTL((dwS), 24) & (DWORD)0xff00ff00) )


/************************ Block Encryption *************************/
void CSeedDws::SeedEncrypt(BYTE *pbData, DWORD *pdwRoundKey)
{
	DWORD L0, L1, R0, R1, T0, T1, *K = pdwRoundKey;

    L0 = ((DWORD *)pbData)[0];
    L1 = ((DWORD *)pbData)[1];
	R0 = ((DWORD *)pbData)[2];
    R1 = ((DWORD *)pbData)[3];
#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif

    SeedRound(L0, L1, R0, R1, K   ); /*   1 */
    SeedRound(R0, R1, L0, L1, K+ 2); /*   2 */
    SeedRound(L0, L1, R0, R1, K+ 4); /*   3 */
    SeedRound(R0, R1, L0, L1, K+ 6); /*   4 */
    SeedRound(L0, L1, R0, R1, K+ 8); /*   5 */
    SeedRound(R0, R1, L0, L1, K+10); /*   6 */
    SeedRound(L0, L1, R0, R1, K+12); /*   7 */
    SeedRound(R0, R1, L0, L1, K+14); /*   8 */
    SeedRound(L0, L1, R0, R1, K+16); /*   9 */
    SeedRound(R0, R1, L0, L1, K+18); /*  10 */
    SeedRound(L0, L1, R0, R1, K+20); /*  11 */
    SeedRound(R0, R1, L0, L1, K+22); /*  12 */
#if NoRounds == 16
    SeedRound(L0, L1, R0, R1, K+24); /*  13 */
    SeedRound(R0, R1, L0, L1, K+26); /*  14 */
    SeedRound(L0, L1, R0, R1, K+28); /*  15 */
    SeedRound(R0, R1, L0, L1, K+30); /*  16 */
#endif

#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif
    ((DWORD *)pbData)[0] = R0;
    ((DWORD *)pbData)[1] = R1;
    ((DWORD *)pbData)[2] = L0;
    ((DWORD *)pbData)[3] = L1;
}

CSeedDws::CSeedDws()
{
	memset(pbSeedUserKey, 0x00, sizeof(pbSeedUserKey));
	memcpy(pbSeedUserKey, "SEC@BANK#ENC3KE2", strlen("SEC@BANK#ENC3KE2"));
	memset(prSeedRoundKey, 0x00, sizeof(prSeedRoundKey));
}

/* same as encrypt, except that round keys are applied in reverse order. */
void CSeedDws::SeedDecrypt(BYTE *pbData, DWORD *pdwRoundKey)
{
    DWORD L0, L1, R0, R1, T0, T1, *K=pdwRoundKey;

    L0 = ((DWORD *)pbData)[0];
    L1 = ((DWORD *)pbData)[1];
    R0 = ((DWORD *)pbData)[2];
    R1 = ((DWORD *)pbData)[3];
#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif

#if NoRounds == 16
    SeedRound(L0, L1, R0, R1, K+30); /*   1 */
    SeedRound(R0, R1, L0, L1, K+28); /*   2 */
    SeedRound(L0, L1, R0, R1, K+26); /*   3 */
    SeedRound(R0, R1, L0, L1, K+24); /*   4 */
#endif
    SeedRound(L0, L1, R0, R1, K+22); /*   5 */
    SeedRound(R0, R1, L0, L1, K+20); /*   6 */
    SeedRound(L0, L1, R0, R1, K+18); /*   7 */
    SeedRound(R0, R1, L0, L1, K+16); /*   8 */
    SeedRound(L0, L1, R0, R1, K+14); /*   9 */
    SeedRound(R0, R1, L0, L1, K+12); /*  10 */
    SeedRound(L0, L1, R0, R1, K+10); /*  11 */
    SeedRound(R0, R1, L0, L1, K+ 8); /*  12 */
    SeedRound(L0, L1, R0, R1, K+ 6); /*  13 */
    SeedRound(R0, R1, L0, L1, K+ 4); /*  14 */
    SeedRound(L0, L1, R0, R1, K+ 2); /*  15 */
    SeedRound(R0, R1, L0, L1, K+ 0); /*  16 */

#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif
    ((DWORD *)pbData)[0] = R0;
    ((DWORD *)pbData)[1] = R1;
    ((DWORD *)pbData)[2] = L0;
    ((DWORD *)pbData)[3] = L1;
}



/******************** Key Scheduling **********************/

/* Constants for key schedule:
KC0 = golden ratio; KCi = ROTL(KCi-1, 1) */
#define KC0     0x9e3779b9UL
#define KC1     0x3c6ef373UL
#define KC2     0x78dde6e6UL
#define KC3     0xf1bbcdccUL
#define KC4     0xe3779b99UL
#define KC5     0xc6ef3733UL
#define KC6     0x8dde6e67UL
#define KC7     0x1bbcdccfUL
#define KC8     0x3779b99eUL
#define KC9     0x6ef3733cUL
#define KC10    0xdde6e678UL
#define KC11    0xbbcdccf1UL
#if NoRounds == 16
#define KC12    0x779b99e3UL
#define KC13    0xef3733c6UL
#define KC14    0xde6e678dUL
#define KC15    0xbcdccf1bUL
#endif

#define EncRoundKeyUpdate0(K, A, B, C, D, KC) {  \
    T0 = A;                                      \
    A = (A>>8) ^ (B<<24);                        \
    B = (B>>8) ^ (T0<<24);                       \
    T0 = A + C - KC;                             \
    T1 = B + KC - D;                             \
    (K)[0] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^   \
             SS2[GetB2(T0)] ^ SS3[GetB3(T0)];    \
    (K)[1] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^   \
             SS2[GetB2(T1)] ^ SS3[GetB3(T1)];    \
}


#define EncRoundKeyUpdate1(K, A, B, C, D, KC) {  \
    T0 = C;                                      \
    C = (C<<8) ^ (D>>24);                        \
    D = (D<<8) ^ (T0>>24);                       \
    T0 = A + C - KC;                             \
    T1 = B + KC - D;                             \
    (K)[0] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^   \
             SS2[GetB2(T0)] ^ SS3[GetB3(T0)];    \
    (K)[1] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^   \
             SS2[GetB2(T1)] ^ SS3[GetB3(T1)];    \
}


/* Encryption key schedule */
void CSeedDws::SeedEncRoundKey(DWORD *pdwRoundKey, BYTE *pbUserKey)
{
  DWORD A, B, C, D, T0, T1, *K=pdwRoundKey;

  A = ((DWORD *)pbUserKey)[0];
  B = ((DWORD *)pbUserKey)[1];
  C = ((DWORD *)pbUserKey)[2];
  D = ((DWORD *)pbUserKey)[3];
#ifndef BIG_ENDIAN
  A = EndianChange(A);
  B = EndianChange(B);
  C = EndianChange(C);
  D = EndianChange(D);
#endif

    T0 = A + C - KC0;
    T1 = B - D + KC0;
    K[0] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^
           SS2[GetB2(T0)] ^ SS3[GetB3(T0)];
    K[1] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^
           SS2[GetB2(T1)] ^ SS3[GetB3(T1)];

    EncRoundKeyUpdate0(K+ 2, A, B, C, D, KC1 );
    EncRoundKeyUpdate1(K+ 4, A, B, C, D, KC2 );
    EncRoundKeyUpdate0(K+ 6, A, B, C, D, KC3 );
    EncRoundKeyUpdate1(K+ 8, A, B, C, D, KC4 );
    EncRoundKeyUpdate0(K+10, A, B, C, D, KC5 );
    EncRoundKeyUpdate1(K+12, A, B, C, D, KC6 );
    EncRoundKeyUpdate0(K+14, A, B, C, D, KC7 );
    EncRoundKeyUpdate1(K+16, A, B, C, D, KC8 );
    EncRoundKeyUpdate0(K+18, A, B, C, D, KC9 );
    EncRoundKeyUpdate1(K+20, A, B, C, D, KC10);
    EncRoundKeyUpdate0(K+22, A, B, C, D, KC11);
#if NoRounds == 16
    EncRoundKeyUpdate1(K+24, A, B, C, D, KC12);
    EncRoundKeyUpdate0(K+26, A, B, C, D, KC13);
    EncRoundKeyUpdate1(K+28, A, B, C, D, KC14);
    EncRoundKeyUpdate0(K+30, A, B, C, D, KC15);
#endif
}


/* aaaack but it's fast and const should make it shared text page. */
static const unsigned char pr2six[256] =
{
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

int CSeedDws::base64DecodeLen(const char *bufcoded) {
	int nbytesdecoded;
	register const unsigned char *bufin;
	register int nprbytes;

	bufin = (const unsigned char *) bufcoded;
	while (pr2six[*(bufin++)] <= 63);

	nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
	nbytesdecoded = ((nprbytes + 3) / 4) * 3;

	return nbytesdecoded + 1;
}

int CSeedDws::base64Decode(char *bufplain, const char *bufcoded) {
	int len;

	len = base64DecodeBinary((unsigned char *) bufplain, bufcoded);
	bufplain[len] = '\0';
	return len;
}

/* This is the same as ap_base64udecode() except on EBCDIC machines, where
 *  * the conversion of the output to ebcdic is left System.out.println(
 *   */
int CSeedDws::base64DecodeBinary(unsigned char *bufplain, const char *bufcoded) {
	int nbytesdecoded;
	register const unsigned char *bufin;
	register unsigned char *bufout;
	register int nprbytes;

	bufin = (const unsigned char *) bufcoded;
	while (pr2six[*(bufin++)] <= 63);
	nprbytes = (bufin - (const unsigned char *)bufcoded) - 1;
	nbytesdecoded = ((nprbytes + 3) / 4) * 3;

	bufout = (unsigned char *)bufplain;
	bufin = (const unsigned char *) bufcoded;

	while (nprbytes > 4) {
		*(bufout++) = 
			(unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
		*(bufout++) = 
			(unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
		*(bufout++) = 
			(unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
		bufin += 4;
		nprbytes -= 4;
	}

	/* Note: (nprbytes == 1) would
	 * be an error, so just ingore
	 * that case */
	if (nprbytes > 1) {
		*(bufout++) = 
			(unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
	}
	if (nprbytes > 2) {
		*(bufout++) = 
			(unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
	}
	if (nprbytes > 3) {
		*(bufout++) = 
			(unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
	}

	nbytesdecoded -= (4 - nprbytes) & 3;
	return nbytesdecoded;
}

static const char basis_64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int CSeedDws::base64EncodeLen(int len)
{
    return ((len + 2) / 3 * 4) + 1;
}

int CSeedDws::base64Encode(char *encoded, const char *string, int len)
{
    return base64EncodeBinary(encoded, (const unsigned char *) string, len);
}

/* This is the same as ap_base64encode() except on EBCDIC machines, where
 * the conversion of the input to ascii is left out.
 */
int CSeedDws::base64EncodeBinary(char *encoded,
						const unsigned char *string, int len)
{
    int i;
    char *p;

    p = encoded;
    for (i = 0; i < len - 2; i += 3) {
	*p++ = basis_64[(string[i] >> 2) & 0x3F];
	*p++ = basis_64[((string[i] & 0x3) << 4) |
	                ((int) (string[i + 1] & 0xF0) >> 4)];
	*p++ = basis_64[((string[i + 1] & 0xF) << 2) |
	                ((int) (string[i + 2] & 0xC0) >> 6)];
	*p++ = basis_64[string[i + 2] & 0x3F];
    }
    if (i < len) {
	*p++ = basis_64[(string[i] >> 2) & 0x3F];
	if (i == (len - 1)) {
	    *p++ = basis_64[((string[i] & 0x3) << 4)];
	    *p++ = '=';
	}
	else {
	    *p++ = basis_64[((string[i] & 0x3) << 4) |
	                    ((int) (string[i + 1] & 0xF0) >> 4)];
	    *p++ = basis_64[((string[i + 1] & 0xF) << 2)];
	}
	*p++ = '=';
    }

    *p++ = '\0';
    return p - encoded;
}

int CSeedDws::base64encode_buf (char *buf, int buflen, char *result, int *resultlen) {

    char* pTemp;
        if (!buf || buflen < 0 || !result || !resultlen) return -1;


    pTemp = (char *)malloc(base64EncodeLen(buflen) + 1);
    if (pTemp == NULL)
        return -1;

        base64EncodeBinary(pTemp, (BYTE*)buf, buflen);
        strcpy(result, pTemp);
        *resultlen = strlen (result);

        if (pTemp != NULL)
        free(pTemp);

        return 0;
}


int CSeedDws::base64decode_buf (char *buf, int buflen, char *result, int *resultlen) {

    char* pTempBuf = NULL;

    if (!buf || buflen < 0 || !result || !resultlen) return -1;

    pTempBuf = (char*) malloc(buflen + 1);

    strncpy(pTempBuf, buf, buflen);

        *resultlen = base64DecodeBinary((BYTE*)result, pTempBuf);

        result [*resultlen] = '\0';
        if (pTempBuf != NULL)
            free(pTempBuf);

    return 0;
}


void CSeedDws::SeedEncryptCBC(char *bpData, int inlen, BYTE *bpIV)
{
	int	i, j;

	BYTE	bTEMP[DIVISOR];

	for( i = 0 ; i < inlen / DIVISOR ; i++ )
	{
		memset( bTEMP,	0x00,	DIVISOR );
		memcpy( bTEMP,	bpData + (i*DIVISOR), DIVISOR);

		if(i == 0)
			for(j = 0; j < DIVISOR; j++) bTEMP[j] ^= bpIV[j];
		else
			for(j = 0; j < DIVISOR; j++) bTEMP[j] ^= bpData[(i-1)*DIVISOR + j];

		SeedEncrypt(bTEMP, prSeedRoundKey);

		memcpy(bpData + (i*DIVISOR), bTEMP, DIVISOR);
	}

	return ;
}

void CSeedDws::SeedDecryptCBC(BYTE *bpData, int ilen, BYTE *bpIV)
{
	int 	i, j;

	BYTE	bTEMP[DIVISOR], bPrev[DIVISOR];
	
	for( i = 0 ; i < ilen/DIVISOR ; i++)
	{		
		memset(bTEMP, 	0x00, 	DIVISOR);		
		memcpy(bTEMP, 	bpData + (i*DIVISOR), DIVISOR);    	

		SeedDecrypt(bTEMP, prSeedRoundKey);

		if(i == 0)
			for(j = 0; j < DIVISOR; j++) bTEMP[j] ^= bpIV[j];
		else		
			for(j = 0; j < DIVISOR; j++) bTEMP[j] ^= bPrev[j];

		memcpy(bPrev,	bpData + (i*DIVISOR), DIVISOR);
		memcpy(bpData + (i*DIVISOR), bTEMP, DIVISOR);
	}

	return ;
}

void CSeedDws::SeedEncCBCPKCS5(BYTE *bpData, int *olen, int ilen, BYTE *bpIV)
{
	int	plen, i;	

	BYTE	pc;


	plen = DIVISOR - ilen % DIVISOR;

	if (plen == 16) plen = 0;

	(*olen) = ilen + plen;

	pc = (BYTE)plen;

	for(i = 0 ; i < plen ; i++)
		bpData[ilen+i] = pc;	

	SeedEncryptCBC((char *)bpData, (int)(ilen+plen), bpIV);
}

void CSeedDws::SeedDecCBCPKCS5(BYTE *bpData, int *olen, int ilen, BYTE *bpIV)
{
	int	plen;	

	SeedDecryptCBC(bpData, ilen, bpIV);

	plen = (int)bpData[ilen-1];

	if(plen <= 0 || plen > DIVISOR)
		(*olen) = ilen;		/* Not Padding */
	else
		(*olen) = ilen - plen;

	return ;
}

/*--------------------------------------------------------------------------*/
/* Seed를 이용한 암호화 모듈                                                */
/*        char *indata    -- 암호화 대상이 되는 원 DATA                     */
/*        int  inlen     -- 원 DATA의 길이                                 */
/*        char *outdata   -- 암호화 된 DATA를 넣을 Buffer                   */
/*        int  outlen    -- 암호화 된 DATA의 길이                          */
/*--------------------------------------------------------------------------*/
int CSeedDws::SeedEncryptData(char *indata, int inlen, char *outdata, int *olen)
{
	BYTE	bpIV[16] = {0x00,};
	int	rtn, osize, rsize = 0;

	char 	buf[DATA_MAX_SIZE] = {0x00,};

	SeedEncRoundKey(prSeedRoundKey, (unsigned char *)pbSeedUserKey);

	memcpy(buf,	indata,	inlen);

	SeedEncCBCPKCS5((BYTE *)buf, &rsize, inlen, bpIV);

        rtn = base64encode_buf((char*)buf, rsize, (char*)outdata, &osize);
        if(rtn < 0)
        {
            printf("Base64 Encode Error!\n");
            return 1;
        }

        *olen = osize;

	return 0;
}

/*--------------------------------------------------------------------------*/
/* Seed를 이용한 복호화 모듈                                                */
/*        char *indata    -- 복호화 대상이 되는 원 DATA                     */
/*        int  inlen     -- 원 DATA의 길이                                 */
/*        char *outdata   -- 복호화 된 DATA를 넣을 Buffer                   */
/*        int  outlen    -- 복호화 된 DATA의 길이                          */
/*--------------------------------------------------------------------------*/
int CSeedDws::SeedDecryptData(char *indata, int inlen, char *outdata, int *olen)
{
	BYTE	bpIV[16] = {0x00,};
	int	rtn, osize, rsize = 0;

	char	buf[DATA_MAX_SIZE] = {0x00,};

	SeedEncRoundKey(prSeedRoundKey, (unsigned char *)pbSeedUserKey);

	memcpy( buf,	indata,		inlen	);

        rtn = base64decode_buf((char*)buf, inlen, (char*)outdata, &osize);
        if(rtn < 0)
        {
            printf("Base64 Decode Error!\n");
            return 1;
        }

	memcpy(buf,	outdata,	osize);
	memset(outdata, 0x00, inlen);

	SeedDecCBCPKCS5((BYTE *)buf, &rsize, osize, bpIV);

	memcpy( outdata,	buf,	rsize);
	*olen = rsize;

	return 0;
}