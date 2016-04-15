/////////////////////////////////////////////////////////////////////////////
//	DevCmn.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\WinAtm\nhmwi.h"
#include "..\..\WinAtm\nhiccm.h"
#include "..\..\WinAtm\nhmwiemv.h"
#include "..\..\WinAtm\nhalert4spackx.h"

#include "..\..\H\EMVInc\\AppErrCode.h"
#include "..\..\H\EMVInc\\SCSp_ATM_EMV.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"
#include "..\..\Dll\DevCtrl\DevDes.h"

#include "..\..\H\Dll\CFileVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	CDevCmn Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
CDevCmn::CDevCmn()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CDevCmn()");
	CString     strTmp;
	m_pProfile	= NULL;
	m_pNetWork	= NULL;
	m_pSnd		= NULL;

	OpenKey		= TRUE;
	OpenKeySave = TRUE;
///////////////////////////////////// 한글입력
	fnSCR_HANGULInit();

/////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CDevCmn():return");
}

CDevCmn::~CDevCmn()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~CDevCmn()");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~CDevCmn():return");
}


BEGIN_MESSAGE_MAP(CDevCmn, CWnd)
	//{{AFX_MSG_MAP(CDevCmn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
//	CDevCmn message handlers
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	함수군
/////////////////////////////////////////////////////////////////////////////
// 포인터설정및초기화
void CDevCmn::SetOwner(CWinAtmCtrl* pOwner, CNHMWI* pMwi, CNHAlert4SPackX* pNHAlertSpack, CNHICCM* pIccm, CNHMwiEmv* pMwiEmv)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~SetOwner()");

#include "..\..\H\Common\ClassSetOwner.h"

	m_pProfile	= new CProfile;
	m_pMwi		= pMwi;
	m_pICCM		= pIccm;					// #NICCM01	ICCM 모듈 적용으로 인한 ICCM 포인터 추가
	m_pMwiEmv	= pMwiEmv;					// #NICCM01	EMV 포인터 추가
	m_NHAlertSpack	= pNHAlertSpack;
	fnAPL_InitializeDS();
	

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~SetOwner():return");
	return;
}

/////////////////////////////////////////////////////////////////////////////
//	저널부함수(JPR) : JPR SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnJPR_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	저널부함수(JPR) : JPR GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrJPR_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrJPR_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_JPR))
		strReturn = m_pMwi->JprGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrJPR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치센서구하기
CString CDevCmn::fstrJPR_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrJPR_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_JPR))
		strReturn = m_pMwi->JprGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrJPR_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnJPR_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 용지상태구하기
int	CDevCmn::fnJPR_GetPaperStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetPaperStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprGetPaperStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetPaperStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 매체잔류정보구하기
int	CDevCmn::fnJPR_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	저널부함수(JPR) : JPR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnJPR_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Initialize()");

	int		nReturn = FALSE;

	JprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_JPR))
	{
		nReturn = m_pMwi->JprInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											DEVNM_JPR,
											RegGetStr(_REGKEY_EPVERSION, DEVNM_JPR, "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, DEVNM_JPR, "000000").GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnJPR_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Deinitialize()");

	int		nReturn = FALSE;

	JprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 엠보스인자
int	CDevCmn::fnJPR_EmbossPrint()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_EmbossPrint()");

	int		nReturn = FALSE;

	JprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprEmbossPrint();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_EmbossPrint():return(%d)", nReturn);
	return nReturn;
}

// 인자(인자자료)
int	CDevCmn::fnJPR_Print(LPCTSTR szPrintData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Print(szPrintData[%s])", szPrintData);

	int		nReturn = FALSE;

	JprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_JPR))
		nReturn = m_pMwi->JprPrint("", szPrintData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_Print(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	명세표부함수(SPR) : SPR SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnSPR_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// 회수매수초기화
int	CDevCmn::fnSPR_ClearRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_ClearRetractCnt()");

	int		nReturn = FALSE;


	if(m_pProfile->DEVICE.MachineType == U3100K)
		return nReturn;

	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprClearRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_ClearRetractCnt():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	명세표부함수(SPR) : SPR GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrSPR_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_SPR))
		strReturn = m_pMwi->SprGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치센서구하기
CString CDevCmn::fstrSPR_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_SPR))
		strReturn = m_pMwi->SprGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnSPR_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 용지상태구하기
int	CDevCmn::fnSPR_GetPaperStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetPaperStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprGetPaperStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetPaperStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 매체잔류정보구하기
int	CDevCmn::fnSPR_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// 회수매수구하기
CString CDevCmn::fstrSPR_GetRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetRetractCnt()");

	CString strReturn("");

	//#N0137
	if(m_pProfile->DEVICE.MachineType == U3100K)
		return strReturn;

	if (fnAPL_GetDefineDevice(DEV_SPR))
		strReturn = m_pMwi->SprGetRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSPR_GetRetractCnt():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	명세표부함수(SPR) : SPR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnSPR_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Initialize()");

	int		nReturn = FALSE;

	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
	{
		nReturn = m_pMwi->SprInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											DEVNM_SPR,
											RegGetStr(_REGKEY_EPVERSION, DEVNM_SPR, "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, DEVNM_SPR, "000000").GetBuffer(0));
	}
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[SPR      : INIT ]"));

	fnSPR_GetEjectBothAvail();									// V07-05-01-#02
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnSPR_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Deinitialize()");

	int		nReturn = FALSE;

	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 인자(인자자료)
int	CDevCmn::fnSPR_Print(LPCTSTR szPrintData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Print(szPrintData[%s])", szPrintData);

	int		nReturn = FALSE;

	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprPrint("", szPrintData);			

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Print(...):return(%d)", nReturn);
	return nReturn;
}

// 방출(수취대기시간)
int	CDevCmn::fnSPR_Eject(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Eject(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprEject(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Eject(...):return(%d)", nReturn);
	return nReturn;
}

// V07-05-01-#02:카드/명세표 동시방출 지원
// 카드/명세표 방출(수취대기시간)
int	CDevCmn::fnSPR_EjectBoth(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_EjectBoth(nWaitSec[%d])", nWaitSec);
	
	int		nReturn = FALSE;
	
	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR | DEV_MCU))
		nReturn = m_pMwi->SprEjectBoth(nWaitSec);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_EjectBoth(...):return(%d)", nReturn);
	return nReturn;
}	

// 수취대기
int	CDevCmn::fnSPR_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_WaitTaken()");

	int		nReturn = FALSE;

	//#N0137
	if (m_pProfile->DEVICE.MachineType == U3100K)
		return nReturn;

	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 수취대기취소
int	CDevCmn::fnSPR_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_CancelWaitTaken()");

	int		nReturn = FALSE;

	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 회수
int	CDevCmn::fnSPR_Retract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Retract()");

	int		nReturn = FALSE;


	if(m_pProfile->DEVICE.MachineType == U3100K)
		return nReturn;

	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprRetract();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Retract():return(%d)", nReturn);
	return nReturn;
}

// V02.07.01 AutoLoad 수행
int	CDevCmn::fnSPR_Autoload()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Autoload()");

	int		nReturn = FALSE;

#ifdef _SPR_AUTOLOAD_SUPPORT									// 명세표SP 오토로드 지원시 해당문구 삭제
	SprRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nReturn = m_pMwi->SprAutoLoad();
#endif

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_Autoload():return(%d)", nReturn);
	return nReturn;
}

// V02.07.01 AutoLoad 수행
int	CDevCmn::fnSPR_GetAutoloadCondition()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetAutoloadCondition()");

	if ((m_pProfile->DEVICE.MachineType != U8100)	&&			// U8100-AP변경
		(m_pProfile->DEVICE.MachineType != U3100K))
	{
		return NORMAL;											// T1ATM 기종만 해당처리 실시
	}

	int		nMediaStatus = FALSE;
	int		nPaperStatus = FALSE;
	int		nReturn      = FALSE;

#ifdef _SPR_AUTOLOAD_SUPPORT									// 명세표SP 오토로드 지원시 해당문구 삭제
	if (fnAPL_GetDefineDevice(DEV_SPR))
		nMediaStatus = m_pMwi->SprGetMediaStatus();
#endif

	if (fnAPL_GetDefineDevice(DEV_SPR))
		nPaperStatus = m_pMwi->SprGetPaperStatus();

if ((nMediaStatus == SLIP_MEDIA_PRESENT) && (nPaperStatus == SLIP_EMPTY_PAPER))
{
	nReturn = TRUE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetAutoloadCondition():return(%d)", nReturn);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetAutoloadCondition():return(%d)", nMediaStatus);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetAutoloadCondition():return(%d)", nPaperStatus);
}

	return nReturn;
}

//명세표/카드 동시방출 가능여부 확인
// 카드/명세표 동시방출 가능한 EP인지 검사
//BOOL CTranCmn::fnAPP_GetEjectBothAvail()
BOOL CDevCmn::fnSPR_GetEjectBothAvail()
{
	int		dpi = 0;
	CString	strVersion, strReg;
	strVersion.Empty();
	strReg.Empty();
	EjectBothAvail = FALSE;

	if (HBRM_SET != m_pProfile->DEVICE.CashHandler)
		return FALSE;

	strReg = RegGetStr(_REGKEY_EPVERSION, DEVNM_SPR, "000000");
	strReg.Replace(".", "");									// 버전 string의 '.' 제거
	strReg.Replace("-", "");									// 버전 string의 '-' 제거
	strVersion = strReg.Right(6);								// 우측의 6개 숫자만 남김

//	dpi = RegGetInt(_REGKEY_JPRINFO, "DPI", 0);
	dpi = RegGetInt(_REGKEY_SPRINFO, "DPI", 0);

/*  #N0168
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetEjectBothAvail():동시방출가능(%d)(%s)", dpi,strVersion);
	if (dpi == 300)
	{
		if (strVersion.Compare("030044") >= 0)
			EjectBothAvail = TRUE;
	}
	else if (dpi == 200)
	{
		if (strVersion.Compare("030105") >= 0)
			EjectBothAvail = TRUE;
	}
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPR_GetEjectBothAvail():동시방출 가능여부[%d]", EjectBothAvail);
*/
  return EjectBothAvail;
}
// V07-05-01-#02 End
/////////////////////////////////////////////////////////////////////////////
//	카드부함수(MCU) : MCU SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnMCU_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// 회수매수초기화
int	CDevCmn::fnMCU_ClearRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ClearRetractCnt()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuClearRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ClearRetractCnt():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	카드부함수(MCU) : MCU GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrMCU_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_MCU))
		strReturn = m_pMwi->McuGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치센서구하기
CString CDevCmn::fstrMCU_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_MCU))
		strReturn = m_pMwi->McuGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnMCU_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnMCU_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 매체잔류정보구하기
int	CDevCmn::fnMCU_GetMaterialInfo(int nType)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnMCU_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;



	if (fnAPL_GetDefineDevice(DEV_MCU))
	{
		switch(AtmDefine.MCUHandler)
		{
		case D_TYPE:
			if(nType) nReturn = m_pMwi->McuGetMaterialInfo();
			break;
		case S_TYPE:
			nReturn = m_pMwi->McuGetMaterialInfo();
			nReturn &= ~ST_SENSOR2;
			nReturn &= ~ST_SENSOR1;
			break;
		default:
			nReturn = m_pMwi->McuGetMaterialInfo();
			break;
		}
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// 회수매수구하기
CString CDevCmn::fstrMCU_GetRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetRetractCnt()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_MCU))
		strReturn = m_pMwi->McuGetRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetRetractCnt():return(%s)", strReturn);
	return strReturn;
}

// 카드자료구하기
CString CDevCmn::fstrMCU_GetCardData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetCardData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_MCU))
		strReturn = m_pMwi->McuGetCardData(0);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetCardData():return(%s)", strReturn);
	return strReturn;
}

// IC자료구하기
CString	CDevCmn::fstrMCU_GetICData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetICData()");

	CString	strReturn("");

	// #NICCM01	ICCM 모듈 적용	
	if (IfmDirect)
	{
		if(IfmDirect_Init)
		{	// V06-35-01 시범운영시에만 CODE 사용 전점적용시에는 PLUS
			strReturn = m_pMwi->McuGetICData();
			IfmDirect_Init = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetICData(MWI):return(%s), IfmDirect_Init(%d)", strReturn, IfmDirect_Init);
		}
		else
		{	// V06-35-01 시범운영시에만 CODE 사용 전점적용시에는 PLUS
			strReturn = m_pICCM->ICRecvData();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetICData(ICCM):return(%s)", strReturn);
		}
	}
	else
	{
		strReturn = m_pMwi->McuGetICData();
		// V06-35-01 시범운영시에만 CODE 사용 전점적용시에는 PLUS
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetICData():return(%s)", strReturn);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrMCU_GetICData():return(%s)", strReturn);
	return strReturn;
}

// IC Board Version Read V070300 3.
int	CDevCmn::fnMCU_GetICVersion()
{
	CString	strTemp(""), strReg("");
	char	sTemp[5] = {0,};
	int		nReturn = FALSE, iRet = NULL, iRet2 = NULL;

	IdcVersion = NULL;

	strReg = RegGetStr(_REGKEY_MCUINFO, DEVNM_ICVERSION);
	// #NICCM01	ICCM 모듈 적용
	if (AtmDefine.MachineType == U8100)
	{
		memcpy(FICEPVersion, strReg, strReg.GetLength());

		if (memcmp(FICEPVersion, "GemPosP", 7) == 0)
		{
			IfmDirect = TRUE;
			Mcu97934Error = FALSE;
		}
		else
		if (strlen((char *)FICEPVersion) == 0)
		{
			m_pDevCmn->fnAPL_StackError("97934@@", "ICCM Version Fail", DEV_MCU);
			DeviceStatus |= DEV_MCU;
			IfmDirect = FALSE;
			Mcu97934Error = TRUE;
		}
		else
		{
			IfmDirect = FALSE;
			Mcu97934Error = FALSE;
		}
		
		IfmDirect_Init = FALSE;
	}
	else
	{
		IfmDirect = FALSE;
		IfmDirect_Init = FALSE;
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_GetICVersion():return(%s), IfmDirect(%d), IfmDirect_Init(%d)", FICEPVersion, IfmDirect, IfmDirect_Init);

	strTemp = strReg.Left(4);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_GetICVersion():return(%s)", strReg);
// 	if (strTemp.CompareNoCase("V000"))
// 		IdcVersion = CMI_BOARD;
// 	else IdcVersion = EMV_BOARD;

	IdcVersion = EMV_BOARD;

	nReturn = TRUE;
	ChkIcVersion = TRUE;										// T99-99-99-#01 IC 버전 체크 여부

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	카드부함수(MCU) : MCU OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnMCU_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Initialize()");

	int		nReturn = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
	{
		nReturn = m_pMwi->McuInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											"IDC",
											RegGetStr(_REGKEY_EPVERSION, "IDC", "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, "IDC", "000000").GetBuffer(0));
	}
	fnMCU_GetICVersion();										// 금융IC Board 구하기 V070300 3.
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[MCU      : INIT ]"));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnMCU_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Deinitialize()");

	int		nReturn = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// #NICCM01	ICCM 모듈 연결
int CDevCmn::fnMCU_ICCMOpenConnection()
{
	int		nReturn = FALSE;
	
	if(!IfmDirect) //#NICCM01-1
		return nReturn;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICCMOpenConnection : m_pICCM[%x]", m_pICCM);
	
	nReturn = m_pICCM->OpenConnection(MID_ICCMRCV_TIME);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICCMOpenConnection(%x)", nReturn);
	
	return nReturn;
}

// #NICCM01	ICCM 모듈 초기화
int CDevCmn::fnMCU_ICCMInitialize()
{
	int		nReturn = FALSE;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICCMInitialize()");
	
	nReturn = m_pICCM->ICCMInitialize(MID_ICCMRCV_TIME);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICCMInitialize(%x)", nReturn);
	
	return nReturn;
}

// #NICCM01	ICCM 모듈 연결종료
int CDevCmn::fnMCU_ICCMDeinitialize()
{
	int		nReturn = FALSE;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICCMDeinitialize()");
	
	nReturn = m_pICCM->CloseConnection();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICCMDeinitialize():return(%d)", nReturn);
	
	return nReturn;
}

// 투입허가(투입대기시간)
int	CDevCmn::fnMCU_EntryEnable(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_EntryEnable(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuEntryEnable(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_EntryEnable(...):return(%d)", nReturn);
	return nReturn;
}

// 투입금지
int	CDevCmn::fnMCU_EntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_EntryDisable()");

	int		nReturn = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuEntryDisable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_EntryDisable():return(%d)", nReturn);
	return nReturn;
}

// 읽기
int	CDevCmn::fnMCU_Read()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Read()");

	int		nReturn = FALSE;
	McuRecvTime = MID_DEVRSP_TIME;	

	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuRead();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Read():return(%d)", nReturn);
	return nReturn;
}

// 방출(수취대기시간)
int	CDevCmn::fnMCU_Eject(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Eject(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;
	
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuEject(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Eject(...):return(%d)", nReturn);
	return nReturn;
}

// V07-05-01-#02:카드/명세표 동시방출 지원
// 카드/명세표 방출(수취대기시간)
int	CDevCmn::fnMCU_EjectBoth(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_EjectBoth(nWaitSec[%d])", nWaitSec);
	
	int		nReturn = FALSE;
	
	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU | DEV_SPR))
		nReturn = m_pMwi->McuEjectBoth(nWaitSec);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_EjectBoth(...):return(%d)", nReturn);
	return nReturn;
} // V07-05-01-#02 End

// 수취대기
int	CDevCmn::fnMCU_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_WaitTaken()");

	int		nReturn = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 수취대기취소
int	CDevCmn::fnMCU_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_CancelWaitTaken()");

	int		nReturn = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 회수
int	CDevCmn::fnMCU_Retract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Retract()");


	
	int		nReturn = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuRetract();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Retract():return(%d)", nReturn);
	return nReturn;
}

// 기록(카드자료3TR)
int	CDevCmn::fnMCU_Write(LPCTSTR szTrackData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Write(szTrackData[%s])", szTrackData);

	int		nReturn = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuWrite(szTrackData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_Write(...):return(%d)", nReturn);
	return nReturn;
}

// IC칩초기화:접점이동
int CDevCmn::fnMCU_ICChipInitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICChipInitialize()");

	int		nReturn = FALSE;

	memset(ICSendData, 0, sizeof(ICSendData));	// #NICCM01	 IC전송자료초기화

	if (IfmDirect)		// #NICCM01	ICCM 모듈 적용
	{
		IfmDirect_Init = TRUE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICChipInitialize(), IfmDirect_Init(%d)", IfmDirect_Init);
	}
	else
		IfmDirect_Init = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuICChipInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICChipInitialize():return(%d)", nReturn);
	return nReturn;
}

// IC투입허가(투입대기시간)
int	CDevCmn::fnMCU_ICEntryEnable(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICEntryEnable(nWaitSec[%d])", nWaitSec);

	int	nReturn = FALSE;

	IsEmvChipPowerOn = TRUE;				// #NICCM01	ICCM 모듈 적용

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuICEntryEnable(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICEntryEnable(...):return(%d)", nReturn);
	return nReturn;
}

// IC투입금지
int	CDevCmn::fnMCU_ICEntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICEntryDisable()");

	int	nReturn = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuICEntryDisable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICEntryDisable(...):return(%d)", nReturn);
	return nReturn;
}

// IC자료전송(통신방식, 송신자료, 전송대기시간)
int	CDevCmn::fnMCU_ICSendData(int nProtocolId, LPCTSTR szICSendData, int nWaitSec)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICSendData(nProtocolId[%d], szICSendData[%s], nWaitSec[%d])", nProtocolId, szICSendData, nWaitSec);

	int		nReturn = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nReturn = m_pMwi->McuICSendData(nProtocolId, szICSendData, nWaitSec);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICSendData(...):return(%d)", nReturn);
	return nReturn;
}

// IC자료전송(ICCM SEND)		// #NICCM01	ICCM 모듈 적용
int	CDevCmn::fnMCU_ICCMSendData(int nProtocolId, short nCmdType, LPCTSTR szICSendData, int nWaitSec)
{
	IfmResult = FALSE;			// Success
	
	if (IfmDirect_Init)
	{
		IfmDirect_Init = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICCMSendData(), IfmDirect_Init(%d)", IfmDirect_Init);
	}
	
	int		nReturn = FALSE;
	
	if (fnAPL_GetDefineDevice(DEV_MCU))
	{
		nReturn = m_pICCM->ICSendData(nProtocolId, nCmdType, szICSendData, nWaitSec);
		IfmResult = nReturn;
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_ICCMSendData(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//개발4팀 남호현입니다. 
//참고로, 아래의 내용은 AP에서 남기고 관리합니다
//[Ucm]
//CrntInCheckCnt=0						// 입금 Cassette에 들어간 수표매수
//CrntOutCheckNo=00000000				// 출금수표번호
//CrntOutCheckCnt=1000					// 출금 Cassette에있는 수표 매수
//CrntOutCheckKind=XXX					// 출금 Cassette에있는 수표 권종	V02-08-01-R1
/////////////////////////////////////////////////////////////////////////////
//	수표입출금부함수(UCM) : UCM SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnUCM_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	수표입출금부함수(UCM) : UCM GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrUCM_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrUCM_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_UCM))
		strReturn = m_pMwi->UcmGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrUCM_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치센서구하기
CString CDevCmn::fstrUCM_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrUCM_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_UCM))
		strReturn = m_pMwi->UcmGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrUCM_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnUCM_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 정위치구하기
int	CDevCmn::fnUCM_GetPosition()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetPosition()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmGetPosition();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetPosition():return(%d)", nReturn);
}
	return nReturn;
}

// 입금카세트상태구하기
int	CDevCmn::fnUCM_GetDepCSTStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetDepCSTStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmGetDepCSTStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetDepCSTStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 출금카세트상태구하기
int	CDevCmn::fnUCM_GetWithCSTStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetWithCSTStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmGetWithCSTStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetWithCSTStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//개발4팀 권선주다. 
//	case INCASSETTE :
//		if (((m_SNDataAccept & 0x02) == 0x02) || 
//			((m_SNDataAccept & 0x01) == 0x01))		RetVal |= 0x01;
//		if ((m_SNDataAccept & 0x04) == 0x04 )		RetVal |= 0x02;
//		if ((m_SNDataAccept & 0x08) == 0x08 )		RetVal |= 0x04;
//		if ((m_SNDataAccept & 0x10) == 0x10 )		RetVal |= 0x08;
//		if ((m_SNDataAccept & 0x20) == 0x20 )		RetVal |= 0x10;
//		if ((m_SNDataAccept & 0x80) == 0x00 )		RetVal |= 0x20; ==>사양서에 잘못 표기되어 있음
//			break;
//	case OUTCASSETTE:
//		if ((m_SNDataDispense & 0x10) == 0x10)		RetVal |= 0x01;
//		if ((m_SNDataDispense & 0x01) == 0x01)		RetVal |= 0x02;
//		if ((m_SNDataDispense & 0x02) == 0x02)		RetVal |= 0x04;
//		if ((m_SNDataDispense & 0x08) == 0x08)		RetVal |= 0x08;
//		if ((DispenseStackSens & 0x01)== 0x01)		RetVal |= 0x10; ==>사양서 잘못표기
//			break;
//	Accept = 0 or { (S1orS2|S3|S4|S5|S6|S8) = (0x01|0x02|0x04|0x08|0x10|0x20) }
//		S1orS2 = 투입구, S3,S4,S5,S6 = 경로, S8 = 일시스태커
//	Dispense = 0 or { S13|S9|S10|S12|S15} = (0x01|0x02|0x04|0x08|0x10}
//		S13 = 방출구, S9,S10,S12 = 경로, S15 = 스태커
/////////////////////////////////////////////////////////////////////////////
// 매체잔류정보구하기(입출금구분)
int	CDevCmn::fnUCM_GetMaterialInfo(int nDepWithFlag)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetMaterialInfo(nDepWithFlag[%d])", nDepWithFlag);

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmGetMaterialInfo(nDepWithFlag);

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetMaterialInfo(...):return(%d)", nReturn);
}
	return nReturn;
}

// 입금가능상태구하기
int	CDevCmn::fnUCM_GetAvailDeposit()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetAvailDeposit()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmGetAvailDeposit();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetAvailDeposit():return(%d)", nReturn);
}
	return nReturn;
}

// 출금가능상태구하기
int	CDevCmn::fnUCM_GetAvailWithdraw()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetAvailWithdraw()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmGetAvailWithdraw();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_GetAvailWithdraw():return(%d)", nReturn);
}
	return nReturn;
}

// 낱장수표자료구하기
CString CDevCmn::fstrUCM_GetACheckData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrUCM_GetACheckData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_UCM))
		strReturn = m_pMwi->UcmGetACheckData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrUCM_GetACheckData():return(%s)", strReturn);
	return strReturn;
}

// 출금계수한수표자료구하기(인덱스)
CString CDevCmn::fstrUCM_GetCheckDataDispensed(int nIndex)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrUCM_GetCheckDataDispensed(nIndex[%d])", nIndex);

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_UCM))
		strReturn = m_pMwi->UcmGetCheckDataDispensed(nIndex);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrUCM_GetCheckDataDispensed():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	수표입출금부함수(UCM) : UCM OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnUCM_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Initialize()");

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
	{
		nReturn = m_pMwi->UcmInitialize(UcmRecvTime);
		MsgDumpAnal(TRACE_FREE_MODE, "AP", "[VERSION][%s][%s][%s]", 
											DEVNM_UCM,
											RegGetStr(_REGKEY_EPVERSION, DEVNM_UCM, "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, DEVNM_UCM, "000000").GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnUCM_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Deinitialize()");

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 열기
int	CDevCmn::fnUCM_OpenShutter()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_OpenShutter()");

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmOpenShutter(UcmRecvTime);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_OpenShutter():return(%d)", nReturn);
	return nReturn;
}

// 닫기
int	CDevCmn::fnUCM_CloseShutter()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_CloseShutter()");

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmCloseShutter(UcmRecvTime);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_CloseShutter():return(%d)", nReturn);
	return nReturn;
}

// 투입허가(투입대기시간)및열기
int	CDevCmn::fnUCM_Accept(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Accept(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmAccept(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Accept(...):return(%d)", nReturn);
	return nReturn;
}

// 투입금지및닫기
int	CDevCmn::fnUCM_CancelAccept()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_CancelAccept()");

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmCancelAccept();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_CancelAccept():return(%d)", nReturn);
	return nReturn;
}

// 읽기
int	CDevCmn::fnUCM_Read()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Read()");

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmRead();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Read():return(%d)", nReturn);
	return nReturn;
}

// 임시보관(입금배서자료)
int	CDevCmn::fnUCM_Escrow(LPCTSTR szPrintData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Stack(szPrintData[%s])", szPrintData);

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmEscrow(szPrintData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Stack(...):return(%d)", nReturn);
	return nReturn;
}

// 수납
int	CDevCmn::fnUCM_Stack()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Store()");

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmStack();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Store():return(%d)", nReturn);
	return nReturn;
}

// 낱장수표반환(수취대기시간)
int	CDevCmn::fnUCM_RejectA(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_RejectA(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmRejectA(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_RejectA(...):return(%d)", nReturn);
	return nReturn;
}

// 전체반환(수취대기시간)
int	CDevCmn::fnUCM_RejectAll(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_RejectAll(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmRejectAll(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_RejectAll(...):return(%d)", nReturn);
	return nReturn;
}

// 수취대기
int	CDevCmn::fnUCM_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_WaitTaken()");

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 수취대기취소
int	CDevCmn::fnUCM_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_CancelWaitTaken()");

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 회수(입출금구분)
int	CDevCmn::fnUCM_Retract(int nDepWithFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Retract(nDepWithFlag[%d])", nDepWithFlag);

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmRetract(nDepWithFlag);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Retract(...):return(%d)", nReturn);
	return nReturn;
}

// 출금계수(수표매수, 출금배서자료)
int	CDevCmn::fnUCM_Dispense(int nCheckCnt, LPCTSTR szPrintData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Dispense(nCheckCnt[%d], szPrintData[%s])", nCheckCnt, szPrintData);

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmDispense(nCheckCnt, szPrintData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Dispense(...):return(%d)", nReturn);
	return nReturn;
}

// 방출(수취대기시간)
int	CDevCmn::fnUCM_Present(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Present(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	UcmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_UCM))
		nReturn = m_pMwi->UcmPresent(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUCM_Present(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//개발4팀 장석원입니다.
//* 입금 불가 조건
//1. 기기가 DOWN일 때
//2. 기기가 정위치가 아닐 때
//3. reject box가 set NG인 경우
//4. reject box 1이 full인 경우 또는 reject box 2가 full인 경우
//5. 현재 reject box 1에 회수되어 있는 회수지폐의 매수와 1회 최대입금매수의 합이 RJB1한도매수를 초과하는 경우
//6. recycle box가 full상태일 때
//7. recycle box내의 지폐매수 + 1회 최대 입금매수가 recycle box 최대 수납 매수보다 클 때
//8. 현재 RJB2의 지폐매수와 1회 최대입금매수를 더하여 RJB2최대매수를 초과할 때
//
//* 만원권만 입금가능
//- 현재 RJB2의 지폐매수와 1회 최대입금매수를 더하여 RJB2한도매수를 초과할 때
//
//*출금 불가 조건
//1. 기기가 DOWN일 때
//2. 기기가 정위치가 아닐 때
//3. reject box가 set NG인 경우
//4. reject box 1이 full인 경우 또는 reject box 2가 full인 경우
//5. 현재 reject box 1에 회수되어 있는 회수지폐의 매수와 1회 최대방출매수의 합이
//   RJB1한도매수를 초과하는 경우
//6. reject box 2에 회수되어 있는 RJB2한도매수를 초과하는 경우
//7. recycle box의 상태를 조사하여 전부 empty상태일 경우
//8. recycle box내의 지폐의 매수가 (1회 최대 방출매수 + 만원권잔량예비매수)미만일 경우
// 
//* Device.ini  설명
//[BRM]
//CrntCheckNo=(12345693) : 수표 번호(사용안함)
//CycleMode=0 : 환류(0)/비환류(1) 선택(사용안함)
//StackOfCSM=0 : CSM 재고 매수
//StackOfNSTK=0 : NSTK 재고 매수
//StackOfCST=0 : CST 재고 매수
//StackOfRB1=110 : RB1 재고 매수
//StackOfRB2=120 : RB2 재고 매수
//StackOfRB3=130 : RB3 재고 매수
//StackOfRJB1=0 : RJB1 재고 매수
//StackOfRJB2=0 : RJB2 재고 매수
//StackOfRJSD=0 : RJSD 재고 매수
//StackOfKBOX=0 : KBOX 재고 매수
//StackOfManwon=0 : 만원권 매수 합계(사용안함)
//StackOfChonwon=0 : 천원권 매수 합계(사용안함)
//SliceLevel= : Sensor Slice Level값
//LowLimitOfCIU=50 : 잔량소/요보충 검지용 CIU 재고 매수
//LowLimitOfRB1=110 : 잔량소/요보충 검지용 RB1 재고 매수
//LowLimitOfRB2=120 : 잔량소/요보충 검지용 RB2 재고 매수
//LowLimitOfRB3=130 : 잔량소/요보충 검지용 RB3 재고 매수
//FullLimitOfCIU=1000 : 만배 검지용 CIU 재고 매수
//FullLimitOfRB1=110 : 만배 검지용 RB1 재고 매수
//FullLimitOfRB2=120 : 만배 검지용 RB2 재고 매수
//FullLimitOfRB3=130 : 만배 검지용 RB3 재고 매수
//FullLimitOfRJB1=217 : RJB1 최대 매수(사용안함)
//FullLimitOfRJB2=500 : RJB2 최대 매수(사용안함)
//CrntCheckCnt=100 : 수표 재고 매수(사용안함)
//MaxCashOutCnt=70 : 1회 최대 현금 출금 매수
//MaxCheckOutCnt=20 : 1회 최대 수표 출금 매수(사용안함)
//PrintData=(2002 08 16) : 수표 발행 날짜 정보(사용안함)
//MaxCashInCnt=70 : 1회 최대 현금 입금 매수
//DistinctionMode=255 : 감별 모드(254:진권, 255:테스트권)
//DistinctionMethod=255 : 감별 방법(사용안함)
//DepositRetryCount=0 : 입금 재시도 횟수(사용안함)
//CrntCashCnt=0 : 지폐 재고 매수(사용안함)
//현금박스최대매수=2500
//램프정보= : Illustration Lamp 정보
//RJB1최대매수=150 : RJB1의 최대 수납 매수		(히다치권고:150=>입출금불가)
//RJB1한도매수=150 : RJB1의 한도 매수			(사용안함)
//RJB2최대매수=300 : RJB2의 최대 수납 매수		(히다치권고:300=>입출금불가)	
//RJB2한도매수=250 : RJB2의 한도 매수			(히다치권고:250=>입금만원권만,계원경고)
//만원권잔량예비매수=20 : 만원권 잔량 예비 매수
//ShutterSpeed=고속 : 셔터 속도(고속/중속/저속)
//현금부족알림매수=300 : 현금 부족 알림 매수(GetAutoloadCondition과 관련)
//현금회수알림매수=300 : 현금 회수 알림 매수(GetAutoloadCondition과 관련)
//현금자동보충매수=500 : 현금 회수 알림 매수(GetAutoloadCondition과 관련)
//현금자동회수매수=500 : 현금 회수 알림 매수(GetAutoloadCondition과 관련)
//현금잔량소매수=400 : 현금 잔량소 매수
/////////////////////////////////////////////////////////////////////////////
//	현금입출금부함수(BRM) : BRM SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnBRM_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

int CDevCmn::fnBRM_SetCarrySpeed(BOOL bSpeed)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_BrmSetCarrySpeed(%s)", (bSpeed == TRUE)? "TRUE" : "FALSE");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmSetCarrySpeed(bSpeed);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_BrmSetCarrySpeed():return(%d)", nReturn);
	return nReturn;
}

// T2ATM 지원 2006.07.29 mwseo ------------------------------------------------
//////////////////////////////////////////////////////////////////////////
// for T2ATM : 카세트(RB)별 권종설정 2006.07.31
int CDevCmn::fnBRM_SetNoteInfo(short nNoteKind1, short nNoteKind2, short nNoteKind3, short nNoteKind4)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_SetNoteInfo(0x%02X,0x%02X,0x%02X,0x%02X)", 
													nNoteKind1, nNoteKind2, nNoteKind3, nNoteKind4);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmSetNoteInfo(nNoteKind1, nNoteKind2, nNoteKind3, nNoteKind4);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_SetNoteInfo(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	현금입출금부함수(BRM) : BRM GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrBRM_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_BRM))
		strReturn = m_pMwi->BrmGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치센서구하기
CString CDevCmn::fstrBRM_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_BRM))
		strReturn = m_pMwi->BrmGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnBRM_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 정위치구하기
int	CDevCmn::fnBRM_GetPosition()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetPosition()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetPosition();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetPosition():return(%d)", nReturn);
}
	return nReturn;
}

// 탈착정보구하기
int	CDevCmn::fnBRM_GetChangeUnitLock()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetChangeUnitLock()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetChangeUnitLock();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetChangeUnitLock():return(%d)", nReturn);
}
	return nReturn;
}

// 계원카세트상태구하기
int	CDevCmn::fnBRM_GetClerkCSTStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetClerkCSTStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetClerkCSTStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetClerkCSTStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 회수카세트상태구하기
int	CDevCmn::fnBRM_GetRejectCSTStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRejectCSTStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetRejectCSTStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRejectCSTStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 현금카세트상태구하기
int	CDevCmn::fnBRM_GetRecycleBoxStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRecycleBoxStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetRecycleBoxStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRecycleBoxStatus():return(%d)", nReturn);
}
	return nReturn;
}

// V07-00-00-#01 오만원권 지원 ────────────────────────
// 현금카세트상태구하기(5만원권)
int	CDevCmn::fnBRM_GetRecycleBox50000Status()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRecycleBox50000Status()");
	
	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;
	
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetRecycleBox50000Status();
	
if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetRecycleBox50000Status():return(%d)", nReturn);
}
	return nReturn;
}
// ──────────────────────────────────────


// 현금매수구하기
int	CDevCmn::fnBRM_GetNumberOfCash()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfCash()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetNumberOfCash();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOfCash():return(%d)", nReturn);
}
	return nReturn;
}


int CDevCmn::fnBRM_GetNumberOf50000Cash()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOf50000Cash()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetNumberOf50000Cash();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNumberOf50000Cash():return(%d)", nReturn);
}
	return nReturn;
}

// V06-01-01-#18-start
int	CDevCmn::fnBRM_GetClerkRJBStatus()
{
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetClerkRJBStatus()");
	
	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;
	

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetClerkRJBStatus();

	if (nReturn != nSaveReturn)
	{
		nSaveReturn = nReturn;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetClerkRJBStatus():return(%d)", nReturn);
	}
	return nReturn;
}

// 감별부 H/W 및 F/W 변경유무
int CDevCmn::fnBRM_GetLargeMoneySupport()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetLargeMoneySupport()");
	
	int		nReturn = FALSE;
	
	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetLargeMoneySupport();
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetLargeMoneySupport(...):return(%d)", nReturn);
	return nReturn;
}
// ──────────────────────────────────────


// 자동보충및자동회수정보구하기
int	CDevCmn::fnBRM_GetAutoloadCondition()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAutoloadCondition()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetAutoloadCondition();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAutoloadCondition():return(%d)", nReturn);
}
	return nReturn;
}

// 매체잔류정보구하기
int	CDevCmn::fnBRM_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// 입금가능상태구하기
int	CDevCmn::fnBRM_GetAvailDeposit()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAvailDeposit()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetAvailDeposit();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAvailDeposit():return(%d)", nReturn);
}

	return nReturn;
}

// 출금가능상태구하기
int	CDevCmn::fnBRM_GetAvailWithdraw()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAvailWithdraw()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetAvailWithdraw();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAvailWithdraw():return(%d)", nReturn);
}
	return nReturn;
}

// 투입및입금계수상태
int	CDevCmn::fnBRM_GetAcceptCountStatus()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAcceptCountStatus()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetAcceptCountStatus();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetAcceptCountStatus():return(%d)", nReturn);
	return nReturn;
}

// 입금계수결과구하기
CString CDevCmn::fstrBRM_GetCountResult()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetCountResult()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_BRM))
		strReturn = m_pMwi->BrmGetCountResult();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetCountResult():return(%s)", strReturn);
	return strReturn;
}

// 회수결과구하기
CString CDevCmn::fstrBRM_GetRetractResult()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetRetractResult()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_BRM))
		strReturn = m_pMwi->BrmGetRetractResult();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetRetractResult():return(%s)", strReturn);
	return strReturn;
}

// 보충결과구하기
CString CDevCmn::fstrBRM_GetRefillResult()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetRefillResult()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_BRM))
		strReturn = m_pMwi->BrmGetRefillResult();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetRefillResult():return(%s)", strReturn);
	return strReturn;
}

// 회수결과구하기
CString CDevCmn::fstrBRM_GetTakeupResult()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetTakeupResult()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_BRM))
		strReturn = m_pMwi->BrmGetTakeupResult();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetTakeupResult():return(%s)", strReturn);
	return strReturn;
}

// 진/위권 모드 조회	// T2ATM 2006.5.24 yscho
CString CDevCmn::fstrBRM_GetBCDistinctionMode()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetBCDistinctionMode()");

	static CString strSaveReturn = "";
	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_BRM))
		strReturn = m_pMwi->BrmGetBCDistinctionMode();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBRM_GetBCDistinctionMode():return(%s)", strReturn);
}

	return strReturn;
}

// T2ATM 지원 2006.07.29 mwseo ------------------------------------------------
//////////////////////////////////////////////////////////////////////////
// for T2ATM : 현금 투입전 지폐 상태 확인 2006.07.31
int CDevCmn::fnBRM_GetCSMNoteStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSMNoteStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetCSMNoteStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetCSMNoteStatus():return(%d)", nReturn);
}
	return nReturn;
}

//////////////////////////////////////////////////////////////////////////
// for T2ATM : 구만/신만 우선 순위 조회 2006.07.31
int CDevCmn::fnBRM_Get10000NoteUsePriority()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_Get10000NoteUsePriority()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGet10000NoteUsePriority();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Get10000NoteUsePriority():return(0x%02X)", nReturn);
}
	return nReturn;
}


// for T2ATM : 권종 조회(RB1) 2006.07.31
int CDevCmn::fnBRM_GetNoteKindOfRB1()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNoteKindOfRB1()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetNoteKindOfRB1();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNoteKindOfRB1():return(0x%02X)", nReturn);
}
	return nReturn;
}

//////////////////////////////////////////////////////////////////////////
// for T2ATM : 권종 조회(RB2) 2006.07.31
int CDevCmn::fnBRM_GetNoteKindOfRB2()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNoteKindOfRB2()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetNoteKindOfRB2();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNoteKindOfRB2():return(0x%02X)", nReturn);
}
	return nReturn;
}

//////////////////////////////////////////////////////////////////////////
// for T2ATM : 권종 조회(RB3) 2006.07.31
int CDevCmn::fnBRM_GetNoteKindOfRB3()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNoteKindOfRB3()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetNoteKindOfRB3();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNoteKindOfRB3():return(0x%02X)", nReturn);
}
	return nReturn;
}

//////////////////////////////////////////////////////////////////////////
// for T2ATM : 권종 조회(RB4) 2006.07.31
int CDevCmn::fnBRM_GetNoteKindOfRB4()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNoteKindOfRB4()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetNoteKindOfRB4();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetNoteKindOfRB4():return(0x%02X)", nReturn);
}
	return nReturn;
}
//-----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//	현금입출금부함수(BRM) : BRM OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnBRM_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Initialize()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
	{
		nReturn = m_pMwi->BrmInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											DEVNM_BRM,
											RegGetStr(_REGKEY_EPVERSION, "BRM_EP", "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, DEVNM_BRM, "000000").GetBuffer(0));
	}
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[BRM      : INIT ]"));	

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnBRM_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Deinitialize()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 열기
int	CDevCmn::fnBRM_OpenShutter()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_OpenShutter()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmOpenShutter();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_OpenShutter():return(%d)", nReturn);
	return nReturn;
}

// 닫기
int	CDevCmn::fnBRM_CloseShutter()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CloseShutter()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmCloseShutter();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CloseShutter():return(%d)", nReturn);
	return nReturn;
}

// 입출금준비해제및객교대복구
int	CDevCmn::fnBRM_Ready()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_Ready()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmReady();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBRM_Ready():return(%d)", nReturn);
	return nReturn;
}

// 입금준비
int	CDevCmn::fnBRM_ReadyCount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ReadyCount()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmReadyCount();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ReadyCount():return(%d)", nReturn);
	return nReturn;
}

// 투입(입금가능현금매수)
int	CDevCmn::fnBRM_Accept(int nCashCnt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Accept(nCashCnt[%d])", nCashCnt);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmAccept(nCashCnt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Accept(...):return(%d)", nReturn);
	return nReturn;
}

// V06-01-01-#01
int	CDevCmn::fnBRM_AcceptLarge(int nCashCnt, int nAmount)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_AcceptLarge(nCashCnt[%d], nAmount[%d])", nCashCnt, nAmount);
	
	int		nReturn = FALSE;
	
	BrmRecvTime = MAX_DEVRSP_TIME;								// T2ATM V03-02-02 R3 시간조정(유승현대리)
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmAcceptLarge(nCashCnt, nAmount);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_AcceptLarge(...):return(%d)", nReturn);
	return nReturn;
}

// 입금계수
int	CDevCmn::fnBRM_Count()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Count()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmCount();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Count():return(%d)", nReturn);
	return nReturn;
}

// 수납
int	CDevCmn::fnBRM_Stack()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Store()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmStack();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Store():return(%d)", nReturn);
	return nReturn;
}

// 반환(수취대기시간)
int	CDevCmn::fnBRM_Reject(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Reject(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmReject(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Reject(...):return(%d)", nReturn);
	return nReturn;
}

// 수취전동작대기
int	CDevCmn::fnBRM_WaitLift()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_WaitLift()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmWaitLift();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_WaitLift():return(%d)", nReturn);
	return nReturn;
}

// 수취대기
int	CDevCmn::fnBRM_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_WaitTaken()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 수취대기취소
int	CDevCmn::fnBRM_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CancelWaitTaken()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 회수
int	CDevCmn::fnBRM_Retract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Retract()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmRetract();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Retract():return(%d)", nReturn);
	return nReturn;
}

// 방출(수취대기시간):닫기없음
int	CDevCmn::fnBRM_Present2(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Present2(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmPresent2(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Present2(...):return(%d)", nReturn);
	return nReturn;
}

// 출금준비
int	CDevCmn::fnBRM_ReadyDispense()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ReadyDispense()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmReadyDispense();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ReadyDispense():return(%d)", nReturn);
	return nReturn;
}

// 출금계수(현금매수)
int	CDevCmn::fnBRM_Dispense(int nCashCnt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Dispense(nCashCnt[%d])", nCashCnt);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmDispense(nCashCnt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Dispense(...):return(%d)", nReturn);
	return nReturn;
}

// V06-01-01-#01-start
int CDevCmn::fnBRM_DispenseLarge(int nCash100TCNT, int nCash50TCNT, int nCash10TCNT)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, 
						"fnBRM_DispenseLarge(nCash100TCNT[%d], nCash50TCNT[%d], nCash10TCNT[%d])", 
						nCash100TCNT, nCash50TCNT, nCash10TCNT);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;								// T2ATM V03-02-02 R3 시간조정(유승현대리)
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmDispenseLarge(nCash100TCNT, nCash50TCNT, nCash10TCNT);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_DispenseLarge(...):return(%d)", nReturn);
	return nReturn;
}
// V06-01-01-#01-end

// 방출(수취대기시간)
int	CDevCmn::fnBRM_Present(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Present(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmPresent(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Present(...):return(%d)", nReturn);
	return nReturn;
}

// 계원보충(현금매수)
int	CDevCmn::fnBRM_RefillMoney(int nCashCnt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_RefillMoney(nCashCnt[%d])", nCashCnt);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmRefillMoney(nCashCnt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_RefillMoney(...):return(%d)", nReturn);
	return nReturn;
}

// 계원회수(현금매수)
int	CDevCmn::fnBRM_TakeupMoney(int nCashCnt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_TakeupMoney(nCashCnt[%d])", nCashCnt);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmTakeupMoney(nCashCnt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_TakeupMoney(...):return(%d)", nReturn);
	return nReturn;
}

// 장치리셋(통상,강제)		// T2ATM 2006.5.24 yscho
int CDevCmn::fnBRM_Reset(BOOL bEnforce)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Reset(bEnforce[%d])", bEnforce);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmReset(bEnforce);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Reset(...):return(%d)", nReturn);
	return nReturn;
}

// 만원권 출금 우선권 지정
int CDevCmn::fnBRM_Set10000NoteUsePriority(short n10000NoteKind)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Set10000NoteUsePriority(n10000NoteKind[%d])", n10000NoteKind);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
	{
		m_pMwi->BrmSet10000NoteUsePriority(n10000NoteKind);
		nReturn = TRUE;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_Set10000NoteUsePriority(...):return(%d)", nReturn);
	return nReturn;
}

// 입금 가능 권종 설정
int CDevCmn::fnBRM_EnableDepositNote(short nNoteKind, boolean bEnable)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_EnableDepositNote(nNoteKind[%d], bEnable[%d])", nNoteKind, bEnable);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
	{
		m_pMwi->BrmEnableDepositNote(nNoteKind, bEnable);
		nReturn = TRUE;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_EnableDepositNote(...):return(%d)", nReturn);
	return nReturn;
}

// 입금 가능 권종 정보
int CDevCmn::fnBRM_GetEnabledDepositNote()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetEnabledDepositNote()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetEnabledDepositNote();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_GetEnabledDepositNote(...):return(%d)", nReturn);
	return nReturn;
}

// 만원권 출금 우선권 정보
int CDevCmn::fnBRM_BrmGetLog()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_BrmGetLog()");

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmGetLog();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_BrmGetLog(...):return(%d)", nReturn);
	return nReturn;
}

// U8100 AP 변경내역 #04 - HBRM관련 변경내역
// HBRM의 경우 보충/회수시 READY동작 들어감
int CDevCmn::fnBRM_ReadyStore()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ReadyStore()");
	
	int		nReturn = FALSE;
	
	BrmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_BRM))
		nReturn = m_pMwi->BrmReadyStore();
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_ReadyStore(...):return(%d)", nReturn);
	return nReturn;
}
// nkh end

// BRM 입출금 내역 Clear
int CDevCmn::fnBRM_BrmCashClear(int nFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_BrmCashClear()");

	int		nReturn = FALSE;

	if(m_pProfile->DEVICE.MachineType == U3100K)
		return nReturn;



MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBRM_BrmCashClear(...):return(%d)", nReturn);
	return nReturn;
}


/////////////////////////////////////////////////////////////////////////////
//	CDU FUNCTION(CDU) : CDU SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnCDU_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Clear Dispense Information
int	CDevCmn::fnCDU_ClearDispenseInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_ClearDispenseInfo()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "RejTotCount", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount1", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount1", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount2", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount2", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount3", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount3", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount4", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount4", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "UserCount5", 0);
		nReturn = IniSetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", "CstCount5", 0);
		
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette1", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette2", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette3", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette4", "0 0 0 0 0 0");
		nReturn = IniSetStr(_SP_DISPINFO_INI, "REJECTED_NOTES", "Cassette5", "0 0 0 0 0 0");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_ClearDispenseInfo():return(%d)", nReturn);
	return nReturn;
}

// Set Value Of Cash(Cash Value Data)
int	CDevCmn::fnCDU_SetValueOfCash(LPCTSTR szCashValueData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetValueOfCash(szCashValueData[%s])", szCashValueData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
//		nReturn = m_pMwi->CduSetValueOfCash(szCashValueData);	// 2005.02.24

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetValueOfCash(...):return(%d)", nReturn);
	return nReturn;
}


// Set MinMax Of CST(Min Count Data, Max Count Data)
int	CDevCmn::fnCDU_SetMinMaxCST(LPCTSTR szMinCntData, LPCTSTR szMaxCntData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetMinMaxCST(szMinCntData[%s], szMaxCntData[%s])", szMinCntData, szMaxCntData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduSetMinMaxCST(szMinCntData, szMaxCntData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetMinMaxCST(...):return(%d)", nReturn);
	return nReturn;
}

// Set Number Of Cash(Cash Count Data)
int	CDevCmn::fnCDU_SetNumberOfCash(LPCTSTR szCashCntData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetNumberOfCash(szCashCntData[%s])", szCashCntData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduSetNumberOfCash(szCashCntData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_SetNumberOfCash(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CDU FUNCTION(CDU) : CDU GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrCDU_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->CduGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Sensor Information
CString CDevCmn::fstrCDU_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->CduGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnCDU_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Position
// V03-01-01 CDU의 형태에 따른 장/탈착 정보조회
// * CD/CDP : Cabinet형
// * TTW    : Shield형
int	CDevCmn::fnCDU_GetPosition()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetPosition()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		nReturn = m_pMwi->CduGetPosition(CDUDOOR_CABINET);

	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetPosition():return(%d)", nReturn);
}
	return nReturn;
}

// Get Reject CST Status
int	CDevCmn::fnCDU_GetRejectCSTStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetRejectCSTStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetRejectCSTStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetRejectCSTStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Recycle Box Status
int	CDevCmn::fnCDU_GetRecycleBoxStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetRecycleBoxStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetRecycleBoxStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetRecycleBoxStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Material Information
int	CDevCmn::fnCDU_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// Get Avail Withdraw
int	CDevCmn::fnCDU_GetAvailWithdraw()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetAvailWithdraw()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	int		nCDUCurrency    = FALSE;
	int		nCDUCurrencyKRW = FALSE;
	int		nCDUCurrencyCHK = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		nCDUCurrency = m_pMwi->CduGetAvailWithdraw();

		nReturn = WITH_NOT_AVAILABLE;

//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "nCDUCurrency[%x]", nCDUCurrency);
		if ((nCDUCurrency & 0x0001) == 0x0001)					// 만원 출금가능 확인
			nCDUCurrencyKRW = CASH_TEN_THOUSAND;

		if ((nCDUCurrency & 0x0100) == 0x0100)					// 5만원 출금가능 확인
			nCDUCurrencyKRW += CASH_FIFTY_THOUSAND;
		nCDUCurrencyCHK = ((nCDUCurrency & 0x0010) == 0x0010) ? TRUE : FALSE;

		nCDUCurrencyCHK = ((nCDUCurrency & 0x0010) == 0x0010) ? TRUE : FALSE;	// 수표 출금가능 확인
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "nCDUCurrencyKRW[%d]", nCDUCurrencyKRW);
		switch(nCDUCurrencyKRW)
		{
			case CASH_TEN_THOUSAND :							// 만원 출금가능
				if (nCDUCurrencyCHK)
					nReturn = (WITH_TEN_TH + WITH_CHECK_ONLY);	// (만원+수표) 출금가능 : 6
				else
					nReturn = WITH_TEN_TH;						// : 4
				break;
			case CASH_FIFTY_THOUSAND :							// 5만원 출금가능
				if (nCDUCurrencyCHK)
					nReturn = (WITH_FIFTY_TH + WITH_CHECK_ONLY);// (5만원+수표) 출금가능
				else
					nReturn = WITH_FIFTY_TH;					
				break;
			case (CASH_TEN_THOUSAND+CASH_FIFTY_THOUSAND):		// (만원+5만원) 출금가능
				if (nCDUCurrencyCHK)
					nReturn = WITH_BOTH_AVAILABLE;				// (만원+5만원+수표) 출금가능
				else
					nReturn = WITH_CASH_ONLY;
				break;
			case WITH_NOT_AVAILABLE:							// 현금출금불가
				if (nCDUCurrencyCHK)
					nReturn = WITH_CHECK_ONLY;					// 수표만 출금가능
				else
					nReturn = WITH_NOT_AVAILABLE;				// 현금+수표 출금불가
				break;
			default:
				break;
		}
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetAvailWithdraw():return(%d)", nReturn);
}
	return nReturn;
}

// Get Number Of CST
int	CDevCmn::fnCDU_GetNumberOfCST()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCST()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetNumberOfCST();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCST():return(%d)", nReturn);
}
	return nReturn;
}

// Get CST Status(CST No)
int	CDevCmn::fnCDU_GetCSTStatus(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn = FALSE;
	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetCSTStatus(nCSTNo);

	switch (nCSTNo)
	{
		case CDU_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetCSTStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Value Of Cash(CST No)
int	CDevCmn::fnCDU_GetValueOfCash(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn = FALSE;
	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetValueOfCash(nCSTNo);

	switch (nCSTNo)
	{
		case CDU_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetValueOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Number Of Cash(CST No)
int	CDevCmn::fnCDU_GetNumberOfCash(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn = FALSE;
	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetNumberOfCash(nCSTNo);

	switch (nCSTNo)
	{
		case CDU_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Number Of Set Cash(CST No)
int	CDevCmn::fnCDU_GetNumberOfSetCash(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn = FALSE;
	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduGetNumberOfSetCash(nCSTNo);

	switch (nCSTNo)
	{
		case CDU_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Dispense Of CST(Cash Amount)
CString	CDevCmn::fstrCDU_GetDispenseOfCST(int nCashAmt)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetDispenseOfCST(nCashAmt[%d])", nCashAmt);

	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CSH))
		strReturn = m_pMwi->CduGetDispenseOfCST(nCashAmt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCDU_GetDispenseOfCST(...):return(%s)", strReturn);
	return strReturn;
}

// Get Number Of Dispense(CST No)
int	CDevCmn::fnCDU_GetNumberOfDispense(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfDispense(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		if ((nCSTNo >= CDU_CST_1)	&&
			(nCSTNo <= CDU_CST_4))
		{
			nReturn = IniGetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", 
									GetSprintf("CstCount%d", nCSTNo));
			if (nReturn < 0)
				nReturn = 0;
		}
		else
			nReturn = 0;
	}

	switch (nCSTNo)
	{
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfDispense(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfDispense(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfDispense(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfDispense(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Number Of Reject(CST No)
int	CDevCmn::fnCDU_GetNumberOfReject(int nCSTNo)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfReject(nCSTNo[%d])", nCSTNo);

	static	int		nSaveReturn = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		if ((nCSTNo >= CDU_CST_1)	&&
			(nCSTNo <= CDU_CST_4))
		{
			nReturn = IniGetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", 
									GetSprintf("CstCount%d", nCSTNo))	-
					  IniGetInt(_SP_DISPINFO_INI, "DISPENSED_NOTES", 
									GetSprintf("UserCount%d", nCSTNo));
			if (nReturn < 0)
				nReturn = 0;
		}
		else
			nReturn = 0;
	}

	switch (nCSTNo)
	{
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_GetNumberOfReject(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CDU FUNCTION(CDU) : CDU OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnCDU_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Initialize()");

	int		nReturn = FALSE;

	BrmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		nReturn = m_pMwi->CduInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											DEVNM_CDU,
											RegGetStr(_REGKEY_EPVERSION, "CDM", "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, "CDM", "000000").GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnCDU_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Deinitialize()");

	int		nReturn = FALSE;

	BrmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Open Shutter
int	CDevCmn::fnCDU_OpenShutter()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_OpenShutter()");

	int		nReturn = FALSE;

	BrmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduOpenShutter();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_OpenShutter():return(%d)", nReturn);
	return nReturn;
}

// Close Shutter
int	CDevCmn::fnCDU_CloseShutter()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_CloseShutter()");

	int		nReturn = FALSE;

	BrmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduCloseShutter();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_CloseShutter():return(%d)", nReturn);
	return nReturn;
}

// Wait Taken
int	CDevCmn::fnCDU_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_WaitTaken()");

	int		nReturn = FALSE;

	BrmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Cancel Wait Taken
int	CDevCmn::fnCDU_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_CancelWaitTaken()");

	int		nReturn = FALSE;

	BrmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// Retract
int	CDevCmn::fnCDU_Retract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Retract()");

	int		nReturn = FALSE;

	BrmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduRetract();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Retract():return(%d)", nReturn);
	return nReturn;
}

// V06-01-01-#01-start 각 권종에대한 매수를 인수로 넣어준다
int CDevCmn::fnCDU_DispenseLarge(int nCash50TCNT, int nCash10TCNT, int nCheckCNT)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, 
						"fnCDU_DispenseLarge(nCash50TCNT[%d], nCash10TCNT[%d], nCheckCNT[%d])", 
						nCash50TCNT, nCash10TCNT, nCheckCNT);

	int		nReturn = FALSE;

	BrmRecvTime = MAX_DEVRSP_TIME;								// T2ATM V03-02-02 R3 시간조정(유승현대리)
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduDispenseLarge(nCash50TCNT, nCash10TCNT, nCheckCNT);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_DispenseLarge(...):return(%d)", nReturn);
	return nReturn;
}
// V06-01-01-#01-end

// Dispense(Cash Amount)
int	CDevCmn::fnCDU_Dispense(int nCashAmt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Dispense(nCashAmt[%d])", nCashAmt);

	int		nReturn = FALSE;

	BrmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduDispenseAmount(nCashAmt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Dispense(...):return(%d)", nReturn);
	return nReturn;
}

// Present(Wait Time)
int	CDevCmn::fnCDU_Present(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Present(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	BrmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduPresent(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_Present(...):return(%d)", nReturn);
	return nReturn;
}

// Dispense(Cash Amount)
int	CDevCmn::fnCDU_DispenseCashCheckCount(int nCashCnt, int nCheckCnt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_DispenseCashCheckCount(nCashCnt[%d], nCheckCnt[%d])", nCashCnt, nCheckCnt);

	int		nReturn = FALSE;

	BrmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_CSH))
		nReturn = m_pMwi->CduDispenseCashCheckCount(nCashCnt, nCheckCnt);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCDU_DispenseCashCheckCount(...):return(%d)", nReturn);
	return nReturn;
}


/////////////////////////////////////////////////////////////////////////////
//	통장부함수(PBM) : PBM SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnPBM_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// 회수매수초기화
int	CDevCmn::fnPBM_ClearRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_ClearRetractCnt()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmClearRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_ClearRetractCnt():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	통장부함수(PBM) : PBM GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrPBM_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치센서구하기
CString CDevCmn::fstrPBM_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnPBM_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnPBM_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 매체잔류정보구하기
int	CDevCmn::fnPBM_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnPBM_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// 회수매수구하기
CString CDevCmn::fstrPBM_GetRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetRetractCnt()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetRetractCnt():return(%s)", strReturn);
	return strReturn;
}

// 통장Ms자료구하기
CString CDevCmn::fstrPBM_GetMsData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetMsData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetMsData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetMsData():return(%s)", strReturn);
	return strReturn;
}

// 통장Bar자료구하기
CString CDevCmn::fstrPBM_GetBarData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetBarData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetBarData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetBarData():return(%s)", strReturn);
	return strReturn;
}

// 통장Line자료구하기
CString CDevCmn::fstrPBM_GetLineData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetLineData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetLineData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPBM_GetLineData():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	통장부함수(PBM) : PBM OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnPBM_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Initialize()");

	int		nReturn = FALSE;

	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
	{
		nReturn = m_pMwi->PbmInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											DEVNM_PBM,
											RegGetStr(_REGKEY_EPVERSION, DEVNM_PBM, "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, DEVNM_PBM, "000000").GetBuffer(0));
	}
	
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("PBM      : INIT "));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnPBM_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Deinitialize()");

	int		nReturn = FALSE;

	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 투입허가(투입대기시간)
int	CDevCmn::fnPBM_EntryEnable(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_EntryEnable(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmEntryEnable(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_EntryEnable(...):return(%d)", nReturn);
	return nReturn;
}

// 투입금지
int	CDevCmn::fnPBM_EntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_EntryDisable()");

	int		nReturn = FALSE;

	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmEntryDisable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_EntryDisable():return(%d)", nReturn);
	return nReturn;
}

// 통장Ms읽기
int	CDevCmn::fnPBM_Read()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Read()");

	int		nReturn = FALSE;

	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmRead();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Read():return(%d)", nReturn);
	return nReturn;
}

// 통장BarLine읽기
int	CDevCmn::fnPBM_BarLineRead()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_BarLineRead()");

	int		nReturn = FALSE;

	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmBarLineRead();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_BarLineRead():return(%d)", nReturn);
	return nReturn;
}

// 인자(인자자료)
int	CDevCmn::fnPBM_Print(LPCTSTR szPrintData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Print(szPrintData[%s])", szPrintData);

	int		nReturn = FALSE;

	PbmRecvTime = MAX_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmPrint("",szPrintData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Print(...):return(%d)", nReturn);
	return nReturn;
}

// 방출(수취대기시간)
int	CDevCmn::fnPBM_Eject(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Eject(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmEject(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Eject(...):return(%d)", nReturn);
	return nReturn;
}

// 수취대기
int	CDevCmn::fnPBM_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_WaitTaken()");

	int		nReturn = FALSE;

	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 수취대기취소
int	CDevCmn::fnPBM_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_CancelWaitTaken()");

	int		nReturn = FALSE;

	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 회수
int	CDevCmn::fnPBM_Retract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Retract()");

	int		nReturn = FALSE;

	PbmRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmRetract();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Retract():return(%d)", nReturn);
	return nReturn;
}

// 기록(통장 MS 기록)
int	CDevCmn::fnPBM_MSWrite(LPCTSTR szMSData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Write(szTrackData[%s])", szMSData);

	int		nReturn = FALSE;

	McuRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_PBM))
		nReturn = m_pMwi->PbmMSWrite("",szMSData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_Write(...):return(%d)", nReturn);
	return nReturn;
}

// U8100 AP 변경내역 #07 - IPBM 소모품 잔여정보 지원
CString	CDevCmn::fnPBM_GetInkStatus()
{
	
	CString				strReturn("");
	static CString		strReturnSave("");
	
	PbmRecvTime = MID_DEVRSP_TIME;
	
	// 적용후 1차 수정 (U8100 AP 변경내역 #13) - IPBM INK 상태관련 추가
	if (AtmDefine.PBHandler != I_PBM_SET)
		strReturn = PBM_INK_FULL;
	else
	if (fnAPL_GetDefineDevice(DEV_PBM))
		strReturn = m_pMwi->PbmGetInkStatus();
			
	if(strReturnSave != strReturn)
	{
		strReturnSave = strReturn;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPBM_GetInkStatus():return(%s)", strReturn);
	}
	return strReturn;
}


/////////////////////////////////////////////////////////////////////////////
//	CAMERA함수(CMR) : CMR SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnCMR_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CAMERA함수(CMR) : CMR GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrCMR_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCMR_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_CMR))
		strReturn = m_pMwi->CmrGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCMR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnCMR_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMR_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 얼굴부카메라 장치상태구하기 V03.01.01
int	CDevCmn::fnCMR_GetFaceCameraDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMR_GetFaceCameraDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrGetPersonCameraStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_GetFaceCameraDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 수취구카메라 장치상태구하기 V03.01.01
int	CDevCmn::fnCMR_GetHandCameraDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMR_GetHandCameraDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;


	
	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrGetExitSlotCameraStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_GetHandCameraDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// T3ATM AP 변경내역 #19 - OM에 Room Camera의 상태를 나타내기 위해 필요함
// 후방카메라 장치상태구하기 V03.01.01
int	CDevCmn::fnCMR_GetRoomCameraDeviceStatus()
{
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMR_GetRoomCameraDeviceStatus()");
	
	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;
	
	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrGetRoomCameraStatus();
	
	if (nReturn != nSaveReturn)
	{
		nSaveReturn = nReturn;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_GetRoomCameraDeviceStatus():return(%d)", nReturn);
	}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CAMERA함수(CMR) : CMR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnCMR_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
	{
		nReturn = m_pMwi->CmrInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											"CAM",
											RegGetStr(_REGKEY_EPVERSION, "CAM", "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, "CAM", "000000").GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnCMR_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 얼굴촬영(파일이름)
int	CDevCmn::fnCMR_CaptureFace(LPCTSTR szFileName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_CaptureFace(szFileName[%s])", szFileName);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrCaptureFace(szFileName);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_CaptureFace(...):return(%d)", nReturn);
	return nReturn;
}

// 손촬영(파일이름)
int	CDevCmn::fnCMR_CaptureHand(LPCTSTR szFileName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_CaptureHand(szFileName[%s])", szFileName);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrCaptureHand(szFileName);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_CaptureHand(...):return(%d)", nReturn);
	return nReturn;
}

// U8100 AP 변경내역 #01 - 3Camera, Preview지원
// 후면촬영 Mwi Call 함수
int	CDevCmn::fnCMR_CaptureRoom(LPCTSTR szFileName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_CaptureRoom(szFileName[%s])", szFileName);
	
	int		nReturn = FALSE;
	
	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrCaptureRoom(szFileName);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_CaptureRoom(...):return(%d)", nReturn);
	return nReturn;
}


// V05-06-03-#08:OM Preview Camera
// 카메라 프리뷰 처리(OM)
// U8100 AP 변경내역 #01 - 3Camera, Preview지원
// OM_화면상태가 후면일 경우에만 실행되도록 변경
int CDevCmn::fnCMR_PreviewToOM(BOOL bShow)
{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_PreviewToOM(%d)", bShow);

	int		nReturn = FALSE;
	CString strPreviewInfo("0,0,0,146,108");
	CString	strOmCommand("");

	static BOOL bIsShow = FALSE;

	// V06-02-01-#05:Camera Preview 사양변경 Start
	CString strX		= RegGetStr(_REGKEY_CAMINFO, "PreviewX", "0");
	CString strY		= RegGetStr(_REGKEY_CAMINFO, "PreviewY", "0");
	CString strWidth	= RegGetStr(_REGKEY_CAMINFO, "PreviewWidth", "0");
	CString strHeight	= RegGetStr(_REGKEY_CAMINFO, "PreviewHeight", "0");
	// V06-02-01-#05:Camera Preview 사양변경 End

	if (bShow == bIsShow)
		return FALSE;
	else
		bIsShow = bShow;

	if (bShow == FALSE)
		SetShareData("OM_COMMAND_PREVIEW", "OFF");
	
	strOmCommand = GetShareData("OM_화면상태");					// 커맨드,파라메터...
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_PreviewToOM(%s)(%d)(%d)", strOmCommand, fnAPL_GetDefineDevice(DEV_CMR), m_pMwi->CmrGetPersonCameraStatus());	
	
	CString strTemp = "";
	strTemp.Format("%s,%s,%s,%s", strX, strY, strWidth, strHeight);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_PreviewToOM(%s)", strTemp );
	if ((fnAPL_GetDefineDevice(DEV_CMR))				&&		// 카메라 설치가 되어있고
		(m_pMwi->CmrGetPersonCameraStatus() == NORMAL)	)		// 얼굴카메라가 정상인경우에 처리
	{
//		SetShareData("OM_CMSTATUS", "정상");					// T3ATM AP 변경내역 #19
	}
	else
	{
		bShow = FALSE;
//		SetShareData("OM_CMSTATUS", "에러");					// T3ATM AP 변경내역 #19
	}

	(bShow == TRUE) ? strPreviewInfo = "1" : strPreviewInfo = "0";
	strPreviewInfo += ",";
	// V06-02-01-#05:Camera Preview 사양변경 Start (Position & Size Registry 참조함)
	strPreviewInfo += strX.GetBuffer(0);
	strPreviewInfo += ",";
	strPreviewInfo += strY.GetBuffer(0);
	strPreviewInfo += ",";
	strPreviewInfo += strWidth.GetBuffer(0);
	strPreviewInfo += ",";
	strPreviewInfo += strHeight.GetBuffer(0);
	// V06-02-01-#05:Camera Preview 사양변경 End

	// V06-02-01-#05:Registry 참조하도록 변경
	// V05-07-01-#02:소모품Display를 위한 좌표수정 Start
// 	strPreviewInfo += "593,";									// X Coodinates
// 	strPreviewInfo += "497,";									// Y Coodinates
// 	strPreviewInfo += "168,";									// Width : 고정
// 	strPreviewInfo += "90";										// Height : 고정 V05-07-04-#02
	// V05-07-01-#02:소모품Display를 위한 좌표수정 End

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_CaptureHand(Preview:%s)", strPreviewInfo.GetBuffer(0));
	if (HBRM_SET == AtmDefine.CashHandler)
	{
		if (strOmCommand == "후면")
		{
			if (fnAPL_GetDefineDevice(DEV_CMR))
				nReturn = m_pMwi->CmrCaptureFace(strPreviewInfo);
		}
	}
	else
		nReturn = m_pMwi->CmrCaptureFace(strPreviewInfo);

// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_PreviewToOM(Preview:%d):return(%d)", bShow, nReturn);
	return nReturn;
}
//<---- end of Preview Camera


// U8100 AP 변경내역 #01 - 3Camera, Preview지원
// OM_화면상태가 전면일 경우 Preview를 선택하면 OPL에서 Preview를 보여줌
int CDevCmn::fnCMR_PreviewToFront(BOOL bShow)					//U8100
{
	int		nReturn = FALSE;
	CString strPreviewInfo("0,0,0,0,0");						// V05-04-01-#01, PreviewCamera 최종 적용
	CString	strOmCommand("");
	
	static BOOL bIsShow = FALSE;
	CString strX		= RegGetStr(_REGKEY_CAMINFO, "PreviewX", "0");
	CString strY		= RegGetStr(_REGKEY_CAMINFO, "PreviewY", "0");
	CString strWidth	= RegGetStr(_REGKEY_CAMINFO, "PreviewWidth", "0");
	CString strHeight	= RegGetStr(_REGKEY_CAMINFO, "PreviewHeight", "0");

// 적용후 1차 수정 (U8100 AP 변경내역 #13)
// 후방 Preview는 T3에서만 지원한다 
	if (AtmDefine.CashHandler != HBRM_SET)
		return FALSE;

	if (bShow != bIsShow)			// V05-04-01-#01, PreviewCamera 최종 적용
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_PreviewToFront(%d)", bShow);	
	
	if (bShow == bIsShow)
	 	return FALSE;
	else
		bIsShow = bShow;

	if (bShow == FALSE)
		SetShareData("OM_COMMAND_PREVIEW", "OFF");
	
	strOmCommand = GetShareData("OM_화면상태");					// 커맨드,파라메터...
	
	// U8100 개발관련
	if (strOmCommand == "전면")
	{
		if(OSType == OS_WIN7)									//#N0228 WIN7 지원 (사용X)
			strX.Format("%d", Asc2Int(strX) + 2048);
		else
			strX.Format("%d", Asc2Int(strX) + 1024);
	}
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_PreviewToFront(%s)(%d)(%d)", strOmCommand, fnAPL_GetDefineDevice(DEV_CMR), m_pMwi->CmrGetPersonCameraStatus());	
	
	if ((fnAPL_GetDefineDevice(DEV_CMR))				&&		// 카메라 설치가 되어있고
		(m_pMwi->CmrGetRoomCameraStatus() == NORMAL))			// 후면카메라가 정상인경우에 처리
	{
//		SetShareData("OM_CMSTATUS", "정상");					// T3ATM AP 변경내역 #19
	}
	else
	{
		bShow = FALSE;
//		SetShareData("OM_CMSTATUS", "에러");					// T3ATM AP 변경내역 #19
	}
	// V05-04-01-#01, PreviewCamera 최종 적용
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pMwi->CmrGetRoomCameraStatus(%d)", m_pMwi->CmrGetRoomCameraStatus());	
	
	
	if (strX.GetLength() == 0 ||
		strY.GetLength() == 0 ||
		strWidth.GetLength() == 0 ||
		strHeight.GetLength() == 0)
	{
		strPreviewInfo = "0,0,0,0,0";
	}
	else
	{
		(bShow == TRUE) ? strPreviewInfo = "1" : strPreviewInfo = "0";
		strPreviewInfo += ",";
		strPreviewInfo += strX.GetBuffer(0);		
		strPreviewInfo += ",";
		strPreviewInfo += strY.GetBuffer(0);
		strPreviewInfo += ",";
		strPreviewInfo += strWidth.GetBuffer(0);
		strPreviewInfo += ",";
		strPreviewInfo += strHeight.GetBuffer(0);
		
	}
		
		
	// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_PreviewToOM(Preview:%s)", strPreviewInfo.GetBuffer(0));
	if (strOmCommand == "전면")
	{
		if (fnAPL_GetDefineDevice(DEV_CMR))
			nReturn = m_pMwi->CmrCaptureRoom(strPreviewInfo);    
	}
	// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_PreviewToOM(Preview:%d):return(%d)", bShow, nReturn);
	return nReturn;
}

// U8100-AP변경 #01 - 3Camera, Preview지원
// 거래진입시 LCD광고 영역에 얼굴쪽 Preview를 실시하여 보여준다
// 카메라 프리뷰 처리(LCD)
int CDevCmn::fnCMR_PreviewToAD(BOOL bShow)
{
	int		nReturn = FALSE;
	int		Ret = 0;
	int		nCameraPos = 0;

	int		nIndex = 0;			//#N0228 WIN7 지원
	
	CString strPreviewInfo("0,0,0,0,0");						// V05-04-01-#01, PreviewCamera 최종 적용
	
	static BOOL bIsShow = FALSE;
	CString strX("");
	CString strY("");
	CString strWidth("");
	CString strHeight("");
// 적용후 1차 수정 (U8100 AP 변경내역 #13)
// 광고LCD의 카메라 PreView는 T3ATM에서만 지원한다
	if (m_pDevCmn->AtmDefine.CashHandler != HBRM_SET)
		return FALSE;

	nCameraPos = GetPrivateProfileInt("LCDINFO", "LCDCameraPos", 0, "d:\\ini\\LCDInfo.ini");

	if(OSType == OS_WIN7)   //#N0228 WIN7 지원
		nIndex = 1024;
	else
		nIndex = 2048;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LCDInfo.ini파일 read[%d]", nCameraPos);
	
	if (bShow == TRUE)
	{
		switch (nCameraPos)
		{
			case 1:												
				strX.Format("%d", nIndex + 34);		//#N0228 WIN7 지원
				strY.Format("%d", 70);
				break;
			case 2:												
				strX.Format("%d", nIndex + 34);		//#N0228 WIN7 지원
				strY.Format("%d", 343);
				break;
			case 3:												
				strX.Format("%d", nIndex + 1070);	//#N0228 WIN7 지원
				strY.Format("%d", 70);
				break;
			case 4:													
				strX.Format("%d", nIndex + 1070);	//#N0228 WIN7 지원
				strY.Format("%d", 343);
				break;
			default:
				return TRUE;
				break;
		}
	}
	
	strWidth.Format("%d",  255);
	strHeight.Format("%d",  210);


	if (bShow != bIsShow)										// V05-04-01-#01, PreviewCamera 최종 적용
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_PreviewToOM(%d)", bShow);	
	
	if (bShow == bIsShow)
		return FALSE;
	else
		bIsShow = bShow;
	
	
	if ((fnAPL_GetDefineDevice(DEV_CMR))				&&		// 카메라 설치가 되어있고
		(m_pMwi->CmrGetPersonCameraStatus() == NORMAL))			// 얼굴카메라가 정상인경우에 처리
	{
//		SetShareData("OM_CMSTATUS", "정상");					// T3ATM AP 변경내역 #19
	}
	else
	{
		bShow = FALSE;
//		SetShareData("OM_CMSTATUS", "에러");					// T3ATM AP 변경내역 #19
	}

	// V05-04-01-#01, PreviewCamera 최종 적용
	MsgDump(TRACE_CODE_MODE, "Cmr", __FILE__, __LINE__, "m_pMwi->CmrGetPersonCameraStatus(%d)", m_pMwi->CmrGetPersonCameraStatus());	
	MsgDump(TRACE_CODE_MODE, "Cmr", __FILE__, __LINE__, "m_pMwi->CmrGetRoomCameraStatus(%d)", m_pMwi->CmrGetRoomCameraStatus());	
	
	
	if (strX.GetLength() == 0 ||
		strY.GetLength() == 0 ||
		strWidth.GetLength() == 0 ||
		strHeight.GetLength() == 0)
	{
		strPreviewInfo = "0,0,0,0,0";
	}
	else
	{
		(bShow == TRUE) ? strPreviewInfo = "1" : strPreviewInfo = "0";
		strPreviewInfo += ",";
		strPreviewInfo += strX.GetBuffer(0);		
		strPreviewInfo += ",";
		strPreviewInfo += strY.GetBuffer(0);
		strPreviewInfo += ",";
		strPreviewInfo += strWidth.GetBuffer(0);
		strPreviewInfo += ",";
		strPreviewInfo += strHeight.GetBuffer(0);
		
	}
	
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_PreviewToOM(Preview:%s)", strPreviewInfo.GetBuffer(0));
	
	if (fnAPL_GetDefineDevice(DEV_CMR))
		nReturn = m_pMwi->CmrCaptureFace(strPreviewInfo);
	
	// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMR_PreviewToOM(Preview:%d):return(%d)", bShow, nReturn);
	return nReturn;
}



/////////////////////////////////////////////////////////////////////////////
//	DOOR함수(DOR) : DOR SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnDOR_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DOR))
		nReturn = m_pMwi->DorClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	DOOR함수(DOR) : DOR GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrDOR_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrDOR_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_DOR))
		strReturn = m_pMwi->DorGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrDOR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnDOR_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnDOR_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DOR))
		nReturn = m_pMwi->DorGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 뒷문상태구하기
int	CDevCmn::fnDOR_GetDoorStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnDOR_GetDoorStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DOR))
		nReturn = m_pMwi->DorGetDoorStatus(0);

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_GetDoorStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 뒷문상태구하기(Cabinet Type)
int	CDevCmn::fnDOR_GetCabinetStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnDOR_GetCabinetStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DOR))
		nReturn = m_pMwi->DorGetCabinetStatus();

	if (nReturn != nSaveReturn)
	{
		nSaveReturn = nReturn;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_GetCabinetStatus():return(%d)", nReturn);
	}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	DOOR함수(DOR) : DOR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnDOR_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DOR))
	{
		nReturn = m_pMwi->DorInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", "DOOR", "--.--.--", "--.--.--");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnDOR_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DOR))
		nReturn = m_pMwi->DorDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDOR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	LIGHT함수(LGT) : LGT SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnLGT_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_LGT))
		nReturn = m_pMwi->LgtClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Flicker설정(인덱스,값)
int	CDevCmn::fnLGT_SetFlicker(int nIndex, int nValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_SetFlicker(nIndex[%x], nValue[%x])", nIndex, nValue);

	int		nReturn = FALSE;
	int		nUseValue  = 0x02;

	switch (m_pProfile->DEVICE.MachineType)						// TTW 대응
	{
	case U8100:												// V07-01-01
	case U3100K:
		nIndex &= ~(UCM_FLICKER | KU_PIN_ATM_FLICKER); // 수표부/PINPAD 처리불가 //#0183
		break;
	}

	if (HBRM_SET == AtmDefine.CashHandler)						// U8100 AP 변경내역 #08 - 사용가능램프 변경 
	{															// 사용가능램프를 사용하지 않고 BRM이나 UCM Flicker가 켜질때에 한하여
		if (nIndex & (KU_BRM_ATM_FLICKER | KU_UCM_ATM_FLICKER))	// 셔터위측 램프를 켜주는 것으로 변경
		{
			if (nValue == FLICKER_ON)
				nUseValue = 0x01;
			else
				nUseValue = 0x02;
			
			fnLGT_SetIndicator(nUseValue);
		}
	}

	if (fnAPL_GetDefineDevice(DEV_LGT))
		nReturn = m_pMwi->LgtSetFlicker(nIndex, nValue);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_SetFlicker(...):return(%d)", nReturn);
	return nReturn;
}

// Indicator설정(값)
int	CDevCmn::fnLGT_SetIndicator(int nValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_SetIndicator(nValue[%x])", nValue);

	int		nReturn = FALSE;

	switch (m_pProfile->DEVICE.MachineType)						// TTW 대응
	{
		case U8100:
		if (nValue & 0x02)									// 준비중인경우에는 ALL OFF
				nValue = -2;
			else
			if (nValue & 0x01)									// 사용가능
				nValue = -1;
			else
				nValue = -2;									// 사용가능이 아닌경우 준비중처리   2005.02.02
			break;

		case U3100K:
			break;

	}

	if (fnAPL_GetDefineDevice(DEV_LGT))
		nReturn = m_pMwi->LgtSetIndicator(nValue);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_SetIndicator(0x%2.2X):return(%d)", nValue, nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	LIGHT함수(LGT) : LGT GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrLGT_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrLGT_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_LGT))
		strReturn = m_pMwi->LgtGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrLGT_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnLGT_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnLGT_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_LGT))
		nReturn = m_pMwi->LgtGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	LIGHT함수(LGT) : LGT OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnLGT_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_LGT))
	{
		nReturn = m_pMwi->LgtInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", "LGT", "--.--.--", "--.--.--");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnLGT_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_LGT))
		nReturn = m_pMwi->LgtDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLGT_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SPL함수(SPL) : SPL SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnSPL_ClearErrorCode()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Segment설정(값)
int	CDevCmn::fnSPL_SetSegment(LPCTSTR szValue)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetSegment(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (HBRM_SET == AtmDefine.CashHandler)						// U8100 AP 변경내역 #11 - 기타항목...
		return TRUE;											// T3에서는 SegMent를 사용하지 않음

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetSegment(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetSegment(...):return(%d)", nReturn);
	return nReturn;
}

// Led설정(값)
int	CDevCmn::fnSPL_SetLed(LPCTSTR szValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetLed(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (HBRM_SET == AtmDefine.CashHandler)						// U8100 AP 변경내역 #11 - 기타항목...
		return TRUE;											// T3에서는 SegMent를 사용하지 않음

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetLed(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetLed(...):return(%d)", nReturn);
	return nReturn;
}

// Led설정(인덱스, 값)
int	CDevCmn::fnSPL_SetLedOnOff(int nLedIndex, int bOnOff)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetLedOnOff(nLedIndex[%d], bOnOff[%d])", nLedIndex, bOnOff);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetLedOnOff(nLedIndex, bOnOff);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetLedOnOff(...):return(%d)", nReturn);
	return nReturn;
}

// Rpl설정(값)
int	CDevCmn::fnSPL_SetRpl(LPCTSTR szValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetRpl(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetRpl(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetRpl(...):return(%d)", nReturn);
	return nReturn;
}

// Inform설정(값)
int	CDevCmn::fnSPL_SetInform(LPCTSTR szValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetInform(szValue[%s])", szValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplSetInform(szValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_SetInform(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SPL함수(SPL) : SPL GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrSPL_GetErrorCode()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrJPR_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_SPL))
		strReturn = m_pMwi->SplGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrJPR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnSPL_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 단체테스트키구하기
int	CDevCmn::fnSPL_GetTestKey()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetTestKey()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplGetTestKey();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetTestKey():return(%d)", nReturn);
}
	return nReturn;
}

// 불휘발램로드스위치구하기
int	CDevCmn::fnSPL_GetLoadSwitch()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetLoadSwitch()");
	
	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;
	//VanCD Spec 0616
	switch(m_pProfile->DEVICE.MachineType)
	{
	case U3100K:
		break;
	case U8100:
		if (fnAPL_GetDefineDevice(DEV_SPL))
			nReturn = m_pMwi->SplGetLoadSwitch();
		break;
	default:
		if (fnAPL_GetDefineDevice(DEV_SPL))
			nReturn = m_pMwi->SplGetLoadSwitch();
		break;
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetLoadSwitch():return(%d)", nReturn);
}
	return nReturn;
}

// 호출키구하기
int	CDevCmn::fnSPL_GetCallKey()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetCallKey()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

//	if (fnAPL_GetDefineDevice(DEV_SPL))
//		nReturn = m_pMwi->SplGetCallKey();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetCallKey():return(%d)", nReturn);
}
	return nReturn;
}

// SCDP SPL 키구하기
// V03-01-01 함수추가
int	CDevCmn::fnSPL_GetSplKeyValue()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetSplKeyValue()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

																// V01-00-00-#305
	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplGetKeyValue();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_GetSplKeyValue():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SPL함수(SPL) : SPL OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnSPL_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
	{
		nReturn = m_pMwi->SplInitialize();
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", "SPL", "--.--.--", "--.--.--");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnSPL_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SPL))
		nReturn = m_pMwi->SplDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPL_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	UPS함수(UPS) : UPS SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnUPS_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
		nReturn = m_pMwi->UpsClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	UPS함수(UPS) : UPS GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrUPS_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrUPS_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_UPS))
		strReturn = m_pMwi->UpsGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrUPS_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnUPS_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUPS_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
		nReturn = m_pMwi->UpsGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 전원상태구하기
int	CDevCmn::fnUPS_GetPowerStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnUPS_GetPowerStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
		nReturn = m_pMwi->UpsGetPowerStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_GetPowerStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	UPS함수(UPS) : UPS OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnUPS_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
	{
		nReturn = m_pMwi->UpsInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											DEVNM_UPS,
											RegGetStr(_REGKEY_EPVERSION, DEVNM_UPS, "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, DEVNM_UPS, "000000").GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnUPS_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
		nReturn = m_pMwi->UpsDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 전원Off(종료대기시간)
int	CDevCmn::fnUPS_PowerOff(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_PowerOff(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
		nReturn = m_pMwi->UpsPowerOff(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_PowerOff(...):return(%d)", nReturn);
	return nReturn;
}

// 전원Restart(종료대기시간,시작대기시간)
int	CDevCmn::fnUPS_PowerRestart(int nWaitSec, int nRestartSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_PowerRestart(nWaitSec[%d], nRestartSec[%d])", nWaitSec, nRestartSec);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_UPS))
		nReturn = m_pMwi->UpsPowerRestart(nWaitSec, nRestartSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnUPS_PowerRestart(...):return(%d)", nReturn);
	return nReturn;
}


/////////////////////////////////////////////////////////////////////////////
//	PIN FUNCTION(PIN) : PIN SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnPIN_ClearErrorCode()    
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	PIN FUNCTION(PIN) : PIN GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrPIN_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PIN))
		strReturn = m_pMwi->PinGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnPIN_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnPIN_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Pin Key Data
CString CDevCmn::fstrPIN_GetPinKeyData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetPinKeyData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PIN))
		strReturn = m_pMwi->PinGetPinKeyData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetPinKeyData():return(%s)", strReturn);
	return strReturn;
}

// Get Macing Data
CString CDevCmn::fstrPIN_GetMacingData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetMacingData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_PIN))
		strReturn = m_pMwi->PinGetMacingData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrPIN_GetMacingData():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	PIN FUNCTION(PIN) : PIN OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
/*
int	CDevCmn::fnPIN_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinInitialize();

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("PIN      : INIT "));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_Initialize():return(%d)", nReturn);
	return nReturn;
}
*/

int	CDevCmn::fnPIN_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_Initialize()");

	int		nReturn = FALSE;
	char szUnPack[512];
	BYTE o_MASETER_KEY[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_WORKING_KEY[8] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86};

	if (fnAPL_GetDefineDevice(DEV_PIN))
	{
		nReturn = m_pMwi->PinInitialize();
		//#N0177
		switch(AtmDefine.MachineType)
		{
		case U3100K:
			sprintf(szUnPack,"%16.16s",	IniGetStr(_ATMKEYTABELE_INI, "ENCDATA","VALUE", "B6140B636BBE7A86")); //check sum = 19BD1D
			MakePack(szUnPack, o_WORKING_KEY, 16);

			fnPIN_LoadKey("M1", MakeUnPack(&o_MASETER_KEY[0], DES_LEN));
			Delay_Msg(500);
			fnPIN_LoadKey("M2", MakeUnPack(&o_MASETER_KEY[0], DES_LEN));
			Delay_Msg(500);
			fnPIN_PinLoadEncryptedKey("W1", MakeUnPack(&o_WORKING_KEY[0], DES_LEN), "M1");
			Delay_Msg(500);
			fnPIN_PinLoadEncryptedKey("W2", MakeUnPack(&o_WORKING_KEY[0], DES_LEN), "M1");
			Delay_Msg(500);
			break;
		case U8100:
			break;
		default:
			break;
		}

	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnPIN_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Entry Enable(EnableMode, MinKeyCount, MaxKeyCount, AutoEnd, ActiveKeys, TerminatorKeys, CardData, Timeout)
int	CDevCmn::fnPIN_EntryEnable(int nEnableMode, int nMinKeyCount, int nMaxKeyCount, int bAutoEnd, LPCTSTR szActiveKeys, LPCTSTR szTerminatorKeys, LPCTSTR szCardData, int nTimeout)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_EntryEnable(nEnableMode[%d], nMinKeyCount[%d], nMaxKeyCount[%d], bAutoEnd[%d], szActiveKeys[%s], szTerminatorKeys[%s], szCardData[%s], nTimeout[%d])", nEnableMode, nMinKeyCount, nMaxKeyCount, bAutoEnd, szActiveKeys, szTerminatorKeys, szCardData, nTimeout);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinEntryEnable(nEnableMode, nMinKeyCount, nMaxKeyCount, bAutoEnd, szActiveKeys, szTerminatorKeys, szCardData, nTimeout,"","","");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_EntryEnable(...):return(%d)", nReturn);
	return nReturn;
}

// Entry Disable
int	CDevCmn::fnPIN_EntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_EntryDisable()");

	int		nReturn = FALSE;
	
	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinEntryDisable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_EntryDisable():return(%d)", nReturn);
	return nReturn;
}

// Load IV(Key Name)
int	CDevCmn::fnPIN_LoadIV(LPCTSTR szKeyName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_LoadIV(szKeyName[%s])", szKeyName);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinLoadIV(szKeyName);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_LoadIV(...):return(%d)", nReturn);
	return nReturn;
}

// Load IV(Key Name, Key Value)
int	CDevCmn::fnPIN_LoadKey(LPCTSTR szKeyName, LPCTSTR szKeyValue)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnPIN_LoadKey(szKeyName[%s], szKeyValue[%s])", szKeyName, szKeyValue);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinLoadKey(szKeyName, szKeyValue);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_LoadKey(...):return(%d)", nReturn);
	return nReturn;
}
//#N0177
int	CDevCmn::fnPIN_PinLoadEncryptedKey(LPCTSTR szKeyName, LPCTSTR szKeyValue, LPCTSTR szEncKeyName)
{

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_PinLoadEncryptedKey(szKeyName[%s], szKeyValue[%s], szEncKeyName[%s])", szKeyName, szKeyValue, szEncKeyName);
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinLoadEncryptedKey(szKeyName, szKeyValue, szEncKeyName,"");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_PinLoadEncryptedKey(...):return(%d)", nReturn);
	return nReturn;
}

// Macing Data(Mac Data)
int	CDevCmn::fnPIN_MacingData(LPCTSTR szMacData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_MacingData(szMacData[%s])", szMacData);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_PIN))
		nReturn = m_pMwi->PinMacingData(szMacData,"K1","");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnPIN_MacingData(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	DES함수(DES) : DES SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnDES_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DES))
		nReturn = m_pMwi->DesClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	DES함수(DES) : DES GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrDES_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrDES_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_DES))
		strReturn = m_pMwi->DesGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrDES_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnDES_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnDES_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DES))
		nReturn = m_pMwi->DesGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	DES함수(DES) : DES OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnDES_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DES))
	{
		nReturn = m_pMwi->DesInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											DEVNM_DES,
											RegGetStr(_REGKEY_EPVERSION, DEVNM_DES, "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, DEVNM_DES, "000000").GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnDES_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_DES))
		nReturn = m_pMwi->DesDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// PIN 암호화 : 2003.12.02
// 2004.05.13 teo MemberNumSize추가, Data Unpack처리
int CDevCmn::fnDES_EncryptPin(void* vPinNum, int PinSize, void* vMemberNum, int MemberNumSize)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_EncryptPin(..)");

	int		nReturn = FALSE;
	CString strRData("");
	int		nDataLen = 0;

	memset(DesRPIN, 0, sizeof(DesRPIN));						// Des RPIN
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_EncryptPin(%s)" ,MakeUnPack(vMemberNum, MemberNumSize));
	m_pMwi->DesFunction(NULL, MakeUnPack(vPinNum, PinSize), PinSize, MakeUnPack(vMemberNum, MemberNumSize), 3, m_pProfile->DEVICE.DesHandler);	
																// PIN Encryption
	ULONG nWaitTime = TimerSet(10);
	while (!CheckTimer(nWaitTime))
	{
		Delay_Msg();
		switch(m_pMwi->DesGetDeviceStatus())
		{
		case NORMAL:
			nReturn = TRUE;
			strRData = m_pMwi->DesGetData();
			nDataLen = MakePack(strRData.GetBuffer(0), DesRPIN, __min(sizeof(DesRPIN)*2, strRData.GetLength()));
			// memcpy(DesRPIN, strRData.GetBuffer(0), __min(sizeof(DesRPIN), strRData.GetLength()));
																// 2004.04.08 DES Data Pack
			break;

		case DOING:
			continue;

		case DOWN:
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_EncryptPin_DOWN!!!!" ,MakeUnPack(vMemberNum, MemberNumSize));
		default :
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_STATUS(%d)" ,m_pMwi->DesGetDeviceStatus());
			nReturn = FALSE;
			break;
		}

		if (nDataLen)											// 2004.11.05 while문 Break추가
			break;
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_EncryptPin():return(%s)", m_pMwi->DesGetData());
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_EncryptPin():return(%d)", nReturn);
	return nReturn;
}

// Key Load : 2003.12.02
// 2004.05.13 teo Data Unpack처리
int CDevCmn::fnDES_LoadKey(void* vKeyData, int Param)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_LoadKey(..)");

	int		nReturn = FALSE;
	CString strRData("");	
	int		nDataLen = 0;

	memset(DesRPIN, 0, sizeof(DesRPIN));						// Des RPIN

	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnDES_LoadKey(%s)", MakeUnPack(vKeyData, 16));
	switch (m_pProfile->DEVICE.DesHandler)						// KEY LOAD
	{
	case S_DES_SET:
		m_pMwi->DesFunction(MakeUnPack(vKeyData, 16), NULL, 0, NULL, Param, m_pProfile->DEVICE.DesHandler);
		break;
	case T_DES_SET:
		m_pMwi->DesFunction(MakeUnPack(vKeyData, 32), NULL, 0, NULL, Param, m_pProfile->DEVICE.DesHandler);
		break;
	default:
		return T_OK;
	}

	ULONG nWaitTime = TimerSet(10);
	while (!CheckTimer(nWaitTime))
	{
		Delay_Msg();
		switch(m_pMwi->DesGetDeviceStatus())
		{
		case NORMAL:
			nReturn = TRUE;
			strRData = m_pMwi->DesGetData();
			nDataLen = MakePack(strRData.GetBuffer(0), DesRPIN, __min(sizeof(DesRPIN)*2, strRData.GetLength()));
			// memcpy(DesRPIN, strRData.GetBuffer(0), __min(sizeof(DesRPIN), strRData.GetLength()));
																// 2004.04.08 DES Data Pack
			break;

		case DOING:
			continue;

		case DOWN:
		default :
			nReturn = FALSE;
			break;
		}

		if (nDataLen)											// 2004.11.05 while문 Break추가
			break;
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_LoadKey():return(%d)", nReturn);
	return nReturn;
}

// Des Test : 2003.12.02
int CDevCmn::fnDES_Test()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_Test()");

	CString strRData("");
	int		nDataLen = 0;

	DesBoardStatus = FALSE;										// Des Board Status
	DesITKStatus = FALSE;										// Des ITK Status
	DesTMKStatus = FALSE;										// Des TMK Status
	memset(DesRPIN, 0, sizeof(DesRPIN));						// Des RPIN

	m_pMwi->DesFunction(NULL, NULL, 0, NULL, 4, m_pProfile->DEVICE.DesHandler);
	ULONG nWaitTime = TimerSet(10);
	while (!CheckTimer(nWaitTime))
	{
		Delay_Msg();
		switch(m_pMwi->DesGetDeviceStatus())
		{
		case NORMAL:
			DesBoardStatus = TRUE;
			strRData = m_pMwi->DesGetData();
			nDataLen = MakePack(strRData.GetBuffer(0), DesRPIN, __min(sizeof(DesRPIN)*2, strRData.GetLength()));
			// memcpy(DesRPIN, strRData.GetBuffer(0), __min(sizeof(DesRPIN), strRData.GetLength()));
																// 2004.04.08 DES Data Pack

			if (m_pProfile->DEVICE.DesHandler == D_DES_SET)
			{
				if (memcmp(&DesRPIN[0], "FF", 2) == 0)
					DesITKStatus = TRUE;
				if (memcmp(&DesRPIN[2], "FF", 2) == 0)
					DesTMKStatus = TRUE;
			}
			else
			if (m_pProfile->DEVICE.DesHandler == T_DES_SET)
			{
				if ((memcmp(&DesRPIN[0], "FF", 2) == 0) &&
					(memcmp(&DesRPIN[2], "FF", 2) == 0) &&
					(memcmp(&DesRPIN[4], "FF", 2) == 0))
					DesITKStatus = TRUE;
				if ((memcmp(&DesRPIN[6], "FF", 2) == 0) &&
					(memcmp(&DesRPIN[8], "FF", 2) == 0) &&
					(memcmp(&DesRPIN[10],"FF", 2) == 0))
					DesTMKStatus = TRUE;
			}			
			break;

		case DOING:
			continue;

		case DOWN:
		default :
			break;
		}

		if (nDataLen)											// 2004.11.05 while문 Break추가
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_Test():return(ITK %d, TMK %d)", DesITKStatus, DesTMKStatus);

	return DesITKStatus;
}
// void ENCRYPT(unsigned char *encodedtext, const unsigned char *plaintext, int textlen, unsigned char *deskey);
// void DECRYPT(unsigned char *plaintext, const unsigned char *encodedtext, int textlen, unsigned char *deskey);
//void T_DES_ENCRYPTOR(BYTE* byData, USHORT usDataLen, BYTE* byKey1, BYTE* byKey2, BYTE* byKey3, BYTE* byResult);
//void T_DES_DECRYPTOR(BYTE* byData, USHORT usDataLen, BYTE* byKey1, BYTE* byKey2, BYTE* byKey3, BYTE* byResult);
/******************************************************************************************************************/
// Des Test : 2009.01.15 Kim Gi Jin --> #N0176	
// [S/W DES Logic]
// This logic use when Des B/D disable, it is requested for My Boss(whang in sung) last Week 2008.12.27   
/******************************************************************************************************************/
int CDevCmn::fnDES_SW_EncryptPin(void* vPinNum, int PinSize, void* vMemberNum, int MemberNumSize, void* vWorkingKey)
{

	BYTE szDummy1[17] = "FFFFFFFFFFFFFFFF";
	BYTE szDummy2[17] = "0000";
	BYTE szPinSize[3] = "00";
	BYTE szPinBlock[9] = "        ";
	BYTE szValidData[13] = "            ";					// 암호화에 필요한 유효데이터 12 바이트
	BYTE szXorPindata[17] ="";
	BYTE szPinNum[512];
	BYTE szDummyWork[512] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86} ;
	BYTE szMemberNum[512];
	BYTE o_MASETER_KEY[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_WORKING_KEY[8] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86};
	BYTE o_REAL_KEY[8];
	BYTE o_REAL_KEY_LEFT[8] =  {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_REAL_KEY_RIGHT[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_PINBLOCK[8];
	BYTE e_PINBLOCK[8];

	int	 nPinSize;
	int	 nReturn = FALSE;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnDES_SW_EncryptPin(%s %d %s %d %s)", vPinNum, PinSize, vMemberNum, MemberNumSize, vWorkingKey);


	memset(DesRPIN, 0, sizeof(DesRPIN));						// Des RPIN

	memcpy(szPinNum, vPinNum, PinSize);
	nPinSize = PinSize;

//	Kim.Gi.Jin 0625
//	memcpy(szMemberNum, vMemberNum, MemberNumSize);
	MakePack(vMemberNum, szMemberNum, MemberNumSize);	
	
	memcpy(szDummyWork, vWorkingKey, 16);
	MakePack(szDummyWork, o_WORKING_KEY, sizeof(o_WORKING_KEY)*2);	
	DECRYPT(o_REAL_KEY, o_WORKING_KEY, 8, o_MASETER_KEY); 

//	T_DES_DECRYPTOR(o_WORKING_KEY, DES_LEN, o_REAL_KEY_LEFT, o_REAL_KEY_RIGHT, o_REAL_KEY_LEFT, o_REAL_KEY);
	//#0100 -- 
//	memcpy(o_REAL_KEY_LEFT, o_REAL_KEY, DES_LEN);

	szPinSize[1] |= (UINT)nPinSize;				// SIZE
	memcpy((char *)szDummy1, (char *)szPinSize, 2);
	memcpy((char *)&szDummy1[2], (char *)szPinNum, nPinSize);
    memcpy((char *)&szDummy2[4], (char *)&szMemberNum[3],12);
	fnUTL_PinDataXor(szDummy1, szDummy2, szXorPindata, 16);
	fnUTL_JisPack(szXorPindata,o_PINBLOCK,16);
	ENCRYPT(e_PINBLOCK, o_PINBLOCK, 8, o_REAL_KEY);

	//#0100 -- 
//	T_DES_ENCRYPTOR(o_PINBLOCK, DES_LEN, o_REAL_KEY_LEFT, o_REAL_KEY_RIGHT, o_REAL_KEY_LEFT, e_PINBLOCK);
	
	MakeUnPack(e_PINBLOCK, DesRPIN, sizeof(e_PINBLOCK));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_SW_EncryptPin(%s)", DesRPIN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnDES_SW_EncryptPin():return(%d)", nReturn);

    //PIN -> (:8B3D9B3080A05B6D len:16)	
	return 	nReturn;
}


/////////////////////////////////////////////////////////////////////////////
//	FINGER함수(FNG) : FNG SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnFNG_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_FNG))
		nReturn = m_pMwi->FngClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	FINGER함수(FNG) : FNG GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrFNG_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrFNG_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_FNG))
		strReturn = m_pMwi->FngGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrFNG_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnFNG_GetDeviceStatus(int nFlag)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFNG_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_FNG))
	{
//		nReturn = m_pMwi->FngGetDeviceStatus();		
		if(nFlag)
		{
			nReturn = m_pMwi->FngInitialize();
			FNG_Status = nReturn;
		}
		else 
			nReturn = FNG_Status;

	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 지문자료구하기
CString CDevCmn::fstrFNG_GetData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrFNG_GetData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_FNG))
		strReturn = m_pMwi->FngGetData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrFNG_GetData():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	FINGER함수(FNG) : FNG OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnFNG_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_FNG))
	{
		nReturn = m_pMwi->FngInitialize();

		FNG_Status = nReturn;
		
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											"FINGER",
											"--.--.--",
											RegGetStr(_REGKEY_SPVERSION, "AxFingerPrint", "000000").GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnFNG_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_FNG))
		nReturn = m_pMwi->FngDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 지문읽기
int	CDevCmn::fnFNG_Acquire()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_Acquire()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_FNG))
		nReturn = m_pMwi->FngAcquire();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_Acquire():return(%d)", nReturn);
	return nReturn;
}

// 지문읽기취소
int	CDevCmn::fnFNG_CancelAcquire()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_CancelAcquire()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_FNG))
		nReturn = m_pMwi->FngCancelAcquire();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_CancelAcquire():return(%d)", nReturn);
	return nReturn;
}

// 지문표시하기(표시구분)
int	CDevCmn::fnFNG_ShowData(LPCTSTR szFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_ShowData(szFlag[%s])", szFlag);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_FNG))
		nReturn = m_pMwi->FngShowData(szFlag);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_ShowData(...):return(%d)", nReturn);
	return nReturn;
}

// 지문표시창설정
int	CDevCmn::fnFNG_ShowSize(int nXPos, int nYPos, int nWideSize, int nHeightSize)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_ShowSize(nXPos[%d], nYPos[%d], nWideSize[%d], nHeightSize[%d])", nXPos, nYPos, nWideSize, nHeightSize);

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_FNG))
		nReturn = m_pMwi->FngShowSize(nXPos, nYPos, nWideSize, nHeightSize);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFNG_ShowSize(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	IRDA함수(IRD) : IRD SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnIRD_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pMwi->IrdClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	IRDA함수(IRD) : IRD GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrIRD_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrIRD_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_IRD))
		strReturn = m_pMwi->IrdGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrIRD_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnIRD_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnIRD_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pMwi->IrdGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 적외선자료구하기
CString CDevCmn::fstrIRD_GetData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrIRD_GetData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_IRD))
		strReturn = m_pMwi->IrdGetData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrIRD_GetData():return(%s)", strReturn);
	return strReturn;
}

// 적외선 자료분석
// Harex 통합모듈 적용 : 2004.07.03
int CDevCmn::fnIRD_AnalData(LPCTSTR szData)
{
	BYTE	IRDAUnPackData[BASEBUFFSIZE];
	BYTE	IRDARecvData[BASEBUFFSIZE];							// IRDA수신자료
	int		IRDARecvLen = 0;									// IRDA수신데이타길이
	int		nOffset = 0;										// Data OffSet

	CString strTag("");

	memset(IRDARecvData, 0, sizeof(IRDARecvData));
	memset(IRDAUnPackData, 0, sizeof(IRDAUnPackData));
	memset(&IRDAInfo, 0, sizeof(IRDAInfo));

	memcpy(IRDAUnPackData, szData, __min(sizeof(IRDAUnPackData), strlen(szData)));

	IRDARecvLen = MakePack(IRDAUnPackData, IRDARecvData, __min(sizeof(IRDAUnPackData), strlen(szData)));

	if (USERNAME == USER_DGB)
	{
		BYTE	bTempData[128];
		int		i, j = 0;

		memcpy(&DgbIrdData, IRDARecvData, IRDARecvLen);

		memset(bTempData, 0x20, sizeof(bTempData));
		for (i = 0; i < sizeof(DgbIrdData.IRAccountInfo); i++)
		{
			if ((DgbIrdData.IRAccountInfo[i] == 'D')	||
				(DgbIrdData.IRAccountInfo[i] == 'E')	||
				(DgbIrdData.IRAccountInfo[i] == 'C')	||
				(DgbIrdData.IRAccountInfo[i] == 'B')	)
			{
				bTempData[j] = '=';
				j++;
			}
			else
			if (DgbIrdData.IRAccountInfo[i] == 'F')
				;
			else
			{
				bTempData[j] = DgbIrdData.IRAccountInfo[i];
				j++;
			}
		}

		memcpy(DgbIrdData.IRAccountInfo, bTempData, sizeof(DgbIrdData.IRAccountInfo));

		memset(IRDAISO3Info.ISO3, 0, sizeof(IRDAISO3Info.ISO3));

		char	tmpBuff[128];
		memset(tmpBuff, 0, sizeof(tmpBuff));
		memcpy(tmpBuff, &DgbIrdData.IRAccountInfo[6], 114);

		MakePack(tmpBuff, IRDAISO3Info.ISO3, strlen(tmpBuff), '|', FALSE);
		return T_OK;
	}

	if ((IRDARecvData[0] & 0x01) == 0x01)						// TYPE1 : K-BANK Type
		strTag = "K";
	else
	if ((IRDARecvData[0] & 0x02) == 0x02)						// TYPE2 : M-BANK Type
		strTag = "A";
	else
	if ((IRDARecvData[0] & 0x10) == 0x10)						// 2Track : Credit Card
		strTag = "C";
	else
	if ((IRDARecvData[0] & 0x20) == 0x20)						// 2Track : Check Card
		strTag = "H";
	else
	if ((IRDARecvData[0] & 0x60) == 0x60)						// 2Track : OK-Cashbag Card
		strTag = "O";
	else
	if ((IRDARecvData[0] & 0x70) == 0x70)						// 2Track : SKT Membership Card
		strTag = "M";
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_AnalData : Cannot Alaysis Irda Data");
		return T_OK;
	}

	memcpy(&IRDAInfo.ISODataTag, strTag.GetBuffer(0), 1);		// ISO Data Tag
	
	if (strTag.CompareNoCase("A") == 0)							// 일반(3Track) Data
	{
		int		nLength = 0;
		nOffset = 0;

		//**********************************************************************
		//  Head Data (Type(1) + Length(2) + Mode(1))
		//**********************************************************************
		nOffset += 1;
		nOffset += 2;
		IRDAInfo.IrdaType2Mode = IRDARecvData[nOffset];
		nOffset += 1;
		
		//**********************************************************************
		//  FCI Data
		//**********************************************************************
		nOffset += 2;											// 0x6F 0x28(Tag+FCI Len)

//		memcpy(IRDAInfo.FCI, &IRDARecvData[nOffset], 40);			// FCI복사
		MakeUnPack(&IRDARecvData[nOffset], IRDAInfo.FCI, 40);
		nOffset += 40;

		//**********************************************************************
		//  BasicInfo Part
		//**********************************************************************

		// Cardholder Data Template Tag
		if ( FIC_TAG_CHDATATEMP == IRDARecvData[nOffset] )
		{
			nOffset += 2;										// Tag + Len
		}
		
		// Cardholder Name Tag ----------------------------------------------------
		if ( memcmp( FIC_TAG_CHNAME, &IRDARecvData[nOffset], sizeof(FIC_TAG_CHNAME) ) == 0 )
		{
			nOffset += sizeof(FIC_TAG_CHNAME);
			
			IRDAInfo.CardHolderNameLen = IRDARecvData[nOffset];// 카드 소지자 이름 길이
			nOffset++;
			
			memcpy( IRDAInfo.CardHolderName, &IRDARecvData[nOffset],
					__min(sizeof(IRDAInfo.CardHolderName), IRDAInfo.CardHolderNameLen));
			nOffset += IRDAInfo.CardHolderNameLen;				// 카드 소지자 이름
		}

		// 실명 ID Tag ------------------------------------------------------------
		if ( FIC_TAG_REALNAME == IRDARecvData[nOffset] )
		{
			nOffset++;

			IRDAInfo.RealNameNOLen = IRDARecvData[nOffset];	// 실명번호 길이
			nOffset++;

			IRDAInfo.RealNameType = IRDARecvData[nOffset];		// 실명구분
			nOffset++;
			if (1 <= IRDAInfo.RealNameNOLen)
			{
				memcpy( IRDAInfo.RealNameNO, &IRDARecvData[nOffset],
						__min(sizeof(IRDAInfo.RealNameNO), (IRDAInfo.RealNameNOLen - 1)));
				nOffset += __min(sizeof(IRDAInfo.RealNameNO), (IRDAInfo.RealNameNOLen - 1));					// 실명번호(실명번호 길이는 실명구분 1Bytes를 포함)
			}
		}

		// 만기일 Tag -------------------------------------------------------------
		if ( memcmp( FIC_TAG_EXPIREDAY, &IRDARecvData[nOffset], sizeof(FIC_TAG_EXPIREDAY) ) == 0 )
		{
			nOffset += sizeof(FIC_TAG_EXPIREDAY);

			IRDAInfo.ExpireDayLen = IRDARecvData[nOffset];		// 만기일 길이
			nOffset++;

			MakeUnPack(&IRDARecvData[nOffset], IRDAInfo.ExpireDay, IRDAInfo.ExpireDayLen);
			nOffset += IRDAInfo.ExpireDayLen;					// 만기일(BCD -> String)
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_AnalData(FINANCEIC_PROC_READDUALINFO) 만기일 = %s", IRDAInfo.ExpireDay);

		// 알고리즘 식별코드 Tag --------------------------------------------------
		if ( FIC_TAG_ALGOCODE == IRDARecvData[nOffset] )
		{
			nOffset += sizeof(BYTE) + sizeof(BYTE);				// 알고리즘 식별코드 길이
			
			IRDAInfo.AlgorithmCode = IRDARecvData[nOffset];	// 알고리즘 식별코드
			nOffset++;
		}
		
		// 발행기관 식별코드 Tag(필수) --------------------------------------------
		if ( FIC_TAG_ISSURECODE != IRDARecvData[nOffset] )
		{
			memset(&IRDAInfo, 0x00, sizeof(IRDAInfo));
			return T_ERROR;										// Data Error
		}
		nOffset++;

		IRDAInfo.IssureCodeLen = IRDARecvData[nOffset];		// 발행기관 식별코드 길이
		nOffset++;
		
		MakeUnPack(&IRDARecvData[nOffset], IRDAInfo.IssureCode, IRDAInfo.IssureCodeLen);
		nOffset += IRDAInfo.IssureCodeLen;						// 발행기관 식별코드(BCD -> String)

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_AnalData(FINANCEIC_PROC_READDUALINFO) 발행기관 식별코드 = %s", IRDAInfo.IssureCode);

		// 카드 일련번호 Tag(필수) ------------------------------------------------
		if ( FIC_TAG_CSN != IRDARecvData[nOffset] )
		{
			memset(&IRDAInfo, 0x00, sizeof(IRDAInfo));
			return T_ERROR;										// Data Error
		}
		nOffset += sizeof(BYTE) + sizeof(BYTE);					// 카드 일련번호 길이

		// 카드 일련번호
		memcpy( IRDAInfo.CSN, &IRDARecvData[nOffset], FIC_CSN_LEN );
		MakeUnPack(&IRDARecvData[nOffset], IRDAInfo.szCSN, FIC_CSN_LEN);
		nOffset += FIC_CSN_LEN;
																// 카드 일련번호 for Screen(BCD -> String)
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnIRD_AnalData(FINANCEIC_PROC_READDUALINFO) CSN = %s", IRDAInfo.szCSN);

		//**********************************************************************
		//  AccountInfo Part
		//**********************************************************************

//		// 계좌 정보 Tag(필수)
//		if ( FIC_TAG_ACCOUNTINFO != IRDARecvData[nOffset] )
//		{
//			memset(&IRDAInfo, 0x00, sizeof(IRDAInfo));
//			return T_ERROR;										// Data Error
//		}
		nOffset++;

		if (FIC_ACCOUNTINFO_LEN != IRDARecvData[nOffset] )
		{
			memset(&IRDAInfo2, 0x00, sizeof(IRDAInfo2));
			return T_ERROR;										// Data Error
		}
		nOffset++;

		memcpy( &IRDAISO3Info, &IRDARecvData[nOffset], FIC_ACCOUNTINFO_LEN );
		nOffset += FIC_ACCOUNTINFO_LEN;
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_AnalData::Account", &IRDAISO3Info, FIC_ACCOUNTINFO_LEN);

//		memcpy( IRDAInfo2.CardRandom, &IRDARecvData[nOffset], FIC_RANDOM_LEN );
		MakeUnPack(&IRDARecvData[nOffset], IRDAInfo.CardRandom, FIC_RANDOM_LEN);
		nOffset += FIC_RANDOM_LEN;
		 
//		memcpy( IRDAInfo2.CipherPW, &IRDARecvData[nOffset], FIC_RANDOM_LEN );
		MakeUnPack(&IRDARecvData[nOffset], IRDAInfo.CipherPW, FIC_RANDOM_LEN);
		nOffset += FIC_RANDOM_LEN;

		memcpy( IRDAInfo.CSN, &IRDARecvData[nOffset], FIC_CSN_LEN );
		MakeUnPack(&IRDARecvData[nOffset], IRDAInfo.szCSN, FIC_CSN_LEN);
		nOffset += FIC_CSN_LEN;

//		memcpy( IRDAInfo2.TerminalRandom, &IRDARecvData[nOffset], FIC_RANDOM_LEN );
		MakeUnPack(&IRDARecvData[nOffset], IRDAInfo.TerminalRandom, FIC_RANDOM_LEN);
		nOffset += FIC_RANDOM_LEN;
	
	}
	else
	if (strTag.CompareNoCase("K") == 0)							// 일반(3Track) Data
	{
		memcpy(IRDAInfo1.IRBankCode, &IRDARecvData[nOffset], 4);// 은행코드 4자리
		nOffset += 4;

		IRDAInfo1.IRChipLenInfo = IRDARecvData[nOffset++];		// Chip 길이 정보 1자리
		memcpy(IRDAInfo1.IRAccount, &IRDARecvData[nOffset], 16);// 계좌정보 16자리
		nOffset += 16;
	
		memcpy(IRDAInfo1.IRCSN, &IRDARecvData[nOffset], 16);	// CSN
		nOffset += 16;

		memcpy(IRDAInfo1.IRSubjectCode, &IRDARecvData[nOffset], 2);	// 과목코드
		nOffset += 2;

		IRDAInfo1.IRTagInfo = IRDARecvData[nOffset++];			// TAG 정보

		memcpy(IRDAInfo1.IRSomeInfo, &IRDARecvData[nOffset], 8);// 추가 정보
		nOffset += 8;

		// 적외선 데이터 조립
		IRDAInfo2.TrInfo = ISO3_TRACK;
		memcpy(IRDAInfo2.IRBankCode, IRDAInfo1.IRBankCode, 4);
		IRDAInfo2.IRChipLenInfo = IRDAInfo1.IRChipLenInfo;
		memcpy(IRDAInfo2.IRAccount, IRDAInfo1.IRAccount, 16);
		memcpy(IRDAInfo2.IRCSN, IRDAInfo1.IRCSN, 16);
		memcpy(IRDAInfo2.IRSubjectCode, IRDAInfo1.IRSubjectCode, 2);
		IRDAInfo2.IRTagInfo = IRDAInfo1.IRTagInfo;
		memcpy(IRDAInfo2.IRSomeInfo, IRDAInfo1.IRSomeInfo, 8);

		for (int i = 0; i < 16; i++)
		{
			if (IsNum(&IRDAInfo2.IRAccount[i], 1))
				;
			else
				break;
		}

		char	tmpBuff[81];

		memset(IRDAISO3Info.ISO3, 0, sizeof(IRDAISO3Info.ISO3));

		memset(tmpBuff, 0, sizeof(tmpBuff));
		memcpy(tmpBuff, "B0159003=0000000000000000=", 26);
		memcpy(&tmpBuff[10], IRDAInfo2.IRAccount, i);

		MakePack(tmpBuff, IRDAISO3Info.ISO3, strlen(tmpBuff), '|', FALSE);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_AnalData():3Track Data[%s]", IRDAISO3Info.ISO3);
	}
	//------------------------------------------------------------------------
	else
	if (strTag.CompareNoCase("C") == 0)							// ISO2 Track Data
	{
		memcpy(IRDAISO2Info.ISO2, &IRDARecvData[2], __min(sizeof(IRDAISO2Info.ISO2), IRDARecvData[1]));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_AnalData():2Track Tag[%c], Data[%s]", IRDAInfo.ISODataTag, IRDAISO2Info.ISO2);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_AnalData():return()");
	return T_OK;
}


/////////////////////////////////////////////////////////////////////////////
//	IRDA함수(IRD) : IRD OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnIRD_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_IRD))
	{
		nReturn = m_pMwi->IrdInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											DEVNM_IRD,
											RegGetStr(_REGKEY_EPVERSION, "AxIrda", "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, "AxIrda", "000000").GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnIRD_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pMwi->IrdDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 투입허가
int	CDevCmn::fnIRD_EntryEnable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_EntryEnable()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pMwi->IrdEntryEnable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_EntryEnable(...):return(%d)", nReturn);
	return nReturn;
}

// 투입금지
int	CDevCmn::fnIRD_EntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_EntryDisable()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_IRD))
		nReturn = m_pMwi->IrdEntryDisable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnIRD_EntryDisable():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	BAR함수(BAR) : BAR SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnBAR_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBAR_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BAR))
		nReturn = m_pMwi->BarClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBAR_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	BAR함수(BAR) : BAR GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrBAR_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBAR_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_BAR))
		strReturn = m_pMwi->BarGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBAR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnBAR_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnBAR_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BAR))
		nReturn = m_pMwi->BarGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBAR_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 바코드자료구하기
CString CDevCmn::fstrBAR_GetData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBAR_GetData()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_BAR))
		strReturn = m_pMwi->BarGetData();
	else
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBAR_GetData()-BarGetData호출하지 않음");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrBAR_GetData():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	BAR함수(BAR) : BAR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnBAR_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBAR_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BAR))
	{
		nReturn = m_pMwi->BarInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", "BAR", "--.--.--", "--.--.--");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBAR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnBAR_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBAR_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BAR))
		nReturn = m_pMwi->BarDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBAR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 읽기허가
int CDevCmn::fnBAR_EntryEnable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBAR_EntryEnable()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BAR))
		nReturn = m_pMwi->BarEntryEnable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBAR_EntryEnable():return(%d)", nReturn);
	return nReturn;
}

// 읽기금지
int CDevCmn::fnBAR_EntryDisable()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBAR_EntryDisable()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_BAR))
		nReturn = m_pMwi->BarEntryDisable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnBAR_EntryDisable():return(%d)", nReturn);
	return nReturn;
}


/////////////////////////////////////////////////////////////////////////////
// VFD함수 (VFD)									// T2ATM 2005.5.24 yscho
/////////////////////////////////////////////////////////////////////////////
// VFD SET MODE
int	CDevCmn::fnVFD_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_ClearErrorCode()");
	int nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_ClearErrorCode(...):return(%d)", nReturn);
	return nReturn;
}

// 밝기조절
int	CDevCmn::fnVFD_SetBrightnessLevel(int nLevel)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_SetBrightnessLevel(%d)", nLevel);
	int nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdSetBrightnessLevel(nLevel);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_SetBrightnessLevel(...):return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnVFD_SetDisplayAtPowerOn(int nSMemIndex, int nEMemIndex, int nScrolled, int nInterval)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_SetDisplayAtPowerOn(%d,%d,%d,%d)", nSMemIndex, nEMemIndex, nScrolled, nInterval);
	int nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdSetDisplayAtPowerOn(nSMemIndex, nEMemIndex, nScrolled, nInterval);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_SetDisplayAtPowerOn(...):return(%d)", nReturn);
	return nReturn;
}

// VFD GET MODE
CString CDevCmn::fstrVFD_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrVFD_GetErrorCode()");

	CString strReturn("");

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		strReturn = m_pMwi->VfdGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrVFD_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

int	CDevCmn::fnVFD_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdGetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// VFD OPERATION MODE
// 연결시작및초기화
int	CDevCmn::fnVFD_Initialize(int nPortNum, int nBaudRate)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_Initialize(%d,%d)", nPortNum, nBaudRate);

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdInitialize(nPortNum, nBaudRate);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnVFD_DeInitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_DeInitialize()");

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_DeInitialize():return(%d)", nReturn);
	return nReturn;
}

// 화면지움/설정값초기화
int	CDevCmn::fnVFD_InitializeDisplay()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_InitializeDisplay()");

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdInitializeDisplay();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_InitializeDisplay():return(%d)", nReturn);
	return nReturn;
}

// 화면지움
int	CDevCmn::fnVFD_ClearDisplay()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_ClearDisplay()");

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdClearDisplay();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnVFD_ClearDisplay():return(%d)", nReturn);
	return nReturn;
}

// 화면Scroll
int	CDevCmn::fnVFD_ScrollDisplay(int nShiftNum, int lnRepeatNum, int nInterval)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_ScrollDisplay(%d,%d,%d)", nShiftNum, lnRepeatNum, nInterval);

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdScrollDisplay(nShiftNum, lnRepeatNum, nInterval);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_ScrollDisplay():return(%d)", nReturn);
	return nReturn;
}

// 화면Blink
int	CDevCmn::fnVFD_BlinkDisplay(int nPattern, int nNormalTime, int nReverseTime, int nRepeatNumber)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_BlinkDisplay(%d,%d,%d,%d)", nPattern, nNormalTime, nReverseTime, nRepeatNumber);

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdBlinkDisplay(nPattern, nNormalTime, nReverseTime, nRepeatNumber);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_BlinkDisplay():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnVFD_RealTimeTextDisplay(LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeTextDisplay(%s,%d,%d,%d,%d,%d,%d,%s)", szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName);

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdRealTimeTextDisplay(szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeTextDisplay():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnVFD_RealTimeBitImageDisplay(LPCTSTR szFileName, int nOffsetX, int nOffsetY)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeBitImageDisplay(%s,%d,%d)", szFileName, nOffsetX, nOffsetY);

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdRealTimeBitImageDisplay(szFileName, nOffsetX, nOffsetY);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeBitImageDisplay():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////////////////// 04.11.18 NKB
int	CDevCmn::fnVFD_RealTimeTextBitImageDisplay(LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName, LPCTSTR szFileName, int nImgOffsetX, int nImgOffsetY)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeTextBitImageDisplay(%s,%d,%d,%d,%d,%d,%d,%s,%s,%d,%d)", szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName, szFileName, nImgOffsetX, nImgOffsetY);

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdRealTimeTextBitImageDisplay(szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName, szFileName, nImgOffsetX, nImgOffsetY);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_RealTimeTextBitImageDisplay():return(%d)", nReturn);
	return nReturn;
}
/////////////////////////////////////////////////////////////////////////////////////////// 04.11.18 NKB

int	CDevCmn::fnVFD_FROMTextSave(int nMemIndex, LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBoldStyle, int nItalic, LPCTSTR szFontName)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMTextSave(%d,%s,%d,%d,%d,%d,%d,%d,%s)", nMemIndex, szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName);

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdFROMTextSave(nMemIndex, szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBoldStyle, nItalic, szFontName);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMTextSave():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnVFD_FROMBitImageSave(int nMemIndex, LPCTSTR szFileName, int nOffsetX, int nOffsetY)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMBitImageSave(%d,%s,%d,%d)", nMemIndex, szFileName, nOffsetX, nOffsetY);

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdFROMBitImageSave(nMemIndex, szFileName, nOffsetX, nOffsetY);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMBitImageSave():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnVFD_FROMTextBitImageSave(int nMemIndex, LPCTSTR szTextData, int nOffsetX, int nOffsetY, int nCharSzX, int nCharSzY, int nBolStyle, int nItalic, LPCTSTR szFontName, LPCTSTR szFileName, int nImgOffsetX, int nImgOffsetY)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMTextBitImageSave(%d,%s,%d,%d,%d,%d,%d,%d,%s,%s,%d,%d)", nMemIndex, szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBolStyle, nItalic, szFontName, szFileName, nImgOffsetX, nImgOffsetY);

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdFROMTextBitImageSave(nMemIndex, szTextData, nOffsetX, nOffsetY, nCharSzX, nCharSzY, nBolStyle, nItalic, szFontName, szFileName, nImgOffsetX, nImgOffsetY);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMTextBitImageSave():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnVFD_FROMDisplay(int nMemIndex)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMDisplay(%d)", nMemIndex);

	int		nReturn = FALSE;

	VfdRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_VFD))
		nReturn = m_pMwi->VfdFROMDisplay(nMemIndex);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnVFD_FROMDisplay():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SNS함수(SNS) : for T2ATM Demo 2006.07.25
/////////////////////////////////////////////////////////////////////////////
// SNS SET MODE
int	CDevCmn::fnSNS_ClearOperatorSwitch()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_ClearOperatorSwitch()");
	int nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsClearOperatorSwitch();
	return nReturn;
}

int	CDevCmn::fnSNS_ClearProximity()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_ClearProximity()");
	int nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsClearProximity();

	return nReturn;
}

// SNS GET MODE
int	CDevCmn::fnSNS_GetOperatorSwitch()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSNS_GetOperatorSwitch()");
	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsGetOperatorSwitch();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_GetOperatorSwitch():return(%d)", nReturn);
}

	return nReturn;
}

int	CDevCmn::fnSNS_GetProximity() 
{
	static	int		nSaveReturn = -2;
	int nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsGetProximity();

	if (nReturn != nSaveReturn)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_GetProximity():return(%d -> %d)", nSaveReturn, nReturn);
		nSaveReturn = nReturn;
	}

	return nReturn;
}

// SNS OPERATION MODE
int CDevCmn::fnSNS_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_Initialize()");
	int nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsInitialize();

	return nReturn;
}

int CDevCmn::fnSNS_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_Deinitialize()");
	int nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_SNS))
		nReturn = m_pMwi->SnsDeinitialize();

	return nReturn;
}


//////////////////////////////////////////////////////////////////////////
int CDevCmn::fnSNS_GetEarPhoneStatus()	
{
	static	int		nSaveReturn = FALSE;
	int nReturn = FALSE;
	
	nReturn = m_pMwi->SnsGetEnhancedAudio();
	
	if (nReturn != nSaveReturn)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSNS_GetEarPhoneStatus():return(%d -> %d)", nSaveReturn, nReturn);
		nSaveReturn = nReturn;
	}

	return nReturn;
}

int CDevCmn::fnSNS_ClearPhoneStatus()	
{
	int nReturn = FALSE;

	nReturn = m_pMwi->SnsClearEnhancedAudio();

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	RFID함수(RFD) : for T2ATM Demo 2006.04.20
/////////////////////////////////////////////////////////////////////////////
// RFD SET MODE
int	CDevCmn::fnRFD_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_RFD))
		nReturn = m_pMwi->RfdClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// RFD GET MODE
CString	CDevCmn::fstrRFD_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRFD_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_RFD))
		strReturn = m_pMwi->RfdGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRFD_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

CString	CDevCmn::fstrRFD_GetData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRFD_GetData()");

	CString strReturn("");
	CString strPackData("");

	if (fnAPL_GetDefineDevice(DEV_RFD))
		strReturn = m_pMwi->RfdGetData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrRFD_GetData():return(%s)", strReturn);
	return strReturn;
}

// RFD 자료분석
int CDevCmn::fnRFD_AnalData(LPCTSTR szData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_AnalData(%s)", szData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_AnalData():return()");
	return T_OK;
}

// RFD OPERATION MODE
int	CDevCmn::fnRFD_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Initialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_RFD))
	{
		nReturn = m_pMwi->RfdInitialize();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[VERSION][%s][%s][%s]", 
											DEVNM_RFD,
											RegGetStr(_REGKEY_EPVERSION, "RFD", "000000").GetBuffer(0),
											RegGetStr(_REGKEY_SPVERSION, "RFD", "000000").GetBuffer(0));

		nReturn = fnRFC_Initialize();
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Initialize():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnRFD_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Deinitialize()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_RFD))
		nReturn = m_pMwi->RfdDeInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnRFD_SendData(int nSendCmd, LPCTSTR szSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_SendData()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_RFD))
		nReturn = m_pMwi->RfdSendData(nSendCmd, szSendData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_SendData():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	GIRO함수(GRO) GRO SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnGRO_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_ClearErrorCode()");

	int		nReturn = FALSE;

//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// 회수매수초기화
int	CDevCmn::fnGRO_ClearRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_ClearRetractCnt()");

	int		nReturn = FALSE;

//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroClearRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_ClearRetractCnt():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	GIRO함수(GRO) GRO GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrGRO_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetErrorCode()");

	CString strReturn("");

	strReturn.Empty();
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		strReturn = m_pMwi->GroGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
CString CDevCmn::fstrGRO_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetDeviceStatus()");

	static	CString		strSaveReturn("");
	CString		strReturn("");

//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		strReturn = m_pMwi->GroGetDeviceStatus();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnJPR_GetDeviceStatus():return(%s)", strReturn);
}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetDeviceStatus():return(%s)", strReturn);
	return strReturn;
}

// 매체정보구하기
CString CDevCmn::fstrGRO_GetMediaStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetMediaStatus()");

	CString	strReturn("");

//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		strReturn = m_pMwi->GroGetMediaStatus();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetMediaStatus():return(%s)", strReturn);
	return strReturn;
}

// 매체잔류정보구하기
int	CDevCmn::fnGRO_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnGRO_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	// 2004.10.14 Add CSW
	if (fnAPL_GetDefineDevice(DEV_GRO))
	{
		if ((!fstrGRO_GetMediaStatus().CompareNoCase(GRO_INBOX))		||
			(!fstrGRO_GetMediaStatus().CompareNoCase(GRO_INJAW))		||
			(!fstrGRO_GetMediaStatus().CompareNoCase(GRO_INJAWINBOX))	||
			(!fstrGRO_GetMediaStatus().CompareNoCase(GRO_SCANED))		||			// 05.03.25 LDC_A
			(!fstrGRO_GetMediaStatus().CompareNoCase(GRO_INJAWSCANED))	||			// 05.03.25 LDC_A
			(bEscrowFlg)												)
		{
			nReturn = TRUE;
		}
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_GetMaterialInfo():MediaStatus(%s)", fstrGRO_GetMediaStatus().GetBuffer(0));
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// 회수매수구하기
CString CDevCmn::fstrGRO_GetRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetRetractCnt()");

	CString strReturn("");

//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		strReturn = m_pMwi->GroGetRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetRetractCnt():return(%s)", strReturn);
	return strReturn;
}

// 지로자료구하기
CString CDevCmn::fstrGRO_GetGiroData()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetGiroData()");

	CString	strReturn("");

//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		strReturn = m_pMwi->GroGetGiroData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetGiroData():return(%s)", strReturn);
	return strReturn;
}

// 지로종류구하기
CString CDevCmn::fstrGRO_GetGiroType()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetGiroType()");

	CString	strReturn("");

//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		strReturn = m_pMwi->GroGetGiroType();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrGRO_GetGiroType():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	GIRO함수(GRO) GRO OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnGRO_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Initialize()");

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnGRO_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Deinitialize()");

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 투입허가(투입대기시간)및열기
int	CDevCmn::fnGRO_Accept(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Accept(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroAccept(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Accept(...):return(%d)", nReturn);
	return nReturn;
}

// 투입금지및닫기
int	CDevCmn::fnGRO_CancelAccept()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_CancelAccept()");

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroCancelAccept();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_CancelAccept(...):return(%d)", nReturn);
	return nReturn;
}

// 투입준비
int	CDevCmn::fnGRO_ReadyAccept()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_ReadyAccept()");

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroReadyAccept();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_ReadyAccept(...):return(%d)", nReturn);
	return nReturn;
}

// 읽기(이미지이름)
int	CDevCmn::fnGRO_Read(LPCTSTR szFileName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Read(szFileName[%s])", szFileName);

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroRead(szFileName);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Read(...):return(%d)", nReturn);
	return nReturn;
}

// 2D읽기(이미지이름)
int	CDevCmn::fnGRO_2DRead(LPCTSTR szFileName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_2DRead(szFileName[%s])", szFileName);

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GiroRead2D(szFileName);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_2DRead(...):return(%d)", nReturn);
	return nReturn;
}

// OCR & 2D읽기(이미지이름)
int	CDevCmn::fnGRO_ReadOCRn2D(LPCTSTR szFileName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_ReadOCRn2D(szFileName[%s])", szFileName);

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GiroReadOCRn2D(szFileName);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_ReadOCRn2D(...):return(%d)", nReturn);
	return nReturn;
}

// 2D읽기(이미지이름)
int	CDevCmn::fnGRO_Read2DNotGetImg(LPCTSTR szFileName)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Read2DNotGetImg(szFileName[%s])", szFileName);

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GiroRead2DNotGetImg(szFileName);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Read2DNotGetImg(...):return(%d)", nReturn);
	return nReturn;
}

// 반환
int	CDevCmn::fnGRO_Reject()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Reject()");

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroReject();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Reject(...):return(%d)", nReturn);
	return nReturn;
}

// 전체반환
int	CDevCmn::fnGRO_RejectAll()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_RejectAll()");

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroRejectAll();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_RejectAll(...):return(%d)", nReturn);
	return nReturn;
}

// 임시보관
int	CDevCmn::fnGRO_Escrow()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Escrow()");

	int		nReturn = FALSE;

	// 지로 품질 개선으로 인한 수정
	// RejectAll 또는 Stack 발생 시에 이미 지로용지가 Escrow 된 상태이면 
	// 중복하여 Escrow를 발행하지 않도록 함.
	if (GroEscrowFlg)
		return TRUE;

	GroRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_GRO))
	{
//		nReturn = m_pMwi->GroEscrow();
		GroEscrowFlg = TRUE;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Escrow(...):return(%d)", nReturn);
	return nReturn;
}

// 수납
int	CDevCmn::fnGRO_Stack()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Stack()");

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroStack();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_Stack(...):return(%d)", nReturn);
	return nReturn;
}

// 상태변경허용
int	CDevCmn::fnGRO_EnableStatusEvents()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_EnableStatusEvents()");

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroEnableStatusEvents();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_EnableStatusEvents(...):return(%d)", nReturn);
	return nReturn;
}

// 상태변경불가
int	CDevCmn::fnGRO_DisableStatusEvents()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_DisableStatusEvents()");

	int		nReturn = FALSE;

	GroRecvTime = MID_DEVRSP_TIME;
//	if (fnAPL_GetDefineDevice(DEV_GRO))
//		nReturn = m_pMwi->GroDisableStatusEvents();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnGRO_DisableStatusEvents(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	COMMON함수(CMN) : CMN SET MODE
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	COMMON함수(CMN) : CMN GET MODE
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	COMMON함수(CMN) : CMN OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 동작대기및장애검지(장치Id,체크시간)
CString	CDevCmn::fstrCMN_CheckDeviceAction(int nDevId, int nCheckTime)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrCMN_CheckDeviceAction(nDevId[%x], nCheckTime[%d])", nDevId, nCheckTime);

	CString			strReturn("");
	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler == CDU_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;
	
	if (DeviceAutoOff & nDevId)	
		nDevId &= ~DeviceAutoOff;

	strReturn = m_pMwi->CheckDeviceAction(nDevId, nCheckTime);
if (strReturn != CHK_NORMAL)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrCMN_CheckDeviceAction():return(%s)", strReturn);
}
	return strReturn;
}

// 이벤트종류별검지된장치구하기(장치Id,검색시간,이벤트종류)
int	CDevCmn::fnCMN_ScanDeviceAction(int nDevId, int nScanTime, int nEventKind)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnCMN_ScanDeviceAction(nDevId[%x], nScanTime[%d], nEventKind[%d])", nDevId, nScanTime, nEventKind);

	int		nReturn = FALSE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler == CDU_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;

	nReturn = m_pMwi->ScanDeviceAction(nDevId, nScanTime, nEventKind);

//	if ((AtmDefine.CashHandler == CDU_SET)	&&
//		(nReturn & DEV_CDU))
//		nReturn = (nReturn & (~DEV_CDU)) | DEV_CSH;

if (nReturn)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_ScanDeviceAction(...):return(%d)", nReturn);
}
	return nReturn;
}

// 장애장치구하기(장치Id)
int	CDevCmn::fnCMN_GetErrorDevice(int nDevId)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetErrorDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler == CDU_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;

	nReturn = m_pMwi->GetErrorDevice(nDevId);

	if ((AtmDefine.CashHandler == CDU_SET)	&&
		(nReturn & DEV_CDU))
		nReturn = (nReturn & (~DEV_CDU)) | DEV_CSH;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetErrorDevice(...):return(%d)", nReturn);
	return nReturn;
}

// 시간초과장치구하기(장치Id)
int	CDevCmn::fnCMN_GetTimeoutDevice(int nDevId)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetTimeoutDevice(nDevId[%x])", nDevId);

	int		nReturn = FALSE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if ((AtmDefine.CashHandler == CDU_SET)	&&
		(nDevId & DEV_CSH))
		nDevId = (nDevId & (~DEV_CSH)) | DEV_CDU;

	nReturn = m_pMwi->GetTimeoutDevice(nDevId);

	if ((AtmDefine.CashHandler == CDU_SET)	&&
		(nReturn & DEV_CDU))
		nReturn = (nReturn & (~DEV_CDU)) | DEV_CSH;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetTimeoutDevice(...):return(%d)", nReturn);
	return nReturn;
}

// Kal Trace Backup(파일이름,파일길이)
int	CDevCmn::fnCMN_ProcBackupTrace(LPCTSTR szFileName, int nFileSize)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_ProcBackupKalTrace(szFileName[%s], nFileSize[%d])", szFileName, nFileSize);

	int		nReturn = FALSE;

	nReturn = m_pMwi->ProcBackupTrace(szFileName, nFileSize);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_ProcBackupKalTrace(...):return(%d)", nReturn);
	return nReturn;
}

//////////////////////////////////////////////////////////////////////////
//	A4P프린터부함수 SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnA4P_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_ClearErrorCode()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pClearErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// 회수매수초기화
int	CDevCmn::fnA4P_ClearRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_ClearRetractCnt()");

	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pClearRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_ClearRetractCnt():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	A4P프린터부함수 SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrA4P_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrA4P_GetErrorCode()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_A4P))
		strReturn = m_pMwi->A4pGetErrorCode();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrA4P_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치센서구하기
CString CDevCmn::fstrA4P_GetSensorInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrA4P_GetSensorInfo()");

	static	CString strSaveReturn("");
	CString			strReturn("");

	if (fnAPL_GetDefineDevice(DEV_A4P))
		strReturn = m_pMwi->A4pGetSensorInfo();

if (strReturn != strSaveReturn)
{
	strSaveReturn = strReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrA4P_GetSensorInfo():return(%s)", strReturn);
}
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnA4P_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnA4P_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pGetDeviceStatus();

if (nReturn != nSaveReturn)
{	
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 용지상태구하기
int	CDevCmn::fnA4P_GetPaperStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnA4P_GetPaperStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pGetPaperStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_GetPaperStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 매체잔류정보구하기
int	CDevCmn::fnA4P_GetMaterialInfo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnA4P_GetMaterialInfo()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pGetMaterialInfo();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_GetMaterialInfo():return(%d)", nReturn);
}
	return nReturn;
}

// 회수매수구하기
CString CDevCmn::fstrA4P_GetRetractCnt()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrA4P_GetRetractCnt()");

	CString strReturn("");

	if (fnAPL_GetDefineDevice(DEV_A4P))
		strReturn = m_pMwi->A4pGetRetractCnt();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrA4P_GetRetractCnt():return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	A4P프린터부함수 SET MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnA4P_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_Initialize()");

	int		nReturn = FALSE;

	A4pRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pInitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnA4P_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_Deinitialize()");

	int		nReturn = FALSE;

	A4pRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pDeinitialize();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 인자(인자자료)
int	CDevCmn::fnA4P_Print(LPCTSTR szPrintForm, LPCTSTR szPrintData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_Print(szPrintForm[%s], szPrintData[%s])", szPrintForm, szPrintData);

	int		nReturn = FALSE;

	fnLGT_SetFlicker(A4P_FLICKER, FLICKER_ON);

	A4pRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pPrint(szPrintForm, szPrintData,0);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_Print(...):return(%d)", nReturn);
	return nReturn;
}

// 인자및 방출(수취대기시간)
int	CDevCmn::fnA4P_PrintAndEject(LPCTSTR szPrintForm, LPCTSTR szPrintData, int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_PrintAndEject(szPrintForm[%s], szPrintData[%s], nWaitSec[%d])", szPrintForm, szPrintData, nWaitSec);
	int		nReturn = FALSE;

	A4pRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pPrintAndEject(szPrintForm, szPrintData, nWaitSec,0);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_PrintAndEject(...):return(%d)", nReturn);
	return nReturn;
}

// 방출(수취대기시간)
int	CDevCmn::fnA4P_Eject(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_Eject(nWaitSec[%d])", nWaitSec);

	int		nReturn = FALSE;

	A4pRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pEject(nWaitSec);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_Eject(...):return(%d)", nReturn);
	return nReturn;
}

// 수취대기
int	CDevCmn::fnA4P_WaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_WaitTaken()");

	int		nReturn = FALSE;

	A4pRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_WaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 수취대기취소
int	CDevCmn::fnA4P_CancelWaitTaken()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_CancelWaitTaken()");

	int		nReturn = FALSE;

	A4pRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pCancelWaitTaken();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_CancelWaitTaken():return(%d)", nReturn);
	return nReturn;
}

// 회수
int	CDevCmn::fnA4P_Retract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_Retract()");

	int		nReturn = FALSE;

	A4pRecvTime = MID_DEVRSP_TIME;
	if (fnAPL_GetDefineDevice(DEV_A4P))
		nReturn = m_pMwi->A4pRetract();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnA4P_Retract():return(%d)", nReturn);
	return nReturn;
}

// 기존 T2 Preview에 전면에서 볼수 있는 Preview 동시에 처리함
UINT thrPreviewThread(LPVOID pParam)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "thrPreviewThread() Start!");

	CDevCmn*	m_pDevCmn = (CDevCmn*)pParam;
	CString			strOmCommand("");
	CStringArray	strOmCommandArray;
	static	CString	strOmResult("");

	Delay_Msg(500);
	
	while (TRUE)
	{
		if (m_pDevCmn->fnAPL_GetDefineDevice(DEV_CMR))
		{
			strOmCommand = GetShareData("OM_COMMAND_PREVIEW");
			SplitString(strOmCommand, "||", strOmCommandArray);

			if (strOmCommandArray[0] == "OM_카메라프리뷰")
			{
				if (strOmCommandArray.GetSize() < 2)
				{
					m_pDevCmn->fnCMR_PreviewToOM(FALSE);
					m_pDevCmn->fnCMR_PreviewToFront(FALSE);				// U8100-AP변경
					strOmResult = OM_ERROR;
					strOmResult += "||카메라 프리뷰 처리요청 오류";
				}
				else
				if (strOmCommandArray[1] == "ON")						// PreVeiw Show
				{
					m_pDevCmn->fnCMR_PreviewToOM(TRUE);
					m_pDevCmn->fnCMR_PreviewToFront(TRUE);				// U8100-AP변경
					strOmResult = OM_NORMAL;
				}
				else
				if (strOmCommandArray[1] == "OFF")						// PreView Hide
				{
					m_pDevCmn->fnCMR_PreviewToOM(FALSE);
					m_pDevCmn->fnCMR_PreviewToFront(FALSE);				// U8100-AP변경
					strOmResult = OM_NORMAL;
				}
				else
				{
					m_pDevCmn->fnCMR_PreviewToOM(FALSE);
					m_pDevCmn->fnCMR_PreviewToFront(FALSE);				// U8100-AP변경
					strOmResult = OM_ERROR;
					strOmResult += "||카메라 프리뷰 처리이상";
				}
				SetShareData("OM_RESULT_PREVIEW", strOmResult);
			}	
		}
		else break;
		
		Delay_Msg(500);
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "thrPreviewThread() End!");
	return 0;
}
//////////////////////////////////////////////////////////////////////////


// 모든 AP의 FileVersion을 체크하여 다른경우 장애처리
int CDevCmn::fnAPP_APVersionCheck()
{

	return TRUE;

	CString strVer("0");
	
	strVer = "";	
	strVer = fnUTL_GetFileVer(_WinATM_OCX_PATH);
	
	if(memcmp(VERSION, strVer.GetBuffer(0), 9) != 0)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "WinATM_OCX[%9.9s]", strVer.GetBuffer(0));
		return FALSE;
	}
	
	strVer = "";	
	strVer = fnUTL_GetFileVer(_DevCtrl_DLL_PATH);
	
	if(memcmp(VERSION, strVer.GetBuffer(0), 9) != 0)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DevCtrl_DLL[%9.9s]", strVer.GetBuffer(0));
		return FALSE;
	}
	
	strVer = "";	
	strVer = fnUTL_GetFileVer(_TranCtrl_DLL_PATH);
	
	if(memcmp(VERSION, strVer.GetBuffer(0), 9) != 0)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranCtrl_DLL[%9.9s]", strVer.GetBuffer(0));
		return FALSE;
	}
	
	strVer = "";	
	strVer = fnUTL_GetFileVer(_NetCtrl_DLL_PATH);
	if(memcmp(VERSION, strVer.GetBuffer(0), 9) != 0)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NetCtrl_DLL[%9.9s]", strVer.GetBuffer(0));
		return FALSE;
	}
	
	strVer = "";	
	strVer = fnUTL_GetFileVer(_ProCtrl_DLL_PATH);
	if(memcmp(VERSION, strVer.GetBuffer(0), 9) != 0)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ProCtrl_DLL[%9.9s]", strVer.GetBuffer(0));
		return FALSE;
	}
	
	strVer = "";	
	strVer = fnUTL_GetFileVer(_VolCtrl_DLL_PATH);
	
	if(memcmp(VERSION, strVer.GetBuffer(0), 9) != 0)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "VolCtrl_DLL[%9.9s]", strVer.GetBuffer(0));
		return FALSE;
	}	
	
	return TRUE;
}


// 파일정보얻기
CString	CDevCmn::fnUTL_GetFileVer(LPCTSTR szFile)
{
	CFileVersion fv;
	CString	szReturn("0");
	szReturn = "";
	
	if (fv.Open(szFile))
	{
		szReturn = fv.GetFileVersion();
		fv.Close();
	}
	
	return szReturn.AllocSysString();
}
