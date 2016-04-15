// NetWork.h: interface for the CNetWork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORK_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_)
#define AFX_NETWORK_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Profiledef.h"
#include "Profile.h"

class CTcpip;

class AFX_EXT_CLASS CNetWork  
{
public:
	CNetWork(NETWORKINFO NETWORK);
	virtual ~CNetWork();

	int 		LineOpen(int OpenSec, NETWORKINFO *pNETWORK);
	int 		LineClose(int CloseSec);
	int 		SendData(BYTE *SendBuff, int Length, int SendSec);
	int 		PollSendData(BYTE *SendBuff, int Length, int SendSec);			
	int 		RecvData(BYTE *RecvBuff, int *Length, int RecvSec);
	int 		GetStatus();
	int			ICMP_Status(int nSyncFlag);
	int			GetRemoteData(BYTE *GetBidData);
	int			ClearRemoteData();
	
private:

	CTcpip*		m_pTcpip;

	NETWORKINFO	m_ConfBuff;
	int			m_nServerNo;
};

#endif // !defined(AFX_NETWORK_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_)
