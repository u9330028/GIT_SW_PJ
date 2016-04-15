/////////////////////////////////////////////////////////////////////////////
// TranNms.cpp: implementation of the CTranCmn class.
// Desription : Nms �ۼ��� ���� �Լ���
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
//	Nmsȣ���Լ�
/////////////////////////////////////////////////////////////////////////////
// Nmsó��
int CTranCmn::fnAPP_NmsProc(int InitFlg)								
{
	int		Cnt = 0;
	int		i = 0;
	int		RetStatus = T_OK;
	int		ErrorFlg = FALSE;									// ���Flg
	int		ErrorSendFlg = FALSE;								// ���SendFlg
	int		StatusFlg = FALSE;									// ����Flg
	static	CString	strReBootDate("");
	CString	strReBootTime("");
	CString	strReBootCode("");

	CString	szSaveTime("");
	CString strFileName, strTime;
	static	int nDvrflag = FALSE;


	if (InitFlg)		
	{
		strReBootDate = RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_REBOOT);

		ErrorSaveFlg = FALSE;									// �������SaveFlg
		ErrorInDirectFlg = FALSE;								// �������(��,�ڶ�,�ۼ��ŵ�)
		ErrorAutoOffSave = 0;									// �����ڵ�����

		RecycleBoxStatus = CST_NORMAL;							//01:Brm����ī��Ʈ����(����,�ܷ���,�亸��,�ܷ���)
		RejectCSTLoc = NORMAL_POS;								//02:Brmȸ���Ի���(����,�ܷ���,Ż��)
		RejectCSTStatus = NORMAL_POS;							//02:Brm����Ʈ�Ի���(����,�ܷ���,Ż��) - CST_EMPTY, CST_SET_NG, CST_NEAR, CST_FULL
		RetractCSTStatus = NORMAL_POS;							//02:Brmȸ���Ի���(����,�ܷ���,Ż��) - CST_EMPTY, CST_SET_NG, CST_NEAR, CST_FULL
		ClerkCSTLoc  = NORMAL_POS;								//03:Brm���ī��Ʈ����(����,Ż��)
																//04:ī��Ʈ����ġ����(Cd)
																//05:2ī��Ʈ����ġ����(Cd)
		FstCSTStatus = NORMAL_POS;									//06:ī��Ʈ����ġ����(Cd)
		SndCSTStatus = NORMAL_POS;									//07:ī��Ʈ����ġ����(Cd)
		TrdCSTStatus = NORMAL_POS;									//07:ī��Ʈ����ġ����(Cd)
		FthCSTStatus = NORMAL_POS;									//07:ī��Ʈ����ġ����(Cd)
																
		CheckCSTStatus = CST_NORMAL;							//08::Ucm��ݼ�ǥ�ܷ�����(����,�ܷ���,�亸��)
		CheckCSTStatusSensor = CST_NORMAL;						//09:��ݼ�ǥ����(�ǹ�)
		CheckCSTStatusReg = CST_NORMAL;							//10:��ݼ�ǥ����(���)
		CassetteStatus = CST_NORMAL;							//11:Ucm�Աݼ�ǥ�ܷ�����(����,�ܷ���)
		SlipHeadStatus = HEADER_NORMAL;							//12:��ǥCover����
																//13:SPR Cover����

		SlipStatus = SLIP_NORMAL;								//14:Spr�����ǥ�ܷ�����(����,�ܷ���,�亸��)
		JnlStatus = JNL_NORMAL;									//15:Jpr����1�ܷ�����(����,�ܷ���,�亸��)
		BrmLoc = NORMAL_POS	;									//16:Brm����ġ(����,Ż��)
																//17:Brm����ġ(����1)
																//18:Brm����ġ(����2)
		UcmLoc = NORMAL_POS;									//19:Ucm����ġ(����,Ż��)
		DoorStatus = DOOR_CLOSED;								//20:�޹�����(����,����)
		CabinetStatus = DOOR_CLOSED;							//20:��ܺλ���(����,����)
																//21:DES
																//22:��ǥȸ��
																//23:CARD ȸ��
		                                                        //24:��ǥȸ��
		                                                        //25:����ȸ��
		TmpTranAvail = TRAN_WITH_NOT;							//26:������ݻ���(����,�Ұ�)
		                                                        //27:�����Աݻ���(����,�Ұ�)
		                                                        //28:��ǥ��ݻ���(����,�Ұ�)
																//29:��ǥ�Աݻ���(����,�Ұ�)
																//30:����ŷ�����(����,�Ұ�)
																//31:����ȭ�����(����,�Ұ�)
																//32:�ؿܰŷ�����(����,�Ұ�)
																//33:�����ŷ�����(����,�Ұ�)
																//34:���ܼ��ŷ�����(����,�Ұ�)
																//35:���ڵ�ŷ�����(����,�Ұ�)
																//36:��ȣȭ�ŷ�����(����,�Ұ�)
		A4pStatus = A4P_NORMAL;									//37:�����ͻ���
		RejectCSTStatus = CST_NORMAL;							//38:����ȸ�����ǻ���
		CameraStatus    = NORMAL_POS;				// ī�޶����

		GSensorStatus   = NORMAL_POS;				// ��ݼ�������
		GSensorLelvel   = 0;						// ��ݼ�������
		GBatteryStatus  = NORMAL_POS;

		CheckPbmInkStaus = PBMINK_NORMAL;			 // ������ 1�� ���� (U8100-AP���� #13) - IPBM INK ���°��� �߰�

		GExternalSensorStatus1 = NORMAL_POS;            // �ܺμ���1
		GExternalSensorStatus2 = NORMAL_POS;            // �ܺμ���2

		TmpWithAvail = TRAN_WITH_NOT;
		TmpDepAvail  = TRAN_DEP_NOT;

		OpenKey = TRUE;
		CameraStatus = NORMAL_POS;
		ForceMissMatchDownFlag = FALSE;
		ForceSendStatus		   = STATUS_OFF_TIME;
		DeviceLoss = 0;

		memset(&NmsDataSendData, 0, sizeof(NmsDataSendData));	// ���/������������
		memset(&NmsSaveDataSendData, 0, sizeof(NmsSaveDataSendData));


		return T_OK;
	}

	if ((memcmp(m_pDevCmn->PBMInkStatus, PBM_INK_OUT, m_pDevCmn->PBMInkStatus.GetLength()) == 0)		||
		(memcmp(m_pDevCmn->PBMInkStatus, PBM_INK_NOTSET, m_pDevCmn->PBMInkStatus.GetLength()) == 0))
	{
		m_pDevCmn->CheckPbmInkStaus = PBMINK_OUT;
	}
	else
		m_pDevCmn->CheckPbmInkStaus = PBMINK_NORMAL;

  
/////////////////////////////////////////////////////////////////////////////
	fnAPP_RecvDataClear();
/////////////////////////////////////////////////////////////////////////////
if (!m_pDevCmn->fnAPL_CheckError())
{
MsgDump(MAC_TRACE_DUMP_FILE("log"), "NMS_PROC m_pDevCmn->AtmStatusSave[%d], m_pDevCmn->TranType[%d]", m_pDevCmn->AtmStatusSave, m_pDevCmn->TranType);

}


if(m_pDevCmn->AtmStatus !=m_pDevCmn->AtmStatusSave)
{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "m_pDevCmn->AtmStatusSave[%d], m_pDevCmn->AtmStatus[%d] ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d] NmsErrorSendFlg[%d]",
	m_pDevCmn->AtmStatusSave, m_pDevCmn->AtmStatus, ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);

}

	if(ForceSendStatus == STATUS_FORCE_SEND)
	{ 
		fnAPP_NmsUpdateStatus();								
		return TRAN_TXRX_STATUS;
	}

	
	if(m_pDevCmn->GAlarmStatus != GAlarmStatus || m_pDevCmn->GBatteryStatus != GBatteryStatus)
	{ 
		fnAPP_NmsUpdateStatus();								
		return TRAN_TXRX_STATUS;
	}

	if(m_pDevCmn->ForceMissMatchDownFlag != ForceMissMatchDownFlag)
	{ 
		fnAPP_NmsUpdateStatus();								
		return TRAN_TXRX_STATUS;
	}


	if ((m_pDevCmn->AtmStatusSave == ATM_TRAN || m_pDevCmn->AtmStatusSave == ATM_CUSTOM) &&				// �������ŷ��� (Van-CD ����, 2009.10.23 "ATM_CUSTOM �������")
		(!m_pDevCmn->fnAPL_CheckError())		&&				// ���üũ
		(m_pDevCmn->TranType == 1))								// �������(��ּ۽Ű���)
	{
		m_pDevCmn->TranType = 0;								// 0:���ŷ���,1:�������(��ּ۽Ű���)

		ErrorAutoOffSave = m_pDevCmn->DeviceAutoOff;			// �����ڵ�����
		ErrorFlg = TRUE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "1. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d], NmsErrorSendFlg[%d]",
	ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);
		
		if (NmsErrorSendFlg)
		{
			RetStatus = fnAPP_NmsUpdateStatus();	
			if (RetStatus == TRAN_TXRX_STATUS)
				return RetStatus;
			else 		
				return T_OK;
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "1-1. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d], NmsErrorSendFlg[%d]",
	ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);

	}
	else 
	if ((m_pDevCmn->AtmStatusSave == ATM_ERROR)	&&				// Cwc Send Check
		 (!m_pDevCmn->fnAPL_CheckError()) &&				// Error
		     (m_pDevCmn->TranType == 0))						// Customer Not Use
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "2. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d], NmsErrorSendFlg[%d]",
	ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);

		if (NmsErrorSendFlg)
		{
			RetStatus = fnAPP_NmsUpdateStatus();	
			if (RetStatus == TRAN_TXRX_STATUS)
				return RetStatus;
			else 		
				return T_OK;
		}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "2-1. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d], NmsErrorSendFlg[%d]",
	ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);
		ErrorSendFlg = TRUE;									// ���SendFlg(�⺻���۸��)

	}
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_ERROR) &&				// ������ֹ߻�(�ڵ�����ó����)
		(ErrorSaveFlg))											// ������������
	{
		ErrorSaveFlg = FALSE;									// ��������������
		ErrorSendFlg = TRUE;									// ���SendFlg(�⺻���۸��)
		if (ErrorInDirectFlg)									// ������ֽü۽�ó��
			;
		else
		if (ErrorAutoOffSave != m_pDevCmn->DeviceAutoOff)		// �ڵ����𰪺�ȭ�ü۽�ó��
			;
		else
		if ((TranProc == TRAN_WITH)	||							// ����ݰŷ���(������ðŷ�)
			(TranProc == TRAN_DEP)	)
		{
//			if ((m_pDevCmn->AtmStatus != ATM_ERROR)	&&			// �ڵ�����OK(�����ܷ��ô�NG)
//				(!CancelWithProcFlg)	&&						// �������ó����
//				(!CancelDepProcFlg)		)						// �Ա����ó����
//				ErrorSendFlg = FALSE;							// ��ּ۽ű���ó��
		}
		else													// �׿ܰŷ���
		{
			if (m_pDevCmn->AtmStatus != ATM_ERROR)				// �ڵ�����OK
				ErrorSendFlg = FALSE;							// ��ּ۽ű���ó��
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "3. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d]",
	ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag);

	}
	else
	if (m_pDevCmn->HostStatus == HOST_WAITONLINE)
	{
		return T_OK;
	}
	else
	if ((m_pDevCmn->AtmStatus == ATM_CUSTOM) &&
		(m_pDevCmn->AtmStatusSave == ATM_CUSTOM))
	{
		return T_OK;
	}
	else
	if ((m_pDevCmn->AtmStatus == ATM_TRAN) &&
		(m_pDevCmn->AtmStatusSave == ATM_TRAN))
	{
		return T_OK;
	}
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_CLERK) &&			// READY->����->READY
		(m_pDevCmn->AtmStatus     == ATM_READY))
 		return T_OK;													
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_READY)		&&			// READY->����->READY
		(m_pDevCmn->AtmStatus     == ATM_CUSTOM)	&&			// ������� CUSTOM
		(m_pDevCmn->HostOpenFlag)					)			// �������������۽��÷���
	{
		if ((BrmLoc != m_pDevCmn->BrmLoc)					            ||  // ����ġ
			(DoorStatus != m_pDevCmn->DoorStatus)						||  // Door Open�� ���¼۽�
			(CabinetStatus != m_pDevCmn->CabinetStatus)					||  // Cabinet Door Open�� ���¼۽�
			(OpenKey      != m_pDevCmn->OpenKey))                           // ������	
		{
			StatusFlg = TRUE;
		}
	}
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_CUSTOM) &&			// READY->����->READY
		(m_pDevCmn->AtmStatus     == ATM_READY)	 &&			// ������� CUSTOM
		(m_pDevCmn->HostOpenFlag)					)		// �������������۽��÷���
		;													// ������������ �۽�ó��
	else
	if (m_pDevCmn->PowerOffSendFlag)							// PowerOff����ó��
		;
	else
	if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)		// ���°����Ͻ���ȿ
	{
		if ((FstCSTStatus != m_pDevCmn->FstCSTStatus)					||  // Rb1 
		    (SndCSTStatus != m_pDevCmn->SndCSTStatus)					||  // Rb2 
		    (TrdCSTStatus != m_pDevCmn->TrdCSTStatus)					||  // Rb3 
		    (FthCSTStatus != m_pDevCmn->FthCSTStatus)					||  // Rb3 
			(SlipHeadStatus != m_pDevCmn->SlipHeadStatus)				||	// ��ǥcover����
			(SlipStatus != m_pDevCmn->SlipStatus)						||	// ��ǥ����
		    (RejectCSTStatus != m_pDevCmn->RejectCSTStatus)				||  // ����Ʈ�ڽ�
		    (RetractCSTStatus != m_pDevCmn->RetractCSTStatus))              // ȸ���ڽ�
		{
			StatusFlg = TRUE;

// 			if (SndCSTStatus != m_pDevCmn->SndCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szSndSTStatus[m_pDevCmn->SndCSTStatus]);
// 			}
// 			else	
// 			if (FstCSTStatus != m_pDevCmn->FstCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szFstCSTStatus[m_pDevCmn->FstCSTStatus]);
// 			}
// 			else	
// 			if (TrdCSTStatus != m_pDevCmn->TrdCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szTrdCSTStatus[m_pDevCmn->TrdCSTStatus]);
// 			}
// 			else	
// 			if (FthCSTStatus != m_pDevCmn->FthCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szFthCSTStatus[m_pDevCmn->FthCSTStatus]);
// 			}
// 			else
// 			if (RetractCSTStatus != m_pDevCmn->RetractCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szRetractCSTStatus[m_pDevCmn->RetractCSTStatus]);
// 			}
// 			else
// 			if (SlipStatus != m_pDevCmn->SlipStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szSlipStatus[m_pDevCmn->SlipStatus]);
// 			}
// 			else
// 			if (RejectCSTStatus != m_pDevCmn->RejectCSTStatus)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(szRejectCSTStatus[m_pDevCmn->RejectCSTStatus]);
// 			}
// 
// 			if (m_pDevCmn->OpenKey) 
// 				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "44. ErrorFlg");

		}
		else
			return T_OK;
	}
	else
	if (!m_pDevCmn->HostFirstOpenFlag)							// ���� ������ ������ ���¼۽Ź� : 2002.10.26
	{
		return T_OK;											
	}
	else
	if ((RecycleBoxStatus != m_pDevCmn->RecycleBoxStatus)			||	// ���ݻ��´ٸ�
		(CheckCSTStatus != m_pDevCmn->CheckCSTStatus)				||	// ��ݼ�ǥ���´ٸ�
		(CheckCSTStatusSensor != m_pDevCmn->CheckCSTStatusSensor)	||	// ��ݼ�ǥ���´ٸ�(�ǹ�)
		(CheckCSTStatusReg != m_pDevCmn->CheckCSTStatusReg)			||	// ��ݼ�ǥ���´ٸ�(���)
		(CassetteStatus != m_pDevCmn->CassetteStatus)				||	// �Աݼ�ǥ���´ٸ�
		(SlipHeadStatus != m_pDevCmn->SlipHeadStatus)				||	// ��ǥ Cover����
		(SlipStatus != m_pDevCmn->SlipStatus)						||	// ��ǥ����
		(JnlStatus != m_pDevCmn->JnlStatus)							||	// ���λ���
		(A4pStatus != m_pDevCmn->A4pStatus)							||	// A4�����ͻ���
		(RejectCSTStatus != m_pDevCmn->RejectCSTStatus)				||  // ����Ʈ�ڽ�
		(RetractCSTStatus != m_pDevCmn->RetractCSTStatus)			||  // ȸ���ڽ�
		(BrmLoc != m_pDevCmn->BrmLoc)								||  // ����ġ 
		(FstCSTStatus != m_pDevCmn->FstCSTStatus)					||  // Rb1 
		(SndCSTStatus != m_pDevCmn->SndCSTStatus)					||  // Rb2 
		(TrdCSTStatus != m_pDevCmn->TrdCSTStatus)					||  // Rb3 
		(FthCSTStatus != m_pDevCmn->FthCSTStatus)					||  // Rb4 //#N0134 
	    (TmpTranAvail != m_pDevCmn->TranAvail)			    		||	// �ŷ����� 
		(DoorStatus != m_pDevCmn->DoorStatus)						||  // Door Open�� ���¼۽�
		(CabinetStatus != m_pDevCmn->CabinetStatus)					||  // Cabinet Door Open�� ���¼۽�
		(CameraStatus != m_pDevCmn->CameraStatus)					||  // ī�޶���� ���¼۽�
		(OpenKey      != m_pDevCmn->OpenKey)                        ||  // ������	
		(m_pDevCmn->DeviceRetract))										// ȸ��������
	{
		StatusFlg = TRUE;

		if (m_pDevCmn->OpenKey)
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "45. ErrorFlg");

		if (NmsErrorSendFlg)
		{
			RetStatus = fnAPP_NmsUpdateStatus();	
			if (RetStatus == TRAN_TXRX_STATUS)
				return RetStatus;
			else 		
				return T_OK;
		}
	}
	else
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	if ((ErrorFlg)		||										// ��ֹ߻���
		(ErrorSendFlg)||										// ��������۽Ž�
		(StatusFlg)		||										// ���º���߻�
		(m_pDevCmn->HostOpenFlag)||								// �������������۽Ž�
		(m_pDevCmn->PowerOffSendFlag))							// PowerOff���������۽Ž�
		;
	else
		return T_OK;

	if (m_pDevCmn->OpenKey) 
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "4. ErrorFlg[%d], ErrorSendFlg[%d], StatusFlg[%d], m_pDevCmn->HostOpenFlag[%d] NmsErrorSendFlg[%d]", ErrorFlg, ErrorSendFlg, StatusFlg, m_pDevCmn->HostOpenFlag, NmsErrorSendFlg);

/////////////////////////////////////////////////////////////////////////////

	if (ErrorFlg)												// ��ֹ߻���
	{
		if (CancelWithProcFlg)									// ���������� �۽Ž�
		{
			;													// ���ó�� �ϵ��� �Ѵ�.
		}
		if (CancelDepProcFlg)									// �Ա�������� �۽Ž�
		{														// �۱��̳� �Աݽ� �������������
			;													// �۽��ؾ� �Ѵ�.
		}														

		ErrorInDirectFlg = FALSE;								// �������(��,�ڷ�,�ۼ��ŵ�)

		int	RetId = m_pDevCmn->fnAPL_GetErrorCodeKind(m_pDevCmn->CurErrBuff.ErrorCode);
																// 0:������,1:������,2:���̼�,3:����,4:�������
		if ((RetId == 2) || 									// ���̼�
			(RetId == 3) || 									// ����
			(RetId == 4))	 									// �������
			ErrorInDirectFlg = TRUE;						

//		ErrorSaveFlg = TRUE;									// ��������߻���
		fnAPP_NmsIncDeviceCnt();								// ��ġ���ī���� ����ó��
		fnAPP_NmsUpdateStatus();								// ���¹��� �ۼ�
		NmsErrorSendFlg = TRUE;									// ���Send��

		return TRAN_TXRX_ERROR;
	}
	else
	if (ErrorSendFlg)											// ��������۽Ž�
	{															// �ٹ߱����ִ�å : 2002.04.30
		fnAPP_NmsIncDeviceCnt();								// ��ġ���ī���� ����ó��
		fnAPP_NmsUpdateStatus();								// ���¹��� �ۼ�
		NmsErrorSendFlg = TRUE;									// ���Send��

		return TRAN_TXRX_ERROR;

	}
	else
	if (m_pDevCmn->HostOpenFlag)								// "��������" ����
	{
		m_pDevCmn->HostOpenFlag = FALSE;						// ������������ �۽��÷��� RESET
		fnAPP_NmsUpdateStatus();								// ���¹��� �ۼ�
		return TRAN_TXRX_STATUS;								
	}
	else
	if (StatusFlg)
	{
//		if (m_pDevCmn->DeviceRetract)							// ���¹�ó�� : ��üȸ��
//		{
//			m_pDevCmn->DeviceRetract = 0;
//			return T_OK;
//		}

//		if (JnlStatus != m_pDevCmn->JnlStatus)					// ���¹�ó�� : ���ο��� 
//		{
//			JnlStatus = m_pDevCmn->JnlStatus;
//			return T_OK;
//		}

//		if (RejectCSTStatus != m_pDevCmn->RejectCSTStatus)
//		{														// ���¹�ó�� : ȸ���Ի��� 
//			RejectCSTStatus = m_pDevCmn->RejectCSTStatus;
//			return T_OK;
//		}
		//#N0152
		RetStatus = fnAPP_NmsUpdateStatus();	
		if (RetStatus == TRAN_TXRX_STATUS)
			return RetStatus;
		else 		
			return T_OK;
	}
	else
	if (m_pDevCmn->PowerOffSendFlag)								// "��������" ����
	{
		m_pDevCmn->PowerOffSendFlag = FALSE;						// ������������ �۽��÷��� RESET

		fnAPP_NmsUpdateStatus();								// ���¹��� �ۼ�
		return TRAN_TXRX_STATUS;								
	}

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	�ŷ� �ۼ��� �Լ�
/////////////////////////////////////////////////////////////////////////////
// Send Data
int CTranCmn::fnAPP_SendData(BYTE* SendBuff, int Length, int SendSec, int RecvBuffClearFlg)
{
	BYTE	TempBuff[NETBUFFSIZE];
	int		TempLength;
	CString	szService("");

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendData[SendBuff]", SendBuff, Length);
		return T_OK;											// SEND OK
	}
/////////////////////////////////////////////////////////////////////////////
//	fnAPP_GetAtmSysInfo(ATM_CPUMEMINFO);
/////////////////////////////////////////////////////////////////////////////
	if(TranCode2 != TRANID_0006 && TranCode2 != TRANID_A020)
	{
		m_pDevCmn->Om.SendRecvStatus = "ON";						// �ۼ����߼���
		m_pDevCmn->fnAPL_ClerkInformation();						// �������
	}
/////////////////////////////////////////////////////////////////////////////

	if (RecvBuffClearFlg)
	{
		fnAPP_RecvDataClear();
	}

	memcpy(TempBuff, SendBuff, Length);							// 2002.08.28
	TempLength = Length;

	CString strSendData = MakeUnPack(TempBuff, TempLength);
	HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendData[SendBuff] UnpackData", 
		strSendData.GetBuffer(0), strSendData.GetLength());


//	fnAPP_SetPollTimeStart();

	if (m_pDevCmn->fnNET_SendData((LPCTSTR)strSendData, SendSec))
		return T_OK;
	else
		return T_TIMEOVER;
}

// Recv Data Clear
int CTranCmn::fnAPP_RecvDataClear()
{
	BYTE	ClearBuff[NETBUFFSIZE];
	int		ClearLength;

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
		return T_OK;											// RECV OK
/////////////////////////////////////////////////////////////////////////////
	while (fnAPP_RecvData(ClearBuff, &ClearLength, 0) != T_TIMEOVER)
		;
	return T_OK;
}
	
// Recv Data
int CTranCmn::fnAPP_RecvData(BYTE* RecvBuff, int* Length, int RecvSec)
{
	BYTE	TempBuff[NETBUFFSIZE];
	int		TempLength = 0;

	CString	strRecvData;
	strRecvData.Empty();

																// ����BP_Header ����Ÿ ����
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
		Delay_Msg(1000);										// 2003.12.21
		return T_OK;											// RECV OK
	}
/////////////////////////////////////////////////////////////////////////////

	int nRecvResult = 0;
	do
	{
		nRecvResult = m_pDevCmn->fnNET_RecvData(strRecvData, RecvSec);
		if (RecvSec)
			MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[m_pDevCmn->fnNET_RecvData : %d]", nRecvResult);

		if (RecvSec)
			HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[Unpacked Recv Data]", strRecvData.GetBuffer(0) , strRecvData.GetLength());
		TempLength = MakePack(strRecvData.GetBuffer(0), TempBuff, strRecvData.GetLength());
		if (RecvSec)
			HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[Packed Recv Data]", TempBuff , TempLength);

		if (nRecvResult == FALSE)
		{
			*Length = 0;
			return T_TIMEOVER;
		}

		if (!TempLength)
		{
			*Length = 0;
			return T_RECVERROR;
		}

		memcpy(RecvBuff, TempBuff, TempLength);
		*Length = TempLength;

if ((RecvSec)									&&
	(m_pDevCmn->BrmCashMode == BRM_TESTCASH_MODE))
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[RecvBuff]", RecvBuff, *Length);

		if ((*Length) > 0)
			return T_OK;
	} while(TRUE);

	return T_OK;
}

// Recv In Tbl
int CTranCmn::fnAPP_RecvInTbl(BYTE* Buff, int Length)
{
	int		NmsRecvInTblTemp;

	NmsRecvInTblTemp = NmsRecvInTbl;
	NmsRecvInTblTemp++;
	if (NmsRecvInTblTemp >= NMSRECVTBL)							// In Tbl Temp Adj.
		NmsRecvInTblTemp = 0;

	if (NmsRecvInTblTemp != NmsRecvOutTbl)						// Data Tbl Ok
	{
		memcpy(NmsRecvBufferTbl[NmsRecvInTbl], Buff, Length);
		NmsRecvLengthTbl[NmsRecvInTbl] = Length;

		NmsRecvInTbl = NmsRecvInTblTemp;						// In Tbl Adj.
	}

	return T_OK;
}

// Recv Out Tbl
int CTranCmn::fnAPP_RecvOutTbl(BYTE* Buff, int* Length)
{
	if (NmsRecvInTbl == NmsRecvOutTbl)							// No Data
	{
		*Length = 0;
		return T_TIMEOVER;
	}

	memcpy(Buff, NmsRecvBufferTbl[NmsRecvOutTbl], NmsRecvLengthTbl[NmsRecvOutTbl]);
	*Length = NmsRecvLengthTbl[NmsRecvOutTbl];

	NmsRecvOutTbl++;
	if (NmsRecvOutTbl >= NMSRECVTBL)							// Out Tbl Adj.
		NmsRecvOutTbl = 0;

	if (!(*Length))
		return T_TIMEOVER;

	return T_OK;
}

// ��� ����̽� ī���� ����
int CTranCmn::fnAPP_NmsIncDeviceCnt()
{
	int		DeviceBrmCnt = m_pProfile->TRANS.DeviceBrmCnt;	// �񱳿뺸�� : 2002.04.26
	int		DeviceUcmCnt = m_pProfile->TRANS.DeviceUcmCnt;
	int		DevicePbmCnt = m_pProfile->TRANS.DevicePbmCnt;
	int		DeviceExecOff = m_pProfile->TRANS.DeviceExecOff;

	if (ErrorInDirectFlg)									// �������(��,�ڶ�,�ۼ��ŵ�)
		;
	else
	{
//		if ((NmsSaveDataSendData.ErrorMTCCode[0] == '4') && 
//			((NmsSaveDataSendData.ErrorMTCCode[1] == '4') ||
//			 (NmsSaveDataSendData.ErrorMTCCode[1] == '5') ||
//			 (NmsSaveDataSendData.ErrorMTCCode[1] == '6') ))
//			DeviceUcmCnt++;
//		else
//		if ((NmsSaveDataSendData.ErrorMTCCode[0] == '4') ||
//			((NmsSaveDataSendData.ErrorMTCCode[0] == '5')))
//			DeviceBrmCnt++;
//		else
//		if (NmsSaveDataSendData.ErrorMTCCode[0] == '6')
//			DevicePbmCnt++;
	}

	if (DeviceBrmCnt >= 4)
	{
		DeviceBrmCnt = 4;
		DeviceExecOff |= DEV_BRM;
	}
	else
		DeviceExecOff &= ~DEV_BRM;

	if (DeviceUcmCnt >= 3)
	{
		DeviceUcmCnt = 3;
		DeviceExecOff |= DEV_UCM;
	}
	else
		DeviceExecOff &= ~DEV_UCM;

	if (DevicePbmCnt >= 3)
	{
		DevicePbmCnt = 3;
		DeviceExecOff |= DEV_PBM;
	}
	else
		DeviceExecOff &= ~DEV_PBM;

	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)					// HOST LOCAL MODE(���������) : 2002.08.20
	{
		DeviceBrmCnt = 0;
		DeviceUcmCnt = 0;
		DevicePbmCnt = 0;
		DeviceExecOff = 0;
	}

	if ((DeviceBrmCnt == m_pProfile->TRANS.DeviceBrmCnt)	&&	// ��������
		(DeviceUcmCnt == m_pProfile->TRANS.DeviceUcmCnt)	&&
		(DevicePbmCnt == m_pProfile->TRANS.DevicePbmCnt)	&&
		(DeviceExecOff == m_pProfile->TRANS.DeviceExecOff));
	else
	{
		m_pProfile->GetTransProfile();
		m_pProfile->TRANS.DeviceBrmCnt = DeviceBrmCnt;			// ���� 
		m_pProfile->TRANS.DeviceUcmCnt = DeviceUcmCnt;
		m_pProfile->TRANS.DevicePbmCnt = DevicePbmCnt;
		m_pProfile->TRANS.DeviceExecOff = DeviceExecOff;
		m_pProfile->PutTransProfile();
	}

	return T_OK;
}



// ���¹��� �ۼ�
int CTranCmn::fnAPP_NmsUpdateStatus(BYTE* StatusBuff)
{	
	static BYTE	szStatusBuff[51];			
	int nGetInt = 0;
	memset(szStatusBuff, ' ', sizeof(szStatusBuff));

	char	szDoorSt[][256]       = {
		                           "[  :  :  �޹� ����     ]",
		                           "[  :  :  �޹� ����     ]"  };

	char	szCabinetSt[][256]   = {
		                           "[  :  :  ��ܺ� ����    ]",
								   "[  :  :  ��ܺ� ����    ]"       };
	
	char	szCameraSt[][256]   = {
		                           "[  :  :  ����� ����    ]",
		                           "[  :  :  ����� ������  ]",
		                           "[  :  :  ����� ������  ]",
		                           "[  :  :  ����� ������  ]",
								   "[  :  :  ����� ������  ]"       };
	char	szFstCSTStatus[][256]   = {
		                           "[  :  :  1ī��Ʈ NORMAL     ]",
		                           "[  :  :  1ī��Ʈ CST_NEAR   ]",
		                           "[  :  :  1ī��Ʈ CST_EMPTY  ]",
		                           "[  :  :  1ī��Ʈ CST_SET_NG ]",
								   "[  :  :  1ī��Ʈ CST_FULL   ]"       };

	char	szSndSTStatus[][256]   = {
		                           "[  :  :  2ī��Ʈ NORMAL     ]",
		                           "[  :  :  2ī��Ʈ CST_NEAR   ]",
		                           "[  :  :  2ī��Ʈ CST_EMPTY  ]",
		                           "[  :  :  2ī��Ʈ CST_SET_NG ]",
								   "[  :  :  2ī��Ʈ CST_FULL   ]"       };

	char	szTrdCSTStatus[][256]   = {
		                           "[  :  :  3ī��Ʈ NORMAL     ]",
		                           "[  :  :  3ī��Ʈ CST_NEAR   ]",
		                           "[  :  :  3ī��Ʈ CST_EMPTY  ]",
		                           "[  :  :  3ī��Ʈ CST_SET_NG ]",
								   "[  :  :  3ī��Ʈ CST_FULL   ]"       };

	char	szFthCSTStatus[][256]   = {
		                           "[  :  :  4ī��Ʈ NORMAL     ]",
		                           "[  :  :  4ī��Ʈ CST_NEAR   ]",
		                           "[  :  :  4ī��Ʈ CST_EMPTY  ]",
		                           "[  :  :  4ī��Ʈ CST_SET_NG ]",
								   "[  :  :  4ī��Ʈ CST_FULL   ]"       };

	char	szRetractCSTStatus[][256]   = {
		                           "[  :  :  ȸ���� NORMAL     ]",
		                           "[  :  :  ȸ���� CST_NEAR   ]",
		                           "[  :  :  ȸ���� CST_EMPTY  ]",
		                           "[  :  :  ȸ���� CST_SET_NG ]",
								   "[  :  :  ȸ���� CST_FULL   ]"       };


	char	szRejectCSTStatus[][256]   = {
		                           "[  :  :  ����Ʈ NORMAL     ]",
		                           "[  :  :  ����Ʈ CST_NEAR   ]",
		                           "[  :  :  ����Ʈ CST_EMPTY  ]",
		                           "[  :  :  ����Ʈ CST_SET_NG ]",
								   "[  :  :  ����Ʈ CST_FULL   ]"       };

								   
	char	szBrmLocSt[][256]	   = {
		                          "[  :  :  ��ݺ� ����ġ ]",
								  "[  :  :  ��ݺ� ����   ]" };								   
								   
	char	szCashStatus[][80] = {
		                          "[  :  :  ���� ��ݺ� ����  ]",
								  "[  :  :  ���� ��ݺ� �ܷ���]",
								  "[  :  :  ���� ��ݺ� �亸��]",
								  "[  :  :  ���� ��ݺ� FULL  ]"  };

	char	szCheckStatus[][80] = {"[  :  :  ��ǥ ��ݺ� ����  ]",
								   "[  :  :  ��ǥ ��ݺ� �ܷ���]",
								   "[  :  :  ��ǥ ��ݺ� �亸��]",
								   "[  :  :  ��ǥ ��ݺ� FULL  ]"
								};
	char	szCheckDepStatus[][80] = {"[  :  :  ��ǥ �Աݺ� ����  ]",
									  "[  :  :  ��ǥ �Աݺ� �ܷ���]",
									  "[  :  :  ��ǥ �Աݺ� �亸��]",
									  "[  :  :  ��ǥ �Աݺ� FULL  ]"
									};

	char	szSlipHeadStatus[][80] = {"[  :  :  ��ǥ Cover ����  ]",
								  "[  :  :  ��ǥ Cover Open]"
									};

	char	szSlipStatus[][80] = {"[  :  :  ��ǥ ���� ����  ]",
								  "[  :  :  ��ǥ ���� �亸��]",
								  "[  :  :  ��ǥ ���� �ܷ���]"
								};
	char	szJnlStatus[][80] = {"[  :  :  ���� ���� ����  ]",
								 "[  :  :  ���� ���� �亸��]",
								 "[  :  :  ���� ���� �ܷ���]"
								};
	char	szA4pStatus[][80] = {"[  :  :  A4������ ���� ����  ]",
								 "[  :  :  A4������ ���� �亸��]",
								 "[  :  :  A4������ ���� �ܷ���]"
								};
	
	char	szGSensorStatus[][80] = {"[  :  :  GSonser ��� ����]",
								     "[  :  :  GSonser ��� ����]"
									};

	char	szOpMode[][80] = {
		                      "[  :  :  ������]",
							  "[  :  :  �ŷ����]"};

/////////////////////////////////////////////////////////////////////////////
	
	if(!OpenKey && (OpenKey == m_pDevCmn->OpenKey))
		return T_OK;

	Int2Asc( m_pDevCmn->RecycleBoxStatus                      ,&szStatusBuff[0] ,  1);
	Int2Asc( m_pDevCmn->RejectCSTLoc                          ,&szStatusBuff[1] ,  1);
	Int2Asc( m_pDevCmn->ClerkCSTLoc                           ,&szStatusBuff[2] ,  1);
	Int2Asc( m_pDevCmn->CheckCSTStatus                        ,&szStatusBuff[3] ,  1);
	Int2Asc( m_pDevCmn->CheckCSTStatusSensor                  ,&szStatusBuff[4] ,  1);
	Int2Asc( m_pDevCmn->CheckCSTStatusReg                     ,&szStatusBuff[5] ,  1);
	Int2Asc( m_pDevCmn->CassetteStatus                        ,&szStatusBuff[6] ,  1);
	Int2Asc( m_pDevCmn->SlipStatus                            ,&szStatusBuff[7] ,  1);
	Int2Asc( m_pDevCmn->JnlStatus                             ,&szStatusBuff[8] ,  1);
	Int2Asc( m_pDevCmn->FstCSTStatus						  ,&szStatusBuff[9] ,  1);
	Int2Asc( m_pDevCmn->SndCSTStatus		                  ,&szStatusBuff[10],  1);
	Int2Asc( m_pDevCmn->TrdCSTStatus		                  ,&szStatusBuff[11],  1);
	Int2Asc( m_pDevCmn->BrmLoc                                ,&szStatusBuff[12],  1);
	Int2Asc( m_pDevCmn->UcmLoc                                ,&szStatusBuff[13],  1);
	Int2Asc( m_pDevCmn->DoorStatus                            ,&szStatusBuff[14],  1);
	Int2Asc( m_pDevCmn->CabinetStatus                         ,&szStatusBuff[15],  1);
	Int2Asc( m_pDevCmn->TranAvail                             ,&szStatusBuff[16],  1);
	Int2Asc( m_pDevCmn->A4pStatus                             ,&szStatusBuff[17],  1);
	Int2Asc( m_pDevCmn->RejectCSTStatus                       ,&szStatusBuff[18],  1);
	Int2Asc( m_pDevCmn->RetractCSTStatus                      ,&szStatusBuff[19],  1);
	Int2Asc( m_pDevCmn->GAlarmStatus		                  ,&szStatusBuff[20],  1);
	Int2Asc( m_pDevCmn->GBatteryStatus		                  ,&szStatusBuff[21],  1);
	Int2Asc( m_pDevCmn->GExternalSensorStatus1	              ,&szStatusBuff[22],  1);
	Int2Asc( m_pDevCmn->GExternalSensorStatus2	              ,&szStatusBuff[23],  1);
	Int2Asc( m_pDevCmn->GSensorStatus		                  ,&szStatusBuff[24],  1);
	Int2Asc( m_pDevCmn->CheckPbmInkStaus	                  ,&szStatusBuff[25],  1);
	Int2Asc( m_pDevCmn->OpenKey                               ,&szStatusBuff[26],  1);
	Int2Asc( m_pDevCmn->CameraStatus                          ,&szStatusBuff[27],  1);
	Int2Asc( m_pDevCmn->ForceMissMatchDownFlag                ,&szStatusBuff[28],  1);
	Int2Asc( m_pDevCmn->FthCSTStatus		                  ,&szStatusBuff[29],  1);
	Int2Asc( m_pDevCmn->FthCSTStatus		                  ,&szStatusBuff[29],  1);
	Int2Asc( m_pDevCmn->TranAvail		                      ,&szStatusBuff[30],  1);
	Int2Asc( m_pDevCmn->SlipHeadStatus                        ,&szStatusBuff[31],  1);
//	Int2Asc( m_pDevCmn->DeviceLoss							  ,&szStatusBuff[32],  1);
	
	szStatusBuff[50] = 0x00;
	NmsSendBuffer[50] = 0x00;

	if(memcmp(NmsSendBuffer, szStatusBuff, sizeof(szStatusBuff)) == 0)
		return T_OK;

MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_NmsUpdateStatus NmsSendBuffer [%50.50s]",NmsSendBuffer);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "----------------------------------------------------------------------------------------");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecycleBoxStatus[%d][%d]",		RecycleBoxStatus,		m_pDevCmn->RecycleBoxStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RejectCSTLoc[%d][%d]",			RejectCSTLoc,			m_pDevCmn->RejectCSTLoc);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkCSTLoc[%d][%d]",			ClerkCSTLoc,			m_pDevCmn->ClerkCSTLoc);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CheckCSTStatus[%d][%d]",		CheckCSTStatus,			m_pDevCmn->CheckCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CheckCSTStatusSensor[%d][%d]",	CheckCSTStatusSensor,	m_pDevCmn->CheckCSTStatusSensor);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CheckCSTStatusReg[%d][%d]",	CheckCSTStatusReg,		m_pDevCmn->CheckCSTStatusReg);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CassetteStatus[%d][%d]",		CassetteStatus,			m_pDevCmn->CassetteStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SlipHeadStatus[%d][%d]",			SlipHeadStatus,				m_pDevCmn->SlipHeadStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SlipStatus[%d][%d]",			SlipStatus,				m_pDevCmn->SlipStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "JnlStatus[%d][%d]",			JnlStatus,				m_pDevCmn->JnlStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "BrmLoc[%d][%d]",				BrmLoc,					m_pDevCmn->BrmLoc);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "UcmLoc[%d][%d]",				UcmLoc,					m_pDevCmn->UcmLoc);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DoorStatus[%d][%d]",			DoorStatus,				m_pDevCmn->DoorStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CabinetStatus[%x][%x]",		CabinetStatus,			m_pDevCmn->CabinetStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TmpTranAvail[%x][%x]",			TmpTranAvail,			m_pDevCmn->TranAvail);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "A4pStatus[%d][%d]",			A4pStatus,				m_pDevCmn->A4pStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RejectCSTStatus[%d][%d]",		RejectCSTStatus,		m_pDevCmn->RejectCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RetractCSTStatus[%d][%d]",		RetractCSTStatus,		m_pDevCmn->RetractCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->DeviceRetract[%x]",	m_pDevCmn->DeviceRetract);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FstCSTStatus[%d][%d]",			FstCSTStatus,		m_pDevCmn->FstCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SndCSTStatus[%d][%d]",			SndCSTStatus,       m_pDevCmn->SndCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TrdCSTStatus[%d][%d]",			TrdCSTStatus,		m_pDevCmn->TrdCSTStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FthCSTStatus[%d][%d]",			FthCSTStatus,       m_pDevCmn->FthCSTStatus);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GExternalSensorStatus1[%d][%d]",	    	GExternalSensorStatus1,		    m_pDevCmn->GExternalSensorStatus1); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GExternalSensorStatus2[%d][%d]",	    	GExternalSensorStatus2,		    m_pDevCmn->GExternalSensorStatus2); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GSensorStatus[%d][%d]",	    	        GSensorStatus,		    m_pDevCmn->GSensorStatus);  
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CheckPbmInkStaus[%d][%d]",	    	        CheckPbmInkStaus,		m_pDevCmn->CheckPbmInkStaus); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OpenKey[%d][%d]",	    	                OpenKey,	 	        m_pDevCmn->OpenKey); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CameraStatus[%d][%d]",	    	            CameraStatus,	 	        m_pDevCmn->CameraStatus); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ForceMissMatchDownFlag[%d][%d]",	    	ForceMissMatchDownFlag,	 	        m_pDevCmn->ForceMissMatchDownFlag); 
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DeviceLoss[%d][%d]",	                	DeviceLoss,	 	        m_pDevCmn->DeviceLoss); 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "---------------------------------------------------------------------------------------");

	if (CameraStatus != m_pDevCmn->CameraStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szCameraSt[m_pDevCmn->CameraStatus]);
	}
	
	if (OpenKey != m_pDevCmn->OpenKey)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szOpMode[m_pDevCmn->OpenKey]);
	}
	else
	if (BrmLoc != m_pDevCmn->BrmLoc)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szBrmLocSt[m_pDevCmn->BrmLoc]);
	}
	else
	if (CameraStatus != m_pDevCmn->CameraStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szCameraSt[m_pDevCmn->CameraStatus]);
	}
	else	
	if (CabinetStatus != m_pDevCmn->CabinetStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szCabinetSt[m_pDevCmn->CabinetStatus]);
	}
	else	
	if (DoorStatus != m_pDevCmn->DoorStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szDoorSt[m_pDevCmn->DoorStatus]);
	}
	else	
	if (SndCSTStatus != m_pDevCmn->SndCSTStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szSndSTStatus[m_pDevCmn->SndCSTStatus]);
	}
	else	
	if (FstCSTStatus != m_pDevCmn->FstCSTStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szFstCSTStatus[m_pDevCmn->FstCSTStatus]);
	}
	else
	if (RetractCSTStatus != m_pDevCmn->RetractCSTStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szRetractCSTStatus[m_pDevCmn->RetractCSTStatus]);
	}
	else
	if (RejectCSTStatus != m_pDevCmn->RejectCSTStatus)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(szRejectCSTStatus[m_pDevCmn->RejectCSTStatus]);
	}
	else
	if (SlipHeadStatus != m_pDevCmn->SlipHeadStatus)
	{																// ��ǥ���´ٸ�
		m_pDevCmn->fnAPL_PrintJnlInfo(szSlipHeadStatus[m_pDevCmn->SlipHeadStatus]);
	}
	else
	if (SlipStatus != m_pDevCmn->SlipStatus)
	{																// ��ǥ���´ٸ�
		m_pDevCmn->fnAPL_PrintJnlInfo(szSlipStatus[m_pDevCmn->SlipStatus]);
	}
	else
	if (RecycleBoxStatus != m_pDevCmn->RecycleBoxStatus)
	{																// ���ݻ��´ٸ�
		m_pDevCmn->fnAPL_PrintJnlInfo(szCashStatus[m_pDevCmn->RecycleBoxStatus]);
	}
	else
	if (CheckCSTStatus != m_pDevCmn->CheckCSTStatus)
	{																// ��ݼ�ǥ���´ٸ�
		m_pDevCmn->fnAPL_PrintJnlInfo(szCheckStatus[m_pDevCmn->CheckCSTStatus]);
	}
	else
	if (CheckCSTStatusSensor != m_pDevCmn->CheckCSTStatusSensor)
	{																// ��ݼ�ǥ���´ٸ�(�ǹ�)
		m_pDevCmn->fnAPL_PrintJnlInfo(szCheckStatus[m_pDevCmn->CheckCSTStatusSensor]);
	}
	else
	if (CassetteStatus != m_pDevCmn->CassetteStatus)
	{																// �Աݼ�ǥ���´ٸ�
		m_pDevCmn->fnAPL_PrintJnlInfo(szCheckDepStatus[m_pDevCmn->CassetteStatus]);
	}
	else
	if (JnlStatus != m_pDevCmn->JnlStatus)
	{																// ���λ��´ٸ�
		m_pDevCmn->fnAPL_PrintJnlInfo(szSlipStatus[m_pDevCmn->SlipStatus]);
	}
	else
	if (A4pStatus != m_pDevCmn->A4pStatus)
	{																// A4�����ͻ��´ٸ�
		m_pDevCmn->fnAPL_PrintJnlInfo(szA4pStatus[m_pDevCmn->A4pStatus]);
	}
// 	else
// 	if (DeviceLoss != m_pDevCmn->DeviceLoss)
// 	{																
// 		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�̼��� : %s", (m_pDevCmn->DeviceLoss == DEV_BRM) ? "����" :
// 																(m_pDevCmn->DeviceLoss == DEV_MCU) ? "ī��" : 
// 		                                                        (m_pDevCmn->DeviceLoss == (DEV_BRM | DEV_MCU)) ? "����+ī��" : "��"));
// 	}


	RecycleBoxStatus     = 	m_pDevCmn->RecycleBoxStatus;
	RejectCSTLoc         =	m_pDevCmn->RejectCSTLoc;
	ClerkCSTLoc          =	m_pDevCmn->ClerkCSTLoc;
	CheckCSTStatus       =	m_pDevCmn->CheckCSTStatus;
	CheckCSTStatusSensor =	m_pDevCmn->CheckCSTStatusSensor;
	CheckCSTStatusReg    =	m_pDevCmn->CheckCSTStatusReg;
	CassetteStatus       =	m_pDevCmn->CassetteStatus;
	SlipHeadStatus       = 	m_pDevCmn->SlipHeadStatus;	
	SlipStatus           = 	m_pDevCmn->SlipStatus;
	JnlStatus            = 	m_pDevCmn->JnlStatus;
	FstCSTStatus		 = m_pDevCmn->FstCSTStatus;  // Rb1 
	SndCSTStatus		 = m_pDevCmn->SndCSTStatus;  // Rb2 
	TrdCSTStatus		 = m_pDevCmn->TrdCSTStatus;  // Rb3 	
	FthCSTStatus		 = m_pDevCmn->FthCSTStatus;  // Rb4 //#N0134	
	BrmLoc               =	m_pDevCmn->BrmLoc;	
	UcmLoc               =	m_pDevCmn->UcmLoc;	
	DoorStatus           =	m_pDevCmn->DoorStatus;	
	CabinetStatus        =	m_pDevCmn->CabinetStatus;	
	TmpTranAvail         =  m_pDevCmn->TranAvail;	
	A4pStatus            =  m_pDevCmn->A4pStatus;	
	RejectCSTStatus      =  m_pDevCmn->RejectCSTStatus;	
	RetractCSTStatus     =  m_pDevCmn->RetractCSTStatus;	
	GAlarmStatus		 = m_pDevCmn->GAlarmStatus;
	GBatteryStatus		 = m_pDevCmn->GBatteryStatus;
	GExternalSensorStatus1		 = m_pDevCmn->GExternalSensorStatus1;
	GExternalSensorStatus2		 = m_pDevCmn->GExternalSensorStatus2;
	GSensorStatus		 = m_pDevCmn->GSensorStatus;
	CheckPbmInkStaus	 = m_pDevCmn->CheckPbmInkStaus;		
	OpenKey              = m_pDevCmn->OpenKey;	
	CameraStatus         = m_pDevCmn->CameraStatus;
	ForceMissMatchDownFlag = m_pDevCmn->ForceMissMatchDownFlag;
	m_pDevCmn->DeviceRetract	= 0;						
//	m_pDevCmn->DeviceLoss	= m_pDevCmn->DeviceLoss;						

	memcpy(NmsSendBuffer, szStatusBuff, sizeof(szStatusBuff));
	return TRAN_TXRX_STATUS;
}

// �������� �ۼ��� ����/���� ���� Ȯ�� (�̻��)
int CTranCmn::fnAPP_NmsDistinctStatus()
{
	const int STATUS_SET = 1;
	const int STATUS_RESET = 2;

	int nStatus = 0;

	switch(RecycleBoxStatus)									// NmsDataSendData.CashEmpty : ��ݸ����ǽǹ�
	{
		case CST_NEAR			:								// �ܷ���/NEAR END
			switch(m_pDevCmn->RecycleBoxStatus)
			{
			case CST_NEAR		:								
				break;
			case CST_EMPTY		:								
			case CST_SET_NG		:
				nStatus = STATUS_SET;
				break;
			case CST_FULL		:								
			case CST_NORMAL		:								
			case RB_ALREADY_FULL:
			default				:
				nStatus = STATUS_RESET;
				break;
			}
			break;
		case CST_EMPTY		:									// �亸��
		case CST_SET_NG		:									// Ż��
			switch(m_pDevCmn->RecycleBoxStatus)
			{
			case CST_NEAR		:								
				nStatus = STATUS_RESET;
				break;
			case CST_EMPTY		:								
			case CST_SET_NG		:
				break;
			case CST_FULL		:								
			case CST_NORMAL		:								
			case RB_ALREADY_FULL:
			default				:
				nStatus = STATUS_RESET;
				break;
			}
			break;
		case CST_FULL		:									// FULL
		case CST_NORMAL		:									// ����
		case RB_ALREADY_FULL:									// ALREADY FULL
		default				:
			switch(m_pDevCmn->RecycleBoxStatus)
			{
			case CST_NEAR		:								
				nStatus = STATUS_SET;
				break;
			case CST_EMPTY		:								
			case CST_SET_NG		:
				nStatus = STATUS_SET;
				break;
			case CST_FULL		:								
			case CST_NORMAL		:								
			case RB_ALREADY_FULL:
			default				:
				break;
			}
			break;
	}
	if (nStatus)
		return nStatus;

	if (CheckCSTStatus == CST_EMPTY)							// NmsDataSendData.CheckEmpty : ��ǥ�ǹ�
	{
		if (m_pDevCmn->CheckCSTStatusSensor != CST_EMPTY)
			nStatus = STATUS_RESET;
	}
	else 
	if (CheckCSTStatus != CST_EMPTY)
	{
		if (m_pDevCmn->CheckCSTStatusSensor == CST_EMPTY)
			nStatus = STATUS_SET;
	}
	if (nStatus)
		return nStatus;

	switch(SlipStatus)											// NmsDataSendData.SlipEmpty : ��ǥ����
	{
		case SLIP_NORMAL		:								// ��������
			switch(m_pDevCmn->SlipStatus)
			{
				case SLIP_NORMAL		:						// ��������
					break;
				case SLIP_LOW_END		:						// ���� �ܷ�
					nStatus = STATUS_SET;
					break;
				case SLIP_EMPTY_PAPER	:						// ��������
				default					:						// ��������Ȯ�κҰ�
					nStatus = STATUS_SET;
					break;
			}
			break;
		case SLIP_LOW_END		:								// ���� �ܷ�
			switch(m_pDevCmn->SlipStatus)
			{
				case SLIP_NORMAL		:						// ��������
					nStatus = STATUS_RESET;
					break;
				case SLIP_LOW_END		:						// ���� �ܷ�
					break;
				case SLIP_EMPTY_PAPER	:						// ��������
				default					:						// ��������Ȯ�κҰ�
					nStatus = STATUS_SET;
					break;
			}
			break;
		case SLIP_EMPTY_PAPER	:								// ��������
		default					:								// ��������Ȯ�κҰ�
			switch(m_pDevCmn->SlipStatus)
			{
				case SLIP_NORMAL		:						// ��������
					nStatus = STATUS_RESET;
					break;
				case SLIP_LOW_END		:						// ���� �ܷ�
					nStatus = STATUS_RESET;
					break;
				case SLIP_EMPTY_PAPER	:						// ��������
				default					:						// ��������Ȯ�κҰ�
					break;
			}
			break;
	}
	if (nStatus)
		return nStatus;

	switch(RecycleBoxStatus)									// NmsDataSendData.CashDepFull : �Աݸ����ǽǹ�FULL
	{
		case CST_NEAR		:									// �ܷ���/NEAR END
		case CST_EMPTY		:									// �亸��
			switch(m_pDevCmn->CassetteStatus)
			{
				case CST_NEAR		:							// �ܷ���/NEAR FULL
				case CST_EMPTY		:							// �亸��
					break;
				case CST_SET_NG		:							// Ż��
				case CST_FULL		:							// FULL
					nStatus = STATUS_SET;
					break;
				case CST_NORMAL		:							// ����
				case RB_ALREADY_FULL:							// ALREADY FULL
				default				:
					break;
			}
			break;
		case CST_SET_NG		:									// Ż��
		case CST_FULL		:									// FULL
			switch(m_pDevCmn->CassetteStatus)
			{
				case CST_NEAR		:							// �ܷ���/NEAR FULL
				case CST_EMPTY		:							// �亸��
					nStatus = STATUS_RESET;
					break;
				case CST_SET_NG		:							// Ż��
				case CST_FULL		:							// FULL
					break;
				case CST_NORMAL		:							// ����
				case RB_ALREADY_FULL:							// ALREADY FULL
				default				:
					nStatus = STATUS_RESET;
					break;
			}
			break;
		case CST_NORMAL		:									// ����
		case RB_ALREADY_FULL:									// ALREADY FULL
		default				:
			switch(m_pDevCmn->CassetteStatus)
			{
				case CST_NEAR		:							// �ܷ���/NEAR FULL
				case CST_EMPTY		:							// �亸��
					break;
				case CST_SET_NG		:							// Ż��
				case CST_FULL		:							// FULL
					nStatus = STATUS_SET;
					break;
				case CST_NORMAL		:							// ����
				case RB_ALREADY_FULL:							// ALREADY FULL
				default				:
					break;
			}
			break;
	}
	if (nStatus)
		return nStatus;

	if (CassetteStatus == CST_FULL)								// NmsDataSendData.CheckDepFull : �Աݼ�ǥ�ǹ�FULL
	{
		if (m_pDevCmn->CassetteStatus != CST_FULL)
			nStatus = STATUS_RESET;
	}
	else
	if (CassetteStatus != CST_FULL)								// NmsDataSendData.CheckDepFull : �Աݼ�ǥ�ǹ�FULL
	{
		if (m_pDevCmn->CassetteStatus == CST_FULL)
			nStatus = STATUS_SET;
	}

	return nStatus;
}


int CTranCmn::fnAPP_SetPollTimeStart()
{
	time_t	CurrentTime;
	time(&CurrentTime);
	m_dwPollStartTime = CurrentTime;

	if(TranCode2 == TRANID_0006)
		m_pDevCmn->fnNET_ICMP_DeviceStatus();
	else
		m_pDevCmn->fnNET_ICMP_DeviceStatus(TRUE);


	return TRUE;
}

int CTranCmn::fnAPP_RemoteProc(int nFlag)
{

	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;
	int nRet = 0;
	CString strRecvData("");
	CString strGetBid("NORMAL");

	BYTE	szActData[NETBUFFSIZE];
	int		nActDataLength = 0;

	strGetBid = RegGetStr(_REGKEY_REMOTECTRL, "BID_RECV_RESULT", "NORMAL");

	if (strGetBid != OM_DO)
		return T_OK;

	if (!m_pDevCmn->OpenKey)						
		return T_OK;

	RegSetStr(_REGKEY_REMOTECTRL, "BID_RECV_RESULT", OM_NORMAL);
	
	memset(szActData, 0, sizeof(szActData));
	nActDataLength = m_pNetWork->GetRemoteData(szActData);

	if (nActDataLength <= 0)
		return T_OK;

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_RemoteControl  nActDataLength-1[%d]", nActDataLength);

	fnAPP_ClearArrDataField(RX_RECV_F);
	memcpy(&m_RXECASH.byTLen, szActData, nActDataLength);
	m_pNetWork->ClearRemoteData();
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_RemoteControl  nActDataLength-2[%d]", nActDataLength);

	fnAPP_RxECash_GetDataField_RecvProc();
	
	if(!fnAPP_RXGetFindArryDataField(x006X, strlen(x006X), strGetRxDataField, PACK_F))
	{
		MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPL_RemoteControl Data Error [%s]" , x006X);
		return T_OK;
	}

	if(strGetRxDataField.CompareNoCase("A040") != 0)
	{
		return T_OK;
	}




	if(!fnAPP_RXGetFindArryDataField(x051X, strlen(x051X), strGetRxDataField, PACK_F))
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_RemoteControl Data Error [%s]" , x051X);
		return T_OK;
	}
	else
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��������  => ����]"));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��������  => %s]", strGetRxDataField));
		if(strGetRxDataField.CompareNoCase("001") == 0) //�� ���� �䱸
		{
//			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REOPEN;
			m_pDevCmn->HostFirstOpenFlag = FALSE;
			m_pDevCmn->HostStatus        = HOST_WAITONLINE;
			m_pDevCmn->AtmStatus         = ATM_READY;
			m_pDevCmn->RemoteDownFlag	 = FALSE;
		}
		else
		if(strGetRxDataField.CompareNoCase("002") == 0) //�������� �䱸
		{
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_RESTATUS;
			m_pDevCmn->AtmStatus = ATM_READY;
			m_pDevCmn->AtmStatusSave = ATM_READY;

			m_pDevCmn->fnSCR_DisplayString(2, "������� ���� �Դϴ�");
			m_pDevCmn->fnSCR_DisplayString(3, "���� �ð����� ���������̿���");
			m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
			m_pDevCmn->fnSCR_DisplayScreen(701);

			m_pDevCmn->TranResult = FALSE;						
			m_pDevCmn->TranStatus = TRAN_STATUS;		
			m_pDevCmn->TranStatusSave = m_pDevCmn->TranStatus;
			AddSerialFlg = FALSE;
			fnAPP_Tran_TxSend2(TRANID_A020, LINE_SEND_5_TIME);

		}
		else
		if(strGetRxDataField.CompareNoCase("003") == 0) //��ҿ������� �䱸
		{
			return T_OK;
			//m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;

		}
		else
		if(strGetRxDataField.CompareNoCase("004") == 0) //�������� 
		{

			if(!fnAPP_RXGetFindArryDataField(x046X, strlen(x046X), strGetRxDataField1, PACK_F))
			{
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_RemoteControl Data Error [%s]" , x046X);
				return T_OK;
			}
			else
			{
				m_pDevCmn->AtmStatus = ATM_READY;
				m_pDevCmn->AtmStatusSave = ATM_READY;
				m_pDevCmn->fnSCR_DisplayString(2, "������� ���� �Դϴ�");
				m_pDevCmn->fnSCR_DisplayString(3, "���� �ð����� ���������̿���");
				m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
				m_pDevCmn->fnSCR_DisplayScreen(701);
				fnAPP_TranReadyProc();                   //�����ʿ�ݵ��� (ȭ��ó��)
			}
//			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;

		}
		else
		if(strGetRxDataField.CompareNoCase("005") == 0) //������ �䱸
		{
			m_pDevCmn->AtmStatus = ATM_READY;
			m_pDevCmn->AtmStatusSave = ATM_READY;
			m_pDevCmn->RemoteDownFlag = TRUE;
//			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;

		}
		else
		if(strGetRxDataField.CompareNoCase("006") == 0) //�ܸ��� ReBoot�䱸
		{
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;
		}
		else
		if(strGetRxDataField.CompareNoCase("007") == 0) //DnLoadFlag
		{
// 			m_pDevCmn->ForceDownFlag = TRUE;
			m_pDevCmn->fnSCR_DisplayString(2, "AP VERSION UP .....");
			m_pDevCmn->fnSCR_DisplayString(3, "���� �ð����� ���������̿���");
			m_pDevCmn->fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
			m_pDevCmn->fnSCR_DisplayScreen(701);
			Delay_Msg(3000);
			nRet = m_pDevCmn->fnUTL_FTP_Download_File();
			
			if (nRet == T_ERROR)
			{
				m_pDevCmn->AtmStatus = ATM_READY;
			}
// 			else
// 			{
// 				m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT;
// 			}
		}
		else
		if(strGetRxDataField.CompareNoCase("008") == 0) //�ŷ��α����� Up Load ��û
		{
			return T_OK;
		}

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��������  => ��]"));
	}
						
	return T_OK;
}
