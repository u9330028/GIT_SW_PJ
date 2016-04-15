/////////////////////////////////////////////////////////////////////////////
//	DevKCash.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevICCard.h"
#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	K-CASH함수(KCS) : K-CASH SET MODE
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	K-CASH함수(KCS) : K-CASH GET MODE
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	K-CASH함수(KCS) : K-CASH OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// 전원공급
int	CDevCmn::fnKCS_PowerOn()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PowerOn()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	if(IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_POWERONCARD, "");
	else
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;									// 0x30
		ICSendData[1] = IC_MFC_BASIC;								// 0x01
		ICSendData[2] = IC_SFC_POWERON;								// 0x01
		
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PowerOn():return[%d]", nReturn);
	return nReturn;
}

// 전원차단
int	CDevCmn::fnKCS_PowerOff()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PowerOff()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	if(IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_POWEROFFCARD, "");
	else
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;									// 0x30
		ICSendData[1] = IC_MFC_BASIC;								// 0x01
		ICSendData[2] = IC_SFC_POWEROFF;							// 0x04

		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PowerOff():return[%d]", nReturn);
	return nReturn;
}

// ATR읽기:시범카드,본사업카드
int	CDevCmn::fnKCS_ReadATR()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadATR()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_STDEP;								// 0x02
	ICSendData[2] = IC_SFC_ATRINFO;								// 0x03

	if(IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_READATR);
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadATR():return[%d]", nReturn);
	return nReturn;
}

// 비밀번호검사
int	CDevCmn::fnKCS_CheckPin(LPCTSTR szPassWord)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_CheckPin(szPassWord[%s])", szPassWord);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_STDEP;								// 0x02
	ICSendData[2] = IC_SFC_PINCHECK;							// 0x02

	// ISO-IN CMD를 이용하여 처리
//	ICSendData[0] = 0x30;
//	ICSendData[1] = 0x01;
//	ICSendData[2] = 0x02;
//	ICSendData[3] = 0x00;
//	ICSendData[4] = 0x20;
//	ICSendData[5] = 0x00;
//	ICSendData[6] = 0x01;
//	ICSendData[7] = 0x08;

	if(IfmDirect) //#NICCM01
	{
		strICSendData = szPassWord;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_CHECKPIN, strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szPassWord;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_CheckPin(...):return[%d]", nReturn);
	return nReturn;
}

// 거래중지Bit확인
int	CDevCmn::fnKCS_VerifyTranBit()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_VerifyTranBit()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_STDEP;								// 0x02
	ICSendData[2] = IC_SFC_VERIFYTRANBIT;						// 0x05

	if(IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_VERIFYTRANBIT);
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_VerifyTranBit():return[%d]", nReturn);
	return nReturn;
}

// 거래중지Bit제거
int	CDevCmn::fnKCS_RemoveTranBit()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_RemoveTranBit()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_STDEP;								// 0x02
	ICSendData[2] = IC_SFC_TRANCMPLT;							// 0x04

	if(IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_REMOVETRANBIT);
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_RemoveTranBit():return[%d]", nReturn);
	return nReturn;
}

// 교통정보읽기
int CDevCmn::fnKCS_ReadTrafficInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadTrafficInfo()");

	int nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_STDEP;								// 0x02
	ICSendData[2] = IC_SFC_TRAFFICINFO;							// 0x06

	if(IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_READTRAFFICINFO);	
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadTrafficInfo():return[%d]", nReturn);
	return nReturn;
}

// 계좌정보읽기
int	CDevCmn::fnKCS_ReadPan(LPCTSTR szPassWord)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadPan(szPassWord[%s])", szPassWord);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_VALUE;								// 0x06
	ICSendData[2] = IC_SFC_READPAN;								// 0x0B

	if(IfmDirect) //#NICCM01
	{
		strICSendData = szPassWord;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_READPAN, strICSendData);
	}else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szPassWord;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadPan(...):return[%d]", nReturn);
	return nReturn;
}

// 잔액정보읽기
int	CDevCmn::fnKCS_ReadBAmount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadBAmount()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_STDEP;								// 0x02
	ICSendData[2] = IC_SFC_READJAN;								// 0x01

	if(IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_READBAMOUNT);
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}
MsgDumpAnal(TRACE_CODE_MODE, "Log", "fnKCS_ReadBAmount():return[%d]", nReturn);
	return nReturn;
}

// 개인정보읽기
int	CDevCmn::fnKCS_ReadPersonInfo(LPCTSTR szPassWord)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadPersonInfo(szPassWord[%s])", szPassWord);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

//////****** APBD 미구현 업무코드
	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_CC;									// 0x07
	ICSendData[2] = IC_SFC_READPERSONINFO;						// 0x08

	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szPassWord;
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadPersonInfo(...):return[%d]", nReturn);
	return nReturn;
}

// 주계좌정보읽기
int	CDevCmn::fnKCS_ReadPassBook1(LPCTSTR szPassWord)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadPassBook1(szPassWord[%s])", szPassWord);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_PBOOK;								// 0x0a
	ICSendData[2] = IC_SFC_MAINPASSBK;							// 0x01

	if(IfmDirect) //#NICCM01
	{
		strICSendData = szPassWord;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, EPB_GETPASSBOOK1);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szPassWord;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadPassBook1(...):return[%d]", nReturn);
	return nReturn;
}

// 충전전처리(거래금액4+암호8)
int	CDevCmn::fnKCS_PrepareLoadIEP(LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PrepareLoadIEP(szICSendData[%s])", szICSendData);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_VALUE;								// 0x06
	ICSendData[2] = IC_SFC_PRELOAD;								// 0x01

	if(IfmDirect) //#NICCM01
	{
		strICSendData = szICSendData;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_PREPARELOADIEP,strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szICSendData;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PrepareLoadIEP(...):return[%d]", nReturn);
	return nReturn;
}

// 충전요구(난수4+서명8)
int	CDevCmn::fnKCS_LoadIEP(LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_LoadIEP(szICSendData[%s])", szICSendData);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_VALUE;								// 0x06
	ICSendData[2] = IC_SFC_LOADIEP;								// 0x02

	if(IfmDirect) //#NICCM01
	{
		strICSendData = szICSendData;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_LOADIEP,strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szICSendData;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_LoadIEP(...):return[%d]", nReturn);
	return nReturn;
}

// 환불전처리(거래금액4+암호8)
int	CDevCmn::fnKCS_PrepareUnLoadIEP(LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PrepareUnLoadIEP(szICSendData[%s])", szICSendData);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_VALUE;								// 0x06
	ICSendData[2] = IC_SFC_PREUNLOAD;							// 0x04

	if(IfmDirect) //#NICCM01
	{
		strICSendData = szICSendData;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_PREPAREUNLOADIEP,strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szICSendData;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PrepareUnLoadIEP(...):return[%d]", nReturn);
	return nReturn;
}

// 환불요구(난수4+서명8)
int	CDevCmn::fnKCS_UnloadIEP(LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_UnloadIEP(szICSendData[%s])", szICSendData);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_VALUE;								// 0x06
	ICSendData[2] = IC_SFC_UNLOAD;								// 0x05

	if(IfmDirect) //#NICCM01
	{
		strICSendData = szICSendData;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_UNLOADIEP,strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szICSendData;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_UnloadIEP(...):return[%d]", nReturn);
	return nReturn;
}

// 파라메터갱신전처리
int	CDevCmn::fnKCS_PrepareUpdateIEP()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PrepareUpdateIEP()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_PARAM;								// 0x08
	ICSendData[2] = IC_SFC_PREUPDATE;							// 0x01

	if(IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_PREPAREUPDATEIEP);
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PrepareUpdateIEP():return[%d]", nReturn);
	return nReturn;
}

// 파라메터갱신(SamId8+Parameter4+Versin2+Value32+난수16+서명8)
int	CDevCmn::fnKCS_UpdateIEP(LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_UpdateIEP(szICSendData[%s])", szICSendData);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_PARAM;								// 0x08
	ICSendData[2] = IC_SFC_UPDATEIEP;							// 0x02

	if(IfmDirect) //#NICCM01
	{
		strICSendData = szICSendData;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_UPDATEIEP,strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szICSendData;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_UpdateIEP(...):return[%d]", nReturn);
	return nReturn;
}

// 비밀번호변경(변경전암호8+변경후암호8)
int	CDevCmn::fnKCS_ChangePin(LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ChangePin(szICSendData[%s])", szICSendData);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_PIN;									// 0x09
	ICSendData[2] = IC_SFC_PINCHANGE;							// 0x09

	if(IfmDirect) //#NICCM01
	{
		strICSendData = szICSendData;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_PINCHANGE2,strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szICSendData;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ChangePin(...):return[%d]", nReturn);
	return nReturn;
}

// 충전환불거래내역읽기
int	CDevCmn::fnKCS_ReadCLog()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadCLog()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_VALUE;								// 0x06
	ICSendData[2] = IC_SFC_READCLOG;							// 0x09

	if(IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_READCLOG);
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadCLog():return[%d]", nReturn);
	return nReturn;
}

// 구매거래내역읽기(1-5)
int	CDevCmn::fnKCS_ReadPLog1()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadPLog1()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_VALUE;								// 0x06
	ICSendData[2] = IC_SFC_READPLOG;							// 0x08

	if(IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_READPLOG1);
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadPLog1():return[%d]", nReturn);
	return nReturn;
}

// 구매거래내역읽기(6-10)
int	CDevCmn::fnKCS_ReadPLog2()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadPLog2()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_VALUE;								// 0x06
	ICSendData[2] = IC_SFC_READPLOG2;							// 0x0A

	if(IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_READPLOG2);
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadPLog2():return[%d]", nReturn);
	return nReturn;
}

// 환불복구전처리
int CDevCmn::fnKCS_PrepareUnLoadRcvIEP(LPCTSTR szPassWord)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PrepareUnLoadRcvIEP()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_VALUE;								// 0x06
	ICSendData[2] = IC_SFC_PRERCVUNLOAD;						// 0x0C

	if(IfmDirect) //#NICCM01
	{
		strICSendData = szPassWord;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_PREPAREUNLOADRCVIEP,strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szPassWord;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_PrepareUnLoadRcvIEP():return[%d]", nReturn);
	return nReturn;
}

// 전자화폐번호 읽어오기(기업용) 
int	CDevCmn::fnKCS_ReadEPID()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadEPID()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_VALUE;								// 0x06
	ICSendData[2] = IC_SFC_READEPID;							// 0x0D

	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_ReadEPID():return[%d]", nReturn);
	return nReturn;
}

// 수신분석
int CDevCmn::fnKCS_AnalRecvData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_AnalRecvData()");

	int		nReturn = 0, nErrorReturn = 9999;
	CString	strICRecvData("");
	int		i = 0;

	memset(ICRecvData, 0, sizeof(ICRecvData));
	strICRecvData = m_pDevCmn->fstrMCU_GetICData();
	MakePack(strICRecvData.GetBuffer(0), ICRecvData, __min(sizeof(ICRecvData) * 2, strICRecvData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_AnalRecvData:ICRecvData", ICRecvData, 
			__min(sizeof(ICRecvData) * 2, strICRecvData.GetLength()) / 2);

/////////////////////////////////////////////////////////////////////////////
	if ((!memcmp(ICRecvData, IC_RECV_OK, 3))	||				// 정상
		(!memcmp(ICRecvData, IC_RECV_OK2, 3))	)
		;
	else														// 비정상
	{
		// V07-00-04-#01
		fnICA_SetFCError(ICRecvData);							// IC 인식률 변경 IC 동작 장애코드를 저장한다
		
		nReturn = nErrorReturn;									// 기타오류
		for (int i = 0; i < itemof(FCErrMsg); i++)			// 2004.01.27
		{
			if (!memcmp(ICRecvData, &FCErrMsg[i].FC, IC_ERRORRESP_LEN))
			{
				nReturn = i + 1;
				break;
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////
	switch (nReturn)											
	{
		case	0:												// 정상시분석처리
			if ((ICSendData[1] == IC_MFC_BASIC) &&				// 전원공급
				(ICSendData[2] == IC_SFC_POWERON))
			{
				// 초기화
				memset(&KCashInfo, 0, sizeof(KCashInfo));
			}
			else
			if ((ICSendData[1] == IC_MFC_BASIC) &&				// 전원차단
				(ICSendData[2] == IC_SFC_POWEROFF))
			{
				// 초기화
				memset(&KCashInfo, 0, sizeof(KCashInfo));
			}
			else
			if ((ICSendData[1] == IC_MFC_STDEP) &&				// ATR읽기
				(ICSendData[2] == IC_SFC_ATRINFO))
			{
				// 데이타의길이복사
				int		RecvSize = 36;

				// 식별자ID(카드번호)
				for (i = 0 ; i < RecvSize ; i++)
				{
					if ((ICRecvData[i] == 0x45) && (ICRecvData[i + 1] == 0x10)) 
						break;
				}
				if ((i + 1) >= RecvSize) 
				{
					nReturn = nErrorReturn;						// 기타오류
					break;
				}
				// 카드번호
				memcpy(KCashInfo.CardNum, &ICRecvData[i + 7], 8);
				
				// 카드발급일자
				if (RecvSize < 15)
				{
					nReturn = nErrorReturn;						// 기타오류
					break;
				}
				for (i = RecvSize - 15 ; i < RecvSize ; i++)
				{
					if ((ICRecvData[i] == 0x5f) && (ICRecvData[i + 1] == 0x26)) 
						break;
				}
				if ((i + 1) >= RecvSize) 
				{
					nReturn = nErrorReturn;						// 기타오류
					break;
				}
				// 카드발급일자(BCD -> String)					
				for ( i = 0; i < 4; i++ )						
				{												
					sprintf(KCashInfo.CardIssueDate + 2 * i, "%02X", ICRecvData[i + 26] );			
				}												
				
				// 카드정보
				if (RecvSize < 8)
				{
					nReturn = nErrorReturn;						// 기타오류
					break;
				}
				for (i = RecvSize - 8 ; i < RecvSize ; i++)
				{
					if ((ICRecvData[i] == 0x47) && (ICRecvData[i + 1] == 0x04)) 
						break;
				}
				if ((i + 1) >= RecvSize) 
				{
					nReturn = nErrorReturn;						// 기타오류
					break;
				}

				// 카드구분
				memcpy(KCashInfo.CardInform, &ICRecvData[i + 2], 4);
			}
			else
			if ((ICSendData[1] == IC_MFC_STDEP) &&				// 비밀번호검사
				(ICSendData[2] == IC_SFC_PINCHECK))
			{
			}
			else
			if ((ICSendData[1] == IC_MFC_STDEP) &&				// 거래중지Bit확인
				(ICSendData[2] == IC_SFC_VERIFYTRANBIT))
			{
			}
			else
			if ((ICSendData[1] == IC_MFC_STDEP) &&				// 거래중지Bit제거
				(ICSendData[2] == IC_SFC_TRANCMPLT))
			{
			}
			else
			if ((ICSendData[1] == IC_MFC_STDEP) &&				// 교통정보조회
				(ICSendData[2] == IC_SFC_TRAFFICINFO))
			{
				memcpy(&KCashInfo.UserCode, &ICRecvData[3], 1);	// 소지자정보
				memcpy(KCashInfo.ValidDate, &ICRecvData[4], 2);	// 유효기간
			}
			else
			if ((ICSendData[1] == IC_MFC_VALUE) &&				// 계좌정보읽기
				(ICSendData[2] == IC_SFC_READPAN))
			{
				// 계좌정보
				// 01234567890123456789012345
				// 31000059020=16026536702101
				memcpy(KCashInfo.Account, &ICRecvData[3], 10);
			}
			else
			if ((ICSendData[1] == IC_MFC_STDEP) &&				// 잔액정보읽기
				(ICSendData[2] == IC_SFC_READJAN))
			{
				// 잔액
				memcpy(KCashInfo.Balance, &ICRecvData[3], 4);
				// 충전한도금액
				memcpy(KCashInfo.MaxLimitAmount, &ICRecvData[13], 4);
				// 충전한도금액
				memcpy(KCashInfo.MaxLoadUnloadAmount, &ICRecvData[17], 4);
			}
			else
			if ((ICSendData[1] == IC_MFC_CC) &&					// 개인정보읽기
				(ICSendData[2] == IC_SFC_READPERSONINFO))
			{
				memcpy(KCashInfo.EnglishName,   &ICRecvData[8],  24);
				memcpy(KCashInfo.KoreanName,    &ICRecvData[37], 24);
				memcpy(KCashInfo.CitizenNumber, &ICRecvData[66], 13);
			}
			else
			if ((ICSendData[1] == IC_MFC_PBOOK) &&				// 주계좌정보읽기
				(ICSendData[2] == IC_SFC_MAINPASSBK))
			{
				memcpy(KCashInfo.MainAccount, &ICRecvData[4], 52);
			}
			else
			if ((ICSendData[1] == IC_MFC_VALUE) &&				// 충전전처리
				(ICSendData[2] == IC_SFC_PRELOAD))
			{
				KCashInfo.LoadUnloadAlgorithmID			= ICRecvData[3];
				KCashInfo.LoadUnloadKeyVersion			= ICRecvData[4];
				memcpy(KCashInfo.LoadUnloadSerialNo,	&ICRecvData[5], 2);
				memcpy(KCashInfo.LoadUnloadRandomNum,	&ICRecvData[7], 8);
				memcpy(KCashInfo.LoadUnloadEPMakerID,	&ICRecvData[15], 3);
				memcpy(KCashInfo.LoadUnloadEPID,		&ICRecvData[18], 5);
				memcpy(KCashInfo.LoadUnloadBalance,		&ICRecvData[23], 4);
				memcpy(KCashInfo.LoadUnloadAccount,		&ICRecvData[27], 10);
				memcpy(KCashInfo.LoadUnloadSignature,	&ICRecvData[37], 4);
			}
			else
			if ((ICSendData[1] == IC_MFC_VALUE) &&				// 충전요구
				(ICSendData[2] == IC_SFC_LOADIEP))
			{
				memcpy(KCashInfo.LoadUnloadSignature3 , &ICRecvData[3], 4);
			}
			else
			if ((ICSendData[1] == IC_MFC_VALUE) &&				// 환불전처리
				(ICSendData[2] == IC_SFC_PREUNLOAD))
			{
				KCashInfo.LoadUnloadAlgorithmID			= ICRecvData[3];
				KCashInfo.LoadUnloadKeyVersion			= ICRecvData[4];
				memcpy(KCashInfo.LoadUnloadSerialNo,	&ICRecvData[5], 2);
				memcpy(KCashInfo.LoadUnloadRandomNum,	&ICRecvData[7], 8);
				memcpy(KCashInfo.LoadUnloadEPMakerID,	&ICRecvData[15], 3);
				memcpy(KCashInfo.LoadUnloadEPID,		&ICRecvData[18], 5);
				memcpy(KCashInfo.LoadUnloadBalance,		&ICRecvData[23], 4);
				memcpy(KCashInfo.LoadUnloadAccount,		&ICRecvData[27], 10);
				memcpy(KCashInfo.LoadUnloadSignature,	&ICRecvData[37], 4);
			}
			else
			if ((ICSendData[1] == IC_MFC_VALUE) &&				// 환불요구
				(ICSendData[2] == IC_SFC_UNLOAD))
			{
				memcpy(KCashInfo.LoadUnloadSignature3 , &ICRecvData[3], 4);
			}
			else
			if ((ICSendData[1] == IC_MFC_PARAM) &&				// 파라메터갱신전처리
				(ICSendData[2] == IC_SFC_PREUPDATE))			// 2004.02.27
			{
				KCashInfo.LoadUnloadAlgorithmID			= ICRecvData[3];
				KCashInfo.LoadUnloadKeyVersion			= ICRecvData[4];
				memcpy(KCashInfo.LoadUnloadSerialNo,	&ICRecvData[5], 2);
				memcpy(KCashInfo.LoadUnloadRandomNum,	&ICRecvData[7], 8);
				memcpy(KCashInfo.LoadUnloadEPMakerID,	&ICRecvData[15], 3);
				memcpy(KCashInfo.LoadUnloadEPID,		&ICRecvData[18], 5);
				memcpy(KCashInfo.LoadUnloadSignature,	&ICRecvData[23], 4);
			}
			else
			if ((ICSendData[1] == IC_MFC_PARAM) &&				// 파라메터갱신
				(ICSendData[2] == IC_SFC_UPDATEIEP))
			{
			}
			else
			if ((ICSendData[1] == IC_MFC_PIN) &&				// 비밀번호변경
				(ICSendData[2] == IC_SFC_PINCHANGE))
			{
			}
			else
			if ((ICSendData[1] == IC_MFC_VALUE) &&				// 충전환불거래내역읽기
				(ICSendData[2] == IC_SFC_READCLOG))
			{
				KCashInfo.LoadUnloadCnt = ICRecvData[3] & 0x0f;

				if (!KCashInfo.LoadUnloadCnt)					// 건수무
					break;
			
				for (i = 0; i < KCashInfo.LoadUnloadCnt; i++)
					memcpy(&KCashInfo.LoadUnloadBuff[i][0], &ICRecvData[4 + (i * 13)], 13);
			}
			else
			if ((ICSendData[1] == IC_MFC_VALUE) &&				// 구매거래내역읽기(1-5)
				(ICSendData[2] == IC_SFC_READPLOG))
			{
				KCashInfo.BuyListCnt = ICRecvData[3] & 0x0f;

				if (!KCashInfo.BuyListCnt)						// 건수무
					break;
			
				for (i = 0; i < KCashInfo.BuyListCnt; i++)
					memcpy(&KCashInfo.BuyListBuff[i][0], &ICRecvData[4 + (i * 28)], 28);	// 2004.02.16 : 13=>28
			}
			else
			if ((ICSendData[1] == IC_MFC_VALUE) &&				// 구매거래내역읽기(6-10)
				(ICSendData[2] == IC_SFC_READPLOG2))
			{
				KCashInfo.BuyListCnt = ICRecvData[3] & 0x0f;

				if (!KCashInfo.BuyListCnt)						// 건수무
					break;
			
				for (i = 0; i < KCashInfo.BuyListCnt; i++)
					memcpy(&KCashInfo.BuyListBuff[i][0], &ICRecvData[4 + (i * 28)], 28);	// 2004.02.16 : 13=>28
			}
			else
			if ((ICSendData[1] == IC_MFC_VALUE) &&				// 환불복구전처리
				(ICSendData[2] == IC_SFC_PRERCVUNLOAD))
			{
				memcpy(KCashInfo.Account, &ICRecvData[3], 10);	// PAN정보

				memcpy(KCashInfo.Balance, &ICRecvData[13], 4);	// 잔액	
				memcpy(KCashInfo.MaxLimitAmount, &ICRecvData[23], 4);// 충전한도금액

				KCashInfo.LoadUnloadAlgorithmID			= ICRecvData[33];
				KCashInfo.LoadUnloadKeyVersion			= ICRecvData[34];
				memcpy(KCashInfo.LoadUnloadSerialNo,	&ICRecvData[35], 2);
				memcpy(KCashInfo.LoadUnloadRandomNum,	&ICRecvData[37], 8);
				memcpy(KCashInfo.LoadUnloadEPMakerID,	&ICRecvData[45], 3);
				memcpy(KCashInfo.LoadUnloadEPID,		&ICRecvData[48], 5);
				memcpy(KCashInfo.LoadUnloadSignature,	&ICRecvData[53], 4);

				memcpy(KCashInfo.LoadUnloadProof, &ICRecvData[57], 7);	// Proof
			}
			else
			if ((ICSendData[1] == IC_MFC_VALUE) &&				// 전자화폐번호읽어오기
				(ICSendData[2] == IC_SFC_READEPID))
			{
				memcpy(KCashInfo.CardNum , &ICRecvData[3], 5);
			}
			break;

		default:												// 비정상
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnKCS_AnalRecvData():return(nReturn[%d])", nReturn);
	return nReturn;
}
