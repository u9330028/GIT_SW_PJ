/////////////////////////////////////////////////////////////////////////////
//	DevFinanceIC.cpp : implementation file
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

//******************************************************************************
// File Name         : DevFinanceIC.cpp
// Description       : IC Card APDU interface class
// Author            : Han Su Woong
// Initial Date      : 2004.01.27
// Revision History  : 
// Notes             : (C)Nautilus Hyosung Inc. 2004 All Rights Reserved.
//******************************************************************************

////////////////////////////////////////////////////////////////////////////////
//  Operation Function
////////////////////////////////////////////////////////////////////////////////

/*==============================================================================
* Function Name : fnICA_InitProcess()
* Description   : IC 거래 초기화 함수(Buffer Clear)
* Parameters    : (NONE)
* Return        : (NONE)
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
void CDevCmn::fnICA_InitProcess()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_InitProcess()");

	// Buffer Clear
	memset( m_cbFCError, NULL,      IC_ERRORRESP_LEN );
	memset( m_szErrCode, NULL, sizeof(m_szErrCode) );
	memset(  m_szErrMsg, NULL,  sizeof(m_szErrMsg) );
}

/*==============================================================================
* Function Name : fnICA_ConstructAPDU() 
* Description   : IC Card로 보낼 표준 APDU Message를 조립하는 함수
* Parameters    : lpSendData : Send Message Buffer
*                 ISOCase    : ISO-7816 APDU Command Type
*                 CLA        : Class Byte
*                 INS        : Instruction Byte
*                 P1         : Parameter 1
*                 P2         : Parameter 2
*                 Lc         : Length of Command. Case2에서는 Le
*                 pData      : Data
*                 Le         : Length of Response
* Return        : Success : ICData Length		Fail : FALSE
* Release       : Ver 1.0
* Notes			: 2004.01.14 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_ConstructAPDU(LPBYTE lpSendData, BYTE ISOCase, BYTE CLA,
				BYTE INS, BYTE P1, BYTE P2, BYTE Lc, LPBYTE pData, BYTE Le)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ConstructAPDU()");

	int nRet(0);

	memset( lpSendData, NULL, IC_APDU_LEN );		// Buffer Initialize
	
	// CLA, INS, P1, P2
	lpSendData[nRet] = CLA;
	lpSendData[nRet + 1] = INS;
	lpSendData[nRet + 2] = P1;
	lpSendData[nRet + 3] = P2;
	nRet += sizeof(BYTE) * 4;

	switch (ISOCase)
	{		
	case ISO_7816_CASE1 :							// CLA, INS, P1, P2
		break ;
	
	case ISO_7816_CASE2 :							// CLA, INS, P1, P2, LE
		lpSendData[nRet] = Lc;
		nRet += sizeof(BYTE);
		break ;
	
	case ISO_7816_CASE3 :							// CLA, INS, P1, P2, LC, Data
		lpSendData[nRet] = Lc;
		nRet += sizeof(BYTE);

		memcpy( lpSendData + nRet, pData, Lc );
		nRet += Lc;

		break ;
	
	case ISO_7816_CASE4 :							// CLA, INS, P1, P2, LC, Data, LE
		lpSendData[nRet] = Lc;
		nRet += sizeof(BYTE);

		memcpy( lpSendData + nRet, pData, Lc );
		nRet += Lc;

		lpSendData[nRet] = Le;
		nRet += sizeof(BYTE);

		break ;
	
	case ISO_7816_CASEFREE :						// Non ISO format
		break ;

	default :
		break ;
		}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ConstructAPDU():return[%d]", nRet);

	return nRet;
}


/*==============================================================================
* Function Name : fnICA_ConstructTLV() 
* Description   : Data 객체를 표준 TLV 형태로 만들어 조립하는 함수(2바이트 Tag용)
* Parameters    : lpDestData : Output Buffer
*                 lpTag      : Tag Data
*                 TagLen     : Length of lpTag
*                 lpVaule    : Vaule
*                 ValueLen   : Length of lpVaule
* Return        : Success : lpDestData Length		Fail : FALSE
* Release       : Ver 1.0
* Notes			: 2004.07.01 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_ConstructTLV(LPBYTE lpDestData, LPBYTE lpTag, int TagLen,
								LPBYTE lpVaule, int ValueLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ConstructTLV()");

	int		nOffset(0);

	if ( TagLen > IC_SW_LEN || ValueLen >= IC_APDU_LEN )
		return FALSE;								// Data Error

	// Tag
	if ( TagLen > 1 )
	{
		memcpy( lpDestData, lpTag, TagLen );
		nOffset += TagLen;
	}
	else
	{
		return FALSE;
	}

	// Length
	lpDestData[nOffset] = (BYTE)ValueLen;
	nOffset++;

	// Value
	if ( ValueLen == 0 )
	{
		return nOffset;
	}
	else if ( ValueLen == 1 )
	{
		lpDestData[nOffset] = lpVaule[0];
		nOffset++;
	}
	else if ( ValueLen > 1 )
	{
		memcpy( lpDestData + nOffset, lpVaule, ValueLen );
		nOffset += ValueLen;
	}
	else
	{
		return FALSE;
	}

	return nOffset;
}

/*==============================================================================
* Function Name : fnICA_ConstructTLV() 
* Description   : Data 객체를 표준 TLV 형태로 만들어 조립하는 함수(1바이트 Tag용)
* Parameters    : lpDestData : Output Buffer
*                 bTag       : Tag Data
*                 lpVaule    : Vaule
*                 ValueLen   : Length of lpVaule
* Return        : Success : lpDestData Length		Fail : FALSE
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_ConstructTLV(LPBYTE lpDestData, BYTE bTag, LPBYTE lpVaule,
								 int ValueLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ConstructTLV()");

	int		nOffset(0);

	if ( ValueLen >= IC_APDU_LEN )
		return FALSE;								// Data Error

	// Tag
	lpDestData[0] = bTag;
	nOffset++;

	// Length
	lpDestData[nOffset] = (BYTE)ValueLen;
	nOffset++;

	// Value
	if ( ValueLen == 0 )
	{
		return nOffset;
	}
	else if ( ValueLen == 1 )
	{
		lpDestData[nOffset] = lpVaule[0];
		nOffset++;
	}
	else if ( ValueLen > 1 )
	{
		memcpy( lpDestData + nOffset, lpVaule, ValueLen );
		nOffset += ValueLen;
	}
	else
	{
		return FALSE;
	}

	return nOffset;
}

/*==============================================================================
* Function Name : fnICA_ISOIn()
* Description   : IC Card로 표준 APDU 명령을 전송하는 함수(응답 데이터 없음)
* Parameters    : lpAPDU   : 표준 APDU 명령
*                 nAPDULen : Length of lpSendData
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.01.15 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_ISOIn(LPBYTE lpAPDU, int nAPDULen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ISOIn()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));		// IC전송자료

	if (IfmDirect) //#NICCM01
	{
		memcpy( ICSendData, lpAPDU, nAPDULen );
		strICSendData = MakeUnPack(ICSendData, nAPDULen);
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_ISOIN, strICSendData);
		
	}
	else
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;						// 0x30
		ICSendData[1] = IC_MFC_BASIC;					// 0x01
		ICSendData[2] = IC_SFC_ISOIN;					// 0x02
		memcpy( ICSendData + IC_FCMS_LEN, lpAPDU, nAPDULen );

		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + nAPDULen);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ISOIn(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnICA_ISOOut()
* Description   : IC Card로 표준 APDU 명령을 전송하는 함수(응답 데이터 있음)
* Parameters    : lpAPDU   : 표준 APDU 명령
*                 nAPDULen : Length of lpSendData
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.01.15 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_ISOOut(LPBYTE lpAPDU, int nAPDULen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ISOOut()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));		// IC전송자료

	if (IfmDirect) //#NICCM01
	{
		memcpy( ICSendData, lpAPDU, nAPDULen );
		strICSendData = MakeUnPack(ICSendData, nAPDULen);
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_ISOOUT, strICSendData);
		
	}
	else
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;						// 0x30
		ICSendData[1] = IC_MFC_BASIC;					// 0x01
		ICSendData[2] = IC_SFC_ISOOUT;					// 0x03
		memcpy( ICSendData + IC_FCMS_LEN, lpAPDU, nAPDULen );

		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + nAPDULen);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ISOOut(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnICA_SelectApplet()
* Description   : IC 카드 종류를 통해서 Applet을 선택하는 함수
* Parameters    : nICKind : 선택하려는 IC Card 종류
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_SelectApplet(int nICKind)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_SelectApplet(ICKind=%d)", nICKind);

	int nReturn(FALSE);
	BYTE cbAID[IC_AID_LEN];

	memset( cbAID, NULL, IC_AID_LEN );

	FICDFType = nICKind;										// SET Select DF Type 

	switch (nICKind)
	{
	case TRAN_IC_FINANCEIC :									// 금융 IC Applet
		memcpy( cbAID, FIC_AID, FIC_AID_LEN );
		nReturn = fnICA_SelectApplet( cbAID, FIC_AID_LEN );
		break;
	
	case TRAN_IC_KCASH :										// K-CASH Applet
		memcpy( cbAID, FIC_KCASH_AID, FIC_AID_LEN );
		nReturn = fnICA_SelectApplet( cbAID, FIC_AID_LEN );
		break;
	
	case TRAN_IC_MYBI :											// MYBI Applet
		memcpy( cbAID, FIC_MYBI_AID, FIC_AID_LEN );
		nReturn = fnICA_SelectApplet( cbAID, FIC_AID_LEN );
		break;

	case TRAN_IC_MONDEX1 :										// Mondex Applet
		// Rserved for Future Use
		break;

	case TRAN_IC_VCASH :										// V-CASH Applet
		// Rserved for Future Use
		break;
	
	case TRAN_IC_AUTHEN :										// 인증서 Applet
		// Rserved for Future Use
		break;

	case TRAN_IC_ELECPB	:										// 전자통장 Applet //#NICCM01
		memcpy( cbAID, ELECPB_AID, ELECPB_AID_LEN );
		nReturn = fnICA_SelectApplet( cbAID, ELECPB_AID_LEN );
		break;

	case TRAN_IC_HIPASS:										// Hi-Pass접촉식충전개발 #N0266
		memcpy( cbAID, HIPASS_AID, sizeof(HIPASS_AID) );
		nReturn = fnICA_SelectApplet( cbAID, FIC_AID_LEN );
		break;
	default:
		break;
	}

	return nReturn;
}

/*==============================================================================
* Function Name : fnICA_SelectApplet()
* Description   : AID를 이용하여 IC 카드 내의 Applet을 직접 선택하는 함수
* Parameters    : lpAID   : Application ID
*                 nAIDLen : Length of lpAID
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_SelectApplet(LPBYTE lpAID, int nAIDLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_SelectApplet()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));		// IC전송자료
	
	if (IfmDirect)  //#NICCM01
	{
		// Message 조립
		memcpy( ICSendData, lpAID, FIC_AID_LEN );
		strICSendData = MakeUnPack(ICSendData, FIC_AID_LEN);
		
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_SELECTAPPLET, strICSendData);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_SelectApplet(...):IfmDirect_return[%d]", nReturn);
		
	}
	else
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;						// 0x30
		ICSendData[1] = IC_MFC_BASIC;					// 0x01
		ICSendData[2] = IC_SFC_SELECTAPP;               // 0x30
		memcpy( ICSendData + IC_FCMS_LEN, lpAID, nAIDLen );

		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + nAIDLen);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_SelectApplet(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnICA_ICConfirmType()
* Description   : AID를 이용하여 IC 카드 내의 Application 종류를 구별하는 함수
* Parameters    : BankType : 사용하는 은행 타입. 검사할 카드 종류를 결정.
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.01 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_ICConfirmType(BYTE BankType)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ICConfirmType()");

	int		nReturn(FALSE);
	CString strICSendData("");

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC전송자료


	if (IfmDirect)												// //#NICCM01	ICCM 모듈 적용
	{
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_ICTYPECONFIRM, "0");		// 0:일반은행, 1:국민은행
	}
	else
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;						// 0x30
		ICSendData[1] = IC_MFC_BASIC;					// 0x01
		ICSendData[2] = IC_SFC_ICCONFIRMTYPE;			// 0x31
		ICSendData[3] = BankType;

		strICSendData = MakeUnPack( ICSendData, IC_FCMS_LEN + sizeof(BYTE) );
		nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ICConfirmType(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnICA_ISO7816WarningProc()
* Description   : 재송신 처리. 61xx, 6Cxx.
* Parameters    : lpData   : 이전 응답 데이터
*                 nDataLen : Length of lpData
* Return        : Success : TRUE				Fail : FALSE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_ISO7816WarningProc(LPBYTE lpData, int &nDataLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ISO7816WarningProc()");

	////////////////////////////////////////////////////////////////////////////
	// APDU Element
	////////////////////////////////////////////////////////////////////////////
	BYTE	bSendData[BASEBUFFSIZE], bAPDUData[IC_APDU_LEN];
	int		nSendLen(0), nAPDULen(0), nRecvLen(0);
	BYTE	bLc;

	memset( bSendData, NULL, sizeof(bSendData) );

	// Format Ex : 0x3100006108
	if ( lpData[nDataLen - IC_SW_LEN] == ISO7816_SW_BYTES_REMAINING[0] )	// 0x61
	{
		bLc = lpData[nDataLen - 1];
		
		// Message Construction
		nAPDULen = fnICA_ConstructAPDU
						( bAPDUData,
						  ISO_7816_CASE2,
						  CLA_7816_STANDARD,		// 0x00
						  INS_7816_GET_RESPONSE,	// 0xC0
						  P1P2_7816_ZERO,			// 0x00
						  P1P2_7816_ZERO,			// 0x00
						  bLc );					// 0xXX
	}
	else if ( lpData[nDataLen - IC_SW_LEN] == ISO7816_SW_CORRECT_LENGTH[0] )	// 0x6C
	{
		// Message Construction : 이전 CLA + INS + P1 + P2 + xx
		memset( bAPDUData, NULL, IC_APDU_LEN );
		memcpy( bAPDUData, ICSendData + IC_FCMS_LEN, 5 );
		bAPDUData[IC_FCMS_LEN + 4] = lpData[nDataLen - 1];
		nAPDULen = 5;
	}

	////////////////////////////////////////////////////////////////////////////
	//  ISOOUT Chip Send
	////////////////////////////////////////////////////////////////////////////
	int		nReturn = 0, nErrorReturn = 9999;
	CString strICSendData("");

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC전송자료

	if (IfmDirect) //#NICCM01
	{
		memcpy( ICSendData, bAPDUData, nAPDULen );
		strICSendData = MakeUnPack(ICSendData, nAPDULen);
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_ISOOUT, strICSendData);
		
	}
	else
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;						// 0x30
		ICSendData[1] = IC_MFC_BASIC;					// 0x01
		ICSendData[2] = IC_SFC_ISOOUT;					// 0x03
		memcpy( ICSendData + IC_FCMS_LEN, bAPDUData, nAPDULen );

		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + nAPDULen);
		fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
		if ( !fnAPL_CheckDeviceAction(DEV_MCU) )
			return nErrorReturn;
	}

	////////////////////////////////////////////////////////////////////////////
	//  Chip Receive
	////////////////////////////////////////////////////////////////////////////
	CString strICRecvData("");

	memset(lpData, 0, sizeof(ICRecvData));
	strICRecvData = fstrMCU_GetICData();
	nRecvLen = __min( sizeof(ICRecvData) * 2, strICRecvData.GetLength() );
	MakePack(strICRecvData.GetBuffer(0), lpData, nRecvLen);

	nDataLen = nRecvLen / 2;

/////////////////////////////////////////////////////////////////////////////
//	if ((!memcmp(lpData, IC_RECV_OK, 3))	||					// 정상
//		(!memcmp(lpData, IC_RECV_OK2, 3))	)
//		;
//	else														// 비정상
//	{
//		nReturn = nErrorReturn;									// 기타오류
//		for (int i = 0; i < itemof(FCErrMsg); i++)				// 2004.01.27
//		{
//			if (!memcmp(ICRecvData, &FCErrMsg[i].FC, IC_ERRORRESP_LEN))
//			{
//				nReturn = i + 1;
//				break;
//			}
//		}
//	}
/////////////////////////////////////////////////////////////////////////////
	nReturn = fnICA_CheckFCCode();
	if ( nReturn != TRUE )
	{
		fnICA_SetFCError(lpData);
		fnICA_AnalyzeFCError();
		return nReturn;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ISO7816WarningProc(%d):return", nReturn);
	return nReturn;
}


/*==============================================================================
* Function Name : fnICA_CheckFCCode()
* Description   : Chip Recive Data에 대한 FC Code 점검
* Parameters    : (NONE)
* Return        : Success : TRUE				Fail : FALSE
* Release       : Ver 1.0
* Notes			: 2004.01.26 우리판 수정 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_CheckFCCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_CheckFCCode()");
	int nReturn(FALSE);

	if ( ICRecvData[0] == IC_RECV_OK[0] )			// 정상응답
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ldc_fnICA_CheckFCCode():true");
		nReturn = TRUE;
	}
	else if ( ICRecvData[0] == IC_RECV_NG )			// 오류응답
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ldc_fnICA_CheckFCCode():false");
		nReturn = FALSE;
	}
	else if ( memcmp( ICRecvData, IC_RECV_OK2, IC_FCMS_LEN ) == 0 )	// 정상응답2
	{
		nReturn = TRUE;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_CheckFCCode():return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnICA_AnalyzeFCError()
* Description   : 업무 코드 에러시 에러를 분석하는 함수
* Parameters    : (NONE)
* Return        : Error Code NO. Default -1(No Match).
* Release       : Ver 1.0
* Notes			: 2003.12.26 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_AnalyzeFCError()
{

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_AnalyzeFCError()");
	int nReturn(-1);

	// Error Code & Error Message Search
	for ( int i = 0; i < FC_MAX_ERRCNT; i++ )
	{
		if ( memcmp( &m_cbFCError[3], &FCErrMsg[i].Err2, 1 ) == 0 )
		{
			if ( memcmp( &m_cbFCError[4], &FCErrMsg[i].Err3, 1 ) == 0 )
			{
				nReturn = i;
				break;
			}
		}
	}

	// Error Code & Error Message Setting
	if ( nReturn != -1 )
	{
	//	m_szErrCode = unpack 처리
		wsprintf( m_szErrMsg, FCErrMsg[nReturn].ICErrGuide );
	}
	else											// 기본 Error Setting
	{
		wsprintf( m_szErrCode, "FFFFFFFF" );
		wsprintf( m_szErrMsg, "Unknown Error" );
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_AnalyzeFCError():return[%d]", nReturn);

	return nReturn;
}

/*==============================================================================
* Function Name : fnICA_AnalyzeISOError()
* Description   : ISOIN, ISOOUT 사용시 표준 APDU Error를 점검하는 함수
* Parameters    : lpData   : 이전 응답 데이터
*                 nDataLen : Length of lpData
* Return        : No Error : TRUE				Error : FALSE
* Release       : Ver 1.0
* Notes			: 2003.12.26 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_AnalyzeISOError(LPBYTE lpData, int &nDataLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_AnalyzeISOError()");
	
	int		nReturn(0);

	// Get Response, Incorrect Length 처리
	if ( lpData[nDataLen - IC_SW_LEN] == ISO7816_SW_BYTES_REMAINING[0] ||	// 0x61
		 lpData[nDataLen - IC_SW_LEN] == ISO7816_SW_CORRECT_LENGTH[0]  )	// 0x6C
	{
		if ( fnICA_ISO7816WarningProc(lpData, nDataLen) == TRUE)//0 )	// #N0266 에러처리 확인필요
		{
			nReturn = TRUE;
		}
	}

	if ( nReturn != TRUE )
	{
		// ISO 표준 에러 처리 : 현재는 Data Error로 처리(0xD2 0x42 0x46 0x46)
		memcpy( m_cbFCError, "\xD2\x42\x46\x46", IC_ERRORRESP_LEN );
		fnICA_AnalyzeFCError();
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_AnalyzeISOError(...):return[%d]", nReturn);
	return nReturn;
}

////////////////////////////////////////////////////////////////////////////////
//  Get/Set Function
////////////////////////////////////////////////////////////////////////////////

/*==============================================================================
* Function Name : fnICA_GetFCError()
* Description   : 업무코드 Error Get
* Parameters    : (NONE)
* Return        : m_cbFCError
* Release       : Ver 1.0
* Notes			: 2003.12.26 Han Su Woong
* ============================================================================*/
LPBYTE CDevCmn::fnICA_GetFCError()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_GetFCError()");
	return m_cbFCError;
}

/*==============================================================================
* Function Name : fnICA_SetFCError()
* Description   : 업무코드 Error Set
* Parameters    : lpErrData : m_cbFCError
* Return        : (NONE)
* Release       : Ver 1.0
* Notes			: 2003.12.26 Han Su Woong
* ============================================================================*/
void CDevCmn::fnICA_SetFCError(LPBYTE lpErrData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_SetFCError()");
	CString strErrorCode = "";
	if (IfmDirect)	//#NICCM01	ICCM 모듈 적용
	{
		strErrorCode = m_pICCM->ICCMGetErrorCode();				// d24331fe
		MakePack((void*)(LPCTSTR)strErrorCode, (void*)m_cbFCError, strErrorCode.GetLength());
	}
	else
		memcpy( m_cbFCError, lpErrData, IC_ERRORRESP_LEN );

}

/*==============================================================================
* Function Name : fnICA_GetErrCode()
* Description   : Error Code Get for Screen Display
* Parameters    : (NONE)
* Return        : m_szErrCode
* Release       : Ver 1.0
* Notes			: 2003.12.26 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnICA_GetErrCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_GetErrCode():return[%s]", m_szErrCode);
	return m_szErrCode;
}

/*==============================================================================
* Function Name : fnICA_SetErrCode()
* Description   : Error Code Set for Screen Display
* Parameters    : strErrCode : m_szErrCode
* Return        : (NONE)
* Release       : Ver 1.0
* Notes			: 2003.12.26 Han Su Woong
* ============================================================================*/
void CDevCmn::fnICA_SetErrCode(LPSTR strErrCode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_SetErrCode:ErrCode[%s]", strErrCode);

	int nLen = strlen(strErrCode);
	if ( nLen <= sizeof(m_szErrCode) )
	{
		strcpy( m_szErrCode, strErrCode );
	}
}

/*==============================================================================
* Function Name : fnICA_GetErrMsg()
* Description   : Error Message Get for Screen Display
* Parameters    : (NONE)
* Return        : m_szErrMsg
* Release       : Ver 1.0
* Notes			: 2003.12.26 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnICA_GetErrMsg()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_GetErrMsg():return[%s]", m_szErrMsg);
	return m_szErrMsg;
}

/*==============================================================================
* Function Name : fnICA_SetErrMsg()
* Description   : Error Message Set for Screen Display
* Parameters    : strErrMsg : m_szErrMsg
* Return        : (NONE)
* Release       : Ver 1.0
* Notes			: 2003.12.26 Han Su Woong
* ============================================================================*/
void CDevCmn::fnICA_SetErrMsg(LPSTR strErrMsg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_SetErrMsg:ErrMsg[%s]", strErrMsg);
	int nLen = strlen(strErrMsg);
	if ( nLen <= sizeof(m_szErrMsg) )
	{
		strcpy( m_szErrMsg, strErrMsg );
	}
}

////////////////////////////////////////////////////////////////////////////////
//  Operation Function
////////////////////////////////////////////////////////////////////////////////

/*==============================================================================
* Function Name : fnFIC_InitProcess()
* Description   : 금융 IC 거래 초기화 함수(Buffer Clear)
* Parameters    : (NONE)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_InitProcess()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_InitProcess()");

	FICCardType = FIC_NONE;										// 카드無
	FICDFType = 0;												// IC DF미선택
	FICAppletType = 0;											// IC카드 APPLET정보
	FICAccountCount = 0;										// 금융IC 계좌갯수
	FICEPBAccountCount = 0;										// 금융IC 전자통장 계좌갯수
	FICEPBAccountTotalCount = 0;								// 금융IC 전자통장 총계좌갯수

	memset( FICAccount,  NULL, sizeof(FICAccount)  );
	memset( FICBankCode, NULL, sizeof(FICBankCode) );

	memset( FinanceICInfo.CAN, NULL, sizeof(FinanceICInfo) );
	memset( &FinanceICISO3Info[0], NULL, sizeof(FinanceICISO3Info) );

	memset(&EPBAccountInfo, 0, sizeof(EPBAccountInfo));	// e-모든통장 계좌정보
	memset(&FinanceICEPBDisp, 0, sizeof(FinanceICEPBDisp));
	memset(&FinanceICEPBDisp2, 0, sizeof(FinanceICEPBDisp2));

	return T_OK;
}

/*==============================================================================
* Function Name : fnFIC_GetICType()
* Description   : IC TYPE 읽기
* Parameters    : 
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.11.03 X MAN
* ============================================================================*/
int CDevCmn::fnFIC_GetICType()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetICType()");

	int		nReturn = FALSE;
	BYTE	btData  = 0x00;

	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	if (IfmDirect)									// #NICCM01	ICCM 모듈 적용
	{
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_ICTYPECONFIRM, "0");		// 0:일반은행, 1:국민은행
	}
	else
	{
	
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;									// 0x30
		ICSendData[1] = IC_MFC_BASIC;								// 0x01
		ICSendData[2] = IC_SFC_ICCONFIRMTYPE;						// 0x31
		ICSendData[3] = btData;										// 0x00 : 일반은행 0x10 : 국민은행
		
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + 1);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);

	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetICType():return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_ReadBasicInfo()
* Description   : 금융 IC Applet의 기본 정보를 조회하는 함수
* Parameters    : (NONE)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_ReadBasicInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadBasicInfo()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));		// IC전송자료

	if (IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, FIC_READBASICINFO);		
	else
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;						// 0x30
		ICSendData[1] = IC_MFC_CC;						// 0x07
		ICSendData[2] = IC_SFC_READBASICINFO;			// 0x20

		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadBasicInfo():return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_ReadAccountInfo()
* Description   : 금융 IC Applet의 계좌 정보를 조회하는 함수
* Parameters    : bIndex : 0x00(1 ~ 10), 0x01(11 ~ 20)...
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_ReadAccountInfo(BYTE bIndex)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadAccountInfo(Index=%d)", bIndex);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));		// IC전송자료

	if (IfmDirect) //#NICCM01
	{
		ICSendData[0] = bIndex;							// Index
		strICSendData = MakeUnPack(ICSendData, sizeof(BYTE));
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, FIC_READACCOUNTINFO, strICSendData);
	}
	else
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;						// 0x30
		ICSendData[1] = IC_MFC_CC;						// 0x07
		ICSendData[2] = IC_SFC_READACCOUNTINFO;			// 0x21
		ICSendData[3] = bIndex;							// Index

		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + sizeof(BYTE));
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadAccountInfo(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_ReadDualInfo()
* Description   : 금융 IC Applet의 기본 정보 + 계좌 정보를 조회하는 함수
* Parameters    : bIndex : 0x00(1 ~ 10), 0x01(11 ~ 20)...
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.01.12 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_ReadDualInfo(BYTE bIndex)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadDualInfo(Index=%d)", bIndex);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));		// IC전송자료

	if (IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T6, FIC_READDUALINFO);
	else
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;						// 0x30
		ICSendData[1] = IC_MFC_CC;						// 0x07
		ICSendData[2] = IC_SFC_READDUALINFO;			// 0x22
		ICSendData[3] = bIndex;							// Index

		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + sizeof(BYTE));
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadDualInfo(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_GetEncipher()
* Description   : 금융 IC Get Encipher
* Parameters    : szPassWord : Account Password(4Bytes)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_GetEncipher(char* szPassWord, int PassWordSize)
{

CString strLog;
strLog.Format( "%4.4s", szPassWord );
strLog.Delete( strLog.GetLength() - 2, 2 );
strLog = strLog + "**";
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetEncipher(PassWord=%s)", (LPSTR)(LPCTSTR)strLog);

	int		nReturn = FALSE;
	int		nSendLen, nPassLen;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	if (IfmDirect) //#NICCM01
		nSendLen = 0;
	else
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;									// 0x30
		ICSendData[1] = IC_MFC_CC;									// 0x07
		ICSendData[2] = IC_SFC_GETENCIPHER;							// 0x23

		nSendLen = IC_FCMS_LEN;
	}

//t	nPassLen = strlen(szPassWord);								// 길이오류 
	nPassLen = PassWordSize;

	// Parameter Length(0x14)
	ICSendData[nSendLen] = FIC_RANDOM_LEN + nPassLen;
	nSendLen++;

	// Terminal Random(16Bytes)
	fnFIC_MakeRandom(FinanceICInfo.TerminalRandom, FIC_RANDOM_LEN);

	memcpy( ICSendData + nSendLen, FinanceICInfo.TerminalRandom, FIC_RANDOM_LEN );
	nSendLen += FIC_RANDOM_LEN;

	// 입력받은 Account Password(4Bytes)
	memcpy( ICSendData + nSendLen, szPassWord, nPassLen );
	nSendLen += nPassLen;

	strICSendData = MakeUnPack(ICSendData, nSendLen);

	if (IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, FIC_GETENCIPHER, strICSendData);
	else
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetEncipher(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_GetENCData()
* Description   : 금융 IC Get ENCData(MyBi). Define 정리 후 Tran단에 위치함이 옮음.
* Parameters    : bIndex     : Get ENCData1 = 0x01, Get ENCData2 = 0x02
*                 szPassWord : 고객이 입력한 계좌 비밀번호
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.01.15 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_GetENCData(BYTE bIndex, char* szPassWord)
{
CString strLog;
strLog.Format( "%s", szPassWord );
strLog.Delete( strLog.GetLength() - 2, 2 );
strLog = strLog + "**";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetENCData(Index=%d, PassWord=%s)", bIndex, (LPSTR)(LPCTSTR)strLog);

	int		nReturn = FALSE;
	BYTE	bRandom[FIC_ENCDATA_LEN];							// 암호화에 사용할 Random Buffer
	BYTE	bAPDUData[IC_APDU_LEN];
	int		nAPDULen;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	//**********************************************************************
	//  Random Set
	//**********************************************************************
	memset( bRandom, NULL, FIC_ENCDATA_LEN );
	if ( bIndex == 0x01 )										// Get ENCData 1
	{
		//  Terminal Random MSB(8) XOR Terminal Random LSB(8) XOR PassWord(8)
		fnFIC_MakeRandom(FinanceICInfo.TerminalRandom, FIC_RANDOM_LEN);
		fnFIC_XORData( szPassWord, FinanceICInfo.TerminalRandom, bRandom );
	}
	else														// Get ENCData 2
	{
		//  Card Random MSB(8) XOR Card Random LSB(8) XOR PassWord(8)
		fnFIC_XORData( szPassWord, FinanceICInfo.CardRandom, bRandom );
	}

strLog.Empty();
MakeUnPack( bRandom, strLog, FIC_ENCDATA_LEN );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetENCData() Random = %s", (LPSTR)(LPCTSTR)strLog);

	//**********************************************************************
	//  Command Construction
	//**********************************************************************
	nAPDULen = fnICA_ConstructAPDU
					( bAPDUData,
					  ISO_7816_CASE3,
					  CLA_USER_STANDARD,						// 0x90
					  INS_USER_GET_ENCDATA,						// 0x8E
					  P1P2_7816_ZERO,							// 0x00
					  P1P2_7816_ZERO,							// 0x00
					  FIC_ENCDATA_LEN,							// 0x08
					  bRandom ) ;								// Random Data

	nReturn = fnICA_ISOIn( bAPDUData, nAPDULen );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetENCData(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_GetChallenge()
* Description   : 금융 IC GetChallenge 함수(MyBi)
*                 GetChallenge : Update Record 등을 하기 위해서 IC Card의
*                 Session Key를 얻어오는 ISO-7816-4 표준 명령이다.
* Parameters    : (None)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.01.15 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_GetChallenge()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetChallenge()");

	int		nReturn = FALSE;
	BYTE	bAPDUData[IC_APDU_LEN];
	int		nAPDULen;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	//**********************************************************************
	//  Command Construction
	//**********************************************************************
	nAPDULen = fnICA_ConstructAPDU
					( bAPDUData,
					  ISO_7816_CASE2,
					  CLA_7816_STANDARD,						// 0x00
					  INS_7816_GET_CHALLANGE,					// 0x84
					  P1P2_7816_ZERO,							// 0x00
					  P1P2_7816_ZERO,							// 0x00
					  FIC_RANDOM_LEN );							// 0x10

	nReturn = fnICA_ISOOut( bAPDUData, nAPDULen );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetChallenge():return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_CheckPIN()
* Description   : 금융 IC PIN Verify
* Parameters    : szPIN : 금융 IC PIN
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_CheckPIN(LPCTSTR szPassWord)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_CheckPIN(%s)", szPassWord);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	if (IfmDirect) //#NICCM01
	{
		strICSendData = szPassWord;
		
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, FIC_CHECKPIN, strICSendData);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_CheckPIN(...):IfmDirect_return[%d]", nReturn);
		
	}
	else
	{
		ICSendData[0] = IC_FC_MAIN;						// 0x30
		ICSendData[1] = IC_MFC_CC;						// 0x07
		ICSendData[2] = IC_SFC_VERIFYPIN;				// 0x24

		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szPassWord;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	
MsgDumpAnal(TRACE_CODE_MODE, "Log", "fnFIC_CheckPIN(...):return[%d]", nReturn);
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_CheckPIN(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_ReadPan()
* Description   : 금융 IC PAN File REad
* Parameters    : szPIN : 금융 IC PIN
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_ReadPan(LPCTSTR szPassWord)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadPan(%s)", szPassWord);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));		// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_VALUE;					// 0x06
	ICSendData[2] = IC_SFC_OPENREADPAN;				// 0x20

	if (IfmDirect) //#NICCM01
	{
		strICSendData = szPassWord;
		
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_OPENREADPAN, strICSendData);
		
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szPassWord;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadPan(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_ReadBAmount()
* Description   : 금융 IC JAN File REad
* Parameters    : szPIN : 금융 IC PIN
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_ReadBAmount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadBAmount()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));		// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_STDEP;					// 0x02
	ICSendData[2] = IC_SFC_OPENREADJAN;				// 0x20

	if (IfmDirect) //#NICCM01
	{
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_OPENREADBAMOUNT);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadBAmount(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_ChangePin()
* Description   : 금융 IC JAN File REad
* Parameters    : 변경전암호8+변경후암호8
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int	CDevCmn::fnFIC_ChangePin(LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ChangePin(szICSendData[%s])", szICSendData);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_PIN;									// 0x09
	ICSendData[2] = IC_SFC_OPENPINCHANGE;						// 0x20

	if (IfmDirect) //#NICCM01
	{
		strICSendData = szICSendData;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_PINCHANGE2,strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szICSendData;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ChangePin(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_OptioalChangePin()
* Description   : 금융 IC JAN File REad
* Parameters    : IC TYPE(OPEN,CLOSE)+변경전암호8+변경후암호8
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int	CDevCmn::fnFIC_OptionalChangePin(int nOpenCloseType, BYTE bChangeAppletType, BYTE bChangeKeyPinType, LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_OptionalChangePin(nOpenCloseType[%d], bChangeAppletType[%x], bChangeKeyPinType[%x], szICSendData[%s])"
																			, nOpenCloseType, bChangeAppletType, bChangeKeyPinType, szICSendData);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	if (nOpenCloseType == 1)									// 폐쇄형
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;								// 0x30
		ICSendData[1] = IC_MFC_PIN;								// 0x09
		ICSendData[2] = IC_SFC_OPTIONALCLOSEPINCHANGE;			// 0x22
		ICSendData[3] = 0x00;									// 0x00 ALL PIN CHANGE
		ICSendData[4] = bChangeKeyPinType;						// KEY변경(0x00),PIN변경(Ox01)
	}
	else														// 개방형
	{
		// Message 조립
		ICSendData[0] = IC_FC_MAIN;								// 0x30
		ICSendData[1] = IC_MFC_PIN;								// 0x09
		ICSendData[2] = IC_SFC_OPTIONALOPENPINCHANGE;			// 0x21
		ICSendData[3] = bChangeAppletType;						// OPTION PIN-Ox01:금융IC,0x02:K-CASH,0x04:인증서(서명용), 0x08:인증서(분배용)
		ICSendData[4] = bChangeKeyPinType;						// KEY변경(0x00),PIN변경(Ox01)
	}


	if (IfmDirect)												// #NICCM01	ICCM 모듈 적용
	{
		strICSendData = szICSendData;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_OPENPINCHANGE,strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + 2) + szICSendData; // #NICCM01	ICCM 모듈 적용-01
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_OptionalChangePin(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_ReadTrafficInfo()
* Description   : 금융 IC 소지자정보조회
* Parameters    : 
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_ReadTrafficInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadTrafficInfo()");

	int nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_STDEP;								// 0x02
	ICSendData[2] = IC_SFC_OPENTRAFFICINFO;						// 0x21

	if (IfmDirect) // #NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_OPENREADTRAFFICINFO);
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadTrafficInfo():return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_PrepareUnLoadRcvIEP()
* Description   : 금융 IC 환불복구전처리
* Parameters    : 
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_PrepareUnLoadRcvIEP(LPCTSTR szPassWord)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_PrepareUnLoadRcvIEP()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_VALUE;								// 0x06
	ICSendData[2] = IC_SFC_OPENPRERCVUNLOAD;					// 0x21


	if (IfmDirect) // #NICCM01
	{
		strICSendData = szPassWord;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_OPENPREPAREUNLOADRCVIEP,strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szPassWord;
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_PrepareUnLoadRcvIEP():return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_XORData()
* Description   : 금융 IC GetENCData를 위한 XOR Data를 구하는 함수(MyBi)
* Parameters    : szPassWord : 고객이 입력한 계좌 비밀번호
*                 lpSource   : XOR 하기 위한 Seed Data(16Bytes)
*                 lpResult   : XOR 한 결과 Data(8Bytes)
* Return        : Length of lpResult
* Release       : Ver 1.0
* Notes			: 2004.01.14 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_XORData(char* szPassWord, LPBYTE lpSource, LPBYTE lpResult)
{
CString strLog;
strLog.Format( "%s", szPassWord );
strLog.Delete( strLog.GetLength() - 2, 2 );
strLog = strLog + "**";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_XORData(PassWord=%s)", (LPSTR)(LPCTSTR)strLog);

	int		i, nPassLen;
	BYTE	bMSB[FIC_ENCDATA_LEN], bLSB[FIC_ENCDATA_LEN];
	BYTE	bPassWord[FIC_ENCDATA_LEN];

	// 계좌 비밀번호 Set
	nPassLen = ( strlen(szPassWord) > FIC_ENCDATA_LEN ) ?
				FIC_ENCDATA_LEN : strlen(szPassWord);
	memset( bPassWord, NULL, FIC_ENCDATA_LEN );
	memcpy( bPassWord, szPassWord, __min(nPassLen, 5));				

	// XOR Data Set : MSB(8) XOR LSB(8) XOR PassWord(8)
	memcpy( bMSB, lpSource, FIC_ENCDATA_LEN );
	memcpy( bLSB, lpSource + FIC_ENCDATA_LEN, FIC_ENCDATA_LEN );
	
	for ( i = 0; i < FIC_ENCDATA_LEN; i++ )
	{
		lpResult[i] = bMSB[i] ^ bLSB[i] ^ bPassWord[i];
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_XORData(...):return[%d]", FIC_ENCDATA_LEN);
	return FIC_ENCDATA_LEN;
}

/*==============================================================================
* Function Name : fnFIC_AnalRecvData()
* Description   : 금융 IC Chip Receive Data 해석하고 버퍼에 저장하는 함수
* Parameters    : nCmdType : 분석할 응답 메시지의 종류
*                 +---------------- ------+------------------------------------+
*				  | 7816_CHECK            | ISO 7816 표준 에러 Check           |
*				  | FINANCEIC_PROC_XXXX   | 각 거래 Flow에 따른 처리           |
*				  +-----------------------+------------------------------------+
* Return        : Success : TRUE				Fail : FALSE
* Release       : Ver 1.0
* Notes			: 2004.01.08 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_AnalRecvData(int nCmdType)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(CmdType[%d])", nCmdType);

	int		nOffset(0), nLength(0), nCount(0);
	int		i(0),j(0);
	int		nEpbLen1(0),nEpbLen2(0);
	CString strLog;

	int		nReturn(0), nErrorReturn(9999);
	CString	strICRecvData("");
	int		nRecvLen;
//	BYTE	tmpICRecvData[BASEBUFFSIZE];
	int		nAlgoCodeValueError = FALSE;	// #NICCM01
		
	memset(ICRecvData, 0, sizeof(ICRecvData));
	strICRecvData = fstrMCU_GetICData();
	nRecvLen = __min( sizeof(ICRecvData) * 2, strICRecvData.GetLength() );
	MakePack(strICRecvData.GetBuffer(0), ICRecvData, nRecvLen);

	if ((!memcmp(ICRecvData, IC_RECV_OK, 3))	||				// 정상
		(!memcmp(ICRecvData, IC_RECV_OK2, 3))	)
		;
	else														// 비정상
	{
		fnICA_SetFCError(ICRecvData); // #NICCM01
				
		nReturn = nErrorReturn;									// 기타오류
		for (int i = 0; i < itemof(FCErrMsg); i++)				// 2004.01.27
		{
			if (!memcmp(ICRecvData, &FCErrMsg[i].FC, IC_ERRORRESP_LEN))
			{
				nReturn = i + 1;
				break;
			}
		}
	}
/////////////////////////////////////////////////////////////////////////////

	if (nReturn)												// 오류응답 검지시
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FICDFType[%d]) NG Return[%d]", FICDFType, nReturn);
		// AP B/D 제거 후 오류 응답은 D2436285+Data 로 옴
		if(IfmDirect) // #NICCM01
		{
			if(strICRecvData.Left(4).CompareNoCase("D2436285") == 0)
				nReturn = 29;
		}
																// Exception-1 : 정상응답처리
		if ((nCmdType  == FINANCEIC_PROC_SELECTAPPLICATION)	&&	// SelectApplet인경우
			(FICDFType == TRAN_IC_KCASH)					&&	// DF == 전자화폐
			(nReturn   == 29)								)	// Resp == (D2 42 62 85)
		{
			nOffset += 1;										// 오류응답은 정상응답보다 1Byte크다
			nReturn = 0;										// 오류없음설정
		}
		else													// Exception상황이 아닌경우
		{
			fnICA_SetErrCode((char *)&ICRecvData[4]);
			return nReturn;										// NG검증시 Error Return
		}
	}
/////////////////////////////////////////////////////////////////////////////

	nOffset  += IC_FCMS_LEN;									// 업무 코드
	nRecvLen /= 2;												// Pack Data Len

	/*==============================================================================*/
	// 금융IC거래인경우에는 분석후 정보를 FinanceICInfo구조체에 그 값을 저장하지만
	// 전자화페 거래인경우에는 분석후 정보를 KCashInfo구조체에 그 값을 저장하도록 한다
	/*==============================================================================*/
	switch (nCmdType)
	{
	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_GETICTYPE:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(카드타입) FINANCEIC_PROC_GETICTYPE");
		memcpy( &ICTypeInfo.ICTypeConfirm, &ICRecvData[nOffset], FIC_ICTYPE_LEN );
		nOffset += FIC_ICTYPE_LEN;

		break;

	case FINANCEIC_PROC_SELECTAPPLICATION :						// Select Application
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION)");

		if (!FICDFType)											// Select된 DF를 알수 없을때..
			return nErrorReturn;
		
		if (FICDFType == TRAN_IC_FINANCEIC)						// SELECT 금융IC
		{
			// FCI Template Tag(필수)
			if ( FIC_MYBI_TAG_FCI != ICRecvData[nOffset] )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset += 1;										// Tag Length
			nOffset += 1;										// Data Length

			// DF Name Tag(필수)
			if ( FIC_MYBI_TAG_DFNAME != ICRecvData[nOffset] )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset++;											// DFNAME Tag Length
			nLength = ICRecvData[nOffset];						// DF Name Length
			nOffset++;											// DF NAME Data Length
			nOffset += nLength;									// Add DF Name Data Length

			// FCI Proprietary Template Tag(필수)
			if ( FIC_MYBI_TAG_PROP != ICRecvData[nOffset] )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset += 1;										// FCI Proprietary Template Tag Length
			nOffset += 1;										// FCI Proprietary Template Data Length

			// Application Label Tag
			if ( FIC_MYBI_TAG_LABEL == ICRecvData[nOffset] )
			{
				nOffset++;										// Label Tag Length

				// Application Label Length
				FinanceICInfo.FCI_AppLabelLen = ICRecvData[nOffset];
				nOffset++;

				// Application Label
				memcpy( FinanceICInfo.FCI_AppLabel, &ICRecvData[nOffset],
						__min(FinanceICInfo.FCI_AppLabelLen, sizeof(FinanceICInfo.FCI_AppLabel)));
				nOffset += FinanceICInfo.FCI_AppLabelLen;
			}

			// 파일정보 Tag(필수)
			if ( memcmp( FIC_MYBI_TAG_FILEINFO, &ICRecvData[nOffset],
						 sizeof(FIC_MYBI_TAG_FILEINFO) ) != 0 )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset += 2;										// Tag Length
			nOffset++;											// 파일정보 Length

			memcpy(FinanceICInfo.FCI_FInformation, &ICRecvData[nOffset], 8);


			// 표준화 버전
			FinanceICInfo.FCI_StandardVersion = ICRecvData[nOffset];
			nOffset += sizeof(BYTE);

			// File Key Version
			FinanceICInfo.FCI_FileKeyVersion = ICRecvData[nOffset];
			nOffset += sizeof(BYTE);

			break;
		}
		else
		if (FICDFType == TRAN_IC_KCASH)							// SELECT 전자화폐
		{
			if (ICRecvData[nOffset] != FIC_KCASH_TAG_FCI)		// FCI Templete Code Check
				return nErrorReturn;
			nOffset += 1;

//t			if (ICRecvData[nOffset] != 0x1F)					// FCI Data Length Check Skip
//t				return nErrorReturn;
			nOffset += 1;

			if (ICRecvData[nOffset] != FIC_KCASH_TAG_CARDID)	// FCI 발급자ID Check
				return nErrorReturn;
			nOffset += 1;
			if (ICRecvData[nOffset] != 0x10)					// FCI 발급자ID Length Check
				return nErrorReturn;
			nOffset += 1;
			nOffset += 5;										// FCI 발급자ID : 5Byte Skip
			memcpy(KCashInfo.CardNum, &ICRecvData[nOffset], 8);	// FCI 발급자ID중 카드번호
			nOffset += 11;										// 카드번호(8) + Dummy(3)

			if (memcmp(&ICRecvData[nOffset], FIC_KCASH_TAG_MAKEDAY, 2) != 0)
				return nErrorReturn;									// 발급일Check
			nOffset += 2;										// 전자화폐 발급일 Tag Length
			if (ICRecvData[nOffset] != 0x04)					// 전자화폐 발급일 Length Check
				return nErrorReturn;
			nOffset += 1;										// 전자화폐 발급일 DataLen Length
			nOffset += 4;										// 전자화폐 발급일 Data Length

			if (ICRecvData[nOffset] != FIC_KCASH_TAG_CARDINFO)	// FCI 전자화폐정보 Tag Check
				return nErrorReturn;
			nOffset += 1;
			if (ICRecvData[nOffset] != 0x04)					// 전자화폐정보 Length Check
				return nErrorReturn;
			nOffset += 1;										// 전자화폐정보 DataLen Length
			memcpy(KCashInfo.CardInform, &ICRecvData[nOffset], 4);
			nOffset += 4;										// 전자화폐정보(카드구분) 
		}
		break;

	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_READBASICINFO :							// 기본정보 조회
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO)");
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

			FinanceICInfo.RealNameNOLen = ICRecvData[nOffset];	// 실명번호 길이
			nOffset++;

			FinanceICInfo.RealNameType = ICRecvData[nOffset];	// 실명구분
			nOffset++;

			if (FinanceICInfo.RealNameNOLen >= 1)	
			{
				memcpy( FinanceICInfo.RealNameNO, ICRecvData + nOffset,
						FinanceICInfo.RealNameNOLen - 1 );			// 실명번호(실명번호 길이는 실명구분 1Bytes를 포함)
				nOffset += FinanceICInfo.RealNameNOLen - 1;
			}
		}

		// 만기일 Tag
		if ( memcmp( FIC_TAG_EXPIREDAY, ICRecvData + nOffset,
					 sizeof(FIC_TAG_EXPIREDAY) ) == 0 )
		{
			nOffset += sizeof(FIC_TAG_EXPIREDAY);

			FinanceICInfo.ExpireDayLen = ICRecvData[nOffset];	// 만기일 길이
			nOffset++;

			// 만기일(BCD -> String)
			MakeUnPack( &ICRecvData[nOffset], FinanceICInfo.ExpireDay, FinanceICInfo.ExpireDayLen );
			nOffset += FinanceICInfo.ExpireDayLen;
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) 만기일 = %s", FinanceICInfo.ExpireDay);
		}


		// 알고리즘 식별코드 Tag
		if ( FIC_TAG_ALGOCODE == ICRecvData[nOffset] )
		{
			nOffset += sizeof(BYTE) + sizeof(BYTE);				// 알고리즘 식별코드 길이
			
			FinanceICInfo.AlgorithmCode = ICRecvData[nOffset];	// 알고리즘 식별코드
			nOffset++;
		}
		
		// 발행기관 식별코드 Tag(필수)
		if ( FIC_TAG_ISSURECODE != ICRecvData[nOffset] )
			return nErrorReturn;								// Data Error
		nOffset++;

		FinanceICInfo.IssureCodeLen = ICRecvData[nOffset];		// 발행기관 식별코드 길이
		nOffset++;
		
		// 발행기관 식별코드(BCD -> String)
		MakeUnPack( &ICRecvData[nOffset], FinanceICInfo.IssureCode, FinanceICInfo.IssureCodeLen );
		nOffset += FinanceICInfo.IssureCodeLen;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) 발행기관 식별코드 = %s", FinanceICInfo.IssureCode);

		// 카드 일련번호 Tag(필수)
		if ( FIC_TAG_CSN != ICRecvData[nOffset] )
			return nErrorReturn;								// Data Error
		nOffset += sizeof(BYTE) + sizeof(BYTE);					// 카드 일련번호 길이

		// 카드 일련번호
		memcpy( FinanceICInfo.CSN, ICRecvData + nOffset, FIC_CSN_LEN );
		nOffset += FIC_CSN_LEN;

		// 카드 일련번호 for Screen(BCD -> String)
		MakeUnPack( FinanceICInfo.CSN, FinanceICInfo.szCSN, FIC_CSN_LEN );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) CSN = %s", FinanceICInfo.szCSN);
		break;

	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_READACCOUNTINFO :						// 계좌정보 조회
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READACCOUNTINFO)");
		
		nOffset++;												// 계좌정보 Command Index
		
		FICAccountCount = ICRecvData[nOffset];					// 계좌 갯수
		nOffset++;

		// Total Size Check : 업무코드(3) + Send Index(1) + 계좌갯수(1)
		//                  + 계좌갯수 * ( 계좌 정보 Tag + 계좌정보 길이 + 계좌정보 )
		nLength = IC_FCMS_LEN + 2
				  + FICAccountCount * ( 2 + FIC_ACCOUNTINFO_LEN );
		if ( nRecvLen != nLength )
			return nErrorReturn;								// Data Error

		// 계좌정보 Set
		// 무조건 카피해서는 안된다. NULL 계좌 정보가 있을 수 있다.
		nCount = 0;
		for ( i = 0; i < FICAccountCount; i++ ) // #NICCM01
		{
			// 계좌 정보 Tag(필수)
			if ( FIC_TAG_ACCOUNTINFO != ICRecvData[nOffset] )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset++;

			nOffset++;											// 계좌 정보 크기(0x3E). 검증할 필요 없다고 판단.
			
			// 계좌 정보
			memcpy( &FinanceICISO3Info[nCount], ICRecvData + nOffset,
					FIC_ACCOUNTINFO_LEN );
			nOffset += FIC_ACCOUNTINFO_LEN;

			// 계좌정보 검증. 계좌 종류 판별 AP 반환 부분에서 필요시 한다.
			if ( FinanceICISO3Info[nCount].AccountState != 0x00 )
			{
				// 정상계좌에 한해서 화면출력및 거래처리를 수행하도록 한다.
				// 2004.02.27 전산부 장호길과장요청
				if (FinanceICISO3Info[nCount].AccountState == FIC_ACCOUNT_NORMAL)
				{
					nCount++;
					MakeUnPack( &FinanceICISO3Info[nCount-1], strLog, FIC_ACCOUNTINFO_LEN );
				}
				else
				{
					MakeUnPack( &FinanceICISO3Info[nCount], strLog, FIC_ACCOUNTINFO_LEN );
					strLog = "[정상계좌아님]" + strLog;
				}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READACCOUNTINFO) 계좌 %d = %s", nCount, (LPSTR)(LPCTSTR)strLog);
			}
		}
		FICAccountCount = nCount;								// 실제 계좌의 수

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READACCOUNTINFO) FICAccountCount = %d", FICAccountCount);

		fnFIC_SetFICDispInfo();									// 화면에 Format을 세팅. 은행 사양에 따라 달라짐

		break;
	
	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_READDUALINFO :							// 개인정보 + 계좌정보 조회
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READDUALINFO)");

		//**********************************************************************
		//  BasicInfo Part
		//**********************************************************************
		// 개인정보 길이 Set
		nLength = ICRecvData[nOffset];
		nOffset++;

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

			FinanceICInfo.RealNameNOLen = ICRecvData[nOffset];	// 실명번호 길이
			nOffset++;

			FinanceICInfo.RealNameType = ICRecvData[nOffset];	// 실명구분
			nOffset++;

			if (FinanceICInfo.RealNameNOLen >= 1)	
			{
				memcpy( FinanceICInfo.RealNameNO, ICRecvData + nOffset,
						FinanceICInfo.RealNameNOLen - 1 );			// 실명번호(실명번호 길이는 실명구분 1Bytes를 포함)
				nOffset += FinanceICInfo.RealNameNOLen - 1;
			}
		}

		// 만기일 Tag
		if ( memcmp( FIC_TAG_EXPIREDAY, ICRecvData + nOffset,
					 sizeof(FIC_TAG_EXPIREDAY) ) == 0 )
		{
			nOffset += sizeof(FIC_TAG_EXPIREDAY);

			FinanceICInfo.ExpireDayLen = ICRecvData[nOffset];	// 만기일 길이
			nOffset++;

			// 만기일(BCD -> String)
			for ( i = 0; i < FinanceICInfo.ExpireDayLen; i++ )
			{
				sprintf( FinanceICInfo.ExpireDay + 2 * i,
						 "%2.2X", ICRecvData[nOffset + i] );
			}
			nOffset += FinanceICInfo.ExpireDayLen;
		}

		// 알고리즘 식별코드 Tag
		if ( FIC_TAG_ALGOCODE == ICRecvData[nOffset] )
		{
			nOffset += sizeof(BYTE) + sizeof(BYTE);				// 알고리즘 식별코드 길이
				
			// // #NICCM01 V04-02-00-#0a : 공통장애대책(No.93) ─────────────────────
			if ( FIC_TAG_ISSURECODE != ICRecvData[nOffset] )	
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FIC_TAG_ISSURECODE != ICRecvData[nOffset]");
			
				FinanceICInfo.AlgorithmCode = ICRecvData[nOffset];	// 알고리즘 식별코드
				nOffset++;
			}
			else
			{
				FinanceICInfo.AlgorithmCode = 0x00;
				nAlgoCodeValueError = TRUE;
			}
			// ───────────────────────────────────────	

		}


		// 발행기관 식별코드 Tag(필수)
		if ( FIC_TAG_ISSURECODE != ICRecvData[nOffset] )
			return nErrorReturn;								// Data Error
		nOffset++;

		FinanceICInfo.IssureCodeLen = ICRecvData[nOffset];		// 발행기관 식별코드 길이
		nOffset++;
		
		// 발행기관 식별코드(BCD -> String)
		for ( i = 0; i < FinanceICInfo.IssureCodeLen; i++ )
		{
			sprintf( FinanceICInfo.IssureCode + 2 * i,
					 "%02X", ICRecvData[nOffset + i] );
		}
		nOffset += FinanceICInfo.IssureCodeLen;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READDUALINFO) 발행기관 식별코드 = %s", FinanceICInfo.IssureCode);

		// 카드 일련번호 Tag(필수)
		if ( FIC_TAG_CSN != ICRecvData[nOffset] )
			return nErrorReturn;								// Data Error
		nOffset += sizeof(BYTE) + sizeof(BYTE);					// 카드 일련번호 길이

		// 카드 일련번호
		memcpy( FinanceICInfo.CSN, ICRecvData + nOffset, FIC_CSN_LEN );
		nOffset += FIC_CSN_LEN;

		// 카드 일련번호 for Screen(BCD -> String)
		for ( i = 0; i < FIC_CSN_LEN; i++ )
		{
			sprintf( FinanceICInfo.szCSN + 2 * i, "%02X", FinanceICInfo.CSN[i] );
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READDUALINFO) CSN = %s", FinanceICInfo.szCSN);

		// #NICCM01 공통장애대책(No.93) ─────────────────────
		if (nAlgoCodeValueError)									
			nOffset++;
		// ──────────────────────────────────

		//**********************************************************************
		//  AccountInfo Part
		//**********************************************************************
		nOffset++;												// 계좌정보 Command Index
		
		FICAccountCount = ICRecvData[nOffset];					// 계좌 갯수
		nOffset++;

	
		// #NICCM01공통장애대책(No.95) ────────────────────── 
		// Total Size Check : 업무코드(3) + 기본정보(N) + Send Index(1) + 계좌갯수(1)
		//                  + 계좌갯수 * ( 계좌 정보 Tag + 계좌정보 길이 + 계좌정보 )
// 		nLength += IC_FCMS_LEN + 3 +
// 				   FICAccountCount * ( 2 + FIC_ACCOUNTINFO_LEN );
// 		if ( nRecvLen != nLength )
// 		{
// 			return nErrorReturn;								// Data Error
// 		}
		// ────────────────────────────────

		// 계좌정보 Set
		// 무조건 카피해서는 안된다. NULL 계좌 정보가 있을 수 있다.
		nCount = 0;
		for ( i = 0; i < FICAccountCount; i++ )
		{

			// 공통장애대책(No.94) 계좌 정보 Tag(필수) 이미되어 있네 V07-00-03-#08
// 계좌 정보 Tag(필수) #NICCM01 
//			if ( FIC_TAG_ACCOUNTINFO != ICRecvData[nOffset] )
//			{
//				return nErrorReturn;							// Data Error
//			}

			nOffset++;

																// 계좌길이 데이타
			nOffset++;											// 계좌 정보 크기(0x3E). 검증할 필요 없다고 판단.
			
			// 계좌 정보
			memcpy( &FinanceICISO3Info[nCount], ICRecvData + nOffset,
					FIC_ACCOUNTINFO_LEN );
			nOffset += FIC_ACCOUNTINFO_LEN;

			// 계좌정보 검증. 계좌 종류 판별 AP 반환 부분에서 필요시 한다.
			if ( FinanceICISO3Info[nCount].AccountState != 0x00 )
			{
				// 정상계좌에 한해서 화면출력및 거래처리를 수행하도록 한다.
				// 2004.02.27 전산부 장호길과장요청
				if (FinanceICISO3Info[nCount].AccountState == FIC_ACCOUNT_NORMAL)
				{
					nCount++;
					MakeUnPack( &FinanceICISO3Info[nCount-1], strLog, FIC_ACCOUNTINFO_LEN );
				}
				else
				{
					MakeUnPack( &FinanceICISO3Info[nCount], strLog, FIC_ACCOUNTINFO_LEN );
					strLog = "[정상계좌아님]" + strLog;
				}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READACCOUNTINFO) 계좌 %d = %s", nCount-1, (LPSTR)(LPCTSTR)strLog);
			}
		}
		FICAccountCount = nCount;								// 실제 계좌의 수

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READACCOUNTINFO) FICAccountCount = %d", FICAccountCount);

		fnFIC_SetFICDispInfo();									// 화면에 Format을 세팅. 은행 사양에 따라 달라짐

		break;
	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_GETDATACSN :							// Get Data CSN. 한수웅 수정코드 2004.09.16
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_GETDATACSN)");

		// APDU Error Check
		if ( memcmp( &ICRecvData[nRecvLen - IC_SW_LEN], ISO7816_SW_NO_ERROR,
				sizeof(IC_SW_LEN) ) != 0 )						// 0x90 0x00
		{
			// APDU Error, Get Response(61xx), Incorrect Length(6Cxx) 처리
			if ( fnICA_AnalyzeISOError(ICRecvData, nRecvLen) == FALSE )
				return nErrorReturn;
		}

		// Format Ex : 0x310000CA58982701020304059000
		// Total Size Check
		if ( nRecvLen != IC_FCMS_LEN        +					// 업무 코드
						 sizeof(BYTE)       +					// 0xCA
						 FIC_CSN_LEN		+					// CSN Data
						 IC_SW_LEN          )					// Status Word
		{
			return nErrorReturn;								// Data Error
		}

		nOffset++;												// Add 0xCA Size

		// 카드 일련번호
		memcpy( FinanceICInfo.CSN, ICRecvData + nOffset, FIC_CSN_LEN );
		nOffset += FIC_CSN_LEN;

		// 카드 일련번호 for Screen(BCD -> String)
		for ( i = 0; i < FIC_CSN_LEN; i++ )
		{
			sprintf( FinanceICInfo.szCSN + 2 * i, "%02X", FinanceICInfo.CSN[i] );
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_GETDATACSN) CSN = %s", FinanceICInfo.szCSN);

		break;

	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_GETENCIPHER :							// Get Encipher
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_GETENCIPHER)");
		
		// Total Size Check
		if ( nRecvLen != IC_FCMS_LEN		+					// 업무 코드
						 sizeof(BYTE)		+					// 응답 코드 길이
						 FIC_RANDOM_LEN		+					// Card Random
						 FIC_RANDOM_LEN )						// 암호화된 계좌 비밀번호
		{
			return nErrorReturn;								// Data Error
		}

		nOffset++;												// 응답 코드 길이

		// Card Random
		memcpy( FinanceICInfo.CardRandom, ICRecvData + nOffset, FIC_RANDOM_LEN );
		nOffset += FIC_RANDOM_LEN;
 
		// 암호화된 계좌 비밀번호
		memcpy( FinanceICInfo.CipherPW, ICRecvData + nOffset, FIC_RANDOM_LEN );
		nOffset += FIC_RANDOM_LEN;

		break;

	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_GETENCDATA1 :							// Get ENCData 1
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_GETENCDATA1)");

		// APDU Error Check
		if ( memcmp( &ICRecvData[nRecvLen - IC_SW_LEN], ISO7816_SW_NO_ERROR,
				sizeof(IC_SW_LEN) ) != 0 )						// 0x90 0x00
		{
			// APDU Error, Get Response(61xx), Incorrect Length(6Cxx) 처리
			if ( fnICA_AnalyzeISOError(ICRecvData, nRecvLen) == FALSE )
				return nErrorReturn;
		}

		switch(IdcVersion)
		{
		case EMV_BOARD:
			// Format Ex : 0x3100000BAD50951F77E4469000
			// Total Size Check
			if ( nRecvLen != IC_FCMS_LEN        +					// 업무 코드
							 FIC_ENCDATA_LEN	+					// ENCData 1
							 IC_SW_LEN          )					//	Status Word
			{
				return nErrorReturn;								// Data Error
			}
			break;
		case CMI_BOARD:
		default:
			// Format Ex : 0x310000C00BAD50951F77E4469000
			// Total Size Check
			if ( nRecvLen != IC_FCMS_LEN        +					// 업무 코드
							 sizeof(BYTE)       +					// 0xC0
							 FIC_ENCDATA_LEN	+					// ENCData 1
							 IC_SW_LEN          )					//	Status Word
			{
				return nErrorReturn;								// Data Error
			}

			nOffset++;												// Add 0xC0 Size
			break;
		}
																// ENCData 1
		memcpy( FinanceICInfo.ENCData1, ICRecvData + nOffset, FIC_ENCDATA_LEN );
		nOffset += FIC_ENCDATA_LEN;

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FinanceICInfo.ENCData1", FinanceICInfo.ENCData1, FIC_ENCDATA_LEN);
		break;

	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_GETCHALLENGE :							// Get Challenge
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_GETCHALLENGE)");
		
		// APDU Error Check
		if ( memcmp( &ICRecvData[nRecvLen - IC_SW_LEN], ISO7816_SW_NO_ERROR,
				sizeof(IC_SW_LEN) ) != 0 )						// 0x90 0x00
		{
			// APDU Error, Get Response(61xx), Incorrect Length(6Cxx) 처리
			if ( fnICA_AnalyzeISOError(ICRecvData, nRecvLen) == FALSE )
				return nErrorReturn;
		}

		switch(IdcVersion)
		{
		case EMV_BOARD:
			// Format Ex : 0x310000D8F3598E5F423FF7E20D9759173D41B49000
			// Total Size Check
			if ( nRecvLen != IC_FCMS_LEN	+						// 업무 코드
							 FIC_RANDOM_LEN +						// Card Random
							 IC_SW_LEN		)						// Status Word
			{
				return nErrorReturn;								// Data Error
			}
			break;
		case CMI_BOARD:
		default:
			// Format Ex : 0x31000084D8F3598E5F423FF7E20D9759173D41B49000
			// Total Size Check
			if ( nRecvLen != IC_FCMS_LEN	+						// 업무 코드
							 sizeof(BYTE)       +					// 0x84
							 FIC_RANDOM_LEN +						// Card Random
							 IC_SW_LEN		)						// Status Word
			{
				return nErrorReturn;								// Data Error
			}

			nOffset++;												// Add 0xC0 Size
			break;
		}
																// Card Random
		memcpy( FinanceICInfo.CardRandom, ICRecvData + nOffset, FIC_RANDOM_LEN );
		nOffset += FIC_RANDOM_LEN;

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FinanceICInfo.CardRandom", FinanceICInfo.CardRandom, FIC_RANDOM_LEN);
		break;

	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_GETENCDATA2 :							// Get ENCData 2
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_GETENCDATA2)");

		// APDU Error Check
		if ( memcmp( &ICRecvData[nRecvLen - IC_SW_LEN], ISO7816_SW_NO_ERROR,
				sizeof(IC_SW_LEN) ) != 0 )						// 0x90 0x00
		{
			// APDU Error, Get Response(61xx), Incorrect Length(6Cxx) 처리
			if ( fnICA_AnalyzeISOError(ICRecvData, nRecvLen) == FALSE )
				return nErrorReturn;
		}

		switch(IdcVersion)
		{
		case EMV_BOARD:
			// Format Ex : 0x310000035AAAD85E0228199000
			// Total Size Check
			if ( nRecvLen != IC_FCMS_LEN        +					// 업무 코드
							 FIC_ENCDATA_LEN	+					// ENCData 1
							 IC_SW_LEN          )					//	Status Word
			{
				return nErrorReturn;								// Data Error
			}
			break;
		case CMI_BOARD:
		default:
			// Format Ex : 0x310000C0035AAAD85E0228199000
			// Total Size Check
			if ( nRecvLen != IC_FCMS_LEN        +					// 업무 코드
							 sizeof(BYTE)       +					// 0xC0
							 FIC_ENCDATA_LEN	+					// ENCData 1
							 IC_SW_LEN          )					//	Status Word
			{
				return nErrorReturn;								// Data Error
			}

			nOffset++;												// Add 0xC0 Size
			break;
		}

		// ENCData 2
		memcpy( FinanceICInfo.ENCData2, ICRecvData + nOffset, FIC_ENCDATA_LEN );
		nOffset += FIC_ENCDATA_LEN;

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FinanceICInfo.ENCData2", FinanceICInfo.ENCData2, FIC_ENCDATA_LEN);
		break;

	/*==============================================================================*/
	// 금융IC거래인경우에는 분석후 정보를 FinanceICInfo구조체에 그 값을 저장하지만
	// 전자화페 거래인경우에는 분석후 정보를 KCashInfo구조체에 그 값을 저장하도록 한다
	/*==============================================================================*/
	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_CHECKPIN:
	case FINANCEIC_PROC_VERIFYTRANBIT:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(전자화폐)");
		break;

	case FINANCEIC_PROC_READPAN:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(전자화폐) FINANCEIC_PROC_READPAN");
		memcpy(KCashInfo.Account, &ICRecvData[IC_FCMS_LEN], 10);
		break;

	case FINANCEIC_PROC_READBAMOUNT:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(전자화폐) FINANCEIC_PROC_READBAMOUNT");
		memcpy(KCashInfo.Balance, &ICRecvData[3], 4);			// 잔액
		memcpy(KCashInfo.MaxLimitAmount, &ICRecvData[13], 4);	// 충전한도금액
		break;

	case FINANCEIC_PROC_READTRAFFICINFO:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(전자화폐) FINANCEIC_PROC_READTRAFFICINFO");
		memcpy(&KCashInfo.UserCode, &ICRecvData[3], 1);			// 소지자정보
		memcpy(KCashInfo.ValidDate, &ICRecvData[4], 2);			// 유효기간
		break;

	case FINANCEIC_PROC_PREUNLOADRCV:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(전자화폐) FINANCEIC_PROC_PREUNLOADRCV");
		memcpy(KCashInfo.Account, &ICRecvData[3], 10);			// PAN정보
		memcpy(KCashInfo.Balance, &ICRecvData[13], 4);			// 잔액	
		memcpy(KCashInfo.MaxLimitAmount, &ICRecvData[23], 4);	// 충전한도금액
		KCashInfo.LoadUnloadAlgorithmID			= ICRecvData[33];
		KCashInfo.LoadUnloadKeyVersion			= ICRecvData[34];
		memcpy(KCashInfo.LoadUnloadSerialNo,	&ICRecvData[35], 2);
		memcpy(KCashInfo.LoadUnloadRandomNum,	&ICRecvData[37], 8);
		memcpy(KCashInfo.LoadUnloadEPMakerID,	&ICRecvData[45], 3);
		memcpy(KCashInfo.LoadUnloadEPID,		&ICRecvData[48], 5);
		memcpy(KCashInfo.LoadUnloadSignature,	&ICRecvData[53], 4);
		memcpy(KCashInfo.LoadUnloadProof, &ICRecvData[57], 7);	// Proof
		break;

	case FINANCEIC_PROC_EPBREADACCOUNTINFO :					// 1~10 계좌정보 조회
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(전자통장): FINANCEIC_PROC_EPBREADACCOUNTINFO");
		nOffset++;												// 계좌정보 Command Index
		
		FICEPBAccountCount = ICRecvData[nOffset];				// 계좌 갯수
		nOffset++;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(전자통장): FICEPBAccountCount = %d", FICEPBAccountCount);

		// Total Size Check : 업무코드(3) + 1 + 계좌갯수(1) + 계좌갯수 * (계좌정보)
		nLength += IC_FCMS_LEN + 1 + 1 + FICEPBAccountCount * (1 + sizeof(FINANCEICEPBINFO));

		// 계좌정보 Set
		// 무조건 카피해서는 안된다. 없는 계좌도 정보가 있을 수 있다.
		nCount  = 0;

		for (i = 0; i < FICEPBAccountCount; i++)
		{
			nOffset++;
			
			// 계좌 정보
			memcpy(&EPBAccountInfo[nCount], ICRecvData + nOffset, sizeof(FINANCEICEPBINFO));
			nOffset += sizeof(FINANCEICEPBINFO);

			// 계좌정보 검증. 계좌 종류 판별 AP 반환 부분에서 필요시 한다.
			if (EPBAccountInfo[nCount].AccountState != 0x00)
			{
				nCount++;
			}
		}

		FICEPBAccountCount = nCount;								// 실제 계좌의 수

		fnFIC_SetEPBDispInfo();
		break;

	default :
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(default)");
		break;

	}

	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_MakeRandom()
* Description   : 금융 IC Random Data 생성 함수. 은행 스팩에 따름.
* Parameters    : lpSource : Buffer for Random Data
*                 nLen     : Length of lpSource
*                 Lv       : 0 : 0x00(16 Bytes). 랜덤 생성하지 않음
*                            1 : N Bytes의 Random 생성
*                            2 : N Bytes 자리 위치 Random 적용
*                            3 : 8 Bytes Key 값으로 XOR
* Return        : (None)
* Release       : Ver 1.0
* Notes			: 2004.01.19 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_MakeRandom(LPBYTE lpSource, int nLen, int Lv)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_MakeRandom()");

	memset( lpSource, NULL, nLen );

	if ( Lv == 0 )									// 랜덤 생성하지 않음
	{
		return T_OK;
	}

	srand( (unsigned)time(NULL) );

	BYTE bSeed;
	for ( int i = 0 ; i < nLen ; i++ )				// 입력된 자리수의 난수 발생
	{
		bSeed = rand() % 0xFF;
		lpSource[i] = bSeed;
	}

	if ( Lv == 2 )									// N Bytes 자리 위치 Random 적용
	{
	}

	if ( Lv == 3 )									// 8 Bytes Key 값으로 XOR
	{
		BYTE MKey[8] = { 0x43, 0x41, 0x50, 0x54, 0x41, 0x49, 0x4E, 0x20 };
		LPBYTE lpXORKey = new BYTE[nLen];
		memset( lpXORKey, NULL, nLen );

		( nLen >= sizeof(MKey) ) ? memcpy( lpXORKey, MKey, sizeof(MKey) ) :
								   memcpy( lpXORKey, MKey, sizeof(nLen) );
		
		for ( int i = 0 ; i < nLen ; i++ )
		{
			lpSource[i] = lpSource[i] ^ lpXORKey[i];
		}
		
		SAFE_DELETE_ARR(lpXORKey);
	}

	return T_OK;
}


////////////////////////////////////////////////////////////////////////////////
//  Get/Set Function
////////////////////////////////////////////////////////////////////////////////

/*==============================================================================
* Function Name : fnFIC_GetCardHolderName()
* Description   : 카드 소지자 이름 Get
* Parameters    : (NONE)
* Return        : FinanceICInfo.CardHolderName
* Release       : Ver 1.0
* Notes			: 2003.12.29 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnFIC_GetCardHolderName()
{
	return FinanceICInfo.CardHolderName;
}

/*==============================================================================
* Function Name : fnFIC_GetRealNameType()
* Description   : 실명구분 Get
* Parameters    : (NONE)
* Return        : FinanceICInfo.RealNameType
* Release       : Ver 1.0
* Notes			: 2003.12.29 Han Su Woong
* ============================================================================*/
BYTE CDevCmn::fnFIC_GetRealNameType()
{
	return FinanceICInfo.RealNameType;
}

/*==============================================================================
* Function Name : fnFIC_GetRealNameNO()
* Description   : 실명번호 Get
* Parameters    : (NONE)
* Return        : FinanceICInfo.RealNameNO
* Release       : Ver 1.0
* Notes			: 2003.12.29 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnFIC_GetRealNameNO()
{
	return FinanceICInfo.RealNameNO;
}

/*==============================================================================
* Function Name : fnFIC_GetExpireDay()
* Description   : 만기일 Get
* Parameters    : (NONE)
* Return        : FinanceICInfo.ExpireDay(Ex : "20991231")
* Release       : Ver 1.0
* Notes			: 2003.12.29 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnFIC_GetExpireDay()
{
	return FinanceICInfo.ExpireDay;
}

/*==============================================================================
* Function Name : fnFIC_GetAlgorithmCode()
* Description   : 알고리즘 식별코드 Get
* Parameters    : (NONE)
* Return        : FinanceICInfo.AlgorithmCode
* Release       : Ver 1.0
* Notes			: 2003.12.30 Han Su Woong
* ============================================================================*/
BYTE CDevCmn::fnFIC_GetAlgorithmCode()
{
	return FinanceICInfo.AlgorithmCode;
}

/*==============================================================================
* Function Name : fnFIC_GetIssureCode()
* Description   : 발행기관 식별코드 Get(계좌 발행기관이 아닌 카드 발행기관임)
* Parameters    : (NONE)
* Return        : FinanceICInfo.IssureCode(Ex : "0023")
* Release       : Ver 1.0
* Notes			: 2003.12.30 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnFIC_GetIssureCode()
{
	return FinanceICInfo.IssureCode;
}

/*==============================================================================
* Function Name : fnFIC_GetCSN()
* Description   : 카드 일련번호 Get
* Parameters    : (NONE)
* Return        : FinanceICInfo.szCSN(Ex : "0650070000000265")
* Release       : Ver 1.0
* Notes			: 2004.01.08 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnFIC_GetCSN()
{
	return FinanceICInfo.szCSN;
}

/*==============================================================================
* Function Name : fnFIC_GetAccountCnt()
* Description   : 금융 IC 계좌 갯수 Get
* Parameters    : (NONE)
* Return        : FICAccountCount
* Release       : Ver 1.0
* Notes			: 2003.12.30 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_GetAccountCnt()
{
	return FICAccountCount;
}

/*==============================================================================
* Function Name : fnFIC_GetAccountState()
* Description   : 금융 IC 계좌정보 파일 중 계좌상태 Get
* Parameters    : bIndex : 읽을 계좌의 Index
* Return        : Success : FinanceICISO3Info[Index].AccountState	Fail : NULL
* Release       : Ver 1.0
* Notes			: 2003.12.30 Han Su Woong
* ============================================================================*/
BYTE CDevCmn::fnFIC_GetAccountState(BYTE bIndex)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetAccountState(Index[%d])", bIndex);

	if ( bIndex > FICAccountCount || bIndex < 1 )
	{
		return NULL;
	}

	return FinanceICISO3Info[bIndex - 1].AccountState;
}

/*==============================================================================
* Function Name : fnFIC_GetAccountNum()
* Description   : 금융 IC 계좌정보 파일 중 계좌번호 Get For Screen.
* Parameters    : bIndex : 읽을 계좌의 Index
* Return        : Success : FICAccount					Fail : NULL
* Release       : Ver 1.0
* Notes			: 2004.01.08 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnFIC_GetAccountNum(BYTE bIndex)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetAccountNum(Index[%d])", bIndex);

	char	strTemp[120];
	int		nAccountStart, nAccountNOCnt;

	if ( bIndex > FICAccountCount || bIndex < 1 )
	{
		return "";
	}

	nAccountStart = FinanceICISO3Info[bIndex - 1].AccountStart;
	nAccountNOCnt = FinanceICISO3Info[bIndex - 1].AccountNOCnt;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetAccountNum() 계좌시작 위치 = %d", nAccountStart);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetAccountNum() 계좌번호 길이 = %d", nAccountNOCnt);

	// Byte->String
	memset( strTemp, NULL, sizeof(strTemp) );
	MakeUnPack( FinanceICISO3Info[bIndex - 1].ISO3, strTemp, FIC_ISO3_LEN, FALSE );

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetAccountNum() ISO3 Data = %s", strTemp);
	
	// 1                    : ISO 3 Track 정보 중 처음 더미 한 바이트
	// FIC_ISO3_ACCSTART : 금융 IC 계좌 시작 위치. 첫번째 분리자(=)
	// nAccountStart        : 계좌 시작 위치부터 계좌 번호의 위치
	memset( FICAccount, NULL, sizeof(FICAccount) );
	memcpy( FICAccount, &strTemp[nAccountStart+ FIC_ISO3_ACCSTART], __min(nAccountNOCnt, sizeof(FICAccount)) );

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetAccountNum() 계좌번호 = %s", FICAccount);

	return FICAccount;
}

/*==============================================================================
* Function Name : fnFIC_GetISO3()
* Description   : 거래가 선택된 계좌의 ISO 3 Data Get. Host 전문에 포함
* Parameters    : bIndex : 읽을 계좌의 Index
*                 szISO3 : ISO 3 Data Output Buffer
* Return        : Success : TRUE						Fail : FALSE
* Release       : Ver 1.0
* Notes			: 2004.01.11 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_GetISO3(BYTE bIndex, LPSTR szISO3)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetISO3(Index[%d])", bIndex);

	if ( bIndex > FICAccountCount || bIndex < 1 )
	{
		return FALSE;
	}

	char	szTemp[200];

	memset( szTemp, NULL, sizeof(szTemp) );

	// ISO 3(108) -> Start 1, End 3 Bytes 제외 -> ISO 3 Track(104Bytes)
	MakeUnPack( FinanceICISO3Info[bIndex - 1].ISO3, szTemp, FIC_ISO3_LEN, FALSE );
	memcpy( szISO3, szTemp + 1, FIC_ISO3_LEN * 2 - 4 );
	szISO3[FIC_ISO3_LEN * 2 - 4] = 0x00;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetISO3() ISO3 = %s", szISO3);

	// Bank Code Set
	memset( FICBankCode, NULL, sizeof(FICBankCode) );
	memcpy( FICBankCode, szISO3 + 4, FIC_BANKCODE_LEN );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetISO3():return");
	return TRUE;
}

/*==============================================================================
* Function Name : fnFIC_GetBankCode()
* Description   : 거래가 선택된 계좌 ISO 3 Track 내의 은행 코드를 가져온다.
*                 fnFIC_GetISO3()에서 정보를 입력하며 자타행 구분에 쓰인다.
* Parameters    : (NONE)
* Return        : FICBankCode(Ex : "007")
* Release       : Ver 1.0
* Notes			: 2004.01.10 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnFIC_GetBankCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetBankCode() BankCode = %s", FICBankCode);
	return FICBankCode;
}

/*==============================================================================
* Function Name : fnFIC_SetFICDispInfo()
* Description   : 화면에 보여줄 계좌정보 Format을 세팅. 은행 사양에 따라 다름
* Parameters    : (NONE)
* Return        : (NONE)
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_SetFICDispInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_SetFICDispInfo()");

	BYTE bState;
	int	 nAccountCnt = 0;

	// Buffer Clear
	memset( &FinanceICDisp[0], NULL, sizeof(FinanceICDisp) );

	for ( int i = 0; i < FICAccountCount; i++ )
	{
		// Account State Set
		bState = fnFIC_GetAccountState( nAccountCnt + 1 );

		if (bState != FIC_ACCOUNT_NORMAL)
			continue;

		// Count Set
		sprintf( FinanceICDisp[nAccountCnt].Num, "%2d", nAccountCnt + 1 );

		// Account Number Set. Index 1부터 시작함을 주의
		fnFIC_GetAccountNum( nAccountCnt + 1 );
		
		memset( FinanceICDisp[nAccountCnt].AccNum, 0x20, sizeof(FinanceICDisp[nAccountCnt].AccNum) );	// Buffer Clear
		memcpy( FinanceICDisp[nAccountCnt].AccNum, 
				FICAccount, 
				__min(sizeof(FinanceICDisp[nAccountCnt].AccNum), FinanceICISO3Info[nAccountCnt].AccountNOCnt));
		
		memset( FinanceICDisp[nAccountCnt].AccState, 0x20, sizeof(FinanceICDisp[nAccountCnt].AccState) );	// Buffer Clear

		nAccountCnt++;

//		if ( bState == FIC_ACCOUNT_NORMAL )
//		{
//			memcpy( FinanceICDisp[i].AccState, "정상", 4 );
//		}
//		else if ( bState == FIC_ACCOUNT_CLOSE )
//		{
//			memcpy( FinanceICDisp[i].AccState, "해지", 4 );
//		}	
//		else if ( bState == FIC_ACCOUNT_TEMPSTOP )
//		{
//			memcpy( FinanceICDisp[i].AccState, "임시중지", 8 );
//		}
//		else if ( bState == FIC_ACCOUNT_TRANSFER )
//		{
//			memcpy( FinanceICDisp[i].AccState, "이관", 4 );
//		}
//		else if ( bState == FIC_ACCOUNT_ICCARD )
//		{
//			memcpy( FinanceICDisp[i].AccState, "전자화폐용", 10 );
//		}
	}

	FICAccountCount = nAccountCnt;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_SetFICDispInfo():return");

	return T_OK;
}


/*==============================================================================
* Function Name : fnFIC_GetTRandom()
* Description   : Terminal Random Get
* Parameters    : (NONE)
* Return        : FinanceICInfo.TerminalRandom
* Release       : Ver 1.0
* Notes			: 2004.01.02 Han Su Woong
* ============================================================================*/
LPBYTE CDevCmn::fnFIC_GetTRandom()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetTRandom()");
	return FinanceICInfo.TerminalRandom;
}

/*==============================================================================
* Function Name : fnFIC_GetCRandom()
* Description   : Card Random Get
* Parameters    : (NONE)
* Return        : FinanceICInfo.CardRandom
* Release       : Ver 1.0
* Notes			: 2004.01.02 Han Su Woong
* ============================================================================*/
LPBYTE CDevCmn::fnFIC_GetCRandom()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetCRandom()");
	return FinanceICInfo.CardRandom;
}

/*==============================================================================
* Function Name : fnFIC_GetCipherPW()
* Description   : 암호화된 계좌 비밀번호 Get
* Parameters    : (NONE)
* Return        : FinanceICInfo.CipherPW
* Release       : Ver 1.0
* Notes			: 2004.01.02 Han Su Woong
* ============================================================================*/
LPBYTE CDevCmn::fnFIC_GetCipherPW()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetCipherPW()");
	return FinanceICInfo.CipherPW;
}

/*==============================================================================
* Function Name : fnFIC_GetENCData()
* Description   : ENC Data Get(MyBi)
* Parameters    : bIndex : ENCData 1 or 2 선택
* Return        : FinanceICInfo.ENCData1 or FinanceICInfo.ENCData2
* Release       : Ver 1.0
* Notes			: 2004.01.15 Han Su Woong
* ============================================================================*/
LPBYTE CDevCmn::fnFIC_GetENCData(BYTE bIndex)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetCipherPW(Index=%d)", bIndex);
	return ( bIndex == 0x02 ) ? FinanceICInfo.ENCData2 : FinanceICInfo.ENCData1;
}

/*==============================================================================
* Function Name : fnFIC_GetHostTranData()
* Description   : Host로 전송할 금융 IC 암호화 Data를 구하는 함수
* Parameters    : MyBi : 얻어올 데이터 종류 판별
* Return        : 금융 IC      : Terminal Random(32) + Card Random(32)
*                                + 암호화된 계좌 비밀번호(32)
*                 MyBi 금융 IC : ENC DATA 1(16) + ENC DATA 2(16) + 0x20(64)
* Release       : Ver 1.0
* Notes			: 2004.01.11 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnFIC_GetHostTranData(int IsMyBi)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetHostTranData(IsMyBi=%d)", IsMyBi);

	char szTemp[100];
	memset( FinanceICInfo.HostTranData, NULL, sizeof(FinanceICInfo.HostTranData) );

	//**************************************************************************
	//  금융 IC 호스트 전문 Data
	//  Card Random(32) + Terminal Random(32) + 암호화된 계좌 비밀번호(32)
	//  + File Key Version(2)
	//**************************************************************************
	if ( IsMyBi == FALSE )
	{
		// Card Random(32)
		memset( szTemp, NULL, sizeof(szTemp) );
		MakeUnPack( FinanceICInfo.CardRandom, szTemp, FIC_RANDOM_LEN );
		memcpy( &FinanceICInfo.HostTranData[0], szTemp, 32);

		// Terminal Random(32)
		memset( szTemp, NULL, sizeof(szTemp) );
		MakeUnPack( FinanceICInfo.TerminalRandom, szTemp, FIC_RANDOM_LEN );
		memcpy( &FinanceICInfo.HostTranData[32], szTemp, 32);
		
		// 암호화된 계좌 비밀번호(32)
		memset( szTemp, NULL, sizeof(szTemp) );
		MakeUnPack( FinanceICInfo.CipherPW, szTemp, FIC_RANDOM_LEN );
		memcpy( &FinanceICInfo.HostTranData[64], szTemp, 32);

		// File Key Version(2)
		memset( szTemp, NULL, sizeof(szTemp) );
		sprintf( szTemp, "%02X", FinanceICInfo.FCI_FileKeyVersion );
		memcpy( &FinanceICInfo.HostTranData[96], szTemp, 2 );
	}
	//**************************************************************************
	//  MyBi 금융 IC 호스트 전문 Data
	//  Card Random(32) + Terminal Random(32) + 
	//  ENC DATA 1(16) + ENC DATA 2(16) + 0x20(64) + File Key Version(2)
	//**************************************************************************
	else											// 금융 IC 호스트 전문 Data
	{
		// Card Random(32)
		memset( szTemp, NULL, sizeof(szTemp) );
		MakeUnPack( FinanceICInfo.CardRandom, szTemp, FIC_RANDOM_LEN );
		memcpy( &FinanceICInfo.HostTranData[0], szTemp, 32);

		// Terminal Random(32)
		memset( szTemp, NULL, sizeof(szTemp) );
		MakeUnPack( FinanceICInfo.TerminalRandom, szTemp, FIC_RANDOM_LEN );
		memcpy( &FinanceICInfo.HostTranData[32], szTemp, 32);
		
		// ENC DATA 1(16)
		memset( szTemp, NULL, sizeof(szTemp) );
		MakeUnPack( FinanceICInfo.ENCData1, szTemp, FIC_ENCDATA_LEN );
		memcpy( &FinanceICInfo.HostTranData[64], szTemp, 16);

		// ENC DATA 2(16)
		memset( szTemp, NULL, sizeof(szTemp) );
		MakeUnPack( FinanceICInfo.ENCData2, szTemp, FIC_ENCDATA_LEN );
		memcpy( &FinanceICInfo.HostTranData[80], szTemp, 16);

			// File Key Version(2)
		memset( szTemp, NULL, sizeof(szTemp) );
		sprintf( szTemp, "%02X", FinanceICInfo.FCI_FileKeyVersion );
		memcpy( &FinanceICInfo.HostTranData[96], szTemp, 2 );
}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetHostTranData():return(%s)", FinanceICInfo.HostTranData);
	return FinanceICInfo.HostTranData;
}

/*==============================================================================
* Function Name : fnFIC_EPBReadAccountInfo()
* Description   : IBK e-모든통장 계좌정보파일(요구불/저축성/여신)을 읽어 온다.
* Parameters    : bIndex : 0x00(1 ~ 10), 0x01(11 ~ 20), 0x02(21 ~ 30)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2005.04.26 Nam Sung Ho
* History		: 1. 2005.04.26, V01.02.09, 신규작성
*                 2. 2005.12.21, V05.01.01, 기업은행 Modify
* ============================================================================*/
int CDevCmn::fnFIC_EPBReadAccountInfo(BYTE bIndex)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_EPBReadAccountInfo(%d)", bIndex);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC전송자료

	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_EPB;						// 0x0B
	ICSendData[2] = IC_SFC_EPBREAD;					// 0x11
	ICSendData[3] = bIndex;							// Index

	if (IfmDirect) //#NICCM01
	{
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, EPB_READACCOUNTINFO_JK);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + 1);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
    }
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_EPBReadAccountInfo(...):return[%d]", nReturn);
	return nReturn;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Function Name	: fnFIC_SetEPBDispInfo()
// Description		: 화면에 보여줄 계좌정보 Format을 세팅. 정상계좌만 표시.
// Parameters		: (NONE)
// Return			: (NONE)
// Release			: Ver V05-00-02-#01
// Notes			: 2005.12.27
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int CDevCmn::fnFIC_SetEPBDispInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_SetEPBDispInfo");

	for ( int i = 0; i < FICEPBAccountCount; i++ )
	{
		if ((EPBAccountInfo[i].EPBAccountKind != 0x20)	&&
			(EPBAccountInfo[i].EPBAccountKind != 0x30)	)
		{
			memcpy(FinanceICEPBDisp[FICEPBAccountTotalCount].Num, "%2d", FICEPBAccountTotalCount);

			memset(FinanceICEPBDisp[FICEPBAccountTotalCount].AccNum, NULL, sizeof(FinanceICEPBDisp[FICEPBAccountTotalCount].AccNum));

			MakeUnPack(EPBAccountInfo[i].EPBAccountNum, FinanceICEPBDisp[FICEPBAccountTotalCount].AccNum, 8, TRUE);

			if (EPBAccountInfo[i].EPBAccountKind == 0x33)		// 여신계좌
				FinanceICISO3Info[FICEPBAccountTotalCount].AccountNOCnt = 16;
			else												// 요구불&저축성 계좌
				FinanceICISO3Info[FICEPBAccountTotalCount].AccountNOCnt = 14;

			FinanceICEPBDisp[FICEPBAccountTotalCount].OrgIndex = FICEPBAccountTotalCount;
																// 원본 INDEX
			FinanceICEPBDisp[FICEPBAccountTotalCount].EPBAccountKind = EPBAccountInfo[i].EPBAccountKind;
																// 계좌종류

			FinanceICISO3Info[FICEPBAccountTotalCount].Reserved[0] = EPBAccountInfo[i].EPBAccountKind;
			FinanceICISO3Info[FICEPBAccountTotalCount].AccountStart = 1;
			FinanceICISO3Info[FICEPBAccountTotalCount].AccountState = 0x01;
			memcpy(FinanceICISO3Info[FICEPBAccountTotalCount].ISO3, EPBAccountInfo[i].ISO3, 54);

			if (EPBAccountInfo[i].EPBAccountKind != 0x31)
			{
				MakePack("B0159003D00000000000000000D", 
						FinanceICISO3Info[FICEPBAccountTotalCount].ISO3, 27, '|', FALSE);
				
				memcpy(&FinanceICISO3Info[FICEPBAccountTotalCount].ISO3[5], EPBAccountInfo[i].EPBAccountNum,
						FinanceICISO3Info[FICEPBAccountTotalCount].AccountNOCnt / 2);
			}

			FICEPBAccountTotalCount++;
		}
	}

	if (FICEPBAccountTotalCount > 20)
		FICEPBAccountTotalCount = 20;
	
	return T_OK;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Function Name	: EPB_GetISO3Normal()
// Description		: 화면에서 선택된 계좌정보 세팅
// Parameters		: (NONE)
// Return			: (NONE)
// Release			: Ver V05-00-02-#01
// Notes			: 2005.12.27
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int CDevCmn::fnFIC_GetEPBISO3(BYTE bIndex, LPSTR szISO3)
{
	char	szTemp[200]; //012345678901234567890123456
	char	szTempAcc[] = "B0159003=00000000000000000=";
	int		nCount(0);

	// ISO 3(108) -> Start 1, End 3 Bytes 제외 -> ISO 3 Track(104Bytes)
	memset( szTemp, NULL, sizeof(szTemp) );
	memset( szISO3, NULL, 104 );

	memset(szTemp, 0x30, sizeof(szTemp));
	MakeUnPack(EPBAccountInfo[bIndex].ISO3, szTemp, FIC_ISO3_LEN, FALSE );
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetEPBISO31", EPBAccountInfo[bIndex].ISO3, FIC_ISO3_LEN);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetEPBISO32", szTemp, FIC_ISO3_LEN * 2);
	memset(szISO3, 0x30, 104);
	memcpy(szISO3, szTemp + 1, FIC_ISO3_LEN * 2 - 4);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetEPBISO33", szISO3, FIC_ISO3_LEN * 2);

	// Bank Code Set
	memset( FICBankCode, NULL, sizeof(FICBankCode) );
	memcpy( FICBankCode, szISO3 + 4, FIC_BANKCODE_LEN );

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Function Name	: fnFIC_GetEPBAccountCnt()
// Description		: e-모든통장 계좌 갯수 Get
// Parameters		: (NONE)
// Return			: m_nEPBNAccCnt
// Release			: Ver V05-00-02-#01
// Notes			: 2006.01.10
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int CDevCmn::fnFIC_GetEPBAccountCnt()
{
	return FICEPBAccountTotalCount;								// 정상 계좌 갯수
}

//#NICCM01
int CDevCmn::fnFIC_CheckIFMError(int nICRecvData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_CheckIFMError():(%d)", nICRecvData);

	if (!IfmDirect)
		return T_OK;

	if ((nICRecvData == WFS_ERR_HARDWARE_ERROR) ||					// -14
		(nICRecvData == WFS_ERR_TIMEOUT))							// -48
	{
		fnAPL_PrintJnlInfo(GetSprintf("IC Read Failure"));
		fnAPL_StackError(fstrMCU_GetErrorCode(), "카드부장애(IFM)", DEV_MCU);
		DeviceStatus |= DEV_MCU;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_CheckIFMError():ERROR");
		return T_ERROR;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_CheckIFMError():OK");
	return T_OK;
}
