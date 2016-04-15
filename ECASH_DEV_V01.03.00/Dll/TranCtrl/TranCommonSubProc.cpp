#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\MacroDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"
#include "..\..\H\Tran\TranInputProc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**********************************************************************************************************/
/* Function Name : CommonSubProc Make (send) gijin
/* 
/* Date			 : 2010.10.14
/* 
/* Input	     : TRCODE
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/  

int CTranCmn::fnAPP_TranCommonInitProc()
{

	fnAPP_InitializeSerialTranDS();														

	switch (TranProc)
	{
	case TRAN_UTILYBILLS:										// 지로공과금
		fnAPP_UTILYBILLS_InitProc();
		break;
	case TRAN_LOCALTAX:											// 지방세
		fnAPP_LOCALTAX_InitProc();
		break;
	case TRAN_FOREGIN:											// // 해외카드 #N0171
	case TRAN_FOREGIN_INQ:										// // 해외카드 조회 #N0171
		fnAPP_FOREGIN_InitProc();
		break;
	case TRAN_HIPASS:											// 하이패스 충전 #N0266
		fnAPP_HIPASS_InitProc();
		break;
	default:
		break;
		
	}

	return T_OK;
}


int CTranCmn::fnAPP_TranCommonInputProc()
{
	switch (TranProc)
	{
	case TRAN_UTILYBILLS:										// 지로공과금
		fnAPP_UTILYBILLS_InputProc();
		break;
	case TRAN_LOCALTAX:										// 지로공과금
		fnAPP_LOCALTAX_InputProc();
		break;
	case TRAN_FOREGIN:											// // 해외카드 #N0171
	case TRAN_FOREGIN_INQ:										// // 해외카드 조회 #N0171
		fnAPP_FOREGIN_InputProc();
		break;
	case TRAN_HIPASS:											// 하이패스 충전 #N0266
		fnAPP_HIPASS_InputProc();
		break;	
	default:
		break;
		
	}

	return T_OK;
}

int CTranCmn::fnAPP_TranCommonOutputProc()
{
	switch (TranProc)
	{
	case TRAN_UTILYBILLS:										// 지로공과금
		fnAPP_UTILYBILLS_OutputProc();
		break;
	case TRAN_LOCALTAX:										// 지로공과금
		fnAPP_LOCALTAX_OutputProc();
		break;
	case TRAN_FOREGIN:											// // 해외카드 #N0171
	case TRAN_FOREGIN_INQ:										// // 해외카드 조회 #N0171
		fnAPP_FOREGIN_OutputProc();
		break;
	case TRAN_HIPASS:											// 하이패스 충전 #N0266
		fnAPP_HIPASS_OutputProc();
		break;
	default:
		break;
		
	}

	return T_OK;
}

int CTranCmn::fnAPP_TranCommonCheckDeviceActionProc()
{
	switch (TranProc)
	{
	case TRAN_UTILYBILLS:										// 지로공과금
		fnAPP_UTILYBILLS_CheckDeviceActionProc();
		break;
	case TRAN_LOCALTAX:										// 지로공과금
		fnAPP_LOCALTAX_CheckDeviceActionProc();
		break;
	case TRAN_FOREGIN:											// // 해외카드 #N0171
	case TRAN_FOREGIN_INQ:										// // 해외카드 조회 #N0171
		fnAPP_FOREGIN_CheckDeviceActionProc();
		break;
	case TRAN_HIPASS:											// 하이패스 충전 #N0266
		fnAPP_HIPASS_CheckDeviceActionProc();
		break;
	default:
		break;
		
	}
	
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);
	return T_OK;
}


int CTranCmn::fnAPP_TranCommonCheckMenuContinueProc()
{
	switch (TranProc)
	{
	case TRAN_UTILYBILLS:										// 지로공과금
		fnAPP_UTILYBILLS_CheckMenuContinueProc();
		break;
	case TRAN_LOCALTAX:										// 지로공과금
		fnAPP_LOCALTAX_CheckMenuContinueProc();
		break;
	case TRAN_FOREGIN:											// 해외카드 #N0171
	case TRAN_FOREGIN_INQ:										// // 해외카드 조회 #N0171
		fnAPP_FOREGIN_CheckMenuContinueProc();
		break;
	case TRAN_HIPASS:											// 하이패스 충전	#N0266
		fnAPP_HIPASS_CheckMenuContinueProc();
		break;
	default:
		break;
		
	}

	return T_OK;
}


int CTranCmn::fnAPP_SubStep_Accept_Card_Proc()
{
	//Card Avail Setting
	MenuTblTemp[0].MenuInfo[10][0] = MENU_MCU2;

	fnAPP_MenuCardPbMuAccept(MENU_MCU);

	if(CardTran & MENU_MCU2);
	else
	{
		fnAPP_CancelProc(T_DISMSG, TranMsgTbl[6][m_pDevCmn->fnSCR_GetCurrentLangMode()]);	
	}

	if (!MenuFICMode)
	{
		fnAPD_MaterialOut(DEV_MCU);							// Card Out
		fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract
	}


	return T_OK;
}


/********************************************************************************************************************
/*
/* 지로공과금 Proc : 2010.10.14   Kim.Gi.Jin
/*
/********************************************************************************************************************/

int	CTranCmn::fnAPP_UTILYBILLS_InitProc()
{
	memset(m_RXJIRODATA.JiroBankCode, ' ', sizeof(m_RXJIRODATA));
	m_RXJIRODATA.JiroPayData[0] = 'C';
	memset(Accept.JiroBillCount, '0', sizeof(Accept.JiroBillCount)); //#N0162-1

	//#N0191
	if ( !(MenuFICMode & MENU_IC) && (MenuAvail & MENU_MCU3) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		fnAPP_Accpet_MS_Block_Confirm();
	}
	else  //#N0257
	if ( !(MenuFICMode & MENU_IC) && (MenuAvail & MENU_MCU3) && (m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE))
	{
		fnAPP_CancelProc(T_MSG, "Magnetic stripe cards are not available anymore.", "Please use IC card.");
	}

	return T_OK;
}
int	CTranCmn::fnAPP_UTILYBILLS_InputProc()
{
	fnAPP_UTILYBILLS_Sub01_InputProc();
	//#GIRO 지로대금 외의것은 '0' 7자리
	memset(Accept.JiroNum, '0', sizeof(Accept.JiroNum));		// 지로번호
	if(Accept.JiroTranCode == TC_JRJIRO)							// 지로대금납입
		fnAPP_UTILYBILLS_Sub02_InputProc();							// 지로번호입력
	
	fnAPP_UTILYBILLS_Sub03_InputProc();								// 지로전자납부번호입력
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "JiroTranOrgCode: [%9.9s] JiroNum(%7.7s)", Accept.JiroTranOrgCode, Accept.JiroNum);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "JiroElecNum: [%19.19s]", Accept.JiroElecNum);
	fnAPP_AcceptPassword();

	m_pDevCmn->fnAPL_SetProcCount('3');						
	if(TranCode2 == TRANID_4810)
	{
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
			fnAPP_AcceptSlipChoice();
		else
		{
			m_pDevCmn->fnSCR_DisplayScreen(526);
			Delay_Msg(2000);
		}
		fnAPP_SendHost();										
		fnAPP_RecvHost();	
		
		if (m_pDevCmn->TranResult)								
			m_pDevCmn->fnAPL_SetProcCount('5');					
		
		fnAPP_UTILYBILLS_Sub01_A_OutputProc();
		
		if(m_RXJIRODATA.JiroPayData[1] == '0')
		{
			fnAPP_UTILYBILLS_SR1_Sub01_OutputProc();
		}

		AddSerialFlg = FALSE;
		TranCode2Save = TRANID_4810;
	}
	else
	{
		TranCode2Save = TRANID_7321;
	}



	return T_OK;
}
int	CTranCmn::fnAPP_UTILYBILLS_OutputProc()
{

	if(TranCode2Save == TRANID_7321)
	{
		fnAPP_UTILYBILLS_Sub01_B_OutputProc();
		fnAPP_PSPProc(DEV_JPR);
		Accept.SlipChoiceFlag = FALSE;
	}
	else
	if(TranCode2Save == TRANID_4810)
	{
		//1회 Send, Recv = SR1 Next Step
		//fnAPP_UTILYBILLS_SR1_Sub01_OutputProc();
		//Accept.SlipChoiceFlag = TRUE;  #N0197
		if(!Accept.SlipChoiceFlag)
		{
			m_pDevCmn->fnSCR_DisplayString(3, "이용해 주셔서 감사합니다");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(2 * 1000);
		}

		fnAPP_PSPProc(DEV_JPR);
	}

	fnAPP_PSPProc(DEV_SPR);
	fnAPD_CheckDeviceAction(DEV_SPR);
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				
	fnAPD_MaterialOutCheckRetract(DEV_SPR |DEV_MCU | DEV_PBM);  //#N0147

	return T_OK;
}

int	CTranCmn::fnAPP_UTILYBILLS_CheckDeviceActionProc()
{
	return T_OK;
}
int	CTranCmn::fnAPP_UTILYBILLS_CheckMenuContinueProc()
{
	return T_OK;
}



int	CTranCmn::fnAPP_LOCALTAX_InitProc()
{
	memset(m_RXLOCALTAXDATA.JiroBankCode, ' ', sizeof(m_RXLOCALTAXDATA));
	m_RXLOCALTAXDATA.JiroPayData[0] = 'C';
	memset(Accept.JiroPayMonth, ' ', sizeof(Accept.JiroPayMonth));
	memset(Accept.JiroElecNum , ' ', sizeof(Accept.JiroElecNum));

	//#N0191
	if ( !(MenuFICMode & MENU_IC) && (MenuAvail & MENU_MCU3) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		fnAPP_Accpet_MS_Block_Confirm();
	}
	else  //#N0257
	if ( !(MenuFICMode & MENU_IC) && (MenuAvail & MENU_MCU3) && (m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE))
	{
		fnAPP_CancelProc(T_MSG, "Magnetic stripe cards are not available anymore.", "Please use IC card.");
	}

	return T_OK;
}

int	CTranCmn::fnAPP_LOCALTAX_InputProc()
{

	if(TranCode == TC_UTILYBILLS_02)
	{
//		fnAPP_LOCALTAX_Sub01_InputProc();
		fnAPP_LOCALTAX_AcceptSimpleNum();
		memcpy(m_RXLOCALTAXDATA.Gubun, "02", 2);
	}
	else
	if(TranCode == TC_UTILYBILLS_03)
	{
		memcpy(m_RXLOCALTAXDATA.Gubun, "03", 2);
		fnAPP_LOCALTAX_AcceptElecNum();
	}
	else
	if(TranCode == TC_UTILYBILLS_01)
	{
		memcpy(m_RXLOCALTAXDATA.Gubun, "01", 2);
	}
	else
	{
		memcpy(m_RXLOCALTAXDATA.Gubun, "01", 2);
	}

	//납부결과조회시 전자납부번호입력
	if(TranCode2 == TRANID_7326 || TranCode2 == TRANID_7329)
		;
	else
	{
		fnAPP_AcceptPassword();
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
			fnAPP_AcceptSlipChoice();
		else
		{
			m_pDevCmn->fnSCR_DisplayScreen(526);
			Delay_Msg(2000);
		}
	}

	m_pDevCmn->fnAPL_SetProcCount('3');						
	if(TranCode2 == TRANID_4811)
	{
		fnAPP_SendHost();										
		fnAPP_RecvHost();	
	
		if (m_pDevCmn->TranResult)								
			m_pDevCmn->fnAPL_SetProcCount('5');					
		
		if(TranCode != TC_UTILYBILLS_03)
			fnAPP_LOCALTAX_Sub01_OutputProc();
		
		memset(Accept.JiroPayMonth, '0', sizeof(Accept.JiroPayMonth));
		
		if(Accept.JiroTranCode == TC_ELECINQ)
		{
			memset(m_RXLOCALTAXDATA.JiroBankCode, ' ', sizeof(m_RXLOCALTAXDATA));
			m_RXLOCALTAXDATA.JiroPayData[0] = 'C';
			memcpy(m_RXLOCALTAXDATA.Gubun, "03", 2);

			AddSerialFlg = FALSE;
			fnAPP_SendHost();										
			fnAPP_RecvHost();
		}
		

		fnAPP_LOCALTAX_Sub02_OutputProc();

		AddSerialFlg = FALSE;
		TranCode2Save = TRANID_4811;
	}
	else
	if(TranCode2 == TRANID_4812)
	{
		fnAPP_SendHost();										
		fnAPP_RecvHost();	
		
		if (m_pDevCmn->TranResult)								
			m_pDevCmn->fnAPL_SetProcCount('5');					
		
		if(TranCode != TC_UTILYBILLS_03)  //#N0165-1
			fnAPP_LOCALTAX_Sub01_OutputProc();
		
		memset(Accept.JiroPayMonth, '0', sizeof(Accept.JiroPayMonth));

		if(Accept.JiroTranCode == TC_ELECINQ)
		{
			memset(m_RXLOCALTAXDATA.JiroBankCode, ' ', sizeof(m_RXLOCALTAXDATA));
			m_RXLOCALTAXDATA.JiroPayData[0] = 'C';
			memcpy(m_RXLOCALTAXDATA.Gubun, "03", 2);

			AddSerialFlg = FALSE;
			fnAPP_SendHost();										
			fnAPP_RecvHost();
		}
		
		fnAPP_LOCALTAX_Sub02_OutputProc();
		
		if(Asc2Int(Accept.Money, sizeof(Accept.Money)) >= 50000 )	
		{
			fnAPP_LOCALTAX_Sub02_InputProc();
		}
		else
			memset(Accept.JiroPayMonth, '0', sizeof(Accept.JiroPayMonth));
		AddSerialFlg = FALSE;
		TranCode2Save = TRANID_4812;

	}
	else 
	{
		TranCode2Save = TranCode2;
	}

	return T_OK;
}

int	CTranCmn::fnAPP_LOCALTAX_OutputProc()
{

	if(TranCode2Save == TRANID_7326)
	{
		fnAPP_LOCALTAX_Sub03_OutputProc();
		fnAPP_PSPProc(DEV_JPR);
		Accept.SlipChoiceFlag = FALSE;
	}
	else
	{
		fnAPP_LOCALTAX_Sub03_OutputProc();
		fnAPP_PSPProc(DEV_JPR);
	}

	fnAPP_PSPProc(DEV_SPR);
	fnAPD_CheckDeviceAction(DEV_SPR);
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				
	fnAPD_MaterialOutCheckRetract(DEV_SPR |DEV_MCU | DEV_PBM);  //#N0147

	return T_OK;
}

int	CTranCmn::fnAPP_LOCALTAX_CheckDeviceActionProc()
{
	return T_OK;
}
int	CTranCmn::fnAPP_LOCALTAX_CheckMenuContinueProc()
{
	return T_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#N0171
int	CTranCmn::fnAPP_FOREGIN_InitProc()
{
	m_pDevCmn->fnSCR_SetCurrentLangMode(FOR_MODE);

	
	if((TranProc == TRAN_FOREGIN_INQ) && (Accept.MuConfirmNum[0] == 'E')) // 금결원 해외카드 조회에 수수료 부과됨
	{
		fnAPP_Accpet_FOREGIN_INQ_Fee_Confirm();
	}
	
	return T_OK;
}

int	CTranCmn::fnAPP_FOREGIN_InputProc()
{

	if (MainTranDoingCount == 0)
	{
		fnAPP_InitializeSerialTranDS();							// Initialize Serial Transaction DS
		if(TranProc == TRAN_FOREGIN)
			fnAPP_AcceptWith();										// 출금입력
		else
		if(TranProc == TRAN_FOREGIN_INQ)
			fnAPP_AcceptInq();										// 출금입력
	}

	if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
	{
		if(TranProc == TRAN_FOREGIN)
			fnAPP_AcceptSlipChoice();
	}
	else
	{
		m_pDevCmn->fnSCR_DisplayScreen(526);
		Delay_Msg(2000);
	}
	
	//출금
	if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranProc == TRAN_FOREGIN) && (TranCode2 == TRANID_4330)) 
	{
		
		
		TranCode2 = TRANID_7530;
		TranCode2Save = TRANID_7530;
		
		TranEncCode = fnAPP_EncType(TranCode2);
		fnAPP_SendHost();										
		fnAPP_RecvHost();

		fnAPP_TranWithChargeInqInputProc();
		AddSerialFlg = FALSE;
		TranResultNGCode = 0;

		TranCode2 = TRANID_4330;
		TranCode2Save = TRANID_4330;
	}


	fnAPD_CardEmbossScan();										// 카드엠보스스캔
	TranEncCode = fnAPP_EncType(TranCode2);


	return T_OK;
}

int	CTranCmn::fnAPP_FOREGIN_OutputProc()
{

	char	szTempBuff[256] = {0,};
	int		nNextTran = 0;
	if (!m_pDevCmn->TranResult)
		CancelWithSendFlg = TRUE;								// 취소출금전송Flg(초기:실행무)

	if(TranCode2 == TRANID_7230) 
	{
		if (m_pDevCmn->TranResult)									// HOST OK
		{
			fnAPP_PMEAcceptInquiryTranServiceConfirm();
		}

		fnAPP_PSPProc(DEV_JPR);    									// 저널프린트 V01-00-00

		if (!m_pDevCmn->TranResult)
			fnAPP_PMDProc();										// NG 화면메세지표시

		fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_PBM);	
																	// 동작완료검지&장애검지
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// 매체방출	
		fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM);
		return T_OK;
	}

	RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 0);

	fnAPP_PSPProc(DEV_JPR);    								// 저널프린트
	fnAPP_PMDProc();										// 메세지표시

/*********************************************************************** 출금 FlOW 제어*/
	TranProc = TRAN_WITH;    
/***********************************************************************/
	if (m_pDevCmn->TranResult)								
	{		
		RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 1);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[현금계수 수행]"));
		fnAPD_MoneyCount();										// 현금수표출금계수
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[현금계수 완료]"));
	}
	
	fnAPD_CheckDeviceAction(DEV_JPR | DEV_MCU | DEV_BRM | DEV_UCM | DEV_PBM, BRM_IN_CSM_STACK);	// 동작완료검지&장애검지 -> //#N0138 -> #N0158 (DEV_SPR 무시)
	RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 2);	
	fnAPP_PSPProc(DEV_SPR);    									// 명세프린트
	fnAPD_CheckDeviceAction(DEV_SPR); //#N0158 (DEV_SPR 무시)	

																// 동작완료검지&장애검지
	if (m_pDevCmn->TranResult)									// HOST OK
	{
		fnAPP_SumProc(0);										// 출금합계
		
		m_pDevCmn->fnAPL_SetProcCount('6');						// 진행설정
		fnAPD_MaterialOut(DEV_MCU);								// 매체방출
		fnAPD_MaterialOutCheckRetract(DEV_MCU);	
		RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 3);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[셔터오픈  => 시작]"));
		
		fnAPD_MaterialOut(DEV_SPR | DEV_BRM);								// 매체방출
		CancelWithSendFlg = TRUE;								// 취소출금전송Flg(초기:실행무)

		fnAPD_MoneyOutCheck();									// 현금수표방출수취대기
		RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 4);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[셔터닫음  => 완료]"));
		fnAPD_MaterialOutCheckRetract(DEV_SPR);																// 매체방출수취대기회수
	}
	else
	{
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);			// 매체방출	
		fnAPD_MaterialOutCheckRetract(DEV_SPR |DEV_MCU | DEV_PBM);																// 매체방출수취대기회수
	}

	m_pDevCmn->fnAPL_CaptureSaveEnd();				

//	sprintf(szTempBuff, "[현금:%5.5d매 수표:%4.4d매]", m_pDevCmn->fnAPL_GetNumberOfCash(), m_pDevCmn->fnAPL_GetNumberOfOutCheck());
	sprintf(szTempBuff, "[현    금  => %5.5d매]", m_pDevCmn->fnAPL_GetNumberOfCash());
	m_pDevCmn->fnAPL_PrintJnlInfo(szTempBuff, JNL_STATUS);

/*********************************************************************** 출금 FlOW 제어*/
	TranProc = TRAN_FOREGIN;
/***********************************************************************/

	return T_OK;
}

int	CTranCmn::fnAPP_FOREGIN_CheckDeviceActionProc()
{
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_PBM | DEV_CSH | DEV_UCM);
	return T_OK;
}
int	CTranCmn::fnAPP_FOREGIN_CheckMenuContinueProc()
{
	return T_OK;
}



int CTranCmn::fnAPP_HIPASS_InitProc()
{
	memset(Accept.HiPass.AvailLoadAmt, 0x00, sizeof(Accept.HiPass.AvailLoadAmt));
	memset(Accept.HiPass.LoadAmt, 0x00, sizeof(Accept.HiPass.LoadAmt));
	memset(Accept.HiPass.CardNum, 0x00, sizeof(Accept.HiPass.CardNum));
	memset(&m_RXHIPASS, 0x20, sizeof(m_RXHIPASS));
	memset(&m_pDevCmn->RFHipassLoad1,  0x20, sizeof(m_pDevCmn->RFHipassLoad1));

	memset(Accept.ETicketConfirmNum, 0x20, sizeof(Accept.ETicketConfirmNum));

	m_pDevCmn->HiPassLLog.PPCardTranType = 0x01;
	
	return T_OK;
}

int CTranCmn::fnAPP_HIPASS_InputProc()
{
	CString strReturn("");
	int		ScrNum = 0;

	if(TranCode == TC_HIPASS_BALANCE)
	{
		fnAPP_HPSReadBalance();									// Hi-Pass 잔액조회
		fnAPP_HPSBalanceConfirm();								// Hi-Pass 잔액조회확인

		fnAPD_MaterialOut(DEV_MCU);							
		fnAPD_MaterialOutCheckRetract(DEV_MCU);				

		fnAPP_CancelProc(T_OK);									// 정상종료
	}
	else if(TranCode == TC_HIPASS_CASH)
	{
		ScrNum = 2923;
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

		fnAPP_HPSReadBalance();									// Hi-Pass 잔액조회	
		CancelHipassPrintFlg = FALSE;
		fnAPP_AcceptTelNum();		
		fnAPP_InitializeSerialTranDS();							// 연속거래변수초기화
		Accept.MoneyInKind = TRAN_DEP_CASH;

		fnAPD_CheckDeposit();										// 입금수표
		fnAPD_CashDeposit();										// 입금현금

		AddString(Accept.CheckMoney, Accept.CashMoney, 15, Accept.Money);	
		memcpy(Accept.HiPass.LoadAmt, &Accept.Money[7], 8);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HPSAcceptMoney():Accept.HiPass.LoadAmt[%8.8s] : %s", Accept.HiPass.LoadAmt, strReturn);

		if (Asc2Int(Accept.Money, 15) > Asc2Int(Accept.HiPass.AvailLoadAmt, 8))
		{
			if (Asc2Int(Accept.Money, 15) > Asc2Int(Accept.HiPass.AvailLoadAmt, 8))	// 충전한도 초과
				fnAPP_CancelDepRejectProc(T_MSG, "충전한도가 초과되어 거래가 취소되었습니다. ", 
												"반환된 금액을 확인하시고, 다시거래하여주시기 바랍니다.");
																			// 취소입금반환처리
		}

		if(Asc2Int(Accept.Money, 15) < 10000)
		{
			fnAPP_CancelDepRejectProc(T_MSG, "충전 최소금액은 1만원 이상 입니다.", 
											"반환된 금액을 확인하시고, 다시거래하여주시기 바랍니다.");
																			// 취소입금반환처리
		}

		

		AddSerialFlg = FALSE;
		TranCode2Save = TRANID_4717;
		TranCode2 = TRANID_4717;
		fnAPP_SendHost();										// 송신
		fnAPP_RecvHost();										// 수신
		if (m_pDevCmn->TranResult)
		{
			fnAPP_HPSAcceptMoneyInConfirm();

			CancelHIPassLoadFlg = TRUE;							// 거래실패시 Complete 처리Flag #0439
			if (fnAPP_HPSLoadS1() == T_ERROR)					// Hi-Pass 충전S1
			{
				m_pDevCmn->fnSCR_DisplayString(1, "하이패스 미충전 입니다");
				m_pDevCmn->fnSCR_DisplayString(2, "반환금액과 카드를 받으십시요");
				m_pDevCmn->fnSCR_DisplayString(3, "카드는 영업소에서 확인 하시기 바랍니다");
				m_pDevCmn->fnSCR_DisplayString(5, "불편을 드려 죄송합니다");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				Delay_Msg(3000);
				fnAPP_CancelDepRejectProc(T_MSG);
			}

			AddSerialFlg = FALSE;
			TranCode2Save = TRANID_4670;
			TranCode2 = TRANID_4670;
		}
		else
		{
			fnAPP_CancelDepRejectProc(T_MSG, "도로공사 회선 장애로", "하이패스 충전 거래를 이용하실 수 없습니다", "불편을 드려 죄송합니다");			
		}
	}
	else if(TranCode == TC_HIPASS_CREDIT)
	{
		ScrNum = 2920;
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

		fnAPP_InitializeSerialTranDS();							// 연속거래변수초기화
		fnAPP_HPSReadBalance();									// Hi-Pass 잔액조회
		fnAPP_HPSAcceptMoney();
		//fnAPP_AcceptTelNum();	
		ExtraServiceFlag = TC_HIPASS_CREDIT;
		fnAPP_AcceptCmnLibJuminNum();							// HI-PASS 충전서비스 결제정보(생년월일) 추가 #0443
		fnAPP_HIPASS_AcceptPassword();									// HI-PASS 충전서비스 결제정보(비밀번호) 추가 #0443


		CancelHipassPrintFlg = FALSE;

		m_pDevCmn->fnAPL_SetProcCount('3');						// 진행설정			
		AddSerialFlg = FALSE;
		TranCode2Save = TRANID_4840;
		TranCode2 = TRANID_4840;
		fnAPP_SendHost();										// 송신
		fnAPP_RecvHost();										// 수신

		if (m_pDevCmn->TranResult)
		{
			CancelHIPassLoadFlg = TRUE;							// 거래실패시 Complete 처리Flag #0439
			if (fnAPP_HPSLoadS1() == T_ERROR)					// Hi-Pass 충전S1
			{
				m_pDevCmn->fnSCR_DisplayString(1, "하이패스 미충전 입니다");
				m_pDevCmn->fnSCR_DisplayString(2, "카드를 받으십시요");
				m_pDevCmn->fnSCR_DisplayString(3, "카드는 영업소에서 확인 하시기 바랍니다");
				m_pDevCmn->fnSCR_DisplayString(5, "불편을 드려 죄송합니다");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				Delay_Msg(3000);

				fnAPP_SendHostCancel();
				fnAPP_RecvHost();									// 수신
				if (m_pDevCmn->TranResult)							// HOST OK
					fnAPP_CancelProc(T_MSG, "하이패스 신용카드 결제 부분은 취소 되었습니다", "결제 카드사에 잔액을 확인 하시기 바랍니다");
				else
					fnAPP_CancelProc(T_MSG, "하이패스 신용카드 결제취소가 실패하였습니다.", "하이패스 영업소에 문의하시기 바랍니다.");
			}

			AddSerialFlg = FALSE;
			TranCode2Save = TRANID_4670;
			TranCode2 = TRANID_4670;
		}
		else
			fnAPP_CancelProc(T_MSG, "도로공사 회선 장애로 ", "하이패스 충전 거래를 이용하실 수 없습니다","불편을 드려 죄송합니다" );	
	}
		
	return T_OK;
}

int CTranCmn::fnAPP_HIPASS_OutputProc()
{
	if (TranCode == TC_HIPASS_CASH)
	{
		if (m_pDevCmn->TranResult)
		{
			if (fnAPP_HPSLoadS2() == T_OK)								// Hi-Pass 충전S2
			{
				CancelHIPassLoadFlg = FALSE;							// 거래실패시 Complete 처리Flag #0439
				//fnAPP_SumProc(2);										// 입금(휴일)
				fnAPD_CheckDepositStore();								// 입금수표수납
				if(m_pProfile->DEVICE.MachineType == U8100 || m_pProfile->DEVICE.MachineType == U3100K)
					fnAPD_CashDepositStore(TRUE);						// 입금현금수납
				else
					fnAPD_CashDepositStore();							// 입금현금수납

				fnAPP_HIPASS_SendComplet();                           // Hi-Pass 충전완료
				fnAPP_HPSLoadConfirm();

				fnAPD_MaterialOut(DEV_MCU);								// Card Out	#0336
				fnAPD_MaterialOutCheckRetract(DEV_MCU);					// Card Out Check & Retract #0336
			}
			else
			{
				m_pDevCmn->fnSCR_DisplayString(1, "하이패스 미충전 입니다");
				m_pDevCmn->fnSCR_DisplayString(2, "반환금액과 카드를 받으십시요");
				m_pDevCmn->fnSCR_DisplayString(3, "카드는 영업소에서 확인 하시기 바랍니다");
				m_pDevCmn->fnSCR_DisplayString(5, "불편을 드려 죄송합니다");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				Delay_Msg(3000);

				m_pDevCmn->fnHPS_ReadBalance();
				fnAPD_CheckDeviceAction(DEV_MCU);
				if (!m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_READBALANCE)) // 충전실패시 잔액 조회
				{
					fnAPD_MaterialOut(DEV_MCU);							// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract

					if (!memcmp(m_pDevCmn->RFHipassLoad1.CardBalance, m_pDevCmn->RFHipassBalance.Balance, sizeof(m_pDevCmn->RFHipassBalance.Balance)))
					{
						AddSerialFlg = FALSE;
						TranCode2Save = TRANID_4680;
						TranCode2 = TRANID_4680;
						fnAPP_SendHost();										// 송신
						fnAPP_RecvHost();										// 수신

						if (m_pDevCmn->TranResult)
							// 충전 실패시 잔액확인/충전취소 정상응답 받을 경우만 현금반환
							fnAPP_CancelDepRejectProc(T_MSG, "하이패스 충전거래가 취소 되었습니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");
						else
							fnAPP_CancelProc(T_MSG, "하이패스 충전 거래가 실패하였습니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");
					}

					fnAPP_CancelProc(T_MSG, "하이패스 잔액이 변경되었습니다.", "하이패스 카드 잔액을 확인 하시기 바랍니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");
				}
				else
				{
					fnAPD_MaterialOut(DEV_MCU);										// Card Out	#0336
					fnAPD_MaterialOutCheckRetract(DEV_MCU);							// Card Out Check & Retract	#0336

					fnAPP_CancelProc(T_MSG, "하이패스 잔액을 확인할 수 없습니다.", "카드는 영업소에서 확인 하시기 바랍니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");
				}
			}
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayString(1, "하이패스 미충전 입니다.");
			m_pDevCmn->fnSCR_DisplayString(2, "반환금액과 카드를 받으십시요.");
			m_pDevCmn->fnSCR_DisplayString(3, "카드는 영업소에서 확인 하시기 바랍니다.");
			m_pDevCmn->fnSCR_DisplayString(5, "불편을 드려 죄송합니다.");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(3000);

			fnAPP_CancelDepRejectProc(T_MSG);
		}
	}
	else
	if (TranCode == TC_HIPASS_CREDIT)
	{
		if (m_pDevCmn->TranResult)
		{
			if (fnAPP_HPSLoadS2() == T_OK)					// Hi-Pass 충전S2
			{
				m_pDevCmn->fnHPS_ReadBalance();
				fnAPD_CheckDeviceAction(DEV_MCU);
				if (!m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_READBALANCE)) // 충전실패시 잔액 조회
				{
					if (!memcmp(m_pDevCmn->RFHipassLoad1.CardBalance, m_pDevCmn->RFHipassBalance.Balance, sizeof(m_pDevCmn->RFHipassBalance.Balance)))
					{
						fnAPP_HIPASS_SendErrorComplet();
						AddSerialFlg = FALSE;
						TranCode2Save = TRANID_4840;
						TranCode2 = TRANID_4840;
						fnAPP_SendHostCancel();	// SEND TRANDID_4212
						fnAPP_RecvHost();									// 수신
						if (m_pDevCmn->TranResult)							// HOST OK
							fnAPP_CancelProc(T_MSG, "하이패스 충전이 취소 되었습니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");
						else
							fnAPP_CancelProc(T_MSG, "하이패스 충전취소가 실패하였습니다.", "하이패스 영업소에 문의하시기 바랍니다.");

					}
				}
				
				CancelHIPassLoadFlg = FALSE;							// 거래실패시 Complete 처리Flag #0439
//				fnAPP_SumProc(2);										// 입금(휴일)
				fnAPP_HIPASS_SendComplet();                           // Hi-Pass 충전완료
				fnAPP_HPSLoadConfirm();

//				fnAPD_MaterialOut(DEV_MCU);								// Card Out	#0336
//				fnAPD_MaterialOutCheckRetract(DEV_MCU);					// Card Out Check & Retract	#0336
			}
			else
			{
				m_pDevCmn->fnSCR_DisplayString(1, "하이패스 미충전 입니다.");
				m_pDevCmn->fnSCR_DisplayString(2, "신용 취소 후 카드를 받으십시요.");
				m_pDevCmn->fnSCR_DisplayString(3, "카드는 영업소에서 확인 하시기 바랍니다.");
				m_pDevCmn->fnSCR_DisplayString(5, "불편을 드려 죄송합니다.");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				Delay_Msg(3000);

				m_pDevCmn->fnHPS_ReadBalance();
				fnAPD_CheckDeviceAction(DEV_MCU);
				if (!m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_READBALANCE)) // 충전실패시 잔액 조회
				{
					if (!memcmp(m_pDevCmn->RFHipassLoad1.CardBalance, m_pDevCmn->RFHipassBalance.Balance, sizeof(m_pDevCmn->RFHipassBalance.Balance)))
					{
						AddSerialFlg = FALSE;
						fnAPP_HIPASS_SendErrorComplet();										// 수신

						if (m_pDevCmn->TranResult)
						{
							AddSerialFlg = FALSE;
							TranCode2Save = TRANID_4840;
							TranCode2 = TRANID_4840;
							fnAPP_SendHostCancel();	// SEND TRANDID_4212
							fnAPP_RecvHost();									// 수신
							if (m_pDevCmn->TranResult)							// HOST OK
								fnAPP_CancelProc(T_MSG, "하이패스 충전이 취소 되었습니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");
							else
								fnAPP_CancelProc(T_MSG, "하이패스 충전취소가 실패하였습니다.", "하이패스 영업소에 문의하시기 바랍니다.");
						}
						else
							fnAPP_CancelProc(T_MSG, "하이패스 충전이 취소 되었습니다.", "신용카드 결제 부분은 영업소에서 확인 하시기 바랍니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");
					}
					else
						fnAPP_CancelProc(T_MSG, "하이패스 잔액이 변경되었습니다.", "하이패스 카드 잔액을 확인 하시기 바랍니다.");
				}

				fnAPP_CancelProc(T_MSG, "하이패스 잔액을 확인할 수 없습니다.", "카드는 영업소에서 확인 하시기 바랍니다.");
			}
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayString(1, "하이패스 미충전 입니다.");
			m_pDevCmn->fnSCR_DisplayString(2, "신용 취소 후 카드를 받으십시요.");
			m_pDevCmn->fnSCR_DisplayString(3, "카드는 영업소에서 확인 하시기 바랍니다.");
			m_pDevCmn->fnSCR_DisplayString(5, "불편을 드려 죄송합니다.");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(3000);

			AddSerialFlg = FALSE;
			TranCode2Save = TRANID_4840;
			TranCode2 = TRANID_4840;
			fnAPP_SendHostCancel();	// SEND TRANDID_4212
			fnAPP_RecvHost();									// 수신
			if (m_pDevCmn->TranResult)							// HOST OK
				fnAPP_CancelProc(T_MSG, "하이패스 신용카드 결제 부분은 취소 되었습니다", "결제 카드사에 잔액을 확인 하시기 바랍니다");
			else
				fnAPP_CancelProc(T_MSG, "하이패스 신용카드 결제취소가 실패하였습니다.", "하이패스 영업소에 문의하시기 바랍니다.");
		}

	}
	
	CancelHipassPrintFlg = TRUE;
	Accept.SlipChoiceFlag = TRUE;

	fnAPP_PSPProc(DEV_JPR);    									// 저널프린트
	fnAPP_PSPProc(DEV_SPR);    									// 명세프린트
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_PBM);	

	return T_OK;
}

int CTranCmn::fnAPP_HIPASS_CheckDeviceActionProc()
{
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// 매체방출	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// 매체방출수취대기회수

	return T_OK;
}

int CTranCmn::fnAPP_HIPASS_CheckMenuContinueProc()
{
	return T_OK;
}


int CTranCmn::fnAPP_HIPASS_SendErrorComplet()
{
	AddSerialFlg = FALSE;
	TranCode2Save = TRANID_4680;
	TranCode2 = TRANID_4680;
	fnAPP_SendHost();										// 송신
	fnAPP_RecvHost();

	if (m_pDevCmn->TranResult)
	{
		fnAPD_HPSComplete();
	}
	
	//m_pDevCmn->fnHPS_ReadLLog();
	//fnAPD_CheckDeviceAction(DEV_MCU);
	//m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSLLOG);

	return T_OK;
}


int CTranCmn::fnAPP_HIPASS_SendComplet()
{
	AddSerialFlg = FALSE;
	TranCode2Save = TRANID_4671;
	TranCode2 = TRANID_4671;
	fnAPP_SendHost();										// 송신
	fnAPP_RecvHost();

	if (m_pDevCmn->TranResult)
	{
		fnAPD_HPSComplete();
	}

	return T_OK;
}

