/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranDevProc_H_INCLUDED_)
#define _TranDevProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	매체변수
/////////////////////////////////////////////////////////////////////////////
	int		DevEnDisable;				// 카드통장En/Disable
	int		CardTran;					// Card거래상태변수
	int		ICCardTran;					// Card Transaction
	int		MSCardTran;					// Card Transaction
	int		PbTran;						// Pb거래상태변수
	int		PbKind;						// Pb종류
	int		PbBank;						// 은행통장종류
	int		CardBank;					// 은행카드종류
	int		CardService;				// 신용카드종류
	int		CardNew;					// 신규카드종류
	int		CardBankFlag;				// Card Service
	int		CardServiceFlag;			// Card Service 
	int		ExtraServiceFlag;			// 부가서비스 종류 
	int		BranchKind;					// 자타구분
	int		MenuFICMode;				// 금융IC 카드거래상태변수	2004.02.03
	int		MenuEPBMode;				// 금융IC 전자통장 거래상태 변수
	int		MenuEMVMode;				// EMV카드 거래상태변수
	BYTE	FallBackCnt;				// FallBack 진행카운트 
	int		MenuKCashMode;				// 전자화폐변수
	int		MenuRFDMode;				// RF거래상태변수
	int		FICMyBiFlg;					// 금융IC카드 MyBi카드여부	2004.02.03
	int		FallBackMenuFICMode;		// 금융IC 카드FALLBACK상태변수	V03-00-26-#01
	int		FallBackEMVMode;			// EMV IC 카드거래상태변수		V03-00-26-#01
	int		EMVReadFlg;					// EMV READ FLAG V03-00-25-#02
//	int		BrandSelectIndex;			// 연속거래시 선택된 Brand
	int		KCashStopbit;				// 중지비트확인
	int		KCashPingChangFlag;			// PIN등록/변경변수

	int		CardOnEMV;					// EMV가능 신용카드
	int		EMVAuthError;				// EMV인증에러

	MCAP	CardData;					// 카드자료 
	MCISO2*	pCardData2;					// 2TR Pointer
	MCISO3*	pCardData3;					// 3TR Pointer
	char	CardAccountNo[256];			// Card Account No			// V09-33-00-#01 ICCM 모듈 적용
	char	CardPan[256];				// Card Pan					// V09-33-00-#01 ICCM 모듈 적용

	MS		PbMsData;					// 통장자료
	BAR		PbBarData;					// 통장 Bar Info
	PBMS*	pPbMsData;					// Pb Pointer
	FINGER	FingerData;					// 지문자료
	BAR2D	BarData;					// 바코드자료

	int		CardEmbossScanFlg;			// 카드엠보스스캔유무
	int		CardImprintFlg;				// 카드임프린트유무
	int		CardPowerOnFlg;				// 카드접속유무(전자화폐)
	int		PbDataCnt;					// Pb출력길이
	char	PbData[10240];				// Pb출력영역		

	int				EjrIndexFlg;		// 전자저널유무
	EjrTbl			EjrData;			// 전자저녈자료
	EjrAmountTbl	EjrAmountData;		// 전자저녈금액관련자료

	char	ShuData[34][256];			// Shu출력영역
	int		SlipPrintFlg;				// 명세표인자유무
	int		JnlPrintFlg;				// 저널인자유무
	
	int		MaterialOutFlg;				// 매체방출유무
	int		MaterialOutCheckFlg;		// 매체방출후수취유무
	int		MoneyOutFlg;				// 현금수표방출유무
	int		MoneyOutCheckFlg;			// 현금수표방출후수취유무

	int		SprOutRetryFlg;				// 명세표방출재시도플래그	
	int		BrandSelectIndex;			// 연속거래시 선택된 Brand		V09-33-00-#01  ICCM 모듈 적용
	int		EMVServiceCodeChk;			// V07-08-00-#01 서비스코드 확인 여부

/////////////////////////////////////////////////////////////////////////////
//	시재관련변수
/////////////////////////////////////////////////////////////////////////////
	int		CashCountFlg;				// 출금현금계수유무
	int		CheckCountFlg;				// 출금수표계수유무

	int		CashInCountFlg;				// 입금현금계수유무
	int		CashRejectFlg;				// 입금현금반환유무
	int		CashRejectAFlg;				// 입금현금초과반환유무
	int		CheckInCountFlg;			// 입금수표계수유무
	int		CheckRejectFlg;				// 입금수표반환유무
	int		CheckRejectAFlg;			// 입금수표오류반환유무
	int		CheckRejectKind;			// 입금수표반환사유

	RejectMoney		RejectData;			// 회수내역
	DepositMoney	CashData;			// 현금입금내역
	CheckInfo		CheckInfoData[UCM_MAXCHECKINCNT];	
										// 수표입금내역
	int		CheckInfoStats;				// 수표입금상태
	int		CheckInfoCnt;				// 수표입금매수
	int		CheckInfoAmt;				// 수표입금금액
	CheckPrint		CheckPrintData;		// 수표배서내용
	int		CheckInPrintFlg;			// 수표입금정보인자유무
	int		SumClearFlag;				// Sum Clear Flag

	int		TranEncCode;				// Encrypt Code
	/////////////////////////////////////////////////////////////////////////////////
	int		KbGwamok;						// 통장과목변수(주택 1:적립 2:신탁 3:요구불 4:대출)
	int		KbTongBank; 					// 통장종류(1:구주택,2:구국민,3:통합국민)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//	EMV LIBRARY
//  V09-33-00-#01 ICCM 모듈 적용
/////////////////////////////////////////////////////////////////////////////
	BOOL		EMVEnabled;		
	CString		EMVPrefLanguage; 
	CString		EMVCountryCode;	
	CString		AID;				
	CString		AIDDescription;
	int			EmvReadAppDataFlag;
	
	CStringArray AIDescList;												
	CStringArray AIDList;												
	CStringArray AIDConvertFlag;											
//////////////////////////////////////////////////////////////////////////
	int		m_SelectIndex;										// 연속거래시 선택된 Brand
	int		m_EMVCancelFlg;										// 인증취소FLG

	//#0164
	int		HiPassLoadFailFlg;									// Hi-Pass충전실패유무	
	int		HiPassDeviceKind;									// Hi-Pass거래방법	

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	매체함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPD_CheckDeviceAction(int ChkID, int ErrChkFlg = TRUE);			
																// 동작완료검지&장애검지

	int		fnAPD_CardEnDisable(int Action, int InitFlg = FALSE, int WaitFlg = FALSE, int ContinueFlg = FALSE);
																// 카드흡입모드설정
	int		fnAPD_ICCardEnDisable(int Action, int InitFlg = FALSE, int WaitFlg = FALSE);
																// IC카드흡입모드설정
	int		fnAPD_PbEnDisable(int Action, int InitFlg = FALSE, int WaitFlg = FALSE);			
																// 통장흡입모드설정

	int		fnAPD_CardReadCheck(LPCTSTR strInCardData = NULL);	// 카드읽기&체크
	int		fnAPD_ICCardReadCheck();							// IC카드읽기&체크
	int		fnAPD_PbReadCheck();								// 통장읽기&체크
	int		fnAPD_IBK_PbReadCheck();							// 통장읽기&체크 - IBK
	int		fnAPD_NONGHYUP_PbReadCheck();						// 통장읽기&체크 - NONGHYUP
	int		fnAPD_KEB_PbReadCheck();							// 통장읽기&체크 - KEB
	int		fnAPD_KJB_PbReadCheck();							// 통장읽기&체크 - KJB	
	int		fnAPD_HANA_PbReadCheck();							// 통장읽기&체크 - HANA  #N0214
	int		fnAPD_KB_PbReadCheck();								// 통장읽기&체크 - KB  #N0226
	int		fnAPD_CTB_PbReadCheck();							// 통장읽기&체크 - CITI	#N0252
	int		fnAPD_SC_PbReadCheck();								// 통장읽기&체크 - SC  #N0245
	int		fnAPD_KNB_PbReadCheck();							// 통장읽기&체크 - KNB  #N0277

	int		fnAPD_GwamokCheck_Kb(char * InGwamok);				// 국민 통장 과목 체크#N0226

	int		fnAPD_KEB_GetPbAccount(BYTE* pbSendAccount);		// 통장계좌정보구하기
	int		fnAPD_GetPbAccount(BYTE* pbSendAccount);			// 통장계좌정보구하기
	int		fnAPD_PbMSWrite();		

	int		fnAPD_CardEmbossScan(int WaitFlg = FALSE);			// 카드엠보스스캔
	int		fnAPD_CardImprint(int WaitFlg = FALSE);				// 카드임프린트
	int		fnAPD_CardWrite(MCAP* WriteCardData, int WaitFlg = FALSE);
																// 카드기록

	int		fnAPD_PbPrint(int WaitFlg = FALSE);					// 통장인자
	int		fnAPD_EjrIndexSet();								// 전자저널INDEX정보설정
	int		fnAPD_ShuPrint(int PrintDevice, int JnlType = JNL_TRAN, int WaitFlg = FALSE);	
																// 명세표&저널인자
	int		fnAPD_JnlPrint(int WaitFlg = FALSE);				// 저널인자(내부처리)

	int		fnAPD_MaterialOut(int Device, int WaitFlg = FALSE, int CheckTime = K_45_WAIT);	
																// 매체방출
	int		fnAPD_A4PMaterialOut(int nPrintEndFlag, int WaitFlg, int CheckTime = K_45_WAIT);
																// A4P매체방출
	int		fnAPD_MaterialOutCheckRetract(int Device, int WaitFlg = TRUE);
																// 매체방출수취대기회수
	int		fnAPD_MaterialRetract(int Device, int WaitFlg = TRUE);			
																// 매체회수

	int		fnAPD_MoneyCount(int WaitFlg = FALSE);				// 현금수표출금계수
	int		fnAPD_MoneyOutCheck(int ScrNum = 0, int CheckTime = K_60_WAIT);	//#0134
																// 현금수표방출수취대기
	int		fnAPD_MoneyRetract();								// 현금수표회수

	int		fnAPD_CashDeposit();								// 입금현금
	int		fnAPD_CashDepositCount();							// 입금현금계수
	int		fnAPD_CashDepositStore(int WaitFlg = FALSE);		// 입금현금수납
	int		fnAPD_CashDepositReject();							// 입금현금반환

	int		fnAPD_CheckDeposit();								// 입금수표
	int		fnAPD_CheckDepositCount();							// 입금수표계수
	int		fnAPD_CheckDepositCurOutCheck(int ScrNum, int CheckTime = K_45_WAIT);	
																// 입금수표오류반환수취대기
	int		fnAPP_CheckDepositPrintEdit();						// 입금수표배서내용편집
	int		fnAPD_CheckDepositStore(int WaitFlg = FALSE);		// 입금수표수납
	int		fnAPD_CheckDepositReject();							// 입금수표반환
	int		fnAPD_CheckDepositPrint(int PrintDevice, int WaitFlg = FALSE);			
																// 입금수표정보인자
	int		fnAPD_CashDepositPrint(int WaitFlg = FALSE);		// 현금계수정보인자

	int		fnAPD_MaterialOutRetryAfterError(int nDevice);		// 매체방출후장애시방출재시도	V02-06-02-#05 21405명세표재방출

	int		fnAPD_CardReadSerial();								// 카드연속거래처리
	int		IsBCCRight(BYTE* pbyBuf, DWORD dwBufSize);			// 인증카드 형식 (MS 직원)
	BYTE	fnAuthGetDigit(CString strTemp);
	BYTE	fnAuthGetAccumlate(int nPreSum, CString strTemp);
	BYTE	fnAuthGetAccumlate(CString strTemp);
/////////////////////////////////////////////////////////////////////////////
//	금융 IC 함수 2004.02.03
/////////////////////////////////////////////////////////////////////////////
	int		fnAPD_FICCardReadCheck();							// 금융IC카드읽기&체크		2004.02.03
	int		fnAPD_FICDistinctCardType(int nReadDF = 0);			// IC카드 구분처리(폐쇄형/NATIVE/개방형등) : IC선행시처리대응
	int		fnAPD_FICBasicInquiry();							// 금융IC 정보 조회(모든 거래시 공통)
	int		fnAPD_FICMyBiProc();								// MyBi 금융 IC 거래

	int		fnAPD_FICGetEncipher();								// 금융IC 비밀번호암호화처리2004.02.03	
																// 카드난수(32) + 암호화비밀번호(32)
	int		fnAPD_FICGetHostTranData();							// 금융IC 송신데이타얻기	2004.02.03
																// 서버난수(32) + 카드난수(32) + 암호화비밀번호(32)
	int		fnAPD_PbSpeedUpCheck();									// 통장 바코드 읽기 속도 개선
	int		fnAPD_PBBarCodeRead();									//#N0245
/////////////////////////////////////////////////////////////////////////////
//	EMV 함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_EMVInitProc();								// EMV 관련 Buffer를 초기화하는 함수(기기 Start시 1회 호출)
	int		fnAPP_EMVOpenSetProc(int ConfigCode =1);			// 개국시(또는 인증시 Configuratin Code Setting시) EMV 정보를 세팅
	int		fnAPP_EMVReadProc();								// EMV Application Read 함수
	int		fnAPP_EMVAfterOnlineTranProc();						// EMV 온라인 후 거래 처리 함수
	int		fnAPP_EMVReversalProc();							// EMV취소거래 처리함수
	int		fnAPD_EMVJaTaCheck();								// 자타행구분 임시함수

	int		fnAPD_EMVTerminalConstant();						// #N0268 FB율 개선 EMV TerminalConstant 설정
	
	int		fnAPD_EMVContinueTrans();							// 조회 후 거래 등의 연속 거래를 지원을 위한 전문 재작성
	int		fnAPD_EMVSelectProc();								// EMV Application을 확인하는 함수
	int		fnAPD_EMVBrandChoice();								// EMV Application 중 거래할 Brand를 선택하는 함수
//D	int		fnAPD_EMVJaTaCheck();								// 자타행구분
	int		fnAPD_EMVSelectBrand();								// 거래할 EMV Application이 복수개 일 때 고객의 선택을 받는 함수
	int		fnAPD_EMVSendMakeProc();							// EMV거래를 위한 온라인 송신 전처리 함수
	int		fnAPD_EMVAfterRecvProc();							// 온라인거래 후처리 함수
	int		fnAPD_EMVResultProc();								// EMV거래 결과를 조회하는 함수(인증&디버깅用)
	CString fnAPP_GetCaptureData();								// U8100 AP 변경내역 #01 - 3Camera, Preview지원, 텍스트출력	

/////////////////////////////////////////////////////////////////////////////
//	EMV LIBRARY
//  V09-33-00-#01 ICCM 모듈 적용 
/////////////////////////////////////////////////////////////////////////////
	CString	fnAPDLib_WCStoMBS(LPBYTE pAIDName, int nLen);		// @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK
	CString fnAPDLib_MBStoWCS(LPCSTR pAIDName);					// @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK
		
	int		fnAPPLib_EMVReadProc();								// EMV Application Read 함수
	int 	fnAPDLib_EMVUserAIDLoad(int nAIDIndex, BYTE btUserConfirm, int nAIDNum, LPCTSTR strAID);	// @ADD:[EMVKERNEL5_001] 2013.06.12 SHPARK - AID DESC 목록 저장
	int		fnAPDLib_EMVSelectAID();							// [#156] US Justin 2012.05.04 Implement EMV Transaction
	int		fnAPDLib_EMVUserAIDSelection(int nAIDNum, LPCTSTR strAID);		// [#156] US Justin 2012.06.06 Implement EMV Transaction
	int		fnAPDLib_EMVSendMakeProc(int nEmvType = 1);						// Default is TRANSACTION
	int		fnAPDLib_OnlineProcessing(LPSTR lpszHostData, int nHostResp, BOOL HostRecvFlag);		// 온라인 승인 요청에 대한 호스트 응답을 카드로 전달

	int		fnAPD_PBMPrintTest();								// 통장부프린트테스트	#N0211

	// #0164 Hi-Pass접촉식충전개발 #N0266 
/////////////////////////////////////////////////////////////////////////////
//  하이패스 함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPD_HPSVerifyTranBit();							// 하이패스카드 거래중지비트 체크
	int		fnAPD_HPSReadBalance(int nFlag = FALSE);			// 하이패스카드 잔액조회		(접촉식)
	int		fnAPD_HPSPrepareLoad(char *pAmount);				// 하이패스카드 충전1단계		(접촉식)
	int		fnAPD_HPSLoad(BYTE *pRN, BYTE *pSign2);				// 하이패스카드 충전2단계		(접촉식)
	int		fnAPD_HPSComplete();								// 하이패스카드 충전완료(취소)	(접촉식)
// ──────────────────────────────────────

//////////////////////////////////////////////////////////////////////////
	
/////////////////////////////////////////////////////////////////////////////
#endif
