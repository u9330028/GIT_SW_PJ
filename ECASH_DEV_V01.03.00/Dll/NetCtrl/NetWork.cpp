// NetWork.cpp: implementation of the CNetWork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Dll\Tcpip.h"

#include "..\..\H\Dll\NetWork.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetWork::CNetWork(NETWORKINFO NETWORK)
{

	m_ConfBuff		= NETWORK;
	m_pTcpip		= NULL;


MsgDump(TRACE_CODE_MODE, "log",__FILE__,__LINE__,"[NETWORK:CNetWork()]  Interface = [%x]", m_ConfBuff.Interface);
	switch (m_ConfBuff.Interface)
	{
			break;
		case	NET_TCPIP:
			m_pTcpip = new CTcpip();
			m_nServerNo = 0;
			m_pTcpip->SetIpChange(TRUE);
			break;
		case	NET_BAS:
			break;
		default:
			break;
	}
}

CNetWork::~CNetWork()
{
	switch (m_ConfBuff.Interface)
	{
		case NET_TCPIP:
			if (m_pTcpip)		{	delete m_pTcpip;		m_pTcpip		= NULL;	}
			break;
		default:
			break;
	}
}


int CNetWork::LineOpen(int OpenSec, NETWORKINFO *pNETWORK)
{
	TCPNETINFO	TcpNetInfo;									
	int			i = 0;
	char		temp[10] = {0,};
	char		IpAddress[256] = {0,};
	int			PortNo = 0;

MsgDump(TRACE_CODE_MODE, "log",__FILE__,__LINE__,"[NETWORK:LineOpen()]  Interface = [%x]", m_ConfBuff.Interface);

	switch (m_ConfBuff.Interface)
	{

		case NET_TCPIP:
MsgDump(TRACE_CODE_MODE, "log",__FILE__,__LINE__,"[NETWORK:LineOpen()]  IP Change = [%d][%x]", m_pTcpip->GetIpChange(), m_nServerNo);
			if ((m_pTcpip->GetIpChange()) ||
				((!m_nServerNo) && ((!memcmp(m_ConfBuff.BpCurIpAddress, "0000", 4)) || (strlen(m_ConfBuff.BpCurIpAddress) < 15))))
			{
				m_pTcpip->SetIpChange(FALSE);
				if (!m_nServerNo)	// if no set address then 'a'
				{
					m_nServerNo = 'A';
				}
				else
				{
					m_nServerNo++;
					if ((m_nServerNo < 'A') || (m_nServerNo > 'B'))
						m_nServerNo = 'A';
				}

				if ((m_nServerNo < 'A') || (m_nServerNo > 'B')) // if 'a' < address <= 'c' then 'a'
				{
					m_nServerNo = 'A';
				}

				switch (m_nServerNo)
				{
				case 'A':
					memcpy(m_ConfBuff.BpCurIpAddress, m_ConfBuff.BpIpAddress, sizeof(m_ConfBuff.BpCurIpAddress));
					memcpy(m_ConfBuff.BpCurPortNo, m_ConfBuff.BpPortNo, sizeof(m_ConfBuff.BpCurPortNo));
					break;
				case 'B':
				default:
					memcpy(m_ConfBuff.BpCurIpAddress, m_ConfBuff.BpIpAddress2, sizeof(m_ConfBuff.BpCurIpAddress));
					memcpy(m_ConfBuff.BpCurPortNo, m_ConfBuff.BpPortNo2, sizeof(m_ConfBuff.BpCurPortNo));
					break;
				}
				memcpy(pNETWORK->BpCurIpAddress, m_ConfBuff.BpCurIpAddress, sizeof(m_ConfBuff.BpCurIpAddress));
				memcpy(pNETWORK->BpCurPortNo, m_ConfBuff.BpCurPortNo, sizeof(m_ConfBuff.BpCurPortNo));
			}
			TcpNetInfo.ServerNo = m_nServerNo;
			sprintf(IpAddress, "%d.%d.%d.%d", Asc2Int(&m_ConfBuff.BpCurIpAddress[0], 3),
											  Asc2Int(&m_ConfBuff.BpCurIpAddress[4], 3),
											  Asc2Int(&m_ConfBuff.BpCurIpAddress[8], 3),
											  Asc2Int(&m_ConfBuff.BpCurIpAddress[12], 3));
			PortNo = atoi(m_ConfBuff.BpCurPortNo);

			strcpy(TcpNetInfo.BpIpAddress, IpAddress);
			TcpNetInfo.BpPortNo = PortNo;
			Sprintf(TcpNetInfo.AtmIPAddress, 16, "%d.%d.%d.%d",
											  Asc2Int(&m_ConfBuff.AtmIPAddress[0], 3),
											  Asc2Int(&m_ConfBuff.AtmIPAddress[4], 3),
											  Asc2Int(&m_ConfBuff.AtmIPAddress[8], 3),
											  Asc2Int(&m_ConfBuff.AtmIPAddress[12], 3));
			i = strlen(TcpNetInfo.AtmIPAddress);
			if (i < 16)
				memset(&TcpNetInfo.AtmIPAddress[i], 0x20, 16 - i);
			else
			{
				TcpNetInfo.AtmIPAddress[15] = NULL;
			}

			Sprintf(temp, 9, "%05d", Asc2Int(&m_ConfBuff.SubBranchNum[0], 8));			
			memcpy(TcpNetInfo.BranchID, temp, 5);
			Sprintf(temp, 9, "%03d", Asc2Int(&m_ConfBuff.AtmSerialNum[0], 8));			
			memcpy(TcpNetInfo.MachineID, temp, 3);

			return (m_pTcpip->LineOpen((LPTCPNETINFO)&TcpNetInfo, OpenSec));
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::LineClose(int CloseSec)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_TCPIP:
			return (m_pTcpip->LineClose(CloseSec));
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::SendData(BYTE *SendBuff, int Length, int SendSec)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_TCPIP:
			return (m_pTcpip->SendData(SendBuff, Length, SendSec));
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::PollSendData(BYTE *SendBuff, int Length, int SendSec)
{
	return (m_pTcpip->PollSendData(SendBuff, Length, SendSec));
}
	
int CNetWork::RecvData(BYTE *RecvBuff, int *Length, int RecvSec)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_TCPIP:
			return (m_pTcpip->RecvData(RecvBuff, Length, RecvSec));
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::GetStatus()
{
	switch (m_ConfBuff.Interface)
	{
		case NET_TCPIP:
			return (m_pTcpip->GetStatus());
			break;
		default:
			break;
	}
	return FALSE;
}


int CNetWork::ICMP_Status(int nSyncFlag)
{
	switch (m_ConfBuff.Interface)
	{
	case NET_TCPIP:
		return (m_pTcpip->ICMPStatus(nSyncFlag));
		break;
	default:
		break;
	}
	return FALSE;
}

int CNetWork::GetRemoteData(BYTE *GetBidData)
{
	if (m_ConfBuff.Interface == NET_TCPIP)
	{
		return (m_pTcpip->GetRemoteData(GetBidData));
	}

	return FALSE;
}

int CNetWork::ClearRemoteData()
{
	if (m_ConfBuff.Interface == NET_TCPIP)
		m_pTcpip->ClearRemoteData();

	return FALSE;
}