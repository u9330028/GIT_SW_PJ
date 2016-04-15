/////////////////////////////////////////////////////////////////////////////
// TranHost.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\MacroDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"
#include "..\..\H\Dll\DevICCard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	송수신함수
/////////////////////////////////////////////////////////////////////////////
// 송신
int CTranCmn::fnAPP_SendHost()
{
	int		ScrNum = 0;

	int		nSendWaitSec = LINE_SEND_TIME;						
	if ((m_pDevCmn->TranStatus == TRAN_READY)	||				
		(m_pDevCmn->TranStatus == TRAN_OPEN)	)			
		nSendWaitSec = LINE_OPCL_TIME;

/////////////////////////////////////////////////////////////////////////////
//	일련번호 증가처리
/////////////////////////////////////////////////////////////////////////////
	if (!AddSerialFlg)											// 일련번호증가무
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost AddSerialNo++");
		AddSerialFlg = TRUE;									// 일련번호증가유
		m_pDevCmn->fnAPL_AddSerialNo();							// 거래일련번호증가
	}

/////////////////////////////////////////////////////////////////////////////
//	송신변수
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->TranResult = FALSE;								// Host결과
	SendLength = 0;												// 송신Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// 송신Buffer	
/////////////////////////////////////////////////////////////////////////////
	ScrNum = 601;												// 진행중화면
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);						// 진행중화면
	
	fnAPP_SendHostSet(TranCode2);
/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost::TranCode2 [%04d]", TranCode2);
	fnAPP_Tran_TxSend(TranCode2);
	fnAPD_CheckDepositPrint(DEV_JPR);							// 입금수표정보인자

/////////////////////////////////////////////////////////////////////////////
//	수신변수
/////////////////////////////////////////////////////////////////////////////
	RecvLength = 0;												// 수신Length
	memset(RecvBuffer, 0, sizeof(RecvBuffer));					// 수신Buffer
	memset(&RecvComm,	0, sizeof(RECVCOMM));					// 수신헤더
	memset(&PSPRecvData, 0, sizeof(PSPRecvData));				// DATA부/명세프린트(거래)
	memset(&PPPRecvData, 0, sizeof(PPPRecvData));				// DATA부/통장부프린트(거래)
	memset(&PPPRecvDataNon, 0, sizeof(PPPRecvDataNon));				// DATA부/통장부프린트(거래) - Non
	memset(&PPPRecvDataKb, 0, sizeof(PPPRecvDataKb));				// DATA부/통장부프린트(거래) - KB #N0227
	memset(&PPWRecvData, 0, sizeof(PPWRecvData));				// DATA부/통장M/S WRITE(거래)
	memset(&PCWRecvData, 0, sizeof(PCWRecvData));				// DATA부/카드M/S WRITE(거래)
	memset(&PMERecvData, 0, sizeof(PMERecvData));				// DATA부/메세지편집표시(거래/개국)
	memset(&PMDRecvData, 0, sizeof(PMDRecvData));				// DATA부/메세지표시(거래)

/////////////////////////////////////////////////////////////////////////////
//	수신후처리변수
/////////////////////////////////////////////////////////////////////////////
	ChoiceTran = TRAN_NULL;										// 조회후선택된거래
	DepNextTranFlag = FALSE;									// 연속거래_입금_Flag
	PbNextTranFlag = FALSE;										// 통장 다음거래 통보플래그
	PbNextPageFlag = FALSE;										// 통장 다음페이지 통보플래그
	InqNextTranFlag = FALSE;									// 조회 다음목록 통보 플래그
	KCashHostICFlag = FALSE;									// K-CASH Host IC Flag
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->HostLineMode == HOST_ONLINE_MODE)			// HOST_ONLINE_MODE
	{
		if (!SendLength)
			fnAPP_CancelProc(T_PROGRAM);
	}

	if (!m_pDevCmn->fnAPL_CheckHostLine())						// 라인체크
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래송신 : 장애]"));
		if (m_pDevCmn->TranStatus == TRAN_TOTAL || m_pDevCmn->TranStatus == TRAN_CASH_REG)
			m_pDevCmn->TranResult = FALSE;							// V01-00-00
		else
		{
			m_pDevCmn->fnAPL_StackError("7710891", "송신불가");
			{
//#0081
				if (TranCode == TC_WITHDRAW || TranCode == TC_SVCWITH)
				{
					fnAPP_WithErrSumProc(0);				
				}
				else
				if (TranCode == TC_DEPOSIT)
				{
					fnAPP_DepErrSumProc(0);
				}

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "거래가 불가합니다", "다음에 이용 바랍니다");
				else
					fnAPP_CancelProc(T_MSG, "Transaction is not valid");
			}
		}
	}


	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래송신]"));	
	if (fnAPP_SendData(SendBuffer, SendLength, LINE_SEND_TIME) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래송신 : 장애]"));
		if(TranCode2 == TRANID_4410 )
			m_pDevCmn->ForceDownFlag = TRUE;

		if (m_pDevCmn->TranStatus == TRAN_TOTAL || m_pDevCmn->TranStatus == TRAN_CASH_REG)
			m_pDevCmn->TranResult = FALSE;							// V01-00-00
		else
		{
			m_pDevCmn->fnAPL_StackError("7710991", "송신시간초과");	
			if (TranCode == TC_WITHDRAW || TranCode == TC_SVCWITH)
			{
				fnAPP_WithErrSumProc(0);				
			}
			else
			if (TranCode == TC_DEPOSIT)
			{
				fnAPP_DepErrSumProc(0);
			}

			fnAPP_CancelProc(T_SENDERROR);
		}
	}
	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_PROCESS);

	return T_OK;
}

// 송신(취소출금/입금)
int CTranCmn::fnAPP_SendHostCancel()
{
	int	ScrNum = 0;			
/////////////////////////////////////////////////////////////////////////////
//	송신변수
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->TranResult = FALSE;								// Host결과

	SendLength = 0;												// 송신Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// 송신Buffer
/////////////////////////////////////////////////////////////////////////////

	ScrNum = 701;												// 진행중화면
	m_pDevCmn->fnSCR_DisplayString(1, "거래 취소정보를 전송중입니다.");
	m_pDevCmn->fnSCR_DisplayString(2, "잠시만 기다려 주십시오.");
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);						// 진행중화면

/////////////////////////////////////////////////////////////////////////////
// 	if (!AddSerialFlg)											// 일련번호증가무
// 	{
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost::송신처리시 거래일련번호 증가처리");
// 		AddSerialFlg = TRUE;									// 일련번호증가유
//		m_pDevCmn->fnAPL_AddSerialNo();							// 거래일련번호증가
// 	}
/////////////////////////////////////////////////////////////////////////////
	if(TranCode2 == TRANID_4310)
		TranCode2 = TRANID_4320;
	else
	if(TranCode2 == TRANID_4330)
		TranCode2 = TRANID_4340;
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "취소 _ Trancode 없음 송신 안함");
		return T_OK;
	}


	fnAPP_Tran_TxSend(TranCode2);
/////////////////////////////////////////////////////////////////////////////
//	수신변수
/////////////////////////////////////////////////////////////////////////////
	RecvLength = 0;												// 수신Length
	memset(RecvBuffer, 0, sizeof(RecvBuffer));					// 수신Buffer

	memset(&PSPRecvData, 0, sizeof(PSPRecvData));				// DATA부/명세프린트(거래)
	memset(&PPPRecvData, 0, sizeof(PPPRecvData));				// DATA부/통장부프린트(거래)
	memset(&PPPRecvDataNon, 0, sizeof(PPPRecvDataNon));				// DATA부/통장부프린트(거래)
	memset(&PPPRecvDataKb, 0, sizeof(PPPRecvDataKb));				// DATA부/통장부프린트(거래) - KB #N0227
	memset(&PPWRecvData, 0, sizeof(PPWRecvData));				// DATA부/통장M/S WRITE(거래)
	memset(&PCWRecvData, 0, sizeof(PCWRecvData));				// DATA부/카드M/S WRITE(거래)
	memset(&PMERecvData, 0, sizeof(PMERecvData));				// DATA부/메세지편집표시(거래/개국)
	memset(&PMDRecvData, 0, sizeof(PMDRecvData));				// DATA부/메세지표시(거래)

/////////////////////////////////////////////////////////////////////////////
//	수신후처리변수
/////////////////////////////////////////////////////////////////////////////
	ChoiceTran = TRAN_NULL;										// 조회후선택된거래
	TranResultNGCode = 0;										// 수신결과 NG코드
	PbNextTranFlag = FALSE;										// 통장 다음거래 통보플래그
	DepNextTranFlag = FALSE;									// 연속거래_입금_Flag
	PbNextPageFlag = FALSE;										// 통장 다음페이지 통보플래그
	InqNextTranFlag = FALSE;									// 조회 다음목록 통보 플래그
	KCashHostICFlag = FALSE;									// K-CASH Host IC Flag
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->HostLineMode == HOST_ONLINE_MODE)			// HOST_ONLINE_MODE
	{
		if (!SendLength)
			fnAPP_CancelProc(T_PROGRAM);
	}

	if (!m_pDevCmn->fnAPL_CheckHostLine())						// 라인체크
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래송신 : 장애]"));
		m_pDevCmn->fnAPL_StackError("7710892", "취소송신불가");
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "취소처리가 불가합니다", "[상황실: 1577 - 4655] 문의하여 주십시오.");
		else
			fnAPP_CancelProc(T_MSG, "Transaction is not valid");
	}

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래송신]"));													
	if (fnAPP_SendData(SendBuffer, SendLength, LINE_SEND_TIME) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래송신 : 장애]"));
		m_pDevCmn->fnAPL_StackError("7710992", "취소송신시간초과");	// 2003.11.07
		fnAPP_CancelProc(T_SENDERROR);
	}


	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_PROCESS);// 진행버튼 깜빡임

	return T_OK;
}


// 완료
int CTranCmn::fnAPP_SendHostTranComplete()
{
	int		ScrNum = 0;
			
/////////////////////////////////////////////////////////////////////////////
//	송신변수
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->TranResult = FALSE;								// Host결과

	SendLength = 0;												// 송신Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// 송신Buffer
/////////////////////////////////////////////////////////////////////////////
	fnAPP_Tran_TxSend(TranCode2);
/////////////////////////////////////////////////////////////////////////////
//	수신변수
/////////////////////////////////////////////////////////////////////////////
	RecvLength = 0;												// 수신Length
	memset(RecvBuffer, 0, sizeof(RecvBuffer));					// 수신Buffer

	memset(&PSPRecvData, 0, sizeof(PSPRecvData));				// DATA부/명세프린트(거래)
	memset(&PPPRecvData, 0, sizeof(PPPRecvData));				// DATA부/통장부프린트(거래)
	memset(&PPWRecvData, 0, sizeof(PPWRecvData));				// DATA부/통장M/S WRITE(거래)
	memset(&PPPRecvDataNon, 0, sizeof(PPPRecvDataNon));				// DATA부/통장부프린트(거래) - Non
	memset(&PPPRecvDataKb, 0, sizeof(PPPRecvDataKb));				// DATA부/통장부프린트(거래) - KB #N0227
	memset(&PCWRecvData, 0, sizeof(PCWRecvData));				// DATA부/카드M/S WRITE(거래)
	memset(&PMERecvData, 0, sizeof(PMERecvData));				// DATA부/메세지편집표시(거래/개국)
	memset(&PMDRecvData, 0, sizeof(PMDRecvData));				// DATA부/메세지표시(거래)

/////////////////////////////////////////////////////////////////////////////
//	수신후처리변수
/////////////////////////////////////////////////////////////////////////////
	ChoiceTran = TRAN_NULL;										// 조회후선택된거래
	TranResultNGCode = 0;										// 수신결과 NG코드
	PbNextTranFlag = FALSE;										// 통장 다음거래 통보플래그
	DepNextTranFlag = FALSE;									// 연속거래_입금_Flag
	PbNextPageFlag = FALSE;										// 통장 다음페이지 통보플래그
	InqNextTranFlag = FALSE;									// 조회 다음목록 통보 플래그
	KCashHostICFlag = FALSE;									// K-CASH Host IC Flag
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->HostLineMode == HOST_ONLINE_MODE)			// HOST_ONLINE_MODE
	{
		if (!SendLength)
			fnAPP_CancelProc(T_PROGRAM);
	}

	if (!m_pDevCmn->fnAPL_CheckHostLine())						// 라인체크
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래송신 : 장애]"));
		m_pDevCmn->fnAPL_StackError("7710892", "취소송신불가");
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "취소처리가 불가합니다", "[상황실: 1577 - 4655] 문의하여 주십시오.");
		else
			fnAPP_CancelProc(T_MSG, "Transaction is not valid");
	}

	fnAPP_ClearArrDataField(); //Clear
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래송신]"));
	if (fnAPP_SendData(SendBuffer, SendLength, LINE_SEND_TIME) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래송신 : 장애]"));
		m_pDevCmn->fnAPL_StackError("7710992", "취소송신시간초과");	
		fnAPP_CancelProc(T_SENDERROR);
	}


	return T_OK;
}

// SNOD Send Recv
int	CTranCmn::fnAPP_SendRecvHostSnod(int nTranCodeID)
{
	int		ScrNum = 0;
	int		TempLength = 0;
	int		Index  = 0;

/////////////////////////////////////////////////////////////////////////////
//	Line Check
/////////////////////////////////////////////////////////////////////////////

//  Flag Reset처리를 송신전문 작성후 처리토록 수순변경//
//	PbNextTranFlag = FALSE;										// 통장 다음거래 통보플래그
//	PbNextPageFlag = FALSE;										// 통장 다음페이지 통보플래그
//	InqNextTranFlag = FALSE;									// 조회 다음목록 통보 플래그

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendRecvHostSnod (%d)", nTranCodeID);

	if (!m_pDevCmn->fnAPL_CheckHostLine())						// 라인체크
	{
		m_pDevCmn->fnAPL_StackError("7710891", "송신불가");
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				fnAPP_CancelProc(T_MSG, "거래가 불가합니다", "다음에 이용 바랍니다");
			else
				fnAPP_CancelProc(T_MSG, "Transaction is not valid");
		}
	}


//Kim.Gi.Jin TranCode2
/////////////////////////////////////////////////////////////////////////////
	if(nTranCodeID)
		TranCode2 = nTranCodeID;
/////////////////////////////////////////////////////////////////////////////

	
/////////////////////////////////////////////////////////////////////////////
//	일련번호 증가처리
/////////////////////////////////////////////////////////////////////////////
	if (!AddSerialFlg)											// 일련번호증가무
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost::송신처리시 거래일련번호 증가처리");
		AddSerialFlg = TRUE;									// 일련번호증가유
		m_pDevCmn->fnAPL_AddSerialNo();							// 거래일련번호증가
	}

/////////////////////////////////////////////////////////////////////////////
//	송신변수
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->TranResult = FALSE;								// Host결과

	SendLength = 0;												// 송신Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// 송신Buffer

/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(601);						// 진행중화면

	fnAPP_Tran_TxSend(TranCode2);

	if (m_pDevCmn->HostLineMode == HOST_ONLINE_MODE)			// HOST_ONLINE_MODE
	{
		if (!SendLength)
			fnAPP_CancelProc(T_PROGRAM);
	}

/////////////////////////////////////////////////////////////////////////////
//	수신후처리변수
/////////////////////////////////////////////////////////////////////////////
	DepNextTranFlag = FALSE;									// 연속거래_입금_Flag
	PbNextTranFlag = FALSE;										// 통장 다음거래 통보플래그
	PbNextPageFlag = FALSE;										// 통장 다음페이지 통보플래그
	InqNextTranFlag = FALSE;									// 조회 다음목록 통보 플래그
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//	송신처리
/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[SNOD 전문 송신]");	// 자료전송
	fnAPP_SendData(SendBuffer, SendLength, LINE_SEND_TIME);	

	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_PROCESS);// 진행버튼 깜빡임
	
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
		m_pDevCmn->TranResult = TRUE;							// Host결과
		return T_OK;											// HOST OK
	}
/////////////////////////////////////////////////////////////////////////////
//	수신변수
/////////////////////////////////////////////////////////////////////////////
	RecvLength = 0;												// 수신Length
	memset(RecvBuffer, 0, sizeof(RecvBuffer));					// 수신Buffer
	memset(&PPPRecvData, 0, sizeof(PPPRecvData));				// DATA부/통장부프린트(거래)
	memset(&PPPRecvDataNon, 0, sizeof(PPPRecvDataNon));				// DATA부/통장부프린트(거래) - Non
	memset(&PPPRecvDataKb, 0, sizeof(PPPRecvDataKb));				// DATA부/통장부프린트(거래) - KB #N0227
/////////////////////////////////////////////////////////////////////////////
	if (fnAPP_RecvData(RecvBuffer, &RecvLength, LINE_RECV_TIME) != T_OK)		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "호스트 전문 수신처리 확인(SNOD수신장애)");
		m_pDevCmn->TranResult = FALSE;
		switch(m_pDevCmn->fnNET_GetDeviceStatus())
		{
		case LINE_CLOSE:
			m_pDevCmn->fnAPL_StackError("8860293", "Line Close");
			break;
		default:
			m_pDevCmn->fnAPL_StackError("8860293", "수신시간초과");
			break;
		}

		//#0058
		/*
		if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP && (PbTran & MENU_PB) && (TranCode2 == TRANID_4730))
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendRecvHostSnod() 통장부 거래 TIMEOUT!! 불능 Write!!");
			fnAPD_PbMSWrite();
		}
		*/
		
		fnAPP_CancelProc(T_RECVERROR);

		return T_OK;
	}

	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_FINISH);// 종료버튼 깜빡임
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendRecvHostSnod() 호스트 전문 수신완료 확인(수신장애 없음)");
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
		m_pDevCmn->TranResult = TRUE;							// Host결과
		InqNextTranFlag = TRUE;
		PMERecvData.y = 2;
		for(int i=0; i<256; i++)
			PMERecvData.Data[0][i] = ' ';
		for(i=0; i<256; i++)
			PMERecvData.Data[1][i] = ' ';
		return T_OK;											// HOST OK
	}

	fnAPP_Tran_RxRecv(TranCode2);
	return T_OK;
}


// 송신전문작성(관리부 Send)
int CTranCmn::fnAPP_SendMakeManageSend(int RealFlg)
{
	return T_OK;
}


// 거래별자료설정
int CTranCmn::fnAPP_SendHostSet(int nTranCode2)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostSet(%04d)", nTranCode2);
	
	switch(nTranCode2)
	{
	case TRANID_0006:
	case TRANID_A010:
	case TRANID_A020:
	case TRANID_A060:
	case TRANID_2190:
		break;
	case TRANID_4310:
	case TRANID_4210:
	case TRANID_4116:
	case TRANID_4330:
		TranSend++;   //Cancel Send Flag
		break;
	case TRANID_7220:
	case TRANID_7230:
	case TRANID_4320:
	case TRANID_4340:
	case TRANID_4350:
		break;
	default:
		break;
	}


if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)                       // 보안상
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostSet", SendBuffer, SendLength);	


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostSet return (%04d)", nTranCode2);

	return TRUE;
}

/*
// 금융IC정보편집
int CTranCmn::fnAPP_SendMakeICInformation()
{
	BYTE	StrBuff[256];
	int		nIndex = 0;

	memset(StrBuff, ' ', sizeof(StrBuff));

	memcpy(&StrBuff[nIndex], m_pDevCmn->FinanceICInfo.szCSN, 16);
	nIndex += 16;

	fnAPD_FICGetHostTranData();

	memcpy(&StrBuff[nIndex], &m_pDevCmn->FinanceICInfo.HostTranData, 98);
	nIndex += 98;

	memcpy(&SendBuffer[SendLength], StrBuff, nIndex);
	SendLength += nIndex;

	return TRUE;
}
*/
// 동글이정보편집
int CTranCmn::fnAPP_SendMakeIRDInformation()
{
	BYTE	StrBuff[256];
	int		nIndex = 0;

	memset(StrBuff, ' ', sizeof(StrBuff));

	memcpy(&StrBuff[nIndex], MakeUnPack(m_pDevCmn->IRDAInfo.CSN, 8), 16);
	nIndex += 16;												// 카드일련번호(16)

	memcpy(&StrBuff[nIndex], MakeUnPack(m_pDevCmn->IRDAInfo.CardRandom, 16), 32);
	nIndex += 32;												// 카드난수(32)

	memcpy(&StrBuff[nIndex], MakeUnPack(m_pDevCmn->IRDAInfo.TerminalRandom, 16), 32);
	nIndex += 32;												// 터미널난수(32)

	memcpy(&StrBuff[nIndex], MakeUnPack(m_pDevCmn->IRDAInfo.CipherPW, 16), 32);
	nIndex += 32;												// 암호화비밀번호(32)

	memcpy(&StrBuff[nIndex], MakeUnPack((LPBYTE)&m_pDevCmn->IRDAInfo.FCI[33], 1), 2);
	nIndex += 2;												// FCI 中 KeyVersion(2)

	memcpy(&SendBuffer[SendLength], StrBuff, nIndex);
	SendLength += nIndex;

	return TRUE;
}

// EMV정보편집
int CTranCmn::fnAPP_SendMakeEMVInformation()
{
	BYTE	StrBuff[1024];
	int		nIndex = 0;

	memset(StrBuff, ' ', sizeof(StrBuff));

	if (CancelWithProcFlg)
	{
		nIndex = 147;
		m_pDevCmn->fnEMV_GetHostTranData(EMV_SENDHOST_REVERSAL);
		memcpy(&StrBuff[nIndex], m_pDevCmn->m_EMVszData, 12);
		nIndex += 12;
	}
	else
	{
		if (!TranSend)
		{
			if (fnAPD_EMVSendMakeProc())
			{
				fnAPP_CancelProc(T_MSG, "카드를 읽는 중 장애가 발생하였습니다.", "창구에 문의하여 주시기 바랍니다.");
			}
		}
		else
		{
			m_pDevCmn->fnEMV_GetHostTranData(EMV_SENDHOST_ARQC);
		}

		memcpy(&StrBuff[nIndex], m_pDevCmn->m_EMVszData, 156);
		nIndex += 156;
	}

	nIndex = 192;

	memcpy(&SendBuffer[SendLength], StrBuff, nIndex);
	SendLength += nIndex;

	return TRUE;
}



// 에러코드편집
int	CTranCmn::fnAPP_SendMakeDataSetErrorCode()
{
	return TRUE;
}

// 수표매수편집
int	CTranCmn::fnAPP_SendMakeDataSetCheckCount()
{
	char	StrBuff[16] = {0,};

	sprintf(StrBuff, "%02d", Asc2Int(Accept.CheckMoney, 10));
	memcpy(&SendBuffer[SendLength], StrBuff, 2);
	SendLength += 2;
	SendBuffer[SendLength++] = 0x1e;

	if(Asc2Int(Accept.CheckMoney, 15) > 0)
	{
		if(m_pProfile->UCM.CrntOutCheckCnt > 0)
		{
			memcpy(&SendBuffer[SendLength], m_pProfile->UCM.BankNo, 2);
			SendLength += 2;
			memcpy(&SendBuffer[SendLength], m_pProfile->UCM.CrntOutCheckNo, 8);
			SendLength += 8;
		}
		else
		{
			memcpy(&SendBuffer[SendLength], m_pProfile->UCM.BankNo2, 2);
			SendLength += 2;
			memcpy(&SendBuffer[SendLength], m_pProfile->UCM.CrntOutCheckNo2, 8);
			SendLength += 8;
		}
		memcpy(Accept.CheckNumber, &SendBuffer[SendLength-10], 10);
	}
	else
	{
		memcpy(&SendBuffer[SendLength], "          ", 10);
		SendLength += 10;
	}

	return TRUE;
}



// 입금수표정보편집
int	CTranCmn::fnAPP_SendMakeDataSetDepCheckInfo()
{
	BYTE	StrBuff[400] = {0,};
	int		nIndex = 0;

	memset(StrBuff, ' ', sizeof(StrBuff));

	sprintf((char*)&StrBuff[0], "%02d", CheckInfoCnt);			// 수표매수
	nIndex += 2;
	StrBuff[nIndex++] = 0x1e;									// 구분자
	for (int i = 0; i < 10; i++)
	{
		memcpy(&StrBuff[nIndex], CheckInfoData[i].ReadData, 8);	// 수표번호
		nIndex += 8;
		memcpy(&StrBuff[nIndex], CheckInfoData[i].Bank, 2);		// 은행코드
		nIndex += 2;
		memcpy(&StrBuff[nIndex], CheckInfoData[i].Branch, 4);	// 지점번호
		nIndex += 4;
		memcpy(&StrBuff[nIndex], CheckInfoData[i].AccountNum, 6);	// 계좌번호
		nIndex += 6;
		memcpy(&StrBuff[nIndex], CheckInfoData[i].Kind, 2);		// 권종
		nIndex += 2;
		memcpy(&StrBuff[nIndex], CheckInfoData[i].Amount, 10);	// 권종
		nIndex += 10;
		StrBuff[nIndex++] = 0x20;								// 구분자
	}

	memcpy(&SendBuffer[SendLength], StrBuff, nIndex);
	SendLength += nIndex;
	
	return TRUE;
}

// 송금(이체)정보편집
int	CTranCmn::fnAPP_SendMakeDataSetTransferInfo()
{
	BYTE	StrBuff[128] = {0,};
	int		nIndex = 0;

	memset(StrBuff, ' ', sizeof(StrBuff));

	sprintf((char*)&StrBuff[0], "%02d", Accept.InAccountNumSize);		// 이체은행계좌번호길이
	nIndex += 2;
	StrBuff[nIndex++] = 0x1e;												// 구분자
	memcpy(&StrBuff[nIndex], Accept.InAccountNum, sizeof(Accept.InAccountNum));	// 이체은행계좌번호
	nIndex += sizeof(Accept.InAccountNum);
	if(TranProc == TRAN_TRANS)											// 이체
	{
		StrBuff[nIndex++] = 0x1e;											// 구분자
		if(memcmp(Accept.BankNum, BANKJJ2, 3) == 0)	// 자->자
		{
			memcpy(&StrBuff[nIndex], BANKJJ2, 3);						// 자행이체
			nIndex += 2;
		}
		else										// 자->타
		{
			memcpy(&StrBuff[nIndex], Accept.BankNum, 2);				// 이체은행코드
			nIndex += 2;
		}
	}
	StrBuff[nIndex++] = 0x1e;
	memcpy(&StrBuff[nIndex], &RecvComm.TransData[139], 20);
	nIndex += 20;
	memcpy(&SendBuffer[SendLength], StrBuff, nIndex);
	SendLength += nIndex;

	return TRUE;
}

// 다음조회조건편집
int	CTranCmn::fnAPP_SendMakeDataSetNextInqInfo()
{
	memcpy(&SendBuffer[SendLength], Accept.NextInqCondition, sizeof(Accept.NextInqCondition));
	SendLength += sizeof(Accept.NextInqCondition);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPP_RecvHost()
{
	int		Index  = 0;
	int		nOpenFlag = FALSE;
	int		i = 0, j = 0;
	int		nRecvWaitSec = LINE_RECV_TIME;						// 송수신대기시간
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost()");
	if ((m_pDevCmn->TranStatus == TRAN_READY)	||				// 개국/준비완은 30초대기처리
		(m_pDevCmn->TranStatus == TRAN_OPEN)	)
		nRecvWaitSec = LINE_OPCL_TIME;

	if (fnAPP_RecvData(RecvBuffer, &RecvLength, nRecvWaitSec) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("거래수신 : 시간초과"));
		m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_FINISH);// 종료버튼 깜빡임
		if ((m_pDevCmn->TranStatus == TRAN_OPEN)	||
			(m_pDevCmn->TranStatus == TRAN_READY)	||
			(m_pDevCmn->TranStatus == TRAN_STATUS)	)
		{
			fnAPP_CancelProc(T_RECVERROR);
		}
		else
		{
			if(TranCode2 == TRANID_4410)
				m_pDevCmn->ForceDownFlag = TRUE;

			if (m_pDevCmn->TranStatus == TRAN_TOTAL || m_pDevCmn->TranStatus == TRAN_CASH_REG)
				m_pDevCmn->TranResult = FALSE;					
			else
			{
				switch(m_pDevCmn->fnNET_GetDeviceStatus())
				{
				case LINE_CLOSE:
					m_pDevCmn->fnAPL_StackError("8860293", "Line Close");
					break;
				default:
					m_pDevCmn->fnAPL_StackError("8860293", "수신시간초과");
					break;
				}

				if (TranCode == TC_WITHDRAW || TranCode == TC_SVCWITH)
				{
					fnAPP_WithErrSumProc(0);				
				}
				else
				if (TranCode == TC_DEPOSIT)
				{
					fnAPP_DepErrSumProc(0);
				}

				fnAPP_CancelProc(T_RECVERROR);
			}
		}
	}
	else m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래수신]"));

	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_FINISH);// 종료버튼 깜빡임

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
		m_pDevCmn->TranResult = TRUE;							// Host결과
		InqNextTranFlag = TRUE;
		PMERecvData.y = 1;
		if (TranCode == TC_INQWIDTH || TranCode == TC_INQDEP)	// 출금가능액조회
			RecvComm.TransData[83] = '1';
		
		PMERecvData.y = 2;
		for(int i=0; i<256; i++)
			PMERecvData.Data[0][i] = '1';
		for(i=0; i<256; i++)
			PMERecvData.Data[1][i] = '2';
		return T_OK;											// HOST OK
	}
/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost : m_pDevCmn->TranStatus - %d", m_pDevCmn->TranStatus);
	switch (m_pDevCmn->TranStatus)
	{
	case TRAN_READY:											// 준비완처리
		m_pDevCmn->TranResult = TRUE;
		break;
	case TRAN_STATUS:
	case TRAN_OPEN:												// 개국처리
	case TRAN_TRAN:												// 거래처리
	case TRAN_TOTAL:											// 마감인자
	case TRAN_SUBTOTAL:											// 마감조회
	case TRAN_CASH_REG:											// 출금현금등록
	case TRAN_CHECK_REG:										// 출금수표등록
	case TRAN_CHECK_UNREG:										// 출금수표해제
	case TRAN_MSFINGER_CONFIRM:									// 지문인증 Flow 진행
		fnAPP_Tran_RxRecv(TranCode2);
		break;
	}

	return T_OK;
}

// 수신해석
int CTranCmn::fnAPP_RecvAnal()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal : 수신데이타 분석시작");
	BYTE	StrBuff[NETBUFFSIZE] = {0,};
	int		nReturn = FALSE;
	int		nIndex = 0, i = 0, j = 0;

	return T_OK;
}

// DATA부/명세프린트(거래)
int CTranCmn::fnAPP_RecvPSPGet(void* RecvArea, PSPRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		HanFlg = FALSE, HanCnt = 0;
	BYTE	HanLeft = 0, HanRight = 0;
	char	szTempBuff[256] = {0,};
	char	szTempBuff1[256] = {0,};
	char	szTempBuff3[256] = {0,};
	char	szTempBuff4[256] = {0,};
	int		ii = 0, nSize = 0;
	CString strTempBuff;

for (ii = 0; ii < PMERecvData.y; ii++)
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PMERecvData[%d][%s]", ii, PMERecvData.Data[ii]);

	if (TranCode == TC_TOTAL)
	{															// 마감합계
/*///
		memset(szTempBuff, NULL, sizeof(szTempBuff));
		memcpy(szTempBuff, &RecvHead.CardData[54], 10);
		sprintf((char *)DataArea->Data[DataArea->y++], "지급현금(WC) : %s천원", Asc2Amt(szTempBuff, 10, 13).GetBuffer(0));
		memset(szTempBuff, NULL, sizeof(szTempBuff));
		memcpy(szTempBuff, &RecvHead.CardData[64], 10);
		sprintf((char *)DataArea->Data[DataArea->y++], "지급자점(WS) : %s천원", Asc2Amt(szTempBuff, 10, 13).GetBuffer(0));
		memset(szTempBuff, NULL, sizeof(szTempBuff));
		memcpy(szTempBuff, &RecvHead.CardData[74], 10);
		sprintf((char *)DataArea->Data[DataArea->y++], "입금현금(DC) : %s천원", Asc2Amt(szTempBuff, 10, 13).GetBuffer(0));
		memset(szTempBuff, NULL, sizeof(szTempBuff));
		memcpy(szTempBuff, &RecvHead.CardData[84], 10);
		sprintf((char *)DataArea->Data[DataArea->y++], "입금자점(DS) : %s천원", Asc2Amt(szTempBuff, 10, 13).GetBuffer(0));
		memset(szTempBuff, NULL, sizeof(szTempBuff));
		memcpy(szTempBuff, &RecvHead.CardData[94], 10);
		sprintf((char *)DataArea->Data[DataArea->y++], "입금타점(DT) : %s천원", Asc2Amt(szTempBuff, 10, 13).GetBuffer(0));
///*/
	}

	DataArea->Ey = DataArea->y;

	if (DataArea->Ey)
		DataArea->ParaLength = DataArea->Ey;

	return TRUE;
}

// 명세영역Move
int CTranCmn::fnAPP_RecvPSPMove(BYTE Data, PSPRecv* pDataArea, int Length)
{
	int		i;

	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->y >= itemof(pDataArea->Data)) ||		// 한계검증
			(pDataArea->x >= itemof(pDataArea->Data[0])))
			break;
		
		pDataArea->Data[pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line 초기설정
		{
			pDataArea->By = pDataArea->y;
			pDataArea->Ey = pDataArea->y;
		}
		pDataArea->ParaLength += Length;						// 자료유무구하기
		if (pDataArea->y < pDataArea->By)						// Find Begin Line
			pDataArea->By = pDataArea->y;
		if (pDataArea->y > pDataArea->Ey)						// Find End Line
			pDataArea->Ey = pDataArea->y;
	}
	return T_OK;
}
/*
 	
[2012/09/02 11:14:01 607] Name : 통장ff 기장 데이터 sbuff , Length : 360
00001:00016 31 30 30 30 31 32 30 37  31 37 20 20 20 20 20 20  1000120717      
00017:00032 20 20 20 20 32 30 30 31  32 5c 30 30 30 30 30 30      20012\000000
00033:00048 30 30 30 30 32 34 30 30  33 30 30 30 be e7 bc ba  000024003000....
00049:00064 b3 f3 c7 f9 c7 cf b3 aa  20 20 20 20 34 30 30 31  ........    4001
00065:00080 32 5c 30 30 30 30 30 30  30 30 30 30 34 37 30 34  2\00000000004704
00081:00096 35 30 30 30 4e 48 42 43  c3 bc c5 a9 20 20 20 20  5000NHBC....    
00097:00112 20 20 20 20 36 30 33 30  32 33 34 20 20 20 20 20      6030234     
00113:00128 20 20 20 20 20 20 20 20  31 30 30 30 31 32 30 37          10001207
00129:00144 32 30 20 20 20 20 20 20  20 20 20 20 32 30 30 30  20          2000
00145:00160 a3 b1 a3 b1 b9 f8 b0 a1  a1 a1 a1 a1 20 20 20 20  ............    
00161:00176 33 30 30 31 32 5c 30 30  30 30 30 30 30 30 30 31  30012\0000000001
00177:00192 31 39 30 30 34 30 30 31  32 5c 30 30 30 30 30 30  190040012\000000
00193:00208 30 30 30 31 36 36 30 34  35 30 30 30 50 47 b0 e1  000166045000PG..
00209:00224 c1 a6 c8 af ba d2 20 20  20 20 20 20 36 30 33 30  ......      6030
00225:00240 30 30 37 20 20 20 20 20  20 20 20 20 20 20 20 20  007             
00241:00256 39 30 30 30 b0 a1 b1 ee  bf ee 20 bf b5 be f7 c1  9000...... .....
00257:00272 a1 c0 b8 b7 ce 20 b9 e6  b9 ae c7 cf bf a9 20 c5  ..... ........ .
00273:00288 eb c0 e5 20 c0 cc bf f9  b9 df b1 de c0 bb 20 b9  ... .......... .
00289:00304 de c0 b8 bd c3 b1 e2 20  b9 d9 b6 f8 b4 cf b4 d9  ....... ........
00305:00320 2e 20 20 20 20 20 20 20  20 20 20 20 20 20 20 20  .               
00321:00336 20 20 20 20 20 20 20 20  20 20 20 20 20 20 20 20                  
00337:00352 20 20 20 20 20 20 20 20  20 20 20 20 20 20 20 20                  
00353:00360 20 20 20 20 20 20 20 20                                           


[2012/09/18 21:32:45:876 TranEnCryptProc(14760)] Name : fnAPP_TRAN_DEC_011 Res , Length : 120
00000:00016 31 30 30 30 31 32 30 39  31 38 20 20 20 20 20 20 1000120918      
00016:00032 20 20 20 20 32 30 30 31  32 5c 30 30 30 30 30 30     20012\000000
00032:00048 30 30 30 33 30 30 30 30  33 30 30 30 20 4e 43 49 000300003000 NCI
00048:00064 39 38 34 30 20 20 20 20  20 20 20 20 34 30 30 31 9840        4001
00064:00080 32 5c 30 30 30 30 30 30  30 33 30 32 39 39 30 30 2\00000003029900
00080:00096 35 30 30 30 c5 eb c0 e5  b3 aa c0 cc bd ba 20 20 5000..........  
00096:00112 20 20 20 20 36 30 30 30  4e 43 49 39 38 34 20 20     6000NCI984  
00112:00128 20 20 20 20 20 20 20 20     

 */
// DATA부/통장부프린트(거래)
int CTranCmn::fnAPP_NONGHYUP_RecvPPPGet1(void* RecvArea, PPPRecvNon* DataArea, int SubPatton, int FindLength)
{
	int nSsP = SubPatton;
	BYTE*	pRecvArea	= (BYTE*)RecvArea;
	int		Cnt = 0, ElementCnt = 0, Startcnt = 0;
	int		PrintChar = 0;				// 1 : 문자, 2 : 숫자
	int		nMoveCnt = 0;
	int		nGSpaceCnt = 0;
	char	szGetPbBuff[1024];
	BYTE	hanFlag = 0;
	int		nTCnt = 0;
	int		nSlen = 0;
	int		nCh = 0;
	BYTE	TotElement[120];
	BYTE	Element1[21];
	BYTE	Element2[21];
	BYTE	Element3[21];
	BYTE	Element4[21];
	BYTE	Element5[21];
	BYTE	Element6[21];
	char	szEditElement[21];
	BYTE	EditElement3[21];
	BYTE	EditElement4[21];
	memset(Element1, 0x20, sizeof(Element1));
	memset(Element2, 0x20, sizeof(Element2));
	memset(Element3, 0x20, sizeof(Element3));
	memset(Element4, 0x20, sizeof(Element4));
	memset(Element5, 0x20, sizeof(Element5));
	memset(Element6, 0x20, sizeof(Element6));
	memset(szEditElement, 0, sizeof(szEditElement));
	memset(EditElement3, 0, sizeof(Element4));
	memset(EditElement4, 0, sizeof(Element5));
	CString cstrGet("");


	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 SubPatton[%d][%d] y[%d] p[%d]", SubPatton, FindLength, DataArea->y, DataArea->p);
	
	//#H0066
	if(pRecvArea[0] != 0x39)
	{
		memcpy(Element1, &pRecvArea[Startcnt+4], 16);	
		Startcnt += 20;

		if (pRecvArea[Startcnt + 3] == 0x30)			//문자이면
		{
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);
			memcpy(&Element2[nGSpaceCnt], &pRecvArea[Startcnt+4], 16 - nGSpaceCnt);	
	//		memset(szEditElement, ' ', sizeof(szEditElement));
	//		strRTrim((char *)Element2, szEditElement, 16 - nGSpaceCnt);
	//		memcpy(Element2, szEditElement, strlen(szEditElement));
		}
		else
		if (pRecvArea[Startcnt + 3] == 0x31)			//숫자이면)
		{
			// \ 기호가 있는지 확인
			// 금액 편집 flag -> 0은 제외하고, 유효 숫자이후가 금액에 해당됨.
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);

			memcpy(Element2, Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);			// 인출금액
			cstrGet.Format("%s", Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);
			cstrGet.TrimLeft();
			cstrGet.TrimRight();

			memcpy(&Element2[14 - cstrGet.GetLength()], cstrGet.GetBuffer(0), cstrGet.GetLength());	
			//편집문자 *, -, \ //
			memcpy(&Element2[14 - cstrGet.GetLength() - 1], &pRecvArea[Startcnt+4+1], 1);	
			
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 - 0");

		Startcnt += 20;
		if (pRecvArea[Startcnt + 3] == 0x30)			//문자이면
		{
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);
			memcpy(&Element3[nGSpaceCnt], &pRecvArea[Startcnt+4], 16 - nGSpaceCnt);		
		}
		else
		if (pRecvArea[Startcnt + 3] == 0x31)			//숫자이면)
		{
			// \ 기호가 있는지 확인
			// 금액 편집 flag -> 0은 제외하고, 유효 숫자이후가 금액에 해당됨.
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);

			memcpy(Element3, Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);			// 인출금액
			cstrGet.Format("%s", Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);
			cstrGet.TrimLeft();
			cstrGet.TrimRight();

			memcpy(&Element3[14 - cstrGet.GetLength()], cstrGet.GetBuffer(0), cstrGet.GetLength());	
			//편집문자 *, -, \ //
			memcpy(&Element3[14 - cstrGet.GetLength() - 1], &pRecvArea[Startcnt+4+1], 1);	
			
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 - 1");

		Startcnt += 20;
		if (pRecvArea[Startcnt + 3] == 0x30)			//문자이면
		{
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);
			memcpy(&Element4[nGSpaceCnt], &pRecvArea[Startcnt+4], 16 - nGSpaceCnt);	
		}
		else
		if (pRecvArea[Startcnt + 3] == 0x31)			//숫자이면)
		{
			// \ 기호가 있는지 확인
			// 금액 편집 flag -> 0은 제외하고, 유효 숫자이후가 금액에 해당됨.
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);

			memcpy(Element4, Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);			// 인출금액
			cstrGet.Format("%s", Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);
			cstrGet.TrimLeft();
			cstrGet.TrimRight();

			memcpy(&Element4[14 - cstrGet.GetLength()], cstrGet.GetBuffer(0), cstrGet.GetLength());	
			//편집문자 *, Element4, \ //
			memcpy(&Element4[14 - cstrGet.GetLength() - 1], &pRecvArea[Startcnt+4+1], 1);	
			
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 - 2");

		Startcnt += 20;
		if (pRecvArea[Startcnt + 3] == 0x30)			//문자이면
		{
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);
			memcpy(&Element5[nGSpaceCnt], &pRecvArea[Startcnt+4], 16 - nGSpaceCnt);	
		}
		else
		if (pRecvArea[Startcnt + 3] == 0x31)			//숫자이면)
		{
			// \ 기호가 있는지 확인
			// 금액 편집 flag -> 0은 제외하고, 유효 숫자이후가 금액에 해당됨.
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);

			memcpy(Element5, Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);			// 인출금액
			cstrGet.Format("%s", Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);
			cstrGet.TrimLeft();
			cstrGet.TrimRight();

			memcpy(&Element5[14 - cstrGet.GetLength()], cstrGet.GetBuffer(0), cstrGet.GetLength());	
			//편집문자 *, -, \ //
			memcpy(&Element5[14 - cstrGet.GetLength() - 1], &pRecvArea[Startcnt+4+1], 1);	
			
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 - 3");

		Startcnt += 20;
		if (pRecvArea[Startcnt + 3] == 0x30)			//문자이면
		{
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);
			memcpy(&Element6[nGSpaceCnt], &pRecvArea[Startcnt+4], 16 - nGSpaceCnt);	
		}
		else
		if (pRecvArea[Startcnt + 3] == 0x31)			//숫자이면)
		{
			// \ 기호가 있는지 확인
			// 금액 편집 flag -> 0은 제외하고, 유효 숫자이후가 금액에 해당됨.
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);

			memcpy(Element6, Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);			// 인출금액
			cstrGet.Format("%s", Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);
			cstrGet.TrimLeft();
			cstrGet.TrimRight();

			memcpy(&Element6[14 - cstrGet.GetLength()], cstrGet.GetBuffer(0), cstrGet.GetLength());	
			//편집문자 *, -, \ //
			memcpy(&Element6[14 - cstrGet.GetLength() - 1], &pRecvArea[Startcnt+4+1], 1);	
			
		}

		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E1", (char *)Element1, 20);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E2", (char *)Element2, 20);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E3", (char *)Element3, 20);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E4", (char *)Element4, 20);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E5", (char *)Element5, 20);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E6", (char *)Element6, 20);

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 - 0 - Sub[%d] Pos[%x]", nSsP, pRecvArea[23]);
		switch(nSsP) 
		{
		case 0:
			nCh = FindChar(Element1, 16, ' ');
			if(nCh == 6)
			{
				//6	13	13	14	10	6 -> 금액 (14Byte) - 보정
				if(pRecvArea[23] == 0x30)
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 6);
					nSlen +=6;				
					memcpy(&TotElement[nSlen], &Element2[0], 13); //문자보정 (편집문자 : *, -, \ Space) = +1 <- 협의 필요
					nSlen +=13;				
	//				nSlen +=1;				
					memcpy(&TotElement[nSlen], &Element3[1], 13);   //숫자
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element4, 14);       //숫자
					nSlen +=14;				
					memcpy(&TotElement[nSlen], Element5, 10);     //문자
					nSlen +=10;				
	//				nSlen +=1;				
					memcpy(&TotElement[nSlen], Element6, 6);	    //문자

	//				sprintf((char *)TotElement, "%6.6s%13.13s%13.13s%14.14s%10.10s%6.6s", Element1,&Element2[0],&Element3[1],Element4,Element5,Element6);
				}
				else
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 6);
					nSlen +=6;				
					memcpy(&TotElement[nSlen], &Element2[1], 13); //숫자
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element3[0], 13); //문자
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element4, 14);     //숫자
					nSlen +=14;				
					memcpy(&TotElement[nSlen], Element5, 10);     //문자
					nSlen +=10;				
					memcpy(&TotElement[nSlen], Element6, 6);	  //문자
	//				sprintf((char *)TotElement, "%6.6s%13.13s%13.13s%14.14s%10.10s%6.6s", Element1,&Element2[1],&Element3[0],Element4,Element5,Element6);
				}
			}
			else
			if(nCh == 8)
			{
				//8	12	12	13	10	6
				if(pRecvArea[23] == 0x30)
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 8);
					nSlen +=8;				
					memcpy(&TotElement[nSlen], &Element2[0], 12);
					nSlen +=12;				
					memcpy(&TotElement[nSlen], &Element3[2], 12);
					nSlen +=12;				
					memcpy(&TotElement[nSlen], &Element4[1], 13); //#H0044
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element5, 10);
					nSlen +=10;				
					memcpy(&TotElement[nSlen], Element6, 6);	

	//				sprintf((char *)TotElement, "%8.8s%8.8s%14.14s%14.14s%12.12s%6.6s", Element1,&Element2[0],Element3,Element4,Element5,Element6);
				}
				else
				{

					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 8);
					nSlen +=8;				
					memcpy(&TotElement[nSlen], &Element2[2], 12);
					nSlen +=12;				
					memcpy(&TotElement[nSlen], &Element3[0], 12);
					nSlen +=12;				
					memcpy(&TotElement[nSlen], &Element4[1], 13); //#H0044
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element5, 10);
					nSlen +=10;				
					memcpy(&TotElement[nSlen], Element6, 6);	

	//				sprintf((char *)TotElement, "%8.8s%8.8s%14.14s%14.14s%12.12s%6.6s", Element1,&Element2[6],Element3,Element4,Element5,Element6);
				}

			}
			else
			if(nCh == 10)
			{
				//10	13	9	7	16	7
				if(pRecvArea[23] == 0x30)
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 10);
					nSlen +=10;				
					memcpy(&TotElement[nSlen], &Element2[0], 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element3[5], 9);
					nSlen +=9;				
					memcpy(&TotElement[nSlen], &Element4[7], 7);
					nSlen +=7;				
					memcpy(&TotElement[nSlen], Element5, 16);
					nSlen +=16;				
					memcpy(&TotElement[nSlen], Element6, 7);	

	//				sprintf((char *)TotElement, "%10.10s%13.13s%9.9s%7.7s%16.16s%7.7s", Element1,&Element2[0],&Element3[5],&Element4[7],Element5,Element6);
				}
				else
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 10);
					nSlen +=10;				
					memcpy(&TotElement[nSlen], &Element2[1], 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element3[0], 9);
					nSlen +=9;				
					memcpy(&TotElement[nSlen], &Element4[7], 7);
					nSlen +=7;				
					memcpy(&TotElement[nSlen], Element5, 16);
					nSlen +=16;				
					memcpy(&TotElement[nSlen], Element6, 7);	

	//				sprintf((char *)TotElement, "%10.10s%13.13s%9.9s%7.7s%16.16s%7.7s", Element1,&Element2[1],&Element3[0],&Element4[7],Element5,Element6);
				}

			}
			else
			if(nCh == 13)
			{
				//13	13	10	13	6	8
				if(pRecvArea[23] == 0x30)
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element2[0], 13); //문자
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element3[4], 10);
					nSlen +=10;				
					memcpy(&TotElement[nSlen], &Element4[1], 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element5, 6);
					nSlen +=6;				
					memcpy(&TotElement[nSlen], Element6, 8);
					
	//				sprintf((char *)TotElement, "%13.13s%13.13s%10.10s%13.13s%6.6s%8.8s", Element1,&Element2[0],&Element3[4],&Element4[1],Element5,Element6);
				}
				else
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element2[1], 13); //숫자 - 
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element3[0], 10); //문자
					nSlen +=10;				
					memcpy(&TotElement[nSlen], &Element4[1], 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element5, 6);
					nSlen +=6;				
					memcpy(&TotElement[nSlen], Element6, 8);

	//				sprintf((char *)TotElement, "%13.13s%13.13s%10.10s%13.13s%6.6s%8.8s", Element1,&Element2[1],&Element3[0],&Element4[1],Element5,Element6);
				}

			}
			break;
		case 1:
			//8	14	14	12	6	8
			nSlen =0;
			memcpy(&TotElement[nSlen], Element1, 8);
			nSlen +=8;				
			memcpy(&TotElement[nSlen], &Element2[1], 14); //숫자 - 
			nSlen +=14;				
			memcpy(&TotElement[nSlen], &Element3[0], 14); //문자
			nSlen +=14;				
			memcpy(&TotElement[nSlen], &Element4[2], 12);
			nSlen +=12;				
			memcpy(&TotElement[nSlen], Element5, 6);
			nSlen +=6;				
			memcpy(&TotElement[nSlen], Element6, 8);

	//		sprintf((char *)TotElement, "%8.8s%14.14s%14.14s%12.12s%6.6s%8.8s", Element1,Element2,Element3,&Element4[2],Element5,Element6);
			break;
		case 3:
			//8	13	13	14	8	6
			if(pRecvArea[23] == 0x30)
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[0], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], &Element3[1], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 6);
				nSlen +=6;				
				memcpy(&TotElement[nSlen], Element6, 8);
	//			sprintf((char *)TotElement, "%8.8s%13.13s%13.13s%14.14s%8.8s%6.6s", Element1,&Element2[0],&Element3[1],Element4,Element5,Element6);
			}
			else
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[1], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], &Element3[0], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 6);
				nSlen +=6;				
				memcpy(&TotElement[nSlen], Element6, 8);

	//			sprintf((char *)TotElement, "%8.8s%13.13s%13.13s%14.14s%8.8s%6.6s", Element1,&Element2[1],&Element3[0],Element4,Element5,Element6);
			}
			break;
		case 4:
			//8	12	12	13	10	6
			if(pRecvArea[23] == 0x30)
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[0], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], &Element3[2], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], &Element4[1], 13);
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element5, 10);
				nSlen +=10;				
				memcpy(&TotElement[nSlen], Element6, 6);

	//			sprintf((char *)TotElement, "%8.8s%12.12s%12.12s%13.13s%10.10s%6.6s", Element1,&Element2[0],&Element3[2],&Element4[1],Element5,Element6);
			}
			else
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[2], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], &Element3[0], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], Element4, 13);
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element5, 10);
				nSlen +=10;				
				memcpy(&TotElement[nSlen], Element6, 6);

	//			sprintf((char *)TotElement, "%8.8s%12.12s%12.12s%13.13s%10.10s%6.6s", Element1,&Element2[2],&Element3[0],&Element4[1],Element5,Element6);
			}
			break;
		case 9:
			//8	12	14	15	6	8
			if(pRecvArea[23] == 0x30)
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[0], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], &Element3[0], 14); 
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 6);
				nSlen +=6;				
				memcpy(&TotElement[nSlen], Element6, 8);

	//			sprintf((char *)TotElement, "%8.8s%12.12s%14.14s %14.14s%6.6s%8.8s", Element1,&Element2[0],Element3,Element4,Element5,Element6);
			}
			else
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[2], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], &Element3[0], 14); 
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 6);
				nSlen +=6;				
				memcpy(&TotElement[nSlen], Element6, 8);

	//			sprintf((char *)TotElement, "%8.8s%12.12s%14.14s %14.14s%6.6s%8.8s", Element1,&Element2[2],Element3,Element4,Element5,Element6);
			}
			break;
		case 90:
			//6	13	13	14	10	6
			if(pRecvArea[23] == 0x30)
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[0], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], &Element3[1], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 10);
				nSlen +=10;				
				memcpy(&TotElement[nSlen], Element6, 6);

	//			sprintf((char *)TotElement, "%6.6s%13.13s%13.13s%14.14s%10.10s%6.6s", Element1,&Element2[0],&Element3[1],Element4,Element5,Element6);
			}
			else
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[1], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], &Element3[0], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 10);
				nSlen +=10;				
				memcpy(&TotElement[nSlen], Element6, 6);

	//			sprintf((char *)TotElement, "%6.6s%13.13s%13.13s%14.14s%10.10s%6.6s", Element1,&Element2[1],&Element3[0],Element4,Element5,Element6);
			}
			break;
		default:
			break;
		}
	}
	else
	if(pRecvArea[0] == 0x39)
	{
		cstrGet.Format("%114.114s", &pRecvArea[4]);
		cstrGet.TrimRight();
		sprintf((char *)TotElement, "%s", cstrGet);
	}
	else
	{
		cstrGet.Format("%114.114s", &pRecvArea[4]);
		cstrGet.TrimRight();
		sprintf((char *)TotElement, "%s", cstrGet);
	}
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4", (char *)TotElement, 120);
//	for(int ii=0; ii< strlen((char *)TotElement); )
	nCh = FindChar(TotElement, 120, NULL);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 Len = (%d)", nCh);
	for(int ii=0; ii < nCh; )
	{
		if((BYTE)TotElement[Cnt+ii] >= 0x80)			// 한글시작처리
		{
			if(hanFlag != 1)							// 영문상태
			{
				memcpy(&szGetPbBuff[nTCnt], PbHanStart, 2);
				nTCnt += 2;
			}
			memcpy(&szGetPbBuff[nTCnt], &TotElement[Cnt+ii], 2);
			ii += 2;
			nTCnt += 2;
			hanFlag = 1;
		}
		else											// 한글끝처리
		{
			if(hanFlag == 1)							// 한글상태
			{
				memcpy(&szGetPbBuff[nTCnt], PbHanEnd, 2);
				nTCnt += 2;
				hanFlag = 0;
			}
/*
			if ((TotElement[ii] == ' ') ||			// ' '
				(TotElement[ii] == ',') ||			// ','
				(TotElement[ii] == '.') ||			// '.'
				(TotElement[ii] == '\''))			// '''
			{	
//				if (TotElement[ii] == ' ')			// ' '
//					TotElement[ii] = PbHalfSpace;
				if (TotElement[ii] == ',')			// ','
					TotElement[ii] = PbHalfcomma;
				if (TotElement[ii] == '.')			// '.'
					TotElement[ii] = PbHalfperiod;
				if (TotElement[ii] == '\'')			// '''
					TotElement[ii] = PbHalfapostrophe;
			}
*/
			szGetPbBuff[nTCnt++] = TotElement[Cnt+ii];
			ii += 1;
		}
	}
	
	memcpy(&DataArea->Data[DataArea->p][(DataArea->y)][DataArea->x], szGetPbBuff, nTCnt);
	return T_OK;
}

int CTranCmn::fnAPP_NONGHYUP_RecvPPPGet(void* RecvArea, PPPRecvNon* DataArea, int FindLength)
{
	BYTE*	pRecvArea	= (BYTE*)RecvArea;
	int		Cnt = 0, ResultCnt = 0;
	int		HanFlg = FALSE, HalfCnt = 0;
	
	Cnt = 4;
	DataArea->p = 0;		
	DataArea->y = 0;		
	DataArea->x = 0;
	Cnt += 2; //통장 지정 행
	if (Asc2Int(&pRecvArea[Cnt], 2) > 0)	
	{															// 라인개행
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SkipLineCnt 라인개행 = %d", Asc2Int(&pRecvArea[Cnt], 2));

		DataArea->y = Asc2Int(&pRecvArea[Cnt], 2);		// 개행수
		DataArea->x = 0;
	}
	Cnt += 2; //통장 대상 건수
	Cnt += 2;
	while (Cnt < FindLength)
	{															
		if (memcmp(&pRecvArea[Cnt], PPPRecvCmdId, sizeof(PPPRecvCmdId)) == 0)
		{														// 페이지바뀜
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PPPRecvCmdId 페이지바뀜");
			fnAPP_NONGHYUP_RecvPPPMove(' ', DataArea, 1);				// V04-01-06-#01
			ResultCnt = 120;
			if (HanFlg)
			{													// 한글End
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
				HanFlg = FALSE;
			}
			Cnt += 4;
			if (ResultCnt < 4)
				ResultCnt = 0;
			else ResultCnt -= 4;
		}
		else
		if (memcmp(&pRecvArea[Cnt], SUBNewLineCmdId, sizeof(SUBNewLineCmdId)) == 0)	
		{														// 라인개행
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SUBNewLineCmdId 라인개행");
			if (HanFlg)
			{													// 한글End
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
				HanFlg = FALSE;
			}

			// V04-03-05-#01 Start──────────────────────────
			// 데이타 없이 라인개행만 오는 경우 대책 
			if (ResultCnt == 0)
			{
				fnAPP_NONGHYUP_RecvPPPMove(' ', DataArea, 1);			// V04-03-05-#01
				ResultCnt = 120;
			}
			// V04-03-05-#01 End───────────────────────────

			DataArea->y ++;										// 개행수

			if (DataArea->y >= itemof(DataArea->Data[0]))		// 라인체크
			{
				DataArea->y -= 24;
				DataArea->p++;
			}
			DataArea->x = 0;
			Cnt += 4;
			if (ResultCnt < 4)
				ResultCnt = 0;
			else ResultCnt -= 4;
			Cnt += ResultCnt;

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "통장 Cnt = %d, ResultCnt = %d", Cnt, ResultCnt);
			ResultCnt = 0;

			if ((!ResultCnt) && (HanFlg))
			{													// 한글End
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
				HanFlg = FALSE;
			}

			if ((!ResultCnt) && (HalfCnt & 0x01))				// 반각코드짝수로
				fnAPP_NONGHYUP_RecvPPPMove(HalfSpace, DataArea, 1);

			if (!ResultCnt)
			{
				HanFlg = FALSE;
				HalfCnt = 0;
			}
		}
		else
		{
			if (ResultCnt)
			{													// 압축
				if (memcmp(&pRecvArea[Cnt], DATAPackCmdId, sizeof(DATAPackCmdId)) == 0)	
				{
					if (HanFlg)
					{											// 한글End
						fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
						fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
						HanFlg = FALSE;
					}
					fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt + 3], DataArea, pRecvArea[Cnt + 2]);
					Cnt += 4;
					if (ResultCnt < 4)
						ResultCnt = 0;
					else ResultCnt -= 4;
				}
				else											// 반각
				if (memcmp(&pRecvArea[Cnt], DATAHalfCmdId, sizeof(DATAHalfCmdId)) == 0)	
				{												// 반각코드설정
					if (HanFlg)
					{											// 한글End
						fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
						fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
						HanFlg = FALSE;
					}
					if ((pRecvArea[Cnt + 2] == ' ') ||			// ' '
						(pRecvArea[Cnt + 2] == ',') ||			// ','
						(pRecvArea[Cnt + 2] == '.') ||			// '.'
						(pRecvArea[Cnt + 2] == '\''))			// '''
					{	
						HalfCnt++;
						if (pRecvArea[Cnt + 2] == ' ')			// ' '
							fnAPP_NONGHYUP_RecvPPPMove(PbHalfSpace, DataArea, 1);
						if (pRecvArea[Cnt + 2] == ',')			// ','
							fnAPP_NONGHYUP_RecvPPPMove(PbHalfcomma, DataArea, 1);
						if (pRecvArea[Cnt + 2] == '.')			// '.'
							fnAPP_NONGHYUP_RecvPPPMove(PbHalfperiod, DataArea, 1);
						if (pRecvArea[Cnt + 2] == '\'')			// '''
							fnAPP_NONGHYUP_RecvPPPMove(PbHalfapostrophe, DataArea, 1);
					}
					else fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt + 2], DataArea, 1);
					Cnt += 3;									// 반각코드해제
					if (ResultCnt < 3)
						ResultCnt = 0;
					else ResultCnt -= 3;
				}
				else
				{
					if (pRecvArea[Cnt] & 0x80)					// 한글1st
					{
						if (ResultCnt >= 2)						// 잔여갯수2개
						{
							if (pRecvArea[Cnt + 1] & 0x80)		// 한글2st
							{
								if (!HanFlg)
								{								// 한글Start
									fnAPP_NONGHYUP_RecvPPPMove(HanStart[0], DataArea, 1);
									fnAPP_NONGHYUP_RecvPPPMove(HanStart[1], DataArea, 1);
									HanFlg = TRUE;
								}
								// 한글 Filtering 장애 대책 PJH 2002.07.18
								// 한글영역: 0xb0|0xa1 ~ 0xc8|0xfe 이외의 데이터는 "없"자로 Set함.
								if ((pRecvArea[Cnt] < 0xb0) ||
									(pRecvArea[Cnt] > 0xc8))
								{
									fnAPP_NONGHYUP_RecvPPPMove(0xbe, DataArea, 1);
									fnAPP_NONGHYUP_RecvPPPMove(0xf8, DataArea, 1);
									Cnt += 2;
								}
								else
								{
									fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt++], DataArea, 1);
									fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt++], DataArea, 1);
								}
								if (ResultCnt < 2)
									ResultCnt = 0;
								else ResultCnt -= 2;
							}
							else
							{
								if (HanFlg)
								{								// 한글End
									fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
									fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
									HanFlg = FALSE;
								}
								fnAPP_NONGHYUP_RecvPPPMove(' ', DataArea, 1);
								fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt + 1], DataArea, 1);
								Cnt += 2;
								if (ResultCnt < 2)
									ResultCnt = 0;
								else ResultCnt -= 2;
							}
						}
						else									// 잔여갯수1개
						{
							if (HanFlg)
							{									// 한글End
								fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
								fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
								HanFlg = FALSE;
							}
							fnAPP_NONGHYUP_RecvPPPMove(' ', DataArea, 1);
							Cnt++;
							ResultCnt--;
						}
					}
					else										// 영문
					{
						if (HanFlg)
						{										// 한글End
							fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
							fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
							HanFlg = FALSE;
						}
						fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt++], DataArea, 1);
						ResultCnt--;
					}
				}

				if ((!ResultCnt) && (HanFlg))
				{												// 한글End
					fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
					fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
					HanFlg = FALSE;
				}

				if ((!ResultCnt) && (HalfCnt & 0x01))			// 반각코드짝수로
					fnAPP_NONGHYUP_RecvPPPMove(HalfSpace, DataArea, 1);

				if (!ResultCnt)
				{
					HanFlg = FALSE;
					HalfCnt = 0;
				}
			}
			else
			{
				if (HanFlg)
				{												// 한글End
					fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
					fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
					HanFlg = FALSE;
				}
				ResultCnt = 120;
			}
		}
	}

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPP_NONGHYUP_RecvPPPGet - 0]", &DataArea->Data[0][0], sizeof(DataArea->Data));

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPMove[FindLength = %d]", FindLength);

	return FindLength;
}
// DATA부/통장부프린트(거래)


int CTranCmn::fnAPP_IBK_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0;
	BYTE	hanFlag = 0;
	BYTE	tempBuff[256];


	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// 인쇄시작라인
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// 인쇄라인수
	Cnt += 2;
	DataArea->x = 0;
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		for(int ii=0; ii<75; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// 한글시작처리
			{
				if(hanFlag != 1)							// 영문상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// 한글끝처리
			{
				if(hanFlag == 1)							// 한글상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}
		memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][(DataArea->y)+i] = tmpCnt;
		Cnt+= 75;
	}
	return Cnt;
}

// DATA부/통장부프린트(거래)
int CTranCmn::fnAPP_KEB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0, StartPosX = 0;
	BYTE	hanFlag = 0;
	char	szBuff[256];
	BYTE	tempBuff[256];

	Accept.Index = 0;
	memset(szBuff, 0x00, sizeof(szBuff));
	memset(Accept.CrmDummy, 0x00, sizeof(Accept.CrmDummy));
	
	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// 인쇄시작라인
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// 인쇄라인수
	Cnt += 2;
	DataArea->x = 0;
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
	
		if(pRecvArea[Cnt] == 'M') //#0099
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPPGet - 1 [%3.3s]", &pRecvArea[Cnt]); //#0113
			if (memcmp(&pRecvArea[Cnt + 1], "00", 2) == 0) 
			{
				Accept.Index = 99;
				memcpy(Accept.CrmDummy, &pRecvArea[Cnt + 3], 80);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPPGet PBM M00 -> Msg[%s]", strCT(Accept.CrmDummy,80));
			}
			else
			if (memcmp(&pRecvArea[Cnt + 1], "02", 2) == 0) 
			{
				memcpy(szBuff, &pRecvArea[Cnt + 3], 80);
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPPGet PBM M02 -> Msg[%s]",szBuff);
			}
		}
		
		for(int ii=0; ii<83; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// 한글시작처리
			{
				if(hanFlag != 1)							// 영문상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// 한글끝처리
			{
				if(hanFlag == 1)							// 한글상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}
		
		//D20 = 라인인자(구분자(1Byte) + Pbm_L_Cnt(2Byte)), M01 = M - 다음 Page => M02 인쇄 하지 않음! M00 = 문구 출력  //#0099
		
		
		if(tempBuff[0] == 'M')
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPPGet - 2 [%3.3s]",tempBuff);
			if (memcmp(&tempBuff[1], "01", 2) == 0) 
			{
				memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], &tempBuff[StartPosX], tmpCnt - 3);
			}
		}
		else
		{
			memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], &tempBuff[StartPosX + 3], tmpCnt - 3);		
		}

		
		DataArea->DataLength[DataArea->p][(DataArea->y)+i] = tmpCnt - 3;
		Cnt+= 83;	
	}
	
	return Cnt;
}

// DATA부/통장부프린트(거래)
int CTranCmn::fnAPP_KJB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)	// #N0181
{
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KJB_RecvPPPGet()");

	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0;
	BYTE	hanFlag = 0;
	BYTE	tempBuff[256];

	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// 인쇄시작라인
	Cnt += 2;
	//DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		
	DataArea->ParaLength = 0;								// 인쇄라인수가 실제 라인수가 아님, 카운트 필요 (의미없음 00:없음, 01:있음)
	Cnt += 2;
	DataArea->x = 0;
	for(int i=0; ; i++)
	{
		if (pRecvArea[Cnt] != 'D')							// 인쇄라인 여부확인
			break;

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KJB_RecvPPPGet():Cnt[%d]", Cnt);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KJB_RecvPPPGet()", &pRecvArea[Cnt], 71);

		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		for(int ii=1; ii<71; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// 한글시작처리
			{
				if(hanFlag != 1)							// 영문상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// 한글끝처리
			{
				if(hanFlag == 1)							// 한글상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}
//		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[fnAPP_IBK_RecvPPPGet P]", tempBuff, tmpCnt);
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_IBK_RecvPPPGet[%s]",tempBuff);
		memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][(DataArea->y)+i] = tmpCnt;
		Cnt += 71;
		DataArea->ParaLength ++;
	}

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KJB_RecvPPPGet():return(%d) - DataArea->ParaLength[%d]", Cnt, DataArea->ParaLength);
	return Cnt;
}

int CTranCmn::fnAPP_HANA_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0, StartPosX = 0;
	BYTE	hanFlag = 0;
	char	szBuff[256];
	BYTE	tempBuff[256];
	BYTE	tempArea1[10], tempArea2[10];
	BYTE	HanLeft = 0, HanRight = 0;
	
	//Accept.Index = 0;
	memset(szBuff, 0x00, sizeof(szBuff));
	//memset(Accept.CrmDummy, 0x00, sizeof(Accept.CrmDummy));
	
	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// 인쇄시작라인
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// 인쇄라인수
	Cnt += 2;
	DataArea->x = 0;

	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", RecvArea, Length);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_HANA_RecvPPPGet():return(%d) - DataArea->ParaLength[%d]", DataArea->y, DataArea->ParaLength);
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		
		for(int ii=0; ii<83; )
		{
			if(ii == 0 && (pRecvArea[Cnt+ii] == 'D'))
			{
				fnAPP_HANA_RecvPPPMove(' ', DataArea, 1);
				ii += 3;
			}
			else
			if(ii == 0 && (pRecvArea[Cnt+ii] == 'M') && memcmp(&pRecvArea[Cnt+ii+1], "25", 2) == 0)
			{				
				fnAPP_HANA_RecvPPPMove(' ', DataArea, 1);
				ii += 3;
			}
			else
			if ((pRecvArea[Cnt+ii] == '#')		&&						// 가로확대일경우 무시
			(pRecvArea[Cnt+ii+1] == '$')		&&
			(pRecvArea[Cnt+ii+2] == '#')		&&
			(pRecvArea[Cnt+ii+3] == 0x1B)	&&
			((pRecvArea[Cnt+ii+4] == 0x33)	||
			 (pRecvArea[Cnt+ii+4] == 0x34))	)
				ii += 5;
			else
			{
				if (pRecvArea[Cnt+ii] & 0x80)							// 한글1st
				{
					if (pRecvArea[Cnt + ii + 1] & 0x80)					// 한글2st
					{
						if (!hanFlag)
						{											// 한글Start
							fnAPP_HANA_RecvPPPMove(PbHanStart[0], DataArea, 1);
							fnAPP_HANA_RecvPPPMove(PbHanStart[1], DataArea, 1);
							hanFlag = TRUE;		
						}
						memset(tempArea1, 0x00, sizeof(tempArea1));
						memset(tempArea2, 0x00, sizeof(tempArea2));

						HanLeft = pRecvArea[Cnt + ii];
						ii++;
						HanRight = pRecvArea[Cnt+ ii];
						ii++;

						tempArea1[0] = HanLeft;
						tempArea1[1] = HanRight;

						CodeCvt(tempArea1, tempArea2, 2, KSIBM);
						fnAPP_HANA_RecvPPPMove(tempArea2[0], DataArea, 1);
						fnAPP_HANA_RecvPPPMove(tempArea2[1], DataArea, 1);
					}
					else
					{
						if (hanFlag)
						{											// 한글End
							fnAPP_HANA_RecvPPPMove(PbHanEnd[0], DataArea, 1);
							fnAPP_HANA_RecvPPPMove(PbHanEnd[1], DataArea, 1);
							hanFlag = FALSE;
						}
						fnAPP_HANA_RecvPPPMove(A2E(' '), DataArea, 1);
						fnAPP_HANA_RecvPPPMove(pRecvArea[Cnt +ii+ 1], DataArea, 1);
						ii += 2;
					}
				}
				else												// 영문
				{
					if (hanFlag)
					{												// 한글End
						fnAPP_HANA_RecvPPPMove(PbHanEnd[0], DataArea, 1);
						fnAPP_HANA_RecvPPPMove(PbHanEnd[1], DataArea, 1);
						hanFlag = FALSE;
					}

					fnAPP_HANA_RecvPPPMove(pRecvArea[Cnt+ii], DataArea, 1);
					
					ii++;
				}
			}
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HANA_RecvPPPGet[%d]",DataArea->DataLength[DataArea->p][(DataArea->y)]);
		//DataArea->DataLength[DataArea->p][(DataArea->y+i)] = ii;
		DataArea->y ++;
		/*
		if (DataArea->y >= itemof(DataArea->Data[0]))		// 라인체크
		{
			DataArea->y -= 24;
			DataArea->p++;
		}
		*/
		
		DataArea->x = 0;
		Cnt += 83;	
	}
	
	DataArea->y = Asc2Int(&pRecvArea[4+2], 2)-1;
	DataArea->x = 0;
	DataArea->p = 0;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HANA_RecvPPPGet__!!![%d]",DataArea->DataLength[DataArea->p][(DataArea->y)]);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "DataArea->Data", DataArea->Data, Length);
	return Cnt;
}


//szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3 + 2] 
int CTranCmn::fnAPP_KB_RecvPPPGet(void* RecvArea, PPPRecvKb* DataArea, int Length) //#N0226
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0, StartPosX = 0;
	BYTE	hanFlag = 0;
	char	szBuff[256];
	BYTE	tempBuff[256];

	memset(szBuff, 0x00, sizeof(szBuff));

	Cnt = 1 + 1 + 1 + 1 + 1; // 5
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;                                               // 지정면
	DataArea->Bp = Asc2Int(&pRecvArea[Cnt], 3);				// 인쇄시작라인
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 3);				// 인쇄시작라인
	Cnt += 3;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// 인쇄라인수
	Cnt += 2;

	Cnt += 1; //FS

	DataArea->x = 0;
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
	
		for(int ii=0; ii<80; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// 한글시작처리
			{
				if(hanFlag != 1)							// 영문상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// 한글끝처리
			{
				if(hanFlag == 1)							// 한글상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}

		memcpy(&DataArea->Data[DataArea->p][DataArea->y][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][DataArea->y] = tmpCnt;
		//Cnt+= 68;	
		Cnt+= 80;
		
		DataArea->y++;

		if (DataArea->y >= itemof(DataArea->Data[0]))		    // 라인체크
		{
			DataArea->y -= 24;
			DataArea->p++;
		}
	}
	
	return Cnt;
}


// DATA부/통장부프린트(거래)
int CTranCmn::fnAPP_CTB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)	// #N0252
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0;
	BYTE	hanFlag = 0;
	BYTE	tempBuff[256];

	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = 0;	//Asc2Int(&pRecvArea[Cnt], 2)-1;	// 인쇄시작라인 - D Type 뒤 두자리 숫자로 인자라인 결정
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// 인쇄라인수
	Cnt += 2;
	DataArea->x = 0;
	
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		if (pRecvArea[Cnt] == 'M')							// M Type (00:추가정리요망, 01:다음페이지, 02:인쇄끝, 03:통장교체)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CTB_RecvPPPGet():DataType[%3.3s] M00:추가정리요망, M01:다음페이지, M02:인쇄끝, M03:통장교체", &pRecvArea[Cnt]);
			if (!memcmp(&pRecvArea[Cnt + 1], "01", 2))
			{
				DataArea->p ++;								// 다음페이지
				DataArea->y = 0;							// 라인초기화
				Cnt+= 83;
				continue;
			}
			else
			if (!memcmp(&pRecvArea[Cnt + 1], "00", 2))		// M00은 추가정리내역 존재
			{
				PbNextTranFlag = TRUE;
				break;
			}
			else
			if (!memcmp(&pRecvArea[Cnt + 1], "02", 2))
				break;
			else
			if (!memcmp(&pRecvArea[Cnt + 1], "03", 2))		// M03은 통장이월
			{
				if(DataArea->ParaLength == 0)				
				{
					m_pDevCmn->fnSCR_DisplayString(2, "[통장이월]");
					m_pDevCmn->fnSCR_DisplayString(3, "창구에서 통장을 재발급 받으십시오 ");
					m_pDevCmn->fnSCR_DisplayScreen(701);
					Delay_Msg(3000);
					fnAPP_CancelProc(T_OK);
				}
			}
			else
				break;
		}

		if (pRecvArea[Cnt] == 'D')							// D Type (두자리 숫자가 넘길 라인수)
		{
			if (DataArea->y == 0)
			{
				tmpCnt = Asc2Int(&pRecvArea[Cnt + 1], 2);
				if ((tmpCnt >= 3) &&(tmpCnt <= 14))
					DataArea->y += tmpCnt - 3;		// 씨티은행은 3줄부터 시작 (D03)
				else
				if ((tmpCnt >= 17) &&(tmpCnt <= 29))
					DataArea->y += tmpCnt - 5;		// 씨티은행은 3줄부터 시작 + 재봉선 2줄 (D17)
				else
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CTB_RecvPPPGet():DataType[%3.3s] 시작줄 범위 오류 (유효범위 3~14, 17~19)", &pRecvArea[Cnt]);

				if (DataArea->y < 0)
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CTB_RecvPPPGet():DataType[%3.3s], DataArea->p[%d], DataArea->y[%d -> 0]", &pRecvArea[Cnt], DataArea->p, DataArea->y);
					DataArea->y = 0;
				}
			}
			else
				;	//DataArea->y ++; 아래에서 증가시킴
				// DataArea->y += Asc2Int(&pRecvArea[Cnt + 1], 2);	재봉선을 위한 D03도 무시한다

			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CTB_RecvPPPGet():DataType[%3.3s], DataArea->p[%d], DataArea->y[%d]", &pRecvArea[Cnt], DataArea->p, DataArea->y);
		}

		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		for(int ii=3; ii<83; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// 한글시작처리
			{
				if(hanFlag != 1)							// 영문상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// 한글끝처리
			{
				if(hanFlag == 1)							// 한글상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}
//		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[fnAPP_IBK_RecvPPPGet P]", tempBuff, tmpCnt);
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_IBK_RecvPPPGet[%s]",tempBuff);
		memcpy(&DataArea->Data[DataArea->p][DataArea->y][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][DataArea->y] = tmpCnt;
		DataArea->y ++;
		Cnt+= 83;
	}
	return Cnt;
}

int CTranCmn::fnAPP_SC_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)  // #N0245
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		HanFlg = FALSE, HanCnt = 0, HalfCnt = 0;
	int		Cnt = 0;
	int		SpaceCnt = 0, i = 0;
	int		PrintCnt = 0;
	int		StartLine = 0;
	BYTE	HanLeft = 0, HanRight = 0;
	BYTE	HanIBM[2], HanKS[2];
	int		HanCmdCnt = 0;

	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;	// 인쇄시작라인 - D Type 뒤 두자리 숫자로 인자라인 결정
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// 인쇄라인수
	Cnt += 2;
	DataArea->x = 0;

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():Length[%d], DataArea->y[%d], DataArea->ParaLength[%d]", Length, DataArea->y, DataArea->ParaLength);

	while (Cnt < Length)
	{
		if ((pRecvArea[Cnt] == RecvCmdCR)	||					// Carrige Return
			(pRecvArea[Cnt] == RecvCmdNL)	||					// LINE FEED+CARRIAGE RETURN
			(pRecvArea[Cnt] == RecvCmdESC)	)					// 제어시작
		{
			//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdCR{0x15}, RecvCmdNL{0x0d}, RecvCmdESC{0x27}", Cnt, pRecvArea[Cnt]);
			if (HanFlg)
			{													// 한글End
				fnAPP_RecvPPPMove(PbHanEnd[0], DataArea, 1);
				fnAPP_RecvPPPMove(PbHanEnd[1], DataArea, 1);

				HanCmdCnt += 2;									// 한글제어문자수증가 : 2003.11.13
			}
			if (HanCnt & 0x01)									// Han First
			{
				fnAPP_RecvPPPMove(' ', DataArea, 1);
			}
			HanFlg = FALSE;
			HanCnt = 0;
		}

		if (pRecvArea[Cnt] == RecvCmdESC)
		{
			//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdESC{0x27}", Cnt, pRecvArea[Cnt]);
			if (pRecvArea[Cnt + 1] == RecvCmdROW)
			{
				//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdROW{0x82}", Cnt + 1, pRecvArea[Cnt + 1]);
				if (pRecvArea[Cnt + 2])
					DataArea->y = pRecvArea[Cnt + 2] - 1;
				else
					DataArea->y = pRecvArea[Cnt + 2];

				DataArea->x = 0;

				if (Cnt + 2 > Length)
					break;

				Cnt += 2;
			}
			else
			if (pRecvArea[Cnt + 1] == RecvCmdLF)
			{
				//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdLF{0x71}", Cnt + 1, pRecvArea[Cnt + 1]);
				if (DataArea->x == 0)							// 신규라인시 
				{
					HanCmdCnt = 0;								// 한글코드문자수초기화
					SpaceCnt = 0;								// 삽입한공백문자수초기화
				}

				if ((DataArea->x - HanCmdCnt) == (pRecvArea[Cnt + 2]))
					;											// 인덱스VS인덱스비교 : 2004.03.11
				else
				if ((DataArea->x - HanCmdCnt) > (pRecvArea[Cnt + 2]))
				{												// 인덱스VS인덱스비교 : 2004.03.11
					DataArea->x = pRecvArea[Cnt + 2];
					DataArea->x += HanCmdCnt;					// 한글제어코드 갯수 보정
					if (DataArea->x < 0)
						DataArea->x = 0;
				}
				else
				{
					SpaceCnt = pRecvArea[Cnt + 2] - DataArea->x;	
																// Index 0 base처리
					SpaceCnt += HanCmdCnt;						// 한글제어코드 갯수 보정
					for( i = 0 ; i < SpaceCnt ; i++ )
						fnAPP_RecvPPPMove(' ', DataArea, 1);
				}

				Cnt += 2;
			}
			else
			{
				Cnt += 1;
				Cnt += ESCAddedData[pRecvArea[Cnt + 1]];
			}
		}
		else
		{
			if ((pRecvArea[Cnt] == RecvCmdCR)	||
				(pRecvArea[Cnt] == RecvCmdNL)	)
			{													// 라인개행
				//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdCR{0x15}, RecvCmdNL{0x0d}", Cnt, pRecvArea[Cnt]);
				if (HanFlg)
				{												// 한글End
					fnAPP_RecvPPPMove(PbHanEnd[0], DataArea, 1);
					fnAPP_RecvPPPMove(PbHanEnd[1], DataArea, 1);

					HanCmdCnt += 2;								// 한글제어문자수증가 : 2003.11.13
					HanFlg = FALSE;
				}
				DataArea->y++;
				DataArea->x = 0;
			}
			else
			if (pRecvArea[Cnt] == RecvCmdSO)					// Shift Out
			{
				//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdSO{0x0e}", Cnt, pRecvArea[Cnt]);
				if (!HanFlg)									// Eng Mode
				{
					fnAPP_RecvPPPMove(PbHanStart[0], DataArea, 1);
					fnAPP_RecvPPPMove(PbHanStart[1], DataArea, 1);

					HanCmdCnt += 2;								// 한글제어문자수증가 : 2003.11.13
				}
				HanFlg = TRUE;
				HanCnt = 0;
			}
			else
			if (pRecvArea[Cnt] == RecvCmdSI)					// Shift In
			{
				//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdSI{0x0f}", Cnt, pRecvArea[Cnt]);
				if (HanFlg)										// Han Mode
				{
					fnAPP_RecvPPPMove(PbHanEnd[0], DataArea, 1);
					fnAPP_RecvPPPMove(PbHanEnd[1], DataArea, 1);

					HanCmdCnt += 2;								// 한글제어문자수증가 : 2003.11.13
				}
				if (HanCnt & 0x01)								// Han First
				{
					fnAPP_RecvPPPMove(' ', DataArea, 1);
				}
				HanFlg = FALSE;
				HanCnt = 0;
			}
			else												// Data
			{
				if (HanFlg)										// Han Mode
				{
					HanCnt++;
					if (HanCnt & 0x01)							// Han First
					{
						HanLeft = pRecvArea[Cnt];
					}
					else										// Han Second
					{
						HanRight = pRecvArea[Cnt];
						HanIBM[0] = HanLeft;
						HanIBM[1] = HanRight;
						CodeCvt(&HanIBM, &HanKS, 2, IBMKS);
						HanLeft = HanKS[0];
						HanRight = HanKS[1];
						fnAPP_RecvPPPMove(HanLeft, DataArea, 1);// (한글짝수)
						fnAPP_RecvPPPMove(HanRight, DataArea, 1);
					}
				}
				else											// Eng Mode
				{
					fnAPP_RecvPPPMove(E2A(pRecvArea[Cnt]), DataArea, 1);
				}
			}
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x, %c] }", Cnt, pRecvArea[Cnt],E2A(pRecvArea[Cnt]));
		Cnt++;
	}

	if (HanFlg)
	{															// 한글End
		fnAPP_RecvPPPMove(PbHanEnd[0], DataArea, 1);
		fnAPP_RecvPPPMove(PbHanEnd[1], DataArea, 1);
	}
	if (HanCnt & 0x01)											// Han First
	{
		fnAPP_RecvPPPMove(' ', DataArea, 1);
	}
	if (HalfCnt & 0x01)											// (줄당짝수)
	{
		fnAPP_RecvPPPMove(PbHalfSpace, DataArea, 1);
	}

	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "DataArea->Data", DataArea->Data, Length);
	return Cnt;
}

// DATA부/통장부프린트(거래)
int CTranCmn::fnAPP_KNB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)	// #N0277
{
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KNB_RecvPPPGet()");
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0, hanStart = -1;
	BYTE	hanFlag = 0;
	BYTE	tempBuff[256];

	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// 인쇄시작라인
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// 인쇄라인수
	Cnt += 2;
	DataArea->x = 0;
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		memcpy(tempBuff, &pRecvArea[Cnt], 62);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KNB_RecvPPPGet_Src[%s][%d]", tempBuff, (DataArea->y)+i);
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		for(int ii=0; ii<62; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// 한글시작처리
			{
				if(hanFlag != 1)							// 영문상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
					hanStart = tmpCnt;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// 한글끝처리
			{
				if(hanFlag == 1)							// 한글상태
				{
					if (hanStart >= 0)
					{
						MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[KS -> TG] start:%d, size:%d", hanStart, tmpCnt - hanStart);
						HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[KS]", &tempBuff[hanStart], tmpCnt - hanStart);
						m_pDevCmn->fnUTL_HangulCodeConvert(1, (char *)&tempBuff[hanStart], (char *)&tempBuff[hanStart], tmpCnt - hanStart);	// 완성형 -> 조합형 변환
						HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TG]", &tempBuff[hanStart], tmpCnt - hanStart);
					}

					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
					hanStart = -1;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}

		if(hanFlag == 1)							// 한글상태
		{
			if (hanStart >= 0)
			{
				MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[KS -> TG] start:%d, size:%d", hanStart, tmpCnt - hanStart);
				HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[KS]", &tempBuff[hanStart], tmpCnt - hanStart);
				m_pDevCmn->fnUTL_HangulCodeConvert(1, (char *)&tempBuff[hanStart], (char *)&tempBuff[hanStart], tmpCnt - hanStart);	// 완성형 -> 조합형 변환
				HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TG]", &tempBuff[hanStart], tmpCnt - hanStart);
			}

			memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
			tmpCnt += 2;
			hanFlag = 0;
			hanStart = -1;
		}

		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[fnAPP_KNB_RecvPPPGet P]", tempBuff, tmpCnt);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KNB_RecvPPPGet[%s][%d]", tempBuff, (DataArea->y)+i);
		memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][(DataArea->y)+i] = tmpCnt;
		Cnt+= 62;
	}

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KNB_RecvPPPGet():return(%d) - DataArea->ParaLength[%d]", Cnt, DataArea->ParaLength);
	return Cnt;
}

// DATA부/통장부프린트(거래)
int CTranCmn::fnAPP_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0;
	BYTE	hanFlag = 0;
	BYTE	tempBuff[256];


	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// 인쇄시작라인
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// 인쇄라인수
	Cnt += 2;
	DataArea->x = 0;
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		for(int ii=0; ii<75; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// 한글시작처리
			{
				if(hanFlag != 1)							// 영문상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// 한글끝처리
			{
				if(hanFlag == 1)							// 한글상태
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}
		memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][(DataArea->y)+i] = tmpCnt;
		Cnt+= 75;
	}
	return Cnt;
}

// 통장영역Move
int CTranCmn::fnAPP_NONGHYUP_RecvPPPMove(BYTE Data, PPPRecvNon* pDataArea, int Length)
{
	int		i;

	for (i = 0; i < Length; i++)
	{
		if ((pDataArea->p >= itemof(pDataArea->Data))		||		// 한계검증
			(pDataArea->y >= itemof(pDataArea->Data[0]))	||
			(pDataArea->x >= itemof(pDataArea->Data[0][0])))
			break;
		
		pDataArea->Data[pDataArea->p][pDataArea->y][pDataArea->x++] = Data;
	}
	return T_OK;
}

// 통장영역Move
int CTranCmn::fnAPP_RecvPPPMove(BYTE Data, PPPRecv* pDataArea, int Length)
{
	int		i;
	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->p >= itemof(pDataArea->Data))	 ||		// 한계검증
			(pDataArea->y >= itemof(pDataArea->Data[0])) ||
			(pDataArea->x >= itemof(pDataArea->Data[0][0])))
			break;
		pDataArea->Data[pDataArea->p][pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line 초기설정
		{
			pDataArea->Bp = pDataArea->p;
			pDataArea->Ep = pDataArea->p;
		}
		pDataArea->ParaLength += Length;						// 자료유무구하기
		if (pDataArea->p < pDataArea->Bp)						// Find Begin Page
			pDataArea->Bp = pDataArea->p;
		if (pDataArea->p > pDataArea->Ep)						// Find End Page
			pDataArea->Ep = pDataArea->p;

		pDataArea->DataLength[pDataArea->p][pDataArea->y] = pDataArea->x;		
																// DATA Length 구하기
	}
	return T_OK;
}


// 통장영역Move #N0214
int CTranCmn::fnAPP_HANA_RecvPPPMove(BYTE Data, PPPRecv* pDataArea, int Length)
{
	int		i;

	for (i = 0; i < Length; i++)
	{
		
		if ((pDataArea->p >= itemof(pDataArea->Data))		||		// 한계검증
		//	(pDataArea->y >= itemof(pDataArea->Data[0]))	||
			(pDataArea->x >= itemof(pDataArea->Data[0][0])))
			break;
		
		
		pDataArea->Data[pDataArea->p][pDataArea->y][pDataArea->x++] = Data;
		pDataArea->DataLength[pDataArea->p][pDataArea->y] = pDataArea->x;		// DATA Length 구하기
		
	}
	
	return T_OK;
}

// DATA부/메세지편집표시(거래/개국)
int CTranCmn::fnAPP_RecvPMEGet(void* RecvArea, PMERecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		TranCnt	= 0, i = 0, j = 0;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPMEGet: TranCode[%d] TranSend[%d]", TranCode, TranSend);
	if( ((TranCode == TC_INQSECUR	||				// 유가증권조회
		  TranCode == TC_INQUIRYA	||				// 거래내역조회
		  TranCode == TC_TRANSTOC	||				// 유가증권대체
		  TranCode == TC_REQSTOCK	||				// 청약신청
		  TranCode == TC_INQSTOCK	||				// 청약취소
		  TranCode == TC_CANSTOCK)	&&				// 청약내역조회
		 TranSend == 1 )	||						// 1차조회일 경우만
		(TranCode == TC_INQPRINT) )				// 잔고출력조회
	{
		for(int i=0; i<Asc2Int(&pRecvArea[103], 2) && i<10; i++)
		{
			if(TranCode == TC_INQSECUR)				// 유가증권조회
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 105+(i*85), 105+(i*85)+84);
			else if(TranCode == TC_INQUIRYA)		// 거래내역조회
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 161+(i*130), 161+(i*130)+129);
			else if(TranCode == TC_TRANSTOC)		// 유가증권대체
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 246+(i*80), 246+(i*80)+79);
			else if(TranCode == TC_REQSTOCK)		// 청약신청
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 317+(i*103), 317+(i*103)+102);
			else if(TranCode == TC_CANSTOCK)		// 청약취소
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 230+(i*105), 230+(i*105)+104);
			else if(TranCode == TC_INQSTOCK)		// 청약내역조회
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 125+(i*181), 125+(i*181)+180);
			else if(TranCode == TC_INQPRINT)		// 잔고출력조회
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 145+(i*110), 145+(i*110)+109);

			DataArea->y++;
			DataArea->x = 0;
		}
	}
	else
	if (TranCnt == TC_TOTAL)
	{
// 		fnAPP_RecvPMESubGet(pRecvArea, DataArea,  28,  37);		// 지급현금
// 		DataArea->y++;
// 		DataArea->x = 0;
// 		fnAPP_RecvPMESubGet(pRecvArea, DataArea,  38,  47);		// 지급자점
// 		DataArea->y++;
// 		DataArea->x = 0;
// 		fnAPP_RecvPMESubGet(pRecvArea, DataArea,  48,  57);		// 입금현금
// 		DataArea->y++;
// 		DataArea->x = 0;
// 		fnAPP_RecvPMESubGet(pRecvArea, DataArea,  58,  67);		// 입금자점
// 		DataArea->y++;
// 		DataArea->x = 0;
// 		fnAPP_RecvPMESubGet(pRecvArea, DataArea,  68,  77);		// 입금타점
// 		DataArea->y++;
// 		DataArea->x = 0;
	}

	return Length;
}

// 메세지편집표시Sub
int CTranCmn::fnAPP_RecvPMESubGet(void* RecvArea, PMERecv* DataArea, int Cnt, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		HanFlg = FALSE, HanCnt = 0;
	BYTE	HanLeft = 0, HanRight = 0;

	while (Cnt <= Length)										// Length Check
	{
		fnAPP_RecvPMEMove(pRecvArea[Cnt], DataArea, 1);
		Cnt++;													// 후의형증가
	}

	if (HanCnt & 0x01)											// Han First
	{
		fnAPP_RecvPMEMove(' ', DataArea, 1);
	}
	return Cnt;
}

// 메세지편집영역Move
int CTranCmn::fnAPP_RecvPMEMove(BYTE Data, PMERecv* pDataArea, int Length)
{
	int		i;

	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->y >= itemof(pDataArea->Data)) ||		// 한계검증
			(pDataArea->x >= itemof(pDataArea->Data[0])))
			break;
		
		pDataArea->Data[pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line 초기설정
		{
			pDataArea->By = pDataArea->y;
			pDataArea->Ey = pDataArea->y;
		}
		pDataArea->ParaLength += Length;						// 자료유무구하기
		if (pDataArea->y < pDataArea->By)						// Find Begin Line
			pDataArea->By = pDataArea->y;
		if (pDataArea->y > pDataArea->Ey)						// Find End Line
			pDataArea->Ey = pDataArea->y;
	}
	return T_OK;
}

// DATA부/메세지표시(거래)
int CTranCmn::fnAPP_RecvPMDGet(void* RecvArea, PMDRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		HanFlg = FALSE, HanCnt = 0;
	BYTE	HanLeft = 0, HanRight = 0;
	BYTE	HanIBM[2], HanKS[2];
	int		Cnt = 0, i, j, DataFlag = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPMDGet", pRecvArea, Length, IBMKS);
	while (Cnt <= Length)										// Length Check
	{
		if (pRecvArea[Cnt] == RecvCmdNL)	
		{
			DataArea->y ++;
			DataArea->x = 0;
		}
		else
		if (pRecvArea[Cnt] == RecvCmdSO)						// Shift Out
		{
			HanFlg = TRUE;
			HanCnt = 0;
		}
		else
		if (pRecvArea[Cnt] == RecvCmdSI)						// Shift In
		{
			if (HanCnt & 0x01)									// Han First
			{
				fnAPP_RecvPMDMove(' ', DataArea, 1);
			}
			HanFlg = FALSE;
			HanCnt = 0;
		}
		else													// Data
		{
			if (HanFlg)											// Han Mode
			{
				HanCnt++;
				if (HanCnt & 0x01)								// Han First
				{
					HanLeft = pRecvArea[Cnt];
				}
				else											// Han Second
				{
					HanRight = pRecvArea[Cnt];
					HanIBM[0] = HanLeft;
					HanIBM[1] = HanRight;
					CodeCvt(&HanIBM, &HanKS, 2, IBMKS);
					HanLeft = HanKS[0];
					HanRight = HanKS[1];
					if (DataArea->x >= 39)						// 한글Word처리
					{
						DataArea->y += 2;
						DataArea->x = 0;
					}
					fnAPP_RecvPMDMove(HanLeft, DataArea, 1);	// (한글짝수)
					fnAPP_RecvPMDMove(HanRight, DataArea, 1);
				}
			}
			else												// Eng Mode
			{
				fnAPP_RecvPMDMove(pRecvArea[Cnt], DataArea, 1);
			}
		}
		Cnt++;													// 후의형증가
	}

	if (HanCnt & 0x01)											// Han First
	{
		fnAPP_RecvPMDMove(' ', DataArea, 1);
	}
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPMDGet", DataArea, Length);

	if (DataArea->ParaLength)									// 자료존재
	{
		for (i = DataArea->By; i <= DataArea->Ey; i++)			// 자료보정
		{
			DataFlag = FALSE;
			for (j = (itemof(DataArea->Data[0]) - 1); j >= 0; j--)
			{
				if (DataArea->Data[i][j] != 0)
					DataFlag = TRUE;
				else
				{
					if (DataFlag)
						DataArea->Data[i][j] = ' ';
				}
			}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PMDRecvData.DataSet[%d] : [%s]", i, PMDRecvData.Data[i] );
		}
	}

	int n = -1;													// ALL-SPACE에 대한 자료보정
	for (i = itemof(DataArea->Data) - 1 ; i >= 0; i--)			// 뒤에서부터 ALL-SPACE가 아닌 라인 인덱스구하기
	{
		if (!IsSpace((char*)GetString(DataArea->Data[i], 40, 40).GetBuffer(0), 40))
		{
			n = i;
			break;
		}
	}

	if (n == -1)												// NOT-ALL-SPACE라인 인덱스가 -1이면 더이상처리없음
	{
		DataArea->ParaLength = 0;
	}

	return Cnt - 1;
}

// 화면영역Move
int CTranCmn::fnAPP_RecvPMDMove(BYTE Data, PMDRecv* pDataArea, int Length)
{
	int		i;

	if (pDataArea->x >= 40)										// 화면영역분리처리
	{
		pDataArea->y += 2;
		pDataArea->x = 0;
	}

	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->y >= itemof(pDataArea->Data)) ||		// 한계검증
			(pDataArea->x >= itemof(pDataArea->Data[0])))
			break;
		
		pDataArea->Data[pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line 초기설정
		{
			pDataArea->By = pDataArea->y;
			pDataArea->Ey = pDataArea->y;
		}
		pDataArea->ParaLength += Length;						// 자료유무구하기
		if (pDataArea->y < pDataArea->By)						// Find Begin Line
			pDataArea->By = pDataArea->y;
		if (pDataArea->y > pDataArea->Ey)						// Find End Line
			pDataArea->Ey = pDataArea->y;
	}
	return T_OK;
}

// 카드M/S기록
int CTranCmn::fnAPP_RecvPCWGet(void* RecvArea, PCWRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0;

	return Cnt;
}
		
// 카드영역Move
int CTranCmn::fnAPP_RecvPCWMove(BYTE Data, PCWRecv* pDataArea, int Length)
{
	int		i;

	for (i = 1; i <= Length; i++)
	{
		if (pDataArea->x >= sizeof(pDataArea->Data))			// 한계검증
			break;
		
		pDataArea->Data[pDataArea->x++] = Data;
		pDataArea->ParaLength += Length;
	}
	return T_OK;
}

// DES TMK/TPK수신처리
// DES OutGoing거래 2004.05.31
int CTranCmn::fnAPP_RecvDesKeyProc(PMERecv* DataBuff)
{
	BYTE SixteenNulls[17];
	BYTE TMK_INIT[17] = "0000000000000000";

	BYTE	MasterKey[32];
	BYTE	MKCheckValue[6];
	
	memset(SixteenNulls, 0x00, sizeof(SixteenNulls));
	memset(MasterKey,	 0x00, sizeof(MasterKey));

	if (fnAPP_RecvDesKeyCheck(DataBuff) == FALSE)
	{
MsgDump(MAC_TRACE_CODE_FILE("Log"), "fnAPP_RecvDesKeyProc() 유효하지 않은 DES키 수신함");
		return T_OK;
	}

	switch (m_pProfile->DEVICE.DesHandler)
	{
	case S_DES_SET:
		memcpy(&MasterKey[0], &DataBuff->Data[1][0], 16);
		memcpy(&MKCheckValue[0], &DataBuff->Data[2][0], 6);
		break;
	case T_DES_SET:
		memcpy(&MasterKey[0], &DataBuff->Data[1][0], 16);
		memcpy(&MasterKey[16], &DataBuff->Data[2][0], 16);
		memcpy(&MKCheckValue[0], &DataBuff->Data[3][0], 6);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TMK:MasterKey", MasterKey, 32);
		break;
	default:
MsgDump(MAC_TRACE_CODE_FILE("Log"), "fnAPP_RecvDesKeyProc() 유효하지 않은 DES방식 설정됨");
		return T_OK;
	}

	if (TranCode == TC_TMKLOAD)
	{
		if (m_pDevCmn->fnDES_LoadKey(&MasterKey[0], 1))
		{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TMK:m_pDevCmn->DesRPIN", m_pDevCmn->DesRPIN, 16);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TMK:MKCheckValue", MKCheckValue, 6);
			if (memcmp(m_pDevCmn->DesRPIN, &MKCheckValue[0], 6) == 0)
			{
				m_pDevCmn->DesTMKStatus = TRUE;
				m_pProfile->GetDeviceProfile();
				m_pProfile->DEVICE.DesMasterKey = TRUE;
				m_pProfile->PutDeviceProfile();
			}
			else
			{
				m_pDevCmn->fnDES_LoadKey(TMK_INIT, 1);
			}
		}
	}
	else																// TPK
	if ((TranCode == TC_TPKLOAD) &&
	    (m_pProfile->DEVICE.DesMasterKey))
	{
		if (m_pDevCmn->fnDES_LoadKey(&MasterKey[0], 2))
		{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TPK:m_pDevCmn->DesRPIN", m_pDevCmn->DesRPIN, 16);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TPK:MKCheckValue", MKCheckValue, 6);
			if (memcmp(m_pDevCmn->DesRPIN, &MKCheckValue[0], 6) == 0)
			{
				m_pDevCmn->DesTranStatus = TRUE;
			}
		}
	}
	return T_OK;
}

// DES TMK/TPK수신 값 확인
// return : TRUE - DES MAster Key 유효함
//			FALSE - 키중 적어도 하나이상은 유효하지 않음
int CTranCmn::fnAPP_RecvDesKeyCheck(PMERecv* DataBuff)
{
	BYTE SixteenNulls[17];
	int		nReturn = FALSE;

	memset(SixteenNulls, 0x00, sizeof(SixteenNulls));

	if ((memcmp(&DataBuff->Data[0][0], SixteenNulls, 16) != 0 ) &&
		(!IsZero(&DataBuff->Data[0][0], 16) )					)
	{
		nReturn =TRUE;											// 1차 MasterKey 유효함

		if (m_pProfile->DEVICE.DesHandler == T_DES_SET)			// T-DES이면 2차키확인필요
		{
			if ((memcmp(&DataBuff->Data[1][0], SixteenNulls, 16) != 0 ) &&
				(!IsZero(&DataBuff->Data[1][0], 16) )					)
				nReturn = TRUE;									// 1/2차키 모두 유효함
			else
				nReturn = FALSE;								// 유효하지 못한 키있음
		}
	}

	return nReturn;
}
// 송신(취소출금/입금)
int CTranCmn::fnAPP_SendRecvHostCancel_PowerOff()
{
	int	ScrNum = 0;			
	m_pDevCmn->TranResult = FALSE;								// Host결과

	SendLength = 0;												// 송신Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// 송신Buffer
/////////////////////////////////////////////////////////////////////////////

	CString cstrGetReg = IniGetStr(_TRANS_POWER_DATA_INI, TRANS_SEC, "TranWithCanCelUnPackData", "CANCEL_NODATA");
	SendLength = MakePack(cstrGetReg.GetBuffer(0), SendBuffer, cstrGetReg.GetLength());

	if (SendLength < 50)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendRecvHostCancel_PowerOff(TranWithCanCelUnPackData = Length(%d):[%s]", SendLength, cstrGetReg);
		return T_OK;
	}

	ScrNum = 701;												// 진행중화면
	m_pDevCmn->fnSCR_DisplayString(1, "거래 취소정보를 전송중입니다.");
	m_pDevCmn->fnSCR_DisplayString(2, "잠시만 기다려 주십시오.");
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);						// 진행중화면
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("취소송신"));													
	if (fnAPP_SendData(SendBuffer, SendLength, LINE_SEND_TIME) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("취소송신 : 시간초과"));
		m_pDevCmn->fnAPL_StackError("7710992", "취소송신시간초과");
	}
	
	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_PROCESS);// 진행버튼 깜빡임
	
	if (fnAPP_RecvData(RecvBuffer, &RecvLength, LINE_SEND_TIME) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("거래수신 : 시간초과"));
		m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_FINISH);// 종료버튼 깜빡임

		switch(m_pDevCmn->fnNET_GetDeviceStatus())
		{
		case LINE_CLOSE:
			m_pDevCmn->fnAPL_StackError("8860293", "Line Close");
			break;
		default:
			m_pDevCmn->fnAPL_StackError("8860293", "수신시간초과");
			break;
		}

	}
	else m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("거래수신"));

	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_FINISH);// 종료버튼 깜빡임
	
	return T_OK;
}