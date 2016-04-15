// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E1D8C316_3341_463B_8CEF_4B4C2A4FCEDB__INCLUDED_)
#define AFX_STDAFX_H__E1D8C316_3341_463B_8CEF_4B4C2A4FCEDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>
#include <afxinet.h>

#pragma comment(lib, "shlwapi.lib")
//#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "..\\..\\Lib\\CmnLib.lib")
#pragma comment(lib, "..\\..\\Lib\\mcis1.lib")
#pragma comment(lib, "..\\..\\Lib\\NetworkCtrl.lib")

#pragma comment(lib, "..\\..\\Lib\\EmbossDll.lib")
#pragma comment(lib, "..\\..\\Lib\\Init_Dll.lib")
#pragma comment(lib, "..\\..\\Lib\\CaptureDevice.lib")
#pragma comment(lib, "..\\..\\Lib\\VolCtrl.lib")
#pragma comment(lib, "..\\..\\Lib\\PsApi.lib")
 
//#pragma comment(lib, "..\\..\\Lib\\PowerTTS.lib") 
 
#ifdef T1ATM_MODE
		#pragma comment(lib, "..\\..\\Lib\\T1ATM_Release\\NetCtrl.lib")
		#pragma comment(lib, "..\\..\\Lib\\NHRDCTRL.lib")
#else	// T3ATM_MODE
		#pragma comment(lib, "..\\..\\Lib\\T3ATM_Release\\NetCtrl.lib")
		#pragma comment(lib, "..\\..\\Lib\\NHRDCTRL.lib")
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E1D8C316_3341_463B_8CEF_4B4C2A4FCEDB__INCLUDED_)
