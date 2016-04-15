/////////////////////////////////////////////////////////////////////////////
#if !defined(_DEFINE_H_INCLUDED_)
#define _DEFINE_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// �������
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

#define	REVISION			"R0"						// Version�� Revision�����ʿ�
#define VERSIONDATE			"20160412"					// ���α׷���������


#define ECASH_LOCALPORT_MODE	20015						// �׽�Ʈ ���� �� 


#define LINE_CLEAR			0
#define LINE_OPEN			'O'
#define LINE_CLOSE			'C'
#define LINE_RESET			'R'
#define LINE_DATA			'D'


/////////////////////////////////////////////////////////////////////////////
// USER Name : 2004.01.29
/////////////////////////////////////////////////////////////////////////////
#define USER_KNB			"KNB"					// �泲����
#define USER_KMB			"KMB"					// ��������
#define USER_IBK			"IBK"					// �������
#define USER_NACF			"NAC"					// ����
#define USER_DGB			"DGB"					// �뱸����
#define USER_PSB			"PSB"					// �λ�����
#define USER_KFCC			"KFC"					// �������ݰ�
#define USER_SHB			"SHB"					// ��������
#define USER_WRB			"WRB"					// �츮����
#define USER_MIC			"MIC"					// ������ź�(��ü��)
#define USER_KFB			"KFB"					// ��������
#define USER_BOC			"BOC"					// ��������
#define USER_CHB			"CHB"					// ��������
#define USER_HNB			"HNB"					// �ϳ�����
#define USER_JBB			"JBB"					// ��������
#define USER_TYS			"TYS"					// ��������
#define USER_NIC			"NIC"					// �ѱ����ڱ���

#define USERNAME			USER_TYS				// ��������

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

#define BRM_RECYCLE_MODE	1						// ȯ���ĸ�尪
#define BRM_UNCYCLE_MODE	0						// ��ȯ����尪(���Ұ�)

#define BRM_REALCASH_MODE	254						// ���Ǹ�尪(UCM����)
#define BRM_TESTCASH_MODE	255						// �����尪(UCM����)

#define BRM_MAXCASHOUTCNT	100						// �����ִ����ż�
#define BRM_MAXCHECKOUTCNT	10						// ��ǥ�ִ����ż�
#define BRM_MAXCASHINCNT	100						// �����ִ��Աݸż�
#define BRM_MAXCASHINCNT_150 150					// �������� �귣�� �����ִ��Աݸż� #N0181
#define BRM_MAXMUCASHINCNT	100						// ����ü�����ִ��Աݸż�
#define BRM_MAXCASHINAMT	500						// �ִ��Աݱݾ� (500����)
#define BRM_MAXCASHINAMT_750 750					// ���� �귣�� �ִ��Աݱݾ� (750 ����) //#N0181
#define BRM_MAXCASHINAMT_150 150					// ���� �귣�� �ִ��Աݱݾ� (150 ����) //#N0226
#define UCM_MAXCHECKINCNT	10						// ��ǥ�ִ��Աݸż�
#define BRM_MAXCHECKOUTCNT_100CHECK	6               // ��ǥ�ִ����ż�(�鸸����ǥ������)

#define BRM_FIXCASHCNT		30						// ����10�ź���(H/W)
#define BRM_FIXCHECKCNT		0						// ��ǥ0�ź���(H/W)

													// REJECT�ż��Ѱ���(��������ְ���) : 2002.01.08
#define BRM_EMPTYCASHCNT	(BRM_MAXCASHOUTCNT + BRM_FIXCASHCNT)		
													// ���ݿ亸�氪:20�ź���(H/W)
#define BRM_EMPTYCHECKCNT	(1 + BRM_FIXCHECKCNT)	// ��ǥ�亸�氪:1�ź���(H/W)
#define BRM_NEARCASHCNT		300						// �����ܷ��Ұ� 2004.05.31 :  400 => 300
#define BRM_NEARCHECKCNT	20						// ��ǥ�ܷ��Ұ�

#define BRM_MONEY_MAXOUT	100						// �ִ����ݾ�(��������)
#define BRM_MONEY_MAXOUTCNT	100						// �ִ����ż�
#define BRM_MONEY_MAXOUT_70	70						// �ִ����ݾ�(��������)
#define BRM_MONEY_MAXOUT_50	50						// �ִ����ݾ�(��������)
#define BRM_MONEY_MAXOUT_40	40						// �ִ����ݾ�(��������)
#define BRM_MONEY_MAXOUT_30	30						// �ִ����ݾ�(��������)

#define BRM_CASHINRETRY		3						// �Ա������԰���Ƚ��
#define BRM_UNKNOWNRETRY	1						// �Ա����󰨺�����Ƚ��
#define UCM_UNKNOWNRETRY	5						// �Աݼ�ǥ��������Ƚ��

#define BRM_SELECTCHECKCNT	20						// ��ǥ���ð��ɸż�(��ǥ����ݽ�)

#define BRM_AREFILLCONDCNT	210						// �ڵ��������Ǹż�
#define BRM_AREFILLCNT		350						// �ڵ�����ż�
#define BRM_ATAKEUPCONDCNT	500						// �ڵ�ȸ�����Ǹż�
#define BRM_ATAKEUPCNT		500						// �ڵ�ȸ���ż�
#define BRM_MAXRJB1CNT		150						// �ִ�RJB1�ż�
#define BRM_MAXRJB2CNT		300						// �ִ�RJB2�ż�(����ġ�˼� : 2002.04.19)
#define BRM_MANRJB2CNT		250						// ����RJB2�ż�
#define	BRM_MAXRBCNT		2500					// �ִ�RB�ż�(BOX��)
#define	BRM_MAXRB23CNT		(BRM_MAXRBCNT * 2)		// �ִ�RB2,RB3�ż�
#define	BRM_MAXRB123CNT		(BRM_MAXRBCNT * 3)		// �ִ�RB1,RB2,RB3�ż�

#define BRM_MAXRJB2DEPCNT	150						// RJB2�����Ǹ��Աݰ��ɸż�		: 2003.12.01

//////////////////////////////////////////////////////////////////////////
// OKI-BRM											// T2ATM 2006.5.24 yscho
//////////////////////////////////////////////////////////////////////////
#define OKI_MAXFRJBCNT		200						// �ִ�FRJB�ż�
#define OKI_MAXRJB1CNT		280						// �ִ�RJB1�ż�
#define OKI_MAXRJB2CNT		280						// �ִ�RJB2�ż�
#define OKI_MINRJB1CNT		205						// �ѵ�RJB1�ż�
#define OKI_MINRJB2CNT		205						// �ѵ�RJB2�ż�
#define	OKI_MAXRBCNT		2700					// �ִ�RB���ż�
#define	OKI_REVISECASHCNT	30						// ���ݿ亸��ż�����
#define OKI_MAXCASHOUTCNT   100						// ����ִ�ż�	
#define OKI_MAXCASHINCNT    100						// �Ա��ִ�ż�	
#define OKI_50T_REVISECASHCNT 5						// 5���� �亸��ż�����	
#define OKI_50T_MAXCASHOUTCNT   20					// 5���� ����ִ�ż�
#define OKI_50T_AREFILLCONDCNT	60					// 5���� �����˸��ż�
#define	OKI_50T_MAXRBCNT	2000					// 5���� RB�ż�			

#define HBRM_MAXRJB1CNT		350						// �ִ�RJB1�ż�		
#define HBRM_MINRJB1CNT		300						// �ѵ�RJB1�ż�		
#define HBRM_MAXRJB2CNT		350						// �ִ�RJB2�ż�		
#define HBRM_MINRJB2CNT		300						// �ѵ�RJB2�ż�		

/////////////////////////////////////////////////////////////////////////////
// HOST �ѵ��ݾ�									V02-08-01-R1
/////////////////////////////////////////////////////////////////////////////
#define HOST_MAXWITHAMT		100						// 10������ ��ݽ� MAX

/////////////////////////////////////////////////////////////////////////////
// Operation Mode : CD ONLY
/////////////////////////////////////////////////////////////////////////////
#define CDU_NEARCASHCNT		100						// NEAR CASH COUNT
#define CDU_NEARCASHCNT2	0						// NEAR CASH COUNT

#define CDU_FULLREJECTCNT	200						// FULL REJECT COUNT
#define CDU_FULLREJECTCNT2	100						// FULL REJECT COUNT

#define CDU_MAXCASHOUTCNT	100						// �����ִ����ż�
#define CDU_MAXCHECKOUTCNT	10						// ��ǥ�ִ����ż�

#define CDU_MONEY_MAXOUT	100						// �ִ����ݾ�(��������)

/////////////////////////////////////////////////////////////////////////////
// ��ǥ�������� define								V02-08-01-R1
/////////////////////////////////////////////////////////////////////////////
#define	CHECKKIND_HUNDRED_THOUSAND				13	// 10 �����Ǹ��
#define	CHECKKIND_THREEHUNDRED_THOUSAND			14	// 30 �����Ǹ��
#define	CHECKKIND_FIVEHUNDRED_THOUSAND			15	// 50 �����Ǹ��
#define	CHECKKIND_MILLION						16	// 100�����Ǹ��
#define	CHECKKIND_UNKNOWN						0	// ��Ȯ�θ��

#define	CHECKMONEY_HUNDRED_THOUSAND				"10"// 10 �����Ǹ��
#define	CHECKMONEY_THREEHUNDRED_THOUSAND		"30"// 30 �����Ǹ��
#define	CHECKMONEY_FIVEHUNDRED_THOUSAND			"50"// 50 �����Ǹ��
#define	CHECKMONEY_MILLION						"100"// 100�����Ǹ��
#define	CHECKMONEY_UNKNOWN						"0"	// ��Ȯ�θ��

#define	CHECK_HUNDRED_THOUSAND					10	// 10 �����Ǹ��
#define	CHECK_THREEHUNDRED_THOUSAND				30	// 30 �����Ǹ��
#define	CHECK_FIVEHUNDRED_THOUSAND				50	// 50 �����Ǹ��
#define	CHECK_MILLION							100	// 100�����Ǹ��
#define	CHECK_UNKNOWN							0	// ��Ȯ�θ��

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

#define _REGKEY_SUPERVISOR	_T("SOFTWARE\\ATM\\SUPERVISOR")		// U8100-AP���� : ���� ����(������ OM)

#define	_REGKEY_BRMSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\BRM")	// 20030719_1.0_1053 : ��ġ���� ���
#define	_REGKEY_MCUSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\IDC")	// 20030719_1.0_1053 : ��ġ���� ���
#define	_REGKEY_JPRSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\JPR")	// 20030719_1.0_1053 : ��ġ���� ���
#define	_REGKEY_PBMSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\PBM")	// 20030719_1.0_1053 : ��ġ���� ���
#define	_REGKEY_SIUSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\SIU")	// 20030719_1.0_1053 : ��ġ���� ���
#define	_REGKEY_SPRSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\SPR")	// 20030719_1.0_1053 : ��ġ���� ���
#define	_REGKEY_UCMSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\UCM")	// 20030719_1.0_1053 : ��ġ���� ���
#define	_REGKEY_UPSSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\UPS")	// 20030719_1.0_1053 : ��ġ���� ���

/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_CDUSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\CDM")
#define	_REGKEY_VDMSTATUS	_T("SOFTWARE\\ATM\\DEVSTATUS\\NHVDM")

#define	_REGKEY_DEVINFO		_T("SOFTWARE\\ATM\\DEVINFO")		// 2006.09.08 SHUŸ�� ����
#define	_REGKEY_UPSINFO		_T("SOFTWARE\\ATM\\DEVINFO\\UPS")	// 20030729_1.0_1055 : UPS Power Control
#define	_REGKEY_A4PINFO		_T("SOFTWARE\\ATM\\DEVINFO\\SAU")
#define	_REGKEY_MCUINFO		_T("SOFTWARE\\ATM\\DEVINFO\\IDC")	// 20030719_1.0_1053 : ����IC Version V070300 3.
#define	_REGKEY_BRMINFO		_T("SOFTWARE\\ATM\\DEVINFO\\BRM")	// T2ATM 2006.5.24 yscho (BRM ����� INI -> Registry)
#define	_REGKEY_CMRINFO		_T("SOFTWARE\\ATM\\DEVINFO\\CAM")	// 2006.09.08 ī�޶� Ÿ������
#define	_REGKEY_EMBOSS		_T("SOFTWARE\\ATM\\EMBOSS")
#define _REGKEY_CAMINFO		_T("SOFTWARE\\ATM\\DEVINFO\\CAM")	// V06-02-01-#05:Camera Preview ī�޶� ����̽� ����
#define	_REGKEY_SPRINFO		_T("SOFTWARE\\ATM\\DEVINFO\\SPR")	// V07-05-01-#02:���ù��� ��ɽ� ����üũ ����

#define	_REGKEY_SPVERSION	_T("SOFTWARE\\ATM\\SP_Version")		// 2004.03.03 SP VERSION
#define	_REGKEY_EPVERSION	_T("SOFTWARE\\ATM\\EP_Version")		// 2004.03.03 EP VERSION
#define _REGKEY_MWI _T("SOFTWARE\\ATM\\MWI") 

#define	_REGKEY_ATMRESTART	_T("SOFTWARE\\ATM\\APP\\AtmRestart")
#define	_REGKEY_ATMEACHNET	_T("SOFTWARE\\ATM\\APP\\AtmEachNet")
#define	_REGKEY_ATM_REJMAT	_T("SOFTWARE\\ATM\\REJECTMATERIAL")
#define _REGKEY_REMOTECTRL	_T("SOFTWARE\\ATM\\ReMoteCtrl")
#define	DEVNM_ICVERSION		"IC_Version"						// �ݿ�IC Board Version V070300 3.
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

#define _SYSTEMCDINI		_T("C:\\SYSTEMCD.INI")				// System CD ����

#define OS_WIN7				1									//WIN7 ���� #N0228
/////////////////////////////////////////////////////////////////////////////
//	SP Profile Define
/////////////////////////////////////////////////////////////////////////////
#define _SP_DEVICE_INI		"C:\\T1ATM\\INI\\DEVICE.INI"
#define _SP_CDMINFO_INI		"C:\\T1ATM\\INI\\CDMINFO.INI"
#define _SP_CSTINFO_INI		"C:\\T1ATM\\INI\\CSTINFO.INI"
#define _SP_DISPINFO_INI	"C:\\T1ATM\\INI\\DISPINFO.INI"
#define _LCDINFO_INI		"D:\\INI\\LCDINFO.INI"			// ����LCD����ó��, ����
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
#define	_DEFAULT_CFMONITOR_U8100	_T("C:\\T1ATM\\CFMONITOR\\SWFPLAYER.EXE")	// U8100-AP����


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
// GRO ���� 
// ����̽� : GRO
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
// U8100 AP ���泻�� #02 - ī�޶� ������ ����
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
#define	_SOUND_OM_REJECT	"k-o001.wav"						// 2007.11.27 �Ҹ�� �ȳ����� ����

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
#define ATM_INIT			0						// ����ʱ�ȭ	���
#define ATM_ERROR			1						// ��ֹ߻�		���
#define ATM_CLERK			2						// ���			���
#define ATM_READY			3						// �غ�			���
#define ATM_CUSTOM			4						// �������	��� 
#define ATM_TRAN			5						// ���ŷ���	���
#define ATM_DOWN			6						// DOWN		MODE

/////////////////////////////////////////////////////////////////////////////
// Host Mode Define
/////////////////////////////////////////////////////////////////////////////
#define HOST_OFFLINE 		0						// ���źҰ�
#define HOST_WAITLINK		1						// ���Ŵ��
#define HOST_LINK			2						// ���Ű���
#define HOST_WAITREADY		3						// �غ�ϴ��
#define HOST_READY			4						// �غ��
#define HOST_WAITONLINE		5						// �������
#define HOST_ONLINE			6						// �����Ϸ�
#define HOST_INIT			99						// �����غ�

#define HOST_MAX_TRAN		25						// Bitmap���� �ŷ���

/////////////////////////////////////////////////////////////////////////////
// Tran Mode Define
/////////////////////////////////////////////////////////////////////////////
#define TRAN_IDLE 			0						// �������
#define TRAN_READY			10						// �غ��ó��	(�ŷ�ó��)
#define TRAN_OPEN			11						// ����ó��		(�ŷ�ó��)
#define TRAN_STATUS			12						// ����/���ó��(�ŷ�ó��)
#define TRAN_TRAN			20						// �ŷ�ó��		(�ŷ�ó��)
#define TRAN_SET			30						// ����ġó��					(����ó��)
#define TRAN_ERROR			31						// ���ó��						(����ó��)
#define TRAN_TOTAL			32						// ����ó��		(�ŷ�ó��)
#define TRAN_SUBTOTAL		33						// ������ȸ		(�ŷ�ó��)
#define TRAN_RESET			34						// RESETó��					(����ó��)
#define TRAN_REFILL			35						// BRM����ó��					(����ó��)
#define TRAN_TAKEUP			36						// BRMȸ��ó��					(����ó��)
#define	TRAN_CHECK_REG		38						// ��ǥ���		(�ŷ�ó��)
#define	TRAN_CHECK_UNREG	39						// ��ǥ����		(�ŷ�ó��)
#define	TRAN_CHECK_ALIVE	40						// ��������		(����ó��)
#define TRAN_CASH_REG		41						// ���ݵ��		(�ŷ�ó��)
#define TRAN_CASH_UNREG		42						// ��������		(�ŷ�ó��)
#define TRAN_MSFINGER_CONFIRM	43					// ���+���� 	(�ŷ�ó��)

/////////////////////////////////////////////////////////////////////////////
// �ŷ�ó������
/////////////////////////////////////////////////////////////////////////////
#define TRAN_NULL			0						// �ŷ�ó�������ʱ�ȭ
#define TRAN_WITH			1						// ����
#define TRAN_PB				2						// ��������
#define TRAN_TRANS			3						// ��ü
#define TRAN_DEP			4						// �Ա�
#define TRAN_INQ			5						// ��ȸ
#define TRAN_IC				6						// K-CASH
#define TRAN_RF				7						// RF�ŷ�
#define TRAN_CLERK			8						// ����ŷ�
#define TRAN_NOMEDIA		9						// ����ü
#define TRAN_TICKET			10						// Ƽ��
#define TRAN_UTILYBILLS		11						// ������
#define TRAN_CARDLOANS		12						// ī���
#define TRAN_POINTS			13						// ����Ʈ
#define TRAN_CREDITINQS		14						// �ſ���ȸ
#define TRAN_INTERAD		16						// ���ͷ�Ƽ�� ����
#define TRAN_MOBILEGAME		17						// ����ϰ��� �ٿ�ε�
#define TRAN_LOCALTAX		18						// ���漼 ������
#define TRAN_E_TICKET		20						// ���ڻ�ǰ��  #N0142

#define TRAN_FOREGIN		21						// �ؿ�ī��  #N0171
#define TRAN_FOREGIN_INQ	22						// �ؿ�ī�� ��ȸ #N0171
#define	TRAN_EASYONE		23						// ��ȯ ������ #N0174

#define TRAN_HIPASS			24						// �����н� ���� #N0266

#define TRAN_NOBANK_SERVICE			1						// ����
#define TRAN_NOCREDIT_SERVICE		2						// �ſ�
#define TRAN_BOTH_SERVICE			3						// ���� + �ſ�

#define TRAN_INQFEE_USE 			1						// ��������� Flag

/////////////////////////////////////////////////////////////////////////////
// IC CardType Definitions. 2004.02.11
/////////////////////////////////////////////////////////////////////////////
#define	FIC_NONE			0						// FICī����
#define	FIC_CLOSE			1						// �����
#define FIC_NATIVE			2						// NATIVE(Semi-Open)
#define FIC_OPEN			3						// ������

/////////////////////////////////////////////////////////////////////////////
// IC Work Definitions. 2004.01.31
/////////////////////////////////////////////////////////////////////////////
#define TRAN_IC_FINANCEIC	1						// ���� IC ����
#define TRAN_IC_KCASH		2						// K-CASH ����
#define TRAN_IC_MYBI		3						// MYBI ����
#define TRAN_IC_MONDEX1		4						// Mondex ����
#define TRAN_IC_MONDEX2		5						// Mondex ����
#define TRAN_IC_VCASH		6						// V-CASH ����
#define TRAN_IC_AUTHEN		7						// ������ ����
#define TRAN_IC_ELECPB		8						// �������� ����		// LDC_20040204_A
#define TRAN_IC_HIPASS		10						// �����н� ����		Hi-Pass���˽��������� #N0266

/////////////////////////////////////////////////////////////////////////////
// ����IC �ŷ� Process ����
/////////////////////////////////////////////////////////////////////////////
#define FINANCEIC_PROC_SELECTAPPLICATION	0x0001	// ���� ������ Select Application
#define FINANCEIC_PROC_READBASICINFO		0x0002	// ���� ������ �⺻���� ��ȸ
#define FINANCEIC_PROC_READACCOUNTINFO		0x0003	// ���� ������ �������� ��ȸ
#define FINANCEIC_PROC_READDUALINFO			0x0004	// ���� ������ �⺻���� + �������� ��ȸ
#define FINANCEIC_PROC_GETDATACSN			0x0005	// ���� ������ Get Data CSN. �Ѽ��� �����ڵ� 2004.09.16
#define FINANCEIC_PROC_GETENCIPHER			0x0006	// ���� ������ Get Encipher
#define FINANCEIC_PROC_GETENCDATA1			0x0007	// ���� ������ Get ENCData 1
#define FINANCEIC_PROC_GETCHALLENGE			0x0008	// ���� ������ Get Challenge
#define FINANCEIC_PROC_GETENCDATA2			0x0009	// ���� ������ Get ENCData 2

#define FINANCEIC_PROC_CHECKPIN				0x0011	// ����ȭ�� Verify PIN
#define FINANCEIC_PROC_VERIFYTRANBIT		0x0012	// ����ȭ�� Verify TRAN_BIT
#define FINANCEIC_PROC_READPAN				0x0013	// ����ȭ�� Read PAN File
#define FINANCEIC_PROC_READBAMOUNT			0x0014	// ����ȭ�� Read JAN File
#define FINANCEIC_PROC_CHANGEPIN			0x0015	// ����ȭ�� Change PIN
#define FINANCEIC_PROC_READTRAFFICINFO		0x0016	// ����ȭ�� ����������
#define FINANCEIC_PROC_PREUNLOADRCV			0x0017	// ����ȭ�� ȯ�Һ�����ó��
#define FINANCEIC_PROC_GETICTYPE			0x0018	// ����IC/����ȭ�� Get IC Chip Type 2004.11.03

#define FINANCEIC_PROC_EPBREADACCOUNTINFO	0x0019	// e-������� �������� ��ȸ(���༺/����)	V05-00-02-#01 e-��������������1
#define FINANCEIC_PROC_EPBREADACCOUNTINFO2	0x001A	// e-������� �Աݰ�����������			V05-00-02-#01 e-��������������2

/////////////////////////////////////////////////////////////////////////////
// EMV �ŷ� Process ����										V03-00-26-#01
/////////////////////////////////////////////////////////////////////////////
#define EMV_PROC_CALISTSET					0x01	// CA Key Set ����(������)
#define EMV_PROC_TERMINALLISTSET			0x02	// �ܸ��� ���� AID List ����
#define EMV_PROC_TERMINALCONSTSET			0x03	// �ܸ��� ���� �⺻ ������ ��� ����
#define EMV_PROC_SELECTPSE					0x04	// EMV �ŷ� ���� ���θ� PSE�� ���� ��� Ȯ��
#define EMV_PROC_REQCANDIDATELIST			0x05	// ī��κ��� EMV �ŷ��� ������ �ĺ� ����� ��û
#define EMV_PROC_SELECTAPPLICATION			0x06	// �ĺ� ��� �� ���õ� Application���� �ŷ� ����
#define EMV_PROC_READAPPLICATION			0x07	// �ŷ��� ���� Application�� ������ ����
#define EMV_PROC_OFFLINEDATAAUTHEN			0x08	// �������� ���� �䱸
#define EMV_PROC_ONLINE						0x09	// �¶��� ���� ��û�� ���� ȣ��Ʈ ������ ī��� ����
#define EMV_PROC_COMPLET					0x0A	// ī��� �ŷ� ���� ��û
#define EMV_PROC_AFTERSCRIPT				0x0B	// �ŷ��� �����ϱ� ���� ��ũ��Ʈ ó���� ��û
#define EMV_PROC_GETDOLDATA					0x0C	// AP�� �ŷ� �� �ʿ��� ������ ��ü�� EMV Kernel�� ��û
#define EMV_PROC_RESULTDATA					0x0D	// �ŷ� ���� �� ȭ������ ������ EMV �ŷ� ��� �������� ��û�ϴ��Լ�(������)

/////////////////////////////////////////////////////////////////////////////
// RFD �ŷ� Process ����
/////////////////////////////////////////////////////////////////////////////
#define RFD_PROC_BASICINFO					0x01	// RF�⺻���� ���ϱ�
#define RFD_PROC_REQCHARGE					0x02	// RF����
#define	RFD_PROC_DRAWBACK					0x03	// RFȯ��
#define	RFD_PROC_UNKNOWN					0x04	// ��Ÿ���

/////////////////////////////////////////////////////////////////////////////
// HiPass �ŷ� Process ����		#N0266
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
// ����
/////////////////////////////////////////////////////////////////////////////
// Timer����
/////////////////////////////////////////////////////////////////////////////
#define	TIMER_mSEC1000		1000					// Timer���

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
#define MMAX_DEVRSP_TIME	1800					// ��ġ�����ִ�
#define MAX_DEVRSP_TIME		365						// ��ġ�����ִ�
#define MID_DEVRSP_TIME		125						// ��ġ�����߰�(����Time���ٴ�Ŀ�ߵ� : 2003.10.29)
#define MIN_DEVRSP_TIME		5						// ��ġ�����ּ�
#define SPR_DEVRSP_TIME		65						// VAN ���� TIMEOUT ���� (65��) //#N0158
#define MID_ICCMRCV_TIME	130						// ICCM���� �ð�(MCU Timeout���ٴ� Ŀ�ߵ�)

/////////////////////////////////////////////////////////////////////////////
// �ŷ� ���� ID
/////////////////////////////////////////////////////////////////////////////
#define TRAN_WITH_NOT		0x00
#define TRAN_WITH_CASH		0x01
#define TRAN_WITH_CHECK		0x02
#define	TRAN_WITH_50TCASH	0x08													// V06-01-01-#01 5�����Ǹ� ��ݰ���
#define TRAN_WITH_2CASH		(TRAN_WITH_CASH | TRAN_WITH_50TCASH)					// V06-01-01-#01 (����+5����) ��ݰ���
#define TRAN_WITH_CASHCHECK (TRAN_WITH_CASH | TRAN_WITH_CHECK)						// V06-01-01-#01 (����+��ǥ) ��ݰ���
#define TRAN_WITH_50TCASHCHECK (TRAN_WITH_50TCASH | TRAN_WITH_CHECK)				// V06-01-01-#01 (5����+��ǥ) ��ݰ���
#define TRAN_WITH_BOTH		(TRAN_WITH_CASH | TRAN_WITH_50TCASH | TRAN_WITH_CHECK)	// V06-01-01-#01 (����+5����+��ǥ) ��ݰ���

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
// ��ǥ �� ���� HEADER ���� 
/////////////////////////////////////////////////////////////////////////////
#define HEADER_NORMAL		0x00					// ��ǥ �� ���� HEADER ����
#define	SLIP_HEAD_UP		0x01					// ��ǥ HEAD UP
#define	JNL_HEAD_UP			0x10					// ���� HEAD UP

/////////////////////////////////////////////////////////////////////////////
// ��/��
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
// ����IC Board ���� ���� 2004.09.18 V070300
/////////////////////////////////////////////////////////////////////////////
#define CMI_BOARD				0
#define EMV_BOARD				1

/////////////////////////////////////////////////////////////////////////////
// Brm ���� �̵� ����
/////////////////////////////////////////////////////////////////////////////
#define REFILL_MOVE			0x01
#define TAKEUP_MOVE			0x02

#define BRM_AUTOLOAD		0x10
#define BRM_MANUALLOAD		0x20
#define BRM_RESET			0x40
#define BRM_LOST			0x80

/////////////////////////////////////////////////////////////////////////////
// �ŷ�������
/////////////////////////////////////////////////////////////////////////////
//#define HAN_MODE			0						// ����
//#define ENG_MODE			1						// ����
//#define JP_MODE				2						// �Ϲ�
//#define CHN_MODE			3						// �߹�

/////////////////////////////////////////////////////////////////////////////
//	Language Mode
/////////////////////////////////////////////////////////////////////////////
#define KOR_MODE			0						// KOREA
#define ENG_MODE			1						// ENGLISH
#define CHN_MODE			2						// CHINA
#define JPN_MODE			3						// JAPAN
#define FOR_MODE			4						// �ؿ�ī�� #N0171
#define KEB_MODE			5						// ������  #N0174


/////////////////////////////////////////////////////////////////////////////
// TRANCTRL RETURN
/////////////////////////////////////////////////////////////////////////////
#define T_OK				0						// ó��OK
#define T_CANCEL			-1						// ��Ҽ���
#define T_TIMEOVER			-2						// �ð��ʰ�
#define T_INPUTOVER			-3						// �Է�Ƚ���ʰ�
#define T_INPUTERROR		-4						// �Է��ڷ��̻�
#define T_ERROR				-5						// �����ֹ߻�(�����ָ޼���)
#define T_SENDERROR			-6						// �۽���ֹ߻�(�ܾ�Ȯ�θ޼���)
#define T_RECVERROR			-7						// ������ֹ߻�(�ܾ�Ȯ�θ޼���)
#define T_DISMSG			-97						// ���α׷��̻���ֹ߻�(����ι���)
#define T_PROGRAM			-98						// ���α׷��̻���ֹ߻�(����ι���)
#define T_MSG				-99						// ��ҹ߻�(�޼���ó��)
#define T_HOSTMSG			-100					// ��ҹ߻�(SVR �޼���ó��)

/////////////////////////////////////////////////////////////////////////////
// ȭ�� ��ư����
/////////////////////////////////////////////////////////////////////////////
#define S_INPUTOVER			"INPUTOVER"				// �Է�Ƚ���ʰ�
#define S_TIMEOVER			"TIMEOVER"				// �ð��ʰ�
#define S_CANCEL			"���"					// ���
#define S_CANCEL2			"CANCEL"				// ���
#define S_CLEAR				"CLEAR"					// CLEAR

#define S_CONFIRM			"Ȯ��"					// Ȯ��
#define S_CONFIRM2			"OK"					// Ȯ��
#define S_CORRECTION		"����"					// ����
#define S_EXTRA				"��Ÿ"					// ��Ÿ
#define S_EXIT				"����"					// ����
#define S_DEPOSIT			"�߰��Ա�"				// �߰��Ա�
#define	S_YES				"��"					// ��
#define	S_NO				"�ƴϿ�"				// �ƴϿ�
#define S_SHUTTERCLOSE		"����"					// ����
#define S_REGIST			"���"					// ���
#define S_NEXT				"����"					// ����
#define S_PREV				"����"					// ����
#define S_CONTINUE			"���"					// ���
#define S_PRINT				"���"					// ���

/////////////////////////////////////////////////////////////////////////////
// KeyCode Define
/////////////////////////////////////////////////////////////////////////////
#define K_MAN				138						// ��(10)
#define K_MANWON			K_MAN					// ����
#define K_CHEN				139						// õ(11)
#define K_WON				140						// ��(12)
#define K_CONFIRM			141						// Ȯ��(13)
#define K_CANCEL			142						// ���(14)
#define K_CORRECTION		147						// ����(19)

#define	K_BACKSPACE			161						//
#define	K_ENTER				162						//
#define	K_HOME				163						//
#define	K_END				164						//
#define	K_PGUP				165						// ����
#define K_BEFORE			K_PGUP					//
#define	K_PGDN				166						// ����
#define	K_NEXT				K_PGDN					//
#define	K_LEFT				167						// <<
#define	K_RIGHT				168						// >>
#define	K_UP				169						//
#define	K_DOWN				170						//
#define	K_ESC				171						//
#define	K_EXIT				K_ESC					// ����
#define	K_TAB				172						//
#define	K_CHOICE			173						// ����
#define	K_EXEC				174						// ����
#define	K_WRITE				175						// ���
#define	K_PRINT				176						// ����
#define K_CHANGE			178						// 2003.05.31 Nam ������������#2�� ����/���� ��ȯŰ �߰� for T1ATM

#define K_OK				0						// Ȯ��
#define K_KEY_EMPTY			-1						// ����
#define K_TIMEOVER			-2						// �ð��ʰ�

/////////////////////////////////////////////////////////////////////////////
// ������ ������ ����� ����, ��ǥ�ص� ����� ��Ÿ���µ� ����Ѵ�.
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
// OM_RESULT ����
/////////////////////////////////////////////////////////////////////////////
#define OM_DO				"DO"					// �����û
#define OM_NORMAL			"NORMAL"				// ��������
#define OM_ERROR			"ERROR"					// �������

/////////////////////////////////////////////////////////////////////////////
//	Pin Input Mode
//  V03-01-01	T1     : ������� ����
//				TTW    : ���ŷ�ȭ�鿡�� �ٻ����
//				SCD(P) : ��й�ȣ �Է½ÿ��� ��� - DUMMY MODE�߰�ó��
/////////////////////////////////////////////////////////////////////////////
#define	PIN_DISABLE_MODE	1
#define	PIN_MENU_MODE		2
#define	PIN_NUMERIC_MODE	3
#define	PIN_PASSWORD_MODE	4
#define	PIN_AMOUNT_MODE		5
#define	PIN_INPUT_MODE		6
#define	PIN_ALL_MODE		7
#define	PIN_DUMMY_MODE		8									// V03-01-01 PINPAD DUUMYó��
#define	PIN_PASSWORD_MODE2	9
#define	PIN_PASSWORD_MODE3	10									// V02.C4.04 2010.01.13
#define	PIN_MENU_MODE1		11
#define	PIN_MENU_MODE2		12                                  // ��ü�ݾ� �Է�

#define	PIN_MENU_PAD		"CANCEL,F1,F2,F3,F4,F5,F6,F7,F8"
#define	PIN_MENU_PAD1		"CANCEL,ENTER,F1,F2,F3,F4,F5,F6,F7,F8"
#define	PIN_NUMERIC_PAD		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_PASSWORD_PAD	"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_PASSWORD_PAD2	"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR"
#define	PIN_PASSWORD_PAD3	"1,2,3,4,5,6,7,8,9,0"
#define	PIN_AMOUNT_PAD		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F12"
#define	PIN_INPUT_PAD		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F1,F2,F3,F4,F5,F6,F7,F8"
#define	PIN_ALL_PAD			"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F12"
#define	PIN_CANCEL_PAD		"CANCEL"							// V03-01-01 PANPAD DUMMYó��

#define	PIN_MENU_TOUCH		"CANCEL"
#define	PIN_NUMERIC_TOUCH	"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_PASSWORD_TOUCH	"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_AMOUNT_TOUCH	"1,2,3,4,5,6,7,8,9,0,00,CANCEL,CLEAR,ENTER"
#define	PIN_INPUT_TOUCH		"1,2,3,4,5,6,7,8,9,0,CANCEL,CLEAR,ENTER"
#define	PIN_ALL_TOUCH		"1,2,3,4,5,6,7,8,9,0,.,00,000,CANCEL,CLEAR,ENTER"


// --------------------------------------------------------------------------------
// Printer(Receipt/Journal/Passbook) ���� Media/Form/Field name����
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
#define		FORM_PBOOKBARCODE				_T("PbookBarCode")		// 20030709_1.0_1049 : ������ڵ� �б�� 

#define		FIELD_PBOOKREAD					_T("ReadData")
#define		FIELD_PBOOKWRITE				_T("RawData")
#define		FIELD_PBOOKBARCODE				_T("ReadBarCode")		// 20030709_1.0_1049 : ������ڵ� �б�� 
/////////////////////////////////////////////////////////////////////////////
//	SCDP SPL LED DEFINE											// V03-01-01
/////////////////////////////////////////////////////////////////////////////
#define SPLLED_POLL			0						// ���� 		(CD��)
#define SPLLED_WITHDRAW		1						// ���			(CD��)
#define SPLLED_DOING_TRAN	2						// �ŷ���		(CD��)
#define SPLLED_PSBK_TRAN	3						// ���� �ŷ�	(CD��)

#define SPLLED_SLIP_LOW		4						// ��ǥ (�ܷ�)(CD��)
#define SPLLED_JNL_LOW		5						// ����   (�ܷ�)(CD��)
#define SPLLED_CST1_LOW		6						// ī��Ʈ1(�ܷ�)(CD��)
#define SPLLED_CST2_LOW		7						// ī��Ʈ2(�ܷ�)(CD��)
#define SPLLED_CST3_LOW		8						// ī��Ʈ3(�ܷ�)(CD��)

#define SPLLED_CARD_MAT		9						// ī��   (��ü)(CD��)
#define SPLLED_SLIP_MAT		10						// ��ǥ (��ü)(CD��)
#define SPLLED_JNL_MAT		11						// ����   (��ü)(CD��)
#define SPLLED_PSBK_MAT		12						// ����   (��ü)(CD��)
#define SPLLED_NOTES_MAT	13						// ����	  (��ü)(CD��)

#define	SPLLED_CASH_CNT		14						// KEY_���ݸż�	(CD��)
#define	SPLLED_OPEN_CLOSE	15						// KEY_����	(CD��)
#define	SPLLED_KEY_7		SPLLED_OPEN_CLOSE
#define	SPLLED_MAGAM_INQ	16						// KEY_������ȸ	(CD��)
#define	SPLLED_KEY_8		SPLLED_MAGAM_INQ
#define	SPLLED_MAGAM		17						// KEY_�����ŷ�	(CD��)
#define	SPLLED_KEY_9		SPLLED_MAGAM
#define	SPLLED_CHECK_CNT	18						// KEY_��ǥ�ż�	(CD��)
#define	SPLLED_KEY_4		19						// KEY_4		(CD��)
#define	SPLLED_PB_ENDIS		20						// KEY_����		(CD��)
#define	SPLLED_KEY_5		SPLLED_PB_ENDIS
#define	SPLLED_SOUND		21						// KEY_����		(CD��)
#define	SPLLED_KEY_6		SPLLED_SOUND

#define	SPLLED_ON_CHAR		'1'						// LED_ON
#define	SPLLED_OFF_CHAR		'0'						// LED_OFF

/////////////////////////////////////////////////////////////////////////////
//	SCDP SPL SWITCH(KEY) DEFINE									// V03-01-01 W R B
////////////////////////////////////////////////////////////////////////////////////  
#define	SPLKEY_CASH_CNT		1						// ���ݸż�		(CD��)	�̻��
#define	SPLKEY_OPEN_CLOSE	2						// ����		(CD��)
#define	SPLKEY_MAGAM_INQ	3						// ������ȸ		(CD��)	�̻��
#define	SPLKEY_MAGAM		4						// �����ŷ�		(CD��)	�̻��
#define	SPLKEY_CHECK_CNT	5						// ��ǥ�ż�		(CD��)	�̻��
#define	SPLKEY_4			6						// 4			(CD��)	�̻��
#define	SPLKEY_PB_ENDIS		7						// ����			(CD��)	������
#define	SPLKEY_6			8						// 6			(CD��)	����ȭ��
#define	SPLKEY_ATM_TOTAL	9						// �Ұ�			(CD��)
#define	SPLKEY_RESET		10						// RESET		(CD��)
#define	SPLKEY_2			11						// 2			(CD��)	�̻��
#define	SPLKEY_GRAND_TOTAL	12						// �հ�			(CD��)

/////////////////////////////////////////////////////////////////////////////
// A4 Printer Form Data
/////////////////////////////////////////////////////////////////////////////
#define A4Prt_TranList			"A4Form_TranList"		// Form Image Data(�ŷ�������ȸ)			2007.06.12(��)
#define A4Prt_DepSecurList		"A4Form_DepSecurList"	// Form Image Data(��������ȸ&����������ȸ)	2007.06.12(��)
#define A4Prt_InqStock   		"A4Form_InqStock"		// Form Image Data(û�೻������)	            2008.04.02(��)
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

// U8100 AP ���泻�� #05 - LED��� ����
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


// ���� ������
#define	BEFORE_SHUTDOWN		1						// ������
#define	AFTER_SHUTDOWN		2						// ������
#define	HOLIDAY				3						// ����
#define	DELAY_SERVICE		4						// ���񽺿�ü
#define	INSURTRAN_NEXTFLOW	5						// ����
#define	DES_TRAN			9						// �ؿܰŷ�

// ������ ����
#define	CVS_GS24			1						// GS
#define	CVS_FM24			2						// FM
#define	E_MART_EVERYDAY		4						// E-Mart EveryDay


#define LOCK_STAT_CHECK_NONE                    (0)
#define LOCK_STAT_CHECK_OK		                (1)
#define LOCK_STAT_CHECK_NG                      (2)

/////////////////////////////////////////////////////////////////////////////
// ī������
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
	BYTE FormatCode[2]; 							// 000.	�����ڵ�		01
	BYTE IdNo[2]; 									// 002. �����ڵ�		59
	BYTE BankNo[3];									// 004. �����ڵ�		BBB
	BYTE Separator1;								// 007. �и���ȣ		=
	BYTE AccountNo[16];								// 008. ������ȣ+���¹�ȣ
	BYTE AccountCheck;								// 024. ������ȣ
	BYTE Separator2;								// 025. �и���ȣ		=
	BYTE Nation[3]; 								// 026. �����ڵ�		410
	BYTE CashCode[3];								// 029. ȭ���ڵ�		410
	BYTE Unknown1[15];								// 032.
	BYTE ValidCode;									// 047. ��ȿ���ڵ� 
	BYTE Password[6];								// 048. ��й�ȣ 
	BYTE Trade;										// 054. ��ȣ
	BYTE Tran1[2];									// 055. ����1
	BYTE Tran2[4];									// 057. ����2
	BYTE OtherData[27];								// 061. ��Ÿ������
	BYTE Remark[16];								// 088. �����ڷ�
	BYTE Dummy[152];								// 104.	(256)
} MCISO3;

/////////////////////////////////////////////////////////////////////////////
// ��������
/////////////////////////////////////////////////////////////////////////////
/*
typedef struct	tagPBMSForm
{
	BYTE BankLine[10];								// 000. ��ũ����
	BYTE AccountNo[13];								// 010. ���¹�ȣ
	BYTE Dummy1[6];									// 023. dummy
	BYTE BankCode[2];								// 029. �����ڵ�
	BYTE PbNumer[8];								// 031. �����ȣ
	BYTE Dummy2[6];									// 039. dummy
	BYTE Filler[211];								// 045. (256)
~} PBMS;
*/
/*
typedef struct	tagPBMSForm
{
	BYTE Filler1[4];		// FixValue
	BYTE BankCode[3];		// ȸ���ڵ�
	BYTE Filler2;			// FixValue
	BYTE AccountNo[11];		// ���¹�ȣ
	BYTE Filler3[2];		// FixValue
	BYTE YYYY[4];			// ����
	BYTE IssurNum[3];		// �߱�ȸ��
	BYTE MMDD[4];			// ����
	BYTE CHeckBit[4];		// üũ��Ʈ
	BYTE HHMMSS[6];			// �ú���
	BYTE Filler4[2];		// FixValue
} PBMS;
*/
/////////////////////////////////////////////////////////////////////////////
// ��������
/////////////////////////////////////////////////////////////////////////////
typedef struct	tagPBMSForm
{
	BYTE AccountNo[17];								// 000. ���¹�ȣ
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
	int	 mTotalAmount;								// ȸ���� �ѱݾ�
	WORD mTenThousand;								// ȸ���� �������� �ż�
	WORD mFiveThousand;								// ȸ���� ��õ������ �ż�
	WORD mOneThousand;								// ȸ���� õ������ �ż�
	WORD mFiftyThousand;							// ȸ���� ���������� �ż�							// V06-01-01-#01
	WORD mCheck;									// ȸ���� ��ǥ�� �ż�
	WORD mUnknown;									// ȸ���� ������ �������� ���� ������ �ż�
} RejectMoney;

/////////////////////////////////////////////////////////////////////////////
// RetInformation Area
/////////////////////////////////////////////////////////////////////////////
typedef struct tagRET_INFORMATION 
{
	int	Status;										// ���� ���� ����
	WORD ActNum;									// ���� ����/ȸ���� �ż�
	WORD RJTNum;									// reject box�� ȸ���� �� �ż�
	WORD TenThRJTNum;								// ����/ȸ�� ������ RJB�� ȸ���� �������� �ż�
	WORD FiftyThRJTNum;								// ����/ȸ�� ������ RJB�� ȸ���� ���������� �ż�	// V06-01-01-#01
	WORD FiveThRJTNum;								// ����/ȸ�� ������ RJB�� ȸ���� ��õ������ �ż�
	WORD OneThRJTNum;								// ����/ȸ�� ������ RJB�� ȸ���� õ������ �ż�
	WORD CheckRJTNum;								// ����/ȸ�� ������ RJB�� ȸ���� ��ǥ�� �ż�
	WORD UnknownRJTNum;								// ����/ȸ�� ������ RJB�� ȸ���� �Ҹ���� �ż�
} RetInformation;


/////////////////////////////////////////////////////////////////////////////
// DepositMoney Area
/////////////////////////////////////////////////////////////////////////////
typedef	struct tagDEPOSIT_MONEY 
{
	int  mStatus;									// ������������ ��������
	int  mTotalAmount;								// ����� �ѱݾ�
	WORD mFiftyThousand;							// ����� ���������� �ż�							// V06-01-01-#01
	WORD mTenThousand;								// ����� �������� �ż�
	WORD mFiveThousand;								// ����� ��õ������ �ż�
	WORD mOneThousand;								// ����� õ������ �ż�
	WORD mUnknown;									// ����� ������ �������� ���� ������ �ż�
} DepositMoney;

/////////////////////////////////////////////////////////////////////////////
// CheckInfo Area
/////////////////////////////////////////////////////////////////////////////
typedef	struct tagCHECK_INFO 
{
	BYTE ReadData[8];								// �Աݵ� ��ǥ�� ��ȣ
	BYTE Bank[3];									// V05-06-03-#03:�����ڵ�3�ڸ�����	// �Աݵ� ��ǥ�� �������� CODE
	BYTE Branch[4];									// �Աݵ� ��ǥ�� �������� CODE
	BYTE Kind[2];									// �Աݵ� ��ǥ�� ����
	BYTE Amount[10];								// �Աݵ� ��ǥ�� �ݾ�
	BYTE AccountNum[6];							// V05-01-06-#02:��ǥ���¹�ȣ
} CheckInfo;

/////////////////////////////////////////////////////////////////////////////
// CheckPrint Area
/////////////////////////////////////////////////////////////////////////////
typedef char CheckPrint[256];						// ��ǥ�輭 ����

/////////////////////////////////////////////////////////////////////////////
// MENU Header
/////////////////////////////////////////////////////////////////////////////
typedef struct tagMENUHeader
{
	char	MenuScrName[81];						// �޴�ȭ���̸�
	int		MenuScrNum;								// �޴�ȭ���ȣ
	char	MenuKind[22][81];						// �޴�(��ư)����(�̸�_
	int		MenuInfo[22][7];						// �޴�����
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
	char	Date[8];								// �ŷ�����				// 9
	char	Fill2;									// Filler7				// 80
	char	Time[12];								// �ŷ��ð�				// 18
	char	Fill3;									// Filler7				// 80
	char	TranMaterial[10];						// �ŷ���ü				// 81
	char	Fill4;									// Filler8				// 91
	char	Dummy[34];								// Dummy				// 92
	char	Cr;										// Cr					// 126
	char	Lf;										// Lf					// 127[128]

													// Data Area(1920 BYTE)
//	char	Data[SPR_TICKET_MAX_ROW][JPR_MAX_COL + 3];		// Ejr����('|'+Cr+Lf����)		
//	char	DataDummy[2048 - 128 - (SPR_TICKET_MAX_ROW * (JPR_MAX_COL + 3)) - 2];		//2048->1028�� 2007 12/20 ������ �������� ���� �����̽�����	

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
	char	SerialNo[8];							// �����Ϸù�ȣ			// 0
	char	Fill1;									// Filler1				// 8
	char	Date[10];								// �ŷ�����				// 9
	char	Fill2;									// Filler2				// 17
	char	Time[8];								// �ŷ��ð�				// 18
	char	Fill3;									// Filler5				// 24
	char	AccountNum[20];							// ���¹�ȣ				// 25
	char	Fill4;									// Filler3				// 45
	char	TranSerialNo[6];						// �ŷ��Ϸù�ȣ			// 46
	char	Fill5;									// Filler4				// 50
	char	TranAmount[12];							// �ŷ��ݾ�				// 51
	char	Fill6;									// Filler6				// 59
	char	KeyNum[20];								// Ű��ȣ				// 60
	char	Fill7;									// Filler7				// 80
	char	TranMaterial[10];						// �ŷ���ü				// 81
	char	Fill8;									// Filler8				// 91
	char	Dummy[160];								// Dummy				// 92
	char	Cr;										// Cr					// 252
	char	Lf;										// Lf					// 123[254]
} NEjrTblDump;

typedef struct tagEEjrTblDump					    // Total Area(256 BYTE)
{
	char	Fill1;									// Filler7				// 80
	char	Date[8];								// �ŷ�����				// 9
	char	Fill2;									// Filler7				// 80
	char	Time[12];								// �ŷ��ð�				// 18
	char	Fill3;									// Filler7				// 80
	char	TranMaterial[10];						// �ŷ���ü				// 81
	char	Fill4;									// Filler8				// 91
	char	Dummy[160];								// Dummy				// 92
	char	Cr;										// Cr					// 252
	char	Lf;										// Lf					// 123[254]
} EEjrTblDump;

/////////////////////////////////////////////////////////////////////////////
// ElecJournal �ݾװ����ڷ�
/////////////////////////////////////////////////////////////////////////////
typedef struct tagEjrAmountTbl
{
	int		TranProc;								// ����ݱ���
	int		TranAmount;								// ����ݰŷ��ݾ�
	int		Cash10000Num;							// ����ݸ����Ǹż�
	int		Cash5000Num;							// ����ݿ�õ���Ǹż�
	int		Cash1000Num;							// �����õ���Ǹż�
	int		Cash50000Num;							// ����ݿ������ż�										// V06-01-01-#01
	int		MotherCheck10Num;						// ����������ǥ10�����Ǹż�
	int		MotherCheck30Num;						// ����������ǥ30�����Ǹż�
	int		MotherCheck50Num;						// ����������ǥ50�����Ǹż�
	int		MotherCheck100Num;						// ����������ǥ100�����Ǹż�
	int		OtherCheck10Num;						// �����Ÿ���ǥ10�����Ǹż�
	int		OtherCheck30Num;						// �����Ÿ���ǥ30�����Ǹż�
	int		OtherCheck50Num;						// �����Ÿ���ǥ50�����Ǹż�
	int		OtherCheck100Num;						// �����Ÿ���ǥ100�����Ǹż�
} EjrAmountTbl;


// ����ȭ�� ȣ��Ʈ���� Flag
#define STOPBITREMOVE		1						// 0xF1 = STOP BIT ������������
#define PARAMCHANGE			2						// 0xF2 = �Ķ���� ���Űŷ��䱸
#define HSOTSIGNOK			3						// 0xF3 = �ŷ� ���� �Ϸ�
#define PARAMCHANGEWANT		4						// 0xF4 = �Ķ���� ���� ��������
#define BANKICKEYCHANGE		5						// 0xF5 = ����IC Ű���� ���Űŷ��䱸
#define BANKICKEYCHANGEWANT	6						// 0xF6 = ����IC Ű���� ����������������
#define KCASHPINREG			7						// 0xF7 = ����ȭ��PIN��ϰŷ�

//////////////////////////////////////////////////////////////////////
//	AJMS���� Define
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
// ���ξ��ⱸ��
//////////////////////////////////////////////////////////////////////
#define AJMS_JOURNL			0x01
#define AJMS_PAPER			0x02
#define AJMS_BOTH			(AJMS_JOURNL | AJMS_PAPER)

//////////////////////////////////////////////////////////////////////
// �����ͱ���
//////////////////////////////////////////////////////////////////////
#define	DATA_JNL			'1'						// ����
#define	DATA_SENDRECV		'2'						// ����
#define	DATA_IMG_FACE		'3'						// ȭ���̹���
#define DATA_IMG_EMIT		'4'						// ���ⱸ�̹���
#define	DATA_EMBOSS			'5'						// ī���̹���

#define	AJMS_TYPE_JNL		0x01					// ����
#define	AJMS_TYPE_SENDRECV	0x02					// ����
#define	AJMS_TYPE_IMG_FACE	0x04					// ȭ���̹���
#define AJMS_TYPE_IMG_EMIT	0x08					// ���ⱸ�̹���
#define	AJMS_TYPE_EMBOSS	0x10					// ī���̹���

//////////////////////////////////////////////////////////////////////
// ������������
//////////////////////////////////////////////////////////////////////
#define	JNL_OPEN			1						// ��������
#define	JNL_TRAN			2						// �ŷ�����
#define	JNL_CANCEL			3						// ��Ұŷ�����
#define	JNL_STATUS			4						// ��������
#define	JNL_CLOSE			5						// ��������
#define	JNL_OPER			6						// �����
#define	JNL_CASH			7						// ��������
#define	JNL_ERROR			8						// �������

#define	ATM_CPUMEMINFO		3						// �ڵ��� (CPU,MEM) ����

//////////////////////////////////////////////////////////////////////
// �޼�������
//////////////////////////////////////////////////////////////////////
#define ERROR_HDD			"HDDERROR"				// HDD���
#define ERROR_ENCRYPTFAIL	"ENCRYPTFAIL"			// ��ȣȭ���
#define ERROR_OPEN			"OPEN"					// ����
#define ERROR_TRANEND		"TRANEND"				// �ŷ�����

//////////////////////////////////////////////////////////////////////
// 2007.12.14 msj��ġ���� �� ���� Define
#define EXE_TOUCH	_T("C:\\PROGRAM FILES\\TPDRV\\TPDRV.EXE")	
#define EXE_TOUCH_PATH	_T("C:\\PROGRAM FILES\\TPDRV\\")			//DUAL TOUCH DRIVER

#define EXE_DUAL_TOUCH	_T("C:\\PROGRAM FILES\\TPDUAL_DRV\\TPDUAL_DRV.EXE")	
#define EXE_DUAL_TOUCH_PATH	_T("C:\\PROGRAM FILES\\TPDUAL_DRV\\")			//DUAL TOUCH DRIVER
 

#define EXE_OSK			_T("%SYSTEMROOT%\\SYSTEM32\\OSK.EXE")				
#define EXE_TMFA		_T("C:\\TM\\VDM\\TMFA.EXE")							 
#define EXE_VDMFA		_T("C:\\TM\\VDM\\VDMFA.EXE")	
#define EXE_ATMINSIDE	_T("C:\\Program Files\\Ubiator20\\EXE\\ATMInside.exe")						 

// ������ ���� Lnk����
#define	EXE_TOUCH_LNK1	_T("C:\\Documents and Settings\\Administrator\\���� �޴�\\���α׷�\\�������α׷�\\tpdrv.LNK")
#define	EXE_TOUCH_LNK2	_T("C:\\Documents and Settings\\All Users\\���� �޴�\\���α׷�\\�������α׷�\\tpdrv.LNK")
#define	EXE_TOUCH_LNK3	_T("C:\\Documents and Settings\\Default User\\���� �޴�\\���α׷�\\�������α׷�\\tpdrv.LNK")
#define	EXE_TOUCH_LNK4	_T("C:\\Documents and Settings\\vancd3\\���� �޴�\\���α׷�\\�������α׷�\\tpdrv.LNK")
#define	EXE_TOUCH_LNK5	_T("C:\\Documents and Settings\\HSCUSER\\���� �޴�\\���α׷�\\�������α׷�\\tpdrv.LNK")

// ������ ���� Lnk����
#define	EXE_DUAL_TOUCH_LNK1	_T("C:\\Documents and Settings\\Administrator\\���� �޴�\\���α׷�\\�������α׷�\\tpdual_drv.Lnk")
#define	EXE_OSK_LNK1		_T("C:\\Documents and Settings\\Administrator\\���� �޴�\\���α׷�\\�������α׷�\\���� �ʿ��� �ɼ�\\ȭ�� Ű����.Lnk")

#define	EXE_DUAL_TOUCH_LNK2	_T("C:\\Documents and Settings\\All Users\\���� �޴�\\���α׷�\\�������α׷�\\tpdual_drv.Lnk")
#define	EXE_OSK_LNK2		_T("C:\\Documents and Settings\\All Users\\���� �޴�\\���α׷�\\�������α׷�\\���� �ʿ��� �ɼ�\\ȭ�� Ű����.Lnk")

#define	EXE_DUAL_TOUCH_LNK3	_T("C:\\Documents and Settings\\Default User\\���� �޴�\\���α׷�\\�������α׷�\\tpdual_drv.Lnk")
#define	EXE_OSK_LNK3		_T("C:\\Documents and Settings\\Default User\\���� �޴�\\���α׷�\\�������α׷�\\���� �ʿ��� �ɼ�\\ȭ�� Ű����.Lnk")

#define	EXE_DUAL_TOUCH_LNK4	_T("C:\\Documents and Settings\\T1ATM\\���� �޴�\\���α׷�\\�������α׷�\\tpdual_drv.Lnk")
#define	EXE_OSK_LNK4		_T("C:\\Documents and Settings\\T1ATM\\���� �޴�\\���α׷�\\�������α׷�\\���� �ʿ��� �ɼ�\\ȭ�� Ű����.Lnk")

#define	EXE_DUAL_TOUCH_LNK5	_T("C:\\Documents and Settings\\HSCUSER\\���� �޴�\\���α׷�\\�������α׷�\\tpdual_drv.Lnk")
#define	EXE_OSK_LNK5		_T("C:\\Documents and Settings\\HSCUSER\\���� �޴�\\���α׷�\\�������α׷�\\���� �ʿ��� �ɼ�\\ȭ�� Ű����.Lnk")

#define	EXE_NEXTWARE_LNK	_T("C:\\Documents and Settings\\All Users\\���� �޴�\\���α׷�\\Nextware")
#define	EXE_UBIATOR_LNK 	_T("C:\\Documents and Settings\\All Users\\���� �޴�\\���α׷�\\Ubiator")

#define	EXE_ETC_LNK1		_T("C:\\Documents and Settings\\All Users\\���� �޴�\\���α׷�\\�������α׷�")
#define	EXE_ETC_LNK2 		_T("C:\\Documents and Settings\\Administrator\\���� �޴�\\���α׷�\\�������α׷�")
#define	EXE_ETC_LNK3 		_T("C:\\Documents and Settings\\Default User\\���� �޴�\\���α׷�\\�������α׷�")



// 2010.05.13 Yun.H.J ��ü ����� �귣�� ȭ�� ����
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
// ��Ÿ� ���� (LGT, KTF, KT_ADSL, KT_VDSL, NANARO
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
#define MI_READY			0x60					// �غ��
#define MI_KEYSHARE			0x61					// Ű����
#define MI_TRAN				0x41					// �Ϲݰŷ�
#define MI_OPEN				0x41					// ����
#define MI_STAT				0xa8					// ����
#define MI_ERROR			0xa3					// ���
#define MI_SNOD				0x50					// SNOD
#define MI_READY_OFF		0x22					// �غ�� OFF
#define MI_CLOSE			0xa2					// ��

//////////////////////////////////////////////////////////////////////////
// 2007.06.11 for ATMS Define
#define AGT2ATM_MEM_SIZE	516
#define ATM2AGT_MEM_SIZE	256

#define ATMS_CMD_IDLE		0
#define ATMS_CMD_POLL		1
#define ATMS_CMD_INIT		2
#define ATMS_CMD_REBOOT		3  //������ ���� -> ���� (����)
#define ATMS_CMD_JNLMODE	4
#define ATMS_CMD_DOWNLOAD	5
#define ATMS_CMD_REBOOT1	6  //������ ���� -> HOST ����
#define ATMS_CMD_CAMMODE	7
#define ATMS_CMD_REBOOT2	8  //������ ���� -> �������
#define ATMS_CMD_REBOOT3	9  //������ ���� -> ����� ���� 
#define ATMS_CMD_UPLOAD  	10
#define ATMS_CMD_REBOOT4	11 //������ ���� -> ��� ���� 
#define ATMS_CMD_REBOOT5	12 //������ ���� -> AMS ���� ������
#define ATMS_CMD_REBOOT6	13 //������ ���� -> AMS ���� POWER OFF 
#define ATMS_CMD_REBOOT7	14 //���� �� ���¿����� ���Ͽ� ������
#define ATMS_CMD_REOPEN		15 //���� �� ���¿����� ���Ͽ� ������
#define ATMS_CMD_RESTATUS	17 


#define TRAN_TXRX_STATUS	10
#define TRAN_TXRX_ERROR  	20


#define ATMS_JNL_HDD		0									//�������θ��
#define ATMS_JNL_MIX		1									//ȥ�ո��(����+��������)
#define ATMS_JNL_PAPER		2									//�������θ��


#define DVR_CAPTURE_FACE		0								//DVR ��
#define DVR_CAPTURE_HAND		1								//DVR ��
#define DVR_CAPTURE_DOOR		2								//DVR �ݰ��
#define DVR_CAPTURE_ETC0		3								//DVR ��ü

#define PIDILE       	1
#define SENDING      	10
#define SENDING_OK     	11
#define RECVING      	20
#define RECVING_OK     	21


//////////////////////////////////////////////////////////////////////////

// �����Ϸù�ȣ���� �����
typedef struct _tagAjmsHead
{
	BYTE	JnlSerial[12];				// ���������Ϸù�ȣ
	BYTE	DataFlag;					// �����ͱ���
										// 1 : ����
										// 2 : ����
	BYTE	StepNum1[12];				// STEP�Ϸù�ȣ1
	BYTE	StepNum2[12];				// STEP�Ϸù�ȣ2
	BYTE	JnlFlag;					// �������������ڵ�
	BYTE	TranFlag;					// ��������
	BYTE	DirInfo[64];				// ���丮����
	BYTE	YYMMDD[8];					// �߻�����
	BYTE	HHMMSS[6];					// �߻��ð�
	BYTE	BranchNum[3];				// �������ڵ�
	BYTE	AtmNo[2];					// �ܸ���ȣ
	BYTE	AtmIp[15];					// ATM IP
	BYTE	JnlLineCnt[2];				// ���ζ��μ�
	BYTE	FileCreate;					// ���ϻ�������		(��:1, ��:0)
	BYTE	PaperWrite;					// �����������ڿ��� (��:1, ��:0)
	BYTE	Encryt;						// ��ȣȭ����		(��:1, ��:0)
} AjmsHead;

// �����Ϸù�ȣ���� �����ͺ�
typedef struct _tagAjmsBody
{
	BYTE	OpenTime[6];				// �����ð�
	BYTE	DevSerial[16];				// ����Ϸù�ȣ
	BYTE	ModelName[10];				// �𵨸�
	BYTE	TranFlag[2];				// �ŷ������ڵ�
	BYTE	TranDetailFlag[2];			// �ŷ����л�
	BYTE	TranDate[8];				// �ŷ������
	BYTE	TranSerial[4];				// �ŷ���ȣ
	BYTE	TranCode[4];	 			// �ŷ��ڵ�
	BYTE	MaterialInfo[2]; 			// �ڵ�ȭ����̿��ü�����ڵ�
	BYTE	MSData[25];					// �ڵ�ȭ���MS����
	BYTE	MotherBank[2]; 				// ��ü���������ڵ�
	BYTE	OtherBank[2];	 			// ��������ڵ�
	BYTE	AccountM[16];				// �ŷ����¹�ȣ(ī���ȣ)
	BYTE	AccountT[16];				// �����¹�ȣ(ī���ȣ)
	BYTE	CashCnt[3];					// ���ݰŷ��ż�
	BYTE	CheckCnt[3];				// ��ǥ�ŷ��ż�
	BYTE	OneThousand[3]; 			// õ���ǰŷ��ż�
	BYTE	FiveThousand[3]; 			// ��õ���ǰŷ��ż�
	BYTE	TenThousand[3]; 			// �����ǰŷ��ż�
	BYTE	Check10Cnt[3];				// ��ǥ(10��)
	BYTE	Check30Cnt[3];				// ��ǥ(30��)
	BYTE	Check50Cnt[3]; 				// ��ǥ(50��)
	BYTE	Check100Cnt[3]; 			// ��ǥ(100��)
	BYTE	HostError[4]; 				// ȣ��Ʈ�����ڵ�(�Ҵ��ڵ�)
	BYTE	TranAmount[15];				// �ŷ��ݾ�
	BYTE	LCCode[5]; 					// ����ڵ�
	BYTE	ErrorInfo[40];				// ��ֳ���
	BYTE	MTCCode[12];				// �ڵ�ȭ����ü������ڵ�
	BYTE	ProcCount[2]; 				// ����ī����
	BYTE	TranTime[6]; 				// �ŷ��ð�(������)
	BYTE	TranFee[11];				// �̿������(������)
	BYTE	Balance[15];				// �ܾ�(������)
	BYTE	TimeOut;		 			// �����俩��
	BYTE	Status[10];					// �����±���
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

// V09-33-00-#01		ICCM ��� ����
// EMV LIB
/////////////////////////////////////////////////////////////////////////////
#define	S_CHECKING			"CHECKING"				// CHECKING
#define	S_SAVINGS			"SAVINGS"				// SAVINGS
#define	S_CREDITCARD		"CREDITCARD"			// CREDIT CARD
#define	S_LOANPAYMENT		"LOANPAYMENT"			// ASY 2008.07.23 V010304-#11A LoanPayment ���� �߰�
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
