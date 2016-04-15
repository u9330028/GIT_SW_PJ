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
//	�޴��Լ�
/////////////////////////////////////////////////////////////////////////////
// Main Menu Display
int CTranCmn::fnAPP_MainMenuDisplay(int InitFlg)
{
	static	int		TranAvailSave = 0 ;							// �ŷ����º���
	static	int		TranICExistInfo = 0 ;						// Dip Card MediaStatus 
	int		i;
	int		ScrNum = 0;
	char	WithMsg[81], DepMsg[81];
	char	HanDeviceMsg[81], EngDeviceMsg[81];
	CString strGetAtmNum("");

	if (!InitFlg)
	{
		if ((m_pDevCmn->fnSCR_GetCurrentScreenNo() == MenuTblTemp[MenuIndex].MenuScrNum) &&
			(m_pDevCmn->TranAvail == TranAvailSave))			// �ŷ�����üũ
			return T_OK;

	}
	
	TranAvailSave = m_pDevCmn->TranAvail;						// �ŷ����º���
	fnAPP_InitializeDS();										// �����ʱ�ȭ
	memcpy(MenuTblTemp, MenuTbl, sizeof(MenuTblTemp));			// �ŷ���ư�ʱ�ȭ
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

	TranAvail = m_pDevCmn->TranAvail;							// ��ü�ŷ�����
	WithAvail = m_pDevCmn->WithAvail;							// ��ݰŷ�����
	DepAvail = m_pDevCmn->DepAvail;								// �Աݰŷ�����
	PbmAvail = m_pDevCmn->PbmAvail;								// ����ŷ�����
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_MainMenuDisplay:TranAvail[%x], WithAvail[%x], DepAvail[%x], PbmAvail[%x]", TranAvail, WithAvail, DepAvail, PbmAvail);

	if (m_pDevCmn->AtmDefine.DesHandler == NOT_SET)				// �ؿܰŷ���
	{
///		m_pDevCmn->DesTranStatus = FALSE;						// OutGoing�Ұ�
	}

	if (m_pDevCmn->AtmDefine.IrdaHandler == NOT_SET)			// ���ܼ��� 
	{
///		MenuTblTemp[0].MenuInfo[2][0] = MENU_NULL;				// �޴����ŷ�
	}
	else
	if (m_pDevCmn->fnIRD_GetDeviceStatus() == DOWN)
	{
///		MenuTblTemp[0].MenuInfo[2][0] = MENU_NULL;				// �޴����ŷ�
	}
	else
	if (!m_pDevCmn->fnAPL_GetYesDevice(DEV_IRD))				// ���ܼ�Ȯ��
	{
///		MenuTblTemp[0].MenuInfo[2][0] = MENU_NULL;				// �޴����ŷ�
	}

//	MenuTblTemp[3].MenuInfo[11][0] = MENU_NULL;					// �ؿܰŷ�
//	MenuTblTemp[6].MenuInfo[11][0] = MENU_NULL;					// �ؿܰŷ�

	switch (WithAvail)											// ��ݰŷ����� 	
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
			fnAPP_MenuClearTranProc(0, TRAN_WITH, MENU_NULL);	// ��ݻ���
			fnAPP_MenuClearTranProc(0, TRAN_FOREGIN, MENU_NULL);	//�ؿ�ī�� ��ݻ��� #N0171
			break;
	}

	switch (DepAvail)											// �Աݰŷ�����
	{
		case TRAN_DEP_CASH:
		case TRAN_DEP_CHECK:
		case TRAN_DEP_BOTH:								
			break;
		case TRAN_DEP_NOT:
		default: 
			fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);	// �Աݻ���
			break;
	}

	switch (PbmAvail)											// ����ŷ�����
	{
		case TRAN_PBM_OK:										
			break;
		case TRAN_PBM_NOT:
		default:												// �������
			fnAPP_MenuClearTranProc(0, TRAN_PB, MENU_NULL);
			fnAPP_MenuClearMenuCode(0, MENU_PB, ~MENU_PB);
			break;
	}
	
	if(!m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
	{
		fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);	// �Աݻ���
		fnAPP_MenuClearTranProc(0, TRAN_WITH, MENU_NULL);	// ��ݻ���
		fnAPP_MenuClearTranProc(0, TRAN_FOREGIN, MENU_NULL);	//�ؿ�ī�� ��ݻ��� #N0171
	}
	


	//���̽� ��û ������ - ������ ����� ����
	if(m_pProfile->DEVICE.SightHandicapped  != TRUE || (m_pProfile->DEVICE.ETCDevice20 != NONGHYUP && m_pProfile->DEVICE.ETCDevice20 != SC))  //#N0219 �������� ����� ����
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
//		fnAPP_MenuClearTranCode(0, TC_WITHDRAW, ~MENU_PB);		// #N0227 �������� ���� �Աݸ� ����
//		fnAPP_MenuClearTranCode(0, TC_TRANSFER, ~MENU_PB);		// #N0227 �������� ���� �Աݸ� ����

		m_pDevCmn->fnSCR_DisplayImage(8, FALSE);  //����
		m_pDevCmn->fnSCR_DisplayImage(13, FALSE); //����ȭ��
	}
	
	//#0052
	if(!fnAPP_MenuBeforeSprCheck())
	{
		fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);	// �Աݻ���
	}

// 	if(m_pDevCmn->AtmDefine.MachineType == U8100)
// 	{
// 		if (!m_pDevCmn->DesITKStatus && !m_pDevCmn->DesTMKStatus)
// 		{
// 			fnAPP_MenuClearTranProc(0, TRAN_FOREGIN, MENU_NULL);		//�ؿ�ī�� ��ݻ��� #N0171
// 			fnAPP_MenuClearTranProc(0, TRAN_FOREGIN_INQ, MENU_NULL);	//�ؿ�ī�� ��ȸ���� #N0171
// 		}
// 	}


	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 1�ܰ�����)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 2�ܰ�����)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 3�ܰ�����)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 4�ܰ�����)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 5�ܰ�����)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 6�ܰ�����)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 7�ܰ�����)
	memcpy(&MenuTblSave, &MenuTblTemp, sizeof(MenuTblSave));	// �޴����̺���

	fnAPP_MenuAvailCheck(MenuIndex);							// �޴����ɻ��±��ϱ�
	if (MenuAvail & MENU_MCU)									// ī���밡��
	{
		fnAPD_ICCardEnDisable(ENABLE, TRUE); 			
	}
	else
		fnAPD_CardEnDisable(DISABLE, TRUE);

	if (MenuAvail & MENU_PB)									// �����밡��
		fnAPD_PbEnDisable(ENABLE, TRUE);
	else
		fnAPD_PbEnDisable(DISABLE, TRUE);

	fnAPP_MenuBtnCheck(MenuIndex);								// Menu Button Check
	ScrNum = MenuTblTemp[MenuIndex].MenuScrNum;

	// ���Ⱑ ���� ���� ��� ����ȭ�� Display�� ������ ���� ������
	if (m_pProfile->DEVICE.MachineType == U8100)			
	{
		if ((InitFlg) && (m_pProfile->DEVICE.EarHandler) && (m_pProfile->DEVICE.ETCDevice20 == NONGHYUP))
			m_pDevCmn->EarPhoneInserted = m_pDevCmn->fnSNS_GetEarPhoneStatus();
		
		if (m_pDevCmn->EarPhoneInserted)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��    �� : [����� EarPhone Inserted]"));
			m_pDevCmn->fnSCR_SetDisplayData(EAR_KINDMODE, "on");
		}
	}

	for (i = 1; i < itemof(MenuTblTemp[0].MenuInfo); i++) 
	{															// ��ư����
		if ((MenuTblTemp[MenuIndex].MenuInfo[i][0] == MENU_NULL) && 
			(MenuTbl[MenuIndex].MenuInfo[i][0] != MENU_NULL))
			m_pDevCmn->fnSCR_DisplayImage(i, FALSE);
	}

	fnAPP_MenuTranMsg(WithAvail, WithMsg, DepAvail, DepMsg);
	fnAPP_MenuMaterialMsg(KOR_MODE, HanDeviceMsg);
	fnAPP_MenuMaterialMsg(ENG_MODE, EngDeviceMsg);
	fnAPP_MenuWithDepMsg();			

	switch (m_pDevCmn->AtmDefine.MachineType)					// �ʱ�ȭ������
	{
		case U8100:
		case U3100K:
			strGetAtmNum.Format("%6.6s", &m_pProfile->NETWORK.AtmSerialNum[2]);
			m_pDevCmn->fnSCR_DisplayString(1, "1577 - 0068");			
			m_pDevCmn->fnSCR_DisplayString(2, strGetAtmNum);		

			m_pDevCmn->fnSCR_DisplayString(11, StrDepMsg);			// �Ա��������
			m_pDevCmn->fnSCR_DisplayString(12, StrWithMsg);			// ����������
			m_pDevCmn->fnSCR_DisplayString(13, StrSprMsg);			// ��ǥ�������
			m_pDevCmn->fnSCR_DisplayString(1001, m_RXHOSTMSG.host_Allmsg);			// ����������
			m_pDevCmn->fnSCR_DisplayString(1006, "C:\\T1ATM\\APP\\SWF\\MainAD.swf");
			break;
		default:
			m_pDevCmn->fnSCR_DisplayString(1,  GetSprintf("%s", WithMsg));			// ����������
			break;
	}

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_NO_WAIT, PIN_DISABLE_MODE);

	m_pDevCmn->UserTranAvail = TranAvail;						// User��ü�ŷ�����
	m_pDevCmn->UserWithAvail = WithAvail;						// User���ްŷ�����
	m_pDevCmn->UserDepAvail = DepAvail;							// User�Աݰŷ�����
	m_pDevCmn->UserPbmAvail = PbmAvail;							// User����ŷ�����

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
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MainMenuDeviceEvent : �ŷ������ν�");
		m_pDevCmn->DeviceTran = DEV_SCR;
		return T_OK;
	}
	
	while (TRUE)
	{
		Delay_Msg();											// Msg & Task

		CheckDev |= DEV_SCR;									// Mouse Or Touch

		if ((!CardTran)									&&		// ī�幫
			(DevEnDisable & DEV_MCU)					)
			CheckDev |= DEV_MCU;

		if ((!PbTran)									&&		// ���幫
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
			return T_TIMEOVER;									// AtmModeCtrl���
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
		"   ���ݸ�  ��ݰ���   ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"   ��ǥ��  ��ݰ���   ",
		"   CHECK  AVAILABLE   ",
		"   CHECK  AVAILABLE   ",
		"   CHECK  AVAILABLE   ",
		"  ����/��ǥ ��ݰ���  ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		"       ��ݺҰ�       ",
		"    NO WITHDRAWAL     ",
		"    NO WITHDRAWAL     ",
		"    NO WITHDRAWAL     ",
		"   ���ݸ�  ��ݰ���   ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"   ���ݸ�  ��ݰ���   ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"    CASH AVAILABLE    ",
		"  ����/��ǥ ��ݰ���  ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		"  ����/��ǥ ��ݰ���  ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		"  ����/��ǥ ��ݰ���  ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE ",
		" CASH/CHECK AVAILABLE "
	};

	char*	WithMsgTblWith100Check[9][4] =					
	{
//		 123456789012345678901234567
		"     ���ݸ�  ��ݰ���     ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		" 1�鸸�� ��ǥ�� ��ݰ���  ",
		"     CHECK  AVAILABLE     ",
		"     CHECK  AVAILABLE     ",
		"     CHECK  AVAILABLE     ",
		" ����/1�鸸����ǥ��ݰ��� ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		"         ��ݺҰ�         ",
		"      NO WITHDRAWAL       ",
		"      NO WITHDRAWAL       ",
		"      NO WITHDRAWAL       ",
		"     ���ݸ�  ��ݰ���     ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		"     ���ݸ�  ��ݰ���     ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		"      CASH AVAILABLE      ",
		" ����/1�鸸����ǥ���ް��� ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		" ����/1�鸸����ǥ���ް��� ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		" ����/1�鸸����ǥ���ް��� ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   ",
		"   CASH/CHECK AVAILABLE   "
	};

	char*	DepMsgTbl[4][4] = 
	{
//		 1234567890123456789012
		"   ���ݸ�  �Աݰ���   ",
		"  CASH  DEPOSIT ONLY  ",
		"  CASH  DEPOSIT ONLY  ",
		"  CASH  DEPOSIT ONLY  ",
		"   ��ǥ��  �Աݰ���   ",
		"  CHECK DEPOSIT ONLY  ",
		"  CHECK DEPOSIT ONLY  ",
		"  CHECK DEPOSIT ONLY  ",
		"  ����/��ǥ �Աݰ���  ",
		"  CASH/CHECK DEPOSIT  ",
		"  CASH/CHECK DEPOSIT  ",
		"  CASH/CHECK DEPOSIT  ",
		"       �ԱݺҰ�       ",
		"      NO DEPOSIT      ",
		"      NO DEPOSIT      ",
		"      NO DEPOSIT      "
	};

	strcpy(WithMsg, "");										// ��������ʱ�ȭ
	strcpy(DepMsg, "");											// �Ա������ʱ�ȭ


	if (m_pDevCmn->fnAPL_GetKindOfOutCheck() == CHECKKIND_MILLION)
	{															
		switch (WithAvail)										// ��ݰŷ����� 	
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
		switch (WithAvail)										// ��ݰŷ����� 	
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
	{															// 100�����Ǽ�ǥ����-������������
		switch (WithAvail)										// ��ݰŷ����� 	
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

	switch (DepAvail)											// �Աݰŷ�����
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
		"        ī�带 �־� �ֽʽÿ�.       ",
		"      Please insert your card.      ",
		"      Please insert your card.      ",
		"      Please insert your card.      ",
		"        ������ �־� �ֽʽÿ�.       ",
		"    Please insert your passbook.    ",
		"    Please insert your passbook.    ",
		"    Please insert your passbook.    ",
		"   ī�� �Ǵ� ������ �־� �ֽʽÿ�.  ",
		"Please insert your card or passbook.",
		"Please insert your card or passbook.",
		"Please insert your card or passbook."
	};

	strcpy(DeviceMsg, "");										// ��ü�����ʱ�ȭ

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
		j = 0;													// 0���޴��ۼ�
		MenuTblWork[i].MenuInfo[j][0] = MENU_NULL;				// �ʱ�ȭ

		if (!MenuTblWork[i].MenuInfo[j][1])						// 0���̸����ȭ��ó��
			MenuTblWork[i].MenuInfo[j][1] = i;
		for (k = 0; k < itemof(MenuTblWork[0].MenuInfo); k++)	// ī�弱��ú�
		{
			MenuTblWork[i].MenuInfo[j][0] |= 
				MenuTblWork[MenuTblWork[i].MenuInfo[j][1]].MenuInfo[k][0];
		}

		if (!MenuTblWork[i].MenuInfo[j][2])						// 0���̸����ȭ��ó��
			MenuTblWork[i].MenuInfo[j][2] = i;
		for (k = 0; k < itemof(MenuTblWork[0].MenuInfo); k++)	// ���弱��ú�
		{
			MenuTblWork[i].MenuInfo[j][0] |= 
				MenuTblWork[MenuTblWork[i].MenuInfo[j][2]].MenuInfo[k][0];
		}

		if (!MenuTblWork[i].MenuInfo[j][3])						// 0���̸����ȭ��ó��
			MenuTblWork[i].MenuInfo[j][3] = i;
		for (k = 0; k < itemof(MenuTblWork[0].MenuInfo); k++)	// �Է¼���ú�
		{
			MenuTblWork[i].MenuInfo[j][0] |= 
				MenuTblWork[MenuTblWork[i].MenuInfo[j][3]].MenuInfo[k][0];
		}

		for (j = 1; j < itemof(MenuTblWork[0].MenuInfo); j++)	// 1-n���޴��ۼ�
		{
			if (MenuTblWork[i].MenuInfo[j][3])					// �ŷ�����ú�
			{
				MenuTblWork[i].MenuInfo[j][0] = MENU_NULL;		// �ʱ�ȭ
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
			if (MenuTblTemp[Index].MenuInfo[j][0] & MenuCode)	// �޴�������
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
			if (MenuTblTemp[Index].MenuInfo[j][4] == TranValue)	// �ŷ�ó���ڵ��
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
			if (MenuTblTemp[Index].MenuInfo[j][5] == TranValue)	// �ŷ��ڵ��
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
	if (SubIndex)												// ����ó��
		MenuAvail |= MenuTblTemp[Index].MenuInfo[SubIndex][0];
	else														// ó������&�޴���ü
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

	MenuBtnCnt = 0;												// �޴����ð����ѹ�ư��
	MenuBtnIndex = 0;											// �޴��ڵ����ð�����������ư��
	for (i = 1; i < itemof(MenuTblTemp[0].MenuInfo); i++)		// ��ư�����ϱ�
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
	m_pDevCmn->fnAPL_CaptureFaceSave(2, TRUE);					// ���Կ�
	m_pDevCmn->fnAPL_CaptureHandSave(1);						// ���Կ�

	while (TRUE)
	{
		fnAPP_MenuDeviceProc();									// �޴����ɻ�������

		if (TranProc && TranCode)								// �ŷ�ó��, �ŷ��ڵ�
			break;

		if (!MenuIndexNext)
			fnAPP_CancelProc(T_MSG, "�ش� ������ ���� ���� �����Դϴ�");
		else
		{
			MenuIndex = MenuIndexNext;
			MenuIndexSub = 0;
		}

		if (MenuTblTemp[MenuIndex].MenuInfo[0][0] != MENU_NULL)	// ó������(��ü���Կ�û��)
		{
			if (MenuTblTemp[MenuIndex].MenuInfo[0][0] & CardTran)
			{													// ī�������
				m_pDevCmn->DeviceTran = DEV_MCU;
				fnAPP_MenuDeviceProc();							// �޴����ɻ�������
				if (TranProc && TranCode)						// �ŷ�ó��, �ŷ��ڵ�
					break;
				if (MenuIndex != MenuIndexNext)					// �ٸ�ȭ���̵�ó�� : 
					continue;					
			}
			else
			if (MenuTblTemp[MenuIndex].MenuInfo[0][0] & PbTran)
			{													// ���������
				m_pDevCmn->DeviceTran = DEV_PBM;
				fnAPP_MenuDeviceProc();							// �޴����ɻ�������
				if (TranProc && TranCode)						// �ŷ�ó��, �ŷ��ڵ�
					break;
				if (MenuIndex != MenuIndexNext)					// �ٸ�ȭ���̵�ó�� : 
					continue;					
			}
		}

		if (CardTran)											// ó������(ī�������)
		{
			m_pDevCmn->DeviceTran = DEV_MCU;
			fnAPP_MenuDeviceProc();								// �޴����ɻ�������
			if (TranProc && TranCode)							// �ŷ�ó��, �ŷ��ڵ�
				break;
			if (MenuIndex != MenuIndexNext)						// �ٸ�ȭ���̵�ó�� :
				continue;					
		}


		if (PbTran)												// ó������(���������)
		{
			m_pDevCmn->DeviceTran = DEV_PBM;
			fnAPP_MenuDeviceProc();								// �޴����ɻ�������
			if (TranProc && TranCode)							// �ŷ�ó��, �ŷ��ڵ�
				break;
			if (MenuIndex != MenuIndexNext)						// �ٸ�ȭ���̵�ó�� : 
				continue;					
		}

		if(CardTran)								// ó������(ī�������)
			fnAPP_MenuClearMenuCode(0, MENU_MU, ~MENU_MU);

		fnAPP_MenuDisplay();									// �޴����
		fnAPP_MenuDeviceEvent();								// �޴��̺�Ʈó��
	}

	if ((!CardTran) && (!PbTran) && (!PbTran))					// ī��, ���幫 
		fnAPP_MenuCardPbMuAccept();								// ī��, ����, ����ü

	// �ŷ��ڵ庯���߰��κ�
	TranProcSave = TranProc;									// ����������
	TranCodeSave = TranCode;									// ����������
	TranCode2Save = TranCode2;									// ����������

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc(%8.8x, %d, %d)", MenuAvail, CardNew, TranCode);

	fnAPP_MenuAvailCheck(MenuIndex, MenuIndexSub);				// �޴����ɻ��±��ϱ�

	if (MenuMuTran)
	{
		WithAvail &= ~TRAN_WITH_CHECK;							// ��ǥ��ݺҰ�
		PbmAvail &= ~TRAN_PBM_OK;								// ����ŷ��Ұ�
	}

	if (MenuAvail & MENU_MCU2)									// 2TR�ŷ�
	{
//		WithAvail &= ~TRAN_WITH_CHECK;							// ��ǥ��ݺҰ�
		DepAvail &= ~TRAN_DEP_BOTH;								// �ԱݺҰ�
		PbmAvail &= ~TRAN_PBM_OK;								// ����ŷ��Ұ�
	}

	if (MenuAvail & MENU_TT)									// Ÿ���������
	{
//		if (TranProc == TRAN_DEP)								// �Ա� Ÿ���Ա�����
//			fnAPP_CancelProc(T_MSG, "Ÿ��ŷ��� �Ա��� �Ұ� �մϴ�");
		if (TranProc == TRAN_PB)								// ��������
			fnAPP_CancelProc(T_MSG, "Ÿ��ŷ��� ���������� �Ұ� �մϴ�");

//		WithAvail &= ~TRAN_WITH_CHECK;							// ��ǥ��ݺҰ�
		DepAvail &= ~TRAN_DEP_CHECK;							// ��ǥ�ԱݺҰ� Ÿ���Ա�����
//		DepAvail &= ~TRAN_DEP_BOTH;								// �ԱݺҰ�
		PbmAvail &= ~TRAN_PBM_OK;								// ����ŷ��Ұ�
		TranAvail = (WithAvail | DepAvail | PbmAvail);			// ��ü �ŷ� ���� check
	}

	TranAvail = (WithAvail | DepAvail | PbmAvail);				// ��ü �ŷ� ���� check
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc:CardTran[%x], PbTran[%x]",
	CardTran, PbTran);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc:MenuAvail[%x], TranProc[%d], TranCode[%d], TranCode2[%d]",
	MenuAvail, TranProc, TranCode, TranCode2);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc:WithAvail[%x], DepAvail[%x], PbmAvail[%x], TranAvail[%x]",
	WithAvail, DepAvail, PbmAvail, TranAvail);

	if ((CardTran) && (PbTran))									// ���ø�ü����
		fnAPP_CancelProc(T_MSG, "    ī��,���� ���� ����� �Ұ����մϴ�");
	MsgDumpAnal(TRACE_FREE_MODE, "AP", "[TRANKIND][%s]", 
										(TranProc == TRAN_WITH)	 ? "���"		:
										(TranProc == TRAN_PB)	 ? "��������"	:
										(TranProc == TRAN_TRANS) ? "��ü"		:
										(TranProc == TRAN_DEP)   ? "�Ա�"		:
										(TranProc == TRAN_INQ)   ? "��ȸ"		:
										(TranProc == TRAN_IC)    ? "IC"			:
										(TranProc == TRAN_RF)	 ? "����ī��"	: "��Ÿ");

	return T_OK;
}

// Menu Display
int CTranCmn::fnAPP_MenuDisplay()
{
	int		i;
	int		ScrNum = 0;
	int		MenuAvailSave = MenuAvail;
	char	WithMsg[81], DepMsg[81];

	fnAPP_MenuAvailCheck(MenuIndex);							// �޴����ɻ��±��ϱ�
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc(%x]",MenuAvail);
	if ((MenuAvail & MENU_MCU)	&&								// ī���밡��
		(!MenuIrdaMode		 )	)							
	{
		fnAPD_CardEnDisable(ENABLE);
	}
	else
	{
		fnAPD_CardEnDisable(DISABLE);
	}

	if (MenuAvail & MENU_PB)									// �����밡��
		fnAPD_PbEnDisable(ENABLE);
	else fnAPD_PbEnDisable(DISABLE);

	fnAPP_MenuBtnCheck(MenuIndex);								// �޴����ð����ѹ�ư�����ϱ�

	if (m_pDevCmn->EarPhoneInserted)							// #N0193 �����ATM
	{
		WithAvail &= ~TRAN_WITH_CHECK;							// ��ǥ��ݺҰ�
		DepAvail &= ~TRAN_DEP_CHECK;							// ��ǥ�ԱݺҰ�
		if(WithAvail == WITH_NOT_AVAILABLE)
			fnAPP_MenuClearTranProc(0, TRAN_WITH,  MENU_NULL);
		if(DepAvail == DEP_NOT_AVAILABLE)
			fnAPP_MenuClearTranProc(0, TRAN_DEP,  MENU_NULL);
	}

	if (TranProc && TranCode);									// �ŷ�ó��, �ŷ��ڵ�
	else
	if ((MenuBtnCnt == 1)	&&									// 1��ư
		(MenuBtnIndex))											// �ڵ����ù�ư�������
	{
		return T_OK;
	}

	ScrNum = MenuTblTemp[MenuIndex].MenuScrNum;

	for (i = 1; i < itemof(MenuTblTemp[0].MenuInfo); i++) 
	{															// ��ư����
		if ((MenuTblTemp[MenuIndex].MenuInfo[i][0] == MENU_NULL) && 
			(MenuTbl[MenuIndex].MenuInfo[i][0] != MENU_NULL))
		{
			m_pDevCmn->fnSCR_DisplayImage(i, FALSE);
		}
	}
	
	MenuAvailSave = MenuAvail;
	MenuAvail = MenuAvailSave;									// ���尪����

	fnAPP_MenuTranMsg(WithAvail, WithMsg, DepAvail, DepMsg);
	fnAPP_MenuWithDepMsg();				//#N0217
	switch (m_pDevCmn->AtmDefine.MachineType)					// �ʱ�ȭ������
	{
		case U3100K:
		case U8100:
			m_pDevCmn->fnSCR_DisplayString(11, StrDepMsg);			// �Ա�������� #N0217
			m_pDevCmn->fnSCR_DisplayString(12, StrWithMsg);			// ���������� #N0217
			m_pDevCmn->fnSCR_DisplayString(13, StrSprMsg);			// ��ǥ�������
			break;
	}

	//[�����ʿ�ݵ���]
	if (ScrNum == 2)
	{
		m_pDevCmn->fnSCR_DisplayImage(8, FALSE);  //����
		m_pDevCmn->fnSCR_DisplayImage(13, FALSE); //����ȭ��
	}
	else
	if (ScrNum == 10)
	{
		m_pDevCmn->fnSCR_DisplayImage(2, FALSE);  //���� ��ü
	}
	else
	if (ScrNum == 121)
	{
		//#CS0004
		//m_pDevCmn->fnSCR_DisplayImage(3, FALSE);  //JCB
		m_pDevCmn->fnSCR_DisplayImage(6, FALSE);  //��Ÿ
	}



	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
																// ȭ�����(�޴�ȭ�������� ALL_MODE�� �⺻ó��)
	return T_OK;
}

// Menu Device Event
int CTranCmn::fnAPP_MenuDeviceEvent(int CheckTime)
{
	int	   nCardEntering = 0;
	ULONG  WiatTime = TimerSet(CheckTime);
	
		if (m_pDevCmn->EarPhoneInserted == TRUE)				
			m_pDevCmn->fnPIN_EntryEnable(PINMODE_NORMAL, 0, 100, FALSE, PIN_NUMERIC_PAD, "", "",  -1);

	if (TranProc && TranCode);									// �ŷ�ó��, �ŷ��ڵ�
	else
	if ((MenuBtnCnt == 1)	&&									// 1��ư
		(MenuBtnIndex))											// �ڵ����ù�ư�������
	{
		m_pDevCmn->DeviceTran = DEV_SCR;						// �޴��ڵ�����
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
		
		if ((!CardTran)									&&		// ī�幫
			(DevEnDisable & DEV_MCU)					&&
			(m_pDevCmn->fnAPL_GetDeviceEvent(DEV_MCU))	&&
			(m_pDevCmn->DeviceTran == DEV_MCU))
			break;
	
		if ((!PbTran)									&&		// ���幫
			(DevEnDisable & DEV_PBM)					&&
			(m_pDevCmn->fnAPL_GetDeviceEvent(DEV_PBM))	&&
			(m_pDevCmn->DeviceTran == DEV_PBM))
			break;

 		fnAPP_MenuDeviceCheck();							// Menu Device Check //#0021

		if ((!CheckTime) ||
			(CheckTimer(WiatTime)))
			fnAPP_CancelProc(T_TIMEOVER);						// TranCmn���
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
									fnAPP_MenuClearTranProc(0, TRAN_EASYONE, MENU_NULL);  //#N0174 Ÿ��ī�� �Է½� ������ ��ư ����
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
			if (TranProc && TranCode)							// �ŷ�ó��, �ŷ��ڵ�
				break;

			MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[0][1];
			if (!MenuIndexNext)									// 0���̸����ȭ��ó��
				MenuIndexNext = MenuIndex;
			TranProc = MenuTblTemp[MenuIndex].MenuInfo[0][4];	// �ŷ�ó��
			TranCode = MenuTblTemp[MenuIndex].MenuInfo[0][5];	// �ŷ��ڵ�
			TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[0][6];	// �ŷ��ڵ�2

			break;
		case DEV_PBM:
			if(m_pProfile->DEVICE.ETCDevice20 == SC || m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)  //#N0252  #N0245 #N0277 SC ���� ������ ��� �ٱ��� ��ư�� ���� �ϱ� ����. 
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
				m_pDevCmn->fnSCR_DisplayScreen(805);			// �б�(����)
				m_pDevCmn->fnLGT_SetFlicker(KU_PBM_ATM_FLICKER, FLICKER_OFF);
				fnAPD_PbReadCheck();
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : PASSBOOK READ"));
			}

			fnAPP_MenuClearMenuCode(0, MENU_ALL, PbTran);		// ������ø޴�ó��

			if (TranProc && TranCode)							// �ŷ�ó��, �ŷ��ڵ�
				break;

			MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[0][2];
			if (!MenuIndexNext)									// 0���̸����ȭ��ó��
				MenuIndexNext = MenuIndex;
			TranProc = MenuTblTemp[MenuIndex].MenuInfo[0][4];	// �ŷ�ó��
			TranCode = MenuTblTemp[MenuIndex].MenuInfo[0][5];	// �ŷ��ڵ�
			TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[0][6];	// �ŷ��ڵ�2
			break;

		case DEV_PIN:										
		case DEV_SCR:										

			// ������ �ŷ��� ����ü, ����(���������� ����) �ŷ� ������
			if ((m_pDevCmn->EarPhoneInserted == TRUE) &&		
				((m_pDevCmn->fnSCR_GetCurrentScreenNo() == 1)))
			{
				MenuIndex = 17;

				MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[0][2];
				if (!MenuIndexNext)									// 0���̸����ȭ��ó��
					MenuIndexNext = MenuIndex;
				TranProc = MenuTblTemp[MenuIndex].MenuInfo[0][4];	// �ŷ�ó��
				TranCode = MenuTblTemp[MenuIndex].MenuInfo[0][5];	// �ŷ��ڵ�
				TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[0][6];	// �ŷ��ڵ�2
				
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:MenuIndex[%d], MenuIndexNext[%d], TranProc[%d], TranCode[%d], TranCode2[%d]", MenuIndex, MenuIndexNext, TranProc, TranCode, TranCode2);
				return T_OK;
			}

			if (TranProc && TranCode)
				;												// �ŷ�ó��, �ŷ��ڵ�
			else
			if ((MenuBtnCnt == 1)	&&							// 1��ư
				(MenuBtnIndex))									// �ڵ����ù�ư�������
			{
				GetKeyID = MenuBtnIndex;
				MenuIndexSub = GetKeyID;
				MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][3];
				if (!MenuIndexNext)								// 0���̸����ȭ��ó��
					MenuIndexNext = MenuIndex;
				TranProc = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][4];	// �ŷ�ó��
				TranCode = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][5];	// �ŷ��ڵ�
				TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][6];	// �ŷ��ڵ�2
				break;
			}

			GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString(FALSE);
			GetKeyID = 0;										// �ʱ�ȭ
			if (TranProc && TranCode)
				;							// �ŷ�ó��, �ŷ��ڵ�
			else
			if ((m_pDevCmn->fnSCR_GetCurrentScreenNo() == MenuTblTemp[MenuIndex].MenuScrNum) &&
				(GetKeyStr.GetLength()))						// ȭ��,����üũ
			{
				for (int i = 1; i < itemof(MenuTblTemp[0].MenuKind); i++)
				{												// ��ưã��
					if (!GetKeyStr.CompareNoCase(MenuTblTemp[MenuIndex].MenuKind[i]))
						GetKeyID = i;
				}
			}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:GetKeyStr[%s], GetKeyID[%d]", GetKeyStr, GetKeyID);

			if ((GetKeyID >= 1) && (GetKeyID <= 21))			// �޴���ư��
			{
				MenuIndexSub = GetKeyID;
				MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][3];

				if (!MenuIndexNext)								// 0���̸����ȭ��ó��
					MenuIndexNext = MenuIndex;

				TranProc = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][4];	// �ŷ�ó��
				TranCode = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][5];	// �ŷ��ڵ�
				TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][6];	// �ŷ��ڵ�2
				ScrNum = m_pDevCmn->fnSCR_GetCurrentScreenNo();

				if (!GetKeyStr.CompareNoCase("ȭ��Ȯ��")) 
				{	
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��    �� : [%s]", "���÷��� ȭ�� Ȯ��"));
					m_pDevCmn->SightHandiCapped = TRUE;

				}

				//����귣��  //#N0262 #N0246	#N0271
				
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "����귣��:GetKeyStr[%s], GetKeyID[%d]", GetKeyStr, GetKeyID);
				if(m_pDevCmn->fnSCR_GetCurrentScreenNo() == 1 || m_pDevCmn->fnSCR_GetCurrentScreenNo() == 2)
				{
					if((MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][0] & MENU_MCU) &&
						GetKeyStr.CompareNoCase("ȭ��Ȯ��"))		//#N0276
					{
							fnAPP_Accpet_AntiSkimming_Confirm();
					}
				}
				/*
				if (!GetKeyStr.CompareNoCase("�Ա�") || !GetKeyStr.CompareNoCase("���") || !GetKeyStr.CompareNoCase("��ȸ") || 
					!GetKeyStr.CompareNoCase("��ü") || !GetKeyStr.CompareNoCase("�ܱ�ī�����") || !GetKeyStr.CompareNoCase("������/����") ||
					!GetKeyStr.CompareNoCase("�ؿ�ī��") || !GetKeyStr.CompareNoCase("���ݼ�������") || !GetKeyStr.CompareNoCase("���ݼ�����ȸ")) 
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "����귣��:GetKeyStr[%s], GetKeyID[%d]!!!!", GetKeyStr, GetKeyID);
					if (PbTran & MENU_PB)						//����ŷ��� ǥ�� ����.
						;
					else
						fnAPP_Accpet_AntiSkimming_Confirm();
				}
				*/
				
				// �α��������� ����	#N0172
				if (!GetKeyStr.CompareNoCase("�α�IBK�ڳ���ȸ")) 
					fnAPL_NearBranchListView();
				
				if (!GetKeyStr.CompareNoCase("�ؿ�ī��")) 
					m_pDevCmn->fnSCR_SetCurrentLangMode(FOR_MODE); //#N0171

				if (!GetKeyStr.CompareNoCase("������")) 
					m_pDevCmn->fnSCR_SetCurrentLangMode(KEB_MODE); //#N0174

				if (m_pDevCmn->fnSCR_GetCurrentScreenNo() == 9)//����ü �ŷ� ����
				{
					if (m_pDevCmn->fnMCU_GetMaterialInfo() || m_pDevCmn->fnPBM_GetMaterialInfo())
					{
						fnAPP_CancelProc(T_MSG, "�� ��ü�δ� ���ŷ��� �� �� �����ϴ�");  //#N0219
					}
				}



				if (m_pDevCmn->fnSCR_GetCurrentScreenNo() == 103)// �ܱ����ȭ��
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�ܱ����ȭ��: GetKeyStr[%s], GetKeyID[%d]", GetKeyStr, GetKeyID);
					if ((GetKeyID == 1)		||		(GetKeyID == 4))							// ��������
						m_pDevCmn->fnSCR_SetCurrentLangMode(ENG_MODE);
					else
					if ((GetKeyID == 2)		||		(GetKeyID == 5))							// �Ϲ�����
						m_pDevCmn->fnSCR_SetCurrentLangMode(JPN_MODE);
					else
					if ((GetKeyID == 3)		||		(GetKeyID == 6))							// �ѹ�����
						m_pDevCmn->fnSCR_SetCurrentLangMode(CHN_MODE);
				}
			
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE)
				{
					//Foregin
					if(GetKeyStr.CompareNoCase("VISA_���") == 0) //#N0171
					{
						memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));
						memcpy(Accept.MuConfirmNum, "VV", 2);
						CardServiceFlag = VISA_CARD;	
					}
					else
					if(GetKeyStr.CompareNoCase("MASTER_���") == 0)
					{
						memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));
						memcpy(Accept.MuConfirmNum, "MM", 2);
						CardServiceFlag = MASTERCARD;	
					}
					else
					if(GetKeyStr.CompareNoCase("CUP_���") == 0)
					{
						memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));
						memcpy(Accept.MuConfirmNum, "CC", 2);
						CardServiceFlag = CUPCARD;	
					}
					else
					if(GetKeyStr.CompareNoCase("CUP_��ȸ") == 0)
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
					if(GetKeyStr.CompareNoCase("JCB_���") == 0)
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
			if (GetKeyStr == S_CANCEL)							// ��ҹ�ư
				fnAPP_CancelProc(T_CANCEL);
			else 
			if (GetKeyStr == S_TIMEOVER)						// �ð��ʰ�
				fnAPP_CancelProc(T_TIMEOVER);
			break;

		default: 
			break;
	}


	// ī�弱�� �� �޴������ (Motor-Type) 
	if((TranCode2 == TRANID_7200 || TranCode2 == TRANID_4310 || TranCode2 == TRANID_7220 || TranCode2 == TRANID_7260 \
		|| TranCode2 == TRANID_4811 || TranCode2 == TRANID_4810 || TranCode2 == TRANID_7321  || TranCode2 == TRANID_7326 || TranCode2 == TRANID_7500 || TranCode2 == TRANID_7503) \
		&& (CardTran & MENU_IC) && (m_pProfile->DEVICE.MCUHandler == O_TYPE))  //#N0174
	{
				
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "��fnAPP_MenuDeviceProc:FICAppletType[%d]", m_pDevCmn->FICAppletType);
		if (MenuAvail & MENU_MCU3)
		{
			if ((m_pDevCmn->FICAppletType & 0x01) && !(MenuFICMode & MENU_INFO_3))	//#0138		
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CARDREAD : IC READ"));

				if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 804)			
					m_pDevCmn->fnSCR_DisplayScreen(804);		// �б�(ī��)

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

					CardPowerOnFlg = FALSE;						// ī�����ӹ�

					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}
				else
				if (MenuFICMode & MENU_IC)						// ����IC�ŷ�ó��	
				{
					MenuFICMode = MENU_IC; 

					if (m_pDevCmn->FICAccountCount > 1)				// ���߰��½� PIN Check
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CARDREAD : IC READ -> ����(%d)", m_pDevCmn->FICAccountCount));
						fnAPP_AcceptKCashPassword(TRAN_IC_FINANCEIC);
																	// ��й�ȣ�Է�
						fnAPP_KCashCheckPin();						// ��й�ȣ�˻�
					}
					
					if(fnAPP_FICSelectAccount() == T_ERROR)			// ����IC ���¼�����
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// ī�����ӹ�

    					fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					else
					if(fnAPD_FICCardReadCheck() == T_ERROR)
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// ī�����ӹ�

						fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					else
					if(!(CardTran & MENU_J3T3))  //#0071
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// ī�����ӹ�

						fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					
				}
				else
				{
					MenuFICMode = MENU_NULL;
					MenuEPBMode = MENU_NULL;

					CardPowerOnFlg = FALSE;						// ī�����ӹ�

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

					CardPowerOnFlg = FALSE;							// ī�����ӹ�

					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}
			}
			fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);		// ī����ø޴�ó��
		}
		
		fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);			// ī����ø޴�ó��
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

			//Kim 11/20 ������IC ���� ����ó��
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
		if (FallBackCnt != 0x30)						// POWER ON ����
		{
			if ((TranCode2 == TRANID_4330) ||
				(TranCode2 == TRANID_7111) ||
				(TranCode2 == TRANID_7230))
			{
				if (fnAPD_ICCardReadCheck() != T_ERROR)				// ICī���б�		
				{

					if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 804)			
						m_pDevCmn->fnSCR_DisplayScreen(804);		// �б�(ī��)

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
						if (fnAPP_EMVReadProc() == T_ERROR)		// EMV�⺻����READ
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
				//Kim 11/20 ������IC ���� ����ó��
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
			//Kim 11/20 ������IC ���� ����ó��
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

	fnAPP_MenuAvailCheck(MenuIndex, MenuIndexSub);				// �޴����ɻ��±��ϱ�

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:MenuAvail[%x], MenuIndex[%d], MenuIndexSub[%d]", MenuAvail, MenuIndex, MenuIndexSub);

	if (!MenuAvail)
	{
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KEB_MODE)  // Cancel Msg �ѱ� ǥ�� #N0174
			m_pDevCmn->fnSCR_SetCurrentLangMode(KOR_MODE);
		
		if (MenuIndexSub)										// Mouse or Touch
		{
			if ((CardTran) && (MenuTbl[MenuIndex].MenuInfo[MenuIndexSub][0]))
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("ī��     : �� ī�����δ� �ش� �ŷ� �Ұ�")); 

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				{
					fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�");

				}
				else
					fnAPP_CancelProc(T_MSG, "Your card can not", "be used at this transaction");
			}
			if ((PbTran) && (MenuTbl[MenuIndex].MenuInfo[MenuIndexSub][0]))
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : �� �������δ� �ش� �ŷ� �Ұ�")); 

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "�� �������δ� ���ŷ��� �� �� �����ϴ�");
				else
					fnAPP_CancelProc(T_MSG, "Your passbook can not", "be used at this transaction");
			}
			
		}
		else													// MCU, PBM
		{
			if (CardTran)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("ī��     : �� ī�����δ� �ش� �ŷ� �Ұ�"));

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "�� ī��δ� ���ŷ��� �� �� �����ϴ�");
				else
					fnAPP_CancelProc(T_MSG, "Your card can not", "be used at this transaction");
			}
			if (PbTran)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("����    : �� �������δ� �ش� �ŷ� �Ұ�"));

				if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
					fnAPP_CancelProc(T_MSG, "�� �������δ� ���ŷ��� �� �� �����ϴ�");
				else
					fnAPP_CancelProc(T_MSG, "Your passbook can not", "be used at this transaction");
			}
		}
		fnAPP_CancelProc(T_MSG, "�ش� ������ ���� ���� �����Դϴ�");
	}

	fnAPP_MenuDeviceCheck();									// Menu Device Check //#0021
	return T_OK;
}
		
// Menu Device Check
int CTranCmn::fnAPP_MenuDeviceCheck()
{
	if ((m_pDevCmn->fnAPL_GetDeviceEvent(DEV_MCU))	&&			// ���ø�ü����
		(m_pDevCmn->fnAPL_GetDeviceEvent(DEV_PBM)))
		fnAPP_CancelProc(T_MSG, "ī��,���� ���� ����� �Ұ��մϴ�");

	if ((m_pDevCmn->fnMCU_GetMaterialInfo() & ST_SENSOR2)	&&	// ���ø�ü����
		(m_pDevCmn->fnPBM_GetMaterialInfo() & ST_SENSOR2))
		fnAPP_CancelProc(T_MSG, "ī��,���� ���� ����� �Ұ��մϴ�");
	

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
		fnAPP_MenuAvailCheck(MenuIndex, MenuIndexSub);			// �޴����ɻ��±��ϱ�
		ProcAvail = MenuAvail;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuCardPbMuAccept:ProcAvail[%x]", ProcAvail);
	}

	if (ProcAvail == MENU_IC)									// ICī���밡��
	{
		fnAPD_CardEnDisable(DISABLE, TRUE);
		fnAPD_PbEnDisable(DISABLE, TRUE);						// �ӵ����� : 2003.11.21(���ȿ��:300ms)
		fnAPD_ICCardEnDisable(ENABLE);
	}
	else
	if (ProcAvail & MENU_MCU)									// ī���밡��
		fnAPD_CardEnDisable(ENABLE);
	else fnAPD_CardEnDisable(DISABLE);

	
	if (ProcAvail & MENU_PB)									// �����밡��
		fnAPD_PbEnDisable(ENABLE);
	else fnAPD_PbEnDisable(DISABLE);

	if (TranCode == TC_HIPASS_CREDIT)
		ScrNum = 2921;
	else
	if ((ProcAvail & MENU_MCU)	&& 
		(ProcAvail & MENU_PB))
		ScrNum = 802;											// ī��/����
	else
	if (ProcAvail & MENU_MCU)
		ScrNum = 801;											// ī��
	else
	if (ProcAvail & MENU_PB)
		ScrNum = 803;											// ����
	else
	if (ProcAvail & MENU_MU)
	{
		fnAPD_CardEnDisable(DISABLE);
		fnAPD_PbEnDisable(DISABLE);
	}

	

	if (ScrNum)													// ��ü&Touch����ó��
	{
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
																// ȭ�����
		fnAPP_MenuDeviceEvent();								// �޴��̺�Ʈó��
		fnAPP_MenuDeviceProc();									// �޴����ɻ�������
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
	
	fnAPD_CardEnDisable(DISABLE);								// ī�����Ա���
	fnAPD_PbEnDisable(DISABLE);									// �������Ա���

	m_pDevCmn->fnIRD_EntryEnable();								// �����㰡
	DevEnDisable |= DEV_MCU;									// ī���ó������	2004.02.24
	m_pDevCmn->fnSCR_DisplayScreen(821, K_NO_WAIT, PIN_MENU_MODE);// ���ܼ��Է�

	nReturn = m_pDevCmn->fnCMN_ScanDeviceAction(DEV_IRD | DEV_SCR, K_65_WAIT, EVENT_IN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuIrdaAccept:nReturn[%d]", nReturn);
	if (nReturn == R_TIMEOUT)
	{
		m_pDevCmn->fnIRD_EntryDisable();						// ���Ա���
		fnAPP_CancelProc(T_TIMEOVER);
	}
	else
	if (nReturn == R_TIMEOVER)									// 2004.07.27 IRD TIMEOVERó����ȭ
	{
		m_pDevCmn->fnIRD_EntryDisable();						// ���Ա���
		fnAPP_CancelProc(T_TIMEOVER);
	}
	else
	if (nReturn == DEV_IRD)
	{
																// 2004.02.05 ���ܼ�����Ÿ ����ó��
		strReturn = m_pDevCmn->fstrIRD_GetData();
		m_pDevCmn->fnIRD_AnalData(strReturn.GetBuffer(strReturn.GetLength()));

		if ((m_pDevCmn->IRDAInfo.ISODataTag != 'A') &&			// �Ϲݰ�������
			(m_pDevCmn->IRDAInfo.ISODataTag != 'K') &&			// �Ϲݰ�������
			(m_pDevCmn->IRDAInfo.ISODataTag != 'C') )			// �ſ�����
		{
			fnAPP_CancelProc(T_MSG, "�� �޴������δ�  ", "�ŷ��� �Ͻ� �� �����ϴ�");
		}

		strReturn = "";											// 1Track��������
		strReturn += ",";										// Add Delemeter

		if ((m_pDevCmn->IRDAInfo.ISODataTag == 'C') ||			// �ſ����
			(m_pDevCmn->IRDAInfo.ISODataTag == 'H')	)			// �ſ����
		{
			MakeUnPack(m_pDevCmn->IRDAISO2Info.ISO2,			// Make 2Track Data
						strUnpackISO2, 
						__min(37, sizeof(m_pDevCmn->IRDAISO2Info.ISO2)));
			strReturn += strUnpackISO2;
		}
		strReturn += ",";										// Add Delemeter

		if ((m_pDevCmn->IRDAInfo.ISODataTag == 'A')	||			// �Ϲݰ���
			(m_pDevCmn->IRDAInfo.ISODataTag == 'K')	)			// �Ϲݰ���
		{
			MakeUnPack( m_pDevCmn->IRDAISO3Info.ISO3,			// Make 3Track Data
						strISO3, 
						__min(54, sizeof(m_pDevCmn->IRDAISO3Info.ISO3)),
						FALSE);

			MakeUnPack( strISO3.GetBuffer(strISO3.GetLength()),	// 3Track ī�嵥��Ÿ�м��� ���� �ٽ� UnPack
						strUnpackISO3, 
						strISO3.GetLength());

			strReturn += strUnpackISO3.Mid(2, 104*2);			// (UnPack����Ÿ�̹Ƿ� Mid������)
		}
																
		fnAPD_CardReadCheck(strReturn);							// ī��üũ
	}
	else
	if (nReturn == DEV_SCR)
	{
		strReturn = m_pDevCmn->fstrSCR_GetKeyString(FALSE);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuIrdaAccept:strReturn[%s]", strReturn);
		m_pDevCmn->fnIRD_EntryDisable();						// ���Ա���
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else fnAPP_CancelProc(T_TIMEOVER);
	}

	DevEnDisable &= (~DEV_MCU);									// ī��ó����������	2004.02.24
	return T_OK;
}

// Menu Irda/2DBarCode Accept
int CTranCmn::fnAPP_MenuIrdaBarCodeAccept()
{
	return T_OK;
}


// Menu Before Input Spr Check  (����ó��)	
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

	ScrNum = 801;											// ī��
	
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);

	fnAPP_MenuDeviceEvent();								// �޴��̺�Ʈó��

	if (m_pDevCmn->DeviceTran == DEV_SCR)
	{
		GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString(FALSE);

		if (GetKeyStr == S_CANCEL)							// ��ҹ�ư
			fnAPP_CancelProc(T_CANCEL);
		else 
		if (GetKeyStr == S_TIMEOVER)						// �ð��ʰ�
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
					m_pDevCmn->fnSCR_DisplayScreen(804);		// �б�(ī��)

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

					CardPowerOnFlg = FALSE;						// ī�����ӹ�

					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}
				else
				if (MenuFICMode & MENU_IC)						// ����IC�ŷ�ó��	
				{
					MenuFICMode = MENU_IC; 

					if (m_pDevCmn->FICAccountCount > 1)				// ���߰��½� PIN Check
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CARDREAD : IC READ -> ����(%d)", m_pDevCmn->FICAccountCount));
						fnAPP_AcceptKCashPassword(TRAN_IC_FINANCEIC);
																	// ��й�ȣ�Է�
						fnAPP_KCashCheckPin();						// ��й�ȣ�˻�
					}
					
					if(fnAPP_FICSelectAccount() == T_ERROR)			// ����IC ���¼�����
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// ī�����ӹ�

    					fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					else
					if(fnAPD_FICCardReadCheck() == T_ERROR)
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// ī�����ӹ�

						fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					else
					if(!(CardTran & MENU_J3T3))  //#0071
					{
						MenuFICMode = MENU_NULL;
						MenuEPBMode = MENU_NULL;

						CardPowerOnFlg = FALSE;						// ī�����ӹ�

						fnAPD_CardEnDisable(ENABLE, TRUE);	
						fnAPD_CardReadCheck();
						m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
					}
					
				}
				else
				{
					MenuFICMode = MENU_NULL;
					MenuEPBMode = MENU_NULL;

					CardPowerOnFlg = FALSE;						// ī�����ӹ�

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

					CardPowerOnFlg = FALSE;							// ī�����ӹ�

					fnAPD_CardEnDisable(ENABLE, TRUE);	
					fnAPD_CardReadCheck();
					m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
				}
			}
			fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);		// ī����ø޴�ó��
		}
		
		fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);			// ī����ø޴�ó��
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

	// �α������ȳ�
	ScrNum = 3210;

	m_pDevCmn->bNearBranchSelectBtn = FALSE;   //#N0172

	if (!ScrNum)
		fnAPP_CancelProc(T_PROGRAM);

	strTemp = IniGetStr(_TRANS_NEARBRANCH_INI, "1", "cname");
	if (strTemp.IsEmpty())
		fnAPP_CancelProc(T_MSG, "�˼��մϴ�.", "�α����� �ȳ��� ������ �� �����ϴ�.");

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
	//��� ���� ����
	if(WithAvail & TRAN_WITH_CASH)
		StrWithMsg = "1||";
	else
		StrWithMsg = "2||";


	if(WithAvail & TRAN_WITH_50TCASH )
		StrWithMsg += "1";
	else
		StrWithMsg += "2";


// �Աݰ��� ����
	if((DepAvail & TRAN_DEP_CASH) && (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) )  //��ǥ ��� �Ұ� �� �Ա� �ŷ� �ȵ�
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
		ScrNum = 801;											// ī��
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);

	fnAPP_MenuDeviceEvent();								// �޴��̺�Ʈó��
	//fnAPP_MenuDeviceProc();									// �޴����ɻ�������

	if (m_pDevCmn->DeviceTran == DEV_SCR)
	{
		GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString(FALSE);

		if (GetKeyStr == S_CANCEL)							// ��ҹ�ư
			fnAPP_CancelProc(T_CANCEL);
		else 
		if (GetKeyStr == S_TIMEOVER)						// �ð��ʰ�
			fnAPP_CancelProc(T_TIMEOVER);
	}


	fnAPD_CardReadCheck();
	m_pDevCmn->fnLGT_SetFlicker(KU_MCU_ATM_FLICKER, FLICKER_OFF);
	fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);			

	if(!(CardTran & MENU_MCU2))
		fnAPP_CancelProc(T_MSG, "�ſ�ī�常 ����� �����մϴ�.", "�ٽ� �õ��Ͽ� �ֽʽÿ�");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuCardMCU2Accept():return()");
	
	return T_OK;
}
