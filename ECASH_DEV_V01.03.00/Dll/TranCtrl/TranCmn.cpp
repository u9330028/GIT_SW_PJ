/////////////////////////////////////////////////////////////////////////////
// TranCmn.cpp : implementation file
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
// CTranCmn
/////////////////////////////////////////////////////////////////////////////
CTranCmn::CTranCmn()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn()");

/////////////////////////////////////////////////////////////////////////////
//  개국정보
/////////////////////////////////////////////////////////////////////////////
	memset(&OpenInfo, 0, sizeof(OpenInfo));

	memcpy(OpenInfo.WithCashCount,	"100", 3);
	memcpy(OpenInfo.WithCheckCount,	"010", 3);
	memcpy(OpenInfo.DepCashCount,	"100", 3);
	memcpy(OpenInfo.DepCheckCount,	"010", 3);
	Strcpy(OpenInfo.TransMoney,  "006000000");
	Strcpy(OpenInfo.SvcTransMoney,  "099990000");
	sprintf((char *)OpenInfo.IdleMessage, "%8.8s173000", GetDate().GetBuffer(0));

	memcpy(MenuTbl, MenuTblConst, sizeof(MenuTbl));				// 거래버튼초기화
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 1단계지원)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 2단계지원)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 3단계지원)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 4단계지원)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 5단계지원)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 6단계지원)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 7단계지원)
	
/////////////////////////////////////////////////////////////////////////////
//	Nms변수
/////////////////////////////////////////////////////////////////////////////
	ErrorSaveFlg = FALSE;										// 장애전문SaveFlg
	ErrorInDirectFlg = FALSE;									// 간접장애(고객,텔라,송수신등)
	ErrorAutoOffSave = 0;										// 이전자동축퇴값

	RecycleBoxStatus = CST_NORMAL;								// 현금상태
	CheckCSTStatus = CST_NORMAL;								// 출금수표상태
	CheckCSTStatusSensor = CST_NORMAL;							// 출금수표상태(실물)
	CheckCSTStatusReg = CST_NORMAL;								// 출금수표상태(등록)
	CassetteStatus = CST_NORMAL;								// 입금수표상태
	SlipStatus = SLIP_NORMAL;									// 명세표상태
	JnlStatus = JNL_NORMAL;										// 저널상태
	RejectCSTStatus = CST_NORMAL;								// 현금회수함의상태
	A4pStatus = CST_NORMAL;										// A4프린터상태
	memset(&NmsDataSendData, 0, sizeof(NmsDataSendData));		// 장애/상태전문버퍼
	memset(&NmsSaveDataSendData, 0, sizeof(NmsSaveDataSendData));
																// nms SendData Save
	
	NmsSendLength = 0;											// Nms송신Length
	NmsRecvLength = 0;											// Nms수신Length
	memset(NmsRecvLengthTbl, 0, sizeof(NmsRecvLengthTbl));		// Nms수신LengthTbl
	
	NmsRecvInTbl = 0;											// Nms수신In
	NmsRecvOutTbl = 0;											// Nms수신Out
	memset(NmsSendBuffer, 0, sizeof(NmsSendBuffer));			// Nms송신Buffer
	memset(NmsRecvBuffer, 0, sizeof(NmsRecvBuffer));			// Nms수신Buffer
	memset(NmsRecvBufferTbl, 0, sizeof(NmsRecvBufferTbl));		// Nms수신BufferTbl

	NmsErrorSendFlg = FALSE;									// 장애Send유무

	HotKeyFlag = FALSE;											// U8100 AP 변경내역 #10 - HotKey(퀵버튼) 추가

	StrAllSpace(m_RXHOSTMSG);

	fnAPP_Tran_Init();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn():return");
}

CTranCmn::~CTranCmn()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~CTranCmn()");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~CTranCmn():return");
}


BEGIN_MESSAGE_MAP(CTranCmn, CWnd)
	//{{AFX_MSG_MAP(CTranCmn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTranCmn message handlers
/////////////////////////////////////////////////////////////////////////////
// Set Owner
void CTranCmn::SetOwner(CWinAtmCtrl *pOwner, CNHGuros* pNHGuros, CHandleOCR* pNHOcr)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~SetOwner()");

#include "..\..\H\Common\ClassSetOwner.h"

	m_pNHGuros = pNHGuros;
	m_pNHOcr = pNHOcr;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~SetOwner():return");
	return;
}

/////////////////////////////////////////////////////////////////////////////
//	초기화함수
/////////////////////////////////////////////////////////////////////////////
// 변수초기화
int CTranCmn::fnAPP_InitializeDS()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeDS()");

/////////////////////////////////////////////////////////////////////////////
//	처리결과
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->TranResult = FALSE;								// Host결과
/////////////////////////////////////////////////////////////////////////////
//	거래상태변수
/////////////////////////////////////////////////////////////////////////////
	TranAvail = 0;												// 전체거래상태
	WithAvail = 0;												// 출금거래상태
	DepAvail = 0;												// 입금거래상태
	PbmAvail = 0;												// 통장거래상태
	A4pAvail = 0;												// A4p거래상태
	
/////////////////////////////////////////////////////////////////////////////
//	메뉴변수
/////////////////////////////////////////////////////////////////////////////
	MenuIndex = 0;												// 메뉴Index
	MenuIndexSub = 0;											// 메뉴IndexSub
	MenuIndexNext = 0;											// 메뉴IndexNext
	MenuAvail = MENU_NULL;										// 메뉴가능상태
	MenuMuTran = MENU_NULL;										// 메뉴무매체거래상태
	MenuBtnCnt = 0;												// 메뉴선택가능한버튼수
	MenuBtnIndex = 0;											// 메뉴자동선택가능한최종버튼값
	MenuOutGoingMode = 0;										// CREDIT(WITH):1,SAVING(WITH):2,CHECKING(WITH):3,CREDIT(INQ):4,SAVING(INQ):5,CHECKING(INQ):6
	MenuFingerMode = 0;											// FINGER:1
	MenuIrdaMode = 0;											// IRDA:1
	MenuKCashMode = 0;

	BrandSelectIndex = 0;										// 연속거래시 선택된 Brand
	EMVReadFlg = FALSE;											// EMV READ FLAG
	CardBankFlag = 0;
	CardServiceFlag = 0;
	ExtraServiceFlag = 0;
	m_EMVCancelFlg = FALSE;

	TranProc = TRAN_NULL;										// 거래처리
	TranCode = 0;												// 거래코드
	TranCode2 = 0;												// 거래코드2

	TranProcSave = TRAN_NULL;									// Tran거래처리저장
	TranCodeSave = 0;											// TranCode저장
	TranCode2Save = 0;											// TranCode2저장
	TranEncCode = 0;
/////////////////////////////////////////////////////////////////////////////
//	한도금액변수
/////////////////////////////////////////////////////////////////////////////
	MoneyMaxOut = 0;											// 방출가능한도금액(만원)
	MoneyMaxOutCnt = 0;											// 방출가능매수
	CashMaxOutCnt = 0;											// 현금방출가능매수
	CheckMaxOutCnt = 0;											// 수표방출가능매수
	CheckLowMaxOutCnt = 0;										// 수표방출최소매수
	CheckHighMaxOutCnt = 0;										// 수표방출최대매수

	TMoneyMaxOut = 0;											// 이체가능한도금액(원)

	MoneyMaxIn = 0;												// 입금가능한도금액(만원)
	CashMaxIn = 0;												// 현금입금가능금액
	CheckMaxInCnt = 0;											// 수표입금가능매수

/////////////////////////////////////////////////////////////////////////////
//	입력변수
/////////////////////////////////////////////////////////////////////////////
	memset(&Accept, 0, sizeof(Accept));							// 입력변수

	memset(Accept.PbConfirmNum, 0x20, sizeof(Accept.PbConfirmNum)); // 통장거래승인번호
	Accept.PbConfirmNumSize = 0;								// 통장거래승인번호길이	

	Accept.MuAccountNumSize = 0;								// 무매체계좌번호 길이
	memset(Accept.MuAccountNum, ' ', sizeof(Accept.MuAccountNum));	
																// 무매체계좌번호
	memset(Accept.CardReadTime, ' ', sizeof(Accept.CardReadTime)); //카드 리딩 시간	

	memset(Accept.PassWord, ' ', sizeof(Accept.PassWord));		// 비밀번호
	memset(Accept.PbPassWord, ' ', sizeof(Accept.PbPassWord));	// 통장비밀번호
	memset(Accept.MuPassWord, ' ', sizeof(Accept.MuPassWord));	// 무매체비밀번호
	memset(Accept.KCashPassWord, 0, sizeof(Accept.KCashPassWord));
	memset(Accept.SlockPassWord, 0, sizeof(Accept.SlockPassWord));	
	memset(Accept.UserId, ' ', sizeof(Accept.UserId));	
																// K-CASH비밀번호

	memset(Accept.DesPassWord, 0, sizeof(Accept.DesPassWord));	// Des비밀번호		// DES OutGoing거래 2004.05.31
	memset(Accept.DesEncryptPin, 0, sizeof(Accept.DesEncryptPin));	
																// Des Encrypt PIN	// DES OutGoing거래 2004.05.31
/////////////////////////////////////////////////////////////////////////////
//	금액관련변수
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.Money, '0', sizeof(Accept.Money));			// 금액(만원), 금액(만천원)
	memset(Accept.CashMoney, '0', sizeof(Accept.CashMoney));	// 금액(만원권)/현금입금
	memset(Accept.CheckMoney, '0', sizeof(Accept.CheckMoney));	// 금액(수표권)/수표입금
	memset(Accept.Cash50T_Money, '0', sizeof(Accept.Cash50T_Money));	
	memset(Accept.Cash10T_Money, '0', sizeof(Accept.Cash10T_Money));	
	
	memset(Accept.MotherCheckMoneyM, '0', sizeof(Accept.MotherCheckMoneyM));	
																// 금액(수표권)/수표입금중자행자점금액
	memset(Accept.MotherCheckMoneyO, '0', sizeof(Accept.MotherCheckMoneyO));	
																// 금액(수표권)/수표입금중자행타점금액
	memset(Accept.OtherCheckMoney, '0', sizeof(Accept.OtherCheckMoney));	
																// 금액(수표권)/수표입금중타행금액
	Accept.MoneyInKind = TRAN_DEP_NOT;							// 입금권종

	memset(Accept.RemainRemMoney, '0', sizeof(Accept.RemainRemMoney));		
																// 차액송금액
	memset(Accept.BalanceRemMoney, '0', sizeof(Accept.BalanceRemMoney));	
																// 차액입금액
/////////////////////////////////////////////////////////////////////////////

	memset(Accept.BankNum, ' ', sizeof(Accept.BankNum));		// 은행
	Accept.InAccountNumSize = 0;								// 수취계좌번호길이
	memset(Accept.InAccountNum, ' ', sizeof(Accept.InAccountNum));	
																// 차액입금계좌번호길이
	Accept.RemitAccountNumSize = 0;								// 수취계좌번호길이
	memset(Accept.RemitAccountNum, ' ', sizeof(Accept.RemitAccountNum));	
																// 차액입금계좌번호
	
	memset(Accept.ChangePwd, '0', sizeof(Accept.ChangePwd));	// 비밀번호(변경)
	memset(Accept.ChangePwdConfirm, '0', sizeof(Accept.ChangePwdConfirm));
																// 비밀번호(변경확인)
	memset(Accept.ChangeICPwd, 0, sizeof(Accept.ChangeICPwd));
																// K-CASH비밀번호(변경)
	memset(Accept.ChangeICPwdConfirm, 0, sizeof(Accept.ChangeICPwdConfirm));
																// K-CASH비밀번호(변경확인)
	
	Accept.LoanAccountNumSize = 0;								// 대출계좌번호 길이
	memset(Accept.LoanAccountNum, ' ', sizeof(Accept.LoanAccountNum));	
																// 대출계좌번호
	Accept.JiroTranCode = 0;									// 지로세부거래코드
	memset(Accept.JiroTranOrgCode, 0x00, sizeof(Accept.JiroTranOrgCode));		// 지로납부기관코드
	memset(Accept.JiroNum, ' ', sizeof(Accept.JiroNum));		// 지로번호
	Accept.JiroElecNumSize = 0;									// 전자납부번호길이
	memset(Accept.JiroElecNum, '0', sizeof(Accept.JiroElecNum));// 전자납부번호
	memset(Accept.JiroSelItem, '0', sizeof(Accept.JiroSelItem));// 지로거래시 선택내역
	memset(Accept.JiroTranDetails, ' ', sizeof(Accept.JiroTranDetails));// 지로거래시 수신납부내역
	memset(Accept.JiroMoney, '0', sizeof(Accept.JiroMoney));	// 금액(만원), 금액(만천원)

	Accept.SerialCount = 0;										// 연속거래카운트
//	Accept.SerialTranFlag = FALSE;								// 연속거래여부(출금,이체)
	Accept.SlipChoiceFlag = FALSE;								// 명세표인자여부
	Accept.A4ChoiceFlag = FALSE;								// A4프린터인자여부

	memset(Accept.RegCheckNum, '0', sizeof(Accept.RegCheckNum));// 등록수표번호
	memset(Accept.RegCheckCount, '0', sizeof(Accept.RegCheckCount));
																// 등록수표매수

	memset(Accept.JuminNum, '0', sizeof(Accept.JuminNum));		// 주민번호
	memset(Accept.TelNum, '0', sizeof(Accept.TelNum));			// 전화번호
	Accept.TelNumSize = 0;										// 전화번호길이

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.CMSCode, ' ', sizeof(Accept.CMSCode));		// CMS(직금/고유번호)
	memset(Accept.CMSOrgName, ' ', sizeof(Accept.CMSOrgName));	// CMS(기관명)

	memset(Accept.AutoDate, 0, sizeof(Accept.AutoDate));		// 자동이체일
	memset(Accept.AutoCycle, 0, sizeof(Accept.AutoCycle));		// 이체주기

	memset(Accept.InquiryDate, ' ', sizeof(Accept.InquiryDate));// 기록조회일
	memset(Accept.RemitName, ' ', sizeof(Accept.RemitName));	// 송금인명
	Accept.RemitNameFlag = FALSE;								// 송금인입력플래그
	Accept.MuTranFlag = FALSE;									// 신한-무매체플래크
	Accept.InQuiryCycle = '0';									// 조회기간구분(평일,휴일)
	Accept.MuBankTranBrandFlag = FALSE;									// 신한-무매체플래크	
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.StocksCode, ' ', sizeof(Accept.SelStockIndex));	// 종목코드
	memset(Accept.StocksReqNum, ' ', sizeof(Accept.StocksReqNum));	// 요청수량
	memset(Accept.ByOnMargin, ' ', sizeof(Accept.ByOnMargin));		// 증거금
	memset(Accept.LoanMoney, ' ', sizeof(Accept.LoanMoney));		// 대출금
	memset(Accept.RefundAccountNum, ' ', sizeof(Accept.RefundAccountNum));	// 환불계좌
	memset(Accept.NextInqCondition, ' ', sizeof(Accept.NextInqCondition));	// 다음조회조건
	memset(Accept.StocksDate, ' ', sizeof(Accept.StocksDate));					// 청약일
	memset(Accept.RefundAccountName, ' ', sizeof(Accept.RefundAccountName));	// 환불계좌명
	memset(Accept.RefundDate, ' ', sizeof(Accept.RefundDate));					// 환불일자
	memset(Accept.StocksListDate, ' ', sizeof(Accept.StocksListDate));			// 상장일자
	memset(Accept.InAccountName, ' ', sizeof(Accept.InAccountName));			// 입금계좌명
	memset(Accept.AccountNum, ' ', sizeof(Accept.AccountNum));					// 계좌번호
	memset(Accept.HostReqNum, ' ', sizeof(Accept.HostReqNum));					// HOST원거래번호
	memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));				// 해외카드 구분
	memset(m_yymmddhhmmss,	0, sizeof(m_yymmddhhmmss));			// CARD Read Time

//	송수신변수
/////////////////////////////////////////////////////////////////////////////
	AddSerialFlg = FALSE;										// 일련번호증가Flg
	TranSend = 0;												// N차송신여부

	SendLength = 0;												// 송신Length
	SaveSendLength = 0;											// 저장송신Length
	RecvLength = 0;												// 수신Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// 송신Buffer
	memset(RecvBuffer, 0, sizeof(RecvBuffer));					// 수신Buffer
	memset(SaveSendBuffer, 0, sizeof(SaveSendBuffer));			// 저장송신Buffer

	memset(&PSPRecvData, 0, sizeof(PSPRecvData));				// DATA부/명세프린트(거래)
	memset(&PPPRecvData, 0, sizeof(PPPRecvData));				// DATA부/통장부프린트(거래)
	memset(&PPPRecvDataNon, 0, sizeof(PPPRecvDataNon));				// DATA부/통장부프린트(거래) - Non
	memset(&PPPRecvDataKb, 0, sizeof(PPPRecvDataKb));				// DATA부/통장부프린트(거래) - KB #N0227
	memset(&PPWRecvData, 0, sizeof(PPWRecvData));				// DATA부/통장M/S WRITE(거래)
	memset(&PCWRecvData, 0, sizeof(PCWRecvData));				// DATA부/카드M/S WRITE(거래)
	memset(&PMERecvData, 0, sizeof(PMERecvData));				// DATA부/메세지편집표시(거래종류별)
	memset(&PMDRecvData, 0, sizeof(PMDRecvData));				// DATA부/메세지표시(거래)
	
/////////////////////////////////////////////////////////////////////////////
//	송신버퍼
/////////////////////////////////////////////////////////////////////////////
	
	memset(&m_TXECASH,	0, sizeof(TXRXECASH));					// 송신
	memset(&m_RXECASH,	0, sizeof(TXRXECASH));					// 수신
	memset(&m_RD,	    0, sizeof(RXDISPLAYDATA));				
	memset(&m_RecvEmvrcvData,	    0, sizeof(RECVEMVRCVDATA));	
	fnAPP_ClearArrDataField();
	m_pNetWork->ClearRemoteData();
	RegSetStr(_REGKEY_REMOTECTRL, "BID_RECV_RESULT", "NORMAL");	
/////////////////////////////////////////////////////////////////////////////
//	수신버퍼
/////////////////////////////////////////////////////////////////////////////
	memset(&RecvComm,	0, sizeof(RECVCOMM));					// 수신헤더
	memset(HostReserved, NULL, sizeof(HostReserved));			// HOST예비
	memset(SaveJiroNum, ' ', sizeof(SaveJiroNum));				// 지로번호저장

/////////////////////////////////////////////////////////////////////////////
//	수신후처리변수
/////////////////////////////////////////////////////////////////////////////
	ChoiceTran = TRAN_NULL;										// 조회후선택된거래
	TranResultNGCode = 0;										// Tran거래결과NG코드
	PbNextTranFlag = FALSE;										// 통장 다음거래 통보플래그
	DepNextTranFlag = FALSE;									// 연속거래_입금_Flag
	PbNextPageFlag = FALSE;										// 통장 다음페이지 통보플래그
	InqNextTranFlag = FALSE;									// 조회 다음목록 통보 플래그
	RetractFlag = FALSE;										// 매체회수지시Flag
	KCashHostICFlag = FALSE;									// K-CASH Host IC Flag
	UpdateKeyParam = FALSE;										// 파라미터갱신여부
	ContinueTranCount = 0;										// 연속거래카운트
	InqNextTranCount = 0;										// 조회횟수
	DepNextTranCount = 0;

	SeedErrorCode = 0;											// 암복화에러
	PbPrintFlag	= FALSE;										// 통장기장여부
	GhostAccountFlag = FALSE;									// 가상계좌여부
	PbPrintAfterMsgFlag = FALSE;
/////////////////////////////////////////////////////////////////////////////
//	매체변수
/////////////////////////////////////////////////////////////////////////////
	DevEnDisable = 0;											// 카드통장En/Disable
	ICCardTran = MENU_NULL;
	MSCardTran = MENU_NULL;
	CardTran = MENU_NULL;										// Card거래상태변수
	PbTran = MENU_NULL;											// Pb거래상태변수
	PbKind = 0;													// 은행통장종류
	CardBank = 0;												// 은행카드종류
	PbBank = 0;													// 통장 Flag
	CardService = 0;											// 신용카드종류
	CardNew = 0;												// 신규카드종류
	BranchKind = 0;												// 자타구분
	MenuFICMode	= MENU_NULL;									// 금융IC카드거래변수
	MenuEMVMode = MENU_NULL;									// EMV카드거래변수
	MenuEPBMode = MENU_NULL;									// 전자통장거래변수
	MenuRFDMode = MENU_NULL;									// RF거래상태변수
	FICMyBiFlg = FALSE;											// 금융IC카드 MyBi여부
	FallBackMenuFICMode = FALSE;								// 금융IC 카드FALLBACK상태변수
	FallBackEMVMode = FALSE;									// EMV IC 카드거래상태변수
	EMVReadFlg = FALSE;											// EMV READ FLAG
	m_EMVCancelFlg = FALSE;
	FallBackCnt = 0x20;											// Default : Space
	BrandSelectIndex = 0;										// 연속거래시 선택된 Brand
	KCashPingChangFlag = FALSE;									// PIN등록/변경변수

	CardOnEMV = FALSE;											// EMV가능 신용카드
	EMVAuthError = FALSE;										// EMV인증에러

	m_pDevCmn->FICAppletType = 0;	

	memset(&CardData, 0, sizeof(CardData));						// 카드자료 
	pCardData2 = (MCISO2*)CardData.ISO2Buff;					// 2TR Pointer
	pCardData3 = (MCISO3*)CardData.ISO3Buff;					// 3TR Pointer
	memset(&PbMsData, 0, sizeof(PbMsData));						// 통장자료
	memset(&PbBarData, 0, sizeof(PbBarData));					// 통장 Bar Info
	pPbMsData = (PBMS*)PbMsData.sBuf;							// Pb Pointer
	memset(&FingerData, 0, sizeof(FingerData));					// 지문자료
	memset(&BarData, 0, sizeof(BarData));						// BarCode자료

	CardEmbossScanFlg = FALSE;									// 카드엠보스스캔유무
	CardImprintFlg = FALSE;										// 카드임프린트유무
	CardPowerOnFlg = FALSE;										// 카드접속유무(전자화폐)
	PbDataCnt = 0;												// Pb출력길이
	memset(PbData, 0, sizeof(PbData));							// Pb출력영역

	EjrIndexFlg = FALSE;										// 전자저널유무
	memset(&EjrData, ' ', sizeof(EjrData));						// 전자저녈자료
	memset(&EjrAmountData, 0, sizeof(EjrAmountData));			// 전자저녈금액관련자료

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역
	SlipPrintFlg = FALSE;										// 명세표인자유무
	JnlPrintFlg = FALSE;										// 저널인자유무
	
	MaterialOutFlg = 0;											// 매체방출유무
	MaterialOutCheckFlg = 0;									// 매체방출후수취유무
	MoneyOutFlg = 0;											// 현금수표방출유무
	MoneyOutCheckFlg = 0;										// 현금수표방출후수취유무

	SprOutRetryFlg = FALSE;										// 명세표방출재시도플래그

	Accept.FICSelectAccountIndex = 0;							// 금융IC 선택계좌인덱스

	A4PrintCnt = 0;												// 출력할 A4Print Line Count(PMERecvData.y값)
/////////////////////////////////////////////////////////////////////////////
//	시재관련변수
/////////////////////////////////////////////////////////////////////////////
	CashCountFlg = FALSE;										// 출금현금계수유무
	CheckCountFlg = FALSE;										// 출금수표계수유무

	CashInCountFlg = FALSE;										// 입금현금계수유무
	CashRejectFlg = FALSE;										// 입금현금반환유무
	CashRejectAFlg = FALSE;										// 입금현금초과반환유무
	CheckInCountFlg = FALSE;									// 입금수표계수유무
	CheckRejectFlg = FALSE;										// 입금수표반환유무
	CheckRejectAFlg = FALSE;									// 입금수표오류반환유무
	CheckRejectKind = 0;										// 입금수표반환사유

	memset(&RejectData, 0, sizeof(RejectData));					// 회수내역
	memset(&CashData, 0, sizeof(CashData));						// 입금현금내역
	memset(CheckInfoData, '0', sizeof(CheckInfoData));			// 입금수표내역
	memset(Accept.PbPageInfo, 0, sizeof(Accept.PbPageInfo));	// 통장 Bar Info //#0039
	CheckInfoStats = NORMAL;									// 수표입금상태
	CheckInfoCnt = 0;											// 수표입금매수
	CheckInfoAmt = 0;											// 수표입금금액
	memset(CheckPrintData, NULL, sizeof(CheckPrintData));		// 수표배서내용편집
	CheckInPrintFlg = FALSE;									// 수표입금정보인자유무
	SumClearFlag = FALSE;
/////////////////////////////////////////////////////////////////////////////
//	종료변수
/////////////////////////////////////////////////////////////////////////////
	CancelProcNum = 0;											// 취소처리N번실행변수
	CancelProcCount = ' ';										// 취소처리전진행번호 
	CancelDepRejectFlg = FALSE;									// 입금반환Flg
	CancelDepRejectProcFlg = FALSE;								// 입금반환처리Flg
	CancelCheckClearFlg = FALSE;								// 수표정보초기화Flg
	CancelPrintFlg = FALSE;										// 취소인자Flg
	CancelDepPrintFlg = TRUE;									// 취소입금인자Flg(초기:실행무)
	CancelWithSendFlg = FALSE;									// 취소출금전송Flg(초기:실행무) v01-00-00-#301
	CancelWithProcFlg = FALSE;									// 취소출금처리Flg
	CancelDepSendFlg = FALSE;									// 취소입금전송Flg(초기:실행무)
	CancelDepProcFlg = FALSE;									// 취소입금처리Flg(초기:실행무)
	CancelKCashSendFlg = TRUE;									// K-CASH취소출금전송Flg(초기:실행무)
	CancelKCashProcFlg = FALSE;									// K-CASH취소출금처리Flg


	CancelHIPassLoadFlg = FALSE;								// 하이패스 거래실패시 Complete 처리Flag	#N0266

	CancelHipassPrintFlg = TRUE;								// 하이패스 취소 FLG

	if(m_pProfile->DEVICE.MachineType == U3100K)
		m_pDevCmn->SprRowOption = SPR_MAX_ROW;
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
		m_pDevCmn->SprRowOption = SPR_MAX_ROW_U8100;
	else
		m_pDevCmn->SprRowOption = SPR_MAX_ROW;

	m_pDevCmn->bNearBranchSelectBtn = FALSE;   //#N0172
															
	memset(m_pDevCmn->VODCapturePath, 0x00, sizeof(m_pDevCmn->VODCapturePath));
	m_pDevCmn->RealVodFile	= "";								
	TranStartByScr = NULL;										
	HotKeyFlag = FALSE;											
	m_pDevCmn->TTSEngineLoadFlg = FALSE;						
	m_pDevCmn->PBBarLineReading = FALSE;


	// #NICCM01 EMV Lib
	m_pDevCmn->IsFallBackTran = FALSE;							// #NICCM01 Emv Transaction
	m_pDevCmn->GenerateAC = 0;									// #NICCM01 Emv Transaction								
	m_pDevCmn->Declined = FALSE;								// #NICCM01 ERROR 미존재 시 처리에 DECLINE 처리추가	
	SourceAccount = S_OTHER;							    	// #NICCM01
	
	m_pDevCmn->nEmvStep = 0;
	m_pDevCmn->nEmvStepCode = 0;

	m_pDevCmn->m_nAtmsCommand = ATMS_CMD_IDLE;	
	m_pDevCmn->fnEMVLib_Initial();								// #NICCM01
	
/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeDS():return");
	return T_OK;
}

// 연속거래변수초기화
int CTranCmn::fnAPP_InitializeSerialTranDS()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeSerialTranDS()");

/////////////////////////////////////////////////////////////////////////////
//	금액관련변수
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.Money, '0', sizeof(Accept.Money));			// 금액(만원), 금액(만천원)
	memset(Accept.CashMoney, '0', sizeof(Accept.CashMoney));	// 금액(만원권)/현금입금
	memset(Accept.CheckMoney, '0', sizeof(Accept.CheckMoney));	// 금액(수표권)/수표입금
	memset(Accept.MotherCheckMoneyM, '0', sizeof(Accept.MotherCheckMoneyM));
																// 금액(수표권)/수표입금중자행자점금액
	memset(Accept.MotherCheckMoneyO, '0', sizeof(Accept.MotherCheckMoneyO));
																// 금액(수표권)/수표입금중자행타점금액
	memset(Accept.OtherCheckMoney, '0', sizeof(Accept.OtherCheckMoney));	
																// 금액(수표권)/수표입금중타행금액
	Accept.MoneyInKind = TRAN_DEP_NOT;							// 입금권종

	memset(Accept.RemainRemMoney, '0', sizeof(Accept.RemainRemMoney));		
																// 차액송금액
	memset(Accept.BalanceRemMoney, '0', sizeof(Accept.BalanceRemMoney));	
																// 차액입금액
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PassWord, ' ', sizeof(Accept.PassWord));		// #N0223 조회후 거래 시 비밀번호 초기화
/////////////////////////////////////////////////////////////////////////////
//	시재관련변수
/////////////////////////////////////////////////////////////////////////////
	CashCountFlg = FALSE;										// 출금현금계수유무
	CheckCountFlg = FALSE;										// 출금수표계수유무

	CashInCountFlg = FALSE;										// 입금현금계수유무
	CashRejectFlg = FALSE;										// 입금현금반환유무
	CashRejectAFlg = FALSE;										// 입금현금초과반환유무
	CheckInCountFlg = FALSE;									// 입금수표계수유무
	CheckRejectFlg = FALSE;										// 입금수표반환유무
	CheckRejectAFlg = FALSE;									// 입금수표오류반환유무
	CheckRejectKind = 0;										// 입금수표반환사유

	memset(&RejectData, 0, sizeof(RejectData));					// 회수내역
	memset(&CashData, 0, sizeof(CashData));						// 입금현금내역
	memset(CheckInfoData, '0', sizeof(CheckInfoData));			// 입금수표내역

	CheckInfoStats = NORMAL;									// 수표입금상태
	CheckInfoCnt = 0;											// 수표입금매수
	CheckInfoAmt = 0;											// 수표입금금액
	memset(CheckPrintData, NULL, sizeof(CheckPrintData));		// 수표배서내용편집
	CheckInPrintFlg = FALSE;									// 수표입금정보인자유무
	MenuDoingFlag = FALSE;										// Defualt FALSE
	MainTranDoingFlag = FALSE;									// 연속거래변수 초기화
	TranSend = 0;												// 연속거래시 2TR거래분기로초기화

	GhostAccountFlag = FALSE;									// 가상계좌여부

	AddSerialFlg = FALSE;										// 일련번호증가Flg

	CancelHipassPrintFlg = TRUE;								// 하이패스 충전 거래 취소 #N0266
/////////////////////////////////////////////////////////////////////////////

	memset(m_pDevCmn->OrignalErrorCode, 0, sizeof(m_pDevCmn->OrignalErrorCode));
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeSerialTranDS():return");
	return T_OK;
}

