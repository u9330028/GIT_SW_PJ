/*
    xm_protocol.h
*/

#ifndef __XM_PROTOCOL_H__
#define __XM_PROTOCOL_H__

/* size */
#define	KEY_BLOCK_SIZE		(SEED_IV_SIZE + SEED_KEY_SIZE + SHA_DIGEST_LENGTH) * 2

#define	SESSION_ID_SIZE		20
#define	RANDOM_SIZE			20
#define	SECRET_SIZE			48
#define	MASTER_KEY_SIZE		20

#define RANDOM_SEED_MAX     20


/* version */
#define XM2_PROTOCOL_VERSION_MAJOR      0x02
#define XM2_PROTOCOL_VERSION_MINOR      0x00
#define XM2_SERVER_VERSION_MAJOR        0x02
#define XM2_SERVER_VERSION_MINOR        0x00
#define XM2_CLIENT_VERSION_MAJOR        0x02
#define XM2_CLIENT_VERSION_MINOR        0x00

/* profile */
#define XXX2_RSAE_NULL_WITH_AES_SHA1_NULL       0x011100    /* default but not use in korea */
#define XXX2_RSAE_NULL_WITH_AES_MD5_NULL        0x011200
#define XXX2_RSAE_NULL_WITH_SEED_SHA1_NULL      0x012100    /* use */
#define XXX2_RSAE_NULL_WITH_SEED_SHA1_ZLIB      0x012101    /* use */
#define XXX2_RSAE_NULL_WITH_SEED_MD5_NULL       0x012200
#define XXX2_RSAE_NULL_WITH_3DES_SHA1_NULL      0x013100
#define XXX2_RSAE_NULL_WITH_3DES_MD5_NULL       0x013200
#define XXX2_RSAE_RSAS_WITH_AES_SHA1_NULL       0x021100
#define XXX2_RSAE_RSAS_WITH_AES_MD5_NULL        0x021200
#define XXX2_RSAE_RSAS_WITH_SEED_SHA1_NULL      0x022100
#define XXX2_RSAE_RSAS_WITH_SEED_MD5_NULL       0x022200
#define XXX2_RSAE_RSAS_WITH_3DES_SHA1_NULL      0x023100
#define XXX2_RSAE_RSAS_WITH_3DES_MD5_NULL       0x023200


/*
enum {
    KEYINIT(1), KEYFINAL(2), MESSAGE(3), ALERT(4),
    NEW_PROFILE(10), 
    RESUME_INIT(20), RESUME_FINAL(21), NO_KEY(22),
    SESSION_CLOSE(100), MASTER_SESSION_CLOSE(100), (255)
} ContentType;
*/
#define	XM2_CT_KEYINIT					1
#define	XM2_CT_KEYFINAL					2
#define	XM2_CT_MESSAGE					3
#define	XM2_CT_ALERT					4
#define	XM2_CT_NEW_PROFILE				10
#define	XM2_CT_RESUME_INIT				20
#define	XM2_CT_RESUME_FINAL				21
#define	XM2_CT_NO_KEY					22
#define	XM2_CT_SESSION_CLOSE			100
#define	XM2_CT_MASTER_SESSION_CLOSE		101

/**************************************************************
enum {
	cert (1) , auth_type(2) , both(3),  (255)
} UpdateType ;
***************************************************************/
#define	XM_UT_CERT			1
#define	XM_UT_AUTH_TYPE		2
#define	XM_UT_BOTH			3

#endif
