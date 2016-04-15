#if !defined(AFX_WINATMPPG_H__BEBE3B49_F725_4794_8383_525A38D886ED__INCLUDED_)
#define AFX_WINATMPPG_H__BEBE3B49_F725_4794_8383_525A38D886ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// WinAtmPpg.h : Declaration of the CWinAtmPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CWinAtmPropPage : See WinAtmPpg.cpp.cpp for implementation.

class CWinAtmPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CWinAtmPropPage)
	DECLARE_OLECREATE_EX(CWinAtmPropPage)

// Constructor
public:
	CWinAtmPropPage();

// Dialog Data
	//{{AFX_DATA(CWinAtmPropPage)
	enum { IDD = IDD_PROPPAGE_WINATM };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CWinAtmPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINATMPPG_H__BEBE3B49_F725_4794_8383_525A38D886ED__INCLUDED)
