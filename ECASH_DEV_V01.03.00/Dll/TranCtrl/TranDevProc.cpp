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
//	매체함수
/////////////////////////////////////////////////////////////////////////////

// CheckDeviceAction 처리시 이상처리시 추가확인부
#define		__MATOUT_RETRY_SUPPORT__							// 매체재배출추가처리

// 동작완료검지&장애검지
int CTranCmn::fnAPD_CheckDeviceAction(int ChkID, int ErrChkFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeviceAction(ChkID[%x], ErrChkFlg[%d])", ChkID, ErrChkFlg);

	int		nErrorDevice = DEV_NONE;

	// #NICCM01	ICCM 모듈 적용
	if ((m_pDevCmn->IfmDirect)						&&
		(ChkID & DEV_MCU)							&&
		(m_pDevCmn->AtmDefine.CashHandler == HBRM_SET))
	{
		m_pDevCmn->fnFIC_CheckIFMError(m_pDevCmn->IfmResult);
	}


	ChkID &= m_pDevCmn->fnAPL_GetAvailDevice(ChkID);			// 사용가능한장치구하기
	m_pDevCmn->fnAPL_CheckDeviceAction(ChkID);					// 동작대기및장애검지
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
		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(ChkID))		// 사용하고장애난장치구하기
			fnAPP_CancelProc(T_ERROR);
#endif
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeviceAction(...):return");
	return T_OK;
}

// 카드흡입모드설정
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
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_MCU, ENABLE);	// 장치투입모드설정
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

// IC카드흡입모드설정
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
			m_pDevCmn->fnMCU_ICEntryEnable();					// 장치투입모드설정
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


// 통장흡입모드설정
int CTranCmn::fnAPD_PbEnDisable(int Action, int InitFlg, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbEnDisable(Action[%d], InitFlg[%d], WaitFlg[%d])", Action, InitFlg, WaitFlg);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	if(m_pDevCmn->PBBarLineReading)								// 통장바코드 읽는 부분 뒤에서 처리 하기 위해
		return T_OK;

	if (Action == ENABLE)
	{
		if ((InitFlg) ||
			(!(DevEnDisable & DEV_PBM)))
		{
			DevEnDisable |= DEV_PBM;
			fnAPD_CheckDeviceAction(DEV_PBM);
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PBM, ENABLE);	// 장치투입모드설정
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

// 카드읽기&체크
int CTranCmn::fnAPD_CardReadCheck(LPCTSTR strInCardData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck()");

	CString			strCardInfo("");
	CString			strCardData("");
	CStringArray	strCardDataArray;
	int				i = 0;

	if (!strInCardData)											// 자료지정무
	{
		if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
			return T_OK;
	}

/////////////////////////////////////////////////////////////////////////////
	// V03-01-01 카드처리시 SPL LED처리지원(SCDP통합지원)
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_CARD_MAT, TRUE);
/////////////////////////////////////////////////////////////////////////////
		
	do 
	{
		CardTran = MENU_NULL;									// Card거래상태변수
		ICCardTran = MENU_NULL;									// Card거래상태변수
		CardBank = 0;											// 은행카드종류
		CardService = 0;										// 카드종류
		CardNew = 0;											// 신규카드종류
		memset(&CardData, 0, sizeof(CardData));					// 초기화
		strCardDataArray.RemoveAll();

		if (!strInCardData)										// 자료지정무
		{
			fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnMCU_Read();
			fnAPD_CheckDeviceAction(DEV_MCU);
			strCardData = m_pDevCmn->fstrMCU_GetCardData();		// 카드자료구하기
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
if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)                       // 보안상
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:CardData", &CardData, sizeof(CardData));

		if (!strInCardData)										// 자료지정무
		{
			if (!(CardData.TrInfo & (ISO2_TRACK | ISO3_TRACK)))
			{
/////////////////////////////////////////////////////////////////////////////
				CString		strTempDbData("");
				CString		strTemp("");

				// 미디어Read에러
				// OM_MEDIAREADERR||
				strTempDbData = "OM_MEDIAREADERR";

				// 장애발생일(YYYYMMDD)||
				strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
				strTempDbData += "||" + strTemp;
				
				// 장애발생시간(HHMMSS)||
				strTemp.Format("%6.6s", GetTime().GetBuffer(0));
				strTempDbData += "||" + strTemp;
				
				// 매체구분(0-카드,1-통장,2-수표,3-IC)
				strTempDbData += "||0";
				
				m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// 전자저널자료전송
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

		if (CardData.TrInfo & ISO3_TRACK)						// ISO3 Track 정보가있는경우
		{
			if ((CardData.ISO3size < 104)								||
				(CardData.ISO3size > sizeof(CardData.ISO3Buff))			||
				(FindChar(CardData.ISO3Buff, CardData.ISO3size, NULL) != CardData.ISO3size)
																		||
				(!IsUnPack(CardData.ISO3Buff, CardData.ISO3size))		||
				(!IsNum(pCardData3->BankNo, 3))							||
//				(!IsZero(pCardData3->BankNo, 1))				// 은행이아닌경우문제발생 : 2003.10.03
				(IsZero(pCardData3->BankNo, 3))							||
//				(IsZero(pCardData3->AccountNo, 16))						||	// 동양증권카드 포맷문제발생
				(IsZero(CardData.ISO3Buff, 104))						||
				(IsChar(CardData.ISO3Buff, 104, '=')))
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:TrInfo1[%d]", pCardData3->OtherData[7]);
			}
			else
			if(memcmp(pCardData3->AccountNo, NINE16, 16) == 0)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "은행 : 비정상 <**:%s>",NINE16);
				CardTran |= MENU_T3_NOT;
			}
			else
			if(IsZero(pCardData3->AccountNo, 16))
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "은행 : 비정상 <**:%s>",ZERO16);
				CardTran |= MENU_T3_NOT;
			}
			else
			{
		
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:TrInfo2[%d]", pCardData3->OtherData[7]);

				if (memcmp(pCardData3->FormatCode, BANKID, 4) == 0 ||
					memcmp(pCardData3->FormatCode, STOCKID, 4) == 0)
				{												// 은행카드

					// 2009.06.11 변경
					// 산업은행	
					if (memcmp(pCardData3->BankNo, KDBBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = KDBBANK;					
					}
																// 기업은행
					if (memcmp(pCardData3->BankNo, KIUBANKID,  3) == 0)
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == KIUP)
							CardTran |= MENU_J3;	
						else
							CardTran |= MENU_T3;	

						CardBank = KIUBANK;		
					}		
																// 외환은행 
					if (memcmp(pCardData3->BankNo, KOEBANKID,  3) == 0)
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == HANA || m_pDevCmn->AtmDefine.ETCDevice20 == KEB)		//#N0246
							CardTran |= MENU_J3;	
						else
							CardTran |= MENU_T3;
						
						CardBank = KOEBANK;					
		
					}
																// 수협 
					if (memcmp(pCardData3->BankNo, SUHBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SUHBANK;					
					}
																// 조흥은행 
																// 제일은행
					if (memcmp(pCardData3->BankNo, KOFBANKID,  3) == 0)
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == SC)		//#N0219
							CardTran |= MENU_J3;
						else
							CardTran |= MENU_T3;						
						CardBank = KOFBANK;					
					}
																// 대구은행 
					if (memcmp(pCardData3->BankNo, DEABANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = DEABANK;					
					}
																// 부산은행 
					if (memcmp(pCardData3->BankNo, PUSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = PUSBANK;					
					}
																// 광주은행 
					if (memcmp(pCardData3->BankNo, KAWBANKID,  3) == 0)
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == KWANGJU)   
							CardTran |= MENU_J3;	
						else
							CardTran |= MENU_T3;
					
						CardBank = KAWBANK;					
						CardBankFlag = KAWBANK;
					}
																// 제주은행 
					if (memcmp(pCardData3->BankNo, JEJBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = JEJBANK;					
					}
																// 전북은행 
					if (memcmp(pCardData3->BankNo, JEOBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = JEOBANK;					
					}
																// 경남은행 
					if (memcmp(pCardData3->BankNo, KYOBANKID,  3) == 0)
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == KYONGNAM)   //#N0192
							CardTran |= MENU_J3;
						else
							CardTran |= MENU_T3;					
						CardBank = KYOBANK;					
					}
																// 새마을금고  
					if (memcmp(pCardData3->BankNo, KFCBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = KFCBANK;					
					}
																// 신협 
					if (memcmp(pCardData3->BankNo, SINBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SINBANK;					
					}
																// 상호저축 					
					if (memcmp(pCardData3->BankNo, SANBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SANBANK;					
					}
																// 씨티은행 
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
					
																// 신한은행, 조흥은행		
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
																 // 국민은행
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
																 // 농협 
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
																 // 우리은행 
					if ((memcmp(pCardData3->BankNo, WORBANKID1, 3) == 0)	||
						(memcmp(pCardData3->BankNo, WORBANKID2, 3) == 0)	||
						(memcmp(pCardData3->BankNo, WORBANKID3, 3) == 0)	||
						(memcmp(pCardData3->BankNo, WORBANKID4, 3) == 0))
					{										
						CardTran |= MENU_T3;					
						CardBank = Asc2Int(pCardData3->BankNo,3);					
					}
																 // 하나은행 
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
																 // 한미은행 
					if ((memcmp(pCardData3->BankNo, GOOBANKID1, 3) == 0)	||
						(memcmp(pCardData3->BankNo, GOOBANKID2, 3) == 0))
					{
						if(m_pDevCmn->AtmDefine.ETCDevice20 == CITI)		//#N0238
							CardTran |= MENU_J3;
						else
							CardTran |= MENU_T3;					
						CardBank = Asc2Int(pCardData3->BankNo,3);					
					}
																// 우체국
					if ((memcmp(pCardData3->BankNo, POSBANKID1, 3) == 0)	||
						(memcmp(pCardData3->BankNo, POSBANKID2, 3) == 0)    ||
						(memcmp(pCardData3->BankNo, POSBANKID3, 3) == 0)    ||
						(memcmp(pCardData3->BankNo, POSBANKID4, 3) == 0)    ||
						(memcmp(pCardData3->BankNo, POSBANKID5, 3) == 0))
					{										
						CardTran |= MENU_T3;					
						CardBank = Asc2Int(pCardData3->BankNo,3);					
					}

																// 동양종합금융증권
					if(memcmp(pCardData3->BankNo, TYJSBANKID,  3) == 0)// || CardBankFlag == MYASSET)
					{
						CardTran |= MENU_T3;					
						CardBank = TYJSBANK;					
						CardBankFlag = MYASSET;	
					}
																// 현대증권
					if(memcmp(pCardData3->BankNo, HYDSBANKID,  3) == 0)// || CardBankFlag == HYDSSTOCK)
					{
						CardTran |= MENU_T3;					
						CardBank = HYDSBANK;
						CardBankFlag = HYDSSTOCK;				
					}
																// 미래에셋증권
					if(memcmp(pCardData3->BankNo, MRASBANKID,  3) == 0)// || CardBankFlag == MRASSTOCK)
					{
						CardTran |= MENU_T3;					
						CardBank = MRASBANK;	
						CardBankFlag = MRASSTOCK;	
						
					}
																// 대우증권
					if(memcmp(pCardData3->BankNo, DAWSBANKID,  3) == 0)// || CardBankFlag == DAEWOO)
					{
						CardTran |= MENU_T3;					
						CardBank = DAWSBANK;					
						CardBankFlag = DAEWOO;
					}
																// 삼성증권
					if(memcmp(pCardData3->BankNo, SASSBANKID,  3) == 0)// || CardBankFlag == SAMSSTOCK)
					{
						CardTran |= MENU_T3;					
						CardBank = SASSBANK;					
						CardBankFlag = SAMSSTOCK;
					}
																// 우리투자증권
					if(memcmp(pCardData3->BankNo, WRTSBANKID,  3) == 0)// || CardBankFlag == WOORISTOCK)
					{
						CardTran |= MENU_T3;					
						CardBank = WRTSBANK;					
						CardBankFlag = WOORISTOCK;
					}
																// 한국투자증권
					if(memcmp(pCardData3->BankNo, HKTSBANKID,  3) == 0)// || CardBankFlag == HKTSSTOCK)
					{
						CardTran |= MENU_T3;					
						CardBank = HKTSBANK;					
						CardBankFlag = HKTSSTOCK;
					}
																// 교보증권
					if(memcmp(pCardData3->BankNo, KYBSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = KYBSBANK;					
					}
																// 하이투자증권
					if(memcmp(pCardData3->BankNo, HISBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = HISBANK;					
					}
																// 에이치엠씨투자증권
					if(memcmp(pCardData3->BankNo, HMCSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = HMCSBANK;					
					}
																// 키움증권
					if(memcmp(pCardData3->BankNo, KIWSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = KIWSBANK;					
					}
																// 이트레이드증권
					if(memcmp(pCardData3->BankNo, ETRSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = ETRSBANK;					
					}
																// 에스케이증권
					if(memcmp(pCardData3->BankNo, SKSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SKSBANK;					
					}
																// 대신증권
					if(memcmp(pCardData3->BankNo, DASSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = DASSBANK;		
						CardBankFlag = DASSSTOCK;	
						
					}
																// 솔로몬투자증권
					if(memcmp(pCardData3->BankNo, SOLSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SOLSBANK;					
					}
																// 한화증권
					if(memcmp(pCardData3->BankNo, HAHSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = HAHSBANK;					
						CardBankFlag = HAHSSTOCK;
					}
																// 하나대투증권
					if(memcmp(pCardData3->BankNo, HNDSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = HNDSBANK;					
						CardBankFlag = HANASTOCK;
					}
																// 굿모닝신한증권
					if(memcmp(pCardData3->BankNo, GOMSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = GOMSBANK;	
						CardBankFlag = SHHSSTOCK;					
					}
																// 동부증권
					if(memcmp(pCardData3->BankNo, DOBSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = DOBSBANK;					
					}
																// 유진투자증권
					if(memcmp(pCardData3->BankNo, EUGSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = EUGSBANK;					
					}
																// 메리트중권
					if(memcmp(pCardData3->BankNo, MRISBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = MRISBANK;					
						CardBankFlag = MERITZ;
					}
																// 엔에이치투자증권
					if(memcmp(pCardData3->BankNo, NHTSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = NHTSBANK;					
					}
																// 부국증권
					if(memcmp(pCardData3->BankNo, BOKSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = BOKSBANK;					
					}
																// 신영증권
					if(memcmp(pCardData3->BankNo, SIYSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = SIYSBANK;					
					}
																// 엘아이지투자증권
					if(memcmp(pCardData3->BankNo, LIGSBANKID,  3) == 0)
					{
						CardTran |= MENU_T3;					
						CardBank = LIGSBANK;					
					}					

					if (CardBank == 0)								// 해당사항무
					{
						CardTran |= MENU_NULL;						// 타행은행(3TR)
						CardBank = OTHBANK;							// 기타카드
					}
					else
					{
						memcpy(Accept.AccountNum, &pCardData3->AccountNo[0], 16);
					}
				}
			}	
		
		}



		if (CardData.TrInfo & ISO2_TRACK)						// ISO2 Track 정보가있는경우
		{		
			if ((CardData.ISO2size < 12)								||
				(CardData.ISO2size > sizeof(CardData.ISO2Buff))			||
				(FindChar(CardData.ISO2Buff, CardData.ISO2size, NULL) != CardData.ISO2size)
																		||
				(!IsUnPack(CardData.ISO2Buff, CardData.ISO2size))		||
				(!IsNum(CardData.ISO2Buff, 12))							||	// 2003.12.12 : 2트랙 Numeric Check
				(IsZero(CardData.ISO2Buff, 12))							||
				(IsChar(CardData.ISO2Buff, 12, '=')))
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "신용카드 : 비정상 <**:%2.2s>",CardData.ISO2Buff);
			}
			else
			if(memcmp(CardData.ISO2Buff, NOT_TRACK2, 4) == 0)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "신용카드 : 비정상 <**:%s>","체크카드");
				CardTran |= MENU_T2_NOT;
			}
			else
			{
				CardTran |= MENU_J2T2;


				strCardInfo = fnAPP_Get_CreditOrgCode(pCardData2->MembershipNo);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "신용카드 : [%s]-[%s]", strCardInfo, fnAPP_Get_CreditName(pCardData2->MembershipNo));
	
				CardBank = Asc2Int(strCardInfo);

				BYTE ServiceCode = '0';
				CardOnEMV = FALSE;
				
				if ('=' == CardData.ISO2Buff[16])				// 일반
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
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("카드     : 사용 불가 카드임"));

		if (!strInCardData)										// 자료지정무
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			{
				fnAPP_CancelProc(T_MSG, "이 카드는 사용 할 수 없습니다");
			}
			else 
				fnAPP_CancelProc(T_MSG, "This card is not valid");
		}
		else
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			{
				if (MenuIrdaMode)
					fnAPP_CancelProc(T_MSG, "이 휴대폰은 사용 할 수 없습니다");
				else
				if (MenuAvail & MENU_IC)
				{
					if ((TranCode == TC_KCLOAD		)||			// 충전
						(TranCode == TC_KCUNLOAD	)||			// 환불
						(TranCode == TC_KCREINQ		)||			// 잔액조회
						(TranCode == TC_KCBUYLST	)||			// 구매내역조회
						(TranCode == TC_KCLOADRCV	)||			// 충전복구
						(TranCode == TC_KCUNLOADRCV	)||			// 환불복구
						(TranCode == TC_KCPARAKEY	)||			// 파라미터갱신(Key)
						(TranCode == TC_KCPARALIMIT ))			// 파라미터갱신(최대저장한도)
						fnAPP_CancelProc(T_MSG, "이 전자화폐카드는 사용 할 수 없습니다");
					else
					{
						fnAPP_CancelProc(T_MSG, "이 카드는 사용 할 수 없습니다");
					}
				}
				else
				{
					fnAPP_CancelProc(T_MSG, "이 카드는 사용 할 수 없습니다");
				}
			}
			else 
				fnAPP_CancelProc(T_CANCEL);
		}
	}

	
	strCardInfo.Empty();
	if (CardTran & MENU_MCU2)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("카드     : CREDIT READ2")); 

		strCardInfo.Format("%s",    (CardTran & MENU_J2)			? "자행2" : "타행2");
		if (MenuIrdaMode)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][핸드폰][%s]", strCardInfo.GetBuffer(0));
		else
		if (MenuFICMode)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][금융IC][%s]", strCardInfo.GetBuffer(0));
		else
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][카드][%s]", strCardInfo.GetBuffer(0));
	}
	if (CardTran & MENU_MCU3)
	{	
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("카드     : BANK READ3")); 

		if(m_pDevCmn->AtmDefine.ETCDevice20 == KEB)
		{
			if (CardTran & MENU_T3)
			{
				fnAPP_MenuClearTranProc(0, TRAN_EASYONE, MENU_NULL);  //#N0174 타행카드 입력시 이지원 버튼 삭제
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
				fnAPP_MenuClearTranProc(0, TRAN_EASYONE, MENU_NULL);  //#N0174 타행카드 입력시 이지원 버튼 삭제
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
		strCardInfo.Format("%s",    (CardTran & MENU_J3)			? "자행3" :
									(CardTran & MENU_S3)			? "자행3" :
									(CardTran & MENU_J2T2)			? "신용카드" :
									(CardTran & MENU_IC)			? "자행C" : "타행3");
		strCardInfo.Format("%s%s",  strCardInfo.GetBuffer(0),
									(CardBankFlag == MYASSET)		? "유안타증권" :			
									(CardBankFlag == DAEWOO)		? "대우증권" : 
									(CardBankFlag == WOORISTOCK)	? "우리증권" : 
									(CardBankFlag == SAMSSTOCK)		? "삼성증권" : 
									(CardBankFlag == MERITZ)		? "메리츠증권" : 
									(CardBankFlag == MRASSTOCK)		? "미래에셋증권" : 
									(CardBankFlag == HANASTOCK)		? "하나투자증권" : 
									(CardBankFlag == MRASSTOCK)		? "미래에셋증권" : 
									(CardBankFlag == DASSSTOCK)		? "대신투자증권" : 
									(CardBankFlag == HYDSSTOCK)		? "현대증권" : 
									(CardBankFlag == JOINCARD)		? "제휴" : "기타"); 
		strCardInfo.Format("%s카드",strCardInfo.GetBuffer(0));
		if (MenuIrdaMode)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][핸드폰][%s]", strCardInfo.GetBuffer(0));
		else
		if (MenuFICMode)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][금융IC][%s]", strCardInfo.GetBuffer(0));
		else
		if (MenuEPBMode)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][전자통장][%s]", strCardInfo.GetBuffer(0));
		else
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][카드][%s]", strCardInfo.GetBuffer(0));

		if (CardNew == NEWCARD)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][카드][신규발급카드]");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck():return");
	return T_OK;
}


// IC카드읽기&체크
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
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("IC카드", "인식실패 (ICChipInitialize[%d])", nRet));	
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
			fnAPP_CancelProc(T_MSG, "IC 카드 정보 읽기 오류입니다", "처음부터 거래를 다시 시작하여 주시기 바랍니다");
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

	CardPowerOnFlg = TRUE;										// 카드접속유(전자화폐)


	m_pDevCmn->FICCardType = FIC_OPEN;							// 개방형 설정

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

// 통장읽기&체크 -> 
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
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : PASSBOOK READ FAIL")); 
		fnAPP_CancelProc(T_MSG, "이 통장은 사용 할 수 없습니다");
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

	// 통장거래 Total 건수를 위해 통장 Read시 무조건 Cnt 진행
	if (memcmp(szTempReadPbmNomalCnt, "9999", 4) == 0)	
		memset(szTempReadPbmNomalCnt, '0', 4);
	AddString(szTempReadPbmNomalCnt, "0001", 4);
	// 통장 Read 데이터 기록 (Total 건수)
	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.ReadPbmNomalCnt, szTempReadPbmNomalCnt, 4);
	m_pProfile->TRANS.ReadPbmNomalCnt[4] = NULL;
	m_pProfile->PutTransProfile();
	//<-- end of V07-00-01-#11
*/
	do 
	{
		PbTran = MENU_NULL;										// Pb거래상태변수
		memset(&PbMsData, 0, sizeof(PbMsData));					// 통장자료
		memset(&PbBarData, 0, sizeof(PbBarData));				// 통장페이지자료

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// 통장Ms자료구하기

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_NONGHYUP_PbReadCheck:PbMsData", &PbMsData.sBuf, PbMsData.Len);

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// 읽지못함
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// 미디어Read에러
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// 장애발생일(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// 장애발생시간(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// 매체구분(0-카드,1-통장,2-수표,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// 전자저널자료전송
/////////////////////////////////////////////////////////////////////////////
			nPbReadFailed = TRUE;

			break;
		}
				
		if ((PbMsData.Len != NAC_ACCNUM_SIZE1) &&
 			(PbMsData.Len != NAC_ACCNUM_SIZE2))					// 통장데이타길이체크 : 농협(54 or 50)
 		{
			nPbReadFailed = TRUE;
 			PbTran = MENU_NULL;
 		}		
		else		
		if (PbMsData.Len > sizeof(PbMsData.sBuf))				// 데이타길이 확인
		{
			PbTran = MENU_NULL;
		}
		else
		if (!IsNum(PbMsData.sBuf, 14))							// 계좌번호자리(MAX 14)
		{
			PbTran = MENU_NULL;
		}
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// 데이타검증
		{
			PbTran = MENU_NULL;
		}
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// 데이타검증
		{
			PbTran = MENU_NULL;
		}
		else
		if (!memcmp(&PbMsData.sBuf[41], "01", 2) == 0 &&				// 중앙통장?
			!memcmp(&PbMsData.sBuf[41], "10", 2) == 0 &&				// 조합통장?
			!memcmp(&PbMsData.sBuf[48], "10", 2) == 0)				// V04-01-01-#01 
		{
			PbTran = MENU_NULL;
		}
/*
		else //#0074
		if (!memcmp(&PbMsData.sBuf[42], "1", 1) == 0 &&				// 중앙통장?
			!memcmp(&PbMsData.sBuf[41], "1", 1) == 0 &&				// 조합통장?
			!memcmp(&PbMsData.sBuf[43], "1", 1) == 0 &&				// 중앙/조합통장?
			!memcmp(&PbMsData.sBuf[48], "1", 1) == 0)			    // 신통장

		{
			PbTran = MENU_NULL;
		}
*/		else													// ※검증이상무※
		{
			PbTran |= MENU_PB;
			PbBank  = NONBANK;
			CardBank = NONBANK;	
			//#0047
			//memcpy(Accept.AccountNum, pPbMsData->AccountNo, 13);
			if (memcmp(&PbMsData.sBuf[42], "1", 1) == 0) 				// 중앙통장 : 113계좌 : 5-2-6
			{
				memcpy(Accept.AccountNum, &PbMsData.sBuf[1], 13);
			}
			else
			if (memcmp(&PbMsData.sBuf[41], "1", 1) == 0) 				// 조합통장 : 14계좌 : 6-2-6		
			{
				memcpy(Accept.AccountNum, &PbMsData.sBuf[0], 14);
			}
			else
			if (memcmp(&PbMsData.sBuf[43], "1", 1) == 0) 				// 조합통장 : 14계좌 : 6-3-6		
			{
				memcpy(Accept.AccountNum, &PbMsData.sBuf[0], 15);

			}
			else
			if (memcmp(&PbMsData.sBuf[48], "1", 1) == 0 && memcmp(&PbMsData.sBuf[41], "000", 3) ==0)				// 조합통장 : 14계좌 : 6-2-6		
			{
				memcpy(Accept.AccountNum, &PbMsData.sBuf[0], 13);
			}
			else
				memcpy(Accept.AccountNum, &PbMsData.sBuf[1], 13);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_NONGHYUP_PbReadCheck(PbBank[%d])", PbBank);
//////////////////////////////////////////////////////////////////////////
// Bar Line 읽기 : 농협 YSH 2003.05.18
// ※ 통장 페이지 정보 SET 사양
//    1 Page : '11'
//    2 Page : '12'
//    3 Page : '13'
//    4 Page : '14'
//    5 Page : '15'
//    6 Page : '01'
//    7 Page : '02'
//    8 Page : '03'		9페이지통장지원
//    9 Page : '04'		9페이지통장지원
//    처음 또는 마지막페이지 : 방출처리
//////////////////////////////////////////////////////////////////////////
			fnAPD_CheckDeviceAction(DEV_PBM);
			m_pDevCmn->fnPBM_BarLineRead();						// 페이지정보읽기
			m_pDevCmn->PBBarLineReading = TRUE;					// 통장 바코드 읽는 부분 뒤로 이동.

// 			fnAPD_CheckDeviceAction(DEV_PBM);
// 			strBarData = m_pDevCmn->fstrPBM_GetBarData();
// 			PbBarData.Len = __min(sizeof(PbBarData.sBuf) * 2, strBarData.GetLength()) / 2;
// 			MakePack(strBarData.GetBuffer(0), PbBarData.sBuf, __min(sizeof(PbBarData.sBuf) * 2, strBarData.GetLength()));
// MsgDumpAnal(TRACE_CODE_MODE, "Log", "[페이지정보 = %s]", &PbBarData.sBuf[0]);
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
// 				PbTran &= ~(MENU_PB);							// 페이지정보이상
// 			}
//////////////////////////////////////////////////////////////////////////
		}
	} while(FALSE);
	
	if (!PbTran)
	{

		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			// 통장 Read실패시 안내문구 변경
			if (nPbReadFailed == TRUE)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : PASSBOOK READ FAIL")); //#KIMH0143
				fnAPP_CancelProc(T_MSG, "이 통장은 읽을 수 없습니다", "직원에게 문의하여 주십시오");
			}
			else
			if (PbBarData.sBuf[0] == '0')						// 인자용페이지아님
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 인자용 페이지 미인식")); //#KIMH0143
				fnAPP_CancelProc(T_MSG, "인쇄할 페이지를 찾을 수 없습니다", "통장의 인자면을 확인하신 후", "다시 거래하여 주십시오");
			}			
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 사용 불가 통장임")); //#KIMH0143
				fnAPP_CancelProc(T_MSG, "이 통장은 사용 할 수 없습니다", "직원에게 문의하여 주십시오");
			}
		}
		else 
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : PASSBOOK READ FAIL")); //#KIMH0143

			if (PbBarData.sBuf[0] == '0')						// 인자용페이지아님
				fnAPP_CancelProc(T_MSG, "Please check the page where to print", "and try again.");
			else
				fnAPP_CancelProc(T_MSG, "This passbook is not valid", "Please check at a bank counter");
		}
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_NONGHYUP_PbReadCheck(PbBank[%d]):return", PbBank);
	return T_OK;
}


int CTranCmn::fnAPD_KJB_PbReadCheck()	// #N0181  광주 통장체크
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
		PbTran = MENU_NULL;										// Pb거래상태변수
		memset(&PbMsData, 0, sizeof(PbMsData));					// 통장자료

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// 통장Ms자료구하기

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// 읽지못함
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// 미디어Read에러
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// 장애발생일(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// 장애발생시간(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// 매체구분(0-카드,1-통장,2-수표,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// 전자저널자료전송
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if ((PbMsData.sBuf[0] < '0')	||
			(PbMsData.sBuf[0] > '9')	)
			break;
		
		if (PbMsData.Len > sizeof(PbMsData.sBuf))			// 데이타길이 확인
			PbTran = MENU_NULL;
		else
		if (!IsNum(&PbMsData.sBuf[1], 14))		        	// 계좌번호자리
			PbTran = MENU_NULL;
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// 데이타검증
			PbTran = MENU_NULL;
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// 데이타검증
			PbTran = MENU_NULL;


		if (IsNum(PbMsData.sBuf, PbMsData.Len) &&
           !memcmp(&PbMsData.sBuf[3], KAWBANKID, 3))                             // 통장M/S Read시 은행번호 Check : 자행만 거래가능
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
// Bar Line 읽기
// ※ 통장 페이지 정보 SET 사양
//    1 Page : '01'
//    2 Page : '02'
//    3 Page : '03'
//    4 Page : '04'
//    5 Page : '05'
//    6 Page : '06'
//    7 Page : '07'
//    8 Page : '08'
//    9 Page : '09'
//    처음 또는 마지막페이지 : 방출처리
//////////////////////////////////////////////////////////////////////////
		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_BarLineRead();						// 페이지정보읽기
		m_pDevCmn->PBBarLineReading = TRUE;					// 통장 바코드 읽는 부분 뒤로 이동.

	} while(FALSE);
	

	if (!PbTran)	
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "이 통장은 사용 할 수 없습니다");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}

	MsgDumpAnal(TRACE_CODE_MODE, "APL", "[MATERIAL][통장][%s통장]",
									(PbKind == PASSBOOK_NORMAL)		 ? "일반" : "기타");
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_DGB_PbReadCheck(PbKind[%d]):return", PbKind);
	return T_OK;
}


int CTranCmn::fnAPD_HANA_PbReadCheck()		//#N0214
{
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck()");

	CString	strPbData("");
	MS		PbMsTemp1;
	// 카드 데이타 변경 처리 V03-00-09 2003.05.26
	char	TempPbNum = (m_pProfile->TRANS.YYYYMMDD[7] & 0x0f);
	int		i = 0;

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	do 
	{
		PbTran = MENU_NULL;										// Pb거래상태변수
		memset(&PbMsData, 0, sizeof(PbMsData));					// 통장자료
//		StockPB = 0;											// 통장종류(1:수익증권)		// V01.00.30_#3
//		FundPB  = 0;											// 펀드통장					// V06-33-00-#01

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// 통장Ms자료구하기

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck():return=[%d]",PbMsData.Len);

if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)				// 보안상		
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		int BankCheckPb = PbMsData.Len;
//		memcpy(&PbMsData, &PbMsData, sizeof(PbMsData));

		memcpy(&PbMsTemp1, &PbMsData, sizeof(PbMsData));

		for (i=0; i < PbMsData.Len;i++)
		{
			PbMsTemp1.sBuf[i] += TempPbNum;
		}

		if (((BankCheckPb != 36) && ((BankCheckPb != 18) )) ||		// 통장
		    (PbMsData.Len < 14)						||			// 통장
			(PbMsData.Len > sizeof(PbMsData.sBuf))	||
			(!IsNum(&PbMsData.sBuf[0], 14))			||			// 계좌번호자리
			(IsZero(PbMsData.sBuf, 14))				||
			(IsChar(PbMsData.sBuf, 14, '=')))
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "통장확인1");
		else 
		{														// V01.00.30_#3
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "통장확인2");

			if (BankCheckPb == 18)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "하나통장");
				if (memcmp(&PbMsData.sBuf[12], "13", 2) == 0)	// V01.00.30_#3
				{
//					StockPB = 1;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck():투입된 통장의 종류 수익증권[StockPB]");
				}

				if (memcmp(&PbMsData.sBuf[12], "12", 2) == 0)	// V06-33-00-#01
				{
//					FundPB = 1;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck():투입된 통장의 종류 펀드통장[FUNDPB]");
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
			fnAPP_CancelProc(T_MSG, "이 통장은 사용 할 수 없습니다");
		else													// ENG001 KYD
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck():return");

	return T_OK;
}



/////////////////////////////////////////////////////////////////////////////
static char* Gwamok_01[] =		//적립식과목
{"03", "07", "08", "13", "14", "21", "22", "23", "26", "27", "48", "53", "56"};

static char* Gwamok_02[] =		//신탁과목 - 24과목 추가(해외펀드)
{"43", "45", "49", "51", "52", "54", "55", "57", "58", "24", "68"};

static char* Gwamok_03[] =		//요구불과목
{"01", "02", "06", "25", "37", "18", "41", "42"};

static char* Gwamok_04[] =		//대출과목
{"09", "16", "29", "38", "44", "59"};				// V01-19-00-#02:대출"38"과목 추가

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
	BYTE	KMBCheck[2] = { 0x2d, 0x2e };						// "=>" window 값
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
		PbTran = MENU_NULL;										// Pb거래상태변수
		memset(&PbMsData, 0, sizeof(PbMsData));					// 통장자료

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// 통장Ms자료구하기

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)                       // 보안상
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// 읽지못함
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// 미디어Read에러
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// 장애발생일(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// 장애발생시간(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// 매체구분(0-카드,1-통장,2-수표,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// 전자저널자료전송
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if ((PbMsData.Len < 14)						||			// 통장
			(PbMsData.Len > sizeof(PbMsData.sBuf))	||
//			(!IsNum(&PbMsData.sBuf[0], 14))			||			// 계좌번호자리 (통합)
			(IsZero(PbMsData.sBuf, 14))				||
			(IsChar(PbMsData.sBuf, 14, '=')))
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_1");
			PbTran = MENU_NULL;
		}
		else
		{
			if ((PbMsData.Len == 17) || (PbMsData.Len == 18)) 	// 구국민 (17테스트통장:18) 																
			{
				PbTran = MENU_NULL; //브랜드 미 사용(사용불가 통장임)
				if (memcmp(&PbMsData.sBuf[12], KMBCheck, 2) == 0)
				{												// => (2D 2E)확인	
					KbTongBank = 2;

					for(int i=0; i < itemof(ChangeValue); i++) //// 과목변환
					{
						if(memcmp(ChangeValue[i], &PbMsData.sBuf[3], 2) == 0)			//
						{
							memcpy(ChangeGwamok, &ChangeValue[i][3], 2);		// 과목적용
						}
					}

					KbGwamok = fnAPD_GwamokCheck_Kb(ChangeGwamok);			// 과목체크 ??

					// 차세대
					memcpy(Accept.AccountNum, PbMsData.sBuf, 12);

					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_2");
				}
				else											// 17,18중 "=>" 없으면 reject
					KbGwamok = 0;
			}
			else 
			if ((PbMsData.Len == 39)							&& 
				(memcmp(&PbMsData.sBuf[36], "-", 1) == 0)		&&	// '='
				((memcmp(&PbMsData.sBuf[18], "06", 2) == 0) || //KUMBANKID2
				 (memcmp(&PbMsData.sBuf[18], "04", 2) == 0)))  //KUMBANKID1
			{
				// 타업체와 코드 맞춤
				// '+' -> ';'
				if (PbMsData.sBuf[35] == 0x2b)  // '+' -> 테스트 통장, ';' -> 리얼통장
					PbMsData.sBuf[35] = 0x3b;

				// '-' -> '='
				if (PbMsData.sBuf[36] == 0x2d)   // '-' -> 테스트 통장, '=' -> 리얼통장 
					PbMsData.sBuf[36] = 0x3d;

				if (memcmp(&PbMsData.sBuf[18], "04", 2) == 0)// 구국민 재발급통장
				{

					if (PbMsData.sBuf[21] == '1')				// 고객지정계좌
					{
						KbTongBank = 4;								
						KbGwamok = 3;									// 요구불
						memcpy(Accept.AccountNum, PbMsData.sBuf, 11);
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_1 Gwamok (%d) TongBank(%d) &PbMsData.sBuf[3](%3.3s) ChangeGwamok(%3.3s)", KbGwamok, KbTongBank, &PbMsData.sBuf[3], ChangeGwamok);

					}
					else
					{
						KbTongBank = 2;

						for(int i=0; i < itemof(ChangeValue); i++) //// 과목변환
						{
							if(memcmp(ChangeValue[i], &PbMsData.sBuf[3], 2) == 0)			//
							{
								memcpy(ChangeGwamok, &ChangeValue[i][3], 2);		// 과목적용
							}
						}
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_2 Gwamok (%d) TongBank(%d) &PbMsData.sBuf[3](%3.3s) ChangeGwamok(%3.3s)", KbGwamok, KbTongBank, &PbMsData.sBuf[3], ChangeGwamok);
						KbGwamok = fnAPD_GwamokCheck_Kb(ChangeGwamok);	 // 과목체크 ??
						memcpy(Accept.AccountNum, PbMsData.sBuf, 12);
					}
				}
				else											// 
				{
					KbTongBank = 3;								// 주택통합통장
					KbGwamok = fnAPD_GwamokCheck_Kb((char *)&PbMsData.sBuf[4]);	// 과목체크
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
				PbTran = MENU_NULL; //브랜드 미 사용(사용불가 통장임)

				// 타업체와 코드 맞춤
				// '+' -> ';'
				if (PbMsData.sBuf[35] == 0x2b)
					PbMsData.sBuf[35] = 0x3b;

				// '-' -> '='
				if (PbMsData.sBuf[36] == 0x2d)
					PbMsData.sBuf[36] = 0x3d;


				KbTongBank = 1;									// 구주택
				KbGwamok = fnAPD_GwamokCheck_Kb((char *)&PbMsData.sBuf[4]);		// 과목체크

				// 차세대
				memcpy(Accept.AccountNum, PbMsData.sBuf, 14);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_4 Gwamok (%d) TongBank(%d)", KbGwamok, KbTongBank);
			}

			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_5 Gwamok (%d) TongBank(%d)", KbGwamok, KbTongBank);
		}

	} while(FALSE);


	if ((PbMsData.Len == 18) && (PbMsData.sBuf[17] == 0x2b))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_6 Gwamok (%d) TongBank(%d)", KbGwamok, KbTongBank);
		PbTran = NULL;											// 구국민테스트통장거부
	}

	if (m_pDevCmn->BrmCashMode == BRM_REALCASH_MODE)
	{
		if ((PbMsData.Len == 39)						&& 
			(memcmp(&PbMsData.sBuf[36], "-", 1) == 0)	&&
			(PbMsData.sBuf[35] == 0x2b))						// 통합테스트통장
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

	if (!KbGwamok)												// 과목을체크하지 못한경우
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck()_8 Gwamok (%d) TongBank(%d)", KbGwamok, KbTongBank);
		PbTran = NULL;
	}
	


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][통장][%s통장]", 
											(KbTongBank== 1)		 ? "구주택"		:
											(KbTongBank== 2)		 ? "구국민"		:
											(KbTongBank== 3)		 ? "통합국민"	: "기타");
	if (!PbTran)	
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 사용 불가 ")); //#KIMH0143
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "해당기기는 이 통장을 사용 할 수 없습니다");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}

	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KB_PbReadCheck(PbKind[%d]):return", PbKind);
	return T_OK;
}


int	CTranCmn::fnAPD_GwamokCheck_Kb(char * InGwamok)						// 통장과목체크 (주택)
{
	int		i;

	for(i = 0 ; i < itemof(Gwamok_01); i++ )
	{
		if(memcmp(Gwamok_01[i], InGwamok, 2) == 0)				// 적립식과목
		{
			return 1;
		}
	}

	for(i = 0 ; i < itemof(Gwamok_02); i++ )
	{
		if(memcmp(Gwamok_02[i], InGwamok, 2) == 0)				// 신탁과목
		{
			return 2;
		}
	}

	for(i = 0 ; i < itemof(Gwamok_03); i++ )
	{
		if(memcmp(Gwamok_03[i], InGwamok, 2) == 0)				// 요구불과목
		{
			return 3;
		}
	}

	for(i = 0 ; i < itemof(Gwamok_04); i++ )
	{
		if(memcmp(Gwamok_04[i], InGwamok, 2) == 0)				// 대출과목
		{
			return 4;
		}
	}

	// V01-28-00-#01:퇴직연금통장
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
		PbTran = MENU_NULL;										// Pb거래상태변수
		memset(&PbMsData, 0, sizeof(PbMsData));					// 통장자료

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// 통장Ms자료구하기

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// 읽지못함
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// 미디어Read에러
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// 장애발생일(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// 장애발생시간(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// 매체구분(0-카드,1-통장,2-수표,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// 전자저널자료전송
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if ((PbMsData.sBuf[0] < '0')	||
			(PbMsData.sBuf[0] > '9')	)
			break;
		
		if (PbMsData.Len > sizeof(PbMsData.sBuf))			// 데이타길이 확인
			PbTran = MENU_NULL;
		else
		if (!IsNum(&PbMsData.sBuf[1], 14))		        	// 계좌번호자리
			PbTran = MENU_NULL;
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// 데이타검증
			PbTran = MENU_NULL;
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// 데이타검증
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
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 사용 불가 ")); //#KIMH0143
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "이 통장은 사용 할 수 없습니다");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}
	else
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장", "통장정상 (구분:씨티(%d), 계좌:확인안됨)", PbBank));			// #0379

	MsgDumpAnal(TRACE_CODE_MODE, "APL", "[MATERIAL][통장][%s통장]",
									(PbKind == PASSBOOK_NORMAL)		 ? "일반" : "기타");
	
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
	// V03-01-01 카드처리시 SPL LED처리지원(SCDP통합지원)
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, TRUE);

/////////////////////////////////////////////////////////////////////////////

	do 
	{
		PbTran = MENU_NULL;										// Pb거래상태변수
		memset(&PbMsData, 0, sizeof(PbMsData));					// 통장자료

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// 통장Ms자료구하기

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)                       // 보안상
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// 읽지못함
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// 미디어Read에러
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// 장애발생일(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// 장애발생시간(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// 매체구분(0-카드,1-통장,2-수표,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// 전자저널자료전송
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if (((PbMsData.Len != 25) && (PbMsData.Len != 13))			||			// 통장
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
			fnAPP_CancelProc(T_MSG, "이 통장은 사용 할 수 없습니다");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}

	if (fnAPD_PBBarCodeRead() == FALSE)				// BarCode 읽기 정상, 인쇄 불가 PAGE 
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "BarCode 읽기 정상, 인쇄 불가 PAGE PBM Return 0 !!!");
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PB Read OK 통장 방출 (바코드 있어 통장 방출)!!!");
		PbTran = MENU_NULL;	
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranDevProc PbTran = MENU_NULL !!!");
		// 2010.10.14 소스 리뷰 관련 수정  (MS Read 실패, 및 문구 수정)
		// 2010.10.14 전산부 검수시 수정사항 fnAPP_CancelProc 거래가 취소 되었습니다. 문구 삭제 (정성훈씨)
		fnAPP_CancelProc(T_MSG, "통장의 인쇄할 면을 바르게 펴서 넣어주십시오");
	}
	else											// BarCode 읽기 정상, 인쇄 가능 PAGE 
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "BarCode 읽기 실패, 인쇄 가능 PAGE PBM Return 1 !!!");
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PB Read ERROR  (바코드 없어 통장 거래진행)!!!");
	}
	
	MsgDumpAnal(TRACE_FREE_MODE, "AP", "[MATERIAL][통장][%s통장]",
									(PbKind == PASSBOOK_LOAN)	? "대출" : 
									(PbKind == PASSBOOK_NORMAL)	? "일반" : "기타");
	
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
		PbTran = MENU_NULL;										// Pb거래상태변수
		memset(&PbMsData, 0, sizeof(PbMsData));					// 통장자료

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// 통장Ms자료구하기

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// 읽지못함
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// 미디어Read에러
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// 장애발생일(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// 장애발생시간(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// 매체구분(0-카드,1-통장,2-수표,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// 전자저널자료전송
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if (IsNull(PbMsData.sBuf, PbMsData.Len))		       // 계좌번호자리
		{
			PbTran = MENU_NULL;
			break;
		}

		//if ((PbMsData.sBuf[0] < '0')	||
		//	(PbMsData.sBuf[0] > '9')	)
		//	break;
		
		if (PbMsData.Len > sizeof(PbMsData.sBuf))			// 데이타길이 확인
			PbTran = MENU_NULL;
		//else
		//if (!IsNum(&PbMsData.sBuf[1], 14))		        	// 계좌번호자리
		//	PbTran = MENU_NULL;
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// 데이타검증
			PbTran = MENU_NULL;
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// 데이타검증
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

		// 신통장은 계좌가 13자리 임.
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
			fnAPP_CancelProc(T_MSG, "이 통장은 사용 할 수 없습니다");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}
	else
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장", "통장정상 (구분:경남(%d), 계좌:확인안됨)", PbBank));

	MsgDumpAnal(TRACE_CODE_MODE, "APL", "[MATERIAL][통장][%s통장]",
									(PbKind == PASSBOOK_NORMAL)		 ? "일반" : "기타");
	
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
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbSpeedUpCheck()[페이지 정보 = %1.1s]", &PbBarData.sBuf[0]);

		if (m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)			// 농협 9페이지통장지원
		{
			//if (PbBarData.sBuf[0] >= '1' && PbBarData.sBuf[0] <= '7')
			if (PbBarData.sBuf[0] >= '1' && PbBarData.sBuf[0] <= '9')	// 9페이지통장지원
			{
				//if ((PbBarData.sBuf[0] == '6') || (PbBarData.sBuf[0] == '7'))
				if ((PbBarData.sBuf[0] == '6') ||		// 1
					(PbBarData.sBuf[0] == '7') ||		// 2
					(PbBarData.sBuf[0] == '8') ||		// 3			// 9페이지통장지원
					(PbBarData.sBuf[0] == '9')		)	// 4			// 9페이지통장지원
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
				//PbTran &= ~(MENU_PB);							// 페이지정보이상 ==> #0079
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
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 통장 인쇄면 데이터 인식 불가")); //#KIMH0143
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "통장 인쇄면 데이터를 확인하지 못하였습니다.", "다시 거래하여 주십시오");
		}
		else 
		{
			if (PbBarData.sBuf[0] == '0')						// 인자용페이지아님
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
* Description   : 통장바코드 리드 
* Parameters    : (NONE)
* Return        : T_OK
* Release       : Ver 1.0
* Notes			: 2010.09.03 // V03-19-03-#01 통장바코드 업무지원
* ============================================================================*/
int CTranCmn::fnAPD_PBBarCodeRead()   //#N0245
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PBBarCodeRead() IN");
	
	BOOL	bIsPbBarCode = FALSE;
	CString strPbPage("");
	char	PbBarCodeTemp[10] = {0, };
	//			H/W			PGU(CIS센서)
	//	T2ATM	L-PBM-E		유(바코드유)
	//	GATM  	L-PBM-E		유(바코드유)
	//	U8100 	I-PBM		유(바코드유)
	//	T1ATM	L-PBM+		바코드 및 PAGE 이월 업무 없음
	
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
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "♣♣♣fnAPD_PBBarCodeRead [%s]♣♣♣", strPbPage);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "♣♣♣fnAPD_PBBarCodeRead [%s]♣♣♣", PbBarCodeTemp);
	//	301E BAR가 있는 첫장과 막장은 0(0x30) 페이지
	//	3110 기타 BAR가 없는 페이지는 1(0x31) 보고
	// - 바코드 리드시 정상 경우 
	// [2010/09/15 20:29:55:562 TranDevProc.cpp(07262)] ♣♣♣fnAPD_PBBarCodeRead [301E]♣♣♣
	// - 바코드 리드시 없을 경우 
	// [2010/09/15 20:30:51:417 TranDevProc.cpp(07263)] ♣♣♣fnAPD_PBBarCodeRead [3110]♣♣♣
	// 	1. V00.02.20_002
	// 		1) BASE : V00.02.10 (상판 OPEN 장애 6001 대응 시범운영 판)
	// 		2) 변경 내역
	// 		- KFB BAR 지원
	// 		: BAR가 있는 첫장과 막장은 0(0x30) 페이지 보고
	// 		: 기타 BAR가 없는 페이지는 1(0x31) 보고
	// 		- BAR RETRY 없이 1회만 READ
	// 		- KFB USER SETTING 수정 : BAR 추가
	
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
		PbTran = MENU_NULL;										// Pb거래상태변수
		memset(&PbMsData, 0, sizeof(PbMsData));					// 통장자료

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// 통장Ms자료구하기

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// 읽지못함
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// 미디어Read에러
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// 장애발생일(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// 장애발생시간(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// 매체구분(0-카드,1-통장,2-수표,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// 전자저널자료전송
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
		
		if (PbMsData.Len > sizeof(PbMsData.sBuf))			// 데이타길이 확인
			PbTran = MENU_NULL;
		else
		if (!IsNum(&PbMsData.sBuf[1], 14))		        	// 계좌번호자리
			PbTran = MENU_NULL;
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// 데이타검증
			PbTran = MENU_NULL;
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// 데이타검증
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

				if (memcmp(&PbMsData.sBuf[9], "96", 2) == 0)	// 수익증권계좌
					PbKind = PASSBOOK_BENEFICERTI;
				else
				if (memcmp(&PbMsData.sBuf[9], "94", 2) == 0)	// 퇴직연금통장
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
				if ((PbMsData.sBuf[9] == '3') && (PbMsData.sBuf[10] >= '1' && PbMsData.sBuf[10] <= '9') ) // 과목 31~39
					  PbKind = PASSBOOK_LOAN;                                                                                                              // 대출통장
				else 
				if (memcmp(&PbMsData.sBuf[9], "40", 2) == 0)                                     // 과목 40
					  PbKind = PASSBOOK_LOAN;                                                                                                              // 대출통장
				else 
				if (memcmp(&PbMsData.sBuf[9], "56", 2) == 0)                                     // 과목 56 - 외화통장
					  PbKind = PASSBOOK_FOREIGN;                                                                                                          // 외화통장
				else
					  PbTran = MENU_NULL;

			}
			else
			{
				PbTran = MENU_NULL;
			}
		}
		else
		if (i == 32)											// 신통장
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
				if (memcmp(&PbMsData.sBuf[9], "96", 2) == 0)	// 수익증권계좌
					PbKind = PASSBOOK_BENEFICERTI;
				else
				if (memcmp(&PbMsData.sBuf[9], "94", 2) == 0)	// 퇴직연금통장
					PbKind = PASSBOOK_RETIRE;
			}
			else
			{
				PbTran = MENU_NULL;
			}
		}

	} while(FALSE);
	

	if (PbKind == PASSBOOK_LOAN)							// 대출통장
	{
		if (TranCode != TC_PBOOK)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 통장정리만 가능")); //#KIMH0143
			fnAPP_CancelProc(T_MSG, "이 통장으로는 통장정리만 가능합니다");
		}
	}
	else
	if (PbKind == PASSBOOK_BENEFICERTI)						// 수익증권통장
	{
		if ((TranCode != TC_DEPOSIT) && (TranCode != TC_PBOOK))    // 입금 - 통장정리만 거래가능
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 통장 거래 불가")); //#KIMH0143
			fnAPP_CancelProc(T_MSG, "이 통장으로는 본거래를 할 수 없습니다");
		}
	}
	else
	if (PbKind == PASSBOOK_FOREIGN)	//#0034				
	{
		if (TranCode != TC_PBOOK)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 통장정리만 가능")); //#KIMH0143
			fnAPP_CancelProc(T_MSG, "이 통장으로는 통장정리만 가능합니다");
		}
	}
	else
	if (PbKind == PASSBOOK_RETIRE)							// 퇴직연금통장 //#0202 -> 내수 사양와 다름. //#0037
	{
		if (TranCode != TC_PBOOK)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 통장정리만 가능")); //#KIMH0143
			fnAPP_CancelProc(T_MSG, "이 통장으로는 통장정리만 가능합니다");
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
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : MS 재 수록 요청")); //#KIMH0143
			fnAPP_CancelProc(T_MSG, "기업은행 창구에서 통장정리 후", "MS 재수록 거래 하십시오");
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MATERIAL][통장][%s통장]",
									(PbKind == PASSBOOK_LOAN)		 ? "대출" :
									(PbKind == PASSBOOK_BENEFICERTI) ? "수익" :
									(PbKind == PASSBOOK_NORMAL)		 ? "일반" : "기타");
	
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
		PbTran = MENU_NULL;										// Pb거래상태변수
		memset(&PbMsData, 0, sizeof(PbMsData));					// 통장자료

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();				// 통장Ms자료구하기

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE)                       // 보안상
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// 읽지못함
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// 미디어Read에러
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// 장애발생일(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// 장애발생시간(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// 매체구분(0-카드,1-통장,2-수표,3-IC)
			strTempDbData += "||1";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// 전자저널자료전송
/////////////////////////////////////////////////////////////////////////////
			break;
		}

		if (PbMsData.Len > sizeof(PbMsData.sBuf))				// 데이타길이 확인
			PbTran = MENU_NULL;
		else
		if (!IsNum(pPbMsData->AccountNo, PbMsData.Len))			// 계좌번호자리
			PbTran = MENU_NULL;
		else
		if (IsZero(PbMsData.sBuf, PbMsData.Len))				// 데이타검증
			PbTran = MENU_NULL;
		else
		if (IsChar(PbMsData.sBuf, PbMsData.Len, '='))			// 데이타검증
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

		// 외환은행 통장 과목 체크 
		if (PbMsData.Len == 36)					// 외환은행 통장의 경우  -> #0096
		{
			if ((memcmp(&pPbMsData->AccountNo[4], "739", 3) == 0)	||	// 거래불가 통장
				(memcmp(&pPbMsData->AccountNo[4], "809", 3) == 0)	||
				(memcmp(&pPbMsData->AccountNo[4], "747", 3) == 0))
				PbTran = MENU_NULL;
			else
			if ((pPbMsData->AccountNo[7] == '9')	&&				// 거래불가 계좌 (계좌의 4번째 5번째가 9와 0인 경우)
				(pPbMsData->AccountNo[8] == '0'))
				PbTran = MENU_NULL;
			else
			if (PbMsData.sBuf[2] == '4') //신통장 - SystemID
			{

				if (m_pDevCmn->HostSvrPortMode == ECASH_LOCALPORT_MODE)
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Local Mode SystemID(%c), MSType(%c), PBM Type(%c)", PbMsData.sBuf[2], PbMsData.sBuf[3], PbMsData.sBuf[19]);

					if (PbMsData.sBuf[19] == '1') //통장발급구분 (Real)
					{
						nTestPbmflag = 0;
					}
					else
					if (PbMsData.sBuf[19] == '0') //통장발급구분 (Test)
					{
						nTestPbmflag = 1;
					}
				}
				else
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Real Mode SystemID(%c), MSType(%c), PBM Type(%c)", PbMsData.sBuf[2], PbMsData.sBuf[3], PbMsData.sBuf[19]);

					if (PbMsData.sBuf[19] == '1') //통장발급구분 (Real)
					{
						nTestPbmflag = 0;
					}
					else
					if (PbMsData.sBuf[19] == '0') //통장발급구분 (Test)
					{
						nTestPbmflag = 1;
					}
				}

	
				if (m_pDevCmn->HostSvrPortMode == ECASH_LOCALPORT_MODE)
				{
					if (nTestPbmflag == FALSE) //Real Pbm
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TEST 서버 REAL 통장 사용 불가"));
						fnAPP_CancelProc(T_MSG, "본 기기에서는 사용이 불가한 통장입니다", "청호이지캐쉬(주)으로 문의 바랍니다");
					}
				}
				else
				{
					if (nTestPbmflag == TRUE) //TEST Pbm
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("REAL 서버 TEST 통장 사용 불가"));
						fnAPP_CancelProc(T_MSG, "본 기기에서는 사용이 불가한 통장입니다", "청호이지캐쉬(주)으로 문의 바랍니다");
					}
				}

				CardBankFlag = KOEBANK;
				CardBank = KOEBANK;	
				PbTran |= MENU_PB;		
				PbBank = KOEBANK;
				memcpy(Accept.AccountNum, &PbMsData.sBuf[4], 13);

			}
			else
			if (PbMsData.sBuf[2] == '9') //구통장 - SystemID //#0096
			{
				if (PbMsData.sBuf[3] == '9' || PbMsData.sBuf[3] == '8') //MsType - 9,8
				{
					if (m_pDevCmn->HostSvrPortMode == ECASH_LOCALPORT_MODE)
					{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Local Mode SystemID(%c), MSType(%c), PBM Type(%c)", PbMsData.sBuf[2], PbMsData.sBuf[3], PbMsData.sBuf[17]);

						if (PbMsData.sBuf[17] == '1') //통장발급구분 (Real)
						{
							nTestPbmflag = 0;
						}
						else
						if (PbMsData.sBuf[17] == '0') //통장발급구분 (Test)
						{
							nTestPbmflag = 1;
						}
					}
					else
					{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Real Mode SystemID(%c), MSType(%c), PBM Type(%c)", PbMsData.sBuf[2], PbMsData.sBuf[3], PbMsData.sBuf[17]);

						if (PbMsData.sBuf[17] == '1') //통장발급구분 (Real)
						{
							nTestPbmflag = 0;
						}
						else
						if (PbMsData.sBuf[17] == '0') //통장발급구분 (Test)
						{
							nTestPbmflag = 1;
						}
					}
					
					if (m_pDevCmn->HostSvrPortMode == ECASH_LOCALPORT_MODE)
					{
						if (nTestPbmflag == FALSE) //Real Pbm
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TEST 서버 REAL 통장 사용 불가"));
							fnAPP_CancelProc(T_MSG, "본 기기에서는 사용이 불가한 통장입니다", "청호이지캐쉬(주)으로 문의 바랍니다");
						}
					}
					else
					{
						if (nTestPbmflag == TRUE) //TEST Pbm
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("REAL 서버 TEST 통장 사용 불가"));
							fnAPP_CancelProc(T_MSG, "본 기기에서는 사용이 불가한 통장입니다", "청호이지캐쉬(주)으로 문의 바랍니다");
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
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 사용 불가 ")); //#KIMH0143

		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			fnAPP_CancelProc(T_MSG, "이 통장은 사용 할 수 없습니다");
		}
		else 
		{
			fnAPP_CancelProc(T_MSG, "This passbook is not valid");
		}
	}

	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_KEB_PbReadCheck(PbKind[%d]):return", PbKind);
	return T_OK;
}


// 외환은행 통장 
#define	PB_DEMANDACC			101						// 요구불/저축성/신탁
#define PB_CREDITACC			102						// 여신통장
#define PB_FOREINEX				103						// 외환통장
#define PB_FOREINEXMB			104						// 외환 머니백 통장 
#define PB_SAVINGBILL			105						// 보관어음 (NCR52바이트)
#define PB_TRUST				106						// 신탁통장

#define PB_NEXT_CREDITACC		111						// 차세대 여신
#define PB_NEXT_FOREINEX		112						// 차세대 외국환,보관어음 
#define PB_NEXT_FOREINEXMB		113						// 차세대 머니백

#define PB_HNB_DEMAND			201						// 하나은행 요구불 통장
#define PB_HNB_STOCK			202						// 하나은행 펀트 통장
// 통장계좌정보구하기
int CTranCmn::fnAPD_KEB_GetPbAccount(BYTE* pbSendAccount)
{
	int nPbAccLen = 0;
	PbKind = 0;

	// 하나은행 통장
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

	// '1' : 요구불, '2': 저축성, '3':여신,'4':외국환,'5':신탁, '6':머니백, '4' : 보관어음
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_GetPbAccount: SystemID(%c), MSType(%c)", PbMsData.sBuf[2], PbMsData.sBuf[3]);

	if (PbMsData.sBuf[2] == '9')		// 현행
	{
		switch (PbMsData.sBuf[3])
		{
		case '1':				// 요구불
		case '7':				// 신요구불				
			PbKind = PB_DEMANDACC;
			nPbAccLen = 11;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
			break;
 
		case '2':				// 저축성
			PbKind = PB_DEMANDACC;
			nPbAccLen = 11;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
				break;

		case '3':				// 여신
			PbKind = PB_CREDITACC;
			nPbAccLen = 12;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
			break;

		case '4':				// 수신(외국환,보관어음)
		case '8':				// 신외화				
			if (PbMsData.Len < 52)
			{
				PbKind = PB_FOREINEX;
				nPbAccLen = 12;

				// 구계좌 외화통장
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
			else				// 보관어음
			{
				PbKind = PB_SAVINGBILL;
				nPbAccLen = 11;
				memcpy(Accept.AccountNum, &PbMsData.sBuf[7], nPbAccLen);
			}
			break;

		case '5':				// 신탁
				PbKind = PB_TRUST;
				nPbAccLen = 11;
				memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
				break;
		
		case '6':				// 머니백
				PbKind = PB_FOREINEXMB;
				nPbAccLen = 13;
				memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
				break;
		}
	}
	else
	if (PbMsData.sBuf[2] == '4')		// 차세대
	{
		switch(PbMsData.sBuf[3])
		{
		case '1':				// 수신
			PbKind = PB_NEXT_FOREINEX;
			nPbAccLen = 12;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
			break;

		case '3':				// 여신	
			PbKind = PB_NEXT_CREDITACC;
			nPbAccLen = 15;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
			break;

		case '4':				// 보관어음.
			PbKind = PB_NEXT_CREDITACC;
			nPbAccLen = 12;
			memcpy(Accept.AccountNum, &PbMsData.sBuf[4], nPbAccLen);
			break;

		case '6':				// 머니백
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

// 통장계좌정보구하기
int CTranCmn::fnAPD_GetPbAccount(BYTE* pbSendAccount)
{
	BYTE MakeTranAccount[16];
	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
	{
		if ((PbMsData.Len == NAC_ACCNUM_SIZE1) ||
			(PbMsData.Len == NAC_ACCNUM_SIZE2))			// 농협통장데이터길이체크
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
	2009-01-09 : 농협 신규 통장 Format 수용
	[1] MS DATA 41 ~ 43 의 값이 "000" 이며 48 값이 "1" 이면 신규 통장
	[2] 신규 통장일 경우 유효성 코드는 48번째 자리로 구분한다.
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
			  // MS에 은행코드가 존재하는 경우...
			  // 중앙회 통장(Pattern1)인 경우 (유효성코드의 위치가 [43] 1자리 (배열 : 42)이므로...)
			  // 유효성코드 위치에 "0" SET한다(통장 사용 불능 처리)
			  memcpy(&szPbReadMsData[42], "0", 1);
			  MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Pattern1 Passbook Unavailable Code Set : 0");
		}
		else 
		if (!memcmp(&szPbReadMsData[34], "12", 2))
		{
			  // MS에 은행코드가 존재하는 경우...
			  // 조합 통장(Pattern2)인 경우 (유효성코드의 위치가 [42] 1자리 (배열 : 41)이므로...)
			  // 유효성코드 위치에 "0" SET한다(통장 사용 불능 처리)
			  memcpy(&szPbReadMsData[41], "0", 1);
			  MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Pattern2 Passbook Unavailable Code Set : 0");
		}
		else 
		{
			  // (구)농협통장인 경우 MS에 은행코드가 존재하지 않으므로...유효성코드 위치로 SET 수행
			  if (!memcmp(&szPbReadMsData[42], "1", 1))
			  {
					 // 중앙회 통장(Pattern1)인 경우 (유효성코드의 위치가 [43] 1자리 (배열 : 42)이므로...)
					 // 유효성코드 위치에 "0" SET한다(통장 사용 불능 처리)
					 memcpy(&szPbReadMsData[42], "0", 1);
					 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Unavailable Pattern1 Passbook Code Set : Success");
			  }
			  else 
			  if (!memcmp(&szPbReadMsData[41], "1", 1))
			  {
					 // 조합 통장(Pattern2)인 경우 (유효성코드의 위치가 [42] 1자리 (배열 : 41)이므로...)
					 // 유효성코드 위치에 "0" SET한다(통장 사용 불능 처리)
					 memcpy(&szPbReadMsData[41], "0", 1);
					 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Unavailable Pattern2 Passbook Code Set : Success");
			  }
			  else 
			  if (!memcmp(&szPbReadMsData[43], "1", 1)) //#0074
			  {
					 // 조합 통장(Pattern2)인 경우 (유효성코드의 위치가 [43] 1자리 (배열 : 41)이므로...)
					 // 유효성코드 위치에 "0" SET한다(통장 사용 불능 처리)
					 memcpy(&szPbReadMsData[43], "0", 1);
					 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Unavailable Pattern2 Passbook Code Set : Success");
			  }
			  else 
			  {
					 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ProcessPPR] Invalid Pattern1 or Pattern2 Passbook Code Position");
			  }
		}
	}

	m_pDevCmn->fnSCR_DisplayString(2, "죄송합니다!");
	m_pDevCmn->fnSCR_DisplayString(3, "일시적인 장애로 인하여 처리가 안되었습니다 ");
	m_pDevCmn->fnSCR_DisplayString(4, "가까운 지점을 방문하시어 통장정리 내용을 확인하시기 바랍니다");			//오타수정  #N0198
	m_pDevCmn->fnSCR_DisplayScreen(701);
	Delay_Msg(5000);	
	m_pDevCmn->fnPBM_MSWrite(szPbReadMsData);
	fnAPD_CheckDeviceAction(DEV_PBM);
	
	return T_OK;
}

// 카드엠보스스캔
int CTranCmn::fnAPD_CardEmbossScan(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardEmbossScan(WaitFlg[%d])", WaitFlg);

	if (MenuIrdaMode)											// 적외선거래
		return T_OK;
	if (MenuEMVMode)											// EMV카드
		return T_OK;

	if(m_pProfile->DEVICE.MachineType == U3100K || m_pProfile->DEVICE.MachineType == U8100)
		return T_OK;


	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_OK;

	if ((!CardTran)			||									// 카드유무체크
		(CardEmbossScanFlg))									// 카드엠보스스캔체크
		return T_OK;

	if (CardPowerOnFlg)
		CardPowerOnFlg = FALSE;									// 카드접속무
	CardEmbossScanFlg = TRUE;									// 카드엠보스스캔유무
	fnAPD_CheckDeviceAction(DEV_MCU);

	
	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_MCU);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardEmbossScan(...):return");
	return T_OK;
}

// 카드임프린트
int CTranCmn::fnAPD_CardImprint(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardImprint(WaitFlg[%d])", WaitFlg);

	if(m_pProfile->DEVICE.MachineType == U3100K || m_pProfile->DEVICE.MachineType == U8100)
		return T_OK;

	if (MenuIrdaMode)											// 적외선거래
		return T_OK;
	if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_JPR | DEV_MCU) != (DEV_JPR | DEV_MCU))
		return T_OK;
//	if (MenuFICMode)											// 금융IC카드	2004.02.03
//		return T_OK;
//	if (MenuAvail == MENU_IC)									// IC거래
//		return T_OK;

	if ((!CardTran)				||								// 카드유무체크
		(!CardEmbossScanFlg)	||								// 카드엠보스스캔무
		(CardImprintFlg))										// 카드임프린트체크
		return T_OK;
	
	if ((MenuFICMode)	||
		(MenuEPBMode)	||
		(MenuEMVMode)	||
		(MenuKCashMode)	)
	{
//		Delay_Msg(3000);
//		DWORD WaitTime = GetTickCount() + 5000;
		DWORD WaitTime = GetTickCount() + K_10_WAIT * 1000  ;				// 5초에서 10초로 늘림
		do {
			Delay_Msg();

			if (RegGetInt(_REGKEY_MCUINFO, "EMBOSSREADDONE"))
				break;
		} while(GetTickCount() < WaitTime);
	}

	CardImprintFlg = TRUE;										// 카드임프린트유무
	fnAPD_CheckDeviceAction(DEV_JPR | DEV_MCU);
	m_pDevCmn->fnJPR_EmbossPrint();								// 카드임프린트

	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_JPR);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardImprint(...):return");
	return T_OK;
}

// 카드기록 : 함수미사용
int CTranCmn::fnAPD_CardWrite(MCAP* WriteCardData, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardWrite(WriteCardData, WaitFlg[%d])", WaitFlg);
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardWrite:WriteCardData", WriteCardData, sizeof(MCAP));

	if (MenuIrdaMode)											// 적외선거래
		return T_OK;

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_OK;

	if (!CardTran)												// 카드유무체크
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_MCU);
//	m_pDevCmn->fnMCU_Write(MakeUnPack(WriteCardData->ISO3Buff, WriteCardData->ISO3size));
																// 기록(카드자료3TR)	: makeUnPak하면 안됨(장애발생됨)
	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_MCU);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardWrite(...):return");
	return T_OK;
}

// 통장인자
int	CTranCmn::fnAPD_PbPrint(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbPrint(WaitFlg[%d])", WaitFlg);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	if (!PbTran)												// 통장유무체크
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_PBM);
	m_pDevCmn->fnPBM_Print(MakeUnPack(PbData, PbDataCnt));		// 인자(인자자료)

	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_PBM);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbPrint(...):return");
	return T_OK;
}

// 전자저널INDEX정보설정
int	CTranCmn::fnAPD_EjrIndexSet()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EjrIndexSet()");

	char		szYYMMDD[9], szHHMMSS[13], szDummy0[1024];
	EjrIndexFlg = TRUE;											// 전자저널유
	memset(&EjrData, ' ', sizeof(EjrData));						// 전자저녈자료
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
			memcpy(EjrData.TranMaterial, "합계거래", 8);
			break;
		case TRANID_A020:		 
			memcpy(EjrData.TranMaterial, "상태전송", 8);
			break;
		case TRANID_A010:		 
			memcpy(EjrData.TranMaterial, "개국전송", 8);
			break;
		default:
			memcpy(EjrData.TranMaterial, "운영업무", 8);
			break;
		}

	}

/////////////////////////////////////////////////////////////////////////////
	// 거래통계정도 : 2003.12.15
	memset(&EjrAmountData, 0, sizeof(EjrAmountData));			// 전자저녈금액관련자료

	if (TranProc == TRAN_WITH)									// 지급
	{
		EjrAmountData.TranProc = TranProc;								// 입출금구분
		EjrAmountData.TranAmount = Asc2Int(Accept.Money, 15);			// 입출금거래금액
		if (m_pDevCmn->Withdraw50TFlag)										
		{
			EjrAmountData.Cash50000Num = Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND;	// 입출금5만원권매수
			EjrAmountData.Cash10000Num = Asc2Int(Accept.Cash10T_Money, 11);	// 입출금만원권매수
		}
		else																
			EjrAmountData.Cash10000Num = Asc2Int(Accept.CashMoney, 11);		// 입출금만원권매수

		EjrAmountData.MotherCheck10Num = Asc2Int(Accept.CheckMoney, 10);	// 입출금자행수표10만원권매수
	}

	if (TranProc == TRAN_DEP)									// 입급
	{
		EjrAmountData.TranProc = TranProc;						// 입출금구분
		EjrAmountData.TranAmount = Asc2Int(Accept.Money, 8);	// 입출금거래금액
		
		if ((HBRM_SET == m_pProfile->DEVICE.CashHandler) &&	
			(m_pDevCmn->ChangeBCFlag))							
			EjrAmountData.Cash50000Num = CashData.mFiftyThousand;	// 입출금5만원권매수

		EjrAmountData.Cash10000Num = CashData.mTenThousand;		// 입출금만원권매수
		EjrAmountData.Cash5000Num = CashData.mFiveThousand;		// 입출금오천원권매수
		EjrAmountData.Cash1000Num = CashData.mOneThousand;		// 입출금천원권매수

		if (CheckInfoCnt)
		{
			for (int i = 0; i < UCM_MAXCHECKINCNT; i++)
			{													// 자행
				if (memcmp(CheckInfoData[i].Bank, BANKJJ2, 2) == 0)
				{
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 100000)
						EjrAmountData.MotherCheck10Num++;		// 입출금자행수표10만원권매수
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 300000)
						EjrAmountData.MotherCheck30Num++;		// 입출금자행수표30만원권매수
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 500000)
						EjrAmountData.MotherCheck50Num++;		// 입출금자행수표50만원권매수
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 1000000)
						EjrAmountData.MotherCheck100Num++;		// 입출금자행수표100만원권매수
				}
				else
				{
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 100000)
						EjrAmountData.OtherCheck10Num++;		// 입출금타행수표10만원권매수
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 300000)
						EjrAmountData.OtherCheck30Num++;		// 입출금타행수표30만원권매수
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 500000)
						EjrAmountData.OtherCheck50Num++;		// 입출금타행수표50만원권매수
					if (Asc2Int(CheckInfoData[i].Amount, 10) == 1000000)
						EjrAmountData.OtherCheck100Num++;		// 입출금타행수표100만원권매수
				}
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EjrIndexSet():return");
	return T_OK;
}

// 명세표&저널인자
int	CTranCmn::fnAPD_ShuPrint(int PrintDevice, int JnlType, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_ShuPrint(PrintDevice[%x], WaitFlg[%d])", PrintDevice, WaitFlg);

	int		i, js = 0, jj =0, Len, FCh = 0;
/////////////////////////////////////////////////////////////////////////////
	// V03-01-01 카드처리시 SPL LED처리지원(SCDP통합지원)
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_SLIP_MAT, TRUE);

/////////////////////////////////////////////////////////////////////////////

	fnAPD_EjrIndexSet();										// 전자저널INDEX정보설정
/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->TranResult)		&&							// 거래정상 : 2003.12.15
		(CancelProcNum == 0))									// 취소없음
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
					m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, js++, Len, GetString("계좌잔액:*원                    ", 24, 24));
				else
					m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, js++, Len, GetString("  계좌잔액 : *원                    ", 36, 36));
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

// 저널인자(내부처리)
int	CTranCmn::fnAPD_JnlPrint(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_JnlPrint(WaitFlg[%d])", WaitFlg);

	int		i, jj =0, Len;

	fnAPD_EjrIndexSet();										// 전자저널INDEX정보설정

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

// 매체방출
int CTranCmn::fnAPD_MaterialOut(int Device, int WaitFlg, int CheckTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut(Device[%x], WaitFlg[%d], CheckTime[%d])", Device, WaitFlg, CheckTime);

	int		ScrNum = 0;
	int		ActDevice = Device;

/////////////////////////////////////////////////////////////////////////////
	MaterialOutFlg = 0;											// 매체방출유무
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
	if (MenuIrdaMode)											// 적외선거래
		;
	else
	if (CardTran)
	{
		if ((TranSend)				&&							// 거래송신후
			(TranResultNGCode == 22))							// 카드회수	
		{
			ActDevice &= ~DEV_MCU;
			m_pDevCmn->fnSCR_DisplayString(2, "카드는 당행에서 보관하겠습니다");
			m_pDevCmn->fnSCR_DisplayString(4, "은행으로 문의하여 주십시오");
			m_pDevCmn->fnSCR_DisplayScreen(701);				// 분실회수 안내화면
			Delay_Msg(3*1000);
		}
	}
//	else
//	if (PbTran)
//	{
//		if ((TranSend)		&&									// 거래송신후
//			(RetractFlag)	)									// 통장회수	
//		{
//			ActDevice &= ~DEV_PBM;
//			m_pDevCmn->fnSCR_DisplayString(2, "통장은 당행에서 보관하겠습니다");
//			m_pDevCmn->fnSCR_DisplayString(4, "은행으로 문의하여 주십시오");
//			m_pDevCmn->fnSCR_DisplayScreen(701);				// 분실회수 안내화면
//			Delay_Msg(3*1000);
//		}
//	}

/////////////////////////////////////////////////////////////////////////////
	if (ActDevice & DEV_SPR)									// 명세표방출지시
	{
		if (!SlipPrintFlg)										// 명세표인자무
			ActDevice &= ~DEV_SPR;

		if(SprOutRetryFlg)                                      // 명세표 장애시 2009.01.05
			ActDevice &= ~DEV_SPR;

	}

	if (ActDevice & DEV_BRM)									// 현금방출반환지시
	{
		if ((TranProc == TRAN_WITH) &&							// 출금
			(!CashCountFlg))									// 출금현금계수무
			ActDevice &= ~DEV_BRM;
		else
		if ((TranProc == TRAN_DEP)		&&						// 입금
			((Accept.MoneyInKind & TRAN_DEP_CASH) != TRAN_DEP_CASH))
			ActDevice &= ~DEV_BRM;								// 입금현금불가시
		else
		{
			fnAPD_CheckDeviceAction(DEV_BRM);
			Delay_Msg(200);										// 검지대기(200ms)
			fnAPD_CheckDeviceAction(DEV_BRM);
		
			if (!m_pDevCmn->fnCSH_GetMaterialInfo())			// 현금무
				ActDevice &= ~DEV_BRM;
		}
	}

	if (ActDevice & DEV_UCM)									// 수표방출반환지시
	{
		if ((TranProc == TRAN_WITH) &&							// 출금
			(!CheckCountFlg))									// 출금수표계수무
			ActDevice &= ~DEV_UCM;
		else
		if ((TranProc == TRAN_DEP) &&							// 입금
			((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK))	
			ActDevice &= ~DEV_UCM;								// 입금수표불가시
		else
		{
			fnAPD_CheckDeviceAction(DEV_UCM);

			if ((m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)) ||	
				(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW)));
			else ActDevice &= ~DEV_UCM;							// 수표무
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

	if (CardPowerOnFlg)											// 카드접속유(전자화폐)
	{
		CardPowerOnFlg = FALSE;									// 카드접속무(전자화폐)
		m_pDevCmn->fnKCS_PowerOff();							// 전원차단
		fnAPD_CheckDeviceAction(DEV_MCU);
	}

/////////////////////////////////////////////////////////////////////////////
//	거래매체 방출처리 : 2003.12.01
//	거래를 진행하는 매체에 대해서 확인한다.
//	비거래매체는 잔류하여 다음거래시 확인하도록 한다.
//  아래와 같이 처리하여도 되나(하단코드 테스트버전) 개별매체 확인하는 방식으로
//	하단에 처리하였다.
/////////////////////////////////////////////////////////////////////////////
//	if (CardTran)
//		ActDevice &= ~DEV_PBM;
//	else
//	if (PbTran)
//		ActDevice &= ~DEV_MCU;

/////////////////////////////////////////////////////////////////////////////
	if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo()) &&	// 명세표방출
		((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo()) &&	// 카드방출						
		(!((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo())))	// 통장무/지시무
	{
			MaterialOutFlg |= DEV_SPR;							// 명세표방출유
			MaterialOutFlg |= DEV_MCU;							// 카드방출유

			ScrNum = 809;										// 명세표/카드방출화면
			if (TranProc == TRAN_WITH)							// 출금
			{
				if (ActDevice & DEV_BRM)						// 현금방출유
					ScrNum = 810;								// 명세표/카드/현금방출화면
				if (ActDevice & DEV_UCM)						// 현금방출유
					ScrNum = 842;								// 명세표/카드/수표방출화면
				if ((ActDevice & DEV_BRM) &&					// 현금방출유
					(ActDevice & DEV_UCM))						// 수표방출유
					ScrNum = 843;								// 명세표/카드/현금/수표방출화면
			}
			if (TranProc == TRAN_DEP)							// 입금
			{
				if (ActDevice & DEV_BRM)						// 현금반환유
					ScrNum = 809;								// 명세표/카드/현금반환화면
				if (ActDevice & DEV_UCM)						// 현금반환유
					ScrNum = 809;								// 명세표/카드/수표반환화면
				if ((ActDevice & DEV_BRM) &&					// 현금반환유
					(ActDevice & DEV_UCM))						// 수표반환유
					ScrNum = 809;								// 명세표/카드/현금/수표반환화면
			}

			m_pDevCmn->DeviceTimeout &= ~(DEV_SPR | DEV_MCU);	// 시간초과정보초기화
			if (m_pDevCmn->EjectBothAvail)						// 카드/명세표 동시방출 지원	
			{
				if (ScrNum)
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum);		// 후의형화면출력

				m_pDevCmn->fnMCU_EjectBoth(CheckTime);
				fnAPD_CheckDeviceAction(DEV_MCU);
				m_pDevCmn->fnSPR_EjectBoth(CheckTime);
				fnAPD_CheckDeviceAction(DEV_SPR);
			}
			else
			{
				m_pDevCmn->fnSPR_Eject(CheckTime);				// U8100 AP 변경내역 #06 - 속도개선
				m_pDevCmn->fnMCU_Eject(CheckTime);				// SPR과 MCU 위치 바꾸면 반응이 빨라짐
			}													// V07-01-10-#01-end

			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("카드/명세표 방출 "));
			m_pDevCmn->fnLGT_SetFlicker(KU_SPR_ATM_FLICKER, FLICKER_ON);
			m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
	}
	else 
	if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo()) &&	// 명세표방출
		((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo()) &&	// 통장방출							
		(!((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())))	// 카드무/지시무
	{
			MaterialOutFlg |= DEV_SPR;							// 명세표방출유
			MaterialOutFlg |= DEV_PBM;							// 통장방출유

			ScrNum = 817;										// 명세표/통장방출화면
			if (TranProc == TRAN_WITH)							// 출금
			{
				if (ActDevice & DEV_BRM)						// 현금방출유
					ScrNum = 850;								// 명세표/통장/현금방출화면
				if (ActDevice & DEV_UCM)						// 현금방출유
					ScrNum = 856;								// 명세표/통장/수표방출화면
				if ((ActDevice & DEV_BRM) &&					// 현금방출유
					(ActDevice & DEV_UCM))						// 수표방출유
					ScrNum = 819;								// 명세표/통장/현금/수표방출화면
			}
			if (TranProc == TRAN_DEP)							// 입금
			{
				if (ActDevice & DEV_BRM)						// 현금반환유
					ScrNum = 817;								// 명세표/통장/현금반환화면
				if (ActDevice & DEV_UCM)						// 현금반환유
					ScrNum = 817;								// 명세표/통장/수표반환화면
				if ((ActDevice & DEV_BRM) &&					// 현금반환유
					(ActDevice & DEV_UCM))						// 수표반환유
					ScrNum = 817;								// 명세표/통장/현금/수표반환화면
			}

			m_pDevCmn->DeviceTimeout &= ~(DEV_SPR | DEV_PBM);	// 시간초과정보초기화
			m_pDevCmn->fnSPR_Eject(CheckTime);
			m_pDevCmn->fnPBM_Eject(CheckTime);	
			m_pDevCmn->fnLGT_SetFlicker(KU_SPR_ATM_FLICKER, FLICKER_ON);
			m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_ON);
	}
	else 
	if (((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo()) &&	// 카드방출
		((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo()) &&	// 통장방출					
		(!((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())))	// 명세무/지시무
	{
			MaterialOutFlg |= DEV_MCU;							// 카드방출유
			MaterialOutFlg |= DEV_PBM;							// 통장방출유

			ScrNum = 820;										// 카드/통장방출화면
			if (TranProc == TRAN_WITH)							// 출금
			{
				if (ActDevice & DEV_BRM)						// 현금방출유
					ScrNum = 820;								// 카드/통장/현금방출화면
			}
			if (TranProc == TRAN_DEP)							// 입금
			{
				if (ActDevice & DEV_BRM)						// 현금반환유
					ScrNum = 820;								// 카드/통장/현금반환화면
				if (ActDevice & DEV_UCM)						// 현금반환유
					ScrNum = 820;								// 카드/통장/수표반환화면
				if ((ActDevice & DEV_BRM) &&					// 현금반환유
					(ActDevice & DEV_UCM))						// 수표반환유
					ScrNum = 820;								// 카드/통장/현금/수표반환화면
			}

			m_pDevCmn->DeviceTimeout &= ~(DEV_MCU | DEV_PBM);	// 시간초과정보초기화
			m_pDevCmn->fnMCU_Eject(CheckTime);
			m_pDevCmn->fnPBM_Eject(CheckTime);	
			m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
			m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_ON);
	}
	else 
	{
		if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())		||	// 명세표방출
			((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())		||	// 카드방출
			((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo()))			// 통장방출
			;
		else
		{
			if (TranProc == TRAN_WITH)							// 출금
			{
				if (ActDevice & DEV_BRM)						// 현금방출유
					ScrNum = 848;								// 현금방출화면
				if (ActDevice & DEV_UCM)						// 수표방출유
					ScrNum = 849;								// 수표방출화면
				if ((ActDevice & DEV_BRM) &&					// 현금반환유
					(ActDevice & DEV_UCM))						// 수표반환유
					ScrNum = 816;								// 현금수표방출화면
			}
			if (TranProc == TRAN_DEP)							// 입금
			{
				if (ActDevice & DEV_BRM)						// 현금반환유
					ScrNum = 0;									// 현금반환화면
				if (ActDevice & DEV_UCM)						// 현금반환유
					ScrNum = 0;									// 수표반환화면
				if ((ActDevice & DEV_BRM) &&					// 현금반환유
					(ActDevice & DEV_UCM))						// 수표반환유
					ScrNum = 0;									// 현금/수표반환화면
			}
		}

																// 2003.12.01 : 카드/통장/명세표검지시
		if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())	&&	// 명세표방출
			((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())	&&	// 카드방출
			((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo()))		// 통장방출
		{
			MaterialOutFlg |= DEV_SPR;							// 명세표방출유
			MaterialOutFlg |= DEV_MCU;							// 카드방출유
			MaterialOutFlg |= DEV_PBM;							// 통장방출유

			ScrNum = 817;										// 명세표/카드방출화면
			if (TranProc == TRAN_WITH)							// 출금
			{
				if (ActDevice & DEV_BRM)						// 현금방출유
					ScrNum = 850;								// 명세표/카드/현금방출화면
				if (ActDevice & DEV_UCM)						// 현금방출유
					ScrNum = 856;								// 명세표/카드/수표방출화면
				if ((ActDevice & DEV_BRM) &&					// 현금방출유
					(ActDevice & DEV_UCM))						// 수표방출유
					ScrNum = 819;								// 명세표/카드/현금/수표방출화면
			}
			if (TranProc == TRAN_DEP)							// 입금
			{
				if (ActDevice & DEV_BRM)						// 현금반환유
					ScrNum = 817;								// 명세표/카드/현금반환화면
				if (ActDevice & DEV_UCM)						// 현금반환유
					ScrNum = 817;								// 명세표/카드/수표반환화면
				if ((ActDevice & DEV_BRM) &&					// 현금반환유
					(ActDevice & DEV_UCM))						// 수표반환유
					ScrNum = 817;								// 명세표/카드/현금/수표반환화면
			}

			m_pDevCmn->DeviceTimeout &= ~(DEV_SPR | DEV_MCU | DEV_PBM);	// 시간초과정보초기화
			m_pDevCmn->fnSPR_Eject(CheckTime);					// 동시방출보정
			m_pDevCmn->fnMCU_Eject(CheckTime);					// 동시방출보정
			m_pDevCmn->fnPBM_Eject(CheckTime);					// 동시방출보정
			m_pDevCmn->fnLGT_SetFlicker(KU_SPR_ATM_FLICKER, FLICKER_ON);
			m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
			m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_ON);
		}
		else
		{
			if ((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())
			{													// 명세표방출
				MaterialOutFlg |= DEV_SPR;						// 명세표방출유

				ScrNum = 813;									// 명세표방출화면
				if (TranProc == TRAN_WITH )						// 출금, 카드론출금 
				{
					if (ActDevice & DEV_BRM)					// 현금방출유
						ScrNum = 815;							// 명세표/현금방출화면
					if (ActDevice & DEV_UCM)					// 현금방출유
						ScrNum = 857;							// 명세표/수표방출화면
					if ((ActDevice & DEV_BRM) &&				// 현금방출유
						(ActDevice & DEV_UCM))					// 수표방출유
						ScrNum = 858;							// 명세표/현금/수표방출화면
				}
				if (TranProc == TRAN_DEP)						// 입금
				{
					if (ActDevice & DEV_BRM)					// 현금반환유
						ScrNum = 813;							// 명세표/현금반환화면
					if (ActDevice & DEV_UCM)					// 현금반환유
						ScrNum = 813;							// 명세표/수표반환화면
					if ((ActDevice & DEV_BRM) &&				// 현금반환유
						(ActDevice & DEV_UCM))					// 수표반환유
						ScrNum = 813;							// 명세표/현금/수표반환화면
				}

				m_pDevCmn->DeviceTimeout &= ~DEV_SPR;			// 시간초과정보초기화
				m_pDevCmn->fnSPR_Eject(CheckTime);
				m_pDevCmn->fnLGT_SetFlicker(KU_SPR_ATM_FLICKER, FLICKER_ON);
			}

			if ((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())
			{													// 카드방출
				MaterialOutFlg |= DEV_MCU;						// 카드방출유

				ScrNum = 806;									// 카드방출화면
				if (TranProc == TRAN_WITH)						// 출금
				{
					int nGetReg = RegGetInt(_REGKEY_DEVICE, "BRM_WITH_STEP", 0);
					if(nGetReg == 2)
						m_pDevCmn->fnSCR_DisplayString(1, "TRAN_WITH");
					
					if (ActDevice & DEV_BRM)					// 현금방출유
						ScrNum = 846;							// 카드/현금방출화면
					if (ActDevice & DEV_UCM)					// 현금방출유
						ScrNum = 847;							// 카드/수표방출화면
					if ((ActDevice & DEV_BRM) &&				// 현금방출유
						(ActDevice & DEV_UCM))					// 수표방출유
						ScrNum = 812;							// 카드/현금/수표방출화면


				}
				
				if (TranProc == TRAN_DEP)						// 입금
				{
					if (ActDevice & DEV_BRM)					// 현금반환유
						ScrNum = 806;							// 카드/현금반환화면
					if (ActDevice & DEV_UCM)					// 현금반환유
						ScrNum = 806;							// 카드/수표반환화면
					if ((ActDevice & DEV_BRM) &&				// 현금반환유
						(ActDevice & DEV_UCM))					// 수표반환유
						ScrNum = 806;							// 카드/현금/수표반환화면
				}

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("카드방출 ==>"));

				m_pDevCmn->DeviceTimeout &= ~DEV_MCU;		// 시간초과정보초기화
				m_pDevCmn->fnMCU_Eject(CheckTime);	
				m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
			}



			if ((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo())
			{													// 통장방출
				MaterialOutFlg |= DEV_PBM;						// 통장방출유

//				if (!PbNextPageFlag) - Auto Page 기능 //#0022
					ScrNum = 807;								// 통장방출화면

				if (TranProc == TRAN_WITH)						// 출금
				{
					if (ActDevice & DEV_BRM)					// 현금방출유
						ScrNum = 844;							// 통장/현금방출화면
					if (ActDevice & DEV_UCM)					// 현금방출유
						ScrNum = 845;							// 통장/수표방출화면
					if ((ActDevice & DEV_BRM) &&				// 현금방출유
						(ActDevice & DEV_UCM))					// 수표방출유
						ScrNum = 808;							// 통장/현금/수표방출화면
				}
				if (TranProc == TRAN_DEP)						// 입금
				{
					if (ActDevice & DEV_BRM)					// 현금반환유
						ScrNum = 807;							// 통장/현금반환화면
					if (ActDevice & DEV_UCM)					// 현금반환유
						ScrNum = 807;							// 통장/수표반환화면
					if ((ActDevice & DEV_BRM) &&				// 현금반환유
						(ActDevice & DEV_UCM))					// 수표반환유
						ScrNum = 807;							// 통장/현금/수표반환화면
				}

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장방출:"));

				m_pDevCmn->DeviceTimeout &= ~DEV_PBM;		// 시간초과정보초기화
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

		if (ActDevice & DEV_PBM)								// 통장방출확인(인자대기)
		{
			ULONG  WaitTime = TimerSet(10);						// 인자대기 10Sec
			while (m_pDevCmn->fnPBM_GetMaterialInfo() & ST_SENSOR2)
			{													// 안쪽검지유무
				Delay_Msg();									// 검지대기
				if (CheckTimer(WaitTime))						// 인자대기 TimeOver
					break;
			}
		}
	}

	// 통장안내메세지추가부분
	if (MaterialOutFlg & DEV_PBM)								// 통장방출유
	{
																// 통장정리이월메세지
																// 통장재발행메세지
	}

	if (ScrNum == 808)											// 통장/현금/수표
	{
		m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.CashMoney, 15);
																// 현금
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
																// 수수료
	}
	if (ScrNum == 844)											// 통장/현금
	{
		m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.CashMoney, 15);
																// 현금
		switch(TranCode2)
		{
		case TRANID_4330: 
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
		case TRANID_4310:
		case TRANID_4210:
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			break;
		}
																// 수수료
	}
	if (ScrNum == 845)											// 통장/수표
	{
		m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.CheckMoney, 15);
																// 수표
		switch(TranCode2)
		{
		case TRANID_4330: 
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
		case TRANID_4310:
		case TRANID_4210:
			m_pDevCmn->fnSCR_DisplayAmount(2, GetString(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)));
			break;
		}
																// 수수료
	}
	if ((ScrNum == 812)	||										// 카드/현금/수표
		(ScrNum == 816)	)
	{
		m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.CashMoney, 15);
																// 현금
		m_pDevCmn->fnSCR_DisplayAmount(2, (LPCTSTR)Accept.CheckMoney, 15);
																// 수표
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
	if ((ScrNum == 846)	||										// 카드/현금
		(ScrNum == 848)	)										// 현금
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
	if ((ScrNum == 847)	||										// 카드/수표
		(ScrNum == 849)	)										// 수표
	{
		m_pDevCmn->fnSCR_DisplayAmount(1, (LPCTSTR)Accept.CheckMoney, 15);
																// 수표
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
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum);					// 후의형화면출력

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut(...):return");
	return T_OK;
}

// A4용지 방출
int CTranCmn::fnAPD_A4PMaterialOut(int nPrintEndFlag, int WaitFlg, int CheckTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_A4PMaterialOut(nPrintEndFlag[%d], WaitFlg[%d], CheckTime[%d])", nPrintEndFlag, WaitFlg, CheckTime);
/////////////////////////////////////////////////////////////////////////////
	MaterialOutFlg = 0;											// 매체방출유무
/////////////////////////////////////////////////////////////////////////////

	int nScrNum = 0;
/////////////////////////////////////////////////////////////////////////////
//L	if (m_pDevCmn->fnA4P_GetMaterialInfo())						// 2007.06.21
	{															// A4용지방출
			MaterialOutFlg |= DEV_A4P;							// A4용지방출유

// 			if (nPrintEndFlag)
// 				nScrNum = 859;									// A4용지방출화면
// 			else
// 				nScrNum = 859;									// A4용지방출화면
			if(TranCode == TC_INQUIRYA)							// 거래내역출력
				nScrNum = 859;
			else		// TC_INQPRINT							// 잔고출력
				nScrNum = 861;

			m_pDevCmn->DeviceTimeout &= ~DEV_A4P;				// 시간초과정보초기화
//			m_pDevCmn->fnLGT_SetFlicker(KU_A4P_TOP_FLICKER, FLICKER_ON);	// 플리커위치 설계실수로 인자전 ON
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_A4PMaterialOut:MaterialOutFlg[%x]", MaterialOutFlg);

/////////////////////////////////////////////////////////////////////////////
	if (WaitFlg)
	{
		fnAPD_CheckDeviceAction(DEV_A4P);
	}

	if (nScrNum)
		m_pDevCmn->fnSCR_DisplayScreen(nScrNum);				// 후의형화면출력

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_A4PMaterialOut(...):return");
	return T_OK;
}

// 매체방출수취대기회수
int CTranCmn::fnAPD_MaterialOutCheckRetract(int Device, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract(Device[%x], WaitFlg[%d])", Device, WaitFlg);

	int		ActDevice = Device;
	int		RetractDevice = Device;
	int		ErrDevice = DEV_NONE;

/////////////////////////////////////////////////////////////////////////////
	MaterialOutCheckFlg = 0;									// 매체방출후수취유무
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		ActDevice &= ~DEV_SPR;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		ActDevice &= ~DEV_MCU;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		ActDevice &= ~DEV_PBM;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_A4P))
		ActDevice &= ~DEV_A4P;
	RetractDevice = ActDevice;									// 회수는장애유무만감지
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:ActDevice[%x]", ActDevice);

	if (!(MaterialOutFlg & DEV_SPR))							// 방출무
		ActDevice &= ~DEV_SPR;
	if (!(MaterialOutFlg & DEV_MCU))							// 방출무
		ActDevice &= ~DEV_MCU;
	if (!(MaterialOutFlg & DEV_PBM))							// 방출무
		ActDevice &= ~DEV_PBM;
	if (!(MaterialOutFlg & DEV_A4P))							// 방출무
		ActDevice &= ~DEV_A4P;
/*
	if ((Accept.SerialTranFlag)	&&								// 연속거래이고
		(CardTran)				)								// 카드가 있으면
	{
		ActDevice &= ~DEV_MCU;									// 카드는 회수하지 않음
	}

	if ((Accept.SerialTranFlag)	&&								// 연속거래이고
		(PbTran)				)								// 카드가 있으면
	{
		ActDevice &= ~DEV_PBM;									// 통장은 회수하지 않음
	}
*/
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:ActDevice[%x], MaterialOutFlg[%x]", ActDevice, MaterialOutFlg);

/////////////////////////////////////////////////////////////////////////////
	fnAPD_CheckDeviceAction(ActDevice);
	if (ActDevice)												// 매체존재
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

		// 카드/통장/명세표의 수취확인에 대한 로직의 변경 History
		// 1. 원안 : SP이벤트에 의존
		// 2. 수정 : SP이벤트 + 매체잔류정보확인 : 2003.11.07
		// 3. 수정 : SP이벤트에만 의존하도록 수정로직 삭제 : 2003.12.06
		if ((ActDevice & DEV_SPR) && (!(m_pDevCmn->DeviceTimeout & DEV_SPR)))
		{
//t			if (!m_pDevCmn->fnSPR_GetMaterialInfo())			// 명세표존재 : 2003.11.07 : 2003.12.06막음
//t			{
				ActDevice &= ~DEV_SPR;
				MaterialOutCheckFlg |= DEV_SPR;					// 매체방출후수취유
//t			}
		}
		if ((ActDevice & DEV_MCU) && (!(m_pDevCmn->DeviceTimeout & DEV_MCU)))
		{
//t			if (!m_pDevCmn->fnMCU_GetMaterialInfo())			// 카드존재 : 2003.11.07 : 2003.12.06막음
//t			{
				ActDevice &= ~DEV_MCU;
				MaterialOutCheckFlg |= DEV_MCU;					// 매체방출후수취유
//t			}
		}
		if ((ActDevice & DEV_PBM) && (!(m_pDevCmn->DeviceTimeout & DEV_PBM)))
		{
//t			if (!m_pDevCmn->fnPBM_GetMaterialInfo())			// 통장존재 : 2003.11.07 : 2003.12.06막음
//t			{
				ActDevice &= ~DEV_PBM;
				MaterialOutCheckFlg |= DEV_PBM;					// 매체방출후수취유
//t			}
		}
		if ((ActDevice & DEV_A4P) && (!(m_pDevCmn->DeviceTimeout & DEV_A4P)))
		{
//t			if (!m_pDevCmn->fnA4P_GetMaterialInfo())			// 프린터용지 존재
//t			{
				ActDevice &= ~DEV_A4P;
				MaterialOutCheckFlg |= DEV_A4P;					// 매체방출후수취유
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
		
	if (!(DevEnDisable & DEV_MCU))								// 카드투입금지상태
	{
		ActDevice &= ~DEV_MCU;
		RetractDevice &= ~DEV_MCU;
	}
	if (!(DevEnDisable & DEV_PBM))								// 통장투입금지상태
	{
		ActDevice &= ~DEV_PBM;
		RetractDevice &= ~DEV_PBM;
	}
/*
	if ((Accept.SerialTranFlag)	&&								// 연속거래이고
		(CardTran)				)								// 카드가 있으면
	{
		ActDevice &= ~DEV_MCU;									// 카드는 회수하지 않음
		RetractDevice &= ~DEV_MCU;
	}
	if ((Accept.SerialTranFlag)	&&								// 연속거래이고
		(PbTran)				)								// 카드가 있으면
	{
		RetractDevice &= ~DEV_PBM;								// 통장은 회수하지 않음
	}
*/
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:ActDevice[%x], RetractDevice[%x], DevEnDisable[%x]", ActDevice, RetractDevice, DevEnDisable);

	if (ActDevice)												// 고객미수취
	{
/*t---------------------------------------------------------------------------
// 미수취장애검지로직 원판임.

		if (ActDevice & DEV_SPR)								// 명세표미수취
		{
			m_pDevCmn->fnAPL_StackError("0110091", "명세표미수취", DEV_SPR);
			ErrDevice |= DEV_SPR;
		}
		
		if (ActDevice & DEV_MCU)								// 카드미수취
		{
			m_pDevCmn->fnAPL_StackError("0110191", "카드미수취", DEV_MCU);
			ErrDevice |= DEV_MCU;
		}
		
		if (ActDevice & DEV_PBM)								// 통장미수취(SQA통보요)
		{
			m_pDevCmn->fnAPL_StackError("0110591", "통장미수취", DEV_PBM);
			ErrDevice |= DEV_PBM;
		}
--------------------------------------------------------------------------- */

		// 카드+통장/카드+통장+명세표 미수취에 대한 고찰
		// DevEnDisable Flag는 선행된 매체에 대해서 그 이외의 매체가 Disable임을 설정하고 
		// 이 값으로 ActDevice의 값을 조작한다.
		// 따라서 이러한 로직에서 본다면, 절대 카드+통장이 미수취되는 경우는 없다.
		// 이러한 로직의 수정은, 
		// 1. 매체잔류 센서값(GetMaterialInfo)으로 볼수도 있으나 이럴경우 다음거래를 위해
		//	  투입된 매체를 미수취한것으로 오인할 수도 있다.
		// 2. 별도 플래그 관리...
		// 3. 방법모색중....
		// 2003.11.10
		
		// 2003.11.07
		if ((ActDevice & (DEV_MCU | DEV_SPR)) == (DEV_MCU | DEV_SPR))	// 카드+명세표미수취
		{
			m_pDevCmn->fnAPL_StackError("0110391", "카드명세표미수취", DEV_MCU | DEV_SPR);
			ErrDevice |= DEV_MCU;
			ErrDevice |= DEV_SPR;
		}
		else
		if ((ActDevice & (DEV_PBM | DEV_SPR)) == (DEV_PBM | DEV_SPR))	// 통장+명세표미수취(SQA통보요)
		{
			m_pDevCmn->fnAPL_StackError("0110691", "통장명세표미수취", DEV_PBM | DEV_SPR);
			ErrDevice |= DEV_PBM;
			ErrDevice |= DEV_SPR;
		}
		else
		if (ActDevice & DEV_MCU)								// 카드미수취
		{
			m_pDevCmn->fnAPL_StackError("0110191", "카드미수취", DEV_MCU);
			ErrDevice |= DEV_MCU;
		}
		else
		if (ActDevice & DEV_PBM)								// 통장미수취(SQA통보요)
		{
			m_pDevCmn->fnAPL_StackError("0110591", "통장미수취", DEV_PBM);
			ErrDevice |= DEV_PBM;
		}
		else
		if (ActDevice & DEV_SPR)								// 명세표미수취
		{
//			if ((TranProc == TRAN_WITH)	&&						// 출금성거래	: 2003.12.09
//				(m_pDevCmn->TranResult)	)						// HOSTOK인경우
//			{
//				m_pDevCmn->fnAPL_StackError("0110091", "명세표미수취", DEV_SPR);
//				ErrDevice |= DEV_SPR;
//			}
		}
	}

//	if (RetractDevice & DEV_SPR)
	if ((ActDevice & DEV_SPR)		&&
		(RetractDevice & DEV_SPR)	)
	{
		if (m_pDevCmn->fnSPR_GetMaterialInfo())					// 명세표존재
			fnAPD_MaterialRetract(DEV_SPR);						// 명세표지시회수
	}
	if (MaterialOutFlg & DEV_SPR)								// 방출
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_SPR_ATM_FLICKER, FLICKER_OFF);

		// V03-01-01 카드처리시 SPL LED처리지원(SCDP통합지원)
		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_SLIP_MAT, FALSE);
	}

	if (RetractDevice & DEV_MCU)
	{
		if (m_pDevCmn->fnMCU_GetMaterialInfo())					// 카드존재
			fnAPD_MaterialRetract(DEV_MCU);						// 카드지시회수
	}
	if (MaterialOutFlg & DEV_MCU)								// 방출
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);

		// V03-01-01 카드처리시 SPL LED처리지원(SCDP통합지원)
		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_CARD_MAT, FALSE);
	}

	if (RetractDevice & DEV_PBM) 
	{
		if (m_pDevCmn->fnPBM_GetMaterialInfo())					// 통장존재
			fnAPD_MaterialRetract(DEV_PBM);						// 통장지시회수
	}
	if (MaterialOutFlg & DEV_PBM)								// 방출
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_OFF);

		// V03-01-01 카드처리시 SPL LED처리지원(SCDP통합지원)
		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, FALSE);
	}

	if (RetractDevice & DEV_A4P) 
	{
		if (m_pDevCmn->fnA4P_GetMaterialInfo())					// 프린터용지존재
			fnAPD_MaterialRetract(DEV_A4P);						// 프린터용지지시회수
	}

	if (MaterialOutFlg & DEV_A4P)								// 방출
		m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_OFF);


	// 2006.7.28 카드와 통장을 방출 후 통장에 대한 플래그가 사라짐.
	// 실물은 존재하나, 플래그가 사라져 자동으로 다음 거래로 넘어가는
	// 폐단이 발생. 그래서 실물이 있다면 강제로 회수해버림.
	// 여기에 이의있다면 수정바람. CSW
//	if (m_pDevCmn->fnPBM_GetMaterialInfo())						// 통장존재
//	{
//		m_pDevCmn->fnAPL_RetractMaterial(DEV_PBM);
//		m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_OFF);
//
//		// V03-01-01 카드처리시 SPL LED처리지원(SCDP통합지원)
//		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_PSBK_MAT, FALSE);
//	}

	if (WaitFlg)
		fnAPD_CheckDeviceAction(RetractDevice);

	if (ErrDevice)												// 미수취장애검지
	{
		m_pDevCmn->DeviceLoss |= ErrDevice;
		fnAPP_CancelProc(T_ERROR);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract(...):return");
	return T_OK;
}

// 매체회수
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

	if (MaterialOutCheckFlg & DEV_MCU)							// 매체방출후수취유
		ActDevice &= ~DEV_MCU;
	if (MaterialOutCheckFlg & DEV_PBM)							// 매체방출후수취유
		ActDevice &= ~DEV_PBM;
	if (MaterialOutCheckFlg & DEV_A4P)							// 매체방출후수취유
		ActDevice &= ~DEV_A4P;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract:ActDevice[%x], MaterialOutCheckFlg[%x]", ActDevice, MaterialOutCheckFlg);

	if (!(DevEnDisable & DEV_MCU))								// 카드투입금지상태
		ActDevice &= ~DEV_MCU;
	if (!(DevEnDisable & DEV_PBM))								// 통장투입금지상태
		ActDevice &= ~DEV_PBM;
/*
	if ((Accept.SerialTranFlag)	&&								// 연속거래이고
		(CardTran)				)								// 카드가 있으면
		ActDevice &= ~DEV_MCU;									// 카드는 회수하지 않음
	if ((Accept.SerialTranFlag)	&&								// 연속거래이고
		(PbTran)				)								// 카드가 있으면
		ActDevice &= ~DEV_PBM;									// 통장은 회수하지 않음
*/
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract:ActDevice[%x], DevEnDisable[%x]", ActDevice, DevEnDisable);

	fnAPD_CheckDeviceAction(ActDevice);

	if (ActDevice & DEV_SPR)
	{
		if (m_pDevCmn->fnSPR_GetMaterialInfo())					// 명세표존재
		{
			if (MaterialOutFlg & DEV_SPR)						// 방출
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);		// 명세표회수(미수취)
			else
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);		// 명세표회수(회수지시)
		}
	}

	if (ActDevice & DEV_MCU)
	{
		if (m_pDevCmn->fnMCU_GetMaterialInfo())					// 카드존재
		{
			if (MaterialOutFlg & DEV_MCU)						// 방출
				m_pDevCmn->fnAPL_RetractMaterial(DEV_MCU);		// 카드회수(미수취)
			else
			{
//				m_pDevCmn->fnAPL_RetractMaterial(DEV_MCU);		// 카드회수(회수지시 Kim.Gi.Jin 2008.11.18)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract: Card Capture Retract Ignore When Printer Taken Standby");
				
			}
		}
	}

	if (ActDevice & DEV_PBM) 
	{
		if (m_pDevCmn->fnPBM_GetMaterialInfo())					// 통장존재
		{
			if (MaterialOutFlg & DEV_PBM)						// 방출
				m_pDevCmn->fnAPL_RetractMaterial(DEV_PBM);		// 통장회수(미수취)
			else
				m_pDevCmn->fnAPL_RetractMaterial(DEV_PBM);		// 통장회수(회수지시)
		}
	}

	if (ActDevice & DEV_A4P)
	{
		if (m_pDevCmn->fnA4P_GetMaterialInfo())					// 명세표존재
		{
			if (MaterialOutFlg & DEV_A4P)						// 방출
				m_pDevCmn->fnAPL_RetractMaterial(DEV_A4P);		// 명세표회수(미수취)
			else
				m_pDevCmn->fnAPL_RetractMaterial(DEV_A4P);		// 명세표회수(회수지시)
		}
	}

	if (WaitFlg)
		fnAPD_CheckDeviceAction(ActDevice);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract(...):return");
	return T_OK;
}

// 현금수표출금계수
int CTranCmn::fnAPD_MoneyCount(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCount(WaitFlg[%d])", WaitFlg);

	int		nDisp10000 = 0;
	int		nDisp50000 = 0;
	int		nGetCurDisp50000 = 0; 
	int		nGetCurDisp10000 = 0;

	int		CashMoney = Asc2Int(Accept.CashMoney, 11);			// 현금방출매수
	int		CheckMoney = Asc2Int(Accept.CheckMoney, 10);		// 수표방출매수
	int		nCurrCheckKind = m_pDevCmn->fnAPL_GetKindOfOutCheck();// 현재 설정된 수표권종
	
	char	CrntCheckDate[256];									// 출금수표인자내용
//	char	AddCheckNo[256];									// 수표방출매수
//	char	CrntCheckNo[256];									// 현재수표번호
//	int		CrntCheckCnt = 0;									// 현재수표매수
	int		CheckNo1=0, NumOfCheck1=0;
	int		CheckNo2=0, NumOfCheck2=0;
	int		ScrNum = 0;

/////////////////////////////////////////////////////////////////////////////
	CashCountFlg = FALSE;										// 출금현금계수유무
	CheckCountFlg = FALSE;										// 출금수표계수유무
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
			case U8100:											// U8100-AP변경
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
	// V03-01-01 카드처리시 SPL LED처리지원(SCDP통합지원)
	m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_NOTES_MAT, TRUE);

/////////////////////////////////////////////////////////////////////////////

	if ((CashMoney)		&&										// 현금+수표
		(CheckMoney)	)
		ScrNum = 841;
	else
	if ((CashMoney)		&&										// 현금만
		(!CheckMoney)	)
		ScrNum = 835;
	else
	if ((!CashMoney)	&&										// 수표만
		(CheckMoney)	)
		ScrNum = 840;

	if (ScrNum)
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum);

	switch (m_pProfile->DEVICE.MachineType)
	{
		case U8100:											// U8100-AP변경
		case U3100K:
			if (CashMoney)
			{
//				fnAPD_CheckDeviceAction(DEV_BRM);				// 속도개선 (금액입력후로 이동)
//				m_pDevCmn->fnCSH_ReadyDispense();				// 출금준비
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
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("현금계수 : 만%d +오만%d = %d만원", nDisp10000, nDisp50000, CashMoney));
				CashCountFlg = TRUE;							// 출금현금계수유
				if ((m_pDevCmn->Withdraw50TFlag)		&&		
					(WithAvail & TRAN_WITH_50TCASH)		&&
					(Asc2Int(Accept.Cash50T_Money, 15))	)
					m_pDevCmn->fnCSH_DispenseLarge(0, Asc2Int(Accept.Cash50T_Money, 11)/CASH_FIFTY_THOUSAND, Asc2Int(Accept.Cash10T_Money, 11));
				else										
					m_pDevCmn->fnCSH_Dispense(CashMoney);		// 출금계수(현금매수)
			}

			if (CheckMoney)										// 출금수표
			{
				fnAPD_CheckDeviceAction(DEV_UCM);

//				memset(CrntCheckDate, 0, sizeof(CrntCheckDate));// 출금수표인자내용편집	
				memset(CrntCheckDate, ' ', sizeof(CrntCheckDate));// 출금수표인자내용편집	
/*				
				sprintf(CrntCheckDate, "%4.4s %2.2s %2.2s", 
						&m_pProfile->TRANS.CheckYYYYMMDD[0],
						&m_pProfile->TRANS.CheckYYYYMMDD[4],
						&m_pProfile->TRANS.CheckYYYYMMDD[6]);
*/
				CheckCountFlg = TRUE;							// 출금수표계수유
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

				m_pDevCmn->fnAPL_SetNumberOfOutCheckNo(Int2Asc(CheckNo1, 8), Int2Asc(CheckNo2, 8));	// 출금수표번호설정
				m_pDevCmn->fnAPL_SetNumberOfOutCheck(NumOfCheck1, NumOfCheck2);						// 출금수표매수설정
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

// 현금수표방출수취대기
int CTranCmn::fnAPD_MoneyOutCheck(int ScrNum, int CheckTime)
{

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck(ScrNum[%d], CheckTime[%d])", ScrNum, CheckTime);

	int		ActDevice = DEV_NONE;
	int		CashRejectMethod = 0;
	int		TimeOverBrmFlg = FALSE;								// 수취타임오버BrmFlg
	int		TimeOverUcmFlg = FALSE;								// 수취타임오버UcmFlg

/////////////////////////////////////////////////////////////////////////////
	MoneyOutFlg = 0;											// 현금수표방출유무
	MoneyOutCheckFlg = 0;										// 현금수표방출후수취유무
/////////////////////////////////////////////////////////////////////////////

	if (TranProc == TRAN_WITH)									// 출금
	{
		if (CashCountFlg)										// 출금현금계수유
		{
			ActDevice |= DEV_BRM;
			CashRejectMethod = 0;								// 방출
		}
		if (CheckCountFlg)										// 출금수표계수유
			ActDevice |= DEV_UCM;
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:ActDevice[%x], CashCountFlg[%d], CheckCountFlg[%d]", ActDevice, CashCountFlg, CheckCountFlg);

	if (TranProc == TRAN_DEP || TranProc == TRAN_HIPASS)									// 입금
	{
		if ((Accept.MoneyInKind & TRAN_DEP_CASH)	||			// 입금현금
			(Accept.MoneyInKind & TRAN_DEP_CHECK));				// 입금수표
		else return T_OK;

		if (1 == CashRejectFlg)										// 입금현금반환유
		{
			ActDevice |= DEV_BRM;
			CashRejectMethod = 1;								// 반환
		}

		if (CashRejectAFlg)										// 입금현금초과반환유
		{
			ActDevice |= DEV_BRM;
			CashRejectMethod = 2;								// 초과반환
		}

		if (CheckRejectFlg)										// 입금수표반환유
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
	Delay_Msg(200);												// 검지대기(200ms)
	if ((ActDevice & DEV_BRM)	||
		(ActDevice & DEV_UCM))
	{
/////////////////////////////////////////////////////////////////////////////
		CashCountFlg = FALSE;									// 출금현금계수유무
		CheckCountFlg = FALSE;									// 출금수표계수유무
		CashRejectFlg = FALSE;									// 입금현금반환유무
		CashRejectAFlg = FALSE;									// 입금현금초과반환유무
		CheckRejectFlg = FALSE;									// 입금수표반환유무
/////////////////////////////////////////////////////////////////////////////

		if (ScrNum)												// 화면지정유
			;
		else
		if (TranProc == TRAN_DEP || TranProc == TRAN_HIPASS)								// 입금
		{
			if (ActDevice & DEV_BRM)
			{
				//#0055
				if(1 == CashRejectFlg)
					ScrNum = 839;									// 현금반환(입금취소) : 현금계수 
				else
					ScrNum = 835;                               // 현금 계수
			}
			else
			if (ActDevice & DEV_UCM)
				ScrNum = 855;									// 수표반환(입금취소)
		}

		if (ScrNum == 838)										// 현금반환(매수초과)
		{
			m_pDevCmn->fnSCR_DisplayNumeric(1, fnAPP_CalcCashMaxIn());
			m_pDevCmn->fnSCR_DisplayNumeric(2, fnAPP_CalcCashMaxInAmount());										//#N0214

			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("셔터오픈 : 1회입금한도 초과"));
		}

		if (ScrNum)												// 화면유
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
				m_pDevCmn->fnAPL_CaptureHandSave(3, FALSE, fnAPP_GetCaptureData());		// 손촬영
				m_pDevCmn->fnAPL_CaptureFaceSave(4, FALSE, fnAPP_GetCaptureData());		// 얼굴촬영
				MoneyOutFlg |= DEV_BRM;							// 현금방출유
				m_pDevCmn->DeviceTimeout &= ~DEV_BRM;			// 시간초과정보초기화
				if (TranProc == TRAN_WITH)						// 출금
				{
					m_pDevCmn->fnCSH_Present(CheckTime);		// 방출(수취대기시간)
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("셔터오픈 : 완료"));
				}
				else
				{

					if(CashRejectMethod == 1)					// 반환
						m_pDevCmn->fnCSH_Reject(CheckTime);		// 반환(수취대기시간)
					else
						m_pDevCmn->fnCSH_Present(CheckTime);	// 초과반환(수취대기시간)
				}
			}
			if (ActDevice & DEV_UCM)
			{
				MoneyOutFlg |= DEV_UCM;							// 수표방출유
				m_pDevCmn->DeviceTimeout &= ~DEV_UCM;			// 시간초과정보초기화

////////////////////////////////////////////////////////////////
//				if (TranProc == TRAN_WITH)						// 출금
//					m_pDevCmn->fnUCM_Present(CheckTime);		// 방출(수취대기시간)
//				else
//					m_pDevCmn->fnUCM_RejectAll(CheckTime);		// 전체반환(수취대기시간)
////////////////////////////////////////////////////////////////
																// V02-06-01-#10 수표방출전 수표잔류여부 판단
				if (TranProc == TRAN_WITH)						// 출금
				{
					if (!m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW))
					{											// V02-06-01-#10 출금이고 수표가 경로에 없으면 장애처리
						m_pDevCmn->fnAPL_StackError("4699991", "출금수표방출이상", DEV_UCM);
						m_pDevCmn->DeviceStatus |= DEV_UCM;
						m_pDevCmn->DeviceLoss	|= DEV_UCM;
						fnAPP_CancelProc(T_ERROR);
					}
					else										// V02-06-01-#10 출금이고 수표가 경로에 있으면 방출처리
					{
						m_pDevCmn->fnUCM_Present(CheckTime);	// 방출(수취대기시간)
					}
				}
				else
					m_pDevCmn->fnUCM_RejectAll(CheckTime);		// 전체반환(수취대기시간)
			}

// U8100 AP 변경내역 #02 - 카메라 동영상 지원
// 사용하고자 할때 풀어서 사용바람
//			m_pDevCmn->fnAPL_CaptureVODSave(VOD_START, VOD_HAND);	
/////////////////////////////////////////////////////////////////////////////
			if (ActDevice & DEV_BRM)
			{
				fnAPD_CheckDeviceAction(DEV_BRM);
				m_pDevCmn->fnAPL_CaptureHandSave(5, FALSE, fnAPP_GetCaptureData());			// 손촬영

				if((TranProc == TRAN_DEP || TranProc == TRAN_HIPASS) && CashRejectMethod == 1)
					m_pDevCmn->fnSCR_DisplayScreen(839);

				m_pDevCmn->fnCSH_WaitLift();					                        // 수취전동작대기
				fnAPD_CheckDeviceAction(DEV_BRM);

			}
			if (ActDevice & DEV_UCM)
			{
				fnAPD_CheckDeviceAction(DEV_UCM);
				m_pDevCmn->fnAPL_CaptureHandSave(7);			// 손촬영
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
				if (!(m_pDevCmn->DeviceTimeout & DEV_BRM))		// 시간초과정보무
				{

					fnAPD_CheckDeviceAction(DEV_BRM);
					m_pDevCmn->fnCSH_WaitTaken();				// 수취대기
					fnAPD_CheckDeviceAction(DEV_BRM);
					if (m_pDevCmn->DeviceTimeout & DEV_BRM)			// 미수취
						TimeOverBrmFlg = TRUE;
					else											// 정상수취
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
				m_pDevCmn->fnUCM_WaitTaken();					// 수취대기
				fnAPD_CheckDeviceAction(DEV_UCM);
				if (m_pDevCmn->DeviceTimeout & DEV_UCM)			// 미수취
					TimeOverUcmFlg = TRUE;
				else											// 정상수취
					TimeOverUcmFlg = FALSE;
			}

			m_pDevCmn->fnAPL_CaptureHandSave(5, FALSE, fnAPP_GetCaptureData());		// 손촬영
			m_pDevCmn->fnAPL_CaptureFaceSave(6, FALSE, fnAPP_GetCaptureData());		// 얼굴촬영

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
			m_pDevCmn->fnCSH_CloseShutter();					// 셔터닫기
			if (TranProc == TRAN_DEP || TranProc == TRAN_HIPASS)
				fnAPD_CheckDeviceAction(DEV_BRM);


			if ((ActDevice & DEV_BRM)	&&
				(ActDevice & DEV_UCM))
			{
				if ((!TimeOverBrmFlg)	&&						// 정상수취
					(!TimeOverUcmFlg))
				{
					if ((!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))&&	// 입출금현금무
						(!(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)))	&&	// 입금수표무
						(!(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW))))	// 출금수표무
						break;
				}
				if ((TimeOverBrmFlg)	||						// 미수취(한쪽이라도)
					(TimeOverUcmFlg))
					break;
			}
			else
			if (ActDevice & DEV_BRM)
			{

				if (!TimeOverBrmFlg)							// 정상수취
				{
					if (!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))		// 입출금현금무
						break;
				}
				if (TimeOverBrmFlg)								// 미수취
					break;
			}
			else
			if (ActDevice & DEV_UCM)
			{
				if (!TimeOverUcmFlg)							// 정상수취
				{
					if ((!(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)))	&&	// 입금수표무
						(!(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW))))	// 출금수표무
						break;
				}
				if (TimeOverUcmFlg)								// 미수취
					break;
			}
		} 
	}

/////////////////////////////////////////////////////////////////////////////
	if (ActDevice & DEV_BRM)
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_BRM_ATM_FLICKER, FLICKER_OFF);

		// V03-01-01 카드처리시 SPL LED처리지원(SCDP통합지원)
		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_NOTES_MAT, FALSE);
	}
	
	if (ActDevice & DEV_UCM)
	{
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);

		// V03-01-01 카드처리시 SPL LED처리지원(SCDP통합지원)
		m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_NOTES_MAT, FALSE);
	}

/////////////////////////////////////////////////////////////////////////////
	// 현금/수표부 잔류정보 TRACE처리 2004.09.15
MsgDump(TRACE_FREE_MODE, "Log", __FILE__, __LINE__, "방출처리후 잔류확인[%2.2X:%2.2X:%2.2X]", 
													m_pDevCmn->fnCSH_GetMaterialInfo(), 
													m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT),
													m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW));
/////////////////////////////////////////////////////////////////////////////
	if (ActDevice & DEV_BRM)
	{
		if (m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM)	// 입출금현금무
		{
			if (TranProc == TRAN_WITH)
			{
//				m_pDevCmn->fnAPL_StackError("0110291", "출금현금미수취", DEV_BRM);
				m_pDevCmn->DeviceStatus |= DEV_BRM;
				m_pDevCmn->DeviceLoss	|= DEV_BRM;
			}
			else
			{
//				m_pDevCmn->fnAPL_StackError("0110292", "입금현금미수취", DEV_BRM);
				m_pDevCmn->DeviceStatus |= DEV_BRM;
				m_pDevCmn->DeviceLoss	|= DEV_BRM;
			}
//			fnAPP_CancelProc(T_ERROR);
		}
		else
			MoneyOutCheckFlg |= DEV_BRM;						// 현금방출후수취유
	}

	if (ActDevice & DEV_UCM)
	{
		if ((m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)) ||	// 입금수표유
			(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW)))	// 출금수표유
		{
			if (TranProc == TRAN_WITH)
			{
//				m_pDevCmn->fnAPL_StackError("0110491", "출금수표미수취", DEV_UCM);
				m_pDevCmn->DeviceStatus |= DEV_UCM;
				m_pDevCmn->DeviceLoss	|= DEV_UCM;
			}
			else
			{
//				m_pDevCmn->fnAPL_StackError("0110492", "입금수표미수취", DEV_UCM);
				m_pDevCmn->DeviceStatus |= DEV_UCM;
				m_pDevCmn->DeviceLoss	|= DEV_UCM;
			}
//			fnAPP_CancelProc(T_ERROR);
		}
		else
			MoneyOutCheckFlg |= DEV_UCM;						// 수표방출후수취유
	}

	if ((ActDevice & (DEV_UCM | DEV_BRM))			&&
		(m_pDevCmn->DeviceLoss & (DEV_UCM | DEV_BRM)))
	{
		if (TranProc == TRAN_WITH)
		{
			switch(m_pDevCmn->DeviceLoss & (DEV_UCM | DEV_BRM))
			{
			case DEV_BRM:
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("셔터닫음 : 현금미수취"));
				m_pDevCmn->fnAPL_StackError("0110291", "출금현금미수취", DEV_BRM);
				fnAPP_WithErrSumProc(3);
				break;
			case DEV_UCM:
				m_pDevCmn->fnAPL_StackError("0110491", "출금수표미수취", DEV_UCM);
				break;
			case DEV_BRM | DEV_UCM :
				m_pDevCmn->fnAPL_StackError("0110991", "출금현금수표미수취", DEV_BRM | DEV_UCM);
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
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("셔터닫음 : 입금현금미수취"));
				m_pDevCmn->fnAPL_StackError("0110292", "입금현금미수취", DEV_BRM);
				fnAPP_DepErrSumProc(3);
				break;
			case DEV_UCM:
				m_pDevCmn->fnAPL_StackError("0110492", "입금수표미수취", DEV_UCM);
				break;
			case DEV_BRM | DEV_UCM :
				m_pDevCmn->fnAPL_StackError("0110992", "입금현금수표미수취", DEV_BRM | DEV_UCM);
				break;
			default:
				break;
			}
		}
		
		fnAPD_MaterialRetract(MaterialOutFlg & (DEV_MCU | DEV_PBM | DEV_SPR));	// 2004.07.09
		fnAPP_CancelProc(T_ERROR);
	}

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("셔터닫음 : 현금 수취 완료"));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:MoneyOutCheckFlg[%d]", MoneyOutCheckFlg);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck(...):return");
	return T_OK;
}

// 현금수표회수
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

	Delay_Msg(200);												// 검지대기(200ms)
	if (ActDevice & DEV_BRM)
	{
		if (m_pDevCmn->fnCSH_GetMaterialInfo())					// 입출금현금유
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyRetract()_1");
			m_pDevCmn->fnAPL_RetractMaterial(DEV_BRM);			// 현금회수
		}
	}

	if (ActDevice & DEV_UCM)
	{
		if ((m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)) ||	// 입금수표유
			(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_WITHDRAW)))	// 출금수표유
		{
			m_pDevCmn->fnAPL_RetractMaterial(DEV_UCM);			// 수표회수
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyRetract():return");
	return T_OK;
}

// 입금현금
int CTranCmn::fnAPD_CashDeposit()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDeposit()");

	int		ScrNum = 0;
	int		CashInCnt = 0;										// 입금현금투입횟수
	int		ExitFlag = FALSE;
	CString	strReturn("");
	int		TimeOverFlg = FALSE;								// 수취타임오버Flg
	int		nDepAvail = 0;										// V06-01-01-#29:입금가능권종
	int		n50000DevAvail = FALSE;								// V06-01-01-#29:5만원권 입금가능여부
	int		nReturn = -1;										// CSM상태 조회
	BOOL	bNotePosion = FALSE;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.CashMoney, '0', sizeof(Accept.CashMoney));	// 금액(만원권)/입금현금
	CashInCountFlg = FALSE;										// 입금현금계수유무
	CashRejectFlg = FALSE;										// 입금현금반환유무
	CashRejectAFlg = FALSE;										// 입금현금초과반환유무
	memset(&CashData, 0, sizeof(CashData));						// 초기화
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("입금불가 : BRM 사용불가 상태"));
		return T_OK;
	}

	if ((Accept.MoneyInKind & TRAN_DEP_CASH) != TRAN_DEP_CASH)	// 입금현금불가시
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_BRM);
	m_pDevCmn->fnCSH_ReadyCount();								// 입금준비         
	fnAPD_CheckDeviceAction(DEV_BRM);                           // #N0190 (CashInEvent Ready)
	
/////////////////////////////////////////////////////////////////////////////
	ScrNum = 831;												// 입금현금

	nDepAvail = m_pDevCmn->fnCSH_GetAvailDeposit();				// 입금가능권종
	if ((m_pProfile->DEVICE.CashHandler == HBRM_SET)        &&	//
		(m_pDevCmn->ChangeBCFlag)							&&	// 5만원권 개체여부
		((m_pProfile->BRM.DepositCashKind & 0x08) == 0x08)	)	// 5만원권 가능여부
//		(m_pDevCmn->Withdraw50TFlag)						)	// 5만원권 카세트여부
		n50000DevAvail = TRUE;

	
	m_pDevCmn->fnLGT_SetFlicker(KU_BRM_ATM_FLICKER, FLICKER_ON);
	if (!CashInCnt)												// 첫투입시
	{
		m_pDevCmn->fnCSH_OpenShutter();							// 열기
		fnAPD_CheckDeviceAction(DEV_BRM);
	}
//////////////////////////////////////////////////////////////////////////

	CashInCnt = 0;												// 입금현금투입횟수
	ExitFlag = FALSE;
	while ((CashInCnt <= BRM_CASHINRETRY) && (!ExitFlag))		// 입금현금재투입가능횟수
	{
		if ((CashInCnt)	&&										// 재투입시
			(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM) &&
			(FALSE == bNotePosion))								// 투입구현금유
		{
			nDepAvail = m_pDevCmn->fnCSH_GetAvailDeposit();
			switch (nDepAvail)
			{
				case DEP_CASH_ONLY:
					if ((n50000DevAvail)			&&
						(m_pDevCmn->Withdraw50TFlag))			// 5만+1만원만 입금가능
					{
						m_pDevCmn->fnSCR_DisplayString(1, "만원권/5만원권만 입금이 가능합니다");
						m_pDevCmn->fnSCR_DisplayString(12, "고액권입금가능");
					}
					else
						m_pDevCmn->fnSCR_DisplayString(1, "만원권만 입금이 가능합니다");
					break;
				case DEP_ONLY_FIFTY_TH:
					if (n50000DevAvail)
					{
						if (m_pDevCmn->Withdraw50TFlag)
							m_pDevCmn->fnSCR_DisplayString(1, "5만원권만 입금이 가능합니다");
						
						m_pDevCmn->fnSCR_DisplayString(12, "고액권입금가능");
					}
					break;
				case DEP_ONLY_TEN_TH:
					m_pDevCmn->fnSCR_DisplayString(1, "만원권만 입금이 가능합니다");
					break;
				case DEP_FIFTY_TH :								// V06-01-01-#17-start
					m_pDevCmn->fnSCR_DisplayString(12, "고액권입금가능");
					break;
				case DEP_BOTH_AVAILABLE:								
					if (n50000DevAvail)
						m_pDevCmn->fnSCR_DisplayString(12, "고액권입금가능");
					break;										// V06-01-01-#17-end
				default:
					break;
			}
			
//////////////////////////////////////////////////////////////////////////
			bNotePosion = FALSE;								// 투입전 자세이상 OFF
//////////////////////////////////////////////////////////////////////////
			m_pDevCmn->fnSCR_DisplayScreen(839);				// 현금반환
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
			
				if ((CashInCnt >= BRM_CASHINRETRY) || (TimeOverFlg) ||                         // 반환, 수납(진행) - #N0190		
					(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))	
				{													// 투입구현금유
/*
					m_pDevCmn->fnCSH_CloseShutter();				// 닫기
					fnAPD_CheckDeviceAction(DEV_BRM);
					m_pDevCmn->fnLGT_SetFlicker(KU_BRM_ATM_FLICKER, FLICKER_OFF);
*/
					if (m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM)
					{												// 투입구현금유
						if (!TimeOverFlg)							// 수취정상
						{
							continue;								// 재반환(확률이적음)
						}
						else
						{
							m_pDevCmn->fnAPL_StackError("0110293", "입금현금미수취", DEV_BRM);
							m_pDevCmn->DeviceStatus |= DEV_BRM;
							m_pDevCmn->DeviceLoss	|= DEV_BRM;
							fnAPP_CancelProc(T_TIMEOVER);			// 미수취처리
						}
					}
					else
					{
						ExitFlag = TRUE;				
						continue;									// 반환, 수납(진행)
					}
				}
				

				if((CashInCnt < BRM_CASHINRETRY) && (ScrNum == 832 || ScrNum == 833))
				{
					if(!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM) && !(TimeOverFlg))  //입금 중 문제점 개선 - #N0190
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
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("셔터오픈_[%d] : 재투입", CashInCnt));	
							fnAPD_CheckDeviceAction(DEV_BRM);
						}
					}
				}

			}
			else
			if(m_pDevCmn->AtmDefine.MachineType == U8100)
			{
				m_pDevCmn->DeviceTimeout &= ~DEV_BRM;				// 시간초과정보초기화
				m_pDevCmn->fnCSH_Present2();						// 방출(수취대기시간):닫기없음
				fnAPD_CheckDeviceAction(DEV_BRM);
				m_pDevCmn->fnCSH_WaitTaken();						// 수취대기
				fnAPD_CheckDeviceAction(DEV_BRM);
				
				if (m_pDevCmn->DeviceTimeout & DEV_BRM)				// 미수취
					TimeOverFlg = TRUE;
				else												// 수취정상
					TimeOverFlg = FALSE;
	
				if ((TimeOverFlg) || (CashInCnt >= BRM_CASHINRETRY))	
				{												
					if ((CashInCnt >= BRM_CASHINRETRY) ||		// 반환, 수납(진행)
						(m_pDevCmn->fnBRM_GetMaterialInfo() & BRM_IN_CSM))	
					{											// 투입구현금유
						m_pDevCmn->fnBRM_CloseShutter();		// 닫기
						fnAPD_CheckDeviceAction(DEV_BRM);
						m_pDevCmn->fnLGT_SetFlicker(KU_BRM_ATM_FLICKER, FLICKER_OFF);
						if (m_pDevCmn->fnBRM_GetMaterialInfo() & BRM_IN_CSM)
						{										// 투입구현금유
							if (!TimeOverFlg)					// 수취정상
							{
								ExitFlag = TRUE;				// HBRM				
//							    continue;						// 재반환(확률이적음)
								break;							// 국민은행 소스참고(20061214 센서이상 보완) 
							}
							else
							{
								m_pDevCmn->fnAPL_StackError("0110293", "입금현금미수취", DEV_BRM);
								m_pDevCmn->DeviceStatus |= DEV_BRM;
								m_pDevCmn->DeviceLoss	|= DEV_BRM;
								fnAPP_CancelProc(T_ERROR);		// 미수취처리
							}
						}
						else
						{
							ExitFlag = TRUE;				
							continue;							// 반환, 수납(진행)
						}
					}
				}

				if((CashInCnt < BRM_CASHINRETRY) && (ScrNum == 832 || ScrNum == 833))
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("셔터오픈_[%d] : 재투입", CashInCnt));	
			}
			else
			{
				fnAPP_CancelDepRejectProc(T_MSG, "기종 설정 오류 입니다","상황실에 문의하여 주시기 바랍니다.");				
			}
		}

		if (ScrNum == 831)										// 입금현금
		{
			m_pDevCmn->fnSCR_DisplayNumeric(1, fnAPP_CalcCashMaxIn());												// 만원권만 입금가능안내
			m_pDevCmn->fnSCR_DisplayNumeric(2, fnAPP_CalcCashMaxInAmount());										//#N0214

			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("셔터오픈 : 입금"));	// #H0257
		}

		if (ScrNum == 833)										// 현금투입(불량현금/금액확인)
		{
			m_pDevCmn->fnSCR_DisplayAmount(1, CashData.mTotalAmount);
			if (m_pDevCmn->fnCSH_GetAvailDeposit() == DEP_ONLY_TEN_TH)
			{													// 만원권만 입금가능안내
				m_pDevCmn->fnSCR_DisplayString(2, "만원권만 입금이 가능합니다");
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
					m_pDevCmn->fnSCR_DisplayString(12, "고액권입금가능");
				}
				break;
			default:
				break;
		}
		
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE1);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if ((strReturn == S_SHUTTERCLOSE)	||					// 닫음입력
			(strReturn == S_CONFIRM)		||					// 확인입력
			(strReturn == S_CONFIRM2)		||                  // OK입력
			(strReturn == S_TIMEOVER))					        // TIMEOVER (정상처리) #N0190	
		{
			m_pDevCmn->fnAPL_CaptureHandSave(3, FALSE, fnAPP_GetCaptureData());		// 손촬영
			m_pDevCmn->fnAPL_CaptureFaceSave(4, FALSE, fnAPP_GetCaptureData());		// 얼굴촬영

			m_pDevCmn->fnLGT_SetFlicker(KU_BRM_ATM_FLICKER, FLICKER_OFF);
			fnAPD_CashDepositCount();							// 입금현금계수
			if (CashData.mTotalAmount)
				CashInCountFlg = TRUE;							// 입금현금계수유
			fnAPD_CheckDeviceAction(DEV_BRM);
			switch (CashData.mStatus)							// 계수상태처리
			{
				case NORMAL:									// 현금계수완료
					ExitFlag = TRUE;
					break;

				case NO_MONEY:									// 현금무
					ExitFlag = TRUE;
					break;

				case SKEW_NOTE:									// 현금조정
					ScrNum = 832;								// 현금재투입(자세이상)
					if (CashData.mTotalAmount)					// 금액유
						ScrNum = 833;							// 현금재투입(불량현금/금액확인)
					break;

				case UNKNOWN_MONEY:								// 감별오류현금존재(SKEW포함)
					ScrNum = 832;								// 현금재투입(불량현금)
					if (CashData.mTotalAmount)					// 금액유
						ScrNum = 833;							// 현금재투입(불량현금/금액확인)
					break;
				
				case MAX_OVER:									// 입금매수초과
					CashRejectAFlg = TRUE;						// 입금현금초과반환유				    
					fnAPD_MoneyOutCheck(838);					// 현금반환(매수초과)
					ExitFlag = TRUE;
					break;

				case DOWN:										// 장애발생
				default:
					fnAPD_CheckDeviceAction(DEV_BRM);
					fnAPP_CancelProc(T_PROGRAM);
					break;
			}
		}
		else
		{
			m_pDevCmn->fnCSH_CloseShutter();					// 닫기
			fnAPD_CheckDeviceAction(DEV_BRM);
			m_pDevCmn->fnLGT_SetFlicker(BRM_FLICKER, FLICKER_OFF);
			if (strReturn == S_CANCEL)
				fnAPP_CancelDepRejectProc(T_CANCEL);
			else fnAPP_CancelDepRejectProc(T_TIMEOVER);

		}

		CashInCnt++;											// 입금현금투입횟수증가
	}

	m_pDevCmn->fnCSH_CloseShutter();							// 닫기:현금무등
	fnAPD_CheckDeviceAction(DEV_BRM);

	
	if (!m_pDevCmn->ChangeBCFlag)								// V06-01-01-#11
		CashData.mFiftyThousand = 0;							// V06-01-01-#11


	if ((m_pProfile->BRM.DepositCashKind & 0x08) != 0x08)		
		CashData.mFiftyThousand = 0;							


	if((CashData.mTenThousand + CashData.mFiveThousand + CashData.mOneThousand + CashData.mFiftyThousand) > fnAPP_CalcCashMaxIn())	
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelDepRejectProc(T_MSG, "입금 투입 매수 초과입니다");
		else
			fnAPP_CancelDepRejectProc(T_CANCEL);
	}


//	if (!m_pDevCmn->fnCSH_GetMaterialInfo())					// 현금무
//	{
//		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
//			fnAPP_CancelDepRejectProc(T_MSG, "입금된 현금이 없습니다");
//		else
//			fnAPP_CancelDepRejectProc(T_CANCEL);
//	}

	if (!CashData.mTotalAmount)									// 금액무
	{
		if ((Accept.MoneyInKind & TRAN_DEP_CHECK)	&&			// 수표입금유
			(CheckInfoCnt))										// 수표금액유
		{
			Accept.MoneyInKind &= ~TRAN_DEP_CASH;				// 현금입금무처리
		}
		else
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("입금 금액 없음"));
			fnAPP_CancelDepRejectProc(T_MSG, "입금된 금액이 없습니다");
		}
		else
			fnAPP_CancelDepRejectProc(T_CANCEL);
	}
																
	if (CashData.mTotalAmount)									// 금액유 : 2003.11.01
	{
		if (m_pDevCmn->fnCSH_GetMaterialInfo() != IN_NSTK)		// 일시스택커에만현금존재체크
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("재투입 초과(3회)"));
				fnAPP_CancelDepRejectProc(T_MSG, "입금 재투입 횟수 초과입니다(3회이상)");
			}
			else
				fnAPP_CancelDepRejectProc(T_CANCEL);
		}
		else
		{
			if ((CheckInfoAmt + CashData.mTotalAmount) <= Accept.ChargeCommission)
				fnAPP_CancelDepRejectProc(T_MSG, "수수료금액 차감후 입금할 금액이 없습니다.");
		}
	}

	Int2Asc(CashData.mTotalAmount, Accept.CashMoney, 15, 10, '0');

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDeposit():return");
	return T_OK;												// 입금현금금액Set
}


// 입금현금계수
int CTranCmn::fnAPD_CashDepositCount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositCount()");

	int		UnknownCnt = 0;										// 입금감별횟수
	int		ExitFlag = FALSE;
	CString			strCashData("");
	CStringArray	strCashDataArray;

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CASH) != TRAN_DEP_CASH)	// 입금현금불가시
		return T_OK;


	UnknownCnt = 0;												// 입금감별횟수
	ExitFlag = FALSE;

	if (!m_pDevCmn->ChangeBCFlag)								// V06-01-01-#11
		m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_50000, FALSE);
	
	while ((UnknownCnt < BRM_UNKNOWNRETRY) && (!ExitFlag))		// 입금현금감별가능횟수
	{
		if (((OKI_SET == m_pProfile->DEVICE.CashHandler) ||
			(HBRM_SET == m_pProfile->DEVICE.CashHandler))&&	// U8100-AP변경
			(m_pDevCmn->ChangeBCFlag))							// V06-01-01-#03
		{														
			m_pDevCmn->fnCSH_AcceptLarge(fnAPP_CalcCashMaxIn(), fnAPP_CalcCashMaxInAmount()*10000);
		}
		else
			m_pDevCmn->fnCSH_Accept(fnAPP_CalcCashMaxIn());		// 투입(입금가능현금매수)
		
		fnAPD_CheckDeviceAction(DEV_BRM);
		CashData.mStatus = m_pDevCmn->fnCSH_GetAcceptCountStatus();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositCount:CashData.mStatus[%d]", CashData.mStatus);
		switch (CashData.mStatus)								// 계수시작상태처리
		{
			case NORMAL:										// 현금계수시작완료
				m_pDevCmn->fnSCR_DisplayScreen(835);			// 현금계수(입금)
				m_pDevCmn->fnCSH_Count();						// 입금계수
				fnAPD_CheckDeviceAction(DEV_BRM);
				strCashData = m_pDevCmn->fstrBRM_GetCountResult();	// 계수결과
				SplitString(strCashData, ",", strCashDataArray);	// 금액누적
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

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("계수 S:%d TOT:%d 50K:%d 10K:%d 5K:%d 1K:%d UK:%d",
						CashData.mStatus,
						CashData.mTotalAmount,
						CashData.mFiftyThousand,
						CashData.mTenThousand,
						CashData.mFiveThousand,
						CashData.mOneThousand,
						CashData.mUnknown));


				if (m_pDevCmn->ChangeBCFlag)								
				{
					if (CashData.mTotalAmount !=				// 금액체크
						((CashData.mFiftyThousand * 50000) +
						 (CashData.mTenThousand	  * 10000) +
						 (CashData.mFiveThousand  *  5000) + 
						 (CashData.mOneThousand   *  1000)))
						fnAPP_CancelProc(T_PROGRAM);
				}

				fnAPD_CashDepositPrint();						// 현금계수정보인자

				switch (CashData.mStatus)
				{
					case UNKNOWN_MONEY:							// 감별오류현금존재(SKEW포함)
						break;									// 내부현금감별

					case DOWN:									// 장애발생
						fnAPD_CheckDeviceAction(DEV_BRM);
					case MAX_OVER:								// 입금매수초과
					case NORMAL:								// 현금계수완료
					default:
						ExitFlag = TRUE;
						break;
				}
				break;

			case DOWN:											// 장애발생
				fnAPD_CheckDeviceAction(DEV_BRM);
			case NO_MONEY:										// 현금무
			case SKEW_NOTE:										// 현금조정
			default:
				ExitFlag = TRUE;
				break;
		}

		UnknownCnt++;											// 입금현금감별가능횟수증가
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositCount():return");
	return T_OK;
}

// 입금현금수납
int CTranCmn::fnAPD_CashDepositStore(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositStore(WaitFlg[%d])", WaitFlg);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CASH) != TRAN_DEP_CASH)	// 입금현금불가시
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_BRM);

	m_pDevCmn->fnAPL_CaptureHandSave(7, FALSE, fnAPP_GetCaptureData());		// 손촬영
	m_pDevCmn->fnAPL_CaptureFaceSave(8, FALSE, fnAPP_GetCaptureData());		// 얼굴촬영	

	m_pDevCmn->fnCSH_Stack();									// 수납
	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_BRM);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositStore(...):return");
	return T_OK;
}

// 입금현금반환
int CTranCmn::fnAPD_CashDepositReject()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositReject()");

/////////////////////////////////////////////////////////////////////////////
	CashRejectFlg = FALSE;										// 입금현금반환유무
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CASH) != TRAN_DEP_CASH)	// 입금현금불가시
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_BRM);

	// 5FF51 대책 Start: 2006.11.28 --------------------------------------------------
	if (m_pDevCmn->fnBRM_GetMaterialInfo())                 // 현금존재만Check
		CashRejectFlg = TRUE;                               // 입금현금반환유
	// 5FF51 대책 End: 2006.11.28 --------------------------------------------------

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositReject():CashRejectFlg[%d]", CashRejectFlg);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositReject():return");
	return T_OK;
}

// 입금수표
int CTranCmn::fnAPD_CheckDeposit()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit()");

	int		ScrNum = 0;
	int		UnknownCnt = 0;										// 입금감별횟수
	int		ExitFlag = FALSE;
	CString	strReturn("");
	int		nReturn = 0;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.CheckMoney, '0', sizeof(Accept.CheckMoney));	// 금액(수표권)/입금수표
	CheckInCountFlg = FALSE;									// 입금수표계수유무
	CheckRejectFlg = FALSE;										// 입금수표반환유무
	CheckRejectAFlg = FALSE;									// 입금수표오류반환유무
	CheckRejectKind = 0;										// 입금수표반환사유
	memset(CheckInfoData, '0', sizeof(CheckInfoData));			// 초기화
	CheckInfoStats = NORMAL;									// 입금수표상태
	CheckInfoCnt = 0;											// 입금수표매수
	CheckInfoAmt = 0;											// 입금수표금액
/////////////////////////////////////////////////////////////////////////////

	fnAPP_CheckDepositPrintEdit();								// 입금수표배서내용편집

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)// 입금수표불가시
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_UCM);

/////////////////////////////////////////////////////////////////////////////
	CheckInfoCnt = 0;											// 입금수표매수
	UnknownCnt = 0;												// 입금감별횟수
	ExitFlag = FALSE;
	while ((CheckInfoCnt < fnAPP_CalcCheckMaxIn())	&&			// 입금수표가능매수
		   (UnknownCnt < UCM_UNKNOWNRETRY)			&&			// 입금수표감별가능횟수
		   (!ExitFlag))	
	{
		if (!CheckInfoCnt)
			ScrNum = 851;										// 입금수표
		else
			ScrNum = 852;										// 입금수표(연속)

		if (ScrNum == 851)										// 입금수표
		{
			m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("익영업일 12시00분"));
		}
		if (ScrNum == 852)										// 입금수표(연속)
		{
			m_pDevCmn->fnSCR_DisplayNumeric(1, CheckInfoCnt);
			m_pDevCmn->fnSCR_DisplayAmount(2, CheckInfoAmt + CashData.mTotalAmount);
			m_pDevCmn->fnSCR_DisplayNumeric(3, fnAPP_CalcCheckMaxIn() - CheckInfoCnt);
		}

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
		m_pDevCmn->DeviceTimeout &= ~DEV_UCM;					// 시간초과정보초기화
		m_pDevCmn->fnUCM_Accept();								// 투입허가(투입대기시간)및열기
																// 65Sec충분히(화면에서먼저온다)

		if (m_pDevCmn->EarPhoneInserted == TRUE)	
			m_pDevCmn->fnPIN_EntryEnable(PINMODE_NORMAL, 0, 100, FALSE, PIN_NUMERIC_PAD, "", "",  -1);

		nReturn = m_pDevCmn->fnCMN_ScanDeviceAction(DEV_UCM | DEV_SCR, K_65_WAIT, EVENT_IN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit:nReturn[%d], ", nReturn);
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
		if (nReturn == R_TIMEOUT)
		{
			fnAPD_CheckDeviceAction(DEV_UCM);
			m_pDevCmn->fnUCM_CancelAccept();					// 투입금지및닫기
			fnAPD_CheckDeviceAction(DEV_UCM);
			fnAPP_CancelDepRejectProc(T_TIMEOVER);				// 취소입금반환함수(정상업무처리)
		}

		if (nReturn == DEV_UCM)
		{
			fnAPD_CheckDeviceAction(DEV_UCM);
			fnAPD_CheckDepositCount();							// 입금수표계수
			if (CheckInfoAmt)
				CheckInCountFlg = TRUE;							// 입금수표계수유
			fnAPD_CheckDeviceAction(DEV_UCM);
			switch (CheckInfoStats)								// 계수상태처리
			{
				case NORMAL:									// 현금계수완료
					UnknownCnt = 0;
					break;

				case READ_ERROR:								// 수표오류판독
					if (CheckRejectKind == 3)					// 타행입금수표불가
						fnAPD_CheckDepositCurOutCheck(855);		// 수표오류반환(동일수표)
					else
					if (CheckRejectKind == 2)					// 타행입금수표불가
						fnAPD_CheckDepositCurOutCheck(855);		// 수표오류반환(타행수표)
					else
					{
						Delay_Msg(500);							// 2003.12.03
						fnAPD_CheckDepositCurOutCheck(854);		// 수표오류반환(불량수표)
					}
					UnknownCnt++;
					break;

				case READ_TIME_OUT:								// 수표투입TimeOver
					fnAPP_CancelDepRejectProc(T_TIMEOVER);		// 취소입금반환함수(정상업무처리)
					break;

				case DOWN:										// 장애발생
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

			if (strReturn == S_CONFIRM)							// 확인입력
			{
				fnAPD_CheckDeviceAction(DEV_UCM);
				m_pDevCmn->fnUCM_CancelAccept();				// 투입금지및닫기
				fnAPD_CheckDeviceAction(DEV_UCM);				
				ExitFlag = TRUE;								// 입금수표종료
			}
			else
			{
				fnAPD_CheckDeviceAction(DEV_UCM);
				m_pDevCmn->fnUCM_CancelAccept();				// 투입금지및닫기
				fnAPD_CheckDeviceAction(DEV_UCM);
				if (strReturn == S_CANCEL)
				{
					if (!CheckInCountFlg)						// U8100-공장출하 장애대책-start
					{
						m_pDevCmn->fnUCM_CloseShutter();		// 닫기:최대입금완료시
						fnAPD_CheckDeviceAction(DEV_UCM);
						Delay_Msg(2*1000);
					}											// U8100-공장출하 장애대책-end
					fnAPP_CancelDepRejectProc(T_CANCEL);		// 취소입금반환함수(정상업무처리)
				}
				else fnAPP_CancelDepRejectProc(T_TIMEOVER);		// 취소입금반환함수(정상업무처리)
			}
		}
	}

	m_pDevCmn->fnUCM_CloseShutter();							// 닫기:최대입금완료시
	fnAPD_CheckDeviceAction(DEV_UCM);

	if (m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT) & (UCM_DEP_IN_CSM | UCM_DEP_ON_PATH))
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit:m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)[%x], ", m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT));
		CheckRejectAFlg = TRUE;									// 입금수표오류반환유
		Delay_Msg(500);											// 2003.12.03
		fnAPD_CheckDepositCurOutCheck(854);						// 확인과수표동시입력
		m_pDevCmn->fnUCM_CloseShutter();
		fnAPD_CheckDeviceAction(DEV_UCM);
	}

//	if (!m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT))			// 입금수표무
//	{
//		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
//			fnAPP_CancelDepRejectProc(T_MSG, "입금된 수표가 없습니다");// 취소입금반환함수(정상업무처리)
//		else
//			fnAPP_CancelDepRejectProc(T_CANCEL);
//	}

	if ((!CheckInfoCnt) ||										// 입금수표내역무
		(!CheckInfoAmt))										// 입금수표금액무
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)							
			fnAPP_CancelDepRejectProc(T_MSG, "입금된 수표금액이 없습니다");
		else													// 취소입금반환함수(정상업무처리)
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

	Int2Asc(CheckInfoAmt, Accept.CheckMoney, 15, 10, '0');		// 입금수표금액Set
	Int2Asc(CheckInfoMotherAmtTemp1, Accept.MotherCheckMoneyM, 15, 10, '0');	
																// 입금수표자행자점금액Set
	Int2Asc(CheckInfoMotherAmtTemp2, Accept.MotherCheckMoneyO, 15, 10, '0');	
																// 입금수표자행타점금액Set
	Int2Asc(CheckInfoOtherAmtTemp, Accept.OtherCheckMoney, 15, 10, '0');
																// 입금수표타행금액Set

	fnAPP_PrintCheckDeposit(DEV_JPR);							// 수표입금정보인자

	if (m_pDevCmn->EarPhoneInserted == TRUE)				
		m_pDevCmn->fnPIN_EntryDisable();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit():return");
	return T_OK;
}

// 입금수표계수
int CTranCmn::fnAPD_CheckDepositCount()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositCount()");

	CString		strCheckData("");
	char		CheckData[38];
	CheckInfo	CheckInfoTemp;
	int			i = 0;
	int			nIndex = 0;

/////////////////////////////////////////////////////////////////////////////
	CheckRejectAFlg = FALSE;									// 입금수표오류반환유무
	CheckRejectKind = 0;										// 입금수표반환사유
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)// 입금수표불가시
		return T_OK;

	m_pDevCmn->fnSCR_DisplayScreen(853);						// 수표계수(입금)
	fnAPD_CheckDeviceAction(DEV_UCM);
	m_pDevCmn->fnUCM_Read();									// 읽기
	fnAPD_CheckDeviceAction(DEV_UCM);
	if (m_pDevCmn->DeviceTimeout & DEV_UCM)
		CheckInfoStats = READ_TIME_OUT;
	else
		CheckInfoStats = NORMAL;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeposit:m_pDevCmn->DeviceTimeout[%x], CheckInfoStats[%d]", m_pDevCmn->DeviceTimeout, CheckInfoStats);

	switch (CheckInfoStats)										// 수표상태처리
	{
		case NORMAL:											// 수표계수정상완료
			if (CheckInfoCnt >= fnAPP_CalcCheckMaxIn())			// 입금수표한도매수초과
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
			{													// 입금수표길이이상
				CheckInfoStats = READ_ERROR;
				break;
			}

			// 01234567890123456789012345678901234567			
			// 12345678<24=1759:000484<15;0000500000;			// 수표의 분리부호를 체크한다.
			if ((CheckData[8]  != '<') || (CheckData[11] != '=') ||
				(CheckData[16] != ':') || (CheckData[23] != '<') ||
				(CheckData[26] != ';') || (CheckData[37] != ';'))
			{
				CheckInfoStats = READ_ERROR;
				break;
			}

			for ( nIndex = 27 ; nIndex < 37 ; nIndex++ )		// 수표부독관련 보정처리	// V02.06.01
			{
				  if (CheckData[nIndex] == '_')
					  CheckData[nIndex] = '0';
			}

			if ((!IsNum((void*)&CheckData[0], 8))	||			// 수표의 자료를 체크한다. (유효범위 : 0x30 ~ 0x39)
				(!IsNum((void*)&CheckData[9], 2))	||
				(!IsNum((void*)&CheckData[12], 4))	||
				(!IsNum((void*)&CheckData[17], 6))	||
				(!IsNum((void*)&CheckData[24], 2))	||
				(!IsNum((void*)&CheckData[27], 10)))
			{
				CheckInfoStats = READ_ERROR;
				break;
			}

			if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE);	// 위권모드
			else												// 모의 수표인지를 체크한다.
			{
				// 아래의 수표는 알고리즘 체크를 하지 않는다.
				if (!memcmp(&CheckData[9], "71", 2))
					;			// 체신부수표
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

			memcpy(CheckInfoTemp.ReadData, &CheckData[0], 8);	// 자료복사
			memcpy(CheckInfoTemp.Bank, &CheckData[9], 2);
			memcpy(CheckInfoTemp.Branch, &CheckData[12], 4);
			memcpy(CheckInfoTemp.AccountNum, &CheckData[17], 6);
			memcpy(CheckInfoTemp.Kind, &CheckData[24], 2);
			memcpy(CheckInfoTemp.Amount, &CheckData[27], 10);

																// 수표자료검사
			if ((!IsNum(CheckInfoTemp.ReadData, 8)) ||			// 수표번호체크
				IsZero(CheckInfoTemp.ReadData, 8)	||
				(!IsNum(CheckInfoTemp.Bank, 2))		||			// 은행체크
				IsZero(CheckInfoTemp.Bank, 2)		||
				(!IsNum(CheckInfoTemp.Branch, 4))	||			// 점번체크
				IsZero(CheckInfoTemp.Branch, 4)		||
// 				(!IsNum(CheckInfoTemp.AccountNum, 6))||			// 계좌번호체크
// 				IsZero(CheckInfoTemp.AccountNum, 6)	||
				(!IsNum(CheckInfoTemp.Kind, 2))		||			// 권종체크
				IsZero(CheckInfoTemp.Kind, 2)		||
				(!IsNum(CheckInfoTemp.Amount, 10))	||			// 금액체크
				IsZero(CheckInfoTemp.Amount, 10))
			{
				CheckInfoStats = READ_ERROR;
			}
			else
			if ((Asc2Int(CheckInfoTemp.Kind, 2) == 13) ||		// 수표종별검사
				(Asc2Int(CheckInfoTemp.Kind, 2) == 14) ||
				(Asc2Int(CheckInfoTemp.Kind, 2) == 15) ||
				(Asc2Int(CheckInfoTemp.Kind, 2) == 16))
			{
				if (Asc2Int(CheckInfoTemp.Kind, 2) == 13)		// 10만원검증
				{
					if (Asc2Int(CheckInfoTemp.Amount, 10) != 100000)
						CheckInfoStats = READ_ERROR;
				}
				if (Asc2Int(CheckInfoTemp.Kind, 2) == 14)		// 30만원검증
				{
					if (Asc2Int(CheckInfoTemp.Amount, 10) != 300000)
						CheckInfoStats = READ_ERROR;
				}
				if (Asc2Int(CheckInfoTemp.Kind, 2) == 15)		// 50만원검증
				{
					if (Asc2Int(CheckInfoTemp.Amount, 10) != 500000)
						CheckInfoStats = READ_ERROR;
				}
				if (Asc2Int(CheckInfoTemp.Kind, 2) == 16)		// 100만원검증
				{
					if (Asc2Int(CheckInfoTemp.Amount, 10) != 1000000)
						CheckInfoStats = READ_ERROR;
				}
			}
			else
				CheckInfoStats = READ_ERROR;

			if (CheckInfoStats == READ_ERROR)					// 수표오류판독
			{
				CheckRejectKind = 1;							// 수표오류판독
				break;
			}
	
			if (m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE);	// 위권모드 : 2003.12.20
			else												// 모의 수표인지를 체크한다.
			{
				for (i = 0 ; i < CheckInfoCnt ; i++)			// 입금수표정보 비교
				{
					if (!memcmp(&CheckInfoData[i], &CheckInfoTemp, sizeof(CheckInfo)))
					{
						CheckInfoStats = READ_ERROR;
						CheckRejectKind = 3;					// 불량수표(기입금수표와동일정보)
						break;									// 동일수표안내 필요시 CheckRejectKind값조정 처리요
					}
				}
			}

			if (CheckInfoStats == READ_ERROR)					// 불량수표확인
			{
				if (!CheckRejectKind)
					CheckRejectKind = 1;				
				break;
			}

			m_pDevCmn->fnUCM_Escrow((LPCTSTR)&CheckPrintData);	// 수표배서및수표Stack
			fnAPD_CheckDeviceAction(DEV_UCM);
			
			memcpy(&CheckInfoData[CheckInfoCnt], &CheckInfoTemp, sizeof(CheckInfo));
			CheckInfoAmt += Asc2Int(CheckInfoData[CheckInfoCnt].Amount, 10);
			CheckInfoCnt++;
			break;

		case READ_ERROR:										// 수표오류판독
			CheckRejectKind = 1;								// 수표오류판독
			break;

		case READ_TIME_OUT:										// 수표투입TimeOver
			break;

		case DOWN:												// 장애발생
			fnAPD_CheckDeviceAction(DEV_UCM);
		default:
			break;
	}

	if (CheckInfoStats == READ_ERROR)							// 수표오류판독
	{
		CheckRejectAFlg = TRUE;									// 입금수표오류반환유

		if (CheckRejectKind == 1)								// 수표오류판독
		{
/////////////////////////////////////////////////////////////////////////////
			CString		strTempDbData("");
			CString		strTemp("");

			// 미디어Read에러
			// OM_MEDIAREADERR||
			strTempDbData = "OM_MEDIAREADERR";

			// 장애발생일(YYYYMMDD)||
			strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			strTempDbData += "||" + strTemp;
			
			// 장애발생시간(HHMMSS)||
			strTemp.Format("%6.6s", GetTime().GetBuffer(0));
			strTempDbData += "||" + strTemp;
			
			// 매체구분(0-카드,1-통장,2-수표,3-IC)
			strTempDbData += "||3";
			
			m_pDevCmn->fnAPL_EjrDbManagerSend(strTempDbData);	// 전자저널자료전송
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

// 입금수표오류반환수취대기
int CTranCmn::fnAPD_CheckDepositCurOutCheck(int ScrNum, int CheckTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositCurOutCheck(ScrNum[%d], CheckTime[%d])", ScrNum, CheckTime);

	int		TimeOverFlg = FALSE;								// 수취타임오버Flg

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)// 입금수표불가시
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_UCM);

	if (CheckRejectAFlg)										// 입금수표오류반환유
	{
		CheckRejectAFlg = FALSE;								// 입금수표오류반환유무

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum);					// 낱장수표반환
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_ON);
		m_pDevCmn->DeviceTimeout &= ~DEV_UCM;					// 시간초과정보초기화
		m_pDevCmn->fnUCM_RejectA(CheckTime);					// 낱장수표반환(수취대기시간)
		fnAPD_CheckDeviceAction(DEV_UCM);
		m_pDevCmn->fnUCM_WaitTaken();							// 수취대기
		fnAPD_CheckDeviceAction(DEV_UCM);
		if (m_pDevCmn->DeviceTimeout & DEV_UCM)					// 미수취
			TimeOverFlg = TRUE;
		else													// 수취정상
			TimeOverFlg = FALSE;
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);

		if (TimeOverFlg)
		{
			m_pDevCmn->fnAPL_StackError("0110493", "입금수표미수취", DEV_UCM);
			m_pDevCmn->DeviceStatus |= DEV_UCM;
			m_pDevCmn->DeviceLoss	|= DEV_UCM;
			fnAPP_CancelProc(T_TIMEOVER);						// 미수취처리
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositCurOutCheck(...):return");
	return T_OK;
}

// 입금수표배서내용편집
// 기업은행은 기기번호와 거래일련번호대신에
// 거래일자를 인자한다.
int CTranCmn::fnAPP_CheckDepositPrintEdit()
{
	memset(CheckPrintData, 0x00, sizeof(CheckPrintData));		// 수표배서내용편집

	memcpy(&CheckPrintData[0], &m_pProfile->TRANS.CheckYYYYMMDD, 8);
	CheckPrintData[8] = '-';									// 날짜
	if ((MenuAvail & MENU_MCU3)	||								// 계좌번호
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
	CheckPrintData[20] = '-';									// 기번

	memcpy(&CheckPrintData[21], &m_pProfile->NETWORK.AtmSerialNum[5], 3);
	CheckPrintData[24] = '-';									// 기번
	AddString(m_pProfile->TRANS.SerialNo, "000001", 6, &CheckPrintData[25]);
																// 거래일련번호

	return T_OK;
}	

// 입금수표수납
int CTranCmn::fnAPD_CheckDepositStore(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositStore(WaitFlg[%d])", WaitFlg);

	int		CrntInCheckCnt = 0;									// 입금수표매수

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)// 입금수표불가시
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_UCM);

	m_pDevCmn->fnUCM_Stack();									// 수표수납

	if (CheckInfoCnt)											// 입금수표내역유
	{
		CrntInCheckCnt = m_pDevCmn->fnAPL_GetNumberOfInCheck();	// 입금수표매수구하기
		CrntInCheckCnt += CheckInfoCnt;							// 수표매수+

		m_pDevCmn->fnAPL_SetNumberOfInCheck(CrntInCheckCnt);	// 입금수표매수설정
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositStore:CrntInCheckCnt[%d]", CrntInCheckCnt);
	}
	
	if (WaitFlg)	
		fnAPD_CheckDeviceAction(DEV_UCM);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositStore():return");
	return T_OK;
}

// 입금수표반환
int CTranCmn::fnAPD_CheckDepositReject()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositReject()");

/////////////////////////////////////////////////////////////////////////////
	CheckRejectFlg = FALSE;										// 입금수표반환유무
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_UCM))
		return T_OK;

	if ((Accept.MoneyInKind & TRAN_DEP_CHECK) != TRAN_DEP_CHECK)// 입금수표불가시
		return T_OK;	

	fnAPD_CheckDeviceAction(DEV_UCM);

	if ((CheckInCountFlg)	||									// 입금수표계수유무
		(m_pDevCmn->fnUCM_GetMaterialInfo(UCM_DEPOSIT)))		// 수표존재
		CheckRejectFlg = TRUE;									// 입금수표반환유
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositReject:CheckRejectFlg[%d], CheckInCountFlg[%d]", CheckRejectFlg, CheckInCountFlg);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositReject():return");
	return T_OK;
}

// 입금수표정보인자
int	CTranCmn::fnAPD_CheckDepositPrint(int PrintDevice, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositPrint(PrintDevice[%x], WaitFlg[%d])", PrintDevice, WaitFlg);

	int		i = 0, js = 0, jj = 0;

	if ((TranProc == TRAN_DEP)					&&				// 입금
		(Asc2Int(Accept.CheckMoney, 8) != 0)	&&				// 입금수표
		(!CheckInPrintFlg));									// 입금수표정보인자체크
	else return T_OK;

	CheckInPrintFlg = TRUE;										// 입금수표정보인자유무
	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역

//                       01234567890123456789012345678901
	sprintf(ShuData[0], "수표번호 은행 점번 계좌번호 종별");

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

	fnAPD_JnlPrint(WaitFlg);									// 저널인자(내부처리)

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDepositPrint(...):return");
	return T_OK;
}

// 현금계수정보인자
int	CTranCmn::fnAPD_CashDepositPrint(int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositPrint(WaitFlg[%d])", WaitFlg);

	int		i = 0, js = 0, jj = 0;
	char	StrBuff[256] = {0,};

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역

	GetTime(StrBuff);
//                       012345678901234567890123
	sprintf(ShuData[0], "%2.2s:%2.2s:%2.2s", &StrBuff[0], &StrBuff[2], &StrBuff[4]);
	sprintf(ShuData[1], "[현금계수결과]");

	sprintf(ShuData[3], "총계수금액:%d", CashData.mTotalAmount);
	sprintf(ShuData[4], "오만원권매수:%d", CashData.mFiftyThousand);
	sprintf(ShuData[5], "만원권매수:%d", CashData.mTenThousand);
	sprintf(ShuData[6], "오천원권매수:%d", CashData.mFiveThousand);
	sprintf(ShuData[7], "천원권매수:%d", CashData.mOneThousand);

	if (CashData.mUnknown > 500)
	{
		CashData.mUnknown = 0;
	}
	sprintf(ShuData[8], "불명권매수:%d", CashData.mUnknown);

	fnAPD_JnlPrint(WaitFlg);									// 저널인자(내부처리)

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CashDepositPrint(...):return");
	return T_OK;
}

// 매체방출후장애시방출재시도	V02-06-02-#05 21405명세표재방출
int CTranCmn::fnAPD_MaterialOutRetryAfterError(int	nDevice)
{
/*
	if ((nDevice == DEV_SPR) &&
		(!m_pDevCmn->fnAPL_CheckError())	&&					// 장애검지
		(memcmp(&m_pDevCmn->CurErrBuff.ErrorCode[0], "21405", 5) == 0))
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "★ 명세표장애검지 : [21405]장애 복구 시작 ");
		if (SprOutRetryFlg == TRUE)
			fnAPP_CancelProc(T_ERROR);

		m_pDevCmn->fnAPL_ClearError();
		m_pDevCmn->DeviceStatus &= ~nDevice; 

		SprOutRetryFlg = TRUE;
		m_pDevCmn->fnSPR_Eject();								// 매체방출
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR);			// 동작완료검지&장애검지
		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_SPR))
			fnAPP_CancelProc(T_ERROR);
		else SprOutRetryFlg = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "★ 명세표장애검지 : [21405]장애 복구 성공 ");
	}
	else
*/
//	#N0157
	if ((nDevice == DEV_SPR) &&
		(!m_pDevCmn->fnAPL_CheckError()))					// 장애검지
	{
		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(nDevice))
		{

/*  #N0157
			if(SprOutRetryFlg) return T_OK;

			m_pDevCmn->fnAPL_ClearError();
			m_pDevCmn->DeviceStatus &= ~nDevice; 

			SprOutRetryFlg = TRUE;
			m_pDevCmn->fnSPR_Eject();								// 매체방출
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR);			// 동작완료검지&장애검지
*/
			if(SprOutRetryFlg) return T_OK;

			SprOutRetryFlg = TRUE;
			
			if(m_pProfile->TRANS.TransPowerOnOff == ON)
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
					m_pDevCmn->fnSCR_DisplayString(1, "[명세표 장애]");
					m_pDevCmn->fnSCR_DisplayString(2, "해당 거래는 완료 되었습니다");
					m_pDevCmn->fnSCR_DisplayString(3, "장애가 발생하여 명세표를 출력할 수 없습니다.");
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
					m_pDevCmn->fnSCR_DisplayString(1, "[명세표 장애]");
					m_pDevCmn->fnSCR_DisplayString(2, "장애가 발생하여 명세표를 출력할 수 없습니다");
					m_pDevCmn->fnSCR_DisplayString(3, "빠른 복구 예정이오니 잠시만 기다려 주십시요.");
			}

			m_pDevCmn->fnSCR_DisplayScreen(701);						// 사용중지
			Delay_Msg(3000);			
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeviceAction(fnAPL_GetAvailErrorDevice):return");
		}
	}

	else
		fnAPP_CancelProc(T_ERROR);

	return TRUE;	
}




////////////////////////////////////////////////////////////////////////////////
//	금융 IC 함수
////////////////////////////////////////////////////////////////////////////////

// 금융 IC카드읽기&체크
int CTranCmn::fnAPD_FICCardReadCheck()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICCardReadCheck()");

	CString strFICISO3Data("");
	CString strFICISO3DataUnPack("");

	if (!MenuFICMode)											// 금융IC거래확인
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

	// 카드데이타분석을 위해 다시 UnPack
	MakeUnPack( strFICISO3Data.GetBuffer(strFICISO3Data.GetLength()), 
				strFICISO3DataUnPack, 
				strFICISO3Data.GetLength());

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICCardReadCheck():strFICISO3DataUnPack[%s]", strFICISO3DataUnPack);

	strFICISO3DataUnPack = ",," + strFICISO3DataUnPack.Mid(2, 104*2);		
																// 1/2트랙정보없음(UnPack데이타이므로 Mid시주의)
	
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICCardReadCheck():strFICISO3DataUnPack[%s]", strFICISO3DataUnPack);
	fnAPD_CardReadCheck(strFICISO3DataUnPack);					// 카드체크
	
	return T_OK;
}

// 금융IC카드 카드구분	2004.02.11
// ATR/FCI확인 - 개방형과 폐쇄형을 따로 처리해야 함
int CTranCmn::fnAPD_FICDistinctCardType(int nReadDF)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICDistinctCardType(%d)", nReadDF);


	if (MenuFICMode & MENU_INFO_1)  //Kim.Gi.Jin 0726
		return T_OK;


	if (!nReadDF)
		nReadDF = TRAN_IC_FINANCEIC;							// 금융IC DF를 Default로 설정


	m_pDevCmn->FICCardType = FIC_CLOSE;							// 기본:폐쇄형카드로 설정

	if (nReadDF == TRAN_IC_KCASH)
	{
		m_pDevCmn->fnKCS_ReadATR();								// ATR읽기
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (m_pDevCmn->fnKCS_AnalRecvData())					// 금융IC카드인경우(개방형-KCASH)
		{
			m_pDevCmn->fnMCU_ICChipInitialize();				// 에러후면 ChipInitialize부터 다시처리
			fnAPD_CheckDeviceAction(DEV_MCU);					// Error후 EP에서 강제 PowerOff가능성있음
																// EP PowerOff없으면 본코드 삭제요망

			m_pDevCmn->fnICA_SelectApplet(nReadDF);
			fnAPD_CheckDeviceAction(DEV_MCU);
			if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
				fnAPP_CancelProc(T_MSG, "이 카드는 사용 할 수 없습니다");

			m_pDevCmn->FICCardType = FIC_OPEN;					// 개방형카드설정
		}
		else
		{
			m_pDevCmn->FICCardType = FIC_CLOSE;					// CLOSE 카드설정
		}
																// 시범사업카드입니다. 거래불가
	}
	else
	if (nReadDF == TRAN_IC_FINANCEIC)
	{
		m_pDevCmn->fnICA_SelectApplet(nReadDF);					// 
		fnAPD_CheckDeviceAction(DEV_MCU);
		if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
			return T_ERROR;
//			fnAPP_CancelProc(T_MSG, "이 카드는 사용 할 수 없습니다");

		m_pDevCmn->FICCardType = FIC_OPEN;						// 개방형카드설정	
	}

	 MenuFICMode |= MENU_INFO_1;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICDistinctCardType():return CardType[%d]", m_pDevCmn->FICCardType);
	return T_OK;
}

/*==============================================================================
* Function Name : fnAPD_FICBasicInquiry()
* Description   : 금융 IC 정보를 조회하는 함수(모든 거래시 공통)
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

	if (!MenuFICMode)											// 금융IC거래확인
		return T_ERROR;

	/* ------------------------------------------------------------------------
	// fnAPD_FICDistinctCardType(.)에서 SelectApplet가 실행됨
	m_pDevCmn->fnICA_SelectApplet(TRAN_IC_FINANCEIC);
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
		fnAPP_CancelProc(T_CANCEL, NULL, NULL, NULL, 599);
	------------------------------------------------------------------------ */

	m_pDevCmn->fnFIC_ReadDualInfo(0x00);						// 기본 정보 및 계좌 정보 조회
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_READDUALINFO) )
		m_pDevCmn->FICAccountCount = 0;
//		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");

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
* Description   : MyBi 금융 IC의 경우 거래하는 함수
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.01.27 Han Su Woong
* ============================================================================*/
int CTranCmn::fnAPD_FICMyBiProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICMyBiProc()");


	if (!MenuFICMode)											// 금융IC거래확인
		return T_ERROR;

	char Password[5];
	memset(Password, 0x00, sizeof(Password));
	memcpy(Password, Accept.PassWord, 4);

	// Get ENC Data 1
//	m_pDevCmn->fnFIC_GetENCData(0x01, Accept.PassWord);
	m_pDevCmn->fnFIC_GetENCData(0x01, Password);
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_GETENCDATA1) )
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");

	// GetChallenge
	m_pDevCmn->fnFIC_GetChallenge();
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_GETCHALLENGE) )
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");

	// Get ENC Data 2
	m_pDevCmn->fnFIC_GetENCData(0x02, Password);
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_GETENCDATA2) )
		fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICMyBiProc():return");
	return T_OK;
}

// 금융IC카드 (계좌)비밀번호 암호화
// 카드난수 + 암호화된 비밀번호 조회
int CTranCmn::fnAPD_FICGetEncipher()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICGetEncipher(..)");


	if ((!MenuFICMode)	&&										// 금융IC거래확인
		(!MenuEPBMode)	)										// 잔자통장거래확인
		return T_ERROR;

	//m_pDevCmn->fnFIC_GetEncipher(Accept.PassWord, sizeof(Accept.PassWord));
	m_pDevCmn->fnFIC_GetEncipher(Accept.PassWord, 4);
	fnAPD_CheckDeviceAction(DEV_MCU);
	if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_GETENCIPHER) )
	{
		// 여기서 에러시는 마이비 IC.
		// MyBi IC 처리
		// History : EP설계상 에러시 자동 Power Off함...ㅠ.ㅠ
		////////////////////////////////////////////////////////////////////////
		m_pDevCmn->fnMCU_ICChipInitialize();
		fnAPD_CheckDeviceAction(DEV_MCU);
		
		m_pDevCmn->fnICA_SelectApplet(TRAN_IC_FINANCEIC);
		fnAPD_CheckDeviceAction(DEV_MCU);
		if ( m_pDevCmn->fnFIC_AnalRecvData(FINANCEIC_PROC_SELECTAPPLICATION) )
			fnAPP_CancelProc(T_MSG, "카드를 읽는 중 오류가 발생하였습니다.", "[상황실: 1577 - 4655] 문의하여 주십시오.");

		// 여기를 제외하고 정상 동작한다면 삭제해도 됨.
		////////////////////////////////////////////////////////////////////////

		fnAPD_FICMyBiProc();

		FICMyBiFlg = TRUE;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICGetEncipher():return");
	return T_OK;
}

// 금융IC host송신데이타 조회
// 서버난수(32) + 카드난수(32) + 암호화비밀번호(32)
int CTranCmn::fnAPD_FICGetHostTranData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICGetHostTranData(..)");

	if (!MenuFICMode)											// 금융IC거래확인
		return T_ERROR;

	m_pDevCmn->fnFIC_GetHostTranData(FICMyBiFlg);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_FICGetHostTranData():return");
	return T_OK;
}

//───────────────────────────────────────
// EMV FUNCTION	DEFINITION										V03-00-25-#02
//───────────────────────────────────────
/*==============================================================================
* Function Name : fnAPP_EMVInitProc()
* Description   : EMV 관련 Buffer를 초기화하는 함수(기기 Start시 1회 호출)
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
* Description   : 개국시(또는 인증시 Configuratin Code Setting시) EMV 정보를 세팅
* Parameters    : ConfigCode : 인증시 세팅하는 환경 설정 값의 Index.
*                              개국시에는 인수 없이 호출. 기본인수 = 1.
* Return        : Success : T_OK
*                 Fail    : T_ERROR - 이후 EMV 거래를 지원하지 않음.
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CTranCmn::fnAPP_EMVOpenSetProc(int ConfigCode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_EMVOpenSetProc() ConfigCode = %d", ConfigCode);
	int		nAIDList;
	int		nRet(T_ERROR), nReturn(T_ERROR);

	if(m_pDevCmn->IfmDirect)									// V09-33-00-#01 ICCM 모듈 적용
		return TRUE;
	
#if EMV_TA_MODE													// 인증 모드
	if ( m_pDevCmn->EMVInfo.ConfigCode == NULL )				// 최초 개국시
	{
		//**********************************************************************
		//  CA 공개키 List Set Process
		//**********************************************************************
//		m_pDevCmn->EMVInfo.CAPKRIDIndex = EMV_CAPKLISTRID_CNT;
//		m_pDevCmn->EMVInfo.CAPKKeyIndex = EMV_CAPKLISTKEY_CNT;
//		
//		EMV_DefaultCAPKListSet();								// Default 지원 CA 공개키 List Set
//		for ( int i = 0; i < EMV_CAPKLISTRID_CNT * EMV_CAPKLISTKEY_CNT; i++ )
//		{
//			nRet = EMV_CAListSet(i);							// CA 공개키 List Set
//			if ( nRet != T_OK )
//				return nReturn;
//		}

		//**********************************************************************
		//  Terminal List(AID List) Set Process
		//**********************************************************************
		// Default 지원 AID List Set
		nRet = m_pDevCmn->fnEMV_DefaultAIDListSetINI();
		if ( nRet != TRUE )
		nAIDList = m_pDevCmn->fnEMV_DefaultAIDListSet();
		
		if ( nAIDList != m_pDevCmn->EMVInfo.AIDListCount )		// 기본 AID 세팅시 Error
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
		// 이전 세팅값과 현재값을 비교한다. 같다면 아무일도 하지 않는다.
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
	if ( m_pDevCmn->EMVInfo.ConfigCode == NULL )				// 최초 개국시
	{
		// 운영시에는 CA 공개키 List Set을 하지 않는다.
		// 아직 ATM에서는 Offline Data Authtication(SDA, DDA)가 없다.
//**************************************************************************
// 최초 개국시 EMV.ini의 정보를 세팅한다. EP로의 전달은 SP에서 한다.
// (제일은행 사양)
//**************************************************************************
		//**********************************************************************
		//  Terminal List(AID List) Set Process
		//**********************************************************************
		// Default 지원 AID List Set
		nRet = m_pDevCmn->fnEMV_DefaultAIDListSetINI();
		if ( nRet != TRUE ) {
			nAIDList = m_pDevCmn->fnEMV_DefaultAIDListSet();
			if ( nAIDList != m_pDevCmn->EMVInfo.AIDListCount )	// 기본 AID 세팅시 Error
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
* Description   : EMV Application Read 함수
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.11.03
* ============================================================================*/
int	CTranCmn::fnAPP_EMVReadProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_EMVReadProc()");

//	if (!m_pDevCmn->EarPhoneInserted)	// 장애인ATM 동일화면 처리로 음성 반복하는 경우 처리 확인하기!!
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
			return T_ERROR;											// FallBack지원
		}										// FallBack지원
	}

	if (fnAPD_EMVBrandChoice() != T_OK)
	{
		EMVReadFlg = FALSE;
		return T_ERROR;											// FallBack지원	
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
* Notes			: V09-33-00-#01		ICCM 모듈적용
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
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():이동함m_pDevCmn->fnEMV_ReadAppData():nResult(%d)", nResult);
	if((nResult == EMV_ERR_NOT_SUPPORT)	||(nResult == EMV_ERR_CARD_SW) )
	{
		CardTran &= ~MENU_IC;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_ReadAppData():nResult=EMV_ERR_FALLBACK!!");
		return fnAPLLib_EmvFallBackProc(TRUE);
	}
	else if(nResult != EMV_RSLT_OK)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPLLib__EmvTranDataMake():m_pDevCmn->fnEMV_ReadAppData():nResult=Error(%d)", nResult);
		return fnAPP_CancelProc(T_MSG, "이 카드로는 본거래를 할 수 없습니다.");
	}
	
	fnAPLLib_EmvMakeCardPanProc();

    // MS Track 2 Image 전문
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
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "제휴카드는 MS로 거래!!");
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
* Description   : EMV 온라인 후 거래 처리 함수
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
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_EMVAfterOnlineTranProc() 카드 인증 실패!");
			
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("ARPC 오류 : 거래일련번호(%12.12s)", m_RD.byDataField_026x));
			if (TranProc != TRAN_INQ)
			{
				EMVAuthError = TRUE;
				TranCode2 = TRANID_A020;		
				fnAPP_SendHost();
				fnAPP_RecvHost();
				fnAPP_CancelProc(T_MSG, "카드 인증 실패!",  "거래를 취소합니다");
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
* Description   : 조회 후 거래 등의 연속 거래를 지원을 위한 전문 재작성 
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
	int nSelected = 0;							// V09-33-00-#01		ICCM 모듈적용

	if (BrandSelectIndex < 1)
		return T_ERROR;
	else
		nIndex = BrandSelectIndex;

	// 전원 재공급
	m_pDevCmn->fnKCS_PowerOff();
	fnAPD_CheckDeviceAction(DEV_MCU);
	m_pDevCmn->fnKCS_PowerOn();
	fnAPD_CheckDeviceAction(DEV_MCU);


	if (fnAPD_EMVSelectProc() != T_OK)
	{
		fnAPP_CancelProc(T_MSG, " 이 카드로는 본거래를 할 수 없습니다",
								" 창구로 문의하여 주시기 바랍니다. "); 
		return T_ERROR;
	}

	// 선택된 EMV AID를 선택
	m_pDevCmn->fnEMV_SelectApplication(nIndex);
	fnAPD_CheckDeviceAction(DEV_MCU);
	nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_SELECTAPPLICATION);

	if (nRet != T_OK)	// Index는 1부터 시작함을 주의
	{
		fnAPP_CancelProc(T_MSG, " 이 카드로는 본거래를 할 수 없습니다",
								" 창구로 문의하여 주시기 바랍니다. "); 
		return T_ERROR;
	}

	//**************************************************************************
	//  Read Application : 거래를 위해 Application의 정보를 수집
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

		fnAPP_CancelProc(T_MSG, " 이 카드로는 본거래를 할 수 없습니다",
						  " 창구로 문의하여 주시기 바랍니다. "); 
		return T_ERROR;
	}
	
	return T_OK;
}

/*==============================================================================
* Function Name : fnAPD_EMVSelectProc()
* Description   : EMV Application을 확인하는 함수
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
	m_pDevCmn->fnEMV_SelectPSE();								// EMV Application 확인
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
	
	m_pDevCmn->fnEMV_ReqCandidateList();						// EMV 후보 목록 조회
	fnAPD_CheckDeviceAction(DEV_MCU);
	nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_REQCANDIDATELIST);

	if (nRet != T_OK)
	{
		if ( nRet == EMV_RESULT_INITDATAERROR )					// #N0268 FB율 개선 EMV_FLAG_INITDATAERROR -> EMV_RESULT_INITDATAERROR
		{
			// 2015.01.19 LDC 이 부분은 초기설정 재전송 후 현재 거래는 Fallback처리 한다.

			// V09-47-00-#07 FB율 개선 Start
			if (fnAPD_EMVTerminalConstant() != T_OK)					// 다음 거래의 FB 방지
			{
				EMVReadFlg = FALSE;
				return nRet;											// FallBack지원
			}
			// V09-47-00-#07 FB율 개선 End

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

// V09-47-00-#07 FB율 개선 Start
/*==============================================================================
* Function Name : fnAPD_EMVTerminalConstant()
* Description   : EMV TerminalConstant 설정
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
* Description   : EMV Application 중 거래할 Brand를 선택하는 함수.
*                 만약 거래 가능한 Brand가 하나이고 화면에 표시할 의무가 없다면
*                 (AIP가 0x80이 아니라면) 표시 없이 다음으로 진행한다.
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
	//  후보 목록 중 거래할 Application 선택
	//**************************************************************************
	nBrandCnt = m_pDevCmn->fnEMV_GetBrandCount();				// Brand 갯수 얻어오기
	// 만약 nBrandCnt가 1개이고, 그 어플리케이션의 API가 0x80이 아니면 자동 선택한다.
	/*
	if ((nBrandCnt == 1)							&&
		(m_pDevCmn->EMVCandidateList[0].API != 0x80))
	{
		nIndex = 1;
		BrandSelectIndex = 1;									// 무조건 1 Brand
		
		//**********************************************************************
		//  Select Application : 선택된 Application으로 거래 시작
		//**********************************************************************
		m_pDevCmn->fnEMV_SelectApplication(nIndex);				// Index는 1부터 시작함을 주의
		fnAPD_CheckDeviceAction(DEV_MCU);
		nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_SELECTAPPLICATION);
		if (nRet != T_OK)
		{
			if ((memcmp(m_pDevCmn->m_szEMVFBCode, EMV_FBCODE_NOFALLBACK, 6) == 0 ) ||
				(memcmp(m_pDevCmn->m_szEMVFBCode, "      ", 6) == 0 )				)
				fnAPP_CancelProc(T_MSG, "이 카드로는 본거래를 할 수 없습니다");
			else
				return T_ERROR;
		}
	}
	else														// 리스트 별 화면 표시
	{
		for (i = 0; i < nBrandCnt; i++)
		{
			nRet = fnAPD_EMVSelectBrand();
			if (nRet == T_OK)									// 성공
				break;											// 정상 진행
			else
			if (nRet == EMV_RESULT_REVERSAL)					// 후보 리스트 재 표시
				continue;
			else
				return nRet;
		}
	}
	*/
	if (m_pDevCmn->EMVCandidateList[0].API != 0x80)
	{
		nIndex = 1;
		BrandSelectIndex = 1;									// 무조건 1 Brand
		
		//**********************************************************************
		//  Select Application : 선택된 Application으로 거래 시작
		//**********************************************************************
		m_pDevCmn->fnEMV_SelectApplication(nIndex);				// Index는 1부터 시작함을 주의
		fnAPD_CheckDeviceAction(DEV_MCU);
		nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_SELECTAPPLICATION);
		if (nRet != T_OK)
		{
			if ((memcmp(m_pDevCmn->m_szEMVFBCode, EMV_FBCODE_NOFALLBACK, 6) == 0 ) ||
				(memcmp(m_pDevCmn->m_szEMVFBCode, "      ", 6) == 0 )				)
				fnAPP_CancelProc(T_MSG, "이 카드로는 본거래를 할 수 없습니다");
			else
				return T_ERROR;
		}
	}
	m_pDevCmn->fnEMV_ReadApplication();							// 거래를 위해 Application의 정보를 수집
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

	// MS Track 2 Image 전문
	char szISO2[100] = {0x00,};
	
	m_pDevCmn->fnEMV_GetISO2(szISO2);

	CString strISO2("");

	strISO2  = ",";												// 1Track
	strISO2 += MakeUnPack(szISO2, strlen(szISO2));				// 2Track
	strISO2 += ",";												// 3Track

	fnAPD_CardReadCheck(strISO2);


	if (fnAPD_EMVJaTaCheck() == FALSE)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "제휴카드는 MS로 거래!!");
//		if ((Accept.DCC.BrandType[0] == 'M') &&			//V02-02-00-#18
//			(Accept.DCC.ICType == 'Y'))
//			return T_ERROR;
//		else
//			return T_OK;								//V02-02-00-#18
	}
	
	return T_OK;
}

// 자타행구분 임시함수	//HNBV01-00-10 #1(EMV)
int CTranCmn::fnAPD_EMVJaTaCheck()
{
	BOOL    SeoulBinCheck = FALSE;	
	
//	CardTran &= (~MENU_J2);
//	CardTran &= (~MENU_T2);
//	SeoulDebitCard = FALSE;
/*
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV용 카드 자타행 구분 EMV_JaTa() IN");
HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVJaTaCheck EMV_CardData.ISO2Buff", CardData.ISO2Buff, sizeof(CardData.ISO2Buff));
	if ((CardData.ISO2size < 6)									||
		(CardData.ISO2size > sizeof(CardData.ISO2Buff))			||
		(FindChar(CardData.ISO2Buff, CardData.ISO2size, NULL) != CardData.ISO2size)
																||
		(IsZero(CardData.ISO2Buff, 6))							||
		(IsChar(CardData.ISO2Buff, 6, '='))){
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV용 카드CardData.ISO2size11");			
	}
	else
	{
		if (memcmp(pCardData2->MembershipNo, BUYONLY, 6) == 0)
		{												// 구매전용카드(하나은행:카드반각)
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EMVJaTaCheck_CARD2_MENU_T2",2);
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() == HAN_MODE)
				fnAPP_CancelProc(T_MSG, "구매 전용 카드 입니다",  "이 카드는 사용 할 수 없습니다");
			else
				fnAPP_CancelProc(T_MSG, "This card is not valid");
		}
//				else
// 직불카드 예외처리 없슴 -HNB10011
//				if ((memcmp(pCardData2->MembershipNo, DEBIT, 4) == 0)	||
//					(memcmp(pCardData2->MembershipNo, DEBIT2, 4) == 0)	||
//					(memcmp(pCardData2->MembershipNo, DEBIT3, 4) == 0));
//																// 직불카드(2TR카드무처리)

		else
		if ((memcmp(pCardData2->MembershipNo, VISAJJ, 6) == 0)		||
			(memcmp(pCardData2->MembershipNo, VISAJJ2, 6) == 0)		||
			(memcmp(pCardData2->MembershipNo, VISAJJ3, 6) == 0)		||
			(memcmp(pCardData2->MembershipNo, VISAJJ6, 6) == 0)		||		//20030404
			(memcmp(pCardData2->MembershipNo, VISAJJ7, 6) == 0)		||		// V01.00.29
			(memcmp(pCardData2->MembershipNo, VISAJJ12, 6) == 0)	||		// V06-23-00-#02 하나SK카드 BIN번호 추가
			(memcmp(pCardData2->MembershipNo, BCJJ18, 6) == 0)		||		// V01.00.29
			(memcmp(pCardData2->MembershipNo, BCJJ19, 6) == 0)		||		// V01.00.29
			(memcmp(pCardData2->MembershipNo, BCJJ20, 6) == 0)		||		// V01.00.29
			(memcmp(pCardData2->MembershipNo, BCJJ21, 6) == 0)		||		// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ22, 6) == 0)		||		// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ23, 6) == 0)		||		// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ24, 6) == 0)		||		// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ25, 6) == 0)		||		// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ26, 6) == 0)		||	  	// V05-00-07_#01
			(memcmp(pCardData2->MembershipNo, BCJJ27, 6) == 0)		||	  	// V06-23-00-#02 하나SK카드 BIN번호 추가
			(memcmp(pCardData2->MembershipNo, BCJJ28, 6) == 0)		||	  	// V06-23-00-#02 하나SK카드 BIN번호 추가
			(memcmp(pCardData2->MembershipNo, BCJJ29, 6) == 0)		||	  	// V06-23-00-#02 하나SK카드 BIN번호 추가
			(memcmp(pCardData2->MembershipNo, BCJJ30, 6) == 0)		||	  	// V06-23-00-#02 하나SK카드 BIN번호 추가
			(memcmp(pCardData2->MembershipNo, BCJJ31, 6) == 0)		||	  	// V06-26-01-#01 하나SK카드 BIN번호 추가
			(memcmp(pCardData2->MembershipNo, BCJJ32, 6) == 0)		||	  	// V06-29-00-#02 하나SK카드 BIN번호 추가
			(memcmp(pCardData2->MembershipNo, BCJJ33, 6) == 0)		||	  	// V06-29-00-#02 하나SK카드 BIN번호 추가
			(memcmp(pCardData2->MembershipNo, MASTERJJ1, 6) == 0)	||  	// V05-02-04-#03 
			(memcmp(pCardData2->MembershipNo, MASTERJJ2, 6) == 0)	||      // V05-02-04-#03 
			(memcmp(pCardData2->MembershipNo, MASTERJJ3, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ4, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ5, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ6, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ7, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ8, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ9, 6) == 0)	||      // V06-08-02-#04 
			(memcmp(pCardData2->MembershipNo, MASTERJJ10, 6) == 0)	||      // V06-23-00-#02 하나SK카드 BIN번호 추가
			(memcmp(pCardData2->MembershipNo, AMEXJJ1, 6) == 0)		||      // V05-02-04-#03 
			(memcmp(pCardData2->MembershipNo, AMEXJJ2, 6) == 0)		||		// V05-02-04-#03
			(memcmp(pCardData2->MembershipNo, LOCALJJ1, 6) == 0)	||		// V05-02-04-#03 
			(memcmp(pCardData2->MembershipNo, HANABIN1, 6) == 0)	||      // V05-02-17-#02 
			(memcmp(pCardData2->MembershipNo, HANABIN2, 6) == 0)	||		// V05-02-17-#02
			(memcmp(pCardData2->MembershipNo, HANABIN3, 6) == 0)	||		// V05-02-17-#02
			(memcmp(pCardData2->MembershipNo, HANABIN4, 6) == 0)	||		// V05-06-01-#01		  	  					
			(memcmp(pCardData2->MembershipNo, HANABIN5, 6) == 0)	)		// V06-15-00-#02 하나SK카드 Bin코드 추가
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV용 카드CardData.ISO2size22");
			CardTran |= MENU_J2;						// 자행2TR
			CardService = 1;							// 자행비자(하나은행)
		}
		else
		if ((memcmp(pCardData2->MembershipNo, VISAJJ4, 6) == 0)	||
			(memcmp(pCardData2->MembershipNo, VISAJJ5, 6) == 0))
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV용 카드CardData.ISO2size33");
			CardTran |= MENU_J2;						// 자행2TR
			CardService = 2;							// 자행비자(보람은행)
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
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV용 카드CardData.ISO2size44");
			CardTran |= MENU_J2;								// 자행2TR
			CardService = 3;									// 자행비씨(충청은행)
		}
		else 
		{
			if (memcmp(pCardData2->MembershipNo, SEOULDEBITID, 6) == 0)				// V01.01.12_#2 // 02.10
			{
				SeoulDebitCard = TRUE;
			}
		
			for (int i=0;i<21;i++)								//20030402 서울BC카드
			{
				if (memcmp(pCardData2->MembershipNo, SeoulBCBinCheck[i], 6) == 0)
					SeoulBinCheck = TRUE;
			}

			if (SeoulBinCheck)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV용 카드CardData.ISO2size55");
				CardService = 4;								// 서울은행
				CardTran |= MENU_J2;							// 타행2TR
			}
			else
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV용 카드CardData.ISO2size66");
				for (int j =0; j<25; j++)					// V06-33-00-#03 Bin 24->25
				{
					if (memcmp(pCardData2->MembershipNo, TaHangBIN_Table[j], 6) == 0)
					{
						CardTran &= ~MENU_JJ;
						CardTran |= MENU_T2;				// 타행2TR
						CardService = 0;
					}
				}
						
				if (CardTran & MENU_T2);
				else
				if (fnAPD_CheckBinCode(GetString(pCardData2->MembershipNo, 6, 6).GetBuffer(0)))
				{
					CardTran |= MENU_J2;						// 자행2TR
					CardService = 5;							// 자행비자(하나은행)
				}
				else
					CardTran |= MENU_T2;						// 타행2TR
			}
		}
	}
*/
	if (CardTran & MENU_J2)
		MenuAvail &= ~MENU_T2;
	else
		MenuAvail &= ~MENU_J2;

	// 서비스(결제계좌이체), 비자결제대금조회는 자행만 가능함
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV_JaTa에서 Trancode = [%d]", TranCode);
	if ((TranCode == 15) ||
		(TranCode == 14)) 
	{
		if ((CardTran & MENU_T2)	||
			(CardService == 5)		)
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				fnAPP_CancelProc(T_MSG,"이 카드로는 본 거래를 할수 없습니다");
			else
				fnAPP_CancelProc(T_MSG, "Your card can not", "be used at this transaction");
		}
	}

	return T_OK;
}

/*==============================================================================
* Function Name : fnAPD_EMVSelectBrand()
* Description   : 거래할 EMV Application이 복수개 일 때 고객의 선택을 받는 함수
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

	nBrandCnt = m_pDevCmn->fnEMV_GetBrandCount();				// Brand 갯수 얻어오기
	
	// 화면 버튼 Disable
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
		if (strcmp(m_pDevCmn->EMVCandidateList[i].Brand, "국내 전용 신용/Check") == 0)
			m_pDevCmn->fnSCR_DisplayString(4, "국내 전용 신용/Check");
		else
		if (strcmp(m_pDevCmn->EMVCandidateList[i].Brand, "국내 금융공동망") == 0)
			m_pDevCmn->fnSCR_DisplayString(4, "국내 금융공동망");
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
	if (strReturn == "국내전용")
		strcpy( szTemp, "국내 전용 신용/Check" );
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
	if (strReturn == "금융IC")
		strcpy( szTemp, "국내 금융공동망" );
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);

	// 선택된 Application 찾기
	for (i = 0; i < nBrandCnt; i++)
	{
		if (strcmp(szTemp, m_pDevCmn->EMVCandidateList[i].Brand) == 0)
		{
			nIndex = i + 1;										// Index는 1부터 시작함을 주의
			break;
		}
	}

	BrandSelectIndex = nIndex;	
	//**************************************************************************
	//  Select Application : 선택된 Application으로 거래 시작
	//**************************************************************************
	m_pDevCmn->fnEMV_SelectApplication(nIndex);					// Index는 1부터 시작함을 주의
	fnAPD_CheckDeviceAction(DEV_MCU);
	nReturn = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_SELECTAPPLICATION);

	return nReturn;
}

/*==============================================================================
* Function Name : fnAPD_EMVSendMakeProc()
* Description   : EMV 거래를 위한 온라인 송신 전처리 함수
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
	// OFFLINE AUTHENTICATION 요구
	m_pDevCmn->fnEMV_OfflineDataAuthentication( Accept.Money,
					sizeof(Accept.Money), AnotherMoney, sizeof(AnotherMoney) );
	fnAPD_CheckDeviceAction(DEV_MCU);
	nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_OFFLINEDATAAUTHEN);
	if (nRet != T_OK)
	{
		if (nRet == EMV_RESULT_ONLINEDECLINE)
		{
			fnAPD_EMVResultProc();
			fnAPP_CancelProc( T_MSG, "이 카드로는 본거래를 할 수 없습니다. #1" );
		}
		else
		if (nRet == EMV_RESULT_TERMINATE)
		{
			fnAPD_EMVResultProc();
			fnAPP_CancelProc( T_MSG, "이 카드로는 본거래를 할 수 없습니다. #2" );
		}

		//**********************************************************************
		// EMV 카드 상태 Check : CID의 LSB를 검사
		// +----+----+----+----+-----------------------------------------------+
		// | b4 | b3 | b2 | b1 |                   설    명                    |
		// +----+----+----+----+-----------------------------------------------+
		// |  1 |    |    |    |                                통지가 필요함  |
		// +----+----+----+----+-----------------------------------------------+
		// |    |  0 |  0 |  0 |                                  정보가 없음  |
		// |    |  0 |  0 |  1 |                       서비스가 허용되지 않음  |
		// |    |  0 |  1 |  0 |                 PIN 시도 제한횟수를 초과했음  |
		// |    |  0 |  1 |  1 |                     발급사 인증이 실패하였음  |
		// |    |  X |  X |  X |                                          RFU  |
		// +----+----+----+----+-----------------------------------------------+
		//**********************************************************************
		BYTE bFlag;
		char StrBuff[81] = {0};
		
		bFlag = m_pDevCmn->fnEMV_GetCID9F27() & 0x0F;			// CID Error Check
		if (bFlag)												// Flag가 있다면 EMV 장애
		{
			switch (bFlag)
			{
			case 0x01 :											// 서비스가 허용되지 않음
				strcpy( StrBuff, "이 카드로는 이 서비스를 이용하실 수 없습니다" );
				break;
			
			case 0x02 :											// PIN 시도 제한횟수를 초과했음
				strcpy( StrBuff, "PIN 입력 횟수를 초과하였습니다" );
				break;
			
			case 0x03 :											// 발급사 인증이 실패하였음
				strcpy( StrBuff, "카드 발급사로부터 인증이 실패하였습니다" );
				break;
			
			default :
				strcpy( StrBuff, "이 카드로는 본거래를 할 수 없습니다. #3" );
				break;
			}
				
			fnAPD_EMVResultProc();

			if ( bFlag & 0x08 )									// 은행 또는 발급사로 통지 필요
				fnAPP_CancelProc( T_MSG, StrBuff, "창구로 문의하여 주시기 바랍니다" );
			else
				fnAPP_CancelProc( T_MSG, StrBuff);
		}
		else
		{
			memcpy(m_pDevCmn->m_szEMVFBCode, EMV_FBCODE_AFTER_OFFAUTHERR, sizeof(EMV_FBCODE_AFTER_OFFAUTHERR));
			fnAPP_CancelProc( T_MSG, "이 카드로는 본거래를 할 수 없습니다. #4" );
		}

		return nReturn;
	}

	//**************************************************************************
	//  EMV 관련 데이터 얻어오기
	//**************************************************************************
	m_pDevCmn->fnEMV_GetHostTranData(EMV_SENDHOST_ARQC);

	return T_OK;
}

/*==============================================================================
* Function Name : fnAPD_EMVAfterRecvProc()
* Description   : 온라인 후 거래 처리 함수
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
	// 이 세 개의 변수는 AP 담당자와 상의하여 정한다.
	char	szHostData[512] = {0x00,};
	BOOL	bHostRecvFlag(TRUE);
	BYTE SixteenNulls[17];
	BYTE SixteenZeros[17];
		
	memset(SixteenNulls, 0x00, sizeof(SixteenNulls));
	memset(SixteenZeros,  0x30, sizeof(SixteenZeros));

	// #H0248 ARPC 인증오류 승인  ────────────
	// ARPC가 all zero이면 ARPC 인증을 생략한다
	// M/Chip4, D-PAS, Local M/Chip은 ARPC가 20바이트지만 일단 16자리만 비교하는 것으로....
	if (memcmp(m_RecvEmvrcvData.ARPC, SixteenZeros, 16) == 0)
	{
MsgDump(MAC_TRACE_CODE_FILE("Log"), "◎◎◎◎ 한시적으로 ARPC 인증 생략하고 승인 처리!!! ◎◎◎◎");
		return T_OK;
	}
	// ─────────────────────────────


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
	//  Online Processing : 온라인 승인 요청에 대한 호스트 응답을 카드로 전달
	//**************************************************************************
	if(m_pDevCmn->IfmDirect)									// V09-33-00-#01 ICCM 모듈 적용
		nRet = fnAPDLib_OnlineProcessing( szHostData, m_pDevCmn->TranResult, bHostRecvFlag );
	else
	{
		m_pDevCmn->fnEMV_OnlineProcessing( szHostData, m_pDevCmn->TranResult, bHostRecvFlag );
		fnAPD_CheckDeviceAction(DEV_MCU);
		nRet = m_pDevCmn->fnEMV_AnalRecvData(EMV_PROC_ONLINE);
	}

	if (nRet == EMV_RESULT_REVERSAL)
	{
		// 취소 거래로 설정
		nReturn = EMV_RESULT_REVERSAL;	 

		nRet = fnAPP_EMVReversalProc();							// 취소 거래 송수신
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

	// V09-33-00-#01 ICCM 모듈 적용
	// 수출 EMV Lib를 사용하는 경우는 아래 처리가 필요 없다.
	if(m_pDevCmn->IfmDirect)
		return nReturn;

	//**************************************************************************
	//  Completion Processing : 카드로 거래 종료를 요청
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
		nRet = fnAPP_EMVReversalProc();							// 취소 거래 송수신
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
	//  AfterScript Processing : 거래 종료 후 Issuer Script 처리
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
//			fnAPP_CancelProc(T_MSG, "온라인 승인이 거부되었습니다"); //#H0248
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
* Description   : 취소거래 처리 함수
* Parameters    : (NONE)
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CTranCmn::fnAPP_EMVReversalProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_EMVReversalProc()");

	int		nReturn(T_ERROR);

	// MS Track 2 전문
	// 취소 거래시 이전 MS 2 Track 정보를 사용한다면 이 부분은 생략해도 무방하다.
	char szISO2[100] = {0x00,};
	memset( &CardData.ISO2Buff, 0, sizeof(CardData.ISO2Buff) );			// 초기화 HNBV010010 #1(EMV) DATA지우기
	
	CardData.TrInfo |= ISO2_TRACK;
	CardData.ISO2size = m_pDevCmn->fnEMV_GetISO2(szISO2);
	memcpy( CardData.ISO2Buff, szISO2, CardData.ISO2size );

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CardData.ISO2Buff", CardData.ISO2Buff, sizeof(CardData.ISO2Buff));
	fnAPDLib_EMVSendMakeProc(2);								// V09-33-00-#01	ICCM 모듈 적용	
	m_pDevCmn->fnEMV_GetHostTranData(EMV_SENDHOST_REVERSAL);	// kh 20041007


	return TRUE;
}

/*==============================================================================
* Function Name : fnAPD_EMVResultProc()
* Description   : EMV 거래 결과를 조회하는 함수(인증용 & 디버깅용)
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

	return T_OK;												// 결과를 알기 위한 부가적인 함수로 에러처리 하지 않음
}

// 카드연속거래처리
int CTranCmn::fnAPD_CardReadSerial()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadSerial()");
	m_pDevCmn->fnAPL_CaptureSaveEnd();				
	if ((!MenuFICMode)	&&										// 금융IC
		(!MenuEMVMode)	&&										// EMV
		(!MenuKCashMode))
		return T_OK;

	if ((MenuFICMode)	||										// 금융IC
		(MenuEMVMode)	||										// EMV
		(MenuKCashMode)	)										// 전자화폐
	{
		if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 823)
			m_pDevCmn->fnSCR_DisplayScreen(823);
		DevEnDisable &= (~DEV_MCU);
		fnAPD_CardEnDisable(ENABLE, TRUE, TRUE, TRUE);			// 금융IC/K-CASH 사용가능 V03-00-22(LSY) 2004.10.04
		fnAPD_CheckDeviceAction(DEV_MCU);
		if (fnAPD_ICCardReadCheck() == T_ERROR)					// IC카드읽기	V03-00-22 2004.10.14
			fnAPP_CancelProc(T_MSG, "이 카드는 사용할 수 없습니다.",
									"카드정보를 확인 하시기 바랍니다.");

		if (MenuFICMode)
		{
			fnAPD_FICDistinctCardType(TRAN_IC_FINANCEIC);
			fnAPD_FICBasicInquiry();						// 기본정보조회
		}
		else
		if (MenuEMVMode)
		{
			if (fnAPD_EMVContinueTrans() == T_ERROR)			// EMV기본정보READ
			{
				CardPowerOnFlg = FALSE;							// 카드접속무
				fnAPD_CardEnDisable(ENABLE, TRUE, TRUE, TRUE);	// IC카드읽기 실패後 카드엔트리무조건 발행 2004.10.26
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
	
	if(lrc >= 10) //':'보다 크면 10으로 나눈다
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

// U8100-AP변경 #01 - 3Camera, Preview지원, 텍스트출력
CString CTranCmn::fnAPP_GetCaptureData()
{
	CString szTempData("");
	
	if (m_pDevCmn->AtmDefine.CashHandler != HBRM_SET)
		;
	else
	{
		if (Asc2Int(Accept.Money, 8) > 0)
		{
			if (!AddSerialFlg)							// 입금송신전등 
				szTempData.Format("%1.1c0%1.1c0%1.1cNO:%6.6s(+1) ACN:%16.16s AMT:%8.8s", 
				0x1B, 0x1C, 0x1D, m_pProfile->TRANS.SerialNo, fchpAPP_EditAcnt(FALSE, TRUE), Accept.Money);
			else										// 거래일련번호, 계좌번호, 금액
				szTempData.Format("%1.1c0%1.1c0%1.1cNO:%6.6s ACN:%16.16s AMT:%8.8s", 
				0x1B, 0x1C, 0x1D, m_pProfile->TRANS.SerialNo, fchpAPP_EditAcnt(FALSE, TRUE), Accept.Money);
		}
		else
		{
			if (!AddSerialFlg)
				szTempData.Format("%1.1c0%1.1c0%1.1cNO:%6.6s(+1) ACN:%16.16s ", 
					0x1B, 0x1C, 0x1D, m_pProfile->TRANS.SerialNo, fchpAPP_EditAcnt(FALSE, TRUE));
			else
				szTempData.Format("%1.1c0%1.1c0%1.1cNO:%6.6s ACN:%16.16s ", // 5차수정 보완 (T3ATM AP 변경내역 #18) : AMT관련 부분 삭제
					0x1B, 0x1C, 0x1D, m_pProfile->TRANS.SerialNo, fchpAPP_EditAcnt(FALSE, TRUE));
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetCaptureData():%s", szTempData);
	return	szTempData;
}


/*==============================================================================
* Function Name : fnAPDLib_OnlineProcessing()
* Description   : 온라인 승인 요청에 대한 호스트 응답을 카드로 전달하는 함수
* Parameters    : lpszHostData : Host Response Data
*                 nHostResp    : 거래에 대한 Host의 성공, 실패 응답
*                 HostRecvFlag : Host 응답을 받았는지 송수신 에러인지를 판단.
*                                Host 응답을 받았지만 ARC가 없는 경우에는
*                                nHostResp를 통해 ARC를 만들어준다.
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
	// 실패하면 reversal 

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

		// AID ID, Label 저장
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

// 농협사양 - start
		// V07-08-00-#01: 해외카드 거래  EMV 지원
		if ((TranCode2 == TC_WITHCASHAD)	||		// 서비스출금
			(TranCode2 == TC_WITHSAVEAC)	||		// 보통예금지급
			(TranCode2 == TC_WITHCHKAC)		||		// 당좌예금지급
			(TranCode2 == TC_INQCREDAC)		||		// 현금서비스조회
			(TranCode2 == TC_INQSAVEAC)		||		// 보통예금조회
			(TranCode2 == TC_INQCHKAC)			)	// 당좌예금조회
		{
			// 해외카드 거래 마스터계열만 EMV 가능, 마스터외 M/S 전환처리함.
			if( (memcmp(m_pDevCmn->EMVCandidateList[nSelected-1].AID, MASTER_AID,  sizeof(MASTER_AID)) == 0)  ||
				(memcmp(m_pDevCmn->EMVCandidateList[nSelected-1].AID, MAESTRO_AID, sizeof(MAESTRO_AID)) == 0) ||
				(memcmp(m_pDevCmn->EMVCandidateList[nSelected-1].AID, CIRRUS_AID,  sizeof(CIRRUS_AID)) == 0)	   )
			{
				MsgDump(TRACE_CODE_MODE, "EMV", __FILE__, __LINE__, "해외카드 거래 MASTER 계열만 EMV지원");
				EMVServiceCodeChk = TRUE;		// 서비스코드 확인여부
			}
			else
			{
				MsgDump(TRACE_CODE_MODE, "EMV", __FILE__, __LINE__, "해외카드 거래 VISA 계열 Fallback");
				EMVServiceCodeChk = FALSE;		// 서비스코드 확인여부
			}	
		}	
// 농협사양 - end

		long nRtn = m_pDevCmn->fnEMVLib_Sel_FinalAppSelection(nSelected-1);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Sel_FinalAppSelection(%d) = [%ld]", nSelected-1, nRtn);
		if (nRtn == EMV_RSLT_OK)
		{
			// [#2188-1] InitApp 수행 후 성공시 여부 확인
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
				// [#2188-1] AID Select Fail시 Error Message 표시
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

			
			
			// [#2188-2] AID Select Success시에 AID Label을 화면에 표시해야함 MODIFY
			
			// >>> @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK - 선택된 AID의 Label 정보 표시
			// strTemp.Format("Selected AID : %s", strAPPLabel);
			strTemp = AIDescList.GetAt(nSelected-1);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Sel_FinalAppSelection() SUCCESS [EMV_RSLT_OK], Selected Label = [%s]", strTemp);
			if (AIDConvertFlag.GetAt(nSelected-1) == "TRUE")
			{

//동-s			wstring wsAppLabel = fnAPDLib_MBStoWCS(strTemp);
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
			// [#2188-1] AID Select Fail시 Error Message 표시
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

	// @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK - 사용전 초기화
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

		// >>> @ADD:[EMVKERNEL5_001] 2013.05.30 SHPARK - ISO 코드값에 다른 문자코드 MAPPING 처리위함
		// Select Flag(1) + appCount(1) + [9F11(2)][AppLabel길이(2)][AppLabel(가 변)][AidName길이(2)][AidName(가변)] + 반복...
		// "00 01  0010 2020202020202020202020202020202007A0000010100020"
		// "010201104243204A434220437265646974202020 07A000000065101000104369727275732020202020202020202007A0000000046000)

		// Select Flag(1) + appCount(1) + [9F11(2)][AppLabel길이(2)][priority(1)][AppLabel(가 변)][[AidName길이(2)][AidName(가변)] + 반복...
		// 우선순위 추가한거 "010201104243204A434220437265646974202020 01 07A0000000651010001043697272757320202020202020202020 02 07A0000000046000]
			

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AID List : [%s]", sAIDList);
		// Code Index Range : 01 - 10 (이외의 경우는 WCS MBS 변환없이 그대로 사용)
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
						
						// >>> @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK - Issure Code에 따른 문자열 Convert
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

							// 문자열을 버퍼에 할당
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

						// 향후 선택된 AID Label 정보를 파악하기위해 전역으로 저장
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

	// V07-13-01-#01: 농협사양 2 Brand이상인 경우 최우선 순위의 Brand를 자동선택한다
	int HighIndex = 1000;
	if (nMaxAID > 1)  
	{		
		// 우선순위정렬
		for(int i = 0; i < nMaxAID; i++)
		{
			HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVCandidateList[i].API", 
																&m_pDevCmn->EMVCandidateList[i].API, 
																sizeof(m_pDevCmn->EMVCandidateList[i].API));
			if ((m_pDevCmn->EMVCandidateList[i].API & 0x80) != 0x80)	// 0x80 고객선택
			{				
				if((m_pDevCmn->EMVCandidateList[i].API & 0x0F) != 0x0F)	// 0x0f 우선순위 미존재
				{
					if(HighIndex > m_pDevCmn->EMVCandidateList[i].API)
					{
						HighIndex = m_pDevCmn->EMVCandidateList[i].API;
						nSelected = i+1;	// 최우선순위 Index저장
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
	
	m_pDevCmn->EMVInfo.TranIndex = nSelected - 1;					// 현재 선택된 Index 저장	

	// [#2188-2] TEST CODE
	CString sAIDConvertFlag = AIDConvertFlag.GetAt(nSelected-1);
	CString sAID = AIDList.GetAt(nSelected-1);
	CString sAIDDescription = AIDescList.GetAt(nSelected-1);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Selected AID=[%s], AID Desc = [%s], Flag = [%s]", sAID, sAIDDescription, sAIDConvertFlag);

/* V07-13-01-#01: 타 유저는 Brand 화면에 표시하여 선택된 Brand로 거래 처리해야함----------------------
	if(nMaxAID>=1)
	{
		int i;
		// [#2188-1] TEST CODE
		for(i=1; i<=nMaxAID; i++)
		{
			// >>> @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK - 화면에 AID LABEL 정보 표시
			if (AIDConvertFlag.GetAt(i-1) == "TRUE")
			{

// 동-s
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
		
// 동-s		
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
* Description   : EMV 거래를 위한 온라인 송신 전처리 함수
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

	fnAPD_PbEnDisable(ENABLE, TRUE);							// 통장부 엔트리 발행

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

	if (!m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))		// 동작완료검지&장애검지
		return T_ERROR;


	if (m_pDevCmn->fnPBM_GetMaterialInfo())					// 통장존재
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
		if (!m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))	// 동작완료검지&장애검지
			return T_ERROR;

		fnAPD_MaterialRetract(DEV_PBM);						// 통장지시회수
		if (!m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))	// 동작완료검지&장애검지
			return T_ERROR;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PBMPrintTest():Return(...)");
	return T_OK;
}

// ──────────────────────────────────────

// TranDevProc.cpp End
////////////////////////////////////////////////////////////////////////


// Hi-Pass접촉식충전개발 #N0266 
// 하이패스카드 거래중지비트 체크 (접촉식)
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
		case 0:											// 정상
			CancelHIPassLoadFlg = FALSE;
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HPS_PROC_SELECTAPPLICATION____1");
			break;
		case 1:											// 하이패스 Applet 미발견
			fnAPP_CancelProc(T_MSG, "하이패스카드만 거래가 가능합니다");
			break;
		case 29:										// Lock설정카드			
			CancelHIPassLoadFlg = TRUE;
			nLockFlag = TRUE;
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HPS_PROC_SELECTAPPLICATION____2");
			break;
		default:										// 장애
			nLockFlag = TRUE;
			CancelHIPassLoadFlg = TRUE;
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HPS_PROC_SELECTAPPLICATION____3");
			//fnAPP_CancelProc(T_MSG, "하이패스카드 오류입니다. 다시 시도하여 주십시오.", FCErrMsg[nReturn].ICErrGuide);
			break;
	}

	return T_OK;
}

// 하이패스카드 잔액조회 (접촉식)
int CTranCmn::fnAPD_HPSReadBalance(int nFlag)
{
	m_pDevCmn->fnHPS_ReadBalance();								// 잔액조회
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_READBALANCE))
		fnAPP_CancelProc(T_MSG, "카드(Read Balance)를 읽는중에", "오류가 발생하였습니다");

	if (nFlag)													// 잔액만조회
		return T_OK;

	m_pDevCmn->fnHPS_ReadPan();									// 카드번호조회
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSREADPAN))
		fnAPP_CancelProc(T_MSG, "카드(Read PAN)를 읽는중에", "오류가 발생하였습니다");

	return T_OK;
}

// 하이패스카드 충전1단계 (접촉식)
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
		m_pDevCmn->fnSCR_DisplayString(1, "하이패스 충전거래(S1)를 실패하였습니다");
		m_pDevCmn->fnSCR_DisplayString(2, "상황실로 문의하여 주십시오");
		m_pDevCmn->fnSCR_DisplayScreen(701);
		Delay_Msg(1 * 1000);

		return T_ERROR;
	}

	return T_OK;;
}

// 하이패스카드 충전2단계 (접촉식)
int CTranCmn::fnAPD_HPSLoad(BYTE *pRN, BYTE *pSign2)
{
	BYTE cbTemp[128]	 = {NULL, };
	BYTE cbSignData[128] = {NULL, };

	memcpy(&cbTemp[0],	pRN,	16);
	memcpy(&cbTemp[16],	pSign2, 8);
	MakePack(cbTemp, cbSignData, 24);	

//	m_pDevCmn->fnHPS_LoadIEP(GetSprintf("%16.16s", pRN)			// 호스트난수
//						+	 GetSprintf("%8.8s",   pSign2));	// 서명2
	m_pDevCmn->fnHPS_LoadIEP(cbSignData);						// 호스트난수 + 서명2
	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnHPS_LoadIEP", cbSignData, sizeof(cbSignData));
		fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSLOAD))
	{
		m_pDevCmn->fnSCR_DisplayString(1, "하이패스 충전거래(S3)를 실패하였습니다.");
		m_pDevCmn->fnSCR_DisplayString(2, "창구에 문의하여 주십시오");
		m_pDevCmn->fnSCR_DisplayScreen(701);
		Delay_Msg(1 * 1000);

		return T_ERROR;
	}

	return T_OK;
}

// 하이패스카드 충전완료(취소)
int CTranCmn::fnAPD_HPSComplete()
{
	m_pDevCmn->fnHPS_CompleteTran();
	fnAPD_CheckDeviceAction(DEV_MCU);
	if (m_pDevCmn->fnHPS_AnalRecvData(HPS_PROC_HIPASSCOMPLETE))
		return T_ERROR;

	return T_OK;
}

// ──────────────────────────────────────

