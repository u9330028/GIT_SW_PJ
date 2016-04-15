/////////////////////////////////////////////////////////////////////////////
// TranInput.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ScrDefine.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"
#include "..\..\H\Tran\TranCmnDefine.h"

// 동양 비밀번호 암호화 라이브러리 함수
__declspec(dllimport) long TYEncrypt(char *pIn, char *pOut);


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	기기관리함수
/////////////////////////////////////////////////////////////////////////////
// 관리비밀번호입력
int CTranCmn::fnAPP_AcceptManagerPassword()
{
	int		ScrNum = 0;
	CString	ManagerPassword("51000114");
	CString strReturn("");

	CString strVersionData("");
	strVersionData.Format("[[청호이지캐쉬] U3100K/U8100 %9.9s(%2.2s)]", m_pProfile->TRANS.ApVersion, REVISION);

	ScrNum = SCR_MANAGER_PASSWORD;								// 관리자비밀번호입력

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	while (TRUE)
	{
		m_pDevCmn->fnSCR_DisplayString(1, ManagerPassword);
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CANCEL)
			;
		else
		if (strReturn == S_TIMEOVER)
			;
		else
		if (strReturn == S_INPUTOVER)
			;
		else
		{
			if (strReturn == ManagerPassword)
			{
				m_pDevCmn->fnSCR_DisplayString(1, strVersionData.GetBuffer(0));		// 버젼출력
				m_pDevCmn->fnSCR_DisplayScreen(SCR_ATM_START);						// 준비중화면
				m_pDevCmn->fnSCR_DisplayString(3, "자동화기기를 준비하고 있습니다.");
				m_pDevCmn->fnSCR_DisplayString(4, "잠시만 기다려 주십시오.");
				return T_OK;
			}
		}
	}
	return T_OK;
}

// OP관리자 비밀번호입력
int CTranCmn::fnAPP_AcceptOpPassword()
{
	int		ScrNum = 0;
	int		nGetCnt = 0;
	int		nRet = 0;
	CString	ManagerOpPassword("000000");
	CString strReturn("");

	CString strVersionData("");
	strVersionData.Format("[[청호이지캐쉬] U3100K/U8100 %9.9s(%2.2s)]", m_pProfile->TRANS.ApVersion, REVISION);

	
	ManagerOpPassword.Format("%s", m_pProfile->TRANS.OpPassWord);
	ScrNum = 211;	

	while (TRUE)
	{
		if(nGetCnt > 2) 
		{
			nRet = T_CANCEL;
			break;
		}

		m_pDevCmn->fnSCR_DisplayString(1, ManagerOpPassword);
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CANCEL)
		{
			nRet = T_CANCEL;
			break;
		}
		else
		if (strReturn == S_TIMEOVER)
		{
			nRet = T_CANCEL;
			break;

		}
		else 
		if (strReturn == S_INPUTOVER)
		{
			nRet = T_CANCEL;
			break;

		}
		else
		{
			if (strReturn == ManagerOpPassword)
			{
				nRet = T_OK;
				break;
			}
			else 
			{
				nGetCnt++;
				continue;
			}
		}
	}
	return nRet;
}

int CTranCmn::fnAPP_AcceptEveryDayManagerPassword()
{
	int		ScrNum = 0;
	int		nGetCnt = 0;												
	CString strReturn("");

	ScrNum = 106;
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == "NH입금")					// NH입금(평일)
	{
		Accept.InQuiryCycle = '1';
		TranCode2 = MenuTblTemp[11].MenuInfo[1][6];	// 거래코드2-> TRANID_1090
	}
	else
	if (strReturn == "계좌입금")				// 계좌입금(토,일요일,공휴일)
	{
		Accept.InQuiryCycle = '2';
		TranCode2 = MenuTblTemp[11].MenuInfo[2][6];	// 거래코드2-> TRANID_1060
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
	else
		fnAPP_CancelProc(T_INPUTOVER);


	ScrNum = 206;								// convenience store's boss approve access when password identified

			
	while (TRUE)
	{

		if(nGetCnt > 2) fnAPP_CancelProc(T_INPUTOVER);

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
		else
		if (strReturn == S_CLEAR)
		{
			nGetCnt++;
			continue;
		}
		else
		{
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptPassword2(%s)", strReturn);
			if ((strReturn.GetLength() != sizeof(Accept.PassWord))	    ||	// 자료검증
				(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())) ||
				(IsZero(strReturn.GetBuffer(0), 4)))
			{
					nGetCnt++;
					continue;
			}				
		}
		break;
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptEveryDayManagerPassword NumID[%4.4s]", &m_pProfile->NETWORK.AtmSerialNum[4]);

	memcpy(Accept.MuPassWord, strReturn.GetBuffer(0), strReturn.GetLength());
	memcpy(&Accept.MuPassWord[strReturn.GetLength()], &m_pProfile->NETWORK.AtmSerialNum[4], strReturn.GetLength());

	return T_OK;
}



// 지문 인증 지원
int CTranCmn::fnAPP_FNGAccept()
{
	CString GetKeyStr("");
	int		nReturn = 0;
	CString strReturn("");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept()");
	if (!MenuFingerMode)
		return T_OK;

	if (MenuFingerMode)											// 지문인식거래
	{
		m_pDevCmn->fnSCR_DisplayScreen(870, K_60_WAIT, PIN_MENU_MODE);
																// 지문입력
		
		if(m_pDevCmn->fnFNG_GetDeviceStatus(1) == NORMAL)
		{
			m_pDevCmn->fnFNG_Initialize();							// 연결시작및초기화
			m_pDevCmn->fnFNG_Acquire();								// 지문읽기
		}
		else
			fnAPP_CancelProc(T_MSG, "지문인식 장치 연결 실패로 사용 할 수 없습니다");

		nReturn = m_pDevCmn->fnCMN_ScanDeviceAction(DEV_FNG | DEV_SCR, K_65_WAIT, EVENT_IN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept:nReturn[%d]", nReturn);
		if (nReturn == R_TIMEOUT)
		{
			m_pDevCmn->fnFNG_CancelAcquire();					// 지문읽기취소
			fnAPP_CancelProc(T_TIMEOVER);
		}
		else
		if (nReturn == DEV_FNG)
		{
			m_pDevCmn->fnSCR_DisplayScreen(871);				// 지문확인
			Delay_Msg(2000);									// 지문확인대기								
			strReturn = m_pDevCmn->fstrFNG_GetData();
			FingerData.Len = __min(sizeof(FingerData.sBuf) * 2, strReturn.GetLength()) / 2;
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept:FingerData", strReturn.GetBuffer(0), strReturn.GetLength());
			MakePack(strReturn.GetBuffer(0), FingerData.sBuf, __min(sizeof(FingerData.sBuf) * 2, strReturn.GetLength()));
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept:FingerData", &FingerData, sizeof(FingerData));
			m_pDevCmn->fnFNG_CancelAcquire();					// 지문읽기취소
		}
		else
		if (nReturn == DEV_SCR)
		{
			strReturn = m_pDevCmn->fstrSCR_GetKeyString(FALSE);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept:strReturn[%s]", strReturn);
			m_pDevCmn->fnFNG_CancelAcquire();					// 지문읽기취소
			if (strReturn == S_CANCEL)
				fnAPP_CancelProc(T_CANCEL);
			else fnAPP_CancelProc(T_TIMEOVER);
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept()::return");
	return T_OK;
}

// V02.C4.04:지문 인증 지원(상황실 Call)
int CTranCmn::fnAPP_RDCtrlPasswdAccept()
{


	CString strReturn("");
	int		ScrNum = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.SlockPassWord, 0, sizeof(Accept.SlockPassWord));	
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(872, K_90_WAIT, PIN_MENU_MODE);
																// 주민번호입력
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else 
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.SlockPassWord))		||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.SlockPassWord, strReturn.GetBuffer(0), strReturn.GetLength());
		MsgDump(TRACE_CODE_MODE, "LOCKID", __FILE__, __LINE__, "fnAPP_RDCtrlPasswdAccept(%s)", strReturn);
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RDCtrlPasswdAccept()::return");
	return T_OK;
}


int CTranCmn::fnAPP_RDCtrlSMSAccept()
{

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RDCtrlSMSAccept()");
	int		ScrNum = 0;
	CString strReturn("");
	int		nRetryCnt = 0;
		


	return T_OK;
}

// V02.C4.04:User Accept pwd Registration
int CTranCmn::fnAPP_RDCtrlPasswdAccept2()
{


	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RDCtrlPasswdAccept2()::return");
	return T_OK;
}

// V02.C4.04:User Accept Id Registration
int CTranCmn::fnAPP_RDCtrlOpenConfirm()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RDCtrlOpenConfirm()");
	int		ScrNum = 0;
	CString GetKeyStr("");

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RDCtrlOpenConfirm()::return");
	return T_OK;
}

int CTranCmn::fnAPP_RDCtrlOpenConfirm2()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RDCtrlOpenConfirm2()");
	int		ScrNum = 0;
	CString GetKeyStr("");
	int		nReturn = FALSE;


	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RDCtrlOpenConfirm2()::return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	한도금액설정함수
/////////////////////////////////////////////////////////////////////////////
// 방출한도구하기
int CTranCmn::fnAPP_CalcMoneyMaxOut()
{


	int		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
	int		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
	int		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;					// 최대방출금액(만원단위)
 	int		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수

	int		CashMaxOutCnt1	= 0;
	int		MoneyMaxOut1	= 0;
	int		CashMaxOutCnt2	= 0;
	int		MoneyMaxOut2	= 0;
	int		iMax50TCashOut = 0;
	
	int		CheckValue = 0;
	int		nCurrCheckKind = m_pDevCmn->fnAPL_GetKindOfOutCheck();
	if (nCurrCheckKind == CHECKKIND_MILLION)					// 100만원권
	{
		CheckValue = CHECK_MILLION;								// 100만원
		MAXCHECKOUTCNT = MONEY_MAXOUT / CheckValue;				// 300만원시 3매
	}
	else
	if (nCurrCheckKind == CHECKKIND_HUNDRED_THOUSAND)			// 10만원권
	{
		CheckValue = CHECK_HUNDRED_THOUSAND;					// 10만원
		MAXCHECKOUTCNT = BRM_MAXCHECKOUTCNT;					// 선언부와 동일(30매)
	}
	else
	{
		CheckValue = CHECK_UNKNOWN;
	}
	
	switch(m_pDevCmn->AtmDefine.ETCDevice20)
	{
	case JEJU: 
	case JEJU2:
	case POST:
	case SUHYUP:
		MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;
		MAXCHECKOUTCNT = 0;
		break;
	case KIUP:
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;					// 최대방출금액(만원단위)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수
		if ((CardBank == KIUBANK) && (TranCode == TC_WITHDRAW)) 
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			
//			if (!m_pDevCmn->ChangeBCFlag)
			{
				MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
				MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			}
			
			MAXCHECKOUTCNT = 0;
		}
		else
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;

//			if (!m_pDevCmn->ChangeBCFlag)
			{
				MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;
				MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;
			}
			
			MAXCHECKOUTCNT = 0;
		}
		break;
	case NONGHYUP: 
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_70;				// 최대방출금액(만원단위)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수

		if ((CardBank == INONBANKID1 || CardBank == INONBANKID2 || CardBank == INONBANKID3 ||CardBank == INONBANKID4) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCHECKOUTCNT = 0;
		}
		else
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;			
			MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;				
			MAXCHECKOUTCNT = 0;
		}
		break;
	case KEB: //#0085
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;					// 최대방출금액(만원단위)	#N0276
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수
		if ((CardBank == KOEBANK || CardBank == IHANBANKID1 || CardBank == IHANBANKID2 || CardBank == IHANBANKID3 ||CardBank == IHANBANKID4) && (TranCode == TC_WITHDRAW))	//#N0276
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCHECKOUTCNT = 0;
		}
		else
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;			
			MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;				
			MAXCHECKOUTCNT = 0;
		}
		break;
	case KWANGJU:   //#N0181
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;				// 최대방출금액(만원단위)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수
		if ((CardBank == KAWBANK) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCHECKOUTCNT = 0;
		}
		else
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;			
			MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;				
			MAXCHECKOUTCNT = 0;
		}
		break;
	case KYONGNAM:   //#N0192
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;				// 최대방출금액(만원단위)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수
		if ((CardBank == KYOBANK) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCHECKOUTCNT = 0;
		}
		else
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;			
			MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;				
			MAXCHECKOUTCNT = 0;
		}
		break;
	case HANA:			//#N0214
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;				// 최대방출금액(만원단위)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수
		if ((CardBank == KOEBANK || CardBank == IHANBANKID1 || CardBank == IHANBANKID2 || CardBank == IHANBANKID3 ||CardBank == IHANBANKID4) && (TranCode == TC_WITHDRAW))	//#N0276
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCHECKOUTCNT = 0;
		}
		else
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;			
			MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;				
			MAXCHECKOUTCNT = 0;
		}
		break;
	case SHINHAN:			//#N0215
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_70;				// 최대방출금액(만원단위)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수
		if ((CardBank == ISHIBANKID || CardBank == ISHIBANKID1 || CardBank == ICHOBANKID) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCHECKOUTCNT = 0;
		}
		else
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;			
			MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;				
			MAXCHECKOUTCNT = 0;
		}
		break;
	case SC:			//#N0219
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_70;				// 최대방출금액(만원단위)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수
		if ((CardBank == IKOFBANKID) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCHECKOUTCNT = 0;
		}
		else
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;			
			MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;				
			MAXCHECKOUTCNT = 0;
		}
		break;
	case KB: //#N0226
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_70;				// 최대방출금액(만원단위)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수
		if ((CardBank == IKUMBANKID1 || CardBank == IKUMBANKID2 || CardBank == IKUMBANKID3 ||CardBank == IKUMBANKID4 ||CardBank == IKUMBANKID5) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			
			MAXCHECKOUTCNT = 0;
		}
		else
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;
			MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;
			MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;
			MAXCHECKOUTCNT = 0;
		}
		break;
	case CITI:			//#N0238
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_70;				// 최대방출금액(만원단위)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수
		if ((CardBank == IGOOBANKID1 || CardBank == IGOOBANKID2 || CardBank == ICITBANKID) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// 최대방출매수
			MAXCHECKOUTCNT = 0;
		}
		else
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;			
			MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;				
			MAXCHECKOUTCNT = 0;
		}
		break;
	case NICE:			//#N0219
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// 현금최대방출매수
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// 수표최대방출매수															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_30;				// 최대방출금액(만원단위)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// 최대방출매수
		
		MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;                 
		MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;			
		MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;				
		MAXCHECKOUTCNT = 0;
		break;
	default:
		MONEY_MAXOUT =  m_pProfile->TRANS.TaMaxDispAmt;                 
		MONEY_MAXOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;			
		MAXCASHOUTCNT = m_pProfile->TRANS.TaMaxDispAmt;	
		MAXCHECKOUTCNT = 0;
		break;
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxOutATMLarge - 0:MAXCASHOUTCNT [%d], MONEY_MAXOUT[%d], MONEY_MAXOUTCNT[%d]", MAXCASHOUTCNT, MONEY_MAXOUT, MONEY_MAXOUTCNT);
	WithAvail &= ~TRAN_WITH_CHECK;

	if ((WithAvail == TRAN_WITH_50TCASH)	||
		(WithAvail == TRAN_WITH_50TCASHCHECK))
	{

		MAXCASHOUTCNT = MONEY_MAXOUTCNT % CASH_FIFTY_THOUSAND;
		if (MAXCASHOUTCNT)
			MAXCASHOUTCNT = (MONEY_MAXOUTCNT / CASH_FIFTY_THOUSAND) + 1;
		else
			MAXCASHOUTCNT = (MONEY_MAXOUTCNT / CASH_FIFTY_THOUSAND);


		iMax50TCashOut = MONEY_MAXOUTCNT % CASH_FIFTY_THOUSAND;
		if (iMax50TCashOut)
			iMax50TCashOut = (MONEY_MAXOUTCNT / CASH_FIFTY_THOUSAND) + 1;
		else
			iMax50TCashOut = (MONEY_MAXOUTCNT / CASH_FIFTY_THOUSAND);
		
		if ((MAXCASHOUTCNT >= 1)	&&							// 1만 ~ 기기한도
			(MAXCASHOUTCNT <= iMax50TCashOut))
			;
		else MAXCASHOUTCNT = 0;									// 금액이상시 0만원
	}
	else
	{
		if ((MAXCASHOUTCNT >= 1)	&&							// 1만 ~ 기기한도
			(MAXCASHOUTCNT <= BRM_MAXCASHOUTCNT))
			;
		else MAXCASHOUTCNT = 0;									// 금액이상시 0만원
	}


	if (nCurrCheckKind == CHECKKIND_MILLION)					// 메카보정
	{
		if ((MAXCHECKOUTCNT >= 1)	&&							// 1매 ~ 기기한도
			(MAXCHECKOUTCNT <= BRM_MAXCHECKOUTCNT_100CHECK));
		else MAXCHECKOUTCNT = 0;								// 매수이상시 0매
	}
	else
	{
		if ((MAXCHECKOUTCNT >= 1)	&&							// 1매 ~ 기기한도
			(MAXCHECKOUTCNT <= BRM_MAXCHECKOUTCNT));
		else MAXCHECKOUTCNT = 0;								// 매수이상시 0매
	}
	// 100만원 출금시 한도가 상향될 경우 BRM_MONEY_MAXOUT 수정필요
	if (nCurrCheckKind == CHECKKIND_MILLION)
	{															// V02-08-01-R1 백만원권지원
		if ((MONEY_MAXOUT >= 1)	&&								// 1만 ~ 기기한도
			(MONEY_MAXOUT <= BRM_MONEY_MAXOUT));
		else MONEY_MAXOUT = 0;									// 금액이상시 0만원
	}
	else
	{
		if ((MONEY_MAXOUT >= 1)	&&								// 1만 ~ 기기한도
			(MONEY_MAXOUT <= BRM_MONEY_MAXOUT));
		else MONEY_MAXOUT = 0;									// 금액이상시 0만원
	}

//	MONEY_MAXOUT = MAXCASHOUTCNT;								// 현금액과동일
//	MONEY_MAXOUTCNT = MAXCASHOUTCNT;							// 현금매수와동일


	MoneyMaxOut = 0;											// 방출가능한도금액
	MoneyMaxOutCnt = 0;											// 방출가능매수
	CashMaxOutCnt = 0;											// 현금방출가능매수
	CheckMaxOutCnt = 0;											// 수표방출가능매수
	CheckLowMaxOutCnt = 0;										// 수표방출최소매수
	CheckHighMaxOutCnt = 0;										// 수표방출최대매수

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxOutATMLarge - 1:MAXCASHOUTCNT [%d], MONEY_MAXOUT[%d], MONEY_MAXOUTCNT[%d]", MAXCASHOUTCNT, MONEY_MAXOUT, MONEY_MAXOUTCNT);

	switch (WithAvail)
	{
		case TRAN_WITH_CASH:
		case TRAN_WITH_2CASH:									
			CashMaxOutCnt = m_pDevCmn->fnAPL_GetNumberOfCash(); //1만 + 5만    
			if (CashMaxOutCnt > MAXCASHOUTCNT)
				CashMaxOutCnt = MAXCASHOUTCNT;

			break;
			
		case TRAN_WITH_50TCASH:
			CashMaxOutCnt = m_pDevCmn->fnAPL_GetNumberOf50000Cash();// 현금매수구하기
			if (CashMaxOutCnt > MAXCASHOUTCNT)
				CashMaxOutCnt = MAXCASHOUTCNT;

			break;

		case TRAN_WITH_CHECK:
			if (m_pDevCmn->fnAPL_GetNumberOfOutCheck() > BRM_FIXCHECKCNT)
				CheckMaxOutCnt = m_pDevCmn->fnAPL_GetNumberOfOutCheck() - BRM_FIXCHECKCNT;
			else CheckMaxOutCnt = 0;

			if (CheckMaxOutCnt > MAXCHECKOUTCNT)
				CheckMaxOutCnt = MAXCHECKOUTCNT;


			break;

		case TRAN_WITH_50TCASHCHECK:
			CashMaxOutCnt = m_pDevCmn->fnAPL_GetNumberOf50000Cash();// 현금매수구하기
			if (CashMaxOutCnt > MAXCASHOUTCNT)
				CashMaxOutCnt = MAXCASHOUTCNT;			
			if (m_pDevCmn->fnAPL_GetNumberOfOutCheck() > BRM_FIXCHECKCNT)	
				CheckMaxOutCnt = m_pDevCmn->fnAPL_GetNumberOfOutCheck() - BRM_FIXCHECKCNT;
			else CheckMaxOutCnt = 0;

			if (CheckMaxOutCnt > MAXCHECKOUTCNT)
				CheckMaxOutCnt = MAXCHECKOUTCNT;
			if (CheckMaxOutCnt > MONEY_MAXOUTCNT)
				CheckMaxOutCnt = MONEY_MAXOUTCNT;

			MoneyMaxOut = CashMaxOutCnt;
			break;

		case TRAN_WITH_CASHCHECK:		
		case TRAN_WITH_BOTH:
			CashMaxOutCnt = m_pDevCmn->fnAPL_GetNumberOfCash();   
			if (CashMaxOutCnt > MAXCASHOUTCNT)
				CashMaxOutCnt = MAXCASHOUTCNT;

			if (m_pDevCmn->fnAPL_GetNumberOfOutCheck() > BRM_FIXCHECKCNT)
				CheckMaxOutCnt = m_pDevCmn->fnAPL_GetNumberOfOutCheck() - BRM_FIXCHECKCNT;
			else CheckMaxOutCnt = 0;

			if (CheckMaxOutCnt > MAXCHECKOUTCNT)
				CheckMaxOutCnt = MAXCHECKOUTCNT;


			break;

		case TRAN_WITH_NOT:
		default: 
			CashMaxOutCnt = 0;
			CheckMaxOutCnt = 0;
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxOutATMLarge - 2:CashMaxOutCnt [%d], MONEY_MAXOUT[%d], MONEY_MAXOUTCNT[%d]", CashMaxOutCnt, MONEY_MAXOUT, MONEY_MAXOUTCNT);

	while (CheckMaxOutCnt)										// 수표 Low, High Max 보정
	{
		if ((WithAvail == TRAN_WITH_50TCASH)	||
			(WithAvail == TRAN_WITH_50TCASHCHECK))
			MoneyMaxOut = CashMaxOutCnt*CASH_FIFTY_THOUSAND + (CheckMaxOutCnt * CheckValue);
		else
			MoneyMaxOut = CashMaxOutCnt + (CheckMaxOutCnt * CheckValue);		
																
		if (MoneyMaxOut > MONEY_MAXOUT)
			MoneyMaxOut = MONEY_MAXOUT;

		fnAPP_CalcCheckLowMaxOutCnt(MoneyMaxOut);				// 최소수표매수구하기
		fnAPP_CalcCheckHighMaxOutCnt(MoneyMaxOut);				// 최대수표매수구하기

		if ((WithAvail == TRAN_WITH_50TCASH)	||
			(WithAvail == TRAN_WITH_2CASH)		||
			(WithAvail == TRAN_WITH_50TCASHCHECK))
		{
			CashMaxOutCnt1 = (MoneyMaxOut - (CheckLowMaxOutCnt * CheckValue)) / CASH_FIFTY_THOUSAND;	// V05-04-01-#09:100만원 수표지원
			MoneyMaxOut1 = (CheckLowMaxOutCnt * CheckValue) + (CashMaxOutCnt1*CASH_FIFTY_THOUSAND);		// V05-04-01-#09:100만원 수표지원
			CashMaxOutCnt2 = (MoneyMaxOut - (CheckHighMaxOutCnt * CheckValue)) / CASH_FIFTY_THOUSAND;	// V05-04-01-#09:100만원 수표지원
			MoneyMaxOut2 = (CheckHighMaxOutCnt * CheckValue) + (CashMaxOutCnt2*CASH_FIFTY_THOUSAND);	// V05-04-01-#09:100만원 수표지원
		}
		else
		{
			CashMaxOutCnt1 = (MoneyMaxOut - (CheckLowMaxOutCnt * CheckValue));							// V05-04-01-#09:100만원 수표지원
			MoneyMaxOut1 = (CheckLowMaxOutCnt * CheckValue) + CashMaxOutCnt1;							// V05-04-01-#09:100만원 수표지원
			CashMaxOutCnt2 = (MoneyMaxOut - (CheckHighMaxOutCnt * CheckValue));							// V05-04-01-#09:100만원 수표지원
			MoneyMaxOut2 = (CheckHighMaxOutCnt * CheckValue) + CashMaxOutCnt2;							// V05-04-01-#09:100만원 수표지원
		}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxOutATMLarge:CashMaxOutCnt[%d], "		\
														"CheckMaxOutCnt[%d], "		\
														"MoneyMaxOut[%d], "			\
														"CheckLowMaxOutCnt[%d], "	\
														"CheckHighMaxOutCnt[%d], "	\
														"CashMaxOutCnt1[%d], "		\
														"MoneyMaxOut1[%d], "		\
														"CashMaxOutCnt2[%d], "		\
														"MoneyMaxOut2[%d]",
														CashMaxOutCnt, 
														CheckMaxOutCnt, 
														MoneyMaxOut, 		
														CheckLowMaxOutCnt, 
														CheckHighMaxOutCnt,		
														CashMaxOutCnt1, 
														MoneyMaxOut1, 
														CashMaxOutCnt2, 
														MoneyMaxOut2);

		if ((WithAvail == TRAN_WITH_CASH) ||
			(WithAvail == TRAN_WITH_2CASH))
		{
			if ((CashMaxOutCnt1 >= 0)					&&		// 방출현금매수체크
				(CashMaxOutCnt1 <= CashMaxOutCnt)		&&
				(CashMaxOutCnt2 >= 0)					&&
				(CashMaxOutCnt2 <= CashMaxOutCnt)		&&
				(MoneyMaxOut1 == MoneyMaxOut)			&&		// 방출금액동일체크
				(MoneyMaxOut2 == MoneyMaxOut))
				break;
		}

		if (WithAvail == TRAN_WITH_CHECK)
		{
			if ((CheckLowMaxOutCnt >= 0)				&&		// 방출수표매수체크
				(CheckLowMaxOutCnt <= CheckMaxOutCnt)	&&
				(CheckHighMaxOutCnt >= 0)				&&
				(CheckHighMaxOutCnt <= CheckMaxOutCnt)	&&
				(MoneyMaxOut1 == MoneyMaxOut)			&&		// 방출금액동일체크
				(MoneyMaxOut2 == MoneyMaxOut))
				break;
		}

		if ((WithAvail == TRAN_WITH_BOTH)		||
			(WithAvail == TRAN_WITH_CASHCHECK)	||
			(WithAvail == TRAN_WITH_50TCASHCHECK))
		{
			if ((CashMaxOutCnt1 >= 0)					&&		// 방출현금매수체크
				(CashMaxOutCnt1 <= CashMaxOutCnt)		&&
				(CashMaxOutCnt2 >= 0)					&&
				(CashMaxOutCnt2 <= CashMaxOutCnt)		&&
				(CheckLowMaxOutCnt >= 0)				&&		// 방출수표매수체크
				(CheckLowMaxOutCnt <= CheckMaxOutCnt)	&&
				(CheckHighMaxOutCnt >= 0)				&&
				(CheckHighMaxOutCnt <= CheckMaxOutCnt)	&&
				(MoneyMaxOut1 == MoneyMaxOut)			&&		// 방출금액동일체크
				(MoneyMaxOut2 == MoneyMaxOut))
				break;
		}
		
		CheckMaxOutCnt--;
	}

	if ((WithAvail == TRAN_WITH_50TCASH)	||
		(WithAvail == TRAN_WITH_50TCASHCHECK))
		MoneyMaxOut = CashMaxOutCnt*CASH_FIFTY_THOUSAND + (CheckMaxOutCnt * CheckValue);		// V05-04-01-#09:100만원 수표지원
	else
		MoneyMaxOut = CashMaxOutCnt + (CheckMaxOutCnt * CheckValue);							// V05-04-01-#09:100만원 수표지원

	if (MoneyMaxOut > MONEY_MAXOUT)
		MoneyMaxOut = MONEY_MAXOUT;

	MoneyMaxOutCnt = CashMaxOutCnt + CheckMaxOutCnt;			// 방출가능매수
	if (MoneyMaxOutCnt > MONEY_MAXOUTCNT)
		MoneyMaxOutCnt = MONEY_MAXOUTCNT;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxOutATMLarge:CashMaxOutCnt[%d], "		\
														"CheckMaxOutCnt[%d], "		\
														"MoneyMaxOut[%d], "			\
														"CheckLowMaxOutCnt[%d], "	\
														"CheckHighMaxOutCnt[%d], "	\
														"CashMaxOutCnt1[%d], "		\
														"MoneyMaxOut1[%d], "		\
														"CashMaxOutCnt2[%d], "		\
														"MoneyMaxOut2[%d]",
														CashMaxOutCnt, 
														CheckMaxOutCnt, 
														MoneyMaxOut, 		
														CheckLowMaxOutCnt, 
														CheckHighMaxOutCnt,		
														CashMaxOutCnt1, 
														MoneyMaxOut1, 
														CashMaxOutCnt2, 
														MoneyMaxOut2);

	return MoneyMaxOut;
}

// 수표최소매수구하기
int CTranCmn::fnAPP_CalcCheckLowMaxOutCnt(int MoneyOut)
{
	int		MAXCASHOUTCNT = BRM_MONEY_MAXOUTCNT;				// 현금최대방출매수
	int		CheckValue = 0;										// 수표기본금액처리
	int		TempMan = 0;

	switch(m_pDevCmn->AtmDefine.ETCDevice20)
	{
	case JEJU: 
	case JEJU2:
	case SC:
	case KWANGJU:
	case CITI:
	case HANA:
	case POST:
	case SUHYUP:
		MAXCASHOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
		break;
	case KEB: //#0085
		if (CardBank == KEB)
		{
			MAXCASHOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MAXCASHOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	case KIUP:
		if (CardBank == KIUBANK)
		{
			MAXCASHOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MAXCASHOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	default:
		MAXCASHOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
		break;
	}
	

																// 메카보정
	if ((MAXCASHOUTCNT >= 1)	&&								// 1만 ~ 기기한도
		(MAXCASHOUTCNT <= BRM_MAXCASHOUTCNT));
	else MAXCASHOUTCNT = 0;										// 금액이상시 0만원

	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
	{
		CheckValue = CHECK_MILLION;
	}
	else
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
	{
		CheckValue = CHECK_HUNDRED_THOUSAND;
	}
	else
	{
		CheckValue = CHECK_UNKNOWN;
	}

	CheckLowMaxOutCnt = 0;										// 수표방출최소매수구하기
	
	switch (WithAvail)
	{
		case TRAN_WITH_CASH:
		case TRAN_WITH_50TCASH:
		case TRAN_WITH_2CASH:
			CheckLowMaxOutCnt = 0;
			break;
		case TRAN_WITH_CHECK:										
			if (CheckMaxOutCnt)									
				CheckLowMaxOutCnt = 1;
			break;
		case TRAN_WITH_50TCASHCHECK:
		case TRAN_WITH_CASHCHECK:
		case TRAN_WITH_BOTH:
			if (MoneyOut < (MAXCASHOUTCNT + 1))					// 하나사양(현금,수표통합매수)
				CheckLowMaxOutCnt = 1;
			else CheckLowMaxOutCnt = ((MoneyOut - (MAXCASHOUTCNT + 1)) / 9) + 1;
			break;
		case TRAN_WITH_NOT:
		default: 
			CheckLowMaxOutCnt = 0;
			break;
	}
	if (CheckLowMaxOutCnt > CheckMaxOutCnt)
		CheckLowMaxOutCnt = CheckMaxOutCnt;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcCheckLowMaxOutCnt:CheckLowMaxOutCnt[%d]", CheckLowMaxOutCnt);
	return CheckLowMaxOutCnt;
}
// 수표최대매수구하기
int CTranCmn::fnAPP_CalcCheckHighMaxOutCnt(int MoneyOut)
{
	int		CheckValue = 0;										// 수표기본금액처리

	CheckHighMaxOutCnt = 0;										// 수표방출최대매수구하기
	
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
	{
		CheckValue = CHECK_MILLION;
	}
	else
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
	{
		CheckValue = CHECK_HUNDRED_THOUSAND;
	}
	else
	{
		CheckValue = CHECK_UNKNOWN;
	}

	switch (WithAvail)
	{
		case TRAN_WITH_CASH:
		case TRAN_WITH_50TCASH:
		case TRAN_WITH_2CASH:
			CheckHighMaxOutCnt = 0;
			break;
		case TRAN_WITH_CHECK:
		case TRAN_WITH_50TCASHCHECK:
		case TRAN_WITH_CASHCHECK:
		case TRAN_WITH_BOTH:
			if (CheckValue == CHECK_UNKNOWN)					// CodeSonar 수정
				fnAPP_CancelProc(T_PROGRAM);
			else
				CheckHighMaxOutCnt = MoneyOut / CheckValue;
			break;
		case TRAN_WITH_NOT:
		default: 
			CheckHighMaxOutCnt = 0;
			break;
	}
	if (CheckHighMaxOutCnt > CheckMaxOutCnt)
		CheckHighMaxOutCnt = CheckMaxOutCnt;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcCheckHighMaxOutCnt:CheckHighMaxOutCnt[%d]", CheckHighMaxOutCnt);
	return CheckHighMaxOutCnt;
}

// 이체한도구하기
int CTranCmn::fnAPP_CalcMoneyMaxTrans()
{
	if(TranCode == TC_SVCTRAN1)
		TMoneyMaxOut = Asc2Int(OpenInfo.SvcTransMoney, 9);				// 이체가능한도금액
	else
		TMoneyMaxOut = Asc2Int(OpenInfo.TransMoney, 9);				// 이체가능한도금액

	if ((TMoneyMaxOut >= 1000000)	&&							// 100만 ~ 1억미만
		(TMoneyMaxOut < 100000000));
	else
		TMoneyMaxOut = 1000000;									// 금액이상시 100만원

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxTrans:TMoneyMaxOut[%d]", TMoneyMaxOut);
	return TMoneyMaxOut;
}

// 입금한도구하기
int CTranCmn::fnAPP_CalcMoneyMaxIn()
{
	CashMaxIn = fnAPP_CalcCashMaxIn();							// 현금입금한도구하기
	CheckMaxInCnt = fnAPP_CalcCheckMaxIn();						// 수표입금한도구하기

	if ((CashMaxIn >= 10)	&&									// 10만 ~ 기기한도
		(CashMaxIn <= BRM_MAXCASHINCNT));
	else
		CashMaxIn = 10;											// 금액이상시 10만원

	if ((CheckMaxInCnt >= 1)	&&								// 1매 ~ 기기한도
		(CheckMaxInCnt <= UCM_MAXCHECKINCNT));
	else
		CheckMaxInCnt = 0;										// 매수이상시 0매

	MoneyMaxIn = CashMaxIn + (CheckMaxInCnt * 100);				// 입금가능한도금액
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxIn:CashMaxIn[%d], CheckMaxInCnt[%d], MoneyMaxIn[%d]", CashMaxIn, CheckMaxInCnt, MoneyMaxIn);
	return MoneyMaxIn;
}

// 현금입금한도구하기
int CTranCmn::fnAPP_CalcCashMaxIn()
{
	int	DEP_MAXIN;

//	DEP_MAXIN = Asc2Int(OpenInfo.DepCashCount, 3);
	//#0085
	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
		DEP_MAXIN = BRM_MAXCASHINCNT;					
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
		DEP_MAXIN = BRM_MAXCASHINCNT;					
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KEB)				
	{
		if((CardBank == IHANBANKID1 || CardBank == IHANBANKID2 || CardBank == IHANBANKID3 ||CardBank == IHANBANKID4))			//#N0246
			DEP_MAXIN = BRM_MAXCASHINCNT_150;
		else
		if(CardBank == KOEBANK)
			DEP_MAXIN = BRM_MAXCASHINCNT_150;	// #N0276
		else
			DEP_MAXIN = BRM_MAXCASHINCNT;
	}
	else 
	if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU)				//#N0181		
	{
		if (CardTran & MENU_T3)									//#N0273
			DEP_MAXIN = BRM_MAXCASHINCNT;
		else
			DEP_MAXIN = BRM_MAXCASHINCNT_150;
	}
	else 
	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)				//#N0192		
	{
		if (CardTran & MENU_T3)									//#N0273
			DEP_MAXIN = BRM_MAXCASHINCNT;
		else
			DEP_MAXIN = BRM_MAXCASHINCNT_150;
	}
	else 
	if(m_pProfile->DEVICE.ETCDevice20 == HANA)					//#N0214		
	{
		if((CardBank == IHANBANKID1 || CardBank == IHANBANKID2 || CardBank == IHANBANKID3 ||CardBank == IHANBANKID4))			//#N0246
			DEP_MAXIN = BRM_MAXCASHINCNT_150;
		else
		if(CardBank == KOEBANK)
			DEP_MAXIN = BRM_MAXCASHINCNT_150;	// #N0276
		else
			DEP_MAXIN = BRM_MAXCASHINCNT;
	}
	else 
	if(m_pProfile->DEVICE.ETCDevice20 == SHINHAN)				//#N0215		
		DEP_MAXIN = BRM_MAXCASHINCNT;
	else 
	if(m_pProfile->DEVICE.ETCDevice20 == SC)					//#N0219		
		DEP_MAXIN = BRM_MAXCASHINCNT;
	else 
	if(m_pProfile->DEVICE.ETCDevice20 == KB)					//#N0226		
	{
		if (CardTran & MENU_T3)									//#N0273
			DEP_MAXIN = BRM_MAXCASHINCNT;
		else
			DEP_MAXIN = BRM_MAXCASHINCNT_150;
	}
	else 
	if(m_pProfile->DEVICE.ETCDevice20 == CITI)					//#N0238		
		DEP_MAXIN = BRM_MAXCASHINCNT;
	else 
	if(m_pProfile->DEVICE.ETCDevice20 == NICE)					//#N0219		
		DEP_MAXIN = BRM_MAXCASHINCNT;
	else
		DEP_MAXIN = BRM_MAXCASHINCNT;
	
	CashMaxIn = DEP_MAXIN;
	
	if ((CashMaxIn >= 1)	&&									// 1만 ~ 기기한도
		(CashMaxIn <= DEP_MAXIN));
	else
		CashMaxIn = 1;		

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcCashMaxIn:CashMaxIn[%d]", CashMaxIn);
	return CashMaxIn;
}

// 호출전 fnAPP_CalcCashMaxIn()을 호출 후에 사용해야한다.
int CTranCmn::fnAPP_CalcCashMaxInAmount()
{
	int		nMaxAmount = 0;
	int		nTemp = 1;

	// 오만원권만 가능시는 최대금액 기준으로함.(100만원/20매)
	// 일만원권만 가능시는 최대금액 기준으로함.(100만원/100매)
	if (((m_pProfile->DEVICE.CashHandler == OKI_SET)        ||
		 (m_pProfile->DEVICE.CashHandler == HBRM_SET))      &&	// OKI(T2ATM)
		(m_pDevCmn->ChangeBCFlag)							&&	// 5만원권 개체여부
		((m_pProfile->BRM.DepositCashKind & 0x08) == 0x08)	&&	// 5만원권 가능여부
		(m_pDevCmn->Withdraw50TFlag)						)	// 고액권카세트유무
		nTemp = CASH_FIFTY_THOUSAND;

	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
	{
		if (CardTran & MENU_T3)									//#N0273  
			nMaxAmount = BRM_MAXMUCASHINCNT;
		else
			nMaxAmount = BRM_MAXCASHINAMT;						// 100만원
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
	{
		if (CardTran & MENU_T3)		//#N0233  기업은행 타행 입금 금액
			nMaxAmount = BRM_MAXMUCASHINCNT;
		else
			nMaxAmount = BRM_MAXCASHINAMT;
		/*
		if (m_pProfile->DEVICE.MachineType == U8100)
			nMaxAmount = BRM_MAXCASHINAMT;						// 100만원 -> 500만원 //#0126
		else	
		if (m_pProfile->DEVICE.MachineType == U3100K)
			nMaxAmount = BRM_MAXCASHINAMT;						// 500만원	#N0233
		else
			nMaxAmount = BRM_MAXMUCASHINCNT;					// 100만원
			*/
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KEB)					// #0085
	{
		if((CardBank == IHANBANKID1 || CardBank == IHANBANKID2 || CardBank == IHANBANKID3 ||CardBank == IHANBANKID4))			//#N0246
			nMaxAmount = BRM_MAXCASHINAMT_750;
		else
		if(CardBank == KOEBANK)
			nMaxAmount = BRM_MAXCASHINAMT_750; 	// #N0276
		else
			nMaxAmount = BRM_MAXMUCASHINCNT;
	}					    // 500만원
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU)				// #N0181
	{
		if (CardTran & MENU_T3)									//#N0273
			nMaxAmount = BRM_MAXMUCASHINCNT;
		else
			nMaxAmount = BRM_MAXCASHINAMT_750;						// 750만원  
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)				// #N0192
	{
		if (CardTran & MENU_T3)									//#N0273
			nMaxAmount = BRM_MAXMUCASHINCNT;
		else
			nMaxAmount = BRM_MAXCASHINAMT_750;
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == HANA)					// #N0214 하나은행 입금 750만원 무매체 100만원
	{
		if((CardBank == IHANBANKID1 || CardBank == IHANBANKID2 || CardBank == IHANBANKID3 ||CardBank == IHANBANKID4))			//#N0246
		{
			if(MenuAvail & MENU_MU)
				nMaxAmount = BRM_MAXMUCASHINCNT;
			else
				nMaxAmount = BRM_MAXCASHINAMT_750;
		}
		else
		if(CardBank == KOEBANK)
			nMaxAmount = BRM_MAXCASHINAMT_750; 	// #N0276
		else
			nMaxAmount = BRM_MAXMUCASHINCNT;
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == SHINHAN)				// #N0215
	{
		if (CardTran & MENU_T3)									//#N0273
			nMaxAmount = BRM_MAXMUCASHINCNT;
		else
			nMaxAmount = BRM_MAXMUCASHINCNT;
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == SC)					// #N0219
	{
		if (CardTran & MENU_T3)									//#N0273
			nMaxAmount = BRM_MAXMUCASHINCNT;
		else
			nMaxAmount = BRM_MAXCASHINAMT;
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KB)					// #N0226
	{
		if (CardTran & MENU_T3)									//#N0273
			nMaxAmount = BRM_MAXMUCASHINCNT;
		else
			nMaxAmount = BRM_MAXCASHINAMT_150;						// 150만원  
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == CITI)					// #N0238
	{
		nMaxAmount = BRM_MAXMUCASHINCNT;
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == NICE)					// #N0219
	{
		if (CardTran & MENU_T3)									//#N0273
			nMaxAmount = BRM_MAXMUCASHINCNT;
		else
			nMaxAmount = BRM_MAXMUCASHINCNT;
	}
	else
		nMaxAmount = BRM_MAXMUCASHINCNT;						// 100만원

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcCashMaxIn:fnAPP_CalcCashMaxInAmount[%d]", nMaxAmount);
	return nMaxAmount;
}


// 수표입금한도구하기
int CTranCmn::fnAPP_CalcCheckMaxIn()
{
	CheckMaxInCnt = Asc2Int(OpenInfo.DepCheckCount, 3);			// 수표입금가능매수

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcCheckMaxIn:CheckMaxInCnt[%d]", CheckMaxInCnt);
	return CheckMaxInCnt;
}

/////////////////////////////////////////////////////////////////////////////
//	입력함수
/////////////////////////////////////////////////////////////////////////////
// 출금입력
int CTranCmn::fnAPP_AcceptWith()
{


	fnAPP_AcceptPbConfirmNum();
	
	if (MenuIrdaMode)
		fnAPP_MenuIrdaAccept();

	//#0038
	if (MenuAvail & MENU_MU)
	{
		if(TranCode == TC_MOBILEWITHDRAW) // #N0182
		{
			;
		}
		else
		{
			if(m_pProfile->DEVICE.ETCDevice20 != HANA)				//#N0214
				fnAPP_AcceptJuminNum();									// 주민번호입력
			fnAPP_AcceptMuAccountNum();                             // 계좌번호 
			
			if(TranCode == TC_WITHDRAW)
				fnAPP_AcceptMuConfirmNum();							// 무매체거래승인번호입력
		}
	}

	fnAPP_AcceptMoney();		    							// 출금금액입력
	if (!MenuIrdaMode)
	{
		if(TranCode == TC_FOREGIN)
			fnAPP_AcceptDesPassword();
		else
		if (TranCode == TC_MOBILEWITHDRAW)
		{
			fnAPP_AcceptMuConfirmNum();							// 휴대폰 승인번호
			fnAPP_AcceptMPINConfirmNum();
		}
		else
			fnAPP_AcceptPassword();								// 비밀번호입력
	}


	fnAPD_CheckDeviceAction(DEV_CSH);							// 속도개선 (금액입력후로 이동)
	m_pDevCmn->fnCSH_ReadyDispense();							// 출금준비 

	return T_OK;
}

// 조회조건설정
int CTranCmn::fnAPP_AcceptInq()
{
	if((m_pProfile->DEVICE.ETCDevice20 == KB) && (PbTran & MENU_PB))
		memset(Accept.PbConfirmNum, '0', sizeof(Accept.PbConfirmNum));
	
	if(TranCode == TC_FOREGIN)
		fnAPP_AcceptDesPassword();
	else
		fnAPP_AcceptPassword();								

	if (TranCode == TC_INQUIRYA || TranCode == TC_INQSTOCK)		// 예금기록조회
	{
		fnAPP_AcceptInqDate();
	}
	else
	{
		memset(Accept.InquiryDate, ' ', sizeof(Accept.InquiryDate));
		memset(Accept.InquiryDateEnd, ' ', sizeof(Accept.InquiryDateEnd));
	}

	return T_OK;
}

// 조회일입력
int CTranCmn::fnAPP_AcceptInqDate()
{
	CString strReturn("");

	memset(Accept.InquiryDate, ' ', sizeof(Accept.InquiryDate));
	memset(Accept.InquiryDateEnd, ' ', sizeof(Accept.InquiryDateEnd));

	if(TranCode == TC_INQSTOCK)					// 청약내역조회 기간설정
	{
		m_pDevCmn->fnSCR_DisplayScreen(961, K_30_WAIT, PIN_MENU_MODE);
		m_pDevCmn->fnSCR_DisplayImage(7, FALSE);
	}
	else										// 거래내역조회 기간설정
		m_pDevCmn->fnSCR_DisplayScreen(423, K_30_WAIT, PIN_MENU_MODE);

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == "당일")					// 당일
		Accept.InQuiryCycle = '1';
	else
	if (strReturn == "최근1주일")				// 최근1주일
		Accept.InQuiryCycle = '2';
	else
	if (strReturn == "최근15일")				// 최근15일
		Accept.InQuiryCycle = '3';
	else
	if (strReturn == "최근30일")				// 최근30일
		Accept.InQuiryCycle = '4';
	else
	if (strReturn == "최근45일")				// 최근45일
		Accept.InQuiryCycle = '5';
	else
	if (strReturn == "최근60일")				// 최근60일
		Accept.InQuiryCycle = '6';
	else
	if (strReturn == "기타")					// 기타
	{
		Accept.InQuiryCycle = '7';
		fnAPP_AcceptInqDateEnd();
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
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

// 이체입력
int CTranCmn::fnAPP_AcceptTrans()
{
	if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
	{
		if(TranCode == TC_TRANSFER)
			fnAPP_AcceptTranChoice();
	}

	if(TranCode == TC_SVCTRAN1)
	{
		fnAPP_AcceptTMoney();									// 이체금액입력
		fnAPP_AcceptPassword();								
	}
	else
	if(TranCode == TC_SVCTRAN)									//#N0181
	{
		fnAPP_AcceptInAccountNum();								// 수취계좌번호입력
		fnAPP_AcceptTMoney();									// 이체금액입력
		fnAPP_AcceptPassword();								
	}
	else
	{
		//#0038
		if (MenuAvail & MENU_MU)
		{
//			memcpy(Accept.BankNum, KIUBANKID, sizeof(Accept.BankNum));		
			if(m_pProfile->DEVICE.ETCDevice20 != HANA)				//#N0214
				fnAPP_AcceptJuminNum();									// 주민번호입력
			fnAPP_AcceptMuAccountNum();                             // 계좌번호 			
			fnAPP_AcceptMuConfirmNum();								// 무매체거래승인번호입력
		}
		else
		{
			fnAPP_AcceptPbConfirmNum();
		}

		fnAPP_AcceptBankNum();									// 은행번호입력
		fnAPP_AcceptInAccountNum();								// 수취계좌번호입력
		fnAPP_AcceptTMoney();									// 이체금액입력
		//fnAPP_AcceptPbConfirmNum();
		fnAPP_AcceptPassword();								
	}
	
	return T_OK;
}

// 입금입력
int CTranCmn::fnAPP_AcceptDep()
{

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("입금은행 : %s", fnAPP_Get_BankName(CardBank)));
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("입금은행 : %d", CardBank));

	if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
	{
		Accept.SlipChoiceFlag = TRUE;
//		fnAPP_AcceptSlipChoice();
	}
	else
	{
		m_pDevCmn->fnSCR_DisplayScreen(526);
		Delay_Msg(2000);
	}

	//#0038
	if (MenuAvail & MENU_MU)
	{
		fnAPP_AcceptJuminNum();									// 주민번호입력
		fnAPP_AcceptMuAccountNum();                             // 계좌번호 			
//		fnAPP_AcceptMuConfirmNum();								// 무매체거래승인번호입력
		fnAPP_AcceptTelNum(); //#0107 - #0111 ***
		if (m_pProfile->DEVICE.ETCDevice20 == KIUP  || m_pProfile->DEVICE.ETCDevice20 == HANA)  //#N0201 무매체 송금인 입력 위치 수정
		{
			fnAPP_AcceptRemitName(); //#0101 -> #0107 ** (공장출하)
		}
	}


	//통장승인번호 -> #0035
	//fnAPP_AcceptPbConfirmNum();
	fnAPP_AcceptPassword();

	memcpy(Accept.BankNum, &pCardData3->BankNo, 3);

	fnAPP_InitializeSerialTranDS();								// 연속거래변수초기화
	Accept.MoneyInKind = TRAN_DEP_CASH;

	m_pDevCmn->fnAPL_SetProcCount('3');						// 진행설정
	fnAPP_SendHost();										// 송신
	fnAPP_RecvHost();										// 수신
	if (m_pDevCmn->TranResult)								// HOST OK
	{
		m_pDevCmn->fnAPL_SetProcCount('5');					// 진행설정
		fnAPP_PMEAcceptDepConfirm();					// 타행입금조회 확인화면
		/*
		if ((m_pProfile->DEVICE.ETCDevice20 == KIUP) && (MenuAvail & MENU_MU))
		{
			fnAPP_AcceptRemitName(); //#0101 -> #0107 ** (공장출하)
		}
		*/
		AddSerialFlg = FALSE;		
		TranCode2 = TRANID_4410;
		TranCode2Save = TRANID_4410;
	}
	else
	{
		m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG

		fnAPP_PSPProc(DEV_JPR);								// 저널프린트
		fnAPP_PMDProc();									// 메세지표시
		fnAPP_PSPProc(DEV_SPR);								// 명세프린트

		fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU );	
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU , FALSE);
		fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU);
		fnAPP_TerminateProc(T_CANCEL);
	}


	CancelDepPrintFlg = FALSE;									// 취소입금인자Flg(초기:실행무)
//	CancelDepSendFlg = TRUE;									// 취소입금처리Flag(초기:실행무) Set
	fnAPD_CheckDeposit();										// 입금수표

	RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP", 1);;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("입금계수 : 시작"));
	fnAPD_CashDeposit();										// 입금현금
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("입금계수 : 완료"));
	RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP", 0);

	AddString(Accept.CheckMoney, Accept.CashMoney, 15, Accept.Money);

	fnAPP_AcceptMoneyInConfirm();								// 입금금액확인


	return T_OK;
}

int CTranCmn::fnAPP_AcceptDep_KB()  //#N0226
{

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("입금은행 : %s", fnAPP_Get_BankName(CardBank)));
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("입금은행 : %d", CardBank));

	if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
	{
		Accept.SlipChoiceFlag = TRUE;
//		fnAPP_AcceptSlipChoice();
	}
	else
	{
		m_pDevCmn->fnSCR_DisplayScreen(526);
		Delay_Msg(2000);
	}

	fnAPP_AcceptPassword();

	memcpy(Accept.BankNum, &pCardData3->BankNo, 3);

	if (m_pDevCmn->AtmDefine.MachineType == U3100K || m_pDevCmn->AtmDefine.MachineType == U8100)
	{
		fnAPP_InitializeSerialTranDS();							// 연속거래변수초기화
		Accept.MoneyInKind = TRAN_DEP_CASH;
	}
	else
	{
		fnAPP_AcceptMoneyInKind();									// 입금권종선택
	}

	if (PbTran & MENU_PB)
		m_pDevCmn->fnSCR_DisplayScreen(805);			// 읽기(통장)

	CancelDepPrintFlg = FALSE;									// 취소입금인자Flg(초기:실행무)
//	CancelDepSendFlg = TRUE;									// 취소입금처리Flag(초기:실행무) Set
	fnAPD_CheckDeposit();										// 입금수표

	RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP", 1);;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("입금계수 : 시작"));
	fnAPD_CashDeposit();										// 입금현금
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("입금계수 : 완료"));
	RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP", 0);

	AddString(Accept.CheckMoney, Accept.CashMoney, 15, Accept.Money);

	fnAPP_AcceptMoneyInConfirm();								// 입금금액확인

	m_pDevCmn->fnAPL_SetProcCount('3');						// 진행설정		
	TranCode2 = TRANID_4410;
	TranCode2Save = TRANID_4410;

	return T_OK;
}

// 신한카드선택 (유/무)
int	CTranCmn::fnAPP_MuAcceptCardChoice()
{
	CString strReturn("");


	m_pDevCmn->fnSCR_DisplayScreen(434, K_30_WAIT, PIN_MENU_MODE);
																
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == "동양종합금융증권")
		Accept.MuBankTranBrandFlag = TYJSBANK;
	else
	if (strReturn == "신한은행")
		Accept.MuBankTranBrandFlag = SHIBANK;
	else
	if ((strReturn == S_NO)		||
		(strReturn == S_TIMEOVER))
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else ;

	return T_OK;
}


// 무매체계좌번호입력
int CTranCmn::fnAPP_AcceptMuAccountNum()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.MuAccountNumSize = 0;								// 무매체계좌번호길이
	memset(Accept.MuAccountNum, ' ', sizeof(Accept.MuAccountNum));
																// 무매체계좌번호
/////////////////////////////////////////////////////////////////////////////
	ScrNum = 408;												

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);		
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.MuAccountNum))	||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		Accept.MuAccountNumSize = strReturn.GetLength();
		memcpy(Accept.MuAccountNum, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// 무매체거래승인번호입력
int CTranCmn::fnAPP_AcceptMuConfirmNum()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		nCnt = 0;
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PbConfirmNum, '0', sizeof(Accept.PbConfirmNum));		
																// 무매체거래승인번호
/////////////////////////////////////////////////////////////////////////////
	if (MenuAvail & MENU_MU)   // 무매체
	{							
		if((TranCode == TC_MOBILEWITHDRAW) && (m_pProfile->DEVICE.ETCDevice20 == KIUP))
			ScrNum = 3802;											// 무매체거래승인번호 
		else
			ScrNum = 215;											// #N0182										// 무매체거래승인번호 
	}

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	while (TRUE)
	{
		if(nCnt > 2) fnAPP_CancelProc(T_INPUTOVER);
		// 통장거래승인번호 6자리 확대
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE, "", 6, 6, FALSE, "CANCEL");
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if ((strReturn == S_CANCEL)	||
			(strReturn == S_CANCEL2))
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);		
		else														// 자료검증   //무매체 이용자번호 5자리 가능
		if ((strReturn.GetLength() < sizeof(Accept.PbConfirmNum) - 1)	||
			(strReturn.GetLength() > sizeof(Accept.PbConfirmNum))		||
			(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))		||		
			(IsZero(strReturn.GetBuffer(0), strReturn.GetLength()))	)
		{
//			fnAPP_CancelProc(T_INPUTERROR);
			nCnt++;
			continue;

		}
		else
		{
			Accept.PbConfirmNumSize = strReturn.GetLength();		// 통장거래승인번호 6자리 확대
			memcpy(Accept.PbConfirmNum, strReturn.GetBuffer(0), Accept.PbConfirmNumSize);
			break;
		}
	}

	return T_OK;
}


// 사고신고계좌번호입력
int CTranCmn::fnAPP_AcceptLostAccountNum()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.LostAccountNumSize = 0;								// 분실계좌번호길이
	memset(Accept.LostAccountNum, ' ', sizeof(Accept.LostAccountNum));
																// 분실계좌번호
/////////////////////////////////////////////////////////////////////////////

	if (TranCode == TC_LOSTCARD)								// 카드사고신고
		ScrNum = 413;
	else
	if (TranCode == TC_LOSTPB)									// 통장사고신고
		ScrNum = 414;

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);		
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.LostAccountNum))	||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		Accept.LostAccountNumSize = strReturn.GetLength();
		memcpy(Accept.LostAccountNum, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// 비밀번호입력
int CTranCmn::fnAPP_AcceptPassword()
{
	int		ScrNum = 0;
	CString strReturn("");
	int			nCnt = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PassWord, 0, sizeof(Accept.PassWord));		// 비밀번호
	memset(Accept.PassWordEnc, ' ', sizeof(Accept.PassWordEnc));		// 비밀번호
/////////////////////////////////////////////////////////////////////////////

	if(TranProc == TRAN_DEP)
	{
		memcpy(Accept.PassWord, ZERO4, 4);
		fnAPD_FICGetEncipher();								// 비밀번호 암호화

		return T_OK;
	}

	ScrNum = 201;
	
	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnAPL_CaptureFaceSave(3, FALSE, fnAPP_GetCaptureData());		// 얼굴촬영


	while (TRUE)
	{

		if(nCnt > 2) fnAPP_CancelProc(T_INPUTOVER);
		
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE2, "", 4, 4, FALSE, "CANCEL");
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);	
		else
		if (strReturn == S_CLEAR)
		{
			nCnt++;
			continue;
		}
		else
		{
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptPassword2(%s)", strReturn);
			if ((strReturn.GetLength() != sizeof(Accept.PassWord))	    ||	// 자료검증
				(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())) ||
				(IsZero(strReturn.GetBuffer(0), 4)))
			{
					nCnt++;
					continue;
			}				
		}
		break;
	}

	memcpy(Accept.PassWord, strReturn.GetBuffer(0), strReturn.GetLength());

/*
	char tempPassword[64];
	char tempPassword2[64];
	memset(tempPassword, 0x00, sizeof(tempPassword));
	memset(tempPassword2, 0x00, sizeof(tempPassword2));
	memcpy(tempPassword, Accept.PassWord, sizeof(Accept.PassWord));
	int nRet = TYEncrypt(tempPassword, tempPassword2);
	MakeUnPack(tempPassword2, Accept.PassWordEnc, 16);
*/
	if (((MenuFICMode)	||									// 금융IC거래	2004.02.03
		(MenuEPBMode)) &&
		!(EMVReadFlg))
	{
	
		
		if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 823)
			m_pDevCmn->fnSCR_DisplayScreen(823);			// 읽기(카드)
	
		fnAPD_FICGetEncipher();								// 비밀번호 암호화
/*
		fnAPD_MaterialOut(DEV_MCU);							// Card Out
		fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract
*/
	}

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("비밀번호 : 입력완료 (%d)", strReturn.GetLength()));
	m_pDevCmn->fnAPL_CaptureFaceSave(4, FALSE, fnAPP_GetCaptureData());		// 얼굴촬영


	return T_OK;
}
// 통장거래승인번호입력
int CTranCmn::fnAPP_AcceptPbConfirmNum()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		nCnt = 0;
	int		nGlen = 0;
	if (!(MenuAvail & MENU_PB))
		return T_OK;
	
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PbConfirmNum, 0x20, sizeof(Accept.PbConfirmNum));		
																// 통장거래승인번호
	Accept.PbConfirmNumSize = 0;								// 통장거래승인번호길이	통장거래승인번호 6자리 확대
/////////////////////////////////////////////////////////////////////////////
//#0045
	if((m_pProfile->DEVICE.ETCDevice20 == NONGHYUP) || 
	   (m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	||
	   (m_pProfile->DEVICE.ETCDevice20 == KYONGNAM))      //#N0181 -> #N0282
		return T_OK;

	if (MenuAvail & MENU_PB)									// 통장
		ScrNum = 213;											// 통장거래승인번호입력

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);


	if(m_pProfile->DEVICE.ETCDevice20 == KIUP || m_pProfile->DEVICE.ETCDevice20 == HANA)
		nGlen = 5;
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KEB)
		nGlen = 6;
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KB)					//#N0239 국민 통장 비밀번호 4~6
		nGlen = 4;
	else
		nGlen = 6;



	while (TRUE)
	{
		if(nCnt > 2) fnAPP_CancelProc(T_INPUTOVER);
		// 통장거래승인번호 6자리 확대
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE, "", 5, 6, FALSE, "CANCEL");
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if ((strReturn == S_CANCEL)	||
			(strReturn == S_CANCEL2))
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);		
		else														// 자료검증
		if ((strReturn.GetLength() < nGlen)	||
			(strReturn.GetLength() > sizeof(Accept.PbConfirmNum))		||
			(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))		||		
			(IsZero(strReturn.GetBuffer(0), strReturn.GetLength()))	)
		{
//			fnAPP_CancelProc(T_INPUTERROR);
			nCnt++;
			continue;

		}
		else
		{
			Accept.PbConfirmNumSize = strReturn.GetLength();		// 통장거래승인번호 6자리 확대
			memcpy(Accept.PbConfirmNum, strReturn.GetBuffer(0), Accept.PbConfirmNumSize);

			if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
			{
				//통장거래승인번호 6자리 확대 ─────────────
				if (Accept.PbConfirmNumSize == 5)
					fnAPP_AccpetPbConfirmMSG();
				// ──────────────────────────────────
			}
			break;
		}
	}

	return T_OK;
}

// 폰뱅킹비밀번호입력
int CTranCmn::fnAPP_AcceptTelPassword()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PbPassWord, '0', sizeof(Accept.PbPassWord));	// 폰뱅킹비밀번호
/////////////////////////////////////////////////////////////////////////////

	ScrNum = 201;
	
	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE, "", 6, 6, FALSE, "CANCEL");
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);		
	else
	if ((strReturn.GetLength() != sizeof(Accept.PbPassWord))	||	// 자료검증
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.PbPassWord, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// 무매체거래비밀번호입력(사용안함)
int CTranCmn::fnAPP_AcceptMuPassword()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.MuPassWord, '0', sizeof(Accept.MuPassWord));	// 무매체비밀번호
/////////////////////////////////////////////////////////////////////////////

	ScrNum = 201;
	
	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE, "", 6, 6, FALSE, "CANCEL");
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);		
	else
	if ((strReturn.GetLength() != sizeof(Accept.MuPassWord))  ||// 자료검증
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.MuPassWord, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// 출금금액입력
int CTranCmn::fnAPP_AcceptMoney()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		GetCase;
	int		MoneyMaxOutScr = 30;
	int		CheckValue = 10;
	char	WithMsg[81], DepMsg[81];
	CString	szOnOffString("");
	int		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;
	int		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;
	int		MAXCASHOUTCNT = BRM_MONEY_MAXOUT;
	int		iMax50TCashOut = BRM_MONEY_MAXOUT;
	int		FOREGIN_MAXOUT = IniGetInt(_TRANS_INI, TRANS_SEC, "ForeginMaxDispAmt", 30);

	memset(Accept.Money, '0', sizeof(Accept.Money));			//#N0265
	memset(Accept.Cash10T_Money, '0', sizeof(Accept.Cash10T_Money));
	memset(Accept.Cash50T_Money, '0', sizeof(Accept.Cash50T_Money));
	
	if (!fnAPP_CalcMoneyMaxOut())								// 방출한도금액구하기
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("출금불가 : BRM 사용불가 상태"));

		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "출금이 불가 합니다");
		else
			fnAPP_CancelProc(T_MSG, "Withdrawal is not valid");
	}

	switch(m_pDevCmn->AtmDefine.ETCDevice20)
	{
	case KIUP:														
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;					// 최대방출금액(만원단위)
		if ((CardBank == KIUBANK) && (TranCode == TC_WITHDRAW || TranCode == TC_MOBILEWITHDRAW)) //#N0187
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	case KEB: //#0085	 													
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;					// 최대방출금액(만원단위)	#N0276
		if ((CardBank == KOEBANK || CardBank == IHANBANKID1 || CardBank == IHANBANKID2 || CardBank == IHANBANKID3 ||CardBank == IHANBANKID4) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	case NONGHYUP: 												
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT_70;				// 최대방출금액(만원단위)
		if ((CardBank == INONBANKID1 || CardBank == INONBANKID2 || CardBank == INONBANKID3 ||CardBank == INONBANKID4) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	case KWANGJU: 												
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// 최대방출금액(100만원) //#N0181
		if ((CardBank == IKAWBANKID) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	case KYONGNAM: 												
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// 최대방출금액(100만원) //#N0192
		if ((CardBank == IKYOBANKID) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	case HANA: 												
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// 최대방출금액(100만원) //#N0214
		if ((CardBank == KOEBANK || CardBank == IHANBANKID1 || CardBank == IHANBANKID2 || CardBank == IHANBANKID3 ||CardBank == IHANBANKID4) && (TranCode == TC_WITHDRAW))	//#N0276
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	case SHINHAN: 												
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// 최대방출금액(100만원) //#N0215
		if ((CardBank == ISHIBANKID || CardBank == ISHIBANKID1 || CardBank == ICHOBANKID) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	case SC: 												
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// 최대방출금액(100만원) //#N0219
		if ((CardBank == IKOFBANKID) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	case KB: 												
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT_70;				// 최대방출금액(만원단위) -> //#N0226
		if ((CardBank == IKUMBANKID1 || CardBank == IKUMBANKID2 || CardBank == IKUMBANKID3 || CardBank == IKUMBANKID4 || CardBank == IKUMBANKID5) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	case CITI: 												
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT_70;				// 최대방출금액(70만원) //#N0238
		if ((CardBank == IGOOBANKID1 || CardBank == IGOOBANKID2 || CardBank == ICITBANKID) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.JaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.JaMaxDispAmt;
		}
		else
		{
			MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
			MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;
		}
		break;
	case NICE: 												
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// 최대방출금액(100만원) //#N0219

		MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
		MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;

		break;
	default:
		MONEY_MAXOUTCNT =  m_pProfile->TRANS.TaMaxDispAmt;
		MONEY_MAXOUT    =  m_pProfile->TRANS.TaMaxDispAmt;
		break;
	}

	if (WithAvail == TRAN_WITH_50TCASH)
	{

		MAXCASHOUTCNT = MONEY_MAXOUTCNT % CASH_FIFTY_THOUSAND;
		if (MAXCASHOUTCNT)
			MAXCASHOUTCNT = (MONEY_MAXOUTCNT / CASH_FIFTY_THOUSAND) + 1;
		else
			MAXCASHOUTCNT = (MONEY_MAXOUTCNT / CASH_FIFTY_THOUSAND);


		iMax50TCashOut = MONEY_MAXOUTCNT % CASH_FIFTY_THOUSAND;
		if (iMax50TCashOut)
			iMax50TCashOut = (MONEY_MAXOUTCNT / CASH_FIFTY_THOUSAND) + 1;
		else
			iMax50TCashOut = (MONEY_MAXOUTCNT / CASH_FIFTY_THOUSAND);
		
		if ((MAXCASHOUTCNT >= 1)	&&							// 1만 ~ 기기한도
			(MAXCASHOUTCNT <= iMax50TCashOut))
			;
		else MAXCASHOUTCNT = 0;									// 금액이상시 0만원
	}
	else
	{
		MAXCASHOUTCNT = MONEY_MAXOUTCNT;
		if ((MAXCASHOUTCNT >= 1)	&&							// 1만 ~ 기기한도
			(MAXCASHOUTCNT <= MONEY_MAXOUTCNT))
			;
		else MAXCASHOUTCNT = 0;									// 금액이상시 0만원
	}

	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
		CheckValue = CHECK_MILLION;								// 100만원
	else
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
		CheckValue = CHECK_HUNDRED_THOUSAND;					// 10만원
	else
		CheckValue = CHECK_UNKNOWN;


	//#0103
	if ((TranCode == TC_SVCWITH) && (CardService == SHINHANCARD))
	{
		MoneyMaxOut = BRM_MONEY_MAXOUT_50;

		//#N0170
		if (WithAvail == TRAN_WITH_50TCASH)
			CashMaxOutCnt = BRM_MONEY_MAXOUT_50 / CASH_FIFTY_THOUSAND;
		else
			CashMaxOutCnt = BRM_MONEY_MAXOUT_50;
			
		MONEY_MAXOUT = BRM_MONEY_MAXOUT_50;  //#N0167  기존 타행 설정 금액이 50보다 크면 바로 기타금액 입력으로 표시됨 
	}
	else
	if ((TranCode == TC_SVCWITH) && (CardService != SHINHANCARD))   //#N0167
	{
		MoneyMaxOut = BRM_MONEY_MAXOUT_30;
		//#N0170
		if (WithAvail == TRAN_WITH_50TCASH)
			CashMaxOutCnt = BRM_MONEY_MAXOUT_30 / CASH_FIFTY_THOUSAND;
		else
			CashMaxOutCnt = BRM_MONEY_MAXOUT_30;
		
		MONEY_MAXOUT = BRM_MONEY_MAXOUT_30;
	}
	else
	if ((TranCode == TC_FOREGIN))   //#N0171
	{
		MoneyMaxOut = FOREGIN_MAXOUT;
		//#N0171
		if (WithAvail == TRAN_WITH_50TCASH)
			CashMaxOutCnt = FOREGIN_MAXOUT / CASH_FIFTY_THOUSAND;
		else
			CashMaxOutCnt = FOREGIN_MAXOUT;
		
		MONEY_MAXOUT = FOREGIN_MAXOUT;
	}
	else
	if((TranCode == TC_MOBILEWITHDRAW) && (MONEY_MAXOUT > BRM_MONEY_MAXOUT_50))		  //#N0182
	{
		MoneyMaxOut = BRM_MONEY_MAXOUT_50;
		//#N0171
		if (WithAvail == TRAN_WITH_50TCASH)
			CashMaxOutCnt = BRM_MONEY_MAXOUT_50 / CASH_FIFTY_THOUSAND;
		else
			CashMaxOutCnt = BRM_MONEY_MAXOUT_50;
		
		MONEY_MAXOUT = BRM_MONEY_MAXOUT_50;
	}
	
	GetCase = 0;												// 현금선택
	Accept.CheckCount = 0;										// 수표매수 초기화

	if ((MoneyMaxOut < MONEY_MAXOUT) || (TranCode == TC_MOBILEWITHDRAW))  //#N0182
		GetCase = 1;

	if (m_pDevCmn->EarPhoneInserted == TRUE)		
		GetCase = 1;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptMoney:WithAvail[%x]", WithAvail);
	fnAPP_MenuTranMsg(WithAvail, WithMsg, DepAvail, DepMsg);	// Menu Tran Msg
	while (GetCase <= 7)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptMoney WithAvail [0x%02x]", WithAvail); // test code
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptMoney CASE [%d] START", GetCase); // test code
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "                            Accept.Money[%15.15s]", Accept.Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "                            Accept.CashMoney[%15.15s]", Accept.CashMoney);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "                            Accept.Cash50T_Money[%15.15s]", Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "                            Accept.Cash10T_Money[%15.15s]", Accept.Cash10T_Money);
		switch (GetCase)
		{
		case 0:												// 금액선택
			GetCase = fnAPP_SelectMoneyCase0(WithMsg);
			break;
		case 1:												// 금액입력(만원)
			GetCase = fnAPP_InputMoneyCase1(WithMsg);
			break;
		case 2:												// 권종선택
			GetCase = fnAPP_SelectKindofMoneyCase2(CheckValue);	// 100만원 수표지원
			break;
		case 3:												// 수표선택&수표금액입력
			GetCase = fnAPP_SelectCheckCase3(WithMsg);
			break;
		case 4:												// 수표입력(최소~최대)
			GetCase = fnAPP_InputCheckCase4(CheckValue);	// 100만원 수표지원
			break;
		case 7:
			if(m_pProfile->DEVICE.ETCDevice20 == HANA)									// 권종별 방출매수설정(자동)	#N0214
				GetCase = fnAPP_Input50TCashMoney_HANA();
			else
				GetCase = fnAPP_Input50TCashMoney();
			break;
		case 5:												// 금액확인
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptMoney:Accept.Money[%15.15s], " \
												"Accept.CashMoney[%15.15s], "		\
												"Accept.CheckMoney[%15.15s], "	\
												"Accept.Cash50T_Money[%15.15s], "		\
												"Accept.Cash10T_Money[%15.15s], "	\
												"MoneyMaxOut[%d], "				\
												"MoneyMaxOutCnt[%d], "			\
												"CashMaxOutCnt[%d], "			\
												"CheckMaxOutCnt[%d], "			\
												"CheckLowMaxOutCnt[%d], "		\
												"CheckHighMaxOutCnt[%d]",
												Accept.Money,
												Accept.CashMoney,
												Accept.CheckMoney,
												Accept.Cash50T_Money,
												Accept.Cash10T_Money,
												MoneyMaxOut,
												MoneyMaxOutCnt,
												CashMaxOutCnt,
												CheckMaxOutCnt,
												CheckLowMaxOutCnt,
												CheckHighMaxOutCnt);

			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
			{
				if (!IsZero(&Accept.CheckMoney[2], 5))
					fnAPP_CancelProc(T_PROGRAM);
			}
			else
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
			{
				if (!IsZero(&Accept.CheckMoney[3], 5))
					fnAPP_CancelProc(T_PROGRAM);
			}

			if (!IsNum(Accept.Money, 15)									||		
				!IsNum(Accept.CashMoney, sizeof(Accept.CashMoney))		||
				!IsNum(Accept.CheckMoney, sizeof(Accept.CheckMoney))	||
				!IsNum(Accept.Cash50T_Money, sizeof(Accept.Cash50T_Money))	||
				!IsNum(Accept.Cash10T_Money, sizeof(Accept.Cash10T_Money))	||
				IsZero(Accept.Money, 15)									||
				!IsZero(&Accept.Money[11], 4)							||
				!IsZero(&Accept.CashMoney[11], 4)						||
				!IsZero(&Accept.Cash50T_Money[11], 4)						||
				!IsZero(&Accept.Cash10T_Money[11], 4)						)
				fnAPP_CancelProc(T_PROGRAM);

			// 금액자료합계검증			
			if ((Asc2Int(Accept.CashMoney, 11) + Asc2Int(Accept.CheckMoney, 11)) != Asc2Int(Accept.Money, 11))					
				fnAPP_CancelProc(T_PROGRAM);

			if ((m_pDevCmn->ChangeBCFlag)		&&		
				(m_pDevCmn->Withdraw50TFlag))		
			{
				if (((Asc2Int(Accept.Cash10T_Money, 11) + (Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND)) > 
					CashMaxOutCnt)	||
					((Asc2Int(Accept.Cash10T_Money, 11) + (Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND)) > 
					BRM_MAXCASHOUTCNT))
					fnAPP_CancelProc(T_PROGRAM);		// (만원권매수+5만원권매수) > 총현금금액매수 체크
														// (만원권매수+5만원권매수) > 현금최대방출매수 체크 
			}
			else
			{
				if ((Asc2Int(Accept.CashMoney, 11) > CashMaxOutCnt)		|| 
					(Asc2Int(Accept.CashMoney, 11) > BRM_MAXCASHOUTCNT))
					fnAPP_CancelProc(T_PROGRAM);		// 금액자료현금매수검증
			}

			if ((m_pDevCmn->Withdraw50TFlag)            &&					
				((WithAvail == TRAN_WITH_50TCASH)		||
				(WithAvail == TRAN_WITH_50TCASHCHECK)))
			{						
				if (((Asc2Int(Accept.CashMoney, sizeof(Accept.CashMoney)) % CASH_FIFTY_THOUSAND) != 0) ||		
					(Asc2Int(Accept.Cash10T_Money, sizeof(Accept.Cash10T_Money)) != 0))
					fnAPP_CancelProc(T_MSG, "입력금액이 5만원 단위만 가능합니다.");						
			}
			
			if ((Asc2Int(Accept.Cash50T_Money, sizeof(Accept.Cash50T_Money)) % CASH_FIFTY_THOUSAND) != 0)						
				fnAPP_CancelProc(T_MSG, "입력금액이 5만원 단위만 가능합니다.");

			// 100만원권 사용시 금액자료검증 Start
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
			{
				if ((Asc2Int(Accept.CheckMoney, 9) > CheckMaxOutCnt)	||
					(Asc2Int(Accept.CheckMoney, 9) > BRM_MAXCHECKOUTCNT_100CHECK))
					fnAPP_CancelProc(T_PROGRAM);				// 금액자료수표매수검증
			}
			else
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
			{
				if ((Asc2Int(Accept.CheckMoney, 10) > CheckMaxOutCnt)	||
					(Asc2Int(Accept.CheckMoney, 10) > BRM_MAXCHECKOUTCNT))
					fnAPP_CancelProc(T_PROGRAM);				// 금액자료수표매수검증
			}

			if ((m_pDevCmn->ChangeBCFlag)		&&			
				(m_pDevCmn->Withdraw50TFlag))				
			{
				if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
				{
					if ((((Asc2Int(Accept.Cash10T_Money, 11) + (Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND)) + Asc2Int(Accept.CheckMoney, 2)) >
						 MoneyMaxOutCnt)	||
						(((Asc2Int(Accept.Cash10T_Money, 11) + (Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND)) + Asc2Int(Accept.CheckMoney, 2)) >
						BRM_MONEY_MAXOUTCNT))
						fnAPP_CancelProc(T_PROGRAM);			// 금액자료현금매수검증(만원권)						
				}
				else
				if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
				{
					if ((((Asc2Int(Accept.Cash10T_Money, 11) + (Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND)) + Asc2Int(Accept.CheckMoney, 3)) >
						 MoneyMaxOutCnt)	||
						(((Asc2Int(Accept.Cash10T_Money, 11) + (Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND)) + Asc2Int(Accept.CheckMoney, 3)) >
						BRM_MONEY_MAXOUTCNT))
						fnAPP_CancelProc(T_PROGRAM);			// 금액자료현금매수검증(만원권)						
				}
			}
			else
			{
				if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
				{
					if (((Asc2Int(Accept.CashMoney, 11) + Asc2Int(Accept.CheckMoney, 9)) >
						MoneyMaxOutCnt)										||
						((Asc2Int(Accept.CashMoney, 11) + Asc2Int(Accept.CheckMoney, 9)) >
						BRM_MONEY_MAXOUTCNT))							
						fnAPP_CancelProc(T_PROGRAM);			// 금액자료매수검증
				}
				else
				if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
				{
				if (((Asc2Int(Accept.CashMoney, 11) + Asc2Int(Accept.CheckMoney, 10)) >
					MoneyMaxOutCnt)										||
					((Asc2Int(Accept.CashMoney, 11) + Asc2Int(Accept.CheckMoney, 10)) >
					BRM_MONEY_MAXOUTCNT))							
					fnAPP_CancelProc(T_PROGRAM);			// 금액자료매수검증
				}
			}

			// 금액자료합계검증			
			if ((Asc2Int(Accept.CashMoney, 11) + Asc2Int(Accept.CheckMoney, 11)) != Asc2Int(Accept.Money, 11))					
				fnAPP_CancelProc(T_PROGRAM);

			if (((Asc2Int(Accept.CashMoney, 11) + (Asc2Int(Accept.CheckMoney, 10) * 10)) >  
				MoneyMaxOut)										||
				((Asc2Int(Accept.CashMoney, 11) + (Asc2Int(Accept.CheckMoney, 10) * 10)) >
				BRM_MONEY_MAXOUT))							
				fnAPP_CancelProc(T_PROGRAM);				// 금액자료금액검증
															
			GetCase = 9;									// 종료

		}
	}
 
	if ((TranProc == TRAN_WITH)	||
		(TranProc == TRAN_DEP)	)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[CASH][%15.15s]", Accept.CashMoney);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[CHECK][%15.15s]", Accept.CheckMoney);
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[CASH][%15.15s]", Accept.Money);
	}

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("금액입력 : %d만원", Asc2Int(Accept.Money, sizeof(Accept.Money))/10000));

	return T_OK;
}
// 금액입력, case 1		
int	CTranCmn::fnAPP_InputMoneyCase_Etc()	
{
	int	GetCase = 9;
	int ScrNum;
	CString strReturn;

	ScrNum = 302;	
	m_pDevCmn->fnSCR_DisplayNumeric(1, BRM_MONEY_MAXOUT);	// 출금정보출력
	m_pDevCmn->fnSCR_DisplayNumeric(2, BRM_MONEY_MAXOUT);	// 현금최대방출매수
	m_pDevCmn->fnSCR_DisplayNumeric(3, BRM_MONEY_MAXOUT);	// 최대방출금액

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT);		// 금액입력(만원)
	strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(15);
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((strReturn.GetLength() != 15)		||				// 금액자료검증
		(!IsNum(strReturn.GetBuffer(0), 15))	||		
		(IsZero(strReturn.GetBuffer(0), 15))	||
		(!IsZero(&(strReturn.GetBuffer(0)[11]), 4)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	if (Asc2Int(strReturn.GetBuffer(0), 11) > BRM_MONEY_MAXOUT)
	{															// 출금금액입력초과시
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "입력금액 초과입니다");
		else
			fnAPP_CancelProc(T_MSG, "Withdrawal limit exceeded", "Please enter amount again");
	}
	else
	{
		memcpy(Accept.Money, strReturn.GetBuffer(0), 15);
		memcpy(Accept.CashMoney, strReturn.GetBuffer(0), 15);
	}

	return T_OK;
}


// 금액선택(버튼), case 0 
int	CTranCmn::fnAPP_SelectMoneyCase0(char * WithMsg)
{
	int	GetCase = 9;
	int ScrNum;
	CString strReturn;

	if ((m_pDevCmn->Withdraw50TFlag)		&&					// V05-01-01-#01
	    ((WithAvail == TRAN_WITH_50TCASH)	||					// 5만원권만 출금가능
		 (WithAvail == TRAN_WITH_50TCASHCHECK)))				// 5만원+수표
	{
		GetCase = 1;
		return GetCase;
	}

	// 100매출금가능(자행이고 예금인출인경우)											
	if(WithAvail == TRAN_WITH_CHECK)
		ScrNum = 306;
	else
		ScrNum = 301;										// 금액선택

	//#N0249
	if( MoneyMaxOut < BRM_MONEY_MAXOUT_50)			//50만원 미만
		m_pDevCmn->fnSCR_DisplayImage(7, FALSE);
	if( MoneyMaxOut < BRM_MONEY_MAXOUT_70)			//70만원 미만
		m_pDevCmn->fnSCR_DisplayImage(8, FALSE);
	if( MoneyMaxOut < BRM_MONEY_MAXOUT)				//100만원 미만
		m_pDevCmn->fnSCR_DisplayImage(9, FALSE);
	
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT , PIN_MENU_MODE1);   
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	GetCase = 2;												// 권종선택
	if (strReturn == "1만원")
		memcpy(Accept.Money, "000000000010000", 15);
	else
	if (strReturn == "2만원")
		memcpy(Accept.Money, "000000000020000", 15);
	else
	if (strReturn == "3만원")
		memcpy(Accept.Money, "000000000030000", 15);
	else
	if (strReturn == "5만원")
		memcpy(Accept.Money, "000000000050000", 15);
	else
	if (strReturn == "7만원")
		memcpy(Accept.Money, "000000000070000", 15);
	else
	if (strReturn == "10만원")
		memcpy(Accept.Money, "000000000100000", 15);
	else
	if (strReturn == "15만원")
		memcpy(Accept.Money, "000000000150000", 15);
	else
	if (strReturn == "20만원")
		memcpy(Accept.Money, "000000000200000", 15);
	else
	if (strReturn == "30만원")
		memcpy(Accept.Money, "000000000300000", 15);
	else
	if (strReturn == "40만원")
		memcpy(Accept.Money, "000000000400000", 15);
	else
	if (strReturn == "50만원")
		memcpy(Accept.Money, "000000000500000", 15);
	else
	if (strReturn == "60만원")
		memcpy(Accept.Money, "000000000600000", 15);
	else
	if (strReturn == "70만원")
		memcpy(Accept.Money, "000000000700000", 15);
	else
	if (strReturn == "80만원")
		memcpy(Accept.Money, "000000000800000", 15);
	else
	if (strReturn == "90만원")
		memcpy(Accept.Money, "000000000900000", 15);
	else
	if (strReturn == "100만원")
		memcpy(Accept.Money, "000000001000000", 15);
	else
	if (strReturn == "기타")						// 기타
		GetCase = 1;								// 금액입력(만원)
	else	
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
	
	return GetCase;
}

// 금액입력, case 1		
int	CTranCmn::fnAPP_InputMoneyCase1(char * WithMsg)	
{
	int	GetCase = 9;
	int ScrNum;
	CString strReturn;
	int		nCurrCheckKind = m_pDevCmn->fnAPL_GetKindOfOutCheck();


	if ((m_pDevCmn->Withdraw50TFlag)		&&			
		((WithAvail == TRAN_WITH_50TCASH)	||					// 5만원권만 출금가능
		 (WithAvail == TRAN_WITH_50TCASHCHECK)))				// 5만원+수표
	{
		if (TranCode == TC_MOBILEWITHDRAW) //#N0182
			ScrNum = 325;							
		else
			ScrNum = 325;
								
		m_pDevCmn->fnSCR_DisplayNumeric(1, MoneyMaxOut);	// 출금정보출력
		m_pDevCmn->fnSCR_DisplayNumeric(2, CashMaxOutCnt * CASH_FIFTY_THOUSAND);	// 현금최대방출매수
		m_pDevCmn->fnSCR_DisplayNumeric(3, CheckMaxOutCnt * 10);	// 최대방출금액

	}
	else
	{
		if (TranCode == TC_MOBILEWITHDRAW) //#N0182
			ScrNum = 3801;							
		else
			ScrNum = 302;
		
		if (nCurrCheckKind == CHECKKIND_MILLION)		
		{
			m_pDevCmn->fnSCR_DisplayNumeric(1, MoneyMaxOut);	// 출금정보출력
			m_pDevCmn->fnSCR_DisplayNumeric(2, CashMaxOutCnt);	// 현금최대방출매수
			m_pDevCmn->fnSCR_DisplayNumeric(3, CheckMaxOutCnt * 100);	// 최대방출금액
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayNumeric(1, MoneyMaxOut);	// 출금정보출력
			m_pDevCmn->fnSCR_DisplayNumeric(2, CashMaxOutCnt);	// 현금최대방출매수
			m_pDevCmn->fnSCR_DisplayNumeric(3, CheckMaxOutCnt * 10);	// 최대방출금액
		}
	}

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);		// 금액입력(만원)  #N0171
	strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(15);
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((strReturn.GetLength() != 15)		||				// 금액자료검증
		(!IsNum(strReturn.GetBuffer(0), 15))	||		
		(IsZero(strReturn.GetBuffer(0), 15))	||
		(!IsZero(&(strReturn.GetBuffer(0)[11]), 4)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	if (Asc2Int(strReturn.GetBuffer(0), 11) > MoneyMaxOut)
	{															// 출금금액입력초과시
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "입력금액 초과입니다");
		else
			fnAPP_CancelProc(T_MSG, "Withdrawal limit exceeded", "Please enter amount again");
	}
	else
	if ((Asc2Int(strReturn.GetBuffer(0), 10) + Asc2Int(&(strReturn.GetBuffer(0)[10]), 1)) > MoneyMaxOutCnt)
	{															// 예) 693만원
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "입력금액 매수 초과입니다");
		else
			fnAPP_CancelProc(T_MSG, "Withdrawal limit exceeded", "Please enter amount again");
	}
	else
	{
		memcpy(Accept.Money, strReturn.GetBuffer(0), 15);
		GetCase = 2;										// 권종선택
	}

	return GetCase;
}


// 권종선택, case 2		
// 100만원 수표지원 Parameter 변경
int	CTranCmn::fnAPP_SelectKindofMoneyCase2(int CheckValue)
{
	int	GetCase = 9;
	int ScrNum;
	CString strReturn;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SelectKindofMoneyCase2[CheckMaxOutCnt:%d],[Accept.Money:%d],[WithAvail:%x]", 
																CheckMaxOutCnt,  Asc2Int(Accept.Money, 11), WithAvail);

	if ((m_pDevCmn->EarPhoneInserted)				||			// 장애인ATM - 이어폰 삽입시 수표출금 불가 설정
		(!CheckMaxOutCnt) ||									// 수표불가
		(Asc2Int(Accept.Money, 10) < CheckValue))				// 권종별미만 금액(10/100만원)
	{
		memcpy(Accept.CashMoney, Accept.Money, sizeof(Accept.CashMoney));

		if ((m_pDevCmn->Withdraw50TFlag)		&&				
		    ((WithAvail == TRAN_WITH_50TCASH) ||
			 (WithAvail == TRAN_WITH_50TCASHCHECK)))
		{
			memcpy(Accept.Cash50T_Money, Accept.CashMoney, sizeof(Accept.Cash50T_Money));
			memset(Accept.Cash10T_Money, '0', sizeof(Accept.Cash10T_Money));
			GetCase = 5;
		}
		else if (Asc2Int(Accept.Money, 15) < 50000)				// 5만원 미만
		{
			memset(Accept.Cash50T_Money, '0', sizeof(Accept.Cash50T_Money));
			memcpy(Accept.Cash10T_Money, Accept.CashMoney, sizeof(Accept.Cash10T_Money));
			GetCase = 5;										// 금액확인
		}
		else													// 5~9만원 또는 5~99만원
		{				
			if ((WithAvail & TRAN_WITH_50TCASH))
				GetCase = 7;								// 현금방출매수(자동)
			else
			{											
				memcpy(Accept.Cash10T_Money, Accept.CashMoney, sizeof(Accept.Cash10T_Money));
				GetCase = 5;									
			}
		}
		return GetCase;
	}

	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)		// 10만원/100만원 수표선택에 따른 화면 변경
		ScrNum = 312;												// 출금권종선택
	else
		ScrNum = 312;

	if ((m_pDevCmn->Withdraw50TFlag)			&&				
		((WithAvail == TRAN_WITH_50TCASH)		||
		(WithAvail == TRAN_WITH_50TCASHCHECK))	)
	{
		if (Asc2Int(Accept.Money, 11) > CashMaxOutCnt*CASH_FIFTY_THOUSAND)
			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);			// 전액현금삭제
	}
	else
	{
		if (Asc2Int(Accept.Money, 11) > CashMaxOutCnt)
			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);			// 전액현금삭제
	}
																// 100만원 수표지원

	if (CheckValue == 0)										// CodeSonar
	{
		CheckValue = CHECK_HUNDRED_THOUSAND;
	}
	else
	if ((Asc2Int(Accept.Money, 10)%(CheckValue)) != 0)
		m_pDevCmn->fnSCR_DisplayImage(2, FALSE);				// 전액수표삭제
	if (Asc2Int(Accept.Money, 10) > (CheckMaxOutCnt * CheckValue))
		m_pDevCmn->fnSCR_DisplayImage(2, FALSE);				// 전액수표삭제
	if (Asc2Int(Accept.Money, 10) == CheckValue)							
		m_pDevCmn->fnSCR_DisplayImage(3, FALSE);				// 현금+수표삭제


	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT);								
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == "현금")									// 전액현금
	{	

		memcpy(Accept.CashMoney, Accept.Money, sizeof(Accept.CashMoney));

		if ((m_pDevCmn->Withdraw50TFlag)		&&			
		    ((WithAvail == TRAN_WITH_50TCASH)	||
			 (WithAvail == TRAN_WITH_50TCASHCHECK)))
		{
			memcpy(Accept.Cash50T_Money, Accept.CashMoney, sizeof(Accept.Cash50T_Money));
			memset(Accept.Cash10T_Money, '0', sizeof(Accept.Cash10T_Money));
			GetCase = 5;										// 금액확인
		}
		else
		if (WithAvail & (TRAN_WITH_50TCASH))
		{
			if	(m_pDevCmn->ChangeBCFlag)						
			{
				if (m_pDevCmn->Withdraw50TFlag		&&			
					WithAvail & TRAN_WITH_50TCASH	)
				{
					memcpy(Accept.Cash50T_Money, Accept.CashMoney, sizeof(Accept.Cash50T_Money));
					memset(Accept.Cash10T_Money, '0', sizeof(Accept.Cash10T_Money));
					GetCase = 7;
				}
				else
				{
					GetCase = 5;								// 현금방출매수(자동)
				}
			}
			else												
			{
				GetCase = 5;									
			}
		}
		else
		{
			memset(Accept.Cash50T_Money, '0', sizeof(Accept.Cash50T_Money));
			memcpy(Accept.Cash10T_Money, Accept.CashMoney, sizeof(Accept.Cash10T_Money));		
			GetCase = 5;										// 금액확인
		}
	}
	else
	if (strReturn == "수표")									// 전액수표
	{	
		memcpy(Accept.CheckMoney, Accept.Money, sizeof(Accept.CheckMoney));
		GetCase = 5;											// 금액확인
	}
	else
	if (strReturn == "수표+현금")								// 수표현금
	{
		GetCase = 4;
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
	else
		fnAPP_CancelProc(T_PROGRAM);		

	return GetCase;
}

// 수표선택, 입력, case 3					
int	CTranCmn::fnAPP_SelectCheckCase3(char * WithMsg)
{
	int	GetCase = 9;
	int ScrNum = 204;
	CString strReturn;

	int		GetMoneyMaxOut;

	// 수표권종에 따른 화면분기 필요 Start
	int		nCheckValue = 0;
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
	{
		nCheckValue = CHECK_MILLION;
		ScrNum = 521;
	}
	else
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
	{
		nCheckValue = CHECK_HUNDRED_THOUSAND;
		ScrNum = 521;
	}
	else
		fnAPP_CancelProc(T_INPUTOVER);
	// 수표권종에 따른 화면분기 필요 End

	if (!CashMaxOutCnt)											// 현금불가
		GetMoneyMaxOut = MoneyMaxOut;
	else GetMoneyMaxOut = Asc2Int(Accept.Money, 11);

	fnAPP_CalcCheckLowMaxOutCnt(GetMoneyMaxOut);	// 최소수표매수구하기
	fnAPP_CalcCheckHighMaxOutCnt(GetMoneyMaxOut);	// 최대수표매수구하기

//	ScrNum = 204;												// 수표금액입력
	m_pDevCmn->fnSCR_DisplayNumeric(1, GetMoneyMaxOut);			// 거래금액
	m_pDevCmn->fnSCR_DisplayNumeric(2, CheckLowMaxOutCnt);		// 수표최대방출매수
	m_pDevCmn->fnSCR_DisplayNumeric(3, CheckHighMaxOutCnt);		// 수표최대방출매수
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT);			// 수표금액입력(만원)
	strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(3);
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((strReturn.GetLength() != 3)		||					// 수표매수자료검증
		(!IsNum(strReturn.GetBuffer(0), 3))	||		
		(IsZero(strReturn.GetBuffer(0), 3)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	if ((Asc2Int(strReturn.GetBuffer(0))*nCheckValue) > MoneyMaxOut)// 만원단위	// 수표권종에 따른 한도체크
	{															// 수표출금금액입력초과시
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "수표 입력금액 초과입니다");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
 	if (Asc2Int(strReturn.GetBuffer(0), 3) > CheckMaxOutCnt)		
	{															// 수표매수초과
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "수표 입력금액 매수 초과입니다");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
	{
		sprintf(Accept.CheckMoney, "%15d", Asc2Int(strReturn.GetBuffer(0))*nCheckValue*10000);		
		memcpy(Accept.Money, Accept.CheckMoney, 15);
		GetCase = 5;											// 금액확인
	}

	return GetCase;
}

// 수표입력, case 4						
// 100만원권 지원 Parameter 변경
int	CTranCmn::fnAPP_InputCheckCase4(int CheckValue)				
{
	int	GetCase = 9;
	int ScrNum = 0;
	CString strReturn;

	int		GetMoneyMaxOut;
	int		CashMaxOutCnt1;
	int		MoneyMaxOut1;
	int		CashMaxOutCnt2;
	int		MoneyMaxOut2;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InputCheckCase4");

	if (!CashMaxOutCnt)								// 현금불가
		GetMoneyMaxOut = MoneyMaxOut;
	else GetMoneyMaxOut = Asc2Int(Accept.Money, 11);

	fnAPP_CalcCheckLowMaxOutCnt(GetMoneyMaxOut);	// 최소수표매수구하기
	fnAPP_CalcCheckHighMaxOutCnt(GetMoneyMaxOut);	// 최대수표매수구하기

	CashMaxOutCnt1 = (GetMoneyMaxOut - (CheckLowMaxOutCnt * CheckValue));	// 100만원권 지원
	MoneyMaxOut1 = (CheckLowMaxOutCnt * CheckValue) + CashMaxOutCnt1;		// 100만원권 지원
	CashMaxOutCnt2 = (GetMoneyMaxOut - (CheckHighMaxOutCnt * CheckValue));	// 100만원권 지원
	MoneyMaxOut2 = (CheckHighMaxOutCnt * CheckValue) + CashMaxOutCnt2;		// 100만원권 지원


	if ((m_pDevCmn->ChangeBCFlag)			&&
		(m_pDevCmn->Withdraw50TFlag)		&&				
		((WithAvail == TRAN_WITH_50TCASH)	||
		(WithAvail == TRAN_WITH_50TCASHCHECK)))
	{
		CashMaxOutCnt1 = CashMaxOutCnt1/CASH_FIFTY_THOUSAND;
		CashMaxOutCnt2 = CashMaxOutCnt2/CASH_FIFTY_THOUSAND;
	}


	if ((WithAvail == TRAN_WITH_CASH) ||
		(WithAvail == TRAN_WITH_2CASH))
	{
		if ((CashMaxOutCnt1 >= 0)					&&	// 방출만원매수체크
			(CashMaxOutCnt1 <= CashMaxOutCnt)		&&
			(CashMaxOutCnt2 >= 0)					&&
			(CashMaxOutCnt2 <= CashMaxOutCnt)		&&
			(MoneyMaxOut1 == GetMoneyMaxOut)		&&	// 방출금액동일체크
			(MoneyMaxOut2 == GetMoneyMaxOut));
		else
		{
			fnAPP_CancelProc(T_PROGRAM);
		}
	}

	if (WithAvail == TRAN_WITH_CHECK)
	{
		if ((CheckLowMaxOutCnt >= 0)				&&	// 방출수표매수체크
			(CheckLowMaxOutCnt <= CheckMaxOutCnt)	&&
			(CheckHighMaxOutCnt >= 0)				&&
			(CheckHighMaxOutCnt <= CheckMaxOutCnt)	&&
			(MoneyMaxOut1 == GetMoneyMaxOut)		&&	// 방출금액동일체크
			(MoneyMaxOut2 == GetMoneyMaxOut));
		else
		{
			fnAPP_CancelProc(T_PROGRAM);
		}
	}

	if ((WithAvail == TRAN_WITH_BOTH)			||
		(WithAvail == TRAN_WITH_50TCASHCHECK)	||
		(WithAvail == TRAN_WITH_CASHCHECK))
	{
		if ((CashMaxOutCnt1 >= 0)					&&	// 방출만원매수체크
			(CashMaxOutCnt1 <= CashMaxOutCnt)		&&
			(CashMaxOutCnt2 >= 0)					&&
			(CashMaxOutCnt2 <= CashMaxOutCnt)		&&
			(CheckLowMaxOutCnt >= 0)				&&	// 방출수표매수체크
			(CheckLowMaxOutCnt <= CheckMaxOutCnt)	&&
			(CheckHighMaxOutCnt >= 0)				&&
			(CheckHighMaxOutCnt <= CheckMaxOutCnt)	&&
			(MoneyMaxOut1 == GetMoneyMaxOut)		&&	// 방출금액동일체크
			(MoneyMaxOut2 == GetMoneyMaxOut));
		else
		{
			fnAPP_CancelProc(T_PROGRAM);
		}
	}

	// 수표권종에 따른 화면분기 필요 Start
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
		ScrNum = 305;
	else
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
		ScrNum = 305;											// 수표매수입력
	else
		fnAPP_CancelProc(T_INPUTOVER);
	// 수표권종에 따른 화면분기 필요 End
	m_pDevCmn->fnSCR_DisplayNumeric(1, GetMoneyMaxOut);			// 거래금액
	m_pDevCmn->fnSCR_DisplayNumeric(2, CheckLowMaxOutCnt);		// 최소매수
	m_pDevCmn->fnSCR_DisplayNumeric(3, CheckHighMaxOutCnt);		// 최대매수
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT);	
	strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(3);
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((strReturn.GetLength() != 3)		||					// 수표매수자료검증
		(!IsNum(strReturn.GetBuffer(0), 3))	||		
		(IsZero(strReturn.GetBuffer(0), 3)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	if ((Asc2Int(strReturn.GetBuffer(0), 3) < CheckLowMaxOutCnt)	|| 
		(Asc2Int(strReturn.GetBuffer(0), 3) > CheckHighMaxOutCnt))
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "입력매수가 잘못 되었습니다");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
	{
		if (!CashMaxOutCnt)							// 현금불가
		{
			memcpy(Accept.CheckMoney, "000000000000000", 15);

			// 권종별 처리분기
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
				memcpy(Accept.CheckMoney, strReturn.Right(2), 2);
			else
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
				memcpy(Accept.CheckMoney, strReturn.GetBuffer(0), 3);
			else
				fnAPP_CancelProc(T_INPUTERROR);
			// 권종별 처리분기

			memcpy(Accept.Money, Accept.CheckMoney, 15);
		}
		else 
		{
			memcpy(Accept.CheckMoney, "000000000000000", 15);

			// 권종별 처리분기
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
				memcpy(&Accept.CheckMoney[7], strReturn.Right(2), 2);
			else
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
				memcpy(&Accept.CheckMoney[7], strReturn.GetBuffer(0), 3);
			else
				fnAPP_CancelProc(T_INPUTERROR);
			// 권종별 처리분기

			SubString(Accept.Money, Accept.CheckMoney, 15, Accept.CashMoney);
		}

		if ((m_pDevCmn->Withdraw50TFlag)		&&			
			((WithAvail == TRAN_WITH_50TCASH)	||
			 (WithAvail == TRAN_WITH_50TCASHCHECK)))
		{
			memcpy(Accept.Cash50T_Money, Accept.CashMoney, sizeof(Accept.Cash50T_Money));
			memset(Accept.Cash10T_Money, '0', sizeof(Accept.Cash10T_Money));
			GetCase = 5;
			return GetCase;
		}

		if ((m_pDevCmn->Withdraw50TFlag)		&&			
			(WithAvail & TRAN_WITH_50TCASH))
		{
			if (Asc2Int(Accept.CashMoney, 11) >= CASH_FIFTY_THOUSAND)
			{
				if (m_pDevCmn->ChangeBCFlag)				
					GetCase = 7;								// 현금방출매수(자동)
				else											
					GetCase = 5;							
			}
			else
			{
				memset(Accept.Cash50T_Money, '0', sizeof(Accept.Cash50T_Money));
				memcpy(Accept.Cash10T_Money, Accept.CashMoney, sizeof(Accept.Cash10T_Money));
				GetCase = 5;
			}
		}
		else
		{
			memset(Accept.Cash50T_Money, '0', sizeof(Accept.Cash50T_Money));
			memcpy(Accept.Cash10T_Money, Accept.CashMoney, sizeof(Accept.Cash10T_Money));
			GetCase = 5;										// 금액확인
		}
	}
	return GetCase;
}


// case7			
int	CTranCmn::fnAPP_Input50TCashMoney()							
{
	int	GetCase = 9;
	int nScrNum = 0;
	int nIndex = 2;
	CString strTemp("");
	CString strReturn("");
	CStringArray strTempArray;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%8.8s] Accept.Cash50T_Money[%15.15s]", Accept.Money, Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Cash50T_Money=[%8.8s]",Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.CashMoney=[%8.8s] Accept.Cash10T_Money[%15.15s]", Accept.CashMoney, Accept.Cash10T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Cash10T_Money=[%8.8s]",Accept.Cash10T_Money);

	nScrNum = 319;		
	if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)		//총금액 삭제
	{
		// 1. 총금액
		//m_pDevCmn->fnSCR_DisplayNumeric(1, Asc2Int(Accept.Money, 4));	// 거래금액		
		// 2. 최소매수
		m_pDevCmn->fnSCR_DisplayNumeric(1, ZERO);
		// 3. 최대매수
		m_pDevCmn->fnSCR_DisplayNumeric(2, Asc2Int(Accept.CashMoney, 11)/CASH_FIFTY_THOUSAND);
	}
	else
	{
		// 1. 총금액
		//m_pDevCmn->fnSCR_DisplayNumeric(1, Asc2Int(Accept.Money, 4));	// 거래금액		
		// 2. 최소매수
		m_pDevCmn->fnSCR_DisplayNumeric(1, ZERO);
		// 3. 최대매수
		m_pDevCmn->fnSCR_DisplayNumeric(2, Asc2Int(Accept.CashMoney, 11)/CASH_FIFTY_THOUSAND);
	}

	m_pDevCmn->fnSCR_DisplayScreen(nScrNum, K_30_WAIT);	
	strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(3);

	memcpy(Accept.Cash50T_Money, "000000000000000", 15);
	memcpy(Accept.Cash10T_Money, "000000000000000", 15);

	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((strReturn.GetLength() != 3)		||					// 매수자료검증
		(!IsNum(strReturn.GetBuffer(0), 3)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney strReturn.GetBuffer=[%s]",strReturn.GetBuffer(0));
		memcpy(Accept.Cash50T_Money, Int2Asc(Asc2Int(strReturn.GetBuffer(0)) * 50000, 15), 15);			// 오만원 거래금액
		SubString(Accept.CashMoney, Accept.Cash50T_Money, 15, Accept.Cash10T_Money);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%15.15s] Accept.Cash50T_Money[%15.15s]", Accept.Money, Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney Accept.Cash50T_Money=[%8.8s]",Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.CashMoney[%15.15s] Accept.Cash10T_Money[%15.15s]", Accept.CashMoney, Accept.Cash10T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney Accept.CashMoney=[%8.8s]",Accept.CashMoney);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Cash10T_Money[%8.8s]",Accept.Cash10T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.CheckMoney[%15.15s]", Accept.CheckMoney);

		GetCase = 5;											// 금액확인	
	}

	return GetCase;
}


int	CTranCmn::fnAPP_Input50TCashMoney_HANA()							
{
	int	GetCase = 9;
	int nScrNum = 0;
	int nIndex = 2;
	CString strTemp("");
	CString strReturn("");
	CStringArray strTempArray;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%8.8s] Accept.Cash50T_Money[%15.15s]", Accept.Money, Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Cash50T_Money=[%8.8s]",Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.CashMoney=[%8.8s] Accept.Cash10T_Money[%15.15s]", Accept.CashMoney, Accept.Cash10T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Cash10T_Money=[%8.8s]",Accept.Cash10T_Money);

	nScrNum = 319;		
	if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)		//총금액 삭제
	{
		// 1. 총금액
		//m_pDevCmn->fnSCR_DisplayNumeric(1, Asc2Int(Accept.Money, 4));	// 거래금액		
		// 2. 최소매수
		m_pDevCmn->fnSCR_DisplayNumeric(1, ZERO);
		// 3. 최대매수
		m_pDevCmn->fnSCR_DisplayNumeric(2, Asc2Int(Accept.CashMoney, 11)/CASH_FIFTY_THOUSAND);
	}
	else
	{
		// 1. 총금액
		//m_pDevCmn->fnSCR_DisplayNumeric(1, Asc2Int(Accept.Money, 4));	// 거래금액		
		// 2. 최소매수
		m_pDevCmn->fnSCR_DisplayNumeric(1, ZERO);
		// 3. 최대매수
		m_pDevCmn->fnSCR_DisplayNumeric(2, Asc2Int(Accept.CashMoney, 11)/CASH_FIFTY_THOUSAND);
	}

	m_pDevCmn->fnSCR_DisplayScreen(nScrNum, K_30_WAIT);	
	strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(15);

	memcpy(Accept.Cash50T_Money, "000000000000000", 15);
	memcpy(Accept.Cash10T_Money, "000000000000000", 15);

	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((strReturn.GetLength() != 15)		||					// 매수자료검증
		(!IsNum(strReturn.GetBuffer(0), 15)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney strReturn.GetBuffer=[%s]",strReturn.GetBuffer(0));
		memcpy(Accept.Cash50T_Money, strReturn.GetBuffer(0), 15);			// 오만원 거래금액
		SubString(Accept.CashMoney, Accept.Cash50T_Money, 15, Accept.Cash10T_Money);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%15.15s] Accept.Cash50T_Money[%15.15s]", Accept.Money, Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney Accept.Cash50T_Money=[%8.8s]",Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.CashMoney[%15.15s] Accept.Cash10T_Money[%15.15s]", Accept.CashMoney, Accept.Cash10T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney Accept.CashMoney=[%8.8s]",Accept.CashMoney);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Cash10T_Money[%8.8s]",Accept.Cash10T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.CheckMoney[%15.15s]", Accept.CheckMoney);

		GetCase = 5;											// 금액확인	
	}

	return GetCase;
}

// 이체금액입력
int CTranCmn::fnAPP_AcceptTMoney()
{
	int		ScrNum = 0;
	int		GetCase = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	fnAPP_InitializeSerialTranDS();							// 연속거래변수초기화
/////////////////////////////////////////////////////////////////////////////

	if(TranCode == TC_SVCTRAN1 || TranCode == TC_SVCTRAN)   //#N0181  광주 서비스 이체금액
	{
		ScrNum = 313;											// 이체금액입력(만/천/원)
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);	}
	else
	{
		ScrNum = 303;											// 이체금액입력(만/천/원)
		if((m_pProfile->DEVICE.ETCDevice20 == HANA) && (MenuAvail & MENU_MU))
			m_pDevCmn->fnSCR_DisplayNumeric(1, 1000000);						//하나은행 무매체 이체 한도 100만원. #N0214
		else
			m_pDevCmn->fnSCR_DisplayNumeric(1, Asc2Int(OpenInfo.TransMoney, 9));

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE2);
	}
	

	strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(15);
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((strReturn.GetLength() != 15)	||						// 자료검증
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
//	else
//	if ((ScrNum == 303)								&&			// 이체금액입력(만원)
//		(!IsZero(&(strReturn.GetBuffer(0)[11]), 4))	)			// 만원단위검증
//		fnAPP_CancelProc(T_PROGRAM);
	else
	if (Asc2Int(strReturn.GetBuffer(0), 15) > fnAPP_CalcMoneyMaxTrans())
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "이체 입력금액 초과입니다");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
	{
		memcpy(Accept.Money, strReturn.GetBuffer(0), 15);
	}

	return T_OK;
}

// 입금권종선택
int CTranCmn::fnAPP_AcceptMoneyInKind()
{
	int		ScrNum = 311;										// 입금권종선택
	CString strReturn("");
	CString	szOnOffString("");

/////////////////////////////////////////////////////////////////////////////
	if( (MenuAvail & MENU_MU)	&&
		(TranCode == TC_REMIT) )
		;
	else
		fnAPP_InitializeSerialTranDS();							// 연속거래변수초기화
/////////////////////////////////////////////////////////////////////////////

	if(TranCode == TC_REMIT)									// 무매체입금은 현금만가능
	{
		Accept.MoneyInKind = TRAN_DEP_CASH;
		return T_OK;
	}

	if (m_pDevCmn->EarPhoneInserted)							//장애인ATM - 음성시 현금입금만 지원함
	{
		Accept.MoneyInKind = TRAN_DEP_CASH;							
		return T_OK; 
	}

	if ((DepAvail & TRAN_DEP_CASH) != TRAN_DEP_CASH)			// 현금입금불가
		m_pDevCmn->fnSCR_DisplayImage(1, FALSE);
	if ((DepAvail & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)			// 수표입금불가
		m_pDevCmn->fnSCR_DisplayImage(2, FALSE);
	if ((DepAvail & TRAN_DEP_BOTH) != TRAN_DEP_BOTH)			// 수표현금입금불가
		m_pDevCmn->fnSCR_DisplayImage(3, FALSE);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
																// 입금권종선택
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == "현금")									// 현금입금
		Accept.MoneyInKind = TRAN_DEP_CASH;
	else
	if (strReturn == "수표")									// 수표입금
		Accept.MoneyInKind = TRAN_DEP_CHECK;
	else
	if (strReturn == "현금+수표")								// 현금수표입금
		Accept.MoneyInKind = TRAN_DEP_BOTH;
	else
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

	return T_OK;
}

// 매출금입금 확인 
int CTranCmn::fnAPP_AcceptNhMoneyInConfirm()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		j = 1;

	m_pDevCmn->fnAPL_GetAvailDeposit();
	DepAvail = m_pDevCmn->DepAvail;								// 입금거래상태

	if( Asc2Int(Accept.Money, 15) > 1000000)						// 100만원초과금액 	// 취소입금반환처리
		fnAPP_CancelDepRejectProc(T_MSG, "거래가 취소되었습니다. ", "반환된 금액을 확인하시고, 다시거래하여주시기 바랍니다.", "무매체 1회입금한도는 100만원 입니다.");
				
	if (DepAvail == TRAN_DEP_CASH)
	{
		ScrNum = 528;
		if(Accept.InQuiryCycle == '1') //평일
		{
			m_pDevCmn->fnSCR_DisplayString(j++, "****-****-****-****"); // 계좌번호	
			m_pDevCmn->fnSCR_DisplayAmount(j++, Accept.CashMoney, 15); // 입금금액
		}
		else
		if(Accept.InQuiryCycle == '2') //휴일
		{
			m_pDevCmn->fnSCR_DisplayString(j++, "****-****-****-****"); // 계좌번호
			m_pDevCmn->fnSCR_DisplayAmount(j++, Accept.CashMoney, 15); // 입금금액
		}
		else
		{
			fnAPP_CancelDepRejectProc(T_CANCEL);					// 취소입금반환함수(정상업무처리)
		}
																												 
	}else
		fnAPP_CancelDepRejectProc(T_MSG, "[최대 입금가능 금액 초과]"," ", "카세트 금액을 확인하시기 바랍니다.");

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
																// 입금확인
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CONFIRM)
		return T_OK;
	else 
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);					// 취소입금반환함수(정상업무처리)
	else
	if (strReturn == S_TIMEOVER)
	{
		return T_OK;											// 입력시간초과시입금처리
	}
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);					// 취소입금반환함수(정상업무처리)
	else
		fnAPP_CancelDepRejectProc(T_PROGRAM);					// 취소입금반환함수(정상업무처리)

	return T_OK;
}

// 입금금액확인
int CTranCmn::fnAPP_AcceptMoneyInConfirm()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		j = 1;

	if( (Asc2Int(Accept.Money, 15) > 1000000)	&&							// 100만원초과금액
		((MenuAvail & MENU_MU) && (TranCode == TC_REMIT)) )					// 무매체 입금시 반환처리
		fnAPP_CancelDepRejectProc(T_MSG, "거래가 취소되었습니다. ", 
										"반환된 금액을 확인하시고, 다시거래하여주시기 바랍니다.",
										"무매체 1회입금한도는 100만원 입니다.");
																			// 취소입금반환처리
	ScrNum = 519;		

	if (PbTran & MENU_PB)
	{
		if(!IsZero(&Accept.AccountNum, 14))
			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsSpace(&Accept.AccountNum, 14))
			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
	}
	else
	if (MenuAvail & MENU_MU)  
	{
		if(!IsZero(&Accept.MuAccountNum, 14))
			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsSpace(&Accept.MuAccountNum, 14))
			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
	}
	else
	{
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsSpace(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
	}
	
	m_pDevCmn->fnSCR_DisplayString(j++, m_RD.byDataField_017x);	// 계좌번호		
	m_pDevCmn->fnSCR_DisplayAmount(j++, Accept.CashMoney, 15);	// 현금입금금액

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE1);
																// 입금확인
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CONFIRM)
		return T_OK;
	else 
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);					// 취소입금반환함수(정상업무처리)
	else
	if (strReturn == S_TIMEOVER)
	{
		return T_OK;											// 입력시간초과시입금처리
	}
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);					// 취소입금반환함수(정상업무처리)
	else
		fnAPP_CancelDepRejectProc(T_PROGRAM);					// 취소입금반환함수(정상업무처리)

	return T_OK;
}

// 송금인명입력
int CTranCmn::fnAPP_AcceptRemitName()
{
	CString strReturn("");
	BYTE	StrBuff[81] = {0, };
	BYTE	strTemp[81] = {0, };

	int		ScrNum = 0;

	int				i = 0;
	unsigned int	j = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.RemitName, ' ', sizeof(Accept.RemitName));	// 송금인명초기화
	Accept.RemitName[0] = 'N';
/////////////////////////////////////////////////////////////////////////////

	Accept.RemitNameFlag = TRUE;   //#N0201

	ScrNum = 431;
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_90_WAIT, PIN_MENU_MODE); //#0117
	strReturn = m_pDevCmn->fnSCR_GETKEYHANGUL(ScrNum , 1, StrBuff, 14, K_90_WAIT, 0, 0); // 입력은 14 Bytes로 제한
	
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelDepRejectProc(T_TIMEOVER);	
	else 
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);
	else
	{
		Accept.RemitNameFlag = TRUE;
		Accept.RemitName[0] = 'Y';
		memcpy(&Accept.RemitName[1], StrBuff, __min(strlen((char *)StrBuff), 14));  //#N0216
	}

	return T_OK;
}

// 차액송금금액입력
int CTranCmn::fnAPP_AcceptRMoney()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.RemainRemMoney, '0', sizeof(Accept.RemainRemMoney));	
																// 차액송금액
	memset(Accept.BalanceRemMoney, '0', sizeof(Accept.BalanceRemMoney));
																// 차액입금액
/////////////////////////////////////////////////////////////////////////////

	ScrNum = 304;												// 차액송금금액입력
	m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.Money, 15);
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_AMOUNT_MODE);
																// 금액입력(만천원)
	strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(15);
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);					// 취소입금반환함수(정상업무처리)
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelDepRejectProc(T_TIMEOVER);					// 취소입금반환함수(정상업무처리)
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);					// 취소입금반환함수(정상업무처리)
	else
	if ((strReturn.GetLength() != 15)		||					// 금액자료검증
		(!IsNum(strReturn.GetBuffer(0), 15))	||		
		(IsZero(strReturn.GetBuffer(0), 15)))
		fnAPP_CancelDepRejectProc(T_INPUTERROR);
	else
	if (Asc2Int(strReturn.GetBuffer(0), 15) > Asc2Int(Accept.Money,15))
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelDepRejectProc(T_MSG, "차액송금 입력금액 초과입니다");
		else
			fnAPP_CancelDepRejectProc(T_CANCEL);
	}
	else
	{
		memcpy(Accept.RemainRemMoney, strReturn.GetBuffer(0), 15);
		SubString(Accept.Money, Accept.RemainRemMoney, 15, Accept.BalanceRemMoney);
	}

	return T_OK;
}

// 송금확인
int CTranCmn::fnAPP_AcceptRemitConfirm()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		j = 1;

	if (Accept.RemitNameFlag)
			ScrNum = 514;
	else
			ScrNum = 513;

	if (Accept.RemitNameFlag && ScrNum == 514)
		m_pDevCmn->fnSCR_DisplayString(j++, GetString(Accept.RemitName, 20, 20));		// 송금인

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CONFIRM)
		return T_OK;
	else 
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);					// 취소입금반환함수(정상업무처리)
	else
	if (strReturn == S_TIMEOVER)
	{
		return T_OK;											// 입력시간초과시입금처리
	}
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);					// 취소입금반환함수(정상업무처리)
	else
		fnAPP_CancelDepRejectProc(T_PROGRAM);					// 취소입금반환함수(정상업무처리)

	return T_OK;
}

// 은행번호입력
int CTranCmn::fnAPP_AcceptBankNum()
{
	CString strReturn("");
	int		GetCase = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.BankNum, '0', sizeof(Accept.BankNum));		// 은행번호
/////////////////////////////////////////////////////////////////////////////

		
	switch(m_pProfile->DEVICE.MachineType)
	{
	case U3100K:
		GetCase = 4;
		break;
	case U8100:
		if ((m_pDevCmn->EarPhoneInserted)	||					// 장애인ATM
			(m_pDevCmn->SightHandiCapped))
			GetCase = 0;
		else
		{
			if(m_pProfile->DEVICE.ETCDevice20 == HANA)			
				GetCase = 7;
			else
				GetCase = 4;
		}
		break;
	default:
		break;
	}


	/* 
	if(CardTran & MENU_T3)
	{
		m_pDevCmn->fnSCR_DisplayImage(1, FALSE);
	}
	*/

	while (GetCase <= 7)			//#N0208
	{
		switch (GetCase)
		{
			case 0:												// 은행선택
			m_pDevCmn->fnSCR_DisplayScreen(402, K_30_WAIT, PIN_MENU_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyString();
				GetCase = 9;									// 종료
				if (strReturn == S_CANCEL)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_CANCEL2)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_TIMEOVER)
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
				if (strReturn == "기타")
					GetCase = 1;
				else
				if ((strReturn.GetLength() != 3)	||			// 자료검증
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// 종료
				}
				break;

			case 1:
				m_pDevCmn->fnSCR_DisplayScreen(403, K_30_WAIT, PIN_MENU_MODE); 
				strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(3);
				if (strReturn == S_CANCEL)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_CANCEL2)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_TIMEOVER)
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
				if ((strReturn.GetLength() != 3)	||			// 자료검증
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				if (IsZero(strReturn.GetBuffer(0), strReturn.GetLength()))
					GetCase = 3;								// 증권사번호 입력
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// 종료
				}
				break;
 
			case 2:												// 은행번호입력, // 증권번호입력
				m_pDevCmn->fnSCR_DisplayScreen(401, K_30_WAIT, PIN_NUMERIC_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(3);
				if (strReturn == S_CANCEL)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_CANCEL2)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_TIMEOVER)
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
				if ((strReturn.GetLength() != 3)	||			// 자료검증
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// 종료
				}
				break;
			case 3:												// 증권번호입력
				m_pDevCmn->fnSCR_DisplayScreen(409, K_30_WAIT, PIN_NUMERIC_MODE);   //증권사 은행코드 입력화면 변경
				strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(3);
				if (strReturn == S_CANCEL)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_CANCEL2)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_TIMEOVER)
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
				if ((strReturn.GetLength() != 3)	||			// 자료검증
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// 종료
				}
				break;
			case 4:												// #N0208,은행/증권사 선택
				m_pDevCmn->fnSCR_DisplayScreen(401, K_30_WAIT, PIN_NUMERIC_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyString();
				if (strReturn == S_CANCEL)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_CANCEL2)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_TIMEOVER)
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
				if (strReturn == "은행")				//#N0208
					GetCase = 5;
				else
				if (strReturn == "증권사")
					GetCase = 6;
				else
					fnAPP_CancelProc(T_PROGRAM);
				break;
			case 5:												// 은행번호입력#N0208
				m_pDevCmn->fnSCR_DisplayScreen(415, K_30_WAIT, PIN_NUMERIC_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(3);
				if (strReturn == S_CANCEL)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_CANCEL2)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_TIMEOVER)
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
				if ((strReturn.GetLength() != 3)	||			// 자료검증
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// 종료
				}
				break;
			case 6:												// 증권번호입력//#N0208
				m_pDevCmn->fnSCR_DisplayScreen(416, K_30_WAIT, PIN_NUMERIC_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(3);
				if (strReturn == S_CANCEL)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_CANCEL2)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_TIMEOVER)
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
				if ((strReturn.GetLength() != 3)	||			// 자료검증
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// 종료
				}
				break;
			case 7:												// #N0225
				m_pDevCmn->fnSCR_DisplayScreen(410, K_30_WAIT, PIN_NUMERIC_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(3);
				if (strReturn == "기타은행")
					GetCase = 5;
				else
				if (strReturn == "증권사")
					GetCase = 6;
				else
				if (strReturn == S_CANCEL)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_CANCEL2)
					fnAPP_CancelProc(T_CANCEL);
				else
				if (strReturn == S_TIMEOVER)
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
				if ((strReturn.GetLength() != 3)	||			// 자료검증
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// 종료
				}
				break;
			default: 
				break;
		}
	}

	return T_OK;
}

// 수취계좌번호입력
int CTranCmn::fnAPP_AcceptInAccountNum()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.InAccountNumSize = 0;								// 수취계좌번호길이
	memset(Accept.InAccountNum, ' ', sizeof(Accept.InAccountNum));	
																// 수취계좌번호
/////////////////////////////////////////////////////////////////////////////

	ScrNum = 405;											// 타행이체

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_45_WAIT, PIN_NUMERIC_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelDepRejectProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.InAccountNum))	||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelDepRejectProc(T_INPUTERROR);
	else
	{
		Accept.InAccountNumSize = strReturn.GetLength();
		memcpy(Accept.InAccountNum, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// 청약수량입력
int CTranCmn::fnAPP_AcceptInTransReqStock()
{
	int		ScrNum = 0, nIndex = 1;
	CString strReturn("");

	ScrNum = 922;												// 청약수량입력


	return T_OK;
}

// 청약환불금고계좌입력
int CTranCmn::fnAPP_AcceptInTransReqStockAccount()
{
	int		ScrNum = 0, nIndex = 1;
	CString strReturn("");

	return T_OK;
}

// 유가증권대체출고입력
int CTranCmn::fnAPP_AcceptInTransStock()
{
	int		ScrNum = 0, nIndex = 1;
	CString strReturn("");

	return T_OK;
}

// 모집인번호입력
int CTranCmn::fnAPP_AcceptCmsCode()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.CMSCodeSize = 0;										// CMS길이
	memset(Accept.CMSCode, ' ', sizeof(Accept.CMSCode));		// CMS(직금/고유번호)
/////////////////////////////////////////////////////////////////////////////

	return T_OK;
}

// 비밀번호변경입력
int CTranCmn::fnAPP_AcceptChangePassword()
{
	CString strReturn("");
	int		ScrNum = 0;
	int		RetryCnt = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.ChangePwd, '0', sizeof(Accept.ChangePwd));	// 비밀번호(변경)
/////////////////////////////////////////////////////////////////////////////

		ScrNum = 210;

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE, "", 4, 4, FALSE, "CANCEL");
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else													// 자료검증
	if ((strReturn.GetLength() != sizeof(Accept.ChangePwd))		||	
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.ChangePwd, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	ScrNum = 215;

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE, "", 4, 4, FALSE, "CANCEL");
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else													// 자료검증
	if ((strReturn.GetLength() != sizeof(Accept.ChangePwd))		||	
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.ChangePwdConfirm, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// 대출계좌번호입력
int	CTranCmn::fnAPP_AcceptLoanAccountNum()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.LoanAccountNumSize = 0;								// 대출계좌번호길이
	memset(Accept.LoanAccountNum, 0x20, sizeof(Accept.LoanAccountNum));
																// 대출계좌번호 2004.09.15 초기화SPACE
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(403, K_30_WAIT, PIN_NUMERIC_MODE);
																// 계좌번호입력
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);		
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.LoanAccountNum))	||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		Accept.LoanAccountNumSize = strReturn.GetLength();
		memcpy(Accept.LoanAccountNum, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}


// 전화번호입력
int CTranCmn::fnAPP_AcceptTelNum()
{
	CString strReturn("");
	int		ScrNum = 0;
	
/////////////////////////////////////////////////////////////////////////////
	Accept.TelNumSize = 0;										// 전화번호길이
	memset(Accept.TelNum, ' ', sizeof(Accept.TelNum));			// 전화번호
/////////////////////////////////////////////////////////////////////////////

	if (TranCode == TC_DEPOSIT)
		ScrNum = 413;
	else
		ScrNum = 413;

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);
																// 전화번호입력
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else 
	if (strReturn == "입력생략")  //한네트 사양임.(요청사항)-2013.03.04 //#0111
	{
		Accept.TelNumSize = 12;
		memset(Accept.TelNum, ' ', sizeof(Accept.TelNum));			
	}
	else 
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.TelNum))			||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		Accept.TelNumSize = strReturn.GetLength();
		memcpy(Accept.TelNum, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// 주민번호입력
int CTranCmn::fnAPP_AcceptJuminNum()
{
	CString strReturn("");
	int		ScrNum = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.JuminNum, ' ', sizeof(Accept.JuminNum));		// 주민번호
/////////////////////////////////////////////////////////////////////////////


	ScrNum = 411;

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);
																// 주민번호입력
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else 
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.JuminNum))		||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.JuminNum, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// 이체주기,이체희망일
int	CTranCmn::fnAPP_AcceptAutoDetail()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.AutoDate, ' ', sizeof(Accept.AutoDate));		// 자동이체일
	memset(Accept.AutoCycle, ' ', sizeof(Accept.AutoCycle));	// 이체주기
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(418, K_30_WAIT, PIN_NUMERIC_MODE);
																// 이체주기
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else 
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if (strReturn == "1개월")
		memcpy(Accept.AutoCycle, "01", 2);
	else
	if (strReturn == "3개월")
		memcpy(Accept.AutoCycle, "03", 2);
	else
		fnAPP_CancelProc(T_INPUTERROR);

	m_pDevCmn->fnSCR_DisplayScreen(419, K_30_WAIT, PIN_NUMERIC_MODE);
																// 이체희망일
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else 
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if (strReturn == "말일")
		memcpy(Accept.AutoDate, "32", 2);
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.AutoDate))		||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.AutoDate, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// 자동이체설정확인
int CTranCmn::fnAPP_AcceptAutoConfirm()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		j = 1;

	ScrNum = 515;

	m_pDevCmn->fnSCR_DisplayString(j++, fchpAPP_EditDestAcnt(Accept.InAccountNum, 12, 14, FALSE));
																// 설정계좌번호
	m_pDevCmn->fnSCR_DisplayAmount(j++, (LPCTSTR)Accept.Money, 15);
																// 설정금액
	if (memcmp(Accept.AutoDate, "32", 2) == 0)					// 이체희망일
		m_pDevCmn->fnSCR_DisplayString(j++, "말");
	else
		m_pDevCmn->fnSCR_DisplayString(j++, (LPCTSTR)Accept.AutoDate, 2);
	m_pDevCmn->fnSCR_DisplayString(j++, (LPCTSTR)Accept.AutoCycle, 2);
																// 이체주기

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn = S_CONFIRM)
		return T_OK;
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

int CTranCmn::fnAPP_AcceptInqDateEnd()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		j = 1;

	if(TranCode == TC_PBOOKTRM)
		ScrNum = 424;			// 통장정리 시작날짜 입력
	else //if(TranCode == TC_INQUIRYA || TranCode == TC_INQSTOCK)
		ScrNum = 421;			// 거래내역조회 시작날짜입력

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayString(1, m_pProfile->TRANS.YYYYMMDD, 8);
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.InquiryDateEnd))	||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.InquiryDate, m_pProfile->TRANS.YYYYMMDD, 8);
		memcpy(Accept.InquiryDateEnd, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}


// 이체 시 전화사기 안내문구 추가
int	CTranCmn::fnAPP_AcceptTranChoice()
{
	CString strReturn("");
	int		ScrNum = 0;


	ScrNum = 430;	
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT, PIN_MENU_MODE1);		
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if(strReturn == S_CONFIRM)
		;
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
		fnAPP_CancelDepRejectProc(T_PROGRAM);
	

	return T_OK;
}

// 명세표인자여부선택
int	CTranCmn::fnAPP_AcceptSlipChoice()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.SlipChoiceFlag = TRUE;								// 명세표인자여부
/////////////////////////////////////////////////////////////////////////////

	if ((!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) || (m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_SPR)))
	{
		m_pDevCmn->fnSCR_DisplayScreen(526);
		Delay_Msg(2000);
		return T_OK;
	}
	else
	if ((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL) || (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
	{
		m_pDevCmn->fnSCR_DisplayScreen(526);
		Delay_Msg(2000);
		return T_OK;
	}

	m_pDevCmn->fnSCR_DisplayScreen(525, K_30_WAIT, PIN_DISABLE_MODE);
																// 명세표인자여부선택
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_YES)										//#N0196
		Accept.SlipChoiceFlag = TRUE;
	else
	if ((strReturn == S_NO) 	||								//#N0196
		(strReturn == S_TIMEOVER))
		Accept.SlipChoiceFlag = FALSE;
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);
	else
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	return T_OK;
}

// K-CASH비밀번호입력
int CTranCmn::fnAPP_AcceptKCashPassword(int nReadDF, int ScrNum)
{
	CString strReturn("");
	int			nCnt = 0;	
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.KCashPassWord, 0, sizeof(Accept.KCashPassWord));
																// K-CASH비밀번호
/////////////////////////////////////////////////////////////////////////////
	while (TRUE)
	{

		if(nCnt > 2) fnAPP_CancelProc(T_INPUTOVER);

		if (!ScrNum)
		{
			if (nReadDF == TRAN_IC_KCASH)
			{
				if (TranCode == TC_KCPASSRE)
				{
					ScrNum = 206;
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE2, "", 4, 4);
				}
				else
				{
					ScrNum = 205;
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE2, "", 4, 8);
				}
			}
			else
			if (nReadDF == TRAN_IC_FINANCEIC)
			{
				ScrNum = 203;
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE2, "", 6, 6);
			}
			else
				fnAPP_CancelProc(T_PROGRAM);
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE2, "", 6, 6);
		}

		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);		
		else
		if (strReturn == S_CLEAR)
		{
			nCnt++;
			continue;
		}
		else
		if ((strReturn.GetLength() < 4)	||		// 자료검증
			(strReturn.GetLength() > sizeof(Accept.KCashPassWord))	||
			(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
			(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		{
			nCnt++;
			continue;
		}
		else
		{
			break;
		}

	}

	memcpy(Accept.KCashPassWord, strReturn.GetBuffer(0), strReturn.GetLength());

	return T_OK;
}

// K-CASH금액입력
int CTranCmn::fnAPP_AcceptKCashMoney()
{
	int		ScrNum = 0;
	CString strReturn("");
	CString	strCardNum("");

/////////////////////////////////////////////////////////////////////////////
	fnAPP_InitializeSerialTranDS();								// 연속거래변수초기화
/////////////////////////////////////////////////////////////////////////////

	strCardNum = MakeUnPack(m_pDevCmn->KCashInfo.CardNum, 5);

	if (TranCode == TC_KCLOAD)									// K-CASH충전
	{
		ScrNum = 317;											// K-CASH충전금액입력
		m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
																// 잔액
		m_pDevCmn->fnSCR_DisplayNumeric(2, ByteHighOrder2Int(m_pDevCmn->KCashInfo.MaxLimitAmount, 4));

		if (ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4) == ByteHighOrder2Int(m_pDevCmn->KCashInfo.MaxLimitAmount, 4))
		{
			m_pDevCmn->fnSCR_DisplayString(2, "한도금액만큼 충전되어있어");
			m_pDevCmn->fnSCR_DisplayString(3, "더 이상 충전하실 수 없습니다.");
			m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("충전 한도금액:%d원", ByteHighOrder2Int(m_pDevCmn->KCashInfo.MaxLimitAmount, 4)));
			m_pDevCmn->fnSCR_DisplayScreen(701);				// 충전거래 불가안내
			Delay_Msg(3000);
			fnAPP_CancelProc(T_CANCEL);
		}
	}
	else
	if (TranCode == TC_KCUNLOAD)								// K-CASH환불
	{
		ScrNum = 318;											// K-CASH환불금액입력
		m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
																// 환불가능액
		m_pDevCmn->fnSCR_DisplayNumeric(2, ByteHighOrder2Int(m_pDevCmn->KCashInfo.MaxLimitAmount, 4));

		if (!ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4))// 잔액이 0인경우
		{
			fnAPP_CancelProc(T_MSG, "환불하실 잔액이 없습니다.", "환불 가능금액 : 0원");
		}
	}

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_AMOUNT_MODE);// 금액입력
	strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(8);
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((strReturn.GetLength() != 8)		||					// 금액자료검증
		(!IsNum(strReturn.GetBuffer(0), 8))	||		
		(IsZero(strReturn.GetBuffer(0), 8)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	if ((TranCode == TC_KCLOAD)	&&								// K-CASH충전
		((Asc2Int(strReturn.GetBuffer(0), 8)) > 
				(ByteHighOrder2Int(m_pDevCmn->KCashInfo.MaxLimitAmount, 4) - ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4))))
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "충전 가능금액 초과입니다");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
	if ((TranCode == TC_KCUNLOAD) &&							// K-CASH환불
		((Asc2Int(strReturn.GetBuffer(0), 8)) > (ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4))))
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			{
				fnAPP_CancelProc(T_MSG, "환불금액 초과입니다");
			}
			else
				fnAPP_CancelProc(T_CANCEL);
		}
	else
	{
		memcpy(Accept.Money, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// K-CASH비밀번호변경입력
// V02-06-01-#9 IC-PIN변경 지원 : 다른 DF들도 적용가능토록 수정
int CTranCmn::fnAPP_AcceptKCashChangePassword(int nReadDF)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptKCashChangePassword(%d)", nReadDF);

	CString	strReturn("");
	int		RetryCnt = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.ChangeICPwd, 0, sizeof(Accept.ChangeICPwd));
																// K-CASH비밀번호(변경)
	memset(Accept.ChangeICPwdConfirm, 0, sizeof(Accept.ChangeICPwdConfirm));
																// K-CASH비밀번호(변경확인)
/////////////////////////////////////////////////////////////////////////////

	RetryCnt = 3;												// 입력오버체크3회
	while (RetryCnt)
	{
		memset(Accept.ChangeICPwd, 0, sizeof(Accept.ChangeICPwd));
																// K-CASH비밀번호(변경)

		if (nReadDF == TRAN_IC_KCASH)
			m_pDevCmn->fnSCR_DisplayScreen(207, K_30_WAIT, PIN_PASSWORD_MODE, "", 6, 8);
		else
			fnAPP_CancelProc(T_PROGRAM);

		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
		else
		if ((strReturn.GetLength() < 6)								||	// 자료검증
			(strReturn.GetLength() > sizeof(Accept.ChangeICPwd))	||
			(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
			(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
			fnAPP_CancelProc(T_INPUTERROR);
		else
		{
			memcpy(Accept.ChangeICPwd, strReturn.GetBuffer(0), strReturn.GetLength());
		}

		memset(Accept.ChangeICPwdConfirm, 0, sizeof(Accept.ChangeICPwdConfirm));
																// K-CASH비밀번호(변경확인)

		if (nReadDF == TRAN_IC_KCASH)
			m_pDevCmn->fnSCR_DisplayScreen(208, K_30_WAIT, PIN_PASSWORD_MODE, "", 6, 8);
		else
			fnAPP_CancelProc(T_PROGRAM);

		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
		else
		if ((strReturn.GetLength() < 6)									||	// 자료검증 :길이수정(4->6) 2004.02.27
			(strReturn.GetLength() > sizeof(Accept.ChangeICPwdConfirm))	||
			(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))		||
			(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
			fnAPP_CancelProc(T_INPUTERROR);
		else
		{
			memcpy(Accept.ChangeICPwdConfirm, strReturn.GetBuffer(0), strReturn.GetLength());
		}

		// 두번 입력받은 비밀번호를 비교하여 같으면 변경하고
		// 다르면 3번까지 재입력을 받는다.
		if (memcmp(Accept.ChangeICPwd, Accept.ChangeICPwdConfirm, 8) == 0)
		{
			if (fnAPP_KCashCheckChangePin() == T_OK)			// 동일/연속숫자확인	2004.02.27
				break;											// 동일/연속이 아닌경우만 유효처리

			if (!--RetryCnt)									// 취소/재입력화면처리  2004.02.27
			{
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "비밀번호 입력횟수가 초과되었습니다");
				else
					fnAPP_CancelProc(T_CANCEL);
			}
			else
			{
				m_pDevCmn->fnSCR_DisplayString(1, "PIN 입력오류");
				m_pDevCmn->fnSCR_DisplayString(2, "동일한 번호나 연속된 번호는");
				m_pDevCmn->fnSCR_DisplayString(4, "PIN으로 사용하실 수 없습니다");
				m_pDevCmn->fnSCR_DisplayString(5, "다시 입력하여 주십시오");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				Delay_Msg(3 * 1000);
			}
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayString(2, "변경하신 비밀번호가 일치하지 않습니다.");
			m_pDevCmn->fnSCR_DisplayString(3, "변경하실 전자화폐 비밀번호를");
			m_pDevCmn->fnSCR_DisplayString(4, "처음부터 다시 입력하여 주십시오");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(2 * 1000);
			if (!--RetryCnt)
			{
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "비밀번호 입력횟수가 초과되었습니다");
				else
					fnAPP_CancelProc(T_CANCEL);
			}
		}
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptKCashChangePassword():return");
	return T_OK;
}

// 금융IC 계좌선택
int CTranCmn::fnAPP_FICSelectAccount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FICSelectAccount()");

	if (!MenuFICMode)											// 금융IC거래확인
		return T_ERROR;

	if (m_pDevCmn->FICAccountCount > 10)					
		return T_ERROR;
//		fnAPP_CancelProc(T_MSG, "금융IC 카드내의", "계좌번호읽기중 오류가 발생하였습니다", " [상황실: 1577 - 4655] 문의하여 주십시오.");
	if (m_pDevCmn->FICAccountCount <= 0)
		return T_ERROR;
//		fnAPP_CancelProc(T_MSG, "금융IC 카드내에", "사용가능한 계좌정보가 없습니다", " [상황실: 1577 - 4655] 문의하여 주십시오.");

	CString strTemp("");
	CString GetKeyStr("");
	int		ScrNum = 407;										// 화면
	int		ScrOffset = 0;										// 5계좌출력하는 화면Offset(0 or 5)
	CString	strBankCode("");									// 발급은행코드

	Accept.FICSelectAccountIndex = 0;							// 첫계좌	
	strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Mid(1));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FICSelectAccount() strBankCode[%s]", strBankCode);

/*
//#0023 Kim.G.J ==> "#0034 (현대증권, 우체국 : 이체 IC)
	if( (strBankCode.CompareNoCase(NONBANKID1) == 0)    ||
		(strBankCode.CompareNoCase(NONBANKID2) == 0)	||
		(strBankCode.CompareNoCase(NONBANKID3) == 0)	||
		(strBankCode.CompareNoCase(NONBANKID4) == 0)    ||
		(strBankCode.CompareNoCase(WORBANKID1) == 0)	||
		(strBankCode.CompareNoCase(WORBANKID2) == 0)    ||
		(strBankCode.CompareNoCase(WORBANKID3) == 0)	||
		(strBankCode.CompareNoCase(WORBANKID4) == 0)    ||
		(strBankCode.CompareNoCase(HANBANKID1) == 0)	||
		(strBankCode.CompareNoCase(HANBANKID2) == 0)    ||
		(strBankCode.CompareNoCase(HANBANKID3) == 0)	||
		(strBankCode.CompareNoCase(HANBANKID4) == 0)    ||
		(strBankCode.CompareNoCase(KIUBANKID) == 0)     ||
		(strBankCode.CompareNoCase(SUHBANKID) == 0)     ||
		(strBankCode.CompareNoCase(KOEBANKID) == 0)     ||
	    (strBankCode.CompareNoCase(JEJBANKID) == 0)		||
	    (strBankCode.CompareNoCase(PUSBANKID) == 0)		||
	    (strBankCode.CompareNoCase(KFCBANKID) == 0)		||
	    (strBankCode.CompareNoCase(KUMBANKID1) == 0)	||
	    (strBankCode.CompareNoCase(KUMBANKID2) == 0)	||
	    (strBankCode.CompareNoCase(KUMBANKID3) == 0)	||
	    (strBankCode.CompareNoCase(KUMBANKID4) == 0)	||
	    (strBankCode.CompareNoCase(KUMBANKID5) == 0)	||
	    (strBankCode.CompareNoCase(KDBBANKID) == 0)		||
	    (strBankCode.CompareNoCase(HSBABNKID) == 0)		||
	    (strBankCode.CompareNoCase(CHOBANKID) == 0)	    ||
	    (strBankCode.CompareNoCase(SHIBANKID) == 0)	    ||
	    (strBankCode.CompareNoCase(SHIBANKID1) == 0)	||
// 2010.12.20
//		(strBankCode.CompareNoCase(POSBANKID1) == 0)	||
//	    (strBankCode.CompareNoCase(POSBANKID2) == 0)	||
//	    (strBankCode.CompareNoCase(POSBANKID3) == 0)	||
//	    (strBankCode.CompareNoCase(POSBANKID4) == 0)	||
//	    (strBankCode.CompareNoCase(POSBANKID5) == 0)	||

		(strBankCode.CompareNoCase(SANBANKID) == 0)     ||
		(strBankCode.CompareNoCase(GOOBANKID1) == 0)	||
	    (strBankCode.CompareNoCase(SASSBANKID) == 0)    ||
	    (strBankCode.CompareNoCase(KOFBANKID) == 0)     ||
		(strBankCode.CompareNoCase(TYJSBANKID) == 0)    ||
	    (strBankCode.CompareNoCase(DAWSBANKID) == 0)    ||
	    (strBankCode.CompareNoCase(MRASBANKID) == 0)    ||
		(strBankCode.CompareNoCase(HKTSBANKID) == 0)    ||
		(strBankCode.CompareNoCase(GOMSBANKID) == 0)    ||
		(strBankCode.CompareNoCase(HNDSBANKID) == 0)    ||
		(strBankCode.CompareNoCase(WRTSBANKID) == 0)    ||
		(strBankCode.CompareNoCase(SINBANKID) == 0)     ||
		(strBankCode.CompareNoCase(KAWBANKID) == 0)		||
	    (strBankCode.CompareNoCase(HAHSBANKID) == 0)	||
		(strBankCode.CompareNoCase(DASSBANKID) == 0)    ||
		(strBankCode.CompareNoCase(KYOBANKID) == 0)     ||
		(strBankCode.CompareNoCase(DEABANKID) == 0)     ||
		(strBankCode.CompareNoCase(JEOBANKID) == 0)     || //#0043 Kim.Gi.Jin
		(strBankCode.CompareNoCase(HYDSBANKID) == 0)
		);
	else
	if(	(strBankCode.CompareNoCase(POSBANKID1) == 0)	||
	    (strBankCode.CompareNoCase(POSBANKID2) == 0)	||
	    (strBankCode.CompareNoCase(POSBANKID3) == 0)	||
	    (strBankCode.CompareNoCase(POSBANKID4) == 0)	||
	    (strBankCode.CompareNoCase(POSBANKID5) == 0))
	{
//		if(TranCode2 == TRANID_1020)
//			;
//		else
			return T_ERROR;
			
	}	
	else return T_ERROR;
*/
		

	if (m_pDevCmn->FICAccountCount == 1)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FICSelectAccount() Select AccIndex[%d]", Accept.FICSelectAccountIndex);
		return T_OK;											// Accept.FICSelectAccountIndex = 0;
	}

	while(true)
	{
		for (int Index = 0 ; Index < 5 ; Index++ )
		{
			// 라인번호 출력
			if (m_pDevCmn->EarPhoneInserted == TRUE)		// 장애인ATM
			{
				// 0번 계좌 출력으로 인하여 조건문 추가
				if (Asc2Int(m_pDevCmn->FinanceICDisp[ScrOffset + Index].Num, sizeof(m_pDevCmn->FinanceICDisp[ScrOffset + Index].Num)) != 0)
				{
					strTemp.Format("%d번, ",Index+1);
					m_pDevCmn->fnSCR_DisplayString(Index*2+1, strTemp.GetBuffer(0));
				}
			}
			else
				m_pDevCmn->fnSCR_DisplayString(Index*2+1, m_pDevCmn->FinanceICDisp[ScrOffset + Index].Num);
			
			// 계좌 출력
			strTemp = GetString(m_pDevCmn->FinanceICDisp[ScrOffset + Index].AccNum, 
								m_pDevCmn->FinanceICISO3Info[ScrOffset + Index].AccountNOCnt, 
								m_pDevCmn->FinanceICISO3Info[ScrOffset + Index].AccountNOCnt);

			CString strAccountNum("");
			if (m_pDevCmn->EarPhoneInserted == TRUE)		// 장애인ATM
			{
				strAccountNum.Format("<digit>%s</digit>", strTemp);
				m_pDevCmn->fnSCR_DisplayString(Index*2+2, strAccountNum);
			}
			else
				m_pDevCmn->fnSCR_DisplayString(Index*2+2, strTemp.GetBuffer(0));

		}

		if (ScrOffset == 0)
		{
			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);		    // "이전"불가

			if ((m_pDevCmn->EarPhoneInserted == TRUE) &&		// 장애인ATM
				(m_pDevCmn->FICAccountCount > 5) )				// 장애인ATM
				m_pDevCmn->fnSCR_DisplayString(21, "다음 계좌를 확인 하시려면 7번 버튼을");
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayImage(2, FALSE);		// "다음"불가

			if (m_pDevCmn->EarPhoneInserted == TRUE)				// 장애인ATM
				m_pDevCmn->fnSCR_DisplayString(21, "이전 계좌를 확인 하시면 6번 버튼을");
		}

		
		if (m_pDevCmn->FICAccountCount <= 5)				// 계좌갯수가 한화면이하인경우
		{
			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);		// "이전"불가
			m_pDevCmn->fnSCR_DisplayImage(2, FALSE);		// "다음"불가
		}

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_INPUT_MODE);
		GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();

		if (GetKeyStr == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
		else
		if (GetKeyStr == S_CANCEL2)								// #장애인ATM 영문 CANCEL 수용
			fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
		else 
		if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (GetKeyStr == S_INPUTOVER)							// INPUTOVER
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (GetKeyStr == S_NEXT)								// 다음
			ScrOffset = 5;
		else
		if (GetKeyStr == S_PREV)								// 이전
			ScrOffset = 0;
		else
		{
			//Accept.FICSelectAccountIndex = Asc2Int(GetKeyStr.GetBuffer(0), 1) - 1; //#N0133
			Accept.FICSelectAccountIndex = ScrOffset + (Asc2Int(GetKeyStr.GetBuffer(0), 1) - 1);
			break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FICSelectAccount() Select AccIndex[%d]", Accept.FICSelectAccountIndex);
	return T_OK;
}

// DES비밀번호입력
// DES OutGoing거래 2004.05.31  -> #N0176
int CTranCmn::fnAPP_AcceptDesPassword()
{
	CString strReturn("");
	CString	strDesPin("");
	char szUnPack[512];
	char szPack[512];
	char szUnCardNum[512];
	char szMemberNum[512];
	int	nCnt = 0;
	int	nEncMode = 0;	
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.DesPassWord, 0, sizeof(Accept.DesPassWord));	// K-CASH비밀번호
	memset(Accept.DesEncryptPin, 0, sizeof(Accept.DesEncryptPin));
	memset(szUnCardNum, 0, sizeof(szUnCardNum));
	memset(szMemberNum, 0, sizeof(szMemberNum));
	memset(szUnPack, 0, sizeof(szUnPack));
	memset(szPack, 0, sizeof(szPack));
																// Des Encrypt PIN	// DES OutGoing거래 2004.05.31
/////////////////////////////////////////////////////////////////////////////


	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_AcceptDesPassword()");		
	//#N0174
	if (m_pDevCmn->DesITKStatus || m_pDevCmn->DesTMKStatus)
	{
		sprintf(szUnPack,"%16.16s",	IniGetStr(_ATMKEYTABELE_INI, "ENCDATA","VALUE", "B6140B636BBE7A86")); //check sum = 19BD1D
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "MASTE KEY -> szUnPack", szUnPack, strlen(szUnPack));
		MakePack(szUnPack, szPack, 16);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "MASTE KEY -> szPack", szUnPack, strlen(szUnPack));
		m_pDevCmn->fnDES_LoadKey(szUnPack, 2);
	}


	while (TRUE)
	{
		if(nCnt > 2) fnAPP_CancelProc(T_INPUTOVER);

		switch(m_pProfile->DEVICE.MachineType)
		{
		case U3100K:
			sprintf(szUnCardNum, "%s",fnDES_Cvt_CardPan((char *)CardData.ISO2Buff,37));
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "szUnCardNum(%s)", szUnCardNum);
			m_pDevCmn->fnSCR_DisplayScreen(202, K_30_WAIT, PIN_PASSWORD_MODE3, szUnCardNum, 4, 6, TRUE, "ENTER,CANCEL");
//			m_pDevCmn->fnSCR_DisplayScreen(202, K_30_WAIT, PIN_PASSWORD_MODE, "", 4, 9, FALSE, NULL);
			nEncMode = TRUE;
			break;
		case U8100:
			m_pDevCmn->fnSCR_DisplayScreen(202, K_30_WAIT, PIN_PASSWORD_MODE, "", 4, 9, FALSE, NULL);
			nEncMode = FALSE;
			break;
		default:
			m_pDevCmn->fnSCR_DisplayScreen(202, K_30_WAIT, PIN_PASSWORD_MODE, "", 4, 9, FALSE, NULL);	
			break;
		}

		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);	
		else
		if (strReturn == S_CLEAR)
		{
			nCnt++;
			continue;
		}
		else
		{
			if (nEncMode)
			{	
				strDesPin = m_pDevCmn->fstrPIN_GetPinKeyData();	// Save PassWord First				
				if(strDesPin == CHK_ERROR || strDesPin == CHK_TIMEOVER) 
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.DesEncryptPin, strDesPin.GetBuffer(0), __min(sizeof(Accept.DesEncryptPin), strDesPin.GetLength()));
					MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EPP ENC PASSWORD -> (:%16.16s len:%d)", Accept.DesEncryptPin, strDesPin.GetLength());
					return T_OK;
				}
				
			}
			else
			{
				if ((strReturn.GetLength() > sizeof(Accept.DesPassWord))	||
					(strReturn.GetLength() < 4)	    						||		
					(strReturn.GetLength() > 8)	    						||		
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
					(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
					{
							nCnt++;
							continue;
					}
			}
				
		}
		break;
	}
	
	memcpy(Accept.DesPassWord, strReturn.GetBuffer(0), strReturn.GetLength());

	m_pDevCmn->fnSCR_DisplayString(1, "Processing");
	m_pDevCmn->fnSCR_DisplayString(2, "   Encipher your PIN");
	m_pDevCmn->fnSCR_DisplayString(4, "   Please wait a while");
	m_pDevCmn->fnSCR_DisplayScreen(701);			// 안내화면(5Line)
	Delay_Msg(1000);
	
	MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "Pwd -> (:%s len:%d)", Accept.DesPassWord, strReturn.GetLength());
	MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "CRD -> (:%s len:%d)", CardData.ISO2Buff,  CardData.ISO2size);


// 	향후사용 - [청호미사용]
// 	if (m_pDevCmn->DesITKStatus || m_pDevCmn->DesTMKStatus)
// 	{
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnDES_EncryptPin!!!!", szUnPack);
// 		if (!m_pDevCmn->fnDES_EncryptPin(Accept.DesPassWord, strReturn.GetLength(), CardData.ISO2Buff, CardData.ISO2size))
// 		//if (!m_pDevCmn->fnDES_EncryptPin(Accept.DesPassWord, strReturn.GetLength(), "2462922830500025=0812                ", strlen("2462922830500025=0812                ")))
// 			fnAPP_CancelProc(T_ERROR);
// 	}
// 	else
// 	{
// 		sprintf(szUnPack,"%16.16s",	IniGetStr(_ATMKEYTABELE_INI, "ENCDATA","VALUE", "B6140B636BBE7A86"));
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "MASTE KEY -> szUnPack(:%s)", szUnPack);
// 		//MakePack(szUnPack, szPack, 16);
// 
// 		sprintf(szUnCardNum, "%s",fnDES_Cvt_CardNum((char *)CardData.ISO2Buff,37)); 
// 		m_pDevCmn->fnDES_SW_EncryptPin(Accept.DesPassWord, strReturn.GetLength(), szUnCardNum, strlen(szUnCardNum), szUnPack);
// 	}
// 	
// 	memcpy(Accept.DesEncryptPin, m_pDevCmn->DesRPIN, sizeof(Accept.DesEncryptPin));			
// 	MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "PIN -> (:%16.16s len:%d)", Accept.DesEncryptPin,  sizeof(Accept.DesEncryptPin));


	return T_OK;
}


// 모바일 PIN 번호입력
int CTranCmn::fnAPP_AcceptMPINConfirmNum()   //#N0182
{
	int		ScrNum = 3800;
	CString strReturn("");
	int		nCnt = 0;
	
	/////////////////////////////////////////////////////////////////////////////
	memset(Accept.MPINConfirmNum, '0', sizeof(Accept.MPINConfirmNum));	
																// 모바일PIN번호
	/////////////////////////////////////////////////////////////////////////////
	
	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);
	
	while (TRUE)
	{

		if(nCnt > 2) fnAPP_CancelProc(T_INPUTOVER);
		
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_PASSWORD_MODE, "", 6, 6, FALSE, "CANCEL");
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);	
		else
		if (strReturn == S_CLEAR)
		{
			nCnt++;
			continue;
		}
		else														// 자료검증
		if ((strReturn.GetLength() != sizeof(Accept.MPINConfirmNum))||	
			(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
			(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
			fnAPP_CancelProc(T_INPUTERROR);
		else
		{
			memcpy(Accept.MPINConfirmNum, strReturn.GetBuffer(0), strReturn.GetLength());
		}
		break;
	}
	
	return T_OK;
}

CString CTranCmn::fnDES_Cvt_CardNum(char* pVCardNum, int n_size)
{
	CString strResult("");

	char VCardData[30];
	char TmpVCardData[30];
	int  nCardNum = 0;


	memset(VCardData, 0x00, sizeof(VCardData));
	memset(VCardData, 0x00, sizeof(TmpVCardData));


	for(int i = 0 ; i < n_size ; i++) 
	{
		if(pVCardNum[nCardNum] == '=')
			break;
		TmpVCardData[nCardNum] = pVCardNum[nCardNum];
		nCardNum++;
	}


	
	if(nCardNum > 16) 
	{
		memcpy(&VCardData[0], &pVCardNum[nCardNum - 16], 16);
	} 
	else 
	if(nCardNum == 16) 
	{
		memcpy(&VCardData[0], pVCardNum, nCardNum);
	} 
	else 
	if(nCardNum < 16) {
		memset(&VCardData[0], 0x30, 16 - nCardNum);// 4byte를 0으로 세팅
		memcpy(&VCardData[16 - nCardNum], pVCardNum, nCardNum);
	}

	strResult.Format("%s", MakeUnPack(VCardData, strlen(VCardData)));
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnDES_Cvt_CardNum UnPack:[%s]" , strResult);
	
	return strResult; 
}


CString CTranCmn::fnDES_Cvt_CardPan(char* pVCardNum, int n_size)    //#N0176
{
	CString strResult("");

	char VCardData[30];
	char TmpVCardData[30];
	int  nCardNum = 0;
	int  nResult = 0;


	memset(VCardData, 0x00, sizeof(VCardData));
	memset(TmpVCardData, 0x00, sizeof(TmpVCardData));


	nResult = FindNotNum(CardData.ISO2Buff, 20);			// Make Card Pan
	if (nResult <= 12)
	{
		memset(&VCardData[0], '0', 1);
		memcpy(&VCardData[1], CardData.ISO2Buff, 11);
	}
	else
	if (nResult <= 18)
		memcpy(&VCardData[0], &CardData.ISO2Buff[nResult - 13], 12);
	else 
		memcpy(&VCardData[0], &CardData.ISO2Buff[nResult - 13], 12);

	strResult.Format("%s", VCardData);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnDES_Cvt_CardPan[%s]" , strResult);
	
	return strResult; 
}


// 송금주민번호비교
int CTranCmn::fnAPP_AcceptCompareJuminNum()
{
	CString strReturn("");
	int		nLoopCount = 0;
	
	while (TRUE)
	{
		m_pDevCmn->fnSCR_DisplayScreen(411, K_30_WAIT, PIN_NUMERIC_MODE);
																// 주민번호입력
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if ((strReturn == S_CANCEL)		||
			(strReturn == S_TIMEOVER)	||
			(strReturn == S_INPUTOVER)	)
			nLoopCount++;
		else
		if ((!strReturn.GetLength())	||							// 자료검증
			(strReturn.GetLength() > sizeof(Accept.JuminNum))		||
			(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
			(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
			nLoopCount++;
		else
		{
			if (memcmp(strReturn.GetBuffer(0), Accept.JuminNum, strReturn.GetLength()))
				nLoopCount++;
			else
			{
				nLoopCount = 0;
				break;
			}
		}

		if (nLoopCount >= 5)
			break;
	}

	if (nLoopCount >= 5)
	{
		m_pDevCmn->fnSCR_DisplayString(1, "처음에 입력하신 주민번호와");
		m_pDevCmn->fnSCR_DisplayString(2, "재입력하신 주민번호가 틀립니다.");
		m_pDevCmn->fnSCR_DisplayString(3, "창구에 문의하여 주시기 바랍니다.");
		m_pDevCmn->fnSCR_DisplayScreen(701);

		m_pDevCmn->fnAPL_StackError("0110295", "송금현금회수", DEV_BRM);
		fnAPD_CashDepositStore(TRUE);							// 현금입금수납
		fnAPP_CancelProc(T_OK, NULL, NULL, NULL);				// 2007-01-03-#01
	}

	return T_OK;
}

// 통장 계속인자 여부 확인
int	CTranCmn::fnAPP_AcceptContinuousPBChoice()
{
	CString strReturn("");

	MS		BfrPbMsData;								//11/22
	memcpy (BfrPbMsData.sBuf, PbMsData.sBuf, 256);		//test

	int		nGetKeyID = NULL;

	if (m_pDevCmn->TranResult == FALSE)							// 본거래 정상처리시만 지원
		return T_OK;

	if (!PbTran)												// 통장거래일때만 지원
		return T_OK;

	fnAPD_CardEnDisable(DISABLE, TRUE);
	fnAPD_PbEnDisable(ENABLE, TRUE);
	m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_ON);

	m_pDevCmn->fnSCR_DisplayScreen (550, K_30_WAIT, PIN_MENU_MODE);

	nGetKeyID = m_pDevCmn->fnCMN_ScanDeviceAction(DEV_PBM | DEV_SCR, K_45_WAIT, EVENT_IN);
	if (nGetKeyID == DEV_PBM)
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_OFF);
		m_pDevCmn->fnSCR_DisplayScreen(805);			// 통장을 읽고있습니다.
		fnAPD_PbReadCheck();
//		if (memcmp(BfrPbMsData.sBuf, PbMsData.sBuf, 256))						//11/22	01-01-01														
//			fnAPP_CancelProc(T_MSG, "계좌번호 확인후 다시 거래하여 주십시오 ");	//test
// 		return T_OK;
		if (memcmp(BfrPbMsData.sBuf, PbMsData.sBuf, 256))	//11/22	01-01-01 통장비교														
		{							// 안내화면 3초출력
			m_pDevCmn->fnSCR_DisplayString(3, "계좌번호 확인후 다시 거래하여 주십시오");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(3 * 1000);
			PbNextPageFlag = FALSE;									// 연속처리 없음
			PbNextTranFlag = FALSE;									// 연속처리 없음
			fnAPD_MaterialOut(DEV_PBM);								// 매체방출	
			fnAPD_MaterialOutCheckRetract(DEV_PBM);					// 메체방출수취대기회수
			return T_OK;		
		}
		return T_OK;
	}
	else
	if (nGetKeyID == DEV_SCR)
	{
		strReturn = m_pDevCmn->fstrSCR_GetKeyString(FALSE);

		if ((strReturn == S_CANCEL)	||
			(strReturn == S_TIMEOVER))
		{														// 안내화면 3초출력
			fnAPD_PbEnDisable(DISABLE);                         // #N0136
			m_pDevCmn->fnSCR_DisplayString(3, "이용해 주셔서 감사합니다");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(3 * 1000);

			PbNextPageFlag = FALSE;								// 연속처리 없음
			PbNextTranFlag = FALSE;								// 연속처리 없음

			return T_OK;										// 정상거래완료처리
		}
		else
			fnAPP_CancelProc(T_PROGRAM);
	}
	else
		fnAPP_CancelProc(T_PROGRAM);
	
	return T_OK;
}
//#0014
int CTranCmn::fnAPP_AcceptTranPBProc()
{
	if ((m_pDevCmn->SightHandiCapped) || (m_pDevCmn->EarPhoneInserted)	
		|| (m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE))			
	{
		TranProc  = TRAN_PB;
		TranCode  = TC_PBOOK;
		TranCode2 = TRANID_4730;
		PbNextTranFlag = TRUE;

		return T_OK;
	}

	int		ScrNum = 549;
	CString strReturn("");
	PbNextTranFlag = FALSE;

	if (m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
	{
		m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_012x); //은행명
		m_pDevCmn->fnSCR_DisplayString(2, strCT(m_RD.byDataField_021x, strlen(m_RD.byDataField_021x))); //성명
		m_pDevCmn->fnSCR_DisplayString(3, m_RD.byDataField_017x); //계좌
		ScrNum = 554;
	}
	else
	if (m_pProfile->DEVICE.ETCDevice20 == KIUP)
		ScrNum = 549;
	else
	if (m_pProfile->DEVICE.ETCDevice20 == KEB) //#0085
		ScrNum = 555;
	else
	if (m_pProfile->DEVICE.ETCDevice20 == KWANGJU)  //#N0181
		ScrNum = 549;
	else
	if (m_pProfile->DEVICE.ETCDevice20 == HANA)  //#N0214
		ScrNum = 549;
	else
	if (m_pProfile->DEVICE.ETCDevice20 == SHINHAN)  //#N0215
		ScrNum = 549;
	else
	if (m_pProfile->DEVICE.ETCDevice20 == SC)		//#N0219
		ScrNum = 549;
	else
	if (m_pProfile->DEVICE.ETCDevice20 == KB)  //#N0226
	{
		//ScrNum = 549;
		TranProc  = TRAN_PB;
		TranCode  = TC_PBOOK;
		TranCode2 = TRANID_4730;
		PbNextTranFlag = TRUE;

		return T_OK;
	}
	else
	if (m_pProfile->DEVICE.ETCDevice20 == CITI)  //#N0238
		ScrNum = 549;
	else
	if (m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)  //#N0282
		ScrNum = 549;
	else
		ScrNum = 554;

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == S_CONFIRM)
	{
		TranProc  = TRAN_PB;
		TranCode  = TC_PBOOK;
		TranCode2 = TRANID_4730;
		PbNextTranFlag = TRUE;
	}
	else
	if (strReturn == S_YES)
	{
		TranProc  = TRAN_PB;
		TranCode  = TC_PBOOK;
		TranCode2 = TRANID_4730;
		PbNextTranFlag = TRUE;
	}
	else
	if ((strReturn == S_NO) || (strReturn == S_EXIT) || (strReturn == S_CANCEL) || strReturn == S_CANCEL2)	// #N0272
	{
		;
	}
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);		

	return T_OK;
}

int CTranCmn::fnAPP_TranWithChargeInqInputProc()
{
	int		ScrNum = 429;
	CString strReturn("");
	CString	cstrTemp("");

	if(TranCode == TC_WITHDRAW || TranCode == TC_FOREGIN)   //#N0171
	{
		m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_017x);
		m_pDevCmn->fnSCR_DisplayAmount(2, m_RD.byDataField_023x);
		m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)m_RD.byDataField_024x);

		if(m_pProfile->DEVICE.ETCDevice20 == KEB && memcmp(&m_RD.byDataField_345x[0], "PT", 2) == 0)  //#N0180
			m_pDevCmn->fnSCR_DisplayString(10, m_RD.byDataField_345x);
	}
	else  //#N0161
	if(TranCode == TC_SVCWITH)
	{
		if(TranResultNGCode == TRAN_MSG_EXCEPTION_707)
		{
			ScrNum = 427;
		}
		else
		{
			ScrNum = 426;
			if (memcmp(m_RD.byDataField_023x, ZERO8, strlen(ZERO8)) == 0)
				m_pDevCmn->fnSCR_DisplayAmount(1, Accept.Money, 15);
			else
				m_pDevCmn->fnSCR_DisplayAmount(1, m_RD.byDataField_023x);
			
			if((memcmp(m_RD.byDataField_458x, "????", 4) != 0) && (memcmp(m_RD.byDataField_458x, "****", 4) != 0))
			{
				cstrTemp.Empty();
				if (m_pDevCmn->EarPhoneInserted == FALSE)		
					cstrTemp.Format("%d.%2.2s", Asc2Int(&m_RD.byDataField_458x[0], 2), &m_RD.byDataField_458x[2]);
				else
					cstrTemp.Format("이자율 연 %d.%2.2s  퍼센트", Asc2Int(&m_RD.byDataField_458x[0], 2), &m_RD.byDataField_458x[2]);
				m_pDevCmn->fnSCR_DisplayString(2, cstrTemp);
			}
			else
			if(memcmp(m_RD.byDataField_458x, "****", 4) == 0)
			{
				if (m_pDevCmn->EarPhoneInserted == TRUE)		// 2번째 값이 비어있을 경우 3번째 값이 2번째에 읽혀지는 현상 방지
					m_pDevCmn->fnSCR_DisplayString(2, " ");
				else
					m_pDevCmn->fnSCR_DisplayString(2, (LPCTSTR)m_RD.byDataField_458x);
			}
			if(memcmp(m_RD.byDataField_024x, "******", 6) == 0 || memcmp(m_RD.byDataField_024x, "    ", 4) == 0)    //수수료 * 화면 표시
			{
				if (m_pDevCmn->EarPhoneInserted == TRUE)		// 3번째 값이 비어있을 경우 4번째 값이 3번째에 읽혀지는 현상 방지
					m_pDevCmn->fnSCR_DisplayString(3, " ");
				else
					m_pDevCmn->fnSCR_DisplayString(3, "****");
//					m_pDevCmn->fnSCR_DisplayString(3, (LPCTSTR)m_RD.byDataField_024x);
			}
			else
				m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)m_RD.byDataField_024x);
		}
	}
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT, PIN_MENU_MODE1);

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == S_CONFIRM)
	{
		;
	}
	else
	if (strReturn == S_YES)
	{
		;
	}
	else
	if (strReturn == S_NO)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_EXIT)
		fnAPP_CancelProc(T_CANCEL);
	else
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

	return T_OK;
}

// 지로세부거래코드선택
int CTranCmn::fnAPP_UTILYBILLS_Sub01_InputProc()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.JiroTranCode = 0;									// 지로세부거래코드
	memset(Accept.JiroTranOrgCode, 0x00, sizeof(Accept.JiroTranOrgCode));		// 지로납부기관코드
/////////////////////////////////////////////////////////////////////////////

	//m_pDevCmn->fnSCR_DisplayImage(7, FALSE); //상하수도 Disable

	m_pDevCmn->fnSCR_DisplayScreen(770, K_30_WAIT, PIN_MENU_MODE1);				// 납부요금종류선택  #N0173
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == "일반전화요금")
	{
		Accept.JiroTranCode = TC_JRPHONE;						// KT전화요금
		memcpy(Accept.JiroTranOrgCode, "710000000", 9);
	}
	else
	if (strReturn == "전기요금")
	{
		Accept.JiroTranCode = TC_JRELEC;						// 전기요금
		memcpy(Accept.JiroTranOrgCode, "720000000", 9);
	}
	else
	if (strReturn == "건강보험료")
	{
		Accept.JiroTranCode = TC_JRINSURE;						// 건강보험료
		memcpy(Accept.JiroTranOrgCode, "730000000", 9);
	}
	else
	if (strReturn == "국민연금")
	{
		Accept.JiroTranCode = TC_JRPANS;						// 국민연금
		memcpy(Accept.JiroTranOrgCode, "740000000", 9);
	}
	else
	if (strReturn == "지로대금")
	{
		Accept.JiroTranCode = TC_JRJIRO;						// 지로대금
		memcpy(Accept.JiroTranOrgCode, "900000000", 9);
	}
	else
	if (strReturn == "국고/국세")
	{
		Accept.JiroTranCode = TC_JRNTAX;						// 국고/국세
		memcpy(Accept.JiroTranOrgCode, "000000000", 9);
	}
	else
	if (strReturn == "지방세")
	{
		Accept.JiroTranCode = TC_JRLTAX;						// 지방세
		memcpy(Accept.JiroTranOrgCode, "000000000", 9);
	}
	else
	if (strReturn == "상하수도요금")
	{
		Accept.JiroTranCode = TC_JRWATER;						// 상하수도요금
		memcpy(Accept.JiroTranOrgCode, "000000000", 9);
	}
	else
	if (strReturn == "교통범칙금")
	{
		Accept.JiroTranCode = TC_JRTRAFF;						// 교통범칙금
		memcpy(Accept.JiroTranOrgCode, "000000000", 9);
	}
	else
	if ((strReturn == S_CANCEL)||(strReturn == S_CANCEL))
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);


	return T_OK;
}

// 지로번호입력
int	CTranCmn::fnAPP_UTILYBILLS_Sub02_InputProc()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.JiroNum, ' ', sizeof(Accept.JiroNum));		// 지로번호
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(771, K_30_WAIT, PIN_NUMERIC_MODE);				// 지로번호입력  #N0173
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((strReturn.GetLength() != sizeof(Accept.JiroNum))	||	// 자료검증
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.JiroNum, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// 전자납부번호입력
int	CTranCmn::fnAPP_UTILYBILLS_Sub03_InputProc()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.JiroElecNumSize = 0;									// 전자납부번호길이
	memset(Accept.JiroElecNum, ' ', sizeof(Accept.JiroElecNum));// 전자납부번호
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(772, K_30_WAIT, PIN_NUMERIC_MODE);				// 전자납부번호입력  #N0173
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.JiroElecNum))	||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		Accept.JiroElecNumSize = strReturn.GetLength();
		memcpy(Accept.JiroElecNum, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}


//#GIRO 지방세 타인 조회방법 선택
int CTranCmn::fnAPP_LOCALTAX_Sub01_InputProc()
{

	CString strReturn("");

	m_pDevCmn->fnSCR_DisplayScreen(3017, K_30_WAIT);				// 조회방법 선택

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == "간편납부번호")
	{
		fnAPP_LOCALTAX_AcceptSimpleNum();
//		Accept.JiroGubun = 'E';
	}
	else
	if (strReturn == "전자납부번호")
	{
		fnAPP_LOCALTAX_AcceptElecNum();
//		Accept.JiroGubun = 'Y';
	}
	else
	if ((strReturn == S_CANCEL)||(strReturn == S_CANCEL))
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

//#GIRO 신용카드 할부 선택
int CTranCmn::fnAPP_LOCALTAX_Sub02_InputProc()
{

	CString strReturn("");
	CString strTemp("");

	strTemp.Format("%d", Asc2Int(&m_RXLOCALTAXDATA.JiroBankCode[5],2));  //카드번호
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);

	m_pDevCmn->fnSCR_DisplayScreen(3013, K_30_WAIT, PIN_MENU_MODE1);				// 조회방법 선택 , #N0173

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == "일시불결제")
	{
		memcpy(&m_RXLOCALTAXDATA.JiroBankCode[5], "00", 2);
	}
	else
	if (strReturn == "할부결제")
	{
		fnAPP_LOCALTAX_AcceptPayMonth();
	}
	else
	if ((strReturn == S_CANCEL)||(strReturn == S_CANCEL))
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}


int	CTranCmn::fnAPP_LOCALTAX_AcceptPayMonth()
{
	CString strReturn("");
	char szT[10];

	m_pDevCmn->fnSCR_DisplayString(1, strCT(m_RXLOCALTAXDATA.BillPayMonth,2));

	m_pDevCmn->fnSCR_DisplayScreen(3014, K_30_WAIT , PIN_NUMERIC_MODE);				// 간편납부번호 #N0173
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.JiroElecNum))	||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		Sprintf(szT, 2, "%02d", Asc2Int(strReturn)); //#H0177 - Bug 수정 
		memcpy(Accept.JiroPayMonth, szT, 2);
	}

	return T_OK;
}

int	CTranCmn::fnAPP_LOCALTAX_AcceptSimpleNum()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.JiroElecNum, ' ', sizeof(Accept.JiroElecNum));// 전자납부번호
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(3011, K_30_WAIT , PIN_NUMERIC_MODE);				// 간편납부번호 #N0173
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.JiroElecNum))	||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.JiroElecNum, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

int	CTranCmn::fnAPP_LOCALTAX_AcceptElecNum()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.JiroElecNumSize = 0;									// 전자납부번호길이
	memset(Accept.JiroElecNum, ' ', sizeof(Accept.JiroElecNum));// 전자납부번호
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(772, K_30_WAIT, PIN_NUMERIC_MODE);		// 전자납부번호입력 #N0173
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((!strReturn.GetLength())	||							// 자료검증
		(strReturn.GetLength() > sizeof(Accept.JiroElecNum))	||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		Accept.JiroElecNumSize = strReturn.GetLength();
		memcpy(Accept.JiroElecNum, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}


