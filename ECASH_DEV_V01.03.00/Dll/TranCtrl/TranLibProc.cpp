/////////////////////////////////////////////////////////////////////////////
// TranLibProc.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#include "..\..\H\EMVInc\\AppErrCode.h"	
#include "..\..\H\EMVInc\\SCSp_ATM_EMV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	Library Function
/////////////////////////////////////////////////////////////////////////////
// Format Mac Data
int	CTranCmn::fnAPLLib_FormatMacData(LPCTSTR SourceData, int& Length, LPSTR DeData)
{
	int		i = 0; 
	int		j = 0;
	char	MacBuffer1[NETBUFFSIZE];
	char	MacBuffer2[NETBUFFSIZE];

	memset(MacBuffer1, 0, sizeof(MacBuffer1));
	memset(MacBuffer2, 0, sizeof(MacBuffer2));
	
	memcpy(MacBuffer1, SourceData, Length);

	for (i = 0 ; i < Length ; i++)
	{
		if (MacBuffer1[i] >= 'a' && MacBuffer1[i] <= 'z')
			MacBuffer2[j++] = MacBuffer1[i] - 32;
		else 
		if ((MacBuffer1[i] >= 'A' && MacBuffer1[i] <= 'Z')	||
			(MacBuffer1[i] >= '0' && MacBuffer1[i] <= '9')	||
			(MacBuffer1[i] == ',')							||  
			(MacBuffer1[i] == '.'))
				MacBuffer2[j++] = MacBuffer1[i];
		else 
		if (MacBuffer1[i] == ' ')
		{
			MacBuffer2[j] = MacBuffer1[i];
			if (MacBuffer1[i+1] != ' ')
				j++;
		}
	}

	sprintf(DeData, "%s ", MacBuffer2);
	Length = j + 1 ;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_FormatMacData():InData[%s], Length[%d], OutData[%s]", SourceData, Length, DeData);
	return TRUE;
}

/*****************************************************************************
    # 8�ڸ��� Ramdom�� ���ڿ� ���� (1~9, A~F)
*****************************************************************************/
CString	CTranCmn::fstrAPL_MakeRandomMac()
{
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[MAC��������� �������� ����]    fstrAPL_MakeRandomMac()");

    CString JArray;
    int HexDigit = 0;

    JArray.Empty();

    for (int i=0 ;  i < 8 ; i++) {
        HexDigit = rand()%16;
        if(HexDigit == 0) JArray       += '0';
        else if(HexDigit == 1) JArray  += '1';
        else if(HexDigit == 2) JArray  += '2';
        else if(HexDigit == 3) JArray  += '3';
        else if(HexDigit == 4) JArray  += '4';
        else if(HexDigit == 5) JArray  += '5';
        else if(HexDigit == 6) JArray  += '6';
        else if(HexDigit == 7) JArray  += '7';
        else if(HexDigit == 8) JArray  += '8';
        else if(HexDigit == 9) JArray  += '9';
        else if(HexDigit == 10) JArray += 'A';
        else if(HexDigit == 11) JArray += 'B';
        else if(HexDigit == 12) JArray += 'C';
        else if(HexDigit == 13) JArray += 'D';
        else if(HexDigit == 14) JArray += 'E';
        else if(HexDigit == 15) JArray += 'F';
    }

    return JArray;
}

// ����Ʈ�� ���� �߾� �����ϴ� �����͸� ������.
CString CTranCmn::fnAPLLib_GetCenterPrtData(CString PrtData, int MaxColumn)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PrtData=[%s],MaxColumn=[%d]",PrtData, MaxColumn);
	int		nIndex = 0;
	CString strSpace("");
	CString strRet("");

	nIndex = PrtData.GetLength();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PrtData.GetLength()=[%d]",PrtData.GetLength());
	nIndex = nIndex/2;
	nIndex = (MaxColumn/2) - nIndex;
	if (nIndex < 0) nIndex = 0;
	else
	if (nIndex > MaxColumn)
		nIndex = MaxColumn;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "nIndex=[%d]",nIndex);

	for (int i=1; i<=nIndex; i++)
		strSpace += " ";
	strRet = strSpace + PrtData;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PrtData=[%s]",PrtData);
	return strRet;
}



CString	CTranCmn::fnAPLLib_MakePackCardTrack(LPSTR szCardData)						// YHJ 20080306A
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_MakePackCardTrack");

// ��-s
// 			BYTE	byCardData[_MAX_CARDDATA_LEN];
// 			int		nLen = __min(strlen(szCardData), _MAX_CARDDATA_LEN);
// 			int		i;
 			CString strReturn = "";
// 			CString strTemp = "";
// 
// 			memset(byCardData, 0, sizeof(byCardData));
// 			memcpy(byCardData, (LPVOID)szCardData, nLen);
// 			for (i = 0; i < nLen; i++)
// 			{
// 				byCardData[i] = byCardData[i] ^ _TEMP_PACK_CHAR;
// 				strTemp.Format("%02X", byCardData[i]);
// 				strReturn += strTemp;
// 			}
// 
// 		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_MakePackCardTrack return");
// ��-e

	return strReturn;
}

CString CTranCmn::fnAPLLib_MakeUnPackCardTrack(LPSTR szPackedCardData)				// YHJ 20080306A
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_MakeUnPackCardTrack");

// ��-s
// 			BYTE	byCardData[_MAX_CARDDATA_LEN];
// 			int		nLen = __min(strlen(szPackedCardData), _MAX_CARDDATA_LEN);
// 			int		i;
 			CString strReturn = "";
// 			CString strTemp = "";
// 
// 			memset(byCardData, 0, sizeof(byCardData));
// 			Asc2Byten(szPackedCardData, byCardData, nLen);
// 
// 			for (i = 0; i < nLen/2; i++)
// 			{
// 				byCardData[i] = byCardData[i] ^ _TEMP_PACK_CHAR;
// 				strTemp.Format("%c", byCardData[i]);
// 				strReturn += strTemp;
// 			}
// 
// 		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_MakeUnPackCardTrack return");
// ��-e
	return strReturn;
}


// [#156] US Justin 2012.04.40 Add EMV Transaction

//////////////////////////////////////////////////////////////////////////
// EMV SUPPORT
//////////////////////////////////////////////////////////////////////////

CString CTranCmn::fnAPLLib_EmvMakeCardPanProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPLLib_EmvMakeCardPanProc()");

	CString strReturn("");
	CString strIDCTrack2("");
	CString strIDCTbank("");
	
	int index = 0;

	memset(&CardData, 0, sizeof(CardData));					// Card Data
	memset(CardPan, 0, sizeof(CardPan));					// Card Pan

	strIDCTrack2 = 	m_pDevCmn->fnEMVLib_GetVal(EMVTid_Track2EquData);
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvMakeCardPanProc, EMVTid_Track2EquData-91(%s)", strIDCTrack2);
	
	if (strIDCTrack2.GetLength())
	{
		index = strIDCTrack2.FindOneOf("D");
		
		/* Replace 'D' to '=' in the Track 2 data from the IC */
		/* Upper & Lower Case Check!! "D" or "d"*/
		if(index != -1)
			strIDCTrack2.Replace("D", "=");
		
		index = strIDCTrack2.FindOneOf("F");
		if(index != -1)
			strIDCTrack2.Replace("F", "");
		
		CardData.TrInfo |= ISO2_TRACK;
		m_pDevCmn->EMVDOLListInfo.MSTrack2Len = strIDCTrack2.GetLength();
		memcpy(m_pDevCmn->EMVDOLListInfo.MSTrack2Image, strIDCTrack2.GetBuffer(0), m_pDevCmn->EMVDOLListInfo.MSTrack2Len);
		MakePack((void*)(LPCTSTR)strIDCTrack2, (void*)m_pDevCmn->EMVDOLListInfo.MSTrack2Image, strIDCTrack2.GetLength());

		// MS Track 2 Image ����
		char szISO2[100] = {0x00,};
		CardData.ISO2size = m_pDevCmn->fnEMV_GetISO2(szISO2);
		memcpy( CardData.ISO2Buff, szISO2, CardData.ISO2size );

HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "�쾾 CardData.ISO2Buff", CardData.ISO2Buff,  sizeof(CardData.ISO2Buff));
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "�쾾 ���� (%d)", CardData.ISO2size);

		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvMakeCardPanProc strIDCTrack2(%s)", strIDCTrack2);
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvMakeCardPanProc CardData.ISO2Buff(%s)", CardData.ISO2Buff);

		/* Abstract Account Number, Bank ID, PAN */
		if (CardData.TrInfo & ISO2_TRACK)
		{
			if ((CardData.ISO2size < 2)								||
				(CardData.ISO2size > sizeof(CardData.ISO2Buff))		||
				(FindChar(CardData.ISO2Buff, CardData.ISO2size, NULL) != CardData.ISO2size))
				;
			else
			{
				CStringArray	strTr2DataArray;
				CardTran |= MENU_MCU2;
				SplitString(strIDCTrack2, "=", strTr2DataArray);
				if (strTr2DataArray[0].GetLength() >= 4)
					strIDCTbank = strTr2DataArray[0].Right(4);

				/*
				index = strIDCTrack2.FindOneOf("=");
				if (index >= 13) 
					strIDCTbank = strIDCTrack2.Mid(index-4,4); 
				else
				if ((index < 13) && (index >=0))
					strIDCTbank = strIDCTrack2.Mid(12,4);
				*/
			}
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvMakeCardPanProc strIDCTbank(%s)", strIDCTbank);
		}

		strIDCTbank.TrimLeft();
		strIDCTbank.TrimRight();
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvMakeCardPanProc After Trim strIDCTbank(%s)", strIDCTbank);

// ��-s		RegSetStr(_REGKEY_APP_ERRTRANS, _REGNAME_APP_TRANSBANKCODE,		strIDCTbank);		// ��-e
		
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvMakeCardPanProc CardTran(%d)", CardTran);
		if ( (CardTran & MENU_MCU2) == MENU_MCU2 )							// CARD(2TR)
		{
			// Make Card Account No
			for (int i = 0; i < 19; i++)
			{
				if (IsNum(&CardData.ISO2Buff[i], 1))
					CardAccountNo[i] = CardData.ISO2Buff[i];
				else
					break;
			}

			// Make Card Pan
			int Result = FindNotNum(CardData.ISO2Buff, 20);			

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvMakeCardPanProc Result(%d)", Result);

			if (Result <= 12)
			{
				memset(&CardPan[0], '0', 1);
				memcpy(&CardPan[1], CardData.ISO2Buff, 11);
				m_pDevCmn->EMVDOLListInfo.PANLen = 0x0b;
			}
			else if (Result <= 18)
			{
				memcpy(&CardPan[0], &CardData.ISO2Buff[Result - 13], 12);
				m_pDevCmn->EMVDOLListInfo.PANLen = 0x0c;
			}
			else 
			{
				memcpy(&CardPan[0], &CardData.ISO2Buff[Result - 13], 12);
				m_pDevCmn->EMVDOLListInfo.PANLen = 0x0c;
			}

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�쾾 CardPan", CardPan,  sizeof(CardPan));

//			memcpy(m_pDevCmn->EMVDOLListInfo.PAN
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvMakeCardPanProc() CardPan(%s)", CardPan);
		}
	}

	strReturn.Format("%s", CardPan);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvMakeCardPanProc() return(%s)", strReturn);
	return strReturn;
}

int CTranCmn::fnAPLLib_EmvTranDataMake(int nEmvTrType)
{
	// EMV �ŷ��� �ƴϸ� return ó���Ѵ�.
	if (EMVReadFlg == FALSE)
	{
		return TRUE;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake() nEmvTrType(TranCode)[%d]", nEmvTrType);
	int			nReturn = 0;
	int			nResult = 0;
	int			ScrNum	= 0;
	CString		strTemp("");

	/////////////////////////////////////////////////////////////////////////////
	//
	// EMV_InitAppProcess();
	//
	/////////////////////////////////////////////////////////////////////////////
	CString strProcCode = _T("00");
	CString strAmount	= _T("000000000000");
	CString strDateTime = _T("000000000000");
	
	EMVHostResponse		= _T("");

		// [#2188]
		// Proc Code�� Setting�Ѵ�.
//#define 	TRTYPE_GOODSERVICE 		0x00			// Purchase of goods or service
//#define 	TRTYPE_CASH				0x01			// Cash Disbursement
//#define 	TRTYPE_CASHBACK 		0x09			// Purchase with Cash Back
//#define 	TRTYPE_INQUIRY			0x30			// Inquiry 
//#define 	TRTYPE_PAYMENT			0x19			// Payment    
//#define 	TRTYPE_TRANSFER			0x40			// Transfer 

		
	// �� �κ��� ������ �°� ������ Ȯ���� �ʿ��ϴ�.
	// Tran Code
	if (nEmvTrType == TC_SVCINQ)				// ���ݼ�����ȸ
		strProcCode.Format("30");
	else 
	if (nEmvTrType == TC_SVCTRAN1)             // ����������ü(������ü ��������/�ٸ�����)
		strProcCode.Format("40");
	else 
	if (nEmvTrType == TC_SVCWITH)				// ���ݼ���(���)
		strProcCode.Format("01");
	else
		return T_OK;

	nResult = m_pDevCmn->fnEMVLib_TlvStoreVal(EMVTid_TrType, strProcCode);
	if(nResult != EMV_RSLT_OK)	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvTranDataMake:m_pDevCmn->fnEMV_TlvStoreVal():nResult=Error(%d)", nResult);
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");			
	}

	// �ݾ������� Setting�Ѵ�.
	// Transaction Amount

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "������ fnAPLLib_EmvTranDataMake: �ݾ����� Ȯ�� �Ұ�(%s)", Accept.Money);
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "�ݾ�", Accept.Money,  sizeof(Accept.Money));

	int	nAmount = Asc2Int(Asc2String(Accept.Money, 8, 8));
//	int	nAmount = Asc2Int(Accept.Money, 8);

	strAmount.Format("%012d", nAmount);
	nResult = m_pDevCmn->fnEMVLib_TlvStoreVal(EMVTid_AmountAuthN, strAmount);
	if(nResult != EMV_RSLT_OK)	
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvTranDataMake:m_pDevCmn->fnEMV_TlvStoreVal():nResult=Error(%d)", nResult);
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");			
	}

	/////////////////////////////////////////////////////////////////////////////
	//
	// Store 5F57; Source Account Type
	//
	/////////////////////////////////////////////////////////////////////////////
	if (SourceAccount == S_CHECKING)			strTemp.Format("20");
	else if (SourceAccount == S_SAVINGS)		strTemp.Format("10");
	else if (SourceAccount == S_CREDITCARD)		strTemp.Format("30");
	else										strTemp.Format("00");
	nResult = m_pDevCmn->fnEMVLib_TlvStoreVal(EMVTid_AccountType, strTemp);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_TlvStoreVal(EMVTid_AccountType, %s):nResult(%d)", strTemp, nResult);
	if(nResult != EMV_RSLT_OK)	
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvTranDataMake:m_pDevCmn->fnEMV_TlvStoreVal():nResult=Error(%d)", nResult);
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");			
	}

	/////////////////////////////////////////////////////////////////////////////
	//
	// EMV_ReadAppData();
	//
	/////////////////////////////////////////////////////////////////////////////
	// fnAPPLib_EMVReadProc() �Լ����� �����ϰ� �ִ�.
	if (!EmvReadAppDataFlag)
	{
		nResult = m_pDevCmn->fnEMVLib_ReadAppData();
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_ReadAppData():nResult(%d)", nResult);
		if((nResult == EMV_ERR_NOT_SUPPORT)	||(nResult == EMV_ERR_CARD_SW) )
		{
			CardTran &= ~MENU_IC;
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_ReadAppData():nResult=EMV_ERR_FALLBACK!!");
			return fnAPLLib_EmvFallBackProc(TRUE);
		}
		else if(nResult != EMV_RSLT_OK)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_ReadAppData():nResult=Error(%d)", nResult);
			return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//
	// GET APP LABEL
	//
	/////////////////////////////////////////////////////////////////////////////
	// [#2188] ȭ�鿡�� ǥ���ϰ� ��ǥ���� �μ����� �ʴ´�.
// 	m_sAIDDescription = m_pDevCmn->fnEMV_GetVal(EMVTid_AppLabel);
// 	if( m_sAIDDescription.GetLength()>0)
// 	{
// 		BYTE* sOut = new BYTE[m_sAIDDescription.GetLength() +1];
// 		memset(sOut, NULL, m_sAIDDescription.GetLength()+1);
// 		MakePack( (void*)(LPCTSTR)m_sAIDDescription, (void*)sOut, m_sAIDDescription.GetLength(), '^', 0);
// 		m_sAIDDescription.Format("%s", sOut);
// 		delete [] sOut;
// 	}
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_GetVal(EMVTid_AppLabel) = [%s]", m_sAIDDescription);
	// end of [#2188]

	/////////////////////////////////////////////////////////////////////////////
	//
	// fnEMV_OffDataAuth();
	//
	/////////////////////////////////////////////////////////////////////////////
	nResult = m_pDevCmn->fnEMVLib_OffDataAuth();
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_OffDataAuth():nResult(%d)", nResult);
	if((nResult == EMV_ERR_NOT_SUPPORT)	|| (nResult == EMV_ERR_CARD_SW))
	{
		CardTran &= ~MENU_IC;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_OffDataAuth():nResult=EMV_ERR_FALLBACK!!");
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");
	}
	else if(nResult != EMV_RSLT_OK)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvTranDataMake:m_pDevCmn->fnEMV_OffDataAuth():nResult=Error(%d)", nResult);
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");
	}

	/////////////////////////////////////////////////////////////////////////////
	//
	// fnEMV_ProcRestriction();
	//
	/////////////////////////////////////////////////////////////////////////////
	nResult = m_pDevCmn->fnEMVLib_ProcRestriction();
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_ProcRestriction():nResult(%d)", nResult);
	if((nResult == EMV_ERR_NOT_SUPPORT)||(nResult == EMV_ERR_CARD_SW) )
	{
		CardTran &= ~MENU_IC;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_ProcRestriction():nResult=EMV_ERR_FALLBACK!!");
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");
	}
	else if(nResult != EMV_RSLT_OK)	
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvTranDataMake:m_pDevCmn->fnEMV_ProcRestriction():nResult=Error(%d)", nResult);
		fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");
	}

	/////////////////////////////////////////////////////////////////////////////
	//
	// Build and Get PinBlock
	//
	/////////////////////////////////////////////////////////////////////////////
// 	strTemp = fnAPLLib_EmvMakeCardPanProc();
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvMakeCardPanProc = [%s]", strTemp);
// 
// 
// //�λ����� ��� 	fnAPP_EMVReadBCCheck();										// ī��üũ		2006.11.02
// 
// 	if(CardService == BCCARD)
// 		EMVReadFlg = TRUE;
// 	else
// 	{
// 		EMVReadFlg = FALSE;
// 		return fnAPLLib_EmvFallBackProc();									// Emv -> MS Fallback Processing
// //		return T_ERROR;											// FallBack����	
// 	}

	// [#2188] PAN DATA�� ��� ONLINE���� �ش� DATA�� �۽��ؾ��� (�����׸�)
	// �� PIN�� ��ָ� ���� ���� ������ PINBLOCK�� �����ؼ� �۽��ϵ��� ����

	/////////////////////////////////////////////////////////////////////////////
	//
	// fnEMV_CardholderVerify1
	//
	/////////////////////////////////////////////////////////////////////////////
	nResult = m_pDevCmn->fnEMVLib_CardholderVerify1(Accept.PassWord);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->EMV_CardholderVerify1(PassWord):nResult(%d)", nResult);
	if((nResult == EMV_ERR_NOT_SUPPORT)||(nResult == EMV_ERR_CARD_SW))
	{
		CardTran &= ~MENU_IC;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->EMV_CardholderVerify1(PassWord):nResult=EMV_ERR_FALLBACK!!");
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");
	}
	else if(nResult!= EMV_RSLT_OK)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->EMV_CardholderVerify1(PassWord):nResult=Error(%d)", nResult);
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");
	}

	/////////////////////////////////////////////////////////////////////////////
	//
	// fnEMV_TerminalRiskMgmt
	//
	/////////////////////////////////////////////////////////////////////////////
	nResult = m_pDevCmn->fnEMVLib_TerminalRiskMgmt();
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_TerminalRiskMgmt():nResult(%d)", nResult);
	if((nResult == EMV_ERR_NOT_SUPPORT)||(nResult == EMV_ERR_CARD_SW))
	{
		CardTran &= ~MENU_IC;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_TerminalRiskMgmt():nResult = EMV_ERR_FALLBACK!!");
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");
	}
	else if(nResult!= EMV_RSLT_OK)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_TerminalRiskMgmt(PassWord):nResult=Error(%d)", nResult);
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");
	}

	/////////////////////////////////////////////////////////////////////////////
	//
	// fnEMV_ActionAnalysis
	//
	/////////////////////////////////////////////////////////////////////////////
	BSTR	bstrReturn = NULL;			
	nResult = m_pDevCmn->fnEMVLib_ActionAnalysis(&bstrReturn);
	CString strResult = bstrReturn;
	::SysFreeString(bstrReturn);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_ActionAnalysis():m_pDevCmn->fnEMV_ActionAnalysis():nResult(%d), strResult(%s)", nResult, strResult);

	if((nResult == EMV_ERR_NOT_SUPPORT)||(nResult == EMV_ERR_CARD_SW))
	{
		CardTran &= ~MENU_IC;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_ActionAnalysis(.):nResult = EMV_ERR_FALLBACK!!");
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");
	}
	else if(nResult!= EMV_RSLT_OK)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_ActionAnalysis(.):nResult=Error(%d)", nResult);
		return fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�.");
	}

	// Check CARD ONLINE Parameter
	if(strResult != "  ")
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_ActionAnalysis(.) => Card does not support Online => EMV_CompleteProcess()");
		nReturn = m_pDevCmn->fnEMVLib_CompleteProcess();
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_CompleteProcess() : return=[%d]", nReturn);
		m_pDevCmn->Declined = TRUE;	// [#2188-7]

		return fnAPP_CancelProc(T_ERROR);
	}

	m_pDevCmn->GenerateAC = 1;			// First Generate AC....
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake() First Generate AC OK return(%d)", nReturn);
	return nReturn;
}

int CTranCmn::fnAPLLib_EmvOnlineApproval(int nOnlineMode, int nHostResponse)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvOnlineApproval(nOnlineMode(%d), nHostResponse(%d), EMV Tag = [%s]", nOnlineMode, nHostResponse, EMVHostResponse);

	CString sTag8A, sTag91, sTag71, sTag72;
	sTag8A = sTag91 = sTag71 = sTag72 = _T("");	
	if( EMVHostResponse.GetLength() >= 2 )
	{
		CString strEMVTag = EMVHostResponse;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvOnlineApproval(EMV strEMVTag = [%s]", strEMVTag);

		BOOL bDataLeft = TRUE;
		do
		{
			bDataLeft = FALSE;
			CString sTagName = strEMVTag.Left(2);				
			strEMVTag = strEMVTag.Mid(2);
			CString	sTagName2 = strEMVTag.Left(2);
			if(sTagName.CompareNoCase("9F") == 0 && sTagName2.CompareNoCase("36") == 0)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "sTagName (9F36)");
				strEMVTag = strEMVTag.Mid(2);
			}
			// ��������������������������������������������������

			if(strEMVTag.GetLength()>2)
			{
				CString sTagLength = strEMVTag.Left(2);				
				strEMVTag = strEMVTag.Mid(2);
				BYTE nValueLength;
				Asc2Byte( (void*)(LPCTSTR)sTagLength, &nValueLength);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "sTagName=[%s], Length=[%d], Remain=[%s]", sTagName, nValueLength, strEMVTag);
				if(strEMVTag.GetLength() >= (nValueLength*2) )
				{
					CString sTagValue = strEMVTag.Left(nValueLength*2);	
					if(sTagName == "8A")			sTag8A = sTagValue;
					else if(sTagName == "91")		sTag91 = sTagValue;
					else if(sTagName == "71")		sTag71 += sTagName + sTagLength + sTagValue; // 2013.06.18 SHPARK
					else if(sTagName == "72")		sTag72 += sTagName + sTagLength + sTagValue; // 2013.06.18 SHPARK

					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Parsing EMV Host Tag : sTag8A = [%s], sTag91 = [%s], sTag71 = [%s], sTag72 = [%s]", sTag8A, sTag91, sTag71, sTag72);
					if(strEMVTag.GetLength()>(nValueLength*2) )
					{
						strEMVTag = strEMVTag.Mid(nValueLength*2);
						if( strEMVTag.GetLength() > 2 )
							bDataLeft = TRUE;
					}
				}
			}
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Remain EMV Tag = [%s], bDataLeft=[%d]", strEMVTag, bDataLeft);
		} while(bDataLeft == TRUE);
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV Host Tag : sTag8A = [%s], sTag91 = [%s], sTag71 = [%s], sTag72 = [%s]", sTag8A, sTag91, sTag71, sTag72);

	RegSetStr(_REGKEY_ATM_APP, "EMV_ARPC", sTag91);		// [#159] US Justin 2012.07.12 FOR EMV Journal...

	CString sTag7172 = _T("");
	//CString sTemp;
	if(sTag71.GetLength()>0)
	{
		// sTemp.Format("71%02X%s", (int)(sTag71.GetLength()/2+0.0001), sTag71); // 2013.06.18 SHPARK
		sTag7172 += sTag71;
	}
	if(sTag72.GetLength()>0)
	{
		// sTemp.Format("72%02X%s", (int)(sTag72.GetLength()/2+0.0001), sTag72); // 2013.06.18 SHPARK
		sTag7172 += sTag72;
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV Host Tag : sTag7172 = [%s]", sTag7172);

	int nReturn = m_pDevCmn->fnEMVLib_OnlineApproval(nOnlineMode, nHostResponse, sTag8A, sTag91.GetLength(), sTag91, sTag7172.GetLength(), sTag7172 );
	m_pDevCmn->GenerateAC = 2;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->fnEMV_OnlineApproval(%d, %d, %s, %s, %s) : return=[%d]", nOnlineMode, nHostResponse, sTag8A, sTag91, sTag7172, nReturn);
	if(nReturn != EMV_RSLT_OK)
	{
		// m_pDevCmn->m_bDeclined = TRUE; // 2013.06.17 SHPARK - ERROR ������ �� ó�� // 2013.06.20 ���� 
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_OnlineApproval(...) FAIL");
//		return 7;
		return -1;		// [#2188-6]
	}

// EMV �� ����� Compelte���� �����ؾ� ī�������� �����ε� ������ üũ���� �ʵ��� �Ǿ� �־� ���� ó�� ��.
// AP BD������ Complete������ 31 00 00 01 => ���������� NGó���� ������ �´�.
	nReturn = m_pDevCmn->fnEMVLib_CompleteProcess();
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_CompleteProcess() : return=[%d]", nReturn);

	return 0;
// 20131008-s
// 	if(nReturn != 7)	// 7:Approved, 8:Declined
// 	{
// 		m_pDevCmn->m_bDeclined = TRUE; // 2013.06.17 SHPARK - ERROR ������ �� ó��
// 		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_CompleteProcess() : FAIL (DENIED) => REVERSAL");
// 		return 7;
// 	}
// 
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvOnlineApproval() return : SUCCESS");
// 	return 0;
// 20131008-e
}

// Emv FallBack Proc
int CTranCmn::fnAPLLib_EmvFallBackProc(BOOL bGeneratePinBlock)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib_EmvFallBackProc() bGeneratePinBlock=%d", bGeneratePinBlock);
// --  �� Comment ���� ���� --
// ������ �� �Լ����� IC PowerOff �� ȭ��ó�� PIN PAD�� Pin Read/Block�� ó����
// ������ ���ʿ� �ϱ� ������ �ϱ� ���� ��� ���� ó�� �Ͽ���
// �� �� �Լ����� ���� ������ �ʿ��� ó���� ���� ��� ����Ͽ� Flag����� �Լ��� ���� ��.
//	FallBackEMVMode = TRUE;	
	m_pDevCmn->EmvCardService = 1;
	return T_ERROR;
}
// End of [#156]