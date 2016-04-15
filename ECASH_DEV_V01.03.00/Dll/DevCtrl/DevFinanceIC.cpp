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
* Description   : IC �ŷ� �ʱ�ȭ �Լ�(Buffer Clear)
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
* Description   : IC Card�� ���� ǥ�� APDU Message�� �����ϴ� �Լ�
* Parameters    : lpSendData : Send Message Buffer
*                 ISOCase    : ISO-7816 APDU Command Type
*                 CLA        : Class Byte
*                 INS        : Instruction Byte
*                 P1         : Parameter 1
*                 P2         : Parameter 2
*                 Lc         : Length of Command. Case2������ Le
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
* Description   : Data ��ü�� ǥ�� TLV ���·� ����� �����ϴ� �Լ�(2����Ʈ Tag��)
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
* Description   : Data ��ü�� ǥ�� TLV ���·� ����� �����ϴ� �Լ�(1����Ʈ Tag��)
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
* Description   : IC Card�� ǥ�� APDU ����� �����ϴ� �Լ�(���� ������ ����)
* Parameters    : lpAPDU   : ǥ�� APDU ���
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

	memset(ICSendData, 0, sizeof(ICSendData));		// IC�����ڷ�

	if (IfmDirect) //#NICCM01
	{
		memcpy( ICSendData, lpAPDU, nAPDULen );
		strICSendData = MakeUnPack(ICSendData, nAPDULen);
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_ISOIN, strICSendData);
		
	}
	else
	{
		// Message ����
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
* Description   : IC Card�� ǥ�� APDU ����� �����ϴ� �Լ�(���� ������ ����)
* Parameters    : lpAPDU   : ǥ�� APDU ���
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

	memset(ICSendData, 0, sizeof(ICSendData));		// IC�����ڷ�

	if (IfmDirect) //#NICCM01
	{
		memcpy( ICSendData, lpAPDU, nAPDULen );
		strICSendData = MakeUnPack(ICSendData, nAPDULen);
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_ISOOUT, strICSendData);
		
	}
	else
	{
		// Message ����
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
* Description   : IC ī�� ������ ���ؼ� Applet�� �����ϴ� �Լ�
* Parameters    : nICKind : �����Ϸ��� IC Card ����
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
	case TRAN_IC_FINANCEIC :									// ���� IC Applet
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
	
	case TRAN_IC_AUTHEN :										// ������ Applet
		// Rserved for Future Use
		break;

	case TRAN_IC_ELECPB	:										// �������� Applet //#NICCM01
		memcpy( cbAID, ELECPB_AID, ELECPB_AID_LEN );
		nReturn = fnICA_SelectApplet( cbAID, ELECPB_AID_LEN );
		break;

	case TRAN_IC_HIPASS:										// Hi-Pass���˽��������� #N0266
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
* Description   : AID�� �̿��Ͽ� IC ī�� ���� Applet�� ���� �����ϴ� �Լ�
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

	memset(ICSendData, 0, sizeof(ICSendData));		// IC�����ڷ�
	
	if (IfmDirect)  //#NICCM01
	{
		// Message ����
		memcpy( ICSendData, lpAID, FIC_AID_LEN );
		strICSendData = MakeUnPack(ICSendData, FIC_AID_LEN);
		
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_SELECTAPPLET, strICSendData);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_SelectApplet(...):IfmDirect_return[%d]", nReturn);
		
	}
	else
	{
		// Message ����
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
* Description   : AID�� �̿��Ͽ� IC ī�� ���� Application ������ �����ϴ� �Լ�
* Parameters    : BankType : ����ϴ� ���� Ÿ��. �˻��� ī�� ������ ����.
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.01 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_ICConfirmType(BYTE BankType)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_ICConfirmType()");

	int		nReturn(FALSE);
	CString strICSendData("");

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC�����ڷ�


	if (IfmDirect)												// //#NICCM01	ICCM ��� ����
	{
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_ICTYPECONFIRM, "0");		// 0:�Ϲ�����, 1:��������
	}
	else
	{
		// Message ����
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
* Description   : ��۽� ó��. 61xx, 6Cxx.
* Parameters    : lpData   : ���� ���� ������
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
		// Message Construction : ���� CLA + INS + P1 + P2 + xx
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

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC�����ڷ�

	if (IfmDirect) //#NICCM01
	{
		memcpy( ICSendData, bAPDUData, nAPDULen );
		strICSendData = MakeUnPack(ICSendData, nAPDULen);
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_ISOOUT, strICSendData);
		
	}
	else
	{
		// Message ����
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
//	if ((!memcmp(lpData, IC_RECV_OK, 3))	||					// ����
//		(!memcmp(lpData, IC_RECV_OK2, 3))	)
//		;
//	else														// ������
//	{
//		nReturn = nErrorReturn;									// ��Ÿ����
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
* Description   : Chip Recive Data�� ���� FC Code ����
* Parameters    : (NONE)
* Return        : Success : TRUE				Fail : FALSE
* Release       : Ver 1.0
* Notes			: 2004.01.26 �츮�� ���� Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_CheckFCCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_CheckFCCode()");
	int nReturn(FALSE);

	if ( ICRecvData[0] == IC_RECV_OK[0] )			// ��������
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ldc_fnICA_CheckFCCode():true");
		nReturn = TRUE;
	}
	else if ( ICRecvData[0] == IC_RECV_NG )			// ��������
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ldc_fnICA_CheckFCCode():false");
		nReturn = FALSE;
	}
	else if ( memcmp( ICRecvData, IC_RECV_OK2, IC_FCMS_LEN ) == 0 )	// ��������2
	{
		nReturn = TRUE;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_CheckFCCode():return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnICA_AnalyzeFCError()
* Description   : ���� �ڵ� ������ ������ �м��ϴ� �Լ�
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
	//	m_szErrCode = unpack ó��
		wsprintf( m_szErrMsg, FCErrMsg[nReturn].ICErrGuide );
	}
	else											// �⺻ Error Setting
	{
		wsprintf( m_szErrCode, "FFFFFFFF" );
		wsprintf( m_szErrMsg, "Unknown Error" );
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_AnalyzeFCError():return[%d]", nReturn);

	return nReturn;
}

/*==============================================================================
* Function Name : fnICA_AnalyzeISOError()
* Description   : ISOIN, ISOOUT ���� ǥ�� APDU Error�� �����ϴ� �Լ�
* Parameters    : lpData   : ���� ���� ������
*                 nDataLen : Length of lpData
* Return        : No Error : TRUE				Error : FALSE
* Release       : Ver 1.0
* Notes			: 2003.12.26 Han Su Woong
* ============================================================================*/
int CDevCmn::fnICA_AnalyzeISOError(LPBYTE lpData, int &nDataLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_AnalyzeISOError()");
	
	int		nReturn(0);

	// Get Response, Incorrect Length ó��
	if ( lpData[nDataLen - IC_SW_LEN] == ISO7816_SW_BYTES_REMAINING[0] ||	// 0x61
		 lpData[nDataLen - IC_SW_LEN] == ISO7816_SW_CORRECT_LENGTH[0]  )	// 0x6C
	{
		if ( fnICA_ISO7816WarningProc(lpData, nDataLen) == TRUE)//0 )	// #N0266 ����ó�� Ȯ���ʿ�
		{
			nReturn = TRUE;
		}
	}

	if ( nReturn != TRUE )
	{
		// ISO ǥ�� ���� ó�� : ����� Data Error�� ó��(0xD2 0x42 0x46 0x46)
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
* Description   : �����ڵ� Error Get
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
* Description   : �����ڵ� Error Set
* Parameters    : lpErrData : m_cbFCError
* Return        : (NONE)
* Release       : Ver 1.0
* Notes			: 2003.12.26 Han Su Woong
* ============================================================================*/
void CDevCmn::fnICA_SetFCError(LPBYTE lpErrData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_SetFCError()");
	CString strErrorCode = "";
	if (IfmDirect)	//#NICCM01	ICCM ��� ����
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
* Description   : ���� IC �ŷ� �ʱ�ȭ �Լ�(Buffer Clear)
* Parameters    : (NONE)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_InitProcess()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_InitProcess()");

	FICCardType = FIC_NONE;										// ī����
	FICDFType = 0;												// IC DF�̼���
	FICAppletType = 0;											// ICī�� APPLET����
	FICAccountCount = 0;										// ����IC ���°���
	FICEPBAccountCount = 0;										// ����IC �������� ���°���
	FICEPBAccountTotalCount = 0;								// ����IC �������� �Ѱ��°���

	memset( FICAccount,  NULL, sizeof(FICAccount)  );
	memset( FICBankCode, NULL, sizeof(FICBankCode) );

	memset( FinanceICInfo.CAN, NULL, sizeof(FinanceICInfo) );
	memset( &FinanceICISO3Info[0], NULL, sizeof(FinanceICISO3Info) );

	memset(&EPBAccountInfo, 0, sizeof(EPBAccountInfo));	// e-������� ��������
	memset(&FinanceICEPBDisp, 0, sizeof(FinanceICEPBDisp));
	memset(&FinanceICEPBDisp2, 0, sizeof(FinanceICEPBDisp2));

	return T_OK;
}

/*==============================================================================
* Function Name : fnFIC_GetICType()
* Description   : IC TYPE �б�
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

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	if (IfmDirect)									// #NICCM01	ICCM ��� ����
	{
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, CMN_ICTYPECONFIRM, "0");		// 0:�Ϲ�����, 1:��������
	}
	else
	{
	
		// Message ����
		ICSendData[0] = IC_FC_MAIN;									// 0x30
		ICSendData[1] = IC_MFC_BASIC;								// 0x01
		ICSendData[2] = IC_SFC_ICCONFIRMTYPE;						// 0x31
		ICSendData[3] = btData;										// 0x00 : �Ϲ����� 0x10 : ��������
		
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + 1);
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);

	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetICType():return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_ReadBasicInfo()
* Description   : ���� IC Applet�� �⺻ ������ ��ȸ�ϴ� �Լ�
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

	memset(ICSendData, 0, sizeof(ICSendData));		// IC�����ڷ�

	if (IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, FIC_READBASICINFO);		
	else
	{
		// Message ����
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
* Description   : ���� IC Applet�� ���� ������ ��ȸ�ϴ� �Լ�
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

	memset(ICSendData, 0, sizeof(ICSendData));		// IC�����ڷ�

	if (IfmDirect) //#NICCM01
	{
		ICSendData[0] = bIndex;							// Index
		strICSendData = MakeUnPack(ICSendData, sizeof(BYTE));
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, FIC_READACCOUNTINFO, strICSendData);
	}
	else
	{
		// Message ����
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
* Description   : ���� IC Applet�� �⺻ ���� + ���� ������ ��ȸ�ϴ� �Լ�
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

	memset(ICSendData, 0, sizeof(ICSendData));		// IC�����ڷ�

	if (IfmDirect) //#NICCM01
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T6, FIC_READDUALINFO);
	else
	{
		// Message ����
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
* Description   : ���� IC Get Encipher
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

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	if (IfmDirect) //#NICCM01
		nSendLen = 0;
	else
	{
		// Message ����
		ICSendData[0] = IC_FC_MAIN;									// 0x30
		ICSendData[1] = IC_MFC_CC;									// 0x07
		ICSendData[2] = IC_SFC_GETENCIPHER;							// 0x23

		nSendLen = IC_FCMS_LEN;
	}

//t	nPassLen = strlen(szPassWord);								// ���̿��� 
	nPassLen = PassWordSize;

	// Parameter Length(0x14)
	ICSendData[nSendLen] = FIC_RANDOM_LEN + nPassLen;
	nSendLen++;

	// Terminal Random(16Bytes)
	fnFIC_MakeRandom(FinanceICInfo.TerminalRandom, FIC_RANDOM_LEN);

	memcpy( ICSendData + nSendLen, FinanceICInfo.TerminalRandom, FIC_RANDOM_LEN );
	nSendLen += FIC_RANDOM_LEN;

	// �Է¹��� Account Password(4Bytes)
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
* Description   : ���� IC Get ENCData(MyBi). Define ���� �� Tran�ܿ� ��ġ���� ����.
* Parameters    : bIndex     : Get ENCData1 = 0x01, Get ENCData2 = 0x02
*                 szPassWord : ���� �Է��� ���� ��й�ȣ
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
	BYTE	bRandom[FIC_ENCDATA_LEN];							// ��ȣȭ�� ����� Random Buffer
	BYTE	bAPDUData[IC_APDU_LEN];
	int		nAPDULen;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

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
* Description   : ���� IC GetChallenge �Լ�(MyBi)
*                 GetChallenge : Update Record ���� �ϱ� ���ؼ� IC Card��
*                 Session Key�� ������ ISO-7816-4 ǥ�� ����̴�.
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

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

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
* Description   : ���� IC PIN Verify
* Parameters    : szPIN : ���� IC PIN
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_CheckPIN(LPCTSTR szPassWord)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_CheckPIN(%s)", szPassWord);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

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
* Description   : ���� IC PAN File REad
* Parameters    : szPIN : ���� IC PIN
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_ReadPan(LPCTSTR szPassWord)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadPan(%s)", szPassWord);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));		// IC�����ڷ�

	// Message ����
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
* Description   : ���� IC JAN File REad
* Parameters    : szPIN : ���� IC PIN
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_ReadBAmount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ReadBAmount()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));		// IC�����ڷ�

	// Message ����
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
* Description   : ���� IC JAN File REad
* Parameters    : ��������ȣ8+�����ľ�ȣ8
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2003.12.23 Han Su Woong
* ============================================================================*/
int	CDevCmn::fnFIC_ChangePin(LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_ChangePin(szICSendData[%s])", szICSendData);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	// Message ����
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
* Description   : ���� IC JAN File REad
* Parameters    : IC TYPE(OPEN,CLOSE)+��������ȣ8+�����ľ�ȣ8
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

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	if (nOpenCloseType == 1)									// �����
	{
		// Message ����
		ICSendData[0] = IC_FC_MAIN;								// 0x30
		ICSendData[1] = IC_MFC_PIN;								// 0x09
		ICSendData[2] = IC_SFC_OPTIONALCLOSEPINCHANGE;			// 0x22
		ICSendData[3] = 0x00;									// 0x00 ALL PIN CHANGE
		ICSendData[4] = bChangeKeyPinType;						// KEY����(0x00),PIN����(Ox01)
	}
	else														// ������
	{
		// Message ����
		ICSendData[0] = IC_FC_MAIN;								// 0x30
		ICSendData[1] = IC_MFC_PIN;								// 0x09
		ICSendData[2] = IC_SFC_OPTIONALOPENPINCHANGE;			// 0x21
		ICSendData[3] = bChangeAppletType;						// OPTION PIN-Ox01:����IC,0x02:K-CASH,0x04:������(�����), 0x08:������(�й��)
		ICSendData[4] = bChangeKeyPinType;						// KEY����(0x00),PIN����(Ox01)
	}


	if (IfmDirect)												// #NICCM01	ICCM ��� ����
	{
		strICSendData = szICSendData;
		nReturn = fnMCU_ICCMSendData(TRAN_IC_PROTOCOL_T1, KCS_OPENPINCHANGE,strICSendData);
	}
	else
	{
		strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN + 2) + szICSendData; // #NICCM01	ICCM ��� ����-01
		nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	}


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_OptionalChangePin(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_ReadTrafficInfo()
* Description   : ���� IC ������������ȸ
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

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	// Message ����
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
* Description   : ���� IC ȯ�Һ�����ó��
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

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	// Message ����
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
* Description   : ���� IC GetENCData�� ���� XOR Data�� ���ϴ� �Լ�(MyBi)
* Parameters    : szPassWord : ���� �Է��� ���� ��й�ȣ
*                 lpSource   : XOR �ϱ� ���� Seed Data(16Bytes)
*                 lpResult   : XOR �� ��� Data(8Bytes)
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

	// ���� ��й�ȣ Set
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
* Description   : ���� IC Chip Receive Data �ؼ��ϰ� ���ۿ� �����ϴ� �Լ�
* Parameters    : nCmdType : �м��� ���� �޽����� ����
*                 +---------------- ------+------------------------------------+
*				  | 7816_CHECK            | ISO 7816 ǥ�� ���� Check           |
*				  | FINANCEIC_PROC_XXXX   | �� �ŷ� Flow�� ���� ó��           |
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

	if ((!memcmp(ICRecvData, IC_RECV_OK, 3))	||				// ����
		(!memcmp(ICRecvData, IC_RECV_OK2, 3))	)
		;
	else														// ������
	{
		fnICA_SetFCError(ICRecvData); // #NICCM01
				
		nReturn = nErrorReturn;									// ��Ÿ����
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

	if (nReturn)												// �������� ������
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FICDFType[%d]) NG Return[%d]", FICDFType, nReturn);
		// AP B/D ���� �� ���� ������ D2436285+Data �� ��
		if(IfmDirect) // #NICCM01
		{
			if(strICRecvData.Left(4).CompareNoCase("D2436285") == 0)
				nReturn = 29;
		}
																// Exception-1 : ��������ó��
		if ((nCmdType  == FINANCEIC_PROC_SELECTAPPLICATION)	&&	// SelectApplet�ΰ��
			(FICDFType == TRAN_IC_KCASH)					&&	// DF == ����ȭ��
			(nReturn   == 29)								)	// Resp == (D2 42 62 85)
		{
			nOffset += 1;										// ���������� �������亸�� 1Byteũ��
			nReturn = 0;										// ������������
		}
		else													// Exception��Ȳ�� �ƴѰ��
		{
			fnICA_SetErrCode((char *)&ICRecvData[4]);
			return nReturn;										// NG������ Error Return
		}
	}
/////////////////////////////////////////////////////////////////////////////

	nOffset  += IC_FCMS_LEN;									// ���� �ڵ�
	nRecvLen /= 2;												// Pack Data Len

	/*==============================================================================*/
	// ����IC�ŷ��ΰ�쿡�� �м��� ������ FinanceICInfo����ü�� �� ���� ����������
	// ����ȭ�� �ŷ��ΰ�쿡�� �м��� ������ KCashInfo����ü�� �� ���� �����ϵ��� �Ѵ�
	/*==============================================================================*/
	switch (nCmdType)
	{
	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_GETICTYPE:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(ī��Ÿ��) FINANCEIC_PROC_GETICTYPE");
		memcpy( &ICTypeInfo.ICTypeConfirm, &ICRecvData[nOffset], FIC_ICTYPE_LEN );
		nOffset += FIC_ICTYPE_LEN;

		break;

	case FINANCEIC_PROC_SELECTAPPLICATION :						// Select Application
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION)");

		if (!FICDFType)											// Select�� DF�� �˼� ������..
			return nErrorReturn;
		
		if (FICDFType == TRAN_IC_FINANCEIC)						// SELECT ����IC
		{
			// FCI Template Tag(�ʼ�)
			if ( FIC_MYBI_TAG_FCI != ICRecvData[nOffset] )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset += 1;										// Tag Length
			nOffset += 1;										// Data Length

			// DF Name Tag(�ʼ�)
			if ( FIC_MYBI_TAG_DFNAME != ICRecvData[nOffset] )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset++;											// DFNAME Tag Length
			nLength = ICRecvData[nOffset];						// DF Name Length
			nOffset++;											// DF NAME Data Length
			nOffset += nLength;									// Add DF Name Data Length

			// FCI Proprietary Template Tag(�ʼ�)
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

			// �������� Tag(�ʼ�)
			if ( memcmp( FIC_MYBI_TAG_FILEINFO, &ICRecvData[nOffset],
						 sizeof(FIC_MYBI_TAG_FILEINFO) ) != 0 )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset += 2;										// Tag Length
			nOffset++;											// �������� Length

			memcpy(FinanceICInfo.FCI_FInformation, &ICRecvData[nOffset], 8);


			// ǥ��ȭ ����
			FinanceICInfo.FCI_StandardVersion = ICRecvData[nOffset];
			nOffset += sizeof(BYTE);

			// File Key Version
			FinanceICInfo.FCI_FileKeyVersion = ICRecvData[nOffset];
			nOffset += sizeof(BYTE);

			break;
		}
		else
		if (FICDFType == TRAN_IC_KCASH)							// SELECT ����ȭ��
		{
			if (ICRecvData[nOffset] != FIC_KCASH_TAG_FCI)		// FCI Templete Code Check
				return nErrorReturn;
			nOffset += 1;

//t			if (ICRecvData[nOffset] != 0x1F)					// FCI Data Length Check Skip
//t				return nErrorReturn;
			nOffset += 1;

			if (ICRecvData[nOffset] != FIC_KCASH_TAG_CARDID)	// FCI �߱���ID Check
				return nErrorReturn;
			nOffset += 1;
			if (ICRecvData[nOffset] != 0x10)					// FCI �߱���ID Length Check
				return nErrorReturn;
			nOffset += 1;
			nOffset += 5;										// FCI �߱���ID : 5Byte Skip
			memcpy(KCashInfo.CardNum, &ICRecvData[nOffset], 8);	// FCI �߱���ID�� ī���ȣ
			nOffset += 11;										// ī���ȣ(8) + Dummy(3)

			if (memcmp(&ICRecvData[nOffset], FIC_KCASH_TAG_MAKEDAY, 2) != 0)
				return nErrorReturn;									// �߱���Check
			nOffset += 2;										// ����ȭ�� �߱��� Tag Length
			if (ICRecvData[nOffset] != 0x04)					// ����ȭ�� �߱��� Length Check
				return nErrorReturn;
			nOffset += 1;										// ����ȭ�� �߱��� DataLen Length
			nOffset += 4;										// ����ȭ�� �߱��� Data Length

			if (ICRecvData[nOffset] != FIC_KCASH_TAG_CARDINFO)	// FCI ����ȭ������ Tag Check
				return nErrorReturn;
			nOffset += 1;
			if (ICRecvData[nOffset] != 0x04)					// ����ȭ������ Length Check
				return nErrorReturn;
			nOffset += 1;										// ����ȭ������ DataLen Length
			memcpy(KCashInfo.CardInform, &ICRecvData[nOffset], 4);
			nOffset += 4;										// ����ȭ������(ī�屸��) 
		}
		break;

	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_READBASICINFO :							// �⺻���� ��ȸ
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
			
			// ī�� ������ �̸� ����
			FinanceICInfo.CardHolderNameLen = ICRecvData[nOffset];
			nOffset++;
			
			// ī�� ������ �̸�
			memcpy( FinanceICInfo.CardHolderName, ICRecvData + nOffset,
					FinanceICInfo.CardHolderNameLen );
			nOffset += FinanceICInfo.CardHolderNameLen;
		}

		// �Ǹ� ID Tag
		if ( FIC_TAG_REALNAME == ICRecvData[nOffset] )
		{
			nOffset++;

			FinanceICInfo.RealNameNOLen = ICRecvData[nOffset];	// �Ǹ��ȣ ����
			nOffset++;

			FinanceICInfo.RealNameType = ICRecvData[nOffset];	// �Ǹ���
			nOffset++;

			if (FinanceICInfo.RealNameNOLen >= 1)	
			{
				memcpy( FinanceICInfo.RealNameNO, ICRecvData + nOffset,
						FinanceICInfo.RealNameNOLen - 1 );			// �Ǹ��ȣ(�Ǹ��ȣ ���̴� �Ǹ��� 1Bytes�� ����)
				nOffset += FinanceICInfo.RealNameNOLen - 1;
			}
		}

		// ������ Tag
		if ( memcmp( FIC_TAG_EXPIREDAY, ICRecvData + nOffset,
					 sizeof(FIC_TAG_EXPIREDAY) ) == 0 )
		{
			nOffset += sizeof(FIC_TAG_EXPIREDAY);

			FinanceICInfo.ExpireDayLen = ICRecvData[nOffset];	// ������ ����
			nOffset++;

			// ������(BCD -> String)
			MakeUnPack( &ICRecvData[nOffset], FinanceICInfo.ExpireDay, FinanceICInfo.ExpireDayLen );
			nOffset += FinanceICInfo.ExpireDayLen;
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) ������ = %s", FinanceICInfo.ExpireDay);
		}


		// �˰��� �ĺ��ڵ� Tag
		if ( FIC_TAG_ALGOCODE == ICRecvData[nOffset] )
		{
			nOffset += sizeof(BYTE) + sizeof(BYTE);				// �˰��� �ĺ��ڵ� ����
			
			FinanceICInfo.AlgorithmCode = ICRecvData[nOffset];	// �˰��� �ĺ��ڵ�
			nOffset++;
		}
		
		// ������ �ĺ��ڵ� Tag(�ʼ�)
		if ( FIC_TAG_ISSURECODE != ICRecvData[nOffset] )
			return nErrorReturn;								// Data Error
		nOffset++;

		FinanceICInfo.IssureCodeLen = ICRecvData[nOffset];		// ������ �ĺ��ڵ� ����
		nOffset++;
		
		// ������ �ĺ��ڵ�(BCD -> String)
		MakeUnPack( &ICRecvData[nOffset], FinanceICInfo.IssureCode, FinanceICInfo.IssureCodeLen );
		nOffset += FinanceICInfo.IssureCodeLen;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) ������ �ĺ��ڵ� = %s", FinanceICInfo.IssureCode);

		// ī�� �Ϸù�ȣ Tag(�ʼ�)
		if ( FIC_TAG_CSN != ICRecvData[nOffset] )
			return nErrorReturn;								// Data Error
		nOffset += sizeof(BYTE) + sizeof(BYTE);					// ī�� �Ϸù�ȣ ����

		// ī�� �Ϸù�ȣ
		memcpy( FinanceICInfo.CSN, ICRecvData + nOffset, FIC_CSN_LEN );
		nOffset += FIC_CSN_LEN;

		// ī�� �Ϸù�ȣ for Screen(BCD -> String)
		MakeUnPack( FinanceICInfo.CSN, FinanceICInfo.szCSN, FIC_CSN_LEN );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) CSN = %s", FinanceICInfo.szCSN);
		break;

	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_READACCOUNTINFO :						// �������� ��ȸ
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READACCOUNTINFO)");
		
		nOffset++;												// �������� Command Index
		
		FICAccountCount = ICRecvData[nOffset];					// ���� ����
		nOffset++;

		// Total Size Check : �����ڵ�(3) + Send Index(1) + ���°���(1)
		//                  + ���°��� * ( ���� ���� Tag + �������� ���� + �������� )
		nLength = IC_FCMS_LEN + 2
				  + FICAccountCount * ( 2 + FIC_ACCOUNTINFO_LEN );
		if ( nRecvLen != nLength )
			return nErrorReturn;								// Data Error

		// �������� Set
		// ������ ī���ؼ��� �ȵȴ�. NULL ���� ������ ���� �� �ִ�.
		nCount = 0;
		for ( i = 0; i < FICAccountCount; i++ ) // #NICCM01
		{
			// ���� ���� Tag(�ʼ�)
			if ( FIC_TAG_ACCOUNTINFO != ICRecvData[nOffset] )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset++;

			nOffset++;											// ���� ���� ũ��(0x3E). ������ �ʿ� ���ٰ� �Ǵ�.
			
			// ���� ����
			memcpy( &FinanceICISO3Info[nCount], ICRecvData + nOffset,
					FIC_ACCOUNTINFO_LEN );
			nOffset += FIC_ACCOUNTINFO_LEN;

			// �������� ����. ���� ���� �Ǻ� AP ��ȯ �κп��� �ʿ�� �Ѵ�.
			if ( FinanceICISO3Info[nCount].AccountState != 0x00 )
			{
				// ������¿� ���ؼ� ȭ����¹� �ŷ�ó���� �����ϵ��� �Ѵ�.
				// 2004.02.27 ����� ��ȣ������û
				if (FinanceICISO3Info[nCount].AccountState == FIC_ACCOUNT_NORMAL)
				{
					nCount++;
					MakeUnPack( &FinanceICISO3Info[nCount-1], strLog, FIC_ACCOUNTINFO_LEN );
				}
				else
				{
					MakeUnPack( &FinanceICISO3Info[nCount], strLog, FIC_ACCOUNTINFO_LEN );
					strLog = "[������¾ƴ�]" + strLog;
				}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READACCOUNTINFO) ���� %d = %s", nCount, (LPSTR)(LPCTSTR)strLog);
			}
		}
		FICAccountCount = nCount;								// ���� ������ ��

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READACCOUNTINFO) FICAccountCount = %d", FICAccountCount);

		fnFIC_SetFICDispInfo();									// ȭ�鿡 Format�� ����. ���� ��翡 ���� �޶���

		break;
	
	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_READDUALINFO :							// �������� + �������� ��ȸ
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READDUALINFO)");

		//**********************************************************************
		//  BasicInfo Part
		//**********************************************************************
		// �������� ���� Set
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
			
			// ī�� ������ �̸� ����
			FinanceICInfo.CardHolderNameLen = ICRecvData[nOffset];
			nOffset++;
			
			// ī�� ������ �̸�
			memcpy( FinanceICInfo.CardHolderName, ICRecvData + nOffset,
					FinanceICInfo.CardHolderNameLen );
			nOffset += FinanceICInfo.CardHolderNameLen;
		}

		// �Ǹ� ID Tag
		if ( FIC_TAG_REALNAME == ICRecvData[nOffset] )
		{
			nOffset++;

			FinanceICInfo.RealNameNOLen = ICRecvData[nOffset];	// �Ǹ��ȣ ����
			nOffset++;

			FinanceICInfo.RealNameType = ICRecvData[nOffset];	// �Ǹ���
			nOffset++;

			if (FinanceICInfo.RealNameNOLen >= 1)	
			{
				memcpy( FinanceICInfo.RealNameNO, ICRecvData + nOffset,
						FinanceICInfo.RealNameNOLen - 1 );			// �Ǹ��ȣ(�Ǹ��ȣ ���̴� �Ǹ��� 1Bytes�� ����)
				nOffset += FinanceICInfo.RealNameNOLen - 1;
			}
		}

		// ������ Tag
		if ( memcmp( FIC_TAG_EXPIREDAY, ICRecvData + nOffset,
					 sizeof(FIC_TAG_EXPIREDAY) ) == 0 )
		{
			nOffset += sizeof(FIC_TAG_EXPIREDAY);

			FinanceICInfo.ExpireDayLen = ICRecvData[nOffset];	// ������ ����
			nOffset++;

			// ������(BCD -> String)
			for ( i = 0; i < FinanceICInfo.ExpireDayLen; i++ )
			{
				sprintf( FinanceICInfo.ExpireDay + 2 * i,
						 "%2.2X", ICRecvData[nOffset + i] );
			}
			nOffset += FinanceICInfo.ExpireDayLen;
		}

		// �˰��� �ĺ��ڵ� Tag
		if ( FIC_TAG_ALGOCODE == ICRecvData[nOffset] )
		{
			nOffset += sizeof(BYTE) + sizeof(BYTE);				// �˰��� �ĺ��ڵ� ����
				
			// // #NICCM01 V04-02-00-#0a : ������ִ�å(No.93) ������������������������������������������
			if ( FIC_TAG_ISSURECODE != ICRecvData[nOffset] )	
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FIC_TAG_ISSURECODE != ICRecvData[nOffset]");
			
				FinanceICInfo.AlgorithmCode = ICRecvData[nOffset];	// �˰��� �ĺ��ڵ�
				nOffset++;
			}
			else
			{
				FinanceICInfo.AlgorithmCode = 0x00;
				nAlgoCodeValueError = TRUE;
			}
			// ������������������������������������������������������������������������������	

		}


		// ������ �ĺ��ڵ� Tag(�ʼ�)
		if ( FIC_TAG_ISSURECODE != ICRecvData[nOffset] )
			return nErrorReturn;								// Data Error
		nOffset++;

		FinanceICInfo.IssureCodeLen = ICRecvData[nOffset];		// ������ �ĺ��ڵ� ����
		nOffset++;
		
		// ������ �ĺ��ڵ�(BCD -> String)
		for ( i = 0; i < FinanceICInfo.IssureCodeLen; i++ )
		{
			sprintf( FinanceICInfo.IssureCode + 2 * i,
					 "%02X", ICRecvData[nOffset + i] );
		}
		nOffset += FinanceICInfo.IssureCodeLen;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READDUALINFO) ������ �ĺ��ڵ� = %s", FinanceICInfo.IssureCode);

		// ī�� �Ϸù�ȣ Tag(�ʼ�)
		if ( FIC_TAG_CSN != ICRecvData[nOffset] )
			return nErrorReturn;								// Data Error
		nOffset += sizeof(BYTE) + sizeof(BYTE);					// ī�� �Ϸù�ȣ ����

		// ī�� �Ϸù�ȣ
		memcpy( FinanceICInfo.CSN, ICRecvData + nOffset, FIC_CSN_LEN );
		nOffset += FIC_CSN_LEN;

		// ī�� �Ϸù�ȣ for Screen(BCD -> String)
		for ( i = 0; i < FIC_CSN_LEN; i++ )
		{
			sprintf( FinanceICInfo.szCSN + 2 * i, "%02X", FinanceICInfo.CSN[i] );
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READDUALINFO) CSN = %s", FinanceICInfo.szCSN);

		// #NICCM01 ������ִ�å(No.93) ������������������������������������������
		if (nAlgoCodeValueError)									
			nOffset++;
		// ��������������������������������������������������������������������

		//**********************************************************************
		//  AccountInfo Part
		//**********************************************************************
		nOffset++;												// �������� Command Index
		
		FICAccountCount = ICRecvData[nOffset];					// ���� ����
		nOffset++;

	
		// #NICCM01������ִ�å(No.95) �������������������������������������������� 
		// Total Size Check : �����ڵ�(3) + �⺻����(N) + Send Index(1) + ���°���(1)
		//                  + ���°��� * ( ���� ���� Tag + �������� ���� + �������� )
// 		nLength += IC_FCMS_LEN + 3 +
// 				   FICAccountCount * ( 2 + FIC_ACCOUNTINFO_LEN );
// 		if ( nRecvLen != nLength )
// 		{
// 			return nErrorReturn;								// Data Error
// 		}
		// ����������������������������������������������������������������

		// �������� Set
		// ������ ī���ؼ��� �ȵȴ�. NULL ���� ������ ���� �� �ִ�.
		nCount = 0;
		for ( i = 0; i < FICAccountCount; i++ )
		{

			// ������ִ�å(No.94) ���� ���� Tag(�ʼ�) �̵̹Ǿ� �ֳ� V07-00-03-#08
// ���� ���� Tag(�ʼ�) #NICCM01 
//			if ( FIC_TAG_ACCOUNTINFO != ICRecvData[nOffset] )
//			{
//				return nErrorReturn;							// Data Error
//			}

			nOffset++;

																// ���±��� ����Ÿ
			nOffset++;											// ���� ���� ũ��(0x3E). ������ �ʿ� ���ٰ� �Ǵ�.
			
			// ���� ����
			memcpy( &FinanceICISO3Info[nCount], ICRecvData + nOffset,
					FIC_ACCOUNTINFO_LEN );
			nOffset += FIC_ACCOUNTINFO_LEN;

			// �������� ����. ���� ���� �Ǻ� AP ��ȯ �κп��� �ʿ�� �Ѵ�.
			if ( FinanceICISO3Info[nCount].AccountState != 0x00 )
			{
				// ������¿� ���ؼ� ȭ����¹� �ŷ�ó���� �����ϵ��� �Ѵ�.
				// 2004.02.27 ����� ��ȣ������û
				if (FinanceICISO3Info[nCount].AccountState == FIC_ACCOUNT_NORMAL)
				{
					nCount++;
					MakeUnPack( &FinanceICISO3Info[nCount-1], strLog, FIC_ACCOUNTINFO_LEN );
				}
				else
				{
					MakeUnPack( &FinanceICISO3Info[nCount], strLog, FIC_ACCOUNTINFO_LEN );
					strLog = "[������¾ƴ�]" + strLog;
				}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READACCOUNTINFO) ���� %d = %s", nCount-1, (LPSTR)(LPCTSTR)strLog);
			}
		}
		FICAccountCount = nCount;								// ���� ������ ��

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_READACCOUNTINFO) FICAccountCount = %d", FICAccountCount);

		fnFIC_SetFICDispInfo();									// ȭ�鿡 Format�� ����. ���� ��翡 ���� �޶���

		break;
	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_GETDATACSN :							// Get Data CSN. �Ѽ��� �����ڵ� 2004.09.16
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(FINANCEIC_PROC_GETDATACSN)");

		// APDU Error Check
		if ( memcmp( &ICRecvData[nRecvLen - IC_SW_LEN], ISO7816_SW_NO_ERROR,
				sizeof(IC_SW_LEN) ) != 0 )						// 0x90 0x00
		{
			// APDU Error, Get Response(61xx), Incorrect Length(6Cxx) ó��
			if ( fnICA_AnalyzeISOError(ICRecvData, nRecvLen) == FALSE )
				return nErrorReturn;
		}

		// Format Ex : 0x310000CA58982701020304059000
		// Total Size Check
		if ( nRecvLen != IC_FCMS_LEN        +					// ���� �ڵ�
						 sizeof(BYTE)       +					// 0xCA
						 FIC_CSN_LEN		+					// CSN Data
						 IC_SW_LEN          )					// Status Word
		{
			return nErrorReturn;								// Data Error
		}

		nOffset++;												// Add 0xCA Size

		// ī�� �Ϸù�ȣ
		memcpy( FinanceICInfo.CSN, ICRecvData + nOffset, FIC_CSN_LEN );
		nOffset += FIC_CSN_LEN;

		// ī�� �Ϸù�ȣ for Screen(BCD -> String)
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
		if ( nRecvLen != IC_FCMS_LEN		+					// ���� �ڵ�
						 sizeof(BYTE)		+					// ���� �ڵ� ����
						 FIC_RANDOM_LEN		+					// Card Random
						 FIC_RANDOM_LEN )						// ��ȣȭ�� ���� ��й�ȣ
		{
			return nErrorReturn;								// Data Error
		}

		nOffset++;												// ���� �ڵ� ����

		// Card Random
		memcpy( FinanceICInfo.CardRandom, ICRecvData + nOffset, FIC_RANDOM_LEN );
		nOffset += FIC_RANDOM_LEN;
 
		// ��ȣȭ�� ���� ��й�ȣ
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
			// APDU Error, Get Response(61xx), Incorrect Length(6Cxx) ó��
			if ( fnICA_AnalyzeISOError(ICRecvData, nRecvLen) == FALSE )
				return nErrorReturn;
		}

		switch(IdcVersion)
		{
		case EMV_BOARD:
			// Format Ex : 0x3100000BAD50951F77E4469000
			// Total Size Check
			if ( nRecvLen != IC_FCMS_LEN        +					// ���� �ڵ�
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
			if ( nRecvLen != IC_FCMS_LEN        +					// ���� �ڵ�
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
			// APDU Error, Get Response(61xx), Incorrect Length(6Cxx) ó��
			if ( fnICA_AnalyzeISOError(ICRecvData, nRecvLen) == FALSE )
				return nErrorReturn;
		}

		switch(IdcVersion)
		{
		case EMV_BOARD:
			// Format Ex : 0x310000D8F3598E5F423FF7E20D9759173D41B49000
			// Total Size Check
			if ( nRecvLen != IC_FCMS_LEN	+						// ���� �ڵ�
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
			if ( nRecvLen != IC_FCMS_LEN	+						// ���� �ڵ�
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
			// APDU Error, Get Response(61xx), Incorrect Length(6Cxx) ó��
			if ( fnICA_AnalyzeISOError(ICRecvData, nRecvLen) == FALSE )
				return nErrorReturn;
		}

		switch(IdcVersion)
		{
		case EMV_BOARD:
			// Format Ex : 0x310000035AAAD85E0228199000
			// Total Size Check
			if ( nRecvLen != IC_FCMS_LEN        +					// ���� �ڵ�
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
			if ( nRecvLen != IC_FCMS_LEN        +					// ���� �ڵ�
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
	// ����IC�ŷ��ΰ�쿡�� �м��� ������ FinanceICInfo����ü�� �� ���� ����������
	// ����ȭ�� �ŷ��ΰ�쿡�� �м��� ������ KCashInfo����ü�� �� ���� �����ϵ��� �Ѵ�
	/*==============================================================================*/
	// ------------------------------------------------------------------------
	case FINANCEIC_PROC_CHECKPIN:
	case FINANCEIC_PROC_VERIFYTRANBIT:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(����ȭ��)");
		break;

	case FINANCEIC_PROC_READPAN:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(����ȭ��) FINANCEIC_PROC_READPAN");
		memcpy(KCashInfo.Account, &ICRecvData[IC_FCMS_LEN], 10);
		break;

	case FINANCEIC_PROC_READBAMOUNT:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(����ȭ��) FINANCEIC_PROC_READBAMOUNT");
		memcpy(KCashInfo.Balance, &ICRecvData[3], 4);			// �ܾ�
		memcpy(KCashInfo.MaxLimitAmount, &ICRecvData[13], 4);	// �����ѵ��ݾ�
		break;

	case FINANCEIC_PROC_READTRAFFICINFO:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(����ȭ��) FINANCEIC_PROC_READTRAFFICINFO");
		memcpy(&KCashInfo.UserCode, &ICRecvData[3], 1);			// ����������
		memcpy(KCashInfo.ValidDate, &ICRecvData[4], 2);			// ��ȿ�Ⱓ
		break;

	case FINANCEIC_PROC_PREUNLOADRCV:
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(����ȭ��) FINANCEIC_PROC_PREUNLOADRCV");
		memcpy(KCashInfo.Account, &ICRecvData[3], 10);			// PAN����
		memcpy(KCashInfo.Balance, &ICRecvData[13], 4);			// �ܾ�	
		memcpy(KCashInfo.MaxLimitAmount, &ICRecvData[23], 4);	// �����ѵ��ݾ�
		KCashInfo.LoadUnloadAlgorithmID			= ICRecvData[33];
		KCashInfo.LoadUnloadKeyVersion			= ICRecvData[34];
		memcpy(KCashInfo.LoadUnloadSerialNo,	&ICRecvData[35], 2);
		memcpy(KCashInfo.LoadUnloadRandomNum,	&ICRecvData[37], 8);
		memcpy(KCashInfo.LoadUnloadEPMakerID,	&ICRecvData[45], 3);
		memcpy(KCashInfo.LoadUnloadEPID,		&ICRecvData[48], 5);
		memcpy(KCashInfo.LoadUnloadSignature,	&ICRecvData[53], 4);
		memcpy(KCashInfo.LoadUnloadProof, &ICRecvData[57], 7);	// Proof
		break;

	case FINANCEIC_PROC_EPBREADACCOUNTINFO :					// 1~10 �������� ��ȸ
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(��������): FINANCEIC_PROC_EPBREADACCOUNTINFO");
		nOffset++;												// �������� Command Index
		
		FICEPBAccountCount = ICRecvData[nOffset];				// ���� ����
		nOffset++;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_AnalRecvData(��������): FICEPBAccountCount = %d", FICEPBAccountCount);

		// Total Size Check : �����ڵ�(3) + 1 + ���°���(1) + ���°��� * (��������)
		nLength += IC_FCMS_LEN + 1 + 1 + FICEPBAccountCount * (1 + sizeof(FINANCEICEPBINFO));

		// �������� Set
		// ������ ī���ؼ��� �ȵȴ�. ���� ���µ� ������ ���� �� �ִ�.
		nCount  = 0;

		for (i = 0; i < FICEPBAccountCount; i++)
		{
			nOffset++;
			
			// ���� ����
			memcpy(&EPBAccountInfo[nCount], ICRecvData + nOffset, sizeof(FINANCEICEPBINFO));
			nOffset += sizeof(FINANCEICEPBINFO);

			// �������� ����. ���� ���� �Ǻ� AP ��ȯ �κп��� �ʿ�� �Ѵ�.
			if (EPBAccountInfo[nCount].AccountState != 0x00)
			{
				nCount++;
			}
		}

		FICEPBAccountCount = nCount;								// ���� ������ ��

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
* Description   : ���� IC Random Data ���� �Լ�. ���� ���ѿ� ����.
* Parameters    : lpSource : Buffer for Random Data
*                 nLen     : Length of lpSource
*                 Lv       : 0 : 0x00(16 Bytes). ���� �������� ����
*                            1 : N Bytes�� Random ����
*                            2 : N Bytes �ڸ� ��ġ Random ����
*                            3 : 8 Bytes Key ������ XOR
* Return        : (None)
* Release       : Ver 1.0
* Notes			: 2004.01.19 Han Su Woong
* ============================================================================*/
int CDevCmn::fnFIC_MakeRandom(LPBYTE lpSource, int nLen, int Lv)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_MakeRandom()");

	memset( lpSource, NULL, nLen );

	if ( Lv == 0 )									// ���� �������� ����
	{
		return T_OK;
	}

	srand( (unsigned)time(NULL) );

	BYTE bSeed;
	for ( int i = 0 ; i < nLen ; i++ )				// �Էµ� �ڸ����� ���� �߻�
	{
		bSeed = rand() % 0xFF;
		lpSource[i] = bSeed;
	}

	if ( Lv == 2 )									// N Bytes �ڸ� ��ġ Random ����
	{
	}

	if ( Lv == 3 )									// 8 Bytes Key ������ XOR
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
* Description   : ī�� ������ �̸� Get
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
* Description   : �Ǹ��� Get
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
* Description   : �Ǹ��ȣ Get
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
* Description   : ������ Get
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
* Description   : �˰��� �ĺ��ڵ� Get
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
* Description   : ������ �ĺ��ڵ� Get(���� �������� �ƴ� ī�� ��������)
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
* Description   : ī�� �Ϸù�ȣ Get
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
* Description   : ���� IC ���� ���� Get
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
* Description   : ���� IC �������� ���� �� ���»��� Get
* Parameters    : bIndex : ���� ������ Index
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
* Description   : ���� IC �������� ���� �� ���¹�ȣ Get For Screen.
* Parameters    : bIndex : ���� ������ Index
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

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetAccountNum() ���½��� ��ġ = %d", nAccountStart);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetAccountNum() ���¹�ȣ ���� = %d", nAccountNOCnt);

	// Byte->String
	memset( strTemp, NULL, sizeof(strTemp) );
	MakeUnPack( FinanceICISO3Info[bIndex - 1].ISO3, strTemp, FIC_ISO3_LEN, FALSE );

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetAccountNum() ISO3 Data = %s", strTemp);
	
	// 1                    : ISO 3 Track ���� �� ó�� ���� �� ����Ʈ
	// FIC_ISO3_ACCSTART : ���� IC ���� ���� ��ġ. ù��° �и���(=)
	// nAccountStart        : ���� ���� ��ġ���� ���� ��ȣ�� ��ġ
	memset( FICAccount, NULL, sizeof(FICAccount) );
	memcpy( FICAccount, &strTemp[nAccountStart+ FIC_ISO3_ACCSTART], __min(nAccountNOCnt, sizeof(FICAccount)) );

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetAccountNum() ���¹�ȣ = %s", FICAccount);

	return FICAccount;
}

/*==============================================================================
* Function Name : fnFIC_GetISO3()
* Description   : �ŷ��� ���õ� ������ ISO 3 Data Get. Host ������ ����
* Parameters    : bIndex : ���� ������ Index
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

	// ISO 3(108) -> Start 1, End 3 Bytes ���� -> ISO 3 Track(104Bytes)
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
* Description   : �ŷ��� ���õ� ���� ISO 3 Track ���� ���� �ڵ带 �����´�.
*                 fnFIC_GetISO3()���� ������ �Է��ϸ� ��Ÿ�� ���п� ���δ�.
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
* Description   : ȭ�鿡 ������ �������� Format�� ����. ���� ��翡 ���� �ٸ�
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

		// Account Number Set. Index 1���� �������� ����
		fnFIC_GetAccountNum( nAccountCnt + 1 );
		
		memset( FinanceICDisp[nAccountCnt].AccNum, 0x20, sizeof(FinanceICDisp[nAccountCnt].AccNum) );	// Buffer Clear
		memcpy( FinanceICDisp[nAccountCnt].AccNum, 
				FICAccount, 
				__min(sizeof(FinanceICDisp[nAccountCnt].AccNum), FinanceICISO3Info[nAccountCnt].AccountNOCnt));
		
		memset( FinanceICDisp[nAccountCnt].AccState, 0x20, sizeof(FinanceICDisp[nAccountCnt].AccState) );	// Buffer Clear

		nAccountCnt++;

//		if ( bState == FIC_ACCOUNT_NORMAL )
//		{
//			memcpy( FinanceICDisp[i].AccState, "����", 4 );
//		}
//		else if ( bState == FIC_ACCOUNT_CLOSE )
//		{
//			memcpy( FinanceICDisp[i].AccState, "����", 4 );
//		}	
//		else if ( bState == FIC_ACCOUNT_TEMPSTOP )
//		{
//			memcpy( FinanceICDisp[i].AccState, "�ӽ�����", 8 );
//		}
//		else if ( bState == FIC_ACCOUNT_TRANSFER )
//		{
//			memcpy( FinanceICDisp[i].AccState, "�̰�", 4 );
//		}
//		else if ( bState == FIC_ACCOUNT_ICCARD )
//		{
//			memcpy( FinanceICDisp[i].AccState, "����ȭ���", 10 );
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
* Description   : ��ȣȭ�� ���� ��й�ȣ Get
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
* Parameters    : bIndex : ENCData 1 or 2 ����
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
* Description   : Host�� ������ ���� IC ��ȣȭ Data�� ���ϴ� �Լ�
* Parameters    : MyBi : ���� ������ ���� �Ǻ�
* Return        : ���� IC      : Terminal Random(32) + Card Random(32)
*                                + ��ȣȭ�� ���� ��й�ȣ(32)
*                 MyBi ���� IC : ENC DATA 1(16) + ENC DATA 2(16) + 0x20(64)
* Release       : Ver 1.0
* Notes			: 2004.01.11 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnFIC_GetHostTranData(int IsMyBi)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetHostTranData(IsMyBi=%d)", IsMyBi);

	char szTemp[100];
	memset( FinanceICInfo.HostTranData, NULL, sizeof(FinanceICInfo.HostTranData) );

	//**************************************************************************
	//  ���� IC ȣ��Ʈ ���� Data
	//  Card Random(32) + Terminal Random(32) + ��ȣȭ�� ���� ��й�ȣ(32)
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
		
		// ��ȣȭ�� ���� ��й�ȣ(32)
		memset( szTemp, NULL, sizeof(szTemp) );
		MakeUnPack( FinanceICInfo.CipherPW, szTemp, FIC_RANDOM_LEN );
		memcpy( &FinanceICInfo.HostTranData[64], szTemp, 32);

		// File Key Version(2)
		memset( szTemp, NULL, sizeof(szTemp) );
		sprintf( szTemp, "%02X", FinanceICInfo.FCI_FileKeyVersion );
		memcpy( &FinanceICInfo.HostTranData[96], szTemp, 2 );
	}
	//**************************************************************************
	//  MyBi ���� IC ȣ��Ʈ ���� Data
	//  Card Random(32) + Terminal Random(32) + 
	//  ENC DATA 1(16) + ENC DATA 2(16) + 0x20(64) + File Key Version(2)
	//**************************************************************************
	else											// ���� IC ȣ��Ʈ ���� Data
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
* Description   : IBK e-������� ������������(�䱸��/���༺/����)�� �о� �´�.
* Parameters    : bIndex : 0x00(1 ~ 10), 0x01(11 ~ 20), 0x02(21 ~ 30)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2005.04.26 Nam Sung Ho
* History		: 1. 2005.04.26, V01.02.09, �ű��ۼ�
*                 2. 2005.12.21, V05.01.01, ������� Modify
* ============================================================================*/
int CDevCmn::fnFIC_EPBReadAccountInfo(BYTE bIndex)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_EPBReadAccountInfo(%d)", bIndex);

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

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

//������������������������������������������������������������������������������
// Function Name	: fnFIC_SetEPBDispInfo()
// Description		: ȭ�鿡 ������ �������� Format�� ����. ������¸� ǥ��.
// Parameters		: (NONE)
// Return			: (NONE)
// Release			: Ver V05-00-02-#01
// Notes			: 2005.12.27
//������������������������������������������������������������������������������
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

			if (EPBAccountInfo[i].EPBAccountKind == 0x33)		// ���Ű���
				FinanceICISO3Info[FICEPBAccountTotalCount].AccountNOCnt = 16;
			else												// �䱸��&���༺ ����
				FinanceICISO3Info[FICEPBAccountTotalCount].AccountNOCnt = 14;

			FinanceICEPBDisp[FICEPBAccountTotalCount].OrgIndex = FICEPBAccountTotalCount;
																// ���� INDEX
			FinanceICEPBDisp[FICEPBAccountTotalCount].EPBAccountKind = EPBAccountInfo[i].EPBAccountKind;
																// ��������

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

//������������������������������������������������������������������������������
// Function Name	: EPB_GetISO3Normal()
// Description		: ȭ�鿡�� ���õ� �������� ����
// Parameters		: (NONE)
// Return			: (NONE)
// Release			: Ver V05-00-02-#01
// Notes			: 2005.12.27
//������������������������������������������������������������������������������
int CDevCmn::fnFIC_GetEPBISO3(BYTE bIndex, LPSTR szISO3)
{
	char	szTemp[200]; //012345678901234567890123456
	char	szTempAcc[] = "B0159003=00000000000000000=";
	int		nCount(0);

	// ISO 3(108) -> Start 1, End 3 Bytes ���� -> ISO 3 Track(104Bytes)
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

//������������������������������������������������������������������������������
// Function Name	: fnFIC_GetEPBAccountCnt()
// Description		: e-������� ���� ���� Get
// Parameters		: (NONE)
// Return			: m_nEPBNAccCnt
// Release			: Ver V05-00-02-#01
// Notes			: 2006.01.10
//������������������������������������������������������������������������������
int CDevCmn::fnFIC_GetEPBAccountCnt()
{
	return FICEPBAccountTotalCount;								// ���� ���� ����
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
		fnAPL_StackError(fstrMCU_GetErrorCode(), "ī������(IFM)", DEV_MCU);
		DeviceStatus |= DEV_MCU;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_CheckIFMError():ERROR");
		return T_ERROR;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_CheckIFMError():OK");
	return T_OK;
}
