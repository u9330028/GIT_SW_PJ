#if !defined(AFX_TRANCMN_H__94D1FADB_96A6_4E42_A156_4886674FB303__INCLUDED_)
#define AFX_TRANCMN_H__94D1FADB_96A6_4E42_A156_4886674FB303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TranCmn.h : header file
//
/////////////////////////////////////////////////////////////////////////////
#include "..\..\H\Common\Class.h"
#include "..\..\H\Tran\TranCmnEtc.h"
#include "..\..\H\Tran\TranCmnHost.h"
#include "..\..\H\Tran\TranCmnMenu.h"
#include "..\..\H\Tran\TranCmnUser.h"

#include "..\..\H\Dll\EncryptKiup\xmclient.h"
#include "..\..\H\Dll\EncryptKb\kb_xmclient.h"
#include "..\..\H\Dll\EncryptHana\api.h"
#include "..\..\H\Dll\EncryptNon\isaapi.h"
#include "..\..\H\Dll\EncryptNon\isaerr.h"
#include "..\..\H\Dll\EncryptSC\wrap.h"

#define NMSRECVTBL				8
#define NMSBUFFSIZE				32768

/////////////////////////////////////////////////////////////////////////////
// CTranCmn window

class AFX_EXT_CLASS CTranCmn : public CWnd
{
#include "..\..\H\Builder\TranCtrlProc.h"
#include "..\..\H\Builder\TranCtrlProc_STA.h"
#include "..\..\H\Builder\TranFuncProc.h"
#include "..\..\H\Builder\TranFuncProc_STA.h"
#include "..\..\H\Builder\TranRetCode.h"
#include "..\..\H\Builder\TranTranProc.h"

#include "..\..\H\Common\ClassPointer.h"
#include "..\..\H\Tran\TranDevProc.h"
#include "..\..\H\Tran\TranHostProc.h"
#include "..\..\H\Tran\TranInputProc.h"
#include "..\..\H\Tran\TranMainProc.h"
#include "..\..\H\Tran\TranMainSubProc.h"
#include "..\..\H\Tran\TranECashDataFieldProc.h"
#include "..\..\H\Tran\TranMenuProc.h"
#include "..\..\H\Tran\TranNmsProc.h"
#include "..\..\H\Tran\TranOutputProc.h"
#include "..\..\H\Tran\TranTermProc.h"
#include "..\..\H\Tran\TranKCashProc.h"
#include "..\..\H\Tran\TranLibProc.h"

	
/////////////////////////////////////////////////////////////////////////////
// Construction
public:
	CTranCmn();

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
	CNHGuros*	m_pNHGuros;							// NHGuros Pointer
	CHandleOCR*	m_pNHOcr;							// HandleOCR Pointer
	int		m_nCPU;
	int		m_nMEM;

/////////////////////////////////////////////////////////////////////////////
// �Ʒ��� ������ �����ø� �ʱ�ȭ��
/////////////////////////////////////////////////////////////////////////////
struct	tagOpen							// ��������
{
	BYTE	WithCashCount[3];			// 1ȸ�������޸ż�
	BYTE	DepCashCount[3];			// 1ȸ�����Աݸż�
	BYTE	WithCheckCount[3];			// 1ȸ��ǥ���޸ż�
	BYTE	DepCheckCount[3];			// 1ȸ��ǥ�Աݸż�
	BYTE	TransMoney[9];				// 1ȸ��ü�ݾ�
	BYTE	SvcTransMoney[9];			// 1ȸ��ü�ݾ�
	BYTE	IdleMessage[40];			// IDLE�ȳ�����
} OpenInfo;

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	�ʱ�ȭ�Լ�(TranCmn.cpp)
/////////////////////////////////////////////////////////////////////////////
	void	SetOwner(CWinAtmCtrl *pOwner, CNHGuros* pNHGuros, CHandleOCR* pNHOcr);
	int		fnAPP_InitializeDS();					// �����ʱ�ȭ
	int		fnAPP_InitializeSerialTranDS();			// ���Ӱŷ������ʱ�ȭ

/////////////////////////////////////////////////////////////////////////////
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTranCmn)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTranCmn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTranCmn)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
class CTranCmnExit
{
public:
	int		TranExitResult;

public:
	CTranCmnExit(int ExitResult)
	{
		TranExitResult = ExitResult;
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANCMN_H__94D1FADB_96A6_4E42_A156_4886674FB303__INCLUDED_)
