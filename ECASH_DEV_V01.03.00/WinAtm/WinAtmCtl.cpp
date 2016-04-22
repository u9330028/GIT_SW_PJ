/////////////////////////////////////////////////////////////////////////////
// WinAtmCtl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include "shlwapi.h"
#include "..\H\Common\CmnLibIn.h"
#include "..\H\Common\Define.h"
#include "..\H\Common\ConstDef.h"
#include "..\H\Common\ClassInclude.h"
#include "..\H\Dll\DevCmn.h"
#include "..\H\Tran\TranCmn.h"

#include "WinAtm.h"
#include "WinAtmCtl.h"
#include "WinAtmPpg.h"

#define COMPILE_MULTIMON_STUBS									// multimon.h Define
#include "MultiMon.h"											// multi-monitor

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// 출력 디바이스 초기화 정보 Define
// Multi-Monitor관련하여 변수/구조체등을 설정한다.
// by teo Using MSDN
// ----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
#define		MAX_MONITOR		10		// 최대 모니터 수

// 모니터 정보저장을 위한 구조체 선언
struct		structMonitorInfo
{
	HMONITOR	hMonitor;			// 모니터의 핸들
	char		szDevice[100];		// 모니터의 디바이스명
	RECT		rcMonitor;			// 모니터의 영역
	RECT		rcWork;				// 모니터의 작업영역
	DWORD		dwFlag;				// 모니터 설정(기본모니터 여부)
};

// 시스템에 10까지 모니터를 설치가능하다고 설정하자. 그럴리는 없지만.
structMonitorInfo MonitorInfo[MAX_MONITOR];

static int MaxMonitorIndex = 0;		// 기기에 설치된 모니터의 총 인덱스
static int CurMonitorIndex = 0;		// 1)모니터정보조회시 인덱스, 2)기본모니터가 아닌 모니터의 첫인덱스
static int PriMonitorIndex = 0;		// 기본모니터의 인덱스


IMPLEMENT_DYNCREATE(CWinAtmCtrl, COleControl)

/////////////////////////////////////////////////////////////////////////////
// AXEvent map : AXINAX

BEGIN_EVENTSINK_MAP(CWinAtmCtrl, COleControl)
    //{{AFX_EVENTSINK_MAP(CWinAtmCtrl)
	ON_EVENT(CWinAtmCtrl, IDC_NHMWI, 3 /* OnDisplayData			*/, OnDisplayData, VTS_BSTR VTS_BSTR)
	ON_EVENT(CWinAtmCtrl, IDC_NHMWI, 4 /* OnDisplayPinpadData	*/, OnDisplayPinpadData, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CWinAtmCtrl, COleControl)
	//{{AFX_MSG_MAP(CWinAtmCtrl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CWinAtmCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CWinAtmCtrl)
	DISP_FUNCTION(CWinAtmCtrl, "AtmStart", AtmStart, VT_I4, VTS_NONE)
	DISP_FUNCTION(CWinAtmCtrl, "TraceDump", TraceDump, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CWinAtmCtrl, "SetScreenData", SetScreenData, VT_I4, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CWinAtmCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CWinAtmCtrl, COleControl)
	//{{AFX_EVENT_MAP(CWinAtmCtrl)
	EVENT_CUSTOM("OnDisplayData", FireOnDisplayData, VTS_BSTR  VTS_BSTR)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

BEGIN_PROPPAGEIDS(CWinAtmCtrl, 1)
	PROPPAGEID(CWinAtmPropPage::guid)
END_PROPPAGEIDS(CWinAtmCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CWinAtmCtrl, "WINATM.WinAtmCtrl.1",
	0xbba80ecc, 0xeef0, 0x4e7c, 0x8b, 0x5e, 0xfd, 0xee, 0xba, 0x56, 0x8e, 0x1d)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CWinAtmCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DWinAtm =
		{ 0xef0057b, 0x458a, 0x4d2f, { 0xa5, 0x6c, 0x51, 0x67, 0xee, 0x33, 0x90, 0x16 } };
const IID BASED_CODE IID_DWinAtmEvents =
		{ 0xccc22c52, 0x2027, 0x460d, { 0xb0, 0x79, 0x39, 0x32, 0x94, 0xc0, 0x48, 0x3a } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwWinAtmOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CWinAtmCtrl, IDS_WINATM, _dwWinAtmOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::CWinAtmCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CWinAtmCtrl
// 웹페이지에서 사용시 보안문제 해결용
#include "Helpers.h"
#include "ObjSafe.h"

BOOL CWinAtmCtrl::CWinAtmCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.
	
	if (bRegister)
	{
		int retval = AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_WINATM,
			IDB_WINATM,
			afxRegApartmentThreading,
			_dwWinAtmOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);

		
		HRESULT hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls that are safely scriptable");
		
		if (SUCCEEDED(hr))
			RegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting);

		
		hr = CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely initializable from persistent data");

		if (SUCCEEDED(hr))
			RegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing);

		return retval;
	}
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::CWinAtmCtrl - Constructor

CWinAtmCtrl::CWinAtmCtrl()
{
	InitializeIIDs(&IID_DWinAtm, &IID_DWinAtmEvents);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CWinAtmCtrl()");

	m_pOwner	= NULL;

	m_pDevCmn	= NULL;
	m_pTranCmn	= NULL;

	m_pProfile	= NULL;
	m_pNetWork	= NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::~CWinAtmCtrl - Destructor

CWinAtmCtrl::~CWinAtmCtrl()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~CWinAtmCtrl()");
	exit(0);
}


/////////////////////////////////////////////////////////////////////////////
// 포인터설정및초기화
void CWinAtmCtrl::SetOwner(CWinAtmCtrl *pOwner)
{
#include "..\H\Common\ClassSetOwner.h"

	return;
}

/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::OnDraw - Drawing function

void CWinAtmCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::DoPropExchange - Persistence support

void CWinAtmCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::OnResetState - Reset control to default state

void CWinAtmCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::AboutBox - Display an "About" box to the user

void CWinAtmCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_WINATM);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl message handlers

int CWinAtmCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	long iccmResult;					// V09-33-00-#01	ICCM 모듈 적용
	long MwiEmvResult;					// V09-33-00-#01	ICCM 모듈 적용

	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OnCreate()");

	// 장치제어를 위해서 Middleware Interface Object를 생성한다.
	Mwi.Create(NULL, 0, CRect(0,0,0,0), this, IDC_NHMWI);
	{
		CArray<int, int> arIDs;	
		arIDs.RemoveAll();
		for(int i = 0; i < DEV_COUNT; i++)
			arIDs.Add(S_DEVICE_IDS[i]);
		Mwi.GenerateDeviceID(IntArrayToSafeArray(arIDs));
	}

	iccmResult = Iccm.Create(NULL, 0, CRect(0,0,0,0), this, IDC_NHICCM);		// V09-33-00-#01	ICCM 모듈 적용
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OnCreate():ICCM return[%x]",iccmResult);

	MwiEmvResult = MwiEmv.Create(NULL, 0, CRect(0,0,0,0), this, IDC_NHMWIEMV);	// V09-33-00-#01	ICCM 모듈 적용
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OnCreate():MwiEmvResult  return[%x]",MwiEmvResult );

	NHGuros.Create(NULL, 0, CRect(0,0,0,0), this, IDC_NHGUROS);
	NHOcr.Create(NULL, 0, CRect(0,0,0,0), this, IDC_NHOCR);
	NHAlertSpack.Create(NULL, 0, CRect(0,0,0,0), this, IDC_NHSPACK);

//	InitializeDisplayDevice();									// for T2ATM 2006.07.25 : Get Monitor Information


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OnCreate():return");
	return 0;
}


VARIANT CWinAtmCtrl::IntArrayToSafeArray(CArray<int, int>& IArray) 
{
	static SAFEARRAY *m_pSafeArray = NULL; // 2004.05.10
	
	VARIANT Result;
	VariantInit(&Result);
	
	VARIANT *ElemVar;
	long    ArrayIndex[1]; // This is a 1D array, so it's index is a 1 vertex array
	
	SAFEARRAYBOUND Bounds[1];	
	int i;	
	HRESULT hRc ;
	
	// Setup the array boundrys
	Bounds[0].lLbound = 0;
	Bounds[0].cElements = IArray.GetSize();
	
	// If m_pSafeArray is not NULL, initialize this array 
	if (m_pSafeArray) // 2004.05.10
	{
		SafeArrayDestroy(m_pSafeArray);	
		m_pSafeArray = NULL;
	}
	
	m_pSafeArray = SafeArrayCreate( VT_VARIANT, 1, Bounds);
	if ( m_pSafeArray == NULL) 
		return Result;
	
	for( i = 0; i< IArray.GetSize(); i++ )
	{
		// Create a new Variant to put his element into
		ElemVar = new VARIANT;
		if( ElemVar == NULL) 
			return Result;
		
		// Init variant
		VariantInit( ElemVar );
		
		// Set type and data
		ElemVar->vt = VT_I4;
		ElemVar->lVal = (long)(IArray.GetAt(i));
		
		// put elemtent into array
		ArrayIndex[0] = i;
		hRc = SafeArrayPutElement( m_pSafeArray, ArrayIndex, ElemVar ); 
		if( FAILED( hRc ) ) // 2004.05.10
			return Result;
		
		// The element has now been copied, so we can delete it.
		VariantClear( ElemVar );
		delete ElemVar;
	}
	
	// Setup the array variant
	Result.vt = VT_ARRAY | VT_VARIANT;
	Result.parray = m_pSafeArray;	
	return Result;
}


/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : void OnDisplayData(LPCTSTR szDataName, LPCTSTR szDataValue)
// DESCRIPT  : 화면출력을 이벤트로 요청한다.(To Flash)
// PARAMETER : 데이타이름, 데이타
// RETURN    : 
// REMARK    : 이 함수는 이벤트 핸들러이다.
//             MWI로 부터 화면출력을 요구하는 이벤트를 받아서 처리한다.
/////////////////////////////////////////////////////////////////////////////
void CWinAtmCtrl::OnDisplayData(LPCTSTR szDataName, LPCTSTR szDataValue)
{
	// 조건 : 장애우 atm이고, Ear Phone이 Inserted된 경우에만 Set Focus 적용
	if ((m_pDevCmn->EarPhoneInserted == TRUE)		&&					// #N0193 장애인ATM
		(memcmp(szDataName, SCR_SCREEN, sizeof(SCR_SCREEN)-1) == 0))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OnDisplayData %s  -  %s", szDataName, szDataValue);
		SetCursorPos(1040, 10);								// 정통부, 신협(해상도1024일때)
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 
			0,
			0,
			0, 
			0);
	}
	
	FireOnDisplayData(szDataName, szDataValue);
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : void OnDisplayPinpadData(LPCTSTR szData)
// DESCRIPT  : PinPad Data를 Emulation한다.(To Flash)
// PARAMETER : PinPad Data
// RETURN    : 
// REMARK    : 이 함수는 이벤트 핸들러이다.
//             MWI로 부터 받은 값으로 Mouse/Keybd Event를 발생시킨다
/////////////////////////////////////////////////////////////////////////////
void CWinAtmCtrl::OnDisplayPinpadData(LPCTSTR szData) 
{

	CString	strDataValue(szData);

	BYTE	VirtualKeyCode = 0;

	CPoint	MousePoint(0,0);	

if ((strDataValue >= "0")	&&									// 0-9
	(strDataValue <= "9")	&&
	(strDataValue.GetLength() == 1))							// 2004.08.19
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OnDisplayPinpadData(szDataValue[%s])", "*");
else
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OnDisplayPinpadData(szDataValue[%s])", szData);
}

	switch (m_pProfile->DEVICE.MachineType)
	{
//		case U8500:
		case U8100:
			if(m_pDevCmn->OSType == OS_WIN7)					//#N0228 WIN7지원
			{
				SetCursorPos(m_rOplRect.left + 10, 10);
				mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 
						0,							// Focus x(Mouse)
						0,								// Focus y(Mouse)
						0, 
						0);	
			}
			break;

		case U3100K:
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 
						65535 / 10 * 9,							// Focus x(Mouse)
						65535 / 10,								// Focus y(Mouse)
						NULL, 
						NULL);
			break;

	}

	if ((strDataValue >= "0")	&&								// 0-9
		(strDataValue <= "9")	&&
		(strDataValue.GetLength() == 1))
	{
		memcpy(&VirtualKeyCode, strDataValue.GetBuffer(0), 1);
	}
	else
	if (strDataValue == ".")									// .
		VirtualKeyCode = 'Z';
	else
	if (strDataValue == "00")									// 00
		VirtualKeyCode = 'X';
	else
	if (strDataValue == "CANCEL")								// Cancel
		VirtualKeyCode = VK_ESCAPE;
	else
	if (strDataValue == "CLEAR")								// Clear
		VirtualKeyCode = VK_DELETE;
	else
	if (strDataValue == "ENTER")								// Enter
		VirtualKeyCode = VK_RETURN;								// TTW TEST
	else
	if (strDataValue == "XXX")									// BackSpace : 2004.06.30
		VirtualKeyCode = VK_BACK;
	else
	if (strDataValue == "000")									// "C" : 원 - 변경가능
		VirtualKeyCode = 'C';
	else
	if (strDataValue == "F1")									// F1
		VirtualKeyCode = 'A';
	else
	if (strDataValue == "F2")									// F2
		VirtualKeyCode = 'E';
	else
	if (strDataValue == "F3")									// F3
		VirtualKeyCode = 'B';
	else
	if (strDataValue == "F4")									// F4
		VirtualKeyCode = 'F';
	else
	if (strDataValue == "F5")									// F5
		VirtualKeyCode = 'C';
	else
	if (strDataValue == "F6")									// F6
		VirtualKeyCode = 'G';
	else
	if (strDataValue == "F7")									// F7
		VirtualKeyCode = 'D';
	else
	if (strDataValue == "F8")									// F8
		VirtualKeyCode = 'H';
	else
	if (strDataValue == "F9")									// F9 : 만
		VirtualKeyCode = 'I';
	else
	if (strDataValue == "F10")									// F10:천
		VirtualKeyCode = 'J';
	else
	if (strDataValue == "F12")									// F12:원
		VirtualKeyCode = 'L';
	else return;


	keybd_event(VirtualKeyCode, 0, KEYEVENTF_KEYUP, 0);			
	return;
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : long AtmStart()
// DESCRIPT  : 자동화기기를 시작한다.
// PARAMETER : 없음
// RETURN    : NORMAL(0)
// REMARK    : 
/////////////////////////////////////////////////////////////////////////////
long CWinAtmCtrl::AtmStart() 
{
	// TODO: Add your dispatch handler code here
	

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStart()");

	Atm_SW_Control();
	//m_pDevCmn = new CDevCmn();									// 장치생성
	//m_pTranCmn = new CTranCmn();								// 업무생성
	static CDevCmn tmpDevCmn;
	m_pDevCmn = &tmpDevCmn;										// 장치생성	
	static CTranCmn tmpTranCmn;
	m_pTranCmn = &tmpTranCmn;										// 업무생성
	// 

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStart():WorkArea설정처리");
	SetWorkArea();												// 2004.06.28 Kalignite logo숨김

	m_pDevCmn->SetOwner(this, &Mwi, &NHAlertSpack, &Iccm, &MwiEmv);				// 장치포인터설정및초기화
	if (!m_pDevCmn->CheckMachineType)							// V05-06-03-#13:기종과 CashHandler불일치시 안내화면
	{
		ResetWorkArea();										// Area 복구
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStart() 기종 불일치 기기 POWER OFF");
		fnAPL_ProgramReInstMsg();
	}

	m_pDevCmn->fnAPL_LoadDevice();								// 장치올리기
	m_pTranCmn->SetOwner(this, &NHGuros, &NHOcr);				// 업무포인터설정및초기화 for Giro
	SetOwner(this);												// 메인포인터설정및초기화

	InitializeDisplayDevice();									// for T2ATM 2006.07.25 : Get Monitor Information

	CheckSecurity();											// 2007.12.14 msj 보안관련 처리삽입
	RestoreBrandFile();                                         // #0091  => #N0169

	// EMV Lib													// V09-33-00-#01 ICCM 모듈 적용 
	m_pDevCmn->fnEMVLib_SetKernelType(5);						// EMV 2013.05.30 SHPARK - MAKE IT USE KERNEL V5
	
	SetTimer(TIMER_mSEC1000, TIMER_mSEC1000, NULL);				// TIMER_mSEC1000 기동

	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		WinExecEx("C:\\ATM\\EPFILE\\EPDOWNLOAD.EXE", SW_HIDE, (310 * 1000)); //#N0194
	}

	if(m_pDevCmn->HostLocalSvrMode == TRUE)
		m_pTranCmn->fnAPP_AcceptManagerPassword();


	m_pDevCmn->AtmDoing = TRUE;									// Atm실행여부
	while (m_pDevCmn->AtmDoing)
	{
		Delay_Msg();
		AtmHostCtrl();											// 자동기호스트제어

		Delay_Msg();
		AtmModeCtrl();											// 자동기모드제어

		if ((m_pDevCmn->AtmStatusSave == ATM_INIT)	&&			// INIT MODE를 빠져나온경우
			(m_pDevCmn->AtmStatus != ATM_INIT)		)			// 2004.06.28 Kalignite logo숨김
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStart():WorkArea복구처리");
			ResetWorkArea();
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStart():return");
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : long TraceDump(LPCTSTR szData)
// DESCRIPT  : 외부에서 트레이스를 남기는 것을 지원한다.
// PARAMETER : 남기고자 하는 데이타
// RETURN    : NORMAL(0)
// REMARK    : 
/////////////////////////////////////////////////////////////////////////////
long CWinAtmCtrl::TraceDump(LPCTSTR szTraceData) 
{
	// TODO: Add your dispatch handler code here

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[HTML] %s", szTraceData);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : long SetScreenData(LPCTSTR SetScreenDataName, LPCTSTR SetScreenDataValue)
// DESCRIPT  : 화면에서오는 자료를 전달한다.(To Mwi)
// PARAMETER : 데이타이름, 데이타
// RETURN    : NORMAL(0)
// REMARK    : 
/////////////////////////////////////////////////////////////////////////////
long CWinAtmCtrl::SetScreenData(LPCTSTR SetScreenDataName, LPCTSTR SetScreenDataValue) 
{
	// TODO: Add your dispatch handler code here
    CString strCommand("");
	CString strDataValue(SetScreenDataValue);

    strCommand.Format("%s", SetScreenDataName);
    if (strCommand == "DING")
    {
        m_pDevCmn->fnSND_KeySound(_SOUND_DDING);
        return T_OK;
    }
	
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "SetScreenData Name,Value[%s, %s]", strCommand, SetScreenDataValue);

	m_pDevCmn->fnSCR_SetScreenData(SetScreenDataName, SetScreenDataValue);
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Active X Control 문제 해결
// 문제점 : Invisible on Runtime설정시 ActiveX의 Window가 만들어지지 않아 핸들이 없음
// 증상   : ActiveX를 이용한 어떠한 수행도 처리되지 않음
// 해결   : 윈도를 재생성하는 과정을 수행(?)
// 참고   : MSDN Article No : Q195188
/////////////////////////////////////////////////////////////////////////////
void CWinAtmCtrl::OnSetClientSite() 
{
    if (m_pClientSite)
        // It doesn't matter who the parent window is or what the size of
        // the window is because the control's window will be reparented
        // and resized correctly later when it's in-place activated.
        VERIFY (CreateControlWindow (::GetDesktopWindow(), CRect(0,0,0,0),
                                      CRect(0,0,0,0)));
    COleControl::OnSetClientSite();
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : void OnTimer(UINT nIDEvent) 
// DESCRIPT  : 
// PARAMETER : 
// RETURN    : 
/////////////////////////////////////////////////////////////////////////////
void CWinAtmCtrl::OnTimer(UINT nIDEvent) 
{
	int			*pTimer;
	int			i = 0;
	
	KillTimer(nIDEvent);										// TIMER 중지
	switch(nIDEvent)
	{
		case TIMER_mSEC1000:
			// 1Sec마다 0일때까지									
			pTimer = (int *)&m_pDevCmn->Timer;
			for (i = 0; i < (sizeof(m_pDevCmn->Timer) / sizeof(int)); i++)
			{
				if (pTimer[i])
					pTimer[i]--;
			}
			
			SetTimer(TIMER_mSEC1000, TIMER_mSEC1000, NULL);		// TIMER_mSEC1000 기동
			break;
			
		default:
			break;
	}
	
	COleControl::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : int SetWorkArea()
// DESCRIPT  : Primary Monitor의 작업영역을 조작한다.
// PARAMETER : 없음
// RETURN    : 
// REMARK	 : 2004.06.28
/////////////////////////////////////////////////////////////////////////////
int CWinAtmCtrl::SetWorkArea()
{
#define	_REGKEY_DEFSETTING			_T("SOFTWARE\\ATM\\APP\\DEFAULTSETTING")

	SystemParametersInfo(SPI_GETWORKAREA, NULL, rWorkArea, 0);

	if ( RegGetInt(_REGKEY_DEFSETTING, "HIDELOGO") )
		SystemParametersInfo(SPI_SETWORKAREA, 
							 NULL, 
							 CRect(rWorkArea.left, rWorkArea.top+rWorkArea.bottom-1, rWorkArea.right, rWorkArea.bottom),
							 0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : int ResetWorkArea()
// DESCRIPT  : primary Monitor의 작업영역을 복구한다.
// PARAMETER : 없음
// RETURN    : 
// REMARK	 : 2004.06.28
/////////////////////////////////////////////////////////////////////////////
int CWinAtmCtrl::ResetWorkArea()
{
	SystemParametersInfo(SPI_SETWORKAREA, NULL, rWorkArea, 0);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// for T1ATM 2008.6.17 : 화면교대(Loader<>RearAp)
//////////////////////////////////////////////////////////////////////////
// 화면교대(Loader<>RearAp)
void CWinAtmCtrl::SetClerkScreen()
{
//	if (MaxMonitorIndex >= 1)									// 2Monitor이상
	m_pProfile->GetDeviceProfile();

	static int RearIsFront = 0;
	HWND hFront = NULL;
	HWND hRear	= NULL;

	hFront	= ::FindWindow(NULL, "Loader");
	hRear	= ::FindWindow(NULL, "[Hyosung T1Atm] REARAP");


	
	if (RearIsFront)
	{
		::SetWindowPos(hRear, HWND_NOTOPMOST, 800, 0, 1600, 600, SWP_NOACTIVATE);
		::MoveWindow(hRear, 800, 0, 800, 600, FALSE);			// RearAp Move

		Delay_Msg(100);

		::SetWindowPos(hFront, HWND_NOTOPMOST, 0 - 4, 0 - 50, 800 + 4, 600 + 100, SWP_SHOWWINDOW);
		::MoveWindow(hFront, 0 - 4, 0 - 50, 800 + 4, 600 + 100, TRUE);				// Loader Move

		RearIsFront = 0;
	}
	else
	{
		::SetWindowPos(hFront, HWND_NOTOPMOST, 800+4, 0+50, 1600-4, 600-100, SWP_NOACTIVATE);
		::MoveWindow(hFront, 800, 0, 800, 600, FALSE);			// Loader Move
		
		Delay_Msg(100);
		
		::SetWindowPos(hRear, HWND_NOTOPMOST, 0, 0, 800, 600, SWP_SHOWWINDOW);
		::MoveWindow(hRear, 0, 0, 800, 600, TRUE);				// RearAp Move
		
		RearIsFront = 1;
	}	
	
	return;
}

////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------
// Callback Function
// Call From	: CMainFrame::InitializeDisplayDevice()::EnumDisplayMonitor()
// DESC			: 모니터를 찾을때마다 호출되며, True를 반환하면 
//				  정보가 있는한 계속 찾는다.
//                함수에서는 그 정보를 구조체에 저장시킨다.
// Edited by teo 2002.01.09
// -------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK CallbackMonitorInfo(HMONITOR	hMonitor,	// 핸들
								  HDC		hdc,		// DC
								  LPRECT	prc,		// RECT
								  LPARAM	lParam)		// Reserved by 0
{
	// 모니터 정보 구조체 선언 1
	MONITORINFOEX	MIX;
	// 모니터 정보 구조체 선언 2
	MONITORINFO		MI;

	MI.cbSize = sizeof(MI);
	GetMonitorInfo(hMonitor, &MI);		// 현재핸들의 모니터 정보조회 1

	MIX.cbSize = sizeof(MIX);
	GetMonitorInfo(hMonitor, &MIX);		// 현재핸들의 모니터 정보조회 2

	MonitorInfo[CurMonitorIndex].hMonitor	= hMonitor;					// 모니터의 핸들
	strcpy(MonitorInfo[CurMonitorIndex].szDevice, (LPSTR)MIX.szDevice);	// 모니터의 디바이스정보
	MonitorInfo[CurMonitorIndex].rcMonitor	= MI.rcMonitor;				// 모니터 영역
	MonitorInfo[CurMonitorIndex].rcWork		= MI.rcWork;				// 모니터의 작업영역
	MonitorInfo[CurMonitorIndex].dwFlag		= MI.dwFlags;				// 모니터 설정(기본모니터 여부)

	// 현재의 모니터 정보중 설정값이 MONITORINFOF_PRIMARY(기본모니터)인 경우
	// 그 인덱스를 PriMonitorIndex에 저장한다.(기본모니터 인덱스 저장)
	if (MonitorInfo[CurMonitorIndex].dwFlag == MONITORINFOF_PRIMARY)
		PriMonitorIndex = CurMonitorIndex;

	// 저장 인덱스 증가
	CurMonitorIndex++;

	// True를 리턴하면 EnumDisplayMonitor의 결과가 참(모니터의 정보 더있음)일
	// 동안 계속 찾기를 수행한다.
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// 출력 디바이스 초기화
// Multi-Monitor관련하여 출력디바이스 정보를 확인하고 설정한다.
// by teo
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
void CWinAtmCtrl::InitializeDisplayDevice()
{
	///////////////////////////////////////////////////////////////////////////
	// ------------------------------------------------------------------------
	// Multi-Monitor를 위한 설정(2002.01.09 by teo)
	// 설치된 모니터의 정보를 얻어온다.
	// GetDC의 인자가 NULL이면 모든 Screen의 DC를 얻어온다.
	// 얻어온 모든 DC를 가지고 연관된 모니터의 정보를 확인한다.
	// ------------------------------------------------------------------------
	///////////////////////////////////////////////////////////////////////////
	HDC		hDc;
	CString strSysVer = _T("");
	// DC얻음
	hDc = ::GetDC(NULL);		
	// 얻어온 DC를 이용하여 계속 모니터찾기와 정보설정을 반복함.
	EnumDisplayMonitors(hDc, NULL, CallbackMonitorInfo, 0);
	// 다 사용한 DC 반환
	::ReleaseDC(NULL, hDc);

	// 마지막 모니터 정보의 인덱스를 저장한다.
	MaxMonitorIndex = CurMonitorIndex - 1;

	RegSetInt( _REGKEY_DEFSETTING, "DisplayCount", (MaxMonitorIndex + 1) );
	// 2006.12.21 V04.00.04 Max모니터 개수를 레지스트리에 등록
	
	// 기본모니터가 아닌 모니터정보중 가장 처음에 있는 정보의 
	// 인덱스를 얻어와 저장한다.
	// 이때부터 CurMonitorIndex는 모니터정보저장 인덱스가 아니고
	// 첫 보조모니터 정보의 인덱스가 된다.

//-----------------------------------------------------------------------------------------
	// U8100의 경우 SYSTEMCD의 버전을 확인하여 모니터의 인덱스를 확인한다.
	strSysVer = IniGetStr(_SYSTEMCDINI, "SYSTEM", "VERSION");

	if((m_pProfile->DEVICE.CashHandler == HBRM_SET) && (strSysVer.Find("V02.") >= 0) && (m_pDevCmn->OSType != OS_WIN7))  //#N0228 WIN7 지원
	{	
		CurMonitorIndex = MaxMonitorIndex;
	}
	else
	{
		for (int i = 0; i<=MaxMonitorIndex;i++)
		{
			if (i != PriMonitorIndex) 
			{
				CurMonitorIndex = i;
				break;
			}
		}
	}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// V01.01.09
	int nMachineType = GetPrivateProfileInt(DEV_SEC, "MachineType", 0, _DEVICE_INI);
	if ((nMachineType == U8100)		||							
		(nMachineType == U3100K)		)						
	{
		m_rOplRect = MonitorInfo[CurMonitorIndex].rcWork;
		m_rSplRect = MonitorInfo[PriMonitorIndex].rcWork;
		// Primary Set - Start : 2006.11.27
		//	m_rSplRect = MonitorInfo[PriMonitorIndex].rcWork;
		//	m_rOplRect = MonitorInfo[CurMonitorIndex].rcWork;
		DEVMODE DM;
		EnumDisplaySettings((LPCTSTR)"\\\\.\\DISPLAY1", ENUM_CURRENT_SETTINGS, &DM);
		if(m_pDevCmn->OSType == OS_WIN7)			//#N0228 WIN7 지원
		{
			m_rSplRect = MonitorInfo[PriMonitorIndex].rcWork;
			m_rOplRect = MonitorInfo[CurMonitorIndex].rcWork;
		}
		else
		if (0 == DM.dmPosition.x)								// Primary = OPL
		{
			m_rOplRect = MonitorInfo[PriMonitorIndex].rcWork;
			m_rSplRect = MonitorInfo[CurMonitorIndex].rcWork;
		}
		else													// Primary = SPL
		{
			m_rSplRect = MonitorInfo[PriMonitorIndex].rcWork;
			m_rOplRect = MonitorInfo[CurMonitorIndex].rcWork;
		}
		// Primary Set - End : 2006.11.27
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[해상도][Front=%dx%d][Rear=%dx%d]", 
													m_rOplRect.Width(), m_rOplRect.Height(), m_rSplRect.Width(), m_rSplRect.Height());

	return;
}

void CWinAtmCtrl::SetClerkScreen(BOOL bRearIsFront)
{
	int nResult = 0;
	DEVMODE DM;

	static int RearIsFront = 0;
	HWND hFront = NULL;
	HWND hRear	= NULL;

	hFront	= ::FindWindow(NULL, "Loader");
	hRear	= ::FindWindow(NULL, "[Hyosung T1Atm] REARAP");

	if (bRearIsFront)
	{
		m_pDevCmn->fnSCR_DisplayScreen(998);					// Black Screen

		if ((EnumDisplaySettings((LPCTSTR)"\\\\.\\DISPLAY1", ENUM_CURRENT_SETTINGS, &DM) != 0) &&
			(m_pDevCmn->OSType != OS_WIN7))						//#N0228 WIN7 지원
		{
			DM.dmSize = sizeof(DEVMODE);

			DM.dmBitsPerPel = 32;
			DM.dmPelsWidth = m_rSplRect.Width();
			DM.dmPelsHeight = m_rSplRect.Height();
			DM.dmFields = (DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT);

			nResult = ChangeDisplaySettingsEx((LPCTSTR)"\\\\.\\DISPLAY1" , &DM, NULL, (CDS_FULLSCREEN), NULL);

			Delay_Msg(100);
		}

		if(m_pDevCmn->OSType == OS_WIN7)						//#N0228 WIN7 지원
		{
			::SetWindowPos(hRear, HWND_TOP, m_rOplRect.left, 0, m_rOplRect.Width(), m_rOplRect.Height(), SWP_NOACTIVATE);
			
			Delay_Msg(100);

			::SetWindowPos(hFront, HWND_TOP, m_rSplRect.left, 0, m_rSplRect.Width(), m_rSplRect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			::SetWindowPos(hFront, HWND_NOTOPMOST, m_rSplRect.Width(), 0, 0, 0, SWP_NOACTIVATE);
			::MoveWindow(hFront, m_rSplRect.left, 0, m_rOplRect.Width(), m_rOplRect.Height(), FALSE);	// Loader Move
			
			Delay_Msg(100);
			
			::SetWindowPos(hRear, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW);
			::MoveWindow(hRear, 0, 0, m_rOplRect.left, m_rSplRect.Height(), TRUE);						// RearAp Move
		}

		RearIsFront = 1;
	}
	else
	{
		if (!RearIsFront)	return;

		if ((EnumDisplaySettings((LPCTSTR)"\\\\.\\DISPLAY1", ENUM_CURRENT_SETTINGS, &DM) != 0)  &&
			(m_pDevCmn->OSType != OS_WIN7))						//#N0228 WIN7 지원
		{
			DM.dmSize = sizeof(DEVMODE);

			DM.dmBitsPerPel = 32;
			DM.dmPelsWidth = m_rOplRect.Width();
			DM.dmPelsHeight = m_rOplRect.Height();
			DM.dmFields = (DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT);

			nResult = ChangeDisplaySettingsEx((LPCTSTR)"\\\\.\\DISPLAY1" , &DM, NULL, (CDS_FULLSCREEN), NULL);

			Delay_Msg(100);
		}

		if(m_pDevCmn->OSType == OS_WIN7)						//#N0228 WIN7 지원
		{
			::SetWindowPos(hRear, HWND_TOP, m_rOplRect.left, 0, m_rOplRect.Width(), m_rOplRect.Height(), SWP_NOACTIVATE);
			
			Delay_Msg(100);

			::SetWindowPos(hFront, HWND_TOP, m_rSplRect.left, 0, m_rSplRect.Width(), m_rSplRect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			::SetWindowPos(hRear, HWND_NOTOPMOST, m_rOplRect.Width(), 0, 0, 0, SWP_NOACTIVATE);
			::MoveWindow(hRear, m_rSplRect.left, 0, m_rSplRect.Width(), m_rSplRect.Height(), TRUE);	// RearAp Move

			Delay_Msg(100);

			::SetWindowPos(hFront, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW);
			::MoveWindow(hFront, 0, 0, m_rOplRect.left, m_rOplRect.Height(), TRUE);					// Loader Move
		}

		RearIsFront = 0;
	}	
	
	return;
}
//2008/01/04 AP실행시 터시 실행(실행중일때는 미실행)
/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : int CheckSecurity()
// DESCRIPT  : 보안을 위해서 파일확인/정리처리를 담당
/////////////////////////////////////////////////////////////////////////////
int CWinAtmCtrl::CheckSecurity()
{
MsgDump(TRACE_CODE_MODE, "Log",__FILE__, __LINE__, "CheckSecurity()");
	CFileFind FileFind;

	int nMachineType = IniGetInt(_DEVICE_INI,DEV_SEC, "MachineType", 0);
	HWND hTpdual = ::FindWindow(NULL, "TPDUAL_DRV");
	if (hTpdual)
	{
		if (FileFind.FindFile(EXE_DUAL_TOUCH_LNK1))				// 시작프로그램 단축아이콘을 찾아 삭제한다
			DeleteFile(EXE_DUAL_TOUCH_LNK1);
		if (FileFind.FindFile(EXE_DUAL_TOUCH_LNK2))				// 시작프로그램 단축아이콘을 찾아 삭제한다
			DeleteFile(EXE_DUAL_TOUCH_LNK2);
		if (FileFind.FindFile(EXE_DUAL_TOUCH_LNK3))				// 시작프로그램 단축아이콘을 찾아 삭제한다
			DeleteFile(EXE_DUAL_TOUCH_LNK3);
		if (FileFind.FindFile(EXE_DUAL_TOUCH_LNK4))				// 시작프로그램 단축아이콘을 찾아 삭제한다
			DeleteFile(EXE_DUAL_TOUCH_LNK4);
		if (FileFind.FindFile(EXE_DUAL_TOUCH_LNK5))				// 시작프로그램 단축아이콘을 찾아 삭제한다
			DeleteFile(EXE_DUAL_TOUCH_LNK5);
	}
	else
	{
		// 1. Dual Touch Driver를 실행한다.-실행여부를 확인하여 미실행시 실행하도록 한다
		if (FileFind.FindFile(EXE_DUAL_TOUCH))					// Touch실행프로그램이 있는 경우 처리
		{
			char szCurPath[MAX_PATH];
			memset(szCurPath, 0x00, MAX_PATH);

			GetCurrentDirectory(256, szCurPath);				//현재 Dir얻어 저장(AP Path)
			SetCurrentDirectory(EXE_DUAL_TOUCH_PATH);			//현재 Dir바꾸기(Touch Path) 

			if (::FindWindow(NULL, "TPDUAL_DRV") == NULL)
				WinExec(EXE_DUAL_TOUCH, SW_HIDE);					//Touch 실행
			SetCurrentDirectory(szCurPath);						//현재 Dir복원(AP Path)
		}

		if (FileFind.FindFile(EXE_DUAL_TOUCH_LNK1))				// 시작프로그램 단축아이콘을 찾아 삭제한다
			DeleteFile(EXE_DUAL_TOUCH_LNK1);
		if (FileFind.FindFile(EXE_DUAL_TOUCH_LNK2))				// 시작프로그램 단축아이콘을 찾아 삭제한다
			DeleteFile(EXE_DUAL_TOUCH_LNK2);
		if (FileFind.FindFile(EXE_DUAL_TOUCH_LNK3))				// 시작프로그램 단축아이콘을 찾아 삭제한다
			DeleteFile(EXE_DUAL_TOUCH_LNK3);
		if (FileFind.FindFile(EXE_DUAL_TOUCH_LNK4))				// 시작프로그램 단축아이콘을 찾아 삭제한다
			DeleteFile(EXE_DUAL_TOUCH_LNK4);
		if (FileFind.FindFile(EXE_DUAL_TOUCH_LNK5))				// 시작프로그램 단축아이콘을 찾아 삭제한다
			DeleteFile(EXE_DUAL_TOUCH_LNK5);
	}


	// 2. 화상키보드 프로그램을 삭제처리한다.
	if (FileFind.FindFile(EXE_OSK))
		DeleteFile(EXE_OSK);

	if (FileFind.FindFile(EXE_OSK_LNK1))						// 화상키보드 단축아이콘을 찾아 삭제한다
		DeleteFile(EXE_OSK_LNK1);
	if (FileFind.FindFile(EXE_OSK_LNK2))						// 화상키보드 단축아이콘을 찾아 삭제한다
		DeleteFile(EXE_OSK_LNK2);
	if (FileFind.FindFile(EXE_OSK_LNK3))						// 화상키보드 단축아이콘을 찾아 삭제한다
		DeleteFile(EXE_OSK_LNK3);
	if (FileFind.FindFile(EXE_OSK_LNK4))						// 화상키보드 단축아이콘을 찾아 삭제한다
		DeleteFile(EXE_OSK_LNK4);
	if (FileFind.FindFile(EXE_OSK_LNK5))						// 화상키보드 단축아이콘을 찾아 삭제한다
		DeleteFile(EXE_OSK_LNK5);


	// 3. 공장출하테스트용 테스트프로그램이 있으면 삭제처리한다
	if (FileFind.FindFile(EXE_TMFA))
		DeleteFile(EXE_TMFA);
	if (FileFind.FindFile(EXE_VDMFA))	
		DeleteFile(EXE_VDMFA);

		

	return TRUE;
}


// 2010.05.13 Yun.H.J 매체 적용시 브랜드 화면 복원
/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : int RestoreBrandFile()
// DESCRIPT  : 매체 적용시 브랜드 화면 복원
/////////////////////////////////////////////////////////////////////////////
int CWinAtmCtrl::RestoreBrandFile()
{
	//#N0169
	CString strUpdateFile = "";
	CString strCMD = "";
	int nApKind = IniGetInt(_DEVICE_INI,DEV_SEC, "ETCDevice20", 0);
	int BrandJehyu = IniGetInt(_TRANS_INI, TRANS_SEC, "BrandJehyu", 0);

	switch (nApKind)
	{
	case NONGHYUP:
		if(BrandJehyu == NONGHYUPDAN)	// #N0284
		{
			strUpdateFile = BAT_NOHD_UPDATE; 
		}
		else
		{
			strUpdateFile = BAT_NOH_UPDATE; 
		}
		break;
	case KIUP:
		strUpdateFile = BAT_KI_UPDATE; 
		break;
	case KEB:
		strUpdateFile = BAT_KEB_UPDATE; 
		break;
	case KWANGJU:
		strUpdateFile = BAT_KW_UPDATE; 
		break;
	case KYONGNAM:
		strUpdateFile = BAT_KYN_UPDATE;   //#N0192
		break;
	case HANA:
		strUpdateFile = BAT_HANA_UPDATE;   //#N0214
		break;
	case SHINHAN:
		strUpdateFile = BAT_SHINHAN_UPDATE;   //#N0215
		break;
	case SC:
		strUpdateFile = BAT_SC_UPDATE;	  //#N0215
		break;
	case KB:
		strUpdateFile = BAT_KB_UPDATE;	  //#N0226
		break;
	case CITI:
		strUpdateFile = BAT_CITI_UPDATE;   //#N0238
		break;
	default:
		return TRUE;
	}

	MsgDump(TRACE_CODE_MODE, "Log",__FILE__, __LINE__, "RestoreBrandFile() : %d [%s]", nApKind, strUpdateFile);

	if(PathFileExists(strUpdateFile))
	{
		WinExecWaitEnd(strUpdateFile, SW_HIDE);
		DeleteFile(strUpdateFile);
		MsgDump(TRACE_CODE_MODE, "Log",__FILE__, __LINE__, "RestoreBrandFile() : return()");
	}
	return TRUE;
}

// 2012.01.03 Kim.G.J 매체 적용시 브랜드 화면 복원 //#0133
/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : int RestoreBrandFileCopy()
// DESCRIPT  : 매체 적용시 브랜드 화면 복원
/////////////////////////////////////////////////////////////////////////////
int CWinAtmCtrl::RestoreBrandFileCopy()
{
	CString strCurrentDir="";
	CString strCurrentAdDir="";
	CString strCommand = "";
	int nApKind = IniGetInt(_DEVICE_INI,DEV_SEC, "ETCDevice20", 0);
	int BrandJehyu = IniGetInt(_TRANS_INI, TRANS_SEC, "BrandJehyu", 0);

	//#0091
	switch (nApKind)
	{
	case KB:
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\KB\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\KB\\CFMonitor");
		break;
	case KIUP:
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\KI\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\KI\\CFMonitor");
		break;
	case JEJU:
	case JEJU2:
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\JE\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\JE\\CFMonitor");
		break;
	case MYASSET:
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\TYJ\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\TYJ\\CFMonitor");
		break;
	case KWANGJU:
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\KW\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\KW\\CFMonitor");
		break;
	case SC:
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\SC\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\SC\\CFMonitor");
		break;
	case HYDCPTL:
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\HYDCPTL\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\HYDCPTL\\CFMonitor");
		break;
	case NONGHYUP:
		// 농협과 농협은행으로 분리 되지만 BG만 변경되어 이미지 복사처리
		if(BrandJehyu == NONGHYUPDAN)	// #N0284
		{
			strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\NOHD\\Swf");
			strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\NOHD\\CFMonitor");
		}
		else
		{
			strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\NOH\\Swf");
			strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\NOH\\CFMonitor");
		}
		break;
	case POST:
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\POS\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\POS\\CFMonitor");
		break;
	case KEB:
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\KEB\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\KEB\\CFMonitor");
		break;
	case SUHYUP:
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\SUH\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\SUH\\CFMonitor");
		break;
	case CITI:		//#N0238
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\CITI\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\CITI\\CFMonitor");
		break;
	case HANA:
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\HANA\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\HANA\\CFMonitor");
		break;
	case KYONGNAM:  //#N0192
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\KYN\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\KYN\\CFMonitor");
		break;
	case SHINHAN:  //#N0215
		strCurrentDir.Format("C:\\T1Atm\\App\\BrandSwf\\SH\\Swf");
		strCurrentAdDir.Format("C:\\T1Atm\\App\\BrandSwf\\SH\\CFMonitor");
		break;
	default:
		return TRUE;
	}

	strCommand.Format("C:\\T1atm\\App\\Main\\Goma.exe SwfPlayer.exe");		//#N0218
	WinExecWaitEnd(strCommand, SW_HIDE);
	//WinExecWaitEnd("C:\\T1ATM\\APP\\Main\\GOMA.EXE   SwfPlayer.exe", SW_HIDE);
	Delay_Msg(2000);

	if (SetCurrentDirectory(strCurrentDir))
	{
		DeleteFile(_ATMTRCODEENCRYPT_INI);
		strCommand.Format("XCOPY *.* C:\\T1Atm\\App\\Swf\\ /S /Y");
		WinExecWaitEnd(strCommand, SW_HIDE);
		MsgDump(TRACE_CODE_MODE, "Log",__FILE__, __LINE__, "RestoreBrandFileCopy() : %s [%s]", strCurrentDir, strCommand);
	}

	if (SetCurrentDirectory(strCurrentAdDir))
	{
		DeleteFile(_ATMTRCODEENCRYPT_INI);
		strCommand.Format("XCOPY *.* C:\\T1Atm\\CFMonitor\\ /S /Y");
		WinExecWaitEnd(strCommand, SW_HIDE);
		MsgDump(TRACE_CODE_MODE, "Log",__FILE__, __LINE__, "RestoreBrandFileCopy() : %s [%s]", strCurrentAdDir, strCommand);

		if(m_pProfile->DEVICE.MachineType == U3100K)  //#N0224
		{	
			strCommand.Empty();
			strCommand.Format("XCOPY AdList.Ini D:\\INI\\ /S /Y");
			WinExecWaitEnd(strCommand, SW_HIDE);
			MsgDump(TRACE_CODE_MODE, "Log",__FILE__, __LINE__, "RestoreBrandFileCopy() : %s [%s]", strCurrentAdDir, strCommand);
		}
	}
	MsgDump(TRACE_CODE_MODE, "Log",__FILE__, __LINE__, "RestoreBrandFileCopy() return");

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : Atm_SW_Control()
// DESCRIPT  : Registry의 Run 디렉토리에 WinLock.exe 등록
// PARAMETER : 없음
// RETURN    : 등록 성공시(기존에 있어도) TRUE, 실패시(등록 안됨) FALSE
// REMARK    : 2007.03.13
/////////////////////////////////////////////////////////////////////////////
int CWinAtmCtrl::Atm_SW_Control()
{
	HKEY hSubKey = NULL, hKey = NULL;
	char szString[255];
	DWORD dwDisp = 0, dwType = 0, cbData = 255; 

	char lpData[] = "C:\\T1ATM\\App\\Main\\WinLogin.exe";
	LONG lRet = 0;
 	CString strGetIp("");
	CFileStatus fs;   
	
	memset(szString, 0, sizeof(szString));
  
	//ATMInside
	if (::FindWindow(NULL, "ATMinside") == NULL)
		WinExec(EXE_ATMINSIDE, SW_HIDE);
/*
	if (::FindWindow(NULL, "ATM Control Server 0.2") == NULL)
		WinExec(EXE_FTP_SERVER, SW_HIDE);
*/                  
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
/**
 *	@author		
 *	@date		2009.02.20
 *
 *	@return		Default Value (의미없음)
 *
 *	@remarks
 *		V05-06-03-#13 : 설치AP/기종 불일치에 따른 MessageDisplay & ShutDown
 */
int CWinAtmCtrl::fnAPL_ProgramReInstMsg()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ProgramReInstMsg()");
	CString strReturn("");
	CString strPwd("8850199");

	while (TRUE)
	{
		m_pDevCmn->fnSCR_DisplayScreen(121);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == strPwd)
			WindowsShutdownProcess();
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ProgramReInstMsg()::nReturn");
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// SW공통-0710-#01 EP Download 지원
int CWinAtmCtrl::WinExecEx(LPCTSTR lpCmdLind, UINT uCmdShow, DWORD dwTimeOut)
{
	PROCESS_INFORMATION	pi;
	STARTUPINFO			si;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	BOOL b = CreateProcess(NULL, const_cast<LPTSTR>(lpCmdLind), NULL, NULL, TRUE,
						   NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);

	if (!b)
	{
MsgDumpAnal(TRACE_CODE_MODE, "Log", "[SHU EPDOWN Fail] = [%d]",b);
		return FALSE;
	}

	WaitForSingleObject(pi.hProcess, dwTimeOut);

	CloseHandle(pi.hThread);									// V08-04-01:Codesonar
	CloseHandle(pi.hProcess);									// V08-04-01:Codesonar

	return TRUE;
}



