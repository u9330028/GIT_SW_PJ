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
	case TRAN_UTILYBILLS:										// ���ΰ�����
		fnAPP_UTILYBILLS_InitProc();
		break;
	case TRAN_LOCALTAX:											// ���漼
		fnAPP_LOCALTAX_InitProc();
		break;
	case TRAN_FOREGIN:											// // �ؿ�ī�� #N0171
	case TRAN_FOREGIN_INQ:										// // �ؿ�ī�� ��ȸ #N0171
		fnAPP_FOREGIN_InitProc();
		break;
	case TRAN_HIPASS:											// �����н� ���� #N0266
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
	case TRAN_UTILYBILLS:										// ���ΰ�����
		fnAPP_UTILYBILLS_InputProc();
		break;
	case TRAN_LOCALTAX:										// ���ΰ�����
		fnAPP_LOCALTAX_InputProc();
		break;
	case TRAN_FOREGIN:											// // �ؿ�ī�� #N0171
	case TRAN_FOREGIN_INQ:										// // �ؿ�ī�� ��ȸ #N0171
		fnAPP_FOREGIN_InputProc();
		break;
	case TRAN_HIPASS:											// �����н� ���� #N0266
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
	case TRAN_UTILYBILLS:										// ���ΰ�����
		fnAPP_UTILYBILLS_OutputProc();
		break;
	case TRAN_LOCALTAX:										// ���ΰ�����
		fnAPP_LOCALTAX_OutputProc();
		break;
	case TRAN_FOREGIN:											// // �ؿ�ī�� #N0171
	case TRAN_FOREGIN_INQ:										// // �ؿ�ī�� ��ȸ #N0171
		fnAPP_FOREGIN_OutputProc();
		break;
	case TRAN_HIPASS:											// �����н� ���� #N0266
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
	case TRAN_UTILYBILLS:										// ���ΰ�����
		fnAPP_UTILYBILLS_CheckDeviceActionProc();
		break;
	case TRAN_LOCALTAX:										// ���ΰ�����
		fnAPP_LOCALTAX_CheckDeviceActionProc();
		break;
	case TRAN_FOREGIN:											// // �ؿ�ī�� #N0171
	case TRAN_FOREGIN_INQ:										// // �ؿ�ī�� ��ȸ #N0171
		fnAPP_FOREGIN_CheckDeviceActionProc();
		break;
	case TRAN_HIPASS:											// �����н� ���� #N0266
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
	case TRAN_UTILYBILLS:										// ���ΰ�����
		fnAPP_UTILYBILLS_CheckMenuContinueProc();
		break;
	case TRAN_LOCALTAX:										// ���ΰ�����
		fnAPP_LOCALTAX_CheckMenuContinueProc();
		break;
	case TRAN_FOREGIN:											// �ؿ�ī�� #N0171
	case TRAN_FOREGIN_INQ:										// // �ؿ�ī�� ��ȸ #N0171
		fnAPP_FOREGIN_CheckMenuContinueProc();
		break;
	case TRAN_HIPASS:											// �����н� ����	#N0266
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
/* ���ΰ����� Proc : 2010.10.14   Kim.Gi.Jin
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
	//#GIRO ���δ�� ���ǰ��� '0' 7�ڸ�
	memset(Accept.JiroNum, '0', sizeof(Accept.JiroNum));		// ���ι�ȣ
	if(Accept.JiroTranCode == TC_JRJIRO)							// ���δ�ݳ���
		fnAPP_UTILYBILLS_Sub02_InputProc();							// ���ι�ȣ�Է�
	
	fnAPP_UTILYBILLS_Sub03_InputProc();								// �������ڳ��ι�ȣ�Է�
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
		//1ȸ Send, Recv = SR1 Next Step
		//fnAPP_UTILYBILLS_SR1_Sub01_OutputProc();
		//Accept.SlipChoiceFlag = TRUE;  #N0197
		if(!Accept.SlipChoiceFlag)
		{
			m_pDevCmn->fnSCR_DisplayString(3, "�̿��� �ּż� �����մϴ�");
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

	//���ΰ����ȸ�� ���ڳ��ι�ȣ�Է�
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

	
	if((TranProc == TRAN_FOREGIN_INQ) && (Accept.MuConfirmNum[0] == 'E')) // �ݰ�� �ؿ�ī�� ��ȸ�� ������ �ΰ���
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
			fnAPP_AcceptWith();										// ����Է�
		else
		if(TranProc == TRAN_FOREGIN_INQ)
			fnAPP_AcceptInq();										// ����Է�
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
	
	//���
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


	fnAPD_CardEmbossScan();										// ī�忥������ĵ
	TranEncCode = fnAPP_EncType(TranCode2);


	return T_OK;
}

int	CTranCmn::fnAPP_FOREGIN_OutputProc()
{

	char	szTempBuff[256] = {0,};
	int		nNextTran = 0;
	if (!m_pDevCmn->TranResult)
		CancelWithSendFlg = TRUE;								// ����������Flg(�ʱ�:���๫)

	if(TranCode2 == TRANID_7230) 
	{
		if (m_pDevCmn->TranResult)									// HOST OK
		{
			fnAPP_PMEAcceptInquiryTranServiceConfirm();
		}

		fnAPP_PSPProc(DEV_JPR);    									// ��������Ʈ V01-00-00

		if (!m_pDevCmn->TranResult)
			fnAPP_PMDProc();										// NG ȭ��޼���ǥ��

		fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_PBM);	
																	// ���ۿϷ����&��ְ���
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// ��ü����	
		fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM);
		return T_OK;
	}

	RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 0);

	fnAPP_PSPProc(DEV_JPR);    								// ��������Ʈ
	fnAPP_PMDProc();										// �޼���ǥ��

/*********************************************************************** ��� FlOW ����*/
	TranProc = TRAN_WITH;    
/***********************************************************************/
	if (m_pDevCmn->TranResult)								
	{		
		RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 1);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���ݰ�� ����]"));
		fnAPD_MoneyCount();										// ���ݼ�ǥ��ݰ��
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���ݰ�� �Ϸ�]"));
	}
	
	fnAPD_CheckDeviceAction(DEV_JPR | DEV_MCU | DEV_BRM | DEV_UCM | DEV_PBM, BRM_IN_CSM_STACK);	// ���ۿϷ����&��ְ��� -> //#N0138 -> #N0158 (DEV_SPR ����)
	RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 2);	
	fnAPP_PSPProc(DEV_SPR);    									// ������Ʈ
	fnAPD_CheckDeviceAction(DEV_SPR); //#N0158 (DEV_SPR ����)	

																// ���ۿϷ����&��ְ���
	if (m_pDevCmn->TranResult)									// HOST OK
	{
		fnAPP_SumProc(0);										// ����հ�
		
		m_pDevCmn->fnAPL_SetProcCount('6');						// ���༳��
		fnAPD_MaterialOut(DEV_MCU);								// ��ü����
		fnAPD_MaterialOutCheckRetract(DEV_MCU);	
		RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 3);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���Ϳ���  => ����]"));
		
		fnAPD_MaterialOut(DEV_SPR | DEV_BRM);								// ��ü����
		CancelWithSendFlg = TRUE;								// ����������Flg(�ʱ�:���๫)

		fnAPD_MoneyOutCheck();									// ���ݼ�ǥ���������
		RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 4);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���ʹ���  => �Ϸ�]"));
		fnAPD_MaterialOutCheckRetract(DEV_SPR);																// ��ü���������ȸ��
	}
	else
	{
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);			// ��ü����	
		fnAPD_MaterialOutCheckRetract(DEV_SPR |DEV_MCU | DEV_PBM);																// ��ü���������ȸ��
	}

	m_pDevCmn->fnAPL_CaptureSaveEnd();				

//	sprintf(szTempBuff, "[����:%5.5d�� ��ǥ:%4.4d��]", m_pDevCmn->fnAPL_GetNumberOfCash(), m_pDevCmn->fnAPL_GetNumberOfOutCheck());
	sprintf(szTempBuff, "[��    ��  => %5.5d��]", m_pDevCmn->fnAPL_GetNumberOfCash());
	m_pDevCmn->fnAPL_PrintJnlInfo(szTempBuff, JNL_STATUS);

/*********************************************************************** ��� FlOW ����*/
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
		fnAPP_HPSReadBalance();									// Hi-Pass �ܾ���ȸ
		fnAPP_HPSBalanceConfirm();								// Hi-Pass �ܾ���ȸȮ��

		fnAPD_MaterialOut(DEV_MCU);							
		fnAPD_MaterialOutCheckRetract(DEV_MCU);				

		fnAPP_CancelProc(T_OK);									// ��������
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

		fnAPP_HPSReadBalance();									// Hi-Pass �ܾ���ȸ	
		CancelHipassPrintFlg = FALSE;
		fnAPP_AcceptTelNum();		
		fnAPP_InitializeSerialTranDS();							// ���Ӱŷ������ʱ�ȭ
		Accept.MoneyInKind = TRAN_DEP_CASH;

		fnAPD_CheckDeposit();										// �Աݼ�ǥ
		fnAPD_CashDeposit();										// �Ա�����

		AddString(Accept.CheckMoney, Accept.CashMoney, 15, Accept.Money);	
		memcpy(Accept.HiPass.LoadAmt, &Accept.Money[7], 8);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HPSAcceptMoney():Accept.HiPass.LoadAmt[%8.8s] : %s", Accept.HiPass.LoadAmt, strReturn);

		if (Asc2Int(Accept.Money, 15) > Asc2Int(Accept.HiPass.AvailLoadAmt, 8))
		{
			if (Asc2Int(Accept.Money, 15) > Asc2Int(Accept.HiPass.AvailLoadAmt, 8))	// �����ѵ� �ʰ�
				fnAPP_CancelDepRejectProc(T_MSG, "�����ѵ��� �ʰ��Ǿ� �ŷ��� ��ҵǾ����ϴ�. ", 
												"��ȯ�� �ݾ��� Ȯ���Ͻð�, �ٽðŷ��Ͽ��ֽñ� �ٶ��ϴ�.");
																			// ����Աݹ�ȯó��
		}

		if(Asc2Int(Accept.Money, 15) < 10000)
		{
			fnAPP_CancelDepRejectProc(T_MSG, "���� �ּұݾ��� 1���� �̻� �Դϴ�.", 
											"��ȯ�� �ݾ��� Ȯ���Ͻð�, �ٽðŷ��Ͽ��ֽñ� �ٶ��ϴ�.");
																			// ����Աݹ�ȯó��
		}

		

		AddSerialFlg = FALSE;
		TranCode2Save = TRANID_4717;
		TranCode2 = TRANID_4717;
		fnAPP_SendHost();										// �۽�
		fnAPP_RecvHost();										// ����
		if (m_pDevCmn->TranResult)
		{
			fnAPP_HPSAcceptMoneyInConfirm();

			CancelHIPassLoadFlg = TRUE;							// �ŷ����н� Complete ó��Flag #0439
			if (fnAPP_HPSLoadS1() == T_ERROR)					// Hi-Pass ����S1
			{
				m_pDevCmn->fnSCR_DisplayString(1, "�����н� ������ �Դϴ�");
				m_pDevCmn->fnSCR_DisplayString(2, "��ȯ�ݾװ� ī�带 �����ʽÿ�");
				m_pDevCmn->fnSCR_DisplayString(3, "ī��� �����ҿ��� Ȯ�� �Ͻñ� �ٶ��ϴ�");
				m_pDevCmn->fnSCR_DisplayString(5, "������ ��� �˼��մϴ�");
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
			fnAPP_CancelDepRejectProc(T_MSG, "���ΰ��� ȸ�� ��ַ�", "�����н� ���� �ŷ��� �̿��Ͻ� �� �����ϴ�", "������ ��� �˼��մϴ�");			
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

		fnAPP_InitializeSerialTranDS();							// ���Ӱŷ������ʱ�ȭ
		fnAPP_HPSReadBalance();									// Hi-Pass �ܾ���ȸ
		fnAPP_HPSAcceptMoney();
		//fnAPP_AcceptTelNum();	
		ExtraServiceFlag = TC_HIPASS_CREDIT;
		fnAPP_AcceptCmnLibJuminNum();							// HI-PASS �������� ��������(�������) �߰� #0443
		fnAPP_HIPASS_AcceptPassword();									// HI-PASS �������� ��������(��й�ȣ) �߰� #0443


		CancelHipassPrintFlg = FALSE;

		m_pDevCmn->fnAPL_SetProcCount('3');						// ���༳��			
		AddSerialFlg = FALSE;
		TranCode2Save = TRANID_4840;
		TranCode2 = TRANID_4840;
		fnAPP_SendHost();										// �۽�
		fnAPP_RecvHost();										// ����

		if (m_pDevCmn->TranResult)
		{
			CancelHIPassLoadFlg = TRUE;							// �ŷ����н� Complete ó��Flag #0439
			if (fnAPP_HPSLoadS1() == T_ERROR)					// Hi-Pass ����S1
			{
				m_pDevCmn->fnSCR_DisplayString(1, "�����н� ������ �Դϴ�");
				m_pDevCmn->fnSCR_DisplayString(2, "ī�带 �����ʽÿ�");
				m_pDevCmn->fnSCR_DisplayString(3, "ī��� �����ҿ��� Ȯ�� �Ͻñ� �ٶ��ϴ�");
				m_pDevCmn->fnSCR_DisplayString(5, "������ ��� �˼��մϴ�");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				Delay_Msg(3000);

				fnAPP_SendHostCancel();
				fnAPP_RecvHost();									// ����
				if (m_pDevCmn->TranResult)							// HOST OK
					fnAPP_CancelProc(T_MSG, "�����н� �ſ�ī�� ���� �κ��� ��� �Ǿ����ϴ�", "���� ī��翡 �ܾ��� Ȯ�� �Ͻñ� �ٶ��ϴ�");
				else
					fnAPP_CancelProc(T_MSG, "�����н� �ſ�ī�� ������Ұ� �����Ͽ����ϴ�.", "�����н� �����ҿ� �����Ͻñ� �ٶ��ϴ�.");
			}

			AddSerialFlg = FALSE;
			TranCode2Save = TRANID_4670;
			TranCode2 = TRANID_4670;
		}
		else
			fnAPP_CancelProc(T_MSG, "���ΰ��� ȸ�� ��ַ� ", "�����н� ���� �ŷ��� �̿��Ͻ� �� �����ϴ�","������ ��� �˼��մϴ�" );	
	}
		
	return T_OK;
}

int CTranCmn::fnAPP_HIPASS_OutputProc()
{
	if (TranCode == TC_HIPASS_CASH)
	{
		if (m_pDevCmn->TranResult)
		{
			if (fnAPP_HPSLoadS2() == T_OK)								// Hi-Pass ����S2
			{
				CancelHIPassLoadFlg = FALSE;							// �ŷ����н� Complete ó��Flag #0439
				//fnAPP_SumProc(2);										// �Ա�(����)
				fnAPD_CheckDepositStore();								// �Աݼ�ǥ����
				if(m_pProfile->DEVICE.MachineType == U8100 || m_pProfile->DEVICE.MachineType == U3100K)
					fnAPD_CashDepositStore(TRUE);						// �Ա����ݼ���
				else
					fnAPD_CashDepositStore();							// �Ա����ݼ���

				fnAPP_HIPASS_SendComplet();                           // Hi-Pass �����Ϸ�
				fnAPP_HPSLoadConfirm();

				fnAPD_MaterialOut(DEV_MCU);								// Card Out	#0336
				fnAPD_MaterialOutCheckRetract(DEV_MCU);					// Card Out Check & Retract #0336
			}
			else
			{
				m_pDevCmn->fnSCR_DisplayString(1, "�����н� ������ �Դϴ�");
				m_pDevCmn->fnSCR_DisplayString(2, "��ȯ�ݾװ� ī�带 �����ʽÿ�");
				m_pDevCmn->fnSCR_DisplayString(3, "ī��� �����ҿ��� Ȯ�� �Ͻñ� �ٶ��ϴ�");
				m_pDevCmn->fnSCR_DisplayString(5, "������ ��� �˼��մϴ�");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				Delay_Msg(3000);

				m_pDevCmn->fnHPS_ReadBalance();
				fnAPD_CheckDeviceAction(DEV_MCU);
				if (!m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_READBALANCE)) // �������н� �ܾ� ��ȸ
				{
					fnAPD_MaterialOut(DEV_MCU);							// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract

					if (!memcmp(m_pDevCmn->RFHipassLoad1.CardBalance, m_pDevCmn->RFHipassBalance.Balance, sizeof(m_pDevCmn->RFHipassBalance.Balance)))
					{
						AddSerialFlg = FALSE;
						TranCode2Save = TRANID_4680;
						TranCode2 = TRANID_4680;
						fnAPP_SendHost();										// �۽�
						fnAPP_RecvHost();										// ����

						if (m_pDevCmn->TranResult)
							// ���� ���н� �ܾ�Ȯ��/������� �������� ���� ��츸 ���ݹ�ȯ
							fnAPP_CancelDepRejectProc(T_MSG, "�����н� �����ŷ��� ��� �Ǿ����ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
						else
							fnAPP_CancelProc(T_MSG, "�����н� ���� �ŷ��� �����Ͽ����ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
					}

					fnAPP_CancelProc(T_MSG, "�����н� �ܾ��� ����Ǿ����ϴ�.", "�����н� ī�� �ܾ��� Ȯ�� �Ͻñ� �ٶ��ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
				}
				else
				{
					fnAPD_MaterialOut(DEV_MCU);										// Card Out	#0336
					fnAPD_MaterialOutCheckRetract(DEV_MCU);							// Card Out Check & Retract	#0336

					fnAPP_CancelProc(T_MSG, "�����н� �ܾ��� Ȯ���� �� �����ϴ�.", "ī��� �����ҿ��� Ȯ�� �Ͻñ� �ٶ��ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
				}
			}
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayString(1, "�����н� ������ �Դϴ�.");
			m_pDevCmn->fnSCR_DisplayString(2, "��ȯ�ݾװ� ī�带 �����ʽÿ�.");
			m_pDevCmn->fnSCR_DisplayString(3, "ī��� �����ҿ��� Ȯ�� �Ͻñ� �ٶ��ϴ�.");
			m_pDevCmn->fnSCR_DisplayString(5, "������ ��� �˼��մϴ�.");
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
			if (fnAPP_HPSLoadS2() == T_OK)					// Hi-Pass ����S2
			{
				m_pDevCmn->fnHPS_ReadBalance();
				fnAPD_CheckDeviceAction(DEV_MCU);
				if (!m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_READBALANCE)) // �������н� �ܾ� ��ȸ
				{
					if (!memcmp(m_pDevCmn->RFHipassLoad1.CardBalance, m_pDevCmn->RFHipassBalance.Balance, sizeof(m_pDevCmn->RFHipassBalance.Balance)))
					{
						fnAPP_HIPASS_SendErrorComplet();
						AddSerialFlg = FALSE;
						TranCode2Save = TRANID_4840;
						TranCode2 = TRANID_4840;
						fnAPP_SendHostCancel();	// SEND TRANDID_4212
						fnAPP_RecvHost();									// ����
						if (m_pDevCmn->TranResult)							// HOST OK
							fnAPP_CancelProc(T_MSG, "�����н� ������ ��� �Ǿ����ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
						else
							fnAPP_CancelProc(T_MSG, "�����н� ������Ұ� �����Ͽ����ϴ�.", "�����н� �����ҿ� �����Ͻñ� �ٶ��ϴ�.");

					}
				}
				
				CancelHIPassLoadFlg = FALSE;							// �ŷ����н� Complete ó��Flag #0439
//				fnAPP_SumProc(2);										// �Ա�(����)
				fnAPP_HIPASS_SendComplet();                           // Hi-Pass �����Ϸ�
				fnAPP_HPSLoadConfirm();

//				fnAPD_MaterialOut(DEV_MCU);								// Card Out	#0336
//				fnAPD_MaterialOutCheckRetract(DEV_MCU);					// Card Out Check & Retract	#0336
			}
			else
			{
				m_pDevCmn->fnSCR_DisplayString(1, "�����н� ������ �Դϴ�.");
				m_pDevCmn->fnSCR_DisplayString(2, "�ſ� ��� �� ī�带 �����ʽÿ�.");
				m_pDevCmn->fnSCR_DisplayString(3, "ī��� �����ҿ��� Ȯ�� �Ͻñ� �ٶ��ϴ�.");
				m_pDevCmn->fnSCR_DisplayString(5, "������ ��� �˼��մϴ�.");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				Delay_Msg(3000);

				m_pDevCmn->fnHPS_ReadBalance();
				fnAPD_CheckDeviceAction(DEV_MCU);
				if (!m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_READBALANCE)) // �������н� �ܾ� ��ȸ
				{
					if (!memcmp(m_pDevCmn->RFHipassLoad1.CardBalance, m_pDevCmn->RFHipassBalance.Balance, sizeof(m_pDevCmn->RFHipassBalance.Balance)))
					{
						AddSerialFlg = FALSE;
						fnAPP_HIPASS_SendErrorComplet();										// ����

						if (m_pDevCmn->TranResult)
						{
							AddSerialFlg = FALSE;
							TranCode2Save = TRANID_4840;
							TranCode2 = TRANID_4840;
							fnAPP_SendHostCancel();	// SEND TRANDID_4212
							fnAPP_RecvHost();									// ����
							if (m_pDevCmn->TranResult)							// HOST OK
								fnAPP_CancelProc(T_MSG, "�����н� ������ ��� �Ǿ����ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
							else
								fnAPP_CancelProc(T_MSG, "�����н� ������Ұ� �����Ͽ����ϴ�.", "�����н� �����ҿ� �����Ͻñ� �ٶ��ϴ�.");
						}
						else
							fnAPP_CancelProc(T_MSG, "�����н� ������ ��� �Ǿ����ϴ�.", "�ſ�ī�� ���� �κ��� �����ҿ��� Ȯ�� �Ͻñ� �ٶ��ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
					}
					else
						fnAPP_CancelProc(T_MSG, "�����н� �ܾ��� ����Ǿ����ϴ�.", "�����н� ī�� �ܾ��� Ȯ�� �Ͻñ� �ٶ��ϴ�.");
				}

				fnAPP_CancelProc(T_MSG, "�����н� �ܾ��� Ȯ���� �� �����ϴ�.", "ī��� �����ҿ��� Ȯ�� �Ͻñ� �ٶ��ϴ�.");
			}
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayString(1, "�����н� ������ �Դϴ�.");
			m_pDevCmn->fnSCR_DisplayString(2, "�ſ� ��� �� ī�带 �����ʽÿ�.");
			m_pDevCmn->fnSCR_DisplayString(3, "ī��� �����ҿ��� Ȯ�� �Ͻñ� �ٶ��ϴ�.");
			m_pDevCmn->fnSCR_DisplayString(5, "������ ��� �˼��մϴ�.");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(3000);

			AddSerialFlg = FALSE;
			TranCode2Save = TRANID_4840;
			TranCode2 = TRANID_4840;
			fnAPP_SendHostCancel();	// SEND TRANDID_4212
			fnAPP_RecvHost();									// ����
			if (m_pDevCmn->TranResult)							// HOST OK
				fnAPP_CancelProc(T_MSG, "�����н� �ſ�ī�� ���� �κ��� ��� �Ǿ����ϴ�", "���� ī��翡 �ܾ��� Ȯ�� �Ͻñ� �ٶ��ϴ�");
			else
				fnAPP_CancelProc(T_MSG, "�����н� �ſ�ī�� ������Ұ� �����Ͽ����ϴ�.", "�����н� �����ҿ� �����Ͻñ� �ٶ��ϴ�.");
		}

	}
	
	CancelHipassPrintFlg = TRUE;
	Accept.SlipChoiceFlag = TRUE;

	fnAPP_PSPProc(DEV_JPR);    									// ��������Ʈ
	fnAPP_PSPProc(DEV_SPR);    									// ������Ʈ
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_PBM);	

	return T_OK;
}

int CTranCmn::fnAPP_HIPASS_CheckDeviceActionProc()
{
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// ��ü���������ȸ��

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
	fnAPP_SendHost();										// �۽�
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
	fnAPP_SendHost();										// �۽�
	fnAPP_RecvHost();

	if (m_pDevCmn->TranResult)
	{
		fnAPD_HPSComplete();
	}

	return T_OK;
}

