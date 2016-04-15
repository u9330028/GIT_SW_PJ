/////////////////////////////////////////////////////////////////////////////
//	TranMenu.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	메뉴함수
/////////////////////////////////////////////////////////////////////////////
// Main Menu Display
int CTranCmn::fnAPP_MainMenuDisplay(int InitFlg)
{
	static	int		TranAvailSave = 0 ;							// 거래상태보관
	static	int		TranICExistInfo = 0 ;						// Dip Card MediaStatus 
	int		i;
	int		ScrNum = 0;
	char	WithMsg[81], DepMsg[81];
	char	HanDeviceMsg[81], EngDeviceMsg[81];
	CString strGetAtmNum("");

	if (!InitFlg)
	{
		if ((m_pDevCmn->fnSCR_GetCurrentScreenNo() == MenuTblTemp[MenuIndex].MenuScrNum) &&
			(m_pDevCmn->TranAvail == TranAvailSave))			// 거래가능체크
			return T_OK;

	}
	
	TranAvailSave = m_pDevCmn->TranAvail;						// 거래상태보관
	fnAPP_InitializeDS();										// 변수초기화
	memcpy(MenuTblTemp, MenuTbl, sizeof(MenuTblTemp));			// 거래버튼초기화
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_MainMenuDisplay:m_pDevCmn->AtmDefine.MachineType[%d]", m_pDevCmn->AtmDefine.MachineType);

	switch (m_pDevCmn->AtmDefine.MachineType)					
	{
	case U3100K:
	case U8100:
		MenuTblTemp[0].MenuScrNum = 1;						
		MenuTblTemp[1].MenuScrNum = 2;						 
		break;
		break;
	default:
		break;
	}

	TranAvail = m_pDevCmn->TranAvail;							// 전체거래상태
	WithAvail = m_pDevCmn->WithAvail;							// 출금거래상태
	DepAvail = m_pDevCmn->DepAvail;								// 입금거래상태
	PbmAvail = m_pDevCmn->PbmAvail;								// 통장거래상태
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_MainMenuDisplay:TranAvail[%x], WithAvail[%x], DepAvail[%x], PbmAvail[%x]", TranAvail, WithAvail, DepAvail, PbmAvail);

	if (m_pDevCmn->AtmDefine.DesHandler == NOT_SET)				// 해외거래무
	{
///		m_pDevCmn->DesTranStatus = FALSE;						// OutGoing불가
	}

	if (m_pDevCmn->AtmDefine.IrdaHandler == NOT_SET)			// 적외선무 
	{
///		MenuTblTemp[0].MenuInfo[2][0] = MENU_NULL;				// 휴대폰거래
	}
	else
	if (m_pDevCmn->fnIRD_GetDeviceStatus() == DOWN)
	{
///		MenuTblTemp[0].MenuInfo[2][0] = MENU_NULL;				// 휴대폰거래
	}
	else
	if (!m_pDevCmn->fnAPL_GetYesDevice(DEV_IRD))				// 적외선확인
	{
///		MenuTblTemp[0].MenuInfo[2][0] = MENU_NULL;				// 휴대폰거래
	}

//	MenuTblTemp[3].MenuInfo[11][0] = MENU_NULL;					// 해외거래
//	MenuTblTemp[6].MenuInfo[11][0] = MENU_NULL;					// 해외거래

	switch (WithAvail)											// 출금거래상태 	
	{
		case TRAN_WITH_CASH:
		case TRAN_WITH_CHECK:
		case TRAN_WITH_BOTH:
		case TRAN_WITH_50TCASH:									
		case TRAN_WITH_2CASH:		
		case TRAN_WITH_CASHCHECK:								
		case TRAN_WITH_50TCASHCHECK:
			break;
		case TRAN_WITH_NOT:
		default: 
			fnAPP_MenuClearTranProc(0, TRAN_WITH, MENU_NULL);	// 출금삭제
			fnAPP_MenuClearTranProc(0, TRAN_FOREGIN, MENU_NULL);	//해외카드 출금삭제 #N0171
			break;
	}

	switch (DepAvail)											// 입금거래상태
	{
		case TRAN_DEP_CASH:
		case TRAN_DEP_CHECK:
		case TRAN_DEP_BOTH:								
			break;
		case TRAN_DEP_NOT:
		default: 
			fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);	// 입금삭제
			break;
	}

	switch (PbmAvail)											// 통장거래상태
	{
		case TRAN_PBM_OK:										
			break;
		case TRAN_PBM_NOT:
		default:												// 통장삭제
			fnAPP_MenuClearTranProc(0, TRAN_PB, MENU_NULL);
			fnAPP_MenuClearMenuCode(0, MENU_PB, ~MENU_PB);
			break;
	}
	
	if(!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
	{
		fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);	// 입금삭제
		fnAPP_MenuClearTranProc(0, TRAN_WITH, MENU_NULL);	// 출금삭제
		fnAPP_MenuClearTranProc(0, TRAN_FOREGIN, MENU_NULL);	//해외카드 출금삭제 #N0171
	}
	


	//나이스 요청 사항임 - 농협만 장애인 업무
	if(m_pProfile->DEVICE.SightHandicapped  != TRUE || (m_pProfile->DEVICE.ETCDevice20 != NONGHYUP && m_pProfile->DEVICE.ETCDevice20 != SC))  //#N0219 제일은행 장애인 지원
	{
		MenuTblTemp[17].MenuInfo[1][0] = MENU_NULL;
		MenuTblTemp[17].MenuInfo[2][0] = MENU_NULL;
		MenuTblTemp[17].MenuInfo[3][0] = MENU_NULL;
		MenuTblTemp[17].MenuInfo[4][0] = MENU_NULL;
		MenuTblTemp[17].MenuInfo[5][0] = MENU_NULL;
		MenuTblTemp[17].MenuInfo[6][0] = MENU_NULL;

		MenuTblTemp[18].MenuInfo[1][0] = MENU_NULL;
		MenuTblTemp[18].MenuInfo[2][0] = MENU_NULL;
		MenuTblTemp[18].MenuInfo[3][0] = MENU_NULL;
	}


	if(m_pProfile->DEVICE.ETCDevice20 == KB)	 //#N0226  
	{
//		fnAPP_MenuClearTranCode(0, TC_WITHDRAW, ~MENU_PB);		// #N0227 국민은행 통장 입금만 가능
//		fnAPP_MenuClearTranCode(0, TC_TRANSFER, ~MENU_PB);		// #N0227 국민은행 통장 입금만 가능

		m_pDevCmn->fnSCR_DisplayImage(8, FALSE);  //통장
		m_pDevCmn->fnSCR_DisplayImage(13, FALSE); //전자화폐
	}
	
	//#0052
	if(!fnAPP_MenuBeforeSprCheck())
	{
		fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);	// 입금삭제
	}

// 	if(m_pDevCmn->AtmDefine.MachineType == U8100)
// 	{
// 		if (!m_pDevCmn->DesITKStatus && !m_pDevCmn->DesTMKStatus)
// 		{
// 			fnAPP_MenuClearTranProc(0, TRAN_FOREGIN, MENU_NULL);		//해외카드 출금삭제 #N0171
// 			fnAPP_MenuClearTranProc(0, TRAN_FOREGIN_INQ, MENU_NULL);	//해외카드 조회삭제 #N0171
// 		}
// 	}


	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 1단계지원)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 2단계지원)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 3단계지원)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 4단계지원)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 5단계지원)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 6단계지원)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 7단계지원)
	memcpy(&MenuTblSave, &MenuTblTemp, sizeof(MenuTblSave));	// 메뉴테이블보관

	fnAPP_MenuAvailCheck(MenuIndex);							// 메뉴가능상태구하기
	if (MenuAvail & MENU_MCU)									// 카드사용가능
	{
		fnAPD_ICCardEnDisable(ENABLE, TRUE); 			
	}
	else
		fnAPD_CardEnDisable(DISABLE, TRUE);

	if (MenuAvail & MENU_PB)									// 통장사용가능
		fnAPD_PbEnDisable(ENABLE, TRUE);
	else
		fnAPD_PbEnDisable(DISABLE, TRUE);

	fnAPP_MenuBtnCheck(MenuIndex);								// Menu Button Check
	ScrNum = MenuTblTemp[MenuIndex].MenuScrNum;

	// 동기가 맞지 않을 경우 메인화면 Display시 전맹인 모드로 진입함
	if (m_pProfile->DEVICE.MachineType == U8100)			
	{
		if ((InitFlg) && (m_pProfile->DEVICE.EarHandler) && (m_pProfile->DEVICE.ETCDevice20 == NONGHYUP))
			m_pDevCmn->EarPhoneInserted = m_pDevCmn->fnSNS_GetEarPhoneStatus();
		
		if (m_pDevCmn->EarPhoneInserted)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("거    래 : [장애인 EarPhone Inserted]"));
			m_pDevCmn->fnSCR_SetDisplayData(EAR_KINDMODE, "on");
		}
	}

	for (i = 1; i < itemof(MenuTblTemp[0].MenuInfo); i++) 
	{															// 버튼삭제
		if ((MenuTblTemp[MenuIndex].MenuInfo[i][0] == MENU_NULL) && 
			(MenuTbl[MenuIndex].MenuInfo[i][0] != MENU_NULL))
			m_pDevCmn->fnSCR_DisplayImage(i, FALSE);
	}

	fnAPP_MenuTranMsg(WithAvail, WithMsg, DepAvail, DepMsg);
	fnAPP_MenuMaterialMsg(KOR_MODE, HanDeviceMsg);
	fnAPP_MenuMaterialMsg(ENG_MODE, EngDeviceMsg);
	fnAPP_MenuWithDepMsg();			

	switch (m_pDevCmn->AtmDefine.MachineType)					// 초기화면조정
	{
		case U8100:
		case U3100K:
			strGetAtmNum.Format("%6.6s", &m_pProfile->NETWORK.AtmSerialNum[2]);
			m_pDevCmn->fnSCR_DisplayString(1, "1577 - 0068");			
			m_pDevCmn->fnSCR_DisplayString(2, strGetAtmNum);		

			m_pDevCmn->fnSCR_DisplayString(11, StrDepMsg);			// 입금정보출력
			m_pDevCmn->fnSCR_DisplayString(12, StrWithMsg);			// 출금정보출력
			m_pDevCmn->fnSCR_DisplayString(13, StrSprMsg);			// 명세표정보출력
			m_pDevCmn->fnSCR_DisplayString(1001, m_RXHOSTMSG.host_Allmsg);			// 출금정보출력
			m_pDevCmn->fnSCR_DisplayString(1006, "C:\\T1ATM\\APP\\SWF\\MainAD.swf");
			break;
		default:
			m_pDevCmn->fnSCR_DisplayString(1,  GetSprintf("%s", WithMsg));			// 출금정보출력
			break;
	}

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_NO_WAIT, PIN_DISABLE_MODE);

	m_pDevCmn->UserTranAvail = TranAvail;						// User전체거래상태
	m_pDevCmn->UserWithAvail = WithAvail;						// User지급거래상태
	m_pDevCmn->UserDepAvail = DepAvail;							// User입금거래상태
	m_pDevCmn->UserPbmAvail = PbmAvail;							// User통장거래상태

	return T_OK;
}

// Main Menu Device Event
int CTranCmn::fnAPP_MainMenuDeviceEvent(int CheckTime)
{
	ULONG  WiatTime = TimerSet(CheckTime);
	int	   CheckDev = DEV_NONE;
	int    nCardEntering = 0;

	if ((CheckTime == 0)			&&							
		(TranStartByScr == DEV_SCR)	)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MainMenuDeviceEvent : 거래선행인식");
		m_pDevCmn->DeviceTran = DEV_SCR;
		return T_OK;
	}
	
	while (TRUE)
	{
		Delay_Msg();											// Msg & Task

		CheckDev |= DEV_SCR;									// Mouse Or Touch

		if ((!CardTran)									&&		// 카드무
			(DevEnDisable & DEV_MCU)					)
			CheckDev |= DEV_MCU;

		if ((!PbTran)									&&		// 통장무
			(DevEnDisable & DEV_PBM)					)
			CheckDev |= DEV_PBM;

		if (m_pDevCmn->fnAPL_GetDeviceEvent(CheckDev))
		{
			if ((m_pDevCmn->DeviceTran & CheckDev) == m_pDevCmn->DeviceTran)
				break;
		}

		if (m_pProfile->DEVICE.MachineType == U8100)			
		{
			if ((m_pProfile->DEVICE.EarHandler) && (!m_pDevCmn->EarPhoneInserted))
			{
				m_pDevCmn->EarPhoneInserted = m_pDevCmn->fnSNS_GetEarPhoneStatus();

				if (m_pDevCmn->EarPhoneInserted)
				{
					m_pDevCmn->TTS_StartProc();
					m_pDevCmn->DeviceTran = DEV_PIN;
					return T_OK;
				}
			}
		}
		
		if ((!CheckTime) ||
			(CheckTimer(WiatTime)))
			return T_TIMEOVER;									// AtmModeCtrl사용
	}
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Menu Tran Msg
int CTranCmn::fnAPP_MenuTranMsg(int WithAvail, char *WithMsg, int DepAvail, char *DepMsg)
{
	char*	WithMsgTbl[9][4] = 
	{
//		 1234567890123456789012
		"   현금만  출금가능   ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"   수표만  출금가능   ",
		"   CHECK  AVAILABLE   ",
		"   CHECK  AVAILABLE   ",
		"   CHECK  AVAILABLE   ",
		"  현금/수표 출금가능  ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		"       출금불가       ",
		"    NO WITHDRAWAL     ",
		"    NO WITHDRAWAL     ",
		"    NO WITHDRAWAL     ",
		"   현금만  출금가능   ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"   현금만  출금가능   ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"  현금/수표 출금가능  ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		"  현금/수표 출금가능  ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		"  현금/수표 출금가능  ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE "
	};

	char*	WithMsgTblWith100Check[9][4] =					
	{
//		 123456789012345678901234567
		"     현금만  출금가능     ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		" 1백만원 수표만 출금가능  ",
		"     CHECK  AVAILABLE     ",
		"     CHECK  AVAILABLE     ",
		"     CHECK  AVAILABLE     ",
		" 현금/1백만원수표출금가능 ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		"         출금불가         ",
		"      NO WITHDRAWAL       ",
		"      NO WITHDRAWAL       ",
		"      NO WITHDRAWAL       ",
		"     현금만  출금가능     ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		"     현금만  출금가능     ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		" 현금/1백만원수표지급가능 ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		" 현금/1백만원수표지급가능 ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		" 현금/1백만원수표지급가능 ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   "
	};

	char*	DepMsgTbl[4][4] = 
	{
//		 1234567890123456789012
		"   현금만  입금가능   ",
		"  CASH  DEPOSIT ONLY  ",
		"  CASH  DEPOSIT ONLY  ",
		"  CASH  DEPOSIT ONLY  ",
		"   수표만  입금가능   ",
		"  CHECK DEPOSIT ONLY  ",
		"  CHECK DEPOSIT ONLY  ",
		"  CHECK DEPOSIT ONLY  ",
		"  현금/수표 입금가능  ",
		"  CASH/CHECK DEPOSIT  ",
		"  CASH/CHECK DEPOSIT  ",
		"  CASH/CHECK DEPOSIT  ",
		"       입금불가       ",
		"      NO DEPOSIT      ",
		"      NO DEPOSIT      ",
		"      NO DEPOSIT      "
	};

	strcpy(WithMsg, "");										// 출금정보초기화
	strcpy(DepMsg, "");											// 입금정보초기화


	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
	{															
		switch (WithAvail)										// 출금거래상태 	
		{
			case TRAN_WITH_CASH:
				strcpy(WithMsg, WithMsgTblWith100Check[0][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_2CASH:
				strcpy(WithMsg, WithMsgTblWith100Check[5][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_50TCASH:
				strcpy(WithMsg, WithMsgTblWith100Check[4][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_CHECK:
				strcpy(WithMsg, WithMsgTblWith100Check[1][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_BOTH:
				if (m_pDevCmn->Withdraw50TFlag)
					strcpy(WithMsg, WithMsgTblWith100Check[8][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				else
					strcpy(WithMsg, WithMsgTblWith100Check[2][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_CASHCHECK:
				strcpy(WithMsg, WithMsgTblWith100Check[6][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_50TCASHCHECK:
				strcpy(WithMsg, WithMsgTblWith100Check[7][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_NOT:
			default:
				strcpy(WithMsg, WithMsgTblWith100Check[3][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
		}
	}
	else
	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_HUNDRED_THOUSAND)
	{														
		switch (WithAvail)										// 출금거래상태 	
		{
			case TRAN_WITH_CASH:
				strcpy(WithMsg, WithMsgTbl[0][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_2CASH:
				strcpy(WithMsg, WithMsgTbl[5][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_50TCASH:
				strcpy(WithMsg, WithMsgTbl[4][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_CHECK:
				strcpy(WithMsg, WithMsgTbl[1][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_BOTH:
				if (m_pDevCmn->Withdraw50TFlag)
					strcpy(WithMsg, WithMsgTbl[8][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				else
					strcpy(WithMsg, WithMsgTbl[2][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_CASHCHECK:						
				strcpy(WithMsg, WithMsgTbl[6][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_50TCASHCHECK:					
				strcpy(WithMsg, WithMsgTbl[7][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_NOT:
			default:
				strcpy(WithMsg, WithMsgTbl[3][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
		}
	}
	else
	{															// 100만원권수표지급-권종정보오류
		switch (WithAvail)										// 출금거래상태 	
		{
			case TRAN_WITH_CASH:
			case TRAN_WITH_50TCASH:							
			case TRAN_WITH_2CASH:							
				strcpy(WithMsg, WithMsgTbl[0][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_CHECK:
				strcpy(WithMsg, WithMsgTbl[3][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_BOTH:
			case TRAN_WITH_CASHCHECK:						
			case TRAN_WITH_50TCASHCHECK:					
				strcpy(WithMsg, WithMsgTbl[0][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
			case TRAN_WITH_NOT:
			default:
				strcpy(WithMsg, WithMsgTbl[3][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				break;
		}
	}

	switch (DepAvail)											// 입금거래상태
	{
		case TRAN_DEP_CASH:
			strcpy(DepMsg, DepMsgTbl[0][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			break;
		case TRAN_DEP_CHECK:
			strcpy(DepMsg, DepMsgTbl[1][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			break;
		case TRAN_DEP_BOTH:
			strcpy(DepMsg, DepMsgTbl[2][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			break;
		case TRAN_DEP_NOT:
		default: 
			strcpy(DepMsg, DepMsgTbl[3][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			break;
	}

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Menu Tran Msg
int CTranCmn::fnAPP_MenuMaterialMsg(int LanMode, char *DeviceMsg)
{
	char*	DeviceMsgTbl[3][4] = 
	{
//		 123456789012345678901234567890123456
		"        카드를 넣어 주십시오.       ",
		"      Please insert your card.      ",
		"      Please insert your card.      ",
		"      Please insert your card.      ",
		"        통장을 넣어 주십시오.       ",
		"    Please insert your passbook.    ",
		"    Please insert your passbook.    ",
		"    Please insert your passbook.    ",
		"   카드 또는 통장을 넣어 주십시오.  ",
		"Please insert your card or passbook.",
		"Please insert your card or passbook.",
		"Please insert your card or passbook."
	};

	strcpy(DeviceMsg, "");										// 매체정보초기화

	if (MenuAvail & MENU_MCU)
		strcpy(DeviceMsg, DeviceMsgTbl[0][LanMode]);
	if (MenuAvail & MENU_PB)
		strcpy(DeviceMsg, DeviceMsgTbl[1][LanMode]);
	if ((MenuAvail & MENU_MCU)	&&
		(MenuAvail & MENU_PB))
		strcpy(DeviceMsg, DeviceMsgTbl[2][LanMode]);

	return T_OK;
}

// Menu Make
int	CTranCmn::fnAPP_MenuMake(void* MenuTblMake)
{
	MENUHeader*	MenuTblWork = (MENUHeader*)MenuTblMake;
	int		i, j, k;

	for (i = 0; i < itemof(MenuTbl); i++)
	{
		j = 0;													// 0번메뉴작성
		MenuTblWork[i].MenuInfo[j][0] = MENU_NULL;				// 초기화

		if (!MenuTblWork[i].MenuInfo[j][1])						// 0번이면기존화면처리
			MenuTblWork[i].MenuInfo[j][1] = i;
		for (k = 0; k < itemof(MenuTblWork[0].MenuInfo); k++)	// 카드선행시비교
		{
			MenuTblWork[i].MenuInfo[j][0] |= 
				MenuTblWork[MenuTblWork[i].MenuInfo[j][1]].MenuInfo[k][0];
		}

		if (!MenuTblWork[i].MenuInfo[j][2])						// 0번이면기존화면처리
			MenuTblWork[i].MenuInfo[j][2] = i;
		for (k = 0; k < itemof(MenuTblWork[0].MenuInfo); k++)	// 통장선행시비교
		{
			MenuTblWork[i].MenuInfo[j][0] |= 
				MenuTblWork[MenuTblWork[i].MenuInfo[j][2]].MenuInfo[k][0];
		}

		if (!MenuTblWork[i].MenuInfo[j][3])						// 0번이면기존화면처리
			MenuTblWork[i].MenuInfo[j][3] = i;
		for (k = 0; k < itemof(MenuTblWork[0].MenuInfo); k++)	// 입력선행시비교
		{
			MenuTblWork[i].MenuInfo[j][0] |= 
				MenuTblWork[MenuTblWork[i].MenuInfo[j][3]].MenuInfo[k][0];
		}

		for (j = 1; j < itemof(MenuTblWork[0].MenuInfo); j++)	// 1-n번메뉴작성
		{
			if (MenuTblWork[i].MenuInfo[j][3])					// 거래선행시비교
			{
				MenuTblWork[i].MenuInfo[j][0] = MENU_NULL;		// 초기화
				for (k = 0; k < itemof(MenuTblWork[0].MenuInfo); k++)
				{
					MenuTblWork[i].MenuInfo[j][0] |= 
					MenuTblWork[MenuTblWork[i].MenuInfo[j][3]].MenuInfo[k][0];
				}
			}
		}
	}
	return T_OK;
}

// Menu Clear Menu Code
int CTranCmn::fnAPP_MenuClearMenuCode(int Index, int MenuCode, int MenuValue)
{
	int		i, j;
	
	if (Index)
	{
		for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)	
		{
			if (MenuTblTemp[Index].MenuInfo[j][0] & MenuCode)	// 메뉴정보비교
				MenuTblTemp[Index].MenuInfo[j][0] &= MenuValue;
		}
	}
	else
	{
		for (i = 0; i < itemof(MenuTblTemp); i++)
			for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)
			{
				if (MenuTblTemp[i].MenuInfo[j][0] & MenuCode)
					MenuTblTemp[i].MenuInfo[j][0] &= MenuValue;
			}
	}
	
	return T_OK;
}

// Menu Clear Tran Proc
int CTranCmn::fnAPP_MenuClearTranProc(int Index, int TranValue, int MenuValue)
{
	int		i, j;

	if (Index)
	{
		for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)
		{
			if (MenuTblTemp[Index].MenuInfo[j][4] == TranValue)	// 거래처리코드비교
				MenuTblTemp[Index].MenuInfo[j][0] &= MenuValue;
		}
	}
	else
	{
		for (i = 0; i < itemof(MenuTblTemp); i++)
			for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)
			{
				if (MenuTblTemp[i].MenuInfo[j][4] == TranValue)
					MenuTblTemp[i].MenuInfo[j][0] &= MenuValue;
			}
	}

	return T_OK;
}
	
// Menu Clear Tran Code
int CTranCmn::fnAPP_MenuClearTranCode(int Index, int TranValue, int MenuValue)
{
	int		i, j;
	
	if (Index)
	{
		for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)
		{
			if (MenuTblTemp[Index].MenuInfo[j][5] == TranValue)	// 거래코드비교
				MenuTblTemp[Index].MenuInfo[j][0] &= MenuValue;
		}
	}
	else
	{
		for (i = 0; i < itemof(MenuTblTemp); i++)
			for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)
			{
				if (MenuTblTemp[i].MenuInfo[j][5] == TranValue)
					MenuTblTemp[i].MenuInfo[j][0] &= MenuValue;
			}
	}
	
	return T_OK;
}

// Menu Avail Check
int CTranCmn::fnAPP_MenuAvailCheck(int Index, int SubIndex)
{
	int		i;

	MenuAvail = MENU_NULL;
	if (SubIndex)												// 선택처리
		MenuAvail |= MenuTblTemp[Index].MenuInfo[SubIndex][0];
	else														// 처리관련&메뉴전체
	{
		for (i = 0; i < itemof(MenuTblTemp[0].MenuInfo); i++)
			MenuAvail |= MenuTblTemp[Index].MenuInfo[i][0];
	}

	if (MenuAvail)
		return T_OK;
	else 
		return T_CANCEL;
}
	
// Menu Button Check
int CTranCmn::fnAPP_MenuBtnCheck(int Index)
{
	int		i;

	MenuBtnCnt = 0;												// 메뉴선택가능한버튼수
	MenuBtnIndex = 0;											// 메뉴자동선택가능한최종버튼값
	for (i = 1; i < itemof(MenuTblTemp[0].MenuInfo); i++)		// 버튼수구하기
	{
		if (MenuTblTemp[Index].MenuInfo[i][0] != MENU_NULL)
		{
			MenuBtnCnt++;
			if (MenuTblTemp[Index].MenuInfo[i][2] == ON)
				MenuBtnIndex = i;
		}
	}

	if (MenuBtnCnt)
		return T_OK;
	else 
		return T_CANCEL;
}

/////////////////////////////////////////////////////////////////////////////
// Menu Procedure
int CTranCmn::fnAPP_MenuProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc()");

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("START:01=====>>=====>>=====>>=====>>=====>>>>"));
	m_pDevCmn->fnAPL_CaptureFaceSave(2, TRUE);					// 얼굴촬영
	m_pDevCmn->fnAPL_CaptureHandSave(1);						// 손촬영

	while (TRUE)
	{
		fnAPP_MenuDeviceProc();									// 메뉴가능상태조정

		if (TranProc && TranCode)								// 거래처리, 거래코드
			break;

		if (!MenuIndexNext)
			fnAPP_CancelProc(T_MSG, "해당 업무는 향후 서비스 예정입니다");
		else
		{
			MenuIndex = MenuIndexNext;
			MenuIndexSub = 0;
		}

		if (MenuTblTemp[MenuIndex].MenuInfo[0][0] != MENU_NULL)	// 처리관련(매체투입요청시)
		{
			if (MenuTblTemp[MenuIndex].MenuInfo[0][0] & CardTran)
			{													// 카드기존재
				m_pDevCmn->DeviceTran = DEV_MCU;
				fnAPP_MenuDeviceProc();							// 메뉴가능상태조정
				if (TranProc && TranCode)						// 거래처리, 거래코드
					break;
				if (MenuIndex != MenuIndexNext)					// 다른화면이동처리 : 
					continue;					
			}
			else
			if (MenuTblTemp[MenuIndex].MenuInfo[0][0] & PbTran)
			{													// 통장기존재
				m_pDevCmn->DeviceTran = DEV_PBM;
				fnAPP_MenuDeviceProc();							// 메뉴가능상태조정
				if (TranProc && TranCode)						// 거래처리, 거래코드
					break;
				if (MenuIndex != MenuIndexNext)					// 다른화면이동처리 : 
					continue;					
			}
		}

		if (CardTran)											// 처리관련(카드존재시)
		{
			m_pDevCmn->DeviceTran = DEV_MCU;
			fnAPP_MenuDeviceProc();								// 메뉴가능상태조정
			if (TranProc && TranCode)							// 거래처리, 거래코드
				break;
			if (MenuIndex != MenuIndexNext)						// 다른화면이동처리 :
				continue;					
		}


		if (PbTran)												// 처리관련(통장존재시)
		{
			m_pDevCmn->DeviceTran = DEV_PBM;
			fnAPP_MenuDeviceProc();								// 메뉴가능상태조정
			if (TranProc && TranCode)							// 거래처리, 거래코드
				break;
			if (MenuIndex != MenuIndexNext)						// 다른화면이동처리 : 
				continue;					
		}

		if(CardTran)								// 처리관련(카드존재시)
			fnAPP_MenuClearMenuCode(0, MENU_MU, ~MENU_MU);

		fnAPP_MenuDisplay();									// 메뉴출력
		fnAPP_MenuDeviceEvent();								// 메뉴이벤트처리
	}

	if ((!CardTran) && (!PbTran) && (!PbTran))					// 카드, 통장무 
		fnAPP_MenuCardPbMuAccept();								// 카드, 통장, 무매체

	// 거래코드변경추가부분
	TranProcSave = TranProc;									// 이전값저장
	TranCodeSave = TranCode;									// 이전값저장
	TranCode2Save = TranCode2;									// 이전값저장

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc(%8.8x, %d, %d)", MenuAvail, CardNew, TranCode);

	fnAPP_MenuAvailCheck(MenuIndex, MenuIndexSub);				// 메뉴가능상태구하기

	if (MenuMuTran)
	{
		WithAvail &= ~TRAN_WITH_CHECK;							// 수표출금불가
		PbmAvail &= ~TRAN_PBM_OK;								// 통장거래불가
	}

	if (MenuAvail & MENU_MCU2)									// 2TR거래
	{
//		WithAvail &= ~TRAN_WITH_CHECK;							// 수표출금불가
		DepAvail &= ~TRAN_DEP_BOTH;								// 입금불가
		PbmAvail &= ~TRAN_PBM_OK;								// 통장거래불가
	}

	if (MenuAvail & MENU_TT)									// 타행업무사용시
	{
//		if (TranProc == TRAN_DEP)								// 입금 타행입금지원
//			fnAPP_CancelProc(T_MSG, "타행거래시 입금이 불가 합니다");
		if (TranProc == TRAN_PB)								// 통장정리
			fnAPP_CancelProc(T_MSG, "타행거래시 통장정리가 불가 합니다");

//		WithAvail &= ~TRAN_WITH_CHECK;							// 수표출금불가
		DepAvail &= ~TRAN_DEP_CHECK;							// 수표입금불가 타행입금지원
//		DepAvail &= ~TRAN_DEP_BOTH;								// 입금불가
		PbmAvail &= ~TRAN_PBM_OK;								// 통장거래불가
		TranAvail = (WithAvail | DepAvail | PbmAvail);			// 전체 거래 가능 check
	}

	TranAvail = (WithAvail | DepAvail | PbmAvail);				// 전체 거래 가능 check
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc:CardTran[%x], PbTran[%x]",
	CardTran, PbTran);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc:MenuAvail[%x], TranProc[%d], TranCode[%d], TranCode2[%d]",
	MenuAvail, TranProc, TranCode, TranCode2);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc:WithAvail[%x], DepAvail[%x], PbmAvail[%x], TranAvail[%x]",
	WithAvail, DepAvail, PbmAvail, TranAvail);

	if ((CardTran) && (PbTran))									// 동시매체삽입
		fnAPP_CancelProc(T_MSG, "    카드,통장 동시 사용이 불가능합니다");
	MsgDumpAnal(TRACE_FREE_MODE, "AP", "[TRANKIND][%s]", 
										(TranProc == TRAN_WITH)	 ? "출금"		:
										(TranProc == TRAN_PB)	 ? "통장정리"	:
										(TranProc == TRAN_TRANS) ? "이체"		:
										(TranProc == TRAN_DEP)   ? "입금"		:
										(TranProc == TRAN_INQ)   ? "조회"		:
										(TranProc == TRAN_IC)    ? "IC"			:
										(TranProc == TRAN_RF)	 ? "교통카드"	: "기타");

	return T_OK;
}

// Menu Display
int CTranCmn::fnAPP_MenuDisplay()
{
	int		i;
	int		ScrNum = 0;
	int		MenuAvailSave = MenuAvail;
	char	WithMsg[81], DepMsg[81];

	fnAPP_MenuAvailCheck(MenuIndex);							// 메뉴가능상태구하기
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc(%x]",MenuAvail);
	if ((MenuAvail & MENU_MCU)	&&								// 카드사용가능
		(!MenuIrdaMode		 )	)							
	{
		fnAPD_CardEnDisable(ENABLE);
	}
	else
	{
		fnAPD_CardEnDisable(DISABLE);
	}

	if (MenuAvail & MENU_PB)									// 통장사용가능
		fnAPD_PbEnDisable(ENABLE);
	else fnAPD_PbEnDisable(DISABLE);

	fnAPP_MenuBtnCheck(MenuIndex);								// 메뉴선택가능한버튼수구하기

	if (m_pDevCmn->EarPhoneInserted)							// #N0193 장애인ATM
	{
		WithAvail &= ~TRAN_WITH_CHECK;							// 수표출금불가
		DepAvail &= ~TRAN_DEP_CHECK;							// 수표입금불가
		if(WithAvail == WITH_NOT_AVAILABLE)
			fnAPP_MenuClearTranProc(0, TRAN_WITH,  MENU_NULL);
		if(DepAvail == DEP_NOT_AVAILABLE)
			fnAPP_MenuClearTranProc(0, TRAN_DEP,  MENU_NULL);
	}

	if (TranProc && TranCode);									// 거래처리, 거래코드
	else
	if ((MenuBtnCnt == 1)	&&									// 1버튼
		(MenuBtnIndex))											// 자동선택버튼값존재시
	{
		return T_OK;
	}

	ScrNum = MenuTblTemp[MenuIndex].MenuScrNum;

	for (i = 1; i < itemof(MenuTblTemp[0].MenuInfo); i++) 
	{															// 버튼삭제
		if ((MenuTblTemp[MenuIndex].MenuInfo[i][0] == MENU_NULL) && 
			(MenuTbl[MenuIndex].MenuInfo[i][0] != MENU_NULL))
		{
			m_pDevCmn->fnSCR_DisplayImage(i, FALSE);
		}
	}
	
	MenuAvailSave = MenuAvail;
	MenuAvail = MenuAvailSave;									// 저장값복원

	fnAPP_MenuTranMsg(WithAvail, WithMsg, DepAvail, DepMsg);
	fnAPP_MenuWithDepMsg();				//#N0217
	switch (m_pDevCmn->AtmDefine.MachineType)					// 초기화면조정
	{
		case U3100K:
		case U8100:
			m_pDevCmn->fnSCR_DisplayString(11, StrDepMsg);			// 입금정보출력 #N0217
			m_pDevCmn->fnSCR_DisplayString(12, StrWithMsg);			// 출금정보출력 #N0217
			m_pDevCmn->fnSCR_DisplayString(13, StrSprMsg);			// 명세표정보출력
			break;
	}

	//[정리필요반듯이]
	if (ScrNum == 2)
	{
		m_pDevCmn->fnSCR_DisplayImage(8, FALSE);  //통장
		m_pDevCmn->fnSCR_DisplayImage(13, FALSE); //전자화폐
	}
	else
	if (ScrNum == 10)
	{
		m_pDevCmn->fnSCR_DisplayImage(2, FALSE);  //서비스 이체
	}
	else
	if (ScrNum == 121)
	{
		//#CS0004
		//m_pDevCmn->fnSCR_DisplayImage(3, FALSE);  //JCB
		m_pDevCmn->fnSCR_DisplayImage(6, FALSE);  //기타
	}



	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
																// 화면출력(메뉴화면이지만 ALL_MODE로 기본처리)
	return T_OK;
}

// Menu Device Event
int CTranCmn::fnAPP_MenuDeviceEvent(int CheckTime)
{
	int	   nCardEntering = 0;
	ULONG  WiatTime = TimerSet(CheckTime);
	
		if (m_pDevCmn->EarPhoneInserted == TRUE)				
			m_pDevCmn->fnPIN_EntryEnable(PINMODE_NORMAL, 0, 100, FALSE, PIN_NUMERIC_PAD, "", "",  -1);

	if (TranProc && TranCode);									// 거래처리, 거래코드
	else
	if ((MenuBtnCnt == 1)	&&									// 1버튼
		(MenuBtnIndex))											// 자동선택버튼값존재시
	{
		m_pDevCmn->DeviceTran = DEV_SCR;						// 메뉴자동선택
		return T_OK;
	}

	while (TRUE)
	{
		Delay_Msg();											// Msg & Task

		if (m_pDevCmn->EarPhoneInserted)	
			if (m_pDevCmn->fnSNS_GetEarPhoneStatus() == FALSE)
				fnAPP_CancelProc(T_CANCEL);

		if ((m_pDevCmn->fnAPL_GetDeviceEvent(DEV_SCR))	&&		// Mouse or Touch
			(m_pDevCmn->DeviceTran == DEV_SCR))
			break;
		
		if ((!CardTran)									&&		// 카드무
			(DevEnDisable & DEV_MCU)					&&
			(m_pDevCmn->fnAPL_GetDeviceEvent(DEV_MCU))	&&
			(m_pDevCmn->DeviceTran == DEV_MCU))
			break;
	
		if ((!PbTran)									&&		// 통장무
			(DevEnDisable & DEV_PBM)					&&
			(m_pDevCmn->fnAPL_GetDeviceEvent(DEV_PBM))	&&
			(m_pDevCmn->DeviceTran == DEV_PBM))
			break;

 		fnAPP_MenuDeviceCheck();							// Menu Device Check //#0021

		if ((!CheckTime) ||
			(CheckTimer(WiatTime)))
			fnAPP_CancelProc(T_TIMEOVER);						// TranCmn사용
	}

	if (m_pDevCmn->EarPhoneInserted == TRUE)				
		m_pDevCmn->fnPIN_EntryDisable();
	return T_OK;
}

// Menu Device Procedure
int CTranCmn::fnAPP_MenuDeviceProc()
{
	CString GetKeyStr("");
	int		GetKeyID = 0;
	int		ScrNum = 0;
	int		FICReadFlg = FALSE;
	CString	cstrCode("");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:m_pDevCmn->DeviceTran[%x]", m_pDevCmn->DeviceTran);

	switch (m_pDevCmn->DeviceTran)
	{
		case DEV_MCU:
			if (!CardTran)
			{
				m_pDevCmn->fnSCR_DisplayScreen(804);			
				fnAPD_PbEnDisable(DISABLE);
				m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				if (fnAPD_ICCardReadCheck() == T_ERROR)			
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CARDREAD : IC FAIL -> MS READ"));
				
					MenuEPBMode = MENU_NULL;					
					MenuFICMode = MENU_NULL;				
					MenuEMVMode	= FALSE;
					CardPowerOnFlg = FALSE;	
				
					fnAPD_CardEnDisable(ENABLE, TRUE);		
					fnAPD_CardReadCheck();						
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);

				}
				else
				{
					if (m_pDevCmn->FICAppletType & 0x01)
					{
						MenuFICMode = MENU_IC;
						if(fnAPD_FICDistinctCardType(TRAN_IC_FINANCEIC) == T_ERROR || fnAPD_FICBasicInquiry() == T_ERROR)
						{
							MenuEPBMode = MENU_NULL;					
							MenuFICMode = MENU_NULL;
							CardPowerOnFlg = FALSE;	

							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CARDREAD : IC FAIL -> MS READ"));
							
							fnAPD_CardEnDisable(ENABLE, TRUE);
							fnAPD_CardReadCheck();
							
							m_pDevCmn->FICAppletType &= ~0x01;
							m_pDevCmn->FICAppletType &= ~0x02;
							m_pDevCmn->FICAppletType &= ~0x04;
							m_pDevCmn->FICAppletType &= ~0x08;

							m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
						}
						else
						{
							/*#N0233 => //#N0273
							if(m_pDevCmn->AtmDefine.ETCDevice20 == KIUP)
							{
								CString strBankCode("");
								strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Right(3));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strBankCode[%s]", strBankCode);
								if (strBankCode.CompareNoCase(KIUBANKID) != 0)
									fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);	
							}
							else
						
							if(m_pDevCmn->AtmDefine.ETCDevice20 == NONGHYUP)
							{
								CString strBankCode("");
								strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Right(3));
								if ((strBankCode.CompareNoCase(NONBANKID1) == 0) || (strBankCode.CompareNoCase(NONBANKID2) == 0) || (strBankCode.CompareNoCase(NONBANKID3) == 0) ||(strBankCode.CompareNoCase(NONBANKID4) == 0))
									MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strBankCode[%s]", strBankCode);
								else
									fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);	
							
							}
							else
							if(m_pDevCmn->AtmDefine.ETCDevice20 == KEB) //#0085
							{
								CString strBankCode("");
								strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Right(3));
								if ((strBankCode.CompareNoCase(KOEBANKID) == 0) || (strBankCode.CompareNoCase(HANBANKID1) == 0) || (strBankCode.CompareNoCase(HANBANKID2) == 0) || (strBankCode.CompareNoCase(HANBANKID3) == 0) ||(strBankCode.CompareNoCase(HANBANKID4) == 0)) // #N0246
									MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strBankCode[%s]", strBankCode);
								else
								{
									fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);
									fnAPP_MenuClearTranProc(0, TRAN_EASYONE, MENU_NULL);  //#N0174 타행카드 입력시 이지원 버튼 삭제
								}	
								
							}
							else
							if(m_pDevCmn->AtmDefine.ETCDevice20 == KWANGJU) //#N0181
							{
								CString strBankCode("");
								strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Right(3));
								if (strBankCode.CompareNoCase(KAWBANKID) == 0)
									MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strBankCode[%s]", strBankCode);
								else
								{
									fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);
								}	
								
							}
							else
							if(m_pDevCmn->AtmDefine.ETCDevice20 == KYONGNAM) //#N0192
							{
								CString strBankCode("");
								strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Right(3));
								if (strBankCode.CompareNoCase(KYOBANKID) == 0)
									MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strBankCode[%s]", strBankCode);
								else
								{
									fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);
								}	
								
							}
							else
							if(m_pDevCmn->AtmDefine.ETCDevice20 == HANA) //#N0214
							{
								CString strBankCode("");
								strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Right(3));
								if ((strBankCode.CompareNoCase(HANBANKID1) == 0) || (strBankCode.CompareNoCase(HANBANKID2) == 0) || (strBankCode.CompareNoCase(HANBANKID3) == 0) ||(strBankCode.CompareNoCase(HANBANKID4) == 0) || (strBankCode.CompareNoCase(KOEBANKID) == 0))  //#N0246
									MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strBankCode[%s]", strBankCode);
								else
								{
									fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);
								}	
							}
							else
							if(m_pDevCmn->AtmDefine.ETCDevice20 == SHINHAN) //#N0215
							{
								CString strBankCode("");
								strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Right(3));
								if ((strBankCode.CompareNoCase(SHIBANKID) == 0) || (strBankCode.CompareNoCase(SHIBANKID1) == 0) || (strBankCode.CompareNoCase(CHOBANKID) == 0))
									MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strBankCode[%s]", strBankCode);
								else
								{
									fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);
								}	
							}
							else
							if(m_pDevCmn->AtmDefine.ETCDevice20 == SC) //#N0219
							{
								CString strBankCode("");
								strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Right(3));
								if (strBankCode.CompareNoCase(KOFBANKID) == 0)
									MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strBankCode[%s]", strBankCode);
								else
								{
									fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);
								}	
							}
							else
							if(m_pDevCmn->AtmDefine.ETCDevice20 == KB) //#N0226
							{
								CString strBankCode("");
								strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Right(3));
								if ((strBankCode.CompareNoCase(KUMBANKID1) == 0) || (strBankCode.CompareNoCase(KUMBANKID2) == 0) || (strBankCode.CompareNoCase(KUMBANKID3) == 0) ||(strBankCode.CompareNoCase(KUMBANKID4) == 0) ||(strBankCode.CompareNoCase(KUMBANKID5) == 0))
									MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strBankCode[%s]", strBankCode);
								else
									fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);		
							}
							else
							if(m_pDevCmn->AtmDefine.ETCDevice20 == CITI) //#N0238
							{
								CString strBankCode("");
								strBankCode.Format("%3.3s", MakeUnPack(&m_pDevCmn->FinanceICISO3Info[0].ISO3[2], 2).Right(3));
								if ((strBankCode.CompareNoCase(GOOBANKID1) == 0) || (strBankCode.CompareNoCase(GOOBANKID2) == 0) || (strBankCode.CompareNoCase(CITBANKID) == 0))
									MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strBankCode[%s]", strBankCode);
								else
								{
									fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);
								}	
							}
							*/

							CardTran |= MSCardTran;
							CardTran |= MENU_MCU; //2Tr Ready
						}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->FICAppletType[%d]", m_pDevCmn->FICAppletType);
					} 
					else CardTran = MENU_MCU;
				}

			}

			fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:"
													"CardTran[%x], "		\
													"CardBank[%d], "		\
													"CardService[%d], "		\
													"CardNew[%d]",
													CardTran,
													CardBank,
													CardService,
													CardNew);
			if (TranProc && TranCode)							// 거래처리, 거래코드
				break;

			MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[0][1];
			if (!MenuIndexNext)									// 0번이면기존화면처리
				MenuIndexNext = MenuIndex;
			TranProc = MenuTblTemp[MenuIndex].MenuInfo[0][4];	// 거래처리
			TranCode = MenuTblTemp[MenuIndex].MenuInfo[0][5];	// 거래코드
			TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[0][6];	// 거래코드2

			break;
		case DEV_PBM:
			if(m_pProfile->DEVICE.ETCDevice20 == SC || m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)  //#N0252  #N0245 #N0277 SC 통장 선행일 경우 다국어 버튼을 제거 하기 위해. 
			{
				MenuTblTemp[1].MenuInfo[10][0] = MENU_NULL;

				MenuTblTemp[2].MenuInfo[1][0] = MENU_NULL;
				MenuTblTemp[2].MenuInfo[2][0] = MENU_NULL;
				MenuTblTemp[2].MenuInfo[3][0] = MENU_NULL;
				MenuTblTemp[2].MenuInfo[4][0] = MENU_NULL;
				MenuTblTemp[2].MenuInfo[5][0] = MENU_NULL;
				MenuTblTemp[2].MenuInfo[6][0] = MENU_NULL;

				MenuTblTemp[4].MenuInfo[1][0] = MENU_NULL;
				MenuTblTemp[4].MenuInfo[2][0] = MENU_NULL;
				MenuTblTemp[4].MenuInfo[3][0] = MENU_NULL;
				MenuTblTemp[4].MenuInfo[4][0] = MENU_NULL;
				MenuTblTemp[4].MenuInfo[5][0] = MENU_NULL;
				MenuTblTemp[4].MenuInfo[6][0] = MENU_NULL;
				MenuTblTemp[4].MenuInfo[7][0] = MENU_NULL;
			}

			if (!PbTran)
			{
				fnAPD_CardEnDisable(DISABLE);
				m_pDevCmn->fnSCR_DisplayScreen(805);			// 읽기(통장)
				m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_OFF);
				fnAPD_PbReadCheck();
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : PASSBOOK READ"));
			}

			fnAPP_MenuClearMenuCode(0, MENU_ALL, PbTran);		// 통장관련메뉴처리

			if (TranProc && TranCode)							// 거래처리, 거래코드
				break;

			MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[0][2];
			if (!MenuIndexNext)									// 0번이면기존화면처리
				MenuIndexNext = MenuIndex;
			TranProc = MenuTblTemp[MenuIndex].MenuInfo[0][4];	// 거래처리
			TranCode = MenuTblTemp[MenuIndex].MenuInfo[0][5];	// 거래코드
			TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[0][6];	// 거래코드2
			break;

		case DEV_PIN:										
		case DEV_SCR:										

			// 전맹인 거래시 무매체, 통장(통장정리만 지원) 거래 미지원
			if ((m_pDevCmn->EarPhoneInserted == TRUE) &&		
				((m_pDevCmn->fnSCR_GetCurrentScreenNo() == 1)))
			{
				MenuIndex = 17;

				MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[0][2];
				if (!MenuIndexNext)									// 0번이면기존화면처리
					MenuIndexNext = MenuIndex;
				TranProc = MenuTblTemp[MenuIndex].MenuInfo[0][4];	// 거래처리
				TranCode = MenuTblTemp[MenuIndex].MenuInfo[0][5];	// 거래코드
				TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[0][6];	// 거래코드2
				
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:MenuIndex[%d], MenuIndexNext[%d], TranProc[%d], TranCode[%d], TranCode2[%d]", MenuIndex, MenuIndexNext, TranProc, TranCode, TranCode2);
				return T_OK;
			}

			if (TranProc && TranCode)
				;												// 거래처리, 거래코드
			else
			if ((MenuBtnCnt == 1)	&&							// 1버튼
				(MenuBtnIndex))									// 자동선택버튼값존재시
			{
				GetKeyID = MenuBtnIndex;
				MenuIndexSub = GetKeyID;
				MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][3];
				if (!MenuIndexNext)								// 0번이면기존화면처리
					MenuIndexNext = MenuIndex;
				TranProc = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][4];	// 거래처리
				TranCode = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][5];	// 거래코드
				TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][6];	// 거래코드2
				break;
			}

			GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString(FALSE);
			GetKeyID = 0;										// 초기화
			if (TranProc && TranCode)
				;							// 거래처리, 거래코드
			else
			if ((m_pDevCmn->fnSCR_GetCurrentScreenNo() == MenuTblTemp[MenuIndex].MenuScrNum) &&
				(GetKeyStr.GetLength()))						// 화면,길이체크
			{
				for (int i = 1; i < itemof(MenuTblTemp[0].MenuKind); i++)
				{												// 버튼찾기
					if (!GetKeyStr.CompareNoCase(MenuTblTemp[MenuIndex].MenuKind[i]))
						GetKeyID = i;
				}
			}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:GetKeyStr[%s], GetKeyID[%d]", GetKeyStr, GetKeyID);

			if ((GetKeyID >= 1) && (GetKeyID <= 21))			// 메뉴버튼값
			{
				MenuIndexSub = GetKeyID;
				MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][3];

				if (!MenuIndexNext)								// 0번이면기존화면처리
					MenuIndexNext = MenuIndex;

				TranProc = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][4];	// 거래처리
				TranCode = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][5];	// 거래코드
				TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][6];	// 거래코드2
				ScrNum = m_pDevCmn->fnSCR_GetCurrentScreenNo();

				if (!GetKeyStr.CompareNoCase("화면확대")) 
				{	
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("거    래 : [%s]", "저시력자 화면 확대"));
					m_pDevCmn->SightHandiCapped = TRUE;

				}

				//기업브랜드  //#N0262 #N0246	#N0271
				
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "기업브랜드:GetKeyStr[%s], GetKeyID[%d]", GetKeyStr, GetKeyID);
				if(m_pDevCmn->fnSCR_GetCurrentScreenNo() == 1 || m_pDevCmn->fnSCR_GetCurrentScreenNo() == 2)
				{
					if((MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][0] & MENU_MCU) &&
						GetKeyStr.CompareNoCase("화면확대"))		//#N0276
					{
							fnAPP_Accpet_AntiSkimming_Confirm();
					}
				}
				/*
				if (!GetKeyStr.CompareNoCase("입금") || !GetKeyStr.CompareNoCase("출금") || !GetKeyStr.CompareNoCase("조회") || 
					!GetKeyStr.CompareNoCase("이체") || !GetKeyStr.CompareNoCase("단기카드대출") || !GetKeyStr.CompareNoCase("공과금/지로") ||
					!GetKeyStr.CompareNoCase("해외카드") || !GetKeyStr.CompareNoCase("현금서비스지급") || !GetKeyStr.CompareNoCase("현금서비스조회")) 
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "기업브랜드:GetKeyStr[%s], GetKeyID[%d]!!!!", GetKeyStr, GetKeyID);
					if (PbTran & MENU_PB)						//통장거래시 표시 안함.
						;
					else
						fnAPP_Accpet_AntiSkimming_Confirm();
				}
				*/
				
				// 인근점포보기 지원	#N0172
				if (!GetKeyStr.CompareNoCase("인근IBK코너조회")) 
					fnAPL_NearBranchListView();
				
				if (!GetKeyStr.CompareNoCase("해외카드")) 
					m_pDevCmn->fnSCR_SetCurrentLangMode(FOR_MODE); //#N0171

				if (!GetKeyStr.CompareNoCase("이지원")) 
					m_pDevCmn->fnSCR_SetCurrentLangMode(KEB_MODE); //#N0174

				if (m_pDevCmn->fnSCR_GetCurrentScreenNo() == 9)//무매체 거래 선택
				{
					if (m_pDevCmn->fnMCU_GetMaterialInfo() || m_pDevCmn->fnPBM_GetMaterialInfo())
					{
						fnAPP_CancelProc(T_MSG, "이 매체로는 본거래를 할 수 없습니다");  //#N0219
					}
				}



				if (m_pDevCmn->fnSCR_GetCurrentScreenNo() == 103)// 외국어선택화면
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "외국어선택화면: GetKeyStr[%s], GetKeyID[%d]", GetKeyStr, GetKeyID);
					if ((GetKeyID == 1)		||		(GetKeyID == 4))							// 영문선택
						m_pDevCmn->fnSCR_SetCurrentLangMode(ENG_MODE);
					else
					if ((GetKeyID == 2)		||		(GetKeyID == 5))							// 일문선택
						m_pDevCmn->fnSCR_SetCurrentLangMode(JPN_MODE);
					else
					if ((GetKeyID == 3)		||		(GetKeyID == 6))							// 한문선택
						m_pDevCmn->fnSCR_SetCurrentLangMode(CHN_MODE);
				}
			
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE)
				{
					//Foregin
					if(GetKeyStr.CompareNoCase("VISA_출금") == 0) //#N0171
					{
						memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));
						memcpy(Accept.MuConfirmNum, "VV", 2);
						CardServiceFlag = VISA_CARD;	
					}
					else
					if(GetKeyStr.CompareNoCase("MASTER_출금") == 0)
					{
						memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));
						memcpy(Accept.MuConfirmNum, "MM", 2);
						CardServiceFlag = MASTERCARD;	
					}
					else
					if(GetKeyStr.CompareNoCase("CUP_출금") == 0)
					{
						memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));
						memcpy(Accept.MuConfirmNum, "CC", 2);
						CardServiceFlag = CUPCARD;	
					}
					else
					if(GetKeyStr.CompareNoCase("CUP_조회") == 0)
					{
						memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));
						memcpy(Accept.MuConfirmNum, "CC", 2);
						CardServiceFlag = CUPCARD;	
					}
					else
					if(GetKeyStr.CompareNoCase("AMEX") == 0)
					{
						CardServiceFlag = AMEXCARD;	
					}
					else
					if(GetKeyStr.CompareNoCase("JCB_출금") == 0)
					{
						memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));
						memcpy(Accept.MuConfirmNum, "JJ", 2);
						CardServiceFlag = JCBCARD;	
					}
					

					if(GetKeyStr.CompareNoCase("SAVINGACCOUNT") == 0)
					{
						memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));
						memcpy(Accept.MuConfirmNum, "E1", 2);
					}
					else
					if(GetKeyStr.CompareNoCase("CHECKINGACCOUNT") == 0)
					{
						memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));
						memcpy(Accept.MuConfirmNum, "E2", 2);
					}
					else
					if(GetKeyStr.CompareNoCase("CREDITCARD") == 0)
					{
						memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));
						memcpy(Accept.MuConfirmNum, "E3", 2);
					}

				}
				
			}
			else 
			if (GetKeyStr == S_CANCEL)							// 취소버튼
				fnAPP_CancelProc(T_CANCEL);
			else 
			if (GetKeyStr == S_TIMEOVER)						// 시간초과
				fnAPP_CancelProc(T_TIMEOVER);
			break;

		default: 
			break;
	}


	// 카드선행 및 메뉴선행시 (Motor-Type) 
	if((TranCode2 == TRANID_7200 || TranCode2 == TRANID_4310 || TranCode2 == TRANID_7220 || TranCode2 == TRANID_7260 \
		|| TranCode2 == TRANID_4811 || TranCode2 == TRANID_4810 || TranCode2 == TRANID_7321  || TranCode2 == TRANID_7326 || TranCode2 == TRANID_7500 || TranCode2 == TRANID_7503) \
		&& (CardTran & MENU_IC) && (m_pProfile->DEVICE.MCUHandler == O_TYPE))  //#N0174
	{
				
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "⊙fnAPP_MenuDeviceProc:FICAppletType[%d]", m_pDevCmn->FICAppletType);
		if (MenuAvail & MENU_MCU3)
		{
			if ((m_pDevCmn->FICAppletType & 0x01) && !(MenuFICMode & MENU_INFO_3))	//#0138		
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CARDREAD : IC READ"));

				if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 804)			
					m_pDevCmn->fnSCR_DisplayScreen(804);		// 읽기(카드)

				if (!(MenuFICMode & MENU_INFO_2))  //Kim.Gi.Jin 0726
				{
	
					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}								
				else
				if(fnAPD_FICDistinctCardType(TRAN_IC_FINANCEIC) == T_ERROR || fnAPD_FICBasicInquiry() == T_ERROR)
				{
					MenuEPBMode = MENU_NULL;					// 2006.11.10
					MenuFICMode = MENU_NULL;

					CardPowerOnFlg = FALSE;						// 카드접속무

					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}
				else
				if (MenuFICMode & MENU_IC)						// 금융IC거래처리	
				{
					MenuFICMode = MENU_IC; 

					if (m_pDevCmn->FICAccountCount > 1)				// 다중계좌시 PIN Check
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CARDREAD : IC READ -> 계좌(%d)", m_pDevCmn->FICAccountCount));
						fnAPP_AcceptKCashPassword(TRAN_IC_FINANCEIC);
																	// 비밀번호입력
						fnAPP_KCashCheckPin();						// 비밀번호검사
					}
					
					if(fnAPP_FICSelectAccount() == T_ERROR)			// 금융IC 계좌선택無
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// 카드접속무

    					fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					else
					if(fnAPD_FICCardReadCheck() == T_ERROR)
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// 카드접속무

						fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					else
					if(!(CardTran & MENU_J3T3))  //#0071
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// 카드접속무

						fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					
				}
				else
				{
					MenuFICMode = MENU_NULL;
					MenuEPBMode = MENU_NULL;

					CardPowerOnFlg = FALSE;						// 카드접속무

					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}
			}
			else
			{
				if(!CardTran) //Read Double Check 2009.10.25 Kim.Gi.Jin
				{
					MenuFICMode = MENU_NULL;
					MenuEPBMode = MENU_NULL;
					m_pDevCmn->FICAppletType = 0;

					CardPowerOnFlg = FALSE;							// 카드접속무

					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}
			}
			fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);		// 카드관련메뉴처리
		}
		
		fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);			// 카드관련메뉴처리
	}
	else
	if((TranCode2 == TRANID_4330 || TranCode2 == TRANID_7230 || TranCode2 == TRANID_7260 || TranCode2 == TRANID_7326 
		|| TranCode2 == TRANID_4812 || TranCode2 == TRANID_7111 || TranCode2 == TRANID_7105 || TranCode2 == TRANID_4332 
		|| TranCode2 == TRANID_7232 || TranCode2 == TRANID_4311 || TranCode2 == TRANID_4312 || TranCode2 == TRANID_4331
		|| TranCode2 == TRANID_7221 || TranCode2 == TRANID_4840) && (CardTran & MENU_J2T2))   // #N0161-1   #N0174 >> #N0278
	{
		if ((TranCode  == TC_FOREGIN) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE))
		{
			EMVReadFlg = FALSE;
			FallBackEMVMode = FALSE;

			//Kim 11/20 인증서IC 포함 예외처리
			if(MenuFICMode & MENU_IC)
			{
				MenuFICMode = MENU_NULL;
				MenuEPBMode = MENU_NULL;

				CardPowerOnFlg = FALSE;	
				
				fnAPD_CardEnDisable(ENABLE, TRUE);	
				fnAPD_CardReadCheck();
				m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);	
			}
			else
			{
				;
			}
		}
		else
		if (FallBackCnt != 0x30)						// POWER ON 성공
		{
			if ((TranCode2 == TRANID_4330) ||
				(TranCode2 == TRANID_7111) ||
				(TranCode2 == TRANID_7230))
			{
				if (fnAPD_ICCardReadCheck() != T_ERROR)				// IC카드읽기		
				{

					if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 804)			
						m_pDevCmn->fnSCR_DisplayScreen(804);		// 읽기(카드)

					if (m_pDevCmn->IfmDirect == TRUE)
					{
						if (fnAPPLib_EMVReadProc() == T_ERROR)
						{
							m_pDevCmn->fnKCS_PowerOff();			// #N0268
							fnAPD_CheckDeviceAction(DEV_MCU);
							m_pDevCmn->fnKCS_PowerOn();
							fnAPD_CheckDeviceAction(DEV_MCU);
							
							if (fnAPPLib_EMVReadProc() == T_ERROR)
							{
								FallBackCnt = 0x31;					// FallBack : Select Fail : 0
								EMVReadFlg = FALSE;
								FallBackEMVMode = TRUE;

								MenuFICMode = MENU_NULL;
								MenuEPBMode = MENU_NULL;

								CardPowerOnFlg = FALSE;	
								
								fnAPD_CardEnDisable(ENABLE, TRUE);	
								fnAPD_CardReadCheck();
								m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);	
							}
						}
					}
					else
					{
						if (fnAPP_EMVReadProc() == T_ERROR)		// EMV기본정보READ
						{
							FallBackCnt = 0x31;					// FallBack : Select Fail : 0
							EMVReadFlg = FALSE;
							FallBackEMVMode = TRUE;

							MenuFICMode = MENU_NULL;
							MenuEPBMode = MENU_NULL;

							CardPowerOnFlg = FALSE;	
							
							fnAPD_CardEnDisable(ENABLE, TRUE);	
							fnAPD_CardReadCheck();
							m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);	
						}
					}
				}
			}
			else
			{
				//Kim 11/20 인증서IC 포함 예외처리
				if(MenuFICMode & MENU_IC)
				{
					MenuFICMode = MENU_NULL;
					MenuEPBMode = MENU_NULL;

					CardPowerOnFlg = FALSE;	
					
					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);	
				}

			}
		}
		else
		{
			//Kim 11/20 인증서IC 포함 예외처리
			if(MenuFICMode & MENU_IC)
			{
				MenuFICMode = MENU_NULL;
				MenuEPBMode = MENU_NULL;

				CardPowerOnFlg = FALSE;	
				
				fnAPD_CardEnDisable(ENABLE, TRUE);	
				fnAPD_CardReadCheck();
				m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);		
			}
		}

		fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);	
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:MenuAvail[%x], MenuIndex[%d], MenuIndexSub[%d]", MenuAvail, MenuIndex, MenuIndexSub);

	fnAPP_MenuAvailCheck(MenuIndex, MenuIndexSub);				// 메뉴가능상태구하기

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:MenuAvail[%x], MenuIndex[%d], MenuIndexSub[%d]", MenuAvail, MenuIndex, MenuIndexSub);

	if (!MenuAvail)
	{
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KEB_MODE)  // Cancel Msg 한글 표시 #N0174
			m_pDevCmn->fnSCR_SetCurrentLangMode(KOR_MODE);
		
		if (MenuIndexSub)										// Mouse or Touch
		{
			if ((CardTran) && (MenuTbl[MenuIndex].MenuInfo[MenuIndexSub][0]))
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("카드     : 이 카드으로는 해당 거래 불가")); 

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
					fnAPP_CancelProc(T_MSG, "이 카드로는 본거래를 할 수 없습니다");

				}
				else
					fnAPP_CancelProc(T_MSG, "Your card can not", "be used at this transaction");
			}
			if ((PbTran) && (MenuTbl[MenuIndex].MenuInfo[MenuIndexSub][0]))
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 이 통장으로는 해당 거래 불가")); 

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "이 통장으로는 본거래를 할 수 없습니다");
				else
					fnAPP_CancelProc(T_MSG, "Your passbook can not", "be used at this transaction");
			}
			
		}
		else													// MCU, PBM
		{
			if (CardTran)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("카드     : 이 카드으로는 해당 거래 불가"));

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "이 카드로는 본거래를 할 수 없습니다");
				else
					fnAPP_CancelProc(T_MSG, "Your card can not", "be used at this transaction");
			}
			if (PbTran)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("통장    : 이 통장으로는 해당 거래 불가"));

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "이 통장으로는 본거래를 할 수 없습니다");
				else
					fnAPP_CancelProc(T_MSG, "Your passbook can not", "be used at this transaction");
			}
		}
		fnAPP_CancelProc(T_MSG, "해당 업무는 향후 서비스 예정입니다");
	}

	fnAPP_MenuDeviceCheck();									// Menu Device Check //#0021
	return T_OK;
}
		
// Menu Device Check
int CTranCmn::fnAPP_MenuDeviceCheck()
{
	if ((m_pDevCmn->fnAPL_GetDeviceEvent(DEV_MCU))	&&			// 동시매체삽입
		(m_pDevCmn->fnAPL_GetDeviceEvent(DEV_PBM)))
		fnAPP_CancelProc(T_MSG, "카드,통장 동시 사용이 불가합니다");

	if ((m_pDevCmn->fnMCU_GetMaterialInfo() & ST_SENSOR2)	&&	// 동시매체삽입
		(m_pDevCmn->fnPBM_GetMaterialInfo() & ST_SENSOR2))
		fnAPP_CancelProc(T_MSG, "카드,통장 동시 사용이 불가합니다");
	

	return T_OK;
}

// Menu Card Pb Mu Accept
int CTranCmn::fnAPP_MenuCardPbMuAccept(int ProcAvail)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuCardPbMuAccept(%x)", ProcAvail);

	int		ScrNum = 0;
	CString GetKeyStr("");
	int		nReturn = 0;
	CString strReturn("");
	
	if (!ProcAvail)
	{
		fnAPP_MenuAvailCheck(MenuIndex, MenuIndexSub);			// 메뉴가능상태구하기
		ProcAvail = MenuAvail;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuCardPbMuAccept:ProcAvail[%x]", ProcAvail);
	}

	if (ProcAvail == MENU_IC)									// IC카드사용가능
	{
		fnAPD_CardEnDisable(DISABLE, TRUE);
		fnAPD_PbEnDisable(DISABLE, TRUE);						// 속도개선 : 2003.11.21(기대효과:300ms)
		fnAPD_ICCardEnDisable(ENABLE);
	}
	else
	if (ProcAvail & MENU_MCU)									// 카드사용가능
		fnAPD_CardEnDisable(ENABLE);
	else fnAPD_CardEnDisable(DISABLE);

	
	if (ProcAvail & MENU_PB)									// 통장사용가능
		fnAPD_PbEnDisable(ENABLE);
	else fnAPD_PbEnDisable(DISABLE);

	if (TranCode == TC_HIPASS_CREDIT)
		ScrNum = 2921;
	else
	if ((ProcAvail & MENU_MCU)	&& 
		(ProcAvail & MENU_PB))
		ScrNum = 802;											// 카드/통장
	else
	if (ProcAvail & MENU_MCU)
		ScrNum = 801;											// 카드
	else
	if (ProcAvail & MENU_PB)
		ScrNum = 803;											// 통장
	else
	if (ProcAvail & MENU_MU)
	{
		fnAPD_CardEnDisable(DISABLE);
		fnAPD_PbEnDisable(DISABLE);
	}

	

	if (ScrNum)													// 매체&Touch동시처리
	{
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
																// 화면출력
		fnAPP_MenuDeviceEvent();								// 메뉴이벤트처리
		fnAPP_MenuDeviceProc();									// 메뉴가능상태조정
	}
	
	return T_OK;
}

// Menu Irda Accept
int CTranCmn::fnAPP_MenuIrdaAccept()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuIrdaAccept()");

	int		nReturn = 0;
	CString strReturn("");
	
	CString strISO2("");
	CString strUnpackISO2("");

	CString strISO3("");
	CString strUnpackISO3("");
	
	fnAPD_CardEnDisable(DISABLE);								// 카드투입금지
	fnAPD_PbEnDisable(DISABLE);									// 통장투입금지

	m_pDevCmn->fnIRD_EntryEnable();								// 투입허가
	DevEnDisable |= DEV_MCU;									// 카드부처리설정	2004.02.24
	m_pDevCmn->fnSCR_DisplayScreen(821, K_NO_WAIT, PIN_MENU_MODE);// 적외선입력

	nReturn = m_pDevCmn->fnCMN_ScanDeviceAction(DEV_IRD | DEV_SCR, K_65_WAIT, EVENT_IN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuIrdaAccept:nReturn[%d]", nReturn);
	if (nReturn == R_TIMEOUT)
	{
		m_pDevCmn->fnIRD_EntryDisable();						// 투입금지
		fnAPP_CancelProc(T_TIMEOVER);
	}
	else
	if (nReturn == R_TIMEOVER)									// 2004.07.27 IRD TIMEOVER처리강화
	{
		m_pDevCmn->fnIRD_EntryDisable();						// 투입금지
		fnAPP_CancelProc(T_TIMEOVER);
	}
	else
	if (nReturn == DEV_IRD)
	{
																// 2004.02.05 적외선데이타 변경처리
		strReturn = m_pDevCmn->fstrIRD_GetData();
		m_pDevCmn->fnIRD_AnalData(strReturn.GetBuffer(strReturn.GetLength()));

		if ((m_pDevCmn->IRDAInfo.ISODataTag != 'A') &&			// 일반계좌정보
			(m_pDevCmn->IRDAInfo.ISODataTag != 'K') &&			// 일반계좌정보
			(m_pDevCmn->IRDAInfo.ISODataTag != 'C') )			// 신용정보
		{
			fnAPP_CancelProc(T_MSG, "이 휴대폰으로는  ", "거래를 하실 수 없습니다");
		}

		strReturn = "";											// 1Track정보없음
		strReturn += ",";										// Add Delemeter

		if ((m_pDevCmn->IRDAInfo.ISODataTag == 'C') ||			// 신용계좌
			(m_pDevCmn->IRDAInfo.ISODataTag == 'H')	)			// 신용계좌
		{
			MakeUnPack(m_pDevCmn->IRDAISO2Info.ISO2,			// Make 2Track Data
						strUnpackISO2, 
						__min(37, sizeof(m_pDevCmn->IRDAISO2Info.ISO2)));
			strReturn += strUnpackISO2;
		}
		strReturn += ",";										// Add Delemeter

		if ((m_pDevCmn->IRDAInfo.ISODataTag == 'A')	||			// 일반계좌
			(m_pDevCmn->IRDAInfo.ISODataTag == 'K')	)			// 일반계좌
		{
			MakeUnPack( m_pDevCmn->IRDAISO3Info.ISO3,			// Make 3Track Data
						strISO3, 
						__min(54, sizeof(m_pDevCmn->IRDAISO3Info.ISO3)),
						FALSE);

			MakeUnPack( strISO3.GetBuffer(strISO3.GetLength()),	// 3Track 카드데이타분석을 위해 다시 UnPack
						strUnpackISO3, 
						strISO3.GetLength());

			strReturn += strUnpackISO3.Mid(2, 104*2);			// (UnPack데이타이므로 Mid시주의)
		}
																
		fnAPD_CardReadCheck(strReturn);							// 카드체크
	}
	else
	if (nReturn == DEV_SCR)
	{
		strReturn = m_pDevCmn->fstrSCR_GetKeyString(FALSE);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuIrdaAccept:strReturn[%s]", strReturn);
		m_pDevCmn->fnIRD_EntryDisable();						// 투입금지
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else fnAPP_CancelProc(T_TIMEOVER);
	}

	DevEnDisable &= (~DEV_MCU);									// 카드처리설정해제	2004.02.24
	return T_OK;
}

// Menu Irda/2DBarCode Accept
int CTranCmn::fnAPP_MenuIrdaBarCodeAccept()
{
	return T_OK;
}


// Menu Before Input Spr Check  (축퇴처리)	
int CTranCmn::fnAPP_MenuBeforeSprCheck()
{
	if(m_pProfile->DEVICE.SHUHandler == NOT_SET)
	{	
		return FALSE;
	}

	if ((!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) || (m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_SPR)))
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetAvailDevice (%x)", m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetAvailErrorDevice (%x)", m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_SPR));
		return FALSE;
	}
	else
	if ((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL) || (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->SlipHeadStatus (%d)", m_pDevCmn->SlipHeadStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->SlipStatus (%d)", m_pDevCmn->SlipStatus);	
		m_pDevCmn->DeviceAutoOff |= DEV_SPR;
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


int CTranCmn::fnGetNhCardConFirm()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnGetNhCardConFirm():[%x]", CardTran & MENU_ONLY2);
	if(CardTran & MENU_ONLY2)
		return TRUE;
	else
		return FALSE;
}


// Menu Card MCU3 Accept
int CTranCmn::fnAPP_MenuCardMCU3Accept()	// #N0142
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuCardMCU3Accept()");

	int		ScrNum = 0;
	CString GetKeyStr;

	fnAPD_ICCardEnDisable(ENABLE, TRUE);
	
	fnAPD_PbEnDisable(DISABLE);

	ScrNum = 801;											// 카드
	
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);

	fnAPP_MenuDeviceEvent();								// 메뉴이벤트처리

	if (m_pDevCmn->DeviceTran == DEV_SCR)
	{
		GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString(FALSE);

		if (GetKeyStr == S_CANCEL)							// 취소버튼
			fnAPP_CancelProc(T_CANCEL);
		else 
		if (GetKeyStr == S_TIMEOVER)						// 시간초과
			fnAPP_CancelProc(T_TIMEOVER);
	}

	//fnAPD_CardReadCheck();
	/////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(804);			
	fnAPD_PbEnDisable(DISABLE);
	m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
	//Delay_Msg(5000);
	if (fnAPD_ICCardReadCheck() == T_ERROR)			
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CARDREAD : IC FAIL -> MS READ"));
				
		MenuEPBMode = MENU_NULL;					
		MenuFICMode = MENU_NULL;				

		CardPowerOnFlg = FALSE;	
	
		fnAPD_CardEnDisable(ENABLE, TRUE);		
		fnAPD_CardReadCheck();						
		m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
	}
	else
	{
		if (m_pDevCmn->FICAppletType & 0x01)
		{
			MenuFICMode = MENU_IC;
			if(fnAPD_FICDistinctCardType(TRAN_IC_FINANCEIC) == T_ERROR || fnAPD_FICBasicInquiry() == T_ERROR)
			{
				MenuEPBMode = MENU_NULL;					
				MenuFICMode = MENU_NULL;
				CardPowerOnFlg = FALSE;	

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CARDREAD : IC FAIL -> MS READ"));
				
				fnAPD_CardEnDisable(ENABLE, TRUE);
				fnAPD_CardReadCheck();
				
				m_pDevCmn->FICAppletType &= ~0x01;
				m_pDevCmn->FICAppletType &= ~0x02;
				m_pDevCmn->FICAppletType &= ~0x04;
				m_pDevCmn->FICAppletType &= ~0x08;

				m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
			}
			else
			{
				CardTran |= MSCardTran;
				CardTran |= MENU_MCU; //2Tr Ready
			}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->FICAppletType[%d]", m_pDevCmn->FICAppletType);
		} 
		else CardTran = MENU_MCU;
	}
	
	if (MenuAvail & MENU_MCU3)
		{
			if ((m_pDevCmn->FICAppletType & 0x01) && !(MenuFICMode & MENU_INFO_3))	//#0138		
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CARDREAD : IC READ"));

				if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 804)			
					m_pDevCmn->fnSCR_DisplayScreen(804);		// 읽기(카드)

				if (!(MenuFICMode & MENU_INFO_2))  //Kim.Gi.Jin 0726
				{
	
					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}								
				else
				if(fnAPD_FICDistinctCardType(TRAN_IC_FINANCEIC) == T_ERROR || fnAPD_FICBasicInquiry() == T_ERROR)
				{
					MenuEPBMode = MENU_NULL;					// 2006.11.10
					MenuFICMode = MENU_NULL;

					CardPowerOnFlg = FALSE;						// 카드접속무

					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}
				else
				if (MenuFICMode & MENU_IC)						// 금융IC거래처리	
				{
					MenuFICMode = MENU_IC; 

					if (m_pDevCmn->FICAccountCount > 1)				// 다중계좌시 PIN Check
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CARDREAD : IC READ -> 계좌(%d)", m_pDevCmn->FICAccountCount));
						fnAPP_AcceptKCashPassword(TRAN_IC_FINANCEIC);
																	// 비밀번호입력
						fnAPP_KCashCheckPin();						// 비밀번호검사
					}
					
					if(fnAPP_FICSelectAccount() == T_ERROR)			// 금융IC 계좌선택無
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// 카드접속무

    					fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					else
					if(fnAPD_FICCardReadCheck() == T_ERROR)
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// 카드접속무

						fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					else
					if(!(CardTran & MENU_J3T3))  //#0071
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// 카드접속무

						fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					
				}
				else
				{
					MenuFICMode = MENU_NULL;
					MenuEPBMode = MENU_NULL;

					CardPowerOnFlg = FALSE;						// 카드접속무

					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}
			}
			else
			{
				if(!CardTran) //Read Double Check 2009.10.25 Kim.Gi.Jin
				{
					MenuFICMode = MENU_NULL;
					MenuEPBMode = MENU_NULL;
					m_pDevCmn->FICAppletType = 0;

					CardPowerOnFlg = FALSE;							// 카드접속무

					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}
			}
			fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);		// 카드관련메뉴처리
		}
		
		fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);			// 카드관련메뉴처리
	//////////////////////////////

	m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
	fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);			

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuCardMCU3Accept():return()");
	
	return T_OK;
}


int CTranCmn::fnAPL_NearBranchListView()	// #0302
{
	int		ScrNum = 0;
	CString GetKeyStr("");
	CString strTemp("");

	// 인근점포안내
	ScrNum = 3210;

	m_pDevCmn->bNearBranchSelectBtn = FALSE;   //#N0172

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	strTemp = IniGetStr(_TRANS_NEARBRANCH_INI, "1", "cname");
	if (strTemp.IsEmpty())
		fnAPP_CancelProc(T_MSG, "죄송합니다.", "인근점포 안내를 제공할 수 없습니다.");

	for (int i=1; i<=5; i++)
	{
		strTemp = IniGetStr(_TRANS_NEARBRANCH_INI, Int2Asc(i), "cname");
		if (strTemp.IsEmpty())
			break;
		else
		if(IsSpace(&strTemp, strTemp.GetLength()))
			break;

		m_pDevCmn->fnSCR_DisplayString(i * 2 - 1, strTemp);
		m_pDevCmn->fnSCR_DisplayString(i * 2, IniGetStr(_TRANS_NEARBRANCH_INI, Int2Asc(i), "caddr"));
	}

	if(m_pDevCmn->AtmStatus == ATM_READY)
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE1);  //#N0172-1
	else
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_60_WAIT, PIN_MENU_MODE1);  //#N0172-1

	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);		 //#N0172-1				
	else if (GetKeyStr == S_TIMEOVER)						
		fnAPP_CancelProc(T_OK);
	else if (GetKeyStr == S_EXIT)
		fnAPP_CancelProc(T_OK);						

	return T_OK;
}




// #N0217
int CTranCmn::fnAPP_MenuWithDepMsg()
{
	StrWithMsg.Empty();
	StrDepMsg.Empty();
	StrSprMsg.Empty();
	//출금 가능 상태
	if(WithAvail & TRAN_WITH_CASH)
		StrWithMsg = "1||";
	else
		StrWithMsg = "2||";


	if(WithAvail & TRAN_WITH_50TCASH )
		StrWithMsg += "1";
	else
		StrWithMsg += "2";


// 입금가능 상태
	if((DepAvail & TRAN_DEP_CASH) && (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) )  //명세표 사용 불가 시 입금 거래 안됨
		StrDepMsg = "1||1";
	else
		StrDepMsg = "2||2";

	if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		StrSprMsg = "1";
	else
		StrSprMsg = "2";

	return T_OK;
}



// Menu Card MCU2 Accept
int CTranCmn::fnAPP_MenuCardMCU2Accept()	// #N0266
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuCardMCU2Accept()");

	int		ScrNum = 0;
	CString GetKeyStr;

	fnAPD_CardEnDisable(ENABLE);
	
	fnAPD_PbEnDisable(DISABLE);

	if (TranCode == TC_HIPASS_CREDIT)
		ScrNum = 2921;
	else
		ScrNum = 801;											// 카드
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);

	fnAPP_MenuDeviceEvent();								// 메뉴이벤트처리
	//fnAPP_MenuDeviceProc();									// 메뉴가능상태조정

	if (m_pDevCmn->DeviceTran == DEV_SCR)
	{
		GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString(FALSE);

		if (GetKeyStr == S_CANCEL)							// 취소버튼
			fnAPP_CancelProc(T_CANCEL);
		else 
		if (GetKeyStr == S_TIMEOVER)						// 시간초과
			fnAPP_CancelProc(T_TIMEOVER);
	}


	fnAPD_CardReadCheck();
	m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
	fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);			

	if(!(CardTran & MENU_MCU2))
		fnAPP_CancelProc(T_MSG, "신용카드만 사용이 가능합니다.", "다시 시도하여 주십시오");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuCardMCU2Accept():return()");
	
	return T_OK;
}
