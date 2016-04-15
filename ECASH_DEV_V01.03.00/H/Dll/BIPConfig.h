//////////////////////////////////////////////////////////////////////
// BIPConfig.h: interface for the CBIPConfig class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BIPCONFIG_H__94B36919_9802_46D7_B2E5_C6D11ACEEFB6__INCLUDED_)
#define AFX_BIPCONFIG_H__94B36919_9802_46D7_B2E5_C6D11ACEEFB6__INCLUDED_

#include <windows.h>
#include <stdio.h>
#include "iptypes.h"	//This file request installing Windows SDK.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBIPConfig  
{
public:
	int DeleteAdapterGateway(const int nGatewayIndex = 0, const int nAdapterIndex = 0);
	int DeleteAdapterIpAddress(const int nIPIndex = 0, const int nAdapterIndex = 0);
	int DeleteAdapterDNSServer(const int nAdapterIndex = 0);
	int SetAdapterIP();
	int AtmSetIpAddress(char* szIPAddress, int nAdapterIndex = 0);
	int AtmSetSubnetMask(char* szSubnetMask, int nAdapterIndex = 0);
	int AtmSetGateWay(char* szGateway, int nAdapterIndex = 0);
	int AtmSetDHCPEnable(int nAdapterIndex = 0);
	int AtmSetIpAll(char *szIPAddress, char *szSubnetMask, char *szGateway, int nAdapterIndex = NULL);
	int AtmSetDNSDHCPEnable(int nAdapterIndex = 0);
	int AtmSetDNS(char *szDNSServer, char *szSubDNSServer = 0, int nAdapterIndex = 0);
	int AtmGetIpAddress(char* szIpAddress, int nAdapterIndex = 0);
	int AtmGetSubnetMask(char* szSubnetMask, int nAdapterIndex = 0);
	int AtmGetGateWay(char* szGateWay, int nAdapterIndex = 0);
	int AtmGetDNSServer(LPCTSTR szDNSServer, int nAdapterIndex = 0);
	LPCTSTR GetSubDNSServer(const int nAdapterIndex);
	LPCTSTR GetBasicDNSServer(const int nAdapterIndex);
	LPCSTR IPConvert(DWORD dwIPAddress);
	DWORD IPConvert(LPCSTR szIPAddress);
	int SetNetworkValue(const int nAdapterIndex = 0, LPCTSTR szIPAddress = 0, LPCTSTR szSubnetMask = 0,
		 LPCTSTR szGateway = 0, LPCTSTR szBasicDNSServer = 0, LPCTSTR szSubDNSServer = 0);
	struct tm * GetAdapterLeaseExpiresTime(const int nAdapterIndex = 0);
	struct tm * GetAdapterLeaseObtainedTime(const int nAdapterIndex = 0);
	LPCTSTR GetAdapterSecondaryWinsServer(const int nAdapterIndex = 0);
	LPCTSTR GetAdapterPrimaryWinsServer(const int nAdapterIndex = 0);
	LPCTSTR GetAdapterDHCPServer(const int nAdapterIndex = 0);
	LPCTSTR GetAdapterGateway(const int nGatewayIndex = 0, const int nAdapterIndex = 0);
	PIP_ADDR_STRING GetAdapterGatewayList(const int nGatewayIndex = 0, const int nAdapterIndex = 0);
	int GetAdapterGatewayCount(const int nAdapterIndex = 0);
	LPCTSTR GetAdapterSubnetMask(const int nIPIndex = 0, const int nAdapterIndex = 0);
	LPCTSTR GetAdapterIPAddress(const int nIPIndex = 0, const int nAdapterIndex = 0);
	PIP_ADDR_STRING GetAdapterIPAddressList(const int nIPIndex = 0, const int nAdapterIndex = 0);
	int GetAdapterIPCount(const int nAdapterIndex = 0);
	BOOL IsEnableAdapterDHCP(const int nAdapterIndex = 0);
	void GetAdapterAddress(LPTSTR lpAddress, const int nAdapterIndex = 0);
	LPCTSTR GetAdapterDescription(const int nAdapterIndex = 0);
	LPCTSTR GetAdapterName(const int nAdapterIndex = 0);
	LPCTSTR GetAdapterType(const int nAdapterIndex = 0);
	PIP_ADAPTER_INFO GetAdapterInfo(const int nAdapterIndex = 0);
	int GetAdapterCount() const;
	BOOL IsEnableDns() const;
	BOOL IsEnableProxy() const;
	BOOL IsEnableRouting() const;
	LPCTSTR GetScopeId() const;
	LPCTSTR GetNodeType() const;
	LPCTSTR GetDnsServer(const int nAdapterIndex = 0) const;
	int GetDnsServerCount() const;
	LPCTSTR GetDomainName() const;
	LPCTSTR GetHostName() const;
	LPCTSTR GetNetworkName(const int nAdapterIndex);
	int GetNetworkName(LPCSTR szOutBuff, int nMaxSize, const int nAdapterIndex);
	LPCTSTR GetMacAddress(const int nAdapterIndex);
	int GetMacAddress(LPCSTR szOutBuff, int nMaxSize, const int nAdapterIndex);
	CBIPConfig();
	virtual ~CBIPConfig();

private:
	int SetAdapterGateway(LPCTSTR szIPAddress, const int nAdapterIndex);
	int SetAdapterSubnetMask(LPCTSTR szIPAddress, const int nAdapterIndex);
	int SetAdapterDHCP(const int nAdapterIndex, int DNSFlag = NULL);
	int SetDNSServer(const int nAdapterIndex, LPCTSTR szDNSServer, LPCSTR szSubDnsServer = NULL);
	int SetAdapterIPAddress(LPCTSTR szIPAddress, const int nAdapterIndex);
	LONG RegSetValueExt(LPCTSTR hKeyName, LPSTR lpszValueName, DWORD dwType, DWORD dwLen, LPVOID lpszData);
	int SetTcpipInfo(LPCTSTR szName, LPCTSTR szIPAddress, const int nAdapterIndex);
	LONG RegQueryValueExt(LPCTSTR hKeyName, LPSTR lpszValueName, DWORD dwType, DWORD dwLen, LPVOID lpszData);
	int GetOSVersion(char *szOSDetailName = NULL);
	int AddDNSServer(const int nAdapterIndex, LPCTSTR szDNSServer);
	int DeleteDNSServer(const int nAdapterIndex);
	TCHAR m_szErrMsg[MAX_PATH];
	//DWORD m_dwAdapterInfoSize;
	//PIP_ADAPTER_INFO m_pAdapt;
	PIP_ADAPTER_INFO m_pAdapterInfo;
	//PIP_ADDR_STRING m_pAddrStr;
	//DWORD m_dwFixedInfoSize;
	PFIXED_INFO m_pFixedInfo;
	LPCTSTR GetTcpipInfo(LPCTSTR szName, int nAdapterIndex = 0);

};

#endif // !defined(AFX_BIPCONFIG_H__94B36919_9802_46D7_B2E5_C6D11ACEEFB6__INCLUDED_)
