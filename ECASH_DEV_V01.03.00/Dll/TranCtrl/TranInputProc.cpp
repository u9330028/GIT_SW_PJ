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

// ���� ��й�ȣ ��ȣȭ ���̺귯�� �Լ�
__declspec(dllimport) long TYEncrypt(char *pIn, char *pOut);


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	�������Լ�
/////////////////////////////////////////////////////////////////////////////
// ������й�ȣ�Է�
int CTranCmn::fnAPP_AcceptManagerPassword()
{
	int		ScrNum = 0;
	CString	ManagerPassword("51000114");
	CString strReturn("");

	CString strVersionData("");
	strVersionData.Format("[[ûȣ����ĳ��] U3100K/U8100 %9.9s(%2.2s)]", m_pProfile->TRANS.ApVersion, REVISION);

	ScrNum = SCR_MANAGER_PASSWORD;								// �����ں�й�ȣ�Է�

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
				m_pDevCmn->fnSCR_DisplayString(1, strVersionData.GetBuffer(0));		// �������
				m_pDevCmn->fnSCR_DisplayScreen(SCR_ATM_START);						// �غ���ȭ��
				m_pDevCmn->fnSCR_DisplayString(3, "�ڵ�ȭ��⸦ �غ��ϰ� �ֽ��ϴ�.");
				m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�.");
				return T_OK;
			}
		}
	}
	return T_OK;
}

// OP������ ��й�ȣ�Է�
int CTranCmn::fnAPP_AcceptOpPassword()
{
	int		ScrNum = 0;
	int		nGetCnt = 0;
	int		nRet = 0;
	CString	ManagerOpPassword("000000");
	CString strReturn("");

	CString strVersionData("");
	strVersionData.Format("[[ûȣ����ĳ��] U3100K/U8100 %9.9s(%2.2s)]", m_pProfile->TRANS.ApVersion, REVISION);

	
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
	if (strReturn == "NH�Ա�")					// NH�Ա�(����)
	{
		Accept.InQuiryCycle = '1';
		TranCode2 = MenuTblTemp[11].MenuInfo[1][6];	// �ŷ��ڵ�2-> TRANID_1090
	}
	else
	if (strReturn == "�����Ա�")				// �����Ա�(��,�Ͽ���,������)
	{
		Accept.InQuiryCycle = '2';
		TranCode2 = MenuTblTemp[11].MenuInfo[2][6];	// �ŷ��ڵ�2-> TRANID_1060
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
			if ((strReturn.GetLength() != sizeof(Accept.PassWord))	    ||	// �ڷ����
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



// ���� ���� ����
int CTranCmn::fnAPP_FNGAccept()
{
	CString GetKeyStr("");
	int		nReturn = 0;
	CString strReturn("");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept()");
	if (!MenuFingerMode)
		return T_OK;

	if (MenuFingerMode)											// �����νİŷ�
	{
		m_pDevCmn->fnSCR_DisplayScreen(870, K_60_WAIT, PIN_MENU_MODE);
																// �����Է�
		
		if(m_pDevCmn->fnFNG_GetDeviceStatus(1) == NORMAL)
		{
			m_pDevCmn->fnFNG_Initialize();							// ������۹��ʱ�ȭ
			m_pDevCmn->fnFNG_Acquire();								// �����б�
		}
		else
			fnAPP_CancelProc(T_MSG, "�����ν� ��ġ ���� ���з� ��� �� �� �����ϴ�");

		nReturn = m_pDevCmn->fnCMN_ScanDeviceAction(DEV_FNG | DEV_SCR, K_65_WAIT, EVENT_IN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept:nReturn[%d]", nReturn);
		if (nReturn == R_TIMEOUT)
		{
			m_pDevCmn->fnFNG_CancelAcquire();					// �����б����
			fnAPP_CancelProc(T_TIMEOVER);
		}
		else
		if (nReturn == DEV_FNG)
		{
			m_pDevCmn->fnSCR_DisplayScreen(871);				// ����Ȯ��
			Delay_Msg(2000);									// ����Ȯ�δ��								
			strReturn = m_pDevCmn->fstrFNG_GetData();
			FingerData.Len = __min(sizeof(FingerData.sBuf) * 2, strReturn.GetLength()) / 2;
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept:FingerData", strReturn.GetBuffer(0), strReturn.GetLength());
			MakePack(strReturn.GetBuffer(0), FingerData.sBuf, __min(sizeof(FingerData.sBuf) * 2, strReturn.GetLength()));
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept:FingerData", &FingerData, sizeof(FingerData));
			m_pDevCmn->fnFNG_CancelAcquire();					// �����б����
		}
		else
		if (nReturn == DEV_SCR)
		{
			strReturn = m_pDevCmn->fstrSCR_GetKeyString(FALSE);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept:strReturn[%s]", strReturn);
			m_pDevCmn->fnFNG_CancelAcquire();					// �����б����
			if (strReturn == S_CANCEL)
				fnAPP_CancelProc(T_CANCEL);
			else fnAPP_CancelProc(T_TIMEOVER);
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FNGAccept()::return");
	return T_OK;
}

// V02.C4.04:���� ���� ����(��Ȳ�� Call)
int CTranCmn::fnAPP_RDCtrlPasswdAccept()
{


	CString strReturn("");
	int		ScrNum = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.SlockPassWord, 0, sizeof(Accept.SlockPassWord));	
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(872, K_90_WAIT, PIN_MENU_MODE);
																// �ֹι�ȣ�Է�
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
	if ((!strReturn.GetLength())	||							// �ڷ����
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
//	�ѵ��ݾ׼����Լ�
/////////////////////////////////////////////////////////////////////////////
// �����ѵ����ϱ�
int CTranCmn::fnAPP_CalcMoneyMaxOut()
{


	int		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
	int		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
	int		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;					// �ִ����ݾ�(��������)
 	int		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�

	int		CashMaxOutCnt1	= 0;
	int		MoneyMaxOut1	= 0;
	int		CashMaxOutCnt2	= 0;
	int		MoneyMaxOut2	= 0;
	int		iMax50TCashOut = 0;
	
	int		CheckValue = 0;
	int		nCurrCheckKind = m_pDevCmn->fnAPL_GetKindOfOutCheck();
	if (nCurrCheckKind == CHECKKIND_MILLION)					// 100������
	{
		CheckValue = CHECK_MILLION;								// 100����
		MAXCHECKOUTCNT = MONEY_MAXOUT / CheckValue;				// 300������ 3��
	}
	else
	if (nCurrCheckKind == CHECKKIND_HUNDRED_THOUSAND)			// 10������
	{
		CheckValue = CHECK_HUNDRED_THOUSAND;					// 10����
		MAXCHECKOUTCNT = BRM_MAXCHECKOUTCNT;					// ����ο� ����(30��)
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
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;					// �ִ����ݾ�(��������)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�
		if ((CardBank == KIUBANK) && (TranCode == TC_WITHDRAW)) 
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			
//			if (!m_pDevCmn->ChangeBCFlag)
			{
				MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
				MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
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
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_70;				// �ִ����ݾ�(��������)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�

		if ((CardBank == INONBANKID1 || CardBank == INONBANKID2 || CardBank == INONBANKID3 ||CardBank == INONBANKID4) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
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
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;					// �ִ����ݾ�(��������)	#N0276
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�
		if ((CardBank == KOEBANK || CardBank == IHANBANKID1 || CardBank == IHANBANKID2 || CardBank == IHANBANKID3 ||CardBank == IHANBANKID4) && (TranCode == TC_WITHDRAW))	//#N0276
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
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
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;				// �ִ����ݾ�(��������)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�
		if ((CardBank == KAWBANK) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
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
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;				// �ִ����ݾ�(��������)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�
		if ((CardBank == KYOBANK) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
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
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT;				// �ִ����ݾ�(��������)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�
		if ((CardBank == KOEBANK || CardBank == IHANBANKID1 || CardBank == IHANBANKID2 || CardBank == IHANBANKID3 ||CardBank == IHANBANKID4) && (TranCode == TC_WITHDRAW))	//#N0276
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
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
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_70;				// �ִ����ݾ�(��������)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�
		if ((CardBank == ISHIBANKID || CardBank == ISHIBANKID1 || CardBank == ICHOBANKID) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
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
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_70;				// �ִ����ݾ�(��������)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�
		if ((CardBank == IKOFBANKID) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
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
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_70;				// �ִ����ݾ�(��������)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�
		if ((CardBank == IKUMBANKID1 || CardBank == IKUMBANKID2 || CardBank == IKUMBANKID3 ||CardBank == IKUMBANKID4 ||CardBank == IKUMBANKID5) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
			
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
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_70;				// �ִ����ݾ�(��������)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�
		if ((CardBank == IGOOBANKID1 || CardBank == IGOOBANKID2 || CardBank == ICITBANKID) && (TranCode == TC_WITHDRAW))
		{
			MONEY_MAXOUT =  m_pProfile->TRANS.JaMaxDispAmt;                 
			MONEY_MAXOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
			MAXCASHOUTCNT = m_pProfile->TRANS.JaMaxDispAmt;				// �ִ����ż�
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
		MAXCASHOUTCNT	= BRM_MAXCASHOUTCNT;				// �����ִ����ż�
		MAXCHECKOUTCNT	= BRM_MAXCHECKOUTCNT;				// ��ǥ�ִ����ż�															
		MONEY_MAXOUT	= BRM_MONEY_MAXOUT_30;				// �ִ����ݾ�(��������)
		MONEY_MAXOUTCNT = BRM_MONEY_MAXOUTCNT;				// �ִ����ż�
		
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
		
		if ((MAXCASHOUTCNT >= 1)	&&							// 1�� ~ ����ѵ�
			(MAXCASHOUTCNT <= iMax50TCashOut))
			;
		else MAXCASHOUTCNT = 0;									// �ݾ��̻�� 0����
	}
	else
	{
		if ((MAXCASHOUTCNT >= 1)	&&							// 1�� ~ ����ѵ�
			(MAXCASHOUTCNT <= BRM_MAXCASHOUTCNT))
			;
		else MAXCASHOUTCNT = 0;									// �ݾ��̻�� 0����
	}


	if (nCurrCheckKind == CHECKKIND_MILLION)					// ��ī����
	{
		if ((MAXCHECKOUTCNT >= 1)	&&							// 1�� ~ ����ѵ�
			(MAXCHECKOUTCNT <= BRM_MAXCHECKOUTCNT_100CHECK));
		else MAXCHECKOUTCNT = 0;								// �ż��̻�� 0��
	}
	else
	{
		if ((MAXCHECKOUTCNT >= 1)	&&							// 1�� ~ ����ѵ�
			(MAXCHECKOUTCNT <= BRM_MAXCHECKOUTCNT));
		else MAXCHECKOUTCNT = 0;								// �ż��̻�� 0��
	}
	// 100���� ��ݽ� �ѵ��� ����� ��� BRM_MONEY_MAXOUT �����ʿ�
	if (nCurrCheckKind == CHECKKIND_MILLION)
	{															// V02-08-01-R1 �鸸��������
		if ((MONEY_MAXOUT >= 1)	&&								// 1�� ~ ����ѵ�
			(MONEY_MAXOUT <= BRM_MONEY_MAXOUT));
		else MONEY_MAXOUT = 0;									// �ݾ��̻�� 0����
	}
	else
	{
		if ((MONEY_MAXOUT >= 1)	&&								// 1�� ~ ����ѵ�
			(MONEY_MAXOUT <= BRM_MONEY_MAXOUT));
		else MONEY_MAXOUT = 0;									// �ݾ��̻�� 0����
	}

//	MONEY_MAXOUT = MAXCASHOUTCNT;								// ���ݾװ�����
//	MONEY_MAXOUTCNT = MAXCASHOUTCNT;							// ���ݸż��͵���


	MoneyMaxOut = 0;											// ���Ⱑ���ѵ��ݾ�
	MoneyMaxOutCnt = 0;											// ���Ⱑ�ɸż�
	CashMaxOutCnt = 0;											// ���ݹ��Ⱑ�ɸż�
	CheckMaxOutCnt = 0;											// ��ǥ���Ⱑ�ɸż�
	CheckLowMaxOutCnt = 0;										// ��ǥ�����ּҸż�
	CheckHighMaxOutCnt = 0;										// ��ǥ�����ִ�ż�

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxOutATMLarge - 1:MAXCASHOUTCNT [%d], MONEY_MAXOUT[%d], MONEY_MAXOUTCNT[%d]", MAXCASHOUTCNT, MONEY_MAXOUT, MONEY_MAXOUTCNT);

	switch (WithAvail)
	{
		case TRAN_WITH_CASH:
		case TRAN_WITH_2CASH:									
			CashMaxOutCnt = m_pDevCmn->fnAPL_GetNumberOfCash(); //1�� + 5��    
			if (CashMaxOutCnt > MAXCASHOUTCNT)
				CashMaxOutCnt = MAXCASHOUTCNT;

			break;
			
		case TRAN_WITH_50TCASH:
			CashMaxOutCnt = m_pDevCmn->fnAPL_GetNumberOf50000Cash();// ���ݸż����ϱ�
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
			CashMaxOutCnt = m_pDevCmn->fnAPL_GetNumberOf50000Cash();// ���ݸż����ϱ�
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

	while (CheckMaxOutCnt)										// ��ǥ Low, High Max ����
	{
		if ((WithAvail == TRAN_WITH_50TCASH)	||
			(WithAvail == TRAN_WITH_50TCASHCHECK))
			MoneyMaxOut = CashMaxOutCnt*CASH_FIFTY_THOUSAND + (CheckMaxOutCnt * CheckValue);
		else
			MoneyMaxOut = CashMaxOutCnt + (CheckMaxOutCnt * CheckValue);		
																
		if (MoneyMaxOut > MONEY_MAXOUT)
			MoneyMaxOut = MONEY_MAXOUT;

		fnAPP_CalcCheckLowMaxOutCnt(MoneyMaxOut);				// �ּҼ�ǥ�ż����ϱ�
		fnAPP_CalcCheckHighMaxOutCnt(MoneyMaxOut);				// �ִ��ǥ�ż����ϱ�

		if ((WithAvail == TRAN_WITH_50TCASH)	||
			(WithAvail == TRAN_WITH_2CASH)		||
			(WithAvail == TRAN_WITH_50TCASHCHECK))
		{
			CashMaxOutCnt1 = (MoneyMaxOut - (CheckLowMaxOutCnt * CheckValue)) / CASH_FIFTY_THOUSAND;	// V05-04-01-#09:100���� ��ǥ����
			MoneyMaxOut1 = (CheckLowMaxOutCnt * CheckValue) + (CashMaxOutCnt1*CASH_FIFTY_THOUSAND);		// V05-04-01-#09:100���� ��ǥ����
			CashMaxOutCnt2 = (MoneyMaxOut - (CheckHighMaxOutCnt * CheckValue)) / CASH_FIFTY_THOUSAND;	// V05-04-01-#09:100���� ��ǥ����
			MoneyMaxOut2 = (CheckHighMaxOutCnt * CheckValue) + (CashMaxOutCnt2*CASH_FIFTY_THOUSAND);	// V05-04-01-#09:100���� ��ǥ����
		}
		else
		{
			CashMaxOutCnt1 = (MoneyMaxOut - (CheckLowMaxOutCnt * CheckValue));							// V05-04-01-#09:100���� ��ǥ����
			MoneyMaxOut1 = (CheckLowMaxOutCnt * CheckValue) + CashMaxOutCnt1;							// V05-04-01-#09:100���� ��ǥ����
			CashMaxOutCnt2 = (MoneyMaxOut - (CheckHighMaxOutCnt * CheckValue));							// V05-04-01-#09:100���� ��ǥ����
			MoneyMaxOut2 = (CheckHighMaxOutCnt * CheckValue) + CashMaxOutCnt2;							// V05-04-01-#09:100���� ��ǥ����
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
			if ((CashMaxOutCnt1 >= 0)					&&		// �������ݸż�üũ
				(CashMaxOutCnt1 <= CashMaxOutCnt)		&&
				(CashMaxOutCnt2 >= 0)					&&
				(CashMaxOutCnt2 <= CashMaxOutCnt)		&&
				(MoneyMaxOut1 == MoneyMaxOut)			&&		// ����ݾ׵���üũ
				(MoneyMaxOut2 == MoneyMaxOut))
				break;
		}

		if (WithAvail == TRAN_WITH_CHECK)
		{
			if ((CheckLowMaxOutCnt >= 0)				&&		// �����ǥ�ż�üũ
				(CheckLowMaxOutCnt <= CheckMaxOutCnt)	&&
				(CheckHighMaxOutCnt >= 0)				&&
				(CheckHighMaxOutCnt <= CheckMaxOutCnt)	&&
				(MoneyMaxOut1 == MoneyMaxOut)			&&		// ����ݾ׵���üũ
				(MoneyMaxOut2 == MoneyMaxOut))
				break;
		}

		if ((WithAvail == TRAN_WITH_BOTH)		||
			(WithAvail == TRAN_WITH_CASHCHECK)	||
			(WithAvail == TRAN_WITH_50TCASHCHECK))
		{
			if ((CashMaxOutCnt1 >= 0)					&&		// �������ݸż�üũ
				(CashMaxOutCnt1 <= CashMaxOutCnt)		&&
				(CashMaxOutCnt2 >= 0)					&&
				(CashMaxOutCnt2 <= CashMaxOutCnt)		&&
				(CheckLowMaxOutCnt >= 0)				&&		// �����ǥ�ż�üũ
				(CheckLowMaxOutCnt <= CheckMaxOutCnt)	&&
				(CheckHighMaxOutCnt >= 0)				&&
				(CheckHighMaxOutCnt <= CheckMaxOutCnt)	&&
				(MoneyMaxOut1 == MoneyMaxOut)			&&		// ����ݾ׵���üũ
				(MoneyMaxOut2 == MoneyMaxOut))
				break;
		}
		
		CheckMaxOutCnt--;
	}

	if ((WithAvail == TRAN_WITH_50TCASH)	||
		(WithAvail == TRAN_WITH_50TCASHCHECK))
		MoneyMaxOut = CashMaxOutCnt*CASH_FIFTY_THOUSAND + (CheckMaxOutCnt * CheckValue);		// V05-04-01-#09:100���� ��ǥ����
	else
		MoneyMaxOut = CashMaxOutCnt + (CheckMaxOutCnt * CheckValue);							// V05-04-01-#09:100���� ��ǥ����

	if (MoneyMaxOut > MONEY_MAXOUT)
		MoneyMaxOut = MONEY_MAXOUT;

	MoneyMaxOutCnt = CashMaxOutCnt + CheckMaxOutCnt;			// ���Ⱑ�ɸż�
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

// ��ǥ�ּҸż����ϱ�
int CTranCmn::fnAPP_CalcCheckLowMaxOutCnt(int MoneyOut)
{
	int		MAXCASHOUTCNT = BRM_MONEY_MAXOUTCNT;				// �����ִ����ż�
	int		CheckValue = 0;										// ��ǥ�⺻�ݾ�ó��
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
	

																// ��ī����
	if ((MAXCASHOUTCNT >= 1)	&&								// 1�� ~ ����ѵ�
		(MAXCASHOUTCNT <= BRM_MAXCASHOUTCNT));
	else MAXCASHOUTCNT = 0;										// �ݾ��̻�� 0����

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

	CheckLowMaxOutCnt = 0;										// ��ǥ�����ּҸż����ϱ�
	
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
			if (MoneyOut < (MAXCASHOUTCNT + 1))					// �ϳ����(����,��ǥ���ոż�)
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
// ��ǥ�ִ�ż����ϱ�
int CTranCmn::fnAPP_CalcCheckHighMaxOutCnt(int MoneyOut)
{
	int		CheckValue = 0;										// ��ǥ�⺻�ݾ�ó��

	CheckHighMaxOutCnt = 0;										// ��ǥ�����ִ�ż����ϱ�
	
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
			if (CheckValue == CHECK_UNKNOWN)					// CodeSonar ����
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

// ��ü�ѵ����ϱ�
int CTranCmn::fnAPP_CalcMoneyMaxTrans()
{
	if(TranCode == TC_SVCTRAN1)
		TMoneyMaxOut = Asc2Int(OpenInfo.SvcTransMoney, 9);				// ��ü�����ѵ��ݾ�
	else
		TMoneyMaxOut = Asc2Int(OpenInfo.TransMoney, 9);				// ��ü�����ѵ��ݾ�

	if ((TMoneyMaxOut >= 1000000)	&&							// 100�� ~ 1��̸�
		(TMoneyMaxOut < 100000000));
	else
		TMoneyMaxOut = 1000000;									// �ݾ��̻�� 100����

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxTrans:TMoneyMaxOut[%d]", TMoneyMaxOut);
	return TMoneyMaxOut;
}

// �Ա��ѵ����ϱ�
int CTranCmn::fnAPP_CalcMoneyMaxIn()
{
	CashMaxIn = fnAPP_CalcCashMaxIn();							// �����Ա��ѵ����ϱ�
	CheckMaxInCnt = fnAPP_CalcCheckMaxIn();						// ��ǥ�Ա��ѵ����ϱ�

	if ((CashMaxIn >= 10)	&&									// 10�� ~ ����ѵ�
		(CashMaxIn <= BRM_MAXCASHINCNT));
	else
		CashMaxIn = 10;											// �ݾ��̻�� 10����

	if ((CheckMaxInCnt >= 1)	&&								// 1�� ~ ����ѵ�
		(CheckMaxInCnt <= UCM_MAXCHECKINCNT));
	else
		CheckMaxInCnt = 0;										// �ż��̻�� 0��

	MoneyMaxIn = CashMaxIn + (CheckMaxInCnt * 100);				// �Աݰ����ѵ��ݾ�
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxIn:CashMaxIn[%d], CheckMaxInCnt[%d], MoneyMaxIn[%d]", CashMaxIn, CheckMaxInCnt, MoneyMaxIn);
	return MoneyMaxIn;
}

// �����Ա��ѵ����ϱ�
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
	
	if ((CashMaxIn >= 1)	&&									// 1�� ~ ����ѵ�
		(CashMaxIn <= DEP_MAXIN));
	else
		CashMaxIn = 1;		

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcCashMaxIn:CashMaxIn[%d]", CashMaxIn);
	return CashMaxIn;
}

// ȣ���� fnAPP_CalcCashMaxIn()�� ȣ�� �Ŀ� ����ؾ��Ѵ�.
int CTranCmn::fnAPP_CalcCashMaxInAmount()
{
	int		nMaxAmount = 0;
	int		nTemp = 1;

	// �������Ǹ� ���ɽô� �ִ�ݾ� ����������.(100����/20��)
	// �ϸ����Ǹ� ���ɽô� �ִ�ݾ� ����������.(100����/100��)
	if (((m_pProfile->DEVICE.CashHandler == OKI_SET)        ||
		 (m_pProfile->DEVICE.CashHandler == HBRM_SET))      &&	// OKI(T2ATM)
		(m_pDevCmn->ChangeBCFlag)							&&	// 5������ ��ü����
		((m_pProfile->BRM.DepositCashKind & 0x08) == 0x08)	&&	// 5������ ���ɿ���
		(m_pDevCmn->Withdraw50TFlag)						)	// ��ױ�ī��Ʈ����
		nTemp = CASH_FIFTY_THOUSAND;

	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
	{
		if (CardTran & MENU_T3)									//#N0273  
			nMaxAmount = BRM_MAXMUCASHINCNT;
		else
			nMaxAmount = BRM_MAXCASHINAMT;						// 100����
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
	{
		if (CardTran & MENU_T3)		//#N0233  ������� Ÿ�� �Ա� �ݾ�
			nMaxAmount = BRM_MAXMUCASHINCNT;
		else
			nMaxAmount = BRM_MAXCASHINAMT;
		/*
		if (m_pProfile->DEVICE.MachineType == U8100)
			nMaxAmount = BRM_MAXCASHINAMT;						// 100���� -> 500���� //#0126
		else	
		if (m_pProfile->DEVICE.MachineType == U3100K)
			nMaxAmount = BRM_MAXCASHINAMT;						// 500����	#N0233
		else
			nMaxAmount = BRM_MAXMUCASHINCNT;					// 100����
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
	}					    // 500����
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU)				// #N0181
	{
		if (CardTran & MENU_T3)									//#N0273
			nMaxAmount = BRM_MAXMUCASHINCNT;
		else
			nMaxAmount = BRM_MAXCASHINAMT_750;						// 750����  
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
	if(m_pProfile->DEVICE.ETCDevice20 == HANA)					// #N0214 �ϳ����� �Ա� 750���� ����ü 100����
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
			nMaxAmount = BRM_MAXCASHINAMT_150;						// 150����  
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
		nMaxAmount = BRM_MAXMUCASHINCNT;						// 100����

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcCashMaxIn:fnAPP_CalcCashMaxInAmount[%d]", nMaxAmount);
	return nMaxAmount;
}


// ��ǥ�Ա��ѵ����ϱ�
int CTranCmn::fnAPP_CalcCheckMaxIn()
{
	CheckMaxInCnt = Asc2Int(OpenInfo.DepCheckCount, 3);			// ��ǥ�Աݰ��ɸż�

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcCheckMaxIn:CheckMaxInCnt[%d]", CheckMaxInCnt);
	return CheckMaxInCnt;
}

/////////////////////////////////////////////////////////////////////////////
//	�Է��Լ�
/////////////////////////////////////////////////////////////////////////////
// ����Է�
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
				fnAPP_AcceptJuminNum();									// �ֹι�ȣ�Է�
			fnAPP_AcceptMuAccountNum();                             // ���¹�ȣ 
			
			if(TranCode == TC_WITHDRAW)
				fnAPP_AcceptMuConfirmNum();							// ����ü�ŷ����ι�ȣ�Է�
		}
	}

	fnAPP_AcceptMoney();		    							// ��ݱݾ��Է�
	if (!MenuIrdaMode)
	{
		if(TranCode == TC_FOREGIN)
			fnAPP_AcceptDesPassword();
		else
		if (TranCode == TC_MOBILEWITHDRAW)
		{
			fnAPP_AcceptMuConfirmNum();							// �޴��� ���ι�ȣ
			fnAPP_AcceptMPINConfirmNum();
		}
		else
			fnAPP_AcceptPassword();								// ��й�ȣ�Է�
	}


	fnAPD_CheckDeviceAction(DEV_CSH);							// �ӵ����� (�ݾ��Է��ķ� �̵�)
	m_pDevCmn->fnCSH_ReadyDispense();							// ����غ� 

	return T_OK;
}

// ��ȸ���Ǽ���
int CTranCmn::fnAPP_AcceptInq()
{
	if((m_pProfile->DEVICE.ETCDevice20 == KB) && (PbTran & MENU_PB))
		memset(Accept.PbConfirmNum, '0', sizeof(Accept.PbConfirmNum));
	
	if(TranCode == TC_FOREGIN)
		fnAPP_AcceptDesPassword();
	else
		fnAPP_AcceptPassword();								

	if (TranCode == TC_INQUIRYA || TranCode == TC_INQSTOCK)		// ���ݱ����ȸ
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

// ��ȸ���Է�
int CTranCmn::fnAPP_AcceptInqDate()
{
	CString strReturn("");

	memset(Accept.InquiryDate, ' ', sizeof(Accept.InquiryDate));
	memset(Accept.InquiryDateEnd, ' ', sizeof(Accept.InquiryDateEnd));

	if(TranCode == TC_INQSTOCK)					// û�೻����ȸ �Ⱓ����
	{
		m_pDevCmn->fnSCR_DisplayScreen(961, K_30_WAIT, PIN_MENU_MODE);
		m_pDevCmn->fnSCR_DisplayImage(7, FALSE);
	}
	else										// �ŷ�������ȸ �Ⱓ����
		m_pDevCmn->fnSCR_DisplayScreen(423, K_30_WAIT, PIN_MENU_MODE);

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == "����")					// ����
		Accept.InQuiryCycle = '1';
	else
	if (strReturn == "�ֱ�1����")				// �ֱ�1����
		Accept.InQuiryCycle = '2';
	else
	if (strReturn == "�ֱ�15��")				// �ֱ�15��
		Accept.InQuiryCycle = '3';
	else
	if (strReturn == "�ֱ�30��")				// �ֱ�30��
		Accept.InQuiryCycle = '4';
	else
	if (strReturn == "�ֱ�45��")				// �ֱ�45��
		Accept.InQuiryCycle = '5';
	else
	if (strReturn == "�ֱ�60��")				// �ֱ�60��
		Accept.InQuiryCycle = '6';
	else
	if (strReturn == "��Ÿ")					// ��Ÿ
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

// ��ü�Է�
int CTranCmn::fnAPP_AcceptTrans()
{
	if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
	{
		if(TranCode == TC_TRANSFER)
			fnAPP_AcceptTranChoice();
	}

	if(TranCode == TC_SVCTRAN1)
	{
		fnAPP_AcceptTMoney();									// ��ü�ݾ��Է�
		fnAPP_AcceptPassword();								
	}
	else
	if(TranCode == TC_SVCTRAN)									//#N0181
	{
		fnAPP_AcceptInAccountNum();								// ������¹�ȣ�Է�
		fnAPP_AcceptTMoney();									// ��ü�ݾ��Է�
		fnAPP_AcceptPassword();								
	}
	else
	{
		//#0038
		if (MenuAvail & MENU_MU)
		{
//			memcpy(Accept.BankNum, KIUBANKID, sizeof(Accept.BankNum));		
			if(m_pProfile->DEVICE.ETCDevice20 != HANA)				//#N0214
				fnAPP_AcceptJuminNum();									// �ֹι�ȣ�Է�
			fnAPP_AcceptMuAccountNum();                             // ���¹�ȣ 			
			fnAPP_AcceptMuConfirmNum();								// ����ü�ŷ����ι�ȣ�Է�
		}
		else
		{
			fnAPP_AcceptPbConfirmNum();
		}

		fnAPP_AcceptBankNum();									// �����ȣ�Է�
		fnAPP_AcceptInAccountNum();								// ������¹�ȣ�Է�
		fnAPP_AcceptTMoney();									// ��ü�ݾ��Է�
		//fnAPP_AcceptPbConfirmNum();
		fnAPP_AcceptPassword();								
	}
	
	return T_OK;
}

// �Ա��Է�
int CTranCmn::fnAPP_AcceptDep()
{

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�Ա����� : %s", fnAPP_Get_BankName(CardBank)));
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�Ա����� : %d", CardBank));

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
		fnAPP_AcceptJuminNum();									// �ֹι�ȣ�Է�
		fnAPP_AcceptMuAccountNum();                             // ���¹�ȣ 			
//		fnAPP_AcceptMuConfirmNum();								// ����ü�ŷ����ι�ȣ�Է�
		fnAPP_AcceptTelNum(); //#0107 - #0111 ***
		if (m_pProfile->DEVICE.ETCDevice20 == KIUP  || m_pProfile->DEVICE.ETCDevice20 == HANA)  //#N0201 ����ü �۱��� �Է� ��ġ ����
		{
			fnAPP_AcceptRemitName(); //#0101 -> #0107 ** (��������)
		}
	}


	//������ι�ȣ -> #0035
	//fnAPP_AcceptPbConfirmNum();
	fnAPP_AcceptPassword();

	memcpy(Accept.BankNum, &pCardData3->BankNo, 3);

	fnAPP_InitializeSerialTranDS();								// ���Ӱŷ������ʱ�ȭ
	Accept.MoneyInKind = TRAN_DEP_CASH;

	m_pDevCmn->fnAPL_SetProcCount('3');						// ���༳��
	fnAPP_SendHost();										// �۽�
	fnAPP_RecvHost();										// ����
	if (m_pDevCmn->TranResult)								// HOST OK
	{
		m_pDevCmn->fnAPL_SetProcCount('5');					// ���༳��
		fnAPP_PMEAcceptDepConfirm();					// Ÿ���Ա���ȸ Ȯ��ȭ��
		/*
		if ((m_pProfile->DEVICE.ETCDevice20 == KIUP) && (MenuAvail & MENU_MU))
		{
			fnAPP_AcceptRemitName(); //#0101 -> #0107 ** (��������)
		}
		*/
		AddSerialFlg = FALSE;		
		TranCode2 = TRANID_4410;
		TranCode2Save = TRANID_4410;
	}
	else
	{
		m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG

		fnAPP_PSPProc(DEV_JPR);								// ��������Ʈ
		fnAPP_PMDProc();									// �޼���ǥ��
		fnAPP_PSPProc(DEV_SPR);								// ������Ʈ

		fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU );	
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU , FALSE);
		fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU);
		fnAPP_TerminateProc(T_CANCEL);
	}


	CancelDepPrintFlg = FALSE;									// ����Ա�����Flg(�ʱ�:���๫)
//	CancelDepSendFlg = TRUE;									// ����Ա�ó��Flag(�ʱ�:���๫) Set
	fnAPD_CheckDeposit();										// �Աݼ�ǥ

	RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP", 1);;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�Աݰ�� : ����"));
	fnAPD_CashDeposit();										// �Ա�����
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�Աݰ�� : �Ϸ�"));
	RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP", 0);

	AddString(Accept.CheckMoney, Accept.CashMoney, 15, Accept.Money);

	fnAPP_AcceptMoneyInConfirm();								// �Աݱݾ�Ȯ��


	return T_OK;
}

int CTranCmn::fnAPP_AcceptDep_KB()  //#N0226
{

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�Ա����� : %s", fnAPP_Get_BankName(CardBank)));
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�Ա����� : %d", CardBank));

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
		fnAPP_InitializeSerialTranDS();							// ���Ӱŷ������ʱ�ȭ
		Accept.MoneyInKind = TRAN_DEP_CASH;
	}
	else
	{
		fnAPP_AcceptMoneyInKind();									// �Աݱ�������
	}

	if (PbTran & MENU_PB)
		m_pDevCmn->fnSCR_DisplayScreen(805);			// �б�(����)

	CancelDepPrintFlg = FALSE;									// ����Ա�����Flg(�ʱ�:���๫)
//	CancelDepSendFlg = TRUE;									// ����Ա�ó��Flag(�ʱ�:���๫) Set
	fnAPD_CheckDeposit();										// �Աݼ�ǥ

	RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP", 1);;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�Աݰ�� : ����"));
	fnAPD_CashDeposit();										// �Ա�����
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�Աݰ�� : �Ϸ�"));
	RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP", 0);

	AddString(Accept.CheckMoney, Accept.CashMoney, 15, Accept.Money);

	fnAPP_AcceptMoneyInConfirm();								// �Աݱݾ�Ȯ��

	m_pDevCmn->fnAPL_SetProcCount('3');						// ���༳��		
	TranCode2 = TRANID_4410;
	TranCode2Save = TRANID_4410;

	return T_OK;
}

// ����ī�弱�� (��/��)
int	CTranCmn::fnAPP_MuAcceptCardChoice()
{
	CString strReturn("");


	m_pDevCmn->fnSCR_DisplayScreen(434, K_30_WAIT, PIN_MENU_MODE);
																
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == "�������ձ�������")
		Accept.MuBankTranBrandFlag = TYJSBANK;
	else
	if (strReturn == "��������")
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


// ����ü���¹�ȣ�Է�
int CTranCmn::fnAPP_AcceptMuAccountNum()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.MuAccountNumSize = 0;								// ����ü���¹�ȣ����
	memset(Accept.MuAccountNum, ' ', sizeof(Accept.MuAccountNum));
																// ����ü���¹�ȣ
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
	if ((!strReturn.GetLength())	||							// �ڷ����
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

// ����ü�ŷ����ι�ȣ�Է�
int CTranCmn::fnAPP_AcceptMuConfirmNum()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		nCnt = 0;
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PbConfirmNum, '0', sizeof(Accept.PbConfirmNum));		
																// ����ü�ŷ����ι�ȣ
/////////////////////////////////////////////////////////////////////////////
	if (MenuAvail & MENU_MU)   // ����ü
	{							
		if((TranCode == TC_MOBILEWITHDRAW) && (m_pProfile->DEVICE.ETCDevice20 == KIUP))
			ScrNum = 3802;											// ����ü�ŷ����ι�ȣ 
		else
			ScrNum = 215;											// #N0182										// ����ü�ŷ����ι�ȣ 
	}

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	while (TRUE)
	{
		if(nCnt > 2) fnAPP_CancelProc(T_INPUTOVER);
		// ����ŷ����ι�ȣ 6�ڸ� Ȯ��
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
		else														// �ڷ����   //����ü �̿��ڹ�ȣ 5�ڸ� ����
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
			Accept.PbConfirmNumSize = strReturn.GetLength();		// ����ŷ����ι�ȣ 6�ڸ� Ȯ��
			memcpy(Accept.PbConfirmNum, strReturn.GetBuffer(0), Accept.PbConfirmNumSize);
			break;
		}
	}

	return T_OK;
}


// ���Ű���¹�ȣ�Է�
int CTranCmn::fnAPP_AcceptLostAccountNum()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.LostAccountNumSize = 0;								// �нǰ��¹�ȣ����
	memset(Accept.LostAccountNum, ' ', sizeof(Accept.LostAccountNum));
																// �нǰ��¹�ȣ
/////////////////////////////////////////////////////////////////////////////

	if (TranCode == TC_LOSTCARD)								// ī����Ű�
		ScrNum = 413;
	else
	if (TranCode == TC_LOSTPB)									// ������Ű�
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
	if ((!strReturn.GetLength())	||							// �ڷ����
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

// ��й�ȣ�Է�
int CTranCmn::fnAPP_AcceptPassword()
{
	int		ScrNum = 0;
	CString strReturn("");
	int			nCnt = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PassWord, 0, sizeof(Accept.PassWord));		// ��й�ȣ
	memset(Accept.PassWordEnc, ' ', sizeof(Accept.PassWordEnc));		// ��й�ȣ
/////////////////////////////////////////////////////////////////////////////

	if(TranProc == TRAN_DEP)
	{
		memcpy(Accept.PassWord, ZERO4, 4);
		fnAPD_FICGetEncipher();								// ��й�ȣ ��ȣȭ

		return T_OK;
	}

	ScrNum = 201;
	
	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnAPL_CaptureFaceSave(3, FALSE, fnAPP_GetCaptureData());		// ���Կ�


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
			if ((strReturn.GetLength() != sizeof(Accept.PassWord))	    ||	// �ڷ����
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
	if (((MenuFICMode)	||									// ����IC�ŷ�	2004.02.03
		(MenuEPBMode)) &&
		!(EMVReadFlg))
	{
	
		
		if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 823)
			m_pDevCmn->fnSCR_DisplayScreen(823);			// �б�(ī��)
	
		fnAPD_FICGetEncipher();								// ��й�ȣ ��ȣȭ
/*
		fnAPD_MaterialOut(DEV_MCU);							// Card Out
		fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract
*/
	}

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��й�ȣ : �Է¿Ϸ� (%d)", strReturn.GetLength()));
	m_pDevCmn->fnAPL_CaptureFaceSave(4, FALSE, fnAPP_GetCaptureData());		// ���Կ�


	return T_OK;
}
// ����ŷ����ι�ȣ�Է�
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
																// ����ŷ����ι�ȣ
	Accept.PbConfirmNumSize = 0;								// ����ŷ����ι�ȣ����	����ŷ����ι�ȣ 6�ڸ� Ȯ��
/////////////////////////////////////////////////////////////////////////////
//#0045
	if((m_pProfile->DEVICE.ETCDevice20 == NONGHYUP) || 
	   (m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	||
	   (m_pProfile->DEVICE.ETCDevice20 == KYONGNAM))      //#N0181 -> #N0282
		return T_OK;

	if (MenuAvail & MENU_PB)									// ����
		ScrNum = 213;											// ����ŷ����ι�ȣ�Է�

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);


	if(m_pProfile->DEVICE.ETCDevice20 == KIUP || m_pProfile->DEVICE.ETCDevice20 == HANA)
		nGlen = 5;
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KEB)
		nGlen = 6;
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KB)					//#N0239 ���� ���� ��й�ȣ 4~6
		nGlen = 4;
	else
		nGlen = 6;



	while (TRUE)
	{
		if(nCnt > 2) fnAPP_CancelProc(T_INPUTOVER);
		// ����ŷ����ι�ȣ 6�ڸ� Ȯ��
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
		else														// �ڷ����
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
			Accept.PbConfirmNumSize = strReturn.GetLength();		// ����ŷ����ι�ȣ 6�ڸ� Ȯ��
			memcpy(Accept.PbConfirmNum, strReturn.GetBuffer(0), Accept.PbConfirmNumSize);

			if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
			{
				//����ŷ����ι�ȣ 6�ڸ� Ȯ�� ��������������������������
				if (Accept.PbConfirmNumSize == 5)
					fnAPP_AccpetPbConfirmMSG();
				// ��������������������������������������������������������������������
			}
			break;
		}
	}

	return T_OK;
}

// ����ŷ��й�ȣ�Է�
int CTranCmn::fnAPP_AcceptTelPassword()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PbPassWord, '0', sizeof(Accept.PbPassWord));	// ����ŷ��й�ȣ
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
	if ((strReturn.GetLength() != sizeof(Accept.PbPassWord))	||	// �ڷ����
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.PbPassWord, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// ����ü�ŷ���й�ȣ�Է�(������)
int CTranCmn::fnAPP_AcceptMuPassword()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.MuPassWord, '0', sizeof(Accept.MuPassWord));	// ����ü��й�ȣ
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
	if ((strReturn.GetLength() != sizeof(Accept.MuPassWord))  ||// �ڷ����
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.MuPassWord, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// ��ݱݾ��Է�
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
	
	if (!fnAPP_CalcMoneyMaxOut())								// �����ѵ��ݾױ��ϱ�
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��ݺҰ� : BRM ���Ұ� ����"));

		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "����� �Ұ� �մϴ�");
		else
			fnAPP_CancelProc(T_MSG, "Withdrawal is not valid");
	}

	switch(m_pDevCmn->AtmDefine.ETCDevice20)
	{
	case KIUP:														
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;					// �ִ����ݾ�(��������)
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
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;					// �ִ����ݾ�(��������)	#N0276
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
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT_70;				// �ִ����ݾ�(��������)
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
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// �ִ����ݾ�(100����) //#N0181
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
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// �ִ����ݾ�(100����) //#N0192
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
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// �ִ����ݾ�(100����) //#N0214
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
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// �ִ����ݾ�(100����) //#N0215
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
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// �ִ����ݾ�(100����) //#N0219
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
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT_70;				// �ִ����ݾ�(��������) -> //#N0226
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
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT_70;				// �ִ����ݾ�(70����) //#N0238
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
		MONEY_MAXOUTCNT	= BRM_MONEY_MAXOUT;				// �ִ����ݾ�(100����) //#N0219

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
		
		if ((MAXCASHOUTCNT >= 1)	&&							// 1�� ~ ����ѵ�
			(MAXCASHOUTCNT <= iMax50TCashOut))
			;
		else MAXCASHOUTCNT = 0;									// �ݾ��̻�� 0����
	}
	else
	{
		MAXCASHOUTCNT = MONEY_MAXOUTCNT;
		if ((MAXCASHOUTCNT >= 1)	&&							// 1�� ~ ����ѵ�
			(MAXCASHOUTCNT <= MONEY_MAXOUTCNT))
			;
		else MAXCASHOUTCNT = 0;									// �ݾ��̻�� 0����
	}

	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
		CheckValue = CHECK_MILLION;								// 100����
	else
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
		CheckValue = CHECK_HUNDRED_THOUSAND;					// 10����
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
			
		MONEY_MAXOUT = BRM_MONEY_MAXOUT_50;  //#N0167  ���� Ÿ�� ���� �ݾ��� 50���� ũ�� �ٷ� ��Ÿ�ݾ� �Է����� ǥ�õ� 
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
	
	GetCase = 0;												// ���ݼ���
	Accept.CheckCount = 0;										// ��ǥ�ż� �ʱ�ȭ

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
		case 0:												// �ݾ׼���
			GetCase = fnAPP_SelectMoneyCase0(WithMsg);
			break;
		case 1:												// �ݾ��Է�(����)
			GetCase = fnAPP_InputMoneyCase1(WithMsg);
			break;
		case 2:												// ��������
			GetCase = fnAPP_SelectKindofMoneyCase2(CheckValue);	// 100���� ��ǥ����
			break;
		case 3:												// ��ǥ����&��ǥ�ݾ��Է�
			GetCase = fnAPP_SelectCheckCase3(WithMsg);
			break;
		case 4:												// ��ǥ�Է�(�ּ�~�ִ�)
			GetCase = fnAPP_InputCheckCase4(CheckValue);	// 100���� ��ǥ����
			break;
		case 7:
			if(m_pProfile->DEVICE.ETCDevice20 == HANA)									// ������ ����ż�����(�ڵ�)	#N0214
				GetCase = fnAPP_Input50TCashMoney_HANA();
			else
				GetCase = fnAPP_Input50TCashMoney();
			break;
		case 5:												// �ݾ�Ȯ��
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

			// �ݾ��ڷ��հ����			
			if ((Asc2Int(Accept.CashMoney, 11) + Asc2Int(Accept.CheckMoney, 11)) != Asc2Int(Accept.Money, 11))					
				fnAPP_CancelProc(T_PROGRAM);

			if ((m_pDevCmn->ChangeBCFlag)		&&		
				(m_pDevCmn->Withdraw50TFlag))		
			{
				if (((Asc2Int(Accept.Cash10T_Money, 11) + (Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND)) > 
					CashMaxOutCnt)	||
					((Asc2Int(Accept.Cash10T_Money, 11) + (Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND)) > 
					BRM_MAXCASHOUTCNT))
					fnAPP_CancelProc(T_PROGRAM);		// (�����Ǹż�+5�����Ǹż�) > �����ݱݾ׸ż� üũ
														// (�����Ǹż�+5�����Ǹż�) > �����ִ����ż� üũ 
			}
			else
			{
				if ((Asc2Int(Accept.CashMoney, 11) > CashMaxOutCnt)		|| 
					(Asc2Int(Accept.CashMoney, 11) > BRM_MAXCASHOUTCNT))
					fnAPP_CancelProc(T_PROGRAM);		// �ݾ��ڷ����ݸż�����
			}

			if ((m_pDevCmn->Withdraw50TFlag)            &&					
				((WithAvail == TRAN_WITH_50TCASH)		||
				(WithAvail == TRAN_WITH_50TCASHCHECK)))
			{						
				if (((Asc2Int(Accept.CashMoney, sizeof(Accept.CashMoney)) % CASH_FIFTY_THOUSAND) != 0) ||		
					(Asc2Int(Accept.Cash10T_Money, sizeof(Accept.Cash10T_Money)) != 0))
					fnAPP_CancelProc(T_MSG, "�Է±ݾ��� 5���� ������ �����մϴ�.");						
			}
			
			if ((Asc2Int(Accept.Cash50T_Money, sizeof(Accept.Cash50T_Money)) % CASH_FIFTY_THOUSAND) != 0)						
				fnAPP_CancelProc(T_MSG, "�Է±ݾ��� 5���� ������ �����մϴ�.");

			// 100������ ���� �ݾ��ڷ���� Start
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
			{
				if ((Asc2Int(Accept.CheckMoney, 9) > CheckMaxOutCnt)	||
					(Asc2Int(Accept.CheckMoney, 9) > BRM_MAXCHECKOUTCNT_100CHECK))
					fnAPP_CancelProc(T_PROGRAM);				// �ݾ��ڷ��ǥ�ż�����
			}
			else
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
			{
				if ((Asc2Int(Accept.CheckMoney, 10) > CheckMaxOutCnt)	||
					(Asc2Int(Accept.CheckMoney, 10) > BRM_MAXCHECKOUTCNT))
					fnAPP_CancelProc(T_PROGRAM);				// �ݾ��ڷ��ǥ�ż�����
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
						fnAPP_CancelProc(T_PROGRAM);			// �ݾ��ڷ����ݸż�����(������)						
				}
				else
				if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
				{
					if ((((Asc2Int(Accept.Cash10T_Money, 11) + (Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND)) + Asc2Int(Accept.CheckMoney, 3)) >
						 MoneyMaxOutCnt)	||
						(((Asc2Int(Accept.Cash10T_Money, 11) + (Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND)) + Asc2Int(Accept.CheckMoney, 3)) >
						BRM_MONEY_MAXOUTCNT))
						fnAPP_CancelProc(T_PROGRAM);			// �ݾ��ڷ����ݸż�����(������)						
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
						fnAPP_CancelProc(T_PROGRAM);			// �ݾ��ڷ�ż�����
				}
				else
				if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
				{
				if (((Asc2Int(Accept.CashMoney, 11) + Asc2Int(Accept.CheckMoney, 10)) >
					MoneyMaxOutCnt)										||
					((Asc2Int(Accept.CashMoney, 11) + Asc2Int(Accept.CheckMoney, 10)) >
					BRM_MONEY_MAXOUTCNT))							
					fnAPP_CancelProc(T_PROGRAM);			// �ݾ��ڷ�ż�����
				}
			}

			// �ݾ��ڷ��հ����			
			if ((Asc2Int(Accept.CashMoney, 11) + Asc2Int(Accept.CheckMoney, 11)) != Asc2Int(Accept.Money, 11))					
				fnAPP_CancelProc(T_PROGRAM);

			if (((Asc2Int(Accept.CashMoney, 11) + (Asc2Int(Accept.CheckMoney, 10) * 10)) >  
				MoneyMaxOut)										||
				((Asc2Int(Accept.CashMoney, 11) + (Asc2Int(Accept.CheckMoney, 10) * 10)) >
				BRM_MONEY_MAXOUT))							
				fnAPP_CancelProc(T_PROGRAM);				// �ݾ��ڷ�ݾװ���
															
			GetCase = 9;									// ����

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

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�ݾ��Է� : %d����", Asc2Int(Accept.Money, sizeof(Accept.Money))/10000));

	return T_OK;
}
// �ݾ��Է�, case 1		
int	CTranCmn::fnAPP_InputMoneyCase_Etc()	
{
	int	GetCase = 9;
	int ScrNum;
	CString strReturn;

	ScrNum = 302;	
	m_pDevCmn->fnSCR_DisplayNumeric(1, BRM_MONEY_MAXOUT);	// ����������
	m_pDevCmn->fnSCR_DisplayNumeric(2, BRM_MONEY_MAXOUT);	// �����ִ����ż�
	m_pDevCmn->fnSCR_DisplayNumeric(3, BRM_MONEY_MAXOUT);	// �ִ����ݾ�

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT);		// �ݾ��Է�(����)
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
	if ((strReturn.GetLength() != 15)		||				// �ݾ��ڷ����
		(!IsNum(strReturn.GetBuffer(0), 15))	||		
		(IsZero(strReturn.GetBuffer(0), 15))	||
		(!IsZero(&(strReturn.GetBuffer(0)[11]), 4)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	if (Asc2Int(strReturn.GetBuffer(0), 11) > BRM_MONEY_MAXOUT)
	{															// ��ݱݾ��Է��ʰ���
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "�Է±ݾ� �ʰ��Դϴ�");
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


// �ݾ׼���(��ư), case 0 
int	CTranCmn::fnAPP_SelectMoneyCase0(char * WithMsg)
{
	int	GetCase = 9;
	int ScrNum;
	CString strReturn;

	if ((m_pDevCmn->Withdraw50TFlag)		&&					// V05-01-01-#01
	    ((WithAvail == TRAN_WITH_50TCASH)	||					// 5�����Ǹ� ��ݰ���
		 (WithAvail == TRAN_WITH_50TCASHCHECK)))				// 5����+��ǥ
	{
		GetCase = 1;
		return GetCase;
	}

	// 100����ݰ���(�����̰� ���������ΰ��)											
	if(WithAvail == TRAN_WITH_CHECK)
		ScrNum = 306;
	else
		ScrNum = 301;										// �ݾ׼���

	//#N0249
	if( MoneyMaxOut < BRM_MONEY_MAXOUT_50)			//50���� �̸�
		m_pDevCmn->fnSCR_DisplayImage(7, FALSE);
	if( MoneyMaxOut < BRM_MONEY_MAXOUT_70)			//70���� �̸�
		m_pDevCmn->fnSCR_DisplayImage(8, FALSE);
	if( MoneyMaxOut < BRM_MONEY_MAXOUT)				//100���� �̸�
		m_pDevCmn->fnSCR_DisplayImage(9, FALSE);
	
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT , PIN_MENU_MODE1);   
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	GetCase = 2;												// ��������
	if (strReturn == "1����")
		memcpy(Accept.Money, "000000000010000", 15);
	else
	if (strReturn == "2����")
		memcpy(Accept.Money, "000000000020000", 15);
	else
	if (strReturn == "3����")
		memcpy(Accept.Money, "000000000030000", 15);
	else
	if (strReturn == "5����")
		memcpy(Accept.Money, "000000000050000", 15);
	else
	if (strReturn == "7����")
		memcpy(Accept.Money, "000000000070000", 15);
	else
	if (strReturn == "10����")
		memcpy(Accept.Money, "000000000100000", 15);
	else
	if (strReturn == "15����")
		memcpy(Accept.Money, "000000000150000", 15);
	else
	if (strReturn == "20����")
		memcpy(Accept.Money, "000000000200000", 15);
	else
	if (strReturn == "30����")
		memcpy(Accept.Money, "000000000300000", 15);
	else
	if (strReturn == "40����")
		memcpy(Accept.Money, "000000000400000", 15);
	else
	if (strReturn == "50����")
		memcpy(Accept.Money, "000000000500000", 15);
	else
	if (strReturn == "60����")
		memcpy(Accept.Money, "000000000600000", 15);
	else
	if (strReturn == "70����")
		memcpy(Accept.Money, "000000000700000", 15);
	else
	if (strReturn == "80����")
		memcpy(Accept.Money, "000000000800000", 15);
	else
	if (strReturn == "90����")
		memcpy(Accept.Money, "000000000900000", 15);
	else
	if (strReturn == "100����")
		memcpy(Accept.Money, "000000001000000", 15);
	else
	if (strReturn == "��Ÿ")						// ��Ÿ
		GetCase = 1;								// �ݾ��Է�(����)
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

// �ݾ��Է�, case 1		
int	CTranCmn::fnAPP_InputMoneyCase1(char * WithMsg)	
{
	int	GetCase = 9;
	int ScrNum;
	CString strReturn;
	int		nCurrCheckKind = m_pDevCmn->fnAPL_GetKindOfOutCheck();


	if ((m_pDevCmn->Withdraw50TFlag)		&&			
		((WithAvail == TRAN_WITH_50TCASH)	||					// 5�����Ǹ� ��ݰ���
		 (WithAvail == TRAN_WITH_50TCASHCHECK)))				// 5����+��ǥ
	{
		if (TranCode == TC_MOBILEWITHDRAW) //#N0182
			ScrNum = 325;							
		else
			ScrNum = 325;
								
		m_pDevCmn->fnSCR_DisplayNumeric(1, MoneyMaxOut);	// ����������
		m_pDevCmn->fnSCR_DisplayNumeric(2, CashMaxOutCnt * CASH_FIFTY_THOUSAND);	// �����ִ����ż�
		m_pDevCmn->fnSCR_DisplayNumeric(3, CheckMaxOutCnt * 10);	// �ִ����ݾ�

	}
	else
	{
		if (TranCode == TC_MOBILEWITHDRAW) //#N0182
			ScrNum = 3801;							
		else
			ScrNum = 302;
		
		if (nCurrCheckKind == CHECKKIND_MILLION)		
		{
			m_pDevCmn->fnSCR_DisplayNumeric(1, MoneyMaxOut);	// ����������
			m_pDevCmn->fnSCR_DisplayNumeric(2, CashMaxOutCnt);	// �����ִ����ż�
			m_pDevCmn->fnSCR_DisplayNumeric(3, CheckMaxOutCnt * 100);	// �ִ����ݾ�
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayNumeric(1, MoneyMaxOut);	// ����������
			m_pDevCmn->fnSCR_DisplayNumeric(2, CashMaxOutCnt);	// �����ִ����ż�
			m_pDevCmn->fnSCR_DisplayNumeric(3, CheckMaxOutCnt * 10);	// �ִ����ݾ�
		}
	}

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);		// �ݾ��Է�(����)  #N0171
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
	if ((strReturn.GetLength() != 15)		||				// �ݾ��ڷ����
		(!IsNum(strReturn.GetBuffer(0), 15))	||		
		(IsZero(strReturn.GetBuffer(0), 15))	||
		(!IsZero(&(strReturn.GetBuffer(0)[11]), 4)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	if (Asc2Int(strReturn.GetBuffer(0), 11) > MoneyMaxOut)
	{															// ��ݱݾ��Է��ʰ���
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "�Է±ݾ� �ʰ��Դϴ�");
		else
			fnAPP_CancelProc(T_MSG, "Withdrawal limit exceeded", "Please enter amount again");
	}
	else
	if ((Asc2Int(strReturn.GetBuffer(0), 10) + Asc2Int(&(strReturn.GetBuffer(0)[10]), 1)) > MoneyMaxOutCnt)
	{															// ��) 693����
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "�Է±ݾ� �ż� �ʰ��Դϴ�");
		else
			fnAPP_CancelProc(T_MSG, "Withdrawal limit exceeded", "Please enter amount again");
	}
	else
	{
		memcpy(Accept.Money, strReturn.GetBuffer(0), 15);
		GetCase = 2;										// ��������
	}

	return GetCase;
}


// ��������, case 2		
// 100���� ��ǥ���� Parameter ����
int	CTranCmn::fnAPP_SelectKindofMoneyCase2(int CheckValue)
{
	int	GetCase = 9;
	int ScrNum;
	CString strReturn;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SelectKindofMoneyCase2[CheckMaxOutCnt:%d],[Accept.Money:%d],[WithAvail:%x]", 
																CheckMaxOutCnt,  Asc2Int(Accept.Money, 11), WithAvail);

	if ((m_pDevCmn->EarPhoneInserted)				||			// �����ATM - �̾��� ���Խ� ��ǥ��� �Ұ� ����
		(!CheckMaxOutCnt) ||									// ��ǥ�Ұ�
		(Asc2Int(Accept.Money, 10) < CheckValue))				// �������̸� �ݾ�(10/100����)
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
		else if (Asc2Int(Accept.Money, 15) < 50000)				// 5���� �̸�
		{
			memset(Accept.Cash50T_Money, '0', sizeof(Accept.Cash50T_Money));
			memcpy(Accept.Cash10T_Money, Accept.CashMoney, sizeof(Accept.Cash10T_Money));
			GetCase = 5;										// �ݾ�Ȯ��
		}
		else													// 5~9���� �Ǵ� 5~99����
		{				
			if ((WithAvail & TRAN_WITH_50TCASH))
				GetCase = 7;								// ���ݹ���ż�(�ڵ�)
			else
			{											
				memcpy(Accept.Cash10T_Money, Accept.CashMoney, sizeof(Accept.Cash10T_Money));
				GetCase = 5;									
			}
		}
		return GetCase;
	}

	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)		// 10����/100���� ��ǥ���ÿ� ���� ȭ�� ����
		ScrNum = 312;												// ��ݱ�������
	else
		ScrNum = 312;

	if ((m_pDevCmn->Withdraw50TFlag)			&&				
		((WithAvail == TRAN_WITH_50TCASH)		||
		(WithAvail == TRAN_WITH_50TCASHCHECK))	)
	{
		if (Asc2Int(Accept.Money, 11) > CashMaxOutCnt*CASH_FIFTY_THOUSAND)
			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);			// �������ݻ���
	}
	else
	{
		if (Asc2Int(Accept.Money, 11) > CashMaxOutCnt)
			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);			// �������ݻ���
	}
																// 100���� ��ǥ����

	if (CheckValue == 0)										// CodeSonar
	{
		CheckValue = CHECK_HUNDRED_THOUSAND;
	}
	else
	if ((Asc2Int(Accept.Money, 10)%(CheckValue)) != 0)
		m_pDevCmn->fnSCR_DisplayImage(2, FALSE);				// ���׼�ǥ����
	if (Asc2Int(Accept.Money, 10) > (CheckMaxOutCnt * CheckValue))
		m_pDevCmn->fnSCR_DisplayImage(2, FALSE);				// ���׼�ǥ����
	if (Asc2Int(Accept.Money, 10) == CheckValue)							
		m_pDevCmn->fnSCR_DisplayImage(3, FALSE);				// ����+��ǥ����


	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT);								
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == "����")									// ��������
	{	

		memcpy(Accept.CashMoney, Accept.Money, sizeof(Accept.CashMoney));

		if ((m_pDevCmn->Withdraw50TFlag)		&&			
		    ((WithAvail == TRAN_WITH_50TCASH)	||
			 (WithAvail == TRAN_WITH_50TCASHCHECK)))
		{
			memcpy(Accept.Cash50T_Money, Accept.CashMoney, sizeof(Accept.Cash50T_Money));
			memset(Accept.Cash10T_Money, '0', sizeof(Accept.Cash10T_Money));
			GetCase = 5;										// �ݾ�Ȯ��
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
					GetCase = 5;								// ���ݹ���ż�(�ڵ�)
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
			GetCase = 5;										// �ݾ�Ȯ��
		}
	}
	else
	if (strReturn == "��ǥ")									// ���׼�ǥ
	{	
		memcpy(Accept.CheckMoney, Accept.Money, sizeof(Accept.CheckMoney));
		GetCase = 5;											// �ݾ�Ȯ��
	}
	else
	if (strReturn == "��ǥ+����")								// ��ǥ����
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

// ��ǥ����, �Է�, case 3					
int	CTranCmn::fnAPP_SelectCheckCase3(char * WithMsg)
{
	int	GetCase = 9;
	int ScrNum = 204;
	CString strReturn;

	int		GetMoneyMaxOut;

	// ��ǥ������ ���� ȭ��б� �ʿ� Start
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
	// ��ǥ������ ���� ȭ��б� �ʿ� End

	if (!CashMaxOutCnt)											// ���ݺҰ�
		GetMoneyMaxOut = MoneyMaxOut;
	else GetMoneyMaxOut = Asc2Int(Accept.Money, 11);

	fnAPP_CalcCheckLowMaxOutCnt(GetMoneyMaxOut);	// �ּҼ�ǥ�ż����ϱ�
	fnAPP_CalcCheckHighMaxOutCnt(GetMoneyMaxOut);	// �ִ��ǥ�ż����ϱ�

//	ScrNum = 204;												// ��ǥ�ݾ��Է�
	m_pDevCmn->fnSCR_DisplayNumeric(1, GetMoneyMaxOut);			// �ŷ��ݾ�
	m_pDevCmn->fnSCR_DisplayNumeric(2, CheckLowMaxOutCnt);		// ��ǥ�ִ����ż�
	m_pDevCmn->fnSCR_DisplayNumeric(3, CheckHighMaxOutCnt);		// ��ǥ�ִ����ż�
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT);			// ��ǥ�ݾ��Է�(����)
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
	if ((strReturn.GetLength() != 3)		||					// ��ǥ�ż��ڷ����
		(!IsNum(strReturn.GetBuffer(0), 3))	||		
		(IsZero(strReturn.GetBuffer(0), 3)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	if ((Asc2Int(strReturn.GetBuffer(0))*nCheckValue) > MoneyMaxOut)// ��������	// ��ǥ������ ���� �ѵ�üũ
	{															// ��ǥ��ݱݾ��Է��ʰ���
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "��ǥ �Է±ݾ� �ʰ��Դϴ�");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
 	if (Asc2Int(strReturn.GetBuffer(0), 3) > CheckMaxOutCnt)		
	{															// ��ǥ�ż��ʰ�
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "��ǥ �Է±ݾ� �ż� �ʰ��Դϴ�");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
	{
		sprintf(Accept.CheckMoney, "%15d", Asc2Int(strReturn.GetBuffer(0))*nCheckValue*10000);		
		memcpy(Accept.Money, Accept.CheckMoney, 15);
		GetCase = 5;											// �ݾ�Ȯ��
	}

	return GetCase;
}

// ��ǥ�Է�, case 4						
// 100������ ���� Parameter ����
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

	if (!CashMaxOutCnt)								// ���ݺҰ�
		GetMoneyMaxOut = MoneyMaxOut;
	else GetMoneyMaxOut = Asc2Int(Accept.Money, 11);

	fnAPP_CalcCheckLowMaxOutCnt(GetMoneyMaxOut);	// �ּҼ�ǥ�ż����ϱ�
	fnAPP_CalcCheckHighMaxOutCnt(GetMoneyMaxOut);	// �ִ��ǥ�ż����ϱ�

	CashMaxOutCnt1 = (GetMoneyMaxOut - (CheckLowMaxOutCnt * CheckValue));	// 100������ ����
	MoneyMaxOut1 = (CheckLowMaxOutCnt * CheckValue) + CashMaxOutCnt1;		// 100������ ����
	CashMaxOutCnt2 = (GetMoneyMaxOut - (CheckHighMaxOutCnt * CheckValue));	// 100������ ����
	MoneyMaxOut2 = (CheckHighMaxOutCnt * CheckValue) + CashMaxOutCnt2;		// 100������ ����


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
		if ((CashMaxOutCnt1 >= 0)					&&	// ���⸸���ż�üũ
			(CashMaxOutCnt1 <= CashMaxOutCnt)		&&
			(CashMaxOutCnt2 >= 0)					&&
			(CashMaxOutCnt2 <= CashMaxOutCnt)		&&
			(MoneyMaxOut1 == GetMoneyMaxOut)		&&	// ����ݾ׵���üũ
			(MoneyMaxOut2 == GetMoneyMaxOut));
		else
		{
			fnAPP_CancelProc(T_PROGRAM);
		}
	}

	if (WithAvail == TRAN_WITH_CHECK)
	{
		if ((CheckLowMaxOutCnt >= 0)				&&	// �����ǥ�ż�üũ
			(CheckLowMaxOutCnt <= CheckMaxOutCnt)	&&
			(CheckHighMaxOutCnt >= 0)				&&
			(CheckHighMaxOutCnt <= CheckMaxOutCnt)	&&
			(MoneyMaxOut1 == GetMoneyMaxOut)		&&	// ����ݾ׵���üũ
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
		if ((CashMaxOutCnt1 >= 0)					&&	// ���⸸���ż�üũ
			(CashMaxOutCnt1 <= CashMaxOutCnt)		&&
			(CashMaxOutCnt2 >= 0)					&&
			(CashMaxOutCnt2 <= CashMaxOutCnt)		&&
			(CheckLowMaxOutCnt >= 0)				&&	// �����ǥ�ż�üũ
			(CheckLowMaxOutCnt <= CheckMaxOutCnt)	&&
			(CheckHighMaxOutCnt >= 0)				&&
			(CheckHighMaxOutCnt <= CheckMaxOutCnt)	&&
			(MoneyMaxOut1 == GetMoneyMaxOut)		&&	// ����ݾ׵���üũ
			(MoneyMaxOut2 == GetMoneyMaxOut));
		else
		{
			fnAPP_CancelProc(T_PROGRAM);
		}
	}

	// ��ǥ������ ���� ȭ��б� �ʿ� Start
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
		ScrNum = 305;
	else
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
		ScrNum = 305;											// ��ǥ�ż��Է�
	else
		fnAPP_CancelProc(T_INPUTOVER);
	// ��ǥ������ ���� ȭ��б� �ʿ� End
	m_pDevCmn->fnSCR_DisplayNumeric(1, GetMoneyMaxOut);			// �ŷ��ݾ�
	m_pDevCmn->fnSCR_DisplayNumeric(2, CheckLowMaxOutCnt);		// �ּҸż�
	m_pDevCmn->fnSCR_DisplayNumeric(3, CheckHighMaxOutCnt);		// �ִ�ż�
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
	if ((strReturn.GetLength() != 3)		||					// ��ǥ�ż��ڷ����
		(!IsNum(strReturn.GetBuffer(0), 3))	||		
		(IsZero(strReturn.GetBuffer(0), 3)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	if ((Asc2Int(strReturn.GetBuffer(0), 3) < CheckLowMaxOutCnt)	|| 
		(Asc2Int(strReturn.GetBuffer(0), 3) > CheckHighMaxOutCnt))
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "�Է¸ż��� �߸� �Ǿ����ϴ�");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
	{
		if (!CashMaxOutCnt)							// ���ݺҰ�
		{
			memcpy(Accept.CheckMoney, "000000000000000", 15);

			// ������ ó���б�
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
				memcpy(Accept.CheckMoney, strReturn.Right(2), 2);
			else
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
				memcpy(Accept.CheckMoney, strReturn.GetBuffer(0), 3);
			else
				fnAPP_CancelProc(T_INPUTERROR);
			// ������ ó���б�

			memcpy(Accept.Money, Accept.CheckMoney, 15);
		}
		else 
		{
			memcpy(Accept.CheckMoney, "000000000000000", 15);

			// ������ ó���б�
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
				memcpy(&Accept.CheckMoney[7], strReturn.Right(2), 2);
			else
			if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
				memcpy(&Accept.CheckMoney[7], strReturn.GetBuffer(0), 3);
			else
				fnAPP_CancelProc(T_INPUTERROR);
			// ������ ó���б�

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
					GetCase = 7;								// ���ݹ���ż�(�ڵ�)
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
			GetCase = 5;										// �ݾ�Ȯ��
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
	if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)		//�ѱݾ� ����
	{
		// 1. �ѱݾ�
		//m_pDevCmn->fnSCR_DisplayNumeric(1, Asc2Int(Accept.Money, 4));	// �ŷ��ݾ�		
		// 2. �ּҸż�
		m_pDevCmn->fnSCR_DisplayNumeric(1, ZERO);
		// 3. �ִ�ż�
		m_pDevCmn->fnSCR_DisplayNumeric(2, Asc2Int(Accept.CashMoney, 11)/CASH_FIFTY_THOUSAND);
	}
	else
	{
		// 1. �ѱݾ�
		//m_pDevCmn->fnSCR_DisplayNumeric(1, Asc2Int(Accept.Money, 4));	// �ŷ��ݾ�		
		// 2. �ּҸż�
		m_pDevCmn->fnSCR_DisplayNumeric(1, ZERO);
		// 3. �ִ�ż�
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
	if ((strReturn.GetLength() != 3)		||					// �ż��ڷ����
		(!IsNum(strReturn.GetBuffer(0), 3)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney strReturn.GetBuffer=[%s]",strReturn.GetBuffer(0));
		memcpy(Accept.Cash50T_Money, Int2Asc(Asc2Int(strReturn.GetBuffer(0)) * 50000, 15), 15);			// ������ �ŷ��ݾ�
		SubString(Accept.CashMoney, Accept.Cash50T_Money, 15, Accept.Cash10T_Money);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%15.15s] Accept.Cash50T_Money[%15.15s]", Accept.Money, Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney Accept.Cash50T_Money=[%8.8s]",Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.CashMoney[%15.15s] Accept.Cash10T_Money[%15.15s]", Accept.CashMoney, Accept.Cash10T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney Accept.CashMoney=[%8.8s]",Accept.CashMoney);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Cash10T_Money[%8.8s]",Accept.Cash10T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.CheckMoney[%15.15s]", Accept.CheckMoney);

		GetCase = 5;											// �ݾ�Ȯ��	
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
	if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)		//�ѱݾ� ����
	{
		// 1. �ѱݾ�
		//m_pDevCmn->fnSCR_DisplayNumeric(1, Asc2Int(Accept.Money, 4));	// �ŷ��ݾ�		
		// 2. �ּҸż�
		m_pDevCmn->fnSCR_DisplayNumeric(1, ZERO);
		// 3. �ִ�ż�
		m_pDevCmn->fnSCR_DisplayNumeric(2, Asc2Int(Accept.CashMoney, 11)/CASH_FIFTY_THOUSAND);
	}
	else
	{
		// 1. �ѱݾ�
		//m_pDevCmn->fnSCR_DisplayNumeric(1, Asc2Int(Accept.Money, 4));	// �ŷ��ݾ�		
		// 2. �ּҸż�
		m_pDevCmn->fnSCR_DisplayNumeric(1, ZERO);
		// 3. �ִ�ż�
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
	if ((strReturn.GetLength() != 15)		||					// �ż��ڷ����
		(!IsNum(strReturn.GetBuffer(0), 15)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney strReturn.GetBuffer=[%s]",strReturn.GetBuffer(0));
		memcpy(Accept.Cash50T_Money, strReturn.GetBuffer(0), 15);			// ������ �ŷ��ݾ�
		SubString(Accept.CashMoney, Accept.Cash50T_Money, 15, Accept.Cash10T_Money);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%15.15s] Accept.Cash50T_Money[%15.15s]", Accept.Money, Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney Accept.Cash50T_Money=[%8.8s]",Accept.Cash50T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.CashMoney[%15.15s] Accept.Cash10T_Money[%15.15s]", Accept.CashMoney, Accept.Cash10T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Input50TCashMoney Accept.CashMoney=[%8.8s]",Accept.CashMoney);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Cash10T_Money[%8.8s]",Accept.Cash10T_Money);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.CheckMoney[%15.15s]", Accept.CheckMoney);

		GetCase = 5;											// �ݾ�Ȯ��	
	}

	return GetCase;
}

// ��ü�ݾ��Է�
int CTranCmn::fnAPP_AcceptTMoney()
{
	int		ScrNum = 0;
	int		GetCase = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	fnAPP_InitializeSerialTranDS();							// ���Ӱŷ������ʱ�ȭ
/////////////////////////////////////////////////////////////////////////////

	if(TranCode == TC_SVCTRAN1 || TranCode == TC_SVCTRAN)   //#N0181  ���� ���� ��ü�ݾ�
	{
		ScrNum = 313;											// ��ü�ݾ��Է�(��/õ/��)
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);	}
	else
	{
		ScrNum = 303;											// ��ü�ݾ��Է�(��/õ/��)
		if((m_pProfile->DEVICE.ETCDevice20 == HANA) && (MenuAvail & MENU_MU))
			m_pDevCmn->fnSCR_DisplayNumeric(1, 1000000);						//�ϳ����� ����ü ��ü �ѵ� 100����. #N0214
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
	if ((strReturn.GetLength() != 15)	||						// �ڷ����
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
//	else
//	if ((ScrNum == 303)								&&			// ��ü�ݾ��Է�(����)
//		(!IsZero(&(strReturn.GetBuffer(0)[11]), 4))	)			// ������������
//		fnAPP_CancelProc(T_PROGRAM);
	else
	if (Asc2Int(strReturn.GetBuffer(0), 15) > fnAPP_CalcMoneyMaxTrans())
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "��ü �Է±ݾ� �ʰ��Դϴ�");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
	{
		memcpy(Accept.Money, strReturn.GetBuffer(0), 15);
	}

	return T_OK;
}

// �Աݱ�������
int CTranCmn::fnAPP_AcceptMoneyInKind()
{
	int		ScrNum = 311;										// �Աݱ�������
	CString strReturn("");
	CString	szOnOffString("");

/////////////////////////////////////////////////////////////////////////////
	if( (MenuAvail & MENU_MU)	&&
		(TranCode == TC_REMIT) )
		;
	else
		fnAPP_InitializeSerialTranDS();							// ���Ӱŷ������ʱ�ȭ
/////////////////////////////////////////////////////////////////////////////

	if(TranCode == TC_REMIT)									// ����ü�Ա��� ���ݸ�����
	{
		Accept.MoneyInKind = TRAN_DEP_CASH;
		return T_OK;
	}

	if (m_pDevCmn->EarPhoneInserted)							//�����ATM - ������ �����Աݸ� ������
	{
		Accept.MoneyInKind = TRAN_DEP_CASH;							
		return T_OK; 
	}

	if ((DepAvail & TRAN_DEP_CASH) != TRAN_DEP_CASH)			// �����ԱݺҰ�
		m_pDevCmn->fnSCR_DisplayImage(1, FALSE);
	if ((DepAvail & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)			// ��ǥ�ԱݺҰ�
		m_pDevCmn->fnSCR_DisplayImage(2, FALSE);
	if ((DepAvail & TRAN_DEP_BOTH) != TRAN_DEP_BOTH)			// ��ǥ�����ԱݺҰ�
		m_pDevCmn->fnSCR_DisplayImage(3, FALSE);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
																// �Աݱ�������
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == "����")									// �����Ա�
		Accept.MoneyInKind = TRAN_DEP_CASH;
	else
	if (strReturn == "��ǥ")									// ��ǥ�Ա�
		Accept.MoneyInKind = TRAN_DEP_CHECK;
	else
	if (strReturn == "����+��ǥ")								// ���ݼ�ǥ�Ա�
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

// ������Ա� Ȯ�� 
int CTranCmn::fnAPP_AcceptNhMoneyInConfirm()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		j = 1;

	m_pDevCmn->fnAPL_GetAvailDeposit();
	DepAvail = m_pDevCmn->DepAvail;								// �Աݰŷ�����

	if( Asc2Int(Accept.Money, 15) > 1000000)						// 100�����ʰ��ݾ� 	// ����Աݹ�ȯó��
		fnAPP_CancelDepRejectProc(T_MSG, "�ŷ��� ��ҵǾ����ϴ�. ", "��ȯ�� �ݾ��� Ȯ���Ͻð�, �ٽðŷ��Ͽ��ֽñ� �ٶ��ϴ�.", "����ü 1ȸ�Ա��ѵ��� 100���� �Դϴ�.");
				
	if (DepAvail == TRAN_DEP_CASH)
	{
		ScrNum = 528;
		if(Accept.InQuiryCycle == '1') //����
		{
			m_pDevCmn->fnSCR_DisplayString(j++, "****-****-****-****"); // ���¹�ȣ	
			m_pDevCmn->fnSCR_DisplayAmount(j++, Accept.CashMoney, 15); // �Աݱݾ�
		}
		else
		if(Accept.InQuiryCycle == '2') //����
		{
			m_pDevCmn->fnSCR_DisplayString(j++, "****-****-****-****"); // ���¹�ȣ
			m_pDevCmn->fnSCR_DisplayAmount(j++, Accept.CashMoney, 15); // �Աݱݾ�
		}
		else
		{
			fnAPP_CancelDepRejectProc(T_CANCEL);					// ����Աݹ�ȯ�Լ�(�������ó��)
		}
																												 
	}else
		fnAPP_CancelDepRejectProc(T_MSG, "[�ִ� �Աݰ��� �ݾ� �ʰ�]"," ", "ī��Ʈ �ݾ��� Ȯ���Ͻñ� �ٶ��ϴ�.");

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
																// �Ա�Ȯ��
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CONFIRM)
		return T_OK;
	else 
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);					// ����Աݹ�ȯ�Լ�(�������ó��)
	else
	if (strReturn == S_TIMEOVER)
	{
		return T_OK;											// �Է½ð��ʰ����Ա�ó��
	}
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);					// ����Աݹ�ȯ�Լ�(�������ó��)
	else
		fnAPP_CancelDepRejectProc(T_PROGRAM);					// ����Աݹ�ȯ�Լ�(�������ó��)

	return T_OK;
}

// �Աݱݾ�Ȯ��
int CTranCmn::fnAPP_AcceptMoneyInConfirm()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		j = 1;

	if( (Asc2Int(Accept.Money, 15) > 1000000)	&&							// 100�����ʰ��ݾ�
		((MenuAvail & MENU_MU) && (TranCode == TC_REMIT)) )					// ����ü �Աݽ� ��ȯó��
		fnAPP_CancelDepRejectProc(T_MSG, "�ŷ��� ��ҵǾ����ϴ�. ", 
										"��ȯ�� �ݾ��� Ȯ���Ͻð�, �ٽðŷ��Ͽ��ֽñ� �ٶ��ϴ�.",
										"����ü 1ȸ�Ա��ѵ��� 100���� �Դϴ�.");
																			// ����Աݹ�ȯó��
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
	
	m_pDevCmn->fnSCR_DisplayString(j++, m_RD.byDataField_017x);	// ���¹�ȣ		
	m_pDevCmn->fnSCR_DisplayAmount(j++, Accept.CashMoney, 15);	// �����Աݱݾ�

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE1);
																// �Ա�Ȯ��
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CONFIRM)
		return T_OK;
	else 
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);					// ����Աݹ�ȯ�Լ�(�������ó��)
	else
	if (strReturn == S_TIMEOVER)
	{
		return T_OK;											// �Է½ð��ʰ����Ա�ó��
	}
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);					// ����Աݹ�ȯ�Լ�(�������ó��)
	else
		fnAPP_CancelDepRejectProc(T_PROGRAM);					// ����Աݹ�ȯ�Լ�(�������ó��)

	return T_OK;
}

// �۱��θ��Է�
int CTranCmn::fnAPP_AcceptRemitName()
{
	CString strReturn("");
	BYTE	StrBuff[81] = {0, };
	BYTE	strTemp[81] = {0, };

	int		ScrNum = 0;

	int				i = 0;
	unsigned int	j = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.RemitName, ' ', sizeof(Accept.RemitName));	// �۱��θ��ʱ�ȭ
	Accept.RemitName[0] = 'N';
/////////////////////////////////////////////////////////////////////////////

	Accept.RemitNameFlag = TRUE;   //#N0201

	ScrNum = 431;
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_90_WAIT, PIN_MENU_MODE); //#0117
	strReturn = m_pDevCmn->fnSCR_GETKEYHANGUL(ScrNum , 1, StrBuff, 14, K_90_WAIT, 0, 0); // �Է��� 14 Bytes�� ����
	
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

// ���׼۱ݱݾ��Է�
int CTranCmn::fnAPP_AcceptRMoney()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.RemainRemMoney, '0', sizeof(Accept.RemainRemMoney));	
																// ���׼۱ݾ�
	memset(Accept.BalanceRemMoney, '0', sizeof(Accept.BalanceRemMoney));
																// �����Աݾ�
/////////////////////////////////////////////////////////////////////////////

	ScrNum = 304;												// ���׼۱ݱݾ��Է�
	m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.Money, 15);
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_AMOUNT_MODE);
																// �ݾ��Է�(��õ��)
	strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(15);
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);					// ����Աݹ�ȯ�Լ�(�������ó��)
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelDepRejectProc(T_TIMEOVER);					// ����Աݹ�ȯ�Լ�(�������ó��)
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);					// ����Աݹ�ȯ�Լ�(�������ó��)
	else
	if ((strReturn.GetLength() != 15)		||					// �ݾ��ڷ����
		(!IsNum(strReturn.GetBuffer(0), 15))	||		
		(IsZero(strReturn.GetBuffer(0), 15)))
		fnAPP_CancelDepRejectProc(T_INPUTERROR);
	else
	if (Asc2Int(strReturn.GetBuffer(0), 15) > Asc2Int(Accept.Money,15))
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelDepRejectProc(T_MSG, "���׼۱� �Է±ݾ� �ʰ��Դϴ�");
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

// �۱�Ȯ��
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
		m_pDevCmn->fnSCR_DisplayString(j++, GetString(Accept.RemitName, 20, 20));		// �۱���

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CONFIRM)
		return T_OK;
	else 
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);					// ����Աݹ�ȯ�Լ�(�������ó��)
	else
	if (strReturn == S_TIMEOVER)
	{
		return T_OK;											// �Է½ð��ʰ����Ա�ó��
	}
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);					// ����Աݹ�ȯ�Լ�(�������ó��)
	else
		fnAPP_CancelDepRejectProc(T_PROGRAM);					// ����Աݹ�ȯ�Լ�(�������ó��)

	return T_OK;
}

// �����ȣ�Է�
int CTranCmn::fnAPP_AcceptBankNum()
{
	CString strReturn("");
	int		GetCase = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.BankNum, '0', sizeof(Accept.BankNum));		// �����ȣ
/////////////////////////////////////////////////////////////////////////////

		
	switch(m_pProfile->DEVICE.MachineType)
	{
	case U3100K:
		GetCase = 4;
		break;
	case U8100:
		if ((m_pDevCmn->EarPhoneInserted)	||					// �����ATM
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
			case 0:												// ���༱��
			m_pDevCmn->fnSCR_DisplayScreen(402, K_30_WAIT, PIN_MENU_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyString();
				GetCase = 9;									// ����
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
				if (strReturn == "��Ÿ")
					GetCase = 1;
				else
				if ((strReturn.GetLength() != 3)	||			// �ڷ����
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// ����
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
				if ((strReturn.GetLength() != 3)	||			// �ڷ����
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				if (IsZero(strReturn.GetBuffer(0), strReturn.GetLength()))
					GetCase = 3;								// ���ǻ��ȣ �Է�
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// ����
				}
				break;
 
			case 2:												// �����ȣ�Է�, // ���ǹ�ȣ�Է�
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
				if ((strReturn.GetLength() != 3)	||			// �ڷ����
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// ����
				}
				break;
			case 3:												// ���ǹ�ȣ�Է�
				m_pDevCmn->fnSCR_DisplayScreen(409, K_30_WAIT, PIN_NUMERIC_MODE);   //���ǻ� �����ڵ� �Է�ȭ�� ����
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
				if ((strReturn.GetLength() != 3)	||			// �ڷ����
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// ����
				}
				break;
			case 4:												// #N0208,����/���ǻ� ����
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
				if (strReturn == "����")				//#N0208
					GetCase = 5;
				else
				if (strReturn == "���ǻ�")
					GetCase = 6;
				else
					fnAPP_CancelProc(T_PROGRAM);
				break;
			case 5:												// �����ȣ�Է�#N0208
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
				if ((strReturn.GetLength() != 3)	||			// �ڷ����
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// ����
				}
				break;
			case 6:												// ���ǹ�ȣ�Է�//#N0208
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
				if ((strReturn.GetLength() != 3)	||			// �ڷ����
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// ����
				}
				break;
			case 7:												// #N0225
				m_pDevCmn->fnSCR_DisplayScreen(410, K_30_WAIT, PIN_NUMERIC_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(3);
				if (strReturn == "��Ÿ����")
					GetCase = 5;
				else
				if (strReturn == "���ǻ�")
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
				if ((strReturn.GetLength() != 3)	||			// �ڷ����
					(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength())))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memcpy(Accept.BankNum, strReturn.GetBuffer(0), strReturn.GetLength());
					GetCase = 9;								// ����
				}
				break;
			default: 
				break;
		}
	}

	return T_OK;
}

// ������¹�ȣ�Է�
int CTranCmn::fnAPP_AcceptInAccountNum()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.InAccountNumSize = 0;								// ������¹�ȣ����
	memset(Accept.InAccountNum, ' ', sizeof(Accept.InAccountNum));	
																// ������¹�ȣ
/////////////////////////////////////////////////////////////////////////////

	ScrNum = 405;											// Ÿ����ü

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
	if ((!strReturn.GetLength())	||							// �ڷ����
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

// û������Է�
int CTranCmn::fnAPP_AcceptInTransReqStock()
{
	int		ScrNum = 0, nIndex = 1;
	CString strReturn("");

	ScrNum = 922;												// û������Է�


	return T_OK;
}

// û��ȯ�ұݰ�����Է�
int CTranCmn::fnAPP_AcceptInTransReqStockAccount()
{
	int		ScrNum = 0, nIndex = 1;
	CString strReturn("");

	return T_OK;
}

// �������Ǵ�ü����Է�
int CTranCmn::fnAPP_AcceptInTransStock()
{
	int		ScrNum = 0, nIndex = 1;
	CString strReturn("");

	return T_OK;
}

// �����ι�ȣ�Է�
int CTranCmn::fnAPP_AcceptCmsCode()
{
	int		ScrNum = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.CMSCodeSize = 0;										// CMS����
	memset(Accept.CMSCode, ' ', sizeof(Accept.CMSCode));		// CMS(����/������ȣ)
/////////////////////////////////////////////////////////////////////////////

	return T_OK;
}

// ��й�ȣ�����Է�
int CTranCmn::fnAPP_AcceptChangePassword()
{
	CString strReturn("");
	int		ScrNum = 0;
	int		RetryCnt = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.ChangePwd, '0', sizeof(Accept.ChangePwd));	// ��й�ȣ(����)
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
	else													// �ڷ����
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
	else													// �ڷ����
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

// ������¹�ȣ�Է�
int	CTranCmn::fnAPP_AcceptLoanAccountNum()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.LoanAccountNumSize = 0;								// ������¹�ȣ����
	memset(Accept.LoanAccountNum, 0x20, sizeof(Accept.LoanAccountNum));
																// ������¹�ȣ 2004.09.15 �ʱ�ȭSPACE
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(403, K_30_WAIT, PIN_NUMERIC_MODE);
																// ���¹�ȣ�Է�
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
	if ((!strReturn.GetLength())	||							// �ڷ����
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


// ��ȭ��ȣ�Է�
int CTranCmn::fnAPP_AcceptTelNum()
{
	CString strReturn("");
	int		ScrNum = 0;
	
/////////////////////////////////////////////////////////////////////////////
	Accept.TelNumSize = 0;										// ��ȭ��ȣ����
	memset(Accept.TelNum, ' ', sizeof(Accept.TelNum));			// ��ȭ��ȣ
/////////////////////////////////////////////////////////////////////////////

	if (TranCode == TC_DEPOSIT)
		ScrNum = 413;
	else
		ScrNum = 413;

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);
																// ��ȭ��ȣ�Է�
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else 
	if (strReturn == "�Է»���")  //�ѳ�Ʈ �����.(��û����)-2013.03.04 //#0111
	{
		Accept.TelNumSize = 12;
		memset(Accept.TelNum, ' ', sizeof(Accept.TelNum));			
	}
	else 
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if ((!strReturn.GetLength())	||							// �ڷ����
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

// �ֹι�ȣ�Է�
int CTranCmn::fnAPP_AcceptJuminNum()
{
	CString strReturn("");
	int		ScrNum = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.JuminNum, ' ', sizeof(Accept.JuminNum));		// �ֹι�ȣ
/////////////////////////////////////////////////////////////////////////////


	ScrNum = 411;

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);
																// �ֹι�ȣ�Է�
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
	if ((!strReturn.GetLength())	||							// �ڷ����
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

// ��ü�ֱ�,��ü�����
int	CTranCmn::fnAPP_AcceptAutoDetail()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.AutoDate, ' ', sizeof(Accept.AutoDate));		// �ڵ���ü��
	memset(Accept.AutoCycle, ' ', sizeof(Accept.AutoCycle));	// ��ü�ֱ�
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(418, K_30_WAIT, PIN_NUMERIC_MODE);
																// ��ü�ֱ�
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
	if (strReturn == "1����")
		memcpy(Accept.AutoCycle, "01", 2);
	else
	if (strReturn == "3����")
		memcpy(Accept.AutoCycle, "03", 2);
	else
		fnAPP_CancelProc(T_INPUTERROR);

	m_pDevCmn->fnSCR_DisplayScreen(419, K_30_WAIT, PIN_NUMERIC_MODE);
																// ��ü�����
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
	if (strReturn == "����")
		memcpy(Accept.AutoDate, "32", 2);
	else
	if ((!strReturn.GetLength())	||							// �ڷ����
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

// �ڵ���ü����Ȯ��
int CTranCmn::fnAPP_AcceptAutoConfirm()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		j = 1;

	ScrNum = 515;

	m_pDevCmn->fnSCR_DisplayString(j++, fchpAPP_EditDestAcnt(Accept.InAccountNum, 12, 14, FALSE));
																// �������¹�ȣ
	m_pDevCmn->fnSCR_DisplayAmount(j++, (LPCTSTR)Accept.Money, 15);
																// �����ݾ�
	if (memcmp(Accept.AutoDate, "32", 2) == 0)					// ��ü�����
		m_pDevCmn->fnSCR_DisplayString(j++, "��");
	else
		m_pDevCmn->fnSCR_DisplayString(j++, (LPCTSTR)Accept.AutoDate, 2);
	m_pDevCmn->fnSCR_DisplayString(j++, (LPCTSTR)Accept.AutoCycle, 2);
																// ��ü�ֱ�

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
		ScrNum = 424;			// �������� ���۳�¥ �Է�
	else //if(TranCode == TC_INQUIRYA || TranCode == TC_INQSTOCK)
		ScrNum = 421;			// �ŷ�������ȸ ���۳�¥�Է�

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
	if ((!strReturn.GetLength())	||							// �ڷ����
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


// ��ü �� ��ȭ��� �ȳ����� �߰�
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
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
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

// ��ǥ���ڿ��μ���
int	CTranCmn::fnAPP_AcceptSlipChoice()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.SlipChoiceFlag = TRUE;								// ��ǥ���ڿ���
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
																// ��ǥ���ڿ��μ���
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

// K-CASH��й�ȣ�Է�
int CTranCmn::fnAPP_AcceptKCashPassword(int nReadDF, int ScrNum)
{
	CString strReturn("");
	int			nCnt = 0;	
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.KCashPassWord, 0, sizeof(Accept.KCashPassWord));
																// K-CASH��й�ȣ
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
		if ((strReturn.GetLength() < 4)	||		// �ڷ����
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

// K-CASH�ݾ��Է�
int CTranCmn::fnAPP_AcceptKCashMoney()
{
	int		ScrNum = 0;
	CString strReturn("");
	CString	strCardNum("");

/////////////////////////////////////////////////////////////////////////////
	fnAPP_InitializeSerialTranDS();								// ���Ӱŷ������ʱ�ȭ
/////////////////////////////////////////////////////////////////////////////

	strCardNum = MakeUnPack(m_pDevCmn->KCashInfo.CardNum, 5);

	if (TranCode == TC_KCLOAD)									// K-CASH����
	{
		ScrNum = 317;											// K-CASH�����ݾ��Է�
		m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
																// �ܾ�
		m_pDevCmn->fnSCR_DisplayNumeric(2, ByteHighOrder2Int(m_pDevCmn->KCashInfo.MaxLimitAmount, 4));

		if (ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4) == ByteHighOrder2Int(m_pDevCmn->KCashInfo.MaxLimitAmount, 4))
		{
			m_pDevCmn->fnSCR_DisplayString(2, "�ѵ��ݾ׸�ŭ �����Ǿ��־�");
			m_pDevCmn->fnSCR_DisplayString(3, "�� �̻� �����Ͻ� �� �����ϴ�.");
			m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("���� �ѵ��ݾ�:%d��", ByteHighOrder2Int(m_pDevCmn->KCashInfo.MaxLimitAmount, 4)));
			m_pDevCmn->fnSCR_DisplayScreen(701);				// �����ŷ� �Ұ��ȳ�
			Delay_Msg(3000);
			fnAPP_CancelProc(T_CANCEL);
		}
	}
	else
	if (TranCode == TC_KCUNLOAD)								// K-CASHȯ��
	{
		ScrNum = 318;											// K-CASHȯ�ұݾ��Է�
		m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
																// ȯ�Ұ��ɾ�
		m_pDevCmn->fnSCR_DisplayNumeric(2, ByteHighOrder2Int(m_pDevCmn->KCashInfo.MaxLimitAmount, 4));

		if (!ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4))// �ܾ��� 0�ΰ��
		{
			fnAPP_CancelProc(T_MSG, "ȯ���Ͻ� �ܾ��� �����ϴ�.", "ȯ�� ���ɱݾ� : 0��");
		}
	}

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_AMOUNT_MODE);// �ݾ��Է�
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
	if ((strReturn.GetLength() != 8)		||					// �ݾ��ڷ����
		(!IsNum(strReturn.GetBuffer(0), 8))	||		
		(IsZero(strReturn.GetBuffer(0), 8)))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	if ((TranCode == TC_KCLOAD)	&&								// K-CASH����
		((Asc2Int(strReturn.GetBuffer(0), 8)) > 
				(ByteHighOrder2Int(m_pDevCmn->KCashInfo.MaxLimitAmount, 4) - ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4))))
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "���� ���ɱݾ� �ʰ��Դϴ�");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
	if ((TranCode == TC_KCUNLOAD) &&							// K-CASHȯ��
		((Asc2Int(strReturn.GetBuffer(0), 8)) > (ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4))))
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			{
				fnAPP_CancelProc(T_MSG, "ȯ�ұݾ� �ʰ��Դϴ�");
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

// K-CASH��й�ȣ�����Է�
// V02-06-01-#9 IC-PIN���� ���� : �ٸ� DF�鵵 ���밡����� ����
int CTranCmn::fnAPP_AcceptKCashChangePassword(int nReadDF)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptKCashChangePassword(%d)", nReadDF);

	CString	strReturn("");
	int		RetryCnt = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.ChangeICPwd, 0, sizeof(Accept.ChangeICPwd));
																// K-CASH��й�ȣ(����)
	memset(Accept.ChangeICPwdConfirm, 0, sizeof(Accept.ChangeICPwdConfirm));
																// K-CASH��й�ȣ(����Ȯ��)
/////////////////////////////////////////////////////////////////////////////

	RetryCnt = 3;												// �Է¿���üũ3ȸ
	while (RetryCnt)
	{
		memset(Accept.ChangeICPwd, 0, sizeof(Accept.ChangeICPwd));
																// K-CASH��й�ȣ(����)

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
		if ((strReturn.GetLength() < 6)								||	// �ڷ����
			(strReturn.GetLength() > sizeof(Accept.ChangeICPwd))	||
			(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
			(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
			fnAPP_CancelProc(T_INPUTERROR);
		else
		{
			memcpy(Accept.ChangeICPwd, strReturn.GetBuffer(0), strReturn.GetLength());
		}

		memset(Accept.ChangeICPwdConfirm, 0, sizeof(Accept.ChangeICPwdConfirm));
																// K-CASH��й�ȣ(����Ȯ��)

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
		if ((strReturn.GetLength() < 6)									||	// �ڷ���� :���̼���(4->6) 2004.02.27
			(strReturn.GetLength() > sizeof(Accept.ChangeICPwdConfirm))	||
			(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))		||
			(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
			fnAPP_CancelProc(T_INPUTERROR);
		else
		{
			memcpy(Accept.ChangeICPwdConfirm, strReturn.GetBuffer(0), strReturn.GetLength());
		}

		// �ι� �Է¹��� ��й�ȣ�� ���Ͽ� ������ �����ϰ�
		// �ٸ��� 3������ ���Է��� �޴´�.
		if (memcmp(Accept.ChangeICPwd, Accept.ChangeICPwdConfirm, 8) == 0)
		{
			if (fnAPP_KCashCheckChangePin() == T_OK)			// ����/���Ӽ���Ȯ��	2004.02.27
				break;											// ����/������ �ƴѰ�츸 ��ȿó��

			if (!--RetryCnt)									// ���/���Է�ȭ��ó��  2004.02.27
			{
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "��й�ȣ �Է�Ƚ���� �ʰ��Ǿ����ϴ�");
				else
					fnAPP_CancelProc(T_CANCEL);
			}
			else
			{
				m_pDevCmn->fnSCR_DisplayString(1, "PIN �Է¿���");
				m_pDevCmn->fnSCR_DisplayString(2, "������ ��ȣ�� ���ӵ� ��ȣ��");
				m_pDevCmn->fnSCR_DisplayString(4, "PIN���� ����Ͻ� �� �����ϴ�");
				m_pDevCmn->fnSCR_DisplayString(5, "�ٽ� �Է��Ͽ� �ֽʽÿ�");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				Delay_Msg(3 * 1000);
			}
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayString(2, "�����Ͻ� ��й�ȣ�� ��ġ���� �ʽ��ϴ�.");
			m_pDevCmn->fnSCR_DisplayString(3, "�����Ͻ� ����ȭ�� ��й�ȣ��");
			m_pDevCmn->fnSCR_DisplayString(4, "ó������ �ٽ� �Է��Ͽ� �ֽʽÿ�");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(2 * 1000);
			if (!--RetryCnt)
			{
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "��й�ȣ �Է�Ƚ���� �ʰ��Ǿ����ϴ�");
				else
					fnAPP_CancelProc(T_CANCEL);
			}
		}
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptKCashChangePassword():return");
	return T_OK;
}

// ����IC ���¼���
int CTranCmn::fnAPP_FICSelectAccount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FICSelectAccount()");

	if (!MenuFICMode)											// ����IC�ŷ�Ȯ��
		return T_ERROR;

	if (m_pDevCmn->FICAccountCount > 10)					
		return T_ERROR;
//		fnAPP_CancelProc(T_MSG, "����IC ī�峻��", "���¹�ȣ�б��� ������ �߻��Ͽ����ϴ�", " [��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
	if (m_pDevCmn->FICAccountCount <= 0)
		return T_ERROR;
//		fnAPP_CancelProc(T_MSG, "����IC ī�峻��", "��밡���� ���������� �����ϴ�", " [��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");

	CString strTemp("");
	CString GetKeyStr("");
	int		ScrNum = 407;										// ȭ��
	int		ScrOffset = 0;										// 5��������ϴ� ȭ��Offset(0 or 5)
	CString	strBankCode("");									// �߱������ڵ�

	Accept.FICSelectAccountIndex = 0;							// ù����	
	strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Mid(1));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FICSelectAccount() strBankCode[%s]", strBankCode);

/*
//#0023 Kim.G.J ==> "#0034 (��������, ��ü�� : ��ü IC)
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
			// ���ι�ȣ ���
			if (m_pDevCmn->EarPhoneInserted == TRUE)		// �����ATM
			{
				// 0�� ���� ������� ���Ͽ� ���ǹ� �߰�
				if (Asc2Int(m_pDevCmn->FinanceICDisp[ScrOffset + Index].Num, sizeof(m_pDevCmn->FinanceICDisp[ScrOffset + Index].Num)) != 0)
				{
					strTemp.Format("%d��, ",Index+1);
					m_pDevCmn->fnSCR_DisplayString(Index*2+1, strTemp.GetBuffer(0));
				}
			}
			else
				m_pDevCmn->fnSCR_DisplayString(Index*2+1, m_pDevCmn->FinanceICDisp[ScrOffset + Index].Num);
			
			// ���� ���
			strTemp = GetString(m_pDevCmn->FinanceICDisp[ScrOffset + Index].AccNum, 
								m_pDevCmn->FinanceICISO3Info[ScrOffset + Index].AccountNOCnt, 
								m_pDevCmn->FinanceICISO3Info[ScrOffset + Index].AccountNOCnt);

			CString strAccountNum("");
			if (m_pDevCmn->EarPhoneInserted == TRUE)		// �����ATM
			{
				strAccountNum.Format("<digit>%s</digit>", strTemp);
				m_pDevCmn->fnSCR_DisplayString(Index*2+2, strAccountNum);
			}
			else
				m_pDevCmn->fnSCR_DisplayString(Index*2+2, strTemp.GetBuffer(0));

		}

		if (ScrOffset == 0)
		{
			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);		    // "����"�Ұ�

			if ((m_pDevCmn->EarPhoneInserted == TRUE) &&		// �����ATM
				(m_pDevCmn->FICAccountCount > 5) )				// �����ATM
				m_pDevCmn->fnSCR_DisplayString(21, "���� ���¸� Ȯ�� �Ͻ÷��� 7�� ��ư��");
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayImage(2, FALSE);		// "����"�Ұ�

			if (m_pDevCmn->EarPhoneInserted == TRUE)				// �����ATM
				m_pDevCmn->fnSCR_DisplayString(21, "���� ���¸� Ȯ�� �Ͻø� 6�� ��ư��");
		}

		
		if (m_pDevCmn->FICAccountCount <= 5)				// ���°����� ��ȭ�������ΰ��
		{
			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);		// "����"�Ұ�
			m_pDevCmn->fnSCR_DisplayImage(2, FALSE);		// "����"�Ұ�
		}

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_INPUT_MODE);
		GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();

		if (GetKeyStr == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
		else
		if (GetKeyStr == S_CANCEL2)								// #�����ATM ���� CANCEL ����
			fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
		else 
		if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (GetKeyStr == S_INPUTOVER)							// INPUTOVER
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (GetKeyStr == S_NEXT)								// ����
			ScrOffset = 5;
		else
		if (GetKeyStr == S_PREV)								// ����
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

// DES��й�ȣ�Է�
// DES OutGoing�ŷ� 2004.05.31  -> #N0176
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
	memset(Accept.DesPassWord, 0, sizeof(Accept.DesPassWord));	// K-CASH��й�ȣ
	memset(Accept.DesEncryptPin, 0, sizeof(Accept.DesEncryptPin));
	memset(szUnCardNum, 0, sizeof(szUnCardNum));
	memset(szMemberNum, 0, sizeof(szMemberNum));
	memset(szUnPack, 0, sizeof(szUnPack));
	memset(szPack, 0, sizeof(szPack));
																// Des Encrypt PIN	// DES OutGoing�ŷ� 2004.05.31
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
	m_pDevCmn->fnSCR_DisplayScreen(701);			// �ȳ�ȭ��(5Line)
	Delay_Msg(1000);
	
	MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "Pwd -> (:%s len:%d)", Accept.DesPassWord, strReturn.GetLength());
	MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "CRD -> (:%s len:%d)", CardData.ISO2Buff,  CardData.ISO2size);


// 	���Ļ�� - [ûȣ�̻��]
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


// ����� PIN ��ȣ�Է�
int CTranCmn::fnAPP_AcceptMPINConfirmNum()   //#N0182
{
	int		ScrNum = 3800;
	CString strReturn("");
	int		nCnt = 0;
	
	/////////////////////////////////////////////////////////////////////////////
	memset(Accept.MPINConfirmNum, '0', sizeof(Accept.MPINConfirmNum));	
																// �����PIN��ȣ
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
		else														// �ڷ����
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
		memset(&VCardData[0], 0x30, 16 - nCardNum);// 4byte�� 0���� ����
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


// �۱��ֹι�ȣ��
int CTranCmn::fnAPP_AcceptCompareJuminNum()
{
	CString strReturn("");
	int		nLoopCount = 0;
	
	while (TRUE)
	{
		m_pDevCmn->fnSCR_DisplayScreen(411, K_30_WAIT, PIN_NUMERIC_MODE);
																// �ֹι�ȣ�Է�
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if ((strReturn == S_CANCEL)		||
			(strReturn == S_TIMEOVER)	||
			(strReturn == S_INPUTOVER)	)
			nLoopCount++;
		else
		if ((!strReturn.GetLength())	||							// �ڷ����
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
		m_pDevCmn->fnSCR_DisplayString(1, "ó���� �Է��Ͻ� �ֹι�ȣ��");
		m_pDevCmn->fnSCR_DisplayString(2, "���Է��Ͻ� �ֹι�ȣ�� Ʋ���ϴ�.");
		m_pDevCmn->fnSCR_DisplayString(3, "â���� �����Ͽ� �ֽñ� �ٶ��ϴ�.");
		m_pDevCmn->fnSCR_DisplayScreen(701);

		m_pDevCmn->fnAPL_StackError("0110295", "�۱�����ȸ��", DEV_BRM);
		fnAPD_CashDepositStore(TRUE);							// �����Աݼ���
		fnAPP_CancelProc(T_OK, NULL, NULL, NULL);				// 2007-01-03-#01
	}

	return T_OK;
}

// ���� ������� ���� Ȯ��
int	CTranCmn::fnAPP_AcceptContinuousPBChoice()
{
	CString strReturn("");

	MS		BfrPbMsData;								//11/22
	memcpy (BfrPbMsData.sBuf, PbMsData.sBuf, 256);		//test

	int		nGetKeyID = NULL;

	if (m_pDevCmn->TranResult == FALSE)							// ���ŷ� ����ó���ø� ����
		return T_OK;

	if (!PbTran)												// ����ŷ��϶��� ����
		return T_OK;

	fnAPD_CardEnDisable(DISABLE, TRUE);
	fnAPD_PbEnDisable(ENABLE, TRUE);
	m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_ON);

	m_pDevCmn->fnSCR_DisplayScreen (550, K_30_WAIT, PIN_MENU_MODE);

	nGetKeyID = m_pDevCmn->fnCMN_ScanDeviceAction(DEV_PBM | DEV_SCR, K_45_WAIT, EVENT_IN);
	if (nGetKeyID == DEV_PBM)
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_OFF);
		m_pDevCmn->fnSCR_DisplayScreen(805);			// ������ �а��ֽ��ϴ�.
		fnAPD_PbReadCheck();
//		if (memcmp(BfrPbMsData.sBuf, PbMsData.sBuf, 256))						//11/22	01-01-01														
//			fnAPP_CancelProc(T_MSG, "���¹�ȣ Ȯ���� �ٽ� �ŷ��Ͽ� �ֽʽÿ� ");	//test
// 		return T_OK;
		if (memcmp(BfrPbMsData.sBuf, PbMsData.sBuf, 256))	//11/22	01-01-01 �����														
		{							// �ȳ�ȭ�� 3�����
			m_pDevCmn->fnSCR_DisplayString(3, "���¹�ȣ Ȯ���� �ٽ� �ŷ��Ͽ� �ֽʽÿ�");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(3 * 1000);
			PbNextPageFlag = FALSE;									// ����ó�� ����
			PbNextTranFlag = FALSE;									// ����ó�� ����
			fnAPD_MaterialOut(DEV_PBM);								// ��ü����	
			fnAPD_MaterialOutCheckRetract(DEV_PBM);					// ��ü���������ȸ��
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
		{														// �ȳ�ȭ�� 3�����
			fnAPD_PbEnDisable(DISABLE);                         // #N0136
			m_pDevCmn->fnSCR_DisplayString(3, "�̿��� �ּż� �����մϴ�");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(3 * 1000);

			PbNextPageFlag = FALSE;								// ����ó�� ����
			PbNextTranFlag = FALSE;								// ����ó�� ����

			return T_OK;										// ����ŷ��Ϸ�ó��
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
		m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_012x); //�����
		m_pDevCmn->fnSCR_DisplayString(2, strCT(m_RD.byDataField_021x, strlen(m_RD.byDataField_021x))); //����
		m_pDevCmn->fnSCR_DisplayString(3, m_RD.byDataField_017x); //����
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
					cstrTemp.Format("������ �� %d.%2.2s  �ۼ�Ʈ", Asc2Int(&m_RD.byDataField_458x[0], 2), &m_RD.byDataField_458x[2]);
				m_pDevCmn->fnSCR_DisplayString(2, cstrTemp);
			}
			else
			if(memcmp(m_RD.byDataField_458x, "****", 4) == 0)
			{
				if (m_pDevCmn->EarPhoneInserted == TRUE)		// 2��° ���� ������� ��� 3��° ���� 2��°�� �������� ���� ����
					m_pDevCmn->fnSCR_DisplayString(2, " ");
				else
					m_pDevCmn->fnSCR_DisplayString(2, (LPCTSTR)m_RD.byDataField_458x);
			}
			if(memcmp(m_RD.byDataField_024x, "******", 6) == 0 || memcmp(m_RD.byDataField_024x, "    ", 4) == 0)    //������ * ȭ�� ǥ��
			{
				if (m_pDevCmn->EarPhoneInserted == TRUE)		// 3��° ���� ������� ��� 4��° ���� 3��°�� �������� ���� ����
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

// ���μ��ΰŷ��ڵ弱��
int CTranCmn::fnAPP_UTILYBILLS_Sub01_InputProc()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.JiroTranCode = 0;									// ���μ��ΰŷ��ڵ�
	memset(Accept.JiroTranOrgCode, 0x00, sizeof(Accept.JiroTranOrgCode));		// ���γ��α���ڵ�
/////////////////////////////////////////////////////////////////////////////

	//m_pDevCmn->fnSCR_DisplayImage(7, FALSE); //���ϼ��� Disable

	m_pDevCmn->fnSCR_DisplayScreen(770, K_30_WAIT, PIN_MENU_MODE1);				// ���ο����������  #N0173
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == "�Ϲ���ȭ���")
	{
		Accept.JiroTranCode = TC_JRPHONE;						// KT��ȭ���
		memcpy(Accept.JiroTranOrgCode, "710000000", 9);
	}
	else
	if (strReturn == "������")
	{
		Accept.JiroTranCode = TC_JRELEC;						// ������
		memcpy(Accept.JiroTranOrgCode, "720000000", 9);
	}
	else
	if (strReturn == "�ǰ������")
	{
		Accept.JiroTranCode = TC_JRINSURE;						// �ǰ������
		memcpy(Accept.JiroTranOrgCode, "730000000", 9);
	}
	else
	if (strReturn == "���ο���")
	{
		Accept.JiroTranCode = TC_JRPANS;						// ���ο���
		memcpy(Accept.JiroTranOrgCode, "740000000", 9);
	}
	else
	if (strReturn == "���δ��")
	{
		Accept.JiroTranCode = TC_JRJIRO;						// ���δ��
		memcpy(Accept.JiroTranOrgCode, "900000000", 9);
	}
	else
	if (strReturn == "����/����")
	{
		Accept.JiroTranCode = TC_JRNTAX;						// ����/����
		memcpy(Accept.JiroTranOrgCode, "000000000", 9);
	}
	else
	if (strReturn == "���漼")
	{
		Accept.JiroTranCode = TC_JRLTAX;						// ���漼
		memcpy(Accept.JiroTranOrgCode, "000000000", 9);
	}
	else
	if (strReturn == "���ϼ������")
	{
		Accept.JiroTranCode = TC_JRWATER;						// ���ϼ������
		memcpy(Accept.JiroTranOrgCode, "000000000", 9);
	}
	else
	if (strReturn == "�����Ģ��")
	{
		Accept.JiroTranCode = TC_JRTRAFF;						// �����Ģ��
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

// ���ι�ȣ�Է�
int	CTranCmn::fnAPP_UTILYBILLS_Sub02_InputProc()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.JiroNum, ' ', sizeof(Accept.JiroNum));		// ���ι�ȣ
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(771, K_30_WAIT, PIN_NUMERIC_MODE);				// ���ι�ȣ�Է�  #N0173
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
	if ((strReturn.GetLength() != sizeof(Accept.JiroNum))	||	// �ڷ����
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.JiroNum, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// ���ڳ��ι�ȣ�Է�
int	CTranCmn::fnAPP_UTILYBILLS_Sub03_InputProc()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	Accept.JiroElecNumSize = 0;									// ���ڳ��ι�ȣ����
	memset(Accept.JiroElecNum, ' ', sizeof(Accept.JiroElecNum));// ���ڳ��ι�ȣ
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(772, K_30_WAIT, PIN_NUMERIC_MODE);				// ���ڳ��ι�ȣ�Է�  #N0173
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
	if ((!strReturn.GetLength())	||							// �ڷ����
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


//#GIRO ���漼 Ÿ�� ��ȸ��� ����
int CTranCmn::fnAPP_LOCALTAX_Sub01_InputProc()
{

	CString strReturn("");

	m_pDevCmn->fnSCR_DisplayScreen(3017, K_30_WAIT);				// ��ȸ��� ����

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == "�����ι�ȣ")
	{
		fnAPP_LOCALTAX_AcceptSimpleNum();
//		Accept.JiroGubun = 'E';
	}
	else
	if (strReturn == "���ڳ��ι�ȣ")
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

//#GIRO �ſ�ī�� �Һ� ����
int CTranCmn::fnAPP_LOCALTAX_Sub02_InputProc()
{

	CString strReturn("");
	CString strTemp("");

	strTemp.Format("%d", Asc2Int(&m_RXLOCALTAXDATA.JiroBankCode[5],2));  //ī���ȣ
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);

	m_pDevCmn->fnSCR_DisplayScreen(3013, K_30_WAIT, PIN_MENU_MODE1);				// ��ȸ��� ���� , #N0173

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == "�ϽúҰ���")
	{
		memcpy(&m_RXLOCALTAXDATA.JiroBankCode[5], "00", 2);
	}
	else
	if (strReturn == "�Һΰ���")
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

	m_pDevCmn->fnSCR_DisplayScreen(3014, K_30_WAIT , PIN_NUMERIC_MODE);				// �����ι�ȣ #N0173
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
	if ((!strReturn.GetLength())	||							// �ڷ����
		(strReturn.GetLength() > sizeof(Accept.JiroElecNum))	||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		Sprintf(szT, 2, "%02d", Asc2Int(strReturn)); //#H0177 - Bug ���� 
		memcpy(Accept.JiroPayMonth, szT, 2);
	}

	return T_OK;
}

int	CTranCmn::fnAPP_LOCALTAX_AcceptSimpleNum()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.JiroElecNum, ' ', sizeof(Accept.JiroElecNum));// ���ڳ��ι�ȣ
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(3011, K_30_WAIT , PIN_NUMERIC_MODE);				// �����ι�ȣ #N0173
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
	if ((!strReturn.GetLength())	||							// �ڷ����
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
	Accept.JiroElecNumSize = 0;									// ���ڳ��ι�ȣ����
	memset(Accept.JiroElecNum, ' ', sizeof(Accept.JiroElecNum));// ���ڳ��ι�ȣ
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(772, K_30_WAIT, PIN_NUMERIC_MODE);		// ���ڳ��ι�ȣ�Է� #N0173
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
	if ((!strReturn.GetLength())	||							// �ڷ����
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


