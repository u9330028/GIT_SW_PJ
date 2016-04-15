// NetWork.cpp: implementation of the CNetWork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\MacroDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"
#include <direct.h>
#include < nb30.h>
#include <mbstring.h>
#include "winbase.h"
#include "cpu_info.h"
#include "ResourceMonitor.h"
CResourceMonitor	resMonitor;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// igetinfo = 1 : cpu ����
// igetinfo = 2 : memory ũ��
// igetinfo = 3 : cpu �����
// igetinfo = 4 : memory �����
// igetinfo = 5 : os ����
// igetinfo = 6 : HDD ��ũ ������ "C:\"
// igetinfo = 7 : HDD ��ũ ������ "C:\"
// igetinfo = 8 : FLOPPY ����,CD-ROM ����
// igetinfo = 9 : IE �������� ������
typedef BOOL (WINAPI *P_GDFSE)(LPCTSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);

CString CTranCmn::fnAPP_GetAtmSysInfo(int nSystmInfo)
{

	CString cstrGetSytemInfo("");

	//cpu����	
	if(nSystmInfo == 1) //Ŭ���� ���
	{
		CPUInfo * CPU = new CPUInfo ();
		TCHAR szCPU[60] = {0};
		memcpy(szCPU, CPU->GetExtendedProcessorName (), 
			strlen(CPU->GetExtendedProcessorName ()));
		delete CPU;
		cstrGetSytemInfo.Format("%s",szCPU);
	} 
	else //memory ũ��
	if(nSystmInfo == 2)
	{
		MEMORYSTATUS ms;
		TCHAR szMemSize[20] = {0};
		
		ms.dwLength = sizeof(MEMORYSTATUS);
		::GlobalMemoryStatus(&ms);
		DWORD dwPhysicalMemory;
		
		dwPhysicalMemory=ms.dwTotalPhys;
		wsprintf(szMemSize, "%012d", (int)(dwPhysicalMemory / (1024)));
		cstrGetSytemInfo.Format("%s",szMemSize);

	}
	else //cpu and memory �����
	if(nSystmInfo == 3)
	{
		TCHAR szCpuUsage[16] = {0};
		TCHAR szMemUsage[16] = {0};
		UINT	nCpuUsage = 0;
		UINT	nMemUsage = 0;
		resMonitor.Initialize();			
		resMonitor.GetResourceInfo(nCpuUsage, nMemUsage);	//�� ���� ���� ���� cpu�� 99% �̹Ƿ�
		Sleep(100);
		resMonitor.GetResourceInfo(nCpuUsage, nMemUsage);	//�ι�° ���� ���Ѵ�.
		if(nCpuUsage < 1)
			nCpuUsage = 1;
		wsprintf(szCpuUsage, " %d%%", nCpuUsage);
		wsprintf(szMemUsage, " %d%%", nMemUsage);
		
		cstrGetSytemInfo.Format("CPU%s - MEM%s Rate", szCpuUsage, szMemUsage);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmSysInfo (%s)", cstrGetSytemInfo);

	}
	else
	if(nSystmInfo == 4)
	{

		TCHAR szCpuUsage[16] = {0};
		TCHAR szMemUsage[16] = {0};
		UINT	nCpuUsage = 0;
		UINT	nMemUsage = 0;
		resMonitor.Initialize();			
		resMonitor.GetResourceInfo(nCpuUsage, nMemUsage);	//�� ���� ���� ���� cpu�� 99% �̹Ƿ�
		Sleep(100);
		resMonitor.GetResourceInfo(nCpuUsage, nMemUsage);	//�ι�° ���� ���Ѵ�.
		if(nCpuUsage < 1)
			nCpuUsage = 1;
		wsprintf(szCpuUsage, " %d%%", nCpuUsage);
		wsprintf(szMemUsage, " %d%%", nMemUsage);
		
		cstrGetSytemInfo.Format("CPU%s - MEM%s Rate", szCpuUsage, szMemUsage);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmSysInfo (%s)", cstrGetSytemInfo);

	}
	else //os ����
	if(nSystmInfo == 5)
	{
		TCHAR szOS[64] = {0};
		OSVERSIONINFO osvi;
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		::GetVersionEx(&osvi);
		
		switch(osvi.dwPlatformId){
			
		case VER_PLATFORM_WIN32s:
			strcpy(szOS, "Windows 3.1 ");
			cstrGetSytemInfo.Format("%s",szOS);
			break;

		case VER_PLATFORM_WIN32_WINDOWS:
				if(osvi.dwMinorVersion == 0)
				{
					strcpy(szOS, "Windows 95 ");
					cstrGetSytemInfo.Format("%s",szOS);
				}else if(osvi.dwMinorVersion == 10)
				{
					strcpy(szOS, "Windows 98 ");
					cstrGetSytemInfo.Format("%s",szOS);
				}else if(osvi.dwMinorVersion == 90)
				{
					strcpy(szOS, "Windows me ");
					cstrGetSytemInfo.Format("%s",szOS);
				}else
				{
					strcpy(szOS, "Unknown System ");
					cstrGetSytemInfo.Format("%s",szOS);
				}
			break;

		case VER_PLATFORM_WIN32_NT:
			switch(osvi.dwMajorVersion){
			case 3:
				strcpy(szOS, "Windows NT 3.51 ");
				cstrGetSytemInfo.Format("%s",szOS);
				break;
			case 4:
				strcpy(szOS, "Windows NT 4.0 ");
				cstrGetSytemInfo.Format("%s",szOS);
				break;
			case 5:
				switch(osvi.dwMinorVersion ){
				case 0:
					strcpy(szOS, "Windows 2000 ");
					cstrGetSytemInfo.Format("%s",szOS);
					break;
					
				case 1:
					strcpy(szOS, "Windows xp ");
					cstrGetSytemInfo.Format("%s",szOS);
					break;
					
				default:
					strcpy(szOS, "Unknown System ");
					cstrGetSytemInfo.Format("%s",szOS);
					break;
				}
				break;
			default:
				strcpy(szOS, "Unknown System ");
				cstrGetSytemInfo.Format("%s",szOS);
				break;
			}
			break;
		default:
			strcpy(szOS, "Unknown System ");
			cstrGetSytemInfo.Format("%s",szOS);
			break;
		}
	}
	else
	if(nSystmInfo == 6)
	{
		BOOL bResult;
		//GetDiskFreeSpaceEx�� 98�̻��� ��ũ ������ ������ ���� ���� �Լ�
		P_GDFSE pGetDiskFreeSpaceEx = NULL;
		
		//pGetDiskFreeSpaceEx�� �Ű�����
		unsigned __int64	i64FreeBytesToCaller,	
			i64TotalBytes,
			i64FreeBytes;
		
		//kernel32.dll���� � ���� �ý����ΰ��� Ȯ���Ѵ�.
		//���⼭ True or FALSe�� ��ȯ�Ѵ�.
		pGetDiskFreeSpaceEx = (P_GDFSE)GetProcAddress (GetModuleHandle ("kernel32.dll"), "GetDiskFreeSpaceExA");
		
		TCHAR szHDD[40] = {0};
		TCHAR szHDD1[40] = {0};

		//95�̻��� ���� �ý��� �ϰ��
		if (pGetDiskFreeSpaceEx) {
			//HDD ��ũ ������ "C:\"
			bResult = pGetDiskFreeSpaceEx ("C:\\",
										   (PULARGE_INTEGER)&i64FreeBytesToCaller,
										   (PULARGE_INTEGER)&i64TotalBytes,
										   (PULARGE_INTEGER)&i64FreeBytes);
			if (bResult) {
				
				//HDD ��ũ ������ "C:\"
				wsprintf(szHDD, TEXT("%010d"), i64TotalBytes/(1024));
				wsprintf(szHDD1, TEXT("%010d"), i64FreeBytes/(1024));
				cstrGetSytemInfo.Format("%s%s",szHDD,szHDD1);

			}
	
		}
		
	}
	else
	if(nSystmInfo == 7)
	{
		BOOL bResult;
		//GetDiskFreeSpaceEx�� 98�̻��� ��ũ ������ ������ ���� ���� �Լ�
		P_GDFSE pGetDiskFreeSpaceEx = NULL;
		
		//pGetDiskFreeSpaceEx�� �Ű�����
		unsigned __int64	i64FreeBytesToCaller,	
			i64TotalBytes,
			i64FreeBytes;
		
		//kernel32.dll���� � ���� �ý����ΰ��� Ȯ���Ѵ�.
		//���⼭ True or FALSe�� ��ȯ�Ѵ�.
		pGetDiskFreeSpaceEx = (P_GDFSE)GetProcAddress (GetModuleHandle ("kernel32.dll"), "GetDiskFreeSpaceExA");
		
		TCHAR szHDD[40] = {0};
		TCHAR szHDD1[40] = {0};

		//95�̻��� ���� �ý��� �ϰ��
		if (pGetDiskFreeSpaceEx) {
			//HDD ��ũ ������ "C:\"
			bResult = pGetDiskFreeSpaceEx ("D:\\",
										   (PULARGE_INTEGER)&i64FreeBytesToCaller,
										   (PULARGE_INTEGER)&i64TotalBytes,
										   (PULARGE_INTEGER)&i64FreeBytes);
			if (bResult) {

				//HDD ��ũ ������ "C:\"
				wsprintf(szHDD, TEXT("%010d"), i64TotalBytes/(1024));
				wsprintf(szHDD1, TEXT("%010d"), i64FreeBytes/(1024));
				cstrGetSytemInfo.Format("%s%s",szHDD,szHDD1);
			}
	
		}
		
	}
	else //FLOPPY ����//CD-ROM ����
	if(nSystmInfo == 8)
	{
		char strtmp[3];				
		CString strDrive;
		int realDrive = 0;
		memset(strtmp, 0x30, sizeof(strtmp));
		for (int nDrive = 0 ; nDrive < 26 ; nDrive++) {
			
			if (::GetLogicalDrives() & (1 << nDrive)) {
				
				UINT nType;
				strDrive.Format("%c:\\", nDrive + 'A');
				nType = ::GetDriveType(strDrive);
				
				if(nType == DRIVE_REMOVABLE)
				{
					//�÷��� ��ũ ����
					strtmp[0] = 0x31;

				}else if(nType == DRIVE_CDROM)
				{
					//CD-ROM ����
					strtmp[1] = 0x31;
				}
			}
		}
		strtmp[2] = 0x00;							
		cstrGetSytemInfo.Format("%s",strtmp);


	}
	else//IE �������� ������
	if(nSystmInfo == 9)
	{
		HKEY hTemp = NULL;						
		DWORD dwType; 
		DWORD dwBytes = 16; 
		long lRet = 0;
		unsigned char szIEBuf[16];
		
		memset(szIEBuf, 0x00, sizeof(szIEBuf));		
		TCHAR szSubKey[] = _T("Software\\Microsoft\\Internet Explorer");
		
		RegOpenKeyEx(HKEY_LOCAL_MACHINE, szSubKey, 0, KEY_READ, &hTemp);
		
		if(hTemp != NULL)							
		{
			lRet = RegQueryValueEx(hTemp, _T("Version"), 0, &dwType, szIEBuf, &dwBytes);
			RegCloseKey(hTemp);
		}
		
		if(lRet == ERROR_SUCCESS)
			cstrGetSytemInfo.Format("IE%s",szIEBuf);
		else
			cstrGetSytemInfo.Format("%s","UNKNOWN");
	}	
	return cstrGetSytemInfo;
}



