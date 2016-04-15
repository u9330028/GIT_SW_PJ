#if !defined(AFX_DEVCMN_H__C1CA462D_218C_4BA9_88BA_DBC13B14F86A__INCLUDED_)
#define AFX_DEVCMN_H__C1CA462D_218C_4BA9_88BA_DBC13B14F86A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DevCmn.h : header file
//

#include <wininet.h>

#include "..\..\H\Common\Class.h"
#include "..\..\WinAtm\nhmwi.h"
#include "..\..\WinAtm\nhalert4spackx.h"
#include "..\..\H\Dll\DevICCard.h"
#include "..\..\WinAtm\nhiccm.h"
#include "..\..\WinAtm\nhmwiemv.h"

#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

//#include "..\..\H\Seed\qsl.h"		// for SEED
//#include "..\..\H\Seed\qslkey.h"	// for SEED

///////////////////////////	�ѱ��Է�
#ifndef HANINPUT_H
	#include "HanInput.h"
#endif
///////////////////////////

#include "..\..\H\Dll\INIT_DLL.h"

DllUseport	BOOL WINAPI EmbossBmp(LPCTSTR imgPathName);		// EmbossDll.lib���

#include "MMSystem.h"
typedef struct DBInfo {
	char	szName[5];
	int		nGender;
	int		nLoad;
} DBINFO;

/////////////////////////////////////////////////////////////////////////////
// CDevCmn window
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CDevCmn : public CWnd
{
/////////////////////////////////////////////////////////////////////////////
// Attributes
/////////////////////////////////////////////////////////////////////////////
public:
#include "..\..\H\Common\ClassPointer.h"
//#include "..\..\H\Dll\DevSeed.h"
	
	CNHMWI*	m_pMwi;						// Mwi Pointer
	CNHICCM* m_pICCM;					// IC Common Module Pointer		// V09-33-00-#01
	CNHMwiEmv*	m_pMwiEmv;				// NHMwiEMV Pointer				// V09-33-00-#01
	CNHAlert4SPackX*	m_NHAlertSpack;			// CNHAlert4SPackX Pointer
	DEVICEINFO	AtmDefine;				// Atm��ⱸ�� ���κ���(����Ұ�)

	int		SprRowOption;				// bank/Ticket Row option
	int		HostLineMode;				// Online/Local
	int		HostLocalSvrMode;			// Online/Local
	int		HostSvrPortMode;			// Host Online/Local
	int		BrmCycleMode;				// Recycle/Uncycle
	int		BrmCashMode;				// RealCash/TestCash
	int		IdcVersion;					// ����IC ���� Version
	int		EjectBothAvail;				// V07-05-01-#02:ī��/��ǥ ���ù��� ���ɿ���
	int		ChkIcVersion;				// T99-99-99-#01 IC ���� �б� ����	

	int		GroDateMode;				// DateCheck/NotCheck
	int		GroCheckMode;				// DataCheck/NotCheck
	int		GroEscrowFlg;				// Escrow���࿩�� ǰ���������� ���� �߰� 05.02.28 NKB

	int		AtmDoing;					// Atm���࿩��
	int		AtmFirstDoingFlag;			// Atm���ʽ���Flag
	int		AtmStatus;					// Atm���º��� 
	int		AtmStatusSave;				// Atm�������º���
	int		AtmStatusReEntryFlag;		// Atm����������Flag
	int		HostStatus;					// Host���º��� 
	int		HostStatusSave;				// Host�������º���
	int		HostFirstOpenFlag;			// Host���ʰ���Flag
	int		HostOpenFlag;				// Host����Flag
	int		HostFirstReadyFlag;			// Host���ʻ���Flag
	int		PowerOffSendFlag;			// PowerOff�����۽ź���
	int		TranStatus;					// Tran���º��� 
	int		TranStatusSave;				// Tran�������º��� 
	int		TranProc;					// Tran�ŷ�ó��
	int		TranResult;					// Tran�ŷ����

	int		DeviceDefine;				// ��⿡ ���ǵ�	Device Set
	int		DeviceNoDevice;				// �̿���			Device Set
	int		DeviceDownDevice;			// Device Down Device Set
	int		DeviceStatus;				// ��ְ� ��		Device Set
	int		DeviceAutoOff;				// �ڵ����� ��	Device Set
	int		DeviceRetract;				// ȸ���� ��		Device Set
	int		DeviceTran;					// ���� Event		Device Set 
	int		DeviceTimeout;				// �̼�������		Device set
	int		DeviceLoss;					// �̼������		Device set

	int		ForceDownFlag;				// ����Down����
	int		RemoteDownFlag;				// ����Down����
	int		ForceMissMatchDownFlag;     // MisMatch


	int		IcmRecvTime;				// Icm����Time
	int		SprRecvTime;				// Spr����Time
	int		JprRecvTime;				// Jpr����Time
	int		McuRecvTime;				// Mcu����Time
	int		BrmRecvTime;				// Brm����Time
	int		UcmRecvTime;				// Ucm����Time
	int		PbmRecvTime;				// Pbm����Time
	int		VfdRecvTime;				// VFD����Time		// T2ATM 2006.5.24 yscho
	int		A4pRecvTime;				// A4P����Time
	int		GroRecvTime;				// Gro����Time
	int		RfdRecvTime;				// Rfd����Time

    int		OpenKey;					// '�غ�' '�' ��ȯ����ġ 
    int		OpenKeySave;				// '�غ�' '�' ��ȯ����ġ ����
    int		ClerkKey;					// �������  
    int		ClerkKeySave;				// ������� ����
	int		PowerOffDetect;				// Power ���º���
	int		RecoveryType;				// 0:�ڵ�����,1:��������
	int		TranType;					// 0:���ŷ���,1:�������(��ּ۽Ű���)

	int		CurErrCnt;					// ������ְ���
	ERRSTACK	CurErrBuff;				// ������ֳ���
	ERRSTACK	FirstErrBuff;			// ������ֳ���
	ERRSTACK	LastErrBuff;			// ������ֳ���

	char	OrignalErrorCode[8];		// V07-01-04-#02:��ֹ߻��� ���� ErrorCode����
	BYTE	UserDefineSetErrCode[8];		// UserDefineSet Err (Van-CD Spec)
	
	int		CSTCnt;						// CST Count
	int		FstCSTValue;				// First CST Value
	int		SndCSTValue;				// Second CST Value
	int		TrdCSTValue;				// Third CST Value
	int		FthCSTValue;				// Fourth CST Value
	int		FstCSTSetCash;				// First CST Set Cash
	int		SndCSTSetCash;				// Second CST Set Cash
	int		TrdCSTSetCash;				// Third CST Set Cash
	int		FthCSTSetCash;				// Fourth CST Set Cash
	int		FstCSTCnt;					// First CST Count(ClerkInformation)
	int		SndCSTCnt;					// Second CST Count(ClerkInformation)
	int		TrdCSTCnt;					// Third CST Count(ClerkInformation)
	int		FthCSTCnt;					// Fourth CST Count(ClerkInformation)

	int		SCSTCntF;					// CST Count
	int		SFstCSTValue;				// First CST Value
	int		SSndCSTValue;				// Second CST Value
	int		STrdCSTValue;				// Third CST Value
	int		SFthCSTValue;				// Fourth CST Value
	int		SFstCSTSetCash;				// First CST Set Cash
	int		SSndCSTSetCash;				// Second CST Set Cash
	int		STrdCSTSetCash;				// Third CST Set Cash
	int		SFthCSTSetCash;				// Fourth CST Set Cash
	int		SFstCSTCnt;					// First CST Count(ClerkInformation)
	int		SSndCSTCnt;					// Second CST Count(ClerkInformation)
	int		STrdCSTCnt;					// Third CST Count(ClerkInformation)
	int		SFthCSTCnt;					// Fourth CST Count(ClerkInformation)
	int		STotCSTCntF;				// Fourth CST Count(ClerkInformation)
	int		SWithMaxAmt;				// nnWithMaxAmt(ClerkInformation)

	int		CshChangeLoc;				// Csh Change Location
	int		CshLoc;						// Csh Location
	int		BrmChangeLoc;				// Brm����ġ�������
	int		BrmLoc;						// Brm����ġ����
	int		RejectCSTLoc;				// ����Ʈ������ġ����
	int		RetractCSTLoc;               // ȸ����
	int		ClerkCSTLoc;				// ����������ġ����
	int		FstCSTLoc;					// 1ī��Ʈ����ġ����(Cd)
	int		SndCSTLoc;					// 2ī��Ʈ����ġ����(Cd)
	int		TrdCSTLoc;					// 3ī��Ʈ����ġ����(Cd)
	int		FthCSTLoc;					// Fourth CST Location(Cd)
	int		UcmLoc;						// Ucm����ġ����
	int		DoorStatus;					// DoorStatus����
	int		DoorStatusSave;				// DoorStatus���� ����
	int		CabinetStatus;				// CabinetStatus
	int		SaveCabinetStatus;			// CabinetStatus���� ����

	int		SlipHeadStatus;				// ��ǥCover����
	int		JnlHeadStatus;				// ����Cover����
	int		A4pHeadStatus;				// A4pCover����
	int		SlipStatus;					// ��ǥ����
	int		JnlStatus;					// ���λ���
	int		A4pStatus;					// A4��������
	int		GAlarmStatus;				// �˶���������
	int		GExternalSensorStatus1;		// �ܺμ���1
	int		GExternalSensorStatus2;		// �ܺμ���2
	int		GSensorStatus;				// ��ݼ�������
	int		GSensorLelvel;				// ��ݼ�������
	int		GBatteryStatus;				// �嵥����������

	int		RejectCSTStatus;			// ȸ��ī��Ʈ����
	int		RetractCSTStatus;			// ȸ��������ġ����
	int		ClerkCSTStatus;				// ���ī��Ʈ�Ի���
	int		RecycleBoxStatus;			// ���ݻ���1
	int		FstCSTStatus;				// 1ī��Ʈ����(Cd)
	int		SndCSTStatus;				// 2ī��Ʈ����(Cd)
	int		TrdCSTStatus;				// 3ī��Ʈ����(Cd)
	int		FthCSTStatus;				// 4ī��Ʈ����(Cd)
	int		CheckCSTStatus;				// ��ݼ�ǥ����
	int		CheckCSTStatusSensor;		// ��ݼ�ǥ����(�ǹ�)
	int		CheckCSTStatusReg;			// ��ݼ�ǥ����(���)
	int		CassetteStatus;				// �Աݼ�ǥ����

	int		ClerkRejectBoxStatus;		// ����RejectBox���� V06-01-01-#18
	int		RecycleBox50000Status;		// ���ݻ���(5����) V06-01-01-#19

	int		CaptureLastIndex;			// ī�޶���������Index
	int		CameraStatus;           	// ī�޶���������Index
	
	int		DesBoardStatus;				// Des Board Status
	int		DesITKStatus;				// Des ITK Status
	int		DesTMKStatus;				// Des TMK Status
	BYTE	DesRPIN[17];				// Des RPIN
	int		DesTMKTryFlag;				// Des TMK Try Flag
	int		DesTranStatus;				// Des Tran Status
	int		DesOpenRetryCnt;			// Des Open Retry Count

	int		SeedEnStatus;				// Seed Enable Status
	int		SeedTranStatus;				// Seed Tran Status
	int		SeedSendRetryCnt;			// Seed Send Retry Count

	int		RF_IrMD_Flg;				// RF ����� ����

	int		TranAvail;					// ��ü�ŷ�����
	int		WithAvail;					// ���ްŷ�����
	int		DepAvail;					// �Աݰŷ�����
	int		PbmAvail;					// ����ŷ�����
	int		A4pAvail;					// ���ްŷ�����
	int		GroAvail;					// �Աݰŷ�����

	int		UserTranAvail;				// User��ü�ŷ����� 2003.12.15
	int		UserWithAvail;				// User���ްŷ�����
	int		UserDepAvail;				// User�Աݰŷ�����
	int		UserPbmAvail;				// User����ŷ�����
	int		UserA4pAvail;				// User���ްŷ�����
	int		UserGroAvail;				// User�Աݰŷ�����
	int		Cmr1Status;					// ī�޶�1 ����
	int		Cmr2Status;					// ī�޶�2 ����
	
	int		HostOpenRetryCnt;			// �����õ�Ƚ��(�غ������)
	int		bReadyCountFlg;				// 

	int		bEscrowFlg;					// ESCROWó������

	int		OldCashDepMode;				
	int		OldCasetteType;				
	int		TTSEngineLoadFlg;			

	int		RfdSeq;						
	int		RfdTestStatus;				
	char	RfdVersion[2];				
	int		Rfd3GAvail;					
	char	RfErrorCode[3];				
	int		RfErrorCodeIdx;				

	int		CheckMachineType;			
	int		ChangeBCFlag;				
	int		Withdraw50TFlag;			

	int		FNG_Status;					// Fng Status 
	int		BrmCshInfo;					// BRM CashInfo (���� ���Ա��� ���� ��/��)
	int		BrmCsmIn;					// BRM BrmCsmIn (���� ���Ա��� ���� ��/��)

	int		OSType;						// WIN7 ���� #N0228

	CStringArray	strMsgArray; //Remote Ctrl
	// U8100 AP ���泻�� #02 - ī�޶� ������ ����
	char	VODCapturePath[128];		
	
	BOOL	VODFaceRecording;
	BOOL	VODHandRecording;
	BOOL	VODRoomRecording;
	CString RealVodFile;				// ������ 1�� ���� (U8100 AP ���泻�� #13) - ī�޶� ������
	
	BOOL	bVODIdx;

	BOOL	bNearBranchSelectBtn;		// �α��������Ϻ��� ��ư ���ý�		#N0172

	int		Mcu97933Error;				// 97933 ���					// V09-33-00-#01  ICCM �������
	int		Mcu97934Error;				// 97934 ���					// V09-33-00-#01  ICCM �������
	int		IsEmvChipPowerOn;			// EMV Chip PowerOn Flag		// V09-33-00-#01  ICCM �������

struct	tagTimer						// Ÿ�Ӻ�������
{
	int		HostWaitLinkTime;			// ������õ�Ÿ��
	int		HostOpenRetryTime;			// ������õ�Ÿ��
	int		HostAliveTime;
	int		HostRebRetryTime;
	int		HostStatusSendTime;
} Timer;

	EjrTbl			Ejr;				// �������ο���
	EjrAmountTbl	EjrAmount;			// �������αݾװ����ڷ�
	EEjrTblDump		EEjrD;				// �������ο���

struct	tagScr							// ȭ�麯������
{
	int		CurrentScreenNo;			// ����ȭ���ȣ
	int		CurrentLangMode;			// ��������
	char	MachineFileType[4];            // ������(ȭ���̸�)
} Scr;

struct	tagOm							// Om����
{
	// Om��������:��Ư�� ��ġ���� ���ϴ°� ����
	CString	ProcCount;					// ����ī����
	CString	ErrorCode;					// ����ڵ�
	CString	McuDeviceStatus;			// Mcu��ġ����(����,���)
	CString	McuProcCount;				// Mcu�������ī����(����=' ',' ','0','1','3','5','6','9')
	CString	McuErrorCode;				// Mcu����ڵ�(����="0000000",���="1234567")
	CString	SprDeviceStatus;			// Spr��ġ����
	CString	SprProcCount;				// Spr�������ī����
	CString	SprErrorCode;				// Spr����ڵ�
	CString	JprDeviceStatus;			// Jpr��ġ����
	CString	JprProcCount;				// Jpr�������ī����
	CString	JprErrorCode;				// Jpr����ڵ�
	CString	PbmDeviceStatus;			// Pbm��ġ����
	CString	PbmProcCount;				// Pbm�������ī����
	CString	PbmErrorCode;				// Pbm����ڵ�
	CString	UcmDeviceStatus;			// Ucm��ġ����
	CString	UcmProcCount;				// Ucm�������ī����
	CString	UcmErrorCode;				// Ucm����ڵ�
	CString	BrmDeviceStatus;			// Brm��ġ����
	CString	BrmProcCount;				// Brm�������ī����
	CString	BrmErrorCode;				// Brm����ڵ�
	CString	NetDeviceStatus;			// Net��ġ����
	CString	NetProcCount;				// Net�������ī����
	CString	NetErrorCode;				// Net����ڵ�
	CString	AppDeviceStatus;			// App��ġ����
	CString	AppProcCount;				// App�������ī����
	CString	AppErrorCode;				// App����ڵ�
	CString	A4pDeviceStatus;			// A4P��ġ����
	CString	A4pProcCount;				// A4P�������ī����
	CString A4pErrorCode;				// A4P����ڵ�
	CString	SendRecvStatus;				// �ۼ��Ż���(ON,OFF)
	CString	OperationStatus;			// ���۰��ɻ���(ON,OFF)
	CString	CheckKind;					// ��ǥ����(10,50,100)
	CString CasetteType;				// ī��Ʈ��������(������,����õ)
	CString Sub_DepKindReg;				// �Աݱ�������

	
	// ��������(NH2700 - Only )				
	CString	Main_CD_VanIII_Status;				// OM_���_VANIII_��Ȳ
	CString	Main_CD_VanIII_ModeStatus;			// OM_������_VANIII_��Ȳ
	CString	Main_CD_VanIII_ApSpEpVersion;			// OM_������_VANIII_��Ȳ


	// ��������(�������1)				
	CString	Main_1LineStatus;			// OM_�������1
										// 0:ATMSTATUS(INIT,ERROR,CLERK,READY,CUSTOM,TRAN)
										// 1:������ݻ���(����,�Ұ�)
										// 2:�����Աݻ���(����,�Ұ�)
										// 3:��ǥ��ݻ���(����,�Ұ�)
										// 4:��ǥ�Աݻ���(����,�Ұ�)
										// 5:����ŷ�����(����,�Ұ�)
										// 6:����ȭ�����(����,�Ұ�)
										// 7:�ؿܰŷ�����(����,�Ұ�)
										// 8:�����ŷ�����(����,�Ұ�)
										// 9:���ܼ��ŷ�����(����,�Ұ�)
										// 10:���ڵ�ŷ�����(����,�Ұ�)
										// 11:���͸�������(0-100)
										// 12:���͸�����(����,����)
										// 13:��Ż���(ONLINE,OFFLINE)
										// 14:�޹�����(����,����)
										// 15:�������ġ����(ON,OFF)

	// ��������(�������2)
	CString	Main_2LineStatus;			// OM_�������2
										// 0:����(3�ڸ�)
										// 1:���(2�ڸ�)
										// 2:�������(IOC,HDLC,SWP,SDLC,TCP/IP,CCL,BAS)
										// 3:APVERSION(V01-01-01)
										// 4:LOCALMODE(ON,OFF)
										// 5:�����Ǹ��(����,����)
										// 6:����ī����(' ','0','1','3','5','6','9')
										// 7:����ڵ�(1234567)

	// ��������(�������ݺ�����Ȳ)
	CString	Main_CashStatus;			// OM_�������ݺ�����Ȳ
										// 0:ī��Ʈ1�����
										// 1:ī��Ʈ2�����
										// 2:ī��Ʈ3�����

	// ��������(�����ܷ�����)
	CString	Main_Status;				// OM_�����ܷ�����
										// 0:��ǥ�ܷ�����(����,�ܷ���,�亸��)
										// 1:�����ܷ�����(����,�ܷ���,�亸��)
										// 2:��ݼ�ǥ�ܷ�����(����,�ܷ���,�亸��)
										// 3:�Աݼ�ǥ�ܷ�����(����,�ܷ���)
										// 4:�����ܷ�����(����,�ܷ���,�亸��,�ܷ���)

	// ��������(���μ�ǥ������Ȳ)
	CString	Main_CheckStatus;			// OM_���μ�ǥ������Ȳ
										// 0:��ݼ�ǥ�ż�
										// 1:�Աݼ�ǥ�ż�

	// ��������(��������ȸ����Ȳ)
	CString	Main_CashRejectStatus;		// OM_��������ȸ����Ȳ
										// 0:���ȸ���Ըż�(�н�,�ܷ�)
										// 1:�ϴ�ȸ���Ըż�(��ݺҷ�����,�Աݺҷ�����,��õ��,õ��)

	// ��������(������ġ��Ȳ)
	CString	Main_DeviceStatus;			// OM_������ġ��Ȳ
										// 0:Mcu����(����,���)
										// 1:Mcu�������ī����(����='0',' ','0','1','3','5','6','9')
										// 2:Mcu����ڵ�(����="0000000",���="1234567")

										// 3:Spr����
										// 4:Spr�������ī����
										// 5:Spr����ڵ�
										// 6:Spr�����ǥ�ܷ�����(����,�ܷ���,�亸��)
										// 7:Spr�����ǥ�ܷ�����(��,��)

										// 8:Jpr����
										// 9:Jpr�������ī����
										// 10:Jpr����ڵ�
										// 11:Jpr����1�ܷ�����(����,�ܷ���,�亸��)
										// 12:Jpr����2�ܷ�����(����,�ܷ���,�亸��)

										// 13:Pbm����
										// 14:Pbm�������ī����
										// 15:Pbm����ڵ�

										// 16:Ucm����
										// 17:Ucm�������ī����
										// 18:Ucm����ڵ�
										// 19:Ucm����ġ(����,Ż��)
										// 20:Ucm��ݼ�ǥ�ܷ�����(����,�ܷ���,�亸��)
										// 21:Ucm�Աݼ�ǥ�ܷ�����(����,�ܷ���)

										// 22:Brm����
										// 23:Brm�������ī����
										// 24:Brm����ڵ�
										// 25:Brm����ġ(����,Ż��)
										// 26:Brm����ī��Ʈ����(����,�ܷ���,�亸��,�ܷ���)
										// 27:Brm���ī��Ʈ����(����,Ż��)
										// 28:Brmȸ���Ի���(����,�ܷ���,Ż��)
										// 29:Brm��������

										// 30:Net����
										// 31:Net�������ī����
										// 32:Net����ڵ�

										// 33:App����
										// 34:App�������ī����
										// 35:App����ڵ�

	// ��������(���κΰ�����)	
	CString	Main_ReffInfomation;		// OM_���κΰ�����
										// 0:�,�غ�
										// 1:�ۼ��Ż���(ON,OFF)
										// 2:���۰��ɻ���(ON,OFF)
										// 3:��ǥ����(10,50,100)

	// ��ݼ�ǥ���	
	CString Sub_WithCheckReg;			// OM_��ݼ�ǥ���
										// 0:��ݼ�ǥ��ȣ
										// 1:��ݼ�ǥ�ż�

	// ��ġ����
	CString Sub_InstInformation;		// OM_��ġ����
										// 0:��ġ��¥(YYYYMMDD)
										// 1:����(NNNNNNNN)
										// 2:���(NNNNNNNN)
										// 3:��ġ���(NNNNNNNN)
										// 4:����(ON,OFF)
										// 5:����(NNNNNNNN)
										// 6:��������(ON,OFF)
										// 7:24�ð��(ON,OFF)
										// 8:365�(ON,OFF)
										// 9:CE�繫���ڵ�(NNNN)
										// 10:��������ȣ(NNNNNNNNNNNN)
										// 11:�����(NNNNNNNN)
										// 12:��ġ��(NNNNNNNNNN)
										// 13:��ġȮ����(NNNNNNNNNN)
										// 14:���������(NNNNNNNNNN)
										// 15:��������ڿ���ó(NNNNNNNNNNNNNNNN)

	// �ΰ���ġ����
	CString Sub_SubDeviceReg;			// OM_�ΰ���ġ����
										// 0:����ȭ��(����,�Ұ�)
										// 1:�ؿܰŷ�(����,�Ұ�)
										// 2:�����ŷ�(����,�Ұ�)
										// 3:���ܼ��ŷ�(����,�Ұ�)
										// 4:���ڵ�ŷ�(����,�Ұ�)

	// Lan��������
	CString	Sub_LanInformation;			// OM_LAN��������
										// 0:IP�ּ�(NNN.NNN.NNN.NNN)
										// 1:����ݸ���ũ(NNN.NNN.NNN.NNN)
										// 2:����Ʈ����(NNN.NNN.NNN.NNN)
										// 3:BP1IP�ּ�(NNN.NNN.NNN.NNN)
										// 4:BP1PORT(NNNNN)
										// 5:BP2IP�ּ�(NNN.NNN.NNN.NNN)
										// 6:BP2PORT(NNNNN)
										// 7:BP3IP�ּ�(NNN.NNN.NNN.NNN)
										// 8:BP3PORT(NNNNN)

	// ȸ������
	CString	Sub_LineInformation;		// OM_ȸ������
										// 0:�������(IOC,HDLC,SWP,SDLC,TCP/IP,CCL,BAS)
										// 1:DA(XX)
										// 2:SA(XX)
										// 3:�ۼ��Ż�����
										// 4:ȸ���ӵ�(1200,2400,4800,9600,19200)
										// 5:POLLCHECK(ON,OFF)
										// 6:POINT(PTP,MTP)
										// 7:DUPLEX(HALF,FULL)
										// 8:TIMEFILLER(MTF,FTF)
										// 9:RSTYPE(RS-422,RS-232C)
										// 10:NRZ(NRZ,NRZI)
										// 11:CLOCK(INCLK,EXCLK)

	// ATMS Ftp_DownLoad server ��������
	CString	Sub_ATMSServerInformation;	// OM_ATMS��������
										// 0:SERVER IP�ּ�(NNN.NNN.NNN.NNN)
										// 1:SERVER PORT(NNNNN)
										// 2:����SERVER1 IP�ּ�(NNN.NNN.NNN.NNN)
										// 3:����SERVER2 IP�ּ�(NNN.NNN.NNN.NNN)
										// 4:����SERVER3 IP�ּ�(NNN.NNN.NNN.NNN)

	// �ŷ���������
	CString	Sub_TranInformationReg;		// OM_�ŷ���������
										// 0:�������(����,�Ұ�)
										// 1:�����Ա�(����,�Ұ�)
										// 2:��ǥ���(����,�Ұ�)
										// 3:��ǥ�Ա�(����,�Ұ�)
										// 4:����ŷ�(����,�Ұ�)

	// ���ݺ���ȸ��
	CString	Sub_CashRefillTakeup;		// OM_���ݺ���ȸ��
										// 0:ī��Ʈ1�����
										// 1:ī��Ʈ2�����
										// 2:ī��Ʈ3�����
										// 3:Brm����(����,���)

	// ��⼳��
	CString	Sub_MachineType;			// OM_��⼳��
										// 0:�������
										// 1:��ǥ������

	// �ڵ�������
	CString	Sub_AutoComputeNote;		// OM_�ڵ���꼳��		
										// 5:5��������			
										// 10:10��������		

	CString Sub_CstStateInfo;			// OM_CDP CST INFO		
										// 1:ī��Ʈ1����		
										// 2:ī��Ʈ2����		
										// 3:ī��Ʈ3����		
										// 4:ī��Ʈ4����		

	CString	Sub_SerialUpdate;			// OM_�ŷ��Ϸù�ȣ����
										// 0:�Ϸù�ȣ

	// T3ATM AP ���泻�� #19 - ī�޶� 3���� ���¸� ��� ǥ���ϱ� ����
	CString	Sub_CamerStatus;
	
} Om, OmSave;

	int		IfmDirect;
	int		IfmDirect_Init;
	int		IfmResult;
	
	BYTE	FICEPVersion[32];			// IC ī�� ����

	BYTE	ICSendData[BASEBUFFSIZE];	// IC�����ڷ�
	BYTE	ICRecvData[BASEBUFFSIZE];	// IC�����ڷ�
	BYTE	m_cbFCError[4];				// �����ڵ� ��������
	char	m_szErrCode[10];			// Error Code for Screen
	char	m_szErrMsg[20];				// Error Message for Screen

struct tagKCashInfo						// K-CASH��������
{
	BYTE	CardInform[4];				// ī�屸��
	BYTE	CardNum[8];					// ī���ȣ
	char	CardIssueDate[8];			// ī��߱�����(8BYTE)

	BYTE	UserCode;					// ����������   // 01 : �Ϲ���	, 02 : �ʵ�	 , 03 : �߰�	, 04 : ���л�
														// 05 : ���	, 06 : �����, 07 : Test

	BYTE	ValidDate[2];				// ��ȿ�Ⱓ		// YYMM

	BYTE	Account[10];				// ��������

	BYTE	Balance[4];					// �ܾ�
	BYTE	MaxLimitAmount[4];			// �����ѵ��ݾ�
	BYTE	MaxLoadUnloadAmount[4];		// 1ȸ����ѵ�			

	BYTE	EnglishName[24];			// �����̸�
	BYTE	KoreanName[24];				// �ѱ��̸�
	BYTE	CitizenNumber[13];			// �Ǹ��ȣ
	
	BYTE	MainAccount[52];			// �ְ�������

	BYTE	LoadUnloadAlgorithmID;		// �˰���ID
	BYTE	LoadUnloadKeyVersion;		// Ű����
	BYTE	LoadUnloadSerialNo[2];		// �Ϸù�ȣ
	BYTE	LoadUnloadRandomNum[8];		// ����
	BYTE	LoadUnloadEPMakerID[3];		// ������ID
	BYTE	LoadUnloadEPID[5];			// ID
	BYTE	LoadUnloadBalance[4];		// �ܾ�
	BYTE	LoadUnloadAccount[10];		// ��������
	BYTE	LoadUnloadSignature[4];		// ����

	BYTE	LoadUnloadSignature3[4];	// ����3

	BYTE	LoadUnloadCnt;				// ����/ȯ�ҳ����Ǽ�
	BYTE	LoadUnloadBuff[16][13];		// ����/ȯ�ҳ���

	BYTE	BuyListCnt;					// ���Űŷ������Ǽ�
	BYTE	BuyListBuff[16][28];		// ���Űŷ�����			2004.02.16

	BYTE	LoadUnloadProof[7];			// Transaction Proof
} KCashInfo;

////////////////////////////////////////////////////////////////////////////////
// IC Use Variables. 20040126-HSW-A
////////////////////////////////////////////////////////////////////////////////
	int		FICCardType;				// ICī��Ÿ��(�����/NATIVE/��������)
	int		FICDFType;					// ����IC DF Type(����/����ȭ��/�ſ�����)
	BYTE	FICAppletType;				// ICī�� APPLET����

	int		FICAccountCount;			// ���� ����
	char	FICAccount[50];				// ���¹�ȣ
	char	FICBankCode[10];			// Bank Code

	char	m_EMVszData[IC_DATA_LEN*2 + 1];			// EMV Send RecvData ����

	char	m_szEMVFBCode[8];						// ���� �ŷ��� ���� ���� �ڵ� 
	int		EmvCardService;							// Fall back ������
	int		m_SelectIndex;							// ���Ӱŷ��� ���õ� 

	char	m_btEMVTerminalConstant[1024];			//  FB�� ����
	int		m_nEMVTerminalConstantLen;				//  FB�� ����

//������������������������������������������������������������������������������
// b8	b7	b6	b5	b4	b3	b2	b1		�� ��
//������������������������������������������������������������������������������
//								1		���� IC
//							1			K-Cash
//						1				EMV
//					1					������(�����). Ű �й���� �Ϲ� ī�忡�� ������ ���� ������ ���δ�.
//				X						RFU
//			X							RFU
//		X								RFU		
//	1									������ ī��(0), ����� ī��(1)						
//������������������������������������������������������������������������������

struct tagICTypeInfo					// IC CHIP TYPE ��������
{
	BYTE	ICTypeConfirm;				// ī�屸��
	BYTE	ICTypeConfirm2;				// �������� �󵦽�/��������(���� �������࿡���� ���)
} ICTypeInfo;

////////////////////////////////////////////////////////////////////////////////
//  ���� IC Use Structure. 20040126-HSW-A
////////////////////////////////////////////////////////////////////////////////
//  ���� IC Infomation
struct tagFinanceICInfo {
	BYTE	FCI_FInformation[8];		// FCI ����Ү ��������(8 Bytes)
	BYTE	FCI_AppLabelLen;			// FCI Application Label ����
	char	FCI_AppLabel[30];			// FCI Application Label
	BYTE	FCI_StandardVersion;		// FCI ǥ��ȭ ����
	BYTE	FCI_FileKeyVersion;			// FCI File Key Version
	BYTE	CAN[8];						// Card Application Number
	BYTE	CardHolderNameLen;			// ī�� ������ �̸� ����
	char	CardHolderName[27];			// ī�� ������ �̸�
	BYTE	RealNameNOLen;				// �Ǹ��ȣ ����
	BYTE	RealNameType;				// �Ǹ���
	char	RealNameNO[15];				// �Ǹ��ȣ
	BYTE	ExpireDayLen;				// ������ ����
	char	ExpireDay[10];				// ������
	BYTE	AlgorithmCode;				// �˰��� �ĺ��ڵ�
	BYTE	IssureCodeLen;				// ������ �ĺ��ڵ� ����
	char	IssureCode[6];				// ������ �ĺ��ڵ�
	BYTE	CSN[8];						// ī�� �Ϸù�ȣ(Card Serial Number)
	char	szCSN[20];					// ī�� �Ϸù�ȣ for Screen
	BYTE	TerminalRandom[16];			// Terminal Random
	BYTE	CardRandom[16];				// Card Random
	BYTE	CipherPW[16];				// ��ȣȭ�� ���� ��й�ȣ
	BYTE	ENCData1[8];				// First ENCData(MyBi)
	BYTE	ENCData2[8];				// Second ENCData(MyBi)
	char	HostTranData[100];			// Host ���� ������ ����
} FinanceICInfo;

//  ���� IC ���� Infomation
struct tagFinanceICISO3Info {
	BYTE	AccountState;				// ���»���
	char	ISO3[54];					// ISO Track 3
	BYTE	AccountStart;				// ���½�����ġ
	BYTE	AccountNOCnt;				// �����ڸ���
	BYTE	Reserved[5];				// Reserved Area for Future Use
} FinanceICISO3Info[20];

struct tagFinanceICISO2Info {			// ����IC ISO2 Track DATA
	BYTE	ISO2[37];					// ī������
} FinanceICISO2Info;

//  ���� IC ���� Infomation for Screen Display
struct tagFinanceICDisp {
	char	Num[3];						// ���� Index
	char	AccNum[17];					// ���¹�ȣ
	char	AccState[13];				// ���»���
} FinanceICDisp[10];

	// �������� ���� ����
	int		FICEPBAccountCount;			// �������� ���� ����
	int		FICEPBAccountTotalCount;	// �������� �Ѱ��� ����

// e-������� ��������STRUCTURE(�䱸��,����,���Ű���)
typedef struct tagEPBACCOUNTINFO {				
	BYTE	AccountState;				// ���»���
	BYTE	EPBAccountKind;				// ��������, 1:�䱸�� , 2:���༺, 3:����
	BYTE	EPBAccountNum[8];			// ���¹�ȣ(8)
	char	ISO3[54];					// ISO Track 3
} FINANCEICEPBINFO;

	FINANCEICEPBINFO EPBAccountInfo[20];

//  �������� ���� Infomation for Screen Display
typedef struct tagFinanceICEPBDisp {
	int		OrgIndex;					// Original Index
	BYTE	EPBAccountKind;				// ��������, 1:�䱸�� , 2:���༺, 3:����
	int		AccountCnt;					// �Ѱ��°���
	char	Num[3];						// ���� Index
	BYTE	AccNum[17];					// ���¹�ȣ
	char	AccState[13];				// ���»���
} FINANCEICEPBDISP;

	FINANCEICEPBDISP FinanceICEPBDisp[20];
	FINANCEICEPBDISP FinanceICEPBDisp2[20];

////////////////////////////////////////////////////////////////////////////////
//  EMV Use Structure
////////////////////////////////////////////////////////////////////////////////
//  EMV Infomation
struct tagEMVInfo {
	BYTE	ConfigCode;					// ���� Configuration Code
	BYTE	CAPKRIDIndex;				// EMV CA List RID Index ����
	BYTE	CAPKKeyIndex;				// EMV CA List Key Set�� Index ����
	BYTE	AIDListCount;				// �ܸ��� ���� ���� AID ��� ����
	BYTE	CandidateCount;				// EMV �ĺ� ��� ����
	BYTE	TranIndex;					// �ĺ� ��� �� �ŷ��� ���õ� Application�� Index
	BYTE	EncryptPIN[8];				// ��ȣȭ�� PIN ����
	char	CertificationType[20];		// ������ ����(AAC, TC, ARQC, AAR)
	char	GetDOLData[50];				// EMV ���� Data Get�� ���
} EMVInfo;

//  EMV Data Object List Infomation
struct tagEMVDOLListInfo {
	BYTE	AppIDLen;					// Application Identifier Length
	BYTE	AppID[16];					// Application Identifier
	BYTE	MSTrack2Len;				// MS Track 2 Image Length
	BYTE	MSTrack2Image[20];			// MS Track 2 Image
	BYTE	PANLen;						// Primary Account Number Length
	BYTE	PAN[10];					// Primary Account Number
	BYTE	TranCurrencyCode[2];		// Transaction Currency Code
	BYTE	CSN;						// PAN Sequence Number
	BYTE	DDOLLen;					// Dynamic Data Authentication Data Object List Length
	BYTE	DDOL[9];					// Dynamic Data Authentication Data Object List
	BYTE	TranAmountHexa[4];			// Transaction Amount(Hexadecimal)
	BYTE	AppInterchangeProfile[2];	// Application Interchange Profile
	BYTE	AuthResponseCode[2];		// Authorization Response Code
	BYTE	CVMLLen;					// Cardholder Verification Method List Length
	BYTE	CVML[252];					// Cardholder Verification Method List
	BYTE	TerminalVerifyResult[5];	// Terminal Verification Result
	BYTE	TransactionDate[3];			// Transaction Date
	BYTE	TerminalStatusInfo[2];		// Terminal Status Infomation
	BYTE	TransactionType;			// Transaction Type
	BYTE	TranAmountBCD[6];			// Transaction Amount(BCD)
	BYTE	AnotherTranAmountBCD[6];	// Another Transaction Amount(BCD)
	BYTE	AnotherTranAmountHexa[4];	// Another Transaction Amount(Hexadecimal)
	BYTE	AppVersionNum[2];			// Application Version Number
	BYTE	IADLen;						// Issuer Application Data Length
	BYTE	IssuerAppData[64];			// Issuer Application Data
	BYTE	TerminalCountryCode[2];		// Terminal Country Code
	BYTE	TranFloorLimit[4];			// Transaction Floor Limit
	BYTE	TransactionTime[3];			// Transaction Time
	BYTE	AppCryptogram[8];			// Application Cryptogram
	BYTE	CryptogramInfoData;			// Cryptogram Information Data
	BYTE	TerminalCapabilitise[3];	// Terminal Capabilitise
	BYTE	CVMR[3];					// Cardholder Verification Method Result
	BYTE	TerminalType;				// Terminal Type
	BYTE	AppTranCounter[2];			// Application Transaction Counter
	BYTE	UnpredictableNumber[4];		// Unpredictable Number
	BYTE	AddTerminalCapabilitise[5];	// Additional Terminal Capabilitise
	BYTE	TranSequenceCounter[4];		// Transaction Sequence Counter
	BYTE	IssuerScriptResultLen;		// Issuer Script Result Length
	BYTE	IssuerScriptResult[50];		// Issuer Script Result

	BYTE	TACDenial[5];				// Terminal Action Code - Denial
	BYTE	TACOnline[5];				// Terminal Action Code - Online
	BYTE	TACDefault[5];				// Terminal Action Code - Default
	BYTE	TDOLLen;					// Transaction Certificate Data Object List Length
	BYTE	TDOL[8];					// Transaction Certificate Data Object List
	char	PaySysEnviroment[20];		// Payment System Enviroment "1PAY.SYS.DDF01"
} EMVDOLListInfo;

//  EMV CAPK List Infomation(CA ����Ű List)
struct tagEMVCAPKListInfo {
	BYTE	Offset;						// Offset
	BYTE	RID[5];						// RID of CA
	BYTE	Index;						// Index Depend on RID
	BYTE	ExponentLen;				// Length of Exponent
	BYTE	Exponent[3];				// Exponent(Max. 3)
	BYTE	ModulusLen;					// Length of Modulus
	BYTE	Modulus[248];				// Modulus(Max. 248)
} EMVCAPKListInfo[7];

//  EMV ���� ���� AID List Infomation(�ܸ��� ���� ����)
struct tagEMVAIDListInfo {
	BYTE	AIDLen;						// Length of AID
	BYTE	AID[18];					// AID
	char	Brand[40];					// Brand Name
} EMVAIDListInfo[15];				

//  EMV Candidate List Infomation(ī�� �� �ܸ��� ��ȣ�������� Display��)
struct tagEMVCandidateList {
	char	Index[3];					// Index(Ex : " 1")
	BYTE	API;						// Application Priority Indicator
	BYTE	AIDLen;						// Length of AID
	BYTE	AID[16];					// AID
	char	Brand[40];					// Brand Name
} EMVCandidateList[9];

////////////////////////////////////////////////////////////////////////////////
//  ���ܼ�(��â/SKT) ����Ÿ 
////////////////////////////////////////////////////////////////////////////////
struct tagIRDAInfo1						// CD/ATM ���� : type I
{
	BYTE		IRBankCode[4];			// �����ڵ�								ASCII
	WORD		IRChipLenInfo;			// Chip ���� ����						HEX
	BYTE		IRAccount[16];			// ��������								ASCII
	BYTE		IRCSN[16];				// ī�� �Ϸù�ȣ(Card Serial Number)	ASCII
	BYTE		IRSubjectCode[2];		// �����ڵ�								ASCII
	BYTE		IRTagInfo;				// Tag ����								HEX
	BYTE		IRSomeInfo[8];			// �����ڸ���							ASCII
} IRDAInfo1;

struct tagIRDAInfo {					// CD/ATM ���� : type II
	BYTE	ISODataTag;					// ISO Data Tag
	BYTE	IrdaType2Mode;				// Harex���� : Type2(M-Bank) ���۸��
	BYTE	FCI[40];					// FCI����
	BYTE	CAN[8];						// Card Application Number
	BYTE	CardHolderNameLen;			// ī�� ������ �̸� ����
	char	CardHolderName[27];			// ī�� ������ �̸�
	BYTE	RealNameNOLen;				// �Ǹ��ȣ ����
	BYTE	RealNameType;				// �Ǹ���
	char	RealNameNO[15];				// �Ǹ��ȣ
	BYTE	ExpireDayLen;				// ������ ����
	char	ExpireDay[10];				// ������
	BYTE	AlgorithmCode;				// �˰��� �ĺ��ڵ�
	BYTE	IssureCodeLen;				// ������ �ĺ��ڵ� ����
	char	IssureCode[6];				// ������ �ĺ��ڵ�
	BYTE	CSN[8];						// ī�� �Ϸù�ȣ(Card Serial Number)
	char	szCSN[20];					// ī�� �Ϸù�ȣ for Screen
	BYTE	TerminalRandom[16];			// Terminal Random
	BYTE	CardRandom[16];				// Card Random
	BYTE	CipherPW[16];				// ��ȣȭ�� ���� ��й�ȣ
} IRDAInfo;

struct tagIRDAInfo2						// IRDA Infomation
{
	BYTE	ISODataTag;					// ISO Data Tag
	BYTE	TrInfo;						// 2Track, 3Track, Mobile Identification Number(MIN)���翩��
	WORD	IRISO2size;	 				// 2Track Data Size
	MCISO2	IRISO2;						// 2Track Data
	BYTE	IRBankCode[4];				// �����ڵ�								ASCII
	WORD	IRChipLenInfo;				// Chip ���� ����						HEX
	BYTE	IRAccount[16];				// ��������								ASCII
	BYTE	IRCSN[16];					// ī�� �Ϸù�ȣ(Card Serial Number)	ASCII
	BYTE	IRSubjectCode[2];			// �����ڵ�								ASCII
	BYTE	IRTagInfo;					// Tag ����								HEX
	BYTE	IRSomeInfo[8];				// �����ڸ���							ASCII
} IRDAInfo2;

struct tagIRDAISO3Info {				//  IRDA ���� Infomation
	BYTE	AccountState;				// ���»���
	char	ISO3[54];					// ISO Track 3
	BYTE	AccountStart;				// ���½�����ġ
	BYTE	AccountNOCnt;				// �����ڸ���
	BYTE	Reserved[5];				// Reserved Area for Future Use
} IRDAISO3Info;

struct tagIRDAISO2Info {				// IRDA ISO2 Track DATA
	BYTE	ISO2[37];					// ī������
} IRDAISO2Info;

struct tagDgbIrdData
{
	BYTE	IRServiceID[1];
	BYTE	IRFCI[84];
	BYTE	IRBasicInfo[142];
	BYTE	IRAccountInfo[128];
	BYTE	IRCSN[16];
	BYTE	IRCRN[32];
	BYTE	IRPassWord[32];
	BYTE	IRHRN[32];
	BYTE	IRKeyVersion[2];
} DgbIrdData;

struct tagAtmTranNetInfo				// ��ⰳ���� ���� ����		2012.02.28
{
	int		bUseEachNet;				// ��ⰳ���� ��뿩��Flag
	CString	strCheckStartTime;			// ��ⰳ���� �ð�Ȯ�� ���۽ð�
	CString	strCheckEndTime;			// ��ⰳ���� �ð�Ȯ�� ����ð�
	int		bReadyEachNet;				// Ȯ�νð��� ����ۿ���Flag
} AtmTranNetInfo;


struct tagAtmRestartInfo				// �������� ����		2005.02.02
{
	int		bUseRestart;				// �������� ��뿩��Flag
	CString	strCheckStartTime;			// ����� �ð�Ȯ�� ���۽ð�
	CString	strCheckEndTime;			// ����� �ð�Ȯ�� ����ð�
	int		bReadyRestart;				// Ȯ�νð��� ����ۿ���Flag
} AtmRestartInfo;
	
	int		SplUpdateNow;				// SPL ���¸� ���������� Update���� Control
	int		m_DisplayJamInfo;			// Brm Jam Information Display
	int		ToggleDisp;					// ���ݸż�/ȸ���ż� �� SPLȭ�鿡 DISPLAY ��ۺ��� 
	bool	bIsBRMGetSensorOk;			// Get Sensor Action Result from BRM; false-> Sensor Read Error  //010003
	int		JamScreenNoSave[17];		// PJH 1231

////////////////////////////////////////////////////////////////////////////////
//  RFD ����Ÿ 
////////////////////////////////////////////////////////////////////////////////
	BYTE	RFSendData[BASEBUFFSIZE];	// RF�����ڷ�
	BYTE	RFRecvData[BASEBUFFSIZE];	// RF�����ڷ�
	int		RFDAnalError;				// RF�м�����
	BYTE	RFDStatus[8];				// RF����

struct tagRFDBasicInfo
{
	BYTE	CardNo[10];					// ī���ȣ
	BYTE	CardRID[10];				// ī��RID
	BYTE	CardState[2];				// ī�����
	BYTE	TranCount[10];				// �ŷ�ī��Ʈ
	BYTE	CardID[10];					// ī��ܸ�ID
	BYTE	CardRemain[8];				// ī���ܾ�
	BYTE	ValidDate[8];				// ī����ȿ�Ⱓ
	BYTE	LastLoadDate[8];			// ������������
	BYTE	CardType[2];				// ī������
	BYTE	MAC[4];						// MAC
	BYTE	TranNo[20];					// ����ó����ȣ
} RFDBasicInfo;

struct tagRFDCharge
{
	BYTE	CardNo[10];					// ī���ȣ
	BYTE	ChargeMoney[8];				// ����ݾ�
	BYTE	PreChargeMoney[8];			// �������ݾ�
	BYTE	CardRemain[8];				// �������ܾ�
	BYTE	CardRID[10];				// ī��RID
	BYTE	CardType[2];				// ī������
	BYTE	TranCount[10];				// �ŷ�ī��Ʈ
	BYTE	MacID[4];					// MAC
} RFDCharge;

struct tagRFDDrawBack
{
	BYTE	CardNo[10];					// ī���ȣ
	BYTE	PreBackMoney[8];			// ȯ�����ܾ�
	BYTE	PostBackMoney[8];			// ȯ�����ܾ�
	BYTE	CardRID[10];				// ī��RID
	BYTE	CardType[2];				// ī������
	BYTE	TranCount[10];				// �ŷ�ī��Ʈ
	BYTE	MacID[4];					// MAC
} RFDDrawBack;

//  �����ATM Start
//////////////////////////////////////////////////////////////////////
//	ȭ��Ȯ��
	int		SightHandiCapped;			// ���÷��� - ȭ��Ȯ�� ���ɿ���
	int		EarPhoneInserted;			// EarPhone ���Կ���

//TTS ��������
	char	*m_pSpeech;
	int		m_nSpeechNumBytes;
	char	m_pText[1000];
	void    *pTTS;
	int		m_playDoing;

	char	PdisableBtn[13];			// ���� �κл���
	int		BackDispCnt;

typedef	struct	tagBackDisp				// ȭ�麯������
{
	int		Index;						// ����ȭ���ȣ
	char	DispData[256];				// ��������
} BackDisp;
BackDisp	PbackDisp[30];

// �����ATM End
//	int		OSType;						//  Win7 ������ ���� OS��������

	BOOL	IsFallBackTran;				// Fallback �ŷ� ����				V09-33-00-#01 ICCM ������� 
	int		GenerateAC;					// AC ����							V09-33-00-#01 ICCM ������� 
	BOOL	Declined ;					// ERROR ������ �� ó���� DECLINE ó���߰�	V09-33-00-#01 ICCM ������� 

//////////////////////////////////////////////////////////////////////


	int		ProximitySensor;			// ��������
	DWORD	m_PSoundTime;				// �������� �ð�

	BOOL	PBBarLineReading;			// ���� ���ڵ� 
	BOOL	MchipFlag;		
	
	int nEmvStep;
	int nEmvStepCode;

	
// Hi-Pass������������ #N0266
struct tagRFHipassBalance {
	BYTE	Balance[8];					// �������� �ܾ�						ACSII (HEX)
	BYTE	CardNum[16];				// ī���ȣ								BCD
	BYTE	RespCode[4];				// ī�� �����ڵ�						ASCII (HEX)
} RFHipassBalance;

// Hi-Pass���˽��������� ����������������������������������������#N0266
// LLOG Data
struct tagHiPassLLog					// HiPass
{
	BYTE	PPCardTranType;				// �ŷ�����
	BYTE	PPCardTranSeqNum[4];		// �ŷ��Ϸù�ȣ
	BYTE	PPCardTranBalance[4];		// �ŷ��� �ܾ�
	BYTE	PPCardTranAmount[4];		// �ŷ��ݾ�
	BYTE	PPCardCompleteCode[2];		// �Ϸ��ڵ�
	BYTE	PPCardLoadOrg;				// �������
	BYTE	PPCardBranchNum[2];			// �����ҹ�ȣ
	BYTE	PPCardTerminalID;			// �ܸ���ID
	BYTE	PPCardLoadDate[4];			// ��������
	BYTE	PPCardLoadTime[3];			// �����ð�
	BYTE	EPMakerID[3];				// ������ID
	BYTE	LSAMID[5];					// LSAM ID
} HiPassLLog;
// ��������������

struct tagRFHipassLoad1 {
	BYTE	CardNum[16];				// ī���ȣ								BCD
	BYTE	AlgorithmID[2];				// �˰���ID							ASCII (HEX)
	BYTE	KeyVersion[2];				// Ű ����								ASCII (HEX)
	BYTE	CardTranSeq[8];				// ī��ŷ� �Ϸù�ȣ					ASCII (HEX)
	BYTE	RandomNum[16];				// ����									ASCII (HEX)
	BYTE	PublisherID[6];				// �����ID								ASCII (HEX)
	BYTE	CardID[10];					// ����ī��ID							ASCII (HEX)
	BYTE	CardBalance[8];			// ����ī�� �ܾ�					ASCII (HEX)
	BYTE	LedgerNum[20];				// �����ȣ  							ASCII (HEX)
	BYTE	UniqueNum[2];				// �����ĺ���ȣ							ASCII (HEX)
	BYTE	CardServiceID[6];			// ī�弭��ID		 					ASCII (HEX)
	BYTE	SIGN1[8];					// SIGN1								ASCII (HEX)
	BYTE	RespCode[4];				// ī�� �����ڵ�						ASCII (HEX)
	BYTE	CardBalanceHEX[8];				// HEX
} RFHipassLoad1;

struct tagRFHipassLoad2 {
	BYTE	CardNum[16];				// ī���ȣ								BCD
	BYTE	Balance[8];					// �������� �ܾ�						ACSII (HEX)
	BYTE	SIGN3[8];					// ����ī�忡�� ����� ���� (SIGN3)		ASCII (HEX)
	BYTE	RespCode[4];				// ī�� �����ڵ�						ASCII (HEX)
} RFHipassLoad2;

struct tagRFHipassUnload1 {
	BYTE	CardNum[16];				// ī���ȣ								BCD
	BYTE	PreLoadAmount[8];			// ���������ݾ�							ASCII (HEX)
	BYTE	TranType[2];				// �ŷ�����								ASCII (HEX)
	BYTE	AlgorithmID[2];				// �˰���ID							ASCII (HEX)
	BYTE	KeyVersion[2];				// Ű ����								ASCII (HEX)
	BYTE	CardTranSeq[8];				// ī��ŷ� �Ϸù�ȣ					ASCII (HEX)
	BYTE	RandomNum[16];				// ����									ASCII (HEX)
	BYTE	PublisherID[6];				// �����ID								ASCII (HEX)
	BYTE	CardID[10];					// ����ī��ID							ASCII (HEX)
	BYTE	CardBalance[8];				// ����ī�� �ܾ�						ASCII (HEX)
	BYTE	LedgerNum[20];				// �����ȣ  							ASCII (HEX)
	BYTE	UniqueNum[2];				// �����ĺ���ȣ							ASCII (HEX)
	BYTE	CardServiceID[6];			// ī�弭��ID		 					ASCII (HEX)
	BYTE	SIGN1[8];					// SIGN1								ASCII (HEX)
	BYTE	RespCode[4];				// ī�� �����ڵ�						ASCII (HEX)
} RFHipassUnload1;

struct tagRFHipassUnload2 {
	BYTE	SIGN3[8];					// ����ī�忡�� ����� ���� (SIGN3)		ASCII (HEX)
	BYTE	RespCode[4];				// ī�� �����ڵ�						ASCII (HEX)
} RFHipassUnload2;

char RFVersion[16];
char RFHipassErrorCode[5];
// ����������������������������������������������������������������������������
/////////////////////////////////////////////////////////////////////////////
// Operations
/////////////////////////////////////////////////////////////////////////////
public:

/////////////////////////////////////////////////////////////////////////////
//	Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
	CDevCmn();
	virtual ~CDevCmn();

/////////////////////////////////////////////////////////////////////////////
//	�Լ���
/////////////////////////////////////////////////////////////////////////////
//	return ��Ģ1:��������=TRUE, ��ֶǴ´ٸ�����=FALSE(DEVICE�� ����)
//			   2:��� RETURN(int)�� TRUE, FALSE�� �Ѵ�
//			   3:TRUE!=0, FALSE=0(�����ϱ�� ����)
//			   4:CString�߰�									2003.08.23
/////////////////////////////////////////////////////////////////////////////
	void	SetOwner(CWinAtmCtrl* pOwner, CNHMWI* pMwi, CNHAlert4SPackX* pNHAlertSpack, CNHICCM* pIccm, CNHMwiEmv* pMwiEmv);			
													// �����ͼ������ʱ�ȭ

	void ENCRYPT(unsigned char *encodedtext, const unsigned char *plaintext, int textlen, unsigned char *deskey);
	void DECRYPT(unsigned char *plaintext, const unsigned char *encodedtext, int textlen, unsigned char *deskey);
	void T_DES_ENCRYPTOR(BYTE* byData, USHORT usDataLen, BYTE* byKey1, BYTE* byKey2, BYTE* byKey3, BYTE* byResult);
	void T_DES_DECRYPTOR(BYTE* byData, USHORT usDataLen, BYTE* byKey1, BYTE* byKey2, BYTE* byKey3, BYTE* byResult);
/////////////////////////////////////////////////////////////////////////////
//	���κ��Լ�(JPR)
/////////////////////////////////////////////////////////////////////////////
// JPR SET MODE
	int		fnJPR_ClearErrorCode();					// ����ڵ��ʱ�ȭ

// JPR GET MODE
	CString	fstrJPR_GetErrorCode();					// ����ڵ屸�ϱ�
	CString	fstrJPR_GetSensorInfo();				// ��ġ�������ϱ�
	int		fnJPR_GetDeviceStatus();				// ��ġ���±��ϱ�

	int		fnJPR_GetPaperStatus();					// �������±��ϱ�
	int		fnJPR_GetMaterialInfo();				// ��ü�ܷ��������ϱ�

// JPR OPERATION MODE
	int		fnJPR_Initialize();						// ������۹��ʱ�ȭ
	int		fnJPR_Deinitialize();					// ��������

	int		fnJPR_EmbossPrint();					// ����������
	int		fnJPR_Print(LPCTSTR szPrintData);		// ����(�����ڷ�)

/////////////////////////////////////////////////////////////////////////////
//	��ǥ���Լ�(SPR)
/////////////////////////////////////////////////////////////////////////////
// SPR SET MODE
	int		fnSPR_ClearErrorCode();					// ����ڵ��ʱ�ȭ
	int		fnSPR_ClearRetractCnt();				// ȸ���ż��ʱ�ȭ

// SPR GET MODE
	CString	fstrSPR_GetErrorCode();					// ����ڵ屸�ϱ�
	CString	fstrSPR_GetSensorInfo();				// ��ġ�������ϱ�
	int		fnSPR_GetDeviceStatus();				// ��ġ���±��ϱ�

	int		fnSPR_GetPaperStatus();					// �������±��ϱ�
	int		fnSPR_GetMaterialInfo();				// ��ü�ܷ��������ϱ�

	CString	fstrSPR_GetRetractCnt();				// ȸ���ż����ϱ�

// SPR OPERATION MODE
	int		fnSPR_Initialize();						// ������۹��ʱ�ȭ
	int		fnSPR_Deinitialize();					// ��������

	int		fnSPR_Print(LPCTSTR szPrintData);		// ����(�����ڷ�)
	int		fnSPR_Eject(int nWaitSec = K_45_WAIT);	// ����(������ð�)
	int		fnSPR_EjectBoth(int nWaitSec = K_45_WAIT);	// ī��/��ǥ ���ù���(������ð�) V07-05-01-#02
	int		fnSPR_WaitTaken();						// ������
	int		fnSPR_CancelWaitTaken();				// ���������
	int		fnSPR_Retract();						// ȸ��
	int		fnSPR_Autoload();						// ����ε�
	int		fnSPR_GetAutoloadCondition();			// ����ε�����Ȯ��
	int		fnSPR_GetEjectBothAvail();				// 
/////////////////////////////////////////////////////////////////////////////
//	ī����Լ�(MCU)
/////////////////////////////////////////////////////////////////////////////
// MCU SET MODE
	int		fnMCU_ClearErrorCode();					// ����ڵ��ʱ�ȭ
	int		fnMCU_ClearRetractCnt();				// ȸ���ż��ʱ�ȭ

// MCU GET MODE
	CString	fstrMCU_GetErrorCode();					// ����ڵ屸�ϱ�
	CString	fstrMCU_GetSensorInfo();				// ��ġ�������ϱ�
	int		fnMCU_GetDeviceStatus();				// ��ġ���±��ϱ�

	int		fnMCU_GetMaterialInfo(int nType = 0);				// ��ü�ܷ��������ϱ�

	CString	fstrMCU_GetRetractCnt();				// ȸ���ż����ϱ�
	CString	fstrMCU_GetCardData();					// ī���ڷᱸ�ϱ�
	CString	fstrMCU_GetICData();					// IC�ڷᱸ�ϱ�
	int		fnMCU_GetICVersion();					// IC Board Version ���ϱ� V070300 3.

// MCU OPERATION MODE
	int		fnMCU_Initialize();						// ������۹��ʱ�ȭ
	int		fnMCU_Deinitialize();					// ��������
	int		fnMCU_ICCMOpenConnection();				// ICCM �������
	int		fnMCU_ICCMInitialize();					// ICCM �ʱ�ȭ
	int		fnMCU_ICCMDeinitialize();				// ICCM ��������


	int		fnMCU_EntryEnable(int nWaitSec = K_WAIT);	
													// �����㰡(���Դ��ð�)
	int		fnMCU_EntryDisable();					// ���Ա���
	int		fnMCU_Read();							// �б�	
	int		fnMCU_Eject(int nWaitSec = K_45_WAIT);	// ����(������ð�)
	int		fnMCU_EjectBoth(int nWaitSec = K_45_WAIT);	// ī��/��ǥ ���ù���(������ð�)	V07-05-01-#02	
	int		fnMCU_WaitTaken();						// ������
	int		fnMCU_CancelWaitTaken();				// ���������
	int		fnMCU_Retract();						// ȸ��
	int		fnMCU_Write(LPCTSTR szTrackData);		// ���(ī���ڷ�3TR)

	int		fnMCU_ICChipInitialize();				// ICĨ�ʱ�ȭ:�����̵�
	int		fnMCU_ICEntryEnable(int nWaitSec = K_WAIT);	
													// IC�����㰡(���Դ��ð�)
	int		fnMCU_ICEntryDisable();					// IC���Ա���
	int		fnMCU_ICSendData(int nProtocolId, LPCTSTR szICSendData, int nWaitSec = K_WAIT);	
													// IC�ڷ�����(��Ź��, �۽��ڷ�, ���۴��ð�)
	int		fnMCU_ICCMSendData(int nProtocolId, short nCmdType, LPCTSTR szICSendData = "", int nWaitSec = MID_ICCMRCV_TIME);	

	/////////////////////////////////////////////////////////////////////////////
	//	EMV Library
	// V09-33-00-#01 ICCM �������
	/////////////////////////////////////////////////////////////////////////////
	long	fnEMVLib_SetKernelType(long nType);
	long	fnEMVLib_SystemInitial();
	long	fnEMVLib_Sel_BuildCandidateApp();
	CString	fnEMVLib_Sel_GetAppList();
	CString	fnEMVLib_Sel_GetAppListEx();
	long	fnEMVLib_Sel_FinalAppSelection(long iSelApp);
	long	fnEMVLib_InitAppProcess(LPCTSTR TrProcCode, LPCTSTR TrAmount, LPCTSTR TrDateTime, long TrSerialCnt);
	long	fnEMVLib_ReadAppData();
	long	fnEMVLib_OffDataAuth();
	long	fnEMVLib_ProcRestriction();
	long	fnEMVLib_CardholderVerify1(LPCTSTR EncPinData);
	long	fnEMVLib_TerminalRiskMgmt();
	long	fnEMVLib_ActionAnalysis(BSTR* CardRslt);
	long	fnEMVLib_OnlineApproval(int nOnlineMode, int nHostResponse, LPCTSTR ResponseCd, long ARPCLen, LPCTSTR ARPCData, long IsuScriptLen, LPCTSTR IsuScriptData);
	long	fnEMVLib_CompleteProcess();
	
	CString	fnEMVLib_GetVal(long Val_Id);
	long	fnEMVLib_TlvStoreVal(long nTid, LPCTSTR strVal);
	
	CString fnEMVLib_MakeEMVTagData(LPCTSTR sTagType);
	void	fnEMVLib_Initial();
	long	fnEMVLib_Sel_SetOnlyOneAID(CString strYN);



/////////////////////////////////////////////////////////////////////////////
//	��ǥ����ݺ��Լ�(UCM)
/////////////////////////////////////////////////////////////////////////////
// UCM SET MODE
	int		fnUCM_ClearErrorCode();					// ����ڵ��ʱ�ȭ

// UCM GET MODE
	CString	fstrUCM_GetErrorCode();					// ����ڵ屸�ϱ�
	CString	fstrUCM_GetSensorInfo();				// ��ġ�������ϱ�
	int		fnUCM_GetDeviceStatus();				// ��ġ���±��ϱ�

	int		fnUCM_GetPosition();					// ����ġ���ϱ�
	int		fnUCM_GetDepCSTStatus();				// �Ա�ī��Ʈ���±��ϱ�
	int		fnUCM_GetWithCSTStatus();				// ���ī��Ʈ���±��ϱ�
	int		fnUCM_GetMaterialInfo(int nDepWithFlag);// ��ü�ܷ��������ϱ�(����ݱ���)
	int		fnUCM_GetAvailDeposit();				// �Աݰ��ɻ��±��ϱ�
	int		fnUCM_GetAvailWithdraw();				// ��ݰ��ɻ��±��ϱ�

	CString	fstrUCM_GetACheckData();				// �����ǥ�ڷᱸ�ϱ�
	CString	fstrUCM_GetCheckDataDispensed(int nIndex = -1);				
													// ��ݰ���Ѽ�ǥ�ڷᱸ�ϱ�(�ε���)

// UCM OPERATION MODE
	int		fnUCM_Initialize();						// ������۹��ʱ�ȭ
	int		fnUCM_Deinitialize();					// ��������

	int		fnUCM_OpenShutter();					// ����
	int		fnUCM_CloseShutter();					// �ݱ�

	int		fnUCM_Accept(int nWaitSec = K_WAIT);	// �����㰡(���Դ��ð�)�׿���
	int		fnUCM_CancelAccept();					// ���Ա����״ݱ�
	int		fnUCM_Read();							// �б�
	int		fnUCM_Escrow(LPCTSTR szPrintData);		// �ӽú���(�Աݹ輭�ڷ�)
	int		fnUCM_Stack();							// ����
	int		fnUCM_RejectA(int nWaitSec = K_45_WAIT);// �����ǥ��ȯ(������ð�)
	int		fnUCM_RejectAll(int nWaitSec = K_45_WAIT);			
													// ��ü��ȯ(������ð�)
	int		fnUCM_WaitTaken();						// ������
	int		fnUCM_CancelWaitTaken();				// ���������
	int		fnUCM_Retract(int nDepWithFlag);		// ȸ��(����ݱ���)

	int		fnUCM_Dispense(int nCheckCnt, LPCTSTR szPrintData);
													// ��ݰ��(��ǥ�ż�, ��ݹ輭�ڷ�)
	int		fnUCM_Present(int nWaitSec = K_45_WAIT);// ����(������ð�)

/////////////////////////////////////////////////////////////////////////////
//	��������ݺ��Լ�(BRM)
/////////////////////////////////////////////////////////////////////////////
// BRM SET MODE
	int		fnBRM_ClearErrorCode();					// ����ڵ��ʱ�ȭ
	int		fnBRM_SetCarrySpeed(BOOL bSpeed);		// BRM Speed ����		// T2ATM 2006.5.24 yscho
	int		fnBRM_SetNoteInfo(short nNoteKind1, short nNoteKind2, short nNoteKind3, short nNoteKind4);

// BRM GET MODE
	CString	fstrBRM_GetErrorCode();					// ����ڵ屸�ϱ�
	CString	fstrBRM_GetSensorInfo();				// ��ġ�������ϱ�
	int		fnBRM_GetDeviceStatus();				// ��ġ���±��ϱ�

	int		fnBRM_GetPosition();					// ����ġ���ϱ�
	int		fnBRM_GetChangeUnitLock();				// Ż���������ϱ�
	int		fnBRM_GetClerkCSTStatus();				// ���ī��Ʈ���±��ϱ�
	int		fnBRM_GetRejectCSTStatus();				// ȸ��ī��Ʈ���±��ϱ�
	int		fnBRM_GetRecycleBoxStatus();			// ����ī��Ʈ���±��ϱ�
	int		fnBRM_GetRecycleBox50000Status();		// ����ī��Ʈ���±��ϱ�(5��)	// V06-01-01-#19
	int		fnBRM_GetNumberOfCash();				// ���ݸż����ϱ�
	int		fnBRM_GetNumberOf50000Cash();			// ���ݸż����ϱ�(5������)		
	int		fnBRM_GetClerkRJBStatus();				// ����Reject���±��ϱ�			// V06-01-01-#18 
	int		fnBRM_GetNumberOfCash(int nCSTNo);		// �ش� ī��Ʈ ���ݸż� ���ϱ�	
	int		fnBRM_GetAutoloadCondition();			// �ڵ�������ڵ�ȸ���������ϱ�
	int		fnBRM_GetMaterialInfo();				// ��ü�ܷ��������ϱ�
	int		fnBRM_GetAvailDeposit();				// �Աݰ��ɻ��±��ϱ�
	int		fnBRM_GetAvailWithdraw();				// ��ݰ��ɻ��±��ϱ�

	int		fnBRM_GetAcceptCountStatus();			// ���Թ��Աݰ������
	CString	fstrBRM_GetCountResult();				// �Աݰ��������ϱ�
	CString	fstrBRM_GetRetractResult();				// ȸ��������ϱ�(�н�)
	CString	fstrBRM_GetRefillResult();				// ���������ϱ�
	CString	fstrBRM_GetTakeupResult();				// ȸ��������ϱ�
	CString	fstrBRM_GetBCDistinctionMode();			// ��/���� ��� ��ȸ	// T2ATM 2006.5.24 yscho
	// T2ATM ���� 2006.07.29 mwseo --------------------------------------------
	int		fnBRM_GetCSMNoteStatus();				// for T2ATM : ���� ������ ���� ���� Ȯ�� 2006.07.31
	int		fnBRM_Get10000NoteUsePriority();		// for T2ATM : ����/�Ÿ� �켱 ���� ��ȸ 2006.07.31
	int		fnBRM_GetEnableDepositNote();			// for T2ATM : �Ա� ���� ���� ��ȸ 2006.07.31
	int		fnBRM_GetNoteKindOfRB1();				// for T2ATM : ���� ��ȸ(RB1) 2006.07.31
	int		fnBRM_GetNoteKindOfRB2();				// for T2ATM : ���� ��ȸ(RB2) 2006.07.31
	int		fnBRM_GetNoteKindOfRB3();				// for T2ATM : ���� ��ȸ(RB3) 2006.07.31
	int		fnBRM_GetNoteKindOfRB4();				// for T2ATM : ���� ��ȸ(RB4) 2006.07.31
	//-------------------------------------------------------------------------
	
// BRM OPERATION MODE
	int		fnBRM_Initialize();						// ������۹��ʱ�ȭ
	int		fnBRM_Deinitialize();					// ��������

	int		fnBRM_Reset(BOOL bEnforce);				// ��ġ����				// T2ATM 2006.5.24 yscho

	int		fnBRM_OpenShutter();					// ����
	int		fnBRM_CloseShutter();					// �ݱ�

	int		fnBRM_Ready();							// ������غ������װ����뺹��

	int		fnBRM_ReadyCount();						// �Ա��غ�
	int		fnBRM_Accept(int nCashCnt);				// ����(�Աݰ������ݸż�)
	int		fnBRM_AcceptLarge(int nCashCnt, int nAmount);		
	int		fnBRM_Count();							// �Աݰ��
	int		fnBRM_Stack();							// ����
	int		fnBRM_Reject(int nWaitSec = K_45_WAIT);	// ��ȯ(������ð�)
	int		fnBRM_WaitLift();						// ���������۴��
	int		fnBRM_WaitTaken();						// ������
	int		fnBRM_CancelWaitTaken();				// ���������
	int		fnBRM_Retract();						// ȸ��
	int		fnBRM_Present2(int nWaitSec = K_45_WAIT);			
													// ����(������ð�):�ݱ����

	int		fnBRM_ReadyDispense();					// ����غ�
	int		fnBRM_Dispense(int nCashCnt);			// ��ݰ��(���ݸż�)
	int		fnBRM_DispenseLarge(int nCash100TCNT, int nCash50TCNT, int nCash10TCNT);	
	int		fnBRM_Present(int nWaitSec = K_45_WAIT);// ����(������ð�)

	int		fnBRM_RefillMoney(int nCashCnt);		// ����(���ݸż�)
	int		fnBRM_TakeupMoney(int nCashCnt);		// ȸ��(���ݸż�)
	int		fnBRM_GetLargeMoneySupport();			// ������ H/W �� F/W �������� 06-01-02-#08

	int		fnBRM_Set10000NoteUsePriority(short n10000NoteKind);
	int		fnBRM_EnableDepositNote(short nNoteKind, boolean bEnable);
	int		fnBRM_GetEnabledDepositNote();
	int		fnBRM_BrmGetLog();
	int		fnBRM_BrmCashClear(int nFlag = 0);

/////////////////////////////////////////////////////////////////////////////
//	CDU FUNCTION(CDU)
/////////////////////////////////////////////////////////////////////////////
// CDU SET MODE
	int		fnCDU_ClearErrorCode();					// Clear Error Code
	int		fnCDU_ClearDispenseInfo();				// Clear Dispense Information
	int		fnCDU_SetValueOfCash(LPCTSTR szCashValueData);
	int		fnCDU_SetMinMaxCST(LPCTSTR szMinCntData, LPCTSTR szMaxCntData);
													// Set MinMax Of CST(Min Count Data, Max Count Data)
	int		fnCDU_SetNumberOfCash(LPCTSTR szCashCntData);
													// Set Number Of Cash(Cash Count Data)
// CDU GET MODE
	CString	fstrCDU_GetErrorCode();					// Get Error Code
	CString	fstrCDU_GetSensorInfo();				// Get Sensor Information
	int		fnCDU_GetDeviceStatus();				// Get Device Status

	int		fnCDU_GetPosition();					// Get Position
	int		fnCDU_GetRejectCSTStatus();				// Get Reject CST Status
	int		fnCDU_GetRecycleBoxStatus();			// Get Recycle Box Status
	int		fnCDU_GetMaterialInfo();				// Get Material Information
	int		fnCDU_GetAvailWithdraw();				// Get Avail Withdraw

	int		fnCDU_GetNumberOfCST();					// Get Number Of CST
	int		fnCDU_GetCSTStatus(int nCSTNo);			// Get CST Status(CST No)
	int		fnCDU_GetValueOfCash(int nCSTNo);		// Get Value Of Cash(CST No)
	int		fnCDU_GetNumberOfCash(int nCSTNo);		// Get Number Of Cash(CST No)
	int		fnCDU_GetNumberOfSetCash(int nCSTNo);	// Get Number Of Set Cash(CST No)
	CString	fstrCDU_GetDispenseOfCST(int nCashAmt);	// Get Dispense Of CST(Cash Amount)
	int		fnCDU_GetNumberOfDispense(int nCSTNo);	// Get Number Of Dispense(CST No)
	int		fnCDU_GetNumberOfReject(int nCSTNo);	// Get Number Of Reject(CST No)

// CDU OPERATION MODE
	int		fnCDU_Initialize();						// Initialize
	int		fnCDU_Deinitialize();					// Deinitialize

	int		fnCDU_OpenShutter();					// Open Shutter
	int		fnCDU_CloseShutter();					// Close Shutter

	int		fnCDU_WaitTaken();						// Wait Taken
	int		fnCDU_CancelWaitTaken();				// Cancel Wait Taken
	int		fnCDU_Retract();						// Retract

	int		fnCDU_DispenseLarge(int nCash50TCNT, int nCash10TCNT, int nCheckCNT);	
	int		fnCDU_Dispense(int nCashAmt);			// Dispense(Cash Amount)
	int		fnCDU_Present(int nWaitSec = K_45_WAIT);// Present(Wait Time)
	int		fnCDU_DispenseCashCheckCount(int nCashCnt, int nCheckCnt);
													// Dispense Cash/Check

/////////////////////////////////////////////////////////////////////////////
//	CASH FUNCTION ROUTING(CASH)
/////////////////////////////////////////////////////////////////////////////
// CASH SET MODE
	int		fnCSH_ClearErrorCode();					// Clear Error Code
	int		fnCSH_ClearDispenseInfo();				// Clear Dispense Information
	int		fnCSH_SetValueOfCash(LPCTSTR szCashValueData);

	int		fnCSH_SetMinMaxCST(LPCTSTR szMinCntData, LPCTSTR szMaxCntData);
													// Set MinMax Of CST(Min Count Data, Max Count Data)
	int		fnCSH_SetNumberOfCash(LPCTSTR szCashCntData, LPCTSTR szTotalCashCntData);		
													// Set Number Of Cash(Cash Count Data, Total Cash Count Data)
	int		fnCSH_SetNumberOfDispenseCash(LPCTSTR szDispCntData);
													// Set Number Of Dispense Cash(Dispense Count Data)

// CASH GET MODE
	CString	fstrCSH_GetErrorCode();					// Get Error Code
	CString	fstrCSH_GetSensorInfo();				// Get Sensor Information
	int		fnCSH_GetDeviceStatus();				// Get Device Status

	int		fnCSH_GetPosition();					// Get Position
	int		fnCSH_GetChangeUnitLock();				// Get Change Unit Lock
	int		fnCSH_GetClerkCSTStatus();				// Get Clerk CST Status
	int		fnCSH_GetRejectCSTStatus();				// Get Reject CST Status
	int		fnCSH_GetRetractCSTStatus();			// Get Retract CST Status
	int		fnCSH_GetRecycleBoxStatus();			// Get Recycle Box Status
	int		fnCSH_GetRecycleBox50000Status();		// ����ī��Ʈ���±��ϱ�(5��)	// V06-01-01-#19
	int		fnCSH_GetNumberOfCash();				// Get Number Of Cash
	int		fnCSH_GetNumberOf50000Cash();			
	int		fnCSH_GetClerkRJBStatus();				// V06-01-01-#18
	int		fnCSH_GetAutoloadCondition();			// Get Autoload Condition
	int		fnCSH_GetMaterialInfo();				// Get Material Information
	int		fnCSH_GetAvailDeposit();				// Get Avail Deposit
	int		fnCSH_GetAvailWithdraw();				// Get Avail Withdraw

	int		fnCSH_GetAcceptCountStatus();			// Get Accept Count Status
	CString	fstrCSH_GetCountResult();				// Get Count Result
	CString	fstrCSH_GetRetractResult();				// Get Retract Result
	CString	fstrCSH_GetRefillResult();				// Get Refill Result
	CString	fstrCSH_GetTakeupResult();				// Get Takeup Result

	int		fnCSH_GetNumberOfCST();					// Get Number Of CST
	int		fnCSH_GetAmountOfCST();					// Get Amount Of CST
	int		fnCSH_GetCSTStatus(int nCSTNo);			// Get CST Status(CST No)
	int		fnCSH_GetRBStatus(int nCSTNo);			// Get RB Status(CST No)
	int		fnCSH_GetValueOfCash(int nCSTNo);		// Get Value Of Cash(CST No)
	int		fnCSH_GetNumberOfCash(int nCSTNo);		// Get Number Of Cash(CST No)
	int		fnCSH_GetAmountOfCash(int nCSTNo);		// Get Amount Of Cash(CST No)
	int		fnCSH_GetNumberOfSetCash(int nCSTNo);	// Get Number Of Set Cash(CST No)
	CString	fstrCSH_GetDispenseOfCST(int nCashAmt);	// Get Dispense Of CST(Cash Amount)
	int		fnCSH_GetNumberOfDispense(int nCSTNo);	// Get Number Of Dispense(CST No)
	int		fnCSH_GetNumberOfReject(int nCSTNo);	// Get Number Of Reject(CST No)

	int		fnCSH_GetNumberOfTotalSetCash(int nCSTNo);	
													// Get Number Of Total Set Cash(CST No)
	int		fnCSH_GetNumberOfDispenseCash(int nCSTNo);	
													// Get Number Of Dispense Cash(CST No)
		
// CASH OPERATION MODE
	int		fnCSH_Initialize();						// Initialize
	int		fnCSH_Deinitialize();					// Deinitialize

	int		fnCSH_OpenShutter();					// Open Shutter
	int		fnCSH_CloseShutter();					// Close Shutter

	int		fnCSH_Ready();							// Ready

	int		fnCSH_ReadyCount();						// Ready Count
	int		fnCSH_Accept(int nCashCnt);				// Accept(Avail Deposit Count)
	int		fnCSH_AcceptLarge(int nCashCnt, int nAmount);
													// Accept(Avail Deposit Count, Deposit Amount)	V07-00-00-#01 �������� ����
	int		fnCSH_Count();							// Count
	int		fnCSH_Stack();							// Stack
	int		fnCSH_Reject(int nWaitSec = K_45_WAIT);	
													// Reject(Wait Time)
	int		fnCSH_WaitLift();						// Wait Lift
	int		fnCSH_WaitTaken();						// Wait Taken
	int		fnCSH_CancelWaitTaken();				// Cancel Wait Taken
	int		fnCSH_Retract();						// Retract
	int		fnCSH_Present2(int nWaitSec = K_45_WAIT);			
													// Present2(Wait Time):Not Close

	int		fnCSH_ReadyDispense();					// Ready Dispense
	int		fnCSH_Dispense(int nCashAmt);			// Dispense(Cash Amount)
	int		fnCSH_DispenseLarge(int nCash100TCNT, int nCash50TCNT, int nCash10TCNT, int nCheckCNT = 0);	
													// Dispsense(100T Count, 50T Count, 10T Count)
	int		fnCSH_Present(int nWaitSec = K_45_WAIT);
													// Present(Wait Time)
	int		fnCSH_DispenseCashCheckCount(int nCashCnt, int nCheckCnt);
													// Dispense(CashCount, CheckCount)

	int		fnCSH_RefillMoney(int nCashCnt);		// Refill Money(Cash Count)
	int		fnCSH_TakeupMoney(int nCashCnt);		// Takeup Money(Cash Count)
	
/////////////////////////////////////////////////////////////////////////////
//	������Լ�(PBM)
/////////////////////////////////////////////////////////////////////////////
// PBM SET MODE
	int		fnPBM_ClearErrorCode();					// ����ڵ��ʱ�ȭ
	int		fnPBM_ClearRetractCnt();				// ȸ���ż��ʱ�ȭ

// PBM GET MODE
	CString	fstrPBM_GetErrorCode();					// ����ڵ屸�ϱ�
	CString	fstrPBM_GetSensorInfo();				// ��ġ�������ϱ�
	int		fnPBM_GetDeviceStatus();				// ��ġ���±��ϱ�

	int		fnPBM_GetMaterialInfo();				// ��ü�ܷ��������ϱ�
	
	CString	fstrPBM_GetRetractCnt();				// ȸ���ż����ϱ�
	CString	fstrPBM_GetMsData();					// ����Ms�ڷᱸ�ϱ�
	CString	fstrPBM_GetBarData();					// ����Bar�ڷᱸ�ϱ�
	CString	fstrPBM_GetLineData();					// ����Line�ڷᱸ�ϱ�

// PBM OPERATION MODE
	int		fnPBM_Initialize();						// ������۹��ʱ�ȭ
	int		fnPBM_Deinitialize();					// ��������

	int		fnPBM_EntryEnable(int nWaitSec = K_WAIT);	
													// �����㰡(���Դ��ð�)
	int		fnPBM_EntryDisable();					// ���Ա���
	int		fnPBM_Read();							// ����Ms�б�	
	int		fnPBM_BarLineRead();					// ����BarLine�б�	
	int		fnPBM_Print(LPCTSTR szPrintData);		// ����(�����ڷ�)
	int		fnPBM_Eject(int nWaitSec = K_45_WAIT);	// ����(������ð�)
	int		fnPBM_WaitTaken();						// ������
	int		fnPBM_CancelWaitTaken();				// ���������
	int		fnPBM_Retract();						// ȸ��
	int		fnPBM_MSWrite(LPCTSTR szMSData);		// MS Write
	CString	fnPBM_GetInkStatus();					// U8100 AP ���泻�� #07 - IPBM �Ҹ�ǰ �ܿ����� ����
/////////////////////////////////////////////////////////////////////////////
//	A4P ������ �Լ�
/////////////////////////////////////////////////////////////////////////////
// SPR SET MODE
	int		fnA4P_ClearErrorCode();					// ����ڵ��ʱ�ȭ
	int		fnA4P_ClearRetractCnt();				// ȸ���ż��ʱ�ȭ

// SPR GET MODE
	CString	fstrA4P_GetErrorCode();					// ����ڵ屸�ϱ�
	CString	fstrA4P_GetSensorInfo();				// ��ġ�������ϱ�
	int		fnA4P_GetDeviceStatus();				// ��ġ���±��ϱ�

	int		fnA4P_GetPaperStatus();					// �������±��ϱ�
	int		fnA4P_GetMaterialInfo();				// ��ü�ܷ��������ϱ�

	CString	fstrA4P_GetRetractCnt();				// ȸ���ż����ϱ�

// SPR OPERATION MODE
	int		fnA4P_Initialize();						// ������۹��ʱ�ȭ
	int		fnA4P_Deinitialize();					// ��������

	int		fnA4P_Print(LPCTSTR szPrintForm, LPCTSTR szPrintData);		
													// ����(�����ڷ�)
	int		fnA4P_PrintAndEject(LPCTSTR szPrintForm, LPCTSTR szPrintData, int nWaitSec = K_65_WAIT);		
													// ���ڿ͹���(�����ڷ�)	2007.06.13(�)
	int		fnA4P_Eject(int nWaitSec = K_45_WAIT);	// ����(������ð�)
	int		fnA4P_WaitTaken();						// ������
	int		fnA4P_CancelWaitTaken();				// ���������
	int		fnA4P_Retract();						// ȸ��

/////////////////////////////////////////////////////////////////////////////
//	CAMERA�Լ�(CMR)
/////////////////////////////////////////////////////////////////////////////
// CAMERA SET MODE
	int		fnCMR_ClearErrorCode();					// ����ڵ��ʱ�ȭ

// CAMERA GET MODE
	CString	fstrCMR_GetErrorCode();					// ����ڵ屸�ϱ�
	int		fnCMR_GetDeviceStatus();				// ��ġ���±��ϱ�
	int		fnCMR_GetFaceCameraDeviceStatus();		// �󱼺�ī�޶� ��ġ���±��ϱ� V03.01.01
	int		fnCMR_GetHandCameraDeviceStatus();		// ���뱸ī�޶� ��ġ���±��ϱ� V03.01.01

// T3ATM AP ���泻�� #19 - OM�� Room Camera�� ���¸� ��Ÿ���� ���� �ʿ���
	int		fnCMR_GetRoomCameraDeviceStatus();		

// CAMERA OPERATION MODE
	int		fnCMR_Initialize();						// ������۹��ʱ�ȭ
	int		fnCMR_Deinitialize();					// ��������

	int		fnCMR_CaptureFace(LPCTSTR szFileName);	// ���Կ�(�����̸�)
	int		fnCMR_CaptureHand(LPCTSTR szFileName);	// ���Կ�(�����̸�)

	
	int		fnCMR_CaptureRoom(LPCTSTR szFileName);	// U8100 AP ���泻�� #01 - 3Camera, Preview���� �ĸ�ī�޶��Կ�
	
	int		fnCMR_PreviewToAD(BOOL bShow);			// LCD�� ī�޶� ������ó��
	int		fnCMR_PreviewToFront(BOOL bShow);		// OPL�� ī�޶� ������ó��

	int		fnCMR_PreviewToOM(BOOL bShow);			// OM�� ī�޶� ������ó��


/////////////////////////////////////////////////////////////////////////////
//	DOOR�Լ�(DOR)
/////////////////////////////////////////////////////////////////////////////
// DOOR SET MODE
	int		fnDOR_ClearErrorCode();					// ����ڵ��ʱ�ȭ

// DOOR GET MODE
	CString	fstrDOR_GetErrorCode();					// ����ڵ屸�ϱ�
	int		fnDOR_GetDeviceStatus();				// ��ġ���±��ϱ�

	int		fnDOR_GetDoorStatus();					// �޹����±��ϱ�
	int		fnDOR_GetCabinetStatus();				// �޹����±��ϱ�(TTW:Cabinet Type)

// DOOR OPERATION MODE
	int		fnDOR_Initialize();						// ������۹��ʱ�ȭ
	int		fnDOR_Deinitialize();					// ��������

/////////////////////////////////////////////////////////////////////////////
//	LIGHT�Լ�(LGT)
/////////////////////////////////////////////////////////////////////////////
// LIGHT SET MODE
	int		fnLGT_ClearErrorCode();					// ����ڵ��ʱ�ȭ

	int		fnLGT_SetFlicker(int nIndex, int nValue);					
													// Flicker����(�ε���,��)
	int		fnLGT_SetIndicator(int nValue);			// Indicator����(��)

// LIGHT GET MODE
	CString	fstrLGT_GetErrorCode();					// ����ڵ屸�ϱ�
	int		fnLGT_GetDeviceStatus();				// ��ġ���±��ϱ�

// LIGHT OPERATION MODE
	int		fnLGT_Initialize();						// ������۹��ʱ�ȭ
	int		fnLGT_Deinitialize();					// ��������

/////////////////////////////////////////////////////////////////////////////
//	SPL�Լ�(SPL)
/////////////////////////////////////////////////////////////////////////////
// SPL SET MODE
	int		fnSPL_ClearErrorCode();					// ����ڵ��ʱ�ȭ

	int		fnSPL_SetSegment(LPCTSTR szValue);		// Segment����(��)
	int		fnSPL_SetLed(LPCTSTR szValue);			// Led����(��)
	int		fnSPL_SetLedOnOff(int nLedIndex, int bOnOff);
													// Led����(�ε���, ��) V03-01-01
	int		fnSPL_SetRpl(LPCTSTR szValue);			// Rpl����(��)
	int		fnSPL_SetInform(LPCTSTR szValue);		// Inform����(��)
	
// SPL GET MODE
	CString	fstrSPL_GetErrorCode();					// ����ڵ屸�ϱ�
	int		fnSPL_GetDeviceStatus();				// ��ġ���±��ϱ�

	int		fnSPL_GetTestKey();						// ��ü�׽�ƮŰ���ϱ�
	int		fnSPL_GetLoadSwitch();					// ���ֹ߷��ε彺��ġ���ϱ�
	int		fnSPL_GetCallKey();						// ȣ��Ű���ϱ�
	int		fnSPL_GetSplKeyValue();					// SCDP SPLŰ�����ϱ�		V03-01-01 �߰�

// SPL OPERATION MODE
	int		fnSPL_Initialize();						// ������۹��ʱ�ȭ
	int		fnSPL_Deinitialize();					// ��������

/////////////////////////////////////////////////////////////////////////////
//	UPS�Լ�(UPS)
/////////////////////////////////////////////////////////////////////////////
// UPS SET MODE
	int		fnUPS_ClearErrorCode();					// ����ڵ��ʱ�ȭ

// UPS GET MODE
	CString	fstrUPS_GetErrorCode();					// ����ڵ屸�ϱ�
	int		fnUPS_GetDeviceStatus();				// ��ġ���±��ϱ�

	int		fnUPS_GetPowerStatus();					// �������±��ϱ�

// UPS OPERATION MODE
	int		fnUPS_Initialize();						// ������۹��ʱ�ȭ
	int		fnUPS_Deinitialize();					// ��������

	int		fnUPS_PowerOff(int nWaitSec);			// ����Off(������ð�)
	int		fnUPS_PowerRestart(int nWaitSec, int nRestartSec);					
													// ����Restart(������ð�,���۴��ð�)

/////////////////////////////////////////////////////////////////////////////
//	DES�Լ�(DES)
/////////////////////////////////////////////////////////////////////////////
// DES SET MODE
	int		fnDES_ClearErrorCode();					// ����ڵ��ʱ�ȭ

// DES GET MODE
	CString	fstrDES_GetErrorCode();					// ����ڵ屸�ϱ�
	int		fnDES_GetDeviceStatus();				// ��ġ���±��ϱ�

// DES OPERATION MODE
	int		fnDES_Initialize();						// ������۹��ʱ�ȭ
	int		fnDES_Deinitialize();					// ��������

	int		fnDES_EncryptPin(void* vPinNum, int PinSize, void* vMemberNum, int MemberNumSize);
													// Des Pin Encryption : 2004.05.13 teo MemberNumSize�߰�

	int		fnDES_LoadKey(void* vKeyData, int Param);	// Des KEY Load
	int		fnDES_Test();							// Des Test
	int		fnDES_SW_EncryptPin(void* vPinNum, int PinSize, void* vMemberNum, int MemberNumSize, void* vWorkingKey);
/////////////////////////////////////////////////////////////////////////////
//	FINGER�Լ�(FNG)
/////////////////////////////////////////////////////////////////////////////
// FINGER SET MODE
	int		fnFNG_ClearErrorCode();					// ����ڵ��ʱ�ȭ

// FINGER GET MODE
	CString	fstrFNG_GetErrorCode();					// ����ڵ屸�ϱ�
	int		fnFNG_GetDeviceStatus(int nFlag = FALSE);				// ��ġ���±��ϱ�
	CString fstrFNG_GetData();						// �����ڷᱸ�ϱ�

// FINGER OPERATION MODE
	int		fnFNG_Initialize();						// ������۹��ʱ�ȭ
	int		fnFNG_Deinitialize();					// ��������
	int		fnFNG_Acquire();						// �����б�
	int		fnFNG_CancelAcquire();					// �����б����
	int		fnFNG_ShowData(LPCTSTR szFlag);			// ����ǥ���ϱ�(ǥ�ñ���)
	int		fnFNG_ShowSize(int nXPos, int nYPos, int nWideSize, int nHeightSize);
													// ����ǥ��â����

/////////////////////////////////////////////////////////////////////////////
//	IRDA�Լ�(IRD)
/////////////////////////////////////////////////////////////////////////////
// IRDA SET MODE
	int		fnIRD_ClearErrorCode();					// ����ڵ��ʱ�ȭ

// IRDA GET MODE
	CString	fstrIRD_GetErrorCode();					// ����ڵ屸�ϱ�
	int		fnIRD_GetDeviceStatus();				// ��ġ���±��ϱ�
	CString fstrIRD_GetData();						// ���ܼ��ڷᱸ�ϱ�
	int		fnIRD_AnalData(LPCTSTR szData);			// ���ܼ�����Ÿ�м�		2004.02.05			

// IRDA OPERATION MODE
	int		fnIRD_Initialize();						// ������۹��ʱ�ȭ
	int		fnIRD_Deinitialize();					// ��������
	int		fnIRD_EntryEnable();					// �����㰡
	int		fnIRD_EntryDisable();					// ���Ա���

/////////////////////////////////////////////////////////////////////////////
//	BAR�Լ�(BAR)
/////////////////////////////////////////////////////////////////////////////
// BAR SET MODE
	int		fnBAR_ClearErrorCode();					// ����ڵ��ʱ�ȭ

// BAR GET MODE
	CString	fstrBAR_GetErrorCode();					// ����ڵ屸�ϱ�
	int		fnBAR_GetDeviceStatus();				// ��ġ���±��ϱ�
	CString	fstrBAR_GetData();						// ���ڵ��ڷᱸ�ϱ�

// BAR OPERATION MODE
	int		fnBAR_Initialize();						// ������۹��ʱ�ȭ
	int		fnBAR_Deinitialize();					// ��������
	int		fnBAR_EntryEnable();					// �б��㰡
	int		fnBAR_EntryDisable();					// �б����

/////////////////////////////////////////////////////////////////////////////
//	COMMON�Լ�(CMN)
/////////////////////////////////////////////////////////////////////////////
// CMN SET MODE
// CMN GET MODE
// CMN OPERATION MODE
	CString	fstrCMN_CheckDeviceAction(int nDevId, int nCheckTime);	
													// ���۴�����ְ���(��ġId,üũ�ð�)
	int		fnCMN_ScanDeviceAction(int nDevId, int nScanTime, int nEventKind);	
													// �̺�Ʈ��������������ġ���ϱ�(��ġId,�˻��ð�,�̺�Ʈ����)
	int		fnCMN_GetErrorDevice(int nDevId);		// �����ġ���ϱ�(��ġId)
	int		fnCMN_GetTimeoutDevice(int nDevId);		// �ð��ʰ���ġ���ϱ�(��ġId)


	int		fnCMN_ProcBackupTrace(LPCTSTR szFileName, int nFileSize = 10000000);
													// Kal Trace Backup(�����̸�,���ϱ���)

/////////////////////////////////////////////////////////////////////////////
//	K-CASH�Լ�(KCS)
/////////////////////////////////////////////////////////////////////////////
// K-CASH SET MODE
// K-CASH GET MODE
// K-CASH OPERATION MODE
	int		fnKCS_PowerOn();						// ��������
	int		fnKCS_PowerOff();						// ��������
	int		fnKCS_ReadATR();						// ATR�б�:�ù�ī��,�����ī��

	int		fnKCS_CheckPin(LPCTSTR szPassWord);		// ��й�ȣ�˻�
	int		fnKCS_VerifyTranBit();					// �ŷ�����BitȮ��
	int		fnKCS_RemoveTranBit();					// �ŷ�����Bit����
	int		fnKCS_ReadTrafficInfo();				// ���������б�

	int		fnKCS_ReadPan(LPCTSTR szPassWord);		// ���������б�
	int		fnKCS_ReadBAmount();					// �ܾ������б�
	int		fnKCS_ReadPersonInfo(LPCTSTR szPassWord);	
													// ���������б�
	int		fnKCS_ReadPassBook1(LPCTSTR szPassWord);	
													// �ְ��������б�

	int		fnKCS_PrepareLoadIEP(LPCTSTR szSendData);	
													// ������ó��(�ŷ��ݾ�8+��ȣ16)
	int		fnKCS_LoadIEP(LPCTSTR szSendData);		// �����䱸(����16+����8)

	int		fnKCS_PrepareUnLoadIEP(LPCTSTR szSendData);	
													// ȯ����ó��(�ŷ��ݾ�8+��ȣ16)
	int		fnKCS_UnloadIEP(LPCTSTR szSendData);	// ȯ�ҿ䱸(����16+����8)

	int		fnKCS_PrepareUpdateIEP();				// �Ķ���Ͱ�����ó��
	int		fnKCS_UpdateIEP(LPCTSTR szSendData);	// �Ķ���Ͱ���(SamId8+Parameter4+Versin2+Value32+����16+����8)

	int		fnKCS_ChangePin(LPCTSTR szSendData);	// ��й�ȣ����(��������ȣ16+�����ľ�ȣ16)

	int		fnKCS_ReadCLog();						// ����ȯ�Ұŷ������б�
	int		fnKCS_ReadPLog1();						// ���Űŷ������б�(1-5)
	int		fnKCS_ReadPLog2();						// ���Űŷ������б�(6-10)

	int		fnKCS_PrepareUnLoadRcvIEP(LPCTSTR szPassWord);
													// ȯ�Һ�����ó��
	int		fnKCS_ReadEPID();						// ����ȭ���ȣ �о����(�����) 

	int		fnKCS_AnalRecvData();					// ���źм�

/////////////////////////////////////////////////////////////////////////////
//	IC ���� �Լ�(ICA). 20040127-HSW-A
/////////////////////////////////////////////////////////////////////////////
//  Operation Function
	void	fnICA_InitProcess();					// IC �ŷ� �ʱ�ȭ(Buffer Clear)
	int		fnICA_ConstructAPDU(LPBYTE lpSendData, BYTE ISOCase, BYTE CLA, BYTE INS,
				BYTE P1, BYTE P2, BYTE Lc = NULL, LPBYTE pData = NULL, BYTE Le = NULL);
													// IC Card ǥ�� APDU Message�� ����
	int		fnICA_ConstructTLV(LPBYTE lpDestData, LPBYTE lpTag, int TagLen,
				LPBYTE lpVaule, int ValueLen);		// Data ��ü�� ǥ�� TLV ���·� ����� ��(2����Ʈ Tag��)
	int		fnICA_ConstructTLV(LPBYTE lpDestData, BYTE bTag, LPBYTE lpVaule,
				int ValueLen);						// Data ��ü�� ǥ�� TLV ���·� ����� ��(1����Ʈ Tag��)
	int		fnICA_ISOIn(LPBYTE lpAPDU, int nAPDULen);
													// IC Card ǥ�� APDU ��� ����(���� ������ ����)
	int		fnICA_ISOOut(LPBYTE lpAPDU, int nAPDULen);
													// IC Card ǥ�� APDU ��� ����(���� ������ ����)
	int		fnICA_SelectApplet(int nICKind);		// IC Card ������ ���ؼ� Applet�� ����
	int		fnICA_SelectApplet(LPBYTE lpAID, int nAIDLen);
													// AID�� �̿��Ͽ� Applet ���� ����
	int		fnICA_ICConfirmType(BYTE BankType);		// AID�� �̿��Ͽ� IC ī�� ���� Application ������ ����
	int		fnICA_ISO7816WarningProc(LPBYTE lpData, int &nDataLen);
													// APDU ǥ�� ��۽� ó��. 61xx, 6Cxx.
	int		fnICA_CheckFCCode();					// FC Code ����
	int		fnICA_AnalyzeFCError();					// FC Error �м�
	int		fnICA_AnalyzeISOError(LPBYTE lpData, int &nDataLen);
													// ISO Error �м�
	LPBYTE	fnICA_GetFCError();						// �����ڵ� Error Get
	void	fnICA_SetFCError(LPBYTE lpErrData);		// �����ڵ� Error Set
	LPSTR	fnICA_GetErrCode();						// Error Code Get for Screen Display
	void	fnICA_SetErrCode(LPSTR strErrCode);		// Error Code Set for Screen Display
	LPSTR	fnICA_GetErrMsg();						// Error Message Get for Screen Display
	void	fnICA_SetErrMsg(LPSTR strErrMsg);		// Error Message Set for Screen Display

/////////////////////////////////////////////////////////////////////////////
//	���� IC �Լ�(ICA). 20040127-HSW-A
/////////////////////////////////////////////////////////////////////////////
//  Operation Function
	int		fnFIC_InitProcess();					// ���� IC �ŷ� �ʱ�ȭ(Buffer Clear)
	int		fnFIC_GetICType();						// IC CHIP TYPE ����
	int		fnFIC_ReadBasicInfo();					// ���� IC �⺻ ���� ��ȸ
	int		fnFIC_ReadAccountInfo(BYTE bIndex = 0x00);
													// ���� IC ���� ���� ��ȸ
	int		fnFIC_ReadDualInfo(BYTE bIndex = 0x00);	// ���� IC �⺻ ���� + ���� ���� ��ȸ
	int		fnFIC_GetEncipher(char* szPassWord, int PassWordSize);	
													// ���� IC Get Encipher
	int		fnFIC_GetENCData(BYTE bIndex, char* szPassWord);
													// ���� IC Get ENCData(MyBi)
	int		fnFIC_GetChallenge();					// ���� IC Get Challenge(MyBi)

	int		fnFIC_CheckPIN(LPCTSTR szPassWord);		// ���� IC PIN Verify
	int		fnFIC_ReadPan(LPCTSTR szPassWord);		// ���� IC ReadPAN			
	int		fnFIC_ReadBAmount();					// ���� IC ReadJAN			
	int		fnFIC_ChangePin(LPCTSTR szSendData);	// ���� IC ��й�ȣ����(��������ȣ16+�����ľ�ȣ16)
	int		fnFIC_OptionalChangePin(int nOpenCloseType, BYTE bChangeAppletType, BYTE bChangeKeyPinType, LPCTSTR szICSendData);

	int		fnFIC_ReadTrafficInfo();				// ���� IC ���������б�
	int		fnFIC_PrepareUnLoadRcvIEP(LPCTSTR szPassWord);
													// ���� IC ȯ�Һ�����ó��

	
	int		fnFIC_XORData(char* szPassWord, LPBYTE lpSource, LPBYTE lpResult);
													// ���� IC XOR Data for GetENCData(MyBi)
	int		fnFIC_AnalRecvData(int nCmdType);		// ���� IC Chip Receive Data �ؼ�
	int		fnFIC_MakeRandom(LPBYTE lpSource, int nLen, int Lv = 1);
													// ���� IC Random Data ���� �Լ�
	int		fnFIC_CheckIFMError(int nICRecvData);	// ���� IC RecvData Error Check(IFM Direct)			// V09-33-00-#01

//  Get/Set Function
	LPSTR	fnFIC_GetCardHolderName();				// ī�� ������ �̸� Get
	BYTE	fnFIC_GetRealNameType();				// �Ǹ��� Get
	LPSTR	fnFIC_GetRealNameNO();					// �Ǹ��ȣ Get
	LPSTR	fnFIC_GetExpireDay();					// ������ Get
	BYTE	fnFIC_GetAlgorithmCode();				// �˰��� �ĺ��ڵ� Get
	LPSTR	fnFIC_GetIssureCode();					// ������ �ĺ��ڵ� Get
	LPSTR	fnFIC_GetCSN();							// ī�� �Ϸù�ȣ Get
	int		fnFIC_GetAccountCnt();					// ���� ���� Get
	BYTE	fnFIC_GetAccountState(BYTE bIndex);		// �������� ���� �� ���»��� Get
	LPSTR	fnFIC_GetAccountNum(BYTE bIndex);		// �������� ���� �� ���¹�ȣ Get
	int		fnFIC_GetISO3(BYTE bIndex, LPSTR szISO3);
													// �ŷ��� ���õ� ������ ISO 3 Data Get
	LPSTR	fnFIC_GetBankCode();					// �ŷ��� ���õ� ���� ISO 3 Track ���� ���� �ڵ� Get
	int		fnFIC_SetFICDispInfo();					// ȭ�鿡 ������ �������� Format�� ����. ���� ��翡 ���� �ٸ�
	LPBYTE	fnFIC_GetTRandom();						// Terminal Random Get
	LPBYTE	fnFIC_GetCRandom();						// Card Random Get
	LPBYTE	fnFIC_GetCipherPW();					// ��ȣȭ�� ���� ��й�ȣ Get
	LPBYTE	fnFIC_GetENCData(BYTE bIndex);			// ENC Data Get(MyBi)
	LPSTR	fnFIC_GetHostTranData(int IsMyBi);		// Host ���� ��ȣȭ ������ Get

	int		fnFIC_EPBReadAccountInfo(BYTE bIndex = 0x00);
	int		fnFIC_GetEPBAccountCnt();				// ���� ���� Get
	int		fnFIC_SetEPBDispInfo();					// ȭ�鿡 ������ �������� Format�� ����. ���� ��翡 ���� �ٸ�
	int		fnFIC_GetEPBISO3(BYTE bIndex, LPSTR szISO3);
													// �ŷ��� ���õ� ������ ISO 3 Data Get

/////////////////////////////////////////////////////////////////////////////
//	EMV �Լ�(EMV)
/////////////////////////////////////////////////////////////////////////////
	//  Main Operation Function
	int		fnEMV_Initialize();						// EMV ���� Buffer�� �ʱ�ȭ(��� Init�� 1ȸ ȣ��)
	int		fnEMV_CAListSet(LPBYTE m_Data, WORD Len); // EMV CA Public Key�� �޾ƿ� Ŀ�η� ����(������)
	int		fnEMV_TerminalListSet(LPBYTE m_Data, BYTE Len);
													// EMV Application List�� �޾ƿ� EMVĿ�η� ������.
	int		fnEMV_TerminalConstSet(LPBYTE m_Data, BYTE Len); 
	int		fnEMV_SelectPSE();						// EMV �ŷ� ���� ���θ� PSE�� ���� Ȯ��
	int		fnEMV_ReqCandidateList();				// ī��κ��� EMV �ŷ��� ������ �ĺ� ����� ��û
	int		fnEMV_SelectApplication(BYTE Index);	// �ĺ� ��� �� ���õ� Application���� �ŷ��� ����
	int		fnEMV_ReadApplication();				// �ŷ��� ���� Application�� ������ ����
	int		fnEMV_OfflineDataAuthentication(char* szAmount, int AmountLen,
							char* szAnotherAmount, int AnotherAmountLen);
													// �������� ������ �䱸
	int		fnEMV_OnlineProcessing(LPSTR lpszHostData, int nHostResp, BOOL HostRecvFlag, int Certificate = 0);
													// �¶��� ���� ��û�� ���� ȣ��Ʈ ������ ī��� ����
	int		fnEMV_CompletionProcessing();			// ī��� �ŷ� ���� ��û
	int		fnEMV_AfterScriptProcessing();			// �ŷ��� �����ϱ� ���� ��ũ��Ʈ ó���� ��û
	int		fnEMV_GetDOLData(LPBYTE lpTag, int nTagLen);
													// AP�� �ŷ� �� �ʿ��� ������ ��ü�� EMV Kernel�� ��û
	int		fnEMV_ResultData();						// �ŷ� ���� �� ȭ������ ������ EMV �ŷ� ��� �������� ��û(������ & ������)
	int		fnEMV_AnalRecvData(int nCmdType);		// EMV Chip Receive Data�� �ؼ��ϰ� ���ۿ� ����

//  Sub Operation Function
	int		fnEMV_InitProcess();					// �ŷ��ø��� �ʱ�ȭ �Ǿ�� �� Buffer Clear
	int		fnEMV_DefaultDOLSet();					// Default Data Object List Set
	int		fnEMV_DefaultDOLSetINI();				// Default Data Object List Set from INI File
	int		fnEMV_DefaultAIDListSet();				// Default ���� AID List Set
	int		fnEMV_DefaultAIDListSetINI();			// Default ���� AID List Set from INI File
	
	int		fnEMV_MakeReqCandidate(LPBYTE lpList);	// fnEMV_ReqCandidateList()�� ����� ������ ��� ����
	int		fnEMV_MakeOfflineDataAuthen(LPBYTE lpList, char* szAmount, int AmountLen,
							char* szAnotherAmount, int AnotherAmountLen);
													// fnEMV_OfflineDataAuthentication()�� ����� ������ ��� ����
	int		fnEMV_MakeOnlineProcessing(LPBYTE lpEMVResp, LPSTR lpszHostData,
							int nHostResp, BOOL HostRecvFlag);
													// fnEMV_OnlineProcessing()�� ����� ������ ��� ����

//  Get/Set Function
	BYTE	fnEMV_GetConfigCode();					// Configuration Code Get
	void	fnEMV_SetConfigCode(BYTE ConfigCode);	// Configuration Code Set
	BYTE	fnEMV_GetAIDListCount();				// AID List Count Get
	BYTE	fnEMV_GetCandidateCount();				// Candidate List Count Get
	void	fnEMV_SetCandidateCount(BYTE CandidateCount);
													// Candidate List Count Set
	BYTE	fnEMV_GetTranIndex();					// ���õ� Application�� Index Get
	void	fnEMV_SetTranIndex(BYTE TranIndex);		// ���õ� Application�� Index Set
	LPSTR	fnEMV_GetCertificationType();			// ������ ����(AAC, TC, ARQC, AAR) Get
	void	fnEMV_SetCertificationType(char* szCertificationType);
													// ������ ����(AAC, TC, ARQC, AAR) Set
	int		fnEMV_GetBrandCount();					// Application ��� Get
	int		fnEMV_GetISO2(char* ISO2);				// MS Track 2 Image(Tag 37) Get
	BYTE	fnEMV_GetCID9F27();						// Cryptogram Information Data Get
	int		fnEMV_SetAppCryptogram();				// ������ ������ ����
	LPSTR	fnEMV_GetHostTranData(BYTE TranType, int nEmvType = FALSE);	// Host ���� ������ Get
	int		fnEMV_GetINIPath(LPSTR lpstrPath);		// ini ������ ���� ������Ʈ�� ��� �б�

/////////////////////////////////////////////////////////////////////////////
//	NETWORK�Լ�(NET)
/////////////////////////////////////////////////////////////////////////////
// NET SET MODE
	int		fnNET_ClearErrorCode();					// ����ڵ��ʱ�ȭ

// NET GET MODE
	CString	fstrNET_GetErrorCode();					// ����ڵ屸�ϱ�
	int		fnNET_GetDeviceStatus();				// ��ġ���±��ϱ�

// NET OPERATION MODE
	int		fnNET_Initialize(int nOpenSec);			// ������۹��ʱ�ȭ
	int		fnNET_Deinitialize(int nCloseSec);		// ��������

	int 	fnNET_SendData(LPCTSTR szSendData, int nSendSec);
													// �ڷ�����(�۽��ڷ�,�۽Žð�)
	int		fnNET_RecvData(CString& strRecvData, int nRecvSec);			
													// �ڷ����(�����ڷ�,���Žð�)
	CString	fnNET_GetIpAddress(int nFlag = FALSE);
	CString	fnNET_GetMacAddress(int nFlag = FALSE);
	int		fnNET_ICMP_DeviceStatus(int nSyncFlag = FALSE);
	CString	fnNET_GetIpAddressFromWeb();
	int		fnNET_RequestUrl(CHttpConnection * httpConnection, CString strUrl, CString strPostData, int nSSLFlag, CString &strData);
	int		fn_fnNET_Polling();
/////////////////////////////////////////////////////////////////////////////
//	VFD�Լ�											// T2ATM 2006.5.24 yscho
/////////////////////////////////////////////////////////////////////////////
// VFD SET MODE
	int		fnVFD_ClearErrorCode();
	int		fnVFD_SetBrightnessLevel(int nLevel);	// �������
	int		fnVFD_SetDisplayAtPowerOn(int nSMemIndex, int nEMemIndex, int nScrolled, int nInterval);

// VFD GET MODE
	CString fstrVFD_GetErrorCode();
	int		fnVFD_GetDeviceStatus();

// VFD OPERATION MODE
	int		fnVFD_Initialize(int nPortNum = 13, int nBaudRate = 38400);
													// ������۹��ʱ�ȭ
	int		fnVFD_DeInitialize();					// ��������
	int		fnVFD_InitializeDisplay();				// ȭ������/�������ʱ�ȭ
	int		fnVFD_ClearDisplay();					// ȭ������
	int		fnVFD_ScrollDisplay(int nShiftNum, int lnRepeatNum, int nInterval);
													// ȭ��Scroll
	int		fnVFD_BlinkDisplay(int nPattern, int nNormalTime, int nReverseTime, int nRepeatNumber);
													// ȭ��Blink
	int		fnVFD_RealTimeTextDisplay(LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName);	
	int		fnVFD_RealTimeBitImageDisplay(LPCTSTR szFileName, int nOffsetX, int nOffsetY);
	int		fnVFD_RealTimeTextBitImageDisplay(LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName, LPCTSTR szFileName, int nImgOffsetX, int nImgOffsetY);
													// Text�� Bitmap Image File�� Overlapping�Ͽ� Display. 04.11.18

	int		fnVFD_FROMTextSave(int nMemIndex, LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName);
	int		fnVFD_FROMBitImageSave(int nMemIndex, LPCTSTR szFileName, int nOffsetX, int nOffsetY);
	int		fnVFD_FROMTextBitImageSave(int nMemIndex, LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBolStyle, int nItalic, LPCTSTR szFontName, LPCTSTR szFileName, int nImgOffsetX, int nImgOffsetY);
	int		fnVFD_FROMDisplay(int nMemIndex);

/////////////////////////////////////////////////////////////////////////////
//	SENSOR�Լ�(SNS) : for T2ATM Demo 2006.07.25
/////////////////////////////////////////////////////////////////////////////
// SNS SET MODE
	int		fnSNS_ClearOperatorSwitch();
	int		fnSNS_ClearProximity();
// SNS GET MODE
	int		fnSNS_GetOperatorSwitch();
	int		fnSNS_GetProximity();
// SNS OPERATION MODE
	int		fnSNS_Initialize();
	int		fnSNS_Deinitialize();

	//  �����ATM Start
	int		fnSNS_GetEarPhoneStatus();
	int		fnSNS_ClearPhoneStatus();

	void	TTS_initialize();
	void	TTS_StartProc();
	void	TTS_EndProc();										// V07-01-11-#00
	void	fnAPL_TTSplay();
	void	fnAPL_TTSstop();

	void	fnAPL_MakeVoicePlay(int InitFlag);					// V06-07-01
	//  �����ATM End


/////////////////////////////////////////////////////////////////////////////
//	RFID�Լ�(RFD) : for T2ATM Demo 2006.04.20
/////////////////////////////////////////////////////////////////////////////
// RFD SET MODE
	int		fnRFD_ClearErrorCode();					// ����ڵ��ʱ�ȭ
// RFD GET MODE
	CString	fstrRFD_GetErrorCode();					// ����ڵ屸�ϱ�
	CString	fstrRFD_GetData();
	int		fnRFD_AnalData(LPCTSTR szData);			// ���ܼ�����Ÿ�м�		2004.02.05

// RFD OPERATION MODE
	int		fnRFD_Initialize();
	int		fnRFD_Deinitialize();
	int		fnRFD_SendData(int nSendCmd, LPCTSTR szSendData);

	int		fnRFC_Initialize();						// RF���� �ʱ�ȭ
	int		fnRFC_GetBasicInfo();					// RF�⺻���� ���ϱ�
	int		fnRFC_GetDevStatus();					// RF���� ���ϱ�
	int		fnRFC_SelectMode(int nMode);			// RF���� ��弳��
	int		fnRFC_ReqCharge(char *szMoney, char *szTime);
													// RF����
	int		fnRFC_DrawBack(char *szTime);			// RFȯ��
	int		fnRFC_AnalRecvData(int nCmdType);

/////////////////////////////////////////////////////////////////////////////
//	GIRO�Լ�(GRO)
/////////////////////////////////////////////////////////////////////////////
// GRO SET MODE
	int		fnGRO_ClearErrorCode();					// ����ڵ��ʱ�ȭ
	int		fnGRO_ClearRetractCnt();				// ȸ���ż��ʱ�ȭ

// GRO GET MODE
	CString fstrGRO_GetErrorCode();					// ����ڵ屸�ϱ�
	CString fstrGRO_GetMediaStatus();				// ��ü�������ϱ�
	CString	fstrGRO_GetDeviceStatus();				// ��ġ���±��ϱ�

	int		fnGRO_GetMaterialInfo();				// ��ü�ܷ��������ϱ�

	CString fstrGRO_GetRetractCnt();				// ȸ���ż����ϱ�

	CString fstrGRO_GetGiroData();					// �����ڷᱸ�ϱ�
	CString fstrGRO_GetGiroType();					// �����������ϱ�

// GRO OPERATION MODE
	int		fnGRO_Initialize();						// ������۹��ʱ�ȭ
	int		fnGRO_Deinitialize();					// ��������

	int		fnGRO_Accept(int nWaitSec = K_30_WAIT);	// �����㰡(���Դ��ð�)�׿���
	int		fnGRO_CancelAccept();					// ���Ա����״ݱ�
	int		fnGRO_ReadyAccept();					// �����غ�
	int		fnGRO_Read(LPCTSTR szFileName);			// �б�(�̹����̸�)
	int		fnGRO_2DRead(LPCTSTR szFileName);		// 2D�б�(�̹����̸�)
	int		fnGRO_ReadOCRn2D(LPCTSTR szFileName);	// OCR & 2D�б�(�̹����̸�)
	int		fnGRO_Read2DNotGetImg(LPCTSTR szFileName);
													// 2D�б�(�̹����̸�)
	int		fnGRO_Reject();							// ��ȯ
	int		fnGRO_RejectAll();						// ��ü��ȯ
	int		fnGRO_Escrow();							// �ӽú���
	int		fnGRO_Stack();							// ����

	int		fnGRO_EnableStatusEvents();				// ���º������
	int		fnGRO_DisableStatusEvents();			// ���º���Ұ�

/////////////////////////////////////////////////////////////////////////////
//	SCR�Լ�(SCR)
/////////////////////////////////////////////////////////////////////////////
// SCR SET MODE
	int		fnSCR_ClearErrorCode();					// ����ڵ��ʱ�ȭ
	int		fnSCR_ClearKeyData();					// Ű���ʱ�ȭ
	int		fnSCR_SetScreenData(LPCTSTR szSetScreenDataName, LPCTSTR szSetScreenDataValue);
													// ��������(�ڷ��̸�,�ڷᰪ)
	int		fnSCR_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue);
													// ����ڷ�����(����ڷ��̸�,����ڷᰪ)
	int		fnSCR_SetCurrentLangMode(int nLangMode = KOR_MODE);				
													// �������弳��	+ MachineFileType		

// SCR GET MODE
	CString	fstrSCR_GetErrorCode();					// ����ڵ屸�ϱ�
	int		fnSCR_GetDeviceStatus();				// ��ġ���±��ϱ�

	CString	fstrSCR_GetKeyData();					// �Է�Ű�����ϱ�
	CString	fstrSCR_GetKeyString(int nInitFlag = TRUE);	
													// �����Է�Ű�����ϱ�(��������:0=��������)
	CString	fstrSCR_GetKeyNumeric(int nRetLength = 0, int nInitFlag = TRUE);	
													// �����Է�Ű�����ϱ�(��������:0=��������)
	CString	fnSCR_GetCheckScreen(int nScreenNo, int nLangMode = KOR_MODE);	
													// ȭ�������������ϱ�(ȭ���ȣ,�����)
	int		fnSCR_GetCurrentScreenNo();				// ����ȭ���ȣ���ϱ�
	int		fnSCR_GetCurrentLangMode();				// �������屸�ϱ�

// SCR OPERATION MODE
	int		fnSCR_Initialize();						// ������۹��ʱ�ȭ
	int		fnSCR_Deinitialize();					// ��������

//t	int 	fnSCR_DisplayScreen(int nScreenNo, int nDisplaySec = 0);
	int 	fnSCR_DisplayScreen(int nScreenNo,								// ȭ���ȣ
								int nDisplaySec = K_NO_WAIT,				// ȭ����½ð�
								int nPinInputMode = PIN_DISABLE_MODE,		// PinPad�Է¹��
								LPCTSTR szCardData = "",					// Macing�� ī�嵥��Ÿ(EPP���)
								int nPinPassWordMin = 4,					// ��й�ȣ�ּ��Է��ڸ���
								int nPinPassWordMax = 8,					// ��й�ȣ�ִ��Է��ڸ���
								int nPinPassWordAuto = FALSE,				// �ִ��ڸ����Է½��ڵ��Է�����
								LPCTSTR szPinPassWordTerm = "ENTER,CANCEL");// �Է�����ó���Է°�
													// ȭ�����(ȭ���ȣ,�����...)
	int		fnSCR_DisplayImage(int nImageNo, int nOnOffFlag);
	int		fnSCR_DisplayString(int nStringNo, LPCTSTR szString, int nInLength, int Flag = FALSE);
													// �������(���ڹ�ȣ,����,����)
	int		fnSCR_DisplayString(int nStringNo, LPCTSTR szString);
	int		fnSCR_DisplayStringArray(int nStringNo, LPCTSTR szString, LPCTSTR szSeperator = "|");													// �������(���ڹ�ȣ,����)
	int		fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric, int nInLength);
													// �������(���ڹ�ȣ,����,����)
	int		fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric);
													// �������(���ڹ�ȣ,����)
	int		fnSCR_DisplayNumeric(int nStringNo, int nNumeric);
													// �������(���ڹ�ȣ,����)
	int		fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, int nInLength, char cMarkChar = ' ', int nIsMinus = FALSE);
													// �ݾ����(���ڹ�ȣ,����,����,��ũ,��ȣ)
	int		fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, char cMarkChar = ' ', int nIsMinus = FALSE);
													// �ݾ����(���ڹ�ȣ,����,��ũ,��ȣ)
	int		fnSCR_DisplayAmount(int nStringNo, int nAmount,	char cMarkChar = ' ', int nIsMinus = FALSE);
													// �ݾ����(���ڹ�ȣ,�ݾ�,��ũ,��ȣ)

/////////////////////////////////////////////////////////////////////////////
// �ѱ��Է�
/////////////////////////////////////////////////////////////////////////////
	void	fnSCR_HANGULInit();
	CString	fnSCR_GETKEYHANGUL(int	scrNum, int strNum,	BYTE *keyBuff, int inLen, int LimitTime, int *RetCnt, int SizeMin);
	void	fnSCR_CALLchar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	fnSCR_Input2Byte(BYTE* TBuff, BYTE* SBuff , int Len);
	int		fnSCR_KStrlen(HAN_INPUT_KEY_BUFFER *p);
	int		fnSCR_KStrlen(BYTE *buf);		

	HAN_INPUT_KEY_BUFFER m_key_buf;

	BYTE	m_text[25][200];
	BYTE	m_text_Buff[200];

/////////////////////////////////////////////////////////////////////////////
//	PIN FUNCTION(PIN)
/////////////////////////////////////////////////////////////////////////////
// PIN SET MODE
	int		fnPIN_ClearErrorCode();					// Clear Error Code

// PIN GET MODE
	CString	fstrPIN_GetErrorCode();					// Get Error Code
	int		fnPIN_GetDeviceStatus();				// Get Device Status

	CString	fstrPIN_GetPinKeyData();				// Get Pin Key Data
	CString	fstrPIN_GetMacingData();				// Get Macing Data

// PIN OPERATION MODE
	int		fnPIN_Initialize();						// Initialize
	int		fnPIN_Deinitialize();					// Deinitialize

	int		fnPIN_EntryEnable(int nEnableMode, int nMinKeyCount, int nMaxKeyCount, int bAutoEnd, LPCTSTR szActiveKeys, LPCTSTR szTerminatorKeys, LPCTSTR szCardData, int nTimeout = K_WAIT);
													// Entry Enable(EnableMode, MinKeyCount, MaxKeyCount, AutoEnd, ActiveKeys, TerminatorKeys, CardData, Timeout)
	int		fnPIN_EntryDisable();					// Entry Disable

	int		fnPIN_LoadIV(LPCTSTR szKeyName);		// Load IV(Key Name)
	int		fnPIN_LoadKey(LPCTSTR szKeyName, LPCTSTR szKeyValue);		
													// Load Key(Key Name, Key Value)
	int		fnPIN_MacingData(LPCTSTR szMacData);	// Macing Data(Mac Data)
	int		fnPIN_PinLoadEncryptedKey(LPCTSTR szKeyName, LPCTSTR szKeyValue, LPCTSTR szEncKeyName);

/////////////////////////////////////////////////////////////////////////////
//	SOUND�Լ�(SND)
/////////////////////////////////////////////////////////////////////////////
// VOL SET MODE
	int		fnSND_SetCurrentVolume(int nVolume);	// ��������

// VOL GET MODE
	int		fnSND_GetCurrentVolume();				// ����������

/////////////////////////////////////////////////////////////////////////////
// FTP�Լ�(FTP) 
/////////////////////////////////////////////////////////////////////////////
	int		fnFTP_Initialize();						// �ʱ�ȭ
	int 	fnFTP_Connection(LPCTSTR szSvrIP, LPCTSTR szLoginID, LPCTSTR szLoginPwd);
													// �������
	int		fnFTP_Deinitialize();					// ��������
	int 	fnFTP_FindFirstFile(LPCTSTR szFindFile);// ����ã��
	int 	fnFTP_GetFile(LPCTSTR szSvrDir, LPCTSTR szSvrFile);
													// ���ϰ�������
	int 	fnFTP_PutFile(LPCTSTR szLocalFile, LPCTSTR szSvrFile);
													// ���Ͽø���
	int 	fnFTP_DeleteFile(LPCTSTR szFile);		// ���������
	int 	fnFTP_RenameFile(LPCTSTR szSrcFile, LPCTSTR szDstFile);
													// �����̸��ٲٱ�
	int 	fnFTP_OpenFile(LPCTSTR szFile, int nOption);
													//���Ͽ���
	int 	fnFTP_CreateDir(LPCTSTR szDir);			// ���丮�����
	int 	fnFTP_RemoveDir(LPCTSTR szDir);			// ���丮�����
	int 	fnFTP_SetCurDir(LPCTSTR szDir);			// ������丮�ٲٱ�
	int 	fnFTP_GetCurDir(LPCTSTR szDir);			// ������丮�о����


/////////////////////////////////////////////////////////////////////////////
//	�����Լ�(APL)
/////////////////////////////////////////////////////////////////////////////
// APL SET MODE
// APL GET MODE
// APL OPERATION MODE
	int		fnAPL_InitializeDS();					// �����ʱ�ȭ

	int		fnAPL_LoadDevice();						// ��ġ�ø���
	int		fnAPL_UnLoadDevice(int nEndFlag = FALSE);					// ��ġ������

	int		fnAPL_GetDefineDevice(int nDevId);		// ���ǵ���ġ���ϱ�
	int		fnAPL_GetYesDevice(int nDevId);			// ���ǵǰ�����ġ���ϱ�
	int		fnAPL_GetNoDevice(int nDevId);			// ���ǵǰ�̿�����ġ���ϱ�
	int		fnAPL_GetDownDevice(int nDevId);		// ���ǵǰ�ٿ���ġ���ϱ�
	int		fnAPL_GetAvailDevice(int nDevId);		// ��밡������ġ���ϱ�
	int		fnAPL_GetAvailErrorDevice(int nDevId);	// ����ϰ���ֳ���ġ���ϱ�
	int		fnAPL_GetErrorDevice(int nDevId);		// ��ֳ���ġ���ϱ�
	int		fnAPL_GetDownErrorDevice(int nDevId);	// ��ֳ���ġ���ϱ�

	int		fnAPL_GetNumberOfCash();				// ���ݸż����ϱ�
	int		fnAPL_GetNumberOf50000Cash();			// ���ݸż����ϱ�(5������)	
	CString fnAPL_GetOutCheckBankNo();				// ��ݼ�ǥ�����ڵ屸�ϱ�
	CString fnAPL_GetOutCheckBankNo2();				// ��ݼ�ǥ�����ڵ屸�ϱ�2
	int		fnAPL_GetNumberOfOutCheck();			// ��ݼ�ǥ�ż����ϱ�
	CString	fnAPL_GetNumberOfOutCheckNo();			// ��ݼ�ǥ��ȣ���ϱ�
	int		fnAPL_GetNumberOfOutCheck2();			// ��ݼ�ǥ�ż����ϱ�
	CString	fnAPL_GetNumberOfOutCheckNo2();			// ��ݼ�ǥ��ȣ���ϱ�
	int		fnAPL_GetKindOfOutCheck();				// ��ݼ�ǥ�������ϱ� ��ݼ�ǥ������ȸ
	int		fnAPL_GetNumberOfInCheck();				// �Աݼ�ǥ�ż����ϱ�
	int		fnAPL_SetNumberOfCash(int nCashCnt);	// ���ݸż�����
	CString	fnAPL_SetOutCheckBankNo(LPCTSTR szBankNo, LPCTSTR szBankNo2);// ��ݼ�ǥ�����ڵ弳��
	int		fnAPL_SetNumberOfOutCheck(int nOutCheckCnt, int nOutCheckCnt2);
													// ��ݼ�ǥ�ż�����
	CString	fnAPL_SetNumberOfOutCheckNo(LPCTSTR szOutCheckNo, LPCTSTR szOutCheckNo2);
													// ��ݼ�ǥ��ȣ����
	int		fnAPL_SetNumberOfInCheck(int nInCheckCnt);
													// �Աݼ�ǥ�ż�����
//	int		fnAPL_SetKindfOutCheck(int nOutCheckKind);
													// ��ݼ�ǥ��������
	int 	fnAPL_GetAvailTrans();					// �ŷ����ɻ��±��ϱ�
	int 	fnAPL_GetAvailWithdraw();				// ��ݰ��ɻ��±��ϱ�
	int 	fnAPL_GetAvailDeposit();				// �Աݰ��ɻ��±��ϱ�
	int 	fnAPL_GetAvailPassbook();				// ���尡�ɻ��±��ϱ�
	int 	fnAPL_GetClerkTransOnOff();				// ����ŷ���屸�ϱ�
	int 	fnAPL_GetAvailA4printer();				// A4 �����Ͱ��ɻ��±��ϱ�

	int		fnAPL_SetSensorInfo();					// ������������
	int		fnAPL_SetDate(LPCTSTR szDate);			// �ŷ����ڼ���
	int		fnAPL_SetCheckDate(LPCTSTR szCheckDate);// ��ǥ���ڼ���
	int		fnAPL_ClearSerialNo();					// �Ϸù�ȣ�ʱ�ȭ
	int 	fnAPL_AddSerialNo();					// �Ϸù�ȣ����
	int		fnAPL_SetSerialNo();					// �Ϸù�ȣ����
	int 	fnAPL_SetProcCount(char chProcCount);	// �����ȣ����
	int 	fnAPL_SetOnTransaction(int nTransDoing);// �ŷ������߼���
	int		fnAPL_SetTranStart();					// �ŷ����ۼ��� : �ŷ����۽ýð�����(fnAPL_SetOnTransaction(ON)+fnAPL_SetProcCount(1)) : 2003.11.21
	int 	fnAPL_ClearDeviceExecOffInfo(int nInitFlag = FALSE);			
													// ��ġ��Ұ������ʱ�ȭ
	int		fnAPL_CshRetactCntInfo(int nCnt = 0, int nInitFlag = FALSE);
	int		fnAPL_SetClerkPowerOff();				// �Ŀ����°���
	int		fnAPL_CheckPowerStatus(int nRealFlag = FALSE);	// �Ŀ����°���
	int 	fnAPL_CheckPowerOffError();				// �Ŀ�������ְ���

	int		fnAPL_ClearError();						// ��������ʱ�ȭ
	int 	fnAPL_CheckError();						// ��ְ���
	int		fnAPL_CheckErrorCodeValid(LPCTSTR szErrorCode);		
													// ����ڵ������˻�
	int 	fnAPL_StackError(LPCTSTR szErrorCode, LPCTSTR szErrorString, int nDevId = DEV_NONE, int nTranType = FALSE);
													// ��ּ����������������
	CString	fnAPL_ErrorConv(LPCTSTR szErrorCode);	// U8100 ErrorCode Conversion
	CString	fnAPL_ErrorU3100KConv(LPCTSTR szErrorCode);	// fnAPL_ErrorU3100KConv ErrorCode Conversion
	int 	fnAPL_CheckHostLine();					// ȣ��Ʈ���ΰ���
	int		fnAPL_CheckHostOpen();					// ȣ��Ʈ��������
	int		fnAPL_CloseLine();						// ȣ��Ʈ������
	int		fnAPL_CloseHost();						// ȣ��Ʈ��
	int		fnAPL_OpenLine();

	int		fnAPL_InitializeDevice(int nDevId, int nInitFlag = FALSE);
													// ��ġ�ʱ�ȭ
	int		fnAPL_CheckDeviceAction(int nDevId);	// ���۴�����ְ���
	int		fnAPL_CheckMaterial(int nDevId = DEV_MAIN);		
													// ��ü�ܷ��������ܷ���ֹ߻�
	int		fnAPL_RetractMaterial(int nDevId);		// �нǸ�üȸ��
	int		fnAPL_AutoOffErrorDevice();				// ��ֳ���ġ�ڵ�����
	
	int 	fnAPL_DisplayNormalSegment();			// ����Segment���
	int 	fnAPL_DisplayErrorSegment(char chProcCount, LPCTSTR szErrorCode);
													// ���Segment���
	int 	fnAPL_DisplayProcCountSegment();		// �Segment���
	int 	fnAPL_DisplayInformationLed(int nInitFlag = FALSE);			
													// ����Led���
	int 	fnAPL_DisplayInformationLedOnOff(int nLedIndex, int bOnOff);
													// ����Led��� - ����ó��
	int		fnAPL_DisplayInformationIndicator(int nInitFlag = FALSE);			
													// ����Indicator���
	int 	fnAPL_SendInformationRpl(int nInitFlag = FALSE);			
													// ����Rpl����
	int 	fnAPL_SendInformationInform(int nInitFlag = FALSE);			
													// ����Inform����
	
	int		fnAPL_DisplayError(int nErrorStatus, int nInitFlag = FALSE);
													// ���ȭ����¹�����
	int 	fnAPL_PrintError(int nDevId);			// �������

	int 	fnAPL_CheckPrintDevicePosition();		// ��ġ����ġ����
	int 	fnAPL_CheckPrintBrmPosition();			// Brm����ġ����
	int 	fnAPL_CheckPrintUcmPosition();			// Ucm����ġ����
	int		fnAPL_CheckPrintRearDoorStatus();		// �x����������
	int 	fnAPL_PrintPosition(LPCTSTR szPrintData);
													// ����ġ����
	
													//:5����������(nFiftyThRJT �߰�)
													// V06-02-01-#07:����ȸ���� 5���� �� ������ǥ�ø� ���� Param �߰� (nTenThMoveCnt, nFiftyThMoveCnt)
	int		fnAPL_PrintCashMoveInfo(int nMoveFlag, int nRealMove, int nTenThRJT, int nFiveThRJT, int nOneThRJT, int nUnknownRJT, 
									int nFiftyThRJT = 0, int nTenThMoveCnt = 0, int nFiftyThMoveCnt = 0);
													// �����̵���������
	int		fnAPL_PrintCashInfo();					// ���ݵ����������
	int		fnAPL_PrintCheckInfo();					// ��ǥ�����������
	int		fnAPL_PrintJnlInfo(LPCTSTR szJnlInfoData, int JnlType = JNL_OPER, int nWaitFlag = FALSE);
													// ��������

	int		fnAPL_SprAutoload();					// ��ǥ ����ε�

	int 	fnAPL_AutoRefillBrm(int nBrmRefillMode);// Brm�ڵ�����/ȸ��
	int 	fnAPL_AutoResetBrm();					// Brm�ڵ�����
	int 	fnAPL_AutoReadyBrm();					// Brm�ڵ��غ�

	int		fnAPL_ResetDevice(int nDevId, int nInitFlag = FALSE);				
													// ��ġ����
	int		fnAPL_UnUsed();							// �������
//t	int 	fnAPL_DeviceEnDisable(int nDevId, int nAction, int nWaitFlag = FALSE);
	int 	fnAPL_DeviceEnDisable(int nDevId, int nAction, int nWaitFlag = FALSE, int nPinInputMode = PIN_DISABLE_MODE, LPCTSTR szCardData = "", int nPinPassWordMin = 4, int nPinPassWordMax = 6, int nPinPassWordAuto = FALSE, LPCTSTR szPinPassWordTerm = "ENTER,CANCEL");
													// ��ġ���Ը�弳��(Pinpad�߰�Ȯ��ó��)
	int 	fnAPL_DeviceSetFlicker(int nDevId, int nAction);

	int 	fnAPL_GetDeviceEvent(int nDevId);		// ������ġ���ϱ�

	int		fnAPL_ECashEjrSpoolPrint(int nDevId, LPCTSTR szSpoolData, NEjrTblDump* pEjrData = NULL, EjrAmountTbl* pEjrAmountData =  NULL);
	int		fnAPL_EjrSpoolPrintStart(int nDevId, EjrTbl* pEjrData = NULL, EjrAmountTbl* pEjrAmountData = NULL);
													// �������ν���
	int		fnAPL_EjrSpoolData(int nDevId, int nLine, int nSize, LPCTSTR szSpoolData);
													// ��������
	int		fnAPL_EjrSpoolEnd(int nDevId, int JnlType = JNL_TRAN, int nWaitFlag = FALSE);
													// ������������&���������ó��
	int		fnAPL_EjrAddSerialNo();					// ���������Ϸù�ȣ����
	int		fnAPL_EjrSave(int JnlType);				// ������������
	int		fnAPL_EjrDbManagerSend(LPCTSTR szDbData);
													// ���������ڷ�����

	int		fnAPL_CaptureHandSave(int nIndex, int nFirstFlag = FALSE, LPCTSTR szDefault = NULL);		
	// ���Կ�
	int		fnAPL_CaptureFaceSave(int nIndex, int nFirstFlag = FALSE, LPCTSTR szDefault = NULL);	
	// ���Կ�
	int		fnAPL_CaptureRoomSave(int nIndex, int nFirstFlag = FALSE, LPCTSTR szDefault = NULL);	
	// U8100 ���߰��� �ĸ��Կ�
	int		fnAPL_CaptureVODSave(int nStartEndFlag = FALSE, int nCaptureVODPart = FALSE);
	// �������Կ� U8100-VOD
	int		fnAPL_CaptureSaveEnd(int nSaveFlag = TRUE);	 // �Կ�����&����ó��

	int		fnAPL_CaptureFaceAtmShocked(int nStep = 0);
	int		fnAPL_CaptureFaceAuthLockDoorOpen(int nStep = 0);
	int		fnAPL_CaptureFaceCustomDoorOpen(int nClearFlag = FALSE);

	char	fnAPL_GetErrorCodeGrade(LPCTSTR szHsErrorCode);
	int		fnAPL_GetErrorCodeKind(LPCTSTR szHsErrorCode);
													// ����ڵ��������ϱ�
	CString	fnAPL_GetLC(LPCTSTR szHsErrorCode, int nTranProc = TRAN_NULL);
													// LC���ϱ�

	int		fnAPL_ClerkInitModeSet();				// ����ʱ�ȭ��弳��
	int		fnAPL_ClerkInformation(int nInitFlag = FALSE);			
													// U8100 AP ���泻�� #12 - ������OM���� ���� : T3����
	int		fnAPL_ClerkExecCheck();					// �������˻�

	int		fnAPL_SetSegment(LPCTSTR szValue);		// Segment���� ������Լ�
	int		fnAPL_PowerOff(int nWaitSec);			// ����Off(������ð�) ����� �Լ�
	int		fnAPL_GetPowerStatus();					// �������±��ϱ� ������Լ�

//////////////////////////////////////////////////////////////////////////
	int		fnAPL_VFDDislpay(int nPos);				// VFD����
	int		fnAPL_CheckCatchPlay();					// �������˻�
	int		m_nVfdDoing;							// VFD Thread��������
	int		VFDTotalCount;							// VFDǥ���ѹ��ڰ���
	int		VFDCurCount;							// VFDǥ��������ġ
	int		VFDDisTerm;								// VFDǥ�ýð�����(��)
	int		m_nTrPollDoing;							// Polling ��������
	int		m_SendRecvStaus;						// Polling ��������

	int		PrevInformFlg;										// �����˸� �ߴ�/�簳 ������
	int		PBMInkCheckCnt;										// PBM Ink Chekc Count (1000ȸ�� �ѹ��� ��ũ üũ)
	CString	PBMInkStatus;										// PBM Ink ����
	int		CheckPbmInkStaus;									// PBM Ink ������������
	int		fnAPL_CheckLEDDipalyData(CString strDisplay);		// LED ���� ���� �˻� 
	int		fnAPL_SetLedInformation();							// U8100 ����
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	int		m_nJournalMode;										// ���θ��(���� or ��������)
	int		m_nAtmsCommand;										// ATMS��ɱ���
	int		m_nAtmsDoing;										// ATMS��ɼ�������Ȯ��
	BYTE	m_szJnlIndexBuff[NETBUFFSIZE];
	int		m_nJnlIndexLength;
	CString	m_szSaveTime;							// �������νð�����
	int		m_nJnlType;								// ��������Ÿ������

/////////////////////////////////////////////////////////////////////////////
//	SOUND�Լ�(SND)
/////////////////////////////////////////////////////////////////////////////
	int		fnSND_KeySound(LPCTSTR fname = NULL);	// V03-01-01 SPL KEY����ó��



//	LOCK Function
	int	    fnLDCtrl_Initialize();
	int	    fnLDCtrl_Deinitialize();
	int		fnLDCtrl_OpenRpcLock(LPCTSTR lpSetOpenKey, int nOption);
	int		fnLDCtrl_CloseLock();
	int		fnLDCtrl_GetDoorStatus(int nFlag = 0);
	int		fnLDCtrl_GetLoadKeyCheckFlag(LPCTSTR lpSetOpenKey);
	int		fnLDCtrl_GetDeviceStatus();
	int		fnLDCtrl_GetKeyCount();
	int		fnLDCtrl_SetClearLogandKey(BOOL bClearKeyTable = FALSE, BOOL bClearLog = FALSE);
	int		fnLDCtrl_SetKeytableFile(int nKeyType, void* pData);
	int		fnLDCtrl_GetErrorCode(char* szReturnValue);
//	RPC Function
	int		fnLDCtrl_PowerOnRpc(int nPort = 3);
	int		fnLDCtrl_PowerOffRpc(int nPort = 3);
	int		fnLDCtrl_PowerStatusRpc(int nPort = 0);
	int		fnLDCtrl_PowerResetRpc(int nPort = 3);
	int		fnLDCtrl_EnableWatchdogRpc(int nPort = 0);
	int		fnLDCtrl_DisableWatchdogRpc(int nPort = 0);
	int		fnLDCtrl_SetTimeWatchdogRpc(int nInterval = 0);
	int		fnLDCtrl_Ch1TimeResetRpc(int nInterval = 0);
	int		fnLDCtrl_Ch2TimeResetRpc(int nInterval = 0);


// SALERT4PACK Function
	int	    fnSPACK_Initialize();
	int	    fnSPACK_Deinitialize();
	int		fnSPACK_AlarmOnOff(int nOnOff);
	int		fnSPACK_ReSet();
	int		fnSPACK_GetDeviceStatus();
	int		fnSPACK_GetGSensorSensibility();
	int		fnSPACK_GetAlarmStatus();
	int		fnSPACK_GetButtonPushStatus();
	//"#0075"
	int		fnSPACK_GetExternalSensorStatus_1();
	int		fnSPACK_GetExternalSensorStatus_2();
	int		fnSPACK_GetExternalSensorEnableStatus_1();
	int		fnSPACK_GetExternalSensorEnableStatus_2();
	int		fnSPACK_GetDipSwitchStatus_1();
	int		fnSPACK_GetDipSwitchStatus_2();
	int		fnSPACK_GetGSensor();
	int		fnSPACK_GetBatteryLowStatus();
	int		fnSPACK_SetEnableControl(int nbyAlramEnable, int nbyExternal1Enable, int nbyExternal2Enable);

/////////////////////////////////////////////////////////////////////////////
//	��Ÿ�Լ�(UTIL)
/////////////////////////////////////////////////////////////////////////////
	int		fnUTL_KillExProgram(LPCTSTR pName);		
	void	fnUTL_SetClerkScreen_T3_Van();
	ULONG	fnUTL_Asc2ULong(void* lpvoid, int length);
	ULONG	fnUTL_ByteLowOrder2Long(BYTE* Src, int inLen);
	CString fnUTL_WinExecWaitEndTrace(LPCTSTR lpCmdLind, UINT uCmdShow, DWORD dSleep = INFINITE);
	int		fnUTL_WinExecWaitEndUtil(LPCTSTR lpCmdLind, UINT uCmdShow, DWORD dSleep = INFINITE);
	void    fnUTL_PinDataXor(BYTE* src1, BYTE* src2, BYTE* dst,int size);
	void	fnUTL_PinBlockPackXor(BYTE* src1, BYTE* src2, BYTE* dst,int size);
	void	fnUTL_JisPack(BYTE* src, BYTE* dst, int size);
	void	fnUTL_JisPackXor(BYTE* src, BYTE* dst, int size);
	void	fnUTL_UnpackPinData(BYTE* src, BYTE* dst, int size);
	int		fnUTL_MakeDirectory(char *lpStr);
	int		fnUTL_RegSetValueExtUtil(LPCTSTR hKeyName, LPCTSTR lpszValueName, DWORD dwType, DWORD dwLen, LPVOID lpszData);
	int		fnUTL_RegQueryValueExtUtil(LPCTSTR hKeyName, LPCTSTR lpszValueName, DWORD dwType, DWORD dwLen, LPVOID lpszData);

	int		fnUTL_UserRegGetInt(LPCTSTR hKeyName, LPCTSTR lpszValueName, int InitValue = NULL);
	CString	fnUTL_UserRegGetStr(LPCTSTR hKeyName, LPCTSTR lpszValueName, LPCTSTR InitValue = NULL);
	int		fnUTL_UserRegSetInt(LPCTSTR hKeyName, LPCTSTR lpszValueName, int SetValue);
	int		fnUTL_UserRegSetStr(LPCTSTR hKeyName, LPCTSTR lpszValueName, CString SetValue);
	int		fnUTL_MsgPeekMessageProc(DWORD dTime = 0);
	int		fnUTL_TranWithCanCelFlagClear();
	int		fnUTL_TranWithCanCelFlagSet();
	int		fnUTL_IniGetInt(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, int nDefault = NULL);
	CString fnUTL_IniGetStr(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, LPCTSTR szDefault = NULL);
	int		fnUTL_IniSetInt(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, int nValue);
	int		fnUTL_IniSetStr(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, LPCTSTR szValue);
	int		fnUTL_IniFlush(LPCTSTR szFileName);
	int		fnUTL_TextFileSetStr(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, LPCTSTR szValue);
	int		fnUTL_ActionJnlPrint(LPCTSTR szSelect, char* fmt = NULL, ...);
	int		fnUTL_RunCheckProcess(CString szAtmName);		
	int		fnUTL_RunCheckProcess(LPCTSTR szAtmName);
	int		fnUTL_FTP_Download_File(int bF = FALSE);
	void	fnUTL_HangulCodeConvert(int nFlag, char* strSrc, char* strDest, int nSize);		
	unsigned int  Binary(unsigned int val, unsigned int range);							
	void	TG2KS(unsigned char *c1, unsigned char *c2);							
	void	KS2TG(unsigned char *c1, unsigned char *c2);								
// Get Log Start ����������������������������������������������
	int		fnAPL_GetLogFileStart(int nMonth, int nDate);		// 2006.11.22 : Get Log File(��, ��)
	int		fnAPL_GetDriveInfo(CString& szDrive, DWORD& dwFreeSize, TCHAR cStartDirve = 'A');
	int		fnAPL_DeleteFiles(LPCTSTR DirName);
// Get Log End ����������������������������������������������
	int		fnAPL_NetworkInfoSet(int nFixIp = FALSE);					// ��Ʈ��ũ IP Setting
	int		fnAPL_CheckMachineType();				
	int		fnAPL_SetComputerName();				
	int		fnAPL_GetRemoteCtrlFileStart(CString strLocalPath, int nYear, int nMonth, int nDate, int nTran, int nSeralF = FALSE);		// 2006.11.22 : Get Log File(��, ��)

	int		fnAPP_APVersionCheck();								// ��� AP�� FileVersion�� üũ�Ͽ� �ٸ���� ���ó��
	CString	fnUTL_GetFileVer(LPCTSTR szFile);					// �����������
	// ��������������������������������������������������������������������������������������������������������������
	
	int	nTranStopFlg;										// AP File Version����ġ�� �ŷ��ߴ� Flag
	// -------------------------------------------------------------


	// Hi-Pass���˽��������� #N0266 
/////////////////////////////////////////////////////////////////////////////
//	�����н� �Լ�(HPS)
/////////////////////////////////////////////////////////////////////////////

	int		fnHPS_PowerOn();						// ��������
	int		fnHPS_PowerOff();						// ��������

// HiPass Dev<->IC Process
	int		fnHPS_ReadBalance();
	int		fnHPS_PrepareLoadIEP(LPBYTE lpICSendData);	
	int		fnHPS_LoadIEP(LPBYTE lpICSendData);
	int		fnHPS_CompleteTran();
	int		fnHPS_ReadLLog();
	int		fnHPS_ReadPan();
	
	int		fnHPS_AnalRecvData(int nCmdType);		// HiPass IC Chip Receive Data �ؼ�
	int		fnHPS_APDUInitialize(LPCTSTR szICSendData);
	int		fnHPS_APDUBalRead();
// ����������������������������������������������������������������������������
	
	int		fnAPL_BRMCashInfo();					// �α׺м����
	int		fnAPL_BRMCstSetInfo();					// U3100K - 5������ �ݾ׼���
	int		fnAPL_BRMCstSetInfo_Empty_On();			// U3100K - �߰� ���� ��å ��.
	int		fnAPL_BRMCstMatchInfo();				// U3100K - ī��Ʈ ���� ���� 
	int		fnBRM_ReadyStore();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDevCmn)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CDevCmn)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
UINT	thrVfdThread(LPVOID pParam);							// T2ATM 2006.5.24 yscho
UINT	thrPreviewThread(LPVOID pParam);						// V06-02-01-#05:Camera Preview Thread(OM���κ����� ������ ����� ó��)
UINT	thrPollThead(LPVOID pParam);							// Nice Polling (30 Sec)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVCMN_H__C1CA462D_218C_4BA9_88BA_DBC13B14F86A__INCLUDED_)
