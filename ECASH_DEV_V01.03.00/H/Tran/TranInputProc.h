/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranInputProc_H_INCLUDED_)
#define _TranInputProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	한도금액변수
/////////////////////////////////////////////////////////////////////////////
	int		MoneyMaxOut;				// 방출가능한도금액(만원)
	int		MoneyMaxOutCnt;				// 방출가능매수
	int		CashMaxOutCnt;				// 현금방출가능매수
	int		CheckMaxOutCnt;				// 수표방출가능매수
	int		CheckLowMaxOutCnt;			// 수표방출최소매수
	int		CheckHighMaxOutCnt;			// 수표방출최대매수

	int		TMoneyMaxOut;				// 이체가능한도금액(원)

	int		MoneyMaxIn;					// 입금가능한도금액(만원)
	int		CashMaxIn;					// 현금입금가능금액
	int		CheckMaxInCnt;				// 수표입금가능매수
	int		HotKeyFlag;					// U8100 개발
/////////////////////////////////////////////////////////////////////////////
//	입력변수
/////////////////////////////////////////////////////////////////////////////
struct	tagAccept						// 입력변수
{
	int		MuAccountNumSize;			// 무매체계좌번호길이
	char	MuAccountNum[16];			// 무매체계좌번호
	char	PbConfirmNum[6];			// 통장거래승인번호		
	int		PbConfirmNumSize;			// 통장거래승인번호길이	
	char	CardReadTime[12];			// 거래매체 리딩 시간

	char	PassWord[4];				// 비밀번호
	char	PassWordEnc[64];			// 암호화된 비밀번호
	char	PbPassWord[6];				// 폰뱅킹비밀번호
	char	MuPassWord[8];				// 무매체비밀번호
	char	KCashPassWord[8];			// K-CASH비밀번호
	char	SlockPassWord[8];			// Offline Lock 비밀번호
	char	UserId[16];					// User Id
	char	UserPassWord[16];			// User Pwd
	char	MPINConfirmNum[6];			// 모바일PIN번호  #N0182

	char	DesPassWord[12];			// DES OutGoing거래 2004.05.31
	char	DesEncryptPin[16];			// DES OutGoing거래 2004.05.31

/////////////////////////////////////////////////////////////////////////////
//	금액관련변수
/////////////////////////////////////////////////////////////////////////////
	char	Money[15];					// 금액(만원), 금액(만천원)
	char	CashMoney[15];				// 금액(만원권)/현금입금
	char	CheckMoney[15];				// 금액(수표권)/수표입금
	char	Cash10T_Money[15];			// 금액(만원권)		
	char	Cash50T_Money[15];			// 금액(5만원권)	
	char	MotherCheckMoneyM[15];		// 금액(수표권)/수표입금중자행자점금액
	char	MotherCheckMoneyO[15];		// 금액(수표권)/수표입금중자행타점금액
	char	OtherCheckMoney[15];		// 금액(수표권)/수표입금중타행금액
	int		MoneyInKind;				// 입금권종
	int		ChargeCommission;			// V02-10-02-R1 타행입금 수수료 2006.12.06
	
	int		CheckCount;					// 출금수표매수
	char	CheckNumber[10];			// 은행코드(2)+출금수표번호(8)

	char	RemainRemMoney[15];			// 차액송금액
	char	BalanceRemMoney[15];		// 차액입금액
/////////////////////////////////////////////////////////////////////////////
	BYTE	PbPageInfo[4];				// 통장페이지정보:농협
	char	BankNum[3];					// 은행번호

	int		AccoutnNumSize;				// 계좌번호길이
	char	AccountNum[16];				// 계좌번호

	int     InAccountNumSize;			// 수취계좌번호길이
	char	InAccountNum[16];			// 수취계좌번호

	int		RemitAccountNumSize;		// 차액입금계좌번호길이
	char	RemitAccountNum[16];		// 차액입금계좌번호

	char	JuminNum[13];				// 주민번호
	char	TelNum[20];					// 전화번호
	int		TelNumSize;					// 전화번호길이

	char	ChangePwd[4];				// 비밀번호(변경)
	char	ChangePwdConfirm[4];		// 비밀번호(변경확인)
	char	ChangeICPwd[8];				// K-CASH비밀번호(변경)
	char	ChangeICPwdConfirm[8];		// K-CASH비밀번호(변경확인)

	int		LoanAccountNumSize;			// 대출계좌번호길이
	char	LoanAccountNum[16];			// 대출계좌번호

	int		JiroTranCode;				// 지로세부거래코드
	char	JiroTranOrgCode[9];			// 지로납부 기관코드
	char	JiroNum[7];					// 지로번호
	int		JiroElecNumSize;			// 전자납부번호길이
	char	JiroElecNum[19];			// 전자납부번호
	char	JiroSelItem[5];				// 일반지로 거래시 선택내역
	char	JiroBillCount[2];			// 지로 선택건수
	char	JiroPayMonth[2];			// 지로 선택건수

	BYTE	JiroTranDetails[1024];		// 지로납부 수신내역데이타
	char	JiroMoney[8];				// 금액(만원), 금액(만천원)

	int		ETicketIndex;				// 전자상품권 대분류 선택 #N0142
	int		ETicketConfirmFlg;			// 
	int		ETicketDetail;				// 전자상품권 소분류 선택 #N0142
	int		ETicketPayWay;				// 전자상품권 결제 방법 #N0142
	int		ETicketAmount;				// 전자상품권 금액 #N0142
	int		ETicketChange;				// 전자상품권 잔돈 #N0142
	char	SendTelNum[12];				// 전자상품권 보내는 전화번호 #N0142
	char	SendMsg[50];				// 전자상품권 보내는 전화번호 #N0142
	char	ETicketConfirmNum[8];		// 쿠폰 승인번호
	char	ETicketNum[12];				// 거래일련번호
	char	ETicketSerialNum[16];		// 쿠폰번호

	char	RegCheckNum[8];				// 등록수표번호
	char	RegCheckCount[5];			// 등록수표매수

	int		SerialCount;				// 연속거래카운트
//	int		SerialTranFlag;				// 연속거래여부(출금,송금,입금)
	int		SlipChoiceFlag;				// 명세표인자여부
	int		A4ChoiceFlag;				// A4프린터인자여부

/////////////////////////////////////////////////////////////////////////////
	char	CMSCode[20];				// CMS(직금/고유번호)
	int		CMSCodeSize;				// CMS길이
	char	CMSOrgName[30];				// CMS(기관명)

	int		LostAccountNumSize;			// 분실계좌번호길이
	char	LostAccountNum[16];			// 분실계좌번호

	char	AutoDate[3];				// 자동이체일
	char	AutoCycle[3];				// 이체주기

	char	InquiryDate[8];				// 기록조회일
	char	InquiryDateEnd[8];			// 기록조회일
	char	InQuiryCycle;				// 조회기간구분(평일,휴일)
	char	RemitName[20];				// 송금인명

	int		RemitNameFlag;				// 송금인입력플래그

/////////////////////////////////////////////////////////////////////////////
	int		FICSelectAccountIndex;		// 금융IC선택계좌인덱스	// 2004.02.03

/////////////////////////////////////////////////////////////////////////////
	int		SelStockIndex;				// 증권종목선택 인덱스
	char	StocksCode[20];				// 종목코드
	char	StocksReqNum[15];			// 요청수량
	char	ByOnMargin[15];				// 증거금
	char	LoanMoney[15];				// 대출금
	char	RefundAccountNum[16];		// 환불계좌
	char	NextInqCondition[20];		// 다음조회조건

	char	StocksDate[8];				// 청약일
	char	RefundAccountName[20];		// 환불계좌명
	char	RefundDate[8];				// 환불일자
	char	StocksListDate[8];			// 상장일자
	char	InAccountName[20];			// 입금계좌명
	char	HostReqNum[12];				// HOST 원거래번호(취소거래시 사용)

	char	MuConfirmNum[2];			// 해외카드 구분
	int		MuTranFlag;
	int		MuBankTranBrandFlag;

	char	SMSCode[20];				// Phone SMS
	int		SMSCodeSize;				// SMS길이

	int		AuthFlag;					// 인증 Y,N 
	int		ProcessStep;				// 진행카운트 
	int		InComeCode[10];				// 수입금액
	int		JobCode[10];				// 직업코드

	int		Index;

	int		LoanFlag;
	char	LoanCode[5];				// 론코드
	char	LoanGubunCode[10];				// 론코드
	char	LoanType[3];
	char	LoanMonth[3];
	char	LoanName[20];				//sj.Kim  2011.03.10

	char	CrmSrcFlag[4];
	char	CrmStartFlag[6];
	char	CrmMenuFlag[6];
	char	CrmCode[4];
	char	CrmDummy[100];

	char	InTelNum[16];

	int		EasyOneNextInqFlg;			//이지원 다음 조회 선택 여부.#N0174
	char	EasyOneName[25];			//이지원 수취인 성명
	char	EasyOneOrgName[35];			//이지원 수취 기관
	union	
	{
		struct {
			char	UserName[12];					// 성명
			char	UserAge[3];						// 나이
			char	UserGender[1];					// 성별
			char	UserHeight[3];					// 키
			char	UserWeight[3];					// 몸무게
			char	Date[8];						// 날짜
			char	Time[6];						// 시간
			char	TeleCom[3];						// 통신사
			char	ADGubunCode[2];					// 인터렉티브광고 구분코드
			char	ADName[20];						// 인터렉티브광고 이름
		} InterAD;

		struct {
			char	GameName[20];					// 모바일게임 다운로드 이름
			char	TeleCom[3];						// 통신사
			char	GameCompanyCode[3];				// 모바일게임 다운로드 회사코드
			char	GameServiceCode[8];				// 모바일게임 다운로드 서비스코드
		} MobileGame;

		struct {
			//  Hi-Pass충전업무개발	#N0266
			char	AvailLoadAmt[8];				// Hi-Pass 충전가능금액
			char	LoadAmt[8];						// Hi-Pass 충전금액
			char	CardNum[16];					// Hi-Pass 카드번호
		} HiPass;

	};

} Accept;
	
	// Operations - EMV 추가
	CString	SourceAccount;				// Source Account		// V09-33-00-#01
/////////////////////////////////////////////////////////////////////////////
// Operations
public:
	int		fnAPP_FNGAccept();									// Menu Finger Accept
	int		fnAPP_RDCtrlPasswdAccept();							// User Sms Pwd
	int		fnAPP_RDCtrlPasswdAccept2();						// User pwd
	int		fnAPP_RDCtrlSMSAccept();
	int		fnAPP_RDCtrlOpenConfirm();						// UserID
	int		fnAPP_RDCtrlOpenConfirm2();						// UserID

/////////////////////////////////////////////////////////////////////////////
//	기기관리함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_AcceptOpPassword();
	int		fnAPP_AcceptManagerPassword();
	int		fnAPP_AcceptEveryDayManagerPassword();

/////////////////////////////////////////////////////////////////////////////
//	한도금액설정함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_CalcMoneyMaxOut();							// 방출한도구하기
	int		fnAPP_CalcMoneyMaxOutATM();							// ATM기종방출한도구하기
	int		fnAPP_CalcMoneyMaxOutCD();							// CD기종방출한도구하기
	int		fnAPP_CalcCheckLowMaxOutCnt(int MoneyOut);			// 수표최소매수구하기
	int		fnAPP_CalcCheckHighMaxOutCnt(int MoneyOut);			// 수표최대매수구하기
	int		fnAPP_CalcMoneyMaxTrans();							// 이체한도구하기
	int		fnAPP_CalcMoneyMaxIn();								// 입금한도구하기
	int		fnAPP_CalcCashMaxIn();								// 현금입금한도구하기
	int		fnAPP_CalcCashMaxInAmount();
	int		fnAPP_CalcCheckMaxIn();								// 수표입금한도구하기

/////////////////////////////////////////////////////////////////////////////
//	입력함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_AcceptWith();									// 출금입력
	int		fnAPP_AcceptInq();									// 조회입력
	int		fnAPP_AcceptInqDate();								// 조회일입력
	int		fnAPP_AcceptTrans();								// 이체입력
	int		fnAPP_AcceptDep();									// 입금입력
	int		fnAPP_AcceptDep_KB();								// 입금입력 국민 //#N0226

	int		fnAPP_AcceptMuAccountNum();							// 무매체계좌번호입력
	int		fnAPP_AcceptMuConfirmNum();							// 무매체거래승인번호입력
	int		fnAPP_AcceptLostAccountNum();						// 사고신고계좌번호입력
	int		fnAPP_MuAcceptCardChoice();

	int		fnAPP_AcceptPassword();								// 비밀번호입력
	int		fnAPP_AcceptPbConfirmNum();							// 통장거래승인번호입력
	int		fnAPP_AcceptTelPassword();							// 폰뱅킹비밀번호입력
	int		fnAPP_AcceptMuPassword();							// 무매체거래비밀번호입력
	int		fnAPP_AcceptDesPassword();							// DES비밀번호입력
	int		fnAPP_AcceptMPINConfirmNum();						// ONEMONEY  #N0182
	
	int		fnAPP_AcceptMoney();								// 출금금액입력
	int		fnAPP_AcceptTMoney();								// 이체금액입력
	int		fnAPP_AcceptMoneyInKind();							// 입금권종선택
	int		fnAPP_AcceptMoneyInConfirm();						// 입금금액확인
	int		fnAPP_AcceptNhMoneyInConfirm();						// 매출금 입금금액확인

	int		fnAPP_InputMoneyCase_Etc();			// 금액선택(버튼) - Hnet 특이 사양 		
	int		fnAPP_SelectMoneyCase0(char * WithMsg);				// 금액선택(버튼),			
	int		fnAPP_InputMoneyCase1(char * WithMsg);				// 금액입력,			
	int		fnAPP_SelectKindofMoneyCase2(int CheckValue);		// 권종선택,				//수표권종 parameter 추가
	int		fnAPP_SelectCheckCase3(char * WithMsg);				// 수표선택, 입력,		
	int		fnAPP_InputCheckCase4(int CheckValue);				// 수표입력,				//수표권종 parameter 추가
	int		fnAPP_Input50TCashMoney();
	int		fnAPP_Input50TCashMoney_HANA();						//#N0214
	
	int		fnAPP_AcceptRemitName();							// 송금인명입력
	int		fnAPP_AcceptRMoney();								// 차액송금금액입력
	int		fnAPP_AcceptRemitConfirm();							// 송금확인

	int		fnAPP_AcceptBankNum();								// 은행번호입력
	int		fnAPP_AcceptInAccountNum();							// 수취계좌번호입력
	int		fnAPP_AcceptCmsCode();								// 모집인번호입력

	int		fnAPP_AcceptInTransStock();							// 유가증권자사대체입력
	int		fnAPP_AcceptInTransReqStock();						// 청약수량입력
	int		fnAPP_AcceptInTransReqStockAccount();				// 환불금고계좌번호입력

	int		fnAPP_AcceptChangePassword();						// 비밀번호변경입력
	int		fnAPP_AcceptLoanAccountNum();						// 대출계좌번호입력

	int		fnAPP_AcceptTelNum();								// 전화번호입력
	int		fnAPP_AcceptJuminNum();								// 주민번호입력
	int		fnAPP_AcceptCompareJuminNum();						// 송금주민번호비교
	CString fnDES_Cvt_CardNum(char* pVCardNum, int n_size);
	CString fnDES_Cvt_CardPan(char* pVCardNum, int n_size);      //#N0176

	int		fnAPP_AcceptAutoDetail();							// 이체주기,이체희망일
	int		fnAPP_AcceptAutoConfirm();							// 자동이체설정확인

	int		fnAPP_AcceptInqDateEnd();							// 거래기록조회일입력

	int		fnAPP_AcceptTranChoice();							// 전화사기 문구 추가 

	int		fnAPP_AcceptSlipChoice();							// 명세표인자여부선택
	int		fnAPP_AcceptContinuousPBChoice();					// 통장정리 연속거래 여부선택
	int		fnAPP_AcceptTranPBProc();							// 통장정리 연속거래 여부선택

	int		fnAPP_AcceptKCashPassword(int nReadDF, int ScrNum = 0);
																// K-CASH비밀번호입력
	int		fnAPP_AcceptKCashMoney();							// K-CASH금액입력
	int		fnAPP_AcceptKCashChangePassword(int nReadDF = TRAN_IC_KCASH);
																// K-CASH비밀번호변경입력
	int		fnAPP_FICSelectAccount();							// 금융IC 계좌선택

	int		fnAPP_TranWithChargeInqInputProc();

	
	int		fnAPP_UTILYBILLS_Sub01_InputProc();							// 지로세부거래코드선택
	int		fnAPP_UTILYBILLS_Sub02_InputProc();								// 지로번호입력
	int		fnAPP_UTILYBILLS_Sub03_InputProc();							// 전자납부번호입력

	//#GIRO 
	int		fnAPP_LOCALTAX_Sub01_InputProc();							// 조회방법 선택
	int		fnAPP_LOCALTAX_Sub02_InputProc();							// 할부선택
	int		fnAPP_LOCALTAX_AcceptPayMonth();							// 할부개월 선택
	int		fnAPP_LOCALTAX_AcceptSimpleNum();							// 간편납부번호 
	int		fnAPP_LOCALTAX_AcceptElecNum();								// 전자납부번호

					

/////////////////////////////////////////////////////////////////////////////
#endif
