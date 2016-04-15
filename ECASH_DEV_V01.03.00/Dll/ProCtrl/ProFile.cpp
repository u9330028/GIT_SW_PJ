/////////////////////////////////////////////////////////////////////////////
//	Profile.cpp: implementation of the CProfile class.
/////////////////////////////////////////////////////////////////////////////
/* ──────────────────────────────────────
   ▷ 파일변경이력
  ┌─────┬─────┬────┬────────────────────┐
  │A P 버 전 │ 변경일자 │항목번호│  변경내용(사유)                        │
  └─────┴─────┴────┴────────────────────┘
   ──────────────────────────────────────*/
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include "stdio.h"
#include "stdlib.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Dll\Profile.h"
#include "..\..\H\Dll\BIPConfig.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

iDkeyDEVICE DEVKEY[]={
	"MachineType"  		,
	"SHUHandler"  		,
	"JournalDest"  		,
	"MCUHandler"  		,
	"CashHandler"		,
	"CheckHandler"		,
	"PBHandler"    		,
	"KeyHandler" 		,
	"SoundDevice"  		,
	"SoundVolume"  		,
	"CameraDevice" 		,
	"TenKeyHandler" 	,
	"ICCardDevice"		,
	"PowerHandler"		,
	"DesHandler"		,
	"DesMasterKey"		,
	"FingerHandler"		,
	"IrdaHandler"		,
	"BarHandler"		,
	"SeedHandler"		,
	"KCashHandler"		,
	"VfdHandler"		,
	"RfdHandler"		,
	"A4PHandler"        ,
	"GiroHandler"		,
	"AjmsHandler"       ,
	"ETCDevice01"       ,
	"ETCDevice02"       ,
	"ETCDevice03"       ,
	"ETCDevice04"       ,
	"ETCDevice05"       ,
	"ETCDevice06"       ,
	"ETCDevice07"       ,
	"ETCDevice08"       ,
	"ETCDevice09"       ,
	"ETCDevice10"       ,
	"ETCDevice11"       ,
	"ETCDevice12"       ,
	"ETCDevice13"       ,
	"ETCDevice14"       ,
	"ETCDevice15"       ,
	"ETCDevice16"       ,
	"ETCDevice17"       ,
	"ETCDevice18"       ,
	"ETCDevice19"       ,
	"ETCDevice20"       ,
	"EarHandler"		,
	"Proximity"			,
	"SightHandicapped"	

};

iDkeyBRM BRMKEY[]={
	"CrntCashCnt",
	"DepositCashKind",
	"PriorityNote"
};

iDkeyUCM UCMKEY[]={
	"CrntInCheckCnt"	,
	"BankNo"			,
	"CrntOutCheckNo"	,
	"CrntOutCheckCnt"	,
	"BankNo2"			,
	"CrntOutCheckNo2"	,
	"CrntOutCheckCnt2"	,
	"CrntOutCheckKind"
};

iDkeyNET NETKEY[]={
	"AtmSerialNum"		,
	"BranchNum"			,
	"SubBranchNum"		,
	"RoomNum"			,
	"MachineMakeDate"	,
	"MachineMakeNum"	,
	"BranchInOut"		,
	"AutoBranchOnOff"	,
	"All24OnOff"		,
	"All365OnOff"		,
	"CheckOnOff"		,
	"CeOfficeCode"		,
	"OutBranchNum"		,
	"InstName"			,
	"InstConfirmName"	,
	"CeName"			,
	"CeTel"				,

	"Interface"			,
	"BpCurIpAddress"	,
	"BpCurPortNo"		,
	"BpIpAddress"		,
	"BpPortNo"			,
	"BpIpAddress2"		,
	"BpPortNo2"			,
	"BpIpAddress3"		,
	"BpPortNo3"			,
    "AtmIPAddress"		,
    "AtmSubnetMask"		,
    "AtmGateWay"		,
	
	"DeviceAddr"		,
	"SelectAddr"		,
	"SendRecvSize"		,
	"LineSpeed"			,
	"PollCheck"			,
	"Point"				,
	"Duplex"			,
	"TimeFiller"		,
	"RsType"			,
	"Nrz"				,
	"Clk"
};

iDkeyTRANS TRANKEY[]={
	"ApVersion"			,		
	"YYYYMMDD"			,			
	"YYYYMMDDHHMMSS"    ,			
	"SerialNo"			,			
	"StartSerialNo"		,
	"ClerkTransOnOff"	,	
	"ProcCount"			,		
	"TransPowerOnOff"	,
	"EjrSerialNo"		,
	"HostSerialNo"		,
	"MagamFlg"			,
	"DeviceExecOff"		,
	"DeviceYYYYMMDD"	,
	"DeviceSprCnt"		,
	"DeviceJprCnt"		,
	"DeviceMcuCnt"		,
	"DeviceBrmCnt"		,
	"DeviceUcmCnt"		,
	"DevicePbmCnt"		,
	"DeviceA4pCnt"		,
	"DeviceCshCnt"      ,
	"CheckYYYYMMDD"		,
	"AsPassWord"        ,
	"OpPassWord"        ,
	"GuideMsg1"			,
	"GuideMsg2"			,
	"GuideMsg3"			,
	"IngGuideMsg1"		,
	"IngGuideMsg2"		,
	"IngGuideMsg3"		,
	"IngGuideMsg4"		,
	"IngGuideMsg5"		,
	"AjmsMode"			,
	"ZoomFlag"			,			
	"JaMaxDispAmt"    	,		
	"TaMaxDispAmt"    	,	
	"TranService"		,	
	"TranInqFee"		,	
	"RebootHHMMSS"		,	
	"RebootFlag"	    	
};

iDkeyLCD LCDKEY[]={												// 광고LCD위젯처리, 변경
	"LCDType",													// LCD관련 정보 (광고동영상)
	"LCDDisplay",
	"LCDADInfo",
	"LCDCameraPos",	
	"LCDVolume"													// 적용후 2차 수정 (U8100 AP 변경내역 #14) - 광고 동영상 음성볼륨
};

iDkeyAD ADKEY[]={
	"ADVersion"					,		
	"ADSize"					,		
	"ADMaxSize"					,		
	"ADStartIndex"				,		
	"AD1_Cmd"					,	
	"AD1_Size"					,	
	"AD1_Path"					,	
	"AD1_StartDate"				,			
	"AD1_EndDate"				,			
	"AD1_AllowList"				,			
	"AD1_DenyList"				,			
	"AD2_Cmd"					,	
	"AD2_Size"					,	
	"AD2_Path"					,	
	"AD2_StartDate"				,			
	"AD2_EndDate"				,			
	"AD2_AllowList"				,			
	"AD2_DenyList"				,			
	"AD3_Cmd"					,	
	"AD3_Size"					,	
	"AD3_Path"					,	
	"AD3_StartDate"				,			
	"AD3_EndDate"				,			
	"AD3_AllowList"				,			
	"AD3_DenyList"				,			
	"AD4_Cmd"					,	
	"AD4_Size"					,	
	"AD4_Path"					,	
	"AD4_StartDate"				,			
	"AD4_EndDate"				,			
	"AD4_AllowList"				,			
	"AD4_DenyList"				,			
	"AD5_Cmd"					,	
	"AD5_Size"					,	
	"AD5_Path"					,	
	"AD5_StartDate"				,			
	"AD5_EndDate"				,			
	"AD5_AllowList"				,			
	"AD5_DenyList"				,			
	"AD6_Cmd"					,	
	"AD6_Size"					,	
	"AD6_Path"					,	
	"AD6_StartDate"				,			
	"AD6_EndDate"				,			
	"AD6_AllowList"				,			
	"AD6_DenyList"				,			
	"AD7_Cmd"					,	
	"AD7_Size"					,	
	"AD7_Path"					,	
	"AD7_StartDate"				,			
	"AD7_EndDate"				,			
	"AD7_AllowList"				,			
	"AD7_DenyList"				,			
	"AD8_Cmd"					,	
	"AD8_Size"					,	
	"AD8_Path"					,	
	"AD8_StartDate"				,			
	"AD8_EndDate"				,			
	"AD8_AllowList"				,			
	"AD8_DenyList"				,			
	"AD9_Cmd"					,	
	"AD9_Size"					,	
	"AD9_Path"					,	
	"AD9_StartDate"				,			
	"AD9_EndDate"				,			
	"AD9_AllowList"				,			
	"AD9_DenyList"				,			
	"AD10_Cmd"					,	
	"AD10_Size"					,	
	"AD10_Path"					,	
	"AD10_StartDate"			,			
	"AD10_EndDate"				,			
	"AD10_AllowList"			,			
	"AD10_DenyList"				,			
	"AD11_Cmd"					,	
	"AD11_Size"					,	
	"AD11_Path"					,	
	"AD11_StartDate"			,			
	"AD11_EndDate"				,			
	"AD11_AllowList"			,			
	"AD11_DenyList"				,			
	"AD12_Cmd"					,	
	"AD12_Size"					,	
	"AD12_Path"					,	
	"AD12_StartDate"			,			
	"AD12_EndDate"				,			
	"AD12_AllowList"			,			
	"AD12_DenyList"				,			
	"AD13_Cmd"					,	
	"AD13_Size"					,	
	"AD13_Path"					,	
	"AD13_StartDate"			,			
	"AD13_EndDate"				,			
	"AD13_AllowList"			,			
	"AD13_DenyList"				,			
	"AD14_Cmd"					,	
	"AD14_Size"					,	
	"AD14_Path"					,	
	"AD14_StartDate"			,			
	"AD14_EndDate"				,			
	"AD14_AllowList"			,			
	"AD14_DenyList"				,			
	"AD15_Cmd"					,	
	"AD15_Size"					,	
	"AD15_Path"					,	
	"AD15_StartDate"			,			
	"AD15_EndDate"				,			
	"AD15_AllowList"			,			
	"AD15_DenyList"				,			
	
	"ADVersion"					,		
	"ADSize"					,		
	"ADMaxSize"					,		
	"ADStartIndex"				,			
	"AD1_Cmd"					,	
	"AD1_Path1"					,			
	"AD1_Path2"					,			
	"AD1_StartDate"				,			
	"AD1_EndDate"				,			
	"AD1_AllowList"				,			
	"AD1_DenyList"				,			
	"AD2_Cmd"					,	
	"AD2_Path1"					,			
	"AD2_Path2"					,			
	"AD2_StartDate"				,			
	"AD2_EndDate"				,			
	"AD2_AllowList"				,			
	"AD2_DenyList"				,			
	"AD3_Cmd"					,	
	"AD3_Path1"					,			
	"AD3_Path2"					,			
	"AD3_StartDate"				,			
	"AD3_EndDate"				,			
	"AD3_AllowList"				,			
	"AD3_DenyList"				,			
	"AD4_Cmd"					,	
	"AD4_Path1"					,			
	"AD4_Path2"					,			
	"AD4_StartDate"				,			
	"AD4_EndDate"				,			
	"AD4_AllowList"				,			
	"AD4_DenyList"				,			
	"AD5_Cmd"					,	
	"AD5_Path1"					,			
	"AD5_Path2"					,			
	"AD5_StartDate"				,			
	"AD5_EndDate"				,			
	"AD5_AllowList"				,			
	"AD5_DenyList"				,			
	"AD6_Cmd"					,	
	"AD6_Path1"					,			
	"AD6_Path2"					,			
	"AD6_StartDate"				,			
	"AD6_EndDate"				,			
	"AD6_AllowList"				,			
	"AD6_DenyList"				,			
	"AD7_Cmd"					,	
	"AD7_Path1"					,			
	"AD7_Path2"					,			
	"AD7_StartDate"				,			
	"AD7_EndDate"				,			
	"AD7_AllowList"				,			
	"AD7_DenyList"				,			
	"AD8_Cmd"					,	
	"AD8_Path1"					,			
	"AD8_Path2"					,			
	"AD8_StartDate"				,			
	"AD8_EndDate"				,			
	"AD8_AllowList"				,			
	"AD8_DenyList"				,			
	"AD9_Cmd"					,	
	"AD9_Path1"					,			
	"AD9_Path2"					,			
	"AD9_StartDate"				,			
	"AD9_EndDate"				,			
	"AD9_AllowList"				,			
	"AD9_DenyList"				,			
	"AD10_Cmd"					,
	"AD10_Path1"				,			
	"AD10_Path2"				,			
	"AD10_StartDate"			,			
	"AD10_EndDate"				,			
	"AD10_AllowList"			,			
	"AD10_DenyList"				,

	"ADVersion"					,		
	"StartDate"					,			
	"EndDate"					,			
	"AllowList"					,			
	"DenyList"					,
	"Line00"					,
	"Line01"					,
	"Line02"					,
	"Line03"					,
	"Line04"					,
	"Line05"					,
	"Line06"					,
	"Line07"					,
	"Line08"					,
	"Line09"					,

	"ADVersion"					,		
	"ADSize"					,		
	"ADMaxSize"					,		
	"ADStartIndex"				,		
	"BrandADOrgList"			,			
	"TargetADCodeList"				

};

CRITICAL_SECTION CProfile::m_csLock;

/////////////////////////////////////////////////////////////////////////////
//	Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
CProfile::CProfile()
{
	InitializeCriticalSection(&m_csLock);
}

CProfile::~CProfile()
{
	DeleteCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	내부함수
/////////////////////////////////////////////////////////////////////////////
#define DefSize		32768
int CProfile::GetPInt(char* SectName, char* KeyName, int nDef, char* FileName)
{
	return (GetPrivateProfileInt(SectName, KeyName, nDef, FileName));
}

void CProfile::GetPStr(char* SectName, char* KeyName, char sDef, void *cDest, int nSize, char* FileName)
{
	char	DefTemp[DefSize];
	char	Temp[DefSize];
	char*	Dest = (char*)cDest;

	DefTemp[0] = '(';
	memset(&DefTemp[1], sDef, nSize - 1);
	DefTemp[nSize] = ')';
	DefTemp[nSize + 1] = NULL;
	GetPrivateProfileString(SectName, KeyName, DefTemp, Temp, sizeof(Temp), FileName);
	memcpy(Dest, &Temp[1], nSize - 1);
	Dest[nSize - 1] = NULL;
}

void CProfile::GetPByten(char* SectName, char* KeyName, BYTE *Dest, int nSize, char* FileName)
{
	char	DefTemp[DefSize];
	char	Temp[DefSize];

	DefTemp[0] = '(';
	memset(&DefTemp[1], '0', nSize * 2);
	DefTemp[nSize * 2 + 1] = ')';
	DefTemp[nSize * 2 + 2] = NULL;
	GetPrivateProfileString(SectName, KeyName, DefTemp, Temp, sizeof(Temp), FileName);
	Asc2Byten(&Temp[1], Dest, nSize);
}

void CProfile::PutPInt(char* SectName, char* KeyName, int Src, char* FileName)
{
	char	Temp[DefSize];

	sprintf(Temp, "%d", Src);
	WritePrivateProfileString(SectName, KeyName, Temp, FileName);
}

void CProfile::PutPStr(char* SectName, char* KeyName, void *cSrc, int nSize, char* FileName)
{
	char	Temp[DefSize];
	int		sLen;
	char*	Src = (char*)cSrc;

	Temp[0] = '(';
	memset(&Temp[1], ' ', sizeof(Temp) - 1);

	sLen = strlen(Src);
	if (nSize > sLen)
	{
		memcpy(&Temp[1], Src, sLen);
		Temp[nSize] = ')';
		Temp[nSize + 1] = NULL;
	}
	else
	{
		memcpy(&Temp[1], Src, nSize - 1);
		Temp[nSize] = ')';
		Temp[nSize + 1] = NULL;
	}
	WritePrivateProfileString(SectName, KeyName, Temp, FileName);
}

void CProfile::PutPByten(char* SectName, char* KeyName, BYTE *Src, int nSize, char* FileName)
{
	char	Temp[DefSize];

	Temp[0] = '(';
	Byte2Ascn(Src, &Temp[1], nSize, 16);
	Temp[nSize * 2 + 1] = ')';
	Temp[nSize * 2 + 2] = NULL;
	WritePrivateProfileString(SectName, KeyName, Temp, FileName);
}

/////////////////////////////////////////////////////////////////////////////
//	Device/Network/Trans
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitProfileCheck()
{
	HFILE fi;

	// Device Ini File이 없는 경우 초기값으로 생성 
	fi = _lopen(_DEVICE_INI, OF_READWRITE);
	if (fi < 0)
	{
		_mkdir(_INI);											// \\INI\\ DIRECTORY 생성
		InitProfile(DEV_INI);
	}
	else
	{
		_lclose(fi);
	    GetDeviceProfile();
		GetBRMProfile();
		GetUCMProfile();
	}

	// Network Ini File이 없는 경우 초기값으로 생성 
	fi = _lopen(_NETWORK_INI, OF_READWRITE);
	if (fi < 0)
		InitProfile(NETWORK_INI);
	else
	{
		_lclose(fi);
		GetNetworkProfile();
	}

	// Trans Ini File이 없는 경우 초기값으로 생성 
	fi = _lopen(_TRANS_INI, OF_READWRITE);
	if (fi < 0)
		InitProfile(TRANS_INI);
	else
	{
		_lclose(fi);
		GetTransProfile();
	}
/*
	// AD Ini File,If the file doesn't exist then create the file
	fi = _lopen(_MAINAD_INI, OF_READWRITE);
	if (fi < 0)
		InitProfile(AD_INI);
	else
	{
		_lclose(fi);
		GetADProfile();
	}
*/
	// ErrStack Ini File이 없는 경우 초기값으로 생성 
	fi = _lopen(_ERRDETAILHOST_INI, OF_READWRITE);
	if (fi < 0) 
		DetailHClearAllErrStack();
	else _lclose(fi);

	// ErrDeTailHost Ini File이 없는 경우 초기값으로 생성 
	fi = _lopen(_ERRSTACK_INI, OF_READWRITE);
	if (fi < 0) 
		ClearAllErrStack();
	else _lclose(fi);

	// Error Ini File이 없는 경우 초기값으로 생성
	fi = _lopen(_ERRHOST_INI, OF_READWRITE);
	if (fi < 0) 
	{
		ClearAllErrTran();										// 거래장애정보초기화
		ClearAllErrCancel();									// 취소장애정보초기화
		ClearAllErrHost();										// 호스트장애정보초기화
		ClearAllErrKCash();										// KCash장애정보초기화
	}
	else _lclose(fi);

	// Total Ini File이 없는 경우 초기값으로 생성 
	fi = _lopen(_ATMTOTAL_INI, OF_READWRITE);
	if (fi < 0) 
	{
		InitWithTotal();										// 지급초기화
		InitDepTotal();											// 입금초기화
		InitTransTotal();										// 이체초기화
		InitDepErrTotal();										// 이체초기화
		InitWithErrTotal();										// 이체초기화
	}
	else _lclose(fi);

}

/////////////////////////////////////////////////////////////////////////////
//	초기 INI file이 없는 경우 기본값으로 INI file을 만든다. 
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitProfile(int type)
{
	switch (type) 
	{
		case DEV_INI:
			InitDeviceProfile();								
			InitBRMProfile();
			InitUCMProfile();
			break;
		case NETWORK_INI:	
			InitNetworkProfile();
			break;
		case TRANS_INI:
			InitTransProfile();
			break;
		case AD_INI:
			InitADProfile();
			break;
		default:
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	Initial Device Profile 
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitDeviceProfile()
{
	memset(&DEVICE, 0, sizeof(DEVICE));
	LONG	lResult;											// T2ATM 2006.5.24 yscho
	HKEY	hKeyResult = 0;
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_REGKEY_OKI,0,KEY_ALL_ACCESS,&hKeyResult);

	DEVICE.MachineType   = U8500;								

	switch (DEVICE.MachineType)
	{
//		case U8100:													
		case U8500:													
			DEVICE.SHUHandler    = SHU_MECHA_A2;
			DEVICE.JournalDest[0]= JNL_A;
			DEVICE.JournalDest[1]= NULL;
			DEVICE.MCUHandler	 = O_TYPE;
			
			DEVICE.CashHandler  = HBRM_SET;							// U8100 AP 변경내역 #04 - HBRM관련 변경내역
			
			DEVICE.CheckHandler  = UCM_SET;
			DEVICE.PBHandler     = I_PBM_SET;
			DEVICE.KeyHandler    = TOUCH_TYPE;
			DEVICE.SoundDevice   = SOUND_SET;
			DEVICE.SoundVolume   = SOUND_ONE;
			DEVICE.CameraDevice  = USB_CAMERA_SET;
			DEVICE.TenKeyHandler = HOTKEY_SET;
			DEVICE.ICCardDevice  = ICCARD_SET;
			DEVICE.PowerHandler  = PNC_TYPE;
			DEVICE.DesHandler	 = NOT_SET;
			DEVICE.DesMasterKey	 = NOT_SET;
			DEVICE.FingerHandler = NOT_SET;
			DEVICE.IrdaHandler   = NOT_SET;
			DEVICE.BarHandler    = NOT_SET;
			DEVICE.SeedHandler   = NOT_SET;
			DEVICE.VfdHandler	 = NOT_SET;						
			DEVICE.RfdHandler	 = RFD_SET;
			DEVICE.EarHandler    = NOT_SET;							// V06-07-01
			DEVICE.Proximity	 = NOT_SET;							// V07-01-12-#01:근접센서
			DEVICE.SightHandicapped = NOT_SET;					
			break;



		case U3100K:
			DEVICE.SHUHandler    = SHU_MECHA_A2;
			DEVICE.JournalDest[0]= JNL_A;
			DEVICE.JournalDest[1]= NULL;
			DEVICE.MCUHandler	 = O_TYPE;
			DEVICE.CashHandler	 = HBRM_SET;					

			DEVICE.CheckHandler  = NOT_SET;
			DEVICE.PBHandler     = NOT_SET;
			DEVICE.KeyHandler    = TOUCH_TYPE;
			DEVICE.SoundDevice   = SOUND_SET;
			DEVICE.SoundVolume   = SOUND_ONE;
			DEVICE.CameraDevice  = USB_CAMERA_SET;
			DEVICE.TenKeyHandler = NOT_SET;
			DEVICE.ICCardDevice  = ICCARD_SET;
			DEVICE.PowerHandler  = PNC_TYPE;
			DEVICE.DesHandler	 = NOT_SET;
			DEVICE.DesMasterKey	 = NOT_SET;
			DEVICE.FingerHandler = FINGER_SET;
			DEVICE.IrdaHandler   = NOT_SET;
			DEVICE.BarHandler    = NOT_SET;
			DEVICE.SeedHandler   = NOT_SET;
			DEVICE.KCashHandler	 = ICCARD_SET;
			DEVICE.VfdHandler	 = NOT_SET;
			DEVICE.RfdHandler	 = NOT_SET;
			DEVICE.A4PHandler	 = NOT_SET;
			DEVICE.GiroHandler	 = NOT_SET;
			DEVICE.AjmsHandler	 = NOT_SET;
			DEVICE.ETCDevice01	 = NOT_SET;
			DEVICE.ETCDevice02	 = NOT_SET;
			DEVICE.ETCDevice03	 = NOT_SET;
			DEVICE.ETCDevice04	 = NOT_SET;
			DEVICE.ETCDevice05	 = NOT_SET;
			DEVICE.ETCDevice06	 = NOT_SET;
			DEVICE.ETCDevice07	 = NOT_SET;
			DEVICE.ETCDevice08	 = NOT_SET;
			DEVICE.ETCDevice09	 = NOT_SET;
			DEVICE.ETCDevice10	 = NOT_SET;
			DEVICE.ETCDevice11	 = NOT_SET;
			DEVICE.ETCDevice12	 = NOT_SET;
			DEVICE.ETCDevice13	 = NOT_SET;
			DEVICE.ETCDevice14	 = NOT_SET;
			DEVICE.ETCDevice15	 = NOT_SET;
			DEVICE.ETCDevice16	 = NOT_SET;
			DEVICE.ETCDevice17	 = NOT_SET;
			DEVICE.ETCDevice18	 = NOT_SET;
			DEVICE.ETCDevice19	 = NOT_SET;
			DEVICE.ETCDevice20	 = NOT_SET;
			break;
	}

	PutDeviceProfile();
	if (lResult == ERROR_SUCCESS)
		RegCloseKey(hKeyResult);	
}

/////////////////////////////////////////////////////////////////////////////
//	Initial BRM Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitBRMProfile()
{
	memset(&BRM, 0, sizeof(BRM));

	PutBRMProfile();
}

/////////////////////////////////////////////////////////////////////////////
//	Initial UCM Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitUCMProfile()
{
	memset(&UCM, 0, sizeof(UCM));

	StrAllZero(UCM.CrntOutCheckNo);
	StrAllZero(UCM.CrntOutCheckNo2);

	PutUCMProfile();
}

/////////////////////////////////////////////////////////////////////////////
//	Initial Network Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitNetworkProfile()
{
	CString			strTemp("");
	CBIPConfig *	pstrBipConfig;
	char	szTempBuff[256] = {0,};
	DWORD dwIpAddress = NULL;
	memset(&NETWORK, 0, sizeof(NETWORK));

	strcpy(NETWORK.AtmSerialNum,	"00000004");
	strcpy(NETWORK.BranchNum,		"00000002");
	StrAllZero(NETWORK.SubBranchNum);
	StrAllZero(NETWORK.RoomNum);
	strcpy(NETWORK.MachineMakeDate,	"20070701");
	StrAllZero(NETWORK.MachineMakeNum);
	NETWORK.BranchInOut		= ON; 
	NETWORK.AutoBranchOnOff	= OFF;
	NETWORK.All24OnOff		= OFF; 
	NETWORK.All365OnOff		= OFF; 
	NETWORK.CheckOnOff		= ON;
	StrAllSpace(NETWORK.CeOfficeCode);
	StrAllZero(NETWORK.OutBranchNum);
	StrAllSpace(NETWORK.InstName);
	StrAllSpace(NETWORK.InstConfirmName);
	StrAllSpace(NETWORK.CeName);
	StrAllSpace(NETWORK.CeTel);

	NETWORK.Interface		= NET_TCPIP;


	strTemp.Empty();

	pstrBipConfig = new CBIPConfig();

// 	pstrBipConfig->GetAdapterAddress(szTempBuff);
// 	strTemp.Format("%2.2s%2.2s%2.2s%2.2s%2.2s%2.2s", &szTempBuff[0], &szTempBuff[3],
// 		&szTempBuff[6], &szTempBuff[9], &szTempBuff[12], &szTempBuff[15]);
// // 	memcpy(NETWORK.MACAddress,strTemp.GetBuffer(0),12);
// // 	strcpy(NETWORK.BpIpAddress,		"atmmntl-pkr.apac.nsroot.net");
// // 	strcpy(NETWORK.BpPortNo,		"19001");

// network.ini정리	strcpy(NETWORK.BpIpAddress2,	"150.214.109.254");
// network.ini정리	strcpy(NETWORK.BpPortNo2,		"09002");
// network.ini정리	strcpy(NETWORK.BpIpAddress3,	"150.214.109.254");
// network.ini정리	strcpy(NETWORK.BpPortNo3,		"09002");

	memset(szTempBuff, NULL, sizeof(szTempBuff));
	strTemp.Empty();
	pstrBipConfig->AtmGetIpAddress(szTempBuff);
	dwIpAddress = pstrBipConfig->IPConvert(szTempBuff);
	strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", (dwIpAddress & 0xff000000) >> 24, 
		(dwIpAddress & 0x00ff0000) >> 16, (dwIpAddress & 0x0000ff00) >> 8,
		(dwIpAddress & 0x000000ff));
    strcpy(NETWORK.AtmIPAddress, strTemp.GetBuffer(0));

	memset(szTempBuff, NULL, sizeof(szTempBuff));
	strTemp.Empty();
	pstrBipConfig->AtmGetSubnetMask(szTempBuff);
	dwIpAddress = pstrBipConfig->IPConvert(szTempBuff);
	strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", (dwIpAddress & 0xff000000) >> 24, 
		(dwIpAddress & 0x00ff0000) >> 16, (dwIpAddress & 0x0000ff00) >> 8,
		(dwIpAddress & 0x000000ff));
    strcpy(NETWORK.AtmSubnetMask, strTemp.GetBuffer(0));

	memset(szTempBuff, NULL, sizeof(szTempBuff));
	strTemp.Empty();
	pstrBipConfig->AtmGetGateWay(szTempBuff);
	dwIpAddress = pstrBipConfig->IPConvert(szTempBuff);
	delete pstrBipConfig;

	strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", (dwIpAddress & 0xff000000) >> 24, 
		(dwIpAddress & 0x00ff0000) >> 16, (dwIpAddress & 0x0000ff00) >> 8,
		(dwIpAddress & 0x000000ff));
    strcpy(NETWORK.AtmGateWay, strTemp.GetBuffer(0));


	NETWORK.DeviceAddr		= 0x00; 
	NETWORK.SelectAddr		= 0x00; 
	NETWORK.SendRecvSize    = 1024;
	NETWORK.LineSpeed		= L19200;
	NETWORK.PollCheck		= POLL_ON;
	NETWORK.Point			= PTP;
	NETWORK.Duplex			= HALF_DUPLEX;
	NETWORK.TimeFiller		= MTF;
	NETWORK.RsType          = RS422;
	NETWORK.Nrz             = NRZ;
	NETWORK.Clk			    = INCLK;

	PutNetworkProfile();
}

/////////////////////////////////////////////////////////////////////////////
//	Initial Trans Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitTransProfile()
{
	memset(&TRANS, 0, sizeof(TRANS));

	switch (DEVICE.MachineType)
	{
//		case U8100:
		case U8500:
			//						 1234567890123456789012345678901234567890
			strcpy(TRANS.ApVersion, "V00-00-00 000 U8100                     ");
			break;
		case U3100K:
			//						 1234567890123456789012345678901234567890
			strcpy(TRANS.ApVersion, "V00-00-00 000 U3100K                    ");
			break;

	}
	//VanCD Spec 0616
	memcpy(&TRANS.ApVersion[9], USERNAME, __min(strlen(TRANS.ApVersion)-9, strlen(USERNAME)));			

	GetDate(TRANS.YYYYMMDD);
	TRANS.YYYYMMDD[8]		= NULL;
	GetDate(TRANS.YYYYMMDDHHMMSS);
	GetTime(&TRANS.YYYYMMDDHHMMSS[8]);
	TRANS.YYYYMMDDHHMMSS[14]= NULL;
	StrAllZero(TRANS.SerialNo);
	StrAllZero(TRANS.StartSerialNo);
	TRANS.ClerkTransOnOff	= TRAN_ALL_OK;
	TRANS.ProcCount[0]		= ' ';
	TRANS.ProcCount[1]		= NULL;
	TRANS.TransPowerOnOff	= OFF;
	StrAllZero(TRANS.EjrSerialNo);
	StrAllZero(TRANS.HostSerialNo);
	TRANS.MagamFlg			= OFF;
	TRANS.DeviceExecOff		= DEV_NONE;
	GetDate(TRANS.DeviceYYYYMMDD);
	TRANS.DeviceYYYYMMDD[8]	= NULL;
	TRANS.DeviceSprCnt		= 0;
	TRANS.DeviceJprCnt		= 0;
	TRANS.DeviceMcuCnt		= 0;
	TRANS.DeviceBrmCnt		= 0;
	TRANS.DeviceUcmCnt		= 0;
	TRANS.DevicePbmCnt		= 0;
	TRANS.DeviceA4pCnt		= 0;
	TRANS.DeviceCshCnt		= 0;
	GetDate(TRANS.CheckYYYYMMDD);
	TRANS.CheckYYYYMMDD[8]	= NULL;
	StrAllZero(TRANS.AsPassWord);
	TRANS.AsPassWord[6]	= NULL;
	StrAllZero(TRANS.OpPassWord);
	TRANS.OpPassWord[6]	= NULL;
	StrAllSpace(TRANS.GuideMsg1);
	StrAllSpace(TRANS.GuideMsg2);
	StrAllSpace(TRANS.GuideMsg3);
	StrAllSpace(TRANS.IngGuideMsg1);
	StrAllSpace(TRANS.IngGuideMsg2);
	StrAllSpace(TRANS.IngGuideMsg3);
	StrAllSpace(TRANS.IngGuideMsg4);
	StrAllSpace(TRANS.IngGuideMsg5);
//	TRANS.AjmsMode			= ATMS_JNL_HDD;
	TRANS.AjmsMode			= ATMS_JNL_MIX;
	TRANS.ZoomFlag			= 0;											
	TRANS.JaMaxDispAmt		= 50;											// 
	TRANS.TaMaxDispAmt		= 30;											// 
	TRANS.TranService		= 0;											// 
	TRANS.TranInqFee		= 0;											// 
	StrAllZero(TRANS.RebootHHMMSS);
	TRANS.RebootFlag		= FALSE;							// U8100-97925, 97926 장애발생시 리부팅 처리 : 초기 Initialize시에만 한번 합니다

	PutTransProfile();
}

/////////////////////////////////////////////////////////////////////////////
//	Initial AD Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitADProfile()
{
	memset(&TRANS, 0, sizeof(TRANS));

	AD.MainADVersion	= "";
	AD.MainADSize		= 0;
	AD.MainADMaxSize	= 0;
	AD.MainADStartIndex	= 0;
	for (int i=0; i<15; i++)
	{
		MAINADITEM *pADItem = new MAINADITEM;
		
		pADItem->ADCmd = "";
		pADItem->ADPath = "";
		pADItem->StartDate = "";
		pADItem->EndDate = "";
		pADItem->AllowList = "";
		pADItem->DenyList = "";

		AD.MainADItemArray.Add(pADItem);
	}
	AD.MainADSize = AD.MainADItemArray.GetSize();

	AD.InteractiveADVersion		= "";
	AD.InteractiveADSize		= 0;
	AD.InteractiveADMaxSize		= 0;
	AD.InteractiveADStartIndex	= 0;
	for (i=0; i<10; i++)
	{
		INTERACTIVEADITEM *pADItem = new INTERACTIVEADITEM;
		
		pADItem->ADCmd = "";
		pADItem->ADPath1 = "";
		pADItem->ADPath2 = "";
		pADItem->StartDate = "";
		pADItem->EndDate = "";
		pADItem->AllowList = "";
		pADItem->DenyList = "";

		AD.InteractiveADItemArray.Add(pADItem);
	}
	AD.InteractiveADSize = AD.InteractiveADItemArray.GetSize();
	AD.InteractiveADFlag		= FALSE;

	AD.ReceiptAD.ADVersion		= "";
	AD.ReceiptAD.StartDate		= "";
	AD.ReceiptAD.EndDate		= "";
	AD.ReceiptAD.AllowList		= "";
	AD.ReceiptAD.DenyList		= "";
	AD.ReceiptAD.ReceiptLine.RemoveAll();
	AD.ReceiptAD.ReceiptLine.Add(""); // 0
	AD.ReceiptAD.ReceiptLine.Add(""); // 1
	AD.ReceiptAD.ReceiptLine.Add(""); // 2
	AD.ReceiptAD.ReceiptLine.Add(""); // 3
	AD.ReceiptAD.ReceiptLine.Add(""); // 4
	AD.ReceiptAD.ReceiptLine.Add(""); // 5
	AD.ReceiptAD.ReceiptLine.Add(""); // 6
	AD.ReceiptAD.ReceiptLine.Add(""); // 7
	AD.ReceiptAD.ReceiptLine.Add(""); // 8
	AD.ReceiptAD.ReceiptLine.Add(""); // 9
	AD.ReceiptADFlag			= FALSE;

	AD.BannerADVersion			= "";
	AD.BannerADSize				= 0;
	AD.BannerADMaxSize			= 0;
	AD.BannerADStartIndex		= 0;
	AD.BannerBrandADOrgList		= "";
	AD.BannerTargetADCodeList	= "";

	PutADProfile();
}

/////////////////////////////////////////////////////////////////////////////
//	Get Device Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetDeviceProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	DEVICE.MachineType   = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.SHUHandler    = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	GetPStr(DEV_SEC, DEVKEY[i++].name, '?', DEVICE.JournalDest, sizeof(DEVICE.JournalDest), _DEVICE_INI);
	DEVICE.MCUHandler    = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.CashHandler   = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.CheckHandler  = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.PBHandler     = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.KeyHandler    = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.SoundDevice   = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.SoundVolume   = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.CameraDevice  = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.TenKeyHandler = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ICCardDevice  = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.PowerHandler  = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.DesHandler	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.DesMasterKey	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.FingerHandler = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.IrdaHandler   = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.BarHandler    = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.SeedHandler   = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.KCashHandler	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.VfdHandler	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.RfdHandler	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.A4PHandler	= GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.GiroHandler	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.AjmsHandler	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);

	DEVICE.ETCDevice01	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI); //RDCTRL Seting
	DEVICE.ETCDevice02	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI); //S-Pack Seting
	DEVICE.ETCDevice03	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice04	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice05	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice06	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice07	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice08	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice09	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI); //Mobile Kind
	DEVICE.ETCDevice10	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI); //
	DEVICE.ETCDevice11	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI); //브랜드, 제휴업무 설정(에브리데이)
	DEVICE.ETCDevice12	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice13	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice14	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice15	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice16	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice17	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice18	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice19	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice20	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.EarHandler    = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);	
	DEVICE.Proximity	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);	
	DEVICE.SightHandicapped = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);	


	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Get BRM Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetBRMProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	BRM.CrntCashCnt	    = GetPInt(BRM_SEC, BRMKEY[i++].name, 0, _DEVICE_INI);
	BRM.DepositCashKind	= GetPInt(BRM_SEC, BRMKEY[i++].name, 0, _DEVICE_INI);
	BRM.PriorityNote	= GetPInt(BRM_SEC, BRMKEY[i++].name, 5, _DEVICE_INI);	
	
	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Get UCM Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetUCMProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	UCM.CrntInCheckCnt  = GetPInt(UCM_SEC, UCMKEY[i++].name, 0, _DEVICE_INI);
	GetPStr(UCM_SEC, UCMKEY[i++].name, '0', UCM.BankNo, sizeof(UCM.BankNo), _DEVICE_INI);
	GetPStr(UCM_SEC, UCMKEY[i++].name, '0', UCM.CrntOutCheckNo, sizeof(UCM.CrntOutCheckNo), _DEVICE_INI);
	UCM.CrntOutCheckCnt = GetPInt(UCM_SEC, UCMKEY[i++].name, 0, _DEVICE_INI);
	GetPStr(UCM_SEC, UCMKEY[i++].name, '0', UCM.BankNo2, sizeof(UCM.BankNo2), _DEVICE_INI);
	GetPStr(UCM_SEC, UCMKEY[i++].name, '0', UCM.CrntOutCheckNo2, sizeof(UCM.CrntOutCheckNo2), _DEVICE_INI);
	UCM.CrntOutCheckCnt2 = GetPInt(UCM_SEC, UCMKEY[i++].name, 0, _DEVICE_INI);
	UCM.CrntOutCheckKind= GetPInt(UCM_SEC, UCMKEY[i++].name, 0, _DEVICE_INI);	// 100만원 수표지원
	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Get Network Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetNetworkProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.AtmSerialNum,	sizeof(NETWORK.AtmSerialNum),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BranchNum,		sizeof(NETWORK.BranchNum),      _NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.SubBranchNum,	sizeof(NETWORK.SubBranchNum),   _NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.RoomNum,		sizeof(NETWORK.RoomNum),		_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.MachineMakeDate,sizeof(NETWORK.MachineMakeDate),_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.MachineMakeNum,	sizeof(NETWORK.MachineMakeNum),	_NETWORK_INI);
	NETWORK.BranchInOut		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.AutoBranchOnOff	= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.All24OnOff		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.All365OnOff		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.CheckOnOff		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, ' ', NETWORK.CeOfficeCode,	sizeof(NETWORK.CeOfficeCode),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.OutBranchNum,	sizeof(NETWORK.OutBranchNum),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, ' ', NETWORK.InstName,		sizeof(NETWORK.InstName),		_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, ' ', NETWORK.InstConfirmName,sizeof(NETWORK.InstConfirmName),_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, ' ', NETWORK.CeName,			sizeof(NETWORK.CeName),			_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, ' ', NETWORK.CeTel,			sizeof(NETWORK.CeTel),			_NETWORK_INI);

	NETWORK.Interface		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpCurIpAddress,	sizeof(NETWORK.BpCurIpAddress),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpCurPortNo,	sizeof(NETWORK.BpCurPortNo),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpIpAddress,	sizeof(NETWORK.BpIpAddress),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpPortNo,		sizeof(NETWORK.BpPortNo),		_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpIpAddress2,	sizeof(NETWORK.BpIpAddress2),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpPortNo2,		sizeof(NETWORK.BpPortNo2),		_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpIpAddress3,	sizeof(NETWORK.BpIpAddress3),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpPortNo3,		sizeof(NETWORK.BpPortNo3),		_NETWORK_INI);
    GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.AtmIPAddress,	sizeof(NETWORK.AtmIPAddress),	_NETWORK_INI);
    GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.AtmSubnetMask,	sizeof(NETWORK.AtmSubnetMask),	_NETWORK_INI);
    GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.AtmGateWay,		sizeof(NETWORK.AtmGateWay),		_NETWORK_INI);

	GetPByten(NET_SEC, NETKEY[i++].name, &NETWORK.DeviceAddr, sizeof(NETWORK.DeviceAddr), _NETWORK_INI);
	GetPByten(NET_SEC, NETKEY[i++].name, &NETWORK.SelectAddr, sizeof(NETWORK.SelectAddr), _NETWORK_INI);
	NETWORK.SendRecvSize	= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.LineSpeed		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.PollCheck  		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.Point			= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.Duplex			= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.TimeFiller		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.RsType			= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.Nrz				= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.Clk				= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Get Trans Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetTransProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	GetPStr(TRANS_SEC, TRANKEY[i++].name, '?', TRANS.ApVersion,		sizeof(TRANS.ApVersion),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.YYYYMMDD,		sizeof(TRANS.YYYYMMDD),			_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.YYYYMMDDHHMMSS,sizeof(TRANS.YYYYMMDDHHMMSS),	_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.SerialNo,		sizeof(TRANS.SerialNo),			_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.StartSerialNo,	sizeof(TRANS.StartSerialNo),	_TRANS_INI);
	GetPByten(TRANS_SEC, TRANKEY[i++].name, &TRANS.ClerkTransOnOff, sizeof(TRANS.ClerkTransOnOff),	_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, ' ', TRANS.ProcCount,		sizeof(TRANS.ProcCount),		_TRANS_INI);
	TRANS.TransPowerOnOff = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,								_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.EjrSerialNo,	sizeof(TRANS.EjrSerialNo),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.HostSerialNo,	sizeof(TRANS.HostSerialNo),		_TRANS_INI);
	TRANS.MagamFlg = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,										_TRANS_INI);
	TRANS.DeviceExecOff = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.DeviceYYYYMMDD,sizeof(TRANS.DeviceYYYYMMDD),	_TRANS_INI);
	TRANS.DeviceSprCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DeviceJprCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DeviceMcuCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DeviceBrmCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DeviceUcmCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DevicePbmCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DeviceA4pCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DeviceCshCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.CheckYYYYMMDD,	sizeof(TRANS.CheckYYYYMMDD),	_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.AsPassWord,	sizeof(TRANS.AsPassWord),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.OpPassWord,	sizeof(TRANS.OpPassWord),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.GuideMsg1,		sizeof(TRANS.GuideMsg1),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.GuideMsg2,		sizeof(TRANS.GuideMsg2),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.GuideMsg3,		sizeof(TRANS.GuideMsg3),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.IngGuideMsg1,	sizeof(TRANS.IngGuideMsg1),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.IngGuideMsg2,	sizeof(TRANS.IngGuideMsg2),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.IngGuideMsg3,	sizeof(TRANS.IngGuideMsg3),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.IngGuideMsg4,	sizeof(TRANS.IngGuideMsg4),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.IngGuideMsg5,	sizeof(TRANS.IngGuideMsg5),		_TRANS_INI);
	TRANS.AjmsMode		= GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.ZoomFlag = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,										_TRANS_INI);
	TRANS.JaMaxDispAmt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,										_TRANS_INI);	
	TRANS.TaMaxDispAmt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,										_TRANS_INI);	
	TRANS.TranService = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,										_TRANS_INI);	
	TRANS.TranInqFee = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,										_TRANS_INI);	
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.RebootHHMMSS,	sizeof(TRANS.RebootHHMMSS),		_TRANS_INI);																
	TRANS.RebootFlag   = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Get AD Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetADProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);
	RemoveAllADItem();

	AD.MainADVersion		= IniGetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, "");
	AD.MainADSize			= IniGetInt(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, 0);
	AD.MainADMaxSize		= IniGetInt(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, 0);
	AD.MainADStartIndex		= IniGetInt(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, 0);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetADProfile():%s Version[%s], Size[%d], MaxSize[%d], StartIndex[%d]", MAIN_AD_SEC, AD.MainADVersion, AD.MainADSize, AD.MainADMaxSize, AD.MainADStartIndex);

	for (int j=0; j<AD.MainADMaxSize; j++)
	{
		MAINADITEM *pADItem = new MAINADITEM;

		pADItem->ADCmd = IniGetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, "");
		pADItem->ADSize = IniGetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, "");
		pADItem->ADPath = IniGetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, "");
		pADItem->StartDate = IniGetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, "");
		pADItem->EndDate = IniGetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, "");
		pADItem->AllowList = IniGetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, "");
		pADItem->DenyList = IniGetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, "");
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetMainADProfile() : [%d] %s %s %s %s %s %s %s", j, pADItem->ADCmd, pADItem->ADSize, pADItem->ADPath, pADItem->StartDate, pADItem->EndDate, pADItem->AllowList, pADItem->DenyList);
	
		AD.MainADItemArray.Add(pADItem);
	}
	MakeMainADList();

	AD.InteractiveADVersion			= IniGetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, "");
	AD.InteractiveADSize			= IniGetInt(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, 0);
	AD.InteractiveADMaxSize			= IniGetInt(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, 0);
	AD.InteractiveADStartIndex		= IniGetInt(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, 0);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetADProfile():%s Version [%s], Size[%d], MaxSize[%d], StartIndex[%d]", INTERACTIVE_AD_SEC, AD.MainADVersion, AD.InteractiveADSize, AD.InteractiveADMaxSize, AD.InteractiveADStartIndex);

	for (j=0; j<AD.InteractiveADMaxSize; j++)
	{
		INTERACTIVEADITEM *pADItem = new INTERACTIVEADITEM;

		pADItem->ADCmd = IniGetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, "");
		pADItem->ADPath1 = IniGetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, "");
		pADItem->ADPath2 = IniGetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, "");
		pADItem->StartDate = IniGetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, "");
		pADItem->EndDate = IniGetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, "");
		pADItem->AllowList = IniGetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, "");
		pADItem->DenyList = IniGetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, "");
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetInteractiveADProfile() : [%d] %s %s %s %s %s %s %s", j, pADItem->ADCmd, pADItem->ADPath1, pADItem->ADPath2, pADItem->StartDate, pADItem->EndDate, pADItem->AllowList, pADItem->DenyList);
	
		AD.InteractiveADItemArray.Add(pADItem);
	}
	NextInteractiveAD();

	AD.ReceiptAD.ADVersion		= IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, "");
	AD.ReceiptAD.StartDate		= IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, "");
	AD.ReceiptAD.EndDate		= IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, "");
	AD.ReceiptAD.AllowList		= IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, "");
	AD.ReceiptAD.DenyList		= IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, "");
	AD.ReceiptAD.ReceiptLine.RemoveAll();
	AD.ReceiptAD.ReceiptLine.Add(IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, ""));	// 0
	AD.ReceiptAD.ReceiptLine.Add(IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, ""));	// 1
	AD.ReceiptAD.ReceiptLine.Add(IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, ""));	// 2
	AD.ReceiptAD.ReceiptLine.Add(IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, ""));	// 3
	AD.ReceiptAD.ReceiptLine.Add(IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, ""));	// 4
	AD.ReceiptAD.ReceiptLine.Add(IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, ""));	// 5
	AD.ReceiptAD.ReceiptLine.Add(IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, ""));	// 6
	AD.ReceiptAD.ReceiptLine.Add(IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, ""));	// 7
	AD.ReceiptAD.ReceiptLine.Add(IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, ""));	// 8
	AD.ReceiptAD.ReceiptLine.Add(IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, ""));	// 9
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetADProfile():%s Version[%s], Start[%s], End[%s]", RECEIPT_AD_SEC, AD.MainADVersion, AD.ReceiptAD.StartDate, AD.ReceiptAD.EndDate);
	InitReceiptAD();

	AD.BannerADVersion		= IniGetStr(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, "");
	AD.BannerADSize			= IniGetInt(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, 0);
	AD.BannerADMaxSize		= IniGetInt(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, 0);
	AD.BannerADStartIndex	= IniGetInt(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, 0);
	AD.BannerBrandADOrgList	= IniGetStr(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, "");
	AD.BannerTargetADCodeList	= IniGetStr(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, "");
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetADProfile():%s Version[%s], ADSize[%d], ADMaxSize[%d], BrandOrg[%s], TargetOrg[%s]", BANNER_AD_SEC, AD.MainADVersion, AD.BannerADSize, AD.BannerADMaxSize, AD.BannerBrandADOrgList, AD.BannerTargetADCodeList);
	InitBannerAD();
	NextBannerAD();

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Put Device Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutDeviceProfile()
{
	int	i=0;

	EnterCriticalSection(&m_csLock);

	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.MachineType,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.SHUHandler,		_DEVICE_INI);
	PutPStr(DEV_SEC, DEVKEY[i++].name, DEVICE.JournalDest,		sizeof(DEVICE.JournalDest), _DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.MCUHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.CashHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.CheckHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.PBHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.KeyHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.SoundDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.SoundVolume,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.CameraDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.TenKeyHandler,	_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ICCardDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.PowerHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.DesHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.DesMasterKey,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.FingerHandler,	_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.IrdaHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.BarHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.SeedHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.KCashHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.VfdHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.RfdHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.A4PHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.GiroHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.AjmsHandler,		_DEVICE_INI);

	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice01,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice02,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice03,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice04,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice05,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice06,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice07,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice08,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice09,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice10,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice11,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice12,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice13,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice14,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice15,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice16,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice17,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice18,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice19,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice20,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.EarHandler,		_DEVICE_INI);	
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.Proximity,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.SightHandicapped,	_DEVICE_INI);	// #0264 장애인ATM 

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Put BRM Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutBRMProfile()
{
	int		i=0;
    
	EnterCriticalSection(&m_csLock);

	PutPInt(BRM_SEC, BRMKEY[i++].name, BRM.CrntCashCnt,		_DEVICE_INI);
	PutPInt(BRM_SEC, BRMKEY[i++].name, BRM.DepositCashKind,	_DEVICE_INI);
	PutPInt(BRM_SEC, BRMKEY[i++].name, BRM.PriorityNote,	_DEVICE_INI);		
	
	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Put UCM Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutUCMProfile()
{
	int		i=0;
    
	EnterCriticalSection(&m_csLock);

	PutPInt(UCM_SEC, UCMKEY[i++].name, UCM.CrntInCheckCnt,	_DEVICE_INI);
	PutPStr(UCM_SEC, UCMKEY[i++].name, UCM.BankNo, sizeof(UCM.BankNo), _DEVICE_INI);
	PutPStr(UCM_SEC, UCMKEY[i++].name, UCM.CrntOutCheckNo,	sizeof(UCM.CrntOutCheckNo), _DEVICE_INI);
	PutPInt(UCM_SEC, UCMKEY[i++].name, UCM.CrntOutCheckCnt,	_DEVICE_INI);
	PutPStr(UCM_SEC, UCMKEY[i++].name, UCM.BankNo2, sizeof(UCM.BankNo2), _DEVICE_INI);
	PutPStr(UCM_SEC, UCMKEY[i++].name, UCM.CrntOutCheckNo2,	sizeof(UCM.CrntOutCheckNo2), _DEVICE_INI);
	PutPInt(UCM_SEC, UCMKEY[i++].name, UCM.CrntOutCheckCnt2,	_DEVICE_INI);
	PutPInt(UCM_SEC, UCMKEY[i++].name, UCM.CrntOutCheckKind,_DEVICE_INI);	// 100만원 수표지원
	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Put Network Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutNetworkProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.AtmSerialNum,	sizeof(NETWORK.AtmSerialNum),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BranchNum,		sizeof(NETWORK.BranchNum),		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.SubBranchNum,	sizeof(NETWORK.SubBranchNum),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.RoomNum,			sizeof(NETWORK.RoomNum),		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.MachineMakeDate,	sizeof(NETWORK.MachineMakeDate),_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.MachineMakeNum,	sizeof(NETWORK.MachineMakeNum),	_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.BranchInOut,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.AutoBranchOnOff,	_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.All24OnOff,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.All365OnOff,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.CheckOnOff,		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.CeOfficeCode,	sizeof(NETWORK.CeOfficeCode),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.OutBranchNum,	sizeof(NETWORK.OutBranchNum),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.InstName,		sizeof(NETWORK.InstName),		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.InstConfirmName,	sizeof(NETWORK.InstConfirmName),_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.CeName,			sizeof(NETWORK.CeName),			_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.CeTel,			sizeof(NETWORK.CeTel),			_NETWORK_INI);

	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.Interface,		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpCurIpAddress,	sizeof(NETWORK.BpCurIpAddress),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpCurPortNo,		sizeof(NETWORK.BpCurPortNo),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpIpAddress,		sizeof(NETWORK.BpIpAddress),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpPortNo,		sizeof(NETWORK.BpPortNo),		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpIpAddress2,	sizeof(NETWORK.BpIpAddress2),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpPortNo2,		sizeof(NETWORK.BpPortNo2),		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpIpAddress3,	sizeof(NETWORK.BpIpAddress3),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpPortNo3,		sizeof(NETWORK.BpPortNo3),		_NETWORK_INI);
    PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.AtmIPAddress,	sizeof(NETWORK.AtmIPAddress),	_NETWORK_INI);
    PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.AtmSubnetMask,	sizeof(NETWORK.AtmSubnetMask),	_NETWORK_INI);
    PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.AtmGateWay,		sizeof(NETWORK.AtmGateWay),		_NETWORK_INI);

	PutPByten(NET_SEC, NETKEY[i++].name, &NETWORK.DeviceAddr,	sizeof(NETWORK.DeviceAddr),		_NETWORK_INI);
	PutPByten(NET_SEC, NETKEY[i++].name, &NETWORK.SelectAddr,	sizeof(NETWORK.SelectAddr),		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.SendRecvSize,	_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.LineSpeed,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.PollCheck,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.Point,			_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.Duplex,			_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.TimeFiller,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.RsType,			_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.Nrz,				_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.Clk,				_NETWORK_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Put Trans Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutTransProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.ApVersion,			sizeof(TRANS.ApVersion),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.YYYYMMDD,			sizeof(TRANS.YYYYMMDD),			_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.YYYYMMDDHHMMSS,		sizeof(TRANS.YYYYMMDDHHMMSS),	_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.SerialNo,			sizeof(TRANS.SerialNo),			_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.StartSerialNo,		sizeof(TRANS.StartSerialNo),	_TRANS_INI);
	PutPByten(TRANS_SEC, TRANKEY[i++].name, &TRANS.ClerkTransOnOff, sizeof(TRANS.ClerkTransOnOff),	_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.ProcCount,			sizeof(TRANS.ProcCount),		_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.TransPowerOnOff,									_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.EjrSerialNo,		sizeof(TRANS.EjrSerialNo),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.HostSerialNo,		sizeof(TRANS.HostSerialNo),		_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.MagamFlg,											_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceExecOff,										_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceYYYYMMDD,		sizeof(TRANS.DeviceYYYYMMDD),	_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceSprCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceJprCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceMcuCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceBrmCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceUcmCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DevicePbmCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceA4pCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceCshCnt,										_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.CheckYYYYMMDD,		sizeof(TRANS.CheckYYYYMMDD),	_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.AsPassWord,			sizeof(TRANS.AsPassWord),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.OpPassWord,			sizeof(TRANS.OpPassWord),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.GuideMsg1,			sizeof(TRANS.GuideMsg1),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.GuideMsg2,			sizeof(TRANS.GuideMsg2),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.GuideMsg3,			sizeof(TRANS.GuideMsg3),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.IngGuideMsg1,		sizeof(TRANS.IngGuideMsg1),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.IngGuideMsg2,		sizeof(TRANS.IngGuideMsg2),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.IngGuideMsg3,		sizeof(TRANS.IngGuideMsg3),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.IngGuideMsg4,		sizeof(TRANS.IngGuideMsg4),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.IngGuideMsg5,		sizeof(TRANS.IngGuideMsg5),		_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.AjmsMode,											_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.ZoomFlag,											_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.JaMaxDispAmt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.TaMaxDispAmt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.TranService,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.TranInqFee,											_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.RebootHHMMSS,		sizeof(TRANS.RebootHHMMSS),		_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.RebootFlag,											_TRANS_INI);	// U8100-97925, 97926 장애발생시 리부팅 처리 : 초기 Initialize시에만 한번 합니다	


	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Put AD Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutADProfile()
{
	int	i=0;

	EnterCriticalSection(&m_csLock);

	IniSetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, AD.MainADVersion);
	IniSetInt(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, AD.MainADSize);
	IniSetInt(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, AD.MainADMaxSize);
	IniSetInt(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, AD.MainADStartIndex);

	for (int j=0; j<AD.MainADMaxSize; j++)
	{
		IniSetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, AD.MainADItemArray[j]->ADCmd);
		IniSetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, AD.MainADItemArray[j]->ADPath);
		IniSetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, AD.MainADItemArray[j]->StartDate);
		IniSetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, AD.MainADItemArray[j]->EndDate);
		IniSetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, AD.MainADItemArray[j]->AllowList);
		IniSetStr(_MAINAD_INI, MAIN_AD_SEC, ADKEY[i++].name, AD.MainADItemArray[j]->DenyList);
	}

	IniSetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, AD.InteractiveADVersion);
	IniSetInt(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, AD.InteractiveADSize);
	IniSetInt(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, AD.InteractiveADMaxSize);
	IniSetInt(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, AD.InteractiveADStartIndex);

	for (j=0; j<AD.InteractiveADMaxSize; j++)
	{
		IniSetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, AD.InteractiveADItemArray[j]->ADCmd);
		IniSetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, AD.InteractiveADItemArray[j]->ADPath1);
		IniSetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, AD.InteractiveADItemArray[j]->ADPath2);
		IniSetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, AD.InteractiveADItemArray[j]->StartDate);
		IniSetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, AD.InteractiveADItemArray[j]->EndDate);
		IniSetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, AD.InteractiveADItemArray[j]->AllowList);
		IniSetStr(_MAINAD_INI, INTERACTIVE_AD_SEC, ADKEY[i++].name, AD.InteractiveADItemArray[j]->DenyList);
	}

	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, AD.ReceiptAD.ADVersion);
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, AD.ReceiptAD.StartDate);
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, AD.ReceiptAD.EndDate);
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, AD.ReceiptAD.AllowList);
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, AD.ReceiptAD.DenyList);
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, (AD.ReceiptAD.ReceiptLine.GetSize()>=1)?AD.ReceiptAD.ReceiptLine[0]:"");	//0
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, (AD.ReceiptAD.ReceiptLine.GetSize()>=2)?AD.ReceiptAD.ReceiptLine[0]:"");	//1
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, (AD.ReceiptAD.ReceiptLine.GetSize()>=3)?AD.ReceiptAD.ReceiptLine[0]:"");	//2
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, (AD.ReceiptAD.ReceiptLine.GetSize()>=4)?AD.ReceiptAD.ReceiptLine[0]:"");	//3
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, (AD.ReceiptAD.ReceiptLine.GetSize()>=5)?AD.ReceiptAD.ReceiptLine[0]:"");	//4
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, (AD.ReceiptAD.ReceiptLine.GetSize()>=6)?AD.ReceiptAD.ReceiptLine[0]:"");	//5
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, (AD.ReceiptAD.ReceiptLine.GetSize()>=7)?AD.ReceiptAD.ReceiptLine[0]:"");	//6
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, (AD.ReceiptAD.ReceiptLine.GetSize()>=8)?AD.ReceiptAD.ReceiptLine[0]:"");	//7
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, (AD.ReceiptAD.ReceiptLine.GetSize()>=9)?AD.ReceiptAD.ReceiptLine[0]:"");	//8
	IniSetStr(_MAINAD_INI, RECEIPT_AD_SEC, ADKEY[i++].name, (AD.ReceiptAD.ReceiptLine.GetSize()>=10)?AD.ReceiptAD.ReceiptLine[0]:"");	//9

	IniSetStr(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, AD.BannerADVersion);
	IniSetInt(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, AD.BannerADSize);
	IniSetInt(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, AD.BannerADMaxSize);
	IniSetInt(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, AD.BannerADStartIndex);
	IniSetStr(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, AD.BannerBrandADOrgList);
	IniSetStr(_MAINAD_INI, BANNER_AD_SEC, ADKEY[i++].name, AD.BannerTargetADCodeList);

	LeaveCriticalSection(&m_csLock);
}


// U8100-AP변경 LCD광고
/*==============================================================================
* Function Name : GetLCDInfoProfile()
* Description   : LCDInfo.INI 파일 읽기
* Parameters    : 
* Return        : 
* version       : 
// T3ATM AP 변경내역 #03 - 광고LCD위젯처리, 변경
* ============================================================================*/
void CProfile::GetLCDInfoProfile()
{
	int		i=0;
	
	EnterCriticalSection(&m_csLock);
	
	LCD.LCDType	= GetPInt(LCDINFO_SEC, LCDKEY[i++].name, 0,										_LCDINFO_INI);
	GetPStr(LCDINFO_SEC, LCDKEY[i++].name, 0, LCD.LCDDisplay,			sizeof(LCD.LCDDisplay),	_LCDINFO_INI);
	GetPStr(LCDINFO_SEC, LCDKEY[i++].name, 0, LCD.LCDADInfo,			sizeof(LCD.LCDADInfo),	_LCDINFO_INI);
	LCD.LCDCameraPos	= GetPInt(LCDINFO_SEC, LCDKEY[i++].name, 0,								_LCDINFO_INI);
	LCD.LCDVolume		= GetPInt(LCDINFO_SEC, LCDKEY[i++].name, 0,								_LCDINFO_INI);		// 적용후 2차 수정 (U8100 AP 변경내역 #14) - 광고 동영상 음성볼륨
	LeaveCriticalSection(&m_csLock);
}

/*==============================================================================
* Function Name : PutLCDInfoProfile()
* Description   : LCDInfo.INI 파일 쓰기
* Parameters    : 
* Return        : 
* version       : 
// U8100 AP 변경내역 #03 - 광고LCD위젯처리, 변경

* ============================================================================*/
void CProfile::PutLCDInfoProfile()
{
	int		i=0;
	
	EnterCriticalSection(&m_csLock);
	
	PutPInt(LCDINFO_SEC, LCDKEY[i++].name,		LCD.LCDType,									_LCDINFO_INI);
	PutPStr(LCDINFO_SEC, LCDKEY[i++].name,		LCD.LCDDisplay,		sizeof(LCD.LCDDisplay),		_LCDINFO_INI);
	PutPStr(LCDINFO_SEC, LCDKEY[i++].name,		LCD.LCDADInfo,		sizeof(LCD.LCDADInfo),		_LCDINFO_INI);
	PutPInt(LCDINFO_SEC, LCDKEY[i++].name,		LCD.LCDCameraPos,								_LCDINFO_INI);
	PutPInt(LCDINFO_SEC, LCDKEY[i++].name,		LCD.LCDVolume,									_LCDINFO_INI);  // 적용후 2차 수정 (U8100 AP 변경내역 #14) - 광고 동영상 음성볼륨
	LeaveCriticalSection(&m_csLock);
}


/////////////////////////////////////////////////////////////////////////////
//	Interactive AD
/////////////////////////////////////////////////////////////////////////////
void CProfile::RemoveAllADItem()
{
	for (int i=AD.MainADItemArray.GetSize()-1; i>=0; i--)
	{
		delete AD.MainADItemArray[i];
		AD.MainADItemArray.RemoveAt(i);
	}

	for (i=AD.InteractiveADItemArray.GetSize()-1; i>=0; i--)
	{
		delete AD.InteractiveADItemArray[i];
		AD.InteractiveADItemArray.RemoveAt(i);
	}

	for (i=AD.BannerADItemArray.GetSize()-1; i>=0; i--)
	{
		delete AD.BannerADItemArray[i];
		AD.BannerADItemArray.RemoveAt(i);
	}

	for (i=AD.BannerBrandADItemArray.GetSize()-1; i>=0; i--)
	{
		delete AD.BannerBrandADItemArray[i];
		AD.BannerBrandADItemArray.RemoveAt(i);
	}

	for (i=AD.BannerTargetADItemArray.GetSize()-1; i>=0; i--)
	{
		delete AD.BannerTargetADItemArray[i];
		AD.BannerTargetADItemArray.RemoveAt(i);
	}
}

int	CProfile::GetCurInteractiveADPath(int nAD, CString &strReturn)
{
	int nRet = TRUE;

	if ((!AD.InteractiveADFlag) || (!AD.InteractiveADItemArray.GetSize()))
		nRet = FALSE;

	if (nRet)
	{
		if (AD.InteractiveADStartIndex >= AD.InteractiveADItemArray.GetSize())
			AD.InteractiveADStartIndex = 0;

		if (nAD == 1)
			strReturn = AD.InteractiveADItemArray[AD.InteractiveADStartIndex]->ADPath1;
		else
			strReturn = AD.InteractiveADItemArray[AD.InteractiveADStartIndex]->ADPath2;
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetCurInteractiveADPath(%d, %s):return(%d)", nAD, strReturn, nRet);
	return nRet;
}

int	CProfile::GetCurInteractiveADCmd(CString &strReturn)
{
	int nRet = TRUE;

	if ((!AD.InteractiveADFlag) || (!AD.InteractiveADItemArray.GetSize()))
		nRet = FALSE;

	if (nRet)
	{
		if (AD.InteractiveADStartIndex >= AD.InteractiveADItemArray.GetSize())
			AD.InteractiveADStartIndex = 0;

		strReturn = AD.InteractiveADItemArray[AD.InteractiveADStartIndex]->ADCmd;
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetCurInteractiveADCmd(%s):return(%d)", strReturn, nRet);
	return nRet;
}

int CProfile::NextInteractiveAD()
{
	int nRet = FALSE;

	int nIndexSave = AD.InteractiveADStartIndex;

	CString strDate = GetDate();

	for (int i=0; i<AD.InteractiveADSize; i++)
	{
		AD.InteractiveADStartIndex ++;

		if (AD.InteractiveADStartIndex >= AD.InteractiveADSize)
			AD.InteractiveADStartIndex = 0;

		INTERACTIVEADITEM *pItem = AD.InteractiveADItemArray[AD.InteractiveADStartIndex];
		if ((strDate.CompareNoCase(pItem->StartDate) < 0)
			|| (strDate.CompareNoCase(pItem->EndDate) > 0))
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NextInteractiveAD():[%s] Date Expired! (%s ~ %s)", pItem->ADCmd, pItem->StartDate, pItem->EndDate);
			continue;
		}

		nRet = FALSE;
		if (!pItem->AllowList.CompareNoCase("ALL"))
			nRet = TRUE;
		else
		{
			CStringArray saTemp;
			SplitString(pItem->AllowList, ",", saTemp);
			if (saTemp.GetSize())
			{
				int nAllowApKind = 0;
				for (int j=0; j<saTemp.GetSize(); j++)
				{
					if (saTemp[j].IsEmpty())
						continue;

					nAllowApKind = Asc2Int(saTemp[j]);
					if (nAllowApKind < 100)	// 브랜드 구분
					{
						if (DEVICE.ETCDevice20 == nAllowApKind)
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NextInteractiveAD():[%s] Allowed by List! (%d)", pItem->ADCmd, nAllowApKind);
							nRet = TRUE;
							break;
						}
					}
					else	// 편의점 구분
					{
						if (DEVICE.ETCDevice11 == (nAllowApKind % 100))
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NextInteractiveAD():[%s] Allowed by List! (%d)", pItem->ADCmd, nAllowApKind);
							nRet = TRUE;
							break;
						}
					}
				}

				if (j >= saTemp.GetSize())
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NextInteractiveAD():[%s] Not Allowed!", pItem->ADCmd);
					nRet = FALSE;
				}
			}
		}

		if (nRet)
		{
			if (!pItem->DenyList.CompareNoCase("ALL"))
			{
				if (pItem->AllowList.IsEmpty())
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NextInteractiveAD():[%s] Deny All!", pItem->ADCmd);
					nRet = FALSE;
				}
			}
			else
			{
				CStringArray saTemp;
				SplitString(pItem->DenyList, ",", saTemp);
				if (saTemp.GetSize())
				{
					int nDenyApKind = 0;
					for (int j=0; j<saTemp.GetSize(); j++)
					{
						if (saTemp[j].IsEmpty())
							continue;

						nDenyApKind = Asc2Int(saTemp[j]);
						if (nDenyApKind < 100)	// 브랜드 구분
						{
							if (DEVICE.ETCDevice20 == nDenyApKind)
							{
								MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NextInteractiveAD():[%s] Denied by List! (%d)", pItem->ADCmd, nDenyApKind);
								nRet = FALSE;
								break;
							}
						}
						else	// 편의점 구분
						{
							if (DEVICE.ETCDevice11 == (nDenyApKind % 100))
							{
								MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NextInteractiveAD():[%s] Denied by List! (%d)", pItem->ADCmd, nDenyApKind);
								nRet = FALSE;
								break;
							}
						}
					}
				}
			}
		}
		
		if (nRet)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NextInteractiveAD():[%s]", pItem->ADCmd);
			break;
		}
	}

	AD.InteractiveADFlag = nRet;

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NextInteractiveAD():return(%d)", nRet);
	return nRet;
}

void CProfile::MakeMainADList()
{
	CString strList;
	CString strTemp;
	CString strDate = GetDate();
	CStringArray saTemp;
	int nCount = 0;
	int nAllowFlag = FALSE;

	for (int i=0; i<AD.MainADSize; i++)
	{
		MAINADITEM *pItem = AD.MainADItemArray[i];

		if ((strDate.CompareNoCase(pItem->StartDate) < 0)
			|| (strDate.CompareNoCase(pItem->EndDate) > 0))
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeMainADList():Date Expired [%s] (%s ~ %s)", pItem->ADCmd, pItem->StartDate, pItem->EndDate);
			continue;
		}

		int nADSize = Asc2Int(pItem->ADSize);
		if (nADSize <= 0)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeMainADList():[%s] 0 Size!", pItem->ADCmd);
			continue;
		}

		if (pItem->ADPath.IsEmpty())
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeMainADList():[%s] Empty Path!", pItem->ADCmd);
			continue;
		}

		nAllowFlag = FALSE;
		if (!pItem->AllowList.CompareNoCase("ALL"))
			nAllowFlag = TRUE;
		else
		{
			CStringArray saTemp;
			SplitString(pItem->AllowList, ",", saTemp);
			if (saTemp.GetSize())
			{
				int nAllowApKind = 0;
				for (int j=0; j<saTemp.GetSize(); j++)
				{
					if (saTemp[j].IsEmpty())
						continue;

					nAllowApKind = Asc2Int(saTemp[j]);
					if (nAllowApKind <= 100)	// 브랜드 구분
					{
						if (DEVICE.ETCDevice20 == nAllowApKind)
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeMainADList():[%s] Allowed by List! (%d)", pItem->ADCmd, nAllowApKind);
							nAllowFlag = TRUE;
							break;
						}
					}
					else	// 편의점 구분
					{
						if (DEVICE.ETCDevice11 == (nAllowApKind % 100))
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeMainADList():[%s] Allowed by List! (%d)", pItem->ADCmd, nAllowApKind);
							nAllowFlag = TRUE;
							break;
						}
					}
				}

				if (j >= saTemp.GetSize())
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeMainADList():[%s] Not Allowed!", pItem->ADCmd);
					nAllowFlag = FALSE;
				}
			}
		}

		if (nAllowFlag)
		{
			if (!pItem->DenyList.CompareNoCase("ALL"))
			{
				if (pItem->AllowList.IsEmpty())
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeMainADList():[%s] Deny All!", pItem->ADCmd);
					nAllowFlag = FALSE;
				}
			}
			else
			{
				CStringArray saTemp;
				SplitString(pItem->DenyList, ",", saTemp);
				if (saTemp.GetSize())
				{
					int nDenyApKind = 0;
					for (int j=0; j<saTemp.GetSize(); j++)
					{
						if (saTemp[j].IsEmpty())
							continue;

						nDenyApKind = Asc2Int(saTemp[j]);
						if (nDenyApKind <= 100)	// 브랜드 구분
						{
							if (DEVICE.ETCDevice20 == nDenyApKind)
							{
								MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeMainADList():[%s] Denied by List! (%d)", pItem->ADCmd, nDenyApKind);
								nAllowFlag = FALSE;
								break;
							}
						}
						else	// 편의점 구분
						{
							if (DEVICE.ETCDevice11 == (nDenyApKind % 100))
							{
								MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeMainADList():[%s] Denied by List! (%d)", pItem->ADCmd, nDenyApKind);
								nAllowFlag = FALSE;
								break;
							}
						}
					}
				}
			}
		}

		if (nAllowFlag)
		{
			strList.Format("|%s", pItem->ADPath);
			strTemp += strList;
			nCount += nADSize;
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeMainADList():List Added [%s, %s]", pItem->ADCmd, pItem->ADPath);
		}
	}

	strList.Format("%d%s", nCount, strTemp);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeMainADList():return[%s]", strList);
	AD.MainADList = strList;
}

CString CProfile::GetMainADList()
{
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetMainADList(): return[%s]", AD.MainADList);
	return AD.MainADList;
}

int CProfile::LoadBannerADItem(CString strSection, BANNERADITEM *pItem)
{
	int nRet = FALSE;
	CString strDate = GetDate();

	pItem->StartDate = IniGetStr(_MAINAD_INI, strSection, "StartDate", "");
	if (!pItem->StartDate.IsEmpty())
	{
		pItem->EndDate = IniGetStr(_MAINAD_INI, strSection, "EndDate", "");
		pItem->AllowList = IniGetStr(_MAINAD_INI, strSection, "AllowList", "");
		pItem->DenyList = IniGetStr(_MAINAD_INI, strSection, "DenyList", "");
		pItem->BrandOrg = IniGetInt(_MAINAD_INI, strSection, "BrandOrg", 0);
		pItem->TargetCode = IniGetStr(_MAINAD_INI, strSection, "TargetCode", "");
		pItem->ADName = IniGetStr(_MAINAD_INI, strSection, "ADName", "");
		nRet = TRUE;
	}

	if (nRet)
	{
		if ((strDate.CompareNoCase(pItem->StartDate) < 0)
			|| (strDate.CompareNoCase(pItem->EndDate) > 0))
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LoadBannerADItem(): Date Expired! (%s ~ %s)", pItem->StartDate, pItem->EndDate);
			nRet = FALSE;
		}
	}

	if (nRet)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LoadBannerADItem(): Allow:(%s), Deny:(%s)", pItem->AllowList, pItem->DenyList);
		if (!pItem->AllowList.CompareNoCase("ALL"))
			nRet = TRUE;
		else
		{
			CStringArray saTemp;
			SplitString(pItem->AllowList, ",", saTemp);
			if (saTemp.GetSize())
			{
				int nAllowApKind = 0;
				for (int j=0; j<saTemp.GetSize(); j++)
				{
					if (saTemp[j].IsEmpty())
						continue;

					nAllowApKind = Asc2Int(saTemp[j]);
					if (nAllowApKind < 100)	// 브랜드 구분
					{
						if (DEVICE.ETCDevice20 == nAllowApKind)
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LoadBannerADItem(): Allowed by List! (%d)", nAllowApKind);
							nRet = TRUE;
							break;
						}
					}
					else	// 편의점 구분
					{
						if (DEVICE.ETCDevice11 == (nAllowApKind % 100))
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LoadBannerADItem(): Allowed by List! (%d)", nAllowApKind);
							nRet = TRUE;
							break;
						}
					}
				}

				if (j >= saTemp.GetSize())
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LoadBannerADItem(): Not Allowed!");
					nRet = FALSE;
				}
			}
		}
	}

	if (nRet)
	{
		if (!pItem->DenyList.CompareNoCase("ALL"))
		{
			if (pItem->AllowList.IsEmpty())
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LoadBannerADItem(): Deny All!");
				nRet = FALSE;
			}
		}
		else
		{
			CStringArray saTemp;
			SplitString(pItem->DenyList, ",", saTemp);
			if (saTemp.GetSize())
			{
				int nDenyApKind = 0;
				for (int j=0; j<saTemp.GetSize(); j++)
				{
					if (saTemp[j].IsEmpty())
						continue;

					nDenyApKind = Asc2Int(saTemp[j]);
					if (nDenyApKind < 100)	// 브랜드 구분
					{
						if (DEVICE.ETCDevice20 == nDenyApKind)
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LoadBannerADItem(): Denied by List! (%d)", nDenyApKind);
							nRet = FALSE;
							break;
						}
					}
					else	// 편의점 구분
					{
						if (DEVICE.ETCDevice11 == (nDenyApKind % 100))
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LoadBannerADItem(): Denied by List! (%d)", nDenyApKind);
							nRet = FALSE;
							break;
						}
					}
				}
			}
		}
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LoadBannerADItem(%s):return(%d) [%s]", strSection, nRet, pItem->ADName);
	return nRet;
}

int CProfile::InitBannerAD()
{
	CString strTemp;
	CStringArray saTemp;
	int nRet = TRUE;

	for (int i=0; i<AD.BannerADSize; i++)
	{
		strTemp.Format("%s_%d", BANNER_AD_SEC, i+1);

		BANNERADITEM *pItem = new BANNERADITEM;
		nRet = LoadBannerADItem(strTemp, pItem);

		if (nRet)
			AD.BannerADItemArray.Add(pItem);
		else
			delete pItem;
	}

	if (!AD.BannerBrandADOrgList.IsEmpty())
	{
		SplitString(AD.BannerBrandADOrgList, ", ", saTemp);
		if (saTemp.GetSize() > 0)
		{
			for (int i=0; i<saTemp.GetSize(); i++)
			{
				saTemp[i].TrimLeft();
				saTemp[i].TrimRight();

				if (saTemp[i].IsEmpty())
					continue;

				strTemp.Format("%s_%s", "BrandAD", saTemp[i]);

				BANNERADITEM *pItem = new BANNERADITEM;
				nRet = LoadBannerADItem(strTemp, pItem);

				if (nRet)
					AD.BannerBrandADItemArray.Add(pItem);
				else
					delete pItem;
			}
		}
	}

	if (!AD.BannerTargetADCodeList.IsEmpty())
	{
		SplitString(AD.BannerTargetADCodeList, ", ", saTemp);
		if (saTemp.GetSize() > 0)
		{
			for (int i=0; i<saTemp.GetSize(); i++)
			{
				saTemp[i].TrimLeft();
				saTemp[i].TrimRight();

				if (saTemp[i].IsEmpty())
					continue;

				strTemp.Format("%s_%s", "TargetAD", saTemp[i]);

				BANNERADITEM *pItem = new BANNERADITEM;
				nRet = LoadBannerADItem(strTemp, pItem);

				if (nRet)
					AD.BannerTargetADItemArray.Add(pItem);
				else
					delete pItem;
			}
		}
	}

	nRet = AD.BannerADItemArray.GetSize() || AD.BannerBrandADItemArray.GetSize() || AD.BannerTargetADItemArray.GetSize();
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InitBannerAD():return(%d)", nRet);
	return nRet;
}

int CProfile::GetBannerAD(int nScreenNum, CString &strReturn)
{
	int nRet = TRUE;
	CString strTemp;

	AD.BannerADIncIndexFlag = TRUE;

	if (!AD.BannerADItemArray.GetSize() ||
		(AD.BannerADStartIndex < 0) ||
		(AD.BannerADStartIndex >= AD.BannerADItemArray.GetSize()))
		nRet = FALSE;

	if (nRet)
	{
		strReturn.Format("%d", nScreenNum);
		strTemp.Format("%s_%d", BANNER_AD_SEC, AD.BannerADStartIndex + 1);
		strReturn = IniGetStr(_MAINAD_INI, strTemp, strReturn, "");
		if (strReturn.IsEmpty())
			nRet = FALSE;
	}
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetBannerAD(%d, %s):return(%d)", nScreenNum, strReturn, nRet);
	return nRet;
}

int CProfile::NextBannerAD()
{
	int nRet = FALSE;

	if (AD.BannerADItemArray.GetSize())
	{
		if (AD.BannerADIncIndexFlag)
		{
			AD.BannerADStartIndex ++;

			if (AD.BannerADStartIndex >= AD.BannerADSize)
				AD.BannerADStartIndex = 0;

			AD.BannerADIncIndexFlag = FALSE;
			nRet = TRUE;
		}

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NextBannerAD():return(%d) [%d, %s]", nRet, AD.BannerADStartIndex, AD.BannerADItemArray[AD.BannerADStartIndex]->ADName);
	}
	
	return nRet;
}

int CProfile::GetBannerBrandAD(int nScreenNum, CString &strReturn)
{
	int nRet = TRUE;
	CString strTemp;

	if (!AD.BannerBrandADItemArray.GetSize())
		nRet = FALSE;

	if (nRet)
	{
		strReturn.Format("%d", nScreenNum);
		strTemp.Format("%s_%d", "BrandAD", AD.BannerBrandADItemArray[0]->BrandOrg);
		strReturn = IniGetStr(_MAINAD_INI, strTemp, strReturn, "");
		if (strReturn.IsEmpty())
			nRet = FALSE;
	}
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetBannerBrandAD(%d, %s):return(%d)", nScreenNum, strReturn, nRet);
	return nRet;
}

int CProfile::GetBannerTargetAD(int nTargetOrg, CString strGubun, int nScreenNum, CString &strReturn)
{
	int nRet = TRUE;
	CString strTemp;
	BANNERADITEM *pItem;

	if (!AD.BannerTargetADItemArray.GetSize())
		nRet = FALSE;

	if (nRet)
	{
		nRet = FALSE;
		strTemp.Format("%d%s", nTargetOrg, strGubun);
		for (int i=0; i<AD.BannerTargetADItemArray.GetSize(); i++)
		{
			pItem = AD.BannerTargetADItemArray[i];
			if ((pItem->TargetCode.CompareNoCase(strTemp)) == 0)
			{
				nRet = TRUE;
				break;
			}
		}
	}

	if (nRet)
	{
		strReturn.Format("%d", nScreenNum);
		strTemp.Format("%s_%s", "TargetAD", pItem->TargetCode);
		strReturn = IniGetStr(_MAINAD_INI, strTemp, strReturn, "");
		if (strReturn.IsEmpty())
			nRet = FALSE;
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetBannerTargetAD(%d, %s, %d, %s):return(%d)", nTargetOrg, strGubun, nScreenNum, strReturn, nRet);
	if (!nRet && !strGubun.IsEmpty())
		nRet = GetBannerTargetAD(nTargetOrg, "", nScreenNum, strReturn);
	
	return nRet;
}

int CProfile::InitReceiptAD()
{
	int nRet = TRUE;
	CString strDate = GetDate();

	if ((strDate.CompareNoCase(AD.ReceiptAD.StartDate) < 0)
		|| (strDate.CompareNoCase(AD.ReceiptAD.EndDate) > 0))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InitReceiptAD(): Date Expired! (%s ~ %s)", AD.ReceiptAD.StartDate, AD.ReceiptAD.EndDate);
		nRet = FALSE;
	}

	if (nRet)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InitReceiptAD(): Allow:(%s), Deny(%s)", AD.ReceiptAD.AllowList, AD.ReceiptAD.DenyList);
		if (!AD.ReceiptAD.AllowList.CompareNoCase("ALL"))
			nRet = TRUE;
		else
		{
			CStringArray saTemp;
			SplitString(AD.ReceiptAD.AllowList, ",", saTemp);
			if (saTemp.GetSize())
			{
				int nAllowApKind = 0;
				for (int j=0; j<saTemp.GetSize(); j++)
				{
					if (saTemp[j].IsEmpty())
						continue;

					nAllowApKind = Asc2Int(saTemp[j]);
					if (nAllowApKind < 100)	// 브랜드 구분
					{
						if (DEVICE.ETCDevice20 == nAllowApKind)
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InitReceiptAD(): Allowed by List! (%d)", nAllowApKind);
							nRet = TRUE;
							break;
						}
					}
					else	// 편의점 구분
					{
						if (DEVICE.ETCDevice11 == (nAllowApKind % 100))
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InitReceiptAD(): Allowed by List! (%d)", nAllowApKind);
							nRet = TRUE;
							break;
						}
					}
				}

				if (j >= saTemp.GetSize())
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InitReceiptAD(): Not Allowed!");
					nRet = FALSE;
				}
			}
		}
	}

	if (nRet)
	{
		if (!AD.ReceiptAD.DenyList.CompareNoCase("ALL"))
		{
			if (AD.ReceiptAD.AllowList.IsEmpty())
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InitReceiptAD(): Deny All!");
				nRet = FALSE;
			}
		}
		else
		{
			CStringArray saTemp;
			SplitString(AD.ReceiptAD.DenyList, ",", saTemp);
			if (saTemp.GetSize())
			{
				int nDenyApKind = 0;
				for (int j=0; j<saTemp.GetSize(); j++)
				{
					if (saTemp[j].IsEmpty())
						continue;

					nDenyApKind = Asc2Int(saTemp[j]);
					if (nDenyApKind < 100)	// 브랜드 구분
					{
						if (DEVICE.ETCDevice20 == nDenyApKind)
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InitReceiptAD(): Denied by List! (%d)", nDenyApKind);
							nRet = FALSE;
							break;
						}
					}
					else	// 편의점 구분
					{
						if (DEVICE.ETCDevice11 == (nDenyApKind % 100))
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InitReceiptAD(): Denied by List! (%d)", nDenyApKind);
							nRet = FALSE;
							break;
						}
					}
				}
			}
		}
	}	

	AD.ReceiptADFlag = nRet;
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InitReceiptAD():return(%d)", nRet);
	return nRet;
}

int CProfile::GetReceiptADText(int nLine, CString &strReturn)
{
	int nRet = TRUE;

	if (!AD.ReceiptADFlag)
		nRet = FALSE;

	if (nLine >= AD.ReceiptAD.ReceiptLine.GetSize())
		nRet = FALSE;

	if (nRet)
		strReturn = AD.ReceiptAD.ReceiptLine[nLine];

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetReceiptADText(%d, %s):return(%d)", nLine, strReturn, nRet);
	return nRet;
}

int CProfile::GetReceiptADImagePath(int nScreenNum, CString &strReturn)
{
	int nRet = TRUE;

	if (!AD.ReceiptADFlag)
		nRet = FALSE;

	if (nRet)
	{
		strReturn.Format("%d", nScreenNum);
		strReturn = IniGetStr(_MAINAD_INI, RECEIPT_AD_SEC, strReturn, "");
		if (strReturn.IsEmpty())
			nRet = FALSE;
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetReceiptADImagePath(%d, %s):return(%d)", nScreenNum, strReturn, nRet);
	return nRet;
}
