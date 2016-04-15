/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranHostProc_H_INCLUDED_)
#define _TranHostProc_H_INCLUDED_




/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	송수신변수
/////////////////////////////////////////////////////////////////////////////
	int		AddSerialFlg;				// 일련번호증가Flg
	int		TranSend;					// N차송신여부

	int		SendLength;					// 송신Length
	int		SaveSendLength;				// 저장송신Length
	int		RecvLength;					// 수신Length
	BYTE	SendBuffer[NETBUFFSIZE];	// 송신Buffer
	BYTE	SaveSendBuffer[NETBUFFSIZE];// 저장송신Buffer
	BYTE	RecvBuffer[NETBUFFSIZE];	// 수신Buffer


	CRITICAL_SECTION	m_csLock;
	B_E_INFO_INI		m_B_E_INFO_INI;
	GETKEYTABLE			m_G_KEY_T;
	int					m_TranMode;
	int					m_TranEncrypt_f;
	BYTE				m_yymmddhhmmss[16];
	int					m_EnCryptFlag;
	char				m_src_seedencrypt[NETBUFFSIZE];
	char				m_des_seedencrypt[NETBUFFSIZE];
	char				m_AtmCode[20];
struct tabSend
{
	BYTE	TranSerial[4];				// 송신거래일련번호
	BYTE	TranCashAmount[15];			// 송신거래금액
	BYTE	TranCheckAmount[15];		// 송신거래금액
	BYTE	TranInputID[4];				// 송신거래InputID
	BYTE	TranAccount[16];			// 송신계좌번호
} Send;

	BYTE	SaveCashAmount[11];			// 거래금액저장(취소관련)
	BYTE	SaveCheckAmount[11];		// 거래금액저장(취소관련)
	BYTE	SaveSerialNo[6];			// 거래일련번호저장(장애전문관련)
	BYTE	SaveJiroNum[9];				// 지로번호저장

	BYTE			SeedKey[263];		// 암호화 키
//	QSLSession		*cl_sess;
//	QSLPubKey		*pubKey;
	unsigned char	SeedBuffer[NMSBUFFSIZE];// 암호화를 위한 버퍼
	int				SeedLength;			// 암호화를 위한 길이

	PSPRecv		PSPRecvData;			// DATA부/명세프린트(거래)
	PPPRecv		PPPRecvData;			// DATA부/통장부프린트(거래)
	PPPRecvNon	PPPRecvDataNon;			// DATA부/통장부프린트(거래) - 농협
	PPPRecvKb	PPPRecvDataKb;			// DATA부/통장부프린트(거래) - 농협
	PPWRecv		PPWRecvData;			// DATA부/통장M/S WRITE(거래)
	PCWRecv		PCWRecvData;			// DATA부/카드M/S WRITE(거래)
	PMERecv		PMERecvData;			// DATA부/메세지편집표시(거래종류별)
	PMDRecv		PMDRecvData;			// DATA부/메세지표시(거래)
	ETCRecv		ETCRecvData;			// DATA부/기타거래출력(거래)

										// Send / Receive Data Buffer
										// HOST Data Structure
	RECVCOMM	RecvComm;				// 수신공통데이터

	BYTE		HostReserved[40];		// HOST예비

	BYTE		BranchName[40];			// 지점명
	BYTE		BranchTelNum[20];		// 지점전화번호

	SREncKey	   m_TXENCKEY;
	SREncKey	   m_RXENCKEY;
	F031XINFO	   m_F031X;	
	BYTE		   m_FRX101X[4];	

	CStringArray s_saTXDataField;
	CStringArray s_saRXDataField;

	RECVEMVRCVDATA m_RecvEmvrcvData;
	RXDISPLAYDATA m_RD;
	RXHOSTMSG m_RXHOSTMSG;

	SREncData m_STXEncBuff;
	SREncData m_SRXEncBuff;


	TXRXECASH m_TXECASH;
	TXRXECASH m_RXECASH;
	DataField m_TXDataField;
	DataField m_RXDataField;

	//#GIRO
	RXHNETJIRODATA m_RXJIRODATA;
	RXHNETLOCALTAXDATA m_RXLOCALTAXDATA;

	//전자상품권 DATA
	RXETICKETDATA m_RXETICKETDATA;

	//이지원 DATA  #N0174
	RXEASYONEDATA m_RXEASYONEDATA;

	CArray<DataField *, DataField *> m_arrTXDataStructure;
	CArray<DataField *, DataField *> m_arrRXDataStructure;

	DataField pArrDataFieldArr[100];
	int		  nSendDataIndex;

	//#N0266
	RXHIPASS		m_RXHIPASS;
/////////////////////////////////////////////////////////////////////////////
//	수신후처리변수
/////////////////////////////////////////////////////////////////////////////
	BYTE	FinishFlag;					// 시재구분
	int		ChoiceTran;					// 조회후선택된거래
	int		TranResultNGCode;			// Tran거래결과NG코드
	int		PbNextTranFlag;				// 통장다음거래통보Flag
	int		PbNextPageFlag;				// 통장다음페이지통보Flag
	int		InqNextTranFlag;			// 목록조회다음거래통보Flag
	int		RetractFlag;				// 매체회수지시Flag
	int		KCashHostICFlag;			// K-CASH Host IC Flag
	int		UpdateKeyParam;				// 파라미터갱신여부
	int		DepNextTranFlag;			// 연속거래_입금_Flag
	int		DepNextTranCount;			// 연속거래_횟수


	int		SeedErrorCode;				// 암복화에러
										// 1 : 자동기 암호화 NG
										// 2 : 자동기 복호화 NG
										// 3 : BP 0x31, 0x32 NG
										// 4 : BP 0x31, 0x32 NG
	int		ContinueTranCount;			// 연속거래카운트(출금,송금,입금)
	int		PbPrintFlag;				// 통장기장여부
	int		PbPrintAfterMsgFlag;		// 통장기장여부

	int		InqNextTranCount;			// 조회횟수
	int		GhostAccountFlag;			// 가상계좌여부


//우리투자증권 2009.07.14 Gi.Jin
	BYTE m_247MK_TABLE[320+1];
	BYTE m_247W1_KEY[46+1]; /* Client용 키정보 */	
	int  m_247Wlen;
//한화증권
	BYTE m_269MK_TABLE[320+1];
	BYTE m_269W1_KEY[46+1]; /* Client용 키정보 */	
	int  m_269Wlen;
//동부증권
	BYTE m_279MK_TABLE[320+1];
	BYTE m_279W1_KEY[46+1]; /* Client용 키정보 */	
	int  m_279Wlen;

//삼성증권 2009.08.28 Gi.Jin
    char m_szTranBuffer[4000 + 1];
	// Operations EMV Lib
	CString		EMVHostResponse;		// EMV HOST 응답처리	V09-33-00-#01 ICCM 모듈적용
    int				EmvICStatus;				// 금융IC 탑재 App			// V01.00.11_0412
    int				ICnIcApp;				// 금융IC 탑재 App			// HNB10501
	CString		m_strEMVHostResponse;	

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	송수신함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_SendHost();									// 송신
	int		fnAPP_SendHostCancel();								// 송신(취소출금/취소입금)
	int		fnAPP_SendHostTranComplete();						// 완료
	int		fnAPP_SendRecvHostSnod(int nTranCodeID = FALSE);							// SNOD 처리
	int		fnAPP_SendMakeManageSend(int RealFlg);				// 송신전문작성(관리부 Send)
	int		fnAPP_SendMake(int RealFlg = TRUE);					// 송신전문작성 RealFlg = 0:일반, 1:상태, 2:장애
	int		fnAPP_SendHostSet(int RealFlg);						// 거래별자료설정
	int		fnAPP_SendRecvHostCancel_PowerOff();

/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_RecvHost();									// 수신
	int		fnAPP_RecvAnal();									// 수신해석

	int		fnAPP_RecvPSPGet(void* RecvArea, PSPRecv* DataArea, int Length);
																// DATA부/명세프린트(거래)
	int		fnAPP_RecvPSPMove(BYTE Data, PSPRecv* pDataArea, int Length);
																// 명세영역Move

	int		fnAPP_NONGHYUP_RecvPPPGet(void* RecvArea, PPPRecvNon* DataArea, int Length);
	int		fnAPP_NONGHYUP_RecvPPPGet1(void* RecvArea, PPPRecvNon* DataArea,int SubPatton, int Length);
	int		fnAPP_IBK_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length);
	int		fnAPP_KEB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length);
	int		fnAPP_KJB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length);  //#N0181
	int		fnAPP_HANA_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length);  //#N0214
	int		fnAPP_KB_RecvPPPGet(void* RecvArea, PPPRecvKb* DataArea, int Length);
	int		fnAPP_CTB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length);	// #N0252
	int		fnAPP_SC_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length);		//#N0245
	int		fnAPP_KNB_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length);	//#N0277

	int		fnAPP_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length);
																// DATA부/통장부프린트(거래)
	int		fnAPP_RecvPPPMove(BYTE Data, PPPRecv* pDataArea, int Length);
	int		fnAPP_HANA_RecvPPPMove(BYTE Data, PPPRecv* pDataArea, int Length);   //#N0214
	int		fnAPP_NONGHYUP_RecvPPPMove(BYTE Data, PPPRecvNon* pDataArea, int Length);
																// 통장영역Move

	int		fnAPP_RecvPMEGet(void* RecvArea, PMERecv* DataArea, int Length);
																// DATA부/메세지편집표시(거래/개국)
	int		fnAPP_RecvPMESubGet(void* RecvArea, PMERecv* DataArea, int Cnt, int Length);
																// 메세지편집표시Sub
	int		fnAPP_RecvPMEMove(BYTE Data, PMERecv* pDataArea, int Length);
																// 메세지편집영역Move

	int		fnAPP_RecvPMDGet(void* RecvArea, PMDRecv* DataArea, int Length);
																// DATA부/메세지표시(거래)
	int		fnAPP_RecvPMDMove(BYTE Data, PMDRecv* pDataArea, int Length);
																// 화면영역Move
	int		fnAPP_RecvPCWGet(void* RecvArea, PCWRecv* DataArea, int Length);
																// 카드M/S기록
	int		fnAPP_RecvPCWMove(BYTE Data, PCWRecv* pDataArea, int Length);
																// 카드영역Move

/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_RecvDesKeyProc(PMERecv* DataBuff);			// TMK/TPK Recv Proc
	int		fnAPP_RecvDesKeyCheck(PMERecv* DataBuff);			// TMK/TPK Recv DES KEY NULL/ZERO Check V02.06.01

/////////////////////////////////////////////////////////////////////////////
//	송수신함수
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_SendHostDataSet(int RealFlg);					// 거래별자료설정


	int		fnAPP_SendMakeDataSetErrorCode();					// 에러코드편집
//	int		fnAPP_SendMakeDataSetBankNum();						// 은행코드편집
	int		fnAPP_SendMakeDataSetCheckCount();					// 출금수표매수편집
	int		fnAPP_SendMakeDataSetPassword();					// 비밀번호편집

	// 개별부 조립
	int		fnAPP_SendMakeDataSetDepCheckInfo();				// 입금수표정보편집
	int		fnAPP_SendMakeDataSetTransferInfo();				// 송금정보편집
//	int		fnAPP_SendMakeDataSetAutoInfo();					// 자동이체정보편집
//	int		fnAPP_SendMakeDataSetCmsInfo();						// CMS정보편집
//	int		fnAPP_SendMakeDataSetLoanInfo();					// 대출정보편집
//	int		fnAPP_SendMakeDataSetJiroInFo();					// 지로정보편집

//	int		fnAPP_SendMakeICInformation();						// 금융IC정보편집
	int		fnAPP_SendMakeIRDInformation();						// 동글이정보편집
	int		fnAPP_SendMakeEMVInformation();						// EMV정보편집

//	int		fnAPP_SendMakeTaDepositInfo();						// 타행입금정보편집
	int		fnAPP_SendMakeDataSetNextInqInfo();					// 다음조회조건편집


/****************************************************************************************/
/*
/*	
/*
/*	
/*****************************************************************************************/
	// Bank Encrypt Function (Kim.G.J 2008.1.29)
	int		fnAPP_BankKey_Flag_Init();
	int		fnAPP_Encrypt_Flag_Init();
	int		fnAPP_FileTable_Init(int type);
	int		fnAPP_KeyTable_Init();
	int		fnAPP_TranBank_Flag_Init();
	int		fnAPP_TranBank_Flag_Init_T();
	int		fnAPP_File_Init(int type = FALSE);
	int		fnAPP_FileMake_Init(int f_type = FALSE);
	int		fnAPP_BankKey_Set_Flag(int bc, int k_type);
	int		fnAPP_BankKey_Get_Flag(int bc, int k_type);
	int		fnAPP_Bank_Key_Convert(int bankcode = FALSE); //SOFO, INTECH, (KB,KUB,NON)
	int		fnAPP_CertKey_Limit(char *yymmdd,int type);


	int		fnAPP_KeyInitStart(int bc, int nFlag = 0);
	int		fnAPP_KeyInitStartRecv(int bc, int bc_type, int nEncFlag = FALSE);
	int		fnAPP_EncType(int nTypeEnc);
	int		fnAPP_DecType(int nTypeDec);
	int		fnAPP_EncCall(int nTypeEnc, int nTranCode, int nEncrypt = FALSE );
	int		fnAPP_DecCall(int nTypeDec, int nTranCode, int nEncrypt = FALSE  );


	int		fnAPP_KeyInit004(BYTE * out_b,char *out_i, char *out_v, int out_t);
	int		fnAPP_KeyInit003(BYTE * out_b,char *out_i, char *out_v, int out_t);
	int		fnAPP_KeyInit011(BYTE * out_b,char *out_i, char *out_v, int out_t);
	int		fnAPP_KeyInit023(BYTE * out_b,char *out_i, char *out_v, int out_t);	
	int		fnAPP_KeyInit247(BYTE * out_b,char *out_i, char *out_v, int out_t);	
	int		fnAPP_KeyInit240(BYTE * out_b,char *out_i, char *out_v, int out_t);	
	int		fnAPP_KeyInit269(BYTE * out_b,char *out_i, char *out_v, int out_t);	
	int		fnAPP_KeyInit005(BYTE * out_b,char *out_i, char *out_v, int out_t);	

	int		fnAPP_KeyInit278(BYTE * out_b,char *out_i, char *out_v, int out_t);	
	int		fnAPP_KeyInit279(BYTE * out_b,char *out_i, char *out_v, int out_t);	

	int		fnAPP_CertV006(int ikv);	
	int		fnAPP_CertV003(int ikv);	
	int		fnAPP_CertV011(int ikv);	
	int		fnAPP_CertV023(int ikv);	

	int    fnAPP_TranEnc000_000(int nTrCode = FALSE);		//"????????????"
	int    fnAPP_TranEnc001_001(int nTrCode = FALSE);		//"한국은행    "
	int    fnAPP_TranEnc002_002(int nTrCode = FALSE);		//"산업은행    "
	int    fnAPP_TranEnc003_003(int nTrCode = FALSE);		//"기업은행    "
	int    fnAPP_TranEnc005_005(int nTrCode = FALSE);		//"외환은행    "
	int    fnAPP_TranEnc004_006(int nTrCode = FALSE);		//"국민은행    "
	int    fnAPP_TranEnc007_007(int nTrCode = FALSE);		//"수협중앙회  "
	int    fnAPP_TranEnc008_008(int nTrCode = FALSE);		//"수출입은행  "
	int    fnAPP_TranEnc011_011(int nTrCode = FALSE);		//"농협중앙회  "
	int    fnAPP_TranEnc012_012(int nTrCode = FALSE);		//"단위농협    "
	int    fnAPP_TranEnc020_020(int nTrCode = FALSE);		//"우리은행    "
	int    fnAPP_TranEnc023_023(int nTrCode = FALSE);		//"제일은행    "
	int    fnAPP_TranEnc053_053(int nTrCode = FALSE);		//"씨티은행    "
	int    fnAPP_TranEnc031_031(int nTrCode = FALSE);		//"대구은행    "
	int    fnAPP_TranEnc032_032(int nTrCode = FALSE);		//"부산은행    "
	int    fnAPP_TranEnc034_034(int nTrCode = FALSE);		//"광주은행    "
	int    fnAPP_TranEnc035_035(int nTrCode = FALSE);		//"제주은행    "
	int    fnAPP_TranEnc037_037(int nTrCode = FALSE);		//"전북은행    "
	int    fnAPP_TranEnc039_039(int nTrCode = FALSE);		//"경남은행    "
	int    fnAPP_TranEnc045_045(int nTrCode = FALSE);		//"새마을금고  "
	int    fnAPP_TranEnc048_048(int nTrCode = FALSE);		//"신협중앙회  "
	int    fnAPP_TranEnc050_050(int nTrCode = FALSE);		//"상호저축은행"
	int    fnAPP_TranEnc054_054(int nTrCode = FALSE);		//"HSBC은행    "
	int    fnAPP_TranEnc055_055(int nTrCode = FALSE);		//"도이치은행  "
	int    fnAPP_TranEnc056_056(int nTrCode = FALSE);		//"ABN은행     "
	int    fnAPP_TranEnc071_071(int nTrCode = FALSE);		//"우체국      "
	int    fnAPP_TranEnc025_081(int nTrCode = FALSE);		//"하나은행    "
	int    fnAPP_TranEnc021_088(int nTrCode = FALSE);		//"신한은행    "
	int    fnAPP_TranEnc105_105(int nTrCode = FALSE);		//"KEB하나카드 "
	int    fnAPP_TranEnc105_115(int nTrCode = FALSE);		//"KEB해외카드 "
	int    fnAPP_TranEnc106_106(int nTrCode = FALSE);		//"국민카드    "
	int    fnAPP_TranEnc107_107(int nTrCode = FALSE);		//"수협카드    "
	int    fnAPP_TranEnc111_111(int nTrCode = FALSE);		//"농협비자    "
	int    fnAPP_TranEnc120_120(int nTrCode = FALSE);		//"우리카드    "
	int    fnAPP_TranEnc121_121(int nTrCode = FALSE);		//"조흥카드    "
	int    fnAPP_TranEnc126_192(int nTrCode = FALSE);		//"신한카드    "
	int    fnAPP_TranEnc127_127(int nTrCode = FALSE);		//"씨티카드    "
	int    fnAPP_TranEnc134_034(int nTrCode = FALSE);		//"광주카드    "
	int    fnAPP_TranEnc135_135(int nTrCode = FALSE);		//"제주비자    "
	int    fnAPP_TranEnc137_037(int nTrCode = FALSE);		//"전북카드    "
	int    fnAPP_TranEnc181_105(int nTrCode = FALSE);		//"KEB하나카드 "
	int    fnAPP_TranEnc184_184(int nTrCode = FALSE);		//"삼성카드    "
	int    fnAPP_TranEnc192_192(int nTrCode = FALSE);		//"신한카드    "
	int    fnAPP_TranEnc193_193(int nTrCode = FALSE);		//"현대카드    "
	int    fnAPP_TranEnc194_194(int nTrCode = FALSE);		//"BC카드      "
	int    fnAPP_TranEnc195_195(int nTrCode = FALSE);		//"롯데카드    "
	int    fnAPP_TranEnc209_209(int nTrCode = FALSE);		//"유안타증권	 "
	int    fnAPP_TranEnc218_218(int nTrCode = FALSE);		//"현대증권    "
	int    fnAPP_TranEnc230_230(int nTrCode = FALSE);		//"미래에증권  "
	int    fnAPP_TranEnc238_238(int nTrCode = FALSE);		//"대우증권    "
	int    fnAPP_TranEnc240_240(int nTrCode = FALSE);		//"삼성증권    "
	int    fnAPP_TranEnc243_243(int nTrCode = FALSE);		//"한국투자    "
	int    fnAPP_TranEnc247_247(int nTrCode = FALSE);		//"우리투자    "
	int    fnAPP_TranEnc261_261(int nTrCode = FALSE);		//"교보증권    "
	int    fnAPP_TranEnc262_262(int nTrCode = FALSE);		//"하이투자    "
	int    fnAPP_TranEnc263_263(int nTrCode = FALSE);		//"HMC투자     "
	int    fnAPP_TranEnc264_264(int nTrCode = FALSE);		//"키움증권    "
	int    fnAPP_TranEnc265_265(int nTrCode = FALSE);		//"이트레이드  "
	int    fnAPP_TranEnc266_266(int nTrCode = FALSE);		//"에스케이    "
	int    fnAPP_TranEnc267_267(int nTrCode = FALSE);		//"대신증권    "
	int    fnAPP_TranEnc268_268(int nTrCode = FALSE);		//"솔로몬투자  "
	int    fnAPP_TranEnc269_269(int nTrCode = FALSE);		//"한화증권    "
	int    fnAPP_TranEnc270_270(int nTrCode = FALSE);		//"하나대투    "
	int    fnAPP_TranEnc278_278(int nTrCode = FALSE);		//"신한금융    "
	int    fnAPP_TranEnc279_279(int nTrCode = FALSE);		//"동부증권    "
	int    fnAPP_TranEnc280_280(int nTrCode = FALSE);		//"유진투자    "
	int    fnAPP_TranEnc287_287(int nTrCode = FALSE);		//"메리츠증권  "
	int    fnAPP_TranEnc289_289(int nTrCode = FALSE);		//"NH증권      "
	int    fnAPP_TranEnc290_290(int nTrCode = FALSE);		//"부국증권    "
	int    fnAPP_TranEnc291_291(int nTrCode = FALSE);		//"신영증권    "
	int    fnAPP_TranEnc292_292(int nTrCode = FALSE);		//"LIG증권	   "
	int    fnAPP_TranEnc294_294(int nTrCode = FALSE);		//"해외카드 BC "
	int    fnAPP_TranEnc294_294_BC_000_ENC_FOREGIN(int nTrCode = FALSE);		//"해외비씨    "
	int    fnAPP_TranEnc294_294_BC_JCB_ENC_FOREGIN(int nTrCode = FALSE);		//"해외비씨    "
	int    fnAPP_TranEnc384_364(int nTrCode = FALSE);		//"삼성증권    "
	int    fnAPP_TranEnc484_484(int nTrCode = FALSE);		//"CJ시스템    "
	int    fnAPP_TranEnc601_601(int nTrCode = FALSE);		//"발렉스      "
	int    fnAPP_TranEnc701_701(int nTrCode = FALSE);		//"KTF         "
	int    fnAPP_TranEnc702_702(int nTrCode = FALSE);		//"LGT         "
	int    fnAPP_TranEnc703_703(int nTrCode = FALSE);		//"SKT         "


	int    fnAPP_TranDec000_000(int nTrCode = FALSE);		//"????????????"
	int    fnAPP_TranDec001_001(int nTrCode = FALSE);		//"한국은행    "
	int    fnAPP_TranDec002_002(int nTrCode = FALSE);		//"산업은행    "
	int    fnAPP_TranDec003_003(int nTrCode = FALSE);		//"기업은행    "
	int    fnAPP_TranDec005_005(int nTrCode = FALSE);		//"외환은행    "
	int    fnAPP_TranDec004_006(int nTrCode = FALSE);		//"국민은행    "
	int    fnAPP_TranDec007_007(int nTrCode = FALSE);		//"수협중앙회  "
	int    fnAPP_TranDec008_008(int nTrCode = FALSE);		//"수출입은행  "
	int    fnAPP_TranDec011_011(int nTrCode = FALSE);		//"농협중앙회  "
	int    fnAPP_TranDec012_012(int nTrCode = FALSE);		//"단위농협    "
	int    fnAPP_TranDec020_020(int nTrCode = FALSE);		//"우리은행    "
	int    fnAPP_TranDec023_023(int nTrCode = FALSE);		//"제일은행    "
	int    fnAPP_TranDec053_053(int nTrCode = FALSE);		//"씨티은행    "
	int    fnAPP_TranDec031_031(int nTrCode = FALSE);		//"대구은행    "
	int    fnAPP_TranDec032_032(int nTrCode = FALSE);		//"부산은행    "
	int    fnAPP_TranDec034_034(int nTrCode = FALSE);		//"광주은행    "
	int    fnAPP_TranDec035_035(int nTrCode = FALSE);		//"제주은행    "
	int    fnAPP_TranDec037_037(int nTrCode = FALSE);		//"전북은행    "
	int    fnAPP_TranDec039_039(int nTrCode = FALSE);		//"경남은행    "
	int    fnAPP_TranDec045_045(int nTrCode = FALSE);		//"새마을금고  "
	int    fnAPP_TranDec048_048(int nTrCode = FALSE);		//"신협중앙회  "
	int    fnAPP_TranDec050_050(int nTrCode = FALSE);		//"상호저축은행"
	int    fnAPP_TranDec054_054(int nTrCode = FALSE);		//"HSBC은행    "
	int    fnAPP_TranDec055_055(int nTrCode = FALSE);		//"도이치은행  "
	int    fnAPP_TranDec056_056(int nTrCode = FALSE);		//"ABN은행     "
	int    fnAPP_TranDec071_071(int nTrCode = FALSE);		//"우체국      "
	int    fnAPP_TranDec025_081(int nTrCode = FALSE);		//"하나은행    "
	int    fnAPP_TranDec021_088(int nTrCode = FALSE);		//"신한은행    "
	int    fnAPP_TranDec105_105(int nTrCode = FALSE);		//"KEB하나카드 "
	int    fnAPP_TranDec106_106(int nTrCode = FALSE);		//"국민카드    "
	int    fnAPP_TranDec107_107(int nTrCode = FALSE);		//"수협카드    "
	int    fnAPP_TranDec111_111(int nTrCode = FALSE);		//"농협비자    "
	int    fnAPP_TranDec120_120(int nTrCode = FALSE);		//"우리카드    "
	int    fnAPP_TranDec121_121(int nTrCode = FALSE);		//"조흥카드    "
	int    fnAPP_TranDec126_192(int nTrCode = FALSE);		//"신한카드    "
	int    fnAPP_TranDec127_127(int nTrCode = FALSE);		//"씨티카드    "
	int    fnAPP_TranDec134_034(int nTrCode = FALSE);		//"광주카드    "
	int    fnAPP_TranDec135_135(int nTrCode = FALSE);		//"제주비자    "
	int    fnAPP_TranDec137_037(int nTrCode = FALSE);		//"전북카드    "
	int    fnAPP_TranDec181_105(int nTrCode = FALSE);		//"KEB하나카드 "
	int    fnAPP_TranDec184_184(int nTrCode = FALSE);		//"삼성카드    "
	int    fnAPP_TranDec192_192(int nTrCode = FALSE);		//"신한카드    "
	int    fnAPP_TranDec193_193(int nTrCode = FALSE);		//"현대카드    "
	int    fnAPP_TranDec194_194(int nTrCode = FALSE);		//"BC카드      "
	int    fnAPP_TranDec195_195(int nTrCode = FALSE);		//"롯데카드    "
	int    fnAPP_TranDec209_209(int nTrCode = FALSE);		//"유안타증권	 "
	int    fnAPP_TranDec218_218(int nTrCode = FALSE);		//"현대증권    "
	int    fnAPP_TranDec230_230(int nTrCode = FALSE);		//"미래에증권  "
	int    fnAPP_TranDec238_238(int nTrCode = FALSE);		//"대우증권    "
	int    fnAPP_TranDec240_240(int nTrCode = FALSE);		//"삼성증권    "
	int    fnAPP_TranDec243_243(int nTrCode = FALSE);		//"한국투자    "
	int    fnAPP_TranDec247_247(int nTrCode = FALSE);		//"우리투자    "
	int    fnAPP_TranDec261_261(int nTrCode = FALSE);		//"교보증권    "
	int    fnAPP_TranDec262_262(int nTrCode = FALSE);		//"하이투자    "
	int    fnAPP_TranDec263_263(int nTrCode = FALSE);		//"HMC투자     "
	int    fnAPP_TranDec264_264(int nTrCode = FALSE);		//"키움증권    "
	int    fnAPP_TranDec265_265(int nTrCode = FALSE);		//"이트레이드  "
	int    fnAPP_TranDec266_266(int nTrCode = FALSE);		//"에스케이    "
	int    fnAPP_TranDec267_267(int nTrCode = FALSE);		//"대신증권    "
	int    fnAPP_TranDec268_268(int nTrCode = FALSE);		//"솔로몬투자  "
	int    fnAPP_TranDec269_269(int nTrCode = FALSE);		//"한화증권    "
	int    fnAPP_TranDec270_270(int nTrCode = FALSE);		//"하나대투    "
	int    fnAPP_TranDec278_278(int nTrCode = FALSE);		//"신한금융    "
	int    fnAPP_TranDec279_279(int nTrCode = FALSE);		//"동부증권    "
	int    fnAPP_TranDec280_280(int nTrCode = FALSE);		//"유진투자    "
	int    fnAPP_TranDec287_287(int nTrCode = FALSE);		//"메리츠증권  "
	int    fnAPP_TranDec289_289(int nTrCode = FALSE);		//"NH증권      "
	int    fnAPP_TranDec290_290(int nTrCode = FALSE);		//"부국증권    "
	int    fnAPP_TranDec291_291(int nTrCode = FALSE);		//"신영증권    "
	int    fnAPP_TranDec292_292(int nTrCode = FALSE);		//"LIG증권	   "
	int    fnAPP_TranDec294_294(int nTrCode = FALSE);		//"LIG증권	   "
	int    fnAPP_TranDec384_364(int nTrCode = FALSE);		//"삼성증권    "
	int    fnAPP_TranDec484_484(int nTrCode = FALSE);		//"CJ시스템    "
	int    fnAPP_TranDec601_601(int nTrCode = FALSE);		//"발렉스      "
	int    fnAPP_TranDec701_701(int nTrCode = FALSE);		//"KTF         "
	int    fnAPP_TranDec702_702(int nTrCode = FALSE);		//"LGT         "
	int    fnAPP_TranDec703_703(int nTrCode = FALSE);		//"SKT         "


	////////////////////////////////////////////////////
	int		fnAPP_Tran_Init(int nFlag = FALSE);
	int		fnAPP_Tran_TxSend( int nTranCode, int nSendSec = LINE_BIDSEND_TIME);
	int		fnAPP_Tran_TxSend2( int nTranCode, int nSendSec = LINE_BIDSEND_TIME, int nSendFlag = FALSE);
	int		fnAPP_Tran_RxRecv( int nTranCode, int nRecvSec = LINE_BIDSEND_TIME);
	int		fnAPP_Tran_RxRecv2( int nTranCode, int nRecvSec = LINE_BIDSEND_TIME, int nRecvFlag = FALSE);
	
public:
	//**--
	BOOL	fnAPP_IsBCC(BYTE* pbyBuf, DWORD dwBufSize);
	BYTE	fnAPP_SetBCC(BYTE* pbyBuf, DWORD dwBufSize);	
	int     fnAPP_CardBank_C(char * OrgCode);
	int		fnAPP_Certificate_Confirm(int n = 0);
	int		fnAPP_CreditUnUsed_Check(int n = 0);
	int		fnAPP_TranHostStatus_Confirm(int n = 0);
	int		Om_GetCurrentActionRemain(int nFlag = FALSE);
	int		Om_SetActionGapRemainAsycSend(int nFlag = FALSE);	
public:
	CString	strCT(void *inbuff,int slen);
	int		strLTrim(char *sBuff, char *dBuff, int slen);
	int		strRTrim(char *sBuff, char *dBuff, int slen);
	int		strTrim(char *sBuff, char *dBuff, int slen);
	CString fnAPP_GetAtmSysInfo(int nSystmInfo);
//common
public:
	int		fnAPP_Set_SystemTime(LPCTSTR szDate);
	int		fnAPP_Get_Ip_Address(int nTranCode = FALSE);
	CString	fnAPP_Get_BankName(char* szOrgCode);
	CString	fnAPP_Get_BankName(int nOrgCode);
	CString	fnAPP_Get_BankOrgCode(int nCardBank);
	int		fnAPP_Get_BankOrgCodeInt(int nCardBank);
	int		fnAPP_Get_BankOrgEncCvtInt(int nCardBank);
	int		fnAPP_Get_BankEncOrgCvtInt(int nCardBank);
	CString	fnAPP_Get_BankOrgCode(char* szCardBank);
	CString	fnAPP_Get_CreditName(BYTE* szOrgCode);
	CString	fnAPP_Get_CreditName(int nBinNo);
	CString	fnAPP_Get_CreditOrgCode(BYTE* byBinNo);
	CString	fnAPP_Get_BankOrgEnc(int nCardBank, int nTranCode = FALSE);
	int		fnAPP_Get_BankOrgEncInt(int nCardBank, int nTranCode = FALSE);

public:
	/* Send */
	int	fn_FTX_TRANID_0006(int nTranCode, int nSec = FALSE);    //라인전문
	int	fn_FTX_TRANID_A010(int nTranCode, int nSec = FALSE);    //0800	0810	단말기 개국전문                                                                                                                                                                       
	int	fn_FTX_TRANID_A020(int nTranCode, int nSec = FALSE);    //0800	0810	단말기 상태전문                                                                                                                                                                       
	int	fn_FTX_TRANID_2190(int nTranCode, int nSec = FALSE, int nEncFlag = FALSE);    //0800	0810	단말기 승인거래                                                                                                                                                                       
	int	fn_FTX_TRANID_A040(int nTranCode, int nSec = FALSE);    //0800	0810	단말기 제어전문                                                                                                                                                                       
	int	fn_FTX_TRANID_A060(int nTranCode, int nSec = FALSE);    //0800	0810	단말기 시제전문                                                                                                                                                                       

	int	fn_FTX_TRANID_7210(int nTranCode, int nSec = FALSE);    //0200	0210	수수료조회
	int	fn_FTX_TRANID_4210(int nTranCode, int nSec = FALSE);    //0200	0210	예금출금(통장)                                                                                                                                                                            
	int	fn_FTX_TRANID_4310(int nTranCode, int nSec = FALSE);    //0200	0210	예금출금                                                                                                                                                                                  
	int	fn_FTX_TRANID_4320(int nTranCode, int nSec = FALSE);    //5400	5410	예금출금취소                                                                                                                                                                              
	int	fn_FTX_TRANID_4330(int nTranCode, int nSec = FALSE);    //0200	0210	서비스출금                                                                                                                                                                                
	int	fn_FTX_TRANID_4340(int nTranCode, int nSec = FALSE);    //5400	5410	서비스출금취소                                                                                                                                                                            
	int	fn_FTX_TRANID_4350(int nTranCode, int nSec = FALSE);    //0200	0210	서비스이체                                                                                                                                                                                
	int	fn_FTX_TRANID_4370(int nTranCode, int nSec = FALSE);    //0200	0210	계좌이체(자행)                                                                                                                                                                                  
	int	fn_FTX_TRANID_4390(int nTranCode, int nSec = FALSE);    //0200	0210	계좌이체   
	int	fn_FTX_TRANID_7260(int nTranCode, int nSec = FALSE);    //0200	0210	수취계좌조회   	
	int	fn_FTX_TRANID_4410(int nTranCode, int nSec = FALSE);    //0200	0210	카드입금
 	int	fn_FTX_TRANID_7230(int nTranCode, int nSec = FALSE);    //0200	0210	서비스조회   
 	int	fn_FTX_TRANID_7220(int nTranCode, int nSec = FALSE);    //0200	0210	예금조회
	int	fn_FTX_TRANID_7250(int nTranCode, int nSec = FALSE);    //0200	0210	서비스이체조회  
	int	fn_FTX_TRANID_7200(int nTranCode, int nSec = FALSE);    //0200	0210	입금가능여부조회   
	
	//현금서비스 이자율 조회  
	int fn_FTX_TRANID_7380(int nTranCode, int nSec = FALSE);    //0800	0810	현금서비스 출금 이자율 조회
	int fn_FTX_TRANID_7530(int nTranCode, int nSec = FALSE);    //0800	0810	해외
 	int fn_FTX_EMVSendMake_Lib01(int nTranCode, int nSec = FALSE);//EMVSendLib

// 	int	fn_FTX_TRANID_4610(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money 제휴충전                                                                                                                                                                          
// 	int	fn_FTX_TRANID_4620(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money 제휴충전취소                                                                                                                                                                      
// 	int	fn_FTX_TRANID_4611(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money 비제휴충전                                                                                                                                                                        
// 	int	fn_FTX_TRANID_4621(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money 비제휴충전취소                                                                                                                                                                    
// 	int	fn_FTX_TRANID_4631(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money 마일리지조회                                                                                                                                                                      
// 	int	fn_FTX_TRANID_4630(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money 마일리지충전                                                                                                                                                                      
// 	int	fn_FTX_TRANID_4640(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money 마일리지취소                                                                                                                                                                      
// 	int	fn_FTX_TRANID_4650(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money비제휴 예금출금                                                                                                                                                                    
// 	int	fn_FTX_TRANID_4660(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money비제휴 예금출금취소                                                                                                                                                                
// 	int	fn_FTX_TRANID_4670(int nTranCode, int nSec = FALSE);    //0200	0210	하이패스 충전요청                                                                                                                                                                         
// 	int	fn_FTX_TRANID_4671(int nTranCode, int nSec = FALSE);    //0200	0210	하이패스 충전확인                                                                                                                                                                         
// 	int	fn_FTX_TRANID_4680(int nTranCode, int nSec = FALSE);    //0200	0210	하이패스 충전복구                                                                                                                                                                         
// 	int	fn_FTX_TRANID_4681(int nTranCode, int nSec = FALSE);    //0200	0210	하이패스 충전복구조회                                                                                                                                                                     
// 	int	fn_FTX_TRANID_4682(int nTranCode, int nSec = FALSE);    //0200	0210	하이패스 충전복구확인                                                                                                                                                                     
                                                                                                                                                                                  
// 	int	fn_FTX_TRANID_4720(int nTranCode, int nSec = FALSE);    //0200	0210	통장입금 
// 	int	fn_FTX_TRANID_4730(int nTranCode, int nSec = FALSE);    //0200	0210	통장정리                                                                                                                                                                                  
// 	int	fn_FTX_TRANID_4810(int nTranCode, int nSec = FALSE);    //0200	0210	공과금납부                                                                                                                                                                                
// 	int	fn_FTX_TRANID_4811(int nTranCode, int nSec = FALSE);    //0200	0210	지방세 간편납부(MS)                                                                                                                                                                       
// 	int	fn_FTX_TRANID_4812(int nTranCode, int nSec = FALSE);    //0200	0210	지방세 간편납부(신용카드)                                                                                                                                                                 
// 	int	fn_FTX_TRANID_4813(int nTranCode, int nSec = FALSE);    //0200	0210	IC카드 공과금납부                                                                                                                                                                         
// 	int	fn_FTX_TRANID_4814(int nTranCode, int nSec = FALSE);    //0200	0210	지방세 간편납부(IC)                                                                                                                                                                       
// 	int	fn_FTX_TRANID_4820(int nTranCode, int nSec = FALSE);    //0200	0210	자동좌석/주문결제                                                                                                                                                                         
// 	int	fn_FTX_TRANID_4821(int nTranCode, int nSec = FALSE);    //0200	-	출력보고                                                                                                                                                                                  
// 	int	fn_FTX_TRANID_4822(int nTranCode, int nSec = FALSE);    //0200	0210	발권정보 출력                                                                                                                                                                             
// 	int	fn_FTX_TRANID_4830(int nTranCode, int nSec = FALSE);    //0200	0210	TC 예매                                                                                                                                                                                   
                                                                                                                                                                                                                                                                                                                                           
// 	int	fn_FTX_TRANID_7120(int nTranCode, int nSec = FALSE);    //0200	0210	예금조회(통장)                                                                                                                                                                            
// 	int	fn_FTX_TRANID_7124(int nTranCode, int nSec = FALSE);    //0200	0210	대체출금가능조회(통장)                                                                                                                                                                    
// 	int	fn_FTX_TRANID_7125(int nTranCode, int nSec = FALSE);    //0200	0210	은행이체출금가능조회(통장)                                                                                                                                                                
// 	int	fn_FTX_TRANID_7126(int nTranCode, int nSec = FALSE);    //0200	0210	입금가능여부조회(통장) 
// 	int	fn_FTX_TRANID_7236(int nTranCode, int nSec = FALSE);    //0200	0210	입금가능여부조회(무매체)
                                                                                                                                                                                  
// 	int	fn_FTX_TRANID_7221(int nTranCode, int nSec = FALSE);    //0200	0210	해외예금잔액조회                                                                                                                                                                          
// 	int	fn_FTX_TRANID_7222(int nTranCode, int nSec = FALSE);    //0200	0210	해외당좌거래잔액조회                                                                                                                                                                      
// 	int	fn_FTX_TRANID_7224(int nTranCode, int nSec = FALSE);    //0200	0210	대체출금가능조회                                                                                                                                                                          
// 	int	fn_FTX_TRANID_7225(int nTranCode, int nSec = FALSE);    //0200	0210	은행이체출금가능조회                                                                                                                                                                      
                                                                                                                                                                                                                                                                                                                                           
// 	int	fn_FTX_TRANID_7228(int nTranCode, int nSec = FALSE);    //0200	0210	지점정보조회                                                                                                                                                                              
                                                                                                                                                                             
// 	int	fn_FTX_TRANID_7231(int nTranCode, int nSec = FALSE);    //0200	0210	해외현금서비스조회                                                                                                                                                                        
// 	int	fn_FTX_TRANID_7240(int nTranCode, int nSec = FALSE);    //0200	0210	보험계약대출조회                                                                                                                                                                          
// 	int	fn_FTX_TRANID_7241(int nTranCode, int nSec = FALSE);    //0200	0210	캐피탈론잔액조회                                                                                                                                                                          
                                                                                                                                                                          
                                                                                                                                                                           
// 	int	fn_FTX_TRANID_7261(int nTranCode, int nSec = FALSE);    //0200	0210	수취계좌조회(통장)  
// 	int	fn_FTX_TRANID_7276(int nTranCode, int nSec = FALSE);    //0200	0210	수취계좌조회(통장)
// 	int	fn_FTX_TRANID_7262(int nTranCode, int nSec = FALSE);    //0200	0210	계좌성명조회(e-경조 수취조회)                                                                                                                                                                                                                                                                                                                            
// 	int	fn_FTX_TRANID_7266(int nTranCode, int nSec = FALSE);    //0200	0210	실명확인                                                                                                                                                                                  
// 	int	fn_FTX_TRANID_7267(int nTranCode, int nSec = FALSE);    //0200	0210	받을금액조회                                                                                                                                                                              
// 	int	fn_FTX_TRANID_7268(int nTranCode, int nSec = FALSE);    //0200	0210	인증번호받기                                                                                                                                                                              
// 	int	fn_FTX_TRANID_7269(int nTranCode, int nSec = FALSE);    //0200	0210	수취계좌조회(폰번호/받기)                                                                                                                                                                 
// 	int	fn_FTX_TRANID_7280(int nTranCode, int nSec = FALSE);    //0200	0210	롯데 론서비스 조회                                                                                                                                                                        
// 	int	fn_FTX_TRANID_7281(int nTranCode, int nSec = FALSE);    //0200	0210	롯데 론서비스 지급조회                                                                                                                                                                    
// 	int	fn_FTX_TRANID_7290(int nTranCode, int nSec = FALSE);    //0200	0210	외환 해외송금 계좌 조회                                                                                                                                                                   
// 	int	fn_FTX_TRANID_7300(int nTranCode, int nSec = FALSE);    //0200	0210	외환 해외송금 결과 조회                                                                                                                                                                   
// 	int	fn_FTX_TRANID_7310(int nTranCode, int nSec = FALSE);    //0200	0210	신용정보조회                                                                                                                                                                              
// 	int	fn_FTX_TRANID_7311(int nTranCode, int nSec = FALSE);    //0200	0210	대출정보조회                                                                                                                                                                              
// 	int	fn_FTX_TRANID_7312(int nTranCode, int nSec = FALSE);    //0200	0210	CB연체정보조회                                                                                                                                                                            
// 	int	fn_FTX_TRANID_7313(int nTranCode, int nSec = FALSE);    //0200	0210	신용카드 승인(한신정)                                                                                                                                                                     
// 	int	fn_FTX_TRANID_7314(int nTranCode, int nSec = FALSE);    //0200	0210	신용카드 결제                                                                                                                                                                             
// 	int	fn_FTX_TRANID_7315(int nTranCode, int nSec = FALSE);    //0200	0210	신용정보 스코어 조회                                                                                                                                                                      
// 	int	fn_FTX_TRANID_7316(int nTranCode, int nSec = FALSE);    //0200	0210	신용카드 승인(현대캐피탈)                                                                                                                                                                 
// 	int	fn_FTX_TRANID_7317(int nTranCode, int nSec = FALSE);    //0200	0210	대출가능금액조회 - 진행                                                                                                                                                                   
// 	int	fn_FTX_TRANID_7318(int nTranCode, int nSec = FALSE);    //0200	0210	대출가능금액조회 - 종료                                                                                                                                                                                                                                                                                                                                                 
// 	int	fn_FTX_TRANID_7320(int nTranCode, int nSec = FALSE);    //0200	0210	공과금납부조회                                                                                                                                                                            
// 	int	fn_FTX_TRANID_7321(int nTranCode, int nSec = FALSE);    //0200	0210	공과금납부처리결과조회                                                                                                                                                                    
// 	int	fn_FTX_TRANID_7322(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부조회(MS)                                                                                                                                                                    
// 	int	fn_FTX_TRANID_7323(int nTranCode, int nSec = FALSE);    //0200	0210	IC카드 공과금납부조회                                                                                                                                                                     
// 	int	fn_FTX_TRANID_7324(int nTranCode, int nSec = FALSE);    //0200	0210	IC카드 공과금납부처리결과조회                                                                                                                                                             
// 	int	fn_FTX_TRANID_7325(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부조회(IC)                                                                                                                                                                    
// 	int	fn_FTX_TRANID_7326(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부결과조회(MS)                                                                                                                                                                
// 	int	fn_FTX_TRANID_7327(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부결과조회(IC)                                                                                                                                                                
// 	int	fn_FTX_TRANID_7328(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부조회(신용카드)                                                                                                                                                              
// 	int	fn_FTX_TRANID_7329(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부결과조회(신용카드)                                                                                                                                                          
// 	int	fn_FTX_TRANID_7330(int nTranCode, int nSec = FALSE);    //0200	0210	통장정리조회                                                                                                                                                                              
                                                                                                                                                                       
                                                                                                                                                                        
                                                                                                             

// 	int fn_FTX_TRANID_4717(int nTranCode, int nSec = FALSE);    //0800	0810	하이패스 입금 
// 	int fn_FTX_TRANID_7229(int nTranCode, int nSec = FALSE);    //0800	0810	통장잔액 조회 
// 	int fn_FTX_EMVSendMake_Lib01(int nTranCode, int nSec = FALSE);//EMVSendLib
    /* Recv */         
	int	fn_FRX_TRANID_A010(int nTranCode, int nSec = FALSE);    //0800	0810	단말기 개국전문                                                                                                                                                                       
	int	fn_FRX_TRANID_A020(int nTranCode, int nSec = FALSE);    //0800	0810	단말기 상태전문                                                                                                                                                                       
	int	fn_FRX_TRANID_2190(int nTranCode, int nSec = FALSE, int nEncFlag = FALSE);    //0800	0810	단말기 승인거래                                                                                                                                                                       
	int	fn_FRX_TRANID_A060(int nTranCode, int nSec = FALSE);    //0800	0810	단말기 시제전문                                                                               	
	int	fn_FRX_TRANID_4210(int nTranCode, int nSec = FALSE);    //0200	0210	예금출금(통장)                                                                                                                                                                            
//	int	fn_FRX_TRANID_4220(int nTranCode, int nSec = FALSE);    //5400	5410	출금취소(통장)
	int	fn_FRX_TRANID_4310(int nTranCode, int nSec = FALSE);    //0200	0210	예금출금                                                                                                                                                                                  
	int	fn_FRX_TRANID_4320(int nTranCode, int nSec = FALSE);    //5400	5410	예금출금취소                                                                                                                                                                              
	int	fn_FRX_TRANID_4330(int nTranCode, int nSec = FALSE);    //0200	0210	서비스출금                                                                                                                                                                                
	int	fn_FRX_TRANID_4340(int nTranCode, int nSec = FALSE);    //5400	5410	서비스출금취소                                                                                                                                                                            
	int	fn_FRX_TRANID_4350(int nTranCode, int nSec = FALSE);    //0200	0210	서비스이체                                                                                                                                                                                
	int	fn_FRX_TRANID_4370(int nTranCode, int nSec = FALSE);    //0200	0210	계좌이체 (자행)                                                                                                                                                                                 
	int	fn_FRX_TRANID_4390(int nTranCode, int nSec = FALSE);    //0200	0210	계좌이체  
	int	fn_FRX_TRANID_4410(int nTranCode, int nSec = FALSE);    //0200	0210	카드입금    
	int fn_FRX_TRANID_7380(int nTranCode, int nSec = FALSE);    //0800	0810	현금서비스 출금 이자율 조회
	int fn_FRX_TRANID_7530(int nTranCode, int nSec = FALSE);    //0800	0810	현금서비스 이체 이자율 조회
 	int	fn_FRX_TRANID_7210(int nTranCode, int nSec = FALSE);    //0200	0210	수수료조회     
 	int	fn_FRX_TRANID_7230(int nTranCode, int nSec = FALSE);    //0200	0210	서비스조회
 	int	fn_FRX_TRANID_7220(int nTranCode, int nSec = FALSE);    //0200	0210	예금조회  
    int	fn_FRX_TRANID_7250(int nTranCode, int nSec = FALSE);    //0200	0210	서비스이체조회  
	int fn_FRX_TRANID_7260(int nTranCode, int nSec = FALSE);    //0200	0210	수취계좌조회     
	int	fn_FRX_TRANID_7200(int nTranCode, int nSec = FALSE);    //0200	0210	입금가능여부조회  	
	//////////////////////////////////////////////////////////////////////////////////////////////
	int fn_FRX_EMVRecv_Lib(int nTranCode, int nSec = FALSE);    //EmvLib
	
// 	int	fn_FRX_TRANID_4391(int nTranCode, int nSec = FALSE);    //0200	0210	계좌이체(온누리상품권 이체판매)                                                                                                                                                           
// 	int	fn_FRX_TRANID_4392(int nTranCode, int nSec = FALSE);    //0200	0210	e-경조 계좌이체                                                                                                                                                                           
// 	int	fn_FRX_TRANID_4393(int nTranCode, int nSec = FALSE);    //0200	0210	금융공동망IC카드 계좌이체                                                                                                                                                                 
// 	int	fn_FRX_TRANID_4394(int nTranCode, int nSec = FALSE);    //0200	0210	IC카드 계좌이체(온누리상품권 이체판매)                                                                                                                                                    
// 	int	fn_FRX_TRANID_4395(int nTranCode, int nSec = FALSE);    //0200	0210	e-경조 수납이체                                                                                                                                                                           
// 	int	fn_FRX_TRANID_4399(int nTranCode, int nSec = FALSE);    //0200	0210	계좌이체(폰번호/받기)                                                                                                                                                                     
// 	int	fn_FRX_TRANID_4520(int nTranCode, int nSec = FALSE);    //0200	0210	현대 론서비스 지급                                                                                                                                                                        
// 	int	fn_FRX_TRANID_4530(int nTranCode, int nSec = FALSE);    //5400	5410	현대 론서비스 지급취소                                                                                                                                                                    
// 	int	fn_FRX_TRANID_4540(int nTranCode, int nSec = FALSE);    //0200	0210	롯데 론서비스 지급                                                                                                                                                                        
// 	int	fn_FRX_TRANID_4550(int nTranCode, int nSec = FALSE);    //5400	5410	롯데 론서비스 지급취소                                                                                                                                                                    
// 	int	fn_FRX_TRANID_4560(int nTranCode, int nSec = FALSE);    //0200	0210	외환 해외송금                                                                                                                                                                             
// 	int	fn_FRX_TRANID_4570(int nTranCode, int nSec = FALSE);    //0200	0210	신한카드론 지급                                                                                                                                                                           
// 	int	fn_FRX_TRANID_4571(int nTranCode, int nSec = FALSE);    //0200	0210	신한카드론 이체                                                                                                                                                                           
// 	int	fn_FRX_TRANID_4572(int nTranCode, int nSec = FALSE);    //0200	0210	신한카드론 약정                                                                                                                                                                           
// 	int	fn_FRX_TRANID_4580(int nTranCode, int nSec = FALSE);    //0200	0210	신한카드론 지급취소                                                                                                                                                                       
// 	int	fn_FRX_TRANID_4610(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money 제휴충전                                                                                                                                                                          
// 	int	fn_FRX_TRANID_4620(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money 제휴충전취소                                                                                                                                                                      
// 	int	fn_FRX_TRANID_4611(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money 비제휴충전                                                                                                                                                                        
// 	int	fn_FRX_TRANID_4621(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money 비제휴충전취소                                                                                                                                                                    
// 	int	fn_FRX_TRANID_4631(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money 마일리지조회                                                                                                                                                                      
// 	int	fn_FRX_TRANID_4630(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money 마일리지충전                                                                                                                                                                      
// 	int	fn_FRX_TRANID_4640(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money 마일리지취소                                                                                                                                                                      
// 	int	fn_FRX_TRANID_4650(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money비제휴 예금출금                                                                                                                                                                    
// 	int	fn_FRX_TRANID_4660(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money비제휴 예금출금취소                                                                                                                                                                
// 	int	fn_FRX_TRANID_4670(int nTranCode, int nSec = FALSE);    //0200	0210	하이패스 충전요청                                                                                                                                                                         
// 	int	fn_FRX_TRANID_4671(int nTranCode, int nSec = FALSE);    //0200	0210	하이패스 충전확인                                                                                                                                                                         
// 	int	fn_FRX_TRANID_4680(int nTranCode, int nSec = FALSE);    //0200	0210	하이패스 충전복구                                                                                                                                                                         
// 	int	fn_FRX_TRANID_4681(int nTranCode, int nSec = FALSE);    //0200	0210	하이패스 충전복구조회                                                                                                                                                                     
// 	int	fn_FRX_TRANID_4682(int nTranCode, int nSec = FALSE);    //0200	0210	하이패스 충전복구확인                                                                                                                                                                     
// 
// 	
// 	int	fn_FRX_TRANID_4711(int nTranCode, int nSec = FALSE);    //0200	0210	무매체입금                                                                                                                                                                                
// 	int	fn_FRX_TRANID_4712(int nTranCode, int nSec = FALSE);    //0200	0210	이마트슈퍼 매출금입금                                                                                                                                                                     
// 	int	fn_FRX_TRANID_4713(int nTranCode, int nSec = FALSE);    //0200	0210	IC카드입금                                                                                                                                                                                
// 	int	fn_FRX_TRANID_4719(int nTranCode, int nSec = FALSE);    //5400	5410	입금취소                                                                                                                                                                                  
// 	int	fn_FRX_TRANID_4720(int nTranCode, int nSec = FALSE);    //0200	0210	통장입금 
// 	int	fn_FRX_TRANID_4716(int nTranCode, int nSec = FALSE);    //0200	0210	무매체입금 
// 	int	fn_FRX_TRANID_4730(int nTranCode, int nSec = FALSE);    //0200	0210	통장정리                                                                                                                                                                                  
// 	int	fn_FRX_TRANID_4810(int nTranCode, int nSec = FALSE);    //0200	0210	공과금납부                                                                                                                                                                                
// 	int	fn_FRX_TRANID_4811(int nTranCode, int nSec = FALSE);    //0200	0210	지방세 간편납부(MS)                                                                                                                                                                       
// 	int	fn_FRX_TRANID_4812(int nTranCode, int nSec = FALSE);    //0200	0210	지방세 간편납부(신용카드)                                                                                                                                                                 
// 	int	fn_FRX_TRANID_4813(int nTranCode, int nSec = FALSE);    //0200	0210	IC카드 공과금납부                                                                                                                                                                         
// 	int	fn_FRX_TRANID_4814(int nTranCode, int nSec = FALSE);    //0200	0210	지방세 간편납부(IC)                                                                                                                                                                       
// 	int	fn_FRX_TRANID_4820(int nTranCode, int nSec = FALSE);    //0200	0210	자동좌석/주문결제                                                                                                                                                                         
// 	int	fn_FRX_TRANID_4821(int nTranCode, int nSec = FALSE);    //0200	-	출력보고                                                                                                                                                                                  
// 	int	fn_FRX_TRANID_4822(int nTranCode, int nSec = FALSE);    //0200	0210	발권정보 출력                                                                                                                                                                             
// 	int	fn_FRX_TRANID_4830(int nTranCode, int nSec = FALSE);    //0200	0210	TC 예매                                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4840(int nTranCode, int nSec = FALSE);    //0200	0210	신용카드 승인(도로공사)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4850(int nTranCode, int nSec = FALSE);    //5400	5410	신용카드 취소(도로공사)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4841(int nTranCode, int nSec = FALSE);    //0200	0210	신용카드 승인(에버랜드)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4851(int nTranCode, int nSec = FALSE);    //5400	5410	신용카드 취소(에버랜드)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4842(int nTranCode, int nSec = FALSE);    //0200	0210	신용카드 승인(효성티켓)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4852(int nTranCode, int nSec = FALSE);    //5400	5410	신용카드 취소(효성티켓)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4860(int nTranCode, int nSec = FALSE);    //0200	0210	상품권판매정보통보                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7000(int nTranCode, int nSec = FALSE);    //0200	0210	다계좌조회                                                                                                                                                                                
// 	int	fn_FRX_TRANID_7004(int nTranCode, int nSec = FALSE);    //0200	0210	다계좌조회(입금)                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7005(int nTranCode, int nSec = FALSE);    //0200	0210	다계좌조회(투신)                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7006(int nTranCode, int nSec = FALSE);    //0200	0210	다계좌조회(대체)                                                                                                                                                                          
                                                                                                                                                                           
// 	int	fn_FRX_TRANID_7101(int nTranCode, int nSec = FALSE);    //0200	0210	수수료조회(통장)
// 	int	fn_FRX_TRANID_7103(int nTranCode, int nSec = FALSE);    //0200	0210	IC카드 수수료조회
// 	int	fn_FRX_TRANID_7104(int nTranCode, int nSec = FALSE);    //0200	0210	해외취급수수료조회
// 	int	fn_FRX_TRANID_7105(int nTranCode, int nSec = FALSE);    //0200	0210	해외BC취급수수료조회 
// 	int	fn_FRX_TRANID_7106(int nTranCode, int nSec = FALSE);    //0200	0210	수수료조회(무매체)
// 	int	fn_FRX_TRANID_7120(int nTranCode, int nSec = FALSE);    //0200	0210	예금조회(통장)                                                                                                                                                                            
// 	int	fn_FRX_TRANID_7124(int nTranCode, int nSec = FALSE);    //0200	0210	대체출금가능조회(통장)                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7125(int nTranCode, int nSec = FALSE);    //0200	0210	은행이체출금가능조회(통장)                                                                                                                                                                
// 	int	fn_FRX_TRANID_7126(int nTranCode, int nSec = FALSE);    //0200	0210	입금가능여부조회(통장) 
// 	int	fn_FRX_TRANID_7236(int nTranCode, int nSec = FALSE);    //0200	0210	입금가능여부조회(무매체)
                                                                                                                                                                            
// 	int	fn_FRX_TRANID_7221(int nTranCode, int nSec = FALSE);    //0200	0210	해외예금잔액조회                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7222(int nTranCode, int nSec = FALSE);    //0200	0210	해외당좌거래잔액조회                                                                                                                                                                      
// 	int	fn_FRX_TRANID_7224(int nTranCode, int nSec = FALSE);    //0200	0210	대체출금가능조회                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7225(int nTranCode, int nSec = FALSE);    //0200	0210	은행이체출금가능조회                                                                                                                                                                      
                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7228(int nTranCode, int nSec = FALSE);    //0200	0210	지점정보조회                                                                                                                                                                              
                                                                                                                                                                                
// 	int	fn_FRX_TRANID_7231(int nTranCode, int nSec = FALSE);    //0200	0210	해외현금서비스조회                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7232(int nTranCode, int nSec = FALSE);    //0200	0210	중국은련서비스 잔액조회 
// 	int	fn_FRX_TRANID_7233(int nTranCode, int nSec = FALSE);    //0200	0210	해외 예금 조회 취소 #N0235                                                                                                                                                                
// 	int	fn_FRX_TRANID_7235(int nTranCode, int nSec = FALSE);    //0200	0210	해외 신용 조회 취소 #N0235 
// 	int	fn_FRX_TRANID_7237(int nTranCode, int nSec = FALSE);    //0200	0210	해외 당좌 조회 취소 #N0235 
// 	int	fn_FRX_TRANID_7240(int nTranCode, int nSec = FALSE);    //0200	0210	보험계약대출조회                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7241(int nTranCode, int nSec = FALSE);    //0200	0210	캐피탈론잔액조회                                                                                                                                                                          
                                                                                                                                                                           
                                                                                                                                                                       
// 	int	fn_FRX_TRANID_7261(int nTranCode, int nSec = FALSE);    //0200	0210	수취계좌조회(통장)                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7262(int nTranCode, int nSec = FALSE);    //0200	0210	계좌성명조회(e-경조 수취조회)                                                                                                                                                             
// 	int	fn_FRX_TRANID_7265(int nTranCode, int nSec = FALSE);    //0200	0210	e-경조 수납이체 계좌등록                                                                                                                                                                  
// 	int	fn_FRX_TRANID_7266(int nTranCode, int nSec = FALSE);    //0200	0210	실명확인                                                                                                                                                                                  
// 	int	fn_FRX_TRANID_7267(int nTranCode, int nSec = FALSE);    //0200	0210	받을금액조회                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7268(int nTranCode, int nSec = FALSE);    //0200	0210	인증번호받기                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7269(int nTranCode, int nSec = FALSE);    //0200	0210	수취계좌조회(폰번호/받기)
// 	int	fn_FRX_TRANID_7276(int nTranCode, int nSec = FALSE);    //0200	0210	수취계좌조회(무매체)
// 	int	fn_FRX_TRANID_7280(int nTranCode, int nSec = FALSE);    //0200	0210	롯데 론서비스 조회                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7281(int nTranCode, int nSec = FALSE);    //0200	0210	롯데 론서비스 지급조회                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7290(int nTranCode, int nSec = FALSE);    //0200	0210	외환 해외송금 계좌 조회                                                                                                                                                                   
// 	int	fn_FRX_TRANID_7300(int nTranCode, int nSec = FALSE);    //0200	0210	외환 해외송금 결과 조회                                                                                                                                                                   
// 	int	fn_FRX_TRANID_7310(int nTranCode, int nSec = FALSE);    //0200	0210	신용정보조회                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7311(int nTranCode, int nSec = FALSE);    //0200	0210	대출정보조회                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7312(int nTranCode, int nSec = FALSE);    //0200	0210	CB연체정보조회                                                                                                                                                                            
// 	int	fn_FRX_TRANID_7313(int nTranCode, int nSec = FALSE);    //0200	0210	신용카드 승인(한신정)                                                                                                                                                                     
// 	int	fn_FRX_TRANID_7314(int nTranCode, int nSec = FALSE);    //0200	0210	신용카드 결제                                                                                                                                                                             
// 	int	fn_FRX_TRANID_7315(int nTranCode, int nSec = FALSE);    //0200	0210	신용정보 스코어 조회                                                                                                                                                                      
// 	int	fn_FRX_TRANID_7316(int nTranCode, int nSec = FALSE);    //0200	0210	신용카드 승인(현대캐피탈)                                                                                                                                                                 
// 	int	fn_FRX_TRANID_7317(int nTranCode, int nSec = FALSE);    //0200	0210	대출가능금액조회 - 진행                                                                                                                                                                   
// 	int	fn_FRX_TRANID_7318(int nTranCode, int nSec = FALSE);    //0200	0210	대출가능금액조회 - 종료                                                                                                                                                                   
// 	int	fn_FRX_TRANID_7319(int nTranCode, int nSec = FALSE);    //0200	0210	사전스크린                                                                                                                                                                                
// 	int	fn_FRX_TRANID_7320(int nTranCode, int nSec = FALSE);    //0200	0210	공과금납부조회                                                                                                                                                                            
// 	int	fn_FRX_TRANID_7321(int nTranCode, int nSec = FALSE);    //0200	0210	공과금납부처리결과조회                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7322(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부조회(MS)                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7323(int nTranCode, int nSec = FALSE);    //0200	0210	IC카드 공과금납부조회                                                                                                                                                                     
// 	int	fn_FRX_TRANID_7324(int nTranCode, int nSec = FALSE);    //0200	0210	IC카드 공과금납부처리결과조회                                                                                                                                                             
// 	int	fn_FRX_TRANID_7325(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부조회(IC)                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7326(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부결과조회(MS)                                                                                                                                                                
// 	int	fn_FRX_TRANID_7327(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부결과조회(IC)                                                                                                                                                                
// 	int	fn_FRX_TRANID_7328(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부조회(신용카드)                                                                                                                                                              
// 	int	fn_FRX_TRANID_7329(int nTranCode, int nSec = FALSE);    //0200	0210	지방세간편납부결과조회(신용카드)                                                                                                                                                          
// 	int	fn_FRX_TRANID_7330(int nTranCode, int nSec = FALSE);    //0200	0210	통장정리조회                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7336(int nTranCode, int nSec = FALSE);    //0200	0210	투신매도잔액조회                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7337(int nTranCode, int nSec = FALSE);    //0200	0210	주문체결내역출력                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7338(int nTranCode, int nSec = FALSE);    //0200	0210	잔고조회출력                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7339(int nTranCode, int nSec = FALSE);    //0200	0210	거래내역출력                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7340(int nTranCode, int nSec = FALSE);    //0200	0210	TC 예매 가능 여부 조회                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7341(int nTranCode, int nSec = FALSE);    //0200	0210	티켓종류 조회                                                                                                                                                                             
// 	int	fn_FRX_TRANID_7342(int nTranCode, int nSec = FALSE);    //0200	0210	판매처정보 조회                                                                                                                                                                           
// 	int	fn_FRX_TRANID_7343(int nTranCode, int nSec = FALSE);    //0200	0210	일정/공지사항 조회                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7344(int nTranCode, int nSec = FALSE);    //0200	0210	권종정보 조회                                                                                                                                                                             
// 	int	fn_FRX_TRANID_7345(int nTranCode, int nSec = FALSE);    //0200	0210	출발시간 조회                                                                                                                                                                             
// 	int	fn_FRX_TRANID_7350(int nTranCode, int nSec = FALSE);    //0200	0210	아파트담보대출 - 진행                                                                                                                                                                     
// 	int	fn_FRX_TRANID_7351(int nTranCode, int nSec = FALSE);    //0200	0210	아파트담보대출 - 종료                                                                                                                                                                     
// 	int	fn_FRX_TRANID_7352(int nTranCode, int nSec = FALSE);    //0200	0210	전세자금대출 - 진행                                                                                                                                                                       
// 	int	fn_FRX_TRANID_7353(int nTranCode, int nSec = FALSE);    //0200	0210	전세자금대출 - 종료                                                                                                                                                                       
// 	int	fn_FRX_TRANID_7354(int nTranCode, int nSec = FALSE);    //0200	0210	휴대전화 승인(현대캐피탈)                                                                                                                                                                 
// 	int	fn_FRX_TRANID_7357(int nTranCode, int nSec = FALSE);    //0200	0210	고객연락처 등록(조인스랜드)                                                                                                                                                               
// 	int	fn_FRX_TRANID_7358(int nTranCode, int nSec = FALSE);    //0200	0210	주민번호,통신사구분,핸드폰번호입력(운세서비스)                                                                                                                                            
// 	int	fn_FRX_TRANID_7359(int nTranCode, int nSec = FALSE);    //0200	0210	인증번호입력(운세서비스)                                                                                                                                                                  
// 	int	fn_FRX_TRANID_7360(int nTranCode, int nSec = FALSE);    //0200	0210	한도상향조회                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7361(int nTranCode, int nSec = FALSE);    //0200	0210	행사등록조회                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7362(int nTranCode, int nSec = FALSE);    //0200	0210	예약할인등록                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7363(int nTranCode, int nSec = FALSE);    //0200	0210	대출한도증액등록                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7364(int nTranCode, int nSec = FALSE);    //0200	0210	신상보정조회                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7365(int nTranCode, int nSec = FALSE);    //0200	0210	신상보정등록                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7366(int nTranCode, int nSec = FALSE);    //0200	0210	한도상향등록                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7367(int nTranCode, int nSec = FALSE);    //0200	0210	행사등록                                                                                                                                                                                  
// 	int	fn_FRX_TRANID_7370(int nTranCode, int nSec = FALSE);    //0200	0210	대출가능상품조회                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7371(int nTranCode, int nSec = FALSE);    //0200	0210	지급상세조회                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7372(int nTranCode, int nSec = FALSE);    //0200	0210	이체상세조회                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7373(int nTranCode, int nSec = FALSE);    //0200	0210	약정상세조회                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7420(int nTranCode, int nSec = FALSE);    //0200	0210	아파트단지리시트조회                                                                                                                                                                  
// 	int	fn_FRX_TRANID_7421(int nTranCode, int nSec = FALSE);    //0200	0210	시세조회                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7425(int nTranCode, int nSec = FALSE);    //0200	0210	여부조회(현대)                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7426(int nTranCode, int nSec = FALSE);    //0200	0210	한도상향가능금액조회(현대)                                                                                                                                                            
// 	int	fn_FRX_TRANID_7427(int nTranCode, int nSec = FALSE);    //0200	0210	한도상향신청(현대)                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7428(int nTranCode, int nSec = FALSE);    //0200	0210	캠페인조회(현대)                                                                                                                                                                      
// 	int	fn_FRX_TRANID_7430(int nTranCode, int nSec = FALSE);    //0200	0210	현대 론서비스 조회                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7431(int nTranCode, int nSec = FALSE);    //0200	0210	현대 론서비스 지급조회                                                                                                                                                                
	


// 	//////////////////////////////////////////////////////////////////////////////////////////////
// 	//현금서비스 이자율 조회  
// 	int fn_FRX_TRANID_7380(int nTranCode, int nSec = FALSE);    //0800	0810	현금서비스 출금 이자율 조회
// 	int fn_FRX_TRANID_7530(int nTranCode, int nSec = FALSE);    //0800	0810	현금서비스 이체 이자율 조회
// 	//////////////////////////////////////////////////////////////////////////////////////////////
// 	int fn_FRX_TRANID_7229(int nTranCode, int nSec = FALSE);    //0800	0810	통장잔액 조회 
// 
// 	int fn_FRX_TRANID_4717(int nTranCode, int nSec = FALSE);    //0800	0810	하이패스 입금 

//	int fn_FRX_EMVRecv_Lib(int nTranCode, int nSec = FALSE);    //EmvLib
/////////////////////////////////////////////////////////////////////////////
#endif
