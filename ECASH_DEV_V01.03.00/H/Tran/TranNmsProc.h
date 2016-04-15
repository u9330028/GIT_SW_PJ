/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranNmsProc_H_INCLUDED_)
#define _TranNmsProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Nms����
/////////////////////////////////////////////////////////////////////////////
	int		ErrorSaveFlg;										// �������SaveFlg
	int		ErrorInDirectFlg;									// �������(��,�ڶ�,�ۼ��ŵ�)
	int		ErrorAutoOffSave;									// �����ڵ�����

	int		RecycleBoxStatus;									// ���ݻ���
	int		RejectCSTLoc;
	int		ClerkCSTLoc;
	
	int		CheckCSTStatus;										// ��ݼ�ǥ����
	int		CheckCSTStatusSensor;								// ��ݼ�ǥ����(�ǹ�)
	int		CheckCSTStatusReg;									// ��ݼ�ǥ����(���)
	int		CassetteStatus;										// ī��Ʈ����
	int		SlipHeadStatus;										// ��ǥ Cover ����
	int		SlipStatus;											// ��ǥ����
	int		JnlStatus ;											// ���λ���
	int		BrmLoc;
	int		FstCSTStatus;
	int		SndCSTStatus;
	int		TrdCSTStatus;
	int		FthCSTStatus;
	int		UcmLoc;
	int		DoorStatus;
	int		CabinetStatus;
	int		TmpTranAvail;
	int		TmpWithAvail;
	int		TmpDepAvail;
	
	int		RejectCSTStatus;									// ����Ʈȸ�����ǻ���
	int		RetractCSTStatus;									// ����ȸ�����ǻ���
	int		A4pStatus;											// A4������

	int		GAlarmStatus;				// �˶���������
	int		GExternalSensorStatus1;		// �ܺμ���1
	int		GExternalSensorStatus2;		// �ܺμ���2
	int		GSensorStatus;				// ��ݼ�������
	int		GSensorLelvel;				// ��ݼ�������
	int		GBatteryStatus;				// �嵥����������	
	int		OpenKey;				    // A4������
	int     CameraStatus;       		// ī�޶����
	int		ForceMissMatchDownFlag;     // ī��Ʈ���� (����)
	int		ForceSendStatus;			// �������������۽�
	int		DeviceLoss;

	NMSSEND	NmsDataSendData;									// ���/������������
	NMSSEND	NmsSaveDataSendData;								// nms SendData Save

	int		NmsSendLength;										// Nms�۽�Length
	int		NmsRecvLength;										// Nms����Length
	int		NmsRecvLengthTbl[NMSRECVTBL];						// Nms����LengthTbl

	int		NmsRecvInTbl;										// Nms����In
	int		NmsRecvOutTbl;										// Nms����Out
	BYTE	NmsSendBuffer[NETBUFFSIZE];							// Nms�۽�Buffer
	BYTE	NmsRecvBuffer[NETBUFFSIZE];							// Nms����Buffer
	BYTE	NmsRecvBufferTbl[NMSRECVTBL][NETBUFFSIZE];			// Nms����BufferTbl

	int		NmsErrorSendFlg;									// ���Send����

	int		NmsErrorSendInfo;									// 1:�ۼ������
																// 2:������
																// 3:���̼���
																// 4:��Ÿ�Ҵ��ڵ�(341~349 347����)
	int		CheckPbmInkStaus;									// ������ 1�� ���� (U8100-AP���� #13) - IPBM INK ���°��� �߰�

private:
	time_t	m_dwPollStartTime;									// V01-00-00-#304

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Nmsȣ���Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_NmsProc(int InitFlg);							// Nmsó��
	int		fnAPP_SetPollTimeStart();							// Data�۽� �� ���� Poll Time Set
	int		fnAPP_RemoteProc(int nFlag = 0);
/////////////////////////////////////////////////////////////////////////////
//	�ۼ��� �Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_SendData(BYTE* SendBuff, int Length, int SendSec, int RecvBuffClearFlg = TRUE);
																// Send Data
	int		fnAPP_RecvDataClear();								// Recv Data Clear
	int		fnAPP_RecvData(BYTE* RecvBuff, int* Length, int RecvSec);
	int		fnAPP_RecvInTbl(BYTE* Buff, int Length);			// Recv In Tbl
	int		fnAPP_RecvOutTbl(BYTE* Buff, int* Length);			// Recv Out Tbl

/////////////////////////////////////////////////////////////////////////////
//	�ۼ��� ����ó��
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_NmsIncDeviceCnt();							// ��ֳ���ġ ��ġ���ī��������
	int		fnAPP_NmsUpdateStatus(BYTE* StatusBuff = NULL);		// ���¹��� �ۼ�
	int		fnAPP_NmsDistinctStatus();							// �������� ����

/////////////////////////////////////////////////////////////////////////////
#endif
