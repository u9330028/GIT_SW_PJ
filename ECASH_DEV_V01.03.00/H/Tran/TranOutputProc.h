/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranOutputProc_H_INCLUDED_)
#define _TranOutputProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
	int	A4PrintCnt;												// 출력할 A4Print Line Count(PMERecvData.y값)

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
////////////////////////////////////////////////////////////////////////////
//	수신후처리함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_PSPProc(int PrintDevice, int WaitFlg = FALSE);// 명세표/저널 기록																				// 명세프린트
	int		fnAPP_IBK_PPPProc(int WaitFlg = FALSE);					// 통장부프린트
	int		fnAPP_NONGHYUP_PPPProc(int WaitFlg = FALSE);					// 통장부프린트
	int		fnAPP_KEB_PPPProc(int WaitFlg = FALSE);				// 통장부프린트
	int		fnAPP_KJB_PPPProc(int WaitFlg = FALSE);				// 통장부프린트 #N0181
	int		fnAPP_HANA_PPPProc(int WaitFlg = FALSE);			// 통장부프린트 #N0214
	int		fnAPP_KB_PPPProc(int WaitFlg = FALSE);				// 통장부프린트 #N0226
	int		fnAPP_CTB_PPPProc(int WaitFlg = FALSE);				// 통장부프린트	#N0252
	int		fnAPP_SC_PPPProc(int WaitFlg = FALSE);				// 통장부프린트 #N0245
	int		fnAPP_KNB_PPPProc(int WaitFlg = FALSE);				// 통장부프린트 #N0277

	int		fnAPP_TestPPPProc();								// 통장부프린트테스트 #N0211
	
	int		fnAPP_PPPProc(int WaitFlg = FALSE);					// 통장부프린트
	int		fnAPP_PCWProc(int WaitFlg = FALSE);					// 카드M/S기록
	int		fnAPP_PMDProc();									// 메세지표시

	int		fnAPP_A4PrintProc(int nKind);						// A4 Print 메인Procedure				2007.06.12(火)
	int		fnAPP_A4PrintTranList();							// 거래내역조회 A4Print					2007.06.12(火)
	int		fnAPP_A4PrintDepSequreInquiry();					// 예수금조회 & 유가증권조회 A4Print	2007.06.12(火)
	int		fnAPP_A4PrintInqStock();							// 청약조회 내역  A4Print	            2008.04.02(火)

	int		fnAPP_PMEAcceptInquiryTranSel();					// 조회거래 연속거래선택
	int		fnAPP_PMEAcceptInquiryDisplay();					// 조회결과표시(예수금조회)
	int		fnAPP_PMEAcceptInquiryTranServiceConfirm();					// 조회결과표시(예수금조회)

	int		fnAPP_PMEAcceptTransConfirm();						// 이체거래 확인
	int		fnAPP_PMEAcceptRemitConfirm();						// 송금거래 확인
	int		fnAPP_PMEAcceptAutoConfirm();						// 자동이체설정 확인
	int		fnAPP_PMEAcceptTranListConfirm();					// 거래내역조회 거래 확인
	int		fnAPP_PMEAcceptSeqListConfirm();					// 증권리스트 항목확인
	int		fnAPP_PMEAcceptSeqTranConfirm();					// 청약확인조회
	int		fnAPP_PMEAcceptCanSeqTranConfirm();					// 취소종목확인
	int		fnAPP_PMEAcceptTranStocConfirm();					// 자사대체확인
	int		fnAPP_PMEAcceptRemitTranSel();						// 차액계좌선택
	int		fnAPP_PMEAcceptJiroTaxSel();						// 지로공과금 일반지로납부 납부내역확인 1차
	int		fnAPP_PMEAcceptJiroTaxConfirm();					// 지로공과금 일반지로납부 납부내역확인 2차
	int		fnAPP_PMEAcceptJiroFinConfirm();					// 지로공과금 재정납부 납부내역확인
	int		fnAPP_PMEAcceptJiroInqComfirm();					// 지로납부내역 조회확인
	int		fnAPP_PMEAcceptJiroFinInqComfirm();					// 재정 지로납부내역 조회확인
	int		fnAPP_PMEAcceptTaDepositConfirm();					// 무매체입금 조회확인
	int		fnAPP_PMEAcceptDepConfirm();					// 타행입금조회 확인화면
	int		fnAPP_PMEAcceptMuDepConfirm();					// 타행입금조회 확인화면
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_ReadyProc();									// 준비완처리
	int		fnAPP_ReadyPrintResult(int PrintDevice, int WaitFlg = FALSE);
																// 준비완결과인자
	int		fnAPP_OpenProc();									// 개국처리
	int		fnAPP_OpenPrintResult(int PrintDevice, LPCTSTR szDate, LPCTSTR szTime, int WaitFlg = FALSE);
																// 개국결과인자
	int		fnAPP_OpenDesPrintResult(int PrintDevice, int WaitFlg = FALSE);
																// Des개국결과인자
	int		fnAPP_SumProc(int InputID);							// 합계처리
	int		fnAPP_SumClear(int InputID = FALSE);						// 합계처리
	int		fnAPP_DepErrSumProc(int InputID);							// 입금장애
	int		fnAPP_WithErrSumProc(int InputID);							// 출금장애

	int		fnAPP_SumPrintResult(int TranId, int PrintDevice, int WaitFlg = FALSE);
																// 합계결과인자
	int		fnAPP_ClosePrintResult(int TranId, int PrintDevice, int WaitFlg = FALSE);
																// 마감결과인자
	int		fnAPP_CheckResultPrint(int PrintDevice, int nFlag = FALSE, int WaitFlg = FALSE);
																// 수표등록인자

/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_MiniATM_SumPrint(int PrintDevice, int WaitFlag = FALSE);	
	int		fnAPP_MiniATM_SumPrint1(int PrintDevice, int WaitFlag = FALSE);	
	int		fnAPP_MiniATM_SumPrint2(int PrintDevice, int WaitFlag = FALSE);	
	int		fnAPP_MiniATM_SumPrint3(int PrintDevice, int WaitFlag = FALSE);	
	int		fnAPP_MiniATM_CashPrint(int PrintDevice, int WaitFlag= FALSE);
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
	int		fnAPP_ErrorTranProc();								// 거래장애정보설정
	int		fnAPP_ErrorCancelProc();							// 취소장애정보설정
	int		fnAPP_ErrorHostProc();								// 호스트장애정보설정
	int		fnAPP_ErrorKCashProc();								// KCash 복구거래정보설정

//////////////////////////////////////////////////////////////////////
	int		fnAPP_PrintError(int PrintDevice, int WaitFlg = FALSE);			
																// 장애인자
	int		fnAPP_PrintWithCancelTran(int PrintDevice, int WaitFlg = FALSE);			
																// 출금취소거래인자
	int		fnAPP_PrintDepCancelTran(int PrintDevice, int WaitFlg = FALSE);			
																// 입금취소거래인자
	int		fnAPP_PrintCancel(int PrintDevice, int WaitFlg = FALSE);
																// 취소인자
	void	fvAPP_AnalyzeICError(char* ErrorData, int Param = FALSE);
																// K-CASH Module의 장애정보 분석/프린트
	int		fnAPP_PrintCheckDeposit(int PrintDevice, int WaitFlg = FALSE);
																// 수표입금정보인자
/////////////////////////////////////////////////////////////////////////////
//	편집함수
/////////////////////////////////////////////////////////////////////////////
	char*	fchpAPP_EditBank();									// 은행코드편집
	char*	fchpAPP_EditAcnt(int HiddenFlg,  int AcntOnlyFlg = FALSE);				
																// 계좌번호편집
	CString	fchpAPP_EditSrcAcnt(void *inbuff, int inLen, int outLen, int HiddenFlg = FALSE,  int AcntOnlyFlg = FALSE);				
																// 인출 계좌번호편집
	CString fchpAPP_EditDestAcnt(void *inbuff, int inLen, int outLen, int HiddenFlg = FALSE,  int AcntOnlyFlg = FALSE);				
																// 수취 계좌번호편집
	int		fnAPP_EditPrintLine(int PrintDevice, void* cData, int LineNum, int HiddenFlg = FALSE);
	int		fnAPP_EditPrintLine_Sub_U8100_1(int PrintDevice, void* cData, int LineNum, int HiddenFlg = FALSE);
	int		fnAPP_EditPrintLine_Sub_T3_1(int PrintDevice, void* cData, int LineNum, int HiddenFlg = FALSE);
	int		fnAPP_EditPrintLine_Sub_T3_2(int PrintDevice, void* cData, int LineNum, int HiddenFlg = FALSE);
	int		fnAPP_EditPrintLine_Sub_T3_3(int PrintDevice, void* cData, int LineNum, int HiddenFlg = FALSE);
																// 명세표편집
	BOOL	fBAPP_EditAmount(void *inbuff,void *outbuff,int inLen,int outLen,int isminus);
																// 문자열 변환
	CString fchpAPP_Asc2Amt(void* inbuff, int inLen, int outLen, char MarkChar = ' ');

	// 연속거래 
	int		fnAPP_TranGetContinueProc(int nTranTmpCode);
	int		fnAPP_TranGetContinueProc_Sub(int nTranTmpCode);

	int		fnAPP_POINTS_Sub01_OutputProc();

	int		fnAPP_UTILYBILLS_Sub01_A_OutputProc();
	int		fnAPP_UTILYBILLS_Sub01_B_OutputProc();

	int		fnAPP_UTILYBILLS_SR1_Sub01_OutputProc();
	
	//#GIRO
	int		fnAPP_LOCALTAX_Sub01_OutputProc();
	int		fnAPP_LOCALTAX_Sub02_OutputProc();
	int		fnAPP_LOCALTAX_Sub03_OutputProc();

	//#N0142
	int		fnAPP_E_TICKET_Sub01_OutputProc();
	int		fnAPP_E_TICKET_Sub02_OutputProc();
	int		fnAPP_E_TICKET_Sub03_OutputProc();
	int		fnAPP_E_TICKET_Sub04_OutputProc();

	int		fnAPP_TranCRMSProc();
	int		fnAPP_TranPOINTSProc();
	int		fnAPP_TranINTERADProc();
	int		fnAPP_TranMOBILEGAMEProc();

	//#N0174 외환 이지원  
	int		fnAPP_EASYONE_Sub01_OutputProc();
	int		fnAPP_EASYONE_Sub02_OutputProc();


/////////////////////////////////////////////////////////////////////////////
#endif
