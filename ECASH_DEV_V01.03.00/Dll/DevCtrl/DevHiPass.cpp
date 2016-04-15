 /////////////////////////////////////////////////////////////////////////////
//	DevHiPass.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\\CmnLibIn.h"
#include "..\..\H\Common\\Define.h"
#include "..\..\H\Common\\ConstDef.h"
#include "..\..\H\Common\\ClassInclude.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevICCard.h"
#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	HiPass�Լ�) : HI-PASS OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// ��������
int	CDevCmn::fnHPS_PowerOn()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_PowerOn()");
	
	int		nReturn = FALSE;
	CString strICSendData("0");	strICSendData.Empty();
	
	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�
	
	// Message ����
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_BASIC;								// 0x01
	ICSendData[2] = IC_SFC_POWERON;								// 0x01
	
	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_PowerOn():return[%d]", nReturn);
	return nReturn;
}

// ��������
int	CDevCmn::fnHPS_PowerOff()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_PowerOff()");
	
	int		nReturn = FALSE;
	CString strICSendData("0");	strICSendData.Empty();
	
	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�
	
	// Message ����
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_BASIC;								// 0x01
	ICSendData[2] = IC_SFC_POWEROFF;							// 0x04
	
	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_PowerOff():return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnHPS_ReadPan
* Description   : �����н� PAN File Read
* Parameters    : 
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 
* History		: 
* ============================================================================*/
int	CDevCmn::fnHPS_ReadPan()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadPan()");
	
	int		nReturn = FALSE;
	BYTE	bAPDUData[IC_APDU_LEN] = {NULL, };
	int		nAPDULen = 0;
	
	//**********************************************************************
	//  Command Construction
	//**********************************************************************
	nAPDULen = fnICA_ConstructAPDU
					( bAPDUData,
					  ISO_7816_CASE2,
					  CLA_7816_STANDARD,						// 0x00
					  INS_7816_READ_BINARY,						// 0xB0
					  P1_7816_SELECTION_BY_HPSPAN,				// 0x82
					  P1P2_7816_ZERO,							// 0x00
					  HPS_READPAN_RECVLEN);						// 0x0A

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadPan()", bAPDUData, nAPDULen);
	nReturn = fnICA_ISOOut( bAPDUData, nAPDULen );
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadPan():return[%d]", nReturn);
	return nReturn;
}

/* 2008.08.05 �系 ��å�������� �����ڵ� ������
int	CDevCmn::fnHPS_ReadPan()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadPan()");
	
	int		nReturn = FALSE;
	CString strICSendData("");
	
	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�
	
	// Message ����
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_HPS;									// 0x0C
	ICSendData[2] = IC_SFC_HPSREADPAN;							// 0x07

	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadPan():return[%d]", nReturn);
	return nReturn;
}
*/

/*==============================================================================
* Function Name : fnHPS_ReadBalance()
* Description   : �ܾ���ȸ
* Parameters    : 
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 
* History		:
* ============================================================================*/
int	CDevCmn::fnHPS_ReadBalance()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadBalance()");
	
	int		nReturn = FALSE;
	BYTE	bAPDUData[IC_APDU_LEN] = {NULL, };
	int		nAPDULen = 0;
	
	//**********************************************************************
	//  Command Construction
	//**********************************************************************
	nAPDULen = fnICA_ConstructAPDU
					( bAPDUData,
					  ISO_7816_CASE2,
					  CLA_USER_STANDARD,						// 0x90
					  INS_HPS_READBALANCE,						// 0x5C
					  P1P2_7816_ZERO,							// 0x00
					  P1P2_7816_ZERO,							// 0x00
					  HPS_REALBAL_RECVLEN );					// 0x04
	
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadBalance()", bAPDUData, nAPDULen);
	nReturn = fnICA_ISOOut( bAPDUData, nAPDULen );
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadBalance():return[%d]", nReturn);
	return nReturn;
}

/* 2008.08.05 �系 ��å�������� �����ڵ� ������
int	CDevCmn::fnHPS_ReadBalance()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadBalance()");
	
	int		nReturn = FALSE;
	CString strICSendData("");
	
	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�
	
	// Message ����
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_HPS;									// 0x0C
	ICSendData[2] = IC_SFC_HPSREADJAN;							// 0x02
	
	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadBalance():return[%d]", nReturn);
	return nReturn;
}
*/

/*==============================================================================
* Function Name : fnHPS_PrepareLoadIEP
* Description   : ������ó��(Initialize IEP) 
* Parameters    : szICSendData - �������� ������ �����ݾ�(16����)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 
* History		:
* ============================================================================*/
int	CDevCmn::fnHPS_PrepareLoadIEP(LPBYTE lpICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_PrepareLoadIEP():lpICSendData[%s]", lpICSendData);

	int		nReturn = FALSE;
	BYTE	bAPDUData[IC_APDU_LEN] = {NULL, };
	int		nAPDULen = 0;
	
	//**********************************************************************
	//  Command Construction
	//**********************************************************************
	nAPDULen = fnICA_ConstructAPDU
					( bAPDUData,
					  ISO_7816_CASE4,
					  CLA_USER_STANDARD,						// 0x90
					  INS_HPS_PRELOADIEP,						// 0x50
					  P1P2_7816_ZERO,							// 0x00
					  P1P2_7816_ZERO,							// 0x00
					  HPS_INITLOAD_LEN,							// 0x04
					  lpICSendData,								// �ݾ�
					  HPS_INITLOAD_RECVLEN);					// 0x2C
	
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_PrepareLoadIEP()", bAPDUData, nAPDULen);
	nReturn = fnICA_ISOIn( bAPDUData, nAPDULen );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_PrepareLoadIEP(...):return[%d]", nReturn);
	return nReturn;
}

/* 2008.08.05 �系 ��å�������� �����ڵ� ������
int	CDevCmn::fnHPS_PrepareLoadIEP(LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_PrepareLoadIEP()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	// Message ����
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_HPS;									// 0x0C
	ICSendData[2] = IC_SFC_HPSPRELOAD;							// 0x03

	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szICSendData;
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_PrepareLoadIEP(...):return[%d]", nReturn);

	return nReturn;
}
*/

/*==============================================================================
* Function Name : fnHPS_LoadIEP
* Description   : �����䱸(����4+����8) 
* Parameters    : szICSendData - ���������κ��� ���� ����(4) + ����(8)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 
* History		:
* ============================================================================*/
int	CDevCmn::fnHPS_LoadIEP(LPBYTE lpICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_LoadIEP(lpICSendData[%s])", lpICSendData);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_LoadIEP", lpICSendData, strlen((char *)lpICSendData));

	int		nReturn = FALSE;
	BYTE	bAPDUData[IC_APDU_LEN] = {NULL, };
	int		nAPDULen = 0;
	
	//**********************************************************************
	//  Command Construction
	//**********************************************************************
	nAPDULen = fnICA_ConstructAPDU
					( bAPDUData,
					  ISO_7816_CASE4,
					  CLA_USER_STANDARD,						// 0x90
					  INS_HPS_LOADIEP,							// 0x52
					  P1P2_7816_ZERO,							// 0x00
					  P1P2_7816_ZERO,							// 0x00
					  HPS_LOADIEP_LEN,							// 0x0C
					  lpICSendData,								// �ݾ�
					  HPS_LOADIECP_RECVLEN);					// 0x04
	
	nReturn = fnICA_ISOIn( bAPDUData, nAPDULen );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_LoadIEP(...):return[%d]", nReturn);
	return nReturn;
}

/* 2008.08.05 �系 ��å�������� �����ڵ� ������
int	CDevCmn::fnHPS_LoadIEP(LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_LoadIEP(szICSendData[%s])", szICSendData);
	
	int		nReturn = FALSE;
	CString strICSendData("");
	
	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�
	
	// Message ����
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_HPS;									// 0x0C
	ICSendData[2] = IC_SFC_HPSLOAD;								// 0x04
	
	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN) + szICSendData;
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_LoadIEP(...):return[%d]", nReturn);
	return nReturn;
}
*/

/*==============================================================================
* Function Name : fnHPS_ReadLLog
* Description   : ��ġ���� �ŷ�����(���ڵ� ����)
* Parameters    : 
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 
* History		:
* ============================================================================*/
int	CDevCmn::fnHPS_ReadLLog()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadLLog()");
	
	int		nReturn = FALSE;
	BYTE	bAPDUData[IC_APDU_LEN] = {NULL, };
	int		nAPDULen = 0;
	
	//**********************************************************************
	//  Command Construction
	//**********************************************************************
	nAPDULen = fnICA_ConstructAPDU
					( bAPDUData,
					  ISO_7816_CASE2,
					  CLA_7816_STANDARD,						// 0x00
					  INS_7816_READ_RECORD,						// 0xB2
					  P1_7816_SELECTION_BY_FIRSTFILE,			// 0x01
					  P2_7816_SELECTION_BY_HPSLLOG,				// 0x34
					  HPS_READLLOG_RECVLEN);					// 0x24

	nReturn = fnICA_ISOOut( bAPDUData, nAPDULen );

	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadLLog():return[%d]", nReturn);
	return nReturn;
}

/* 2008.08.05 �系 ��å�������� �����ڵ� ������
int	CDevCmn::fnHPS_ReadLLog(BYTE btData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadLLog()");
	
	int		nReturn = FALSE;
	CString strICSendData("");
	
	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�
	
	// Message ����
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_HPS;									// 0x0C
	ICSendData[2] = IC_SFC_HPSREADLLOG;							// 0x06
	ICSendData[3] = btData;										// 0x00: ��ü ����Ÿ, 0x01 ~ 0x14 : �ش緹�ڵ� 

	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN  + sizeof(BYTE));
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadLLog():return[%d]", nReturn);
	return nReturn;
}
*/

/*==============================================================================
* Function Name : fnHPS_CompleteTran
* Description   : ����/ȯ�Ұŷ��� ���ۺ�Ʈ ����
* Parameters    : 
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 
* History		: 
* ============================================================================*/
int	CDevCmn::fnHPS_CompleteTran()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_CompleteTran()");
	
	int		nReturn = FALSE;
	BYTE	bAPDUData[IC_APDU_LEN] = {NULL, };
	int		nAPDULen = 0;
	
	//**********************************************************************
	//  Command Construction
	//**********************************************************************
	nAPDULen = fnICA_ConstructAPDU
					( bAPDUData,
					  ISO_7816_CASE3,//ISO_7816_CASE1,
					  CLA_USER_STANDARD,						// 0x90
					  INS_HPS_COMPLETE,							// 0x5E
					  P1P2_7816_ZERO,							// 0x00
					  P1P2_7816_ZERO);						// 0x00

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_CompleteTran()", bAPDUData, nAPDULen);
	nReturn = fnICA_ISOIn( bAPDUData, nAPDULen );
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_CompleteTran():return[%d]", nReturn);
	return nReturn;
}

/* 2008.08.05 �系 ��å�������� �����ڵ� ������
int	CDevCmn::fnHPS_CompleteTran()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_CompleteTran()");
	
	int		nReturn = FALSE;
	CString strICSendData("");
	
	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�
	
	// Message ����
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_HPS;									// 0x0C
	ICSendData[2] = IC_SFC_HPSCOMPLETE;							// 0x05

	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_CompleteTran():return[%d]", nReturn);
	return nReturn;
}
*/

/*==============================================================================
* Function Name : fnHPS_AnalRecvData
* Description   : ���źм� �Լ� 
* Parameters    : nCmdType - �м��� ����
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 
* History		: 
* ============================================================================*/
int CDevCmn::fnHPS_AnalRecvData(int	nCmdType)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_AnalRecvData(%d)",nCmdType);

	int		nLength(0);
	int		nReturn = 0, nErrorReturn = 9999;
	CString	strICRecvData("");
	int		i = 0;
	int		RecvLen = 0;
	int		nOffset = 0;
	int		nTemp = 0, nHex = 0;

	memset(ICRecvData, 0, sizeof(ICRecvData));
	strICRecvData = fstrMCU_GetICData();

	MakePack(strICRecvData.GetBuffer(0), ICRecvData, __min(sizeof(ICRecvData) * 2, strICRecvData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_AnalRecvData:ICRecvData", ICRecvData, 
			__min(sizeof(ICRecvData) * 2, strICRecvData.GetLength()) / 2);

	RecvLen = (strICRecvData.GetLength() / 2);

/////////////////////////////////////////////////////////////////////////////
	if ((!memcmp(ICRecvData, IC_RECV_OK, 3))	||				// ����
		(!memcmp(ICRecvData, IC_RECV_OK2, 3))	)
		;
	else														// ������
	{
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

	if (nReturn)
	{
		// �Ҵ��ڵ�
		MakeUnPack(&ICRecvData[2], RFHipassErrorCode, 2);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_AnalRecvData():return(%d) - RFHipassErrorCode[%4.4s]", nReturn, RFHipassErrorCode);
		return nReturn;
	}

	nOffset  += IC_FCMS_LEN;									// ���� �ڵ�

	switch(nCmdType)
	{
		case HPS_PROC_SELECTPSE:
			if (!FICDFType)										// Select�� DF�� �˼� ������..
				return nErrorReturn;

			// FCI Template Tag(�ʼ�)
			if ( HIC_HPSS_TAG_FCI != ICRecvData[nOffset] )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset += 1;										// Tag Length
			nOffset += 1;										// Data Length
			
			// DF Name Tag(�ʼ�)
			if ( HIC_HPSS_TAG_DFNAME != ICRecvData[nOffset] )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset++;											// DFNAME Tag Length
			nLength = ICRecvData[nOffset];						// DF Name Length
			nOffset++;											// DF NAME Data Length
			nOffset += nLength;									// Add DF Name Data Length
			
			// FCI Proprietary Template Tag(�ʼ�)
			if ( HIC_HPSS_TAG_PROP != ICRecvData[nOffset] )
			{
				return nErrorReturn;							// Data Error
			}
			nOffset += 1;										// FCI Proprietary Template Tag Length
			nOffset += 1;										// FCI Proprietary Template Data Length
			
			// Application Label Tag
			if ( HIC_HPSS_TAG_SFI == ICRecvData[nOffset] )
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
			if ( memcmp( HIC_HPSS_TAG_FILEINFO, &ICRecvData[nOffset],
				sizeof(HIC_HPSS_TAG_FILEINFO) ) != 0 )
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
		case HPS_PROC_SELECTAPPLICATION:
			break;
		case HPS_PROC_READBALANCE:
//			memcpy(HiPassInfo.Balance, &ICRecvData[nOffset], 4);
			MakeUnPack(&ICRecvData[nOffset], RFHipassBalance.Balance, 4);

			for (i = 0; i < 8; i++)
			{
				if (('0' <= RFHipassBalance.Balance[i])	&&
					(RFHipassBalance.Balance[i] <= '9')	)
					nTemp = RFHipassBalance.Balance[i] - '0';
				else
				if (('A' <= RFHipassBalance.Balance[i])	&&
					(RFHipassBalance.Balance[i] <= 'F')	)
					nTemp = RFHipassBalance.Balance[i] - 'A' + 10;
				else
				if (('a' <= RFHipassBalance.Balance[i])	&&
					(RFHipassBalance.Balance[i] <= 'f')	)
					nTemp = RFHipassBalance.Balance[i] - 'a' + 10;
				else
					nTemp = 0;

				nHex = (nHex * 16) + nTemp;
			}
			Int2Asc(nHex, RFHipassBalance.Balance, 8);

			break;
		case HPS_PROC_HIPASSPRELOAD:
			memset(&RFHipassLoad1, 0x00, sizeof(RFHipassLoad1));// Hi-Pass ����S1����

			// APDU Error Check
			if ( memcmp( &ICRecvData[RecvLen - IC_SW_LEN], ISO7816_SW_NO_ERROR,
				sizeof(IC_SW_LEN) ) != 0 )						// 0x90 0x00
			{
				// APDU Error, Get Response(61xx), Incorrect Length(6Cxx) ó��


				if ( fnICA_AnalyzeISOError(ICRecvData, RecvLen) == FALSE )
				{
					// �����ڵ�
					HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnICA_AnalyzeISOError", &ICRecvData, sizeof(ICRecvData));
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvLen[%d]",	RecvLen);
					if ( memcmp( &ICRecvData[RecvLen - IC_SW_LEN], ISO7816_SW_NO_ERROR,
						sizeof(IC_SW_LEN) ) != 0 )						// 0x90 0x00
					{
						;
					}
					else
					{
						MakeUnPack(&ICRecvData[RecvLen - IC_SW_LEN], RFHipassErrorCode, 2);
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassErrorCode[%4.4s]",	RFHipassErrorCode);
						return nErrorReturn;
					}	
				}
			}

			// �˰��� ID
//			HiPassInfo.AlgorithmID	= ICRecvData[nOffset];
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.AlgorithmID, sizeof(RFHipassLoad1.AlgorithmID) / 2);
			nOffset += sizeof(RFHipassLoad1.AlgorithmID) / 2;
			// Ű����
//			HiPassInfo.KeyVersion	= ICRecvData[nOffset];
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.KeyVersion, sizeof(RFHipassLoad1.KeyVersion) / 2);
			nOffset += sizeof(RFHipassLoad1.KeyVersion) / 2;
			// ���ΰ��� ����ī�� �ŷ��Ϸù�ȣ
//			memcpy(HiPassInfo.PPCardCSN, &ICRecvData[nOffset], sizeof(HiPassInfo.PPCardCSN));
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.CardTranSeq, sizeof(RFHipassLoad1.CardTranSeq) / 2);
			nOffset += sizeof(RFHipassLoad1.CardTranSeq) / 2;
			// ���ΰ��� ����ī�忡�� ������ ����
//			memcpy(HiPassInfo.PPCardRandomNum, &ICRecvData[nOffset], sizeof(HiPassInfo.PPCardRandomNum));
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.RandomNum, sizeof(RFHipassLoad1.RandomNum) / 2);
			nOffset += sizeof(RFHipassLoad1.RandomNum) / 2;
			// ���ΰ��� ����ī�� ������ID
//			memcpy(HiPassInfo.PPCardEPMakerID, &ICRecvData[nOffset], sizeof(HiPassInfo.PPCardEPMakerID));
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.PublisherID, sizeof(RFHipassLoad1.PublisherID) / 2);
			nOffset += sizeof(RFHipassLoad1.PublisherID) / 2;
			// ���ΰ��� ����ī�� ID
//			memcpy(HiPassInfo.PPCardEPID, &ICRecvData[nOffset], sizeof(HiPassInfo.PPCardEPID));
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.CardID, sizeof(RFHipassLoad1.CardID) / 2);
			nOffset += sizeof(RFHipassLoad1.CardID) / 2;
			// ���ΰ��� ����ī�� �ܾ�
//			memcpy(HiPassInfo.PPCardBalance, &ICRecvData[nOffset], sizeof(HiPassInfo.PPCardBalance));
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.CardBalance, sizeof(RFHipassLoad1.CardBalance) / 2);
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.CardBalanceHEX, sizeof(RFHipassLoad1.CardBalanceHEX) / 2);
			

			
			for (i = 0; i < 8; i++)
			{
				if (('0' <= RFHipassLoad1.CardBalance[i])	&&
					(RFHipassLoad1.CardBalance[i] <= '9')	)
					nTemp = RFHipassLoad1.CardBalance[i] - '0';
				else
				if (('A' <= RFHipassLoad1.CardBalance[i])	&&
					(RFHipassLoad1.CardBalance[i] <= 'F')	)
					nTemp = RFHipassLoad1.CardBalance[i] - 'A' + 10;
				else
				if (('a' <= RFHipassLoad1.CardBalance[i])	&&
					(RFHipassLoad1.CardBalance[i] <= 'f')	)
					nTemp = RFHipassLoad1.CardBalance[i] - 'a' + 10;
				else
					nTemp = 0;

				nHex = (nHex * 16) + nTemp;
			}
			Int2Asc(nHex, RFHipassLoad1.CardBalance, 8);
			
			
			nOffset += sizeof(RFHipassLoad1.CardBalance) / 2;
			// ���ΰ��� �����ȣ(PAN)
//			memcpy(HiPassInfo.PPCardPAN, &ICRecvData[nOffset], sizeof(HiPassInfo.PPCardPAN));
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.LedgerNum, sizeof(RFHipassLoad1.LedgerNum) / 2);
			nOffset += sizeof(RFHipassLoad1.LedgerNum) / 2;
			// ���ΰ��� ����ī����� �����ĺ���ȣ(����SAM)
//			HiPassInfo.PPCardCenterID = ICRecvData[nOffset];
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.UniqueNum, sizeof(RFHipassLoad1.UniqueNum) / 2);
			nOffset += sizeof(RFHipassLoad1.UniqueNum) / 2;
			// ī�� ����ID
//			memcpy(HiPassInfo.CardServiceID, &ICRecvData[nOffset], sizeof(HiPassInfo.CardServiceID));
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.CardServiceID, sizeof(RFHipassLoad1.CardServiceID) / 2);
			nOffset += sizeof(RFHipassLoad1.CardServiceID) / 2;
			// ���ΰ��� ����ī�忡�� ���� ����
//			memcpy(HiPassInfo.PPCardSign1, &ICRecvData[nOffset], sizeof(HiPassInfo.PPCardSign1));
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad1.SIGN1, sizeof(RFHipassLoad1.SIGN1) / 2);
			nOffset += sizeof(RFHipassLoad1.SIGN1) / 2;
			// �����н�ī���ȣ
			memcpy(RFHipassLoad1.CardNum, &RFHipassLoad1.LedgerNum[4], 16);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.Card Number[%16.16s]",	RFHipassLoad1.CardNum);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.AlgorithmID[%2.2s]",		RFHipassLoad1.AlgorithmID);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.Key Version[%2.2s]",		RFHipassLoad1.KeyVersion);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.CardTran Seq[%8.8s]",	RFHipassLoad1.CardTranSeq);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.Random Number[%16.16s]",	RFHipassLoad1.RandomNum);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.PublisherID[%6.6s]",		RFHipassLoad1.PublisherID);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.CardID[%10.10s]",		RFHipassLoad1.CardID);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.Card Balance[%10.10s]",	RFHipassLoad1.CardBalance);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.Ledger Number[%20.20s]",	RFHipassLoad1.LedgerNum);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.Unique Number[%2.2s]",	RFHipassLoad1.UniqueNum);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.Card ServiceID[%6.6s]",	RFHipassLoad1.CardServiceID);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassLoad1.SIGN1[%8.8s]",			RFHipassLoad1.SIGN1);
			break;
		case HPS_PROC_HIPASSLOAD:
			memset(&RFHipassLoad2, 0x00, sizeof(RFHipassLoad2));// Hi-Pass ����S2����

			// APDU Error Check
			if ( memcmp( &ICRecvData[RecvLen - IC_SW_LEN], ISO7816_SW_NO_ERROR,
				sizeof(IC_SW_LEN) ) != 0 )						// 0x90 0x00
			{
				// APDU Error, Get Response(61xx), Incorrect Length(6Cxx) ó��
				if ( fnICA_AnalyzeISOError(ICRecvData, RecvLen) == FALSE )
				{
					// �����ڵ�
					MakeUnPack(&ICRecvData[RecvLen - IC_SW_LEN], RFHipassErrorCode, 2);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassErrorCode[%4.4s]",	RFHipassErrorCode);
					return nErrorReturn;
				}
			}

			// ���ΰ��� ����ī�忡�� ���� ���� 
			MakeUnPack(&ICRecvData[nOffset], RFHipassLoad2.SIGN3, sizeof(RFHipassLoad2.SIGN3) / 2);
			nOffset += sizeof(RFHipassLoad2.SIGN3) / 2;
			break;
		case HPS_PROC_HIPASSRMVTRANBIT:
			break;
		case HPS_PROC_HIPASSLLOG:
			//HiPassLLog.PPCardTranType = ICRecvData[nOffset];
			nOffset++;		// Tag
			nOffset++;		// Length
			// �ŷ�����
			HiPassLLog.PPCardTranType = ICRecvData[nOffset];
			nOffset++;
			// �ŷ��Ϸù�ȣ
			memcpy(HiPassLLog.PPCardTranSeqNum, &ICRecvData[nOffset], sizeof(HiPassLLog.PPCardTranSeqNum));
			nOffset += sizeof(HiPassLLog.PPCardTranSeqNum);
			// �ŷ��� �ܾ�
			memcpy(HiPassLLog.PPCardTranBalance, &ICRecvData[nOffset], sizeof(HiPassLLog.PPCardTranBalance));
			nOffset += sizeof(HiPassLLog.PPCardTranBalance);
			// �ŷ��ݾ�
			memcpy(HiPassLLog.PPCardTranAmount, &ICRecvData[nOffset], sizeof(HiPassLLog.PPCardTranAmount));
			nOffset += sizeof(HiPassLLog.PPCardTranAmount);
			// �Ϸ��ڵ�
			memcpy(HiPassLLog.PPCardCompleteCode, &ICRecvData[nOffset], sizeof(HiPassLLog.PPCardCompleteCode));
			nOffset += sizeof(HiPassLLog.PPCardCompleteCode);
			// �������
			HiPassLLog.PPCardLoadOrg = ICRecvData[nOffset];
			nOffset ++;
			// �����ҹ�ȣ
			memcpy(HiPassLLog.PPCardBranchNum, &ICRecvData[nOffset], sizeof(HiPassLLog.PPCardBranchNum));
			nOffset += sizeof(HiPassLLog.PPCardBranchNum);
			// �ܸ���ID
			HiPassLLog.PPCardTerminalID = ICRecvData[nOffset];
			nOffset ++;
			// ��������
			memcpy(HiPassLLog.PPCardLoadDate, &ICRecvData[nOffset], sizeof(HiPassLLog.PPCardLoadDate));
			nOffset += sizeof(HiPassLLog.PPCardLoadDate);
			// �����ð�
			memcpy(HiPassLLog.PPCardLoadTime, &ICRecvData[nOffset], sizeof(HiPassLLog.PPCardLoadTime));
			nOffset += sizeof(HiPassLLog.PPCardLoadTime);
			// ������ID
			memcpy(HiPassLLog.EPMakerID, &ICRecvData[nOffset], sizeof(HiPassLLog.EPMakerID));
			nOffset += sizeof(HiPassLLog.EPMakerID);
			// LSAM ID
			memcpy(HiPassLLog.LSAMID, &ICRecvData[nOffset], sizeof(HiPassLLog.LSAMID));
			nOffset += sizeof(HiPassLLog.LSAMID);
			break;
		case HPS_PROC_HIPASSREADPAN:
			nOffset += 2;										// ī���ȣ�� PAN 4��°����
			MakeUnPack(&ICRecvData[nOffset], RFHipassBalance.CardNum, sizeof(RFHipassBalance.CardNum) / 2);
			nOffset += sizeof(RFHipassBalance.CardNum) / 2;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RFHipassBalance.CardNum [%16.16s]", RFHipassBalance.CardNum);
			break;
		case HPS_PROC_HIPASSCOMPLETE:
			break;
		default:
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_AnalRecvData():return(nReturn[%d])", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnFIC_GetData()
* Description   : IC Card GetData �Լ�
* Parameters    : nCmdType : �м��� ���� �޽����� ����
*				  FINANCEIC_GETDATA_XXXX   | �� ��ȸData�� ���� ó��
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2008.06.24 Edit by Seo Myong won
* ============================================================================*/
/*
int CDevCmn::fnHPS_APDUInitialize(LPCTSTR szICSendData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetData(CmdType[%s])", szICSendData);

	int		nReturn = FALSE;
	BYTE	bAPDUData[IC_APDU_LEN];
	int		nAPDULen;
	CString strICSendData("");

	BYTE	P1_Command = 0x00;
	BYTE	P2_Command = 0x00;
	BYTE	Le_Command = 0x2C;
	BYTE	Lc_Command = 0x04;

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	//**********************************************************************
	//  Command Construction
	//**********************************************************************
	nAPDULen = fnICA_ConstructAPDU
					( bAPDUData,
					  ISO_7816_CASE4,
					  0x90,						// 0x00
					  0x50,						// 0xCA
					  P1_Command,
					  P2_Command,
					  Lc_Command,
					  (BYTE *) szICSendData,
					  Le_Command );

	nReturn = fnICA_ISOOut( bAPDUData, nAPDULen );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFIC_GetData():return[%d]", nReturn);
	return nReturn;
}

int CDevCmn::fnHPS_APDUBalRead()
{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_APDUBalRead()");

	int		nReturn = FALSE;
	BYTE	bAPDUData[IC_APDU_LEN];
	int		nAPDULen;
	CString strICSendData("");

	BYTE	P1_Command = 0x00;
	BYTE	P2_Command = 0x00;
	BYTE	Le_Command = 0x04;

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	//**********************************************************************
	//  Command Construction
	//**********************************************************************
	nAPDULen = fnICA_ConstructAPDU
					( bAPDUData,
					  ISO_7816_CASE2,
					  0x90,						// 0x00
					  0x5C,						// 0xCA
					  P1_Command,
					  P2_Command,
					  Le_Command );

	nReturn = fnICA_ISOOut( bAPDUData, nAPDULen );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_APDUBalRead():return[%d]", nReturn);
	return nReturn;
}

int CDevCmn::fnHPS_ReadPAN()
{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadPAN()");

	int		nReturn = FALSE;
	BYTE	bAPDUData[IC_APDU_LEN];
	int		nAPDULen;
	CString strICSendData("");

	BYTE	P1_Command = 0x82;
	BYTE	P2_Command = 0x00;
	BYTE	Le_Command = 0x0A;

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	//**********************************************************************
	//  Command Construction
	//**********************************************************************
	nAPDULen = fnICA_ConstructAPDU
					( bAPDUData,
					  ISO_7816_CASE2,
					  0x00,						// 0x00
					  0xB0,						// 0xCA
					  P1_Command,
					  P2_Command,
					  Le_Command );

	nReturn = fnICA_ISOOut( bAPDUData, nAPDULen );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_ReadPAN():return[%d]", nReturn);
	return nReturn;
}
*/