/////////////////////////////////////////////////////////////////////////////
// TranOutput.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	종료함수
/////////////////////////////////////////////////////////////////////////////
// 취소입금반환함수(정상업무처리)
int	CTranCmn::fnAPP_CancelDepRejectProc(int CancelCode, char *pCancelMsg, char *pCancelMsg2, char *pCancelMsg3, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelDepRejectProc:"	\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlg[%d], "		\
													"m_pDevCmn->TranStatus[%d]",
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlg,
													 m_pDevCmn->TranStatus);




/////////////////////////////////////////////////////////////////////////////
//	입금반환처리
/////////////////////////////////////////////////////////////////////////////
	if (!CancelDepRejectFlg)									// 입금반환Flg
	{
		CancelDepRejectFlg = TRUE;	
		if (((TranProc == TRAN_DEP) || (TranCode == TC_HIPASS_CASH))	&&			//#N0270				
			(m_pDevCmn->fnAPL_CheckError()))					// 장애검지무
		{
			CancelDepRejectProcFlg = TRUE;						// 입금반환처리Flg
			// 현금수표먼저반환시지원코딩부분
			fnAPD_CheckDepositReject();							// 입금수표반환
			fnAPD_CashDepositReject();							// 입금현금반환
			fnAPD_MoneyOutCheck();								// 현금수표방출수취대기
		}
	}
	
	fnAPP_CancelProc(CancelCode, pCancelMsg, pCancelMsg2, pCancelMsg3, CancelScrNum);
	return T_CANCEL;
}

// 취소함수
int	CTranCmn::fnAPP_CancelProc(int CancelCode, char *pCancelMsg, char *pCancelMsg2, char *pCancelMsg3, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc:"				\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlg[%d], "		\
													"m_pDevCmn->TranStatus[%d],"	\
													"CancelScrNum[%d]",	
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlg,
													 m_pDevCmn->TranStatus,
													 CancelScrNum);

	int		DelaySec = 1;
	int		nBrmStepErr = DEV_NONE;
	int		nDeviceStatusSave = DEV_NONE;
	
	if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KEB_MODE)  // Cancel Msg 한글 표시 #N0174
		m_pDevCmn->fnSCR_SetCurrentLangMode(KOR_MODE);

/////////////////////////////////////////////////////////////////////////////
//	Main호출시
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->TranStatus == TRAN_IDLE)						// Return단계고려?
		return T_CANCEL;

/* //#0002
	if (m_pDevCmn->TranStatus == TRAN_STATUS)					// Return단계고려?
		return T_CANCEL;
*/
/////////////////////////////////////////////////////////////////////////////
//	기본금지처리
/////////////////////////////////////////////////////////////////////////////
	if (CancelProcNum == 0)										// 첫실행시만
	{
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);			// 동작완료처리 : 2003.11.06
		m_pDevCmn->fnCSH_CloseShutter();						// 닫기
/******************************************************************************************/
		if(m_pProfile->DEVICE.MachineType == U3100K) //#N0138
		{
			//예외처리 -> #N0138
			nDeviceStatusSave = m_pDevCmn->DeviceStatus;
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);			// 동작완료처리  //-> 한네트 특이 사양 검토
			
			if (m_pDevCmn->DeviceStatus != nDeviceStatusSave)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc - Old nDeviceStatusSave (%x), m_pDevCmn->DeviceStatus (%x)", nDeviceStatusSave, m_pDevCmn->DeviceStatus);		
				m_pDevCmn->DeviceStatus = nDeviceStatusSave;
			}

			if (!m_pDevCmn->fnBRM_GetMaterialInfo() & BRM_IN_CSM)
			{
				if (m_pDevCmn->BrmCsmIn & BRM_IN_CSM)
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc - Old BrmCsmIn (%d), fnBRM_GetMaterialInfo (%d)", m_pDevCmn->BrmCsmIn, m_pDevCmn->fnBRM_GetMaterialInfo());		
					CancelWithSendFlg = TRUE;
				}
			}
			m_pDevCmn->BrmCsmIn = FALSE;
		}
/******************************************************************************************/

		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_UCM);			// 동작완료처리
		m_pDevCmn->fnUCM_CloseShutter();						// 닫기
		m_pDevCmn->fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM, DISABLE);	// 장치투입모드설정 
		m_pDevCmn->fnLGT_SetFlicker(KU_ALL_FLICKER, FLICKER_OFF);		// Flicker설정(인덱스,값) 
	}

/////////////////////////////////////////////////////////////////////////////
// 현금/수표부 잔류정보 TRACE처리
/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_FREE_MODE, "Log", __FILE__, __LINE__, "CANCEL처리시 잔류확인[%2.2X:%2.2X:%2.2X]", 
													m_pDevCmn->fnCSH_GetMaterialInfo(), 
													m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT),
													m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW));
/////////////////////////////////////////////////////////////////////////////
//	고객거래외호출시처리
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->TranStatus != TRAN_TRAN)			// 거래처리아님
	{

		if ((!m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_MAIN)) &&// 사용하고장애난장치구하기
			(!m_pDevCmn->fnAPL_CheckError()))					// 장애검지유
			m_pDevCmn->fnAPL_ClearError();						// 장애정보초기화
	
		
		if (m_pDevCmn->TranStatus == TRAN_OPEN)
		{
			if (pCancelMsg)
				m_pDevCmn->fnSCR_DisplayStringArray(1, pCancelMsg,  "\n");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(DelaySec * 5000);

MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnSCR_DisplayStringArray1[%s]", pCancelMsg);
		}

		fnAPP_TerminateProc(T_CANCEL);							// 종료함수
	}

	
	if (m_pDevCmn->TranStatus == TRAN_TRAN)			// 거래처리아님
	{
		
		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) == DEV_CSH)
		{
			if(TranProc == TRAN_WITH)
			{
				nBrmStepErr = RegGetInt(_REGKEY_DEVICE, "BRM_WITH_STEP",0);
				if(nBrmStepErr == 1)
					fnAPP_WithErrSumProc(1); //계수장애
				else
				if(nBrmStepErr == 3)
					fnAPP_WithErrSumProc(2); //셔터장애

				RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP",0);
			}
			else
			if(TranProc == TRAN_DEP)
			{
				nBrmStepErr = RegGetInt(_REGKEY_DEVICE, "BRM_DEP_STEP",0);
				if(nBrmStepErr == 1)
					fnAPP_DepErrSumProc(1); //입금계수장애
				else
				if(nBrmStepErr == 2)
					fnAPP_DepErrSumProc(2); //입금수납장애

				RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP",0);
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	에러정보가져오기
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_CheckError();								// 장애검지
/////////////////////////////////////////////////////////////////////////////
//	취소처리횟수제한
/////////////////////////////////////////////////////////////////////////////
	switch (CancelProcNum)										// 취소함수N번실행변수
	{
		case	0:
			CancelProcNum = 1;
			CancelProcCount = m_pProfile->TRANS.ProcCount[0];	// 취소처리전진행번호 
			break;

		case	1:
			CancelProcNum = 2;
			fnAPP_CancelProc2(CancelCode, pCancelMsg, pCancelMsg2, pCancelMsg3, CancelScrNum);	
																// 취소함수2
			break;

		case	2:
			CancelProcNum = 3;
			fnAPP_CancelProc3(CancelCode, pCancelMsg, pCancelMsg2, pCancelMsg3, CancelScrNum);	
																// 취소함수3
			break;

		case	3:
			CancelProcNum = 4;
			fnAPP_CancelProc4(CancelCode, pCancelMsg, pCancelMsg2, pCancelMsg3, CancelScrNum);	
																// 취소함수4
			break;

		default:
			if (!m_pDevCmn->fnAPL_CheckError())					// 장애검지유
			{
				if (!CancelWithProcFlg)							// 취소출금처리Flg : 출금취소가 아닌경우만 처리
					fnAPP_ErrorTranProc();						// 거래장애정보설정
			}
			if (!m_pDevCmn->fnAPL_CheckError())					// 장애검지유
			{
				fnAPP_SendMakeManageSend(FALSE);				// 송신전문작성(관리부 Send)

			}

			if (   TranCode2 == TRANID_7210 || TranCode2 == TRANID_7220 || TranCode2 == TRANID_7260 
				|| TranCode2 == TRANID_7230 || TranCode2 == TRANID_7250 || TranCode2 == TRANID_7380 
				|| TranCode2 == TRANID_4370 || TranCode2 == TRANID_4390 || TranCode2 == TRANID_4310
				|| TranCode2 == TRANID_4330 || TranCode2 == TRANID_4350 || TranCode2 == TRANID_4320
				|| TranCode2 == TRANID_4340 || TranCode2 == TRANID_7200 || TranCode2 == TRANID_4410 || TranCode2 == TRANID_7530) 
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("END  :01OOOOOOOOOOOO<<=====<<=====<<=====<<<<"));
			}

			fnAPP_TerminateProc(T_CANCEL);						// 취소종료
			break;
	}

/////////////////////////////////////////////////////////////////////////////
//	수표정보초기화처리
/////////////////////////////////////////////////////////////////////////////
	if (!CancelCheckClearFlg)									// 수표정보초기화Flg
	{
		CancelCheckClearFlg = TRUE;
		if ((TranProc == TRAN_WITH)					&&			// 출금
			(Asc2Int(Accept.CheckMoney, 8) != 0)	&&			// 수표출금
			(TranSend)								&&			// 거래전송
			(!m_pDevCmn->fnAPL_CheckError()))					// 장애검지유
		{
			m_pDevCmn->fnAPL_SetNumberOfOutCheck(0, 0);			// 출금수표매수초기화
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	연속거래여부처리
/////////////////////////////////////////////////////////////////////////////
//	Accept.SerialCount = 0;
//	Accept.SerialTranFlag = FALSE;
/////////////////////////////////////////////////////////////////////////////
//	화면처리
/////////////////////////////////////////////////////////////////////////////
	if (((m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) && (m_pDevCmn->fnSPR_GetMaterialInfo()))	||	// 명세표존재
		((m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU)) && (m_pDevCmn->fnMCU_GetMaterialInfo()))	||	// 카드존재
		((m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM)) && (m_pDevCmn->fnPBM_GetMaterialInfo())))		// 통장존재
		DelaySec = 1;											// 매체존재시대기

	if (CancelScrNum)
	{
		if ((CancelScrNum == 763)	||							// 카드-해당거래불가
			(CancelScrNum == 765)	||							// 통장-해당거래불가
			(CancelScrNum == 764)	)							// 작외선-해당거래불가
		{
			m_pDevCmn->fnSCR_DisplayString(1, MenuTbl[MenuIndex].MenuScrName);
		}

		m_pDevCmn->fnSCR_DisplayScreen(CancelScrNum);			// 지정화면출력
		Delay_Msg(DelaySec * 3000);
	}
	else 
	{
		int		ScrNum = 701;									// 안내화면(5Line)
		
		// 장애인ATM 지원거래중 취소될 때는 아래 음성을 모두 Play 가능하도록 Delay를 늘린다.
		if (m_pDevCmn->EarPhoneInserted == TRUE)				// 장애인ATM
			DelaySec = 5;

		switch (CancelCode)
		{
			case	T_CANCEL:									// 취소선택
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "안      내");
					m_pDevCmn->fnSCR_DisplayString(2, "    거래가 취소 되었습니다");
					m_pDevCmn->fnSCR_DisplayString(4, "    처음부터 다시 하여 주십시오");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please try again");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);

				break;

			case	T_TIMEOVER:									// 시간초과
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "안      내");
					m_pDevCmn->fnSCR_DisplayString(2, "    시간이 초과 하였습니다");
					m_pDevCmn->fnSCR_DisplayString(4, "    처음부터 다시 하여 주십시오");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please try again");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);

				break;
			
			case	T_INPUTOVER:								// 입력횟수초과
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "안      내");
					m_pDevCmn->fnSCR_DisplayString(2, "    입력 횟수를 초과 하였습니다");
					m_pDevCmn->fnSCR_DisplayString(4, "    처음부터 다시 하여 주십시오");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please try again");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);

				break;
			
			case	T_INPUTERROR:								// 입력자료이상
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "안      내");
					m_pDevCmn->fnSCR_DisplayString(2, "    입력자료 이상 발생");
					m_pDevCmn->fnSCR_DisplayString(4, "    [상황실: 1577 - 4655] 문의하여 주십시오.");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please try again");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;
			
			case	T_ERROR:									// 장애발생(기기장애메세지)
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "안      내");
					m_pDevCmn->fnSCR_DisplayString(2, "    장애가 발생 하였습니다");
					m_pDevCmn->fnSCR_DisplayString(4, "    잔액을 확인하여 주십시오");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please check your account balance");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;

			case	T_SENDERROR:								// 2003.11.07
			case	T_RECVERROR:
//				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
//					m_pDevCmn->fnSCR_DisplayString(1, "안      내");
//				else
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
				
				if ((TranProc == TRAN_WITH)		||				// 출금
					(TranProc == TRAN_TRANS)	||				// 이체
					(TranProc == TRAN_DEP)		||				// 입금
					(TranProc == TRAN_HIPASS))					// 하이패스	#N0266
				{
					if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
						m_pDevCmn->fnSCR_DisplayString(3, "    잔액을 확인하여 주십시오");
					else
						m_pDevCmn->fnSCR_DisplayString(3, "Please check your account balance");
				}
				else 
				{
					if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
						m_pDevCmn->fnSCR_DisplayString(3, "    처리중 장애가 발생하였습니다");
					else
						m_pDevCmn->fnSCR_DisplayString(3, "Your transaction has been cancelled");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;
			
			case	T_PROGRAM:									// 프로그램이상장애발생(전산부문의)

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "안      내");
					m_pDevCmn->fnSCR_DisplayString(2, "    프로그램 이상 발생");
					m_pDevCmn->fnSCR_DisplayString(4, "    [상황실: 1577 - 4655] 문의하여 주십시오.");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please check your account balance");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				
				break;		
			case	T_MSG:										// 취소발생(메세지처리)
				if ((pCancelMsg)	||
					(pCancelMsg2)	||
					(pCancelMsg3))
				{
//					if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
//						m_pDevCmn->fnSCR_DisplayString(1, "안      내");
//					else
//						m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					if (pCancelMsg)
						m_pDevCmn->fnSCR_DisplayString(2, pCancelMsg);
					if (pCancelMsg2)
						m_pDevCmn->fnSCR_DisplayString(3, pCancelMsg2);
					if (pCancelMsg3)
						m_pDevCmn->fnSCR_DisplayString(4, pCancelMsg3);
					
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_5_WAIT);
					m_pDevCmn->fstrSCR_GetKeyString();						
//					m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
//					Delay_Msg(DelaySec * 5000);
				}
				break;
			case	T_DISMSG:	// 취소발생(메세지처리)
				if (pCancelMsg)
					m_pDevCmn->fnSCR_DisplayStringArray(1, pCancelMsg,  "\n");
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 5000);
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnSCR_DisplayStringArray1[%s]", pCancelMsg);
				break;
			case	T_HOSTMSG:
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnSCR_DisplayStringArray1[%s]", pCancelMsg2);
				if (pCancelMsg)
					m_pDevCmn->fnSCR_DisplayStringArray(1, pCancelMsg);
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(5000);
				break;

			default:
				break;
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	송신전전문작성
/////////////////////////////////////////////////////////////////////////////
//	if ((!TranSend)			&&									// 거래전송무 V01-00-00-#301
//		(!CancelDepSendFlg)	)
//	{
//		if (!AddSerialFlg)										// 일련번호증가무
//		{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc::입금거래중 거래전송무시 거래일련번호 증가처리");
//			AddSerialFlg = TRUE;								// 일련번호증가유
//			m_pDevCmn->fnAPL_AddSerialNo();						// 거래일련번호증가
//		}
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc::입금거래중 거래전송무시 송신전문 편집만 처리");
//		fnAPP_SendMake(FALSE);									// 송신전문작성만
// 	}
	
/////////////////////////////////////////////////////////////////////////////
//	매체인자및방출처리
/////////////////////////////////////////////////////////////////////////////
	if (!CancelPrintFlg)										// 취소인자Flg
	{
		CancelPrintFlg = TRUE;
		if ((!m_pDevCmn->fnAPL_CheckError())	||				// 장애검지유
			(!CancelKCashSendFlg)				||				// K-CASH장애
			((EMVReadFlg)						&&				// EMV Read
			 (TranSend))						)
		{
			fnAPD_CardEmbossScan();								// 카드엠보스스캔
			fnAPD_CardImprint(TRUE);							// 카드임프린트

// 지급성거래일 경우(CancelWithSendFlg : Set(FALSE)) 장애가 검지되더라도 이곳에서 장애명세표를 인자할 필요는 없다.
// 명세표의 인자는 Cancel-#3에서 그 처리를 하여도 문제가 없을 것으로 사료된다.
// 그러나 지급성이 아닌경우(CancelWithSendFlg : TRUE)에는 이곳에서 장애명세표를 인자하여야 한다.
// 좀더 신중한 검토를 필요로 한다.
// 입금취소플래그와 출금취소플래그를 같이 처리하여야 한다. 하나로 인해서 다른 하나가 영향받지 않도록...
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc::취소플래그[%d/%d]", CancelWithSendFlg, CancelDepSendFlg);
//			if (((TranProc == TRAN_WITH) && (!CancelWithSendFlg)) ||
//				((TranProc == TRAN_DEP)  && (!CancelDepSendFlg))  )
			if ((TranProc == TRAN_WITH) && (!CancelWithSendFlg))// V01-00-00-#301
			{
				;												// 출금취소/입금취소시 장애미인자
			}
//t			if ((CancelWithSendFlg)	||							// 취소출금전송Flg(초기:실행무)
//t				(CancelDepSendFlg)	)							// 취소입금전송Flg
			else
			{
				JnlPrintFlg = FALSE;							// 저널인자무
				fnAPP_PrintError(DEV_JPR);						// 장애인자(저널)

				if (SlipPrintFlg)								// 명세표인자유
				{
					fnAPD_MaterialRetract(DEV_SPR);				// 회수후장애명세표지원
					SlipPrintFlg = FALSE;						// 명세표인자무
					fnAPD_CheckDeviceAction(DEV_SPR);			// 동작완료검지&장애검지
				}

//2012.01.11 Kim.Gi.Jin
//				fnAPP_PrintError(DEV_SPR);
//				fnAPD_CheckDeviceAction(DEV_SPR);				
			}
		}
		else
		{
			JnlPrintFlg = FALSE;								// 저널인자무
			if (((TranProc == TRAN_DEP && TranProcSave != TRAN_E_TICKET )		&&					// 입금  #N0142 전자상품권 입금 취소 명세표 방출x
				 (!CancelDepPrintFlg)	))//	||				// 취소입금인자Flg(초기:실행무)
// 				((TranProc == TRAN_TRANS)	&&					// 송금
// 				 (TranSend)				)	)					// 수취조회
			{
				CancelDepPrintFlg = TRUE;
				fnAPD_CardEmbossScan();							// 카드엠보스스캔
				fnAPD_CardImprint(TRUE);						// 카드임프린트
																// 취소인자(저널)
				if (CashInCountFlg && AddSerialFlg) //#0021 -> #0059
				{
					fnAPP_PrintCancel(DEV_SPR);						// 취소인자(명세표)
					fnAPD_CheckDeviceAction(DEV_SPR);				// 동작완료검지&장애검지
				}
				fnAPP_PrintCancel(DEV_JPR); //#0059
			}
			else
			if (TranSend)										// 거래전송유
			{
				fnAPD_CardEmbossScan();							// 카드엠보스스캔
				fnAPD_CardImprint(TRUE);						// 카드임프린트
				fnAPP_PrintCancel(DEV_JPR);						// 취소인자(저널)
			}
		}
	}
	
//	if ((!CancelWithSendFlg)	||								// 취소출금전송Flg(초기:실행무)
//		(!CancelDepSendFlg)	)									// 취소입금전송Flg(초기:실행무)
	if (!CancelWithSendFlg)										// 취소출금전송Flg(초기:실행무) V01-00-00-#301
	{
		if (((TranProc == TRAN_WITH)				&&			// 출금
			 (TranSend)								&&			// 거래전송
			 (!m_pDevCmn->fnAPL_CheckError()))		||			// 장애검지유
			((TranProc == TRAN_WITH)				&&			// 출금
			 (EMVReadFlg)							&&
			 (TranSend))							)
		{
			if (SlipPrintFlg)									// 명세표인자유
			{
				fnAPD_MaterialRetract(DEV_SPR);					// 회수(고객1명세표)
				SlipPrintFlg = FALSE;							// 명세표인자무
				fnAPD_CheckDeviceAction(DEV_SPR);				// 동작완료검지&장애검지
			}
		}
//		else
//		if (((TranProc == TRAN_DEP)			||					// 입금	V01-00-00-#301
//			 (TranCode == TC_RFCASH))		&&					// 교통카드현금충전
//			(!m_pDevCmn->fnAPL_CheckError()))					// 장애검지유
//		{
//			if ((SlipPrintFlg)		&&							// 명세표인자유
//				(!CancelDepSendFlg)	)
//			{
//				fnAPD_MaterialRetract(DEV_SPR);					// 회수(고객1명세표)
//				SlipPrintFlg = FALSE;							// 명세표인자무
//				fnAPD_CheckDeviceAction(DEV_SPR);				// 동작완료검지&장애검지
//			}
//		}
	}
//	else														// 2004.02.16
//	if (!CancelKCashSendFlg)									// 복구거래Flg(초기:실행무)
//	{
//		if (SlipPrintFlg)										// 명세표인자유
//		{
//			fnAPD_MaterialRetract(DEV_SPR);						// 회수(고객1명세표)
//			SlipPrintFlg = FALSE;								// 명세표인자무
//			fnAPD_CheckDeviceAction(DEV_SPR);					// 동작완료검지&장애검지
//		}
//	}

	// 출금현금회수시지원코딩부분
	// 입금현금수납시지원코드이부분
	if (((TranProc == TRAN_DEP)		&&	// 입금송수신장애시
		 (!m_pDevCmn->fnAPL_CheckError())							&&	// 수납처리 : 주택사양 2002.04.30
		 ((memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "771", 3) == 0)	||
		  (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "88", 2) == 0)))	)
	{
		fnAPP_SumProc(2);										// 입금집계
		fnAPD_CheckDepositStore(TRUE);							// 수표입금수납
		fnAPD_CashDepositStore(TRUE);							// 현금입금수납
	}

	CStringArray strTempRetractArray;
	char	StrBuff[256] = {0,};
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc::미수취플래그[%d/%x/%5.5s]", TranProc, MenuAvail, m_pDevCmn->CurErrBuff.ErrorCode);
	if ((TranProc == TRAN_WITH)										&&
		(!m_pDevCmn->fnAPL_CheckError())							&&
		(memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "01102", 5) == 0)	&&
		((m_pDevCmn->AtmDefine.CashHandler == OKI_SET)				||
		 (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET)				||
		 (m_pDevCmn->AtmDefine.CashHandler == BRM_SET))				)
	{
//		fnAPD_MoneyRetract();
		m_pDevCmn->DeviceAutoOff = 0;							// 자동축퇴가 난 Device = Set
		m_pDevCmn->DeviceLoss = DEV_NONE;						// 분실정보초기화
		m_pDevCmn->ForceDownFlag = FALSE;						// 강제Down변수 

		if (m_pDevCmn->fnCSH_GetMaterialInfo())					// 매체잔류정보구하기
		{
			if (m_pProfile->DEVICE.MachineType == U8100)
				m_pDevCmn->fnBRM_Reset(TRUE); 
			else
			{
				m_pDevCmn->fnCSH_Initialize();
			}
			
		}
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			// 동작대기및장애검지
		Delay_Msg(2000);										// 검지대기(2000ms)
																// 회수내역대기
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			// 동작대기및장애검지

		if (Asc2Int(Accept.Money, 8))
		{
			SplitString(m_pDevCmn->fstrCSH_GetRetractResult(), ",", strTempRetractArray);

			GetTime(StrBuff);
			sprintf(ShuData[0], "%2.2s:%2.2s:%2.2s", &StrBuff[0], &StrBuff[2], &StrBuff[4]);
			sprintf(ShuData[1], "[미수취 회수내역]");
			sprintf(ShuData[2], "미수취금액  :%d", Asc2Int(Accept.Money, 8));
			sprintf(ShuData[3], " (현금:%d,수표:%d)", Asc2Int(Accept.CashMoney, 8), Asc2Int(Accept.CheckMoney, 8));
			sprintf(ShuData[4], "회수된내역  :%d", Asc2Int(strTempRetractArray[0]));
			sprintf(ShuData[5], "총계수매수  :%d", Asc2Int(strTempRetractArray[1]));
			sprintf(ShuData[6], "만원권매수  :%d", Asc2Int(strTempRetractArray[2]));
			sprintf(ShuData[7], "오천원권매수:%d", Asc2Int(strTempRetractArray[3]));
			sprintf(ShuData[8], "천원권매수  :%d", Asc2Int(strTempRetractArray[4]));
			sprintf(ShuData[9], "불명권매수  :%d", Asc2Int(strTempRetractArray[5]));


			if (Asc2Int(Accept.Money, 8) == Asc2Int(strTempRetractArray[0]))
			{
				m_pDevCmn->ForceDownFlag = FALSE;
				sprintf(ShuData[10], "미수취 회수내역 정상임");
			}
			else
			{
				m_pDevCmn->ForceDownFlag = TRUE;
				sprintf(ShuData[10], "미수취 회수내역 불일치!!");
			}
			fnAPD_JnlPrint(TRUE);
		}
	}

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}

// 취소함수2
int	CTranCmn::fnAPP_CancelProc2(int CancelCode, char *pCancelMsg, char *pCancelMsg2, char *pCancelMsg3, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc2:"			\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlg[%d], "		\
													"CancelWithSendFlg[%d], "			\
													"m_pDevCmn->TranStatus[%d]",
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlg,
													 CancelWithSendFlg,
													 m_pDevCmn->TranStatus);

/////////////////////////////////////////////////////////////////////////////
//	취소출금처리
/////////////////////////////////////////////////////////////////////////////
	if (!CancelWithSendFlg)										// 취소출금전송Flg(초기:실행무)
	{
		CancelWithSendFlg = TRUE;
		
		if ((TranProc == TRAN_WITH ) &&			  // 출금
			(TranSend)				 &&		      // 거래전송
			(!m_pDevCmn->fnAPL_CheckError())) // 장애검지유
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc2-2");
			CancelWithProcFlg = TRUE;							// 취소출금처리Flg
			m_EMVCancelFlg = FALSE;
			SlipPrintFlg = FALSE;								// 명세표인자유무
			JnlPrintFlg = FALSE;								// 저널인자유무

			AddSerialFlg = FALSE;								// 일련번호증가Flag초기화
			m_pDevCmn->fnAPL_SetProcCount('3');					// 진행설정
			fnAPP_SendHostCancel();								// 송신(취소출금)
			fnAPP_SumProc(0);									// 취소출금집계
			fnAPP_RecvHost();									// 수신
			if (m_pDevCmn->TranResult)							// HOST OK
				m_pDevCmn->fnAPL_SetProcCount('5');				// 진행설정
			else
				m_pDevCmn->fnAPL_SetProcCount('9');			    // HOST NG

			fnAPP_ErrorCancelProc();
/////////////////////////////////////////////////////////////////////////////
// 취소처리3에서 저널및 명세표를 재인자처리한다.
// 따라서 이곳에서의 명세/저널인자는 굳이 필요없다.
// 따라서 명세/저널 인자를 취소함수3에 처리를 위임하고 이곳에서의 처리를 막는다.
// 이곳에서는 단지 호스트메시지 출력만을 수행한다.
/////////////////////////////////////////////////////////////////////////////
//t			fnAPP_PSPProc(DEV_JPR);								// 저널프린트
//			fnAPP_PMDProc();									// 메세지표시
//			if (PMDRecvData.ParaLength)							// DATA부/메세지표시(거래)
//				Delay_Msg(2*1000);
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				m_pDevCmn->fnSCR_DisplayString(1, "   출금취소가 정상처리되었습니다");
				m_pDevCmn->fnSCR_DisplayString(3, "   잔액을 확인하여 주십시오");
				m_pDevCmn->fnSCR_DisplayScreen(701);			// 안내화면(5Line)
			}
			else
			{
				m_pDevCmn->fnSCR_DisplayString(1, "   출금취소가 불능처리되었습니다");
				m_pDevCmn->fnSCR_DisplayString(3, "   [상황실: 1577 - 4655] 문의하여 주십시오.");
				m_pDevCmn->fnSCR_DisplayScreen(701);			// 안내화면(5Line)
			}
			Delay_Msg(3*1000);

//t			fnAPP_PSPProc(DEV_SPR);								// 명세프린트
//t			fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR);			// 동작완료검지&장애검지
		}
	}
	else
	if (!CancelDepSendFlg)										// 취소입금전송Flg(초기:실행무) V01-00-00-#301
	{
		CancelDepSendFlg = TRUE;
		
		if ((TranProc == TRAN_DEP)					&&			// 입금
			(!m_pDevCmn->fnAPL_CheckError()))					// 장애검지유
		{
			CancelDepProcFlg = TRUE;							// 취소입금처리Flg

//			SlipPrintFlg = FALSE;								// 명세표인자유무
//			JnlPrintFlg = FALSE;								// 저널인자유무
//
//			m_pDevCmn->fnAPL_SetProcCount('3');					// 진행설정
//			fnAPP_SendHostCancel();								// 송신(취소입금)
//			fnAPP_RecvHost();									// 수신
//			if (m_pDevCmn->TranResult)							// HOST OK
//				m_pDevCmn->fnAPL_SetProcCount('5');				// 진행설정
//			else m_pDevCmn->fnAPL_SetProcCount('9');			// HOST NG
//
////t			fnAPP_PSPProc(DEV_JPR);								// 저널프린트
//			fnAPP_PMDProc();									// 메세지표시
//			if (PMDRecvData.ParaLength)							// DATA부/메세지표시(거래)
//				Delay_Msg(2*1000);
////			if (m_pDevCmn->TranResult)							// HOST OK
////			{
////				//m_pDevCmn->fnSCR_DisplayString(2, "   입금취소가 정상처리되었습니다");
////				m_pDevCmn->fnSCR_DisplayString(1, "   잔액을 확인하여 주십시오");
////				m_pDevCmn->fnSCR_DisplayScreen(710);			// 안내화면(5Line)
////			}
////			else
////			{
////				//m_pDevCmn->fnSCR_DisplayString(2, "   입금취소가 불능처리되었습니다");
////				m_pDevCmn->fnSCR_DisplayString(2, "   [상황실: 1577 - 4655] 문의하여 주십시오.");
////				m_pDevCmn->fnSCR_DisplayScreen(710);			// 안내화면(5Line)
////			}
////			Delay_Msg(3*1000);
//
////t			fnAPP_PSPProc(DEV_SPR);								// 명세프린트
////t			fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR);			// 동작완료검지&장애검지
		}
	}
						

	if (CancelHIPassLoadFlg)									//#N0266
	{
		CancelHIPassLoadFlg = FALSE;							// 하이패스 거래실패시 Complete 처리Flag #0439
		fnAPP_HPSComplete();									// 하이패스 거래실패시 Complete 처리Flag #0439
		//fnAPP_HIPASS_SendErrorComplet();
	}
	
//	else														// 2004.02.16
//	if (!CancelKCashSendFlg)									// 복구거래Flg(초기:실행무) V01-00-00-#301
//	{
//		CancelKCashSendFlg = TRUE;
//		
//		fnAPP_KCashRemoveTranBit();								// 복구거래수행
//	}

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}
	
// 취소함수3
int	CTranCmn::fnAPP_CancelProc3(int CancelCode, char *pCancelMsg, char *pCancelMsg2, char *pCancelMsg3, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc3:"			\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlg[%d], "		\
													"CancelWithProcFlg[%d], "		\
													"m_pDevCmn->TranStatus[%d]",	
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlg,
													 CancelWithProcFlg,
													 m_pDevCmn->TranStatus);

/////////////////////////////////////////////////////////////////////////////
//	취소출금후처리
/////////////////////////////////////////////////////////////////////////////
	if (CancelWithProcFlg)										// 취소거래
	{
		JnlPrintFlg = FALSE;									// 저널인자무
		fnAPP_PrintWithCancelTran(DEV_JPR);
																// 취소거래인자(저널)
		if (SlipPrintFlg)										// 명세표인자유
		{
			fnAPD_MaterialRetract(DEV_SPR);						// 회수후장애명세표지원
			SlipPrintFlg = FALSE;								// 명세표인자무
			fnAPD_CheckDeviceAction(DEV_SPR);					// 동작완료검지&장애검지
		}
		MaterialOutFlg &= ~DEV_SPR;								// 취소인자 및 방출처리
		fnAPP_PrintWithCancelTran(DEV_SPR);
																// 취소거래인자(명세표)
	}

	if (CancelDepProcFlg)										// 취소거래
	{
		JnlPrintFlg = FALSE;									// 저널인자무
		fnAPP_PrintDepCancelTran(DEV_JPR);
																// 취소거래인자(저널)
		if (SlipPrintFlg)										// 명세표인자유
		{
			fnAPD_MaterialRetract(DEV_SPR);						// 회수후장애명세표지원
			SlipPrintFlg = FALSE;								// 명세표인자무
			fnAPD_CheckDeviceAction(DEV_SPR);					// 동작완료검지&장애검지
		}
		MaterialOutFlg &= ~DEV_SPR;								// 취소인자 및 방출처리
		fnAPP_PrintDepCancelTran(DEV_SPR);
																// 취소거래인자(명세표)
	}

	if (CancelDepRejectProcFlg)									// 입금반환처리
	{
		// 동시방출시지원코딩부분
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// 매체방출(고객1명세표)
		fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM);	// 매체방출수취대기회수
	}
	else
	{
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM | DEV_ICM);				// 매체방출(고객1명세표)
		fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM | DEV_ICM);	// 매체방출수취대기회수
	}

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}
	
// 취소함수4
int	CTranCmn::fnAPP_CancelProc4(int CancelCode, char *pCancelMsg, char *pCancelMsg2, char *pCancelMsg3, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc4:"			\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlg[%d], "		\
													"m_pDevCmn->TranStatus[%d]",
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlg,
													 m_pDevCmn->TranStatus);

/////////////////////////////////////////////////////////////////////////////
//	매체회수처리
/////////////////////////////////////////////////////////////////////////////
	if (CancelDepRejectProcFlg)									// 입금반환처리
	{
		if (((m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) && (m_pDevCmn->fnSPR_GetMaterialInfo())) ||	// 명세표존재
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU)) && (m_pDevCmn->fnMCU_GetMaterialInfo())) ||	// 카드존재
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM)) && (m_pDevCmn->fnPBM_GetMaterialInfo())))	// 통장존재
			fnAPD_MaterialRetract(DEV_SPR | DEV_MCU | DEV_PBM);								// 매체회수
		// 동시방출시지원코딩부분
	}
	else
	{
		if (((m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) && (m_pDevCmn->fnSPR_GetMaterialInfo())) ||	// 명세표존재
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU)) && (m_pDevCmn->fnMCU_GetMaterialInfo())) ||	// 카드존재
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM)) && (m_pDevCmn->fnPBM_GetMaterialInfo())))	// 통장존재
			fnAPD_MaterialRetract(DEV_SPR | DEV_MCU | DEV_PBM);								// 매체회수
	}

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}

// 종료함수
int	CTranCmn::fnAPP_TerminateProc(int TerminateCode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateProc(TerminateCode[%d])", TerminateCode);

	int		ChkID = DEV_MAIN;

	switch (TerminateCode)										// 장애인ATM
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateProc : EarPhoneInserted[%d], TerminateCode[%d]", m_pDevCmn->EarPhoneInserted, TerminateCode);

		case	T_OK:
			if (m_pDevCmn->TranStatus == TRAN_TRAN)
			{
				if (m_pDevCmn->EarPhoneInserted == TRUE)
				{
					while(TRUE)
					{
						m_pDevCmn->fnSCR_DisplayString(1, "거래를 종료하시려면");
						m_pDevCmn->fnSCR_DisplayString(2, "이어폰을 뽑아서 가져가시기 바랍니다.");
						m_pDevCmn->fnSCR_DisplayString(3, "이용해 주셔서 감사합니다.");
						m_pDevCmn->fnSCR_DisplayScreen(701);
						Delay_Msg(5000);							// 최종멘트 출력되지 않음 대책
						m_pDevCmn->EarPhoneInserted = m_pDevCmn->fnSNS_GetEarPhoneStatus();

						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("거    래 : [장애인 EarPhone 제거]"));	
						
						if ((m_pDevCmn->EarPhoneInserted == TRUE))
						{
							Delay_Msg(6000);
						}
						else	
							break;
					}
				}
				else	//	#N0272
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
					m_pDevCmn->fnSCR_DisplayScreen(445, K_5_WAIT);
					m_pDevCmn->fstrSCR_GetKeyString();
				}
			}	
			

			break;
		default:
			break;
	}

	if (m_pDevCmn->TranStatus == TRAN_OPEN)						// 개국처리
	{
/////////////////////////////////////////////////////////////////////////////
		CString		strTempDbData("");
		CString		strTemp("");

		m_pDevCmn->fnSCR_DisplayString(1, "자동화기기를 준비하고 있습니다.");
		m_pDevCmn->fnSCR_DisplayString(2, "잠시만 기다려 주십시오.");
		m_pDevCmn->fnSCR_DisplayScreen(701);			

		// 센서정보
		// OM_SENSORINFO||
		strTempDbData = "OM_SENSORINFO";

		// 일자(YYYYMMDD)||
		strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
		strTempDbData += "||" + strTemp;

		// 발생시간(HHMMSS)||
		strTemp.Format("%6.6s", GetTime().GetBuffer(0));
		strTempDbData += "||" + strTemp;

		// SliceLevel정보
		strTemp = IniGetStr(_SP_DEVICE_INI, "BRM", "SliceLevel");
		strTemp.Replace("(", "");								// 가로삭제
		strTemp.Replace(")", "");
		strTempDbData += "||" + strTemp;

		m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);		// 전자저널자료전송
/////////////////////////////////////////////////////////////////////////////
	}
	m_pDevCmn->TranProc = TranProc;								// Tran거래처리보관

	if(m_pDevCmn->HostFirstOpenFlag)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateProc - 사용가능한장치구하기");
		ChkID &= m_pDevCmn->fnAPL_GetAvailDevice(ChkID);			// 사용가능한장치구하기
		m_pDevCmn->fnAPL_CheckDeviceAction(ChkID);					// 전체동작완료처리
	}

//	fnAPP_SetPollTimeStart();									
	// V07-01-11-#00
	if (m_pDevCmn->TTSEngineLoadFlg)
	{
		MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "TTSEngineLoadFlg : FALSE");
		m_pDevCmn->TTSEngineLoadFlg = FALSE;
		m_pDevCmn->TTS_EndProc();
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateProc(TerminateCode 1 [%d])", TerminateCode);

	if (TerminateCode == T_OK)				
		throw CTranCmnExit(TRUE);								// 업무정상종료
	else
		throw CTranCmnExit(FALSE);								// 업무취소종료	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateProc(TerminateCode 2 [%d])", TerminateCode);

	return TerminateCode;
}
// 종료함수
int	CTranCmn::fnAPP_TerminateDummyProc(int TerminateCode)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateDummyProc(TerminateCode[%d])", TerminateCode);

	if (TerminateCode == T_OK)				
		throw CTranCmnExit(TRUE);								// 업무정상종료
	else
		throw CTranCmnExit(FALSE);								// 업무취소종료	
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateDummyProc(TerminateCode 2 [%d])", TerminateCode);

	return TerminateCode;
}
