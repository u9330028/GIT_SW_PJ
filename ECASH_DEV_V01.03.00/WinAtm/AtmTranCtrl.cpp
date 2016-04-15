/////////////////////////////////////////////////////////////////////////////
// AtmTranCtrl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\H\Common\CmnLibIn.h"
#include "..\H\Common\Define.h"
#include "..\H\Common\ConstDef.h"
#include "..\H\Common\ClassInclude.h"
#include "..\H\Dll\DevCmn.h"
#include "..\H\Tran\TranCmn.h"

#include "WinAtm.h"
#include "WinAtmCtl.h"
#include "WinAtmPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// �ŷ�ó��
int	CWinAtmCtrl::TranProc(int TranValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranProc(%d)", TranValue);

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->Om.OperationStatus = "OFF";						// ���۱���
	m_pDevCmn->fnAPL_ClerkInformation();						// �������
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->AtmStatusReEntryFlag = FALSE;					// �����Լ���
	m_pDevCmn->TranResult = FALSE;								// �ŷ�����ʱ�ȭ(FALSE)
	m_pDevCmn->TranStatus = TranValue;							// �ŷ�ó��


	switch (m_pDevCmn->TranStatus) 
	{
		case TRAN_IDLE:											// �ŷ����
			m_pDevCmn->TranStatusSave = TRAN_IDLE;				// �������º���
			break;
		case TRAN_READY:										// �غ��ó��
		case TRAN_STATUS:										// ����/���ó��
		case TRAN_OPEN:											// ����ó��
		case TRAN_TRAN:											// �ŷ�ó��
		case TRAN_TOTAL:										// �հ�����
		case TRAN_SUBTOTAL:										// �Ұ�����
		case TRAN_CASH_REG:										// ���ݵ��
		case TRAN_CHECK_REG:									// ��ǥ���
		case TRAN_CHECK_UNREG:									// ��ǥ����
		case TRAN_MSFINGER_CONFIRM:								// �������� Flow ����
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranStatus[%d]", m_pDevCmn->TranStatus);
			m_pDevCmn->TranStatusSave = m_pDevCmn->TranStatus;	// �������º���

			m_pTranCmn->fnAPP_TranVirtualMainProc();			// �ŷ��������
			break;

		default:
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranProc():return(%d)", m_pDevCmn->TranResult);
	return m_pDevCmn->TranResult;								// Tran�ŷ����
}

// ����ó��
int	CWinAtmCtrl::OpenProc()
{
	if (m_pDevCmn->fnAPL_CheckHostOpen())						// ȣ��Ʈ��������
		return TRUE;

	if (m_pDevCmn->Timer.HostOpenRetryTime)						// ������õ�Ÿ��
		return FALSE;

	if (m_pDevCmn->HostStatus == ATM_CLERK)						// ������õ�Ÿ��
		return FALSE;

	if (m_pDevCmn->HostStatus == HOST_WAITREADY)				// �غ�ϴ��
	{
		m_pDevCmn->SeedTranStatus = FALSE;						// 

		if (m_pDevCmn->fnAPL_CheckHostLine())					// �غ��ó�� (��ȣȭ Űó��)
		{														// Host Ok
			m_pDevCmn->HostStatus = HOST_READY;					// �غ��
			Delay_Msg();										// �����������
		}
		else 
		{														// Host Ng
			if (m_pDevCmn->HostOpenRetryCnt++ < 6)				// �����õ�Ƚ��(�غ������)
				m_pDevCmn->Timer.HostOpenRetryTime = 10;			// ������õ�Ÿ��(5������)
			else 
				m_pDevCmn->Timer.HostOpenRetryTime = OPEN_RETRY_TIME;		
																// ������õ�Ÿ��(6���̻�)
		}
	}

	if (m_pDevCmn->HostStatus == HOST_WAITONLINE)					// �������
	{
		if(m_pDevCmn->HostFirstOpenFlag)
		{
			if(TranProc(TRAN_STATUS))
			{
				m_pDevCmn->HostOpenFlag = TRUE;
				m_pDevCmn->HostStatus = HOST_ONLINE;				// �����Ϸ�			
			}
			else
			{
				m_pDevCmn->HostStatus = HOST_WAITREADY;				// �غ�ϴ��
				if (m_pDevCmn->HostOpenRetryCnt++ > 30)				// �����õ�Ƚ��(�غ������)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ - RECV NG Cnt(%d)]", m_pDevCmn->HostOpenRetryCnt));
					m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
				}
				else 
					m_pDevCmn->Timer.HostOpenRetryTime = OPEN_RETRY_TIME;		
			}
		}
		else
		if (TranProc(TRAN_OPEN))								// ����ó��
		{														// Host Ok
			m_pDevCmn->HostStatus = HOST_ONLINE;				// �����Ϸ�
			m_pDevCmn->HostFirstOpenFlag = TRUE;				// Host���ʰ���Flag
			m_pDevCmn->HostFirstReadyFlag = TRUE;
			m_pDevCmn->HostOpenFlag = TRUE;						// Host����Flag
		}
		else 
		{														// Host Ng
			m_pDevCmn->HostStatus = HOST_WAITREADY;				// �غ�ϴ��
			
			if (m_pDevCmn->HostOpenRetryCnt++ > 30)				// �����õ�Ƚ��(�غ������)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ - RECV NG Cnt(%d)]", m_pDevCmn->HostOpenRetryCnt));
				m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			}
			else 
				m_pDevCmn->Timer.HostOpenRetryTime = OPEN_RETRY_TIME;		
																// ������õ�Ÿ��(6���̻�)
		}
	}

	if (!m_pDevCmn->fnAPL_CheckHostOpen())						// ȣ��Ʈ��������
		return FALSE;

	m_pDevCmn->HostOpenRetryCnt = 0;							// �����õ�Ƚ��(�غ������)
	m_pDevCmn->Timer.HostOpenRetryTime = 0;						// ������õ�Ÿ��
	return TRUE;
}	

int	CWinAtmCtrl::ReOpenUploadProc()
{	
	CString strTmp("");
	char tmpYYYYMMDD[9];
	char tmpHHSS[5];
	static int sendFlg = FALSE;

	memcpy(tmpYYYYMMDD, GetDate().GetBuffer(0), GetDate().GetLength());
	memcpy(tmpHHSS, GetTime().GetBuffer(0), 4);
	strTmp.Format("%4.4s", &m_pProfile->NETWORK.AtmSerialNum[4]);

	if(memcmp(m_pProfile->TRANS.YYYYMMDD, GetDate().GetBuffer(0), GetDate().GetLength()) != 0)
	{
		//m_pTranCmn->DeleteJnlFiles(_EJR_DIR, 30);		//30���� ���� ����


		sendFlg = FALSE;
		
		//#N0280
		m_pDevCmn->fnAPL_SetDate(tmpYYYYMMDD);
		m_pDevCmn->fnAPL_ClearSerialNo();

		if (!m_pDevCmn->fnAPL_OpenLine() || !m_pDevCmn->fnAPL_CheckHostOpen())
			return T_OK;
	}

	// #N0274
	if (!sendFlg)
	{
		int nRebootTime = Asc2Int(strTmp, 1) / 2 * 10;	// ������� �����ýð� ���

		if (nRebootTime != Asc2Int(tmpHHSS, 4))
			return T_OK;

		sendFlg = TRUE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetDate[%s]", GetDate().GetBuffer(0));
		if (TranProc(TRAN_OPEN))								// ����ó��
		{														// Host Ok
			m_pDevCmn->HostStatus = HOST_ONLINE;				// �����Ϸ�
			m_pDevCmn->HostFirstOpenFlag = TRUE;				// Host���ʰ���Flag
			m_pDevCmn->HostFirstReadyFlag = TRUE;
			m_pDevCmn->HostOpenFlag = TRUE;						// Host����Flag
		}
		else 
		{														// Host Ng
			m_pDevCmn->HostStatus = HOST_WAITREADY;			// �غ�ϴ��
			if (m_pDevCmn->HostOpenRetryCnt++ > 30)				// �����õ�Ƚ��(�غ������)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ - RECV NG Cnt(%d)]", m_pDevCmn->HostOpenRetryCnt));
				m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			}
			else 
				m_pDevCmn->Timer.HostOpenRetryTime = OPEN_RETRY_TIME;		
		}
	}

	return T_OK;
}

