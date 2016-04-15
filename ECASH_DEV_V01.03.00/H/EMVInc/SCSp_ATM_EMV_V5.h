/***************************************************************************
                    SCSp_ATM_EMV.h  -  description
                         -------------------
copyright            : (C) 2013 by SCSpro
***************************************************************************/
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define		EMV_DEBUG				1			// 디버깅 시에만 사용

//---------------------------  DEFINE RESULT VALUE ------------------------//
/************************************************************************/

#define     EMV_RSLT_OK     		1
#define     EMV_RSLT_FAIL 			0
#define     EMV_RSLT_ERROR    		-1
#define		EMV_RSLT_CANCEL			-2

#define		EMV_EQUAL    			2
#define		EMV_GREAT         		3
#define		EMV_LESS         		4
#define		EMV_CANCEL				5
#define		EMV_APPROVED			6
#define		EMV_DECLINED			7
#define		EMV_REVERSAL			8

//----------------------------  DEFINE EMV Error --------------------------//
#define 	EMV_ERR_BASE            0
        	
#define 	EMV_ERR_CARD_SW			(EMV_ERR_BASE - 200)
#define 	EMV_ERR_NO_CARD			(EMV_ERR_BASE - 201)
#define 	EMV_ERR_CARD_DATA		(EMV_ERR_BASE - 202)
#define 	EMV_ERR_TERM_DATA		(EMV_ERR_BASE - 203)
#define 	EMV_ERR_ONLINE_DATA		(EMV_ERR_BASE - 204)
#define 	EMV_ERR_CARD_BLOCK		(EMV_ERR_BASE - 205)
#define 	EMV_ERR_NOT_SUPPORT		(EMV_ERR_BASE - 300)
#define 	EMV_ERR_NOT_ACCEPT		(EMV_ERR_BASE - 301)

#define 	ICEMV_APP_CNT			255				//VISA, MASTER, JCB, NonEMV etc..
        	                                   		
#define		CA_CNT					6				//VISA, MASTER, LOCAL_EMV
#define		CAPK_CNT				8       		
        	                		        		
#define		MAX_CAKEY				248				// Max CA Plu Key Length
#define		MAX_ISKEY				248				// Max Issuer Plu Key Length
#define		MAX_ICKEY				248				// Max ICC Plu Key Length

#define		MAX_LABEL_AID_LEGNTH	16				// Max Label or AID Length

// define of Emv Option
#define		USER_CONFRM_SUPPOPT		0x01
#define		PARTIAL_AID_SUPPOPT		0x08
#define		PSE_SUPPOPT				0x80
#define		FORCEDONLINE_SUPPOPT	0x40
#define		EXCEPTION_SUPPOPT		0x20
#define		NOT_SUPPORT				0x00

/************************************************************************/
/*	DEFINE BER-TLV Format Size  										*/             			
/************************************************************************/
#define 	ONEBYTE_SIZE			256
#define 	HASH_SIZE				20  			// Hash Result Size
                                	
//---------------------- DEFINITION Transaction type ----------------------------//
#define 	TRTYPE_GOODSERVICE 		0x00			// Purchase of goods or service
#define 	TRTYPE_CASH				0x01			// Cash Disbursement
#define 	TRTYPE_CASHBACK 		0x09			// Purchase with Cash Back
#define 	TRTYPE_INQUIRY			0x30			// Inquiry 
#define 	TRTYPE_PAYMENT			0x19			// Payment    
#define 	TRTYPE_TRANSFER			0x40			// Transfer  

#define 	PSE						"1PAY.SYS.DDF01"

//---------------------- Definition of Cryptogram ----------------------------------------//
#define 	AAC 					0x00			// Declined
#define 	TC  					0x40			// Approved
#define 	ARQC					0x80			// Go to online
#define 	AAR 					0xC0			// Referral
                            		
//-----------------------Definition Transaction Type-------------------------------------//
#define		ONLINE_TRANS			0x8F
#define		OFFLINE_TRANS   		0x4F
#define		REVERSAL_TRANS   		0x1F

//-----------------------Definition Issuer or Acquirer Response Code -------------------------------------//
#define		ISSUER_APPROVAL_VALUE	"00"
#define		ISSUER_DECLINE_VALUE	"05"
#define		ISSUER_REFERAL_VALUE	"01"

//------------------------------- DEFINE EMV TLV ID -----------------------//
#define 	EMVTid_AcquirerId       0          		// Tag "9F01" - Acquirer Identifier
#define 	EMVTid_AddTerminalCapa  1               // Tag "9F40" - Additional Terminal Capability                    
#define 	EMVTid_AmountAuthB      2               // Tag "8100" - Amount Authorized (Binary)                        
#define 	EMVTid_AmountAuthN      3               // Tag "9F02" - Amount Authorized (Numeric)                       
#define 	EMVTid_AmountOtherB     4               // Tag "9F04" - Amount Other (Binary)                             
#define 	EMVTid_AmountOtherN     5               // Tag "9F03" - Amount Other (Numeric)                            
#define 	EMVTid_AmountRefCurB    6               // Tag "9F3A" - Amount Reference Currency                         
#define 	EMVTid_AmountTrans      7               // Tag "DF01" - Amount Transaction                                
#define 	EMVTid_AC               8               // Tag "9F26" - Application Cryptogram                            
#define 	EMVTid_AppCurCod        9               // Tag "9F42" - Application Currency Code                         
                                                                                                                
#define 	EMVTid_AppCurExp        10         		// Tag "9F44" - Application Currency Exponent                     
#define 	EMVTid_AppDisData       11              // Tag "9F05" - Application Discretionary Data                    
#define 	EMVTid_AppEffDate       12              // Tag "5F25" - Application Effective Date                        
#define 	EMVTid_AppExpiredDate   13              // Tag "5F24" - Application Expired Date                          
#define 	EMVTid_AFL              14              // Tag "9400" - Application File Locator                          
#define 	EMVTid_AID              15              // Tag "4F00" - Application Identifier                            
#define 	EMVTid_AIP              16              // Tag "8200" - Application Interchange Profile                   
#define 	EMVTid_AppLabel         17              // Tag "5000" - Application Label                                 
#define 	EMVTid_AppPreferName    18              // Tag "9F12" - Application Preferred Name                        
#define 	EMVTid_PAN              19              // Tag "5A00" - Primary Account Number                            
                                                                                                                
#define 	EMVTid_PANSeqNo         20         		// Tag "5F34" - Primary Account Sequence Number                   
#define 	EMVTid_AppPriorityInd   21              // Tag "8700" - Application Priority Indicator                    
#define 	EMVTid_AppRefCur        22              // Tag "9F3B" - Application Reference Currency                    
#define 	EMVTid_AppRefCurExp     23              // Tag "9F43" - Application Reference Currency Exponent           
#define 	EMVTid_ATC              24              // Tag "9F36" - Application Transaction Counter                   
#define 	EMVTid_AUC              25              // Tag "9F07" - Application Usage Control                         
#define 	EMVTid_ICAppVerNo       26              // Tag "9F08" - ICC Application Version Number                    
#define 	EMVTid_TrAppVerNo       27              // Tag "9F09" - Terminal Application Version Number               
#define 	EMVTid_AuthRespCod      28              // Tag "8A00" - Authrization Response Code                        
#define 	EMVTid_CDOL1           	29              // Tag "8C00" - CDOL1                                             
                                                                                                                
#define 	EMVTid_CDOL2            30         		// Tag "8D00" - CDOL2                                             
#define 	EMVTid_HolderName       31              // Tag "5F20" - Cardholder Name                                   
#define 	EMVTid_HolderNameExt    32              // Tag "9F0B" - Cardholder Name Extended                          
#define 	EMVTid_CVMList          33              // Tag "8E00" - Cardholder Verification Method List               
#define 	EMVTid_CVMRslt          34              // Tag "9F34" - Cardholder Verification Method Result             
#define 	EMVTid_CAPubKeyIdx      35              // Tag "8F00" - CA Public Key Index                               
#define 	EMVTid_CryptInfData     36              // Tag "9F27" - Cryptogram Information Data                       
#define 	EMVTid_DataAuthCod      37              // Tag "9F45" - Data Authorization Code                           
#define 	EMVTid_DFName           38              // Tag "8400" - DF Name                                           
#define 	EMVTid_DefaultDDOL      39              // Tag "D600" - Default DDOL                                      
                                                                                                                
#define 	EMVTid_DefaultTDOL      40         		// Tag "D700" - Default TDOL                                      
#define 	EMVTid_EncipherPINData  41              // Tag "DF02" - Enciphered PIN Data                               
#define 	EMVTid_DDOL             42              // Tag "9F49" - DDOL                                              
#define 	EMVTid_FciIsuDisData    43              // Tag "BF0C" - FCI Issuer Discretionary Data                     
#define 	EMVTid_IccDynamicNum    44              // Tag "9F4C" - ICC Dynamic Data                                  
#define 	EMVTid_IccPINPubKeyCert 45              // Tag "9F2D" - ICC PIN Public Key Certificate                    
#define 	EMVTid_IccPINPubKeyExp  46              // Tag "9F2E" - ICC PIN Public Key Exponent                       
#define 	EMVTid_IccPINPubKeyRem  47              // Tag "9F2F" - ICC PIN Public Key Remainder                      
#define 	EMVTid_IccPubKeyCert    48              // Tag "9F46" - ICC Public Key Certificate                        
#define 	EMVTid_IccPubKeyExp     49              // Tag "9F47" - ICC Public Key Exponent                           
                                                                                                                
#define 	EMVTid_IccPubKeyRem     50         		// Tag "9F48" - ICC Public Key Remainder                          
#define 	EMVTid_IFDSerialNo      51              // Tag "9F1E" - IFD Serial Number                                 
#define 	EMVTid_IACDefault       52              // Tag "9F0D" - Issuer Action Code Default                        
#define 	EMVTid_IACDenial        53              // Tag "9F0E" - Issuer Action Code Denial                         
#define 	EMVTid_IACOnline        54              // Tag "9F0F" - Issuer Action Code Online                         
#define 	EMVTid_IsuAppData       55              // Tag "9F10" - Issuer Application Date                           
#define 	EMVTid_IsuAuthData      56              // Tag "9100" - Issuer Authorization Data                         
#define 	EMVTid_IsuCodTblIdx     57              // Tag "9F11" - Issuer Code Table Index                           
#define 	EMVTid_IsuCountryCod    58              // Tag "5F28" - Issuer Country Code                               
#define 	EMVTid_IsuPubKeyCert    59              // Tag "9000" - Issuer Public Key Certificate                     
                                               		                                                            
#define 	EMVTid_IsuPubKeyExp     60         		// Tag "9F32" - Issuer Public Key Exponent                        
#define 	EMVTid_IsuPubKeyRem     61              // Tag "9200" - Issuer Public Key Remainder                       
#define 	EMVTid_IsuScriptId      62              // Tag "9F18" - Issuer Script Identifier                          
#define 	EMVTid_IsuScriptRslt    63              // Tag "DF03" - Issuer Script Result                              
#define 	EMVTid_IsuScriptTemp1   64              // Tag "7100" - Issuer Script Template 1                          
#define 	EMVTid_IsuScriptTemp2   65              // Tag "7200" - Issuer Script Template 2                          
#define 	EMVTid_LangPrefer       66              // Tag "5F2D" - Language Preference                               
#define 	EMVTid_LastOnlineATC    67              // Tag "9F13" - Last Online Application Transaction Counter       
#define 	EMVTid_LowOffLimit      68              // Tag "9F14" - Lower Offline Limit                               
#define 	EMVTid_MCC              69              // Tag "9F15" - Merchant Category Code                            
                                                                                                                
#define 	EMVTid_MerchantId       70       		// Tag "9F16" - Merchant Identifier                               
#define 	EMVTid_MerchantNameLoc  71              // Tag "DF04" - Merchant Name Location                            
#define 	EMVTid_MsgType          72              // Tag "DF05" - Message Type                                      
#define 	EMVTid_POSEntryMode     73              // Tag "9F39" - POS Entry Code                                    
#define 	EMVTid_PDOL             74              // Tag "9F38" - PDOL                                              
#define 	EMVTid_ServiceCod       75              // Tag "5F30" - Service Ocde                                      
#define 	EMVTid_SignDynAppData   76              // Tag "9F4B" - Signed Dynamic Application Data                   
#define 	EMVTid_SignStaAppData   77              // Tag "9300" - Signed Static Application Data                    
#define 	EMVTid_StaDataAuthTList 78              // Tag "9F4A" - Static Data Authentication Tag List               
#define 	EMVTid_TACDefault       79              // Tag "D800" - Terminal Action Code Default                      
                                                                                                                
#define 	EMVTid_TACDenial        80         		// Tag "D900" - Terminal Action Code Denial                       
#define 	EMVTid_TACOnline        81              // Tag "DA00" - Terminal Action Code Online                       
#define 	EMVTid_TerminalCapa     82              // Tag "9F33" - Terminal Capability                               
#define 	EMVTid_TCountryCod      83              // Tag "9F1A" - Terminal Countery Code                            
#define 	EMVTid_TFloorLimit      84              // Tag "9F1B" - Terminal Floor Limit                              
#define 	EMVTid_TerminalId       85              // Tag "9F1C" - Terminal Identifier                               
#define 	EMVTid_TRiskMgmtData    86              // Tag "9F1D" - Terminal Risk Management Data                     
#define 	EMVTid_TerminalType     87              // Tag "9F35" - Terminal Type                                     
#define 	EMVTid_TVR              88              // Tag "9500" - Terminal Verification Result                      
#define 	EMVTid_Track1DisData    89              // Tag "9F1F" - Track1 Discretionary Data                         
                                                                                                                
#define 	EMVTid_Track2DisData    90         		// Tag "9F20" - Track2 Discretionary Data                         
#define 	EMVTid_Track2EquData    91              // Tag "5700" - Track2 Equivalent Data                            
#define 	EMVTid_TDOL             92              // Tag "9700" - TDOL                                              
#define 	EMVTid_TCHashVal        93              // Tag "9800" - Transaction Certificate Hash Value                
#define 	EMVTid_TrCurCod         94              // Tag "5F2A" - Transaction Currency Code                         
#define 	EMVTid_TrCurExp         95              // Tag "5F36" - Transaction Currency Exponent                     
#define 	EMVTid_TrDate           96              // Tag "9A00" - Transaction Date                                  
#define 	EMVTid_TrRefCurCod      97              // Tag "9F3C" - Transaction Reference Currency Code               
#define 	EMVTid_TrRefCurCnv      98              // Tag "DB00" - Transaction Reference Currency Conversion         
#define 	EMVTid_TrRefCurExp      99              // Tag "9F3D" - Transaction Reference Currency Exponent           
                                                                                                                
#define 	EMVTid_TrSeqCnt         100        		// Tag "9F41" - Transaction Sequence Counter                      
#define 	EMVTid_TSI              101             // Tag "9B00" - Transaction Status Information                    
#define 	EMVTid_TrTime           102             // Tag "9F21" - Transaction Time                                  
#define 	EMVTid_TrType           103             // Tag "9C00" - Transaction Type                                  
#define 	EMVTid_UnpredictNo      104             // Tag "9F37" - Unpredictable Number                              
#define 	EMVTid_UpOffLimit       105             // Tag "9F23" - Upper Offline Limit                               
#define 	EMVTid_InstallmentNo    106             // Tag "D100" - Installment Number                                
#define 	EMVTid_ApprovalNo       107             // Tag "D400" - Approval Number                                   
#define 	EMVTid_IsuPubKey       	108             // Tag "DF06" - Issuer Public Key                                 
#define 	EMVTid_StaAppData       109             // Tag "DF07" - Static Application Data                           
                                                                                                                
#define		EMVTid_TaxAmount		110             // Tag "D200" - Tax Amount                                        
#define		EMVTid_ServiceFee		111             // Tag "D300" - Service Fee                                       
#define		EMVTid_OriTrDate      	112             // Tag "D500" - Original Transaction Date                         
#define		EMVTid_MerchantNo		113             // Tag "DC00" - Merchant No                                       
#define 	EMVTid_OriApprovalNo    114             // Tag "DE00" - Original Approval No                              
#define 	EMVTid_AcquirerName		115             // Tag "DF08" - Acquirer Name                                     
#define 	EMVTid_HostMessage		116             // Tag "DF09" - Host Message                                      
#define 	EMVTid_TerminalCnt		117             // Tag "DF0A" - Terminal Counter                                  
#define 	EMVTid_IccPubKey		118        		// Tag "DF0B" - Recovered Icc Public Key                          
#define 	EMVTid_DynAppData		119        		// Tag "DF0C" - Dynamic Application Data                          
                                                                                                                
#define 	EMVTid_TerminalAID		120		   		// Tag "9F06" - Application Identifier (AID) - terminal           
#define 	EMVTid_TermCAPubKeyIdx  121		   		// Tag "9F22" - Certification Authority Public Key Index          
#define		EMVTid_TargetPercent	122	       		// Tag "DF0D" - Target Percentage                                 
#define		EMVTid_ThresholdValue	123        		// Tag "DF0E" - Threshold Value                                   
#define		EMVTid_MaxTargetPercent	124        		// Tag "DF0F" - Maximum Target Percentage                         
#define 	EMVTid_TCC     			125	       		// Tag "9F53" - Transaction Category Code                         
#define 	EMVTid_HostNotice		126        		// Tag "DF1D" - Host Notice                                       
#define 	EMVTid_DispControl		127        		// Tag "DF1E" - Display Control                                   
#define 	EMVTid_LOGEntry			128        		// Tag "9F4D" - Log Entry    
#define		EMVTid_AccountType		129				// Tag "5F57" - Account Type    
#define		EMVTid_ASIndicator		130				// Tag "DF10" - Application Selection Indicator                                 
                                                                  
#define 	EMVTid_LAST_TAG         255

//----------------------------------------------------------------------------        	
//------------------------ Description about Structure -----------------------//
//----------------------------------------------------------------------------        	
typedef struct
{
	unsigned char	rid[5];
	unsigned char	index;
	unsigned char 	mlen;
	unsigned char	mod[MAX_CAKEY];		//	MAX_CAKEY = 248
	unsigned char 	elen;
	unsigned char	exp[3];
	unsigned char	chkLen;
	unsigned char	chk[HASH_SIZE]; 	//	HASH_SIZE = 20
} EMVst_CA_PUBKEY;						//	size : 280

typedef  struct
{
    unsigned char    aidlen;
    unsigned char    aid[MAX_LABEL_AID_LEGNTH];
    unsigned char    datalen;
    unsigned char    data[120]; 
}EMVst_TERMDE;					//	size : 138

typedef struct {
	int		aidLen;
	unsigned char	aid[MAX_LABEL_AID_LEGNTH];
	unsigned char	DspIdx;
}EMVst_AIDLIST_V5;

typedef  struct {
	int		aidCNT;
	EMVst_AIDLIST_V5	aidI[ICEMV_APP_CNT];
}EMVst_LIST_OF_AID_V5;

typedef  struct
{
    unsigned char	AppLabelLen;
    unsigned char	AppLabel[MAX_LABEL_AID_LEGNTH*2];
    unsigned char	AidLen;
    unsigned char	AidName[MAX_LABEL_AID_LEGNTH];
    unsigned char	priority;
    unsigned char	IsrCntryCodeLen;
    unsigned char	IsrCntryCode[3];
    unsigned char	ApSelCodeLen;
    unsigned char	ApSelCode[32];
    unsigned char	PreferredLen;
    unsigned char	PreferredName[MAX_LABEL_AID_LEGNTH*2];
	unsigned char   IsrCodeTableIndex;
	unsigned char   Reserved[62];
} ICC_APP_DESC_V5;

typedef  struct
{
    int				CardHolderConfirm;
    int				AppCnt;
    ICC_APP_DESC_V5	IccAppDesc[ICEMV_APP_CNT];
} EMV_ICC_APPS_INFORM_V5;

typedef  struct
{
    unsigned char	AidLen;
    unsigned char	AidName[MAX_LABEL_AID_LEGNTH];
    unsigned char	DiscretionaryDataLen;
    unsigned char	DiscretionaryData[256];        
} EMVst_APPS_TAGS;

typedef  struct
{
    int			AppCnt;
    EMVst_APPS_TAGS	DescTemplate[ICEMV_APP_CNT];
} EMVst_ICC_DESCDATA_INFORM;

typedef  struct
{
    int				CAPDULen;
    unsigned char   CAPDU[256];
    int				RAPDULen;
    unsigned char   RAPDU[256];   
    unsigned char	SW12[4];
} ISAPDU_INFORM;

typedef  struct
{
    int				APDUCnt;
    ISAPDU_INFORM	IsApdu[20];
} EMVst_LIST_OF_ISAPDU;

typedef struct {
	unsigned int	AidLen;
	unsigned char	Aid[MAX_LABEL_AID_LEGNTH];
	unsigned int	SecndAidCnt;	
	unsigned int	SecndAIDInx[ICEMV_APP_CNT];	
}EMVst_PRIM_AIDLIST;

typedef  struct {
	int					PrimAidCnt;
	EMVst_PRIM_AIDLIST	PrimAidI[ICEMV_APP_CNT];
}EMVst_LIST_OF_NDCAID;

//------------------------ Description about Extern Functions -----------------------//
#define DLL_IMPORT		__declspec(dllimport)
//-------------------------------------------------------------------
// EMV Application Selection
DLL_IMPORT void emvkrnl_Sel_SetOnlyOneAID(unsigned char ucMatchUpYN);
DLL_IMPORT int  emvkrnl_Sel_ApplicationSelection(unsigned char SupportPSE, EMVst_LIST_OF_AID_V5 *AIDList, int *CandidateCnt); //nh request
DLL_IMPORT int  emvkrnl_Sel_MakeDisplayAppList(unsigned char SupportUserConfirm, 
								   unsigned short CandidateCnt, 
								   EMV_ICC_APPS_INFORM_V5 *IccAppInform); //nh request
DLL_IMPORT void emvkrnl_Sel_RemoveCandidateList(unsigned short CandidateCnt, unsigned short AidLen, unsigned char *AIDVal);
DLL_IMPORT int  emvkrnl_Sel_FinalAppSelection(unsigned short AidLen, unsigned char *AIDVal, unsigned char *SW);
DLL_IMPORT void emvkrnl_Sel_DescretionaryDataList(EMVst_ICC_DESCDATA_INFORM *EmvstIccDescInform);
DLL_IMPORT int  emvkrnl_Sel_NDCAIDSelection(EMVst_LIST_OF_NDCAID *NDCAIDList, int *CandidateCnt);
DLL_IMPORT void emvkrnl_Sel_ASI(unsigned char SupportASI);

// EMV Transaction
DLL_IMPORT int emvkrnl_Trans_InitApplication(void);
DLL_IMPORT int emvkrnl_Trans_ReadAppData(void);
DLL_IMPORT int emvkrnl_Trans_OfflineDataAuth(EMVst_CA_PUBKEY *stCAPKey);
DLL_IMPORT int emvkrnl_Trans_ProcessRestrict(void);
DLL_IMPORT int emvkrnl_Trans_CardholderVerify(unsigned short OnlinePinLen, unsigned char *EncipheredOnlinePIN);
DLL_IMPORT int emvkrnl_Trans_TerminalRiskMgmt(unsigned char SupportForceOnline);
DLL_IMPORT unsigned char emvkrnl_Trans_TerminalActionAnalysis(void);
DLL_IMPORT int emvkrnl_Trans_CardActionAnalysis(unsigned char *OnlineFlag);
DLL_IMPORT int emvkrnl_Trans_OnlineProcess(int OnlineMode, unsigned char Acquirer_CID);
DLL_IMPORT int emvkrnl_Trans_Completion(void);
DLL_IMPORT void emvkrnl_Get_ISAPDUList(EMVst_LIST_OF_ISAPDU *IsApduList);
//-------------------------------------------------------------------
// for Tlv Function
DLL_IMPORT void emvkrnl_Tlv_GetTidTag(int tid, unsigned char *tag);
DLL_IMPORT int  emvkrnl_Tlv_GetTid(unsigned char *tag, int len);
DLL_IMPORT int	emvkrnl_Tlv_GetTag(unsigned char *buf, unsigned char *tag);
DLL_IMPORT int	emvkrnl_Tlv_GetVal(int tid, int *len, unsigned char *val);
DLL_IMPORT int	emvkrnl_Tlv_GetTidLen(int tid);
DLL_IMPORT int	emvkrnl_Tlv_GetLen(unsigned char *buf, int *vlen);
DLL_IMPORT int	emvkrnl_Tlv_StoreVal(int tid, int len, unsigned char *val);
DLL_IMPORT int	emvkrnl_Tlv_StoreValFromRecord(unsigned char *record, int type, int inlen, int source);

//-------------------------------------------------------------------
// EMV Utility
DLL_IMPORT void emvkrnl_Util_BINtoN(int val, unsigned char *data, int len);
DLL_IMPORT void emvkrnl_Util_toYYYYMMDD(unsigned char *data, unsigned char *out, int format);
DLL_IMPORT unsigned char emvkrnl_Util_GetCNRealLen(unsigned char *data, int len);
DLL_IMPORT void emvkrnl_Util_RightJustify(unsigned char *data, int len, unsigned char *dest);
DLL_IMPORT void emvkrnl_Util_Decompress(unsigned char *bySource, unsigned char *byDestination, int byCount);
DLL_IMPORT void emvkrnl_Util_Compress(unsigned char *bySource, unsigned char *byDestination, int byCount);

//-------------------------------------------------------------------
// Other Functions
DLL_IMPORT int emvkrnl_RegistIFMCommFunc(int (CALLBACK* SMCSENDAPDU)(BYTE dCode, BYTE *apdu, UINT apduLen, BYTE *resp,int *rlen));
DLL_IMPORT void emvkrnl_Util_GetAPDUSW(unsigned char *SW12);
DLL_IMPORT void emvkrnl_Util_GetVersion(unsigned char *KernelVer);
DLL_IMPORT void emvkrnl_Util_SetKernelLogPath(unsigned char *PathName, unsigned int PathLen);
DLL_IMPORT void emvkrnl_Trans_Initial(void);
