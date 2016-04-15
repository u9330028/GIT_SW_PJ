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


// igetinfo = 1 : cpu 정보
// igetinfo = 2 : memory 크기
// igetinfo = 3 : cpu 사용율
// igetinfo = 4 : memory 사용율
// igetinfo = 5 : os 정보
// igetinfo = 6 : HDD 디스크 사이즈 "C:\"
// igetinfo = 7 : HDD 디스크 사이즈 "C:\"
// igetinfo = 8 : FLOPPY 정보,CD-ROM 정보
// igetinfo = 9 : IE 버전정보 얻어오기
typedef BOOL (WINAPI *P_GDFSE)(LPCTSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);

CString CTranCmn::fnAPP_GetAtmSysInfo(int nSystmInfo)
{

	CString cstrGetSytemInfo("");

	//cpu정보	
	if(nSystmInfo == 1) //클럭수 얻기
	{
		CPUInfo * CPU = new CPUInfo ();
		TCHAR szCPU[60] = {0};
		memcpy(szCPU, CPU->GetExtendedProcessorName (), 
			strlen(CPU->GetExtendedProcessorName ()));
		delete CPU;
		cstrGetSytemInfo.Format("%s",szCPU);
	} 
	else //memory 크기
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
	else //cpu and memory 사용율
	if(nSystmInfo == 3)
	{
		TCHAR szCpuUsage[16] = {0};
		TCHAR szMemUsage[16] = {0};
		UINT	nCpuUsage = 0;
		UINT	nMemUsage = 0;
		resMonitor.Initialize();			
		resMonitor.GetResourceInfo(nCpuUsage, nMemUsage);	//한 번만 했을 경우는 cpu가 99% 이므로
		Sleep(100);
		resMonitor.GetResourceInfo(nCpuUsage, nMemUsage);	//두번째 값을 취한다.
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
		resMonitor.GetResourceInfo(nCpuUsage, nMemUsage);	//한 번만 했을 경우는 cpu가 99% 이므로
		Sleep(100);
		resMonitor.GetResourceInfo(nCpuUsage, nMemUsage);	//두번째 값을 취한다.
		if(nCpuUsage < 1)
			nCpuUsage = 1;
		wsprintf(szCpuUsage, " %d%%", nCpuUsage);
		wsprintf(szMemUsage, " %d%%", nMemUsage);
		
		cstrGetSytemInfo.Format("CPU%s - MEM%s Rate", szCpuUsage, szMemUsage);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmSysInfo (%s)", cstrGetSytemInfo);

	}
	else //os 정보
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
		//GetDiskFreeSpaceEx는 98이상의 디스크 정보를 가지고 오기 위한 함수
		P_GDFSE pGetDiskFreeSpaceEx = NULL;
		
		//pGetDiskFreeSpaceEx의 매개변수
		unsigned __int64	i64FreeBytesToCaller,	
			i64TotalBytes,
			i64FreeBytes;
		
		//kernel32.dll에서 어떤 파일 시스템인가를 확인한다.
		//여기서 True or FALSe를 반환한다.
		pGetDiskFreeSpaceEx = (P_GDFSE)GetProcAddress (GetModuleHandle ("kernel32.dll"), "GetDiskFreeSpaceExA");
		
		TCHAR szHDD[40] = {0};
		TCHAR szHDD1[40] = {0};

		//95이상의 파일 시스템 일경우
		if (pGetDiskFreeSpaceEx) {
			//HDD 디스크 사이즈 "C:\"
			bResult = pGetDiskFreeSpaceEx ("C:\\",
										   (PULARGE_INTEGER)&i64FreeBytesToCaller,
										   (PULARGE_INTEGER)&i64TotalBytes,
										   (PULARGE_INTEGER)&i64FreeBytes);
			if (bResult) {
				
				//HDD 디스크 사이즈 "C:\"
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
		//GetDiskFreeSpaceEx는 98이상의 디스크 정보를 가지고 오기 위한 함수
		P_GDFSE pGetDiskFreeSpaceEx = NULL;
		
		//pGetDiskFreeSpaceEx의 매개변수
		unsigned __int64	i64FreeBytesToCaller,	
			i64TotalBytes,
			i64FreeBytes;
		
		//kernel32.dll에서 어떤 파일 시스템인가를 확인한다.
		//여기서 True or FALSe를 반환한다.
		pGetDiskFreeSpaceEx = (P_GDFSE)GetProcAddress (GetModuleHandle ("kernel32.dll"), "GetDiskFreeSpaceExA");
		
		TCHAR szHDD[40] = {0};
		TCHAR szHDD1[40] = {0};

		//95이상의 파일 시스템 일경우
		if (pGetDiskFreeSpaceEx) {
			//HDD 디스크 사이즈 "C:\"
			bResult = pGetDiskFreeSpaceEx ("D:\\",
										   (PULARGE_INTEGER)&i64FreeBytesToCaller,
										   (PULARGE_INTEGER)&i64TotalBytes,
										   (PULARGE_INTEGER)&i64FreeBytes);
			if (bResult) {

				//HDD 디스크 사이즈 "C:\"
				wsprintf(szHDD, TEXT("%010d"), i64TotalBytes/(1024));
				wsprintf(szHDD1, TEXT("%010d"), i64FreeBytes/(1024));
				cstrGetSytemInfo.Format("%s%s",szHDD,szHDD1);
			}
	
		}
		
	}
	else //FLOPPY 정보//CD-ROM 정보
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
					//플로피 디스크 있음
					strtmp[0] = 0x31;

				}else if(nType == DRIVE_CDROM)
				{
					//CD-ROM 있음
					strtmp[1] = 0x31;
				}
			}
		}
		strtmp[2] = 0x00;							
		cstrGetSytemInfo.Format("%s",strtmp);


	}
	else//IE 버전정보 얻어오기
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



