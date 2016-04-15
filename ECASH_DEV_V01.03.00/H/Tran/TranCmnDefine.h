/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCmnDefine_H_)
#define _TranCmnDefine_H_

#if !defined(KB_SOFO)
#define KB_SOFO
#endif

/* **************************************************************************
//
// Nautilus Hyosung Inc. 2005 All Rights Reserved
//
// 파일명 : KtxCardIntf.h
// 개  요 : Dll 헤더파일
// 이  력 : 2005/08/29 신규작성 (gijin)
//  
// *************************************************************************/
#ifndef XDllExport
	#ifdef __cplusplus
		#define XDllExport   extern "C" __declspec( dllexport )
	#else 
		#define XDllExport __declspec( dllexport )
	#endif
#endif


// CHB (0411) Encrypt  (bank encrypt)
/*******************************************************************************************************************/
typedef struct MsgData
{
	char cs_acctno[24];   /*  계좌번호 23자리  */
	char cs_txamt[13];    /*  거래금액 12자리  */
	char cs_serialno[4];  /*  카드회차  3자리  */
	char cs_custno[9];    /*  비밀번호 및 유효기간 8자리  */
}MSG_DATA, *PMSG_DATA;

XDllExport void   WINAPI WooriSeed( char *key, int index, char *pin, char *epin );
XDllExport LPCSTR chb_convert(MSG_DATA m_pMsgData);
XDllExport void	  HouseSeed(char *mk,int index,char *sour,char *dest,int opt);
XDllExport void   HanaSeed(char *key,int option, int length, char *sour, char *dest,int check);
XDllExport int    PEMencrypt( char *pin, char *xprday, char *acno, char *epin ); 
XDllExport int __stdcall ax_encrypt(char* ibuf, int l_ibuf, char* keyhdr, int l_keyhdr, char* obuf);


/******************************************************************************************************************/

#define		LINE_CLEAR				0
#define		LINE_OPEN				'O'
#define		LINE_CLOSE				'C'

#define		UDP_SYNC				0
#define		UDP_ASYNC				1

#define		TCP_TSEC				3

#define		_BK_E_INI				1
	
#define		_KEYTABELE_INI			0	
#define		_ORGENCRYPT_INI			1	
#define		_CARDBINNO_NO			2
#define		_ATM_CFG_INI_T 			3
#define		_ATM_MOBILE_INI_T 		4

#define		ATM_SEND_ASYSC			2
#define		SEND_ENCRYPT			1
#define		RECV_DECRYPT			2

#define		LOGTRACE				10
#define		JOURNALTRACE			20 

#define		RET_OK					0 
#define RET_EMV                                            473                 // EMV
#define RET_EMVREADERROR                                   474                 // EMVREAD실패

#define		_JOURNAL_ROOT				_T("D:\\T1ATM")
#define		_DIR_JOURNAL				_T("D:\\T1ATM\\Journal")
#define		_DIR_EJOURNAL				_T("D:\\T1ATM\\EJournal")
#define		_DIR_MOBILEGAME				_T("D:\\MobileGame")
#define		_B_E_INFO_INI				_T("D:\\INI\\B_E_INFO.INI")
#define		_ATMKEYTABELE_INI			_T("D:\\INI\\KEYTABLE.INI")
#define		_ATMTRCODEENCRYPT_INI		_T("D:\\INI\\TRCODEENCRYPT.INI")
#define		_ECASH_SVR_RES_INI			_T("D:\\INI\\ECASH_SVR_RESPONSE.INI")
#define		_HOST_STATUS_INI			_T("C:\\T1ATM\\INI\\HOST_STATUS.INI")

#define		KB_104_CERT					_T("C:\\T1ATM\\App\\Main\\qbank_cert.scb")
#define		KUB_103_CERT				_T("C:\\T1ATM\\App\\Main\\KUB103CERT.der")
#define		NON_111_CERT				_T("C:\\T1ATM\\App\\Main\\nh_cli.key")
#define		SC_123_CERT				    _T("C:\\T1ATM\\App\\Main\\SC_CLI.KEY")
#define		LOCALDIREXE					_T("C:\\T1ATM\\App\\Main\\ECash_Simulator.exe")
#define		HANA_ENCRIPT_DIR			_T("C:\\T1ATM\\App\\Main\\key.dat")
#define		NON_ENCRIPT_DIR				_T("C:\\T1ATM\\App\\Main\\ctx.bin")
#define		SC_ENCRIPT_DIR				_T("C:\\T1ATM\\App\\Main\\SC.bin")

#define		_ENCRYPT_ERROR_MSG1			_T("ATM_(인증서/암호화) 유효기간 EXFIRE!!| 기관에 연락하여 주십시오 ???")
#define		_ENCRYPT_ERROR_MSG2			_T("ATM_(인증서/암호화) 유효기간 EXFIRE!!| 기관에 연락하여 주십시오 ???")
#define		_COMM_ERROR_MSG3			_T("전문불일치(88501)|서버와통신내역을 확인 하십시오 ???")


// EN/DISABLE 
#define		ENABLE			1
#define		DISABLE			2

#define		PLAINTEXT		1
#define		CIPHERTEXT		2
#define		CIPHERTEXT_TMP	3

#define		SEND_F			0			//	C_SUCCESS
#define		RECV_F			1			//	C_SUCCESS

// KEY SUCCESS (SOFO, INITECH)
#define		C_SUCCESS		0			//	C_SUCCESS

#define		K_CERTIFY_C		1			//	C_SUCCESS
#define		K_VERIFY_C		2			//	C_SUCCESS
#define		C_F				0			//	CHECK FLAG
#define		D_E				1			//	DATA_ENCRYPT
#define		D_D				2			//	DATA_DECRYPT
#define		C_R				1			// 인증서등록날자
#define		C_V				2			// 인증서유효일자
#define		K_R				3			// 세션키등록날자
#define		K_V				4			// 세션키유효일자	
#define		K_S_F			5			//	KEY_SETUP_FLAG
#define		K_N_C			4			//	KEY_NON_CHECK
#define		K_C_S			10			//	KEY_CERTIFY_SUCCESS
#define		K_C_F			11			//	KEY_CERTIFY_FAIL
#define		K_V_S			20			//	KEY_VERIFY_SUCCESS
#define		K_V_F			21			//	KEY_VERIFY_FAIL

#define		TX_SEND_F		1
#define		RX_RECV_F		2

#define		PACK_F	    	1
#define		UNPACK_F		2

#define		NH_ATM_TYPE			_T("S0") //ECASH 기종 구분 코드


#define		ATM_CFG_SEC			_T("ATMINFO")
#define		ATM_MOBILE_SEC		_T("MOBILE")
#define		KBS_ORG_SEC			_T("006") //국민
#define		KUB_ORG_SEC			_T("003") //기업
#define		NON_ORG_SEC			_T("011") //농협
#define		SCO_ORG_SEC			_T("023") //제일
#define		WRT_ORG_SEC			_T("247") //NH투자증권
#define		SST_ORG_SEC			_T("240") //삼성증권
#define		HWT_ORG_SEC			_T("269") //한화증권
#define		KJB_ORG_SEC			_T("034") //광주은행
#define		KEB_ORG_SEC			_T("005") //외환은행
#define		WRB_ORG_SEC			_T("020") //우리은행
#define		DBS_ORG_SEC			_T("279") //동부


// ATM ORG CODE encrypt 
#define		x00_ORG_INT_000		0  
#define		KBS_ORG_INT_004		4  
#define		KBS_ORG_INT_006		6  
#define		KBS_ORG_INT_029		29 
#define		KBS_ORG_INT_030		30 
#define		KUB_ORG_INT_003		3 
#define		NON_ORG_INT_011		11 
#define		NON_ORG_INT_012		12 
#define		NON_ORG_INT_016		16 
#define		NON_ORG_INT_017		17 
#define		SCO_ORG_INT_023		23 
#define		WRT_ORG_INT_247		247
#define		SST_ORG_INT_240		240
#define		SST_ORG_INT_CVT_240	364
#define		HWT_ORG_INT_269		269
#define		WRT_ORG_INT_278		278
#define		KJB_ORG_INT_034		34   
#define 	KEB_ORG_INT_005		5	//외환은행
#define 	WRB_ORG_INT_020		20	//우리
#define		DOB_ORG_INT_279		279	  

// ATM SEND ENCRYT METHOD
#define		TRAN_ENC_000_00    0			//기타은행 
#define 	TRAN_ENC_006_01	   1			//국민은행	(30,29,04,06,09)
#define		TRAN_ENC_034_02    2			//광주 (34)  
#define 	TRAN_ENC_026_03	   3			//조흥은행,신한 (21,26)     
#define 	TRAN_ENC_025_04	   4			//하나은행  (25,81,82,33)   			
#define 	TRAN_ENC_032_05	   5			//부산은행  (32)    
#define 	TRAN_ENC_003_06	   6			//기업은행  (03)	
#define 	TRAN_ENC_011_07	   7			//농협      (17,16,12,11)
#define 	TRAN_ENC_023_08	   8			//제일      (23)
#define 	TRAN_ENC_247_09	   9			//NH투자증권 (207)
#define 	TRAN_ENC_240_10	   10			//삼성증권 (240)
#define 	TRAN_ENC_269_11	   11			//한화투자증권 (269)
#define 	TRAN_ENC_218_12	   12			//현대증권 (218)
#define 	TRAN_ENC_270_13	   13			//하나대투증권 (270)
#define 	TRAN_ENC_071_14	   14			//우체국 (071)
#define 	TRAN_ENC_238_15	   15			//대우증권(238)
#define 	TRAN_ENC_267_16	   16			//대신증권(267)
#define 	TRAN_ENC_278_17	   17			//굿모닝신한증권
#define 	TRAN_ENC_027_18	   18			//씨티브랜드

#define nEnCmd000_0			0		//"????????????"      
#define nEnCmd001_1			1		//"한국은행    "      
#define nEnCmd002_2			2		//"산업은행    "      
#define nEnCmd003_3			3		//"기업은행    "      
#define nEnCmd005_5			5		//"외환은행    "      
#define nEnCmd004_6			6		//"국민은행    "      
#define nEnCmd006_6			6		//"국민은행    "      
#define nEnCmd009_6			6		//"국민은행    "      
#define nEnCmd029_6			6		//"국민은행    "      
#define nEnCmd030_6			6		//"국민은행    "      
#define nEnCmd007_7			7		//"수협중앙회  "      
#define nEnCmd008_8			8		//"수출입은행  "      
#define nEnCmd011_11		11		//"농협중앙회  "    
#define nEnCmd016_11		11		//"농협중앙회  "    
#define nEnCmd012_12		12		//"단위농협    "    
#define nEnCmd013_12		12		//"단위농협    "    
#define nEnCmd014_12		12		//"단위농협    "    
#define nEnCmd015_12		12		//"단위농협    "    
#define nEnCmd017_12		12		//"단위농협    "    
#define nEnCmd020_20		20		//"우리은행    "    
#define nEnCmd022_20		20		//"우리은행    "    
#define nEnCmd024_20		20		//"우리은행    "    
#define nEnCmd083_20		20		//"우리은행    "    
#define nEnCmd023_23		23		//"제일은행    "    
#define nEnCmd027_27		27		//"씨티은행    "    
#define nEnCmd053_53		53		//"씨티은행    "    
#define nEnCmd031_31		31		//"대구은행    "    
#define nEnCmd032_32		32		//"부산은행    "    
#define nEnCmd034_34		34		//"광주은행    "    
#define nEnCmd035_35		35		//"제주은행    "    
#define nEnCmd037_37		37		//"전북은행    "    
#define nEnCmd039_39		39		//"경남은행    "    
#define nEnCmd045_45		45		//"새마을금고  "    
#define nEnCmd048_48		48		//"신협중앙회  "    
#define nEnCmd050_50		50		//"상호저축은행"    
#define nEnCmd054_54		54		//"HSBC은행    "    
#define nEnCmd055_55		55		//"도이치은행  "    
#define nEnCmd056_56		56		//"ABN은행     "    
#define nEnCmd071_71		71		//"우체국      "    
#define nEnCmd072_71		71		//"우체국      "    
#define nEnCmd073_71		71		//"우체국      "    
#define nEnCmd074_71		71		//"우체국      "    
#define nEnCmd075_71		71		//"우체국      "    
#define nEnCmd025_81		81		//"하나은행    "    
#define nEnCmd033_81		81		//"하나은행    "    
#define nEnCmd081_81		81		//"하나은행    "    
#define nEnCmd082_81		81		//"하나은행    "    
#define nEnCmd021_88		88		//"신한은행    "    
#define nEnCmd026_88		88		//"신한은행    "    
#define nEnCmd028_88		88		//"신한은행    "    
#define nEnCmd038_88		88		//"신한은행    "    
#define nEnCmd040_88		88		//"신한은행    "    
#define nEnCmd088_88		88		//"신한은행    "    
#define nEnCmd105_105 		105		//"KEB하나카드 " 
#define nEnCmd105_115 		115		//"KEB하나카드 " 
#define nEnCmd105_115 		115		//"외환카드    "    
#define nEnCmd106_106 		106		//"국민카드    "    
#define nEnCmd107_107 		107		//"수협카드    "    
#define nEnCmd111_111 		111		//"농협비자    "    
#define nEnCmd120_120 		120		//"우리카드    "    
#define nEnCmd121_121 		121		//"조흥카드    "    
#define nEnCmd126_192 		192		//"신한카드    "    
#define nEnCmd127_127 		127		//"씨티카드    "    
#define nEnCmd134_134 		134		//"광주카드    "    
#define nEnCmd135_135 		135		//"제주비자    "    
#define nEnCmd137_137 		137		//"전북카드    "    
#define nEnCmd181_105 		105		//"KEB하나카드 "    
#define nEnCmd184_184 		184		//"삼성카드    "    
#define nEnCmd192_192 		192		//"신한카드    "    
#define nEnCmd193_193 		193		//"현대카드    "    
#define nEnCmd194_194 		194		//"BC카드      "    
#define nEnCmd195_195 		195		//"롯데카드    "    
#define nEnCmd209_369 				369		//"유안타증권  "    
#define nEnCmd209_379_PUBLIC 		379		//"유안타증권  "    
#define nEnCmd218_218_PUBLIC 		218		//"현대증권    "
#define nEnCmd218_218 		218		//"현대증권    "    
#define nEnCmd230_230 		230		//"미래에증권  "    
#define nEnCmd238_368 		368		//"대우증권    "    
#define nEnCmd240_364 		364		//"삼성증권    "    
#define nEnCmd243_243 		243		//"한국투자    "    
#define nEnCmd247_247 		247		//"우리투자    "    
#define nEnCmd261_261 		261		//"교보증권    "    
#define nEnCmd262_262 		262		//"하이투자    "    
#define nEnCmd263_263 		263		//"HMC투자     "    
#define nEnCmd264_264 		264		//"키움증권    "    
#define nEnCmd265_265 		265		//"이트레이드  "    
#define nEnCmd266_266 		266		//"에스케이    "    
#define nEnCmd267_267 		267		//"대신증권    "    
#define nEnCmd268_268 		268		//"솔로몬투자  "    
#define nEnCmd269_269 		269		//"한화증권    "    
#define nEnCmd270_270 		270		//"하나대투    "    
#define nEnCmd278_278 		278		//"신한금융    "    
#define nEnCmd279_279 		279		//"동부증권    "    
#define nEnCmd280_280 		280		//"유진투자    "    
#define nEnCmd287_287 		287		//"메리츠증권  "    
#define nEnCmd289_289 		289		//"NH증권      "    
#define nEnCmd290_290 		290		//"부국증권    "    
#define nEnCmd291_291 		291		//"신영증권    "    
#define nEnCmd292_292 		292		//"LIG증권	   "    
#define nEnCmd294_294_BC	294		//"해외 BC	   "    
#define nEnCmd384_364 		364		//"삼성증권    "    
#define nEnCmd484_484 		484		//"CJ시스템    "    
#define nEnCmd601_601 		601		//"발렉스      "    
#define nEnCmd701_701 		701		//"KTF         "    
#define nEnCmd702_702 		702		//"LGT         "    
#define nEnCmd703_703 		703		//"SKT         "    



#define		ASC2INT(x,y)	(Strlen(x) == y ? Asc2Int(x,Strlen(x)) : 0)

#define 	TRAN_MSG_EXCEPTION_700	   700	//공동망 수수료   - 700 에러처리
#define 	TRAN_MSG_EXCEPTION_707	   707	//이자율조회 불가 - 707 에러처리 
#define 	TRAN_MSG_EXCEPTION_2190	   2190	//인증전문 에러처리
#define 	TRAN_MSG_EXCEPTION_A010	   10	//인증전문 에러처리
#define 	TRAN_MSG_EXCEPTION_A020	   20	//인증전문 에러처리
#define 	TRAN_MSG_EXCEPTION_845	   845	
#define 	TRAN_MSG_EXCEPTION_798	   798	
#define 	TRAN_MSG_EXCEPTION_094	   94	


#define TRAN_ORG_INIT_002_N		_T("산업은행")	  
#define TRAN_ORG_INIT_003_N		_T("기업은행")	  
#define TRAN_ORG_INIT_004_N		_T("국민은행")	  
#define TRAN_ORG_INIT_005_N		_T("외환은행")	  
#define TRAN_ORG_INIT_006_N		_T("국민은행")	  
#define TRAN_ORG_INIT_007_N		_T("수협은행")	  
#define TRAN_ORG_INIT_009_N		_T("국민은행")	  
#define TRAN_ORG_INIT_011_N		_T("농협(중앙)")  
#define TRAN_ORG_INIT_012_N		_T("농협(단위)")  
#define TRAN_ORG_INIT_016_N		_T("농협(중앙)")  
#define TRAN_ORG_INIT_017_N		_T("농협(단위)")  
#define TRAN_ORG_INIT_020_N		_T("우리은행")	  
#define TRAN_ORG_INIT_021_N		_T("조흥은행")	  
#define TRAN_ORG_INIT_022_N		_T("우리은행")	  
#define TRAN_ORG_INIT_023_N		_T("SC은행")	  
#define TRAN_ORG_INIT_024_N		_T("우리은행")	  
#define TRAN_ORG_INIT_025_N		_T("하나은행")	  
#define TRAN_ORG_INIT_026_N		_T("신한은행")	  
#define TRAN_ORG_INIT_027_N		_T("씨티은행")	  
#define TRAN_ORG_INIT_029_N		_T("국민은행")	  
#define TRAN_ORG_INIT_030_N		_T("국민은행")	  
#define TRAN_ORG_INIT_031_N		_T("대구은행")	  
#define TRAN_ORG_INIT_032_N		_T("부산은행")	  
#define TRAN_ORG_INIT_033_N		_T("하나은행")	  
#define TRAN_ORG_INIT_034_N		_T("광주은행")	  
#define TRAN_ORG_INIT_035_N		_T("제주은행")	  
#define TRAN_ORG_INIT_036_N		_T("씨티은행")	  
#define TRAN_ORG_INIT_037_N		_T("전북은행")	  
#define TRAN_ORG_INIT_039_N		_T("경남은행")	  
#define TRAN_ORG_INIT_045_N		_T("새마을금고")  
#define TRAN_ORG_INIT_048_N		_T("신협")	  
#define TRAN_ORG_INIT_050_N		_T("상호저축은행")
#define TRAN_ORG_INIT_053_N		_T("씨티은행")	  
#define TRAN_ORG_INIT_054_N		_T("H.S.B.C")     
#define TRAN_ORG_INIT_071_N		_T("우체국")      
#define TRAN_ORG_INIT_072_N		_T("우체국")      
#define TRAN_ORG_INIT_081_N		_T("하나은행")	  
#define TRAN_ORG_INIT_082_N		_T("하나은행")	  
#define TRAN_ORG_INIT_083_N		_T("우리은행")	  
#define TRAN_ORG_INIT_401_N		_T("국민카드")	  
#define TRAN_ORG_INIT_402_N		_T("현대카드")	  
#define TRAN_ORG_INIT_403_N		_T("MASTER  ")    
#define TRAN_ORG_INIT_404_N		_T("비씨카드")	  
#define TRAN_ORG_INIT_405_N		_T("삼성카드")	  
#define TRAN_ORG_INIT_406_N		_T("롯데카드")	  
#define TRAN_ORG_INIT_407_N		_T("엘지카드")	  
#define TRAN_ORG_INIT_408_N		_T("외환카드")	  
#define TRAN_ORG_INIT_409_N		_T("VISA   ")     
#define TRAN_ORG_INIT_410_N		_T("JCB   ")      
#define TRAN_ORG_INIT_411_N		_T("CUP	   ")     
#define TRAN_ORG_INIT_412_N		_T("AMEX   ")     
#define TRAN_ORG_INIT_421_N		_T("광주카드")	  	  
#define TRAN_ORG_INIT_430_N		_T("조흥카드")	  
#define TRAN_ORG_INIT_432_N		_T("하나카드")	  
#define TRAN_ORG_INIT_433_N		_T("씨티카드")	  
#define TRAN_ORG_INIT_436_N		_T("씨티카드")	  
#define TRAN_ORG_INIT_437_N		_T("우리카드")	 

#define TRAN_ORG_SHINCARD	_T("신한카드")
#define TRAN_ORG_KJBCARD	_T("광주비자")				//#N0181

#define	SAMSCARDLOAN_RAINBOW	1						// 삼성카드 무지개카드론
#define	SAMSCARDLOAN_HANARO		2						// 삼성카드 하나로카드론
#define	HYUNCARDLOAN_CDLOAN		3						// 현대카드 CD론
#define	LOTTECARDLOAN			4						// 롯데카드론
#define	SHINHANCARDLOAN			5						// 신한카드론



#define SAMSUNG_LIFE  _T("403") //삼성생명
#define SAMSUNG_FIRE  _T("402") //삼성화재
#define DONGBU_FIRE   _T("401") //동부화재
#define SAMSUNG_LOAN  _T("205") //삼성카드론
#define HYUNDAI_LOAN  _T("202") //현대카드론
      
                 
#define MASTER_ORG    _T("203") //MASTER
#define VISA_ORG      _T("209") //VISA
#define JCB_ORG       _T("210") //JCB
#define CUP_ORG       _T("211") //CUP
#define AMEX_ORG      _T("212") //AMEX
#define DINERS_ORG    _T("213") //DINERS
#define DISCOVER_ORG  _T("214") //DISCOVER

#define OK_CASHBAG_ORG _T("501") //OKCASH
#define GSNPOINT_ORG   _T("507")
#define GIRO_TAX_APT  _T("601")
#define TRUST_ORG     _T("502")
#define KMERCE_ORG    _T("503")
#define KOREAN_AIR    _T("701")
#define XCASH_ORG     _T("504")
#define TMONEY_ORG		_T("505")	//T-Money
#define CGV_ORG			_T("703")	//CGV
#define WIRETRANSFER_ORG _T("105")	//해외송금
#define DOTONG_ORG     _T("506")
#define DAMON_ORG      _T("704")
#define PHOTO_ORG      _T("705")
#define CGV_ORG_PRIMUS _T("706")	//CGV
#define CJ_ORG_PIFF	   _T("707")	//CGV
#define FC_ORG_GAME    _T("801")	//CGV
#define EVERLAND_ORG_TICKET _T("802")	//CGV
#define FM_SPORTS_TICKET _T("803")	//CGV
#define GSTICKET_ORG    _T("801")	//CGV
#define FMTICKET_ORG    _T("802")	//CGV
#define GAME_MONEY_ORG	_T("504")

#define ORG_LOAN_SAMSCARD  _T("205") //삼성카드론
#define ORG_LOAN_HYUNCARD  _T("202") //현대카드론
#define ORG_LOAN_LOTTCARD  _T("206") //롯데카드론
#define ORG_LOAN_SHINCARD  _T("228") //신한카드론




// 은행별 코드정의 
// 기관별 코드정의
#define		BANK_IB			_T("0159")
#define     KDBBANKID		_T("002")	//산업은행	                
#define 	KIUBANKID		_T("003")	//기업은행
	                
#define 	KUMBANKID1		_T("004")	//국민은행	
#define 	KUMBANKID2		_T("006")	//국민은행	
#define 	KUMBANKID3		_T("009")	//국민은행	
#define 	KUMBANKID4		_T("029")	//국민은행	
#define 	KUMBANKID5		_T("030")	//국민은행	

#define 	KOEBANKID		_T("005")	//외환은행	                
#define 	SUHBANKID		_T("007")	//수협은행
	         
#define 	NONBANKID1		_T("011")	//농협      
#define 	NONBANKID2		_T("012")	//농협      
#define 	NONBANKID3		_T("016")	//농협      
#define 	NONBANKID4		_T("017")	//농협
 
#define 	WORBANKID1		_T("020")	//우리은행  
#define 	WORBANKID2		_T("022")	//우리은행  
#define 	WORBANKID3		_T("024")	//우리은행  
#define 	WORBANKID4		_T("083")	//우리은행  
  
#define 	CHOBANKID		_T("021")	//조흥은행                      
#define 	KOFBANKID		_T("023")	//제일은행
#define 	KOFBANKIDC		_T("0J2")	//제일은행

#define 	HANBANKID1		_T("025")	//하나은행  
#define 	HANBANKID2		_T("033")	//하나은행  
#define 	HANBANKID3		_T("081")	//하나은행  
#define 	HANBANKID4		_T("082")	//하나은행
#define 	HANBANKIDC		_T("080")	//하나은행 브랜드 ATM 청호이지캐쉬 내부기관 코드 #N0214 
 
#define 	SHIBANKID		_T("026")	//신한은행  
#define 	SHIBANKID1		_T("088")	//신한은행  

#define 	GOOBANKID1		_T("027")	//한미은행  
#define 	GOOBANKID2		_T("036")	//한미은행  

#define 	DEABANKID		_T("031")	//대구은행                      
#define 	PUSBANKID		_T("032")	//부산은행                      
#define 	KAWBANKID		_T("034")	//광주은행                      
#define 	JEJBANKID		_T("035")	//제주은행                      
#define 	JEOBANKID		_T("037")	//전북은행                      
#define 	KYOBANKID		_T("039")	//경남은행                      
#define 	KFCBANKID		_T("045")	//새마을금고                       
#define 	SINBANKID		_T("048")	//신협	                        
#define 	SANBANKID		_T("050")	//상호저축                      
#define 	CITBANKID		_T("053")	//씨티은행                      
#define 	HSBABNKID		_T("054")	//H.S.B.C

#define 	POSBANKID1		_T("071")	//우체국  
#define 	POSBANKID2		_T("072")	//우체국  
#define 	POSBANKID3		_T("073")	//우체국  
#define 	POSBANKID4		_T("074")	//우체국  
#define 	POSBANKID5		_T("075")	//우체국 

#define     IKDBBANKID		2	//산업은행	                
#define 	IKIUBANKID		3	//기업은행
	                
#define 	IKUMBANKID1		4	//국민은행	
#define 	IKUMBANKID2		6	//국민은행	
#define 	IKUMBANKID3		9	//국민은행	
#define 	IKUMBANKID4		29	//국민은행	
#define 	IKUMBANKID5		30	//국민은행	

#define 	IKOEBANKID		5	//외환은행	                
#define 	ISUHBANKID		7	//수협은행
	                
#define 	INONBANKID1		11	//농협      
#define 	INONBANKID2		12	//농협      
#define 	INONBANKID3		16	//농협      
#define 	INONBANKID4		17	//농협
          
#define 	IWORBANKID1		20	//우리은행  
#define 	IWORBANKID2		22	//우리은행  
#define 	IWORBANKID3		24	//우리은행  
#define 	IWORBANKID4		83	//우리은행  
      
#define 	ICHOBANKID		21	//조흥은행                      
#define 	IKOFBANKID		23	//제일은행
                      
#define 	IHANBANKID1		25	//하나은행  
#define 	IHANBANKID2		33	//하나은행  
#define 	IHANBANKID3		81	//하나은행  
#define 	IHANBANKID4		82	//하나은행
       
#define 	ISHIBANKID		26	//신한은행  
#define 	ISHIBANKID1		88	//신한은행  
                    
#define 	IGOOBANKID1		27	//한미은행  
#define 	IGOOBANKID2		36	//한미은행  
#define 	ICITBANKID		53	//씨티은행                      
            
#define 	IDEABANKID		31	//대구은행                      
#define 	IPUSBANKID		32	//부산은행                      
#define 	IKAWBANKID		34	//광주은행                      
#define 	IJEJBANKID		35	//제주은행                      
#define 	IJEOBANKID		37	//전북은행                      
#define 	IKYOBANKID		39	//경남은행                      
#define 	IKFCBANKID		45	//새마을금고                       
#define 	ISINBANKID		48	//신협	                        
#define 	ISANBANKID		50	//상호저축                      
#define 	IHSBABNKID		54	//H.S.B.C
	                
#define 	IPOSBANKID1		71	//우체국  
#define 	IPOSBANKID2		72	//우체국  
#define 	IPOSBANKID3		73	//우체국  
#define 	IPOSBANKID4		74	//우체국  
#define 	IPOSBANKID5		75	//우체국  

#define 	NHMUINPUT		100	//매출금입금  



#define     KDBINNERID		_T("002")	//산업은행	                
#define 	KIUINNERID		_T("003")	//기업은행
#define 	KUMINNERID1		_T("004")	//국민은행	
#define 	KUMINNERID2		_T("006")	//국민은행	
#define 	KUMINNERID3		_T("009")	//국민은행	
#define 	KUMINNERID4		_T("029")	//국민은행	
#define 	KUMINNERID5		_T("030")	//국민은행	
#define 	KOEINNERID		_T("005")	//외환은행	                
#define 	SUHINNERID		_T("007")	//수협은행
#define 	NONINNERID1		_T("011")	//농협      
#define 	NONINNERID2		_T("012")	//농협      
#define 	NONINNERID3		_T("016")	//농협      
#define 	NONINNERID4		_T("017")	//농협
#define 	WORINNERID1		_T("020")	//우리은행  
#define 	WORINNERID2		_T("022")	//우리은행  
#define 	WORINNERID3		_T("024")	//우리은행  
#define 	WORINNERID4		_T("083")	//우리은행  
#define 	CHOINNERID		_T("021")	//조흥은행                      
#define 	KOFINNERID		_T("023")	//제일은행
#define 	HANINNERID1		_T("025")	//하나은행  
#define 	HANINNERID2		_T("033")	//하나은행  
#define 	HANINNERID3		_T("081")	//하나은행  
#define 	HANINNERID4		_T("082")	//하나은행 
#define 	SHIINNERID		_T("026")	//신한은행  
#define 	SHIINNERID1		_T("088")	//신한은행  
#define 	GOOINNERID1		_T("027")	//한미은행  
#define 	GOOINNERID2		_T("036")	//한미은행  
#define 	DEAINNERID		_T("031")	//대구은행                      
#define 	PUSINNERID		_T("032")	//부산은행                      
#define 	KAWINNERID		_T("034")	//광주은행                      
#define 	JEJINNERID		_T("035")	//제주은행                      
#define 	JEOINNERID		_T("037")	//전북은행                      
#define 	KYOINNERID		_T("039")	//경남은행                      
#define 	KFCINNERID		_T("045")	//새마을금고                       
#define 	SININNERID		_T("048")	//신협	                        
#define 	SANINNERID		_T("050")	//상호저축                      
#define 	CITINNERID		_T("053")	//씨티은행                      
#define 	HSBINNERID		_T("054")	//H.S.B.C
#define 	POSINNERID1		_T("071")	//우체국  
#define 	POSINNERID2		_T("072")	//우체국  
#define 	POSINNERID3		_T("073")	//우체국  
#define 	POSINNERID4		_T("074")	//우체국  
#define 	POSINNERID5		_T("075")	//우체국  


#define 	TMASTERCARD		_T("162")	//CUP	
#define 	TVISACARD		_T("161")	//AMEX	
#define 	TJCBCARD		_T("163")	//JCB	
#define 	TCUPCARD		_T("165")	//CUP	
#define 	TAMEXCARD		_T("160")	//AMEX	
#define 	TDINERSCARD		_T("164")	//DINERS
#define 	TDISCOVERCARD	_T("214")	//DISCOVER


#define 	HYDSBANKID_PUBLIC		_T("218")	//현대증권 공동망
#define 	TYJSBANKID_PUBLIC		_T("379")	//유안타 공동망 


#define 	TSAMSUNGCARD	_T("205")	//삼성카드

#define 	HYDCAPITALID	_T("261")	//현대캐피탈

#define 	TYJSBANKID		_T("209")	//동양종합금융증권
#define 	TYJSBANKIDC		_T("902")	//동양종합금융증권


#define 	HYDSBANKID		_T("218")	//현대증권

#define 	MRASBANKID		_T("230")	//미래에셋증권
#define 	DAWSBANKID		_T("238")	//대우증권
#define 	SASSBANKID		_T("240")	//삼성증권

#define 	MRASBANKIDC		_T("9MR")	//미래에셋증권
#define 	DAWSBANKIDC		_T("9DW")	//대우증권
#define 	SASSBANKIDC		_T("9SG")	//삼성증권

#define 	WRTSBANKID		_T("247")	//NH투자증권
#define 	WRTSBANKIDC		_T("9WM")	//NH투자증권
#define 	HKTSBANKID		_T("243")	//한국투자증권
#define 	KYBSBANKID		_T("261")	//교보증권
#define 	HISBANKID		_T("262")	//하이투자증권
#define 	HMCSBANKID		_T("263")	//에이치엠씨투자증권
#define 	KIWSBANKID		_T("264")	//키움증권
#define 	ETRSBANKID		_T("265")	//이트레이드증권
#define 	SKSBANKID		_T("266")	//에스케이증권
#define 	DASSBANKID		_T("267")	//대신증권
#define 	DASSBANKIDC		_T("9DS")	//대신증권
#define 	SOLSBANKID		_T("268")	//솔로몬투자증권
#define 	HAHSBANKID		_T("269")	//한화증권
#define 	HNDSBANKID		_T("270")	//하나대투증권
#define 	GOMSBANKID		_T("278")	//굿모닝신한증권
#define 	GOMSBANKIDC		_T("9GM")	//굿모닝신한증권
#define 	DOBSBANKID		_T("279")	//동부증권
#define 	EUGSBANKID		_T("280")	//유진투자증권
#define 	MRISBANKID		_T("206")	//메리트중권
#define 	MRISBANKIDC		_T("9ME")	//메리트중권
#define 	NHTSBANKID		_T("289")	//엔에이치투자증권
#define 	BOKSBANKID		_T("290")	//부국증권
#define 	SIYSBANKID		_T("291")	//신영증권
#define 	LIGSBANKID		_T("292")	//엘아이지투자증권

#define 	NHMUINPUTID		_T("100")	//매출금입금  
#define 	DEFAULTID		_T("902")	//                                     	


/*************브랜드구분코드************************/  
#define NICE					0           // 전자금융
#define KB						1           // KB Brand CD
#define KIUP					2			// KIUP Brand CD
#define HOMEPLUS				3			// HOMEPLUS Brand CD
#define JEJU					4           // JEJU Brand CD
#define JEJU2					5			// WOORI Brand CD
#define SHINHAN					6			// SHINHAN Brand CD
#define MYASSET					7			// TONGYANG Brand CD
#define KWANGJU					8			// KWANGJU Brand CD
#define DAEWOO					9			// DEAWOO Brand CD
#define SC  					10			// SC Brand CD
#define MERITZ 					11			// MERITZ Brand CD
#define WOORISTOCK 				12			// WOORI Investments & Securities Brand CD
#define HYDCPTL 				13			// Hyundai Capital Brand CD
#define NONGHYUP 				14			// NongHyup Brand CD (012)
#define HANASTOCK 				15			// Hana Daetoo Securities Brand CD
#define POST					16			// POST Brand CD
#define SAMSSTOCK 				17			// Samsung Stock Securities Brand CD
#define MRASSTOCK 				18			// 미래에셋 Stock Securities Brand CD
#define HKTSSTOCK 				19			// 한국투자 Stock Securities Brand CD
#define HAHSSTOCK 				20			// 한화 Stock Securities Brand CD
#define SHHSSTOCK 				21			// 신한금융투자 Stock Securities Brand CD
#define KEB						22			// KEB Brand CD
#define SUHYUP					23			// SUHYUP Brand CD
#define CITI					24			// CITI Brand CD
#define HANA					25			// HANA Brand CD
#define DASSSTOCK				26			// 대신증권 Stock Securities Brand CD
#define HYDSSTOCK 				27			// 현대증권 Stock Securities Brand CD
#define KYONGNAM 				28			// KYONGNAM Brand CD  #N0192
#define BUSAN 					29			// BUSAN Brand CD
// 30번은 서버에서 외환/하나 브랜드로 사용중임
#define DAEGU 					31			// DAEGU Brand ATM
#define JEONBUK 				32			// JEONBUK Brand CD
#define DAEGU_CD 				33			// JEONBUK Brand CD (휴게소 설치기기 전용, 서버의 브랜드코드 (수수료체계)가 다름)
#define NONGHYUPDAN				34			// 농협단위(012)

#define KB_Brand				"01"			// KB Brand CD
#define KIUP_Brand				"02"			// KIUP Brand CD
#define HOMEPLUS_Brand			"03"			// HOMEPLUS Brand CD
#define JEJU_Brand				"04"			// JEJU Brand CD
#define JEJU2_Brand				"05"			// WOORI Brand CD
#define SHINHAN_Brand			"06"			// SHINHAN Brand CD
#define MYASSET_Brand			"07"			// TONGYANG Brand CD
#define KWANGJU_Brand			"08"			// KWANGJU Brand CD
#define DAEWOO_Brand			"09"			// DEAWOO Brand CD
#define SC_Brand  				"10"			// SC Brand CD
#define MERITZ_Brand 			"11"			// MERITZ Brand CD
#define WOORISTOCK_Brand 		"12"			// WOORI Investments & Securities Brand CD
#define HYDCPTL_Brand 			"13"			// Hyundai Capital Brand CD
#define NONGHYUP_Brand 			"14"			// NongHyup Brand CD
#define HANASTOCK_Brand 		"15"			// Hana Daetoo Securities Brand CD
#define POST_Brand				"16"			// POST Brand CD
#define SAMSSTOCK_Brand 		"17"			// Samsung Stock Securities Brand CD
#define MRASSTOCK_Brand 		"18"			// 미래에셋 Stock Securities Brand CD
#define HKTSSTOCK_Brand 		"19"			// 한국투자 Stock Securities Brand CD
#define HAHSSTOCK_Brand 		"20"			// 한화 Stock Securities Brand CD
#define SHHSSTOCK_Brand 		"21"			// 신한금융투자 Stock Securities Brand CD
#define KEB_Brand				"22"			// KEB Brand CD
#define SUHYUP_Brand			"23"			// SUHYUP Brand CD
#define CITI_Brand				"24"			// CITI Brand CD
#define HANA_Brand				"25"			// HANA Brand CD
#define DASSSTOCK_Brand 		"26"			// 대신증권 Stock Securities Brand CD
#define KYONGNAM_Brand			"28"			// KYONGNAM Brand CD #N0192
#define BUSAN_Brand				"29"			// BUSAN Brand CD
#define DAEGU_Brand				"31"			// DAEGU Brand ATM
#define JEONBUK_Brand			"32"			// JEONBUK Brand CD
#define DAEGU_Brand_CD			"33"			// DAEGU Brand CD  (휴게소 설치기기 전용, 서버의 브랜드코드 (수수료체계)가 다름)
#define NONGHYUPDAN_Brand		"34"			// 농협단위

/*************브랜드구분코드************************/  

/*************내부 코드************************/  
#define		OTHBANK		100 //기타은행
#define     KDBBANK		2	//산업은행	
#define 	KIUBANK		3	//기업은행	
#define 	KUMBANK		4	//국민은행	(30,29,04,06,09)
#define 	KOEBANK		5	//외환은행	
#define 	SUHBANK		7	//수협은행	
#define 	NONBANK		11	//농협      (17,16,12,11)
#define 	WORBANK		20	//우리은행  (20,22,24,83)  
#define 	CHOBANK		21	//조흥은행     
#define 	KOFBANK		23	//제일은행     
#define 	HANBANK		25	//하나은행  (25,81,82,33)   
#define 	SHIBANK		26	//신한은행     
#define 	GOOBANK		27	//한미은행  (27,36)   
#define 	DEABANK		31	//대구은행     
#define 	PUSBANK		32	//부산은행     
#define 	KAWBANK		34	//광주은행     
#define 	JEJBANK		35	//제주은행          
#define 	JEOBANK		37	//전북은행     
#define 	KYOBANK		39	//경남은행     
#define 	KFCBANK		45	//새마을금     
#define 	SINBANK		48	//신협	     
#define 	SANBANK		50	//상호저축     
#define 	CITBANK		53	//씨티은행     
#define 	HSBABNK		54	//H.S.B.C	     
#define 	POSBANK		71	//우체국  (71,72)	     
   
#define		ETCCARD		400	//해당사항무
#define 	KUMINCARD	401	//국민카드     
#define 	HYUNDAICARD	402	//현대카드     
#define 	MASTERCARD	403	//MASTER	     
#define 	BC_CARD		404	//비씨카드     
#define 	SAMSUNGCARD	405	//삼성카드     
#define 	LOTTECARD	406	//롯데카드     
#define 	LG_CARD		407	//엘지카드     
#define 	YESCARD		408	//외환카드     
#define 	VISA_CARD	409	//VISA	
#define 	JCBCARD		410	//JCB	
#define 	CUPCARD		411	//CUP	
#define 	AMEXCARD	412	//AMEX	
#define 	DINERS		413	//DINERS	
#define 	DISCOVER	414	//DISCOVER	     
#define 	KWANGSUCARD 421	//광주카드     
#define 	SHINHANCARD 428	//신한카드     
#define 	CHBCARD		430	//조흥카드     
#define 	GOODCARD	433	//한미카드     
#define 	WOORICARD	437	//우리카드
#define 	NHCARD	    411	//농협카드
#define 	ISUHCARD    407	//수협카드
#define 	JEJCARD		435	//제주카드   

#define 	HYDCAPITAL	261	//현대캐피탈

#define 	TYJSBANK	209 //동양종합금융증권
#define 	HYDSBANK	218	//현대증권
#define 	MRASBANK	230	//미래에셋증권
#define 	DAWSBANK	238	//대우증권
#define 	SASSBANK	240	//삼성증권
#define 	WRTSBANK	247	//NH투자증권
#define 	HKTSBANK	243	//한국투자증권
#define 	KYBSBANK	261	//교보증권
#define 	HISBANK		262	//하이투자증권
#define 	HMCSBANK	263	//에이치엠씨투자증권
#define 	KIWSBANK	264	//키움증권
#define 	ETRSBANK	265	//이트레이드증권
#define 	SKSBANK		266	//에스케이증권
#define 	DASSBANK	267	//대신증권
#define 	SOLSBANK	268	//솔로몬투자증권
#define 	HAHSBANK	269	//한화증권
#define 	HNDSBANK	270	//하나대투증권
#define 	GOMSBANK	278	//굿모닝신한증권
#define 	DOBSBANK	279	//동부증권
#define 	EUGSBANK	280	//유진투자증권
/*
#define 	MRISBANK	206	//메리트중권
#define 	NHTSBANK	225	//엔에이치투자증권
#define 	BOKSBANK	226	//부국증권
#define 	SIYSBANK	227	//신영증권
#define 	LIGSBANK	228	//엘아이지투자증권
*/
//#N0131
#define 	MRISBANK	287	//메리트중권
#define 	NHTSBANK	289	//엔에이치투자증권
#define 	BOKSBANK	290	//부국증권
#define 	SIYSBANK	291	//신영증권
#define 	LIGSBANK	292	//엘아이지투자증권




#define 	UTICKET		804	//통합티켓
#define 	INTERAD		806	//인터렉티브광고
#define 	MOBILEGAME	804	//모바일게임다운로드


// ATM EP_VERSION
#define REG_GETEP_VERSION		_T("SOFTWARE\\ATM\\EP_Version")
// ATM SP_VERSION
#define REG_GETSP_VERSION		_T("SOFTWARE\\ATM\\SP_Version")
				

// ATM SEND FLAG
#define		ATM_SENDFLAG				_T("CQ")
#define		ATM_RECVFLAG				_T("SR")
#define		ATM_RETURN_OK				_T("0000")

// ATM SEND FORMAT REG_
#define		REG_TXATM0300				_T("SOFTWARE\\ATM\\TXRXData\\TXATM0300")
#define		REG_TXATM0330				_T("SOFTWARE\\ATM\\TXRXData\\TXATM0330")

// ATM DEVICE UNIT STATUS_
#define		REG_STATUSCOM				_T("SOFTWARE\\ATM\\TXRSData\\STATUSCOM")


#define	    TXDATAFIELD		1									//송신
#define	    RXDATAFIELD		2									//수신

#define     TRANID_0300		300									//0300:현송집계
#define     TRANID_0310		310									//0310:수표장착 정보 통보 전문
#define     TRANID_0311		311									//0311:수표 집계 요청 전문
#define     TRANID_0330		330									//0330:현송정보통보
#define     TRANID_0410		410									//0410:인증서신청통보
#define     TRANID_0415		415									//0415:세션키신청통보
#define     TRANID_0501		501									//0501:인증완료통보

#define		SUBINDEX00		0
#define		SUBINDEX01		1
#define		SUBINDEX02		2
#define		SUBINDEX03		3
#define		SUBINDEX04		4
#define		SUBINDEX05		5
#define		SUBINDEX06		6

#define		ENCRYPTMETHOD0  0
#define		ENCRYPTMETHOD1  1
#define		ENCRYPTMETHOD2  2
#define		ENCRYPTMETHOD3  3
#define		ENCRYPTMETHOD4  4
#define		ENCRYPTMETHOD5  5
#define		ENCRYPTMETHOD6  6
#define		ENCRYPTMETHOD7  7
#define		ENCRYPTMETHOD8  8
#define		ENCRYPTMETHOD9  9

#define	    TRANID_0006			6									//0006:라인전문
#define		TRANID_0006_S		"0006"

typedef struct tag031XFormat{		
	char	OrgNo[3];				// OrgCode
	char	Dummy;					// NULL
} F031X;

typedef struct tag031XINFO{	
	F031X    F031XLIST[100];				// 거래가능기관정보																	
} F031XINFO;

/*********************************************************************************************************************************/  
/* 업무구분 - Tran-I */
#define	    TRANID_A010         10      //0800	0810	단말기 개국전문                               
#define	    TRANID_A020         20      //0800	0810	단말기 상태전문      
#define	    TRANID_A040         40      //0800	0810	단말기 제어전문                                                           
#define	    TRANID_A060         60      //0800	0810	단말기 시제전문    
#define	    TRANID_2190         2190    //0800	0810	단말기 승인거래   

#define	    TRANID_7230         7230    //0200	0210	서비스조회 
#define	    TRANID_7250         7250    //0200	0210	서비스이체조회 
#define	    TRANID_7380         7380    //0200	0210	이자율조회              //define	    TRANID_7110         7110    //0200	0210	이자율조회 현금서비스 인출	
#define	    TRANID_4330         4330    //0200	0210	서비스출금  
#define	    TRANID_4350         4350    //0200	0210	서비스이체    

#define	    TRANID_7210         7210    //0200	0210	수수료조회              //#define	    TRANID_7100         7100    //0200	0210	수수료조회   
#define	    TRANID_4310         4310    //0200	0210	예금출금  
#define	    TRANID_7260         7260    //0200	0210	수취계좌조회     
#define	    TRANID_4370         4370    //0200	0210	자행이체                //자행 별도 
#define	    TRANID_4390         4390    //0200	0210	타사간이체  
#define	    TRANID_7220         7220    //0200	0210	예금조회

#define	    TRANID_7530         7530    //0200	0210	수수료조회(해외)  

#define	    TRANID_4320         4320    //5400	5410	예금출금취소   
#define	    TRANID_4340         4340    //5400	5410	서비스출금취소 

#define	    TRANID_7200         7200    //0200	0210	카드입금                 //#define	    TRANID_7226         7226    //0200	0210	입금가능여부조회 
#define	    TRANID_4410         4410    //0200	0210	카드입금                 //#define	    TRANID_4710         4710    //0200	0210	카드입금 
#define	    TRANID_4420         4420    //0200	0210	카드입금                 //국민브랜드


/*********************************************************************************************************************************/                   
#define	    TRANID_4311         4311    //0200	0210	해외예금지급                          
#define	    TRANID_4312         4312    //0200	0210	해외당좌거래지급                                      
#define	    TRANID_4314         4314    //0200	0210	대체출금                              
#define	    TRANID_4315         4315    //0200	0210	은행이체출금 
#define	    TRANID_4316         4316    //0200	0210	해외 BC출금   #N0171                        
                       
#define	    TRANID_4321         4321    //5400	5410	해외예금지급취소                      
#define	    TRANID_4322         4322    //5400	5410	해외당좌거래지급취소                               
#define	    TRANID_4324         4324    //5400	5410	대체출금 취소 
#define	    TRANID_4325         4325    //0200	0210	해외 BC출금 취소   #N0171       
#define	    TRANID_4326         4326    //5400	4326	무매체출금 취소                    
                          
#define	    TRANID_4331         4331    //0200	0210	해외현금서비스지급                    
#define	    TRANID_4332         4332    //0200	0210	중국은련서비스 지급                   
#define	    TRANID_4336         4336    //0200	0210	투신매도 처리                         
#define	    TRANID_4339         4339    //0200	0210	폰번호이체 받기                       
                   
#define	    TRANID_4341         4341    //5400	5410	해외서비스지급취소                    
#define	    TRANID_4342         4342    //5400	5410	중국은련서비스 지급취소               
#define	    TRANID_4349         4349    //5400	5410	폰번호이체 받기취소                   
                          
#define	    TRANID_4360         4360    //0200	0210	보험계약대출지급                      
#define	    TRANID_4361         4361    //0200	0210	캐피탈론지급                                                    
#define	    TRANID_4391         4391    //0200	0210	계좌이체(온누리상품권 이체판매)       
#define	    TRANID_4392         4392    //0200	0210	e-경조 계좌이체                                
#define	    TRANID_4394         4394    //0200	0210	IC카드 계좌이체(온누리상품권 이체판매)
#define	    TRANID_4395         4395    //0200	0210	e-경조 수납이체                       
#define	    TRANID_4399         4399    //0200	0210	계좌이체(폰번호/받기)

#define	    TRANID_4300         4300    //5400	5410	보험계약대출지급취소                  
#define	    TRANID_4301         4301    //5400	5410	캐피탈론 지급취소                     
#define	    TRANID_4210         4210    //0200	0210	예금출금(통장)                        
#define	    TRANID_4116         4116    //0200	0210	무매체출금                       
#define	    TRANID_4214         4214    //0200	0210	대체출금(통장)                        
#define	    TRANID_4215         4215    //0200	0210	은행이체출금(통장)                    
#define	    TRANID_4396         4396    //0200	0210	은행이체출금(무매체)                    
#define	    TRANID_4216         4216    //0200	0210	e-경조이체                            
#define	    TRANID_4220         4220    //5400	5410	출금취소(통장)    
                 
#define	    TRANID_4520         4520    //0200	0210	현대 론서비스 지급                    
#define	    TRANID_4530         4530    //5400	5410	현대 론서비스 지급취소                
#define	    TRANID_4540         4540    //0200	0210	롯데 론서비스 지급                    
#define	    TRANID_4550         4550    //5400	5410	롯데 론서비스 지급취소                
#define	    TRANID_4560         4560    //0200	0210	외환 해외송금                         
#define	    TRANID_4570         4570    //0200	0210	신한카드론 지급                       
#define	    TRANID_4571         4571    //0200	0210	신한카드론 이체                       
#define	    TRANID_4572         4572    //0200	0210	신한카드론 약정                       
#define	    TRANID_4580         4580    //0200	0210	신한카드론 지급취소                   
#define	    TRANID_4610         4610    //0200	0210	T-Money 제휴충전                      
#define	    TRANID_4620         4620    //5400	5410	T-Money 제휴충전취소                  
#define	    TRANID_4611         4611    //0200	0210	T-Money 비제휴충전                    
#define	    TRANID_4621         4621    //5400	5410	T-Money 비제휴충전취소                
#define	    TRANID_4631         4631    //0200	0210	T-Money 마일리지조회                  
#define	    TRANID_4630         4630    //0200	0210	T-Money 마일리지충전                  
#define	    TRANID_4640         4640    //5400	5410	T-Money 마일리지취소                  
#define	    TRANID_4650         4650    //0200	0210	T-Money비제휴 예금출금                
#define	    TRANID_4660         4660    //5400	5410	T-Money비제휴 예금출금취소            
#define	    TRANID_4670         4670    //0200	0210	하이패스 충전요청                     
#define	    TRANID_4671         4671    //0200	0210	하이패스 충전확인                     
#define	    TRANID_4680         4680    //0200	0210	하이패스 충전복구                     
#define	    TRANID_4681         4681    //0200	0210	하이패스 충전복구조회                 
#define	    TRANID_4682         4682    //0200	0210	하이패스 충전복구확인                 
                            
#define	    TRANID_4711         4711    //0200	0210	무매체입금                            
#define	    TRANID_4712         4712    //0200	0210	이마트슈퍼 매출금입금                 
#define	    TRANID_4713         4713    //0200	0210	IC카드입금                            
#define	    TRANID_4719         4719    //5400	5410	입금취소                              
#define	    TRANID_4720         4720    //0200	0210	통장입금                              
#define	    TRANID_4716         4716    //0200	0210	입금(무매체)                              
#define	    TRANID_4730         4730    //0200	0210	통장정리                              
#define	    TRANID_4810         4810    //0200	0210	공과금납부                            
#define	    TRANID_4811         4811    //0200	0210	지방세 간편납부(MS)                   
#define	    TRANID_4812         4812    //0200	0210	지방세 간편납부(신용카드)             
#define	    TRANID_4813         4813    //0200	0210	IC카드 공과금납부                     
#define	    TRANID_4814         4814    //0200	0210	지방세 간편납부(IC)                   
#define	    TRANID_4820         4820    //0200	0210	자동좌석/주문결제                     
#define	    TRANID_4821         4821    //0200	-	출력보고                              
#define	    TRANID_4822         4822    //0200	0210	발권정보 출력                         
#define	    TRANID_4830         4830    //0200	0210	TC 예매                               
#define	    TRANID_4840         4840    //0200	0210	신용카드 승인(도로공사)               
#define	    TRANID_4850         4850    //5400	5410	신용카드 취소(도로공사)               
#define	    TRANID_4841         4841    //0200	0210	신용카드 승인(에버랜드)               
#define	    TRANID_4851         4851    //5400	5410	신용카드 취소(에버랜드)               
#define	    TRANID_4842         4842    //0200	0210	신용카드 승인(효성티켓)               
#define	    TRANID_4852         4852    //5400	5410	신용카드 취소(효성티켓)               
#define	    TRANID_4860         4860    //0200	0210	상품권판매정보통보                    
#define	    TRANID_7000         7000    //0200	0210	다계좌조회                                    
#define	    TRANID_7004         7004    //0200	0210	다계좌조회(입금)                              
#define	    TRANID_7005         7005    //0200	0210	다계좌조회(투신)                              
#define	    TRANID_7006         7006    //0200	0210	다계좌조회(대체)                              
                                    
#define	    TRANID_7101         7101    //0200	0210	수수료조회(통장)  
#define	    TRANID_7106         7106    //0200	0210	수수료조회(무매체)                              
#define	    TRANID_7103         7103    //0200	0210	IC카드 수수료조회
#define	    TRANID_7104         7104    //0200	0210	해외취급수수료조회                              
#define	    TRANID_7105         7105    //0200	0210	해외BC취급수수료조회  
#define	    TRANID_7110         7110    //0200	0210	이자율조회 현금서비스 인출	#N0161
#define	    TRANID_7111         7111    //0200	0210	이자율조회 현금서비스 이체	#N0161                           
#define	    TRANID_7120         7120    //0200	0210	예금조회(통장)                                
#define	    TRANID_7124         7124    //0200	0210	대체출금가능조회(통장)                        
#define	    TRANID_7125         7125    //0200	0210	은행이체출금가능조회(통장)                    
#define	    TRANID_7126         7126    //0200	0210	입금가능여부조회(통장)                        
                                      
#define	    TRANID_7221         7221    //0200	0210	해외예금잔액조회                              
#define	    TRANID_7222         7222    //0200	0210	해외당좌거래잔액조회                          
#define	    TRANID_7224         7224    //0200	0210	대체출금가능조회                              
#define	    TRANID_7225         7225    //0200	0210	은행이체출금가능조회                                                     
#define	    TRANID_7236         7236    //0200	0210	입금가능여부조회(무매체)                                                       
#define	    TRANID_7228         7228    //0200	0210	지점정보조회 
#define	    TRANID_7229         7229    //0200	0210	통장 잔액 조회   #N0239                               
                                   
#define	    TRANID_7231         7231    //0200	0210	해외현금서비스조회                            
#define	    TRANID_7232         7232    //0200	0210	중국은련서비스 잔액조회  
#define	    TRANID_7233         7233    //5400	0210	해외예금잔액조회 취소                              
#define	    TRANID_7235         7235    //5400	0210	해외당좌거래잔액조회 취소
#define	    TRANID_7237         7237    //5400	0210	해외신용조회 취소                      
#define	    TRANID_7240         7240    //0200	0210	보험계약대출조회                              
#define	    TRANID_7241         7241    //0200	0210	캐피탈론잔액조회                              
                               
                                
#define	    TRANID_7261         7261    //0200	0210	수취계좌조회(통장)                            
#define	    TRANID_7276         7276    //0200	0210	수취계좌조회(무매체)                            
#define	    TRANID_7262         7262    //0200	0210	계좌성명조회(e-경조 수취조회)                                     
#define	    TRANID_7265         7265    //0200	0210	e-경조 수납이체 계좌등록                      
#define	    TRANID_7266         7266    //0200	0210	실명확인                                      
#define	    TRANID_7267         7267    //0200	0210	받을금액조회                                  
#define	    TRANID_7268         7268    //0200	0210	인증번호받기                                  
#define	    TRANID_7269         7269    //0200	0210	수취계좌조회(폰번호/받기)                     
#define	    TRANID_7280         7280    //0200	0210	롯데 론서비스 조회                            
#define	    TRANID_7281         7281    //0200	0210	롯데 론서비스 지급조회                        
#define	    TRANID_7290         7290    //0200	0210	외환 해외송금 계좌 조회                       
#define	    TRANID_7300         7300    //0200	0210	외환 해외송금 결과 조회                       
#define	    TRANID_7310         7310    //0200	0210	신용정보조회                                  
#define	    TRANID_7311         7311    //0200	0210	대출정보조회                                  
#define	    TRANID_7312         7312    //0200	0210	CB연체정보조회                                
#define	    TRANID_7313         7313    //0200	0210	신용카드 승인(한신정)                         
#define	    TRANID_7314         7314    //0200	0210	신용카드 결제                                 
#define	    TRANID_7315         7315    //0200	0210	신용정보 스코어 조회                          
#define	    TRANID_7316         7316    //0200	0210	신용카드 승인(현대캐피탈)                     
#define	    TRANID_7317         7317    //0200	0210	대출가능금액조회 - 진행                       
#define	    TRANID_7318         7318    //0200	0210	대출가능금액조회 - 종료                       
#define	    TRANID_7319         7319    //0200	0210	사전스크린                                    
#define	    TRANID_7320         7320    //0200	0210	공과금납부조회                                
#define	    TRANID_7321         7321    //0200	0210	공과금납부처리결과조회                        
#define	    TRANID_7322         7322    //0200	0210	지방세간편납부조회(MS)                        
#define	    TRANID_7323         7323    //0200	0210	IC카드 공과금납부조회                         
#define	    TRANID_7324         7324    //0200	0210	IC카드 공과금납부처리결과조회                 
#define	    TRANID_7325         7325    //0200	0210	지방세간편납부조회(IC)                        
#define	    TRANID_7326         7326    //0200	0210	지방세간편납부결과조회(MS)                    
#define	    TRANID_7327         7327    //0200	0210	지방세간편납부결과조회(IC)                    
#define	    TRANID_7328         7328    //0200	0210	지방세간편납부조회(신용카드)                  
#define	    TRANID_7329         7329    //0200	0210	지방세간편납부결과조회(신용카드)              
#define	    TRANID_7330         7330    //0200	0210	통장정리조회                                  
#define	    TRANID_7336         7336    //0200	0210	투신매도잔액조회                              
#define	    TRANID_7337         7337    //0200	0210	주문체결내역출력                              
#define	    TRANID_7338         7338    //0200	0210	잔고조회출력                                  
#define	    TRANID_7339         7339    //0200	0210	거래내역출력                                  
#define	    TRANID_7340         7340    //0200	0210	TC 예매 가능 여부 조회                        
#define	    TRANID_7341         7341    //0200	0210	티켓종류 조회                                 
#define	    TRANID_7342         7342    //0200	0210	판매처정보 조회                               
#define	    TRANID_7343         7343    //0200	0210	일정/공지사항 조회                            
#define	    TRANID_7344         7344    //0200	0210	권종정보 조회                                 
#define	    TRANID_7345         7345    //0200	0210	출발시간 조회                                 
                      
#define	    TRANID_7351         7351    //0200	0210	아파트담보대출 - 종료                         
#define	    TRANID_7352         7352    //0200	0210	전세자금대출 - 진행                           
#define	    TRANID_7353         7353    //0200	0210	전세자금대출 - 종료                           
#define	    TRANID_7354         7354    //0200	0210	휴대전화 승인(현대캐피탈)                     
#define	    TRANID_7357         7357    //0200	0210	고객연락처 등록(조인스랜드)                   
#define	    TRANID_7358         7358    //0200	0210	주민번호,통신사구분,핸드폰번호입력(운세서비스)
#define	    TRANID_7359         7359    //0200	0210	인증번호입력(운세서비스)                      
                                 
#define	    TRANID_7361         7361    //0200	0210	행사등록조회                                  
#define	    TRANID_7362         7362    //0200	0210	예약할인등록                                  
#define	    TRANID_7363         7363    //0200	0210	대출한도증액등록                              
#define	    TRANID_7364         7364    //0200	0210	신상보정조회                                  
#define	    TRANID_7365         7365    //0200	0210	신상보정등록                                  
#define	    TRANID_7366         7366    //0200	0210	한도상향등록                                  
#define	    TRANID_7367         7367    //0200	0210	행사등록                                      
#define	    TRANID_7370         7370    //0200	0210	대출가능상품조회                              
#define	    TRANID_7371         7371    //0200	0210	지급상세조회                                  
#define	    TRANID_7372         7372    //0200	0210	이체상세조회                                  
#define	    TRANID_7373         7373    //0200	0210	약정상세조회                                  
                                   
#define	    TRANID_7390         7390    //0200	0210	띠별운세                                      
#define	    TRANID_7400         7400    //0200	0210	포츈쿠키                                      
#define	    TRANID_7410         7410    //0200	0210	혈액형운세                                    
#define	    TRANID_7411         7411    //0200	0210	혈액형-사교                                   
#define	    TRANID_7412         7412    //0200	0210	혈액형-연애                                   
#define	    TRANID_7413         7413    //0200	0210	혈액형-레크레이션                             
#define	    TRANID_7420         7420    //0200	0210	아파트단지리시트조회                          
#define	    TRANID_7421         7421    //0200	0210	시세조회                                      
#define	    TRANID_7425         7425    //0200	0210	여부조회(현대)                                
#define	    TRANID_7426         7426    //0200	0210	한도상향가능금액조회(현대)                    
#define	    TRANID_7427         7427    //0200	0210	한도상향신청(현대)                            
#define	    TRANID_7428         7428    //0200	0210	캠페인조회(현대)                              
#define	    TRANID_7430         7430    //0200	0210	현대 론서비스 조회                            
#define	    TRANID_7431         7431    //0200	0210	현대 론서비스 지급조회                        
#define	    TRANID_7440         7440    //0200	0210	러쉬앤캐쉬 초간편 신청                        
#define	    TRANID_7441         7441    //0200	0210	러쉬앤캐쉬 본인인증                           
#define	    TRANID_7442         7442    //0200	0210	러쉬앤캐쉬 기고객필터                         
#define	    TRANID_7443         7443    //0200	0210	러쉬앤캐쉬 신용카드인증                       
#define	    TRANID_7444         7444    //0200	0210	러쉬앤캐쉬 인증번호 송신                      
#define	    TRANID_7445         7445    //0200	0210	러쉬앤캐쉬 핸드폰 정보 입력                   
#define	    TRANID_7446         7446    //0200	0210	러쉬앤캐쉬 인증건수조회                       
#define	    TRANID_7447         7447    //0200	0210	러쉬앤캐쉬 본인인증결과입력                   
#define	    TRANID_7448         7448    //0200	0210	러쉬앤캐쉬 대출한도 조회                      
#define	    TRANID_7449         7449    //0200	0210	러쉬앤캐쉬 계좌인증                           
#define	    TRANID_7450         7450    //0200	0210	러쉬앤캐쉬 송금요청                           
#define	    TRANID_7451         7451    //0200	0210	할인구매한도조회                              
                   


/////////////////////////////////////////////////////////////////////////////
//#N0174  외환 이지원 TRANID
#define	    TRANID_7500		    7500    //0200	0210	외환이지원 조회 MS
#define	    TRANID_7503		    7503    //0200	0210	외환이지원 조회 IC
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//#N0182  원머니 TRANID
#define	    TRANID_7107		    7107    //0200	0210	원머니 출금 조회
#define	    TRANID_4117		    4117    //0200	0210	원머니 출금
#define	    TRANID_4327		    4327    //0200	0210	원머니 출금 취소

/////////////////////////////////////////////////////////////////////////////
//#N0184  신한카드 한도 상향
#define	    TRANID_7374		    7374    //0200	0210	신한카드 한도상향 조회
#define	    TRANID_7375		    7375    //0200	0210	신한카드 한도상향 등록
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//#N0266  하이패스 충전
#define	    TRANID_4670		    4670    //0200	0210	하이패스 충전요청
#define	    TRANID_4671		    4671    //0200	0210	하이패스 충전확인
#define	    TRANID_4680		    4680    //0200	0210	하이패스 충전복구
#define	    TRANID_4681		    4681    //0200	0210	하이패스 충전복구확인

#define	    TRANID_4717		    4717    //0200	0210	하이패스 현금입금

#define	    TRANID_4840		    4840    //0200	0210	하이패스 신용카드 승인
#define	    TRANID_4850		    4850    //0200	0210	하이패스 신용카드 승인 취소

/////////////////////////////////////////////////////////////////////////////
 
/* DataField */
#define	   x081X                "0810"//전문Format구분
#define	   x001X                "0010"//거래날짜                                          
#define	   x002X                "0020"//거래시각                            
#define	   x003X                "0030"//SST 일련번호                        
#define	   x004X                "0040"//제휴사 일련번호 
#define	   x005X                "0050"//전문종류(Msg-Type)                  
#define	   x006X                "0060"//업무구분코드(Tran-ID)               
#define	   x007X                "0070"//거래매체구분                        
#define	   x008X                "0080"//응답코드(RspCode)   
#define	   x009X                "0090"//마감구분              
#define	   x011X                "0110"//SST기기번호(MecaNum)                
#define	   x048X                "0480"//신용카드정보유무                    
#define	   x049X                "0490"//SST기종구분  (14자리)                        
#define	   x010X                "0100"//암호화구분                          
#define	   x0120                "0120"//취급기관코드1                       
#define	   x0121                "0120"//취급기관코드2                       
#define	   x013X                "0130"//ISO3 Data                           
#define	   x0131                "0131"//ISO3 Data                           
#define	   x014X                "0141"//ISO2 Data                           
#define	   x015X                "0150"//비밀번호                            
#define	   x0151                "0151"//비밀번호 - 복호화  
#define	   x0152                "0152"//비밀번호 - 해외카드 DES      //#N0172                       
#define	   x016X                "0160"//통장 M/S Data
#define	   x0161                "0161"//통장 M/S Data 국민 출금 이체 조회 #N0239                          
#define	   x017X                "0170"//거래계좌(카드)번호1                 
#define	   x018X                "0180"//거래계좌(카드)성명                  
#define	   x019X                "0190"//Filler  
#define	   x0191                "0191"//광주 신용카드 암호화 DATA 104바이트 #N0181                   
#define	   x020X                "0200"//수취계좌번호                        
#define	   x021X                "0210"//수취계좌성명                        
#define	   x022X                "0220"//거래계좌(카드)잔액                  
#define	   x0221                "0221"//거래계좌(카드)잔액 - 암호화                  
#define	   x023X                "0230"//거래금액                            
#define	   x024X                "0240"//거래수수료                          
#define	   x025X                "0250"//미결제타점권 금액                   
#define	   x026X                "0260"//HOST일련번호     
#define	   x0262                "0262"//해외송금 계좌정보 외환 이지원 #N0174                        
#define	   x027X                "0270"//수표거래금액                        
#define	   x028X                "0280"//수표매수                            
#define	   x029X                "0290"//수표관련 정보  
#define	   x031X                "0310"//거래가능기관정보 
#define	   x038X                "0380"                    
#define	   x041X                "0410"//암호화Key길이 (삼성증권암호화)                     
#define	   x042X                "0420"                      
#define	   x043X                "0430"                     
#define	   x044X                "0440"//보조암호화Key 
#define	   x0441                "0441"//보조암호화Key 
                      
#define	   x045X                "0450"//최종승인허가일자 
#define	   x0451                "0451"//최종승인허가일자
#define	   x050X                "0500"//일련번호  
#define	   x0520                "0520"//거래계좌번호
#define	   x0521                "0521"
#define	   x0522                "0522"
#define	   x054X                "0540"
#define	   x055X                "0550"
#define	   x056X                "0560"//이전시제합계일자 
#define	   x057X                "0570"
#define	   x058X                "0580"
 
#define	   x0571                "0571"
#define	   x0581                "0581"
        
#define	   x059X                "0590"//출금가능 금액(수표)       
#define	   x061X                "0610"//권종별 매수정보
#define	   x062X                "0620"//권종별 매수정보          
#define	   x063X                "0630"//거래개시일시                        
#define	   x064X                "0640"//HOST일련번호                        
#define	   x065X                "0650"//전자화폐 충전 할증금액              
#define	   x066X                "0660"//화폐종류                            
#define	   x067X                "0670"//전자화폐 일련번호(IC카드일련번호)   
#define	   x068X                "0680"//거래카운터                          
#define	   x0691                "0691"//충전전카드잔액                      
#define	   x070X                "0700"//난수                                
#define	   x071X                "0710"//인증자                              
#define	   x072X                "0720"//카드종류                            
#define	   x073X                "0730"//알고리즘 ID                         
#define	   x074X                "0740"//VK                                  
#define	   x075X                "0750"//센터ID                              
#define	   x076X                "0760"//발행기관ID                          
#define	   x077X                "0770"//이용자카드ID                        
#define	   x078X                "0780"//PAN                                 
#define	   x079X                "0790"//발행자ID                            
#define	   x080X                "0800"//승인 신용카드(계좌)번호             
#define	   x085X                "0850"//통신사 구분                         
#define	   x086X                "0860"//인증서 요청 데이터 (086X)                           
#define	   x087X                "0870"//최초거래구분                        
#define	   x088X                "0880"//통장정리 대상 여부                  
#define	   x089X                "0890"//통장거래관련 화면안내 메시지        
#define	   x090X                "0900"//계좌잔액 인자여부                   
#define	   x091X                "0910"//통장정리 지정면                     
#define	   x092X                "0920"//통장정리 지정행                     
#define	   x093X                "0930"//통장정리 대상건수                   
#define	   x094X                "0940"//통장정리내역                        
#define	   x0941                "0941"//통장정리내역  
#define	   x0942                "0942"//통장정리내역  
#define	   x0943                "0943"//통장정리내역                          
#define	   x095X                "0950"//통장정리자료 추가 존재 여부         
#define	   x0951                "0951"//통장정리자료 추가 존재 여부  (경남) #N0277
#define	   x096X                "0960"//통장패턴번호                        
#define	   x097X                "0970"//통장서브패턴                        
#define	   x098X                "0980"//추가 기장용 KEY                     
#define	   x099X                "0990"//화면안내문구                         
#define	   x100X                "1000"//대상기간                            
#define	   x101X                "1010"//조회구분                            
#define	   x104X                "1040"//명세표 인자Data                     
#define	   x127X                "1270"//만원권 출금매수                     
#define	   x1271                "1271"//만원권 입금매수                     
#define	   x128X                "1280"//5만원권 출금매수                    
#define	   x1281                "1281"//5만원권 출금매수                    
#define	   x129X                "1290"//10만원권 출금매수                   
#define	   x130X                "1300"//거절응답사유                        
#define	   x131X                "1310"//기사용금액                          
#define	   x166X                "1660"//대출상품코드                        
#define	   x167X                "1670"//대출요청개월수                      
#define	   x1680                "1680"//대출이율                            
#define	   x1810                "1810"//연체이율   
#define	   x458X                "4580"//연체이율   
                         
#define	   x169X                "1690"//원리금납입일                        
#define	   x170X                "1700"//만기일                              
#define	   x171X                "1710"//대출가능금액1                       
#define	   x172X                "1720"//취급수수료
#define	   x1721                "1721"//하나은행 취급수수료  #N0214                         
#define	   x189X                "1890"//오천원권 매수                       
#define	   x1891                "1891"//오천원권 매수                       
#define	   x190X                "1900"//천원권 매수                         
#define	   x1901                "1901"//천원권 매수                         
#define	   x191X                "1910"//지로번호                            
#define	   x192X                "1920"//전자납부번호(일련번호)              
#define	   x193X                "1930"//지로납부내역
#define	   x1931                "1931"//지방세 납부내역       
#define	   x194X                "1940"//이용기관명(거래점 번호)             
#define	   x195X                "1950"//납부결과조회내역                    
#define	   x196X                "1960"//할부개월수                          
#define	   x201X                "2010"//신용정보조회서비스 요청공통부       
#define	   x202X                "2020"//휴대전화 SMS발송 인증키             
#define	   x204X                "2040"//단지코드                            
#define	   x205X                "2050"//아파트명                            
#define	   x206X                "2060"//아파트주소                          
#define	   x207X                "2070"//부동산 시세조회 상세내역            
#define	   x208X                "2080"//추가 응답코드                       
#define	   x209X                "2090"//운세 데이터                         
#define	   x210X                "2100"//신용정보조회서비스 응답공통부       
#define	   x211X                "2110"//신용정보조회 채무불이행(은연) 내역  
#define	   x811X                "8110"//신용정보조회 채무불이행(은연) 2     
#define	   x212X                "2120"//신용정보조회 채무불이행(신정) 내역  
#define	   x812X                "8120"//신용정보조회 채무불이행(신정) 2     
#define	   x213X                "2130"//신용정보조회 신용개설정보 내역      
#define	   x813X                "8130"//신용정보조회 신용개설정보 2         
#define	   x214X                "2140"//신용정보조회 조회기록정보 내역      
#define	   x215X                "2150"//대출정보 내역                       
#define	   x815X                "8150"//대출정보 내역 2                     
#define	   x216X                "2160"//현금서비스 정보 내역                
#define	   x816X                "8160"//현금서비스 정보 내역 2              
#define	   x217X                "2170"//채무보증정보 내역                   
#define	   x817X                "8170"//채무보증정보 내역 2                 
#define	   x218X                "2180"//CB연체정보 내역                     
#define	   x818X                "8180"//CB연체정보 내역 2                   
#define	   x828X                "8280"//CB연체정보 내역 3                   
#define	   x219X                "2190"//평점정보 내역                       
#define	   x220X                "2200"//주민등록번호(농협:암호화해서)       
#define	   x221X                "2210"//승인번호(주문번호)                  
#define	   x222X                "2220"//전단계 거래고유번호                 
#define	   x223X                "2230"//대출진단 송수신 공통부              
#define	   x224X                "2240"//이전거래 SST일련번호                
#define	   x225X                "2250"//운세구분 데이터  
#define	   x226X                "2260"//통장정리 1:일반기장, 2:압축기장                    
#define	   x230X                "2300"//통장정리데이터 길이                 
#define	   x231X                "2310"//통장정리내역1                       
#define	   x232X                "2320"//통장정리내역2                       
#define	   x233X                "2330"//통장정리내역3                       
#define	   x234X                "2340"//통장정리내역4                       
#define	   x235X                "2350"//통장정리내역5                       
#define	   x236X                "2360"//통장정리내역6                       
#define	   x237X                "2370"//통장정리내역7                       
#define	   x238X                "2380"//통장정리내역8                       
#define	   x240X                "2400"//판매처/일정 정보 내역(에버랜드티켓) 
#define	   x241X                "2410"//권종 정보 응답내역(에버랜드티켓)    
#define	   x242X                "2420"//신용승인요청내역(에버랜드티켓)      
#define	   x243X                "2430"//신용승인응답내역(에버랜드티켓)      
#define	   x244X                "2440"//안내메세지                          
#define	   x245X                "2450"//공지사항1                           
#define	   x246X                "2460"//공지사항2                           
#define	   x247X                "2470"//티켓내역 응답(에버랜드티켓)         
#define	   x248X                "2480"//판매처 응답내역(에버랜드티켓)       
#define	   x249X                "2490"//일정/공지사항 응답내역(에버랜드티켓)
#define	   x250X                "2500"//출발시간 응답내역                   
#define	   x251X                "2510"//자동좌석 지정 요청 내역             
#define	   x252X                "2520"//자동좌석 지정 응답 내역             
#define	   x253X                "2530"//주문결제 응답 내역                  
#define	   x254X                "2540"//출력매수보고 내역                   
#define	   x255X                "2550"//발권 응답 내역 1(에버랜드티켓)      
#define	   x256X                "2560"//발권 응답 내역 2(에버랜드티켓)      
#define	   x257X                "2570"//발권 응답 내역 3(에버랜드티켓)      
#define	   x258X                "2580"//발권 응답 내역 4(에버랜드티켓)      
#define	   x260X                "2600"//TC/TE예매 조회/요청 공통내역        
#define	   x261X                "2610"//TC/TE예매 요청 내역                 
#define	   x270X                "2700"//지급가능 금액조회                   
#define	   x271X                "2710"//예수금 조회                         
#define	   x272X                "2720"//계좌관련 기타정보                   
#define	   x273X                "2730"//상품, 종목관련 기타정보             
#define	   x280X                "2800"//종목코드                            
#define	   x281X                "2810"//종목명                              
#define	   x282X                "2820"//수량                                
#define	   x283X                "2830"//청약증거금                          
#define	   x284X                "2840"//대출(가능)금액                      
#define	   x285X                "2850"//청약(가능)금액                      
#define	   x286X                "2860"//대출금액화면                        
#define	   x287X                "2870"//대출상품유형분류                    
#define	   x288X                "2880"//청구방법                            
#define	   x289X                "2890"//상환방법                            
#define	   x290X                "2900"//대출상품화면                        
#define	   x291X                "2910"//신한카드론 상품상세                 
#define	   x299X                "2990"//시스템 공지사항                     
#define	   x300X                "3000"//마케팅 대상여부                     
#define	   x301X                "3010"//한도상향 데이터                     
#define	   x302X                "3020"//행사등록 데이터                     
#define	   x303X                "3030"//신상보정 데이터                     
#define	   x304x                "3040"//여부조회 데이터(현대)               
#define	   x305x                "3050"//한도상향 데이터(현대)               
#define	   x306x                "3060"//캠페인 데이터(현대)                 
#define	   x310X                "3100"//인증매체구분 
#define	   x3101                "3101"//인증매체구분                       
#define	   x311X                "3110"//가상계좌은행코드                    
#define	   x312X                "3120"//가상계좌번호                        
#define	   x313X                "3130"//첫이자입금일                        
#define	   x314X                "3140"//대출진단ID                          
#define	   x315X                "3150"//할인율                              
#define	   x316X                "3160"//판매상품권일련번호                  
#define	   x320X                "3200"//거스름돈 방출정보                   
#define	   x915X                "9150"//비밀번호(농협/하나/삼성증/광주)     
#define	   x9151                "9151"//비밀번호(농협/하나/삼성증/광주)     
#define	   x034X                "0340"//처리요구구분(장애구분)      
#define	   x035X                "0350"//지폐함 및 용지 상태정보     
#define	   x036X                "0360"//지폐함별 권종 설정정보      
#define	   x047X                "0470"//SST Serial Number(SST-ID)   
#define	   x046X                "0460"//제어세부내용
#define	   x051X                "0510"//제어코드
#define	   x083X                "0830"//통신망관리정보              
#define	   x084X                "0840"//Shutdown시각                
#define	   x102X                "1020"//H/W Module 상태정보         
#define	   x103X                "1030"//자원 감시 상태정보          
#define	   x105X                "1050"//총예금출금건수              
#define	   x106X                "1060"//총예금출금금액              
#define	   x107X                "1070"//수표 출금건수               
#define	   x108X                "1080"//수표 출금금액               
#define	   x109X                "1090"//총서비스출금건수  
#define	   x110X                "1100"//총서비스출금금액                 
#define	   x111X                "1110"//총서비스출금취소건수      
#define	   x112X                "1120"//총서비스출금취소금액
#define	   x113X                "1130"//이체건수      
#define	   x114X                "1140"//이체금액
#define	   x117X                "1170"//타행이체건수      
#define	   x118X                "1180"//타행금액
            
#define	   x122X                "1220"//총입금건수                  
#define	   x123X                "1230"//총입금금액                  
#define	   x124X                "1240"//수표입금건수                
#define	   x125X                "1250"//수표입금금액                
#define	   x126X                "1260"//권종설정정보                
#define	   x132X                "1320"//이전시제합계일자            
#define	   x133X                "1330"//이전시제합계시각            
#define	   x134X                "1340"//현재시제합계일자            
#define	   x135X                "1350"//현재시제합계시각            
#define	   x136X                "1360"//Cassette1 총방출매수        
#define	   x137X                "1370"//Cassette2 총방출매수        
#define	   x138X                "1380"//Cassette3 총방출매수        
#define	   x139X                "1390"//Cassette4 총방출매수        
#define	   x140X                "1400"//Cassette1 총회수매수        
#define	   x141X                "1410"//Cassette2 총회수매수        
#define	   x142X                "1420"//Cassette3 총회수매수        
#define	   x143X                "1430"//Cassette4 총회수매수        
#define	   x145X                "1450"//해외/약관/카드론총거래건수  
#define	   x146X                "1460"//해외/약관/카드론총거래금액  
#define	   x147X                "1470"//현금미수취 총건수           
#define	   x148X                "1480"//현금미수취 총금액           
#define	   x149X                "1490"//출금셔터부 에러 총건수      
#define	   x150X                "1500"//출금셔터부 에러 총금액      
#define	   x151X                "1510"//통신장애 총건수             
#define	   x152X                "1520"//통신장애 총금액             
#define	   x153X                "1530"//출금계수장애 총건수         
#define	   x154X                "1540"//출금계수장애 총금액         
#define	   x155X                "1550"//미완료취소 총건수(삭제요망) 
#define	   x156X                "1560"//미완료취소 총금액(삭제요망) 
#define	   x157X                "1570"//지폐 방출 총 건수           
#define	   x158X                "1580"//지폐 방출 총 금액           
#define	   x159X                "1590"//T-Money비제휴충전 총건수    
#define	   x160X                "1600"//T-Money비제휴충전 총금액    
#define	   x161X                "1610"//기타거래 총건수             
#define	   x162X                "1620"//기타거래 총금액             
#define	   x163X                "1630"//기타취소 총건수             
#define	   x164X                "1640"//기타취소 총금액             
#define	   x165X                "1650"//기기프로그램버전            
#define	   x173X                "1730"//입금통신장애 총건수         
#define	   x174X                "1740"//입금통신장애 총금액         
#define	   x175X                "1750"//입금계수장애 총건수         
#define	   x176X                "1760"//입금계수장애 총금액         
#define	   x177X                "1770"//입금수납장애 총건수         
#define	   x178X                "1780"//입금수납장애 총금액         
#define	   x179X                "1790"//입금미수취 총건수           
#define	   x180X                "1800"//입금미수취 총금액           
#define	   x182X                "1820"//만원권 총방출매수           
#define	   x183X                "1830"//5만원권 총방출매수          
#define	   x184X                "1840"//10만원권 총방출매수         
#define	   x185X                "1850"//만원권 총입금매수           
#define	   x186X                "1860"//5만원권 총입금매수          
#define	   x187X                "1870"//10만원권 총입금매수         
#define	   x188X                "1880"//기타권종 총입금금액         
#define	   x317X                "3170"//5천원권 총방출매수          
#define	   x318X                "3180"//1천원권 총방출매수          
#define	   x319X                "3190"//주화 총방출금액             
#define	   x910x                "9100"//브랜드제휴 구분             
#define	   x911x                "9110"//기기설정정보                
#define	   x912X                "9120"//러쉬앤캐쉬 거래선택건수 집계
#define	   x463X                "4630"//기업제휴 인근점포 안내 
#define	   x914x                "9140"//RPC 설정 정보
#define	   x916x                "9160"//VPN IP

#define	   x337x                "3370"//기기환경점검표             
#define	   x338x                "3380"//조치자업체구분              
#define	   x339X                "3390"//조차자명(or 직원번호)

#define	   x2082                "2082"//Host NG(기관)

////////////////////////////////////////////////////////////////////////////////
//#N0142 전자상품권
#define	   x0132				"0132" //전자상품권 IC DATA
#define	   x0153				"0153" //전자상품권 IC DATA
#define	   x3161				"3161" //전자상품권 일련번호
#define	   x340X				"3400" //전자상품권 대분류대이타
#define	   x341X				"3410" //전자상품권 보내는사람 전화번호
#define	   x342X				"3420" //전자상품권 권종데이터
#define	   x343X				"3430" //전자상품권 유효기간
#define	   x0231				"0231" //전자상품권 판매 취소 금액


////////////////////////////////////////////////////////////////////////////////
//#N0161 이자율 조회
#define	   x168X					"1680" //신용카드 현금서비스 이자율

////////////////////////////////////////////////////////////////////////////////
//#N0176 해외카드
#define	   x346X					"3460" //해외카드 브랜드 구분

#define	   x345X					"3450" // 수수료 포인트 결재

////////////////////////////////////////////////////////////////////////////////
//#N0184 신한카드 한도상향
#define	   x307X					"3070" //신한카드 한도상향

#define	   x040x					"0400" //macadress  #N0164
////////////////////////////////////////////////////////////////////////////////
//#N9999 EMVSETINFO
#define	   x358X					"3580" //1. Terminal Capabilitise(9F33)(3)
#define	   x359X					"3590" //2. Terminal Verification Result(95)(5)
#define	   x360X					"3600" //3. Unpredictable Number(9F37)(4)
#define	   x361X					"3610" //4. Application Cryptogram(9F26)(8)
#define	   x362X					"3620" //5. Application Transaction Counter(9F36)(2)
#define	   x363X					"3630" //6. Application Interchange Profile(82)(2)
#define	   x364X					"3640" //7. Transaction Type(9C)(1)
#define	   x365X					"3650" //8. Terminal Transaction Date(9A)(3)
#define	   x366X					"3660" //9. Transaction Amount(9F02)(6)
#define	   x367X					"3670" //10. Transaction Currency Code(5F2A)(2)
#define	   x368X					"3680" //11. Another Transaction Amount(9F03)(6)
#define	   x369X					"3690" //12. Cryptogram Information Data(9F27)(1)	
#define	   x370X					"3700" //13. PAN Sequence Number(5F34)(1)
#define	   x371X					"3710" //14. TerminalType(9F35)(1)	
#define	   x372X					"3720" //15. Amount in Ref. Currency(9F3A)(4)
#define	   x373X					"3730" //16. Transaction Ref. Currency Code(9F3C) (2)
#define	   x374X					"3740" //17. Data Authentication Code(DAC) ( 9F45 ) (2)
#define	   x375X					"3750" //18. M/CHIP Version(9F09)(1)	
#define	   x376X					"3760" //19. Issuer Application Data(9F10)(Variables) (2) -> 20. Length Indicator[2]	
#define	   x377X					"3770" //20. Derivation Key Index[2]
#define	   x378X					"3780" //21. Cryptogram Version Nr[2]
#define	   x379X					"3790" //22. Card Verification Result[12]
#define	   x380X					"3800" //23. Issuer Discretionary Data[50]			           --> 사용안함 (협의필요)
#define	   x386X					"3860" //24. Terminal Country Code[4]		

#define	   x387X					"3870" //통장증서 발행번호(회차)		

// 폐국 #N0278	
#define	   x390X					"3900" //Rebooting 사유정보	
#define	   x388X					"3880" //통신장애 내용 필드	
#define	   x389X					"3890" //기기모델정보 필드		

//ARPC
#define	   x381X					"3810" //ARPC (16)
#define	   x382X					"3820" //APRCResposeCode(4)
#define	   x383X					"3830" //IssuerScriptLen+IssuerScriptResult 

#define	   x353X					"3530" //하이패스 SAM DATA					//#N0266
#define	   x354X					"3540" //하이패스 거래로그데이터
#define	   x355X					"3550" //하이패스 거래일려번호로그
#define	   x356X					"3560" //하이패스 SAM DATA 로그
#define	   x357X					"3570" //하이패스 ID 서비스 데이터

#define	   x401X					"4010" 
#define	   x4015					"4015" 

#define	   x464X					"4640" //현금부족체크
#define	   x445X					"4450" //출금사유 분석 위한 출금취소사유 (전문사양서 참조)
#define	   x459X					"4590" 
#define	   x470X					"4700" 
#define	   x471X					"4710" 
#define	   x4721					"4721" 
#define	   x4751					"4751" 

#endif
