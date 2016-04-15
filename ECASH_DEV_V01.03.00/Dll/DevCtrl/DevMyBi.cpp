/////////////////////////////////////////////////////////////////////////////
//	DevMyBi.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CmnLibIn.h"
#include "Define.h"
#include "ConstDef.h"
#include "ClassInclude.h"

#include "DevEtc.h"
#include "DevICCard.h"
#include "DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnIMB_MyBiCardInfoRead()
// 설    명 : 카드의 각종 정보를 일괄로 읽는다(GET MYBI DATA)
// 파라미터 : 없음
// 리턴코드 : fnMCU_ICSendData()에 대한 결과
// 주의사항 : 
// 히스토리 : 2004/07/07, 경남은행, 01.00.00
//////////////////////////////////////////////////////////////////////////////
int	CDevCmn::fnIMB_MyBiCardInfoRead()
{
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiCardInfoRead()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_MYBI;								// 0x30
	ICSendData[2] = IC_SFC_MYBICARDINFOREAD;					// 0x01

	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);

MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiCardInfoRead():return[%d]", nReturn);
	return nReturn;
}

//////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnIMB_MyBiLoadInit()
// 설    명 : 보충 또는 환불을 위한 카드 초기화를 한다
// 호출방법 : m_pDevCmn->fnIMB_MyBiCardInfoRead(m_pDevCmn->ParamLoadInit.Flag)
// 파라미터 : 없음
// 리턴코드 : fnMCU_ICSendData()에 대한 결과
// 주의사항 : 
// 히스토리 : 2004/07/07, 경남은행, 01.00.00
//////////////////////////////////////////////////////////////////////////////
int	CDevCmn::fnIMB_MyBiLoadInit(LPBYTE lpFlag, int nLen)
{
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiLoadInit()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_MYBI;								// 0x30
	ICSendData[2] = IC_SFC_MYBILOADINIT;						// 0x02
	memcpy(ICSendData + IC_FCMS_LEN, lpFlag, nLen);
	
	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + nLen);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);

MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiLoadInit():return[%d]", nReturn);
	return nReturn;
}

//////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnIMB_MyBiLoadEXE()
// 설    명 : 보충 절차를 수행하다
// 호출방법 : m_pDevCmn->MybiLoadExe(m_pDevCmn->ParamLoadEXE.RandomNum, sizeof(m_pDevCmn->ParamLoadEXE))
// 파라미터 : 없음
// 리턴코드 : fnMCU_ICSendData()에 대한 결과
// 주의사항 : 
// 히스토리 : 2004/07/07, 경남은행, 01.00.00
//////////////////////////////////////////////////////////////////////////////
int	CDevCmn::fnIMB_MyBiLoadEXE(LPBYTE lpRandomNum, int RandomLen)
{
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiLoadEXE()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_MYBI;								// 0x30
	ICSendData[2] = IC_SFC_MYBILOADEXE;							// 0x03
	memcpy(ICSendData + IC_FCMS_LEN, lpRandomNum, RandomLen);

	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + RandomLen);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);

MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiLoadEXE():return[%d]", nReturn);
	return nReturn;
}

//////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnIMB_MyBiUnloadEXE()
// 설    명 : 환불 절차를 수행하다
// 호출방법 : m_pDevCmn->fnIMB_MyBiUnloadEXE(m_pDevCmn->ParamLoadEXE.RandomNum, sizeof(m_pDevCmn->ParamLoadEXE))
// 파라미터 : 없음
// 리턴코드 : fnMCU_ICSendData()에 대한 결과
// 주의사항 : 
// 히스토리 : 2004/07/07, 경남은행, 01.00.00
//////////////////////////////////////////////////////////////////////////////
int	CDevCmn::fnIMB_MyBiUnloadEXE(LPBYTE lpRandomNum, int RandomLen)
{
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiUnloadEXE()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_MYBI;								// 0x30
	ICSendData[2] = IC_SFC_MYBIUNLOADEXE;						// 0x04
	memcpy(ICSendData + IC_FCMS_LEN, lpRandomNum, RandomLen);
	
	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + RandomLen);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);

MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiUnloadEXE():return[%d]", nReturn);
	return nReturn;
}

//////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnIMB_MyBiIEPLock()
// 설    명 : 사용자카드를 Lock 하거나 Lock상태를 해제한다
// 파라미터 : 없음
// 리턴코드 : 
// 주의사항 : 
// 히스토리 : 2004/07/07, 경남은행, 01.00.00
//////////////////////////////////////////////////////////////////////////////
int	CDevCmn::fnIMB_MyBiIEPLock(BYTE * m_Data, INT Len)
{
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiIEPLock()");
	
	int		nReturn = FALSE;
	CString strICSendData("");
	
	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료
	
	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_MYBI;								// 0x30
	ICSendData[2] = IC_SFC_MYBIIEPLOCK;							// 0x05
	memcpy(ICSendData + IC_FCMS_LEN, m_Data, Len);
	
	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + Len);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiIEPLock():return[%d]", nReturn);
	return nReturn;
}

//////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnIMB_AnalRecvData()
// 설    명 : 수신분석
// 파라미터 : 없음
// 리턴코드 : 
// 주의사항 : 
// 히스토리 : 2004/07/07, 경남은행, 01.00.00
//////////////////////////////////////////////////////////////////////////////
int CDevCmn::fnIMB_AnalRecvData(int nCmdType)
{
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData()");

	int		nReturn(T_ERROR);
	CString	strICRecvData("");
	int		nRecvLen;

	int		nOffset = 0, nLength = 0, nCount = 0;

	memset( ICRecvData, 0, sizeof(ICRecvData) );
	strICRecvData = fstrMCU_GetICData();
	nRecvLen = __min( sizeof(ICRecvData) * 2, strICRecvData.GetLength() );

	MakePack( strICRecvData.GetBuffer(0), ICRecvData, nRecvLen );

HexDumpAnal(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData:ICRecvData", ICRecvData, nRecvLen);

//	Error코드 확인필요!! 2004.07.07
	if ( fnICA_CheckFCCode() != TRUE )
	{
		fnICA_SetFCError(ICRecvData);
		fnICA_AnalyzeFCError();
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(...) FCError Return!!!!");	
		return nReturn;
	}

	nOffset  += IC_FCMS_LEN;									// 업무 코드
	nRecvLen /= 2;												// Pack Data Len

	switch (nCmdType)
	{
	case MYBI_PROC_CARDINFOREAD :								// 마이비 카드정보읽기
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_CARDINFOREAD)");

		memcpy(MybiInfo.LOCKStatus, &ICRecvData[nOffset], sizeof(MybiInfo));
		nReturn = T_OK;

		break;

	case MYBI_PROC_LOADINIT :									// 마이비 보충/환불 1차
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_LOADINIT)");

		memcpy( MybiLoadInit.BeforeType, &ICRecvData[nOffset], sizeof(MybiLoadInit));
		nReturn = T_OK;

		break;

	case MYBI_PROC_LOADEXE :									// 마이비 보충2차
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_LOADEXE)");

		nReturn = T_OK;

		break;

	case MYBI_PROC_UNLOADEXE :									// 마이비 환불2차
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_UNLOADEXE)");
		
		memcpy( rtnSign3, &ICRecvData[nOffset], sizeof(rtnSign3) );
		nReturn = T_OK;

		break;

	case MYBI_PROC_F204 :										// 마이비 로그정보읽기
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_F204)");

	break;

	case MYBI_PROC_F205 :										// 마이비 로그정보읽기
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_F205)");

	break;

	case MYBI_PROC_F206 :										// 마이비 로그정보읽기
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_F206)");

	break;

	case MYBI_PROC_LOCKCHECK:									// 마이비 UNLOCK REQUEST
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_LOCKCHECK)");

		return T_OK;
		break;

	default :
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(default)");
		break;
	}

MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData():return(nReturn[%d])", nReturn);
	return nReturn;
}


int	CDevCmn::fnIMB_BasicInfoAnal()
{
	int		nReturn = T_ERROR;
	int		nOffset = 0, nCount = 0;

	// Cardholder Data Template Tag
	if ( FIC_TAG_CHDATATEMP == ICRecvData[nOffset] )
	{
		nOffset += 2;
	}
	
	// Cardholder Name Tag
	if ( memcmp( FIC_TAG_CHNAME, ICRecvData + nOffset,
		sizeof(FIC_TAG_CHNAME) ) == 0 )
	{
		nOffset += sizeof(FIC_TAG_CHNAME);
		
		// 카드 소지자 이름 길이
		FinanceICInfo.CardHolderNameLen = ICRecvData[nOffset];
		nOffset++;
		
		// 카드 소지자 이름
		memcpy( FinanceICInfo.CardHolderName, ICRecvData + nOffset,
												FinanceICInfo.CardHolderNameLen );
		nOffset += FinanceICInfo.CardHolderNameLen;
	}
	
	// 실명 ID Tag
	if ( FIC_TAG_REALNAME == ICRecvData[nOffset] )
	{
		nOffset++;
		
		FinanceICInfo.RealNameNOLen = ICRecvData[nOffset];		// 실명번호 길이
		nOffset++;
		
		FinanceICInfo.RealNameType = ICRecvData[nOffset];		// 실명구분
		nOffset++;
		
		if (FinanceICInfo.RealNameNOLen >= 1)
		{
			memcpy( FinanceICInfo.RealNameNO, ICRecvData + nOffset,
				FinanceICInfo.RealNameNOLen - 1 );					// 실명번호(실명번호 길이는 실명구분 1Bytes를 포함)
			nOffset += FinanceICInfo.RealNameNOLen - 1;
		}
	}

	// 만기일 Tag
	if ( memcmp( FIC_TAG_EXPIREDAY, ICRecvData + nOffset,
		sizeof(FIC_TAG_EXPIREDAY) ) == 0 )
	{
		nOffset += sizeof(FIC_TAG_EXPIREDAY);
		
		FinanceICInfo.ExpireDayLen = ICRecvData[nOffset];		// 만기일 길이
		nOffset++;
		
		// 만기일(BCD -> String)
		MakeUnPack( &ICRecvData[nOffset], FinanceICInfo.ExpireDay, FinanceICInfo.ExpireDayLen );
		nOffset += FinanceICInfo.ExpireDayLen;
MsgDump(TRACE_CODE_MODE, "Log",  "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) 만기일 = %s", FinanceICInfo.ExpireDay);
	}


	// 알고리즘 식별코드 Tag
	if ( FIC_TAG_ALGOCODE == ICRecvData[nOffset] )
	{
		nOffset += sizeof(BYTE) + sizeof(BYTE);					// 알고리즘 식별코드 길이
		
		FinanceICInfo.AlgorithmCode = ICRecvData[nOffset];		// 알고리즘 식별코드
		nOffset++;
	}

	// 발행기관 식별코드 Tag(필수)
	if ( FIC_TAG_ISSURECODE != ICRecvData[nOffset] )
		return nReturn;											// Data Error
	nOffset++;

	FinanceICInfo.IssureCodeLen = ICRecvData[nOffset];			// 발행기관 식별코드 길이
	nOffset++;

	// 발행기관 식별코드(BCD -> String)
	MakeUnPack( &ICRecvData[nOffset], FinanceICInfo.IssureCode, FinanceICInfo.IssureCodeLen );
	nOffset += FinanceICInfo.IssureCodeLen;
MsgDump(TRACE_CODE_MODE, "Log",  "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) 발행기관 식별코드 = %s", FinanceICInfo.IssureCode);

	// 카드 일련번호 Tag(필수)
	if ( FIC_TAG_CSN != ICRecvData[nOffset] )
		return nReturn;											// Data Error
	nOffset += sizeof(BYTE) + sizeof(BYTE);						// 카드 일련번호 길이

	// 카드 일련번호
	memcpy( FinanceICInfo.CSN, ICRecvData + nOffset, FIC_CSN_LEN );
	nOffset += FIC_CSN_LEN;

	// 카드 일련번호 for Screen(BCD -> String)
	MakeUnPack( FinanceICInfo.CSN, FinanceICInfo.szCSN, FIC_CSN_LEN );
MsgDump(TRACE_CODE_MODE, "Log",  "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) CSN = %s", FinanceICInfo.szCSN);

	return nReturn;
}

