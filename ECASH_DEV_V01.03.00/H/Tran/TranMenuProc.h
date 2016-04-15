/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranMenuProc_H_INCLUDED_)
#define _TranMenuProc_H_INCLUDED_

#define itemof(item)	(sizeof(item) / sizeof(item[0]))

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	�޴�����
/////////////////////////////////////////////////////////////////////////////
	MENUHeader MenuTbl[itemof(MenuTblConst)];
										// �޴����̺� 
	MENUHeader MenuTblTemp[itemof(MenuTblConst)];			
										// �޴����̺��ӽ�
	MENUHeader MenuTblSave[itemof(MenuTblConst)];			
										// �޴����̺���

/////////////////////////////////////////////////////////////////////////////
//	�ŷ����º���
/////////////////////////////////////////////////////////////////////////////
    int     m_nScrNum;                  // ȭ���ȣ
    int     m_nRetCode;                 // �����ڵ�

	int		TranAvail;					// ��ü�ŷ�����
	int		WithAvail;					// ��ݰŷ�����
	int		DepAvail;					// �Աݰŷ�����
	int		PbmAvail;					// ����ŷ�����
	int		A4pAvail;					// ���ްŷ�����
	
/////////////////////////////////////////////////////////////////////////////
//	�޴�����
/////////////////////////////////////////////////////////////////////////////
	int		MenuIndex;					// �޴�Index
	int		MenuIndexSub;				// �޴�IndexSub
	int		MenuIndexNext;				// �޴�IndexNext
	int		MenuAvail;					// �޴����ɻ���
	int		MenuMuTran;					// �޴�����ü�ŷ�����
	int		MenuBtnCnt;					// �޴����ð����ѹ�ư��
	int		MenuBtnIndex;				// �޴��ڵ����ð�����������ư��
	int		MenuOutGoingMode;			// CREDIT(WITH):1,SAVING(WITH):2,CHECKING(WITH):3,CREDIT(INQ):4,SAVING(INQ):5,CHECKING(INQ):6 ==> WRB������ �̸� TranCode2��ó��(2��������λ�밡��) // DES OutGoing�ŷ� 2004.05.31
	int		MenuFingerMode;				// FINGER:1
	int		MenuIrdaMode;				// IRDA:1 , �ڵ���BARCODE(����ī��):2, �ڵ���BARCODE(�۱�ī��):3
	int		MenuDoingFlag;				//
	
	int		TranProc;					// �ŷ�ó��
	int		TranCode;					// �ŷ��ڵ�
	int		TranCode2;					// �ŷ��ڵ�2

	int		TranProcSave;				// �ŷ�ó������
	int		TranCodeSave;				// �ŷ��ڵ�����
	int		TranCode2Save;				// �ŷ��ڵ�2����

	int		TranStartByScr;				// U8100 AP ���泻�� #06 - �ӵ�����

	CString StrWithMsg;					//#N0217
	CString StrDepMsg;
	CString StrSprMsg;

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	�޴��Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_MainMenuDisplay(int InitFlg);					// Main Menu Display
	int		fnAPP_MainMenuDeviceEvent(int CheckTime = 0);		// Main Menu Device Event

/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_MenuTranMsg(int WithAvail, char *WithMsg, int DepAvail, char *DepMsg);
																// Menu Tran Msg
	int		fnAPP_MenuMaterialMsg(int LanMode, char *DeviceMsg);// Menu Device Msg
	int		fnAPP_MenuMake(void* MenuTblMake);					// Menu Make
	int		fnAPP_MenuClearMenuCode(int Index = 0, int MenuCode = NULL, int MenuValue = NULL);
																// Menu Clear Menu Code
	int		fnAPP_MenuClearTranProc(int Index = 0, int TranValue = NULL, int MenuValue = NULL);
																// Menu Clear Tran Proc
	int		fnAPP_MenuCodeTranProc(int Index = 0, int TranValue = NULL, int MenuValue = NULL);
	int		fnAPP_MenuClearTranCode(int Index = 0, int TranValue = NULL, int MenuValue = NULL);
																// Menu Clear Tran Code
	int		fnAPP_MenuAvailCheck(int Index, int SubIndex = 0);	// Menu Avail Check
	int		fnAPP_MenuBtnCheck(int Index);						// Menu Button Check

/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_MenuProc();									// Menu Procedure
	int		fnAPP_MenuDisplay();								// Menu Display
	int		fnAPP_MenuDeviceEvent(int CheckTime = K_65_WAIT);	// Menu Device Event
	int		fnAPP_MenuDeviceProc();								// Menu Device Procedure
	int		fnAPP_MenuDeviceCheck();							// Menu Device Check
	int		fnAPP_MenuCardPbMuAccept(int ProcAvail = 0);		// Menu Card Pb Mu Accept
	int		fnAPP_MenuIrdaAccept();								// Menu Irda Accept
	int		fnAPP_MenuIrdaBarCodeAccept();						// Menu Irda/2DBarCode Accept
	int		fnAPP_MenuBeforeSprCheck();							// 
	int		fnGetNhCardConFirm();								// 

	int		fnAPP_MenuWithDepMsg();	// #N0217

/////////////////////////////////////////////////////////////////////////////
// User Define ��
    int     fnAPP_TranStartProc();                              // �ŷ�����ó��
    int     fnAPP_InitializeTran();                             // �ʱ�ȭ

	int		fnAPP_MenuCardMCU3Accept();							// Menu Card MCU3 Accept  #N0142
	int		fnAPP_MenuCardMCU2Accept();							// Menu Card MCU2 Accept  #N0266

	int		fnAPL_NearBranchListView();							//#N0172 �α����� �ȳ�

	int		fnAPL_RemoteControl();								//#N0175 ���ݰ���
/////////////////////////////////////////////////////////////////////////////
#endif
