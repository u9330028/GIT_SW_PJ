/////////////////////////////////////////////////////////////////////////////
// TranNms.cpp: implementation of the CTranCmn class.
// Desription : Nms 송수신 관련 함수군
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
//	Nms호출함수
/////////////////////////////////////////////////////////////////////////////
// Nms처리
int CTranCmn::fnAPP_NmsProc(int InitFlg)								
{
	int		Cnt = 0;
	int		i = 0;
	int		RetStatus = T_OK;
	int		ErrorFlg = FALSE;									// 장애Flg
	int		ErrorSendFlg = FALSE;								// 장애SendFlg
	int		StatusFlg = FALSE;									// 상태Flg
	static	CString	strReBootDate("");
	CString	strReBootTime("");
	CString	strReBootCode("");

	CString	szSaveTime("");
	CString strFileName, strTime;
	static	int nDvrflag = FALSE;


	if (InitFlg)		
	{
		strReBootDate = RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_REBOOT);

		ErrorSaveFlg = FALSE;									// 장애전문SaveFlg
		ErrorInDirectFlg = FALSE;								// 간접장애(고객,텔라,송수신등)
		ErrorAutoOffSave = 0;									// 이전자동축퇴값

		RecycleBoxStatus = CST_NORMAL;							//01:Brm현금카세트상태(정상,잔량소,요보충,잔량만)
		RejectCSTLoc = NORMAL_POS;								//02:Brm회수함상태(정상,잔량만,탈착)
		RejectCSTStatus = NORMAL_POS;							//02:Brm리젝트함상태(정상,잔량만,탈착) - CST_EMPTY, CST_SET_NG, CST_NEAR, CST_FULL
		RetractCSTStatus = NORMAL_POS;							//02:Brm회수함상태(정상,잔량만,탈착) - CST_EMPTY, CST_SET_NG, CST_NEAR, CST_FULL
		ClerkCSTLoc  = NORMAL_POS;								//03:Brm계원카세트상태(정상,탈착)
																//04:카세트정위치상태(Cd)
																//05:2카세트정위치상태(Cd)
		FstCSTStatus = NORMAL_POS;									//06:카세트정위치상태(Cd)
		SndCSTStatus = NORMAL_POS;									//07:카세트정위치상태(Cd)
		TrdCSTStatus = NORMAL_POS;									//07:카세트정위치상태(Cd)
		FthCSTStatus = NORMAL_POS;									//07:카세트정위치상태(Cd)
																
		CheckCSTStatus = CST_NORMAL;							//08::Ucm출금수표잔량상태(정상,잔량소,요보충)
		CheckCSTStatusSensor = CST_NORMAL;						//09:출금수표상태(실물)
		CheckCSTStatusReg = CST_NORMAL;							//10:출금수표상태(등록)
		CassetteStatus = CST_NORMAL;							//11:Ucm입금수표잔량상태(정상,잔량만)
		SlipHeadStatus = HEADER_NORMAL;							//12:명세표Cover상태
																//13:SPR Cover상태

		SlipStatus = SLIP_NORMAL;								//14:Spr현재명세표잔량상태(정상,잔량소,요보충)
		JnlStatus = JNL_NORMAL;									//15:Jpr저널1잔량상태(정상,잔량소,요보충)
		BrmLoc = NORMAL_POS	;									//16:Brm정위치(정상,탈착)
																//17:Brm정위치(예비1)
																//18:Brm정위치(예비2)
		UcmLoc = NORMAL_POS;									//19:Ucm정위치(정상,탈착)
		DoorStatus = DOOR_CLOSED;								//20:뒷문상태(닫힘,열림)
		CabinetStatus = DOOR_CLOSED;							//20:상단부상태(닫힘,열림)
																//21:DES
																//22:수표회수
																//23:CARD 회수
		                                                        //24:명세표회수
		                                                        //25:현금회수
		TmpTranAvail = TRAN_WITH_NOT;							//26:현금출금상태(가능,불가)
		                                                        //27:현금입금상태(가능,불가)
		                                                        //28:수표출금상태(가능,불가)
																//29:수표입금상태(가능,불가)
																//30:통장거래상태(가능,불가)
																//31:전자화폐상태(가능,불가)
																//32:해외거래상태(가능,불가)
																//33:지문거래상태(가능,불가)
																//34:적외선거래상태(가능,불가)
																//35:바코드거래상태(가능,불가)
																//36:암호화거래상태(가능,불가)
		A4pStatus = A4P_NORMAL;									//37:프린터상태
		RejectCSTStatus = CST_NORMAL;							//38:현금회수함의상태
		CameraStatus    = NORMAL_POS;				// 카메라상태

		GSensorStatus   = NORMAL_POS;				// 충격센서상태
		GSensorLelvel   = 0;						// 충격센서감도
		GBatteryStatus  = NORMAL_POS;

		CheckPbmInkStaus = PBMINK_NORMAL;			 // 적용후 1차 수정 (U8100-AP변경 #13) - IPBM INK 상태관련 추가

		GExternalSensorStatus1 = NORMAL_POS;            // 외부센서1
		GExternalSensorStatus2 = NORMAL_POS;            // 외부센서2

		TmpWithAvail = TRAN_WITH_NOT;
		TmpDepAvail  = TRAN_DEP_NOT;

		OpenKey = TRUE;
		CameraStatus = NORMAL_POS;
		ForceMissMatchDownFlag = FALSE;
		ForceSendStatus		   = STATUS_OFF_TIME;
		DeviceLoss = 0;

		memset(&NmsDataSendData, 0, sizeof(NmsDataSendData));	// 장애/상태전문버퍼
		memset(&NmsSaveDataSendData, 0, sizeof(NmsSaveDataSendData));


		return T_OK;
	}

	if ((memcmp(m_pDevCmn->PBMInkStatus, PBM_INK_OUT, m_pDevCmn->PBMInkStatus.GetLength()) == 0)		||
		(memcmp(m_pDevCmn->PBMInkStatus, PBM_INK_NOTSET, m_pDevCmn->PBMInkStatus.GetLength()) == 0))
	{
		m_pDevCmn->CheckPbmInkStaus = PBMINK_OUT;
	}
	else
		m_pDevCmn->CheckPbmInkStaus = PBMINK_NORMAL;

  
/////////////////////////////////////////////////////////////////////////////
	fnAPP_RecvDataClear();
/////////////////////////////////////////////////////////////////////////////
if (!m_pDevCmn->fnAPL_CheckError())
{
MsgDump(MAC_TRACE_DUMP_FILE("log"), "NMS_PROC m_pDevCmn->AtmStatusSave[%d], m_pDevCmn->TranType[%d]", m_pDevCmn->AtmStatusSave, m_pDevCmn->TranType);

}


if(m_pDevCmn->AtmStatus !=m_pDevCmn->AtmStatusSave)
{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "m_pDevCmn->AtmStatusSave[%d], m_pDevCmn->AtmStatus[%d] ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d] NmsErrorSendFlg[%d]",
	m_pDevCmn->AtmStatusSave, m_pDevCmn->AtmStatus, ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);

}

	if(ForceSendStatus == STATUS_FORCE_SEND)
	{ 
		fnAPP_NmsUpdateStatus();								
		return TRAN_TXRX_STATUS;
	}

	
	if(m_pDevCmn->GAlarmStatus != GAlarmStatus || m_pDevCmn->GBatteryStatus != GBatteryStatus)
	{ 
		fnAPP_NmsUpdateStatus();								
		return TRAN_TXRX_STATUS;
	}

	if(m_pDevCmn->ForceMissMatchDownFlag != ForceMissMatchDownFlag)
	{ 
		fnAPP_NmsUpdateStatus();								
		return TRAN_TXRX_STATUS;
	}


	if ((m_pDevCmn->AtmStatusSave == ATM_TRAN || m_pDevCmn->AtmStatusSave == ATM_CUSTOM) &&				// 이전고객거래중 (Van-CD 업무, 2009.10.23 "ATM_CUSTOM 장애전송")
		(!m_pDevCmn->fnAPL_CheckError())		&&				// 장애체크
		(m_pDevCmn->TranType == 1))								// 고객사용유(장애송신관련)
	{
		m_pDevCmn->TranType = 0;								// 0:고객거래무,1:고객사용유(장애송신관련)

		ErrorAutoOffSave = m_pDevCmn->DeviceAutoOff;			// 이전자동축퇴값
		ErrorFlg = TRUE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "1. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d], NmsErrorSendFlg[%d]",
	ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);
		
		if (NmsErrorSendFlg)
		{
			RetStatus = fnAPP_NmsUpdateStatus();	
			if (RetStatus == TRAN_TXRX_STATUS)
				return RetStatus;
			else 		
				return T_OK;
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "1-1. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d], NmsErrorSendFlg[%d]",
	ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);

	}
	else 
	if ((m_pDevCmn->AtmStatusSave == ATM_ERROR)	&&				// Cwc Send Check
		 (!m_pDevCmn->fnAPL_CheckError()) &&				// Error
		     (m_pDevCmn->TranType == 0))						// Customer Not Use
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "2. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d], NmsErrorSendFlg[%d]",
	ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);

		if (NmsErrorSendFlg)
		{
			RetStatus = fnAPP_NmsUpdateStatus();	
			if (RetStatus == TRAN_TXRX_STATUS)
				return RetStatus;
			else 		
				return T_OK;
		}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "2-1. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d], NmsErrorSendFlg[%d]",
	ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);
		ErrorSendFlg = TRUE;									// 장애SendFlg(기본전송모드)

	}
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_ERROR) &&				// 이전장애발생(자동복구처리후)
		(ErrorSaveFlg))											// 장애전문존재시
	{
		ErrorSaveFlg = FALSE;									// 장애전문재사용금지
		ErrorSendFlg = TRUE;									// 장애SendFlg(기본전송모드)
		if (ErrorInDirectFlg)									// 간접장애시송신처리
			;
		else
		if (ErrorAutoOffSave != m_pDevCmn->DeviceAutoOff)		// 자동축퇴값변화시송신처리
			;
		else
		if ((TranProc == TRAN_WITH)	||							// 입출금거래시(시재관련거래)
			(TranProc == TRAN_DEP)	)
		{
//			if ((m_pDevCmn->AtmStatus != ATM_ERROR)	&&			// 자동복구OK(현금잔류시는NG)
//				(!CancelWithProcFlg)	&&						// 지급취소처리무
//				(!CancelDepProcFlg)		)						// 입급취소처리무
//				ErrorSendFlg = FALSE;							// 장애송신금지처리
		}
		else													// 그외거래시
		{
			if (m_pDevCmn->AtmStatus != ATM_ERROR)				// 자동복구OK
				ErrorSendFlg = FALSE;							// 장애송신금지처리
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "3. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d]",
	ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag);

	}
	else
	if (m_pDevCmn->HostStatus == HOST_WAITONLINE)
	{
		return T_OK;
	}
	else
	if ((m_pDevCmn->AtmStatus == ATM_CUSTOM) &&
		(m_pDevCmn->AtmStatusSave == ATM_CUSTOM))
	{
		return T_OK;
	}
	else
	if ((m_pDevCmn->AtmStatus == ATM_TRAN) &&
		(m_pDevCmn->AtmStatusSave == ATM_TRAN))
	{
		return T_OK;
	}
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_CLERK) &&			// READY->개국->READY
		(m_pDevCmn->AtmStatus     == ATM_READY))
 		return T_OK;													
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_READY)		&&			// READY->개국->READY
		(m_pDevCmn->AtmStatus     == ATM_CUSTOM)	&&			// 현재상태 CUSTOM
		(m_pDevCmn->HostOpenFlag)					)			// 개국상태전문송신플래그
	{
		if ((BrmLoc != m_pDevCmn->BrmLoc)					            ||  // 정위치
			(DoorStatus != m_pDevCmn->DoorStatus)						||  // Door Open시 상태송신
			(CabinetStatus != m_pDevCmn->CabinetStatus)					||  // Cabinet Door Open시 상태송신
			(OpenKey      != m_pDevCmn->OpenKey))                           // 계원모드	
		{
			StatusFlg = TRUE;
		}
	}
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_CUSTOM) &&			// READY->개국->READY
		(m_pDevCmn->AtmStatus     == ATM_READY)	 &&			// 현재상태 CUSTOM
		(m_pDevCmn->HostOpenFlag)					)		// 개국상태전문송신플래그
		;													// 개국상태전문 송신처리
	else
	if (m_pDevCmn->PowerOffSendFlag)							// PowerOff전문처리
		;
	else
	if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)		// 상태가동일시유효
	{
		if ((FstCSTStatus != m_pDevCmn->FstCSTStatus)					||  // Rb1 
		    (SndCSTStatus != m_pDevCmn->SndCSTStatus)					||  // Rb2 
		    (TrdCSTStatus != m_pDevCmn->TrdCSTStatus)					||  // Rb3 
		    (FthCSTStatus != m_pDevCmn->FthCSTStatus)					||  // Rb3 
			(SlipHeadStatus != m_pDevCmn->SlipHeadStatus)				||	// 명세표cover상태
			(SlipStatus != m_pDevCmn->SlipStatus)						||	// 명세표상태
		    (RejectCSTStatus != m_pDevCmn->RejectCSTStatus)				||  // 리젝트박스
		    (RetractCSTStatus != m_pDevCmn->RetractCSTStatus))              // 회수박스
		{
			StatusFlg = TRUE;

// 			if (SndCSTStatus != m_pDevCmn->SndCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szSndSTStatus[m_pDevCmn->SndCSTStatus]);
// 			}
// 			else	
// 			if (FstCSTStatus != m_pDevCmn->FstCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szFstCSTStatus[m_pDevCmn->FstCSTStatus]);
// 			}
// 			else	
// 			if (TrdCSTStatus != m_pDevCmn->TrdCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szTrdCSTStatus[m_pDevCmn->TrdCSTStatus]);
// 			}
// 			else	
// 			if (FthCSTStatus != m_pDevCmn->FthCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szFthCSTStatus[m_pDevCmn->FthCSTStatus]);
// 			}
// 			else
// 			if (RetractCSTStatus != m_pDevCmn->RetractCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szRetractCSTStatus[m_pDevCmn->RetractCSTStatus]);
// 			}
// 			else
// 			if (SlipStatus != m_pDevCmn->SlipStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szSlipStatus[m_pDevCmn->SlipStatus]);
// 			}
// 			else
// 			if (RejectCSTStatus != m_pDevCmn->RejectCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szRejectCSTStatus[m_pDevCmn->RejectCSTStatus]);
// 			}
// 
// 			if (m_pDevCmn->OpenKey) 
// 				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "44. ErrorFlg");

		}
		else
			return T_OK;
	}
	else
	if (!m_pDevCmn->HostFirstOpenFlag)							// 개국 된적이 없으면 상태송신무 : 2002.10.26
	{
		return T_OK;											
	}
	else
	if ((RecycleBoxStatus != m_pDevCmn->RecycleBoxStatus)			||	// 현금상태다름
		(CheckCSTStatus != m_pDevCmn->CheckCSTStatus)				||	// 출금수표상태다름
		(CheckCSTStatusSensor != m_pDevCmn->CheckCSTStatusSensor)	||	// 출금수표상태다름(실물)
		(CheckCSTStatusReg != m_pDevCmn->CheckCSTStatusReg)			||	// 출금수표상태다름(등록)
		(CassetteStatus != m_pDevCmn->CassetteStatus)				||	// 입금수표상태다름
		(SlipHeadStatus != m_pDevCmn->SlipHeadStatus)				||	// 명세표 Cover상태
		(SlipStatus != m_pDevCmn->SlipStatus)						||	// 명세표상태
		(JnlStatus != m_pDevCmn->JnlStatus)							||	// 저널상태
		(A4pStatus != m_pDevCmn->A4pStatus)							||	// A4프린터상태
		(RejectCSTStatus != m_pDevCmn->RejectCSTStatus)				||  // 리젝트박스
		(RetractCSTStatus != m_pDevCmn->RetractCSTStatus)			||  // 회수박스
		(BrmLoc != m_pDevCmn->BrmLoc)								||  // 정위치 
		(FstCSTStatus != m_pDevCmn->FstCSTStatus)					||  // Rb1 
		(SndCSTStatus != m_pDevCmn->SndCSTStatus)					||  // Rb2 
		(TrdCSTStatus != m_pDevCmn->TrdCSTStatus)					||  // Rb3 
		(FthCSTStatus != m_pDevCmn->FthCSTStatus)					||  // Rb4 //#N0134 
	    (TmpTranAvail != m_pDevCmn->TranAvail)			    		||	// 거래상태 
		(DoorStatus != m_pDevCmn->DoorStatus)						||  // Door Open시 상태송신
		(CabinetStatus != m_pDevCmn->CabinetStatus)					||  // Cabinet Door Open시 상태송신
		(CameraStatus != m_pDevCmn->CameraStatus)					||  // 카메라상태 상태송신
		(OpenKey      != m_pDevCmn->OpenKey)                        ||  // 계원모드	
		(m_pDevCmn->DeviceRetract))										// 회수정보유
	{
		StatusFlg = TRUE;

		if (m_pDevCmn->OpenKey)
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "45. ErrorFlg");

		if (NmsErrorSendFlg)
		{
			RetStatus = fnAPP_NmsUpdateStatus();	
			if (RetStatus == TRAN_TXRX_STATUS)
				return RetStatus;
			else 		
				return T_OK;
		}
	}
	else
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	if ((ErrorFlg)		||										// 장애발생시
		(ErrorSendFlg)||										// 장애전문송신시
		(StatusFlg)		||										// 상태변경발생
		(m_pDevCmn->HostOpenFlag)||								// 개국상태전문송신시
		(m_pDevCmn->PowerOffSendFlag))							// PowerOff상태전문송신시
		;
	else
		return T_OK;

	if (m_pDevCmn->OpenKey) 
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "4. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d] NmsErrorSendFlg[%d]", ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);

/////////////////////////////////////////////////////////////////////////////

	if (ErrorFlg)												// 장애발생시
	{
		if (CancelWithProcFlg)									// 출금취소전문 송신시
		{
			;													// 장애처리 하도록 한다.
		}
		if (CancelDepProcFlg)									// 입금취소전문 송신시
		{														// 송금이나 입금시 시재관련전문을
			;													// 송신해야 한다.
		}														

		ErrorInDirectFlg = FALSE;								// 간접장애(고객,텔러,송수신등)

		int	RetId = m_pDevCmn->fnAPL_GetErrorCodeKind(m_pDevCmn->CurErrBuff.ErrorCode);
																// 0:미정의,1:기기장애,2:고객미숙,3:운영장애,4:전송장애
		if ((RetId == 2) || 									// 고객미숙
			(RetId == 3) || 									// 운영장애
			(RetId == 4))	 									// 전송장애
			ErrorInDirectFlg = TRUE;						

//		ErrorSaveFlg = TRUE;									// 장애전문발생시
		fnAPP_NmsIncDeviceCnt();								// 장치장애카운터 증가처리
		fnAPP_NmsUpdateStatus();								// 상태버퍼 작성
		NmsErrorSendFlg = TRUE;									// 장애Send유

		return TRAN_TXRX_ERROR;
	}
	else
	if (ErrorSendFlg)											// 장애전문송신시
	{															// 다발기기장애대책 : 2002.04.30
		fnAPP_NmsIncDeviceCnt();								// 장치장애카운터 증가처리
		fnAPP_NmsUpdateStatus();								// 상태버퍼 작성
		NmsErrorSendFlg = TRUE;									// 장애Send유

		return TRAN_TXRX_ERROR;

	}
	else
	if (m_pDevCmn->HostOpenFlag)								// "개국상태" 설정
	{
		m_pDevCmn->HostOpenFlag = FALSE;						// 개국상태전문 송신플래그 RESET
		fnAPP_NmsUpdateStatus();								// 상태버퍼 작성
		return TRAN_TXRX_STATUS;								
	}
	else
	if (StatusFlg)
	{
//		if (m_pDevCmn->DeviceRetract)							// 상태미처리 : 매체회수
//		{
//			m_pDevCmn->DeviceRetract = 0;
//			return T_OK;
//		}

//		if (JnlStatus != m_pDevCmn->JnlStatus)					// 상태미처리 : 저널용지 
//		{
//			JnlStatus = m_pDevCmn->JnlStatus;
//			return T_OK;
//		}

//		if (RejectCSTStatus != m_pDevCmn->RejectCSTStatus)
//		{														// 상태미처리 : 회수함상태 
//			RejectCSTStatus = m_pDevCmn->RejectCSTStatus;
//			return T_OK;
//		}
		//#N0152
		RetStatus = fnAPP_NmsUpdateStatus();	
		if (RetStatus == TRAN_TXRX_STATUS)
			return RetStatus;
		else 		
			return T_OK;
	}
	else
	if (m_pDevCmn->PowerOffSendFlag)								// "개국상태" 설정
	{
		m_pDevCmn->PowerOffSendFlag = FALSE;						// 개국상태전문 송신플래그 RESET

		fnAPP_NmsUpdateStatus();								// 상태버퍼 작성
		return TRAN_TXRX_STATUS;								
	}

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	거래 송수신 함수
/////////////////////////////////////////////////////////////////////////////
// Send Data
int CTranCmn::fnAPP_SendData(BYTE* SendBuff, int Length, int SendSec, int RecvBuffClearFlg)
{
	BYTE	TempBuff[NETBUFFSIZE];
	int		TempLength;
	CString	szService("");

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendData[SendBuff]", SendBuff, Length);
		return T_OK;											// SEND OK
	}
/////////////////////////////////////////////////////////////////////////////
//	fnAPP_GetAtmSysInfo(ATM_CPUMEMINFO);
/////////////////////////////////////////////////////////////////////////////
	if(TranCode2 != TRANID_0006 && TranCode2 != TRANID_A020)
	{
		m_pDevCmn->Om.SendRecvStatus = "ON";						// 송수신중설정
		m_pDevCmn->fnAPL_ClerkInformation();						// 계원정보
	}
/////////////////////////////////////////////////////////////////////////////

	if (RecvBuffClearFlg)
	{
		fnAPP_RecvDataClear();
	}

	memcpy(TempBuff, SendBuff, Length);							// 2002.08.28
	TempLength = Length;

	CString strSendData = MakeUnPack(TempBuff, TempLength);
	HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendData[SendBuff] UnpackData", 
		strSendData.GetBuffer(0), strSendData.GetLength());


//	fnAPP_SetPollTimeStart();

	if (m_pDevCmn->fnNET_SendData((LPCTSTR)strSendData, SendSec))
		return T_OK;
	else
		return T_TIMEOVER;
}

// Recv Data Clear
int CTranCmn::fnAPP_RecvDataClear()
{
	BYTE	ClearBuff[NETBUFFSIZE];
	int		ClearLength;

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
		return T_OK;											// RECV OK
/////////////////////////////////////////////////////////////////////////////
	while (fnAPP_RecvData(ClearBuff, &ClearLength, 0) != T_TIMEOVER)
		;
	return T_OK;
}
	
// Recv Data
int CTranCmn::fnAPP_RecvData(BYTE* RecvBuff, int* Length, int RecvSec)
{
	BYTE	TempBuff[NETBUFFSIZE];
	int		TempLength = 0;

	CString	strRecvData;
	strRecvData.Empty();

																// 수신BP_Header 데이타 길이
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
		Delay_Msg(1000);										// 2003.12.21
		return T_OK;											// RECV OK
	}
/////////////////////////////////////////////////////////////////////////////

	int nRecvResult = 0;
	do
	{
		nRecvResult = m_pDevCmn->fnNET_RecvData(strRecvData, RecvSec);
		if (RecvSec)
			MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[m_pDevCmn->fnNET_RecvData : %d]", nRecvResult);

		if (RecvSec)
			HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[Unpacked Recv Data]", strRecvData.GetBuffer(0) , strRecvData.GetLength());
		TempLength = MakePack(strRecvData.GetBuffer(0), TempBuff, strRecvData.GetLength());
		if (RecvSec)
			HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[Packed Recv Data]", TempBuff , TempLength);

		if (nRecvResult == FALSE)
		{
			*Length = 0;
			return T_TIMEOVER;
		}

		if (!TempLength)
		{
			*Length = 0;
			return T_RECVERROR;
		}

		memcpy(RecvBuff, TempBuff, TempLength);
		*Length = TempLength;

if ((RecvSec)									&&
	(m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE))
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[RecvBuff]", RecvBuff, *Length);

		if ((*Length) > 0)
			return T_OK;
	} while(TRUE);

	return T_OK;
}

// Recv In Tbl
int CTranCmn::fnAPP_RecvInTbl(BYTE* Buff, int Length)
{
	int		NmsRecvInTblTemp;

	NmsRecvInTblTemp = NmsRecvInTbl;
	NmsRecvInTblTemp++;
	if (NmsRecvInTblTemp >= NMSRECVTBL)							// In Tbl Temp Adj.
		NmsRecvInTblTemp = 0;

	if (NmsRecvInTblTemp != NmsRecvOutTbl)						// Data Tbl Ok
	{
		memcpy(NmsRecvBufferTbl[NmsRecvInTbl], Buff, Length);
		NmsRecvLengthTbl[NmsRecvInTbl] = Length;

		NmsRecvInTbl = NmsRecvInTblTemp;						// In Tbl Adj.
	}

	return T_OK;
}

// Recv Out Tbl
int CTranCmn::fnAPP_RecvOutTbl(BYTE* Buff, int* Length)
{
	if (NmsRecvInTbl == NmsRecvOutTbl)							// No Data
	{
		*Length = 0;
		return T_TIMEOVER;
	}

	memcpy(Buff, NmsRecvBufferTbl[NmsRecvOutTbl], NmsRecvLengthTbl[NmsRecvOutTbl]);
	*Length = NmsRecvLengthTbl[NmsRecvOutTbl];

	NmsRecvOutTbl++;
	if (NmsRecvOutTbl >= NMSRECVTBL)							// Out Tbl Adj.
		NmsRecvOutTbl = 0;

	if (!(*Length))
		return T_TIMEOVER;

	return T_OK;
}

// 장애 디바이스 카운터 증가
int CTranCmn::fnAPP_NmsIncDeviceCnt()
{
	int		DeviceBrmCnt = m_pProfile->TRANS.DeviceBrmCnt;	// 비교용보관 : 2002.04.26
	int		DeviceUcmCnt = m_pProfile->TRANS.DeviceUcmCnt;
	int		DevicePbmCnt = m_pProfile->TRANS.DevicePbmCnt;
	int		DeviceExecOff = m_pProfile->TRANS.DeviceExecOff;

	if (ErrorInDirectFlg)									// 간접장애(고객,텔라,송수신등)
		;
	else
	{
//		if ((NmsSaveDataSendData.ErrorMTCCode[0] == '4') && 
//			((NmsSaveDataSendData.ErrorMTCCode[1] == '4') ||
//			 (NmsSaveDataSendData.ErrorMTCCode[1] == '5') ||
//			 (NmsSaveDataSendData.ErrorMTCCode[1] == '6') ))
//			DeviceUcmCnt++;
//		else
//		if ((NmsSaveDataSendData.ErrorMTCCode[0] == '4') ||
//			((NmsSaveDataSendData.ErrorMTCCode[0] == '5')))
//			DeviceBrmCnt++;
//		else
//		if (NmsSaveDataSendData.ErrorMTCCode[0] == '6')
//			DevicePbmCnt++;
	}

	if (DeviceBrmCnt >= 4)
	{
		DeviceBrmCnt = 4;
		DeviceExecOff |= DEV_BRM;
	}
	else
		DeviceExecOff &= ~DEV_BRM;

	if (DeviceUcmCnt >= 3)
	{
		DeviceUcmCnt = 3;
		DeviceExecOff |= DEV_UCM;
	}
	else
		DeviceExecOff &= ~DEV_UCM;

	if (DevicePbmCnt >= 3)
	{
		DevicePbmCnt = 3;
		DeviceExecOff |= DEV_PBM;
	}
	else
		DeviceExecOff &= ~DEV_PBM;

	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)					// HOST LOCAL MODE(공장용제외) : 2002.08.20
	{
		DeviceBrmCnt = 0;
		DeviceUcmCnt = 0;
		DevicePbmCnt = 0;
		DeviceExecOff = 0;
	}

	if ((DeviceBrmCnt == m_pProfile->TRANS.DeviceBrmCnt)	&&	// 비교후저장
		(DeviceUcmCnt == m_pProfile->TRANS.DeviceUcmCnt)	&&
		(DevicePbmCnt == m_pProfile->TRANS.DevicePbmCnt)	&&
		(DeviceExecOff == m_pProfile->TRANS.DeviceExecOff));
	else
	{
		m_pProfile->GetTransProfile();
		m_pProfile->TRANS.DeviceBrmCnt = DeviceBrmCnt;			// 저장 
		m_pProfile->TRANS.DeviceUcmCnt = DeviceUcmCnt;
		m_pProfile->TRANS.DevicePbmCnt = DevicePbmCnt;
		m_pProfile->TRANS.DeviceExecOff = DeviceExecOff;
		m_pProfile->PutTransProfile();
	}

	return T_OK;
}



// 상태버퍼 작성
int CTranCmn::fnAPP_NmsUpdateStatus(BYTE* StatusBuff)
{	
	static BYTE	szStatusBuff[51];			
	int nGetInt = 0;
	memset(szStatusBuff, ' ', sizeof(szStatusBuff));

	char	szDoorSt[][256]       = {
		                           "[  :  :  뒷문 열림     ]",
		                           "[  :  :  뒷문 닫힘     ]"  };

	char	szCabinetSt[][256]   = {
		                           "[  :  :  상단부 열림    ]",
								   "[  :  :  상단부 닫힘    ]"       };
	
	char	szCameraSt[][256]   = {
		                           "[  :  :  연결부 정상    ]",
		                           "[  :  :  연결부 비정상  ]",
		                           "[  :  :  연결부 비정상  ]",
		                           "[  :  :  연결부 비정상  ]",
								   "[  :  :  연결부 비정상  ]"       };
	char	szFstCSTStatus[][256]   = {
		                           "[  :  :  1카세트 NORMAL     ]",
		                           "[  :  :  1카세트 CST_NEAR   ]",
		                           "[  :  :  1카세트 CST_EMPTY  ]",
		                           "[  :  :  1카세트 CST_SET_NG ]",
								   "[  :  :  1카세트 CST_FULL   ]"       };

	char	szSndSTStatus[][256]   = {
		                           "[  :  :  2카세트 NORMAL     ]",
		                           "[  :  :  2카세트 CST_NEAR   ]",
		                           "[  :  :  2카세트 CST_EMPTY  ]",
		                           "[  :  :  2카세트 CST_SET_NG ]",
								   "[  :  :  2카세트 CST_FULL   ]"       };

	char	szTrdCSTStatus[][256]   = {
		                           "[  :  :  3카세트 NORMAL     ]",
		                           "[  :  :  3카세트 CST_NEAR   ]",
		                           "[  :  :  3카세트 CST_EMPTY  ]",
		                           "[  :  :  3카세트 CST_SET_NG ]",
								   "[  :  :  3카세트 CST_FULL   ]"       };

	char	szFthCSTStatus[][256]   = {
		                           "[  :  :  4카세트 NORMAL     ]",
		                           "[  :  :  4카세트 CST_NEAR   ]",
		                           "[  :  :  4카세트 CST_EMPTY  ]",
		                           "[  :  :  4카세트 CST_SET_NG ]",
								   "[  :  :  4카세트 CST_FULL   ]"       };

	char	szRetractCSTStatus[][256]   = {
		                           "[  :  :  회수함 NORMAL     ]",
		                           "[  :  :  회수함 CST_NEAR   ]",
		                           "[  :  :  회수함 CST_EMPTY  ]",
		                           "[  :  :  회수함 CST_SET_NG ]",
								   "[  :  :  회수함 CST_FULL   ]"       };


	char	szRejectCSTStatus[][256]   = {
		                           "[  :  :  리젝트 NORMAL     ]",
		                           "[  :  :  리젝트 CST_NEAR   ]",
		                           "[  :  :  리젝트 CST_EMPTY  ]",
		                           "[  :  :  리젝트 CST_SET_NG ]",
								   "[  :  :  리젝트 CST_FULL   ]"       };

								   
	char	szBrmLocSt[][256]	   = {
		                          "[  :  :  출금부 정위치 ]",
								  "[  :  :  출금부 빠짐   ]" };								   
								   
	char	szCashStatus[][80] = {
		                          "[  :  :  현금 출금부 정상  ]",
								  "[  :  :  현금 출금부 잔량소]",
								  "[  :  :  현금 출금부 요보충]",
								  "[  :  :  현금 출금부 FULL  ]"  };

	char	szCheckStatus[][80] = {"[  :  :  수표 출금부 정상  ]",
								   "[  :  :  수표 출금부 잔량소]",
								   "[  :  :  수표 출금부 요보충]",
								   "[  :  :  수표 출금부 FULL  ]"
								};
	char	szCheckDepStatus[][80] = {"[  :  :  수표 입금부 정상  ]",
									  "[  :  :  수표 입금부 잔량소]",
									  "[  :  :  수표 입금부 요보충]",
									  "[  :  :  수표 입금부 FULL  ]"
									};

	char	szSlipHeadStatus[][80] = {"[  :  :  명세표 Cover 정상  ]",
								  "[  :  :  명세표 Cover Open]"
									};

	char	szSlipStatus[][80] = {"[  :  :  명세표 용지 정상  ]",
								  "[  :  :  명세표 용지 요보충]",
								  "[  :  :  명세표 용지 잔량소]"
								};
	char	szJnlStatus[][80] = {"[  :  :  저널 용지 정상  ]",
								 "[  :  :  저널 용지 요보충]",
								 "[  :  :  저널 용지 잔량소]"
								};
	char	szA4pStatus[][80] = {"[  :  :  A4프린터 용지 정상  ]",
								 "[  :  :  A4프린터 용지 요보충]",
								 "[  :  :  A4프린터 용지 잔량소]"
								};
	
	char	szGSensorStatus[][80] = {"[  :  :  GSonser 충격 감지]",
								     "[  :  :  GSonser 충격 정상]"
									};

	char	szOpMode[][80] = {
		                      "[  :  :  계원모드]",
							  "[  :  :  거래모드]"};

/////////////////////////////////////////////////////////////////////////////
	
	if(!OpenKey && (OpenKey == m_pDevCmn->OpenKey))
		return T_OK;

	Int2Asc( m_pDevCmn->RecycleBoxStatus                      ,&szStatusBuff[0] ,  1);
	Int2Asc( m_pDevCmn->RejectCSTLoc                          ,&szStatusBuff[1] ,  1);
	Int2Asc( m_pDevCmn->ClerkCSTLoc                           ,&szStatusBuff[2] ,  1);
	Int2Asc( m_pDevCmn->CheckCSTStatus                        ,&szStatusBuff[3] ,  1);
	Int2Asc( m_pDevCmn->CheckCSTStatusSensor                  ,&szStatusBuff[4] ,  1);
	Int2Asc( m_pDevCmn->CheckCSTStatusReg                     ,&szStatusBuff[5] ,  1);
	Int2Asc( m_pDevCmn->CassetteStatus                        ,&szStatusBuff[6] ,  1);
	Int2Asc( m_pDevCmn->SlipStatus                            ,&szStatusBuff[7] ,  1);
	Int2Asc( m_pDevCmn->JnlStatus                             ,&szStatusBuff[8] ,  1);
	Int2Asc( m_pDevCmn->FstCSTStatus						  ,&szStatusBuff[9] ,  1);
	Int2Asc( m_pDevCmn->SndCSTStatus		                  ,&szStatusBuff[10],  1);
	Int2Asc( m_pDevCmn->TrdCSTStatus		                  ,&szStatusBuff[11],  1);
	Int2Asc( m_pDevCmn->BrmLoc                                ,&szStatusBuff[12],  1);
	Int2Asc( m_pDevCmn->UcmLoc                                ,&szStatusBuff[13],  1);
	Int2Asc( m_pDevCmn->DoorStatus                            ,&szStatusBuff[14],  1);
	Int2Asc( m_pDevCmn->CabinetStatus                         ,&szStatusBuff[15],  1);
	Int2Asc( m_pDevCmn->TranAvail                             ,&szStatusBuff[16],  1);
	Int2Asc( m_pDevCmn->A4pStatus                             ,&szStatusBuff[17],  1);
	Int2Asc( m_pDevCmn->RejectCSTStatus                       ,&szStatusBuff[18],  1);
	Int2Asc( m_pDevCmn->RetractCSTStatus                      ,&szStatusBuff[19],  1);
	Int2Asc( m_pDevCmn->GAlarmStatus		                  ,&szStatusBuff[20],  1);
	Int2Asc( m_pDevCmn->GBatteryStatus		                  ,&szStatusBuff[21],  1);
	Int2Asc( m_pDevCmn->GExternalSensorStatus1	              ,&szStatusBuff[22],  1);
	Int2Asc( m_pDevCmn->GExternalSensorStatus2	              ,&szStatusBuff[23],  1);
	Int2Asc( m_pDevCmn->GSensorStatus		                  ,&szStatusBuff[24],  1);
	Int2Asc( m_pDevCmn->CheckPbmInkStaus	                  ,&szStatusBuff[25],  1);
	Int2Asc( m_pDevCmn->OpenKey                               ,&szStatusBuff[26],  1);
	Int2Asc( m_pDevCmn->CameraStatus                          ,&szStatusBuff[27],  1);
	Int2Asc( m_pDevCmn->ForceMissMatchDownFlag                ,&szStatusBuff[28],  1);
	Int2Asc( m_pDevCmn->FthCSTStatus		                  ,&szStatusBuff[29],  1);
	Int2Asc( m_pDevCmn->FthCSTStatus		                  ,&szStatusBuff[29],  1);
	Int2Asc( m_pDevCmn->TranAvail		                      ,&szStatusBuff[30],  1);
	Int2Asc( m_pDevCmn->SlipHeadStatus                        ,&szStatusBuff[31],  1);
//	Int2Asc( m_pDevCmn->DeviceLoss							  ,&szStatusBuff[32],  1);
	
	szStatusBuff[50] = 0x00;
	NmsSendBuffer[50] = 0x00;

	if(memcmp(NmsSendBuffer, szStatusBuff, sizeof(szStatusBuff)) == 0)
		return T_OK;

MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_NmsUpdateStatus NmsSendBuffer [%50.50s]",NmsSendBuffer);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "----------------------------------------------------------------------------------------");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecycleBoxStatus[%d][%d]",		RecycleBoxStatus,		m_pDevCmn->RecycleBoxStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RejectCSTLoc[%d][%d]",			RejectCSTLoc,			m_pDevCmn->RejectCSTLoc);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkCSTLoc[%d][%d]",			ClerkCSTLoc,			m_pDevCmn->ClerkCSTLoc);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CheckCSTStatus[%d][%d]",		CheckCSTStatus,			m_pDevCmn->CheckCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CheckCSTStatusSensor[%d][%d]",	CheckCSTStatusSensor,	m_pDevCmn->CheckCSTStatusSensor);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CheckCSTStatusReg[%d][%d]",	CheckCSTStatusReg,		m_pDevCmn->CheckCSTStatusReg);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CassetteStatus[%d][%d]",		CassetteStatus,			m_pDevCmn->CassetteStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SlipHeadStatus[%d][%d]",			SlipHeadStatus,				m_pDevCmn->SlipHeadStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SlipStatus[%d][%d]",			SlipStatus,				m_pDevCmn->SlipStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "JnlStatus[%d][%d]",			JnlStatus,				m_pDevCmn->JnlStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "BrmLoc[%d][%d]",				BrmLoc,					m_pDevCmn->BrmLoc);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "UcmLoc[%d][%d]",				UcmLoc,					m_pDevCmn->UcmLoc);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DoorStatus[%d][%d]",			DoorStatus,				m_pDevCmn->DoorStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CabinetStatus[%x][%x]",		CabinetStatus,			m_pDevCmn->CabinetStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TmpTranAvail[%x][%x]",			TmpTranAvail,			m_pDevCmn->TranAvail);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "A4pStatus[%d][%d]",			A4pStatus,				m_pDevCmn->A4pStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RejectCSTStatus[%d][%d]",		RejectCSTStatus,		m_pDevCmn->RejectCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RetractCSTStatus[%d][%d]",		RetractCSTStatus,		m_pDevCmn->RetractCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->DeviceRetract[%x]",	m_pDevCmn->DeviceRetract);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FstCSTStatus[%d][%d]",			FstCSTStatus,		m_pDevCmn->FstCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SndCSTStatus[%d][%d]",			SndCSTStatus,       m_pDevCmn->SndCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TrdCSTStatus[%d][%d]",			TrdCSTStatus,		m_pDevCmn->TrdCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FthCSTStatus[%d][%d]",			FthCSTStatus,       m_pDevCmn->FthCSTStatus);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GExternalSensorStatus1[%d][%d]",	    	GExternalSensorStatus1,		    m_pDevCmn->GExternalSensorStatus1); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GExternalSensorStatus2[%d][%d]",	    	GExternalSensorStatus2,		    m_pDevCmn->GExternalSensorStatus2); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GSensorStatus[%d][%d]",	    	        GSensorStatus,		    m_pDevCmn->GSensorStatus);  
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CheckPbmInkStaus[%d][%d]",	    	        CheckPbmInkStaus,		m_pDevCmn->CheckPbmInkStaus); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OpenKey[%d][%d]",	    	                OpenKey,	 	        m_pDevCmn->OpenKey); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CameraStatus[%d][%d]",	    	            CameraStatus,	 	        m_pDevCmn->CameraStatus); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ForceMissMatchDownFlag[%d][%d]",	    	ForceMissMatchDownFlag,	 	        m_pDevCmn->ForceMissMatchDownFlag); 
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DeviceLoss[%d][%d]",	                	DeviceLoss,	 	        m_pDevCmn->DeviceLoss); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "---------------------------------------------------------------------------------------");

	if (CameraStatus != m_pDevCmn->CameraStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szCameraSt[m_pDevCmn->CameraStatus]);
	}
	
	if (OpenKey != m_pDevCmn->OpenKey)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szOpMode[m_pDevCmn->OpenKey]);
	}
	else
	if (BrmLoc != m_pDevCmn->BrmLoc)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szBrmLocSt[m_pDevCmn->BrmLoc]);
	}
	else
	if (CameraStatus != m_pDevCmn->CameraStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szCameraSt[m_pDevCmn->CameraStatus]);
	}
	else	
	if (CabinetStatus != m_pDevCmn->CabinetStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szCabinetSt[m_pDevCmn->CabinetStatus]);
	}
	else	
	if (DoorStatus != m_pDevCmn->DoorStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szDoorSt[m_pDevCmn->DoorStatus]);
	}
	else	
	if (SndCSTStatus != m_pDevCmn->SndCSTStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szSndSTStatus[m_pDevCmn->SndCSTStatus]);
	}
	else	
	if (FstCSTStatus != m_pDevCmn->FstCSTStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szFstCSTStatus[m_pDevCmn->FstCSTStatus]);
	}
	else
	if (RetractCSTStatus != m_pDevCmn->RetractCSTStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szRetractCSTStatus[m_pDevCmn->RetractCSTStatus]);
	}
	else
	if (RejectCSTStatus != m_pDevCmn->RejectCSTStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szRejectCSTStatus[m_pDevCmn->RejectCSTStatus]);
	}
	else
	if (SlipHeadStatus != m_pDevCmn->SlipHeadStatus)
	{																// 명세표상태다름
		m_pDevCmn->fnAPL_PrintJnlInfo(szSlipHeadStatus[m_pDevCmn->SlipHeadStatus]);
	}
	else
	if (SlipStatus != m_pDevCmn->SlipStatus)
	{																// 명세표상태다름
		m_pDevCmn->fnAPL_PrintJnlInfo(szSlipStatus[m_pDevCmn->SlipStatus]);
	}
	else
	if (RecycleBoxStatus != m_pDevCmn->RecycleBoxStatus)
	{																// 현금상태다름
		m_pDevCmn->fnAPL_PrintJnlInfo(szCashStatus[m_pDevCmn->RecycleBoxStatus]);
	}
	else
	if (CheckCSTStatus != m_pDevCmn->CheckCSTStatus)
	{																// 출금수표상태다름
		m_pDevCmn->fnAPL_PrintJnlInfo(szCheckStatus[m_pDevCmn->CheckCSTStatus]);
	}
	else
	if (CheckCSTStatusSensor != m_pDevCmn->CheckCSTStatusSensor)
	{																// 출금수표상태다름(실물)
		m_pDevCmn->fnAPL_PrintJnlInfo(szCheckStatus[m_pDevCmn->CheckCSTStatusSensor]);
	}
	else
	if (CassetteStatus != m_pDevCmn->CassetteStatus)
	{																// 입금수표상태다름
		m_pDevCmn->fnAPL_PrintJnlInfo(szCheckDepStatus[m_pDevCmn->CassetteStatus]);
	}
	else
	if (JnlStatus != m_pDevCmn->JnlStatus)
	{																// 저널상태다름
		m_pDevCmn->fnAPL_PrintJnlInfo(szSlipStatus[m_pDevCmn->SlipStatus]);
	}
	else
	if (A4pStatus != m_pDevCmn->A4pStatus)
	{																// A4프린터상태다름
		m_pDevCmn->fnAPL_PrintJnlInfo(szA4pStatus[m_pDevCmn->A4pStatus]);
	}
// 	else
// 	if (DeviceLoss != m_pDevCmn->DeviceLoss)
// 	{																
// 		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("미수취 : %s", (m_pDevCmn->DeviceLoss == DEV_BRM) ? "현금" :
// 																(m_pDevCmn->DeviceLoss == DEV_MCU) ? "카드" : 
// 		                                                        (m_pDevCmn->DeviceLoss == (DEV_BRM | DEV_MCU)) ? "현금+카드" : "모름"));
// 	}


	RecycleBoxStatus     = 	m_pDevCmn->RecycleBoxStatus;
	RejectCSTLoc         =	m_pDevCmn->RejectCSTLoc;
	ClerkCSTLoc          =	m_pDevCmn->ClerkCSTLoc;
	CheckCSTStatus       =	m_pDevCmn->CheckCSTStatus;
	CheckCSTStatusSensor =	m_pDevCmn->CheckCSTStatusSensor;
	CheckCSTStatusReg    =	m_pDevCmn->CheckCSTStatusReg;
	CassetteStatus       =	m_pDevCmn->CassetteStatus;
	SlipHeadStatus       = 	m_pDevCmn->SlipHeadStatus;	
	SlipStatus           = 	m_pDevCmn->SlipStatus;
	JnlStatus            = 	m_pDevCmn->JnlStatus;
	FstCSTStatus		 = m_pDevCmn->FstCSTStatus;  // Rb1 
	SndCSTStatus		 = m_pDevCmn->SndCSTStatus;  // Rb2 
	TrdCSTStatus		 = m_pDevCmn->TrdCSTStatus;  // Rb3 	
	FthCSTStatus		 = m_pDevCmn->FthCSTStatus;  // Rb4 //#N0134	
	BrmLoc               =	m_pDevCmn->BrmLoc;	
	UcmLoc               =	m_pDevCmn->UcmLoc;	
	DoorStatus           =	m_pDevCmn->DoorStatus;	
	CabinetStatus        =	m_pDevCmn->CabinetStatus;	
	TmpTranAvail         =  m_pDevCmn->TranAvail;	
	A4pStatus            =  m_pDevCmn->A4pStatus;	
	RejectCSTStatus      =  m_pDevCmn->RejectCSTStatus;	
	RetractCSTStatus     =  m_pDevCmn->RetractCSTStatus;	
	GAlarmStatus		 = m_pDevCmn->GAlarmStatus;
	GBatteryStatus		 = m_pDevCmn->GBatteryStatus;
	GExternalSensorStatus1		 = m_pDevCmn->GExternalSensorStatus1;
	GExternalSensorStatus2		 = m_pDevCmn->GExternalSensorStatus2;
	GSensorStatus		 = m_pDevCmn->GSensorStatus;
	CheckPbmInkStaus	 = m_pDevCmn->CheckPbmInkStaus;		
	OpenKey              = m_pDevCmn->OpenKey;	
	CameraStatus         = m_pDevCmn->CameraStatus;
	ForceMissMatchDownFlag = m_pDevCmn->ForceMissMatchDownFlag;
	m_pDevCmn->DeviceRetract	= 0;						
//	m_pDevCmn->DeviceLoss	= m_pDevCmn->DeviceLoss;						

	memcpy(NmsSendBuffer, szStatusBuff, sizeof(szStatusBuff));
	return TRAN_TXRX_STATUS;
}

// 상태전문 작성시 설정/해제 구분 확인 (미사용)
int CTranCmn::fnAPP_NmsDistinctStatus()
{
	const int STATUS_SET = 1;
	const int STATUS_RESET = 2;

	int nStatus = 0;

	switch(RecycleBoxStatus)									// NmsDataSendData.CashEmpty : 출금만원권실물
	{
		case CST_NEAR			:								// 잔량소/NEAR END
			switch(m_pDevCmn->RecycleBoxStatus)
			{
			case CST_NEAR		:								
				break;
			case CST_EMPTY		:								
			case CST_SET_NG		:
				nStatus = STATUS_SET;
				break;
			case CST_FULL		:								
			case CST_NORMAL		:								
			case RB_ALREADY_FULL:
			default				:
				nStatus = STATUS_RESET;
				break;
			}
			break;
		case CST_EMPTY		:									// 요보충
		case CST_SET_NG		:									// 탈착
			switch(m_pDevCmn->RecycleBoxStatus)
			{
			case CST_NEAR		:								
				nStatus = STATUS_RESET;
				break;
			case CST_EMPTY		:								
			case CST_SET_NG		:
				break;
			case CST_FULL		:								
			case CST_NORMAL		:								
			case RB_ALREADY_FULL:
			default				:
				nStatus = STATUS_RESET;
				break;
			}
			break;
		case CST_FULL		:									// FULL
		case CST_NORMAL		:									// 정상
		case RB_ALREADY_FULL:									// ALREADY FULL
		default				:
			switch(m_pDevCmn->RecycleBoxStatus)
			{
			case CST_NEAR		:								
				nStatus = STATUS_SET;
				break;
			case CST_EMPTY		:								
			case CST_SET_NG		:
				nStatus = STATUS_SET;
				break;
			case CST_FULL		:								
			case CST_NORMAL		:								
			case RB_ALREADY_FULL:
			default				:
				break;
			}
			break;
	}
	if (nStatus)
		return nStatus;

	if (CheckCSTStatus == CST_EMPTY)							// NmsDataSendData.CheckEmpty : 수표실물
	{
		if (m_pDevCmn->CheckCSTStatusSensor != CST_EMPTY)
			nStatus = STATUS_RESET;
	}
	else 
	if (CheckCSTStatus != CST_EMPTY)
	{
		if (m_pDevCmn->CheckCSTStatusSensor == CST_EMPTY)
			nStatus = STATUS_SET;
	}
	if (nStatus)
		return nStatus;

	switch(SlipStatus)											// NmsDataSendData.SlipEmpty : 명세표용지
	{
		case SLIP_NORMAL		:								// 용지정상
			switch(m_pDevCmn->SlipStatus)
			{
				case SLIP_NORMAL		:						// 용지정상
					break;
				case SLIP_LOW_END		:						// 용지 잔량
					nStatus = STATUS_SET;
					break;
				case SLIP_EMPTY_PAPER	:						// 용지없음
				default					:						// 용지상태확인불가
					nStatus = STATUS_SET;
					break;
			}
			break;
		case SLIP_LOW_END		:								// 용지 잔량
			switch(m_pDevCmn->SlipStatus)
			{
				case SLIP_NORMAL		:						// 용지정상
					nStatus = STATUS_RESET;
					break;
				case SLIP_LOW_END		:						// 용지 잔량
					break;
				case SLIP_EMPTY_PAPER	:						// 용지없음
				default					:						// 용지상태확인불가
					nStatus = STATUS_SET;
					break;
			}
			break;
		case SLIP_EMPTY_PAPER	:								// 용지없음
		default					:								// 용지상태확인불가
			switch(m_pDevCmn->SlipStatus)
			{
				case SLIP_NORMAL		:						// 용지정상
					nStatus = STATUS_RESET;
					break;
				case SLIP_LOW_END		:						// 용지 잔량
					nStatus = STATUS_RESET;
					break;
				case SLIP_EMPTY_PAPER	:						// 용지없음
				default					:						// 용지상태확인불가
					break;
			}
			break;
	}
	if (nStatus)
		return nStatus;

	switch(RecycleBoxStatus)									// NmsDataSendData.CashDepFull : 입금만원권실물FULL
	{
		case CST_NEAR		:									// 잔량소/NEAR END
		case CST_EMPTY		:									// 요보충
			switch(m_pDevCmn->CassetteStatus)
			{
				case CST_NEAR		:							// 잔량소/NEAR FULL
				case CST_EMPTY		:							// 요보충
					break;
				case CST_SET_NG		:							// 탈착
				case CST_FULL		:							// FULL
					nStatus = STATUS_SET;
					break;
				case CST_NORMAL		:							// 정상
				case RB_ALREADY_FULL:							// ALREADY FULL
				default				:
					break;
			}
			break;
		case CST_SET_NG		:									// 탈착
		case CST_FULL		:									// FULL
			switch(m_pDevCmn->CassetteStatus)
			{
				case CST_NEAR		:							// 잔량소/NEAR FULL
				case CST_EMPTY		:							// 요보충
					nStatus = STATUS_RESET;
					break;
				case CST_SET_NG		:							// 탈착
				case CST_FULL		:							// FULL
					break;
				case CST_NORMAL		:							// 정상
				case RB_ALREADY_FULL:							// ALREADY FULL
				default				:
					nStatus = STATUS_RESET;
					break;
			}
			break;
		case CST_NORMAL		:									// 정상
		case RB_ALREADY_FULL:									// ALREADY FULL
		default				:
			switch(m_pDevCmn->CassetteStatus)
			{
				case CST_NEAR		:							// 잔량소/NEAR FULL
				case CST_EMPTY		:							// 요보충
					break;
				case CST_SET_NG		:							// 탈착
				case CST_FULL		:							// FULL
					nStatus = STATUS_SET;
					break;
				case CST_NORMAL		:							// 정상
				case RB_ALREADY_FULL:							// ALREADY FULL
				default				:
					break;
			}
			break;
	}
	if (nStatus)
		return nStatus;

	if (CassetteStatus == CST_FULL)								// NmsDataSendData.CheckDepFull : 입금수표실물FULL
	{
		if (m_pDevCmn->CassetteStatus != CST_FULL)
			nStatus = STATUS_RESET;
	}
	else
	if (CassetteStatus != CST_FULL)								// NmsDataSendData.CheckDepFull : 입금수표실물FULL
	{
		if (m_pDevCmn->CassetteStatus == CST_FULL)
			nStatus = STATUS_SET;
	}

	return nStatus;
}


int CTranCmn::fnAPP_SetPollTimeStart()
{
	time_t	CurrentTime;
	time(&CurrentTime);
	m_dwPollStartTime = CurrentTime;

	if(TranCode2 == TRANID_0006)
		m_pDevCmn->fnNET_ICMP_DeviceStatus();
	else
		m_pDevCmn->fnNET_ICMP_DeviceStatus(TRUE);


	return TRUE;
}

int CTranCmn::fnAPP_RemoteProc(int nFlag)
{

	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;
	int nRet = 0;
	CString strRecvData("");
	CString strGetBid("NORMAL");

	BYTE	szActData[NETBUFFSIZE];
	int		nActDataLength = 0;

	strGetBid = RegGetStr(_REGKEY_REMOTECTRL, "BID_RECV_RESULT", "NORMAL");

	if (strGetBid != OM_DO)
		return T_OK;

	if (!m_pDevCmn->OpenKey)						
		return T_OK;

	RegSetStr(_REGKEY_REMOTECTRL, "BID_RECV_RESULT", OM_NORMAL);
	
	memset(szActData, 0, sizeof(szActData));
	nActDataLength = m_pNetWork->GetRemoteData(szActData);

	if (nActDataLength <= 0)
		return T_OK;

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_RemoteControl  nActDataLength-1[%d]", nActDataLength);

	fnAPP_ClearArrDataField(RX_RECV_F);
	memcpy(&m_RXECASH.byTLen, szActData, nActDataLength);
	m_pNetWork->ClearRemoteData();
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_RemoteControl  nActDataLength-2[%d]", nActDataLength);

	fnAPP_RxECash_GetDataField_RecvProc();
	
	if(!fnAPP_RXGetFindArryDataField(x006X, strlen(x006X), strGetRxDataField, PACK_F))
	{
		MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPL_RemoteControl Data Error [%s]" , x006X);
		return T_OK;
	}

	if(strGetRxDataField.CompareNoCase("A040") != 0)
	{
		return T_OK;
	}




	if(!fnAPP_RXGetFindArryDataField(x051X, strlen(x051X), strGetRxDataField, PACK_F))
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_RemoteControl Data Error [%s]" , x051X);
		return T_OK;
	}
	else
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[강제수신  => 시작]"));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[강제수신  => %s]", strGetRxDataField));
		if(strGetRxDataField.CompareNoCase("001") == 0) //재 개국 요구
		{
//			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REOPEN;
			m_pDevCmn->HostFirstOpenFlag = FALSE;
			m_pDevCmn->HostStatus        = HOST_WAITONLINE;
			m_pDevCmn->AtmStatus         = ATM_READY;
			m_pDevCmn->RemoteDownFlag	 = FALSE;
		}
		else
		if(strGetRxDataField.CompareNoCase("002") == 0) //상태전문 요구
		{
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_RESTATUS;
			m_pDevCmn->AtmStatus = ATM_READY;
			m_pDevCmn->AtmStatusSave = ATM_READY;

			m_pDevCmn->fnSCR_DisplayString(2, "사용중지 상태 입니다");
			m_pDevCmn->fnSCR_DisplayString(3, "빠른 시간내에 복구예정이오니");
			m_pDevCmn->fnSCR_DisplayString(4, "잠시만 기다려 주십시오");
			m_pDevCmn->fnSCR_DisplayScreen(701);

			m_pDevCmn->TranResult = FALSE;						
			m_pDevCmn->TranStatus = TRAN_STATUS;		
			m_pDevCmn->TranStatusSave = m_pDevCmn->TranStatus;
			AddSerialFlg = FALSE;
			fnAPP_Tran_TxSend2(TRANID_A020, LINE_SEND_5_TIME);

		}
		else
		if(strGetRxDataField.CompareNoCase("003") == 0) //취소오류내역 요구
		{
			return T_OK;
			//m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;

		}
		else
		if(strGetRxDataField.CompareNoCase("004") == 0) //강제인증 
		{

			if(!fnAPP_RXGetFindArryDataField(x046X, strlen(x046X), strGetRxDataField1, PACK_F))
			{
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_RemoteControl Data Error [%s]" , x046X);
				return T_OK;
			}
			else
			{
				m_pDevCmn->AtmStatus = ATM_READY;
				m_pDevCmn->AtmStatusSave = ATM_READY;
				m_pDevCmn->fnSCR_DisplayString(2, "사용중지 상태 입니다");
				m_pDevCmn->fnSCR_DisplayString(3, "빠른 시간내에 복구예정이오니");
				m_pDevCmn->fnSCR_DisplayString(4, "잠시만 기다려 주십시오");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				fnAPP_TranReadyProc();                   //정리필요반듯이 (화면처리)
			}
//			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;

		}
		else
		if(strGetRxDataField.CompareNoCase("005") == 0) //폐국전문 요구
		{
			m_pDevCmn->AtmStatus = ATM_READY;
			m_pDevCmn->AtmStatusSave = ATM_READY;
			m_pDevCmn->RemoteDownFlag = TRUE;
//			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;

		}
		else
		if(strGetRxDataField.CompareNoCase("006") == 0) //단말기 ReBoot요구
		{
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;
		}
		else
		if(strGetRxDataField.CompareNoCase("007") == 0) //DnLoadFlag
		{
// 			m_pDevCmn->ForceDownFlag = TRUE;
			m_pDevCmn->fnSCR_DisplayString(2, "AP VERSION UP .....");
			m_pDevCmn->fnSCR_DisplayString(3, "빠른 시간내에 복구예정이오니");
			m_pDevCmn->fnSCR_DisplayString(4, "잠시만 기다려 주십시오");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(3000);
			nRet = m_pDevCmn->fnUTL_FTP_Download_File();
			
			if (nRet == T_ERROR)
			{
				m_pDevCmn->AtmStatus = ATM_READY;
			}
// 			else
// 			{
// 				m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;
// 			}
		}
		else
		if(strGetRxDataField.CompareNoCase("008") == 0) //거래로그파일 Up Load 요청
		{
			return T_OK;
		}

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[강제수신  => 끝]"));
	}
						
	return T_OK;
}
