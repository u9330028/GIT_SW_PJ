/////////////////////////////////////////////////////////////////////////////
#if !defined(_Profiledef_H_)
#define _Profiledef_H_

// Initial Profile�� Argument
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


#define LCDINFO_SEC		"LCDINFO"	// U8100 AP ���泻�� #03 - ����LCD����ó��, ����

// Machine Type
// V03-01-01	SCDP SRC���ս� �����ڵ� ����
//#define ATM			1
//#define CDP			2
//#define CD			3
//#define TTW			4
#define	THBRM			16			// HBRM
#define	U8100			71			// U8100  - AP����
#define	U8500			U8100		// U8500  - AP����
#define	U3100K			81			// U3100K - AP���� (BRM21)

// SHU Type
#define SHU_MECHA_O		1			// 1 ����, 1 �� ���� SHU (O TYPE) - SHU-O
#define SHU_MECHA_HS	2			// 2 ����, 2 �� ���� SHU (O TYPE) - H-SHU
#define SHU_MECHA_NT	3			// 1 ����, 1 �� ���� SHU (NT TYPE)- NT-SHU
#define SHU_MECHA_S		4			// 1 ����, 1 �� ���� SHU (S TYPE)
#define SHU_MECHA_S2	5			// 2 ����, 2 �� ���� SHU (S TYPE)
#define SHU_MECHA_T		6			// 1 ����, 1 �� ���� SHU (T TYPE) - T-SHU
#define SHU_MECHA_A2	7			// 2 ����, 2 �� ���� SHU (O TYPE) - SHUA2O

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
#define HBRM_SET		4			// U8100 AP ���泻�� #04 - HBRM���� ���泻��

#define CAM_SET			1
#define UCM_SET			2
#define PBM_SET		 	1			// AUTO TURN
#define PBM_NAT_SET 	2			// NO	AUTO	TURN
#define I_PBM_SET 		3			// U8100 AP ���泻�� #07 - IPBM �Ҹ�ǰ �ܿ����� ����

#define SOUND_SET		1
#define PC_SOUND_SET	2			// PC ����Ŀ (����ī�� ���� ���)
#define PCI_CAMERA_SET	1			// PCI Type�� ī�޶� ��ġ��
#define USB_CAMERA_SET	2			// USB Type�� ī�޶� ��ġ��
#define HUB_CAMERA_SET	3			// HUB USB Type�� ī�޶� ��ġ��
#define DVR_CAMERA_SET	4			// DVR Type�� ī�޶� ��ġ��
#define TENKEY_SET		1
#define HOTKEY_SET		2			// ������ 1�� ���� (U8100-AP���� #13)
#define ICCARD_SET		1
#define FINGER_SET		1
#define IRDA_SET		1
#define BAR_SET			1
#define SEED_SET		1
#define VFD_SET			1
#define LED_SET			2			// U8100 AP ���泻�� #05 - LED��� ����
#define RFD_SET			1
#define EAR_SET			1			// #N0193 �����ATM
#define ZOOM_SET		1			// #N0193 �����ATM

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

#define NET_IOC			1			// For ����/����/��� ����
#define NET_SWP			2			// For ����/����/�ϳ�/����/��� ����
#define NET_CCL			3			// For ����,����,����
#define NET_SDLC		4			// For �������
#define NET_TCPIP		5			// For ����/��� ���� 
#define NET_HDLC		6			// For ����/��ȭ ����
#define NET_BAS			7			// For PUSAN BANK
#define	NET_TMAX		8			// For ���/����

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
	int		KCashHandler;			// 0:K-CASH���� ,	1:K-CASHŸ��
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
	int		EarHandler;				// 0:NOT INSTALL,	1:INSTALL �̾���	
	int		Proximity;				// 0:NOT INSTALL,	1:INSTALL ��������	
	int		SightHandicapped;		// 0:NOT SET,		1:SET     ���÷���	

} DEVICEINFO;

typedef struct tagBRMInfo{
	WORD	CrntCashCnt;			// ���� ���� �ż�
	int		DepositCashKind;		// �Աݱ�������(�����Ǹ� ����: 1, ���������� : 0)
	int		PriorityNote;			// V06-01-01-#01	
} BRMINFO;

typedef struct tagUCMInfo{
	WORD	CrntInCheckCnt;			// ���� �Աݼ�ǥ �ż�
	char	BankNo[3];				// �����ڵ�
	char	CrntOutCheckNo[9] ;		// ���� ��ݼ�ǥ ��ȣ
	WORD	CrntOutCheckCnt;		// ���� ��ݼ�ǥ �ż�
	char	BankNo2[3];				// �����ڵ�
	char	CrntOutCheckNo2[9] ;	// ���� ��ݼ�ǥ ��ȣ
	WORD	CrntOutCheckCnt2;		// ���� ��ݼ�ǥ �ż�
	WORD	CrntOutCheckKind;		// ���� ��ݼ�ǥ ����
} UCMINFO;

typedef struct tagNetworkInfo{
	char	AtmSerialNum[9];		// ���
	char	BranchNum[9];			// ����(�Ҽ�����2)
	char	SubBranchNum[9];		// ����(�Ҽ�����1)
	char	RoomNum[9];				// ��ġ����(ó������)
	char	MachineMakeDate[9];		// �����⵵
	char	MachineMakeNum[21];		// ������ȣ
	int		BranchInOut;			// ����/��
	int		AutoBranchOnOff;		// ��������
	int		All24OnOff;				// 24On/Off(24/365� On/Off)
	int		All365OnOff;			// 365On/Off(365CASH On/Off)
	int		CheckOnOff;				// CheckOn/Off(��ǥž�� On/Off:AP SET)
	char	CeOfficeCode[5];		// CE�繫���ڵ�
	char	OutBranchNum[9];		// �����
	char	InstName[11];			// ��ġ��
	char	InstConfirmName[11];	// ��ġȮ����
	char	CeName[11];				// ���������
	char	CeTel[17];				// ��������ڿ���ó
	
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
	int		SendRecvSize;			// ��/���� SIZE
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
	char	YYYYMMDD[9];			// ���� 
	char	YYYYMMDDHHMMSS[15];		// ���ڽð� 
	char	SerialNo[7];			// �ŷ��Ϸù�ȣ 
	char	StartSerialNo[7];		// �����Ϸù�ȣ
	BYTE	ClerkTransOnOff;		// ����� ���� �ŷ� ���
	char	ProcCount[2];			// ���� Count
	int		TransPowerOnOff;		// �ŷ��� POWER OFF
	char	EjrSerialNo[9];			// Ejr�Ϸù�ȣ 
	char	HostSerialNo[13];		// Ejr�Ϸù�ȣ 
	int		MagamFlg;				// ����
	int		DeviceExecOff;			// �Ұ����� �� Device = Set
	char	DeviceYYYYMMDD[9];		// ��ֳ����� 
	int		DeviceSprCnt;			// Spr��� Cnt
	int		DeviceJprCnt;			// Jpr��� Cnt
	int		DeviceMcuCnt;			// Mcu��� Cnt
	int		DeviceBrmCnt;			// Brm��� Cnt
	int		DeviceUcmCnt;			// Ucm��� Cnt
	int		DevicePbmCnt;			// Pbm��� Cnt
	int		DeviceA4pCnt;			// A4��� Cnt
	int		DeviceCshCnt;			// ���ݺ���� Ƚ��
	char	CheckYYYYMMDD[9];		// ��ǥ��������
	char	AsPassWord[7];			// 
	char	OpPassWord[7];			// 
	char	GuideMsg1[41];			// �ȳ�1
	char	GuideMsg2[41];			// �ȳ�2
	char	GuideMsg3[41];			// �ȳ�3
	char	IngGuideMsg1[81];		// ó���߾ȳ�1				
	char	IngGuideMsg2[81];		// ó���߾ȳ�2
	char	IngGuideMsg3[81];		// ó���߾ȳ�3
	char	IngGuideMsg4[81];		// ó���߾ȳ�4
	char	IngGuideMsg5[81];		// ó���߾ȳ�5
	int		EMVSerialNo;			// EMV �ŷ��Ϸù�ȣ			
	int		AjmsMode;				// AJMS��尪
	int		ZoomFlag;				// �����ATM
	int		JaMaxDispAmt;			//
	int		TaMaxDispAmt;			//
	int		TranService;			// 
	int		TranInqFee;				// 
	char	RebootHHMMSS[7];		// 
	int		RebootFlag;				// ��ֹ߻��� ������ ó�� 


} TRANSINFO; 


typedef struct tagLCDInfo{
	int		LCDType;
	char	LCDDisplay[71];
	char	LCDADInfo[64];
	int		LCDCameraPos;
	int		LCDVolume;				
} LCDINFO;



typedef struct tagECASHTOTALINFO{	// 
	char	sCashWith[11];			// �������޾�
	char	sCashDep[11];			// �����Աݾ�
	char	sCheckWith[11];			// ��ǥ���޾�
	char	sCheckDep[11];			// ��ǥ�Աݾ�
	char	sBalance[11];			// �����ܾ�
	char	End;					// Data�� ��
} ECASHTOTALINFO;


// ����/���ͷ�Ƽ�� ����
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

// U8100 AP ���泻�� #03 - ����LCD����ó��, ����
typedef struct tagiDKeyLcd{
	char *name;
}iDkeyLCD;

typedef struct tagiDkeyAD{
	char *name;
}iDkeyAD;

/////////////////////////////////////////////////////////////////////////////
// ErrStack
/////////////////////////////////////////////////////////////////////////////
#define  ErrStack_MaxNumber   256	// ��� STACK TABLE �ִ� ���� ����
#define  ErrSDeTailHost_MaxNumber  20	// ��� STACK TABLE �ִ� ���� ����

typedef struct tagMainInfo{			// ��� STACK TABLE �������� 
	int  Loop;						// ȯ������
	int  CrtCount;					// ��ġ����
	int  StackCount;				// ��������
} MAININFO;

typedef struct tagErrorInfo{		// ��� STACK ���� ���� 
	char Date[11];					// ��¥
	char Time[9];					// �ð�
	char ProcCount[2];				// ����ī����
	char ErrorCode[8];				// ����ڵ�
	char ErrorString[81];			// ��ּ���
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
#define  ErrHost_MaxNumber   256	// ��� HOST TABLE �ִ� ���� ����

typedef struct tagHErrInfo{			// ��� HOST TABLE ��������(HOST)
	int  Loop;						// ȯ������
	int  Count;						// ��ġ����
} HERRINFO;

typedef struct tagDErrInfo{			// HOST ��� �ŷ� ���� 
    char Remark[81];				// ����
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
typedef struct tagLogWFormat{		// ����
	char	Count[10];				// �Ǽ�
	char	Amount[10];				// �Ѿ�
	char	Dummy;					// NULL
} LOGW;

typedef struct tagWITHTOTALINFO{	// ���� ��.�Ұ� ����
	LOGW    DATA[11];				// 0:�������������
									// 1:��������ǥ���
									// 2:����������հ�
									// 3:������������
									// 4:��������ü
									// 5:�������������
									// 6:�����ļ�ǥ���
									// 7:����������հ�
									// 8:������������
									// 9:�����Ĵ�ü
									//10:DUMMY
} WITHTOTALINFO;

typedef struct tagLogDFormat{		// �Ա� ��.�Ұ� ����
	char	Count[10];				// �Ǽ�
	char	Amount[10];				// �Ѿ�
	char	Dummy;					// NULL
} LOGD;

typedef struct tagDEPTOTALINFO{		// �Ա� ��.�Ұ� ����
	LOGD	DATA[9];				// 0:�����������Ա�
									// 1:�����������ǥ�Աݱݾ�
									// 2:������Ÿ���ǥ�Աݱݾ�
									// 3:DUMMY
									// 4:�����������Ա�
									// 5:�����������ǥ�Աݱݾ�
									// 6:������Ÿ���ǥ�Աݱݾ�
									// 7:DUMMY
									// 8:DUMMY
} DEPTOTALINFO;

typedef struct tagLogTFormat{		// ����
	char	Count[10];				// �Ǽ�
	char	Amount[10];				// �Ѿ�
	char	Dummy;					// NULL
} LOGT;

typedef struct tagTRANSTOTALINFO{	// ��ü ��.�Ұ� ����
	LOGT    DATA[5];				// 0:��������ü�ݾ�
									// 1:��������ü�ݾ�
									// 2:DUMMY
} TRANSTOTALINFO;

typedef struct tagMONEYTOTALINFO{	// ������հ�
	LOGT	DATA[3];				// 0:���ݹ�����
									// 1:��ǥ�����Ѿ�
									// 2:Dummy
} MONEYTOTALINFO;

typedef struct tagDEPERRTOTALINFO{	// �Ա���� �հ�
	LOGT	DATA[5];				// 0:�Ա��������Ѿ�      
									// 1:�Աݰ������Ѿ�
									// 2:�Աݼ�������Ѿ�
	                                // 3:�Աݹ̼����Ѿ�        
	                                // 4:DUMMY
} DEPERRTOTALINFO;

typedef struct tagWITHERRTOTALINFO{	// �Ա���� �հ�
	LOGT	DATA[5];				// 0:����������Ѿ�      
									// 1:��ݰ������Ѿ�
									// 2:��ݼ�������Ѿ�
	                                // 3:��ݹ̼����Ѿ�        
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