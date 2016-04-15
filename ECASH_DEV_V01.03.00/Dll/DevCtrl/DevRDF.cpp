/////////////////////////////////////////////////////////////////////////////
//	DevRFD.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// RF정보 초기화
int CDevCmn::fnRFC_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_Initialize()");

	int		nReturn = FALSE;
	CString strRFSendData("");

	memset(RFSendData, 0, sizeof(RFSendData));					// RF전송자료

	nReturn = fnRFD_SendData(COM_DEVINIT, strRFSendData);
	Delay_Msg(1000);
	strRFSendData = fstrRFD_GetData();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_Initialize(...):return[%d]", nReturn);
	return nReturn;
}

// RF기본정보 구하기
int CDevCmn::fnRFC_GetBasicInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_GetBasicInfo()");

	int		nReturn = FALSE;
	CString strRFSendData("");

	memset(RFSendData, 0, sizeof(RFSendData));					// RF전송자료

	nReturn = fnRFD_SendData(COM_BASICINF, strRFSendData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_GetBasicInfo(...):return[%d]", nReturn);
	return nReturn;
}

// RF상태 구하기
int CDevCmn::fnRFC_GetDevStatus()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_GetDevStatus()");

	int		nReturn = FALSE;
	CString strRFSendData("");

	memset(RFSendData, 0, sizeof(RFSendData));					// RF전송자료

	nReturn = fnRFD_SendData(COM_DEVSTAT, strRFSendData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_GetDevStatus(...):return[%d]", nReturn);
	return nReturn;
}

// RF충전 모드설정
int CDevCmn::fnRFC_SelectMode(int nMode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_SelectMode()");

	int		nReturn = FALSE;
	CString strRFSendData("");

	memset(RFSendData, 0, sizeof(RFSendData));					// RF전송자료

	// Message 조립
	RFSendData[0] = nMode;										// 0x30

	strRFSendData.Format("%1.1d", RFSendData);

	nReturn = fnRFD_SendData(COM_SELMODE, strRFSendData);

	strRFSendData = fstrRFD_GetData();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_SelectMode(...):return[%d]", nReturn);
	return nReturn;
}

// RF충전
int CDevCmn::fnRFC_ReqCharge(char *szMoney, char *szTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_ReqCharge()");
	int		nReturn = FALSE;
	int		RFSendLength = 0;
	CString strRFSendData("");
	BYTE	szTempBuf[1024];
	ULONG	TempValue = 0;

	memset(RFSendData, 0, sizeof(RFSendData));					// RF전송자료

	// CSN
	memset(szTempBuf, 0x00, sizeof(szTempBuf));
	TempValue = fnUTL_Asc2ULong(RFDBasicInfo.CardNo, 10);
	Sprintf((char *)szTempBuf, 8, "%08X", TempValue);
	// 1 2 3 4 5 6 7 8 => 7 8 5 6 3 4 1 2
	RFSendData[RFSendLength++] = szTempBuf[6];
	RFSendData[RFSendLength++] = szTempBuf[7];
	RFSendData[RFSendLength++] = szTempBuf[4];
	RFSendData[RFSendLength++] = szTempBuf[5];
	RFSendData[RFSendLength++] = szTempBuf[2];
	RFSendData[RFSendLength++] = szTempBuf[3];
	RFSendData[RFSendLength++] = szTempBuf[0];
	RFSendData[RFSendLength++] = szTempBuf[1];

	// 충전금액
	memset(szTempBuf, 0x00, sizeof(szTempBuf));
	Sprintf((char *)szTempBuf, 8, "%08X", Asc2Int(szMoney, 8));
	memcpy(&RFSendData[RFSendLength], szTempBuf, 8);
	RFSendLength += 8;

	// ATM NO
	memset(szTempBuf, 0x00, sizeof(szTempBuf));
	memcpy(szTempBuf, "3102000", 7);
	memcpy(&szTempBuf[7], &m_pProfile->NETWORK.BranchNum[5], 3);
	TempValue = fnUTL_Asc2ULong(szTempBuf, 10);
	Sprintf((char *)szTempBuf, 8, "%08X", TempValue);
	// 1 2 3 4 5 6 7 8 => 7 8 5 6 3 4 1 2
	RFSendData[RFSendLength++] = szTempBuf[6];
	RFSendData[RFSendLength++] = szTempBuf[7];
	RFSendData[RFSendLength++] = szTempBuf[4];
	RFSendData[RFSendLength++] = szTempBuf[5];
	RFSendData[RFSendLength++] = szTempBuf[2];
	RFSendData[RFSendLength++] = szTempBuf[3];
	RFSendData[RFSendLength++] = szTempBuf[0];
	RFSendData[RFSendLength++] = szTempBuf[1];

	// 충전시각
	memset(szTempBuf, 0x00, sizeof(szTempBuf));
	Sprintf(&szTempBuf[0], 2, "%02X", Asc2Int(&szTime[0], 2));
	Sprintf(&szTempBuf[2], 2, "%02X", Asc2Int(&szTime[2], 2));
	Sprintf(&szTempBuf[4], 2, "%02X", Asc2Int(&szTime[4], 2));
	Sprintf(&szTempBuf[6], 2, "%02X", Asc2Int(&szTime[6], 2));
	memcpy(&RFSendData[RFSendLength], szTempBuf, 8);
	RFSendLength += 8;

	strRFSendData.Format("%s", RFSendData);

	nReturn = fnRFD_SendData(COM_CHARGE, strRFSendData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_ReqCharge(...):return[%d]", nReturn);
	return nReturn;
}

// RF환불
int CDevCmn::fnRFC_DrawBack(char *szTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_DrawBack()");

	int		nReturn = FALSE;
	int		RFSendLength = 0;
	CString strRFSendData("");
	BYTE	szTempBuf[1024];
	ULONG	TempValue = 0;

	memset(RFSendData, 0, sizeof(RFSendData));					// RF전송자료

	// CSN
	memset(szTempBuf, 0x00, sizeof(szTempBuf));
	TempValue = fnUTL_Asc2ULong(RFDBasicInfo.CardNo, 10);
	Sprintf((char *)szTempBuf, 8, "%08X", TempValue);
	// 1 2 3 4 5 6 7 8 => 7 8 5 6 3 4 1 2
	RFSendData[RFSendLength++] = szTempBuf[6];
	RFSendData[RFSendLength++] = szTempBuf[7];
	RFSendData[RFSendLength++] = szTempBuf[4];
	RFSendData[RFSendLength++] = szTempBuf[5];
	RFSendData[RFSendLength++] = szTempBuf[2];
	RFSendData[RFSendLength++] = szTempBuf[3];
	RFSendData[RFSendLength++] = szTempBuf[0];
	RFSendData[RFSendLength++] = szTempBuf[1];

	// ATM NO
	memset(szTempBuf, 0x00, sizeof(szTempBuf));
	memcpy(szTempBuf, "3102000", 7);
	memcpy(&szTempBuf[7], &m_pProfile->NETWORK.BranchNum[5], 3);
	TempValue = fnUTL_Asc2ULong(szTempBuf, 10);
	Sprintf((char *)szTempBuf, 8, "%08X", TempValue);
	// 1 2 3 4 5 6 7 8 => 7 8 5 6 3 4 1 2
	RFSendData[RFSendLength++] = szTempBuf[6];
	RFSendData[RFSendLength++] = szTempBuf[7];
	RFSendData[RFSendLength++] = szTempBuf[4];
	RFSendData[RFSendLength++] = szTempBuf[5];
	RFSendData[RFSendLength++] = szTempBuf[2];
	RFSendData[RFSendLength++] = szTempBuf[3];
	RFSendData[RFSendLength++] = szTempBuf[0];
	RFSendData[RFSendLength++] = szTempBuf[1];

	// 환불시각
	memset(szTempBuf, 0x00, sizeof(szTempBuf));
	Sprintf(&szTempBuf[0], 2, "%02X", Asc2Int(&szTime[0], 2));
	Sprintf(&szTempBuf[2], 2, "%02X", Asc2Int(&szTime[2], 2));
	Sprintf(&szTempBuf[4], 2, "%02X", Asc2Int(&szTime[4], 2));
	Sprintf(&szTempBuf[6], 2, "%02X", Asc2Int(&szTime[6], 2));
	memcpy(&RFSendData[RFSendLength], szTempBuf, 8);
	RFSendLength += 8;

	strRFSendData.Format("%s", RFSendData);

	nReturn = fnRFD_SendData(COM_DRAWBACK, strRFSendData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_DrawBack(...):return[%d]", nReturn);
	return nReturn;
}

int CDevCmn::fnRFC_AnalRecvData(int nCmdType)
{
	int		nOffset = 0;
	int		nReturn = T_OK, nErrorReturn(9999);
	CString	strRFRecvData("");
	int		nRecvLen;
	ULONG	TempValue = 0;
	BYTE	szTempBuf[1024];

	memset(RFRecvData, 0, sizeof(RFRecvData));
	strRFRecvData = fstrRFD_GetData();
	memcpy(RFRecvData, strRFRecvData.GetBuffer(0), strRFRecvData.GetLength());
	nRecvLen = strRFRecvData.GetLength();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_AnalRecvData(%s)", RFRecvData);

	switch (nCmdType)
	{
	case RFD_PROC_BASICINFO:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_AnalRecvData(RFD_PROC_BASICINFO)");
		memset(&RFDBasicInfo, 0x20, sizeof(RFDBasicInfo));

		if (nRecvLen < 60)
		{
			nReturn = T_ERROR;
			break;
		}

		// CSN
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		MakePack(RFRecvData, szTempBuf, 8);
		TempValue = fnUTL_ByteLowOrder2Long(szTempBuf, 4);
		Sprintf((char *)RFDBasicInfo.CardNo, 10, "%010u", TempValue);
		nOffset += 8;

		// 카드잔액
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue, RFDBasicInfo.CardRemain, 8, 10, '0');
		nOffset += 8;

		// 발급자ID
		memcpy(RFDBasicInfo.CardRID, &RFRecvData[nOffset], sizeof(RFDBasicInfo.CardRID));
		nOffset += sizeof(RFDBasicInfo.CardRID);

		// 카드종류
		memcpy(RFDBasicInfo.CardType, &RFRecvData[nOffset], sizeof(RFDBasicInfo.CardType));
		nOffset += sizeof(RFDBasicInfo.CardType);

		// 카드상태
		memcpy(RFDBasicInfo.CardState, &RFRecvData[nOffset], sizeof(RFDBasicInfo.CardState));
		nOffset += sizeof(RFDBasicInfo.CardState);

		// 유효기간
		memcpy(RFDBasicInfo.ValidDate, "20", 2);
		memcpy(&RFDBasicInfo.ValidDate[2], &RFRecvData[nOffset], sizeof(RFDBasicInfo.ValidDate) - 2);
		nOffset += sizeof(RFDBasicInfo.ValidDate) - 2;

		// 거래카운트
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue, RFDBasicInfo.TranCount, 10, 10, '0');
		nOffset += 8;

		// CD/ATM No
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		MakePack(&RFRecvData[nOffset], szTempBuf, 8);
		TempValue = fnUTL_ByteLowOrder2Long(szTempBuf, 4);
		Sprintf((char *)RFDBasicInfo.CardID, 10, "%010u", TempValue);
		nOffset += 8;

		// 최종거래시간
		// 거래월
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue, RFDBasicInfo.LastLoadDate, 2, 10, '0');
		nOffset += 2;
		// 거래일
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue, &RFDBasicInfo.LastLoadDate[2], 2, 10, '0');
		nOffset += 2;
		// 거래시간 (시)
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue, &RFDBasicInfo.LastLoadDate[4], 2, 10, '0');
		nOffset += 2;
		// 거래시간 (분)
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue, &RFDBasicInfo.LastLoadDate[6], 2, 10, '0');
		nOffset += 2;
		break;

	case RFD_PROC_REQCHARGE:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_AnalRecvData(RFD_PROC_REQCHARGE)");
		memset(&RFDCharge, 0x20, sizeof(RFDCharge));

		if (nRecvLen < 60)
		{
			nReturn = T_ERROR;
			break;
		}

		// CSN
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		MakePack(RFRecvData, szTempBuf, 8);
		TempValue = fnUTL_ByteLowOrder2Long(szTempBuf, 4);
		Sprintf((char *)RFDCharge.CardNo, 10, "%010u", TempValue);
		nOffset += 8;

		// 보충잔액
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue,			RFDCharge.ChargeMoney,		8, 10, '0');
		nOffset += 8;

		// 보충전잔액
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue,			RFDCharge.PreChargeMoney,	8, 10, '0');
		nOffset += 8;

		// 보충후잔액
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue,			RFDCharge.CardRemain,		8, 10, '0');
		nOffset += 8;

		// 발급자ID
		memcpy(RFDCharge.CardRID,	&RFRecvData[nOffset],		sizeof(RFDCharge.CardRID));
		nOffset += sizeof(RFDCharge.CardRID);

		// 카드종류
		memcpy(RFDCharge.CardType,	&RFRecvData[nOffset],		sizeof(RFDCharge.CardType));
		nOffset += sizeof(RFDCharge.CardType);

		// 거래카운트
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue,			RFDCharge.TranCount,		8, 10, '0');
		nOffset += 8;

		// MAC
		MakePack(&RFRecvData[nOffset], RFDCharge.MacID, 8);
		break;

	case RFD_PROC_DRAWBACK:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_AnalRecvData(RFD_PROC_DRAWBACK)");
		memset(&RFDDrawBack, 0x20, sizeof(RFDDrawBack));

		if (nRecvLen < 52)
		{
			nReturn = T_ERROR;
			break;
		}

		// CSN
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		MakePack(RFRecvData, szTempBuf, 8);
		TempValue = fnUTL_ByteLowOrder2Long(szTempBuf, 4);
		Sprintf((char *)RFDDrawBack.CardNo, 10, "%010u", TempValue);
		nOffset += 8;

		// 환불전잔액
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue,				RFDDrawBack.PreBackMoney,	8, 10, '0');
		nOffset += 8;

		// 환불후잔액
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue,				RFDDrawBack.PostBackMoney,	8, 10, '0');
		nOffset += 8;

		// 발급자ID
		memcpy(RFDDrawBack.CardRID,		&RFRecvData[nOffset],		sizeof(RFDDrawBack.CardRID));
		nOffset += sizeof(RFDDrawBack.CardRID);

		// 카드종류
		memcpy(RFDDrawBack.CardType,	&RFRecvData[nOffset],		sizeof(RFDDrawBack.CardType));
		nOffset += sizeof(RFDDrawBack.CardType);
		
		// 거래카운트
		memset(szTempBuf, 0x00, sizeof(szTempBuf));
		memcpy(szTempBuf, &RFRecvData[nOffset], 8);
		TempValue = strtoul((char *)szTempBuf, 0, 16);
		Int2Asc(TempValue,			RFDDrawBack.TranCount,	  8, 10, '0');
		nOffset += 8;

		// MAC
		MakePack(&RFRecvData[nOffset], RFDDrawBack.MacID, 8);
		break;

	case RFD_PROC_UNKNOWN:
		MakePack(&RFRecvData[nOffset], RFDStatus, 16);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_AnalRecvData(RFDStatus)", RFDStatus, 8);
		break;

	default :
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnRFC_AnalRecvData(default)");
		break;
	}

	if (nReturn < 0)
		RFDAnalError = nReturn;
	
	return nReturn;
}
