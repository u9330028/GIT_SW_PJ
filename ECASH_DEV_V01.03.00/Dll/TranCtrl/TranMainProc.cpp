/////////////////////////////////////////////////////////////////////////////
// TranMain.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"
#include <tlhelp32.h>
#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	�����Լ�
/////////////////////////////////////////////////////////////////////////////
// �������ó��
int CTranCmn::fnAPP_TranVirtualMainProc()
{
	try 
	{

		fnAPP_TranMainProc();									// ����ó��
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranVirtualMainProc (%d)", m_pDevCmn->TranStatus);

	}
	catch (CTranCmnExit& e)
	{	
		e.TranExitResult = e.TranExitResult;
	}

	return T_OK;
}

int CTranCmn::fnAPP_TranVirtualTranReadyProc()
{
	if (!m_pDevCmn->fnAPL_CheckHostOpen())						
		return T_OK;

	try 
	{
		if(m_pDevCmn->HostFirstReadyFlag)
		{
			fnAPP_TranReadyProc();	
			m_pDevCmn->HostFirstReadyFlag = FALSE;
		}
	}
	catch (CTranCmnExit& e)
	{	
		e.TranExitResult = e.TranExitResult;
	}

	return T_OK;
}

int CTranCmn::fnAPP_TranVirtualStatusProc()
{
	if (!m_pDevCmn->fnAPL_CheckHostOpen())						
		return T_OK;

	try 
	{
		m_pDevCmn->TranResult = FALSE;						
		m_pDevCmn->TranStatus = TRAN_STATUS;				
		m_pDevCmn->TranStatusSave = m_pDevCmn->TranStatus;
		fnAPP_TranStatusProc();
	}
	catch (CTranCmnExit& e)
	{	
		e.TranExitResult = e.TranExitResult;
	}

	return T_OK;
}

// ����ó��
int CTranCmn::fnAPP_TranMainProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMainProc:m_pDevCmn->TranStatus[%d]", m_pDevCmn->TranStatus);

	switch (m_pDevCmn->TranStatus) 
	{
		case TRAN_READY:										// �غ��ó��
			fnAPP_TranReadyProc();
			break;

		case TRAN_STATUS:										// �غ��ó��
			NmsErrorSendFlg = FALSE;
			fnAPP_TranStatusOpenProc();
			break;

		case TRAN_OPEN:											// ����ó�� 
			NmsErrorSendFlg = FALSE;
			fnAPP_TranOpenProc();
			break;

		case TRAN_TRAN:											// �ŷ�ó��
			fnAPPM_TranTranProc();
			break;

		case TRAN_TOTAL:										// �հ�ó��
			fnAPP_TranTotalProc();
			break;

		case TRAN_SUBTOTAL:										// �Ұ�ó��
			fnAPP_TranSubTotalProc();
			break;

		case TRAN_CASH_REG:										// ������ݵ��
			fnAPP_TranCashRegProc();
			break;

		case TRAN_CHECK_REG:									// ��ǥ���
		case TRAN_CHECK_UNREG:									// ��ǥ����
			fnAPP_TranCheckProc();
			break;
		default:
			break;
	}

	fnAPP_TerminateProc(T_OK);									// ��������

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	�����Լ�
/////////////////////////////////////////////////////////////////////////////
// Initialize Procedure
int CTranCmn::fnAPP_TranInitProc()
{
//	MainTranDoingFlag = TRUE;									// Main Transaction Doing Flag
	MainTranDoingCount = 0;										// Main Transaction Doing Count
	return T_OK;
}

// Menu Procedure
int CTranCmn::fnAPP_TranMenuProc()
{
	fnAPP_MenuProc();											// Main Procedure
	return T_OK;
}

// Get Transaciton Procedure
int CTranCmn::fnAPP_TranGetTranProc()
{
	BOOL TranTC_F = FALSE;

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("START:02=====>>=====>>=====>>=====>>=====>>>>"));
	memset(Accept.CardReadTime, ' ', sizeof(Accept.CardReadTime));
	sprintf(Accept.CardReadTime, "%4.4s%6.6s", &(GetDate().GetBuffer(0)[4]), GetTime().GetBuffer(0));
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranGetTranProc Start Time - (%10.10s)", Accept.CardReadTime);
	int nRet = 0;
//	nRet = fnAPP_TranHostStatus_Confirm();
	if (nRet)
	{
		fnAPP_CancelProc(T_MSG, "�ش����� � �غ� �� �Դϴ�", "ûȣ����ĳ��(��)���� ���� �ٶ��ϴ�");
		return T_OK;
	}


	

	switch (TranProc)
	{
		case TRAN_WITH:											// WITHDRAW
			if(TranCode == TC_WITHDRAW)
			{
				if(CardTran & MENU_J3T3)
				{
					TranCode2 = TRANID_4310;
				}
				else
				if(PbTran & MENU_PB)
				{
					TranCode2 = TRANID_4310;
				}
				else
				if(MenuAvail & MENU_MU)
				{
					if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
						CardBank = KIUBANK; 
					else
					if(m_pProfile->DEVICE.ETCDevice20 == HANA)
						CardBank = HANBANK; 
					
					TranCode2 = TRANID_4310;
				}
				else TranTC_F = TRUE;
			}
			else
			if(TranCode == TC_SVCWITH)    
			{
				TranCode2 = TRANID_4330;
			}
			else TranTC_F = TRUE;
			m_nRetCode = RET_WITHDRAW;
			break;

		case TRAN_INQ:											// INQUIRY
			if(TranCode == TC_INQUIRY)
			{
				if(CardTran & MENU_J3T3)
				{
					TranCode2 = TRANID_7220;
				}
				else
				if(PbTran & MENU_PB)							// ���� ���� �ܾ���ȸ
				{
					TranCode2 = TRANID_7220;
				}	
				else TranTC_F = TRUE;
				m_nRetCode = RET_INQUIRY;
			}
			else
			if(TranCode == TC_SVCINQ || TranCode == TC_SVCINQ1)  
			{
				TranCode2 = TRANID_7230;
				m_nRetCode = RET_INQUIRY;
			}
			break;

		case TRAN_TRANS:										// TRANSFER
			if(TranCode == TC_TRANSFER)
			{
				if(CardTran & MENU_J3T3)
				{
					TranCode2 = TRANID_7260;
				}
				else
				if(PbTran & MENU_PB)
				{
					TranCode2 = TRANID_7260;
				}
				else TranTC_F = TRUE;
				m_nRetCode = RET_TRANSFER;
			}
			else
			if(TranCode == TC_SVCTRAN || TranCode == TC_SVCTRAN1)
			{
				if(CardTran & MENU_T2)
				{
					m_nRetCode = RET_TRANSFER;
				}
				else TranTC_F = TRUE;
			}
			break;

		case TRAN_DEP:											// DEPOSIT
			if(MenuAvail & MENU_MU)
			{
				if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
					CardBank = KIUBANK;
				else
				if(m_pProfile->DEVICE.ETCDevice20 == HANA)
					CardBank = HANBANK; 
			}
			TranCode2 = TRANID_7200;
			m_nRetCode = RET_DEPOSIT;
			break;

		case TRAN_PB:											// PASSBOOK
			m_nRetCode = RET_PASSBOOK;
			break;

		case TRAN_FOREGIN:										// �ؿ�ī��
		case TRAN_FOREGIN_INQ:									// �ؿ�ī�� ��ȸ 
			m_nRetCode = RET_COMMONSUB;							
			break;
		default:
			fnAPP_CancelProc(T_PROGRAM);
			break;
	}

	if (TranCode == TC_FOREGIN)
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�ؿ�ī�� : [%s]",
												(CardServiceFlag == VISA_CARD)		? "VISA" : 
												(CardServiceFlag == MASTERCARD)		? "MASTER" : 
												(CardServiceFlag == CUPCARD)	    ? "CUP" : 
												(CardServiceFlag == AMEXCARD)		? "AMEX" : 
												(CardServiceFlag == JCBCARD)		? "JCB" : 
												(CardServiceFlag == DINERS)		    ? "DINERS" : 
												(CardServiceFlag == DISCOVER)		? "DISCOVER" : 
												//(CardServiceFlag == STAR)		    ? "STAR" : 
												(CardServiceFlag == JOINCARD)		? "ETC" : "ETC")); 	



	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��    �� : [%s]",
											(TranCode2 == TRANID_4310)		? "�������" : 
											(TranCode2 == TRANID_4330)		? "�ſ����" : 
											(TranCode2 == TRANID_7220)	    ? "������ȸ" : 
											(TranCode2 == TRANID_7320)		? "�ſ���ȸ" : 
											(TranCode2 == TRANID_4390)		? "������ü" :  
											(TranCode2 == TRANID_7260)		? "������ü" : 
											(TranCode2 == TRANID_4410)		? "�����Ա�" : 

											(TranCode2 == TRANID_7200)		? "�����Ա�" : 
											(TranCode2 == TRANID_4350)		? "�ſ���ü" : 
											(TranCode2 == TRANID_4730)		? "��������" : 
											(m_nRetCode == JOINCARD)		? "����" : "��Ÿ")); 

	fnAPP_Certificate_Confirm(); 
	fnAPP_CreditUnUsed_Check();

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranGetTranProc TranTC_F = %d TranCode2 = %d m_nRetCode = %d CardTran = %x PbTran = %x", TranTC_F, TranCode2,m_nRetCode, CardTran, PbTran);
	if(TranTC_F) fnAPP_CancelProc(T_PROGRAM);
	
	return T_OK;
}

// Get Menu Doing Procedure
int CTranCmn::fnAPP_TranGetMenuDoingProc()
{
	int nTranTmpCode;
	nTranTmpCode = TranCode2;


	//#CS0001
	if (m_pDevCmn->TranResult == TRUE)
	{
		if ((TranProc == TRAN_WITH) || (TranProc == TRAN_DEP))
		{
			m_pDevCmn->Timer.HostStatusSendTime = LINE_OPCL_TIME * 6;
			m_pTranCmn->ForceSendStatus         = STATUS_ON_TIME;
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�ŷ��Ϸ�: TIME (%6.6s)", GetTime().GetBuffer(0)));
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMainProc: End TIME OK(%6.6s)", GetTime().GetBuffer(0));
		}
		else
		{
			m_pDevCmn->Timer.HostStatusSendTime = STATUS_OFF_TIME;
			m_pTranCmn->ForceSendStatus         = STATUS_OFF_TIME;
		}
	}
	else
	{
		m_pDevCmn->Timer.HostStatusSendTime = STATUS_OFF_TIME;
		m_pTranCmn->ForceSendStatus         = STATUS_OFF_TIME;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMainProc: End TIME NG(%6.6s)", GetTime().GetBuffer(0));
	}

	fnAPP_InitializeDS();
	fnAPP_TranGetContinueProc(nTranTmpCode);		
	return T_OK;
}

// �غ��ó�� 
int CTranCmn::fnAPP_TranReadyProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranReadyProc()");

	int	nGetKeyF = 0;
	fnAPP_InitializeDS();										

	if(!m_pDevCmn->HostFirstOpenFlag) 
	{
		m_pDevCmn->TranResult = TRUE;
		return T_OK;
	}

	m_pDevCmn->fnSCR_DisplayString(2, "(Ű��ȯ : ���)");
	m_pDevCmn->fnSCR_DisplayString(3, "�����ð����� �����������̴�");
	m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
	m_pDevCmn->fnSCR_DisplayScreen(701);

	TranCode = TC_READY;									
	TranCode2 = TRANID_2190;

	TranResultNGCode = 0; 
	AddSerialFlg = FALSE;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(���) => ����]"));
	CardBank = KUB_ORG_INT_003;
	sprintf(m_B_E_INFO_INI.m_B_S_C[1].k_s_flag,"%2.2s","00");
	fnAPP_BankKey_Set_Flag(KUB_ORG_INT_003, K_S_F);
	fnAPP_Bank_Key_Convert(CardBank);
	fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME);
	fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME);
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(���) => �Ϸ�]"));

//OK -->

	m_pDevCmn->fnSCR_DisplayString(2, "(Ű��ȯ : ����)");
	m_pDevCmn->fnSCR_DisplayString(3, "�����ð����� �����������̴�");
	m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
	m_pDevCmn->fnSCR_DisplayScreen(701);

	TranResultNGCode = 0; 
	AddSerialFlg = FALSE;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) => ����]"));
	CardBank = KBS_ORG_INT_006;
	sprintf(m_B_E_INFO_INI.m_B_S_C[0].k_s_flag,"%2.2s","00");
	fnAPP_BankKey_Set_Flag(KBS_ORG_INT_006, K_S_F);
	fnAPP_Bank_Key_Convert(CardBank);
	fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME);
	fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME);
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) => �Ϸ�]"));

/*********************************************************************************************/	
// 	TranResultNGCode = 0;
// 	AddSerialFlg = FALSE;
// 	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) => ����]"));
// 	CardBank = NON_ORG_INT_011;
// 	fnAPP_Bank_Key_Convert(CardBank);
// 	fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME);
// 	fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME);
// 	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) => �Ϸ�]"));
/*********************************************************************************************/	


//OK -->
	m_pDevCmn->fnSCR_DisplayString(2, "(Ű��ȯ : ����)");
	m_pDevCmn->fnSCR_DisplayString(3, "�����ð����� �����������̴�");
	m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
	m_pDevCmn->fnSCR_DisplayScreen(701);

	TranResultNGCode = 0;
	AddSerialFlg = FALSE;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) => ����]"));
	CardBank = KJB_ORG_INT_034;
	fnAPP_Bank_Key_Convert(CardBank);
	sprintf(m_B_E_INFO_INI.m_B_S_C[7].k_s_flag,"%2.2s","00");
	fnAPP_BankKey_Set_Flag(KJB_ORG_INT_034, K_S_F);
	fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME);
	fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME);
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) => �Ϸ�]"));

//OK -->
	m_pDevCmn->fnSCR_DisplayString(2, "(Ű��ȯ : ��ȯ)");
	m_pDevCmn->fnSCR_DisplayString(3, "�����ð����� �����������̴�");
	m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
	m_pDevCmn->fnSCR_DisplayScreen(701);

	TranResultNGCode = 0;
	AddSerialFlg = FALSE;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(��ȯ) => ����]"));
	CardBank = KEB_ORG_INT_005;
	fnAPP_Bank_Key_Convert(CardBank);
	sprintf(m_B_E_INFO_INI.m_B_S_C[8].k_s_flag,"%2.2s","00");
	fnAPP_BankKey_Set_Flag(KEB_ORG_INT_005, K_S_F);
	fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME);
	fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME);
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(��ȯ) => �Ϸ�]"));

//OK -->
	m_pDevCmn->fnSCR_DisplayString(2, "(Ű��ȯ : �츮)");
	m_pDevCmn->fnSCR_DisplayString(3, "�����ð����� �����������̴�");
	m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
	m_pDevCmn->fnSCR_DisplayScreen(701);

	TranResultNGCode = 0;
	AddSerialFlg = FALSE;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(�츮) => ����]"));
	CardBank = WRB_ORG_INT_020;
	fnAPP_Bank_Key_Convert(CardBank);
	sprintf(m_B_E_INFO_INI.m_B_S_C[9].k_s_flag,"%2.2s","00");
	fnAPP_BankKey_Set_Flag(WRB_ORG_INT_020, K_S_F);
	fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME);
	fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME);
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(�츮) => �Ϸ�]"));


/*********************************************************************************************/	
// 	TranResultNGCode = 0;
// 	AddSerialFlg = FALSE;
// 	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) => ����]"));
// 	CardBank = SCO_ORG_INT_023;
// 	fnAPP_Bank_Key_Convert(CardBank);
// 	fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME);
// 	fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME);
// 	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(SC) => �Ϸ�]"));
/*********************************************************************************************/	

//OK -->
	m_pDevCmn->fnSCR_DisplayString(2, "(Ű��ȯ : NH��������)");
	m_pDevCmn->fnSCR_DisplayString(3, "�����ð����� �����������̴�");
	m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
	m_pDevCmn->fnSCR_DisplayScreen(701);

	TranResultNGCode = 0;
	AddSerialFlg = FALSE;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(NH��) => ����]"));
	CardBank = WRT_ORG_INT_247;
	fnAPP_Bank_Key_Convert(CardBank);
	sprintf(m_B_E_INFO_INI.m_B_S_C[4].k_s_flag,"%2.2s","00");
	fnAPP_BankKey_Set_Flag(WRT_ORG_INT_247, K_S_F);
	fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME);
	fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME);
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(NH��) => �Ϸ�]"));


	m_pDevCmn->fnSCR_DisplayString(2, "(Ű��ȯ : �Ｚ����)");
	m_pDevCmn->fnSCR_DisplayString(3, "�����ð����� �����������̴�");
	m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
	m_pDevCmn->fnSCR_DisplayScreen(701);

	TranResultNGCode = 0; 
	AddSerialFlg = FALSE;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) => ����]"));
	CardBank = SST_ORG_INT_240;
	sprintf(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,"%2.2s","00");
	fnAPP_BankKey_Set_Flag(CardBank, K_S_F);
	fnAPP_Bank_Key_Convert(CardBank);
	fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME);
	fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME);
	nGetKeyF = fnAPP_Bank_Key_Convert(CardBank);
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) => %s]", (nGetKeyF == CIPHERTEXT_TMP) ? "�Ϸ�" : "����"));
	if(nGetKeyF == CIPHERTEXT_TMP)
	{
		AddSerialFlg = FALSE;
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) =+ ����]"));
		fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME, 1);
		fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME, 1);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) =+ %s]", (m_pDevCmn->TranResult == TRUE) ? "�Ϸ�" : "����"));
	}

//OK -->

	m_pDevCmn->fnSCR_DisplayString(2, "(Ű��ȯ : ��ȭ����)");
	m_pDevCmn->fnSCR_DisplayString(3, "�����ð����� �����������̴�");
	m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
	m_pDevCmn->fnSCR_DisplayScreen(701);

	TranResultNGCode = 0; 
	AddSerialFlg = FALSE;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(��ȭ) => ����]"));
	CardBank = HWT_ORG_INT_269;
	sprintf(m_B_E_INFO_INI.m_B_S_C[6].k_s_flag,"%2.2s","00");
	fnAPP_BankKey_Set_Flag(HWT_ORG_INT_269, K_S_F);
	fnAPP_Bank_Key_Convert(CardBank);
	fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME);
	fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME);
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(��ȭ) => �Ϸ�]"));



//OK -->
	m_pDevCmn->fnSCR_DisplayString(2, "(Ű��ȯ : ��������)");
	m_pDevCmn->fnSCR_DisplayString(3, "�����ð����� �����������̴�");
	m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
	m_pDevCmn->fnSCR_DisplayScreen(701);

	TranResultNGCode = 0; 
	AddSerialFlg = FALSE;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) => ����]"));
	CardBank = DOB_ORG_INT_279;
	sprintf(m_B_E_INFO_INI.m_B_S_C[4].k_s_flag,"%2.2s","00");
	fnAPP_BankKey_Set_Flag(DOB_ORG_INT_279, K_S_F);
	fnAPP_Bank_Key_Convert(CardBank);
	fnAPP_Tran_TxSend2( TRANID_2190, LINE_2190_TIME);
	fnAPP_Tran_RxRecv2( TRANID_2190, LINE_2190_TIME);
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����(����) => �Ϸ�]"));

	CardBank = 0;

	m_pDevCmn->TranResult = TRUE;
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// ���༳��
	else
		m_pDevCmn->fnAPL_SetProcCount('9');					// HOST NG
	
	if (m_pDevCmn->TranResult)									// HOST OK
	{
//		fnAPP_ReadyProc();										// �غ��ó��
		fnAPP_ReadyPrintResult(DEV_JPR, TRUE);					// �غ�ϰ������
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranReadyProc():return");
	return T_OK;
}

// ����ó��
int CTranCmn::fnAPP_TranOpenProc()
{
	char	StrBuff[256] = {0,};
	char	szYYYYMMDD[9] = {0,};
	char	szHHMMSS[6] = {0,};
	CString	cstrTemp("");
	CStringArray strTempArray;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranOpenProc()");

	fnAPP_InitializeDS();										// �����ʱ�ȭ
	TranCode = TC_OPEN;											// ����
	TranCode2 = TRANID_A010;
	m_pDevCmn->fnAPL_SetProcCount('3');							// ���༳��
	fnAPP_SendHost();											// �۽�
	fnAPP_RecvHost();											// ����
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// ���༳��
	else
		m_pDevCmn->fnAPL_SetProcCount('9');						// HOST NG

	if (m_pDevCmn->TranResult)									// HOST OK
	{
		fnAPP_OpenProc();										// ����ó��
		
		if(m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)
		{
			GetDate(szYYYYMMDD);							// ���ø���� ��� �ý��۳�¥ ����
			m_pDevCmn->fnAPL_SetDate(szYYYYMMDD);
			m_pDevCmn->fnAPL_SetCheckDate(szYYYYMMDD);
		}
		else 
		if(m_pDevCmn->TranResult)
		{
			memcpy(szYYYYMMDD, &m_RD.byDataField_002x, 8);
			memcpy(szHHMMSS, &m_RD.byDataField_002x[8], 6);
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����Ϸ� : TIME SET (%s)]", m_RD.byDataField_002x));


			m_pDevCmn->fnAPL_SetDate(szYYYYMMDD);
			m_pDevCmn->fnAPL_SetCheckDate(szYYYYMMDD);
			
			if (TranResultNGCode == TRAN_MSG_EXCEPTION_845)
			{
				fnAPP_Set_SystemTime(m_RD.byDataField_002x);
			}
		
			if (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET)		
			{
				HWND hWnd;
				hWnd = _FIND_CFMON;
				if(hWnd)
					::PostMessage(hWnd, WUM_ADCONTENTS, 1, 0);
			}													
		}
	
		fnAPP_OpenPrintResult(DEV_JPR, (LPCTSTR)szYYYYMMDD, (LPCTSTR)szHHMMSS, TRUE);
																// �����������
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranOpenProc(NG):return");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranOpenProc():return");
	return T_OK;
}

// ����/��� ó��
int CTranCmn::fnAPP_TranStatusProc()
{
	int nReturn;
	if(m_pDevCmn->AtmStatus == ATM_TRAN) return T_OK;

	TranCode = TC_STATUS;
	TranCode2 = TRANID_A020;

	if(!m_pDevCmn->AtmFirstDoingFlag)
		nReturn = fnAPP_NmsProc(TRUE);
	else
		nReturn = fnAPP_NmsProc(FALSE);

	if(nReturn == T_OK) 		return T_OK;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranStatusProc()");
	AddSerialFlg = FALSE;

	if(nReturn == TRAN_TXRX_STATUS)
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��������: ���� - �۽�]"));
	else
	if(nReturn == TRAN_TXRX_ERROR) 
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��������: ��� - �۽�]"));
	else
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��������: ��Ÿ - �۽�]"));

	m_pDevCmn->fnAPL_SetProcCount('3');							// ���༳��

	fnAPP_Tran_TxSend2( TRANID_A020, LINE_SEND_5_TIME);
	//  �ӵ�����
 	fnAPP_Tran_RxRecv2( TRANID_A020, LINE_SEND_5_TIME);
	
// 	ULONG WiatTime = TimerSet(K_30_WAIT * K_30_WAIT);				// POWEROFF 10�ʴ��
// 	while (TRUE)
// 	{	
// 		
// 		fnAPP_SendHost();											// �۽�
// 		fnAPP_RecvHost();											// ����
// 		if (m_pDevCmn->TranResult)									// HOST OK
// 		{
// 			m_pDevCmn->fnAPL_SetProcCount('5');						// ���༳��
// 			break;
// 		}
// 		else
// 		{
// 			m_pDevCmn->fnAPL_SetProcCount('9');						// HOST NG
// 			Delay_Msg(K_30_WAIT * 1000);									
// 		}
// 
// 		if (CheckTimer(WiatTime))
// 		{
// 			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("������  : Status RECV NG"));
// 			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
// 			break;
// 		}
// 
// 	}

//	m_pDevCmn->TranResult = TRUE;

	if (m_pDevCmn->TranResult)									// HOST OK
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranStatusProc(OK):return");
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranStatusProc(NG):return");
	}
	return T_OK;
}

// ����/��� ó��
int CTranCmn::fnAPP_TranStatusOpenProc()
{

	TranCode = TC_STATUS;
	TranCode2 = TRANID_A020;


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranStatusOpenProc()");
	
	AddSerialFlg = FALSE;

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��������: ���� - �۽�]"));
	fnAPP_NmsUpdateStatus();									// ���¹��� �ۼ�
	m_pDevCmn->fnAPL_SetProcCount('3');							// ���༳��
	fnAPP_SendHost();											// �۽�
	fnAPP_RecvHost();											// ����
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// ���༳��
	else
		m_pDevCmn->fnAPL_SetProcCount('9');						// HOST NG

	if (m_pDevCmn->TranResult)									// HOST OK
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranStatusOpenProc(OK):return");
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranStatusOpenProc(NG):return");
	}
	return T_OK;
}

// �հ�ó��
int CTranCmn::fnAPP_TranTotalProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranTotalProc()");
	char		szGetDateTime[256];
	char szDate[9], szTime[7];
	GetDateTime(szDate, szTime);
	sprintf(szGetDateTime,"%8.8s%6.6s",  szDate, szTime);
	fnAPP_InitializeDS();										// �����ʱ�ȭ

	TranCode = TC_TOTAL;										// �հ�
	TranCode2 = TRANID_A060;

	m_pProfile->GetWithTotal();
	m_pProfile->GetDepTotal();
	m_pProfile->GetTransTotal();
	m_pProfile->GetDepErrTotal();
	m_pProfile->GetWithErrTotal();

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�������� : �հ�"));

	m_pDevCmn->fnSCR_DisplayString(2, "���� ���� ���� �� �Դϴ�");
	m_pDevCmn->fnSCR_DisplayString(3, "���� �ð����� ���������̿���");
	m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
	m_pDevCmn->fnSCR_DisplayScreen(701);

//	fnAPP_MiniATM_SumPrint1((DEV_SPR | DEV_JPR), TRUE);
	fnAPP_MiniATM_SumPrint2((DEV_SPR | DEV_JPR), TRUE);

	m_pDevCmn->fnAPL_SetProcCount('3');							// ���༳��		
//	IniSetStr(_SP_BRM_CSTINFO_INI, "REFILL", "HOST_DATETIME_LAST", szGetDateTime);
	fnAPP_SendHost();											// �۽�
	IniSetStr(_SP_BRM_CSTINFO_INI, "REFILL", "HOST_DATETIME_LAST", szGetDateTime); 
	Om_GetCurrentActionRemain(TRUE);

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		//#0057
		RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0);					
		RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
		RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_5000",  0);
		RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_1000",  0);

		RegSetInt(_REGKEY_BRMSTATUS, "NDisp10000", 0); 
		RegSetInt(_REGKEY_BRMSTATUS, "NDisp50000", 0);

		RegSetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����
		RegSetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
		RegSetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
		RegSetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
		//Retract �ż�
		RegSetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //�̼���
		//Reject �ż�
		RegSetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //����Ʈ - �ҷ� 
		RegSetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt",0); //õ��,5õ�� - �Ա�
		RegSetInt(_REGKEY_BRMSTATUS, "ClerkRJBNoteCnt",0); //���ȸ���Ըż�

		m_pDevCmn->fnCSH_Initialize();								//#0037
//		fnAPP_TranCashRegProc();                                    //#0056
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		//�����ʿ�ݵ���
		IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",    0);
		IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", 0);
		
		RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0);					
		RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
		RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_5000",  0);
		RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_1000",  0);

		RegSetInt(_REGKEY_BRMSTATUS, "NDisp10000", 0); 
		RegSetInt(_REGKEY_BRMSTATUS, "NDisp50000", 0);

		RegSetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����
		RegSetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
		RegSetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
		RegSetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
		//Retract �ż�
		RegSetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //�̼���
		//Reject �ż�
		RegSetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //����Ʈ - �ҷ� 
		RegSetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt",0); //õ��,5õ�� - �Ա�
		RegSetInt(_REGKEY_BRMSTATUS, "ClerkRJBNoteCnt",0); //���ȸ���Ըż�

	}
	m_pProfile->InitWithTotal(1);
	m_pProfile->InitWithTotal(2);
	m_pProfile->InitDepTotal(1);
	m_pProfile->InitDepTotal(2);
	m_pProfile->InitTransTotal();
	m_pProfile->InitDepErrTotal();
	m_pProfile->InitWithErrTotal();
	m_pProfile->ClearAllErrCancel();
	m_pProfile->ClearAllErrTran();
	//m_pProfile->ClearAllErrStack();       
	m_pProfile->DetailHClearAllErrStack();

	fnAPP_RecvHost();											// ����
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// ���༳��
	else 
	{
		m_pDevCmn->fnAPL_SetProcCount('9');					    // HOST NG
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranTotalProc():return");
	return T_OK;
}

// �Ұ�ó��
int CTranCmn::fnAPP_TranSubTotalProc(int SprPrintFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranSubTotalProc()");
	int nDevId = DEV_JPR;

	fnAPP_InitializeDS();										// �����ʱ�ȭ
	TranCode = TC_SUBTOTAL;										// �Ұ�
	TranCode2 = 0;
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�������� : �Ұ�"));

	fnAPP_MiniATM_CashPrint((DEV_SPR | DEV_JPR), TRUE);			// �Ұ�����

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranSubTotalProc():return");
	return T_OK;
}
// ���ݵ��
int CTranCmn::fnAPP_TranCashRegProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranCashRegProc()");

//	fnAPP_InitializeDS();										// �����ʱ�ȭ
	fnAPP_TranSetCashProc();												
//	m_pDevCmn->fnCSH_Initialize();							

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranCashRegProc():return");
	return T_OK;
}

int CTranCmn::fnAPP_TranSetCashProc(int nClearFlag)
{
	CString strTemp("");
	CString		strDispCntData("");								// 2004.11.05
	CString		strCashCntData("");
	CString		strTotalCashCntData("");
	int nNoteKind[4] = {BRM_NOTEKIND_10000_OLD, };
	nNoteKind[0] = BRM_NOTEKIND_10000_NEW;			// �Ÿ����� 
	nNoteKind[1] = BRM_NOTEKIND_10000_NEW;			// �Ÿ����� 
	nNoteKind[2] = BRM_NOTEKIND_INPUT_DEP_ONLY;		// �Ա����� 
	nNoteKind[3] = BRM_NOTEKIND_10000_NEW;			// �Ÿ����� : 2009.07.30
	
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{

		//#N0228 -> 2014.09.23
		//m_pDevCmn->fnBRM_SetNoteInfo(nNoteKind[0], nNoteKind[1], nNoteKind[2], nNoteKind[3]); -> �ݾ׼������� ���̽� ���Ұ� (�ѳ�Ʈ�� ����)
		if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("ī����ȯ : 5���� - 1CST[%d], 1���� - 2CST[%d], 1���� - 3CST[%d], 1���� - 4CST[%d]", 
				m_pDevCmn->FstCSTSetCash, m_pDevCmn->SndCSTSetCash, m_pDevCmn->TrdCSTSetCash, m_pDevCmn->FthCSTSetCash));

			strTemp.Format("%s||%s||%s||%s", "�Ÿ�","�Ÿ�","�Ա�����","�Ÿ�");
			SetShareData("OM_ī��Ʈ��������", strTemp);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranCashRegProc():fnBRM_SetNoteInfo()");
		}

	}

	return T_OK;
	
}
// ��ǥ���
int CTranCmn::fnAPP_TranCheckProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranCheckProc()");

	fnAPP_InitializeDS();										// �����ʱ�ȭ

	if (m_pDevCmn->TranStatus == TRAN_CHECK_REG)
		TranCode = TC_CHECK_REG;
	else
	{
		TranCode = TC_CHECK_UNREG;
		m_pDevCmn->fnAPL_SetNumberOfInCheck(0);

	}
	TranCode2 = TRANID_0300;

 	m_pDevCmn->fnAPL_SetProcCount('3');							// ���༳��
 	fnAPP_SendHost();											// �۽�
 	fnAPP_RecvHost();											// ����
 	if (m_pDevCmn->TranResult)									// HOST OK
 		m_pDevCmn->fnAPL_SetProcCount('5');						// ���༳��
 	else m_pDevCmn->fnAPL_SetProcCount('9');					// HOST NG

//	m_pDevCmn->TranResult = TRUE;								// ���ü�ǥ��ϸ� �ֽ�
	m_pProfile->InitWithTotal(2);
	m_pProfile->InitDepTotal(2);

	if ((!m_pDevCmn->TranResult)					&&
		(m_pDevCmn->TranStatus == TRAN_CHECK_REG)	)
		m_pDevCmn->fnAPL_SetNumberOfOutCheck(0, 0);
	
	if ((m_pDevCmn->TranResult)						&&
		(m_pDevCmn->TranStatus == TRAN_CHECK_UNREG)	)
		m_pDevCmn->fnAPL_SetNumberOfOutCheck(0, 0);

	if (m_pDevCmn->TranStatus == TRAN_CHECK_REG)
	{
		fnAPP_CheckResultPrint(DEV_JPR);				// ��ǥ��ϰ������
		fnAPP_CheckResultPrint(DEV_SPR);				// ��ǥ��ϰ������
		fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR);		// ���ۿϷ����&��ְ���
	}
	else
	{
		m_pProfile->InitWithTotal(2);
		m_pProfile->InitDepTotal(2);
	}

	fnAPD_MaterialRetract(DEV_SPR);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranCheckProc():return");
	return T_OK;
}

// �����ν� Ȯ�� ����
int CTranCmn::fnAPP_TranMsFngConfirm()
{

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranFngConfirm():return");
	return T_OK;
}


// �������� (����ó��)
int CTranCmn::fnAPP_TranAliveProc()
{

//	fnAPP_InitializeDS();										// �����ʱ�ȭ

	TranCode = TRANID_0006;
	TranCode2 = TRANID_0006;

 	if(m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave) return T_OK; 
	
	int     nSendLength =0;
	int		nSlen = 0;
	int		nBcc = 0;
	BYTE	strDummy[1024];
	char	szTmp[1024];
	BYTE	byTmpBuff[1024];
	BYTE	bySendBuff[1024];

	BYTE	byTLen[4];
	BYTE	byTData[9216];
	BYTE	byBcc[2];

	memset(strDummy,   0x20, sizeof(strDummy));
	memset(&byTLen,   0x20, sizeof(byTLen));
	memset(&byTData,   0x20, sizeof(byTData));
	memset(&byBcc,   0x20, sizeof(byBcc));
	memset(szTmp,      0x00, sizeof(szTmp));
	memset(bySendBuff,      0x00, sizeof(bySendBuff));

	nSlen = 0;
	memcpy(&strDummy[nSlen], &m_pProfile->NETWORK.BranchNum, 6);
	nSlen = 6;
	memcpy(&strDummy[nSlen], &m_pProfile->NETWORK.AtmSerialNum, 8);
	nSlen += 8;
	memcpy(&strDummy[nSlen], m_pProfile->NETWORK.AtmIPAddress, 15);
	nSlen += 15;


	Int2Asc(nSlen+sizeof(byBcc), byTLen, sizeof(byTLen));
	memcpy(byTData, strDummy, nSlen);
	byTmpBuff[0] = MakeBcc(byTData, nSlen);
	MakeUnPack(byTmpBuff, byBcc, 1);

	memcpy(szTmp,                       byTLen, sizeof(byTLen));
	memcpy(&szTmp[sizeof(byTLen)],      byTData, nSlen);
	memcpy(&szTmp[sizeof(byTLen)+nSlen],byBcc, sizeof(byBcc));

	nSendLength = sizeof(byTLen) + nSlen + sizeof(byBcc);

	memcpy(&bySendBuff[0], szTmp, nSendLength);

	MsgDump(TRACE_CODE_MODE, "POLL", __FILE__, __LINE__, "fnAPP_TranAliveProc [%s] - [%d]", bySendBuff, nSendLength);
	int nRet = fnAPP_SendData(bySendBuff, nSendLength, LINE_SEND_TIME);

	return T_OK;
}

// DES : TMK/TPK : 2003.12.15
int CTranCmn::fnAPP_TranDesProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDesProc()");

	if (!m_pDevCmn->DesITKStatus)								// ITK����Ȯ��
		return T_OK;

	fnAPP_InitializeDS();										// �����ʱ�ȭ

	if ((!m_pProfile->DEVICE.DesMasterKey) ||					// TMK NOT SET
		(!m_pDevCmn->DesTMKStatus)			)
	{
		TranCode = TC_TMKLOAD;									// TMK Load
		TranCode2 = 0;

		AddSerialFlg = FALSE;									// �Ϸù�ȣ����������
																// DES OutGoing�ŷ� 2004.05.31 ����ó��

		m_pDevCmn->fnAPL_SetProcCount('3');						// ���༳��
		fnAPP_SendHost();										// �۽�
		fnAPP_RecvHost();										// ����
		if (m_pDevCmn->TranResult)								// HOST OK
			m_pDevCmn->fnAPL_SetProcCount('5');					// ���༳��
		else m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG

		if (m_pDevCmn->TranResult)
		{
			fnAPP_RecvDesKeyProc(&PMERecvData);
		}
		fnAPP_OpenDesPrintResult(DEV_JPR, TRUE);				// DES�����������
	}

	if (m_pProfile->DEVICE.DesMasterKey)
	{
		TranCode = TC_TPKLOAD;									// TPK Load
		TranCode2 = 0;
		AddSerialFlg = FALSE;									// �Ϸù�ȣ����������

		m_pDevCmn->fnAPL_SetProcCount('3');						// ���༳��
		fnAPP_SendHost();										// �۽�
		fnAPP_RecvHost();										// ����
		if (m_pDevCmn->TranResult)								// HOST OK
			m_pDevCmn->fnAPL_SetProcCount('5');					// ���༳��
		else m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG

		fnAPP_RecvDesKeyProc(&PMERecvData);
		fnAPP_OpenDesPrintResult(DEV_JPR, TRUE);				// DES�����������
	}


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDesProc():return");
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	Transaction Withdraw Function
/////////////////////////////////////////////////////////////////////////////
// Withdraw Initialize Procedure
int CTranCmn::fnAPP_TranWithInitProc()
{
	//#0121  sj.Kim 13.03.20 -> #N0189
	if((m_pProfile->DEVICE.ETCDevice20 == KEB) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		fnAPP_Accpet_KEB_Fee_Confirm();
	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == KIUP) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IKIUBANKID) && (TranCode == TC_WITHDRAW || TranCode == TC_MOBILEWITHDRAW))		// ������ุ ���Ӵ� ��� ����.
		{
			;
		}
		else
		{
			fnAPP_Accpet_KIUP_Fee_Confirm();
		}
	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == NONGHYUP) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == INONBANKID1 || CardBank == INONBANKID2 || CardBank == INONBANKID3 || CardBank == INONBANKID4)
			&& (TranCode == TC_WITHDRAW))
		{
			;
		}
		else
		{
			fnAPP_Accpet_NONGHYUP_Fee_Confirm();
		}

	}
	else
	if ((m_pProfile->DEVICE.ETCDevice20 == KWANGJU) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IKAWBANKID) && (TranCode == TC_WITHDRAW))
		{
			;
		}
		else
		{
			fnAPP_Accpet_KWANGJU_Fee_Confirm();
		}

	}
	else
	if ((m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))   //#N0192
	{
		if ((CardBank == IKYOBANKID) && (TranCode == TC_WITHDRAW))
		{
			;
		}
		else
		{
			fnAPP_Accpet_KYONGNAM_Fee_Confirm();
		}

	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == HANA) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))  //#N0214 �ϳ����� ���� Ÿ�� ���� ���� ���
	{
		fnAPP_Accpet_HANA_Fee_Confirm();
	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == SHINHAN) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))  //#N0214
	{
		if ((CardBank == ISHIBANKID || CardBank == ISHIBANKID1 || CardBank == ICHOBANKID)
			&& (TranCode == TC_WITHDRAW))
		{
			;
		}
		else
		{
			fnAPP_Accpet_SHINHAN_Fee_Confirm();
		}

	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == SC) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))  //#N0219
	{
		if ((CardBank == IKOFBANKID)
			&& (TranCode == TC_WITHDRAW))
		{
			;
		}
		else
		{
			fnAPP_Accpet_SC_Fee_Confirm();
		}

	}
	else //#N0226
	if((m_pProfile->DEVICE.ETCDevice20 == KB) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IKUMBANKID1 || CardBank == IKUMBANKID2 || CardBank == IKUMBANKID3 || CardBank == IKUMBANKID4 || CardBank == IKUMBANKID5) && (TranCode == TC_WITHDRAW))
		{
			;
		}
		else
		{
			fnAPP_Accpet_KB_Fee_Confirm();
		}
	}
	else //#N0238
	if((m_pProfile->DEVICE.ETCDevice20 == CITI) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IGOOBANKID1 || CardBank == IGOOBANKID2 || CardBank == ICITBANKID) && (TranCode == TC_WITHDRAW))
		{
			;
		}
		else
		{
			fnAPP_Accpet_CITI_Fee_Confirm();
		}
	}

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

	//#N0227
	if((TranCode == TC_SVCWITH) && !(EMVReadFlg))
	{
		if(CardOnEMV)
			;
		else
			fnAPP_Accpet_EMV_MS_Confirm();
	}

	return T_OK;
}

// Withdraw Input Procedure
int CTranCmn::fnAPP_TranWithInputProc()
{
	if (MainTranDoingCount == 0)
	{
		fnAPP_InitializeSerialTranDS();							// Initialize Serial Transaction DS
		fnAPP_AcceptWith();										// ����Է�
	}

	// EMVLib //#N9999
	if(m_pDevCmn->IfmDirect)								
	{
		fnAPLLib_EmvTranDataMake(TranCode);
	}

	//#0039
	if (PbTran & MENU_PB)
	{
		if(m_pProfile->DEVICE.ETCDevice20 == KEB && m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE) //#0085  //�ٱ��� ����ŷ��� ������Ʈ X
			fnAPP_Accpet_KEB_PbConfirmMSG();
		else
		if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)   //#N0181 => #N0214
			fnAPD_PbSpeedUpCheck();
	}


	if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		fnAPP_AcceptSlipChoice();
	else
	{
		m_pDevCmn->fnSCR_DisplayScreen(526);
		Delay_Msg(2000);
	}
	
	//  ��������ȸ ���� ���� (������ ��������ȸ �۽�)
	TranEncCode = fnAPP_EncType(TranCode2); 
	if(TranCode2 == TRANID_4310)
	{

		if ((TranEncCode != nEnCmd209_369) && (TranEncCode != nEnCmd238_368) && (TranEncCode != nEnCmd240_364) && (TranEncCode != nEnCmd032_32) && (TranEncCode != nEnCmd230_230) && (TranEncCode != nEnCmd243_243) && (TranEncCode != nEnCmd278_278))
		{
			TranCode2Save = TRANID_7210;
			TranCode2 = TRANID_7210;
			TranEncCode = fnAPP_EncType(TranCode2);
			fnAPP_SendHost();										
			fnAPP_RecvHost();
			fnAPP_TranWithChargeInqInputProc();
		}

		AddSerialFlg = FALSE;
		TranCode2Save = TRANID_4310;
		TranCode2 = TRANID_4310;			
		
	}
	else
	if(TranCode2 == TRANID_4330)   //
	{
		TranEncCode = fnAPP_EncType(TranCode2);
		//ûȣ(Ư�̻��)
		TranCode2Save = TRANID_7380;
		TranCode2 = TRANID_7380;

		fnAPP_SendHost();										
		fnAPP_RecvHost();

		if ( (EMVReadFlg)			 &&						// EMV CHIP READ OK
				 (m_pDevCmn->TranResult))
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV ���ŵ���Ÿ ó�� ����");
			//memset(m_pDevCmn->m_EMVszData, ' ', sizeof(m_pDevCmn->m_EMVszData));
				
			int nRet;
			nRet = 	fnAPP_EMVAfterOnlineTranProc();
				
			if (nRet != T_OK)								
			{
				fnAPP_CancelProc( T_MSG, "ī�� ���� ����! �ŷ��� ����մϴ�");
			}
		}

		fnAPP_TranWithChargeInqInputProc();
		fnAPP_AcceptPassword();
		AddSerialFlg = FALSE;
		TranCode2Save = TRANID_4330;
		TranCode2 = TRANID_4330;			
	}

	TranResultNGCode = 0;	
	fnAPD_CardEmbossScan();										// ī�忥������ĵ
	TranEncCode = fnAPP_EncType(TranCode2);

	return T_OK;
}

// Withdraw Output Procedure
int CTranCmn::fnAPP_TranWithOutputProc()
{
	char	szTempBuff[256] = {0,};
	int		nNextTran = 0;
	if (!m_pDevCmn->TranResult)
		CancelWithSendFlg = TRUE;								// ����������Flg(�ʱ�:���๫)

	RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 0);

	fnAPP_PSPProc(DEV_JPR);    								// ��������Ʈ
	fnAPP_PMDProc();										// �޼���ǥ��

	if (m_pDevCmn->TranResult)									// HOST OK
	{
		RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 1);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���ݰ�� ����"));
		fnAPD_MoneyCount();										// ���ݼ�ǥ��ݰ��
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���ݰ�� �Ϸ�"));
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
		
		if (m_pProfile->DEVICE.ETCDevice20 == HANA)								//#N0222 �ϳ����� �϶��� ��ǥ ī�� ���� ����
		{
			fnAPD_MaterialOut(DEV_MCU | DEV_SPR);								// ��ü���� #N0222
			fnAPD_MaterialOutCheckRetract(DEV_MCU | DEV_SPR);					//#N0222	
		}
		else
		{
			fnAPD_MaterialOut(DEV_MCU);								// ��ü���� #N0222
			fnAPD_MaterialOutCheckRetract(DEV_MCU);					//#N0222	
		}
		
		RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 3);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���Ϳ��� : ����"));
		
		if (m_pProfile->DEVICE.ETCDevice20 == HANA)
			fnAPD_MaterialOut(DEV_BRM);								// ��ü���� //#N0222 �ϳ����� �϶��� ��ǥ ī�� ���� ����
		else
			fnAPD_MaterialOut(DEV_BRM | DEV_SPR);					// ��ü����

		CancelWithSendFlg = TRUE;								// ����������Flg(�ʱ�:���๫)

		fnAPD_MoneyOutCheck();									// ���ݼ�ǥ���������
		RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 4);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���ʹ��� : �Ϸ�"));
		if (m_pProfile->DEVICE.ETCDevice20 != HANA)
			fnAPD_MaterialOutCheckRetract(DEV_SPR);				//#N0222 �ϳ����� �϶��� ��ǥ ī�� ���� ����
	}
	else
	{
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);			// ��ü����	
		fnAPD_MaterialOutCheckRetract(DEV_SPR |DEV_MCU | DEV_PBM);																// ��ü���������ȸ��
	}

	m_pDevCmn->fnAPL_CaptureSaveEnd();				

//	sprintf(szTempBuff, "[����:%5.5d�� ��ǥ:%4.4d��]", m_pDevCmn->fnAPL_GetNumberOfCash(), m_pDevCmn->fnAPL_GetNumberOfOutCheck());
	sprintf(szTempBuff, "��    �� : %5.5d��", m_pDevCmn->fnAPL_GetNumberOfCash());
	m_pDevCmn->fnAPL_PrintJnlInfo(szTempBuff, JNL_STATUS);

	//#0039
	if((m_pProfile->DEVICE.ETCDevice20 == NONGHYUP) && (m_pDevCmn->TranResult) && (PbTran & MENU_PB) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{

		TranCode2 = TRANID_7330;
		TranCode2Save = TRANID_7330;
		memset(Accept.Money, '0', sizeof(Accept.Money));			// �ݾ�(����), �ݾ�(��õ��)
		AddSerialFlg = FALSE;			
		fnAPP_SendHost();										// �۽�
		fnAPP_RecvHost();										// ����
		if (m_pDevCmn->TranResult)								// HOST OK
		{
			AddSerialFlg = FALSE;		
			TranCode2 = TRANID_4730;
			TranCode2Save = TRANID_4730;
		}
		else
		{
			fnAPP_CancelProc(T_MSG, "�� �������δ� ���ŷ��� �� �� �����ϴ�");
		}
	}


	if (m_pDevCmn->TranResult && (PbTran & MENU_PB) && m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
	{
		fnAPP_AcceptTranPBProc();
		while (PbNextTranFlag && nNextTran < 9)							// ��9������ �ۼ����ϹǷ� 3���� ��������
		{
			fnAPP_InitializeSerialTranDS();
			m_pDevCmn->fnAPL_SetProcCount('3');					// ���༳��			
			fnAPP_SendRecvHostSnod();							// �۽�
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				m_pDevCmn->fnAPL_SetProcCount('5');				// ���༳��
				fnAPP_PPPProc();								// ���������Ʈ
			}
			else
			{	
				m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG
				PbNextTranFlag = FALSE;
				break;
			}

			if((m_pProfile->DEVICE.ETCDevice20 == HANA) &&
				(m_pDevCmn->TranResult == TRUE)		&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag))
			{
				nNextTran++;
				continue;
			}

			fnAPD_MaterialOut(DEV_PBM);						// ��ü����	
			fnAPD_MaterialOutCheckRetract(DEV_PBM);			// ��ü���������ȸ��
			if ((m_pDevCmn->TranResult == TRUE)	&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag)	)				// ���ŷ�OK 
			{
				fnAPP_AcceptContinuousPBChoice();
			}

			nNextTran++;
		}

	}
	else
	if(m_pDevCmn->TranResult && (PbTran & MENU_PB) && m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)  //#0125  ��ȯ ��������
	{
		if(m_pProfile->DEVICE.ETCDevice20 == KEB) 
		{
			TranProc  = TRAN_PB;
			TranCode  = TC_PBOOK;
			TranCode2 = TRANID_4730;
			PbNextTranFlag = TRUE;
		}

		while (PbNextTranFlag && nNextTran < 9)							// ��9������ �ۼ����ϹǷ� 3���� ��������
		{
			fnAPP_InitializeSerialTranDS();
			m_pDevCmn->fnAPL_SetProcCount('3');					// ���༳��			
			fnAPP_SendRecvHostSnod();							// �۽�
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				m_pDevCmn->fnAPL_SetProcCount('5');				// ���༳��
				fnAPP_PPPProc();								// ���������Ʈ
			}
			else
			{	
				m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG
				PbNextTranFlag = FALSE;
				break;
			}

			if((m_pProfile->DEVICE.ETCDevice20 == HANA) &&
				(m_pDevCmn->TranResult == TRUE)		&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag))
			{
				nNextTran++;
				continue;
			}

			fnAPD_MaterialOut(DEV_PBM);						// ��ü����	
			fnAPD_MaterialOutCheckRetract(DEV_PBM);			// ��ü���������ȸ��
			if ((m_pDevCmn->TranResult == TRUE)	&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag)	)				// ���ŷ�OK 
			{
				fnAPP_AcceptContinuousPBChoice();
			}

			nNextTran++;
		}
	}

	fnAPD_MaterialOut(DEV_PBM);						// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_PBM);			// ��ü���������ȸ��

	return T_OK;
}

// Withdraw Check Device Action Procedure
int CTranCmn::fnAPP_TranWithCheckDeviceActionProc()
{
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_PBM | DEV_CSH | DEV_UCM);
	return T_OK;
}

// Withdraw Check Menu Continue Procedure
int CTranCmn::fnAPP_TranWithCheckMenuContinueProc()
{
	if (PbTran)
	{
		if (PbNextPageFlag || PbNextTranFlag)				
			MainTranDoingFlag = TRUE;
		else
			MainTranDoingFlag = FALSE;
	}

	
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	Transaction Inquiry Function
/////////////////////////////////////////////////////////////////////////////
// Inquiry Initialize Procedure
int CTranCmn::fnAPP_TranInqInitProc()
{
/*
	if (!MenuFICMode)
	{
		fnAPD_MaterialOut(DEV_MCU);							// Card Out
		fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract
	}
*/

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

	//#N0227
	if((TranCode == TC_SVCINQ)  && !(EMVReadFlg))
	{
		if(CardOnEMV)
			;
		else
			fnAPP_Accpet_EMV_MS_Confirm();
	}

	return T_OK;
}

// Inquiry Input Procedure
int CTranCmn::fnAPP_TranInqInputProc()
{
	if (MainTranDoingCount == 0)
	{
		fnAPP_InitializeSerialTranDS();							// Initialize Serial Transaction DS
		fnAPP_AcceptInq();										//  ��ȸ�Է�
	}
	
	if(m_pDevCmn->IfmDirect)								
	{
		fnAPLLib_EmvTranDataMake(TranCode);
	}
	//#0039
	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)   //#N0181 => #N0214
		fnAPD_PbSpeedUpCheck();


	fnAPD_CardEmbossScan();										// ī�忥������ĵ

	return T_OK;
}

// Inquiry Output Procedure
int CTranCmn::fnAPP_TranInqOutputProc()
{
	if (m_pDevCmn->TranResult)									// HOST OK
	{
		if ((TranCode == TC_SVCINQ) && (EMVReadFlg))											// EMV CHIP READ OK
		{															// EMV HOST ������������
			memset(m_pDevCmn->m_EMVszData, ' ', sizeof(m_pDevCmn->m_EMVszData));
			int nRet;
			nRet = 	fnAPP_EMVAfterOnlineTranProc();
		}

		fnAPP_PMEAcceptInquiryTranServiceConfirm();
	}

	fnAPP_PSPProc(DEV_JPR);    									// ��������Ʈ V01-00-00

	if (!m_pDevCmn->TranResult)
		fnAPP_PMDProc();										// NG ȭ��޼���ǥ��
	
		fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_PBM);
		
	if(ChoiceTran == TRAN_NULL)  //#N0223
	{																	// ���ۿϷ����&��ְ���
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// ��ü����	
		fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM);
	}
	


	return T_OK;
}

// Inquiry Check Device Action Procedure
int CTranCmn::fnAPP_TranInqCheckDeviceActionProc()
{
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_PBM);
	return T_OK;
}

// Inquiry Check Menu Continue Procedure
int CTranCmn::fnAPP_TranInqCheckMenuContinueProc()
{
	MainTranDoingFlag = FALSE;
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	Transaction Transfer Function
/////////////////////////////////////////////////////////////////////////////
// Transfer Initialize Procedure
int CTranCmn::fnAPP_TranTransInitProc()
{
	//#N0189
	if((m_pProfile->DEVICE.ETCDevice20 == KEB) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		fnAPP_Accpet_KEB_Fee_Confirm();
	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == KIUP) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IKIUBANKID) && (TranCode == TC_TRANSFER))
		{
			;
		}
		else
		{
			fnAPP_Accpet_KIUP_Fee_Confirm();
		}
	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == NONGHYUP) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == INONBANKID1 || CardBank == INONBANKID2 || CardBank == INONBANKID3 || CardBank == INONBANKID4)
			&& (TranCode == TC_TRANSFER))
		{
			;
		}
		else
		{
			fnAPP_Accpet_NONGHYUP_Fee_Confirm();
		}

	}
	else
	if ((m_pProfile->DEVICE.ETCDevice20 == KWANGJU) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IKAWBANKID) && (TranCode == TC_TRANSFER))
		{
			;
		}
		else
		{
			fnAPP_Accpet_KWANGJU_Fee_Confirm();
		}

	}
	else
	if ((m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))   //#N0192
	{
		if ((CardBank == IKYOBANKID) && (TranCode == TC_TRANSFER))
		{
			;
		}
		else
		{
			fnAPP_Accpet_KYONGNAM_Fee_Confirm();
		}

	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == HANA) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))		//#N0214 ��/Ÿ ���� ���� ���
	{
		fnAPP_Accpet_HANA_Fee_Confirm();
	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == SHINHAN) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))		//#N0215
	{
		if ((CardBank == ISHIBANKID || CardBank == ISHIBANKID1 || CardBank == ICHOBANKID)
			&& (TranCode == TC_TRANSFER))
		{
			;
		}
		else
		{
			fnAPP_Accpet_SHINHAN_Fee_Confirm();
		}

	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == SC) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))		//#N0219
	{
		if ((CardBank == IKOFBANKID)
			&& (TranCode == TC_TRANSFER))
		{
			;
		}
		else
		{
			fnAPP_Accpet_SC_Fee_Confirm();
		}

	}
	else //#N0226
	if((m_pProfile->DEVICE.ETCDevice20 == KB) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IKUMBANKID1 || CardBank == IKUMBANKID2 || CardBank == IKUMBANKID3 || CardBank == IKUMBANKID4 || CardBank == IKUMBANKID5) && (TranCode == TC_TRANSFER))
		{
			;
		}
		else
		{
			fnAPP_Accpet_KB_Fee_Confirm();
		}
	}
	else //#N0226
	if((m_pProfile->DEVICE.ETCDevice20 == CITI) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IGOOBANKID1 || CardBank == IGOOBANKID2 || CardBank == ICITBANKID) && (TranCode == TC_TRANSFER))
		{
			;
		}
		else
		{
			fnAPP_Accpet_CITI_Fee_Confirm();
		}
	}

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
	
	//#N0227
	if((TranCode == TC_SVCTRAN || TranCode == TC_SVCTRAN1)  && !(EMVReadFlg))
	{
		if(CardOnEMV)
			;
		else
			fnAPP_Accpet_EMV_MS_Confirm();
	}
	
	return T_OK;
}

// Transfer Input Procedure
int CTranCmn::fnAPP_TranTransInputProc()
{
	if((m_pProfile->DEVICE.ETCDevice20) == KWANGJU && (CardService == KWANGSUCARD))
		fnAPP_AcceptKJBTransType();

	if (MainTranDoingCount == 0)
	{
		Accept.SelStockIndex = 0;
		fnAPP_InitializeSerialTranDS();							// Initialize Serial Transaction DS
		fnAPP_AcceptTrans();									// ��ü�Է�
	}
	// EMVLib //#N9999
	if(m_pDevCmn->IfmDirect)								
	{
		fnAPLLib_EmvTranDataMake(TranCode);
	}

	//#0039
	if (PbTran & MENU_PB)
	{
		if(m_pProfile->DEVICE.ETCDevice20 == KEB && m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE) //#0085  //�ٱ��� ����ŷ��� ������Ʈ X
			fnAPP_Accpet_KEB_PbConfirmMSG();
		else
		if((m_pProfile->DEVICE.ETCDevice20 == NONGHYUP) ||  (m_pProfile->DEVICE.ETCDevice20 == KWANGJU))   //#N0181 => #N0214
			fnAPD_PbSpeedUpCheck();
	}

	fnAPD_CardEmbossScan();										// ī�忥������ĵ

	if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		fnAPP_AcceptSlipChoice();
	else
	{
		m_pDevCmn->fnSCR_DisplayScreen(526);
		Delay_Msg(2000);
	}

	return T_OK;
}

// Transfer Output Procedure
int CTranCmn::fnAPP_TranTransOutputProc()
{
	int		nNextTran = 0;
	CString cstCardInfo("000");
	CString cstAcceptInfo("000");
	int     nBankCV1 = 0;
	int     nBankCV2 = 0;


	if (m_pDevCmn->TranResult)									// HOST OK
	{													// (��ü �Ϲݰŷ�)
		if(TranCode == TC_TRANSFER)
		{
			fnAPP_PMEAcceptTransConfirm();							// ��üȮ��
			
			cstAcceptInfo = fnAPP_Get_BankOrgCode(Accept.BankNum);
			cstCardInfo = fnAPP_Get_BankOrgCode(CardBank);
			//#CS0002
			nBankCV1 = fnAPP_CardBank_C(cstAcceptInfo.GetBuffer(0));
			nBankCV2 = fnAPP_CardBank_C(cstCardInfo.GetBuffer(0));
			if (nBankCV1 == nBankCV2)
			{
				TranCode2 = TRANID_4370;
				TranCode2Save = TRANID_4370;
			}
			else
			{	
				TranCode2 = TRANID_4390;
				TranCode2Save = TRANID_4390;
			}

			AddSerialFlg = FALSE;									// �Ϸù�ȣ����Flg
			m_pDevCmn->fnAPL_SetProcCount('3');						// ���༳��
			fnAPP_SendHost();										// �۽�
			fnAPP_RecvHost();										// ����
		}
		else
		if(TranCode == TC_SVCTRAN1 || TranCode == TC_SVCTRAN)   //  #N0181 ���� ���� ��ü �߰�
		{
			if ( (EMVReadFlg)			 &&						// EMV CHIP READ OK
				 (m_pDevCmn->TranResult))
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV ���ŵ���Ÿ ó�� ����");
				//memset(m_pDevCmn->m_EMVszData, ' ', sizeof(m_pDevCmn->m_EMVszData));
					
				int nRet;
				nRet = 	fnAPP_EMVAfterOnlineTranProc();
					
				if (nRet != T_OK)								
				{
					fnAPP_CancelProc( T_MSG, "ī�� ���� ����! �ŷ��� ����մϴ�");
				}
			}
			if(TranResultNGCode == TRAN_MSG_EXCEPTION_700)
			{
				AddSerialFlg = FALSE;
				TranCode2Save = TRANID_4350;
				TranCode2 = TRANID_4350;			
			}
			else
			{
				fnAPP_PMEAcceptTransConfirm();							// ��üȮ��
				fnAPP_AcceptPassword();
				TranCode2 = TRANID_4350;
				TranCode2Save = TRANID_4350;
			}
			
			TranResultNGCode = 0;
			AddSerialFlg = FALSE;									// �Ϸù�ȣ����Flg
			m_pDevCmn->fnAPL_SetProcCount('3');						// ���༳��
		
			fnAPP_SendHost();										// �۽�
			fnAPP_RecvHost();										// ����
			/*
			if ( (EMVReadFlg)			 &&						// EMV CHIP READ OK
				 (m_pDevCmn->TranResult))
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV ���ŵ���Ÿ ó�� ����");
				memset(m_pDevCmn->m_EMVszData, ' ', sizeof(m_pDevCmn->m_EMVszData));
					
				int nRet;
				nRet = 	fnAPP_EMVAfterOnlineTranProc();
					
				if (nRet != T_OK)								
				{
						fnAPP_CancelProc( T_MSG, "ī�� ���� ����! �ŷ��� ����մϴ�");
				}
			}
			*/
		}

		if (m_pDevCmn->TranResult)								// HOST OK
			m_pDevCmn->fnAPL_SetProcCount('5');					// ���༳��
		else m_pDevCmn->fnAPL_SetProcCount('9');					// HOST NG
	}

	if ((m_pDevCmn->TranResult) && (TranProc == TRAN_TRANS))			// HOST OK
	{
		fnAPP_SumProc(1);											// ��ü�հ������Ʈ
	}

	fnAPP_PSPProc(DEV_JPR);    								// ��������Ʈ
	fnAPP_PMDProc();												// �޼���ǥ��
	fnAPP_PSPProc(DEV_SPR);    										// ������Ʈ
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_PBM);	
/*
	if (m_pDevCmn->TranResult)	//#N0181  => ��ü�� ��ǥ ���� �� ���� ����.
	{
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU );
		fnAPD_MaterialOutCheckRetract(DEV_SPR |DEV_MCU);
	}
*/
	//#0039
	if((m_pProfile->DEVICE.ETCDevice20 == NONGHYUP) && (m_pDevCmn->TranResult) && (PbTran & MENU_PB) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		TranCode2 = TRANID_7330;
		TranCode2Save = TRANID_7330;
		memset(Accept.Money, '0', sizeof(Accept.Money));			// �ݾ�(����), �ݾ�(��õ��)
		AddSerialFlg = FALSE;			
		fnAPP_SendHost();										// �۽�
		fnAPP_RecvHost();										// ����
		if (m_pDevCmn->TranResult)								// HOST OK
		{
			AddSerialFlg = FALSE;		
			TranCode2 = TRANID_4730;
			TranCode2Save = TRANID_4730;
		}
		else
		{
			fnAPP_CancelProc(T_MSG, "�� �������δ� ���ŷ��� �� �� �����ϴ�");
		}
	}


	if (m_pDevCmn->TranResult && (PbTran & MENU_PB) && m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
	{
		fnAPP_AcceptTranPBProc();
		while (PbNextTranFlag && nNextTran < 9)							// ��9������ �ۼ����ϹǷ� 3���� ��������
		{
			fnAPP_InitializeSerialTranDS();
			m_pDevCmn->fnAPL_SetProcCount('3');					// ���༳��			
			fnAPP_SendRecvHostSnod();							// �۽�
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				m_pDevCmn->fnAPL_SetProcCount('5');				// ���༳��
				fnAPP_PPPProc();								// ���������Ʈ
			}
			else
			{	
				m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG
				PbNextTranFlag = FALSE;
				break;
			}

			if((m_pProfile->DEVICE.ETCDevice20 == HANA) &&
				(m_pDevCmn->TranResult == TRUE)		&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag))
			{
				nNextTran++;
				continue;
			}

			fnAPD_MaterialOut(DEV_PBM);						// ��ü����	
			fnAPD_MaterialOutCheckRetract(DEV_PBM);			// ��ü���������ȸ��
			if ((m_pDevCmn->TranResult == TRUE)	&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag)	)				// ���ŷ�OK 
			{
				fnAPP_AcceptContinuousPBChoice();
			}

			nNextTran++;
		}

	}	
	else
	if(m_pDevCmn->TranResult && (PbTran & MENU_PB) && m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)  //#0125  ��ȯ ��������
	{
		if(m_pProfile->DEVICE.ETCDevice20 == KEB) 
		{
			TranProc  = TRAN_PB;
			TranCode  = TC_PBOOK;
			TranCode2 = TRANID_4730;
			PbNextTranFlag = TRUE;
		}

		while (PbNextTranFlag && nNextTran < 9)							// ��9������ �ۼ����ϹǷ� 3���� ��������
		{
			fnAPP_InitializeSerialTranDS();
			m_pDevCmn->fnAPL_SetProcCount('3');					// ���༳��			
			fnAPP_SendRecvHostSnod();							// �۽�
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				m_pDevCmn->fnAPL_SetProcCount('5');				// ���༳��
				fnAPP_PPPProc();								// ���������Ʈ
			}
			else
			{	
				m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG
				PbNextTranFlag = FALSE;
				break;
			}

			if((m_pProfile->DEVICE.ETCDevice20 == HANA) &&
				(m_pDevCmn->TranResult == TRUE)		&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag))
			{
				nNextTran++;
				continue;
			}

			fnAPD_MaterialOut(DEV_PBM);						// ��ü����	
			fnAPD_MaterialOutCheckRetract(DEV_PBM);			// ��ü���������ȸ��
			if ((m_pDevCmn->TranResult == TRUE)	&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag)	)				// ���ŷ�OK 
			{
				fnAPP_AcceptContinuousPBChoice();
			}

			nNextTran++;
		}
	}

	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);					// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_SPR |DEV_MCU | DEV_PBM);

	return T_OK;
}

// Transfer Check Device Action Procedure
int CTranCmn::fnAPP_TranTransCheckDeviceActionProc()
{
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);
	return T_OK;
}

// Transfer Check Menu Continue Procedure
int CTranCmn::fnAPP_TranTransCheckMenuContinueProc()
{
	if (PbTran)
	{
		if (PbNextPageFlag || PbNextTranFlag)					//2���� ���°� �´��� Ȯ����.
			MainTranDoingFlag = TRUE;
		else
			MainTranDoingFlag = FALSE;
	}


	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	Transaction Deposit Function
/////////////////////////////////////////////////////////////////////////////
// Deposit Initialize Procedure
int CTranCmn::fnAPP_TranDepInitProc()
{
/*
	if (!MenuFICMode)
	{
		fnAPD_MaterialOut(DEV_MCU);							// Card Out
		fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract
	}
*/

	if((m_pProfile->DEVICE.ETCDevice20 == KEB) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		fnAPP_Accpet_KEB_Fee_Confirm();
	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == KIUP) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IKIUBANKID) && (TranCode == TC_DEPOSIT))		// ������ุ ���Ӵ� ��� ����.
		{
			;
		}
		else
		{
			fnAPP_Accpet_KIUP_Fee_Confirm();
		}
	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == NONGHYUP) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == INONBANKID1 || CardBank == INONBANKID2 || CardBank == INONBANKID3 || CardBank == INONBANKID4)
			&& (TranCode == TC_DEPOSIT))
		{
			;
		}
		else
		{
			fnAPP_Accpet_NONGHYUP_Fee_Confirm();
		}

	}
	else
	if ((m_pProfile->DEVICE.ETCDevice20 == KWANGJU) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IKAWBANKID) && (TranCode == TC_DEPOSIT))
		{
			;
		}
		else
		{
			fnAPP_Accpet_KWANGJU_Fee_Confirm();
		}

	}
	else
	if ((m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))   //#N0192
	{
		if ((CardBank == IKYOBANKID) && (TranCode == TC_DEPOSIT))
		{
			;
		}
		else
		{
			fnAPP_Accpet_KYONGNAM_Fee_Confirm();
		}

	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == HANA) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))  //#N0214 �ϳ����� ���� Ÿ�� ���� ���� ���
	{
		fnAPP_Accpet_HANA_Fee_Confirm();
	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == SHINHAN) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))  //#N0214
	{
		if ((CardBank == ISHIBANKID || CardBank == ISHIBANKID1 || CardBank == ICHOBANKID)
			&& (TranCode == TC_DEPOSIT))
		{
			;
		}
		else
		{
			fnAPP_Accpet_SHINHAN_Fee_Confirm();
		}

	}
	else
	if((m_pProfile->DEVICE.ETCDevice20 == SC) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))  //#N0219
	{
		if ((CardBank == IKOFBANKID)
			&& (TranCode == TC_DEPOSIT))
		{
			;
		}
		else
		{
			fnAPP_Accpet_SC_Fee_Confirm();
		}

	}
	else //#N0226
	if((m_pProfile->DEVICE.ETCDevice20 == KB) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IKUMBANKID1 || CardBank == IKUMBANKID2 || CardBank == IKUMBANKID3 || CardBank == IKUMBANKID4 || CardBank == IKUMBANKID5) && (TranCode == TC_DEPOSIT))
		{
			;
		}
		else
		{
			fnAPP_Accpet_KB_Fee_Confirm();
		}
	}
	else //#N0238
	if((m_pProfile->DEVICE.ETCDevice20 == CITI) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		if ((CardBank == IGOOBANKID1 || CardBank == IGOOBANKID2 || CardBank == ICITBANKID) && (TranCode == TC_DEPOSIT))
		{
			;
		}
		else
		{
			fnAPP_Accpet_CITI_Fee_Confirm();
		}
	}


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

// Deposit Input Procedure
int CTranCmn::fnAPP_TranDepInputProc()
{

	if (PbTran & MENU_PB)
	{
		//#0039 -> #0082
		if(m_pProfile->DEVICE.ETCDevice20 == KEB && m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE) //#0085 �ٱ��� ����ŷ��� ������Ʈ X
			fnAPP_Accpet_KEB_PbConfirmMSG();
		else
		if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)   //#N0181 => #N0214
			fnAPD_PbSpeedUpCheck();
	}

	
	if (MainTranDoingCount == 0)
	{
		fnAPP_InitializeSerialTranDS();							// Initialize Serial Transaction DS
		if((m_pProfile->DEVICE.ETCDevice20 == KB && (CardTran & MENU_J3)) || (m_pProfile->DEVICE.ETCDevice20 == KB && (PbTran & MENU_PB)))				
			fnAPP_AcceptDep_KB();
		else
			fnAPP_AcceptDep();										// �Ա��Է�
	}


	fnAPD_CardEmbossScan();										// ī�忥������ĵ
	m_pDevCmn->fnAPL_CaptureHandSave(5, FALSE, fnAPP_GetCaptureData());		// ���Կ�
	m_pDevCmn->fnAPL_CaptureFaceSave(6, FALSE, fnAPP_GetCaptureData());		// ���Կ�	
	return T_OK;
}

// Deposit Output Procedure
int CTranCmn::fnAPP_TranDepOutputProc()
{
	char	szTempBuff[256] = {0,};
	CString cstrDISP("");
	int		nNextTran = 0;
	int		nNumTotalAmount = 0, nNumTotal = 0, nNum10000 = 0, nNum5000 = 0, nNum1000 = 0, nNum50000 = 0, nUnKnown =0;	
	if(m_pDevCmn->TranResult)									// HOST OK
	{

		fnAPP_SumProc(2);										// �Ա�Host

		if(m_pProfile->DEVICE.MachineType == U8100)
		{
			//5����
			nNum50000 = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0);
			if (nNum50000 < 0) nNum50000 = 0;
			nNum50000 += CashData.mFiftyThousand;
			RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", nNum50000);
							
			nNum10000 = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
			if (nNum10000 < 0) nNum10000 = 0;
			nNum10000 += CashData.mTenThousand;
			RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", nNum10000);
			
			nNum5000 = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_5000", 0);
			if (nNum5000 < 0) nNum5000 = 0;
			nNum5000 += CashData.mFiveThousand;
			RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_5000", nNum5000);

			nNum1000 = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_1000", 0);
			if (nNum1000 < 0) nNum1000 = 0;
			nNum1000 += CashData.mOneThousand;
			RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_1000", nNum1000); //#0088
		}
		else
		if(m_pProfile->DEVICE.MachineType == U3100K)
		{
			nNum5000 = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_5000", 0);
			if (nNum5000 < 0) nNum5000 = 0;
			nNum5000 += CashData.mFiveThousand;
			RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_5000", nNum5000);

			nNum1000 = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_1000", 0);
			if (nNum1000 < 0) nNum1000 = 0;
			nNum1000 += CashData.mOneThousand;
			RegSetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_1000", nNum1000); //#0088

		}



		fnAPD_CheckDepositStore();								// �Աݼ�ǥ����
		
		RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP", 2);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�Աݼ��� : ����"));
		fnAPD_CashDepositStore(TRUE);						// �Ա����ݼ���
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�Աݼ��� : �Ϸ�"));
		RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP", 0);

	}
	else
	{
		cstrDISP.Format("�����ڵ�: %03d", TranResultNGCode);
		fnAPP_CancelProc(T_MSG, cstrDISP.GetBuffer(0), "�Ͻ����� ȸ����ַ� ó�����", "���θ� Ȯ�� �Ͽ� �ֽʽÿ�"); //#0023
	}

	CancelDepSendFlg = TRUE;									// ����Ա�ó��Flag Reset
																// �Ա���Ҵ� �ŷ�����ñ��� ����


	fnAPP_PMDProc();										// �޼���ǥ��
	fnAPP_PSPProc(DEV_JPR);    								// ��������Ʈ


	if(Accept.SlipChoiceFlag)
		fnAPP_PSPProc(DEV_SPR);    								// ������Ʈ

	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_BRM | DEV_UCM | DEV_PBM);	

	
	if (m_pDevCmn->TranResult)									// HOST OK
	{
		CashInCountFlg = FALSE;  //#0049 -> �Ա� ��ǥ ���� �� 	
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU );
		fnAPD_MaterialOutCheckRetract(DEV_SPR |DEV_MCU);
	}
	else
	{															// ��ü����	
		fnAPD_MoneyOutCheck();									// ���ݼ�ǥ���������
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);			// ��ü����
		fnAPD_MaterialOutCheckRetract(DEV_SPR |DEV_MCU | DEV_PBM);
	}

	m_pDevCmn->fnAPL_CaptureSaveEnd();				
//	sprintf(szTempBuff, "[����:%5.5d�� ��ǥ:%3.3d��]", m_pDevCmn->fnAPL_GetNumberOfCash(), m_pDevCmn->fnAPL_GetNumberOfOutCheck());
	sprintf(szTempBuff, "��    �� : %5.5d��", m_pDevCmn->fnAPL_GetNumberOfCash());
	m_pDevCmn->fnAPL_PrintJnlInfo(szTempBuff, JNL_STATUS);
	sprintf(szTempBuff, "�ԱݰǼ� : %04d - �Աݱݾ� : %08d", Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Count,10), Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Amount, 10));	
	m_pDevCmn->fnAPL_PrintJnlInfo(szTempBuff, JNL_STATUS);	

	//#0039
	if((m_pProfile->DEVICE.ETCDevice20 == NONGHYUP) && (m_pDevCmn->TranResult) && (PbTran & MENU_PB) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
	{
		TranCode2 = TRANID_7330;
		TranCode2Save = TRANID_7330;
		memset(Accept.Money, '0', sizeof(Accept.Money));			// �ݾ�(����), �ݾ�(��õ��)
		AddSerialFlg = FALSE;			
		fnAPP_SendHost();										// �۽�
		fnAPP_RecvHost();										// ����
		if (m_pDevCmn->TranResult)								// HOST OK
		{
			AddSerialFlg = FALSE;		
			TranCode2 = TRANID_4730;
			TranCode2Save = TRANID_4730;
		}
		else
		{
			fnAPP_CancelProc(T_MSG, "�� �������δ� ���ŷ��� �� �� �����ϴ�");
		}
	}

	if((m_pProfile->DEVICE.ETCDevice20 == KB) &&
	   (PbTran & MENU_PB) &&
	   (m_pDevCmn->TranResult == TRUE))	
	{
		int nRlen =0;

		char szPbm_P_N[2+1]; //����������󿩺�
		char szPbm_P_F[4+1]; //���� �ڷ� �߰� ���� ����
		char szPbm_S_L[2+1]; //���� ���� ��
		char szPbm_S_C[2+1]; //���� ��� �Ǽ�
		char szPbm_P_M[2+1]; //�����̿� ��� �ȳ�

		memset(szPbm_P_N, 0x00, sizeof(szPbm_P_N));
	
		nRlen += 1;												//���ʰŷ� ���� (�̻��)
		memcpy(szPbm_P_N, &m_RD.byDataField_Dummy[nRlen] , 1);  //�������� ��� ���� ("0" : �������� ����, "1" : �������� ����)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: ���� ���� ��� ����     (%1.1s)", szPbm_P_N);
		nRlen += 1;   

		memcpy(szPbm_P_F, &m_RD.byDataField_Dummy[nRlen] , 1);  //���� �ڷ� �߰� ���� ���� ("0" : �߰��ڷ� ����, "1' : �߰��ڷ� ����) 
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: ���� �ڷ� �߰� ���� ����(%1.1s)", szPbm_P_F);
		nRlen += 1;                                 //�߰��ڷ� ����

		memcpy(szPbm_P_M, &m_RD.byDataField_Dummy[nRlen] , 1);  //�����̿� ��� �ȳ�  (����ŷ� ȭ�� �ȳ� �޽���)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: �����̿� ��� �ȳ�      (%1.1s)", szPbm_P_M);
		nRlen += 1;   

		nRlen += 1;                                 //�����ܾ� ���ڿ��� (�̻��)

		nRlen += 2;                                 //���� ���� �� (�̻��)
		
		memcpy(szPbm_S_L, &m_RD.byDataField_Dummy[nRlen] , 3); //���� ���� �� (Pbm Start Line)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: ���� ���� ��            (%3.3s)", szPbm_S_L);
		nRlen += 3; 

		memcpy(szPbm_S_C, &m_RD.byDataField_Dummy[nRlen], 2);  //���� ��� �Ǽ�
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: ���� ��� �Ǽ�          (%2.2s)", szPbm_S_C);
		nRlen += 2;

		if (m_pDevCmn->TranResult)									// HOST OK
		{
			if(memcmp(szPbm_P_N, "0", 1) == 0) //�������� ��� ���� -> "0" : �������� ����, "1" : �������� ����
			{
				Accept.Index = 0;
				PbNextTranFlag = FALSE;
				m_pDevCmn->fnSCR_DisplayString(1, "���� ������ ������ �����ϴ�");
				m_pDevCmn->fnSCR_DisplayString(3, "�̿��� �ּż� �����մϴ�.");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�������� �������"));
				Delay_Msg(3000);
			}
			else
			if (memcmp(szPbm_P_M, "1", 1) == 0) //����ŷ� ȭ�� �ȳ� �޽��� -> "1" : ���� �̿� ��� �ȳ�
			{
				Accept.Index = 0;
				PbNextTranFlag = FALSE;
				m_pDevCmn->fnSCR_DisplayString(1, "�� �������δ� ���̻� ���� ������ �� �� �����ϴ� ");
				m_pDevCmn->fnSCR_DisplayString(2, "������ ������ �湮�Ͻþ�");
				m_pDevCmn->fnSCR_DisplayString(3, "������ ���� �߱� �����ñ� �ٶ��ϴ�.");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�����̿����"));
				Delay_Msg(3000);
			}
			else 
			{
				if (memcmp(szPbm_P_F, "1", 1) == 0) //�������� �߰� ���� ���� -> "0" : �߰��ڷ� ����, "1' : �߰��ڷ� ����
				{
					PbNextTranFlag = TRUE;			//#H0227 -> �������� �߰� ���� ����
					Accept.Index = 99;
				}
				else
				{
					PbNextTranFlag = FALSE;	
					Accept.Index = 0;
				}

				if (memcmp(m_SRXEncBuff.byPlaintMsdata, "XXXXXXXXXXXXXX",14) == 0)
				{
						PbNextTranFlag = FALSE;
						m_pDevCmn->fnSCR_DisplayString(1, "�˼��մϴ� ���������� �Ϸ���� �ʾҽ��ϴ�");
						m_pDevCmn->fnSCR_DisplayString(2, "������ ������ �湮�Ͻþ�");
						m_pDevCmn->fnSCR_DisplayString(3, "�������� ������ Ȯ���Ͻñ� �ٶ��ϴ�.");
						m_pDevCmn->fnSCR_DisplayScreen(701);
						Delay_Msg(2000);
						Accept.Index = 0;
				}
				else
				if (memcmp(szPbm_S_C, "00", 2) == 0) //�������� ��� �Ǽ� -> MAX : 45
				{
					PbNextTranFlag = FALSE;
					m_pDevCmn->fnSCR_DisplayString(1, "���� ������ ������ �����ϴ�");
					m_pDevCmn->fnSCR_DisplayString(3, "�̿��� �ּż� �����մϴ�.");
					m_pDevCmn->fnSCR_DisplayScreen(701);
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�������� �������"));
					Delay_Msg(2000);
				}
				else
				{
					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
					m_pDevCmn->fnSCR_DisplayScreen(701);

					fnAPP_PPPProc();										// ���������Ʈ
				}					
			}

			PbNextTranFlag = FALSE;
		}
	}
	else
	if (m_pDevCmn->TranResult && (PbTran & MENU_PB) && m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
	{
		fnAPP_AcceptTranPBProc();
		while (PbNextTranFlag && nNextTran < 9)							// ��9������ �ۼ����ϹǷ� 3���� ��������
		{
			fnAPP_InitializeSerialTranDS();
			m_pDevCmn->fnAPL_SetProcCount('3');					// ���༳��			
			fnAPP_SendRecvHostSnod();							// �۽�
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				m_pDevCmn->fnAPL_SetProcCount('5');				// ���༳��
				fnAPP_PPPProc();								// ���������Ʈ
			}
			else
			{	
				m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG
				PbNextTranFlag = FALSE;
				break;
			}

			if((m_pProfile->DEVICE.ETCDevice20 == HANA) &&
				(m_pDevCmn->TranResult == TRUE)		&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag))
			{
				nNextTran++;
				continue;
			}

			fnAPD_MaterialOut(DEV_PBM);						// ��ü����	
			fnAPD_MaterialOutCheckRetract(DEV_PBM);			// ��ü���������ȸ��
			if ((m_pDevCmn->TranResult == TRUE)	&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag)	)				// ���ŷ�OK 
			{
				fnAPP_AcceptContinuousPBChoice();
			}

			nNextTran++;
		}

	}	
	else
	if(m_pDevCmn->TranResult && (PbTran & MENU_PB) && m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)  //#0125  ��ȯ ��������
	{
		if(m_pProfile->DEVICE.ETCDevice20 == KEB) 
		{
			TranProc  = TRAN_PB;
			TranCode  = TC_PBOOK;
			TranCode2 = TRANID_4730;
			PbNextTranFlag = TRUE;
		}

		while (PbNextTranFlag && nNextTran < 9)							// ��9������ �ۼ����ϹǷ� 3���� ��������
		{
			fnAPP_InitializeSerialTranDS();
			m_pDevCmn->fnAPL_SetProcCount('3');					// ���༳��			
			fnAPP_SendRecvHostSnod();							// �۽�
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				m_pDevCmn->fnAPL_SetProcCount('5');				// ���༳��
				fnAPP_PPPProc();								// ���������Ʈ
			}
			else
			{	
				m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG
				PbNextTranFlag = FALSE;
				break;
			}

			if((m_pProfile->DEVICE.ETCDevice20 == HANA) &&
				(m_pDevCmn->TranResult == TRUE)		&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag))
			{
				nNextTran++;
				continue;
			}

			fnAPD_MaterialOut(DEV_PBM);						// ��ü����	
			fnAPD_MaterialOutCheckRetract(DEV_PBM);			// ��ü���������ȸ��
			if ((m_pDevCmn->TranResult == TRUE)	&&					// ���忬��ó��
				(PbNextPageFlag || PbNextTranFlag)	)				// ���ŷ�OK 
			{
				fnAPP_AcceptContinuousPBChoice();
			}

			nNextTran++;
		}
	}
	
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);					// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_SPR |DEV_MCU | DEV_PBM);

	return T_OK;
}

int CTranCmn::fnAPP_TranDepCardMuAllinceProc()
{
	return T_OK;

}


// Deposit Check Device Action Procedure
int CTranCmn::fnAPP_TranDepCheckDeviceActionProc()
{
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_PBM | DEV_CSH | DEV_UCM);
	return T_OK;
}

// Deposit Check Menu Continue Procedure
int CTranCmn::fnAPP_TranDepCheckMenuContinueProc()
{
	if (PbTran)
	{
		if (PbNextPageFlag || PbNextTranFlag)				
			MainTranDoingFlag = TRUE;
		else
			MainTranDoingFlag = FALSE;
	}

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	Transaction Passbook Function
/////////////////////////////////////////////////////////////////////////////
// Passbook Initialize Procedure
int CTranCmn::fnAPP_TranPbInitProc()
{
	return T_OK;
}

// Passbook Input Procedure
int CTranCmn::fnAPP_TranPbInputProc()
{

	if(m_pProfile->DEVICE.ETCDevice20 == KEB) //#0085 -> #0125
	{
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_Accpet_KEB_PbConfirmMSG2();

			if (TranCode == TC_PBOOKTRM)
				fnAPP_Accpet_KEB_PbConfirmMSG1();
		}
		else
		{
			TranCode = TC_PBOOK;
		}
	}
	
	
	if (MainTranDoingCount == 0)
		fnAPP_InitializeSerialTranDS();							// Initialize Serial Transaction DS

	//#0039
	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
	{
		fnAPD_PbSpeedUpCheck();
		memset(Accept.Money, '0', sizeof(Accept.Money));			// �ݾ�(����), �ݾ�(��õ��)
		fnAPP_SendHost();										// �۽�
		fnAPP_RecvHost();										// ����
		if (m_pDevCmn->TranResult)								// HOST OK
		{
			fnAPP_AcceptTranPBProc();
			AddSerialFlg = FALSE;		
			TranCode2 = TRANID_4730;
			TranCode2Save = TRANID_4730;
		}
		else
		{
			fnAPP_CancelProc(T_MSG, "�� �������δ� ���ŷ��� �� �� �����ϴ�");
		}
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KEB) //#0085
	{
		m_pDevCmn->fnSCR_DisplayScreen(805);			// �б�(����)
		Delay_Msg(1000);		
	}
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU)		//#N0181
	{
		fnAPD_PbSpeedUpCheck();
		m_pDevCmn->fnSCR_DisplayScreen(805);			// �б�(����)
		Delay_Msg(1000);		
	}

	
	return T_OK;
}

// Passbook Output Procedure
int CTranCmn::fnAPP_TranPbOutputProc()
{
	fnAPP_PSPProc(DEV_JPR);    									// ��������Ʈ
	fnAPP_PMDProc();											// �޼���ǥ��
	fnAPD_CheckDeviceAction(DEV_JPR | DEV_PBM);		// ���ۿϷ����&��ְ���

	int i = 0;
	if (m_pDevCmn->TranResult)									// HOST OK
	{
		AddSerialFlg = FALSE;									// �Ϸù�ȣ����Flg
		fnAPP_PPPProc();										// ���������Ʈ
	}

	if((m_pProfile->DEVICE.ETCDevice20 == HANA) &&
		(m_pDevCmn->TranResult == TRUE)		&&					// ���忬��ó��
		(PbNextPageFlag || PbNextTranFlag))
		return T_OK;
	else
	if((m_pProfile->DEVICE.ETCDevice20 == SC) &&
		(m_pDevCmn->TranResult == TRUE)		&&					// ���忬��ó�� #N0245
		(PbNextPageFlag || PbNextTranFlag))
	{
		if (fnAPD_PBBarCodeRead())
		{												// ���ڵ尡 ����, BarCode Read ����
			AddSerialFlg = FALSE;						// �����ŷ��� �Ϸù�ȣ ������� �Ѵ�
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�������� PAGE �̿��� PB BarCode Read ���� !!! ");
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�������� PAGE �̿��� fnAPD_PBBarCodeRead Return 1 !!!");
			return T_OK;
		}
		else 
		{												// ���ڵ尡 ������ 
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PB BarCode Read Fail::Init PbNextTranFlag !!!");
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�������� PAGE �̿��� PB BarCode Read Fail !!!");
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�������� PAGE �̿��� fnAPD_PBBarCodeRead Return 0 !!!");
			m_pDevCmn->fnSCR_DisplayString(2, "���忡 ������ ���� �����ϴ�.");
			m_pDevCmn->fnSCR_DisplayString(3, "â������ ������ ��߱� �����ʽÿ�.");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(3000);
			PbNextTranFlag = FALSE;						// �����̿� �Ұ��� ��������ŷ��뺸Flag �ʱ�ȭ
		}
	}

	fnAPD_MaterialOut(DEV_SPR | DEV_PBM);						// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_PBM);			// ��ü���������ȸ��

	if ((m_pDevCmn->TranResult == TRUE)		&&					// ���忬��ó��
		(PbNextPageFlag || PbNextTranFlag)	)					// ���ŷ�OK + �����ŷ�(�ܰ�����)/����������(��Ÿ) Flag Set��
	{
		fnAPP_AcceptContinuousPBChoice();
	}

	return T_OK;
}

// Passbook Check Device Action Procedure
int CTranCmn::fnAPP_TranPbCheckDeviceActionProc()
{
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_PBM);
	return T_OK;
}

// Passbook Check Menu Continue Procedure
int CTranCmn::fnAPP_TranPbCheckMenuContinueProc()
{
	if (PbTran)
	{
		if (PbNextPageFlag || PbNextTranFlag)					// ������, 2���� ���°� �´��� Ȯ����.
			MainTranDoingFlag = TRUE;
		else
			MainTranDoingFlag = FALSE;
	}

	return T_OK;
}

