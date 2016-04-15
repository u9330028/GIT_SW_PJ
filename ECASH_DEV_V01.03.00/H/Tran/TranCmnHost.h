/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCmnHost_H_)
#define _TranCmnHost_H_
#include "..\..\H\Tran\TranCmnDefine.h"
/////////////////////////////////////////////////////////////////////////////
//
//		은행업무 관련 변수 
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// 호스트 응답 코드
/////////////////////////////////////////////////////////////////////////////
#define HOST_OFFON			1			// 개국시 기기 재시작 요청응답
#define	HOST_DFS_ERROR		2			// DFS ERROR
#define	HOST_CAP_ERROR		3			// CAP_HEADER ERROR
#define	HOST_BPH_ERROR		4			// BP_HEADER ERROR
#define	HOST_INP_ERROR		5			// INPUT ERROR

#define SEED_SEND_EXCEPT_LEN	25		// 암호화제외길이
#define SEED_RECV_EXCEPT_LEN	16		// 암호화제외길이

/////////////////////////////////////////////////////////////////////////////
// 전문 정보 공통부
/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// DS Header(DV + MI + DS + US)
typedef struct _tagDSHEAD
{
	BYTE	DeviceID;					// Device id	: 0xa4
	BYTE	MI;							// Message information
	BYTE	ModeInfo;					// Mode 정보
	BYTE	CardType1;					// 카드정보1
	BYTE	TranType;					// 거래종별
	BYTE	CardType2;					// 카드정보2
	BYTE	PbType;						// 통장정보
	BYTE	Reserved1;					// 예약필드
	BYTE	ContInfo;					// CONT정보
	BYTE	JrnInfo;					// 저널정보
	BYTE	Reserved2;					// 예약필드
}DSHEAD;

//////////////////////////////////////////////////////////////////////
// Send CAP Header
typedef struct _tagCAPHEAD
{
	BYTE	US1;						// 구분자	: 0x1f
	BYTE	trId[5];					// 전문ID
	//BYTE	Blink;						// blink	: 0x20
	BYTE	HostSvcName[16];			// Host service(process) name
	BYTE	MachineType;				// 기종구분	: 'A'- ATM, 'C' - CD
	BYTE	TermKey[16];				// 소속지점1:3 + 처리지점:3 + 소속지점2:5 + 기번:3
	BYTE	trIdCode1[2];				// 업무식별코드1	: all set 0x20
	BYTE	trIdCode2[4];				// 업무식별코드2	: all set 0x20
	BYTE	UserFlag[3];				// user flag
	BYTE	statFlag[3];				// 상황 flag
	BYTE	PbIssueNum;					// 통장발행회차		: 0x30(고정)
	BYTE	SeqNo[4];					// 거래일련번호
	BYTE	ReqPrnLine[2];				// 통장: "04", 그외거래: "10"
	BYTE	Reserved[3];				// all set 0x20(고정)
	BYTE	US2;						// 구분자	: 0x1e
}CAPHEAD;

//////////////////////////////////////////////////////////////////////
// 상태/장에 송신 전문
typedef struct _tagNMSSEND
{
	BYTE	StatusInfo[20];				// 상태정보
	BYTE	US1;						// filler
	BYTE	ErrorInfo[10];				// 장애정보
	BYTE	US2;						// filler
	BYTE	HostSeqNum[12];				// 거래일련번호
	BYTE	AccountNum[16];				// 계좌번호
	BYTE	Reserved[2];				// Reserved
//	BYTE	US3;						// filler
} NMSSEND;


//////////////////////////////////////////////////////////////////////
// 
// IC정보 (151 Byte)
typedef struct _tagICINFO
{
	BYTE	ICAccountNum[16];			// 카드에 양각된 일련번호
	BYTE	SeedPassword[96];			// 32Byte : 카드 난수값
										// 32Byte : 터미널(CD기) 난수 값
										// 32Byte : 암호화된 비밀번호 값
										// 입금이체(송금),입금이체수취조회, 3행이체수취조회
										// 3행이체시 입금지시 전문에서는 0xF0으로 SET
	BYTE	KeyVersion[2];				// 금융IC Key Version
	BYTE	Dummy[36];					// 예비 (All Space(0x40))
	BYTE	Delimiter;					// Delimiter (0x3E 고정)
} BANKICINFO;


//////////////////////////////////////////////////////////////////////
// 마감 정보
typedef struct _tagCLOSEINFO
{
	BYTE	TotalWithAmt[15];			// 총출금액
	BYTE	CashWithAmt[15];			// 현금출금금액
	BYTE	CashDeositAmt[15];			// 현금입금금액
	BYTE	CheckWithAmt[15];			// 수표발행액
	BYTE	CheckDepositAmt[15];		// 수표입금액
}CLOSEINFO;

//////////////////////////////////////////////////////////////////////
// 취소거래 정보
/*
typedef struct _tagCANCELINFO
{
	BYTE	CancelType;					// 취소속성		: '2'고정
	BYTE	CancelSerial[4];			// 취소 거래 일련번호
	BYTE	CancelDate[8];				// 취소거래일
	BYTE	DFlag;						// 계정일자구분	: '0'고정
	BYTE	CFlag;						// 마감전후구분	: '0'고정
	BYTE	MaterialFlag;				// 매체유무구분	: '0'매체무, '1'매체유
	BYTE	CancelInfo[30];				// 취소내용		: USER Header입력정보(25) + LC(5)
	BYTE	Reserved[3];				// 예비			: '0'고정
	BYTE	Delimiter;					// 입력속성		: 0x3B
} CANCELINFO;
*/

typedef struct _tagEMVICINFO
{
	BYTE	CSHCAN[16];					// Cash IC Card Application Number
	BYTE	TRK2[37];					// Track II Information
	BYTE	TCP[6];						// Terminal Capability Profile
	BYTE	TVR[10];					// Terminal Verification Value
	BYTE	UN[8];						// Unpredictable Number
	BYTE	CRYPTO[16];					// Cryptogram(ARQC. TC. AAC)
	BYTE	ATCR[4];					// Application Transaction Counter
	BYTE	AIP[4];						// Application Interchange Profile
	BYTE	CTT[2];						// Cryptogram Transaction Type
	BYTE	TCC[4];						// Terminal Country Code
	BYTE	TTD[6];						// Terminal Transaction Date
	BYTE	CA[12];						// Cryptogram Amount
	BYTE	CCC[4];						// Cryptogram Currency code
	BYTE	CCA[12];					// Cryptogram Cashback Amount
	BYTE	CID[2];						// Cryptogram Information Data
	BYTE	CSN[2];						// Card Sequence Number
	BYTE	LI[2];						// Length Indicator
	BYTE	DKI[2];						// Derivation Key Index(DKI)
	BYTE	CVN[2];						// Cryptogram Version Number
	BYTE	CVR[8];						// Card Verification Result(CVR)
	BYTE	PWD[4];						// 비밀번호
	BYTE	Dummy[7];					// 예비
	BYTE	Delimiter;					// Delimiter (0x3E 고정)
} EMVICINFO;

//////////////////////////////////////////////////////////////////////
// 수신 포맷 
//////////////////////////////////////////////////////////////////////
// Receive Cap Header
typedef struct _tagRECVCAPHEAD			// 통장처리 command만 사용
{
	BYTE	CenterCode[4];				// Center Code
	BYTE	AcpStatusFlag[3];			// ACP 상황 Flag
	BYTE	Reserved[3];				// 예비
} RECVCAPHEAD;

//////////////////////////////////////////////////////////////////////
// Receive Common Header
typedef struct _tagRECVHEAD
{
//	BYTE	DeviceId;					// Device id	: 0xa4 고정
	BYTE	MessageInfo;				// Message information
	BYTE	UserInfo;					// user information
	RECVCAPHEAD RecvCapHead;			// receive CAP header
	BYTE	SeqNo[4];					// 거래일련번호
	BYTE	trId[4];					// 전문ID
	BYTE	HostErrCode[4];				// Host error code
	BYTE	HostErrMsg[80];				// Host error message
}RECVHEAD;

// Receive Common Data
typedef struct _tagRECVCOMM
{
	RECVHEAD RecvHead;					// Receive Header
	BYTE	TransData[4096];			// 수신 데이터부
}RECVCOMM;

//////////////////////////////////////////////////////////////////////
// EMV수신 V03-01-01-#01
typedef struct _tagEMVRecv
{
	BYTE	UI_STA;						// 'E'
	BYTE	Certificate;				// 인증유무
										// 1 : 인증, 조회거래시는 인증에러무시
										// 2 : 무시
	BYTE	ARPC[16];					// 
	BYTE	ARPCResCode[4];				// ARPC Response Code
	BYTE	ATC[4];						// ATC
	BYTE	IssuerLen[2];				// Issuer Script Code
	BYTE	IssuerScript[166];			// Issuer Script
	BYTE	UI_End;						// UI_End
} EMVRecv;

/////////////////////////////////////////////////////////////////////////////
typedef struct tagPSPRecv				// DATA부/명세프린트(거래) 기업용으로 변경
{
	BYTE	ParaLength;					// PARA길이
	int		By;							// Begin y
	int		Ey;							// End y
	int		y;							// Position y
	int		x;							// Position x
	BYTE	Data[256][256];				// DATA
} PSPRecv;

typedef struct tagPPPRecv				// DATA부/통장부프린트(거래)
{
	BYTE	ParaLength;					// PARA길이
	int     Bp;							// Begin Page
	int     Ep;							// End Page
	int		p;							// Position p
	int		y;							// Position y
	int		x;							// Position x
	int		DataLength[10][25];			// DATA Length		
	BYTE	Data[10][25][256];			// DATA
} PPPRecv;


typedef struct tagPPPRecvKb				// DATA부/통장부프린트(거래)  //#N0226
{
	BYTE	ParaLength;					// PARA길이
	int     Bp;							// Begin Page
	int     Ep;							// End Page
	int		p;							// Position p
	int		y;							// Position y
	int		x;							// Position x
	int		DataLength[10][24];			// DATA Length		
	BYTE	Data[10][24][256];			// DATA
} PPPRecvKb;

// Data PPP Recv
typedef struct tagPPPRecvNon
{
	// 출력DATA부/통장부프린트(거래)
										// CMD식별자('0xff00')
	BYTE	ParaLength;					// PARA길이('0x01')
	BYTE	CmdPara;					// CMD PARA
	int		p;							// Position p
	int		y;							// Position y
	int		x;							// Position x
	BYTE	SkipLineCnt[2];		
	BYTE	DataLineCnt[3];	
	BYTE	Data[10][24][128];			// DATA
} PPPRecvNon;


typedef struct tagPPWRecv				// DATA부/통장M/S WRITE(거래)
{
	BYTE	ParaLength;					// PARA길이
	int		x;							// Position x
	BYTE	Data[256];					// DATA
} PPWRecv;
	
typedef struct tagPCWRecv				// DATA부/카드M/S WRITE(거래)
{
	BYTE	ParaLength;					// PARA길이
	int		x;							// Position x
	BYTE	Data[256];					// DATA
} PCWRecv;
	
typedef struct tagPMERecv				// DATA부/메세지편집표시(거래/개국)	0xff24
{
	BYTE	ParaLength;					// PARA길이
	int		By;							// Begin y
	int		Ey;							// End y
	int		y;							// Position y
	int		x;							// Position x
	BYTE	Data[256][256];				// DATA
} PMERecv;

typedef struct tagPMDRecv				// DATA부/메세지표시(거래)
{
	BYTE	ParaLength;					// PARA길이
	int		By;							// Begin y
	int		Ey;							// End y
	int		y;							// Position y
	int		x;							// Position x
	BYTE	Data[30][256];				// DATA
} PMDRecv;

typedef struct tagETCRecv				// DATA부/ETC처리(전자화폐, 해외거래)
{
	BYTE	Data[73];					// 길이(자료유무)
} ETCRecv;



typedef struct tagKEYForm
{
	int			k_num;
	char 		m_key[120];
	char		s_key[120];
} KEYForm;

static KEYForm KEYFormTblConst[] =
{ 
	TRAN_ENC_000_00  ,  "2d0434d45aeed36564b3ce1518c1c909",  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
	TRAN_ENC_006_01  ,  "e7ff13da02d40cca18c94cc8698d85e3",  "01MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
	TRAN_ENC_034_02  ,  "4155544f4d414348494e45434f4f5250",  "00MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
	TRAN_ENC_026_03  ,  "4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d",  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
	TRAN_ENC_025_04  ,  "39303834373843363439394233464442",  "DD60B9MMMMMMMMMMMMMMMMMMMMMMMMMM",
	TRAN_ENC_032_05  ,  "39303834373843363439394233464442",  "DD60B9MMMMMMMMMMMMMMMMMMMMMMMMMM",
	TRAN_ENC_003_06  ,  "2d0434d45aeed36564b3ce1518c1c909",  "09MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
	TRAN_ENC_011_07  ,  "4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d",  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
	TRAN_ENC_023_08  ,  "4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d",  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
	TRAN_ENC_247_09  ,  "4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d",  "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    0xff             ,                                  "",                                  ""
};


////////////////////////////////////////////////////////////////
// DATE : 2004.03.08 (ATM_VAN_SYSTEM VER : V1-00-00)
// DEFINE DATA FORMAT 
// MAKE : GI JIN
/////////////////////////////////////////////////////////////////
typedef struct tagGETKEYTABLE
{
	char		m_key[512];
	char		s_key[512];
} GETKEYTABLE;


typedef struct tagB_S_C {			// bank setup code	
	char	b_c[4];					// bank code
	char	c_r_yymmdd[14];			// 인증서등록날자
	char    c_v_yymmdd[14];			// 인증서유효일자
	char	k_r_yymmdd[14];			// 세션키등록날자
	char    k_v_yymmdd[14];			// 세션키유효일자
	char    k_s_flag[1024];			// 세션키_flag
} B_S_C;

typedef struct tagB_E_INFO_INI {
	B_S_C m_B_S_C[20];
}B_E_INFO_INI;

typedef struct tagINI_K_INFO{
	char *n;

}INI_K_INFO;
typedef struct tagSREncData{
	char byOutLen[4];
	char byEncPasswd[16];
	char byEncamount[16];
	char byPlaintMsdata[1024];
	char byChiperMsdata[1024];
	char byDummy[1024];
	char byEnDummy[120*76];
	char byDeDummy[120*76];
} SREncData;

/* Sofo, Initech = Key registration, Certify */
typedef struct	tagSREncKey
{
	char	k_r_yymmdd[12];			// 세션키등록날자
	char    k_v_yymmdd[12];			// 세션키유효일자
	BYTE	k_apart[1];				// 키 구분
	BYTE	k_len[5];				// 키 길이 
	BYTE	k_data[2048];			// 키 데이터 (043x)
	BYTE	k_data_sub[2048];		// 서브 키 데이터  (044x)
	BYTE	k_sublen[5];			// 키 길이 
	BYTE	reserved[12];
}SREncKey, NEAR *NPSREncKey, FAR *LPSREncKey; 

typedef struct	tagRXHOSTMSG
{
	char	host_msg01[50];			// Msg
	char	host_msg02[50];			// Msg
	char	host_msg03[50];			// Msg
	char	host_msg04[50];			// Msg
	char	host_msg05[50];			// Msg
	char	host_Allmsg[1024];		// Msg

}RXHOSTMSG, NEAR *NPRXHOSTMSG, FAR *LPRXHOSTMSG; 

typedef struct tagDataField{
	BYTE byFCode[4];
	BYTE byFLen[3];
	BYTE byFData[9216];
} DataField, *LPDataField;


typedef struct tagTXRXECASH{
	BYTE byTLen[4];
	BYTE byTData[9216];
	BYTE byBcc[2];
} TXRXECASH;



typedef struct tagRXDISPLAYDATA{
	char byDataField_002x[98];  //시간
	char byDataField_004x[98];  //제휴사 일련번호 
	char byDataField_009x[98];  //마감구분 
	char byDataField_017x[98];  //계좌번호
	char byDataField_023x[98];  //거래금액
	char byDataField_026x[98];	//일련번호
	char byDataField_027x[98];	//취급 지점(수취이체)
	char byDataField_024x[98];	//수수료
	char byDataField_022x[98];  //계좌잔액
	char byDataField_025x[98];	//타점권
	char byDataField_021x[98];  //수취인
	char byDataField_012x[98];  //수취기관
	char byDataField_020x[98];	//수취계좌
	char byDataField_104x[98];	//명세표인자
	char byDataField_458x[98];	//이자율 (현금서비스)
	char byDataField_345x[98];	//수수료 포인트 결제 여부 
	char byDataField_018x[98];	//광주 브랜드 계좌 성명
	char byDataField_307x[98];	//신한카드 한도상향	 
	char byDataField_1721[98];	//신한카드 한도상향	 
	char byDataField_065x[98];	//할증률
	char byDataField_Dummy[98];	//신한카드 한도상향	 
} RXDISPLAYDATA;

// TYPE 3 : 신용카드 EMV인증 RCV SCRIPT
typedef struct	tagRecvEmvrcvData		// 신용카드 EMV인증 RCV SCRIPT	
{
	char	ARPC[16];					// ARPC 
	char	APRCResposeCode[4];			// APRCResposeCode 
	char	IssuerScriptLen[3];			// IssuerScriptLen
	char	IssuerScript[256];			// IssuerScript
	char	IssuerScriptResult[40];		// IssuerScriptResult
	char	Reserve[17];				// Reserve
} RECVEMVRCVDATA;

typedef struct tagRXHNETJIRODATA{
	char JiroBankCode[16];  //출금은행점코드
	char Reserve1[1];
	char JiroNum[9];		//지로번호
	char Reserve2[1];
	char ElecNum[21];	//거래금액
	char Reserve3[1];
	char JiroOrgName[16];	//이용기관명
	char Reserve4[1];
	char AccountName[10];	//출금인명
	char Reserve5[1];
	char CustomerName[16];  //납부자성명
	char Reserve6[1];
	char JiroPayData[300];  //납부내역
} RXHNETJIRODATA;

typedef struct tagRXHNETLOCALTAXDATA{
	char JiroBankCode[16];  //출금은행점코드
	char Reserve1[1];
	char CustomerName[16];  //납부자성명
	char Reserve2[1];
	char Gubun[2];			//건별납부 구분자
	char Reserve3[1];
	char ElecNum[21];		//건별납부 일련번호
	char Reserve4[1];
	char TaxCount[4];		//총고지건수
	char Reserve5[1];
	char TotalAmount[12];	//총고지금액
	char Reserve6[1];
	char BillCount[2];		 //납부건수
	char Reserve7[1];
	char BillPayMonth[2];	 //할부개월
	char Reserve8[1];				
	char JiroPayData[300];  //납부내역
} RXHNETLOCALTAXDATA;

typedef struct tagRXETICKETDATA{
	char TicketLen[5];				//상품 DATA 길이
	char BigData[995];				//상품 대분류
	char DetailData[6000];			//상품 분류
	char AcceptDetailData[376];		//상품 세부 선택 data
	char SMSKey[6];					//SMS 인증 번호
	char byDataField_003x[6];		//SMS 전거래 SST일련번호
	char byDataField_343x[4];		//전자상품권 유효기간 
	char ICINFO[130];				//IC DATA
} RXETICKETDATA;


//#N0174 이지원 DATA 선언
typedef struct tagRXEASYONEDATA{
	char Gubun[2];					//이지원 거래 구분
	char NextFlag[1];				//다음출력 존재 유무
	char NextKey[15];				//다음 출력 조회때 필요 키
	char Count[2];					//계좌 반복 횟수 (최대 5)
	char EasyOne_Acc[5][75];				//이지원 이체 계좌 정보
	char Dummy[100];				//Dummy
} RXEASYONEDATA;



/*******************************************************************************************************************************
/* 하이패스 충전 서비스 (Structure TX, RX) ______Begin , Yun.H.J 2012.04.17
/*******************************************************************************************************************************/

/*------------------------------------------------------------------------------*/
/* 하이패스충전 현금입금 응답													*/
/* Length : 250 + 4040 = 4290													*/
/* Date   : 2012.04.17															*/
/* Author.: Yun.H.J																*/
/*------------------------------------------------------------------------------*/
typedef struct tagRXATM4717
{
	BYTE	dsjpdata[160];			// 화면 Data(응답메시지)
	BYTE	seqno[12];				// 거래고유번호
	BYTE	traceno[6];				// 전문 추적 번호
	BYTE	accno[32];				// 계좌/카드번호 길이(2)+계좌번호(30) --> 1091 : Sectionid 12Byte (2010.2.2)
	BYTE	amount[16];				// 금액
	BYTE    remain[16];				// 잔액
	BYTE	interseqno[16];			// 내부거래 고유번호 (주택은행)
	BYTE	reserved[3782];         // pbm list data     3600 byte -> 유준호과장 (장애대책 2010.04.22)
}RXATM4717, NEAR *NPRXATM4717, FAR *LPRXATM4717;

/*------------------------------------------------------------------------------*/
/* 하이패스충전 신용승인/취소 응답												*/
/* Length : 250 + ? = ?															*/
/* Date   : 2012.04.17															*/
/* Author.: Yun.H.J																*/
/*------------------------------------------------------------------------------*/
typedef struct tagRXATM4840_4850
{
	BYTE	dsjpdata[160];			// 화면 Data(응답메시지)
	BYTE	seqno[12];				// 거래고유번호
	BYTE	traceno[6];				// 전문 추적 번호
	BYTE	session_id[18];			// 세션ID
	BYTE	payment[2];				// 결제모드
	BYTE	mobilance[16];			// 모빌리언스사용자고유번호
	BYTE	admno[16];				// 승인번호
	BYTE 	oseqno[16];				// 원거래번호
	BYTE	amount[6];				// 결제금액
	BYTE    admtime[14];			// 승인일시
	BYTE	remain[10];				// 결제후잔액
	BYTE	reserved[50];
}RXATM4840_4850, NEAR *NPRXATM4840_4850, FAR *LPRXATM4840_4850;

/*------------------------------------------------------------------------------*/
/* 하이패스충전 충전 응답														*/
/* Length : 250 + 250 = 500															*/
/* Date   : 2012.04.17															*/
/* Author.: Yun.H.J																*/
/*------------------------------------------------------------------------------*/
typedef struct tagRXATM4670
{
	BYTE	dsjpdata[160];			// 화면 Data(응답메시지)
	BYTE	seqno[12];				// 거래고유번호
	BYTE	traceno[6];				// 전문 추적 번호
	BYTE	usercard_id[10];		// 이용자카드 ID
	BYTE	start_bit[1];			// 거래시작 BIT 상태
	char    random_num[16];			// HSM난수
	char    s2[8];					// HSM서명
	BYTE	reserved[37];
}RXATM4670, NEAR *NPRXATM4670, FAR *LPRXATM4670;

/*------------------------------------------------------------------------------*/
/* 하이패스충전 충전완료 응답													*/
/* Length : 250 + 220 = 470														*/
/* Date   : 2012.04.17															*/
/* Author.: Yun.H.J																*/
/*------------------------------------------------------------------------------*/
typedef struct tagRXATM4671
{
	BYTE	dsjpdata[160];			// 화면 Data(응답메시지)
	BYTE	seqno[12];				// 거래고유번호
	BYTE	traceno[6];				// 전문 추적 번호
	BYTE	usercard_id[10];		// 이용자카드 ID
	BYTE	reserved[32];
}RXATM4671, NEAR *NPRXATM4671, FAR *LPRXATM4671;

/*------------------------------------------------------------------------------*/
/* 하이패스충전 복구 응답														*/
/* Length : 250 + 251 = 501															*/
/* Date   : 2012.04.17															*/
/* Author.: Yun.H.J																*/
/*------------------------------------------------------------------------------*/
typedef struct tagRXATM4680
{
	BYTE	dsjpdata[160];			// 화면 Data(응답메시지)
	BYTE	seqno[12];				// 거래고유번호
	BYTE	traceno[6];				// 전문 추적 번호
	BYTE	usercard_id[10];		// 이용자카드 ID
	BYTE	start_bit[1];			// 거래시작 BIT 상태
	BYTE    random_num[16];			// HSM난수
	BYTE    s2[8];					// HSM서명
	BYTE    restore_type[1];		// 복구유형
	BYTE	reserved[37];
}RXATM4680, NEAR *NPRXATM4680, FAR *LPRXATM4680;

/*******************************************************************************************************************************
/* 하이패스 충전 서비스 (Structure TX, RX) ____End
/*******************************************************************************************************************************/
typedef struct _tagRXHIPASS
{
	RXATM4717	m_RXATM4717;
	RXATM4840_4850	m_RXATM4840;
	RXATM4840_4850	m_RXATM4850;
	RXATM4670	m_RXATM4670;
	RXATM4671	m_RXATM4671;
	RXATM4680	m_RXATM4680;
}RXHIPASS, NEAR *NPRXHIPASS, FAR *LPRXHIPASS;


/////////////////////////////////////////////////////////////////////////////
#endif

