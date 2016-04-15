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
// �� �� �� : fnIMB_MyBiCardInfoRead()
// ��    �� : ī���� ���� ������ �ϰ��� �д´�(GET MYBI DATA)
// �Ķ���� : ����
// �����ڵ� : fnMCU_ICSendData()�� ���� ���
// ���ǻ��� : 
// �����丮 : 2004/07/07, �泲����, 01.00.00
//////////////////////////////////////////////////////////////////////////////
int	CDevCmn::fnIMB_MyBiCardInfoRead()
{
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiCardInfoRead()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	// Message ����
	ICSendData[0] = IC_FC_MAIN;									// 0x30
	ICSendData[1] = IC_MFC_MYBI;								// 0x30
	ICSendData[2] = IC_SFC_MYBICARDINFOREAD;					// 0x01

	strICSendData = MakeUnPack(ICSendData, IC_FCMS_LEN);
	nReturn = fnMCU_ICSendData(TRAN_IC_PROTOCOL_T1, strICSendData);

MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiCardInfoRead():return[%d]", nReturn);
	return nReturn;
}

//////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnIMB_MyBiLoadInit()
// ��    �� : ���� �Ǵ� ȯ���� ���� ī�� �ʱ�ȭ�� �Ѵ�
// ȣ���� : m_pDevCmn->fnIMB_MyBiCardInfoRead(m_pDevCmn->ParamLoadInit.Flag)
// �Ķ���� : ����
// �����ڵ� : fnMCU_ICSendData()�� ���� ���
// ���ǻ��� : 
// �����丮 : 2004/07/07, �泲����, 01.00.00
//////////////////////////////////////////////////////////////////////////////
int	CDevCmn::fnIMB_MyBiLoadInit(LPBYTE lpFlag, int nLen)
{
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiLoadInit()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	// Message ����
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
// �� �� �� : fnIMB_MyBiLoadEXE()
// ��    �� : ���� ������ �����ϴ�
// ȣ���� : m_pDevCmn->MybiLoadExe(m_pDevCmn->ParamLoadEXE.RandomNum, sizeof(m_pDevCmn->ParamLoadEXE))
// �Ķ���� : ����
// �����ڵ� : fnMCU_ICSendData()�� ���� ���
// ���ǻ��� : 
// �����丮 : 2004/07/07, �泲����, 01.00.00
//////////////////////////////////////////////////////////////////////////////
int	CDevCmn::fnIMB_MyBiLoadEXE(LPBYTE lpRandomNum, int RandomLen)
{
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiLoadEXE()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	// Message ����
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
// �� �� �� : fnIMB_MyBiUnloadEXE()
// ��    �� : ȯ�� ������ �����ϴ�
// ȣ���� : m_pDevCmn->fnIMB_MyBiUnloadEXE(m_pDevCmn->ParamLoadEXE.RandomNum, sizeof(m_pDevCmn->ParamLoadEXE))
// �Ķ���� : ����
// �����ڵ� : fnMCU_ICSendData()�� ���� ���
// ���ǻ��� : 
// �����丮 : 2004/07/07, �泲����, 01.00.00
//////////////////////////////////////////////////////////////////////////////
int	CDevCmn::fnIMB_MyBiUnloadEXE(LPBYTE lpRandomNum, int RandomLen)
{
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiUnloadEXE()");

	int		nReturn = FALSE;
	CString strICSendData("");

	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�

	// Message ����
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
// �� �� �� : fnIMB_MyBiIEPLock()
// ��    �� : �����ī�带 Lock �ϰų� Lock���¸� �����Ѵ�
// �Ķ���� : ����
// �����ڵ� : 
// ���ǻ��� : 
// �����丮 : 2004/07/07, �泲����, 01.00.00
//////////////////////////////////////////////////////////////////////////////
int	CDevCmn::fnIMB_MyBiIEPLock(BYTE * m_Data, INT Len)
{
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_MyBiIEPLock()");
	
	int		nReturn = FALSE;
	CString strICSendData("");
	
	memset(ICSendData, 0, sizeof(ICSendData));					// IC�����ڷ�
	
	// Message ����
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
// �� �� �� : fnIMB_AnalRecvData()
// ��    �� : ���źм�
// �Ķ���� : ����
// �����ڵ� : 
// ���ǻ��� : 
// �����丮 : 2004/07/07, �泲����, 01.00.00
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

//	Error�ڵ� Ȯ���ʿ�!! 2004.07.07
	if ( fnICA_CheckFCCode() != TRUE )
	{
		fnICA_SetFCError(ICRecvData);
		fnICA_AnalyzeFCError();
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(...) FCError Return!!!!");	
		return nReturn;
	}

	nOffset  += IC_FCMS_LEN;									// ���� �ڵ�
	nRecvLen /= 2;												// Pack Data Len

	switch (nCmdType)
	{
	case MYBI_PROC_CARDINFOREAD :								// ���̺� ī�������б�
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_CARDINFOREAD)");

		memcpy(MybiInfo.LOCKStatus, &ICRecvData[nOffset], sizeof(MybiInfo));
		nReturn = T_OK;

		break;

	case MYBI_PROC_LOADINIT :									// ���̺� ����/ȯ�� 1��
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_LOADINIT)");

		memcpy( MybiLoadInit.BeforeType, &ICRecvData[nOffset], sizeof(MybiLoadInit));
		nReturn = T_OK;

		break;

	case MYBI_PROC_LOADEXE :									// ���̺� ����2��
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_LOADEXE)");

		nReturn = T_OK;

		break;

	case MYBI_PROC_UNLOADEXE :									// ���̺� ȯ��2��
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_UNLOADEXE)");
		
		memcpy( rtnSign3, &ICRecvData[nOffset], sizeof(rtnSign3) );
		nReturn = T_OK;

		break;

	case MYBI_PROC_F204 :										// ���̺� �α������б�
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_F204)");

	break;

	case MYBI_PROC_F205 :										// ���̺� �α������б�
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_F205)");

	break;

	case MYBI_PROC_F206 :										// ���̺� �α������б�
MsgDump(TRACE_CODE_MODE, "Log",  "fnIMB_AnalRecvData(MYBI_PROC_F206)");

	break;

	case MYBI_PROC_LOCKCHECK:									// ���̺� UNLOCK REQUEST
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
		
		FinanceICInfo.RealNameNOLen = ICRecvData[nOffset];		// �Ǹ��ȣ ����
		nOffset++;
		
		FinanceICInfo.RealNameType = ICRecvData[nOffset];		// �Ǹ���
		nOffset++;
		
		if (FinanceICInfo.RealNameNOLen >= 1)
		{
			memcpy( FinanceICInfo.RealNameNO, ICRecvData + nOffset,
				FinanceICInfo.RealNameNOLen - 1 );					// �Ǹ��ȣ(�Ǹ��ȣ ���̴� �Ǹ��� 1Bytes�� ����)
			nOffset += FinanceICInfo.RealNameNOLen - 1;
		}
	}

	// ������ Tag
	if ( memcmp( FIC_TAG_EXPIREDAY, ICRecvData + nOffset,
		sizeof(FIC_TAG_EXPIREDAY) ) == 0 )
	{
		nOffset += sizeof(FIC_TAG_EXPIREDAY);
		
		FinanceICInfo.ExpireDayLen = ICRecvData[nOffset];		// ������ ����
		nOffset++;
		
		// ������(BCD -> String)
		MakeUnPack( &ICRecvData[nOffset], FinanceICInfo.ExpireDay, FinanceICInfo.ExpireDayLen );
		nOffset += FinanceICInfo.ExpireDayLen;
MsgDump(TRACE_CODE_MODE, "Log",  "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) ������ = %s", FinanceICInfo.ExpireDay);
	}


	// �˰��� �ĺ��ڵ� Tag
	if ( FIC_TAG_ALGOCODE == ICRecvData[nOffset] )
	{
		nOffset += sizeof(BYTE) + sizeof(BYTE);					// �˰��� �ĺ��ڵ� ����
		
		FinanceICInfo.AlgorithmCode = ICRecvData[nOffset];		// �˰��� �ĺ��ڵ�
		nOffset++;
	}

	// ������ �ĺ��ڵ� Tag(�ʼ�)
	if ( FIC_TAG_ISSURECODE != ICRecvData[nOffset] )
		return nReturn;											// Data Error
	nOffset++;

	FinanceICInfo.IssureCodeLen = ICRecvData[nOffset];			// ������ �ĺ��ڵ� ����
	nOffset++;

	// ������ �ĺ��ڵ�(BCD -> String)
	MakeUnPack( &ICRecvData[nOffset], FinanceICInfo.IssureCode, FinanceICInfo.IssureCodeLen );
	nOffset += FinanceICInfo.IssureCodeLen;
MsgDump(TRACE_CODE_MODE, "Log",  "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) ������ �ĺ��ڵ� = %s", FinanceICInfo.IssureCode);

	// ī�� �Ϸù�ȣ Tag(�ʼ�)
	if ( FIC_TAG_CSN != ICRecvData[nOffset] )
		return nReturn;											// Data Error
	nOffset += sizeof(BYTE) + sizeof(BYTE);						// ī�� �Ϸù�ȣ ����

	// ī�� �Ϸù�ȣ
	memcpy( FinanceICInfo.CSN, ICRecvData + nOffset, FIC_CSN_LEN );
	nOffset += FIC_CSN_LEN;

	// ī�� �Ϸù�ȣ for Screen(BCD -> String)
	MakeUnPack( FinanceICInfo.CSN, FinanceICInfo.szCSN, FIC_CSN_LEN );
MsgDump(TRACE_CODE_MODE, "Log",  "fnFIC_AnalRecvData(FINANCEIC_PROC_READBASICINFO) CSN = %s", FinanceICInfo.szCSN);

	return nReturn;
}

