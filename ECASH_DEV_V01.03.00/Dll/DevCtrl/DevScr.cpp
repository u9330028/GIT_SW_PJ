/////////////////////////////////////////////////////////////////////////////
//	DevScr.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmnDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////// 한글입력
#ifndef HANINPUT_C
	#include "HanInput.c"
#endif

#ifndef KS_COMBI_CODE_CONVERT_C
	#include "KS_COMBI_Code_Convert.c"
#endif
///////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	SCR함수(SCR) : SCR SET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드초기화
int	CDevCmn::fnSCR_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_ClearErrorCode()");

	int		nReturn = FALSE;

	nReturn = RegSetStr(_REGKEY_DEVERROR, DEVNM_SCR, "");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// 키값초기화
int	CDevCmn::fnSCR_ClearKeyData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_ClearKeyData()");

	int		nReturn = FALSE;

	nReturn = m_pMwi->ScrClearKeyData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_ClearKeyData():return(%d)", nReturn);
	return nReturn;
}

// 정보전달(자료이름,자료값)
int	CDevCmn::fnSCR_SetScreenData(LPCTSTR szSetDataName, LPCTSTR szSetDataValue)
{

	CString strSetDataName(szSetDataName);
	CString strDataSetValue(szSetDataValue);

//#0123
if ((strSetDataName.Find("PIN") >= 0)			||
	(strSetDataName.Find("PASSWORD") >= 0)		||
	(strSetDataName.Find("PASS") >= 0)			||
	(strSetDataName.Find("PWD") >= 0))
{
	if(!strDataSetValue.IsEmpty())
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetData(szSetDataName[%s], szSetDataValue[%s])", szSetDataName, "*");
	else
	{
		strDataSetValue = S_CLEAR;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetData(szSetDataName[%s], szSetDataValue[%s])", szSetDataName, "**");
	}

}
else
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetData(szSetDataName[%s], szSetDataValue[%s])", szSetDataName, szSetDataValue);

	if (strDataSetValue == "인근IBK코너조회")		// #N0172
	{
		bNearBranchSelectBtn = TRUE;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetData(bNearBranchSelectBtn[%d]", bNearBranchSelectBtn);
	}
	int		nReturn = FALSE;



	nReturn = m_pMwi->ScrSetScreenData(strSetDataName, strDataSetValue);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetData(...):return(%d)", nReturn);
	return nReturn;
}

// 출력자료지정(출력자료이름,출력자료값)
int	CDevCmn::fnSCR_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetDisplayData(szSetDisplayDataName[%s], szSetDisplayDataValue[%s])", szSetDisplayDataName, szSetDisplayDataValue);

	int		nReturn = FALSE;

	// 화면에 Display할 내용을 백업하여 음성엔진에 전달할 텍스트를 만들게 함 
	if (EarPhoneInserted)		
	{
		if ((BackDispCnt < 20) && (memcmp(szSetDisplayDataName, SCR_STRING, sizeof(SCR_STRING)-1) == 0))
		{
			PbackDisp[BackDispCnt].Index = TRUE;
			memcpy(PbackDisp[BackDispCnt].DispData, szSetDisplayDataValue,  __min(256, strlen(szSetDisplayDataValue)));  
			BackDispCnt += 1;
		}
	}

	nReturn = m_pMwi->ScrSetDisplayData(szSetDisplayDataName, szSetDisplayDataValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetDisplayData(...):return(%d)", nReturn);
	return nReturn;
}

// 현재언어모드설정
int	CDevCmn::fnSCR_SetCurrentLangMode(int nLangMode)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetCurrentLangMode(nLangMode[%d])", nLangMode);

	int		nReturn = FALSE;

	memset(Scr.MachineFileType, 0x30, sizeof(Scr.MachineFileType));
	Scr.CurrentLangMode = nLangMode;							// 현재언어모드
	//#0111
	switch (m_pProfile->DEVICE.MachineType)					
	{
		case U8100:
			Strcpy(Scr.MachineFileType, M_TYPE_U81);
			break;									
		case U3100K:
			Strcpy(Scr.MachineFileType, M_TYPE_U3K);
			break;
		default:
			Strcpy(Scr.MachineFileType, M_TYPE_000);
			break;
	}
		
	nReturn = nLangMode;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetCurrentLangMode(MachineFileType %s)", Scr.MachineFileType);
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetCurrentLangMode(...):return(%d)", nReturn);
	return nReturn;
}
													
/////////////////////////////////////////////////////////////////////////////
//	SCR함수(SCR) : SCR GET MODE
/////////////////////////////////////////////////////////////////////////////
// 장애코드구하기
CString CDevCmn::fstrSCR_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetErrorCode()");

	CString strReturn("");

	strReturn = RegGetStr(_REGKEY_DEVERROR, DEVNM_SCR, "0000000");		
	strReturn += "0000000";										// 장애코드보정
	strReturn = strReturn.Left(7);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// 장치상태구하기
int	CDevCmn::fnSCR_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	nReturn = NORMAL;

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// 입력값키구하기
CString	CDevCmn::fstrSCR_GetKeyData()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyData()");

	CString strReturn("");

// Start:EarPhone이 인식된 경우 이곳에서 Pin Entyry를 발행함
	if (EarPhoneInserted)		
		fnPIN_EntryEnable(PINMODE_NORMAL, 0, 100, FALSE, PIN_NUMERIC_PAD, "", "",  -1);
// End

	strReturn = m_pMwi->ScrGetKeyData(MID_DEVRSP_TIME);

// Start
// 장애우ATM 관련 변경사항
// 화면에서 TimeOut이 올라오면 전맹인을 위해 한번더 화면을 Display시도함
// fnSCR_SetDisplayData(REPLAY_MODE, "TIMEOVER") 하면 Flash에서 화면 다시 Load함
	if (EarPhoneInserted)		
	{
		// 이어폰이 빠져있을 때 거래취소 처리
		if (fnSNS_GetEarPhoneStatus() == FALSE)
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetData(...):EarPhoneOut!!!");
			fnPIN_EntryDisable();
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyData():return(%s)", S_CANCEL);
			return S_CANCEL;
		}
		else
		if (strReturn == S_TIMEOVER)
		{
			if (fnSCR_GetCurrentScreenNo() == 701)
				return strReturn;

			fnPIN_EntryDisable();
			Delay_Msg(1000);
			fnPIN_EntryEnable(PINMODE_NORMAL, 0, 100, FALSE, PIN_NUMERIC_PAD, "", "",  -1);
			
			fnSCR_SetDisplayData(REPLAY_MODE, "TIMEOVER");

			fnAPL_TTSstop();
			fnAPL_MakeVoicePlay(TRUE);

			strReturn = "";
			strReturn = m_pMwi->ScrGetKeyData(MID_DEVRSP_TIME);
		}
		fnPIN_EntryDisable();
	}

// End
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyData():return(%s)", strReturn);
	return strReturn;
}

// 문자입력키값구하기(보정길이:0=실제길이)
CString	CDevCmn::fstrSCR_GetKeyString(int nInitFlag)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyString(nInitFlag[%d])", nInitFlag);

	CString strReturn("");

	if (nInitFlag)
		fnSCR_ClearKeyData();									// 키값초기화

	strReturn = fstrSCR_GetKeyData();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyString(...):return(%s)", strReturn);
	return strReturn;
}

// 숫자입력키값구하기(보정길이:0=실제길이)
CString	CDevCmn::fstrSCR_GetKeyNumeric(int nRetLength, int nInitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyNumeric(nRetLength[%d], nInitFlag)", nRetLength, nInitFlag);

	CString strReturn("");
	char	szTemp[256];
	CString strTemp("");

	if (nInitFlag)
		fnSCR_ClearKeyData();									// 키값초기화

	strReturn = fstrSCR_GetKeyData();
	if (!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	// 문자는통과
		;
	else
	if (nRetLength > 0)											
	{
		sprintf(szTemp, "%%%d.%dd%%s", nRetLength, nRetLength);	// 입력자료보정
		strTemp.Format(szTemp, 0, strReturn);
		strReturn = strTemp.Right(nRetLength);
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyNumeric(...):return(%s)", strReturn);
	return strReturn;
}

// 화면존재유무구하기(화면번호,언어모드)
CString	CDevCmn::fnSCR_GetCheckScreen(int nScreenNo, int nLangMode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetCheckScreen(nScreenNo[%d], nLangMode[%d])", nScreenNo, nLangMode);

	CString	strReturn("");
	CFileStatus	FileStatus;
	CString strType("");

	strReturn.Empty();										
	/********************************************************************************
	/* ATM Brand Define => Kim.Gi.Jin 2013.12.30
	/* "A"              => 외환 
	/* "B"              => 기업
	/* "C"              => 광주
	/* "D"              => 경남
	/* "E"              => 농협
	/* "F"              => 국민
	/* "G"              => 전북
	/* "H"              => 하나
	/* "I"              => 신한
	/* "J"              => SC제일
	/* "Z"              => 한국전자금융
	*********************************************************************************/ 
	//#0078
	switch (m_pProfile->DEVICE.ETCDevice20)				
	{
		case KEB:											
			strType.Format("%s", "A");							
			break;
		case KIUP:
			strType.Format("%s", "B");	//#0126						
			break;
		case KWANGJU:
			strType.Format("%s", "C");	//#N0181							
			break;
		case KYONGNAM:
			strType.Format("%s", "D");	//#N0192							
			break;
		case KB:
			strType.Format("%s", "F");	//#N0226							
			break;
		case NONGHYUP:
			strType.Format("%s", "T");							
			break;
		case HANA:
			strType.Format("%s", "H");	//#N0214						
			break;
		case SHINHAN:
			strType.Format("%s", "I");	//#N0215						
			break;
		case SC:
			strType.Format("%s", "J");	//#N0219						
			break;
		case CITI:
			strType.Format("%s", "K");	//#N0238						
			break;
		case NICE:
			strType.Format("%s", "Z");	//#N0219						
			break;
		default:
			strType.Format("%s", "T");							
			break;
	}
	
	if (nScreenNo > 0)
	{
		
		if (nLangMode == KOR_MODE)	
		{
			if ((SightHandiCapped) || (EarPhoneInserted))
				strReturn.Format("%s%1.1s%3.3sH%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);
			else
				strReturn.Format("%s%1.1s%3.3sK%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);
		}
		else 
		if (nLangMode == ENG_MODE)	
			strReturn.Format("%s%1.1s%3.3sE%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);
		else 
		if (nLangMode == JPN_MODE)	
			strReturn.Format("%s%1.1s%3.3sJ%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);
		else 
		if (nLangMode == CHN_MODE)	
			strReturn.Format("%s%1.1s%3.3sC%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);
		else
		if (nLangMode == FOR_MODE)
			strReturn.Format("%s%1.1s%3.3sY%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);   //#N0171  해외카드
		else
		if (nLangMode == KEB_MODE)
			strReturn.Format("%s%1.1s%3.3sZ%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);   //#N0174  이지원
		else							
			strReturn.Format("%s%1.1s%3.3sK%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);

		//T로 한번더 검색 -> Find Swf File => 1)AU81001 -> ATYS001 -> TU81001 -> TTYS001
		if (!CFile::GetStatus(strReturn.GetBuffer(0), FileStatus)) // 1. 파일미존재
		{		
			if (nLangMode == KOR_MODE)	
			{
				if ((SightHandiCapped) || (EarPhoneInserted))
					strReturn.Format("%s%1.1s%3.3sH%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
				else
					strReturn.Format("%s%1.1s%3.3sK%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
			}
			else 
			if (nLangMode == ENG_MODE)	
				strReturn.Format("%s%1.1s%3.3sE%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
			else 
			if (nLangMode == JPN_MODE)	
				strReturn.Format("%s%1.1s%3.3sJ%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
			else 
			if (nLangMode == CHN_MODE)	
				strReturn.Format("%s%1.1s%3.3sC%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
			else
			if (nLangMode == FOR_MODE)
				strReturn.Format("%s%1.1s%3.3sY%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);   //#N0171  해외카드
			else
			if (nLangMode == KEB_MODE)
				strReturn.Format("%s%1.1s%3.3sZ%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);   //#N0174  이지원
			else							
				strReturn.Format("%s%1.1s%3.3sK%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
			
			if (!CFile::GetStatus(strReturn.GetBuffer(0), FileStatus)) // 2. 파일미존재
				strReturn = "";										
		}

		//#H0103  // T로 한번더 검색
		if (!CFile::GetStatus(strReturn.GetBuffer(0), FileStatus)) // 1. 파일미존재
		{
			strType.Empty();
			strType.Format("%s", "T");		

			if (nLangMode == KOR_MODE)	
			{
				if ((SightHandiCapped) || (EarPhoneInserted))
					strReturn.Format("%s%1.1s%3.3sH%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);
				else
					strReturn.Format("%s%1.1s%3.3sK%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);
			}
			else 
			if (nLangMode == ENG_MODE)	
				strReturn.Format("%s%1.1s%3.3sE%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);
			else 
			if (nLangMode == JPN_MODE)	
				strReturn.Format("%s%1.1s%3.3sJ%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);
			else 
			if (nLangMode == CHN_MODE)	
				strReturn.Format("%s%1.1s%3.3sC%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);
			else
			if (nLangMode == FOR_MODE)
				strReturn.Format("%s%1.1s%3.3sY%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);   //#N0171  해외카드
			else
			if (nLangMode == KEB_MODE)
				strReturn.Format("%s%1.1s%3.3sZ%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);   //#N0174  이지원
			else							
				strReturn.Format("%s%1.1s%3.3sK%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), Scr.MachineFileType, nScreenNo);
			
			if (!CFile::GetStatus(strReturn.GetBuffer(0), FileStatus)) // 2. 파일미존재
				strReturn = "";										
		}

		if (!CFile::GetStatus(strReturn.GetBuffer(0), FileStatus)) // 1. 파일미존재
		{
			strType.Empty();
			strType.Format("%s", "T");		

			if (nLangMode == KOR_MODE)	
			{
				if ((SightHandiCapped) || (EarPhoneInserted))
					strReturn.Format("%s%1.1s%3.3sH%3.3d.swf",  _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
				else
					strReturn.Format("%s%1.1s%3.3sK%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
			}
			else 
			if (nLangMode == ENG_MODE)	
				strReturn.Format("%s%1.1s%3.3sE%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
			else 
			if (nLangMode == JPN_MODE)	
				strReturn.Format("%s%1.1s%3.3sJ%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
			else 
			if (nLangMode == CHN_MODE)	
				strReturn.Format("%s%1.1s%3.3sC%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
			else
			if (nLangMode == FOR_MODE)
				strReturn.Format("%s%1.1s%3.3sY%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);   //#N0171  해외카드
			else
			if (nLangMode == KEB_MODE)
				strReturn.Format("%s%1.1s%3.3sZ%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);   //#N0174 이지원
			else							
				strReturn.Format("%s%1.1s%3.3sK%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
			
			if (!CFile::GetStatus(strReturn.GetBuffer(0), FileStatus)) // 2. 파일미존재
				strReturn = "";										
		}
	}

	if (strReturn.IsEmpty())
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetCheckScreen File Not Found!!)");
		strType.Empty();
	/********************************************************************************
	/* ATM Brand Define => Kim.Gi.Jin 2013.12.30
	/* "A"              => 외환 
	/* "B"              => 기업
	/* "C"              => 광주
	/* "D"              => 경남
	/* "E"              => 농협
	/* "F"              => 국민
	/* "J"              => SC제일
	/* "Z"              => 한국전자금융
	*********************************************************************************/ 
		//#0078
		switch (m_pProfile->DEVICE.ETCDevice20)				
		{
			case KEB:											
				strType.Format("%s", "A");							
				break;
			case KIUP:
				strType.Format("%s", "B");	//#0126						
				break;
			case KWANGJU:
				strType.Format("%s", "C");	//#N0181							
				break;
			case KYONGNAM:
				strType.Format("%s", "D");	//#N0192							
				break;
			case KB:
				strType.Format("%s", "F");	//#N0226							
				break;
			case NONGHYUP:
				strType.Format("%s", "T");							
				break;
			case HANA:
				strType.Format("%s", "H");	//#N0214						
				break;
			case SHINHAN:
				strType.Format("%s", "I");	//#N0215						
				break;
			case SC:
				strType.Format("%s", "J");	//#N0219						
				break;
			case CITI:
				strType.Format("%s", "K");	//#N0238						
				break;
			case NICE:
				strType.Format("%s", "Z");	//#N0219						
				break;
			default:
				strType.Format("%s", "T");							
				break;
		}

		if (nScreenNo > 0)	
		{
			if (!CFile::GetStatus(strReturn.GetBuffer(0), FileStatus))
			{
				strType.Format("%s", "T");	//#0126	
				if (nLangMode != KOR_MODE)
					strReturn.Format("%s%1.1s%3.3sK%3.3d.swf", _SCR_DIR, strType.GetBuffer(0), USERNAME, nScreenNo);
				else
					strReturn = "";								// 파일미존재
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetCheckScreen(...):return(%s)", strReturn);
	return strReturn;
}

// 현재화면번호구하기
int	CDevCmn::fnSCR_GetCurrentScreenNo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetCurrentScreenNo()");

	int		nReturn = FALSE;

	nReturn = Scr.CurrentScreenNo;								// 현재화면번호

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetCurrentScreenNo():return(%d)", nReturn);
	return nReturn;
}

// 현재언어모드구하기
int	CDevCmn::fnSCR_GetCurrentLangMode()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetCurrentLangMode()");

	int		nReturn = FALSE;

	nReturn = Scr.CurrentLangMode;								// 현재언어모드

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetCurrentLangMode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SCR함수(SCR) : SCR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 연결시작및초기화
int	CDevCmn::fnSCR_Initialize()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_Initialize()");

	int		nReturn = FALSE;

	switch (m_pProfile->DEVICE.MachineType)
	{
	case U8100:													// U8100-AP변경
//		nReturn = fnSCR_SetDisplayData(SCR_KIND, "U8100");
		nReturn = fnSCR_SetDisplayData(SCR_KIND, "T3ATM");
		m_pProfile->GetTransProfile();								
		if (m_pProfile->TRANS.ZoomFlag == ZOOM_SET)
			nReturn = fnSCR_SetDisplayData(SCR_KINDMODE, "ZOOMMODE");
		break;
	case U3100K:
		nReturn = fnSCR_SetDisplayData(SCR_KIND, "T2ATM");
		break;
	}

	// 기종 설정	#N0271
	switch (m_pProfile->DEVICE.MachineType)
	{
	case U8100:
		nReturn = fnSCR_SetDisplayData(SCR_KINDATM, "U8100"); break;
	case U3100K:
		nReturn = fnSCR_SetDisplayData(SCR_KINDATM, "3100K"); break;
	default:
		nReturn = fnSCR_SetDisplayData(SCR_KINDATM, "VANCD"); break;
	}

	// 브랜드 설정	#N0271
	switch (m_pProfile->DEVICE.ETCDevice20)
	{
		case KB:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "KB"); break;
		case KIUP:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "IBK"); break;
		case JEJU:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "JJ"); break;
		case MYASSET:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "TYS"); break;
		case KWANGJU:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "KJ"); break;
		case SC:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "SC"); break;
		case NONGHYUP:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "NACF"); break;
		case POST:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "MIC"); break;
		case KEB:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "KEB"); break;
		case SUHYUP:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "NFFC"); break;
		case CITI:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "CT"); break;
		case HANA:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "HN"); break;
		case KYONGNAM:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "KN"); break;
		case SHINHAN:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "SH"); break;
		default:
			nReturn = fnSCR_SetDisplayData(SCR_KINDBRAND, "IBK"); break;
	}

	nReturn = NORMAL;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// 연결종료
int	CDevCmn::fnSCR_Deinitialize()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_Deinitialize()");

	int		nReturn = FALSE;

	nReturn = NORMAL;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// 화면출력(화면번호,언어모드)
//t int	CDevCmn::fnSCR_DisplayScreen(int nScreenNo, int nDisplaySec)
int	CDevCmn::fnSCR_DisplayScreen(int nScreenNo, int nDisplaySec, int nPinInputMode, LPCTSTR szCardData, int nPinPassWordMin, int nPinPassWordMax, int nPinPassWordAuto, LPCTSTR szPinPassWordTerm)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayScreen(nScreenNo[%d], nDisplaySec[%d], nPinInputMode[%d], szCardData[%s], nPinPassWordMin[%d], nPinPassWordMax[%d], nPinPassWordAuto[%d], szPinPassWordTerm[%s])", 
		nScreenNo, nDisplaySec, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);
	int		nReturn = FALSE;
	CString strTempTopName("");
	CString strGetAtmNum("");

	//#0064 -> 자동기관리번호 (상황실 요청) Kim.Gi.Jin
	if(nScreenNo == 701)
	{
		if (m_pDevCmn->EarPhoneInserted == FALSE)		// 장애인ATM - TTS에는 적용되지 않도록
		{
			strGetAtmNum.Format("%6.6s", &m_pProfile->NETWORK.AtmSerialNum[2]);
			m_pDevCmn->fnSCR_DisplayString(6, strGetAtmNum);
			m_pDevCmn->fnSCR_DisplayString(7, "1577 - 0068");
		}
	}

	CString	strTempCheckName = fnSCR_GetCheckScreen(nScreenNo, Scr.CurrentLangMode);
	
	fnSCR_ClearKeyData();										// 키값초기화

	switch (m_pProfile->DEVICE.MachineType)						// 기종별 PinPad처리수행
	{
		case U8100:											
			if (EarPhoneInserted)
				fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, FALSE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);
			else
				fnAPL_DeviceEnDisable(DEV_PIN, DISABLE);
			break;
		case U3100K:
			fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, FALSE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);
			break;
	}

	if (HostLineMode == HOST_LOCAL_MODE)						// 로컬모드
	{
		if (BrmCashMode == BRM_TESTCASH_MODE)					// 위권로컬모드
			strTempTopName.Format("%sTOP/TOP%d.swf", _SCR_DIR, 3);
		else													// 진권로컬모드
		{
			strTempTopName.Format("%sTOP/TOP%d.swf", _SCR_DIR, 2);
		}
	}
	else														// 호스트모드
	{
		if (BrmCashMode == BRM_TESTCASH_MODE)					// 위권모드
			strTempTopName.Format("%sTOP/TOP%d.swf", _SCR_DIR, 1);
		else													// 진권모드
		{
			if(HostLocalSvrMode == TRUE)
				strTempTopName.Format("%sTOP/TOP%d.swf", _SCR_DIR, 2);
			else 
				strTempTopName = "";
		}
	}

	if (strTempTopName != "")									// Top Image출력
	{
		nReturn = fnSCR_SetDisplayData(SCR_TOP, strTempTopName);
	}

	if (nDisplaySec > 0)										// 화면입력 대기시간출력
	{
		if (EarPhoneInserted)								
		{
			if(nScreenNo == 701)
				nDisplaySec = K_15_WAIT;
			else
				nDisplaySec = K_60_WAIT;
		}
		else
			fnSCR_SetDisplayData(EAR_KINDMODE, "off");

		nReturn = fnSCR_SetDisplayData(SCR_TIME, Int2Asc(nDisplaySec));
	}
	
	Scr.CurrentScreenNo = nScreenNo;							// 그대로 보관바람
	if (strTempCheckName == "")									// 화면미존재시 한글화면구하기
		strTempCheckName = fnSCR_GetCheckScreen(nScreenNo, KOR_MODE);
	if (strTempCheckName != "")									// 화면존재시 거래화면출력
		nReturn = fnSCR_SetDisplayData(SCR_SCREEN, strTempCheckName);

	if (HostLineMode == HOST_LOCAL_MODE)						// 로컬모드
	{	// FLASH 버퍼 Clear대기 시간, 온라인에서는 연속 화면출력 확률이 희박함
		// 향후 FLASH를 수정후 삭제바랍니다
		// 300ms대기함 : 2003.10.10
		// Delay_Msg(300);
	}

// Start
// 장애우ATM 관련 변경사항
// 음성을 읽어주기 전에 화면이 변경되는 것을 방지 특히 안내화면에서 많이 발생함
	if (EarPhoneInserted)		
	{
		fnAPL_TTSstop();
		fnAPL_MakeVoicePlay(FALSE);
	}

	memset(PdisableBtn, 0x01, sizeof(PdisableBtn));
	memset(PbackDisp, 0x00, sizeof(PbackDisp));
	BackDispCnt = 0;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayScreen(...):return(%d)", nReturn);
	return nReturn;
}

// 이미지출력(이미지번호,온오프값)
int	CDevCmn::fnSCR_DisplayImage(int nImageNo, int nOnOffFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayImage(nImageNo[%d], nOnOffFlag[%d])", nImageNo, nOnOffFlag);

	int		nReturn = FALSE;
	CString strTemp("");

	if (nImageNo > 0)
	{
		strTemp.Format("%s%d", SCR_BTNONOFF, nImageNo);
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), ((nOnOffFlag) ? "on" : "off"));

		// Button Endisable 관련 버튼은 MAX 12개로 표현 (PIN버튼(0~9)과 관련 충분함)
		if (nImageNo > 12)									
			;
		else
		{
			if (EarPhoneInserted)
			{
				if (!nOnOffFlag)
					PdisableBtn[nImageNo] = FALSE;
				else
					PdisableBtn[nImageNo] = TRUE;
			}
		}
	}

	fnSCR_ClearKeyData();										// 키값초기화

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayImage(...):return(%d)", nReturn);
	return nReturn;
}

// 문자출력(문자번호,문자,길이)
int	CDevCmn::fnSCR_DisplayString(int nStringNo, LPCTSTR szString, int nInLength, int Flag)
{
	int		nReturn = FALSE;
	char	szTemp[256];
	CString strTemp("");
	CString strTemp2("");

	sprintf(szTemp, "%%%d.%ds", nInLength, nInLength);			// 출력자료보정
	strTemp2.Format(szTemp, szString);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(nStringNo[%d], szString[%s], nInLength[%d])", nStringNo, strTemp2.GetBuffer(0), nInLength);
	if (Flag)
	{
		strTemp.Format("%s", "m_keyValue");
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}
	else
	if ((nStringNo > 0) &&
		(nInLength > 0))
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(...):return(%d)", nReturn);
	return nReturn;
}

// 문자출력(문자번호,문자)
int	CDevCmn::fnSCR_DisplayString(int nStringNo, LPCTSTR szString)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(nStringNo[%d], szString[%s])", nStringNo, szString);

	int		nReturn = FALSE;
	CString strTemp("");

	if ((nStringNo > 0) &&
		(strlen(szString) > 0))
	{
		//#0028 2010.11.15 Kim.Gi.Jin
		if(nStringNo == 1006)
		{
			strTemp.Format("APCenterADMovie");
			nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), szString);
		}
		// 2011.10.25 Yun.H.J 메인광고/인터렉티브/베너 광고 통합
		if(nStringNo == 1010)
		{
			strTemp.Format("%s", "APWithdrawProcess");
			nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), szString);
			return nReturn;
		}
		else
		{
			strTemp.Format("%s%d", SCR_STRING, nStringNo);
			nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), szString);
		}
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(...):return(%d)", nReturn);
	return nReturn;
}
// Display String (String No, String)
int	CDevCmn::fnSCR_DisplayStringArray(int nStringNo, LPCTSTR szString, LPCTSTR szSeperator)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(nStringNo[%d], szString[%s])", nStringNo, szString);

	int		nReturn = FALSE;
	CString strTemp("");
	CStringArray strTempArray;

	if (nStringNo > 0)
	{
		strTemp.Format("%s", szString);							// 2004.11.05
		SplitString(strTemp, szSeperator, strTempArray);				// 2004.11.05
		for (int i = 0; i < strTempArray.GetSize(); i++)
		{
			strTemp.Format("%s%d", SCR_STRING, nStringNo + i);
			nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTempArray[i].GetBuffer(0));
		}
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(...):return(%d)", nReturn);
	return nReturn;
}
// 숫자출력(문자번호,문자,길이)
int	CDevCmn::fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric, int nInLength)
{
	int		nReturn = FALSE;
	char	szTemp[256];
	CString strTemp("");
	CString strTemp2("");

	sprintf(szTemp, "%%%d.%ds", nInLength, nInLength);			// 출력자료보정
	strTemp2.Format(szTemp, szNumeric);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(nStringNo[%d], szNumeric[%s], nInLength[%d])", nStringNo, strTemp2.GetBuffer(0), nInLength);

	if ((nStringNo > 0) &&
		(nInLength > 0))
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);
		strTemp2.Format("%d", Asc2Int(szNumeric, nInLength));
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(...):return(%d)", nReturn);
	return nReturn;
}

// 숫자출력(문자번호,문자)
int	CDevCmn::fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(nStringNo[%d], szNumeric[%s])", nStringNo, szNumeric);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if ((nStringNo > 0) &&
		(strlen(szNumeric) > 0))
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);
		strTemp2.Format("%d", Asc2Int(szNumeric, strlen(szNumeric)));
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(...):return(%d)", nReturn);
	return nReturn;
}

// 숫자출력(문자번호,숫자)
int	CDevCmn::fnSCR_DisplayNumeric(int nStringNo, int nNumeric)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(nStringNo[%d], nNumeric[%d])", nStringNo, nNumeric);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);
		strTemp2.Format("%d", nNumeric);
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(...):return(%d)", nReturn);
	return nReturn;
}

// 금액출력(문자번호,문자,길이,마크,부호)
int	CDevCmn::fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, int nInLength, char cMarkChar, int nIsMinus)
{
	int		nReturn = FALSE;
	char	szTemp[256];
	CString strTemp("");
	CString strTemp2("");

	sprintf(szTemp, "%%%d.%ds", nInLength, nInLength);			// 출력자료보정
	strTemp2.Format(szTemp, szAmount);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(nStringNo[%d], szAmount[%s], nInLength[%d], cMarkChar[%c], nIsMinus[%d])", nStringNo, strTemp2.GetBuffer(0), nInLength, cMarkChar, nIsMinus);

	if ((nStringNo > 0) &&
		(nInLength > 0))
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);

		strTemp2 = Asc2Amt(strTemp2, strTemp2.GetLength(), 256, cMarkChar);
		strTemp2.TrimLeft();

		if (nIsMinus)
			strTemp2 = "-" + strTemp2;							// 2003.11.27

		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(...):return(%d)", nReturn);
	return nReturn;
}

// 금액출력(문자번호,문자,마크,부호)
int	CDevCmn::fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, char cMarkChar, int nIsMinus)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(nStringNo[%d], szAmount[%s], cMarkChar[%c], nIsMinus[%d])", nStringNo, szAmount, cMarkChar, nIsMinus);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if ((nStringNo > 0) &&
		(strlen(szAmount) > 0))
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);

		strTemp2 = szAmount;
		
		strTemp2 = Asc2Amt(strTemp2, strTemp2.GetLength(), 256, cMarkChar);
		strTemp2.TrimLeft();

		if (nIsMinus)
			strTemp2 = "-" + strTemp2;							// 2003.11.27

		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(...):return(%d)", nReturn);
	return nReturn;
}

// 금액출력(문자번호,금액,마크,부호)
int	CDevCmn::fnSCR_DisplayAmount(int nStringNo, int nAmount, char cMarkChar, int nIsMinus)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(nStringNo[%d], nAmount[%d], cMarkChar[%c], nIsMinus[%d])", nStringNo, nAmount, cMarkChar, nIsMinus);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);

		strTemp2.Format("%d", nAmount);

		strTemp2 = Asc2Amt(strTemp2, strTemp2.GetLength(), 256, cMarkChar);
		strTemp2.TrimLeft();

		if (nIsMinus)
			strTemp2 = "-" + strTemp2;							// 2003.11.27

		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(...):return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
// 한글입력 함수 
/////////////////////////////////////////////////////////////////////////////

void CDevCmn::fnSCR_HANGULInit()
{
	HAN_INPUT_KEY_BUFFER_Create(&m_key_buf, 100);
}

CString	CDevCmn::fnSCR_GETKEYHANGUL(int	scrNum,
									int strNum,
									BYTE *keyBuff, 
									int	inLen,
									int LimitTime, 
									int *RetCnt, 
									int SizeMin)
{
	// 한글 입력 상태로 설정
	gHanMode=COMBI_HAN_MODE;

	// 키를 입력받을 버퍼를 클리어
	HAN_INPUT_KEY_BUFFER_Clear(&m_key_buf);
	ZeroMemory(m_text_Buff, 200);

	CString strReturn("");
	int	retch;
	
	int TempLen = 0;

	int nRetryCnt =0;

	if (SizeMin == 0)											// SizeMin지정무(최대입력)
		SizeMin = inLen;

	fnSCR_ClearKeyData();

	while (1)
	{
		strReturn = fstrSCR_GetKeyData();
		retch = atoi((LPCTSTR)strReturn);
		if (strReturn == S_CONFIRM)
		{

			fnSCR_Input2Byte(keyBuff, m_text_Buff, strlen((char*)m_text_Buff));
			
			TempLen = strlen((char*)m_text_Buff);

			if(TempLen == 0)		//#N0201 입력값 없을때. 3회 Retry
			{
				nRetryCnt ++;

				if(nRetryCnt >= 3)
				{
					return S_INPUTOVER;
				}

				if(gHanMode == COMBI_HAN_MODE)
					scrNum = 431;
				else
					scrNum = 432;

				TempLen = strlen((char*)m_text_Buff);

				if (TempLen > inLen)
					fnSCR_DisplayString(strNum, (char*)m_text_Buff, inLen, TRUE);
				else
					fnSCR_DisplayString(strNum, (char*)m_text_Buff, strlen((char*)m_text_Buff),TRUE);
				fnSCR_DisplayScreen(scrNum);
			}
			else
			{
				if (TempLen > inLen)
				{
					keyBuff[inLen] = NULL;
					TempLen = inLen;
				}

				if (RetCnt)
					*RetCnt = TempLen;
				
				//MsgDump("한글", (char*)keyBuff);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "S_CONFIRM[%s]", (char*)keyBuff);
				return S_CONFIRM;
			}	
		}
		else
		if (strReturn == "영문숫자")
		{
			gHanMode =  0;
			scrNum = 432;
			fnSCR_DisplayString(strNum, (char*)m_text_Buff, strlen((char*)m_text_Buff));
			fnSCR_DisplayScreen(scrNum);				// 영문화면
			fnSCR_DisplayString(strNum, (char*)m_text_Buff, strlen((char*)m_text_Buff),TRUE);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "영문숫자");
		}
		else
		if (strReturn == "한글")
		{
			gHanMode = COMBI_HAN_MODE;
			scrNum = 431;
			fnSCR_DisplayString(strNum, (char*)m_text_Buff, strlen((char*)m_text_Buff));
			fnSCR_DisplayScreen(scrNum);
			fnSCR_DisplayString(strNum, (char*)m_text_Buff, strlen((char*)m_text_Buff),TRUE);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "한글");
		}
		else
		if ((strReturn == S_CANCEL)		||
			(strReturn == S_TIMEOVER)	||
			(strReturn == S_INPUTOVER))
			return strReturn;
		else
		{
			int Len = fnSCR_KStrlen(&m_key_buf);

			if (( Len + 1 >= inLen )	&&
				( retch != 8)			&&		// Back Space
				( retch != 13))					// 정정
				;
			else
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "retch[%d]", retch);
///////////////////////////////////////////////////
				fnSCR_CALLchar(retch, 1, 1);
///////////////////////////////////////////////////
				if(gHanMode == COMBI_HAN_MODE)
					scrNum = 431;
				else
					scrNum = 432;

//				fnSCR_DisplayString(strNum, "                              ", inLen);
				TempLen = strlen((char*)m_text_Buff);

				if (TempLen > inLen)
					fnSCR_DisplayString(strNum, (char*)m_text_Buff, inLen, TRUE);
				else
					fnSCR_DisplayString(strNum, (char*)m_text_Buff, strlen((char*)m_text_Buff),TRUE);	
				
//				fnSCR_DisplayScreen(scrNum);
			}
		}
		Delay_Msg();
	}

	return S_CANCEL;
}

/*
void CDevCmn::fnSCR_IBK_Input2Byte(BYTE* TBuff, BYTE* SBuff , int Len)
{

	int i = 0;
	int j = 0;

	WORD Intbl[] = 
	{
		0xb0a3,				// 0
		0xb1a3,				// 1
		0xb2a3,				// 2
		0xb3a3,				// 3
		0xb4a3,				// 4
		0xb5a3,				// 5
		0xb6a3,				// 6
		0xb7a3,				// 7
		0xb8a3,				// 8
		0xb9a3				// 9
	};

	WORD Engtbl[] = 
	{
		0xc1a3,				// A
		0xc2a3,				// B
		0xc3a3,				// C
		0xc4a3,				// D
		0xc5a3,				// E
		0xc6a3,				// F
		0xc7a3,				// G
		0xc8a3,				// H
		0xc9a3,				// I
		0xcaa3,				// J
		0xcba3,				// K
		0xcca3,				// L
		0xcda3,				// M
		0xcea3,				// N
		0xcfa3,				// O
		0xd0a3,				// P
		0xd1a3,				// Q
		0xd2a3,				// R
		0xd3a3,				// S
		0xd4a3,				// T
		0xd5a3,				// U
		0xd6a3,				// V
		0xd7a3,				// W
		0xd8a3,				// X
		0xd9a3,				// Y
		0xdaa3				// Z
	};


	while(i < Len)
	{
		if (SBuff[i] > 0x80)
		{
			memcpy(&TBuff[j], &SBuff[i], 2);
			i += 2;
			j += 2;
		}
		else
		{
//	국민은행 : 영문, 숫자 2byte 완성형 코드로
//			if ((SBuff[i] >= 0x30) &&
//				(SBuff[i] <= 0x39))
//			{
//				memcpy(&TBuff[j], &Intbl[SBuff[i] - 0x30], 2);
//			}
//			else
//			if ((SBuff[i] >= 0x41) &&
//				(SBuff[i] <= 0x5a))
//			{
//				memcpy(&TBuff[j], &Engtbl[SBuff[i] - 0x41], 2);
//			}
//
//			i += 1;
//			j += 2;
// 조흥은행 : 영문, 숫자 1byte Ascii 코드로
			if ((SBuff[i] >= 0x30) &&
				(SBuff[i] <= 0x39))
			{
				TBuff[j] = SBuff[i];
			}
			else
			if ((SBuff[i] >= 0x41) &&
				(SBuff[i] <= 0x5a))
			{
				TBuff[j] = SBuff[i];
			}
			i += 1;
			j += 1;
		}
	}
}
*/


void CDevCmn::fnSCR_Input2Byte(BYTE* TBuff, BYTE* SBuff , int Len)
{

	int i = 0;
	int j = 0;

	WORD Intbl[] = 
	{
		0xb0a3,				// 0
		0xb1a3,				// 1
		0xb2a3,				// 2
		0xb3a3,				// 3
		0xb4a3,				// 4
		0xb5a3,				// 5
		0xb6a3,				// 6
		0xb7a3,				// 7
		0xb8a3,				// 8
		0xb9a3				// 9
	};

	WORD Engtbl[] = 
	{
		0xc1a3,				// A
		0xc2a3,				// B
		0xc3a3,				// C
		0xc4a3,				// D
		0xc5a3,				// E
		0xc6a3,				// F
		0xc7a3,				// G
		0xc8a3,				// H
		0xc9a3,				// I
		0xcaa3,				// J
		0xcba3,				// K
		0xcca3,				// L
		0xcda3,				// M
		0xcea3,				// N
		0xcfa3,				// O
		0xd0a3,				// P
		0xd1a3,				// Q
		0xd2a3,				// R
		0xd3a3,				// S
		0xd4a3,				// T
		0xd5a3,				// U
		0xd6a3,				// V
		0xd7a3,				// W
		0xd8a3,				// X
		0xd9a3,				// Y
		0xdaa3				// Z
	};


	while(i < Len)
	{
		if (SBuff[i] > 0x80)
		{
			memcpy(&TBuff[j], &SBuff[i], 2);
			i += 2;
			j += 2;
		}
		else
		{
			if ((SBuff[i] >= 0x30) &&
				(SBuff[i] <= 0x39))
			{
				memcpy(&TBuff[j], &Intbl[SBuff[i] - 0x30], 2);
			}
			else
			if ((SBuff[i] >= 0x41) &&
				(SBuff[i] <= 0x5a))
			{
				memcpy(&TBuff[j], &Engtbl[SBuff[i] - 0x41], 2);
			}

			i += 1;
			j += 2;
		}
	
	}

}

void CDevCmn::fnSCR_CALLchar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	HCODE code;
	CString str;
	UINT i;
	int nResult = 0;

	for(i=0; i < nRepCnt; i++)
	{
		switch(nChar)
		{
			case 0:
			case 0xe0:
			case -32:
					//화살표 키
				break;
			case 13:	//Enter
				//구조체를 초기화 합니다.
				HAN_INPUT_KEY_BUFFER_Clear(&m_key_buf);
				break;
			case 27:	//ESC
				break;
			default:
				
				if (gHanMode != COMBI_HAN_MODE)
				{
					if ((nChar >= 97) &&
						(nChar <= 122))
						nChar -= 32;

				}


				//입력받은 키를 한글 키로 변환
				code = COMBI_Key_to_HCODE(nChar, 0);

///////////////////////////////////////////////////////////////////////////////

				if ((gHanMode == COMBI_HAN_MODE) &&
					(nChar != 8)				 &&
					(nChar != 20))
					
				{
					HAN_INPUT_KEY_BUFFER Temp_key_buf;
	//				HAN_INPUT_KEY_BUFFER_Clear(&Temp_key_buf);
					nResult = HAN_INPUT_KEY_BUFFER_Create(&Temp_key_buf, 10);			// Codesonar Detected
					if (nResult == FALSE)												// Codesonar Detected
						return ;

					Temp_key_buf.cho	= m_key_buf.cho;
					Temp_key_buf.cho2	= m_key_buf.cho2;
					Temp_key_buf.jung	= m_key_buf.jung;
					Temp_key_buf.jong	= m_key_buf.jong;
					Temp_key_buf.jong2	= m_key_buf.jong2;
					Temp_key_buf.status = m_key_buf.status;

					HCODE Old_Tcode=COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(	Temp_key_buf.cho, 
																			Temp_key_buf.cho2, 
																			Temp_key_buf.jung, 
																			Temp_key_buf.jong, 
																			Temp_key_buf.jong2);


					COMBI_Han_Input(&Temp_key_buf, code);

					HCODE Tcode=COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(	Temp_key_buf.cho, 
																			Temp_key_buf.cho2, 
																			Temp_key_buf.jung, 
																			Temp_key_buf.jong, 
																			Temp_key_buf.jong2);
					
					if (HCODE_COMBI_to_KS(Tcode) == 0x2020)
					{
	//					AfxMessageBox("1");		// 글자없음
						SUNNY_COMBI_Han_Input(&m_key_buf, code);
					}
					else									// 글자가 같다고 하면
					if (HCODE_COMBI_to_KS(Tcode) == HCODE_COMBI_to_KS(Old_Tcode))
					{
						SUNNY_COMBI_Han_Input(&m_key_buf, code);
					}
					else
					{
						COMBI_Han_Input(&m_key_buf, code);
					}

					HAN_INPUT_KEY_BUFFER_Delete(&Temp_key_buf);

///////////////////////////////////////////////////////////////////////////////

				}
				else
					COMBI_Han_Input(&m_key_buf, code);

					

				//HAN_INPUT_KEY_BUFFER_COMBI_Print(1, 1, &m_key_buf);
				//HAN_INPUT_KEY_BUFFER_KS_Print(1, 1, &m_key_buf);
		}
	}
//	Invalidate();

	//키 버퍼에 있는 내용을 문자열로 변환
	ZeroMemory(m_text[0], 200);
	HAN_INPUT_KEY_BUFFER_COMBI_BufPrint(&m_key_buf, (char*)m_text[0]);

//	char StrBuff[100];
//	ZeroMemory(StrBuff, 100);

	ZeroMemory(m_text_Buff, 200);

	COMBI_Str_to_KS_Str((char*)m_text_Buff, (char*)m_text[0]);

//	COMBI_Str_to_KS_Str(StrBuff,m_text[0]);
	
//	m_EEE = (LPCTSTR)StrBuff;
//	UpdateData(FALSE);

}

int CDevCmn::fnSCR_KStrlen(HAN_INPUT_KEY_BUFFER *p)
{
	int Relen = 0;
	
	if(p->p > p->buf)										// 문자있음
	{
		//2문자 이상 존재
		//한글인지 검사
	
		char* Tp = p->buf;

		while (Tp <= p->p)
		{
			
			if ( ((*Tp) & 0x80) != 0 )
			{
				Tp =  Tp + 2;

				Relen += 2;									// 한글 2BYTE

				if (Tp == p->p)
					break;
			}
			else
			{
				Tp =  Tp + 1;

				Relen += 2;									// 영문숫자 2BYTE 계산

				if (Tp == p->p)
					break;
			}
		}
	}

	return Relen;
}

// V05-04-03-#01:송금인입력시 OVER되는 한글처리
int CDevCmn::fnSCR_KStrlen(BYTE *buf)
{
	int Relen = 0;
	int i;
	int len;

	if (!buf)
		return 0;
	
	len = strlen((char*)buf);

	for (i = 0; i < len; i++) {
		if (buf[i] & 0x80)
		{
			i++;			
		}
		Relen += 2;
	}
	
	return Relen;
}

