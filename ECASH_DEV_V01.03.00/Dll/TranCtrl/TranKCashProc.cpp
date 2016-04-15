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
//	K-CASH�Լ�
/////////////////////////////////////////////////////////////////////////////
// K-CASH��й�ȣ�˻�
int CTranCmn::fnAPP_KCashCheckPin()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashCheckPin()");

	int		nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : ����ȭ��ÿ� ���� ����ó�� 2004.05.21
		m_pDevCmn->fnSCR_DisplayScreen(823);					// ī���б� : ����ȭ��ø� ��� 2004.05.21
		m_pDevCmn->fnKCS_CheckPin(MakeUnPack(Accept.KCashPassWord, 8));
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		break;

	case FIC_OPEN :
		m_pDevCmn->fnSCR_DisplayScreen(823);					// �б�(ī��) 2004.05.21
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
		if (nReturn == 17)										// ��й�ȣ����
		{
			m_pDevCmn->fnKCS_PowerOn();
			fnAPD_CheckDeviceAction(DEV_MCU);
			if (m_pDevCmn->FICCardType == FIC_OPEN)
			{
				fnAPD_FICDistinctCardType(TRAN_IC_FINANCEIC);
				fnAPD_FICBasicInquiry();						// �⺻������ȸ
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
		if (nReturn == 23)										// ��й�ȣ����Ƚ���ʰ�
		{
			fnAPP_CancelProc(T_MSG, "���� ȭ�� ��й�ȣ �����Է�Ƚ���� �ʰ��Ǿ����ϴ�.",
									"�������� �����Ͽ� �ֽʽÿ�.");
																// K-CASH��й�ȣ����Ƚ���ʰ�
		}
		else
			fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
									"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī���б����
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashCheckPin():return");
	return T_OK;
}

// ����ȭ�� PAN �����б�
int CTranCmn::fnAPP_KCashReadPan()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadPan()");

	int		nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : ����ȭ��ÿ� ���� ����ó�� 2004.05.21
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
		if (nReturn == 17)										// ��й�ȣ����
			fnAPP_CancelProc(T_MSG, "���� ȭ�� ī�� ��й�ȣ�� Ʋ���ϴ�.",
									"ó������ �ٽ� �ŷ��Ͽ� �ֽʽÿ�.");
																// K-CASH��й�ȣ����
		else
		if (nReturn == 23)										// ��й�ȣ����Ƚ���ʰ�
			fnAPP_CancelProc(T_MSG, "���� ȭ�� ��й�ȣ �����Է�Ƚ���� �ʰ��Ǿ����ϴ�.",
									"�������� �����Ͽ� �ֽʽÿ�.");
																// K-CASH��й�ȣ����Ƚ���ʰ�
		else
			fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
									"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī���б����
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadPan():return");
	return T_OK;
}

// ����ȭ�� �ְ������� �м�ó��(PAN�������� �м�)
// ���� : �ݵ�� PAN���� ��ȸ���� ó���ؾ� �Ѵ�.
int CTranCmn::fnAPP_KCashCheckMainAccount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashCheckMainAccount()");

	if (IsZero(m_pDevCmn->KCashInfo.Account, sizeof(m_pDevCmn->KCashInfo.Account)))
		return T_OK;											// ���������� ��ȿ�� ��쿡�� ó��

	CString strICData("");
	CString strAccTmp("");
	char	szAccData[1024];
	CString strNewAccCheck("");									// V02-06-01-#10 ��ִ���:����ȭ���ְ����� �Ű���Ȯ��
																//				          �����ڵ尡 24�� �Ű���(13), 20�̸� ������(14)

	strICData.Empty();
	strAccTmp.Empty();
	memset(szAccData, 0x00, sizeof(szAccData));

	memset(  m_pDevCmn->KCashInfo.MainAccount, 0x00, sizeof(m_pDevCmn->KCashInfo.MainAccount));
	memset( &m_pDevCmn->KCashInfo.MainAccount[0], 0x01, 1);		// �����ڵ�[0]
	memcpy( &m_pDevCmn->KCashInfo.MainAccount[1], 
			&m_pDevCmn->KCashInfo.Account[0],  3);				// �����ڵ�[1]~�����ڵ�
	
	strNewAccCheck = MakeUnPack(&m_pDevCmn->KCashInfo.Account[0], 3).Mid(3, 2);
	strAccTmp = "0" + MakeUnPack(&m_pDevCmn->KCashInfo.Account[3], 7) + "0";

	MakePack(strAccTmp.GetBuffer(0), szAccData, 16);
	memcpy( &m_pDevCmn->KCashInfo.MainAccount[4], szAccData, 8);
	strICData = "";
	strICData += ",,";											// 1/2Ʈ������Ÿ ����
	strICData += MakeUnPack(MakeUnPack(m_pDevCmn->KCashInfo.MainAccount, 52, FALSE).GetBuffer(0), 104, FALSE);
	fnAPD_CardReadCheck(strICData.GetBuffer(0));				// Ʈ������ Ȯ��

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashCheckMainAccount():return");
	return T_OK;
}

// ����ȭ�� JAN �����б�
int CTranCmn::fnAPP_KCashReadBAmount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadBAmount()");

	int		nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : ����ȭ��ÿ� ���� ����ó�� 2004.05.21
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
			fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
									"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// ī���б����
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadBAmount():return");
	return T_OK;
}

// ����ȭ���ȣ �б�
int CTranCmn::fnAPP_KCashReadEID()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadEID()");

	int		nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : ����ȭ��ÿ� ���� ����ó�� 2004.05.21
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
			fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
									"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// ī���б����
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadEID():return");
	return T_OK;
}

// ����ȭ�� ���������б�
int CTranCmn::fnAPP_KCashReadTrafficInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadTrafficInfo()");

	int		nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : ����ȭ��ÿ� ���� ����ó�� 2004.05.21
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
	// �������� �����б�� ������ �м����� �ʵ����Ѵ�.(�Ϻΰŷ� �����ϰ� ū�ǹ̾���)
	//	fnAPP_CancelProc(T_CANCEL, NULL, NULL, NULL, 599);	// ī���б����
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashReadTrafficInfo():return");
	return T_OK;
}

// ����ȭ�� ȯ�Һ�����ó��
int CTranCmn::fnAPP_KCashPrepareUnloadRcvIEP()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashPrepareUnloadRcvIEP()");

	int		nReturn = 0;

	m_pDevCmn->fnSCR_DisplayScreen(823);						// ī���б�
	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : ����ȭ��ÿ� ���� ����ó�� 2004.05.21
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
			fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
									"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// ī���б����
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashPrepareUnloadRcvIEP():return");
	return T_OK;
}

// K-CASH�ŷ�����BitȮ��
// 2004.02.18 : ���/������ī�� ����ó��
int CTranCmn::fnAPP_KCashVerifyTranBit()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashVerifyTranBit()");
	
	int nReturn = 0;

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : ����ȭ��ÿ� ���� ����ó�� 2004.05.21
		m_pDevCmn->fnKCS_VerifyTranBit();							// �ŷ�����BitȮ��
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		break;

	case FIC_OPEN :
		m_pDevCmn->fnKCS_VerifyTranBit();							// �ŷ�����BitȮ��
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		break;

	default:
		nReturn = 9999;
		break;
	}

	if (nReturn)
	{
		if (nReturn == 24)										// �ŷ�����BitȮ�ε�
			return T_ERROR;
		else
			fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
									"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// ī���б����
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashVerifyTranBit():return");
	return T_OK;
}

// K-CASH�ŷ�����Bit����
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
	m_pDevCmn->fnKCS_VerifyTranBit();							// �ŷ�����BitȮ��
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData() == 24)					// �ŷ�����Bit
		;
	else
		fnAPP_CancelProc(T_CANCEL, NULL, NULL, NULL, 599);		// K-CASHī���б����
*/
/////////////////////////////////////////////////////////////////////////////
	fnAPP_KCashReadBAmount();									// �ܾ������б�

	m_pDevCmn->fnKCS_ReadCLog();								// ����ȯ�Ұŷ������б�
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
								"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// ī���б����

	if (m_pDevCmn->KCashInfo.LoadUnloadCnt == 0)				// ����/ȯ�ҳ�����ȸ�Ǽ�
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
								"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī����Ұ�

	memcpy(Accept.Money, Int2Asc(ByteHighOrder2Int(&m_pDevCmn->KCashInfo.LoadUnloadBuff[0][7], 4), 8), 8);
																// �ŷ��ݾ׹��

	TranProcSave = TranProc;
	TranCodeSave = TranCode;
	TranCode2Save = TranCode2;
	
	if (m_pDevCmn->KCashInfo.LoadUnloadBuff[0][0] == 0x80)		// �����ŷ�Ȯ��
	{
		TranProc = TRAN_IC;
		TranCode = TC_KCLOADRCV;								// K-CASH��������
		TranCode2 = 0;
		TranSend = 0;											// �߽ż��� �ʱ�ȭ
	}
	else
	if (m_pDevCmn->KCashInfo.LoadUnloadBuff[0][0] == 0x88)		// ȯ�Ұŷ�Ȯ��
	{
		TranProc = TRAN_IC;								
		TranCode = TC_KCUNLOADRCV;								// K-CASHȯ�Һ���
		TranCode2 = 0;
		TranSend = 0;											// �߽ż��� �ʱ�ȭ
	}
	else
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
								"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī����Ұ�

	if (TranCode == TC_KCLOADRCV)								// K-CASH��������
	{															// ������ó��(�ŷ��ݾ�8+��ȣ16)
		m_pDevCmn->fnKCS_PrepareLoadIEP(MakeUnPack(&m_pDevCmn->KCashInfo.LoadUnloadBuff[0][7], 4)
									+	MakeUnPack(Accept.KCashPassWord, 8));
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())
			fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
									"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī����Ұ�

																// ������ó����:LoadUnloadAccount->Account�κ���
		memcpy( m_pDevCmn->KCashInfo.Account,					// PAN���� ����ó��
				m_pDevCmn->KCashInfo.LoadUnloadAccount,
				10);
	}
	else
	if (TranCode == TC_KCUNLOADRCV)								// K-CASHȯ�Һ���
	{															// ȯ�Һ�����ó��(��ȣ16)
		fnAPP_KCashPrepareUnloadRcvIEP();

																// ȯ�Һ�����ó����:Account->LoadUnloadAccount�κ���
		memcpy( m_pDevCmn->KCashInfo.LoadUnloadAccount,			// PAN���� ����ó��
				m_pDevCmn->KCashInfo.Account,
				10);
	}

	fnAPP_KCashCheckMainAccount();								// �ְ������� Ȯ��ó��(�ݵ�� PAN�б� ����ó��)

/////////////////////////////////////////////////////////////////////////////
	AddSerialFlg = FALSE;										// �Ϸù�ȣ����������
																// V02-06-01-#10 �����ŷ��� �ŷ��Ϸù�ȣ �߰�ó��

	m_pDevCmn->fnAPL_SetProcCount('3');							// ���༳��
	fnAPP_SendHost();											// �۽�
	fnAPP_RecvHost();											// ����
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// ���༳��
	else m_pDevCmn->fnAPL_SetProcCount('9');					// HOST NG

	if (((TranCode == TC_KCLOADRCV)		&&						// K-CASH��������
		 (m_pDevCmn->TranResult)		&&						// HOST OK
		 (KCashHostICFlag))						||				// 
		((TranCode == TC_KCUNLOADRCV)	&&						// K-CASHȯ�Һ���
		 (m_pDevCmn->TranResult)))
	{
		m_pDevCmn->fnKCS_RemoveTranBit();						// �ŷ�����Bit����
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())
			fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
									"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī����Ұ�

		fnAPP_KCashReadBAmount();								// �ܾ������б�

		m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
		if (TranCode == TC_KCLOADRCV)							// ���������ŷ���
			m_pDevCmn->fnSCR_DisplayScreen(546, K_30_WAIT, PIN_MENU_MODE);// �����ŷ�Ȯ��
		else
		if (TranCode == TC_KCUNLOADRCV)							// ȯ�Һ����ŷ���
			m_pDevCmn->fnSCR_DisplayScreen(547, K_30_WAIT, PIN_MENU_MODE);// �����ŷ�Ȯ��
		else
			m_pDevCmn->fnSCR_DisplayScreen(546, K_30_WAIT, PIN_MENU_MODE);// �����ŷ�Ȯ��

		m_pDevCmn->fstrSCR_GetKeyString();

		fnAPP_PSPProc(DEV_JPR);									// ��������Ʈ
		fnAPP_PMDProc();										// �޼���ǥ��
		fnAPP_PSPProc(DEV_SPR);							        // ������Ʈ
	}
	else
	if ((TranCode == TC_KCLOADRCV)		&&						// K-CASH��������
		(m_pDevCmn->TranResult))								// HOST OK
	{															// �����䱸(����16+����8)
		m_pDevCmn->fnKCS_LoadIEP(GetSprintf("%16.16s", GetString(PMERecvData.Data[0], 16, 16).GetBuffer(0))
							+	 GetSprintf("%8.8s",   GetString(PMERecvData.Data[1],  8,  8).GetBuffer(0)));
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())
			fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
									"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī����Ұ�

		fnAPP_PSPProc(DEV_JPR);							        // ��������Ʈ
//		fnAPP_PMDProc();										// �޼���ǥ��
//		fnAPP_PSPProc(DEV_SPR);									// ������Ʈ
		
		/* ----------------------------------------------------------------
		// �������ŷ��� ����ŷ����� ����
		// �Žý��� ����� ������ ��û����
		// 2004.06.10
		m_pDevCmn->fnAPL_SetProcCount('3');						// ���༳��
		fnAPP_SendHost();										// �۽�
		fnAPP_RecvHost();										// ����
		if (m_pDevCmn->TranResult)								// HOST OK
			m_pDevCmn->fnAPL_SetProcCount('5');					// ���༳��
		else m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG
		------------------------------------------------------------------ */
//		if (m_pDevCmn->TranResult)								// HOST OK
//		{
//			fnAPP_KCashReadBAmount();							// �ܾ������б�
//
//			m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
//			m_pDevCmn->fnSCR_DisplayScreen(546, K_30_WAIT, PIN_MENU_MODE);// �����ŷ�Ȯ��
//			m_pDevCmn->fstrSCR_GetKeyString();
//		}
	}
	else
	{
		if (KCashHostICFlag == STOPBITREMOVE)
		{
			fnAPP_PSPProc(DEV_JPR);								// ��������Ʈ
			m_pDevCmn->fnKCS_RemoveTranBit();					// �ŷ�����Bit����
			fnAPD_CheckDeviceAction(DEV_MCU);
		}
		else
		{
			fnAPP_PSPProc(DEV_JPR);								// ��������Ʈ
			fnAPP_PMDProc();									// �޼���ǥ��
			fnAPP_PSPProc(DEV_SPR);								// ������Ʈ

			fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU );
																// ���ۿϷ����&��ְ���
			fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);		// ��ü����	
			fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );
																// ��ü���������ȸ��
			fnAPP_TerminateProc(T_OK);							// ��������
		}
	}

	TranProc = TranProcSave;
	TranCode = TranCodeSave;
	TranCode2 = TranCode2Save;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashRemoveTranBit():return");
	return T_OK;
}

// K-CASH����
int CTranCmn::fnAPP_KCashLoad()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashLoad()");
	BYTE	TranAmount[4];

	fnAPP_AcceptPassword();										// ��й�ȣ�Է�
	fnAPP_AcceptKCashMoney();									// K-CASH�ݾ��Է�

	memset(TranAmount, 0x00, sizeof(TranAmount));
	Asc2IntHighOrder(Accept.Money, TranAmount, 8, 4);			// �ݾ׺�ȯ

																// ������ó��(�ŷ��ݾ�8+��ȣ16)
	m_pDevCmn->fnKCS_PrepareLoadIEP(MakeUnPack(TranAmount, 4)
								+	MakeUnPack(Accept.KCashPassWord, 8));
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
								"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// �����߻�

/////////////////////////////////////////////////////////////////////////////
	CString strAccount;											// �ְ��¹�ȣ����ġüũ
	int		nCompareAccount = 0;

	strAccount = MakeUnPack(&m_pDevCmn->KCashInfo.LoadUnloadAccount[3], 7, FALSE);
	if (CardTran & MENU_J3)										// ����ī���ΰ��
	{
		nCompareAccount = memcmp(&pCardData3->AccountNo[1], strAccount.GetBuffer(0), 14);
	}
	else														// ������ �ƴѰ��
		nCompareAccount = memcmp(&pCardData3->AccountNo[0], strAccount.GetBuffer(0), 14);

	if (nCompareAccount)
	{
		m_pDevCmn->fnKCS_RemoveTranBit();						// �ŷ�����Bit����
		fnAPD_CheckDeviceAction(DEV_MCU);
		fnAPP_CancelProc(T_MSG, "ICī�� ���� �ְ���������",
								"������ ���¹�ȣ�� Ʋ������",
								"�������� �����Ͻñ� �ٶ��ϴ�.");
																// �ְ��º���ġ-���
	}

/////////////////////////////////////////////////////////////////////////////
	CancelKCashSendFlg = FALSE;									// ����������Flg(�ʱ�:���๫)
	m_pDevCmn->fnAPL_SetProcCount('3');							// ���༳��
	fnAPP_SendHost();											// �۽�
	fnAPP_RecvHost();											// ����
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// ���༳��
	else m_pDevCmn->fnAPL_SetProcCount('9');					// HOST NG

	if (!m_pDevCmn->TranResult)									// HOST OK 2004.06.21
	{
		CancelKCashSendFlg = TRUE;								// ����������Flg(�ʱ�:���๫)
		m_pDevCmn->fnKCS_RemoveTranBit();						// �ŷ�����Bit����
		fnAPD_CheckDeviceAction(DEV_MCU);

		TranProc = TRAN_IC;								
		TranCode = TC_KCPARAKEY;								// �Ķ���Ͱ���(Key)
		TranCode2 = 0;
		TranSend = 0;											// �߽ż��� �ʱ�ȭ
		fnAPP_KCashUpdate();									// �Ķ���Ͱ���
	}
	else
	if (!m_pDevCmn->TranResult)									// HOST OK 2004.06.21
	{
		CancelKCashSendFlg = TRUE;								// ����������Flg(�ʱ�:���๫)
		m_pDevCmn->fnKCS_RemoveTranBit();						// �ŷ�����Bit����
		fnAPD_CheckDeviceAction(DEV_MCU);

		TranProc = TRAN_IC;								
		TranCode = TC_KCPARALIMIT;								// �Ķ���Ͱ���(�ִ������ѵ�)
		TranCode2 = 0;
		TranSend = 0;											// �߽ż��� �ʱ�ȭ
		fnAPP_KCashUpdate();									// �Ķ���Ͱ���
	}
	else
	if (m_pDevCmn->TranResult)									// HOST OK
	{
																// �����䱸(����16+����8)
		m_pDevCmn->fnKCS_LoadIEP(GetSprintf("%16.16s", GetString(PMERecvData.Data[0], 16, 16).GetBuffer(0))
							+	 GetSprintf("%8.8s",   GetString(PMERecvData.Data[1],  8,  8).GetBuffer(0)));
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())
			fnAPP_CancelProc(T_MSG, NULL, NULL, NULL);			// ���ó��

		fnAPP_PSPProc(DEV_JPR);									// ��������Ʈ
		fnAPP_PMDProc();										// �޼���ǥ��
		fnAPP_PSPProc(DEV_SPR);									// ������Ʈ
		fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);	// ���ۿϷ����&��ְ���

		CancelKCashSendFlg = TRUE;								// ����������Flg(�ʱ�:���๫)

		/* ----------------------------------------------------------------
		// �������ŷ��� ����ŷ����� ����
		// �Žý��� ����� ������ ��û����
		// 2004.06.10
		m_pDevCmn->fnAPL_SetProcCount('3');						// ���༳��
		fnAPP_SendHost();										// �۽�
		fnAPP_RecvHost();										// ����
		if (m_pDevCmn->TranResult)								// HOST OK
			m_pDevCmn->fnAPL_SetProcCount('5');					// ���༳��
		else m_pDevCmn->fnAPL_SetProcCount('9');				// HOST NG
		------------------------------------------------------------------ */

		if (m_pDevCmn->TranResult)								// HOST OK
		{
			fnAPP_KCashReadBAmount();							// �ܾ������б�

			m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
			m_pDevCmn->fnSCR_DisplayScreen(546, K_30_WAIT, PIN_MENU_MODE);// �����ŷ�Ȯ��
			m_pDevCmn->fstrSCR_GetKeyString();
		}
	}
	else
	{
		CancelKCashSendFlg = TRUE;								// ����������Flg(�ʱ�:���๫)

		m_pDevCmn->fnKCS_RemoveTranBit();						// �ŷ�����Bit����
		fnAPD_CheckDeviceAction(DEV_MCU);

		fnAPP_PSPProc(DEV_JPR);        							// ��������Ʈ
		fnAPP_PMDProc();										// �޼���ǥ��
		fnAPP_PSPProc(DEV_SPR);        							// ������Ʈ
		fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);	// ���ۿϷ����&��ְ���
	}
	
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// ��ü���������ȸ��

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashLoad():return");
	return T_OK;
}

// K-CASHȯ��
int CTranCmn::fnAPP_KCashUnload()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashUnload()");

	CString strReturn("");
	BYTE	TranAmount[4];

	fnAPP_AcceptKCashMoney();									// K-CASH�ݾ��Է�

	memset(TranAmount, 0x00, sizeof(TranAmount));
	Asc2IntHighOrder(Accept.Money, TranAmount, 8, 4);			// �ݾ׺�ȯ

																// ȯ����ó��(�ŷ��ݾ�8+��ȣ16)
	m_pDevCmn->fnKCS_PrepareUnLoadIEP(MakeUnPack(TranAmount, 4)
									+ MakeUnPack(Accept.KCashPassWord, 8));
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
								"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī����Ұ�

/////////////////////////////////////////////////////////////////////////////
	CancelKCashSendFlg = FALSE;									// ����������Flg(�ʱ�:���๫)	2004.02.17

	m_pDevCmn->fnAPL_SetProcCount('3');							// ���༳��
	fnAPP_SendHost();											// �۽�
	fnAPP_RecvHost();											// ����
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// ���༳��
	else m_pDevCmn->fnAPL_SetProcCount('9');					// HOST NG

	if (!m_pDevCmn->TranResult)									// HOST OK 2004.06.21
	{
		CancelKCashSendFlg = TRUE;								// ����������Flg(�ʱ�:���๫)
		m_pDevCmn->fnKCS_RemoveTranBit();						// �ŷ�����Bit����
		fnAPD_CheckDeviceAction(DEV_MCU);

		TranProc = TRAN_IC;								
		TranCode = TC_KCPARAKEY;								// �Ķ���Ͱ���(Key)
		TranCode2 = 0;
		fnAPP_KCashUpdate();									// �Ķ���Ͱ���
	}
	else
	if (!m_pDevCmn->TranResult)									// HOST OK 2004.06.21
	{
		CancelKCashSendFlg = TRUE;								// ����������Flg(�ʱ�:���๫)
		m_pDevCmn->fnKCS_RemoveTranBit();						// �ŷ�����Bit����
		fnAPD_CheckDeviceAction(DEV_MCU);
		
		TranProc = TRAN_IC;								
		TranCode = TC_KCPARALIMIT;								// �Ķ���Ͱ���(�ִ������ѵ�)
		TranCode2 = 0;
		fnAPP_KCashUpdate();									// �Ķ���Ͱ���
	}
	else
	if (m_pDevCmn->TranResult)									// HOST OK
	{
		CString NewAccount("");
		int		NewAccountLen = 0;

		m_pDevCmn->fnSCR_DisplayString(1, GetString(PMERecvData.Data[3], 20, 20));	
																// �Ա�����
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
																// ���¹�ȣ
		m_pDevCmn->fnSCR_DisplayString(3, GetString(PMERecvData.Data[6], 20, 20));	
																// ����
		m_pDevCmn->fnSCR_DisplayAmount(4, (LPCTSTR)Accept.Money, 15);
																// �ŷ��ݾ�
		m_pDevCmn->fnSCR_DisplayAmount(5, (LPCTSTR)&PMERecvData.Data[7], 6);
																// ������

		// ȯ�ұݾ�Ȯ��ȭ�����						
		m_pDevCmn->fnSCR_DisplayScreen(548, K_30_WAIT, PIN_MENU_MODE);// ȯ�Ұŷ�Ȯ��
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CONFIRM)
			;
		else 
		{
			m_pDevCmn->fnKCS_RemoveTranBit();					// �ŷ�����Bit����
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
		
																// ȯ�ҿ䱸(����16+����8)
		m_pDevCmn->fnKCS_UnloadIEP(GetSprintf("%16.16s", GetString(PMERecvData.Data[0], 16, 16).GetBuffer(0))
							+	 GetSprintf("%8.8s",   GetString(PMERecvData.Data[1],  8,  8).GetBuffer(0)));
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())
		{
			m_pDevCmn->fnKCS_RemoveTranBit();					// �ŷ�����Bit����
			fnAPD_CheckDeviceAction(DEV_MCU);

			fnAPP_KCashReadBAmount();							// �ܾ������б�

			m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
			m_pDevCmn->fnSCR_DisplayScreen(547, K_30_WAIT, PIN_MENU_MODE);// ȯ�Ұŷ�Ȯ��
			m_pDevCmn->fstrSCR_GetKeyString();
		}
		else
		{
			CancelKCashSendFlg = FALSE;							// ����������Flg(�ʱ�:���๫)
			
			m_pDevCmn->fnAPL_SetProcCount('3');					// ���༳��
			fnAPP_SendHost();									// �۽�
			fnAPP_RecvHost();									// ����
			if (m_pDevCmn->TranResult)							// HOST OK
				m_pDevCmn->fnAPL_SetProcCount('5');				// ���༳��
			else m_pDevCmn->fnAPL_SetProcCount('9');			// HOST NG

			if (m_pDevCmn->TranResult)
				CancelKCashSendFlg = TRUE;						// ����������Flg(�ʱ�:���๫)
			else
				fnAPP_CancelProc(T_MSG, NULL, NULL, NULL);		// ���ó��

			m_pDevCmn->fnKCS_RemoveTranBit();					// �ŷ�����Bit����
			fnAPD_CheckDeviceAction(DEV_MCU);

			fnAPP_KCashReadBAmount();							// �ܾ������б�

			m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
			m_pDevCmn->fnSCR_DisplayScreen(547, K_30_WAIT, PIN_MENU_MODE);// ȯ�Ұŷ�Ȯ��
			m_pDevCmn->fstrSCR_GetKeyString();
		}														// K-CASHī����Ұ�
	}
	else
	{
		m_pDevCmn->fnKCS_RemoveTranBit();						// �ŷ�����Bit����
		fnAPD_CheckDeviceAction(DEV_MCU);
	}
	
	fnAPP_PSPProc(DEV_JPR);										// ��������Ʈ
	fnAPP_PMDProc();											// �޼���ǥ��
	fnAPP_PSPProc(DEV_SPR);										// ������Ʈ
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);		// ���ۿϷ����&��ְ���

	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// ��ü���������ȸ��

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashUnload():return");
	return T_OK;
}

// ��й�ȣ����
int CTranCmn::fnAPP_KCashChangePin()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashChangePin()");

	int nReturn = 0;
	CString strReturn("");
	CString strChangedPIN("");

	switch(m_pDevCmn->FICCardType)
	{
	case FIC_CLOSE:
	case FIC_NATIVE:											// NATIVE : ����ȭ��ÿ� ���� ����ó�� 2004.05.21
//		fnAPP_AcceptKCashPassword(TRAN_IC_KCASH);				// ��й�ȣ�Է�
		fnAPP_AcceptKCashChangePassword(TRAN_IC_KCASH);			// ����ȭ���й�ȣ�����Է�
		m_pDevCmn->fnSCR_DisplayScreen(823);					// ����ȭ��ó����

		m_pDevCmn->fnKCS_ChangePin(MakeUnPack(Accept.KCashPassWord, 8)	
								+  MakeUnPack(Accept.ChangeICPwdConfirm, 8));
																// ��й�ȣ����(��������ȣ16+�����ľ�ȣ16)
		fnAPD_CheckDeviceAction(DEV_MCU);
		nReturn = m_pDevCmn->fnKCS_AnalRecvData();
		strChangedPIN = "����ȭ�� ��й�ȣ��";
		break;

	case FIC_OPEN:												// V02-06-01-#9 20050415_LDC_A
////////////////////////////////////////////////////////////////
// �ּ�ó���� ������ ����ȭ�󿡴� ���������� ����ǳ� ����IC��� ���������� ó������ �ʴ´�.
//		m_pDevCmn->fnFIC_ChangePin(MakeUnPack(Accept.KCashPassWord, 8)	
//								+  MakeUnPack(Accept.ChangeICPwdConfirm, 8));
//		fnAPD_CheckDeviceAction(DEV_MCU);
//		nReturn = m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_CHANGEPIN);
////////////////////////////////////////////////////////////////
		m_pDevCmn->fnSCR_DisplayScreen(26);						// PIN������ DF���� (����ȭ��,����IC,������)
//		m_pDevCmn->fnSCR_DisplayImage(3, FALSE);				// ������ �ŷ����� 
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();

		if (strReturn == "����ȭ��")
		{
			m_pDevCmn->fnICA_SelectApplet(TRAN_IC_KCASH);					// 
			fnAPD_CheckDeviceAction(DEV_MCU);
			if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
				fnAPP_CancelProc(T_MSG, "�� ī��� ��� �� �� �����ϴ�");

			fnAPP_AcceptKCashPassword(TRAN_IC_KCASH);			// ��й�ȣ�Է�
			fnAPP_KCashCheckPin();								// PIN Check
			fnAPP_AcceptKCashChangePassword(TRAN_IC_KCASH);		// ����ȭ���й�ȣ�����Է�
			strChangedPIN = "����ȭ�� ��й�ȣ��";
		}
		else
		if (strReturn == "����IC")
		{
			m_pDevCmn->fnICA_SelectApplet(TRAN_IC_FINANCEIC);					// 
			fnAPD_CheckDeviceAction(DEV_MCU);
			if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
				fnAPP_CancelProc(T_MSG, "�� ī��� ��� �� �� �����ϴ�");

			fnAPP_AcceptKCashPassword(TRAN_IC_FINANCEIC);		// ��й�ȣ�Է�
			fnAPP_KCashCheckPin();								// PIN Check
			fnAPP_AcceptKCashChangePassword(TRAN_IC_FINANCEIC);	// �����й�ȣ�����Է�
			strChangedPIN = "����IC ��й�ȣ��";
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
		fnAPP_CancelProc(T_CANCEL, NULL, NULL, NULL, 597);		// K-CASH��й�ȣ�������	

	m_pDevCmn->fnSCR_DisplayString(1, strChangedPIN.GetBuffer(0));
	m_pDevCmn->fnSCR_DisplayString(2, "���������� ������ �Ǿ����ϴ�.");
	m_pDevCmn->fnSCR_DisplayScreen(701);						// K-CASH��й�ȣ���漺��
	Delay_Msg(2 * 1000);

	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// ��ü���������ȸ��

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashUnload():return");
	return T_OK;
}

// K-CASH�ܾ���ȸ
int CTranCmn::fnAPP_KCashInquiry()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashInquiry()");

	CString	strReturn("");
	CString strTemp("");
	int		i = 0, j = 1;
	CString	strTranType("");									// �ŷ�����
	CString	strTranSeqNo("");									// �ŷ��Ϸù�ȣ
	CString	strTranAmount("");									// �ŷ��ݾ�
	CString	strTranBAmount("");									// �ŷ����ܾ�
	CString	strTranResult("");									// �ŷ����
	CString	strCardNum("");

	m_pDevCmn->fnKCS_ReadCLog();								// ����ȯ�Ұŷ������б�
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
								"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī����Ұ�

	m_pDevCmn->fnSCR_DisplayAmount(1, ByteHighOrder2Int(m_pDevCmn->KCashInfo.Balance, 4));
	m_pDevCmn->fnSCR_DisplayScreen(514, K_30_WAIT, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	fnAPD_CardEmbossScan(TRUE);
	fnAPD_CardImprint(TRUE);
	fnAPP_PSPProc(DEV_JPR);
	
	if (strReturn == "����ȭ������")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}
	else
	if (strReturn == "�ְ��·�ȯ��")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCUNLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}

	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// ��ü���������ȸ��

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashInquiry():return");
	return T_OK;
}

// ����/ȯ�ҳ���
int CTranCmn::fnAPP_KCashLoadUnloadInquiry()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashLoadUnloadInquiry()");

	int		ScrNum = 0;
	CString	strReturn("");
	CString strTemp("");
	int		i = 0, j = 1;
	CString	strTranType("");									// �ŷ�����
	CString	strTranSeqNo("");									// �ŷ��Ϸù�ȣ
	CString	strTranAmount("");									// �ŷ��ݾ�
	CString	strTranBAmount("");									// �ŷ����ܾ�
	CString	strTranResult("");									// �ŷ����

	m_pDevCmn->fnKCS_ReadCLog();								// ����ȯ�Ұŷ������б�
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
								"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī����Ұ�
	ScrNum = 651;
	
	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	for (i = 0; i < m_pDevCmn->KCashInfo.LoadUnloadCnt; i++)
	{
		strTranType  = "";										// �ŷ�����
		strTranSeqNo  = "";										// �ŷ��Ϸù�ȣ
		strTranAmount  = "";									// �ŷ��ݾ�
		strTranBAmount  = "";									// �ŷ����ܾ�
		strTranResult  = "";									// �ŷ����

		if (m_pDevCmn->KCashInfo.LoadUnloadBuff[i][0] == 0x80)
			strTranType = "��  ��";
		else
		if (m_pDevCmn->KCashInfo.LoadUnloadBuff[i][0] == 0x88)
			strTranType = "ȯ  ��";

		// �Ϸù�ȣ
		strTranSeqNo = Int2Asc(ByteHighOrder2Int(&m_pDevCmn->KCashInfo.LoadUnloadBuff[i][1], 2), 5);
		// �ŷ��ݾ�
		strTranAmount = Int2Asc(ByteHighOrder2Int(&m_pDevCmn->KCashInfo.LoadUnloadBuff[i][3], 4), 9);
		// �ŷ����
		if (memcmp(&m_pDevCmn->KCashInfo.LoadUnloadBuff[i][11], ISO7816_SW_NO_ERROR, 2) == 0)
			strTranResult = "����";
		else
		if (memcmp(&m_pDevCmn->KCashInfo.LoadUnloadBuff[i][11], ISO7816_SW_BYTES_REMAINING, 1) == 0)
			strTranResult = "����";
		else
			strTranResult = "�Ҵ�";

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

	if (strReturn == "����ȭ������")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}
	else
	if (strReturn == "�ְ��·�ȯ��")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCUNLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}
	
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// ��ü���������ȸ��

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashLoadUnloadInquiry():return");
	return T_OK;
}

// ���ų�����ȸ
int CTranCmn::fnAPP_KCashBuyListInquiry()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashBuyListInquiry()");

	int		ScrNum = 0;
	CString	strReturn("");
	CString strTemp("");
	int		i = 0, j = 1;
	CString	strTranType("");									// �ŷ�����
	CString	strTranSeqNo("");									// �ŷ��Ϸù�ȣ
	CString	strTranAmount("");									// �ŷ��ݾ�
	CString	strTranBAmount("");									// �ŷ����ܾ�
	CString	strTranResult("");									// �ŷ����

	m_pDevCmn->fnKCS_ReadPLog1();								// ����ȯ�Ұŷ������б�
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
								"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī����Ұ�
	ScrNum = 652;
	
	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	for (i = 0; i < m_pDevCmn->KCashInfo.BuyListCnt; i++)
	{
		strTranType  = "";										// �ŷ�����
		strTranSeqNo  = "";										// �ŷ��Ϸù�ȣ
		strTranAmount  = "";									// �ŷ��ݾ�
		strTranBAmount  = "";									// �ŷ����ܾ�
		strTranResult  = "";									// �ŷ����

		if (m_pDevCmn->KCashInfo.BuyListBuff[i][0] == 0x80)
			strTranType = "��  ��";
		else
		if (m_pDevCmn->KCashInfo.BuyListBuff[i][0] == 0x88)
			strTranType = "ȯ  ��";

		// �Ϸù�ȣ
		strTranSeqNo = Int2Asc(ByteHighOrder2Int(&m_pDevCmn->KCashInfo.BuyListBuff[i][1], 2), 5);
		// �ŷ��ݾ�
		strTranAmount = Int2Asc(ByteHighOrder2Int(&m_pDevCmn->KCashInfo.BuyListBuff[i][3], 4), 9);
		// �ŷ����
		if (memcmp(&m_pDevCmn->KCashInfo.BuyListBuff[i][11], ISO7816_SW_NO_ERROR, 2) == 0)
			strTranResult = "����";
		else
		if (memcmp(&m_pDevCmn->KCashInfo.BuyListBuff[i][11], ISO7816_SW_BYTES_REMAINING, 1) == 0)
			strTranResult = "����";
		else
			strTranResult = "�Ҵ�";

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
	
	if (strReturn == "����ȭ������")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}
	else
	if (strReturn == "�ְ��·�ȯ��")
	{
		ChoiceTran = TRAN_IC;
		TranProc = TRAN_IC;
		TranCode = TC_KCUNLOAD;
		TranCode2 = 0;
		fnAPD_CardReadSerial();
		return T_OK;
	}
	
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// ��ü���������ȸ��

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashBuyListInquiry():return");
	return T_OK;
}

// �Ķ���Ͱ��� 2004.02.27
int CTranCmn::fnAPP_KCashUpdate()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashUpdate()");
	m_pDevCmn->fnKCS_PrepareUpdateIEP();						// �Ķ���ͺ����ʱ�ȭ
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
								"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī���б����

	m_pDevCmn->fnAPL_SetProcCount('3');							// ���༳��
	fnAPP_SendHost();											// �۽�
	fnAPP_RecvHost();											// ����
	if (m_pDevCmn->TranResult)									// HOST OK
		m_pDevCmn->fnAPL_SetProcCount('5');						// ���༳��
	else m_pDevCmn->fnAPL_SetProcCount('9');					// HOST NG

	m_pDevCmn->fnKCS_UpdateIEP(GetSprintf("%8.8s",	 GetString(&PMERecvData.Data[3][0],  8,  8).GetBuffer(0))	// PpsamID
							+  GetSprintf("%4.4s",	 GetString(&PMERecvData.Data[4][2],  4,  4).GetBuffer(0))	// Parameter
							+  GetSprintf("%2.2s",	 GetString(&PMERecvData.Data[2][1],  2,  2).GetBuffer(0))	// Version
							+  GetSprintf("%32.32s", GetString(&PMERecvData.Data[5][0], 32, 32).GetBuffer(0))	// Parameter Value
							+  GetSprintf("%16.16s", GetString(&PMERecvData.Data[6][0], 16, 16).GetBuffer(0))	// Random Value
							+  GetSprintf("%8.8s",	 GetString(&PMERecvData.Data[7][0],  8,  8).GetBuffer(0)));	// Signature
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.",
								"[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
																// K-CASHī���б����

	fnAPP_PSPProc(DEV_JPR);            							// ��������Ʈ
	fnAPP_PMDProc();											// �޼���ǥ��
	fnAPP_PSPProc(DEV_SPR);            							// ������Ʈ
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);		// ���ۿϷ����&��ְ���
	
	fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM);				// ��ü����	
	fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM );// ��ü���������ȸ��

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KCashUpdate():return");
	return T_OK;
}

// ������ PIN��ȣ Ȯ��	2004.02.27
int CTranCmn::fnAPP_KCashCheckChangePin()
{
	if (IsSpace(Accept.ChangeICPwdConfirm, sizeof(Accept.ChangeICPwdConfirm)))
		return T_INPUTERROR;
//	if (!IsNum(Accept.ChangeICPwdConfirm, sizeof(Accept.ChangeICPwdConfirm)))
	if (!IsNum(Accept.ChangeICPwdConfirm, 6))					// �ּ� 6�ڸ��� Ȯ�� 2004.09.15
		return T_INPUTERROR;
	if (IsZero(Accept.ChangeICPwdConfirm, sizeof(Accept.ChangeICPwdConfirm)))
		return T_INPUTERROR;

	int		AscMask[8]  = {1, 2, 3, 4, 5, 6, 7, 8};
	int		DescMask[8] = {8, 7, 6, 5, 4, 3, 2, 1};
	int		InputSize  = __min(sizeof(Accept.ChangeICPwdConfirm), strlen(Accept.ChangeICPwdConfirm));

	char	EquMask[8];

	memset(EquMask, Accept.ChangeICPwdConfirm[0], sizeof(EquMask));	
	if (memcmp(EquMask, Accept.ChangeICPwdConfirm, InputSize) == 0)	// ���Ϲ�ȣ����Ȯ��
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
	if (memcmp(EquMask, AscData, InputSize) == 0)				// �������������Է�Ȯ��
		return T_INPUTERROR;
	memset(EquMask, DescData[0], sizeof(EquMask));	
	if (memcmp(EquMask, DescData, InputSize) == 0)				// �������������Է�Ȯ��
		return T_INPUTERROR;

	return T_OK;
}