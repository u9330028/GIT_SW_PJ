/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranHostProc_H_INCLUDED_)
#define _TranHostProc_H_INCLUDED_




/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	�ۼ��ź���
/////////////////////////////////////////////////////////////////////////////
	int		AddSerialFlg;				// �Ϸù�ȣ����Flg
	int		TranSend;					// N���۽ſ���

	int		SendLength;					// �۽�Length
	int		SaveSendLength;				// ����۽�Length
	int		RecvLength;					// ����Length
	BYTE	SendBuffer[NETBUFFSIZE];	// �۽�Buffer
	BYTE	SaveSendBuffer[NETBUFFSIZE];// ����۽�Buffer
	BYTE	RecvBuffer[NETBUFFSIZE];	// ����Buffer


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
	BYTE	TranSerial[4];				// �۽Űŷ��Ϸù�ȣ
	BYTE	TranCashAmount[15];			// �۽Űŷ��ݾ�
	BYTE	TranCheckAmount[15];		// �۽Űŷ��ݾ�
	BYTE	TranInputID[4];				// �۽Űŷ�InputID
	BYTE	TranAccount[16];			// �۽Ű��¹�ȣ
} Send;

	BYTE	SaveCashAmount[11];			// �ŷ��ݾ�����(��Ұ���)
	BYTE	SaveCheckAmount[11];		// �ŷ��ݾ�����(��Ұ���)
	BYTE	SaveSerialNo[6];			// �ŷ��Ϸù�ȣ����(�����������)
	BYTE	SaveJiroNum[9];				// ���ι�ȣ����

	BYTE			SeedKey[263];		// ��ȣȭ Ű
//	QSLSession		*cl_sess;
//	QSLPubKey		*pubKey;
	unsigned char	SeedBuffer[NMSBUFFSIZE];// ��ȣȭ�� ���� ����
	int				SeedLength;			// ��ȣȭ�� ���� ����

	PSPRecv		PSPRecvData;			// DATA��/������Ʈ(�ŷ�)
	PPPRecv		PPPRecvData;			// DATA��/���������Ʈ(�ŷ�)
	PPPRecvNon	PPPRecvDataNon;			// DATA��/���������Ʈ(�ŷ�) - ����
	PPPRecvKb	PPPRecvDataKb;			// DATA��/���������Ʈ(�ŷ�) - ����
	PPWRecv		PPWRecvData;			// DATA��/����M/S WRITE(�ŷ�)
	PCWRecv		PCWRecvData;			// DATA��/ī��M/S WRITE(�ŷ�)
	PMERecv		PMERecvData;			// DATA��/�޼�������ǥ��(�ŷ�������)
	PMDRecv		PMDRecvData;			// DATA��/�޼���ǥ��(�ŷ�)
	ETCRecv		ETCRecvData;			// DATA��/��Ÿ�ŷ����(�ŷ�)

										// Send / Receive Data Buffer
										// HOST Data Structure
	RECVCOMM	RecvComm;				// ���Ű��뵥����

	BYTE		HostReserved[40];		// HOST����

	BYTE		BranchName[40];			// ������
	BYTE		BranchTelNum[20];		// ������ȭ��ȣ

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

	//���ڻ�ǰ�� DATA
	RXETICKETDATA m_RXETICKETDATA;

	//������ DATA  #N0174
	RXEASYONEDATA m_RXEASYONEDATA;

	CArray<DataField *, DataField *> m_arrTXDataStructure;
	CArray<DataField *, DataField *> m_arrRXDataStructure;

	DataField pArrDataFieldArr[100];
	int		  nSendDataIndex;

	//#N0266
	RXHIPASS		m_RXHIPASS;
/////////////////////////////////////////////////////////////////////////////
//	������ó������
/////////////////////////////////////////////////////////////////////////////
	BYTE	FinishFlag;					// ���籸��
	int		ChoiceTran;					// ��ȸ�ļ��õȰŷ�
	int		TranResultNGCode;			// Tran�ŷ����NG�ڵ�
	int		PbNextTranFlag;				// ��������ŷ��뺸Flag
	int		PbNextPageFlag;				// ��������������뺸Flag
	int		InqNextTranFlag;			// �����ȸ�����ŷ��뺸Flag
	int		RetractFlag;				// ��üȸ������Flag
	int		KCashHostICFlag;			// K-CASH Host IC Flag
	int		UpdateKeyParam;				// �Ķ���Ͱ��ſ���
	int		DepNextTranFlag;			// ���Ӱŷ�_�Ա�_Flag
	int		DepNextTranCount;			// ���Ӱŷ�_Ƚ��


	int		SeedErrorCode;				// �Ϻ�ȭ����
										// 1 : �ڵ��� ��ȣȭ NG
										// 2 : �ڵ��� ��ȣȭ NG
										// 3 : BP 0x31, 0x32 NG
										// 4 : BP 0x31, 0x32 NG
	int		ContinueTranCount;			// ���Ӱŷ�ī��Ʈ(���,�۱�,�Ա�)
	int		PbPrintFlag;				// ������忩��
	int		PbPrintAfterMsgFlag;		// ������忩��

	int		InqNextTranCount;			// ��ȸȽ��
	int		GhostAccountFlag;			// ������¿���


//�츮�������� 2009.07.14 Gi.Jin
	BYTE m_247MK_TABLE[320+1];
	BYTE m_247W1_KEY[46+1]; /* Client�� Ű���� */	
	int  m_247Wlen;
//��ȭ����
	BYTE m_269MK_TABLE[320+1];
	BYTE m_269W1_KEY[46+1]; /* Client�� Ű���� */	
	int  m_269Wlen;
//��������
	BYTE m_279MK_TABLE[320+1];
	BYTE m_279W1_KEY[46+1]; /* Client�� Ű���� */	
	int  m_279Wlen;

//�Ｚ���� 2009.08.28 Gi.Jin
    char m_szTranBuffer[4000 + 1];
	// Operations EMV Lib
	CString		EMVHostResponse;		// EMV HOST ����ó��	V09-33-00-#01 ICCM �������
    int				EmvICStatus;				// ����IC ž�� App			// V01.00.11_0412
    int				ICnIcApp;				// ����IC ž�� App			// HNB10501
	CString		m_strEMVHostResponse;	

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	�ۼ����Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_SendHost();									// �۽�
	int		fnAPP_SendHostCancel();								// �۽�(������/����Ա�)
	int		fnAPP_SendHostTranComplete();						// �Ϸ�
	int		fnAPP_SendRecvHostSnod(int nTranCodeID = FALSE);							// SNOD ó��
	int		fnAPP_SendMakeManageSend(int RealFlg);				// �۽������ۼ�(������ Send)
	int		fnAPP_SendMake(int RealFlg = TRUE);					// �۽������ۼ� RealFlg = 0:�Ϲ�, 1:����, 2:���
	int		fnAPP_SendHostSet(int RealFlg);						// �ŷ����ڷἳ��
	int		fnAPP_SendRecvHostCancel_PowerOff();

/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_RecvHost();									// ����
	int		fnAPP_RecvAnal();									// �����ؼ�

	int		fnAPP_RecvPSPGet(void* RecvArea, PSPRecv* DataArea, int Length);
																// DATA��/������Ʈ(�ŷ�)
	int		fnAPP_RecvPSPMove(BYTE Data, PSPRecv* pDataArea, int Length);
																// ������Move

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
																// DATA��/���������Ʈ(�ŷ�)
	int		fnAPP_RecvPPPMove(BYTE Data, PPPRecv* pDataArea, int Length);
	int		fnAPP_HANA_RecvPPPMove(BYTE Data, PPPRecv* pDataArea, int Length);   //#N0214
	int		fnAPP_NONGHYUP_RecvPPPMove(BYTE Data, PPPRecvNon* pDataArea, int Length);
																// ���念��Move

	int		fnAPP_RecvPMEGet(void* RecvArea, PMERecv* DataArea, int Length);
																// DATA��/�޼�������ǥ��(�ŷ�/����)
	int		fnAPP_RecvPMESubGet(void* RecvArea, PMERecv* DataArea, int Cnt, int Length);
																// �޼�������ǥ��Sub
	int		fnAPP_RecvPMEMove(BYTE Data, PMERecv* pDataArea, int Length);
																// �޼�����������Move

	int		fnAPP_RecvPMDGet(void* RecvArea, PMDRecv* DataArea, int Length);
																// DATA��/�޼���ǥ��(�ŷ�)
	int		fnAPP_RecvPMDMove(BYTE Data, PMDRecv* pDataArea, int Length);
																// ȭ�鿵��Move
	int		fnAPP_RecvPCWGet(void* RecvArea, PCWRecv* DataArea, int Length);
																// ī��M/S���
	int		fnAPP_RecvPCWMove(BYTE Data, PCWRecv* pDataArea, int Length);
																// ī�念��Move

/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_RecvDesKeyProc(PMERecv* DataBuff);			// TMK/TPK Recv Proc
	int		fnAPP_RecvDesKeyCheck(PMERecv* DataBuff);			// TMK/TPK Recv DES KEY NULL/ZERO Check V02.06.01

/////////////////////////////////////////////////////////////////////////////
//	�ۼ����Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_SendHostDataSet(int RealFlg);					// �ŷ����ڷἳ��


	int		fnAPP_SendMakeDataSetErrorCode();					// �����ڵ�����
//	int		fnAPP_SendMakeDataSetBankNum();						// �����ڵ�����
	int		fnAPP_SendMakeDataSetCheckCount();					// ��ݼ�ǥ�ż�����
	int		fnAPP_SendMakeDataSetPassword();					// ��й�ȣ����

	// ������ ����
	int		fnAPP_SendMakeDataSetDepCheckInfo();				// �Աݼ�ǥ��������
	int		fnAPP_SendMakeDataSetTransferInfo();				// �۱���������
//	int		fnAPP_SendMakeDataSetAutoInfo();					// �ڵ���ü��������
//	int		fnAPP_SendMakeDataSetCmsInfo();						// CMS��������
//	int		fnAPP_SendMakeDataSetLoanInfo();					// ������������
//	int		fnAPP_SendMakeDataSetJiroInFo();					// ������������

//	int		fnAPP_SendMakeICInformation();						// ����IC��������
	int		fnAPP_SendMakeIRDInformation();						// ��������������
	int		fnAPP_SendMakeEMVInformation();						// EMV��������

//	int		fnAPP_SendMakeTaDepositInfo();						// Ÿ���Ա���������
	int		fnAPP_SendMakeDataSetNextInqInfo();					// ������ȸ��������


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
	int    fnAPP_TranEnc001_001(int nTrCode = FALSE);		//"�ѱ�����    "
	int    fnAPP_TranEnc002_002(int nTrCode = FALSE);		//"�������    "
	int    fnAPP_TranEnc003_003(int nTrCode = FALSE);		//"�������    "
	int    fnAPP_TranEnc005_005(int nTrCode = FALSE);		//"��ȯ����    "
	int    fnAPP_TranEnc004_006(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc007_007(int nTrCode = FALSE);		//"�����߾�ȸ  "
	int    fnAPP_TranEnc008_008(int nTrCode = FALSE);		//"����������  "
	int    fnAPP_TranEnc011_011(int nTrCode = FALSE);		//"�����߾�ȸ  "
	int    fnAPP_TranEnc012_012(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc020_020(int nTrCode = FALSE);		//"�츮����    "
	int    fnAPP_TranEnc023_023(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc053_053(int nTrCode = FALSE);		//"��Ƽ����    "
	int    fnAPP_TranEnc031_031(int nTrCode = FALSE);		//"�뱸����    "
	int    fnAPP_TranEnc032_032(int nTrCode = FALSE);		//"�λ�����    "
	int    fnAPP_TranEnc034_034(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc035_035(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc037_037(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc039_039(int nTrCode = FALSE);		//"�泲����    "
	int    fnAPP_TranEnc045_045(int nTrCode = FALSE);		//"�������ݰ�  "
	int    fnAPP_TranEnc048_048(int nTrCode = FALSE);		//"�����߾�ȸ  "
	int    fnAPP_TranEnc050_050(int nTrCode = FALSE);		//"��ȣ��������"
	int    fnAPP_TranEnc054_054(int nTrCode = FALSE);		//"HSBC����    "
	int    fnAPP_TranEnc055_055(int nTrCode = FALSE);		//"����ġ����  "
	int    fnAPP_TranEnc056_056(int nTrCode = FALSE);		//"ABN����     "
	int    fnAPP_TranEnc071_071(int nTrCode = FALSE);		//"��ü��      "
	int    fnAPP_TranEnc025_081(int nTrCode = FALSE);		//"�ϳ�����    "
	int    fnAPP_TranEnc021_088(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc105_105(int nTrCode = FALSE);		//"KEB�ϳ�ī�� "
	int    fnAPP_TranEnc105_115(int nTrCode = FALSE);		//"KEB�ؿ�ī�� "
	int    fnAPP_TranEnc106_106(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranEnc107_107(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranEnc111_111(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc120_120(int nTrCode = FALSE);		//"�츮ī��    "
	int    fnAPP_TranEnc121_121(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranEnc126_192(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranEnc127_127(int nTrCode = FALSE);		//"��Ƽī��    "
	int    fnAPP_TranEnc134_034(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranEnc135_135(int nTrCode = FALSE);		//"���ֺ���    "
	int    fnAPP_TranEnc137_037(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranEnc181_105(int nTrCode = FALSE);		//"KEB�ϳ�ī�� "
	int    fnAPP_TranEnc184_184(int nTrCode = FALSE);		//"�Ｚī��    "
	int    fnAPP_TranEnc192_192(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranEnc193_193(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranEnc194_194(int nTrCode = FALSE);		//"BCī��      "
	int    fnAPP_TranEnc195_195(int nTrCode = FALSE);		//"�Ե�ī��    "
	int    fnAPP_TranEnc209_209(int nTrCode = FALSE);		//"����Ÿ����	 "
	int    fnAPP_TranEnc218_218(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc230_230(int nTrCode = FALSE);		//"�̷�������  "
	int    fnAPP_TranEnc238_238(int nTrCode = FALSE);		//"�������    "
	int    fnAPP_TranEnc240_240(int nTrCode = FALSE);		//"�Ｚ����    "
	int    fnAPP_TranEnc243_243(int nTrCode = FALSE);		//"�ѱ�����    "
	int    fnAPP_TranEnc247_247(int nTrCode = FALSE);		//"�츮����    "
	int    fnAPP_TranEnc261_261(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc262_262(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc263_263(int nTrCode = FALSE);		//"HMC����     "
	int    fnAPP_TranEnc264_264(int nTrCode = FALSE);		//"Ű������    "
	int    fnAPP_TranEnc265_265(int nTrCode = FALSE);		//"��Ʈ���̵�  "
	int    fnAPP_TranEnc266_266(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc267_267(int nTrCode = FALSE);		//"�������    "
	int    fnAPP_TranEnc268_268(int nTrCode = FALSE);		//"�ַθ�����  "
	int    fnAPP_TranEnc269_269(int nTrCode = FALSE);		//"��ȭ����    "
	int    fnAPP_TranEnc270_270(int nTrCode = FALSE);		//"�ϳ�����    "
	int    fnAPP_TranEnc278_278(int nTrCode = FALSE);		//"���ѱ���    "
	int    fnAPP_TranEnc279_279(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc280_280(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranEnc287_287(int nTrCode = FALSE);		//"�޸�������  "
	int    fnAPP_TranEnc289_289(int nTrCode = FALSE);		//"NH����      "
	int    fnAPP_TranEnc290_290(int nTrCode = FALSE);		//"�α�����    "
	int    fnAPP_TranEnc291_291(int nTrCode = FALSE);		//"�ſ�����    "
	int    fnAPP_TranEnc292_292(int nTrCode = FALSE);		//"LIG����	   "
	int    fnAPP_TranEnc294_294(int nTrCode = FALSE);		//"�ؿ�ī�� BC "
	int    fnAPP_TranEnc294_294_BC_000_ENC_FOREGIN(int nTrCode = FALSE);		//"�ؿܺ�    "
	int    fnAPP_TranEnc294_294_BC_JCB_ENC_FOREGIN(int nTrCode = FALSE);		//"�ؿܺ�    "
	int    fnAPP_TranEnc384_364(int nTrCode = FALSE);		//"�Ｚ����    "
	int    fnAPP_TranEnc484_484(int nTrCode = FALSE);		//"CJ�ý���    "
	int    fnAPP_TranEnc601_601(int nTrCode = FALSE);		//"�߷���      "
	int    fnAPP_TranEnc701_701(int nTrCode = FALSE);		//"KTF         "
	int    fnAPP_TranEnc702_702(int nTrCode = FALSE);		//"LGT         "
	int    fnAPP_TranEnc703_703(int nTrCode = FALSE);		//"SKT         "


	int    fnAPP_TranDec000_000(int nTrCode = FALSE);		//"????????????"
	int    fnAPP_TranDec001_001(int nTrCode = FALSE);		//"�ѱ�����    "
	int    fnAPP_TranDec002_002(int nTrCode = FALSE);		//"�������    "
	int    fnAPP_TranDec003_003(int nTrCode = FALSE);		//"�������    "
	int    fnAPP_TranDec005_005(int nTrCode = FALSE);		//"��ȯ����    "
	int    fnAPP_TranDec004_006(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec007_007(int nTrCode = FALSE);		//"�����߾�ȸ  "
	int    fnAPP_TranDec008_008(int nTrCode = FALSE);		//"����������  "
	int    fnAPP_TranDec011_011(int nTrCode = FALSE);		//"�����߾�ȸ  "
	int    fnAPP_TranDec012_012(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec020_020(int nTrCode = FALSE);		//"�츮����    "
	int    fnAPP_TranDec023_023(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec053_053(int nTrCode = FALSE);		//"��Ƽ����    "
	int    fnAPP_TranDec031_031(int nTrCode = FALSE);		//"�뱸����    "
	int    fnAPP_TranDec032_032(int nTrCode = FALSE);		//"�λ�����    "
	int    fnAPP_TranDec034_034(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec035_035(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec037_037(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec039_039(int nTrCode = FALSE);		//"�泲����    "
	int    fnAPP_TranDec045_045(int nTrCode = FALSE);		//"�������ݰ�  "
	int    fnAPP_TranDec048_048(int nTrCode = FALSE);		//"�����߾�ȸ  "
	int    fnAPP_TranDec050_050(int nTrCode = FALSE);		//"��ȣ��������"
	int    fnAPP_TranDec054_054(int nTrCode = FALSE);		//"HSBC����    "
	int    fnAPP_TranDec055_055(int nTrCode = FALSE);		//"����ġ����  "
	int    fnAPP_TranDec056_056(int nTrCode = FALSE);		//"ABN����     "
	int    fnAPP_TranDec071_071(int nTrCode = FALSE);		//"��ü��      "
	int    fnAPP_TranDec025_081(int nTrCode = FALSE);		//"�ϳ�����    "
	int    fnAPP_TranDec021_088(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec105_105(int nTrCode = FALSE);		//"KEB�ϳ�ī�� "
	int    fnAPP_TranDec106_106(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranDec107_107(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranDec111_111(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec120_120(int nTrCode = FALSE);		//"�츮ī��    "
	int    fnAPP_TranDec121_121(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranDec126_192(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranDec127_127(int nTrCode = FALSE);		//"��Ƽī��    "
	int    fnAPP_TranDec134_034(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranDec135_135(int nTrCode = FALSE);		//"���ֺ���    "
	int    fnAPP_TranDec137_037(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranDec181_105(int nTrCode = FALSE);		//"KEB�ϳ�ī�� "
	int    fnAPP_TranDec184_184(int nTrCode = FALSE);		//"�Ｚī��    "
	int    fnAPP_TranDec192_192(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranDec193_193(int nTrCode = FALSE);		//"����ī��    "
	int    fnAPP_TranDec194_194(int nTrCode = FALSE);		//"BCī��      "
	int    fnAPP_TranDec195_195(int nTrCode = FALSE);		//"�Ե�ī��    "
	int    fnAPP_TranDec209_209(int nTrCode = FALSE);		//"����Ÿ����	 "
	int    fnAPP_TranDec218_218(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec230_230(int nTrCode = FALSE);		//"�̷�������  "
	int    fnAPP_TranDec238_238(int nTrCode = FALSE);		//"�������    "
	int    fnAPP_TranDec240_240(int nTrCode = FALSE);		//"�Ｚ����    "
	int    fnAPP_TranDec243_243(int nTrCode = FALSE);		//"�ѱ�����    "
	int    fnAPP_TranDec247_247(int nTrCode = FALSE);		//"�츮����    "
	int    fnAPP_TranDec261_261(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec262_262(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec263_263(int nTrCode = FALSE);		//"HMC����     "
	int    fnAPP_TranDec264_264(int nTrCode = FALSE);		//"Ű������    "
	int    fnAPP_TranDec265_265(int nTrCode = FALSE);		//"��Ʈ���̵�  "
	int    fnAPP_TranDec266_266(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec267_267(int nTrCode = FALSE);		//"�������    "
	int    fnAPP_TranDec268_268(int nTrCode = FALSE);		//"�ַθ�����  "
	int    fnAPP_TranDec269_269(int nTrCode = FALSE);		//"��ȭ����    "
	int    fnAPP_TranDec270_270(int nTrCode = FALSE);		//"�ϳ�����    "
	int    fnAPP_TranDec278_278(int nTrCode = FALSE);		//"���ѱ���    "
	int    fnAPP_TranDec279_279(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec280_280(int nTrCode = FALSE);		//"��������    "
	int    fnAPP_TranDec287_287(int nTrCode = FALSE);		//"�޸�������  "
	int    fnAPP_TranDec289_289(int nTrCode = FALSE);		//"NH����      "
	int    fnAPP_TranDec290_290(int nTrCode = FALSE);		//"�α�����    "
	int    fnAPP_TranDec291_291(int nTrCode = FALSE);		//"�ſ�����    "
	int    fnAPP_TranDec292_292(int nTrCode = FALSE);		//"LIG����	   "
	int    fnAPP_TranDec294_294(int nTrCode = FALSE);		//"LIG����	   "
	int    fnAPP_TranDec384_364(int nTrCode = FALSE);		//"�Ｚ����    "
	int    fnAPP_TranDec484_484(int nTrCode = FALSE);		//"CJ�ý���    "
	int    fnAPP_TranDec601_601(int nTrCode = FALSE);		//"�߷���      "
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
	int	fn_FTX_TRANID_0006(int nTranCode, int nSec = FALSE);    //��������
	int	fn_FTX_TRANID_A010(int nTranCode, int nSec = FALSE);    //0800	0810	�ܸ��� ��������                                                                                                                                                                       
	int	fn_FTX_TRANID_A020(int nTranCode, int nSec = FALSE);    //0800	0810	�ܸ��� ��������                                                                                                                                                                       
	int	fn_FTX_TRANID_2190(int nTranCode, int nSec = FALSE, int nEncFlag = FALSE);    //0800	0810	�ܸ��� ���ΰŷ�                                                                                                                                                                       
	int	fn_FTX_TRANID_A040(int nTranCode, int nSec = FALSE);    //0800	0810	�ܸ��� ��������                                                                                                                                                                       
	int	fn_FTX_TRANID_A060(int nTranCode, int nSec = FALSE);    //0800	0810	�ܸ��� ��������                                                                                                                                                                       

	int	fn_FTX_TRANID_7210(int nTranCode, int nSec = FALSE);    //0200	0210	��������ȸ
	int	fn_FTX_TRANID_4210(int nTranCode, int nSec = FALSE);    //0200	0210	�������(����)                                                                                                                                                                            
	int	fn_FTX_TRANID_4310(int nTranCode, int nSec = FALSE);    //0200	0210	�������                                                                                                                                                                                  
	int	fn_FTX_TRANID_4320(int nTranCode, int nSec = FALSE);    //5400	5410	����������                                                                                                                                                                              
	int	fn_FTX_TRANID_4330(int nTranCode, int nSec = FALSE);    //0200	0210	�������                                                                                                                                                                                
	int	fn_FTX_TRANID_4340(int nTranCode, int nSec = FALSE);    //5400	5410	����������                                                                                                                                                                            
	int	fn_FTX_TRANID_4350(int nTranCode, int nSec = FALSE);    //0200	0210	������ü                                                                                                                                                                                
	int	fn_FTX_TRANID_4370(int nTranCode, int nSec = FALSE);    //0200	0210	������ü(����)                                                                                                                                                                                  
	int	fn_FTX_TRANID_4390(int nTranCode, int nSec = FALSE);    //0200	0210	������ü   
	int	fn_FTX_TRANID_7260(int nTranCode, int nSec = FALSE);    //0200	0210	���������ȸ   	
	int	fn_FTX_TRANID_4410(int nTranCode, int nSec = FALSE);    //0200	0210	ī���Ա�
 	int	fn_FTX_TRANID_7230(int nTranCode, int nSec = FALSE);    //0200	0210	������ȸ   
 	int	fn_FTX_TRANID_7220(int nTranCode, int nSec = FALSE);    //0200	0210	������ȸ
	int	fn_FTX_TRANID_7250(int nTranCode, int nSec = FALSE);    //0200	0210	������ü��ȸ  
	int	fn_FTX_TRANID_7200(int nTranCode, int nSec = FALSE);    //0200	0210	�Աݰ��ɿ�����ȸ   
	
	//���ݼ��� ������ ��ȸ  
	int fn_FTX_TRANID_7380(int nTranCode, int nSec = FALSE);    //0800	0810	���ݼ��� ��� ������ ��ȸ
	int fn_FTX_TRANID_7530(int nTranCode, int nSec = FALSE);    //0800	0810	�ؿ�
 	int fn_FTX_EMVSendMake_Lib01(int nTranCode, int nSec = FALSE);//EMVSendLib

// 	int	fn_FTX_TRANID_4610(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money ��������                                                                                                                                                                          
// 	int	fn_FTX_TRANID_4620(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money �����������                                                                                                                                                                      
// 	int	fn_FTX_TRANID_4611(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money ����������                                                                                                                                                                        
// 	int	fn_FTX_TRANID_4621(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money �������������                                                                                                                                                                    
// 	int	fn_FTX_TRANID_4631(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money ���ϸ�����ȸ                                                                                                                                                                      
// 	int	fn_FTX_TRANID_4630(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money ���ϸ�������                                                                                                                                                                      
// 	int	fn_FTX_TRANID_4640(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money ���ϸ������                                                                                                                                                                      
// 	int	fn_FTX_TRANID_4650(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money������ �������                                                                                                                                                                    
// 	int	fn_FTX_TRANID_4660(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money������ ����������                                                                                                                                                                
// 	int	fn_FTX_TRANID_4670(int nTranCode, int nSec = FALSE);    //0200	0210	�����н� ������û                                                                                                                                                                         
// 	int	fn_FTX_TRANID_4671(int nTranCode, int nSec = FALSE);    //0200	0210	�����н� ����Ȯ��                                                                                                                                                                         
// 	int	fn_FTX_TRANID_4680(int nTranCode, int nSec = FALSE);    //0200	0210	�����н� ��������                                                                                                                                                                         
// 	int	fn_FTX_TRANID_4681(int nTranCode, int nSec = FALSE);    //0200	0210	�����н� ����������ȸ                                                                                                                                                                     
// 	int	fn_FTX_TRANID_4682(int nTranCode, int nSec = FALSE);    //0200	0210	�����н� ��������Ȯ��                                                                                                                                                                     
                                                                                                                                                                                  
// 	int	fn_FTX_TRANID_4720(int nTranCode, int nSec = FALSE);    //0200	0210	�����Ա� 
// 	int	fn_FTX_TRANID_4730(int nTranCode, int nSec = FALSE);    //0200	0210	��������                                                                                                                                                                                  
// 	int	fn_FTX_TRANID_4810(int nTranCode, int nSec = FALSE);    //0200	0210	�����ݳ���                                                                                                                                                                                
// 	int	fn_FTX_TRANID_4811(int nTranCode, int nSec = FALSE);    //0200	0210	���漼 ������(MS)                                                                                                                                                                       
// 	int	fn_FTX_TRANID_4812(int nTranCode, int nSec = FALSE);    //0200	0210	���漼 ������(�ſ�ī��)                                                                                                                                                                 
// 	int	fn_FTX_TRANID_4813(int nTranCode, int nSec = FALSE);    //0200	0210	ICī�� �����ݳ���                                                                                                                                                                         
// 	int	fn_FTX_TRANID_4814(int nTranCode, int nSec = FALSE);    //0200	0210	���漼 ������(IC)                                                                                                                                                                       
// 	int	fn_FTX_TRANID_4820(int nTranCode, int nSec = FALSE);    //0200	0210	�ڵ��¼�/�ֹ�����                                                                                                                                                                         
// 	int	fn_FTX_TRANID_4821(int nTranCode, int nSec = FALSE);    //0200	-	��º���                                                                                                                                                                                  
// 	int	fn_FTX_TRANID_4822(int nTranCode, int nSec = FALSE);    //0200	0210	�߱����� ���                                                                                                                                                                             
// 	int	fn_FTX_TRANID_4830(int nTranCode, int nSec = FALSE);    //0200	0210	TC ����                                                                                                                                                                                   
                                                                                                                                                                                                                                                                                                                                           
// 	int	fn_FTX_TRANID_7120(int nTranCode, int nSec = FALSE);    //0200	0210	������ȸ(����)                                                                                                                                                                            
// 	int	fn_FTX_TRANID_7124(int nTranCode, int nSec = FALSE);    //0200	0210	��ü��ݰ�����ȸ(����)                                                                                                                                                                    
// 	int	fn_FTX_TRANID_7125(int nTranCode, int nSec = FALSE);    //0200	0210	������ü��ݰ�����ȸ(����)                                                                                                                                                                
// 	int	fn_FTX_TRANID_7126(int nTranCode, int nSec = FALSE);    //0200	0210	�Աݰ��ɿ�����ȸ(����) 
// 	int	fn_FTX_TRANID_7236(int nTranCode, int nSec = FALSE);    //0200	0210	�Աݰ��ɿ�����ȸ(����ü)
                                                                                                                                                                                  
// 	int	fn_FTX_TRANID_7221(int nTranCode, int nSec = FALSE);    //0200	0210	�ؿܿ����ܾ���ȸ                                                                                                                                                                          
// 	int	fn_FTX_TRANID_7222(int nTranCode, int nSec = FALSE);    //0200	0210	�ؿܴ��°ŷ��ܾ���ȸ                                                                                                                                                                      
// 	int	fn_FTX_TRANID_7224(int nTranCode, int nSec = FALSE);    //0200	0210	��ü��ݰ�����ȸ                                                                                                                                                                          
// 	int	fn_FTX_TRANID_7225(int nTranCode, int nSec = FALSE);    //0200	0210	������ü��ݰ�����ȸ                                                                                                                                                                      
                                                                                                                                                                                                                                                                                                                                           
// 	int	fn_FTX_TRANID_7228(int nTranCode, int nSec = FALSE);    //0200	0210	����������ȸ                                                                                                                                                                              
                                                                                                                                                                             
// 	int	fn_FTX_TRANID_7231(int nTranCode, int nSec = FALSE);    //0200	0210	�ؿ����ݼ�����ȸ                                                                                                                                                                        
// 	int	fn_FTX_TRANID_7240(int nTranCode, int nSec = FALSE);    //0200	0210	�����������ȸ                                                                                                                                                                          
// 	int	fn_FTX_TRANID_7241(int nTranCode, int nSec = FALSE);    //0200	0210	ĳ��Ż���ܾ���ȸ                                                                                                                                                                          
                                                                                                                                                                          
                                                                                                                                                                           
// 	int	fn_FTX_TRANID_7261(int nTranCode, int nSec = FALSE);    //0200	0210	���������ȸ(����)  
// 	int	fn_FTX_TRANID_7276(int nTranCode, int nSec = FALSE);    //0200	0210	���������ȸ(����)
// 	int	fn_FTX_TRANID_7262(int nTranCode, int nSec = FALSE);    //0200	0210	���¼�����ȸ(e-���� ������ȸ)                                                                                                                                                                                                                                                                                                                            
// 	int	fn_FTX_TRANID_7266(int nTranCode, int nSec = FALSE);    //0200	0210	�Ǹ�Ȯ��                                                                                                                                                                                  
// 	int	fn_FTX_TRANID_7267(int nTranCode, int nSec = FALSE);    //0200	0210	�����ݾ���ȸ                                                                                                                                                                              
// 	int	fn_FTX_TRANID_7268(int nTranCode, int nSec = FALSE);    //0200	0210	������ȣ�ޱ�                                                                                                                                                                              
// 	int	fn_FTX_TRANID_7269(int nTranCode, int nSec = FALSE);    //0200	0210	���������ȸ(����ȣ/�ޱ�)                                                                                                                                                                 
// 	int	fn_FTX_TRANID_7280(int nTranCode, int nSec = FALSE);    //0200	0210	�Ե� �м��� ��ȸ                                                                                                                                                                        
// 	int	fn_FTX_TRANID_7281(int nTranCode, int nSec = FALSE);    //0200	0210	�Ե� �м��� ������ȸ                                                                                                                                                                    
// 	int	fn_FTX_TRANID_7290(int nTranCode, int nSec = FALSE);    //0200	0210	��ȯ �ؿܼ۱� ���� ��ȸ                                                                                                                                                                   
// 	int	fn_FTX_TRANID_7300(int nTranCode, int nSec = FALSE);    //0200	0210	��ȯ �ؿܼ۱� ��� ��ȸ                                                                                                                                                                   
// 	int	fn_FTX_TRANID_7310(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ�������ȸ                                                                                                                                                                              
// 	int	fn_FTX_TRANID_7311(int nTranCode, int nSec = FALSE);    //0200	0210	����������ȸ                                                                                                                                                                              
// 	int	fn_FTX_TRANID_7312(int nTranCode, int nSec = FALSE);    //0200	0210	CB��ü������ȸ                                                                                                                                                                            
// 	int	fn_FTX_TRANID_7313(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ�ī�� ����(�ѽ���)                                                                                                                                                                     
// 	int	fn_FTX_TRANID_7314(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ�ī�� ����                                                                                                                                                                             
// 	int	fn_FTX_TRANID_7315(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ����� ���ھ� ��ȸ                                                                                                                                                                      
// 	int	fn_FTX_TRANID_7316(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ�ī�� ����(����ĳ��Ż)                                                                                                                                                                 
// 	int	fn_FTX_TRANID_7317(int nTranCode, int nSec = FALSE);    //0200	0210	���Ⱑ�ɱݾ���ȸ - ����                                                                                                                                                                   
// 	int	fn_FTX_TRANID_7318(int nTranCode, int nSec = FALSE);    //0200	0210	���Ⱑ�ɱݾ���ȸ - ����                                                                                                                                                                                                                                                                                                                                                 
// 	int	fn_FTX_TRANID_7320(int nTranCode, int nSec = FALSE);    //0200	0210	�����ݳ�����ȸ                                                                                                                                                                            
// 	int	fn_FTX_TRANID_7321(int nTranCode, int nSec = FALSE);    //0200	0210	�����ݳ���ó�������ȸ                                                                                                                                                                    
// 	int	fn_FTX_TRANID_7322(int nTranCode, int nSec = FALSE);    //0200	0210	���漼��������ȸ(MS)                                                                                                                                                                    
// 	int	fn_FTX_TRANID_7323(int nTranCode, int nSec = FALSE);    //0200	0210	ICī�� �����ݳ�����ȸ                                                                                                                                                                     
// 	int	fn_FTX_TRANID_7324(int nTranCode, int nSec = FALSE);    //0200	0210	ICī�� �����ݳ���ó�������ȸ                                                                                                                                                             
// 	int	fn_FTX_TRANID_7325(int nTranCode, int nSec = FALSE);    //0200	0210	���漼��������ȸ(IC)                                                                                                                                                                    
// 	int	fn_FTX_TRANID_7326(int nTranCode, int nSec = FALSE);    //0200	0210	���漼�����ΰ����ȸ(MS)                                                                                                                                                                
// 	int	fn_FTX_TRANID_7327(int nTranCode, int nSec = FALSE);    //0200	0210	���漼�����ΰ����ȸ(IC)                                                                                                                                                                
// 	int	fn_FTX_TRANID_7328(int nTranCode, int nSec = FALSE);    //0200	0210	���漼��������ȸ(�ſ�ī��)                                                                                                                                                              
// 	int	fn_FTX_TRANID_7329(int nTranCode, int nSec = FALSE);    //0200	0210	���漼�����ΰ����ȸ(�ſ�ī��)                                                                                                                                                          
// 	int	fn_FTX_TRANID_7330(int nTranCode, int nSec = FALSE);    //0200	0210	����������ȸ                                                                                                                                                                              
                                                                                                                                                                       
                                                                                                                                                                        
                                                                                                             

// 	int fn_FTX_TRANID_4717(int nTranCode, int nSec = FALSE);    //0800	0810	�����н� �Ա� 
// 	int fn_FTX_TRANID_7229(int nTranCode, int nSec = FALSE);    //0800	0810	�����ܾ� ��ȸ 
// 	int fn_FTX_EMVSendMake_Lib01(int nTranCode, int nSec = FALSE);//EMVSendLib
    /* Recv */         
	int	fn_FRX_TRANID_A010(int nTranCode, int nSec = FALSE);    //0800	0810	�ܸ��� ��������                                                                                                                                                                       
	int	fn_FRX_TRANID_A020(int nTranCode, int nSec = FALSE);    //0800	0810	�ܸ��� ��������                                                                                                                                                                       
	int	fn_FRX_TRANID_2190(int nTranCode, int nSec = FALSE, int nEncFlag = FALSE);    //0800	0810	�ܸ��� ���ΰŷ�                                                                                                                                                                       
	int	fn_FRX_TRANID_A060(int nTranCode, int nSec = FALSE);    //0800	0810	�ܸ��� ��������                                                                               	
	int	fn_FRX_TRANID_4210(int nTranCode, int nSec = FALSE);    //0200	0210	�������(����)                                                                                                                                                                            
//	int	fn_FRX_TRANID_4220(int nTranCode, int nSec = FALSE);    //5400	5410	������(����)
	int	fn_FRX_TRANID_4310(int nTranCode, int nSec = FALSE);    //0200	0210	�������                                                                                                                                                                                  
	int	fn_FRX_TRANID_4320(int nTranCode, int nSec = FALSE);    //5400	5410	����������                                                                                                                                                                              
	int	fn_FRX_TRANID_4330(int nTranCode, int nSec = FALSE);    //0200	0210	�������                                                                                                                                                                                
	int	fn_FRX_TRANID_4340(int nTranCode, int nSec = FALSE);    //5400	5410	����������                                                                                                                                                                            
	int	fn_FRX_TRANID_4350(int nTranCode, int nSec = FALSE);    //0200	0210	������ü                                                                                                                                                                                
	int	fn_FRX_TRANID_4370(int nTranCode, int nSec = FALSE);    //0200	0210	������ü (����)                                                                                                                                                                                 
	int	fn_FRX_TRANID_4390(int nTranCode, int nSec = FALSE);    //0200	0210	������ü  
	int	fn_FRX_TRANID_4410(int nTranCode, int nSec = FALSE);    //0200	0210	ī���Ա�    
	int fn_FRX_TRANID_7380(int nTranCode, int nSec = FALSE);    //0800	0810	���ݼ��� ��� ������ ��ȸ
	int fn_FRX_TRANID_7530(int nTranCode, int nSec = FALSE);    //0800	0810	���ݼ��� ��ü ������ ��ȸ
 	int	fn_FRX_TRANID_7210(int nTranCode, int nSec = FALSE);    //0200	0210	��������ȸ     
 	int	fn_FRX_TRANID_7230(int nTranCode, int nSec = FALSE);    //0200	0210	������ȸ
 	int	fn_FRX_TRANID_7220(int nTranCode, int nSec = FALSE);    //0200	0210	������ȸ  
    int	fn_FRX_TRANID_7250(int nTranCode, int nSec = FALSE);    //0200	0210	������ü��ȸ  
	int fn_FRX_TRANID_7260(int nTranCode, int nSec = FALSE);    //0200	0210	���������ȸ     
	int	fn_FRX_TRANID_7200(int nTranCode, int nSec = FALSE);    //0200	0210	�Աݰ��ɿ�����ȸ  	
	//////////////////////////////////////////////////////////////////////////////////////////////
	int fn_FRX_EMVRecv_Lib(int nTranCode, int nSec = FALSE);    //EmvLib
	
// 	int	fn_FRX_TRANID_4391(int nTranCode, int nSec = FALSE);    //0200	0210	������ü(�´�����ǰ�� ��ü�Ǹ�)                                                                                                                                                           
// 	int	fn_FRX_TRANID_4392(int nTranCode, int nSec = FALSE);    //0200	0210	e-���� ������ü                                                                                                                                                                           
// 	int	fn_FRX_TRANID_4393(int nTranCode, int nSec = FALSE);    //0200	0210	����������ICī�� ������ü                                                                                                                                                                 
// 	int	fn_FRX_TRANID_4394(int nTranCode, int nSec = FALSE);    //0200	0210	ICī�� ������ü(�´�����ǰ�� ��ü�Ǹ�)                                                                                                                                                    
// 	int	fn_FRX_TRANID_4395(int nTranCode, int nSec = FALSE);    //0200	0210	e-���� ������ü                                                                                                                                                                           
// 	int	fn_FRX_TRANID_4399(int nTranCode, int nSec = FALSE);    //0200	0210	������ü(����ȣ/�ޱ�)                                                                                                                                                                     
// 	int	fn_FRX_TRANID_4520(int nTranCode, int nSec = FALSE);    //0200	0210	���� �м��� ����                                                                                                                                                                        
// 	int	fn_FRX_TRANID_4530(int nTranCode, int nSec = FALSE);    //5400	5410	���� �м��� �������                                                                                                                                                                    
// 	int	fn_FRX_TRANID_4540(int nTranCode, int nSec = FALSE);    //0200	0210	�Ե� �м��� ����                                                                                                                                                                        
// 	int	fn_FRX_TRANID_4550(int nTranCode, int nSec = FALSE);    //5400	5410	�Ե� �м��� �������                                                                                                                                                                    
// 	int	fn_FRX_TRANID_4560(int nTranCode, int nSec = FALSE);    //0200	0210	��ȯ �ؿܼ۱�                                                                                                                                                                             
// 	int	fn_FRX_TRANID_4570(int nTranCode, int nSec = FALSE);    //0200	0210	����ī��� ����                                                                                                                                                                           
// 	int	fn_FRX_TRANID_4571(int nTranCode, int nSec = FALSE);    //0200	0210	����ī��� ��ü                                                                                                                                                                           
// 	int	fn_FRX_TRANID_4572(int nTranCode, int nSec = FALSE);    //0200	0210	����ī��� ����                                                                                                                                                                           
// 	int	fn_FRX_TRANID_4580(int nTranCode, int nSec = FALSE);    //0200	0210	����ī��� �������                                                                                                                                                                       
// 	int	fn_FRX_TRANID_4610(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money ��������                                                                                                                                                                          
// 	int	fn_FRX_TRANID_4620(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money �����������                                                                                                                                                                      
// 	int	fn_FRX_TRANID_4611(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money ����������                                                                                                                                                                        
// 	int	fn_FRX_TRANID_4621(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money �������������                                                                                                                                                                    
// 	int	fn_FRX_TRANID_4631(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money ���ϸ�����ȸ                                                                                                                                                                      
// 	int	fn_FRX_TRANID_4630(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money ���ϸ�������                                                                                                                                                                      
// 	int	fn_FRX_TRANID_4640(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money ���ϸ������                                                                                                                                                                      
// 	int	fn_FRX_TRANID_4650(int nTranCode, int nSec = FALSE);    //0200	0210	T-Money������ �������                                                                                                                                                                    
// 	int	fn_FRX_TRANID_4660(int nTranCode, int nSec = FALSE);    //5400	5410	T-Money������ ����������                                                                                                                                                                
// 	int	fn_FRX_TRANID_4670(int nTranCode, int nSec = FALSE);    //0200	0210	�����н� ������û                                                                                                                                                                         
// 	int	fn_FRX_TRANID_4671(int nTranCode, int nSec = FALSE);    //0200	0210	�����н� ����Ȯ��                                                                                                                                                                         
// 	int	fn_FRX_TRANID_4680(int nTranCode, int nSec = FALSE);    //0200	0210	�����н� ��������                                                                                                                                                                         
// 	int	fn_FRX_TRANID_4681(int nTranCode, int nSec = FALSE);    //0200	0210	�����н� ����������ȸ                                                                                                                                                                     
// 	int	fn_FRX_TRANID_4682(int nTranCode, int nSec = FALSE);    //0200	0210	�����н� ��������Ȯ��                                                                                                                                                                     
// 
// 	
// 	int	fn_FRX_TRANID_4711(int nTranCode, int nSec = FALSE);    //0200	0210	����ü�Ա�                                                                                                                                                                                
// 	int	fn_FRX_TRANID_4712(int nTranCode, int nSec = FALSE);    //0200	0210	�̸�Ʈ���� ������Ա�                                                                                                                                                                     
// 	int	fn_FRX_TRANID_4713(int nTranCode, int nSec = FALSE);    //0200	0210	ICī���Ա�                                                                                                                                                                                
// 	int	fn_FRX_TRANID_4719(int nTranCode, int nSec = FALSE);    //5400	5410	�Ա����                                                                                                                                                                                  
// 	int	fn_FRX_TRANID_4720(int nTranCode, int nSec = FALSE);    //0200	0210	�����Ա� 
// 	int	fn_FRX_TRANID_4716(int nTranCode, int nSec = FALSE);    //0200	0210	����ü�Ա� 
// 	int	fn_FRX_TRANID_4730(int nTranCode, int nSec = FALSE);    //0200	0210	��������                                                                                                                                                                                  
// 	int	fn_FRX_TRANID_4810(int nTranCode, int nSec = FALSE);    //0200	0210	�����ݳ���                                                                                                                                                                                
// 	int	fn_FRX_TRANID_4811(int nTranCode, int nSec = FALSE);    //0200	0210	���漼 ������(MS)                                                                                                                                                                       
// 	int	fn_FRX_TRANID_4812(int nTranCode, int nSec = FALSE);    //0200	0210	���漼 ������(�ſ�ī��)                                                                                                                                                                 
// 	int	fn_FRX_TRANID_4813(int nTranCode, int nSec = FALSE);    //0200	0210	ICī�� �����ݳ���                                                                                                                                                                         
// 	int	fn_FRX_TRANID_4814(int nTranCode, int nSec = FALSE);    //0200	0210	���漼 ������(IC)                                                                                                                                                                       
// 	int	fn_FRX_TRANID_4820(int nTranCode, int nSec = FALSE);    //0200	0210	�ڵ��¼�/�ֹ�����                                                                                                                                                                         
// 	int	fn_FRX_TRANID_4821(int nTranCode, int nSec = FALSE);    //0200	-	��º���                                                                                                                                                                                  
// 	int	fn_FRX_TRANID_4822(int nTranCode, int nSec = FALSE);    //0200	0210	�߱����� ���                                                                                                                                                                             
// 	int	fn_FRX_TRANID_4830(int nTranCode, int nSec = FALSE);    //0200	0210	TC ����                                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4840(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ�ī�� ����(���ΰ���)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4850(int nTranCode, int nSec = FALSE);    //5400	5410	�ſ�ī�� ���(���ΰ���)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4841(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ�ī�� ����(��������)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4851(int nTranCode, int nSec = FALSE);    //5400	5410	�ſ�ī�� ���(��������)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4842(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ�ī�� ����(ȿ��Ƽ��)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4852(int nTranCode, int nSec = FALSE);    //5400	5410	�ſ�ī�� ���(ȿ��Ƽ��)                                                                                                                                                                   
// 	int	fn_FRX_TRANID_4860(int nTranCode, int nSec = FALSE);    //0200	0210	��ǰ���Ǹ������뺸                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7000(int nTranCode, int nSec = FALSE);    //0200	0210	�ٰ�����ȸ                                                                                                                                                                                
// 	int	fn_FRX_TRANID_7004(int nTranCode, int nSec = FALSE);    //0200	0210	�ٰ�����ȸ(�Ա�)                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7005(int nTranCode, int nSec = FALSE);    //0200	0210	�ٰ�����ȸ(����)                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7006(int nTranCode, int nSec = FALSE);    //0200	0210	�ٰ�����ȸ(��ü)                                                                                                                                                                          
                                                                                                                                                                           
// 	int	fn_FRX_TRANID_7101(int nTranCode, int nSec = FALSE);    //0200	0210	��������ȸ(����)
// 	int	fn_FRX_TRANID_7103(int nTranCode, int nSec = FALSE);    //0200	0210	ICī�� ��������ȸ
// 	int	fn_FRX_TRANID_7104(int nTranCode, int nSec = FALSE);    //0200	0210	�ؿ���޼�������ȸ
// 	int	fn_FRX_TRANID_7105(int nTranCode, int nSec = FALSE);    //0200	0210	�ؿ�BC��޼�������ȸ 
// 	int	fn_FRX_TRANID_7106(int nTranCode, int nSec = FALSE);    //0200	0210	��������ȸ(����ü)
// 	int	fn_FRX_TRANID_7120(int nTranCode, int nSec = FALSE);    //0200	0210	������ȸ(����)                                                                                                                                                                            
// 	int	fn_FRX_TRANID_7124(int nTranCode, int nSec = FALSE);    //0200	0210	��ü��ݰ�����ȸ(����)                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7125(int nTranCode, int nSec = FALSE);    //0200	0210	������ü��ݰ�����ȸ(����)                                                                                                                                                                
// 	int	fn_FRX_TRANID_7126(int nTranCode, int nSec = FALSE);    //0200	0210	�Աݰ��ɿ�����ȸ(����) 
// 	int	fn_FRX_TRANID_7236(int nTranCode, int nSec = FALSE);    //0200	0210	�Աݰ��ɿ�����ȸ(����ü)
                                                                                                                                                                            
// 	int	fn_FRX_TRANID_7221(int nTranCode, int nSec = FALSE);    //0200	0210	�ؿܿ����ܾ���ȸ                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7222(int nTranCode, int nSec = FALSE);    //0200	0210	�ؿܴ��°ŷ��ܾ���ȸ                                                                                                                                                                      
// 	int	fn_FRX_TRANID_7224(int nTranCode, int nSec = FALSE);    //0200	0210	��ü��ݰ�����ȸ                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7225(int nTranCode, int nSec = FALSE);    //0200	0210	������ü��ݰ�����ȸ                                                                                                                                                                      
                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7228(int nTranCode, int nSec = FALSE);    //0200	0210	����������ȸ                                                                                                                                                                              
                                                                                                                                                                                
// 	int	fn_FRX_TRANID_7231(int nTranCode, int nSec = FALSE);    //0200	0210	�ؿ����ݼ�����ȸ                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7232(int nTranCode, int nSec = FALSE);    //0200	0210	�߱����ü��� �ܾ���ȸ 
// 	int	fn_FRX_TRANID_7233(int nTranCode, int nSec = FALSE);    //0200	0210	�ؿ� ���� ��ȸ ��� #N0235                                                                                                                                                                
// 	int	fn_FRX_TRANID_7235(int nTranCode, int nSec = FALSE);    //0200	0210	�ؿ� �ſ� ��ȸ ��� #N0235 
// 	int	fn_FRX_TRANID_7237(int nTranCode, int nSec = FALSE);    //0200	0210	�ؿ� ���� ��ȸ ��� #N0235 
// 	int	fn_FRX_TRANID_7240(int nTranCode, int nSec = FALSE);    //0200	0210	�����������ȸ                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7241(int nTranCode, int nSec = FALSE);    //0200	0210	ĳ��Ż���ܾ���ȸ                                                                                                                                                                          
                                                                                                                                                                           
                                                                                                                                                                       
// 	int	fn_FRX_TRANID_7261(int nTranCode, int nSec = FALSE);    //0200	0210	���������ȸ(����)                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7262(int nTranCode, int nSec = FALSE);    //0200	0210	���¼�����ȸ(e-���� ������ȸ)                                                                                                                                                             
// 	int	fn_FRX_TRANID_7265(int nTranCode, int nSec = FALSE);    //0200	0210	e-���� ������ü ���µ��                                                                                                                                                                  
// 	int	fn_FRX_TRANID_7266(int nTranCode, int nSec = FALSE);    //0200	0210	�Ǹ�Ȯ��                                                                                                                                                                                  
// 	int	fn_FRX_TRANID_7267(int nTranCode, int nSec = FALSE);    //0200	0210	�����ݾ���ȸ                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7268(int nTranCode, int nSec = FALSE);    //0200	0210	������ȣ�ޱ�                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7269(int nTranCode, int nSec = FALSE);    //0200	0210	���������ȸ(����ȣ/�ޱ�)
// 	int	fn_FRX_TRANID_7276(int nTranCode, int nSec = FALSE);    //0200	0210	���������ȸ(����ü)
// 	int	fn_FRX_TRANID_7280(int nTranCode, int nSec = FALSE);    //0200	0210	�Ե� �м��� ��ȸ                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7281(int nTranCode, int nSec = FALSE);    //0200	0210	�Ե� �м��� ������ȸ                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7290(int nTranCode, int nSec = FALSE);    //0200	0210	��ȯ �ؿܼ۱� ���� ��ȸ                                                                                                                                                                   
// 	int	fn_FRX_TRANID_7300(int nTranCode, int nSec = FALSE);    //0200	0210	��ȯ �ؿܼ۱� ��� ��ȸ                                                                                                                                                                   
// 	int	fn_FRX_TRANID_7310(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ�������ȸ                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7311(int nTranCode, int nSec = FALSE);    //0200	0210	����������ȸ                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7312(int nTranCode, int nSec = FALSE);    //0200	0210	CB��ü������ȸ                                                                                                                                                                            
// 	int	fn_FRX_TRANID_7313(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ�ī�� ����(�ѽ���)                                                                                                                                                                     
// 	int	fn_FRX_TRANID_7314(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ�ī�� ����                                                                                                                                                                             
// 	int	fn_FRX_TRANID_7315(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ����� ���ھ� ��ȸ                                                                                                                                                                      
// 	int	fn_FRX_TRANID_7316(int nTranCode, int nSec = FALSE);    //0200	0210	�ſ�ī�� ����(����ĳ��Ż)                                                                                                                                                                 
// 	int	fn_FRX_TRANID_7317(int nTranCode, int nSec = FALSE);    //0200	0210	���Ⱑ�ɱݾ���ȸ - ����                                                                                                                                                                   
// 	int	fn_FRX_TRANID_7318(int nTranCode, int nSec = FALSE);    //0200	0210	���Ⱑ�ɱݾ���ȸ - ����                                                                                                                                                                   
// 	int	fn_FRX_TRANID_7319(int nTranCode, int nSec = FALSE);    //0200	0210	������ũ��                                                                                                                                                                                
// 	int	fn_FRX_TRANID_7320(int nTranCode, int nSec = FALSE);    //0200	0210	�����ݳ�����ȸ                                                                                                                                                                            
// 	int	fn_FRX_TRANID_7321(int nTranCode, int nSec = FALSE);    //0200	0210	�����ݳ���ó�������ȸ                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7322(int nTranCode, int nSec = FALSE);    //0200	0210	���漼��������ȸ(MS)                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7323(int nTranCode, int nSec = FALSE);    //0200	0210	ICī�� �����ݳ�����ȸ                                                                                                                                                                     
// 	int	fn_FRX_TRANID_7324(int nTranCode, int nSec = FALSE);    //0200	0210	ICī�� �����ݳ���ó�������ȸ                                                                                                                                                             
// 	int	fn_FRX_TRANID_7325(int nTranCode, int nSec = FALSE);    //0200	0210	���漼��������ȸ(IC)                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7326(int nTranCode, int nSec = FALSE);    //0200	0210	���漼�����ΰ����ȸ(MS)                                                                                                                                                                
// 	int	fn_FRX_TRANID_7327(int nTranCode, int nSec = FALSE);    //0200	0210	���漼�����ΰ����ȸ(IC)                                                                                                                                                                
// 	int	fn_FRX_TRANID_7328(int nTranCode, int nSec = FALSE);    //0200	0210	���漼��������ȸ(�ſ�ī��)                                                                                                                                                              
// 	int	fn_FRX_TRANID_7329(int nTranCode, int nSec = FALSE);    //0200	0210	���漼�����ΰ����ȸ(�ſ�ī��)                                                                                                                                                          
// 	int	fn_FRX_TRANID_7330(int nTranCode, int nSec = FALSE);    //0200	0210	����������ȸ                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7336(int nTranCode, int nSec = FALSE);    //0200	0210	���Ÿŵ��ܾ���ȸ                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7337(int nTranCode, int nSec = FALSE);    //0200	0210	�ֹ�ü�᳻�����                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7338(int nTranCode, int nSec = FALSE);    //0200	0210	�ܰ���ȸ���                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7339(int nTranCode, int nSec = FALSE);    //0200	0210	�ŷ��������                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7340(int nTranCode, int nSec = FALSE);    //0200	0210	TC ���� ���� ���� ��ȸ                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7341(int nTranCode, int nSec = FALSE);    //0200	0210	Ƽ������ ��ȸ                                                                                                                                                                             
// 	int	fn_FRX_TRANID_7342(int nTranCode, int nSec = FALSE);    //0200	0210	�Ǹ�ó���� ��ȸ                                                                                                                                                                           
// 	int	fn_FRX_TRANID_7343(int nTranCode, int nSec = FALSE);    //0200	0210	����/�������� ��ȸ                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7344(int nTranCode, int nSec = FALSE);    //0200	0210	�������� ��ȸ                                                                                                                                                                             
// 	int	fn_FRX_TRANID_7345(int nTranCode, int nSec = FALSE);    //0200	0210	��߽ð� ��ȸ                                                                                                                                                                             
// 	int	fn_FRX_TRANID_7350(int nTranCode, int nSec = FALSE);    //0200	0210	����Ʈ�㺸���� - ����                                                                                                                                                                     
// 	int	fn_FRX_TRANID_7351(int nTranCode, int nSec = FALSE);    //0200	0210	����Ʈ�㺸���� - ����                                                                                                                                                                     
// 	int	fn_FRX_TRANID_7352(int nTranCode, int nSec = FALSE);    //0200	0210	�����ڱݴ��� - ����                                                                                                                                                                       
// 	int	fn_FRX_TRANID_7353(int nTranCode, int nSec = FALSE);    //0200	0210	�����ڱݴ��� - ����                                                                                                                                                                       
// 	int	fn_FRX_TRANID_7354(int nTranCode, int nSec = FALSE);    //0200	0210	�޴���ȭ ����(����ĳ��Ż)                                                                                                                                                                 
// 	int	fn_FRX_TRANID_7357(int nTranCode, int nSec = FALSE);    //0200	0210	������ó ���(���ν�����)                                                                                                                                                               
// 	int	fn_FRX_TRANID_7358(int nTranCode, int nSec = FALSE);    //0200	0210	�ֹι�ȣ,��Ż籸��,�ڵ�����ȣ�Է�(�����)                                                                                                                                            
// 	int	fn_FRX_TRANID_7359(int nTranCode, int nSec = FALSE);    //0200	0210	������ȣ�Է�(�����)                                                                                                                                                                  
// 	int	fn_FRX_TRANID_7360(int nTranCode, int nSec = FALSE);    //0200	0210	�ѵ�������ȸ                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7361(int nTranCode, int nSec = FALSE);    //0200	0210	�������ȸ                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7362(int nTranCode, int nSec = FALSE);    //0200	0210	�������ε��                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7363(int nTranCode, int nSec = FALSE);    //0200	0210	�����ѵ����׵��                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7364(int nTranCode, int nSec = FALSE);    //0200	0210	�Ż�����ȸ                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7365(int nTranCode, int nSec = FALSE);    //0200	0210	�Ż������                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7366(int nTranCode, int nSec = FALSE);    //0200	0210	�ѵ�������                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7367(int nTranCode, int nSec = FALSE);    //0200	0210	�����                                                                                                                                                                                  
// 	int	fn_FRX_TRANID_7370(int nTranCode, int nSec = FALSE);    //0200	0210	���Ⱑ�ɻ�ǰ��ȸ                                                                                                                                                                          
// 	int	fn_FRX_TRANID_7371(int nTranCode, int nSec = FALSE);    //0200	0210	���޻���ȸ                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7372(int nTranCode, int nSec = FALSE);    //0200	0210	��ü����ȸ                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7373(int nTranCode, int nSec = FALSE);    //0200	0210	��������ȸ                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7420(int nTranCode, int nSec = FALSE);    //0200	0210	����Ʈ��������Ʈ��ȸ                                                                                                                                                                  
// 	int	fn_FRX_TRANID_7421(int nTranCode, int nSec = FALSE);    //0200	0210	�ü���ȸ                                                                                                                                                                              
// 	int	fn_FRX_TRANID_7425(int nTranCode, int nSec = FALSE);    //0200	0210	������ȸ(����)                                                                                                                                                                        
// 	int	fn_FRX_TRANID_7426(int nTranCode, int nSec = FALSE);    //0200	0210	�ѵ����Ⱑ�ɱݾ���ȸ(����)                                                                                                                                                            
// 	int	fn_FRX_TRANID_7427(int nTranCode, int nSec = FALSE);    //0200	0210	�ѵ������û(����)                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7428(int nTranCode, int nSec = FALSE);    //0200	0210	ķ������ȸ(����)                                                                                                                                                                      
// 	int	fn_FRX_TRANID_7430(int nTranCode, int nSec = FALSE);    //0200	0210	���� �м��� ��ȸ                                                                                                                                                                    
// 	int	fn_FRX_TRANID_7431(int nTranCode, int nSec = FALSE);    //0200	0210	���� �м��� ������ȸ                                                                                                                                                                
	


// 	//////////////////////////////////////////////////////////////////////////////////////////////
// 	//���ݼ��� ������ ��ȸ  
// 	int fn_FRX_TRANID_7380(int nTranCode, int nSec = FALSE);    //0800	0810	���ݼ��� ��� ������ ��ȸ
// 	int fn_FRX_TRANID_7530(int nTranCode, int nSec = FALSE);    //0800	0810	���ݼ��� ��ü ������ ��ȸ
// 	//////////////////////////////////////////////////////////////////////////////////////////////
// 	int fn_FRX_TRANID_7229(int nTranCode, int nSec = FALSE);    //0800	0810	�����ܾ� ��ȸ 
// 
// 	int fn_FRX_TRANID_4717(int nTranCode, int nSec = FALSE);    //0800	0810	�����н� �Ա� 

//	int fn_FRX_EMVRecv_Lib(int nTranCode, int nSec = FALSE);    //EmvLib
/////////////////////////////////////////////////////////////////////////////
#endif
