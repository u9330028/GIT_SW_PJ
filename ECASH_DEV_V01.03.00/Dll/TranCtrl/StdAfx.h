// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__EDC8B56A_2020_4C9F_9AB0_D8C698CBB48E__INCLUDED_)
#define AFX_STDAFX_H__EDC8B56A_2020_4C9F_9AB0_D8C698CBB48E__INCLUDED_

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

#include <afxinet.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "..\\..\\Lib\\NetworkCtrl.lib")
#pragma comment(lib, "..\\..\\Lib\\CmnLib.lib")
#pragma comment(lib, "..\\..\\Lib\\TYCrypt.lib")
#pragma comment(lib, "..\\..\\Lib\\VolCtrl.lib")

#ifdef T1ATM_MODE
	#ifdef _DEBUG
		#pragma comment(lib, "..\\..\\Lib\\T1ATM_Debug\\DevCtrl.lib")
		#pragma comment(lib, "..\\..\\Lib\\T1ATM_Debug\\ProCtrl.lib")
		#pragma comment(lib, "..\\..\\Lib\\T1ATM_Debug\\NetCtrl.lib")
	#else	// NDEBUG
		#pragma comment(lib, "..\\..\\Lib\\T1ATM_Release\\DevCtrl.lib")
		#pragma comment(lib, "..\\..\\Lib\\T1ATM_Release\\ProCtrl.lib")
		#pragma comment(lib, "..\\..\\Lib\\T1ATM_Release\\NetCtrl.lib")
	#endif
#else	// T3ATM_MODE
	#ifdef _DEBUG
		#pragma comment(lib, "..\\..\\Lib\\T3ATM_Debug\\DevCtrl.lib")
		#pragma comment(lib, "..\\..\\Lib\\T3ATM_Debug\\ProCtrl.lib")
		#pragma comment(lib, "..\\..\\Lib\\T3ATM_Debug\\NetCtrl.lib")
	#else	// NDEBUG
		#pragma comment(lib, "..\\..\\Lib\\T3ATM_Release\\DevCtrl.lib")
		#pragma comment(lib, "..\\..\\Lib\\T3ATM_Release\\ProCtrl.lib")
		#pragma comment(lib, "..\\..\\Lib\\T3ATM_Release\\NetCtrl.lib")
	#endif
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__EDC8B56A_2020_4C9F_9AB0_D8C698CBB48E__INCLUDED_)
