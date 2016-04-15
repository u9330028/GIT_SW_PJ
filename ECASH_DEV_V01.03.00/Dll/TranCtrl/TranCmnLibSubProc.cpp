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

// 주민번호입력
int CTranCmn::fnAPP_AcceptCmnLibJuminNum()
{
	CString strReturn("");
	int		ScrNum = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.JuminNum, ' ', sizeof(Accept.JuminNum));		// 주민번호
/////////////////////////////////////////////////////////////////////////////

	if(ExtraServiceFlag == TC_NOMEDIA_HYUNCAPTIAL)
		ScrNum = 1166;	//760;	#0356
	else
	if(ExtraServiceFlag == TC_INTERAD)
		ScrNum = 2202;
	else
	if(ExtraServiceFlag == TC_HIPASS_CREDIT)
		ScrNum = 2924;									// HI-PASS 충전서비스 결제정보(생년월일) 추가 #0443
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
	if ((!strReturn.GetLength())	||							// 자료검증
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

// 할부금액
int	CTranCmn::fnAPP_AcceptInstallmentPlan()
{
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.AutoDate, ' ', sizeof(Accept.AutoDate));		// 자동이체일
	memset(Accept.AutoCycle, ' ', sizeof(Accept.AutoCycle));	// 이체주기
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(763, K_30_WAIT, PIN_NUMERIC_MODE);
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
	if (strReturn == "6개월")
		memcpy(Accept.AutoCycle, "06", 2);
	else
	if (strReturn == "10개월")
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
			fnAPP_CancelProc(T_CANCEL);							// 취소함수(정상업무처리)
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);						// 취소함수(정상업무처리)
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);						// 취소함수(정상업무처리)
		else
			fnAPP_CancelProc(T_PROGRAM);						// 취소함수(정상업무처리)
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
// 통합기장 안내 문구
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

// 통합기장 안내 문구
int CTranCmn::fnAPP_Accpet_KEB_PbConfirmMSG2()
{
	CString strReturn("");

	m_pDevCmn->fnSCR_DisplayScreen(552, K_15_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();


	if (strReturn == "통합인자")
	{
		TranCode = TC_PBOOKTRM;
	}
	else 
	if (strReturn == "통장정리")
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

	strTmp = "본 기기는 밴사 제휴 에이티엠이므로 하나은행 및 타행고객은 밴사 수수료가 부과됩니다.";
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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

	strTmp = "본 자동화기기는 IBK기업은행과 청호이지캐쉬 주 가 제휴하여 운영하는 자동화기기이므로 타금융기관 고객께는 수수료 우대혜택이 적용되지 않을 수 있습니다.";
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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

	strTmp = "본 자동화기기는 농협은행과 청호이지캐쉬 주 가 제휴하여 운영하는 자동화기기이므로 타금융기관 고객께는 수수료 우대혜택이 적용되지 않을 수 있습니다.";
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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

	strTmp = "본 자동화기기는 광주은행과 청호이지캐쉬 주 가 제휴하여 운영하는 자동화기기이므로 타금융기관 고객께는 수수료 우대혜택이 적용되지 않을 수 있습니다.";
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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

	strTmp = "본 자동화기기는 경남은행과 청호이지캐쉬 주 가 제휴하여 운영하는 자동화기기이므로 타금융기관 고객께는 수수료 우대혜택이 적용되지 않을 수 있습니다.";
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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

	strTmp = "본 자동화기기는 하나은행과 청호이지캐쉬 주 가 제휴하여 운영하는 자동화기기이므로 타금융기관 고객께는 수수료 우대혜택이 적용되지 않을 수 있습니다.";
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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

	strTmp = "본 자동화기기는 신한은행과 청호이지캐쉬 주 가 제휴하여 운영하는 자동화기기이므로 타금융기관 고객께는 수수료 우대혜택이 적용되지 않을 수 있습니다.";
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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

	strTmp = "본 자동화기기는 SC은행과 청호이지캐쉬 주 가 제휴하여 운영하는 자동화기기이므로 타금융기관 고객께는 수수료 우대혜택이 적용되지 않을 수 있습니다.";
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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

	strTmp = "본 자동화기기는 국민은행과 청호이지캐쉬 주 가 제휴하여 운영하는 자동화기기이므로 타금융기관 고객께는 수수료 우대혜택이 적용되지 않을 수 있습니다.";
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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

	strTmp = "본 자동화기기는 씨티은행과 청호이지캐쉬 주 가 제휴하여 운영하는 자동화기기이므로 타금융기관 고객께는 수수료 우대혜택이 적용되지 않을 수 있습니다.";
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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

	if (strReturn == "카드결제계좌로이체")
	{
		TranCode = TC_SVCTRAN;
		TranCode2 = TRANID_7250;
	}
	else 
	if (strReturn == "당행타계좌로이체")
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}


int CTranCmn::DeleteJnlFiles(LPCTSTR DirName, int DeleteDays)  //#N0205 30일전 파일 삭제
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

		if (DeleteDays < 0)										// 무조건 삭제처리
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
			if (t2 > t1)										// 시간보정
				ts = t2 - t1;  
			else
				ts = t2 - t2;  

			if (ts.GetDays() > DeleteDays)						// 일초과시 삭제처리
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

	strTmp = "고객님께서 소지한 마그네틱 신용카드는 위 변조 등 사고예방을 위해 십오년 삼월부터 자동화기기에서 사용이 불가합니다. 해당 카드 발급사에서 아이씨 신용카드로 교체발급 받으신 후 사용하시길 바랍니다.";
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
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

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "기업브랜드:!! 안티스키밍 문구 출력!!");

	if((m_pProfile->DEVICE.ETCDevice20 == KIUP) && (m_pDevCmn->AtmDefine.MachineType == U3100K))	//#N0271
		m_pDevCmn->fnSCR_DisplayString(1, "인터폰");
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
		fnAPP_CancelProc(T_TIMEOVER);		//#N0189 타임아웃 취소 처리!									
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);					
	else
		fnAPP_CancelProc(T_PROGRAM);					

	return T_OK;
}


//////////////////////////////////////////////////////////////////////////
//#N0266
// Hi-Pass 충전 금액입력
int CTranCmn::fnAPP_HPSAcceptMoney()
{
	int		ScrNum = 0;
	CString strReturn("");

	ScrNum = 2916;											// 이체금액입력(만/천/원)

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
	if ((strReturn.GetLength() != 15)	||						// 자료검증
		(!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	||		
		(IsZero(strReturn.GetBuffer(0), strReturn.GetLength())))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	if (Asc2Int(strReturn.GetBuffer(0), 15) > Asc2Int(Accept.HiPass.AvailLoadAmt, 8))
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "충전 입력금액 초과입니다");
		else
			fnAPP_CancelProc(T_CANCEL);
	}
	else
	{
		memcpy(Accept.Money, strReturn.GetBuffer(0), 15);
		memcpy(Accept.HiPass.LoadAmt, &Accept.Money[7], 8);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HPSAcceptMoney():Accept.HiPass.LoadAmt[%8.8s] : %s", Accept.HiPass.LoadAmt, strReturn);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[충전금액입력  => %d만원]", Asc2Int(Accept.Money, sizeof(Accept.Money))/10000));
	}

	return T_OK;
}

// Hi-Pass 잔액조회확인
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

// Hi-Pass 입금금액확인
int CTranCmn::fnAPP_HPSAcceptMoneyInConfirm()
{
	CString strReturn("");
	int		ScrNum = 2914;

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	if (Asc2Int(Accept.Money, 15) > Asc2Int(Accept.HiPass.AvailLoadAmt, 8))	// 충전한도 초과
		fnAPP_CancelDepRejectProc(T_MSG, "충전한도가 초과되어 거래가 취소되었습니다. ", 
										"반환된 금액을 확인하시고, 다시거래하여주시기 바랍니다.");
																			// 취소입금반환처리

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

// Hi-Pass 충전완료확인
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

// Hi-Pass 잔액조회
int CTranCmn::fnAPP_HPSReadBalance()
{
	int		nReturn = 0;
	int		nLoadAmount = 0;
	int		nScreenNum = 0;										// V06-12-00-#04 Hi-Pass잔액조회 개발
	int		nCardInserted = TRUE;								// V06-13-00-#01 Hi-Pass접촉식충전개발
	double	dTemp = 0;
	CString strReturn("");	

	memset(&m_pDevCmn->RFHipassBalance,  NULL, sizeof(m_pDevCmn->RFHipassBalance));
	MenuKCashMode = MENU_NULL;								

	if (TranCode == TC_HIPASS_CASH)
	{
		if ((DepAvail & TRAN_DEP_CASH) != TRAN_DEP_CASH)
			fnAPP_CancelProc(T_MSG, "하이패스 현금충전 거래를 할 수 없습니다");

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
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[하이패스카드 투입]"));
		m_pDevCmn->fnSCR_DisplayScreen(2912); 
		HiPassDeviceKind = DEV_MCU;

		if (fnAPD_ICCardReadCheck() == T_ERROR)
			fnAPP_CancelProc(T_MSG, "이 카드로는 하이패스충전 거래를 할 수 없습니다", "창구에 문의하여 주십시오");
		
		fnAPD_HPSVerifyTranBit();							// 하이패스 SelectApplet & Start Bit Check
		if(CancelHIPassLoadFlg)
		{
			m_pDevCmn->fnHPS_ReadPan();									// 카드번호조회
			fnAPD_CheckDeviceAction(DEV_MCU);
			if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSREADPAN))

			CancelHIPassLoadFlg = FALSE;
			fnAPP_HIPASS_SendErrorComplet();
			if (fnAPD_ICCardReadCheck() == T_ERROR)
				fnAPP_CancelProc(T_MSG, "이 카드로는 하이패스충전 거래를 할 수 없습니다", "창구에 문의하여 주십시오");
			fnAPD_HPSVerifyTranBit();
			if(CancelHIPassLoadFlg)
			{
				CancelHIPassLoadFlg = FALSE;
				fnAPP_CancelProc(T_MSG, "이 카드로는 하이패스충전 거래를 할 수 없습니다", "창구에 문의하여 주십시오");
			}
		}
		else
		{
			fnAPD_HPSComplete();
			m_pDevCmn->fnHPS_ReadLLog();
			fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSLLOG);
		}
		fnAPD_HPSReadBalance();								// 하이패스 잔액조회

		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);  // Flicker off

		
		if(Asc2Int(m_pDevCmn->RFHipassBalance.Balance, 8) > 485000)
		{
			CString strInform("");
			strInform.Format("[현재카드잔액: %d원]", Asc2Int(m_pDevCmn->RFHipassBalance.Balance, 8));

			fnAPP_CancelProc(T_MSG, "충전한도 초과로", "거래를 하실 수 없습니다", strInform.GetBuffer(0));
		}
		

		nLoadAmount = 500000 - Asc2Int(m_pDevCmn->RFHipassBalance.Balance, 8);				// 전자금융 최대 한도는 485000
		dTemp = (nLoadAmount * 100 / 103 / 10000) * 10000;
		Int2Asc((int) dTemp, Accept.HiPass.AvailLoadAmt, 8);

		if (dTemp < 10000 && TranCode != TC_HIPASS_BALANCE)							// 거래불가
		{
			CString strInform("");
			strInform.Format("[현재카드잔액: %d원]", Asc2Int(m_pDevCmn->RFHipassBalance.Balance, 8));

			fnAPP_CancelProc(T_MSG, "충전한도 초과로", "거래를 하실 수 없습니다", strInform.GetBuffer(0));				
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

// Hi-Pass 충전S1
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
		fnAPP_CancelProc(T_MSG, "Hi-Pass 충전거래를 할 수 없습니다. ", "창구에 문의하여 주십시오");
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

// Hi-Pass 충전S2
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

	if (HiPassDeviceKind == DEV_MCU)							// Hi-Pass접촉식충전개발
	{
		nReturn = DEV_MCU;
	}
	else
	{
		fnAPP_CancelProc(T_MSG, "Hi-Pass 충전거래를 할 수 없습니다. ", "창구에 문의하여 주십시오");
	}

	if (nReturn == DEV_MCU)
	{
		// TEST
		if (!memcmp(Accept.TelNum, "01000000002", 11))
		{														// 충전실패
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

		m_pDevCmn->fnSCR_DisplayString(1, "요청하신 금액을 충전중입니다.");
		m_pDevCmn->fnSCR_DisplayString(3, "잠시만 기다려 주십시오.");

		m_pDevCmn->fnSCR_DisplayScreen(701);						// 충전중

		if (fnAPD_HPSLoad(random_num, s2) == T_ERROR)
		{														// 충전실패
			HiPassLoadFailFlg = TRUE;
			fnAPP_HPSComplete();
			return T_ERROR;
		}

		m_pDevCmn->fnHPS_ReadBalance();
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_READBALANCE))// 충전완료후 잔액조회실패시 충전후잔액계산
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

// Hi-Pass 완료
int CTranCmn::fnAPP_HPSComplete()
{
	int		nReturn = 0;

	if (HiPassDeviceKind == DEV_MCU)
	{
		nReturn = DEV_MCU;
	}
	else
	{
		fnAPP_CancelProc(T_MSG, "Hi-Pass 충전거래를 할 수 없습니다. ", "창구에 문의하여 주십시오");
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

int CTranCmn::fnAPP_HPSUnloadS1()								// Hi-Pass 충전복구S1
{
	return T_OK;
}
int CTranCmn::fnAPP_HPSUnloadS2()								// Hi-Pass 충전복구S2
{
	return T_OK;
}

// 비밀번호입력
int CTranCmn::fnAPP_HIPASS_AcceptPassword()
{
	int		ScrNum = 0;
	CString strReturn("");
	int			nCnt = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PassWord, ' ', sizeof(Accept.PassWord));		// 비밀번호
	memset(Accept.PassWordEnc, ' ', sizeof(Accept.PassWordEnc));		// 비밀번호
/////////////////////////////////////////////////////////////////////////////

	ScrNum = 2925;
	
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
			if (!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))
			{
					nCnt++;
					continue;
			}				
		}
		break;
	}

	memcpy(Accept.PassWord, strReturn.GetBuffer(0), strReturn.GetLength());

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[비밀번호  => 입력완료]", strReturn.GetLength()));
	m_pDevCmn->fnAPL_CaptureFaceSave(4, FALSE, fnAPP_GetCaptureData());		// 얼굴촬영


	return T_OK;
}

//////////////////////////////////////////////////////////////////////////
