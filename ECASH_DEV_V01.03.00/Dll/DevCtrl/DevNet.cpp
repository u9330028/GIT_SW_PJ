/////////////////////////////////////////////////////////////////////////////
//	DevNet.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	NETWORK함수(NET) : NET SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnNET_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_ClearErrorCode()");

	int		nReturn = FALSE;

	nReturn = RegSetStr(_REGKEY_DEVERROR, DEVNM_NET, "");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	NETWORK함수(NET) : NET GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrNET_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrNET_GetErrorCode()");

	CString strReturn("");

	strReturn = RegGetStr(_REGKEY_DEVERROR, DEVNM_NET, "0000000");		
	strReturn += "0000000";										// 장애코드보정
	strReturn = strReturn.Left(7);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrNET_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnNET_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_GetDeviceStatus()");

	static	int		nSaveReturn = LINE_CLOSE;
	int				nReturn = LINE_CLOSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// 로칼모드지원
		nReturn = LINE_OPEN;
	else
		nReturn = m_pNetWork->GetStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnN fnNET_Initialize()turn(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	NETWORK함수(NET) : NET OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnNET_Initialize(int nOpenSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Initialize()");

	int		nReturn = FALSE;

	m_pProfile->GetNetworkProfile();
	if (HostLineMode == HOST_LOCAL_MODE)						// 로칼모드지원
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->LineOpen(nOpenSec, &m_pProfile->NETWORK);
	m_pProfile->PutNetworkProfile();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnNET_Deinitialize(int nCloseSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Deinitialize()");

	int		nReturn = FALSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// 로칼모드지원
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->LineClose(nCloseSec);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 자료전송(송신자료,송신시간)
int	CDevCmn::fnNET_SendData(LPCTSTR szSendData, int nSendSec)
{
HexDumpPack(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_SendData(szSendData)", (LPSTR)szSendData, strlen(szSendData));

	int		nReturn = FALSE;
	BYTE	SendData[NETBUFFSIZE];
	int		nSendLength = MakePack((LPSTR)szSendData, SendData, strlen(szSendData));

	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_SendData(...):nSendLength(%d)", nSendLength);
	m_SendRecvStaus = SENDING;
	if (HostLineMode == HOST_LOCAL_MODE)						// 로칼모드지원
		nReturn = TRUE;
	else
		nReturn = m_pNetWork->SendData(SendData, nSendLength, nSendSec);

	m_SendRecvStaus = PIDILE;
	
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_SendData(...):return(%d)", nReturn);
	return nReturn;
}

// 자료수신(수신시간)
int CDevCmn::fnNET_RecvData(CString& strRecvData, int nRecvSec)
{
if (nRecvSec)
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrNET_RecvData(strRecvData, nRecvSec[%d])", nRecvSec);

	int		nReturn = FALSE;
	BYTE	RecvData[NETBUFFSIZE];
	int		nRecvLength = 0;

	m_SendRecvStaus = RECVING;	
	if (HostLineMode == HOST_LOCAL_MODE)						// 로칼모드지원
	{
		nReturn = TRUE;
		strRecvData = "";
	}
	else
	{
		nReturn = m_pNetWork->RecvData(RecvData, &nRecvLength, nRecvSec);
		strRecvData = MakeUnPack(RecvData, nRecvLength);
	}
if (strRecvData.GetLength())
	HexDumpPack(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrNET_RecvData(...):return(strRecvData)", strRecvData.GetBuffer(0), strRecvData.GetLength());

	m_SendRecvStaus = PIDILE;	
	return nReturn;
}

// 연결종료
CString	CDevCmn::fnNET_GetIpAddress(int nFlag)  
{
	CString strReturn("000.000.000.000");
	CString	strReadTmp("");
	CString	strReadBuffer("");
	CString	strReadData("");
	CString	strReadDataSub("");
	CString	strCode("");
	int nSt =0;
	int nSt1 =0;

	strReadBuffer = fnUTL_WinExecWaitEndTrace("ipconfig /all", SW_HIDE, 3000);
	strCode = IniGetStr("C:\\VSYSTEMCD.INI", "SYSTEM", "NAME","WINXP");

	while(TRUE)
	{
		if(m_pDevCmn->OSType == OS_WIN7)		
			nSt = strReadBuffer.Find("IPv4 주소 . . . . . . . . . :", nSt1);
		else
			nSt = strReadBuffer.Find("IP Address. . . . . . . . . . . . :", nSt1);

		if(nSt < 0) break;
		nSt1 = strReadBuffer.Find("\n", nSt);

		if(m_pDevCmn->OSType == OS_WIN7)	
		{
			strReadTmp = strReadBuffer.Mid(nSt+30, nSt1 - (nSt+32));
			strReadTmp.Replace("(기본 설정)", "");
		}
		else
		{
			strReadTmp = strReadBuffer.Mid(nSt+36, nSt1 - (nSt+38));
		}

		if( 0 <= strReadTmp.Find("0.0",0))
		{
			strReadTmp.Empty();
			continue;
		}

		if( 0 <= strReadTmp.Find("10.",0))
		{
			strReadData = strReadTmp;
			strReadTmp.Empty();
			continue;
		}

		if( 0 <= strReadTmp.Find("173.",0))
		{
			strReadTmp.Empty();
			continue;
		}

		if( 0 <= strReadTmp.Find("169.254",0))
		{
			strReadTmp.Empty();
			continue;
		}

		if( 0 <= strReadTmp.Find("::",0))		// IPv6 제외 처리
		{
			strReadTmp.Empty();
			continue;
		}

		strReadDataSub = strReadTmp;
	}

//	strReturn = strReadData +"||"+ strReadDataSub;
	strReturn = strReadDataSub;
	
	return strReturn;
}


// 연결종료
CString	CDevCmn::fnNET_GetMacAddress(int nFlag)  
{
	CString strReturn("00-FF-F9-C1-1C-56");
	CString	strReadTmp("00-FF-F9-C1-1C-56");
	CString	strReadBuffer("");
	CString	strReadData("");
	CString	strReadDataSub("");
	CString	strCode("");
	int nSt =0;
	int nSt1 =0;



	strReadBuffer = fnUTL_WinExecWaitEndTrace("ipconfig /all", SW_HIDE, 3000);

	nSt = strReadBuffer.Find("설명. . . . . . . . . . . . : Qualcomm Atheros AR8151 PCI-E Gigabit Ethernet Controller (NDIS 6.20)", nSt1);
	if(nSt < 0)
	{
		strReadTmp = strReturn;
	}
	else
	{
		nSt1 = strReadBuffer.Find("\n", nSt);
		strReadTmp = strReadBuffer.Mid(nSt1+34, 17);
	}

	strReadTmp.Remove('-');
	strReturn = strReadTmp;
	
	return strReturn;
}

// Center ICMP(Ping) Send : Gi.jin 0619
int	CDevCmn::fnNET_ICMP_DeviceStatus(int nSyncFlag)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnNET_ICMP_DeviceStatus()");

	static	int		nSaveReturn = LINE_CLOSE;
	int				nReturn = LINE_CLOSE;

	if (HostLineMode == HOST_LOCAL_MODE)						// 로칼모드지원
		nReturn = LINE_OPEN;
	else
		nReturn = m_pNetWork->ICMP_Status(nSyncFlag);

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnN fnNET_ICMP_DeviceStatus(%d)", nReturn);
}
	return nReturn;
}


CString CDevCmn::fnNET_GetIpAddressFromWeb()
{
	int nRet = FALSE;
	int nPort = 0;
	int nSSL = 0;
	CString strDomain;
	CString strUrl;
	CString strResult;

	strDomain = IniGetStr(_NETWORK_INI, NET_SEC, "IpCheckServer1", "110.93.180.10");
	nPort = IniGetInt(_NETWORK_INI, NET_SEC, "IpCheckPort1", 8001);
	strUrl = IniGetStr(_NETWORK_INI, NET_SEC, "IpCheckUrl1", "/ipcheck.jsp");
	nSSL = IniGetInt(_NETWORK_INI, NET_SEC, "IpCheckSsl1", 0);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_GetIpAddressFromWeb(%s, %d)", strDomain, nPort);
    CInternetSession internetSession;
    CHttpConnection *httpConnection = internetSession.GetHttpConnection(strDomain, 
                                    (INTERNET_PORT)nPort, NULL ,NULL); 
	nRet = fnNET_RequestUrl(httpConnection, strUrl, "", nSSL, strResult);

	delete httpConnection;
/*
	if (!nRet)
	{
		strDomain = IniGetStr(_NETWORK_INI, NET_SEC, "IpCheckServer2", "110.93.180.10");
		nPort = IniGetInt(_NETWORK_INI, NET_SEC, "IpCheckPort2", 8085);
		strUrl = IniGetStr(_NETWORK_INI, NET_SEC, "IpCheckUrl2", "/ipcheck.jsp");
		nSSL = IniGetInt(_NETWORK_INI, NET_SEC, "IpCheckSsl2", 1);

	    CInternetSession internetSession;
		CHttpConnection *httpConnection = internetSession.GetHttpConnection(strDomain,
										(INTERNET_PORT)nPort, NULL ,NULL); 
		nRet = fnNET_RequestUrl(httpConnection, strUrl, "", nSSL, strResult);

		delete httpConnection;
	}
*/
	if (!nRet)
		strResult = "";

	strResult.Replace("\r\n", "");

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_GetIpAddressFromWeb:return(%d) [%s]", nRet, strResult);
	return strResult;
}


int CDevCmn::fnNET_RequestUrl(CHttpConnection * httpConnection, CString strUrl, CString strPostData, int nSSLFlag, CString &strReturn)
{
	int nRet = FALSE;
    int nRcvsize = 0;
    char revchar[10240];
	DWORD dwFlag = NULL;
    memset(revchar, 0x00, sizeof(revchar));

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_RequestUrl(%s, %s, %s)", strUrl, strPostData, nSSLFlag?"SSL":"");
	
	if (nSSLFlag)
		dwFlag = INTERNET_FLAG_SECURE;// | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;

    CHttpFile * httpFile = httpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, strUrl
														, NULL, 1, NULL, NULL, nSSLFlag);

    //httpFile->AddRequestHeaders("Accept:text/*\r\nUser-Agent: VANCD\r\n");
    httpFile->AddRequestHeaders("Accept: image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-shockwave-flash, */*\r\n" \
								"Referer: http://wcdms.hyosung.com:8001/\r\n" \
								"Accept-Language: ko\r\n" \
								"User-Agent: Mozillzza/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727)\r\n" \
								"Content-Type: application/x-www-form-urlencoded\r\n" \
								"Accept-Encoding: gzip, deflate\r\n");

	try 
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RequestUrl_1");
		if (strPostData.GetLength() > 0)
		{
			httpFile->SendRequestEx(strPostData.GetLength());
			httpFile->WriteString(strPostData);
			httpFile->EndRequest();
		}
		else
			httpFile->SendRequest();
	}
	catch (CInternetException *pEx)
	{	
		TCHAR szTemp[1024];
		pEx->GetErrorMessage(szTemp, 1024);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RequestUrl():return(0) Exception occured! [%s]", szTemp);
		pEx->Delete();

		DWORD dwError = GetLastError();
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetLastError():[%d]", dwError);

		return FALSE;
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RequestUrl_2");
  
	//InternetSetCookie

    //받는 루틴은 
    DWORD dwRet;
    httpFile->QueryInfoStatusCode(dwRet);   
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RequestUrl_3");

	DWORD dwRetLength;
	DWORD dwRecvLength = 0;
	CString strTemp;

	strReturn = "";
    if(dwRet == HTTP_STATUS_OK)
    {
        // data receive routine..
        httpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, dwRetLength);
        //httpFile->QueryInfo(HTTP_QUERY_CONTENT_TYPE, strType);
		TRACE("TOTAL LENGTH : %u\n", dwRetLength);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RequestUrl_4");

		ULONG nWaitTime = TimerSet(5);
		do {
			memset(revchar, 0x00, sizeof(revchar));
			nRcvsize = httpFile->Read(revchar, sizeof(revchar));
			if (!nRcvsize)
			{
				TRACE("BREAK : %d - %d\n", nRcvsize, dwRecvLength);
				break;
			}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RequestUrl_5");

			dwRecvLength += nRcvsize;
			TRACE("RECV LENGTH : %d - %d\n", nRcvsize, dwRecvLength);
			//SendMsg(MAKEATMLIST_MSG_DOWNLOAD, (DWORD)dwRecvLength);

			strTemp.Empty();
			memcpy(strTemp.GetBuffer(nRcvsize), revchar, nRcvsize);
			strTemp.ReleaseBuffer();
			strReturn += strTemp;
		} while(!CheckTimer(nWaitTime));//(dwRecvLength < dwRetLength);

		HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_RequestUrl(): Received", strReturn.GetBuffer(0), strReturn.GetLength());

		if (dwRecvLength >= dwRetLength)
			nRet = TRUE;
		else
			nRet = FALSE;
    }
	else
		nRet = FALSE;

    httpFile->Close();
    delete httpFile;

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnNET_RequestUrl():return(%d) [%d]", nRet, dwRet);
	return nRet;
}


int	CDevCmn::fn_fnNET_Polling()             
{
	int     nSendLength =0;
	int		nSlen = 0;
	int		nBcc = 0;
	BYTE	strDummy[1024];
	char	szTmp[1024];
	BYTE	byTmpBuff[1024];
	BYTE	bySendBuff[1024];

	BYTE	byTLen[4];
	BYTE	byTData[9216];
	BYTE	byBcc[2];

	memset(strDummy,   0x20, sizeof(strDummy));
	memset(&byTLen,   0x20, sizeof(byTLen));
	memset(&byTData,   0x20, sizeof(byTData));
	memset(&byBcc,   0x20, sizeof(byBcc));
	memset(szTmp,      0x00, sizeof(szTmp));
	memset(bySendBuff,      0x00, sizeof(bySendBuff));




	if (m_pNetWork->GetStatus() == LINE_OPEN)
	{
		nSlen = 0;
		memcpy(&strDummy[nSlen], &m_pProfile->NETWORK.BranchNum, 6);
		nSlen += 6;
		memcpy(&strDummy[nSlen], &m_pProfile->NETWORK.AtmSerialNum, 8);
		nSlen += 8;
		memcpy(&strDummy[nSlen], m_pProfile->NETWORK.AtmIPAddress, 15);
		nSlen += 15;


		Int2Asc(nSlen+sizeof(byBcc), byTLen, sizeof(byTLen));
		memcpy(byTData, strDummy, nSlen);
		byTmpBuff[0] = MakeBcc(byTData, nSlen);
		MakeUnPack(byTmpBuff, byBcc, 1);

		memcpy(szTmp,                       byTLen, sizeof(byTLen));
		memcpy(&szTmp[sizeof(byTLen)],      byTData, nSlen);
		memcpy(&szTmp[sizeof(byTLen)+nSlen],byBcc, sizeof(byBcc));
		
		nSendLength = sizeof(byTLen) + nSlen + sizeof(byBcc);

		memcpy(&bySendBuff[0], szTmp, nSendLength);
		int nRet = m_pNetWork->PollSendData((BYTE *)bySendBuff, nSendLength, LINE_BIDSEND_TIME);	
		MsgDump(TRACE_CODE_MODE, "POLL", __FILE__, __LINE__, "fn_fnNET_Polling [%s] - [%d]", bySendBuff, nSendLength);
	}

   return T_OK;
}
