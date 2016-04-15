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
/* Function Name : TranCmnLibSubProc
/* 
/* Date			 : 2010.06.09
/* 
/* Input	     : TRCODE
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/  

// �ֹι�ȣ�Է�
int CTranCmn::fnAPP_AcceptCmnLibJuminNum()
{
	CString strReturn("");
	int		ScrNum = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.JuminNum, ' ', sizeof(Accept.JuminNum));		// �ֹι�ȣ
/////////////////////////////////////////////////////////////////////////////

	if(ExtraServiceFlag == TC_NOMEDIA_HYUNCAPTIAL)
		ScrNum = 1166;	//760;	#0356
	else
	if(ExtraServiceFlag == TC_INTERAD)
		ScrNum = 2202;
	else
	if(ExtraServiceFlag == TC_HIPASS_CREDIT)
		ScrNum = 2924;									// HI-PASS �������� ��������(�������) �߰� #0443
	else
		ScrNum = 411;

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

// Seting Sms Number
int CTranCmn::fnAPP_AcceptSmsNum()
{
	CString strReturn("");
	int		ScrNum = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.SMSCode, ' ', sizeof(Accept.SMSCode));		
/////////////////////////////////////////////////////////////////////////////


	ScrNum = 761;

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
		(strReturn.GetLength() > sizeof(Accept.JuminNum))		||
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.SMSCode, strReturn.GetBuffer(0), strReturn.GetLength());
	}

	return T_OK;
}

// �Һαݾ�
int	CTranCmn::fnAPP_AcceptInstallmentPlan()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.AutoDate, ' ', sizeof(Accept.AutoDate));		// �ڵ���ü��
	memset(Accept.AutoCycle, ' ', sizeof(Accept.AutoCycle));	// ��ü�ֱ�
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(763, K_30_WAIT, PIN_NUMERIC_MODE);
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
	if (strReturn == "6����")
		memcpy(Accept.AutoCycle, "06", 2);
	else
	if (strReturn == "10����")
		memcpy(Accept.AutoCycle, "10", 2);
	else
		fnAPP_CancelProc(T_INPUTERROR);

	return T_OK;
}

BOOL CTranCmn::fBAPP_EditAmount(void *inbuff,void *outbuff,int inLen,int outLen,int isminus)
{
    char    *instr  = (char* )inbuff;
    char    *outstr = (char* )outbuff;

	EditAmount(instr, outstr, inLen, outLen);
	if (Asc2Int(instr,inLen) != 0)
	{
		int index = Compress((char *)outstr, outLen);
		if (isminus)
		{
			outstr[index-2] = '\\';								//'\'
			outstr[index-1] = '-';
		}
		else 
		{
			outstr[index-1] = '\\';								//'\'
		}
	}
	else
	{
		outstr[outLen-2] = '\\';								//'\'
		outstr[outLen-1] = '0';
	}

	return TRUE;
}

CString CTranCmn::fchpAPP_Asc2Amt(void* inbuff, int inLen,int outLen,char MarkChar)
{
	CString	cstrData("");

	cstrData = Asc2Amt(inbuff, inLen, outLen, MarkChar);
	LTrim(cstrData.GetBuffer(0), cstrData.GetLength(), ' ');
	return cstrData;
}

int CTranCmn::fnAPP_AccpetPbConfirmMSG()
{
	CString strReturn("");

	if (Accept.PbConfirmNumSize == 5)
	{
		m_pDevCmn->fnSCR_DisplayScreen(551, K_30_WAIT, PIN_MENU_MODE);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();


		if ((strReturn == S_CONFIRM)	||
			(strReturn == S_CONFIRM2)	)
			return T_OK;
		else 
		if ((strReturn == S_CANCEL)		||
			(strReturn == S_CANCEL2)	)
			fnAPP_CancelProc(T_CANCEL);							// ����Լ�(�������ó��)
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);						// ����Լ�(�������ó��)
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);						// ����Լ�(�������ó��)
		else
			fnAPP_CancelProc(T_PROGRAM);						// ����Լ�(�������ó��)
	}

	return T_OK;
}


int CTranCmn::fnAPP_Accpet_KEB_PbConfirmMSG()
{
	CString strReturn("");

	m_pDevCmn->fnSCR_DisplayScreen(548, K_15_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		return T_OK;									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}
// ���ձ��� �ȳ� ����
int CTranCmn::fnAPP_Accpet_KEB_PbConfirmMSG1()
{
	CString strReturn("");

	m_pDevCmn->fnSCR_DisplayScreen(553, K_15_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();


	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	||
		(strReturn == S_YES))			
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	||
		(strReturn == S_NO))			
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		return T_OK;									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

// ���ձ��� �ȳ� ����
int CTranCmn::fnAPP_Accpet_KEB_PbConfirmMSG2()
{
	CString strReturn("");

	m_pDevCmn->fnSCR_DisplayScreen(552, K_15_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();


	if (strReturn == "��������")
	{
		TranCode = TC_PBOOKTRM;
	}
	else 
	if (strReturn == "��������")
	{
		TranCode = TC_PBOOK;		
	}
	else
	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	) //#0116
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		return T_OK;									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

//#0121  sj.Kim 13.03.20
int CTranCmn::fnAPP_Accpet_KEB_Fee_Confirm()
{
	CString strReturn("");
	CString strTmp("");

	strTmp = "�� ���� ��� ���� ����Ƽ���̹Ƿ� �ϳ����� �� Ÿ����� ��� �����ᰡ �ΰ��˴ϴ�.";
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);
	
	m_pDevCmn->fnSCR_DisplayScreen(439, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

int CTranCmn::fnAPP_Accpet_KIUP_Fee_Confirm()
{
	CString strReturn("");
	CString strTmp("");

	strTmp = "�� �ڵ�ȭ���� IBK�������� ûȣ����ĳ�� �� �� �����Ͽ� ��ϴ� �ڵ�ȭ����̹Ƿ� Ÿ������� ������ ������ ��������� ������� ���� �� �ֽ��ϴ�.";
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);

	m_pDevCmn->fnSCR_DisplayScreen(439, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

int CTranCmn::fnAPP_Accpet_NONGHYUP_Fee_Confirm()
{
	CString strReturn("");
	CString strTmp("");

	strTmp = "�� �ڵ�ȭ���� ��������� ûȣ����ĳ�� �� �� �����Ͽ� ��ϴ� �ڵ�ȭ����̹Ƿ� Ÿ������� ������ ������ ��������� ������� ���� �� �ֽ��ϴ�.";
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);

	m_pDevCmn->fnSCR_DisplayScreen(439, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

int CTranCmn::fnAPP_Accpet_KWANGJU_Fee_Confirm()
{
	CString strReturn("");
	CString strTmp("");

	strTmp = "�� �ڵ�ȭ���� ��������� ûȣ����ĳ�� �� �� �����Ͽ� ��ϴ� �ڵ�ȭ����̹Ƿ� Ÿ������� ������ ������ ��������� ������� ���� �� �ֽ��ϴ�.";
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);

	m_pDevCmn->fnSCR_DisplayScreen(439, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

int CTranCmn::fnAPP_Accpet_KYONGNAM_Fee_Confirm()
{
	CString strReturn("");
	CString strTmp("");

	strTmp = "�� �ڵ�ȭ���� �泲����� ûȣ����ĳ�� �� �� �����Ͽ� ��ϴ� �ڵ�ȭ����̹Ƿ� Ÿ������� ������ ������ ��������� ������� ���� �� �ֽ��ϴ�.";
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);

	m_pDevCmn->fnSCR_DisplayScreen(439, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

int CTranCmn::fnAPP_Accpet_HANA_Fee_Confirm()		//#N0214
{
	CString strReturn("");
	CString strTmp("");

	strTmp = "�� �ڵ�ȭ���� �ϳ������ ûȣ����ĳ�� �� �� �����Ͽ� ��ϴ� �ڵ�ȭ����̹Ƿ� Ÿ������� ������ ������ ��������� ������� ���� �� �ֽ��ϴ�.";
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);

	m_pDevCmn->fnSCR_DisplayScreen(439, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

int CTranCmn::fnAPP_Accpet_SHINHAN_Fee_Confirm()		//#N0215
{
	CString strReturn("");
	CString strTmp("");

	strTmp = "�� �ڵ�ȭ���� ��������� ûȣ����ĳ�� �� �� �����Ͽ� ��ϴ� �ڵ�ȭ����̹Ƿ� Ÿ������� ������ ������ ��������� ������� ���� �� �ֽ��ϴ�.";
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);

	m_pDevCmn->fnSCR_DisplayScreen(439, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

int CTranCmn::fnAPP_Accpet_SC_Fee_Confirm()		//#N0219
{
	CString strReturn("");
	CString strTmp("");

	strTmp = "�� �ڵ�ȭ���� SC����� ûȣ����ĳ�� �� �� �����Ͽ� ��ϴ� �ڵ�ȭ����̹Ƿ� Ÿ������� ������ ������ ��������� ������� ���� �� �ֽ��ϴ�.";
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);

	m_pDevCmn->fnSCR_DisplayScreen(439, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

int CTranCmn::fnAPP_Accpet_KB_Fee_Confirm()		//#N0214
{
	CString strReturn("");
	CString strTmp("");

	strTmp = "�� �ڵ�ȭ���� ��������� ûȣ����ĳ�� �� �� �����Ͽ� ��ϴ� �ڵ�ȭ����̹Ƿ� Ÿ������� ������ ������ ��������� ������� ���� �� �ֽ��ϴ�.";
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);

	m_pDevCmn->fnSCR_DisplayScreen(439, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

int CTranCmn::fnAPP_Accpet_CITI_Fee_Confirm()		//#N0238
{
	CString strReturn("");
	CString strTmp("");

	strTmp = "�� �ڵ�ȭ���� ��Ƽ����� ûȣ����ĳ�� �� �� �����Ͽ� ��ϴ� �ڵ�ȭ����̹Ƿ� Ÿ������� ������ ������ ��������� ������� ���� �� �ֽ��ϴ�.";
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);

	m_pDevCmn->fnSCR_DisplayScreen(439, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

int CTranCmn::fnAPP_AcceptKJBTransType()
{
	CString strReturn("");
	int		ScrNum = 0;

	ScrNum = 14;

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum , K_15_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == "ī��������·���ü")
	{
		TranCode = TC_SVCTRAN;
		TranCode2 = TRANID_7250;
	}
	else 
	if (strReturn == "����Ÿ���·���ü")
	{
		TranCode = TC_SVCTRAN;
		TranCode2 = TRANID_7250;
	}
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);														
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}


int CTranCmn::fnAPP_Accpet_MS_Block_Confirm()   //#N0191
{
	CString strReturn("");
	CString strTmp("");

	m_pDevCmn->fnSCR_DisplayScreen(441, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == S_EXIT)
		fnAPP_CancelProc(T_OK);
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}


int CTranCmn::DeleteJnlFiles(LPCTSTR DirName, int DeleteDays)  //#N0205 30���� ���� ����
{
	CFileFind	finder;

	CString dir, file;
	dir.Format("%s\\*.*",DirName);

    SYSTEMTIME  lTime;
    GetLocalTime(&lTime);

	BOOL bWorking = finder.FindFile(dir);
	CFileStatus	fileStatus;

	MsgDump(TRACE_CODE_MODE, "TIM", __FILE__, __LINE__, "DeleteDays[%d]",DeleteDays);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		dir  = finder.GetFilePath();
		file = finder.GetFileName();

		if(	file == _T(".") ||
			file == _T(".."))	continue;

		if(finder.IsDirectory())
		{
			if(!DeleteJnlFiles(dir, DeleteDays))
			{
				return FALSE;
			}

			RemoveDirectory(dir);
			continue;
		}

		if (DeleteDays < 0)										// ������ ����ó��
			DeleteFile(dir);
		else
		if (!CFile::GetStatus(dir, fileStatus))	
			continue;
		else
		{
			CTime t1( fileStatus.m_mtime.GetYear(), fileStatus.m_mtime.GetMonth(), 
					  fileStatus.m_mtime.GetDay(), 0, 0, 0 );
			CTime t2( lTime.wYear, lTime.wMonth, lTime.wDay, 0, 0, 0 );
			CTimeSpan ts;
			if (t2 > t1)										// �ð�����
				ts = t2 - t1;  
			else
				ts = t2 - t2;  

			if (ts.GetDays() > DeleteDays)						// ���ʰ��� ����ó��
				DeleteFile(dir);
		}
	}

	finder.Close();
	return TRUE;
}


int CTranCmn::fnAPP_Accpet_EMV_MS_Confirm()
{
	CString strReturn("");
	CString strTmp("");

	strTmp = "���Բ��� ������ ���׳�ƽ �ſ�ī��� �� ���� �� ������� ���� �ʿ��� ������� �ڵ�ȭ��⿡�� ����� �Ұ��մϴ�. �ش� ī�� �߱޻翡�� ���̾� �ſ�ī��� ��ü�߱� ������ �� ����Ͻñ� �ٶ��ϴ�.";
	m_pDevCmn->fnSCR_DisplayString(1, strTmp);
	
	m_pDevCmn->fnSCR_DisplayScreen(442, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if ((strReturn == S_CONFIRM)	||
		(strReturn == S_CONFIRM2)	)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	||
		(strReturn == S_EXIT))				// #N0272
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}


int CTranCmn::fnAPP_Accpet_FOREGIN_INQ_Fee_Confirm()
{
	CString strReturn("");
	CString strTmp("");
	
	m_pDevCmn->fnSCR_DisplayScreen(443, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == S_YES)
		return T_OK;
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	||
		(strReturn == S_NO)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}

//#N0262
int CTranCmn::fnAPP_Accpet_AntiSkimming_Confirm()
{
	CString strReturn("");
	CString strTmp("");
	
	if (m_pDevCmn->SightHandiCapped || m_pDevCmn->EarPhoneInserted) //#N0271 -> #N0276
		return T_OK;

	m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);	//#N0271
	m_pDevCmn->fnLGT_SetFlicker(PBM_FLICKER, FLICKER_OFF);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "����귣��:!! ��Ƽ��Ű�� ���� ���!!");

	if((m_pProfile->DEVICE.ETCDevice20 == KIUP) && (m_pDevCmn->AtmDefine.MachineType == U3100K))	//#N0271
		m_pDevCmn->fnSCR_DisplayString(1, "������");
	else
		m_pDevCmn->fnSCR_DisplayString(1, "");

	m_pDevCmn->fnSCR_DisplayScreen(444, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == S_CONFIRM)
	{
		if(!m_pDevCmn->fnMCU_GetMaterialInfo()) //#N0271
			m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);

		if((MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][0] & MENU_PB) && (!m_pDevCmn->fnPBM_GetMaterialInfo())) //#N0271
			m_pDevCmn->fnLGT_SetFlicker(PBM_FLICKER, FLICKER_ON);

		return T_OK;
	}
	else 
	if ((strReturn == S_CANCEL)		||
		(strReturn == S_CANCEL2)	||
		(strReturn == S_NO)	)
		fnAPP_CancelProc(T_CANCEL);							
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 Ÿ�Ӿƿ� ��� ó��!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}


//////////////////////////////////////////////////////////////////////////
//#N0266
// Hi-Pass ���� �ݾ��Է�
int CTranCmn::fnAPP_HPSAcceptMoney()
{
	int		ScrNum = 0;
	CString strReturn("");

	ScrNum = 2916;											// ��ü�ݾ��Է�(��/õ/��)

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayNumeric(1, Asc2Int(Accept.HiPass.AvailLoadAmt, 8) / 10000);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);
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
	else
	if (Asc2Int(strReturn.GetBuffer(0), 15) > Asc2Int(Accept.HiPass.AvailLoadAmt, 8))
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "���� �Է±ݾ� �ʰ��Դϴ�");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
	{
		memcpy(Accept.Money, strReturn.GetBuffer(0), 15);
		memcpy(Accept.HiPass.LoadAmt, &Accept.Money[7], 8);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HPSAcceptMoney():Accept.HiPass.LoadAmt[%8.8s] : %s", Accept.HiPass.LoadAmt, strReturn);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ݾ��Է�  => %d����]", Asc2Int(Accept.Money, sizeof(Accept.Money))/10000));
	}

	return T_OK;
}

// Hi-Pass �ܾ���ȸȮ��
int CTranCmn::fnAPP_HPSBalanceConfirm()
{
	CString strReturn("");	
	int		ScrNum = 2913;

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	//m_pDevCmn->fnSCR_DisplayString(1, (char *) m_pDevCmn->RFHipassBalance.CardNum, 16);
	m_pDevCmn->fnSCR_DisplayAmount(1, (char *) m_pDevCmn->RFHipassBalance.Balance, 8);
	m_pDevCmn->fnSCR_DisplayAmount(2, Accept.HiPass.AvailLoadAmt, 8);

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
		memcpy(Accept.HiPass.CardNum, m_pDevCmn->RFHipassBalance.CardNum, sizeof(Accept.HiPass.CardNum));

	return T_OK;
}

// Hi-Pass �Աݱݾ�Ȯ��
int CTranCmn::fnAPP_HPSAcceptMoneyInConfirm()
{
	CString strReturn("");
	int		ScrNum = 2914;

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	if (Asc2Int(Accept.Money, 15) > Asc2Int(Accept.HiPass.AvailLoadAmt, 8))	// �����ѵ� �ʰ�
		fnAPP_CancelDepRejectProc(T_MSG, "�����ѵ��� �ʰ��Ǿ� �ŷ��� ��ҵǾ����ϴ�. ", 
										"��ȯ�� �ݾ��� Ȯ���Ͻð�, �ٽðŷ��Ͽ��ֽñ� �ٶ��ϴ�.");
																			// ����Աݹ�ȯó��

	CString strTemp = strCT(Accept.TelNum, 20);
	m_pDevCmn->fnSCR_DisplayAmount(1, Accept.Money, 15);
	m_pDevCmn->fnSCR_DisplayAmount(2, Accept.Money, 15);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelDepRejectProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);

	return T_OK;
}

// Hi-Pass �����Ϸ�Ȯ��
int CTranCmn::fnAPP_HPSLoadConfirm()
{
	CString strReturn("");	
	int		ScrNum = 2915;

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

//	m_pDevCmn->fnSCR_DisplayString(1, (char *) m_pDevCmn->RFHipassLoad2.CardNum, 16);
	m_pDevCmn->fnSCR_DisplayAmount(1, (char *) m_pDevCmn->RFHipassLoad2.Balance, 8);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CANCEL)
		return T_OK;
	else
	if (strReturn == S_TIMEOVER)
		return T_OK;

	return T_OK;
}

// Hi-Pass �ܾ���ȸ
int CTranCmn::fnAPP_HPSReadBalance()
{
	int		nReturn = 0;
	int		nLoadAmount = 0;
	int		nScreenNum = 0;										// V06-12-00-#04 Hi-Pass�ܾ���ȸ ����
	int		nCardInserted = TRUE;								// V06-13-00-#01 Hi-Pass���˽���������
	double	dTemp = 0;
	CString strReturn("");	

	memset(&m_pDevCmn->RFHipassBalance,  NULL, sizeof(m_pDevCmn->RFHipassBalance));
	MenuKCashMode = MENU_NULL;								

	if (TranCode == TC_HIPASS_CASH)
	{
		if ((DepAvail & TRAN_DEP_CASH) != TRAN_DEP_CASH)
			fnAPP_CancelProc(T_MSG, "�����н� �������� �ŷ��� �� �� �����ϴ�");

		nScreenNum = 2911; 
		fnAPD_ICCardEnDisable(ENABLE, TRUE);		
	}
	else
	if (TranCode == TC_HIPASS_BALANCE)
	{
		nScreenNum = 2911; 
		fnAPD_ICCardEnDisable(ENABLE, TRUE);		
	}
	else
	if (TranCode == TC_HIPASS_CREDIT)
	{
		//MenuTblTemp[20].MenuInfo[1][0] = MENU_MCU2;
		//fnAPP_MenuCardPbMuAccept(MENU_MCU2);
		//fnAPP_MenuCardMCU2Accept();

		fnAPD_MaterialOut(DEV_MCU);							// Card Out
		fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract
		
		nScreenNum = 2911; 
		fnAPD_ICCardEnDisable(ENABLE, TRUE);		
	}
	else
		fnAPP_CancelProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(nScreenNum, K_30_WAIT, PIN_MENU_MODE);
	//m_pDevCmn->fnSCR_DisplayScreen(nScreenNum, K_NO_WAIT, PIN_MENU_MODE);
	nReturn = m_pDevCmn->fnCMN_ScanDeviceAction(DEV_MCU | DEV_SCR, K_65_WAIT, EVENT_IN);	// #0213 #0336

	if (nReturn == DEV_MCU)		// #0336
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����н�ī�� ����]"));
		m_pDevCmn->fnSCR_DisplayScreen(2912); 
		HiPassDeviceKind = DEV_MCU;

		if (fnAPD_ICCardReadCheck() == T_ERROR)
			fnAPP_CancelProc(T_MSG, "�� ī��δ� �����н����� �ŷ��� �� �� �����ϴ�", "â���� �����Ͽ� �ֽʽÿ�");
		
		fnAPD_HPSVerifyTranBit();							// �����н� SelectApplet & Start Bit Check
		if(CancelHIPassLoadFlg)
		{
			m_pDevCmn->fnHPS_ReadPan();									// ī���ȣ��ȸ
			fnAPD_CheckDeviceAction(DEV_MCU);
			if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSREADPAN))

			CancelHIPassLoadFlg = FALSE;
			fnAPP_HIPASS_SendErrorComplet();
			if (fnAPD_ICCardReadCheck() == T_ERROR)
				fnAPP_CancelProc(T_MSG, "�� ī��δ� �����н����� �ŷ��� �� �� �����ϴ�", "â���� �����Ͽ� �ֽʽÿ�");
			fnAPD_HPSVerifyTranBit();
			if(CancelHIPassLoadFlg)
			{
				CancelHIPassLoadFlg = FALSE;
				fnAPP_CancelProc(T_MSG, "�� ī��δ� �����н����� �ŷ��� �� �� �����ϴ�", "â���� �����Ͽ� �ֽʽÿ�");
			}
		}
		else
		{
			fnAPD_HPSComplete();
			m_pDevCmn->fnHPS_ReadLLog();
			fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSLLOG);
		}
		fnAPD_HPSReadBalance();								// �����н� �ܾ���ȸ

		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);  // Flicker off

		
		if(Asc2Int(m_pDevCmn->RFHipassBalance.Balance, 8) > 485000)
		{
			CString strInform("");
			strInform.Format("[����ī���ܾ�: %d��]", Asc2Int(m_pDevCmn->RFHipassBalance.Balance, 8));

			fnAPP_CancelProc(T_MSG, "�����ѵ� �ʰ���", "�ŷ��� �Ͻ� �� �����ϴ�", strInform.GetBuffer(0));
		}
		

		nLoadAmount = 500000 - Asc2Int(m_pDevCmn->RFHipassBalance.Balance, 8);				// ���ڱ��� �ִ� �ѵ��� 485000
		dTemp = (nLoadAmount * 100 / 103 / 10000) * 10000;
		Int2Asc((int) dTemp, Accept.HiPass.AvailLoadAmt, 8);

		if (dTemp < 10000 && TranCode != TC_HIPASS_BALANCE)							// �ŷ��Ұ�
		{
			CString strInform("");
			strInform.Format("[����ī���ܾ�: %d��]", Asc2Int(m_pDevCmn->RFHipassBalance.Balance, 8));

			fnAPP_CancelProc(T_MSG, "�����ѵ� �ʰ���", "�ŷ��� �Ͻ� �� �����ϴ�", strInform.GetBuffer(0));				
		}
		
		fnAPP_HPSBalanceConfirm();
		if (TranCode == TC_HIPASS_BALANCE)
			return T_OK;

	}
	else
	if (nReturn == DEV_SCR)
	{
		strReturn = m_pDevCmn->fstrSCR_GetKeyString(FALSE);
		m_pDevCmn->fnMCU_Eject();
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else fnAPP_CancelProc(T_TIMEOVER);
	}
	else
		fnAPP_CancelProc(T_TIMEOVER);

	return T_OK;
}

// Hi-Pass ����S1
int CTranCmn::fnAPP_HPSLoadS1()
{
	int		nReturn = 0;

	memset(&m_pDevCmn->RFHipassLoad1,  NULL, sizeof(m_pDevCmn->RFHipassLoad1));
	memset(m_pDevCmn->RFHipassErrorCode, 0x00, sizeof(m_pDevCmn->RFHipassErrorCode));

	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
		return T_OK;

	if (HiPassDeviceKind == DEV_MCU)
	{
		nReturn = DEV_MCU;
	}
	else
	{
		fnAPP_CancelProc(T_MSG, "Hi-Pass �����ŷ��� �� �� �����ϴ�. ", "â���� �����Ͽ� �ֽʽÿ�");
	}

	if (nReturn == DEV_MCU)
	{
		// TEST
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HPSLoadS1():Accept.TelNum[%11.11s]", Accept.TelNum);
		if (!memcmp(Accept.TelNum, "01000000001", 11))
			return T_ERROR;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HPSLoadS1():Accept.HiPass.LoadAmt[%8.8s]", Accept.HiPass.LoadAmt);
		if (fnAPD_HPSPrepareLoad(Accept.HiPass.LoadAmt) == T_ERROR)
		{
			HiPassLoadFailFlg = TRUE;
			fnAPP_HPSComplete();
			return T_ERROR;
		}
	}
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

// Hi-Pass ����S2
int	CTranCmn::fnAPP_HPSLoadS2()
{
	int		nReturn = 0;
	BYTE	random_num[16] = {0x00, };
	BYTE	s2[8] = {0x00,};
	CString strReturn("0");

	memset(&m_pDevCmn->RFHipassLoad2,  NULL, sizeof(m_pDevCmn->RFHipassLoad2));
	memset(m_pDevCmn->RFHipassErrorCode, 0x00, sizeof(m_pDevCmn->RFHipassErrorCode));

	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
		return T_OK;

	if (HiPassDeviceKind == DEV_MCU)							// Hi-Pass���˽���������
	{
		nReturn = DEV_MCU;
	}
	else
	{
		fnAPP_CancelProc(T_MSG, "Hi-Pass �����ŷ��� �� �� �����ϴ�. ", "â���� �����Ͽ� �ֽʽÿ�");
	}

	if (nReturn == DEV_MCU)
	{
		// TEST
		if (!memcmp(Accept.TelNum, "01000000002", 11))
		{														// ��������
			HiPassLoadFailFlg = TRUE;
			fnAPP_HPSComplete();
			return T_ERROR;
		}

		if (TranCode == TC_HIPASS_CASH)
		{
			memcpy(random_num, m_RXHIPASS.m_RXATM4670.random_num, sizeof(random_num));
			memcpy(s2, m_RXHIPASS.m_RXATM4670.s2, sizeof(s2));
		}
		else if (TranCode == TC_HIPASS_CREDIT)
		{
			memcpy(random_num, m_RXHIPASS.m_RXATM4670.random_num, sizeof(random_num));
			memcpy(s2, m_RXHIPASS.m_RXATM4670.s2, sizeof(s2));
		}
		else
			fnAPP_CancelProc(T_PROGRAM);

		HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_HPSLoad random_num()", random_num, sizeof(random_num));
		HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_HPSLoad s2()", s2, sizeof(s2));

		m_pDevCmn->fnSCR_DisplayString(1, "��û�Ͻ� �ݾ��� �������Դϴ�.");
		m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�.");

		m_pDevCmn->fnSCR_DisplayScreen(701);						// ������

		if (fnAPD_HPSLoad(random_num, s2) == T_ERROR)
		{														// ��������
			HiPassLoadFailFlg = TRUE;
			fnAPP_HPSComplete();
			return T_ERROR;
		}

		m_pDevCmn->fnHPS_ReadBalance();
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_READBALANCE))// �����Ϸ��� �ܾ���ȸ���н� �������ܾװ��
		{
			memcpy(m_pDevCmn->RFHipassLoad2.Balance, m_pDevCmn->RFHipassLoad1.CardBalance, 8);
			AddString(m_pDevCmn->RFHipassLoad2.Balance, Accept.HiPass.LoadAmt, 8);
			memcpy(m_pDevCmn->RFHipassLoad2.CardNum, m_pDevCmn->RFHipassLoad1.CardNum, sizeof(m_pDevCmn->RFHipassLoad2.CardNum));
		}
		else
		{
			memcpy(m_pDevCmn->RFHipassLoad2.Balance, m_pDevCmn->RFHipassBalance.Balance, sizeof(m_pDevCmn->RFHipassLoad2.Balance));
			memcpy(m_pDevCmn->RFHipassLoad2.CardNum, m_pDevCmn->RFHipassBalance.CardNum, sizeof(m_pDevCmn->RFHipassLoad2.CardNum));
		}
	}
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

// Hi-Pass �Ϸ�
int CTranCmn::fnAPP_HPSComplete()
{
	int		nReturn = 0;

	if (HiPassDeviceKind == DEV_MCU)
	{
		nReturn = DEV_MCU;
	}
	else
	{
		fnAPP_CancelProc(T_MSG, "Hi-Pass �����ŷ��� �� �� �����ϴ�. ", "â���� �����Ͽ� �ֽʽÿ�");
	}

	if (nReturn == DEV_MCU)
	{
		if (fnAPD_HPSComplete())
		{
			HiPassLoadFailFlg = TRUE;
			return T_ERROR;
		}
	}
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

int CTranCmn::fnAPP_HPSUnloadS1()								// Hi-Pass ��������S1
{
	return T_OK;
}
int CTranCmn::fnAPP_HPSUnloadS2()								// Hi-Pass ��������S2
{
	return T_OK;
}

// ��й�ȣ�Է�
int CTranCmn::fnAPP_HIPASS_AcceptPassword()
{
	int		ScrNum = 0;
	CString strReturn("");
	int			nCnt = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PassWord, ' ', sizeof(Accept.PassWord));		// ��й�ȣ
	memset(Accept.PassWordEnc, ' ', sizeof(Accept.PassWordEnc));		// ��й�ȣ
/////////////////////////////////////////////////////////////////////////////

	ScrNum = 2925;
	
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
			if (!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))
			{
					nCnt++;
					continue;
			}				
		}
		break;
	}

	memcpy(Accept.PassWord, strReturn.GetBuffer(0), strReturn.GetLength());

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��й�ȣ  => �Է¿Ϸ�]", strReturn.GetLength()));
	m_pDevCmn->fnAPL_CaptureFaceSave(4, FALSE, fnAPP_GetCaptureData());		// ���Կ�


	return T_OK;
}

//////////////////////////////////////////////////////////////////////////
