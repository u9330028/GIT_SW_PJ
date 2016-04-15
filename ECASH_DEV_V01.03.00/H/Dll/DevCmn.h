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

///////////////////////////	한글입력
#ifndef HANINPUT_H
	#include "HanInput.h"
#endif
///////////////////////////

#include "..\..\H\Dll\INIT_DLL.h"

DllUseport	BOOL WINAPI EmbossBmp(LPCTSTR imgPathName);		// EmbossDll.lib사용

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
	DEVICEINFO	AtmDefine;				// Atm기기구성 내부변수(변경불가)

	int		SprRowOption;				// bank/Ticket Row option
	int		HostLineMode;				// Online/Local
	int		HostLocalSvrMode;			// Online/Local
	int		HostSvrPortMode;			// Host Online/Local
	int		BrmCycleMode;				// Recycle/Uncycle
	int		BrmCashMode;				// RealCash/TestCash
	int		IdcVersion;					// 금융IC 관련 Version
	int		EjectBothAvail;				// V07-05-01-#02:카드/명세표 동시방출 가능여부
	int		ChkIcVersion;				// T99-99-99-#01 IC 버전 읽기 여부	

	int		GroDateMode;				// DateCheck/NotCheck
	int		GroCheckMode;				// DataCheck/NotCheck
	int		GroEscrowFlg;				// Escrow실행여부 품질개선으로 인한 추가 05.02.28 NKB

	int		AtmDoing;					// Atm실행여부
	int		AtmFirstDoingFlag;			// Atm최초실행Flag
	int		AtmStatus;					// Atm상태변수 
	int		AtmStatusSave;				// Atm이전상태변수
	int		AtmStatusReEntryFlag;		// Atm상태재진입Flag
	int		HostStatus;					// Host상태변수 
	int		HostStatusSave;				// Host이전상태변수
	int		HostFirstOpenFlag;			// Host최초개국Flag
	int		HostOpenFlag;				// Host개국Flag
	int		HostFirstReadyFlag;			// Host최초상태Flag
	int		PowerOffSendFlag;			// PowerOff전문송신변수
	int		TranStatus;					// Tran상태변수 
	int		TranStatusSave;				// Tran이전상태변수 
	int		TranProc;					// Tran거래처리
	int		TranResult;					// Tran거래결과

	int		DeviceDefine;				// 기기에 정의된	Device Set
	int		DeviceNoDevice;				// 미연결			Device Set
	int		DeviceDownDevice;			// Device Down Device Set
	int		DeviceStatus;				// 장애가 난		Device Set
	int		DeviceAutoOff;				// 자동축퇴가 난	Device Set
	int		DeviceRetract;				// 회수가 난		Device Set
	int		DeviceTran;					// 최초 Event		Device Set 
	int		DeviceTimeout;				// 미수취정보		Device set
	int		DeviceLoss;					// 미수취장애		Device set

	int		ForceDownFlag;				// 강제Down변수
	int		RemoteDownFlag;				// 강제Down변수
	int		ForceMissMatchDownFlag;     // MisMatch


	int		IcmRecvTime;				// Icm수신Time
	int		SprRecvTime;				// Spr수신Time
	int		JprRecvTime;				// Jpr수신Time
	int		McuRecvTime;				// Mcu수신Time
	int		BrmRecvTime;				// Brm수신Time
	int		UcmRecvTime;				// Ucm수신Time
	int		PbmRecvTime;				// Pbm수신Time
	int		VfdRecvTime;				// VFD수신Time		// T2ATM 2006.5.24 yscho
	int		A4pRecvTime;				// A4P수신Time
	int		GroRecvTime;				// Gro수신Time
	int		RfdRecvTime;				// Rfd수신Time

    int		OpenKey;					// '준비' '운영' 전환스위치 
    int		OpenKeySave;				// '준비' '운영' 전환스위치 저장
    int		ClerkKey;					// 계원조작  
    int		ClerkKeySave;				// 계원조작 저장
	int		PowerOffDetect;				// Power 상태변수
	int		RecoveryType;				// 0:자동복구,1:수동복구
	int		TranType;					// 0:고객거래무,1:고객사용유(장애송신관련)

	int		CurErrCnt;					// 현재장애갯수
	ERRSTACK	CurErrBuff;				// 현재장애내역
	ERRSTACK	FirstErrBuff;			// 최초장애내역
	ERRSTACK	LastErrBuff;			// 최종장애내역

	char	OrignalErrorCode[8];		// V07-01-04-#02:장애발생시 최초 ErrorCode저장
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
	int		BrmChangeLoc;				// Brm정위치변경상태
	int		BrmLoc;						// Brm정위치상태
	int		RejectCSTLoc;				// 리젝트함정위치상태
	int		RetractCSTLoc;               // 회수함
	int		ClerkCSTLoc;				// 보충함정위치상태
	int		FstCSTLoc;					// 1카세트정위치상태(Cd)
	int		SndCSTLoc;					// 2카세트정위치상태(Cd)
	int		TrdCSTLoc;					// 3카세트정위치상태(Cd)
	int		FthCSTLoc;					// Fourth CST Location(Cd)
	int		UcmLoc;						// Ucm정위치상태
	int		DoorStatus;					// DoorStatus상태
	int		DoorStatusSave;				// DoorStatus상태 저장
	int		CabinetStatus;				// CabinetStatus
	int		SaveCabinetStatus;			// CabinetStatus상태 저장

	int		SlipHeadStatus;				// 명세표Cover상태
	int		JnlHeadStatus;				// 저널Cover상태
	int		A4pHeadStatus;				// A4pCover상태
	int		SlipStatus;					// 명세표상태
	int		JnlStatus;					// 저널상태
	int		A4pStatus;					// A4용지상태
	int		GAlarmStatus;				// 알람센서상태
	int		GExternalSensorStatus1;		// 외부센서1
	int		GExternalSensorStatus2;		// 외부센서2
	int		GSensorStatus;				// 충격센서상태
	int		GSensorLelvel;				// 충격센서감도
	int		GBatteryStatus;				// 밧데리센서감도

	int		RejectCSTStatus;			// 회수카세트상태
	int		RetractCSTStatus;			// 회수함정위치상태
	int		ClerkCSTStatus;				// 계원카세트함상태
	int		RecycleBoxStatus;			// 현금상태1
	int		FstCSTStatus;				// 1카세트상태(Cd)
	int		SndCSTStatus;				// 2카세트상태(Cd)
	int		TrdCSTStatus;				// 3카세트상태(Cd)
	int		FthCSTStatus;				// 4카세트상태(Cd)
	int		CheckCSTStatus;				// 출금수표상태
	int		CheckCSTStatusSensor;		// 출금수표상태(실물)
	int		CheckCSTStatusReg;			// 출금수표상태(등록)
	int		CassetteStatus;				// 입금수표상태

	int		ClerkRejectBoxStatus;		// 보충RejectBox상태 V06-01-01-#18
	int		RecycleBox50000Status;		// 현금상태(5만원) V06-01-01-#19

	int		CaptureLastIndex;			// 카메라마지막저장Index
	int		CameraStatus;           	// 카메라마지막저장Index
	
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

	int		RF_IrMD_Flg;				// RF 모바일 구분

	int		TranAvail;					// 전체거래상태
	int		WithAvail;					// 지급거래상태
	int		DepAvail;					// 입금거래상태
	int		PbmAvail;					// 통장거래상태
	int		A4pAvail;					// 지급거래상태
	int		GroAvail;					// 입금거래상태

	int		UserTranAvail;				// User전체거래상태 2003.12.15
	int		UserWithAvail;				// User지급거래상태
	int		UserDepAvail;				// User입금거래상태
	int		UserPbmAvail;				// User통장거래상태
	int		UserA4pAvail;				// User지급거래상태
	int		UserGroAvail;				// User입금거래상태
	int		Cmr1Status;					// 카메라1 상태
	int		Cmr2Status;					// 카메라2 상태
	
	int		HostOpenRetryCnt;			// 개국시도횟수(준비완포함)
	int		bReadyCountFlg;				// 

	int		bEscrowFlg;					// ESCROW처리여부

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
	int		BrmCshInfo;					// BRM CashInfo (현금 투입구에 지폐 유/무)
	int		BrmCsmIn;					// BRM BrmCsmIn (현금 투입구에 지폐 유/무)

	int		OSType;						// WIN7 지원 #N0228

	CStringArray	strMsgArray; //Remote Ctrl
	// U8100 AP 변경내역 #02 - 카메라 동영상 지원
	char	VODCapturePath[128];		
	
	BOOL	VODFaceRecording;
	BOOL	VODHandRecording;
	BOOL	VODRoomRecording;
	CString RealVodFile;				// 적용후 1차 수정 (U8100 AP 변경내역 #13) - 카메라 동영상
	
	BOOL	bVODIdx;

	BOOL	bNearBranchSelectBtn;		// 인근지도파일보기 버튼 선택시		#N0172

	int		Mcu97933Error;				// 97933 장애					// V09-33-00-#01  ICCM 모듈적용
	int		Mcu97934Error;				// 97934 장애					// V09-33-00-#01  ICCM 모듈적용
	int		IsEmvChipPowerOn;			// EMV Chip PowerOn Flag		// V09-33-00-#01  ICCM 모듈적용

struct	tagTimer						// 타임변수영역
{
	int		HostWaitLinkTime;			// 교신재시도타임
	int		HostOpenRetryTime;			// 개국재시도타임
	int		HostAliveTime;
	int		HostRebRetryTime;
	int		HostStatusSendTime;
} Timer;

	EjrTbl			Ejr;				// 전자저널영역
	EjrAmountTbl	EjrAmount;			// 전자저널금액관련자료
	EEjrTblDump		EEjrD;				// 전자저널영역

struct	tagScr							// 화면변수영역
{
	int		CurrentScreenNo;			// 현재화면번호
	int		CurrentLangMode;			// 현재언어모드
	char	MachineFileType[4];            // 기종별(화면이름)
} Scr;

struct	tagOm							// Om영역
{
	// Om정보전달:불특정 위치에서 변하는값 정의
	CString	ProcCount;					// 진행카운터
	CString	ErrorCode;					// 장애코드
	CString	McuDeviceStatus;			// Mcu장치상태(정상,장애)
	CString	McuProcCount;				// Mcu장애진행카운터(정상=' ',' ','0','1','3','5','6','9')
	CString	McuErrorCode;				// Mcu장애코드(정상="0000000",장애="1234567")
	CString	SprDeviceStatus;			// Spr장치상태
	CString	SprProcCount;				// Spr장애진행카운터
	CString	SprErrorCode;				// Spr장애코드
	CString	JprDeviceStatus;			// Jpr장치상태
	CString	JprProcCount;				// Jpr장애진행카운터
	CString	JprErrorCode;				// Jpr장애코드
	CString	PbmDeviceStatus;			// Pbm장치상태
	CString	PbmProcCount;				// Pbm장애진행카운터
	CString	PbmErrorCode;				// Pbm장애코드
	CString	UcmDeviceStatus;			// Ucm장치상태
	CString	UcmProcCount;				// Ucm장애진행카운터
	CString	UcmErrorCode;				// Ucm장애코드
	CString	BrmDeviceStatus;			// Brm장치상태
	CString	BrmProcCount;				// Brm장애진행카운터
	CString	BrmErrorCode;				// Brm장애코드
	CString	NetDeviceStatus;			// Net장치상태
	CString	NetProcCount;				// Net장애진행카운터
	CString	NetErrorCode;				// Net장애코드
	CString	AppDeviceStatus;			// App장치상태
	CString	AppProcCount;				// App장애진행카운터
	CString	AppErrorCode;				// App장애코드
	CString	A4pDeviceStatus;			// A4P장치상태
	CString	A4pProcCount;				// A4P장애진행카운터
	CString A4pErrorCode;				// A4P장애코드
	CString	SendRecvStatus;				// 송수신상태(ON,OFF)
	CString	OperationStatus;			// 조작가능상태(ON,OFF)
	CString	CheckKind;					// 수표권종(10,50,100)
	CString CasetteType;				// 카세트권종선택(만만만,만만천)
	CString Sub_DepKindReg;				// 입금권종제한

	
	// 메인정보(NH2700 - Only )				
	CString	Main_CD_VanIII_Status;				// OM_모드_VANIII_현황
	CString	Main_CD_VanIII_ModeStatus;			// OM_계원모드_VANIII_현황
	CString	Main_CD_VanIII_ApSpEpVersion;			// OM_계원모드_VANIII_현황


	// 메인정보(메인헤더1)				
	CString	Main_1LineStatus;			// OM_메인헤더1
										// 0:ATMSTATUS(INIT,ERROR,CLERK,READY,CUSTOM,TRAN)
										// 1:현금출금상태(가능,불가)
										// 2:현금입금상태(가능,불가)
										// 3:수표출금상태(가능,불가)
										// 4:수표입금상태(가능,불가)
										// 5:통장거래상태(가능,불가)
										// 6:전자화폐상태(가능,불가)
										// 7:해외거래상태(가능,불가)
										// 8:지문거래상태(가능,불가)
										// 9:적외선거래상태(가능,불가)
										// 10:바코드거래상태(가능,불가)
										// 11:배터리충전량(0-100)
										// 12:배터리전원(정상,단전)
										// 13:통신상태(ONLINE,OFFLINE)
										// 14:뒷문상태(닫힘,열림)
										// 15:계원스위치상태(ON,OFF)

	// 메인정보(메인헤더2)
	CString	Main_2LineStatus;			// OM_메인헤더2
										// 0:점번(3자리)
										// 1:기번(2자리)
										// 2:통신종류(IOC,HDLC,SWP,SDLC,TCP/IP,CCL,BAS)
										// 3:APVERSION(V01-01-01)
										// 4:LOCALMODE(ON,OFF)
										// 5:진위권모드(진권,위권)
										// 6:진행카운터(' ','0','1','3','5','6','9')
										// 7:장애코드(1234567)

	// 메인정보(메인현금보유현황)
	CString	Main_CashStatus;			// OM_메인현금보유현황
										// 0:카세트1지폐수
										// 1:카세트2지폐수
										// 2:카세트3지폐수

	// 메인정보(메인잔량정보)
	CString	Main_Status;				// OM_메인잔량정보
										// 0:명세표잔량상태(정상,잔량소,요보충)
										// 1:저널잔량상태(정상,잔량소,요보충)
										// 2:출금수표잔량상태(정상,잔량소,요보충)
										// 3:입금수표잔량상태(정상,잔량만)
										// 4:현금잔량상태(정상,잔량소,요보충,잔량만)

	// 메인정보(메인수표보유현황)
	CString	Main_CheckStatus;			// OM_메인수표보유현황
										// 0:출금수표매수
										// 1:입금수표매수

	// 메인정보(메인현금회수현황)
	CString	Main_CashRejectStatus;		// OM_메인현금회수현황
										// 0:상단회수함매수(분실,잔류)
										// 1:하단회수함매수(출금불량지폐,입금불량지폐,오천원,천원)

	// 메인정보(메인장치현황)
	CString	Main_DeviceStatus;			// OM_메인장치현황
										// 0:Mcu상태(정상,장애)
										// 1:Mcu장애진행카운터(정상='0',' ','0','1','3','5','6','9')
										// 2:Mcu장애코드(정상="0000000",장애="1234567")

										// 3:Spr상태
										// 4:Spr장애진행카운터
										// 5:Spr장애코드
										// 6:Spr현재명세표잔량상태(정상,잔량소,요보충)
										// 7:Spr예비명세표잔량상태(유,무)

										// 8:Jpr상태
										// 9:Jpr장애진행카운터
										// 10:Jpr장애코드
										// 11:Jpr저널1잔량상태(정상,잔량소,요보충)
										// 12:Jpr저널2잔량상태(정상,잔량소,요보충)

										// 13:Pbm상태
										// 14:Pbm장애진행카운터
										// 15:Pbm장애코드

										// 16:Ucm상태
										// 17:Ucm장애진행카운터
										// 18:Ucm장애코드
										// 19:Ucm정위치(정상,탈착)
										// 20:Ucm출금수표잔량상태(정상,잔량소,요보충)
										// 21:Ucm입금수표잔량상태(정상,잔량만)

										// 22:Brm상태
										// 23:Brm장애진행카운터
										// 24:Brm장애코드
										// 25:Brm정위치(정상,탈착)
										// 26:Brm현금카세트상태(정상,잔량소,요보충,잔량만)
										// 27:Brm계원카세트상태(정상,탈착)
										// 28:Brm회수함상태(정상,잔량만,탈착)
										// 29:Brm램프정보

										// 30:Net상태
										// 31:Net장애진행카운터
										// 32:Net장애코드

										// 33:App상태
										// 34:App장애진행카운터
										// 35:App장애코드

	// 메인정보(메인부가정보)	
	CString	Main_ReffInfomation;		// OM_메인부가정보
										// 0:운영,준비
										// 1:송수신상태(ON,OFF)
										// 2:조작가능상태(ON,OFF)
										// 3:수표권종(10,50,100)

	// 출금수표등록	
	CString Sub_WithCheckReg;			// OM_출금수표등록
										// 0:출금수표번호
										// 1:출금수표매수

	// 설치정보
	CString Sub_InstInformation;		// OM_설치정보
										// 0:설치날짜(YYYYMMDD)
										// 1:점번(NNNNNNNN)
										// 2:기번(NNNNNNNN)
										// 3:배치장소(NNNNNNNN)
										// 4:점내(ON,OFF)
										// 5:부점(NNNNNNNN)
										// 6:무인점포(ON,OFF)
										// 7:24시간운영(ON,OFF)
										// 8:365운영(ON,OFF)
										// 9:CE사무소코드(NNNN)
										// 10:기기관리번호(NNNNNNNNNNNN)
										// 11:출장소(NNNNNNNN)
										// 12:설치자(NNNNNNNNNN)
										// 13:설치확인자(NNNNNNNNNN)
										// 14:보수담당자(NNNNNNNNNN)
										// 15:보수담당자연락처(NNNNNNNNNNNNNNNN)

	// 부가장치설정
	CString Sub_SubDeviceReg;			// OM_부가장치설정
										// 0:전자화폐(가능,불가)
										// 1:해외거래(가능,불가)
										// 2:지문거래(가능,불가)
										// 3:적외선거래(가능,불가)
										// 4:바코드거래(가능,불가)

	// Lan설정정보
	CString	Sub_LanInformation;			// OM_LAN설정정보
										// 0:IP주소(NNN.NNN.NNN.NNN)
										// 1:서브넷마스크(NNN.NNN.NNN.NNN)
										// 2:게이트웨이(NNN.NNN.NNN.NNN)
										// 3:BP1IP주소(NNN.NNN.NNN.NNN)
										// 4:BP1PORT(NNNNN)
										// 5:BP2IP주소(NNN.NNN.NNN.NNN)
										// 6:BP2PORT(NNNNN)
										// 7:BP3IP주소(NNN.NNN.NNN.NNN)
										// 8:BP3PORT(NNNNN)

	// 회선정보
	CString	Sub_LineInformation;		// OM_회선정보
										// 0:통신종류(IOC,HDLC,SWP,SDLC,TCP/IP,CCL,BAS)
										// 1:DA(XX)
										// 2:SA(XX)
										// 3:송수신사이즈
										// 4:회선속도(1200,2400,4800,9600,19200)
										// 5:POLLCHECK(ON,OFF)
										// 6:POINT(PTP,MTP)
										// 7:DUPLEX(HALF,FULL)
										// 8:TIMEFILLER(MTF,FTF)
										// 9:RSTYPE(RS-422,RS-232C)
										// 10:NRZ(NRZ,NRZI)
										// 11:CLOCK(INCLK,EXCLK)

	// ATMS Ftp_DownLoad server 설정정보
	CString	Sub_ATMSServerInformation;	// OM_ATMS설정정보
										// 0:SERVER IP주소(NNN.NNN.NNN.NNN)
										// 1:SERVER PORT(NNNNN)
										// 2:예비SERVER1 IP주소(NNN.NNN.NNN.NNN)
										// 3:예비SERVER2 IP주소(NNN.NNN.NNN.NNN)
										// 4:예비SERVER3 IP주소(NNN.NNN.NNN.NNN)

	// 거래정보설정
	CString	Sub_TranInformationReg;		// OM_거래정보설정
										// 0:현금출금(가능,불가)
										// 1:현금입금(가능,불가)
										// 2:수표출금(가능,불가)
										// 3:수표입금(가능,불가)
										// 4:통장거래(가능,불가)

	// 현금보충회수
	CString	Sub_CashRefillTakeup;		// OM_현금보충회수
										// 0:카세트1지폐수
										// 1:카세트2지폐수
										// 2:카세트3지폐수
										// 3:Brm상태(정상,장애)

	// 기기설정
	CString	Sub_MachineType;			// OM_기기설정
										// 0:기기종류
										// 1:명세표부종류

	// 자동계산단위
	CString	Sub_AutoComputeNote;		// OM_자동계산설정		
										// 5:5만원단위			
										// 10:10만원단위		

	CString Sub_CstStateInfo;			// OM_CDP CST INFO		
										// 1:카세트1상태		
										// 2:카세트2상태		
										// 3:카세트3상태		
										// 4:카세트4상태		

	CString	Sub_SerialUpdate;			// OM_거래일련번호변경
										// 0:일련번호

	// T3ATM AP 변경내역 #19 - 카메라 3개의 상태를 모두 표시하기 위함
	CString	Sub_CamerStatus;
	
} Om, OmSave;

	int		IfmDirect;
	int		IfmDirect_Init;
	int		IfmResult;
	
	BYTE	FICEPVersion[32];			// IC 카드 버전

	BYTE	ICSendData[BASEBUFFSIZE];	// IC전송자료
	BYTE	ICRecvData[BASEBUFFSIZE];	// IC수신자료
	BYTE	m_cbFCError[4];				// 업무코드 에러정보
	char	m_szErrCode[10];			// Error Code for Screen
	char	m_szErrMsg[20];				// Error Message for Screen

struct tagKCashInfo						// K-CASH변수영역
{
	BYTE	CardInform[4];				// 카드구분
	BYTE	CardNum[8];					// 카드번호
	char	CardIssueDate[8];			// 카드발급일자(8BYTE)

	BYTE	UserCode;					// 소지자정보   // 01 : 일반인	, 02 : 초등	 , 03 : 중고	, 04 : 대학생
														// 05 : 경로	, 06 : 장애자, 07 : Test

	BYTE	ValidDate[2];				// 유효기간		// YYMM

	BYTE	Account[10];				// 계좌정보

	BYTE	Balance[4];					// 잔액
	BYTE	MaxLimitAmount[4];			// 충전한도금액
	BYTE	MaxLoadUnloadAmount[4];		// 1회출금한도			

	BYTE	EnglishName[24];			// 영어이름
	BYTE	KoreanName[24];				// 한글이름
	BYTE	CitizenNumber[13];			// 실명번호
	
	BYTE	MainAccount[52];			// 주계좌정보

	BYTE	LoadUnloadAlgorithmID;		// 알고리듬ID
	BYTE	LoadUnloadKeyVersion;		// 키버전
	BYTE	LoadUnloadSerialNo[2];		// 일련번호
	BYTE	LoadUnloadRandomNum[8];		// 난수
	BYTE	LoadUnloadEPMakerID[3];		// 발행자ID
	BYTE	LoadUnloadEPID[5];			// ID
	BYTE	LoadUnloadBalance[4];		// 잔액
	BYTE	LoadUnloadAccount[10];		// 계좌정보
	BYTE	LoadUnloadSignature[4];		// 서명

	BYTE	LoadUnloadSignature3[4];	// 서명3

	BYTE	LoadUnloadCnt;				// 충전/환불내역건수
	BYTE	LoadUnloadBuff[16][13];		// 충전/환불내역

	BYTE	BuyListCnt;					// 구매거래내역건수
	BYTE	BuyListBuff[16][28];		// 구매거래내역			2004.02.16

	BYTE	LoadUnloadProof[7];			// Transaction Proof
} KCashInfo;

////////////////////////////////////////////////////////////////////////////////
// IC Use Variables. 20040126-HSW-A
////////////////////////////////////////////////////////////////////////////////
	int		FICCardType;				// IC카드타입(폐쇄형/NATIVE/개방형등)
	int		FICDFType;					// 금융IC DF Type(금융/전자화폐/신용정보)
	BYTE	FICAppletType;				// IC카드 APPLET정보

	int		FICAccountCount;			// 계좌 갯수
	char	FICAccount[50];				// 계좌번호
	char	FICBankCode[10];			// Bank Code

	char	m_EMVszData[IC_DATA_LEN*2 + 1];			// EMV Send RecvData 버퍼

	char	m_szEMVFBCode[8];						// 폴백 거래를 위한 에러 코드 
	int		EmvCardService;							// Fall back 구분자
	int		m_SelectIndex;							// 연속거래시 선택된 

	char	m_btEMVTerminalConstant[1024];			//  FB율 개선
	int		m_nEMVTerminalConstantLen;				//  FB율 개선

//───────────────────────────────────────
// b8	b7	b6	b5	b4	b3	b2	b1		의 미
//───────────────────────────────────────
//								1		금융 IC
//							1			K-Cash
//						1				EMV
//					1					인증서(서명용). 키 분배용은 일반 카드에는 사용되지 않을 것으로 보인다.
//				X						RFU
//			X							RFU
//		X								RFU		
//	1									개방형 카드(0), 폐쇄형 카드(1)						
//───────────────────────────────────────

struct tagICTypeInfo					// IC CHIP TYPE 변수영역
{
	BYTE	ICTypeConfirm;				// 카드구분
	BYTE	ICTypeConfirm2;				// 국민은행 몬덱스/전자통장(현재 국민은행에서만 사용)
} ICTypeInfo;

////////////////////////////////////////////////////////////////////////////////
//  금융 IC Use Structure. 20040126-HSW-A
////////////////////////////////////////////////////////////////////////////////
//  금융 IC Infomation
struct tagFinanceICInfo {
	BYTE	FCI_FInformation[8];		// FCI 정보內 파일정보(8 Bytes)
	BYTE	FCI_AppLabelLen;			// FCI Application Label 길이
	char	FCI_AppLabel[30];			// FCI Application Label
	BYTE	FCI_StandardVersion;		// FCI 표준화 버전
	BYTE	FCI_FileKeyVersion;			// FCI File Key Version
	BYTE	CAN[8];						// Card Application Number
	BYTE	CardHolderNameLen;			// 카드 소지자 이름 길이
	char	CardHolderName[27];			// 카드 소지자 이름
	BYTE	RealNameNOLen;				// 실명번호 길이
	BYTE	RealNameType;				// 실명구분
	char	RealNameNO[15];				// 실명번호
	BYTE	ExpireDayLen;				// 만기일 길이
	char	ExpireDay[10];				// 만기일
	BYTE	AlgorithmCode;				// 알고리즘 식별코드
	BYTE	IssureCodeLen;				// 발행기관 식별코드 길이
	char	IssureCode[6];				// 발행기관 식별코드
	BYTE	CSN[8];						// 카드 일련번호(Card Serial Number)
	char	szCSN[20];					// 카드 일련번호 for Screen
	BYTE	TerminalRandom[16];			// Terminal Random
	BYTE	CardRandom[16];				// Card Random
	BYTE	CipherPW[16];				// 암호화된 계좌 비밀번호
	BYTE	ENCData1[8];				// First ENCData(MyBi)
	BYTE	ENCData2[8];				// Second ENCData(MyBi)
	char	HostTranData[100];			// Host 전송 데이터 포맷
} FinanceICInfo;

//  금융 IC 계좌 Infomation
struct tagFinanceICISO3Info {
	BYTE	AccountState;				// 계좌상태
	char	ISO3[54];					// ISO Track 3
	BYTE	AccountStart;				// 계좌시작위치
	BYTE	AccountNOCnt;				// 계좌자릿수
	BYTE	Reserved[5];				// Reserved Area for Future Use
} FinanceICISO3Info[20];

struct tagFinanceICISO2Info {			// 금융IC ISO2 Track DATA
	BYTE	ISO2[37];					// 카드정보
} FinanceICISO2Info;

//  금융 IC 계좌 Infomation for Screen Display
struct tagFinanceICDisp {
	char	Num[3];						// 계좌 Index
	char	AccNum[17];					// 계좌번호
	char	AccState[13];				// 계좌상태
} FinanceICDisp[10];

	// 전자통장 관련 변수
	int		FICEPBAccountCount;			// 전자통장 계좌 갯수
	int		FICEPBAccountTotalCount;	// 전자통장 총계좌 갯수

// e-모든통장 계좌정보STRUCTURE(요구불,저축,여신계좌)
typedef struct tagEPBACCOUNTINFO {				
	BYTE	AccountState;				// 계좌상태
	BYTE	EPBAccountKind;				// 예금종류, 1:요구불 , 2:저축성, 3:여신
	BYTE	EPBAccountNum[8];			// 계좌번호(8)
	char	ISO3[54];					// ISO Track 3
} FINANCEICEPBINFO;

	FINANCEICEPBINFO EPBAccountInfo[20];

//  전자통장 계좌 Infomation for Screen Display
typedef struct tagFinanceICEPBDisp {
	int		OrgIndex;					// Original Index
	BYTE	EPBAccountKind;				// 예금종류, 1:요구불 , 2:저축성, 3:여신
	int		AccountCnt;					// 총계좌갯수
	char	Num[3];						// 계좌 Index
	BYTE	AccNum[17];					// 계좌번호
	char	AccState[13];				// 계좌상태
} FINANCEICEPBDISP;

	FINANCEICEPBDISP FinanceICEPBDisp[20];
	FINANCEICEPBDISP FinanceICEPBDisp2[20];

////////////////////////////////////////////////////////////////////////////////
//  EMV Use Structure
////////////////////////////////////////////////////////////////////////////////
//  EMV Infomation
struct tagEMVInfo {
	BYTE	ConfigCode;					// 이전 Configuration Code
	BYTE	CAPKRIDIndex;				// EMV CA List RID Index 관리
	BYTE	CAPKKeyIndex;				// EMV CA List Key Set의 Index 관리
	BYTE	AIDListCount;				// 단말기 지원 가능 AID 목록 갯수
	BYTE	CandidateCount;				// EMV 후보 목록 갯수
	BYTE	TranIndex;					// 후보 목록 중 거래로 선택된 Application의 Index
	BYTE	EncryptPIN[8];				// 암호화된 PIN 정보
	char	CertificationType[20];		// 인증서 종류(AAC, TC, ARQC, AAR)
	char	GetDOLData[50];				// EMV 관련 Data Get시 사용
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

//  EMV CAPK List Infomation(CA 공개키 List)
struct tagEMVCAPKListInfo {
	BYTE	Offset;						// Offset
	BYTE	RID[5];						// RID of CA
	BYTE	Index;						// Index Depend on RID
	BYTE	ExponentLen;				// Length of Exponent
	BYTE	Exponent[3];				// Exponent(Max. 3)
	BYTE	ModulusLen;					// Length of Modulus
	BYTE	Modulus[248];				// Modulus(Max. 248)
} EMVCAPKListInfo[7];

//  EMV 지원 가능 AID List Infomation(단말기 지원 가능)
struct tagEMVAIDListInfo {
	BYTE	AIDLen;						// Length of AID
	BYTE	AID[18];					// AID
	char	Brand[40];					// Brand Name
} EMVAIDListInfo[15];				

//  EMV Candidate List Infomation(카드 및 단말기 상호지원으로 Display용)
struct tagEMVCandidateList {
	char	Index[3];					// Index(Ex : " 1")
	BYTE	API;						// Application Priority Indicator
	BYTE	AIDLen;						// Length of AID
	BYTE	AID[16];					// AID
	char	Brand[40];					// Brand Name
} EMVCandidateList[9];

////////////////////////////////////////////////////////////////////////////////
//  적외선(한창/SKT) 데이타 
////////////////////////////////////////////////////////////////////////////////
struct tagIRDAInfo1						// CD/ATM 정보 : type I
{
	BYTE		IRBankCode[4];			// 은행코드								ASCII
	WORD		IRChipLenInfo;			// Chip 길이 정보						HEX
	BYTE		IRAccount[16];			// 계좌정보								ASCII
	BYTE		IRCSN[16];				// 카드 일련번호(Card Serial Number)	ASCII
	BYTE		IRSubjectCode[2];		// 과목코드								ASCII
	BYTE		IRTagInfo;				// Tag 정보								HEX
	BYTE		IRSomeInfo[8];			// 계좌자릿수							ASCII
} IRDAInfo1;

struct tagIRDAInfo {					// CD/ATM 정보 : type II
	BYTE	ISODataTag;					// ISO Data Tag
	BYTE	IrdaType2Mode;				// Harex통합 : Type2(M-Bank) 동작모드
	BYTE	FCI[40];					// FCI정보
	BYTE	CAN[8];						// Card Application Number
	BYTE	CardHolderNameLen;			// 카드 소지자 이름 길이
	char	CardHolderName[27];			// 카드 소지자 이름
	BYTE	RealNameNOLen;				// 실명번호 길이
	BYTE	RealNameType;				// 실명구분
	char	RealNameNO[15];				// 실명번호
	BYTE	ExpireDayLen;				// 만기일 길이
	char	ExpireDay[10];				// 만기일
	BYTE	AlgorithmCode;				// 알고리즘 식별코드
	BYTE	IssureCodeLen;				// 발행기관 식별코드 길이
	char	IssureCode[6];				// 발행기관 식별코드
	BYTE	CSN[8];						// 카드 일련번호(Card Serial Number)
	char	szCSN[20];					// 카드 일련번호 for Screen
	BYTE	TerminalRandom[16];			// Terminal Random
	BYTE	CardRandom[16];				// Card Random
	BYTE	CipherPW[16];				// 암호화된 계좌 비밀번호
} IRDAInfo;

struct tagIRDAInfo2						// IRDA Infomation
{
	BYTE	ISODataTag;					// ISO Data Tag
	BYTE	TrInfo;						// 2Track, 3Track, Mobile Identification Number(MIN)존재여부
	WORD	IRISO2size;	 				// 2Track Data Size
	MCISO2	IRISO2;						// 2Track Data
	BYTE	IRBankCode[4];				// 은행코드								ASCII
	WORD	IRChipLenInfo;				// Chip 길이 정보						HEX
	BYTE	IRAccount[16];				// 계좌정보								ASCII
	BYTE	IRCSN[16];					// 카드 일련번호(Card Serial Number)	ASCII
	BYTE	IRSubjectCode[2];			// 과목코드								ASCII
	BYTE	IRTagInfo;					// Tag 정보								HEX
	BYTE	IRSomeInfo[8];				// 계좌자릿수							ASCII
} IRDAInfo2;

struct tagIRDAISO3Info {				//  IRDA 계좌 Infomation
	BYTE	AccountState;				// 계좌상태
	char	ISO3[54];					// ISO Track 3
	BYTE	AccountStart;				// 계좌시작위치
	BYTE	AccountNOCnt;				// 계좌자릿수
	BYTE	Reserved[5];				// Reserved Area for Future Use
} IRDAISO3Info;

struct tagIRDAISO2Info {				// IRDA ISO2 Track DATA
	BYTE	ISO2[37];					// 카드정보
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

struct tagAtmTranNetInfo				// 기기개별망 설정 정보		2012.02.28
{
	int		bUseEachNet;				// 기기개별망 사용여부Flag
	CString	strCheckStartTime;			// 기기개별망 시간확인 시작시간
	CString	strCheckEndTime;			// 기기개별망 시간확인 종료시간
	int		bReadyEachNet;				// 확인시간내 재시작여부Flag
} AtmTranNetInfo;


struct tagAtmRestartInfo				// 기기재시작 정보		2005.02.02
{
	int		bUseRestart;				// 기기재시작 사용여부Flag
	CString	strCheckStartTime;			// 재시작 시간확인 시작시간
	CString	strCheckEndTime;			// 재시작 시간확인 종료시간
	int		bReadyRestart;				// 확인시간내 재시작여부Flag
} AtmRestartInfo;
	
	int		SplUpdateNow;				// SPL 상태를 정기적으로 Update함을 Control
	int		m_DisplayJamInfo;			// Brm Jam Information Display
	int		ToggleDisp;					// 현금매수/회수매수 등 SPL화면에 DISPLAY 토글변수 
	bool	bIsBRMGetSensorOk;			// Get Sensor Action Result from BRM; false-> Sensor Read Error  //010003
	int		JamScreenNoSave[17];		// PJH 1231

////////////////////////////////////////////////////////////////////////////////
//  RFD 데이타 
////////////////////////////////////////////////////////////////////////////////
	BYTE	RFSendData[BASEBUFFSIZE];	// RF전송자료
	BYTE	RFRecvData[BASEBUFFSIZE];	// RF수신자료
	int		RFDAnalError;				// RF분석오류
	BYTE	RFDStatus[8];				// RF상태

struct tagRFDBasicInfo
{
	BYTE	CardNo[10];					// 카드번호
	BYTE	CardRID[10];				// 카드RID
	BYTE	CardState[2];				// 카드상태
	BYTE	TranCount[10];				// 거래카운트
	BYTE	CardID[10];					// 카드단말ID
	BYTE	CardRemain[8];				// 카드잔액
	BYTE	ValidDate[8];				// 카드유효기간
	BYTE	LastLoadDate[8];			// 최종보충일자
	BYTE	CardType[2];				// 카드종류
	BYTE	MAC[4];						// MAC
	BYTE	TranNo[20];					// 센터처리번호
} RFDBasicInfo;

struct tagRFDCharge
{
	BYTE	CardNo[10];					// 카드번호
	BYTE	ChargeMoney[8];				// 보충금액
	BYTE	PreChargeMoney[8];			// 보충전금액
	BYTE	CardRemain[8];				// 보충후잔액
	BYTE	CardRID[10];				// 카드RID
	BYTE	CardType[2];				// 카드종류
	BYTE	TranCount[10];				// 거래카운트
	BYTE	MacID[4];					// MAC
} RFDCharge;

struct tagRFDDrawBack
{
	BYTE	CardNo[10];					// 카드번호
	BYTE	PreBackMoney[8];			// 환불전잔액
	BYTE	PostBackMoney[8];			// 환불후잔액
	BYTE	CardRID[10];				// 카드RID
	BYTE	CardType[2];				// 카드종류
	BYTE	TranCount[10];				// 거래카운트
	BYTE	MacID[4];					// MAC
} RFDDrawBack;

//  장애인ATM Start
//////////////////////////////////////////////////////////////////////
//	화면확대
	int		SightHandiCapped;			// 저시력자 - 화면확대 가능여부
	int		EarPhoneInserted;			// EarPhone 삽입여부

//TTS 음성관련
	char	*m_pSpeech;
	int		m_nSpeechNumBytes;
	char	m_pText[1000];
	void    *pTTS;
	int		m_playDoing;

	char	PdisableBtn[13];			// 음성 부분삭제
	int		BackDispCnt;

typedef	struct	tagBackDisp				// 화면변수영역
{
	int		Index;						// 현재화면번호
	char	DispData[256];				// 현재언어모드
} BackDisp;
BackDisp	PbackDisp[30];

// 장애인ATM End
//	int		OSType;						//  Win7 구분을 위해 OS종류저장

	BOOL	IsFallBackTran;				// Fallback 거래 여부				V09-33-00-#01 ICCM 모듈적용 
	int		GenerateAC;					// AC 생성							V09-33-00-#01 ICCM 모듈적용 
	BOOL	Declined ;					// ERROR 미존재 시 처리에 DECLINE 처리추가	V09-33-00-#01 ICCM 모듈적용 

//////////////////////////////////////////////////////////////////////


	int		ProximitySensor;			// 근접센서
	DWORD	m_PSoundTime;				// 근접센서 시간

	BOOL	PBBarLineReading;			// 통장 바코드 
	BOOL	MchipFlag;		
	
	int nEmvStep;
	int nEmvStepCode;

	
// Hi-Pass충전업무개발 #N0266
struct tagRFHipassBalance {
	BYTE	Balance[8];					// 전자지갑 잔액						ACSII (HEX)
	BYTE	CardNum[16];				// 카드번호								BCD
	BYTE	RespCode[4];				// 카드 응답코드						ASCII (HEX)
} RFHipassBalance;

// Hi-Pass접촉식충전개발 ────────────────────#N0266
// LLOG Data
struct tagHiPassLLog					// HiPass
{
	BYTE	PPCardTranType;				// 거래유형
	BYTE	PPCardTranSeqNum[4];		// 거래일련번호
	BYTE	PPCardTranBalance[4];		// 거래후 잔액
	BYTE	PPCardTranAmount[4];		// 거래금액
	BYTE	PPCardCompleteCode[2];		// 완료코드
	BYTE	PPCardLoadOrg;				// 충전기관
	BYTE	PPCardBranchNum[2];			// 영업소번호
	BYTE	PPCardTerminalID;			// 단말기ID
	BYTE	PPCardLoadDate[4];			// 충전일자
	BYTE	PPCardLoadTime[3];			// 충전시간
	BYTE	EPMakerID[3];				// 발행자ID
	BYTE	LSAMID[5];					// LSAM ID
} HiPassLLog;
// ───────

struct tagRFHipassLoad1 {
	BYTE	CardNum[16];				// 카드번호								BCD
	BYTE	AlgorithmID[2];				// 알고리즘ID							ASCII (HEX)
	BYTE	KeyVersion[2];				// 키 버전								ASCII (HEX)
	BYTE	CardTranSeq[8];				// 카드거래 일련번호					ASCII (HEX)
	BYTE	RandomNum[16];				// 난수									ASCII (HEX)
	BYTE	PublisherID[6];				// 발행사ID								ASCII (HEX)
	BYTE	CardID[10];					// 선불카드ID							ASCII (HEX)
	BYTE	CardBalance[8];			// 선불카드 잔액					ASCII (HEX)
	BYTE	LedgerNum[20];				// 원장번호  							ASCII (HEX)
	BYTE	UniqueNum[2];				// 고유식별번호							ASCII (HEX)
	BYTE	CardServiceID[6];			// 카드서비스ID		 					ASCII (HEX)
	BYTE	SIGN1[8];					// SIGN1								ASCII (HEX)
	BYTE	RespCode[4];				// 카드 응답코드						ASCII (HEX)
	BYTE	CardBalanceHEX[8];				// HEX
} RFHipassLoad1;

struct tagRFHipassLoad2 {
	BYTE	CardNum[16];				// 카드번호								BCD
	BYTE	Balance[8];					// 전자지갑 잔액						ACSII (HEX)
	BYTE	SIGN3[8];					// 선불카드에서 계산한 서명 (SIGN3)		ASCII (HEX)
	BYTE	RespCode[4];				// 카드 응답코드						ASCII (HEX)
} RFHipassLoad2;

struct tagRFHipassUnload1 {
	BYTE	CardNum[16];				// 카드번호								BCD
	BYTE	PreLoadAmount[8];			// 이전충전금액							ASCII (HEX)
	BYTE	TranType[2];				// 거래유형								ASCII (HEX)
	BYTE	AlgorithmID[2];				// 알고리즘ID							ASCII (HEX)
	BYTE	KeyVersion[2];				// 키 버전								ASCII (HEX)
	BYTE	CardTranSeq[8];				// 카드거래 일련번호					ASCII (HEX)
	BYTE	RandomNum[16];				// 난수									ASCII (HEX)
	BYTE	PublisherID[6];				// 발행사ID								ASCII (HEX)
	BYTE	CardID[10];					// 선불카드ID							ASCII (HEX)
	BYTE	CardBalance[8];				// 선불카드 잔액						ASCII (HEX)
	BYTE	LedgerNum[20];				// 원장번호  							ASCII (HEX)
	BYTE	UniqueNum[2];				// 고유식별번호							ASCII (HEX)
	BYTE	CardServiceID[6];			// 카드서비스ID		 					ASCII (HEX)
	BYTE	SIGN1[8];					// SIGN1								ASCII (HEX)
	BYTE	RespCode[4];				// 카드 응답코드						ASCII (HEX)
} RFHipassUnload1;

struct tagRFHipassUnload2 {
	BYTE	SIGN3[8];					// 선불카드에서 계산한 서명 (SIGN3)		ASCII (HEX)
	BYTE	RespCode[4];				// 카드 응답코드						ASCII (HEX)
} RFHipassUnload2;

char RFVersion[16];
char RFHipassErrorCode[5];
// ──────────────────────────────────────
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
//	함수군
/////////////////////////////////////////////////////////////////////////////
//	return 규칙1:정상진행=TRUE, 장애또는다른목적=FALSE(DEVICE는 제외)
//			   2:모든 RETURN(int)은 TRUE, FALSE로 한다
//			   3:TRUE!=0, FALSE=0(값구하기는 제외)
//			   4:CString추가									2003.08.23
/////////////////////////////////////////////////////////////////////////////
	void	SetOwner(CWinAtmCtrl* pOwner, CNHMWI* pMwi, CNHAlert4SPackX* pNHAlertSpack, CNHICCM* pIccm, CNHMwiEmv* pMwiEmv);			
													// 포인터설정및초기화

	void ENCRYPT(unsigned char *encodedtext, const unsigned char *plaintext, int textlen, unsigned char *deskey);
	void DECRYPT(unsigned char *plaintext, const unsigned char *encodedtext, int textlen, unsigned char *deskey);
	void T_DES_ENCRYPTOR(BYTE* byData, USHORT usDataLen, BYTE* byKey1, BYTE* byKey2, BYTE* byKey3, BYTE* byResult);
	void T_DES_DECRYPTOR(BYTE* byData, USHORT usDataLen, BYTE* byKey1, BYTE* byKey2, BYTE* byKey3, BYTE* byResult);
/////////////////////////////////////////////////////////////////////////////
//	저널부함수(JPR)
/////////////////////////////////////////////////////////////////////////////
// JPR SET MODE
	int		fnJPR_ClearErrorCode();					// 장애코드초기화

// JPR GET MODE
	CString	fstrJPR_GetErrorCode();					// 장애코드구하기
	CString	fstrJPR_GetSensorInfo();				// 장치센서구하기
	int		fnJPR_GetDeviceStatus();				// 장치상태구하기

	int		fnJPR_GetPaperStatus();					// 용지상태구하기
	int		fnJPR_GetMaterialInfo();				// 매체잔류정보구하기

// JPR OPERATION MODE
	int		fnJPR_Initialize();						// 연결시작및초기화
	int		fnJPR_Deinitialize();					// 연결종료

	int		fnJPR_EmbossPrint();					// 엠보스인자
	int		fnJPR_Print(LPCTSTR szPrintData);		// 인자(인자자료)

/////////////////////////////////////////////////////////////////////////////
//	명세표부함수(SPR)
/////////////////////////////////////////////////////////////////////////////
// SPR SET MODE
	int		fnSPR_ClearErrorCode();					// 장애코드초기화
	int		fnSPR_ClearRetractCnt();				// 회수매수초기화

// SPR GET MODE
	CString	fstrSPR_GetErrorCode();					// 장애코드구하기
	CString	fstrSPR_GetSensorInfo();				// 장치센서구하기
	int		fnSPR_GetDeviceStatus();				// 장치상태구하기

	int		fnSPR_GetPaperStatus();					// 용지상태구하기
	int		fnSPR_GetMaterialInfo();				// 매체잔류정보구하기

	CString	fstrSPR_GetRetractCnt();				// 회수매수구하기

// SPR OPERATION MODE
	int		fnSPR_Initialize();						// 연결시작및초기화
	int		fnSPR_Deinitialize();					// 연결종료

	int		fnSPR_Print(LPCTSTR szPrintData);		// 인자(인자자료)
	int		fnSPR_Eject(int nWaitSec = K_45_WAIT);	// 방출(수취대기시간)
	int		fnSPR_EjectBoth(int nWaitSec = K_45_WAIT);	// 카드/명세표 동시방출(수취대기시간) V07-05-01-#02
	int		fnSPR_WaitTaken();						// 수취대기
	int		fnSPR_CancelWaitTaken();				// 수취대기취소
	int		fnSPR_Retract();						// 회수
	int		fnSPR_Autoload();						// 오토로드
	int		fnSPR_GetAutoloadCondition();			// 오토로드조건확인
	int		fnSPR_GetEjectBothAvail();				// 
/////////////////////////////////////////////////////////////////////////////
//	카드부함수(MCU)
/////////////////////////////////////////////////////////////////////////////
// MCU SET MODE
	int		fnMCU_ClearErrorCode();					// 장애코드초기화
	int		fnMCU_ClearRetractCnt();				// 회수매수초기화

// MCU GET MODE
	CString	fstrMCU_GetErrorCode();					// 장애코드구하기
	CString	fstrMCU_GetSensorInfo();				// 장치센서구하기
	int		fnMCU_GetDeviceStatus();				// 장치상태구하기

	int		fnMCU_GetMaterialInfo(int nType = 0);				// 매체잔류정보구하기

	CString	fstrMCU_GetRetractCnt();				// 회수매수구하기
	CString	fstrMCU_GetCardData();					// 카드자료구하기
	CString	fstrMCU_GetICData();					// IC자료구하기
	int		fnMCU_GetICVersion();					// IC Board Version 구하기 V070300 3.

// MCU OPERATION MODE
	int		fnMCU_Initialize();						// 연결시작및초기화
	int		fnMCU_Deinitialize();					// 연결종료
	int		fnMCU_ICCMOpenConnection();				// ICCM 연결시작
	int		fnMCU_ICCMInitialize();					// ICCM 초기화
	int		fnMCU_ICCMDeinitialize();				// ICCM 연결종료


	int		fnMCU_EntryEnable(int nWaitSec = K_WAIT);	
													// 투입허가(투입대기시간)
	int		fnMCU_EntryDisable();					// 투입금지
	int		fnMCU_Read();							// 읽기	
	int		fnMCU_Eject(int nWaitSec = K_45_WAIT);	// 방출(수취대기시간)
	int		fnMCU_EjectBoth(int nWaitSec = K_45_WAIT);	// 카드/명세표 동시방출(수취대기시간)	V07-05-01-#02	
	int		fnMCU_WaitTaken();						// 수취대기
	int		fnMCU_CancelWaitTaken();				// 수취대기취소
	int		fnMCU_Retract();						// 회수
	int		fnMCU_Write(LPCTSTR szTrackData);		// 기록(카드자료3TR)

	int		fnMCU_ICChipInitialize();				// IC칩초기화:접점이동
	int		fnMCU_ICEntryEnable(int nWaitSec = K_WAIT);	
													// IC투입허가(투입대기시간)
	int		fnMCU_ICEntryDisable();					// IC투입금지
	int		fnMCU_ICSendData(int nProtocolId, LPCTSTR szICSendData, int nWaitSec = K_WAIT);	
													// IC자료전송(통신방식, 송신자료, 전송대기시간)
	int		fnMCU_ICCMSendData(int nProtocolId, short nCmdType, LPCTSTR szICSendData = "", int nWaitSec = MID_ICCMRCV_TIME);	

	/////////////////////////////////////////////////////////////////////////////
	//	EMV Library
	// V09-33-00-#01 ICCM 모듈적용
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
//	수표입출금부함수(UCM)
/////////////////////////////////////////////////////////////////////////////
// UCM SET MODE
	int		fnUCM_ClearErrorCode();					// 장애코드초기화

// UCM GET MODE
	CString	fstrUCM_GetErrorCode();					// 장애코드구하기
	CString	fstrUCM_GetSensorInfo();				// 장치센서구하기
	int		fnUCM_GetDeviceStatus();				// 장치상태구하기

	int		fnUCM_GetPosition();					// 정위치구하기
	int		fnUCM_GetDepCSTStatus();				// 입금카세트상태구하기
	int		fnUCM_GetWithCSTStatus();				// 출금카세트상태구하기
	int		fnUCM_GetMaterialInfo(int nDepWithFlag);// 매체잔류정보구하기(입출금구분)
	int		fnUCM_GetAvailDeposit();				// 입금가능상태구하기
	int		fnUCM_GetAvailWithdraw();				// 출금가능상태구하기

	CString	fstrUCM_GetACheckData();				// 낱장수표자료구하기
	CString	fstrUCM_GetCheckDataDispensed(int nIndex = -1);				
													// 출금계수한수표자료구하기(인덱스)

// UCM OPERATION MODE
	int		fnUCM_Initialize();						// 연결시작및초기화
	int		fnUCM_Deinitialize();					// 연결종료

	int		fnUCM_OpenShutter();					// 열기
	int		fnUCM_CloseShutter();					// 닫기

	int		fnUCM_Accept(int nWaitSec = K_WAIT);	// 투입허가(투입대기시간)및열기
	int		fnUCM_CancelAccept();					// 투입금지및닫기
	int		fnUCM_Read();							// 읽기
	int		fnUCM_Escrow(LPCTSTR szPrintData);		// 임시보관(입금배서자료)
	int		fnUCM_Stack();							// 수납
	int		fnUCM_RejectA(int nWaitSec = K_45_WAIT);// 낱장수표반환(수취대기시간)
	int		fnUCM_RejectAll(int nWaitSec = K_45_WAIT);			
													// 전체반환(수취대기시간)
	int		fnUCM_WaitTaken();						// 수취대기
	int		fnUCM_CancelWaitTaken();				// 수취대기취소
	int		fnUCM_Retract(int nDepWithFlag);		// 회수(입출금구분)

	int		fnUCM_Dispense(int nCheckCnt, LPCTSTR szPrintData);
													// 출금계수(수표매수, 출금배서자료)
	int		fnUCM_Present(int nWaitSec = K_45_WAIT);// 방출(수취대기시간)

/////////////////////////////////////////////////////////////////////////////
//	현금입출금부함수(BRM)
/////////////////////////////////////////////////////////////////////////////
// BRM SET MODE
	int		fnBRM_ClearErrorCode();					// 장애코드초기화
	int		fnBRM_SetCarrySpeed(BOOL bSpeed);		// BRM Speed 설정		// T2ATM 2006.5.24 yscho
	int		fnBRM_SetNoteInfo(short nNoteKind1, short nNoteKind2, short nNoteKind3, short nNoteKind4);

// BRM GET MODE
	CString	fstrBRM_GetErrorCode();					// 장애코드구하기
	CString	fstrBRM_GetSensorInfo();				// 장치센서구하기
	int		fnBRM_GetDeviceStatus();				// 장치상태구하기

	int		fnBRM_GetPosition();					// 정위치구하기
	int		fnBRM_GetChangeUnitLock();				// 탈착정보구하기
	int		fnBRM_GetClerkCSTStatus();				// 계원카세트상태구하기
	int		fnBRM_GetRejectCSTStatus();				// 회수카세트상태구하기
	int		fnBRM_GetRecycleBoxStatus();			// 현금카세트상태구하기
	int		fnBRM_GetRecycleBox50000Status();		// 현금카세트상태구하기(5만)	// V06-01-01-#19
	int		fnBRM_GetNumberOfCash();				// 현금매수구하기
	int		fnBRM_GetNumberOf50000Cash();			// 현금매수구하기(5만원권)		
	int		fnBRM_GetClerkRJBStatus();				// 보충Reject상태구하기			// V06-01-01-#18 
	int		fnBRM_GetNumberOfCash(int nCSTNo);		// 해당 카세트 현금매수 구하기	
	int		fnBRM_GetAutoloadCondition();			// 자동보충및자동회수정보구하기
	int		fnBRM_GetMaterialInfo();				// 매체잔류정보구하기
	int		fnBRM_GetAvailDeposit();				// 입금가능상태구하기
	int		fnBRM_GetAvailWithdraw();				// 출금가능상태구하기

	int		fnBRM_GetAcceptCountStatus();			// 투입및입금계수상태
	CString	fstrBRM_GetCountResult();				// 입금계수결과구하기
	CString	fstrBRM_GetRetractResult();				// 회수결과구하기(분실)
	CString	fstrBRM_GetRefillResult();				// 보충결과구하기
	CString	fstrBRM_GetTakeupResult();				// 회수결과구하기
	CString	fstrBRM_GetBCDistinctionMode();			// 진/위권 모드 조회	// T2ATM 2006.5.24 yscho
	// T2ATM 지원 2006.07.29 mwseo --------------------------------------------
	int		fnBRM_GetCSMNoteStatus();				// for T2ATM : 현금 투입전 지폐 상태 확인 2006.07.31
	int		fnBRM_Get10000NoteUsePriority();		// for T2ATM : 구만/신만 우선 순위 조회 2006.07.31
	int		fnBRM_GetEnableDepositNote();			// for T2ATM : 입금 가능 권종 조회 2006.07.31
	int		fnBRM_GetNoteKindOfRB1();				// for T2ATM : 권종 조회(RB1) 2006.07.31
	int		fnBRM_GetNoteKindOfRB2();				// for T2ATM : 권종 조회(RB2) 2006.07.31
	int		fnBRM_GetNoteKindOfRB3();				// for T2ATM : 권종 조회(RB3) 2006.07.31
	int		fnBRM_GetNoteKindOfRB4();				// for T2ATM : 권종 조회(RB4) 2006.07.31
	//-------------------------------------------------------------------------
	
// BRM OPERATION MODE
	int		fnBRM_Initialize();						// 연결시작및초기화
	int		fnBRM_Deinitialize();					// 연결종료

	int		fnBRM_Reset(BOOL bEnforce);				// 장치리셋				// T2ATM 2006.5.24 yscho

	int		fnBRM_OpenShutter();					// 열기
	int		fnBRM_CloseShutter();					// 닫기

	int		fnBRM_Ready();							// 입출금준비해제및객교대복구

	int		fnBRM_ReadyCount();						// 입금준비
	int		fnBRM_Accept(int nCashCnt);				// 투입(입금가능현금매수)
	int		fnBRM_AcceptLarge(int nCashCnt, int nAmount);		
	int		fnBRM_Count();							// 입금계수
	int		fnBRM_Stack();							// 수납
	int		fnBRM_Reject(int nWaitSec = K_45_WAIT);	// 반환(수취대기시간)
	int		fnBRM_WaitLift();						// 수취전동작대기
	int		fnBRM_WaitTaken();						// 수취대기
	int		fnBRM_CancelWaitTaken();				// 수취대기취소
	int		fnBRM_Retract();						// 회수
	int		fnBRM_Present2(int nWaitSec = K_45_WAIT);			
													// 방출(수취대기시간):닫기없음

	int		fnBRM_ReadyDispense();					// 출금준비
	int		fnBRM_Dispense(int nCashCnt);			// 출금계수(현금매수)
	int		fnBRM_DispenseLarge(int nCash100TCNT, int nCash50TCNT, int nCash10TCNT);	
	int		fnBRM_Present(int nWaitSec = K_45_WAIT);// 방출(수취대기시간)

	int		fnBRM_RefillMoney(int nCashCnt);		// 보충(현금매수)
	int		fnBRM_TakeupMoney(int nCashCnt);		// 회수(현금매수)
	int		fnBRM_GetLargeMoneySupport();			// 감별부 H/W 및 F/W 변경유무 06-01-02-#08

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
	int		fnCSH_GetRecycleBox50000Status();		// 현금카세트상태구하기(5만)	// V06-01-01-#19
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
													// Accept(Avail Deposit Count, Deposit Amount)	V07-00-00-#01 오만원권 지원
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
//	통장부함수(PBM)
/////////////////////////////////////////////////////////////////////////////
// PBM SET MODE
	int		fnPBM_ClearErrorCode();					// 장애코드초기화
	int		fnPBM_ClearRetractCnt();				// 회수매수초기화

// PBM GET MODE
	CString	fstrPBM_GetErrorCode();					// 장애코드구하기
	CString	fstrPBM_GetSensorInfo();				// 장치센서구하기
	int		fnPBM_GetDeviceStatus();				// 장치상태구하기

	int		fnPBM_GetMaterialInfo();				// 매체잔류정보구하기
	
	CString	fstrPBM_GetRetractCnt();				// 회수매수구하기
	CString	fstrPBM_GetMsData();					// 통장Ms자료구하기
	CString	fstrPBM_GetBarData();					// 통장Bar자료구하기
	CString	fstrPBM_GetLineData();					// 통장Line자료구하기

// PBM OPERATION MODE
	int		fnPBM_Initialize();						// 연결시작및초기화
	int		fnPBM_Deinitialize();					// 연결종료

	int		fnPBM_EntryEnable(int nWaitSec = K_WAIT);	
													// 투입허가(투입대기시간)
	int		fnPBM_EntryDisable();					// 투입금지
	int		fnPBM_Read();							// 통장Ms읽기	
	int		fnPBM_BarLineRead();					// 통장BarLine읽기	
	int		fnPBM_Print(LPCTSTR szPrintData);		// 인자(인자자료)
	int		fnPBM_Eject(int nWaitSec = K_45_WAIT);	// 방출(수취대기시간)
	int		fnPBM_WaitTaken();						// 수취대기
	int		fnPBM_CancelWaitTaken();				// 수취대기취소
	int		fnPBM_Retract();						// 회수
	int		fnPBM_MSWrite(LPCTSTR szMSData);		// MS Write
	CString	fnPBM_GetInkStatus();					// U8100 AP 변경내역 #07 - IPBM 소모품 잔여정보 지원
/////////////////////////////////////////////////////////////////////////////
//	A4P 프린터 함수
/////////////////////////////////////////////////////////////////////////////
// SPR SET MODE
	int		fnA4P_ClearErrorCode();					// 장애코드초기화
	int		fnA4P_ClearRetractCnt();				// 회수매수초기화

// SPR GET MODE
	CString	fstrA4P_GetErrorCode();					// 장애코드구하기
	CString	fstrA4P_GetSensorInfo();				// 장치센서구하기
	int		fnA4P_GetDeviceStatus();				// 장치상태구하기

	int		fnA4P_GetPaperStatus();					// 용지상태구하기
	int		fnA4P_GetMaterialInfo();				// 매체잔류정보구하기

	CString	fstrA4P_GetRetractCnt();				// 회수매수구하기

// SPR OPERATION MODE
	int		fnA4P_Initialize();						// 연결시작및초기화
	int		fnA4P_Deinitialize();					// 연결종료

	int		fnA4P_Print(LPCTSTR szPrintForm, LPCTSTR szPrintData);		
													// 인자(인자자료)
	int		fnA4P_PrintAndEject(LPCTSTR szPrintForm, LPCTSTR szPrintData, int nWaitSec = K_65_WAIT);		
													// 인자와방출(인자자료)	2007.06.13(水)
	int		fnA4P_Eject(int nWaitSec = K_45_WAIT);	// 방출(수취대기시간)
	int		fnA4P_WaitTaken();						// 수취대기
	int		fnA4P_CancelWaitTaken();				// 수취대기취소
	int		fnA4P_Retract();						// 회수

/////////////////////////////////////////////////////////////////////////////
//	CAMERA함수(CMR)
/////////////////////////////////////////////////////////////////////////////
// CAMERA SET MODE
	int		fnCMR_ClearErrorCode();					// 장애코드초기화

// CAMERA GET MODE
	CString	fstrCMR_GetErrorCode();					// 장애코드구하기
	int		fnCMR_GetDeviceStatus();				// 장치상태구하기
	int		fnCMR_GetFaceCameraDeviceStatus();		// 얼굴부카메라 장치상태구하기 V03.01.01
	int		fnCMR_GetHandCameraDeviceStatus();		// 수취구카메라 장치상태구하기 V03.01.01

// T3ATM AP 변경내역 #19 - OM에 Room Camera의 상태를 나타내기 위해 필요함
	int		fnCMR_GetRoomCameraDeviceStatus();		

// CAMERA OPERATION MODE
	int		fnCMR_Initialize();						// 연결시작및초기화
	int		fnCMR_Deinitialize();					// 연결종료

	int		fnCMR_CaptureFace(LPCTSTR szFileName);	// 얼굴촬영(파일이름)
	int		fnCMR_CaptureHand(LPCTSTR szFileName);	// 손촬영(파일이름)

	
	int		fnCMR_CaptureRoom(LPCTSTR szFileName);	// U8100 AP 변경내역 #01 - 3Camera, Preview지원 후면카메라촬영
	
	int		fnCMR_PreviewToAD(BOOL bShow);			// LCD에 카메라 프리뷰처리
	int		fnCMR_PreviewToFront(BOOL bShow);		// OPL에 카메라 프리뷰처리

	int		fnCMR_PreviewToOM(BOOL bShow);			// OM에 카메라 프리뷰처리


/////////////////////////////////////////////////////////////////////////////
//	DOOR함수(DOR)
/////////////////////////////////////////////////////////////////////////////
// DOOR SET MODE
	int		fnDOR_ClearErrorCode();					// 장애코드초기화

// DOOR GET MODE
	CString	fstrDOR_GetErrorCode();					// 장애코드구하기
	int		fnDOR_GetDeviceStatus();				// 장치상태구하기

	int		fnDOR_GetDoorStatus();					// 뒷문상태구하기
	int		fnDOR_GetCabinetStatus();				// 뒷문상태구하기(TTW:Cabinet Type)

// DOOR OPERATION MODE
	int		fnDOR_Initialize();						// 연결시작및초기화
	int		fnDOR_Deinitialize();					// 연결종료

/////////////////////////////////////////////////////////////////////////////
//	LIGHT함수(LGT)
/////////////////////////////////////////////////////////////////////////////
// LIGHT SET MODE
	int		fnLGT_ClearErrorCode();					// 장애코드초기화

	int		fnLGT_SetFlicker(int nIndex, int nValue);					
													// Flicker설정(인덱스,값)
	int		fnLGT_SetIndicator(int nValue);			// Indicator설정(값)

// LIGHT GET MODE
	CString	fstrLGT_GetErrorCode();					// 장애코드구하기
	int		fnLGT_GetDeviceStatus();				// 장치상태구하기

// LIGHT OPERATION MODE
	int		fnLGT_Initialize();						// 연결시작및초기화
	int		fnLGT_Deinitialize();					// 연결종료

/////////////////////////////////////////////////////////////////////////////
//	SPL함수(SPL)
/////////////////////////////////////////////////////////////////////////////
// SPL SET MODE
	int		fnSPL_ClearErrorCode();					// 장애코드초기화

	int		fnSPL_SetSegment(LPCTSTR szValue);		// Segment설정(값)
	int		fnSPL_SetLed(LPCTSTR szValue);			// Led설정(값)
	int		fnSPL_SetLedOnOff(int nLedIndex, int bOnOff);
													// Led설정(인덱스, 값) V03-01-01
	int		fnSPL_SetRpl(LPCTSTR szValue);			// Rpl설정(값)
	int		fnSPL_SetInform(LPCTSTR szValue);		// Inform설정(값)
	
// SPL GET MODE
	CString	fstrSPL_GetErrorCode();					// 장애코드구하기
	int		fnSPL_GetDeviceStatus();				// 장치상태구하기

	int		fnSPL_GetTestKey();						// 단체테스트키구하기
	int		fnSPL_GetLoadSwitch();					// 불휘발램로드스위치구하기
	int		fnSPL_GetCallKey();						// 호출키구하기
	int		fnSPL_GetSplKeyValue();					// SCDP SPL키값구하기		V03-01-01 추가

// SPL OPERATION MODE
	int		fnSPL_Initialize();						// 연결시작및초기화
	int		fnSPL_Deinitialize();					// 연결종료

/////////////////////////////////////////////////////////////////////////////
//	UPS함수(UPS)
/////////////////////////////////////////////////////////////////////////////
// UPS SET MODE
	int		fnUPS_ClearErrorCode();					// 장애코드초기화

// UPS GET MODE
	CString	fstrUPS_GetErrorCode();					// 장애코드구하기
	int		fnUPS_GetDeviceStatus();				// 장치상태구하기

	int		fnUPS_GetPowerStatus();					// 전원상태구하기

// UPS OPERATION MODE
	int		fnUPS_Initialize();						// 연결시작및초기화
	int		fnUPS_Deinitialize();					// 연결종료

	int		fnUPS_PowerOff(int nWaitSec);			// 전원Off(종료대기시간)
	int		fnUPS_PowerRestart(int nWaitSec, int nRestartSec);					
													// 전원Restart(종료대기시간,시작대기시간)

/////////////////////////////////////////////////////////////////////////////
//	DES함수(DES)
/////////////////////////////////////////////////////////////////////////////
// DES SET MODE
	int		fnDES_ClearErrorCode();					// 장애코드초기화

// DES GET MODE
	CString	fstrDES_GetErrorCode();					// 장애코드구하기
	int		fnDES_GetDeviceStatus();				// 장치상태구하기

// DES OPERATION MODE
	int		fnDES_Initialize();						// 연결시작및초기화
	int		fnDES_Deinitialize();					// 연결종료

	int		fnDES_EncryptPin(void* vPinNum, int PinSize, void* vMemberNum, int MemberNumSize);
													// Des Pin Encryption : 2004.05.13 teo MemberNumSize추가

	int		fnDES_LoadKey(void* vKeyData, int Param);	// Des KEY Load
	int		fnDES_Test();							// Des Test
	int		fnDES_SW_EncryptPin(void* vPinNum, int PinSize, void* vMemberNum, int MemberNumSize, void* vWorkingKey);
/////////////////////////////////////////////////////////////////////////////
//	FINGER함수(FNG)
/////////////////////////////////////////////////////////////////////////////
// FINGER SET MODE
	int		fnFNG_ClearErrorCode();					// 장애코드초기화

// FINGER GET MODE
	CString	fstrFNG_GetErrorCode();					// 장애코드구하기
	int		fnFNG_GetDeviceStatus(int nFlag = FALSE);				// 장치상태구하기
	CString fstrFNG_GetData();						// 지문자료구하기

// FINGER OPERATION MODE
	int		fnFNG_Initialize();						// 연결시작및초기화
	int		fnFNG_Deinitialize();					// 연결종료
	int		fnFNG_Acquire();						// 지문읽기
	int		fnFNG_CancelAcquire();					// 지문읽기취소
	int		fnFNG_ShowData(LPCTSTR szFlag);			// 지문표시하기(표시구분)
	int		fnFNG_ShowSize(int nXPos, int nYPos, int nWideSize, int nHeightSize);
													// 지문표시창설정

/////////////////////////////////////////////////////////////////////////////
//	IRDA함수(IRD)
/////////////////////////////////////////////////////////////////////////////
// IRDA SET MODE
	int		fnIRD_ClearErrorCode();					// 장애코드초기화

// IRDA GET MODE
	CString	fstrIRD_GetErrorCode();					// 장애코드구하기
	int		fnIRD_GetDeviceStatus();				// 장치상태구하기
	CString fstrIRD_GetData();						// 적외선자료구하기
	int		fnIRD_AnalData(LPCTSTR szData);			// 적외선데이타분석		2004.02.05			

// IRDA OPERATION MODE
	int		fnIRD_Initialize();						// 연결시작및초기화
	int		fnIRD_Deinitialize();					// 연결종료
	int		fnIRD_EntryEnable();					// 투입허가
	int		fnIRD_EntryDisable();					// 투입금지

/////////////////////////////////////////////////////////////////////////////
//	BAR함수(BAR)
/////////////////////////////////////////////////////////////////////////////
// BAR SET MODE
	int		fnBAR_ClearErrorCode();					// 장애코드초기화

// BAR GET MODE
	CString	fstrBAR_GetErrorCode();					// 장애코드구하기
	int		fnBAR_GetDeviceStatus();				// 장치상태구하기
	CString	fstrBAR_GetData();						// 바코드자료구하기

// BAR OPERATION MODE
	int		fnBAR_Initialize();						// 연결시작및초기화
	int		fnBAR_Deinitialize();					// 연결종료
	int		fnBAR_EntryEnable();					// 읽기허가
	int		fnBAR_EntryDisable();					// 읽기금지

/////////////////////////////////////////////////////////////////////////////
//	COMMON함수(CMN)
/////////////////////////////////////////////////////////////////////////////
// CMN SET MODE
// CMN GET MODE
// CMN OPERATION MODE
	CString	fstrCMN_CheckDeviceAction(int nDevId, int nCheckTime);	
													// 동작대기및장애검지(장치Id,체크시간)
	int		fnCMN_ScanDeviceAction(int nDevId, int nScanTime, int nEventKind);	
													// 이벤트종류별검지된장치구하기(장치Id,검색시간,이벤트종류)
	int		fnCMN_GetErrorDevice(int nDevId);		// 장애장치구하기(장치Id)
	int		fnCMN_GetTimeoutDevice(int nDevId);		// 시간초과장치구하기(장치Id)


	int		fnCMN_ProcBackupTrace(LPCTSTR szFileName, int nFileSize = 10000000);
													// Kal Trace Backup(파일이름,파일길이)

/////////////////////////////////////////////////////////////////////////////
//	K-CASH함수(KCS)
/////////////////////////////////////////////////////////////////////////////
// K-CASH SET MODE
// K-CASH GET MODE
// K-CASH OPERATION MODE
	int		fnKCS_PowerOn();						// 전원공급
	int		fnKCS_PowerOff();						// 전원차단
	int		fnKCS_ReadATR();						// ATR읽기:시범카드,본사업카드

	int		fnKCS_CheckPin(LPCTSTR szPassWord);		// 비밀번호검사
	int		fnKCS_VerifyTranBit();					// 거래중지Bit확인
	int		fnKCS_RemoveTranBit();					// 거래중지Bit제거
	int		fnKCS_ReadTrafficInfo();				// 교통정보읽기

	int		fnKCS_ReadPan(LPCTSTR szPassWord);		// 계좌정보읽기
	int		fnKCS_ReadBAmount();					// 잔액정보읽기
	int		fnKCS_ReadPersonInfo(LPCTSTR szPassWord);	
													// 개인정보읽기
	int		fnKCS_ReadPassBook1(LPCTSTR szPassWord);	
													// 주계좌정보읽기

	int		fnKCS_PrepareLoadIEP(LPCTSTR szSendData);	
													// 충전전처리(거래금액8+암호16)
	int		fnKCS_LoadIEP(LPCTSTR szSendData);		// 충전요구(난수16+서명8)

	int		fnKCS_PrepareUnLoadIEP(LPCTSTR szSendData);	
													// 환불전처리(거래금액8+암호16)
	int		fnKCS_UnloadIEP(LPCTSTR szSendData);	// 환불요구(난수16+서명8)

	int		fnKCS_PrepareUpdateIEP();				// 파라메터갱신전처리
	int		fnKCS_UpdateIEP(LPCTSTR szSendData);	// 파라메터갱신(SamId8+Parameter4+Versin2+Value32+난수16+서명8)

	int		fnKCS_ChangePin(LPCTSTR szSendData);	// 비밀번호변경(변경전암호16+변경후암호16)

	int		fnKCS_ReadCLog();						// 충전환불거래내역읽기
	int		fnKCS_ReadPLog1();						// 구매거래내역읽기(1-5)
	int		fnKCS_ReadPLog2();						// 구매거래내역읽기(6-10)

	int		fnKCS_PrepareUnLoadRcvIEP(LPCTSTR szPassWord);
													// 환불복구전처리
	int		fnKCS_ReadEPID();						// 전자화폐번호 읽어오기(기업용) 

	int		fnKCS_AnalRecvData();					// 수신분석

/////////////////////////////////////////////////////////////////////////////
//	IC 공통 함수(ICA). 20040127-HSW-A
/////////////////////////////////////////////////////////////////////////////
//  Operation Function
	void	fnICA_InitProcess();					// IC 거래 초기화(Buffer Clear)
	int		fnICA_ConstructAPDU(LPBYTE lpSendData, BYTE ISOCase, BYTE CLA, BYTE INS,
				BYTE P1, BYTE P2, BYTE Lc = NULL, LPBYTE pData = NULL, BYTE Le = NULL);
													// IC Card 표준 APDU Message를 조립
	int		fnICA_ConstructTLV(LPBYTE lpDestData, LPBYTE lpTag, int TagLen,
				LPBYTE lpVaule, int ValueLen);		// Data 객체를 표준 TLV 형태로 만들어 줌(2바이트 Tag용)
	int		fnICA_ConstructTLV(LPBYTE lpDestData, BYTE bTag, LPBYTE lpVaule,
				int ValueLen);						// Data 객체를 표준 TLV 형태로 만들어 줌(1바이트 Tag용)
	int		fnICA_ISOIn(LPBYTE lpAPDU, int nAPDULen);
													// IC Card 표준 APDU 명령 전송(응답 데이터 없음)
	int		fnICA_ISOOut(LPBYTE lpAPDU, int nAPDULen);
													// IC Card 표준 APDU 명령 전송(응답 데이터 있음)
	int		fnICA_SelectApplet(int nICKind);		// IC Card 종류를 통해서 Applet을 선택
	int		fnICA_SelectApplet(LPBYTE lpAID, int nAIDLen);
													// AID를 이용하여 Applet 직접 선택
	int		fnICA_ICConfirmType(BYTE BankType);		// AID를 이용하여 IC 카드 내의 Application 종류를 구별
	int		fnICA_ISO7816WarningProc(LPBYTE lpData, int &nDataLen);
													// APDU 표준 재송신 처리. 61xx, 6Cxx.
	int		fnICA_CheckFCCode();					// FC Code 점검
	int		fnICA_AnalyzeFCError();					// FC Error 분석
	int		fnICA_AnalyzeISOError(LPBYTE lpData, int &nDataLen);
													// ISO Error 분석
	LPBYTE	fnICA_GetFCError();						// 업무코드 Error Get
	void	fnICA_SetFCError(LPBYTE lpErrData);		// 업무코드 Error Set
	LPSTR	fnICA_GetErrCode();						// Error Code Get for Screen Display
	void	fnICA_SetErrCode(LPSTR strErrCode);		// Error Code Set for Screen Display
	LPSTR	fnICA_GetErrMsg();						// Error Message Get for Screen Display
	void	fnICA_SetErrMsg(LPSTR strErrMsg);		// Error Message Set for Screen Display

/////////////////////////////////////////////////////////////////////////////
//	금융 IC 함수(ICA). 20040127-HSW-A
/////////////////////////////////////////////////////////////////////////////
//  Operation Function
	int		fnFIC_InitProcess();					// 금융 IC 거래 초기화(Buffer Clear)
	int		fnFIC_GetICType();						// IC CHIP TYPE 정보
	int		fnFIC_ReadBasicInfo();					// 금융 IC 기본 정보 조회
	int		fnFIC_ReadAccountInfo(BYTE bIndex = 0x00);
													// 금융 IC 계좌 정보 조회
	int		fnFIC_ReadDualInfo(BYTE bIndex = 0x00);	// 금융 IC 기본 정보 + 계좌 정보 조회
	int		fnFIC_GetEncipher(char* szPassWord, int PassWordSize);	
													// 금융 IC Get Encipher
	int		fnFIC_GetENCData(BYTE bIndex, char* szPassWord);
													// 금융 IC Get ENCData(MyBi)
	int		fnFIC_GetChallenge();					// 금융 IC Get Challenge(MyBi)

	int		fnFIC_CheckPIN(LPCTSTR szPassWord);		// 금융 IC PIN Verify
	int		fnFIC_ReadPan(LPCTSTR szPassWord);		// 금융 IC ReadPAN			
	int		fnFIC_ReadBAmount();					// 금융 IC ReadJAN			
	int		fnFIC_ChangePin(LPCTSTR szSendData);	// 금융 IC 비밀번호변경(변경전암호16+변경후암호16)
	int		fnFIC_OptionalChangePin(int nOpenCloseType, BYTE bChangeAppletType, BYTE bChangeKeyPinType, LPCTSTR szICSendData);

	int		fnFIC_ReadTrafficInfo();				// 금융 IC 교통정보읽기
	int		fnFIC_PrepareUnLoadRcvIEP(LPCTSTR szPassWord);
													// 금융 IC 환불복구전처리

	
	int		fnFIC_XORData(char* szPassWord, LPBYTE lpSource, LPBYTE lpResult);
													// 금융 IC XOR Data for GetENCData(MyBi)
	int		fnFIC_AnalRecvData(int nCmdType);		// 금융 IC Chip Receive Data 해석
	int		fnFIC_MakeRandom(LPBYTE lpSource, int nLen, int Lv = 1);
													// 금융 IC Random Data 생성 함수
	int		fnFIC_CheckIFMError(int nICRecvData);	// 금융 IC RecvData Error Check(IFM Direct)			// V09-33-00-#01

//  Get/Set Function
	LPSTR	fnFIC_GetCardHolderName();				// 카드 소지자 이름 Get
	BYTE	fnFIC_GetRealNameType();				// 실명구분 Get
	LPSTR	fnFIC_GetRealNameNO();					// 실명번호 Get
	LPSTR	fnFIC_GetExpireDay();					// 만기일 Get
	BYTE	fnFIC_GetAlgorithmCode();				// 알고리즘 식별코드 Get
	LPSTR	fnFIC_GetIssureCode();					// 발행기관 식별코드 Get
	LPSTR	fnFIC_GetCSN();							// 카드 일련번호 Get
	int		fnFIC_GetAccountCnt();					// 계좌 갯수 Get
	BYTE	fnFIC_GetAccountState(BYTE bIndex);		// 계좌정보 파일 중 계좌상태 Get
	LPSTR	fnFIC_GetAccountNum(BYTE bIndex);		// 계좌정보 파일 중 계좌번호 Get
	int		fnFIC_GetISO3(BYTE bIndex, LPSTR szISO3);
													// 거래가 선택된 계좌의 ISO 3 Data Get
	LPSTR	fnFIC_GetBankCode();					// 거래가 선택된 계좌 ISO 3 Track 내의 은행 코드 Get
	int		fnFIC_SetFICDispInfo();					// 화면에 보여줄 계좌정보 Format을 세팅. 은행 사양에 따라 다름
	LPBYTE	fnFIC_GetTRandom();						// Terminal Random Get
	LPBYTE	fnFIC_GetCRandom();						// Card Random Get
	LPBYTE	fnFIC_GetCipherPW();					// 암호화된 계좌 비밀번호 Get
	LPBYTE	fnFIC_GetENCData(BYTE bIndex);			// ENC Data Get(MyBi)
	LPSTR	fnFIC_GetHostTranData(int IsMyBi);		// Host 전송 암호화 데이터 Get

	int		fnFIC_EPBReadAccountInfo(BYTE bIndex = 0x00);
	int		fnFIC_GetEPBAccountCnt();				// 계좌 갯수 Get
	int		fnFIC_SetEPBDispInfo();					// 화면에 보여줄 계좌정보 Format을 세팅. 은행 사양에 따라 다름
	int		fnFIC_GetEPBISO3(BYTE bIndex, LPSTR szISO3);
													// 거래가 선택된 계좌의 ISO 3 Data Get

/////////////////////////////////////////////////////////////////////////////
//	EMV 함수(EMV)
/////////////////////////////////////////////////////////////////////////////
	//  Main Operation Function
	int		fnEMV_Initialize();						// EMV 관련 Buffer를 초기화(기기 Init시 1회 호출)
	int		fnEMV_CAListSet(LPBYTE m_Data, WORD Len); // EMV CA Public Key를 받아와 커널로 전달(인증용)
	int		fnEMV_TerminalListSet(LPBYTE m_Data, BYTE Len);
													// EMV Application List를 받아와 EMV커널로 전달함.
	int		fnEMV_TerminalConstSet(LPBYTE m_Data, BYTE Len); 
	int		fnEMV_SelectPSE();						// EMV 거래 가능 여부를 PSE를 통해 확인
	int		fnEMV_ReqCandidateList();				// 카드로부터 EMV 거래가 가능한 후보 목록을 요청
	int		fnEMV_SelectApplication(BYTE Index);	// 후보 목록 중 선택된 Application으로 거래를 시작
	int		fnEMV_ReadApplication();				// 거래를 위해 Application의 정보를 수집
	int		fnEMV_OfflineDataAuthentication(char* szAmount, int AmountLen,
							char* szAnotherAmount, int AnotherAmountLen);
													// 오프라인 인증을 요구
	int		fnEMV_OnlineProcessing(LPSTR lpszHostData, int nHostResp, BOOL HostRecvFlag, int Certificate = 0);
													// 온라인 승인 요청에 대한 호스트 응답을 카드로 전달
	int		fnEMV_CompletionProcessing();			// 카드로 거래 종료 요청
	int		fnEMV_AfterScriptProcessing();			// 거래를 종료하기 위해 스크립트 처리를 요청
	int		fnEMV_GetDOLData(LPBYTE lpTag, int nTagLen);
													// AP가 거래 중 필요한 데이터 객체를 EMV Kernel에 요청
	int		fnEMV_ResultData();						// 거래 종료 후 화면으로 보여줄 EMV 거래 결과 변수들을 요청(인증용 & 디버깅용)
	int		fnEMV_AnalRecvData(int nCmdType);		// EMV Chip Receive Data를 해석하고 버퍼에 저장

//  Sub Operation Function
	int		fnEMV_InitProcess();					// 거래시마다 초기화 되어야 할 Buffer Clear
	int		fnEMV_DefaultDOLSet();					// Default Data Object List Set
	int		fnEMV_DefaultDOLSetINI();				// Default Data Object List Set from INI File
	int		fnEMV_DefaultAIDListSet();				// Default 지원 AID List Set
	int		fnEMV_DefaultAIDListSetINI();			// Default 지원 AID List Set from INI File
	
	int		fnEMV_MakeReqCandidate(LPBYTE lpList);	// fnEMV_ReqCandidateList()에 사용할 데이터 목록 조립
	int		fnEMV_MakeOfflineDataAuthen(LPBYTE lpList, char* szAmount, int AmountLen,
							char* szAnotherAmount, int AnotherAmountLen);
													// fnEMV_OfflineDataAuthentication()에 사용할 데이터 목록 조립
	int		fnEMV_MakeOnlineProcessing(LPBYTE lpEMVResp, LPSTR lpszHostData,
							int nHostResp, BOOL HostRecvFlag);
													// fnEMV_OnlineProcessing()에 사용할 데이터 목록 조립

//  Get/Set Function
	BYTE	fnEMV_GetConfigCode();					// Configuration Code Get
	void	fnEMV_SetConfigCode(BYTE ConfigCode);	// Configuration Code Set
	BYTE	fnEMV_GetAIDListCount();				// AID List Count Get
	BYTE	fnEMV_GetCandidateCount();				// Candidate List Count Get
	void	fnEMV_SetCandidateCount(BYTE CandidateCount);
													// Candidate List Count Set
	BYTE	fnEMV_GetTranIndex();					// 선택된 Application의 Index Get
	void	fnEMV_SetTranIndex(BYTE TranIndex);		// 선택된 Application의 Index Set
	LPSTR	fnEMV_GetCertificationType();			// 인증서 종류(AAC, TC, ARQC, AAR) Get
	void	fnEMV_SetCertificationType(char* szCertificationType);
													// 인증서 종류(AAC, TC, ARQC, AAR) Set
	int		fnEMV_GetBrandCount();					// Application 목록 Get
	int		fnEMV_GetISO2(char* ISO2);				// MS Track 2 Image(Tag 37) Get
	BYTE	fnEMV_GetCID9F27();						// Cryptogram Information Data Get
	int		fnEMV_SetAppCryptogram();				// 인증서 종류를 결정
	LPSTR	fnEMV_GetHostTranData(BYTE TranType, int nEmvType = FALSE);	// Host 전송 데이터 Get
	int		fnEMV_GetINIPath(LPSTR lpstrPath);		// ini 파일을 위한 레지스트리 경로 읽기

/////////////////////////////////////////////////////////////////////////////
//	NETWORK함수(NET)
/////////////////////////////////////////////////////////////////////////////
// NET SET MODE
	int		fnNET_ClearErrorCode();					// 장애코드초기화

// NET GET MODE
	CString	fstrNET_GetErrorCode();					// 장애코드구하기
	int		fnNET_GetDeviceStatus();				// 장치상태구하기

// NET OPERATION MODE
	int		fnNET_Initialize(int nOpenSec);			// 연결시작및초기화
	int		fnNET_Deinitialize(int nCloseSec);		// 연결종료

	int 	fnNET_SendData(LPCTSTR szSendData, int nSendSec);
													// 자료전송(송신자료,송신시간)
	int		fnNET_RecvData(CString& strRecvData, int nRecvSec);			
													// 자료수신(수신자료,수신시간)
	CString	fnNET_GetIpAddress(int nFlag = FALSE);
	CString	fnNET_GetMacAddress(int nFlag = FALSE);
	int		fnNET_ICMP_DeviceStatus(int nSyncFlag = FALSE);
	CString	fnNET_GetIpAddressFromWeb();
	int		fnNET_RequestUrl(CHttpConnection * httpConnection, CString strUrl, CString strPostData, int nSSLFlag, CString &strData);
	int		fn_fnNET_Polling();
/////////////////////////////////////////////////////////////////////////////
//	VFD함수											// T2ATM 2006.5.24 yscho
/////////////////////////////////////////////////////////////////////////////
// VFD SET MODE
	int		fnVFD_ClearErrorCode();
	int		fnVFD_SetBrightnessLevel(int nLevel);	// 밝기조절
	int		fnVFD_SetDisplayAtPowerOn(int nSMemIndex, int nEMemIndex, int nScrolled, int nInterval);

// VFD GET MODE
	CString fstrVFD_GetErrorCode();
	int		fnVFD_GetDeviceStatus();

// VFD OPERATION MODE
	int		fnVFD_Initialize(int nPortNum = 13, int nBaudRate = 38400);
													// 연결시작및초기화
	int		fnVFD_DeInitialize();					// 연결종료
	int		fnVFD_InitializeDisplay();				// 화면지움/설정값초기화
	int		fnVFD_ClearDisplay();					// 화면지움
	int		fnVFD_ScrollDisplay(int nShiftNum, int lnRepeatNum, int nInterval);
													// 화면Scroll
	int		fnVFD_BlinkDisplay(int nPattern, int nNormalTime, int nReverseTime, int nRepeatNumber);
													// 화면Blink
	int		fnVFD_RealTimeTextDisplay(LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName);	
	int		fnVFD_RealTimeBitImageDisplay(LPCTSTR szFileName, int nOffsetX, int nOffsetY);
	int		fnVFD_RealTimeTextBitImageDisplay(LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName, LPCTSTR szFileName, int nImgOffsetX, int nImgOffsetY);
													// Text와 Bitmap Image File을 Overlapping하여 Display. 04.11.18

	int		fnVFD_FROMTextSave(int nMemIndex, LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName);
	int		fnVFD_FROMBitImageSave(int nMemIndex, LPCTSTR szFileName, int nOffsetX, int nOffsetY);
	int		fnVFD_FROMTextBitImageSave(int nMemIndex, LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBolStyle, int nItalic, LPCTSTR szFontName, LPCTSTR szFileName, int nImgOffsetX, int nImgOffsetY);
	int		fnVFD_FROMDisplay(int nMemIndex);

/////////////////////////////////////////////////////////////////////////////
//	SENSOR함수(SNS) : for T2ATM Demo 2006.07.25
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

	//  장애인ATM Start
	int		fnSNS_GetEarPhoneStatus();
	int		fnSNS_ClearPhoneStatus();

	void	TTS_initialize();
	void	TTS_StartProc();
	void	TTS_EndProc();										// V07-01-11-#00
	void	fnAPL_TTSplay();
	void	fnAPL_TTSstop();

	void	fnAPL_MakeVoicePlay(int InitFlag);					// V06-07-01
	//  장애인ATM End


/////////////////////////////////////////////////////////////////////////////
//	RFID함수(RFD) : for T2ATM Demo 2006.04.20
/////////////////////////////////////////////////////////////////////////////
// RFD SET MODE
	int		fnRFD_ClearErrorCode();					// 장애코드초기화
// RFD GET MODE
	CString	fstrRFD_GetErrorCode();					// 장애코드구하기
	CString	fstrRFD_GetData();
	int		fnRFD_AnalData(LPCTSTR szData);			// 적외선데이타분석		2004.02.05

// RFD OPERATION MODE
	int		fnRFD_Initialize();
	int		fnRFD_Deinitialize();
	int		fnRFD_SendData(int nSendCmd, LPCTSTR szSendData);

	int		fnRFC_Initialize();						// RF정보 초기화
	int		fnRFC_GetBasicInfo();					// RF기본정보 구하기
	int		fnRFC_GetDevStatus();					// RF상태 구하기
	int		fnRFC_SelectMode(int nMode);			// RF충전 모드설정
	int		fnRFC_ReqCharge(char *szMoney, char *szTime);
													// RF충전
	int		fnRFC_DrawBack(char *szTime);			// RF환불
	int		fnRFC_AnalRecvData(int nCmdType);

/////////////////////////////////////////////////////////////////////////////
//	GIRO함수(GRO)
/////////////////////////////////////////////////////////////////////////////
// GRO SET MODE
	int		fnGRO_ClearErrorCode();					// 장애코드초기화
	int		fnGRO_ClearRetractCnt();				// 회수매수초기화

// GRO GET MODE
	CString fstrGRO_GetErrorCode();					// 장애코드구하기
	CString fstrGRO_GetMediaStatus();				// 매체정보구하기
	CString	fstrGRO_GetDeviceStatus();				// 장치상태구하기

	int		fnGRO_GetMaterialInfo();				// 매체잔류정보구하기

	CString fstrGRO_GetRetractCnt();				// 회수매수구하기

	CString fstrGRO_GetGiroData();					// 지로자료구하기
	CString fstrGRO_GetGiroType();					// 지로종류구하기

// GRO OPERATION MODE
	int		fnGRO_Initialize();						// 연결시작및초기화
	int		fnGRO_Deinitialize();					// 연결종료

	int		fnGRO_Accept(int nWaitSec = K_30_WAIT);	// 투입허가(투입대기시간)및열기
	int		fnGRO_CancelAccept();					// 투입금지및닫기
	int		fnGRO_ReadyAccept();					// 투입준비
	int		fnGRO_Read(LPCTSTR szFileName);			// 읽기(이미지이름)
	int		fnGRO_2DRead(LPCTSTR szFileName);		// 2D읽기(이미지이름)
	int		fnGRO_ReadOCRn2D(LPCTSTR szFileName);	// OCR & 2D읽기(이미지이름)
	int		fnGRO_Read2DNotGetImg(LPCTSTR szFileName);
													// 2D읽기(이미지이름)
	int		fnGRO_Reject();							// 반환
	int		fnGRO_RejectAll();						// 전체반환
	int		fnGRO_Escrow();							// 임시보관
	int		fnGRO_Stack();							// 수납

	int		fnGRO_EnableStatusEvents();				// 상태변경허용
	int		fnGRO_DisableStatusEvents();			// 상태변경불가

/////////////////////////////////////////////////////////////////////////////
//	SCR함수(SCR)
/////////////////////////////////////////////////////////////////////////////
// SCR SET MODE
	int		fnSCR_ClearErrorCode();					// 장애코드초기화
	int		fnSCR_ClearKeyData();					// 키값초기화
	int		fnSCR_SetScreenData(LPCTSTR szSetScreenDataName, LPCTSTR szSetScreenDataValue);
													// 정보전달(자료이름,자료값)
	int		fnSCR_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue);
													// 출력자료지정(출력자료이름,출력자료값)
	int		fnSCR_SetCurrentLangMode(int nLangMode = KOR_MODE);				
													// 현재언어모드설정	+ MachineFileType		

// SCR GET MODE
	CString	fstrSCR_GetErrorCode();					// 장애코드구하기
	int		fnSCR_GetDeviceStatus();				// 장치상태구하기

	CString	fstrSCR_GetKeyData();					// 입력키값구하기
	CString	fstrSCR_GetKeyString(int nInitFlag = TRUE);	
													// 문자입력키값구하기(보정길이:0=실제길이)
	CString	fstrSCR_GetKeyNumeric(int nRetLength = 0, int nInitFlag = TRUE);	
													// 숫자입력키값구하기(보정길이:0=실제길이)
	CString	fnSCR_GetCheckScreen(int nScreenNo, int nLangMode = KOR_MODE);	
													// 화면존재유무구하기(화면번호,언어모드)
	int		fnSCR_GetCurrentScreenNo();				// 현재화면번호구하기
	int		fnSCR_GetCurrentLangMode();				// 현재언어모드구하기

// SCR OPERATION MODE
	int		fnSCR_Initialize();						// 연결시작및초기화
	int		fnSCR_Deinitialize();					// 연결종료

//t	int 	fnSCR_DisplayScreen(int nScreenNo, int nDisplaySec = 0);
	int 	fnSCR_DisplayScreen(int nScreenNo,								// 화면번호
								int nDisplaySec = K_NO_WAIT,				// 화면출력시간
								int nPinInputMode = PIN_DISABLE_MODE,		// PinPad입력방식
								LPCTSTR szCardData = "",					// Macing할 카드데이타(EPP모드)
								int nPinPassWordMin = 4,					// 비밀번호최소입력자릿수
								int nPinPassWordMax = 8,					// 비밀번호최대입력자릿수
								int nPinPassWordAuto = FALSE,				// 최대자릿수입력시자동입력종료
								LPCTSTR szPinPassWordTerm = "ENTER,CANCEL");// 입력종료처리입력값
													// 화면출력(화면번호,언어모드...)
	int		fnSCR_DisplayImage(int nImageNo, int nOnOffFlag);
	int		fnSCR_DisplayString(int nStringNo, LPCTSTR szString, int nInLength, int Flag = FALSE);
													// 문자출력(문자번호,문자,길이)
	int		fnSCR_DisplayString(int nStringNo, LPCTSTR szString);
	int		fnSCR_DisplayStringArray(int nStringNo, LPCTSTR szString, LPCTSTR szSeperator = "|");													// 문자출력(문자번호,문자)
	int		fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric, int nInLength);
													// 숫자출력(문자번호,문자,길이)
	int		fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric);
													// 숫자출력(문자번호,문자)
	int		fnSCR_DisplayNumeric(int nStringNo, int nNumeric);
													// 숫자출력(문자번호,숫자)
	int		fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, int nInLength, char cMarkChar = ' ', int nIsMinus = FALSE);
													// 금액출력(문자번호,문자,길이,마크,부호)
	int		fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, char cMarkChar = ' ', int nIsMinus = FALSE);
													// 금액출력(문자번호,문자,마크,부호)
	int		fnSCR_DisplayAmount(int nStringNo, int nAmount,	char cMarkChar = ' ', int nIsMinus = FALSE);
													// 금액출력(문자번호,금액,마크,부호)

/////////////////////////////////////////////////////////////////////////////
// 한글입력
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
//	SOUND함수(SND)
/////////////////////////////////////////////////////////////////////////////
// VOL SET MODE
	int		fnSND_SetCurrentVolume(int nVolume);	// 볼륨조정

// VOL GET MODE
	int		fnSND_GetCurrentVolume();				// 볼륨얻어오기

/////////////////////////////////////////////////////////////////////////////
// FTP함수(FTP) 
/////////////////////////////////////////////////////////////////////////////
	int		fnFTP_Initialize();						// 초기화
	int 	fnFTP_Connection(LPCTSTR szSvrIP, LPCTSTR szLoginID, LPCTSTR szLoginPwd);
													// 연결시작
	int		fnFTP_Deinitialize();					// 연결종료
	int 	fnFTP_FindFirstFile(LPCTSTR szFindFile);// 파일찾기
	int 	fnFTP_GetFile(LPCTSTR szSvrDir, LPCTSTR szSvrFile);
													// 파일가져오기
	int 	fnFTP_PutFile(LPCTSTR szLocalFile, LPCTSTR szSvrFile);
													// 파일올리기
	int 	fnFTP_DeleteFile(LPCTSTR szFile);		// 파일지우기
	int 	fnFTP_RenameFile(LPCTSTR szSrcFile, LPCTSTR szDstFile);
													// 파일이름바꾸기
	int 	fnFTP_OpenFile(LPCTSTR szFile, int nOption);
													//파일열기
	int 	fnFTP_CreateDir(LPCTSTR szDir);			// 디렉토리만들기
	int 	fnFTP_RemoveDir(LPCTSTR szDir);			// 디렉토리지우기
	int 	fnFTP_SetCurDir(LPCTSTR szDir);			// 현재디렉토리바꾸기
	int 	fnFTP_GetCurDir(LPCTSTR szDir);			// 현재디렉토리읽어오기


/////////////////////////////////////////////////////////////////////////////
//	기초함수(APL)
/////////////////////////////////////////////////////////////////////////////
// APL SET MODE
// APL GET MODE
// APL OPERATION MODE
	int		fnAPL_InitializeDS();					// 변수초기화

	int		fnAPL_LoadDevice();						// 장치올리기
	int		fnAPL_UnLoadDevice(int nEndFlag = FALSE);					// 장치내리기

	int		fnAPL_GetDefineDevice(int nDevId);		// 정의된장치구하기
	int		fnAPL_GetYesDevice(int nDevId);			// 정의되고연결장치구하기
	int		fnAPL_GetNoDevice(int nDevId);			// 정의되고미연결장치구하기
	int		fnAPL_GetDownDevice(int nDevId);		// 정의되고다운장치구하기
	int		fnAPL_GetAvailDevice(int nDevId);		// 사용가능한장치구하기
	int		fnAPL_GetAvailErrorDevice(int nDevId);	// 사용하고장애난장치구하기
	int		fnAPL_GetErrorDevice(int nDevId);		// 장애난장치구하기
	int		fnAPL_GetDownErrorDevice(int nDevId);	// 장애난장치구하기

	int		fnAPL_GetNumberOfCash();				// 현금매수구하기
	int		fnAPL_GetNumberOf50000Cash();			// 현금매수구하기(5만원권)	
	CString fnAPL_GetOutCheckBankNo();				// 출금수표은행코드구하기
	CString fnAPL_GetOutCheckBankNo2();				// 출금수표은행코드구하기2
	int		fnAPL_GetNumberOfOutCheck();			// 출금수표매수구하기
	CString	fnAPL_GetNumberOfOutCheckNo();			// 출금수표번호구하기
	int		fnAPL_GetNumberOfOutCheck2();			// 출금수표매수구하기
	CString	fnAPL_GetNumberOfOutCheckNo2();			// 출금수표번호구하기
	int		fnAPL_GetKindOfOutCheck();				// 출금수표권종구하기 출금수표권종조회
	int		fnAPL_GetNumberOfInCheck();				// 입금수표매수구하기
	int		fnAPL_SetNumberOfCash(int nCashCnt);	// 현금매수설정
	CString	fnAPL_SetOutCheckBankNo(LPCTSTR szBankNo, LPCTSTR szBankNo2);// 출금수표은행코드설정
	int		fnAPL_SetNumberOfOutCheck(int nOutCheckCnt, int nOutCheckCnt2);
													// 출금수표매수설정
	CString	fnAPL_SetNumberOfOutCheckNo(LPCTSTR szOutCheckNo, LPCTSTR szOutCheckNo2);
													// 출금수표번호설정
	int		fnAPL_SetNumberOfInCheck(int nInCheckCnt);
													// 입금수표매수설정
//	int		fnAPL_SetKindfOutCheck(int nOutCheckKind);
													// 출금수표권종설정
	int 	fnAPL_GetAvailTrans();					// 거래가능상태구하기
	int 	fnAPL_GetAvailWithdraw();				// 출금가능상태구하기
	int 	fnAPL_GetAvailDeposit();				// 입금가능상태구하기
	int 	fnAPL_GetAvailPassbook();				// 통장가능상태구하기
	int 	fnAPL_GetClerkTransOnOff();				// 계원거래모드구하기
	int 	fnAPL_GetAvailA4printer();				// A4 프린터가능상태구하기

	int		fnAPL_SetSensorInfo();					// 센서정보설정
	int		fnAPL_SetDate(LPCTSTR szDate);			// 거래일자설정
	int		fnAPL_SetCheckDate(LPCTSTR szCheckDate);// 수표일자설정
	int		fnAPL_ClearSerialNo();					// 일련번호초기화
	int 	fnAPL_AddSerialNo();					// 일련번호증가
	int		fnAPL_SetSerialNo();					// 일련번호설정
	int 	fnAPL_SetProcCount(char chProcCount);	// 진행번호설정
	int 	fnAPL_SetOnTransaction(int nTransDoing);// 거래진행중설정
	int		fnAPL_SetTranStart();					// 거래시작설정 : 거래시작시시간단축(fnAPL_SetOnTransaction(ON)+fnAPL_SetProcCount(1)) : 2003.11.21
	int 	fnAPL_ClearDeviceExecOffInfo(int nInitFlag = FALSE);			
													// 장치운영불가정보초기화
	int		fnAPL_CshRetactCntInfo(int nCnt = 0, int nInitFlag = FALSE);
	int		fnAPL_SetClerkPowerOff();				// 파워상태검지
	int		fnAPL_CheckPowerStatus(int nRealFlag = FALSE);	// 파워상태검지
	int 	fnAPL_CheckPowerOffError();				// 파워오프장애검지

	int		fnAPL_ClearError();						// 장애정보초기화
	int 	fnAPL_CheckError();						// 장애검지
	int		fnAPL_CheckErrorCodeValid(LPCTSTR szErrorCode);		
													// 장애코드인증검사
	int 	fnAPL_StackError(LPCTSTR szErrorCode, LPCTSTR szErrorString, int nDevId = DEV_NONE, int nTranType = FALSE);
													// 장애설정및장애정보보관
	CString	fnAPL_ErrorConv(LPCTSTR szErrorCode);	// U8100 ErrorCode Conversion
	CString	fnAPL_ErrorU3100KConv(LPCTSTR szErrorCode);	// fnAPL_ErrorU3100KConv ErrorCode Conversion
	int 	fnAPL_CheckHostLine();					// 호스트라인검지
	int		fnAPL_CheckHostOpen();					// 호스트개국검지
	int		fnAPL_CloseLine();						// 호스트라인폐국
	int		fnAPL_CloseHost();						// 호스트폐국
	int		fnAPL_OpenLine();

	int		fnAPL_InitializeDevice(int nDevId, int nInitFlag = FALSE);
													// 장치초기화
	int		fnAPL_CheckDeviceAction(int nDevId);	// 동작대기및장애검지
	int		fnAPL_CheckMaterial(int nDevId = DEV_MAIN);		
													// 매체잔류검지및잔류장애발생
	int		fnAPL_RetractMaterial(int nDevId);		// 분실매체회수
	int		fnAPL_AutoOffErrorDevice();				// 장애난장치자동축퇴
	
	int 	fnAPL_DisplayNormalSegment();			// 정상Segment출력
	int 	fnAPL_DisplayErrorSegment(char chProcCount, LPCTSTR szErrorCode);
													// 장애Segment출력
	int 	fnAPL_DisplayProcCountSegment();		// 운영Segment출력
	int 	fnAPL_DisplayInformationLed(int nInitFlag = FALSE);			
													// 정보Led출력
	int 	fnAPL_DisplayInformationLedOnOff(int nLedIndex, int bOnOff);
													// 정보Led출력 - 개별처리
	int		fnAPL_DisplayInformationIndicator(int nInitFlag = FALSE);			
													// 정보Indicator출력
	int 	fnAPL_SendInformationRpl(int nInitFlag = FALSE);			
													// 정보Rpl전송
	int 	fnAPL_SendInformationInform(int nInitFlag = FALSE);			
													// 정보Inform전송
	
	int		fnAPL_DisplayError(int nErrorStatus, int nInitFlag = FALSE);
													// 장애화면출력및인자
	int 	fnAPL_PrintError(int nDevId);			// 장애인자

	int 	fnAPL_CheckPrintDevicePosition();		// 장치정위치인자
	int 	fnAPL_CheckPrintBrmPosition();			// Brm정위치인자
	int 	fnAPL_CheckPrintUcmPosition();			// Ucm정위치인자
	int		fnAPL_CheckPrintRearDoorStatus();		// 둿문상태인자
	int 	fnAPL_PrintPosition(LPCTSTR szPrintData);
													// 정위치인자
	
													//:5만원권지원(nFiftyThRJT 추가)
													// V06-02-01-#07:보충회수시 5만원 및 만원권표시를 위한 Param 추가 (nTenThMoveCnt, nFiftyThMoveCnt)
	int		fnAPL_PrintCashMoveInfo(int nMoveFlag, int nRealMove, int nTenThRJT, int nFiveThRJT, int nOneThRJT, int nUnknownRJT, 
									int nFiftyThRJT = 0, int nTenThMoveCnt = 0, int nFiftyThMoveCnt = 0);
													// 현금이동정보인자
	int		fnAPL_PrintCashInfo();					// 현금등록정보인자
	int		fnAPL_PrintCheckInfo();					// 수표등록정보인자
	int		fnAPL_PrintJnlInfo(LPCTSTR szJnlInfoData, int JnlType = JNL_OPER, int nWaitFlag = FALSE);
													// 저널인자

	int		fnAPL_SprAutoload();					// 명세표 오토로드

	int 	fnAPL_AutoRefillBrm(int nBrmRefillMode);// Brm자동보충/회수
	int 	fnAPL_AutoResetBrm();					// Brm자동복구
	int 	fnAPL_AutoReadyBrm();					// Brm자동준비

	int		fnAPL_ResetDevice(int nDevId, int nInitFlag = FALSE);				
													// 장치복구
	int		fnAPL_UnUsed();							// 사용중지
//t	int 	fnAPL_DeviceEnDisable(int nDevId, int nAction, int nWaitFlag = FALSE);
	int 	fnAPL_DeviceEnDisable(int nDevId, int nAction, int nWaitFlag = FALSE, int nPinInputMode = PIN_DISABLE_MODE, LPCTSTR szCardData = "", int nPinPassWordMin = 4, int nPinPassWordMax = 6, int nPinPassWordAuto = FALSE, LPCTSTR szPinPassWordTerm = "ENTER,CANCEL");
													// 장치투입모드설정(Pinpad추가확장처리)
	int 	fnAPL_DeviceSetFlicker(int nDevId, int nAction);

	int 	fnAPL_GetDeviceEvent(int nDevId);		// 선행장치구하기

	int		fnAPL_ECashEjrSpoolPrint(int nDevId, LPCTSTR szSpoolData, NEjrTblDump* pEjrData = NULL, EjrAmountTbl* pEjrAmountData =  NULL);
	int		fnAPL_EjrSpoolPrintStart(int nDevId, EjrTbl* pEjrData = NULL, EjrAmountTbl* pEjrAmountData = NULL);
													// 전자저널시작
	int		fnAPL_EjrSpoolData(int nDevId, int nLine, int nSize, LPCTSTR szSpoolData);
													// 전자저널
	int		fnAPL_EjrSpoolEnd(int nDevId, int JnlType = JNL_TRAN, int nWaitFlag = FALSE);
													// 전자저널종료&저장및인자처리
	int		fnAPL_EjrAddSerialNo();					// 전자저널일련번호증가
	int		fnAPL_EjrSave(int JnlType);				// 전자저널저장
	int		fnAPL_EjrDbManagerSend(LPCTSTR szDbData);
													// 전자저널자료전송

	int		fnAPL_CaptureHandSave(int nIndex, int nFirstFlag = FALSE, LPCTSTR szDefault = NULL);		
	// 손촬영
	int		fnAPL_CaptureFaceSave(int nIndex, int nFirstFlag = FALSE, LPCTSTR szDefault = NULL);	
	// 얼굴촬영
	int		fnAPL_CaptureRoomSave(int nIndex, int nFirstFlag = FALSE, LPCTSTR szDefault = NULL);	
	// U8100 개발관련 후면촬영
	int		fnAPL_CaptureVODSave(int nStartEndFlag = FALSE, int nCaptureVODPart = FALSE);
	// 동영상촬영 U8100-VOD
	int		fnAPL_CaptureSaveEnd(int nSaveFlag = TRUE);	 // 촬영종료&저장처리

	int		fnAPL_CaptureFaceAtmShocked(int nStep = 0);
	int		fnAPL_CaptureFaceAuthLockDoorOpen(int nStep = 0);
	int		fnAPL_CaptureFaceCustomDoorOpen(int nClearFlag = FALSE);

	char	fnAPL_GetErrorCodeGrade(LPCTSTR szHsErrorCode);
	int		fnAPL_GetErrorCodeKind(LPCTSTR szHsErrorCode);
													// 장애코드종류구하기
	CString	fnAPL_GetLC(LPCTSTR szHsErrorCode, int nTranProc = TRAN_NULL);
													// LC구하기

	int		fnAPL_ClerkInitModeSet();				// 계원초기화모드설정
	int		fnAPL_ClerkInformation(int nInitFlag = FALSE);			
													// U8100 AP 변경내역 #12 - 차세대OM관련 변경 : T3전용
	int		fnAPL_ClerkExecCheck();					// 계원실행검사

	int		fnAPL_SetSegment(LPCTSTR szValue);		// Segment설정 라우팅함수
	int		fnAPL_PowerOff(int nWaitSec);			// 전원Off(종료대기시간) 라우팅 함수
	int		fnAPL_GetPowerStatus();					// 전원상태구하기 라우팅함수

//////////////////////////////////////////////////////////////////////////
	int		fnAPL_VFDDislpay(int nPos);				// VFD실행
	int		fnAPL_CheckCatchPlay();					// 광고문구검사
	int		m_nVfdDoing;							// VFD Thread구동여부
	int		VFDTotalCount;							// VFD표시총문자갯수
	int		VFDCurCount;							// VFD표시현재위치
	int		VFDDisTerm;								// VFD표시시간간격(초)
	int		m_nTrPollDoing;							// Polling 구동여부
	int		m_SendRecvStaus;						// Polling 구동여부

	int		PrevInformFlg;										// 사전알림 중단/재개 지시자
	int		PBMInkCheckCnt;										// PBM Ink Chekc Count (1000회에 한번씩 잉크 체크)
	CString	PBMInkStatus;										// PBM Ink 상태
	int		CheckPbmInkStaus;									// PBM Ink 상태전문관련
	int		fnAPL_CheckLEDDipalyData(CString strDisplay);		// LED 광고 문구 검사 
	int		fnAPL_SetLedInformation();							// U8100 개발
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	int		m_nJournalMode;										// 저널모드(종이 or 전자저널)
	int		m_nAtmsCommand;										// ATMS명령구분
	int		m_nAtmsDoing;										// ATMS명령수행종료확인
	BYTE	m_szJnlIndexBuff[NETBUFFSIZE];
	int		m_nJnlIndexLength;
	CString	m_szSaveTime;							// 전자저널시간저장
	int		m_nJnlType;								// 전자저널타입저장

/////////////////////////////////////////////////////////////////////////////
//	SOUND함수(SND)
/////////////////////////////////////////////////////////////////////////////
	int		fnSND_KeySound(LPCTSTR fname = NULL);	// V03-01-01 SPL KEY사운드처리



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
//	기타함수(UTIL)
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
// Get Log Start ───────────────────────
	int		fnAPL_GetLogFileStart(int nMonth, int nDate);		// 2006.11.22 : Get Log File(월, 일)
	int		fnAPL_GetDriveInfo(CString& szDrive, DWORD& dwFreeSize, TCHAR cStartDirve = 'A');
	int		fnAPL_DeleteFiles(LPCTSTR DirName);
// Get Log End ───────────────────────
	int		fnAPL_NetworkInfoSet(int nFixIp = FALSE);					// 네트워크 IP Setting
	int		fnAPL_CheckMachineType();				
	int		fnAPL_SetComputerName();				
	int		fnAPL_GetRemoteCtrlFileStart(CString strLocalPath, int nYear, int nMonth, int nDate, int nTran, int nSeralF = FALSE);		// 2006.11.22 : Get Log File(월, 일)

	int		fnAPP_APVersionCheck();								// 모든 AP의 FileVersion을 체크하여 다른경우 장애처리
	CString	fnUTL_GetFileVer(LPCTSTR szFile);					// 파일정보얻기
	// ───────────────────────────────────────────────────────
	
	int	nTranStopFlg;										// AP File Version불일치시 거래중단 Flag
	// -------------------------------------------------------------


	// Hi-Pass접촉식충전개발 #N0266 
/////////////////////////////////////////////////////////////////////////////
//	하이패스 함수(HPS)
/////////////////////////////////////////////////////////////////////////////

	int		fnHPS_PowerOn();						// 전원공급
	int		fnHPS_PowerOff();						// 전원차단

// HiPass Dev<->IC Process
	int		fnHPS_ReadBalance();
	int		fnHPS_PrepareLoadIEP(LPBYTE lpICSendData);	
	int		fnHPS_LoadIEP(LPBYTE lpICSendData);
	int		fnHPS_CompleteTran();
	int		fnHPS_ReadLLog();
	int		fnHPS_ReadPan();
	
	int		fnHPS_AnalRecvData(int nCmdType);		// HiPass IC Chip Receive Data 해석
	int		fnHPS_APDUInitialize(LPCTSTR szICSendData);
	int		fnHPS_APDUBalRead();
// ──────────────────────────────────────
	
	int		fnAPL_BRMCashInfo();					// 로그분석기용
	int		fnAPL_BRMCstSetInfo();					// U3100K - 5만원권 금액설정
	int		fnAPL_BRMCstSetInfo_Empty_On();			// U3100K - 추가 현송 대책 임.
	int		fnAPL_BRMCstMatchInfo();				// U3100K - 카세트 점퍼 오류 
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
UINT	thrPreviewThread(LPVOID pParam);						// V06-02-01-#05:Camera Preview Thread(OM으로부터의 프리뷰 명령을 처리)
UINT	thrPollThead(LPVOID pParam);							// Nice Polling (30 Sec)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVCMN_H__C1CA462D_218C_4BA9_88BA_DBC13B14F86A__INCLUDED_)
