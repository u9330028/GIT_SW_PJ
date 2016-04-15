/////////////////////////////////////////////////////////////////////////////
// AtmModeCtrl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\H\Common\CmnLibIn.h"
#include "..\H\Common\Define.h"
#include "..\H\Common\ConstDef.h"
#include "..\H\Common\ClassInclude.h"
#include "..\H\Dll\DevCmn.h"
#include "..\H\Tran\TranCmn.h"

#include "WinAtm.h"
#include "WinAtmCtl.h"
#include "WinAtmPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	Atm Status
/////////////////////////////////////////////////////////////////////////////
//	Mode :	ATM_INIT			: 기기초기화	모드
//			ATM_ERROR			: 장애발생		모드
//			ATM_CLERK			: 계원			모드
//			ATM_READY			: 준비			모드
//			ATM_CUSTOM			: 고객대기중	모드 
//			ATM_TRAN			: 고객거래중	모드
/////////////////////////////////////////////////////////////////////////////
// 자동기모드제어
int CWinAtmCtrl::AtmModeCtrl()
{
	HWND hAviWnd;												// 광고제어
	if (m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", AtmStatusName[m_pDevCmn->AtmStatus]);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatusSave[%s]", AtmStatusName[m_pDevCmn->AtmStatusSave]);
	}


	// SCR EVENT가 오면 다른 상태체크에 앞서 거래로 빨리 진행하게 한다
	if ((m_pDevCmn->AtmStatus     == ATM_CUSTOM)	&&			//속도개선
		(m_pDevCmn->AtmStatusSave == ATM_CUSTOM)	&&
		(m_pTranCmn->TranStartByScr == DEV_SCR)		)
	{
		m_pDevCmn->AtmStatus = ATM_TRAN;
		m_pDevCmn->DeviceTran = DEV_SCR;
		m_pTranCmn->TranStartByScr = NULL;
	}

	switch (m_pDevCmn->AtmStatus) 
	{
		case ATM_INIT:											// 기기초기화 모드
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_INIT");
			m_pDevCmn->AtmStatusSave = ATM_INIT;				// 이전상태보관
			m_pDevCmn->fnAPL_CshRetactCntInfo(0);				// CDU Retract Count Reset

			m_pDevCmn->fnAPL_CheckPowerOffError(); 				// 파워오프장애검지
			m_pDevCmn->RecoveryType = 0;						// 0:자동복구,1:수동복구
			m_pDevCmn->DeviceStatus = m_pDevCmn->fnAPL_GetDefineDevice(DEV_MAIN);	
																// 전체장치복구
			m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetErrorDevice(m_pDevCmn->DeviceStatus), TRUE);
			
			m_pDevCmn->fnAPL_CheckMaterial();					// 매체잔류검지및잔류장애발생
			if (!m_pDevCmn->fnAPL_CheckError())					// 장애검지
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// 장애발생 모드
			else 
				m_pDevCmn->AtmStatus = ATM_CLERK;				// 계원 모드

			// 이후 로직에 poll 쓰레드 구동함. 
			m_pDevCmn->fnAPL_SetOnTransaction(OFF);

			break;
		
		case ATM_ERROR: 										// 장애발생 모드
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(!m_pDevCmn->AtmStatusReEntryFlag))				// Atm상태재진입Flag
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_ERROR");
				m_pDevCmn->AtmStatusReEntryFlag = TRUE;			// 재진입금지
				m_pDevCmn->fnAPL_UnUsed();						// 사용중지
				m_pDevCmn->fnAPL_DisplayError(ATM_ERROR, TRUE);	// 장애화면출력및인자
				if (m_pDevCmn->OpenKey)							// 운영모드시만 처리
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s] DeviceStatus[%x]", "ATM_ERROR", m_pDevCmn->DeviceStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s] AvailErrorDevice[%x]", "ATM_ERROR", m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus));

					if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) & (DEV_BRM | DEV_UCM))
					{
						// V02.05.03 2004.08.16 BRM/UCM등 시재관련이라하더라도 송수신장애시 자동복구
						if ((memcmp(&m_pDevCmn->FirstErrBuff.ErrorCode[0], "77", 2) == 0)		||	// 송신장애
							(memcmp(&m_pDevCmn->FirstErrBuff.ErrorCode[0], "88", 2) == 0)		)	// 수신장애
						{
							m_pDevCmn->fnAPL_PrintJnlInfo("[자동복구]-[송수신장애]", JNL_STATUS);
							m_pDevCmn->fnAPL_ClearError();		// 장애정보초기화
							m_pDevCmn->RecoveryType = 0;		// 0:자동복구,1:수동복구
							m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus));
																// 자동복구
						}
						else
						if (m_pTranCmn->TranProc == TRAN_DEP)	
						{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "입금거래 장애[%s]", &m_pDevCmn->FirstErrBuff.ErrorCode[0]);
						}
						else 
						{
							m_pDevCmn->fnAPL_PrintJnlInfo("[자동복구]-[BRM 초기화]", JNL_STATUS);
							m_pDevCmn->fnAPL_ClearError();		// 장애정보초기화
							m_pDevCmn->RecoveryType = 0;		// 0:자동복구,1:수동복구
							m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus));
						}
//						else ;									// 자동복구 하지 않음	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FirstErrorCode[%s]", &m_pDevCmn->FirstErrBuff.ErrorCode[0]);
					}
					else
					if (memcmp(&m_pDevCmn->FirstErrBuff.ErrorCode[0], "46999", 2) == 0)
					{											// V02-06-01-#10 46999(계수후 방출전 수표미검지)장애시
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[자동복구불가] ==================================================");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[자동복구불가] 수표부 센서이상(46999) 장애 : 복구시 시재사고 예상");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[자동복구불가] ==================================================");
					}
					else										// BRM,UCM제외(시재관련)
					{
						m_pDevCmn->fnAPL_PrintJnlInfo("[자동복구] - [초기화]", JNL_STATUS);
						m_pDevCmn->fnAPL_ClearError();			// 장애정보초기화
						m_pDevCmn->RecoveryType = 0;			// 0:자동복구,1:수동복구 //#0127
						m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) & (~(DEV_BRM | DEV_UCM | DEV_SPR)));
																// 자동복구
					}
				}
			}
			m_pDevCmn->AtmStatusSave = ATM_ERROR;				// 이전상태보관
			m_pDevCmn->fnAPL_AutoOffErrorDevice();				// 장애난장치자동축퇴
			m_pDevCmn->fnAPL_CheckMaterial();					// 매체잔류검지및잔류장애발생
			if (!m_pDevCmn->fnAPL_CheckError())					// 장애검지
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// 장애발생 모드
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm사용가능
				(m_pDevCmn->BrmChangeLoc == CHANGE_POS)		&&	// Brm탈착있음
				(m_pDevCmn->BrmLoc == NORMAL_POS)			&&	// Brm정위치
				(!m_pDevCmn->fnAPL_AutoResetBrm()))				// Brm자동복구
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm사용가능
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)		&&	// 회수함정위치상태
				(!m_pDevCmn->fnAPL_AutoReadyBrm()))				// Brm자동준비
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->ForceDownFlag)						// 강제Down변수 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->RemoteDownFlag)						// 강제Down변수 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			{
//				m_pDevCmn->OpenKey = TRUE;						// 운영모드설정
				m_pDevCmn->AtmStatus = ATM_CLERK;				// 계원 모드
			}

			if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)	// 상태가동일시유효
				m_pDevCmn->fnAPL_DisplayError(ATM_ERROR, FALSE);	// 장애화면출력및인자
			break;

		case ATM_CLERK: 										// 계원 모드
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(!m_pDevCmn->AtmStatusReEntryFlag))				// Atm상태재진입Flag
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_CLERK");
				m_pDevCmn->AtmStatusReEntryFlag = TRUE;			// 재진입금지
				m_pDevCmn->fnAPL_UnUsed();						// 사용중지
				m_pDevCmn->fnAPL_DisplayNormalSegment();		// 정상Segment출력
				m_pDevCmn->fnAPL_DisplayError(ATM_CLERK, TRUE);	// 사용중지화면출력및인자
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%d %d %d]", m_pDevCmn->ForceDownFlag, m_pDevCmn->RemoteDownFlag, m_pDevCmn->OpenKey);
//				m_pDevCmn->fnAPL_CloseHost();		 			// 호스트폐국
			}
			m_pDevCmn->AtmStatusSave = ATM_CLERK;				// 이전상태보관

			m_pDevCmn->fnAPL_CheckMaterial();					// 매체잔류검지및잔류장애발생
			if (!m_pDevCmn->fnAPL_CheckError())					// 장애검지
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// 장애발생 모드
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))				&&	// Brm사용가능
				(m_pDevCmn->BrmChangeLoc == CHANGE_POS)					&&	// Brm탈착있음
				(m_pDevCmn->BrmLoc == NORMAL_POS)						&&	// Brm정위치
				(!m_pDevCmn->fnAPL_AutoResetBrm()))							// Brm자동복구
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm사용가능
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)		&&	// 회수함정위치상태
				(!m_pDevCmn->fnAPL_AutoReadyBrm()))				// Brm자동준비
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->ForceDownFlag)						// 강제Down변수 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->RemoteDownFlag)						// 강제Down변수 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->fnAPL_BRMCstMatchInfo())				// U3100K - CST 점퍼 MissMatch -> //#0068
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (!m_pDevCmn->fnAPL_SprAutoload())				// 명세표 Autoload 수해
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->OpenKey)							// 운영키			
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else 
				m_pDevCmn->AtmStatus = ATM_READY;				// 준비 모드

			if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)	// 상태가동일시유효
				m_pDevCmn->fnAPL_DisplayError(ATM_CLERK, FALSE);	// 사용중지화면출력및인자

			if( (m_pTranCmn->DoorStatus != m_pDevCmn->DoorStatus) ||
				(m_pTranCmn->CabinetStatus != m_pDevCmn->CabinetStatus))	
			{
				if (m_pDevCmn->HostStatus == HOST_ONLINE)
				{
					try
					{
						m_pTranCmn->TranCode2 = TRANID_A020;			
						m_pTranCmn->fnAPP_SendHost();
					}
					catch (CTranCmnExit& e)
					{ 
						e.TranExitResult = e.TranExitResult;
					}
					m_pTranCmn->DoorStatus    = m_pDevCmn->DoorStatus;
					m_pTranCmn->CabinetStatus = m_pDevCmn->CabinetStatus;
				}
			}
			break;

		case ATM_READY:											// 준비 모드
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(!m_pDevCmn->AtmStatusReEntryFlag))				// Atm상태재진입Flag
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_READY");
				m_pDevCmn->AtmStatusReEntryFlag = TRUE;			// 재진입금지
				m_pDevCmn->fnAPL_UnUsed();						// 사용중지
				m_pDevCmn->fnAPL_DisplayNormalSegment();		// 정상Segment출력
				m_pDevCmn->fnAPL_DisplayError(ATM_READY, TRUE);	// 사용중지화면출력및인자
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%d %d %d]", m_pDevCmn->ForceDownFlag, m_pDevCmn->RemoteDownFlag, m_pDevCmn->OpenKey);

			}
			m_pDevCmn->AtmStatusSave = ATM_READY;				// 이전상태보관
			
			m_pDevCmn->fnAPL_CheckMaterial();					// 매체잔류검지및잔류장애발생
		
			if (!m_pDevCmn->fnAPL_CheckError())					// 장애검지
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// 장애발생 모드
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))				&&	// Brm사용가능
				(m_pDevCmn->BrmChangeLoc == CHANGE_POS)					&&	// Brm탈착있음
				(m_pDevCmn->BrmLoc == NORMAL_POS)						&&	// Brm정위치
				(!m_pDevCmn->fnAPL_AutoResetBrm()))							// Brm자동복구
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm사용가능
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)		&&	// 회수함정위치상태
				(m_pDevCmn->BrmLoc == NORMAL_POS)			&&	// Brm정위치 : V07-05-01-#01:공통장애 #26
				(!m_pDevCmn->fnAPL_AutoReadyBrm()))				// Brm자동준비
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->ForceDownFlag)						// 강제Down변수 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->RemoteDownFlag)						// 강제Down변수 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->fnAPL_BRMCstMatchInfo())				// U3100K - CST 점퍼 MissMatch -> //#0068
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (!m_pDevCmn->fnAPL_SprAutoload())				// 명세표 Autoload 수행
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->OpenKey)							// 운영키			
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if ((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL) ||	// 명세표Cover상태
				(m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)	 ||	// 저널Cover상태
//				(m_pDevCmn->A4pHeadStatus != HEADER_NORMAL)	 ||	// 저널Cover상태
//				(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)	 ||	// 명세표상태
				(m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)	 ||	// 저널상태
				(m_pDevCmn->BrmLoc != NORMAL_POS)			 ||	// Brm정위치상태
				(m_pDevCmn->RejectCSTLoc != NORMAL_POS)		 ||	// 회수함정위치상태
				(m_pDevCmn->UcmLoc != NORMAL_POS)			 ||	// Ucm정위치상태
//				(m_pDevCmn->DoorStatus == DOOR_OPENED)		 ||	// CabinetStatus
				(m_pDevCmn->CabinetStatus == DOOR_OPENED))		// DoorStatus상태
				m_pDevCmn->AtmStatus = ATM_READY;
// 			else
// 			if ((m_pDevCmn->fnA4P_GetDeviceStatus() != NORMAL)	||	// A4상태 
// 				(m_pDevCmn->fnA4P_GetPaperStatus())				)	// A4용지상태
// 				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pDevCmn->DoorStatus == DOOR_OPENED)	// Capture camera
			{
				m_pDevCmn->fnAPL_CaptureFaceCustomDoorOpen();   
				m_pDevCmn->AtmStatus = ATM_READY;
			}
			else
			if (!m_pDevCmn->fnAPL_OpenLine())					// 호스트라인검지
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostLine())				// 호스트라인검지
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostOpen())						// 호스트개국검지
				m_pDevCmn->AtmStatus = ATM_READY;			
//			else
//			if (!OpenProc())									// 호스트개국검지
//				m_pDevCmn->AtmStatus = ATM_READY;

			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM)) &&
				(!m_pDevCmn->fnAPL_AutoRefillBrm(m_pDevCmn->fnCSH_GetAutoloadCondition())))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_BRMCstSetInfo())				// U3100K - 5만원 고액권 -> #0070 -> #N0145
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->fnAPL_BRMCstSetInfo_Empty_On())		// U3100K - 추가현송 -> #N0145
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else 
			{
				m_pDevCmn->fnAPL_CaptureFaceCustomDoorOpen(TRUE); 
				m_pDevCmn->AtmStatus = ATM_CUSTOM;				// 고객대기중 모드
			}

			if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)	// 상태가동일시유효			
				m_pDevCmn->fnAPL_DisplayError(ATM_READY, FALSE);	// 사용중지화면출력및인자
			break;

		case ATM_CUSTOM:										// 고객대기중 모드
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(!m_pDevCmn->AtmStatusReEntryFlag))				// Atm상태재진입Flag
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_CUSTOM");
				m_pDevCmn->AtmStatusReEntryFlag = TRUE;			// 재진입금지
				m_pDevCmn->TranStatus = TRAN_IDLE;              //#0113

				hAviWnd = _FIND_CFMON;
				if (m_pProfile->DEVICE.MachineType == U3100K)
				{
					if ( hAviWnd )
						::PostMessage(hAviWnd, WUM_DUALCFDISP, 0, 0);	//end

					if (hAviWnd)
						::PostMessage(hAviWnd, WUM_SOUND_ON, 0, 0);	   //#0011					

				}
				else
				if (m_pProfile->DEVICE.MachineType == U8100)
				{					
					if (hAviWnd)
					{
						if (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET)
							::PostMessage(hAviWnd, WUM_DUALCFDISP, 0, 0);	// WParam = 0 : SHOW, 1: HIDE
						else
							::PostMessage(hAviWnd, WUM_PLAY_OFF, 0, 0);
					}
				}

				//  장애인ATM Start
				// 메인화면 Display전 Sensor(EarPhone) Clear 필수
				m_pDevCmn->EarPhoneInserted = FALSE;
				m_pDevCmn->fnSNS_ClearPhoneStatus();
				m_pDevCmn->fnSCR_SetDisplayData(EAR_KINDMODE, "off");

				if (m_pDevCmn->AtmDefine.Proximity == PROXIMITY_ON)
				{
					m_pDevCmn->ProximitySensor = FALSE;			// 근접센서 초기화
					m_pDevCmn->m_PSoundTime = ::GetTickCount();	// 근접센서 시간 초기화
				}
				//  장애인ATM End

				m_pDevCmn->fnAPL_BRMCstSetInfo();			    // U3100K - 5만원 고액권 -> #0070
				m_pDevCmn->fnAPL_BRMCstSetInfo_Empty_On();		// U3100K -> 추가현송 장애개선 -> #N0156
				m_pDevCmn->fnAPL_DisplayInformationLed(TRUE);	// 정보Led출력
				m_pDevCmn->fnAPL_DisplayInformationIndicator(TRUE);	
																// 정보Indicator출력
				m_pDevCmn->fnAPL_SetOnTransaction(OFF);			// 고객거래중아님
				m_pDevCmn->fnAPL_SetProcCount('0');				// 진행번호설정
				m_pDevCmn->fnSCR_SetCurrentLangMode(KOR_MODE);	// 현재언어모드설정
				m_pTranCmn->fnAPP_MainMenuDisplay(TRUE);		// 메인화면출력
				m_pTranCmn->TranStartByScr = NULL;				// 속도개선 초기화
				m_pDevCmn->fnAPL_DisplayInformationIndicator(TRUE);	// 적용후 1차 수정 (U8100-AP변경 #13) - 광고LCD의 정보를 Update하기 위해 여기에서 처리
				m_pDevCmn->fnAPL_CheckMaterial(DEV_BRM | DEV_UCM);	// 매체잔류검지및잔류장애발생
																// 82160 장애감소대책 - CUSTOM MODE에서는 최초한번만확인
			}
			m_pDevCmn->AtmStatusSave = ATM_CUSTOM;				// 이전상태보관

//			m_pDevCmn->fnAPL_CheckMaterial(DEV_BRM | DEV_UCM);	// 매체잔류검지및잔류장애발생
																// 82160 장애감소대책 - CUSTOM MODE에서는 최초한번만확인
			if ((m_pDevCmn->AtmStatus     == ATM_CUSTOM)	&&	// 변경내역 #06 - 속도개선 바로 거래단을 타도록 Return
				(m_pDevCmn->AtmStatusSave == ATM_CUSTOM)	&&
				(m_pTranCmn->TranStartByScr == DEV_SCR)		)
				return TRUE;
			else
			if (!m_pDevCmn->fnAPL_CheckError())					// 장애검지
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// 장애발생 모드
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm사용가능
				(m_pDevCmn->BrmChangeLoc == CHANGE_POS)		&&	// Brm탈착있음
				(m_pDevCmn->BrmLoc == NORMAL_POS)			&&	// Brm정위치
				(!m_pDevCmn->fnAPL_AutoResetBrm()))				// Brm자동복구
					m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm사용가능
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)		&&	// 회수함정위치상태
				(!m_pDevCmn->fnAPL_AutoReadyBrm()))				// Brm자동준비
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->fnAPL_SprAutoload())				// 명세표 Autoload 수행
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->ForceDownFlag)						// 강제Down변수 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->RemoteDownFlag)						// 강제Down변수 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (!m_pDevCmn->OpenKey)							// 운영키			
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if ((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL) ||	// 명세표Cover상태
				(m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)	 ||	// 저널Cover상태
//				(m_pDevCmn->A4pHeadStatus != HEADER_NORMAL)	 ||	// 저널Cover상태
//				(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)	 ||	// 명세표상태
				(m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)	 ||	// 저널상태
				(m_pDevCmn->BrmLoc != NORMAL_POS)			 ||	// Brm정위치상태
				(m_pDevCmn->RejectCSTLoc != NORMAL_POS)		 ||	// 회수함정위치상태
				(m_pDevCmn->UcmLoc != NORMAL_POS)			 ||	// Ucm정위치상태
				(m_pDevCmn->CabinetStatus == DOOR_OPENED)	 ||	// CabinetStatus
				(m_pDevCmn->DoorStatus == DOOR_OPENED))			// DoorStatus상태
				m_pDevCmn->AtmStatus = ATM_READY;
// 			else
// 			if ((m_pDevCmn->fnA4P_GetDeviceStatus() != NORMAL)	||	// A4상태 
// 				(m_pDevCmn->fnA4P_GetPaperStatus())				)	// A4용지상태
// 				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostLine())				// 호스트라인검지
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostOpen())				// 호스트개국검지
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pTranCmn->fnAPP_MainMenuDeviceEvent() != T_OK)// Event체크
				m_pDevCmn->AtmStatus = ATM_CUSTOM;
			else
			{
				if (m_pDevCmn->DeviceTran & DEV_PBM)			// 통장축퇴에 따른 처리로직 보완
				{
					if (m_pDevCmn->fnPBM_GetDeviceStatus() == DOWN)					// 97500 장애대책
					{
						m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PBM, DISABLE, TRUE);   // 97500장애발생시킴
						m_pDevCmn->DeviceStatus |= DEV_PBM;
						m_pDevCmn->AtmStatus = ATM_ERROR;                           // PBM_DOWN
					}
					else
						m_pDevCmn->AtmStatus = ATM_TRAN;		// 고객거래중
				}
				else
					m_pDevCmn->AtmStatus = ATM_TRAN;			// 고객거래중
			}

			// 장애인ATM Start
			if ((m_pProfile->DEVICE.MachineType == U8100) &&
				(m_pDevCmn->AtmDefine.Proximity == PROXIMITY_ON))
			{
				if (::GetTickCount() > m_pDevCmn->m_PSoundTime)
				{
					if (!m_pDevCmn->fnSNS_GetProximity())
					{
						if (m_pDevCmn->ProximitySensor == FALSE)// 안내를 한적이 없을 경우만
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "근접센서 감지");
																// 음성시간만큼
							m_pDevCmn->m_PSoundTime = ::GetTickCount() + 13 * 1000;
							m_pDevCmn->ProximitySensor = TRUE;	// 안내를 한적이 있음
							m_pDevCmn->fnSND_KeySound(_SOUND_PROXIMITY);
						}
					}
					else
						m_pDevCmn->ProximitySensor = FALSE;
				}
				else											// 안내음성 출력중 센서감지범위 벗어나면 소리끔
				{
					if (m_pDevCmn->fnSNS_GetProximity())
					{
						if (m_pDevCmn->ProximitySensor == TRUE)
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "근접센서 해제");

							m_pDevCmn->ProximitySensor = FALSE;
							m_pDevCmn->m_PSoundTime = ::GetTickCount();
							m_pDevCmn->fnSND_KeySound(_SOUND_NONE);
						}
					}
				}
			}

			// 장애인ATM End
			if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)	// 상태가동일시유효
			{
				m_pTranCmn->fnAPP_MainMenuDisplay(FALSE);		// 메인화면출력
				m_pTranCmn->fnAPP_RemoteProc();
			}
			break;

		case ATM_TRAN:											// 고객거래중

			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(!m_pDevCmn->AtmStatusReEntryFlag))				// Atm상태재진입Flag
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_TRAN");


				hAviWnd = _FIND_CFMON;
				if (m_pProfile->DEVICE.MachineType == U3100K)
				{
					if (hAviWnd)
						::PostMessage(hAviWnd, WUM_PLAY_ON, 1, 0);

					if (hAviWnd)
						::PostMessage(hAviWnd, WUM_SOUND_OFF, 0, 0); //#0011		
				}
				else
				if (m_pProfile->DEVICE.MachineType == U8100)
				{
					if ( hAviWnd )
						::PostMessage(hAviWnd, WUM_DUALCFDISP, 1, 0);

				}

				if ((m_pDevCmn->ProximitySensor)	&&						// JHD 근접센서 #N0251
					(m_pDevCmn->DeviceTran != DEV_SCR))						//  장애인ATM
					m_pDevCmn->fnSND_KeySound(_SOUND_NONE);
				
				m_pDevCmn->AtmStatusReEntryFlag = TRUE;			// 재진입금지
				m_pDevCmn->TranType = 1;						// 0:고객거래무,1:고객사용유(장애송신관련)

				// 전맹인 거래시 Flash에 정보 전달 및 센서 초기화
				if (m_pProfile->DEVICE.MachineType == U8100)	//  장애인ATM
				{					
					if (m_pProfile->DEVICE.EarHandler)
						m_pDevCmn->EarPhoneInserted = m_pDevCmn->fnSNS_GetEarPhoneStatus();
					
					if (m_pDevCmn->EarPhoneInserted)
						m_pDevCmn->fnSCR_SetDisplayData(EAR_KINDMODE, "on");
					else
					{
						m_pDevCmn->fnSNS_ClearPhoneStatus();
						m_pDevCmn->fnSCR_SetDisplayData(EAR_KINDMODE, "off");
					}
				}
				
				m_pDevCmn->fnAPL_SetTranStart();				// 거래시작설정 : 2003.11.21(속도개선:기대치-200ms)
				TranProc(TRAN_TRAN);							// 거래처리
				
				if (m_pDevCmn->EarPhoneInserted == TRUE)		//  장애인ATM
				{
					m_pDevCmn->EarPhoneInserted = FALSE;
					m_pDevCmn->fnSNS_ClearPhoneStatus();
					m_pDevCmn->fnSCR_SetDisplayData(EAR_KINDMODE, "off");
				}
				
				if (m_pDevCmn->SightHandiCapped)				// 장애인ATM
					m_pDevCmn->SightHandiCapped = FALSE;

				
				m_pTranCmn->TranStartByScr = NULL;				// U8100-AP변경
				m_pDevCmn->fnAPL_SetOnTransaction(OFF);			// 고객거래중아님
				m_pDevCmn->fnSCR_SetCurrentLangMode(KOR_MODE);	// 현재언어모드설정
				m_pDevCmn->fnAPL_SetSensorInfo();				// 센서정보설정
			}
			m_pDevCmn->AtmStatusSave = ATM_TRAN;				// 이전상태보관

			m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_DOING_TRAN, FALSE);
			m_pDevCmn->fnAPL_CheckMaterial();					// 매체잔류검지및잔류장애발생
			if (!m_pDevCmn->fnAPL_CheckError())					// 장애검지
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if((m_pDevCmn->m_nAtmsCommand == TRAN_MSG_EXCEPTION_798) ||
			   (m_pDevCmn->m_nAtmsCommand == TRAN_MSG_EXCEPTION_094) ||
			   (m_pDevCmn->m_nAtmsCommand == TRAN_MSG_EXCEPTION_845))
			{
				m_pDevCmn->HostFirstOpenFlag = FALSE;
				m_pDevCmn->HostStatus        = HOST_WAITONLINE;
				m_pDevCmn->AtmStatus         = ATM_READY;
			}
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// 장애발생 모드
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm사용가능
				(m_pDevCmn->BrmChangeLoc == CHANGE_POS)		&&	// Brm탈착있음
				(m_pDevCmn->BrmLoc == NORMAL_POS)			&&	// Brm정위치
				(!m_pDevCmn->fnAPL_AutoResetBrm()))				// Brm자동복구
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm사용가능
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)		&&	// 회수함정위치상태
				(m_pDevCmn->BrmLoc == NORMAL_POS)			&&	// Brm정위치 
				(!m_pDevCmn->fnAPL_AutoReadyBrm()))				// Brm자동준비
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->OpenKey)							// 운영키			
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))	&&  // V02-06-02-#04 AutoLoad수행조건 판단
				(m_pDevCmn->fnSPR_GetAutoloadCondition() != NORMAL))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL) ||	// 명세표Cover상태
				(m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)	 ||	// 저널Cover상태
//				(m_pDevCmn->A4pHeadStatus != HEADER_NORMAL)	 ||	// 저널Cover상태
//				(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)	 ||	// 명세표상태
				(m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)	 ||	// 저널상태
				(m_pDevCmn->BrmLoc != NORMAL_POS)			 ||	// Brm정위치상태
				(m_pDevCmn->RejectCSTLoc != NORMAL_POS)		 ||	// 회수함정위치상태
				(m_pDevCmn->UcmLoc != NORMAL_POS)			 ||	// Ucm정위치상태
				(m_pDevCmn->CabinetStatus == DOOR_OPENED)	 ||	// CabinetStatus
				(m_pDevCmn->DoorStatus == DOOR_OPENED))			// DoorStatus상태
				m_pDevCmn->AtmStatus = ATM_READY;
// 			else
// 			if ((m_pDevCmn->fnA4P_GetDeviceStatus() != NORMAL)	||	// A4상태 2007-05-17 Add
// 				(m_pDevCmn->fnA4P_GetPaperStatus())				)	// A4용지상태
// 				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostLine())				// 호스트라인검지
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostOpen())				// 호스트개국검지
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&
				(m_pDevCmn->fnCSH_GetAutoloadCondition() != NORMAL))
				m_pDevCmn->AtmStatus = ATM_READY;
			else 
			{
				m_pDevCmn->AtmStatus = ATM_CUSTOM;
			}
			break;
		case ATM_DOWN:											// Down Mode
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", AtmStatusName[m_pDevCmn->AtmStatus]);

			m_pDevCmn->AtmStatusSave = ATM_DOWN;
			break;
		default:
			if (m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)
			{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%x], AtmStatusSave[%x]", m_pDevCmn->AtmStatus, m_pDevCmn->AtmStatusSave);	// test!!!!!
//HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[AtmModeCtrl()]", (char *)&m_pDevCmn->AtmDefine, (sizeof(DEVICEINFO) + (sizeof(int) * 80)));
				m_pDevCmn->AtmStatusSave = m_pDevCmn->AtmStatus;
			}
			break;
	}




/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmStatusSave == ATM_CUSTOM)	&&
		(m_pTranCmn->TranStartByScr == DEV_SCR)		)
		return TRUE;


	if (m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)
	{
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
			m_pDevCmn->PBMInkStatus = m_pDevCmn->fnPBM_GetInkStatus(); 

		if (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET)		
			m_pDevCmn->fnPIN_EntryDisable();
	}


	if (m_pDevCmn->AtmStatusSave == ATM_CUSTOM)
	{
		if (m_pDevCmn->AtmStatus == ATM_TRAN)					// 스피드업관련
			;	//return TRUE;									// 공통장애대응 #COM07-03
		else
		{
			m_pDevCmn->fnAPL_CaptureSaveEnd();					// 촬영종료&저장처리
		}
	}

	if (m_pDevCmn->AtmStatusSave == ATM_TRAN)
	{
		if (m_pDevCmn->AtmStatus == ATM_CUSTOM)					// 스피드업관련
			;	//return TRUE;									// 공통장애대응 #COM07-03
		else
		{
			m_pDevCmn->fnAPL_CaptureSaveEnd();					// 촬영종료&저장처리
		}
	}

	
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->Om.SendRecvStatus = "OFF";						// 송수신중해제
	m_pDevCmn->Om.OperationStatus = "ON";						// 조작가능
	m_pDevCmn->fnAPL_SetSensorInfo();							// 센서정보설정
	m_pDevCmn->fnAPL_GetAvailTrans();							// 거래가능상태구하기
//////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT)	||
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT1) ||
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT2) ||
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT3) ||
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT4) ||
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT5) ||		
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT6))
	{
		if ((m_pDevCmn->AtmStatus == ATM_READY) || (m_pDevCmn->AtmStatus == ATM_CUSTOM) || (m_pDevCmn->AtmStatus == ATM_DOWN) || (m_pDevCmn->AtmStatus == ATM_ERROR))
		{
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT)		//새벽 (정상)
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅 - 새벽]"));
				m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (전원) POWER OFF]"));			
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(TRUE);
			}
			else
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT1)		//서버 (원격)
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅 - 다운로드 설치]"));
				m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (전원) POWER OFF]"));				
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(TRUE);
			}
			else
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT2)  		//통신장애
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅 - 통신 자동복구]"));
				m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (전원) POWER OFF]"));			
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(TRUE);
			}
			else
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT3)  	
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅 - 버튼 선택]"));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (전원) POWER OFF]"));			
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(FALSE);
			}
			else
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT4)  	
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅 - 계원 설정]"));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (전원) POWER OFF]"));			
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(FALSE);
			}
			else													
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT5)  	
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅 - AMS 원격 리부팅]"));		
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(FALSE);
			}
			else
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT6)  	
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅 - AMS 원격 파워오프]"));
				m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (전원) POWER OFF]"));
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(TRUE);
			}

			return TRUE;
		}
	}

	ReOpenUploadProc();
//////////////////////////////////////////////////////////////////////////	
	if (!m_pDevCmn->fnAPL_CheckPowerStatus())					
	{
		//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[STAND BY SW ON]"));			
//		m_pDevCmn->PowerOffSendFlag = TRUE;				// PowerOff송신처리
		TranProc(TRAN_STATUS);							// Nms처리	
		m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (전원) POWER OFF]"));			
		TerminateProcess();										
		return TRUE;
	}

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->AtmRestartInfo.bUseRestart)					
	{
		if ((m_pDevCmn->AtmStatus != ATM_READY)	&&
			(m_pDevCmn->AtmStatus != ATM_CUSTOM))
			;
		else
		{
			if ((GetTime().CompareNoCase(m_pDevCmn->AtmRestartInfo.strCheckStartTime.GetBuffer(0)) > 0)	&&
				(GetTime().CompareNoCase(m_pDevCmn->AtmRestartInfo.strCheckEndTime.GetBuffer(0)) < 0)	)
			{													
				if (m_pDevCmn->AtmRestartInfo.bReadyRestart)
				{
					;											
				}
				else
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SYSTEM RESTART[%s ~ %s]",
													m_pDevCmn->AtmRestartInfo.strCheckStartTime.GetBuffer(0),
													m_pDevCmn->AtmRestartInfo.strCheckEndTime.GetBuffer(0));

					RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", 0);	
					m_pDevCmn->AtmRestartInfo.bReadyRestart = 1;
					RegSetInt(_REGKEY_ATMRESTART, "ReadyRestart", m_pDevCmn->AtmRestartInfo.bReadyRestart);
					
					m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RESTART (%s ~ %s)]", m_pDevCmn->AtmRestartInfo.strCheckStartTime.GetBuffer(0), m_pDevCmn->AtmRestartInfo.strCheckEndTime.GetBuffer(0)));
					m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (전원) POWER OFF]"));				
					m_pDevCmn->m_nAtmsDoing = FALSE;
					m_pDevCmn->fnAPL_UnUsed();								
					m_pDevCmn->fnAPL_DisplayNormalSegment();			
					RestartProcess(TRUE);							
					return TRUE;
				}
			}
			else
			if (m_pDevCmn->AtmRestartInfo.bReadyRestart)		// NOT Check Time
			{
				m_pDevCmn->AtmRestartInfo.bReadyRestart = 0;
				RegSetInt(_REGKEY_ATMRESTART, "ReadyRestart", m_pDevCmn->AtmRestartInfo.bReadyRestart);
			}
		}

	}

/////////////////////////////////////////////////////////////////////////////
	if (!m_pDevCmn->AtmFirstDoingFlag)							// Atm최초실행Flag
	{
		m_pDevCmn->AtmFirstDoingFlag = TRUE;
		m_pTranCmn->fnAPP_NmsProc(TRUE);
        m_pDevCmn->fnAPL_ClerkInformation(TRUE);				// 계원정보
        m_pDevCmn->fnAPL_DisplayInformationLed(TRUE);			// 정보Led출력
        m_pDevCmn->fnAPL_DisplayInformationIndicator(TRUE);		// 정보Indicator출력
        m_pDevCmn->fnAPL_SendInformationRpl(TRUE);				// 정보Rpl전송
        m_pDevCmn->fnAPL_SendInformationInform(TRUE);			// 정보Inform전송

	}
	else
	{
		OpenProc();
		m_pTranCmn->fnAPP_TranVirtualTranReadyProc();
		m_pTranCmn->fnAPP_TranVirtualStatusProc();				// 거래상태
		m_pDevCmn->fnAPL_ClerkInformation();					// 계원정보
		m_pDevCmn->fnAPL_ClerkExecCheck();						// 계원실행검사
		m_pDevCmn->fnAPL_ClearDeviceExecOffInfo();				// 장치운영불가정보초기화
		m_pDevCmn->fnAPL_DisplayInformationLed();				// 정보Led출력
		m_pDevCmn->fnAPL_DisplayInformationIndicator();			// 정보Indicator출력
		m_pDevCmn->fnAPL_SendInformationRpl();					// 정보Rpl전송
		m_pDevCmn->fnAPL_SendInformationInform();				// 정보Inform전송

		m_pDevCmn->fnAPL_CaptureSaveEnd(FALSE);					// 촬영종료&저장처리

		if (!m_pDevCmn->fnAPL_CheckPrintDevicePosition())		// 장치정위치인자
			m_pDevCmn->AtmStatus = ATM_ERROR;
		if (!ClerkProc())										// 계원처리
			return FALSE;										// 종료처리
	}

	ULONG nWaitTime = TimerSet(1);
	while (!CheckTimer(nWaitTime))
	{
		if ((m_pDevCmn->AtmStatus == ATM_CUSTOM) || (m_pDevCmn->AtmStatus == ATM_TRAN))
		{
			
			if (m_pTranCmn->TranStartByScr == DEV_SCR)			// 화면 Event체크
				break;
			if (m_pDevCmn->AtmStatus == ATM_TRAN)
				break;
			if (m_pDevCmn->AtmStatusSave == ATM_TRAN)
				break;

			if (m_pTranCmn->fnAPP_MainMenuDeviceEvent() == T_OK)	// 카드나 통장 Event체크
			{
				if (m_pDevCmn->DeviceTran & DEV_PBM)				// U8100 AP 변경내역 #18 - 통장 내부통신 장애시 거래로 진입하는 경우가 있음
				{
					if (m_pDevCmn->fnPBM_GetDeviceStatus() == DOWN)
					{												// 97500 장애대책
						m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PBM, DISABLE, TRUE);
						// 97500장애발생시킴
						m_pDevCmn->DeviceStatus |= DEV_PBM;			// PBM 장애처리		// MWI 대책. V07-01-02-#0B
						m_pDevCmn->AtmStatus = ATM_ERROR;			// PBM_DOWN
					}
					else
						m_pDevCmn->AtmStatus = ATM_TRAN;			// 고객거래중
				}
				else
					// ────────────────────────────────
					m_pDevCmn->AtmStatus = ATM_TRAN;				// 고객거래중
				break;
			}
		}
		else
		{
			if (m_pDevCmn->AtmStatusSave == ATM_TRAN)
				break;
		}
		Delay_Msg();
	}

	return TRUE;
}

// 종료처리
int	CWinAtmCtrl::TerminateProcess()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TerminateProcess()");
	CString strOffT("");
	m_pDevCmn->fnSCR_DisplayString(1, "지금은 사용하실 수 없습니다.");
	m_pDevCmn->fnSCR_DisplayString(3, "전원을 끄는 중입니다");

	m_pDevCmn->fnSCR_DisplayScreen(701);						// 사용중지화면
	m_pDevCmn->fnAPL_UnUsed();									// 사용불가
	m_pDevCmn->fnAPL_ClerkInformation();						// 계원스위치상태설정

	if(m_pDevCmn->m_nAtmsCommand != ATMS_CMD_REBOOT2 && m_pDevCmn->fnAPL_CheckHostLine())
	{
		m_pTranCmn->TranCode2 = TRANID_A010;
		m_pTranCmn->fnAPP_SendHost();
	}



	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[POWER OFF: UPS BATTRY]"));
	strOffT.Format("%8.8s%6.6s", GetDate(), GetTime());
	RegSetStr(_REGKEY_DEVICE, "POWER_OFF_TIME", strOffT);		

	ULONG  WiatTime = TimerSet(K_10_WAIT);						// OM_종료10초대기
	while (TRUE)
	{
		Delay_Msg();											// Msg & Task
		if (!ClerkProc())										// OM_종료체크
			break;
		if (CheckTimer(WiatTime))
			break;
	}

	if (m_pDevCmn->fnUPS_GetDeviceStatus() == NORMAL)			// 정상시	2004.02.25
	{
		switch (m_pProfile->DEVICE.MachineType)					// 기종별 별도처리 2005.02.02
		{
		case U8100:
		case U3100K:
			m_pDevCmn->fnAPL_PowerOff(POWER_OFF_TIME);			// 전원Off(종료대기시간)
			break;
		default:
			m_pDevCmn->fnAPL_PowerOff(POWER_OFF_TIME);			// 전원Off(종료대기시간)
			break;
		}

																// V03-01-01 POWER_DOWN_GOOD CHECK
		WiatTime = TimerSet(K_10_WAIT);							// POWEROFF 10초대기
		while (TRUE)
		{
			Delay_Msg();										// Msg & Task

			if (m_pDevCmn->fnAPL_GetPowerStatus() & POWER_DOWN_GOOD)
				break;

			if (CheckTimer(WiatTime))
				break;
		}
	}

	m_pDevCmn->fnAPL_ClerkInitModeSet();						// 계원초기화모드설정
	KillTimer(TIMER_mSEC1000);									// TIMER_mSEC1000 중지
	m_pDevCmn->fnNET_Deinitialize(LINE_OPCL_TIME);				// 연결종료
	m_pDevCmn->AtmDoing = FALSE;								// Atm실행여부
	m_pDevCmn->fnAPL_UnLoadDevice(TRUE);						// 장치내리기	
	WindowsShutdownProcess(TRUE);								// 윈도우즈종료처리

	WinExec("C:\\T1atm\\App\\T1Clear.exe", SW_SHOW);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TerminateProcess():return");
	return TRUE;
}

int	CWinAtmCtrl::RestartProcess(int nReStartFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RestartProcess()");
	CString strOffT("");
	BOOL	bRebootFlag = FALSE;

	m_pDevCmn->fnSCR_DisplayString(1, "지금은 사용하실 수 없습니다.");
	m_pDevCmn->fnSCR_DisplayString(3, "전원을 끄는 중입니다");
	m_pDevCmn->fnSCR_DisplayScreen(701);						// 사용중지화면
	m_pDevCmn->fnAPL_UnUsed();									// 사용불가
	m_pDevCmn->fnAPL_ClerkInformation();						// 계원스위치상태설정


	if(m_pDevCmn->m_nAtmsCommand != ATMS_CMD_REBOOT2  && m_pDevCmn->fnAPL_CheckHostLine())
	{
		m_pTranCmn->TranCode2 = TRANID_A010;
		m_pTranCmn->fnAPP_SendHost();
	}
	//#0018 : K_10_WAIT -> K_3_WAIT 
	ULONG  WiatTime = TimerSet(K_3_WAIT);						// OM_종료10초대기
	while (TRUE)
	{
		Delay_Msg();											// Msg & Task
		if (!ClerkProc())										// OM_종료체크
			break;
		if (CheckTimer(WiatTime))
			break;
	}

	m_pDevCmn->fnAPL_ClerkInitModeSet();						// 계원초기화모드설정
	KillTimer(TIMER_mSEC1000);									// TIMER_mSEC1000 중지
	m_pDevCmn->fnNET_Deinitialize(LINE_OPCL_TIME);				// 연결종료
	m_pDevCmn->fnAPL_UnLoadDevice(TRUE);						// 장치내리기
	m_pDevCmn->AtmDoing = FALSE;								// Atm실행여부

	strOffT.Format("%8.8s%6.6s", GetDate(), GetTime());
	RegSetStr(_REGKEY_DEVICE, "POWER_OFF_TIME", strOffT);		
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[POWER_OFF_TIME : %s]", strOffT));	

	WindowsShutdownProcess(nReStartFlag);						// 윈도우즈종료처리
			
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RestartProcess():return");
	return TRUE;
}

// 윈도우즈종료처리
int	CWinAtmCtrl::WindowsShutdownProcess(int nReStartFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "WindowsShutdownProcess(nRebootFlag[%d])", nReStartFlag);

	HANDLE				hToken;
	LUID				Luid;
	BOOL				bReturn = FALSE;
	DWORD				dwError;
	TOKEN_PRIVILEGES	NewPrivileges;
	CString				strCMD("");


	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[POWER OFF : 리셋]"));
	RegSetInt(_REGKEY_ATMRESTART, "ATMRESTARTFlag", 0); //정상종료

	if (m_pProfile->DEVICE.MachineType == U8100 || m_pProfile->DEVICE.MachineType == U3100K)
	{
		if (nReStartFlag)
		{
			if(m_pDevCmn->fnLDCtrl_GetDeviceStatus() == NORMAL)
			{
				strCMD.Format("shutdown -s -f -t %d", 3);
				WinExecWaitEnd(strCMD, SW_HIDE);	
				return TRUE;
			}
			else
			{
				strCMD.Format("shutdown -r -f -t %d", 3);
				WinExecWaitEnd(strCMD, SW_HIDE);	
				return TRUE;
			}
		}
		else
		{
			strCMD.Format("shutdown -r -f -t %d", 3);
			WinExecWaitEnd(strCMD, SW_HIDE);	
			return TRUE;
		}
	}

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
	{
		dwError = GetLastError();
		if (dwError == ERROR_CALL_NOT_IMPLEMENTED) 
			return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &Luid)) 
	{
		CloseHandle(hToken);
		return FALSE;
	}

	NewPrivileges.PrivilegeCount			= 1;
	NewPrivileges.Privileges[0].Luid		= Luid;
	NewPrivileges.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED;

	bReturn = AdjustTokenPrivileges(hToken,
									FALSE,
									&NewPrivileges,
									0,
									NULL,
									NULL);

	CloseHandle(hToken);

	if (nReStartFlag)
	{
		if(m_pDevCmn->fnLDCtrl_GetDeviceStatus() == NORMAL)
		{
			::ExitWindowsEx(EWX_SHUTDOWN, (DWORD)-1);
		}
		else
		{
			::ExitWindowsEx(EWX_REBOOT, (DWORD)-1);
		}
	}
	else
		::ExitWindowsEx(EWX_SHUTDOWN, (DWORD)-1);

	return TRUE;
}

// 단체테스트준비
int	CWinAtmCtrl::ReadyDevTest()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ReadyDevTest()");
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[운영자 단체테스트]"));

	m_pDevCmn->fnAPL_ClearDeviceExecOffInfo(TRUE);				// 장치운영불가정보초기화
	
	m_pDevCmn->fnAPL_UnUsed();									// 사용불가

	m_pDevCmn->fnAPL_ClerkInitModeSet();						// 계원초기화모드설정
	KillTimer(TIMER_mSEC1000);									// TIMER_mSEC1000 중지
	m_pDevCmn->fnNET_Deinitialize(LINE_OPCL_TIME);				// 연결종료
	m_pDevCmn->fnAPL_UnLoadDevice();							// 장치내리기
	m_pDevCmn->AtmDoing = FALSE;								// Atm실행여부

	//#0075
	WinExecWaitEnd("C:\\T1ATM\\APP\\Main\\GOMA.EXE   ComMonitor4PNC.exe", SW_HIDE);
	if (m_pProfile->DEVICE.MachineType == U8500)		
	{
		WinExecWaitEnd("C:\\T1ATM\\APP\\Main\\GOMA.EXE NHCIM4HBRM.exe", SW_HIDE);
		WinExecWaitEnd("C:\\T1ATM\\APP\\Main\\GOMA.EXE NHCDM4HBRM.exe", SW_HIDE);
		WinExecWaitEnd("C:\\T1ATM\\APP\\Main\\GOMA.EXE ATMInside.exe", SW_HIDE); //#N0194
		
		WinExec("C:\\T1atm\\App\\Main\\ClearA.bat", SW_HIDE);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ReadyDevTest() -1");
		if (m_pDevCmn->BrmCashMode == BRM_REALCASH_MODE)
			WinExec("C:\\TM\\VDM\\vdm_ce.EXE", SW_SHOW);
		else
			WinExec("C:\\TM\\VDM\\vdm_ce.EXE", SW_SHOW);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ReadyDevTest() -2");
	}
	else
	if (m_pProfile->DEVICE.MachineType == U3100K)
		WinExec("C:\\TM\\VDM\\VDMT3.EXE", SW_SHOW);		    	// 진권CE용
	else
		WinExec("C:\\TM\\VDM\\tmfa.EXE", SW_SHOW);				// 위권공장용
	
	Delay_Msg(1000);
	WinExec("C:\\T1atm\\App\\T1Clear.exe", SW_HIDE);
	Delay_Msg(2000);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ReadyDevTest():return");
	return TRUE;
}




