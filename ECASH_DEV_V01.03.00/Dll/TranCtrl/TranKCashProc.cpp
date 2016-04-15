/////////////////////////////////////////////////////////////////////////////
// TranKCashProc.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Dll\DevICCard.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
//	K-CASH함수
/////////////////////////////////////////////////////////////////////////////
// K-CASH비밀번호검사
int CTranCmn::fnAPP_KCashCheckPin()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashCheckPin()");

	int		nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : 전자화폐시엔 폐쇄와 같이처리 2004.05.21
		m_pDevCmn->fnSCR_DisplayScreen(823);					// 카드읽기 : 전자화폐시만 출력 2004.05.21
		m_pDevCmn->fnKCS_CheckPin(MakeUnPack(Accept.KCashPassWord, 8));
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		break;

	case FIC_OPEN :
		m_pDevCmn->fnSCR_DisplayScreen(823);					// 읽기(카드) 2004.05.21
		m_pDevCmn->fnFIC_CheckPIN(MakeUnPack(Accept.KCashPassWord, 8));
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_CHECKPIN);
		break;

	default:
		nReturn = 9999;
		break;
	}

	if (nReturn)
	{
		if (nReturn == 17)										// 비밀번호오류
		{
			m_pDevCmn->fnKCS_PowerOn();
			fnAPD_CheckDeviceAction(DEV_MCU);
			if (m_pDevCmn->FICCardType == FIC_OPEN)
			{
				fnAPD_FICDistinctCardType(TRAN_IC_FINANCEIC);
				fnAPD_FICBasicInquiry();						// 기본정보조회
				fnAPP_AcceptKCashPassword(TRAN_IC_FINANCEIC, 204);
			}
			else
			{
				fnAPD_FICDistinctCardType(TRAN_IC_KCASH);
				fnAPP_AcceptKCashPassword(TRAN_IC_KCASH, 218);
			}

			fnAPP_KCashCheckPin();
		}
		else
		if (nReturn == 23)										// 비밀번호오류횟수초과
		{
			fnAPP_CancelProc(T_MSG, "전자 화폐 비밀번호 오류입력횟수가 초과되었습니다.",
									"은행으로 문의하여 주십시오.");
																// K-CASH비밀번호오류횟수초과
		}
		else
			fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
									"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드읽기오류
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashCheckPin():return");
	return T_OK;
}

// 전자화폐 PAN 파일읽기
int CTranCmn::fnAPP_KCashReadPan()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadPan()");

	int		nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : 전자화폐시엔 폐쇄와 같이처리 2004.05.21
		m_pDevCmn->fnKCS_ReadPan(MakeUnPack(Accept.KCashPassWord, 8));
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		break;

	case FIC_OPEN :
		m_pDevCmn->fnFIC_ReadPan(MakeUnPack(Accept.KCashPassWord, 8));
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_READPAN);
		break;

	default:
		nReturn = 9999;
		break;
	}

	if (nReturn)
	{
		if (nReturn == 17)										// 비밀번호오류
			fnAPP_CancelProc(T_MSG, "전자 화폐 카드 비밀번호가 틀립니다.",
									"처음부터 다시 거래하여 주십시오.");
																// K-CASH비밀번호오류
		else
		if (nReturn == 23)										// 비밀번호오류횟수초과
			fnAPP_CancelProc(T_MSG, "전자 화폐 비밀번호 오류입력횟수가 초과되었습니다.",
									"은행으로 문의하여 주십시오.");
																// K-CASH비밀번호오류횟수초과
		else
			fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
									"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드읽기오류
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadPan():return");
	return T_OK;
}

// 전자화폐 주계좌정보 분석처리(PAN파일정보 분석)
// 주의 : 반드시 PAN정보 조회이후 처리해야 한다.
int CTranCmn::fnAPP_KCashCheckMainAccount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashCheckMainAccount()");

	if (IsZero(m_pDevCmn->KCashInfo.Account, sizeof(m_pDevCmn->KCashInfo.Account)))
		return T_OK;											// 계좌정보가 유효한 경우에만 처리

	CString strICData("");
	CString strAccTmp("");
	char	szAccData[1024];
	CString strNewAccCheck("");									// V02-06-01-#10 장애대응:전자화폐주계좌중 신계좌확인
																//				          은행코드가 24면 신계좌(13), 20이면 구계좌(14)

	strICData.Empty();
	strAccTmp.Empty();
	memset(szAccData, 0x00, sizeof(szAccData));

	memset(  m_pDevCmn->KCashInfo.MainAccount, 0x00, sizeof(m_pDevCmn->KCashInfo.MainAccount));
	memset( &m_pDevCmn->KCashInfo.MainAccount[0], 0x01, 1);		// 업태코드[0]
	memcpy( &m_pDevCmn->KCashInfo.MainAccount[1], 
			&m_pDevCmn->KCashInfo.Account[0],  3);				// 업태코드[1]~은행코드
	
	strNewAccCheck = MakeUnPack(&m_pDevCmn->KCashInfo.Account[0], 3).Mid(3, 2);
	strAccTmp = "0" + MakeUnPack(&m_pDevCmn->KCashInfo.Account[3], 7) + "0";

	MakePack(strAccTmp.GetBuffer(0), szAccData, 16);
	memcpy( &m_pDevCmn->KCashInfo.MainAccount[4], szAccData, 8);
	strICData = "";
	strICData += ",,";											// 1/2트랙데이타 없음
	strICData += MakeUnPack(MakeUnPack(m_pDevCmn->KCashInfo.MainAccount, 52, FALSE).GetBuffer(0), 104, FALSE);
	fnAPD_CardReadCheck(strICData.GetBuffer(0));				// 트랙정보 확인

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashCheckMainAccount():return");
	return T_OK;
}

// 전자화폐 JAN 파일읽기
int CTranCmn::fnAPP_KCashReadBAmount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadBAmount()");

	int		nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : 전자화폐시엔 폐쇄와 같이처리 2004.05.21
		m_pDevCmn->fnKCS_ReadBAmount();
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		break;

	case FIC_OPEN :
		m_pDevCmn->fnFIC_ReadBAmount();
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_READBAMOUNT);
		break;

	default:
		nReturn = 9999;
		break;
	}

	if (nReturn)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadBAmount() Error [%d]:return", nReturn);
			fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
									"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// 카드읽기오류
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadBAmount():return");
	return T_OK;
}

// 전자화폐번호 읽기
int CTranCmn::fnAPP_KCashReadEID()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadEID()");

	int		nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : 전자화폐시엔 폐쇄와 같이처리 2004.05.21
		m_pDevCmn->fnKCS_ReadEPID();
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		break;

	case FIC_OPEN :
	default:
		nReturn = 9999;
		break;
	}

	if (nReturn)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadEID() Error [%d]:return", nReturn);
			fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
									"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// 카드읽기오류
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadEID():return");
	return T_OK;
}

// 전자화폐 교통정보읽기
int CTranCmn::fnAPP_KCashReadTrafficInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadTrafficInfo()");

	int		nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : 전자화폐시엔 폐쇄와 같이처리 2004.05.21
		m_pDevCmn->fnKCS_ReadTrafficInfo();
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		break;

	case FIC_OPEN :
		m_pDevCmn->fnFIC_ReadTrafficInfo();
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_READTRAFFICINFO);
		break;

	default:
		nReturn = 9999;
		break;
	}

	if (nReturn)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadTrafficInfo() Error [%d]:return", nReturn);
	// 교통정보 파일읽기는 오류를 분석하지 않도록한다.(일부거래 제외하곤 큰의미없음)
	//	fnAPP_CancelProc(T_CANCEL, NULL, NULL, NULL, 599);	// 카드읽기오류
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadTrafficInfo():return");
	return T_OK;
}

// 전자화폐 환불복구전처리
int CTranCmn::fnAPP_KCashPrepareUnloadRcvIEP()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashPrepareUnloadRcvIEP()");

	int		nReturn = 0;

	m_pDevCmn->fnSCR_DisplayScreen(823);						// 카드읽기
	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : 전자화폐시엔 폐쇄와 같이처리 2004.05.21
		m_pDevCmn->fnKCS_PrepareUnLoadRcvIEP(MakeUnPack(Accept.KCashPassWord, 8));
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		break;

	case FIC_OPEN :
		m_pDevCmn->fnFIC_PrepareUnLoadRcvIEP(MakeUnPack(Accept.KCashPassWord, 8));
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_PREUNLOADRCV);
		break;

	default:
		nReturn = 9999;
		break;
	}

	if (nReturn)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashPrepareUnloadRcvIEP() Error [%d]:return", nReturn);
			fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
									"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// 카드읽기오류
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashPrepareUnloadRcvIEP():return");
	return T_OK;
}

// K-CASH거래중지Bit확인
// 2004.02.18 : 폐쇄/개방형카드 동일처리
int CTranCmn::fnAPP_KCashVerifyTranBit()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashVerifyTranBit()");
	
	int nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : 전자화폐시엔 폐쇄와 같이처리 2004.05.21
		m_pDevCmn->fnKCS_VerifyTranBit();							// 거래중지Bit확인
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		break;

	case FIC_OPEN :
		m_pDevCmn->fnKCS_VerifyTranBit();							// 거래중지Bit확인
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		break;

	default:
		nReturn = 9999;
		break;
	}

	if (nReturn)
	{
		if (nReturn == 24)										// 거래중지Bit확인됨
			return T_ERROR;
		else
			fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
									"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// 카드읽기오류
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashVerifyTranBit():return");
	return T_OK;
}

// K-CASH거래중지Bit제거
int CTranCmn::fnAPP_KCashRemoveTranBit()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashRemoveTranBit()");

	CString strICData;
	CString strAccTmp("");
	char	szAccData[1024];

	strICData.Empty();
	strAccTmp.Empty();
	memset(szAccData, 0x00, sizeof(szAccData));

/*t	for TimeSaving
	m_pDevCmn->fnKCS_VerifyTranBit();							// 거래중지Bit확인
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData() == 24)					// 거래중지Bit
		;
	else
		fnAPP_CancelProc(T_CANCEL, NULL, NULL, NULL, 599);		// K-CASH카드읽기오류
*/
/////////////////////////////////////////////////////////////////////////////
	fnAPP_KCashReadBAmount();									// 잔액정보읽기

	m_pDevCmn->fnKCS_ReadCLog();								// 충전환불거래내역읽기
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
								"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// 카드읽기오류

	if (m_pDevCmn->KCashInfo.LoadUnloadCnt == 0)				// 충전/환불내역조회건수
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
								"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드사용불가

	memcpy(Accept.Money, Int2Asc(ByteHighOrder2Int(&m_pDevCmn->KCashInfo.LoadUnloadBuff[0][7], 4), 8), 8);
																// 거래금액백업

	TranProcSave = TranProc;
	TranCodeSave = TranCode;
	TranCode2Save = TranCode2;
	
	if (m_pDevCmn->KCashInfo.LoadUnloadBuff[0][0] == 0x80)		// 충전거래확인
	{
		TranProc = TRAN_IC;
		TranCode = TC_KCLOADRCV;								// K-CASH충전복구
		TranCode2 = 0;
		TranSend = 0;											// 발신순번 초기화
	}
	else
	if (m_pDevCmn->KCashInfo.LoadUnloadBuff[0][0] == 0x88)		// 환불거래확인
	{
		TranProc = TRAN_IC;								
		TranCode = TC_KCUNLOADRCV;								// K-CASH환불복구
		TranCode2 = 0;
		TranSend = 0;											// 발신순번 초기화
	}
	else
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
								"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드사용불가

	if (TranCode == TC_KCLOADRCV)								// K-CASH충전복구
	{															// 충전전처리(거래금액8+암호16)
		m_pDevCmn->fnKCS_PrepareLoadIEP(MakeUnPack(&m_pDevCmn->KCashInfo.LoadUnloadBuff[0][7], 4)
									+	MakeUnPack(Accept.KCashPassWord, 8));
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())
			fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
									"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드사용불가

																// 충전전처리시:LoadUnloadAccount->Account로복사
		memcpy( m_pDevCmn->KCashInfo.Account,					// PAN정보 복사처리
				m_pDevCmn->KCashInfo.LoadUnloadAccount,
				10);
	}
	else
	if (TranCode == TC_KCUNLOADRCV)								// K-CASH환불복구
	{															// 환불복구전처리(암호16)
		fnAPP_KCashPrepareUnloadRcvIEP();

																// 환불복구전처리시:Account->LoadUnloadAccount로복사
		memcpy( m_pDevCmn->KCashInfo.LoadUnloadAccount,			// PAN정보 복사처리
				m_pDevCmn->KCashInfo.Account,
				10);
	}

	fnAPP_KCashCheckMainAccount();								// 주계좌정보 확인처리(반드시 PAN읽기 이후처리)

/////////////////////////////////////////////////////////////////////////////
	AddSerialFlg = FALSE;										// 일련번호증가무설정
																// V02-06-01-#10 복구거래시 거래일련번호 추가처리

	m_pDevCmn->fnAPL_SetProcCount('3');							// 진행설정
	fnAPP_SendHost();											// 송신
	fnAPP_RecvHost();											// 수신
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// 진행설정
	else m_pDevCmn->fnAPL_SetProcCount('9');					// HOST NG

	if (((TranCode == TC_KCLOADRCV)		&&						// K-CASH충전복구
		 (m_pDevCmn->TranResult)		&&						// HOST OK
		 (KCashHostICFlag))						||				// 
		((TranCode == TC_KCUNLOADRCV)	&&						// K-CASH환불복구
		 (m_pDevCmn->TranResult)))
	{
		m_pDevCmn->fnKCS_RemoveTranBit();						// 거래중지Bit제거
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())
			fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
									"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드사용불가

		fnAPP_KCashReadBAmount();								// 잔액정보읽기

		m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
		if (TranCode == TC_KCLOADRCV)							// 충전복구거래시
			m_pDevCmn->fnSCR_DisplayScreen(546, K_30_WAIT, PIN_MENU_MODE);// 복구거래확인
		else
		if (TranCode == TC_KCUNLOADRCV)							// 환불복구거래시
			m_pDevCmn->fnSCR_DisplayScreen(547, K_30_WAIT, PIN_MENU_MODE);// 복구거래확인
		else
			m_pDevCmn->fnSCR_DisplayScreen(546, K_30_WAIT, PIN_MENU_MODE);// 복구거래확인

		m_pDevCmn->fstrSCR_GetKeyString();

		fnAPP_PSPProc(DEV_JPR);									// 저널프린트
		fnAPP_PMDProc();										// 메세지표시
		fnAPP_PSPProc(DEV_SPR);							        // 명세프린트
	}
	else
	if ((TranCode == TC_KCLOADRCV)		&&						// K-CASH충전복구
		(m_pDevCmn->TranResult))								// HOST OK
	{															// 충전요구(난수16+서명8)
		m_pDevCmn->fnKCS_LoadIEP(GetSprintf("%16.16s", GetString(PMERecvData.Data[0], 16, 16).GetBuffer(0))
							+	 GetSprintf("%8.8s",   GetString(PMERecvData.Data[1],  8,  8).GetBuffer(0)));
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())
			fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
									"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드사용불가

		fnAPP_PSPProc(DEV_JPR);							        // 저널프린트
//		fnAPP_PMDProc();										// 메세지표시
//		fnAPP_PSPProc(DEV_SPR);									// 명세프린트
		
		/* ----------------------------------------------------------------
		// 충전성거래시 보고거래하지 않음
		// 신시스템 적용시 업무팀 요청사항
		// 2004.06.10
		m_pDevCmn->fnAPL_SetProcCount('3');						// 진행설정
		fnAPP_SendHost();										// 송신
		fnAPP_RecvHost();										// 수신
		if (m_pDevCmn->TranResult)								// HOST OK
			m_pDevCmn->fnAPL_SetProcCount('5');					// 진행설정
		else m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG
		------------------------------------------------------------------ */
//		if (m_pDevCmn->TranResult)								// HOST OK
//		{
//			fnAPP_KCashReadBAmount();							// 잔액정보읽기
//
//			m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
//			m_pDevCmn->fnSCR_DisplayScreen(546, K_30_WAIT, PIN_MENU_MODE);// 복구거래확인
//			m_pDevCmn->fstrSCR_GetKeyString();
//		}
	}
	else
	{
		if (KCashHostICFlag == STOPBITREMOVE)
		{
			fnAPP_PSPProc(DEV_JPR);								// 저널프린트
			m_pDevCmn->fnKCS_RemoveTranBit();					// 거래중지Bit제거
			fnAPD_CheckDeviceAction(DEV_MCU);
		}
		else
		{
			fnAPP_PSPProc(DEV_JPR);								// 저널프린트
			fnAPP_PMDProc();									// 메세지표시
			fnAPP_PSPProc(DEV_SPR);								// 명세프린트

			fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU );
																// 동작완료검지&장애검지
			fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);		// 매체방출	
			fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );
																// 매체방출수취대기회수
			fnAPP_TerminateProc(T_OK);							// 정상종료
		}
	}

	TranProc = TranProcSave;
	TranCode = TranCodeSave;
	TranCode2 = TranCode2Save;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashRemoveTranBit():return");
	return T_OK;
}

// K-CASH충전
int CTranCmn::fnAPP_KCashLoad()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashLoad()");
	BYTE	TranAmount[4];

	fnAPP_AcceptPassword();										// 비밀번호입력
	fnAPP_AcceptKCashMoney();									// K-CASH금액입력

	memset(TranAmount, 0x00, sizeof(TranAmount));
	Asc2IntHighOrder(Accept.Money, TranAmount, 8, 4);			// 금액변환

																// 충전전처리(거래금액8+암호16)
	m_pDevCmn->fnKCS_PrepareLoadIEP(MakeUnPack(TranAmount, 4)
								+	MakeUnPack(Accept.KCashPassWord, 8));
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
								"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// 오류발생

/////////////////////////////////////////////////////////////////////////////
	CString strAccount;											// 주계좌번호불일치체크
	int		nCompareAccount = 0;

	strAccount = MakeUnPack(&m_pDevCmn->KCashInfo.LoadUnloadAccount[3], 7, FALSE);
	if (CardTran & MENU_J3)										// 자행카드인경우
	{
		nCompareAccount = memcmp(&pCardData3->AccountNo[1], strAccount.GetBuffer(0), 14);
	}
	else														// 자행이 아닌경우
		nCompareAccount = memcmp(&pCardData3->AccountNo[0], strAccount.GetBuffer(0), 14);

	if (nCompareAccount)
	{
		m_pDevCmn->fnKCS_RemoveTranBit();						// 거래중지Bit제거
		fnAPD_CheckDeviceAction(DEV_MCU);
		fnAPP_CancelProc(T_MSG, "IC카드 내의 주계좌정보와",
								"공동망 계좌번호가 틀리오니",
								"은행으로 문의하시기 바랍니다.");
																// 주계좌불일치-취소
	}

/////////////////////////////////////////////////////////////////////////////
	CancelKCashSendFlg = FALSE;									// 취소출금전송Flg(초기:실행무)
	m_pDevCmn->fnAPL_SetProcCount('3');							// 진행설정
	fnAPP_SendHost();											// 송신
	fnAPP_RecvHost();											// 수신
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// 진행설정
	else m_pDevCmn->fnAPL_SetProcCount('9');					// HOST NG

	if (!m_pDevCmn->TranResult)									// HOST OK 2004.06.21
	{
		CancelKCashSendFlg = TRUE;								// 취소출금전송Flg(초기:실행무)
		m_pDevCmn->fnKCS_RemoveTranBit();						// 거래중지Bit제거
		fnAPD_CheckDeviceAction(DEV_MCU);

		TranProc = TRAN_IC;								
		TranCode = TC_KCPARAKEY;								// 파라미터갱신(Key)
		TranCode2 = 0;
		TranSend = 0;											// 발신순번 초기화
		fnAPP_KCashUpdate();									// 파라미터갱신
	}
	else
	if (!m_pDevCmn->TranResult)									// HOST OK 2004.06.21
	{
		CancelKCashSendFlg = TRUE;								// 취소출금전송Flg(초기:실행무)
		m_pDevCmn->fnKCS_RemoveTranBit();						// 거래중지Bit제거
		fnAPD_CheckDeviceAction(DEV_MCU);

		TranProc = TRAN_IC;								
		TranCode = TC_KCPARALIMIT;								// 파라미터갱신(최대저장한도)
		TranCode2 = 0;
		TranSend = 0;											// 발신순번 초기화
		fnAPP_KCashUpdate();									// 파라미터갱신
	}
	else
	if (m_pDevCmn->TranResult)									// HOST OK
	{
																// 충전요구(난수16+서명8)
		m_pDevCmn->fnKCS_LoadIEP(GetSprintf("%16.16s", GetString(PMERecvData.Data[0], 16, 16).GetBuffer(0))
							+	 GetSprintf("%8.8s",   GetString(PMERecvData.Data[1],  8,  8).GetBuffer(0)));
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())
			fnAPP_CancelProc(T_MSG, NULL, NULL, NULL);			// 취소처리

		fnAPP_PSPProc(DEV_JPR);									// 저널프린트
		fnAPP_PMDProc();										// 메세지표시
		fnAPP_PSPProc(DEV_SPR);									// 명세프린트
		fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);	// 동작완료검지&장애검지

		CancelKCashSendFlg = TRUE;								// 취소출금전송Flg(초기:실행무)

		/* ----------------------------------------------------------------
		// 충전성거래시 보고거래하지 않음
		// 신시스템 적용시 업무팀 요청사항
		// 2004.06.10
		m_pDevCmn->fnAPL_SetProcCount('3');						// 진행설정
		fnAPP_SendHost();										// 송신
		fnAPP_RecvHost();										// 수신
		if (m_pDevCmn->TranResult)								// HOST OK
			m_pDevCmn->fnAPL_SetProcCount('5');					// 진행설정
		else m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG
		------------------------------------------------------------------ */

		if (m_pDevCmn->TranResult)								// HOST OK
		{
			fnAPP_KCashReadBAmount();							// 잔액정보읽기

			m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
			m_pDevCmn->fnSCR_DisplayScreen(546, K_30_WAIT, PIN_MENU_MODE);// 충전거래확인
			m_pDevCmn->fstrSCR_GetKeyString();
		}
	}
	else
	{
		CancelKCashSendFlg = TRUE;								// 취소출금전송Flg(초기:실행무)

		m_pDevCmn->fnKCS_RemoveTranBit();						// 거래중지Bit제거
		fnAPD_CheckDeviceAction(DEV_MCU);

		fnAPP_PSPProc(DEV_JPR);        							// 저널프린트
		fnAPP_PMDProc();										// 메세지표시
		fnAPP_PSPProc(DEV_SPR);        							// 명세프린트
		fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);	// 동작완료검지&장애검지
	}
	
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// 매체방출	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// 매체방출수취대기회수

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashLoad():return");
	return T_OK;
}

// K-CASH환불
int CTranCmn::fnAPP_KCashUnload()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashUnload()");

	CString strReturn("");
	BYTE	TranAmount[4];

	fnAPP_AcceptKCashMoney();									// K-CASH금액입력

	memset(TranAmount, 0x00, sizeof(TranAmount));
	Asc2IntHighOrder(Accept.Money, TranAmount, 8, 4);			// 금액변환

																// 환불전처리(거래금액8+암호16)
	m_pDevCmn->fnKCS_PrepareUnLoadIEP(MakeUnPack(TranAmount, 4)
									+ MakeUnPack(Accept.KCashPassWord, 8));
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
								"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드사용불가

/////////////////////////////////////////////////////////////////////////////
	CancelKCashSendFlg = FALSE;									// 취소출금전송Flg(초기:실행무)	2004.02.17

	m_pDevCmn->fnAPL_SetProcCount('3');							// 진행설정
	fnAPP_SendHost();											// 송신
	fnAPP_RecvHost();											// 수신
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// 진행설정
	else m_pDevCmn->fnAPL_SetProcCount('9');					// HOST NG

	if (!m_pDevCmn->TranResult)									// HOST OK 2004.06.21
	{
		CancelKCashSendFlg = TRUE;								// 취소출금전송Flg(초기:실행무)
		m_pDevCmn->fnKCS_RemoveTranBit();						// 거래중지Bit제거
		fnAPD_CheckDeviceAction(DEV_MCU);

		TranProc = TRAN_IC;								
		TranCode = TC_KCPARAKEY;								// 파라미터갱신(Key)
		TranCode2 = 0;
		fnAPP_KCashUpdate();									// 파라미터갱신
	}
	else
	if (!m_pDevCmn->TranResult)									// HOST OK 2004.06.21
	{
		CancelKCashSendFlg = TRUE;								// 취소출금전송Flg(초기:실행무)
		m_pDevCmn->fnKCS_RemoveTranBit();						// 거래중지Bit제거
		fnAPD_CheckDeviceAction(DEV_MCU);
		
		TranProc = TRAN_IC;								
		TranCode = TC_KCPARALIMIT;								// 파라미터갱신(최대저장한도)
		TranCode2 = 0;
		fnAPP_KCashUpdate();									// 파라미터갱신
	}
	else
	if (m_pDevCmn->TranResult)									// HOST OK
	{
		CString NewAccount("");
		int		NewAccountLen = 0;

		m_pDevCmn->fnSCR_DisplayString(1, GetString(PMERecvData.Data[3], 20, 20));	
																// 입금은행
		NewAccount.Format("%14.14s", PMERecvData.Data[4]);
		NewAccount.TrimRight();
		NewAccountLen = NewAccount.GetLength();
		if (NewAccountLen == 11)
			m_pDevCmn->fnSCR_DisplayString(2, EditString(NewAccount.GetBuffer(0), "###-##-######"));
		else
		if (NewAccountLen == 12)
			m_pDevCmn->fnSCR_DisplayString(2, EditString(NewAccount.GetBuffer(0), "###-##-####-###"));
		else
		if (NewAccountLen == 13)
			m_pDevCmn->fnSCR_DisplayString(2, EditString(NewAccount.GetBuffer(0), "####-###-######"));
		else
			m_pDevCmn->fnSCR_DisplayString(2, EditString(NewAccount.GetBuffer(0), "###-######-##-###"));
																// 계좌번호
		m_pDevCmn->fnSCR_DisplayString(3, GetString(PMERecvData.Data[6], 20, 20));	
																// 성명
		m_pDevCmn->fnSCR_DisplayAmount(4, (LPCTSTR)Accept.Money, 15);
																// 거래금액
		m_pDevCmn->fnSCR_DisplayAmount(5, (LPCTSTR)&PMERecvData.Data[7], 6);
																// 수수료

		// 환불금액확인화면출력						
		m_pDevCmn->fnSCR_DisplayScreen(548, K_30_WAIT, PIN_MENU_MODE);// 환불거래확인
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CONFIRM)
			;
		else 
		{
			m_pDevCmn->fnKCS_RemoveTranBit();					// 거래중지Bit제거
			fnAPD_CheckDeviceAction(DEV_MCU);
			
			if (strReturn == S_CANCEL)
				fnAPP_CancelProc(T_CANCEL);
			else 
			if (strReturn == S_TIMEOVER)
				fnAPP_CancelProc(T_TIMEOVER);
			else
			if (strReturn == S_INPUTOVER)
				fnAPP_CancelProc(T_INPUTOVER);		
			else
				fnAPP_CancelProc(T_PROGRAM);
		}
		
																// 환불요구(난수16+서명8)
		m_pDevCmn->fnKCS_UnloadIEP(GetSprintf("%16.16s", GetString(PMERecvData.Data[0], 16, 16).GetBuffer(0))
							+	 GetSprintf("%8.8s",   GetString(PMERecvData.Data[1],  8,  8).GetBuffer(0)));
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())
		{
			m_pDevCmn->fnKCS_RemoveTranBit();					// 거래중지Bit제거
			fnAPD_CheckDeviceAction(DEV_MCU);

			fnAPP_KCashReadBAmount();							// 잔액정보읽기

			m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
			m_pDevCmn->fnSCR_DisplayScreen(547, K_30_WAIT, PIN_MENU_MODE);// 환불거래확인
			m_pDevCmn->fstrSCR_GetKeyString();
		}
		else
		{
			CancelKCashSendFlg = FALSE;							// 취소출금전송Flg(초기:실행무)
			
			m_pDevCmn->fnAPL_SetProcCount('3');					// 진행설정
			fnAPP_SendHost();									// 송신
			fnAPP_RecvHost();									// 수신
			if (m_pDevCmn->TranResult)							// HOST OK
				m_pDevCmn->fnAPL_SetProcCount('5');				// 진행설정
			else m_pDevCmn->fnAPL_SetProcCount('9');			// HOST NG

			if (m_pDevCmn->TranResult)
				CancelKCashSendFlg = TRUE;						// 취소출금전송Flg(초기:실행무)
			else
				fnAPP_CancelProc(T_MSG, NULL, NULL, NULL);		// 취소처리

			m_pDevCmn->fnKCS_RemoveTranBit();					// 거래중지Bit제거
			fnAPD_CheckDeviceAction(DEV_MCU);

			fnAPP_KCashReadBAmount();							// 잔액정보읽기

			m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
			m_pDevCmn->fnSCR_DisplayScreen(547, K_30_WAIT, PIN_MENU_MODE);// 환불거래확인
			m_pDevCmn->fstrSCR_GetKeyString();
		}														// K-CASH카드사용불가
	}
	else
	{
		m_pDevCmn->fnKCS_RemoveTranBit();						// 거래중지Bit제거
		fnAPD_CheckDeviceAction(DEV_MCU);
	}
	
	fnAPP_PSPProc(DEV_JPR);										// 저널프린트
	fnAPP_PMDProc();											// 메세지표시
	fnAPP_PSPProc(DEV_SPR);										// 명세프린트
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);		// 동작완료검지&장애검지

	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// 매체방출	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// 매체방출수취대기회수

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashUnload():return");
	return T_OK;
}

// 비밀번호변경
int CTranCmn::fnAPP_KCashChangePin()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashChangePin()");

	int nReturn = 0;
	CString strReturn("");
	CString strChangedPIN("");

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : 전자화폐시엔 폐쇄와 같이처리 2004.05.21
//		fnAPP_AcceptKCashPassword(TRAN_IC_KCASH);				// 비밀번호입력
		fnAPP_AcceptKCashChangePassword(TRAN_IC_KCASH);			// 전자화폐비밀번호변경입력
		m_pDevCmn->fnSCR_DisplayScreen(823);					// 전자화폐처리중

		m_pDevCmn->fnKCS_ChangePin(MakeUnPack(Accept.KCashPassWord, 8)	
								+  MakeUnPack(Accept.ChangeICPwdConfirm, 8));
																// 비밀번호변경(변경전암호16+변경후암호16)
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		strChangedPIN = "전자화폐 비밀번호가";
		break;

	case FIC_OPEN:												// V02-06-01-#9 20050415_LDC_A
////////////////////////////////////////////////////////////////
// 주석처리된 문장은 전자화폐에는 정상적으로 적용되나 금융IC등에는 정상적으로 처리되지 않는다.
//		m_pDevCmn->fnFIC_ChangePin(MakeUnPack(Accept.KCashPassWord, 8)	
//								+  MakeUnPack(Accept.ChangeICPwdConfirm, 8));
//		fnAPD_CheckDeviceAction(DEV_MCU);
//		nReturn = m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_CHANGEPIN);
////////////////////////////////////////////////////////////////
		m_pDevCmn->fnSCR_DisplayScreen(26);						// PIN변경할 DF선택 (전자화폐,금융IC,인증서)
//		m_pDevCmn->fnSCR_DisplayImage(3, FALSE);				// 인증서 거래막기 
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();

		if (strReturn == "전자화폐")
		{
			m_pDevCmn->fnICA_SelectApplet(TRAN_IC_KCASH);					// 
			fnAPD_CheckDeviceAction(DEV_MCU);
			if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
				fnAPP_CancelProc(T_MSG, "이 카드는 사용 할 수 없습니다");

			fnAPP_AcceptKCashPassword(TRAN_IC_KCASH);			// 비밀번호입력
			fnAPP_KCashCheckPin();								// PIN Check
			fnAPP_AcceptKCashChangePassword(TRAN_IC_KCASH);		// 전자화폐비밀번호변경입력
			strChangedPIN = "전자화폐 비밀번호가";
		}
		else
		if (strReturn == "금융IC")
		{
			m_pDevCmn->fnICA_SelectApplet(TRAN_IC_FINANCEIC);					// 
			fnAPD_CheckDeviceAction(DEV_MCU);
			if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
				fnAPP_CancelProc(T_MSG, "이 카드는 사용 할 수 없습니다");

			fnAPP_AcceptKCashPassword(TRAN_IC_FINANCEIC);		// 비밀번호입력
			fnAPP_KCashCheckPin();								// PIN Check
			fnAPP_AcceptKCashChangePassword(TRAN_IC_FINANCEIC);	// 변경비밀번호변경입력
			strChangedPIN = "금융IC 비밀번호가";
		}
		else
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);		

		nReturn = 0;
		break;

	default:
		nReturn = 9999;
		break;
	}

	if (nReturn)
		fnAPP_CancelProc(T_CANCEL, NULL, NULL, NULL, 597);		// K-CASH비밀번호변경실패	

	m_pDevCmn->fnSCR_DisplayString(1, strChangedPIN.GetBuffer(0));
	m_pDevCmn->fnSCR_DisplayString(2, "성공적으로 변경이 되었습니다.");
	m_pDevCmn->fnSCR_DisplayScreen(701);						// K-CASH비밀번호변경성공
	Delay_Msg(2 * 1000);

	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// 매체방출	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// 매체방출수취대기회수

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashUnload():return");
	return T_OK;
}

// K-CASH잔액조회
int CTranCmn::fnAPP_KCashInquiry()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashInquiry()");

	CString	strReturn("");
	CString strTemp("");
	int		i = 0, j = 1;
	CString	strTranType("");									// 거래구분
	CString	strTranSeqNo("");									// 거래일련번호
	CString	strTranAmount("");									// 거래금액
	CString	strTranBAmount("");									// 거래후잔액
	CString	strTranResult("");									// 거래결과
	CString	strCardNum("");

	m_pDevCmn->fnKCS_ReadCLog();								// 충전환불거래내역읽기
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
								"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드사용불가

	m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
	m_pDevCmn->fnSCR_DisplayScreen(514, K_30_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	fnAPD_CardEmbossScan(TRUE);
	fnAPD_CardImprint(TRUE);
	fnAPP_PSPProc(DEV_JPR);
	
	if (strReturn == "전자화폐충전")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}
	else
	if (strReturn == "주계좌로환불")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCUNLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}

	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// 매체방출	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// 매체방출수취대기회수

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashInquiry():return");
	return T_OK;
}

// 충전/환불내역
int CTranCmn::fnAPP_KCashLoadUnloadInquiry()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashLoadUnloadInquiry()");

	int		ScrNum = 0;
	CString	strReturn("");
	CString strTemp("");
	int		i = 0, j = 1;
	CString	strTranType("");									// 거래구분
	CString	strTranSeqNo("");									// 거래일련번호
	CString	strTranAmount("");									// 거래금액
	CString	strTranBAmount("");									// 거래후잔액
	CString	strTranResult("");									// 거래결과

	m_pDevCmn->fnKCS_ReadCLog();								// 충전환불거래내역읽기
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
								"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드사용불가
	ScrNum = 651;
	
	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	for (i = 0; i < m_pDevCmn->KCashInfo.LoadUnloadCnt; i++)
	{
		strTranType  = "";										// 거래구분
		strTranSeqNo  = "";										// 거래일련번호
		strTranAmount  = "";									// 거래금액
		strTranBAmount  = "";									// 거래후잔액
		strTranResult  = "";									// 거래결과

		if (m_pDevCmn->KCashInfo.LoadUnloadBuff[i][0] == 0x80)
			strTranType = "충  전";
		else
		if (m_pDevCmn->KCashInfo.LoadUnloadBuff[i][0] == 0x88)
			strTranType = "환  불";

		// 일련번호
		strTranSeqNo = Int2Asc(ByteHighOrder2Int(&m_pDevCmn->KCashInfo.LoadUnloadBuff[i][1], 2), 5);
		// 거래금액
		strTranAmount = Int2Asc(ByteHighOrder2Int(&m_pDevCmn->KCashInfo.LoadUnloadBuff[i][3], 4), 9);
		// 거래결과
		if (memcmp(&m_pDevCmn->KCashInfo.LoadUnloadBuff[i][11], ISO7816_SW_NO_ERROR, 2) == 0)
			strTranResult = "정상";
		else
		if (memcmp(&m_pDevCmn->KCashInfo.LoadUnloadBuff[i][11], ISO7816_SW_BYTES_REMAINING, 1) == 0)
			strTranResult = "정상";
		else
			strTranResult = "불능";

		strTemp.Format(" %5.5s     %6.6s    %12.12s   %4.4s",
						strTranSeqNo,
						strTranType,
						Asc2Amt(strTranAmount.GetBuffer(0), 9, 12),
						strTranResult);

		m_pDevCmn->fnSCR_DisplayString(j++, strTemp);
		if (j > 5)
			break;
	}

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	fnAPD_CardEmbossScan(TRUE);
	fnAPD_CardImprint(TRUE);
	fnAPP_PSPProc(DEV_JPR);

	if (strReturn == "전자화폐충전")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}
	else
	if (strReturn == "주계좌로환불")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCUNLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}
	
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// 매체방출	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// 매체방출수취대기회수

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashLoadUnloadInquiry():return");
	return T_OK;
}

// 구매내역조회
int CTranCmn::fnAPP_KCashBuyListInquiry()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashBuyListInquiry()");

	int		ScrNum = 0;
	CString	strReturn("");
	CString strTemp("");
	int		i = 0, j = 1;
	CString	strTranType("");									// 거래구분
	CString	strTranSeqNo("");									// 거래일련번호
	CString	strTranAmount("");									// 거래금액
	CString	strTranBAmount("");									// 거래후잔액
	CString	strTranResult("");									// 거래결과

	m_pDevCmn->fnKCS_ReadPLog1();								// 충전환불거래내역읽기
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
								"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드사용불가
	ScrNum = 652;
	
	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	for (i = 0; i < m_pDevCmn->KCashInfo.BuyListCnt; i++)
	{
		strTranType  = "";										// 거래구분
		strTranSeqNo  = "";										// 거래일련번호
		strTranAmount  = "";									// 거래금액
		strTranBAmount  = "";									// 거래후잔액
		strTranResult  = "";									// 거래결과

		if (m_pDevCmn->KCashInfo.BuyListBuff[i][0] == 0x80)
			strTranType = "충  전";
		else
		if (m_pDevCmn->KCashInfo.BuyListBuff[i][0] == 0x88)
			strTranType = "환  불";

		// 일련번호
		strTranSeqNo = Int2Asc(ByteHighOrder2Int(&m_pDevCmn->KCashInfo.BuyListBuff[i][1], 2), 5);
		// 거래금액
		strTranAmount = Int2Asc(ByteHighOrder2Int(&m_pDevCmn->KCashInfo.BuyListBuff[i][3], 4), 9);
		// 거래결과
		if (memcmp(&m_pDevCmn->KCashInfo.BuyListBuff[i][11], ISO7816_SW_NO_ERROR, 2) == 0)
			strTranResult = "정상";
		else
		if (memcmp(&m_pDevCmn->KCashInfo.BuyListBuff[i][11], ISO7816_SW_BYTES_REMAINING, 1) == 0)
			strTranResult = "정상";
		else
			strTranResult = "불능";

		strTemp.Format(" %5.5s     %6.6s    %12.12s   %4.4s",
						strTranSeqNo,
						strTranType,
						Asc2Amt(strTranAmount.GetBuffer(0), 9, 12),
						strTranResult);

		m_pDevCmn->fnSCR_DisplayString(j++, strTemp);
		if (j > 5)
			break;
	}

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	fnAPD_CardEmbossScan(TRUE);
	fnAPD_CardImprint(TRUE);
	fnAPP_PSPProc(DEV_JPR);
	
	if (strReturn == "전자화폐충전")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}
	else
	if (strReturn == "주계좌로환불")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCUNLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}
	
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// 매체방출	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// 매체방출수취대기회수

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashBuyListInquiry():return");
	return T_OK;
}

// 파라미터갱신 2004.02.27
int CTranCmn::fnAPP_KCashUpdate()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashUpdate()");
	m_pDevCmn->fnKCS_PrepareUpdateIEP();						// 파라미터변경초기화
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
								"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드읽기오류

	m_pDevCmn->fnAPL_SetProcCount('3');							// 진행설정
	fnAPP_SendHost();											// 송신
	fnAPP_RecvHost();											// 수신
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// 진행설정
	else m_pDevCmn->fnAPL_SetProcCount('9');					// HOST NG

	m_pDevCmn->fnKCS_UpdateIEP(GetSprintf("%8.8s",	 GetString(&PMERecvData.Data[3][0],  8,  8).GetBuffer(0))	// PpsamID
							+  GetSprintf("%4.4s",	 GetString(&PMERecvData.Data[4][2],  4,  4).GetBuffer(0))	// Parameter
							+  GetSprintf("%2.2s",	 GetString(&PMERecvData.Data[2][1],  2,  2).GetBuffer(0))	// Version
							+  GetSprintf("%32.32s", GetString(&PMERecvData.Data[5][0], 32, 32).GetBuffer(0))	// Parameter Value
							+  GetSprintf("%16.16s", GetString(&PMERecvData.Data[6][0], 16, 16).GetBuffer(0))	// Random Value
							+  GetSprintf("%8.8s",	 GetString(&PMERecvData.Data[7][0],  8,  8).GetBuffer(0)));	// Signature
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.",
								"[상황실: 1577 - 4655] 문의하여 주십시오.");
																// K-CASH카드읽기오류

	fnAPP_PSPProc(DEV_JPR);            							// 저널프린트
	fnAPP_PMDProc();											// 메세지표시
	fnAPP_PSPProc(DEV_SPR);            							// 명세프린트
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);		// 동작완료검지&장애검지
	
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// 매체방출	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// 매체방출수취대기회수

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashUpdate():return");
	return T_OK;
}

// 변경할 PIN번호 확인	2004.02.27
int CTranCmn::fnAPP_KCashCheckChangePin()
{
	if (IsSpace(Accept.ChangeICPwdConfirm, sizeof(Accept.ChangeICPwdConfirm)))
		return T_INPUTERROR;
//	if (!IsNum(Accept.ChangeICPwdConfirm, sizeof(Accept.ChangeICPwdConfirm)))
	if (!IsNum(Accept.ChangeICPwdConfirm, 6))					// 최소 6자리만 확인 2004.09.15
		return T_INPUTERROR;
	if (IsZero(Accept.ChangeICPwdConfirm, sizeof(Accept.ChangeICPwdConfirm)))
		return T_INPUTERROR;

	int		AscMask[8]  = {1, 2, 3, 4, 5, 6, 7, 8};
	int		DescMask[8] = {8, 7, 6, 5, 4, 3, 2, 1};
	int		InputSize  = __min(sizeof(Accept.ChangeICPwdConfirm), strlen(Accept.ChangeICPwdConfirm));

	char	EquMask[8];

	memset(EquMask, Accept.ChangeICPwdConfirm[0], sizeof(EquMask));	
	if (memcmp(EquMask, Accept.ChangeICPwdConfirm, InputSize) == 0)	// 동일번호연속확인
		return T_INPUTERROR;

	char	AscData[8];
	char	DescData[8];
	memset(AscData,  0x00, sizeof(AscData));
	memset(DescData, 0x00, sizeof(DescData));

	for (int i=0; i<InputSize; i++)
	{
		AscData[i]  = ((Accept.ChangeICPwdConfirm[i] & ~0x30) + AscMask[i] ) % 10;
		DescData[i] = ((Accept.ChangeICPwdConfirm[i] & ~0x30) + DescMask[i]) % 10;
	}

	memset(EquMask, AscData[0], sizeof(EquMask));	
	if (memcmp(EquMask, AscData, InputSize) == 0)				// 오름차순연속입력확인
		return T_INPUTERROR;
	memset(EquMask, DescData[0], sizeof(EquMask));	
	if (memcmp(EquMask, DescData, InputSize) == 0)				// 내림차순연속입력확인
		return T_INPUTERROR;

	return T_OK;
}