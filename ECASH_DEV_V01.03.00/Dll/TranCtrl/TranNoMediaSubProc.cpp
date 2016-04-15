#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\MacroDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**********************************************************************************************************/
/* Function Name : NoMediaSubProc Make (send) gijin
/* 
/* Date			 : 2010.06.09
/* 
/* Input	     : TRCODE
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/  

int CTranCmn::fnAPP_TranNoMediaInitProc()
{
	fnAPP_InitializeSerialTranDS();	
	
	switch (TranCode)
	{
	case TC_IBK_MANAGEMENT:										// 환경점검
		fnAPP_IBK_MANAGEMENT_InitProc();
		break;
	default:
		break;
		
	}

	return T_OK;
}


int CTranCmn::fnAPP_TranNoMediaInputProc()
{

	switch (TranCode)
	{
	case TC_IBK_MANAGEMENT:										// 환경점검
		fnAPP_IBK_MANAGEMENT_InputProc();
		break;
	default:
		break;
		
	}

	return T_OK;
}

int CTranCmn::fnAPP_SelectMenu()
{
	CString strReturn("");

	return T_OK;
}

int CTranCmn::fnAPP_TranNoMediaOutputProc()
{
	char	szTempBuff[256] = {0,};

	switch (TranCode)
	{
	case TC_IBK_MANAGEMENT:										// 환경점검
		fnAPP_IBK_MANAGEMENT_OutputProc();
		break;
	default:
		break;
		
	}

	return T_OK;
}


int CTranCmn::fnAPP_TranNoMediaCheckDeviceActionProc()
{
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);
	return T_OK;
}


int CTranCmn::fnAPP_TranNoMediaCheckMenuContinueProc()
{


	return T_OK;
}


int	CTranCmn::fnAPP_IBK_MANAGEMENT_InitProc()
{
	return T_OK;
}

int	CTranCmn::fnAPP_IBK_MANAGEMENT_InputProc()
{
	CString	ManagerOpPassword("00000000");
	CString	KtLinKusOpPassword("0000");
	CString strReturn("");
	CString strTmp("");
	int		nGetCnt = 0;
	CString	strAcceptLen("0");
/////////////////////////////////////////////////////////////////////////////
	Accept.AuthFlag = 0;
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(3200, K_30_WAIT, PIN_MENU_MODE1);				// 환경 업체선택
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == "NICE")
	{
		ManagerOpPassword.Format("%8.8s", GetDate());	
		Accept.AuthFlag = 1;
		strAcceptLen = "8";
	}
	else
	if (strReturn == "KT LINKUS")
	{
		KtLinKusOpPassword = "2580";
		Accept.AuthFlag = 2;
		strAcceptLen = "4";
	}
	else
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
	{
		fnAPP_CancelProc(T_INPUTOVER);
	}



	while (TRUE)
	{
		if(nGetCnt > 2) fnAPP_CancelProc(T_INPUTOVER);
		m_pDevCmn->fnSCR_DisplayString(1, strAcceptLen); //	#0115
		m_pDevCmn->fnSCR_DisplayScreen(3202, K_30_WAIT, PIN_NUMERIC_MODE);
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
			if (Accept.AuthFlag == 2)
			{
				if (strReturn == KtLinKusOpPassword)
				{
					break;
				}
				else 
				{
					nGetCnt++;
					continue;
				}
			
			}
			else
			{
				if (strReturn == ManagerOpPassword)
				{
					break;
				}
				else 
				{
					nGetCnt++;
					continue;
				}
			}
			
		}
	}
	

	strReturn = "";
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.JiroElecNum, '0', sizeof(Accept.JiroElecNum));		   // 환경점검내용
/////////////////////////////////////////////////////////////////////////////
    //#N0214
	strTmp = IniGetStr(_TRANS_INI, TRANS_SEC, "IBK_MANAGEMENT_INFO","00000000000000");	//#N0199  환경점검 이전 내용 표시
	
	//#N0213 환경점검 시 초기 상태 무조건 비정상
	//strTmp = "11111111111111"; //#N0214
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);

	m_pDevCmn->fnSCR_DisplayScreen(3201, K_120_WAIT, PIN_MENU_MODE1);				// 환경점검내용
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
	if ((strReturn.GetLength() != 14)	||	// 자료검증
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	)		
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		Accept.JiroElecNumSize = strReturn.GetLength();
		memcpy(Accept.JiroElecNum, strReturn.GetBuffer(0), strReturn.GetLength());

		strTmp.Empty();
		strTmp = strReturn;
		IniSetStr(_TRANS_INI, TRANS_SEC, "IBK_MANAGEMENT_INFO", strReturn.GetBuffer(0)); //#N0199  환경점검 이전 내용 표시 -> #N0214
	}


	strReturn = "";
/////////////////////////////////////////////////////////////////////////////
	Accept.TelNumSize = 0;									// 직원번호 
	memset(Accept.TelNum, ' ', sizeof(Accept.TelNum));		// 직원번호 
/////////////////////////////////////////////////////////////////////////////
		
	m_pDevCmn->fnSCR_DisplayScreen(3203, K_30_WAIT, PIN_NUMERIC_MODE); //// 직원번호 
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
		(strReturn.GetLength() > sizeof(Accept.TelNum))	||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		Accept.TelNumSize = strReturn.GetLength();
		memcpy(Accept.TelNum, strReturn.GetBuffer(0), strReturn.GetLength());
		IniSetStr(_TRANS_INI, TRANS_SEC, "IBK_MANAGEMENT_INFO", strTmp); //#N0199  환경점검 이전 내용 표시
	}
	

	return T_OK;
}
int	CTranCmn::fnAPP_IBK_MANAGEMENT_OutputProc()
{

	m_pDevCmn->fnSCR_DisplayString(3, "이용해 주셔서 감사합니다");
	m_pDevCmn->fnSCR_DisplayScreen(701);
	Delay_Msg(2 * 1000);

	return T_OK;
}

int	CTranCmn::fnAPP_IBK_MANAGEMENT_CheckDeviceActionProc()
{
	return T_OK;
}
int	CTranCmn::fnAPP_IBK_MANAGEMENT_CheckMenuContinueProc()
{
	return T_OK;
}

