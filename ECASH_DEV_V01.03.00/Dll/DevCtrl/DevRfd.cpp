/////////////////////////////////////////////////////////////////////////////
//	DevRFD.cpp : implementation file
//				 초기개발 : V04.04.00(RF)
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include "shlwapi.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	RFID함수(RFD)
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

int	CDevCmn::fnRFD_Read(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Read()");

	int		nReturn = FALSE;

	if (!fnAPL_GetDefineDevice(DEV_RFD))	return nReturn;
	
	CString strSend;	strSend.Empty();

	strSend.Format("%02X0B010E01%02X%02X%02X%02X", RfdSeq, RFD_READ, (1 >> 8) & 0xff, 1 & 0xff, nWaitSec);

	nReturn = fnRFD_SendData(strSend);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Read():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnRFD_Password(LPCTSTR szTRN, LPCTSTR szCSN, LPCTSTR szPwd)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Password()");

	int		idx		= 0;
	int		nReturn = FALSE;

	if (!fnAPL_GetDefineDevice(DEV_RFD))	return nReturn;

	CString strSend;	strSend.Empty();

	strSend.Format("%02X0B010E01%02X%02X%02X", RfdSeq, RFD_PSWD, (28 >> 8) & 0xff, 28 & 0xff);

	// TRN이 Pack된 데이터 일경우
//	for (idx=0; idx < 16; idx++)
//		sprintf(&strSend.GetBuffer(0)[idx*2 + 16], "%02X", (BYTE)szTRN[idx]);
	// TRN, CSN이 Unpack된 데이터 일경우
	memcpy(&strSend.GetBuffer(0)[16], (void *)szTRN, 32);

	// TRN, CSN이 Pack된 데이터 일경우
//	for (idx=0; idx < 8; idx++)
//		sprintf(&strSend.GetBuffer(0)[idx*2 + 48], "%02X", (BYTE)szPwd[idx]);
	// TRN, CSN이 Unpack된 데이터 일경우
	memcpy(&strSend.GetBuffer(0)[48], (void *)szCSN, 16);

	for (idx=0; idx < 4; idx++)
		sprintf(&strSend.GetBuffer(0)[idx*2 + 64], "%02X", (BYTE)szPwd[idx]);

	strSend.GetBuffer(0)[idx*2+64] = 0;

	nReturn = fnRFD_SendData(strSend);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Password()(%s)", strSend.GetBuffer(0));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Password():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnRFD_Cancel()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Cancel()");

	int		nReturn = FALSE;
	if (!fnAPL_GetDefineDevice(DEV_RFD))	return nReturn;

	CString strSend;	strSend.Empty();

	strSend.Format("%02X0B010E01%02X%02X%02X", RfdSeq, RFD_CNCL, (0 >> 8) & 0xff, 0 & 0xff);

	nReturn = fnRFD_SendData(strSend);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Cancel():return(%d)", nReturn);
	return nReturn;
}


int	CDevCmn::fnRFD_TimeSet(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_TimeSet()");

	int		nReturn = FALSE;

	if (!fnAPL_GetDefineDevice(DEV_RFD))	return nReturn;

	CString strSend;	strSend.Empty();

	strSend.Format("%02X0B010E01%02X%02X%02X%02X", RfdSeq, RFD_TIME, (1 >> 8) & 0xff, 1 & 0xff, nWaitSec);

	nReturn = fnRFD_SendData(strSend);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_TimeSet():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnRFD_Ready()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Ready()");

	int		nReturn = FALSE;

	if (!fnAPL_GetDefineDevice(DEV_RFD))	return nReturn;

	CString strSend;	strSend.Empty();

	strSend.Format("%02X0B010E01%02X%02X%02X", RfdSeq, RFD_INIT, (0 >> 8) & 0xff, 0 & 0xff);

	nReturn = fnRFD_SendData(strSend);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Ready():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnRFD_SendData(LPCTSTR szSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_SendData : RfdSeq[0x%x], szSendData[%s]", RfdSeq, szSendData);

	int		nReturn = FALSE;

	nReturn = m_pMwi->RfdSendData(szSendData);
	if (0xff == RfdSeq)	
		RfdSeq = 0x01;
	else
		RfdSeq++;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_SendData():return(%d)", nReturn);
	return nReturn;
}

// V04.05.00.#1 2007.10.30 by KTY
// T-money RF 잔액조회
int	CDevCmn::fnRFD_Inquiry(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Inquiry()");

	int		nReturn = FALSE;

	if (!fnAPL_GetDefineDevice(DEV_RFD))	return nReturn;
	
	CString strSend;	strSend.Empty();

	strSend.Format("%02X0B010E01%02X%02X%02X", RfdSeq, RFC_INQUIRY, (1 >> 8) & 0xff, (1 >> 8) & 0xff);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Inquiry : RFC_INQUIRY[%s]", strSend);

	nReturn = fnRFD_SendData(strSend);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_Inquiry():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnRFD_ChargeInquiry(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ChargeInquiry()");

	int		nReturn = FALSE;

	if (!fnAPL_GetDefineDevice(DEV_RFD))	return nReturn;
	
	CString strSend;	strSend.Empty();

	strSend.Format("%02X0B010E01%02X%02X%02X", RfdSeq, RFC_CHARGEINQUIRY, (1 >> 8) & 0xff, (1 >> 8) & 0xff);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ChargeInquiry : RFC_CHARGEINQUIRY[%s]", strSend);

	nReturn = fnRFD_SendData(strSend);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ChargeInquiry():return(%d)", nReturn);
	return nReturn;
}

int	CDevCmn::fnRFD_ReCharge1(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1()");

	int		nReturn = FALSE;

	if (!fnAPL_GetDefineDevice(DEV_RFD))	return nReturn;
	
	CString strSend;	strSend.Empty();

	
	BYTE RFSendData [1024];
	int		RFSendLength = 0;
	CString strRFSendData("");
	BYTE	szTempBuf[1024];
    SYSTEMTIME  lTime;
    GetLocalTime(&lTime);
	
	// 충전시각
	memset(szTempBuf, 0x00, sizeof(szTempBuf));
	int Tyyyy = Asc2Int(&lTime.wYear, 2);
	int Tmm = Asc2Int(&lTime.wMonth, 1);
	int Tdd = Asc2Int(&lTime.wDay, 1);
	int Thh = Asc2Int(&lTime.wHour, 1);
	int Tmm1 = Asc2Int(&lTime.wMinute, 1);
	int Tss = Asc2Int(&lTime.wSecond, 1);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1 : wYear[%d]", lTime.wYear);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1 : wYear[0x%02x]", lTime.wYear);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1 : wMonth[0x%02x]", lTime.wMonth);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1 : wDay[0x%02x]", lTime.wDay);

//	CString ttt;	ttt.Empty();
//	int ttt1 = lTime.wYear;
//	int ttt2 = lTime.wMonth;
//	int ttt3 = lTime.wDay;
//	int ttt4 = lTime.wHour;
//	int ttt5 = lTime.wMinute;
//	int ttt6 = lTime.wSecond;
//	ttt.Format("%02d%01d%01d%01d%01d%01d", ttt1, ttt2, ttt3, ttt4, ttt5, ttt6);
//	Sprintf(&szTempBuf[0], 2, "%7.7s", ttt);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1 : ttt[%s]", ttt);


	CString TEST11;
	TEST11.Format("%04d%02d%02d%02d%02d%02d", lTime.wYear,lTime.wMonth,lTime.wDay,lTime.wHour,lTime.wMinute,lTime.wSecond);
	BYTE CardNo [14];
ULONG TempValue = 0;
	memcpy(CardNo, TEST11, 14);
	TempValue = fnUTL_Asc2ULong(CardNo, 14);
//	BYTE	szTempBuf[1024];
	Sprintf((char *)szTempBuf, 14, "%014X", TempValue);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1 : ttt[%s]", ttt);


//	Sprintf(&szTempBuf[0], 2, "%02d", lTime.wYear);
//	Sprintf(&szTempBuf[2], 1, "%01d", lTime.wMonth);
//	Sprintf(&szTempBuf[3], 1, "%01d", lTime.wDay);
//	Sprintf(&szTempBuf[4], 1, "%01d", lTime.wHour);
//	Sprintf(&szTempBuf[5], 1, "%01d", lTime.wMinute);
//	Sprintf(&szTempBuf[6], 1, "%01d", lTime.wSecond);
	memcpy(&RFSendData[RFSendLength], szTempBuf, 7);
	RFSendLength += 7;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1 : RFSendData[%s]", RFSendData);
	// 충전금액
	memset(szTempBuf, 0x00, sizeof(szTempBuf));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptMoney:Accept.Money[%8.8s], [%d]", m_pTranCmn->Accept.Money, Asc2Int(&m_pTranCmn->Accept.Money[2], 8));
//	Sprintf((char *)szTempBuf, 4, "%04X", Asc2Int(&m_pTranCmn->Accept.Money[0], 8));
	Sprintf((char *)szTempBuf, 8, "%08X", Asc2Int(&m_pTranCmn->Accept.Money[0], 8));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1 : szTempBuf[%7.7s]", szTempBuf);
//[2008/01/17 21:32:42:915      DevRfd.cpp(00324)] fnAPP_AcceptMoney:Accept.Money[00005000], [500000]
//[2008/01/17 21:32:42:915      DevRfd.cpp(00326)] fnRFD_ReCharge1 : szTempBuf[1388]
	memcpy(&RFSendData[RFSendLength], szTempBuf, 4);
	RFSendLength += 4;


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1 : TEST11[%s]", TEST11);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1 : szTempBuf[%s]", szTempBuf);

//	strSend.Format("%02X0B010E01%02X000B2008011716312100001388", RfdSeq, RFC_RECHARGE1);
//	strSend.Format("%02X0B010E01%02X000B%11.11s", RfdSeq, RFC_RECHARGE1, RFSendData);
	strSend.Format("%02X0B010E01%02X000B%14.14s%8.8s", RfdSeq, RFC_RECHARGE1, TEST11, szTempBuf);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1 : RFC_CHARGEINQUIRY[%s]", strSend);

	nReturn = fnRFD_SendData(strSend);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge1():return(%d)", nReturn);
//AfxMessageBox("11111111111", MB_OK);
	return nReturn;
}

int	CDevCmn::fnRFD_ReCharge2(int nWaitSec)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge2()");

	int		nReturn = FALSE;

	if (!fnAPL_GetDefineDevice(DEV_RFD))	return nReturn;
	
	CString strSend;	strSend.Empty();

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge2 = m_pTranCmn->stRecvTMoneyData.SAMID", m_pTranCmn->stRecvTMoneyData.SAMID, sizeof(m_pTranCmn->stRecvTMoneyData.SAMID));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge2 = m_pTranCmn->stRecvTMoneyData.SAMSerialNo", m_pTranCmn->stRecvTMoneyData.SAMSerialNo, sizeof(m_pTranCmn->stRecvTMoneyData.SAMSerialNo));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge2 = m_pTranCmn->stRecvTMoneyData.RFTSIGN1", m_pTranCmn->stRecvTMoneyData.RFTSIGN1, sizeof(m_pTranCmn->stRecvTMoneyData.RFTSIGN1));
	
	int		RFSendLength = 0;
	CString strRFSendData("");
	
	BYTE	szTempBuf[1024];
	memset(szTempBuf, 0x00, sizeof(szTempBuf));
	Sprintf((char *)szTempBuf, 8, "%08X", Asc2Int(&m_pTranCmn->stRecvTMoneyData.SAMID[0], 16));

//	BYTE	szTempBuf1[1024];
//	memset(szTempBuf1, 0x00, sizeof(szTempBuf1));
//	Sprintf((char *)szTempBuf1, 4, "%04X", Asc2Int(&m_pTranCmn->stRecvTMoneyData.SAMSerialNo[0], 10));
	BYTE	szTempBuf1[1024];
	memset(szTempBuf1, 0x00, sizeof(szTempBuf1));
	Sprintf((char *)szTempBuf1, 8, "%08X", Asc2Int(&m_pTranCmn->stRecvTMoneyData.SAMSerialNo[0], 10));

	BYTE	szTempBuf2[1024];
	memset(szTempBuf2, 0x00, sizeof(szTempBuf2));
	Sprintf((char *)szTempBuf2, 4, "%04X", Asc2Int(&m_pTranCmn->stRecvTMoneyData.RFTSIGN1[0], 8));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge2 : szTempBuf[%s]", szTempBuf);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge2 : szTempBuf1[%s]", szTempBuf1);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge2 : szTempBuf2[%s]", szTempBuf2);

	BYTE	szTempBuf3[1024];
	memset(szTempBuf3, 0x30, sizeof(szTempBuf3));
	BYTE	szTempBuf4[1024];
	memset(szTempBuf4, 0x31, sizeof(szTempBuf4));

//	strRFSendData.Format("%16.16s%8.8s%8.8s%4.4s%32.32s", 
//		m_pTranCmn->stRecvTMoneyData.SAMID, szTempBuf1, m_pTranCmn->stRecvTMoneyData.RFTSIGN1, szTempBuf3, szTempBuf4);
	strRFSendData.Format("%16.16s%8.8s%8.8s3030%32.32s", 
		m_pTranCmn->stRecvTMoneyData.SAMID, szTempBuf1, m_pTranCmn->stRecvTMoneyData.RFTSIGN1, szTempBuf4);

//	strSend.Format("%02X0B010E01%02X0022%8.8s%4.4s%4.4s%2.2s%16.16s", RfdSeq, RFC_RECHARGE2, szTempBuf, szTempBuf1, szTempBuf2, szTempBuf3, szTempBuf4);
	strSend.Format("%02X0B010E01%02X0022%68.68s", RfdSeq, RFC_RECHARGE2, strRFSendData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge2 : RFC_CHARGEINQUIRY[%s]", strSend);

	nReturn = fnRFD_SendData(strSend);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFD_ReCharge2():return(%d)", nReturn);

	return nReturn;
}
