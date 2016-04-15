#if !defined(AFX_WINATM_H__CC106090_B05E_473C_AA1A_897971C61B54__INCLUDED_)
#define AFX_WINATM_H__CC106090_B05E_473C_AA1A_897971C61B54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// WinAtm.h : main header file for WINATM.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinAtmApp : See WinAtm.cpp for implementation.

class CWinAtmApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINATM_H__CC106090_B05E_473C_AA1A_897971C61B54__INCLUDED)
