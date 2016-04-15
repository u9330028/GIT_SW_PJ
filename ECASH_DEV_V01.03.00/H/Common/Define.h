/////////////////////////////////////////////////////////////////////////////
#if !defined(_DEFINE_H_INCLUDED_)
#define _DEFINE_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// 사양정리
/////////////////////////////////////////////////////////////////////////////
// Mini-ATM, C-ATM(Mini-ATM) Compile Option 
// ** Excute File absolutely need when recompiled option excutes  ***********
// ** compile option "T1ATM_MODE" or "T3ATM_MODE"                 ***********
/////////////////////////////////////////////////////////////////////////////
// Version
/////////////////////////////////////////////////////////////////////////////

#define CERTI_REAL_KEY000
//#define CERTI_TEST_KEY000

#ifdef T1ATM_MODE
	#define	VERSION				"V01-03-00"					//  
#else														//  
	#define	VERSION				"V01-03-00"					//  
#endif

	#define	APVERSION			"160331"				

#define	REVISION			"R0"						// Version별 Revision관리필요
#define VERSIONDATE			"20160412"					// 프로그램변경일자


#define ECASH_LOCALPORT_MODE	20015						// 테스트 서버 용 


#define LINE_CLEAR			0
#define LINE_OPEN			'O'
#define LINE_CLOSE			'C'
#define LINE_RESET			'R'
#define LINE_DATA			'D'


/////////////////////////////////////////////////////////////////////////////
// USER Name : 2004.01.29
/////////////////////////////////////////////////////////////////////////////
#define USER_KNB			"KNB"					// 경남은행
#define USER_KMB			"KMB"					// 국민은행
#define USER_IBK			"IBK"					// 기업은행
#define USER_NACF			"NAC"					// 농협
#define USER_DGB			"DGB"					// 대구은행
#define USER_PSB			"PSB"					// 부산은행
#define USER_KFCC			"KFC"					// 새마을금고
#define USER_SHB			"SHB"					// 신한은행
#define USER_WRB			"WRB"					// 우리은행
#define USER_MIC			"MIC"					// 정보통신부(우체국)
#define USER_KFB			"KFB"					// 제일은행
#define USER_BOC			"BOC"					// 제주은행
#define USER_CHB			"CHB"					// 조흥은행
#define USER_HNB			"HNB"					// 하나은행
#define USER_JBB			"JBB"					// 전북은행
#define USER_TYS			"TYS"					// 동양종금
#define USER_NIC			"NIC"					// 한국전자금융

#define USERNAME			USER_TYS				// 적용은행

//#0111 (Image Path)
#define M_TYPE_T1A			"T1A"					// T1ATM
#define M_TYPE_T2A			"T2A"					// T2ATM
#define M_TYPE_U81			"U81"					// U8100
#define M_TYPE_M28			"M28"					// M2800
#define M_TYPE_U3K			"U3K"					// U3100_K - (BRM21)
#define M_TYPE_U31			"U31"					// U3100   - (OKI)
#define M_TYPE_VAN			"VCD"					// VANCD
#define M_TYPE_000			"000"					// DEFAULT


/////////////////////////////////////////////////////////////////////////////
// TRACE PRE-DEFINE REFERENCE
/////////////////////////////////////////////////////////////////////////////
#define		MAC_TRACE_FREE_FILE(FileName)	TRACE_FREE_MODE, FileName, __FILE__, __LINE__
#define		MAC_TRACE_CODE_FILE(FileName)	TRACE_CODE_MODE, FileName, __FILE__, __LINE__
#define		MAC_TRACE_DUMP_FILE(FileName)	TRACE_DUMP_MODE, FileName, __FILE__, __LINE__

//////////////////////////////////////////////////////////////////////
// NMS SEND COMMAND
//////////////////////////////////////////////////////////////////////
#define NMS_NORMAL			0
#define NMS_ERROR			1
#define NMS_STATUS			2
#define NMS_TMK				3
#define NMS_TPK				4
#define NMS_SNOD			5
#define NMS_CANCEL			6

#define DS_INFO				0
#define CAP_INFO			1

/////////////////////////////////////////////////////////////////////////////
// Operation Mode : ATM ( Somes CD )
/////////////////////////////////////////////////////////////////////////////

#define HOST_ONLINE_MODE	1						// HOST ONLINE MODE
#define HOST_LOCAL_MODE		0						// HOST LOCAL MODE

#define BRM_RECYCLE_MODE	1						// 환류식모드값
#define BRM_UNCYCLE_MODE	0						// 비환류모드값(사용불가)

#define BRM_REALCASH_MODE	254						// 진권모드값(UCM포함)
#define BRM_TESTCASH_MODE	255						// 위폐모드값(UCM포함)

#define BRM_MAXCASHOUTCNT	100						// 현금최대방출매수
#define BRM_MAXCHECKOUTCNT	10						// 수표최대방출매수
#define BRM_MAXCASHINCNT	100						// 현금최대입금매수
#define BRM_MAXCASHINCNT_150 150					// 광주은행 브랜드 현금최대입금매수 #N0181
#define BRM_MAXMUCASHINCNT	100						// 무매체현금최대입금매수
#define BRM_MAXCASHINAMT	500						// 최대입금금액 (500만원)
#define BRM_MAXCASHINAMT_750 750					// 광주 브랜드 최대입금금액 (750 만원) //#N0181
#define BRM_MAXCASHINAMT_150 150					// 광주 브랜드 최대입금금액 (150 만원) //#N0226
#define UCM_MAXCHECKINCNT	10						// 수표최대입금매수
#define BRM_MAXCHECKOUTCNT_100CHECK	6               // 수표최대방출매수(백만원수표지원시)

#define BRM_FIXCASHCNT		30						// 현금10매보정(H/W)
#define BRM_FIXCHECKCNT		0						// 수표0매보정(H/W)

													// REJECT매수한계지(공검지장애관련) : 2002.01.08
#define BRM_EMPTYCASHCNT	(BRM_MAXCASHOUTCNT + BRM_FIXCASHCNT)		
													// 현금요보충값:20매보정(H/W)
#define BRM_EMPTYCHECKCNT	(1 + BRM_FIXCHECKCNT)	// 수표요보충값:1매보정(H/W)
#define BRM_NEARCASHCNT		300						// 현금잔량소값 2004.05.31 :  400 => 300
#define BRM_NEARCHECKCNT	20						// 수표잔량소값

#define BRM_MONEY_MAXOUT	100						// 최대방출금액(만원단위)
#define BRM_MONEY_MAXOUTCNT	100						// 최대방출매수
#define BRM_MONEY_MAXOUT_70	70						// 최대방출금액(만원단위)
#define BRM_MONEY_MAXOUT_50	50						// 최대방출금액(만원단위)
#define BRM_MONEY_MAXOUT_40	40						// 최대방출금액(만원단위)
#define BRM_MONEY_MAXOUT_30	30						// 최대방출금액(만원단위)

#define BRM_CASHINRETRY		3						// 입금재투입가능횟수
#define BRM_UNKNOWNRETRY	1						// 입금지폐감별가능횟수
#define UCM_UNKNOWNRETRY	5						// 입금수표감별가능횟수

#define BRM_SELECTCHECKCNT	20						// 수표선택가능매수(수표만출금시)

#define BRM_AREFILLCONDCNT	210						// 자동보충조건매수
#define BRM_AREFILLCNT		350						// 자동보충매수
#define BRM_ATAKEUPCONDCNT	500						// 자동회수조건매수
#define BRM_ATAKEUPCNT		500						// 자동회수매수
#define BRM_MAXRJB1CNT		150						// 최대RJB1매수
#define BRM_MAXRJB2CNT		300						// 최대RJB2매수(히다치검수 : 2002.04.19)
#define BRM_MANRJB2CNT		250						// 만원RJB2매수
#define	BRM_MAXRBCNT		2500					// 최대RB매수(BOX당)
#define	BRM_MAXRB23CNT		(BRM_MAXRBCNT * 2)		// 최대RB2,RB3매수
#define	BRM_MAXRB123CNT		(BRM_MAXRBCNT * 3)		// 최대RB1,RB2,RB3매수

#define BRM_MAXRJB2DEPCNT	150						// RJB2만원권만입금가능매수		: 2003.12.01

//////////////////////////////////////////////////////////////////////////
// OKI-BRM											// T2ATM 2006.5.24 yscho
//////////////////////////////////////////////////////////////////////////
#define OKI_MAXFRJBCNT		200						// 최대FRJB매수
#define OKI_MAXRJB1CNT		280						// 최대RJB1매수
#define OKI_MAXRJB2CNT		280						// 최대RJB2매수
#define OKI_MINRJB1CNT		205						// 한도RJB1매수
#define OKI_MINRJB2CNT		205						// 한도RJB2매수
#define	OKI_MAXRBCNT		2700					// 최대RB별매수
#define	OKI_REVISECASHCNT	30						// 현금요보충매수조정
#define OKI_MAXCASHOUTCNT   100						// 출금최대매수	
#define OKI_MAXCASHINCNT    100						// 입금최대매수	
#define OKI_50T_REVISECASHCNT 5						// 5만원 요보충매수조정	
#define OKI_50T_MAXCASHOUTCNT   20					// 5만원 출금최대매수
#define OKI_50T_AREFILLCONDCNT	60					// 5만원 부족알림매수
#define	OKI_50T_MAXRBCNT	2000					// 5만원 RB매수			

#define HBRM_MAXRJB1CNT		350						// 최대RJB1매수		
#define HBRM_MINRJB1CNT		300						// 한도RJB1매수		
#define HBRM_MAXRJB2CNT		350						// 최대RJB2매수		
#define HBRM_MINRJB2CNT		300						// 한도RJB2매수		

/////////////////////////////////////////////////////////////////////////////
// HOST 한도금액									V02-08-01-R1
/////////////////////////////////////////////////////////////////////////////
#define HOST_MAXWITHAMT		100						// 10만원권 출금시 MAX

/////////////////////////////////////////////////////////////////////////////
// Operation Mode : CD ONLY
/////////////////////////////////////////////////////////////////////////////
#define CDU_NEARCASHCNT		100						// NEAR CASH COUNT
#define CDU_NEARCASHCNT2	0						// NEAR CASH COUNT

#define CDU_FULLREJECTCNT	200						// FULL REJECT COUNT
#define CDU_FULLREJECTCNT2	100						// FULL REJECT COUNT

#define CDU_MAXCASHOUTCNT	100						// 현금최대방출매수
#define CDU_MAXCHECKOUTCNT	10						// 수표최대방출매수

#define CDU_MONEY_MAXOUT	100						// 최대방출금액(만원단위)

/////////////////////////////////////////////////////////////////////////////
// 수표권종정보 define								V02-08-01-R1
/////////////////////////////////////////////////////////////////////////////
#define	CHECKKIND_HUNDRED_THOUSAND				13	// 10 만원권모드
#define	CHECKKIND_THREEHUNDRED_THOUSAND			14	// 30 만원권모드
#define	CHECKKIND_FIVEHUNDRED_THOUSAND			15	// 50 만원권모드
#define	CHECKKIND_MILLION						16	// 100만원권모드
#define	CHECKKIND_UNKNOWN						0	// 미확인모드

#define	CHECKMONEY_HUNDRED_THOUSAND				"10"// 10 만원권모드
#define	CHECKMONEY_THREEHUNDRED_THOUSAND		"30"// 30 만원권모드
#define	CHECKMONEY_FIVEHUNDRED_THOUSAND			"50"// 50 만원권모드
#define	CHECKMONEY_MILLION						"100"// 100만원권모드
#define	CHECKMONEY_UNKNOWN						"0"	// 미확인모드

#define	CHECK_HUNDRED_THOUSAND					10	// 10 만원권모드
#define	CHECK_THREEHUNDRED_THOUSAND				30	// 30 만원권모드
#define	CHECK_FIVEHUNDRED_THOUSAND				50	// 50 만원권모드
#define	CHECK_MILLION							100	// 100만원권모드
#define	CHECK_UNKNOWN							0	// 미확인모드

#define CASH_TEN_THOUSAND						1	// V06-01-01-#01
#define CASH_FIFTY_THOUSAND						5	// V06-01-01-#01
#define CASH_PRIORITY_FIFTY						5	// V06-01-01-#01
#define CASH_PRIORITY_1000TH					10	// V06-01-01-#01

/////////////////////////////////////////////////////////////////////////////
// POWER
/////////////////////////////////////////////////////////////////////////////
#define POWER_OFF_TIME		45						// POWER OFF TIME
#define POWER_ON_TIME		10						// POWER ON TIME

#define POWER_OFF_TIME_TTW	120						// POWER OFF TIME
#define STATUS_OFF_TIME     1004					
#define STATUS_ON_TIME      1005
#define STATUS_FORCE_SEND   1006

/////////////////////////////////////////////////////////////////////////////
// HOST PROC TIME
/////////////////////////////////////////////////////////////////////////////
#define LINE_GETKEY_TIME	25						// LINE OPEN/CLOSE TIME
#define LINE_OPCL_TIME		30						// LINE OPEN/CLOSE TIME
#define LINE_RETRY_TIME		300						// HOST OPEN RETRY TIME
#define OPEN_RETRY_TIME		30						// HOST OPEN RETRY TIME
//#define LINE_SEND_TIME		180						// HOST SEND TIME				
#define LINE_BIDSEND_TIME	30						// HOST SEND TIME(BID)
//#define LINE_RECV_TIME		180						// HOST RECV TIME				
#define POLL_TIME			180						// V01-00-00-#304
#define LINE_SEND_5_TIME	5						// HOST SEND TIME(BID)

#define LINE_SEND_TIME		90						// HOST SEND TIME				
#define LINE_RECV_TIME		90						// HOST SEND TIME				
#define LINE_2190_TIME		60						// HOST SEND TIME(BID)


/////////////////////////////////////////////////////////////////////////////
//	SP Reg Define
/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_ATM			_T("SOFTWARE\\ATM")
#define	_REGKEY_ATM_APP		_T("SOFTWARE\\ATM\\APP")
#define	_REGKEY_OKI			_T("SOFTWARE\\OKI")					// T2ATM 2006.5.24 yscho
#define	_REGKEY_DEVERROR	_T("SOFTWARE\\ATM\\ERRORCODE")

#define _REGKEY_SUPERVISOR	_T("SOFTWARE\\ATM\\SUPERVISOR")		// U8100-AP변경 : 기종 정보(차세대 OM)

#define	_REGKEY_BRMSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\BRM")	// 20030719_1.0_1053 : 장치정보 경로
#define	_REGKEY_MCUSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\IDC")	// 20030719_1.0_1053 : 장치정보 경로
#define	_REGKEY_JPRSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\JPR")	// 20030719_1.0_1053 : 장치정보 경로
#define	_REGKEY_PBMSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\PBM")	// 20030719_1.0_1053 : 장치정보 경로
#define	_REGKEY_SIUSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\SIU")	// 20030719_1.0_1053 : 장치정보 경로
#define	_REGKEY_SPRSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\SPR")	// 20030719_1.0_1053 : 장치정보 경로
#define	_REGKEY_UCMSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\UCM")	// 20030719_1.0_1053 : 장치정보 경로
#define	_REGKEY_UPSSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\UPS")	// 20030719_1.0_1053 : 장치정보 경로

/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_CDUSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\CDM")
#define	_REGKEY_VDMSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\NHVDM")

#define	_REGKEY_DEVINFO		_T("SOFTWARE\\ATM\\DEVINFO")		// 2006.09.08 SHU타입 지원
#define	_REGKEY_UPSINFO		_T("SOFTWARE\\ATM\\DEVINFO\\UPS")	// 20030729_1.0_1055 : UPS Power Control
#define	_REGKEY_A4PINFO		_T("SOFTWARE\\ATM\\DEVINFO\\SAU")
#define	_REGKEY_MCUINFO		_T("SOFTWARE\\ATM\\DEVINFO\\IDC")	// 20030719_1.0_1053 : 금융IC Version V070300 3.
#define	_REGKEY_BRMINFO		_T("SOFTWARE\\ATM\\DEVINFO\\BRM")	// T2ATM 2006.5.24 yscho (BRM 운영정보 INI -> Registry)
#define	_REGKEY_CMRINFO		_T("SOFTWARE\\ATM\\DEVINFO\\CAM")	// 2006.09.08 카메라 타입지원
#define	_REGKEY_EMBOSS		_T("SOFTWARE\\ATM\\EMBOSS")
#define _REGKEY_CAMINFO		_T("SOFTWARE\\ATM\\DEVINFO\\CAM")	// V06-02-01-#05:Camera Preview 카메라 디바이스 정보
#define	_REGKEY_SPRINFO		_T("SOFTWARE\\ATM\\DEVINFO\\SPR")	// V07-05-01-#02:동시방출 명령시 버전체크 위해

#define	_REGKEY_SPVERSION	_T("SOFTWARE\\ATM\\SP_Version")		// 2004.03.03 SP VERSION
#define	_REGKEY_EPVERSION	_T("SOFTWARE\\ATM\\EP_Version")		// 2004.03.03 EP VERSION
#define _REGKEY_MWI _T("SOFTWARE\\ATM\\MWI") 

#define	_REGKEY_ATMRESTART	_T("SOFTWARE\\ATM\\APP\\AtmRestart")
#define	_REGKEY_ATMEACHNET	_T("SOFTWARE\\ATM\\APP\\AtmEachNet")
#define	_REGKEY_ATM_REJMAT	_T("SOFTWARE\\ATM\\REJECTMATERIAL")
#define _REGKEY_REMOTECTRL	_T("SOFTWARE\\ATM\\ReMoteCtrl")
#define	DEVNM_ICVERSION		"IC_Version"						// 금용IC Board Version V070300 3.
#define	_REGKEY_DEFSETTING	_T("SOFTWARE\\ATM\\APP\\DEFAULTSETTING")
/////////////////////////////////////////////////////////////////////////////
//	Device Reg Define
/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_DEVICE					_T("SOFTWARE\\ATM\\APP\\DEVICE")
#define	_REGKEY_DEVICE_TOTALSETCASHCST1	_T("TOTALSETCASHCST1")	
#define	_REGKEY_DEVICE_TOTALSETCASHCST2	_T("TOTALSETCASHCST2")
#define	_REGKEY_DEVICE_TOTALSETCASHCST3	_T("TOTALSETCASHCST3")
#define	_REGKEY_DEVICE_TOTALSETCASHCST4	_T("TOTALSETCASHCST4")
#define	_REGKEY_DEVICE_DISPCASHCST1		_T("DISPCASHCST1")
#define	_REGKEY_DEVICE_DISPCASHCST2		_T("DISPCASHCST2")
#define	_REGKEY_DEVICE_DISPCASHCST3		_T("DISPCASHCST3")
#define	_REGKEY_DEVICE_DISPCASHCST4		_T("DISPCASHCST4")
#define	_REGKEY_DEVICE_REBOOT			_T("REBOOT")

#define _SYSTEMCDINI		_T("C:\\SYSTEMCD.INI")				// System CD 버전

#define OS_WIN7				1									//WIN7 지원 #N0228
/////////////////////////////////////////////////////////////////////////////
//	SP Profile Define
/////////////////////////////////////////////////////////////////////////////
#define _SP_DEVICE_INI		"C:\\T1ATM\\INI\\DEVICE.INI"
#define _SP_CDMINFO_INI		"C:\\T1ATM\\INI\\CDMINFO.INI"
#define _SP_CSTINFO_INI		"C:\\T1ATM\\INI\\CSTINFO.INI"
#define _SP_DISPINFO_INI	"C:\\T1ATM\\INI\\DISPINFO.INI"
#define _LCDINFO_INI		"D:\\INI\\LCDINFO.INI"			// 광고LCD위젯처리, 변경
/////////////////////////////////////////////////////////////////////////////
//	SP MINI_ATM BRM (CSTINFO: 0710)
/////////////////////////////////////////////////////////////////////////////
#define _SP_BRM_CSTINFO_INI			_T("C:\\T1ATM\\INI\\BRM_CSTINFO.INI")
#define _CH_SSLVPN_INI				_T("C:\\SSLVPN.INI")
/////////////////////////////////////////////////////////////////////////////
//	Line Binary File Define
/////////////////////////////////////////////////////////////////////////////
#define _LCP_BIN			"C:\\DCPLCPBIN"

/////////////////////////////////////////////////////////////////////////////
//	Scr Dir Define
/////////////////////////////////////////////////////////////////////////////
//#define _SCR_DIR			"./SWF/"
//#define _SCR_DIR			"\\T1ATM\\APP\\SWF\\"
#define _SCR_DIR			"C:\\T1ATM\\APP\\SWF\\"
/////////////////////////////////////////////////////////////////////////////
//	CFMONITOR Define
/////////////////////////////////////////////////////////////////////////////
#define	_PNAME_AVIPLAYER	"AviPlayer"
#define	_PNAME_SWFPLAYER	"SwfPlayer"
#define _PNAME_CFMONITOR	_PNAME_AVIPLAYER
#define _PNAME_CFMONITOR_U8100	_PNAME_SWFPLAYER			
#define	_FIND_CFMON			(::FindWindow(NULL, "SwfPlayer") != NULL) ? ::FindWindow(NULL, "SwfPlayer")	: ::FindWindow(NULL, "AviPlayer")

#define	_DEFAULT_CFMONITOR			_T("C:\\T1ATM\\CFMONITOR\\AVIPLAYER.EXE")
#define	_DEFAULT_CFMONITOR_U8100	_T("C:\\T1ATM\\CFMONITOR\\SWFPLAYER.EXE")	// U8100-AP변경


#define _TRANS_C_INI  				"C:\\T1ATM\\INI\\TRANS.INI" //#0092
#define _TRANS_POWER_FAIL_INI  		"C:\\T1ATM\\INI\\POWERFAIL.INI" //#0108
#define _TRANS_POWER_DATA_INI  		"C:\\T1ATM\\INI\\POWERDATA.INI" //#0092
#define _TRANS_NEARBRANCH_INI  		"C:\\T1ATM\\INI\\NEARBRANCH.INI"  //#N0172

/////////////////////////////////////////////////////////////////////////////
//	Profile Define
/////////////////////////////////////////////////////////////////////////////
#define _INI				"D:\\INI\\"
#define _DEVICE_INI			"D:\\INI\\DEVICE.INI"
#define _NETWORK_INI 		"D:\\INI\\NETWORK.INI"
#define _TRANS_INI  		"D:\\INI\\TRANS.INI"
#define _ERRSTACK_INI		"D:\\INI\\ERRSTACK.INI"
#define _ERRDETAILHOST_INI	"D:\\INI\\ERRDETAILHOST.INI"
#define _ERRHOST_INI 		"D:\\INI\\ERRHOST.INI"
#define _ATMTOTAL_INI		"D:\\INI\\ATMTOTAL.INI"
#define _AJMS_INI			"D:\\INI\\AJMS.INI"
#define _ATMS_INI			"C:\\ATMS\\ATMS.INI"				// 2007.06.01
#define _ATMS_FTP_INI		"D:\\INI\\ATMFTPDOWN.INI"
#define	_MAINAD_INI			"D:\\INI\\MAINAD.ini"
#define	_MOBILEGAME_INI		"D:\\INI\\mobilegame.ini"


//////////////////////////////////////////////////////////////////////////
// VFD Dir Define									// T2ATM 2006.5.24 yscho
//////////////////////////////////////////////////////////////////////////
#define _VFD_INI			"D:\\INI\\VFD.INI"
#define _VFD_TMP_INI		"D:\\INI\\VFD_TMP.INI"
#define VFD_DISPLAY			0
#define VFD_SCROLL			2
#define VFD_BLINK			1

//////////////////////////////////////////////////////////////////////////
// RFD Command Define : for T2ATM Demo 2006.04.20
#define	RFD_READ			0xD0
#define	RFD_PSWD			0xD1
#define	RFD_CNCL			0xD2
#define	RFD_TIME			0xD3
#define	RFD_INIT			0xD4

///////////////////////////////////////////////////////////////////////////////
// GRO 정보 
// 디바이스 : GRO
///////////////////////////////////////////////////////////////////////////////
#define	GRO_INBOX			"INBOX"
#define	GRO_INJAW			"INJAW"
#define	GRO_INJAWINBOX		"INJAWINBOX"
#define	GRO_SCANED			"SCANED"
#define	GRO_INJAWSCANED		"INJAWSCANED"

/////////////////////////////////////////////////////////////////////////////
//	Ejr Dir Define
/////////////////////////////////////////////////////////////////////////////
#define _EJR_D				"D:\\"
#define _EJR_CAP			"D:\\CAPTURE000"
#define _EJR_DIR			"D:\\ECASH"
#define _EJR_TXT			"D:\\ECASH\\JNL"
#define _EJR_IMG			"D:\\ECASH\\IMG"
#define _EJR_AUTHLOCK       "D:\\ECASH\\AUTHLOCK"
#define _EJR_IMPACTED       "D:\\ECASH\\IMPACTED"

#define _EJR_DOOR			"DOOR"
#define _EJR_TITLE			"TITLE"

/////////////////////////////////////////////////////////////////////////////
//	VOD Define //U8100
// U8100 AP 변경내역 #02 - 카메라 동영상 지원
/////////////////////////////////////////////////////////////////////////////
#define VOD_START			1		// U8100-VOD
#define VOD_END				0		// U8100-VOD
#define VOD_FACE			1
#define VOD_HAND			2
#define VOD_ROOM			3
#define VOD_FACE_FILE		"D:\\VOD\\Face"
#define VOD_HAND_FILE		"D:\\VOD\\Hand"
#define VOD_ROOM_FILE		"D:\\VOD\\Room"


//////////////////////////////////////////////////////////////////////////
// 2007.06.12 for ATMS
#define _ATMS_JNL			"C:\\ATMS\\JNL"
#define _ATMS_IMG_FACE		"FACE"
#define _ATMS_IMG_EMIT		"EMIT"
#define _ATMS_IMG_CARD		"CARD"

/////////////////////////////////////////////////////////////////////////////
//	Trace Dir Define
/////////////////////////////////////////////////////////////////////////////
#define _LOG_DIR			"D:\\LOG"
#define _TRACE_DIR			"D:\\TRACE"

#define _NHTM_DIR			"C:\\NHTM\\"			
#define _HBRM_DIR			"D:\\HBRM\\TM\\"		
			
#define _NHNW30_TESTPAGE_DIR			"C:\\Program Files\\Nextware\\TestPage\\30\\Html\\cashdispenser"		
#define _NHNW20_TESTPAGE_DIR			"C:\\Program Files\\Nextware\\TestPage\\20\\Html\\cashdispenser"		

/////////////////////////////////////////////////////////////////////////////
//	AP Sound Dir Define
/////////////////////////////////////////////////////////////////////////////
#define _SOUND_DIR			"C:\\T1ATM\\APP\\SWF\\SOUND"
#define	_SOUND_DDING		"C:\\T1ATM\\APP\\SWF\\SOUND\\w200.wav"
#define	_SOUND_OM_REJECT	"k-o001.wav"						// 2007.11.27 불명권 안내문구 개선

#define	_SOUND_PROXIMITY	"K-7922.wav"		
#define	_SOUND_NONE			"NoSound.wav"		
/////////////////////////////////////////////////////////////////////////////
//	AP File Define
/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_LOADER		_T("SOFTWARE\\ATM\\APP\\BROWSER\\LOADER")	// V03-01-01
#define	_DEFAULT_REGDLL		_T("C:\\T1ATM\\APP\\MAIN\\REGDLL.BAT")
#define	_DEFAULT_STARTPAGE	_T("C:\\T1ATM\\APP\\T1ATM.HTM")
#define	_DEFAULT_REARAP		_T("C:\\T1ATM\\SUPERVISOR\\REARAP.EXE")
#define	_DEFAULT_CDREARAP	_T("C:\\T1ATM\\SUPERVISOR\\CDREARAP.EXE")
#define	_DEFAULT_OMDEMON	_T("C:\\T1ATM\\APP\\MAIN\\OMDEMON.EXE")
#define _DEFAULT_APP		_T("C:\\T1ATM\\APP\\MAIN")

#define	_PNAME_REARAP		_T("[Hyosung T1Atm] REARAP")
#define	_PNAME_CDREARAP		_T("[Hyosung T1Scd] SCDREARAP")
#define	_PNAME_OMDEMON		_T("[Hyosung T1Atm] OMDEMON")

#define	_GOMA_REARAP		_T("REARAP.EXE")
#define	_GOMA_CDREARAP		_T("CDREARAP.EXE")

/////////////////////////////////////////////////////////////////////////////
// ATM Mode Define
/////////////////////////////////////////////////////////////////////////////
#define ATM_INIT			0						// 기기초기화	모드
#define ATM_ERROR			1						// 장애발생		모드
#define ATM_CLERK			2						// 계원			모드
#define ATM_READY			3						// 준비			모드
#define ATM_CUSTOM			4						// 고객대기중	모드 
#define ATM_TRAN			5						// 고객거래중	모드
#define ATM_DOWN			6						// DOWN		MODE

/////////////////////////////////////////////////////////////////////////////
// Host Mode Define
/////////////////////////////////////////////////////////////////////////////
#define HOST_OFFLINE 		0						// 교신불가
#define HOST_WAITLINK		1						// 교신대기
#define HOST_LINK			2						// 교신가능
#define HOST_WAITREADY		3						// 준비완대기
#define HOST_READY			4						// 준비완
#define HOST_WAITONLINE		5						// 개국대기
#define HOST_ONLINE			6						// 개국완료
#define HOST_INIT			99						// 라인준비

#define HOST_MAX_TRAN		25						// Bitmap지원 거래수

/////////////////////////////////////////////////////////////////////////////
// Tran Mode Define
/////////////////////////////////////////////////////////////////////////////
#define TRAN_IDLE 			0						// 업무대기
#define TRAN_READY			10						// 준비완처리	(거래처리)
#define TRAN_OPEN			11						// 개국처리		(거래처리)
#define TRAN_STATUS			12						// 상태/장애처리(거래처리)
#define TRAN_TRAN			20						// 거래처리		(거래처리)
#define TRAN_SET			30						// 정위치처리					(내부처리)
#define TRAN_ERROR			31						// 장애처리						(내부처리)
#define TRAN_TOTAL			32						// 마감처리		(거래처리)
#define TRAN_SUBTOTAL		33						// 마감조회		(거래처리)
#define TRAN_RESET			34						// RESET처리					(내부처리)
#define TRAN_REFILL			35						// BRM보충처리					(내부처리)
#define TRAN_TAKEUP			36						// BRM회수처리					(내부처리)
#define	TRAN_CHECK_REG		38						// 수표등록		(거래처리)
#define	TRAN_CHECK_UNREG	39						// 수표해제		(거래처리)
#define	TRAN_CHECK_ALIVE	40						// 라인점검		(내부처리)
#define TRAN_CASH_REG		41						// 현금등록		(거래처리)
#define TRAN_CASH_UNREG		42						// 현금해제		(거래처리)
#define TRAN_MSFINGER_CONFIRM	43					// 사원+본인 	(거래처리)

/////////////////////////////////////////////////////////////////////////////
// 거래처리종류
/////////////////////////////////////////////////////////////////////////////
#define TRAN_NULL			0						// 거래처리종류초기화
#define TRAN_WITH			1						// 지급
#define TRAN_PB				2						// 통장정리
#define TRAN_TRANS			3						// 이체
#define TRAN_DEP			4						// 입금
#define TRAN_INQ			5						// 조회
#define TRAN_IC				6						// K-CASH
#define TRAN_RF				7						// RF거래
#define TRAN_CLERK			8						// 계원거래
#define TRAN_NOMEDIA		9						// 무매체
#define TRAN_TICKET			10						// 티켓
#define TRAN_UTILYBILLS		11						// 공과금
#define TRAN_CARDLOANS		12						// 카드론
#define TRAN_POINTS			13						// 포인트
#define TRAN_CREDITINQS		14						// 신용조회
#define TRAN_INTERAD		16						// 인터렉티브 광고
#define TRAN_MOBILEGAME		17						// 모바일게임 다운로드
#define TRAN_LOCALTAX		18						// 지방세 간편납부
#define TRAN_E_TICKET		20						// 전자상품권  #N0142

#define TRAN_FOREGIN		21						// 해외카드  #N0171
#define TRAN_FOREGIN_INQ	22						// 해외카드 조회 #N0171
#define	TRAN_EASYONE		23						// 외환 이지원 #N0174

#define TRAN_HIPASS			24						// 하이패스 충전 #N0266

#define TRAN_NOBANK_SERVICE			1						// 은행
#define TRAN_NOCREDIT_SERVICE		2						// 신용
#define TRAN_BOTH_SERVICE			3						// 은행 + 신용

#define TRAN_INQFEE_USE 			1						// 수수료공지 Flag

/////////////////////////////////////////////////////////////////////////////
// IC CardType Definitions. 2004.02.11
/////////////////////////////////////////////////////////////////////////////
#define	FIC_NONE			0						// FIC카드無
#define	FIC_CLOSE			1						// 폐쇄형
#define FIC_NATIVE			2						// NATIVE(Semi-Open)
#define FIC_OPEN			3						// 개방형

/////////////////////////////////////////////////////////////////////////////
// IC Work Definitions. 2004.01.31
/////////////////////////////////////////////////////////////////////////////
#define TRAN_IC_FINANCEIC	1						// 금융 IC 업무
#define TRAN_IC_KCASH		2						// K-CASH 업무
#define TRAN_IC_MYBI		3						// MYBI 업무
#define TRAN_IC_MONDEX1		4						// Mondex 업무
#define TRAN_IC_MONDEX2		5						// Mondex 업무
#define TRAN_IC_VCASH		6						// V-CASH 업무
#define TRAN_IC_AUTHEN		7						// 인증서 업무
#define TRAN_IC_ELECPB		8						// 전자통장 업무		// LDC_20040204_A
#define TRAN_IC_HIPASS		10						// 하이패스 업무		Hi-Pass접촉식충전개발 #N0266

/////////////////////////////////////////////////////////////////////////////
// 금융IC 거래 Process 관리
/////////////////////////////////////////////////////////////////////////////
#define FINANCEIC_PROC_SELECTAPPLICATION	0x0001	// 금융 공동망 Select Application
#define FINANCEIC_PROC_READBASICINFO		0x0002	// 금융 공동망 기본정보 조회
#define FINANCEIC_PROC_READACCOUNTINFO		0x0003	// 금융 공동망 계좌정보 조회
#define FINANCEIC_PROC_READDUALINFO			0x0004	// 금융 공동망 기본정보 + 계좌정보 조회
#define FINANCEIC_PROC_GETDATACSN			0x0005	// 금융 공동망 Get Data CSN. 한수웅 수정코드 2004.09.16
#define FINANCEIC_PROC_GETENCIPHER			0x0006	// 금융 공동망 Get Encipher
#define FINANCEIC_PROC_GETENCDATA1			0x0007	// 금융 공동망 Get ENCData 1
#define FINANCEIC_PROC_GETCHALLENGE			0x0008	// 금융 공동망 Get Challenge
#define FINANCEIC_PROC_GETENCDATA2			0x0009	// 금융 공동망 Get ENCData 2

#define FINANCEIC_PROC_CHECKPIN				0x0011	// 전자화폐 Verify PIN
#define FINANCEIC_PROC_VERIFYTRANBIT		0x0012	// 전자화폐 Verify TRAN_BIT
#define FINANCEIC_PROC_READPAN				0x0013	// 전자화폐 Read PAN File
#define FINANCEIC_PROC_READBAMOUNT			0x0014	// 전자화폐 Read JAN File
#define FINANCEIC_PROC_CHANGEPIN			0x0015	// 전자화폐 Change PIN
#define FINANCEIC_PROC_READTRAFFICINFO		0x0016	// 전자화폐 소지자정보
#define FINANCEIC_PROC_PREUNLOADRCV			0x0017	// 전자화폐 환불복구전처리
#define FINANCEIC_PROC_GETICTYPE			0x0018	// 금융IC/전자화폐 Get IC Chip Type 2004.11.03

#define FINANCEIC_PROC_EPBREADACCOUNTINFO	0x0019	// e-모든통장 계좌정보 조회(저축성/여신)	V05-00-02-#01 e-모든통장계좌정보1
#define FINANCEIC_PROC_EPBREADACCOUNTINFO2	0x001A	// e-모든통장 입금계좌정보파일			V05-00-02-#01 e-모든통장계좌정보2

/////////////////////////////////////////////////////////////////////////////
// EMV 거래 Process 관리										V03-00-26-#01
/////////////////////////////////////////////////////////////////////////////
#define EMV_PROC_CALISTSET					0x01	// CA Key Set 저장(인증용)
#define EMV_PROC_TERMINALLISTSET			0x02	// 단말기 지원 AID List 저장
#define EMV_PROC_TERMINALCONSTSET			0x03	// 단말기 지원 기본 데이터 목록 저장
#define EMV_PROC_SELECTPSE					0x04	// EMV 거래 가능 여부를 PSE를 통해 약식 확인
#define EMV_PROC_REQCANDIDATELIST			0x05	// 카드로부터 EMV 거래가 가능한 후보 목록을 요청
#define EMV_PROC_SELECTAPPLICATION			0x06	// 후보 목록 중 선택된 Application으로 거래 시작
#define EMV_PROC_READAPPLICATION			0x07	// 거래를 위해 Application의 정보를 수집
#define EMV_PROC_OFFLINEDATAAUTHEN			0x08	// 오프라인 인증 요구
#define EMV_PROC_ONLINE						0x09	// 온라인 승인 요청에 대한 호스트 응답을 카드로 전달
#define EMV_PROC_COMPLET					0x0A	// 카드로 거래 종료 요청
#define EMV_PROC_AFTERSCRIPT				0x0B	// 거래를 종료하기 위해 스크립트 처리를 요청
#define EMV_PROC_GETDOLDATA					0x0C	// AP가 거래 중 필요한 데이터 객체를 EMV Kernel에 요청
#define EMV_PROC_RESULTDATA					0x0D	// 거래 종료 후 화면으로 보여줄 EMV 거래 결과 변수들을 요청하는함수(인증용)

/////////////////////////////////////////////////////////////////////////////
// RFD 거래 Process 관리
/////////////////////////////////////////////////////////////////////////////
#define RFD_PROC_BASICINFO					0x01	// RF기본정보 구하기
#define RFD_PROC_REQCHARGE					0x02	// RF충전
#define	RFD_PROC_DRAWBACK					0x03	// RF환불
#define	RFD_PROC_UNKNOWN					0x04	// 기타명령

/////////////////////////////////////////////////////////////////////////////
// HiPass 거래 Process 관리		#N0266
/////////////////////////////////////////////////////////////////////////////
#define HPS_PROC_SELECTPSE					0x01
#define HPS_PROC_SELECTAPPLICATION			0x02
#define HPS_PROC_READBALANCE				0x03
#define HPS_PROC_HIPASSPRELOAD				0x04
#define HPS_PROC_HIPASSLOAD					0x05
#define	HPS_PROC_HIPASSRMVTRANBIT			0x06
#define	HPS_PROC_HIPASSLLOG					0x07
#define	HPS_PROC_HIPASSREADPAN				0x08
#define	HPS_PROC_HIPASSCOMPLETE				0x09
// ──
/////////////////////////////////////////////////////////////////////////////
// Timer관련
/////////////////////////////////////////////////////////////////////////////
#define	TIMER_mSEC1000		1000					// Timer모드

/////////////////////////////////////////////////////////////////////////////
// Buffer Size
/////////////////////////////////////////////////////////////////////////////
#define	BASEBUFFSIZE		32768
#define	NETBUFFSIZE			32768

#define	BSZ_USERHEADER		9
#define	BSZ_IMSHEADER		6

/////////////////////////////////////////////////////////////////////////////
// Device Check Maximun Time (365Sec)
/////////////////////////////////////////////////////////////////////////////
#define MMAX_DEVRSP_TIME	1800					// 장치응답최대
#define MAX_DEVRSP_TIME		365						// 장치응답최대
#define MID_DEVRSP_TIME		125						// 장치응답중간(수취Time보다는커야됨 : 2003.10.29)
#define MIN_DEVRSP_TIME		5						// 장치응답최소
#define SPR_DEVRSP_TIME		65						// VAN 공통 TIMEOUT 설정 (65초) //#N0158
#define MID_ICCMRCV_TIME	130						// ICCM응답 시간(MCU Timeout보다는 커야됨)

/////////////////////////////////////////////////////////////////////////////
// 거래 가능 ID
/////////////////////////////////////////////////////////////////////////////
#define TRAN_WITH_NOT		0x00
#define TRAN_WITH_CASH		0x01
#define TRAN_WITH_CHECK		0x02
#define	TRAN_WITH_50TCASH	0x08													// V06-01-01-#01 5만원권만 출금가능
#define TRAN_WITH_2CASH		(TRAN_WITH_CASH | TRAN_WITH_50TCASH)					// V06-01-01-#01 (만원+5만원) 출금가능
#define TRAN_WITH_CASHCHECK (TRAN_WITH_CASH | TRAN_WITH_CHECK)						// V06-01-01-#01 (만원+수표) 출금가능
#define TRAN_WITH_50TCASHCHECK (TRAN_WITH_50TCASH | TRAN_WITH_CHECK)				// V06-01-01-#01 (5만원+수표) 출금가능
#define TRAN_WITH_BOTH		(TRAN_WITH_CASH | TRAN_WITH_50TCASH | TRAN_WITH_CHECK)	// V06-01-01-#01 (만원+5만원+수표) 출금가능

#define TRAN_DEP_NOT 		0x00
#define TRAN_DEP_CASH		0x10
#define TRAN_DEP_CHECK		0x20
#define TRAN_DEP_BOTH		(TRAN_DEP_CASH | TRAN_DEP_CHECK)

#define TRAN_PBM_NOT 		0x00
#define TRAN_PBM_OK			0x80
#define TRAN_A4P_NOT 		0x00
#define TRAN_A4P_OK			0x40

#define TRAN_ALL_NOT		0x00
#define TRAN_ALL_OK			(TRAN_WITH_BOTH | TRAN_DEP_BOTH | TRAN_PBM_OK | TRAN_A4P_OK)

/////////////////////////////////////////////////////////////////////////////
// 명세표 및 저널 HEADER 정보 
/////////////////////////////////////////////////////////////////////////////
#define HEADER_NORMAL		0x00					// 명세표 및 저널 HEADER 정상
#define	SLIP_HEAD_UP		0x01					// 명세표 HEAD UP
#define	JNL_HEAD_UP			0x10					// 저널 HEAD UP

/////////////////////////////////////////////////////////////////////////////
// 유/무
/////////////////////////////////////////////////////////////////////////////
#define ON					1
#define OFF					0

/////////////////////////////////////////////////////////////////////////////
// EN/DISABLE(MCU, PBM)
/////////////////////////////////////////////////////////////////////////////
#define	ENABLE				1
#define	DISABLE				2

/////////////////////////////////////////////////////////////////////////////
// Track Infomation
/////////////////////////////////////////////////////////////////////////////
#define ISO1_TRACK			0x01
#define ISO2_TRACK			0x02
#define ISO3_TRACK			0x04
#define JIS_TRACK			0x08
#define ALL_TRACK			0x0f

/////////////////////////////////////////////////////////////////////////////
// 금융IC Board 종류 관리 2004.09.18 V070300
/////////////////////////////////////////////////////////////////////////////
#define CMI_BOARD				0
#define EMV_BOARD				1

/////////////////////////////////////////////////////////////////////////////
// Brm 지폐 이동 정보
/////////////////////////////////////////////////////////////////////////////
#define REFILL_MOVE			0x01
#define TAKEUP_MOVE			0x02

#define BRM_AUTOLOAD		0x10
#define BRM_MANUALLOAD		0x20
#define BRM_RESET			0x40
#define BRM_LOST			0x80

/////////////////////////////////////////////////////////////////////////////
// 거래판종류
/////////////////////////////////////////////////////////////////////////////
//#define HAN_MODE			0						// 국문
//#define ENG_MODE			1						// 영문
//#define JP_MODE				2						// 일문
//#define CHN_MODE			3						// 중문

/////////////////////////////////////////////////////////////////////////////
//	Language Mode
/////////////////////////////////////////////////////////////////////////////
#define KOR_MODE			0						// KOREA
#define ENG_MODE			1						// ENGLISH
#define CHN_MODE			2						// CHINA
#define JPN_MODE			3						// JAPAN
#define FOR_MODE			4						// 해외카드 #N0171
#define KEB_MODE			5						// 이지원  #N0174


/////////////////////////////////////////////////////////////////////////////
// TRANCTRL RETURN
/////////////////////////////////////////////////////////////////////////////
#define T_OK				0						// 처리OK
#define T_CANCEL			-1						// 취소선택
#define T_TIMEOVER			-2						// 시간초과
#define T_INPUTOVER			-3						// 입력횟수초과
#define T_INPUTERROR		-4						// 입력자료이상
#define T_ERROR				-5						// 기기장애발생(기기장애메세지)
#define T_SENDERROR			-6						// 송신장애발생(잔액확인메세지)
#define T_RECVERROR			-7						// 수신장애발생(잔액확인메세지)
#define T_DISMSG			-97						// 프로그램이상장애발생(전산부문의)
#define T_PROGRAM			-98						// 프로그램이상장애발생(전산부문의)
#define T_MSG				-99						// 취소발생(메세지처리)
#define T_HOSTMSG			-100					// 취소발생(SVR 메세지처리)

/////////////////////////////////////////////////////////////////////////////
// 화면 버튼정보
/////////////////////////////////////////////////////////////////////////////
#define S_INPUTOVER			"INPUTOVER"				// 입력횟수초과
#define S_TIMEOVER			"TIMEOVER"				// 시간초과
#define S_CANCEL			"취소"					// 취소
#define S_CANCEL2			"CANCEL"				// 취소
#define S_CLEAR				"CLEAR"					// CLEAR

#define S_CONFIRM			"확인"					// 확인
#define S_CONFIRM2			"OK"					// 확인
#define S_CORRECTION		"정정"					// 정정
#define S_EXTRA				"기타"					// 기타
#define S_EXIT				"종료"					// 종료
#define S_DEPOSIT			"추가입금"				// 추가입금
#define	S_YES				"예"					// 예
#define	S_NO				"아니오"				// 아니오
#define S_SHUTTERCLOSE		"닫음"					// 닫음
#define S_REGIST			"등록"					// 등록
#define S_NEXT				"다음"					// 다음
#define S_PREV				"이전"					// 이전
#define S_CONTINUE			"계속"					// 계속
#define S_PRINT				"출력"					// 출력

/////////////////////////////////////////////////////////////////////////////
// KeyCode Define
/////////////////////////////////////////////////////////////////////////////
#define K_MAN				138						// 만(10)
#define K_MANWON			K_MAN					// 만원
#define K_CHEN				139						// 천(11)
#define K_WON				140						// 원(12)
#define K_CONFIRM			141						// 확인(13)
#define K_CANCEL			142						// 취소(14)
#define K_CORRECTION		147						// 정정(19)

#define	K_BACKSPACE			161						//
#define	K_ENTER				162						//
#define	K_HOME				163						//
#define	K_END				164						//
#define	K_PGUP				165						// 이전
#define K_BEFORE			K_PGUP					//
#define	K_PGDN				166						// 다음
#define	K_NEXT				K_PGDN					//
#define	K_LEFT				167						// <<
#define	K_RIGHT				168						// >>
#define	K_UP				169						//
#define	K_DOWN				170						//
#define	K_ESC				171						//
#define	K_EXIT				K_ESC					// 종료
#define	K_TAB				172						//
#define	K_CHOICE			173						// 선택
#define	K_EXEC				174						// 실행
#define	K_WRITE				175						// 기록
#define	K_PRINT				176						// 인자
#define K_CHANGE			178						// 2003.05.31 Nam 공주정보설정#2에 숫자/영문 전환키 추가 for T1ATM

#define K_OK				0						// 확인
#define K_KEY_EMPTY			-1						// 없음
#define K_TIMEOVER			-2						// 시간초과

/////////////////////////////////////////////////////////////////////////////
// 다음의 값들은 기기의 상태, 수표해독 결과를 나타내는데 사용한다.
/////////////////////////////////////////////////////////////////////////////
#define NORMAL				0
#define DOING				1
#define DOWN				2
#define READ_ERROR			3
#define READ_TIME_OUT		4


#define DVR_NORMAL			1
#define DVR_ABNORMAL		0

#define SLOCK_NORMAL		1
#define SLOCK_ABNORMAL		0

#define BOOTING_NORMAL		0
#define BOOTING_ABNORMAL	1

/////////////////////////////////////////////////////////////////////////////
// OM_RESULT 정보
/////////////////////////////////////////////////////////////////////////////
#define OM_DO				"DO"					// 실행요청
#define OM_NORMAL			"NORMAL"				// 정상종료
#define OM_ERROR			"ERROR"					// 장애종료

/////////////////////////////////////////////////////////////////////////////
//	Pin Input Mode
//  V03-01-01	T1     : 사용하지 않음
//				TTW    : 전거래화면에서 다사용함
//				SCD(P) : 비밀번호 입력시에만 사용 - DUMMY MODE추가처리
/////////////////////////////////////////////////////////////////////////////
#define	PIN_DISABLE_MODE	1
#define	PIN_MENU_MODE		2
#define	PIN_NUMERIC_MODE	3
#define	PIN_PASSWORD_MODE	4
#define	PIN_AMOUNT_MODE		5
#define	PIN_INPUT_MODE		6
#define	PIN_ALL_MODE		7
#define	PIN_DUMMY_MODE		8									// V03-01-01 PINPAD DUUMY처리
#define	PIN_PASSWORD_MODE2	9
#define	PIN_PASSWORD_MODE3	10									// V02.C4.04 2010.01.13
#define	PIN_MENU_MODE1		11
#define	PIN_MENU_MODE2		12                                  // 이체금액 입력

#define	PIN_MENU_PAD		"CANCEL,F1,F2,F3,F4,F5,F6,F7,F8"
#define	PIN_MENU_PAD1		"CANCEL,ENTER,F1,F2,F3,F4,F5,F6,F7,F8"
#define	PIN_NUMERIC_PAD		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_PASSWORD_PAD	"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_PASSWORD_PAD2	"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR"
#define	PIN_PASSWORD_PAD3	"1,2,3,4,5,6,7,8,9,0"
#define	PIN_AMOUNT_PAD		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F12"
#define	PIN_INPUT_PAD		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F1,F2,F3,F4,F5,F6,F7,F8"
#define	PIN_ALL_PAD			"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F12"
#define	PIN_CANCEL_PAD		"CANCEL"							// V03-01-01 PANPAD DUMMY처리

#define	PIN_MENU_TOUCH		"CANCEL"
#define	PIN_NUMERIC_TOUCH	"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_PASSWORD_TOUCH	"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_AMOUNT_TOUCH	"1,2,3,4,5,6,7,8,9,0,00,CANCEL,CLEAR,ENTER"
#define	PIN_INPUT_TOUCH		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_ALL_TOUCH		"1,2,3,4,5,6,7,8,9,0,.,00,000,CANCEL,CLEAR,ENTER"


// --------------------------------------------------------------------------------
// Printer(Receipt/Journal/Passbook) 관련 Media/Form/Field name설정
// --------------------------------------------------------------------------------
#define		MEDIA_RECEIPT					_T("ReceiptMedia")
#define		MEDIA_JOURNAL					_T("JournalMedia")
#define		MEDIA_PBOOKREAD					_T("PbReadMedia")
#define		MEDIA_PBOOKWRITE				_T("PassbookMedia")

#define		FORM_RECEIPT					_T("Slip")
#define		FORM_RECEIPT_CGV				_T("Cgv")

#define		FORM_JOURNAL					_T("Journal")
#define		FORM_JOURNAL_PRE				_T("Journal")
#define		FORM_JOURNAL_POST				_T("Journal")

#define		FORM_PBOOKREAD					_T("PbookMS")
#define		FORM_PBOOKWRITE					_T("PrintPassBook")
#define		FORM_PBOOKBARCODE				_T("PbookBarCode")		// 20030709_1.0_1049 : 통장바코드 읽기용 

#define		FIELD_PBOOKREAD					_T("ReadData")
#define		FIELD_PBOOKWRITE				_T("RawData")
#define		FIELD_PBOOKBARCODE				_T("ReadBarCode")		// 20030709_1.0_1049 : 통장바코드 읽기용 
/////////////////////////////////////////////////////////////////////////////
//	SCDP SPL LED DEFINE											// V03-01-01
/////////////////////////////////////////////////////////////////////////////
#define SPLLED_POLL			0						// 교신 		(CD용)
#define SPLLED_WITHDRAW		1						// 출금			(CD용)
#define SPLLED_DOING_TRAN	2						// 거래중		(CD용)
#define SPLLED_PSBK_TRAN	3						// 통장 거래	(CD용)

#define SPLLED_SLIP_LOW		4						// 명세표 (잔량)(CD용)
#define SPLLED_JNL_LOW		5						// 저널   (잔량)(CD용)
#define SPLLED_CST1_LOW		6						// 카세트1(잔량)(CD용)
#define SPLLED_CST2_LOW		7						// 카세트2(잔량)(CD용)
#define SPLLED_CST3_LOW		8						// 카세트3(잔량)(CD용)

#define SPLLED_CARD_MAT		9						// 카드   (매체)(CD용)
#define SPLLED_SLIP_MAT		10						// 명세표 (매체)(CD용)
#define SPLLED_JNL_MAT		11						// 저널   (매체)(CD용)
#define SPLLED_PSBK_MAT		12						// 통장   (매체)(CD용)
#define SPLLED_NOTES_MAT	13						// 지폐	  (매체)(CD용)

#define	SPLLED_CASH_CNT		14						// KEY_현금매수	(CD용)
#define	SPLLED_OPEN_CLOSE	15						// KEY_개폐국	(CD용)
#define	SPLLED_KEY_7		SPLLED_OPEN_CLOSE
#define	SPLLED_MAGAM_INQ	16						// KEY_마감조회	(CD용)
#define	SPLLED_KEY_8		SPLLED_MAGAM_INQ
#define	SPLLED_MAGAM		17						// KEY_마감거래	(CD용)
#define	SPLLED_KEY_9		SPLLED_MAGAM
#define	SPLLED_CHECK_CNT	18						// KEY_수표매수	(CD용)
#define	SPLLED_KEY_4		19						// KEY_4		(CD용)
#define	SPLLED_PB_ENDIS		20						// KEY_통장		(CD용)
#define	SPLLED_KEY_5		SPLLED_PB_ENDIS
#define	SPLLED_SOUND		21						// KEY_음성		(CD용)
#define	SPLLED_KEY_6		SPLLED_SOUND

#define	SPLLED_ON_CHAR		'1'						// LED_ON
#define	SPLLED_OFF_CHAR		'0'						// LED_OFF

/////////////////////////////////////////////////////////////////////////////
//	SCDP SPL SWITCH(KEY) DEFINE									// V03-01-01 W R B
////////////////////////////////////////////////////////////////////////////////////  
#define	SPLKEY_CASH_CNT		1						// 현금매수		(CD용)	미사용
#define	SPLKEY_OPEN_CLOSE	2						// 개폐국		(CD용)
#define	SPLKEY_MAGAM_INQ	3						// 마감조회		(CD용)	미사용
#define	SPLKEY_MAGAM		4						// 마감거래		(CD용)	미사용
#define	SPLKEY_CHECK_CNT	5						// 수표매수		(CD용)	미사용
#define	SPLKEY_4			6						// 4			(CD용)	미사용
#define	SPLKEY_PB_ENDIS		7						// 통장			(CD용)	통장사용
#define	SPLKEY_6			8						// 6			(CD용)	전자화폐
#define	SPLKEY_ATM_TOTAL	9						// 소계			(CD용)
#define	SPLKEY_RESET		10						// RESET		(CD용)
#define	SPLKEY_2			11						// 2			(CD용)	미사용
#define	SPLKEY_GRAND_TOTAL	12						// 합계			(CD용)

/////////////////////////////////////////////////////////////////////////////
// A4 Printer Form Data
/////////////////////////////////////////////////////////////////////////////
#define A4Prt_TranList			"A4Form_TranList"		// Form Image Data(거래내역조회)			2007.06.12(火)
#define A4Prt_DepSecurList		"A4Form_DepSecurList"	// Form Image Data(예수금조회&유가증권조회)	2007.06.12(火)
#define A4Prt_InqStock   		"A4Form_InqStock"		// Form Image Data(청약내역조히)	            2008.04.02(수)
//////////////////////////////////////////////////////////////////////
// Spl, Pnc, Flicker, Lamp, Opl
//////////////////////////////////////////////////////////////////////
#define	RATM_SPL			1
#define	CD_SPL				3		// vt01

#define	CALLKEY				0x01
#define	LOADSW				0x02
#define	REARDOOR			0x04
#define	TESTSW				0x08

#define FLICKER_OFF			0
#define FLICKER_ON			1
#define	FLICKER_ON_CONTINUOUS		2	// FLICKER ON Continuous //#0143

#define LAMP_OFF			0
#define LAMP_ON				1

// U8100 AP 변경내역 #05 - LED기능 지원
/////////////////////////////////////////////////////////////////////////////
#define LED_ENTER_STYLE_AUTO		0
#define LED_ENTER_STYLE_STOP		1
#define LED_ENTER_STYLE_LEFT_FLOW	2
#define LED_ENTER_STYLE_UP_MOVE		3
#define LED_ENTER_STYLE_DOWN_MOVE	4
#define LED_ENTER_STYLE_UPDOWN		5
#define LED_ENTER_STYLE_LEFTRIGHT	6
#define LED_ENTER_STYLE_DOWN_STACK	7
#define LED_ENTER_STYLE_SCATTER		8
#define LED_ENTER_STYLE_LEFT_LASER	9
#define LED_ENTER_STYLE_UP_LASER	10
#define LED_ENTER_STYLE_DOWN_LASER	11
#define LED_ENTER_STYLE_ANIMATION1	12
#define LED_ENTER_STYLE_ANIMATION2	13
#define LED_ENTER_STYLE_ANIMATION3	14
#define LED_ENTER_STYLE_ANIMATION4	15

#define LED_EXIT_STYLE_AUTO			0
#define LED_EXIT_STYLE_STOP			1
#define LED_EXIT_STYLE_UP_MOVE		2
#define LED_EXIT_STYLE_DOWN_MOVE	3
#define LED_EXIT_STYLE_UPDOWN		4
#define LED_EXIT_STYLE_LEFTRIGHT	5
#define LED_EXIT_STYLE_DOWNTOUP		6
#define LED_EXIT_STYLE_SCATTER		7
#define LED_EXIT_STYLE_RIGHT_LASER	8
#define LED_EXIT_STYLE_DOWN_LASER	9
#define LED_EXIT_STYLE_UP_LASER		10
#define LED_EXIT_STYLE_ANIMATION1	11
#define LED_EXIT_STYLE_ANIMATION2	12
#define LED_EXIT_STYLE_ANIMATION3	13
#define LED_EXIT_STYLE_ANIMATION4	14

#define LED_TEXT_SELECT				0
#define LED_IMAGE_SELECT			1

#define LED_TEXT_SAVE_COUNT         10
#define LED_IMAGE_SAVE_COUNT        120


// 마감 구분자
#define	BEFORE_SHUTDOWN		1						// 마감전
#define	AFTER_SHUTDOWN		2						// 마감후
#define	HOLIDAY				3						// 휴일
#define	DELAY_SERVICE		4						// 서비스연체
#define	INSURTRAN_NEXTFLOW	5						// 보험
#define	DES_TRAN			9						// 해외거래

// 점포별 구분
#define	CVS_GS24			1						// GS
#define	CVS_FM24			2						// FM
#define	E_MART_EVERYDAY		4						// E-Mart EveryDay


#define LOCK_STAT_CHECK_NONE                    (0)
#define LOCK_STAT_CHECK_OK		                (1)
#define LOCK_STAT_CHECK_NG                      (2)

/////////////////////////////////////////////////////////////////////////////
// 카드정보
/////////////////////////////////////////////////////////////////////////////
typedef struct	tagISO2Form
{
	BYTE MembershipNo[16];							// 000. MEMBERSHIP NO
	BYTE Seperator;									// 016. JARI BUNLI BUHO 
	BYTE EndDay[4];									// 017. TRC.END DAY 
	BYTE ServiceNo[3];								// 021. SERVICE NO 
	BYTE ScrambleNo[4];								// 024. SCRAMBLE NO 
	BYTE Dummy[228];								// 028. (256)
} MCISO2;

typedef struct	tagISO3Form
{
	BYTE FormatCode[2]; 							// 000.	형식코드		01
	BYTE IdNo[2]; 									// 002. 업태코드		59
	BYTE BankNo[3];									// 004. 은행코드		BBB
	BYTE Separator1;								// 007. 분리부호		=
	BYTE AccountNo[16];								// 008. 지점번호+계좌번호
	BYTE AccountCheck;								// 024. 검증번호
	BYTE Separator2;								// 025. 분리부호		=
	BYTE Nation[3]; 								// 026. 국가코드		410
	BYTE CashCode[3];								// 029. 화폐코드		410
	BYTE Unknown1[15];								// 032.
	BYTE ValidCode;									// 047. 유효성코드 
	BYTE Password[6];								// 048. 비밀번호 
	BYTE Trade;										// 054. 상호
	BYTE Tran1[2];									// 055. 과목1
	BYTE Tran2[4];									// 057. 과목2
	BYTE OtherData[27];								// 061. 기타데이터
	BYTE Remark[16];								// 088. 보충자료
	BYTE Dummy[152];								// 104.	(256)
} MCISO3;

/////////////////////////////////////////////////////////////////////////////
// 통장정보
/////////////////////////////////////////////////////////////////////////////
/*
typedef struct	tagPBMSForm
{
	BYTE BankLine[10];								// 000. 뱅크라인
	BYTE AccountNo[13];								// 010. 계좌번호
	BYTE Dummy1[6];									// 023. dummy
	BYTE BankCode[2];								// 029. 은행코드
	BYTE PbNumer[8];								// 031. 통장번호
	BYTE Dummy2[6];									// 039. dummy
	BYTE Filler[211];								// 045. (256)
~} PBMS;
*/
/*
typedef struct	tagPBMSForm
{
	BYTE Filler1[4];		// FixValue
	BYTE BankCode[3];		// 회사코드
	BYTE Filler2;			// FixValue
	BYTE AccountNo[11];		// 계좌번호
	BYTE Filler3[2];		// FixValue
	BYTE YYYY[4];			// 연도
	BYTE IssurNum[3];		// 발급회수
	BYTE MMDD[4];			// 월일
	BYTE CHeckBit[4];		// 체크비트
	BYTE HHMMSS[6];			// 시분초
	BYTE Filler4[2];		// FixValue
} PBMS;
*/
/////////////////////////////////////////////////////////////////////////////
// 통장정보
/////////////////////////////////////////////////////////////////////////////
typedef struct	tagPBMSForm
{
	BYTE AccountNo[17];								// 000. 계좌번호
	BYTE Filler[173];								// 017. (190)
} PBMS;
/////////////////////////////////////////////////////////////////////////////
// Magnetic Card Area
/////////////////////////////////////////////////////////////////////////////
typedef struct	tagMCVW 
{
	BYTE TrInfo;
	WORD JISsize;
	BYTE JISBuff[256];         
	WORD ISO1size;               
	BYTE ISO1Buff[256];     
	WORD ISO2size;              
	BYTE ISO2Buff[256];    
	WORD ISO3size;             
	BYTE ISO3Buff[256];   
} MCAP;

/////////////////////////////////////////////////////////////////////////////
// Magnetic Pb Area
/////////////////////////////////////////////////////////////////////////////
typedef struct tagMS 
{
	WORD Len;
	BYTE sBuf[256];
} MS;

typedef struct tagBAR			// Passbook Bar Info
{
	WORD Len;
	BYTE sBuf[256];
} BAR;
/////////////////////////////////////////////////////////////////////////////
// Finger Area
/////////////////////////////////////////////////////////////////////////////
typedef struct tagFINGER 
{
	WORD Len;
	BYTE sBuf[1024];
} FINGER;

/////////////////////////////////////////////////////////////////////////////
// 2D-BarCode Area
/////////////////////////////////////////////////////////////////////////////
typedef struct tagBARCODE2D
{
	WORD Len;
	BYTE sBuf[1024];
} BAR2D;

/////////////////////////////////////////////////////////////////////////////
// RejectMoney Area
/////////////////////////////////////////////////////////////////////////////
typedef	struct tagREJECT_MONEY 
{
	int	 mTotalAmount;								// 회수된 총금액
	WORD mTenThousand;								// 회수된 만원권의 매수
	WORD mFiveThousand;								// 회수된 오천원권의 매수
	WORD mOneThousand;								// 회수된 천원권의 매수
	WORD mFiftyThousand;							// 회수된 오만원권의 매수							// V06-01-01-#01
	WORD mCheck;									// 회수된 수표의 매수
	WORD mUnknown;									// 회수된 지폐중 감별되지 않은 지폐의 매수
} RejectMoney;

/////////////////////////////////////////////////////////////////////////////
// RetInformation Area
/////////////////////////////////////////////////////////////////////////////
typedef struct tagRET_INFORMATION 
{
	int	Status;										// 동작 시행 상태
	WORD ActNum;									// 실제 보충/회수된 매수
	WORD RJTNum;									// reject box로 회수된 총 매수
	WORD TenThRJTNum;								// 보충/회수 동작중 RJB로 회수된 만원권의 매수
	WORD FiftyThRJTNum;								// 보충/회수 동작중 RJB로 회수된 오만원권의 매수	// V06-01-01-#01
	WORD FiveThRJTNum;								// 보충/회수 동작중 RJB로 회수된 오천원권의 매수
	WORD OneThRJTNum;								// 보충/회수 동작중 RJB로 회수된 천원권의 매수
	WORD CheckRJTNum;								// 보충/회수 동작중 RJB로 회수된 수표의 매수
	WORD UnknownRJTNum;								// 보충/회수 동작중 RJB로 회수된 불명권의 매수
} RetInformation;


/////////////////////////////////////////////////////////////////////////////
// DepositMoney Area
/////////////////////////////////////////////////////////////////////////////
typedef	struct tagDEPOSIT_MONEY 
{
	int  mStatus;									// 동작종료후의 보고정보
	int  mTotalAmount;								// 계수된 총금액
	WORD mFiftyThousand;							// 계수된 오만원권의 매수							// V06-01-01-#01
	WORD mTenThousand;								// 계수된 만원권의 매수
	WORD mFiveThousand;								// 계수된 오천원권의 매수
	WORD mOneThousand;								// 계수된 천원권의 매수
	WORD mUnknown;									// 계수된 지폐중 감별되지 않은 지폐의 매수
} DepositMoney;

/////////////////////////////////////////////////////////////////////////////
// CheckInfo Area
/////////////////////////////////////////////////////////////////////////////
typedef	struct tagCHECK_INFO 
{
	BYTE ReadData[8];								// 입금된 수표의 번호
	BYTE Bank[3];									// V05-06-03-#03:은행코드3자리변경	// 입금된 수표의 발행은행 CODE
	BYTE Branch[4];									// 입금된 수표의 발행지점 CODE
	BYTE Kind[2];									// 입금된 수표의 권종
	BYTE Amount[10];								// 입금된 수표의 금액
	BYTE AccountNum[6];							// V05-01-06-#02:수표계좌번호
} CheckInfo;

/////////////////////////////////////////////////////////////////////////////
// CheckPrint Area
/////////////////////////////////////////////////////////////////////////////
typedef char CheckPrint[256];						// 수표배서 내용

/////////////////////////////////////////////////////////////////////////////
// MENU Header
/////////////////////////////////////////////////////////////////////////////
typedef struct tagMENUHeader
{
	char	MenuScrName[81];						// 메뉴화면이름
	int		MenuScrNum;								// 메뉴화면번호
	char	MenuKind[22][81];						// 메뉴(버튼)종류(이름_
	int		MenuInfo[22][7];						// 메뉴정보
} MENUHeader;

/////////////////////////////////////////////////////////////////////////////
// ElecJournal Form(MDB)
/////////////////////////////////////////////////////////////////////////////
typedef struct tagElecJournalForm 
{
	CString TransSeqNum;
	CString	AccountNum;
	CString TransAmount;	
	CString TransKind;
	CString OpenBank;
	CString CheckNum;
	CString TenNum;
	CString FiveNum;
	CString OneNum;
	CString ImagePath;
	CString SlipData[19];
} ElecJournalForm;

/////////////////////////////////////////////////////////////////////////////
// ElecJournal Form(TXT)
/////////////////////////////////////////////////////////////////////////////
#define	SPR_MAX_ROW			25		
#define	SPR_MAX_COL			40	
#define	SPR_MAX_ROW_U8100	25 //#CS0002		
#define	JPR_MAX_ROW			25
#define	SPR_MAX_ROW_HIPASS	35	
#define	JPR_MAX_COL			40						// JPR SIZE > SPR SIZE(El
#define	JNL_MAX_DATA    	160

typedef struct tagEjrTbl							// Total Area(2048 BYTE)
{
													// Key Area(128 BYTE)
	char	Fill1;									// Filler7				// 80
	char	Date[8];								// 거래일자				// 9
	char	Fill2;									// Filler7				// 80
	char	Time[12];								// 거래시간				// 18
	char	Fill3;									// Filler7				// 80
	char	TranMaterial[10];						// 거래매체				// 81
	char	Fill4;									// Filler8				// 91
	char	Dummy[34];								// Dummy				// 92
	char	Cr;										// Cr					// 126
	char	Lf;										// Lf					// 127[128]

													// Data Area(1920 BYTE)
//	char	Data[SPR_TICKET_MAX_ROW][JPR_MAX_COL + 3];		// Ejr영역('|'+Cr+Lf포함)		
//	char	DataDummy[2048 - 128 - (SPR_TICKET_MAX_ROW * (JPR_MAX_COL + 3)) - 2];		//2048->1028로 2007 12/20 문승준 전자저널 공란 스페이스수정	

	char	Data[JPR_MAX_ROW][JPR_MAX_COL + 3];		// Ejr Area('|'+Cr+Lf)
	char	DataDummy[JPR_MAX_COL];					// DataDummy
	char	DataDummyCr;							// Cr
	char	DataDummyLf;							// Lf
	
//	char	DataDummy[2048 - 128 - (JPR_MAX_ROW * (JPR_MAX_COL + 3)) - 2];														// DataDummy

//	char	DataDummyCr;							// Cr
//	char	DataDummyLf;							// Lf
} EjrTbl;


typedef struct tagNEjrTblDump					    // Total Area(256 BYTE)
{
	                                                // Key Area(128 BYTE)
	char	SerialNo[8];							// 저널일련번호			// 0
	char	Fill1;									// Filler1				// 8
	char	Date[10];								// 거래일자				// 9
	char	Fill2;									// Filler2				// 17
	char	Time[8];								// 거래시간				// 18
	char	Fill3;									// Filler5				// 24
	char	AccountNum[20];							// 계좌번호				// 25
	char	Fill4;									// Filler3				// 45
	char	TranSerialNo[6];						// 거래일련번호			// 46
	char	Fill5;									// Filler4				// 50
	char	TranAmount[12];							// 거래금액				// 51
	char	Fill6;									// Filler6				// 59
	char	KeyNum[20];								// 키번호				// 60
	char	Fill7;									// Filler7				// 80
	char	TranMaterial[10];						// 거래매체				// 81
	char	Fill8;									// Filler8				// 91
	char	Dummy[160];								// Dummy				// 92
	char	Cr;										// Cr					// 252
	char	Lf;										// Lf					// 123[254]
} NEjrTblDump;

typedef struct tagEEjrTblDump					    // Total Area(256 BYTE)
{
	char	Fill1;									// Filler7				// 80
	char	Date[8];								// 거래일자				// 9
	char	Fill2;									// Filler7				// 80
	char	Time[12];								// 거래시간				// 18
	char	Fill3;									// Filler7				// 80
	char	TranMaterial[10];						// 거래매체				// 81
	char	Fill4;									// Filler8				// 91
	char	Dummy[160];								// Dummy				// 92
	char	Cr;										// Cr					// 252
	char	Lf;										// Lf					// 123[254]
} EEjrTblDump;

/////////////////////////////////////////////////////////////////////////////
// ElecJournal 금액관련자료
/////////////////////////////////////////////////////////////////////////////
typedef struct tagEjrAmountTbl
{
	int		TranProc;								// 입출금구분
	int		TranAmount;								// 입출금거래금액
	int		Cash10000Num;							// 입출금만원권매수
	int		Cash5000Num;							// 입출금오천원권매수
	int		Cash1000Num;							// 입출금천원권매수
	int		Cash50000Num;							// 입출금오만원매수										// V06-01-01-#01
	int		MotherCheck10Num;						// 입출금자행수표10만원권매수
	int		MotherCheck30Num;						// 입출금자행수표30만원권매수
	int		MotherCheck50Num;						// 입출금자행수표50만원권매수
	int		MotherCheck100Num;						// 입출금자행수표100만원권매수
	int		OtherCheck10Num;						// 입출금타행수표10만원권매수
	int		OtherCheck30Num;						// 입출금타행수표30만원권매수
	int		OtherCheck50Num;						// 입출금타행수표50만원권매수
	int		OtherCheck100Num;						// 입출금타행수표100만원권매수
} EjrAmountTbl;


// 전자화폐 호스트지정 Flag
#define STOPBITREMOVE		1						// 0xF1 = STOP BIT 강제해지전문
#define PARAMCHANGE			2						// 0xF2 = 파라미터 갱신거래요구
#define HSOTSIGNOK			3						// 0xF3 = 거래 정상 완료
#define PARAMCHANGEWANT		4						// 0xF4 = 파라미터 갱신 응답전문
#define BANKICKEYCHANGE		5						// 0xF5 = 금융IC 키버전 갱신거래요구
#define BANKICKEYCHANGEWANT	6						// 0xF6 = 금융IC 키버전 갱신응답전문있음
#define KCASHPINREG			7						// 0xF7 = 전자화폐PIN등록거래

//////////////////////////////////////////////////////////////////////
//	AJMS관련 Define
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//	AJMS FILE & FOLDER Define
//////////////////////////////////////////////////////////////////////
#define	_AJMS_DIR			_T("D:\\AJMS\\")
#define _SERIAL_LOG			_T("SER\\SERIALNO.LOG")
#define _IMG_CARD			_T("IMG\\CARD\\")
#define _IMG_FACE			_T("IMG\\FACE\\")
#define _IMG_EMIT			_T("IMG\\EMIT\\")

//////////////////////////////////////////////////////////////////////
// 저널쓰기구분
//////////////////////////////////////////////////////////////////////
#define AJMS_JOURNL			0x01
#define AJMS_PAPER			0x02
#define AJMS_BOTH			(AJMS_JOURNL | AJMS_PAPER)

//////////////////////////////////////////////////////////////////////
// 데이터구분
//////////////////////////////////////////////////////////////////////
#define	DATA_JNL			'1'						// 저널
#define	DATA_SENDRECV		'2'						// 전문
#define	DATA_IMG_FACE		'3'						// 화상이미지
#define DATA_IMG_EMIT		'4'						// 방출구이미지
#define	DATA_EMBOSS			'5'						// 카드이미지

#define	AJMS_TYPE_JNL		0x01					// 저널
#define	AJMS_TYPE_SENDRECV	0x02					// 전문
#define	AJMS_TYPE_IMG_FACE	0x04					// 화상이미지
#define AJMS_TYPE_IMG_EMIT	0x08					// 방출구이미지
#define	AJMS_TYPE_EMBOSS	0x10					// 카드이미지

//////////////////////////////////////////////////////////////////////
// 저널종류구분
//////////////////////////////////////////////////////////////////////
#define	JNL_OPEN			1						// 개국저널
#define	JNL_TRAN			2						// 거래저널
#define	JNL_CANCEL			3						// 취소거래저널
#define	JNL_STATUS			4						// 상태저널
#define	JNL_CLOSE			5						// 마감저널
#define	JNL_OPER			6						// 운영저널
#define	JNL_CASH			7						// 시재저널
#define	JNL_ERROR			8						// 장애저널

#define	ATM_CPUMEMINFO		3						// 자동기 (CPU,MEM) 사용률

//////////////////////////////////////////////////////////////////////
// 메세지구분
//////////////////////////////////////////////////////////////////////
#define ERROR_HDD			"HDDERROR"				// HDD장애
#define ERROR_ENCRYPTFAIL	"ENCRYPTFAIL"			// 암호화장애
#define ERROR_OPEN			"OPEN"					// 개국
#define ERROR_TRANEND		"TRANEND"				// 거래종료

//////////////////////////////////////////////////////////////////////
// 2007.12.14 msj터치제어 등 수정 Define
#define EXE_TOUCH	_T("C:\\PROGRAM FILES\\TPDRV\\TPDRV.EXE")	
#define EXE_TOUCH_PATH	_T("C:\\PROGRAM FILES\\TPDRV\\")			//DUAL TOUCH DRIVER

#define EXE_DUAL_TOUCH	_T("C:\\PROGRAM FILES\\TPDUAL_DRV\\TPDUAL_DRV.EXE")	
#define EXE_DUAL_TOUCH_PATH	_T("C:\\PROGRAM FILES\\TPDUAL_DRV\\")			//DUAL TOUCH DRIVER
 

#define EXE_OSK			_T("%SYSTEMROOT%\\SYSTEM32\\OSK.EXE")				
#define EXE_TMFA		_T("C:\\TM\\VDM\\TMFA.EXE")							 
#define EXE_VDMFA		_T("C:\\TM\\VDM\\VDMFA.EXE")	
#define EXE_ATMINSIDE	_T("C:\\Program Files\\Ubiator20\\EXE\\ATMInside.exe")						 

// 계정에 따른 Lnk삭제
#define	EXE_TOUCH_LNK1	_T("C:\\Documents and Settings\\Administrator\\시작 메뉴\\프로그램\\시작프로그램\\tpdrv.LNK")
#define	EXE_TOUCH_LNK2	_T("C:\\Documents and Settings\\All Users\\시작 메뉴\\프로그램\\시작프로그램\\tpdrv.LNK")
#define	EXE_TOUCH_LNK3	_T("C:\\Documents and Settings\\Default User\\시작 메뉴\\프로그램\\시작프로그램\\tpdrv.LNK")
#define	EXE_TOUCH_LNK4	_T("C:\\Documents and Settings\\vancd3\\시작 메뉴\\프로그램\\시작프로그램\\tpdrv.LNK")
#define	EXE_TOUCH_LNK5	_T("C:\\Documents and Settings\\HSCUSER\\시작 메뉴\\프로그램\\시작프로그램\\tpdrv.LNK")

// 계정에 따른 Lnk삭제
#define	EXE_DUAL_TOUCH_LNK1	_T("C:\\Documents and Settings\\Administrator\\시작 메뉴\\프로그램\\시작프로그램\\tpdual_drv.Lnk")
#define	EXE_OSK_LNK1		_T("C:\\Documents and Settings\\Administrator\\시작 메뉴\\프로그램\\보조프로그램\\내게 필요한 옵션\\화상 키보드.Lnk")

#define	EXE_DUAL_TOUCH_LNK2	_T("C:\\Documents and Settings\\All Users\\시작 메뉴\\프로그램\\시작프로그램\\tpdual_drv.Lnk")
#define	EXE_OSK_LNK2		_T("C:\\Documents and Settings\\All Users\\시작 메뉴\\프로그램\\보조프로그램\\내게 필요한 옵션\\화상 키보드.Lnk")

#define	EXE_DUAL_TOUCH_LNK3	_T("C:\\Documents and Settings\\Default User\\시작 메뉴\\프로그램\\시작프로그램\\tpdual_drv.Lnk")
#define	EXE_OSK_LNK3		_T("C:\\Documents and Settings\\Default User\\시작 메뉴\\프로그램\\보조프로그램\\내게 필요한 옵션\\화상 키보드.Lnk")

#define	EXE_DUAL_TOUCH_LNK4	_T("C:\\Documents and Settings\\T1ATM\\시작 메뉴\\프로그램\\시작프로그램\\tpdual_drv.Lnk")
#define	EXE_OSK_LNK4		_T("C:\\Documents and Settings\\T1ATM\\시작 메뉴\\프로그램\\보조프로그램\\내게 필요한 옵션\\화상 키보드.Lnk")

#define	EXE_DUAL_TOUCH_LNK5	_T("C:\\Documents and Settings\\HSCUSER\\시작 메뉴\\프로그램\\시작프로그램\\tpdual_drv.Lnk")
#define	EXE_OSK_LNK5		_T("C:\\Documents and Settings\\HSCUSER\\시작 메뉴\\프로그램\\보조프로그램\\내게 필요한 옵션\\화상 키보드.Lnk")

#define	EXE_NEXTWARE_LNK	_T("C:\\Documents and Settings\\All Users\\시작 메뉴\\프로그램\\Nextware")
#define	EXE_UBIATOR_LNK 	_T("C:\\Documents and Settings\\All Users\\시작 메뉴\\프로그램\\Ubiator")

#define	EXE_ETC_LNK1		_T("C:\\Documents and Settings\\All Users\\시작 메뉴\\프로그램\\보조프로그램")
#define	EXE_ETC_LNK2 		_T("C:\\Documents and Settings\\Administrator\\시작 메뉴\\프로그램\\보조프로그램")
#define	EXE_ETC_LNK3 		_T("C:\\Documents and Settings\\Default User\\시작 메뉴\\프로그램\\보조프로그램")



// 2010.05.13 Yun.H.J 매체 적용시 브랜드 화면 복원
#define BAT_HYDCPTL_UPDATE	_T("C:\\T1Atm\\App\\BrandSwf\\HYDCPTL_Update.bat")	
#define BAT_JE_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\JE_Update.bat")	
#define BAT_KB_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\KB_Update.bat")	
#define BAT_KEB_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\KEB_Update.bat")	
#define BAT_KI_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\KI_Update.bat")	
#define BAT_KW_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\KW_Update.bat")	
#define BAT_NOH_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\NOH_Update.bat")	
#define BAT_NOHD_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\NOHD_Update.bat")	
#define BAT_POS_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\POS_Update.bat")	
#define BAT_SC_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\SC_Update.bat")	
#define BAT_SUH_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\SUH_Update.bat")	
#define BAT_TYJ_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\TYJ_Update.bat")	
#define BAT_CITI_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\CITI_Update.bat")	 
#define BAT_HANA_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\HANA_Update.bat")
#define BAT_SHINHAN_UPDATE	_T("C:\\T1Atm\\App\\BrandSwf\\SHINHAN_Update.bat")
#define BAT_KYN_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\KYN_Update.bat") 
#define BAT_NICE_UPDATE		_T("C:\\T1Atm\\App\\BrandSwf\\NICE_Update.bat")

#define	EXE_FTP_SERVER	_T("C:\\FTP_Server\\Server.exe")
/////////////////////////////////////////////////////////////////////////////////
// 통신모뎀 설정 (LGT, KTF, KT_ADSL, KT_VDSL, NANARO
/////////////////////////////////////////////////////////////////////////////////
#define		MODEM_LGT					3				
#define		MODEM_KTF					6	
#define		MODEM_LGT_UPlus				7				
#define		MODEM_KT_ADSL				2				
#define		MODEM_KT_VDSL				0				
#define		MODEM_HANARO				5				
#define		MODEM_LGT_U_GPS				8				

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Message Information
//////////////////////////////////////////////////////////////////////
#define MI_READY			0x60					// 준비완
#define MI_KEYSHARE			0x61					// 키공유
#define MI_TRAN				0x41					// 일반거래
#define MI_OPEN				0x41					// 개국
#define MI_STAT				0xa8					// 상태
#define MI_ERROR			0xa3					// 장애
#define MI_SNOD				0x50					// SNOD
#define MI_READY_OFF		0x22					// 준비완 OFF
#define MI_CLOSE			0xa2					// 폐국

//////////////////////////////////////////////////////////////////////////
// 2007.06.11 for ATMS Define
#define AGT2ATM_MEM_SIZE	516
#define ATM2AGT_MEM_SIZE	256

#define ATMS_CMD_IDLE		0
#define ATMS_CMD_POLL		1
#define ATMS_CMD_INIT		2
#define ATMS_CMD_REBOOT		3  //리부팅 사유 -> 정상 (새벽)
#define ATMS_CMD_JNLMODE	4
#define ATMS_CMD_DOWNLOAD	5
#define ATMS_CMD_REBOOT1	6  //리부팅 사유 -> HOST 원격
#define ATMS_CMD_CAMMODE	7
#define ATMS_CMD_REBOOT2	8  //리부팅 사유 -> 라인장애
#define ATMS_CMD_REBOOT3	9  //리부팅 사유 -> 사용자 설정 
#define ATMS_CMD_UPLOAD  	10
#define ATMS_CMD_REBOOT4	11 //리부팅 사유 -> 계원 설정 
#define ATMS_CMD_REBOOT5	12 //리부팅 사유 -> AMS 원격 리부팅
#define ATMS_CMD_REBOOT6	13 //리부팅 사유 -> AMS 원격 POWER OFF 
#define ATMS_CMD_REBOOT7	14 //개국 및 상태오류로 인하여 리부팅
#define ATMS_CMD_REOPEN		15 //개국 및 상태오류로 인하여 리부팅
#define ATMS_CMD_RESTATUS	17 


#define TRAN_TXRX_STATUS	10
#define TRAN_TXRX_ERROR  	20


#define ATMS_JNL_HDD		0									//전자저널모드
#define ATMS_JNL_MIX		1									//혼합모드(종이+전자저널)
#define ATMS_JNL_PAPER		2									//종이저널모드


#define DVR_CAPTURE_FACE		0								//DVR 얼굴
#define DVR_CAPTURE_HAND		1								//DVR 손
#define DVR_CAPTURE_DOOR		2								//DVR 금고부
#define DVR_CAPTURE_ETC0		3								//DVR 전체

#define PIDILE       	1
#define SENDING      	10
#define SENDING_OK     	11
#define RECVING      	20
#define RECVING_OK     	21


//////////////////////////////////////////////////////////////////////////

// 통합일련번호파일 공통부
typedef struct _tagAjmsHead
{
	BYTE	JnlSerial[12];				// 통합저널일련번호
	BYTE	DataFlag;					// 데이터구분
										// 1 : 저널
										// 2 : 전문
	BYTE	StepNum1[12];				// STEP일련번호1
	BYTE	StepNum2[12];				// STEP일련번호2
	BYTE	JnlFlag;					// 저널종류구분코드
	BYTE	TranFlag;					// 전문구분
	BYTE	DirInfo[64];				// 디렉토리정보
	BYTE	YYMMDD[8];					// 발생일자
	BYTE	HHMMSS[6];					// 발생시간
	BYTE	BranchNum[3];				// 영업점코드
	BYTE	AtmNo[2];					// 단말번호
	BYTE	AtmIp[15];					// ATM IP
	BYTE	JnlLineCnt[2];				// 저널라인수
	BYTE	FileCreate;					// 파일생성여부		(여:1, 부:0)
	BYTE	PaperWrite;					// 종이저널인자여부 (여:1, 부:0)
	BYTE	Encryt;						// 암호화여부		(여:1, 부:0)
} AjmsHead;

// 통합일련번호파일 데이터부
typedef struct _tagAjmsBody
{
	BYTE	OpenTime[6];				// 개국시각
	BYTE	DevSerial[16];				// 기기일련번호
	BYTE	ModelName[10];				// 모델명
	BYTE	TranFlag[2];				// 거래구분코드
	BYTE	TranDetailFlag[2];			// 거래구분상세
	BYTE	TranDate[8];				// 거래년월일
	BYTE	TranSerial[4];				// 거래번호
	BYTE	TranCode[4];	 			// 거래코드
	BYTE	MaterialInfo[2]; 			// 자동화기기이용매체구분코드
	BYTE	MSData[25];					// 자동화기기MS정보
	BYTE	MotherBank[2]; 				// 매체발행은행코드
	BYTE	OtherBank[2];	 			// 대상은행코드
	BYTE	AccountM[16];				// 거래계좌번호(카드번호)
	BYTE	AccountT[16];				// 대상계좌번호(카드번호)
	BYTE	CashCnt[3];					// 현금거래매수
	BYTE	CheckCnt[3];				// 수표거래매수
	BYTE	OneThousand[3]; 			// 천원권거래매수
	BYTE	FiveThousand[3]; 			// 오천원권거래매수
	BYTE	TenThousand[3]; 			// 만원권거래매수
	BYTE	Check10Cnt[3];				// 수표(10만)
	BYTE	Check30Cnt[3];				// 수표(30만)
	BYTE	Check50Cnt[3]; 				// 수표(50만)
	BYTE	Check100Cnt[3]; 			// 수표(100만)
	BYTE	HostError[4]; 				// 호스트에러코드(불능코드)
	BYTE	TranAmount[15];				// 거래금액
	BYTE	LCCode[5]; 					// 장애코드
	BYTE	ErrorInfo[40];				// 장애내용
	BYTE	MTCCode[12];				// 자동화기기업체별장애코드
	BYTE	ProcCount[2]; 				// 진행카운터
	BYTE	TranTime[6]; 				// 거래시각(후인자)
	BYTE	TranFee[11];				// 이용수수료(후인자)
	BYTE	Balance[15];				// 잔액(후인자)
	BYTE	TimeOut;		 			// 무응답여부
	BYTE	Status[10];					// 기기상태구분
	BYTE	Filler[95];					// Filler
} AjmsBody;

typedef struct _AjmsEjrDataTbl
{
	AjmsHead	Head;
	AjmsBody	Body;
} AjmsEjrDataTbl;

#undef ASC2INT_SAFE_STRARRAY
#define ASC2INT_SAFE_STRARRAY(x,y)	(x.GetSize() > y ? Asc2Int(x[y]) : Asc2Int("0"))
#undef ASC2INT_SAFE_STRARRAY_SIZE
#define ASC2INT_SAFE_STRARRAY_SIZE(x,y,z)	(x.GetSize() > y ? Asc2Int(x[y], z) : Asc2Int("0"))

// V09-33-00-#01		ICCM 모듈 적용
// EMV LIB
/////////////////////////////////////////////////////////////////////////////
#define	S_CHECKING			"CHECKING"				// CHECKING
#define	S_SAVINGS			"SAVINGS"				// SAVINGS
#define	S_CREDITCARD		"CREDITCARD"			// CREDIT CARD
#define	S_LOANPAYMENT		"LOANPAYMENT"			// ASY 2008.07.23 V010304-#11A LoanPayment 계좌 추가
#define	S_OTHER				"OTHER"					// OTHER

/////////////////////////////////////////////////////////////////////////////
// #N0264 -------------------------------------------------------
#define _WinATM_OCX_PATH		"C:\\T1ATM\\APP\\MAIN\\WinAtm.ocx"
#define _DevCtrl_DLL_PATH		"C:\\T1ATM\\APP\\MAIN\\DevCtrl.dll"
#define _TranCtrl_DLL_PATH		"C:\\T1ATM\\APP\\MAIN\\TranCtrl.dll"
#define _NetCtrl_DLL_PATH		"C:\\T1ATM\\APP\\MAIN\\NetCtrl.dll"
#define _ProCtrl_DLL_PATH		"C:\\T1ATM\\APP\\MAIN\\ProCtrl.dll"
#define _VolCtrl_DLL_PATH		"C:\\T1ATM\\APP\\MAIN\\VolCtrl.dll"
// ---------------------------------------------------------------------

#endif
