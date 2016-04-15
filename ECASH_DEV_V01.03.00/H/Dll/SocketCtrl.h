#if !defined(AFX_SOCKETCTRL_H__6E66A161_3EFF_11D5_A80C_00E09880ACB0__INCLUDED_)
#define AFX_SOCKETCTRL_H__6E66A161_3EFF_11D5_A80C_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketCtrl.h : header file
//

#define TCPRECVTBL				8
#define TCPBUFFSIZE				20480

#define DATA_FS					0xfe	/* Session Header Analysis           */

#define TCP_BASE_LEN			20				// BP -> ATM

#define TCP_CMD_DATA			0xfe			// BP <-> ATM
#define TCP_CMD_POLL			0xfd			// BP <-> ATM
#define TCP_CMD_RESET			0xf9			// BP  -> ATM
#define TCP_CMD_REQ				'R'				// BP <-  ATM	: request
#define TCP_CMD_OK				'O'				// BP  -> ATM	: HOST OK
#define TCP_CMD_NG				'N'				// BP  -> ATM	: HOST NG

#define TCP_CMD_ETX1			0xef			// ETX1
#define TCP_CMD_ETX2			0xcd			// ETX2

#define TCP_TOTAL_LEN			4
#define TCP_BCC_LEN				2


typedef struct tagTCPHeader
{
	BYTE	FrameStart;
	BYTE	DeviceAddr;
	BYTE	Command;
	BYTE	BranchID[5];
	BYTE	MachineID[3];
	BYTE	Destnation;
	BYTE	Sign;
	BYTE	Reserved[3];
	BYTE	Length[4];
	BYTE	DATA[TCPBUFFSIZE];					// DATA + BCC
} TCPHeader;

typedef struct _tagTCPNETINFO
{
	char	BpIpAddress[16];									// BP Ip Address
    char	AtmIPAddress[16];									// Atm IP Address
	int		BpPortNo;											// BP Port No
	BYTE	BranchID[5];
	BYTE	MachineID[3];
	BYTE	ServerNo;
} TCPNETINFO, *LPTCPNETINFO;


/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl command target

UINT ConnectCheck(LPVOID pParam);

class AFX_EXT_CLASS CSocketCtrl : public CAsyncSocket
{
// Attributes
public:
	BYTE	TCPDeviceAddress;
	int		TCPConnected;
	int		TCPCompleted;
	int		TCPErrID;
	
	int		TCPSendLength;						// Tcp Buffer		
	int		TCPRecvLength;						
	int		TCPSnodRecvLength;
	BYTE	TCPSendBuffer[TCPBUFFSIZE];
	BYTE	TCPRecvBuffer[TCPBUFFSIZE];			
	BYTE	TCPSnodRecvBuffer[TCPBUFFSIZE];
	
	int		DATASendLength;						// Tran Buffer
	int		PollSendLength;						//#N0275
	int		DATARecvLength;
	int		DATARecvLengthTbl[TCPRECVTBL];
	int		DATARecvInTbl;
	int		DATARecvInTblTemp;
	int		DATARecvOutTbl;
	BYTE	DATASendBuffer[TCPBUFFSIZE];
	BYTE	PollSendBuffer[TCPBUFFSIZE];		//#N0275
	BYTE	DATARecvBufferTbl[TCPRECVTBL][TCPBUFFSIZE];

	CRITICAL_SECTION	m_csLock;
	char	TCPBranchNum[5];
	char	TCPAtmSerialNum[3];
	BOOL	m_fDoingThread;

	int		RemoteLength;
	BYTE	RemoteBuffer[TCPBUFFSIZE];

private:
	WSADATA	WSAData;							/* Windows sockets info return      */

	
	//char m_SaveTcpInfo[16];						// TCP Network Info
	TCPNETINFO	m_SaveTcpInfo;						// TCP Network Info

	int		CheckRecvLength;					// Tran Recv Length
	int		CheckDataLength;					// Tran Data Length

	int		m_nGetIpChange;
	int		m_nServerNo;
	
//	int		m_nSignOnFlag;
	CWinThread * m_pThread;						// V01-02-02-#01
	DWORD	m_dwDataRecvTime;

	DWORD	m_dwCurRecvBytes;
	DWORD	m_dwTotRecvBytes;
	BYTE	*m_pRecvBuf;

// Operations
public:
	CSocketCtrl();
	virtual ~CSocketCtrl();

//	static UINT ConnectCheck(LPVOID pParam);
	
	int InitSocket();
	int OpenSocket(char *IpAddress, int PortNumber, TCPNETINFO szTcpNetInfo);
	int CloseSocket();
	int SendOpenSocket();
	int SendSocket(/*char *IPAddress,*/ BYTE *SendBuff, int Length);
	int PollSendSocket(/*char *IPAddress,*/ BYTE *SendBuff, int Length);	//#N0275
	int DATARecvSocket(BYTE *RecvBuff, int *Length);

	int GetIpChange();
	int SetIpChange(BOOL bFlag);
	DWORD GetDataRecvTime();
	int SetDataRecvTime();
	DWORD RecvTimeCheck(DWORD dwCheckTime);
	BOOL fnAPP_IsBCC(BYTE* pbyBuf, DWORD dwBufSize);
	int GetRemoteData(int nTrCode = FALSE);
	int ClearRemoteData();

private:
	int ProcessData();
	int ParseData();
	int CheckData();
	int RespPollData(int Cmd);
	int CheckTCPError();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketCtrl)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSocketCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETCTRL_H__6E66A161_3EFF_11D5_A80C_00E09880ACB0__INCLUDED_)
