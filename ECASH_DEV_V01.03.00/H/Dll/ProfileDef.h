/////////////////////////////////////////////////////////////////////////////
#if !defined(_Profiledef_H_)
#define _Profiledef_H_

// Initial Profile의 Argument
#define DEV_INI 		 1
#define NETWORK_INI 	 2
#define TRANS_INI  		 3
#define AD_INI  		 4

// Section Name
#define DEV_SEC			"DEVICE"
#define BRM_SEC			"BRM"
#define UCM_SEC			"UCM"
#define NET_SEC			"NETWORK"
#define TRANS_SEC		"TRANS"
#define MAIN_AD_SEC		"MAINAD"
#define INTERACTIVE_AD_SEC	"INTERACTIVEAD"
#define RECEIPT_AD_SEC	"RECEIPTAD"
#define BANNER_AD_SEC	"BANNERAD"

#define BRM_OPMODE		"OPMODE"

#define MAIN_SEC		"MAIN_DATA"
#define ERR_SEC			"ERROR_STACK"

#define MAINC_SEC		"MAIN_COUNT"
#define DETAILS_SEC		"DETAIL_STACK"

#define TRANMAIN_SEC	"MAIN_TRAN"
#define TRANERR_SEC		"ERROR_TRAN"
#define CANCELMAIN_SEC	"MAIN_CANCEL"
#define CANCELERR_SEC	"ERROR_CANCEL"
#define HOSTMAIN_SEC	"MAIN_HOST"
#define HOSTERR_SEC		"ERROR_HOST"
#define KCASHMAIN_SEC	"MAIN_KCASH"
#define KCASHERR_SEC	"ERROR_KCASH"

#define WITHTOTAL_SEC	"WITHTOTAL"
#define TRANSTOTAL_SEC	"TRANSTOTAL"
#define DEPTOTAL_SEC	"DEPTOTAL"
#define MONEYTOTAL_SEC	"MONEYTOTAL"							
#define ECASHTOTAL_SEC	"ECASHTOTAL"				
#define DEPERRTOTAL_SEC	"DEPERRTOTAL"				
#define WITHERRTOTAL_SEC "WITHERRTOTAL"				


#define LCDINFO_SEC		"LCDINFO"	// U8100 AP 변경내역 #03 - 광고LCD위젯처리, 변경

// Machine Type
// V03-01-01	SCDP SRC통합시 기종코드 변경
//#define ATM			1
//#define CDP			2
//#define CD			3
//#define TTW			4
#define	THBRM			16			// HBRM
#define	U8100			71			// U8100  - AP변경
#define	U8500			U8100		// U8500  - AP변경
#define	U3100K			81			// U3100K - AP변경 (BRM21)

// SHU Type
#define SHU_MECHA_O		1			// 1 저널, 1 명세 지원 SHU (O TYPE) - SHU-O
#define SHU_MECHA_HS	2			// 2 저널, 2 명세 지원 SHU (O TYPE) - H-SHU
#define SHU_MECHA_NT	3			// 1 저널, 1 명세 지원 SHU (NT TYPE)- NT-SHU
#define SHU_MECHA_S		4			// 1 저널, 1 명세 지원 SHU (S TYPE)
#define SHU_MECHA_S2	5			// 2 저널, 2 명세 지원 SHU (S TYPE)
#define SHU_MECHA_T		6			// 1 저널, 1 명세 지원 SHU (T TYPE) - T-SHU
#define SHU_MECHA_A2	7			// 2 저널, 2 명세 지원 SHU (O TYPE) - SHUA2O

// Journal Destination
#define JNL_A			'A'
#define JNL_B			'B'

// MCU Type
#define O_TYPE			1
#define S_TYPE			2
#define D_TYPE			3

// Device
#define NOT_SET			0
#define BRM_SET			1
#define CDU_SET			2
#define OKI_SET			3			// T2ATM 2006.5.24 yscho
#define MINI_OKI_SET	4			// T3ATM 2009.7.22 Kim.Gi.Jin
#define HBRM_SET		4			// U8100 AP 변경내역 #04 - HBRM관련 변경내역

#define CAM_SET			1
#define UCM_SET			2
#define PBM_SET		 	1			// AUTO TURN
#define PBM_NAT_SET 	2			// NO	AUTO	TURN
#define I_PBM_SET 		3			// U8100 AP 변경내역 #07 - IPBM 소모품 잔여정보 지원

#define SOUND_SET		1
#define PC_SOUND_SET	2			// PC 스피커 (사운드카드 없을 경우)
#define PCI_CAMERA_SET	1			// PCI Type의 카메라 설치됨
#define USB_CAMERA_SET	2			// USB Type의 카메라 설치됨
#define HUB_CAMERA_SET	3			// HUB USB Type의 카메라 설치됨
#define DVR_CAMERA_SET	4			// DVR Type의 카메라 설치됨
#define TENKEY_SET		1
#define HOTKEY_SET		2			// 적용후 1차 수정 (U8100-AP변경 #13)
#define ICCARD_SET		1
#define FINGER_SET		1
#define IRDA_SET		1
#define BAR_SET			1
#define SEED_SET		1
#define VFD_SET			1
#define LED_SET			2			// U8100 AP 변경내역 #05 - LED기능 지원
#define RFD_SET			1
#define EAR_SET			1			// #N0193 장애인ATM
#define ZOOM_SET		1			// #N0193 장애인ATM

#define A4P_SET			1
#define GIRO_SET		1
#define	AJMS_SET		1

#define	FNG_SET			1
#define	RPCLOCK_SET1	1
#define	RPCLOCK_SET2	2
#define	SPACK_SET		1

#define	FIFTY_THOUSAND_USE		1
#define	NOTE_KIND_50000_USE		0x34

// Key type
#define DRIVER_TYPE		1
#define TOUCH_TYPE		2

// Sound Volume
#define SOUND_OFF		0
#define SOUND_ONE		1
#define SOUND_TWO		2
#define SOUND_THREE		3
#define SOUND_MAX		4

// Power type
#define PNC_TYPE		1
#define UPS_TYPE		2

// Des type
#define D_DES_SET		1
#define T_DES_SET		2
#define S_DES_SET		D_DES_SET

// Des TMK Set Mode : 2003.12.02
#define DES_TMK_SET		1

// Lcp Card Type
#define LCP_NOT_INST	0
#define	LCP_SBC_LCPBD	1
#define	LCP_PCI_DCPBD	2

// protocol type												// 2003.12.01

#define NET_IOC			1			// For 제일/한일/상업 은행
#define NET_SWP			2			// For 주택/보람/하나/서울/기업 은행
#define NET_CCL			3			// For 농협,수협,조흥
#define NET_SDLC		4			// For 상업은행
#define NET_TCPIP		5			// For 광주/산업 은행 
#define NET_HDLC		6			// For 조흥/동화 은행
#define NET_BAS			7			// For PUSAN BANK
#define	NET_TMAX		8			// For 기업/신한

// Network
#define L1200			0			// LINE SPEED : 0:1200,1:2400,2:4800,3:9600,4:19200
#define L2400			1	
#define L4800			2	
#define L9600			3	
#define L19200			4	
#define POLL_ON			0
#define	POLL_OFF		0x01
#define PTP				0
#define MTP				0x80
#define HALF_DUPLEX		0
#define FULL_DUPLEX		0x40
#define MTF				0
#define FTF				0x10
#define RS422			0
#define RS232C			0x04
#define NRZ				0
#define NRZI			0x02
#define INCLK			0
#define EXCLK			0x01

/////////////////////////////////////////////////////////////////////////////
typedef struct tagDeviceInfo{
	int		MachineType;			// 0:UNKNOWN,		1:ATM, 2:CDP, 3:CD, 4:TTW
	int		SHUHandler;				// 0:NOT INSTALL,	1:O, 2:A2, 3:T, 4:S
	char	JournalDest[2];			// A:Journal A,     B:Journal B
	int		MCUHandler;				// 0:NOT INSTALL,	1:O_TYPE, 2:S_TYPE
	int		CashHandler;			// 0:NOT INSTALL,	1:BRM, 2:CDU
	int		CheckHandler;			// 0:NOT INSTALL,	1:CAM, 2:UCM
	int		PBHandler;				// 0:NOT INSTALL,	1:LPBM, 2:LPBM(NO AUTO TURN) 
	int		KeyHandler;				// 0:NOT INSTALL,	1:DRIVER_TYPE, 2:TOUCH_TYPE
	int		SoundDevice;			// 0:NOT INSTALL,	1:INSTALL
	int		SoundVolume;			// 0:NOT SET,		1, 2, 3, MAX
	int		CameraDevice;			// 0:NOT INSTALL,	1:PCI CAMERA   2:USB CAMERA
	int		TenKeyHandler;			// 0:NOT INSTALL,	1:INSTALL
	int		ICCardDevice;			// 0:NOT INSTALL,	1:INSTALL
	int		PowerHandler;			// 0:NOT INSTALL,	1:PNC_TYPE, 2:UPS_TYPE
	int		DesHandler;				// 0:NOT INSTALL,	1:D_DES BOARD, 2:T_DES BOARD
	int		DesMasterKey;			// 0:NOT SET,		1:TMK SET
	int		FingerHandler;			// 0:NOT INSTALL,	1:INSTALL
	int		IrdaHandler;			// 0:NOT INSTALL,	1:INSTALL
	int		BarHandler;				// 0:NOT INSTALL,	1:INSTALL
	int		SeedHandler;			// 0:NOT INSTALL,	1:INSTALL						: 2003.11.22
	int		KCashHandler;			// 0:K-CASH자행 ,	1:K-CASH타행
	int		VfdHandler;				// 0:NOT INSTALL,	1:INSTALL
	int		RfdHandler;				// 0:NOT INSTALL,	1:INSTALL
	int		A4PHandler;				// 0:NOT INSTALL,	1:INSTALL
	int		GiroHandler;			// 0:NOT INSTALL,	1:INSTALL
	int		AjmsHandler;			// 0:NOT INSTALL,	1:INSTALL
	int		ETCDevice01;			// Reserverd0
	int		ETCDevice02;
	int		ETCDevice03;
	int		ETCDevice04;
	int		ETCDevice05;
	int		ETCDevice06;
	int		ETCDevice07;
	int		ETCDevice08;
	int		ETCDevice09;
	int		ETCDevice10;
	int		ETCDevice11;
	int		ETCDevice12;
	int		ETCDevice13;
	int		ETCDevice14;
	int		ETCDevice15;
	int		ETCDevice16;
	int		ETCDevice17;
	int		ETCDevice18;
	int		ETCDevice19;
	int		ETCDevice20;
	int		EarHandler;				// 0:NOT INSTALL,	1:INSTALL 이어폰	
	int		Proximity;				// 0:NOT INSTALL,	1:INSTALL 근접센서	
	int		SightHandicapped;		// 0:NOT SET,		1:SET     저시력자	

} DEVICEINFO;

typedef struct tagBRMInfo{
	WORD	CrntCashCnt;			// 현재 현금 매수
	int		DepositCashKind;		// 입금권종선택(만원권만 가능: 1, 전권종가능 : 0)
	int		PriorityNote;			// V06-01-01-#01	
} BRMINFO;

typedef struct tagUCMInfo{
	WORD	CrntInCheckCnt;			// 현재 입금수표 매수
	char	BankNo[3];				// 은행코드
	char	CrntOutCheckNo[9] ;		// 현재 출금수표 번호
	WORD	CrntOutCheckCnt;		// 현재 출금수표 매수
	char	BankNo2[3];				// 은행코드
	char	CrntOutCheckNo2[9] ;	// 현재 출금수표 번호
	WORD	CrntOutCheckCnt2;		// 현재 출금수표 매수
	WORD	CrntOutCheckKind;		// 현재 출금수표 권종
} UCMINFO;

typedef struct tagNetworkInfo{
	char	AtmSerialNum[9];		// 기번
	char	BranchNum[9];			// 점번(소속지점2)
	char	SubBranchNum[9];		// 부점(소속지점1)
	char	RoomNum[9];				// 배치순서(처리지점)
	char	MachineMakeDate[9];		// 제조년도
	char	MachineMakeNum[21];		// 제조번호
	int		BranchInOut;			// 점내/외
	int		AutoBranchOnOff;		// 무인점포
	int		All24OnOff;				// 24On/Off(24/365운영 On/Off)
	int		All365OnOff;			// 365On/Off(365CASH On/Off)
	int		CheckOnOff;				// CheckOn/Off(수표탑재 On/Off:AP SET)
	char	CeOfficeCode[5];		// CE사무소코드
	char	OutBranchNum[9];		// 출장소
	char	InstName[11];			// 설치자
	char	InstConfirmName[11];	// 설치확인자
	char	CeName[11];				// 보수담당자
	char	CeTel[17];				// 보수담당자연락처
	
	int		Interface;				// IOC, HDLC, SWP, SDLC, TCPIP, CCL, BAS
	char	BpCurIpAddress[16];		// BP current Ip Address
	char	BpCurPortNo[6];			// BP current Port No
	char	BpIpAddress[16];		// BP Ip Address
	char	BpPortNo[6];			// BP Port No
	char	BpIpAddress2[16];		// BP Ip Address2
	char	BpPortNo2[6];			// BP Port No2
	char	BpIpAddress3[16];		// BP Ip Address3
	char	BpPortNo3[6];			// BP Port No3
    char	AtmIPAddress[16];		// Atm IP Address
    char	AtmSubnetMask[16];		// Atm Subnet Mask
    char	AtmGateWay[16];			// Atm GateWay

	BYTE	DeviceAddr;				// DA
	BYTE	SelectAddr;				// SA 
	int		SendRecvSize;			// 송/수신 SIZE
	int		LineSpeed;				// LINE SPEED
	int		PollCheck;				// POLL CHECK ON/OFF
	int		Point;					// PTP, MTP
	int		Duplex;					// HALF_DUPLEX, FULL_DUPLEX
	int		TimeFiller;				// MTF, FTF
	int		RsType;					// RS-422, RS-232C
	int		Nrz;					// NRZ, NRZI
	int		Clk;					// EXCLK, INCLK
} NETWORKINFO;

typedef struct tagTransInfo{
 	char	ApVersion[41];			// AP VERSION
	char	YYYYMMDD[9];			// 일자 
	char	YYYYMMDDHHMMSS[15];		// 일자시간 
	char	SerialNo[7];			// 거래일련번호 
	char	StartSerialNo[7];		// 시작일련번호
	BYTE	ClerkTransOnOff;		// 계원이 조작 거래 모드
	char	ProcCount[2];			// 진행 Count
	int		TransPowerOnOff;		// 거래중 POWER OFF
	char	EjrSerialNo[9];			// Ejr일련번호 
	char	HostSerialNo[13];		// Ejr일련번호 
	int		MagamFlg;				// 마감
	int		DeviceExecOff;			// 불가축퇴가 난 Device = Set
	char	DeviceYYYYMMDD[9];		// 장애난일자 
	int		DeviceSprCnt;			// Spr장애 Cnt
	int		DeviceJprCnt;			// Jpr장애 Cnt
	int		DeviceMcuCnt;			// Mcu장애 Cnt
	int		DeviceBrmCnt;			// Brm장애 Cnt
	int		DeviceUcmCnt;			// Ucm장애 Cnt
	int		DevicePbmCnt;			// Pbm장애 Cnt
	int		DeviceA4pCnt;			// A4장애 Cnt
	int		DeviceCshCnt;			// 현금비수취 횟수
	char	CheckYYYYMMDD[9];		// 수표인자일자
	char	AsPassWord[7];			// 
	char	OpPassWord[7];			// 
	char	GuideMsg1[41];			// 안내1
	char	GuideMsg2[41];			// 안내2
	char	GuideMsg3[41];			// 안내3
	char	IngGuideMsg1[81];		// 처리중안내1				
	char	IngGuideMsg2[81];		// 처리중안내2
	char	IngGuideMsg3[81];		// 처리중안내3
	char	IngGuideMsg4[81];		// 처리중안내4
	char	IngGuideMsg5[81];		// 처리중안내5
	int		EMVSerialNo;			// EMV 거래일련번호			
	int		AjmsMode;				// AJMS모드값
	int		ZoomFlag;				// 장애인ATM
	int		JaMaxDispAmt;			//
	int		TaMaxDispAmt;			//
	int		TranService;			// 
	int		TranInqFee;				// 
	char	RebootHHMMSS[7];		// 
	int		RebootFlag;				// 장애발생시 리부팅 처리 


} TRANSINFO; 


typedef struct tagLCDInfo{
	int		LCDType;
	char	LCDDisplay[71];
	char	LCDADInfo[64];
	int		LCDCameraPos;
	int		LCDVolume;				
} LCDINFO;



typedef struct tagECASHTOTALINFO{	// 
	char	sCashWith[11];			// 현금지급액
	char	sCashDep[11];			// 현금입금액
	char	sCheckWith[11];			// 수표지급액
	char	sCheckDep[11];			// 수표입금액
	char	sBalance[11];			// 현금잔액
	char	End;					// Data의 끝
} ECASHTOTALINFO;


// 메인/인터렉티브 광고
typedef struct tagMainADItem{
	CString	ADCmd;			// AD Command
 	CString ADSize;			// AD Size
 	CString ADPath;			// AD Path
 	CString	StartDate;		// Start Date
 	CString	EndDate;		// End Date
 	CString	AllowList;		// Allow List
 	CString	DenyList;		// Deny List
} MAINADITEM;

typedef struct tagInteractiveADItem{
	CString	ADCmd;			// AD Command
 	CString ADPath1;		// AD Path1
 	CString	ADPath2;		// AD Path2
 	CString	StartDate;		// Start Date
 	CString	EndDate;		// End Date
 	CString	AllowList;		// Allow List
 	CString	DenyList;		// Deny List
} INTERACTIVEADITEM;

typedef struct tagReceiptADItem{
	CString	ADVersion;		// AD Version
 	CString	StartDate;		// Start Date
 	CString	EndDate;		// End Date
 	CString	AllowList;		// Allow List
 	CString	DenyList;		// Deny List
 	CStringArray ReceiptLine;	// ReceiptLine
} RECEIPTADITEM;

typedef struct tagBannerADItem{
 	CString	StartDate;		// Start Date
 	CString	EndDate;		// End Date
 	CString	AllowList;		// Allow List
 	CString	DenyList;		// Deny List
 	int		BrandOrg;		// Brand Org
 	CString	TargetCode;		// Target Code
 	CString	ADName;			// AD Name
} BANNERADITEM;

typedef struct tagADInfo{
	CString	MainADVersion;						// Main AD Version
	int		MainADSize;							// Main AD Size
	int		MainADMaxSize;						// Main AD Max Size
	int		MainADStartIndex;					// Main AD Start Index
	CString	MainADList;							// Main AD List
	CArray<MAINADITEM *, MAINADITEM *>  MainADItemArray;
	CString	InteractiveADVersion;				// Interactive AD Version
	int		InteractiveADSize;					// Interactive AD Size
	int		InteractiveADMaxSize;				// Interactive AD Max Size
	int		InteractiveADStartIndex;			// Interactive AD Start Index
	CArray<INTERACTIVEADITEM *, INTERACTIVEADITEM *>  InteractiveADItemArray;
	int		InteractiveADFlag;					// Interactive AD Flag
	RECEIPTADITEM ReceiptAD;					// Receipt AD
	int		ReceiptADFlag;						// Receipt AD Flag
	CString	BannerADVersion;					// Banner AD Version
	int		BannerADSize;						// Banner AD Size
	int		BannerADMaxSize;					// Banner AD Max Size
	int		BannerADStartIndex;					// Banner AD Start Index
	CString	BannerBrandADOrgList;				// Banner AD BrandAD Org List
	CString	BannerTargetADCodeList;				// Banner AD TargetAD Code List
	CArray<BANNERADITEM *, BANNERADITEM *> BannerADItemArray;	// Banner AD
	CArray<BANNERADITEM *, BANNERADITEM *> BannerBrandADItemArray;	// Banner AD
	CArray<BANNERADITEM *, BANNERADITEM *> BannerTargetADItemArray;	// Banner AD
	int		BannerADIncIndexFlag;				// Banner AD Increase Index Flag
} ADINFO; 


typedef struct tagiDkeyDEVICE{
	char *name;
}iDkeyDEVICE;

typedef struct tagiDkeyBRM{
	char *name;
}iDkeyBRM;

typedef struct tagiDkeyUCM{
	char *name;
}iDkeyUCM;

typedef struct tagiDkeyNet{
	char *name;
}iDkeyNET;

typedef struct tagiDkeyTrans{
	char *name;
}iDkeyTRANS;

// U8100 AP 변경내역 #03 - 광고LCD위젯처리, 변경
typedef struct tagiDKeyLcd{
	char *name;
}iDkeyLCD;

typedef struct tagiDkeyAD{
	char *name;
}iDkeyAD;

/////////////////////////////////////////////////////////////////////////////
// ErrStack
/////////////////////////////////////////////////////////////////////////////
#define  ErrStack_MaxNumber   256	// 장애 STACK TABLE 최대 저장 숫자
#define  ErrSDeTailHost_MaxNumber  20	// 장애 STACK TABLE 최대 저장 숫자

typedef struct tagMainInfo{			// 장애 STACK TABLE 관리정보 
	int  Loop;						// 환영정보
	int  CrtCount;					// 위치정보
	int  StackCount;				// 누적정보
} MAININFO;

typedef struct tagErrorInfo{		// 장애 STACK 관련 정보 
	char Date[11];					// 날짜
	char Time[9];					// 시간
	char ProcCount[2];				// 진행카운터
	char ErrorCode[8];				// 장애코드
	char ErrorString[81];			// 장애설명
} ERRSTACK;

typedef struct tagPrintERRSTACK{
	char DateTime_Serial_Amt[40];
	char TranType_Account[40];
	char ErrType_Result[40];
} PrintERRSTACK;

typedef struct tagMainName{
	char *name;
} MainName;

typedef struct tagErrorName{
	char *name;
} ErrorName;

typedef struct tagMainNameDeTailH{
	char *name;
} MainNameDH;

typedef struct tagErrorNameDeTailH{
	char *name;
} ErrorNameDH;

/////////////////////////////////////////////////////////////////////////////
// ErrHost
/////////////////////////////////////////////////////////////////////////////
#define  ErrHost_MaxNumber   256	// 장애 HOST TABLE 최대 저장 숫자

typedef struct tagHErrInfo{			// 장애 HOST TABLE 관리정보(HOST)
	int  Loop;						// 환영정보
	int  Count;						// 위치정보
} HERRINFO;

typedef struct tagDErrInfo{			// HOST 장애 거래 정보 
    char Remark[81];				// 내용
} DERRINFO;

typedef struct tagHErrName{
	char *name;
} HErrName;

typedef struct tagDErrName{
	char *name;
} DErrName;

/////////////////////////////////////////////////////////////////////////////
// Total
/////////////////////////////////////////////////////////////////////////////
typedef struct tagLogWFormat{		// 지급
	char	Count[10];				// 건수
	char	Amount[10];				// 총액
	char	Dummy;					// NULL
} LOGW;

typedef struct tagWITHTOTALINFO{	// 지급 합.소계 정보
	LOGW    DATA[11];				// 0:마감전현금출금
									// 1:마감전수표출금
									// 2:마감전출금합계
									// 3:마감전출금취소
									// 4:마감전대체
									// 5:마감후현금출금
									// 6:마감후수표출금
									// 7:마감후출그합계
									// 8:마감후출금취소
									// 9:마감후대체
									//10:DUMMY
} WITHTOTALINFO;

typedef struct tagLogDFormat{		// 입금 합.소계 정보
	char	Count[10];				// 건수
	char	Amount[10];				// 총액
	char	Dummy;					// NULL
} LOGD;

typedef struct tagDEPTOTALINFO{		// 입급 합.소계 정보
	LOGD	DATA[9];				// 0:마감전현금입금
									// 1:마감전자행수표입금금액
									// 2:마감전타행수표입금금액
									// 3:DUMMY
									// 4:마감후현금입금
									// 5:마감후자행수표입금금액
									// 6:마감후타행수표입금금액
									// 7:DUMMY
									// 8:DUMMY
} DEPTOTALINFO;

typedef struct tagLogTFormat{		// 지급
	char	Count[10];				// 건수
	char	Amount[10];				// 총액
	char	Dummy;					// NULL
} LOGT;

typedef struct tagTRANSTOTALINFO{	// 이체 합.소계 정보
	LOGT    DATA[5];				// 0:마감전이체금액
									// 1:마감후이체금액
									// 2:DUMMY
} TRANSTOTALINFO;

typedef struct tagMONEYTOTALINFO{	// 실출금합계
	LOGT	DATA[3];				// 0:현금방출총
									// 1:수표방출총액
									// 2:Dummy
} MONEYTOTALINFO;

typedef struct tagDEPERRTOTALINFO{	// 입금장애 합계
	LOGT	DATA[5];				// 0:입금통신장애총액      
									// 1:입금계수장애총액
									// 2:입금수납장애총액
	                                // 3:입금미수취총액        
	                                // 4:DUMMY
} DEPERRTOTALINFO;

typedef struct tagWITHERRTOTALINFO{	// 입금장애 합계
	LOGT	DATA[5];				// 0:출금통신장애총액      
									// 1:출금계수장애총액
									// 2:출금수납장애총액
	                                // 3:출금미수취총액        
	                                // 4:DUMMY
} WITHERRTOTALINFO;

typedef struct tagWITHNAME{
	char *name;
} WithName;

typedef struct tagDEPNAME{
	char *name;
} DepName;

typedef struct tagTRANSNAME{
	char *name;
} TransName;

typedef struct tagMONEYNAME{
	char *name;
} MoneyName;

typedef struct tagNICENAME{
	char *name;
} NiceName;

typedef struct tagDEPERR{
	char *name;
} DepErrName;

typedef struct tagWITHERR{
	char *name;
} WithErrName;

/////////////////////////////////////////////////////////////////////////////
#endif