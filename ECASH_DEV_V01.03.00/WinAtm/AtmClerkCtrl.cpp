/////////////////////////////////////////////////////////////////////////////
// AtmClerkCtrl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
/////////////////////////////////////////////////////////////////////////////
/* ����������������������������������������������������������������������������
   �� ���Ϻ����̷�
  ������������������������������������������������������������������������������
  ��A P �� �� �� �������� ���׸��ȣ��  ���泻��(����)                        ��
  ������������������������������������������������������������������������������
    V03-01-01   2005.07.12  SPLó������
									  SCD(P)�� SPLó���� ����
   ����������������������������������������������������������������������������*/
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\H\Common\CmnLibIn.h"
#include "..\H\Common\Define.h"
#include "..\H\Common\ConstDef.h"
#include "..\H\Common\ClassInclude.h"
#include "..\H\Dll\DevCmn.h"
#include "..\H\Tran\TranCmn.h"

#include "WinAtm.h"
#include "WinAtmCtl.h"
#include "WinAtmPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// ���ó��
int	CWinAtmCtrl::ClerkProc()
{
	// OM_�α����Ϲޱ�
	// OM_��ġ����
	// OM_��ݼ�ǥ���||��ݼ�ǥ��ȣ||��ݼ�ǥ�ż�
	// OM_��ݼ�ǥ����
	// OM_�غ�
	// OM_�
	// OM_��ġ����||��ġ��¥||����||���||...
	// OM_�ΰ���ġ����||����ȭ��(����,�Ұ�)||...
	// OM_LAN��������_CD||BP IP||BP Port||����ȣ (VANIII)
	// OM_LAN��������||IP�ּ�||����ݸ���ũ||...   (C-ATM, T1,T2)
	// OM_ȸ������||�������||DA||...
	// OM_ATMS��������
	// OM_�ŷ���������||�������(����,�Ұ�)||..
	// OM_�Աݱ�������||��õ����/õ���� 
	// OM_ī��Ʈ��������||����(�Ÿ�)||����(�Ÿ�)||����(�Ÿ�)||����(�Ÿ�)
	// OM_�հ�
	// OM_�Ұ�
	// OM_BRM_�ݾ׼��� (C-ATM, VANIII)
	// OM_ȸ���ݵ�� (C-ATM, VANIII)
	// OM_��ü�ʱ�ȭ
	// OM_BRM�ʱ�ȭ
	// OM_��ü����
	// OM_��üȸ��
	// OM_�Ϻκ���||����ż�
	// OM_�Ϻ�ȸ��||ȸ���ż�
	// OM_����ݸ�弱��
	// OM_ī��Ʈ��������
	// OM_��ݿ켱����
	// OM_�Աݱ�������
	// OM_���������Ϻ�ȸ��
	// OM_��õ�����Ϻ�ȸ��
	// OM_�Ÿ������Ϻ�ȸ��
	// OM_��õ�����Ϻ�ȸ��
	// OM_����������üȸ��
	// OM_��õ������üȸ��
	// OM_�Ÿ�������üȸ��
	// OM_��õ������üȸ��
	// OM_��ǥ����||����1||����2||...||����19
	// OM_��������||����1||����2||...||����19
	// OM_��ǥ��������||����1||����2||...||����19
	// OM_��ü�׽�Ʈ
	// OM_��⼳��||���Ÿ��||��ǥ������
	// OM_��ǥ��������||��ǥ����(13,14,15,16)
	// OM_�ŷ��Ϸù�ȣ����||�Ϸù�ȣ
	// OM_��������
	// OM_����
	// OM_ī�޶��׽�Ʈ||CAM1||CAM2||CAM3||CAM4 (C-ATM, VANIII)
	// OM_�н�����||�н����� (VANIII)
	// OM_�귣�弳��||����������||�귣�弳�� (VANIII)
	// OM_��ġ��Ʈ����||SPACK ��Ʈ (VANIII)
	// OM_����������||ȸ������||����ȣ||��������Ʈ||ADSLID||ADSL PWD (VANIII)
	// OM_��ǥ����||0,1
	// OM_��������||0,1
	// OM_�ŷ��Ϸù�ȣ����||�Ϸù�ȣ
	// OM_��¥�ð�����||20120322122300
	// OM_�ŷ���������||���񽺰ŷ�����(1,2,3)||��������ȸ����(0,1)||�ִ�ݾ׼�������(30,50,70)||�ִ�ݾ׼���Ÿ��(30,50,70)	
	// OM_�ڵ��⸮����
	// OM_���ۺ�й�ȣ����||000000
	// OM_����������й�ȣ����||000000
	// OM_�����ýð�����||000000
	// OM_DEVICECHECK||000000
	// OM_DEVICEREGULARCHECK||000000
	// OM_VOLUME||OM_VOLUME=������
	// OM_PINMODE||PINTYPE (VANIII)
	// OM_POWEROFFSTATUS (VANIII)
	// OM_CLOSESHUTTER (VANIII)
	// OM_OPENSHUTTER (VANIII)
	
	static	int		ClerkProcInCnt = 0;							// ������Ƚ������
	static	CString	strOmResult("");							// OM���
	static	int		SetScreen = OFF;
	static	int		nOperatorSwitchStatus = OPER_NONE;
	static	int		nOperatorSwitchStatusSave = OPER_NONE;

	CString			strOmCommand("");
	CStringArray	strOmCommandArray;
	CString			strTemp("");
	CString			strTemp2("");
	CStringArray	strTempArray;
	int				nTemp = 0;
	int				i = 0;
	int				nErrorFlag = FALSE;
	static	BOOL	bOperChange = FALSE;
/////////////////////////////////////////////////////////////////////////////
// SPLó������													// V03-01-01
	nOperatorSwitchStatus = m_pDevCmn->fnSNS_GetOperatorSwitch();
//////////////////////////////////////////////////////////////////////////
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		if(SetScreen)
		{
			SetScreen = OFF;								// Next Clerk Procedure Stop
			m_pDevCmn->fnUTL_SetClerkScreen_T3_Van();										// ȭ�鱳��
		}
		else
		if (nOperatorSwitchStatus == OPER_SUPERVISORCHANGED ||
			nOperatorSwitchStatus == OPER_RUNCHANGED )
		{
			if ((nOperatorSwitchStatusSave != nOperatorSwitchStatus) && !SetScreen )
			{
				nOperatorSwitchStatusSave = nOperatorSwitchStatus;
				nTemp = m_pTranCmn->fnAPP_AcceptOpPassword();
				if (nTemp == T_CANCEL) 
				{
					m_pDevCmn->AtmStatusReEntryFlag = FALSE;
					return TRUE; 
				}
				m_pDevCmn->OpenKey = FALSE;
				strOmResult = OM_NORMAL;
				m_pDevCmn->fnAPL_ClerkInformation();				// �������
				SetShareData("OM_RESULT", strOmResult);				// ����ó��
				//////////////////////////////////////////////////////////////////////////
				SetScreen = ON;
				m_pDevCmn->fnAPL_UnUsed();							// Un Used Mode Setting
				//////////////////////////////////////////////////////////////////////////
			}

		}
	}
/*  //#0048
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		if ((OPER_SUPERVISORCHANGED == nOperatorSwitchStatus) || (OPER_RUNCHANGED == nOperatorSwitchStatus))
		{
			m_pDevCmn->fnSNS_ClearOperatorSwitch();				// Clear Sensor
			if (FALSE == SetScreen)
			{
				m_pDevCmn->fnCMR_PreviewToOM(FALSE);			// Preview�� OFF�� ���ϸ� ȭ�鿡 �ܻ��� ���׿�...
				m_pDevCmn->fnCMR_PreviewToFront(FALSE);
				SetScreen = TRUE;
				SetClerkScreen(TRUE);							// Front OM
				SetShareData("OM_ȭ�����", "����");
				SetShareData("OM_COMMAND","OM_�غ�");
				SetShareData("OM_RESULT","DO");
			}
			else
			{
				m_pDevCmn->fnCMR_PreviewToOM(FALSE);
				m_pDevCmn->fnCMR_PreviewToFront(FALSE);
				SetScreen = FALSE;
				SetClerkScreen(FALSE);							// Rear OM
				SetShareData("OM_COMMAND","OM_�");
				SetShareData("OM_RESULT","DO");
				m_pDevCmn->fnSCR_DisplayScreen(774);			// Black Screen
			}
		}
	}
*/
/////////////////////////////////////////////////////////////////////////////
	if (GetShareData("OM_RESULT") != OM_DO)						// �����û��
		return TRUE;

/////////////////////////////////////////////////////////////////////////////
	if (ClerkProcInCnt > 0)										// ������Ƚ������
	{
		ClerkProcInCnt--;
		if (ClerkProcInCnt == 0)
		{
			m_pDevCmn->fnAPL_ClerkInformation();				// �������
			SetShareData("OM_RESULT", strOmResult);				// ����ó��
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult[%s]", strOmResult);
			return TRUE;
		}
	}

/////////////////////////////////////////////////////////////////////////////
	strOmCommand = GetShareData("OM_COMMAND");					// Ŀ�ǵ�,�Ķ����...
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmCommand[%s]", strOmCommand);
	SplitString(strOmCommand, "||", strOmCommandArray);
	
/////////////////////////////////////////////////////////////////////////////
	strOmResult = OM_ERROR;										// �⺻�������
// Get Log Start ����������������������������������������������
	if (strOmCommandArray[0] == "OM_�α����Ϲޱ�")
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�α����Ϲޱ�]"));

		if (strOmCommandArray.GetSize() < 3)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�α����Ϲޱ� �����Դϴ�";
		}
		else
		{
			if (m_pDevCmn->fnAPL_GetLogFileStart(Asc2Int(strOmCommandArray[1]), Asc2Int(strOmCommandArray[2])))
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||�α����� �ޱⰡ ����ó���Ǿ����ϴ�.";
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||USB ��ġ�� Ȯ���Ͽ� �ֽʽÿ�.";
			}
		}
	}	
	else
	if (strOmCommandArray[0] == "OM_��ġ����")
	{
		m_pDevCmn->RecoveryType = 1;							// 0:�ڵ�����,1:��������
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ġ����]"), JNL_OPER);
		m_pDevCmn->fnAPL_SetSensorInfo();							// ������������
//t		if (m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetErrorDevice(m_pDevCmn->DeviceStatus)))
//		if (m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetErrorDevice(m_pDevCmn->DeviceStatus | m_pDevCmn->DeviceNoDevice)))	// 2003.11.11
		if (m_pDevCmn->RejectCSTLoc != NORMAL_POS)
		{
			strOmResult = OM_ERROR;
			strOmResult += "|| [ REJECT BOX ��ġ���� ]";
		}
		else
		if (m_pDevCmn->RetractCSTLoc != NORMAL_POS)
		{
			strOmResult = OM_ERROR;
			strOmResult += "|| [ RETRACT BOX ����ġ ����]";
		}
		else
		if (m_pDevCmn->BrmLoc != NORMAL_POS)
		{
			strOmResult = OM_ERROR;
			strOmResult += "|| [ BRM ����ġ ���� ]";
		}
		else
		if(m_pDevCmn->FstCSTStatus == CST_SET_NG)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||1ī��Ʈ Ż��";
		}
		else
		if(m_pDevCmn->SndCSTStatus == CST_SET_NG)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||2ī��Ʈ Ż��";
		}
		else
		if(m_pDevCmn->TrdCSTStatus == CST_SET_NG)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||3ī��Ʈ Ż��";
		}
		else //#0063
		{
			m_pDevCmn->RecoveryType = 1;							// 0:�ڵ�����,1:��������
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ġ����]"), JNL_OPER);
			m_pDevCmn->fnAPL_ClearError();							// ��������ʱ�ȭ
			m_pDevCmn->DeviceAutoOff = 0;							// �ڵ����� �� Device = Set
			m_pDevCmn->DeviceLoss = DEV_NONE;						// �н������ʱ�ȭ
			m_pDevCmn->ForceDownFlag = FALSE;						// ����Down����
			m_pDevCmn->RemoteDownFlag = FALSE;						// ����Down���� 
			m_pDevCmn->fnAPL_CshRetactCntInfo(0);								// ȸ��Ƚ�� Clear;
			if (m_pDevCmn->fnAPL_ResetDevice(DEV_MAIN))
			{
				m_pDevCmn->fnAPL_ClearError();							// ��������ʱ�ȭ
				strOmResult = OM_NORMAL;
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||��ġ���� FAIL!";
			}
		}
		m_pDevCmn->RecoveryType = 0;							// 0:�ڵ�����,1:��������
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ġ���� %s]", strOmResult));
	}
	else
	if (strOmCommandArray[0] == "OM_��ݼ�ǥ�հ�")
	{
 		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ݼ�ǥ�հ�]"));

		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == (DEV_SPR | DEV_JPR)) 
		{

			TranProc(TRAN_CHECK_UNREG);
			if (m_pDevCmn->TranResult)
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||��ǥ�հ谡 ó���Ǿ����ϴ�";
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||��ǥ�հ��� ��ְ� �߻��߽��ϴ�";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ,���� ��� �����Դϴ�";
		}

	}
	else
	if (strOmCommandArray[0] == "OM_��ݼ�ǥ���")
	{

 		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ݼ�ǥ���]"));

		if (strOmCommandArray.GetSize() < 7)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ��� �Է� �����Դϴ�";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == (DEV_SPR | DEV_JPR)) 
		{
			// �����ڵ弳��
			strTemp = ZERO2 + strOmCommandArray[1];
			strTemp = strTemp.Right(2);
			strTemp2 = ZERO2 + strOmCommandArray[4];
			strTemp2 = strTemp2.Right(2);
			m_pDevCmn->fnAPL_SetOutCheckBankNo(strTemp, strTemp2);

			strTemp.Empty();	strTemp2.Empty();

			// ��ǥ��ȣ����
			strTemp = ZERO8 + strOmCommandArray[2];
			strTemp = strTemp.Right(8);
			strTemp2 = ZERO8 + strOmCommandArray[5];
			strTemp2 = strTemp2.Right(8);
			m_pDevCmn->fnAPL_SetNumberOfOutCheckNo(strTemp, strTemp2);

			// ��ǥ�ż�����
			m_pDevCmn->fnAPL_SetNumberOfOutCheck(Asc2Int(strOmCommandArray[3]), Asc2Int(strOmCommandArray[6]));

			TranProc(TRAN_CHECK_REG);
			if (m_pDevCmn->TranResult)
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||��ǥ����� ó���Ǿ����ϴ�";
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||��ǥ����� ��ְ� �߻��߽��ϴ�";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ,���� ��� �����Դϴ�";
		}

	}
	else
	if (strOmCommandArray[0] == "OM_�غ�")
	{
		m_pTranCmn->Om_GetCurrentActionRemain();
		m_pDevCmn->OpenKey = FALSE;
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�غ�]"));
		strOmResult = OM_NORMAL;

	}
	else
	if (strOmCommandArray[0] == "OM_�")
	{

		int nCurRB2Cnt = 0;
		int nCurRB1Cnt = 0;
		int nSaveCurRB2Cnt = 0;
		int nSaveCurRB1Cnt = 0;
		int nSetCurRB2Cnt = 0;
		int nSetCurRB1Cnt = 0;
		int nSetCurTotCnt = 0;

		switch (m_pProfile->DEVICE.MachineType)
		{
			case U3100K:
				if(m_pDevCmn->FstCSTStatus == CST_SET_NG)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||1ī��Ʈ Ż��";
				}
				else
				if(m_pDevCmn->SndCSTStatus == CST_SET_NG)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||2ī��Ʈ Ż��";
				}
				else
				if(m_pDevCmn->TrdCSTStatus == CST_SET_NG)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||3ī��Ʈ Ż��";
				}
				else
				if(m_pDevCmn->RejectCSTStatus == CST_SET_NG || m_pDevCmn->RejectCSTStatus == CST_FULL) //#H0020
				{
					strOmResult = OM_ERROR;
					strOmResult += "||REJECX NG,FULL";
				}
				else
				if(m_pDevCmn->RetractCSTStatus == CST_SET_NG || m_pDevCmn->RetractCSTStatus == CST_FULL) //#H0020
				{
					strOmResult = OM_ERROR;
					strOmResult += "||RETRACT NG,FULL";
				}
				else
				if(m_pDevCmn->BrmLoc != NORMAL_POS)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||BRM Ż��";
				}
				else
				if(m_pDevCmn->DoorStatus == DOOR_OPENED)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||������";
				}
				else
				if(m_pDevCmn->CabinetStatus == DOOR_OPENED)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||��ܺι�����";
				}
				else
				if(m_pDevCmn->RejectCSTStatus == CST_SET_NG || m_pDevCmn->RetractCSTStatus == CST_SET_NG)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||ȸ����,����Ʈ�ڽ� Ż��";
				}
				else
				if (!m_pDevCmn->fnAPL_ResetDevice(DEV_CSH))
				{
					m_pDevCmn->OpenKey = TRUE;
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�-FAIL!!]"));
					strOmResult = OM_NORMAL;
					m_pDevCmn->fnAPL_ClerkInformation();									// �������
					m_pDevCmn->fnUTL_SetClerkScreen_T3_Van();										// ȭ�鱳��
					SetScreen = OFF;
					if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT4)
					{
						m_pDevCmn->AtmStatus = ATM_DOWN;
					}							
				}
				else
				{
					int nRB1NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKindUp");
					int nRB2NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteKindUp");
			//		int nRB3NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteKindUp");
					int nNoteKind1     = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKind");
					int nNoteKind2     = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteKind");
			//		int nNoteKind3     = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteKind");
	
					int nRB1NoteCnt = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt");
					int nRB2NoteCnt = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt");
					int nRB3NoteCnt = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt");
					CString strST1  = RegGetStr(_REGKEY_BRMSTATUS, "RB1Status");
					CString strST2  = RegGetStr(_REGKEY_BRMSTATUS, "RB2Status");
					CString strST3  = RegGetStr(_REGKEY_BRMSTATUS, "RB3Status");
	
					if(nRB1NoteKindUp != nNoteKind1)
					{
						strOmResult = OM_ERROR;
						strOmResult += "||1ī��Ʈ ���� ���� ����(5����)"; //#0063
					}
					else
					if(nRB2NoteKindUp != nNoteKind2)
					{
						strOmResult = OM_ERROR;
						strOmResult += "||2ī��Ʈ ���� ���� ����(1����)"; //#0063
					}
/*
					else
					if((0 < nRB1NoteCnt) && (strST1.CompareNoCase("EMPTY") == 0))
					{
						strOmResult = OM_ERROR;
						strOmResult += "||1ī��Ʈ �ݾ� ���� ����"; //#0063
					}
					else
					if((0 < nRB2NoteCnt) && (strST2.CompareNoCase("EMPTY") == 0))
					{
						strOmResult = OM_ERROR;
						strOmResult += "||2ī��Ʈ �ݾ� ���� ����"; //#0063
					}
					else
					if((0 < nRB3NoteCnt) && (strST3.CompareNoCase("EMPTY") == 0))
					{
						strOmResult = OM_ERROR;
						strOmResult += "||3ī��Ʈ �ݾ� ���� ����"; //#0063
					}
*/
					else
					{
						m_pDevCmn->OpenKey = TRUE;
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�]"));
						strOmResult = OM_NORMAL;
						m_pDevCmn->fnAPL_ClerkInformation();									// �������
						m_pDevCmn->fnUTL_SetClerkScreen_T3_Van();										// ȭ�鱳��
						SetScreen = OFF;

						//���� �ܾ�
						if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT4)
						{
							m_pDevCmn->AtmStatus = ATM_DOWN;
						}
					}
				}
				break;
			case U8100:
				m_pDevCmn->OpenKey = TRUE;
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�]"));
				m_pDevCmn->fnCMR_PreviewToOM(FALSE);
				m_pDevCmn->fnCMR_PreviewToFront(FALSE);
				strOmResult = OM_NORMAL;
				SetClerkScreen(FALSE);
				SetScreen = OFF;

				strTemp = IniGetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_T0GAP",   "FFFF");
				if (strTemp.CompareNoCase("FFFF") ==0)
				{
					m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);
				}
				else
				{
//					m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);
					m_pTranCmn->Om_SetActionGapRemainAsycSend();
				}

				if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT4)
				{
					m_pDevCmn->AtmStatus = ATM_DOWN;
				}
				break;
			default:
				break;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_��ġ����")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ġ����]"));
		if (strOmCommandArray.GetSize() < 17)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ġ���� �Է� �����Դϴ�";
		}
		else
		{
			m_pProfile->GetNetworkProfile();

			// 0:��ġ��¥(YYYYMMDD)
			strTemp = ZERO8 + strOmCommandArray[1];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.MachineMakeDate, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.MachineMakeDate[8] = NULL;

			// 1:�Ҽ�����1(NNNNNNNN)
			strTemp = ZERO8 + strOmCommandArray[2];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.BranchNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.BranchNum[8] = NULL;
			
			// 2:���(NNNNNNNN)
			strTemp = ZERO8 + strOmCommandArray[3];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.AtmSerialNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.AtmSerialNum[8] = NULL;
			
			// 3:�Ҽ�����2(NNNNNNNN)
			strTemp = ZERO8 + strOmCommandArray[4];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.SubBranchNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.SubBranchNum[8] = NULL;
			
			// 4:����(ON,OFF)
			if (!strOmCommandArray[5].CompareNoCase("ON"))
				m_pProfile->NETWORK.BranchInOut = TRUE;
			else
				m_pProfile->NETWORK.BranchInOut = FALSE;

			// 5:ó������(NNNNNNNN)
			strTemp = ZERO8 + strOmCommandArray[6];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.RoomNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.RoomNum[8] = NULL;
			
			// 6:��������(ON,OFF)
			if (!strOmCommandArray[7].CompareNoCase("ON"))
				m_pProfile->NETWORK.AutoBranchOnOff = TRUE;
			else
				m_pProfile->NETWORK.AutoBranchOnOff = FALSE;
			
			// 7:24�ð��(ON,OFF)
			if (!strOmCommandArray[8].CompareNoCase("ON"))
				m_pProfile->NETWORK.All24OnOff = TRUE;
			else
				m_pProfile->NETWORK.All24OnOff = FALSE;
			
			// 8:365�(ON,OFF)
			if (!strOmCommandArray[9].CompareNoCase("ON"))
				m_pProfile->NETWORK.All365OnOff = TRUE;
			else
				m_pProfile->NETWORK.All365OnOff = FALSE;
			
			// 9:CE�繫���ڵ�(NNNN)
			strTemp = strOmCommandArray[10] + SPACE4;
			strTemp = strTemp.Left(4);
			memcpy(m_pProfile->NETWORK.CeOfficeCode, strTemp.GetBuffer(0), 4);
			m_pProfile->NETWORK.CeOfficeCode[4] = NULL;
			
			// 10:��������ȣ(NNNNNNNNNNNNNNNN)
			strTemp = strOmCommandArray[11] + SPACE12;
			strTemp = strTemp.Left(16);
			memcpy(m_pProfile->NETWORK.MachineMakeNum, strTemp.GetBuffer(0), 16);
			m_pProfile->NETWORK.MachineMakeNum[16] = NULL;
			
			// 11:�����(NNNNNNNN)
			strTemp = ZERO8 + strOmCommandArray[12];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.OutBranchNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.OutBranchNum[8] = NULL;
			
			// 12:��ġ��(NNNNNNNNNN)
			strTemp = strOmCommandArray[13] + SPACE10;
			strTemp = strTemp.Left(10);
			memcpy(m_pProfile->NETWORK.InstName, strTemp.GetBuffer(0), 10);
			m_pProfile->NETWORK.InstName[10] = NULL;
			
			// 13:��ġȮ����(NNNNNNNNNN)
			strTemp = strOmCommandArray[14] + SPACE10;
			strTemp = strTemp.Left(10);
			memcpy(m_pProfile->NETWORK.InstConfirmName, strTemp.GetBuffer(0), 10);
			m_pProfile->NETWORK.InstConfirmName[10] = NULL;
			
			// 14:���������(NNNNNNNNNN)
			strTemp = strOmCommandArray[15] + SPACE10;
			strTemp = strTemp.Left(10);
			memcpy(m_pProfile->NETWORK.CeName, strTemp.GetBuffer(0), 10);
			m_pProfile->NETWORK.CeName[10] = NULL;
			
			// 15:��������ڿ���ó(NNNNNNNNNNNNNNNN)
			strTemp = strOmCommandArray[16] + SPACE16;
			strTemp = strTemp.Left(16);
			memcpy(m_pProfile->NETWORK.CeTel, strTemp.GetBuffer(0), 16);
			m_pProfile->NETWORK.CeTel[16] = NULL;

			m_pProfile->PutNetworkProfile();
			m_pDevCmn->fnNET_Initialize(LINE_OPCL_TIME);					// Ip setting initialize
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_�ΰ���ġ����")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ΰ���ġ����]"));
		if (strOmCommandArray.GetSize() < 7)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�ΰ���ġ���� �Է� �����Դϴ�";
		}
		else
		{

			m_pProfile->GetDeviceProfile();

			// 0:IC�ŷ�(����,�Ұ�)
			if (!strOmCommandArray[1].CompareNoCase("����"))
				m_pProfile->DEVICE.ICCardDevice = ICCARD_SET;
			else
				m_pProfile->DEVICE.ICCardDevice = NOT_SET;

			// 1:�ؿܰŷ�(����,�Ұ�)
			if (!strOmCommandArray[2].CompareNoCase("����"))
				m_pProfile->DEVICE.DesHandler = T_DES_SET;
			else
				m_pProfile->DEVICE.DesHandler = NOT_SET;
			
			// 2:�����ŷ�(����,�Ұ�)
			if (!strOmCommandArray[3].CompareNoCase("����"))
				m_pProfile->DEVICE.FingerHandler = FINGER_SET;
			else
				m_pProfile->DEVICE.FingerHandler = NOT_SET;

			// 3:���ܼ��ŷ�(����,�Ұ�)
			if (!strOmCommandArray[4].CompareNoCase("����"))
				m_pProfile->DEVICE.IrdaHandler = IRDA_SET;
			else
				m_pProfile->DEVICE.IrdaHandler = NOT_SET;
			
			// 4:���ڵ�ŷ�(����,�Ұ�)
			if (!strOmCommandArray[5].CompareNoCase("����"))
				m_pProfile->DEVICE.BarHandler = BAR_SET;
			else
				m_pProfile->DEVICE.BarHandler = NOT_SET;

			// 5:��ȣȭ�ŷ�(����,�Ұ�)
			if (!strOmCommandArray[6].CompareNoCase("����"))
				m_pProfile->DEVICE.SeedHandler = SEED_SET;
			else
				m_pProfile->DEVICE.SeedHandler = NOT_SET;

			m_pProfile->PutDeviceProfile();
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_LAN��������_CD")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LAN��������]"));
		if (strOmCommandArray.GetSize() < 4)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||LAN�������� �Է� �����Դϴ�";
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LAN %s %s %s]", strOmCommandArray[1], strOmCommandArray[2], strOmCommandArray[3]));
			m_pProfile->GetNetworkProfile();					// Get Network Profile

			strTemp.Empty();
			strTemp = ZERO12 + strOmCommandArray[1];
			strTemp = strTemp.Right(12);
			strTemp.Format("%3.3s.%3.3s.%3.3s.%3.3s", 
							strTemp.Mid(0,3), 
							strTemp.Mid(3,3), 
							strTemp.Mid(6,3),
							strTemp.Mid(9,3));
			memcpy(m_pProfile->NETWORK.BpIpAddress, strTemp.GetBuffer(0), 15);
			m_pProfile->NETWORK.BpIpAddress[15] = NULL;

			strTemp.Empty();
			strTemp = ZERO5 + strOmCommandArray[2];
			strTemp = strTemp.Right(5);
			memcpy(m_pProfile->NETWORK.BpPortNo, strTemp.GetBuffer(0), 5);
			m_pProfile->NETWORK.BpPortNo[5] = NULL;

			strTemp.Empty();
			strTemp = ZERO14 + strOmCommandArray[3];
			strTemp = strTemp.Right(14);

			memcpy(m_pProfile->NETWORK.BranchNum, strTemp.GetBuffer(0), 6);
			m_pProfile->NETWORK.BranchNum[6] = '0';
			m_pProfile->NETWORK.BranchNum[8] = '0';
			m_pProfile->NETWORK.BranchNum[8] = NULL;

			memcpy(m_pProfile->NETWORK.AtmSerialNum, &strTemp.GetBuffer(0)[6], 8);

			strTemp2.Format("%6.6s", &m_pProfile->NETWORK.AtmSerialNum[2]);

			IniSetStr(_CH_SSLVPN_INI, "ATM", "VPN_IP",  "211.55.3.31");
			IniSetStr(_CH_SSLVPN_INI, "ATM", "VPN_ID",  strTemp2);


			m_pProfile->NETWORK.AtmSerialNum[8] = NULL;
// 			DWORD dwAtmPos = Asc2Int(strTemp.Left(5));
// 
// 			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OM_LAN��������_CD: dwAtmPos %d", dwAtmPos);
// 			switch (dwAtmPos)	
// 			{
// 				case 0:
// 				case 1:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "010000", 6);
// 					break;
// 				case 2:
// 				case 3:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "013000", 6);
// 					break;
// 				case 4:
// 				case 5:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "020000", 6);
// 					break;
// 				case 6:
// 				case 7:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "023000", 6);
// 					break;
// 				case 8:
// 				case 9:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "030000", 6);
// 					break;
// 				default:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "010000", 6);
// 					break;
// 
// 			}
			
			m_pProfile->PutTransProfile();



// 			char	szStartRebootTime[256];
// 			char	szEndRebootTime[256];
// 			memset(szStartRebootTime, 0x00, sizeof(szStartRebootTime));
// 			memset(szEndRebootTime,   0x00, sizeof(szEndRebootTime));
// 			memcpy(szStartRebootTime,  m_pProfile->TRANS.RebootHHMMSS, 6);
// 			memcpy(szEndRebootTime,    m_pProfile->TRANS.RebootHHMMSS, 6);
// 			AddString(szEndRebootTime, "010000", 6);

// 			if(memcmp(m_pProfile->TRANS.RebootHHMMSS, ZERO6, 6) != 0)
// 			{
// 				RegSetInt(_REGKEY_ATMRESTART, "UseRestart"    , 1);
// 				RegSetStr(_REGKEY_ATMRESTART, "CheckStartTime", szStartRebootTime);
// 				RegSetStr(_REGKEY_ATMRESTART, "CheckEndTime"  , szEndRebootTime);
// 				RegSetInt(_REGKEY_ATMRESTART, "ReadyRestart"  , 0);
// 			}
// 
// 			m_pDevCmn->AtmRestartInfo.bUseRestart		= RegGetInt(_REGKEY_ATMRESTART, "UseRestart", 1);
// 			m_pDevCmn->AtmRestartInfo.strCheckStartTime= RegGetStr(_REGKEY_ATMRESTART, "CheckStartTime", "040000");
// 			m_pDevCmn->AtmRestartInfo.strCheckEndTime	= RegGetStr(_REGKEY_ATMRESTART, "CheckEndTime", "050000");
// 			m_pDevCmn->AtmRestartInfo.bReadyRestart	= RegGetInt(_REGKEY_ATMRESTART, "ReadyRestart", 0);
// 			
// 			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OM_LAN��������_CD: CheckStartTime[%s] ~ CheckEndTime[%s]", szStartRebootTime, szEndRebootTime);
// 	
			
			m_pProfile->PutNetworkProfile();
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT4;
//			m_pDevCmn->AtmStatus = ATM_DOWN;
			strOmResult = OM_NORMAL;
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LAN %s]", strOmResult));
		}

	}
	else
	if (strOmCommandArray[0] == "OM_LAN��������")
	{
		if (strOmCommandArray.GetSize() < 8)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||LAN�������� �Է� �����Դϴ�";
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LAN��������]"));

			m_pProfile->GetNetworkProfile();

			// 0:IP�ּ�(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[1], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				memcpy(m_pProfile->NETWORK.AtmIPAddress, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmIPAddress[15] = NULL;
			}
			else
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", 0, 0, 0, 0);
				memcpy(m_pProfile->NETWORK.AtmIPAddress, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmIPAddress[15] = NULL;
			}

			// 1:����ݸ���ũ(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[2], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				memcpy(m_pProfile->NETWORK.AtmSubnetMask, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmSubnetMask[15] = NULL;
			}
			else
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", 0, 0, 0, 0);
				memcpy(m_pProfile->NETWORK.AtmSubnetMask, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmSubnetMask[15] = NULL;
			}
			
			// 2:����Ʈ����(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[3], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				memcpy(m_pProfile->NETWORK.AtmGateWay, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmGateWay[15] = NULL;
			}
			else
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", 0, 0, 0, 0);
				memcpy(m_pProfile->NETWORK.AtmGateWay, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmGateWay[15] = NULL;
			}

			// 3:BP1IP�ּ�(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[4], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				memcpy(m_pProfile->NETWORK.BpIpAddress, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.BpIpAddress[15] = NULL;
			}
			else
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", 0, 0, 0, 0);
				memcpy(m_pProfile->NETWORK.BpIpAddress, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.BpIpAddress[15] = NULL;
			}

			// 4:BP1PORT(NNNNN)
			strTemp.Format("%5.5d", Asc2Int(strOmCommandArray[5]));
			memcpy(m_pProfile->NETWORK.BpPortNo, strTemp.GetBuffer(0), 5);
			m_pProfile->NETWORK.BpPortNo[5] = NULL;
			
			// 5:BP2IP�ּ�(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[6], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				memcpy(m_pProfile->NETWORK.BpIpAddress2, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.BpIpAddress2[15] = NULL;
			}
			else
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", 0, 0, 0, 0);
				memcpy(m_pProfile->NETWORK.BpIpAddress2, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.BpIpAddress2[15] = NULL;
			}

			// 6:BP2PORT(NNNNN)
			strTemp.Format("%5.5d", Asc2Int(strOmCommandArray[7]));
			memcpy(m_pProfile->NETWORK.BpPortNo2, strTemp.GetBuffer(0), 5);
			m_pProfile->NETWORK.BpPortNo2[5] = NULL;
			
			m_pProfile->PutNetworkProfile();
			// IP Address ���� Library ����		V01.03.01 #2
			m_pDevCmn->fnAPL_NetworkInfoSet();
//			m_pDevCmn->AtmInfoINI();						// atmInfo setting for BMGuard
			strOmResult = OM_NORMAL;
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LAN�������� %s]", strOmResult));
		}
	}
	else
	if (strOmCommandArray[0] == "OM_ȸ������")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[ȸ������]"));
		if (strOmCommandArray.GetSize() < 13)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||ȸ������ �Է� �����Դϴ�";
		}
		else
		{
			m_pProfile->GetNetworkProfile();

			// 0:�������(IOC,HDLC,SWP,SDLC,TCP/IP,CCL,BAS)
			if (strOmCommandArray[1] == "IOC")
				m_pProfile->NETWORK.Interface = NET_IOC;
			else
			if (strOmCommandArray[1] == "HDLC")
				m_pProfile->NETWORK.Interface = NET_HDLC;
			else
			if (strOmCommandArray[1] == "SWP")
				m_pProfile->NETWORK.Interface = NET_SWP;
			else
			if (strOmCommandArray[1] == "SDLC")
				m_pProfile->NETWORK.Interface = NET_SDLC;
			else
			if (strOmCommandArray[1] == "TCP/IP")
				m_pProfile->NETWORK.Interface = NET_TCPIP;
			else
			if (strOmCommandArray[1] == "CCL")
				m_pProfile->NETWORK.Interface = NET_CCL;
			else
				m_pProfile->NETWORK.Interface = NET_BAS;

			// 1:DA(XX)
			strTemp = ZERO2 + strOmCommandArray[2];
			strTemp = strTemp.Right(2);
			Asc2Byte(strTemp.GetBuffer(0), &m_pProfile->NETWORK.DeviceAddr);
			
			// 2:SA(XX)
			strTemp = ZERO2 + strOmCommandArray[3];
			strTemp = strTemp.Right(2);
			Asc2Byte(strTemp.GetBuffer(0), &m_pProfile->NETWORK.SelectAddr);
			
			// 3:�ۼ��Ż�����
			strTemp = ZERO5 + strOmCommandArray[4];
			strTemp = strTemp.Right(5);
			m_pProfile->NETWORK.SendRecvSize = Asc2Int(strTemp);
			
			// 4:ȸ���ӵ�(1200,2400,4800,9600,19200)
			nTemp = Asc2Int(strOmCommandArray[5]);
			if (nTemp == 1200)
				m_pProfile->NETWORK.LineSpeed = L1200;
			else
			if (nTemp == 2400)
				m_pProfile->NETWORK.LineSpeed = L2400;
			else
			if (nTemp == 4800)
				m_pProfile->NETWORK.LineSpeed = L4800;
			else
			if (nTemp == 9600)
				m_pProfile->NETWORK.LineSpeed = L9600;
			else
				m_pProfile->NETWORK.LineSpeed = L19200;
			
			// 5:POLLCHECK(ON,OFF)
			if (!strOmCommandArray[6].CompareNoCase("ON"))
				m_pProfile->NETWORK.PollCheck = POLL_ON;
			else
				m_pProfile->NETWORK.PollCheck = POLL_OFF;
			
			// 6:POINT(PTP,MTP)
			if (!strOmCommandArray[7].CompareNoCase("PTP"))
				m_pProfile->NETWORK.Point = PTP;
			else
				m_pProfile->NETWORK.Point = MTP;
			
			// 7:DUPLEX(HALF,FULL)
			if (!strOmCommandArray[8].CompareNoCase("HALF"))
				m_pProfile->NETWORK.Duplex = HALF_DUPLEX;
			else
				m_pProfile->NETWORK.Duplex = FULL_DUPLEX;
			
			// 8:TIMEFILLER(MTF,FTF)
			if (!strOmCommandArray[9].CompareNoCase("MTF"))
				m_pProfile->NETWORK.TimeFiller = MTF;
			else
				m_pProfile->NETWORK.TimeFiller = FTF;
			
			// 9:RSTYPE(RS-422,RS-232C)
			if (!strOmCommandArray[10].CompareNoCase("RS-422"))
				m_pProfile->NETWORK.RsType = RS422;
			else
				m_pProfile->NETWORK.RsType = RS232C;
			
			// 10:NRZ(NRZ,NRZI)
			if (!strOmCommandArray[11].CompareNoCase("NRZ"))
				m_pProfile->NETWORK.Nrz = NRZ;
			else
				m_pProfile->NETWORK.Nrz = NRZI;
			
			// 11:CLOCK(INCLK,EXCLK)
			if (!strOmCommandArray[12].CompareNoCase("INCLK"))
				m_pProfile->NETWORK.Clk = INCLK;
			else
				m_pProfile->NETWORK.Clk = EXCLK;

			m_pProfile->PutNetworkProfile();
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_ATMS��������")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[ATMS��������]"));
		if (strOmCommandArray.GetSize() < 6)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||ATMS�������� �Է� �����Դϴ�";
		}
		else
		{
			// 1:SERVER IP�ּ�(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[1], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%d.%d.%d.%d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP", strTemp);
				RegSetStr(_REGKEY_REMOTECTRL, "TCP_IP",   strTemp);
			}
			else
			{
				strTemp.Format("%d.%d.%d.%d", 0, 0, 0, 0);
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP", strTemp);
				RegSetStr(_REGKEY_REMOTECTRL, "TCP_IP",   strTemp);
			}

			// 2:SERVER PORT(NNNNN)
			strTemp.Format("%d", Asc2Int(strOmCommandArray[2]));
			IniSetStr(_ATMS_FTP_INI, "TCP_IP", "Port", strTemp);
			RegGetInt(_REGKEY_REMOTECTRL, "TCP_PORT", Asc2Int(strOmCommandArray[2]));
			
			// 3:����SERVER1 IP�ּ�(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[3], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%d.%d.%d.%d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTONE", strTemp);
			}
			else
			{
				strTemp.Format("%d.%d.%d.%d", 0, 0, 0, 0);
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTONE", strTemp);
			}

			// 4:����SERVER2 IP�ּ�(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[4], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%d.%d.%d.%d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTWO", strTemp);
			}
			else
			{
				strTemp.Format("%d.%d.%d.%d", 0, 0, 0, 0);
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTWO", strTemp);
			}

			// 5:����SERVER3 IP�ּ�(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[5], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%d.%d.%d.%d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTHREE", strTemp);
			}
			else
			{
				strTemp.Format("%d.%d.%d.%d", 0, 0, 0, 0);
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTHREE", strTemp);
			}
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_�ŷ���������")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ���������]"));
		if (strOmCommandArray.GetSize() < 6)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�ŷ��������� �Է� �����Դϴ�";
		}
		else
		{
			m_pProfile->GetTransProfile();

			// 0:�������(����,�Ұ�)
			if (strOmCommandArray[1] == "����")
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_WITH_CASH;
			else
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_WITH_CASH;

			// 1:�����Ա�(����,�Ұ�)
			if (strOmCommandArray[2] == "����")
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_DEP_CASH;
			else
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_DEP_CASH;
			
			// 2:��ǥ���(����,�Ұ�)
			if (strOmCommandArray[3] == "����")
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_WITH_CHECK;
			else
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_WITH_CHECK;
			
			// 3:��ǥ�Ա�(����,�Ұ�)
			if (strOmCommandArray[4] == "����")
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_DEP_CHECK;
			else
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_DEP_CHECK;
			
			// 4:����ŷ�(����,�Ұ�)
			if (strOmCommandArray[5] == "����")
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_PBM_OK;
			else
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_PBM_OK;

			// 5:5������(����,�Ұ�)
			if ((m_pDevCmn->ChangeBCFlag == FALSE)	||
				(strOmCommandArray[6] == "�Ұ�"))				
			{
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_WITH_50TCASH;
//				m_pDevCmn->Withdraw50TFlag = FALSE;			
			}
			else
			{
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_WITH_50TCASH;
				m_pDevCmn->Withdraw50TFlag = TRUE;			
			}

			m_pProfile->PutDeviceProfile();
			m_pProfile->PutTransProfile();
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_�հ�")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�հ�]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR | DEV_JPR)) 
		{
			m_pDevCmn->fnAPL_SetSensorInfo();							// ������������
			// ���۽� BRM����ġ ���� 
			if (m_pDevCmn->RejectCSTLoc != NORMAL_POS)
			{
				strOmResult = OM_ERROR;
				strOmResult += "|| [ REJECT BOX ��ġ���� ]";
			}
			else
			if (m_pDevCmn->RetractCSTLoc != NORMAL_POS)
			{
				strOmResult = OM_ERROR;
				strOmResult += "|| [ RETRACT BOX ����ġ ����]";
			}
			else
			if (m_pDevCmn->BrmLoc != NORMAL_POS)
			{
				strOmResult = OM_ERROR;
				strOmResult += "|| [ BRM ����ġ ���� ]";
			}
			else
			if(m_pDevCmn->FstCSTStatus == CST_SET_NG)
			{
				strOmResult = OM_ERROR;
				strOmResult += "||1ī��Ʈ Ż��";
			}
			else
			if(m_pDevCmn->SndCSTStatus == CST_SET_NG)
			{
				strOmResult = OM_ERROR;
				strOmResult += "||2ī��Ʈ Ż��";
			}
			else
			if(m_pDevCmn->TrdCSTStatus == CST_SET_NG)
			{
				strOmResult = OM_ERROR;
				strOmResult += "||3ī��Ʈ Ż��";
			}
			else
			{
				if(m_pProfile->DEVICE.MachineType == U3100K)
				{
				
					m_pDevCmn->FstCSTSetCash = 10000;
					m_pDevCmn->SndCSTSetCash = 10000;
					m_pDevCmn->TrdCSTSetCash = 10000;
					int nNoteKind1 = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKind", 0);

					if (NOTE_KIND_50000_USE == nNoteKind1)
						IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K", m_pDevCmn->FstCSTSetCash);
					else
						IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K", m_pDevCmn->FstCSTSetCash);										
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);

					//init cst2
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
					//init cst3
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K", m_pDevCmn->TrdCSTSetCash);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
					//init cst4
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

					
					strTemp.Empty();
					strTemp.Format("%8.8s%6.6s", GetDate(), GetTime());
					m_pDevCmn->fnUTL_IniSetInt(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedCstInfo", TRUE);
					m_pDevCmn->fnUTL_IniSetStr(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedDateTime", strTemp);
	//				m_pDevCmn->fnCSH_Initialize();						
				}
				else
				if(m_pProfile->DEVICE.MachineType == U8100)
				{
					strTemp.Empty();
					strTemp.Format("%8.8s%6.6s", GetDate(), GetTime());
					m_pDevCmn->fnUTL_IniSetStr(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedDateTime", strTemp);
					
				}

				TranProc(TRAN_TOTAL);								// �հ�ó��
				if (m_pDevCmn->TranResult)							// HOST OK
				{
					strOmResult = OM_NORMAL;
					strOmResult += "||�հ� �Ϸ�";
				}
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||�հ�ó�� ����";
				}
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ,���κ� ��� �����Դϴ�";
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�հ� %s]", strOmResult));
	}
	else
	if (strOmCommandArray[0] == "OM_�Ұ�")
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�Ұ�]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR | DEV_JPR)) 
		{
			TranProc(TRAN_SUBTOTAL);							// �Ұ�ó��
			strOmResult = OM_NORMAL;
			strOmResult += "||�Ұ� �Ϸ�";
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��� �����Դϴ�";
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�Ұ� %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_BRM_�ݾ׼���")
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[BRM_�ݾ׼���]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_JPR) == m_pDevCmn->fnAPL_GetDefineDevice(DEV_JPR)) 
		{
			if (strOmCommandArray.GetSize() >= 5)
			{
				//C-ATM Set Amt
				if (Asc2Int(strOmCommandArray[1]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[2]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[3]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[4]) > 10000)
					nErrorFlag = TRUE;
			}
			else
			if (strOmCommandArray.GetSize() >= 4)
			{
				//C-ATM Set Amt
				if (Asc2Int(strOmCommandArray[1]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[2]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[3]) > 10000)
					nErrorFlag = TRUE;
			}
			else
			if(strOmCommandArray.GetSize() >= 3)
			{
				if (Asc2Int(strOmCommandArray[1]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[2]) > 10000)
					nErrorFlag = TRUE;

			}

			if (nErrorFlag)
			{
				strOmResult = OM_ERROR;
				strOmResult += "||�Է¸ż��� ī��Ʈ �ִ� ���� �ż����� ŭ";
			}
			else
			{
				m_pDevCmn->FstCSTSetCash = 0;
				m_pDevCmn->SndCSTSetCash = 0;
				m_pDevCmn->TrdCSTSetCash = 0;
				m_pDevCmn->FthCSTSetCash = 0;

				if(m_pProfile->DEVICE.MachineType == U3100K)
				{
					m_pDevCmn->FstCSTSetCash = Asc2Int(strOmCommandArray[1]);
					m_pDevCmn->SndCSTSetCash = Asc2Int(strOmCommandArray[2]);
					m_pDevCmn->TrdCSTSetCash = Asc2Int(strOmCommandArray[3]);

					int nNoteKind1 = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKind", 0);

					if (NOTE_KIND_50000_USE == nNoteKind1)
						IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K", m_pDevCmn->FstCSTSetCash);
					else
						IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K", m_pDevCmn->FstCSTSetCash);										
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);

					//init cst2
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
					//init cst3
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K", m_pDevCmn->TrdCSTSetCash);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
					//init cst4
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

					
					strTemp.Empty();
					strTemp.Format("%8.8s%6.6s", GetDate(), GetTime());
					m_pDevCmn->fnUTL_IniSetInt(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedCstInfo", TRUE);
					m_pDevCmn->fnUTL_IniSetStr(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedDateTime", strTemp);

					TranProc(TRAN_CASH_REG); // ���ݵ��ó��
					if (m_pDevCmn->TranResult)
					{
					
						strOmResult = OM_NORMAL;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->FstCSTSetCash,8,10,'0') ;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->SndCSTSetCash,8,10,'0') ;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->TrdCSTSetCash,8,10,'0') ;
						strOmResult += "||0||0||���ݵ�� �Ϸ�";
					}
					else
					{
						strOmResult = OM_ERROR;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->FstCSTSetCash,8,10,'0') ;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->SndCSTSetCash,8,10,'0') ;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->TrdCSTSetCash,8,10,'0') ;
						strOmResult += "||0||���ݵ���� �Ϸ�Ǿ����ϴ�, ������� ����,[��ġ���¿�û]";
					}
						
				}			

			}

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ,���κ� ��� �����Դϴ�";
		}

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[BRM_�ݾ׼��� %s]", strOmResult));
	}
	else
	if (strOmCommandArray[0] == "OM_������ȸ")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ȸ]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR | DEV_JPR)) 
		{
			TranProc(TRAN_SUBTOTAL);							// ������ȸ
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||������ȸ�� ó���Ǿ����ϴ�";
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||������ȸó���� ���� �Ͽ����ϴ�";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ,���κ� ��� �����Դϴ�";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_����")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[����]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR | DEV_JPR)) 
		{
			TranProc(TRAN_TOTAL);								// �հ�ó��
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||������ ó���Ǿ����ϴ�";
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||����ó���� ���� �Ͽ����ϴ�";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ,���κ� ��� �����Դϴ�";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_��ü�ʱ�ȭ")
	{
		m_pDevCmn->RecoveryType = 1;							// 0:�ڵ�����,1:��������
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ü�ʱ�ȭ]"));
		m_pDevCmn->fnAPL_CloseLine();							// ȣ��Ʈ������
		m_pDevCmn->fnAPL_ClearError();							// ��������ʱ�ȭ
		m_pDevCmn->DeviceAutoOff = 0;							// �ڵ����� �� Device = Set
		m_pDevCmn->DeviceLoss = DEV_NONE;						// �н������ʱ�ȭ
		m_pDevCmn->ForceDownFlag = FALSE;						// ����Down���� 
		m_pDevCmn->RemoteDownFlag = FALSE;						// ����Down���� 
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);
		if (m_pDevCmn->fnAPL_ResetDevice(DEV_MAIN))
			strOmResult = OM_NORMAL;
		else
			strOmResult = OM_ERROR;
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ü�ʱ�ȭ %s]", strOmResult));
		m_pDevCmn->fnSPACK_Deinitialize();
		m_pDevCmn->fnSPACK_Initialize();

		m_pDevCmn->RecoveryType = 0;							// 0:�ڵ�����,1:��������
	}
	else
	if (strOmCommandArray[0] == "OM_BRM�ʱ�ȭ")
	{
		m_pDevCmn->RecoveryType = 1;							// 0:�ڵ�����,1:��������
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[BRM�ʱ�ȭ]"));
		m_pDevCmn->fnAPL_ClearError();							// ��������ʱ�ȭ
		m_pDevCmn->DeviceAutoOff = 0;							// �ڵ����� �� Device = Set
		m_pDevCmn->DeviceLoss = DEV_NONE;						// �н������ʱ�ȭ
		m_pDevCmn->ForceDownFlag = FALSE;						// ����Down���� 2006-12-19-#01
		m_pDevCmn->RemoteDownFlag = FALSE;						// ����Down���� 2006-12-19-#01

		if (m_pDevCmn->fnAPL_ResetDevice(DEV_BRM))
			strOmResult = OM_NORMAL;
		else
			strOmResult = OM_ERROR;
		m_pDevCmn->RecoveryType = 0;							// 0:�ڵ�����,1:��������
	}
	else
	if (strOmCommandArray[0] == "OM_��ü����")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ü����]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{

//			m_pTranCmn->Om_GetCurrentActionRemain();

			m_pDevCmn->fnCSH_RefillMoney(0xffff);				// ����(���ݸż�)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// ���۴�����ְ���

		
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// ����Segment���
			SplitString(m_pDevCmn->fstrCSH_GetRefillResult(), ",", strTempArray);

			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	//
			{
				if(0 < Asc2Int(strTempArray[8]))				// �Ҹ�� �ȳ����� ���� 
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | REFILL_MOVE,
									Asc2Int(strTempArray[1]),	// ����ż�
									Asc2Int(strTempArray[3]),	// ��������ż�
									Asc2Int(strTempArray[4]),	// ��õ������ż�
									Asc2Int(strTempArray[5]),	// õ������ż�
									Asc2Int(strTempArray[7])	// ��ǥ����ż�
								+	Asc2Int(strTempArray[8]),	// �Ҹ�Ǻ���ż�
									Asc2Int(strTempArray[6]),	// ����������ż�	
									Asc2Int(strTempArray[9]),	// ������   ������ż�
									Asc2Int(strTempArray[10]));	// �������� ������ż�

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	

				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1����
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);
				
				m_pDevCmn->SCSTCntF = 2;

			}
			else
			{													
				if(0 < Asc2Int(strTempArray[7]))				//  �Ҹ�� �ȳ����� ���� 
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | REFILL_MOVE,
									Asc2Int(strTempArray[1]),	// ����ż�
									Asc2Int(strTempArray[3]),	// ��������ż�
									Asc2Int(strTempArray[4]),	// ��õ������ż�
									Asc2Int(strTempArray[5]),	// õ������ż�
									Asc2Int(strTempArray[6])	// ��ǥ����ż�
								+	Asc2Int(strTempArray[7]));	// �Ҹ�Ǻ���ż�

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));
			}


			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{

// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��� �����Դϴ�";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_��üȸ��")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��üȸ��]"));

		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{

//			m_pTranCmn->Om_GetCurrentActionRemain();

			RegSetInt(_REGKEY_BRMINFO, "ȸ������", 0x00);		// 0x00 : ��� ����
			m_pDevCmn->fnCSH_TakeupMoney(0xffff);				// ȸ��(���ݸż�)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// ���۴�����ְ���
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// ����Segment���
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// ȸ��������ϱ�
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	
			{
				if(0 < Asc2Int(strTempArray[8]))					// �Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[7])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[8]),	// �Ҹ��ȸ���ż�
									Asc2Int(strTempArray[6]),	// ������ȸ���ż�		
									Asc2Int(strTempArray[9]),	// ������ ����ȸ���ż�	
									Asc2Int(strTempArray[10]));	// �������� ����ȸ���ż�
				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	
				//#0042
				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1����
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135	
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);
				
				m_pDevCmn->SCSTCntF = 2;
			}
			else
			{													
				if(0 < Asc2Int(strTempArray[7]))				// �Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[6])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[7]));	// �Ҹ��ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));
			}

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else										
					strOmResult += "||" + strTemp + "||�Ұ�";

			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��� �����Դϴ�";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_�Ϻκ���")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OM_�Ϻκ���]"));
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Ϻκ��� �Է� �����Դϴ�";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{
			if (((m_pProfile->DEVICE.MachineType == U8100))	&&
				 (strOmCommandArray.GetSize() > 2))
			{
				// ������ �Ϻ� ���� 
				if (strOmCommandArray[2] == "����")
					RegSetInt(_REGKEY_BRMINFO, "�������", BRM_NOTEKIND_10000_OLD);	// 0x30 ������
				else if (strOmCommandArray[2] == "��õ")
					RegSetInt(_REGKEY_BRMINFO, "�������", BRM_NOTEKIND_1000_OLD);	// 0x32 ��õ��
				else if (strOmCommandArray[2] == "�Ÿ�")
					RegSetInt(_REGKEY_BRMINFO, "�������", BRM_NOTEKIND_10000_NEW);	// 0x35 �Ÿ���
				else if (strOmCommandArray[2] == "��õ")
					RegSetInt(_REGKEY_BRMINFO, "�������", BRM_NOTEKIND_1000_NEW);	// 0x37 ��õ��
				else if (strOmCommandArray[2] == "����")							
					RegSetInt(_REGKEY_BRMINFO, "�������", BRM_NOTEKIND_50000);		
				else
					RegSetInt(_REGKEY_BRMINFO, "�������", 0x00);	// 0x00 ALL
			}

//			m_pTranCmn->Om_GetCurrentActionRemain();

			nTemp = Asc2Int(strOmCommandArray[1]);
			m_pDevCmn->fnCSH_RefillMoney(nTemp);				// ����(���ݸż�)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// ���۴�����ְ���
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// ����Segment���
			SplitString(m_pDevCmn->fstrCSH_GetRefillResult(), ",", strTempArray);
																// ���������ϱ�
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	// 
			{
				if(0 < Asc2Int(strTempArray[8]))				// �Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | REFILL_MOVE,
									Asc2Int(strTempArray[1]),	// ����ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[7])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[8]),	// �Ҹ��ȸ���ż�
									Asc2Int(strTempArray[6]),	// ������ȸ���ż�			
									Asc2Int(strTempArray[9]),	// ������ ������ż�	
									Asc2Int(strTempArray[10]));	// �������� ������ż�
				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d",
									Asc2Int(strTempArray[1]),
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	

				//#0042
				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1����
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);
				
				m_pDevCmn->SCSTCntF = 2;
			}
			else
			{												
				if(0 < Asc2Int(strTempArray[7]))				// �Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | REFILL_MOVE,
									Asc2Int(strTempArray[1]),		// ����ż�
									Asc2Int(strTempArray[3]),		// ����ȸ���ż�
									Asc2Int(strTempArray[4]),		// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),		// õ��ȸ���ż�
									Asc2Int(strTempArray[6])		// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[7]));		// �Ҹ��ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));
			}
			
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��� �����Դϴ�";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_�Ϻ�ȸ��")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�Ϻ�ȸ��]",
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4])), JNL_OPER);
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Ϻ�ȸ�� �Է� �����Դϴ�";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{
//			m_pTranCmn->Om_GetCurrentActionRemain();

			nTemp = Asc2Int(strOmCommandArray[1]);
			m_pDevCmn->fnCSH_TakeupMoney(nTemp);				// ȸ��(���ݸż�)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// ���۴�����ְ���
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// ����Segment���
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// ȸ��������ϱ�
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	
			{
				if(0 < Asc2Int(strTempArray[8]))				//�Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[7])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[8]),	// �Ҹ��ȸ���ż�
									Asc2Int(strTempArray[6]),	// ������ȸ���ż�		
									Asc2Int(strTempArray[9]),	// ������ ����ȸ���ż�	
									Asc2Int(strTempArray[10]));	// �������� ����ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	

				//#0042
				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1����
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

				m_pDevCmn->SCSTCntF = 2;

			}
			else
			{
				if(0 < Asc2Int(strTempArray[7]))				// �Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,	
									Asc2Int(strTempArray[1]),	// ����ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[6])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[7]));	// �Ҹ��ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));
			}

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)				
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��� �����Դϴ�";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_����ݸ�弱��")	
	{
		if (0 == strOmCommandArray[1].CompareNoCase("����"))	// ���ϸ��=����
		{
			m_pDevCmn->fnBRM_SetCarrySpeed(FALSE);
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				SetShareData("OM_����ݸ�弱��", "����");
				strOmResult = OM_NORMAL;
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||���ݺ� ��ְ� �߻��Ͽ����ϴ�";
			}
		}
		else													// ���ϸ��=���
		{
			m_pDevCmn->fnBRM_SetCarrySpeed(TRUE);
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				SetShareData("OM_����ݸ�弱��", "����");
				strOmResult = OM_NORMAL;
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||���ݺ� ��ְ� �߻��Ͽ����ϴ�";
			}
		}
	}
	else													
	if (strOmCommandArray[0] == "OM_�ڵ���꼳��")			
	{
		if (strOmCommandArray.GetSize() < 2)				
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�ڵ���꼳�� �Է� �����Դϴ�";	
		}
		else
		if ((HBRM_SET != m_pDevCmn->AtmDefine.CashHandler)	||
			(!m_pDevCmn->ChangeBCFlag)					)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||5������ ������ �����Դϴ�";
		}
		else												
		{
			m_pProfile->GetBRMProfile();
			m_pProfile->BRM.PriorityNote = Asc2Int(strOmCommandArray[1]);
			strOmResult = OM_NORMAL;
			m_pProfile->PutBRMProfile();
		}
	}														
	else
	if (strOmCommandArray[0] == "OM_ī��Ʈ��������")			
	{
		int nNoteKind[4] = {BRM_NOTEKIND_10000_OLD, };
		strTemp.Empty();

		for (int i = 0; i < 4; i++)
		{
			strTemp += strOmCommandArray[i + 1] + ",";
			if (0 == strOmCommandArray[i + 1].CompareNoCase("����"))
				nNoteKind[i] = BRM_NOTEKIND_10000_OLD;			// ��������
			else
			if (0 == strOmCommandArray[i + 1].CompareNoCase("��õ"))
				nNoteKind[i] = BRM_NOTEKIND_1000_OLD;			// ��õ���� : 2006.10.11
			else
			if (0 == strOmCommandArray[i + 1].CompareNoCase("�Ÿ�"))
				nNoteKind[i] = BRM_NOTEKIND_10000_NEW;			// �Ÿ����� : 2006.11.03
			else
			if (0 == strOmCommandArray[i + 1].CompareNoCase("��õ"))
				nNoteKind[i] = BRM_NOTEKIND_1000_NEW;			// ��õ���� : 2006.11.03
			else											
			if (0 == strOmCommandArray[i + 1].CompareNoCase("����"))
				nNoteKind[i] = BRM_NOTEKIND_50000;
			else
			if (0 == strOmCommandArray[i + 1].CompareNoCase("�Աݱ�"))
				nNoteKind[i] = BRM_NOTEKIND_DEP_OLD;			// �Ա�����(��õ,����õ) : 2007.06.29
			else
			if (0 == strOmCommandArray[i + 1].CompareNoCase("�Աݽ�"))
				nNoteKind[i] = BRM_NOTEKIND_DEP_NEW;			// �Ա�����(��õ,�ſ�õ) : 2007.06.29
			else
				nNoteKind[i] = BRM_NOTEKIND_10000_OLD;			// ��������(Default)
		}
		strTemp.Empty();
		
		m_pDevCmn->fnBRM_SetNoteInfo(nNoteKind[0], nNoteKind[1], nNoteKind[2], nNoteKind[3]);
		if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
		{
			strTemp.Format("%s||%s||%s||%s", strOmCommandArray[1], 
											 strOmCommandArray[2], 
											 strOmCommandArray[3], 
											 strOmCommandArray[4]);

			if (m_pDevCmn->ChangeBCFlag)						
				strTemp += "||����";
			else
				strTemp += "||�Ұ�";

			SetShareData("OM_ī��Ʈ��������", strTemp);
			strOmResult = OM_NORMAL;

			if (strTemp.Find("����") >= 0)						
			{												
				m_pProfile->GetTransProfile();
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_WITH_50TCASH;	
				m_pDevCmn->Withdraw50TFlag = TRUE;				
				m_pProfile->PutTransProfile();
			}												
			else												
			{
				m_pProfile->GetTransProfile();
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_WITH_50TCASH; 
				m_pDevCmn->Withdraw50TFlag = FALSE;						 
				m_pProfile->PutTransProfile();
			}													
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��ְ� �߻��Ͽ����ϴ�";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_��ݿ켱����")				
	{
		if (0 == strOmCommandArray[1].CompareNoCase("����"))
		{
			m_pDevCmn->fnBRM_Set10000NoteUsePriority(BRM_NOTEKIND_10000_OLD);
			SetShareData("OM_��ݿ켱����", "����");
		}
		else
		{
			m_pDevCmn->fnBRM_Set10000NoteUsePriority(BRM_NOTEKIND_10000_NEW);
			SetShareData("OM_��ݿ켱����", "�Ÿ�");
		}

		strOmResult = OM_NORMAL;
	}
	else
	if (strOmCommandArray[0] == "OM_�Աݱ�������")				
	{
		m_pProfile->GetBRMProfile();
		if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)
		{
			m_pProfile->GetBRMProfile();

			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)
			{
				int nEnabledNoteKind = RegGetInt(_REGKEY_BRMINFO, "�Աݰ��ɱ���",
											 DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000);	// V06-01-01-#01

				if (strOmCommandArray[1] == "����")					// ��������
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_10000_OLD, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_10000_OLD;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_10000_OLD, FALSE);
					nEnabledNoteKind &= ~ DEP_AVAIL_10000_OLD;
				}

				if (strOmCommandArray[2] == "����")					// ����õ����
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_5000_OLD, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_5000_OLD;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_5000_OLD, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_5000_OLD;
				}

				if (strOmCommandArray[3] == "����")					// ��õ����
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_1000_OLD, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_1000_OLD;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_1000_OLD, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_1000_OLD;
				}

				if (strOmCommandArray[4] == "����")					// �Ÿ�����
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_10000_NEW, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_10000_NEW;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_10000_NEW, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_10000_NEW;
				}

				if (strOmCommandArray[5] == "����")					// �ſ�õ����
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_5000_NEW, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_5000_NEW;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_5000_NEW, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_5000_NEW;
				}

				if (strOmCommandArray[6] == "����")					// ��õ����
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_1000_NEW, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_1000_NEW;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_1000_NEW, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_1000_NEW;
				}


				if (m_pDevCmn->ChangeBCFlag)						
				{
					if (strOmCommandArray[7] == "����")
					{
						m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_50000, TRUE);
						nEnabledNoteKind |= DEP_AVAIL_50000;
					}
					else
					{
						m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_50000, FALSE);
						nEnabledNoteKind &= ~DEP_AVAIL_50000;
					}
				}
				else											
				{												
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_50000, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_50000;		
				}												

				m_pProfile->BRM.DepositCashKind = nEnabledNoteKind;
				//////////////////////////////////////////////////////////////////////////
				if (!(nEnabledNoteKind & (DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD)))
					m_pDevCmn->OldCashDepMode = FALSE;				// ����(��/��õ/õ����) �ԱݺҰ�
				else
					m_pDevCmn->OldCashDepMode = TRUE;				// ����(��/��õ/õ����) �Աݰ���
				//////////////////////////////////////////////////////////////////////////

				RegSetInt(_REGKEY_BRMINFO, "�Աݰ��ɱ���", nEnabledNoteKind);
			
				if (HBRM_SET == m_pProfile->DEVICE.CashHandler)
				{
					strTemp.Format("%s||%s||%s||%s||%s||%s||%s", strOmCommandArray[1],
															 strOmCommandArray[2],
															 strOmCommandArray[3],
															 strOmCommandArray[4],
															 strOmCommandArray[5],
															 strOmCommandArray[6],
															 strOmCommandArray[7]);

					if (m_pDevCmn->ChangeBCFlag)				
						strTemp += "||����";
					else
						strTemp += "||�Ұ�";
				}
				else											
					strTemp.Format("%s||%s||%s||%s||%s||%s", strOmCommandArray[1],
															 strOmCommandArray[2],
															 strOmCommandArray[3],
															 strOmCommandArray[4],
															 strOmCommandArray[5],
															 strOmCommandArray[6]);


			}
			else
			{
				// 0:��õ����/õ����(����,�Ұ�)
				if (strOmCommandArray[1] == "����")						// V200
				{
					m_pProfile->BRM.DepositCashKind = 0;

					RegSetInt(_REGKEY_BRMINFO, "�����Ǹ��Աݰ���",	0);	
					IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2�����Ǹ��Աݰ��ɸż�", BRM_MAXRJB2DEPCNT);
				}
				else
				{
					m_pProfile->BRM.DepositCashKind = 1;

					RegSetInt(_REGKEY_BRMINFO, "�����Ǹ��Աݰ���",	1);
					IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2�����Ǹ��Աݰ��ɸż�", 0);
				}
			}
		}
		
		m_pProfile->PutBRMProfile();
		strOmResult = OM_NORMAL;
	}
	else
	if (strOmCommandArray[0] == "OM_�Ÿ������Ϻ�ȸ��")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�Ÿ����Ϻ�ȸ��]"));
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Ϻ�ȸ�� �Է� �����Դϴ�";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{
			RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_10000_NEW);			// 0x35 �űǸ���

//			m_pTranCmn->Om_GetCurrentActionRemain();

			nTemp = Asc2Int(strOmCommandArray[1]);
			m_pDevCmn->fnBRM_TakeupMoney(nTemp);				// ȸ��(���ݸż�)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// ���۴�����ְ���
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// ����Segment���
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// ȸ��������ϱ�
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	
			{
				if(0 < Asc2Int(strTempArray[8]))				//�Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[7])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[8]),	// �Ҹ��ȸ���ż�
									Asc2Int(strTempArray[6]),	// ������ȸ���ż�		
									Asc2Int(strTempArray[9]),	// ������ ����ȸ���ż�	
									Asc2Int(strTempArray[10]));	// �������� ����ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),		
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	
				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1����
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);
				m_pDevCmn->SCSTCntF = 2;
			}
			else
			{													
				if(0 < Asc2Int(strTempArray[7]))				// �Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[6])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[7]));	// �Ҹ��ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));

			}
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{

// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��� �����Դϴ�";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_��õ�����Ϻ�ȸ��")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��õ���Ϻ�ȸ��]",
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4])), JNL_OPER);
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Ϻ�ȸ�� �Է� �����Դϴ�";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{

			RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_1000_NEW);			// 0x37 �ű�õ��

			nTemp = Asc2Int(strOmCommandArray[1]);
			m_pDevCmn->fnBRM_TakeupMoney(nTemp);				// ȸ��(���ݸż�)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// ���۴�����ְ���
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// ����Segment���
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// ȸ��������ϱ�
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	// 
			{
				if(0 < Asc2Int(strTempArray[8]))				// �Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[7])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[8]),	// �Ҹ��ȸ���ż�
									Asc2Int(strTempArray[6]),	// ������ȸ���ż�	
									Asc2Int(strTempArray[9]),	// ������ ����ȸ���ż�	
									Asc2Int(strTempArray[10]));	// �������� ����ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));
			}
			else
			{												
				if(0 < Asc2Int(strTempArray[7]))				//�Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),		// ȸ���ż�
									Asc2Int(strTempArray[3]),		// ����ȸ���ż�
									Asc2Int(strTempArray[4]),		// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),		// õ��ȸ���ż�
									Asc2Int(strTempArray[6])		// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[7]));		// �Ҹ��ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));

			}
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)				
					strOmResult += "||" + strTemp + "||����";
				else										
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��� �����Դϴ�";
		}

	}
	else														
	if (strOmCommandArray[0] == "OM_���������Ϻ�ȸ��")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�������Ϻ�ȸ��]"));
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Ϻ�ȸ�� �Է� �����Դϴ�";
		}
		else
		if ((HBRM_SET != m_pDevCmn->AtmDefine.CashHandler)	||	
			(!m_pDevCmn->ChangeBCFlag)					)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||5������ ������ �����Դϴ�";
		}
		else												
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{

			RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_50000);			// 0x34 ������
			
//			m_pTranCmn->Om_GetCurrentActionRemain();

			nTemp = Asc2Int(strOmCommandArray[1]);
			m_pDevCmn->fnBRM_TakeupMoney(nTemp);				// ȸ��(���ݸż�)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// ���۴������j����
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// ����Segment���
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// ȸ��������ϱ�

			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)
			{
				if(0 < Asc2Int(strTempArray[8]))				//�Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[7])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[8]),	// �Ҹ��ȸ���ż�
									Asc2Int(strTempArray[6]),	// ������ȸ���ż�		
									Asc2Int(strTempArray[9]),	// ������ ����ȸ���ż�	
									Asc2Int(strTempArray[10]));	// �������� ����ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));

				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1����
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

				m_pDevCmn->SCSTCntF = 2;
			}

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				strOmResult += "||" + strTemp + "||����";	
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||" + strTemp + "||����";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��� �����Դϴ�";
		}
	}			
	else
	if ((strOmCommandArray[0] == "OM_�Ÿ�������üȸ��") &&
		 (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))			// ��õ������������ 2006.10.26
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ű� ���� ��üȸ��]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{
			RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_10000_NEW);			// 0x35 �űǸ���

			m_pDevCmn->fnBRM_TakeupMoney(0xffff);				// ȸ��(���ݸż�)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// ���۴�����ְ���
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// ����Segment���
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// ȸ��������ϱ�
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	//
			{
				if(0 < Asc2Int(strTempArray[8]))				//�Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[7])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[8]),	// �Ҹ��ȸ���ż�
									Asc2Int(strTempArray[6]),	// ������ȸ���ż�		
									Asc2Int(strTempArray[9]),	// ������ ����ȸ���ż�	
									Asc2Int(strTempArray[10]));	// �������� ����ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	



				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1����
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

			}
			else
			{													
				if(0 < Asc2Int(strTempArray[7]))				// �Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[6])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[7]));	// �Ҹ��ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6]) + Asc2Int(strTempArray[7]));

			}
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)				
					strOmResult += "||" + strTemp + "||����";
				else										
					strOmResult += "||" + strTemp + "||�Ұ�";
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��� �����Դϴ�";
		}

		RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_10000_OLD);			// 0x30 ���Ǹ���
	}
	else
	if ((strOmCommandArray[0] == "OM_��õ������üȸ��") &&
		 (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))			// ��õ������������ 2006.10.26
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��õ������üȸ��]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{

			RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_1000_NEW);			// 0x37 �ű�õ��

			m_pDevCmn->fnBRM_TakeupMoney(0xffff);				// ȸ��(���ݸż�)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// ���۴�����ְ���
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// ����Segment���
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// ȸ��������ϱ�
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	
			{
				if(0 < Asc2Int(strTempArray[8]))				//�Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[7])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[8]),	// �Ҹ��ȸ���ż�
									Asc2Int(strTempArray[6]),	// ������ȸ���ż�		
									Asc2Int(strTempArray[9]),	// ������ ����ȸ���ż�	
									Asc2Int(strTempArray[10]));	// �������� ����ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	
			}
			else
			{													
				if(0 < Asc2Int(strTempArray[7]))				// �Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[6])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[7]));	// �Ҹ��ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6]) + Asc2Int(strTempArray[7]));
			}
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)				
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��� �����Դϴ�";
		}
		RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_10000_OLD);			// 0x30 ���Ǹ���
	}
	else														
	if ((strOmCommandArray[0] == "OM_����������üȸ��") &&
		 (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))			// ��õ������������ 2006.10.26
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ ��üȸ��]"));
		if ((HBRM_SET != m_pDevCmn->AtmDefine.CashHandler)	||
			(!m_pDevCmn->ChangeBCFlag)					)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||5������ ������ �����Դϴ�";
		}
		else													
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{


			RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_50000);			// 0x37 �ű�õ��

			m_pDevCmn->fnBRM_TakeupMoney(0xffff);				// ȸ��(���ݸż�)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// ���۴�����ְ���
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// ����Segment���
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// ȸ��������ϱ�
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)
			{
				if(0 < Asc2Int(strTempArray[8]))				// �Ҹ�� �ȳ����� ���� : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// ȸ���ż�
									Asc2Int(strTempArray[3]),	// ����ȸ���ż�
									Asc2Int(strTempArray[4]),	// ��õ��ȸ���ż�
									Asc2Int(strTempArray[5]),	// õ��ȸ���ż�
									Asc2Int(strTempArray[7])	// ��ǥȸ���ż�
								+	Asc2Int(strTempArray[8]),	// �Ҹ��ȸ���ż�
									Asc2Int(strTempArray[6]),	// ������ȸ���ż�		
									Asc2Int(strTempArray[9]),	// ������ ����ȸ���ż�	
									Asc2Int(strTempArray[10]));	// �������� ����ȸ���ż�

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	


				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1����
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

			}

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||" + strTemp + "||����";	
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||" + strTemp + "||����";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��� �����Դϴ�";
		}

	}															
	else
	if ((strOmCommandArray[0] == "OM_�ű��Ա�����üȸ��") &&	// �Ա����� ī��Ʈ ȸ������
		 (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))	
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ű��Ա�����üȸ��]"), JNL_OPER);
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{
			RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_DEP_NEW);

			m_pDevCmn->fnBRM_TakeupMoney(0xffff);				// ȸ��(���ݸż�)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// ���۴�����ְ���
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// ����Segment���
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// ȸ��������ϱ�

			if(0 < Asc2Int(strTempArray[8]))					// �Ҹ�� �ȳ����� ����
			{
				m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
			}

			m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
								Asc2Int(strTempArray[1]),		// ȸ���ż�
								Asc2Int(strTempArray[3]),		// ����ȸ���ż�
								Asc2Int(strTempArray[4]),		// ��õ��ȸ���ż�
								Asc2Int(strTempArray[5]),		// õ��ȸ���ż�
								Asc2Int(strTempArray[7])		// ��ǥȸ���ż�
							+	Asc2Int(strTempArray[8]),		// �Ҹ��ȸ���ż�
								Asc2Int(strTempArray[6]),		// ������ȸ���ż�		
								Asc2Int(strTempArray[9]),		// ������ ����ȸ���ż�	
								Asc2Int(strTempArray[10]));		// �������� ����ȸ���ż�

			strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
								Asc2Int(strTempArray[1]),		
								Asc2Int(strTempArray[3]),
								Asc2Int(strTempArray[4]),
								Asc2Int(strTempArray[5]),
								Asc2Int(strTempArray[7])
							+	Asc2Int(strTempArray[8]),
								Asc2Int(strTempArray[6]),
								Asc2Int(strTempArray[9]),		
								Asc2Int(strTempArray[10]));		
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||����";	
				else											
					strOmResult += "||" + strTemp + "||�Ұ�";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���ݺ� ��� �����Դϴ�";
		}

	}
	else
	if (strOmCommandArray[0] == "OM_��ǥ����")
	{
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ���� �Է� �����Դϴ�";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		{

			m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_SPR);
			for (i = 2; i <= strOmCommandArray.GetSize();  i++)
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, i - 2, 0, strOmCommandArray[i - 1]);
			m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR, JNL_OPER);
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
			{
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);
				if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
					strOmResult = OM_NORMAL;
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||��ǥ������ ��ְ� �߻��Ͽ����ϴ�";
				}
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||��ǥ������ ��ְ� �߻��Ͽ����ϴ�";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ�� ��� �����Դϴ�";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_��������")
	{
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�������� �Է� �����Դϴ�";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_JPR))
		{

			m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_JPR);
			for (i = 2; i <= strOmCommandArray.GetSize();  i++)
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, i - 2, 0, strOmCommandArray[i - 1]);
			m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_JPR, JNL_OPER);
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_JPR))
				strOmResult = OM_NORMAL;
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||���������� ��ְ� �߻��Ͽ����ϴ�";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||���κ� ��� �����Դϴ�";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_��ǥ��������")
	{
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ�������� �Է� �����Դϴ�";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR))
		{

			m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_SPR | DEV_JPR);
			for (i = 2; i <= strOmCommandArray.GetSize();  i++)
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR | DEV_JPR, i - 2, 0, strOmCommandArray[i - 1]);
			m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR | DEV_JPR, JNL_OPER);
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR | DEV_JPR))
			{
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);
				if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
					strOmResult = OM_NORMAL;
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||��ǥ,���������� ��ְ� �߻��Ͽ����ϴ�";
				}
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||��ǥ,���������� ��ְ� �߻��Ͽ����ϴ�";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ,���κ� ��� �����Դϴ�";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_��ü�׽�Ʈ")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ü�׽�Ʈ]"));
		m_pDevCmn->fnLDCtrl_DisableWatchdogRpc();				
		ReadyDevTest();											// ��ü�׽�Ʈ�غ�
		m_pDevCmn->fnAPL_ClerkInformation();					// �������
		SetShareData("OM_RESULT", OM_NORMAL);					// ����ó��
		return FALSE;											// ����ó��
	}
	else
	if (strOmCommandArray[0] == "OM_��⼳��")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��⼳��]"));
		if (strOmCommandArray.GetSize() < 3)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��⼳�� �Է� �����Դϴ�";
		}
		else
		{
			m_pProfile->GetDeviceProfile();

			// 0:�������(T2ATM,T1ATM,SATM,RATM,CDP,CD)
			m_pProfile->DEVICE.MachineType = U8500;

			m_pProfile->InitDeviceProfile();

			// 1:��ǥ������(T1SHU,NTSHU,HSHU,���SHU1,���SHU2)
			if (!strOmCommandArray[2].CompareNoCase("T1SHU"))
			{
				m_pProfile->DEVICE.SHUHandler = SHU_MECHA_A2;
				m_pProfile->DEVICE.CameraDevice = USB_CAMERA_SET;
			}
			else
			if (!strOmCommandArray[2].CompareNoCase("NTSHU"))
			{
				m_pProfile->DEVICE.SHUHandler = SHU_MECHA_NT;
				m_pProfile->DEVICE.CameraDevice = USB_CAMERA_SET;
				RegSetInt(_REGKEY_DEVINFO, "ShuType", 3);
			}
			m_pProfile->PutDeviceProfile();
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_�ŷ��Ϸù�ȣ����")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��Ϸù�ȣ����]"));
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Ϸù�ȣ���� ����";
		}
		else
		{

			m_pProfile->GetTransProfile();
			// 0:�����Ϸù�ȣ(NNNN)
			strTemp = SPACE6 + strOmCommandArray[1];
			strTemp = strTemp.Right(6);
			memcpy(m_pProfile->TRANS.StartSerialNo, strTemp.GetBuffer(0), 6);
			m_pProfile->TRANS.StartSerialNo[6] = NULL;
			m_pProfile->PutTransProfile();
			m_pDevCmn->fnAPL_SetSerialNo();

			strOmResult = OM_NORMAL;
			strOmResult += "||�ŷ��Ϸù�ȣ���� �Ϸ�";
			m_pDevCmn->fnAPL_ClerkInformation();					// �������
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��Ϸù�ȣ����] %s", strOmResult));				

	}
	else
	if (strOmCommandArray[0] == "OM_��������")
	{

		HWND hAviWnd;
		if (0 == strOmCommandArray[1].CompareNoCase("ON"))
		{
			SetShareData("OM_��������", "ON");
//			hAviWnd = ::FindWindow(NULL, "AviPlayer");
			hAviWnd = _FIND_CFMON;
			if (hAviWnd)
				::PostMessage(hAviWnd, WUM_SOUND_ON, 0, 0);
		}
		else
		{
			SetShareData("OM_��������", "OFF");
//			hAviWnd = ::FindWindow(NULL, "AviPlayer");
			hAviWnd = _FIND_CFMON;
			if (hAviWnd)
				::PostMessage(hAviWnd, WUM_SOUND_OFF, 0, 0);
		}

		strOmResult = OM_NORMAL;
	}
	else
	if (strOmCommandArray[0] == "OM_����")
	{

		if ((m_pProfile->DEVICE.MachineType == U3100K)	||
			(m_pProfile->DEVICE.MachineType == U8100)	)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[����]"));
		}
		m_pDevCmn->fnAPL_ClerkInformation();					// �������
		SetShareData("OM_RESULT", OM_NORMAL);					// ����ó��
		return FALSE;											// ����ó��
	}
	else
	if (strOmCommandArray[0] == "OM_ī�޶��׽�Ʈ")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[ī�޶��׽�Ʈ]"));
		if(m_pProfile->DEVICE.MachineType == U3100K || m_pProfile->DEVICE.MachineType == U8100) 
		{
			if (strOmCommandArray.GetSize() < 5)
			{
				strOmResult = OM_ERROR;
				strOmResult += "||ī�޶��׽�Ʈ �Է� �����Դϴ�";
			}
			else
			{
				DeleteFile(strOmCommandArray[1]+".Jpg");
				DeleteFile(strOmCommandArray[2]+".Jpg");
				DeleteFile(strOmCommandArray[3]+".Jpg");
				DeleteFile(strOmCommandArray[4]+".Jpg");


				CFileFind *m_file;
				ULONG nWaitTime;

				if (m_pDevCmn->fnCMR_GetFaceCameraDeviceStatus() == NORMAL)
				{
					nErrorFlag = m_pDevCmn->fnCMR_CaptureFace(strOmCommandArray[1]);
					m_file = new CFileFind;
					nWaitTime = TimerSet(5);
					while (!CheckTimer(nWaitTime))
					{
					
						Delay_Msg(400);
						if(m_file->FindFile(strOmCommandArray[1]+".Jpg"))
						{
							strOmResult = OM_NORMAL;
							strOmResult += "||" + strOmCommandArray[1]+".Jpg";
							break;
						}
						else
						{
							strOmResult = OM_NORMAL;
							strOmResult += "||NG";
						}

					}

					if (m_file!=NULL)
					{
						delete m_file;	
						m_file = NULL;
					}

				}
				else
				{
						strOmResult = OM_NORMAL;
						strOmResult += "||NG";
				}

				if (m_pDevCmn->fnCMR_GetHandCameraDeviceStatus() == NORMAL)
				{
					nErrorFlag = m_pDevCmn->fnCMR_CaptureHand(strOmCommandArray[2]);
					m_file = new CFileFind;
					nWaitTime = TimerSet(5);
					while (!CheckTimer(nWaitTime))
					{
						Delay_Msg(400);
						if(m_file->FindFile(strOmCommandArray[2]+".Jpg"))
						{
							strOmResult += "||" + strOmCommandArray[2]+".Jpg";
							break;
						}
						else
						{
							strOmResult += "||NG";
						}
					}

					if (m_file!=NULL)
					{
						delete m_file;	
						m_file = NULL;
					}

				}
				else
				{
						strOmResult += "||NG";
				}

				strOmResult += "||NG";
				strOmResult += "||NG";
					
			}
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[ī�޶��׽�Ʈ] %s", strOmResult));				
		m_pDevCmn->fnAPL_ClerkInformation();					// �������
		SetShareData("OM_RESULT", strOmResult);					// ����ó��
		return FALSE;											// ����ó��
	}
	else
	if (strOmCommandArray[0] == "OM_POWEROFFSTATUS")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[POWEROFFSTATUS]"));
		{

			m_pDevCmn->AtmStatus = ATM_DOWN;
			strOmResult = OM_NORMAL;
			strOmResult += "||��� ���� �� �Դϴ�!";
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[POWEROFFSTATUS] %s", strOmResult));
		m_pDevCmn->fnAPL_ClerkInformation();					// �������
		SetShareData("OM_RESULT", strOmResult);					// ����ó��
		return FALSE;											// ����ó��
	}
	else
	if (strOmCommandArray[0] == "OM_CLOSESHUTTER")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[CLOSESHUTTER]"));
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
		if (m_pDevCmn->fnCSH_GetDeviceStatus() != NODEVICE)		// 2005.03.30
			m_pDevCmn->fnCSH_CloseShutter();					// Close Shutter
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
		
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
		{
			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";								// Null Message
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||[SHUTTER ���]";								// Null Message
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[CLOSESHUTTER] %s", strOmResult));
		m_pDevCmn->fnAPL_ClerkInformation();					// �������
		SetShareData("OM_RESULT", strOmResult);					// ����ó��
		return FALSE;											// ����ó��
	}
	else
	if (strOmCommandArray[0] == "OM_OPENSHUTTER")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OPENSHUTTER]"));
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
		if (m_pDevCmn->fnCSH_GetDeviceStatus() != NODEVICE)		// 2005.03.30
			m_pDevCmn->fnCSH_OpenShutter();					// Close Shutter
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
		
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
		{
			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";								// Null Message
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||[SHUTTER ���]";								// Null Message
		}

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OPENSHUTTER] %s", strOmResult));

		m_pDevCmn->fnAPL_ClerkInformation();					// �������
		SetShareData("OM_RESULT", strOmResult);					// ����ó��
		return FALSE;											// ����ó��
	}
	else
	if (strOmCommandArray[0] == "OM_LOCKOPEN")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LOCKOPEN]"));

		if (m_pDevCmn->fnLDCtrl_GetDeviceStatus() == NORMAL)		// 2005.03.30
		{
			int nFlag = FALSE;
			
			if((strOmCommandArray[1].GetLength() != 8) || 
			   (!IsNum(strOmCommandArray[1].GetBuffer(0), strOmCommandArray[1].GetLength())) || 
			   (IsZero(strOmCommandArray[1].GetBuffer(0), strOmCommandArray[1].GetLength())))
			{
				strOmResult = OM_ERROR;
				strOmResult += "||[Ű �Է� ���� !!]";								// Null Message				
			}
			else
			{
				if(m_pDevCmn->fnLDCtrl_GetLoadKeyCheckFlag(strOmCommandArray[1]) == LOCK_STAT_CHECK_OK)
				{
					if(m_pDevCmn->fnLDCtrl_OpenRpcLock("", 2) == T_ERROR)
					{
						strOmResult = OM_ERROR;
						strOmResult += "||[RPC LOCK ���(�������)]";								// Null Message				
					}
					else
					{
						strOmResult = OM_NORMAL;
						strOmResult += "||[���� ����] �� 5�� �� �ڵ� ��� ��";								// Null Message
					}			
				}
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||[�Է� Ű �� ��ġ!!]";								// Null Message				
				}
			}

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||[RPC LOCK ���]";								// Null Message
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LOCKOPEN] %s", strOmResult));
		m_pDevCmn->fnAPL_ClerkInformation();					// �������
		SetShareData("OM_RESULT", strOmResult);					// ����ó��
		return FALSE;											// ����ó��
	}
	else
	if (strOmCommandArray[0] == "OM_FNGREAD")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ν��׽�Ʈ]"));
		int nRet = FALSE;
		if (m_pDevCmn->fnAPL_GetDefineDevice(DEV_FNG))
		{
			if(m_pDevCmn->fnFNG_GetDeviceStatus(1) == NORMAL)
			{
				m_pDevCmn->fnFNG_Initialize();							// ������۹��ʱ�ȭ
				m_pDevCmn->fnFNG_Acquire();								// �����б�
				nRet = m_pDevCmn->fnCMN_ScanDeviceAction(DEV_FNG | DEV_SCR, K_15_WAIT, EVENT_IN);
				if (nRet == R_TIMEOUT)
				{
					m_pDevCmn->fnFNG_CancelAcquire();					// �����б����
					strOmResult = OM_ERROR;
					strOmResult += "||NG||[����(�����ν�) �ð� �ʰ�]";								// Null Message
				}
				else
				if (nRet == DEV_FNG)
				{
					Delay_Msg(1000);									// ����Ȯ�δ��								
					strOmResult = OM_NORMAL;
					strOmResult += "||FNGDATA="+m_pDevCmn->fstrFNG_GetData()+"||[���� ����]";								// Null Message
					m_pDevCmn->fnFNG_CancelAcquire();					// �����б����
				}
				else
				{
					m_pDevCmn->fnFNG_CancelAcquire();					// �����б����
					strOmResult = OM_ERROR;
					strOmResult += "||NG||[����(�����ν�) �ð� �ʰ�]";								// Null Message
				}
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||NG||[�����ν� �������]";								// Null Message				
			}
			

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||NG||[�����ν� �̻��]";								// Null Message
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ν��׽�Ʈ %s]", strOmResult));
		m_pDevCmn->fnAPL_ClerkInformation();					// �������
		SetShareData("OM_RESULT", strOmResult);					// ����ó��
		return FALSE;											// ����ó��
	}
	else
	if (strOmCommandArray[0] == "OM_PBMPRINT")  //#N0211
	{
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� �����Դϴ�";
		}
		else
		{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�������� �׽�Ʈ]"));


		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		{
			int	nReturn = 0;
				m_pTranCmn->PPPRecvData.y = Asc2Int(strOmCommandArray[1]);
			nReturn = m_pTranCmn->fnAPD_PBMPrintTest();
			if (nReturn == T_OK)
			{
				if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))
				{
					strOmResult = OM_NORMAL;
					strOmResult += "||�����׽�Ʈ�� �Ϸ� �Ǿ����ϴ�";
				}
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||�����׽�Ʈ�� ��ְ� �߻��Ͽ����ϴ�";
				}
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||�����׽�Ʈ�� ��ְ� �߻��Ͽ����ϴ�";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||����� ��� �����Դϴ�";
			}
		}
	}
	else
	if (strOmCommandArray[0] == "OM_����")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���Ϲ�ư����]"));
		char szTmp[1024];
		sprintf(szTmp, "00%6.6s", GetTime().GetBuffer(0));
		if(m_pDevCmn->fnAPL_GetDefineDevice(DEV_RDCTRL) && (m_pProfile->DEVICE.MachineType == U3100K))
		{
			m_pDevCmn->fnLDCtrl_SetKeytableFile(0, szTmp);
			m_pDevCmn->fnLDCtrl_SetKeytableFile(0, szTmp);
			m_pDevCmn->fnLDCtrl_SetKeytableFile(0, szTmp);
			m_pDevCmn->fnLDCtrl_SetKeytableFile(0, szTmp);
			m_pDevCmn->fnLDCtrl_SetKeytableFile(0, szTmp);			
			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";								// Null Message
		}
		else
		{
			//#0063
			if (m_pProfile->DEVICE.MachineType == U8100)
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||[���� ����]";								// Null Message
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||RPC LOCK ���(�������)";								// Null Message
			}
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���Ϲ�ư���� %s]", strOmResult));
		m_pDevCmn->fnAPL_ClerkInformation();					// �������
		SetShareData("OM_RESULT", strOmResult);					// ����ó��
		return FALSE;											// ����ó��
	}
	else 
	if (strOmCommandArray[0] == "OM_SPACKSETUP") //OM_SPACKSETUP||�����ġ����(use  =1)||�ܺμ���(0, 1 = #1, 1 = #2)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ġ���� %s %s]", strOmCommandArray[1], strOmCommandArray[2]));
		if (strOmCommandArray.GetSize() >= 2)
		{

			if(Asc2Int(strOmCommandArray[1]) == 1)
			{
				nErrorFlag = FALSE;
				if(Asc2Int(strOmCommandArray[2]) == 0)
				{
					if(m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_OFF) == TRUE)
						nErrorFlag = FALSE;
					else
						nErrorFlag = TRUE;
				}
				else
				if(Asc2Int(strOmCommandArray[2]) == 1)
				{
					if(m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_ON, CLKEY_OFF) == TRUE)
						nErrorFlag = FALSE;
					else
						nErrorFlag = TRUE;
				}
				else
				if(Asc2Int(strOmCommandArray[2]) == 2)
				{
					if(m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_ON) == TRUE)
						nErrorFlag = FALSE;
					else
						nErrorFlag = TRUE;
				}
				else
				{
					if(m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_OFF) == TRUE)
						nErrorFlag = FALSE;
					else
						nErrorFlag = TRUE;
				}

				if(nErrorFlag)
				{
						strOmResult = OM_ERROR;
						strOmResult += "||[�����ġ ���]";											
				}
				else
				{
					
					Delay_Msg(5000);
					int nGetExternalS1EnSt = m_pDevCmn->fnSPACK_GetExternalSensorEnableStatus_1();
					int nGetExternalS2EnSt = m_pDevCmn->fnSPACK_GetExternalSensorEnableStatus_2();

					if(nGetExternalS1EnSt)
						IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 1);

					if(nGetExternalS2EnSt)
						IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 2);

					if(nGetExternalS1EnSt && nGetExternalS2EnSt)
						IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 3);

					if(!nGetExternalS1EnSt && !nGetExternalS2EnSt)
						IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 0);

					strOmResult = OM_NORMAL;
					strOmResult += "||[���� ����]";							
				}
			}
			else
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||[�����ġ �̻��]";						// Null Message
			}
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ġ���� %s]", strOmResult));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� �����Դϴ�";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);

		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���������� %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_����������") //OM_����������|ȸ������|����ȣ|��������Ʈ|ADSLID|ADSL��й�ȣ
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���������� %s %s %s %s %s]", strOmCommandArray[1], strOmCommandArray[2], strOmCommandArray[3], strOmCommandArray[4], strOmCommandArray[5]));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		if (strOmCommandArray.GetSize() >= 7)
		{
			//ȸ������
			IniSetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice9", strOmCommandArray[1]);
			//����ȣ
			IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "PHONENUMBER",strOmCommandArray[2]);
			//����Ʈ
			IniSetInt("D:\\INI\\MOBILECFG.ini","MOBILE", "COM", Asc2Int(strOmCommandArray[3]));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);
			int nModemType = Asc2Int(strOmCommandArray[1]);
			if(nModemType == MODEM_LGT)
			{
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "PPP", "777");
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "CMSSERVER", "0190001259");
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "ID", strOmCommandArray[2]+"@lgt.co.kr");
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "PASS", 	strOmCommandArray[2].Right(4));
			}
			else
			if(nModemType == MODEM_KTF)
			{
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "PPP", "*98#");
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "ID",  "WCDMA");
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "PASS","");
			}
			else
			if(nModemType == MODEM_KT_ADSL)
			{
				CString strTmp = "";
				strTmp = strOmCommandArray[4];
				strTmp.MakeLower();
				//ADSL ID
				IniSetStr("D:\\INI\\ATM_CFG.ini", "ATMINFO", "ATM_MOVE_ID",  strTmp);
				strTmp = "";
				strTmp = strOmCommandArray[5];
				strTmp.MakeLower();
				//ADSL PWD
				IniSetStr("D:\\INI\\ATM_CFG.ini", "ATMINFO", "ATM_MOVE_PWD", strTmp);
			}
			else
			if(nModemType == MODEM_LGT_UPlus || nModemType == MODEM_LGT_U_GPS) //#0137
			{
				AtmSetEnableDHCP(FALSE);
			}
			else
			{
				AtmSetEnableDHCP(TRUE);
			}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);

			//��������ȣ(10 �ڸ�)
			strTemp = "";
			strTemp = strOmCommandArray[6] + SPACE16;
			strTemp = strTemp.Left(16);
			memcpy(m_pProfile->NETWORK.MachineMakeNum, strTemp.GetBuffer(0), strTemp.GetLength()); 
			m_pProfile->PutNetworkProfile();
			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";								// Null Message
		}
		else
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult4[%s]", strOmCommandArray[0]);

			strOmResult = OM_ERROR;
			strOmResult += "||�Է� �����Դϴ�";
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���������� %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_�귣�弳��") //OM_�귣�弳��|�귣�弳��
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�귣�弳��1 %s]", strOmCommandArray[1]));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�귣�弳��2 %s]", strOmCommandArray[2]));

		if(m_pDevCmn->AtmDefine.MachineType == U3100K && 
			strOmCommandArray.GetSize() >= 2 && 
			(Asc2Int(strOmCommandArray[2]) != KIUP && Asc2Int(strOmCommandArray[2]) != SHINHAN))
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�������� �ʴ� �귣�� �Դϴ�.";
		}
		else
		if (strOmCommandArray.GetSize() >= 2)
		{
//			IniSetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice11", strOmCommandArray[1]);
			IniSetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice20", strOmCommandArray[2]);
			IniSetInt(_TRANS_INI, TRANS_SEC, "BrandJehyu", 0);

			int nApKind = IniGetInt(_DEVICE_INI,DEV_SEC, "ETCDevice20", 0);
			
			if(nApKind == KIUP)   //#N0190  ��� �귣�� ������ �ִ� ����ݾ� ����
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//Ÿ�� 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//���� 100

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//�����ATM ����
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if((nApKind == NONGHYUP) || (nApKind == NONGHYUPDAN))   //#N0190  ���� �귣�� ������ �ִ� ����ݾ� ����
			{	// ������� NH���� ������ �귣�� 34�� ���� ������ �̹��� ���� �� �ŷ�  Flow �� ���� �귣�� 14�� �� �ٽ� ������.
				// �̹��� ������ "BrandJehyu" ������ ������.
				IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice20", NONGHYUP);
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//Ÿ�� 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 70);					//���� 70

				//#N0284
				if(nApKind == NONGHYUPDAN)
				{
					IniSetInt(_TRANS_INI, TRANS_SEC, "BrandJehyu", NONGHYUPDAN);					
				}
				else
				{
					IniSetInt(_TRANS_INI, TRANS_SEC, "BrandJehyu", 0);					
				}
			}
			else
			if(nApKind == KEB)       //#N0190  ��ȯ �귣�� ������ �ִ� ����ݾ� ����
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//Ÿ�� 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//���� 100	#N0276

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//�����ATM ����
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == KWANGJU)   //#N0181  ���� �귣�� ������ �ִ� ����ݾ� ����
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//Ÿ�� 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//���� 100

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//�����ATM ����
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == KYONGNAM)   //#N0192  �泲 �귣�� ������ �ִ� ����ݾ� ����
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//Ÿ�� 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 50);					//���� 50

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  �����ATM ����
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == HANA)   //#N0214  �ϳ� �귣�� ������ �ִ� ����ݾ� ����
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//Ÿ�� 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//���� 100

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  �����ATM ����
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == SHINHAN)   //#N0215  ���� �귣�� ������ �ִ� ����ݾ� ����
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//Ÿ�� 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//���� 100

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  �����ATM ����
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == SC)	 //#N0219  SC ��������
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//Ÿ�� 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//���� 100  #N0241

				//�������� ����� atm���� => �귣�� ������ ����� �����ϸ� ��� ���� ������ �ڵ� off ��� ����
			}
			else
			if(nApKind == KB)	 //#N0226  ��������
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//Ÿ�� 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 70);					//���� 70

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  �����ATM ����
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == CITI)   //#N0238  ��Ƽ �귣�� ������ �ִ� ����ݾ� ����
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//Ÿ�� 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 70);					//���� 70

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  �����ATM ����
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == NICE)   //#N0219  ���ڱ���
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//Ÿ�� 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 30);					//���� 100

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  �����ATM ����
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			
			m_pProfile->GetDeviceProfile();

			RestoreBrandFileCopy(); //#0091
			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";								
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT4; //#0091
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� �����Դϴ�";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);

		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�귣�弳�� %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_�ΰ����񽺼���") //OM_�ΰ����񽺼���|����������
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ΰ����񽺼��� %s]", strOmCommandArray[1]));

		if (strOmCommandArray.GetSize() >= 2)
		{
			IniSetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice11", strOmCommandArray[1]);
			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";								// Null Message
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� �����Դϴ�";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);

		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ΰ����񽺼��� %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_VOLUME") //OM_VOLUME|OM_VOLUME=������
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[VOLUME ���� %s]", strOmCommandArray[1]));

		if (strOmCommandArray.GetSize() >= 2)
		{
			int		nSoundVaule = 0;
			nSoundVaule = Asc2Int(strOmCommandArray[1]);		
			m_pDevCmn->fnSND_KeySound(_SOUND_DDING);		// DDING

			if ((nSoundVaule >= 1)	&&
				(nSoundVaule <= 5)	)
			{
				m_pProfile->DEVICE.SoundVolume = nSoundVaule;
				m_pProfile->PutDeviceProfile();
				nSoundVaule = (DWORD)(nSoundVaule * 11000);
				m_pDevCmn->fnSND_SetCurrentVolume(nSoundVaule);
			}			
			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";							
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� �����Դϴ�";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);

		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[VOLUME ���� %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_��ǥ����") //OM_��ǥ����|OM_��ǥ����=������(1 : ���, 0 : �̻��)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ǥ���� %s]", strOmCommandArray[1]));

		if (strOmCommandArray.GetSize() >= 2)
		{
			m_pProfile->DEVICE.SHUHandler = Asc2Int(strOmCommandArray[1]);
			m_pProfile->PutDeviceProfile();
			m_pProfile->GetDeviceProfile();
			if(m_pProfile->DEVICE.SHUHandler == NOT_SET)
			{
				m_pDevCmn->DeviceAutoOff |= DEV_SPR;
				m_pDevCmn->DeviceDefine = m_pDevCmn->DeviceDefine & (~DEV_SPR); 
			}
			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";				
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT4;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� �����Դϴ�";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ǥ���� %s]", strOmResult));
	}
	else
	if (strOmCommandArray[0] == "OM_��������")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��������]"));
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ���� �Է� �����Դϴ�";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		{
			m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_SPR);
			strTemp = GetShareData("OM_Main_CD_VanIII_ApSpEpVersion");
			SplitString(strTemp, "||", strTempArray);
			if (m_pProfile->DEVICE.MachineType == U3100K)
			{
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 0, 0, "      [SP Version]    [EP Version]");
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 1, 0, "  BRM :"+strTempArray[2]+"    "+strTempArray[2+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 2, 0, "  JPR :"+strTempArray[3]+"    "+strTempArray[3+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 3, 0, "  SPR :"+strTempArray[4]+"    "+strTempArray[4+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 4, 0, "  MCU :"+strTempArray[5]+"    "+strTempArray[5+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 5, 0, "  PIN :"+strTempArray[6]+"    "+strTempArray[6+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 6, 0, "  CAM :"+strTempArray[7]+"    "+strTempArray[7+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 7, 0, "  SIU :"+strTempArray[8]+"    "+strTempArray[8+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 8, 0, "  UPS :"+strTempArray[9]+"    "+strTempArray[9+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 9, 0, "  LOK :"+strTempArray[10]+"    "+strTempArray[10+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 10, 0, "  FNG :"+strTempArray[11]+"    "+strTempArray[11+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 11, 0, "  PBM :"+strTempArray[23]+"    "+strTempArray[24]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 12, 0, "  MWI :"+strTempArray[12]);
				strTemp.Format("%9.9s", m_pProfile->TRANS.ApVersion);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 13, 0, "  AP :"+strTemp);
				m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR, JNL_OPER);
			}
			else
			if (m_pProfile->DEVICE.MachineType == U8100)
			{
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 0, 0, "[SP Version]");
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 1, 0, "BRM:"+strTempArray[2]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 2, 0, "JPR:"+strTempArray[3]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 3, 0, "SPR:"+strTempArray[4]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 4, 0, "MCU:"+strTempArray[5]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 5, 0, "PIN:"+strTempArray[6]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 6, 0, "CAM:"+strTempArray[7]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 7, 0, "SIU:"+strTempArray[8]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 8, 0, "UPS:"+strTempArray[9]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 9, 0, "LOK:"+strTempArray[10]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 10, 0, "FNG:"+strTempArray[11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 11, 0, "PBM:"+strTempArray[23]);
				m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR, JNL_OPER);
				m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR);
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);
				
				m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_SPR); ////#0095				
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 0, 0, "[EP Version]");
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 1, 0, "BRM:"+strTempArray[2+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 2, 0, "JPR:"+strTempArray[3+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 3, 0, "SPR:"+strTempArray[4+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 4, 0, "MCU:"+strTempArray[5+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 5, 0, "PIN:"+strTempArray[6+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 6, 0, "CAM:"+strTempArray[7+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 7, 0, "SIU:"+strTempArray[8+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 8, 0, "UPS:"+strTempArray[9+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 9, 0, "LOK:"+strTempArray[10+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 10, 0, "FNG:"+strTempArray[11+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 11, 0, "PBM:"+strTempArray[24]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 12, 0, "MWI:"+strTempArray[12]);
				strTemp.Format("%9.9s", m_pProfile->TRANS.ApVersion);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 13, 0, "AP  :"+strTemp);
				m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR, JNL_OPER);
			}

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
			{
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);
				if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
				{
					strOmResult = OM_NORMAL;
					strOmResult += "||[���� ����]";	
				}
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||��ǥ������ ��ְ� �߻��Ͽ����ϴ�";
				}
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||��ǥ������ ��ְ� �߻��Ͽ����ϴ�";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||��ǥ�� ��� �����Դϴ�";
		}
	}
	else 
	if (strOmCommandArray[0] == "OM_��¥�ð�����") //OM_��¥�ð�����||201207230123
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��¥�ð����� %s]", strOmCommandArray[1]));
		if (strOmCommandArray.GetSize() >= 2)
		{
			m_pDevCmn->fnAPL_SetDate(strOmCommandArray[1]);
			m_pDevCmn->fnAPL_SetCheckDate(strOmCommandArray[1]);
			m_pTranCmn->fnAPP_Set_SystemTime(strOmCommandArray[1]);
			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";							
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� �����Դϴ�";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��¥�ð����� %s]", strOmResult));

	}
	else 
	if (strOmCommandArray[0] == "OM_���ۺ�й�ȣ����") //OM_���ۺ�й�ȣ����||123456
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���ۺ�й�ȣ���� %s]", strOmCommandArray[1]));
		if (strOmCommandArray.GetSize() >= 2)
		{
			strTemp = "";
			strTemp = strOmCommandArray[1] + SPACE6;
			strTemp = strTemp.Left(6);
			memcpy(m_pProfile->TRANS.OpPassWord, strTemp.GetBuffer(0), strTemp.GetLength()); 
			m_pProfile->PutTransProfile();
			m_pProfile->GetTransProfile();
			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";							
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� �����Դϴ�";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���ۺ�й�ȣ���� %s]", strOmResult));

	}
	else 
	if (strOmCommandArray[0] == "OM_�ŷ���������") //OM_�ŷ���������||���񽺰ŷ�����(1,2,3)||��������ȸ����(0,1)||�ִ�ݾ׼�������(30,50,70)||�ִ�ݾ׼���Ÿ��(30,50,70)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OM_�ŷ��������� %s %s %s %s %s %s %s]", strOmCommandArray[1], strOmCommandArray[2], strOmCommandArray[3], strOmCommandArray[4], strOmCommandArray[5], strOmCommandArray[6], strOmCommandArray[7])); //#N0278
		if (strOmCommandArray.GetSize() >= 8)
		{
			m_pProfile->TRANS.TranService = 0;
//			m_pProfile->TRANS.TranService = Asc2Int(strOmCommandArray[1]);
			m_pProfile->TRANS.TranInqFee = Asc2Int(strOmCommandArray[2]);
			m_pProfile->TRANS.JaMaxDispAmt = Asc2Int(strOmCommandArray[3]);	
			m_pProfile->TRANS.TaMaxDispAmt = Asc2Int(strOmCommandArray[4]);	
		
			if(m_pProfile->DEVICE.EarHandler != Asc2Int(strOmCommandArray[5]))
			{
				m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT4; //#0091
			}
			m_pProfile->DEVICE.Proximity = Asc2Int(strOmCommandArray[5]);			//  �����ATM ����
			m_pProfile->DEVICE.EarHandler = Asc2Int(strOmCommandArray[5]);
			m_pProfile->DEVICE.SightHandicapped = Asc2Int(strOmCommandArray[5]);
			m_pProfile->PutDeviceProfile();											//  �����ATM ����
			m_pProfile->GetDeviceProfile();
			m_pProfile->PutTransProfile();
			m_pProfile->GetTransProfile();

			IniSetStr("D:\\INI\\TRANS.INI", "TRANS", "foreginBrand", strOmCommandArray[6]);
			IniSetStr("D:\\INI\\TRANS.INI", "TRANS", "viRobotCtrl", strOmCommandArray[7]);

			strOmResult = OM_NORMAL;
			strOmResult += "||���� ����";							
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� ����";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OM_�ŷ��������� %s]", strOmResult));

	}
	else 
	if (strOmCommandArray[0] == "OM_�ڵ��⸮����") //OM_�ڵ��⸮����
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OM_�ڵ��⸮���� %s]", strOmCommandArray[0]));
		m_pDevCmn->AtmStatus = ATM_DOWN;
		//#N0278
		m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT3;
		strOmResult = OM_NORMAL;
		strOmResult += "||���� ����";	
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ڵ��⸮���� %s]", strOmResult));

	}
	else 
	if (strOmCommandArray[0] == "OM_�����ýð�����") //OM_�����ýð�����||000000
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ÿ��Ͻð����� %s %s]", strOmCommandArray[1], strOmCommandArray[2]));
		if (strOmCommandArray.GetSize() >= 3)
		{
			strTemp = "";
			strTemp = strOmCommandArray[1] + SPACE6;
			strTemp = strTemp.Left(6);
			memcpy(m_pProfile->TRANS.RebootHHMMSS, strTemp.GetBuffer(0), strTemp.GetLength()); 
			m_pProfile->PutTransProfile();
			m_pProfile->GetTransProfile();

			char	szStartRebootTime[256];
			char	szEndRebootTime[256];
			memset(szStartRebootTime, 0x00, sizeof(szStartRebootTime));
			memset(szEndRebootTime,   0x00, sizeof(szEndRebootTime));
			memcpy(szStartRebootTime,  m_pProfile->TRANS.RebootHHMMSS, 6);
			memcpy(szEndRebootTime,    m_pProfile->TRANS.RebootHHMMSS, 6);
			AddString(szEndRebootTime, "010000", 6);

			if(memcmp(m_pProfile->TRANS.RebootHHMMSS, ZERO6, 6) != 0)
			{
				RegSetInt(_REGKEY_ATMRESTART, "UseRestart"    , 1);
				RegSetStr(_REGKEY_ATMRESTART, "CheckStartTime", szStartRebootTime);
				RegSetStr(_REGKEY_ATMRESTART, "CheckEndTime"  , szEndRebootTime);
				RegSetInt(_REGKEY_ATMRESTART, "ReadyRestart"  , 0);
			}

			m_pDevCmn->AtmRestartInfo.bUseRestart		= RegGetInt(_REGKEY_ATMRESTART, "UseRestart", 1);
			m_pDevCmn->AtmRestartInfo.strCheckStartTime= RegGetStr(_REGKEY_ATMRESTART, "CheckStartTime", "040000");
			m_pDevCmn->AtmRestartInfo.strCheckEndTime	= RegGetStr(_REGKEY_ATMRESTART, "CheckEndTime", "050000");
			m_pDevCmn->AtmRestartInfo.bReadyRestart	= RegGetInt(_REGKEY_ATMRESTART, "ReadyRestart", 0);
			
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation: CheckStartTime[%s] ~ CheckEndTime[%s]", szStartRebootTime, szEndRebootTime);


			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";							
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� �����Դϴ�";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ýð����� %s]", strOmResult));

	}
	else
	if (strOmCommandArray[0] == "OM_ALARMONOFF") //OM_ALARMONOFF||ON/OFF
	{
		if (strOmCommandArray.GetSize() >= 2)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ġ %s]", strOmCommandArray[1]));
			int nGetIni = 0;
			nGetIni = IniGetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19");

		if( strOmCommandArray[1] == "ON")
			{
				if(nGetIni == 1)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_ON, CLKEY_ON, CLKEY_OFF);
				}
				else
				if(nGetIni == 2)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_ON, CLKEY_OFF, CLKEY_ON);
				}
				else
				if(nGetIni == 3)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_ON, CLKEY_ON, CLKEY_ON);
				}
				else
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_ON, CLKEY_OFF, CLKEY_OFF);

				m_pDevCmn->fnSPACK_AlarmOnOff(FALSE);
			}
			else
			{
				if(nGetIni == 1)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_ON, CLKEY_OFF);
				}
				else
				if(nGetIni == 2)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_ON);
				}
				else
				if(nGetIni == 3)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_ON, CLKEY_ON);
				}
				else
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_OFF);

	
				m_pDevCmn->fnSPACK_AlarmOnOff(TRUE);
			}

			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";	

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� ����!";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ġ %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_PBM_SETUP") //OM_PBM_SETUP|OM_PBM_SETUP=������(1 : ���, 0 : �̻��)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[����μ��� %s]", strOmCommandArray[1]));

		if (strOmCommandArray.GetSize() >= 2)
		{
			m_pProfile->DEVICE.PBHandler = Asc2Int(strOmCommandArray[1]);
			m_pProfile->PutDeviceProfile();
			strOmResult = OM_NORMAL;
			strOmResult += "||[���� ����]";				
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT4;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||�Է� �����Դϴ�";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[����μ��� %s]", strOmResult));
	}
	else														
	{
		strOmResult = OM_ERROR;
	}
/////////////////////////////////////////////////////////////////////////////
	ClerkProcInCnt = 1;											// ��ȣ�������ó��
	return TRUE;
}





CStringArray strAdapterId;	
int CWinAtmCtrl::AtmGetAdapterId()
{
	// The following code is designed for Windows 2K.
	// If you want it to work on Windows NT, 
	//   you should read HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\NetworkCards

	HKEY hKey, hSubKey, hNdiIntKey;
	
	
	strAdapterId.RemoveAll();									// Clear

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"System\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}",
		0,
		KEY_READ,
		&hKey) != ERROR_SUCCESS)
		return FALSE;											// Register Open Failed
	
	DWORD			dwIndex		= 0;
	DWORD			dwBufSize	= 256;
	DWORD			dwDataType;
	char			szSubKey[256];
	unsigned char	szData[256];

	memset(szData, 0x00, sizeof(szData));					
	
	while(RegEnumKeyEx(hKey, dwIndex++, szSubKey, &dwBufSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		if(RegOpenKeyEx(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
		{		
			if(RegOpenKeyEx(hSubKey, "Ndi\\Interfaces", 0, KEY_READ, &hNdiIntKey) == ERROR_SUCCESS)
			{
				dwBufSize = 256;
				if(RegQueryValueEx(hNdiIntKey, "LowerRange", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
				{
					if(strcmp((char*)szData, "ethernet") == 0)	// Identify netcard
					{
						dwBufSize = 256;
						if(RegQueryValueEx(hSubKey, "DriverDesc", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
						{
							dwBufSize = 256;
							if(RegQueryValueEx(hSubKey, "NetCfgInstanceID", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
								strAdapterId.Add((LPCTSTR)szData);
						}
					}
				}
				RegCloseKey(hNdiIntKey);
			}
			RegCloseKey(hSubKey);
		}
		dwBufSize = 256;
	}	/* end of while */
	
	RegCloseKey(hKey);

	if(!strAdapterId.GetSize())									// No Netcard
		return FALSE;
	else
		return TRUE;
}

int CWinAtmCtrl::AtmSetEnableDHCP(int nFlag)
{
	BOOL	bRet = TRUE;

	CString	strIpAddress("192.168.1.10");
	CString strSubnetMask("255.255.255.0");
	CString strGateway("192.168.1.1");

MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "AtmSetEnableDHCP(%d)", nFlag);

	if(nFlag == FALSE)
	{
		ShellExecute( NULL , NULL , "netsh.exe", "interface ip set address \"���� ���� ����\" static " + strIpAddress + " " + strSubnetMask + " " + strGateway + " 1", NULL, SW_SHOW);			
	}
	else
	if(nFlag == TRUE)
	{
		ShellExecute( NULL , NULL , "netsh.exe", "interface ip set address \"���� ���� ����\" dhcp", NULL, SW_SHOW);
	}
	
	
	return bRet;
}




