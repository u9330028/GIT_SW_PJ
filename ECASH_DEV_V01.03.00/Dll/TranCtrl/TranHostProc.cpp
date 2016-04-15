/////////////////////////////////////////////////////////////////////////////
// TranHost.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\MacroDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"
#include "..\..\H\Dll\DevICCard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	�ۼ����Լ�
/////////////////////////////////////////////////////////////////////////////
// �۽�
int CTranCmn::fnAPP_SendHost()
{
	int		ScrNum = 0;

	int		nSendWaitSec = LINE_SEND_TIME;						
	if ((m_pDevCmn->TranStatus == TRAN_READY)	||				
		(m_pDevCmn->TranStatus == TRAN_OPEN)	)			
		nSendWaitSec = LINE_OPCL_TIME;

/////////////////////////////////////////////////////////////////////////////
//	�Ϸù�ȣ ����ó��
/////////////////////////////////////////////////////////////////////////////
	if (!AddSerialFlg)											// �Ϸù�ȣ������
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost AddSerialNo++");
		AddSerialFlg = TRUE;									// �Ϸù�ȣ������
		m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
	}

/////////////////////////////////////////////////////////////////////////////
//	�۽ź���
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->TranResult = FALSE;								// Host���
	SendLength = 0;												// �۽�Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// �۽�Buffer	
/////////////////////////////////////////////////////////////////////////////
	ScrNum = 601;												// ������ȭ��
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);						// ������ȭ��
	
	fnAPP_SendHostSet(TranCode2);
/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost::TranCode2 [%04d]", TranCode2);
	fnAPP_Tran_TxSend(TranCode2);
	fnAPD_CheckDepositPrint(DEV_JPR);							// �Աݼ�ǥ��������

/////////////////////////////////////////////////////////////////////////////
//	���ź���
/////////////////////////////////////////////////////////////////////////////
	RecvLength = 0;												// ����Length
	memset(RecvBuffer, 0, sizeof(RecvBuffer));					// ����Buffer
	memset(&RecvComm,	0, sizeof(RECVCOMM));					// �������
	memset(&PSPRecvData, 0, sizeof(PSPRecvData));				// DATA��/������Ʈ(�ŷ�)
	memset(&PPPRecvData, 0, sizeof(PPPRecvData));				// DATA��/���������Ʈ(�ŷ�)
	memset(&PPPRecvDataNon, 0, sizeof(PPPRecvDataNon));				// DATA��/���������Ʈ(�ŷ�) - Non
	memset(&PPPRecvDataKb, 0, sizeof(PPPRecvDataKb));				// DATA��/���������Ʈ(�ŷ�) - KB #N0227
	memset(&PPWRecvData, 0, sizeof(PPWRecvData));				// DATA��/����M/S WRITE(�ŷ�)
	memset(&PCWRecvData, 0, sizeof(PCWRecvData));				// DATA��/ī��M/S WRITE(�ŷ�)
	memset(&PMERecvData, 0, sizeof(PMERecvData));				// DATA��/�޼�������ǥ��(�ŷ�/����)
	memset(&PMDRecvData, 0, sizeof(PMDRecvData));				// DATA��/�޼���ǥ��(�ŷ�)

/////////////////////////////////////////////////////////////////////////////
//	������ó������
/////////////////////////////////////////////////////////////////////////////
	ChoiceTran = TRAN_NULL;										// ��ȸ�ļ��õȰŷ�
	DepNextTranFlag = FALSE;									// ���Ӱŷ�_�Ա�_Flag
	PbNextTranFlag = FALSE;										// ���� �����ŷ� �뺸�÷���
	PbNextPageFlag = FALSE;										// ���� ���������� �뺸�÷���
	InqNextTranFlag = FALSE;									// ��ȸ ������� �뺸 �÷���
	KCashHostICFlag = FALSE;									// K-CASH Host IC Flag
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->HostLineMode == HOST_ONLINE_MODE)			// HOST_ONLINE_MODE
	{
		if (!SendLength)
			fnAPP_CancelProc(T_PROGRAM);
	}

	if (!m_pDevCmn->fnAPL_CheckHostLine())						// ����üũ
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��۽� : ���]"));
		if (m_pDevCmn->TranStatus == TRAN_TOTAL || m_pDevCmn->TranStatus == TRAN_CASH_REG)
			m_pDevCmn->TranResult = FALSE;							// V01-00-00
		else
		{
			m_pDevCmn->fnAPL_StackError("7710891", "�۽źҰ�");
			{
//#0081
				if (TranCode == TC_WITHDRAW || TranCode == TC_SVCWITH)
				{
					fnAPP_WithErrSumProc(0);				
				}
				else
				if (TranCode == TC_DEPOSIT)
				{
					fnAPP_DepErrSumProc(0);
				}

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "�ŷ��� �Ұ��մϴ�", "������ �̿� �ٶ��ϴ�");
				else
					fnAPP_CancelProc(T_MSG, "Transaction is not valid");
			}
		}
	}


	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��۽�]"));	
	if (fnAPP_SendData(SendBuffer, SendLength, LINE_SEND_TIME) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��۽� : ���]"));
		if(TranCode2 == TRANID_4410 )
			m_pDevCmn->ForceDownFlag = TRUE;

		if (m_pDevCmn->TranStatus == TRAN_TOTAL || m_pDevCmn->TranStatus == TRAN_CASH_REG)
			m_pDevCmn->TranResult = FALSE;							// V01-00-00
		else
		{
			m_pDevCmn->fnAPL_StackError("7710991", "�۽Žð��ʰ�");	
			if (TranCode == TC_WITHDRAW || TranCode == TC_SVCWITH)
			{
				fnAPP_WithErrSumProc(0);				
			}
			else
			if (TranCode == TC_DEPOSIT)
			{
				fnAPP_DepErrSumProc(0);
			}

			fnAPP_CancelProc(T_SENDERROR);
		}
	}
	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_PROCESS);

	return T_OK;
}

// �۽�(������/�Ա�)
int CTranCmn::fnAPP_SendHostCancel()
{
	int	ScrNum = 0;			
/////////////////////////////////////////////////////////////////////////////
//	�۽ź���
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->TranResult = FALSE;								// Host���

	SendLength = 0;												// �۽�Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// �۽�Buffer
/////////////////////////////////////////////////////////////////////////////

	ScrNum = 701;												// ������ȭ��
	m_pDevCmn->fnSCR_DisplayString(1, "�ŷ� ��������� �������Դϴ�.");
	m_pDevCmn->fnSCR_DisplayString(2, "��ø� ��ٷ� �ֽʽÿ�.");
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);						// ������ȭ��

/////////////////////////////////////////////////////////////////////////////
// 	if (!AddSerialFlg)											// �Ϸù�ȣ������
// 	{
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost::�۽�ó���� �ŷ��Ϸù�ȣ ����ó��");
// 		AddSerialFlg = TRUE;									// �Ϸù�ȣ������
//		m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
// 	}
/////////////////////////////////////////////////////////////////////////////
	if(TranCode2 == TRANID_4310)
		TranCode2 = TRANID_4320;
	else
	if(TranCode2 == TRANID_4330)
		TranCode2 = TRANID_4340;
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "��� _ Trancode ���� �۽� ����");
		return T_OK;
	}


	fnAPP_Tran_TxSend(TranCode2);
/////////////////////////////////////////////////////////////////////////////
//	���ź���
/////////////////////////////////////////////////////////////////////////////
	RecvLength = 0;												// ����Length
	memset(RecvBuffer, 0, sizeof(RecvBuffer));					// ����Buffer

	memset(&PSPRecvData, 0, sizeof(PSPRecvData));				// DATA��/������Ʈ(�ŷ�)
	memset(&PPPRecvData, 0, sizeof(PPPRecvData));				// DATA��/���������Ʈ(�ŷ�)
	memset(&PPPRecvDataNon, 0, sizeof(PPPRecvDataNon));				// DATA��/���������Ʈ(�ŷ�)
	memset(&PPPRecvDataKb, 0, sizeof(PPPRecvDataKb));				// DATA��/���������Ʈ(�ŷ�) - KB #N0227
	memset(&PPWRecvData, 0, sizeof(PPWRecvData));				// DATA��/����M/S WRITE(�ŷ�)
	memset(&PCWRecvData, 0, sizeof(PCWRecvData));				// DATA��/ī��M/S WRITE(�ŷ�)
	memset(&PMERecvData, 0, sizeof(PMERecvData));				// DATA��/�޼�������ǥ��(�ŷ�/����)
	memset(&PMDRecvData, 0, sizeof(PMDRecvData));				// DATA��/�޼���ǥ��(�ŷ�)

/////////////////////////////////////////////////////////////////////////////
//	������ó������
/////////////////////////////////////////////////////////////////////////////
	ChoiceTran = TRAN_NULL;										// ��ȸ�ļ��õȰŷ�
	TranResultNGCode = 0;										// ���Ű�� NG�ڵ�
	PbNextTranFlag = FALSE;										// ���� �����ŷ� �뺸�÷���
	DepNextTranFlag = FALSE;									// ���Ӱŷ�_�Ա�_Flag
	PbNextPageFlag = FALSE;										// ���� ���������� �뺸�÷���
	InqNextTranFlag = FALSE;									// ��ȸ ������� �뺸 �÷���
	KCashHostICFlag = FALSE;									// K-CASH Host IC Flag
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->HostLineMode == HOST_ONLINE_MODE)			// HOST_ONLINE_MODE
	{
		if (!SendLength)
			fnAPP_CancelProc(T_PROGRAM);
	}

	if (!m_pDevCmn->fnAPL_CheckHostLine())						// ����üũ
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��۽� : ���]"));
		m_pDevCmn->fnAPL_StackError("7710892", "��Ҽ۽źҰ�");
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "���ó���� �Ұ��մϴ�", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
		else
			fnAPP_CancelProc(T_MSG, "Transaction is not valid");
	}

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��۽�]"));													
	if (fnAPP_SendData(SendBuffer, SendLength, LINE_SEND_TIME) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��۽� : ���]"));
		m_pDevCmn->fnAPL_StackError("7710992", "��Ҽ۽Žð��ʰ�");	// 2003.11.07
		fnAPP_CancelProc(T_SENDERROR);
	}


	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_PROCESS);// �����ư ������

	return T_OK;
}


// �Ϸ�
int CTranCmn::fnAPP_SendHostTranComplete()
{
	int		ScrNum = 0;
			
/////////////////////////////////////////////////////////////////////////////
//	�۽ź���
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->TranResult = FALSE;								// Host���

	SendLength = 0;												// �۽�Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// �۽�Buffer
/////////////////////////////////////////////////////////////////////////////
	fnAPP_Tran_TxSend(TranCode2);
/////////////////////////////////////////////////////////////////////////////
//	���ź���
/////////////////////////////////////////////////////////////////////////////
	RecvLength = 0;												// ����Length
	memset(RecvBuffer, 0, sizeof(RecvBuffer));					// ����Buffer

	memset(&PSPRecvData, 0, sizeof(PSPRecvData));				// DATA��/������Ʈ(�ŷ�)
	memset(&PPPRecvData, 0, sizeof(PPPRecvData));				// DATA��/���������Ʈ(�ŷ�)
	memset(&PPWRecvData, 0, sizeof(PPWRecvData));				// DATA��/����M/S WRITE(�ŷ�)
	memset(&PPPRecvDataNon, 0, sizeof(PPPRecvDataNon));				// DATA��/���������Ʈ(�ŷ�) - Non
	memset(&PPPRecvDataKb, 0, sizeof(PPPRecvDataKb));				// DATA��/���������Ʈ(�ŷ�) - KB #N0227
	memset(&PCWRecvData, 0, sizeof(PCWRecvData));				// DATA��/ī��M/S WRITE(�ŷ�)
	memset(&PMERecvData, 0, sizeof(PMERecvData));				// DATA��/�޼�������ǥ��(�ŷ�/����)
	memset(&PMDRecvData, 0, sizeof(PMDRecvData));				// DATA��/�޼���ǥ��(�ŷ�)

/////////////////////////////////////////////////////////////////////////////
//	������ó������
/////////////////////////////////////////////////////////////////////////////
	ChoiceTran = TRAN_NULL;										// ��ȸ�ļ��õȰŷ�
	TranResultNGCode = 0;										// ���Ű�� NG�ڵ�
	PbNextTranFlag = FALSE;										// ���� �����ŷ� �뺸�÷���
	DepNextTranFlag = FALSE;									// ���Ӱŷ�_�Ա�_Flag
	PbNextPageFlag = FALSE;										// ���� ���������� �뺸�÷���
	InqNextTranFlag = FALSE;									// ��ȸ ������� �뺸 �÷���
	KCashHostICFlag = FALSE;									// K-CASH Host IC Flag
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->HostLineMode == HOST_ONLINE_MODE)			// HOST_ONLINE_MODE
	{
		if (!SendLength)
			fnAPP_CancelProc(T_PROGRAM);
	}

	if (!m_pDevCmn->fnAPL_CheckHostLine())						// ����üũ
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��۽� : ���]"));
		m_pDevCmn->fnAPL_StackError("7710892", "��Ҽ۽źҰ�");
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "���ó���� �Ұ��մϴ�", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");
		else
			fnAPP_CancelProc(T_MSG, "Transaction is not valid");
	}

	fnAPP_ClearArrDataField(); //Clear
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��۽�]"));
	if (fnAPP_SendData(SendBuffer, SendLength, LINE_SEND_TIME) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��۽� : ���]"));
		m_pDevCmn->fnAPL_StackError("7710992", "��Ҽ۽Žð��ʰ�");	
		fnAPP_CancelProc(T_SENDERROR);
	}


	return T_OK;
}

// SNOD Send Recv
int	CTranCmn::fnAPP_SendRecvHostSnod(int nTranCodeID)
{
	int		ScrNum = 0;
	int		TempLength = 0;
	int		Index  = 0;

/////////////////////////////////////////////////////////////////////////////
//	Line Check
/////////////////////////////////////////////////////////////////////////////

//  Flag Resetó���� �۽����� �ۼ��� ó����� ��������//
//	PbNextTranFlag = FALSE;										// ���� �����ŷ� �뺸�÷���
//	PbNextPageFlag = FALSE;										// ���� ���������� �뺸�÷���
//	InqNextTranFlag = FALSE;									// ��ȸ ������� �뺸 �÷���

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendRecvHostSnod (%d)", nTranCodeID);

	if (!m_pDevCmn->fnAPL_CheckHostLine())						// ����üũ
	{
		m_pDevCmn->fnAPL_StackError("7710891", "�۽źҰ�");
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				fnAPP_CancelProc(T_MSG, "�ŷ��� �Ұ��մϴ�", "������ �̿� �ٶ��ϴ�");
			else
				fnAPP_CancelProc(T_MSG, "Transaction is not valid");
		}
	}


//Kim.Gi.Jin TranCode2
/////////////////////////////////////////////////////////////////////////////
	if(nTranCodeID)
		TranCode2 = nTranCodeID;
/////////////////////////////////////////////////////////////////////////////

	
/////////////////////////////////////////////////////////////////////////////
//	�Ϸù�ȣ ����ó��
/////////////////////////////////////////////////////////////////////////////
	if (!AddSerialFlg)											// �Ϸù�ȣ������
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost::�۽�ó���� �ŷ��Ϸù�ȣ ����ó��");
		AddSerialFlg = TRUE;									// �Ϸù�ȣ������
		m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
	}

/////////////////////////////////////////////////////////////////////////////
//	�۽ź���
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->TranResult = FALSE;								// Host���

	SendLength = 0;												// �۽�Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// �۽�Buffer

/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(601);						// ������ȭ��

	fnAPP_Tran_TxSend(TranCode2);

	if (m_pDevCmn->HostLineMode == HOST_ONLINE_MODE)			// HOST_ONLINE_MODE
	{
		if (!SendLength)
			fnAPP_CancelProc(T_PROGRAM);
	}

/////////////////////////////////////////////////////////////////////////////
//	������ó������
/////////////////////////////////////////////////////////////////////////////
	DepNextTranFlag = FALSE;									// ���Ӱŷ�_�Ա�_Flag
	PbNextTranFlag = FALSE;										// ���� �����ŷ� �뺸�÷���
	PbNextPageFlag = FALSE;										// ���� ���������� �뺸�÷���
	InqNextTranFlag = FALSE;									// ��ȸ ������� �뺸 �÷���
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//	�۽�ó��
/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[SNOD ���� �۽�]");	// �ڷ�����
	fnAPP_SendData(SendBuffer, SendLength, LINE_SEND_TIME);	

	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_PROCESS);// �����ư ������
	
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
		m_pDevCmn->TranResult = TRUE;							// Host���
		return T_OK;											// HOST OK
	}
/////////////////////////////////////////////////////////////////////////////
//	���ź���
/////////////////////////////////////////////////////////////////////////////
	RecvLength = 0;												// ����Length
	memset(RecvBuffer, 0, sizeof(RecvBuffer));					// ����Buffer
	memset(&PPPRecvData, 0, sizeof(PPPRecvData));				// DATA��/���������Ʈ(�ŷ�)
	memset(&PPPRecvDataNon, 0, sizeof(PPPRecvDataNon));				// DATA��/���������Ʈ(�ŷ�) - Non
	memset(&PPPRecvDataKb, 0, sizeof(PPPRecvDataKb));				// DATA��/���������Ʈ(�ŷ�) - KB #N0227
/////////////////////////////////////////////////////////////////////////////
	if (fnAPP_RecvData(RecvBuffer, &RecvLength, LINE_RECV_TIME) != T_OK)		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ȣ��Ʈ ���� ����ó�� Ȯ��(SNOD�������)");
		m_pDevCmn->TranResult = FALSE;
		switch(m_pDevCmn->fnNET_GetDeviceStatus())
		{
		case LINE_CLOSE:
			m_pDevCmn->fnAPL_StackError("8860293", "Line Close");
			break;
		default:
			m_pDevCmn->fnAPL_StackError("8860293", "���Žð��ʰ�");
			break;
		}

		//#0058
		/*
		if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP && (PbTran & MENU_PB) && (TranCode2 == TRANID_4730))
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendRecvHostSnod() ����� �ŷ� TIMEOUT!! �Ҵ� Write!!");
			fnAPD_PbMSWrite();
		}
		*/
		
		fnAPP_CancelProc(T_RECVERROR);

		return T_OK;
	}

	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_FINISH);// �����ư ������
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendRecvHostSnod() ȣ��Ʈ ���� ���ſϷ� Ȯ��(������� ����)");
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
		m_pDevCmn->TranResult = TRUE;							// Host���
		InqNextTranFlag = TRUE;
		PMERecvData.y = 2;
		for(int i=0; i<256; i++)
			PMERecvData.Data[0][i] = ' ';
		for(i=0; i<256; i++)
			PMERecvData.Data[1][i] = ' ';
		return T_OK;											// HOST OK
	}

	fnAPP_Tran_RxRecv(TranCode2);
	return T_OK;
}


// �۽������ۼ�(������ Send)
int CTranCmn::fnAPP_SendMakeManageSend(int RealFlg)
{
	return T_OK;
}


// �ŷ����ڷἳ��
int CTranCmn::fnAPP_SendHostSet(int nTranCode2)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostSet(%04d)", nTranCode2);
	
	switch(nTranCode2)
	{
	case TRANID_0006:
	case TRANID_A010:
	case TRANID_A020:
	case TRANID_A060:
	case TRANID_2190:
		break;
	case TRANID_4310:
	case TRANID_4210:
	case TRANID_4116:
	case TRANID_4330:
		TranSend++;   //Cancel Send Flag
		break;
	case TRANID_7220:
	case TRANID_7230:
	case TRANID_4320:
	case TRANID_4340:
	case TRANID_4350:
		break;
	default:
		break;
	}


if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)                       // ���Ȼ�
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostSet", SendBuffer, SendLength);	


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostSet return (%04d)", nTranCode2);

	return TRUE;
}

/*
// ����IC��������
int CTranCmn::fnAPP_SendMakeICInformation()
{
	BYTE	StrBuff[256];
	int		nIndex = 0;

	memset(StrBuff, ' ', sizeof(StrBuff));

	memcpy(&StrBuff[nIndex], m_pDevCmn->FinanceICInfo.szCSN, 16);
	nIndex += 16;

	fnAPD_FICGetHostTranData();

	memcpy(&StrBuff[nIndex], &m_pDevCmn->FinanceICInfo.HostTranData, 98);
	nIndex += 98;

	memcpy(&SendBuffer[SendLength], StrBuff, nIndex);
	SendLength += nIndex;

	return TRUE;
}
*/
// ��������������
int CTranCmn::fnAPP_SendMakeIRDInformation()
{
	BYTE	StrBuff[256];
	int		nIndex = 0;

	memset(StrBuff, ' ', sizeof(StrBuff));

	memcpy(&StrBuff[nIndex], MakeUnPack(m_pDevCmn->IRDAInfo.CSN, 8), 16);
	nIndex += 16;												// ī���Ϸù�ȣ(16)

	memcpy(&StrBuff[nIndex], MakeUnPack(m_pDevCmn->IRDAInfo.CardRandom, 16), 32);
	nIndex += 32;												// ī�峭��(32)

	memcpy(&StrBuff[nIndex], MakeUnPack(m_pDevCmn->IRDAInfo.TerminalRandom, 16), 32);
	nIndex += 32;												// �͹̳γ���(32)

	memcpy(&StrBuff[nIndex], MakeUnPack(m_pDevCmn->IRDAInfo.CipherPW, 16), 32);
	nIndex += 32;												// ��ȣȭ��й�ȣ(32)

	memcpy(&StrBuff[nIndex], MakeUnPack((LPBYTE)&m_pDevCmn->IRDAInfo.FCI[33], 1), 2);
	nIndex += 2;												// FCI �� KeyVersion(2)

	memcpy(&SendBuffer[SendLength], StrBuff, nIndex);
	SendLength += nIndex;

	return TRUE;
}

// EMV��������
int CTranCmn::fnAPP_SendMakeEMVInformation()
{
	BYTE	StrBuff[1024];
	int		nIndex = 0;

	memset(StrBuff, ' ', sizeof(StrBuff));

	if (CancelWithProcFlg)
	{
		nIndex = 147;
		m_pDevCmn->fnEMV_GetHostTranData(EMV_SENDHOST_REVERSAL);
		memcpy(&StrBuff[nIndex], m_pDevCmn->m_EMVszData, 12);
		nIndex += 12;
	}
	else
	{
		if (!TranSend)
		{
			if (fnAPD_EMVSendMakeProc())
			{
				fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ��ְ� �߻��Ͽ����ϴ�.", "â���� �����Ͽ� �ֽñ� �ٶ��ϴ�.");
			}
		}
		else
		{
			m_pDevCmn->fnEMV_GetHostTranData(EMV_SENDHOST_ARQC);
		}

		memcpy(&StrBuff[nIndex], m_pDevCmn->m_EMVszData, 156);
		nIndex += 156;
	}

	nIndex = 192;

	memcpy(&SendBuffer[SendLength], StrBuff, nIndex);
	SendLength += nIndex;

	return TRUE;
}



// �����ڵ�����
int	CTranCmn::fnAPP_SendMakeDataSetErrorCode()
{
	return TRUE;
}

// ��ǥ�ż�����
int	CTranCmn::fnAPP_SendMakeDataSetCheckCount()
{
	char	StrBuff[16] = {0,};

	sprintf(StrBuff, "%02d", Asc2Int(Accept.CheckMoney, 10));
	memcpy(&SendBuffer[SendLength], StrBuff, 2);
	SendLength += 2;
	SendBuffer[SendLength++] = 0x1e;

	if(Asc2Int(Accept.CheckMoney, 15) > 0)
	{
		if(m_pProfile->UCM.CrntOutCheckCnt > 0)
		{
			memcpy(&SendBuffer[SendLength], m_pProfile->UCM.BankNo, 2);
			SendLength += 2;
			memcpy(&SendBuffer[SendLength], m_pProfile->UCM.CrntOutCheckNo, 8);
			SendLength += 8;
		}
		else
		{
			memcpy(&SendBuffer[SendLength], m_pProfile->UCM.BankNo2, 2);
			SendLength += 2;
			memcpy(&SendBuffer[SendLength], m_pProfile->UCM.CrntOutCheckNo2, 8);
			SendLength += 8;
		}
		memcpy(Accept.CheckNumber, &SendBuffer[SendLength-10], 10);
	}
	else
	{
		memcpy(&SendBuffer[SendLength], "          ", 10);
		SendLength += 10;
	}

	return TRUE;
}



// �Աݼ�ǥ��������
int	CTranCmn::fnAPP_SendMakeDataSetDepCheckInfo()
{
	BYTE	StrBuff[400] = {0,};
	int		nIndex = 0;

	memset(StrBuff, ' ', sizeof(StrBuff));

	sprintf((char*)&StrBuff[0], "%02d", CheckInfoCnt);			// ��ǥ�ż�
	nIndex += 2;
	StrBuff[nIndex++] = 0x1e;									// ������
	for (int i = 0; i < 10; i++)
	{
		memcpy(&StrBuff[nIndex], CheckInfoData[i].ReadData, 8);	// ��ǥ��ȣ
		nIndex += 8;
		memcpy(&StrBuff[nIndex], CheckInfoData[i].Bank, 2);		// �����ڵ�
		nIndex += 2;
		memcpy(&StrBuff[nIndex], CheckInfoData[i].Branch, 4);	// ������ȣ
		nIndex += 4;
		memcpy(&StrBuff[nIndex], CheckInfoData[i].AccountNum, 6);	// ���¹�ȣ
		nIndex += 6;
		memcpy(&StrBuff[nIndex], CheckInfoData[i].Kind, 2);		// ����
		nIndex += 2;
		memcpy(&StrBuff[nIndex], CheckInfoData[i].Amount, 10);	// ����
		nIndex += 10;
		StrBuff[nIndex++] = 0x20;								// ������
	}

	memcpy(&SendBuffer[SendLength], StrBuff, nIndex);
	SendLength += nIndex;
	
	return TRUE;
}

// �۱�(��ü)��������
int	CTranCmn::fnAPP_SendMakeDataSetTransferInfo()
{
	BYTE	StrBuff[128] = {0,};
	int		nIndex = 0;

	memset(StrBuff, ' ', sizeof(StrBuff));

	sprintf((char*)&StrBuff[0], "%02d", Accept.InAccountNumSize);		// ��ü������¹�ȣ����
	nIndex += 2;
	StrBuff[nIndex++] = 0x1e;												// ������
	memcpy(&StrBuff[nIndex], Accept.InAccountNum, sizeof(Accept.InAccountNum));	// ��ü������¹�ȣ
	nIndex += sizeof(Accept.InAccountNum);
	if(TranProc == TRAN_TRANS)											// ��ü
	{
		StrBuff[nIndex++] = 0x1e;											// ������
		if(memcmp(Accept.BankNum, BANKJJ2, 3) == 0)	// ��->��
		{
			memcpy(&StrBuff[nIndex], BANKJJ2, 3);						// ������ü
			nIndex += 2;
		}
		else										// ��->Ÿ
		{
			memcpy(&StrBuff[nIndex], Accept.BankNum, 2);				// ��ü�����ڵ�
			nIndex += 2;
		}
	}
	StrBuff[nIndex++] = 0x1e;
	memcpy(&StrBuff[nIndex], &RecvComm.TransData[139], 20);
	nIndex += 20;
	memcpy(&SendBuffer[SendLength], StrBuff, nIndex);
	SendLength += nIndex;

	return TRUE;
}

// ������ȸ��������
int	CTranCmn::fnAPP_SendMakeDataSetNextInqInfo()
{
	memcpy(&SendBuffer[SendLength], Accept.NextInqCondition, sizeof(Accept.NextInqCondition));
	SendLength += sizeof(Accept.NextInqCondition);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPP_RecvHost()
{
	int		Index  = 0;
	int		nOpenFlag = FALSE;
	int		i = 0, j = 0;
	int		nRecvWaitSec = LINE_RECV_TIME;						// �ۼ��Ŵ��ð�
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost()");
	if ((m_pDevCmn->TranStatus == TRAN_READY)	||				// ����/�غ���� 30�ʴ��ó��
		(m_pDevCmn->TranStatus == TRAN_OPEN)	)
		nRecvWaitSec = LINE_OPCL_TIME;

	if (fnAPP_RecvData(RecvBuffer, &RecvLength, nRecvWaitSec) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�ŷ����� : �ð��ʰ�"));
		m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_FINISH);// �����ư ������
		if ((m_pDevCmn->TranStatus == TRAN_OPEN)	||
			(m_pDevCmn->TranStatus == TRAN_READY)	||
			(m_pDevCmn->TranStatus == TRAN_STATUS)	)
		{
			fnAPP_CancelProc(T_RECVERROR);
		}
		else
		{
			if(TranCode2 == TRANID_4410)
				m_pDevCmn->ForceDownFlag = TRUE;

			if (m_pDevCmn->TranStatus == TRAN_TOTAL || m_pDevCmn->TranStatus == TRAN_CASH_REG)
				m_pDevCmn->TranResult = FALSE;					
			else
			{
				switch(m_pDevCmn->fnNET_GetDeviceStatus())
				{
				case LINE_CLOSE:
					m_pDevCmn->fnAPL_StackError("8860293", "Line Close");
					break;
				default:
					m_pDevCmn->fnAPL_StackError("8860293", "���Žð��ʰ�");
					break;
				}

				if (TranCode == TC_WITHDRAW || TranCode == TC_SVCWITH)
				{
					fnAPP_WithErrSumProc(0);				
				}
				else
				if (TranCode == TC_DEPOSIT)
				{
					fnAPP_DepErrSumProc(0);
				}

				fnAPP_CancelProc(T_RECVERROR);
			}
		}
	}
	else m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ�����]"));

	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_FINISH);// �����ư ������

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
		m_pDevCmn->TranResult = TRUE;							// Host���
		InqNextTranFlag = TRUE;
		PMERecvData.y = 1;
		if (TranCode == TC_INQWIDTH || TranCode == TC_INQDEP)	// ��ݰ��ɾ���ȸ
			RecvComm.TransData[83] = '1';
		
		PMERecvData.y = 2;
		for(int i=0; i<256; i++)
			PMERecvData.Data[0][i] = '1';
		for(i=0; i<256; i++)
			PMERecvData.Data[1][i] = '2';
		return T_OK;											// HOST OK
	}
/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost : m_pDevCmn->TranStatus - %d", m_pDevCmn->TranStatus);
	switch (m_pDevCmn->TranStatus)
	{
	case TRAN_READY:											// �غ��ó��
		m_pDevCmn->TranResult = TRUE;
		break;
	case TRAN_STATUS:
	case TRAN_OPEN:												// ����ó��
	case TRAN_TRAN:												// �ŷ�ó��
	case TRAN_TOTAL:											// ��������
	case TRAN_SUBTOTAL:											// ������ȸ
	case TRAN_CASH_REG:											// ������ݵ��
	case TRAN_CHECK_REG:										// ��ݼ�ǥ���
	case TRAN_CHECK_UNREG:										// ��ݼ�ǥ����
	case TRAN_MSFINGER_CONFIRM:									// �������� Flow ����
		fnAPP_Tran_RxRecv(TranCode2);
		break;
	}

	return T_OK;
}

// �����ؼ�
int CTranCmn::fnAPP_RecvAnal()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal : ���ŵ���Ÿ �м�����");
	BYTE	StrBuff[NETBUFFSIZE] = {0,};
	int		nReturn = FALSE;
	int		nIndex = 0, i = 0, j = 0;

	return T_OK;
}

// DATA��/������Ʈ(�ŷ�)
int CTranCmn::fnAPP_RecvPSPGet(void* RecvArea, PSPRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		HanFlg = FALSE, HanCnt = 0;
	BYTE	HanLeft = 0, HanRight = 0;
	char	szTempBuff[256] = {0,};
	char	szTempBuff1[256] = {0,};
	char	szTempBuff3[256] = {0,};
	char	szTempBuff4[256] = {0,};
	int		ii = 0, nSize = 0;
	CString strTempBuff;

for (ii = 0; ii < PMERecvData.y; ii++)
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PMERecvData[%d][%s]", ii, PMERecvData.Data[ii]);

	if (TranCode == TC_TOTAL)
	{															// �����հ�
/*///
		memset(szTempBuff, NULL, sizeof(szTempBuff));
		memcpy(szTempBuff, &RecvHead.CardData[54], 10);
		sprintf((char *)DataArea->Data[DataArea->y++], "��������(WC) : %sõ��", Asc2Amt(szTempBuff, 10, 13).GetBuffer(0));
		memset(szTempBuff, NULL, sizeof(szTempBuff));
		memcpy(szTempBuff, &RecvHead.CardData[64], 10);
		sprintf((char *)DataArea->Data[DataArea->y++], "��������(WS) : %sõ��", Asc2Amt(szTempBuff, 10, 13).GetBuffer(0));
		memset(szTempBuff, NULL, sizeof(szTempBuff));
		memcpy(szTempBuff, &RecvHead.CardData[74], 10);
		sprintf((char *)DataArea->Data[DataArea->y++], "�Ա�����(DC) : %sõ��", Asc2Amt(szTempBuff, 10, 13).GetBuffer(0));
		memset(szTempBuff, NULL, sizeof(szTempBuff));
		memcpy(szTempBuff, &RecvHead.CardData[84], 10);
		sprintf((char *)DataArea->Data[DataArea->y++], "�Ա�����(DS) : %sõ��", Asc2Amt(szTempBuff, 10, 13).GetBuffer(0));
		memset(szTempBuff, NULL, sizeof(szTempBuff));
		memcpy(szTempBuff, &RecvHead.CardData[94], 10);
		sprintf((char *)DataArea->Data[DataArea->y++], "�Ա�Ÿ��(DT) : %sõ��", Asc2Amt(szTempBuff, 10, 13).GetBuffer(0));
///*/
	}

	DataArea->Ey = DataArea->y;

	if (DataArea->Ey)
		DataArea->ParaLength = DataArea->Ey;

	return TRUE;
}

// ������Move
int CTranCmn::fnAPP_RecvPSPMove(BYTE Data, PSPRecv* pDataArea, int Length)
{
	int		i;

	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->y >= itemof(pDataArea->Data)) ||		// �Ѱ����
			(pDataArea->x >= itemof(pDataArea->Data[0])))
			break;
		
		pDataArea->Data[pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line �ʱ⼳��
		{
			pDataArea->By = pDataArea->y;
			pDataArea->Ey = pDataArea->y;
		}
		pDataArea->ParaLength += Length;						// �ڷ��������ϱ�
		if (pDataArea->y < pDataArea->By)						// Find Begin Line
			pDataArea->By = pDataArea->y;
		if (pDataArea->y > pDataArea->Ey)						// Find End Line
			pDataArea->Ey = pDataArea->y;
	}
	return T_OK;
}
/*
 	
[2012/09/02 11:14:01 607] Name : ����ff ���� ������ sbuff , Length : 360
00001:00016 31 30 30 30 31 32 30 37  31 37 20 20 20 20 20 20  1000120717      
00017:00032 20 20 20 20 32 30 30 31  32 5c 30 30 30 30 30 30      20012\000000
00033:00048 30 30 30 30 32 34 30 30  33 30 30 30 be e7 bc ba  000024003000....
00049:00064 b3 f3 c7 f9 c7 cf b3 aa  20 20 20 20 34 30 30 31  ........    4001
00065:00080 32 5c 30 30 30 30 30 30  30 30 30 30 34 37 30 34  2\00000000004704
00081:00096 35 30 30 30 4e 48 42 43  c3 bc c5 a9 20 20 20 20  5000NHBC....    
00097:00112 20 20 20 20 36 30 33 30  32 33 34 20 20 20 20 20      6030234     
00113:00128 20 20 20 20 20 20 20 20  31 30 30 30 31 32 30 37          10001207
00129:00144 32 30 20 20 20 20 20 20  20 20 20 20 32 30 30 30  20          2000
00145:00160 a3 b1 a3 b1 b9 f8 b0 a1  a1 a1 a1 a1 20 20 20 20  ............    
00161:00176 33 30 30 31 32 5c 30 30  30 30 30 30 30 30 30 31  30012\0000000001
00177:00192 31 39 30 30 34 30 30 31  32 5c 30 30 30 30 30 30  190040012\000000
00193:00208 30 30 30 31 36 36 30 34  35 30 30 30 50 47 b0 e1  000166045000PG..
00209:00224 c1 a6 c8 af ba d2 20 20  20 20 20 20 36 30 33 30  ......      6030
00225:00240 30 30 37 20 20 20 20 20  20 20 20 20 20 20 20 20  007             
00241:00256 39 30 30 30 b0 a1 b1 ee  bf ee 20 bf b5 be f7 c1  9000...... .....
00257:00272 a1 c0 b8 b7 ce 20 b9 e6  b9 ae c7 cf bf a9 20 c5  ..... ........ .
00273:00288 eb c0 e5 20 c0 cc bf f9  b9 df b1 de c0 bb 20 b9  ... .......... .
00289:00304 de c0 b8 bd c3 b1 e2 20  b9 d9 b6 f8 b4 cf b4 d9  ....... ........
00305:00320 2e 20 20 20 20 20 20 20  20 20 20 20 20 20 20 20  .               
00321:00336 20 20 20 20 20 20 20 20  20 20 20 20 20 20 20 20                  
00337:00352 20 20 20 20 20 20 20 20  20 20 20 20 20 20 20 20                  
00353:00360 20 20 20 20 20 20 20 20                                           


[2012/09/18 21:32:45:876 TranEnCryptProc(14760)] Name : fnAPP_TRAN_DEC_011 Res , Length : 120
00000:00016 31 30 30 30 31 32 30 39  31 38 20 20 20 20 20 20 1000120918      
00016:00032 20 20 20 20 32 30 30 31  32 5c 30 30 30 30 30 30     20012\000000
00032:00048 30 30 30 33 30 30 30 30  33 30 30 30 20 4e 43 49 000300003000 NCI
00048:00064 39 38 34 30 20 20 20 20  20 20 20 20 34 30 30 31 9840        4001
00064:00080 32 5c 30 30 30 30 30 30  30 33 30 32 39 39 30 30 2\00000003029900
00080:00096 35 30 30 30 c5 eb c0 e5  b3 aa c0 cc bd ba 20 20 5000..........  
00096:00112 20 20 20 20 36 30 30 30  4e 43 49 39 38 34 20 20     6000NCI984  
00112:00128 20 20 20 20 20 20 20 20     

 */
// DATA��/���������Ʈ(�ŷ�)
int CTranCmn::fnAPP_NONGHYUP_RecvPPPGet1(void* RecvArea, PPPRecvNon* DataArea, int SubPatton, int FindLength)
{
	int nSsP = SubPatton;
	BYTE*	pRecvArea	= (BYTE*)RecvArea;
	int		Cnt = 0, ElementCnt = 0, Startcnt = 0;
	int		PrintChar = 0;				// 1 : ����, 2 : ����
	int		nMoveCnt = 0;
	int		nGSpaceCnt = 0;
	char	szGetPbBuff[1024];
	BYTE	hanFlag = 0;
	int		nTCnt = 0;
	int		nSlen = 0;
	int		nCh = 0;
	BYTE	TotElement[120];
	BYTE	Element1[21];
	BYTE	Element2[21];
	BYTE	Element3[21];
	BYTE	Element4[21];
	BYTE	Element5[21];
	BYTE	Element6[21];
	char	szEditElement[21];
	BYTE	EditElement3[21];
	BYTE	EditElement4[21];
	memset(Element1, 0x20, sizeof(Element1));
	memset(Element2, 0x20, sizeof(Element2));
	memset(Element3, 0x20, sizeof(Element3));
	memset(Element4, 0x20, sizeof(Element4));
	memset(Element5, 0x20, sizeof(Element5));
	memset(Element6, 0x20, sizeof(Element6));
	memset(szEditElement, 0, sizeof(szEditElement));
	memset(EditElement3, 0, sizeof(Element4));
	memset(EditElement4, 0, sizeof(Element5));
	CString cstrGet("");


	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 SubPatton[%d][%d] y[%d] p[%d]", SubPatton, FindLength, DataArea->y, DataArea->p);
	
	//#H0066
	if(pRecvArea[0] != 0x39)
	{
		memcpy(Element1, &pRecvArea[Startcnt+4], 16);	
		Startcnt += 20;

		if (pRecvArea[Startcnt + 3] == 0x30)			//�����̸�
		{
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);
			memcpy(&Element2[nGSpaceCnt], &pRecvArea[Startcnt+4], 16 - nGSpaceCnt);	
	//		memset(szEditElement, ' ', sizeof(szEditElement));
	//		strRTrim((char *)Element2, szEditElement, 16 - nGSpaceCnt);
	//		memcpy(Element2, szEditElement, strlen(szEditElement));
		}
		else
		if (pRecvArea[Startcnt + 3] == 0x31)			//�����̸�)
		{
			// \ ��ȣ�� �ִ��� Ȯ��
			// �ݾ� ���� flag -> 0�� �����ϰ�, ��ȿ �������İ� �ݾ׿� �ش��.
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);

			memcpy(Element2, Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);			// ����ݾ�
			cstrGet.Format("%s", Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);
			cstrGet.TrimLeft();
			cstrGet.TrimRight();

			memcpy(&Element2[14 - cstrGet.GetLength()], cstrGet.GetBuffer(0), cstrGet.GetLength());	
			//�������� *, -, \ //
			memcpy(&Element2[14 - cstrGet.GetLength() - 1], &pRecvArea[Startcnt+4+1], 1);	
			
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 - 0");

		Startcnt += 20;
		if (pRecvArea[Startcnt + 3] == 0x30)			//�����̸�
		{
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);
			memcpy(&Element3[nGSpaceCnt], &pRecvArea[Startcnt+4], 16 - nGSpaceCnt);		
		}
		else
		if (pRecvArea[Startcnt + 3] == 0x31)			//�����̸�)
		{
			// \ ��ȣ�� �ִ��� Ȯ��
			// �ݾ� ���� flag -> 0�� �����ϰ�, ��ȿ �������İ� �ݾ׿� �ش��.
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);

			memcpy(Element3, Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);			// ����ݾ�
			cstrGet.Format("%s", Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);
			cstrGet.TrimLeft();
			cstrGet.TrimRight();

			memcpy(&Element3[14 - cstrGet.GetLength()], cstrGet.GetBuffer(0), cstrGet.GetLength());	
			//�������� *, -, \ //
			memcpy(&Element3[14 - cstrGet.GetLength() - 1], &pRecvArea[Startcnt+4+1], 1);	
			
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 - 1");

		Startcnt += 20;
		if (pRecvArea[Startcnt + 3] == 0x30)			//�����̸�
		{
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);
			memcpy(&Element4[nGSpaceCnt], &pRecvArea[Startcnt+4], 16 - nGSpaceCnt);	
		}
		else
		if (pRecvArea[Startcnt + 3] == 0x31)			//�����̸�)
		{
			// \ ��ȣ�� �ִ��� Ȯ��
			// �ݾ� ���� flag -> 0�� �����ϰ�, ��ȿ �������İ� �ݾ׿� �ش��.
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);

			memcpy(Element4, Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);			// ����ݾ�
			cstrGet.Format("%s", Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);
			cstrGet.TrimLeft();
			cstrGet.TrimRight();

			memcpy(&Element4[14 - cstrGet.GetLength()], cstrGet.GetBuffer(0), cstrGet.GetLength());	
			//�������� *, Element4, \ //
			memcpy(&Element4[14 - cstrGet.GetLength() - 1], &pRecvArea[Startcnt+4+1], 1);	
			
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 - 2");

		Startcnt += 20;
		if (pRecvArea[Startcnt + 3] == 0x30)			//�����̸�
		{
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);
			memcpy(&Element5[nGSpaceCnt], &pRecvArea[Startcnt+4], 16 - nGSpaceCnt);	
		}
		else
		if (pRecvArea[Startcnt + 3] == 0x31)			//�����̸�)
		{
			// \ ��ȣ�� �ִ��� Ȯ��
			// �ݾ� ���� flag -> 0�� �����ϰ�, ��ȿ �������İ� �ݾ׿� �ش��.
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);

			memcpy(Element5, Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);			// ����ݾ�
			cstrGet.Format("%s", Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);
			cstrGet.TrimLeft();
			cstrGet.TrimRight();

			memcpy(&Element5[14 - cstrGet.GetLength()], cstrGet.GetBuffer(0), cstrGet.GetLength());	
			//�������� *, -, \ //
			memcpy(&Element5[14 - cstrGet.GetLength() - 1], &pRecvArea[Startcnt+4+1], 1);	
			
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 - 3");

		Startcnt += 20;
		if (pRecvArea[Startcnt + 3] == 0x30)			//�����̸�
		{
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);
			memcpy(&Element6[nGSpaceCnt], &pRecvArea[Startcnt+4], 16 - nGSpaceCnt);	
		}
		else
		if (pRecvArea[Startcnt + 3] == 0x31)			//�����̸�)
		{
			// \ ��ȣ�� �ִ��� Ȯ��
			// �ݾ� ���� flag -> 0�� �����ϰ�, ��ȿ �������İ� �ݾ׿� �ش��.
			nGSpaceCnt = Asc2Int(&pRecvArea[Startcnt + 1], 2);

			memcpy(Element6, Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);			// ����ݾ�
			cstrGet.Format("%s", Asc2Amt(&pRecvArea[Startcnt+4+2], 14, 13).GetBuffer(0), 13);
			cstrGet.TrimLeft();
			cstrGet.TrimRight();

			memcpy(&Element6[14 - cstrGet.GetLength()], cstrGet.GetBuffer(0), cstrGet.GetLength());	
			//�������� *, -, \ //
			memcpy(&Element6[14 - cstrGet.GetLength() - 1], &pRecvArea[Startcnt+4+1], 1);	
			
		}

		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E1", (char *)Element1, 20);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E2", (char *)Element2, 20);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E3", (char *)Element3, 20);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E4", (char *)Element4, 20);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E5", (char *)Element5, 20);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 - E6", (char *)Element6, 20);

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPGet1 - 0 - Sub[%d] Pos[%x]", nSsP, pRecvArea[23]);
		switch(nSsP) 
		{
		case 0:
			nCh = FindChar(Element1, 16, ' ');
			if(nCh == 6)
			{
				//6	13	13	14	10	6 -> �ݾ� (14Byte) - ����
				if(pRecvArea[23] == 0x30)
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 6);
					nSlen +=6;				
					memcpy(&TotElement[nSlen], &Element2[0], 13); //���ں��� (�������� : *, -, \ Space) = +1 <- ���� �ʿ�
					nSlen +=13;				
	//				nSlen +=1;				
					memcpy(&TotElement[nSlen], &Element3[1], 13);   //����
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element4, 14);       //����
					nSlen +=14;				
					memcpy(&TotElement[nSlen], Element5, 10);     //����
					nSlen +=10;				
	//				nSlen +=1;				
					memcpy(&TotElement[nSlen], Element6, 6);	    //����

	//				sprintf((char *)TotElement, "%6.6s%13.13s%13.13s%14.14s%10.10s%6.6s", Element1,&Element2[0],&Element3[1],Element4,Element5,Element6);
				}
				else
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 6);
					nSlen +=6;				
					memcpy(&TotElement[nSlen], &Element2[1], 13); //����
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element3[0], 13); //����
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element4, 14);     //����
					nSlen +=14;				
					memcpy(&TotElement[nSlen], Element5, 10);     //����
					nSlen +=10;				
					memcpy(&TotElement[nSlen], Element6, 6);	  //����
	//				sprintf((char *)TotElement, "%6.6s%13.13s%13.13s%14.14s%10.10s%6.6s", Element1,&Element2[1],&Element3[0],Element4,Element5,Element6);
				}
			}
			else
			if(nCh == 8)
			{
				//8	12	12	13	10	6
				if(pRecvArea[23] == 0x30)
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 8);
					nSlen +=8;				
					memcpy(&TotElement[nSlen], &Element2[0], 12);
					nSlen +=12;				
					memcpy(&TotElement[nSlen], &Element3[2], 12);
					nSlen +=12;				
					memcpy(&TotElement[nSlen], &Element4[1], 13); //#H0044
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element5, 10);
					nSlen +=10;				
					memcpy(&TotElement[nSlen], Element6, 6);	

	//				sprintf((char *)TotElement, "%8.8s%8.8s%14.14s%14.14s%12.12s%6.6s", Element1,&Element2[0],Element3,Element4,Element5,Element6);
				}
				else
				{

					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 8);
					nSlen +=8;				
					memcpy(&TotElement[nSlen], &Element2[2], 12);
					nSlen +=12;				
					memcpy(&TotElement[nSlen], &Element3[0], 12);
					nSlen +=12;				
					memcpy(&TotElement[nSlen], &Element4[1], 13); //#H0044
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element5, 10);
					nSlen +=10;				
					memcpy(&TotElement[nSlen], Element6, 6);	

	//				sprintf((char *)TotElement, "%8.8s%8.8s%14.14s%14.14s%12.12s%6.6s", Element1,&Element2[6],Element3,Element4,Element5,Element6);
				}

			}
			else
			if(nCh == 10)
			{
				//10	13	9	7	16	7
				if(pRecvArea[23] == 0x30)
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 10);
					nSlen +=10;				
					memcpy(&TotElement[nSlen], &Element2[0], 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element3[5], 9);
					nSlen +=9;				
					memcpy(&TotElement[nSlen], &Element4[7], 7);
					nSlen +=7;				
					memcpy(&TotElement[nSlen], Element5, 16);
					nSlen +=16;				
					memcpy(&TotElement[nSlen], Element6, 7);	

	//				sprintf((char *)TotElement, "%10.10s%13.13s%9.9s%7.7s%16.16s%7.7s", Element1,&Element2[0],&Element3[5],&Element4[7],Element5,Element6);
				}
				else
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 10);
					nSlen +=10;				
					memcpy(&TotElement[nSlen], &Element2[1], 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element3[0], 9);
					nSlen +=9;				
					memcpy(&TotElement[nSlen], &Element4[7], 7);
					nSlen +=7;				
					memcpy(&TotElement[nSlen], Element5, 16);
					nSlen +=16;				
					memcpy(&TotElement[nSlen], Element6, 7);	

	//				sprintf((char *)TotElement, "%10.10s%13.13s%9.9s%7.7s%16.16s%7.7s", Element1,&Element2[1],&Element3[0],&Element4[7],Element5,Element6);
				}

			}
			else
			if(nCh == 13)
			{
				//13	13	10	13	6	8
				if(pRecvArea[23] == 0x30)
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element2[0], 13); //����
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element3[4], 10);
					nSlen +=10;				
					memcpy(&TotElement[nSlen], &Element4[1], 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element5, 6);
					nSlen +=6;				
					memcpy(&TotElement[nSlen], Element6, 8);
					
	//				sprintf((char *)TotElement, "%13.13s%13.13s%10.10s%13.13s%6.6s%8.8s", Element1,&Element2[0],&Element3[4],&Element4[1],Element5,Element6);
				}
				else
				{
					nSlen =0;
					memcpy(&TotElement[nSlen], Element1, 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element2[1], 13); //���� - 
					nSlen +=13;				
					memcpy(&TotElement[nSlen], &Element3[0], 10); //����
					nSlen +=10;				
					memcpy(&TotElement[nSlen], &Element4[1], 13);
					nSlen +=13;				
					memcpy(&TotElement[nSlen], Element5, 6);
					nSlen +=6;				
					memcpy(&TotElement[nSlen], Element6, 8);

	//				sprintf((char *)TotElement, "%13.13s%13.13s%10.10s%13.13s%6.6s%8.8s", Element1,&Element2[1],&Element3[0],&Element4[1],Element5,Element6);
				}

			}
			break;
		case 1:
			//8	14	14	12	6	8
			nSlen =0;
			memcpy(&TotElement[nSlen], Element1, 8);
			nSlen +=8;				
			memcpy(&TotElement[nSlen], &Element2[1], 14); //���� - 
			nSlen +=14;				
			memcpy(&TotElement[nSlen], &Element3[0], 14); //����
			nSlen +=14;				
			memcpy(&TotElement[nSlen], &Element4[2], 12);
			nSlen +=12;				
			memcpy(&TotElement[nSlen], Element5, 6);
			nSlen +=6;				
			memcpy(&TotElement[nSlen], Element6, 8);

	//		sprintf((char *)TotElement, "%8.8s%14.14s%14.14s%12.12s%6.6s%8.8s", Element1,Element2,Element3,&Element4[2],Element5,Element6);
			break;
		case 3:
			//8	13	13	14	8	6
			if(pRecvArea[23] == 0x30)
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[0], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], &Element3[1], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 6);
				nSlen +=6;				
				memcpy(&TotElement[nSlen], Element6, 8);
	//			sprintf((char *)TotElement, "%8.8s%13.13s%13.13s%14.14s%8.8s%6.6s", Element1,&Element2[0],&Element3[1],Element4,Element5,Element6);
			}
			else
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[1], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], &Element3[0], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 6);
				nSlen +=6;				
				memcpy(&TotElement[nSlen], Element6, 8);

	//			sprintf((char *)TotElement, "%8.8s%13.13s%13.13s%14.14s%8.8s%6.6s", Element1,&Element2[1],&Element3[0],Element4,Element5,Element6);
			}
			break;
		case 4:
			//8	12	12	13	10	6
			if(pRecvArea[23] == 0x30)
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[0], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], &Element3[2], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], &Element4[1], 13);
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element5, 10);
				nSlen +=10;				
				memcpy(&TotElement[nSlen], Element6, 6);

	//			sprintf((char *)TotElement, "%8.8s%12.12s%12.12s%13.13s%10.10s%6.6s", Element1,&Element2[0],&Element3[2],&Element4[1],Element5,Element6);
			}
			else
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[2], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], &Element3[0], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], Element4, 13);
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element5, 10);
				nSlen +=10;				
				memcpy(&TotElement[nSlen], Element6, 6);

	//			sprintf((char *)TotElement, "%8.8s%12.12s%12.12s%13.13s%10.10s%6.6s", Element1,&Element2[2],&Element3[0],&Element4[1],Element5,Element6);
			}
			break;
		case 9:
			//8	12	14	15	6	8
			if(pRecvArea[23] == 0x30)
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[0], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], &Element3[0], 14); 
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 6);
				nSlen +=6;				
				memcpy(&TotElement[nSlen], Element6, 8);

	//			sprintf((char *)TotElement, "%8.8s%12.12s%14.14s %14.14s%6.6s%8.8s", Element1,&Element2[0],Element3,Element4,Element5,Element6);
			}
			else
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[2], 12); 
				nSlen +=12;				
				memcpy(&TotElement[nSlen], &Element3[0], 14); 
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 6);
				nSlen +=6;				
				memcpy(&TotElement[nSlen], Element6, 8);

	//			sprintf((char *)TotElement, "%8.8s%12.12s%14.14s %14.14s%6.6s%8.8s", Element1,&Element2[2],Element3,Element4,Element5,Element6);
			}
			break;
		case 90:
			//6	13	13	14	10	6
			if(pRecvArea[23] == 0x30)
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[0], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], &Element3[1], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 10);
				nSlen +=10;				
				memcpy(&TotElement[nSlen], Element6, 6);

	//			sprintf((char *)TotElement, "%6.6s%13.13s%13.13s%14.14s%10.10s%6.6s", Element1,&Element2[0],&Element3[1],Element4,Element5,Element6);
			}
			else
			{
				nSlen =0;
				memcpy(&TotElement[nSlen], Element1, 8);
				nSlen +=8;				
				memcpy(&TotElement[nSlen], &Element2[1], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], &Element3[0], 13); 
				nSlen +=13;				
				memcpy(&TotElement[nSlen], Element4, 14);
				nSlen +=14;				
				memcpy(&TotElement[nSlen], Element5, 10);
				nSlen +=10;				
				memcpy(&TotElement[nSlen], Element6, 6);

	//			sprintf((char *)TotElement, "%6.6s%13.13s%13.13s%14.14s%10.10s%6.6s", Element1,&Element2[1],&Element3[0],Element4,Element5,Element6);
			}
			break;
		default:
			break;
		}
	}
	else
	if(pRecvArea[0] == 0x39)
	{
		cstrGet.Format("%114.114s", &pRecvArea[4]);
		cstrGet.TrimRight();
		sprintf((char *)TotElement, "%s", cstrGet);
	}
	else
	{
		cstrGet.Format("%114.114s", &pRecvArea[4]);
		cstrGet.TrimRight();
		sprintf((char *)TotElement, "%s", cstrGet);
	}
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4", (char *)TotElement, 120);
//	for(int ii=0; ii< strlen((char *)TotElement); )
	nCh = FindChar(TotElement, 120, NULL);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-4 Len = (%d)", nCh);
	for(int ii=0; ii < nCh; )
	{
		if((BYTE)TotElement[Cnt+ii] >= 0x80)			// �ѱ۽���ó��
		{
			if(hanFlag != 1)							// ��������
			{
				memcpy(&szGetPbBuff[nTCnt], PbHanStart, 2);
				nTCnt += 2;
			}
			memcpy(&szGetPbBuff[nTCnt], &TotElement[Cnt+ii], 2);
			ii += 2;
			nTCnt += 2;
			hanFlag = 1;
		}
		else											// �ѱ۳�ó��
		{
			if(hanFlag == 1)							// �ѱۻ���
			{
				memcpy(&szGetPbBuff[nTCnt], PbHanEnd, 2);
				nTCnt += 2;
				hanFlag = 0;
			}
/*
			if ((TotElement[ii] == ' ') ||			// ' '
				(TotElement[ii] == ',') ||			// ','
				(TotElement[ii] == '.') ||			// '.'
				(TotElement[ii] == '\''))			// '''
			{	
//				if (TotElement[ii] == ' ')			// ' '
//					TotElement[ii] = PbHalfSpace;
				if (TotElement[ii] == ',')			// ','
					TotElement[ii] = PbHalfcomma;
				if (TotElement[ii] == '.')			// '.'
					TotElement[ii] = PbHalfperiod;
				if (TotElement[ii] == '\'')			// '''
					TotElement[ii] = PbHalfapostrophe;
			}
*/
			szGetPbBuff[nTCnt++] = TotElement[Cnt+ii];
			ii += 1;
		}
	}
	
	memcpy(&DataArea->Data[DataArea->p][(DataArea->y)][DataArea->x], szGetPbBuff, nTCnt);
	return T_OK;
}

int CTranCmn::fnAPP_NONGHYUP_RecvPPPGet(void* RecvArea, PPPRecvNon* DataArea, int FindLength)
{
	BYTE*	pRecvArea	= (BYTE*)RecvArea;
	int		Cnt = 0, ResultCnt = 0;
	int		HanFlg = FALSE, HalfCnt = 0;
	
	Cnt = 4;
	DataArea->p = 0;		
	DataArea->y = 0;		
	DataArea->x = 0;
	Cnt += 2; //���� ���� ��
	if (Asc2Int(&pRecvArea[Cnt], 2) > 0)	
	{															// ���ΰ���
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SkipLineCnt ���ΰ��� = %d", Asc2Int(&pRecvArea[Cnt], 2));

		DataArea->y = Asc2Int(&pRecvArea[Cnt], 2);		// �����
		DataArea->x = 0;
	}
	Cnt += 2; //���� ��� �Ǽ�
	Cnt += 2;
	while (Cnt < FindLength)
	{															
		if (memcmp(&pRecvArea[Cnt], PPPRecvCmdId, sizeof(PPPRecvCmdId)) == 0)
		{														// �������ٲ�
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PPPRecvCmdId �������ٲ�");
			fnAPP_NONGHYUP_RecvPPPMove(' ', DataArea, 1);				// V04-01-06-#01
			ResultCnt = 120;
			if (HanFlg)
			{													// �ѱ�End
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
				HanFlg = FALSE;
			}
			Cnt += 4;
			if (ResultCnt < 4)
				ResultCnt = 0;
			else ResultCnt -= 4;
		}
		else
		if (memcmp(&pRecvArea[Cnt], SUBNewLineCmdId, sizeof(SUBNewLineCmdId)) == 0)	
		{														// ���ΰ���
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SUBNewLineCmdId ���ΰ���");
			if (HanFlg)
			{													// �ѱ�End
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
				HanFlg = FALSE;
			}

			// V04-03-05-#01 Start����������������������������������������������������
			// ����Ÿ ���� ���ΰ��ุ ���� ��� ��å 
			if (ResultCnt == 0)
			{
				fnAPP_NONGHYUP_RecvPPPMove(' ', DataArea, 1);			// V04-03-05-#01
				ResultCnt = 120;
			}
			// V04-03-05-#01 End������������������������������������������������������

			DataArea->y ++;										// �����

			if (DataArea->y >= itemof(DataArea->Data[0]))		// ����üũ
			{
				DataArea->y -= 24;
				DataArea->p++;
			}
			DataArea->x = 0;
			Cnt += 4;
			if (ResultCnt < 4)
				ResultCnt = 0;
			else ResultCnt -= 4;
			Cnt += ResultCnt;

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "���� Cnt = %d, ResultCnt = %d", Cnt, ResultCnt);
			ResultCnt = 0;

			if ((!ResultCnt) && (HanFlg))
			{													// �ѱ�End
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
				fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
				HanFlg = FALSE;
			}

			if ((!ResultCnt) && (HalfCnt & 0x01))				// �ݰ��ڵ�¦����
				fnAPP_NONGHYUP_RecvPPPMove(HalfSpace, DataArea, 1);

			if (!ResultCnt)
			{
				HanFlg = FALSE;
				HalfCnt = 0;
			}
		}
		else
		{
			if (ResultCnt)
			{													// ����
				if (memcmp(&pRecvArea[Cnt], DATAPackCmdId, sizeof(DATAPackCmdId)) == 0)	
				{
					if (HanFlg)
					{											// �ѱ�End
						fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
						fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
						HanFlg = FALSE;
					}
					fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt + 3], DataArea, pRecvArea[Cnt + 2]);
					Cnt += 4;
					if (ResultCnt < 4)
						ResultCnt = 0;
					else ResultCnt -= 4;
				}
				else											// �ݰ�
				if (memcmp(&pRecvArea[Cnt], DATAHalfCmdId, sizeof(DATAHalfCmdId)) == 0)	
				{												// �ݰ��ڵ弳��
					if (HanFlg)
					{											// �ѱ�End
						fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
						fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
						HanFlg = FALSE;
					}
					if ((pRecvArea[Cnt + 2] == ' ') ||			// ' '
						(pRecvArea[Cnt + 2] == ',') ||			// ','
						(pRecvArea[Cnt + 2] == '.') ||			// '.'
						(pRecvArea[Cnt + 2] == '\''))			// '''
					{	
						HalfCnt++;
						if (pRecvArea[Cnt + 2] == ' ')			// ' '
							fnAPP_NONGHYUP_RecvPPPMove(PbHalfSpace, DataArea, 1);
						if (pRecvArea[Cnt + 2] == ',')			// ','
							fnAPP_NONGHYUP_RecvPPPMove(PbHalfcomma, DataArea, 1);
						if (pRecvArea[Cnt + 2] == '.')			// '.'
							fnAPP_NONGHYUP_RecvPPPMove(PbHalfperiod, DataArea, 1);
						if (pRecvArea[Cnt + 2] == '\'')			// '''
							fnAPP_NONGHYUP_RecvPPPMove(PbHalfapostrophe, DataArea, 1);
					}
					else fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt + 2], DataArea, 1);
					Cnt += 3;									// �ݰ��ڵ�����
					if (ResultCnt < 3)
						ResultCnt = 0;
					else ResultCnt -= 3;
				}
				else
				{
					if (pRecvArea[Cnt] & 0x80)					// �ѱ�1st
					{
						if (ResultCnt >= 2)						// �ܿ�����2��
						{
							if (pRecvArea[Cnt + 1] & 0x80)		// �ѱ�2st
							{
								if (!HanFlg)
								{								// �ѱ�Start
									fnAPP_NONGHYUP_RecvPPPMove(HanStart[0], DataArea, 1);
									fnAPP_NONGHYUP_RecvPPPMove(HanStart[1], DataArea, 1);
									HanFlg = TRUE;
								}
								// �ѱ� Filtering ��� ��å PJH 2002.07.18
								// �ѱۿ���: 0xb0|0xa1 ~ 0xc8|0xfe �̿��� �����ʹ� "��"�ڷ� Set��.
								if ((pRecvArea[Cnt] < 0xb0) ||
									(pRecvArea[Cnt] > 0xc8))
								{
									fnAPP_NONGHYUP_RecvPPPMove(0xbe, DataArea, 1);
									fnAPP_NONGHYUP_RecvPPPMove(0xf8, DataArea, 1);
									Cnt += 2;
								}
								else
								{
									fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt++], DataArea, 1);
									fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt++], DataArea, 1);
								}
								if (ResultCnt < 2)
									ResultCnt = 0;
								else ResultCnt -= 2;
							}
							else
							{
								if (HanFlg)
								{								// �ѱ�End
									fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
									fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
									HanFlg = FALSE;
								}
								fnAPP_NONGHYUP_RecvPPPMove(' ', DataArea, 1);
								fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt + 1], DataArea, 1);
								Cnt += 2;
								if (ResultCnt < 2)
									ResultCnt = 0;
								else ResultCnt -= 2;
							}
						}
						else									// �ܿ�����1��
						{
							if (HanFlg)
							{									// �ѱ�End
								fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
								fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
								HanFlg = FALSE;
							}
							fnAPP_NONGHYUP_RecvPPPMove(' ', DataArea, 1);
							Cnt++;
							ResultCnt--;
						}
					}
					else										// ����
					{
						if (HanFlg)
						{										// �ѱ�End
							fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
							fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
							HanFlg = FALSE;
						}
						fnAPP_NONGHYUP_RecvPPPMove(pRecvArea[Cnt++], DataArea, 1);
						ResultCnt--;
					}
				}

				if ((!ResultCnt) && (HanFlg))
				{												// �ѱ�End
					fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
					fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
					HanFlg = FALSE;
				}

				if ((!ResultCnt) && (HalfCnt & 0x01))			// �ݰ��ڵ�¦����
					fnAPP_NONGHYUP_RecvPPPMove(HalfSpace, DataArea, 1);

				if (!ResultCnt)
				{
					HanFlg = FALSE;
					HalfCnt = 0;
				}
			}
			else
			{
				if (HanFlg)
				{												// �ѱ�End
					fnAPP_NONGHYUP_RecvPPPMove(HanEnd[0], DataArea, 1);
					fnAPP_NONGHYUP_RecvPPPMove(HanEnd[1], DataArea, 1);
					HanFlg = FALSE;
				}
				ResultCnt = 120;
			}
		}
	}

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPP_NONGHYUP_RecvPPPGet - 0]", &DataArea->Data[0][0], sizeof(DataArea->Data));

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_NONGHYUP_RecvPPPMove[FindLength = %d]", FindLength);

	return FindLength;
}
// DATA��/���������Ʈ(�ŷ�)


int CTranCmn::fnAPP_IBK_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0;
	BYTE	hanFlag = 0;
	BYTE	tempBuff[256];


	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// �μ���۶���
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// �μ���μ�
	Cnt += 2;
	DataArea->x = 0;
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		for(int ii=0; ii<75; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// �ѱ۽���ó��
			{
				if(hanFlag != 1)							// ��������
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// �ѱ۳�ó��
			{
				if(hanFlag == 1)							// �ѱۻ���
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}
		memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][(DataArea->y)+i] = tmpCnt;
		Cnt+= 75;
	}
	return Cnt;
}

// DATA��/���������Ʈ(�ŷ�)
int CTranCmn::fnAPP_KEB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0, StartPosX = 0;
	BYTE	hanFlag = 0;
	char	szBuff[256];
	BYTE	tempBuff[256];

	Accept.Index = 0;
	memset(szBuff, 0x00, sizeof(szBuff));
	memset(Accept.CrmDummy, 0x00, sizeof(Accept.CrmDummy));
	
	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// �μ���۶���
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// �μ���μ�
	Cnt += 2;
	DataArea->x = 0;
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
	
		if(pRecvArea[Cnt] == 'M') //#0099
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPPGet - 1 [%3.3s]", &pRecvArea[Cnt]); //#0113
			if (memcmp(&pRecvArea[Cnt + 1], "00", 2) == 0) 
			{
				Accept.Index = 99;
				memcpy(Accept.CrmDummy, &pRecvArea[Cnt + 3], 80);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPPGet PBM M00 -> Msg[%s]", strCT(Accept.CrmDummy,80));
			}
			else
			if (memcmp(&pRecvArea[Cnt + 1], "02", 2) == 0) 
			{
				memcpy(szBuff, &pRecvArea[Cnt + 3], 80);
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPPGet PBM M02 -> Msg[%s]",szBuff);
			}
		}
		
		for(int ii=0; ii<83; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// �ѱ۽���ó��
			{
				if(hanFlag != 1)							// ��������
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// �ѱ۳�ó��
			{
				if(hanFlag == 1)							// �ѱۻ���
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}
		
		//D20 = ��������(������(1Byte) + Pbm_L_Cnt(2Byte)), M01 = M - ���� Page => M02 �μ� ���� ����! M00 = ���� ���  //#0099
		
		
		if(tempBuff[0] == 'M')
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPPGet - 2 [%3.3s]",tempBuff);
			if (memcmp(&tempBuff[1], "01", 2) == 0) 
			{
				memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], &tempBuff[StartPosX], tmpCnt - 3);
			}
		}
		else
		{
			memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], &tempBuff[StartPosX + 3], tmpCnt - 3);		
		}

		
		DataArea->DataLength[DataArea->p][(DataArea->y)+i] = tmpCnt - 3;
		Cnt+= 83;	
	}
	
	return Cnt;
}

// DATA��/���������Ʈ(�ŷ�)
int CTranCmn::fnAPP_KJB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)	// #N0181
{
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KJB_RecvPPPGet()");

	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0;
	BYTE	hanFlag = 0;
	BYTE	tempBuff[256];

	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// �μ���۶���
	Cnt += 2;
	//DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		
	DataArea->ParaLength = 0;								// �μ���μ��� ���� ���μ��� �ƴ�, ī��Ʈ �ʿ� (�ǹ̾��� 00:����, 01:����)
	Cnt += 2;
	DataArea->x = 0;
	for(int i=0; ; i++)
	{
		if (pRecvArea[Cnt] != 'D')							// �μ���� ����Ȯ��
			break;

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KJB_RecvPPPGet():Cnt[%d]", Cnt);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KJB_RecvPPPGet()", &pRecvArea[Cnt], 71);

		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		for(int ii=1; ii<71; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// �ѱ۽���ó��
			{
				if(hanFlag != 1)							// ��������
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// �ѱ۳�ó��
			{
				if(hanFlag == 1)							// �ѱۻ���
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}
//		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[fnAPP_IBK_RecvPPPGet P]", tempBuff, tmpCnt);
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_IBK_RecvPPPGet[%s]",tempBuff);
		memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][(DataArea->y)+i] = tmpCnt;
		Cnt += 71;
		DataArea->ParaLength ++;
	}

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KJB_RecvPPPGet():return(%d) - DataArea->ParaLength[%d]", Cnt, DataArea->ParaLength);
	return Cnt;
}

int CTranCmn::fnAPP_HANA_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0, StartPosX = 0;
	BYTE	hanFlag = 0;
	char	szBuff[256];
	BYTE	tempBuff[256];
	BYTE	tempArea1[10], tempArea2[10];
	BYTE	HanLeft = 0, HanRight = 0;
	
	//Accept.Index = 0;
	memset(szBuff, 0x00, sizeof(szBuff));
	//memset(Accept.CrmDummy, 0x00, sizeof(Accept.CrmDummy));
	
	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// �μ���۶���
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// �μ���μ�
	Cnt += 2;
	DataArea->x = 0;

	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", RecvArea, Length);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_HANA_RecvPPPGet():return(%d) - DataArea->ParaLength[%d]", DataArea->y, DataArea->ParaLength);
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		
		for(int ii=0; ii<83; )
		{
			if(ii == 0 && (pRecvArea[Cnt+ii] == 'D'))
			{
				fnAPP_HANA_RecvPPPMove(' ', DataArea, 1);
				ii += 3;
			}
			else
			if(ii == 0 && (pRecvArea[Cnt+ii] == 'M') && memcmp(&pRecvArea[Cnt+ii+1], "25", 2) == 0)
			{				
				fnAPP_HANA_RecvPPPMove(' ', DataArea, 1);
				ii += 3;
			}
			else
			if ((pRecvArea[Cnt+ii] == '#')		&&						// ����Ȯ���ϰ�� ����
			(pRecvArea[Cnt+ii+1] == '$')		&&
			(pRecvArea[Cnt+ii+2] == '#')		&&
			(pRecvArea[Cnt+ii+3] == 0x1B)	&&
			((pRecvArea[Cnt+ii+4] == 0x33)	||
			 (pRecvArea[Cnt+ii+4] == 0x34))	)
				ii += 5;
			else
			{
				if (pRecvArea[Cnt+ii] & 0x80)							// �ѱ�1st
				{
					if (pRecvArea[Cnt + ii + 1] & 0x80)					// �ѱ�2st
					{
						if (!hanFlag)
						{											// �ѱ�Start
							fnAPP_HANA_RecvPPPMove(PbHanStart[0], DataArea, 1);
							fnAPP_HANA_RecvPPPMove(PbHanStart[1], DataArea, 1);
							hanFlag = TRUE;		
						}
						memset(tempArea1, 0x00, sizeof(tempArea1));
						memset(tempArea2, 0x00, sizeof(tempArea2));

						HanLeft = pRecvArea[Cnt + ii];
						ii++;
						HanRight = pRecvArea[Cnt+ ii];
						ii++;

						tempArea1[0] = HanLeft;
						tempArea1[1] = HanRight;

						CodeCvt(tempArea1, tempArea2, 2, KSIBM);
						fnAPP_HANA_RecvPPPMove(tempArea2[0], DataArea, 1);
						fnAPP_HANA_RecvPPPMove(tempArea2[1], DataArea, 1);
					}
					else
					{
						if (hanFlag)
						{											// �ѱ�End
							fnAPP_HANA_RecvPPPMove(PbHanEnd[0], DataArea, 1);
							fnAPP_HANA_RecvPPPMove(PbHanEnd[1], DataArea, 1);
							hanFlag = FALSE;
						}
						fnAPP_HANA_RecvPPPMove(A2E(' '), DataArea, 1);
						fnAPP_HANA_RecvPPPMove(pRecvArea[Cnt +ii+ 1], DataArea, 1);
						ii += 2;
					}
				}
				else												// ����
				{
					if (hanFlag)
					{												// �ѱ�End
						fnAPP_HANA_RecvPPPMove(PbHanEnd[0], DataArea, 1);
						fnAPP_HANA_RecvPPPMove(PbHanEnd[1], DataArea, 1);
						hanFlag = FALSE;
					}

					fnAPP_HANA_RecvPPPMove(pRecvArea[Cnt+ii], DataArea, 1);
					
					ii++;
				}
			}
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HANA_RecvPPPGet[%d]",DataArea->DataLength[DataArea->p][(DataArea->y)]);
		//DataArea->DataLength[DataArea->p][(DataArea->y+i)] = ii;
		DataArea->y ++;
		/*
		if (DataArea->y >= itemof(DataArea->Data[0]))		// ����üũ
		{
			DataArea->y -= 24;
			DataArea->p++;
		}
		*/
		
		DataArea->x = 0;
		Cnt += 83;	
	}
	
	DataArea->y = Asc2Int(&pRecvArea[4+2], 2)-1;
	DataArea->x = 0;
	DataArea->p = 0;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HANA_RecvPPPGet__!!![%d]",DataArea->DataLength[DataArea->p][(DataArea->y)]);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "DataArea->Data", DataArea->Data, Length);
	return Cnt;
}


//szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3 + 2] 
int CTranCmn::fnAPP_KB_RecvPPPGet(void* RecvArea, PPPRecvKb* DataArea, int Length) //#N0226
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0, StartPosX = 0;
	BYTE	hanFlag = 0;
	char	szBuff[256];
	BYTE	tempBuff[256];

	memset(szBuff, 0x00, sizeof(szBuff));

	Cnt = 1 + 1 + 1 + 1 + 1; // 5
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;                                               // ������
	DataArea->Bp = Asc2Int(&pRecvArea[Cnt], 3);				// �μ���۶���
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 3);				// �μ���۶���
	Cnt += 3;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// �μ���μ�
	Cnt += 2;

	Cnt += 1; //FS

	DataArea->x = 0;
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
	
		for(int ii=0; ii<80; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// �ѱ۽���ó��
			{
				if(hanFlag != 1)							// ��������
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// �ѱ۳�ó��
			{
				if(hanFlag == 1)							// �ѱۻ���
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}

		memcpy(&DataArea->Data[DataArea->p][DataArea->y][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][DataArea->y] = tmpCnt;
		//Cnt+= 68;	
		Cnt+= 80;
		
		DataArea->y++;

		if (DataArea->y >= itemof(DataArea->Data[0]))		    // ����üũ
		{
			DataArea->y -= 24;
			DataArea->p++;
		}
	}
	
	return Cnt;
}


// DATA��/���������Ʈ(�ŷ�)
int CTranCmn::fnAPP_CTB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)	// #N0252
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0;
	BYTE	hanFlag = 0;
	BYTE	tempBuff[256];

	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = 0;	//Asc2Int(&pRecvArea[Cnt], 2)-1;	// �μ���۶��� - D Type �� ���ڸ� ���ڷ� ���ڶ��� ����
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// �μ���μ�
	Cnt += 2;
	DataArea->x = 0;
	
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		if (pRecvArea[Cnt] == 'M')							// M Type (00:�߰��������, 01:����������, 02:�μⳡ, 03:���屳ü)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CTB_RecvPPPGet():DataType[%3.3s] M00:�߰��������, M01:����������, M02:�μⳡ, M03:���屳ü", &pRecvArea[Cnt]);
			if (!memcmp(&pRecvArea[Cnt + 1], "01", 2))
			{
				DataArea->p ++;								// ����������
				DataArea->y = 0;							// �����ʱ�ȭ
				Cnt+= 83;
				continue;
			}
			else
			if (!memcmp(&pRecvArea[Cnt + 1], "00", 2))		// M00�� �߰��������� ����
			{
				PbNextTranFlag = TRUE;
				break;
			}
			else
			if (!memcmp(&pRecvArea[Cnt + 1], "02", 2))
				break;
			else
			if (!memcmp(&pRecvArea[Cnt + 1], "03", 2))		// M03�� �����̿�
			{
				if(DataArea->ParaLength == 0)				
				{
					m_pDevCmn->fnSCR_DisplayString(2, "[�����̿�]");
					m_pDevCmn->fnSCR_DisplayString(3, "â������ ������ ��߱� �����ʽÿ� ");
					m_pDevCmn->fnSCR_DisplayScreen(701);
					Delay_Msg(3000);
					fnAPP_CancelProc(T_OK);
				}
			}
			else
				break;
		}

		if (pRecvArea[Cnt] == 'D')							// D Type (���ڸ� ���ڰ� �ѱ� ���μ�)
		{
			if (DataArea->y == 0)
			{
				tmpCnt = Asc2Int(&pRecvArea[Cnt + 1], 2);
				if ((tmpCnt >= 3) &&(tmpCnt <= 14))
					DataArea->y += tmpCnt - 3;		// ��Ƽ������ 3�ٺ��� ���� (D03)
				else
				if ((tmpCnt >= 17) &&(tmpCnt <= 29))
					DataArea->y += tmpCnt - 5;		// ��Ƽ������ 3�ٺ��� ���� + ����� 2�� (D17)
				else
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CTB_RecvPPPGet():DataType[%3.3s] ������ ���� ���� (��ȿ���� 3~14, 17~19)", &pRecvArea[Cnt]);

				if (DataArea->y < 0)
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CTB_RecvPPPGet():DataType[%3.3s], DataArea->p[%d], DataArea->y[%d -> 0]", &pRecvArea[Cnt], DataArea->p, DataArea->y);
					DataArea->y = 0;
				}
			}
			else
				;	//DataArea->y ++; �Ʒ����� ������Ŵ
				// DataArea->y += Asc2Int(&pRecvArea[Cnt + 1], 2);	������� ���� D03�� �����Ѵ�

			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CTB_RecvPPPGet():DataType[%3.3s], DataArea->p[%d], DataArea->y[%d]", &pRecvArea[Cnt], DataArea->p, DataArea->y);
		}

		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		for(int ii=3; ii<83; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// �ѱ۽���ó��
			{
				if(hanFlag != 1)							// ��������
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// �ѱ۳�ó��
			{
				if(hanFlag == 1)							// �ѱۻ���
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}
//		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[fnAPP_IBK_RecvPPPGet P]", tempBuff, tmpCnt);
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_IBK_RecvPPPGet[%s]",tempBuff);
		memcpy(&DataArea->Data[DataArea->p][DataArea->y][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][DataArea->y] = tmpCnt;
		DataArea->y ++;
		Cnt+= 83;
	}
	return Cnt;
}

int CTranCmn::fnAPP_SC_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)  // #N0245
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		HanFlg = FALSE, HanCnt = 0, HalfCnt = 0;
	int		Cnt = 0;
	int		SpaceCnt = 0, i = 0;
	int		PrintCnt = 0;
	int		StartLine = 0;
	BYTE	HanLeft = 0, HanRight = 0;
	BYTE	HanIBM[2], HanKS[2];
	int		HanCmdCnt = 0;

	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;	// �μ���۶��� - D Type �� ���ڸ� ���ڷ� ���ڶ��� ����
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// �μ���μ�
	Cnt += 2;
	DataArea->x = 0;

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():Length[%d], DataArea->y[%d], DataArea->ParaLength[%d]", Length, DataArea->y, DataArea->ParaLength);

	while (Cnt < Length)
	{
		if ((pRecvArea[Cnt] == RecvCmdCR)	||					// Carrige Return
			(pRecvArea[Cnt] == RecvCmdNL)	||					// LINE FEED+CARRIAGE RETURN
			(pRecvArea[Cnt] == RecvCmdESC)	)					// �������
		{
			//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdCR{0x15}, RecvCmdNL{0x0d}, RecvCmdESC{0x27}", Cnt, pRecvArea[Cnt]);
			if (HanFlg)
			{													// �ѱ�End
				fnAPP_RecvPPPMove(PbHanEnd[0], DataArea, 1);
				fnAPP_RecvPPPMove(PbHanEnd[1], DataArea, 1);

				HanCmdCnt += 2;									// �ѱ�����ڼ����� : 2003.11.13
			}
			if (HanCnt & 0x01)									// Han First
			{
				fnAPP_RecvPPPMove(' ', DataArea, 1);
			}
			HanFlg = FALSE;
			HanCnt = 0;
		}

		if (pRecvArea[Cnt] == RecvCmdESC)
		{
			//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdESC{0x27}", Cnt, pRecvArea[Cnt]);
			if (pRecvArea[Cnt + 1] == RecvCmdROW)
			{
				//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdROW{0x82}", Cnt + 1, pRecvArea[Cnt + 1]);
				if (pRecvArea[Cnt + 2])
					DataArea->y = pRecvArea[Cnt + 2] - 1;
				else
					DataArea->y = pRecvArea[Cnt + 2];

				DataArea->x = 0;

				if (Cnt + 2 > Length)
					break;

				Cnt += 2;
			}
			else
			if (pRecvArea[Cnt + 1] == RecvCmdLF)
			{
				//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdLF{0x71}", Cnt + 1, pRecvArea[Cnt + 1]);
				if (DataArea->x == 0)							// �űԶ��ν� 
				{
					HanCmdCnt = 0;								// �ѱ��ڵ幮�ڼ��ʱ�ȭ
					SpaceCnt = 0;								// �����Ѱ��鹮�ڼ��ʱ�ȭ
				}

				if ((DataArea->x - HanCmdCnt) == (pRecvArea[Cnt + 2]))
					;											// �ε���VS�ε����� : 2004.03.11
				else
				if ((DataArea->x - HanCmdCnt) > (pRecvArea[Cnt + 2]))
				{												// �ε���VS�ε����� : 2004.03.11
					DataArea->x = pRecvArea[Cnt + 2];
					DataArea->x += HanCmdCnt;					// �ѱ������ڵ� ���� ����
					if (DataArea->x < 0)
						DataArea->x = 0;
				}
				else
				{
					SpaceCnt = pRecvArea[Cnt + 2] - DataArea->x;	
																// Index 0 baseó��
					SpaceCnt += HanCmdCnt;						// �ѱ������ڵ� ���� ����
					for( i = 0 ; i < SpaceCnt ; i++ )
						fnAPP_RecvPPPMove(' ', DataArea, 1);
				}

				Cnt += 2;
			}
			else
			{
				Cnt += 1;
				Cnt += ESCAddedData[pRecvArea[Cnt + 1]];
			}
		}
		else
		{
			if ((pRecvArea[Cnt] == RecvCmdCR)	||
				(pRecvArea[Cnt] == RecvCmdNL)	)
			{													// ���ΰ���
				//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdCR{0x15}, RecvCmdNL{0x0d}", Cnt, pRecvArea[Cnt]);
				if (HanFlg)
				{												// �ѱ�End
					fnAPP_RecvPPPMove(PbHanEnd[0], DataArea, 1);
					fnAPP_RecvPPPMove(PbHanEnd[1], DataArea, 1);

					HanCmdCnt += 2;								// �ѱ�����ڼ����� : 2003.11.13
					HanFlg = FALSE;
				}
				DataArea->y++;
				DataArea->x = 0;
			}
			else
			if (pRecvArea[Cnt] == RecvCmdSO)					// Shift Out
			{
				//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdSO{0x0e}", Cnt, pRecvArea[Cnt]);
				if (!HanFlg)									// Eng Mode
				{
					fnAPP_RecvPPPMove(PbHanStart[0], DataArea, 1);
					fnAPP_RecvPPPMove(PbHanStart[1], DataArea, 1);

					HanCmdCnt += 2;								// �ѱ�����ڼ����� : 2003.11.13
				}
				HanFlg = TRUE;
				HanCnt = 0;
			}
			else
			if (pRecvArea[Cnt] == RecvCmdSI)					// Shift In
			{
				//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x] ==> RecvCmdSI{0x0f}", Cnt, pRecvArea[Cnt]);
				if (HanFlg)										// Han Mode
				{
					fnAPP_RecvPPPMove(PbHanEnd[0], DataArea, 1);
					fnAPP_RecvPPPMove(PbHanEnd[1], DataArea, 1);

					HanCmdCnt += 2;								// �ѱ�����ڼ����� : 2003.11.13
				}
				if (HanCnt & 0x01)								// Han First
				{
					fnAPP_RecvPPPMove(' ', DataArea, 1);
				}
				HanFlg = FALSE;
				HanCnt = 0;
			}
			else												// Data
			{
				if (HanFlg)										// Han Mode
				{
					HanCnt++;
					if (HanCnt & 0x01)							// Han First
					{
						HanLeft = pRecvArea[Cnt];
					}
					else										// Han Second
					{
						HanRight = pRecvArea[Cnt];
						HanIBM[0] = HanLeft;
						HanIBM[1] = HanRight;
						CodeCvt(&HanIBM, &HanKS, 2, IBMKS);
						HanLeft = HanKS[0];
						HanRight = HanKS[1];
						fnAPP_RecvPPPMove(HanLeft, DataArea, 1);// (�ѱ�¦��)
						fnAPP_RecvPPPMove(HanRight, DataArea, 1);
					}
				}
				else											// Eng Mode
				{
					fnAPP_RecvPPPMove(E2A(pRecvArea[Cnt]), DataArea, 1);
				}
			}
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SCB_RecvPPPGet():pRecvArea[%d, %x, %c] }", Cnt, pRecvArea[Cnt],E2A(pRecvArea[Cnt]));
		Cnt++;
	}

	if (HanFlg)
	{															// �ѱ�End
		fnAPP_RecvPPPMove(PbHanEnd[0], DataArea, 1);
		fnAPP_RecvPPPMove(PbHanEnd[1], DataArea, 1);
	}
	if (HanCnt & 0x01)											// Han First
	{
		fnAPP_RecvPPPMove(' ', DataArea, 1);
	}
	if (HalfCnt & 0x01)											// (�ٴ�¦��)
	{
		fnAPP_RecvPPPMove(PbHalfSpace, DataArea, 1);
	}

	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "DataArea->Data", DataArea->Data, Length);
	return Cnt;
}

// DATA��/���������Ʈ(�ŷ�)
int CTranCmn::fnAPP_KNB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)	// #N0277
{
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KNB_RecvPPPGet()");
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0, hanStart = -1;
	BYTE	hanFlag = 0;
	BYTE	tempBuff[256];

	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// �μ���۶���
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// �μ���μ�
	Cnt += 2;
	DataArea->x = 0;
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		memcpy(tempBuff, &pRecvArea[Cnt], 62);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KNB_RecvPPPGet_Src[%s][%d]", tempBuff, (DataArea->y)+i);
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		for(int ii=0; ii<62; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// �ѱ۽���ó��
			{
				if(hanFlag != 1)							// ��������
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
					hanStart = tmpCnt;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// �ѱ۳�ó��
			{
				if(hanFlag == 1)							// �ѱۻ���
				{
					if (hanStart >= 0)
					{
						MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[KS -> TG] start:%d, size:%d", hanStart, tmpCnt - hanStart);
						HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[KS]", &tempBuff[hanStart], tmpCnt - hanStart);
						m_pDevCmn->fnUTL_HangulCodeConvert(1, (char *)&tempBuff[hanStart], (char *)&tempBuff[hanStart], tmpCnt - hanStart);	// �ϼ��� -> ������ ��ȯ
						HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TG]", &tempBuff[hanStart], tmpCnt - hanStart);
					}

					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
					hanStart = -1;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}

		if(hanFlag == 1)							// �ѱۻ���
		{
			if (hanStart >= 0)
			{
				MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[KS -> TG] start:%d, size:%d", hanStart, tmpCnt - hanStart);
				HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[KS]", &tempBuff[hanStart], tmpCnt - hanStart);
				m_pDevCmn->fnUTL_HangulCodeConvert(1, (char *)&tempBuff[hanStart], (char *)&tempBuff[hanStart], tmpCnt - hanStart);	// �ϼ��� -> ������ ��ȯ
				HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TG]", &tempBuff[hanStart], tmpCnt - hanStart);
			}

			memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
			tmpCnt += 2;
			hanFlag = 0;
			hanStart = -1;
		}

		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[fnAPP_KNB_RecvPPPGet P]", tempBuff, tmpCnt);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KNB_RecvPPPGet[%s][%d]", tempBuff, (DataArea->y)+i);
		memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][(DataArea->y)+i] = tmpCnt;
		Cnt+= 62;
	}

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KNB_RecvPPPGet():return(%d) - DataArea->ParaLength[%d]", Cnt, DataArea->ParaLength);
	return Cnt;
}

// DATA��/���������Ʈ(�ŷ�)
int CTranCmn::fnAPP_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0, tmpCnt = 0;
	BYTE	hanFlag = 0;
	BYTE	tempBuff[256];


	Cnt = 4;
	DataArea->Bp = 0;
	DataArea->Ep = 0;
	DataArea->p = 0;
	Cnt += 2;
	DataArea->y = Asc2Int(&pRecvArea[Cnt], 2)-1;			// �μ���۶���
	Cnt += 2;
	DataArea->ParaLength = Asc2Int(&pRecvArea[Cnt], 2);		// �μ���μ�
	Cnt += 2;
	DataArea->x = 0;
	for(int i=0; i<DataArea->ParaLength; i++)
	{
		memset(tempBuff, 0x00, sizeof(tempBuff));
		tmpCnt = 0;
		for(int ii=0; ii<75; )
		{
			if(pRecvArea[Cnt+ii] >= 0x80)					// �ѱ۽���ó��
			{
				if(hanFlag != 1)							// ��������
				{
					memcpy(&tempBuff[tmpCnt], PbHanStart, 2);
					tmpCnt += 2;
				}
				memcpy(&tempBuff[tmpCnt], &pRecvArea[Cnt+ii], 2);
				ii += 2;
				tmpCnt += 2;
				hanFlag = 1;
			}
			else											// �ѱ۳�ó��
			{
				if(hanFlag == 1)							// �ѱۻ���
				{
					memcpy(&tempBuff[tmpCnt], PbHanEnd, 2);
					tmpCnt += 2;
					hanFlag = 0;
				}
				tempBuff[tmpCnt++] = pRecvArea[Cnt+ii];
				ii += 1;
			}
		}
		memcpy(&DataArea->Data[DataArea->p][(DataArea->y)+i][DataArea->x], tempBuff, tmpCnt);
		DataArea->DataLength[DataArea->p][(DataArea->y)+i] = tmpCnt;
		Cnt+= 75;
	}
	return Cnt;
}

// ���念��Move
int CTranCmn::fnAPP_NONGHYUP_RecvPPPMove(BYTE Data, PPPRecvNon* pDataArea, int Length)
{
	int		i;

	for (i = 0; i < Length; i++)
	{
		if ((pDataArea->p >= itemof(pDataArea->Data))		||		// �Ѱ����
			(pDataArea->y >= itemof(pDataArea->Data[0]))	||
			(pDataArea->x >= itemof(pDataArea->Data[0][0])))
			break;
		
		pDataArea->Data[pDataArea->p][pDataArea->y][pDataArea->x++] = Data;
	}
	return T_OK;
}

// ���念��Move
int CTranCmn::fnAPP_RecvPPPMove(BYTE Data, PPPRecv* pDataArea, int Length)
{
	int		i;
	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->p >= itemof(pDataArea->Data))	 ||		// �Ѱ����
			(pDataArea->y >= itemof(pDataArea->Data[0])) ||
			(pDataArea->x >= itemof(pDataArea->Data[0][0])))
			break;
		pDataArea->Data[pDataArea->p][pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line �ʱ⼳��
		{
			pDataArea->Bp = pDataArea->p;
			pDataArea->Ep = pDataArea->p;
		}
		pDataArea->ParaLength += Length;						// �ڷ��������ϱ�
		if (pDataArea->p < pDataArea->Bp)						// Find Begin Page
			pDataArea->Bp = pDataArea->p;
		if (pDataArea->p > pDataArea->Ep)						// Find End Page
			pDataArea->Ep = pDataArea->p;

		pDataArea->DataLength[pDataArea->p][pDataArea->y] = pDataArea->x;		
																// DATA Length ���ϱ�
	}
	return T_OK;
}


// ���念��Move #N0214
int CTranCmn::fnAPP_HANA_RecvPPPMove(BYTE Data, PPPRecv* pDataArea, int Length)
{
	int		i;

	for (i = 0; i < Length; i++)
	{
		
		if ((pDataArea->p >= itemof(pDataArea->Data))		||		// �Ѱ����
		//	(pDataArea->y >= itemof(pDataArea->Data[0]))	||
			(pDataArea->x >= itemof(pDataArea->Data[0][0])))
			break;
		
		
		pDataArea->Data[pDataArea->p][pDataArea->y][pDataArea->x++] = Data;
		pDataArea->DataLength[pDataArea->p][pDataArea->y] = pDataArea->x;		// DATA Length ���ϱ�
		
	}
	
	return T_OK;
}

// DATA��/�޼�������ǥ��(�ŷ�/����)
int CTranCmn::fnAPP_RecvPMEGet(void* RecvArea, PMERecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		TranCnt	= 0, i = 0, j = 0;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPMEGet: TranCode[%d] TranSend[%d]", TranCode, TranSend);
	if( ((TranCode == TC_INQSECUR	||				// ����������ȸ
		  TranCode == TC_INQUIRYA	||				// �ŷ�������ȸ
		  TranCode == TC_TRANSTOC	||				// �������Ǵ�ü
		  TranCode == TC_REQSTOCK	||				// û���û
		  TranCode == TC_INQSTOCK	||				// û�����
		  TranCode == TC_CANSTOCK)	&&				// û�೻����ȸ
		 TranSend == 1 )	||						// 1����ȸ�� ��츸
		(TranCode == TC_INQPRINT) )				// �ܰ������ȸ
	{
		for(int i=0; i<Asc2Int(&pRecvArea[103], 2) && i<10; i++)
		{
			if(TranCode == TC_INQSECUR)				// ����������ȸ
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 105+(i*85), 105+(i*85)+84);
			else if(TranCode == TC_INQUIRYA)		// �ŷ�������ȸ
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 161+(i*130), 161+(i*130)+129);
			else if(TranCode == TC_TRANSTOC)		// �������Ǵ�ü
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 246+(i*80), 246+(i*80)+79);
			else if(TranCode == TC_REQSTOCK)		// û���û
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 317+(i*103), 317+(i*103)+102);
			else if(TranCode == TC_CANSTOCK)		// û�����
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 230+(i*105), 230+(i*105)+104);
			else if(TranCode == TC_INQSTOCK)		// û�೻����ȸ
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 125+(i*181), 125+(i*181)+180);
			else if(TranCode == TC_INQPRINT)		// �ܰ������ȸ
				fnAPP_RecvPMESubGet(pRecvArea, DataArea, 145+(i*110), 145+(i*110)+109);

			DataArea->y++;
			DataArea->x = 0;
		}
	}
	else
	if (TranCnt == TC_TOTAL)
	{
// 		fnAPP_RecvPMESubGet(pRecvArea, DataArea,  28,  37);		// ��������
// 		DataArea->y++;
// 		DataArea->x = 0;
// 		fnAPP_RecvPMESubGet(pRecvArea, DataArea,  38,  47);		// ��������
// 		DataArea->y++;
// 		DataArea->x = 0;
// 		fnAPP_RecvPMESubGet(pRecvArea, DataArea,  48,  57);		// �Ա�����
// 		DataArea->y++;
// 		DataArea->x = 0;
// 		fnAPP_RecvPMESubGet(pRecvArea, DataArea,  58,  67);		// �Ա�����
// 		DataArea->y++;
// 		DataArea->x = 0;
// 		fnAPP_RecvPMESubGet(pRecvArea, DataArea,  68,  77);		// �Ա�Ÿ��
// 		DataArea->y++;
// 		DataArea->x = 0;
	}

	return Length;
}

// �޼�������ǥ��Sub
int CTranCmn::fnAPP_RecvPMESubGet(void* RecvArea, PMERecv* DataArea, int Cnt, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		HanFlg = FALSE, HanCnt = 0;
	BYTE	HanLeft = 0, HanRight = 0;

	while (Cnt <= Length)										// Length Check
	{
		fnAPP_RecvPMEMove(pRecvArea[Cnt], DataArea, 1);
		Cnt++;													// ����������
	}

	if (HanCnt & 0x01)											// Han First
	{
		fnAPP_RecvPMEMove(' ', DataArea, 1);
	}
	return Cnt;
}

// �޼�����������Move
int CTranCmn::fnAPP_RecvPMEMove(BYTE Data, PMERecv* pDataArea, int Length)
{
	int		i;

	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->y >= itemof(pDataArea->Data)) ||		// �Ѱ����
			(pDataArea->x >= itemof(pDataArea->Data[0])))
			break;
		
		pDataArea->Data[pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line �ʱ⼳��
		{
			pDataArea->By = pDataArea->y;
			pDataArea->Ey = pDataArea->y;
		}
		pDataArea->ParaLength += Length;						// �ڷ��������ϱ�
		if (pDataArea->y < pDataArea->By)						// Find Begin Line
			pDataArea->By = pDataArea->y;
		if (pDataArea->y > pDataArea->Ey)						// Find End Line
			pDataArea->Ey = pDataArea->y;
	}
	return T_OK;
}

// DATA��/�޼���ǥ��(�ŷ�)
int CTranCmn::fnAPP_RecvPMDGet(void* RecvArea, PMDRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		HanFlg = FALSE, HanCnt = 0;
	BYTE	HanLeft = 0, HanRight = 0;
	BYTE	HanIBM[2], HanKS[2];
	int		Cnt = 0, i, j, DataFlag = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPMDGet", pRecvArea, Length, IBMKS);
	while (Cnt <= Length)										// Length Check
	{
		if (pRecvArea[Cnt] == RecvCmdNL)	
		{
			DataArea->y ++;
			DataArea->x = 0;
		}
		else
		if (pRecvArea[Cnt] == RecvCmdSO)						// Shift Out
		{
			HanFlg = TRUE;
			HanCnt = 0;
		}
		else
		if (pRecvArea[Cnt] == RecvCmdSI)						// Shift In
		{
			if (HanCnt & 0x01)									// Han First
			{
				fnAPP_RecvPMDMove(' ', DataArea, 1);
			}
			HanFlg = FALSE;
			HanCnt = 0;
		}
		else													// Data
		{
			if (HanFlg)											// Han Mode
			{
				HanCnt++;
				if (HanCnt & 0x01)								// Han First
				{
					HanLeft = pRecvArea[Cnt];
				}
				else											// Han Second
				{
					HanRight = pRecvArea[Cnt];
					HanIBM[0] = HanLeft;
					HanIBM[1] = HanRight;
					CodeCvt(&HanIBM, &HanKS, 2, IBMKS);
					HanLeft = HanKS[0];
					HanRight = HanKS[1];
					if (DataArea->x >= 39)						// �ѱ�Wordó��
					{
						DataArea->y += 2;
						DataArea->x = 0;
					}
					fnAPP_RecvPMDMove(HanLeft, DataArea, 1);	// (�ѱ�¦��)
					fnAPP_RecvPMDMove(HanRight, DataArea, 1);
				}
			}
			else												// Eng Mode
			{
				fnAPP_RecvPMDMove(pRecvArea[Cnt], DataArea, 1);
			}
		}
		Cnt++;													// ����������
	}

	if (HanCnt & 0x01)											// Han First
	{
		fnAPP_RecvPMDMove(' ', DataArea, 1);
	}
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPMDGet", DataArea, Length);

	if (DataArea->ParaLength)									// �ڷ�����
	{
		for (i = DataArea->By; i <= DataArea->Ey; i++)			// �ڷẸ��
		{
			DataFlag = FALSE;
			for (j = (itemof(DataArea->Data[0]) - 1); j >= 0; j--)
			{
				if (DataArea->Data[i][j] != 0)
					DataFlag = TRUE;
				else
				{
					if (DataFlag)
						DataArea->Data[i][j] = ' ';
				}
			}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PMDRecvData.DataSet[%d] : [%s]", i, PMDRecvData.Data[i] );
		}
	}

	int n = -1;													// ALL-SPACE�� ���� �ڷẸ��
	for (i = itemof(DataArea->Data) - 1 ; i >= 0; i--)			// �ڿ������� ALL-SPACE�� �ƴ� ���� �ε������ϱ�
	{
		if (!IsSpace((char*)GetString(DataArea->Data[i], 40, 40).GetBuffer(0), 40))
		{
			n = i;
			break;
		}
	}

	if (n == -1)												// NOT-ALL-SPACE���� �ε����� -1�̸� ���̻�ó������
	{
		DataArea->ParaLength = 0;
	}

	return Cnt - 1;
}

// ȭ�鿵��Move
int CTranCmn::fnAPP_RecvPMDMove(BYTE Data, PMDRecv* pDataArea, int Length)
{
	int		i;

	if (pDataArea->x >= 40)										// ȭ�鿵���и�ó��
	{
		pDataArea->y += 2;
		pDataArea->x = 0;
	}

	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->y >= itemof(pDataArea->Data)) ||		// �Ѱ����
			(pDataArea->x >= itemof(pDataArea->Data[0])))
			break;
		
		pDataArea->Data[pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line �ʱ⼳��
		{
			pDataArea->By = pDataArea->y;
			pDataArea->Ey = pDataArea->y;
		}
		pDataArea->ParaLength += Length;						// �ڷ��������ϱ�
		if (pDataArea->y < pDataArea->By)						// Find Begin Line
			pDataArea->By = pDataArea->y;
		if (pDataArea->y > pDataArea->Ey)						// Find End Line
			pDataArea->Ey = pDataArea->y;
	}
	return T_OK;
}

// ī��M/S���
int CTranCmn::fnAPP_RecvPCWGet(void* RecvArea, PCWRecv* DataArea, int Length)
{
	BYTE*	pRecvArea = (BYTE*)RecvArea;
	int		Cnt = 0;

	return Cnt;
}
		
// ī�念��Move
int CTranCmn::fnAPP_RecvPCWMove(BYTE Data, PCWRecv* pDataArea, int Length)
{
	int		i;

	for (i = 1; i <= Length; i++)
	{
		if (pDataArea->x >= sizeof(pDataArea->Data))			// �Ѱ����
			break;
		
		pDataArea->Data[pDataArea->x++] = Data;
		pDataArea->ParaLength += Length;
	}
	return T_OK;
}

// DES TMK/TPK����ó��
// DES OutGoing�ŷ� 2004.05.31
int CTranCmn::fnAPP_RecvDesKeyProc(PMERecv* DataBuff)
{
	BYTE SixteenNulls[17];
	BYTE TMK_INIT[17] = "0000000000000000";

	BYTE	MasterKey[32];
	BYTE	MKCheckValue[6];
	
	memset(SixteenNulls, 0x00, sizeof(SixteenNulls));
	memset(MasterKey,	 0x00, sizeof(MasterKey));

	if (fnAPP_RecvDesKeyCheck(DataBuff) == FALSE)
	{
MsgDump(MAC_TRACE_CODE_FILE("Log"), "fnAPP_RecvDesKeyProc() ��ȿ���� ���� DESŰ ������");
		return T_OK;
	}

	switch (m_pProfile->DEVICE.DesHandler)
	{
	case S_DES_SET:
		memcpy(&MasterKey[0], &DataBuff->Data[1][0], 16);
		memcpy(&MKCheckValue[0], &DataBuff->Data[2][0], 6);
		break;
	case T_DES_SET:
		memcpy(&MasterKey[0], &DataBuff->Data[1][0], 16);
		memcpy(&MasterKey[16], &DataBuff->Data[2][0], 16);
		memcpy(&MKCheckValue[0], &DataBuff->Data[3][0], 6);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TMK:MasterKey", MasterKey, 32);
		break;
	default:
MsgDump(MAC_TRACE_CODE_FILE("Log"), "fnAPP_RecvDesKeyProc() ��ȿ���� ���� DES��� ������");
		return T_OK;
	}

	if (TranCode == TC_TMKLOAD)
	{
		if (m_pDevCmn->fnDES_LoadKey(&MasterKey[0], 1))
		{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TMK:m_pDevCmn->DesRPIN", m_pDevCmn->DesRPIN, 16);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TMK:MKCheckValue", MKCheckValue, 6);
			if (memcmp(m_pDevCmn->DesRPIN, &MKCheckValue[0], 6) == 0)
			{
				m_pDevCmn->DesTMKStatus = TRUE;
				m_pProfile->GetDeviceProfile();
				m_pProfile->DEVICE.DesMasterKey = TRUE;
				m_pProfile->PutDeviceProfile();
			}
			else
			{
				m_pDevCmn->fnDES_LoadKey(TMK_INIT, 1);
			}
		}
	}
	else																// TPK
	if ((TranCode == TC_TPKLOAD) &&
	    (m_pProfile->DEVICE.DesMasterKey))
	{
		if (m_pDevCmn->fnDES_LoadKey(&MasterKey[0], 2))
		{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TPK:m_pDevCmn->DesRPIN", m_pDevCmn->DesRPIN, 16);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TPK:MKCheckValue", MKCheckValue, 6);
			if (memcmp(m_pDevCmn->DesRPIN, &MKCheckValue[0], 6) == 0)
			{
				m_pDevCmn->DesTranStatus = TRUE;
			}
		}
	}
	return T_OK;
}

// DES TMK/TPK���� �� Ȯ��
// return : TRUE - DES MAster Key ��ȿ��
//			FALSE - Ű�� ��� �ϳ��̻��� ��ȿ���� ����
int CTranCmn::fnAPP_RecvDesKeyCheck(PMERecv* DataBuff)
{
	BYTE SixteenNulls[17];
	int		nReturn = FALSE;

	memset(SixteenNulls, 0x00, sizeof(SixteenNulls));

	if ((memcmp(&DataBuff->Data[0][0], SixteenNulls, 16) != 0 ) &&
		(!IsZero(&DataBuff->Data[0][0], 16) )					)
	{
		nReturn =TRUE;											// 1�� MasterKey ��ȿ��

		if (m_pProfile->DEVICE.DesHandler == T_DES_SET)			// T-DES�̸� 2��ŰȮ���ʿ�
		{
			if ((memcmp(&DataBuff->Data[1][0], SixteenNulls, 16) != 0 ) &&
				(!IsZero(&DataBuff->Data[1][0], 16) )					)
				nReturn = TRUE;									// 1/2��Ű ��� ��ȿ��
			else
				nReturn = FALSE;								// ��ȿ���� ���� Ű����
		}
	}

	return nReturn;
}
// �۽�(������/�Ա�)
int CTranCmn::fnAPP_SendRecvHostCancel_PowerOff()
{
	int	ScrNum = 0;			
	m_pDevCmn->TranResult = FALSE;								// Host���

	SendLength = 0;												// �۽�Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// �۽�Buffer
/////////////////////////////////////////////////////////////////////////////

	CString cstrGetReg = IniGetStr(_TRANS_POWER_DATA_INI, TRANS_SEC, "TranWithCanCelUnPackData", "CANCEL_NODATA");
	SendLength = MakePack(cstrGetReg.GetBuffer(0), SendBuffer, cstrGetReg.GetLength());

	if (SendLength < 50)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendRecvHostCancel_PowerOff(TranWithCanCelUnPackData = Length(%d):[%s]", SendLength, cstrGetReg);
		return T_OK;
	}

	ScrNum = 701;												// ������ȭ��
	m_pDevCmn->fnSCR_DisplayString(1, "�ŷ� ��������� �������Դϴ�.");
	m_pDevCmn->fnSCR_DisplayString(2, "��ø� ��ٷ� �ֽʽÿ�.");
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);						// ������ȭ��
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��Ҽ۽�"));													
	if (fnAPP_SendData(SendBuffer, SendLength, LINE_SEND_TIME) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��Ҽ۽� : �ð��ʰ�"));
		m_pDevCmn->fnAPL_StackError("7710992", "��Ҽ۽Žð��ʰ�");
	}
	
	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_PROCESS);// �����ư ������
	
	if (fnAPP_RecvData(RecvBuffer, &RecvLength, LINE_SEND_TIME) != T_OK)		
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�ŷ����� : �ð��ʰ�"));
		m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_FINISH);// �����ư ������

		switch(m_pDevCmn->fnNET_GetDeviceStatus())
		{
		case LINE_CLOSE:
			m_pDevCmn->fnAPL_StackError("8860293", "Line Close");
			break;
		default:
			m_pDevCmn->fnAPL_StackError("8860293", "���Žð��ʰ�");
			break;
		}

	}
	else m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�ŷ�����"));

	m_pDevCmn->fnSCR_SetDisplayData(SCR_PROGRESS, PROG_FINISH);// �����ư ������
	
	return T_OK;
}