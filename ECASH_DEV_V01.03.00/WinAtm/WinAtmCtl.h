#if !defined(AFX_WINATMCTL_H__6519D0A0_3B2D_45E7_B955_D90F3E982ECB__INCLUDED_)
#define AFX_WINATMCTL_H__6519D0A0_3B2D_45E7_B955_D90F3E982ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxctl.h>
#include "Class.h"
#include "nhmwi.h"						
#include "NHGuros.h"
#include "HandleOCR.h"
#include "nhalert4spackx.h"
#include "nhiccm.h"	
#include "nhmwiemv.h"
#include <afxtempl.h> 

#define	WUM_SOUND_OFF	(WM_USER) + 9101
#define	WUM_SOUND_ON	(WM_USER) + 9102
#define	WUM_INDICATOR	(WM_USER) + 9103
#define	WUM_PLAY_OFF	(WM_USER) + 9104
#define	WUM_PLAY_ON		(WM_USER) + 9105

#define WM_RD_CTRL_DAILY_REBOOT			(WM_USER) + 9001
#define WM_RD_CTRL_CENTER_REBOOT		(WM_USER) + 9002
#define WM_RD_CTRL_LINE_DOWN			(WM_USER) + 9003
#define WM_RD_CTRL_USER_DOWN			(WM_USER) + 9004

#define	WUM_DUALCF		(WM_USER) + 9200									// T3ATM AP ���泻�� #03 - ����LCD����ó��,����
#define	WUM_DUALCFDISP	(WM_USER) + 9201		// 9105 -> 9201				// T3ATM AP ���泻�� #03 - ����LCD����ó��,����
#define WUM_ADCONTENTS  (WM_USER) + 9202		// 9118 -> 9202				// T3ATM AP ���泻�� #03 - ����LCD����ó��, ����


//#define	WUM_STOP_DISP		(WM_USER) + 9106

// WinAtmCtl.h : Declaration of the CWinAtmCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl : See WinAtmCtl.cpp for implementation.

class CWinAtmCtrl : public COleControl
{
	DECLARE_DYNCREATE(CWinAtmCtrl)

// Constructor
public:
	CWinAtmCtrl();

/////////////////////////////////////////////////////////////////////////////
// Attributes
/////////////////////////////////////////////////////////////////////////////
public:
#include "ClassPointer.h"

	CNHMWI	Mwi;												// Mwi��ü
	CNHICCM Iccm;												// V09-33-00-#01	ICCM ��� ����, ICCM ��ü 
	CNHMwiEmv	MwiEmv;											// V09-33-00-#01	ICCM ��� ����, MWI EMV ��ü 
	CNHAlert4SPackX	NHAlertSpack;								// NHAlertSpack��ü
	CNHGuros	NHGuros;										// NHGuros��ü
	CHandleOCR	NHOcr;											// HandleOCR��ü

/////////////////////////////////////////////////////////////////////////////
// Operations
/////////////////////////////////////////////////////////////////////////////
public:
	void	SetOwner(CWinAtmCtrl *pOwner);						// �����ͼ������ʱ�ȭ

	VARIANT IntArrayToSafeArray(CArray<int, int>& IArray);
	int		AtmModeCtrl();										// �ڵ���������		
	int		TerminateProcess();									// ����ó��
	int		RestartProcess(int nReStartFlag = FALSE);									// �����ó��
	int		ReadyDevTest();										// ��ü�׽�Ʈ�غ�
	int		WindowsShutdownProcess(int nRebootFlag = FALSE);	// ������������ó��

	int		AtmHostCtrl();										// �ڵ���ȣ��Ʈ����

	int		AtmHostLinkCtrl();									// ��� ADSL ��� ����	

	int		TranProc(int TranValue);							// �ŷ�ó��
	int		OpenProc();											// ����ó��
	int		ReOpenUploadProc();									// �� ����ó��

	int		ClerkProc();										// ���ó��
	int		AxisClerkProc();									// U8100-AP���� #12 - ������OM���� ����   


	CRect	rWorkArea;											// OrgWorkArea

	int		SetWorkArea();										// WorkAreaSet
	int		ResetWorkArea();									// Reset WorkArea
	int		Atm_SW_Control();									// V02-12-03-#01 ������ ���ؼ� ����Ȯ��/����ó���� ���
	int		RestoreBrandFile();									// Brand 
	int		RestoreBrandFileCopy();								// Brand 
	int		fnAPL_ProgramReInstMsg();		
	int		WinExecEx(LPCTSTR lpCmdLind, UINT uCmdShow, DWORD dwTimeOut);				
//////////////////////////////////////////////////////////////////////////
// for T2ATM 2006.07.25 : ���Ű ����(Front/Rear ȭ�鱳��)
	CRect	m_rOplRect;											// OPL Screen Rect 
	CRect	m_rSplRect;											// SPL Screen Rect
	void	InitializeDisplayDevice();							// Get Monitor Infomation
	void	SetClerkScreen(BOOL bRearIsFront);					// Change Clerk Screen(OPL<>SPL)
	void	SetClerkScreen();
	void	SetClerkScreen_T3();
//////////////////////////////////////////////////////////////////////////

	void	SetTranInfo(LPCTSTR lpvoid, BOOL bReplace = FALSE);
	void	PrintAtmsRecvInfo(int nMsg, int nSubMsg);
	int		MakeATMSLog(LPCTSTR Logdate);
	int		AtmGetAdapterId();
	int		AtmSetEnableDHCP(int nFlag);


/////////////////////////////////////////////////////////////////////////////
//12/14test	
/////////////////////////////////////////////////////////////////////////////
	int		CheckSecurity();									// 2007.03.12 KHJ ���Ȱ��� ó������
	BYTE	m_szUserEntryPin[6];

/////////////////////////////////////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinAtmCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnSetClientSite();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CWinAtmCtrl();

	DECLARE_OLECREATE_EX(CWinAtmCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CWinAtmCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CWinAtmCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CWinAtmCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CWinAtmCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	//-------------------------------------------------------------------------
	afx_msg void OnDisplayData(LPCTSTR szDataName, LPCTSTR szDataValue);	// AXINAX
	afx_msg void OnDisplayPinpadData(LPCTSTR szData);						// AXINAX

	DECLARE_EVENTSINK_MAP()										// AXINAX
	//-------------------------------------------------------------------------

// Dispatch maps
	//{{AFX_DISPATCH(CWinAtmCtrl)
	afx_msg long AtmStart();
	afx_msg long TraceDump(LPCTSTR szTraceData);
	afx_msg long SetScreenData(LPCTSTR SetScreenDataName, LPCTSTR SetScreenDataValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CWinAtmCtrl)
	void FireOnDisplayData(LPCTSTR szDataName, LPCTSTR szDataValue)
		{FireEvent(eventidOnDisplayData,EVENT_PARAM(VTS_BSTR  VTS_BSTR), szDataName, szDataValue);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CWinAtmCtrl)
	dispidAtmStart = 1L,
	dispidTraceDump = 2L,
	dispidSetScreenData = 3L,
	eventidOnDisplayData = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINATMCTL_H__6519D0A0_3B2D_45E7_B955_D90F3E982ECB__INCLUDED)
