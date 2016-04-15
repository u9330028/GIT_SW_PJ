/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranNmsProc_H_INCLUDED_)
#define _TranNmsProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Nms변수
/////////////////////////////////////////////////////////////////////////////
	int		ErrorSaveFlg;										// 장애전문SaveFlg
	int		ErrorInDirectFlg;									// 간접장애(고객,텔라,송수신등)
	int		ErrorAutoOffSave;									// 이전자동축퇴값

	int		RecycleBoxStatus;									// 현금상태
	int		RejectCSTLoc;
	int		ClerkCSTLoc;
	
	int		CheckCSTStatus;										// 출금수표상태
	int		CheckCSTStatusSensor;								// 출금수표상태(실물)
	int		CheckCSTStatusReg;									// 출금수표상태(등록)
	int		CassetteStatus;										// 카세트상태
	int		SlipHeadStatus;										// 명세표 Cover 상태
	int		SlipStatus;											// 명세표상태
	int		JnlStatus ;											// 저널상태
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
	
	int		RejectCSTStatus;									// 리젝트회수함의상태
	int		RetractCSTStatus;									// 현금회수함의상태
	int		A4pStatus;											// A4프린터

	int		GAlarmStatus;				// 알람센서상태
	int		GExternalSensorStatus1;		// 외부센서1
	int		GExternalSensorStatus2;		// 외부센서2
	int		GSensorStatus;				// 충격센서상태
	int		GSensorLelvel;				// 충격센서감도
	int		GBatteryStatus;				// 밧데리센서감도	
	int		OpenKey;				    // A4프린터
	int     CameraStatus;       		// 카메라상태
	int		ForceMissMatchDownFlag;     // 카세트상태 (점퍼)
	int		ForceSendStatus;			// 강제상태전문송신
	int		DeviceLoss;

	NMSSEND	NmsDataSendData;									// 장애/상태전문버퍼
	NMSSEND	NmsSaveDataSendData;								// nms SendData Save

	int		NmsSendLength;										// Nms송신Length
	int		NmsRecvLength;										// Nms수신Length
	int		NmsRecvLengthTbl[NMSRECVTBL];						// Nms수신LengthTbl

	int		NmsRecvInTbl;										// Nms수신In
	int		NmsRecvOutTbl;										// Nms수신Out
	BYTE	NmsSendBuffer[NETBUFFSIZE];							// Nms송신Buffer
	BYTE	NmsRecvBuffer[NETBUFFSIZE];							// Nms수신Buffer
	BYTE	NmsRecvBufferTbl[NMSRECVTBL][NETBUFFSIZE];			// Nms수신BufferTbl

	int		NmsErrorSendFlg;									// 장애Send유무

	int		NmsErrorSendInfo;									// 1:송수신장애
																// 2:기기장애
																// 3:고객미수취
																// 4:센타불능코드(341~349 347제외)
	int		CheckPbmInkStaus;									// 적용후 1차 수정 (U8100-AP변경 #13) - IPBM INK 상태관련 추가

private:
	time_t	m_dwPollStartTime;									// V01-00-00-#304

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Nms호출함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_NmsProc(int InitFlg);							// Nms처리
	int		fnAPP_SetPollTimeStart();							// Data송신 시 부터 Poll Time Set
	int		fnAPP_RemoteProc(int nFlag = 0);
/////////////////////////////////////////////////////////////////////////////
//	송수신 함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_SendData(BYTE* SendBuff, int Length, int SendSec, int RecvBuffClearFlg = TRUE);
																// Send Data
	int		fnAPP_RecvDataClear();								// Recv Data Clear
	int		fnAPP_RecvData(BYTE* RecvBuff, int* Length, int RecvSec);
	int		fnAPP_RecvInTbl(BYTE* Buff, int Length);			// Recv In Tbl
	int		fnAPP_RecvOutTbl(BYTE* Buff, int* Length);			// Recv Out Tbl

/////////////////////////////////////////////////////////////////////////////
//	송수신 내부처리
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_NmsIncDeviceCnt();							// 장애난장치 장치장애카운터증가
	int		fnAPP_NmsUpdateStatus(BYTE* StatusBuff = NULL);		// 상태버퍼 작성
	int		fnAPP_NmsDistinctStatus();							// 상태전문 구분

/////////////////////////////////////////////////////////////////////////////
#endif
