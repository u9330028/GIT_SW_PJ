/////////////////////////////////////////////////////////////////////////////
// AtmHostCtrl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
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
//	Host Status
/////////////////////////////////////////////////////////////////////////////
//	Mode :	HOST_OFFLINE		: 교신불가
//			HOST_WAITLINK		: 교신대기
//			HOST_LINK			: 교신가능
//			HOST_WAITREADY		: 준비완대기
//			HOST_READY			: 준비완
//			HOST_WAITONLINE		: 개국대기
//			HOST_ONLINE			: 개국완료
/////////////////////////////////////////////////////////////////////////////
// 자동기호스트제어
int CWinAtmCtrl::AtmHostCtrl()
{
if (m_pDevCmn->HostStatus != m_pDevCmn->HostStatusSave)
{
	if(m_pDevCmn->HostStatus != HOST_INIT)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HostStatus[%d], HostStatusSave[%d]", m_pDevCmn->HostStatus, m_pDevCmn->HostStatusSave);
}
	switch (m_pDevCmn->HostStatus) 
	{
		case HOST_INIT:
			m_pDevCmn->Timer.HostWaitLinkTime = LINE_OPCL_TIME;
			if (m_pDevCmn->fnLDCtrl_GetDeviceStatus() == NORMAL)							//#N0183
				m_pDevCmn->Timer.HostRebRetryTime = LINE_OPCL_TIME * K_10_WAIT;
			else
				m_pDevCmn->Timer.HostRebRetryTime = LINE_OPCL_TIME * K_30_WAIT;  
			
			m_pDevCmn->HostStatusSave = HOST_INIT;			// 이전상태보관
			m_pDevCmn->HostStatus = HOST_OFFLINE;
			break;
			
		case HOST_OFFLINE:										// 교신불가
			m_pDevCmn->HostStatusSave = HOST_OFFLINE;			// 이전상태보관

			//m_pDevCmn->fnNET_ClearErrorCode();
			m_pDevCmn->fnNET_Initialize(LINE_OPCL_TIME);		// 연결시작및초기화

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
			{
				m_pDevCmn->HostStatus = HOST_WAITLINK;
	
				m_pDevCmn->Timer.HostWaitLinkTime = LINE_OPCL_TIME;

				if(!m_pDevCmn->Timer.HostRebRetryTime)
				{
					if (m_pDevCmn->fnLDCtrl_GetDeviceStatus() == NORMAL)							//#N0183
						m_pDevCmn->Timer.HostRebRetryTime = LINE_OPCL_TIME * K_10_WAIT;
					else
						m_pDevCmn->Timer.HostRebRetryTime = LINE_OPCL_TIME * K_30_WAIT;  
				}

			}
			else
				//m_pDevCmn->HostStatus = HOST_WAITREADY;
				m_pDevCmn->HostStatus = HOST_LINK;
			break;

		case HOST_WAITLINK:										// 교신대기
			m_pDevCmn->HostStatusSave = HOST_WAITLINK;			// 이전상태보관

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
			{
				if (!m_pDevCmn->Timer.HostWaitLinkTime)			// 교신재시도타임
				{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "m_pDevCmn->Timer.HostRebRetryTime[%d]", m_pDevCmn->Timer.HostRebRetryTime);

					m_pDevCmn->Timer.HostWaitLinkTime = LINE_OPCL_TIME;	
					m_pDevCmn->HostStatus = HOST_OFFLINE;
					if(!m_pDevCmn->OpenKey) break;

					AtmHostLinkCtrl();

					if(!m_pDevCmn->Timer.HostRebRetryTime)
					{
						if (m_pDevCmn->fnLDCtrl_GetDeviceStatus() == NORMAL)							//#N0183
							m_pDevCmn->Timer.HostRebRetryTime = LINE_OPCL_TIME * K_10_WAIT;
						else
							m_pDevCmn->Timer.HostRebRetryTime = LINE_OPCL_TIME * K_30_WAIT;  
					}

				}
				else
					m_pDevCmn->HostStatus = HOST_WAITLINK;
			}
			else
				m_pDevCmn->HostStatus = HOST_LINK;
			break;

		case HOST_LINK:											// 교신가능
			m_pDevCmn->HostStatusSave = HOST_LINK;				// 이전상태보관
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", 0);
			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
				m_pDevCmn->HostStatus = HOST_OFFLINE;
			else
				m_pDevCmn->HostStatus = HOST_WAITREADY;
			break;

		case HOST_WAITREADY:									// 준비완대기
			m_pDevCmn->HostStatusSave = HOST_WAITREADY;			// 이전상태보관

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
				m_pDevCmn->HostStatus = HOST_OFFLINE;
			else
				m_pDevCmn->HostStatus = HOST_WAITREADY;
			break;

		case HOST_READY:										// 준비완
			m_pDevCmn->HostStatusSave = HOST_READY;				// 이전상태보관

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
				m_pDevCmn->HostStatus = HOST_OFFLINE;
			else
				m_pDevCmn->HostStatus = HOST_WAITONLINE;
			break;

		case HOST_WAITONLINE:									// 개국대기
			m_pDevCmn->HostStatusSave = HOST_WAITONLINE;		// 이전상태보관

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
				m_pDevCmn->HostStatus = HOST_OFFLINE;
			else
				m_pDevCmn->HostStatus = HOST_WAITONLINE;
			break;

		case HOST_ONLINE:										// 개국완료
			m_pDevCmn->HostStatusSave = HOST_ONLINE;			// 이전상태보관

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
			{
				if (m_pDevCmn->fnLDCtrl_GetDeviceStatus() == NORMAL)							//#N0183
					m_pDevCmn->Timer.HostRebRetryTime = LINE_OPCL_TIME * K_10_WAIT;
				else
					m_pDevCmn->Timer.HostRebRetryTime = LINE_OPCL_TIME * K_30_WAIT;  
				m_pDevCmn->HostStatus = HOST_OFFLINE;
			}
			else
				m_pDevCmn->HostStatus = HOST_ONLINE;
			break;

		default:
			break;
	}



	switch(m_pDevCmn->AtmStatus)		
	{
	case ATM_INIT:
		m_pDevCmn->Timer.HostAliveTime = LINE_OPCL_TIME;	
		break;
	case ATM_CLERK:
	case ATM_READY:
	case ATM_ERROR:
	case ATM_CUSTOM:
		if(m_pDevCmn->HostStatusSave == HOST_OFFLINE) 
			return T_OK;
		if(m_pDevCmn->HostStatusSave == HOST_WAITLINK) 
			return T_OK;

		if (!m_pDevCmn->fnAPL_CheckHostOpen())						
			return T_OK;

		if(!m_pDevCmn->Timer.HostAliveTime)	
		{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%x], AtmStatusSave[%x]", m_pDevCmn->AtmStatus, m_pDevCmn->AtmStatusSave);	// test!!!!!
			try
			{			
				m_pTranCmn->fnAPP_TranAliveProc();
				m_pDevCmn->Timer.HostAliveTime =  LINE_OPCL_TIME; //30 Sec	
			}
			catch (CTranCmnExit& e)
			{	
				e.TranExitResult = e.TranExitResult;
			}
		}
		break;
	default:
		break;
	}


		
	if (m_pDevCmn->AtmStatus == ATM_CUSTOM)
	{
		if (m_pTranCmn->ForceSendStatus == STATUS_OFF_TIME)
		{
			m_pDevCmn->Timer.HostStatusSendTime     = STATUS_OFF_TIME;
		}
		else
		if((!m_pDevCmn->Timer.HostStatusSendTime) && (m_pTranCmn->ForceSendStatus == STATUS_ON_TIME))
		{
			try
			{	
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%x], AtmStatusSave[%x] m_pTranCmn->ForceSendStatus[%d]", m_pDevCmn->AtmStatus, m_pDevCmn->AtmStatusSave, m_pTranCmn->ForceSendStatus);	
				m_pTranCmn->ForceSendStatus         = STATUS_FORCE_SEND;
				m_pTranCmn->fnAPP_TranVirtualStatusProc();		
				m_pTranCmn->ForceSendStatus         = STATUS_OFF_TIME;
				m_pDevCmn->Timer.HostStatusSendTime = STATUS_OFF_TIME;
			}
			catch (CTranCmnExit& e)
			{	
				e.TranExitResult = e.TranExitResult;
			}
		}
		else
			MsgDump(TRACE_DUMP_MODE, "TMP", __FILE__, __LINE__, "AtmStatus[%x], AtmStatusSave[%x] m_pDevCmn->Timer.HostStatusSendTime[%d]", m_pDevCmn->AtmStatus, m_pDevCmn->AtmStatusSave, m_pDevCmn->Timer.HostStatusSendTime);	

	}

	return TRUE;
}


/***************************************************************************************************************
 * Rebooting (RPC) Logic func
 * Rebooting (3회) 1. Interval (30*30:900초 = 15분)+리부팅시간(60초 = 1분) = 16분 = 3 * 16분 = 48 분 [Off-Line]
 * Rebooting Retry 2. Interval 60분(1Hr) -> 270분(4시간30분) -> 450분(7시간30분) -> 620분(10시간30분)                                                             
 ***************************************************************************************************************/
int CWinAtmCtrl::AtmHostLinkCtrl()
{

	int nGetRegReBootCnt = 0;
	if(!m_pDevCmn->Timer.HostRebRetryTime)
	{
		m_pDevCmn->Timer.HostRebRetryTime = 0;
		nGetRegReBootCnt = RegGetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", 0);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmHostLinkCtrl Cnt Before[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
		
		if(nGetRegReBootCnt == 300)
			return TRUE;

		switch(nGetRegReBootCnt)
		{
		case 0: //5m
		case 1: //10m
		case 2: //15m
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 5: //30m
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 8: //45m 
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 11: //1hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 23: //2hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 35: //3hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 47: //4hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 59: //5hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 71: //6hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 83: //7hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 95: //8hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 107: //9hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 119: //10hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 131: //11hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 143: //12hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 155: //13hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 167: //14hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 179: //15hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 191: //16hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 203: //17hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 215: //18hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 227: //19hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 239: //20hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 251: //21hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 263: //22hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 275: //23hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		case 287: //24hr
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			MsgDump(TRACE_CODE_MODE, "REBOOTCNT", __FILE__, __LINE__, "AtmHostLinkCtrl[%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);	
			break;
		default:
			nGetRegReBootCnt += 1;
			RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", nGetRegReBootCnt);
			break;
		}
	}

	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "AtmHostLinkCtrl Return [%d][%d]", nGetRegReBootCnt,m_pDevCmn->Timer.HostRebRetryTime);
	return TRUE;
}