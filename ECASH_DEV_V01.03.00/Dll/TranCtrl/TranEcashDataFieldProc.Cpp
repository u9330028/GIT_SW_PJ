
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\MacroDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"
#include "..\..\H\Dll\BIPConfig.h"
#include < nb30.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CTranCmn::fnAPP_SetAddArrDataField(DataField *pArrDataField, int nTxRxOption)
{

	char	szTmp[1024];
	CString strUpPackFData("");
	memset(szTmp, 0x00, sizeof(szTmp));
	memcpy(szTmp, pArrDataField->byFData, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	
	if(nTxRxOption == TX_SEND_F)
	{	


		if(memcmp(pArrDataField->byFCode, x043X, sizeof(pArrDataField->byFCode)) == 0 || memcmp(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode)) == 0 || memcmp(pArrDataField->byFCode, x044X, sizeof(pArrDataField->byFCode)) == 0 || memcmp(pArrDataField->byFCode, x054X, sizeof(pArrDataField->byFCode)) == 0)
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "TX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, MakeUnPack(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen))));
			MsgDump(TRACE_DUMP_MODE, "TRX", __FILE__, __LINE__, "TX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, MakeUnPack(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen))));
		}
		else
		if(FindChar(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen)), NULL) != Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen)))
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "TX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, MakeUnPack(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen))));
			MsgDump(TRACE_DUMP_MODE, "TRX", __FILE__, __LINE__, "TX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, MakeUnPack(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen))));
//			MsgDump(TRACE_CODE_MODE, "TXRX", __FILE__, __LINE__, "fnAPP_SetTXAddArrDataField - %4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, MakeUnPack(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen))));
//			HexDump(TRACE_DUMP_MODE, "TXRX", __FILE__, __LINE__, "fnAPP_TXDataField byFData", pArrDataField->byFData, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "TX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, szTmp);

			if(memcmp(pArrDataField->byFCode, x081X, sizeof(pArrDataField->byFCode)) == 0)
				MsgDump(TRACE_DUMP_MODE, "TRX", __FILE__, __LINE__, "[TX Start ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆]");

			MsgDump(TRACE_DUMP_MODE, "TRX", __FILE__, __LINE__, "TX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, szTmp);
//			MsgDump(TRACE_CODE_MODE, "TXRX", __FILE__, __LINE__, "fnAPP_TXDataField - %4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, szTmp);
		}

		HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TXDataField byFData", pArrDataField->byFData, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		strUpPackFData = MakeUnPack(pArrDataField->byFData, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		s_saTXDataField.SetAtGrow(Asc2Int(pArrDataField->byFCode, sizeof(pArrDataField->byFCode)), strUpPackFData);
		
		m_arrTXDataStructure.Add(pArrDataField);


	}
	else
	if(nTxRxOption == RX_RECV_F)
	{   

		if(memcmp(pArrDataField->byFCode, x043X, sizeof(pArrDataField->byFCode)) == 0 || memcmp(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode)) == 0 || memcmp(pArrDataField->byFCode, x044X, sizeof(pArrDataField->byFCode)) == 0 ||  memcmp(pArrDataField->byFCode, x054X, sizeof(pArrDataField->byFCode)) == 0)
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "RX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, MakeUnPack(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen))));
			MsgDump(TRACE_DUMP_MODE, "TRX", __FILE__, __LINE__, "RX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, MakeUnPack(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen))));
		}
		else
		if(FindChar(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen)), NULL) != Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen)))
		{
			MsgDump(TRACE_DUMP_MODE, "TRX", __FILE__, __LINE__, "RX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, MakeUnPack(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen))));
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "RX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, MakeUnPack(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen))));
//			MsgDump(TRACE_CODE_MODE, "TXRX", __FILE__, __LINE__, "fnAPP_SetRXAddArrDataField - %4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, MakeUnPack(szTmp, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen))));
//			HexDump(TRACE_DUMP_MODE, "TXRX", __FILE__, __LINE__, "fnAPP_RXDataField byFData", pArrDataField->byFData, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
		{
			if(memcmp(pArrDataField->byFCode, x081X, sizeof(pArrDataField->byFCode)) == 0)
				MsgDump(TRACE_DUMP_MODE, "TRX", __FILE__, __LINE__, "[RX ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★]");
	
			MsgDump(TRACE_DUMP_MODE, "TRX", __FILE__, __LINE__, "RX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, szTmp);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "RX-%4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, szTmp);
//			MsgDump(TRACE_CODE_MODE, "TXRX", __FILE__, __LINE__, "fnAPP_RXDataField - %4.4s %3.3s : [%s]" , pArrDataField->byFCode, pArrDataField->byFLen, szTmp);
		}

		HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_RXDataField byFData", pArrDataField->byFData, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		strUpPackFData = MakeUnPack(pArrDataField->byFData, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		s_saRXDataField.SetAtGrow(Asc2Int(pArrDataField->byFCode, sizeof(pArrDataField->byFCode)), strUpPackFData);
		m_arrRXDataStructure.Add(pArrDataField);		
	}

	return TRUE;
}

int CTranCmn::fnAPP_Init_031X(int nOption)
{
	memset(&m_F031X, 0, sizeof(m_F031X));

	for (int i = 0; i < itemof(m_F031X.F031XLIST); i++)
		StrAllSpace(m_F031X.F031XLIST[i]);

	memset(m_FRX101X, ' ', sizeof(m_FRX101X));
	return TRUE;
}
int CTranCmn::fnAPP_ClearArrDataField(int nOption)
{

	int i;
	
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_ClearArrDataField(%s)", (nOption == TX_SEND_F) ? "Tx_Field" : (nOption == RX_RECV_F) ? "Rx_Field" : "TxRx_Field");

	if(nOption == TX_SEND_F)
	{
		nSendDataIndex = 0;							
		memset(m_pTranCmn->pArrDataFieldArr, 0x00, sizeof(m_pTranCmn->pArrDataFieldArr));
		s_saTXDataField.RemoveAll();
		for (i = m_arrTXDataStructure.GetSize()-1; i>=0; i--)
		{
			delete m_arrTXDataStructure[i];
			m_arrTXDataStructure.RemoveAt(i);
		}
	}
	else
	if(nOption == RX_RECV_F)
	{
		s_saRXDataField.RemoveAll();
		for (i = m_arrRXDataStructure.GetSize()-1; i>=0; i--)
		{
			delete m_arrRXDataStructure[i];
			m_arrRXDataStructure.RemoveAt(i);
		}

	}
	else
	{
		s_saTXDataField.RemoveAll();
		s_saRXDataField.RemoveAll();
		for (i = m_arrTXDataStructure.GetSize()-1; i>=0; i--)
		{
			delete m_arrTXDataStructure[i];
			m_arrTXDataStructure.RemoveAt(i);
		}

		for (i = m_arrRXDataStructure.GetSize()-1; i>=0; i--)
		{
			delete m_arrRXDataStructure[i];
			m_arrRXDataStructure.RemoveAt(i);
		}
	}
	
MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_ClearArrDataField() return");

	 return TRUE;
}

int CTranCmn::fnAPP_TXGetFindArryDataField(LPCTSTR szbyFCode, int nFLen, CString &strReturn, int nPackOption)
{
	int bIsFind = FALSE;
	char	szTemp[256];
	int	nLen;
    CString strUpPackFData("");
	DataField *pArrDataField;
	for(int i = m_arrTXDataStructure.GetSize() - 1 ; i >= 0; i--)
	{
		pArrDataField = m_arrTXDataStructure.GetAt(i);
		if(memcmp(pArrDataField->byFCode, szbyFCode, nFLen) == 0)
		{
			
			bIsFind = TRUE;
			if(nPackOption == PACK_F)
			{
				nLen = Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen));
				
				if(FindChar(pArrDataField->byFData, nLen, NULL) != nLen)
				{
					bIsFind = FALSE;
					break;
				}

				sprintf(szTemp, "%%%d.%ds", nLen, nLen);
				strReturn.Format(szTemp, pArrDataField->byFData);
			}
			else
			{
				nLen = Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen));
				strUpPackFData = MakeUnPack(pArrDataField->byFData, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				strReturn = strUpPackFData;
			}

			break;
		}
	}
	return bIsFind;
}
int CTranCmn::fnAPP_RXGetFindArryDataField(LPCTSTR szbyFCode, int nFLen, CString &strReturn, int nPackOption)
{
	int bIsFind = FALSE;
	char	szTemp[256];
	int	nLen;
	CString strUpPackFData("");
	DataField *pArrDataField;
	for(int i = m_arrRXDataStructure.GetSize() - 1 ; i >= 0; i--)
	{
		pArrDataField = m_arrRXDataStructure.GetAt(i);
		if(memcmp(pArrDataField->byFCode, szbyFCode, nFLen) == 0)
		{
			bIsFind = TRUE;
			if(nPackOption == PACK_F)
			{
				nLen = Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen));
				
				if(FindChar(pArrDataField->byFData, nLen, NULL) != nLen)
				{
					bIsFind = FALSE;
					break;
				}

				sprintf(szTemp, "%%%d.%ds", nLen, nLen);
				strReturn.Format(szTemp, pArrDataField->byFData);
			}
			else
			{
				nLen = Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen));
				strUpPackFData = MakeUnPack(pArrDataField->byFData, Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				strReturn = strUpPackFData;
			}
			break;
		}
	}
	return bIsFind;
}

int CTranCmn::fnAPP_TxECash_SetDataField_SendProc(int nSec)
{
	BYTE m_TXBuff[1024];
	BYTE byTmpBuff[1024];
	int  nLen =0;
	int  nBcc = 2;
	int  nTXlen = 4;
	CString strReturn("");
	
	SendLength =0;
	memset(m_TXBuff, 0x00, sizeof(m_TXBuff));
	memset(&m_TXECASH, 0x00, sizeof(m_TXECASH));
	DataField *pArrDataField;
	
	for(int i = 0; i < m_arrTXDataStructure.GetSize() ; i++)
	{
		pArrDataField = m_arrTXDataStructure.GetAt(i);
		memcpy(&m_TXECASH.byTData[nLen], pArrDataField->byFCode, sizeof(pArrDataField->byFCode));
		nLen += sizeof(pArrDataField->byFCode); 
		memcpy(&m_TXECASH.byTData[nLen], pArrDataField->byFLen,  sizeof(pArrDataField->byFLen));
		nLen += sizeof(pArrDataField->byFLen); 
		memcpy(&m_TXECASH.byTData[nLen], pArrDataField->byFData, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		nLen += Asc2Int(pArrDataField->byFLen, sizeof(pArrDataField->byFLen));
	}

	Int2Asc(nLen+nBcc, m_TXECASH.byTLen, sizeof(m_TXECASH.byTLen));
	byTmpBuff[0] = fnAPP_SetBCC(m_TXECASH.byTData, nLen);
	MakeUnPack(byTmpBuff, m_TXECASH.byBcc, 1);
	
	memcpy(m_TXBuff, m_TXECASH.byTLen, sizeof(m_TXECASH.byTLen));
	memcpy(&m_TXBuff[sizeof(m_TXECASH.byTLen)], m_TXECASH.byTData, nLen);
	memcpy(&m_TXBuff[sizeof(m_TXECASH.byTLen)+nLen], m_TXECASH.byBcc, sizeof(m_TXECASH.byBcc));

	SendLength = sizeof(m_TXECASH.byTLen) + nLen + sizeof(m_TXECASH.byBcc);

	memcpy(SendBuffer, m_TXBuff, SendLength);

	if(!nSec) return T_OK;

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "byTLen - %4.4s", m_TXECASH.byTLen);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "ByBcc  - %2.2s", m_TXECASH.byBcc);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "TxSend - TXFD",  m_TXBuff, SendLength);

//	MsgDump(TRACE_CODE_MODE, "TXRX", __FILE__, __LINE__, "byTLen - %4.4s", m_TXECASH.byTLen);
//	MsgDump(TRACE_CODE_MODE, "TXRX", __FILE__, __LINE__, "ByBcc  - %2.2s", m_TXECASH.byBcc);
//	HexDump(TRACE_CODE_MODE, "TXRX", __FILE__, __LINE__, "TxSend - TXFD",  m_TXBuff, SendLength);

	return T_OK;
}

int CTranCmn::fnAPP_RxECash_GetDataField_RecvProc(int nSec)
{
	DataField *pGetArrDataField;
	BYTE byTmpBuff[1024] = {0x00, };
	int nAdd = 0;
	int nTLen = Asc2Int(m_RXECASH.byTLen, sizeof(m_RXECASH.byTLen)) - sizeof(m_RXECASH.byBcc);
	while(nTLen > nAdd)
	{
		DataField *pArrDataField = new DataField;
		pGetArrDataField = (DataField *)&m_RXECASH.byTData[nAdd];
		memcpy(pArrDataField->byFCode, pGetArrDataField->byFCode, sizeof(pGetArrDataField->byFCode));
		nAdd += sizeof(pGetArrDataField->byFCode);
		memcpy(pArrDataField->byFLen,  pGetArrDataField->byFLen, sizeof(pGetArrDataField->byFLen));
		nAdd += sizeof(pGetArrDataField->byFLen);
		memcpy(pArrDataField->byFData, pGetArrDataField->byFData, Asc2Int((char *)pGetArrDataField->byFLen, sizeof(pGetArrDataField->byFLen)));
		nAdd += Asc2Int((char *)pGetArrDataField->byFLen, sizeof(pGetArrDataField->byFLen));
		fnAPP_SetAddArrDataField(pArrDataField, RX_RECV_F);
	}

	byTmpBuff[0] = fnAPP_SetBCC(m_RXECASH.byTData, nTLen);
	MakeUnPack(byTmpBuff, m_RXECASH.byBcc, 1);
	
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "byTLen - %4.4s", &m_RXECASH.byTLen);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "ByBcc  - %2.2s", &m_RXECASH.byBcc);

	if(RecvLength > 1024)
		RecvLength = 1024;

	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "RxRecv - RXFD",  &m_RXECASH, RecvLength);

//	MsgDump(TRACE_DUMP_MODE, "TXRX", __FILE__, __LINE__, "byTLen - %4.4s", &m_RXECASH.byTLen);
//	MsgDump(TRACE_DUMP_MODE, "TXRX", __FILE__, __LINE__, "ByBcc  - %2.2s", &m_RXECASH.byBcc);
//	HexDump(TRACE_DUMP_MODE, "TXRX", __FILE__, __LINE__, "RxRecv - RXFD",  &m_RXECASH, RecvLength);

	return T_OK;
}

int CTranCmn::fnAPP_RxECash_GetDataField_AnalProc(int nSec)
{

	CString strGetTxDataField("");
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString strTxGet("");
	CString strRxGet("");
	CString strDisp("");
	CString strGetIniDisp("");



	CString strGetMsgCode("");
	CString strGetMsgDetailCode("");
	int		nTxReturn, nRxReturn;

	int		nLangMode =  m_pDevCmn->fnSCR_GetCurrentLangMode(); 

	if(nLangMode == 4)    
		nLangMode = 1; 
	else
	if(nLangMode == 5)   
		nLangMode = 0;


	if (TranCode2 != TRANID_A010)
	{
		//일련번호
		nTxReturn = fnAPP_TXGetFindArryDataField(x003X, strlen(x003X), strGetTxDataField);
		nRxReturn = fnAPP_RXGetFindArryDataField(x003X, strlen(x003X), strGetRxDataField);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RxECash_GetDataField_AnalProc 0 (%s - %s)", 	strGetTxDataField, strGetRxDataField);


		if (TranCode2 == TRANID_A060)
		{
			m_pDevCmn->TranResult = TRUE;
			return T_OK;
		}

		if(nTxReturn && nRxReturn)
		{
			if(!strGetTxDataField.CompareNoCase(strGetRxDataField))
				m_pDevCmn->TranResult = TRUE;
			else
				m_pDevCmn->TranResult = FALSE;
		}

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래응답(x003X) => %s]", (m_pDevCmn->TranResult == TRUE)	? "OK" : "NG"));

		if(!m_pDevCmn->TranResult)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RxECash_GetDataField_AnalProc 1 (%s - %s)", 	strGetTxDataField, strGetRxDataField);
			m_pDevCmn->fnAPL_StackError("8850191", "전문불일치!!");

			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				fnAPP_CancelProc(T_MSG, "[수신전문거래코드이상]", "장애코드 (8850191)");
			else
				fnAPP_CancelProc(T_MSG, "Transaction is not valid", "장애코드 (8850191)");

			return T_OK;
		}

	}

	//전문종류 
	nTxReturn = fnAPP_TXGetFindArryDataField(x005X, strlen(x005X), strGetTxDataField);
	nRxReturn = fnAPP_RXGetFindArryDataField(x005X, strlen(x005X), strGetRxDataField);

	if(nTxReturn && nRxReturn)
	{
		strTxGet = strGetTxDataField.Left(2);
		strGetRxDataField = ZERO4 + strGetRxDataField;
		strRxGet = strGetRxDataField.Right(2);
		if(!strRxGet.CompareNoCase("10"))
		{
			m_pDevCmn->TranResult = TRUE;
		}
		else
		{
			m_pDevCmn->TranResult = FALSE;
		}
	}
	
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래응답(x005X) => %s]", (m_pDevCmn->TranResult == TRUE)	? "OK" : "NG"));

	//업무구분
	nTxReturn = fnAPP_TXGetFindArryDataField(x006X, strlen(x006X), strGetTxDataField);
	nRxReturn = fnAPP_RXGetFindArryDataField(x006X, strlen(x006X), strGetRxDataField);

	if(nTxReturn && nRxReturn)
	{

		if(!strGetTxDataField.CompareNoCase(strGetRxDataField))
			m_pDevCmn->TranResult = TRUE;
		else
			m_pDevCmn->TranResult = FALSE;


	}

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래응답(x006X) => %s]", (m_pDevCmn->TranResult == TRUE)	? "OK" : "NG"));

	if(!m_pDevCmn->TranResult)
	{
		m_pDevCmn->fnAPL_StackError("8850191", "전문불일치!!");

		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "[수신전문거래코드이상]", "장애코드 (8850191)");
		else
			fnAPP_CancelProc(T_MSG, "Transaction is not valid", "장애코드 (8850191)");

		return T_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	//#N0261
	/*
	nTxReturn = fnAPP_TXGetFindArryDataField(x023X, strlen(x023X), strGetTxDataField);
	nRxReturn = fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField);

	if(TranProc == TRAN_WITH) 
	{
		if(nTxReturn && nRxReturn)
		{

			if(!strGetTxDataField.CompareNoCase(strGetRxDataField))
				m_pDevCmn->TranResult = TRUE;
			else
				m_pDevCmn->TranResult = FALSE;
		}
		else
		if(!nRxReturn && (TranProc == TRAN_WITH ))
		{
			m_pDevCmn->TranResult = FALSE;
		}

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래응답4 => %s]", (m_pDevCmn->TranResult == TRUE)	? "OK" : "NG"));

		if(!m_pDevCmn->TranResult)
		{
			m_pDevCmn->fnAPL_StackError("8850191", "거래금액불일치!!");

			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				fnAPP_CancelProc(T_MSG, "[수신전문거래코드이상]", "장애코드 (8850191)");
			else
				fnAPP_CancelProc(T_MSG, "Transaction is not valid", "장애코드 (8850191)");

			return T_OK;
		}
	}
	*/
	//////////////////////////////////////////////////////////////////////////


	//기기번호
	nTxReturn = fnAPP_TXGetFindArryDataField(x011X, strlen(x011X), strGetTxDataField);
	nRxReturn = fnAPP_RXGetFindArryDataField(x011X, strlen(x011X), strGetRxDataField);

	if(nTxReturn && nRxReturn)
	{

		if(!strGetTxDataField.CompareNoCase(strGetRxDataField))
			m_pDevCmn->TranResult = TRUE;
		else
			m_pDevCmn->TranResult = FALSE;

	}
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래응답(x011X) => %s]", (m_pDevCmn->TranResult == TRUE)	? "OK" : "NG"));

	if(!m_pDevCmn->TranResult)
	{
		m_pDevCmn->fnAPL_StackError("8850191", "전문불일치!!");

		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "[수신전문거래코드이상]", "장애코드 (8850191)");
		else
			fnAPP_CancelProc(T_MSG, "Transaction is not valid", "장애코드 (8850191)");

		return T_OK;
	}
	

	nTxReturn = fnAPP_TXGetFindArryDataField(x008X, strlen(x008X), strGetTxDataField);
	nRxReturn = fnAPP_RXGetFindArryDataField(x008X, strlen(x008X), strGetRxDataField);

	//#CS0005
	if(nRxReturn)
	{
		int nIndexMsg = 0;
		strGetMsgDetailCode = ZERO3 + strGetRxDataField ;
		strGetMsgDetailCode = strGetMsgDetailCode.Right(3);
		strGetIniDisp       = IniGetStr(_ECASH_SVR_RES_INI, "SCREEN_MSG", strGetMsgDetailCode, SPACE3);

		if(!strGetRxDataField.CompareNoCase("000"))
		{
			m_pDevCmn->TranResult = TRUE;
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래응답(x008X) => %s]", (m_pDevCmn->TranResult == TRUE)	? "OK" : "NG"));
		}
		else
		if(!strGetRxDataField.CompareNoCase("KOK"))
		{
			m_pDevCmn->TranResult = TRUE;
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래응답(x008X) => %s]", (m_pDevCmn->TranResult == TRUE)	? "OK" : "NG"));
		}
		else
		if(!strGetRxDataField.CompareNoCase("XXX"))
		{
			m_pDevCmn->TranResult = FALSE;
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래응답(x008X) => %s]", (m_pDevCmn->TranResult == TRUE)	? "OK" : "NG"));
			if(fnAPP_Get_BankOrgCodeInt(CardBank) == nEnCmd003_3 || fnAPP_Get_BankOrgCodeInt(CardBank) == nEnCmd006_6)
			{
				fnAPP_Tran_TxSend2( TRANID_2190, LINE_OPCL_TIME);
				fnAPP_Tran_RxRecv2( TRANID_2190, LINE_OPCL_TIME);

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "[점검중]", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");
				else
					fnAPP_CancelProc(T_MSG, "Transaction is not valid", "시스템 점검 중 입니다.");
			}
			else
			{
				TranResultNGCode = 999;   
				fnAPP_CancelProc(T_DISMSG, strDisp.GetBuffer(0));
			}

		
		
		}
		else
		if(!strGetRxDataField.CompareNoCase("X02"))
		{
			m_pDevCmn->TranResult = FALSE;
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래응답(x008X) => %s]", (m_pDevCmn->TranResult == TRUE)	? "OK" : "NG"));
			m_pDevCmn->TranResult = FALSE;
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래응답(x008X) => %s]", (m_pDevCmn->TranResult == TRUE)	? "OK" : "NG"));
			if(fnAPP_Get_BankOrgCodeInt(CardBank) == nEnCmd269_269 || fnAPP_Get_BankOrgCodeInt(CardBank) == nEnCmd279_279)
			{
				fnAPP_Tran_TxSend2( TRANID_2190, LINE_OPCL_TIME);
				fnAPP_Tran_RxRecv2( TRANID_2190, LINE_OPCL_TIME);

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "[점검중]", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");
				else
					fnAPP_CancelProc(T_MSG, "Transaction is not valid", "시스템 점검 중 입니다.");
			}
			else
			{
				TranResultNGCode = 999;   
				fnAPP_CancelProc(T_DISMSG, strDisp.GetBuffer(0));
			}
		}


// 		else
// 		if(!strGetMsgCode.CompareNoCase("11") || !strGetMsgCode.CompareNoCase("13"))
// 		{
// 			m_pDevCmn->TranResult = FALSE;
// 			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래응답5 => %s]", (m_pDevCmn->TranResult == TRUE)	? "OK" : "NG"));
// 			
// 			if(CardBank == SASSBANK)  //#N0133 (복호화 필요)
// 			{
// 
// 				if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 					MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x0151[%s]" , x0151);
// 				else
// 				{
// 					int nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 					fnAPP_TRAN_DEC_240(TranCode2);
// 				}
// 				
// 			}
// 		}
		else
		{
			nIndexMsg = Asc2Int(strGetMsgDetailCode);
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[응답코드(x008X): %s]", strGetMsgDetailCode));
			strDisp.Format("응답코드 : %s\n%s", strGetMsgDetailCode, strGetIniDisp);

			m_pDevCmn->TranResult = FALSE;
			TranResultNGCode = nIndexMsg;

			if( nIndexMsg == 799 ) //암호화전문
			{
			
				if(CardBank == SASSBANK)
				{
					sprintf(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,"%2.2s","00");
					fnAPP_BankKey_Set_Flag(SST_ORG_INT_240, K_S_F);
					MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_RxECash_GetDataField_AnalProc (CardBank R1 %d nIndexMsg %d)", CardBank, nIndexMsg);		
					
					fnAPP_Tran_TxSend2( TRANID_2190, LINE_OPCL_TIME);
					fnAPP_Tran_RxRecv2( TRANID_2190, LINE_OPCL_TIME);
					MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_RxECash_GetDataField_AnalProc (CardBank R2 %d nIndexMsg %d)", CardBank, nIndexMsg);		
					
					fnAPP_Tran_TxSend2( TRANID_2190, LINE_OPCL_TIME, 1);
					fnAPP_Tran_RxRecv2( TRANID_2190, LINE_OPCL_TIME);
					MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_RxECash_GetDataField_AnalProc (CardBank R2 %d nIndexMsg %d)", CardBank, nIndexMsg);		
				}
				else
				{
					fnAPP_Tran_TxSend2( TRANID_2190, LINE_OPCL_TIME);
					fnAPP_Tran_RxRecv2( TRANID_2190, LINE_OPCL_TIME);
				}
				
				if (TranCode2 == TRANID_7200 || TranCode2 == TRANID_4720 || TranCode2 == TRANID_4410 )
				{

					if (m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
						strDisp.Format("[점검중]\n 응답코드:%s\n %s\n", "Transaction is not valid", " Please repeat from the beginning.");	
					else
						strDisp.Format("[점검중]\n 응답코드:%s\n %s\n", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");	

					m_pDevCmn->fnSCR_DisplayStringArray(1, strDisp,  "\n");
					m_pDevCmn->fnSCR_DisplayScreen(701);
					Delay_Msg(3000);
					fnAPP_CancelDepRejectProc(T_DISMSG, strDisp.GetBuffer(0));
				}
				else
				if (TranCode2 == TRANID_4320 || TranCode2 == TRANID_4340)
				{
					TranResultNGCode = nIndexMsg;
					return T_OK;
				}
				else
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "[점검중]", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");
				else
					fnAPP_CancelProc(T_MSG, "Transaction is not valid", "시스템 점검 중 입니다.");
			}
			else
			if( nIndexMsg == 14 ) //암호화전문
			{
				if(fnAPP_Get_BankOrgCodeInt(CardBank) == nEnCmd003_3 || fnAPP_Get_BankOrgCodeInt(CardBank) == nEnCmd006_6)
				{
					fnAPP_Tran_TxSend2( TRANID_2190, LINE_OPCL_TIME);
					fnAPP_Tran_RxRecv2( TRANID_2190, LINE_OPCL_TIME);
				}

				if (TranCode2 == TRANID_7200 || TranCode2 == TRANID_4720 || TranCode2 == TRANID_4410 )
				{
					if (m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
						strDisp.Format("[점검중]\n 응답코드:%s\n %s\n", "Transaction is not valid", " Please repeat from the beginning.");	
					else
						strDisp.Format("[점검중]\n 응답코드:%s\n %s\n", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");	

					m_pDevCmn->fnSCR_DisplayStringArray(1, strDisp,  "\n");
					m_pDevCmn->fnSCR_DisplayScreen(701);
					Delay_Msg(3000);
					fnAPP_CancelDepRejectProc(T_DISMSG, strDisp.GetBuffer(0));
				}
				else
				if (TranCode2 == TRANID_4320 || TranCode2 == TRANID_4340)
				{
					TranResultNGCode = nIndexMsg;
					return T_OK;
				}
				else
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "[점검중]", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");
				else
					fnAPP_CancelProc(T_MSG, "Transaction is not valid", "시스템 점검 중 입니다.");
			}
			else
			if( nIndexMsg == 798 ) 
			{
				TranResultNGCode = nIndexMsg;
				m_pDevCmn->m_nAtmsCommand = TRAN_MSG_EXCEPTION_798;
				m_pDevCmn->TranResult = TRUE;

				//fnAPP_CancelProc(T_DISMSG, strDisp.GetBuffer(0));
				if (TranCode2 == TRANID_7200 || TranCode2 == TRANID_4720 || TranCode2 == TRANID_4410 )
				{
					if (m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
						strDisp.Format("[점검중]\n 응답코드:%s\n %s\n", "Transaction is not valid", " Please repeat from the beginning.");	
					else
						strDisp.Format("[점검중]\n 응답코드:%s\n %s\n", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");	

					m_pDevCmn->fnSCR_DisplayStringArray(1, strDisp,  "\n");
					m_pDevCmn->fnSCR_DisplayScreen(701);
					Delay_Msg(3000);
					fnAPP_CancelDepRejectProc(T_DISMSG, strDisp.GetBuffer(0));
				}
				else
				if (TranCode2 == TRANID_4320 || TranCode2 == TRANID_4340)
				{
					TranResultNGCode = nIndexMsg;
					return T_OK;
				}
				else
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "[점검중]", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");
				else
					fnAPP_CancelProc(T_MSG, "Transaction is not valid", "시스템 점검 중 입니다.");
			}
			else
			if(nIndexMsg == 94)
			{
				TranResultNGCode = nIndexMsg;
				m_pDevCmn->m_nAtmsCommand = TRAN_MSG_EXCEPTION_094;
				m_pDevCmn->TranResult = TRUE;
				//fnAPP_CancelProc(T_DISMSG, strDisp.GetBuffer(0));

				if (TranCode2 == TRANID_7200 || TranCode2 == TRANID_4720 || TranCode2 == TRANID_4410 )
				{
					if (m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
						strDisp.Format("[점검중]\n 응답코드:%s\n %s\n", "Transaction is not valid", " Please repeat from the beginning.");	
					else
						strDisp.Format("[점검중]\n 응답코드:%s\n %s\n", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");	

					m_pDevCmn->fnSCR_DisplayStringArray(1, strDisp,  "\n");
					m_pDevCmn->fnSCR_DisplayScreen(701);
					Delay_Msg(3000);
					fnAPP_CancelDepRejectProc(T_DISMSG, strDisp.GetBuffer(0));
				}
				else
				if (TranCode2 == TRANID_4320 || TranCode2 == TRANID_4340)
				{
					TranResultNGCode = nIndexMsg;
					return T_OK;
				}
				else
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "[점검중]", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");
				else
					fnAPP_CancelProc(T_MSG, "Transaction is not valid", "시스템 점검 중 입니다.");
			}
			else
			if( nIndexMsg == 845)
			{
				TranResultNGCode = nIndexMsg;
				m_pDevCmn->m_nAtmsCommand = TRAN_MSG_EXCEPTION_845;
				m_pDevCmn->TranResult = TRUE;
				//fnAPP_CancelProc(T_DISMSG, strDisp.GetBuffer(0));

				if (TranCode2 == TRANID_7200 || TranCode2 == TRANID_4720 || TranCode2 == TRANID_4410 )
				{
					if (m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
						strDisp.Format("[점검중]\n 응답코드:%s\n %s\n", "Transaction is not valid", " Please repeat from the beginning.");	
					else
						strDisp.Format("[점검중]\n 응답코드:%s\n %s\n", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");	

					m_pDevCmn->fnSCR_DisplayStringArray(1, strDisp,  "\n");
					m_pDevCmn->fnSCR_DisplayScreen(701);
					Delay_Msg(3000);
					fnAPP_CancelDepRejectProc(T_DISMSG, strDisp.GetBuffer(0));
				}
				else
				if (TranCode2 == TRANID_4320 || TranCode2 == TRANID_4340)
				{
					TranResultNGCode = nIndexMsg;
					return T_OK;
				}
				else
				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "[점검중]", "기관간 일시적 전산장애", "잠시 후 다시 거래하여 주시기 바랍니다");
				else
					fnAPP_CancelProc(T_MSG, "Transaction is not valid", "시스템 점검 중 입니다.");
			}
	
			m_pDevCmn->TranResult = FALSE;
				
			if(TranCode2 == TRANID_7200)	
			{
				TranResultNGCode = nIndexMsg;  
				fnAPP_CancelDepRejectProc(T_DISMSG, strDisp.GetBuffer(0));
			}
			else
			if(TranCode2 == TRANID_4720 || TranCode2 == TRANID_4410)
			{
				TranResultNGCode = nIndexMsg;
				if(nIndexMsg != 68 && nIndexMsg != 97)
					fnAPP_CancelDepRejectProc(T_DISMSG, strDisp.GetBuffer(0));
				else
					fnAPP_CancelProc(T_DISMSG, strDisp.GetBuffer(0));
			}
			else
			if(TranCode2 == TRANID_2190)
			{
				TranResultNGCode = TRAN_MSG_EXCEPTION_2190;
				return T_OK;
			}
			else
			if(TranCode2 == TRANID_A010)
			{
				m_pDevCmn->fnSCR_DisplayStringArray(1, strDisp,  "\n");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				Delay_Msg(2000);

				TranResultNGCode = TRAN_MSG_EXCEPTION_A010;
				return T_OK;
			}
			else
			if(TranCode2 == TRANID_A020)
			{
				m_pDevCmn->fnSCR_DisplayStringArray(1, strDisp,  "\n");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				Delay_Msg(2000);

				TranResultNGCode = TRAN_MSG_EXCEPTION_A020;
				return T_OK;
			}
			else
			if (TranCode2 == TRANID_4320 || TranCode2 == TRANID_4340)
			{
				TranResultNGCode = nIndexMsg;
				return T_OK;
			}
			else
			{
				TranResultNGCode = nIndexMsg;   
				fnAPP_CancelProc(T_DISMSG, strDisp.GetBuffer(0));
			}
		}
		
	}
	
	
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_RxECash_GetDataField_AnalProc() return");
	return T_OK;
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 전문Format구분 - Header 구분(A:고정해더,그외:가변헤더)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/ 
int CTranCmn::fnAPP_SetDataField_Item_081X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x081X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "A", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);		
	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 거래날짜 - 전문을 송신하는 시점의 날짜
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/ 
int CTranCmn::fnAPP_SetDataField_Item_001X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x001X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "008", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, GetDate().GetBuffer(0), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);		
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 거래시각 - 전문을 송신하는 시점의 시각
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/ 

int CTranCmn::fnAPP_SetDataField_Item_002X(int nTranCode , int nTxRxOption) 
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x002X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
	sprintf(m_pProfile->TRANS.YYYYMMDDHHMMSS, "%8.8s%6.6s", GetDate(), GetTime());
	memcpy(pArrDataField->byFData, &m_pProfile->TRANS.YYYYMMDDHHMMSS[8], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	m_pProfile->PutTransProfile();
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);		
	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : SST 일련번호 - 단말기가 관리하는 전문 일련번호
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/ 
int CTranCmn::fnAPP_SetDataField_Item_003X(int nTranCode , int nTxRxOption)
{
	char	szTempSerialNo[256];
	DataField *pArrDataField = new DataField;
	memcpy(szTempSerialNo, m_pProfile->TRANS.SerialNo, 6);

	memcpy(pArrDataField->byFCode, x003X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTempSerialNo, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : //제휴사 일련 번호(계좌이체 조회시 수신하며 이 값을  계좌이체 시 보냄) 
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_004X(int nTranCode , int nTxRxOption)
{
	int nLen = 0;
	char szTmp[1024];
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x004X, sizeof(pArrDataField->byFCode));
	
	nLen = FindChar(m_RD.byDataField_004x, sizeof(m_RD.byDataField_004x), NULL);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_024X nLen[%d]", nLen);
	
	//ECash - 예외처리
	if(nLen < 1)
	{
		memcpy(pArrDataField->byFLen, "010", sizeof(pArrDataField->byFLen));
		memset(pArrDataField->byFData, '0', Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		memset(szTmp, 0x00, sizeof(szTmp));
		Int2Asc(nLen, szTmp, 3);
		memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_RD.byDataField_004x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 원거래 단말기 일련번호 (050X)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/ 
int CTranCmn::fnAPP_SetDataField_Item_050X(int nTranCode , int nTxRxOption)
{
	char	szTempSerialNo[256];
	DataField *pArrDataField = new DataField;
	memcpy(szTempSerialNo, m_pProfile->TRANS.SerialNo, 6);

	memcpy(pArrDataField->byFCode, x050X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTempSerialNo, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 전문종류(Msg-Type) - “전문종류”참조
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_005X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;

	memcpy(pArrDataField->byFCode, x005X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));

	switch(nTranCode)
	{
	case TRANID_A010: //단말기 개국전문    
	case TRANID_A020: //단말기 상태전문
	case TRANID_A040: //단말기 제어전문
	case TRANID_A060: //단말기 시제전문 
		memcpy(pArrDataField->byFData, "0800", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_2190: //단말기 승인거래
		memcpy(pArrDataField->byFData, "0200", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_7210: //수수료조회
	case TRANID_7220: //예금조회
	case TRANID_7260: //수취계좌조회
	case TRANID_7230: //서비스조회
	case TRANID_7250: //서비스이체조회
	case TRANID_7380: //이자율조회
	case TRANID_7530: //이자율조회(해외)
	case TRANID_4370: //자행이체		
	case TRANID_4390: //계좌이체 
	case TRANID_4310: //예금출금
	case TRANID_4330: //서비스출금   
	case TRANID_4350: //서비스이체
	case TRANID_4410: //카드입금
		memcpy(pArrDataField->byFData, "0200", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	case TRANID_4320: //예금출금취소
	case TRANID_4340: //서비스출금취소
		memcpy(pArrDataField->byFData, "5400", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	default:
		memcpy(pArrDataField->byFData, "0200", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 업무구분코드(Tran-ID) - “업무구분코드”참조
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_006X(int nTranCode , int nTxRxOption)
{
	char	szTemp[256];
	DataField *pArrDataField = new DataField;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_006X (%d)", nTranCode);

	memcpy(pArrDataField->byFCode, x006X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	switch(nTranCode)
	{
	case TRANID_A010: //단말기 개국전문    
		memcpy(pArrDataField->byFData, "A010", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_A020: //단말기 상태전문
		memcpy(pArrDataField->byFData, "A020", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_A040: //단말기 상태전문
		memcpy(pArrDataField->byFData, "A040", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_A060: //단말기 시제전문 
		memcpy(pArrDataField->byFData, "A060", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_7380: //이자율조회
	case TRANID_7530: //이자율조회(해외)
		if ((nTranCode == TRANID_7380) && (nEnCmd127_127 == fnAPP_Get_BankOrgEncInt(CardBank, nTranCode)))
		{
			sprintf(szTemp,"%04d" ,TRANID_7230);
		}
		else
		{
			sprintf(szTemp,"%04d" ,nTranCode);
		}
		memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_7210: //수수료조회
	case TRANID_7220: //예금조회
	case TRANID_7260: //수취계좌조회
	case TRANID_7230: //서비스조회
	case TRANID_7250: //서비스이체조회
	case TRANID_4370: //자행이체		
	case TRANID_4390: //계좌이체 
	case TRANID_4310: //예금출금
	case TRANID_4330: //서비스출금   
	case TRANID_4350: //서비스이체
	case TRANID_4410: //카드입금
	case TRANID_4320: //예금출금취소
	case TRANID_4340: //서비스출금취소
		if ((nTranCode == TRANID_4410) && (nEnCmd004_6 == fnAPP_Get_BankOrgEncInt(CardBank, nTranCode)))
		{
			sprintf(szTemp,"%04d" ,TRANID_4420);
		}
		else
		{
			sprintf(szTemp,"%04d" ,nTranCode);
		}
		memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	default:
		sprintf(szTemp,"%04d" ,nTranCode);
		memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	}
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 거래매체구분 - 통장:1, ISO2:2, ISO3:3, 국민은행(입금만):4
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_007X(int nTranCode , int nTxRxOption)
{
	
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x007X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));

	if(MenuAvail & MENU_MU) 
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
	if(MenuAvail & MENU_PB)
		memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
	if(MenuAvail & MENU_MCU3)
	{
		if(MenuFICMode & MENU_IC) 
		{
			if ((nTranCode == TRANID_4410) && (nEnCmd004_6 == fnAPP_Get_BankOrgEncInt(CardBank, nTranCode)))
				memcpy(pArrDataField->byFData, "4", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			else
				memcpy(pArrDataField->byFData, "3", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		}
		else
			memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if(MenuAvail & MENU_MCU2)
	{
		if(EMVReadFlg == TRUE)
		{
			memcpy(pArrDataField->byFData, "3", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));	  //#N9999			
		}
		else
		if((FallBackEMVMode == TRUE) && (!CardOnEMV) && (TranCode == TC_SVCWITH || TranCode == TC_SVCINQ || TranCode == TC_SVCTRAN1 || TranCode == TC_SVCTRAN))
		{
			memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
		if((FallBackEMVMode == TRUE) && (TranCode == TC_SVCWITH || TranCode == TC_SVCINQ || TranCode == TC_SVCTRAN1 || TranCode == TC_SVCTRAN))
		{
			memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));				
		}
		else
			memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		
	
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 예 1) 마감전 : 0예 2) 마감후 : 1예 3) 24시간영업(휴일 포함) : 2
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_009X(int nTranCode , int nTxRxOption)
{

	if (TranEncCode == nEnCmd003_3)
		return T_OK;

	DataField *pArrDataField = new DataField;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_009X (%d)", nTranCode);
	memcpy(pArrDataField->byFCode, x009X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));

	//#CS0002
	if ((TranEncCode == nEnCmd034_34) && (nTranCode == TRANID_4310))
		memcpy(pArrDataField->byFData, m_RD.byDataField_009x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
	if ((TranEncCode == nEnCmd243_243) && (nTranCode == TRANID_4370 || nTranCode == TRANID_4390))  //#CS0003
		memcpy(pArrDataField->byFData, m_RD.byDataField_009x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
		memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 거래매체구분 - 0':무매체,'1':통장,'2':MS카드,'3':IC카드,'4':RF카드,'5':모바일, '6':F/B
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_071X(int nTranCode , int nTxRxOption)
{
	
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x071X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));

	if(MenuAvail & MENU_MU) 
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
	if(MenuAvail & MENU_PB)
		memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
	if(MenuAvail & MENU_MCU3)
	{
		if(MenuFICMode & MENU_IC) 
		{
			memcpy(pArrDataField->byFData, "3", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
			memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if(MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranProc == TRAN_FOREGIN || TranProc == TRAN_FOREGIN_INQ))
		{
			memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
		if(EMVReadFlg == TRUE)
		{
			memcpy(pArrDataField->byFData, "3", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));	  //#N9999			
		}
		else
		if((FallBackEMVMode == TRUE) && (!CardOnEMV) && (TranCode == TC_SVCWITH || TranCode == TC_SVCINQ || TranCode == TC_SVCTRAN1 || TranCode == TC_SVCTRAN))
		{
			memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
		if((FallBackEMVMode == TRUE) && (TranCode == TC_SVCWITH || TranCode == TC_SVCINQ || TranCode == TC_SVCTRAN1 || TranCode == TC_SVCTRAN))
		{
			memcpy(pArrDataField->byFData, "6", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));				
		}
		else
			memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		
	
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 응답코드(RspCode) - 전문처리결과-“응답코드” 참조
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_008X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x008X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "005", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, ZERO5, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}


/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 0:안함,1:기관암호화,2:중계망암호,3:기관&중계망
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/


int CTranCmn::fnAPP_SetDataField_Item_010X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x010X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));

	switch(nTranCode)
	{
	case TRANID_A010: //단말기 개국전문    
	case TRANID_A020: //단말기 상태전문
	case TRANID_A040: //단말기 제어전문
	case TRANID_A060: //단말기 시제전문 
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_2190: //단말기 승인거래 (삼성증권 암호화 2만 확인 필요)
		if (CardBank == KUB_ORG_INT_003 || CardBank == WRB_ORG_INT_020 || CardBank == KJB_ORG_INT_034 || CardBank == HWT_ORG_INT_269 || ((CardBank == SST_ORG_INT_240) &&  (Asc2Int(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,2) == CIPHERTEXT_TMP)))
			memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		else
			memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 

		break;
	case TRANID_7230: //서비스조회
	case TRANID_7250: //서비스이체조회
	case TRANID_7380: //이자율조회
	case TRANID_7530: //이자율조회(해외)
	case TRANID_4330: //서비스출금   
	case TRANID_4350: //서비스이체
	case TRANID_4340: //서비스출금취소
		memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_7210: //수수료조회
	case TRANID_7220: //예금조회
	case TRANID_7260: //수취계좌조회
	case TRANID_4370: //자행이체		
	case TRANID_4390: //계좌이체 
	case TRANID_4310: //예금출금
	case TRANID_4410: //카드입금
	case TRANID_4320: //예금출금취소
	case TRANID_7200: //입금조회
		if ((MenuFICMode & MENU_IC) && ((CardBank == HWT_ORG_INT_269) || (CardBank == KJB_ORG_INT_034) || (CardBank == SST_ORG_INT_240))) //-> 한화, 광주, 삼성
		{
			TranEncCode = fnAPP_EncType(TranCode2);			
			if (TranEncCode == nEnCmd209_379_PUBLIC || TranEncCode == nEnCmd218_218_PUBLIC)
			{
				memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
			}
			else
			{
				memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
			}

		}
		else
			memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	default:
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : SST기기번호(MecaNum) - 단말기의 기기번호
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_011X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memcpy(pArrDataField->byFCode, x011X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "014", sizeof(pArrDataField->byFLen));
	sprintf(szTmp, "%6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         :암호화 종류 - 00:기타Or없음,'01':SEED,'02':소프트포럼,'03':이니텍
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
// 현대증권 = 00, 국민 = 02
int CTranCmn::fnAPP_SetDataField_Item_072X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;	
	memcpy(pArrDataField->byFCode, x072X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
	switch(nTranCode)
	{
	case TRANID_A010: //단말기 개국전문    
	case TRANID_A020: //단말기 상태전문
	case TRANID_A040: //단말기 제어전문
	case TRANID_A060: //단말기 시제전문 
		memcpy(pArrDataField->byFData, "00", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_2190: //단말기 승인거래
//		if ((CardBank == nEnCmd004_6) || (CardBank == nEnCmd003_3))
		if (fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd004_6)
			memcpy(pArrDataField->byFData, "02", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		else
		if ((fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd011_11) || (fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd012_12))
			memcpy(pArrDataField->byFData, "03", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
// 		else
// 		if ((fnAPP_Get_BankOrgCode(CardBank) == nEnCmd027_53))
// 			memcpy(pArrDataField->byFData, "04", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		else
			memcpy(pArrDataField->byFData, "00", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_7230: //서비스조회
	case TRANID_7250: //서비스이체조회
	case TRANID_7380: //이자율조회
	case TRANID_7530: //이자율조회(해외)
	case TRANID_4330: //서비스출금   
	case TRANID_4350: //서비스이체
		memcpy(pArrDataField->byFData, "00", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_7210: //수수료조회
	case TRANID_7220: //예금조회
	case TRANID_7260: //수취계좌조회
	case TRANID_4370: //자행이체		
	case TRANID_4390: //계좌이체 
	case TRANID_4310: //예금출금
	case TRANID_4410: //카드입금
		if (fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd004_6) 
		{
			if (nTranCode == TRANID_4410)
				memcpy(pArrDataField->byFData, "70", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
			else
				memcpy(pArrDataField->byFData, "02", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 

		}
		else
		if ((fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd011_11) || (fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd012_12))
			memcpy(pArrDataField->byFData, "03", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		else
		if ((fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd027_27))
			memcpy(pArrDataField->byFData, "04", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		else
			memcpy(pArrDataField->byFData, "00", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	case TRANID_4320: //예금출금취소
	case TRANID_4340: //서비스출금취소
		if (fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd004_6)
			memcpy(pArrDataField->byFData, "02", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		else
		if ((fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd011_11) || (fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd012_12))
			memcpy(pArrDataField->byFData, "03", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		else
		if ((fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd027_27))
			memcpy(pArrDataField->byFData, "04", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		else
			memcpy(pArrDataField->byFData, "00", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 

		break;
	default:
			memcpy(pArrDataField->byFData, "00", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen))); 
		break;
	}
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 신용카드정보유무 - ISO2정보의 신용카드 여부(1/0)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_048X(int nTranCode , int nTxRxOption)
{
	if(MenuAvail & MENU_MCU2 || MenuAvail & MENU_MCU3)
	{
		DataField *pArrDataField = new DataField;
		memcpy(pArrDataField->byFCode, x048X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));

		switch(nTranCode)
		{
		case TRANID_4320: //예금출금취소
		case TRANID_4719: //입금취소
		case TRANID_4310: //예금출금
		case TRANID_4315: //은행이체출금 
		case TRANID_4390: //계좌이체 //금융공동망IC카드 계좌이체
		case TRANID_7260: //수취계좌조회 //금융IC카드 수취계좌조회
		case TRANID_7220: //예금조회 //금융IC카드 예금조회
		case TRANID_7200: //입금가능여부조회
		case TRANID_7210: //수수료조회
		case TRANID_4410: //카드입금
			memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			break;
		case TRANID_4340: //서비스출금취소
		case TRANID_4330: //서비스출금   
		case TRANID_4350: //서비스이체
		case TRANID_7230: //서비스조회
		case TRANID_7380: //이자율조회 (현금서비스 출금) 
		case TRANID_7530: //이자율조회(해외)
		case TRANID_7250: //광주 서비스 이체 조회
			memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			break;
		default:
			break;
		}
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : SST기종구분 - 기기종류를 구분함
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_049X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x049X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, NH_ATM_TYPE, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}


/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 현금장입금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_055X(int nTranCode , int nTxRxOption)
{
	char szAmt[1024];
	int nCurRB2Cnt = 0;
	int nCurRB1Cnt = 0;
	CString IniGet("");

	if ((nTranCode != TRANID_A060) && (nTranCode != TRANID_A020))
		return T_OK;

// 	if (m_pDevCmn->SCSTCntF == 0)
// 		return T_OK;

	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x055X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "010", sizeof(pArrDataField->byFLen));

	memset(szAmt, 0x00, sizeof(szAmt));

	if (nTranCode == TRANID_A060)
	{
		IniGet = IniGetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_T0GAP",   ZERO10);
	}
	else
	{
		IniGet = IniGetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_A0GAP",   ZERO10);
	}

	if (IniGet.CollateNoCase("FFFF") == 0)
	{
		IniGet = ZERO3;
	}
	sprintf(szAmt, "%s", IniGet);

	memcpy(pArrDataField->byFData, szAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 현금장입시각
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_056X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	memcpy(pArrDataField->byFCode, x056X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "014", sizeof(pArrDataField->byFLen));

	if (m_pDevCmn->SCSTCntF == 2)
	{
		sprintf(szTemp, "%8.8s%6.6s", GetDate().GetBuffer(0), GetTime().GetBuffer(0));
		memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		sprintf(szTemp, "%s", IniGetStr(_SP_BRM_CSTINFO_INI, "REFILL", "HOST_DATETIME_LAST", "00000000"));
		memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 출금가능 금액(수표)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_059X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x059X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "009", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "000000000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 권종별 매수정보
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_061X(int nTranCode , int nTxRxOption)
{


	if (nTranCode == TRANID_7220)
		return T_OK;

	if (nTranCode == TRANID_7230)
		return T_OK;

	if (nTranCode == TRANID_7530)
		return T_OK;

	if (nTranCode == TRANID_7380)
		return T_OK;

	if (nTranCode == TRANID_4350)
		return T_OK;

	if (nTranCode == TRANID_7250)
		return T_OK;

	DataField *pArrDataField = new DataField;
	char szSetNote[1024];
	int nCurRB2Cnt = 0;
	int nCurRB1Cnt = 0;
	memset(szSetNote, 0x30, sizeof(szSetNote));



	memcpy(pArrDataField->byFCode, x061X, sizeof(pArrDataField->byFCode));

	nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원
	nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
	nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
	nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

	if (nTranCode == TRANID_A020)
	{
		memcpy(pArrDataField->byFLen, "012", sizeof(pArrDataField->byFLen));
		sprintf(szSetNote, "04%04d03%04d",nCurRB1Cnt, nCurRB2Cnt);
	}
	else
	if (nTranCode == TRANID_A060)
	{
		memcpy(pArrDataField->byFLen, "012", sizeof(pArrDataField->byFLen));
		sprintf(szSetNote, "04%04d03%04d",nCurRB1Cnt, nCurRB2Cnt);
	}
	else
	if ((nTranCode == TRANID_4310) || (nTranCode == TRANID_4330))
	{
		memcpy(pArrDataField->byFLen, "012", sizeof(pArrDataField->byFLen));
		int nDisp10000 = Asc2Int(Accept.Cash10T_Money, 11);
		int nDisp50000 = Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND;
		sprintf(szSetNote, "04%04d03%04d",nDisp50000, nDisp10000);
	}
	else
	if (nTranCode == TRANID_4410)
	{
		memcpy(pArrDataField->byFLen, "024", sizeof(pArrDataField->byFLen));
		sprintf(szSetNote, "04%04d03%04d02%04d01%04d",CashData.mFiftyThousand, CashData.mTenThousand, CashData.mFiveThousand, CashData.mOneThousand);
	}
	else
	{
		memcpy(pArrDataField->byFLen, "012", sizeof(pArrDataField->byFLen));
		sprintf(szSetNote, "%012d", 0);
	}

	memcpy(pArrDataField->byFData, szSetNote, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 업무세부구분코드 
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_062X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x062X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "009", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "000000000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 취급기관코드1 - 거래가 처리되는 기관 (변환 기관코드)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_0120(int nTranCode , int nTxRxOption)
{
	
	DataField *pArrDataField = new DataField;
	char szTmp[1024] = {0,};
	int nRet = T_ERROR;
	memcpy(pArrDataField->byFCode, x0120, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));

	switch(nTranCode)
	{
	case TRANID_A010: //단말기 개국전문    
	case TRANID_A020: //단말기 상태전문
	case TRANID_A040: //단말기 제어전문
	case TRANID_A060: //단말기 시제전문 
		memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	case TRANID_2190: //단말기 승인거래
		memcpy(pArrDataField->byFData, fnAPP_Get_BankOrgEnc(CardBank, nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	case TRANID_7210: //수수료조회
	case TRANID_7220: //예금조회
	case TRANID_7260: //수취계좌조회
		if (MenuAvail & MENU_MCU3)
		{
			//현대공통망 분기 -> "정리필요반듯이"
			//memcpy(pArrDataField->byFData, fnAPP_Get_BankOrgEnc(Asc2Int(TYJSBANKID_PUBLIC, 3)), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

			TranEncCode = fnAPP_EncType(TranCode2);			
			if (m_pProfile->TRANS.TranService == TranEncCode)
			{
				sprintf(szTmp, "%03d", TranEncCode);
				memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}
			else
				memcpy(pArrDataField->byFData, fnAPP_Get_BankOrgEnc(Asc2Int(pCardData3->BankNo, 3), nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		break;
	case TRANID_7380: //이자율조
	case TRANID_7230: //서비스조회
	case TRANID_7250: //서비스이체조회
	case TRANID_7530:
		if (MenuAvail & MENU_MCU2)
		{
			if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranProc == TRAN_FOREGIN || TranProc == TRAN_FOREGIN_INQ))
			{
				CardBank = nEnCmd294_294_BC;
				memcpy(pArrDataField->byFData,  fnAPP_Get_BankOrgEnc(CardBank, nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}
			else
				memcpy(pArrDataField->byFData,  fnAPP_Get_BankOrgEnc(CardBank, nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		}
		break;

	case TRANID_4370: //자행이체		
	case TRANID_4390: //계좌이체 
	case TRANID_4310: //예금출금
		if (MenuAvail & MENU_MCU3)
		{
			TranEncCode = fnAPP_EncType(TranCode2);			
			if (m_pProfile->TRANS.TranService == TranEncCode)
			{
				sprintf(szTmp, "%03d", TranEncCode);
				memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}
			else
			{
				memcpy(pArrDataField->byFData, fnAPP_Get_BankOrgEnc(Asc2Int(pCardData3->BankNo, 3), nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}
		}
		break;
	case TRANID_4330: //서비스출금   
	case TRANID_4350: //서비스이체
		if (MenuAvail & MENU_MCU2)
		{
			if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranProc == TRAN_FOREGIN || TranProc == TRAN_FOREGIN_INQ))
			{

				CardBank = nEnCmd294_294_BC;
				memcpy(pArrDataField->byFData,  fnAPP_Get_BankOrgEnc(CardBank, nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}
			else
				memcpy(pArrDataField->byFData,  fnAPP_Get_BankOrgEnc(CardBank, nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		}
		break;
	case TRANID_7200: //입금가능여부조회
	case TRANID_4410: //카드입금
		if (MenuAvail & MENU_MCU3)
		{
// 			TranEncCode = fnAPP_EncType(nTranCode);
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_0120 nTranCode (%04d) => DepBank Type(%03d)", nTranCode, TranEncCode);
// 			memcpy(pArrDataField->byFData, fnAPP_Get_BankOrgEnc(TranEncCode, nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			TranEncCode = fnAPP_EncType(TranCode2);			
			if (m_pProfile->TRANS.TranService == nEnCmd209_379_PUBLIC || m_pProfile->TRANS.TranService == nEnCmd218_218_PUBLIC)
			{
				if (TranEncCode == nEnCmd006_6)
				{
					sprintf(szTmp, "%03d", nEnCmd006_6);
					memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				{
					sprintf(szTmp, "%03d", m_pProfile->TRANS.TranService);
					memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
			}
			else
			{
				memcpy(pArrDataField->byFData, fnAPP_Get_BankOrgEnc(Asc2Int(pCardData3->BankNo, 3), nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}
		}
		break;
	case TRANID_4320: //예금출금취소
		if (MenuAvail & MENU_MCU3)
		{
			TranEncCode = fnAPP_EncType(TranCode2);			
			if (m_pProfile->TRANS.TranService == TranEncCode)
			{
				sprintf(szTmp, "%03d", TranEncCode);
				memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}
			else
			{
				memcpy(pArrDataField->byFData, fnAPP_Get_BankOrgEnc(Asc2Int(pCardData3->BankNo, 3), nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}
		}
		break;
	case TRANID_4340: //서비스출금취소
		if (MenuAvail & MENU_MCU2)
		{
			if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranProc == TRAN_FOREGIN || TranProc == TRAN_FOREGIN_INQ))
			{

				CardBank = nEnCmd294_294_BC;
				memcpy(pArrDataField->byFData,  fnAPP_Get_BankOrgEnc(CardBank, nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}
			else
				memcpy(pArrDataField->byFData,  fnAPP_Get_BankOrgEnc(CardBank, nTranCode), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		}
		break;
	default:
		memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 발급기관 - 거래가 처리되는 기관 (매체 고유 데이터)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_016X(int nTranCode , int nTxRxOption)
{

DataField *pArrDataField = new DataField;
	char szTmp[1024] = {0,};
	int nRet = T_ERROR;
	memcpy(pArrDataField->byFCode, x016X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));


	switch(nTranCode)
	{
	case TRANID_A010: //단말기 개국전문    
	case TRANID_A020: //단말기 상태전문
	case TRANID_A040: //단말기 제어전문
	case TRANID_A060: //단말기 시제전문 
		memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	case TRANID_2190: //단말기 승인거래
		memcpy(pArrDataField->byFData, fnAPP_Get_BankOrgCode(CardBank), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	case TRANID_7210: //수수료조회
	case TRANID_7220: //예금조회
	case TRANID_7260: //수취계좌조회
		if (MenuAvail & MENU_MCU3)
		{
			memcpy(pArrDataField->byFData, pCardData3->BankNo, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		break;
	case TRANID_7380: //이자율조
	case TRANID_7530: //이자율조회(해외)
	case TRANID_7230: //서비스조회
	case TRANID_7250: //서비스이체조회
		if (MenuAvail & MENU_MCU2)
		{
			if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
			{
				if (CardServiceFlag == VISA_CARD)
				{
					memcpy(pArrDataField->byFData, "161", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == MASTERCARD)
				{
					memcpy(pArrDataField->byFData, "162", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == CUPCARD)
				{
					memcpy(pArrDataField->byFData, "165", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == AMEXCARD)
				{
					memcpy(pArrDataField->byFData, "160", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == JCBCARD)
				{
					memcpy(pArrDataField->byFData, "163", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == DINERS)
				{
					memcpy(pArrDataField->byFData, "164", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				{
					memcpy(pArrDataField->byFData, "169", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
			}
			else
			{
				memcpy(pArrDataField->byFData, fnAPP_Get_CreditOrgCode(pCardData2->MembershipNo).GetBuffer(0), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}

		}
		break;
	case TRANID_4370: //자행이체		
	case TRANID_4390: //계좌이체 
	case TRANID_4310: //예금출금
		if (MenuAvail & MENU_MCU3)
		{
			memcpy(pArrDataField->byFData, pCardData3->BankNo, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		break;
	case TRANID_4330: //서비스출금   
	case TRANID_4350: //서비스이체
		if (MenuAvail & MENU_MCU2)
		{
			if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
			{
				if (CardServiceFlag == VISA_CARD)
				{
					memcpy(pArrDataField->byFData, "161", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == MASTERCARD)
				{
					memcpy(pArrDataField->byFData, "162", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == CUPCARD)
				{
					memcpy(pArrDataField->byFData, "165", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == AMEXCARD)
				{
					memcpy(pArrDataField->byFData, "160", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == JCBCARD)
				{
					memcpy(pArrDataField->byFData, "163", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == DINERS)
				{
					memcpy(pArrDataField->byFData, "164", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				{
					memcpy(pArrDataField->byFData, "169", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
			}
			else
			{
				memcpy(pArrDataField->byFData, fnAPP_Get_CreditOrgCode(pCardData2->MembershipNo).GetBuffer(0), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}

			//memcpy(pArrDataField->byFData, fnAPP_Get_CreditOrgCode(pCardData2->MembershipNo).GetBuffer(0), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		break;
	case TRANID_7200: //입금가능여부조회
	case TRANID_4410: //카드입금
		if (MenuAvail & MENU_MCU3)
		{
			memcpy(pArrDataField->byFData, pCardData3->BankNo, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		break;
	case TRANID_4320: //예금출금취소
		if (MenuAvail & MENU_MCU3)
		{
			memcpy(pArrDataField->byFData, pCardData3->BankNo, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		break;
	case TRANID_4340: //서비스출금취소
		if (MenuAvail & MENU_MCU2)
		{
			if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
			{
				if (CardServiceFlag == VISA_CARD)
				{
					memcpy(pArrDataField->byFData, "161", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == MASTERCARD)
				{
					memcpy(pArrDataField->byFData, "162", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == CUPCARD)
				{
					memcpy(pArrDataField->byFData, "165", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == AMEXCARD)
				{
					memcpy(pArrDataField->byFData, "160", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == JCBCARD)
				{
					memcpy(pArrDataField->byFData, "163", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				if (CardServiceFlag == DINERS)
				{
					memcpy(pArrDataField->byFData, "164", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				{
					memcpy(pArrDataField->byFData, "169", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
			}
			else
			{
				memcpy(pArrDataField->byFData, fnAPP_Get_CreditOrgCode(pCardData2->MembershipNo).GetBuffer(0), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}

			//memcpy(pArrDataField->byFData, fnAPP_Get_CreditOrgCode(pCardData2->MembershipNo).GetBuffer(0), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		break;
	default:
		memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;

}


// int CTranCmn::fnAPP_SetDataField_Item_013X(int nTranCode , int nTxRxOption)
// {
// 
// 	if(MenuAvail & MENU_MCU3)
// 	{	
// 		DataField *pArrDataField = new DataField;
// 		char szTmp[1024];
// 
// 		memset(szTmp, 0x20, sizeof(szTmp));
// 
// 		if ((MenuFICMode & MENU_IC) && (fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd034_34 || fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd269_269 ||  fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd240_364))
// 			memcpy(pArrDataField->byFCode, x0131, sizeof(pArrDataField->byFCode));
// 		else
// 			memcpy(pArrDataField->byFCode, x013X, sizeof(pArrDataField->byFCode));
// 
// 		memcpy(pArrDataField->byFLen, "104", sizeof(pArrDataField->byFLen));
// 
// 		if (fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd034_34)
// 			memcpy(szTmp, m_STXEncBuff.byChiperMsdata, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 		else
// 			memcpy(szTmp, CardData.ISO3Buff, CardData.ISO3size);
// 
// 		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
// 	}
// 	
// 	return T_OK;	
// }

int CTranCmn::fnAPP_SetDataField_Item_013X(int nTranCode , int nTxRxOption)
{

	if(MenuAvail & MENU_MCU3)
	{	
		DataField *pArrDataField = new DataField;
		char szTmp[1024];

		memset(szTmp, 0x20, sizeof(szTmp));

		if ((MenuFICMode & MENU_IC) && (fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd034_34 || fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd269_269 ||  fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd240_364))
		{
			TranEncCode = fnAPP_EncType(nTranCode);			
			if (TranEncCode == nEnCmd209_379_PUBLIC || TranEncCode == nEnCmd218_218_PUBLIC)
			{
				memcpy(pArrDataField->byFCode, x013X, sizeof(pArrDataField->byFCode));
				memcpy(pArrDataField->byFLen, "104", sizeof(pArrDataField->byFLen));
				memcpy(szTmp, CardData.ISO3Buff, CardData.ISO3size);
			}
			else
			{
				memcpy(pArrDataField->byFCode, x0131, sizeof(pArrDataField->byFCode));
				memcpy(pArrDataField->byFLen, "104", sizeof(pArrDataField->byFLen));

				if (fnAPP_Get_BankOrgEncInt(CardBank, nTranCode) == nEnCmd034_34)
				{
					memcpy(szTmp, m_STXEncBuff.byChiperMsdata, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
				}
				else
				{
					memcpy(pArrDataField->byFLen, "104", sizeof(pArrDataField->byFLen));
					memcpy(szTmp, CardData.ISO3Buff, CardData.ISO3size);
				}
			}

		}
		else
		{
			memcpy(pArrDataField->byFCode, x013X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "104", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, CardData.ISO3Buff, CardData.ISO3size);
		}

		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	}
	
	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_0131(int nTranCode , int nTxRxOption)
{
	return T_OK;
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : ISO2 Data - Track 2 MS Data
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_014X(int nTranCode , int nTxRxOption)
{
	
	if(MenuAvail & MENU_MCU2)
	{
		DataField *pArrDataField = new DataField;
		char szTmp[1024];
		memset(szTmp, 0x20, sizeof(szTmp));

		//#CS0004
		//광주카드
		if (TranEncCode == nEnCmd034_34 && (nTranCode == TRANID_7230 || nTranCode == TRANID_7380 || nTranCode == TRANID_4330))
		{
			memcpy(pArrDataField->byFCode, x014X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "104", sizeof(pArrDataField->byFLen));

			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[0], 104);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
		}
		else
		{
			memcpy(pArrDataField->byFCode, x014X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "104", sizeof(pArrDataField->byFLen));

			memcpy(szTmp, CardData.ISO2Buff, CardData.ISO2size);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
		}
	}

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_0141(int nTranCode , int nTxRxOption)
{
	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 비밀번호 - 사용자 비밀번호 (신용카드, 공동망) 사용 
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_015X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));


	if(MenuAvail & MENU_MCU2)
	{
		if(TranEncCode == nEnCmd034_34) //광주 (Ms)
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[104], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		{

			//[정리필요반듯이]
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
	}
	else
	if(MenuAvail & MENU_MCU3)
	{

// 		//01.TranEncCode == nEnCmd218_218(Ms + pwd) (올림)
// 		if(TranEncCode == nEnCmd004_6 || TranEncCode == nEnCmd003_3 || TranEncCode == nEnCmd013_12 || TranEncCode == nEnCmd247_247 || TranEncCode == nEnCmd269_269 || TranEncCode == nEnCmd279_279) 
// 		{
// 			delete pArrDataField;
// 			return T_OK;
// 		}
// 
		if(TranEncCode == nEnCmd000_0)
		{
			memcpy(pArrDataField->byFCode, x015X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
		if(TranEncCode == nEnCmd003_3) //국민,기업
		{
			memcpy(pArrDataField->byFCode, x015X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));		
		}
		else
		if (TranEncCode == nEnCmd004_6)
		{
			delete pArrDataField;
			return T_OK;
		}
		else
		if(TranEncCode == nEnCmd005_5) //외환
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[0], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd007_7) //수협
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[0], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd012_12 || TranEncCode == nEnCmd011_11) //농협 -> 금융IC Only
		{
			memcpy(pArrDataField->byFCode, x015X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));					
		}
		else
		if(TranEncCode == nEnCmd020_20) //우리
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[0], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd023_23) //제일 
		{
			delete pArrDataField;
			return T_OK;
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		if(TranEncCode == nEnCmd032_32) //부산
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[0], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd034_34) //광주 (Ms)
		{
			if (nTranCode == TRANID_4320 || nTranCode == TRANID_4340)
			{
				memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
				memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
				memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
				memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));	
			}
			else
			{

				memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
				memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
				memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[104], 16);
				memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
			}
		}
		else
		if(TranEncCode == nEnCmd037_37) //전북 (Ms)
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[0], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd048_48) //신협
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[0], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd053_53) //씨티
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[0], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd071_71) //우체국
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd025_81) //하나
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[16], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd021_88) //신한,조흥 
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}

		else
		if(TranEncCode == nEnCmd105_105)  //외환
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd106_106) //국민
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd107_107) //수협
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd111_111) //농협
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
	
		else
		if(TranEncCode == nEnCmd120_120) //우리
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		
		else
		if(TranEncCode == nEnCmd121_121)  //확인
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		
		else
		if(TranEncCode == nEnCmd126_192)
		{
			delete pArrDataField;
			return T_OK;
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		
		else
		if(TranEncCode == nEnCmd127_127)
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		
		else
		if(TranEncCode == nEnCmd134_134) //-> 광주 암호 
		{
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));	
			
			if (nTranCode == TRANID_4320 || nTranCode == TRANID_4340)
			{
				memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
				memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
				memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
				memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));	
			}
			else
			{

				memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
				memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
				memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[104], 16);
				memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));	
			}

		}
		
		else
		if(TranEncCode == nEnCmd135_135)
		{
			delete pArrDataField;
			return T_OK;
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		
		else
		if(TranEncCode == nEnCmd137_137) //-> 전북 암호
		{
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[0], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			

		}
		
		else
		if(TranEncCode == nEnCmd184_184) //삼성
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		
		else
		if(TranEncCode == nEnCmd193_193) //현대
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		
		else
		if(TranEncCode == nEnCmd194_194) //비씨
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		
		else
		if(TranEncCode == nEnCmd195_195) //롯데
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		
// 		else
// 		if(TranEncCode == nEnCmd196_196) //삼성 아멕스 
// 		{
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
// 		}
		else
		if(TranEncCode == nEnCmd209_369 || TranEncCode == nEnCmd209_379_PUBLIC) //유안타증권 
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd218_218) //현대증권 
		{
			delete pArrDataField;
			return T_OK;
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}

		else
		if(TranEncCode == nEnCmd238_368) //대우증권 
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(TranEncCode == nEnCmd240_364) //삼성증권 
		{
			delete pArrDataField;
			return T_OK;
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		if(TranEncCode == nEnCmd243_243) //한국투자 
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}

		else
		if(TranEncCode == nEnCmd247_247) //NH투자증권 
		{
			delete pArrDataField;
			return T_OK;
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		if(TranEncCode == nEnCmd267_267) //대신증권 
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		if(TranEncCode == nEnCmd269_269) //한화증권 
		{
			delete pArrDataField;
			return T_OK;
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		if(TranEncCode == nEnCmd270_270) //하나대투증권 
		{
			delete pArrDataField;
			return T_OK;
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		if(TranEncCode == nEnCmd278_278) //신한금융 
		{
			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		if(TranEncCode == nEnCmd279_279) //동부금융 
		{
			delete pArrDataField;
			return T_OK;
// 			memcpy(pArrDataField->byFCode, x0151, sizeof(pArrDataField->byFCode));
// 			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		{
			memcpy(pArrDataField->byFCode, x015X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
	
	}
	else	
	{
			memcpy(pArrDataField->byFCode, x015X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
//비밀번호
int CTranCmn::fnAPP_SetDataField_Item_0151(int nTranCode , int nTxRxOption)
{
	//[정리필요반듯이]
	return T_OK;

	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, "0151", sizeof(pArrDataField->byFCode));
	MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_0151 nTrCode[%d] nTxRxOption[%d]" , nTranCode, nTxRxOption);

	if(TranEncCode == TRAN_ENC_000_00) 
	{
		if(MenuFICMode & MENU_IC)
		{
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
		{
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			return T_OK;	
		}

	}
	else
	if(MenuAvail & MENU_MCU2)
	{
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if(MenuFICMode & MENU_IC && MenuAvail & MENU_MCU3)
	{

		int nEncType = TranEncCode;
		if(nEncType == TRAN_ENC_003_06 || nEncType == TRAN_ENC_006_01) //국민,기업
		{
			memcpy(pArrDataField->byFLen, "024", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[104], 24);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(nEncType == TRAN_ENC_011_07) //농협
		{
			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		if(nEncType == TRAN_ENC_025_04) //하나
		{
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[16], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(nEncType == TRAN_ENC_071_14) //우체국
		{
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byEncPasswd, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(nEncType == TRAN_ENC_026_03) //신한,조흥 
		{
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byEncPasswd, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(nEncType == TRAN_ENC_023_08) //제일 
		{
			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		if(nEncType == TRAN_ENC_238_15) //대우증권 
		{
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byEncPasswd, 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		if(nEncType == TRAN_ENC_247_09) //우리투자증권 
		{
			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		if(nEncType == TRAN_ENC_240_10) //삼성증권 
		{
			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));						
		}
		else
		if(nEncType == TRAN_ENC_034_02) //광주 (Ms)
		{
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[104], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		{
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
	}
	else
	{
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
// /**********************************************************************************************************/
// /* Function Name : DataField Make - Kim.G.J
// /* 
// /* Date			 : 2016.03.03
// /* 
// /* Input	     : TRCODE
// /*
// /* Desc	         : 통장 M/S Data
// /*
// /* Output	     : T_OK, T_FAIL
// /**********************************************************************************************************/
// 
// int CTranCmn::fnAPP_SetDataField_Item_016X(int nTranCode , int nTxRxOption)
// {
// 	
// 	if (PbTran)
// 	{
// 		DataField *pArrDataField = new DataField;
// 		char szTmp[1024];
// 	
// 		if(m_pProfile->DEVICE.MachineType == U3100K)
// 		{
// 			memset(szTmp, 0x20, sizeof(szTmp));
// 			memcpy(pArrDataField->byFCode, x016X, sizeof(pArrDataField->byFCode));
// 			memcpy(pArrDataField->byFLen, "035", sizeof(pArrDataField->byFLen));
// 			memcpy(szTmp, PbMsData.sBuf, PbMsData.Len);
// 			
// 			if (PbMsData.sBuf[14] == 0x2D)	
// 				szTmp[14] = '=';
// 			else
// 				szTmp[16] = '=';
// 				
// 			memcpy(pArrDataField->byFData, szTmp, 35);
// 			fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
// 		}
// 		else
// 		if(m_pProfile->DEVICE.MachineType == U8100)
// 		{
// 	
// 			if(m_pProfile->DEVICE.ETCDevice20 == KEB) //#0085
// 			{
// 				memset(szTmp, 0x20, sizeof(szTmp));
// 				sprintf(szTmp, "%03d", PbMsData.Len);
// 				memcpy(pArrDataField->byFCode, x016X, sizeof(pArrDataField->byFCode));
// 				memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 				memcpy(pArrDataField->byFData, PbMsData.sBuf, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 				fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
// 			}
// 			else
// 			if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP) //#0085
// 			{		
// 				memset(szTmp, 0x20, sizeof(szTmp));
// 				memcpy(pArrDataField->byFCode, x016X, sizeof(pArrDataField->byFCode));
// 				memcpy(pArrDataField->byFLen, "050", sizeof(pArrDataField->byFLen));
// 				memcpy(szTmp, PbMsData.sBuf, PbMsData.Len);
// 				
// 				for (int i = 0; i < 50; i++)		// 농협用: '+' -> '0' 치환
// 				{
// 					if (szTmp[i] == '+')
// 						szTmp[i] = '0';
// 				}
// 				memcpy(pArrDataField->byFData, szTmp, 50);
// 				fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
// 			}
// 			else
// 			if(m_pProfile->DEVICE.ETCDevice20 == KIUP) //#0126
// 			{		
// 				memset(szTmp, 0x20, sizeof(szTmp));
// 				memcpy(pArrDataField->byFCode, x016X, sizeof(pArrDataField->byFCode));
// 				memcpy(pArrDataField->byFLen, "035", sizeof(pArrDataField->byFLen));
// 				memcpy(szTmp, PbMsData.sBuf, PbMsData.Len);
// 				
// 				if (PbMsData.sBuf[14] == 0x2D)	
// 					szTmp[14] = '=';
// 				else
// 					szTmp[16] = '=';
// 					
// 				memcpy(pArrDataField->byFData, szTmp, 35);
// 				fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
// 			}
// 			else
// 			if(m_pProfile->DEVICE.ETCDevice20 == HANA) //#N0214
// 			{
// 				memset(szTmp, 0x20, sizeof(szTmp));
// 				sprintf(szTmp, "%03d", PbMsData.Len);
// 				memcpy(pArrDataField->byFCode, x016X, sizeof(pArrDataField->byFCode));
// 				memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 				memcpy(pArrDataField->byFData, PbMsData.sBuf, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 				fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
// 			}
// 			else
// 			if(m_pProfile->DEVICE.ETCDevice20 == KB) //#N0227
// 			{
// 				if (TranProc == TRAN_DEP || TranProc == TRAN_PB)
// 				{
// 					memset(szTmp, 0x20, sizeof(szTmp));
// 					sprintf(szTmp, "%03d", PbMsData.Len);
// 					memcpy(pArrDataField->byFCode, x016X, sizeof(pArrDataField->byFCode));
// 					memcpy(pArrDataField->byFLen, "044", sizeof(pArrDataField->byFLen));
// 					memcpy(pArrDataField->byFData, m_STXEncBuff.byChiperMsdata, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 					fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
// 				}
// 				else
// 				{
// 					memset(szTmp, 0x20, sizeof(szTmp));
// 					sprintf(szTmp, "%03d", PbMsData.Len);
// 					memcpy(pArrDataField->byFCode, x0161, sizeof(pArrDataField->byFCode));
// 					memcpy(pArrDataField->byFLen, "168", sizeof(pArrDataField->byFLen));
// 					memcpy(pArrDataField->byFData, m_STXEncBuff.byChiperMsdata, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 					fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
// 				}
// 			}
// 			else
// 			if(m_pProfile->DEVICE.ETCDevice20 == CITI) //#N0252
// 			{
// 				memset(szTmp, 0x20, sizeof(szTmp));
// 				sprintf(szTmp, "%03d", PbMsData.Len);
// 				memcpy(pArrDataField->byFCode, x016X, sizeof(pArrDataField->byFCode));
// 				memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 				memcpy(pArrDataField->byFData, PbMsData.sBuf, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 				fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
// 			}
// 			else
// 			if(m_pProfile->DEVICE.ETCDevice20 == SC) //#N0245
// 			{
// 				memset(szTmp, 0x20, sizeof(szTmp));
// 				sprintf(szTmp, "%03d", PbMsData.Len);
// 				memcpy(pArrDataField->byFCode, x016X, sizeof(pArrDataField->byFCode));
// 				memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 				memcpy(pArrDataField->byFData, PbMsData.sBuf, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 				fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
// 			}
// 			else	
// 			if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0277
// 			{
// 				memset(szTmp, 0x20, sizeof(szTmp));
// 				sprintf(szTmp, "%03d", PbMsData.Len);
// 				memcpy(pArrDataField->byFCode, x016X, sizeof(pArrDataField->byFCode));
// 				memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 				memcpy(pArrDataField->byFData, PbMsData.sBuf, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 				fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
// 			}
// 			else	
// 			{
// 				delete pArrDataField;			
// 			}
// 
// 		}
// 		else
// 		{
// 			memset(szTmp, 0x20, sizeof(szTmp));
// 			memcpy(pArrDataField->byFCode, x016X, sizeof(pArrDataField->byFCode));
// 			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
// 			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 			fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
// 		}
// 
// 
// 		if(m_pProfile->DEVICE.ETCDevice20 == KEB) //#0085
// 		{
// 			DataField *pArrDataField1 = new DataField;
// 			memset(szTmp, 0x20, sizeof(szTmp));
// 
// 			memcpy(pArrDataField1->byFCode, x226X, sizeof(pArrDataField1->byFCode));
// 			memcpy(pArrDataField1->byFLen, "001", sizeof(pArrDataField1->byFLen));
// 			
// 			if (TranCode == TC_PBOOKTRM)
// 				memcpy(pArrDataField1->byFData, "2", Asc2Int((char *)pArrDataField1->byFLen, sizeof(pArrDataField1->byFLen)));	
// 			else 
// 			if (TranCode == TC_PBOOK)
// 				memcpy(pArrDataField1->byFData, "1", Asc2Int((char *)pArrDataField1->byFLen, sizeof(pArrDataField1->byFLen)));	
// 			else
// 				memcpy(pArrDataField1->byFData, "0", Asc2Int((char *)pArrDataField1->byFLen, sizeof(pArrDataField1->byFLen)));	
// 
// 			fnAPP_SetAddArrDataField(pArrDataField1 , nTxRxOption);			
// 
// 		}
// 	}
// 
// 	return T_OK;	
// }

int CTranCmn::fnAPP_SetDataField_Item_017X(int nTranCode , int nTxRxOption)
{

	if(MenuAvail & MENU_MCU3)
	{
		DataField *pArrDataField = new DataField;
		char szTmp[1024];
		memset(szTmp, 0x20, sizeof(szTmp));


		memcpy(pArrDataField->byFCode, x017X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));

		memcpy(szTmp, &pCardData3->AccountNo, 16);	
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	}
	if(MenuAvail & MENU_MCU2)
	{
		DataField *pArrDataField = new DataField;
		char szTmp[1024];
		memset(szTmp, 0x20, sizeof(szTmp));


		memcpy(pArrDataField->byFCode, x017X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));

		memcpy(szTmp, CardData.ISO2Buff, 16);	
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	}
	else
	if(MenuAvail & MENU_PB)
	{
		DataField *pArrDataField = new DataField;
		char szTmp[1024];
		memset(szTmp, 0x20, sizeof(szTmp));
		memcpy(pArrDataField->byFCode, x017X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));

		if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
		{
			if (PbMsData.sBuf[14] == 0x2D)		
				sprintf(szTmp,"%14.14s   ", Accept.AccountNum);
			else
				sprintf(szTmp,"%16.16s ", Accept.AccountNum);
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == KEB)
		{
			sprintf(szTmp,"%13.13s    ", Accept.AccountNum); //6+13+4
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
		{

			if (memcmp(&PbMsData.sBuf[42], "1", 1) == 0) 				// 중앙통장 : 13계좌 : 5-2-6
			{
				sprintf(szTmp,"%13.13s    ", &PbMsData.sBuf[1]);  //6+13+4
			}
			else
			if (memcmp(&PbMsData.sBuf[41], "1", 1) == 0) 				// 조합통장 : 14계좌 : 6-2-6		
			{
				sprintf(szTmp,"%14.14s   ", PbMsData.sBuf);       //6+14+3
			}
			else
			if (memcmp(&PbMsData.sBuf[43], "1", 1) == 0) 				// 조합통장 : 15계좌 : 6-3-6		
			{
				memcpy(Accept.AccountNum, &PbMsData.sBuf[0], 15);
				if (memcmp(&PbMsData.sBuf[21], NONBANKID1, 2) == 0 || memcmp(&PbMsData.sBuf[21], NONBANKID3, 2) == 0)
				{
					sprintf(szTmp,"%15.15s  ", PbMsData.sBuf);       //6+15+2
				}
				else
				{
					sprintf(szTmp,"%15.15s  ", PbMsData.sBuf);       //6+15+2
				}
			}
			else
			if (memcmp(&PbMsData.sBuf[48], "1", 1) == 0 && memcmp(&PbMsData.sBuf[41], "000", 3) ==0)				// 13계좌 - 신계좌번호 체계 통장 TYPE(3-4-4-2)		
			{
				if ((memcmp(&PbMsData.sBuf[12], "1",1) == 0)	|| 
					(memcmp(&PbMsData.sBuf[12], "2",1) == 0)	)
					sprintf(szTmp,"%13.13s    ", PbMsData.sBuf); //6+13+4
				else
					sprintf(szTmp,"%13.13s    ", PbMsData.sBuf); 

			}
			else
				sprintf(szTmp,"%13.13s    ", &PbMsData.sBuf[1]);  //6+13+4

		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU)   
		{
			sprintf(szTmp,"%12.12s     ", Accept.AccountNum); //6+13+4  
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == HANA)   
		{
			sprintf(szTmp,"%14.14s     ", Accept.AccountNum); //6+13+4  
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == KB)   
		{
			sprintf(szTmp,"%16.16s   ", Accept.AccountNum); 
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == CITI)   
		{
			sprintf(szTmp,"%16.16s   ", Accept.AccountNum); 
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == SC)   
		{
			sprintf(szTmp,"%16.16s   ", Accept.AccountNum); 
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)   
		{
			if (memcmp(&PbMsData.sBuf[2], "1", 1) == 0)
			{
				sprintf(szTmp,"1300%13.13s", &PbMsData.sBuf[5]);
			}
			else
			if (memcmp(&PbMsData.sBuf[2], "2", 1) == 0)
			{
				sprintf(szTmp,"1200%12.12s0", &PbMsData.sBuf[5]);
			}
			else
			if (memcmp(&PbMsData.sBuf[2], "+", 1) == 0)
			{
				sprintf(szTmp,"1200%12.12s0", &PbMsData.sBuf[5]);
			}
			else
			{
				sprintf(szTmp,"1200%12.12s0", &PbMsData.sBuf[5]);
			}
		}

		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	}
	return T_OK;	
}


int CTranCmn::fnAPP_SetDataField_Item_052X(int nTranCode , int nTxRxOption)
{
	
	if(MenuAvail & MENU_MCU3)
	{


		DataField *pArrDataField = new DataField;
		char szTmp[1024];
		memset(szTmp, 0x20, sizeof(szTmp));

		memcpy(pArrDataField->byFCode, x0521, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));

		if (TranEncCode == nEnCmd218_218)
		{
			memcpy(pArrDataField->byFCode, x0520, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &pCardData3->AccountNo, 16);	
		}
		else
		if (TranEncCode == nEnCmd025_81)
		{
			memcpy(pArrDataField->byFCode, x0521, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, m_STXEncBuff.byChiperMsdata, 16);	
		}
		else
		{
			memcpy(pArrDataField->byFCode, x0521, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &pCardData3->AccountNo, 16);	
		}

		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	}
	else
	if(MenuAvail & MENU_MCU2)
	{
		DataField *pArrDataField = new DataField;
		char szTmp[1024];
		memset(szTmp, 0x20, sizeof(szTmp));
		memcpy(pArrDataField->byFCode, x0521, sizeof(pArrDataField->byFCode));

		if(CardServiceFlag == AMEXCARD)
			memcpy(pArrDataField->byFLen, "015", sizeof(pArrDataField->byFLen));
		else
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));

		memcpy(szTmp, CardData.ISO2Buff, CardData.ISO2size);
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	}
	else
	if(MenuAvail & MENU_PB)
	{
		DataField *pArrDataField = new DataField;
		char szTmp[1024];
		memset(szTmp, 0x20, sizeof(szTmp));
		memcpy(pArrDataField->byFCode, x0520, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));

		if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
		{
			if (PbMsData.sBuf[14] == 0x2D)		
				sprintf(szTmp,"%14.14s   ", Accept.AccountNum);
			else
				sprintf(szTmp,"%16.16s ", Accept.AccountNum);
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == KEB)
		{
			sprintf(szTmp,"%13.13s    ", Accept.AccountNum); //6+13+4
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
		{

			if (memcmp(&PbMsData.sBuf[42], "1", 1) == 0) 				// 중앙통장 : 13계좌 : 5-2-6
			{
				sprintf(szTmp,"%13.13s    ", &PbMsData.sBuf[1]);  //6+13+4
			}
			else
			if (memcmp(&PbMsData.sBuf[41], "1", 1) == 0) 				// 조합통장 : 14계좌 : 6-2-6		
			{
				sprintf(szTmp,"%14.14s   ", PbMsData.sBuf);       //6+14+3
			}
			else
			if (memcmp(&PbMsData.sBuf[43], "1", 1) == 0) 				// 조합통장 : 15계좌 : 6-3-6		
			{
				memcpy(Accept.AccountNum, &PbMsData.sBuf[0], 15);
				if (memcmp(&PbMsData.sBuf[21], NONBANKID1, 2) == 0 || memcmp(&PbMsData.sBuf[21], NONBANKID3, 2) == 0)
				{
					sprintf(szTmp,"%15.15s  ", PbMsData.sBuf);       //6+15+2
				}
				else
				{
					sprintf(szTmp,"%15.15s  ", PbMsData.sBuf);       //6+15+2
				}
			}
			else
			if (memcmp(&PbMsData.sBuf[48], "1", 1) == 0 && memcmp(&PbMsData.sBuf[41], "000", 3) ==0)				// 13계좌 - 신계좌번호 체계 통장 TYPE(3-4-4-2)		
			{
				if ((memcmp(&PbMsData.sBuf[12], "1",1) == 0)	|| 
					(memcmp(&PbMsData.sBuf[12], "2",1) == 0)	)
					sprintf(szTmp,"%13.13s    ", PbMsData.sBuf); //6+13+4
				else
					sprintf(szTmp,"%13.13s    ", PbMsData.sBuf); 

			}
			else
				sprintf(szTmp,"%13.13s    ", &PbMsData.sBuf[1]);  //6+13+4

		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU)   
		{
			sprintf(szTmp,"%12.12s     ", Accept.AccountNum); //6+13+4  
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == HANA)   
		{
			sprintf(szTmp,"%14.14s     ", Accept.AccountNum); //6+13+4  
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == KB)   
		{
			sprintf(szTmp,"%16.16s   ", Accept.AccountNum); 
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == CITI)   
		{
			sprintf(szTmp,"%16.16s   ", Accept.AccountNum); 
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == SC)   
		{
			sprintf(szTmp,"%16.16s   ", Accept.AccountNum); 
		}
		else
		if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)   
		{
			if (memcmp(&PbMsData.sBuf[2], "1", 1) == 0)
			{
				sprintf(szTmp,"1300%13.13s", &PbMsData.sBuf[5]);
			}
			else
			if (memcmp(&PbMsData.sBuf[2], "2", 1) == 0)
			{
				sprintf(szTmp,"1200%12.12s0", &PbMsData.sBuf[5]);
			}
			else
			if (memcmp(&PbMsData.sBuf[2], "+", 1) == 0)
			{
				sprintf(szTmp,"1200%12.12s0", &PbMsData.sBuf[5]);
			}
			else
			{
				sprintf(szTmp,"1200%12.12s0", &PbMsData.sBuf[5]);
			}
		}

		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	}
	return T_OK;	
}

//006:국민 003,013,014:기업,농협 247:NH투자 269:한화증권 188:신한카드론X 279:동부증권 266:에스케이X 218:현대증권
int CTranCmn::fnAPP_SetDataField_Item_054X(int nTranCode , int nTxRxOption)
{	
	
	//01.TranEncCode == nEnCmd218_218 -> 올림
	if(TranEncCode == nEnCmd004_6 || TranEncCode == nEnCmd003_3 || TranEncCode == nEnCmd013_12 || TranEncCode == nEnCmd247_247 || TranEncCode == nEnCmd269_269 || TranEncCode == nEnCmd279_279 || TranEncCode == nEnCmd218_218 || TranEncCode == nEnCmd240_364) 
	{
		int nLen = 0;
		char szTmp[1024];
		DataField *pArrDataField = new DataField;

		memcpy(pArrDataField->byFCode, x054X, sizeof(pArrDataField->byFCode));
		sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
		memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
		memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
		memcpy(pArrDataField->byFData, m_STXEncBuff.byChiperMsdata, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));	
		
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	}

	return T_OK;

}

//출금매체구분 0: 무매체       1: 만원권 ONLY  2: 수표 ONLY   3: 만원권 + 수표 -> // 출금계좌성명	
int CTranCmn::fnAPP_SetDataField_Item_018X(int nTranCode , int nTxRxOption)
{
	int nLen = 0;
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x018X, sizeof(pArrDataField->byFCode));
	
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));


// 	sprintf(m_RD.byDataField_018x, "%03d", strlen(m_RD.byDataField_018x));	
// 	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 	memcpy(pArrDataField->byFData, m_RD.byDataField_018x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}

// int CTranCmn::fnAPP_SetDataField_Item_018X(int nTranCode , int nTxRxOption)
// {
// 	int nLen = 0;
// 	char szTmp[1024];
// 	DataField *pArrDataField = new DataField;
// 	memcpy(pArrDataField->byFCode, x018X, sizeof(pArrDataField->byFCode));
// 	
// 	//#0101 -> N0201
// 	if ((m_pProfile->DEVICE.ETCDevice20 == KIUP) && (MenuAvail & MENU_MU) && (TranCode2 == TRANID_4716 || TranCode2 == TRANID_7200))
// 	{
// 		memcpy(pArrDataField->byFLen, "019", sizeof(pArrDataField->byFLen));
// 		memcpy(pArrDataField->byFData, Accept.RemitName, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 	}
// 	else
// 	if ((m_pProfile->DEVICE.ETCDevice20 == HANA) && (MenuAvail & MENU_MU))
// 	{
// 		memcpy(pArrDataField->byFLen, "019", sizeof(pArrDataField->byFLen));
// 		memcpy(pArrDataField->byFData, &Accept.RemitName[1], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 	}
// 	else
// 	if ((m_pProfile->DEVICE.ETCDevice20 == NONGHYUP) && (TranProc == TRAN_LOCALTAX))
// 	{
// 		nLen = FindChar(m_RD.byDataField_021x, sizeof(m_RD.byDataField_021x), NULL);
// 		memset(szTmp, 0x00, sizeof(szTmp));
// 		Int2Asc(nLen, szTmp, 3);
// 		memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 		memcpy(pArrDataField->byFData, m_RD.byDataField_021x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 	}
// 	else
// 	{
// 		memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
// 		memcpy(pArrDataField->byFData, ZERO4, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 	}
// 
// 	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
// 	return T_OK;	
// }
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 수취기관코드
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_019X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x019X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, Accept.BankNum, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;	
}
//ø 국민은행 통장정리/통장입금거래 시 SoftForum 모듈 사용하여 암호화된 데이터를 길이만큼 송/수신 처리한다.
int CTranCmn::fnAPP_SetDataField_Item_0191(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, "0191", sizeof(pArrDataField->byFCode));

	if(MenuAvail & MENU_MCU3)
	{
		int nEncType = TranEncCode;
		if(nEncType == TRAN_ENC_034_02) //광주 (Ms)
		{
			memcpy(pArrDataField->byFLen, "104", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, 104);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		{
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
	}
	else
		delete pArrDataField;		
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 수취계좌번호
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_020X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szTmp1[1024];
	memset(szTmp, 0x20, sizeof(szTmp));
	memset(szTmp1, 0x20, sizeof(szTmp));

	MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_020X TranProc[%d] TranCode[%d]" , nTranCode, TranCode);
	if(TranProc == TRAN_TRANS && TranCode == TC_TRANSFER)	
	{
		memcpy(szTmp, Accept.InAccountNum, Accept.InAccountNumSize);

		memcpy(pArrDataField->byFCode, x020X, sizeof(pArrDataField->byFCode));
		Int2Asc(Accept.InAccountNumSize, szTmp1, sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFLen, szTmp1, sizeof(pArrDataField->byFLen));	

// 		if(memcmp(Accept.BankNum, "053", sizeof(Accept.BankNum)) == 0)
// 		{
// 			memcpy(Accept.BankNum, "027", sizeof(Accept.BankNum));
// 		}
// 		else
// 		if(memcmp(Accept.BankNum, "208", sizeof(Accept.BankNum)) == 0)
// 		{
// 			memcpy(Accept.BankNum, "9HD", sizeof(Accept.BankNum));
// 		}
// 		else //#0124
// 		if(memcmp(Accept.BankNum, "011", sizeof(Accept.BankNum)) == 0 || 
// 		   memcmp(Accept.BankNum, "012", sizeof(Accept.BankNum)) == 0 ||
// 		   memcmp(Accept.BankNum, "016", sizeof(Accept.BankNum)) == 0 ||
// 		   memcmp(Accept.BankNum, "017", sizeof(Accept.BankNum)) == 0)
// 		{
// 			//Default 
// 			memcpy(Accept.BankNum, "012", sizeof(Accept.BankNum));
// 
// 			//#0124
// 			if(Accept.InAccountNumSize == 10)
// 			{
// 				if(szTmp1[9] == '8')
// 				{
// 					memcpy(Accept.BankNum, "011", sizeof(Accept.BankNum));
// 				}
// 			}
// 			else
// 			if(Accept.InAccountNumSize == 11 || Accept.InAccountNumSize == 12)
// 			{
// 				memcpy(Accept.BankNum, "011", sizeof(Accept.BankNum));
// 			}
// 			else
// 			if(Accept.InAccountNumSize == 13)
// 			{
// 				//#0046 - 1,2,6,8
// 				if(szTmp1[12] == '1' || szTmp1[12] == '2' || szTmp1[12] == '6' || szTmp1[12] == '8')
// 				{
// 					memcpy(Accept.BankNum, "011", sizeof(Accept.BankNum));
// 				}
// 				else
// 				if(szTmp1[12] == '3' || szTmp1[12] == '4' || szTmp1[12] == '5' || szTmp1[12] == '7' || szTmp1[12] == '9')
// 				{
// 					memcpy(Accept.BankNum, "012", sizeof(Accept.BankNum));
// 				}
// 			}
// 			else
// 			if(Accept.InAccountNumSize == 14)
// 			{
// 				//#0046 - "790", "791"
// 				if(memcmp(&szTmp1[6], "64", 2) == 0 || memcmp(&szTmp1[6], "65", 2) == 0 || memcmp(&szTmp1[0], "790", 3) == 0 || memcmp(&szTmp1[0], "791", 3) == 0)
// 				{
// 					memcpy(Accept.BankNum, "011", sizeof(Accept.BankNum));
// 				}
// 				else
// 				{
// 					memcpy(Accept.BankNum, "012", sizeof(Accept.BankNum));
// 				}
// 
// 			}
//		}
	
	}
	else
	if(TranCode == TC_SVCTRAN)  
	{
		MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_020X TranProc[%d] TranCode[%d]" , nTranCode, TranCode);
		memcpy(szTmp1, Accept.InAccountNum, Accept.InAccountNumSize);

		memcpy(pArrDataField->byFCode, x020X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));		
		sprintf(szTmp, "%16.16s", szTmp1);
	}
	else
	{
		memcpy(pArrDataField->byFCode, x020X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));				
	}

	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 수취계좌성명
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_021X(int nTranCode , int nTxRxOption)
{
	char szTmp[1024];

	if(TranProc == TRAN_TRANS)	
	{
		if(TranCode2 == TRANID_4390 || TranCode2 == TRANID_4370 || TranCode2 == TRANID_4396 || TranCode2 == TRANID_4215)
		{
			DataField *pArrDataField = new DataField;
			memcpy(pArrDataField->byFCode, x021X, sizeof(pArrDataField->byFCode));
			sprintf(szTmp, "%03d", strlen(m_RD.byDataField_021x));
			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
			memcpy(pArrDataField->byFData, m_RD.byDataField_021x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
		}
		else
		{
			DataField *pArrDataField = new DataField;
			memcpy(pArrDataField->byFCode, x021X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "010", sizeof(pArrDataField->byFLen));
			memcpy(pArrDataField->byFData, "0000000000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
		}
		
	}
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 거래계좌(카드)잔액 - 음수일 경우 최상위값 = ‘-‘
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_022X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	
	if(m_pProfile->DEVICE.ETCDevice20 == HANA && CardBank == HANBANK && (TranCode2 == TRANID_4390 || TranCode2 == TRANID_4216|| TranCode2 == TRANID_4370|| TranCode2 == TRANID_4396))
	{
		char szTmp[1024];
		memset(szTmp, 0x30, sizeof(szTmp));
		memcpy(pArrDataField->byFCode, x022X, sizeof(pArrDataField->byFCode));
		Int2Asc(strlen(m_RD.byDataField_022x), szTmp, 3);
		memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_RD.byDataField_022x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if(TranEncCode == TRAN_ENC_006_01)
	{
		char szTmp[1024];
		memset(szTmp, 0x20, sizeof(szTmp));
		memcpy(pArrDataField->byFCode, x0221, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, &m_STXEncBuff.byChiperMsdata[104], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		char szTmp[1024];
		memset(szTmp, 0x30, sizeof(szTmp));
		memcpy(pArrDataField->byFCode, x022X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "010", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, "0000000000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
//ø 국민은행 개별망 거래인 경우 송신 시 필수. (소프트포럼 암호화 사용) 0221 필드를 사용한다
int CTranCmn::fnAPP_SetDataField_Item_0221(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x021X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "010", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "0000000000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 거래금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_023X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];


	sprintf(szTmp, "%d", Asc2Int(Accept.Money, sizeof(Accept.Money)));
	memcpy(pArrDataField->byFCode, x023X, sizeof(pArrDataField->byFCode));
	Int2Asc(strlen(szTmp), pArrDataField->byFLen, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	


	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 보너스 매체 출금가능 정보 (038X)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_038X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;

	memcpy(pArrDataField->byFCode, x038X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 거래수수료
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_024X(int nTranCode , int nTxRxOption)
{
	int nLen = 0;
	char szTmp[1024];
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x024X, sizeof(pArrDataField->byFCode));
	
	nLen = FindChar(m_RD.byDataField_024x, sizeof(m_RD.byDataField_024x), NULL);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_024X nLen[%d]", nLen);
	
	//ECash - 예외처리 (수수료 안내려 올 경우) 
	if(nLen < 1)
	{
		memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
		memset(pArrDataField->byFData, '0', Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		memset(szTmp, 0x00, sizeof(szTmp));
		Int2Asc(nLen, szTmp, 3);
		memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_RD.byDataField_024x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 미결제타점권 금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_025X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));

	memcpy(pArrDataField->byFCode, x025X, sizeof(pArrDataField->byFCode));
	if(m_pProfile->DEVICE.ETCDevice20 == HANA && CardBank == HANBANK && (TranCode2 == TRANID_4390 || TranCode2 == TRANID_4216|| TranCode2 == TRANID_4370|| TranCode2 == TRANID_4396))
	{
		Int2Asc(strlen(m_RD.byDataField_025x), szTmp, 3);
		memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_RD.byDataField_025x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		//memcpy(pArrDataField->byFCode, x025X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, "000000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_026X(int nTranCode , int nTxRxOption)  //#N0174
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x0262, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "395", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, m_RXEASYONEDATA.Gubun, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}


int CTranCmn::fnAPP_SetDataField_Item_027X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x027X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, m_RD.byDataField_027x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_028X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x028X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, &m_RD.byDataField_027x[2], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_029X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x029X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, &m_RD.byDataField_027x[4], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 암호화Key길이 (삼성증권암호화)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_041X(int nTranCode , int nTxRxOption)
{

	if(CardBank == SST_ORG_INT_240)
	{
		int nRet = 0;
		char szKeyLen[1024];
		DataField *pArrDataField = new DataField;
		memcpy(pArrDataField->byFCode, x041X, sizeof(pArrDataField->byFCode));
		sprintf(szKeyLen, "%03d", Asc2Int(m_TXENCKEY.k_len, sizeof(m_TXENCKEY.k_len)));
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
		nRet = Asc2Int(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,2);

		if(nRet == CIPHERTEXT_TMP)
			memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		else
			memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	}
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 암호화Key길이
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
// int CTranCmn::fnAPP_SetDataField_Item_042X(int nTranCode , int nTxRxOption)
// {
// 	if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU)
// 		return T_OK;
// 
// 	char szKeyLen[1024];
// 	DataField *pArrDataField = new DataField;
// 	memcpy(pArrDataField->byFCode, x042X, sizeof(pArrDataField->byFCode));
// 	sprintf(szKeyLen, "%03d", Asc2Int(m_TXENCKEY.k_len, sizeof(m_TXENCKEY.k_len)));
// 	memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));
// 	memcpy(pArrDataField->byFData, szKeyLen, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
// 	return T_OK;	
// }

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 일반전문(출금가능총액), 시재전문(기기보유 현금액)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_042X(int nTranCode , int nTxRxOption)
{
	char szAmt[1024];
	int nCurRB2Cnt = 0;
	int nCurRB1Cnt = 0;

	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x042X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "009", sizeof(pArrDataField->byFLen));

	nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원
	nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
	nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
	nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

	memset(szAmt, 0x00, sizeof(szAmt));
	if (nTranCode == TRANID_A060)
	{
		sprintf(szAmt, "%09d", (nCurRB1Cnt * CASH_KOREA_50000) + (nCurRB2Cnt * CASH_KOREA_10000));
	}
	else
	{
		sprintf(szAmt, "%09d", (nCurRB1Cnt * CASH_KOREA_50000) + (nCurRB2Cnt * CASH_KOREA_10000));
	}

	memcpy(pArrDataField->byFData, szAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 주암호화Key
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_043X(int nTranCode , int nTxRxOption)
{
	if ((CardBank == SST_ORG_INT_240) && (Asc2Int(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,2) != CIPHERTEXT_TMP))
	{
		return T_OK;
	}

	char szKeyLen[1024];
	char szKeyData[1024];
	memset(szKeyData, 0x00, sizeof(szKeyData));
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x043X, sizeof(pArrDataField->byFCode));	
	switch(CardBank)
	{
	case KBS_ORG_INT_004: //기업, 국민, 농협, 제일 (2개)
	case KBS_ORG_INT_006:
	case KBS_ORG_INT_029:
	case KBS_ORG_INT_030:
// 	case NON_ORG_INT_011:
// 	case NON_ORG_INT_012:
// 	case NON_ORG_INT_016:
// 	case NON_ORG_INT_017:
// 	case SCO_ORG_INT_023:
//	case KJB_ORG_INT_034:
		sprintf(szKeyLen, "%03d", Asc2Int(m_TXENCKEY.k_len, sizeof(m_TXENCKEY.k_len)));
		memcpy(pArrDataField->byFLen, szKeyLen, sizeof(pArrDataField->byFLen));
		memcpy(szKeyData, m_TXENCKEY.k_data, Asc2Int(m_TXENCKEY.k_len, sizeof(m_TXENCKEY.k_len)));
		memcpy(pArrDataField->byFData, szKeyData, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	case WRT_ORG_INT_247:	
	case HWT_ORG_INT_269:
	case DOB_ORG_INT_279:
	case SST_ORG_INT_240:
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, SPACE16, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	default:
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, SPACE16, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	}
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 보조암호화Key
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_044X(int nTranCode , int nTxRxOption)
{

	if ((CardBank == SST_ORG_INT_240) && (Asc2Int(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,2) != CIPHERTEXT_TMP))
	{
		return T_OK;
	}

	char szKeyLen[1024];
	char szKeyData[1024];
	memset(szKeyData, 0x00, sizeof(szKeyData));
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x044X, sizeof(pArrDataField->byFCode));

	switch(CardBank)
	{
	case KBS_ORG_INT_004:
	case KBS_ORG_INT_006:
	case KBS_ORG_INT_029:
	case KBS_ORG_INT_030:
		memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, SPACE2, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	case NON_ORG_INT_011:
	case NON_ORG_INT_012:
	case NON_ORG_INT_016:
	case NON_ORG_INT_017:
	case SCO_ORG_INT_023:
	case KJB_ORG_INT_034:
	case WRT_ORG_INT_247:	
	case HWT_ORG_INT_269:
	case DOB_ORG_INT_279:
	case SST_ORG_INT_240:
//	case nEnCmd266_266:
	case KUB_ORG_INT_003:
	case KEB_ORG_INT_005:

		sprintf(szKeyLen, "%03d", Asc2Int(m_TXENCKEY.k_len, sizeof(m_TXENCKEY.k_len)));
		memcpy(pArrDataField->byFLen, szKeyLen, sizeof(pArrDataField->byFLen));
		memcpy(szKeyData, m_TXENCKEY.k_data, Asc2Int(m_TXENCKEY.k_len, sizeof(m_TXENCKEY.k_len)));
		memcpy(pArrDataField->byFData, szKeyData, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	default:
		if(nTranCode == TRANID_A010 || nTranCode == TRANID_A020 || nTranCode == TRANID_A060)
		{
			memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
			memcpy(pArrDataField->byFData, SPACE16, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
		{
			memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
			memcpy(pArrDataField->byFData, SPACE2, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		break;
	}
	
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_0441(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, "0441", sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "0000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 최종승인허가일자 - 최종 거래 승인 일자
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_045X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x045X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "008", sizeof(pArrDataField->byFLen));
//	memcpy(pArrDataField->byFData, GetDate().GetBuffer(0), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	memcpy(pArrDataField->byFData, SPACE8, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : IC카드 일련번호 (057X)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_057X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	if(!(MenuFICMode & MENU_IC) && !(MenuAvail & MENU_MCU3))
	{
		memcpy(pArrDataField->byFCode, x0571, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, ZERO16, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		memcpy(pArrDataField->byFCode, x0571, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_pDevCmn->FinanceICInfo.szCSN, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : IC카드 암호화 비밀번호 (058X)

/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_058X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	if(!(MenuFICMode & MENU_IC) && !(MenuAvail & MENU_MCU3))
	{
		memcpy(pArrDataField->byFCode, x0581, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "098", sizeof(pArrDataField->byFLen));
		memset(m_pDevCmn->FinanceICInfo.HostTranData, ' ', sizeof(m_pDevCmn->FinanceICInfo.HostTranData));
		memcpy(pArrDataField->byFData, m_pDevCmn->FinanceICInfo.HostTranData, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	}
	else
	{
		memcpy(pArrDataField->byFCode, x0581, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "098", sizeof(pArrDataField->byFLen));
		//공동망IC 98 - 100 Byte 
		m_pDevCmn->fnFIC_GetHostTranData(FICMyBiFlg);
		memcpy(pArrDataField->byFData, m_pDevCmn->FinanceICInfo.HostTranData, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	}
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 거래개시일시
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_063X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x063X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "010", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, Accept.CardReadTime, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : HOST일련번호
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_064X(int nTranCode , int nTxRxOption)
{
	char szTmp[1024];
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x064X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "012", sizeof(pArrDataField->byFLen));
	if(nTranCode == TRANID_A010 || nTranCode == TRANID_A020 || nTranCode == TRANID_A060 || nTranCode == TRANID_2190)
	{
		if(EMVAuthError && nTranCode == TRANID_A020)
		{
			sprintf(szTmp, "%12.12s", m_RD.byDataField_026x );
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
			memcpy(pArrDataField->byFData, ZERO12, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if(nTranCode == TRANID_4320 || nTranCode == TRANID_4340 || nTranCode == TRANID_4342  //#N0171 
		|| nTranCode == TRANID_4321 || nTranCode == TRANID_4322 || nTranCode == TRANID_4341  
		|| nTranCode == TRANID_7233 || nTranCode == TRANID_7235 || nTranCode == TRANID_7237) //#N0235
	{
//		m_pProfile->GetTransProfile();
//		memcpy(pArrDataField->byFData, m_pProfile->TRANS.HostSerialNo, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		memcpy(pArrDataField->byFData, ZERO12, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else 
	if(nTranCode == TRANID_7320 || nTranCode == TRANID_7321 || nTranCode == TRANID_7323)
	{
		memcpy(pArrDataField->byFData, SPACE12, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else 
	if(nTranCode == TRANID_4811 || nTranCode == TRANID_4812 || nTranCode == TRANID_4814 || nTranCode == TRANID_4810 || nTranCode == TRANID_4813)
	{
		sprintf(szTmp, "%12.12s", m_RD.byDataField_026x );
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if(nTranCode == TRANID_4670 || nTranCode == TRANID_4671 || nTranCode == TRANID_4850)
	{
		sprintf(szTmp, "%12.12s", m_RD.byDataField_026x );
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		memcpy(pArrDataField->byFData, ZERO12, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 디바이스별 모델 정보
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_065X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x065X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "014", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "HHHHHH0000000H", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 수표등록시간
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_066X(int nTranCode , int nTxRxOption)
{
	return T_OK;

	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x066X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "014", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, ZERO14, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 해외카드 구분 
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_067X(int nTranCode , int nTxRxOption)		
{
	DataField *pArrDataField = new DataField;
	if(memcmp(Accept.MuConfirmNum, "E1", 2) == 0)
	{
		memcpy(pArrDataField->byFCode, x067X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if(memcmp(Accept.MuConfirmNum, "E2", 2) == 0)
	{
		memcpy(pArrDataField->byFCode, x067X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	}
	else
	if(memcmp(Accept.MuConfirmNum, "E3", 2) == 0)
	{
		memcpy(pArrDataField->byFCode, x067X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, "3", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		memcpy(pArrDataField->byFCode, x067X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	}
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_068X(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;	
		memcpy(pArrDataField->byFCode, x068X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "008", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_pDevCmn->RFHipassLoad1.CardTranSeq, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_0691(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;
		memcpy(pArrDataField->byFCode, x0691, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "008", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_pDevCmn->RFHipassLoad1.CardBalanceHEX, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_070X(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;	
		memcpy(pArrDataField->byFCode, x070X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_pDevCmn->RFHipassLoad1.RandomNum, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}



int CTranCmn::fnAPP_SetDataField_Item_073X(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;	
		memcpy(pArrDataField->byFCode, x073X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_pDevCmn->RFHipassLoad1.AlgorithmID, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_074X(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;	
		memcpy(pArrDataField->byFCode, x074X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_pDevCmn->RFHipassLoad1.KeyVersion, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_075X(int nTranCode , int nTxRxOption)
{
	char szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;	
		memcpy(pArrDataField->byFCode, x075X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
		sprintf(szTmp,"%2.2s",m_pDevCmn->RFHipassLoad1.UniqueNum);
		memcpy(pArrDataField->byFData,szTmp , Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_076X(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;	
		memcpy(pArrDataField->byFCode, x076X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_pDevCmn->RFHipassLoad1.PublisherID, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_077X(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;	
		memcpy(pArrDataField->byFCode, x077X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "010", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_pDevCmn->RFHipassLoad1.CardID, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_078X(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;	
		memcpy(pArrDataField->byFCode, x078X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "020", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_pDevCmn->RFHipassLoad1.LedgerNum, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_079X(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;	
		memcpy(pArrDataField->byFCode, x079X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_pDevCmn->RFHipassLoad1.CardServiceID, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_080X(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;	
		memcpy(pArrDataField->byFCode, x080X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, &CardData.ISO2Buff, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_085X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}

//상성증권, 기업만 사용  
int CTranCmn::fnAPP_SetDataField_Item_086X(int nTranCode , int nTxRxOption)
{
	char szKeyData[1024];
	memset(szKeyData, 0x00, sizeof(szKeyData));
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x086X, sizeof(pArrDataField->byFCode));
	
	switch(CardBank)
	{

	case KUB_ORG_INT_003:
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));

		if (nTranCode == TRANID_2190)
			memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		else
			memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		
		break;
	case SST_ORG_INT_240:
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));

		if (Asc2Int(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,2) != CIPHERTEXT_TMP)
			memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		else
			memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		break;
	case NON_ORG_INT_011:
	case NON_ORG_INT_012:
	case NON_ORG_INT_016:
	case NON_ORG_INT_017:
	case KBS_ORG_INT_004:
	case KBS_ORG_INT_006:
	case KBS_ORG_INT_029:
	case KBS_ORG_INT_030:
	case SCO_ORG_INT_023:
	case KJB_ORG_INT_034:
	case HWT_ORG_INT_269:
	case WRT_ORG_INT_247:
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	default:
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		break;
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 장애세부코드 - [CTR(1)+LC(2)+MTC(5)+Rev(2)] X 5개 = 50 
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_087X(int nTranCode , int nTxRxOption)
{
	int	 nLen = 0;
	char szSetHWErrInfo[1024];
	CString cstrLC("00");
	CString cstrLC1("");
	
	if(m_pDevCmn->fnAPL_CheckError())
		return T_OK;

	DataField *pArrDataField = new DataField;
	memset(szSetHWErrInfo, 0x20, sizeof(szSetHWErrInfo));
	memcpy(pArrDataField->byFCode, x087X, sizeof(pArrDataField->byFCode));

	memcpy(pArrDataField->byFLen, "050", sizeof(pArrDataField->byFLen));
	

	if(!m_pDevCmn->fnAPL_CheckError())
	{
		sprintf(&szSetHWErrInfo[0] , "%c%2.2s%5.5s%2.2s", m_pProfile->TRANS.ProcCount[0],   m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]);
 		sprintf(&szSetHWErrInfo[10], "%c%2.2s%5.5s%2.2s", m_pProfile->TRANS.ProcCount[0],  m_pDevCmn->fnAPL_GetLC(m_pDevCmn->LastErrBuff.ErrorCode), m_pDevCmn->LastErrBuff.ErrorCode, &m_pDevCmn->LastErrBuff.ErrorCode[5]);
 		sprintf(&szSetHWErrInfo[20], "%c%2.2s%5.5s%2.2s", m_pProfile->TRANS.ProcCount[0], m_pDevCmn->fnAPL_GetLC(m_pDevCmn->FirstErrBuff.ErrorCode), m_pDevCmn->FirstErrBuff.ErrorCode, &m_pDevCmn->FirstErrBuff.ErrorCode[5]);
 		sprintf(&szSetHWErrInfo[30], "%c%2.2s%5.5s%2.2s", m_pProfile->TRANS.ProcCount[0], m_pDevCmn->fnAPL_GetLC(m_pDevCmn->FirstErrBuff.ErrorCode), m_pDevCmn->FirstErrBuff.ErrorCode, &m_pDevCmn->FirstErrBuff.ErrorCode[5]);
 		sprintf(&szSetHWErrInfo[40], "%c%2.2s%5.5s%2.2s", m_pProfile->TRANS.ProcCount[0], m_pDevCmn->fnAPL_GetLC(m_pDevCmn->FirstErrBuff.ErrorCode), m_pDevCmn->FirstErrBuff.ErrorCode, &m_pDevCmn->FirstErrBuff.ErrorCode[5]);
		memcpy(pArrDataField->byFData, szSetHWErrInfo, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		memcpy(pArrDataField->byFData, szSetHWErrInfo, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	
	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         :단말기의 IP Address(xxx.xxx.xxx.xxx)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_088X(int nTranCode , int nTxRxOption)
{
	CString	cstrTemp("");
	CStringArray strTempArray;
	int nArrSize = 0;
	
	DataField *pArrDataField = new DataField;	
	memcpy(pArrDataField->byFCode, x088X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "015", sizeof(pArrDataField->byFLen));

	cstrTemp.Empty();
	cstrTemp = m_pDevCmn->fnNET_GetIpAddress();
	if ((cstrTemp.GetLength() >= 7))
	{
		nArrSize = SplitString(cstrTemp, ".", strTempArray);
		if (nArrSize >= 4)
		{
			cstrTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", ASC2INT_SAFE_STRARRAY(strTempArray, 0),	
													   ASC2INT_SAFE_STRARRAY(strTempArray, 1), 
													   ASC2INT_SAFE_STRARRAY(strTempArray, 2),	
													   ASC2INT_SAFE_STRARRAY(strTempArray, 3));
			memcpy(m_pProfile->NETWORK.AtmIPAddress, cstrTemp.GetBuffer(0), 15);
			m_pProfile->NETWORK.AtmIPAddress[15] = NULL;
			m_pProfile->PutNetworkProfile();
			m_pProfile->GetNetworkProfile();
			memcpy(pArrDataField->byFData, cstrTemp.GetBuffer(0), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
			memcpy(pArrDataField->byFData, m_pProfile->NETWORK.AtmIPAddress, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		memcpy(pArrDataField->byFData, m_pProfile->NETWORK.AtmIPAddress, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         :단말기의 LanCard의 Physical Address('-'없이 Data만)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
typedef struct _ASTAT_
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER NameBuff [30];
}ASTAT, * PASTAT;

ASTAT Adapter;
int CTranCmn::fnAPP_SetDataField_Item_089X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	CString strResult = "E00000000000000000000000000";
	CString strResult1 = "E00000000000000000000000000";
// 	NCB Ncb;
// 	UCHAR uRetCode;
// 	LANA_ENUM lenum;
// 	int i;

// 	memset( &Ncb, 0, sizeof(Ncb) );
// 	Ncb.ncb_command = NCBENUM;
// 	Ncb.ncb_buffer = (UCHAR *)&lenum;
// 	Ncb.ncb_length = sizeof(lenum);
// 	uRetCode = Netbios( &Ncb );
// 
// 	for(i=0; i < lenum.length ;i++)
// 	{
// 		memset( &Ncb, 0, sizeof(Ncb) );
// 		Ncb.ncb_command = NCBRESET;
// 		Ncb.ncb_lana_num = lenum.lana[i];
// 
// 		uRetCode = Netbios( &Ncb );
// 
// 		memset( &Ncb, 0, sizeof (Ncb) );
// 		Ncb.ncb_command = NCBASTAT;
// 		Ncb.ncb_lana_num = lenum.lana[i];
// 
// 		strcpy( (char*)Ncb.ncb_callname, "* " );
// 		Ncb.ncb_buffer = (unsigned char *) &Adapter;
// 		Ncb.ncb_length = sizeof(Adapter);
// 
// 		uRetCode = Netbios( &Ncb );
// 		if ( uRetCode == 0 )
// 		{
// 
// 			UCHAR   adapter_type  = Adapter.adapt.adapter_type;
// 			if (adapter_type == 0xFE) //#CS0001
// 			{
// 
// 				strResult.Format("%02x%02x%02x%02x%02x%02x",
// 						Adapter.adapt.adapter_address[0],
// 						Adapter.adapt.adapter_address[1],
// 						Adapter.adapt.adapter_address[2],
// 						Adapter.adapt.adapter_address[3],
// 						Adapter.adapt.adapter_address[4],
// 						Adapter.adapt.adapter_address[5] );
// 			}
// 			else{
// 				strResult1.Format("%02x%02x%02x%02x%02x%02x",
// 						Adapter.adapt.adapter_address[0],
// 						Adapter.adapt.adapter_address[1],
// 						Adapter.adapt.adapter_address[2],
// 						Adapter.adapt.adapter_address[3],
// 						Adapter.adapt.adapter_address[4],
// 						Adapter.adapt.adapter_address[5] );
// 			}
// 		}
// 	}


// 	
// 	//[정리필요반듯이]
// 	CBIPConfig * pstrBipConfig;
// 	char szTempBuff[256] = {0,};
// 	static CString string("");
// 	
// 	pstrBipConfig = new CBIPConfig();
// 	pstrBipConfig->GetAdapterAddress(szTempBuff);
// 	delete pstrBipConfig;
// 	string.Empty();
// 	string.Format("%2.2s%2.2s%2.2s%2.2s%2.2s%2.2s", &szTempBuff[0], &szTempBuff[3],
// 		&szTempBuff[6], &szTempBuff[9], &szTempBuff[12], &szTempBuff[15]);
// 
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCMN_GetMacAddressValue() return(%12.12s)", string.GetBuffer(0));
//	string.AllocSysString();
	
	strResult = m_pDevCmn->fnNET_GetMacAddress(); //#CS0001
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_089X - 2(%s)", strResult);
	//[정리필요반듯이]
	memcpy(pArrDataField->byFCode, x089X, sizeof(pArrDataField->byFCode));
	sprintf(szTmp, "%03d", strResult.GetLength());	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));

	memcpy(pArrDataField->byFData, strResult.GetBuffer(0), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
//	memcpy(pArrDataField->byFData, "00fff9c11c56", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);




	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_090X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         :HOST작업 정보의 버전값
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_091X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x091X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));
	CString cstGetReg("000");
	if (nTranCode == TRANID_A010)
	{
		memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		cstGetReg = RegGetStr(_REGKEY_REMOTECTRL, "ECashRX091X", "000");
		cstGetReg = ZERO3 + cstGetReg;
		cstGetReg = cstGetReg.Right(3);

		if (Asc2Int(cstGetReg) <= 0 || 999 < Asc2Int(cstGetReg))
		{
			RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", "000");
			memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}

		memcpy(pArrDataField->byFData, cstGetReg.GetBuffer(0), Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         :단말기 프로그램의 버전(6자리숫자)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_092X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x092X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, APVERSION, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_093X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 명세표 광고 출력 버전
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_094X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x094X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "005", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "00000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_095X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_096X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_097X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_098X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_099X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 전원OFF기간
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_100X(int nTranCode , int nTxRxOption)
{
	
	static	int		SendProcInCnt = 0;						

	if (SendProcInCnt > 0) //#CS0002
		return T_OK;

	DataField *pArrDataField = new DataField;

	memcpy(pArrDataField->byFCode, x100X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "014", sizeof(pArrDataField->byFLen));

	char szTmp[1024] = {0.};
	CString cstGetReg("");
	cstGetReg = RegGetStr(_REGKEY_DEVICE, "POWER_OFF_TIME", "00000000000000");

	if(TranCode2 == TRANID_A020)
	{
		if (cstGetReg == "00000000000000" )
			sprintf(szTmp, "%8.8s%6.6s", GetDate().GetBuffer(0), GetTime().GetBuffer(0));
		else
			sprintf(szTmp, "%s", cstGetReg);

		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
		memcpy(pArrDataField->byFData, ZERO14, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	SendProcInCnt++;

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 거래가능기관 정보의 버젼값
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_101X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	CString cstOrg("");
	memcpy(pArrDataField->byFCode, x101X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));
	if(TranCode2 == TRANID_A010)
	{
		memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		if (CardBank <= 0)
		{
			memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
		{
			memcpy(pArrDataField->byFData, m_FRX101X, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}

	}
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	return T_OK;
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 명세표 인자Data
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_104X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x104X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "010", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, SPACE10, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 거절응답사유
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_130X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x130X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "050", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_131X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_166X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_167X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_1680(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_1810(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_169X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_170X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 171X - 설치장소의 영업업체 코드 정보
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
#define INCHON_AIRPORT_AREA _T("5300000"); //인천공항 별도
int CTranCmn::fnAPP_SetDataField_Item_171X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x171X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "007", sizeof(pArrDataField->byFLen));

	if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)
		memcpy(pArrDataField->byFData, "5300000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
		memcpy(pArrDataField->byFData, "5300000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 172X - 설치장소의 브랜드구분 코드 정보
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_172X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x172X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "007", sizeof(pArrDataField->byFLen));


	if (m_pProfile->DEVICE.ETCDevice20 == KB)
		memcpy(pArrDataField->byFData, "0040002", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
		memcpy(pArrDataField->byFData, "0000001", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 173X - 단말기의 카세트별 설정된 권종정보
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_173X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x173X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "3111", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 현금부족체크 0:정상 1:부족
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_464X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x464X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));

	int nCash_10000 = 0;
	int nCash_50000 = 0;

	nCash_10000 = m_pDevCmn->fnAPL_GetNumberOfCash(); 
	nCash_50000 = m_pDevCmn->fnAPL_GetNumberOf50000Cash();

	if ((nCash_10000 < BRM_MONEY_MAXOUT) || (nCash_50000 < BRM_MONEY_MAXOUT))
		memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 오천원권 매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_189X(int nTranCode , int nTxRxOption)
{
	if(TranProc == TRAN_WITH)
	{
		DataField *pArrDataField = new DataField;
		memcpy(pArrDataField->byFCode, x189X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	else
	if(TranProc == TRAN_DEP)
	{
		
		char szTmp[1024];
		DataField *pArrDataField = new DataField;
		memcpy(pArrDataField->byFCode, x1891, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));
		sprintf(szTmp, "%03d", CashData.mFiveThousand);
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	}
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 천원권 매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_190X(int nTranCode , int nTxRxOption)
{

	if(TranProc == TRAN_WITH)
	{
		DataField *pArrDataField = new DataField;
		memcpy(pArrDataField->byFCode, x190X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	else
	if(TranProc == TRAN_DEP)
	{
		char szTmp[1024];
		DataField *pArrDataField = new DataField;
		memcpy(pArrDataField->byFCode, x1901, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));
		sprintf(szTmp, "%03d", CashData.mOneThousand);
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	}

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_191X(int nTranCode , int nTxRxOption)
{
	char szTmp[1024];
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x191X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "009", sizeof(pArrDataField->byFLen));
	sprintf(szTmp, "%2.2s%7.7s",Accept.JiroTranOrgCode, Accept.JiroNum);
	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_192X(int nTranCode , int nTxRxOption)
{
	char szTmp[1024];
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x192X, sizeof(pArrDataField->byFCode));
	if (TranProc == TRAN_UTILYBILLS)
	{
		memcpy(pArrDataField->byFLen, "019", sizeof(pArrDataField->byFLen));
		sprintf(szTmp, "%19.19s", Accept.JiroElecNum);
	}
	else
	{
		memcpy(pArrDataField->byFLen, "021", sizeof(pArrDataField->byFLen));
		sprintf(szTmp, "%2.2s%19.19s",m_RXLOCALTAXDATA.Gubun,  Accept.JiroElecNum);
	}
	
	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_193X(int nTranCode , int nTxRxOption)
{
	char szTmp[1024];
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x193X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "100", sizeof(pArrDataField->byFLen));
	sprintf(szTmp, "%100.100s", m_RXJIRODATA.JiroPayData   );
	memcpy(pArrDataField->byFData, m_RXJIRODATA.JiroPayData , Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_1931(int nTranCode , int nTxRxOption)
{
	char szTmp[1024];
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x1931, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "295", sizeof(pArrDataField->byFLen));
	
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "m_RXLOCALTAXDATA.JiroPayData", m_RXLOCALTAXDATA.JiroPayData, sizeof(m_RXLOCALTAXDATA.JiroPayData));
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_RXLOCALTAXDATA.JiroPayData=%s", m_RXLOCALTAXDATA.JiroPayData);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_RXLOCALTAXDATA.JiroPayData=%c", m_RXLOCALTAXDATA.JiroPayData[0]);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_RXLOCALTAXDATA.JiroPayData=%c", m_RXLOCALTAXDATA.JiroPayData[1]);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_RXLOCALTAXDATA.JiroPayData=%c", m_RXLOCALTAXDATA.JiroPayData[2]);
	sprintf(szTmp, "%295.295s", m_RXLOCALTAXDATA.JiroPayData );
	memcpy(pArrDataField->byFData, m_RXLOCALTAXDATA.JiroPayData, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_RXLOCALTAXDATA.JiroPayData=%s", pArrDataField->byFData);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_RXLOCALTAXDATA.JiroPayData=%s", szTmp);
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_194X(int nTranCode , int nTxRxOption)
{
	char szTmp[1024];
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x194X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
	sprintf(szTmp, "%16.16s", m_RXJIRODATA.JiroBankCode );
	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_195X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_196X(int nTranCode , int nTxRxOption)
{
	char szTmp[1024];
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x196X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
	sprintf(szTmp, "%02d", Asc2Int(Accept.JiroPayMonth,2));
	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;
}
int CTranCmn::fnAPP_SetDataField_Item_201X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_202X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_204X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_205X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_206X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_207X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_208X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_209X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_210X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_211X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_811X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_212X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_812X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_213X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_813X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_214X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_215X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_815X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_216X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_816X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_217X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_817X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_218X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_818X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_828X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_219X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
//#0038
int CTranCmn::fnAPP_SetDataField_Item_220X(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		char szTmp[1024];
		memset(szTmp, 0x20, sizeof(szTmp));
		DataField *pArrDataField = new DataField;	
		memcpy(pArrDataField->byFCode, x220X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "013", sizeof(pArrDataField->byFLen));
		sprintf(szTmp,"%8.8s     ", Accept.JuminNum);
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}	
	else
	if (MenuAvail & MENU_MU)
	{
		if(m_pProfile->DEVICE.ETCDevice20 == HANA)
		{
			DataField *pArrDataField = new DataField;
			memcpy(pArrDataField->byFCode, x220X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "026", sizeof(pArrDataField->byFLen));
			memcpy(pArrDataField->byFData, &m_STXEncBuff.byChiperMsdata[18], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
		}
		else
		{
			DataField *pArrDataField = new DataField;
			memcpy(pArrDataField->byFCode, x220X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "013", sizeof(pArrDataField->byFLen));
			memcpy(pArrDataField->byFData, Accept.JuminNum, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
		}
		
	}

	return T_OK;	
}


int CTranCmn::fnAPP_SetDataField_Item_221X(int nTranCode , int nTxRxOption)
{
	if (TranProc == TRAN_HIPASS) 			//#N0266
	{
		DataField *pArrDataField = new DataField;
		memcpy(pArrDataField->byFCode, x221X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "008", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, Accept.ETicketConfirmNum, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}

	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_222X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x222X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "012", sizeof(pArrDataField->byFLen));
	m_pProfile->GetTransProfile();
	memcpy(pArrDataField->byFData, m_pProfile->TRANS.HostSerialNo, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_223X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_224X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x224X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, m_RXETICKETDATA.byDataField_003x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_225X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_230X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_231X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_232X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_233X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_234X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_235X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_236X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_237X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_238X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_240X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_241X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_242X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_243X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}


int CTranCmn::fnAPP_SetDataField_Item_245X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_246X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_247X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_248X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_249X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_250X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_251X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_252X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_253X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_254X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_255X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_256X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_257X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_258X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_260X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_261X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_270X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_271X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_272X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_273X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_280X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_281X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_282X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_283X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_284X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_285X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_286X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_287X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_288X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_289X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_290X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_291X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_299X(int nTranCode , int nTxRxOption)
{
	
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_300X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_301X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_302X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_303X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_304x(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_305x(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_306x(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}

//전자상품권 인증매체 구분
int CTranCmn::fnAPP_SetDataField_Item_310X(int nTranCode , int nTxRxOption)
{
	if (TranCode == TC_HIPASS_CREDIT) 		
	{
		DataField *pArrDataField = new DataField;
		memcpy(pArrDataField->byFCode, x3101, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData,"2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	else
	if (TranCode == TC_HIPASS_CASH) 		
	{
		DataField *pArrDataField = new DataField;
		memcpy(pArrDataField->byFCode, x3101, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData,"5", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}	
	else
	{
		DataField *pArrDataField = new DataField;
		memcpy(pArrDataField->byFCode, x3101, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
		if(Accept.ETicketPayWay == 1)
			memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		else
		if(Accept.ETicketPayWay == 2)
			memcpy(pArrDataField->byFData, "5", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		else
			memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}	
	
	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_311X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_312X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_313X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_314X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_315X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_316X(int nTranCode , int nTxRxOption)
{


	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_320X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_915X(int nTranCode , int nTxRxOption)
{
	if(nTranCode == TRANID_7320 || nTranCode == TRANID_7323 || nTranCode == TRANID_4810 || nTranCode == TRANID_4813 || nTranCode == TRANID_7321 || nTranCode == TRANID_7324 || \
		nTranCode == TRANID_7322 || nTranCode == TRANID_7325 || nTranCode == TRANID_7328 || nTranCode == TRANID_4811 || nTranCode == TRANID_4814 || nTranCode == TRANID_4812 || \
		nTranCode == TRANID_7326 || nTranCode == TRANID_7327 || nTranCode == TRANID_7329)
	{
		if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
		{
			char szTmp[1024];
			DataField *pArrDataField = new DataField;
			memcpy(pArrDataField->byFCode, x915X, sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			sprintf(szTmp, "%4.4s", "    "   );
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
		}
	}

	return T_OK;
	//미사용 ****************************

	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x915X, sizeof(pArrDataField->byFCode));
	
	if(MenuFICMode & MENU_IC)
	{
		if(TranEncCode == TRAN_ENC_003_06 || TranEncCode == TRAN_ENC_006_01) //국민,기업
		{
			memcpy(pArrDataField->byFCode, "9151", sizeof(pArrDataField->byFCode));
			memcpy(pArrDataField->byFLen, "024", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[104], 24);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
			
		}
		else
		if(TranEncCode == TRAN_ENC_025_04) //하나
		{
			memcpy(pArrDataField->byFLen, "104", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, CardData.ISO3Buff, CardData.ISO3size);	
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			//ms (16 byte Over Write)
			memcpy(&szTmp[8], &m_STXEncBuff.byChiperMsdata[0], 16);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
			fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
		}

		if(TranEncCode == TRAN_ENC_011_07) //농협
		{
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, SPACE4, 4);
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		{
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_9151(int nTranCode , int nTxRxOption)
{
	return T_OK;
	//미사용 ****************************

	if(TranEncCode == TRAN_ENC_000_00) 
		return T_OK;

	if(TranEncCode == TRAN_ENC_003_06)
		return T_OK;

	int nEncType =TranEncCode;

	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, "9151", sizeof(pArrDataField->byFCode));

	if(MenuAvail & MENU_MCU3)
	{
		if(MenuFICMode & MENU_IC && MenuAvail & MENU_MCU3)
		{
			if(nEncType == TRAN_ENC_240_10) //삼성증권 
			{
				memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
				memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
				memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}
			else
			if(nEncType == TRAN_ENC_034_02) //광주 
			{
				memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
				memcpy(szTmp, &m_STXEncBuff.byChiperMsdata[104], 16);
				memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
			}
			else
			{
				memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
				memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
				memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
			}
		}
		else
		if(nEncType == TRAN_ENC_011_07) //농협
		{
			sprintf(szTmp, "%03d", Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
			memcpy(szTmp, &m_STXEncBuff.byChiperMsdata, Asc2Int(m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen)));
			memcpy(pArrDataField->byFData, m_STXEncBuff.byChiperMsdata, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));			
		}
		else
		{
			memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
			memcpy(szTmp, Accept.PassWord, sizeof(Accept.PassWord));
			memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	}
	
	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 장애구분
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_034X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x034X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	if(m_pDevCmn->TranStatus == TRAN_OPEN)
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
	if(m_pDevCmn->AtmStatus == ATM_INIT || m_pDevCmn->AtmStatus == ATM_ERROR || m_pDevCmn->AtmStatus == ATM_CLERK || m_pDevCmn->AtmStatus == ATM_READY)
	{
// 		if (m_pDevCmn->PowerOffSendFlag) //#CS0001
// 			memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 		else
		memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 지폐함 및 용지 상태정보 - GOOD, NEAR-END, EMPTY 의 상태를 '0', '1', '2'의 숫자로 각각 표현하여 현재 상태를 HOST에 알린다.
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_035X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szStatus[1024];
	CString strMediaStatus("");
	memset(szStatus, 0x30, sizeof(szStatus));

	//1카세트
	if      (m_pDevCmn->FstCSTStatus == CST_NORMAL)	      szStatus[0] = '0';
	else if (m_pDevCmn->FstCSTStatus == CST_EMPTY)		  szStatus[0] = '2';
	else if (m_pDevCmn->FstCSTStatus == CST_NEAR)         szStatus[0] = '1';
	else if (m_pDevCmn->FstCSTStatus == CST_FULL)         szStatus[0] = '0';
	else if (m_pDevCmn->FstCSTStatus == CST_SET_NG)       szStatus[0] = '2';
	else                                                  szStatus[0] = '2';

	//2카세트
	if      (m_pDevCmn->SndCSTStatus == CST_NORMAL)	      szStatus[1] = '0';
	else if (m_pDevCmn->SndCSTStatus == CST_EMPTY)		  szStatus[1] = '2';
	else if (m_pDevCmn->SndCSTStatus == CST_NEAR)         szStatus[1] = '1';
	else if (m_pDevCmn->SndCSTStatus == CST_FULL)         szStatus[1] = '0';
	else if (m_pDevCmn->SndCSTStatus == CST_SET_NG)       szStatus[1] = '2';
	else                                                  szStatus[1] = '2';

	if(m_pProfile->DEVICE.MachineType == U3100K)	
	{
		//3카세트
		if      (m_pDevCmn->TrdCSTStatus == CST_NORMAL)	      szStatus[2] = '0'; 
		else if (m_pDevCmn->TrdCSTStatus == CST_EMPTY)		  szStatus[2] = '2';
		else if (m_pDevCmn->TrdCSTStatus == CST_NEAR)         szStatus[2] = '1';
		else if (m_pDevCmn->TrdCSTStatus == CST_FULL)         szStatus[2] = '0';
		else if (m_pDevCmn->TrdCSTStatus == CST_SET_NG)       szStatus[2] = '2';
		else                                                  szStatus[2] = '2';
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		if      (m_pDevCmn->TrdCSTStatus == CST_NORMAL)	      szStatus[2] = '2';
		else if (m_pDevCmn->TrdCSTStatus == CST_EMPTY)		  szStatus[2] = '2';
		else if (m_pDevCmn->TrdCSTStatus == CST_NEAR)         szStatus[2] = '2';
		else if (m_pDevCmn->TrdCSTStatus == CST_FULL)         szStatus[2] = '2';
		else if (m_pDevCmn->TrdCSTStatus == CST_SET_NG)       szStatus[2] = '2';
		else                                                  szStatus[2] = '2';

	}
	else
	{
		szStatus[2] = '2';
	}
	
	if(m_pProfile->DEVICE.MachineType == U3100K)	
	{
		//4카세트 (Reject)	 
		if      (m_pDevCmn->FthCSTStatus == CST_NORMAL)	     szStatus[3] = '2';
		else if (m_pDevCmn->FthCSTStatus == CST_EMPTY)		 szStatus[3] = '2';
		else if (m_pDevCmn->FthCSTStatus == CST_NEAR)        szStatus[3] = '2';
		else if (m_pDevCmn->FthCSTStatus == CST_FULL)        szStatus[3] = '2';
		else if (m_pDevCmn->FthCSTStatus == CST_SET_NG)      szStatus[3] = '2';
		else                                                 szStatus[3] = '2';

	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		if      (m_pDevCmn->FthCSTStatus == CST_NORMAL)	     szStatus[3] = '2';
		else if (m_pDevCmn->FthCSTStatus == CST_EMPTY)		 szStatus[3] = '2';
		else if (m_pDevCmn->FthCSTStatus == CST_NEAR)        szStatus[3] = '2';
		else if (m_pDevCmn->FthCSTStatus == CST_FULL)        szStatus[3] = '2';
		else if (m_pDevCmn->FthCSTStatus == CST_SET_NG)      szStatus[3] = '2';
		else                                                 szStatus[3] = '2';

	}
	else
	{
		szStatus[3] = '2';
	}
	
	int nRtractCnt = 0;
	nRtractCnt = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //미수취 //Reject 매수
	nRtractCnt += RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //리젝트 - 불량 
	nRtractCnt += RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt",0); //천원,5천원 - 입금
	nRtractCnt += RegGetInt(_REGKEY_BRMSTATUS, "ClerkRJBNoteCnt",0); //계원회수함매수

	//정리필요반듯이
	//5카세트 (Reject)	실제로 회수금액 있으나 0으로 올라감 Cnt 에 따른 상태 없음 비정상일때 값 정의 필요.
	if      (m_pDevCmn->RejectCSTStatus == CST_NORMAL || m_pDevCmn-> RetractCSTStatus == CST_NORMAL)	 szStatus[4] = '0';
	else if (m_pDevCmn->RejectCSTStatus == CST_EMPTY || m_pDevCmn->RetractCSTStatus == CST_EMPTY)		 szStatus[4] = '0';
	else if (m_pDevCmn->RejectCSTStatus == CST_NEAR || m_pDevCmn->RetractCSTStatus == CST_NEAR)			 szStatus[4] = '1';
	else if (m_pDevCmn->RejectCSTStatus == CST_FULL || m_pDevCmn->RetractCSTStatus == CST_FULL)          szStatus[4] = '4';
	else if (m_pDevCmn->RejectCSTStatus == CST_SET_NG || m_pDevCmn->RetractCSTStatus == CST_SET_NG)      szStatus[4] = '4';
	else if (nRtractCnt > 300)																		  	 szStatus[4] = '3';
	else if (nRtractCnt > 200)																		     szStatus[4] = '2';
	else                                                                                                 szStatus[4] = '2';

	
	//5카세트 (Reject)	 
	if      (m_pDevCmn->RejectCSTStatus == CST_NORMAL)	     szStatus[4] = '0';
	else if (m_pDevCmn->RejectCSTStatus == CST_EMPTY)		 szStatus[4] = '0';
	else if (m_pDevCmn->RejectCSTStatus == CST_NEAR)        szStatus[4] = '1';
	else if (m_pDevCmn->RejectCSTStatus == CST_FULL)        szStatus[4] = '2';
	else if (m_pDevCmn->RejectCSTStatus == CST_SET_NG)      szStatus[4] = '2';
	else                                                     szStatus[4] = '2';


	//6명세표
	if (m_pDevCmn->SlipStatus == SLIP_NORMAL)				szStatus[5] = '0';
	else if (m_pDevCmn->SlipStatus == SLIP_LOW_END)			szStatus[5] = '1';
	else if (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)		szStatus[5] = '2';


	//8카세트 (입금전용)
	if(m_pProfile->DEVICE.MachineType == U3100K)	
	{

		if      (m_pDevCmn->RejectCSTStatus == CST_NORMAL)	     szStatus[7] = '0';
		else if (m_pDevCmn->RejectCSTStatus == CST_EMPTY)		 szStatus[7] = '0';
		else if (m_pDevCmn->RejectCSTStatus == CST_NEAR)         szStatus[7] = '0';
		else if (m_pDevCmn->RejectCSTStatus == CST_FULL)         szStatus[7] = '2';
		else if (m_pDevCmn->RejectCSTStatus == CST_SET_NG)       szStatus[7] = '2';
		else                                                     szStatus[7] = '2';
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		if      (m_pDevCmn->RejectCSTStatus == CST_NORMAL)	     szStatus[7] = '0';
		else if (m_pDevCmn->RejectCSTStatus == CST_EMPTY)		 szStatus[7] = '0';
		else if (m_pDevCmn->RejectCSTStatus == CST_NEAR)         szStatus[7] = '0';
		else if (m_pDevCmn->RejectCSTStatus == CST_FULL)         szStatus[7] = '2';
		else if (m_pDevCmn->RejectCSTStatus == CST_SET_NG)       szStatus[7] = '2';
		else                                                     szStatus[7] = '2';

	}
	else
	{
		szStatus[7] = '2';
	}


	//11카세트 (Retract)	 
// 	if      (m_pDevCmn->RetractCSTStatus == CST_NORMAL)	     szStatus[10] = '0';
// 	else if (m_pDevCmn->RetractCSTStatus == CST_EMPTY)		 szStatus[10] = '0';
// 	else if (m_pDevCmn->RetractCSTStatus == CST_NEAR)        szStatus[10] = '1';
// 	else if (m_pDevCmn->RetractCSTStatus == CST_FULL)        szStatus[10] = '2';
// 	else if (m_pDevCmn->RetractCSTStatus == CST_SET_NG)      szStatus[10] = '2';
// 	else                                                     szStatus[10] = '2';

	//매체잔류
	if      (m_pDevCmn->fnCSH_GetMaterialInfo())			 szStatus[10] = '1';
	else if (m_pDevCmn->RetractCSTStatus == CST_EMPTY)		 szStatus[10] = '0';
 	else                                                     szStatus[10] = '0';

	memcpy(pArrDataField->byFCode, x035X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "015", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szStatus, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 지폐함별 권종 설정 정보 '0': 사용안함 '1': 1만원권 사용 '5': 5만원권 사용 'A': 10만원권 사용
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_036X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szSetNote[1024];
	memset(szSetNote, 0x30, sizeof(szSetNote));
	memcpy(pArrDataField->byFCode, x036X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	if(m_pProfile->DEVICE.MachineType == U3100K)	
		sprintf(szSetNote, "%s%s%s%s", "5","1","1","1"); //#0075
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
		sprintf(szSetNote, "%s%s%s%s", "5","1","1","1");
	else
		sprintf(szSetNote, "%s%s%s%s", "1","1","1","1");
		
	memcpy(pArrDataField->byFData, szSetNote, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 자동기 제조사 SerialNum
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_047X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szMachinNum[1024];
	memset(szMachinNum, 0x20, sizeof(szMachinNum));
	memcpy(pArrDataField->byFCode, x047X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "012", sizeof(pArrDataField->byFLen));
	sprintf(szMachinNum, "%12.12s%", m_pProfile->NETWORK.MachineMakeNum);
	memcpy(pArrDataField->byFData, szMachinNum, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 통신망관리정보 (개국전문:001, 상태전문:301)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_083X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x083X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));

	if(TranCode2 == TRANID_A010)
	{
		if ((m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT)	||		//#N0278
			(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT1) ||
			(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT3)	||
			(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT4) ||
			(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT5) ||
			(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT6))
		{
			memcpy(pArrDataField->byFData, "004", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
		{
			memcpy(pArrDataField->byFData, "001", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
	}
	else
	if(TranCode2 == TRANID_A020)
	{
		memcpy(pArrDataField->byFData, "301", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
		memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	


	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : Shutdown시각
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_084X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x084X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "014", sizeof(pArrDataField->byFLen));

	if(TranCode2 == TRANID_A010)
	{
		sprintf(szTmp, "%s", RegGetStr(_REGKEY_DEVICE, "POWER_OFF_TIME", "00000000000000"));
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if(TranCode2 == TRANID_A020)
	{
		memcpy(pArrDataField->byFData, ZERO14, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
		memcpy(pArrDataField->byFData, ZERO14, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : H/W Module 상태정보 3Byte * 21 = 63 Byte
/*
/*               : SST를 구성하는 장치들(H/W)의 상태는 하나의 Module당 총 3 Byte의 정보로 구성되며, 
                 : 1 Byte의 Module상태와 2 Byte의 Module별 세부 장애코드(M-Status)로 구성되어 있다.
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_102X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	int	 nLen = 0;
	char szSetHWInfo[1024];
	CString cstrLC("00");
	CString cstrLC1("");

	memset(szSetHWInfo, 0x30, sizeof(szSetHWInfo));
	memcpy(pArrDataField->byFCode, x102X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "045", sizeof(pArrDataField->byFLen));

	if (m_pDevCmn->ForceMissMatchDownFlag) //TRUE : MisMatch
	{
		szSetHWInfo[3] = '4';
		nLen += 1;
		memcpy(&szSetHWInfo[4], "X8", 2);
		nLen += 2;
	}
	else 
	if (!(m_pDevCmn->DepAvail & TRAN_DEP_CASH) && !(m_pDevCmn->WithAvail & TRAN_WITH_2CASH))
	{
		szSetHWInfo[3] = '4';
		nLen += 1;
		memcpy(&szSetHWInfo[4], "X9", 2);
		nLen += 2;
	}
	else
	if (!(m_pDevCmn->DepAvail & TRAN_DEP_CASH)) 
	{
		szSetHWInfo[3] = '0';
		nLen += 1;
		memcpy(&szSetHWInfo[4], "00", 2);
		nLen += 2;
	}
	else
	if (!(m_pDevCmn->WithAvail & TRAN_WITH_2CASH))
	{
		szSetHWInfo[3] = '0';
		nLen += 1;
		memcpy(&szSetHWInfo[4], "00", 2);
		nLen += 2;
	}
	else 
	if (!(m_pDevCmn->PbmAvail & TRAN_PBM_OK) && m_pProfile->DEVICE.PBHandler)
	{
		szSetHWInfo[36] = '4';
		nLen += 1;
		memcpy(&szSetHWInfo[37], "VD", 2);
		nLen += 2;
	}
	else
	if (m_pDevCmn->DeviceNoDevice & DEV_PBM)
	{
		szSetHWInfo[36] = '4';
		nLen += 1;
		memcpy(&szSetHWInfo[37], "VD", 2);
		nLen += 2;
	}
	else 
	if (m_pDevCmn->DeviceNoDevice & DEV_SPR)
	{
		szSetHWInfo[9] = '4';
		nLen += 1;
		memcpy(&szSetHWInfo[10], "SL", 2);
		nLen += 2;
	}
	else
	if ((m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) & DEV_SPR) || (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)))
	{
		szSetHWInfo[9] = '4';
		nLen += 1;
		memcpy(&szSetHWInfo[10], "SL", 2);
		nLen += 2;
	}

	else
	if ((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL || m_pDevCmn->SlipHeadStatus != SLIP_NORMAL))
	{
		szSetHWInfo[9] = '4';
		nLen += 1;
		memcpy(&szSetHWInfo[10], "SL", 2);
		nLen += 2;
	}
	
	if(!m_pDevCmn->fnAPL_CheckError())
	{

		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) & DEV_CSH)
		{
			szSetHWInfo[3] = m_pDevCmn->fnAPL_GetErrorCodeGrade(m_pDevCmn->Om.BrmErrorCode);
			memcpy(&szSetHWInfo[4], cstrLC.GetBuffer(0), 2);
			//장애상태 -> Shutter Stack 점검
			if(m_pDevCmn->fnBRM_GetMaterialInfo()) 
			{
				szSetHWInfo[3] = '4';
				memcpy(&szSetHWInfo[4], "X3", 2); //지폐미수쉬
			}

		}

		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) & DEV_MCU)
		{
			szSetHWInfo[6] = m_pDevCmn->fnAPL_GetErrorCodeGrade(m_pDevCmn->Om.McuErrorCode);
			nLen += 1;
			memcpy(&szSetHWInfo[7], cstrLC.GetBuffer(0), 2);
			nLen += 2;
		}

		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) & DEV_SPR)
		{
			szSetHWInfo[9] = m_pDevCmn->fnAPL_GetErrorCodeGrade(m_pDevCmn->Om.SprErrorCode);
			nLen += 1;
			memcpy(&szSetHWInfo[10], cstrLC.GetBuffer(0), 2);
			nLen += 2;
		}
		
		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) & DEV_PBM)
		{
			szSetHWInfo[36] = m_pDevCmn->fnAPL_GetErrorCodeGrade(m_pDevCmn->Om.PbmErrorCode);
			nLen += 1;
			memcpy(&szSetHWInfo[37], cstrLC.GetBuffer(0), 2);
			nLen += 2;
		}

	}

    //정리필요반듯이
	if (m_pDevCmn->fnCMR_GetFaceCameraDeviceStatus() != NORMAL && m_pDevCmn->fnCMR_GetHandCameraDeviceStatus() != NORMAL)
	{
		szSetHWInfo[42] = '3';
		memcpy(&szSetHWInfo[43], "X3", 2);
	}
	else
	if (m_pDevCmn->fnCMR_GetFaceCameraDeviceStatus() != NORMAL)
	{
		szSetHWInfo[42] = '1';
		memcpy(&szSetHWInfo[43], "X1", 2);
	}
	else
	if (m_pDevCmn->fnCMR_GetHandCameraDeviceStatus() != NORMAL)
	{
		szSetHWInfo[42] = '2';
		memcpy(&szSetHWInfo[43], "X2", 2);
	}	

	memcpy(pArrDataField->byFData, szSetHWInfo, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 자원 감시 상태정보
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_103X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szNoteStatus[1024];
	CString strMediaStatus("");
	memset(szNoteStatus, 0x30, sizeof(szNoteStatus));
	memcpy(pArrDataField->byFCode, x103X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "015", sizeof(pArrDataField->byFLen));
	
	if(!m_pDevCmn->OpenKey) szNoteStatus[0] =  '1';	// 운영키
	else szNoteStatus[0] = '0';


	if(m_pDevCmn->DoorStatus == DOOR_OPENED) szNoteStatus[1] = '1';	// 도어
	else szNoteStatus[1] = '0';
	
	if(m_pDevCmn->DoorStatus == DOOR_OPENED) szNoteStatus[2] = '1';	// 도어
	else szNoteStatus[2] = '0';
	
	if(m_pDevCmn->GAlarmStatus == NORMAL_POS) szNoteStatus[3] = '0';	// 충격장치
	else szNoteStatus[3] = '1';

	if(m_pDevCmn->CabinetStatus == DOOR_OPENED) szNoteStatus[5] = '1';	// 도어
	else szNoteStatus[5] = '0';


	if(m_pProfile->DEVICE.MachineType == U8100)
	{	
		if      (m_pDevCmn->RejectCSTStatus == CST_NORMAL)	    szNoteStatus[6] = '0';
		else if (m_pDevCmn->RejectCSTStatus == CST_EMPTY)		szNoteStatus[6] = '0';
		else if (m_pDevCmn->RejectCSTStatus == CST_NEAR)        szNoteStatus[6] = '0';
		else if (m_pDevCmn->RejectCSTStatus == CST_FULL)        szNoteStatus[6] = '0';
		else if (m_pDevCmn->RejectCSTStatus == CST_SET_NG)      szNoteStatus[6] = '1';
		else                                                    szNoteStatus[6] = '1';
	}
	else
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		if(m_pDevCmn->RetractCSTStatus == CST_SET_NG) szNoteStatus[6] = '1';	// 회수함
		else szNoteStatus[6] = '0';
	}
	else
	{
		if(m_pDevCmn->RetractCSTStatus == CST_SET_NG) szNoteStatus[6] = '1';	// 회수함
		else szNoteStatus[6] = '0';
	}


	//3회수함
	strMediaStatus = RegGetStr(_REGKEY_BRMSTATUS, "FRJBStatus");
	if ("OK" == strMediaStatus)	          szNoteStatus[6] = '0';
	else if ("LOW" == strMediaStatus)     szNoteStatus[6] = '0';
	else if ("HIGH" == strMediaStatus)	  szNoteStatus[6] = '0';
	else if ("FULL" == strMediaStatus)    szNoteStatus[6] = '0';
	else if ("EMPTY" == strMediaStatus)   szNoteStatus[6] = '0';
	else if ("MISSING" == strMediaStatus) szNoteStatus[6] = '1';

//  금과장님 요청 03.28 -> 09.28 
	if(m_pProfile->DEVICE.MachineType == U8100)
	{	
		if(m_pDevCmn->BrmLoc == NORMAL_POS) szNoteStatus[7] = '0';	        // BRM 정위치 상태
		else szNoteStatus[7] = '1';
	}
	else
	{
		//1카세트
		strMediaStatus = RegGetStr(_REGKEY_BRMSTATUS, "RB1Status");
		if ("OK" == strMediaStatus)	          szNoteStatus[7] = '0';
		else if ("LOW" == strMediaStatus)     szNoteStatus[7] = '0';
		else if ("HIGH" == strMediaStatus)	  szNoteStatus[7] = '0';
		else if ("FULL" == strMediaStatus)    szNoteStatus[7] = '0';
		else if ("EMPTY" == strMediaStatus)   szNoteStatus[7] = '0';
		else if ("MISSING" == strMediaStatus) szNoteStatus[7] = '1';
	}

	//2카세트
	strMediaStatus = RegGetStr(_REGKEY_BRMSTATUS, "RB2Status");
	if ("OK" == strMediaStatus)	          szNoteStatus[8] = '0';
	else if ("LOW" == strMediaStatus)     szNoteStatus[8] = '0';
	else if ("HIGH" == strMediaStatus)	  szNoteStatus[8] = '0';
	else if ("FULL" == strMediaStatus)    szNoteStatus[8] = '0';
	else if ("EMPTY" == strMediaStatus)   szNoteStatus[8] = '0';
	else if ("MISSING" == strMediaStatus) szNoteStatus[8] = '1';

	//3카세트-입금전용
	strMediaStatus = RegGetStr(_REGKEY_BRMSTATUS, "RB3Status");
	if ("OK" == strMediaStatus)	          szNoteStatus[9] = '0';
	else if ("LOW" == strMediaStatus)     szNoteStatus[9] = '0';
	else if ("HIGH" == strMediaStatus)	  szNoteStatus[9] = '0';
	else if ("FULL" == strMediaStatus)    szNoteStatus[9] = '0';
	else if ("EMPTY" == strMediaStatus)   szNoteStatus[9] = '0';
	else if ("MISSING" == strMediaStatus) szNoteStatus[9] = '1';

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{	
		//4 Reject Box
		if(m_pDevCmn->RejectCSTStatus == CST_SET_NG) szNoteStatus[10] = '1';	// 1카세트
		else szNoteStatus[10] = '0';
	}


	//카세트-입금전용
	if (m_pDevCmn->fnMCU_GetMaterialInfo() & ST_SENSOR2)
		szNoteStatus[11] = '1';
	else
		szNoteStatus[11] = '0';

	//보충함정위치상태
	if (m_pDevCmn->ClerkCSTLoc == NORMAL_POS)
		szNoteStatus[12] = '0';
	else
		szNoteStatus[12] = '1';
	
	memcpy(pArrDataField->byFData, szNoteStatus, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 총 예금출금 건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_105X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x105X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[5].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 총 예금출금 금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_106X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];

	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x106X, sizeof(pArrDataField->byFCode));
	sprintf(szGetNoteAmt, "%d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[5].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szGetNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
// int CTranCmn::fnAPP_SetDataField_Item_107X(int nTranCode , int nTxRxOption)
// {
// 
// 
// 	return T_OK;	
// }
// int CTranCmn::fnAPP_SetDataField_Item_108X(int nTranCode , int nTxRxOption)
// {
// 	DataField *pArrDataField = new DataField;
// 	char szTmp[1024];
// 	char szGetNoteAmt[1024];
// 
// 	memset(szGetNoteAmt, 0x30, sizeof(szGetNoteAmt));
// 	memcpy(pArrDataField->byFCode, x108X, sizeof(pArrDataField->byFCode));
// 	sprintf(szTmp, "%3.3s", "008");	
// 	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
// 	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
// 	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
// 
// 
// 	return T_OK;	
// }

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 총예금 취소 총건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_107X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x107X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[1].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 164X - 총예금 취소 총금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/


int CTranCmn::fnAPP_SetDataField_Item_108X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];

	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x108X, sizeof(pArrDataField->byFCode));
	sprintf(szGetNoteAmt, "%d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[1].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szGetNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 총 서비스출금 건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_109X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x109X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[9].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 총 서비스출금 금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_110X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];

	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x110X, sizeof(pArrDataField->byFCode));
	sprintf(szGetNoteAmt, "%d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[9].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szGetNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 총서비스출금취소건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_111X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x111X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[2].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 총서비스출금취소금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_112X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];

	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x112X, sizeof(pArrDataField->byFCode));
	sprintf(szGetNoteAmt, "%d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[2].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szGetNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 자행이체건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_113X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x113X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[0].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 자행이체금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_114X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];

	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x114X, sizeof(pArrDataField->byFCode));
	sprintf(szGetNoteAmt, "%d", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[0].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szGetNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 타자행이체건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_117X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x117X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[1].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 타자행이체금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_118X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];

	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x118X, sizeof(pArrDataField->byFCode));
	sprintf(szGetNoteAmt, "%d", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[1].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szGetNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 총서비스이체건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_122X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x122X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[2].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 총서비스이체금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_123X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];
	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x123X, sizeof(pArrDataField->byFCode));
	sprintf(szGetNoteAmt, "%d", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[2].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szGetNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 총 입금 건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_128X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x128X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 총 입금 금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_129X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];
	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x129X, sizeof(pArrDataField->byFCode));
	sprintf(szGetNoteAmt, "%d", Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szGetNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_124X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];

	memset(szGetNoteAmt, 0x30, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x124X, sizeof(pArrDataField->byFCode));
	sprintf(szTmp, "%3.3s", "004");	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_125X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];

	memset(szGetNoteAmt, 0x30, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x125X, sizeof(pArrDataField->byFCode));
	sprintf(szTmp, "%3.3s", "008");	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 126X - 권종 설정 정보
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_126X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x126X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "6", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 이전 시제출금 합계일자
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_132X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%s", IniGetStr(_SP_BRM_CSTINFO_INI, "REFILL", "HOST_DATETIME_LAST", "00000000"));
	memcpy(pArrDataField->byFCode, x132X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "008", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 이전 시제출금 합계시각
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_133X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%s", IniGetStr(_SP_BRM_CSTINFO_INI, "REFILL", "HOST_DATETIME_LAST", "000000"));
	memcpy(pArrDataField->byFCode, x133X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, &szTemp[8], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 현재 시제출금 합계일자
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_134X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%s", GetDate().GetBuffer(0));
	memcpy(pArrDataField->byFCode, x134X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "008", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 이전 시제출금 합계시각
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_135X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%s", GetTime().GetBuffer(0));
	memcpy(pArrDataField->byFCode, x135X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 136X - Cassette 1 총방출매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_136X(int nTranCode , int nTxRxOption)
{
	int nRB1Cnt = 0;
	int nRB2Cnt = 0;
	int nRB3Cnt = 0;
	int nRB4Cnt = 0;
	int nDepRB1Cnt = 0;
	int nDepRB2Cnt = 0;
	int nDepRB3Cnt = 0;
	int nDepRB4Cnt = 0;
	int nCurRB1Cnt = 0;
	int nCurRB2Cnt = 0;
	int nCurRB3Cnt = 0;
	int nCurRB4Cnt = 0;
	int nRB1DispCnt = 0;
	int nRB2DispCnt = 0;
	int nRBRetract = 0;
	int nRBReject = 0;


	nRB1DispCnt = RegGetInt(_REGKEY_BRMSTATUS, "NDisp50000", 0);
	if (nRB1DispCnt < 0)
		nRB1DispCnt = 0;

	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%04d", nRB1DispCnt);
	memcpy(pArrDataField->byFCode, x136X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 137X - Cassette 2 총방출매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_137X(int nTranCode , int nTxRxOption)
{
	int nRB1Cnt = 0;
	int nRB2Cnt = 0;
	int nRB3Cnt = 0;
	int nRB4Cnt = 0;
	int nDepRB1Cnt = 0;
	int nDepRB2Cnt = 0;
	int nDepRB3Cnt = 0;
	int nDepRB4Cnt = 0;
	int nCurRB1Cnt = 0;
	int nCurRB2Cnt = 0;
	int nCurRB3Cnt = 0;
	int nCurRB4Cnt = 0;
	int nRB1DispCnt = 0;
	int nRB2DispCnt = 0;
	int nRBRetract = 0;
	int nRBReject = 0;


	nRB2DispCnt = RegGetInt(_REGKEY_BRMSTATUS, "NDisp10000", 0);
	if (nRB2DispCnt < 0)
		nRB2DispCnt = 0;
	
	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%04d", nRB2DispCnt);
	memcpy(pArrDataField->byFCode, x137X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	


	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 140X - Cassette 1 총회수매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_140X(int nTranCode , int nTxRxOption)
{

	int nRB1Cnt = 0;
	int nRB2Cnt = 0;
	int nRB3Cnt = 0;
	int nRB4Cnt = 0;
	int nDepRB1Cnt = 0;
	int nDepRB2Cnt = 0;
	int nDepRB3Cnt = 0;
	int nDepRB4Cnt = 0;
	int nCurRB1Cnt = 0;
	int nCurRB2Cnt = 0;
	int nCurRB3Cnt = 0;
	int nCurRB4Cnt = 0;
	int nRB1DispCnt = 0;
	int nRB2DispCnt = 0;
	int nRBRetract = 0;
	int nRBReject = 0;
	
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{	
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //미수취
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //미수취
	}

	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%04d", nRBRetract);
	memcpy(pArrDataField->byFCode, x140X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	


	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 141X - Cassette 2 총회수매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_141X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	int nRBReject = 0;
	char szTemp[1024];

	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //리젝트 - 불량 
	}
	else
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //리젝트 - 불량 
	}
	sprintf(szTemp, "%04d", nRBReject);
	memcpy(pArrDataField->byFCode, x141X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 45X - 해외카드 합계
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_145X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x145X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[4].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 45X - 해외카드 합계
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_146X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];

	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x146X, sizeof(pArrDataField->byFCode));
	sprintf(szGetNoteAmt, "%d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[4].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szGetNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 147X - 현금미수취 총건수 (구 Cash Not Taken)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_147X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x147X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[3].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 148X - 현금미수취 총금액 (구 Cash Not Taken)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_148X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szDepNoteAmt[1024];

	memset(szDepNoteAmt, 0x00, sizeof(szDepNoteAmt));
	memcpy(pArrDataField->byFCode, x148X, sizeof(pArrDataField->byFCode));
	sprintf(szDepNoteAmt, "%d", Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[3].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szDepNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szDepNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 149X - 출금셔터부 에러 총건수 ( 구 Undetermined Bills)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_149X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x149X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[2].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 150X - 출금셔터부 에러 총금액 (구 Undetermined Bills)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/


int CTranCmn::fnAPP_SetDataField_Item_150X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szDepNoteAmt[1024];

	memset(szDepNoteAmt, 0x00, sizeof(szDepNoteAmt));
	memcpy(pArrDataField->byFCode, x150X, sizeof(pArrDataField->byFCode));
	sprintf(szDepNoteAmt, "%d", Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[2].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szDepNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szDepNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 151X - 통신장애 총건수 ( 구 Send Fail)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_151X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x151X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[0].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 152X - 통신장애 총금액 (구 Send Fail)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_152X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szDepNoteAmt[1024];

	memset(szDepNoteAmt, 0x00, sizeof(szDepNoteAmt));
	memcpy(pArrDataField->byFCode, x152X, sizeof(pArrDataField->byFCode));
	sprintf(szDepNoteAmt, "%d", Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[0].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szDepNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szDepNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 153X - 출금계수 장애 총건수 (구 Dispense Fail)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_153X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x153X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[1].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 154X - 출금계수 장애 총금액 (구 Dispense Fail)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_154X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szDepNoteAmt[1024];

	memset(szDepNoteAmt, 0x00, sizeof(szDepNoteAmt));
	memcpy(pArrDataField->byFCode, x154X, sizeof(pArrDataField->byFCode));
	sprintf(szDepNoteAmt, "%d", Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[1].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szDepNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szDepNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_155X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_156X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 157X - 지폐방출 총건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_157X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x157X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[7].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 158X - 지폐방출 총금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_158X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];

	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x158X, sizeof(pArrDataField->byFCode));
	sprintf(szGetNoteAmt, "%d", Asc2Int(&m_pProfile->WITHTOTAL.DATA[7].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szGetNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_159X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%04d", 0);
	memcpy(pArrDataField->byFCode, x159X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_160X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%04d", 0);
	memcpy(pArrDataField->byFCode, x160X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 161X - 계좌이체/서비스이체/T-Money제휴현금이체&마일리지충전 총건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_161X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x161X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[3].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 162X - 계좌이체/서비스이체/T-Money제휴현금이체&마일리지충전 총금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_162X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szGetNoteAmt[1024];

	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x162X, sizeof(pArrDataField->byFCode));
	sprintf(szGetNoteAmt, "%d", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[3].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szGetNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szGetNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 165X - 기기 프로그램 버전
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_165X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteAmt[1024];
	char szApVer[20];
	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x165X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "008", sizeof(pArrDataField->byFLen));

	
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		sprintf(szApVer, "%8.8s", APVERSION);	
		szApVer[0] = '1';
		if(m_pProfile->DEVICE.EarHandler == TRUE && m_pProfile->DEVICE.Proximity == TRUE && m_pProfile->DEVICE.SightHandicapped == TRUE )
			szApVer[4] = '1';
	}
	else
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		sprintf(szApVer, "%8.8s", APVERSION);
	}
	else
		sprintf(szApVer, "%8.8s", APVERSION);

	
	memcpy(pArrDataField->byFData, szApVer, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}


/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 174X - 입금통신장애 총 금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_174X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szDepNoteAmt[1024];

	memset(szDepNoteAmt, 0x00, sizeof(szDepNoteAmt));
	memcpy(pArrDataField->byFCode, x174X, sizeof(pArrDataField->byFCode));
	sprintf(szDepNoteAmt, "%d", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[0].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szDepNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szDepNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 175X - 입금계수장애 총 건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_175X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x175X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[1].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 176X - 입금계수장애 총 금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_176X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szDepNoteAmt[1024];

	memset(szDepNoteAmt, 0x00, sizeof(szDepNoteAmt));
	memcpy(pArrDataField->byFCode, x176X, sizeof(pArrDataField->byFCode));
	sprintf(szDepNoteAmt, "%d", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[1].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szDepNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szDepNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 177X - 입금수납장애 총 건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_177X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x177X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[2].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	


	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 178X - 입금수납장애 총 금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_178X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szDepNoteAmt[1024];

	memset(szDepNoteAmt, 0x00, sizeof(szDepNoteAmt));
	memcpy(pArrDataField->byFCode, x178X, sizeof(pArrDataField->byFCode));
	sprintf(szDepNoteAmt, "%d", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[2].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szDepNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szDepNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 179X - 입금미수취 총 건수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_179X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteCnt[1024];
	memset(szGetNoteCnt, 0x00, sizeof(szGetNoteCnt));
	memcpy(pArrDataField->byFCode, x179X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	sprintf(szGetNoteCnt, "%04d", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[3].Count, 10));
	memcpy(pArrDataField->byFData, szGetNoteCnt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 180X - 입금미수취 총 금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_180X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	char szDepNoteAmt[1024];

	memset(szDepNoteAmt, 0x00, sizeof(szDepNoteAmt));
	memcpy(pArrDataField->byFCode, x180X, sizeof(pArrDataField->byFCode));
	sprintf(szDepNoteAmt, "%d", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[3].Amount, 10));
	sprintf(szTmp, "%03d", strlen(szDepNoteAmt));	
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szDepNoteAmt, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 182X - 만원권 총 방출매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_182X(int nTranCode , int nTxRxOption)
{

	int nRB2DispCnt = 0;
	nRB2DispCnt= RegGetInt(_REGKEY_BRMSTATUS, "NDisp10000", 0);
	
	if (nRB2DispCnt < 0)
		nRB2DispCnt = 0;

	DataField *pArrDataField = new DataField;

	char szTemp[1024];
	sprintf(szTemp, "%04d", nRB2DispCnt);

	
	memcpy(pArrDataField->byFCode, x182X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	


	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 183X - 5만원권 총 방출매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_183X(int nTranCode , int nTxRxOption)
{

	int nRB1DispCnt = 0;
	nRB1DispCnt= RegGetInt(_REGKEY_BRMSTATUS, "NDisp50000", 0);
	
	if (nRB1DispCnt < 0)
		nRB1DispCnt = 0;

	DataField *pArrDataField = new DataField;

	char szTemp[1024];
	sprintf(szTemp, "%04d", nRB1DispCnt);
	
	memcpy(pArrDataField->byFCode, x183X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 184X - 10만원권 총 방출매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_184X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szGetNoteAmt[1024];
	memset(szGetNoteAmt, 0x00, sizeof(szGetNoteAmt));
	memcpy(pArrDataField->byFCode, x184X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, ZERO4, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 185X - 만원권 총 입금매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_185X(int nTranCode , int nTxRxOption)
{
	int nRB1Cnt = 0;
	int nRB2Cnt = 0;
	int nDepRB1Cnt = 0;
	int nDepRB2Cnt = 0;
	int nDepRB3Cnt = 0;
	int nCurRB1Cnt = 0;
	int nCurRB2Cnt = 0;
	int nRB1DispCnt = 0;
	int nRB2DispCnt = 0;
	
	//초기설정금액 -- 
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{	
		nDepRB1Cnt = 0;
		nDepRB2Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000");
		nDepRB3Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_10000");
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		nDepRB2Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);

	}
	DataField *pArrDataField = new DataField;
	char szNoteDep[1024];
	memset(szNoteDep, 0x00, sizeof(szNoteDep));
	sprintf(szNoteDep, "%04d", (nDepRB1Cnt+nDepRB2Cnt+nDepRB3Cnt));
	memcpy(pArrDataField->byFCode, x185X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_185X (%s) %d		",szNoteDep, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	memcpy(pArrDataField->byFData, szNoteDep, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 186X - 5만원권 총 입금매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_186X(int nTranCode , int nTxRxOption)
{
	int nRB1Cnt = 0;
	int nRB2Cnt = 0;
	int nDepRB1Cnt = 0;
	int nDepRB2Cnt = 0;
	int nDepRB3Cnt = 0;
	int nCurRB1Cnt = 0;
	int nCurRB2Cnt = 0;
	int nRB1DispCnt = 0;
	int nRB2DispCnt = 0;
	
	//초기설정금액 -- 
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{	
	   	nDepRB3Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000");
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		nDepRB3Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0);

	}

	DataField *pArrDataField = new DataField;
	char szNoteDep[1024];
	memset(szNoteDep, 0x00, sizeof(szNoteDep));
	sprintf(szNoteDep, "%04d", nDepRB3Cnt);
	
	memcpy(pArrDataField->byFCode, x186X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szNoteDep, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 187X - 10만원권 총 입금매수
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_187X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szNoteDep[1024];
	int nDepRB3Cnt = 0;
	memset(szNoteDep, 0x00, sizeof(szNoteDep));
	sprintf(szNoteDep, "%04d", nDepRB3Cnt);
	
	memcpy(pArrDataField->byFCode, x187X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szNoteDep, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	


	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 188X - 기타권종 총 입금금액
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_188X(int nTranCode , int nTxRxOption)
{
	int nDepRB1000Cnt = 0;
	int nDepRB5000Cnt = 0;
	
	//초기설정금액 -- 
	DataField *pArrDataField = new DataField;
	char szNoteDep[1024];
	

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{	
   		nDepRB5000Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_5000");  //#0088
   		nDepRB1000Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_1000");
		memset(szNoteDep, 0x00, sizeof(szNoteDep));
		sprintf(szNoteDep, "%08d", (nDepRB1000Cnt*1000 + nDepRB5000Cnt*5000) );
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
   		nDepRB5000Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_5000"); //#0088
   		nDepRB1000Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_1000");
		memset(szNoteDep, 0x00, sizeof(szNoteDep));
		sprintf(szNoteDep, "%08d", (nDepRB1000Cnt*1000 + nDepRB5000Cnt*5000) );

	}


	memcpy(pArrDataField->byFCode, x188X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "008", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szNoteDep, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	
	

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_317X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%04d", 0);
	memcpy(pArrDataField->byFCode, x317X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_318X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%04d", 0);
	memcpy(pArrDataField->byFCode, x318X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
int CTranCmn::fnAPP_SetDataField_Item_319X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTemp[1024];
	sprintf(szTemp, "%04d", 0);
	memcpy(pArrDataField->byFCode, x319X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTemp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 910X - 브랜드제휴 설정
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_910x(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x910x, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));
	int nCdType = IniGetInt(_DEVICE_INI, DEV_SEC, "ETCDevice20", 0);

	int BrandJehyu = IniGetInt(_TRANS_INI, TRANS_SEC, "BrandJehyu", 0);

	if(nCdType == KIUP)	// 기업브랜드
	{
		memcpy(pArrDataField->byFData, "003", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if(nCdType == NONGHYUP)	// 농협브랜드 #0041
	{
		if(BrandJehyu == NONGHYUPDAN)	// #N0284
		{
			memcpy(pArrDataField->byFData, "012", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
		else
		{
			memcpy(pArrDataField->byFData, "011", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		}
	}
	else
	if(nCdType == KEB)	    // 외환브랜드 #0085
	{
		memcpy(pArrDataField->byFData, "005", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));		
	}
	else
	if(nCdType == KWANGJU)	    // 광주브랜드 #N0181
	{
		memcpy(pArrDataField->byFData, "034", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));		
	}
	else
	if(nCdType == KYONGNAM)	    // 경남브랜드 #N0192
	{
		memcpy(pArrDataField->byFData, "039", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));		
	}
	else
	if(nCdType == HANA)	    // 하나브랜드 #N0214 하나은행 청호이지캐쉬 내부 기관 코드
	{
		memcpy(pArrDataField->byFData, "080", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));		
	}
	else
	if(nCdType == SHINHAN)	    // 신한브랜드 #N0215
	{
		memcpy(pArrDataField->byFData, "088", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));		
	}
	else
	if(nCdType == SC)	    // 제일브랜드 #N0219
	{
		memcpy(pArrDataField->byFData, "023", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));		
	}
	else
	if(nCdType == KB)	    // 국민브랜드 #N0226 => 수정 004 => 006
	{
		memcpy(pArrDataField->byFData, "006", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));		
	}
	else
	if(nCdType == CITI)	    // 씨티브랜드 #N0238
	{
		memcpy(pArrDataField->byFData, "027", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));		
	}
	else
	if(nCdType == NICE)	    // 전자금융 #N0219
	{
		memcpy(pArrDataField->byFData, "096", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));		
	}
	else
		memcpy(pArrDataField->byFData, "096", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 911X - 기기설정정보
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_911x(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x911x, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 912X - 러쉬앤캐쉬 거래선택건수 집계
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_912X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x912X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "028", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2013.01.16
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 337X - 기기환경점검표
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_337X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x337x, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "014", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, Accept.JiroElecNum, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2013.01.16
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 338X - 조치자업체구분
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_338X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x338x, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));

	if (Accept.AuthFlag == 1)
		memcpy(pArrDataField->byFData, "1", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
	if (Accept.AuthFlag == 2)
		memcpy(pArrDataField->byFData, "2", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
		memcpy(pArrDataField->byFData, "0", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}
/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2013.01.16
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 339X - 조차자명(or 직원번호)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_339X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x339X, sizeof(pArrDataField->byFCode));
	sprintf(szTmp, "%03d", Accept.TelNumSize);
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, Accept.TelNum, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}





int CTranCmn::fnAPP_SetDataField_Item_040X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;

	memcpy(pArrDataField->byFCode, x040x, sizeof(pArrDataField->byFCode));	
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, ZERO4, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;	
}

//-1
int CTranCmn::fnAPP_SetDataField_Item_168X(int nTranCode , int nTxRxOption)
{

	if (memcmp(m_RD.byDataField_458x, "????", 4) == 0)
		return T_OK;
	
	DataField *pArrDataField = new DataField;

	memcpy(pArrDataField->byFCode, x458X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, m_RD.byDataField_458x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;	
}


int CTranCmn::fnAPP_SetDataField_Item_346X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;

	memcpy(pArrDataField->byFCode, x346X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "001", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, Accept.MuConfirmNum, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	
	return T_OK;	
}

//#N0174
int CTranCmn::fnAPP_SetDataField_Item_345X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;

	if(m_pProfile->DEVICE.ETCDevice20 == KEB && memcmp(m_RD.byDataField_345x, "PT", 2) == 0)
	{
		memcpy(pArrDataField->byFCode, x345X, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_RD.byDataField_345x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);
	}
	else
		delete pArrDataField;		
	return T_OK;	
}

//#N0184 신한카드 한도상향 데이터
int CTranCmn::fnAPP_SetDataField_Item_307X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;

	memcpy(pArrDataField->byFCode, x307X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "055", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, m_RD.byDataField_307x, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}


int CTranCmn::fnAPP_SetDataField_Item_914x(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x914x, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "002", sizeof(pArrDataField->byFLen));
	
	if(m_pProfile->DEVICE.MachineType == U3100K)	
	{
		if(m_pProfile->DEVICE.ETCDevice01 == RPCLOCK_SET1)
			memcpy(pArrDataField->byFData, "11", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		else
			memcpy(pArrDataField->byFData, "01", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		memcpy(pArrDataField->byFData, "00", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_916x(int nTranCode , int nTxRxOption)
{

	return T_OK;
}

int CTranCmn::fnAPP_SetDataField_Item_1721(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));

	memcpy(pArrDataField->byFCode, x1721, sizeof(pArrDataField->byFCode));
	Int2Asc(strlen(m_RD.byDataField_1721), szTmp, 3);
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, m_RD.byDataField_1721, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
		
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2015.07.16
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 통장증서발행번호(회차)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_387X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));

	memcpy(pArrDataField->byFCode, x387X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));
	if (memcmp(&PbMsData.sBuf[2], "1", 1) == 0)
	{
		memcpy(pArrDataField->byFData, &PbMsData.sBuf[18], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if (memcmp(&PbMsData.sBuf[2], "2", 1) == 0)
	{
		memcpy(pArrDataField->byFData, &PbMsData.sBuf[17], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if (memcmp(&PbMsData.sBuf[2], "+", 1) == 0)
	{
		sprintf(szTmp, "00%1.1s", &PbMsData.sBuf[17]);	
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		memcpy(pArrDataField->byFData, &PbMsData.sBuf[17], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;

}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2015.09.01
/* 
/* Input	     : TRCODE
/*
/* Desc	         : Rebooting 사유정보
	1)	UNKNOWN									(000)===>Default
	2)	Rebooting 시간이 도래해서 리부팅한경우	(001)
	3)	회선장애로 인해서 리부팅한경우			(002)
	4)	프로그램배포로 인해서 리부팅한경우		(003)
	5)	개국및 상태오류로 인해서 리부팅한경우	(004)
	6)	nibs+에서 원격제어로 Power off			(005)
	7)	nibs+에서 원격제어로 Reboot				(006)
	8)	프로그램 오류로 인해서 리부팅한경우		(007)
	9)	기기장애로 인해서 리부팅한경우			(008) 
	 ex)기기장애를 복구하기 위해서 리부팅(카드리더부 장애, pinpad장애,,,,)
	10) 기기설정에 의해 리부팅한경우			(009)
	11) 관리자모드에서 리부팅 버튼에 의한 리부팅(010)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_390X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	int nCMD_REBOOT = IniGetInt(_TRANS_INI, TRANS_SEC, "atmsCmdReboot", 0);
	memset(szTmp, 0x20, sizeof(szTmp));
	memcpy(pArrDataField->byFCode, x390X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "003", sizeof(pArrDataField->byFLen));

	if (nCMD_REBOOT == ATMS_CMD_REBOOT)
	{
		memcpy(pArrDataField->byFData, "001", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if (nCMD_REBOOT == ATMS_CMD_REBOOT1)
	{
		memcpy(pArrDataField->byFData, "003", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if (nCMD_REBOOT == ATMS_CMD_REBOOT2)
	{
		memcpy(pArrDataField->byFData, "002", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if (nCMD_REBOOT == ATMS_CMD_REBOOT3)
	{
		memcpy(pArrDataField->byFData, "010", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if (nCMD_REBOOT == ATMS_CMD_REBOOT4)
	{
		memcpy(pArrDataField->byFData, "009", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if (nCMD_REBOOT == ATMS_CMD_REBOOT5)
	{
		memcpy(pArrDataField->byFData, "006", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	if (nCMD_REBOOT == ATMS_CMD_REBOOT6)
	{
		memcpy(pArrDataField->byFData, "005", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		memcpy(pArrDataField->byFData, "000", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}

	if(TranCode2 == TRANID_A020)
		IniSetInt(_TRANS_INI, TRANS_SEC, "atmsCmdReboot", 0);

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2015.09.01
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 통신장애 내용 필드
	1)	회선장애일 경우 Socket Error Code
	2)	기기장애로 인해서 리부팅일 경우 기기 장애 코드

/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_388X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	int nErrorCode = IniGetInt(_TRANS_INI, TRANS_SEC, "nErrorCode", 0);

	memset(szTmp, 0x20, sizeof(szTmp));

	memcpy(pArrDataField->byFCode, x388X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "020", sizeof(pArrDataField->byFLen));

	if (nErrorCode)
	{
		Sprintf(szTmp, 5, "%5.5d", nErrorCode);
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		//다음 상태전문에는 소켓장애코드 송신 안하도록 하기 위함.
		if(TranCode2 == TRANID_A020)
			IniSetInt(_TRANS_INI, TRANS_SEC, "nErrorCode", 0);
	}
	else
		memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);	

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2015.09.01
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 기기모델정보 필드
	가)	
	X 필드(30byte)
	제조사(3자리)_기종대분류(3자리)_모델명(24자리)
	* 윙코 pc1500 serial type에 FDK일 경우
	Ex) WNK-CD_-PC1500_SERIAL_FDK
	* 효성 3100K 일경우
	Ex) HYS-ATM-U3100K

	[뒤 모델명 24자리는 자유롭게 활용가능]
	* 네오 kcd 46NI NEO DIP
	Ex)NEO-CD_-KCD_46NI_NEODIP_TUCH

	* HKEY_CURRENT_USER\Software\ANGARA\AGR_SharedData\SSTConfig\_SSTType에도 셋팅

/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_389X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));


	memcpy(pArrDataField->byFCode, x389X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "030", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_353X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_354X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_355X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_356X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_357X(int nTranCode , int nTxRxOption)
{
	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2016.03.03
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 휴대폰번호 
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/
int CTranCmn::fnAPP_SetDataField_Item_379X(int nTranCode , int nTxRxOption)
{
	DataField *pArrDataField = new DataField;
	memcpy(pArrDataField->byFCode, x379X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "012", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, SPACE12, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2015.09.01
/* 
/* Input	     : TRCODE
/*
/* Desc	         : 출금취소사유	445X	STR	n	N	출금사유 분석 위한 출금취소사유 (전문사양서 참조)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_445X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));


	memcpy(pArrDataField->byFCode, x445X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}



/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2015.09.01
/* 
/* Input	     : TRCODE
/*
/* Desc	         : //이자율구분
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_459X(int nTranCode , int nTxRxOption)
{

	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));


	memcpy(pArrDataField->byFCode, x459X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "004", sizeof(pArrDataField->byFLen));

	if (nTranCode == TRANID_7380 || nTranCode == TRANID_7530)
		memcpy(pArrDataField->byFData, "4330", Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	else
		memcpy(pArrDataField->byFData, ZERO4, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2015.09.01
/* 
/* Input	     : TRCODE
/*
/* Desc	         : //신용카드IC관련정보
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_470X(int nTranCode , int nTxRxOption)
{

	if(!(MenuAvail & MENU_MCU2)) 
		return T_OK;

	if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranProc == TRAN_FOREGIN || TranProc == TRAN_FOREGIN_INQ))
		return T_OK;


	DataField *pArrDataField = new DataField;
	int nIdx = 0;
	char szTmp[1024];
	char szPad[1024];
	char szEMVszData[IC_DATA_LEN*2];

	int		nOffset(0);
	int		nSaveOffset(0);
	

	memset(szPad, 0x20, sizeof(szPad));
	memset(szTmp, 0x20, sizeof(szTmp));
	memset(szEMVszData, 0x20, sizeof(szEMVszData));
	memcpy(pArrDataField->byFCode, x470X, sizeof(pArrDataField->byFCode));

	switch(CardBank)
	{
	case  nEnCmd126_192:
		nIdx = 321;
		break;
	case  nEnCmd107_107:
		nIdx = 127;
		break;
	case  nEnCmd111_111:
		nIdx = 646;
		break;
	case  nEnCmd127_127:
		nIdx = 621;
		break;
	case  nEnCmd194_194:
		nIdx = 621;
		break;
	case  nEnCmd120_120:
		nIdx = 621;
		break;
	case  nEnCmd181_105:
		nIdx = 606;
		break;
	default:
		nIdx = 192;
		break;
	}
//	nIdx = 192;
// 	//**************************************************************************/
// 	/*	[IC CARD인 경우 IC 신용카드 정보] -> Kim.Gi.Jin -> 내부검사 소스 1차 완료
// 		1. 001 - 006 : Terminal Capability Profile(9F33)
// 		2. 007 - 016 : Terminal Verification Result(95)
// 		3. 017 - 024 : Unpredictable Number(9F37)
// 		4. 025 - 040 : Cryptogram (ARQC, TC, AAC) (9F26)
// 		5. 041 - 044 : Application Transaction Counter(9F36)
// 		6. 045 - 048 : Application Interchange Profile(82)
// 		7. 049 - 050 : Cryptogram Transaction Type(9C)
// 		8. 051 - 054 : Terminal Country Code(9F1A)
// 		9. 055 - 060 : Terminal Transaction Date(9A)
// 		10.061 - 072 : Cryptogram Amount(9F02)
// 		11.073 - 076 : Cryptogram Currency Code(5F2A)
// 		12.077 - 088 : Cryptogram Cashback Amount(9F03)
// 		13.089 - 090 : Cryptogram Information Data(9F27)
// 		14.091 - 092 : Card Sequence Number(5F34)
// 		15.093 - 094 : Terminal Type(9F35)
// 		16.095 - 102 : Amount in Ref. Currency(9F3A)
// 		17.103 - 106 : Transaction Ref. Currency(9F3C)
// 		18.107 - 110 : Data Authentication Code(9F45)
// 		19.111       : M/chip version 
// 				       (1-M/Chip2 old, 2-M/Chip2 new, 4-M/chip4)
// 		20.112 - 179 : Issuer Application Data(9F10) 
// 		   1) 112 - 113 : Length Indicator ( 2byte ) 
// 		   2) 114 - 115 : Derivation Key Index (2byte)
// 		   3) 116 - 117 : Cryptogram Version Nr.(2byte)
// 		   4) 118 - 129 : Card Verification Result(12byte)
// 		   5) 130 - 179 : Issuer Discretionary Data
// 			  a) Length Indicator2 (1byte)
// 			  b) Data(var.)
// 		21.180 -192 : reserved */
// 	//**************************************************************************/
// 
	sprintf(szTmp, "%03d", nIdx);
	memcpy(pArrDataField->byFLen, szTmp, sizeof(pArrDataField->byFLen));


	if (EMVReadFlg)
	{
		memcpy(pArrDataField->byFData, &m_pDevCmn->m_EMVszData[0], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}
	else
	{
		memcpy(pArrDataField->byFData, &m_pDevCmn->m_EMVszData[0], Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	}


// 	memcpy(pArrDataField->byFLen, "192",sizeof(pArrDataField->byFLen));
//	"60402080800400008182C3FD712E7D55382C014501205C0001041016031100000000000004100000000000008000010103A0A00000001400000000000000000                                                                 "
// 	memset(szTmp, 0x20, sizeof(szTmp));
// 	memcpy(szTmp, "6040008000040000D31EAC49E14D64F9C133938700065800300410160318000000000000041000000000000080000510A000012204001400000000000000004 ", strlen("6040008000040000D31EAC49E14D64F9C133938700065800300410160318000000000000041000000000000080000510A000012204001400000000000000004 "));
//	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));
	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	return T_OK;	
}

/**********************************************************************************************************/
/* Function Name : DataField Make - Kim.G.J
/* 
/* Date			 : 2015.09.01
/* 
/* Input	     : TRCODE
/*
/* Desc	         : ARPC
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/

int CTranCmn::fnAPP_SetDataField_Item_471X(int nTranCode , int nTxRxOption)
{

	if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranProc == TRAN_FOREGIN || TranProc == TRAN_FOREGIN_INQ))
		return T_OK;


	DataField *pArrDataField = new DataField;
	char szTmp[1024];
	memset(szTmp, 0x30, sizeof(szTmp));

	memcpy(pArrDataField->byFCode, x471X, sizeof(pArrDataField->byFCode));
	memcpy(pArrDataField->byFLen, "006", sizeof(pArrDataField->byFLen));
	memcpy(pArrDataField->byFData, szTmp, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

	fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);


	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_4721(int nTranCode , int nTxRxOption)
{
	if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranProc == TRAN_FOREGIN || TranProc == TRAN_FOREGIN_INQ))
	{
		DataField *pArrDataField = new DataField;
		char szTmp[1024];
		memset(szTmp, 0x30, sizeof(szTmp));

//		memcpy(m_STXEncBuff.byChiperMsdata, working, 16);

		memcpy(pArrDataField->byFCode, x4721, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_STXEncBuff.byChiperMsdata, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	}

	return T_OK;	
}

int CTranCmn::fnAPP_SetDataField_Item_4751(int nTranCode , int nTxRxOption)
{

	if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranProc == TRAN_FOREGIN || TranProc == TRAN_FOREGIN_INQ))
	{
		DataField *pArrDataField = new DataField;
		char szTmp[1024];
		memset(szTmp, 0x30, sizeof(szTmp));
//		memcpy(m_STXEncBuff.byEncPasswd, DesRPIN, 16);

		memcpy(pArrDataField->byFCode, x4751, sizeof(pArrDataField->byFCode));
		memcpy(pArrDataField->byFLen, "016", sizeof(pArrDataField->byFLen));
		memcpy(pArrDataField->byFData, m_STXEncBuff.byEncPasswd, Asc2Int((char *)pArrDataField->byFLen, sizeof(pArrDataField->byFLen)));

		fnAPP_SetAddArrDataField(pArrDataField , nTxRxOption);

	}


	return T_OK;	
}