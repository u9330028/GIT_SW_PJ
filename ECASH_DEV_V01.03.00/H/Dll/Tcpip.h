// Tcpip.h: interface for the CTcpip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPIP_H__7A3C6C85_3D61_11D5_A80C_00E09880ACB0__INCLUDED_)
#define AFX_TCPIP_H__7A3C6C85_3D61_11D5_A80C_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ping.h"
#include "SocketCtrl.h"

#define TCP_PING_RETRY 2
/*
typedef struct _tagTCPNETINFO
{
	char	BpIpAddress[16];									// BP Ip Address
    char	AtmIPAddress[16];									// Atm IP Address
	int		BpPortNo;											// BP Port No
} TCPNETINFO, *LPTCPNETINFO;
*/
class AFX_EXT_CLASS CTcpip
{
public:
	CTcpip();
	virtual ~CTcpip();

	int 		LineOpen(LPTCPNETINFO szTcpNetInfo, int OpenSec);
	int 		LineClose(int CloseSec);
	int 		SendData(BYTE *SendBuff, int Length, int SendSec);
	int 		PollSendData(BYTE *SendBuff, int Length, int SendSec);	
	int 		RecvData(BYTE *RecvBuff, int *Length, int RecvSec);
	int 		GetStatus();
	int			GetIpChange();
	int			SetIpChange(BOOL fChangeIP);
	int 		ICMPStatus(int nFlag = FALSE);
	int			GetRemoteData(BYTE *DstData);
	int			ClearRemoteData();
public:
	CPing			m_ping;
	TCPNETINFO		m_SaveTcpInfo;									// TCP Network Info
	HWND			m_hWnd;
	UINT			m_nRetries;

private:
	CSocketCtrl*	m_pSocketCtrl;


	int			m_InitOpen;
	int			m_LineStatus;

};

#endif // !defined(AFX_TCPIP_H__7A3C6C85_3D61_11D5_A80C_00E09880ACB0__INCLUDED_)
