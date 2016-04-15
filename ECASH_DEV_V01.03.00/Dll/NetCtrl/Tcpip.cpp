// Tcpip.cpp: implementation of the CTcpip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Dll\Tcpip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


UINT ICMP_POLL_Status(LPVOID pParam);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTcpip::CTcpip()
{
	m_InitOpen		= FALSE;
	m_LineStatus	= LINE_CLOSE;

	m_pSocketCtrl	= new CSocketCtrl();
}

CTcpip::~CTcpip()
{
	if (m_pSocketCtrl)
	{
		m_pSocketCtrl->CloseSocket();
		delete m_pSocketCtrl;
		m_pSocketCtrl	= NULL;
	}
}

//////////////////////////////////////////////////////////////////////
int CTcpip::LineOpen(LPTCPNETINFO szTcpNetInfo, int OpenSec)
{
	memcpy(&m_SaveTcpInfo.BpIpAddress[0], &szTcpNetInfo->BpIpAddress[0], sizeof(TCPNETINFO));
	LineClose(OpenSec);
	Delay_Msg(500);											
	m_InitOpen = TRUE;
																
	return (m_pSocketCtrl->OpenSocket(m_SaveTcpInfo.BpIpAddress, m_SaveTcpInfo.BpPortNo, m_SaveTcpInfo));
}

int CTcpip::LineClose(int CloseSec)
{
	if (m_InitOpen)												
		return (m_pSocketCtrl->CloseSocket());

	return TRUE;
}


int CTcpip::PollSendData(BYTE *SendBuff, int Length, int SendSec)
{
	int		Ret;
	DWORD	WaitTime;

	Ret = m_pSocketCtrl->SendSocket(/*m_SaveTcpInfo.AtmIPAddress, */SendBuff, Length);
	if(!Ret)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] PollSendData Fail0");
		m_pSocketCtrl->TCPConnected = FALSE;
		return FALSE;
	}

	WaitTime = GetTickCount() + 1000 * SendSec;
	do
	{
		Delay_Msg();
		GetStatus();										
		if (!m_pSocketCtrl->PollSendLength)
			break;
	} while (GetTickCount() < WaitTime);

	Ret = m_pSocketCtrl->PollSendLength;
	m_pSocketCtrl->PollSendLength = 0;							
	if (!Ret)												
		return TRUE;
	else 
	{															
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] PollSendData Fail1");
		m_pSocketCtrl->TCPConnected = FALSE;
		return FALSE;
	}
}

int CTcpip::SendData(BYTE *SendBuff, int Length, int SendSec)
{
	int		Ret;
	DWORD	WaitTime;

	m_pSocketCtrl->DATARecvLength = 0;							

	Ret = m_pSocketCtrl->SendSocket(/*m_SaveTcpInfo.AtmIPAddress, */SendBuff, Length);
	if(!Ret)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Send Fail0");
		m_pSocketCtrl->TCPConnected = FALSE;
		return FALSE;
	}

	WaitTime = GetTickCount() + 1000 * SendSec;
	do
	{
		Delay_Msg();
		GetStatus();											
		if (!m_pSocketCtrl->DATASendLength)
			break;
	} while (GetTickCount() < WaitTime);

	Ret = m_pSocketCtrl->DATASendLength;
	m_pSocketCtrl->DATASendLength = 0;							
	if (!Ret)												
		return TRUE;
	else 
	{															
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Send Fail1");
		m_pSocketCtrl->TCPConnected = FALSE;
		return FALSE;
	}
}

int CTcpip::RecvData(BYTE *RecvBuff, int *Length, int RecvSec)
{
	int		Ret;
	DWORD	WaitTime;
MsgDump(TRACE_DUMP_MODE, "log",__FILE__, __LINE__, "[TCP/IP] Recv Fail RecvSec (%d)", RecvSec);

	if (RecvSec)
	{
		WaitTime = GetTickCount() + 1000 * RecvSec;
		do
		{
			Delay_Msg();
			GetStatus();									
// 			if (GetStatus() == LINE_CLOSE)
// 			{
// 				if (RecvSec)										
// 				{
// 					MsgDump(TRACE_CODE_MODE, "log",__FILE__, __LINE__, "[TCP/IP] Connection LINE Close");
// 					m_pSocketCtrl->TCPConnected = FALSE;
// 				}
// 				*Length = 0;
// 				return FALSE;
// 			}

			if (m_pSocketCtrl->DATARecvInTbl != m_pSocketCtrl->DATARecvOutTbl)
			{
				MsgDump(TRACE_DUMP_MODE, "log",__FILE__, __LINE__, "[TCP/IP] DataRecvInTbl Buffer (unmatch (%d %d)",m_pSocketCtrl->DATARecvInTbl, m_pSocketCtrl->DATARecvOutTbl);
				break;
			}
//			if (m_pSocketCtrl->DATARecvLength)
//				break;
		} while (GetTickCount() < WaitTime);
	}
	else
	{
		//GetStatus();										
		if (GetStatus() == LINE_CLOSE)
		{
			if (RecvSec)											
			{
				MsgDump(TRACE_DUMP_MODE, "log",__FILE__, __LINE__, "[TCP/IP] Connection LINE Close __");
				m_pSocketCtrl->TCPConnected = FALSE;
			}
			*Length = 0;
			return FALSE;
		}
	}

	Ret = m_pSocketCtrl->DATARecvSocket(RecvBuff, Length);		
	if (Ret)													
	{
		MsgDump(TRACE_DUMP_MODE, "log",__FILE__, __LINE__, "[TCP/IP] Data Recv");
		m_pSocketCtrl->DATARecvLength = 0;						
		return TRUE;
	}
	else 
	{														
		if (RecvSec)											
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP/IP] Recv Fail");
			m_pSocketCtrl->TCPConnected = FALSE;
		}
		*Length = 0;
		return FALSE;
	}
}

int CTcpip::GetStatus()
{
	if ((m_pSocketCtrl->TCPConnected) &&						
		(m_pSocketCtrl->TCPCompleted))						
		m_LineStatus = LINE_OPEN;
	else
		m_LineStatus = LINE_CLOSE;

	return m_LineStatus;
}

int CTcpip::GetIpChange()
{
	return m_pSocketCtrl->GetIpChange();
}

int CTcpip::SetIpChange(BOOL fChangeIP)
{
	m_pSocketCtrl->SetDataRecvTime();
	return m_pSocketCtrl->SetIpChange(fChangeIP);
}


int	CTcpip::ICMPStatus(int nFlag)
{
	int nRet = 0;
	if ((m_pSocketCtrl->TCPConnected) &&					
		(m_pSocketCtrl->TCPCompleted))
	{
		if(!nFlag)
		{
			MsgDump(TRACE_CODE_MODE, "PING", __FILE__, __LINE__, "ICMPStatus AfxBeginThread Start !");
			AfxBeginThread(ICMP_POLL_Status, this, THREAD_PRIORITY_NORMAL);
		}
		else
		{
			for(int i = 0; i < 3; i++ )
			{
				nRet = m_ping.Ping(TCP_PING_RETRY, m_SaveTcpInfo.BpIpAddress, m_hWnd);
				if (!nRet)
				{
					MsgDump(TRACE_CODE_MODE, "PING", __FILE__, __LINE__, "ICMPStatus       nRet(%d) [ Ping: %s]", nRet, m_ping.strErrorMsg.GetBuffer(0));
					continue;
				}
				else
					break;
			}
		}
	}
	
	return nRet;
}


UINT ICMP_POLL_Status(LPVOID pParam)
{
	int nRet = 0;
	CTcpip*	pTcpIp = (CTcpip*)pParam;
	
	MsgDump(TRACE_DUMP_MODE, "PING", __FILE__, __LINE__, "ICMP_POLL_Status Start - 1 ");

	for(int i = 0; i < 6; i++ )
	{
		nRet = pTcpIp->m_ping.Ping(TCP_PING_RETRY, pTcpIp->m_SaveTcpInfo.BpIpAddress, pTcpIp->m_hWnd);
		MsgDump(TRACE_DUMP_MODE, "PING", __FILE__, __LINE__, "ICMP_POLL_Status nRet(%d) [ Ping: %s]", nRet, pTcpIp->m_ping.strErrorMsg.GetBuffer(0));

		if (!nRet) 
			continue;
		else
			break;
	}
	
	
	MsgDump(TRACE_DUMP_MODE, "PING", __FILE__, __LINE__, "ICMP_POLL_Status Start - 2 ");
	AfxEndThread(0);
	MsgDump(TRACE_DUMP_MODE, "PING", __FILE__, __LINE__, "ICMP_POLL_Status Start - 3 ");
	return 0;
}

int CTcpip::GetRemoteData(BYTE *DstData)
{
	if (m_pSocketCtrl->RemoteLength)
	{
		memcpy(DstData, m_pSocketCtrl->RemoteBuffer, m_pSocketCtrl->RemoteLength);
		return m_pSocketCtrl->RemoteLength;
	}
	
	return FALSE;
}

int CTcpip::ClearRemoteData()
{
	m_pSocketCtrl->ClearRemoteData();
	return FALSE;
}