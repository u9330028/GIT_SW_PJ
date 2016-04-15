/////////////////////////////////////////////////////////////////////////////
// WinAtmPpg.cpp : Implementation of the CWinAtmPropPage property page class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "WinAtm.h"
#include "WinAtmPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CWinAtmPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CWinAtmPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CWinAtmPropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CWinAtmPropPage, "WINATM.WinAtmPropPage.1",
	0xd88358f8, 0x8ff1, 0x4c62, 0x84, 0x77, 0xa3, 0x93, 0x7c, 0x52, 0x7e, 0x29)


/////////////////////////////////////////////////////////////////////////////
// CWinAtmPropPage::CWinAtmPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CWinAtmPropPage

BOOL CWinAtmPropPage::CWinAtmPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_WINATM_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmPropPage::CWinAtmPropPage - Constructor

CWinAtmPropPage::CWinAtmPropPage() :
	COlePropertyPage(IDD, IDS_WINATM_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CWinAtmPropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmPropPage::DoDataExchange - Moves data between page and properties

void CWinAtmPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CWinAtmPropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmPropPage message handlers
