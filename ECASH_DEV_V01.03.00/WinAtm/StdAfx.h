#if !defined(AFX_STDAFX_H__540B3489_44F0_42E3_8157_FF09EC5C9F7C__INCLUDED_)
#define AFX_STDAFX_H__540B3489_44F0_42E3_8157_FF09EC5C9F7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Comon Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Delete the two includes below if you do not wish to use the MFC
//  database classes
#include <afxdb.h>			// MFC database classes
#include <afxdao.h>			// MFC DAO database classes

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "..\\Lib\\CmnLib.lib")

#ifdef T1ATM_MODE
	#ifdef _DEBUG
		#pragma comment(lib, "..\\Lib\\T1ATM_Debug\\NetCtrl.lib")
	#else	// NDEBUG
		#pragma comment(lib, "..\\Lib\\T1ATM_Release\\DevCtrl.lib")
		#pragma comment(lib, "..\\Lib\\T1ATM_Release\\NetCtrl.lib")
		#pragma comment(lib, "..\\Lib\\T1ATM_Release\\TranCtrl.lib")
	#endif
#else	// T3ATM_MODE
	#ifdef _DEBUG
		#pragma comment(lib, "..\\Lib\\T3ATM_Debug\\NetCtrl.lib")
	#else	// NDEBUG
		#pragma comment(lib, "..\\Lib\\T3ATM_Release\\DevCtrl.lib")
		#pragma comment(lib, "..\\Lib\\T3ATM_Release\\NetCtrl.lib")
		#pragma comment(lib, "..\\Lib\\T3ATM_Release\\TranCtrl.lib")
	#endif
#endif

#include <afxtempl.h>
#include <afxinet.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__540B3489_44F0_42E3_8157_FF09EC5C9F7C__INCLUDED_)
