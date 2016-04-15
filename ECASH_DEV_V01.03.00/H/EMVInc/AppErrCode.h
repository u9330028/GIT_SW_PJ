//Error about All Progress

//--------------------------------------------------------------
// emv return code
//--------------------------------------------------------------
#define     RSLT_SUCCESS		1
#define     RSLT_ERROR    		-1
#define     RSLT_FAIL 			0

#define		OK					1
#define		FAIL				0

//--------------------------------------------------------------
// emv Error
//--------------------------------------------------------------
//Tlv Error
#define TLV_DUPLICATE			11
#define ICC_DATA_FULL			12
#define TERM_DATA_FULL			13
#define TRANS_DATA_FULL			14
#define NOT_FOUND_TLV			15
#define NOT_FOUND_TLV2			16
#define NOT_RECOGNIZED			17
#define INVALID_LEN				18
#define INVALID_TAG				19

/*----- DATA AUTH         ------*/
#define	GET_CAPUBKEY			51		// Not found CA Key with RID and index

#define	IS_CERT_LEN 			52		// Issuer Certificate Len != Modulus len
#define	IS_TRAILER_ERR 			53		// Issuer Data Trailer != DATA_TRAILER 
#define	IS_HEADER_ERR 			54		// Issuer Data Header != DATA_HEADER 
#define	IS_CERT_FORM 			55		// Issuer Certificate Format Err 
#define	IS_HASH_ALG 			56		// Issuer Hash Algorithm Indicator Err 
#define	IS_HASH_ERR 			57		// Issuer Hash Result Err 
#define	IS_ISID_NO 				58		// Issuer Identification Number Err 
#define	IS_CERT_EXP_DATE 		59		// Issuer Certific Expiration Date Err 
#define	IS_PK_ALG         		60		// Issuer Pub Key Alg Indicator Err 

#define	IC_CERT_LEN 			61		// ICC Certificate Len != Modulus len
#define	IC_TRAILER_ERR 			62		// ICC Data Trailer != DATA_TRAILER 
#define	IC_HEADER_ERR 			63		// ICC Data Header != DATA_HEADER 
#define	IC_CERT_FORM 			64		// ICC Certificate Format Err 
#define	IC_HASH_ALG 			65		// ICC Hash Algorithm Indicator Err 
#define	IC_HASH_ERR 			66		// ICC Hash Result Err 
#define	IC_PAN     				67		// ICC Application PAN Err 
#define	IC_CERT_EXP_DATE 		68		// ICC Certific Expiration Date Err 
#define	IC_PK_ALG         		69		// ICC Pub Key Alg Indicator Err 

#define	SN_CERT_LEN 			71		// Signed Static App Data Length
#define	SN_TRAILER_ERR 			72		// Signed SData Trailer != DATA_TRAILER 
#define	SN_HEADER_ERR 			73		// Signed SData Header != DATA_HEADER 
#define	SN_CERT_FORM 			74		// Signed SCertificate Format Err 
#define	SN_HASH_ALG 			75		// Signed SHash Algorithm Indicator Err 
#define	SN_HASH_ERR 			76		// Signed SHash Result Err 

#define	DY_DDOL_NOT 			81		// Both DDOL not found
#define	DY_INT_AUTH 			82		// Internal Authentication Err
#define	DY_CERT_LEN 			83		// Signed Dynamic App Data Length
#define	DY_TRAILER_ERR 			84		// Signed DData Trailer != DATA_TRAILER 
#define	DY_HEADER_ERR 			85		// Signed DData Header != DATA_HEADER 
#define	DY_CERT_FORM 			86		// Signed DCertificate Format Err 
#define	DY_HASH_ALG 			87		// Signed DHash Algorithm Indicator Err 
#define	DY_HASH_ERR 			88		// Signed DHash Result Err 
#define	DY_INVALID_DDOL 		89		// Unpredict No not found in DDOL. 

#define	CVM_TIMEOUT 			91		// Bypassed PIN entry             
#define	CVM_CANCEL  			92		// Pressed 'Cancel' Key           
#define	CVM_PINBLK  			93		// Pin Block making error 
#define	CVM_NOT_SUPPORT			94		// Not support encipher offline PIN

#define	INVALID_CRYPT			101		// Invalid cryptogram from 1st GenAC
#define	INVALID_SCRIPT			111		// Invalid script format

#define	INVALID_DATA			112		// Invalid data

#define CDA_DY_CERT_LEN         121     // Signed Dynamic App Data Length at CDA
#define CDA_DY_TRAILER_ERR      122     // Signed DData Trailer != DATA_TRAILER at CDA
#define CDA_DY_HEADER_ERR       123     // Signed DData Header != DATA_HEADER at CDA
#define CDA_DY_CERT_FORM        124     // Signed DCertificate Format Err at CDA
#define CDA_DY_INVALID_CID      125     // Signed DData CID != CID from GENERATE AC at CDA
#define	CDA_DY_HASH_ALG 		126		// Signed DHash Algorithm Indicator Err at CDA
#define	CDA_DY_HASH_ERR 		127 	// Signed DHash Result Err at CDA

//emv Error
#define ERROR_CODE				0xD2

#define ERROR_CARD				(-200)
#define ERROR_NO_CARD			(-201)
#define ERROR_CARD_DATA			(-202)
#define ERROR_TERM_DATA			(-203)

#define ONLINE_DATA_ERROR		(-204)
#define ONLINE_ERROR			(-205)

#define EMV_NOT_SUPPORT			(-300)
#define EMV_NOT_ACCEPT			(-301)
#define EMV_FALLBACK			(-302)
#define	EMV_REVERSAL_MSG		(-400)
#define	EMV_CARDCAPTURE			(-500)

#define	EMV_TR_APPROVED			7
#define	EMV_TR_DECLINED			8
