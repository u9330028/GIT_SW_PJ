/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCmnHost_H_)
#define _TranCmnHost_H_
#include "..\..\H\Tran\TranCmnDefine.h"
/////////////////////////////////////////////////////////////////////////////
//
//		������� ���� ���� 
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ȣ��Ʈ ���� �ڵ�
/////////////////////////////////////////////////////////////////////////////
#define HOST_OFFON			1			// ������ ��� ����� ��û����
#define	HOST_DFS_ERROR		2			// DFS ERROR
#define	HOST_CAP_ERROR		3			// CAP_HEADER ERROR
#define	HOST_BPH_ERROR		4			// BP_HEADER ERROR
#define	HOST_INP_ERROR		5			// INPUT ERROR

#define SEED_SEND_EXCEPT_LEN	25		// ��ȣȭ���ܱ���
#define SEED_RECV_EXCEPT_LEN	16		// ��ȣȭ���ܱ���

/////////////////////////////////////////////////////////////////////////////
// ���� ���� �����
/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// DS Header(DV + MI + DS + US)
typedef struct _tagDSHEAD
{
	BYTE	DeviceID;					// Device id	: 0xa4
	BYTE	MI;							// Message information
	BYTE	ModeInfo;					// Mode ����
	BYTE	CardType1;					// ī������1
	BYTE	TranType;					// �ŷ�����
	BYTE	CardType2;					// ī������2
	BYTE	PbType;						// ��������
	BYTE	Reserved1;					// �����ʵ�
	BYTE	ContInfo;					// CONT����
	BYTE	JrnInfo;					// ��������
	BYTE	Reserved2;					// �����ʵ�
}DSHEAD;

//////////////////////////////////////////////////////////////////////
// Send CAP Header
typedef struct _tagCAPHEAD
{
	BYTE	US1;						// ������	: 0x1f
	BYTE	trId[5];					// ����ID
	//BYTE	Blink;						// blink	: 0x20
	BYTE	HostSvcName[16];			// Host service(process) name
	BYTE	MachineType;				// ��������	: 'A'- ATM, 'C' - CD
	BYTE	TermKey[16];				// �Ҽ�����1:3 + ó������:3 + �Ҽ�����2:5 + ���:3
	BYTE	trIdCode1[2];				// �����ĺ��ڵ�1	: all set 0x20
	BYTE	trIdCode2[4];				// �����ĺ��ڵ�2	: all set 0x20
	BYTE	UserFlag[3];				// user flag
	BYTE	statFlag[3];				// ��Ȳ flag
	BYTE	PbIssueNum;					// �������ȸ��		: 0x30(����)
	BYTE	SeqNo[4];					// �ŷ��Ϸù�ȣ
	BYTE	ReqPrnLine[2];				// ����: "04", �׿ܰŷ�: "10"
	BYTE	Reserved[3];				// all set 0x20(����)
	BYTE	US2;						// ������	: 0x1e
}CAPHEAD;

//////////////////////////////////////////////////////////////////////
// ����/�忡 �۽� ����
typedef struct _tagNMSSEND
{
	BYTE	StatusInfo[20];				// ��������
	BYTE	US1;						// filler
	BYTE	ErrorInfo[10];				// �������
	BYTE	US2;						// filler
	BYTE	HostSeqNum[12];				// �ŷ��Ϸù�ȣ
	BYTE	AccountNum[16];				// ���¹�ȣ
	BYTE	Reserved[2];				// Reserved
//	BYTE	US3;						// filler
} NMSSEND;


//////////////////////////////////////////////////////////////////////
// 
// IC���� (151 Byte)
typedef struct _tagICINFO
{
	BYTE	ICAccountNum[16];			// ī�忡 �簢�� �Ϸù�ȣ
	BYTE	SeedPassword[96];			// 32Byte : ī�� ������
										// 32Byte : �͹̳�(CD��) ���� ��
										// 32Byte : ��ȣȭ�� ��й�ȣ ��
										// �Ա���ü(�۱�),�Ա���ü������ȸ, 3����ü������ȸ
										// 3����ü�� �Ա����� ���������� 0xF0���� SET
	BYTE	KeyVersion[2];				// ����IC Key Version
	BYTE	Dummy[36];					// ���� (All Space(0x40))
	BYTE	Delimiter;					// Delimiter (0x3E ����)
} BANKICINFO;


//////////////////////////////////////////////////////////////////////
// ���� ����
typedef struct _tagCLOSEINFO
{
	BYTE	TotalWithAmt[15];			// ����ݾ�
	BYTE	CashWithAmt[15];			// ������ݱݾ�
	BYTE	CashDeositAmt[15];			// �����Աݱݾ�
	BYTE	CheckWithAmt[15];			// ��ǥ�����
	BYTE	CheckDepositAmt[15];		// ��ǥ�Աݾ�
}CLOSEINFO;

//////////////////////////////////////////////////////////////////////
// ��Ұŷ� ����
/*
typedef struct _tagCANCELINFO
{
	BYTE	CancelType;					// ��ҼӼ�		: '2'����
	BYTE	CancelSerial[4];			// ��� �ŷ� �Ϸù�ȣ
	BYTE	CancelDate[8];				// ��Ұŷ���
	BYTE	DFlag;						// �������ڱ���	: '0'����
	BYTE	CFlag;						// �������ı���	: '0'����
	BYTE	MaterialFlag;				// ��ü��������	: '0'��ü��, '1'��ü��
	BYTE	CancelInfo[30];				// ��ҳ���		: USER Header�Է�����(25) + LC(5)
	BYTE	Reserved[3];				// ����			: '0'����
	BYTE	Delimiter;					// �Է¼Ӽ�		: 0x3B
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
	BYTE	PWD[4];						// ��й�ȣ
	BYTE	Dummy[7];					// ����
	BYTE	Delimiter;					// Delimiter (0x3E ����)
} EMVICINFO;

//////////////////////////////////////////////////////////////////////
// ���� ���� 
//////////////////////////////////////////////////////////////////////
// Receive Cap Header
typedef struct _tagRECVCAPHEAD			// ����ó�� command�� ���
{
	BYTE	CenterCode[4];				// Center Code
	BYTE	AcpStatusFlag[3];			// ACP ��Ȳ Flag
	BYTE	Reserved[3];				// ����
} RECVCAPHEAD;

//////////////////////////////////////////////////////////////////////
// Receive Common Header
typedef struct _tagRECVHEAD
{
//	BYTE	DeviceId;					// Device id	: 0xa4 ����
	BYTE	MessageInfo;				// Message information
	BYTE	UserInfo;					// user information
	RECVCAPHEAD RecvCapHead;			// receive CAP header
	BYTE	SeqNo[4];					// �ŷ��Ϸù�ȣ
	BYTE	trId[4];					// ����ID
	BYTE	HostErrCode[4];				// Host error code
	BYTE	HostErrMsg[80];				// Host error message
}RECVHEAD;

// Receive Common Data
typedef struct _tagRECVCOMM
{
	RECVHEAD RecvHead;					// Receive Header
	BYTE	TransData[4096];			// ���� �����ͺ�
}RECVCOMM;

//////////////////////////////////////////////////////////////////////
// EMV���� V03-01-01-#01
typedef struct _tagEMVRecv
{
	BYTE	UI_STA;						// 'E'
	BYTE	Certificate;				// ��������
										// 1 : ����, ��ȸ�ŷ��ô� ������������
										// 2 : ����
	BYTE	ARPC[16];					// 
	BYTE	ARPCResCode[4];				// ARPC Response Code
	BYTE	ATC[4];						// ATC
	BYTE	IssuerLen[2];				// Issuer Script Code
	BYTE	IssuerScript[166];			// Issuer Script
	BYTE	UI_End;						// UI_End
} EMVRecv;

/////////////////////////////////////////////////////////////////////////////
typedef struct tagPSPRecv				// DATA��/������Ʈ(�ŷ�) ��������� ����
{
	BYTE	ParaLength;					// PARA����
	int		By;							// Begin y
	int		Ey;							// End y
	int		y;							// Position y
	int		x;							// Position x
	BYTE	Data[256][256];				// DATA
} PSPRecv;

typedef struct tagPPPRecv				// DATA��/���������Ʈ(�ŷ�)
{
	BYTE	ParaLength;					// PARA����
	int     Bp;							// Begin Page
	int     Ep;							// End Page
	int		p;							// Position p
	int		y;							// Position y
	int		x;							// Position x
	int		DataLength[10][25];			// DATA Length		
	BYTE	Data[10][25][256];			// DATA
} PPPRecv;


typedef struct tagPPPRecvKb				// DATA��/���������Ʈ(�ŷ�)  //#N0226
{
	BYTE	ParaLength;					// PARA����
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
	// ���DATA��/���������Ʈ(�ŷ�)
										// CMD�ĺ���('0xff00')
	BYTE	ParaLength;					// PARA����('0x01')
	BYTE	CmdPara;					// CMD PARA
	int		p;							// Position p
	int		y;							// Position y
	int		x;							// Position x
	BYTE	SkipLineCnt[2];		
	BYTE	DataLineCnt[3];	
	BYTE	Data[10][24][128];			// DATA
} PPPRecvNon;


typedef struct tagPPWRecv				// DATA��/����M/S WRITE(�ŷ�)
{
	BYTE	ParaLength;					// PARA����
	int		x;							// Position x
	BYTE	Data[256];					// DATA
} PPWRecv;
	
typedef struct tagPCWRecv				// DATA��/ī��M/S WRITE(�ŷ�)
{
	BYTE	ParaLength;					// PARA����
	int		x;							// Position x
	BYTE	Data[256];					// DATA
} PCWRecv;
	
typedef struct tagPMERecv				// DATA��/�޼�������ǥ��(�ŷ�/����)	0xff24
{
	BYTE	ParaLength;					// PARA����
	int		By;							// Begin y
	int		Ey;							// End y
	int		y;							// Position y
	int		x;							// Position x
	BYTE	Data[256][256];				// DATA
} PMERecv;

typedef struct tagPMDRecv				// DATA��/�޼���ǥ��(�ŷ�)
{
	BYTE	ParaLength;					// PARA����
	int		By;							// Begin y
	int		Ey;							// End y
	int		y;							// Position y
	int		x;							// Position x
	BYTE	Data[30][256];				// DATA
} PMDRecv;

typedef struct tagETCRecv				// DATA��/ETCó��(����ȭ��, �ؿܰŷ�)
{
	BYTE	Data[73];					// ����(�ڷ�����)
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
	char	c_r_yymmdd[14];			// ��������ϳ���
	char    c_v_yymmdd[14];			// ��������ȿ����
	char	k_r_yymmdd[14];			// ����Ű��ϳ���
	char    k_v_yymmdd[14];			// ����Ű��ȿ����
	char    k_s_flag[1024];			// ����Ű_flag
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
	char	k_r_yymmdd[12];			// ����Ű��ϳ���
	char    k_v_yymmdd[12];			// ����Ű��ȿ����
	BYTE	k_apart[1];				// Ű ����
	BYTE	k_len[5];				// Ű ���� 
	BYTE	k_data[2048];			// Ű ������ (043x)
	BYTE	k_data_sub[2048];		// ���� Ű ������  (044x)
	BYTE	k_sublen[5];			// Ű ���� 
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
	char byDataField_002x[98];  //�ð�
	char byDataField_004x[98];  //���޻� �Ϸù�ȣ 
	char byDataField_009x[98];  //�������� 
	char byDataField_017x[98];  //���¹�ȣ
	char byDataField_023x[98];  //�ŷ��ݾ�
	char byDataField_026x[98];	//�Ϸù�ȣ
	char byDataField_027x[98];	//��� ����(������ü)
	char byDataField_024x[98];	//������
	char byDataField_022x[98];  //�����ܾ�
	char byDataField_025x[98];	//Ÿ����
	char byDataField_021x[98];  //������
	char byDataField_012x[98];  //������
	char byDataField_020x[98];	//�������
	char byDataField_104x[98];	//��ǥ����
	char byDataField_458x[98];	//������ (���ݼ���)
	char byDataField_345x[98];	//������ ����Ʈ ���� ���� 
	char byDataField_018x[98];	//���� �귣�� ���� ����
	char byDataField_307x[98];	//����ī�� �ѵ�����	 
	char byDataField_1721[98];	//����ī�� �ѵ�����	 
	char byDataField_065x[98];	//������
	char byDataField_Dummy[98];	//����ī�� �ѵ�����	 
} RXDISPLAYDATA;

// TYPE 3 : �ſ�ī�� EMV���� RCV SCRIPT
typedef struct	tagRecvEmvrcvData		// �ſ�ī�� EMV���� RCV SCRIPT	
{
	char	ARPC[16];					// ARPC 
	char	APRCResposeCode[4];			// APRCResposeCode 
	char	IssuerScriptLen[3];			// IssuerScriptLen
	char	IssuerScript[256];			// IssuerScript
	char	IssuerScriptResult[40];		// IssuerScriptResult
	char	Reserve[17];				// Reserve
} RECVEMVRCVDATA;

typedef struct tagRXHNETJIRODATA{
	char JiroBankCode[16];  //����������ڵ�
	char Reserve1[1];
	char JiroNum[9];		//���ι�ȣ
	char Reserve2[1];
	char ElecNum[21];	//�ŷ��ݾ�
	char Reserve3[1];
	char JiroOrgName[16];	//�̿�����
	char Reserve4[1];
	char AccountName[10];	//����θ�
	char Reserve5[1];
	char CustomerName[16];  //�����ڼ���
	char Reserve6[1];
	char JiroPayData[300];  //���γ���
} RXHNETJIRODATA;

typedef struct tagRXHNETLOCALTAXDATA{
	char JiroBankCode[16];  //����������ڵ�
	char Reserve1[1];
	char CustomerName[16];  //�����ڼ���
	char Reserve2[1];
	char Gubun[2];			//�Ǻ����� ������
	char Reserve3[1];
	char ElecNum[21];		//�Ǻ����� �Ϸù�ȣ
	char Reserve4[1];
	char TaxCount[4];		//�Ѱ����Ǽ�
	char Reserve5[1];
	char TotalAmount[12];	//�Ѱ����ݾ�
	char Reserve6[1];
	char BillCount[2];		 //���ΰǼ�
	char Reserve7[1];
	char BillPayMonth[2];	 //�Һΰ���
	char Reserve8[1];				
	char JiroPayData[300];  //���γ���
} RXHNETLOCALTAXDATA;

typedef struct tagRXETICKETDATA{
	char TicketLen[5];				//��ǰ DATA ����
	char BigData[995];				//��ǰ ��з�
	char DetailData[6000];			//��ǰ �з�
	char AcceptDetailData[376];		//��ǰ ���� ���� data
	char SMSKey[6];					//SMS ���� ��ȣ
	char byDataField_003x[6];		//SMS ���ŷ� SST�Ϸù�ȣ
	char byDataField_343x[4];		//���ڻ�ǰ�� ��ȿ�Ⱓ 
	char ICINFO[130];				//IC DATA
} RXETICKETDATA;


//#N0174 ������ DATA ����
typedef struct tagRXEASYONEDATA{
	char Gubun[2];					//������ �ŷ� ����
	char NextFlag[1];				//������� ���� ����
	char NextKey[15];				//���� ��� ��ȸ�� �ʿ� Ű
	char Count[2];					//���� �ݺ� Ƚ�� (�ִ� 5)
	char EasyOne_Acc[5][75];				//������ ��ü ���� ����
	char Dummy[100];				//Dummy
} RXEASYONEDATA;



/*******************************************************************************************************************************
/* �����н� ���� ���� (Structure TX, RX) ______Begin , Yun.H.J 2012.04.17
/*******************************************************************************************************************************/

/*------------------------------------------------------------------------------*/
/* �����н����� �����Ա� ����													*/
/* Length : 250 + 4040 = 4290													*/
/* Date   : 2012.04.17															*/
/* Author.: Yun.H.J																*/
/*------------------------------------------------------------------------------*/
typedef struct tagRXATM4717
{
	BYTE	dsjpdata[160];			// ȭ�� Data(����޽���)
	BYTE	seqno[12];				// �ŷ�������ȣ
	BYTE	traceno[6];				// ���� ���� ��ȣ
	BYTE	accno[32];				// ����/ī���ȣ ����(2)+���¹�ȣ(30) --> 1091 : Sectionid 12Byte (2010.2.2)
	BYTE	amount[16];				// �ݾ�
	BYTE    remain[16];				// �ܾ�
	BYTE	interseqno[16];			// ���ΰŷ� ������ȣ (��������)
	BYTE	reserved[3782];         // pbm list data     3600 byte -> ����ȣ���� (��ִ�å 2010.04.22)
}RXATM4717, NEAR *NPRXATM4717, FAR *LPRXATM4717;

/*------------------------------------------------------------------------------*/
/* �����н����� �ſ����/��� ����												*/
/* Length : 250 + ? = ?															*/
/* Date   : 2012.04.17															*/
/* Author.: Yun.H.J																*/
/*------------------------------------------------------------------------------*/
typedef struct tagRXATM4840_4850
{
	BYTE	dsjpdata[160];			// ȭ�� Data(����޽���)
	BYTE	seqno[12];				// �ŷ�������ȣ
	BYTE	traceno[6];				// ���� ���� ��ȣ
	BYTE	session_id[18];			// ����ID
	BYTE	payment[2];				// �������
	BYTE	mobilance[16];			// ������𽺻���ڰ�����ȣ
	BYTE	admno[16];				// ���ι�ȣ
	BYTE 	oseqno[16];				// ���ŷ���ȣ
	BYTE	amount[6];				// �����ݾ�
	BYTE    admtime[14];			// �����Ͻ�
	BYTE	remain[10];				// �������ܾ�
	BYTE	reserved[50];
}RXATM4840_4850, NEAR *NPRXATM4840_4850, FAR *LPRXATM4840_4850;

/*------------------------------------------------------------------------------*/
/* �����н����� ���� ����														*/
/* Length : 250 + 250 = 500															*/
/* Date   : 2012.04.17															*/
/* Author.: Yun.H.J																*/
/*------------------------------------------------------------------------------*/
typedef struct tagRXATM4670
{
	BYTE	dsjpdata[160];			// ȭ�� Data(����޽���)
	BYTE	seqno[12];				// �ŷ�������ȣ
	BYTE	traceno[6];				// ���� ���� ��ȣ
	BYTE	usercard_id[10];		// �̿���ī�� ID
	BYTE	start_bit[1];			// �ŷ����� BIT ����
	char    random_num[16];			// HSM����
	char    s2[8];					// HSM����
	BYTE	reserved[37];
}RXATM4670, NEAR *NPRXATM4670, FAR *LPRXATM4670;

/*------------------------------------------------------------------------------*/
/* �����н����� �����Ϸ� ����													*/
/* Length : 250 + 220 = 470														*/
/* Date   : 2012.04.17															*/
/* Author.: Yun.H.J																*/
/*------------------------------------------------------------------------------*/
typedef struct tagRXATM4671
{
	BYTE	dsjpdata[160];			// ȭ�� Data(����޽���)
	BYTE	seqno[12];				// �ŷ�������ȣ
	BYTE	traceno[6];				// ���� ���� ��ȣ
	BYTE	usercard_id[10];		// �̿���ī�� ID
	BYTE	reserved[32];
}RXATM4671, NEAR *NPRXATM4671, FAR *LPRXATM4671;

/*------------------------------------------------------------------------------*/
/* �����н����� ���� ����														*/
/* Length : 250 + 251 = 501															*/
/* Date   : 2012.04.17															*/
/* Author.: Yun.H.J																*/
/*------------------------------------------------------------------------------*/
typedef struct tagRXATM4680
{
	BYTE	dsjpdata[160];			// ȭ�� Data(����޽���)
	BYTE	seqno[12];				// �ŷ�������ȣ
	BYTE	traceno[6];				// ���� ���� ��ȣ
	BYTE	usercard_id[10];		// �̿���ī�� ID
	BYTE	start_bit[1];			// �ŷ����� BIT ����
	BYTE    random_num[16];			// HSM����
	BYTE    s2[8];					// HSM����
	BYTE    restore_type[1];		// ��������
	BYTE	reserved[37];
}RXATM4680, NEAR *NPRXATM4680, FAR *LPRXATM4680;

/*******************************************************************************************************************************
/* �����н� ���� ���� (Structure TX, RX) ____End
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

