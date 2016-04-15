/////////////////////////////////////////////////////////////////////////////
// TranDev.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <wchar.h>
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
//	��ü�Լ�
/////////////////////////////////////////////////////////////////////////////

// CheckDeviceAction ó���� �̻�ó���� �߰�Ȯ�κ�
#define		__MATOUT_RETRY_SUPPORT__							// ��ü������߰�ó��

// ���ۿϷ����&��ְ���
int CTranCmn::fnAPD_CheckDeviceAction(int ChkID, int ErrChkFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeviceAction(ChkID[%x], ErrChkFlg[%d])", ChkID, ErrChkFlg);

	int		nErrorDevice = DEV_NONE;

	// #NICCM01	ICCM ��� ����
	if ((m_pDevCmn->IfmDirect)						&&
		(ChkID & DEV_MCU)							&&
		(m_pDevCmn->AtmDefine.CashHandler == HBRM_SET))
	{
		m_pDevCmn->fnFIC_CheckIFMError(m_pDevCmn->IfmResult);
	}


	ChkID &= m_pDevCmn->fnAPL_GetAvailDevice(ChkID);			// ��밡������ġ���ϱ�
	m_pDevCmn->fnAPL_CheckDeviceAction(ChkID);					// ���۴�����ְ���
	if (ErrChkFlg)
	{
#ifdef __MATOUT_RETRY_SUPPORT__
		nErrorDevice = m_pDevCmn->fnAPL_GetAvailErrorDevice(ChkID); 
/******************************************************************************************///#N0138
		if (BRM_IN_CSM_STACK == ErrChkFlg)
		{
			if (nErrorDevice & DEV_BRM)
			{
				if(m_pProfile->DEVICE.MachineType == U3100K) //#N0138
				{
					m_pDevCmn->BrmCsmIn = FALSE;
					if (m_pDevCmn->fnBRM_GetMaterialInfo() & BRM_IN_CSM)
					{
						m_pDevCmn->BrmCsmIn = BRM_IN_CSM;
					}
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranWithOutputProc - fnBRM_GetMaterialInfo (%d)", m_pDevCmn->BrmCsmIn);		
				}
			}
		}
/******************************************************************************************/
		if (nErrorDevice)
			fnAPD_MaterialOutRetryAfterError(nErrorDevice);
#else
		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(ChkID))		// ����ϰ���ֳ���ġ���ϱ�
			fnAPP_CancelProc(T_ERROR);
#endif
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeviceAction(...):return");
	return T_OK;
}

// ī�����Ը�弳��
int CTranCmn::fnAPD_CardEnDisable(int Action, int InitFlg, int WaitFlg, int ContinueFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardEnDisable(Action[%d], InitFlg[%d], WaitFlg[%d])", Action, InitFlg, WaitFlg);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_OK;

	if (Action == ENABLE)
	{
		if ((InitFlg) ||
			(!(DevEnDisable & DEV_MCU)))
		{
			DevEnDisable |= DEV_MCU;
			fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_MCU, ENABLE);	// ��ġ���Ը�弳��
			if (WaitFlg)
				fnAPD_CheckDeviceAction(DEV_MCU);
			if (!ContinueFlg)
				m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
		}
	}
	else
	{
		if ((InitFlg) ||
			(DevEnDisable & DEV_MCU))
		{
			DevEnDisable &= (~DEV_MCU);
			fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_MCU, DISABLE);
			if (WaitFlg)
				fnAPD_CheckDeviceAction(DEV_MCU);
			if (!ContinueFlg)
				m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardEnDisable(...):return");
	return T_OK;
}

// ICī�����Ը�弳��
int CTranCmn::fnAPD_ICCardEnDisable(int Action, int InitFlg, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_ICCardEnDisable(Action[%d], InitFlg[%d], WaitFlg[%d])", Action, InitFlg, WaitFlg);
	int nTmpInitFlg = 0;
	int nTmpWaitFlg = 0;

	nTmpInitFlg = InitFlg;
	nTmpWaitFlg = WaitFlg;


	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_OK;

	if (Action == ENABLE)
	{
		if ((InitFlg) ||
			(!(DevEnDisable & DEV_MCU)))
		{
			DevEnDisable |= DEV_MCU;
			fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnMCU_ICEntryEnable();					// ��ġ���Ը�弳��
			if (WaitFlg)
				fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
			
			RegSetInt(_REGKEY_MCUINFO, "EMBOSSREADDONE", 0);
		}
	}
	else
	{
		if ((InitFlg) ||
			(DevEnDisable & DEV_MCU))
		{
			DevEnDisable &= (~DEV_MCU);
			fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnMCU_ICEntryDisable();
			if (WaitFlg)
				fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_ICCardEnDisable(...):return");
	return T_OK;
}


// �������Ը�弳��
int CTranCmn::fnAPD_PbEnDisable(int Action, int InitFlg, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbEnDisable(Action[%d], InitFlg[%d], WaitFlg[%d])", Action, InitFlg, WaitFlg);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	if(m_pDevCmn->PBBarLineReading)								// ������ڵ� �д� �κ� �ڿ��� ó�� �ϱ� ����
		return T_OK;

	if (Action == ENABLE)
	{
		if ((InitFlg) ||
			(!(DevEnDisable & DEV_PBM)))
		{
			DevEnDisable |= DEV_PBM;
			fnAPD_CheckDeviceAction(DEV_PBM);
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PBM, ENABLE);	// ��ġ���Ը�弳��
			if (WaitFlg)
				fnAPD_CheckDeviceAction(DEV_PBM);
			m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_ON);
		}
	}
	else
	{
		if ((InitFlg) ||
			(DevEnDisable & DEV_PBM))
		{
			DevEnDisable &= (~DEV_PBM);
			fnAPD_CheckDeviceAction(DEV_PBM);
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PBM, DISABLE);
			if (WaitFlg)
				fnAPD_CheckDeviceAction(DEV_PBM);
			m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_OFF);
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbEnDisable(...):return");
	return T_OK;
}

// ī���б�&üũ
int CTranCmn::fnAPD_CardReadCheck(LPCTSTR strInCardData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck()");

	CString			strCardInfo("");
	CString			strCardData("");
	CStringArray	strCardDataArray;
	int				i = 0;

	if (!strInCardData)											// �ڷ�������
	{
		if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
			return T_OK;
	}

/////////////////////////////////////////////////////////////////////////////
	// V03-01-01 ī��ó���� SPL LEDó������(SCDP��������)
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_CARD_MAT, TRUE);
/////////////////////////////////////////////////////////////////////////////
		
	do 
	{
		CardTran = MENU_NULL;									// Card�ŷ����º���
		ICCardTran = MENU_NULL;									// Card�ŷ����º���
		CardBank = 0;											// ����ī������
		CardService = 0;										// ī������
		CardNew = 0;											// �ű�ī������
		memset(&CardData, 0, sizeof(CardData));					// �ʱ�ȭ
		strCardDataArray.RemoveAll();

		if (!strInCardData)										// �ڷ�������
		{
			fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnMCU_Read();
			fnAPD_CheckDeviceAction(DEV_MCU);
			strCardData = m_pDevCmn->fstrMCU_GetCardData();		// ī���ڷᱸ�ϱ�
		}
		else
		{	
			strCardData = strInCardData;
		}
		SplitString(strCardData, ",", strCardDataArray);

		if (strCardDataArray.GetSize() >= 1)
		{
			if (strCardDataArray[0].GetLength())
			{
				CardData.TrInfo |= ISO1_TRACK;
				CardData.ISO1size = __min(sizeof(CardData.ISO1Buff) * 2, strCardDataArray[0].GetLength()) / 2;
				MakePack(strCardDataArray[0].GetBuffer(0), CardData.ISO1Buff, __min(sizeof(CardData.ISO1Buff) * 2, strCardDataArray[0].GetLength()));
			}
		}
		if (strCardDataArray.GetSize() >= 2)
		{
			if (strCardDataArray[1].GetLength())
			{
				CardData.TrInfo |= ISO2_TRACK;
				CardData.ISO2size = __min(sizeof(CardData.ISO2Buff) * 2, strCardDataArray[1].GetLength()) / 2;
				MakePack(strCardDataArray[1].GetBuffer(0), CardData.ISO2Buff, __min(sizeof(CardData.ISO2Buff) * 2, strCardDataArray[1].GetLength()));
			}
		}
		if (strCardDataArray.GetSize() >= 3)
		{
			if (strCardDataArray[2] != "")
			{
				CardData.TrInfo |= ISO3_TRACK;
				CardData.ISO3size = __min(sizeof(CardData.ISO3Buff) * 2, strCardDataArray[2].GetLength()) / 2;
				MakePack(strCardDataArray[2].GetBuffer(0), CardData.ISO3Buff, __min(sizeof(CardData.ISO3Buff) * 2, strCardDataArray[2].GetLength()));
			}
		}
if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)                       // ���Ȼ�
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:CardData", &CardData, sizeof(CardData));

		if (!strInCardData)										// �ڷ�������
		{
			if (!(CardData.TrInfo & (ISO2_TRACK | ISO3_TRACK)))
			{
/////////////////////////////////////////////////////////////////////////////
				CString		strTempDbData("");
				CString		strTemp("");

				// �̵��Read����
				// OM_MEDIAREADERR||
				strTempDbData = "OM_MEDIAREADERR";

				// ��ֹ߻���(YYYYMMDD)||
				strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
				strTempDbData += "||" + strTemp;
				
				// ��ֹ߻��ð�(HHMMSS)||
				strTemp.Format("%6.6s", GetTime().GetBuffer(0));
				strTempDbData += "||" + strTemp;
				
				// ��ü����(0-ī��,1-����,2-��ǥ,3-IC)
				strTempDbData += "||0";
				
				m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
				break;
			}
		}
		else
		{
			if (!(CardData.TrInfo & (ISO2_TRACK | ISO3_TRACK)))
				break;
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:TrInfo[%d]", CardData.TrInfo);

		if (CardData.TrInfo & ISO3_TRACK)						// ISO3 Track �������ִ°��
		{
			if ((CardData.ISO3size < 104)								||
				(CardData.ISO3size > sizeof(CardData.ISO3Buff))			||
				(FindChar(CardData.ISO3Buff, CardData.ISO3size, NULL) != CardData.ISO3size)
																		||
				(!IsUnPack(CardData.ISO3Buff, CardData.ISO3size))		||
				(!IsNum(pCardData3->BankNo, 3))							||
//				(!IsZero(pCardData3->BankNo, 1))				// �����̾ƴѰ�칮���߻� : 2003.10.03
				(IsZero(pCardData3->BankNo, 3))							||
//				(IsZero(pCardData3->AccountNo, 16))						||	// ��������ī�� ���˹����߻�
				(IsZero(CardData.ISO3Buff, 104))						||
				(IsChar(CardData.ISO3Buff, 104, '=')))
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:TrInfo1[%d]", pCardData3->OtherData[7]);
			}
			else
			if(memcmp(pCardData3->AccountNo, NINE16, 16) == 0)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "���� : ������ <**:%s>",NINE16);
				CardTran |= MENU_T3_NOT;
			}
			else
			if(IsZero(pCardData3->AccountNo, 16))
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "���� : ������ <**:%s>",ZERO16);
				CardTran |= MENU_T3_NOT;
			}
			else
			{
		
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:TrInfo2[%d]", pCardData3->OtherData[7]);

				if (memcmp(pCardData3->FormatCode, BANKID, 4) == 0 ||
					memcmp(pCardData3->FormatCode, STOCKID, 4) == 0)
				{												// ����ī��

					// 2009.06.11 ����
					// �������	
					if (memcmp(pCardData3->BankNo, KDBBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = KDBBANK;					
					}
																// �������
					if (memcmp(pCardData3->BankNo, KIUBANKID,  3) == 0)
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == KIUP)
							CardTran |= MENU_J3;	
						else
							CardTran |= MENU_T3;	

						CardBank = KIUBANK;		
					}		
																// ��ȯ���� 
					if (memcmp(pCardData3->BankNo, KOEBANKID,  3) == 0)
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == HANA || m_pDevCmn->AtmDefine.ETCDevice20 == KEB)		//#N0246
							CardTran |= MENU_J3;	
						else
							CardTran |= MENU_T3;
						
						CardBank = KOEBANK;					
		
					}
																// ���� 
					if (memcmp(pCardData3->BankNo, SUHBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SUHBANK;					
					}
																// �������� 
																// ��������
					if (memcmp(pCardData3->BankNo, KOFBANKID,  3) == 0)
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == SC)		//#N0219
							CardTran |= MENU_J3;
						else
							CardTran |= MENU_T3;						
						CardBank = KOFBANK;					
					}
																// �뱸���� 
					if (memcmp(pCardData3->BankNo, DEABANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = DEABANK;					
					}
																// �λ����� 
					if (memcmp(pCardData3->BankNo, PUSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = PUSBANK;					
					}
																// �������� 
					if (memcmp(pCardData3->BankNo, KAWBANKID,  3) == 0)
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == KWANGJU)   
							CardTran |= MENU_J3;	
						else
							CardTran |= MENU_T3;
					
						CardBank = KAWBANK;					
						CardBankFlag = KAWBANK;
					}
																// �������� 
					if (memcmp(pCardData3->BankNo, JEJBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = JEJBANK;					
					}
																// �������� 
					if (memcmp(pCardData3->BankNo, JEOBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = JEOBANK;					
					}
																// �泲���� 
					if (memcmp(pCardData3->BankNo, KYOBANKID,  3) == 0)
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == KYONGNAM)   //#N0192
							CardTran |= MENU_J3;
						else
							CardTran |= MENU_T3;					
						CardBank = KYOBANK;					
					}
																// �������ݰ�  
					if (memcmp(pCardData3->BankNo, KFCBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = KFCBANK;					
					}
																// ���� 
					if (memcmp(pCardData3->BankNo, SINBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SINBANK;					
					}
																// ��ȣ���� 					
					if (memcmp(pCardData3->BankNo, SANBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SANBANK;					
					}
																// ��Ƽ���� 
					if (memcmp(pCardData3->BankNo, CITBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = CITBANK;					
					}
																// H.S.B.C 
					if(memcmp(pCardData3->BankNo, HSBABNKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = HSBABNK;					
					}
					
																// ��������, ��������		
					if ((memcmp(pCardData3->BankNo, SHIBANKID,  3) == 0)	||
						(memcmp(pCardData3->BankNo, CHOBANKID,  3) == 0)    ||
						(memcmp(pCardData3->BankNo, SHIBANKID1, 3) == 0))
					{										
						if(m_pDevCmn->AtmDefine.ETCDevice20 == SHINHAN)		//#N0215
							CardTran |= MENU_J3;
						else
							CardTran |= MENU_T3;					
						CardBank = Asc2Int(pCardData3->BankNo,3);					
					}						
																 // ��������
					if ((memcmp(pCardData3->BankNo, KUMBANKID1, 3) == 0)	||
						(memcmp(pCardData3->BankNo, KUMBANKID2, 3) == 0)	||
						(memcmp(pCardData3->BankNo, KUMBANKID3, 3) == 0)	||
						(memcmp(pCardData3->BankNo, KUMBANKID4, 3) == 0)	||
						(memcmp(pCardData3->BankNo, KUMBANKID5, 3) == 0))
					{										
						if(m_pDevCmn->AtmDefine.ETCDevice20 == KB)		//#N0226
							CardTran |= MENU_J3;	
						else
							CardTran |= MENU_T3;					
						CardBank = Asc2Int(pCardData3->BankNo,3);					
					}
																 // ���� 
					if ((memcmp(pCardData3->BankNo, NONBANKID1, 3) == 0)	||
						(memcmp(pCardData3->BankNo, NONBANKID3, 3) == 0))
					{
//						CardTran |= MENU_T3;		//#0041			
						CardBank = Asc2Int(NONBANKID1,3);	
						if(m_pDevCmn->AtmDefine.ETCDevice20 == NONGHYUP)
							CardTran |= MENU_J3;	
						else
							CardTran |= MENU_T3;	
						
					}
					
					if  ((memcmp(pCardData3->BankNo, NONBANKID2, 3) == 0)	||
						(memcmp(pCardData3->BankNo, NONBANKID4, 3) == 0))
					{										
//						CardTran |= MENU_T3;		//#0041			
						CardBank = Asc2Int(NONBANKID2,3);
						if(m_pDevCmn->AtmDefine.ETCDevice20 == NONGHYUP)
							CardTran |= MENU_J3;	
						else
							CardTran |= MENU_T3;	
					}
																 // �츮���� 
					if ((memcmp(pCardData3->BankNo, WORBANKID1, 3) == 0)	||
						(memcmp(pCardData3->BankNo, WORBANKID2, 3) == 0)	||
						(memcmp(pCardData3->BankNo, WORBANKID3, 3) == 0)	||
						(memcmp(pCardData3->BankNo, WORBANKID4, 3) == 0))
					{										
						CardTran |= MENU_T3;					
						CardBank = Asc2Int(pCardData3->BankNo,3);					
					}
																 // �ϳ����� 
					if ((memcmp(pCardData3->BankNo, HANBANKID1, 3) == 0)	||
						(memcmp(pCardData3->BankNo, HANBANKID2, 3) == 0)	||
						(memcmp(pCardData3->BankNo, HANBANKID3, 3) == 0)	||
						(memcmp(pCardData3->BankNo, HANBANKID4, 3) == 0))
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == HANA || m_pDevCmn->AtmDefine.ETCDevice20 == KEB)		//#N0246
							CardTran |= MENU_J3;
						else
							CardTran |= MENU_T3;					
						CardBank = Asc2Int(pCardData3->BankNo,3);					
					}
																 // �ѹ����� 
					if ((memcmp(pCardData3->BankNo, GOOBANKID1, 3) == 0)	||
						(memcmp(pCardData3->BankNo, GOOBANKID2, 3) == 0))
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == CITI)		//#N0238
							CardTran |= MENU_J3;
						else
							CardTran |= MENU_T3;					
						CardBank = Asc2Int(pCardData3->BankNo,3);					
					}
																// ��ü��
					if ((memcmp(pCardData3->BankNo, POSBANKID1, 3) == 0)	||
						(memcmp(pCardData3->BankNo, POSBANKID2, 3) == 0)    ||
						(memcmp(pCardData3->BankNo, POSBANKID3, 3) == 0)    ||
						(memcmp(pCardData3->BankNo, POSBANKID4, 3) == 0)    ||
						(memcmp(pCardData3->BankNo, POSBANKID5, 3) == 0))
					{										
						CardTran |= MENU_T3;					
						CardBank = Asc2Int(pCardData3->BankNo,3);					
					}

																// �������ձ�������
					if(memcmp(pCardData3->BankNo, TYJSBANKID,  3) == 0)// || CardBankFlag == MYASSET)
					{
						CardTran |= MENU_T3;					
						CardBank = TYJSBANK;					
						CardBankFlag = MYASSET;	
					}
																// ��������
					if(memcmp(pCardData3->BankNo, HYDSBANKID,  3) == 0)// || CardBankFlag == HYDSSTOCK)
					{
						CardTran |= MENU_T3;					
						CardBank = HYDSBANK;
						CardBankFlag = HYDSSTOCK;				
					}
																// �̷���������
					if(memcmp(pCardData3->BankNo, MRASBANKID,  3) == 0)// || CardBankFlag == MRASSTOCK)
					{
						CardTran |= MENU_T3;					
						CardBank = MRASBANK;	
						CardBankFlag = MRASSTOCK;	
						
					}
																// �������
					if(memcmp(pCardData3->BankNo, DAWSBANKID,  3) == 0)// || CardBankFlag == DAEWOO)
					{
						CardTran |= MENU_T3;					
						CardBank = DAWSBANK;					
						CardBankFlag = DAEWOO;
					}
																// �Ｚ����
					if(memcmp(pCardData3->BankNo, SASSBANKID,  3) == 0)// || CardBankFlag == SAMSSTOCK)
					{
						CardTran |= MENU_T3;					
						CardBank = SASSBANK;					
						CardBankFlag = SAMSSTOCK;
					}
																// �츮��������
					if(memcmp(pCardData3->BankNo, WRTSBANKID,  3) == 0)// || CardBankFlag == WOORISTOCK)
					{
						CardTran |= MENU_T3;					
						CardBank = WRTSBANK;					
						CardBankFlag = WOORISTOCK;
					}
																// �ѱ���������
					if(memcmp(pCardData3->BankNo, HKTSBANKID,  3) == 0)// || CardBankFlag == HKTSSTOCK)
					{
						CardTran |= MENU_T3;					
						CardBank = HKTSBANK;					
						CardBankFlag = HKTSSTOCK;
					}
																// ��������
					if(memcmp(pCardData3->BankNo, KYBSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = KYBSBANK;					
					}
																// ������������
					if(memcmp(pCardData3->BankNo, HISBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = HISBANK;					
					}
																// ����ġ������������
					if(memcmp(pCardData3->BankNo, HMCSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = HMCSBANK;					
					}
																// Ű������
					if(memcmp(pCardData3->BankNo, KIWSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = KIWSBANK;					
					}
																// ��Ʈ���̵�����
					if(memcmp(pCardData3->BankNo, ETRSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = ETRSBANK;					
					}
																// ������������
					if(memcmp(pCardData3->BankNo, SKSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SKSBANK;					
					}
																// �������
					if(memcmp(pCardData3->BankNo, DASSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = DASSBANK;		
						CardBankFlag = DASSSTOCK;	
						
					}
																// �ַθ���������
					if(memcmp(pCardData3->BankNo, SOLSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SOLSBANK;					
					}
																// ��ȭ����
					if(memcmp(pCardData3->BankNo, HAHSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = HAHSBANK;					
						CardBankFlag = HAHSSTOCK;
					}
																// �ϳ���������
					if(memcmp(pCardData3->BankNo, HNDSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = HNDSBANK;					
						CardBankFlag = HANASTOCK;
					}
																// �¸�׽�������
					if(memcmp(pCardData3->BankNo, GOMSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = GOMSBANK;	
						CardBankFlag = SHHSSTOCK;					
					}
																// ��������
					if(memcmp(pCardData3->BankNo, DOBSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = DOBSBANK;					
					}
																// ������������
					if(memcmp(pCardData3->BankNo, EUGSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = EUGSBANK;					
					}
																// �޸�Ʈ�߱�
					if(memcmp(pCardData3->BankNo, MRISBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = MRISBANK;					
						CardBankFlag = MERITZ;
					}
																// ������ġ��������
					if(memcmp(pCardData3->BankNo, NHTSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = NHTSBANK;					
					}
																// �α�����
					if(memcmp(pCardData3->BankNo, BOKSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = BOKSBANK;					
					}
																// �ſ�����
					if(memcmp(pCardData3->BankNo, SIYSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SIYSBANK;					
					}
																// ����������������
					if(memcmp(pCardData3->BankNo, LIGSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = LIGSBANK;					
					}					

					if (CardBank == 0)								// �ش���׹�
					{
						CardTran |= MENU_NULL;						// Ÿ������(3TR)
						CardBank = OTHBANK;							// ��Ÿī��
					}
					else
					{
						memcpy(Accept.AccountNum, &pCardData3->AccountNo[0], 16);
					}
				}
			}	
		
		}



		if (CardData.TrInfo & ISO2_TRACK)						// ISO2 Track �������ִ°��
		{		
			if ((CardData.ISO2size < 12)								||
				(CardData.ISO2size > sizeof(CardData.ISO2Buff))			||
				(FindChar(CardData.ISO2Buff, CardData.ISO2size, NULL) != CardData.ISO2size)
																		||
				(!IsUnPack(CardData.ISO2Buff, CardData.ISO2size))		||
				(!IsNum(CardData.ISO2Buff, 12))							||	// 2003.12.12 : 2Ʈ�� Numeric Check
				(IsZero(CardData.ISO2Buff, 12))							||
				(IsChar(CardData.ISO2Buff, 12, '=')))
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�ſ�ī�� : ������ <**:%2.2s>",CardData.ISO2Buff);
			}
			else
			if(memcmp(CardData.ISO2Buff, NOT_TRACK2, 4) == 0)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�ſ�ī�� : ������ <**:%s>","üũī��");
				CardTran |= MENU_T2_NOT;
			}
			else
			{
				CardTran |= MENU_J2T2;


				strCardInfo = fnAPP_Get_CreditOrgCode(pCardData2->MembershipNo);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�ſ�ī�� : [%s]-[%s]", strCardInfo, fnAPP_Get_CreditName(pCardData2->MembershipNo));
	
				CardBank = Asc2Int(strCardInfo);

				BYTE ServiceCode = '0';
				CardOnEMV = FALSE;
				
				if ('=' == CardData.ISO2Buff[16])				// �Ϲ�
					ServiceCode = CardData.ISO2Buff[21];
				else
				if ('=' == CardData.ISO2Buff[15])				// AMEX
				{
					CardService = AMEXCARD;
					ServiceCode = CardData.ISO2Buff[20];
				}
				else
				if ('=' == CardData.ISO2Buff[14])				// DINERS
				{
					CardService = DINERS;
					ServiceCode = CardData.ISO2Buff[19];
				}

				if (('2' == ServiceCode)	||
					('6' == ServiceCode)	)
					CardOnEMV = TRUE;	
			}
		}
	} while(FALSE);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:"
													"CardTran[%x], "		\
													"CardBank[%d], "		\
													"CardService[%d], "		\
													"CardNew[%d]",
													CardTran,
													CardBank,
													CardService,
													CardNew);

	if (MenuFICMode)											
	{
		CardTran   |= MENU_IC;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "<IC CARD MenuAvail(ReadCheck) = 0x%x>", MenuAvail);
	}
	else
	{
		CardTran   |= MSCardTran;
	}


	if (!CardTran)	
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("ī��     : ��� �Ұ� ī����"));

		if (!strInCardData)										// �ڷ�������
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			{
				fnAPP_CancelProc(T_MSG, "�� ī��� ��� �� �� �����ϴ�");
			}
			else 
				fnAPP_CancelProc(T_MSG, "This card is not valid");
		}
		else
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			{
				if (MenuIrdaMode)
					fnAPP_CancelProc(T_MSG, "�� �޴����� ��� �� �� �����ϴ�");
				else
				if (MenuAvail & MENU_IC)
				{
					if ((TranCode == TC_KCLOAD		)||			// ����
						(TranCode == TC_KCUNLOAD	)||			// ȯ��
						(TranCode == TC_KCREINQ		)||			// �ܾ���ȸ
						(TranCode == TC_KCBUYLST	)||			// ���ų�����ȸ
						(TranCode == TC_KCLOADRCV	)||			// ��������
						(TranCode == TC_KCUNLOADRCV	)||			// ȯ�Һ���
						(TranCode == TC_KCPARAKEY	)||			// �Ķ���Ͱ���(Key)
						(TranCode == TC_KCPARALIMIT ))			// �Ķ���Ͱ���(�ִ������ѵ�)
						fnAPP_CancelProc(T_MSG, "�� ����ȭ��ī��� ��� �� �� �����ϴ�");
					else
					{
						fnAPP_CancelProc(T_MSG, "�� ī��� ��� �� �� �����ϴ�");
					}
				}
				else
				{
					fnAPP_CancelProc(T_MSG, "�� ī��� ��� �� �� �����ϴ�");
				}
			}
			else 
				fnAPP_CancelProc(T_CANCEL);
		}
	}

	
	strCardInfo.Empty();
	if (CardTran & MENU_MCU2)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("ī��     : CREDIT READ2")); 

		strCardInfo.Format("%s",    (CardTran & MENU_J2)			? "����2" : "Ÿ��2");
		if (MenuIrdaMode)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][�ڵ���][%s]", strCardInfo.GetBuffer(0));
		else
		if (MenuFICMode)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][����IC][%s]", strCardInfo.GetBuffer(0));
		else
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][ī��][%s]", strCardInfo.GetBuffer(0));
	}
	if (CardTran & MENU_MCU3)
	{	
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("ī��     : BANK READ3")); 

		if(m_pDevCmn->AtmDefine.ETCDevice20 == KEB)
		{
			if (CardTran & MENU_T3)
			{
				fnAPP_MenuClearTranProc(0, TRAN_EASYONE, MENU_NULL);  //#N0174 Ÿ��ī�� �Է½� ������ ��ư ����
			}
		}
		/*//##0085 #N0273
		if(m_pDevCmn->AtmDefine.ETCDevice20 == NONGHYUP)
		{
			if (CardTran & MENU_T3)
				fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);
		}
		else
		if(m_pDevCmn->AtmDefine.ETCDevice20 == KEB) //#0085
		{
			if (CardTran & MENU_T3)
			{
				fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);
				fnAPP_MenuClearTranProc(0, TRAN_EASYONE, MENU_NULL);  //#N0174 Ÿ��ī�� �Է½� ������ ��ư ����
			}
		}
		else
		if(m_pDevCmn->AtmDefine.ETCDevice20 == KWANGJU) //#N0181
		{
			if (CardTran & MENU_T3)
				fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);
		}
		else
		if(m_pDevCmn->AtmDefine.ETCDevice20 == KYONGNAM) //#N0192
		{
			if (CardTran & MENU_T3)
				fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);
		}
		*/
		
		strCardInfo.Empty();
		strCardInfo.Format("%s",    (CardTran & MENU_J3)			? "����3" :
									(CardTran & MENU_S3)			? "����3" :
									(CardTran & MENU_J2T2)			? "�ſ�ī��" :
									(CardTran & MENU_IC)			? "����C" : "Ÿ��3");
		strCardInfo.Format("%s%s",  strCardInfo.GetBuffer(0),
									(CardBankFlag == MYASSET)		? "����Ÿ����" :			
									(CardBankFlag == DAEWOO)		? "�������" : 
									(CardBankFlag == WOORISTOCK)	? "�츮����" : 
									(CardBankFlag == SAMSSTOCK)		? "�Ｚ����" : 
									(CardBankFlag == MERITZ)		? "�޸�������" : 
									(CardBankFlag == MRASSTOCK)		? "�̷���������" : 
									(CardBankFlag == HANASTOCK)		? "�ϳ���������" : 
									(CardBankFlag == MRASSTOCK)		? "�̷���������" : 
									(CardBankFlag == DASSSTOCK)		? "�����������" : 
									(CardBankFlag == HYDSSTOCK)		? "��������" : 
									(CardBankFlag == JOINCARD)		? "����" : "��Ÿ"); 
		strCardInfo.Format("%sī��",strCardInfo.GetBuffer(0));
		if (MenuIrdaMode)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][�ڵ���][%s]", strCardInfo.GetBuffer(0));
		else
		if (MenuFICMode)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][����IC][%s]", strCardInfo.GetBuffer(0));
		else
		if (MenuEPBMode)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][��������][%s]", strCardInfo.GetBuffer(0));
		else
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][ī��][%s]", strCardInfo.GetBuffer(0));

		if (CardNew == NEWCARD)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][ī��][�űԹ߱�ī��]");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck():return");
	return T_OK;
}


// ICī���б�&üũ
int CTranCmn::fnAPD_ICCardReadCheck()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_ICCardReadCheck()");

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_CARD_MAT, TRUE);
/////////////////////////////////////////////////////////////////////////////

	fnAPD_CheckDeviceAction(DEV_MCU);
	m_pDevCmn->fnFIC_InitProcess();
	
	int nRet = m_pDevCmn->fnMCU_ICChipInitialize();
	if(nRet == R_ERROR)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("ICī��", "�νĽ��� (ICChipInitialize[%d])", nRet));	
		FallBackCnt = 0x30;
		FallBackEMVMode = TRUE;
		return T_ERROR;
	}

	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnKCS_AnalRecvData())
	{

		int nRet1 = m_pDevCmn->fnKCS_PowerOff();
		if(nRet1 == R_ERROR)
		{
			CardPowerOnFlg = FALSE;		
			fnAPP_CancelProc(T_MSG, "IC ī�� ���� �б� �����Դϴ�", "ó������ �ŷ��� �ٽ� �����Ͽ� �ֽñ� �ٶ��ϴ�");
		}

		fnAPD_CheckDeviceAction(DEV_MCU);

		m_pDevCmn->fnKCS_PowerOn();
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())
		{
			if ((memcmp(m_pDevCmn->m_cbFCError, "\xD2\x42\x30\xA2", 4))		&&
				(memcmp(m_pDevCmn->m_cbFCError, "\xD2\x43\x31\xFE", 4)))		
HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->m_cbFCError2", m_pDevCmn->m_cbFCError, sizeof(m_pDevCmn->m_cbFCError));


			CardPowerOnFlg = FALSE;								
			FallBackCnt = 0x30;									
			FallBackEMVMode = TRUE;								
			return T_ERROR;											
		}	
	}

	CardPowerOnFlg = TRUE;										// ī��������(����ȭ��)


	m_pDevCmn->FICCardType = FIC_OPEN;							// ������ ����

	m_pDevCmn->FICAppletType |= 0x01;
	m_pDevCmn->FICAppletType |= 0x02;
	m_pDevCmn->FICAppletType |= 0x04;
	m_pDevCmn->FICAppletType |= 0x08;


	if ((m_pDevCmn->FICAppletType & 0x0f) == 0x00)			
		return T_ERROR;

	if (!(m_pDevCmn->FICAppletType & 0x01))					
		return T_ERROR;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_ICCardReadCheck():return OK == m_pDevCmn->FICAppletType[%d]", m_pDevCmn->FICAppletType);
	return T_OK;
}

// �����б�&üũ -> 
int CTranCmn::fnAPD_PbReadCheck()
{

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck Brand (%d)", m_pProfile->DEVICE.ETCDevice20);

	if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
		fnAPD_IBK_PbReadCheck();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
		fnAPD_NONGHYUP_PbReadCheck();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KEB) 
		fnAPD_KEB_PbReadCheck();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU) 
		fnAPD_KJB_PbReadCheck();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == HANA) 
		fnAPD_HANA_PbReadCheck();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KB) 
		fnAPD_KB_PbReadCheck();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == CITI)
		fnAPD_CTB_PbReadCheck()	;
	else
	if(m_pProfile->DEVICE.ETCDevice20 == SC) 
		fnAPD_SC_PbReadCheck();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) 
		fnAPD_KNB_PbReadCheck();
	else
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : PASSBOOK READ FAIL")); 
		fnAPP_CancelProc(T_MSG, "�� ������ ��� �� �� �����ϴ�");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck(PbKind[%d]):return", PbKind);

	return T_OK;
}




int CTranCmn::fnAPD_NONGHYUP_PbReadCheck()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_NONGHYUP_PbReadCheck()");

	CString	strPbData("");
	CString	strBarData("");

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	int nPbReadFailed = FALSE;
/*
	char	szTempReadPbmMSCnt[256];
	char	szTempReadPbmNomalCnt[256];
	memcpy(szTempReadPbmMSCnt, m_pProfile->TRANS.ReadPbmMSCnt, 4);
	memcpy(szTempReadPbmNomalCnt, m_pProfile->TRANS.ReadPbmNomalCnt, 4);

	// ����ŷ� Total �Ǽ��� ���� ���� Read�� ������ Cnt ����
	if (memcmp(szTempReadPbmNomalCnt, "9999", 4) == 0)	
		memset(szTempReadPbmNomalCnt, '0', 4);
	AddString(szTempReadPbmNomalCnt, "0001", 4);
	// ���� Read ������ ��� (Total �Ǽ�)
	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.ReadPbmNomalCnt, szTempReadPbmNomalCnt, 4);
	m_pProfile->TRANS.ReadPbmNomalCnt[4] = NULL;
	m_pProfile->PutTransProfile();
	//<-- end of V07-00-01-#11
*/
	do 
	{
		PbTran = MENU_NULL;										// Pb�ŷ����º���
		memset(&PbMsData, 0, sizeof(PbMsData));					// �����ڷ�
		memset(&PbBarData, 0, sizeof(PbBarData));				// �����������ڷ�

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// ����Ms�ڷᱸ�ϱ�

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_NONGHYUP_PbReadCheck:PbMsData", &PbMsData.sBuf, PbMsData.Len);

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// ��������
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// �̵��Read����
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// ��ֹ߻���(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// ��ֹ߻��ð�(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// ��ü����(0-ī��,1-����,2-��ǥ,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
			nPbReadFailed = TRUE;

			break;
		}
				
		if ((PbMsData.Len != NAC_ACCNUM_SIZE1) &&
 			(PbMsData.Len != NAC_ACCNUM_SIZE2))					// ���嵥��Ÿ����üũ : ����(54 or 50)
 		{
			nPbReadFailed = TRUE;
 			PbTran = MENU_NULL;
 		}		
		else		
		if (PbMsData.Len > sizeof(PbMsData.sBuf))				// ����Ÿ���� Ȯ��
		{
			PbTran = MENU_NULL;
		}
		else
		if (!IsNum(PbMsData.sBuf, 14))							// ���¹�ȣ�ڸ�(MAX 14)
		{
			PbTran = MENU_NULL;
		}
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// ����Ÿ����
		{
			PbTran = MENU_NULL;
		}
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// ����Ÿ����
		{
			PbTran = MENU_NULL;
		}
		else
		if (!memcmp(&PbMsData.sBuf[41], "01", 2) == 0 &&				// �߾�����?
			!memcmp(&PbMsData.sBuf[41], "10", 2) == 0 &&				// ��������?
			!memcmp(&PbMsData.sBuf[48], "10", 2) == 0)				// V04-01-01-#01 
		{
			PbTran = MENU_NULL;
		}
/*
		else //#0074
		if (!memcmp(&PbMsData.sBuf[42], "1", 1) == 0 &&				// �߾�����?
			!memcmp(&PbMsData.sBuf[41], "1", 1) == 0 &&				// ��������?
			!memcmp(&PbMsData.sBuf[43], "1", 1) == 0 &&				// �߾�/��������?
			!memcmp(&PbMsData.sBuf[48], "1", 1) == 0)			    // ������

		{
			PbTran = MENU_NULL;
		}
*/		else													// �ذ����̻󹫡�
		{
			PbTran |= MENU_PB;
			PbBank  = NONBANK;
			CardBank = NONBANK;	
			//#0047
			//memcpy(Accept.AccountNum, pPbMsData->AccountNo, 13);
			if (memcmp(&PbMsData.sBuf[42], "1", 1) == 0) 				// �߾����� : 113���� : 5-2-6
			{
				memcpy(Accept.AccountNum, &PbMsData.sBuf[1], 13);
			}
			else
			if (memcmp(&PbMsData.sBuf[41], "1", 1) == 0) 				// �������� : 14���� : 6-2-6		
			{
				memcpy(Accept.AccountNum, &PbMsData.sBuf[0], 14);
			}
			else
			if (memcmp(&PbMsData.sBuf[43], "1", 1) == 0) 				// �������� : 14���� : 6-3-6		
			{
				memcpy(Accept.AccountNum, &PbMsData.sBuf[0], 15);

			}
			else
			if (memcmp(&PbMsData.sBuf[48], "1", 1) == 0 && memcmp(&PbMsData.sBuf[41], "000", 3) ==0)				// �������� : 14���� : 6-2-6		
			{
				memcpy(Accept.AccountNum, &PbMsData.sBuf[0], 13);
			}
			else
				memcpy(Accept.AccountNum, &PbMsData.sBuf[1], 13);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_NONGHYUP_PbReadCheck(PbBank[%d])", PbBank);
//////////////////////////////////////////////////////////////////////////
// Bar Line �б� : ���� YSH 2003.05.18
// �� ���� ������ ���� SET ���
//    1 Page : '11'
//    2 Page : '12'
//    3 Page : '13'
//    4 Page : '14'
//    5 Page : '15'
//    6 Page : '01'
//    7 Page : '02'
//    8 Page : '03'		9��������������
//    9 Page : '04'		9��������������
//    ó�� �Ǵ� ������������ : ����ó��
//////////////////////////////////////////////////////////////////////////
			fnAPD_CheckDeviceAction(DEV_PBM);
			m_pDevCmn->fnPBM_BarLineRead();						// �����������б�
			m_pDevCmn->PBBarLineReading = TRUE;					// ���� ���ڵ� �д� �κ� �ڷ� �̵�.

// 			fnAPD_CheckDeviceAction(DEV_PBM);
// 			strBarData = m_pDevCmn->fstrPBM_GetBarData();
// 			PbBarData.Len = __min(sizeof(PbBarData.sBuf) * 2, strBarData.GetLength()) / 2;
// 			MakePack(strBarData.GetBuffer(0), PbBarData.sBuf, __min(sizeof(PbBarData.sBuf) * 2, strBarData.GetLength()));
// MsgDumpAnal(TRACE_CODE_MODE, "Log", "[���������� = %s]", &PbBarData.sBuf[0]);
// 			if (PbBarData.sBuf[0] >= '1' && PbBarData.sBuf[0] <= '7')
// 			{
// 				if ((PbBarData.sBuf[0] == '6') || (PbBarData.sBuf[0] == '7'))
// 				{
// 					PbBarData.sBuf[0] -= 5;						// 
// 					Accept.PbPageInfo[0] = '0';
// 				}
// 				else 
// 					Accept.PbPageInfo[0] = '1';
// 
// 				Accept.PbPageInfo[1] = PbBarData.sBuf[0];
// 			}
// 			else
// 			{
// 				PbTran &= ~(MENU_PB);							// �����������̻�
// 			}
//////////////////////////////////////////////////////////////////////////
		}
	} while(FALSE);
	
	if (!PbTran)
	{

		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			// ���� Read���н� �ȳ����� ����
			if (nPbReadFailed == TRUE)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : PASSBOOK READ FAIL")); //#KIMH0143
				fnAPP_CancelProc(T_MSG, "�� ������ ���� �� �����ϴ�", "�������� �����Ͽ� �ֽʽÿ�");
			}
			else
			if (PbBarData.sBuf[0] == '0')						// ���ڿ��������ƴ�
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : ���ڿ� ������ ���ν�")); //#KIMH0143
				fnAPP_CancelProc(T_MSG, "�μ��� �������� ã�� �� �����ϴ�", "������ ���ڸ��� Ȯ���Ͻ� ��", "�ٽ� �ŷ��Ͽ� �ֽʽÿ�");
			}			
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : ��� �Ұ� ������")); //#KIMH0143
				fnAPP_CancelProc(T_MSG, "�� ������ ��� �� �� �����ϴ�", "�������� �����Ͽ� �ֽʽÿ�");
			}
		}
		else 
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : PASSBOOK READ FAIL")); //#KIMH0143

			if (PbBarData.sBuf[0] == '0')						// ���ڿ��������ƴ�
				fnAPP_CancelProc(T_MSG, "Please check the page where to print", "and try again.");
			else
				fnAPP_CancelProc(T_MSG, "This passbook is not valid", "Please check at a bank counter");
		}
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_NONGHYUP_PbReadCheck(PbBank[%d]):return", PbBank);
	return T_OK;
}


int CTranCmn::fnAPD_KJB_PbReadCheck()	// #N0181  ���� ����üũ
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KJB_PbReadCheck()");

	CString	strPbData("");
	char	szTemp[256] = {0, };								// V06-10-00-#02 AJMS

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, TRUE);
/////////////////////////////////////////////////////////////////////////////

	do 
	{
		PbTran = MENU_NULL;										// Pb�ŷ����º���
		memset(&PbMsData, 0, sizeof(PbMsData));					// �����ڷ�

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// ����Ms�ڷᱸ�ϱ�

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// ��������
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// �̵��Read����
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// ��ֹ߻���(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// ��ֹ߻��ð�(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// ��ü����(0-ī��,1-����,2-��ǥ,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if ((PbMsData.sBuf[0] < '0')	||
			(PbMsData.sBuf[0] > '9')	)
			break;
		
		if (PbMsData.Len > sizeof(PbMsData.sBuf))			// ����Ÿ���� Ȯ��
			PbTran = MENU_NULL;
		else
		if (!IsNum(&PbMsData.sBuf[1], 14))		        	// ���¹�ȣ�ڸ�
			PbTran = MENU_NULL;
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// ����Ÿ����
			PbTran = MENU_NULL;
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// ����Ÿ����
			PbTran = MENU_NULL;


		if (IsNum(PbMsData.sBuf, PbMsData.Len) &&
           !memcmp(&PbMsData.sBuf[3], KAWBANKID, 3))                             // ����M/S Read�� �����ȣ Check : ���ุ �ŷ�����
		{
			PbKind = PASSBOOK_NORMAL;
			PbTran |= MENU_PB;
			PbBank = KAWBANK;
			CardBank = KAWBANK;	
			CardBankFlag = KAWBANK;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[6], 12);
		}
		else
		{
			PbTran = MENU_NULL;
		}

//////////////////////////////////////////////////////////////////////////
// Bar Line �б�
// �� ���� ������ ���� SET ���
//    1 Page : '01'
//    2 Page : '02'
//    3 Page : '03'
//    4 Page : '04'
//    5 Page : '05'
//    6 Page : '06'
//    7 Page : '07'
//    8 Page : '08'
//    9 Page : '09'
//    ó�� �Ǵ� ������������ : ����ó��
//////////////////////////////////////////////////////////////////////////
		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_BarLineRead();						// �����������б�
		m_pDevCmn->PBBarLineReading = TRUE;					// ���� ���ڵ� �д� �κ� �ڷ� �̵�.

	} while(FALSE);
	

	if (!PbTran)	
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "�� ������ ��� �� �� �����ϴ�");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}

	MsgDumpAnal(TRACE_CODE_MODE, "APL", "[MATERIAL][����][%s����]",
									(PbKind == PASSBOOK_NORMAL)		 ? "�Ϲ�" : "��Ÿ");
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_DGB_PbReadCheck(PbKind[%d]):return", PbKind);
	return T_OK;
}


int CTranCmn::fnAPD_HANA_PbReadCheck()		//#N0214
{
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck()");

	CString	strPbData("");
	MS		PbMsTemp1;
	// ī�� ����Ÿ ���� ó�� V03-00-09 2003.05.26
	char	TempPbNum = (m_pProfile->TRANS.YYYYMMDD[7] & 0x0f);
	int		i = 0;

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	do 
	{
		PbTran = MENU_NULL;										// Pb�ŷ����º���
		memset(&PbMsData, 0, sizeof(PbMsData));					// �����ڷ�
//		StockPB = 0;											// ��������(1:��������)		// V01.00.30_#3
//		FundPB  = 0;											// �ݵ�����					// V06-33-00-#01

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// ����Ms�ڷᱸ�ϱ�

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck():return=[%d]",PbMsData.Len);

if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)				// ���Ȼ�		
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		int BankCheckPb = PbMsData.Len;
//		memcpy(&PbMsData, &PbMsData, sizeof(PbMsData));

		memcpy(&PbMsTemp1, &PbMsData, sizeof(PbMsData));

		for (i=0; i < PbMsData.Len;i++)
		{
			PbMsTemp1.sBuf[i] += TempPbNum;
		}

		if (((BankCheckPb != 36) && ((BankCheckPb != 18) )) ||		// ����
		    (PbMsData.Len < 14)						||			// ����
			(PbMsData.Len > sizeof(PbMsData.sBuf))	||
			(!IsNum(&PbMsData.sBuf[0], 14))			||			// ���¹�ȣ�ڸ�
			(IsZero(PbMsData.sBuf, 14))				||
			(IsChar(PbMsData.sBuf, 14, '=')))
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "����Ȯ��1");
		else 
		{														// V01.00.30_#3
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "����Ȯ��2");

			if (BankCheckPb == 18)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�ϳ�����");
				if (memcmp(&PbMsData.sBuf[12], "13", 2) == 0)	// V01.00.30_#3
				{
//					StockPB = 1;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck():���Ե� ������ ���� ��������[StockPB]");
				}

				if (memcmp(&PbMsData.sBuf[12], "12", 2) == 0)	// V06-33-00-#01
				{
//					FundPB = 1;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck():���Ե� ������ ���� �ݵ�����[FUNDPB]");
				}

				CardBankFlag = HANBANK;
				CardBank = HANBANK;	
				PbTran |= MENU_PB;		
				PbBank = HANBANK;
				
				memcpy(Accept.AccountNum, &PbMsData.sBuf[0], 14);
				/*
				if (memcmp(PbMsData.sBuf, &m_pProfile->NETWORK.BranchNum[5], 3) == 0)
					BranchKind = 1;
				else
					BranchKind = 2;
					*/
			}
			else
				;
		}														// V01.00.30_#3
	} while(FALSE);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.AccountNum [%s]", Accept.AccountNum);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck():PbTran[%x]", PbTran);

	if (!PbTran)	
	{
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "�� ������ ��� �� �� �����ϴ�");
		else													// ENG001 KYD
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck():return");

	return T_OK;
}



/////////////////////////////////////////////////////////////////////////////
static char* Gwamok_01[] =		//�����İ���
{"03", "07", "08", "13", "14", "21", "22", "23", "26", "27", "48", "53", "56"};

static char* Gwamok_02[] =		//��Ź���� - 24���� �߰�(�ؿ��ݵ�)
{"43", "45", "49", "51", "52", "54", "55", "57", "58", "24", "68"};

static char* Gwamok_03[] =		//�䱸�Ұ���
{"01", "02", "06", "25", "37", "18", "41", "42"};

static char* Gwamok_04[] =		//�������
{"09", "16", "29", "38", "44", "59"};				// V01-19-00-#02:����"38"���� �߰�

static char ChangeValue[][6] = { 
								"01 01",
								"21 02",
								"05 06",
								"04 18",
								"24 25",
								"25 37",
								"28 03",
								"41 23",
								"30 48",
								"29 56",
								"14 41",
								"13 42",
								"12 43",
								"11 49",
								"18 51",
								"15 52",
								"37 55",	// 2002.10.25
								"19 58",
								"51 16",
								"52 16",
								"57 44",
								"26 90" };


int CTranCmn::fnAPD_KB_PbReadCheck()   ////#N0226
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()");

	CString	strPbData("");
	BYTE	KMBCheck[2] = { 0x2d, 0x2e };						// "=>" window ��
	char	ChangeGwamok [2];
	KbTongBank = 0;
	KbGwamok = 0;	

	memset(ChangeGwamok, ' ', sizeof(ChangeGwamok));
	memset(Accept.AccountNum, ' ', sizeof(Accept.AccountNum));
	
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, TRUE);
/////////////////////////////////////////////////////////////////////////////

	do 
	{
		PbTran = MENU_NULL;										// Pb�ŷ����º���
		memset(&PbMsData, 0, sizeof(PbMsData));					// �����ڷ�

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// ����Ms�ڷᱸ�ϱ�

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)                       // ���Ȼ�
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// ��������
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// �̵��Read����
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// ��ֹ߻���(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// ��ֹ߻��ð�(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// ��ü����(0-ī��,1-����,2-��ǥ,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if ((PbMsData.Len < 14)						||			// ����
			(PbMsData.Len > sizeof(PbMsData.sBuf))	||
//			(!IsNum(&PbMsData.sBuf[0], 14))			||			// ���¹�ȣ�ڸ� (����)
			(IsZero(PbMsData.sBuf, 14))				||
			(IsChar(PbMsData.sBuf, 14, '=')))
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_1");
			PbTran = MENU_NULL;
		}
		else
		{
			if ((PbMsData.Len == 17) || (PbMsData.Len == 18)) 	// ������ (17�׽�Ʈ����:18) 																
			{
				PbTran = MENU_NULL; //�귣�� �� ���(���Ұ� ������)
				if (memcmp(&PbMsData.sBuf[12], KMBCheck, 2) == 0)
				{												// => (2D 2E)Ȯ��	
					KbTongBank = 2;

					for(int i=0; i < itemof(ChangeValue); i++) //// ����ȯ
					{
						if(memcmp(ChangeValue[i], &PbMsData.sBuf[3], 2) == 0)			//
						{
							memcpy(ChangeGwamok, &ChangeValue[i][3], 2);		// ��������
						}
					}

					KbGwamok = fnAPD_GwamokCheck_Kb(ChangeGwamok);			// ����üũ ??

					// ������
					memcpy(Accept.AccountNum, PbMsData.sBuf, 12);

					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_2");
				}
				else											// 17,18�� "=>" ������ reject
					KbGwamok = 0;
			}
			else 
			if ((PbMsData.Len == 39)							&& 
				(memcmp(&PbMsData.sBuf[36], "-", 1) == 0)		&&	// '='
				((memcmp(&PbMsData.sBuf[18], "06", 2) == 0) || //KUMBANKID2
				 (memcmp(&PbMsData.sBuf[18], "04", 2) == 0)))  //KUMBANKID1
			{
				// Ÿ��ü�� �ڵ� ����
				// '+' -> ';'
				if (PbMsData.sBuf[35] == 0x2b)  // '+' -> �׽�Ʈ ����, ';' -> ��������
					PbMsData.sBuf[35] = 0x3b;

				// '-' -> '='
				if (PbMsData.sBuf[36] == 0x2d)   // '-' -> �׽�Ʈ ����, '=' -> �������� 
					PbMsData.sBuf[36] = 0x3d;

				if (memcmp(&PbMsData.sBuf[18], "04", 2) == 0)// ������ ��߱�����
				{

					if (PbMsData.sBuf[21] == '1')				// ����������
					{
						KbTongBank = 4;								
						KbGwamok = 3;									// �䱸��
						memcpy(Accept.AccountNum, PbMsData.sBuf, 11);
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_1 Gwamok (%d) TongBank(%d) &PbMsData.sBuf[3](%3.3s) ChangeGwamok(%3.3s)", KbGwamok, KbTongBank, &PbMsData.sBuf[3], ChangeGwamok);

					}
					else
					{
						KbTongBank = 2;

						for(int i=0; i < itemof(ChangeValue); i++) //// ����ȯ
						{
							if(memcmp(ChangeValue[i], &PbMsData.sBuf[3], 2) == 0)			//
							{
								memcpy(ChangeGwamok, &ChangeValue[i][3], 2);		// ��������
							}
						}
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_2 Gwamok (%d) TongBank(%d) &PbMsData.sBuf[3](%3.3s) ChangeGwamok(%3.3s)", KbGwamok, KbTongBank, &PbMsData.sBuf[3], ChangeGwamok);
						KbGwamok = fnAPD_GwamokCheck_Kb(ChangeGwamok);	 // ����üũ ??
						memcpy(Accept.AccountNum, PbMsData.sBuf, 12);
					}
				}
				else											// 
				{
					KbTongBank = 3;								// ������������
					KbGwamok = fnAPD_GwamokCheck_Kb((char *)&PbMsData.sBuf[4]);	// ����üũ
					memcpy(Accept.AccountNum, PbMsData.sBuf, 14);
				}


				if (KbGwamok == 3)
				{
					CardBankFlag = KUMBANK;
					CardBank = KUMBANK;	
					PbTran |= MENU_PB;		
					PbBank = KUMBANK;
				}

				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_3 Gwamok (%d) TongBank(%d) AccNum(%16.16s)", KbGwamok, KbTongBank, Accept.AccountNum);
			}
			else
			{
				PbTran = MENU_NULL; //�귣�� �� ���(���Ұ� ������)

				// Ÿ��ü�� �ڵ� ����
				// '+' -> ';'
				if (PbMsData.sBuf[35] == 0x2b)
					PbMsData.sBuf[35] = 0x3b;

				// '-' -> '='
				if (PbMsData.sBuf[36] == 0x2d)
					PbMsData.sBuf[36] = 0x3d;


				KbTongBank = 1;									// ������
				KbGwamok = fnAPD_GwamokCheck_Kb((char *)&PbMsData.sBuf[4]);		// ����üũ

				// ������
				memcpy(Accept.AccountNum, PbMsData.sBuf, 14);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_4 Gwamok (%d) TongBank(%d)", KbGwamok, KbTongBank);
			}

			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_5 Gwamok (%d) TongBank(%d)", KbGwamok, KbTongBank);
		}

	} while(FALSE);


	if ((PbMsData.Len == 18) && (PbMsData.sBuf[17] == 0x2b))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_6 Gwamok (%d) TongBank(%d)", KbGwamok, KbTongBank);
		PbTran = NULL;											// �������׽�Ʈ����ź�
	}

	if (m_pDevCmn->BrmCashMode == BRM_REALCASH_MODE)
	{
		if ((PbMsData.Len == 39)						&& 
			(memcmp(&PbMsData.sBuf[36], "-", 1) == 0)	&&
			(PbMsData.sBuf[35] == 0x2b))						// �����׽�Ʈ����
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_7 Gwamok (%d) TongBank(%d)", KbGwamok, KbTongBank);
			PbTran = NULL;
		}
	}
	
    if (KbGwamok != 3)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_7 Gwamok (%d) TongBank(%d)", KbGwamok, KbTongBank);
		PbTran = NULL;
	}

	if (!KbGwamok)												// ������üũ���� ���Ѱ��
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_8 Gwamok (%d) TongBank(%d)", KbGwamok, KbTongBank);
		PbTran = NULL;
	}
	


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][����][%s����]", 
											(KbTongBank== 1)		 ? "������"		:
											(KbTongBank== 2)		 ? "������"		:
											(KbTongBank== 3)		 ? "���ձ���"	: "��Ÿ");
	if (!PbTran)	
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : ��� �Ұ� ")); //#KIMH0143
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "�ش���� �� ������ ��� �� �� �����ϴ�");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}

	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck(PbKind[%d]):return", PbKind);
	return T_OK;
}


int	CTranCmn::fnAPD_GwamokCheck_Kb(char * InGwamok)						// �������üũ (����)
{
	int		i;

	for(i = 0 ; i < itemof(Gwamok_01); i++ )
	{
		if(memcmp(Gwamok_01[i], InGwamok, 2) == 0)				// �����İ���
		{
			return 1;
		}
	}

	for(i = 0 ; i < itemof(Gwamok_02); i++ )
	{
		if(memcmp(Gwamok_02[i], InGwamok, 2) == 0)				// ��Ź����
		{
			return 2;
		}
	}

	for(i = 0 ; i < itemof(Gwamok_03); i++ )
	{
		if(memcmp(Gwamok_03[i], InGwamok, 2) == 0)				// �䱸�Ұ���
		{
			return 3;
		}
	}

	for(i = 0 ; i < itemof(Gwamok_04); i++ )
	{
		if(memcmp(Gwamok_04[i], InGwamok, 2) == 0)				// �������
		{
			return 4;
		}
	}

	// V01-28-00-#01:������������
	if (memcmp("89", InGwamok, 2) == 0)
	{
		return 8;
	}
	
	return 0;
}

int CTranCmn::fnAPD_CTB_PbReadCheck()	// #N0252
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CTB_PbReadCheck()");

	CString	strPbData("");
	char	szTemp[256] = {0, };								// V06-10-00-#02 AJMS

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, TRUE);
/////////////////////////////////////////////////////////////////////////////

	do 
	{
		PbTran = MENU_NULL;										// Pb�ŷ����º���
		memset(&PbMsData, 0, sizeof(PbMsData));					// �����ڷ�

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// ����Ms�ڷᱸ�ϱ�

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// ��������
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// �̵��Read����
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// ��ֹ߻���(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// ��ֹ߻��ð�(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// ��ü����(0-ī��,1-����,2-��ǥ,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if ((PbMsData.sBuf[0] < '0')	||
			(PbMsData.sBuf[0] > '9')	)
			break;
		
		if (PbMsData.Len > sizeof(PbMsData.sBuf))			// ����Ÿ���� Ȯ��
			PbTran = MENU_NULL;
		else
		if (!IsNum(&PbMsData.sBuf[1], 14))		        	// ���¹�ȣ�ڸ�
			PbTran = MENU_NULL;
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// ����Ÿ����
			PbTran = MENU_NULL;
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// ����Ÿ����
			PbTran = MENU_NULL;

		for (int i = 0; i < PbMsData.Len; i++)
		{
			if ((PbMsData.sBuf[i] < 0x20)	||
				(PbMsData.sBuf[i] > 0x39)	)	
				break;
		}

		PbMsData.Len = i;

		PbKind = PASSBOOK_NORMAL;
		PbTran |= MENU_PB;
		PbBank = GOOBANK;
		CardBank = GOOBANK;			// CITBANK
		CardBankFlag = GOOBANK;		// CITBANK
		memcpy(Accept.AccountNum, PbMsData.sBuf, min(PbMsData.Len, sizeof(Accept.AccountNum)));
	} while(FALSE);
	

	if (!PbTran)	
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : ��� �Ұ� ")); //#KIMH0143
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "�� ������ ��� �� �� �����ϴ�");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}
	else
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����", "�������� (����:��Ƽ(%d), ����:Ȯ�ξȵ�)", PbBank));			// #0379

	MsgDumpAnal(TRACE_CODE_MODE, "APL", "[MATERIAL][����][%s����]",
									(PbKind == PASSBOOK_NORMAL)		 ? "�Ϲ�" : "��Ÿ");
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CTB_PbReadCheck(PbKind[%d]):return", PbKind);
	return T_OK;
}


int CTranCmn::fnAPD_SC_PbReadCheck()   ////#N0245
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_SC_PbReadCheck()");

	CString	strPbData("");


	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	// V03-01-01 ī��ó���� SPL LEDó������(SCDP��������)
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, TRUE);

/////////////////////////////////////////////////////////////////////////////

	do 
	{
		PbTran = MENU_NULL;										// Pb�ŷ����º���
		memset(&PbMsData, 0, sizeof(PbMsData));					// �����ڷ�

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// ����Ms�ڷᱸ�ϱ�

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)                       // ���Ȼ�
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// ��������
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// �̵��Read����
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// ��ֹ߻���(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// ��ֹ߻��ð�(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// ��ü����(0-ī��,1-����,2-��ǥ,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if (((PbMsData.Len != 25) && (PbMsData.Len != 13))			||			// ����
			(PbMsData.Len > sizeof(PbMsData.sBuf))	||
			(!IsNum(&PbMsData.sBuf[3], 6)))
			;
		else
		{
			CardBankFlag = KOFBANK;
			CardBank = KOFBANK;	
			PbTran |= MENU_PB;		
			PbBank = KOFBANK;

			memcpy(Accept.AccountNum, &PbMsData.sBuf[3], 6);
		}
	} while(FALSE);
	
	
	if (!PbTran)	
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "�� ������ ��� �� �� �����ϴ�");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}

	if (fnAPD_PBBarCodeRead() == FALSE)				// BarCode �б� ����, �μ� �Ұ� PAGE 
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "BarCode �б� ����, �μ� �Ұ� PAGE PBM Return 0 !!!");
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PB Read OK ���� ���� (���ڵ� �־� ���� ����)!!!");
		PbTran = MENU_NULL;	
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranDevProc PbTran = MENU_NULL !!!");
		// 2010.10.14 �ҽ� ���� ���� ����  (MS Read ����, �� ���� ����)
		// 2010.10.14 ����� �˼��� �������� fnAPP_CancelProc �ŷ��� ��� �Ǿ����ϴ�. ���� ���� (�����ƾ�)
		fnAPP_CancelProc(T_MSG, "������ �μ��� ���� �ٸ��� �켭 �־��ֽʽÿ�");
	}
	else											// BarCode �б� ����, �μ� ���� PAGE 
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "BarCode �б� ����, �μ� ���� PAGE PBM Return 1 !!!");
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PB Read ERROR  (���ڵ� ���� ���� �ŷ�����)!!!");
	}
	
	MsgDumpAnal(TRACE_FREE_MODE, "AP", "[MATERIAL][����][%s����]",
									(PbKind == PASSBOOK_LOAN)	? "����" : 
									(PbKind == PASSBOOK_NORMAL)	? "�Ϲ�" : "��Ÿ");
	
	m_pDevCmn->fnPBM_BarLineRead();	
	m_pDevCmn->PBBarLineReading = TRUE;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_SC_PbReadCheck(PbKind[%d]):return", PbKind);
	return T_OK;
}

int CTranCmn::fnAPD_KNB_PbReadCheck()	// #N0277
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KNB_PbReadCheck()");

	CString	strPbData("");
	char	szTemp[256] = {0, };								// V06-10-00-#02 AJMS

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, TRUE);
/////////////////////////////////////////////////////////////////////////////

	do 
	{
		PbTran = MENU_NULL;										// Pb�ŷ����º���
		memset(&PbMsData, 0, sizeof(PbMsData));					// �����ڷ�

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// ����Ms�ڷᱸ�ϱ�

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// ��������
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// �̵��Read����
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// ��ֹ߻���(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// ��ֹ߻��ð�(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// ��ü����(0-ī��,1-����,2-��ǥ,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if (IsNull(PbMsData.sBuf, PbMsData.Len))		       // ���¹�ȣ�ڸ�
		{
			PbTran = MENU_NULL;
			break;
		}

		//if ((PbMsData.sBuf[0] < '0')	||
		//	(PbMsData.sBuf[0] > '9')	)
		//	break;
		
		if (PbMsData.Len > sizeof(PbMsData.sBuf))			// ����Ÿ���� Ȯ��
			PbTran = MENU_NULL;
		//else
		//if (!IsNum(&PbMsData.sBuf[1], 14))		        	// ���¹�ȣ�ڸ�
		//	PbTran = MENU_NULL;
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// ����Ÿ����
			PbTran = MENU_NULL;
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// ����Ÿ����
			PbTran = MENU_NULL;

		for (int i = 0; i < PbMsData.Len; i++)
		{
			if ((PbMsData.sBuf[i] < 0x20)	||
				(PbMsData.sBuf[i] > 0x39)	)	
				break;
		}

		PbMsData.Len = i;

		PbKind = PASSBOOK_NORMAL;
		PbTran |= MENU_PB;
		PbBank = KYOBANK;
		CardBank = KYOBANK;	
		CardBankFlag = KYOBANK;

		// �������� ���°� 13�ڸ� ��.
		if (memcmp(&PbMsData.sBuf[2], "1", 1) == 0)
		{
			memcpy(Accept.AccountNum, &PbMsData.sBuf[5], 13);
		}
		else
		{
			memcpy(Accept.AccountNum, &PbMsData.sBuf[5], 12);
		}
		
	} while(FALSE);
	

	if (!PbTran)	
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "�� ������ ��� �� �� �����ϴ�");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}
	else
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����", "�������� (����:�泲(%d), ����:Ȯ�ξȵ�)", PbBank));

	MsgDumpAnal(TRACE_CODE_MODE, "APL", "[MATERIAL][����][%s����]",
									(PbKind == PASSBOOK_NORMAL)		 ? "�Ϲ�" : "��Ÿ");
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KNB_PbReadCheck(PbKind[%d]):return", PbKind);
	return T_OK;
}

//#0039
int CTranCmn::fnAPD_PbSpeedUpCheck() 
{
	CString strBarData	= _T("");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbSpeedUpCheck()");

	if((m_pProfile->DEVICE.ETCDevice20 != NONGHYUP) &&		
		(m_pProfile->DEVICE.ETCDevice20 != KWANGJU) &&  //#N0181
		(m_pProfile->DEVICE.ETCDevice20 != SC))  //#N0245
		return T_OK;

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	if(m_pDevCmn->PBBarLineReading == FALSE)					
		return T_OK;


	if(PbTran)
	{
		fnAPD_CheckDeviceAction(DEV_PBM);
		strBarData = m_pDevCmn->fstrPBM_GetBarData();
		PbBarData.Len = __min(sizeof(PbBarData.sBuf) * 2, strBarData.GetLength()) / 2;
		MakePack(strBarData.GetBuffer(0), PbBarData.sBuf, __min(sizeof(PbBarData.sBuf) * 2, strBarData.GetLength()));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbSpeedUpCheck()[������ ���� = %1.1s]", &PbBarData.sBuf[0]);

		if (m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)			// ���� 9��������������
		{
			//if (PbBarData.sBuf[0] >= '1' && PbBarData.sBuf[0] <= '7')
			if (PbBarData.sBuf[0] >= '1' && PbBarData.sBuf[0] <= '9')	// 9��������������
			{
				//if ((PbBarData.sBuf[0] == '6') || (PbBarData.sBuf[0] == '7'))
				if ((PbBarData.sBuf[0] == '6') ||		// 1
					(PbBarData.sBuf[0] == '7') ||		// 2
					(PbBarData.sBuf[0] == '8') ||		// 3			// 9��������������
					(PbBarData.sBuf[0] == '9')		)	// 4			// 9��������������
				{
					PbBarData.sBuf[0] -= 5;
					Accept.PbPageInfo[0] = '0';
				}
				else 
					Accept.PbPageInfo[0] = '1';
				
				Accept.PbPageInfo[1] = PbBarData.sBuf[0];
			}
			else
			{
				//PbTran &= ~(MENU_PB);							// �����������̻� ==> #0079
				Accept.PbPageInfo[0] = '1';            
				Accept.PbPageInfo[1] = PbBarData.sBuf[0];
			}
		}
		else
		//if (m_pProfile->DEVICE.ETCDevice20 == KWANGJU)		// #0305
		{
			Int2Asc(Asc2Int(PbBarData.sBuf, PbBarData.Len),
					Accept.PbPageInfo, max(PbBarData.Len, 2));
		}
	
		m_pDevCmn->PBBarLineReading = FALSE;
	}
	else
	{
		m_pDevCmn->PBBarLineReading = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbSpeedUpCheck(PbTran=%d):return", PbTran);
		return T_OK;
	}
	
	if (!PbTran)
	{	
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : ���� �μ�� ������ �ν� �Ұ�")); //#KIMH0143
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "���� �μ�� �����͸� Ȯ������ ���Ͽ����ϴ�.", "�ٽ� �ŷ��Ͽ� �ֽʽÿ�");
		}
		else 
		{
			if (PbBarData.sBuf[0] == '0')						// ���ڿ��������ƴ�
				fnAPP_CancelProc(T_MSG, "Please check the page where to print", "and try again.");
			else
				fnAPP_CancelProc(T_MSG, "This passbook is not valid", "Please check at a bank counter");
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbSpeedUpCheck():return");

	return T_OK;
}


/*==============================================================================
* Function Name : fnAPD_PBBarCodeRead()
* Description   : ������ڵ� ���� 
* Parameters    : (NONE)
* Return        : T_OK
* Release       : Ver 1.0
* Notes			: 2010.09.03 // V03-19-03-#01 ������ڵ� ��������
* ============================================================================*/
int CTranCmn::fnAPD_PBBarCodeRead()   //#N0245
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PBBarCodeRead() IN");
	
	BOOL	bIsPbBarCode = FALSE;
	CString strPbPage("");
	char	PbBarCodeTemp[10] = {0, };
	//			H/W			PGU(CIS����)
	//	T2ATM	L-PBM-E		��(���ڵ���)
	//	GATM  	L-PBM-E		��(���ڵ���)
	//	U8100 	I-PBM		��(���ڵ���)
	//	T1ATM	L-PBM+		���ڵ� �� PAGE �̿� ���� ����
	
	// 	if ((m_pDevCmn->AtmDefine.PBHandler != PBM_SET) &&	
	// 		(m_pDevCmn->AtmDefine.PBHandler != I_PBM_SET))	
	// 		return FALSE;
	// 
	// 	if (m_pProfile->DEVICE.MachineType == U8100)		
	// 		return FALSE;
	
	memset(PbBarCodeTemp, 0, sizeof(PbBarCodeTemp));
	
	m_pDevCmn->fnPBM_BarLineRead();
	fnAPD_CheckDeviceAction(DEV_PBM);
	
	strPbPage = m_pDevCmn->fstrPBM_GetBarData();
	fnAPD_CheckDeviceAction(DEV_PBM);
	
	memcpy(PbBarCodeTemp, strPbPage.GetBuffer(0), strPbPage.GetLength());
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "������fnAPD_PBBarCodeRead [%s]������", strPbPage);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "������fnAPD_PBBarCodeRead [%s]������", PbBarCodeTemp);
	//	301E BAR�� �ִ� ù��� ������ 0(0x30) ������
	//	3110 ��Ÿ BAR�� ���� �������� 1(0x31) ����
	// - ���ڵ� ����� ���� ��� 
	// [2010/09/15 20:29:55:562 TranDevProc.cpp(07262)] ������fnAPD_PBBarCodeRead [301E]������
	// - ���ڵ� ����� ���� ��� 
	// [2010/09/15 20:30:51:417 TranDevProc.cpp(07263)] ������fnAPD_PBBarCodeRead [3110]������
	// 	1. V00.02.20_002
	// 		1) BASE : V00.02.10 (���� OPEN ��� 6001 ���� �ù�� ��)
	// 		2) ���� ����
	// 		- KFB BAR ����
	// 		: BAR�� �ִ� ù��� ������ 0(0x30) ������ ����
	// 		: ��Ÿ BAR�� ���� �������� 1(0x31) ����
	// 		- BAR RETRY ���� 1ȸ�� READ
	// 		- KFB USER SETTING ���� : BAR �߰�
	
	//	if (Asc2Int(&PbBarCodeTemp[1], 1) == 0)					
	if (Asc2Int(&PbBarCodeTemp[1], 1) == 1)					
		bIsPbBarCode = TRUE;
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PBBarCodeRead()::bIsPbBarCode[%d]", bIsPbBarCode);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PBBarCodeRead() OUT");
	
	return bIsPbBarCode;
	
}

int CTranCmn::fnAPD_IBK_PbReadCheck()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_IBK_PbReadCheck()");
	CString	strPbData("");
	char	szTemp[256] = {0, };								

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, TRUE);
/////////////////////////////////////////////////////////////////////////////

	do 
	{
		PbTran = MENU_NULL;										// Pb�ŷ����º���
		memset(&PbMsData, 0, sizeof(PbMsData));					// �����ڷ�

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// ����Ms�ڷᱸ�ϱ�

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// ��������
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// �̵��Read����
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// ��ֹ߻���(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// ��ֹ߻��ð�(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// ��ü����(0-ī��,1-����,2-��ǥ,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
			break;
		}

//		if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)
		if (m_pDevCmn->HostSvrPortMode == ECASH_LOCALPORT_MODE)
		{
			if (PbMsData.sBuf[0] == 0x2D)
			{
				memcpy(&PbMsData.sBuf[0], &PbMsData.sBuf[1], PbMsData.Len - 1);
				PbMsData.Len -= 1;
			}
		}

		if ((PbMsData.sBuf[0] < '0')	||
			(PbMsData.sBuf[0] > '9')	)
			break;
		
		if (PbMsData.Len > sizeof(PbMsData.sBuf))			// ����Ÿ���� Ȯ��
			PbTran = MENU_NULL;
		else
		if (!IsNum(&PbMsData.sBuf[1], 14))		        	// ���¹�ȣ�ڸ�
			PbTran = MENU_NULL;
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// ����Ÿ����
			PbTran = MENU_NULL;
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// ����Ÿ����
			PbTran = MENU_NULL;

		for (int i = 0; i < PbMsData.Len; i++)
		{
			if ((PbMsData.sBuf[i] < 0x20)	||
				(PbMsData.sBuf[i] > 0x39)	)	
				break;
		}

		if (i == 16 || (i >= 18 && i <= 20))
		{
			PbMsData.Len = i;

			if (PbMsData.sBuf[14] == 0x2D)
			{
//				PbMsData.Len = 14;
				PbKind  = PASSBOOK_NORMAL;
				PbTran |= MENU_PB;
				PbBank  = KIUBANK;
				CardBank = KIUBANK;	
				CardBankFlag = KIUBANK;				
				memcpy(Accept.AccountNum, pPbMsData->AccountNo, 14);

				if (memcmp(&PbMsData.sBuf[9], "96", 2) == 0)	// �������ǰ���
					PbKind = PASSBOOK_BENEFICERTI;
				else
				if (memcmp(&PbMsData.sBuf[9], "94", 2) == 0)	// ������������
					PbKind = PASSBOOK_RETIRE;
			}
			else
			if (PbMsData.Len >= 16)
			{
//				PbMsData.Len = 16;
//				PbKind = PASSBOOK_LOAN;
				PbTran |= MENU_PB;
				PbBank = KIUBANK;
				CardBank = KIUBANK;	
				CardBankFlag = KIUBANK;
				memcpy(Accept.AccountNum, pPbMsData->AccountNo, 16);
				//#0202
				if ((PbMsData.sBuf[9] == '3') && (PbMsData.sBuf[10] >= '1' && PbMsData.sBuf[10] <= '9') ) // ���� 31~39
					  PbKind = PASSBOOK_LOAN;                                                                                                              // ��������
				else 
				if (memcmp(&PbMsData.sBuf[9], "40", 2) == 0)                                     // ���� 40
					  PbKind = PASSBOOK_LOAN;                                                                                                              // ��������
				else 
				if (memcmp(&PbMsData.sBuf[9], "56", 2) == 0)                                     // ���� 56 - ��ȭ����
					  PbKind = PASSBOOK_FOREIGN;                                                                                                          // ��ȭ����
				else
					  PbTran = MENU_NULL;

			}
			else
			{
				PbTran = MENU_NULL;
			}
		}
		else
		if (i == 32)											// ������
		{
//			PbMsData.Len = i;

			if ((PbMsData.sBuf[14] == 0x2D)	&&
				(PbMsData.sBuf[18] == 0x2D)	&&
				(PbMsData.sBuf[19] == 0x30)	&&
				(PbMsData.sBuf[20] == 0x33)	&&
				(PbMsData.sBuf[21] == 0x2D))
			{
//				PbMsData.Len = 14;
				PbKind  = PASSBOOK_NORMAL;
				PbTran |= MENU_PB;
				PbBank  = KIUBANK;
				CardBank = KIUBANK;	
				CardBankFlag = KIUBANK;
				memcpy(Accept.AccountNum, pPbMsData->AccountNo, 14);
				
				//#0034
				if (memcmp(&PbMsData.sBuf[9], "96", 2) == 0)	// �������ǰ���
					PbKind = PASSBOOK_BENEFICERTI;
				else
				if (memcmp(&PbMsData.sBuf[9], "94", 2) == 0)	// ������������
					PbKind = PASSBOOK_RETIRE;
			}
			else
			{
				PbTran = MENU_NULL;
			}
		}

	} while(FALSE);
	

	if (PbKind == PASSBOOK_LOAN)							// ��������
	{
		if (TranCode != TC_PBOOK)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : ���������� ����")); //#KIMH0143
			fnAPP_CancelProc(T_MSG, "�� �������δ� ���������� �����մϴ�");
		}
	}
	else
	if (PbKind == PASSBOOK_BENEFICERTI)						// ������������
	{
		if ((TranCode != TC_DEPOSIT) && (TranCode != TC_PBOOK))    // �Ա� - ���������� �ŷ�����
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : ���� �ŷ� �Ұ�")); //#KIMH0143
			fnAPP_CancelProc(T_MSG, "�� �������δ� ���ŷ��� �� �� �����ϴ�");
		}
	}
	else
	if (PbKind == PASSBOOK_FOREIGN)	//#0034				
	{
		if (TranCode != TC_PBOOK)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : ���������� ����")); //#KIMH0143
			fnAPP_CancelProc(T_MSG, "�� �������δ� ���������� �����մϴ�");
		}
	}
	else
	if (PbKind == PASSBOOK_RETIRE)							// ������������ //#0202 -> ���� ���� �ٸ�. //#0037
	{
		if (TranCode != TC_PBOOK)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : ���������� ����")); //#KIMH0143
			fnAPP_CancelProc(T_MSG, "�� �������δ� ���������� �����մϴ�");
		}
	}

	if ((memcmp(&PbMsData.sBuf[9], "01", 2) == 0)	||
		(memcmp(&PbMsData.sBuf[9], "02", 2) == 0)	||
		(memcmp(&PbMsData.sBuf[9], "03", 2) == 0)	||
		(memcmp(&PbMsData.sBuf[9], "04", 2) == 0)	||
		(memcmp(&PbMsData.sBuf[9], "07", 2) == 0)	)
	{	
		if (IsNum(&PbMsData.sBuf[15], 3) == FALSE)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : MS �� ���� ��û")); //#KIMH0143
			fnAPP_CancelProc(T_MSG, "������� â������ �������� ��", "MS ����� �ŷ� �Ͻʽÿ�");
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][����][%s����]",
									(PbKind == PASSBOOK_LOAN)		 ? "����" :
									(PbKind == PASSBOOK_BENEFICERTI) ? "����" :
									(PbKind == PASSBOOK_NORMAL)		 ? "�Ϲ�" : "��Ÿ");
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_IBK_PbReadCheck(PbKind[%d]):return", PbKind);
	return T_OK;
}

int CTranCmn::fnAPD_KEB_PbReadCheck()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KEB_PbReadCheck()");

	CString	strPbData("");
	int nTestPbmflag = 0;
	
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, TRUE);
/////////////////////////////////////////////////////////////////////////////

	do 
	{
		PbTran = MENU_NULL;										// Pb�ŷ����º���
		memset(&PbMsData, 0, sizeof(PbMsData));					// �����ڷ�

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// ����Ms�ڷᱸ�ϱ�

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)                       // ���Ȼ�
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// ��������
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// �̵��Read����
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// ��ֹ߻���(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// ��ֹ߻��ð�(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// ��ü����(0-ī��,1-����,2-��ǥ,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if (PbMsData.Len > sizeof(PbMsData.sBuf))				// ����Ÿ���� Ȯ��
			PbTran = MENU_NULL;
		else
		if (!IsNum(pPbMsData->AccountNo, PbMsData.Len))			// ���¹�ȣ�ڸ�
			PbTran = MENU_NULL;
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// ����Ÿ����
			PbTran = MENU_NULL;
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// ����Ÿ����
			PbTran = MENU_NULL;
		else
			PbTran |= MENU_PB;

/*
		if (PbTran)
		{
			BYTE	PbAccount[16];
			fnAPD_KEB_GetPbAccount(PbAccount);
		}
*/

		// ��ȯ���� ���� ���� üũ 
		if (PbMsData.Len == 36)					// ��ȯ���� ������ ���  -> #0096
		{
			if ((memcmp(&pPbMsData->AccountNo[4], "739", 3) == 0)	||	// �ŷ��Ұ� ����
				(memcmp(&pPbMsData->AccountNo[4], "809", 3) == 0)	||
				(memcmp(&pPbMsData->AccountNo[4], "747", 3) == 0))
				PbTran = MENU_NULL;
			else
			if ((pPbMsData->AccountNo[7] == '9')	&&				// �ŷ��Ұ� ���� (������ 4��° 5��°�� 9�� 0�� ���)
				(pPbMsData->AccountNo[8] == '0'))
				PbTran = MENU_NULL;
			else
			if (PbMsData.sBuf[2] == '4') //������ - SystemID
			{

				if (m_pDevCmn->HostSvrPortMode == ECASH_LOCALPORT_MODE)
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Local Mode SystemID(%c), MSType(%c), PBM Type(%c)", PbMsData.sBuf[2], PbMsData.sBuf[3], PbMsData.sBuf[19]);

					if (PbMsData.sBuf[19] == '1') //����߱ޱ��� (Real)
					{
						nTestPbmflag = 0;
					}
					else
					if (PbMsData.sBuf[19] == '0') //����߱ޱ��� (Test)
					{
						nTestPbmflag = 1;
					}
				}
				else
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Real Mode SystemID(%c), MSType(%c), PBM Type(%c)", PbMsData.sBuf[2], PbMsData.sBuf[3], PbMsData.sBuf[19]);

					if (PbMsData.sBuf[19] == '1') //����߱ޱ��� (Real)
					{
						nTestPbmflag = 0;
					}
					else
					if (PbMsData.sBuf[19] == '0') //����߱ޱ��� (Test)
					{
						nTestPbmflag = 1;
					}
				}

	
				if (m_pDevCmn->HostSvrPortMode == ECASH_LOCALPORT_MODE)
				{
					if (nTestPbmflag == FALSE) //Real Pbm
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TEST ���� REAL ���� ��� �Ұ�"));
						fnAPP_CancelProc(T_MSG, "�� ��⿡���� ����� �Ұ��� �����Դϴ�", "ûȣ����ĳ��(��)���� ���� �ٶ��ϴ�");
					}
				}
				else
				{
					if (nTestPbmflag == TRUE) //TEST Pbm
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("REAL ���� TEST ���� ��� �Ұ�"));
						fnAPP_CancelProc(T_MSG, "�� ��⿡���� ����� �Ұ��� �����Դϴ�", "ûȣ����ĳ��(��)���� ���� �ٶ��ϴ�");
					}
				}

				CardBankFlag = KOEBANK;
				CardBank = KOEBANK;	
				PbTran |= MENU_PB;		
				PbBank = KOEBANK;
				memcpy(Accept.AccountNum, &PbMsData.sBuf[4], 13);

			}
			else
			if (PbMsData.sBuf[2] == '9') //������ - SystemID //#0096
			{
				if (PbMsData.sBuf[3] == '9' || PbMsData.sBuf[3] == '8') //MsType - 9,8
				{
					if (m_pDevCmn->HostSvrPortMode == ECASH_LOCALPORT_MODE)
					{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Local Mode SystemID(%c), MSType(%c), PBM Type(%c)", PbMsData.sBuf[2], PbMsData.sBuf[3], PbMsData.sBuf[17]);

						if (PbMsData.sBuf[17] == '1') //����߱ޱ��� (Real)
						{
							nTestPbmflag = 0;
						}
						else
						if (PbMsData.sBuf[17] == '0') //����߱ޱ��� (Test)
						{
							nTestPbmflag = 1;
						}
					}
					else
					{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Real Mode SystemID(%c), MSType(%c), PBM Type(%c)", PbMsData.sBuf[2], PbMsData.sBuf[3], PbMsData.sBuf[17]);

						if (PbMsData.sBuf[17] == '1') //����߱ޱ��� (Real)
						{
							nTestPbmflag = 0;
						}
						else
						if (PbMsData.sBuf[17] == '0') //����߱ޱ��� (Test)
						{
							nTestPbmflag = 1;
						}
					}
					
					if (m_pDevCmn->HostSvrPortMode == ECASH_LOCALPORT_MODE)
					{
						if (nTestPbmflag == FALSE) //Real Pbm
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TEST ���� REAL ���� ��� �Ұ�"));
							fnAPP_CancelProc(T_MSG, "�� ��⿡���� ����� �Ұ��� �����Դϴ�", "ûȣ����ĳ��(��)���� ���� �ٶ��ϴ�");
						}
					}
					else
					{
						if (nTestPbmflag == TRUE) //TEST Pbm
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("REAL ���� TEST ���� ��� �Ұ�"));
							fnAPP_CancelProc(T_MSG, "�� ��⿡���� ����� �Ұ��� �����Դϴ�", "ûȣ����ĳ��(��)���� ���� �ٶ��ϴ�");
						}
					}

					CardBankFlag = KOEBANK;
					CardBank = KOEBANK;	
					PbTran |= MENU_PB;		
					PbBank = KOEBANK;
					memcpy(Accept.AccountNum, &PbMsData.sBuf[4], 13);
				}
				else //MsType - 1,2,5
				{
					CardBankFlag = KOEBANK;
					CardBank = KOEBANK;	
					PbTran |= MENU_PB;		
					PbBank = KOEBANK;
					memcpy(Accept.AccountNum, &PbMsData.sBuf[4], 13);
				}

			}
		}		


		if (CardBank != KOEBANK) //#0123
			PbTran = MENU_NULL;
			

	} while(FALSE);

	
	if (!PbTran)	
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : ��� �Ұ� ")); //#KIMH0143

		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "�� ������ ��� �� �� �����ϴ�");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}

	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KEB_PbReadCheck(PbKind[%d]):return", PbKind);
	return T_OK;
}


// ��ȯ���� ���� 
#define	PB_DEMANDACC			101						// �䱸��/���༺/��Ź
#define PB_CREDITACC			102						// ��������
#define PB_FOREINEX				103						// ��ȯ����
#define PB_FOREINEXMB			104						// ��ȯ �ӴϹ� ���� 
#define PB_SAVINGBILL			105						// �������� (NCR52����Ʈ)
#define PB_TRUST				106						// ��Ź����

#define PB_NEXT_CREDITACC		111						// ������ ����
#define PB_NEXT_FOREINEX		112						// ������ �ܱ�ȯ,�������� 
#define PB_NEXT_FOREINEXMB		113						// ������ �ӴϹ�

#define PB_HNB_DEMAND			201						// �ϳ����� �䱸�� ����
#define PB_HNB_STOCK			202						// �ϳ����� ��Ʈ ����
// ��������������ϱ�
int CTranCmn::fnAPD_KEB_GetPbAccount(BYTE* pbSendAccount)
{
	int nPbAccLen = 0;
	PbKind = 0;

	// �ϳ����� ����
	if (PbMsData.Len == 18)
	{
		
		nPbAccLen = 16;
		memcpy(Accept.AccountNum, &PbMsData.sBuf[0], nPbAccLen);

		if ((memcmp(&Accept.AccountNum[12], "13", 2) == 0)	||
			(memcmp(&Accept.AccountNum[12], "36", 2) == 0))
			PbKind = PB_HNB_STOCK;
		else
			PbKind = PB_HNB_DEMAND;
		
		memcpy(pbSendAccount , Accept.AccountNum, nPbAccLen);
		
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "nPbKind(%d), nPbAccLen(%d)", PbKind, nPbAccLen);
		return nPbAccLen;
	}

	// '1' : �䱸��, '2': ���༺, '3':����,'4':�ܱ�ȯ,'5':��Ź, '6':�ӴϹ�, '4' : ��������
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_GetPbAccount: SystemID(%c), MSType(%c)", PbMsData.sBuf[2], PbMsData.sBuf[3]);

	if (PbMsData.sBuf[2] == '9')		// ����
	{
		switch (PbMsData.sBuf[3])
		{
		case '1':				// �䱸��
		case '7':				// �ſ䱸��				
			PbKind = PB_DEMANDACC;
			nPbAccLen = 11;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
			break;
 
		case '2':				// ���༺
			PbKind = PB_DEMANDACC;
			nPbAccLen = 11;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
				break;

		case '3':				// ����
			PbKind = PB_CREDITACC;
			nPbAccLen = 12;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
			break;

		case '4':				// ����(�ܱ�ȯ,��������)
		case '8':				// �ſ�ȭ				
			if (PbMsData.Len < 52)
			{
				PbKind = PB_FOREINEX;
				nPbAccLen = 12;

				// ������ ��ȭ����
				if ((!memcmp(&PbMsData.sBuf[7], "61", 2))	||
					(!memcmp(&PbMsData.sBuf[7], "62", 2))	||
					(!memcmp(&PbMsData.sBuf[7], "63", 2))	||
					(!memcmp(&PbMsData.sBuf[7], "64", 2))	||
					(!memcmp(&PbMsData.sBuf[7], "65", 2))	)
				{
					char SavePbMsData[256] = {0, };
					memcpy(SavePbMsData, PbMsData.sBuf, sizeof(PbMsData.sBuf));
	
					if (!memcmp(&PbMsData.sBuf[7], "61", 2))
						memcpy(&PbMsData.sBuf[7], "JSD", 3);
					else
					if (!memcmp(&PbMsData.sBuf[7], "62", 2))
						memcpy(&PbMsData.sBuf[7], "JCD", 3);
					else
					if (!memcmp(&PbMsData.sBuf[7], "63", 2))
						memcpy(&PbMsData.sBuf[7], "JFD", 3);
					else
					if (!memcmp(&PbMsData.sBuf[7], "64", 2))
						memcpy(&PbMsData.sBuf[7], "JBD", 3);
					else
					if (!memcmp(&PbMsData.sBuf[7], "65", 2))
						memcpy(&PbMsData.sBuf[7], "JTD", 3);
		
					memcpy(&PbMsData.sBuf[10], &SavePbMsData[9], (PbMsData.Len - 9));

//					Accept.IsFCAccount = TRUE;

					nPbAccLen++;
					PbMsData.Len++;
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_GetPbAccount-PB_FOREINEX: PbMsData.sBuf", PbMsData.sBuf, PbMsData.Len);
				}
				memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_GetPbAccount-PB_FOREINEX: Accept.AccountNum(%s), nPbAccLen(%d)", Accept.AccountNum, nPbAccLen);
			}
			else				// ��������
			{
				PbKind = PB_SAVINGBILL;
				nPbAccLen = 11;
				memcpy(Accept.AccountNum, &PbMsData.sBuf[7], nPbAccLen);
			}
			break;

		case '5':				// ��Ź
				PbKind = PB_TRUST;
				nPbAccLen = 11;
				memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
				break;
		
		case '6':				// �ӴϹ�
				PbKind = PB_FOREINEXMB;
				nPbAccLen = 13;
				memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
				break;
		}
	}
	else
	if (PbMsData.sBuf[2] == '4')		// ������
	{
		switch(PbMsData.sBuf[3])
		{
		case '1':				// ����
			PbKind = PB_NEXT_FOREINEX;
			nPbAccLen = 12;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
			break;

		case '3':				// ����	
			PbKind = PB_NEXT_CREDITACC;
			nPbAccLen = 15;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
			break;

		case '4':				// ��������.
			PbKind = PB_NEXT_CREDITACC;
			nPbAccLen = 12;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
			break;

		case '6':				// �ӴϹ�
			PbKind = PB_NEXT_FOREINEXMB;
			nPbAccLen = 13;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
			break;
		}
	}

	memcpy(pbSendAccount , Accept.AccountNum, nPbAccLen);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "nPbKind(%d), nPbAccLen(%d)", PbKind, nPbAccLen);

	return nPbAccLen;
}

// ��������������ϱ�
int CTranCmn::fnAPD_GetPbAccount(BYTE* pbSendAccount)
{
	BYTE MakeTranAccount[16];
	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
	{
		if ((PbMsData.Len == NAC_ACCNUM_SIZE1) ||
			(PbMsData.Len == NAC_ACCNUM_SIZE2))			// �������嵥���ͱ���üũ
		{
			memcpy(pbSendAccount, PbMsData.sBuf, 14);
			return 14;
		}
		else return FALSE;
	}

	memset(MakeTranAccount,	'0', sizeof(MakeTranAccount));
	memcpy(MakeTranAccount, &pPbMsData->AccountNo[0], 11);
	memcpy(pbSendAccount, MakeTranAccount, 11);

	return 12;
}

int CTranCmn::fnAPD_PbMSWrite()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbMSWrite Brand (%d)", m_pProfile->DEVICE.ETCDevice20);

	char szPbReadMsData[100];
	memset(szPbReadMsData, 0x00, sizeof(szPbReadMsData));

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	memcpy(szPbReadMsData, PbMsData.sBuf, PbMsData.Len);
	/*======================================================================================
	2009-01-09 : ���� �ű� ���� Format ����
	[1] MS DATA 41 ~ 43 �� ���� "000" �̸� 48 ���� "1" �̸� �ű� ����
	[2] �ű� ������ ��� ��ȿ�� �ڵ�� 48��° �ڸ��� �����Ѵ�.
	======================================================================================*/
	if ( memcmp(&szPbReadMsData[41], "000", 3) == 0  &&  memcmp(&szPbReadMsData[48], "1", 1) == 0)
	{
		memcpy(&szPbReadMsData[48], "0", 1);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] New Pattern Passbook Unavailable Code Set : 0");
	}
	else
	{
		if ((!memcmp(&szPbReadMsData[34], "10", 2)) || (!memcmp(&szPbReadMsData[34], "11", 2)))
		{
			  // MS�� �����ڵ尡 �����ϴ� ���...
			  // �߾�ȸ ����(Pattern1)�� ��� (��ȿ���ڵ��� ��ġ�� [43] 1�ڸ� (�迭 : 42)�̹Ƿ�...)
			  // ��ȿ���ڵ� ��ġ�� "0" SET�Ѵ�(���� ��� �Ҵ� ó��)
			  memcpy(&szPbReadMsData[42], "0", 1);
			  MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Pattern1 Passbook Unavailable Code Set : 0");
		}
		else 
		if (!memcmp(&szPbReadMsData[34], "12", 2))
		{
			  // MS�� �����ڵ尡 �����ϴ� ���...
			  // ���� ����(Pattern2)�� ��� (��ȿ���ڵ��� ��ġ�� [42] 1�ڸ� (�迭 : 41)�̹Ƿ�...)
			  // ��ȿ���ڵ� ��ġ�� "0" SET�Ѵ�(���� ��� �Ҵ� ó��)
			  memcpy(&szPbReadMsData[41], "0", 1);
			  MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Pattern2 Passbook Unavailable Code Set : 0");
		}
		else 
		{
			  // (��)���������� ��� MS�� �����ڵ尡 �������� �����Ƿ�...��ȿ���ڵ� ��ġ�� SET ����
			  if (!memcmp(&szPbReadMsData[42], "1", 1))
			  {
					 // �߾�ȸ ����(Pattern1)�� ��� (��ȿ���ڵ��� ��ġ�� [43] 1�ڸ� (�迭 : 42)�̹Ƿ�...)
					 // ��ȿ���ڵ� ��ġ�� "0" SET�Ѵ�(���� ��� �Ҵ� ó��)
					 memcpy(&szPbReadMsData[42], "0", 1);
					 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Unavailable Pattern1 Passbook Code Set : Success");
			  }
			  else 
			  if (!memcmp(&szPbReadMsData[41], "1", 1))
			  {
					 // ���� ����(Pattern2)�� ��� (��ȿ���ڵ��� ��ġ�� [42] 1�ڸ� (�迭 : 41)�̹Ƿ�...)
					 // ��ȿ���ڵ� ��ġ�� "0" SET�Ѵ�(���� ��� �Ҵ� ó��)
					 memcpy(&szPbReadMsData[41], "0", 1);
					 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Unavailable Pattern2 Passbook Code Set : Success");
			  }
			  else 
			  if (!memcmp(&szPbReadMsData[43], "1", 1)) //#0074
			  {
					 // ���� ����(Pattern2)�� ��� (��ȿ���ڵ��� ��ġ�� [43] 1�ڸ� (�迭 : 41)�̹Ƿ�...)
					 // ��ȿ���ڵ� ��ġ�� "0" SET�Ѵ�(���� ��� �Ҵ� ó��)
					 memcpy(&szPbReadMsData[43], "0", 1);
					 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Unavailable Pattern2 Passbook Code Set : Success");
			  }
			  else 
			  {
					 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Invalid Pattern1 or Pattern2 Passbook Code Position");
			  }
		}
	}

	m_pDevCmn->fnSCR_DisplayString(2, "�˼��մϴ�!");
	m_pDevCmn->fnSCR_DisplayString(3, "�Ͻ����� ��ַ� ���Ͽ� ó���� �ȵǾ����ϴ� ");
	m_pDevCmn->fnSCR_DisplayString(4, "����� ������ �湮�Ͻþ� �������� ������ Ȯ���Ͻñ� �ٶ��ϴ�");			//��Ÿ����  #N0198
	m_pDevCmn->fnSCR_DisplayScreen(701);
	Delay_Msg(5000);	
	m_pDevCmn->fnPBM_MSWrite(szPbReadMsData);
	fnAPD_CheckDeviceAction(DEV_PBM);
	
	return T_OK;
}

// ī�忥������ĵ
int CTranCmn::fnAPD_CardEmbossScan(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardEmbossScan(WaitFlg[%d])", WaitFlg);

	if (MenuIrdaMode)											// ���ܼ��ŷ�
		return T_OK;
	if (MenuEMVMode)											// EMVī��
		return T_OK;

	if(m_pProfile->DEVICE.MachineType == U3100K || m_pProfile->DEVICE.MachineType == U8100)
		return T_OK;


	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_OK;

	if ((!CardTran)			||									// ī������üũ
		(CardEmbossScanFlg))									// ī�忥������ĵüũ
		return T_OK;

	if (CardPowerOnFlg)
		CardPowerOnFlg = FALSE;									// ī�����ӹ�
	CardEmbossScanFlg = TRUE;									// ī�忥������ĵ����
	fnAPD_CheckDeviceAction(DEV_MCU);

	
	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_MCU);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardEmbossScan(...):return");
	return T_OK;
}

// ī��������Ʈ
int CTranCmn::fnAPD_CardImprint(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardImprint(WaitFlg[%d])", WaitFlg);

	if(m_pProfile->DEVICE.MachineType == U3100K || m_pProfile->DEVICE.MachineType == U8100)
		return T_OK;

	if (MenuIrdaMode)											// ���ܼ��ŷ�
		return T_OK;
	if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_JPR | DEV_MCU) != (DEV_JPR | DEV_MCU))
		return T_OK;
//	if (MenuFICMode)											// ����ICī��	2004.02.03
//		return T_OK;
//	if (MenuAvail == MENU_IC)									// IC�ŷ�
//		return T_OK;

	if ((!CardTran)				||								// ī������üũ
		(!CardEmbossScanFlg)	||								// ī�忥������ĵ��
		(CardImprintFlg))										// ī��������Ʈüũ
		return T_OK;
	
	if ((MenuFICMode)	||
		(MenuEPBMode)	||
		(MenuEMVMode)	||
		(MenuKCashMode)	)
	{
//		Delay_Msg(3000);
//		DWORD WaitTime = GetTickCount() + 5000;
		DWORD WaitTime = GetTickCount() + K_10_WAIT * 1000  ;				// 5�ʿ��� 10�ʷ� �ø�
		do {
			Delay_Msg();

			if (RegGetInt(_REGKEY_MCUINFO, "EMBOSSREADDONE"))
				break;
		} while(GetTickCount() < WaitTime);
	}

	CardImprintFlg = TRUE;										// ī��������Ʈ����
	fnAPD_CheckDeviceAction(DEV_JPR | DEV_MCU);
	m_pDevCmn->fnJPR_EmbossPrint();								// ī��������Ʈ

	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_JPR);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardImprint(...):return");
	return T_OK;
}

// ī���� : �Լ��̻��
int CTranCmn::fnAPD_CardWrite(MCAP* WriteCardData, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardWrite(WriteCardData, WaitFlg[%d])", WaitFlg);
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardWrite:WriteCardData", WriteCardData, sizeof(MCAP));

	if (MenuIrdaMode)											// ���ܼ��ŷ�
		return T_OK;

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_OK;

	if (!CardTran)												// ī������üũ
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_MCU);
//	m_pDevCmn->fnMCU_Write(MakeUnPack(WriteCardData->ISO3Buff, WriteCardData->ISO3size));
																// ���(ī���ڷ�3TR)	: makeUnPak�ϸ� �ȵ�(��ֹ߻���)
	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_MCU);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardWrite(...):return");
	return T_OK;
}

// ��������
int	CTranCmn::fnAPD_PbPrint(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbPrint(WaitFlg[%d])", WaitFlg);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	if (!PbTran)												// ��������üũ
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_PBM);
	m_pDevCmn->fnPBM_Print(MakeUnPack(PbData, PbDataCnt));		// ����(�����ڷ�)

	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_PBM);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbPrint(...):return");
	return T_OK;
}

// ��������INDEX��������
int	CTranCmn::fnAPD_EjrIndexSet()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EjrIndexSet()");

	char		szYYMMDD[9], szHHMMSS[13], szDummy0[1024];
	EjrIndexFlg = TRUE;											// ����������
	memset(&EjrData, ' ', sizeof(EjrData));						// ���������ڷ�
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);
	
	EjrData.Fill1 = '[';
	sprintf(szYYMMDD, "%2.2s/%2.2s/%2.2s", &GetDate().GetBuffer(0)[2], &GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6]);
	memcpy(EjrData.Date, szYYMMDD, sizeof(EjrData.Date));
	sprintf(szHHMMSS, "%2.2s:%2.2s:%2.2s.%03d", &GetTime().GetBuffer(0)[0], &GetTime().GetBuffer(0)[2], &GetTime().GetBuffer(0)[4], sTime.wMilliseconds);
	memcpy(EjrData.Time, szHHMMSS, sizeof(EjrData.Time));
	EjrData.Fill3 = ']';	
		
	sprintf(szDummy0, "[%8.8s %12.12s] ", szYYMMDD, szHHMMSS);					

	if (MenuIrdaMode)
		memcpy(EjrData.TranMaterial, "MOBILE TR", 9);
	else
	if (CardTran & MENU_MCU3)
	{
		if (MenuFICMode & MENU_IC)
			memcpy(EjrData.TranMaterial, "IC CARD 3", 8);
		else
		{
			if (CardTran & MENU_MCU2 && CardTran & MENU_J3T3)
				memcpy(EjrData.TranMaterial, "MSCARD23", 8);
			else
			if (CardTran & MENU_J3T3)
				memcpy(EjrData.TranMaterial, "MSCARD 3", 8);
			else
			if (CardTran & MENU_MCU2)
				memcpy(EjrData.TranMaterial,"MSCARD 2", 8);
			else
				memcpy(EjrData.TranMaterial, "MSCARD -", 8);
		}
	}
	else
	if (CardTran & MENU_MCU2)
		memcpy(EjrData.TranMaterial, "MSCARD 2", 8);
	else
	if (PbTran)
		memcpy(EjrData.TranMaterial, "PBM TRAN", 8);
	else
	{
		switch(TranCode2) 
		{
		case TRANID_A060:
			memcpy(EjrData.TranMaterial, "�հ�ŷ�", 8);
			break;
		case TRANID_A020:		 
			memcpy(EjrData.TranMaterial, "��������", 8);
			break;
		case TRANID_A010:		 
			memcpy(EjrData.TranMaterial, "��������", 8);
			break;
		default:
			memcpy(EjrData.TranMaterial, "�����", 8);
			break;
		}

	}

/////////////////////////////////////////////////////////////////////////////
	// �ŷ�������� : 2003.12.15
	memset(&EjrAmountData, 0, sizeof(EjrAmountData));			// ��������ݾװ����ڷ�

	if (TranProc == TRAN_WITH)									// ����
	{
		EjrAmountData.TranProc = TranProc;								// ����ݱ���
		EjrAmountData.TranAmount = Asc2Int(Accept.Money, 15);			// ����ݰŷ��ݾ�
		if (m_pDevCmn->Withdraw50TFlag)										
		{
			EjrAmountData.Cash50000Num = Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND;	// �����5�����Ǹż�
			EjrAmountData.Cash10000Num = Asc2Int(Accept.Cash10T_Money, 11);	// ����ݸ����Ǹż�
		}
		else																
			EjrAmountData.Cash10000Num = Asc2Int(Accept.CashMoney, 11);		// ����ݸ����Ǹż�

		EjrAmountData.MotherCheck10Num = Asc2Int(Accept.CheckMoney, 10);	// ����������ǥ10�����Ǹż�
	}

	if (TranProc == TRAN_DEP)									// �Ա�
	{
		EjrAmountData.TranProc = TranProc;						// ����ݱ���
		EjrAmountData.TranAmount = Asc2Int(Accept.Money, 8);	// ����ݰŷ��ݾ�
		
		if ((HBRM_SET == m_pProfile->DEVICE.CashHandler) &&	
			(m_pDevCmn->ChangeBCFlag))							
			EjrAmountData.Cash50000Num = CashData.mFiftyThousand;	// �����5�����Ǹż�

		EjrAmountData.Cash10000Num = CashData.mTenThousand;		// ����ݸ����Ǹż�
		EjrAmountData.Cash5000Num = CashData.mFiveThousand;		// ����ݿ�õ���Ǹż�
		EjrAmountData.Cash1000Num = CashData.mOneThousand;		// �����õ���Ǹż�

		if (CheckInfoCnt)
		{
			for (int i = 0; i < UCM_MAXCHECKINCNT; i++)
			{													// ����
				if (memcmp(CheckInfoData[i].Bank, BANKJJ2, 2) == 0)
				{
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 100000)
						EjrAmountData.MotherCheck10Num++;		// ����������ǥ10�����Ǹż�
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 300000)
						EjrAmountData.MotherCheck30Num++;		// ����������ǥ30�����Ǹż�
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 500000)
						EjrAmountData.MotherCheck50Num++;		// ����������ǥ50�����Ǹż�
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 1000000)
						EjrAmountData.MotherCheck100Num++;		// ����������ǥ100�����Ǹż�
				}
				else
				{
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 100000)
						EjrAmountData.OtherCheck10Num++;		// �����Ÿ���ǥ10�����Ǹż�
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 300000)
						EjrAmountData.OtherCheck30Num++;		// �����Ÿ���ǥ30�����Ǹż�
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 500000)
						EjrAmountData.OtherCheck50Num++;		// �����Ÿ���ǥ50�����Ǹż�
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 1000000)
						EjrAmountData.OtherCheck100Num++;		// �����Ÿ���ǥ100�����Ǹż�
				}
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EjrIndexSet():return");
	return T_OK;
}

// ��ǥ&��������
int	CTranCmn::fnAPD_ShuPrint(int PrintDevice, int JnlType, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_ShuPrint(PrintDevice[%x], WaitFlg[%d])", PrintDevice, WaitFlg);

	int		i, js = 0, jj =0, Len, FCh = 0;
/////////////////////////////////////////////////////////////////////////////
	// V03-01-01 ī��ó���� SPL LEDó������(SCDP��������)
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_SLIP_MAT, TRUE);

/////////////////////////////////////////////////////////////////////////////

	fnAPD_EjrIndexSet();										// ��������INDEX��������
/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->TranResult)		&&							// �ŷ����� : 2003.12.15
		(CancelProcNum == 0))									// ��Ҿ���
		m_pDevCmn->fnAPL_EjrSpoolPrintStart(PrintDevice, &EjrData, &EjrAmountData);
	else
		m_pDevCmn->fnAPL_EjrSpoolPrintStart(PrintDevice, &EjrData, NULL);
/////////////////////////////////////////////////////////////////////////////

	for (i = 0; i < itemof(ShuData); i++)
	{
		Len = strlen(ShuData[i]);
		FCh = FindChar(ShuData[i], Len, '@');
		if (PrintDevice & DEV_SPR)
		{
			if(FCh != Len)
			{
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, js++, Len, GetString(&ShuData[i][FCh+1], Len -1 , Len -1));
			}
			else	
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, js++, Len, GetString(ShuData[i], Len, Len));
		}
		else
		if (PrintDevice & DEV_JPR) 
		{
			if(FCh != Len)
			{
				/*			#N0206
				if(m_pProfile->DEVICE.MachineType == U8100)
					m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, js++, Len, GetString("�����ܾ�:*��                    ", 24, 24));
				else
					m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, js++, Len, GetString("  �����ܾ� : *��                    ", 36, 36));
				*/
				;
			}
			else 
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, js++, Len, GetString(ShuData[i], Len, Len));
		}
	}

	if (PrintDevice & DEV_SPR)
		SlipPrintFlg = TRUE;
	if (PrintDevice & DEV_JPR)
		JnlPrintFlg = TRUE;

	m_pDevCmn->fnAPL_EjrSpoolEnd(PrintDevice, JnlType);

	if (WaitFlg)
		fnAPD_CheckDeviceAction(PrintDevice);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_ShuPrint(...):return");
	return T_OK;
}

// ��������(����ó��)
int	CTranCmn::fnAPD_JnlPrint(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_JnlPrint(WaitFlg[%d])", WaitFlg);

	int		i, jj =0, Len;

	fnAPD_EjrIndexSet();										// ��������INDEX��������

	m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_JPR, &EjrData, NULL);

	for (i = 0; i < itemof(ShuData); i++)
	{
		Len = strlen(ShuData[i]);
		m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, jj++, Len, GetString(ShuData[i], Len, Len));
	}

	m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_JPR, JNL_STATUS);

	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_JPR);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_JnlPrint(...):return");
	return T_OK;
}

// ��ü����
int CTranCmn::fnAPD_MaterialOut(int Device, int WaitFlg, int CheckTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut(Device[%x], WaitFlg[%d], CheckTime[%d])", Device, WaitFlg, CheckTime);

	int		ScrNum = 0;
	int		ActDevice = Device;

/////////////////////////////////////////////////////////////////////////////
	MaterialOutFlg = 0;											// ��ü��������
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		ActDevice &= ~DEV_SPR;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		ActDevice &= ~DEV_MCU;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		ActDevice &= ~DEV_BRM;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		ActDevice &= ~DEV_UCM;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		ActDevice &= ~DEV_PBM;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut:ActDevice[%x]", ActDevice);

/////////////////////////////////////////////////////////////////////////////
	if (MenuIrdaMode)											// ���ܼ��ŷ�
		;
	else
	if (CardTran)
	{
		if ((TranSend)				&&							// �ŷ��۽���
			(TranResultNGCode == 22))							// ī��ȸ��	
		{
			ActDevice &= ~DEV_MCU;
			m_pDevCmn->fnSCR_DisplayString(2, "ī��� ���࿡�� �����ϰڽ��ϴ�");
			m_pDevCmn->fnSCR_DisplayString(4, "�������� �����Ͽ� �ֽʽÿ�");
			m_pDevCmn->fnSCR_DisplayScreen(701);				// �н�ȸ�� �ȳ�ȭ��
			Delay_Msg(3*1000);
		}
	}
//	else
//	if (PbTran)
//	{
//		if ((TranSend)		&&									// �ŷ��۽���
//			(RetractFlag)	)									// ����ȸ��	
//		{
//			ActDevice &= ~DEV_PBM;
//			m_pDevCmn->fnSCR_DisplayString(2, "������ ���࿡�� �����ϰڽ��ϴ�");
//			m_pDevCmn->fnSCR_DisplayString(4, "�������� �����Ͽ� �ֽʽÿ�");
//			m_pDevCmn->fnSCR_DisplayScreen(701);				// �н�ȸ�� �ȳ�ȭ��
//			Delay_Msg(3*1000);
//		}
//	}

/////////////////////////////////////////////////////////////////////////////
	if (ActDevice & DEV_SPR)									// ��ǥ��������
	{
		if (!SlipPrintFlg)										// ��ǥ���ڹ�
			ActDevice &= ~DEV_SPR;

		if(SprOutRetryFlg)                                      // ��ǥ ��ֽ� 2009.01.05
			ActDevice &= ~DEV_SPR;

	}

	if (ActDevice & DEV_BRM)									// ���ݹ����ȯ����
	{
		if ((TranProc == TRAN_WITH) &&							// ���
			(!CashCountFlg))									// ������ݰ����
			ActDevice &= ~DEV_BRM;
		else
		if ((TranProc == TRAN_DEP)		&&						// �Ա�
			((Accept.MoneyInKind & TRAN_DEP_CASH) != TRAN_DEP_CASH))
			ActDevice &= ~DEV_BRM;								// �Ա����ݺҰ���
		else
		{
			fnAPD_CheckDeviceAction(DEV_BRM);
			Delay_Msg(200);										// �������(200ms)
			fnAPD_CheckDeviceAction(DEV_BRM);
		
			if (!m_pDevCmn->fnCSH_GetMaterialInfo())			// ���ݹ�
				ActDevice &= ~DEV_BRM;
		}
	}

	if (ActDevice & DEV_UCM)									// ��ǥ�����ȯ����
	{
		if ((TranProc == TRAN_WITH) &&							// ���
			(!CheckCountFlg))									// ��ݼ�ǥ�����
			ActDevice &= ~DEV_UCM;
		else
		if ((TranProc == TRAN_DEP) &&							// �Ա�
			((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK))	
			ActDevice &= ~DEV_UCM;								// �Աݼ�ǥ�Ұ���
		else
		{
			fnAPD_CheckDeviceAction(DEV_UCM);

			if ((m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)) ||	
				(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW)));
			else ActDevice &= ~DEV_UCM;							// ��ǥ��
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut:ActDevice[%x], "	\
													"TranProc[%d], "			\
													"Accept.MoneyInKind[%d], "	\
													"CashCountFlg[%d], "		\
													"CheckCountFlg[%d]",		\
													ActDevice,
													TranProc,
													Accept.MoneyInKind,
													CashCountFlg,
													CheckCountFlg);

/////////////////////////////////////////////////////////////////////////////
	fnAPD_CheckDeviceAction(ActDevice);

	if (CardPowerOnFlg)											// ī��������(����ȭ��)
	{
		CardPowerOnFlg = FALSE;									// ī�����ӹ�(����ȭ��)
		m_pDevCmn->fnKCS_PowerOff();							// ��������
		fnAPD_CheckDeviceAction(DEV_MCU);
	}

/////////////////////////////////////////////////////////////////////////////
//	�ŷ���ü ����ó�� : 2003.12.01
//	�ŷ��� �����ϴ� ��ü�� ���ؼ� Ȯ���Ѵ�.
//	��ŷ���ü�� �ܷ��Ͽ� �����ŷ��� Ȯ���ϵ��� �Ѵ�.
//  �Ʒ��� ���� ó���Ͽ��� �ǳ�(�ϴ��ڵ� �׽�Ʈ����) ������ü Ȯ���ϴ� �������
//	�ϴܿ� ó���Ͽ���.
/////////////////////////////////////////////////////////////////////////////
//	if (CardTran)
//		ActDevice &= ~DEV_PBM;
//	else
//	if (PbTran)
//		ActDevice &= ~DEV_MCU;

/////////////////////////////////////////////////////////////////////////////
	if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo()) &&	// ��ǥ����
		((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo()) &&	// ī�����						
		(!((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo())))	// ���幫/���ù�
	{
			MaterialOutFlg |= DEV_SPR;							// ��ǥ������
			MaterialOutFlg |= DEV_MCU;							// ī�������

			ScrNum = 809;										// ��ǥ/ī�����ȭ��
			if (TranProc == TRAN_WITH)							// ���
			{
				if (ActDevice & DEV_BRM)						// ���ݹ�����
					ScrNum = 810;								// ��ǥ/ī��/���ݹ���ȭ��
				if (ActDevice & DEV_UCM)						// ���ݹ�����
					ScrNum = 842;								// ��ǥ/ī��/��ǥ����ȭ��
				if ((ActDevice & DEV_BRM) &&					// ���ݹ�����
					(ActDevice & DEV_UCM))						// ��ǥ������
					ScrNum = 843;								// ��ǥ/ī��/����/��ǥ����ȭ��
			}
			if (TranProc == TRAN_DEP)							// �Ա�
			{
				if (ActDevice & DEV_BRM)						// ���ݹ�ȯ��
					ScrNum = 809;								// ��ǥ/ī��/���ݹ�ȯȭ��
				if (ActDevice & DEV_UCM)						// ���ݹ�ȯ��
					ScrNum = 809;								// ��ǥ/ī��/��ǥ��ȯȭ��
				if ((ActDevice & DEV_BRM) &&					// ���ݹ�ȯ��
					(ActDevice & DEV_UCM))						// ��ǥ��ȯ��
					ScrNum = 809;								// ��ǥ/ī��/����/��ǥ��ȯȭ��
			}

			m_pDevCmn->DeviceTimeout &= ~(DEV_SPR | DEV_MCU);	// �ð��ʰ������ʱ�ȭ
			if (m_pDevCmn->EjectBothAvail)						// ī��/��ǥ ���ù��� ����	
			{
				if (ScrNum)
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum);		// ������ȭ�����

				m_pDevCmn->fnMCU_EjectBoth(CheckTime);
				fnAPD_CheckDeviceAction(DEV_MCU);
				m_pDevCmn->fnSPR_EjectBoth(CheckTime);
				fnAPD_CheckDeviceAction(DEV_SPR);
			}
			else
			{
				m_pDevCmn->fnSPR_Eject(CheckTime);				// U8100 AP ���泻�� #06 - �ӵ�����
				m_pDevCmn->fnMCU_Eject(CheckTime);				// SPR�� MCU ��ġ �ٲٸ� ������ ������
			}													// V07-01-10-#01-end

			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("ī��/��ǥ ���� "));
			m_pDevCmn->fnLGT_SetFlicker(KU_SPR_ATM_FLICKER, FLICKER_ON);
			m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
	}
	else 
	if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo()) &&	// ��ǥ����
		((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo()) &&	// �������							
		(!((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())))	// ī�幫/���ù�
	{
			MaterialOutFlg |= DEV_SPR;							// ��ǥ������
			MaterialOutFlg |= DEV_PBM;							// ���������

			ScrNum = 817;										// ��ǥ/�������ȭ��
			if (TranProc == TRAN_WITH)							// ���
			{
				if (ActDevice & DEV_BRM)						// ���ݹ�����
					ScrNum = 850;								// ��ǥ/����/���ݹ���ȭ��
				if (ActDevice & DEV_UCM)						// ���ݹ�����
					ScrNum = 856;								// ��ǥ/����/��ǥ����ȭ��
				if ((ActDevice & DEV_BRM) &&					// ���ݹ�����
					(ActDevice & DEV_UCM))						// ��ǥ������
					ScrNum = 819;								// ��ǥ/����/����/��ǥ����ȭ��
			}
			if (TranProc == TRAN_DEP)							// �Ա�
			{
				if (ActDevice & DEV_BRM)						// ���ݹ�ȯ��
					ScrNum = 817;								// ��ǥ/����/���ݹ�ȯȭ��
				if (ActDevice & DEV_UCM)						// ���ݹ�ȯ��
					ScrNum = 817;								// ��ǥ/����/��ǥ��ȯȭ��
				if ((ActDevice & DEV_BRM) &&					// ���ݹ�ȯ��
					(ActDevice & DEV_UCM))						// ��ǥ��ȯ��
					ScrNum = 817;								// ��ǥ/����/����/��ǥ��ȯȭ��
			}

			m_pDevCmn->DeviceTimeout &= ~(DEV_SPR | DEV_PBM);	// �ð��ʰ������ʱ�ȭ
			m_pDevCmn->fnSPR_Eject(CheckTime);
			m_pDevCmn->fnPBM_Eject(CheckTime);	
			m_pDevCmn->fnLGT_SetFlicker(KU_SPR_ATM_FLICKER, FLICKER_ON);
			m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_ON);
	}
	else 
	if (((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo()) &&	// ī�����
		((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo()) &&	// �������					
		(!((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())))	// ����/���ù�
	{
			MaterialOutFlg |= DEV_MCU;							// ī�������
			MaterialOutFlg |= DEV_PBM;							// ���������

			ScrNum = 820;										// ī��/�������ȭ��
			if (TranProc == TRAN_WITH)							// ���
			{
				if (ActDevice & DEV_BRM)						// ���ݹ�����
					ScrNum = 820;								// ī��/����/���ݹ���ȭ��
			}
			if (TranProc == TRAN_DEP)							// �Ա�
			{
				if (ActDevice & DEV_BRM)						// ���ݹ�ȯ��
					ScrNum = 820;								// ī��/����/���ݹ�ȯȭ��
				if (ActDevice & DEV_UCM)						// ���ݹ�ȯ��
					ScrNum = 820;								// ī��/����/��ǥ��ȯȭ��
				if ((ActDevice & DEV_BRM) &&					// ���ݹ�ȯ��
					(ActDevice & DEV_UCM))						// ��ǥ��ȯ��
					ScrNum = 820;								// ī��/����/����/��ǥ��ȯȭ��
			}

			m_pDevCmn->DeviceTimeout &= ~(DEV_MCU | DEV_PBM);	// �ð��ʰ������ʱ�ȭ
			m_pDevCmn->fnMCU_Eject(CheckTime);
			m_pDevCmn->fnPBM_Eject(CheckTime);	
			m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
			m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_ON);
	}
	else 
	{
		if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())		||	// ��ǥ����
			((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())		||	// ī�����
			((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo()))			// �������
			;
		else
		{
			if (TranProc == TRAN_WITH)							// ���
			{
				if (ActDevice & DEV_BRM)						// ���ݹ�����
					ScrNum = 848;								// ���ݹ���ȭ��
				if (ActDevice & DEV_UCM)						// ��ǥ������
					ScrNum = 849;								// ��ǥ����ȭ��
				if ((ActDevice & DEV_BRM) &&					// ���ݹ�ȯ��
					(ActDevice & DEV_UCM))						// ��ǥ��ȯ��
					ScrNum = 816;								// ���ݼ�ǥ����ȭ��
			}
			if (TranProc == TRAN_DEP)							// �Ա�
			{
				if (ActDevice & DEV_BRM)						// ���ݹ�ȯ��
					ScrNum = 0;									// ���ݹ�ȯȭ��
				if (ActDevice & DEV_UCM)						// ���ݹ�ȯ��
					ScrNum = 0;									// ��ǥ��ȯȭ��
				if ((ActDevice & DEV_BRM) &&					// ���ݹ�ȯ��
					(ActDevice & DEV_UCM))						// ��ǥ��ȯ��
					ScrNum = 0;									// ����/��ǥ��ȯȭ��
			}
		}

																// 2003.12.01 : ī��/����/��ǥ������
		if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())	&&	// ��ǥ����
			((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())	&&	// ī�����
			((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo()))		// �������
		{
			MaterialOutFlg |= DEV_SPR;							// ��ǥ������
			MaterialOutFlg |= DEV_MCU;							// ī�������
			MaterialOutFlg |= DEV_PBM;							// ���������

			ScrNum = 817;										// ��ǥ/ī�����ȭ��
			if (TranProc == TRAN_WITH)							// ���
			{
				if (ActDevice & DEV_BRM)						// ���ݹ�����
					ScrNum = 850;								// ��ǥ/ī��/���ݹ���ȭ��
				if (ActDevice & DEV_UCM)						// ���ݹ�����
					ScrNum = 856;								// ��ǥ/ī��/��ǥ����ȭ��
				if ((ActDevice & DEV_BRM) &&					// ���ݹ�����
					(ActDevice & DEV_UCM))						// ��ǥ������
					ScrNum = 819;								// ��ǥ/ī��/����/��ǥ����ȭ��
			}
			if (TranProc == TRAN_DEP)							// �Ա�
			{
				if (ActDevice & DEV_BRM)						// ���ݹ�ȯ��
					ScrNum = 817;								// ��ǥ/ī��/���ݹ�ȯȭ��
				if (ActDevice & DEV_UCM)						// ���ݹ�ȯ��
					ScrNum = 817;								// ��ǥ/ī��/��ǥ��ȯȭ��
				if ((ActDevice & DEV_BRM) &&					// ���ݹ�ȯ��
					(ActDevice & DEV_UCM))						// ��ǥ��ȯ��
					ScrNum = 817;								// ��ǥ/ī��/����/��ǥ��ȯȭ��
			}

			m_pDevCmn->DeviceTimeout &= ~(DEV_SPR | DEV_MCU | DEV_PBM);	// �ð��ʰ������ʱ�ȭ
			m_pDevCmn->fnSPR_Eject(CheckTime);					// ���ù��⺸��
			m_pDevCmn->fnMCU_Eject(CheckTime);					// ���ù��⺸��
			m_pDevCmn->fnPBM_Eject(CheckTime);					// ���ù��⺸��
			m_pDevCmn->fnLGT_SetFlicker(KU_SPR_ATM_FLICKER, FLICKER_ON);
			m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
			m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_ON);
		}
		else
		{
			if ((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())
			{													// ��ǥ����
				MaterialOutFlg |= DEV_SPR;						// ��ǥ������

				ScrNum = 813;									// ��ǥ����ȭ��
				if (TranProc == TRAN_WITH )						// ���, ī������ 
				{
					if (ActDevice & DEV_BRM)					// ���ݹ�����
						ScrNum = 815;							// ��ǥ/���ݹ���ȭ��
					if (ActDevice & DEV_UCM)					// ���ݹ�����
						ScrNum = 857;							// ��ǥ/��ǥ����ȭ��
					if ((ActDevice & DEV_BRM) &&				// ���ݹ�����
						(ActDevice & DEV_UCM))					// ��ǥ������
						ScrNum = 858;							// ��ǥ/����/��ǥ����ȭ��
				}
				if (TranProc == TRAN_DEP)						// �Ա�
				{
					if (ActDevice & DEV_BRM)					// ���ݹ�ȯ��
						ScrNum = 813;							// ��ǥ/���ݹ�ȯȭ��
					if (ActDevice & DEV_UCM)					// ���ݹ�ȯ��
						ScrNum = 813;							// ��ǥ/��ǥ��ȯȭ��
					if ((ActDevice & DEV_BRM) &&				// ���ݹ�ȯ��
						(ActDevice & DEV_UCM))					// ��ǥ��ȯ��
						ScrNum = 813;							// ��ǥ/����/��ǥ��ȯȭ��
				}

				m_pDevCmn->DeviceTimeout &= ~DEV_SPR;			// �ð��ʰ������ʱ�ȭ
				m_pDevCmn->fnSPR_Eject(CheckTime);
				m_pDevCmn->fnLGT_SetFlicker(KU_SPR_ATM_FLICKER, FLICKER_ON);
			}

			if ((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())
			{													// ī�����
				MaterialOutFlg |= DEV_MCU;						// ī�������

				ScrNum = 806;									// ī�����ȭ��
				if (TranProc == TRAN_WITH)						// ���
				{
					int nGetReg = RegGetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 0);
					if(nGetReg == 2)
						m_pDevCmn->fnSCR_DisplayString(1, "TRAN_WITH");
					
					if (ActDevice & DEV_BRM)					// ���ݹ�����
						ScrNum = 846;							// ī��/���ݹ���ȭ��
					if (ActDevice & DEV_UCM)					// ���ݹ�����
						ScrNum = 847;							// ī��/��ǥ����ȭ��
					if ((ActDevice & DEV_BRM) &&				// ���ݹ�����
						(ActDevice & DEV_UCM))					// ��ǥ������
						ScrNum = 812;							// ī��/����/��ǥ����ȭ��


				}
				
				if (TranProc == TRAN_DEP)						// �Ա�
				{
					if (ActDevice & DEV_BRM)					// ���ݹ�ȯ��
						ScrNum = 806;							// ī��/���ݹ�ȯȭ��
					if (ActDevice & DEV_UCM)					// ���ݹ�ȯ��
						ScrNum = 806;							// ī��/��ǥ��ȯȭ��
					if ((ActDevice & DEV_BRM) &&				// ���ݹ�ȯ��
						(ActDevice & DEV_UCM))					// ��ǥ��ȯ��
						ScrNum = 806;							// ī��/����/��ǥ��ȯȭ��
				}

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("ī����� ==>"));

				m_pDevCmn->DeviceTimeout &= ~DEV_MCU;		// �ð��ʰ������ʱ�ȭ
				m_pDevCmn->fnMCU_Eject(CheckTime);	
				m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
			}



			if ((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo())
			{													// �������
				MaterialOutFlg |= DEV_PBM;						// ���������

//				if (!PbNextPageFlag) - Auto Page ��� //#0022
					ScrNum = 807;								// �������ȭ��

				if (TranProc == TRAN_WITH)						// ���
				{
					if (ActDevice & DEV_BRM)					// ���ݹ�����
						ScrNum = 844;							// ����/���ݹ���ȭ��
					if (ActDevice & DEV_UCM)					// ���ݹ�����
						ScrNum = 845;							// ����/��ǥ����ȭ��
					if ((ActDevice & DEV_BRM) &&				// ���ݹ�����
						(ActDevice & DEV_UCM))					// ��ǥ������
						ScrNum = 808;							// ����/����/��ǥ����ȭ��
				}
				if (TranProc == TRAN_DEP)						// �Ա�
				{
					if (ActDevice & DEV_BRM)					// ���ݹ�ȯ��
						ScrNum = 807;							// ����/���ݹ�ȯȭ��
					if (ActDevice & DEV_UCM)					// ���ݹ�ȯ��
						ScrNum = 807;							// ����/��ǥ��ȯȭ��
					if ((ActDevice & DEV_BRM) &&				// ���ݹ�ȯ��
						(ActDevice & DEV_UCM))					// ��ǥ��ȯ��
						ScrNum = 807;							// ����/����/��ǥ��ȯȭ��
				}

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�������:"));

				m_pDevCmn->DeviceTimeout &= ~DEV_PBM;		// �ð��ʰ������ʱ�ȭ
				m_pDevCmn->fnPBM_Eject(CheckTime);	
				m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_ON);
			}
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut:MaterialOutFlg[%x]", MaterialOutFlg);

/////////////////////////////////////////////////////////////////////////////
	if (WaitFlg)
	{
		fnAPD_CheckDeviceAction(ActDevice);

		if (ActDevice & DEV_PBM)								// �������Ȯ��(���ڴ��)
		{
			ULONG  WaitTime = TimerSet(10);						// ���ڴ�� 10Sec
			while (m_pDevCmn->fnPBM_GetMaterialInfo() & ST_SENSOR2)
			{													// ���ʰ�������
				Delay_Msg();									// �������
				if (CheckTimer(WaitTime))						// ���ڴ�� TimeOver
					break;
			}
		}
	}

	// ����ȳ��޼����߰��κ�
	if (MaterialOutFlg & DEV_PBM)								// ���������
	{
																// ���������̿��޼���
																// ���������޼���
	}

	if (ScrNum == 808)											// ����/����/��ǥ
	{
		m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.CashMoney, 15);
																// ����
		m_pDevCmn->fnSCR_DisplayAmount(2, (LPCTSTR)Accept.CheckMoney, 15);

		switch(TranCode2)
		{
		case TRANID_4330: 
			m_pDevCmn->fnSCR_DisplayAmount(3, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			break;
		case TRANID_4310:
		case TRANID_4210:
			m_pDevCmn->fnSCR_DisplayAmount(3, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			break;
		}
																// ������
	}
	if (ScrNum == 844)											// ����/����
	{
		m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.CashMoney, 15);
																// ����
		switch(TranCode2)
		{
		case TRANID_4330: 
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
		case TRANID_4310:
		case TRANID_4210:
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			break;
		}
																// ������
	}
	if (ScrNum == 845)											// ����/��ǥ
	{
		m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.CheckMoney, 15);
																// ��ǥ
		switch(TranCode2)
		{
		case TRANID_4330: 
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
		case TRANID_4310:
		case TRANID_4210:
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			break;
		}
																// ������
	}
	if ((ScrNum == 812)	||										// ī��/����/��ǥ
		(ScrNum == 816)	)
	{
		m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.CashMoney, 15);
																// ����
		m_pDevCmn->fnSCR_DisplayAmount(2, (LPCTSTR)Accept.CheckMoney, 15);
																// ��ǥ
		switch(TranCode2)
		{
		case TRANID_4330: 
			m_pDevCmn->fnSCR_DisplayAmount(3, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			m_pDevCmn->fnSCR_DisplayString(4, m_RD.byDataField_017x);
//#0130		m_pDevCmn->fnSCR_DisplayAmount(5, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			m_pDevCmn->fnSCR_DisplayAmount(5, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		case TRANID_4310:
		case TRANID_4210:
			m_pDevCmn->fnSCR_DisplayAmount(3, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			m_pDevCmn->fnSCR_DisplayString(4, m_RD.byDataField_017x);
//#0130		m_pDevCmn->fnSCR_DisplayAmount(5, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			m_pDevCmn->fnSCR_DisplayAmount(5, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			break;
		}
	}
	if ((ScrNum == 846)	||										// ī��/����
		(ScrNum == 848)	)										// ����
	{
		m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.CashMoney, 15);
		switch(TranCode2)
		{
		case TRANID_4330: 
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			m_pDevCmn->fnSCR_DisplayString(3, m_RD.byDataField_017x);
//#0130		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			m_pDevCmn->fnSCR_DisplayAmount(4, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			break;
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			m_pDevCmn->fnSCR_DisplayString(3, m_RD.byDataField_017x);
//#0130		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			m_pDevCmn->fnSCR_DisplayAmount(4, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			break;
		}
		
	}
	if ((ScrNum == 847)	||										// ī��/��ǥ
		(ScrNum == 849)	)										// ��ǥ
	{
		m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.CheckMoney, 15);
																// ��ǥ
		switch(TranCode2)
		{
		case TRANID_4310: 
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			m_pDevCmn->fnSCR_DisplayString(3, m_RD.byDataField_017x);
//#0130		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			m_pDevCmn->fnSCR_DisplayAmount(4, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		case TRANID_4330: 
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			m_pDevCmn->fnSCR_DisplayString(3, m_RD.byDataField_017x);
//#0130		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			m_pDevCmn->fnSCR_DisplayAmount(4, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			break;
		}
	}

	if (ScrNum)
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum);					// ������ȭ�����

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut(...):return");
	return T_OK;
}

// A4���� ����
int CTranCmn::fnAPD_A4PMaterialOut(int nPrintEndFlag, int WaitFlg, int CheckTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_A4PMaterialOut(nPrintEndFlag[%d], WaitFlg[%d], CheckTime[%d])", nPrintEndFlag, WaitFlg, CheckTime);
/////////////////////////////////////////////////////////////////////////////
	MaterialOutFlg = 0;											// ��ü��������
/////////////////////////////////////////////////////////////////////////////

	int nScrNum = 0;
/////////////////////////////////////////////////////////////////////////////
//L	if (m_pDevCmn->fnA4P_GetMaterialInfo())						// 2007.06.21
	{															// A4��������
			MaterialOutFlg |= DEV_A4P;							// A4����������

// 			if (nPrintEndFlag)
// 				nScrNum = 859;									// A4��������ȭ��
// 			else
// 				nScrNum = 859;									// A4��������ȭ��
			if(TranCode == TC_INQUIRYA)							// �ŷ��������
				nScrNum = 859;
			else		// TC_INQPRINT							// �ܰ����
				nScrNum = 861;

			m_pDevCmn->DeviceTimeout &= ~DEV_A4P;				// �ð��ʰ������ʱ�ȭ
//			m_pDevCmn->fnLGT_SetFlicker(KU_A4P_TOP_FLICKER, FLICKER_ON);	// �ø�Ŀ��ġ ����Ǽ��� ������ ON
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_A4PMaterialOut:MaterialOutFlg[%x]", MaterialOutFlg);

/////////////////////////////////////////////////////////////////////////////
	if (WaitFlg)
	{
		fnAPD_CheckDeviceAction(DEV_A4P);
	}

	if (nScrNum)
		m_pDevCmn->fnSCR_DisplayScreen(nScrNum);				// ������ȭ�����

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_A4PMaterialOut(...):return");
	return T_OK;
}

// ��ü���������ȸ��
int CTranCmn::fnAPD_MaterialOutCheckRetract(int Device, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract(Device[%x], WaitFlg[%d])", Device, WaitFlg);

	int		ActDevice = Device;
	int		RetractDevice = Device;
	int		ErrDevice = DEV_NONE;

/////////////////////////////////////////////////////////////////////////////
	MaterialOutCheckFlg = 0;									// ��ü�����ļ�������
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		ActDevice &= ~DEV_SPR;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		ActDevice &= ~DEV_MCU;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		ActDevice &= ~DEV_PBM;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_A4P))
		ActDevice &= ~DEV_A4P;
	RetractDevice = ActDevice;									// ȸ�����������������
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:ActDevice[%x]", ActDevice);

	if (!(MaterialOutFlg & DEV_SPR))							// ���⹫
		ActDevice &= ~DEV_SPR;
	if (!(MaterialOutFlg & DEV_MCU))							// ���⹫
		ActDevice &= ~DEV_MCU;
	if (!(MaterialOutFlg & DEV_PBM))							// ���⹫
		ActDevice &= ~DEV_PBM;
	if (!(MaterialOutFlg & DEV_A4P))							// ���⹫
		ActDevice &= ~DEV_A4P;
/*
	if ((Accept.SerialTranFlag)	&&								// ���Ӱŷ��̰�
		(CardTran)				)								// ī�尡 ������
	{
		ActDevice &= ~DEV_MCU;									// ī��� ȸ������ ����
	}

	if ((Accept.SerialTranFlag)	&&								// ���Ӱŷ��̰�
		(PbTran)				)								// ī�尡 ������
	{
		ActDevice &= ~DEV_PBM;									// ������ ȸ������ ����
	}
*/
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:ActDevice[%x], MaterialOutFlg[%x]", ActDevice, MaterialOutFlg);

/////////////////////////////////////////////////////////////////////////////
	fnAPD_CheckDeviceAction(ActDevice);
	if (ActDevice)												// ��ü����
	{
		if (ActDevice & DEV_SPR)
			m_pDevCmn->fnSPR_WaitTaken();
		if (ActDevice & DEV_MCU)
			m_pDevCmn->fnMCU_WaitTaken();
		if (ActDevice & DEV_PBM)
			m_pDevCmn->fnPBM_WaitTaken();
		if (ActDevice & DEV_A4P)
			m_pDevCmn->fnA4P_WaitTaken();
		fnAPD_CheckDeviceAction(ActDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:m_pDevCmn->DeviceTimeout[%x]", m_pDevCmn->DeviceTimeout);

		// ī��/����/��ǥ�� ����Ȯ�ο� ���� ������ ���� History
		// 1. ���� : SP�̺�Ʈ�� ����
		// 2. ���� : SP�̺�Ʈ + ��ü�ܷ�����Ȯ�� : 2003.11.07
		// 3. ���� : SP�̺�Ʈ���� �����ϵ��� �������� ���� : 2003.12.06
		if ((ActDevice & DEV_SPR) && (!(m_pDevCmn->DeviceTimeout & DEV_SPR)))
		{
//t			if (!m_pDevCmn->fnSPR_GetMaterialInfo())			// ��ǥ���� : 2003.11.07 : 2003.12.06����
//t			{
				ActDevice &= ~DEV_SPR;
				MaterialOutCheckFlg |= DEV_SPR;					// ��ü�����ļ�����
//t			}
		}
		if ((ActDevice & DEV_MCU) && (!(m_pDevCmn->DeviceTimeout & DEV_MCU)))
		{
//t			if (!m_pDevCmn->fnMCU_GetMaterialInfo())			// ī������ : 2003.11.07 : 2003.12.06����
//t			{
				ActDevice &= ~DEV_MCU;
				MaterialOutCheckFlg |= DEV_MCU;					// ��ü�����ļ�����
//t			}
		}
		if ((ActDevice & DEV_PBM) && (!(m_pDevCmn->DeviceTimeout & DEV_PBM)))
		{
//t			if (!m_pDevCmn->fnPBM_GetMaterialInfo())			// �������� : 2003.11.07 : 2003.12.06����
//t			{
				ActDevice &= ~DEV_PBM;
				MaterialOutCheckFlg |= DEV_PBM;					// ��ü�����ļ�����
//t			}
		}
		if ((ActDevice & DEV_A4P) && (!(m_pDevCmn->DeviceTimeout & DEV_A4P)))
		{
//t			if (!m_pDevCmn->fnA4P_GetMaterialInfo())			// �����Ϳ��� ����
//t			{
				ActDevice &= ~DEV_A4P;
				MaterialOutCheckFlg |= DEV_A4P;					// ��ü�����ļ�����
//t			}
		}
	}

	//#0026
	if ((ActDevice & (DEV_MCU | DEV_PBM)) && (m_pDevCmn->DeviceTimeout & (DEV_MCU | DEV_PBM)))
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:MaterialOutCheckFlg[%x], DevEnDisable[%x] **", MaterialOutCheckFlg, DevEnDisable);
		DevEnDisable |= DEV_MCU;
		DevEnDisable |= DEV_PBM;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:MaterialOutCheckFlg[%x], DevEnDisable[%x]", MaterialOutCheckFlg, DevEnDisable);
		
	if (!(DevEnDisable & DEV_MCU))								// ī�����Ա�������
	{
		ActDevice &= ~DEV_MCU;
		RetractDevice &= ~DEV_MCU;
	}
	if (!(DevEnDisable & DEV_PBM))								// �������Ա�������
	{
		ActDevice &= ~DEV_PBM;
		RetractDevice &= ~DEV_PBM;
	}
/*
	if ((Accept.SerialTranFlag)	&&								// ���Ӱŷ��̰�
		(CardTran)				)								// ī�尡 ������
	{
		ActDevice &= ~DEV_MCU;									// ī��� ȸ������ ����
		RetractDevice &= ~DEV_MCU;
	}
	if ((Accept.SerialTranFlag)	&&								// ���Ӱŷ��̰�
		(PbTran)				)								// ī�尡 ������
	{
		RetractDevice &= ~DEV_PBM;								// ������ ȸ������ ����
	}
*/
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:ActDevice[%x], RetractDevice[%x], DevEnDisable[%x]", ActDevice, RetractDevice, DevEnDisable);

	if (ActDevice)												// ���̼���
	{
/*t---------------------------------------------------------------------------
// �̼�����ְ������� ������.

		if (ActDevice & DEV_SPR)								// ��ǥ�̼���
		{
			m_pDevCmn->fnAPL_StackError("0110091", "��ǥ�̼���", DEV_SPR);
			ErrDevice |= DEV_SPR;
		}
		
		if (ActDevice & DEV_MCU)								// ī��̼���
		{
			m_pDevCmn->fnAPL_StackError("0110191", "ī��̼���", DEV_MCU);
			ErrDevice |= DEV_MCU;
		}
		
		if (ActDevice & DEV_PBM)								// ����̼���(SQA�뺸��)
		{
			m_pDevCmn->fnAPL_StackError("0110591", "����̼���", DEV_PBM);
			ErrDevice |= DEV_PBM;
		}
--------------------------------------------------------------------------- */

		// ī��+����/ī��+����+��ǥ �̼��뿡 ���� ����
		// DevEnDisable Flag�� ����� ��ü�� ���ؼ� �� �̿��� ��ü�� Disable���� �����ϰ� 
		// �� ������ ActDevice�� ���� �����Ѵ�.
		// ���� �̷��� �������� ���ٸ�, ���� ī��+������ �̼���Ǵ� ���� ����.
		// �̷��� ������ ������, 
		// 1. ��ü�ܷ� ������(GetMaterialInfo)���� ������ ������ �̷���� �����ŷ��� ����
		//	  ���Ե� ��ü�� �̼����Ѱ����� ������ ���� �ִ�.
		// 2. ���� �÷��� ����...
		// 3. ��������....
		// 2003.11.10
		
		// 2003.11.07
		if ((ActDevice & (DEV_MCU | DEV_SPR)) == (DEV_MCU | DEV_SPR))	// ī��+��ǥ�̼���
		{
			m_pDevCmn->fnAPL_StackError("0110391", "ī���ǥ�̼���", DEV_MCU | DEV_SPR);
			ErrDevice |= DEV_MCU;
			ErrDevice |= DEV_SPR;
		}
		else
		if ((ActDevice & (DEV_PBM | DEV_SPR)) == (DEV_PBM | DEV_SPR))	// ����+��ǥ�̼���(SQA�뺸��)
		{
			m_pDevCmn->fnAPL_StackError("0110691", "�����ǥ�̼���", DEV_PBM | DEV_SPR);
			ErrDevice |= DEV_PBM;
			ErrDevice |= DEV_SPR;
		}
		else
		if (ActDevice & DEV_MCU)								// ī��̼���
		{
			m_pDevCmn->fnAPL_StackError("0110191", "ī��̼���", DEV_MCU);
			ErrDevice |= DEV_MCU;
		}
		else
		if (ActDevice & DEV_PBM)								// ����̼���(SQA�뺸��)
		{
			m_pDevCmn->fnAPL_StackError("0110591", "����̼���", DEV_PBM);
			ErrDevice |= DEV_PBM;
		}
		else
		if (ActDevice & DEV_SPR)								// ��ǥ�̼���
		{
//			if ((TranProc == TRAN_WITH)	&&						// ��ݼ��ŷ�	: 2003.12.09
//				(m_pDevCmn->TranResult)	)						// HOSTOK�ΰ��
//			{
//				m_pDevCmn->fnAPL_StackError("0110091", "��ǥ�̼���", DEV_SPR);
//				ErrDevice |= DEV_SPR;
//			}
		}
	}

//	if (RetractDevice & DEV_SPR)
	if ((ActDevice & DEV_SPR)		&&
		(RetractDevice & DEV_SPR)	)
	{
		if (m_pDevCmn->fnSPR_GetMaterialInfo())					// ��ǥ����
			fnAPD_MaterialRetract(DEV_SPR);						// ��ǥ����ȸ��
	}
	if (MaterialOutFlg & DEV_SPR)								// ����
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_SPR_ATM_FLICKER, FLICKER_OFF);

		// V03-01-01 ī��ó���� SPL LEDó������(SCDP��������)
		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_SLIP_MAT, FALSE);
	}

	if (RetractDevice & DEV_MCU)
	{
		if (m_pDevCmn->fnMCU_GetMaterialInfo())					// ī������
			fnAPD_MaterialRetract(DEV_MCU);						// ī������ȸ��
	}
	if (MaterialOutFlg & DEV_MCU)								// ����
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);

		// V03-01-01 ī��ó���� SPL LEDó������(SCDP��������)
		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_CARD_MAT, FALSE);
	}

	if (RetractDevice & DEV_PBM) 
	{
		if (m_pDevCmn->fnPBM_GetMaterialInfo())					// ��������
			fnAPD_MaterialRetract(DEV_PBM);						// ��������ȸ��
	}
	if (MaterialOutFlg & DEV_PBM)								// ����
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_OFF);

		// V03-01-01 ī��ó���� SPL LEDó������(SCDP��������)
		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, FALSE);
	}

	if (RetractDevice & DEV_A4P) 
	{
		if (m_pDevCmn->fnA4P_GetMaterialInfo())					// �����Ϳ�������
			fnAPD_MaterialRetract(DEV_A4P);						// �����Ϳ�������ȸ��
	}

	if (MaterialOutFlg & DEV_A4P)								// ����
		m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_OFF);


	// 2006.7.28 ī��� ������ ���� �� ���忡 ���� �÷��װ� �����.
	// �ǹ��� �����ϳ�, �÷��װ� ����� �ڵ����� ���� �ŷ��� �Ѿ��
	// ����� �߻�. �׷��� �ǹ��� �ִٸ� ������ ȸ���ع���.
	// ���⿡ �����ִٸ� �����ٶ�. CSW
//	if (m_pDevCmn->fnPBM_GetMaterialInfo())						// ��������
//	{
//		m_pDevCmn->fnAPL_RetractMaterial(DEV_PBM);
//		m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_OFF);
//
//		// V03-01-01 ī��ó���� SPL LEDó������(SCDP��������)
//		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, FALSE);
//	}

	if (WaitFlg)
		fnAPD_CheckDeviceAction(RetractDevice);

	if (ErrDevice)												// �̼�����ְ���
	{
		m_pDevCmn->DeviceLoss |= ErrDevice;
		fnAPP_CancelProc(T_ERROR);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract(...):return");
	return T_OK;
}

// ��üȸ��
int CTranCmn::fnAPD_MaterialRetract(int Device, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract(Device[%x], WaitFlg[%d])", Device, WaitFlg);

	int		ActDevice = Device;

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		ActDevice &= ~DEV_SPR;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		ActDevice &= ~DEV_MCU;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		ActDevice &= ~DEV_PBM;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_A4P))
		ActDevice &= ~DEV_A4P;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract:ActDevice[%x]", ActDevice);

	if (MaterialOutCheckFlg & DEV_MCU)							// ��ü�����ļ�����
		ActDevice &= ~DEV_MCU;
	if (MaterialOutCheckFlg & DEV_PBM)							// ��ü�����ļ�����
		ActDevice &= ~DEV_PBM;
	if (MaterialOutCheckFlg & DEV_A4P)							// ��ü�����ļ�����
		ActDevice &= ~DEV_A4P;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract:ActDevice[%x], MaterialOutCheckFlg[%x]", ActDevice, MaterialOutCheckFlg);

	if (!(DevEnDisable & DEV_MCU))								// ī�����Ա�������
		ActDevice &= ~DEV_MCU;
	if (!(DevEnDisable & DEV_PBM))								// �������Ա�������
		ActDevice &= ~DEV_PBM;
/*
	if ((Accept.SerialTranFlag)	&&								// ���Ӱŷ��̰�
		(CardTran)				)								// ī�尡 ������
		ActDevice &= ~DEV_MCU;									// ī��� ȸ������ ����
	if ((Accept.SerialTranFlag)	&&								// ���Ӱŷ��̰�
		(PbTran)				)								// ī�尡 ������
		ActDevice &= ~DEV_PBM;									// ������ ȸ������ ����
*/
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract:ActDevice[%x], DevEnDisable[%x]", ActDevice, DevEnDisable);

	fnAPD_CheckDeviceAction(ActDevice);

	if (ActDevice & DEV_SPR)
	{
		if (m_pDevCmn->fnSPR_GetMaterialInfo())					// ��ǥ����
		{
			if (MaterialOutFlg & DEV_SPR)						// ����
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);		// ��ǥȸ��(�̼���)
			else
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);		// ��ǥȸ��(ȸ������)
		}
	}

	if (ActDevice & DEV_MCU)
	{
		if (m_pDevCmn->fnMCU_GetMaterialInfo())					// ī������
		{
			if (MaterialOutFlg & DEV_MCU)						// ����
				m_pDevCmn->fnAPL_RetractMaterial(DEV_MCU);		// ī��ȸ��(�̼���)
			else
			{
//				m_pDevCmn->fnAPL_RetractMaterial(DEV_MCU);		// ī��ȸ��(ȸ������ Kim.Gi.Jin 2008.11.18)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract: Card Capture Retract Ignore When Printer Taken Standby");
				
			}
		}
	}

	if (ActDevice & DEV_PBM) 
	{
		if (m_pDevCmn->fnPBM_GetMaterialInfo())					// ��������
		{
			if (MaterialOutFlg & DEV_PBM)						// ����
				m_pDevCmn->fnAPL_RetractMaterial(DEV_PBM);		// ����ȸ��(�̼���)
			else
				m_pDevCmn->fnAPL_RetractMaterial(DEV_PBM);		// ����ȸ��(ȸ������)
		}
	}

	if (ActDevice & DEV_A4P)
	{
		if (m_pDevCmn->fnA4P_GetMaterialInfo())					// ��ǥ����
		{
			if (MaterialOutFlg & DEV_A4P)						// ����
				m_pDevCmn->fnAPL_RetractMaterial(DEV_A4P);		// ��ǥȸ��(�̼���)
			else
				m_pDevCmn->fnAPL_RetractMaterial(DEV_A4P);		// ��ǥȸ��(ȸ������)
		}
	}

	if (WaitFlg)
		fnAPD_CheckDeviceAction(ActDevice);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract(...):return");
	return T_OK;
}

// ���ݼ�ǥ��ݰ��
int CTranCmn::fnAPD_MoneyCount(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCount(WaitFlg[%d])", WaitFlg);

	int		nDisp10000 = 0;
	int		nDisp50000 = 0;
	int		nGetCurDisp50000 = 0; 
	int		nGetCurDisp10000 = 0;

	int		CashMoney = Asc2Int(Accept.CashMoney, 11);			// ���ݹ���ż�
	int		CheckMoney = Asc2Int(Accept.CheckMoney, 10);		// ��ǥ����ż�
	int		nCurrCheckKind = m_pDevCmn->fnAPL_GetKindOfOutCheck();// ���� ������ ��ǥ����
	
	char	CrntCheckDate[256];									// ��ݼ�ǥ���ڳ���
//	char	AddCheckNo[256];									// ��ǥ����ż�
//	char	CrntCheckNo[256];									// �����ǥ��ȣ
//	int		CrntCheckCnt = 0;									// �����ǥ�ż�
	int		CheckNo1=0, NumOfCheck1=0;
	int		CheckNo2=0, NumOfCheck2=0;
	int		ScrNum = 0;

/////////////////////////////////////////////////////////////////////////////
	CashCountFlg = FALSE;										// ������ݰ������
	CheckCountFlg = FALSE;										// ��ݼ�ǥ�������
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCount:CashMoney[%d], CheckMoney[%d], PMDRecvData.ParaLength[%d]", CashMoney, CheckMoney, PMDRecvData.ParaLength);

/////////////////////////////////////////////////////////////////////////////
	if (CashMoney)
	{
		if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
			return T_OK;
	}

	

	if (CheckMoney)
	{
		switch (m_pProfile->DEVICE.MachineType)
		{
			case U8100:											// U8100-AP����
			case U3100K:
				if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
					return T_OK;
				break;
			default:
				if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))
					return T_OK;
				break;
		}
	}

/////////////////////////////////////////////////////////////////////////////
	// V03-01-01 ī��ó���� SPL LEDó������(SCDP��������)
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_NOTES_MAT, TRUE);

/////////////////////////////////////////////////////////////////////////////

	if ((CashMoney)		&&										// ����+��ǥ
		(CheckMoney)	)
		ScrNum = 841;
	else
	if ((CashMoney)		&&										// ���ݸ�
		(!CheckMoney)	)
		ScrNum = 835;
	else
	if ((!CashMoney)	&&										// ��ǥ��
		(CheckMoney)	)
		ScrNum = 840;

	if (ScrNum)
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum);

	switch (m_pProfile->DEVICE.MachineType)
	{
		case U8100:											// U8100-AP����
		case U3100K:
			if (CashMoney)
			{
//				fnAPD_CheckDeviceAction(DEV_BRM);				// �ӵ����� (�ݾ��Է��ķ� �̵�)
//				m_pDevCmn->fnCSH_ReadyDispense();				// ����غ�
				fnAPD_CheckDeviceAction(DEV_BRM);
/******************************************************************************************/ 
				nGetCurDisp10000 = RegGetInt(_REGKEY_BRMSTATUS, "NDisp10000", 0);
				nGetCurDisp50000 = RegGetInt(_REGKEY_BRMSTATUS, "NDisp50000", 0);
				
				nDisp10000 = Asc2Int(Accept.Cash10T_Money, 11);
				nDisp50000 = Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND;
				
				nGetCurDisp10000 += nDisp10000;
				nGetCurDisp50000 += nDisp50000;

				RegSetInt(_REGKEY_BRMSTATUS, "NDisp10000", nGetCurDisp10000);
				RegSetInt(_REGKEY_BRMSTATUS, "NDisp50000", nGetCurDisp50000);
/******************************************************************************************/
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���ݰ�� : ��%d +����%d = %d����", nDisp10000, nDisp50000, CashMoney));
				CashCountFlg = TRUE;							// ������ݰ����
				if ((m_pDevCmn->Withdraw50TFlag)		&&		
					(WithAvail & TRAN_WITH_50TCASH)		&&
					(Asc2Int(Accept.Cash50T_Money, 15))	)
					m_pDevCmn->fnCSH_DispenseLarge(0, Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND, Asc2Int(Accept.Cash10T_Money, 11));
				else										
					m_pDevCmn->fnCSH_Dispense(CashMoney);		// ��ݰ��(���ݸż�)
			}

			if (CheckMoney)										// ��ݼ�ǥ
			{
				fnAPD_CheckDeviceAction(DEV_UCM);

//				memset(CrntCheckDate, 0, sizeof(CrntCheckDate));// ��ݼ�ǥ���ڳ�������	
				memset(CrntCheckDate, ' ', sizeof(CrntCheckDate));// ��ݼ�ǥ���ڳ�������	
/*				
				sprintf(CrntCheckDate, "%4.4s %2.2s %2.2s", 
						&m_pProfile->TRANS.CheckYYYYMMDD[0],
						&m_pProfile->TRANS.CheckYYYYMMDD[4],
						&m_pProfile->TRANS.CheckYYYYMMDD[6]);
*/
				CheckCountFlg = TRUE;							// ��ݼ�ǥ�����
				m_pDevCmn->fnUCM_Dispense(CheckMoney, CrntCheckDate);

				CheckNo1 =  Asc2Int(m_pDevCmn->fnAPL_GetNumberOfOutCheckNo(), 8);
				CheckNo2 =  Asc2Int(m_pDevCmn->fnAPL_GetNumberOfOutCheckNo2(), 8);
				NumOfCheck1 =  m_pDevCmn->fnAPL_GetNumberOfOutCheck();
				NumOfCheck2 =  m_pDevCmn->fnAPL_GetNumberOfOutCheck2();
				if (CheckMoney >= NumOfCheck1)
				{
					CheckNo2    = CheckNo2    + (CheckMoney - NumOfCheck1);
					NumOfCheck2 = NumOfCheck2 - (CheckMoney - NumOfCheck1);					
					CheckNo1 = 0;
					NumOfCheck1 = 0;
				} 
				else
				{
					CheckNo1    += CheckMoney;
					NumOfCheck1 -= CheckMoney;
				}
				
// 				if(NumOfCheck1 <= 0)
// 				{
// 					CheckNo1	=	CheckNo2 = 0;
// 					NumOfCheck1 =	NumOfCheck2 = 0;
// 				}

				m_pDevCmn->fnAPL_SetNumberOfOutCheckNo(Int2Asc(CheckNo1, 8), Int2Asc(CheckNo2, 8));	// ��ݼ�ǥ��ȣ����
				m_pDevCmn->fnAPL_SetNumberOfOutCheck(NumOfCheck1, NumOfCheck2);						// ��ݼ�ǥ�ż�����
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__,"fnAPD_MoneyCount: CheckNo1[%d], CheckNo2[%d]", CheckNo1, CheckNo2);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__,"fnAPD_MoneyCount: NumOfCheck1[%d], NumOfCheck2[%d]", NumOfCheck1, NumOfCheck2);
			}
			break;
		default:
			break;
	}


	if (WaitFlg)
	{
		if (CashMoney)
			fnAPD_CheckDeviceAction(DEV_BRM);
		if (CheckMoney)
			fnAPD_CheckDeviceAction(DEV_UCM);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCount(...):return");
	return T_OK;
}

// ���ݼ�ǥ���������
int CTranCmn::fnAPD_MoneyOutCheck(int ScrNum, int CheckTime)
{

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck(ScrNum[%d], CheckTime[%d])", ScrNum, CheckTime);

	int		ActDevice = DEV_NONE;
	int		CashRejectMethod = 0;
	int		TimeOverBrmFlg = FALSE;								// ����Ÿ�ӿ���BrmFlg
	int		TimeOverUcmFlg = FALSE;								// ����Ÿ�ӿ���UcmFlg

/////////////////////////////////////////////////////////////////////////////
	MoneyOutFlg = 0;											// ���ݼ�ǥ��������
	MoneyOutCheckFlg = 0;										// ���ݼ�ǥ�����ļ�������
/////////////////////////////////////////////////////////////////////////////

	if (TranProc == TRAN_WITH)									// ���
	{
		if (CashCountFlg)										// ������ݰ����
		{
			ActDevice |= DEV_BRM;
			CashRejectMethod = 0;								// ����
		}
		if (CheckCountFlg)										// ��ݼ�ǥ�����
			ActDevice |= DEV_UCM;
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:ActDevice[%x], CashCountFlg[%d], CheckCountFlg[%d]", ActDevice, CashCountFlg, CheckCountFlg);

	if (TranProc == TRAN_DEP || TranProc == TRAN_HIPASS)									// �Ա�
	{
		if ((Accept.MoneyInKind & TRAN_DEP_CASH)	||			// �Ա�����
			(Accept.MoneyInKind & TRAN_DEP_CHECK));				// �Աݼ�ǥ
		else return T_OK;

		if (1 == CashRejectFlg)										// �Ա����ݹ�ȯ��
		{
			ActDevice |= DEV_BRM;
			CashRejectMethod = 1;								// ��ȯ
		}

		if (CashRejectAFlg)										// �Ա������ʰ���ȯ��
		{
			ActDevice |= DEV_BRM;
			CashRejectMethod = 2;								// �ʰ���ȯ
		}

		if (CheckRejectFlg)										// �Աݼ�ǥ��ȯ��
			ActDevice |= DEV_UCM;
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:ActDevice[%x], CashRejectFlg[%d], CashRejectAFlg[%d], CheckRejectFlg[%d]", ActDevice, CashRejectFlg, CashRejectAFlg, CheckRejectFlg);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		ActDevice &= ~ DEV_BRM;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		ActDevice &= ~ DEV_UCM;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:ActDevice[%x]", ActDevice);

	if (ActDevice & DEV_BRM)
		fnAPD_CheckDeviceAction(DEV_BRM);
	if (ActDevice & DEV_UCM)
		fnAPD_CheckDeviceAction(DEV_UCM);
	
/////////////////////////////////////////////////////////////////////////////
	Delay_Msg(200);												// �������(200ms)
	if ((ActDevice & DEV_BRM)	||
		(ActDevice & DEV_UCM))
	{
/////////////////////////////////////////////////////////////////////////////
		CashCountFlg = FALSE;									// ������ݰ������
		CheckCountFlg = FALSE;									// ��ݼ�ǥ�������
		CashRejectFlg = FALSE;									// �Ա����ݹ�ȯ����
		CashRejectAFlg = FALSE;									// �Ա������ʰ���ȯ����
		CheckRejectFlg = FALSE;									// �Աݼ�ǥ��ȯ����
/////////////////////////////////////////////////////////////////////////////

		if (ScrNum)												// ȭ��������
			;
		else
		if (TranProc == TRAN_DEP || TranProc == TRAN_HIPASS)								// �Ա�
		{
			if (ActDevice & DEV_BRM)
			{
				//#0055
				if(1 == CashRejectFlg)
					ScrNum = 839;									// ���ݹ�ȯ(�Ա����) : ���ݰ�� 
				else
					ScrNum = 835;                               // ���� ���
			}
			else
			if (ActDevice & DEV_UCM)
				ScrNum = 855;									// ��ǥ��ȯ(�Ա����)
		}

		if (ScrNum == 838)										// ���ݹ�ȯ(�ż��ʰ�)
		{
			m_pDevCmn->fnSCR_DisplayNumeric(1, fnAPP_CalcCashMaxIn());
			m_pDevCmn->fnSCR_DisplayNumeric(2, fnAPP_CalcCashMaxInAmount());										//#N0214

			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���Ϳ��� : 1ȸ�Ա��ѵ� �ʰ�"));
		}

		if (ScrNum)												// ȭ����
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum);

/////////////////////////////////////////////////////////////////////////////
		if (ActDevice & DEV_BRM)
			m_pDevCmn->fnLGT_SetFlicker(KU_BRM_ATM_FLICKER, FLICKER_ON);
		if (ActDevice & DEV_UCM)
			m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);

/////////////////////////////////////////////////////////////////////////////
		while (TRUE)
		{
			if (ActDevice & DEV_BRM)
			{
				m_pDevCmn->fnAPL_CaptureHandSave(3, FALSE, fnAPP_GetCaptureData());		// ���Կ�
				m_pDevCmn->fnAPL_CaptureFaceSave(4, FALSE, fnAPP_GetCaptureData());		// ���Կ�
				MoneyOutFlg |= DEV_BRM;							// ���ݹ�����
				m_pDevCmn->DeviceTimeout &= ~DEV_BRM;			// �ð��ʰ������ʱ�ȭ
				if (TranProc == TRAN_WITH)						// ���
				{
					m_pDevCmn->fnCSH_Present(CheckTime);		// ����(������ð�)
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���Ϳ��� : �Ϸ�"));
				}
				else
				{

					if(CashRejectMethod == 1)					// ��ȯ
						m_pDevCmn->fnCSH_Reject(CheckTime);		// ��ȯ(������ð�)
					else
						m_pDevCmn->fnCSH_Present(CheckTime);	// �ʰ���ȯ(������ð�)
				}
			}
			if (ActDevice & DEV_UCM)
			{
				MoneyOutFlg |= DEV_UCM;							// ��ǥ������
				m_pDevCmn->DeviceTimeout &= ~DEV_UCM;			// �ð��ʰ������ʱ�ȭ

////////////////////////////////////////////////////////////////
//				if (TranProc == TRAN_WITH)						// ���
//					m_pDevCmn->fnUCM_Present(CheckTime);		// ����(������ð�)
//				else
//					m_pDevCmn->fnUCM_RejectAll(CheckTime);		// ��ü��ȯ(������ð�)
////////////////////////////////////////////////////////////////
																// V02-06-01-#10 ��ǥ������ ��ǥ�ܷ����� �Ǵ�
				if (TranProc == TRAN_WITH)						// ���
				{
					if (!m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW))
					{											// V02-06-01-#10 ����̰� ��ǥ�� ��ο� ������ ���ó��
						m_pDevCmn->fnAPL_StackError("4699991", "��ݼ�ǥ�����̻�", DEV_UCM);
						m_pDevCmn->DeviceStatus |= DEV_UCM;
						m_pDevCmn->DeviceLoss	|= DEV_UCM;
						fnAPP_CancelProc(T_ERROR);
					}
					else										// V02-06-01-#10 ����̰� ��ǥ�� ��ο� ������ ����ó��
					{
						m_pDevCmn->fnUCM_Present(CheckTime);	// ����(������ð�)
					}
				}
				else
					m_pDevCmn->fnUCM_RejectAll(CheckTime);		// ��ü��ȯ(������ð�)
			}

// U8100 AP ���泻�� #02 - ī�޶� ������ ����
// ����ϰ��� �Ҷ� Ǯ� ���ٶ�
//			m_pDevCmn->fnAPL_CaptureVODSave(VOD_START, VOD_HAND);	
/////////////////////////////////////////////////////////////////////////////
			if (ActDevice & DEV_BRM)
			{
				fnAPD_CheckDeviceAction(DEV_BRM);
				m_pDevCmn->fnAPL_CaptureHandSave(5, FALSE, fnAPP_GetCaptureData());			// ���Կ�

				if((TranProc == TRAN_DEP || TranProc == TRAN_HIPASS) && CashRejectMethod == 1)
					m_pDevCmn->fnSCR_DisplayScreen(839);

				m_pDevCmn->fnCSH_WaitLift();					                        // ���������۴��
				fnAPD_CheckDeviceAction(DEV_BRM);

			}
			if (ActDevice & DEV_UCM)
			{
				fnAPD_CheckDeviceAction(DEV_UCM);
				m_pDevCmn->fnAPL_CaptureHandSave(7);			// ���Կ�
			}

/////////////////////////////////////////////////////////////////////////////
			if (ActDevice & DEV_BRM)
			{
/*************************************************************
				if(!CashInCountFlg)
				{
					TimeOverBrmFlg = TRUE;
					DWORD WaitTime = GetTickCount() + 1000 * K_45_WAIT;
					do
					{
						if(!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))
						{
//							Delay_Msg(500);
							if(!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))
							{
								TimeOverBrmFlg = FALSE;
								break;
							}

						}
						//Sensor polling interval
						//Delay_Msg(500);
						m_pDevCmn->MsgPeekMessageProc(500);

					} while (GetTickCount() < WaitTime);		

				}
**************************************************************************************/				
				if (!(m_pDevCmn->DeviceTimeout & DEV_BRM))		// �ð��ʰ�������
				{

					fnAPD_CheckDeviceAction(DEV_BRM);
					m_pDevCmn->fnCSH_WaitTaken();				// ������
					fnAPD_CheckDeviceAction(DEV_BRM);
					if (m_pDevCmn->DeviceTimeout & DEV_BRM)			// �̼���
						TimeOverBrmFlg = TRUE;
					else											// �������
						TimeOverBrmFlg = FALSE;

/******************************************************************************************
					TimeOverBrmFlg = TRUE;
					DWORD WaitTime = GetTickCount() + 1000 * K_45_WAIT;
					do
					{
						if(!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))
						{
//							Delay_Msg(500);
							if(!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))
							{
								TimeOverBrmFlg = FALSE;
								break;
							}

						}
						//Sensor polling interval
						//Delay_Msg(500);
						m_pDevCmn->MsgPeekMessageProc(500);
						
					} while (GetTickCount() < WaitTime);		
******************************************************************************************************/
				}
				
			}
/////////////////////////////////////////////////////////////////////////////

			if (ActDevice & DEV_UCM)
			{
				fnAPD_CheckDeviceAction(DEV_UCM);
				m_pDevCmn->fnUCM_WaitTaken();					// ������
				fnAPD_CheckDeviceAction(DEV_UCM);
				if (m_pDevCmn->DeviceTimeout & DEV_UCM)			// �̼���
					TimeOverUcmFlg = TRUE;
				else											// �������
					TimeOverUcmFlg = FALSE;
			}

			m_pDevCmn->fnAPL_CaptureHandSave(5, FALSE, fnAPP_GetCaptureData());		// ���Կ�
			m_pDevCmn->fnAPL_CaptureFaceSave(6, FALSE, fnAPP_GetCaptureData());		// ���Կ�

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:"				\
													"ActDevice[%x], "					\
													"m_pDevCmn->DeviceTimeout[%x], "	\
													"TimeOverBrmFlg[%d], "				\
													"TimeOverUcmFlg[%d]", 
													ActDevice, 
													m_pDevCmn->DeviceTimeout, 
													TimeOverBrmFlg, 
													TimeOverUcmFlg);
			fnAPD_CheckDeviceAction(DEV_BRM);					
			m_pDevCmn->fnCSH_CloseShutter();					// ���ʹݱ�
			if (TranProc == TRAN_DEP || TranProc == TRAN_HIPASS)
				fnAPD_CheckDeviceAction(DEV_BRM);


			if ((ActDevice & DEV_BRM)	&&
				(ActDevice & DEV_UCM))
			{
				if ((!TimeOverBrmFlg)	&&						// �������
					(!TimeOverUcmFlg))
				{
					if ((!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))&&	// ��������ݹ�
						(!(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)))	&&	// �Աݼ�ǥ��
						(!(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW))))	// ��ݼ�ǥ��
						break;
				}
				if ((TimeOverBrmFlg)	||						// �̼���(�����̶�)
					(TimeOverUcmFlg))
					break;
			}
			else
			if (ActDevice & DEV_BRM)
			{

				if (!TimeOverBrmFlg)							// �������
				{
					if (!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))		// ��������ݹ�
						break;
				}
				if (TimeOverBrmFlg)								// �̼���
					break;
			}
			else
			if (ActDevice & DEV_UCM)
			{
				if (!TimeOverUcmFlg)							// �������
				{
					if ((!(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)))	&&	// �Աݼ�ǥ��
						(!(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW))))	// ��ݼ�ǥ��
						break;
				}
				if (TimeOverUcmFlg)								// �̼���
					break;
			}
		} 
	}

/////////////////////////////////////////////////////////////////////////////
	if (ActDevice & DEV_BRM)
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_BRM_ATM_FLICKER, FLICKER_OFF);

		// V03-01-01 ī��ó���� SPL LEDó������(SCDP��������)
		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_NOTES_MAT, FALSE);
	}
	
	if (ActDevice & DEV_UCM)
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);

		// V03-01-01 ī��ó���� SPL LEDó������(SCDP��������)
		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_NOTES_MAT, FALSE);
	}

/////////////////////////////////////////////////////////////////////////////
	// ����/��ǥ�� �ܷ����� TRACEó�� 2004.09.15
MsgDump(TRACE_FREE_MODE, "Log", __FILE__, __LINE__, "����ó���� �ܷ�Ȯ��[%2.2X:%2.2X:%2.2X]", 
													m_pDevCmn->fnCSH_GetMaterialInfo(), 
													m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT),
													m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW));
/////////////////////////////////////////////////////////////////////////////
	if (ActDevice & DEV_BRM)
	{
		if (m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM)	// ��������ݹ�
		{
			if (TranProc == TRAN_WITH)
			{
//				m_pDevCmn->fnAPL_StackError("0110291", "������ݹ̼���", DEV_BRM);
				m_pDevCmn->DeviceStatus |= DEV_BRM;
				m_pDevCmn->DeviceLoss	|= DEV_BRM;
			}
			else
			{
//				m_pDevCmn->fnAPL_StackError("0110292", "�Ա����ݹ̼���", DEV_BRM);
				m_pDevCmn->DeviceStatus |= DEV_BRM;
				m_pDevCmn->DeviceLoss	|= DEV_BRM;
			}
//			fnAPP_CancelProc(T_ERROR);
		}
		else
			MoneyOutCheckFlg |= DEV_BRM;						// ���ݹ����ļ�����
	}

	if (ActDevice & DEV_UCM)
	{
		if ((m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)) ||	// �Աݼ�ǥ��
			(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW)))	// ��ݼ�ǥ��
		{
			if (TranProc == TRAN_WITH)
			{
//				m_pDevCmn->fnAPL_StackError("0110491", "��ݼ�ǥ�̼���", DEV_UCM);
				m_pDevCmn->DeviceStatus |= DEV_UCM;
				m_pDevCmn->DeviceLoss	|= DEV_UCM;
			}
			else
			{
//				m_pDevCmn->fnAPL_StackError("0110492", "�Աݼ�ǥ�̼���", DEV_UCM);
				m_pDevCmn->DeviceStatus |= DEV_UCM;
				m_pDevCmn->DeviceLoss	|= DEV_UCM;
			}
//			fnAPP_CancelProc(T_ERROR);
		}
		else
			MoneyOutCheckFlg |= DEV_UCM;						// ��ǥ�����ļ�����
	}

	if ((ActDevice & (DEV_UCM | DEV_BRM))			&&
		(m_pDevCmn->DeviceLoss & (DEV_UCM | DEV_BRM)))
	{
		if (TranProc == TRAN_WITH)
		{
			switch(m_pDevCmn->DeviceLoss & (DEV_UCM | DEV_BRM))
			{
			case DEV_BRM:
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���ʹ��� : ���ݹ̼���"));
				m_pDevCmn->fnAPL_StackError("0110291", "������ݹ̼���", DEV_BRM);
				fnAPP_WithErrSumProc(3);
				break;
			case DEV_UCM:
				m_pDevCmn->fnAPL_StackError("0110491", "��ݼ�ǥ�̼���", DEV_UCM);
				break;
			case DEV_BRM | DEV_UCM :
				m_pDevCmn->fnAPL_StackError("0110991", "������ݼ�ǥ�̼���", DEV_BRM | DEV_UCM);
				break;
			default:
				break;
			}
		}
		else
		if (TranProc == TRAN_DEP || TranProc == TRAN_HIPASS)	//#N0271
		{
			switch(m_pDevCmn->DeviceLoss & (DEV_UCM | DEV_BRM))
			{
			case DEV_BRM:
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���ʹ��� : �Ա����ݹ̼���"));
				m_pDevCmn->fnAPL_StackError("0110292", "�Ա����ݹ̼���", DEV_BRM);
				fnAPP_DepErrSumProc(3);
				break;
			case DEV_UCM:
				m_pDevCmn->fnAPL_StackError("0110492", "�Աݼ�ǥ�̼���", DEV_UCM);
				break;
			case DEV_BRM | DEV_UCM :
				m_pDevCmn->fnAPL_StackError("0110992", "�Ա����ݼ�ǥ�̼���", DEV_BRM | DEV_UCM);
				break;
			default:
				break;
			}
		}
		
		fnAPD_MaterialRetract(MaterialOutFlg & (DEV_MCU | DEV_PBM | DEV_SPR));	// 2004.07.09
		fnAPP_CancelProc(T_ERROR);
	}

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���ʹ��� : ���� ���� �Ϸ�"));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:MoneyOutCheckFlg[%d]", MoneyOutCheckFlg);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck(...):return");
	return T_OK;
}

// ���ݼ�ǥȸ��
int CTranCmn::fnAPD_MoneyRetract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyRetract()");

	int		ActDevice = (DEV_BRM | DEV_UCM);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		ActDevice &= ~ DEV_BRM;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		ActDevice &= ~ DEV_UCM;

	if (ActDevice & DEV_BRM)
		fnAPD_CheckDeviceAction(DEV_BRM);
	if (ActDevice & DEV_UCM)
		fnAPD_CheckDeviceAction(DEV_UCM);

	Delay_Msg(200);												// �������(200ms)
	if (ActDevice & DEV_BRM)
	{
		if (m_pDevCmn->fnCSH_GetMaterialInfo())					// �����������
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyRetract()_1");
			m_pDevCmn->fnAPL_RetractMaterial(DEV_BRM);			// ����ȸ��
		}
	}

	if (ActDevice & DEV_UCM)
	{
		if ((m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)) ||	// �Աݼ�ǥ��
			(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW)))	// ��ݼ�ǥ��
		{
			m_pDevCmn->fnAPL_RetractMaterial(DEV_UCM);			// ��ǥȸ��
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyRetract():return");
	return T_OK;
}

// �Ա�����
int CTranCmn::fnAPD_CashDeposit()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDeposit()");

	int		ScrNum = 0;
	int		CashInCnt = 0;										// �Ա���������Ƚ��
	int		ExitFlag = FALSE;
	CString	strReturn("");
	int		TimeOverFlg = FALSE;								// ����Ÿ�ӿ���Flg
	int		nDepAvail = 0;										// V06-01-01-#29:�Աݰ��ɱ���
	int		n50000DevAvail = FALSE;								// V06-01-01-#29:5������ �Աݰ��ɿ���
	int		nReturn = -1;										// CSM���� ��ȸ
	BOOL	bNotePosion = FALSE;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.CashMoney, '0', sizeof(Accept.CashMoney));	// �ݾ�(������)/�Ա�����
	CashInCountFlg = FALSE;										// �Ա����ݰ������
	CashRejectFlg = FALSE;										// �Ա����ݹ�ȯ����
	CashRejectAFlg = FALSE;										// �Ա������ʰ���ȯ����
	memset(&CashData, 0, sizeof(CashData));						// �ʱ�ȭ
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�ԱݺҰ� : BRM ���Ұ� ����"));
		return T_OK;
	}

	if ((Accept.MoneyInKind & TRAN_DEP_CASH) != TRAN_DEP_CASH)	// �Ա����ݺҰ���
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_BRM);
	m_pDevCmn->fnCSH_ReadyCount();								// �Ա��غ�         
	fnAPD_CheckDeviceAction(DEV_BRM);                           // #N0190 (CashInEvent Ready)
	
/////////////////////////////////////////////////////////////////////////////
	ScrNum = 831;												// �Ա�����

	nDepAvail = m_pDevCmn->fnCSH_GetAvailDeposit();				// �Աݰ��ɱ���
	if ((m_pProfile->DEVICE.CashHandler == HBRM_SET)        &&	//
		(m_pDevCmn->ChangeBCFlag)							&&	// 5������ ��ü����
		((m_pProfile->BRM.DepositCashKind & 0x08) == 0x08)	)	// 5������ ���ɿ���
//		(m_pDevCmn->Withdraw50TFlag)						)	// 5������ ī��Ʈ����
		n50000DevAvail = TRUE;

	
	m_pDevCmn->fnLGT_SetFlicker(KU_BRM_ATM_FLICKER, FLICKER_ON);
	if (!CashInCnt)												// ù���Խ�
	{
		m_pDevCmn->fnCSH_OpenShutter();							// ����
		fnAPD_CheckDeviceAction(DEV_BRM);
	}
//////////////////////////////////////////////////////////////////////////

	CashInCnt = 0;												// �Ա���������Ƚ��
	ExitFlag = FALSE;
	while ((CashInCnt <= BRM_CASHINRETRY) && (!ExitFlag))		// �Ա����������԰���Ƚ��
	{
		if ((CashInCnt)	&&										// �����Խ�
			(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM) &&
			(FALSE == bNotePosion))								// ���Ա�������
		{
			nDepAvail = m_pDevCmn->fnCSH_GetAvailDeposit();
			switch (nDepAvail)
			{
				case DEP_CASH_ONLY:
					if ((n50000DevAvail)			&&
						(m_pDevCmn->Withdraw50TFlag))			// 5��+1������ �Աݰ���
					{
						m_pDevCmn->fnSCR_DisplayString(1, "������/5�����Ǹ� �Ա��� �����մϴ�");
						m_pDevCmn->fnSCR_DisplayString(12, "��ױ��Աݰ���");
					}
					else
						m_pDevCmn->fnSCR_DisplayString(1, "�����Ǹ� �Ա��� �����մϴ�");
					break;
				case DEP_ONLY_FIFTY_TH:
					if (n50000DevAvail)
					{
						if (m_pDevCmn->Withdraw50TFlag)
							m_pDevCmn->fnSCR_DisplayString(1, "5�����Ǹ� �Ա��� �����մϴ�");
						
						m_pDevCmn->fnSCR_DisplayString(12, "��ױ��Աݰ���");
					}
					break;
				case DEP_ONLY_TEN_TH:
					m_pDevCmn->fnSCR_DisplayString(1, "�����Ǹ� �Ա��� �����մϴ�");
					break;
				case DEP_FIFTY_TH :								// V06-01-01-#17-start
					m_pDevCmn->fnSCR_DisplayString(12, "��ױ��Աݰ���");
					break;
				case DEP_BOTH_AVAILABLE:								
					if (n50000DevAvail)
						m_pDevCmn->fnSCR_DisplayString(12, "��ױ��Աݰ���");
					break;										// V06-01-01-#17-end
				default:
					break;
			}
			
//////////////////////////////////////////////////////////////////////////
			bNotePosion = FALSE;								// ������ �ڼ��̻� OFF
//////////////////////////////////////////////////////////////////////////
			m_pDevCmn->fnSCR_DisplayScreen(839);				// ���ݹ�ȯ
			//#0039
			if(m_pDevCmn->AtmDefine.MachineType == U3100K)
			{
				m_pDevCmn->fnBRM_OpenShutter();	
				fnAPD_CheckDeviceAction(DEV_BRM);
				TimeOverFlg = TRUE;
				DWORD WaitTime = GetTickCount() + 1000 * K_45_WAIT;
				do
				{
					if(!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))
					{
						if(!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))
						{
							TimeOverFlg = FALSE;  
							break;						
						}
					}
					m_pDevCmn->fnUTL_MsgPeekMessageProc(500);
				} while (GetTickCount() < WaitTime);
				
				m_pDevCmn->fnBRM_CloseShutter();	
				fnAPD_CheckDeviceAction(DEV_BRM);
			
				if ((CashInCnt >= BRM_CASHINRETRY) || (TimeOverFlg) ||                         // ��ȯ, ����(����) - #N0190		
					(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))	
				{													// ���Ա�������
/*
					m_pDevCmn->fnCSH_CloseShutter();				// �ݱ�
					fnAPD_CheckDeviceAction(DEV_BRM);
					m_pDevCmn->fnLGT_SetFlicker(KU_BRM_ATM_FLICKER, FLICKER_OFF);
*/
					if (m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM)
					{												// ���Ա�������
						if (!TimeOverFlg)							// ��������
						{
							continue;								// ���ȯ(Ȯ��������)
						}
						else
						{
							m_pDevCmn->fnAPL_StackError("0110293", "�Ա����ݹ̼���", DEV_BRM);
							m_pDevCmn->DeviceStatus |= DEV_BRM;
							m_pDevCmn->DeviceLoss	|= DEV_BRM;
							fnAPP_CancelProc(T_TIMEOVER);			// �̼���ó��
						}
					}
					else
					{
						ExitFlag = TRUE;				
						continue;									// ��ȯ, ����(����)
					}
				}
				

				if((CashInCnt < BRM_CASHINRETRY) && (ScrNum == 832 || ScrNum == 833))
				{
					if(!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM) && !(TimeOverFlg))  //�Ա� �� ������ ���� - #N0190
					{
						int nTotalCount = CashData.mFiftyThousand + CashData.mTenThousand + CashData.mFiveThousand + CashData.mOneThousand;
						if(nTotalCount == fnAPP_CalcCashMaxIn())
						{
							ExitFlag = TRUE; 
							break;
						}
						else
						{
							m_pDevCmn->fnBRM_OpenShutter();	
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���Ϳ���_[%d] : ������", CashInCnt));	
							fnAPD_CheckDeviceAction(DEV_BRM);
						}
					}
				}

			}
			else
			if(m_pDevCmn->AtmDefine.MachineType == U8100)
			{
				m_pDevCmn->DeviceTimeout &= ~DEV_BRM;				// �ð��ʰ������ʱ�ȭ
				m_pDevCmn->fnCSH_Present2();						// ����(������ð�):�ݱ����
				fnAPD_CheckDeviceAction(DEV_BRM);
				m_pDevCmn->fnCSH_WaitTaken();						// ������
				fnAPD_CheckDeviceAction(DEV_BRM);
				
				if (m_pDevCmn->DeviceTimeout & DEV_BRM)				// �̼���
					TimeOverFlg = TRUE;
				else												// ��������
					TimeOverFlg = FALSE;
	
				if ((TimeOverFlg) || (CashInCnt >= BRM_CASHINRETRY))	
				{												
					if ((CashInCnt >= BRM_CASHINRETRY) ||		// ��ȯ, ����(����)
						(m_pDevCmn->fnBRM_GetMaterialInfo() & BRM_IN_CSM))	
					{											// ���Ա�������
						m_pDevCmn->fnBRM_CloseShutter();		// �ݱ�
						fnAPD_CheckDeviceAction(DEV_BRM);
						m_pDevCmn->fnLGT_SetFlicker(KU_BRM_ATM_FLICKER, FLICKER_OFF);
						if (m_pDevCmn->fnBRM_GetMaterialInfo() & BRM_IN_CSM)
						{										// ���Ա�������
							if (!TimeOverFlg)					// ��������
							{
								ExitFlag = TRUE;				// HBRM				
//							    continue;						// ���ȯ(Ȯ��������)
								break;							// �������� �ҽ�����(20061214 �����̻� ����) 
							}
							else
							{
								m_pDevCmn->fnAPL_StackError("0110293", "�Ա����ݹ̼���", DEV_BRM);
								m_pDevCmn->DeviceStatus |= DEV_BRM;
								m_pDevCmn->DeviceLoss	|= DEV_BRM;
								fnAPP_CancelProc(T_ERROR);		// �̼���ó��
							}
						}
						else
						{
							ExitFlag = TRUE;				
							continue;							// ��ȯ, ����(����)
						}
					}
				}

				if((CashInCnt < BRM_CASHINRETRY) && (ScrNum == 832 || ScrNum == 833))
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���Ϳ���_[%d] : ������", CashInCnt));	
			}
			else
			{
				fnAPP_CancelDepRejectProc(T_MSG, "���� ���� ���� �Դϴ�","��Ȳ�ǿ� �����Ͽ� �ֽñ� �ٶ��ϴ�.");				
			}
		}

		if (ScrNum == 831)										// �Ա�����
		{
			m_pDevCmn->fnSCR_DisplayNumeric(1, fnAPP_CalcCashMaxIn());												// �����Ǹ� �Աݰ��ɾȳ�
			m_pDevCmn->fnSCR_DisplayNumeric(2, fnAPP_CalcCashMaxInAmount());										//#N0214

			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���Ϳ��� : �Ա�"));	// #H0257
		}

		if (ScrNum == 833)										// ��������(�ҷ�����/�ݾ�Ȯ��)
		{
			m_pDevCmn->fnSCR_DisplayAmount(1, CashData.mTotalAmount);
			if (m_pDevCmn->fnCSH_GetAvailDeposit() == DEP_ONLY_TEN_TH)
			{													// �����Ǹ� �Աݰ��ɾȳ�
				m_pDevCmn->fnSCR_DisplayString(2, "�����Ǹ� �Ա��� �����մϴ�");
			}
		}
	
		switch (m_pDevCmn->fnCSH_GetAvailDeposit()) 
		{
			case DEP_CASH_ONLY:
			case DEP_ONLY_FIFTY_TH:
			case DEP_FIFTY_TH :									
			case DEP_BOTH_AVAILABLE:							
				if ((HBRM_SET == m_pProfile->DEVICE.CashHandler)    &&
					(m_pDevCmn->ChangeBCFlag)						&&
					((m_pProfile->BRM.DepositCashKind & 0x08) == 0x08))
				{
					m_pDevCmn->fnSCR_DisplayString(12, "��ױ��Աݰ���");
				}
				break;
			default:
				break;
		}
		
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE1);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if ((strReturn == S_SHUTTERCLOSE)	||					// �����Է�
			(strReturn == S_CONFIRM)		||					// Ȯ���Է�
			(strReturn == S_CONFIRM2)		||                  // OK�Է�
			(strReturn == S_TIMEOVER))					        // TIMEOVER (����ó��) #N0190	
		{
			m_pDevCmn->fnAPL_CaptureHandSave(3, FALSE, fnAPP_GetCaptureData());		// ���Կ�
			m_pDevCmn->fnAPL_CaptureFaceSave(4, FALSE, fnAPP_GetCaptureData());		// ���Կ�

			m_pDevCmn->fnLGT_SetFlicker(KU_BRM_ATM_FLICKER, FLICKER_OFF);
			fnAPD_CashDepositCount();							// �Ա����ݰ��
			if (CashData.mTotalAmount)
				CashInCountFlg = TRUE;							// �Ա����ݰ����
			fnAPD_CheckDeviceAction(DEV_BRM);
			switch (CashData.mStatus)							// �������ó��
			{
				case NORMAL:									// ���ݰ���Ϸ�
					ExitFlag = TRUE;
					break;

				case NO_MONEY:									// ���ݹ�
					ExitFlag = TRUE;
					break;

				case SKEW_NOTE:									// ��������
					ScrNum = 832;								// ����������(�ڼ��̻�)
					if (CashData.mTotalAmount)					// �ݾ���
						ScrNum = 833;							// ����������(�ҷ�����/�ݾ�Ȯ��)
					break;

				case UNKNOWN_MONEY:								// ����������������(SKEW����)
					ScrNum = 832;								// ����������(�ҷ�����)
					if (CashData.mTotalAmount)					// �ݾ���
						ScrNum = 833;							// ����������(�ҷ�����/�ݾ�Ȯ��)
					break;
				
				case MAX_OVER:									// �Աݸż��ʰ�
					CashRejectAFlg = TRUE;						// �Ա������ʰ���ȯ��				    
					fnAPD_MoneyOutCheck(838);					// ���ݹ�ȯ(�ż��ʰ�)
					ExitFlag = TRUE;
					break;

				case DOWN:										// ��ֹ߻�
				default:
					fnAPD_CheckDeviceAction(DEV_BRM);
					fnAPP_CancelProc(T_PROGRAM);
					break;
			}
		}
		else
		{
			m_pDevCmn->fnCSH_CloseShutter();					// �ݱ�
			fnAPD_CheckDeviceAction(DEV_BRM);
			m_pDevCmn->fnLGT_SetFlicker(BRM_FLICKER, FLICKER_OFF);
			if (strReturn == S_CANCEL)
				fnAPP_CancelDepRejectProc(T_CANCEL);
			else fnAPP_CancelDepRejectProc(T_TIMEOVER);

		}

		CashInCnt++;											// �Ա���������Ƚ������
	}

	m_pDevCmn->fnCSH_CloseShutter();							// �ݱ�:���ݹ���
	fnAPD_CheckDeviceAction(DEV_BRM);

	
	if (!m_pDevCmn->ChangeBCFlag)								// V06-01-01-#11
		CashData.mFiftyThousand = 0;							// V06-01-01-#11


	if ((m_pProfile->BRM.DepositCashKind & 0x08) != 0x08)		
		CashData.mFiftyThousand = 0;							


	if((CashData.mTenThousand + CashData.mFiveThousand + CashData.mOneThousand + CashData.mFiftyThousand) > fnAPP_CalcCashMaxIn())	
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelDepRejectProc(T_MSG, "�Ա� ���� �ż� �ʰ��Դϴ�");
		else
			fnAPP_CancelDepRejectProc(T_CANCEL);
	}


//	if (!m_pDevCmn->fnCSH_GetMaterialInfo())					// ���ݹ�
//	{
//		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
//			fnAPP_CancelDepRejectProc(T_MSG, "�Աݵ� ������ �����ϴ�");
//		else
//			fnAPP_CancelDepRejectProc(T_CANCEL);
//	}

	if (!CashData.mTotalAmount)									// �ݾ׹�
	{
		if ((Accept.MoneyInKind & TRAN_DEP_CHECK)	&&			// ��ǥ�Ա���
			(CheckInfoCnt))										// ��ǥ�ݾ���
		{
			Accept.MoneyInKind &= ~TRAN_DEP_CASH;				// �����Աݹ�ó��
		}
		else
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�Ա� �ݾ� ����"));
			fnAPP_CancelDepRejectProc(T_MSG, "�Աݵ� �ݾ��� �����ϴ�");
		}
		else
			fnAPP_CancelDepRejectProc(T_CANCEL);
	}
																
	if (CashData.mTotalAmount)									// �ݾ��� : 2003.11.01
	{
		if (m_pDevCmn->fnCSH_GetMaterialInfo() != IN_NSTK)		// �Ͻý���Ŀ������������üũ
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("������ �ʰ�(3ȸ)"));
				fnAPP_CancelDepRejectProc(T_MSG, "�Ա� ������ Ƚ�� �ʰ��Դϴ�(3ȸ�̻�)");
			}
			else
				fnAPP_CancelDepRejectProc(T_CANCEL);
		}
		else
		{
			if ((CheckInfoAmt + CashData.mTotalAmount) <= Accept.ChargeCommission)
				fnAPP_CancelDepRejectProc(T_MSG, "������ݾ� ������ �Ա��� �ݾ��� �����ϴ�.");
		}
	}

	Int2Asc(CashData.mTotalAmount, Accept.CashMoney, 15, 10, '0');

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDeposit():return");
	return T_OK;												// �Ա����ݱݾ�Set
}


// �Ա����ݰ��
int CTranCmn::fnAPD_CashDepositCount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositCount()");

	int		UnknownCnt = 0;										// �Աݰ���Ƚ��
	int		ExitFlag = FALSE;
	CString			strCashData("");
	CStringArray	strCashDataArray;

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CASH) != TRAN_DEP_CASH)	// �Ա����ݺҰ���
		return T_OK;


	UnknownCnt = 0;												// �Աݰ���Ƚ��
	ExitFlag = FALSE;

	if (!m_pDevCmn->ChangeBCFlag)								// V06-01-01-#11
		m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_50000, FALSE);
	
	while ((UnknownCnt < BRM_UNKNOWNRETRY) && (!ExitFlag))		// �Ա����ݰ�������Ƚ��
	{
		if (((OKI_SET == m_pProfile->DEVICE.CashHandler) ||
			(HBRM_SET == m_pProfile->DEVICE.CashHandler))&&	// U8100-AP����
			(m_pDevCmn->ChangeBCFlag))							// V06-01-01-#03
		{														
			m_pDevCmn->fnCSH_AcceptLarge(fnAPP_CalcCashMaxIn(), fnAPP_CalcCashMaxInAmount()*10000);
		}
		else
			m_pDevCmn->fnCSH_Accept(fnAPP_CalcCashMaxIn());		// ����(�Աݰ������ݸż�)
		
		fnAPD_CheckDeviceAction(DEV_BRM);
		CashData.mStatus = m_pDevCmn->fnCSH_GetAcceptCountStatus();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositCount:CashData.mStatus[%d]", CashData.mStatus);
		switch (CashData.mStatus)								// ������ۻ���ó��
		{
			case NORMAL:										// ���ݰ�����ۿϷ�
				m_pDevCmn->fnSCR_DisplayScreen(835);			// ���ݰ��(�Ա�)
				m_pDevCmn->fnCSH_Count();						// �Աݰ��
				fnAPD_CheckDeviceAction(DEV_BRM);
				strCashData = m_pDevCmn->fstrBRM_GetCountResult();	// ������
				SplitString(strCashData, ",", strCashDataArray);	// �ݾ״���
				CashData.mStatus = Asc2Int(strCashDataArray[0].GetBuffer(0), strCashDataArray[0].GetLength());
				CashData.mTotalAmount	= Asc2Int(strCashDataArray[1].GetBuffer(0), strCashDataArray[1].GetLength());
				CashData.mTenThousand   = Asc2Int(strCashDataArray[2].GetBuffer(0), strCashDataArray[2].GetLength());
				CashData.mFiveThousand  = Asc2Int(strCashDataArray[3].GetBuffer(0), strCashDataArray[3].GetLength());
				CashData.mOneThousand   = Asc2Int(strCashDataArray[4].GetBuffer(0), strCashDataArray[4].GetLength());

				if (m_pDevCmn->ChangeBCFlag)				
					CashData.mFiftyThousand	= Asc2Int(strCashDataArray[5].GetBuffer(0), strCashDataArray[5].GetLength());	
				else
					CashData.mFiftyThousand = 0;				

				CashData.mUnknown		= Asc2Int(strCashDataArray[6].GetBuffer(0), strCashDataArray[6].GetLength());
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositCount:"		\
						"CashData.mStatus[%d], "		\
						"CashData.mTotalAmount[%d], "	\
						"CashData.mFiftyThousand[%d], "	\
						"CashData.mTenThousand[%d], "	\
						"CashData.mFiveThousand[%d], "	\
						"CashData.mOneThousand[%d], "   \
						"CashData.mUnknown[%d]",
						CashData.mStatus,
						CashData.mTotalAmount,
						CashData.mFiftyThousand,
						CashData.mTenThousand,
						CashData.mFiveThousand,
						CashData.mOneThousand,
						CashData.mUnknown);

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��� S:%d TOT:%d 50K:%d 10K:%d 5K:%d 1K:%d UK:%d",
						CashData.mStatus,
						CashData.mTotalAmount,
						CashData.mFiftyThousand,
						CashData.mTenThousand,
						CashData.mFiveThousand,
						CashData.mOneThousand,
						CashData.mUnknown));


				if (m_pDevCmn->ChangeBCFlag)								
				{
					if (CashData.mTotalAmount !=				// �ݾ�üũ
						((CashData.mFiftyThousand * 50000) +
						 (CashData.mTenThousand	  * 10000) +
						 (CashData.mFiveThousand  *  5000) + 
						 (CashData.mOneThousand   *  1000)))
						fnAPP_CancelProc(T_PROGRAM);
				}

				fnAPD_CashDepositPrint();						// ���ݰ����������

				switch (CashData.mStatus)
				{
					case UNKNOWN_MONEY:							// ����������������(SKEW����)
						break;									// �������ݰ���

					case DOWN:									// ��ֹ߻�
						fnAPD_CheckDeviceAction(DEV_BRM);
					case MAX_OVER:								// �Աݸż��ʰ�
					case NORMAL:								// ���ݰ���Ϸ�
					default:
						ExitFlag = TRUE;
						break;
				}
				break;

			case DOWN:											// ��ֹ߻�
				fnAPD_CheckDeviceAction(DEV_BRM);
			case NO_MONEY:										// ���ݹ�
			case SKEW_NOTE:										// ��������
			default:
				ExitFlag = TRUE;
				break;
		}

		UnknownCnt++;											// �Ա����ݰ�������Ƚ������
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositCount():return");
	return T_OK;
}

// �Ա����ݼ���
int CTranCmn::fnAPD_CashDepositStore(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositStore(WaitFlg[%d])", WaitFlg);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CASH) != TRAN_DEP_CASH)	// �Ա����ݺҰ���
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_BRM);

	m_pDevCmn->fnAPL_CaptureHandSave(7, FALSE, fnAPP_GetCaptureData());		// ���Կ�
	m_pDevCmn->fnAPL_CaptureFaceSave(8, FALSE, fnAPP_GetCaptureData());		// ���Կ�	

	m_pDevCmn->fnCSH_Stack();									// ����
	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_BRM);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositStore(...):return");
	return T_OK;
}

// �Ա����ݹ�ȯ
int CTranCmn::fnAPD_CashDepositReject()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositReject()");

/////////////////////////////////////////////////////////////////////////////
	CashRejectFlg = FALSE;										// �Ա����ݹ�ȯ����
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CASH) != TRAN_DEP_CASH)	// �Ա����ݺҰ���
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_BRM);

	// 5FF51 ��å Start: 2006.11.28 --------------------------------------------------
	if (m_pDevCmn->fnBRM_GetMaterialInfo())                 // �������縸Check
		CashRejectFlg = TRUE;                               // �Ա����ݹ�ȯ��
	// 5FF51 ��å End: 2006.11.28 --------------------------------------------------

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositReject():CashRejectFlg[%d]", CashRejectFlg);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositReject():return");
	return T_OK;
}

// �Աݼ�ǥ
int CTranCmn::fnAPD_CheckDeposit()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit()");

	int		ScrNum = 0;
	int		UnknownCnt = 0;										// �Աݰ���Ƚ��
	int		ExitFlag = FALSE;
	CString	strReturn("");
	int		nReturn = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.CheckMoney, '0', sizeof(Accept.CheckMoney));	// �ݾ�(��ǥ��)/�Աݼ�ǥ
	CheckInCountFlg = FALSE;									// �Աݼ�ǥ�������
	CheckRejectFlg = FALSE;										// �Աݼ�ǥ��ȯ����
	CheckRejectAFlg = FALSE;									// �Աݼ�ǥ������ȯ����
	CheckRejectKind = 0;										// �Աݼ�ǥ��ȯ����
	memset(CheckInfoData, '0', sizeof(CheckInfoData));			// �ʱ�ȭ
	CheckInfoStats = NORMAL;									// �Աݼ�ǥ����
	CheckInfoCnt = 0;											// �Աݼ�ǥ�ż�
	CheckInfoAmt = 0;											// �Աݼ�ǥ�ݾ�
/////////////////////////////////////////////////////////////////////////////

	fnAPP_CheckDepositPrintEdit();								// �Աݼ�ǥ�輭��������

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)// �Աݼ�ǥ�Ұ���
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_UCM);

/////////////////////////////////////////////////////////////////////////////
	CheckInfoCnt = 0;											// �Աݼ�ǥ�ż�
	UnknownCnt = 0;												// �Աݰ���Ƚ��
	ExitFlag = FALSE;
	while ((CheckInfoCnt < fnAPP_CalcCheckMaxIn())	&&			// �Աݼ�ǥ���ɸż�
		   (UnknownCnt < UCM_UNKNOWNRETRY)			&&			// �Աݼ�ǥ��������Ƚ��
		   (!ExitFlag))	
	{
		if (!CheckInfoCnt)
			ScrNum = 851;										// �Աݼ�ǥ
		else
			ScrNum = 852;										// �Աݼ�ǥ(����)

		if (ScrNum == 851)										// �Աݼ�ǥ
		{
			m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("�Ϳ����� 12��00��"));
		}
		if (ScrNum == 852)										// �Աݼ�ǥ(����)
		{
			m_pDevCmn->fnSCR_DisplayNumeric(1, CheckInfoCnt);
			m_pDevCmn->fnSCR_DisplayAmount(2, CheckInfoAmt + CashData.mTotalAmount);
			m_pDevCmn->fnSCR_DisplayNumeric(3, fnAPP_CalcCheckMaxIn() - CheckInfoCnt);
		}

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
		m_pDevCmn->DeviceTimeout &= ~DEV_UCM;					// �ð��ʰ������ʱ�ȭ
		m_pDevCmn->fnUCM_Accept();								// �����㰡(���Դ��ð�)�׿���
																// 65Sec�����(ȭ�鿡�������´�)

		if (m_pDevCmn->EarPhoneInserted == TRUE)	
			m_pDevCmn->fnPIN_EntryEnable(PINMODE_NORMAL, 0, 100, FALSE, PIN_NUMERIC_PAD, "", "",  -1);

		nReturn = m_pDevCmn->fnCMN_ScanDeviceAction(DEV_UCM | DEV_SCR, K_65_WAIT, EVENT_IN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit:nReturn[%d], ", nReturn);
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
		if (nReturn == R_TIMEOUT)
		{
			fnAPD_CheckDeviceAction(DEV_UCM);
			m_pDevCmn->fnUCM_CancelAccept();					// ���Ա����״ݱ�
			fnAPD_CheckDeviceAction(DEV_UCM);
			fnAPP_CancelDepRejectProc(T_TIMEOVER);				// ����Աݹ�ȯ�Լ�(�������ó��)
		}

		if (nReturn == DEV_UCM)
		{
			fnAPD_CheckDeviceAction(DEV_UCM);
			fnAPD_CheckDepositCount();							// �Աݼ�ǥ���
			if (CheckInfoAmt)
				CheckInCountFlg = TRUE;							// �Աݼ�ǥ�����
			fnAPD_CheckDeviceAction(DEV_UCM);
			switch (CheckInfoStats)								// �������ó��
			{
				case NORMAL:									// ���ݰ���Ϸ�
					UnknownCnt = 0;
					break;

				case READ_ERROR:								// ��ǥ�����ǵ�
					if (CheckRejectKind == 3)					// Ÿ���Աݼ�ǥ�Ұ�
						fnAPD_CheckDepositCurOutCheck(855);		// ��ǥ������ȯ(���ϼ�ǥ)
					else
					if (CheckRejectKind == 2)					// Ÿ���Աݼ�ǥ�Ұ�
						fnAPD_CheckDepositCurOutCheck(855);		// ��ǥ������ȯ(Ÿ���ǥ)
					else
					{
						Delay_Msg(500);							// 2003.12.03
						fnAPD_CheckDepositCurOutCheck(854);		// ��ǥ������ȯ(�ҷ���ǥ)
					}
					UnknownCnt++;
					break;

				case READ_TIME_OUT:								// ��ǥ����TimeOver
					fnAPP_CancelDepRejectProc(T_TIMEOVER);		// ����Աݹ�ȯ�Լ�(�������ó��)
					break;

				case DOWN:										// ��ֹ߻�
				default:
					fnAPD_CheckDeviceAction(DEV_UCM);
					fnAPP_CancelProc(T_PROGRAM);
					break;
			}
		}
		else
		if (nReturn == DEV_SCR)
		{
			strReturn = m_pDevCmn->fstrSCR_GetKeyString(FALSE);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit:strReturn[%s], ", strReturn);

			if (strReturn == S_CONFIRM)							// Ȯ���Է�
			{
				fnAPD_CheckDeviceAction(DEV_UCM);
				m_pDevCmn->fnUCM_CancelAccept();				// ���Ա����״ݱ�
				fnAPD_CheckDeviceAction(DEV_UCM);				
				ExitFlag = TRUE;								// �Աݼ�ǥ����
			}
			else
			{
				fnAPD_CheckDeviceAction(DEV_UCM);
				m_pDevCmn->fnUCM_CancelAccept();				// ���Ա����״ݱ�
				fnAPD_CheckDeviceAction(DEV_UCM);
				if (strReturn == S_CANCEL)
				{
					if (!CheckInCountFlg)						// U8100-�������� ��ִ�å-start
					{
						m_pDevCmn->fnUCM_CloseShutter();		// �ݱ�:�ִ��ԱݿϷ��
						fnAPD_CheckDeviceAction(DEV_UCM);
						Delay_Msg(2*1000);
					}											// U8100-�������� ��ִ�å-end
					fnAPP_CancelDepRejectProc(T_CANCEL);		// ����Աݹ�ȯ�Լ�(�������ó��)
				}
				else fnAPP_CancelDepRejectProc(T_TIMEOVER);		// ����Աݹ�ȯ�Լ�(�������ó��)
			}
		}
	}

	m_pDevCmn->fnUCM_CloseShutter();							// �ݱ�:�ִ��ԱݿϷ��
	fnAPD_CheckDeviceAction(DEV_UCM);

	if (m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT) & (UCM_DEP_IN_CSM | UCM_DEP_ON_PATH))
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit:m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)[%x], ", m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT));
		CheckRejectAFlg = TRUE;									// �Աݼ�ǥ������ȯ��
		Delay_Msg(500);											// 2003.12.03
		fnAPD_CheckDepositCurOutCheck(854);						// Ȯ�ΰ���ǥ�����Է�
		m_pDevCmn->fnUCM_CloseShutter();
		fnAPD_CheckDeviceAction(DEV_UCM);
	}

//	if (!m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT))			// �Աݼ�ǥ��
//	{
//		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
//			fnAPP_CancelDepRejectProc(T_MSG, "�Աݵ� ��ǥ�� �����ϴ�");// ����Աݹ�ȯ�Լ�(�������ó��)
//		else
//			fnAPP_CancelDepRejectProc(T_CANCEL);
//	}

	if ((!CheckInfoCnt) ||										// �Աݼ�ǥ������
		(!CheckInfoAmt))										// �Աݼ�ǥ�ݾ׹�
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)							
			fnAPP_CancelDepRejectProc(T_MSG, "�Աݵ� ��ǥ�ݾ��� �����ϴ�");
		else													// ����Աݹ�ȯ�Լ�(�������ó��)
			fnAPP_CancelDepRejectProc(T_CANCEL);
	}
		
	int		CheckInfoCntTemp = 0;
	int		CheckInfoAmtTemp = 0;
	int		CheckInfoMotherAmtTemp1 = 0;
	int		CheckInfoMotherAmtTemp2 = 0;
	int		CheckInfoOtherAmtTemp = 0;
	for (int i = 0; i < UCM_MAXCHECKINCNT; i++)
	{
		if (Asc2Int(CheckInfoData[i].Amount, 10))
		{
			CheckInfoCntTemp++;
			CheckInfoAmtTemp += Asc2Int(CheckInfoData[i].Amount, 10);

			if (memcmp(CheckInfoData[i].Bank, BANKJJ2, 2) == 0)
			{
				if (memcmp(CheckInfoData[i].Branch, &m_pProfile->NETWORK.BranchNum[4], 4) == 0)
					CheckInfoMotherAmtTemp1 += Asc2Int(CheckInfoData[i].Amount, 10);
				else
					CheckInfoMotherAmtTemp2 += Asc2Int(CheckInfoData[i].Amount, 10);
			}
			else
				CheckInfoOtherAmtTemp += Asc2Int(CheckInfoData[i].Amount, 10);
		}
	}

	if ((CheckInfoCntTemp != CheckInfoCnt)	||
		(CheckInfoAmtTemp != CheckInfoAmt)	||
		(CheckInfoAmtTemp != (CheckInfoMotherAmtTemp1 + CheckInfoMotherAmtTemp2 + CheckInfoOtherAmtTemp)))
		fnAPP_CancelProc(T_PROGRAM);

	Int2Asc(CheckInfoAmt, Accept.CheckMoney, 15, 10, '0');		// �Աݼ�ǥ�ݾ�Set
	Int2Asc(CheckInfoMotherAmtTemp1, Accept.MotherCheckMoneyM, 15, 10, '0');	
																// �Աݼ�ǥ���������ݾ�Set
	Int2Asc(CheckInfoMotherAmtTemp2, Accept.MotherCheckMoneyO, 15, 10, '0');	
																// �Աݼ�ǥ����Ÿ���ݾ�Set
	Int2Asc(CheckInfoOtherAmtTemp, Accept.OtherCheckMoney, 15, 10, '0');
																// �Աݼ�ǥŸ��ݾ�Set

	fnAPP_PrintCheckDeposit(DEV_JPR);							// ��ǥ�Ա���������

	if (m_pDevCmn->EarPhoneInserted == TRUE)				
		m_pDevCmn->fnPIN_EntryDisable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit():return");
	return T_OK;
}

// �Աݼ�ǥ���
int CTranCmn::fnAPD_CheckDepositCount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositCount()");

	CString		strCheckData("");
	char		CheckData[38];
	CheckInfo	CheckInfoTemp;
	int			i = 0;
	int			nIndex = 0;

/////////////////////////////////////////////////////////////////////////////
	CheckRejectAFlg = FALSE;									// �Աݼ�ǥ������ȯ����
	CheckRejectKind = 0;										// �Աݼ�ǥ��ȯ����
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)// �Աݼ�ǥ�Ұ���
		return T_OK;

	m_pDevCmn->fnSCR_DisplayScreen(853);						// ��ǥ���(�Ա�)
	fnAPD_CheckDeviceAction(DEV_UCM);
	m_pDevCmn->fnUCM_Read();									// �б�
	fnAPD_CheckDeviceAction(DEV_UCM);
	if (m_pDevCmn->DeviceTimeout & DEV_UCM)
		CheckInfoStats = READ_TIME_OUT;
	else
		CheckInfoStats = NORMAL;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit:m_pDevCmn->DeviceTimeout[%x], CheckInfoStats[%d]", m_pDevCmn->DeviceTimeout, CheckInfoStats);

	switch (CheckInfoStats)										// ��ǥ����ó��
	{
		case NORMAL:											// ��ǥ�������Ϸ�
			if (CheckInfoCnt >= fnAPP_CalcCheckMaxIn())			// �Աݼ�ǥ�ѵ��ż��ʰ�
			{
				CheckInfoStats = READ_ERROR;
				break;
			}

			memset(&CheckData, 0, sizeof(CheckData));
			memset(&CheckInfoTemp, 0, sizeof(CheckInfoTemp));
			strCheckData = m_pDevCmn->fstrUCM_GetACheckData();
			MakePack(strCheckData.GetBuffer(0), &CheckData, __min(sizeof(CheckData) * 2, strCheckData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositCount:CheckData", CheckData, sizeof(CheckData));
			if (strCheckData.GetLength() != (sizeof(CheckData) * 2))
			{													// �Աݼ�ǥ�����̻�
				CheckInfoStats = READ_ERROR;
				break;
			}

			// 01234567890123456789012345678901234567			
			// 12345678<24=1759:000484<15;0000500000;			// ��ǥ�� �и���ȣ�� üũ�Ѵ�.
			if ((CheckData[8]  != '<') || (CheckData[11] != '=') ||
				(CheckData[16] != ':') || (CheckData[23] != '<') ||
				(CheckData[26] != ';') || (CheckData[37] != ';'))
			{
				CheckInfoStats = READ_ERROR;
				break;
			}

			for ( nIndex = 27 ; nIndex < 37 ; nIndex++ )		// ��ǥ�ε����� ����ó��	// V02.06.01
			{
				  if (CheckData[nIndex] == '_')
					  CheckData[nIndex] = '0';
			}

			if ((!IsNum((void*)&CheckData[0], 8))	||			// ��ǥ�� �ڷḦ üũ�Ѵ�. (��ȿ���� : 0x30 ~ 0x39)
				(!IsNum((void*)&CheckData[9], 2))	||
				(!IsNum((void*)&CheckData[12], 4))	||
				(!IsNum((void*)&CheckData[17], 6))	||
				(!IsNum((void*)&CheckData[24], 2))	||
				(!IsNum((void*)&CheckData[27], 10)))
			{
				CheckInfoStats = READ_ERROR;
				break;
			}

			if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE);	// ���Ǹ��
			else												// ���� ��ǥ������ üũ�Ѵ�.
			{
				// �Ʒ��� ��ǥ�� �˰��� üũ�� ���� �ʴ´�.
				if (!memcmp(&CheckData[9], "71", 2))
					;			// ü�źμ�ǥ
				else
				{
					int	CheckDigit = ((CheckData[9]  & 0x0f) * 3) +
									 ((CheckData[10] & 0x0f) * 7) +
									 ((CheckData[12] & 0x0f) * 1) +
									 ((CheckData[13] & 0x0f) * 3) +
									 ((CheckData[14] & 0x0f) * 7);
					CheckDigit = CheckDigit % 10;				// 0,1,2...9
					CheckDigit = 10 - CheckDigit;				// 10,9,8...1
					CheckDigit = CheckDigit % 10;				// 0,9,8...1
					CheckDigit = CheckDigit | '0';				// '0','9','8'...'1'
					
					if (CheckData[15] != CheckDigit)
					{
						CheckInfoStats = READ_ERROR;
						break;
					}
				}
			}

			memcpy(CheckInfoTemp.ReadData, &CheckData[0], 8);	// �ڷẹ��
			memcpy(CheckInfoTemp.Bank, &CheckData[9], 2);
			memcpy(CheckInfoTemp.Branch, &CheckData[12], 4);
			memcpy(CheckInfoTemp.AccountNum, &CheckData[17], 6);
			memcpy(CheckInfoTemp.Kind, &CheckData[24], 2);
			memcpy(CheckInfoTemp.Amount, &CheckData[27], 10);

																// ��ǥ�ڷ�˻�
			if ((!IsNum(CheckInfoTemp.ReadData, 8)) ||			// ��ǥ��ȣüũ
				IsZero(CheckInfoTemp.ReadData, 8)	||
				(!IsNum(CheckInfoTemp.Bank, 2))		||			// ����üũ
				IsZero(CheckInfoTemp.Bank, 2)		||
				(!IsNum(CheckInfoTemp.Branch, 4))	||			// ����üũ
				IsZero(CheckInfoTemp.Branch, 4)		||
// 				(!IsNum(CheckInfoTemp.AccountNum, 6))||			// ���¹�ȣüũ
// 				IsZero(CheckInfoTemp.AccountNum, 6)	||
				(!IsNum(CheckInfoTemp.Kind, 2))		||			// ����üũ
				IsZero(CheckInfoTemp.Kind, 2)		||
				(!IsNum(CheckInfoTemp.Amount, 10))	||			// �ݾ�üũ
				IsZero(CheckInfoTemp.Amount, 10))
			{
				CheckInfoStats = READ_ERROR;
			}
			else
			if ((Asc2Int(CheckInfoTemp.Kind, 2) == 13) ||		// ��ǥ�����˻�
				(Asc2Int(CheckInfoTemp.Kind, 2) == 14) ||
				(Asc2Int(CheckInfoTemp.Kind, 2) == 15) ||
				(Asc2Int(CheckInfoTemp.Kind, 2) == 16))
			{
				if (Asc2Int(CheckInfoTemp.Kind, 2) == 13)		// 10��������
				{
					if (Asc2Int(CheckInfoTemp.Amount, 10) != 100000)
						CheckInfoStats = READ_ERROR;
				}
				if (Asc2Int(CheckInfoTemp.Kind, 2) == 14)		// 30��������
				{
					if (Asc2Int(CheckInfoTemp.Amount, 10) != 300000)
						CheckInfoStats = READ_ERROR;
				}
				if (Asc2Int(CheckInfoTemp.Kind, 2) == 15)		// 50��������
				{
					if (Asc2Int(CheckInfoTemp.Amount, 10) != 500000)
						CheckInfoStats = READ_ERROR;
				}
				if (Asc2Int(CheckInfoTemp.Kind, 2) == 16)		// 100��������
				{
					if (Asc2Int(CheckInfoTemp.Amount, 10) != 1000000)
						CheckInfoStats = READ_ERROR;
				}
			}
			else
				CheckInfoStats = READ_ERROR;

			if (CheckInfoStats == READ_ERROR)					// ��ǥ�����ǵ�
			{
				CheckRejectKind = 1;							// ��ǥ�����ǵ�
				break;
			}
	
			if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE);	// ���Ǹ�� : 2003.12.20
			else												// ���� ��ǥ������ üũ�Ѵ�.
			{
				for (i = 0 ; i < CheckInfoCnt ; i++)			// �Աݼ�ǥ���� ��
				{
					if (!memcmp(&CheckInfoData[i], &CheckInfoTemp, sizeof(CheckInfo)))
					{
						CheckInfoStats = READ_ERROR;
						CheckRejectKind = 3;					// �ҷ���ǥ(���Աݼ�ǥ�͵�������)
						break;									// ���ϼ�ǥ�ȳ� �ʿ�� CheckRejectKind������ ó����
					}
				}
			}

			if (CheckInfoStats == READ_ERROR)					// �ҷ���ǥȮ��
			{
				if (!CheckRejectKind)
					CheckRejectKind = 1;				
				break;
			}

			m_pDevCmn->fnUCM_Escrow((LPCTSTR)&CheckPrintData);	// ��ǥ�輭�׼�ǥStack
			fnAPD_CheckDeviceAction(DEV_UCM);
			
			memcpy(&CheckInfoData[CheckInfoCnt], &CheckInfoTemp, sizeof(CheckInfo));
			CheckInfoAmt += Asc2Int(CheckInfoData[CheckInfoCnt].Amount, 10);
			CheckInfoCnt++;
			break;

		case READ_ERROR:										// ��ǥ�����ǵ�
			CheckRejectKind = 1;								// ��ǥ�����ǵ�
			break;

		case READ_TIME_OUT:										// ��ǥ����TimeOver
			break;

		case DOWN:												// ��ֹ߻�
			fnAPD_CheckDeviceAction(DEV_UCM);
		default:
			break;
	}

	if (CheckInfoStats == READ_ERROR)							// ��ǥ�����ǵ�
	{
		CheckRejectAFlg = TRUE;									// �Աݼ�ǥ������ȯ��

		if (CheckRejectKind == 1)								// ��ǥ�����ǵ�
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// �̵��Read����
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// ��ֹ߻���(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// ��ֹ߻��ð�(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// ��ü����(0-ī��,1-����,2-��ǥ,3-IC)
			strTempDbData += "||3";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit:"		\
													"CheckInfoAmt[%d], "		\
													"CheckInfoCnt[%d], "		\
													"CheckRejectKind[%d], "		\
													"CheckRejectAFlg[%d]",
													CheckInfoAmt, 
													CheckInfoCnt, 
													CheckRejectKind, 
													CheckRejectAFlg);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositCount():return");
	return T_OK;
}

// �Աݼ�ǥ������ȯ������
int CTranCmn::fnAPD_CheckDepositCurOutCheck(int ScrNum, int CheckTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositCurOutCheck(ScrNum[%d], CheckTime[%d])", ScrNum, CheckTime);

	int		TimeOverFlg = FALSE;								// ����Ÿ�ӿ���Flg

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)// �Աݼ�ǥ�Ұ���
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_UCM);

	if (CheckRejectAFlg)										// �Աݼ�ǥ������ȯ��
	{
		CheckRejectAFlg = FALSE;								// �Աݼ�ǥ������ȯ����

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum);					// �����ǥ��ȯ
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
		m_pDevCmn->DeviceTimeout &= ~DEV_UCM;					// �ð��ʰ������ʱ�ȭ
		m_pDevCmn->fnUCM_RejectA(CheckTime);					// �����ǥ��ȯ(������ð�)
		fnAPD_CheckDeviceAction(DEV_UCM);
		m_pDevCmn->fnUCM_WaitTaken();							// ������
		fnAPD_CheckDeviceAction(DEV_UCM);
		if (m_pDevCmn->DeviceTimeout & DEV_UCM)					// �̼���
			TimeOverFlg = TRUE;
		else													// ��������
			TimeOverFlg = FALSE;
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);

		if (TimeOverFlg)
		{
			m_pDevCmn->fnAPL_StackError("0110493", "�Աݼ�ǥ�̼���", DEV_UCM);
			m_pDevCmn->DeviceStatus |= DEV_UCM;
			m_pDevCmn->DeviceLoss	|= DEV_UCM;
			fnAPP_CancelProc(T_TIMEOVER);						// �̼���ó��
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositCurOutCheck(...):return");
	return T_OK;
}

// �Աݼ�ǥ�輭��������
// ��������� ����ȣ�� �ŷ��Ϸù�ȣ��ſ�
// �ŷ����ڸ� �����Ѵ�.
int CTranCmn::fnAPP_CheckDepositPrintEdit()
{
	memset(CheckPrintData, 0x00, sizeof(CheckPrintData));		// ��ǥ�輭��������

	memcpy(&CheckPrintData[0], &m_pProfile->TRANS.CheckYYYYMMDD, 8);
	CheckPrintData[8] = '-';									// ��¥
	if ((MenuAvail & MENU_MCU3)	||								// ���¹�ȣ
		(MenuIrdaMode)			)
	{
		//memcpy(&CheckPrintData[9], &pCardData3->AccountNo[0], 11);
		memcpy(&CheckPrintData[9], &Accept.AccountNum[0], 11);
	}
	else
	if (MenuAvail & MENU_PB)
	{
		//memcpy(&CheckPrintData[9], &pPbMsData->AccountNo[0], 11);
		memcpy(&CheckPrintData[9], &Accept.AccountNum[0], 11);
	}
	CheckPrintData[20] = '-';									// ���

	memcpy(&CheckPrintData[21], &m_pProfile->NETWORK.AtmSerialNum[5], 3);
	CheckPrintData[24] = '-';									// ���
	AddString(m_pProfile->TRANS.SerialNo, "000001", 6, &CheckPrintData[25]);
																// �ŷ��Ϸù�ȣ

	return T_OK;
}	

// �Աݼ�ǥ����
int CTranCmn::fnAPD_CheckDepositStore(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositStore(WaitFlg[%d])", WaitFlg);

	int		CrntInCheckCnt = 0;									// �Աݼ�ǥ�ż�

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)// �Աݼ�ǥ�Ұ���
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_UCM);

	m_pDevCmn->fnUCM_Stack();									// ��ǥ����

	if (CheckInfoCnt)											// �Աݼ�ǥ������
	{
		CrntInCheckCnt = m_pDevCmn->fnAPL_GetNumberOfInCheck();	// �Աݼ�ǥ�ż����ϱ�
		CrntInCheckCnt += CheckInfoCnt;							// ��ǥ�ż�+

		m_pDevCmn->fnAPL_SetNumberOfInCheck(CrntInCheckCnt);	// �Աݼ�ǥ�ż�����
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositStore:CrntInCheckCnt[%d]", CrntInCheckCnt);
	}
	
	if (WaitFlg)	
		fnAPD_CheckDeviceAction(DEV_UCM);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositStore():return");
	return T_OK;
}

// �Աݼ�ǥ��ȯ
int CTranCmn::fnAPD_CheckDepositReject()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositReject()");

/////////////////////////////////////////////////////////////////////////////
	CheckRejectFlg = FALSE;										// �Աݼ�ǥ��ȯ����
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)// �Աݼ�ǥ�Ұ���
		return T_OK;	

	fnAPD_CheckDeviceAction(DEV_UCM);

	if ((CheckInCountFlg)	||									// �Աݼ�ǥ�������
		(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)))		// ��ǥ����
		CheckRejectFlg = TRUE;									// �Աݼ�ǥ��ȯ��
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositReject:CheckRejectFlg[%d], CheckInCountFlg[%d]", CheckRejectFlg, CheckInCountFlg);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositReject():return");
	return T_OK;
}

// �Աݼ�ǥ��������
int	CTranCmn::fnAPD_CheckDepositPrint(int PrintDevice, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositPrint(PrintDevice[%x], WaitFlg[%d])", PrintDevice, WaitFlg);

	int		i = 0, js = 0, jj = 0;

	if ((TranProc == TRAN_DEP)					&&				// �Ա�
		(Asc2Int(Accept.CheckMoney, 8) != 0)	&&				// �Աݼ�ǥ
		(!CheckInPrintFlg));									// �Աݼ�ǥ��������üũ
	else return T_OK;

	CheckInPrintFlg = TRUE;										// �Աݼ�ǥ������������
	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���

//                       01234567890123456789012345678901
	sprintf(ShuData[0], "��ǥ��ȣ ���� ���� ���¹�ȣ ����");

	for (i = 0; i < UCM_MAXCHECKINCNT; i++)
	{
		if ((Asc2Int(CheckInfoData[i].Bank, 2) == 0) ||
			(Asc2Int(CheckInfoData[i].Branch, 4) == 0) ||
			(Asc2Int(CheckInfoData[i].Kind, 2) == 0) ||
			(Asc2Int(CheckInfoData[i].AccountNum, 6) == 0) ||
			(Asc2Int(CheckInfoData[i].ReadData, 8) == 0))
			break;

		Sprintf(ShuData[i + 1], 24, "%8.8s %4.2s %4.4s %6.6s %4.2s ",
			CheckInfoData[i].ReadData,
			CheckInfoData[i].Bank,
			CheckInfoData[i].Branch,
			CheckInfoData[i].AccountNum,
			CheckInfoData[i].Kind);
	}

	fnAPD_JnlPrint(WaitFlg);									// ��������(����ó��)

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositPrint(...):return");
	return T_OK;
}

// ���ݰ����������
int	CTranCmn::fnAPD_CashDepositPrint(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositPrint(WaitFlg[%d])", WaitFlg);

	int		i = 0, js = 0, jj = 0;
	char	StrBuff[256] = {0,};

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���

	GetTime(StrBuff);
//                       012345678901234567890123
	sprintf(ShuData[0], "%2.2s:%2.2s:%2.2s", &StrBuff[0], &StrBuff[2], &StrBuff[4]);
	sprintf(ShuData[1], "[���ݰ�����]");

	sprintf(ShuData[3], "�Ѱ���ݾ�:%d", CashData.mTotalAmount);
	sprintf(ShuData[4], "�������Ǹż�:%d", CashData.mFiftyThousand);
	sprintf(ShuData[5], "�����Ǹż�:%d", CashData.mTenThousand);
	sprintf(ShuData[6], "��õ���Ǹż�:%d", CashData.mFiveThousand);
	sprintf(ShuData[7], "õ���Ǹż�:%d", CashData.mOneThousand);

	if (CashData.mUnknown > 500)
	{
		CashData.mUnknown = 0;
	}
	sprintf(ShuData[8], "�Ҹ�Ǹż�:%d", CashData.mUnknown);

	fnAPD_JnlPrint(WaitFlg);									// ��������(����ó��)

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositPrint(...):return");
	return T_OK;
}

// ��ü��������ֽù�����õ�	V02-06-02-#05 21405��ǥ�����
int CTranCmn::fnAPD_MaterialOutRetryAfterError(int	nDevice)
{
/*
	if ((nDevice == DEV_SPR) &&
		(!m_pDevCmn->fnAPL_CheckError())	&&					// ��ְ���
		(memcmp(&m_pDevCmn->CurErrBuff.ErrorCode[0], "21405", 5) == 0))
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�� ��ǥ��ְ��� : [21405]��� ���� ���� ");
		if (SprOutRetryFlg == TRUE)
			fnAPP_CancelProc(T_ERROR);

		m_pDevCmn->fnAPL_ClearError();
		m_pDevCmn->DeviceStatus &= ~nDevice; 

		SprOutRetryFlg = TRUE;
		m_pDevCmn->fnSPR_Eject();								// ��ü����
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR);			// ���ۿϷ����&��ְ���
		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_SPR))
			fnAPP_CancelProc(T_ERROR);
		else SprOutRetryFlg = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�� ��ǥ��ְ��� : [21405]��� ���� ���� ");
	}
	else
*/
//	#N0157
	if ((nDevice == DEV_SPR) &&
		(!m_pDevCmn->fnAPL_CheckError()))					// ��ְ���
	{
		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(nDevice))
		{

/*  #N0157
			if(SprOutRetryFlg) return T_OK;

			m_pDevCmn->fnAPL_ClearError();
			m_pDevCmn->DeviceStatus &= ~nDevice; 

			SprOutRetryFlg = TRUE;
			m_pDevCmn->fnSPR_Eject();								// ��ü����
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR);			// ���ۿϷ����&��ְ���
*/
			if(SprOutRetryFlg) return T_OK;

			SprOutRetryFlg = TRUE;
			
			if(m_pProfile->TRANS.TransPowerOnOff == ON)
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
					m_pDevCmn->fnSCR_DisplayString(1, "[��ǥ ���]");
					m_pDevCmn->fnSCR_DisplayString(2, "�ش� �ŷ��� �Ϸ� �Ǿ����ϴ�");
					m_pDevCmn->fnSCR_DisplayString(3, "��ְ� �߻��Ͽ� ��ǥ�� ����� �� �����ϴ�.");
				}
				else
				{
					m_pDevCmn->fnSCR_DisplayString(1, "[Print Disable]");
					m_pDevCmn->fnSCR_DisplayString(2, "your transaction has been completed");
					m_pDevCmn->fnSCR_DisplayString(3, "but receipt cannot be issued due to an error.");

				}
			}
			else
			{
					m_pDevCmn->fnSCR_DisplayString(1, "[��ǥ ���]");
					m_pDevCmn->fnSCR_DisplayString(2, "��ְ� �߻��Ͽ� ��ǥ�� ����� �� �����ϴ�");
					m_pDevCmn->fnSCR_DisplayString(3, "���� ���� �����̿��� ��ø� ��ٷ� �ֽʽÿ�.");
			}

			m_pDevCmn->fnSCR_DisplayScreen(701);						// �������
			Delay_Msg(3000);			
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeviceAction(fnAPL_GetAvailErrorDevice):return");
		}
	}

	else
		fnAPP_CancelProc(T_ERROR);

	return TRUE;	
}




////////////////////////////////////////////////////////////////////////////////
//	���� IC �Լ�
////////////////////////////////////////////////////////////////////////////////

// ���� ICī���б�&üũ
int CTranCmn::fnAPD_FICCardReadCheck()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICCardReadCheck()");

	CString strFICISO3Data("");
	CString strFICISO3DataUnPack("");

	if (!MenuFICMode)											// ����IC�ŷ�Ȯ��
		return T_ERROR;

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_ERROR;

	if (Accept.FICSelectAccountIndex >= itemof(m_pDevCmn->FinanceICISO3Info))
		fnAPP_CancelProc(T_PROGRAM);

HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICCardReadCheck:FICData", 
		m_pDevCmn->FinanceICISO3Info[Accept.FICSelectAccountIndex].ISO3, 
		sizeof(m_pDevCmn->FinanceICISO3Info[Accept.FICSelectAccountIndex].ISO3));

	MakeUnPack( m_pDevCmn->FinanceICISO3Info[Accept.FICSelectAccountIndex].ISO3 , 
				strFICISO3Data, 
				__min(54, sizeof(m_pDevCmn->FinanceICISO3Info[Accept.FICSelectAccountIndex].ISO3)),
				FALSE);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICCardReadCheck():strFICISO3Data[%s]", strFICISO3Data);

	// ī�嵥��Ÿ�м��� ���� �ٽ� UnPack
	MakeUnPack( strFICISO3Data.GetBuffer(strFICISO3Data.GetLength()), 
				strFICISO3DataUnPack, 
				strFICISO3Data.GetLength());

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICCardReadCheck():strFICISO3DataUnPack[%s]", strFICISO3DataUnPack);

	strFICISO3DataUnPack = ",," + strFICISO3DataUnPack.Mid(2, 104*2);		
																// 1/2Ʈ����������(UnPack����Ÿ�̹Ƿ� Mid������)
	
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICCardReadCheck():strFICISO3DataUnPack[%s]", strFICISO3DataUnPack);
	fnAPD_CardReadCheck(strFICISO3DataUnPack);					// ī��üũ
	
	return T_OK;
}

// ����ICī�� ī�屸��	2004.02.11
// ATR/FCIȮ�� - �������� ������� ���� ó���ؾ� ��
int CTranCmn::fnAPD_FICDistinctCardType(int nReadDF)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICDistinctCardType(%d)", nReadDF);


	if (MenuFICMode & MENU_INFO_1)  //Kim.Gi.Jin 0726
		return T_OK;


	if (!nReadDF)
		nReadDF = TRAN_IC_FINANCEIC;							// ����IC DF�� Default�� ����


	m_pDevCmn->FICCardType = FIC_CLOSE;							// �⺻:�����ī��� ����

	if (nReadDF == TRAN_IC_KCASH)
	{
		m_pDevCmn->fnKCS_ReadATR();								// ATR�б�
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())					// ����ICī���ΰ��(������-KCASH)
		{
			m_pDevCmn->fnMCU_ICChipInitialize();				// �����ĸ� ChipInitialize���� �ٽ�ó��
			fnAPD_CheckDeviceAction(DEV_MCU);					// Error�� EP���� ���� PowerOff���ɼ�����
																// EP PowerOff������ ���ڵ� �������

			m_pDevCmn->fnICA_SelectApplet(nReadDF);
			fnAPD_CheckDeviceAction(DEV_MCU);
			if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
				fnAPP_CancelProc(T_MSG, "�� ī��� ��� �� �� �����ϴ�");

			m_pDevCmn->FICCardType = FIC_OPEN;					// ������ī�弳��
		}
		else
		{
			m_pDevCmn->FICCardType = FIC_CLOSE;					// CLOSE ī�弳��
		}
																// �ù����ī���Դϴ�. �ŷ��Ұ�
	}
	else
	if (nReadDF == TRAN_IC_FINANCEIC)
	{
		m_pDevCmn->fnICA_SelectApplet(nReadDF);					// 
		fnAPD_CheckDeviceAction(DEV_MCU);
		if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
			return T_ERROR;
//			fnAPP_CancelProc(T_MSG, "�� ī��� ��� �� �� �����ϴ�");

		m_pDevCmn->FICCardType = FIC_OPEN;						// ������ī�弳��	
	}

	 MenuFICMode |= MENU_INFO_1;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICDistinctCardType():return CardType[%d]", m_pDevCmn->FICCardType);
	return T_OK;
}

/*==============================================================================
* Function Name : fnAPD_FICBasicInquiry()
* Description   : ���� IC ������ ��ȸ�ϴ� �Լ�(��� �ŷ��� ����)
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.01.27 Han Su Woong
* ============================================================================*/
int CTranCmn::fnAPD_FICBasicInquiry()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICBasicInquiry()");


	if (MenuFICMode & MENU_INFO_2)  //Kim.Gi.Jin 0726
		return T_OK;

	if (!MenuFICMode)											// ����IC�ŷ�Ȯ��
		return T_ERROR;

	/* ------------------------------------------------------------------------
	// fnAPD_FICDistinctCardType(.)���� SelectApplet�� �����
	m_pDevCmn->fnICA_SelectApplet(TRAN_IC_FINANCEIC);
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
		fnAPP_CancelProc(T_CANCEL, NULL, NULL, NULL, 599);
	------------------------------------------------------------------------ */

	m_pDevCmn->fnFIC_ReadDualInfo(0x00);						// �⺻ ���� �� ���� ���� ��ȸ
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_READDUALINFO) )
		m_pDevCmn->FICAccountCount = 0;
//		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");

	if (m_pDevCmn->FICAccountCount <= 0)
	{
		return T_ERROR;
	}

	MenuFICMode |= MENU_INFO_2;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICBasicInquiry():return");
	return T_OK;
}

/*==============================================================================
* Function Name : fnAPD_FICMyBiProc()
* Description   : MyBi ���� IC�� ��� �ŷ��ϴ� �Լ�
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.01.27 Han Su Woong
* ============================================================================*/
int CTranCmn::fnAPD_FICMyBiProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICMyBiProc()");


	if (!MenuFICMode)											// ����IC�ŷ�Ȯ��
		return T_ERROR;

	char Password[5];
	memset(Password, 0x00, sizeof(Password));
	memcpy(Password, Accept.PassWord, 4);

	// Get ENC Data 1
//	m_pDevCmn->fnFIC_GetENCData(0x01, Accept.PassWord);
	m_pDevCmn->fnFIC_GetENCData(0x01, Password);
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_GETENCDATA1) )
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");

	// GetChallenge
	m_pDevCmn->fnFIC_GetChallenge();
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_GETCHALLENGE) )
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");

	// Get ENC Data 2
	m_pDevCmn->fnFIC_GetENCData(0x02, Password);
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_GETENCDATA2) )
		fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICMyBiProc():return");
	return T_OK;
}

// ����ICī�� (����)��й�ȣ ��ȣȭ
// ī�峭�� + ��ȣȭ�� ��й�ȣ ��ȸ
int CTranCmn::fnAPD_FICGetEncipher()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICGetEncipher(..)");


	if ((!MenuFICMode)	&&										// ����IC�ŷ�Ȯ��
		(!MenuEPBMode)	)										// ��������ŷ�Ȯ��
		return T_ERROR;

	//m_pDevCmn->fnFIC_GetEncipher(Accept.PassWord, sizeof(Accept.PassWord));
	m_pDevCmn->fnFIC_GetEncipher(Accept.PassWord, 4);
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_GETENCIPHER) )
	{
		// ���⼭ �����ô� ���̺� IC.
		// MyBi IC ó��
		// History : EP����� ������ �ڵ� Power Off��...��.��
		////////////////////////////////////////////////////////////////////////
		m_pDevCmn->fnMCU_ICChipInitialize();
		fnAPD_CheckDeviceAction(DEV_MCU);
		
		m_pDevCmn->fnICA_SelectApplet(TRAN_IC_FINANCEIC);
		fnAPD_CheckDeviceAction(DEV_MCU);
		if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
			fnAPP_CancelProc(T_MSG, "ī�带 �д� �� ������ �߻��Ͽ����ϴ�.", "[��Ȳ��: 1577 - 4655] �����Ͽ� �ֽʽÿ�.");

		// ���⸦ �����ϰ� ���� �����Ѵٸ� �����ص� ��.
		////////////////////////////////////////////////////////////////////////

		fnAPD_FICMyBiProc();

		FICMyBiFlg = TRUE;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICGetEncipher():return");
	return T_OK;
}

// ����IC host�۽ŵ���Ÿ ��ȸ
// ��������(32) + ī�峭��(32) + ��ȣȭ��й�ȣ(32)
int CTranCmn::fnAPD_FICGetHostTranData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICGetHostTranData(..)");

	if (!MenuFICMode)											// ����IC�ŷ�Ȯ��
		return T_ERROR;

	m_pDevCmn->fnFIC_GetHostTranData(FICMyBiFlg);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICGetHostTranData():return");
	return T_OK;
}

//������������������������������������������������������������������������������
// EMV FUNCTION	DEFINITION										V03-00-25-#02
//������������������������������������������������������������������������������
/*==============================================================================
* Function Name : fnAPP_EMVInitProc()
* Description   : EMV ���� Buffer�� �ʱ�ȭ�ϴ� �Լ�(��� Start�� 1ȸ ȣ��)
* Parameters    : (NONE)
* Return        : Success : T_OK
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int	CTranCmn::fnAPP_EMVInitProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_EMVInitProc()");
	int nRet = m_pDevCmn->fnEMV_Initialize();					// Buffer Initialize
	return nRet;
}

/*==============================================================================
* Function Name : fnAPP_EMVOpenSetProc()
* Description   : ������(�Ǵ� ������ Configuratin Code Setting��) EMV ������ ����
* Parameters    : ConfigCode : ������ �����ϴ� ȯ�� ���� ���� Index.
*                              �����ÿ��� �μ� ���� ȣ��. �⺻�μ� = 1.
* Return        : Success : T_OK
*                 Fail    : T_ERROR - ���� EMV �ŷ��� �������� ����.
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CTranCmn::fnAPP_EMVOpenSetProc(int ConfigCode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_EMVOpenSetProc() ConfigCode = %d", ConfigCode);
	int		nAIDList;
	int		nRet(T_ERROR), nReturn(T_ERROR);

	if(m_pDevCmn->IfmDirect)									// V09-33-00-#01 ICCM ��� ����
		return TRUE;
	
#if EMV_TA_MODE													// ���� ���
	if ( m_pDevCmn->EMVInfo.ConfigCode == NULL )				// ���� ������
	{
		//**********************************************************************
		//  CA ����Ű List Set Process
		//**********************************************************************
//		m_pDevCmn->EMVInfo.CAPKRIDIndex = EMV_CAPKLISTRID_CNT;
//		m_pDevCmn->EMVInfo.CAPKKeyIndex = EMV_CAPKLISTKEY_CNT;
//		
//		EMV_DefaultCAPKListSet();								// Default ���� CA ����Ű List Set
//		for ( int i = 0; i < EMV_CAPKLISTRID_CNT * EMV_CAPKLISTKEY_CNT; i++ )
//		{
//			nRet = EMV_CAListSet(i);							// CA ����Ű List Set
//			if ( nRet != T_OK )
//				return nReturn;
//		}

		//**********************************************************************
		//  Terminal List(AID List) Set Process
		//**********************************************************************
		// Default ���� AID List Set
		nRet = m_pDevCmn->fnEMV_DefaultAIDListSetINI();
		if ( nRet != TRUE )
		nAIDList = m_pDevCmn->fnEMV_DefaultAIDListSet();
		
		if ( nAIDList != m_pDevCmn->EMVInfo.AIDListCount )		// �⺻ AID ���ý� Error
			return nReturn;

		//**********************************************************************
		//  Terminal Constant Set Process
		//**********************************************************************
		// Default Data Object List Set
		nRet = m_pDevCmn->fnEMV_DefaultDOLSetINI();
		if ( nRet != TRUE )
			m_pDevCmn->fnEMV_DefaultDOLSet();
		
		m_pDevCmn->EMVInfo.ConfigCode = ConfigCode;				// Config Code Set
	}
	else
	{
		// ���� ���ð��� ���簪�� ���Ѵ�. ���ٸ� �ƹ��ϵ� ���� �ʴ´�.
		if ( (int)m_pDevCmn->EMVInfo.ConfigCode != ConfigCode )
		{
			//******************************************************************
			//  Terminal Constant Set Process
			//******************************************************************
			// Default Data Object List Set
			nRet = m_pDevCmn->fnEMV_DefaultDOLSetINI();
			if ( nRet != TRUE )
				m_pDevCmn->fnEMV_DefaultDOLSet();
			
			m_pDevCmn->EMVInfo.ConfigCode = ConfigCode;			// Configuration Code Update	
		}
	}
#else	// #if EMV_TA_MODE
	if ( m_pDevCmn->EMVInfo.ConfigCode == NULL )				// ���� ������
	{
		// ��ÿ��� CA ����Ű List Set�� ���� �ʴ´�.
		// ���� ATM������ Offline Data Authtication(SDA, DDA)�� ����.
//**************************************************************************
// ���� ������ EMV.ini�� ������ �����Ѵ�. EP���� ������ SP���� �Ѵ�.
// (�������� ���)
//**************************************************************************
		//**********************************************************************
		//  Terminal List(AID List) Set Process
		//**********************************************************************
		// Default ���� AID List Set
		nRet = m_pDevCmn->fnEMV_DefaultAIDListSetINI();
		if ( nRet != TRUE ) {
			nAIDList = m_pDevCmn->fnEMV_DefaultAIDListSet();
			if ( nAIDList != m_pDevCmn->EMVInfo.AIDListCount )	// �⺻ AID ���ý� Error
				return nReturn;
		}

		//**********************************************************************
		//  Terminal Constant Set Process
		//**********************************************************************
		// Default Data Object List Set
		nRet = m_pDevCmn->fnEMV_DefaultDOLSetINI();
		if ( nRet != TRUE )
			m_pDevCmn->fnEMV_DefaultDOLSet();
		
		m_pDevCmn->EMVInfo.ConfigCode = ConfigCode;				// Config Code Set
	}
#endif	// #if EMV_TA_MODE

	return T_OK;
}

/*==============================================================================
* Function Name : fnAPP_EMVReadProc()
* Description   : EMV Application Read �Լ�
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.11.03
* ============================================================================*/
int	CTranCmn::fnAPP_EMVReadProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_EMVReadProc()");

//	if (!m_pDevCmn->EarPhoneInserted)	// �����ATM ����ȭ�� ó���� ���� �ݺ��ϴ� ��� ó�� Ȯ���ϱ�!!
//	m_pDevCmn->fnSCR_DisplayScreen(847);

	EmvICStatus = 0;						

	if (fnAPD_EMVSelectProc() != T_OK)
	{
		m_pDevCmn->fnKCS_PowerOff();			// #N0268
		fnAPD_CheckDeviceAction(DEV_MCU);
		m_pDevCmn->fnKCS_PowerOn();
		fnAPD_CheckDeviceAction(DEV_MCU);

		if (fnAPD_EMVSelectProc() != T_OK)
		{
			EMVReadFlg = FALSE;
			return T_ERROR;											// FallBack����
		}										// FallBack����
	}

	if (fnAPD_EMVBrandChoice() != T_OK)
	{
		EMVReadFlg = FALSE;
		return T_ERROR;											// FallBack����	
	}

	EMVReadFlg = TRUE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_EMVReadProc():return EMVReadFlg=[%d]", EMVReadFlg);
	return T_OK;

}
/*==============================================================================
* Function Name : fnAPPLib_EMVReadProc()
* Description   : 
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: V09-33-00-#01		ICCM �������
* ============================================================================*/
int	CTranCmn::fnAPPLib_EMVReadProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPPLib_EMVReadProc()");
	
	m_pDevCmn->fnEMVLib_Sel_SetOnlyOneAID("N");	
	
	long nRtn =m_pDevCmn->fnEMVLib_SystemInitial();
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SystemInitial() = [%ld]", nRtn);
	if(nRtn!= RSLT_SUCCESS)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV_SystemInitial() FAILED.... Will Cancel");
		return T_ERROR;
	}
	
	nRtn = m_pDevCmn->fnEMVLib_Sel_BuildCandidateApp();
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Sel_BuildCandidateApp() = [%ld]", nRtn);
	if( nRtn == EMV_FALLBACK )
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Sel_BuildCandidateApp() return EMV_FALLBACK");
		CardTran &= ~MENU_IC;
//		return fnAPL_EmvFallBackProc();
		return fnAPLLib_EmvFallBackProc(TRUE);
	}
	else if(nRtn!= RSLT_SUCCESS)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Sel_BuildCandidateApp() FAILED.... Will Cancel");
//		fnAPP_CancelProc(T_MSG);
		return T_ERROR;
	}
	
	int nSelected = fnAPDLib_EMVSelectAID();
	m_SelectIndex = nSelected;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVSelectAID() = [%d]", nSelected);
	if(nSelected==0)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Selection Failed => Ask Fall back transaction");
		CardTran &= ~MENU_IC;
//		return fnAPL_EmvFallBackProc();
		return T_ERROR;
	}
	else if(nSelected==-1)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVSelectAID() FAILED.... Will Cancel");
//		fnAPP_CancelProc(T_NOMSG);
		return T_ERROR;
	}
	
	BYTE* sOut;
	EMVPrefLanguage = m_pDevCmn->fnEMVLib_GetVal(EMVTid_LangPrefer);
	if( EMVPrefLanguage.GetLength()>0)
	{
		sOut = new BYTE[EMVPrefLanguage.GetLength() +1];
		memset(sOut, NULL, EMVPrefLanguage.GetLength()+1);
		MakePack( (void*)(LPCTSTR)EMVPrefLanguage, (void*)sOut, EMVPrefLanguage.GetLength(), '^', 0);
		EMVPrefLanguage.Format("%s", sOut);
		delete [] sOut;
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_GetVal(EMVTid_LangPrefer) = [%s]", EMVPrefLanguage);
	
	int nResult = 0;
	EmvReadAppDataFlag = TRUE;
	nResult = m_pDevCmn->fnEMVLib_ReadAppData();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():�̵���m_pDevCmn->fnEMV_ReadAppData():nResult(%d)", nResult);
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
	
	fnAPLLib_EmvMakeCardPanProc();

    // MS Track 2 Image ����
    char szISO2[100] = {0x00,};
    m_pDevCmn->fnEMV_GetISO2(szISO2);

    CString strISO2("");
    strISO2  = ",";                                              // 1Track
    strISO2 += MakeUnPack(szISO2, strlen(szISO2));               // 2Track
    strISO2 += ",";                                              // 3Track

    fnAPD_CardReadCheck(strISO2);
//    fnAPD_CardReadCheck(TRUE);								



	if (fnAPD_EMVJaTaCheck() == FALSE)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "����ī��� MS�� �ŷ�!!");
//		if ((Accept.DCC.BrandType[0] == 'M') &&			//V02-02-00-#18
//			(Accept.DCC.ICType == 'Y'))
//			return T_ERROR;
//		else
//			return T_OK;								//V02-02-00-#18
	}

	EMVReadFlg = TRUE;
	ICnIcApp = ICEMV;
	m_pDevCmn->EmvCardService = 0;

	return T_OK;	
}

/*==============================================================================
* Function Name : fnAPP_EMVAfterOnlineTranProc()
* Description   : EMV �¶��� �� �ŷ� ó�� �Լ�
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.11.09
* ============================================================================*/
int	CTranCmn::fnAPP_EMVAfterOnlineTranProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_EMVAfterOnlineTranProc()");

//	if (TranProc == TRAN_WITH)
//		memset(m_pDevCmn->m_EMVszData, ' ', sizeof(m_pDevCmn->m_EMVszData));
	
	int nRet;
	nRet = fnAPD_EMVAfterRecvProc();

	if (nRet != T_OK)										
	{
		if (m_pDevCmn->TranResult)
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_EMVAfterOnlineTranProc() ī�� ���� ����!");
			
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("ARPC ���� : �ŷ��Ϸù�ȣ(%12.12s)", m_RD.byDataField_026x));
			if (TranProc != TRAN_INQ)
			{
				EMVAuthError = TRUE;
				TranCode2 = TRANID_A020;		
				fnAPP_SendHost();
				fnAPP_RecvHost();
				fnAPP_CancelProc(T_MSG, "ī�� ���� ����!",  "�ŷ��� ����մϴ�");
			}
			else
			{
				EMVAuthError = TRUE;			
				TranCode2 = TRANID_A020;
				fnAPP_SendHost();
				fnAPP_RecvHost();
			}
		}
	}
//	fnAPP_CancelProc(T_ERROR);


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_EMVAfterOnlineTranProc():return");
	return T_OK;

}

/*==============================================================================
* Function Name : EMV_SerialTrans()
* Description   : ��ȸ �� �ŷ� ���� ���� �ŷ��� ������ ���� ���� ���ۼ� 
* Parameters    : (None)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.10.28 kh...
* ============================================================================*/
int CTranCmn::fnAPD_EMVContinueTrans()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVContinueTrans()");
	int nRet = T_OK;
	int nIndex = 0;
	int nSelected = 0;							// V09-33-00-#01		ICCM �������

	if (BrandSelectIndex < 1)
		return T_ERROR;
	else
		nIndex = BrandSelectIndex;

	// ���� �����
	m_pDevCmn->fnKCS_PowerOff();
	fnAPD_CheckDeviceAction(DEV_MCU);
	m_pDevCmn->fnKCS_PowerOn();
	fnAPD_CheckDeviceAction(DEV_MCU);


	if (fnAPD_EMVSelectProc() != T_OK)
	{
		fnAPP_CancelProc(T_MSG, " �� ī��δ� ���ŷ��� �� �� �����ϴ�",
								" â���� �����Ͽ� �ֽñ� �ٶ��ϴ�. "); 
		return T_ERROR;
	}

	// ���õ� EMV AID�� ����
	m_pDevCmn->fnEMV_SelectApplication(nIndex);
	fnAPD_CheckDeviceAction(DEV_MCU);
	nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_SELECTAPPLICATION);

	if (nRet != T_OK)	// Index�� 1���� �������� ����
	{
		fnAPP_CancelProc(T_MSG, " �� ī��δ� ���ŷ��� �� �� �����ϴ�",
								" â���� �����Ͽ� �ֽñ� �ٶ��ϴ�. "); 
		return T_ERROR;
	}

	//**************************************************************************
	//  Read Application : �ŷ��� ���� Application�� ������ ����
	//**************************************************************************
	nRet = m_pDevCmn->fnEMV_ReadApplication();
	fnAPD_CheckDeviceAction(DEV_MCU);
	nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_READAPPLICATION);

	if (nRet != T_OK)
	{
		if (nRet == EMV_RESULT_TERMINATE)
		{
			fnAPD_EMVResultProc();
		}

		fnAPP_CancelProc(T_MSG, " �� ī��δ� ���ŷ��� �� �� �����ϴ�",
						  " â���� �����Ͽ� �ֽñ� �ٶ��ϴ�. "); 
		return T_ERROR;
	}
	
	return T_OK;
}

/*==============================================================================
* Function Name : fnAPD_EMVSelectProc()
* Description   : EMV Application�� Ȯ���ϴ� �Լ�
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int	CTranCmn::fnAPD_EMVSelectProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVSelectProc()");

	int		nReturn(T_ERROR);
	int		nRet = 0;

	/*
	m_pDevCmn->fnEMV_SelectPSE();								// EMV Application Ȯ��
	fnAPD_CheckDeviceAction(DEV_MCU);
	nReturn = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_SELECTPSE);

	if ((nReturn == T_OK)				||
		(nReturn == EMV_RESULT_TERMINATE))
	{
		;
	}
	else
	{
		if (nReturn == EMV_FLAG_INITDATAERROR)
		{
			memcpy(m_pDevCmn->m_szEMVFBCode, EMV_FBCODE_NOATR, _tcslen(EMV_FBCODE_NOATR));
			return EMV_FLAG_INITDATAERROR;
		}

		memcpy(m_pDevCmn->m_szEMVFBCode, EMV_FBCODE_SELECTERROR, _tcslen(EMV_FBCODE_SELECTERROR));
		return EMV_FLAG_INITDATAERROR;
	}
	*/

	m_pDevCmn->fnEMV_InitProcess();								// Buffer Clear
	
	m_pDevCmn->fnEMV_ReqCandidateList();						// EMV �ĺ� ��� ��ȸ
	fnAPD_CheckDeviceAction(DEV_MCU);
	nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_REQCANDIDATELIST);

	if (nRet != T_OK)
	{
		if ( nRet == EMV_RESULT_INITDATAERROR )					// #N0268 FB�� ���� EMV_FLAG_INITDATAERROR -> EMV_RESULT_INITDATAERROR
		{
			// 2015.01.19 LDC �� �κ��� �ʱ⼳�� ������ �� ���� �ŷ��� Fallbackó�� �Ѵ�.

			// V09-47-00-#07 FB�� ���� Start
			if (fnAPD_EMVTerminalConstant() != T_OK)					// ���� �ŷ��� FB ����
			{
				EMVReadFlg = FALSE;
				return nRet;											// FallBack����
			}
			// V09-47-00-#07 FB�� ���� End

		}
		else
		if (nRet == EMV_RESULT_NOTSUPPORT)
		{
			;
		}

		return nRet;
	}

	return T_OK;
}

// V09-47-00-#07 FB�� ���� Start
/*==============================================================================
* Function Name : fnAPD_EMVTerminalConstant()
* Description   : EMV TerminalConstant ����
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2015.01.16 LDC
* ============================================================================*/
int	CTranCmn::fnAPD_EMVTerminalConstant()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVTerminalConstant()");
	
	int		nReturn(T_ERROR);
	int		nRet = 0;
	BYTE	btData[1024] = {0,};
	BYTE	btLen = 0x00;

	btLen = m_pDevCmn->m_nEMVTerminalConstantLen;
	memcpy(btData, m_pDevCmn->m_btEMVTerminalConstant, btLen);

	m_pDevCmn->fnEMV_TerminalConstSet(btData,btLen);
	fnAPD_CheckDeviceAction(DEV_MCU);
	nReturn = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_TERMINALCONSTSET);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVTerminalConstant(...) nReturn : %d", nReturn);

	return nReturn;
}

/*==============================================================================
* Function Name : fnAPD_EMVBrandChoice()
* Description   : EMV Application �� �ŷ��� Brand�� �����ϴ� �Լ�.
*                 ���� �ŷ� ������ Brand�� �ϳ��̰� ȭ�鿡 ǥ���� �ǹ��� ���ٸ�
*                 (AIP�� 0x80�� �ƴ϶��) ǥ�� ���� �������� �����Ѵ�.
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CTranCmn::fnAPD_EMVBrandChoice()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVBrandChoice()");
	int		nBrandCnt(0);
	int		nReturn(T_ERROR), nRet;
	CString strReturn("");
	int		i = 0;
	int		nIndex;

	//**************************************************************************
	//  �ĺ� ��� �� �ŷ��� Application ����
	//**************************************************************************
	nBrandCnt = m_pDevCmn->fnEMV_GetBrandCount();				// Brand ���� ������
	// ���� nBrandCnt�� 1���̰�, �� ���ø����̼��� API�� 0x80�� �ƴϸ� �ڵ� �����Ѵ�.
	/*
	if ((nBrandCnt == 1)							&&
		(m_pDevCmn->EMVCandidateList[0].API != 0x80))
	{
		nIndex = 1;
		BrandSelectIndex = 1;									// ������ 1 Brand
		
		//**********************************************************************
		//  Select Application : ���õ� Application���� �ŷ� ����
		//**********************************************************************
		m_pDevCmn->fnEMV_SelectApplication(nIndex);				// Index�� 1���� �������� ����
		fnAPD_CheckDeviceAction(DEV_MCU);
		nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_SELECTAPPLICATION);
		if (nRet != T_OK)
		{
			if ((memcmp(m_pDevCmn->m_szEMVFBCode, EMV_FBCODE_NOFALLBACK, 6) == 0 ) ||
				(memcmp(m_pDevCmn->m_szEMVFBCode, "      ", 6) == 0 )				)
				fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�");
			else
				return T_ERROR;
		}
	}
	else														// ����Ʈ �� ȭ�� ǥ��
	{
		for (i = 0; i < nBrandCnt; i++)
		{
			nRet = fnAPD_EMVSelectBrand();
			if (nRet == T_OK)									// ����
				break;											// ���� ����
			else
			if (nRet == EMV_RESULT_REVERSAL)					// �ĺ� ����Ʈ �� ǥ��
				continue;
			else
				return nRet;
		}
	}
	*/
	if (m_pDevCmn->EMVCandidateList[0].API != 0x80)
	{
		nIndex = 1;
		BrandSelectIndex = 1;									// ������ 1 Brand
		
		//**********************************************************************
		//  Select Application : ���õ� Application���� �ŷ� ����
		//**********************************************************************
		m_pDevCmn->fnEMV_SelectApplication(nIndex);				// Index�� 1���� �������� ����
		fnAPD_CheckDeviceAction(DEV_MCU);
		nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_SELECTAPPLICATION);
		if (nRet != T_OK)
		{
			if ((memcmp(m_pDevCmn->m_szEMVFBCode, EMV_FBCODE_NOFALLBACK, 6) == 0 ) ||
				(memcmp(m_pDevCmn->m_szEMVFBCode, "      ", 6) == 0 )				)
				fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�");
			else
				return T_ERROR;
		}
	}
	m_pDevCmn->fnEMV_ReadApplication();							// �ŷ��� ���� Application�� ������ ����
	fnAPD_CheckDeviceAction(DEV_MCU);
	nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_READAPPLICATION);
	if (nRet != T_OK)
	{
		if (nRet == EMV_RESULT_TERMINATE)
		{
			fnAPD_EMVResultProc();
		}

		return nReturn;
	}

	// MS Track 2 Image ����
	char szISO2[100] = {0x00,};
	
	m_pDevCmn->fnEMV_GetISO2(szISO2);

	CString strISO2("");

	strISO2  = ",";												// 1Track
	strISO2 += MakeUnPack(szISO2, strlen(szISO2));				// 2Track
	strISO2 += ",";												// 3Track

	fnAPD_CardReadCheck(strISO2);


	if (fnAPD_EMVJaTaCheck() == FALSE)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "����ī��� MS�� �ŷ�!!");
//		if ((Accept.DCC.BrandType[0] == 'M') &&			//V02-02-00-#18
//			(Accept.DCC.ICType == 'Y'))
//			return T_ERROR;
//		else
//			return T_OK;								//V02-02-00-#18
	}
	
	return T_OK;
}

// ��Ÿ�౸�� �ӽ��Լ�	//HNBV01-00-10 #1(EMV)
int CTranCmn::fnAPD_EMVJaTaCheck()
{
	BOOL    SeoulBinCheck = FALSE;	
	
//	CardTran &= (~MENU_J2);
//	CardTran &= (~MENU_T2);
//	SeoulDebitCard = FALSE;
/*
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV�� ī�� ��Ÿ�� ���� EMV_JaTa() IN");
HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVJaTaCheck EMV_CardData.ISO2Buff", CardData.ISO2Buff, sizeof(CardData.ISO2Buff));
	if ((CardData.ISO2size < 6)									||
		(CardData.ISO2size > sizeof(CardData.ISO2Buff))			||
		(FindChar(CardData.ISO2Buff, CardData.ISO2size, NULL) != CardData.ISO2size)
																||
		(IsZero(CardData.ISO2Buff, 6))							||
		(IsChar(CardData.ISO2Buff, 6, '='))){
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV�� ī��CardData.ISO2size11");			
	}
	else
	{
		if (memcmp(pCardData2->MembershipNo, BUYONLY, 6) == 0)
		{												// ��������ī��(�ϳ�����:ī��ݰ�)
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVJaTaCheck_CARD2_MENU_T2",2);
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() == HAN_MODE)
				fnAPP_CancelProc(T_MSG, "���� ���� ī�� �Դϴ�",  "�� ī��� ��� �� �� �����ϴ�");
			else
				fnAPP_CancelProc(T_MSG, "This card is not valid");
		}
//				else
// ����ī�� ����ó�� ���� -HNB10011
//				if ((memcmp(pCardData2->MembershipNo, DEBIT, 4) == 0)	||
//					(memcmp(pCardData2->MembershipNo, DEBIT2, 4) == 0)	||
//					(memcmp(pCardData2->MembershipNo, DEBIT3, 4) == 0));
//																// ����ī��(2TRī�幫ó��)

		else
		if ((memcmp(pCardData2->MembershipNo, VISAJJ, 6) == 0)		||
			(memcmp(pCardData2->MembershipNo, VISAJJ2, 6) == 0)		||
			(memcmp(pCardData2->MembershipNo, VISAJJ3, 6) == 0)		||
			(memcmp(pCardData2->MembershipNo, VISAJJ6, 6) == 0)		||		//20030404
			(memcmp(pCardData2->MembershipNo, VISAJJ7, 6) == 0)		||		// V01.00.29
			(memcmp(pCardData2->MembershipNo, VISAJJ12, 6) == 0)	||		// V06-23-00-#02 �ϳ�SKī�� BIN��ȣ �߰�
			(memcmp(pCardData2->MembershipNo, BCJJ18, 6) == 0)		||		// V01.00.29
			(memcmp(pCardData2->MembershipNo, BCJJ19, 6) == 0)		||		// V01.00.29
			(memcmp(pCardData2->MembershipNo, BCJJ20, 6) == 0)		||		// V01.00.29
			(memcmp(pCardData2->MembershipNo, BCJJ21, 6) == 0)		||		// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ22, 6) == 0)		||		// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ23, 6) == 0)		||		// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ24, 6) == 0)		||		// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ25, 6) == 0)		||		// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ26, 6) == 0)		||	  	// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ27, 6) == 0)		||	  	// V06-23-00-#02 �ϳ�SKī�� BIN��ȣ �߰�
			(memcmp(pCardData2->MembershipNo, BCJJ28, 6) == 0)		||	  	// V06-23-00-#02 �ϳ�SKī�� BIN��ȣ �߰�
			(memcmp(pCardData2->MembershipNo, BCJJ29, 6) == 0)		||	  	// V06-23-00-#02 �ϳ�SKī�� BIN��ȣ �߰�
			(memcmp(pCardData2->MembershipNo, BCJJ30, 6) == 0)		||	  	// V06-23-00-#02 �ϳ�SKī�� BIN��ȣ �߰�
			(memcmp(pCardData2->MembershipNo, BCJJ31, 6) == 0)		||	  	// V06-26-01-#01 �ϳ�SKī�� BIN��ȣ �߰�
			(memcmp(pCardData2->MembershipNo, BCJJ32, 6) == 0)		||	  	// V06-29-00-#02 �ϳ�SKī�� BIN��ȣ �߰�
			(memcmp(pCardData2->MembershipNo, BCJJ33, 6) == 0)		||	  	// V06-29-00-#02 �ϳ�SKī�� BIN��ȣ �߰�
			(memcmp(pCardData2->MembershipNo, MASTERJJ1, 6) == 0)	||  	// V05-02-04-#03 
			(memcmp(pCardData2->MembershipNo, MASTERJJ2, 6) == 0)	||      // V05-02-04-#03 
			(memcmp(pCardData2->MembershipNo, MASTERJJ3, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ4, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ5, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ6, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ7, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ8, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ9, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ10, 6) == 0)	||      // V06-23-00-#02 �ϳ�SKī�� BIN��ȣ �߰�
			(memcmp(pCardData2->MembershipNo, AMEXJJ1, 6) == 0)		||      // V05-02-04-#03 
			(memcmp(pCardData2->MembershipNo, AMEXJJ2, 6) == 0)		||		// V05-02-04-#03
			(memcmp(pCardData2->MembershipNo, LOCALJJ1, 6) == 0)	||		// V05-02-04-#03 
			(memcmp(pCardData2->MembershipNo, HANABIN1, 6) == 0)	||      // V05-02-17-#02 
			(memcmp(pCardData2->MembershipNo, HANABIN2, 6) == 0)	||		// V05-02-17-#02
			(memcmp(pCardData2->MembershipNo, HANABIN3, 6) == 0)	||		// V05-02-17-#02
			(memcmp(pCardData2->MembershipNo, HANABIN4, 6) == 0)	||		// V05-06-01-#01		  	  					
			(memcmp(pCardData2->MembershipNo, HANABIN5, 6) == 0)	)		// V06-15-00-#02 �ϳ�SKī�� Bin�ڵ� �߰�
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV�� ī��CardData.ISO2size22");
			CardTran |= MENU_J2;						// ����2TR
			CardService = 1;							// �������(�ϳ�����)
		}
		else
		if ((memcmp(pCardData2->MembershipNo, VISAJJ4, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, VISAJJ5, 6) == 0))
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV�� ī��CardData.ISO2size33");
			CardTran |= MENU_J2;						// ����2TR
			CardService = 2;							// �������(��������)
		}
		else
		if ((memcmp(pCardData2->MembershipNo, BCJJ, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ2, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ3, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ4, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ5, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ6, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ7, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ8, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ9, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ10, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ11, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ12, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ13, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ14, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ15, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ16, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, BCJJ17, 6) == 0))
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV�� ī��CardData.ISO2size44");
			CardTran |= MENU_J2;								// ����2TR
			CardService = 3;									// �����(��û����)
		}
		else 
		{
			if (memcmp(pCardData2->MembershipNo, SEOULDEBITID, 6) == 0)				// V01.01.12_#2 // 02.10
			{
				SeoulDebitCard = TRUE;
			}
		
			for (int i=0;i<21;i++)								//20030402 ����BCī��
			{
				if (memcmp(pCardData2->MembershipNo, SeoulBCBinCheck[i], 6) == 0)
					SeoulBinCheck = TRUE;
			}

			if (SeoulBinCheck)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV�� ī��CardData.ISO2size55");
				CardService = 4;								// ��������
				CardTran |= MENU_J2;							// Ÿ��2TR
			}
			else
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV�� ī��CardData.ISO2size66");
				for (int j =0; j<25; j++)					// V06-33-00-#03 Bin 24->25
				{
					if (memcmp(pCardData2->MembershipNo, TaHangBIN_Table[j], 6) == 0)
					{
						CardTran &= ~MENU_JJ;
						CardTran |= MENU_T2;				// Ÿ��2TR
						CardService = 0;
					}
				}
						
				if (CardTran & MENU_T2);
				else
				if (fnAPD_CheckBinCode(GetString(pCardData2->MembershipNo, 6, 6).GetBuffer(0)))
				{
					CardTran |= MENU_J2;						// ����2TR
					CardService = 5;							// �������(�ϳ�����)
				}
				else
					CardTran |= MENU_T2;						// Ÿ��2TR
			}
		}
	}
*/
	if (CardTran & MENU_J2)
		MenuAvail &= ~MENU_T2;
	else
		MenuAvail &= ~MENU_J2;

	// ����(����������ü), ���ڰ��������ȸ�� ���ุ ������
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV_JaTa���� Trancode = [%d]", TranCode);
	if ((TranCode == 15) ||
		(TranCode == 14)) 
	{
		if ((CardTran & MENU_T2)	||
			(CardService == 5)		)
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				fnAPP_CancelProc(T_MSG,"�� ī��δ� �� �ŷ��� �Ҽ� �����ϴ�");
			else
				fnAPP_CancelProc(T_MSG, "Your card can not", "be used at this transaction");
		}
	}

	return T_OK;
}

/*==============================================================================
* Function Name : fnAPD_EMVSelectBrand()
* Description   : �ŷ��� EMV Application�� ������ �� �� ���� ������ �޴� �Լ�
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR Etc.
* Release       : Ver 1.0
* Notes			: 
* ============================================================================*/
int CTranCmn::fnAPD_EMVSelectBrand()
{
	int		nReturn(T_ERROR);
	int		nBrandCnt = 0, nIndex = 0;				
	int		ScrNum = 0;
	int		i;
	char	szTemp[50];
	CString	strReturn("");
	CString	strOnOffString("");
	
	ScrNum = 425;

	nBrandCnt = m_pDevCmn->fnEMV_GetBrandCount();				// Brand ���� ������
	
	// ȭ�� ��ư Disable
	for (i = 0; i < nBrandCnt; i++)
	{
		if (strcmp(m_pDevCmn->EMVCandidateList[i].Brand, "VISA") == 0)
			m_pDevCmn->fnSCR_DisplayString(6, "VISA");
		else
		if (strcmp(m_pDevCmn->EMVCandidateList[i].Brand, "VISA ELECTRON") == 0)
			m_pDevCmn->fnSCR_DisplayString(7, "VISA ELECTRON");
		else
		if (strcmp(m_pDevCmn->EMVCandidateList[i].Brand, "PLUS") == 0)
			m_pDevCmn->fnSCR_DisplayString(8, "PLUS");
		else
		if (strcmp(m_pDevCmn->EMVCandidateList[i].Brand, "MASTER CARD") == 0)
			m_pDevCmn->fnSCR_DisplayString(1, "MASTER CARD");
		else
		if (strcmp(m_pDevCmn->EMVCandidateList[i].Brand, "MAESTRO") == 0)
			m_pDevCmn->fnSCR_DisplayString(2, "MAESTRO");
		else
		if (strcmp(m_pDevCmn->EMVCandidateList[i].Brand, "CIRRUS") == 0)
			m_pDevCmn->fnSCR_DisplayString(3, "CIRRUS");
		else
		if (strcmp(m_pDevCmn->EMVCandidateList[i].Brand, "JCB") == 0)
			m_pDevCmn->fnSCR_DisplayString(9, "JCB");
		else
		if (strcmp(m_pDevCmn->EMVCandidateList[i].Brand, "���� ���� �ſ�/Check") == 0)
			m_pDevCmn->fnSCR_DisplayString(4, "���� ���� �ſ�/Check");
		else
		if (strcmp(m_pDevCmn->EMVCandidateList[i].Brand, "���� ����������") == 0)
			m_pDevCmn->fnSCR_DisplayString(4, "���� ����������");
	}

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	memset( szTemp, NULL, sizeof(szTemp) );

	if (strReturn == "MASTER")
		strcpy( szTemp, "MASTER CARD" );
	else
	if (strReturn == "MAESTRO")
		strcpy( szTemp, "MAESTRO" );
	else
	if (strReturn == "CIRRUS")
		strcpy( szTemp, "CIRRUS" );
	else
	if (strReturn == "��������")
		strcpy( szTemp, "���� ���� �ſ�/Check" );
	else
	if (strReturn == "JCB")
		strcpy( szTemp, "JCB" );
	else
	if (strReturn == "VISA")
		strcpy( szTemp, "VISA" );
	else
	if (strReturn == "VISAELECTRON")
		strcpy( szTemp, "VISA ELECTRON" );
	else
	if (strReturn == "PLUS")
		strcpy( szTemp, "PLUS" );
	else
	if (strReturn == "����IC")
		strcpy( szTemp, "���� ����������" );
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);

	// ���õ� Application ã��
	for (i = 0; i < nBrandCnt; i++)
	{
		if (strcmp(szTemp, m_pDevCmn->EMVCandidateList[i].Brand) == 0)
		{
			nIndex = i + 1;										// Index�� 1���� �������� ����
			break;
		}
	}

	BrandSelectIndex = nIndex;	
	//**************************************************************************
	//  Select Application : ���õ� Application���� �ŷ� ����
	//**************************************************************************
	m_pDevCmn->fnEMV_SelectApplication(nIndex);					// Index�� 1���� �������� ����
	fnAPD_CheckDeviceAction(DEV_MCU);
	nReturn = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_SELECTAPPLICATION);

	return nReturn;
}

/*==============================================================================
* Function Name : fnAPD_EMVSendMakeProc()
* Description   : EMV �ŷ��� ���� �¶��� �۽� ��ó�� �Լ�
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CTranCmn::fnAPD_EMVSendMakeProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVSendMakeProc()");

	int		nReturn(T_ERROR), nRet;
	char	AnotherMoney[8];

	memset( AnotherMoney, 0x30, sizeof(AnotherMoney) );
	// OFFLINE AUTHENTICATION �䱸
	m_pDevCmn->fnEMV_OfflineDataAuthentication( Accept.Money,
					sizeof(Accept.Money), AnotherMoney, sizeof(AnotherMoney) );
	fnAPD_CheckDeviceAction(DEV_MCU);
	nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_OFFLINEDATAAUTHEN);
	if (nRet != T_OK)
	{
		if (nRet == EMV_RESULT_ONLINEDECLINE)
		{
			fnAPD_EMVResultProc();
			fnAPP_CancelProc( T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�. #1" );
		}
		else
		if (nRet == EMV_RESULT_TERMINATE)
		{
			fnAPD_EMVResultProc();
			fnAPP_CancelProc( T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�. #2" );
		}

		//**********************************************************************
		// EMV ī�� ���� Check : CID�� LSB�� �˻�
		// +----+----+----+----+-----------------------------------------------+
		// | b4 | b3 | b2 | b1 |                   ��    ��                    |
		// +----+----+----+----+-----------------------------------------------+
		// |  1 |    |    |    |                                ������ �ʿ���  |
		// +----+----+----+----+-----------------------------------------------+
		// |    |  0 |  0 |  0 |                                  ������ ����  |
		// |    |  0 |  0 |  1 |                       ���񽺰� ������ ����  |
		// |    |  0 |  1 |  0 |                 PIN �õ� ����Ƚ���� �ʰ�����  |
		// |    |  0 |  1 |  1 |                     �߱޻� ������ �����Ͽ���  |
		// |    |  X |  X |  X |                                          RFU  |
		// +----+----+----+----+-----------------------------------------------+
		//**********************************************************************
		BYTE bFlag;
		char StrBuff[81] = {0};
		
		bFlag = m_pDevCmn->fnEMV_GetCID9F27() & 0x0F;			// CID Error Check
		if (bFlag)												// Flag�� �ִٸ� EMV ���
		{
			switch (bFlag)
			{
			case 0x01 :											// ���񽺰� ������ ����
				strcpy( StrBuff, "�� ī��δ� �� ���񽺸� �̿��Ͻ� �� �����ϴ�" );
				break;
			
			case 0x02 :											// PIN �õ� ����Ƚ���� �ʰ�����
				strcpy( StrBuff, "PIN �Է� Ƚ���� �ʰ��Ͽ����ϴ�" );
				break;
			
			case 0x03 :											// �߱޻� ������ �����Ͽ���
				strcpy( StrBuff, "ī�� �߱޻�κ��� ������ �����Ͽ����ϴ�" );
				break;
			
			default :
				strcpy( StrBuff, "�� ī��δ� ���ŷ��� �� �� �����ϴ�. #3" );
				break;
			}
				
			fnAPD_EMVResultProc();

			if ( bFlag & 0x08 )									// ���� �Ǵ� �߱޻�� ���� �ʿ�
				fnAPP_CancelProc( T_MSG, StrBuff, "â���� �����Ͽ� �ֽñ� �ٶ��ϴ�" );
			else
				fnAPP_CancelProc( T_MSG, StrBuff);
		}
		else
		{
			memcpy(m_pDevCmn->m_szEMVFBCode, EMV_FBCODE_AFTER_OFFAUTHERR, sizeof(EMV_FBCODE_AFTER_OFFAUTHERR));
			fnAPP_CancelProc( T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�. #4" );
		}

		return nReturn;
	}

	//**************************************************************************
	//  EMV ���� ������ ������
	//**************************************************************************
	m_pDevCmn->fnEMV_GetHostTranData(EMV_SENDHOST_ARQC);

	return T_OK;
}

/*==============================================================================
* Function Name : fnAPD_EMVAfterRecvProc()
* Description   : �¶��� �� �ŷ� ó�� �Լ�
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CTranCmn::fnAPD_EMVAfterRecvProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVAfterRecvProc()");

	int		nReturn(T_OK), nRet;
	int nCnt = 0;
	// �� �� ���� ������ AP ����ڿ� �����Ͽ� ���Ѵ�.
	char	szHostData[512] = {0x00,};
	BOOL	bHostRecvFlag(TRUE);
	BYTE SixteenNulls[17];
	BYTE SixteenZeros[17];
		
	memset(SixteenNulls, 0x00, sizeof(SixteenNulls));
	memset(SixteenZeros,  0x30, sizeof(SixteenZeros));

	// #H0248 ARPC �������� ����  ������������������������
	// ARPC�� all zero�̸� ARPC ������ �����Ѵ�
	// M/Chip4, D-PAS, Local M/Chip�� ARPC�� 20����Ʈ���� �ϴ� 16�ڸ��� ���ϴ� ������....
	if (memcmp(m_RecvEmvrcvData.ARPC, SixteenZeros, 16) == 0)
	{
MsgDump(MAC_TRACE_CODE_FILE("Log"), "�ݡݡݡ� �ѽ������� ARPC ���� �����ϰ� ���� ó��!!! �ݡݡݡ�");
		return T_OK;
	}
	// ����������������������������������������������������������


	if ((m_RecvEmvrcvData.ARPC[0] == 0x32) &&
		(m_RecvEmvrcvData.ARPC[1] == 0x30))
		return T_OK;

HexDumpAnal(TRACE_CODE_MODE, "Log", "ARPC", m_RecvEmvrcvData.ARPC, 16);
HexDumpAnal(TRACE_CODE_MODE, "Log", "ARC", m_RecvEmvrcvData.APRCResposeCode, 4);
HexDumpAnal(TRACE_CODE_MODE, "Log", "Issuer Script Len", m_RecvEmvrcvData.IssuerScriptLen, 3);

/*
	memcpy(szHostData, m_RecvEmvrcvData.ARPC, 16);
	memcpy(&szHostData[16], m_RecvEmvrcvData.APRCResposeCode, 4);
	szHostData[20] = 0x30;
	memcpy(&szHostData[21], m_RecvEmvrcvData.IssuerScriptLen, 3);

	nCnt = Asc2Int(m_RecvEmvrcvData.IssuerScriptLen, 3);

MsgDumpAnal(TRACE_CODE_MODE, "Log", "nCnt[%d]", nCnt);
HexDumpAnal(TRACE_CODE_MODE, "Log", "Issuer Script", m_RecvEmvrcvData.IssuerScript, sizeof(nCnt));

	memcpy(&szHostData[24], m_RecvEmvrcvData.IssuerScript, nCnt);
*/
//	if(memcmp(m_RecvEmvrcvData.APRCResposeCode, "0012", 4)==0)
//		memcpy(m_RecvEmvrcvData.APRCResposeCode, "3030", 4);

	memcpy(szHostData, m_RecvEmvrcvData.ARPC, sizeof(m_RecvEmvrcvData));

	//**************************************************************************
	//  Online Processing : �¶��� ���� ��û�� ���� ȣ��Ʈ ������ ī��� ����
	//**************************************************************************
	if(m_pDevCmn->IfmDirect)									// V09-33-00-#01 ICCM ��� ����
		nRet = fnAPDLib_OnlineProcessing( szHostData, m_pDevCmn->TranResult, bHostRecvFlag );
	else
	{
		m_pDevCmn->fnEMV_OnlineProcessing( szHostData, m_pDevCmn->TranResult, bHostRecvFlag );
		fnAPD_CheckDeviceAction(DEV_MCU);
		nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_ONLINE);
	}

	if (nRet == EMV_RESULT_REVERSAL)
	{
		// ��� �ŷ��� ����
		nReturn = EMV_RESULT_REVERSAL;	 

		nRet = fnAPP_EMVReversalProc();							// ��� �ŷ� �ۼ���
		if (nRet != T_OK)
			nReturn = EMV_MAKE_REVERSAL_DATA;
	}
	else
	if (nRet != T_OK)
	{
		if (nRet == EMV_RESULT_TERMINATE)
			fnAPD_EMVResultProc();

		return T_ERROR;
	}

	// V09-33-00-#01 ICCM ��� ����
	// ���� EMV Lib�� ����ϴ� ���� �Ʒ� ó���� �ʿ� ����.
	if(m_pDevCmn->IfmDirect)
		return nReturn;

	//**************************************************************************
	//  Completion Processing : ī��� �ŷ� ���Ḧ ��û
	//**************************************************************************
	m_pDevCmn->fnEMV_CompletionProcessing();
	fnAPD_CheckDeviceAction(DEV_MCU);
	nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_COMPLET);

	if ((nReturn == EMV_RESULT_REVERSAL)	||
		(nRet == T_OK)						)
		;
	else
	if ((nRet == EMV_RESULT_REVERSAL)	&&
		(nReturn != EMV_RESULT_REVERSAL))
	{
		nReturn = EMV_RESULT_REVERSAL;
		nRet = fnAPP_EMVReversalProc();							// ��� �ŷ� �ۼ���
		if (nRet != T_OK)
			nReturn = EMV_MAKE_REVERSAL_DATA;
	}
	else
	if ((nRet == EMV_RESULT_TERMINATE)	&&
		(nReturn != EMV_RESULT_REVERSAL))
	{
		fnAPD_EMVResultProc();
		return T_ERROR;
	}
	else
	{
		return T_ERROR;
	}

	//**************************************************************************
	//  AfterScript Processing : �ŷ� ���� �� Issuer Script ó��
	//**************************************************************************
	m_pDevCmn->fnEMV_AfterScriptProcessing();
	fnAPD_CheckDeviceAction(DEV_MCU);
	nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_AFTERSCRIPT);

	if (!m_pDevCmn->TranResult)
		;
	else
	if (nRet != T_OK)
	{
		if (nRet == EMV_RESULT_ONLINEDECLINE)
		{
			fnAPD_EMVResultProc();
//			fnAPP_CancelProc(T_MSG, "�¶��� ������ �źεǾ����ϴ�"); //#H0248
		}
		else
		if (nRet == EMV_RESULT_TERMINATE)
		{
			fnAPD_EMVResultProc();
		}

		return T_ERROR;
	}
	
	fnAPD_EMVResultProc();

	return nReturn;
}

/*==============================================================================
* Function Name : fnAPP_EMVReversalProc()
* Description   : ��Ұŷ� ó�� �Լ�
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CTranCmn::fnAPP_EMVReversalProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_EMVReversalProc()");

	int		nReturn(T_ERROR);

	// MS Track 2 ����
	// ��� �ŷ��� ���� MS 2 Track ������ ����Ѵٸ� �� �κ��� �����ص� �����ϴ�.
	char szISO2[100] = {0x00,};
	memset( &CardData.ISO2Buff, 0, sizeof(CardData.ISO2Buff) );			// �ʱ�ȭ HNBV010010 #1(EMV) DATA�����
	
	CardData.TrInfo |= ISO2_TRACK;
	CardData.ISO2size = m_pDevCmn->fnEMV_GetISO2(szISO2);
	memcpy( CardData.ISO2Buff, szISO2, CardData.ISO2size );

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CardData.ISO2Buff", CardData.ISO2Buff, sizeof(CardData.ISO2Buff));
	fnAPDLib_EMVSendMakeProc(2);								// V09-33-00-#01	ICCM ��� ����	
	m_pDevCmn->fnEMV_GetHostTranData(EMV_SENDHOST_REVERSAL);	// kh 20041007


	return TRUE;
}

/*==============================================================================
* Function Name : fnAPD_EMVResultProc()
* Description   : EMV �ŷ� ����� ��ȸ�ϴ� �Լ�(������ & ������)
* Parameters    : (NONE)
* Return        : T_OK
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CTranCmn::fnAPD_EMVResultProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV_ResultProc()");

	int		nReturn(T_ERROR);

	m_pDevCmn->fnEMV_ResultData();
	fnAPD_CheckDeviceAction(DEV_MCU);
	nReturn = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_RESULTDATA);

	return T_OK;												// ����� �˱� ���� �ΰ����� �Լ��� ����ó�� ���� ����
}

// ī�忬�Ӱŷ�ó��
int CTranCmn::fnAPD_CardReadSerial()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadSerial()");
	m_pDevCmn->fnAPL_CaptureSaveEnd();				
	if ((!MenuFICMode)	&&										// ����IC
		(!MenuEMVMode)	&&										// EMV
		(!MenuKCashMode))
		return T_OK;

	if ((MenuFICMode)	||										// ����IC
		(MenuEMVMode)	||										// EMV
		(MenuKCashMode)	)										// ����ȭ��
	{
		if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 823)
			m_pDevCmn->fnSCR_DisplayScreen(823);
		DevEnDisable &= (~DEV_MCU);
		fnAPD_CardEnDisable(ENABLE, TRUE, TRUE, TRUE);			// ����IC/K-CASH ��밡�� V03-00-22(LSY) 2004.10.04
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (fnAPD_ICCardReadCheck() == T_ERROR)					// ICī���б�	V03-00-22 2004.10.14
			fnAPP_CancelProc(T_MSG, "�� ī��� ����� �� �����ϴ�.",
									"ī�������� Ȯ�� �Ͻñ� �ٶ��ϴ�.");

		if (MenuFICMode)
		{
			fnAPD_FICDistinctCardType(TRAN_IC_FINANCEIC);
			fnAPD_FICBasicInquiry();						// �⺻������ȸ
		}
		else
		if (MenuEMVMode)
		{
			if (fnAPD_EMVContinueTrans() == T_ERROR)			// EMV�⺻����READ
			{
				CardPowerOnFlg = FALSE;							// ī�����ӹ�
				fnAPD_CardEnDisable(ENABLE, TRUE, TRUE, TRUE);	// ICī���б� ������ ī�忣Ʈ�������� ���� 2004.10.26
				m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				fnAPD_CardReadCheck();
				FallBackEMVMode = TRUE;
			}
		}
		else
		if (MenuKCashMode)
		{
			fnAPD_FICDistinctCardType(TRAN_IC_KCASH);
			MenuKCashMode = MENU_IC;
		}
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadSerial()::return");
	return T_OK;
}
//#0013 Auth Logic 2010.09.14 
BYTE CTranCmn::fnAuthGetDigit(CString strTemp)
{
	BYTE lrc = 0x00;

	for(int i=0; i<strTemp.GetLength(); i++)
		lrc ^= ((LPCTSTR)strTemp)[i];
	
	if(lrc >= 10) //':'���� ũ�� 10���� ������
		lrc %= 0x0A;
	
	return (lrc |= 0x30);
}

BYTE CTranCmn::fnAuthGetAccumlate(CString strTemp)
{
	BYTE nResult = 0;

	for(int i=0; i<strTemp.GetLength(); i++)
		nResult += Asc2Int(strTemp.Mid(i, 1), 1);

	return (nResult %= 10);
}

BYTE CTranCmn::fnAuthGetAccumlate(int nPreSum, CString strTemp)
{
	BYTE nResult = 0;

	for(int i=0; i<strTemp.GetLength(); i++)
		nResult += Asc2Int(strTemp.Mid(i, 1), 1);

	nResult += nPreSum;

	return (nResult %= 10);
}

BOOL CTranCmn::IsBCCRight(BYTE* pbyBuf, DWORD dwBufSize)
{
	BYTE crckCheck[10];
	CString strCardno((LPCTSTR)pbyBuf, dwBufSize);
	CString strResult;
	CString strTemp;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "IsBCCRight()");

	strTemp = strCardno.Left(strCardno.Find('='));

    crckCheck[0] = fnAuthGetDigit(strTemp.Mid(0, 4));
    crckCheck[1] = fnAuthGetDigit(strTemp.Mid(4, 4));
    crckCheck[2] = fnAuthGetDigit(strTemp.Mid(8, 4));
    crckCheck[3] = fnAuthGetDigit(strTemp.Mid(12, 4));
    
    crckCheck[4] = fnAuthGetAccumlate(strTemp.Mid(0, 4));
    crckCheck[5] = fnAuthGetAccumlate(crckCheck[4], strTemp.Mid(4, 4));
    crckCheck[6] = fnAuthGetAccumlate(crckCheck[5], strTemp.Mid(8, 4));
    crckCheck[7] = fnAuthGetAccumlate(crckCheck[6], strTemp.Mid(12, 4));
    
     strResult.Format("%s=%c%c%c%c=%d=%d=%d=%d=", strTemp
    , crckCheck[0], crckCheck[1], crckCheck[2], crckCheck[3]
    , crckCheck[4], crckCheck[5], crckCheck[6], crckCheck[7]);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "IsBCCRight(): [%s], [%s]", strCardno, strResult);

	if (strResult.CompareNoCase(strCardno))
		return FALSE;

	return TRUE;
}

// U8100-AP���� #01 - 3Camera, Preview����, �ؽ�Ʈ���
CString CTranCmn::fnAPP_GetCaptureData()
{
	CString szTempData("");
	
	if (m_pDevCmn->AtmDefine.CashHandler != HBRM_SET)
		;
	else
	{
		if (Asc2Int(Accept.Money, 8) > 0)
		{
			if (!AddSerialFlg)							// �Աݼ۽����� 
				szTempData.Format("%1.1c0%1.1c0%1.1cNO:%6.6s(+1) ACN:%16.16s AMT:%8.8s", 
				0x1B, 0x1C, 0x1D, m_pProfile->TRANS.SerialNo, fchpAPP_EditAcnt(FALSE, TRUE), Accept.Money);
			else										// �ŷ��Ϸù�ȣ, ���¹�ȣ, �ݾ�
				szTempData.Format("%1.1c0%1.1c0%1.1cNO:%6.6s ACN:%16.16s AMT:%8.8s", 
				0x1B, 0x1C, 0x1D, m_pProfile->TRANS.SerialNo, fchpAPP_EditAcnt(FALSE, TRUE), Accept.Money);
		}
		else
		{
			if (!AddSerialFlg)
				szTempData.Format("%1.1c0%1.1c0%1.1cNO:%6.6s(+1) ACN:%16.16s ", 
					0x1B, 0x1C, 0x1D, m_pProfile->TRANS.SerialNo, fchpAPP_EditAcnt(FALSE, TRUE));
			else
				szTempData.Format("%1.1c0%1.1c0%1.1cNO:%6.6s ACN:%16.16s ", // 5������ ���� (T3ATM AP ���泻�� #18) : AMT���� �κ� ����
					0x1B, 0x1C, 0x1D, m_pProfile->TRANS.SerialNo, fchpAPP_EditAcnt(FALSE, TRUE));
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetCaptureData():%s", szTempData);
	return	szTempData;
}


/*==============================================================================
* Function Name : fnAPDLib_OnlineProcessing()
* Description   : �¶��� ���� ��û�� ���� ȣ��Ʈ ������ ī��� �����ϴ� �Լ�
* Parameters    : lpszHostData : Host Response Data
*                 nHostResp    : �ŷ��� ���� Host�� ����, ���� ����
*                 HostRecvFlag : Host ������ �޾Ҵ��� �ۼ��� ���������� �Ǵ�.
*                                Host ������ �޾����� ARC�� ���� ��쿡��
*                                nHostResp�� ���� ARC�� ������ش�.
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2013.08.22 Lee Dong Chul
* ============================================================================*/
int CTranCmn::fnAPDLib_OnlineProcessing(LPSTR lpszHostData, int nHostResp, BOOL HostRecvFlag)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_OnlineProcessing()");
	
	int		nReturn(FALSE);
	int		nDataLen = 0;
	int		nFailReason = 0;
	CString strICSendData("");
	int		nOffset = 0;
	BYTE	bData[IC_DATA_LEN] = {0,};
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPDLib_OnlineProcessing() lpszHostData = %s", lpszHostData);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPDLib_OnlineProcessing() nHostResp = %d", nHostResp);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPDLib_OnlineProcessing() HostRecvFlag = %d", HostRecvFlag);
	
	memset(    bData, 0,      sizeof(bData) );
	
	// Data Constructuon
	nDataLen = m_pDevCmn->fnEMV_MakeOnlineProcessing( bData, lpszHostData, nHostResp, HostRecvFlag );
	nOffset = 2;

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "bData ", &bData, nDataLen);

//	m_strEMVHostResponse.Format("%s",&bData[2]);

	MakeUnPack( (void*)&bData[nOffset], m_strEMVHostResponse, nDataLen - nOffset);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPDLib_OnlineProcessing() m_strEMVHostResponse = %s", m_strEMVHostResponse);

	nFailReason = fnAPLLib_EmvOnlineApproval(1, nHostResp);
	// �����ϸ� reversal 

	if(nFailReason != 0 )
		nReturn = EMV_RESULT_REVERSAL;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_OnlineProcessing(...):return[%d]", nReturn);

	return nReturn;
}

int	CTranCmn::fnAPDLib_EMVSelectAID()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVSelectAID()");

	CString strAIDList, strUserConfirm, strAIDNumber;
	BYTE nUserConfirm, nAIDNumber;
	int nSelected;
	int nTryCount = 0;
	int nAIDIndex = 0;
	do
	{
		nTryCount ++;
		strAIDList = m_pDevCmn->fnEMVLib_Sel_GetAppListEx();
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Sel_GetAppListEx() = [%s]", strAIDList);
		if( strAIDList.CompareNoCase(_T("ERROR")) == 0)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Geting AID List Failed");
			return -1;
		}
		else if(strAIDList.GetLength()<=4)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AID List string is too short [%d]", strAIDList.GetLength() );
			return 0;
		}
		strUserConfirm = strAIDList.Left(2);
		strAIDNumber = strAIDList.Mid(2,2);
		Asc2Byte( (void*)(LPCTSTR) strUserConfirm, &nUserConfirm);
		Asc2Byte( (void*)(LPCTSTR) strAIDNumber, &nAIDNumber);
	
		nSelected = 0;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "UserConfirm=[%d], Number of AID = [%d]", nUserConfirm, nAIDNumber );

		// AID ID, Label ����
		int nMaxAID = fnAPDLib_EMVUserAIDLoad(nAIDIndex, nUserConfirm, nAIDNumber, strAIDList.Mid(4));
		nAIDIndex++;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AID Number = [%d], Max AID = [%d]", nAIDNumber, nMaxAID );

		if( (nUserConfirm==0)&&(nAIDNumber>=1) )
			nSelected = 1;							// Select the First one
		else if( (nUserConfirm==1)&&(nAIDNumber>=1) )
		{
			nSelected = fnAPDLib_EMVUserAIDSelection(nAIDNumber, strAIDList.Mid(4) );
			if(nSelected==-1)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVUserAIDSelection() Failed or User Cancelled");
				return -1;
			}
		}

		if(nSelected==0)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NO Possible AID, nSelected = [0] => FALL BACK");
			return 0;
		}

		BrandSelectIndex = nSelected;

// ������� - start
		// V07-08-00-#01: �ؿ�ī�� �ŷ�  EMV ����
		if ((TranCode2 == TC_WITHCASHAD)	||		// �������
			(TranCode2 == TC_WITHSAVEAC)	||		// ���뿹������
			(TranCode2 == TC_WITHCHKAC)		||		// ���¿�������
			(TranCode2 == TC_INQCREDAC)		||		// ���ݼ�����ȸ
			(TranCode2 == TC_INQSAVEAC)		||		// ���뿹����ȸ
			(TranCode2 == TC_INQCHKAC)			)	// ���¿�����ȸ
		{
			// �ؿ�ī�� �ŷ� �����Ͱ迭�� EMV ����, �����Ϳ� M/S ��ȯó����.
			if( (memcmp(m_pDevCmn->EMVCandidateList[nSelected-1].AID, MASTER_AID,  sizeof(MASTER_AID)) == 0)  ||
				(memcmp(m_pDevCmn->EMVCandidateList[nSelected-1].AID, MAESTRO_AID, sizeof(MAESTRO_AID)) == 0) ||
				(memcmp(m_pDevCmn->EMVCandidateList[nSelected-1].AID, CIRRUS_AID,  sizeof(CIRRUS_AID)) == 0)	   )
			{
				MsgDump(TRACE_CODE_MODE, "EMV", __FILE__, __LINE__, "�ؿ�ī�� �ŷ� MASTER �迭�� EMV����");
				EMVServiceCodeChk = TRUE;		// �����ڵ� Ȯ�ο���
			}
			else
			{
				MsgDump(TRACE_CODE_MODE, "EMV", __FILE__, __LINE__, "�ؿ�ī�� �ŷ� VISA �迭 Fallback");
				EMVServiceCodeChk = FALSE;		// �����ڵ� Ȯ�ο���
			}	
		}	
// ������� - end

		long nRtn = m_pDevCmn->fnEMVLib_Sel_FinalAppSelection(nSelected-1);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Sel_FinalAppSelection(%d) = [%ld]", nSelected-1, nRtn);
		if (nRtn == EMV_RSLT_OK)
		{
			// [#2188-1] InitApp ���� �� ������ ���� Ȯ��
			int			nResult = 0;
			CString strProcCode = _T("00");
			CString strAmount = _T("000000000000");

			// Transaction Date and Time
			SYSTEMTIME localTime;
			::GetLocalTime(&localTime);
			CString strDateTime = _T("000000000000");
			strDateTime.Format("%02d%02d%02d%02d%02d%02d", (localTime.wYear - 2000), localTime.wMonth, localTime.wDay, localTime.wHour, localTime.wMinute, localTime.wSecond);
			nResult = m_pDevCmn->fnEMVLib_InitAppProcess( (LPCTSTR)strProcCode, (LPCTSTR)strAmount, (LPCTSTR)strDateTime, Asc2Int(m_pProfile->TRANS.SerialNo));
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_EmvTranDataMake():m_pDevCmn->fnEMV_InitAppProcess:nResult(%d)", nResult);

			if (nResult == EMV_ERR_NOT_ACCEPT)
			{
				// [#2188-1] AID Select Fail�� Error Message ǥ��
				m_pDevCmn->fnSCR_DisplayString(2, "Failed to Selected AID");
				m_pDevCmn->fnSCR_DisplayScreen(710);
				Delay_Msg(1000);
				// end of [#2188]
				continue;	// 2013.06.17 SHPARK 
			}
			else if(nResult == EMV_ERR_CARD_SW)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_InitAppProcess()  = EMV_ERR_NOT_SUPPORT or EMV_ERR_CARD_SW ==> FALL BACK...");
				return 0;
			}
			
			else if( (nResult != EMV_RSLT_OK) || (nResult == EMV_ERR_NOT_SUPPORT) )
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_InitAppProcess()  NOT [EMV_ERR_NOT_ACCEPT] and [EMV_NOT_ACCEPT]");
				return -1;		
			}
			// end of [#2188-1]

			AID = AIDList.GetAt(nSelected-1);
 			CString strTemp;		// [#2188-2]
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Sel_FinalAppSelection() SUCCESS [EMV_RSLT_OK], Selected AID = [%s]", AID);

			
			
			// [#2188-2] AID Select Success�ÿ� AID Label�� ȭ�鿡 ǥ���ؾ��� MODIFY
			
			// >>> @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK - ���õ� AID�� Label ���� ǥ��
			// strTemp.Format("Selected AID : %s", strAPPLabel);
			strTemp = AIDescList.GetAt(nSelected-1);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Sel_FinalAppSelection() SUCCESS [EMV_RSLT_OK], Selected Label = [%s]", strTemp);
			if (AIDConvertFlag.GetAt(nSelected-1) == "TRUE")
			{

//��-s			wstring wsAppLabel = fnAPDLib_MBStoWCS(strTemp);
				CString wsAppLabel = fnAPDLib_MBStoWCS(strTemp);
				m_pDevCmn->fnSCR_DisplayString(2, wsAppLabel);
			}
			else
			{
				CString sAppLabel = strTemp;
				m_pDevCmn->fnSCR_DisplayString(2, sAppLabel);
			}
			// <<< @ADD:[EMVKERNEL5_001]

//			m_pDevCmn->fnSCR_DisplayScreen(61);
//			Delay_Msg(1000);
			// end of [#2188-2]
			return nSelected;
		}
		else if( (nRtn == EMV_ERR_NOT_SUPPORT)||(nRtn == EMV_ERR_CARD_SW) )
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Sel_FinalAppSelection()  = EMV_ERR_NOT_SUPPORT or EMV_ERR_CARD_SW ==> FALL BACK...");
			return 0;
		}
		else if (nRtn == EMV_NOT_ACCEPT)
		{
			// [#2188-1] AID Select Fail�� Error Message ǥ��
			m_pDevCmn->fnSCR_DisplayString(2, "Failed to Selected AID");
			m_pDevCmn->fnSCR_DisplayScreen(710);
			Delay_Msg(1000);
			// end of [#2188]
		}
		else if(nRtn!=EMV_ERR_NOT_ACCEPT)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Sel_FinalAppSelection()  NOT [EMV_ERR_NOT_ACCEPT] and [EMV_NOT_ACCEPT]");
			return -1;
		}
	} while (nTryCount <=7 );

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Selection tried [%d] times, fnAPD_EMVSelectAID() return [-1]", nTryCount);
	return -1;
}

int CTranCmn::fnAPDLib_EMVUserAIDLoad(int nAIDIndex, BYTE btUserConfirm, int nAIDNum, LPCTSTR strAID)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVUserAIDLoad(nAIDNum=[%d], strAID=[%s])", nAIDNum, strAID);

	// AID LENGTH CHECK
	CString sAIDList = CString(strAID);
	if (sAIDList.GetLength() < 4)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AID List is too short (Len=%d), return [0]", sAIDList.GetLength() );
		return 0;
	}

	// @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK - ����� �ʱ�ȭ
	AIDescList.RemoveAll();	
	AIDList.RemoveAll();	
	AIDConvertFlag.RemoveAll();	

	// Local variables
	int nAIDListIndex = 0;
	int i=1, nMaxAID = 0;
	BOOL bContinue = FALSE;
	BYTE nLen = 0x00, nLenAID = 0x00;
	CString strLen(""), strValue(""), strConverted(""), strAPI("");
	CString strISOCodeIndex(""), strCodeIndex(""), strConvertFlag("FALSE");
	int nCodeIndex = 0;
	
	do
	{
		bContinue = FALSE;

		// >>> @ADD:[EMVKERNEL5_001] 2013.05.30 SHPARK - ISO �ڵ尪�� �ٸ� �����ڵ� MAPPING ó������
		// Select Flag(1) + appCount(1) + [9F11(2)][AppLabel����(2)][AppLabel(�� ��)][AidName����(2)][AidName(����)] + �ݺ�...
		// "00 01  0010 2020202020202020202020202020202007A0000010100020"
		// "010201104243204A434220437265646974202020 07A000000065101000104369727275732020202020202020202007A0000000046000)

		// Select Flag(1) + appCount(1) + [9F11(2)][AppLabel����(2)][priority(1)][AppLabel(�� ��)][[AidName����(2)][AidName(����)] + �ݺ�...
		// �켱���� �߰��Ѱ� "010201104243204A434220437265646974202020 01 07A0000000651010001043697272757320202020202020202020 02 07A0000000046000]
			

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AID List : [%s]", sAIDList);
		// Code Index Range : 01 - 10 (�̿��� ���� WCS MBS ��ȯ���� �״�� ���)
		strCodeIndex = sAIDList.Left(2);
		nCodeIndex = Asc2Int(strCodeIndex); 
		if (nCodeIndex > 0 && nCodeIndex <= 10)
		{
			strConvertFlag = "TRUE";
			strISOCodeIndex.Format("ISO8859_%s", strCodeIndex);
		}
		else 
		{
			strConvertFlag = "FALSE";
			strISOCodeIndex.Format("NOT USED");
		}
		sAIDList = sAIDList.Mid(2);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AID List without 9F11 tag : [%s]", sAIDList);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Issuer Code Index : [%s]", strISOCodeIndex);

		// <<< @ADD:[EMVKERNEL5_001]

		// Get Length 
		strLen = sAIDList.Left(2);

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strLen : [%s]", strLen );
		Asc2Byte( (void*)(LPCTSTR) strLen, &nLen);

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "nLen ", &nLen, 1);

		if ((nLen > 0) && (sAIDList.GetLength() > (nLen*2 + 4)))
		{
/*
			strValue = sAIDList.Mid(2, nLen*2); 
			sAIDList = sAIDList.Mid(2 + nLen*2);
*/
			strValue = sAIDList.Mid(2, nLen*2);
			strAPI   = sAIDList.Mid(nLen*2 + 2, 2);

			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strAPI   strAPI   strAPI   strAPI    : [%s]", strAPI   );

			sAIDList = sAIDList.Mid(nLen*2 + 2 + 2);

			
			// AID Label
			BYTE *sOut = new BYTE[nLen*2+1];	
			memset(sOut, NULL, nLen*2+1);
			int nConverted = MakePack( (void*)(LPCTSTR)strValue, (void*)sOut, strValue.GetLength(), '^', 0);

			// >>> @ADD:[EMVKERNEL5_001] 2013.05.30 SHPARK - TEST CODE
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "sOut : [%s]", sOut);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strValue : [%s]", strValue);
			// <<< @ADD:[EMVKERNEL5_001]

			if (nConverted > 0)
			{
				CString strAIDLen(""), strAIDRaw(""), strAIDConverted("");
				
				strAIDLen = sAIDList.Left(2);
				Asc2Byte( (void*)(LPCTSTR) strAIDLen, &nLenAID);
				
				if ((nLenAID > 0) && (sAIDList.GetLength() >= (nLenAID*2 + 2)))
				{
					strAIDRaw = sAIDList.Mid(2, nLenAID*2);
					
					// AID
//					BYTE *sOutAID = new BYTE[nLenAID*2+1];
//					memset(sOutAID, NULL, nLenAID*2+1);
//					int nAIDConverted = MakePack( (void*)(LPCTSTR)strAIDRaw, (void*)sOutAID, strAIDRaw.GetLength(), '^', 0);
//					if(nAIDConverted>0)
					if (strAIDRaw.GetLength() > 0)
					{
						nMaxAID++;
						
						// >>> @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK - Issure Code�� ���� ���ڿ� Convert
						if (strConvertFlag == "TRUE")
						{			
							strConverted = "";
							CString strAIDChar("");
							CString strEndian("");

							for (int i=0; i<nLen; i++)
							{
								strAIDChar.Format("%02X",  sOut[i]);
								
								strEndian = IniGetStr("C:\\T1ATM\\INI\\ISO8859Code.ini", strISOCodeIndex, strAIDChar, "0");								
								strConverted += strEndian.Right(2) + strEndian.Left(2);
							}

							// ���ڿ��� ���ۿ� �Ҵ�
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Data before PACK : [%s]", strConverted);
							BYTE *buff = new BYTE[nLen*2 + 1];
							memset(buff, NULL, nLen*2 + 1);
							int nConvertedOutLen = MakePack( (void*)(LPCTSTR)strConverted, (void*)buff, strConverted.GetLength(), '^', 0);
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Packed length : [%d]", nConvertedOutLen);

							// WCS to MBS
							strConverted = fnAPDLib_WCStoMBS(buff, nConvertedOutLen);
							delete [] buff;					
							// <<< @ADD:[EMVKERNEL5_001]
						}
						else
						{
							strConverted.Format("%s", sOut);
						}

						// ���� ���õ� AID Label ������ �ľ��ϱ����� �������� ����
						strAIDConverted.Format("%s", strAIDRaw);
						AIDescList.Add(strConverted);
						AIDList.Add(strAIDConverted);
						AIDConvertFlag.Add(strConvertFlag);

						m_pDevCmn->EMVInfo.CandidateCount = nAIDIndex + 1;

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strAPI22222222222    : [%s]", strAPI   );

						MakePack((void*)(LPCTSTR)strAPI, &m_pDevCmn->EMVCandidateList[nAIDListIndex].API, 2);
						m_pDevCmn->EMVCandidateList[nAIDListIndex].AIDLen = strAIDConverted.GetLength() / 2;

						Asc2Byten((void*)(LPCTSTR)strAIDConverted, m_pDevCmn->EMVCandidateList[nAIDListIndex].AID, m_pDevCmn->EMVCandidateList[nAIDListIndex].AIDLen);
						Asc2Byten((void*)(LPCTSTR)strConverted, (BYTE*)m_pDevCmn->EMVCandidateList[nAIDListIndex].Brand, sizeof(m_pDevCmn->EMVCandidateList[nAIDListIndex].Brand));
						wsprintf( m_pDevCmn->EMVCandidateList[nAIDListIndex].Index, "%2d", m_pDevCmn->EMVInfo.CandidateCount );


HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->EMVCandidateList[nAIDIndex].API", &m_pDevCmn->EMVCandidateList[nAIDListIndex].API, 1);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->EMVCandidateList[nAIDIndex].AIDLen", &m_pDevCmn->EMVCandidateList[nAIDListIndex].AIDLen, 1);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->EMVCandidateList[nAIDIndex].AID", &m_pDevCmn->EMVCandidateList[nAIDListIndex].AID, sizeof(m_pDevCmn->EMVCandidateList[nAIDListIndex].AID));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->EMVCandidateList[nAIDIndex].Brand", &m_pDevCmn->EMVCandidateList[nAIDListIndex].Brand, sizeof(m_pDevCmn->EMVCandidateList[nAIDListIndex].Brand));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Parsed AID [%d], AID=[%s], DESC=[%s], FLAG=[%s]", nMaxAID, strAIDConverted, strConverted, strConvertFlag);
					}

					if (sAIDList.GetLength() >= (nLenAID*2 + 4))
					{
						sAIDList = sAIDList.Mid(nLenAID*2+2);
						bContinue = TRUE;
					}

//					delete [] sOutAID;
				}
			}

			delete [] sOut;
		}

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Continue = [%d], Remain AIDList =[%s]", bContinue, sAIDList);
		nAIDListIndex++;

	} while(bContinue==TRUE);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Max AID = [%d]", nMaxAID);
	return nMaxAID;
}
// <<< @ADD:[EMVKERNEL5_001] 

// [#156] US Justin 2012.04.25 Implement EMV Transaction
int	CTranCmn::fnAPDLib_EMVUserAIDSelection(int nAIDNum, LPCTSTR strAID)
{
	int nSelected = -1;
	int nMaxAID = nAIDNum; //fnAPD_EMVUserAIDLoad(nAIDNum, strAID);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Parsed AID = [%d]", nMaxAID);

	// V07-13-01-#01: ������� 2 Brand�̻��� ��� �ֿ켱 ������ Brand�� �ڵ������Ѵ�
	int HighIndex = 1000;
	if (nMaxAID > 1)  
	{		
		// �켱��������
		for(int i = 0; i < nMaxAID; i++)
		{
			HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVCandidateList[i].API", 
																&m_pDevCmn->EMVCandidateList[i].API, 
																sizeof(m_pDevCmn->EMVCandidateList[i].API));
			if ((m_pDevCmn->EMVCandidateList[i].API & 0x80) != 0x80)	// 0x80 ������
			{				
				if((m_pDevCmn->EMVCandidateList[i].API & 0x0F) != 0x0F)	// 0x0f �켱���� ������
				{
					if(HighIndex > m_pDevCmn->EMVCandidateList[i].API)
					{
						HighIndex = m_pDevCmn->EMVCandidateList[i].API;
						nSelected = i+1;	// �ֿ켱���� Index����
					}
				}
			}
		}
		
		if(nSelected == -1)
			nSelected = 1;
	}
	else
	{
		nSelected = 1;		
	}
	
	m_pDevCmn->EMVInfo.TranIndex = nSelected - 1;					// ���� ���õ� Index ����	

	// [#2188-2] TEST CODE
	CString sAIDConvertFlag = AIDConvertFlag.GetAt(nSelected-1);
	CString sAID = AIDList.GetAt(nSelected-1);
	CString sAIDDescription = AIDescList.GetAt(nSelected-1);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Selected AID=[%s], AID Desc = [%s], Flag = [%s]", sAID, sAIDDescription, sAIDConvertFlag);

/* V07-13-01-#01: Ÿ ������ Brand ȭ�鿡 ǥ���Ͽ� ���õ� Brand�� �ŷ� ó���ؾ���----------------------
	if(nMaxAID>=1)
	{
		int i;
		// [#2188-1] TEST CODE
		for(i=1; i<=nMaxAID; i++)
		{
			// >>> @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK - ȭ�鿡 AID LABEL ���� ǥ��
			if (AIDConvertFlag.GetAt(i-1) == "TRUE")
			{

// ��-s
//				wstring wsDisplayText = fnAPDLib_MBStoWCS(m_sAIDescList.GetAt(i-1));
				CString wsDisplayText = fnAPDLib_MBStoWCS(m_sAIDescList.GetAt(i-1));
				m_pDevCmn->fnSCR_DisplayString(i, wsDisplayText);
			}
			else 
			{
				CString sDisplayText = m_sAIDescList.GetAt(i-1);
				m_pDevCmn->fnSCR_DisplayString(i, sDisplayText);
			}
			// <<< @ADD:[EMVKERNEL5_001] 
		}
		// end of [#2188-1]

		for(i=(nMaxAID+1); i<=7; i++)
			m_pDevCmn->fnSCR_DisplayString(i, "" );
		
// ��-s		
		m_pDevCmn->fnSCR_DisplayScreen(61,K_30_WAIT);
		CString strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyString() = [%s]", strReturn);

		if(strReturn.GetLength()==1)
		{
			int nUserInput = atoi(strReturn);
			if( (nUserInput>0)&&(nUserInput<=nMaxAID) )
			{
				nSelected = nUserInput;
				// [#2188-2] TEST CODE
				CString sAIDConvertFlag = AIDConvertFlag.GetAt(nSelected-1);
				CString sAID = AIDList.GetAt(nSelected-1);
				CString sAIDDescription = m_sAIDescList.GetAt(nSelected-1);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Selected AID=[%s], AID Desc = [%s], Flag = [%s]", sAID, sAIDDescription, sAIDConvertFlag);
				// end of [#2188-2]
			}
		}
	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AID Parsing Error, return [0]");
		return 0;
	}
-------------------------------------------------------------------------------------------------------------*/
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVUserAIDSelection(..) return=[%d]",nSelected);
	return nSelected;
}

CString	CTranCmn::fnAPDLib_WCStoMBS(LPBYTE pAIDName, int nLen)
{
	CString strResult("");
	
	WCHAR *wsAIDName = new WCHAR[nLen / 2+1];

	wmemset(wsAIDName, NULL, nLen / 2+1);
	wmemcpy(wsAIDName, (WCHAR*)pAIDName, nLen / 2);

	// Get Length in MBS
	int nMBSLength = WideCharToMultiByte(CP_UTF8, 0, wsAIDName, -1, NULL, 0, NULL, NULL);						
	char *sAIDName = new char[nMBSLength + 1];
	memset(sAIDName, NULL, nMBSLength + 1);
	
	// Convert to MBS
	WideCharToMultiByte(CP_UTF8, 0, wsAIDName, -1, sAIDName, nMBSLength, NULL, NULL);
	sAIDName[nMBSLength] = NULL;
	strResult.Format("%s", sAIDName);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Log
	int i=0;
	CString strBuff(""), strBuffList("");
	for (i=0; i < nLen/2; i++)
	{
		strBuff.Format("%04X",  wsAIDName[i]);
		strBuffList += strBuff;
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_WCStoMBS - WCHAR   : [%s]", strBuffList);
	
	strBuff = "";
	strBuffList = "";
	for (i=0; i < nMBSLength; i++)
	{
		strBuff.Format("%02X",  (BYTE) sAIDName[i]);
		strBuffList += strBuff;
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_WCStoMBS - CHAR    : [%s]", strBuffList);
	
	strBuff = "";
	strBuffList = "";
	for (i=0; i < strResult.GetLength(); i++)
	{
		strBuff.Format("%02X",  (BYTE) strResult.GetAt(i));
		strBuffList += strBuff;
	} 
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_WCStoMBS - CString : [%s]", strBuffList);	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	delete [] sAIDName;
	delete [] wsAIDName;
	
	return strResult;
}


CString CTranCmn::fnAPDLib_MBStoWCS(LPCSTR pAIDName)
//wstring CTranCmn::fnAPDLib_MBStoWCS(LPCSTR pAIDName)
{
	CString strReturn = "";

	// Get Length in WCS
	int nLength = MultiByteToWideChar(CP_UTF8, 0, pAIDName, -1, NULL, 0);
	WCHAR *wsDisplayText = new WCHAR[nLength+1];

	wmemset(wsDisplayText, NULL, nLength+1);
	
	// Convert to WCS
	MultiByteToWideChar(CP_UTF8, 0, pAIDName, -1, wsDisplayText, nLength);
	wsDisplayText[nLength] = NULL;

	wstring wstrReturn(wsDisplayText);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Log
	int i=0;
	CString strBuff(""), strBuffList("");
	for (i=0; i < nLength; i++)
	{
		strBuff.Format("%04X",  wsDisplayText[i]);
		strBuffList += strBuff;
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPDLib_MBStoWCS - WCHAR   : [%s]", strBuffList);
	
	strBuff = "";
	strBuffList = "";
	for (i=0; i < (int)wstrReturn.length(); i++)
	{
		strBuff.Format("%04X",  wstrReturn[i]);
		strBuffList += strBuff;
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPDLib_MBStoWCS - wstring : [%s]", strBuffList);	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	delete [] wsDisplayText;

	strReturn = (wstrReturn.c_str());

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPDLib_MBStoWCS - strReturn : [%s]", strReturn);	

	return strReturn;
}


/*==============================================================================
* Function Name : fnAPDLib_EMVSendMakeProc()
* Description   : EMV �ŷ��� ���� �¶��� �۽� ��ó�� �Լ�
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			:  
* ============================================================================*/
int CTranCmn::fnAPDLib_EMVSendMakeProc(int nEmvType)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPDLib_EMVSendMakeProc()");	

	switch(nEmvType)
	{
	case 1:
		m_pDevCmn->fnEMVLib_MakeEMVTagData("TRANSACTION");
		m_pDevCmn->fnEMV_GetHostTranData(EMV_SENDHOST_ARQC);
		break;
	case 2:
		m_pDevCmn->fnEMVLib_MakeEMVTagData("REVERSAL");
		m_pDevCmn->fnEMV_GetHostTranData(EMV_SENDHOST_REVERSAL);
		break;
	default:
		break;
	}


	return T_OK; //#N0228
}


int CTranCmn::fnAPD_PBMPrintTest()				//#N0211 
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PBMPrintTest()");
	ULONG  WiatTime = TimerSet(K_60_WAIT);
	CString			strOmCommand("0");	strOmCommand.Empty();
	CStringArray	strOmCommandArray;

	fnAPD_PbEnDisable(ENABLE, TRUE);							// ����� ��Ʈ�� ����

	while (TRUE)
	{
		if ((m_pDevCmn->fnAPL_GetDeviceEvent(DEV_PBM))	&&
			(DevEnDisable & DEV_PBM)					)
		{
			m_pDevCmn->fnLGT_SetFlicker(PBM_FLICKER, FLICKER_OFF);
			break;
		}

		if (CheckTimer(WiatTime))
		{
			fnAPD_PbEnDisable(DISABLE);
			m_pDevCmn->fnLGT_SetFlicker(PBM_FLICKER, FLICKER_OFF);
			return T_TIMEOVER;
		}

		Delay_Msg();
	}

	if (fnAPP_TestPPPProc())
		return T_ERROR;

	memset(&PPPRecvData, 0, sizeof(PPPRecvData));

	if (!m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))		// ���ۿϷ����&��ְ���
		return T_ERROR;


	if (m_pDevCmn->fnPBM_GetMaterialInfo())					// ��������
	{
		if(m_pDevCmn->AtmDefine.MachineType == U3100K)
		{
			m_pDevCmn->fnPBM_Eject();
		}
		else
		if(m_pDevCmn->AtmDefine.MachineType == U8100)
		{
			m_pDevCmn->fnPBM_Retract();
		}
		if (!m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))	// ���ۿϷ����&��ְ���
			return T_ERROR;

		fnAPD_MaterialRetract(DEV_PBM);						// ��������ȸ��
		if (!m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))	// ���ۿϷ����&��ְ���
			return T_ERROR;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PBMPrintTest():Return(...)");
	return T_OK;
}

// ����������������������������������������������������������������������������

// TranDevProc.cpp End
////////////////////////////////////////////////////////////////////////


// Hi-Pass���˽��������� #N0266 
// �����н�ī�� �ŷ�������Ʈ üũ (���˽�)
int CTranCmn::fnAPD_HPSVerifyTranBit()
{
	int nLockFlag = FALSE;
	int nReturn = 0;


	m_pDevCmn->fnICA_SelectApplet(TRAN_IC_HIPASS);
	fnAPD_CheckDeviceAction(DEV_MCU);
	nReturn = m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_SELECTAPPLICATION);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HPS_PROC_SELECTAPPLICATION [%d]", nReturn);
	switch (nReturn)
	{
		case 0:											// ����
			CancelHIPassLoadFlg = FALSE;
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HPS_PROC_SELECTAPPLICATION____1");
			break;
		case 1:											// �����н� Applet �̹߰�
			fnAPP_CancelProc(T_MSG, "�����н�ī�常 �ŷ��� �����մϴ�");
			break;
		case 29:										// Lock����ī��			
			CancelHIPassLoadFlg = TRUE;
			nLockFlag = TRUE;
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HPS_PROC_SELECTAPPLICATION____2");
			break;
		default:										// ���
			nLockFlag = TRUE;
			CancelHIPassLoadFlg = TRUE;
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HPS_PROC_SELECTAPPLICATION____3");
			//fnAPP_CancelProc(T_MSG, "�����н�ī�� �����Դϴ�. �ٽ� �õ��Ͽ� �ֽʽÿ�.", FCErrMsg[nReturn].ICErrGuide);
			break;
	}

	return T_OK;
}

// �����н�ī�� �ܾ���ȸ (���˽�)
int CTranCmn::fnAPD_HPSReadBalance(int nFlag)
{
	m_pDevCmn->fnHPS_ReadBalance();								// �ܾ���ȸ
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_READBALANCE))
		fnAPP_CancelProc(T_MSG, "ī��(Read Balance)�� �д��߿�", "������ �߻��Ͽ����ϴ�");

	if (nFlag)													// �ܾ׸���ȸ
		return T_OK;

	m_pDevCmn->fnHPS_ReadPan();									// ī���ȣ��ȸ
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSREADPAN))
		fnAPP_CancelProc(T_MSG, "ī��(Read PAN)�� �д��߿�", "������ �߻��Ͽ����ϴ�");

	return T_OK;
}

// �����н�ī�� ����1�ܰ� (���˽�)
int CTranCmn::fnAPD_HPSPrepareLoad(char *pAmount)
{
	BYTE	LoadReqAmount[4] = {NULL, };

	Asc2IntHighOrder(pAmount, LoadReqAmount, 8, 4);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_HPSPrepareLoad()", LoadReqAmount, 4);
//	m_pDevCmn->fnHPS_PrepareLoadIEP(MakeUnPack(LoadReqAmount, 4));
	m_pDevCmn->fnHPS_PrepareLoadIEP(LoadReqAmount);
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSPRELOAD))
	{
		m_pDevCmn->fnSCR_DisplayString(1, "�����н� �����ŷ�(S1)�� �����Ͽ����ϴ�");
		m_pDevCmn->fnSCR_DisplayString(2, "��Ȳ�Ƿ� �����Ͽ� �ֽʽÿ�");
		m_pDevCmn->fnSCR_DisplayScreen(701);
		Delay_Msg(1 * 1000);

		return T_ERROR;
	}

	return T_OK;;
}

// �����н�ī�� ����2�ܰ� (���˽�)
int CTranCmn::fnAPD_HPSLoad(BYTE *pRN, BYTE *pSign2)
{
	BYTE cbTemp[128]	 = {NULL, };
	BYTE cbSignData[128] = {NULL, };

	memcpy(&cbTemp[0],	pRN,	16);
	memcpy(&cbTemp[16],	pSign2, 8);
	MakePack(cbTemp, cbSignData, 24);	

//	m_pDevCmn->fnHPS_LoadIEP(GetSprintf("%16.16s", pRN)			// ȣ��Ʈ����
//						+	 GetSprintf("%8.8s",   pSign2));	// ����2
	m_pDevCmn->fnHPS_LoadIEP(cbSignData);						// ȣ��Ʈ���� + ����2
	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_LoadIEP", cbSignData, sizeof(cbSignData));
		fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSLOAD))
	{
		m_pDevCmn->fnSCR_DisplayString(1, "�����н� �����ŷ�(S3)�� �����Ͽ����ϴ�.");
		m_pDevCmn->fnSCR_DisplayString(2, "â���� �����Ͽ� �ֽʽÿ�");
		m_pDevCmn->fnSCR_DisplayScreen(701);
		Delay_Msg(1 * 1000);

		return T_ERROR;
	}

	return T_OK;
}

// �����н�ī�� �����Ϸ�(���)
int CTranCmn::fnAPD_HPSComplete()
{
	m_pDevCmn->fnHPS_CompleteTran();
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSCOMPLETE))
		return T_ERROR;

	return T_OK;
}

// ����������������������������������������������������������������������������

