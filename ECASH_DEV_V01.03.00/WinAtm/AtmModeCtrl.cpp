/////////////////////////////////////////////////////////////////////////////
// AtmModeCtrl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
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
//	Atm Status
/////////////////////////////////////////////////////////////////////////////
//	Mode :	ATM_INIT			: ����ʱ�ȭ	���
//			ATM_ERROR			: ��ֹ߻�		���
//			ATM_CLERK			: ���			���
//			ATM_READY			: �غ�			���
//			ATM_CUSTOM			: �������	��� 
//			ATM_TRAN			: ���ŷ���	���
/////////////////////////////////////////////////////////////////////////////
// �ڵ���������
int CWinAtmCtrl::AtmModeCtrl()
{
	HWND hAviWnd;												// ��������
	if (m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", AtmStatusName[m_pDevCmn->AtmStatus]);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatusSave[%s]", AtmStatusName[m_pDevCmn->AtmStatusSave]);
	}


	// SCR EVENT�� ���� �ٸ� ����üũ�� �ռ� �ŷ��� ���� �����ϰ� �Ѵ�
	if ((m_pDevCmn->AtmStatus     == ATM_CUSTOM)	&&			//�ӵ�����
		(m_pDevCmn->AtmStatusSave == ATM_CUSTOM)	&&
		(m_pTranCmn->TranStartByScr == DEV_SCR)		)
	{
		m_pDevCmn->AtmStatus = ATM_TRAN;
		m_pDevCmn->DeviceTran = DEV_SCR;
		m_pTranCmn->TranStartByScr = NULL;
	}

	switch (m_pDevCmn->AtmStatus) 
	{
		case ATM_INIT:											// ����ʱ�ȭ ���
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_INIT");
			m_pDevCmn->AtmStatusSave = ATM_INIT;				// �������º���
			m_pDevCmn->fnAPL_CshRetactCntInfo(0);				// CDU Retract Count Reset

			m_pDevCmn->fnAPL_CheckPowerOffError(); 				// �Ŀ�������ְ���
			m_pDevCmn->RecoveryType = 0;						// 0:�ڵ�����,1:��������
			m_pDevCmn->DeviceStatus = m_pDevCmn->fnAPL_GetDefineDevice(DEV_MAIN);	
																// ��ü��ġ����
			m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetErrorDevice(m_pDevCmn->DeviceStatus), TRUE);
			
			m_pDevCmn->fnAPL_CheckMaterial();					// ��ü�ܷ��������ܷ���ֹ߻�
			if (!m_pDevCmn->fnAPL_CheckError())					// ��ְ���
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// ��ֹ߻� ���
			else 
				m_pDevCmn->AtmStatus = ATM_CLERK;				// ��� ���

			// ���� ������ poll ������ ������. 
			m_pDevCmn->fnAPL_SetOnTransaction(OFF);

			break;
		
		case ATM_ERROR: 										// ��ֹ߻� ���
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(!m_pDevCmn->AtmStatusReEntryFlag))				// Atm����������Flag
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_ERROR");
				m_pDevCmn->AtmStatusReEntryFlag = TRUE;			// �����Ա���
				m_pDevCmn->fnAPL_UnUsed();						// �������
				m_pDevCmn->fnAPL_DisplayError(ATM_ERROR, TRUE);	// ���ȭ����¹�����
				if (m_pDevCmn->OpenKey)							// ����ø� ó��
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s] DeviceStatus[%x]", "ATM_ERROR", m_pDevCmn->DeviceStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s] AvailErrorDevice[%x]", "ATM_ERROR", m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus));

					if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) & (DEV_BRM | DEV_UCM))
					{
						// V02.05.03 2004.08.16 BRM/UCM�� ��������̶��ϴ��� �ۼ�����ֽ� �ڵ�����
						if ((memcmp(&m_pDevCmn->FirstErrBuff.ErrorCode[0], "77", 2) == 0)		||	// �۽����
							(memcmp(&m_pDevCmn->FirstErrBuff.ErrorCode[0], "88", 2) == 0)		)	// �������
						{
							m_pDevCmn->fnAPL_PrintJnlInfo("[�ڵ�����]-[�ۼ������]", JNL_STATUS);
							m_pDevCmn->fnAPL_ClearError();		// ��������ʱ�ȭ
							m_pDevCmn->RecoveryType = 0;		// 0:�ڵ�����,1:��������
							m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus));
																// �ڵ�����
						}
						else
						if (m_pTranCmn->TranProc == TRAN_DEP)	
						{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�Աݰŷ� ���[%s]", &m_pDevCmn->FirstErrBuff.ErrorCode[0]);
						}
						else 
						{
							m_pDevCmn->fnAPL_PrintJnlInfo("[�ڵ�����]-[BRM �ʱ�ȭ]", JNL_STATUS);
							m_pDevCmn->fnAPL_ClearError();		// ��������ʱ�ȭ
							m_pDevCmn->RecoveryType = 0;		// 0:�ڵ�����,1:��������
							m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus));
						}
//						else ;									// �ڵ����� ���� ����	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FirstErrorCode[%s]", &m_pDevCmn->FirstErrBuff.ErrorCode[0]);
					}
					else
					if (memcmp(&m_pDevCmn->FirstErrBuff.ErrorCode[0], "46999", 2) == 0)
					{											// V02-06-01-#10 46999(����� ������ ��ǥ�̰���)��ֽ�
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[�ڵ������Ұ�] ==================================================");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[�ڵ������Ұ�] ��ǥ�� �����̻�(46999) ��� : ������ ������ ����");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[�ڵ������Ұ�] ==================================================");
					}
					else										// BRM,UCM����(�������)
					{
						m_pDevCmn->fnAPL_PrintJnlInfo("[�ڵ�����] - [�ʱ�ȭ]", JNL_STATUS);
						m_pDevCmn->fnAPL_ClearError();			// ��������ʱ�ȭ
						m_pDevCmn->RecoveryType = 0;			// 0:�ڵ�����,1:�������� //#0127
						m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) & (~(DEV_BRM | DEV_UCM | DEV_SPR)));
																// �ڵ�����
					}
				}
			}
			m_pDevCmn->AtmStatusSave = ATM_ERROR;				// �������º���
			m_pDevCmn->fnAPL_AutoOffErrorDevice();				// ��ֳ���ġ�ڵ�����
			m_pDevCmn->fnAPL_CheckMaterial();					// ��ü�ܷ��������ܷ���ֹ߻�
			if (!m_pDevCmn->fnAPL_CheckError())					// ��ְ���
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// ��ֹ߻� ���
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm��밡��
				(m_pDevCmn->BrmChangeLoc == CHANGE_POS)		&&	// BrmŻ������
				(m_pDevCmn->BrmLoc == NORMAL_POS)			&&	// Brm����ġ
				(!m_pDevCmn->fnAPL_AutoResetBrm()))				// Brm�ڵ�����
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm��밡��
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)		&&	// ȸ��������ġ����
				(!m_pDevCmn->fnAPL_AutoReadyBrm()))				// Brm�ڵ��غ�
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->ForceDownFlag)						// ����Down���� 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->RemoteDownFlag)						// ����Down���� 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			{
//				m_pDevCmn->OpenKey = TRUE;						// ���弳��
				m_pDevCmn->AtmStatus = ATM_CLERK;				// ��� ���
			}

			if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)	// ���°����Ͻ���ȿ
				m_pDevCmn->fnAPL_DisplayError(ATM_ERROR, FALSE);	// ���ȭ����¹�����
			break;

		case ATM_CLERK: 										// ��� ���
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(!m_pDevCmn->AtmStatusReEntryFlag))				// Atm����������Flag
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_CLERK");
				m_pDevCmn->AtmStatusReEntryFlag = TRUE;			// �����Ա���
				m_pDevCmn->fnAPL_UnUsed();						// �������
				m_pDevCmn->fnAPL_DisplayNormalSegment();		// ����Segment���
				m_pDevCmn->fnAPL_DisplayError(ATM_CLERK, TRUE);	// �������ȭ����¹�����
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%d %d %d]", m_pDevCmn->ForceDownFlag, m_pDevCmn->RemoteDownFlag, m_pDevCmn->OpenKey);
//				m_pDevCmn->fnAPL_CloseHost();		 			// ȣ��Ʈ��
			}
			m_pDevCmn->AtmStatusSave = ATM_CLERK;				// �������º���

			m_pDevCmn->fnAPL_CheckMaterial();					// ��ü�ܷ��������ܷ���ֹ߻�
			if (!m_pDevCmn->fnAPL_CheckError())					// ��ְ���
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// ��ֹ߻� ���
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))				&&	// Brm��밡��
				(m_pDevCmn->BrmChangeLoc == CHANGE_POS)					&&	// BrmŻ������
				(m_pDevCmn->BrmLoc == NORMAL_POS)						&&	// Brm����ġ
				(!m_pDevCmn->fnAPL_AutoResetBrm()))							// Brm�ڵ�����
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm��밡��
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)		&&	// ȸ��������ġ����
				(!m_pDevCmn->fnAPL_AutoReadyBrm()))				// Brm�ڵ��غ�
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->ForceDownFlag)						// ����Down���� 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->RemoteDownFlag)						// ����Down���� 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->fnAPL_BRMCstMatchInfo())				// U3100K - CST ���� MissMatch -> //#0068
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (!m_pDevCmn->fnAPL_SprAutoload())				// ��ǥ Autoload ����
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->OpenKey)							// �Ű			
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else 
				m_pDevCmn->AtmStatus = ATM_READY;				// �غ� ���

			if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)	// ���°����Ͻ���ȿ
				m_pDevCmn->fnAPL_DisplayError(ATM_CLERK, FALSE);	// �������ȭ����¹�����

			if( (m_pTranCmn->DoorStatus != m_pDevCmn->DoorStatus) ||
				(m_pTranCmn->CabinetStatus != m_pDevCmn->CabinetStatus))	
			{
				if (m_pDevCmn->HostStatus == HOST_ONLINE)
				{
					try
					{
						m_pTranCmn->TranCode2 = TRANID_A020;			
						m_pTranCmn->fnAPP_SendHost();
					}
					catch (CTranCmnExit& e)
					{ 
						e.TranExitResult = e.TranExitResult;
					}
					m_pTranCmn->DoorStatus    = m_pDevCmn->DoorStatus;
					m_pTranCmn->CabinetStatus = m_pDevCmn->CabinetStatus;
				}
			}
			break;

		case ATM_READY:											// �غ� ���
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(!m_pDevCmn->AtmStatusReEntryFlag))				// Atm����������Flag
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_READY");
				m_pDevCmn->AtmStatusReEntryFlag = TRUE;			// �����Ա���
				m_pDevCmn->fnAPL_UnUsed();						// �������
				m_pDevCmn->fnAPL_DisplayNormalSegment();		// ����Segment���
				m_pDevCmn->fnAPL_DisplayError(ATM_READY, TRUE);	// �������ȭ����¹�����
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%d %d %d]", m_pDevCmn->ForceDownFlag, m_pDevCmn->RemoteDownFlag, m_pDevCmn->OpenKey);

			}
			m_pDevCmn->AtmStatusSave = ATM_READY;				// �������º���
			
			m_pDevCmn->fnAPL_CheckMaterial();					// ��ü�ܷ��������ܷ���ֹ߻�
		
			if (!m_pDevCmn->fnAPL_CheckError())					// ��ְ���
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// ��ֹ߻� ���
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))				&&	// Brm��밡��
				(m_pDevCmn->BrmChangeLoc == CHANGE_POS)					&&	// BrmŻ������
				(m_pDevCmn->BrmLoc == NORMAL_POS)						&&	// Brm����ġ
				(!m_pDevCmn->fnAPL_AutoResetBrm()))							// Brm�ڵ�����
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm��밡��
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)		&&	// ȸ��������ġ����
				(m_pDevCmn->BrmLoc == NORMAL_POS)			&&	// Brm����ġ : V07-05-01-#01:������� #26
				(!m_pDevCmn->fnAPL_AutoReadyBrm()))				// Brm�ڵ��غ�
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->ForceDownFlag)						// ����Down���� 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->RemoteDownFlag)						// ����Down���� 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->fnAPL_BRMCstMatchInfo())				// U3100K - CST ���� MissMatch -> //#0068
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (!m_pDevCmn->fnAPL_SprAutoload())				// ��ǥ Autoload ����
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->OpenKey)							// �Ű			
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if ((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL) ||	// ��ǥCover����
				(m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)	 ||	// ����Cover����
//				(m_pDevCmn->A4pHeadStatus != HEADER_NORMAL)	 ||	// ����Cover����
//				(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)	 ||	// ��ǥ����
				(m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)	 ||	// ���λ���
				(m_pDevCmn->BrmLoc != NORMAL_POS)			 ||	// Brm����ġ����
				(m_pDevCmn->RejectCSTLoc != NORMAL_POS)		 ||	// ȸ��������ġ����
				(m_pDevCmn->UcmLoc != NORMAL_POS)			 ||	// Ucm����ġ����
//				(m_pDevCmn->DoorStatus == DOOR_OPENED)		 ||	// CabinetStatus
				(m_pDevCmn->CabinetStatus == DOOR_OPENED))		// DoorStatus����
				m_pDevCmn->AtmStatus = ATM_READY;
// 			else
// 			if ((m_pDevCmn->fnA4P_GetDeviceStatus() != NORMAL)	||	// A4���� 
// 				(m_pDevCmn->fnA4P_GetPaperStatus())				)	// A4��������
// 				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pDevCmn->DoorStatus == DOOR_OPENED)	// Capture camera
			{
				m_pDevCmn->fnAPL_CaptureFaceCustomDoorOpen();   
				m_pDevCmn->AtmStatus = ATM_READY;
			}
			else
			if (!m_pDevCmn->fnAPL_OpenLine())					// ȣ��Ʈ���ΰ���
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostLine())				// ȣ��Ʈ���ΰ���
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostOpen())						// ȣ��Ʈ��������
				m_pDevCmn->AtmStatus = ATM_READY;			
//			else
//			if (!OpenProc())									// ȣ��Ʈ��������
//				m_pDevCmn->AtmStatus = ATM_READY;

			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM)) &&
				(!m_pDevCmn->fnAPL_AutoRefillBrm(m_pDevCmn->fnCSH_GetAutoloadCondition())))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_BRMCstSetInfo())				// U3100K - 5���� ��ױ� -> #0070 -> #N0145
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->fnAPL_BRMCstSetInfo_Empty_On())		// U3100K - �߰����� -> #N0145
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else 
			{
				m_pDevCmn->fnAPL_CaptureFaceCustomDoorOpen(TRUE); 
				m_pDevCmn->AtmStatus = ATM_CUSTOM;				// ������� ���
			}

			if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)	// ���°����Ͻ���ȿ			
				m_pDevCmn->fnAPL_DisplayError(ATM_READY, FALSE);	// �������ȭ����¹�����
			break;

		case ATM_CUSTOM:										// ������� ���
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(!m_pDevCmn->AtmStatusReEntryFlag))				// Atm����������Flag
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_CUSTOM");
				m_pDevCmn->AtmStatusReEntryFlag = TRUE;			// �����Ա���
				m_pDevCmn->TranStatus = TRAN_IDLE;              //#0113

				hAviWnd = _FIND_CFMON;
				if (m_pProfile->DEVICE.MachineType == U3100K)
				{
					if ( hAviWnd )
						::PostMessage(hAviWnd, WUM_DUALCFDISP, 0, 0);	//end

					if (hAviWnd)
						::PostMessage(hAviWnd, WUM_SOUND_ON, 0, 0);	   //#0011					

				}
				else
				if (m_pProfile->DEVICE.MachineType == U8100)
				{					
					if (hAviWnd)
					{
						if (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET)
							::PostMessage(hAviWnd, WUM_DUALCFDISP, 0, 0);	// WParam = 0 : SHOW, 1: HIDE
						else
							::PostMessage(hAviWnd, WUM_PLAY_OFF, 0, 0);
					}
				}

				//  �����ATM Start
				// ����ȭ�� Display�� Sensor(EarPhone) Clear �ʼ�
				m_pDevCmn->EarPhoneInserted = FALSE;
				m_pDevCmn->fnSNS_ClearPhoneStatus();
				m_pDevCmn->fnSCR_SetDisplayData(EAR_KINDMODE, "off");

				if (m_pDevCmn->AtmDefine.Proximity == PROXIMITY_ON)
				{
					m_pDevCmn->ProximitySensor = FALSE;			// �������� �ʱ�ȭ
					m_pDevCmn->m_PSoundTime = ::GetTickCount();	// �������� �ð� �ʱ�ȭ
				}
				//  �����ATM End

				m_pDevCmn->fnAPL_BRMCstSetInfo();			    // U3100K - 5���� ��ױ� -> #0070
				m_pDevCmn->fnAPL_BRMCstSetInfo_Empty_On();		// U3100K -> �߰����� ��ְ��� -> #N0156
				m_pDevCmn->fnAPL_DisplayInformationLed(TRUE);	// ����Led���
				m_pDevCmn->fnAPL_DisplayInformationIndicator(TRUE);	
																// ����Indicator���
				m_pDevCmn->fnAPL_SetOnTransaction(OFF);			// ���ŷ��߾ƴ�
				m_pDevCmn->fnAPL_SetProcCount('0');				// �����ȣ����
				m_pDevCmn->fnSCR_SetCurrentLangMode(KOR_MODE);	// �������弳��
				m_pTranCmn->fnAPP_MainMenuDisplay(TRUE);		// ����ȭ�����
				m_pTranCmn->TranStartByScr = NULL;				// �ӵ����� �ʱ�ȭ
				m_pDevCmn->fnAPL_DisplayInformationIndicator(TRUE);	// ������ 1�� ���� (U8100-AP���� #13) - ����LCD�� ������ Update�ϱ� ���� ���⿡�� ó��
				m_pDevCmn->fnAPL_CheckMaterial(DEV_BRM | DEV_UCM);	// ��ü�ܷ��������ܷ���ֹ߻�
																// 82160 ��ְ��Ҵ�å - CUSTOM MODE������ �����ѹ���Ȯ��
			}
			m_pDevCmn->AtmStatusSave = ATM_CUSTOM;				// �������º���

//			m_pDevCmn->fnAPL_CheckMaterial(DEV_BRM | DEV_UCM);	// ��ü�ܷ��������ܷ���ֹ߻�
																// 82160 ��ְ��Ҵ�å - CUSTOM MODE������ �����ѹ���Ȯ��
			if ((m_pDevCmn->AtmStatus     == ATM_CUSTOM)	&&	// ���泻�� #06 - �ӵ����� �ٷ� �ŷ����� Ÿ���� Return
				(m_pDevCmn->AtmStatusSave == ATM_CUSTOM)	&&
				(m_pTranCmn->TranStartByScr == DEV_SCR)		)
				return TRUE;
			else
			if (!m_pDevCmn->fnAPL_CheckError())					// ��ְ���
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// ��ֹ߻� ���
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm��밡��
				(m_pDevCmn->BrmChangeLoc == CHANGE_POS)		&&	// BrmŻ������
				(m_pDevCmn->BrmLoc == NORMAL_POS)			&&	// Brm����ġ
				(!m_pDevCmn->fnAPL_AutoResetBrm()))				// Brm�ڵ�����
					m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm��밡��
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)		&&	// ȸ��������ġ����
				(!m_pDevCmn->fnAPL_AutoReadyBrm()))				// Brm�ڵ��غ�
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->fnAPL_SprAutoload())				// ��ǥ Autoload ����
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->ForceDownFlag)						// ����Down���� 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (m_pDevCmn->RemoteDownFlag)						// ����Down���� 
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if (!m_pDevCmn->OpenKey)							// �Ű			
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if ((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL) ||	// ��ǥCover����
				(m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)	 ||	// ����Cover����
//				(m_pDevCmn->A4pHeadStatus != HEADER_NORMAL)	 ||	// ����Cover����
//				(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)	 ||	// ��ǥ����
				(m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)	 ||	// ���λ���
				(m_pDevCmn->BrmLoc != NORMAL_POS)			 ||	// Brm����ġ����
				(m_pDevCmn->RejectCSTLoc != NORMAL_POS)		 ||	// ȸ��������ġ����
				(m_pDevCmn->UcmLoc != NORMAL_POS)			 ||	// Ucm����ġ����
				(m_pDevCmn->CabinetStatus == DOOR_OPENED)	 ||	// CabinetStatus
				(m_pDevCmn->DoorStatus == DOOR_OPENED))			// DoorStatus����
				m_pDevCmn->AtmStatus = ATM_READY;
// 			else
// 			if ((m_pDevCmn->fnA4P_GetDeviceStatus() != NORMAL)	||	// A4���� 
// 				(m_pDevCmn->fnA4P_GetPaperStatus())				)	// A4��������
// 				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostLine())				// ȣ��Ʈ���ΰ���
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostOpen())				// ȣ��Ʈ��������
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pTranCmn->fnAPP_MainMenuDeviceEvent() != T_OK)// Eventüũ
				m_pDevCmn->AtmStatus = ATM_CUSTOM;
			else
			{
				if (m_pDevCmn->DeviceTran & DEV_PBM)			// �������� ���� ó������ ����
				{
					if (m_pDevCmn->fnPBM_GetDeviceStatus() == DOWN)					// 97500 ��ִ�å
					{
						m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PBM, DISABLE, TRUE);   // 97500��ֹ߻���Ŵ
						m_pDevCmn->DeviceStatus |= DEV_PBM;
						m_pDevCmn->AtmStatus = ATM_ERROR;                           // PBM_DOWN
					}
					else
						m_pDevCmn->AtmStatus = ATM_TRAN;		// ���ŷ���
				}
				else
					m_pDevCmn->AtmStatus = ATM_TRAN;			// ���ŷ���
			}

			// �����ATM Start
			if ((m_pProfile->DEVICE.MachineType == U8100) &&
				(m_pDevCmn->AtmDefine.Proximity == PROXIMITY_ON))
			{
				if (::GetTickCount() > m_pDevCmn->m_PSoundTime)
				{
					if (!m_pDevCmn->fnSNS_GetProximity())
					{
						if (m_pDevCmn->ProximitySensor == FALSE)// �ȳ��� ������ ���� ��츸
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�������� ����");
																// �����ð���ŭ
							m_pDevCmn->m_PSoundTime = ::GetTickCount() + 13 * 1000;
							m_pDevCmn->ProximitySensor = TRUE;	// �ȳ��� ������ ����
							m_pDevCmn->fnSND_KeySound(_SOUND_PROXIMITY);
						}
					}
					else
						m_pDevCmn->ProximitySensor = FALSE;
				}
				else											// �ȳ����� ����� ������������ ����� �Ҹ���
				{
					if (m_pDevCmn->fnSNS_GetProximity())
					{
						if (m_pDevCmn->ProximitySensor == TRUE)
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�������� ����");

							m_pDevCmn->ProximitySensor = FALSE;
							m_pDevCmn->m_PSoundTime = ::GetTickCount();
							m_pDevCmn->fnSND_KeySound(_SOUND_NONE);
						}
					}
				}
			}

			// �����ATM End
			if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)	// ���°����Ͻ���ȿ
			{
				m_pTranCmn->fnAPP_MainMenuDisplay(FALSE);		// ����ȭ�����
				m_pTranCmn->fnAPP_RemoteProc();
			}
			break;

		case ATM_TRAN:											// ���ŷ���

			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(!m_pDevCmn->AtmStatusReEntryFlag))				// Atm����������Flag
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", "ATM_TRAN");


				hAviWnd = _FIND_CFMON;
				if (m_pProfile->DEVICE.MachineType == U3100K)
				{
					if (hAviWnd)
						::PostMessage(hAviWnd, WUM_PLAY_ON, 1, 0);

					if (hAviWnd)
						::PostMessage(hAviWnd, WUM_SOUND_OFF, 0, 0); //#0011		
				}
				else
				if (m_pProfile->DEVICE.MachineType == U8100)
				{
					if ( hAviWnd )
						::PostMessage(hAviWnd, WUM_DUALCFDISP, 1, 0);

				}

				if ((m_pDevCmn->ProximitySensor)	&&						// JHD �������� #N0251
					(m_pDevCmn->DeviceTran != DEV_SCR))						//  �����ATM
					m_pDevCmn->fnSND_KeySound(_SOUND_NONE);
				
				m_pDevCmn->AtmStatusReEntryFlag = TRUE;			// �����Ա���
				m_pDevCmn->TranType = 1;						// 0:���ŷ���,1:�������(��ּ۽Ű���)

				// ������ �ŷ��� Flash�� ���� ���� �� ���� �ʱ�ȭ
				if (m_pProfile->DEVICE.MachineType == U8100)	//  �����ATM
				{					
					if (m_pProfile->DEVICE.EarHandler)
						m_pDevCmn->EarPhoneInserted = m_pDevCmn->fnSNS_GetEarPhoneStatus();
					
					if (m_pDevCmn->EarPhoneInserted)
						m_pDevCmn->fnSCR_SetDisplayData(EAR_KINDMODE, "on");
					else
					{
						m_pDevCmn->fnSNS_ClearPhoneStatus();
						m_pDevCmn->fnSCR_SetDisplayData(EAR_KINDMODE, "off");
					}
				}
				
				m_pDevCmn->fnAPL_SetTranStart();				// �ŷ����ۼ��� : 2003.11.21(�ӵ�����:���ġ-200ms)
				TranProc(TRAN_TRAN);							// �ŷ�ó��
				
				if (m_pDevCmn->EarPhoneInserted == TRUE)		//  �����ATM
				{
					m_pDevCmn->EarPhoneInserted = FALSE;
					m_pDevCmn->fnSNS_ClearPhoneStatus();
					m_pDevCmn->fnSCR_SetDisplayData(EAR_KINDMODE, "off");
				}
				
				if (m_pDevCmn->SightHandiCapped)				// �����ATM
					m_pDevCmn->SightHandiCapped = FALSE;

				
				m_pTranCmn->TranStartByScr = NULL;				// U8100-AP����
				m_pDevCmn->fnAPL_SetOnTransaction(OFF);			// ���ŷ��߾ƴ�
				m_pDevCmn->fnSCR_SetCurrentLangMode(KOR_MODE);	// �������弳��
				m_pDevCmn->fnAPL_SetSensorInfo();				// ������������
			}
			m_pDevCmn->AtmStatusSave = ATM_TRAN;				// �������º���

			m_pDevCmn->fnAPL_DisplayInformationLedOnOff(SPLLED_DOING_TRAN, FALSE);
			m_pDevCmn->fnAPL_CheckMaterial();					// ��ü�ܷ��������ܷ���ֹ߻�
			if (!m_pDevCmn->fnAPL_CheckError())					// ��ְ���
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if((m_pDevCmn->m_nAtmsCommand == TRAN_MSG_EXCEPTION_798) ||
			   (m_pDevCmn->m_nAtmsCommand == TRAN_MSG_EXCEPTION_094) ||
			   (m_pDevCmn->m_nAtmsCommand == TRAN_MSG_EXCEPTION_845))
			{
				m_pDevCmn->HostFirstOpenFlag = FALSE;
				m_pDevCmn->HostStatus        = HOST_WAITONLINE;
				m_pDevCmn->AtmStatus         = ATM_READY;
			}
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;				// ��ֹ߻� ���
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm��밡��
				(m_pDevCmn->BrmChangeLoc == CHANGE_POS)		&&	// BrmŻ������
				(m_pDevCmn->BrmLoc == NORMAL_POS)			&&	// Brm����ġ
				(!m_pDevCmn->fnAPL_AutoResetBrm()))				// Brm�ڵ�����
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&	// Brm��밡��
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)		&&	// ȸ��������ġ����
				(m_pDevCmn->BrmLoc == NORMAL_POS)			&&	// Brm����ġ 
				(!m_pDevCmn->fnAPL_AutoReadyBrm()))				// Brm�ڵ��غ�
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->OpenKey)							// �Ű			
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))	&&  // V02-06-02-#04 AutoLoad�������� �Ǵ�
				(m_pDevCmn->fnSPR_GetAutoloadCondition() != NORMAL))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL) ||	// ��ǥCover����
				(m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)	 ||	// ����Cover����
//				(m_pDevCmn->A4pHeadStatus != HEADER_NORMAL)	 ||	// ����Cover����
//				(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)	 ||	// ��ǥ����
				(m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)	 ||	// ���λ���
				(m_pDevCmn->BrmLoc != NORMAL_POS)			 ||	// Brm����ġ����
				(m_pDevCmn->RejectCSTLoc != NORMAL_POS)		 ||	// ȸ��������ġ����
				(m_pDevCmn->UcmLoc != NORMAL_POS)			 ||	// Ucm����ġ����
				(m_pDevCmn->CabinetStatus == DOOR_OPENED)	 ||	// CabinetStatus
				(m_pDevCmn->DoorStatus == DOOR_OPENED))			// DoorStatus����
				m_pDevCmn->AtmStatus = ATM_READY;
// 			else
// 			if ((m_pDevCmn->fnA4P_GetDeviceStatus() != NORMAL)	||	// A4���� 2007-05-17 Add
// 				(m_pDevCmn->fnA4P_GetPaperStatus())				)	// A4��������
// 				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostLine())				// ȣ��Ʈ���ΰ���
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostOpen())				// ȣ��Ʈ��������
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))	&&
				(m_pDevCmn->fnCSH_GetAutoloadCondition() != NORMAL))
				m_pDevCmn->AtmStatus = ATM_READY;
			else 
			{
				m_pDevCmn->AtmStatus = ATM_CUSTOM;
			}
			break;
		case ATM_DOWN:											// Down Mode
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", AtmStatusName[m_pDevCmn->AtmStatus]);

			m_pDevCmn->AtmStatusSave = ATM_DOWN;
			break;
		default:
			if (m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)
			{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%x], AtmStatusSave[%x]", m_pDevCmn->AtmStatus, m_pDevCmn->AtmStatusSave);	// test!!!!!
//HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[AtmModeCtrl()]", (char *)&m_pDevCmn->AtmDefine, (sizeof(DEVICEINFO) + (sizeof(int) * 80)));
				m_pDevCmn->AtmStatusSave = m_pDevCmn->AtmStatus;
			}
			break;
	}




/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmStatusSave == ATM_CUSTOM)	&&
		(m_pTranCmn->TranStartByScr == DEV_SCR)		)
		return TRUE;


	if (m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)
	{
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
			m_pDevCmn->PBMInkStatus = m_pDevCmn->fnPBM_GetInkStatus(); 

		if (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET)		
			m_pDevCmn->fnPIN_EntryDisable();
	}


	if (m_pDevCmn->AtmStatusSave == ATM_CUSTOM)
	{
		if (m_pDevCmn->AtmStatus == ATM_TRAN)					// ���ǵ������
			;	//return TRUE;									// ������ִ��� #COM07-03
		else
		{
			m_pDevCmn->fnAPL_CaptureSaveEnd();					// �Կ�����&����ó��
		}
	}

	if (m_pDevCmn->AtmStatusSave == ATM_TRAN)
	{
		if (m_pDevCmn->AtmStatus == ATM_CUSTOM)					// ���ǵ������
			;	//return TRUE;									// ������ִ��� #COM07-03
		else
		{
			m_pDevCmn->fnAPL_CaptureSaveEnd();					// �Կ�����&����ó��
		}
	}

	
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->Om.SendRecvStatus = "OFF";						// �ۼ���������
	m_pDevCmn->Om.OperationStatus = "ON";						// ���۰���
	m_pDevCmn->fnAPL_SetSensorInfo();							// ������������
	m_pDevCmn->fnAPL_GetAvailTrans();							// �ŷ����ɻ��±��ϱ�
//////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT)	||
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT1) ||
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT2) ||
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT3) ||
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT4) ||
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT5) ||		
		(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT6))
	{
		if ((m_pDevCmn->AtmStatus == ATM_READY) || (m_pDevCmn->AtmStatus == ATM_CUSTOM) || (m_pDevCmn->AtmStatus == ATM_DOWN) || (m_pDevCmn->AtmStatus == ATM_ERROR))
		{
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT)		//���� (����)
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ - ����]"));
				m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (����) POWER OFF]"));			
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(TRUE);
			}
			else
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT1)		//���� (����)
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ - �ٿ�ε� ��ġ]"));
				m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (����) POWER OFF]"));				
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(TRUE);
			}
			else
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT2)  		//������
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ - ��� �ڵ�����]"));
				m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (����) POWER OFF]"));			
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(TRUE);
			}
			else
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT3)  	
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ - ��ư ����]"));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (����) POWER OFF]"));			
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(FALSE);
			}
			else
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT4)  	
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ - ��� ����]"));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (����) POWER OFF]"));			
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(FALSE);
			}
			else													
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT5)  	
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ - AMS ���� ������]"));		
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(FALSE);
			}
			else
			if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT6)  	
			{
				//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[������ - AMS ���� �Ŀ�����]"));
				m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (����) POWER OFF]"));
				m_pDevCmn->m_nAtmsDoing = FALSE;
				m_pDevCmn->fnAPL_UnUsed();								
				m_pDevCmn->fnAPL_DisplayNormalSegment();			
				RestartProcess(TRUE);
			}

			return TRUE;
		}
	}

	ReOpenUploadProc();
//////////////////////////////////////////////////////////////////////////	
	if (!m_pDevCmn->fnAPL_CheckPowerStatus())					
	{
		//m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[STAND BY SW ON]"));			
//		m_pDevCmn->PowerOffSendFlag = TRUE;				// PowerOff�۽�ó��
		TranProc(TRAN_STATUS);							// Nmsó��	
		m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (����) POWER OFF]"));			
		TerminateProcess();										
		return TRUE;
	}

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->AtmRestartInfo.bUseRestart)					
	{
		if ((m_pDevCmn->AtmStatus != ATM_READY)	&&
			(m_pDevCmn->AtmStatus != ATM_CUSTOM))
			;
		else
		{
			if ((GetTime().CompareNoCase(m_pDevCmn->AtmRestartInfo.strCheckStartTime.GetBuffer(0)) > 0)	&&
				(GetTime().CompareNoCase(m_pDevCmn->AtmRestartInfo.strCheckEndTime.GetBuffer(0)) < 0)	)
			{													
				if (m_pDevCmn->AtmRestartInfo.bReadyRestart)
				{
					;											
				}
				else
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SYSTEM RESTART[%s ~ %s]",
													m_pDevCmn->AtmRestartInfo.strCheckStartTime.GetBuffer(0),
													m_pDevCmn->AtmRestartInfo.strCheckEndTime.GetBuffer(0));

					RegSetInt(_REGKEY_DEVICE, "NET_RETRYRE_BOOT_CNT", 0);	
					m_pDevCmn->AtmRestartInfo.bReadyRestart = 1;
					RegSetInt(_REGKEY_ATMRESTART, "ReadyRestart", m_pDevCmn->AtmRestartInfo.bReadyRestart);
					
					m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RESTART (%s ~ %s)]", m_pDevCmn->AtmRestartInfo.strCheckStartTime.GetBuffer(0), m_pDevCmn->AtmRestartInfo.strCheckEndTime.GetBuffer(0)));
					m_pDevCmn->fnLDCtrl_PowerResetRpc(3);	
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[RPC      : (����) POWER OFF]"));				
					m_pDevCmn->m_nAtmsDoing = FALSE;
					m_pDevCmn->fnAPL_UnUsed();								
					m_pDevCmn->fnAPL_DisplayNormalSegment();			
					RestartProcess(TRUE);							
					return TRUE;
				}
			}
			else
			if (m_pDevCmn->AtmRestartInfo.bReadyRestart)		// NOT Check Time
			{
				m_pDevCmn->AtmRestartInfo.bReadyRestart = 0;
				RegSetInt(_REGKEY_ATMRESTART, "ReadyRestart", m_pDevCmn->AtmRestartInfo.bReadyRestart);
			}
		}

	}

/////////////////////////////////////////////////////////////////////////////
	if (!m_pDevCmn->AtmFirstDoingFlag)							// Atm���ʽ���Flag
	{
		m_pDevCmn->AtmFirstDoingFlag = TRUE;
		m_pTranCmn->fnAPP_NmsProc(TRUE);
        m_pDevCmn->fnAPL_ClerkInformation(TRUE);				// �������
        m_pDevCmn->fnAPL_DisplayInformationLed(TRUE);			// ����Led���
        m_pDevCmn->fnAPL_DisplayInformationIndicator(TRUE);		// ����Indicator���
        m_pDevCmn->fnAPL_SendInformationRpl(TRUE);				// ����Rpl����
        m_pDevCmn->fnAPL_SendInformationInform(TRUE);			// ����Inform����

	}
	else
	{
		OpenProc();
		m_pTranCmn->fnAPP_TranVirtualTranReadyProc();
		m_pTranCmn->fnAPP_TranVirtualStatusProc();				// �ŷ�����
		m_pDevCmn->fnAPL_ClerkInformation();					// �������
		m_pDevCmn->fnAPL_ClerkExecCheck();						// �������˻�
		m_pDevCmn->fnAPL_ClearDeviceExecOffInfo();				// ��ġ��Ұ������ʱ�ȭ
		m_pDevCmn->fnAPL_DisplayInformationLed();				// ����Led���
		m_pDevCmn->fnAPL_DisplayInformationIndicator();			// ����Indicator���
		m_pDevCmn->fnAPL_SendInformationRpl();					// ����Rpl����
		m_pDevCmn->fnAPL_SendInformationInform();				// ����Inform����

		m_pDevCmn->fnAPL_CaptureSaveEnd(FALSE);					// �Կ�����&����ó��

		if (!m_pDevCmn->fnAPL_CheckPrintDevicePosition())		// ��ġ����ġ����
			m_pDevCmn->AtmStatus = ATM_ERROR;
		if (!ClerkProc())										// ���ó��
			return FALSE;										// ����ó��
	}

	ULONG nWaitTime = TimerSet(1);
	while (!CheckTimer(nWaitTime))
	{
		if ((m_pDevCmn->AtmStatus == ATM_CUSTOM) || (m_pDevCmn->AtmStatus == ATM_TRAN))
		{
			
			if (m_pTranCmn->TranStartByScr == DEV_SCR)			// ȭ�� Eventüũ
				break;
			if (m_pDevCmn->AtmStatus == ATM_TRAN)
				break;
			if (m_pDevCmn->AtmStatusSave == ATM_TRAN)
				break;

			if (m_pTranCmn->fnAPP_MainMenuDeviceEvent() == T_OK)	// ī�峪 ���� Eventüũ
			{
				if (m_pDevCmn->DeviceTran & DEV_PBM)				// U8100 AP ���泻�� #18 - ���� ������� ��ֽ� �ŷ��� �����ϴ� ��찡 ����
				{
					if (m_pDevCmn->fnPBM_GetDeviceStatus() == DOWN)
					{												// 97500 ��ִ�å
						m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PBM, DISABLE, TRUE);
						// 97500��ֹ߻���Ŵ
						m_pDevCmn->DeviceStatus |= DEV_PBM;			// PBM ���ó��		// MWI ��å. V07-01-02-#0B
						m_pDevCmn->AtmStatus = ATM_ERROR;			// PBM_DOWN
					}
					else
						m_pDevCmn->AtmStatus = ATM_TRAN;			// ���ŷ���
				}
				else
					// ����������������������������������������������������������������
					m_pDevCmn->AtmStatus = ATM_TRAN;				// ���ŷ���
				break;
			}
		}
		else
		{
			if (m_pDevCmn->AtmStatusSave == ATM_TRAN)
				break;
		}
		Delay_Msg();
	}

	return TRUE;
}

// ����ó��
int	CWinAtmCtrl::TerminateProcess()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TerminateProcess()");
	CString strOffT("");
	m_pDevCmn->fnSCR_DisplayString(1, "������ ����Ͻ� �� �����ϴ�.");
	m_pDevCmn->fnSCR_DisplayString(3, "������ ���� ���Դϴ�");

	m_pDevCmn->fnSCR_DisplayScreen(701);						// �������ȭ��
	m_pDevCmn->fnAPL_UnUsed();									// ���Ұ�
	m_pDevCmn->fnAPL_ClerkInformation();						// �������ġ���¼���

	if(m_pDevCmn->m_nAtmsCommand != ATMS_CMD_REBOOT2 && m_pDevCmn->fnAPL_CheckHostLine())
	{
		m_pTranCmn->TranCode2 = TRANID_A010;
		m_pTranCmn->fnAPP_SendHost();
	}



	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[POWER OFF: UPS BATTRY]"));
	strOffT.Format("%8.8s%6.6s", GetDate(), GetTime());
	RegSetStr(_REGKEY_DEVICE, "POWER_OFF_TIME", strOffT);		

	ULONG  WiatTime = TimerSet(K_10_WAIT);						// OM_����10�ʴ��
	while (TRUE)
	{
		Delay_Msg();											// Msg & Task
		if (!ClerkProc())										// OM_����üũ
			break;
		if (CheckTimer(WiatTime))
			break;
	}

	if (m_pDevCmn->fnUPS_GetDeviceStatus() == NORMAL)			// �����	2004.02.25
	{
		switch (m_pProfile->DEVICE.MachineType)					// ������ ����ó�� 2005.02.02
		{
		case U8100:
		case U3100K:
			m_pDevCmn->fnAPL_PowerOff(POWER_OFF_TIME);			// ����Off(������ð�)
			break;
		default:
			m_pDevCmn->fnAPL_PowerOff(POWER_OFF_TIME);			// ����Off(������ð�)
			break;
		}

																// V03-01-01 POWER_DOWN_GOOD CHECK
		WiatTime = TimerSet(K_10_WAIT);							// POWEROFF 10�ʴ��
		while (TRUE)
		{
			Delay_Msg();										// Msg & Task

			if (m_pDevCmn->fnAPL_GetPowerStatus() & POWER_DOWN_GOOD)
				break;

			if (CheckTimer(WiatTime))
				break;
		}
	}

	m_pDevCmn->fnAPL_ClerkInitModeSet();						// ����ʱ�ȭ��弳��
	KillTimer(TIMER_mSEC1000);									// TIMER_mSEC1000 ����
	m_pDevCmn->fnNET_Deinitialize(LINE_OPCL_TIME);				// ��������
	m_pDevCmn->AtmDoing = FALSE;								// Atm���࿩��
	m_pDevCmn->fnAPL_UnLoadDevice(TRUE);						// ��ġ������	
	WindowsShutdownProcess(TRUE);								// ������������ó��

	WinExec("C:\\T1atm\\App\\T1Clear.exe", SW_SHOW);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TerminateProcess():return");
	return TRUE;
}

int	CWinAtmCtrl::RestartProcess(int nReStartFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RestartProcess()");
	CString strOffT("");
	BOOL	bRebootFlag = FALSE;

	m_pDevCmn->fnSCR_DisplayString(1, "������ ����Ͻ� �� �����ϴ�.");
	m_pDevCmn->fnSCR_DisplayString(3, "������ ���� ���Դϴ�");
	m_pDevCmn->fnSCR_DisplayScreen(701);						// �������ȭ��
	m_pDevCmn->fnAPL_UnUsed();									// ���Ұ�
	m_pDevCmn->fnAPL_ClerkInformation();						// �������ġ���¼���


	if(m_pDevCmn->m_nAtmsCommand != ATMS_CMD_REBOOT2  && m_pDevCmn->fnAPL_CheckHostLine())
	{
		m_pTranCmn->TranCode2 = TRANID_A010;
		m_pTranCmn->fnAPP_SendHost();
	}
	//#0018 : K_10_WAIT -> K_3_WAIT 
	ULONG  WiatTime = TimerSet(K_3_WAIT);						// OM_����10�ʴ��
	while (TRUE)
	{
		Delay_Msg();											// Msg & Task
		if (!ClerkProc())										// OM_����üũ
			break;
		if (CheckTimer(WiatTime))
			break;
	}

	m_pDevCmn->fnAPL_ClerkInitModeSet();						// ����ʱ�ȭ��弳��
	KillTimer(TIMER_mSEC1000);									// TIMER_mSEC1000 ����
	m_pDevCmn->fnNET_Deinitialize(LINE_OPCL_TIME);				// ��������
	m_pDevCmn->fnAPL_UnLoadDevice(TRUE);						// ��ġ������
	m_pDevCmn->AtmDoing = FALSE;								// Atm���࿩��

	strOffT.Format("%8.8s%6.6s", GetDate(), GetTime());
	RegSetStr(_REGKEY_DEVICE, "POWER_OFF_TIME", strOffT);		
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[POWER_OFF_TIME : %s]", strOffT));	

	WindowsShutdownProcess(nReStartFlag);						// ������������ó��
			
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RestartProcess():return");
	return TRUE;
}

// ������������ó��
int	CWinAtmCtrl::WindowsShutdownProcess(int nReStartFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "WindowsShutdownProcess(nRebootFlag[%d])", nReStartFlag);

	HANDLE				hToken;
	LUID				Luid;
	BOOL				bReturn = FALSE;
	DWORD				dwError;
	TOKEN_PRIVILEGES	NewPrivileges;
	CString				strCMD("");


	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[POWER OFF : ����]"));
	RegSetInt(_REGKEY_ATMRESTART, "ATMRESTARTFlag", 0); //��������

	if (m_pProfile->DEVICE.MachineType == U8100 || m_pProfile->DEVICE.MachineType == U3100K)
	{
		if (nReStartFlag)
		{
			if(m_pDevCmn->fnLDCtrl_GetDeviceStatus() == NORMAL)
			{
				strCMD.Format("shutdown -s -f -t %d", 3);
				WinExecWaitEnd(strCMD, SW_HIDE);	
				return TRUE;
			}
			else
			{
				strCMD.Format("shutdown -r -f -t %d", 3);
				WinExecWaitEnd(strCMD, SW_HIDE);	
				return TRUE;
			}
		}
		else
		{
			strCMD.Format("shutdown -r -f -t %d", 3);
			WinExecWaitEnd(strCMD, SW_HIDE);	
			return TRUE;
		}
	}

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
	{
		dwError = GetLastError();
		if (dwError == ERROR_CALL_NOT_IMPLEMENTED) 
			return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &Luid)) 
	{
		CloseHandle(hToken);
		return FALSE;
	}

	NewPrivileges.PrivilegeCount			= 1;
	NewPrivileges.Privileges[0].Luid		= Luid;
	NewPrivileges.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED;

	bReturn = AdjustTokenPrivileges(hToken,
									FALSE,
									&NewPrivileges,
									0,
									NULL,
									NULL);

	CloseHandle(hToken);

	if (nReStartFlag)
	{
		if(m_pDevCmn->fnLDCtrl_GetDeviceStatus() == NORMAL)
		{
			::ExitWindowsEx(EWX_SHUTDOWN, (DWORD)-1);
		}
		else
		{
			::ExitWindowsEx(EWX_REBOOT, (DWORD)-1);
		}
	}
	else
		::ExitWindowsEx(EWX_SHUTDOWN, (DWORD)-1);

	return TRUE;
}

// ��ü�׽�Ʈ�غ�
int	CWinAtmCtrl::ReadyDevTest()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ReadyDevTest()");
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��� ��ü�׽�Ʈ]"));

	m_pDevCmn->fnAPL_ClearDeviceExecOffInfo(TRUE);				// ��ġ��Ұ������ʱ�ȭ
	
	m_pDevCmn->fnAPL_UnUsed();									// ���Ұ�

	m_pDevCmn->fnAPL_ClerkInitModeSet();						// ����ʱ�ȭ��弳��
	KillTimer(TIMER_mSEC1000);									// TIMER_mSEC1000 ����
	m_pDevCmn->fnNET_Deinitialize(LINE_OPCL_TIME);				// ��������
	m_pDevCmn->fnAPL_UnLoadDevice();							// ��ġ������
	m_pDevCmn->AtmDoing = FALSE;								// Atm���࿩��

	//#0075
	WinExecWaitEnd("C:\\T1ATM\\APP\\Main\\GOMA.EXE   ComMonitor4PNC.exe", SW_HIDE);
	if (m_pProfile->DEVICE.MachineType == U8500)		
	{
		WinExecWaitEnd("C:\\T1ATM\\APP\\Main\\GOMA.EXE NHCIM4HBRM.exe", SW_HIDE);
		WinExecWaitEnd("C:\\T1ATM\\APP\\Main\\GOMA.EXE NHCDM4HBRM.exe", SW_HIDE);
		WinExecWaitEnd("C:\\T1ATM\\APP\\Main\\GOMA.EXE ATMInside.exe", SW_HIDE); //#N0194
		
		WinExec("C:\\T1atm\\App\\Main\\ClearA.bat", SW_HIDE);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ReadyDevTest() -1");
		if (m_pDevCmn->BrmCashMode == BRM_REALCASH_MODE)
			WinExec("C:\\TM\\VDM\\vdm_ce.EXE", SW_SHOW);
		else
			WinExec("C:\\TM\\VDM\\vdm_ce.EXE", SW_SHOW);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ReadyDevTest() -2");
	}
	else
	if (m_pProfile->DEVICE.MachineType == U3100K)
		WinExec("C:\\TM\\VDM\\VDMT3.EXE", SW_SHOW);		    	// ����CE��
	else
		WinExec("C:\\TM\\VDM\\tmfa.EXE", SW_SHOW);				// ���ǰ����
	
	Delay_Msg(1000);
	WinExec("C:\\T1atm\\App\\T1Clear.exe", SW_HIDE);
	Delay_Msg(2000);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ReadyDevTest():return");
	return TRUE;
}




