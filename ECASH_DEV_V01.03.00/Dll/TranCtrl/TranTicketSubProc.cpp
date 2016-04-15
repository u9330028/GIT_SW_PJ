#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\MacroDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**********************************************************************************************************/
/* Function Name : TicketSubProc Make (send) gijin
/* 
/* Date			 : 2010.06.09
/* 
/* Input	     : TRCODE
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/  

int CTranCmn::fnAPP_TranTicketInitProc()
{
	if (!MenuFICMode)
	{
		fnAPD_MaterialOut(DEV_MCU);							
		fnAPD_MaterialOutCheckRetract(DEV_MCU);			
	}
	//	MainTranDoingFlag = FALSE;	
	
	RegSetStr(REG_UNION_TICKET, "RX8010_AMOUNT", "");
	RegSetStr(REG_UNION_TICKET, "RX8010_TICKET", "");
	RegSetStr(REG_UNION_TICKET, "RX8014_BOOKING", "");
	RegSetStr(REG_UNION_TICKET, "RXPIFF_INFO", "");
	RegSetStr(REG_UNION_TICKET, "RXPIFF_STORY", "");
	RegSetStr(REG_UNION_TICKET, "RXPIFF_ZONE", "");
	RegSetStr(REG_UNION_TICKET, "RXPIFF_GRADE", "");
	RegSetStr(REG_UNION_TICKET, "RXNOTE_TABLE", "");
	RegSetStr(REG_UNION_TICKET, "RXPIFF_BOOKING", "");
	RegSetStr(REG_UNION_TICKET, "TXTICKET_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXTICKET_NAME", "");
	RegSetStr(REG_UNION_TICKET, "TXSPORTS_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXSPORTSCLUB_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXEVENT_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXTICKET_PROVIDER", "");
	RegSetStr(REG_UNION_TICKET, "TXGAME_DATE", "");
	RegSetStr(REG_UNION_TICKET, "TXGAME_TIME", "");
	RegSetStr(REG_UNION_TICKET, "TXGROUND_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXSPORTS_DATA", "");
	RegSetStr(REG_UNION_TICKET, "TXZONE_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXZONE_NAME", "");
	RegSetStr(REG_UNION_TICKET, "TXBLOCK_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXBLOCK_NAME", "");
	RegSetStr(REG_UNION_TICKET, "TXNOTE_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXNOTE_TABLE", "");
	RegSetStr(REG_UNION_TICKET, "TXSETTLE_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXEXHI_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXEXHIGOOD_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXEXHI_DATA", "");
	RegSetStr(REG_UNION_TICKET, "TXPLAY_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXPLAY_NAME", "");
	RegSetStr(REG_UNION_TICKET, "TXGOOD_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXGOOD_NAME", "");
	RegSetStr(REG_UNION_TICKET, "TXGOOD_EVENTTYPE", "");
	RegSetStr(REG_UNION_TICKET, "TXGOOD_STARTDATE", "");
	RegSetStr(REG_UNION_TICKET, "TXGOOD_ENDDATE", "");
	RegSetStr(REG_UNION_TICKET, "TXGOOD_DISCOUNT", "");
	RegSetStr(REG_UNION_TICKET, "TXPLAY_DATE", "");
	RegSetStr(REG_UNION_TICKET, "TXTICK_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXGOOD_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_NAME", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_TRAN", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_SCHEDULE_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXTICKET_ORDER", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_STORY_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_ZONE_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_GRADE_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_NOTE_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_SEAT_TYPE", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_COUPON", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_RESERVED_CODE", "");
	RegSetStr(REG_UNION_TICKET, "TXPIFF_BOOKING_CODE", "");


	return T_OK;
}


int CTranCmn::fnAPP_TranTicketInputProc()
{

	CString strReturn;



	return T_OK;
}


int CTranCmn::fnAPP_TranTicketOutputProc()
{


	return T_OK;
}


int CTranCmn::fnAPP_TranTicketCheckDeviceActionProc()
{
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);
	return T_OK;
}

int CTranCmn::fnAPP_TranTicketCheckMenuContinueProc()
{
	if (PbTran)
	{
		if (PbNextPageFlag || PbNextTranFlag)					
			MainTranDoingFlag = TRUE;
		else
			MainTranDoingFlag = FALSE;
	}

	return T_OK;
}


CString CTranCmn::fnAPP_TranTicket_SubStep_01_Proc()
{
	int		ScrNum = 0;
	CString GetKeyStr("");
	CString strTemp("");
	CStringArray saTemp;
	int nSize = 0;


	return GetKeyStr;
}

// 통합티켓/스포츠티켓
int CTranCmn::fnAPP_TranTicket_SubStep_02_Proc()
{
	int		ScrNum = 0;
	CString GetKeyStr("");
	CString strTemp("");
	CString strSportsCode, strSportsClubCode, strEventCode, strZoneCode, strBlockCode;
	CString strAddNotice;
	CStringArray saTemp;
	int nSize = 0;
	int nPrevCnt = 0;


	return T_OK;
}

// 통합티켓/전시
int CTranCmn::fnAPP_TranTicket_SubStep_03_Proc()
{
	int		ScrNum = 0;
	CString GetKeyStr("");
	CString strTemp("");
	CString strExhiCode, strExhiGoodCode, strEventCode, strAddNotice, strZoneCode, strBlockCode;
	CStringArray saTemp;
	int nSize = 0;
	int nPrevCnt = 0;



	return T_OK;
}

// 통합티켓/놀이공원
int CTranCmn::fnAPP_TranTicket_SubStep_04_Proc()
{
	int		ScrNum = 0;
	CString GetKeyStr("");
	CString strTemp("");
	CString strPlayCode, strGoodCode;
	CStringArray saTemp;
	int nSize = 0;
	int nPrevCnt = 0;


	return T_OK;
}

// 통합티켓/예약발권
int CTranCmn::fnAPP_TranTicket_SubStep_05_Proc()
{
	int		ScrNum = 0;
	CString GetKeyStr("");
	CString strTemp("");
	CString strTickCode;
	CStringArray saTemp;
	//int nSize;
	int nPrevCnt = 0;

	// 행사 선택


	return T_OK;
}

// 통합티켓/영화제
CString CTranCmn::fnAPP_TranTicket_SubStep_06_Proc()
{
	int		ScrNum = 0;
	CString GetKeyStr("");
	CString strTemp("");
	CString strPiffCode, strPiffTran, strAddNotice;
	CStringArray saTemp;
	int nSize = 0;
	int nPrevCnt = 0;



	return GetKeyStr;
}

int CTranCmn::fnAPP_TranTicket_SubStep_Accept_Card_Pwd_Proc()
{
	//Card Avail Setting
	MenuTblTemp[0].MenuInfo[10][0] = MENU_MCU2;

	fnAPP_MenuCardPbMuAccept(MENU_MCU);

	if(CardTran & MENU_MCU2);
	else
	{
		fnAPP_CancelProc(T_DISMSG, TranMsgTbl[6][m_pDevCmn->fnSCR_GetCurrentLangMode()]);	
	}

	if (!MenuFICMode)
	{
		fnAPD_MaterialOut(DEV_MCU);							// Card Out
		fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract
	}

	fnAPP_AcceptPassword();

	return T_OK;
}




int CTranCmn::fnAPP_TranTicket_SubStep_07_Proc()
{
	int		ScrNum = 0;
	CString GetKeyStr("");
	CString strTemp("");

	return T_OK;
}

int CTranCmn::fnAPP_TranTicket_SubStep_08_Proc()
{
	int		ScrNum = 0;
	CString GetKeyStr("");
	CString strTemp("");
	CString strStoryCode, strZoneCode, strGradeCode;
	CStringArray saTemp;
	int nSize = 0;

	return T_OK;
}

int CTranCmn::fnAPP_TranTicket_SubStep_09_Proc()
{

	return T_OK;
}

int CTranCmn::fnAPP_TranTicket_SubStep_10_Proc()
{


	return T_OK;
}