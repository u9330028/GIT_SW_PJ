/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranMenuProc_H_INCLUDED_)
#define _TranMenuProc_H_INCLUDED_

#define itemof(item)	(sizeof(item) / sizeof(item[0]))

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	메뉴변수
/////////////////////////////////////////////////////////////////////////////
	MENUHeader MenuTbl[itemof(MenuTblConst)];
										// 메뉴테이블 
	MENUHeader MenuTblTemp[itemof(MenuTblConst)];			
										// 메뉴테이블임시
	MENUHeader MenuTblSave[itemof(MenuTblConst)];			
										// 메뉴테이블보관

/////////////////////////////////////////////////////////////////////////////
//	거래상태변수
/////////////////////////////////////////////////////////////////////////////
    int     m_nScrNum;                  // 화면번호
    int     m_nRetCode;                 // 리턴코드

	int		TranAvail;					// 전체거래상태
	int		WithAvail;					// 출금거래상태
	int		DepAvail;					// 입금거래상태
	int		PbmAvail;					// 통장거래상태
	int		A4pAvail;					// 지급거래상태
	
/////////////////////////////////////////////////////////////////////////////
//	메뉴변수
/////////////////////////////////////////////////////////////////////////////
	int		MenuIndex;					// 메뉴Index
	int		MenuIndexSub;				// 메뉴IndexSub
	int		MenuIndexNext;				// 메뉴IndexNext
	int		MenuAvail;					// 메뉴가능상태
	int		MenuMuTran;					// 메뉴무매체거래상태
	int		MenuBtnCnt;					// 메뉴선택가능한버튼수
	int		MenuBtnIndex;				// 메뉴자동선택가능한최종버튼값
	int		MenuOutGoingMode;			// CREDIT(WITH):1,SAVING(WITH):2,CHECKING(WITH):3,CREDIT(INQ):4,SAVING(INQ):5,CHECKING(INQ):6 ==> WRB에서는 이를 TranCode2로처리(2개변수모두사용가능) // DES OutGoing거래 2004.05.31
	int		MenuFingerMode;				// FINGER:1
	int		MenuIrdaMode;				// IRDA:1 , 핸드폰BARCODE(현금카드):2, 핸드폰BARCODE(송금카드):3
	int		MenuDoingFlag;				//
	
	int		TranProc;					// 거래처리
	int		TranCode;					// 거래코드
	int		TranCode2;					// 거래코드2

	int		TranProcSave;				// 거래처리저장
	int		TranCodeSave;				// 거래코드저장
	int		TranCode2Save;				// 거래코드2저장

	int		TranStartByScr;				// U8100 AP 변경내역 #06 - 속도개선

	CString StrWithMsg;					//#N0217
	CString StrDepMsg;
	CString StrSprMsg;

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	메뉴함수
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
// User Define ↓
    int     fnAPP_TranStartProc();                              // 거래개시처리
    int     fnAPP_InitializeTran();                             // 초기화

	int		fnAPP_MenuCardMCU3Accept();							// Menu Card MCU3 Accept  #N0142
	int		fnAPP_MenuCardMCU2Accept();							// Menu Card MCU2 Accept  #N0266

	int		fnAPL_NearBranchListView();							//#N0172 인근점포 안내

	int		fnAPL_RemoteControl();								//#N0175 원격관리
/////////////////////////////////////////////////////////////////////////////
#endif
