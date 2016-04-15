/////////////////////////////////////////////////////////////////////////////
// TranOutput.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	�����Լ�
/////////////////////////////////////////////////////////////////////////////
// ����Աݹ�ȯ�Լ�(�������ó��)
int	CTranCmn::fnAPP_CancelDepRejectProc(int CancelCode, char *pCancelMsg, char *pCancelMsg2, char *pCancelMsg3, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelDepRejectProc:"	\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlg[%d], "		\
													"m_pDevCmn->TranStatus[%d]",
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlg,
													 m_pDevCmn->TranStatus);




/////////////////////////////////////////////////////////////////////////////
//	�Աݹ�ȯó��
/////////////////////////////////////////////////////////////////////////////
	if (!CancelDepRejectFlg)									// �Աݹ�ȯFlg
	{
		CancelDepRejectFlg = TRUE;	
		if (((TranProc == TRAN_DEP) || (TranCode == TC_HIPASS_CASH))	&&			//#N0270				
			(m_pDevCmn->fnAPL_CheckError()))					// ��ְ�����
		{
			CancelDepRejectProcFlg = TRUE;						// �Աݹ�ȯó��Flg
			// ���ݼ�ǥ������ȯ�������ڵ��κ�
			fnAPD_CheckDepositReject();							// �Աݼ�ǥ��ȯ
			fnAPD_CashDepositReject();							// �Ա����ݹ�ȯ
			fnAPD_MoneyOutCheck();								// ���ݼ�ǥ���������
		}
	}
	
	fnAPP_CancelProc(CancelCode, pCancelMsg, pCancelMsg2, pCancelMsg3, CancelScrNum);
	return T_CANCEL;
}

// ����Լ�
int	CTranCmn::fnAPP_CancelProc(int CancelCode, char *pCancelMsg, char *pCancelMsg2, char *pCancelMsg3, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc:"				\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlg[%d], "		\
													"m_pDevCmn->TranStatus[%d],"	\
													"CancelScrNum[%d]",	
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlg,
													 m_pDevCmn->TranStatus,
													 CancelScrNum);

	int		DelaySec = 1;
	int		nBrmStepErr = DEV_NONE;
	int		nDeviceStatusSave = DEV_NONE;
	
	if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KEB_MODE)  // Cancel Msg �ѱ� ǥ�� #N0174
		m_pDevCmn->fnSCR_SetCurrentLangMode(KOR_MODE);

/////////////////////////////////////////////////////////////////////////////
//	Mainȣ���
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->TranStatus == TRAN_IDLE)						// Return�ܰ���?
		return T_CANCEL;

/* //#0002
	if (m_pDevCmn->TranStatus == TRAN_STATUS)					// Return�ܰ���?
		return T_CANCEL;
*/
/////////////////////////////////////////////////////////////////////////////
//	�⺻����ó��
/////////////////////////////////////////////////////////////////////////////
	if (CancelProcNum == 0)										// ù����ø�
	{
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);			// ���ۿϷ�ó�� : 2003.11.06
		m_pDevCmn->fnCSH_CloseShutter();						// �ݱ�
/******************************************************************************************/
		if(m_pProfile->DEVICE.MachineType == U3100K) //#N0138
		{
			//����ó�� -> #N0138
			nDeviceStatusSave = m_pDevCmn->DeviceStatus;
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);			// ���ۿϷ�ó��  //-> �ѳ�Ʈ Ư�� ��� ����
			
			if (m_pDevCmn->DeviceStatus != nDeviceStatusSave)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc - Old nDeviceStatusSave (%x), m_pDevCmn->DeviceStatus (%x)", nDeviceStatusSave, m_pDevCmn->DeviceStatus);		
				m_pDevCmn->DeviceStatus = nDeviceStatusSave;
			}

			if (!m_pDevCmn->fnBRM_GetMaterialInfo() & BRM_IN_CSM)
			{
				if (m_pDevCmn->BrmCsmIn & BRM_IN_CSM)
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc - Old BrmCsmIn (%d), fnBRM_GetMaterialInfo (%d)", m_pDevCmn->BrmCsmIn, m_pDevCmn->fnBRM_GetMaterialInfo());		
					CancelWithSendFlg = TRUE;
				}
			}
			m_pDevCmn->BrmCsmIn = FALSE;
		}
/******************************************************************************************/

		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_UCM);			// ���ۿϷ�ó��
		m_pDevCmn->fnUCM_CloseShutter();						// �ݱ�
		m_pDevCmn->fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM, DISABLE);	// ��ġ���Ը�弳�� 
		m_pDevCmn->fnLGT_SetFlicker(KU_ALL_FLICKER, FLICKER_OFF);		// Flicker����(�ε���,��) 
	}

/////////////////////////////////////////////////////////////////////////////
// ����/��ǥ�� �ܷ����� TRACEó��
/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_FREE_MODE, "Log", __FILE__, __LINE__, "CANCELó���� �ܷ�Ȯ��[%2.2X:%2.2X:%2.2X]", 
													m_pDevCmn->fnCSH_GetMaterialInfo(), 
													m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT),
													m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW));
/////////////////////////////////////////////////////////////////////////////
//	���ŷ���ȣ���ó��
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->TranStatus != TRAN_TRAN)			// �ŷ�ó���ƴ�
	{

		if ((!m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_MAIN)) &&// ����ϰ���ֳ���ġ���ϱ�
			(!m_pDevCmn->fnAPL_CheckError()))					// ��ְ�����
			m_pDevCmn->fnAPL_ClearError();						// ��������ʱ�ȭ
	
		
		if (m_pDevCmn->TranStatus == TRAN_OPEN)
		{
			if (pCancelMsg)
				m_pDevCmn->fnSCR_DisplayStringArray(1, pCancelMsg,  "\n");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(DelaySec * 5000);

MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnSCR_DisplayStringArray1[%s]", pCancelMsg);
		}

		fnAPP_TerminateProc(T_CANCEL);							// �����Լ�
	}

	
	if (m_pDevCmn->TranStatus == TRAN_TRAN)			// �ŷ�ó���ƴ�
	{
		
		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) == DEV_CSH)
		{
			if(TranProc == TRAN_WITH)
			{
				nBrmStepErr = RegGetInt(_REGKEY_DEVICE, "BRM_WITH_STEP",0);
				if(nBrmStepErr == 1)
					fnAPP_WithErrSumProc(1); //������
				else
				if(nBrmStepErr == 3)
					fnAPP_WithErrSumProc(2); //�������

				RegSetInt(_REGKEY_DEVICE, "BRM_WITH_STEP",0);
			}
			else
			if(TranProc == TRAN_DEP)
			{
				nBrmStepErr = RegGetInt(_REGKEY_DEVICE, "BRM_DEP_STEP",0);
				if(nBrmStepErr == 1)
					fnAPP_DepErrSumProc(1); //�Աݰ�����
				else
				if(nBrmStepErr == 2)
					fnAPP_DepErrSumProc(2); //�Աݼ������

				RegSetInt(_REGKEY_DEVICE, "BRM_DEP_STEP",0);
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	����������������
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_CheckError();								// ��ְ���
/////////////////////////////////////////////////////////////////////////////
//	���ó��Ƚ������
/////////////////////////////////////////////////////////////////////////////
	switch (CancelProcNum)										// ����Լ�N�����ຯ��
	{
		case	0:
			CancelProcNum = 1;
			CancelProcCount = m_pProfile->TRANS.ProcCount[0];	// ���ó���������ȣ 
			break;

		case	1:
			CancelProcNum = 2;
			fnAPP_CancelProc2(CancelCode, pCancelMsg, pCancelMsg2, pCancelMsg3, CancelScrNum);	
																// ����Լ�2
			break;

		case	2:
			CancelProcNum = 3;
			fnAPP_CancelProc3(CancelCode, pCancelMsg, pCancelMsg2, pCancelMsg3, CancelScrNum);	
																// ����Լ�3
			break;

		case	3:
			CancelProcNum = 4;
			fnAPP_CancelProc4(CancelCode, pCancelMsg, pCancelMsg2, pCancelMsg3, CancelScrNum);	
																// ����Լ�4
			break;

		default:
			if (!m_pDevCmn->fnAPL_CheckError())					// ��ְ�����
			{
				if (!CancelWithProcFlg)							// ������ó��Flg : �����Ұ� �ƴѰ�츸 ó��
					fnAPP_ErrorTranProc();						// �ŷ������������
			}
			if (!m_pDevCmn->fnAPL_CheckError())					// ��ְ�����
			{
				fnAPP_SendMakeManageSend(FALSE);				// �۽������ۼ�(������ Send)

			}

			if (   TranCode2 == TRANID_7210 || TranCode2 == TRANID_7220 || TranCode2 == TRANID_7260 
				|| TranCode2 == TRANID_7230 || TranCode2 == TRANID_7250 || TranCode2 == TRANID_7380 
				|| TranCode2 == TRANID_4370 || TranCode2 == TRANID_4390 || TranCode2 == TRANID_4310
				|| TranCode2 == TRANID_4330 || TranCode2 == TRANID_4350 || TranCode2 == TRANID_4320
				|| TranCode2 == TRANID_4340 || TranCode2 == TRANID_7200 || TranCode2 == TRANID_4410 || TranCode2 == TRANID_7530) 
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("END  :01OOOOOOOOOOOO<<=====<<=====<<=====<<<<"));
			}

			fnAPP_TerminateProc(T_CANCEL);						// �������
			break;
	}

/////////////////////////////////////////////////////////////////////////////
//	��ǥ�����ʱ�ȭó��
/////////////////////////////////////////////////////////////////////////////
	if (!CancelCheckClearFlg)									// ��ǥ�����ʱ�ȭFlg
	{
		CancelCheckClearFlg = TRUE;
		if ((TranProc == TRAN_WITH)					&&			// ���
			(Asc2Int(Accept.CheckMoney, 8) != 0)	&&			// ��ǥ���
			(TranSend)								&&			// �ŷ�����
			(!m_pDevCmn->fnAPL_CheckError()))					// ��ְ�����
		{
			m_pDevCmn->fnAPL_SetNumberOfOutCheck(0, 0);			// ��ݼ�ǥ�ż��ʱ�ȭ
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	���Ӱŷ�����ó��
/////////////////////////////////////////////////////////////////////////////
//	Accept.SerialCount = 0;
//	Accept.SerialTranFlag = FALSE;
/////////////////////////////////////////////////////////////////////////////
//	ȭ��ó��
/////////////////////////////////////////////////////////////////////////////
	if (((m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) && (m_pDevCmn->fnSPR_GetMaterialInfo()))	||	// ��ǥ����
		((m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU)) && (m_pDevCmn->fnMCU_GetMaterialInfo()))	||	// ī������
		((m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM)) && (m_pDevCmn->fnPBM_GetMaterialInfo())))		// ��������
		DelaySec = 1;											// ��ü����ô��

	if (CancelScrNum)
	{
		if ((CancelScrNum == 763)	||							// ī��-�ش�ŷ��Ұ�
			(CancelScrNum == 765)	||							// ����-�ش�ŷ��Ұ�
			(CancelScrNum == 764)	)							// �ۿܼ�-�ش�ŷ��Ұ�
		{
			m_pDevCmn->fnSCR_DisplayString(1, MenuTbl[MenuIndex].MenuScrName);
		}

		m_pDevCmn->fnSCR_DisplayScreen(CancelScrNum);			// ����ȭ�����
		Delay_Msg(DelaySec * 3000);
	}
	else 
	{
		int		ScrNum = 701;									// �ȳ�ȭ��(5Line)
		
		// �����ATM �����ŷ��� ��ҵ� ���� �Ʒ� ������ ��� Play �����ϵ��� Delay�� �ø���.
		if (m_pDevCmn->EarPhoneInserted == TRUE)				// �����ATM
			DelaySec = 5;

		switch (CancelCode)
		{
			case	T_CANCEL:									// ��Ҽ���
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "��      ��");
					m_pDevCmn->fnSCR_DisplayString(2, "    �ŷ��� ��� �Ǿ����ϴ�");
					m_pDevCmn->fnSCR_DisplayString(4, "    ó������ �ٽ� �Ͽ� �ֽʽÿ�");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please try again");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);

				break;

			case	T_TIMEOVER:									// �ð��ʰ�
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "��      ��");
					m_pDevCmn->fnSCR_DisplayString(2, "    �ð��� �ʰ� �Ͽ����ϴ�");
					m_pDevCmn->fnSCR_DisplayString(4, "    ó������ �ٽ� �Ͽ� �ֽʽÿ�");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please try again");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);

				break;
			
			case	T_INPUTOVER:								// �Է�Ƚ���ʰ�
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "��      ��");
					m_pDevCmn->fnSCR_DisplayString(2, "    �Է� Ƚ���� �ʰ� �Ͽ����ϴ�");
					m_pDevCmn->fnSCR_DisplayString(4, "    ó������ �ٽ� �Ͽ� �ֽʽÿ�");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please try again");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);

				break;
			
			case	T_INPUTERROR:								// �Է��ڷ��̻�
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "��      ��");
					m_pDevCmn->fnSCR_DisplayString(2, "    �Է��ڷ� �̻� �߻�");
					m_pDevCmn->fnSCR_DisplayString(4, "    [��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please try again");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;
			
			case	T_ERROR:									// ��ֹ߻�(�����ָ޼���)
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "��      ��");
					m_pDevCmn->fnSCR_DisplayString(2, "    ��ְ� �߻� �Ͽ����ϴ�");
					m_pDevCmn->fnSCR_DisplayString(4, "    �ܾ��� Ȯ���Ͽ� �ֽʽÿ�");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please check your account balance");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;

			case	T_SENDERROR:								// 2003.11.07
			case	T_RECVERROR:
//				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
//					m_pDevCmn->fnSCR_DisplayString(1, "��      ��");
//				else
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
				
				if ((TranProc == TRAN_WITH)		||				// ���
					(TranProc == TRAN_TRANS)	||				// ��ü
					(TranProc == TRAN_DEP)		||				// �Ա�
					(TranProc == TRAN_HIPASS))					// �����н�	#N0266
				{
					if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
						m_pDevCmn->fnSCR_DisplayString(3, "    �ܾ��� Ȯ���Ͽ� �ֽʽÿ�");
					else
						m_pDevCmn->fnSCR_DisplayString(3, "Please check your account balance");
				}
				else 
				{
					if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
						m_pDevCmn->fnSCR_DisplayString(3, "    ó���� ��ְ� �߻��Ͽ����ϴ�");
					else
						m_pDevCmn->fnSCR_DisplayString(3, "Your transaction has been cancelled");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;
			
			case	T_PROGRAM:									// ���α׷��̻���ֹ߻�(����ι���)

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "��      ��");
					m_pDevCmn->fnSCR_DisplayString(2, "    ���α׷� �̻� �߻�");
					m_pDevCmn->fnSCR_DisplayString(4, "    [��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
				}
				else
				{
//					m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					m_pDevCmn->fnSCR_DisplayString(2, "Your transaction has been cancelled");
					m_pDevCmn->fnSCR_DisplayString(4, "Please check your account balance");
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				
				break;		
			case	T_MSG:										// ��ҹ߻�(�޼���ó��)
				if ((pCancelMsg)	||
					(pCancelMsg2)	||
					(pCancelMsg3))
				{
//					if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
//						m_pDevCmn->fnSCR_DisplayString(1, "��      ��");
//					else
//						m_pDevCmn->fnSCR_DisplayString(1, "[Information]");
					if (pCancelMsg)
						m_pDevCmn->fnSCR_DisplayString(2, pCancelMsg);
					if (pCancelMsg2)
						m_pDevCmn->fnSCR_DisplayString(3, pCancelMsg2);
					if (pCancelMsg3)
						m_pDevCmn->fnSCR_DisplayString(4, pCancelMsg3);
					
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_5_WAIT);
					m_pDevCmn->fstrSCR_GetKeyString();						
//					m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
//					Delay_Msg(DelaySec * 5000);
				}
				break;
			case	T_DISMSG:	// ��ҹ߻�(�޼���ó��)
				if (pCancelMsg)
					m_pDevCmn->fnSCR_DisplayStringArray(1, pCancelMsg,  "\n");
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 5000);
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnSCR_DisplayStringArray1[%s]", pCancelMsg);
				break;
			case	T_HOSTMSG:
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnSCR_DisplayStringArray1[%s]", pCancelMsg2);
				if (pCancelMsg)
					m_pDevCmn->fnSCR_DisplayStringArray(1, pCancelMsg);
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(5000);
				break;

			default:
				break;
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	�۽��������ۼ�
/////////////////////////////////////////////////////////////////////////////
//	if ((!TranSend)			&&									// �ŷ����۹� V01-00-00-#301
//		(!CancelDepSendFlg)	)
//	{
//		if (!AddSerialFlg)										// �Ϸù�ȣ������
//		{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc::�Աݰŷ��� �ŷ����۹��� �ŷ��Ϸù�ȣ ����ó��");
//			AddSerialFlg = TRUE;								// �Ϸù�ȣ������
//			m_pDevCmn->fnAPL_AddSerialNo();						// �ŷ��Ϸù�ȣ����
//		}
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc::�Աݰŷ��� �ŷ����۹��� �۽����� ������ ó��");
//		fnAPP_SendMake(FALSE);									// �۽������ۼ���
// 	}
	
/////////////////////////////////////////////////////////////////////////////
//	��ü���ڹ׹���ó��
/////////////////////////////////////////////////////////////////////////////
	if (!CancelPrintFlg)										// �������Flg
	{
		CancelPrintFlg = TRUE;
		if ((!m_pDevCmn->fnAPL_CheckError())	||				// ��ְ�����
			(!CancelKCashSendFlg)				||				// K-CASH���
			((EMVReadFlg)						&&				// EMV Read
			 (TranSend))						)
		{
			fnAPD_CardEmbossScan();								// ī�忥������ĵ
			fnAPD_CardImprint(TRUE);							// ī��������Ʈ

// ���޼��ŷ��� ���(CancelWithSendFlg : Set(FALSE)) ��ְ� �����Ǵ��� �̰����� ��ָ�ǥ�� ������ �ʿ�� ����.
// ��ǥ�� ���ڴ� Cancel-#3���� �� ó���� �Ͽ��� ������ ���� ������ ���ȴ�.
// �׷��� ���޼��� �ƴѰ��(CancelWithSendFlg : TRUE)���� �̰����� ��ָ�ǥ�� �����Ͽ��� �Ѵ�.
// ���� ������ ���並 �ʿ�� �Ѵ�.
// �Ա�����÷��׿� �������÷��׸� ���� ó���Ͽ��� �Ѵ�. �ϳ��� ���ؼ� �ٸ� �ϳ��� ������� �ʵ���...
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc::����÷���[%d/%d]", CancelWithSendFlg, CancelDepSendFlg);
//			if (((TranProc == TRAN_WITH) && (!CancelWithSendFlg)) ||
//				((TranProc == TRAN_DEP)  && (!CancelDepSendFlg))  )
			if ((TranProc == TRAN_WITH) && (!CancelWithSendFlg))// V01-00-00-#301
			{
				;												// ������/�Ա���ҽ� ��ֹ�����
			}
//t			if ((CancelWithSendFlg)	||							// ����������Flg(�ʱ�:���๫)
//t				(CancelDepSendFlg)	)							// ����Ա�����Flg
			else
			{
				JnlPrintFlg = FALSE;							// �������ڹ�
				fnAPP_PrintError(DEV_JPR);						// �������(����)

				if (SlipPrintFlg)								// ��ǥ������
				{
					fnAPD_MaterialRetract(DEV_SPR);				// ȸ������ָ�ǥ����
					SlipPrintFlg = FALSE;						// ��ǥ���ڹ�
					fnAPD_CheckDeviceAction(DEV_SPR);			// ���ۿϷ����&��ְ���
				}

//2012.01.11 Kim.Gi.Jin
//				fnAPP_PrintError(DEV_SPR);
//				fnAPD_CheckDeviceAction(DEV_SPR);				
			}
		}
		else
		{
			JnlPrintFlg = FALSE;								// �������ڹ�
			if (((TranProc == TRAN_DEP && TranProcSave != TRAN_E_TICKET )		&&					// �Ա�  #N0142 ���ڻ�ǰ�� �Ա� ��� ��ǥ ����x
				 (!CancelDepPrintFlg)	))//	||				// ����Ա�����Flg(�ʱ�:���๫)
// 				((TranProc == TRAN_TRANS)	&&					// �۱�
// 				 (TranSend)				)	)					// ������ȸ
			{
				CancelDepPrintFlg = TRUE;
				fnAPD_CardEmbossScan();							// ī�忥������ĵ
				fnAPD_CardImprint(TRUE);						// ī��������Ʈ
																// �������(����)
				if (CashInCountFlg && AddSerialFlg) //#0021 -> #0059
				{
					fnAPP_PrintCancel(DEV_SPR);						// �������(��ǥ)
					fnAPD_CheckDeviceAction(DEV_SPR);				// ���ۿϷ����&��ְ���
				}
				fnAPP_PrintCancel(DEV_JPR); //#0059
			}
			else
			if (TranSend)										// �ŷ�������
			{
				fnAPD_CardEmbossScan();							// ī�忥������ĵ
				fnAPD_CardImprint(TRUE);						// ī��������Ʈ
				fnAPP_PrintCancel(DEV_JPR);						// �������(����)
			}
		}
	}
	
//	if ((!CancelWithSendFlg)	||								// ����������Flg(�ʱ�:���๫)
//		(!CancelDepSendFlg)	)									// ����Ա�����Flg(�ʱ�:���๫)
	if (!CancelWithSendFlg)										// ����������Flg(�ʱ�:���๫) V01-00-00-#301
	{
		if (((TranProc == TRAN_WITH)				&&			// ���
			 (TranSend)								&&			// �ŷ�����
			 (!m_pDevCmn->fnAPL_CheckError()))		||			// ��ְ�����
			((TranProc == TRAN_WITH)				&&			// ���
			 (EMVReadFlg)							&&
			 (TranSend))							)
		{
			if (SlipPrintFlg)									// ��ǥ������
			{
				fnAPD_MaterialRetract(DEV_SPR);					// ȸ��(��1��ǥ)
				SlipPrintFlg = FALSE;							// ��ǥ���ڹ�
				fnAPD_CheckDeviceAction(DEV_SPR);				// ���ۿϷ����&��ְ���
			}
		}
//		else
//		if (((TranProc == TRAN_DEP)			||					// �Ա�	V01-00-00-#301
//			 (TranCode == TC_RFCASH))		&&					// ����ī����������
//			(!m_pDevCmn->fnAPL_CheckError()))					// ��ְ�����
//		{
//			if ((SlipPrintFlg)		&&							// ��ǥ������
//				(!CancelDepSendFlg)	)
//			{
//				fnAPD_MaterialRetract(DEV_SPR);					// ȸ��(��1��ǥ)
//				SlipPrintFlg = FALSE;							// ��ǥ���ڹ�
//				fnAPD_CheckDeviceAction(DEV_SPR);				// ���ۿϷ����&��ְ���
//			}
//		}
	}
//	else														// 2004.02.16
//	if (!CancelKCashSendFlg)									// �����ŷ�Flg(�ʱ�:���๫)
//	{
//		if (SlipPrintFlg)										// ��ǥ������
//		{
//			fnAPD_MaterialRetract(DEV_SPR);						// ȸ��(��1��ǥ)
//			SlipPrintFlg = FALSE;								// ��ǥ���ڹ�
//			fnAPD_CheckDeviceAction(DEV_SPR);					// ���ۿϷ����&��ְ���
//		}
//	}

	// �������ȸ���������ڵ��κ�
	// �Ա����ݼ����������ڵ��̺κ�
	if (((TranProc == TRAN_DEP)		&&	// �Աݼۼ�����ֽ�
		 (!m_pDevCmn->fnAPL_CheckError())							&&	// ����ó�� : ���û�� 2002.04.30
		 ((memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "771", 3) == 0)	||
		  (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "88", 2) == 0)))	)
	{
		fnAPP_SumProc(2);										// �Ա�����
		fnAPD_CheckDepositStore(TRUE);							// ��ǥ�Աݼ���
		fnAPD_CashDepositStore(TRUE);							// �����Աݼ���
	}

	CStringArray strTempRetractArray;
	char	StrBuff[256] = {0,};
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc::�̼����÷���[%d/%x/%5.5s]", TranProc, MenuAvail, m_pDevCmn->CurErrBuff.ErrorCode);
	if ((TranProc == TRAN_WITH)										&&
		(!m_pDevCmn->fnAPL_CheckError())							&&
		(memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "01102", 5) == 0)	&&
		((m_pDevCmn->AtmDefine.CashHandler == OKI_SET)				||
		 (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET)				||
		 (m_pDevCmn->AtmDefine.CashHandler == BRM_SET))				)
	{
//		fnAPD_MoneyRetract();
		m_pDevCmn->DeviceAutoOff = 0;							// �ڵ����� �� Device = Set
		m_pDevCmn->DeviceLoss = DEV_NONE;						// �н������ʱ�ȭ
		m_pDevCmn->ForceDownFlag = FALSE;						// ����Down���� 

		if (m_pDevCmn->fnCSH_GetMaterialInfo())					// ��ü�ܷ��������ϱ�
		{
			if (m_pProfile->DEVICE.MachineType == U8100)
				m_pDevCmn->fnBRM_Reset(TRUE); 
			else
			{
				m_pDevCmn->fnCSH_Initialize();
			}
			
		}
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			// ���۴�����ְ���
		Delay_Msg(2000);										// �������(2000ms)
																// ȸ���������
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			// ���۴�����ְ���

		if (Asc2Int(Accept.Money, 8))
		{
			SplitString(m_pDevCmn->fstrCSH_GetRetractResult(), ",", strTempRetractArray);

			GetTime(StrBuff);
			sprintf(ShuData[0], "%2.2s:%2.2s:%2.2s", &StrBuff[0], &StrBuff[2], &StrBuff[4]);
			sprintf(ShuData[1], "[�̼��� ȸ������]");
			sprintf(ShuData[2], "�̼���ݾ�  :%d", Asc2Int(Accept.Money, 8));
			sprintf(ShuData[3], " (����:%d,��ǥ:%d)", Asc2Int(Accept.CashMoney, 8), Asc2Int(Accept.CheckMoney, 8));
			sprintf(ShuData[4], "ȸ���ȳ���  :%d", Asc2Int(strTempRetractArray[0]));
			sprintf(ShuData[5], "�Ѱ���ż�  :%d", Asc2Int(strTempRetractArray[1]));
			sprintf(ShuData[6], "�����Ǹż�  :%d", Asc2Int(strTempRetractArray[2]));
			sprintf(ShuData[7], "��õ���Ǹż�:%d", Asc2Int(strTempRetractArray[3]));
			sprintf(ShuData[8], "õ���Ǹż�  :%d", Asc2Int(strTempRetractArray[4]));
			sprintf(ShuData[9], "�Ҹ�Ǹż�  :%d", Asc2Int(strTempRetractArray[5]));


			if (Asc2Int(Accept.Money, 8) == Asc2Int(strTempRetractArray[0]))
			{
				m_pDevCmn->ForceDownFlag = FALSE;
				sprintf(ShuData[10], "�̼��� ȸ������ ������");
			}
			else
			{
				m_pDevCmn->ForceDownFlag = TRUE;
				sprintf(ShuData[10], "�̼��� ȸ������ ����ġ!!");
			}
			fnAPD_JnlPrint(TRUE);
		}
	}

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}

// ����Լ�2
int	CTranCmn::fnAPP_CancelProc2(int CancelCode, char *pCancelMsg, char *pCancelMsg2, char *pCancelMsg3, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc2:"			\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlg[%d], "		\
													"CancelWithSendFlg[%d], "			\
													"m_pDevCmn->TranStatus[%d]",
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlg,
													 CancelWithSendFlg,
													 m_pDevCmn->TranStatus);

/////////////////////////////////////////////////////////////////////////////
//	������ó��
/////////////////////////////////////////////////////////////////////////////
	if (!CancelWithSendFlg)										// ����������Flg(�ʱ�:���๫)
	{
		CancelWithSendFlg = TRUE;
		
		if ((TranProc == TRAN_WITH ) &&			  // ���
			(TranSend)				 &&		      // �ŷ�����
			(!m_pDevCmn->fnAPL_CheckError())) // ��ְ�����
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc2-2");
			CancelWithProcFlg = TRUE;							// ������ó��Flg
			m_EMVCancelFlg = FALSE;
			SlipPrintFlg = FALSE;								// ��ǥ��������
			JnlPrintFlg = FALSE;								// ������������

			AddSerialFlg = FALSE;								// �Ϸù�ȣ����Flag�ʱ�ȭ
			m_pDevCmn->fnAPL_SetProcCount('3');					// ���༳��
			fnAPP_SendHostCancel();								// �۽�(������)
			fnAPP_SumProc(0);									// ����������
			fnAPP_RecvHost();									// ����
			if (m_pDevCmn->TranResult)							// HOST OK
				m_pDevCmn->fnAPL_SetProcCount('5');				// ���༳��
			else
				m_pDevCmn->fnAPL_SetProcCount('9');			    // HOST NG

			fnAPP_ErrorCancelProc();
/////////////////////////////////////////////////////////////////////////////
// ���ó��3���� ���ι� ��ǥ�� ������ó���Ѵ�.
// ���� �̰������� ��/�������ڴ� ���� �ʿ����.
// ���� ��/���� ���ڸ� ����Լ�3�� ó���� �����ϰ� �̰������� ó���� ���´�.
// �̰������� ���� ȣ��Ʈ�޽��� ��¸��� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
//t			fnAPP_PSPProc(DEV_JPR);								// ��������Ʈ
//			fnAPP_PMDProc();									// �޼���ǥ��
//			if (PMDRecvData.ParaLength)							// DATA��/�޼���ǥ��(�ŷ�)
//				Delay_Msg(2*1000);
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				m_pDevCmn->fnSCR_DisplayString(1, "   �����Ұ� ����ó���Ǿ����ϴ�");
				m_pDevCmn->fnSCR_DisplayString(3, "   �ܾ��� Ȯ���Ͽ� �ֽʽÿ�");
				m_pDevCmn->fnSCR_DisplayScreen(701);			// �ȳ�ȭ��(5Line)
			}
			else
			{
				m_pDevCmn->fnSCR_DisplayString(1, "   �����Ұ� �Ҵ�ó���Ǿ����ϴ�");
				m_pDevCmn->fnSCR_DisplayString(3, "   [��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
				m_pDevCmn->fnSCR_DisplayScreen(701);			// �ȳ�ȭ��(5Line)
			}
			Delay_Msg(3*1000);

//t			fnAPP_PSPProc(DEV_SPR);								// ������Ʈ
//t			fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR);			// ���ۿϷ����&��ְ���
		}
	}
	else
	if (!CancelDepSendFlg)										// ����Ա�����Flg(�ʱ�:���๫) V01-00-00-#301
	{
		CancelDepSendFlg = TRUE;
		
		if ((TranProc == TRAN_DEP)					&&			// �Ա�
			(!m_pDevCmn->fnAPL_CheckError()))					// ��ְ�����
		{
			CancelDepProcFlg = TRUE;							// ����Ա�ó��Flg

//			SlipPrintFlg = FALSE;								// ��ǥ��������
//			JnlPrintFlg = FALSE;								// ������������
//
//			m_pDevCmn->fnAPL_SetProcCount('3');					// ���༳��
//			fnAPP_SendHostCancel();								// �۽�(����Ա�)
//			fnAPP_RecvHost();									// ����
//			if (m_pDevCmn->TranResult)							// HOST OK
//				m_pDevCmn->fnAPL_SetProcCount('5');				// ���༳��
//			else m_pDevCmn->fnAPL_SetProcCount('9');			// HOST NG
//
////t			fnAPP_PSPProc(DEV_JPR);								// ��������Ʈ
//			fnAPP_PMDProc();									// �޼���ǥ��
//			if (PMDRecvData.ParaLength)							// DATA��/�޼���ǥ��(�ŷ�)
//				Delay_Msg(2*1000);
////			if (m_pDevCmn->TranResult)							// HOST OK
////			{
////				//m_pDevCmn->fnSCR_DisplayString(2, "   �Ա���Ұ� ����ó���Ǿ����ϴ�");
////				m_pDevCmn->fnSCR_DisplayString(1, "   �ܾ��� Ȯ���Ͽ� �ֽʽÿ�");
////				m_pDevCmn->fnSCR_DisplayScreen(710);			// �ȳ�ȭ��(5Line)
////			}
////			else
////			{
////				//m_pDevCmn->fnSCR_DisplayString(2, "   �Ա���Ұ� �Ҵ�ó���Ǿ����ϴ�");
////				m_pDevCmn->fnSCR_DisplayString(2, "   [��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
////				m_pDevCmn->fnSCR_DisplayScreen(710);			// �ȳ�ȭ��(5Line)
////			}
////			Delay_Msg(3*1000);
//
////t			fnAPP_PSPProc(DEV_SPR);								// ������Ʈ
////t			fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR);			// ���ۿϷ����&��ְ���
		}
	}
						

	if (CancelHIPassLoadFlg)									//#N0266
	{
		CancelHIPassLoadFlg = FALSE;							// �����н� �ŷ����н� Complete ó��Flag #0439
		fnAPP_HPSComplete();									// �����н� �ŷ����н� Complete ó��Flag #0439
		//fnAPP_HIPASS_SendErrorComplet();
	}
	
//	else														// 2004.02.16
//	if (!CancelKCashSendFlg)									// �����ŷ�Flg(�ʱ�:���๫) V01-00-00-#301
//	{
//		CancelKCashSendFlg = TRUE;
//		
//		fnAPP_KCashRemoveTranBit();								// �����ŷ�����
//	}

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}
	
// ����Լ�3
int	CTranCmn::fnAPP_CancelProc3(int CancelCode, char *pCancelMsg, char *pCancelMsg2, char *pCancelMsg3, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc3:"			\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlg[%d], "		\
													"CancelWithProcFlg[%d], "		\
													"m_pDevCmn->TranStatus[%d]",	
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlg,
													 CancelWithProcFlg,
													 m_pDevCmn->TranStatus);

/////////////////////////////////////////////////////////////////////////////
//	��������ó��
/////////////////////////////////////////////////////////////////////////////
	if (CancelWithProcFlg)										// ��Ұŷ�
	{
		JnlPrintFlg = FALSE;									// �������ڹ�
		fnAPP_PrintWithCancelTran(DEV_JPR);
																// ��Ұŷ�����(����)
		if (SlipPrintFlg)										// ��ǥ������
		{
			fnAPD_MaterialRetract(DEV_SPR);						// ȸ������ָ�ǥ����
			SlipPrintFlg = FALSE;								// ��ǥ���ڹ�
			fnAPD_CheckDeviceAction(DEV_SPR);					// ���ۿϷ����&��ְ���
		}
		MaterialOutFlg &= ~DEV_SPR;								// ������� �� ����ó��
		fnAPP_PrintWithCancelTran(DEV_SPR);
																// ��Ұŷ�����(��ǥ)
	}

	if (CancelDepProcFlg)										// ��Ұŷ�
	{
		JnlPrintFlg = FALSE;									// �������ڹ�
		fnAPP_PrintDepCancelTran(DEV_JPR);
																// ��Ұŷ�����(����)
		if (SlipPrintFlg)										// ��ǥ������
		{
			fnAPD_MaterialRetract(DEV_SPR);						// ȸ������ָ�ǥ����
			SlipPrintFlg = FALSE;								// ��ǥ���ڹ�
			fnAPD_CheckDeviceAction(DEV_SPR);					// ���ۿϷ����&��ְ���
		}
		MaterialOutFlg &= ~DEV_SPR;								// ������� �� ����ó��
		fnAPP_PrintDepCancelTran(DEV_SPR);
																// ��Ұŷ�����(��ǥ)
	}

	if (CancelDepRejectProcFlg)									// �Աݹ�ȯó��
	{
		// ���ù���������ڵ��κ�
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// ��ü����(��1��ǥ)
		fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM);	// ��ü���������ȸ��
	}
	else
	{
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM | DEV_ICM);				// ��ü����(��1��ǥ)
		fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM | DEV_ICM);	// ��ü���������ȸ��
	}

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}
	
// ����Լ�4
int	CTranCmn::fnAPP_CancelProc4(int CancelCode, char *pCancelMsg, char *pCancelMsg2, char *pCancelMsg3, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc4:"			\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlg[%d], "		\
													"m_pDevCmn->TranStatus[%d]",
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlg,
													 m_pDevCmn->TranStatus);

/////////////////////////////////////////////////////////////////////////////
//	��üȸ��ó��
/////////////////////////////////////////////////////////////////////////////
	if (CancelDepRejectProcFlg)									// �Աݹ�ȯó��
	{
		if (((m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) && (m_pDevCmn->fnSPR_GetMaterialInfo())) ||	// ��ǥ����
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU)) && (m_pDevCmn->fnMCU_GetMaterialInfo())) ||	// ī������
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM)) && (m_pDevCmn->fnPBM_GetMaterialInfo())))	// ��������
			fnAPD_MaterialRetract(DEV_SPR | DEV_MCU | DEV_PBM);								// ��üȸ��
		// ���ù���������ڵ��κ�
	}
	else
	{
		if (((m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) && (m_pDevCmn->fnSPR_GetMaterialInfo())) ||	// ��ǥ����
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU)) && (m_pDevCmn->fnMCU_GetMaterialInfo())) ||	// ī������
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM)) && (m_pDevCmn->fnPBM_GetMaterialInfo())))	// ��������
			fnAPD_MaterialRetract(DEV_SPR | DEV_MCU | DEV_PBM);								// ��üȸ��
	}

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}

// �����Լ�
int	CTranCmn::fnAPP_TerminateProc(int TerminateCode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateProc(TerminateCode[%d])", TerminateCode);

	int		ChkID = DEV_MAIN;

	switch (TerminateCode)										// �����ATM
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateProc : EarPhoneInserted[%d], TerminateCode[%d]", m_pDevCmn->EarPhoneInserted, TerminateCode);

		case	T_OK:
			if (m_pDevCmn->TranStatus == TRAN_TRAN)
			{
				if (m_pDevCmn->EarPhoneInserted == TRUE)
				{
					while(TRUE)
					{
						m_pDevCmn->fnSCR_DisplayString(1, "�ŷ��� �����Ͻ÷���");
						m_pDevCmn->fnSCR_DisplayString(2, "�̾����� �̾Ƽ� �������ñ� �ٶ��ϴ�.");
						m_pDevCmn->fnSCR_DisplayString(3, "�̿��� �ּż� �����մϴ�.");
						m_pDevCmn->fnSCR_DisplayScreen(701);
						Delay_Msg(5000);							// ������Ʈ ��µ��� ���� ��å
						m_pDevCmn->EarPhoneInserted = m_pDevCmn->fnSNS_GetEarPhoneStatus();

						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��    �� : [����� EarPhone ����]"));	
						
						if ((m_pDevCmn->EarPhoneInserted == TRUE))
						{
							Delay_Msg(6000);
						}
						else	
							break;
					}
				}
				else	//	#N0272
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
					m_pDevCmn->fnSCR_DisplayScreen(445, K_5_WAIT);
					m_pDevCmn->fstrSCR_GetKeyString();
				}
			}	
			

			break;
		default:
			break;
	}

	if (m_pDevCmn->TranStatus == TRAN_OPEN)						// ����ó��
	{
/////////////////////////////////////////////////////////////////////////////
		CString		strTempDbData("");
		CString		strTemp("");

		m_pDevCmn->fnSCR_DisplayString(1, "�ڵ�ȭ��⸦ �غ��ϰ� �ֽ��ϴ�.");
		m_pDevCmn->fnSCR_DisplayString(2, "��ø� ��ٷ� �ֽʽÿ�.");
		m_pDevCmn->fnSCR_DisplayScreen(701);			

		// ��������
		// OM_SENSORINFO||
		strTempDbData = "OM_SENSORINFO";

		// ����(YYYYMMDD)||
		strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
		strTempDbData += "||" + strTemp;

		// �߻��ð�(HHMMSS)||
		strTemp.Format("%6.6s", GetTime().GetBuffer(0));
		strTempDbData += "||" + strTemp;

		// SliceLevel����
		strTemp = IniGetStr(_SP_DEVICE_INI, "BRM", "SliceLevel");
		strTemp.Replace("(", "");								// ���λ���
		strTemp.Replace(")", "");
		strTempDbData += "||" + strTemp;

		m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);		// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
	}
	m_pDevCmn->TranProc = TranProc;								// Tran�ŷ�ó������

	if(m_pDevCmn->HostFirstOpenFlag)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateProc - ��밡������ġ���ϱ�");
		ChkID &= m_pDevCmn->fnAPL_GetAvailDevice(ChkID);			// ��밡������ġ���ϱ�
		m_pDevCmn->fnAPL_CheckDeviceAction(ChkID);					// ��ü���ۿϷ�ó��
	}

//	fnAPP_SetPollTimeStart();									
	// V07-01-11-#00
	if (m_pDevCmn->TTSEngineLoadFlg)
	{
		MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "TTSEngineLoadFlg : FALSE");
		m_pDevCmn->TTSEngineLoadFlg = FALSE;
		m_pDevCmn->TTS_EndProc();
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateProc(TerminateCode 1 [%d])", TerminateCode);

	if (TerminateCode == T_OK)				
		throw CTranCmnExit(TRUE);								// ������������
	else
		throw CTranCmnExit(FALSE);								// �����������	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateProc(TerminateCode 2 [%d])", TerminateCode);

	return TerminateCode;
}
// �����Լ�
int	CTranCmn::fnAPP_TerminateDummyProc(int TerminateCode)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateDummyProc(TerminateCode[%d])", TerminateCode);

	if (TerminateCode == T_OK)				
		throw CTranCmnExit(TRUE);								// ������������
	else
		throw CTranCmnExit(FALSE);								// �����������	
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateDummyProc(TerminateCode 2 [%d])", TerminateCode);

	return TerminateCode;
}
