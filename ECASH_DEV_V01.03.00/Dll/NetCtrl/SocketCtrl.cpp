// SocketCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Dll\SocketCtrl.h"
#include "..\..\H\Tran\TranCmnHost.h"
#include "..\..\H\Common\ConstDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		POLL_RECV_DATA_SIZE				3						// TCP Header(fe fe)포함
#define		HEADER_SENDRECV_DATA_SIZE		4						// Header Format Send Data Size

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl

CSocketCtrl::CSocketCtrl()
{
	m_fDoingThread = TRUE;
	InitializeCriticalSection(&m_csLock);			//#N0275
	InitSocket();												// 변수초기화

	m_nServerNo = NULL;										
	m_dwDataRecvTime = NULL;
//	m_nSignOnFlag = FALSE;
	m_pThread = NULL;
}

CSocketCtrl::~CSocketCtrl()
{
	DeleteCriticalSection(&m_csLock);	//#N0275
	if(m_pThread)
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP/IP] : ~CSocketCtrl()");
		m_fDoingThread = FALSE;
		m_pThread = NULL;
	}
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSocketCtrl, CAsyncSocket)
	//{{AFX_MSG_MAP(CSocketCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl member functions

void CSocketCtrl::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TCP OnAccept ERROR] [%d]", nErrorCode);

	MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TCP/IP] OnAccept");
	CAsyncSocket::OnAccept(nErrorCode);
}

void CSocketCtrl::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TCP OnClose ERROR] [%d]", nErrorCode);
	
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP/IP] OnClose");
	TCPConnected = FALSE;

	CAsyncSocket::OnClose(nErrorCode);
}

void CSocketCtrl::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP OnConnect ERROR] [%d]", nErrorCode);

MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP/IP] OnConnect");
	
	CAsyncSocket::OnConnect(nErrorCode);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP OnConnect ERROR] [%d]", nErrorCode);

		
    if (!nErrorCode)
	{
		TCPConnected = TRUE;
		TCPCompleted = TRUE;
//		SetIpChange(FALSE);
	}
	else
	{
		TCPConnected = FALSE;
		TCPCompleted = TRUE;
	}
}

void CSocketCtrl::OnOutOfBandData(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TCP OnOutOfBandData ERROR] [%d]", nErrorCode);
	
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CSocketCtrl::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP OnReceive ERROR] [%d]", nErrorCode);

	if (!nErrorCode)
		ParseData();
	    //ProcessData();
	
	CAsyncSocket::OnReceive(nErrorCode);
}

void CSocketCtrl::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TCP OnSend ERROR] [%d]", nErrorCode);	

	CAsyncSocket::OnSend(nErrorCode);
}

int CSocketCtrl::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class

	int nByte = CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
	int TCPError;

	if (nByte <= 0)
	{
		TCPError = CheckTCPError();
		if (TCPError == 10035) nByte = 0;								// 장애아님
		else MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP Receive ERROR] [%d]", TCPError);
	}

	return nByte;
}

int CSocketCtrl::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nBufLen > 40)	
HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCPIP SENDDATA]", (char*)lpBuf, nBufLen);
	
	int nByte = CAsyncSocket::Send(lpBuf, nBufLen, nFlags);

	if (nByte != nBufLen)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP Send ERROR] [%d]", CheckTCPError());

	if (nBufLen > 10)
MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TCPIP CAsyncSocket::Send Return] [%d]", nByte);

	return nByte;
}

/////////////////////////////////////////////////////////////////////////////
int CSocketCtrl::InitSocket()
{
	TCPDeviceAddress = 0;
	TCPConnected = FALSE;
	TCPCompleted = TRUE;
	//TCPCompleted = FALSE;
	TCPErrID = 0;

	TCPSendLength = 0;
	TCPRecvLength = 0;
	TCPSnodRecvLength = 0;
	memset(TCPSendBuffer, 0, TCPBUFFSIZE);
	memset(TCPRecvBuffer, 0, TCPBUFFSIZE);
	memset(TCPSnodRecvBuffer, 0, TCPBUFFSIZE);

	DATASendLength = 0;
	PollSendLength = 0;	//#N0275
	DATARecvLength = 0;
	memset(DATARecvLengthTbl, 0, sizeof(DATARecvLengthTbl));
	DATARecvInTbl = 0;
	DATARecvInTblTemp = 0;
	DATARecvOutTbl = 0;
	memset(DATASendBuffer, 0, TCPBUFFSIZE);
	memset(DATARecvBufferTbl, 0, sizeof(DATARecvBufferTbl));
	memset(&m_SaveTcpInfo, 0x00, sizeof(m_SaveTcpInfo));
	
	CheckRecvLength = 0;									// Tran Recv Length
	CheckDataLength = 0;									// Tran Data Length

	m_dwTotRecvBytes = 0;
	m_dwCurRecvBytes = 0;

	RemoteLength = 0;
	memset(RemoteBuffer, 0, TCPBUFFSIZE);

	return TRUE;
}

int CSocketCtrl::OpenSocket(char *IpAddress, int PortNumber, TCPNETINFO szTcpNetInfo)
{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP/IP] OpenSocket Ip:%s Port:%d", IpAddress, PortNumber);


	WSAStartup(MAKEWORD(1,1), &WSAData);
	InitSocket();
	TCPDeviceAddress = 0;	// 고정값사용
	m_nServerNo = szTcpNetInfo.ServerNo;
	if (!Socket(SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_CONNECT | FD_CLOSE))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP Socket ERROR] [%d]", CheckTCPError());
		return FALSE;
	}

	if (!Connect((LPCTSTR)IpAddress, PortNumber))
	{
		if (GetLastError() != WSAEWOULDBLOCK)
		{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP Connect ERROR] [%d]", CheckTCPError());
			return FALSE;
		}
	}

	ULONG block = TRUE;
	if (!IOCtl(FIONBIO, &block))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP IOCtl ERROR] [%d]", CheckTCPError());
		return FALSE;
	}

	return TRUE;
}

int CSocketCtrl::CloseSocket()
{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP/IP] CloseSocket");

	ShutDown(CAsyncSocket::both);
	Close();

	Delay_Msg(500);

	InitSocket();
	WSACleanup();

	return TRUE;
}

int CSocketCtrl::SendSocket(/*char *IPAddress,*/ BYTE *SendBuff, int Length)
{
	int nReturn;
	
	EnterCriticalSection(&m_csLock);		//#N0275
	DATASendLength = 0;	
	
	m_dwTotRecvBytes = 0;
	m_dwCurRecvBytes = 0;

	
	memset(DATASendBuffer, 0, TCPBUFFSIZE);					
	
	memcpy(&DATASendBuffer[DATASendLength], SendBuff, Length);
	DATASendLength += Length;



	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TCPIP SENDDATA]", DATASendBuffer, DATASendLength);

	if (Send(DATASendBuffer, DATASendLength) != DATASendLength)
	{
		DWORD dwError = GetLastError();
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "SendSocket GetLastError(%d)", dwError);
		nReturn = FALSE;
	}
	else
		nReturn = TRUE;
	
	DATASendLength = 0;											
	LeaveCriticalSection(&m_csLock);		//#N0275

	return nReturn;
}


int CSocketCtrl::PollSendSocket(/*char *IPAddress,*/ BYTE *SendBuff, int Length)
{
	int nReturn;
	PollSendLength = 0;

	memset(PollSendBuffer, 0, TCPBUFFSIZE);					
	
	memcpy(&PollSendBuffer[0], SendBuff, Length);

	PollSendLength += Length;

	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TCPIP SENDDATA]", PollSendBuffer, Length);

	if (Send(PollSendBuffer, PollSendLength) != PollSendLength)
	{
		DWORD dwError = GetLastError();
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "PollSendSocket GetLastError(%d)", dwError);
		nReturn = FALSE;
	}
	else
		nReturn = TRUE;
	
	PollSendLength = 0;
	
	return nReturn;
}

int CSocketCtrl::DATARecvSocket(BYTE *RecvBuff, int *Length)
{
	if (DATARecvInTbl == DATARecvOutTbl)					
	{
		*Length = 0;
		DATARecvLength = 0;
		return FALSE;
	}

	memcpy(RecvBuff, DATARecvBufferTbl[DATARecvOutTbl], DATARecvLengthTbl[DATARecvOutTbl]);
	*Length = DATARecvLengthTbl[DATARecvOutTbl];
	DATARecvLength = DATARecvLengthTbl[DATARecvOutTbl];
	DATARecvOutTbl++;
	if (DATARecvOutTbl >= TCPRECVTBL)							
		DATARecvOutTbl = 0;

	if (!DATARecvLength)
		return FALSE;

	return TRUE;
}


int CSocketCtrl::ParseData()
{
	UINT nRecv = 0, nBufIdx = 0;
	int nSize;
	BYTE m_byTempBuf[TCPBUFFSIZE] = {0x00,};
	BYTE m_byTempBuf1[TCPBUFFSIZE] = {0x00,};
	BYTE m_byTempBuf2[TCPBUFFSIZE] = {0x00,};
	//IOCtl(FIONREAD, &m_dwTotRecvBytes);

	memset(m_byTempBuf, 0x00, sizeof(m_byTempBuf));
	nRecv = Receive(m_byTempBuf, sizeof(m_byTempBuf));
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "Receive Data", m_byTempBuf, nRecv);

	if ((nRecv == SOCKET_ERROR) || (nRecv == 0))
	{
		DWORD dwError = GetLastError();
		if (GetLastError() == WSAEWOULDBLOCK)
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "OnReceive():WSAEWOULDBLOCK");
		else
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "OnReceive():SOCKET_ERROR");

		return FALSE;
	}

	if ((m_dwTotRecvBytes == 0) && (m_dwCurRecvBytes == 0))
	{
		nSize= Asc2Int(m_byTempBuf, TCP_TOTAL_LEN);
		m_dwTotRecvBytes = nSize + TCP_TOTAL_LEN;

		if (m_dwTotRecvBytes <= 0)
			return FALSE;

		if (m_pRecvBuf)
			delete m_pRecvBuf;

		m_pRecvBuf = new BYTE[m_dwTotRecvBytes];
		memset(m_pRecvBuf, 0x00, m_dwTotRecvBytes);
	}
	
	while(TRUE)
	{
		if ((m_dwTotRecvBytes - m_dwCurRecvBytes) > (nRecv- nBufIdx))
		{
			memcpy(&m_pRecvBuf[m_dwCurRecvBytes], &m_byTempBuf[nBufIdx], (nRecv - nBufIdx));
			m_dwCurRecvBytes += (nRecv - nBufIdx);
			nBufIdx += (nRecv - nBufIdx);
			break;
		}
		else
		{
			memcpy(&m_pRecvBuf[m_dwCurRecvBytes], &m_byTempBuf[nBufIdx], m_dwTotRecvBytes - m_dwCurRecvBytes);
			nBufIdx += m_dwTotRecvBytes - m_dwCurRecvBytes;
			m_dwCurRecvBytes += (m_dwTotRecvBytes - m_dwCurRecvBytes);
		}

		if (m_dwTotRecvBytes == m_dwCurRecvBytes)
		{

			m_byTempBuf1[0] = MakeBcc(&m_pRecvBuf[TCP_TOTAL_LEN], m_dwTotRecvBytes - TCP_TOTAL_LEN - TCP_BCC_LEN);
			MakeUnPack(m_byTempBuf1, m_byTempBuf2, 1);
			if(memcmp(&m_pRecvBuf[m_dwTotRecvBytes - TCP_BCC_LEN], m_byTempBuf2, TCP_BCC_LEN) != 0)
			{
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "OnReceive():Receive Data Terminator Error!");
//				HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "Receive Data", m_pRecvBuf, nRecv);
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "OnReceive():Receive Data Terminator Error! (%d)", nRecv);
				delete [] m_pRecvBuf;		
				m_pRecvBuf = NULL;
				m_dwTotRecvBytes = 0;
				m_dwCurRecvBytes = 0;
				break;
			}
			else
			{
				memcpy(TCPRecvBuffer, m_pRecvBuf, m_dwTotRecvBytes);
				TCPRecvLength += m_dwTotRecvBytes;

				RegSetStr(_REGKEY_REMOTECTRL, "BID_RECV_RESULT", "DO");
				GetRemoteData();

				CheckData();		
				
				TCPRecvLength = 0;
				memset(TCPRecvBuffer, 0x00, TCPBUFFSIZE);

				delete [] m_pRecvBuf;			
				m_pRecvBuf = NULL;
				m_dwTotRecvBytes = 0;
				m_dwCurRecvBytes = 0;

				if (nBufIdx < nRecv)
				{
					MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "OnReceive():Receive Data Remained!");
					nSize = Asc2Int(m_byTempBuf, TCP_TOTAL_LEN);
					m_dwTotRecvBytes = nSize + TCP_TOTAL_LEN;
					m_dwCurRecvBytes = 0;
					m_pRecvBuf = new BYTE[m_dwTotRecvBytes];
					memset(m_pRecvBuf, 0x00, m_dwTotRecvBytes);
				}
				else
					break;
			}

		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CSocketCtrl::ProcessData()
{

	BYTE	TempBuffer[TCPBUFFSIZE] = {0x00,};
	int		nByte;
	int		nHRecvLen = 0;
	ULONG	WiatTime = TimerSet(K_10_WAIT);
	while (TRUE)
	{
		nByte = Receive(TempBuffer, TCPBUFFSIZE);

		if (CheckTimer(WiatTime))
		{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCPIP RECVDATA TIME OVER]");
			TCPConnected = FALSE;
			break;
		}
		
		if (nByte <= 0)
		{
MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TCPIP RECVDATA %d]", TCPErrID);
			if (TCPErrID == 10035)
			{
				continue;
			}
			else 
				break;
		}
		

		if(nHRecvLen == 0)
		{
			nHRecvLen =  Asc2Int(TempBuffer, TCP_TOTAL_LEN);
		}

		if (nByte > HEADER_SENDRECV_DATA_SIZE)
HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCPIP RECVDATA]", TempBuffer, nByte);

		if ((TCPRecvLength + nByte) < TCPBUFFSIZE)
		{
			memcpy(&TCPRecvBuffer[TCPRecvLength], TempBuffer, nByte);
			TCPRecvLength += nByte;
		}
		
		if(nHRecvLen <= TCPRecvLength)
		{
MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TCPIP RECVDATA 2]");
			break;
		}
		
	}


	if (CheckData())		
	{
		TCPRecvLength = 0;
		memset(TCPRecvBuffer, 0x00, TCPBUFFSIZE);
	}
	else
	{
		TCPRecvLength = 0;
		memset(TCPRecvBuffer, 0x00, TCPBUFFSIZE);
	}		
	
	return TRUE;
}


int CSocketCtrl::CheckData()
{
MsgDump(TRACE_CODE_MODE, "log",__FILE__,__LINE__,"[TCP/IP] Enter CheckData()");

	WORD	nWordDataLen = 0;
	BYTE	nTempBuffer[TCPBUFFSIZE] = {0x00,};
	int		nCnt = NULL;										// V01-00-00-#302

	CheckRecvLength = TCPRecvLength;							// Tran Recv Length
	CheckDataLength = TCPRecvLength;										// Tran Data Length

	if (TCPRecvLength < HEADER_SENDRECV_DATA_SIZE)				// Size Check 6바이트 미만 LENGTH
		return FALSE;											// 대기


	if (CheckDataLength > TCPBUFFSIZE)							// 자료길이보정
		CheckDataLength = TCPBUFFSIZE;
	if (CheckRecvLength > TCPBUFFSIZE)							// 전체길이보정
		CheckRecvLength = TCPBUFFSIZE;

if (TCPRecvLength > HEADER_SENDRECV_DATA_SIZE)
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[TCP/IP] CheckData() TCPRecvLength[%d], CheckRecvLength[%d]", TCPRecvLength, CheckRecvLength);


	if (TCPRecvLength < CheckRecvLength)					
		return FALSE;											

	if ((TCPSnodRecvLength + CheckDataLength) < TCPBUFFSIZE)
	{
		memcpy(&TCPSnodRecvBuffer[TCPSnodRecvLength], TCPRecvBuffer, TCPRecvLength);
		TCPSnodRecvLength += TCPRecvLength;
	}

	DATARecvInTblTemp = DATARecvInTbl;
	DATARecvInTblTemp++;
	if (DATARecvInTblTemp >= TCPRECVTBL)			
		DATARecvInTblTemp = 0;

	if (DATARecvInTblTemp != DATARecvOutTbl)			
	{
		memcpy(DATARecvBufferTbl[DATARecvInTbl], TCPSnodRecvBuffer, TCPSnodRecvLength);
		DATARecvLengthTbl[DATARecvInTbl] = TCPSnodRecvLength;

		DATARecvInTbl = DATARecvInTblTemp;			
	}
	TCPSnodRecvLength = 0;							

	return TRUE;											
}	

int CSocketCtrl::GetRemoteData(int nTrCode)
{
	if (TCPRecvLength <= 0)	return FALSE;

	memcpy(RemoteBuffer, TCPRecvBuffer, TCPRecvLength);
	RemoteLength = TCPRecvLength;

	return FALSE;
}

int CSocketCtrl::ClearRemoteData()
{

	RemoteLength = 0;
	memset(RemoteBuffer, 0, TCPBUFFSIZE);

	return FALSE;
}

int CSocketCtrl::RespPollData(int Cmd)
{
MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[TCP/IP] RespPollData CMD[%x]", Cmd);


	return TRUE;												// Buffer Clear
}

int CSocketCtrl::CheckTCPError()
{
	return (TCPErrID = WSAGetLastError());
}

int CSocketCtrl::GetIpChange()
{
	return m_nGetIpChange;
}

int CSocketCtrl::SetIpChange(BOOL bFlag)
{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[SocketCtrl] SetIpChange[%x]", bFlag);
	m_nGetIpChange = bFlag;
	if (bFlag)
	{
		TCPConnected = FALSE;
	}

	return TRUE;
}

int CSocketCtrl::SetDataRecvTime()
{
	time_t tmCurTime;

	time(&tmCurTime);

	m_dwDataRecvTime = tmCurTime;

	return TRUE;
}

DWORD CSocketCtrl::GetDataRecvTime()
{
	return m_dwDataRecvTime;
}

DWORD CSocketCtrl::RecvTimeCheck(DWORD dwCheckTime)
{
	time_t tmCurTime;
	DWORD dwRtn = NULL;

	time(&tmCurTime);
	if ((DWORD)tmCurTime < (m_dwDataRecvTime + dwCheckTime))
	{
		dwRtn = m_dwDataRecvTime + dwCheckTime - tmCurTime;
	}

	return dwRtn;
}

//UINT CSocketCtrl::ConnectCheck(LPVOID pParam)
UINT ConnectCheck(LPVOID pParam)
{
	CSocketCtrl * pSocket;

	pSocket = (CSocketCtrl *)pParam;
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "CSocketCtrl::ConnectCheck() Thread Start [%x]", pSocket);
	pSocket->SetDataRecvTime();

	while (pSocket->m_fDoingThread)
	{
		if (!pSocket->RecvTimeCheck(180))
		{
			pSocket->SetDataRecvTime();
			if (!pSocket->GetIpChange())
			{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "CSocketCtrl::ConnectCheck() No PollData about 180 seconds");
				pSocket->SetIpChange(TRUE);
			}
		}
		Delay_Msg(200);
	}

MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "CSocketCtrl::ConnectCheck() Thread End [%x]", pSocket);
	return NULL;
}
