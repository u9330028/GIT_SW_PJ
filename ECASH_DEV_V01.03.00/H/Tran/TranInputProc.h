/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranInputProc_H_INCLUDED_)
#define _TranInputProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	�ѵ��ݾ׺���
/////////////////////////////////////////////////////////////////////////////
	int		MoneyMaxOut;				// ���Ⱑ���ѵ��ݾ�(����)
	int		MoneyMaxOutCnt;				// ���Ⱑ�ɸż�
	int		CashMaxOutCnt;				// ���ݹ��Ⱑ�ɸż�
	int		CheckMaxOutCnt;				// ��ǥ���Ⱑ�ɸż�
	int		CheckLowMaxOutCnt;			// ��ǥ�����ּҸż�
	int		CheckHighMaxOutCnt;			// ��ǥ�����ִ�ż�

	int		TMoneyMaxOut;				// ��ü�����ѵ��ݾ�(��)

	int		MoneyMaxIn;					// �Աݰ����ѵ��ݾ�(����)
	int		CashMaxIn;					// �����Աݰ��ɱݾ�
	int		CheckMaxInCnt;				// ��ǥ�Աݰ��ɸż�
	int		HotKeyFlag;					// U8100 ����
/////////////////////////////////////////////////////////////////////////////
//	�Էº���
/////////////////////////////////////////////////////////////////////////////
struct	tagAccept						// �Էº���
{
	int		MuAccountNumSize;			// ����ü���¹�ȣ����
	char	MuAccountNum[16];			// ����ü���¹�ȣ
	char	PbConfirmNum[6];			// ����ŷ����ι�ȣ		
	int		PbConfirmNumSize;			// ����ŷ����ι�ȣ����	
	char	CardReadTime[12];			// �ŷ���ü ���� �ð�

	char	PassWord[4];				// ��й�ȣ
	char	PassWordEnc[64];			// ��ȣȭ�� ��й�ȣ
	char	PbPassWord[6];				// ����ŷ��й�ȣ
	char	MuPassWord[8];				// ����ü��й�ȣ
	char	KCashPassWord[8];			// K-CASH��й�ȣ
	char	SlockPassWord[8];			// Offline Lock ��й�ȣ
	char	UserId[16];					// User Id
	char	UserPassWord[16];			// User Pwd
	char	MPINConfirmNum[6];			// �����PIN��ȣ  #N0182

	char	DesPassWord[12];			// DES OutGoing�ŷ� 2004.05.31
	char	DesEncryptPin[16];			// DES OutGoing�ŷ� 2004.05.31

/////////////////////////////////////////////////////////////////////////////
//	�ݾװ��ú���
/////////////////////////////////////////////////////////////////////////////
	char	Money[15];					// �ݾ�(����), �ݾ�(��õ��)
	char	CashMoney[15];				// �ݾ�(������)/�����Ա�
	char	CheckMoney[15];				// �ݾ�(��ǥ��)/��ǥ�Ա�
	char	Cash10T_Money[15];			// �ݾ�(������)		
	char	Cash50T_Money[15];			// �ݾ�(5������)	
	char	MotherCheckMoneyM[15];		// �ݾ�(��ǥ��)/��ǥ�Ա������������ݾ�
	char	MotherCheckMoneyO[15];		// �ݾ�(��ǥ��)/��ǥ�Ա�������Ÿ���ݾ�
	char	OtherCheckMoney[15];		// �ݾ�(��ǥ��)/��ǥ�Ա���Ÿ��ݾ�
	int		MoneyInKind;				// �Աݱ���
	int		ChargeCommission;			// V02-10-02-R1 Ÿ���Ա� ������ 2006.12.06
	
	int		CheckCount;					// ��ݼ�ǥ�ż�
	char	CheckNumber[10];			// �����ڵ�(2)+��ݼ�ǥ��ȣ(8)

	char	RemainRemMoney[15];			// ���׼۱ݾ�
	char	BalanceRemMoney[15];		// �����Աݾ�
/////////////////////////////////////////////////////////////////////////////
	BYTE	PbPageInfo[4];				// ��������������:����
	char	BankNum[3];					// �����ȣ

	int		AccoutnNumSize;				// ���¹�ȣ����
	char	AccountNum[16];				// ���¹�ȣ

	int     InAccountNumSize;			// ������¹�ȣ����
	char	InAccountNum[16];			// ������¹�ȣ

	int		RemitAccountNumSize;		// �����Աݰ��¹�ȣ����
	char	RemitAccountNum[16];		// �����Աݰ��¹�ȣ

	char	JuminNum[13];				// �ֹι�ȣ
	char	TelNum[20];					// ��ȭ��ȣ
	int		TelNumSize;					// ��ȭ��ȣ����

	char	ChangePwd[4];				// ��й�ȣ(����)
	char	ChangePwdConfirm[4];		// ��й�ȣ(����Ȯ��)
	char	ChangeICPwd[8];				// K-CASH��й�ȣ(����)
	char	ChangeICPwdConfirm[8];		// K-CASH��й�ȣ(����Ȯ��)

	int		LoanAccountNumSize;			// ������¹�ȣ����
	char	LoanAccountNum[16];			// ������¹�ȣ

	int		JiroTranCode;				// ���μ��ΰŷ��ڵ�
	char	JiroTranOrgCode[9];			// ���γ��� ����ڵ�
	char	JiroNum[7];					// ���ι�ȣ
	int		JiroElecNumSize;			// ���ڳ��ι�ȣ����
	char	JiroElecNum[19];			// ���ڳ��ι�ȣ
	char	JiroSelItem[5];				// �Ϲ����� �ŷ��� ���ó���
	char	JiroBillCount[2];			// ���� ���ðǼ�
	char	JiroPayMonth[2];			// ���� ���ðǼ�

	BYTE	JiroTranDetails[1024];		// ���γ��� ���ų�������Ÿ
	char	JiroMoney[8];				// �ݾ�(����), �ݾ�(��õ��)

	int		ETicketIndex;				// ���ڻ�ǰ�� ��з� ���� #N0142
	int		ETicketConfirmFlg;			// 
	int		ETicketDetail;				// ���ڻ�ǰ�� �Һз� ���� #N0142
	int		ETicketPayWay;				// ���ڻ�ǰ�� ���� ��� #N0142
	int		ETicketAmount;				// ���ڻ�ǰ�� �ݾ� #N0142
	int		ETicketChange;				// ���ڻ�ǰ�� �ܵ� #N0142
	char	SendTelNum[12];				// ���ڻ�ǰ�� ������ ��ȭ��ȣ #N0142
	char	SendMsg[50];				// ���ڻ�ǰ�� ������ ��ȭ��ȣ #N0142
	char	ETicketConfirmNum[8];		// ���� ���ι�ȣ
	char	ETicketNum[12];				// �ŷ��Ϸù�ȣ
	char	ETicketSerialNum[16];		// ������ȣ

	char	RegCheckNum[8];				// ��ϼ�ǥ��ȣ
	char	RegCheckCount[5];			// ��ϼ�ǥ�ż�

	int		SerialCount;				// ���Ӱŷ�ī��Ʈ
//	int		SerialTranFlag;				// ���Ӱŷ�����(���,�۱�,�Ա�)
	int		SlipChoiceFlag;				// ��ǥ���ڿ���
	int		A4ChoiceFlag;				// A4���������ڿ���

/////////////////////////////////////////////////////////////////////////////
	char	CMSCode[20];				// CMS(����/������ȣ)
	int		CMSCodeSize;				// CMS����
	char	CMSOrgName[30];				// CMS(�����)

	int		LostAccountNumSize;			// �нǰ��¹�ȣ����
	char	LostAccountNum[16];			// �нǰ��¹�ȣ

	char	AutoDate[3];				// �ڵ���ü��
	char	AutoCycle[3];				// ��ü�ֱ�

	char	InquiryDate[8];				// �����ȸ��
	char	InquiryDateEnd[8];			// �����ȸ��
	char	InQuiryCycle;				// ��ȸ�Ⱓ����(����,����)
	char	RemitName[20];				// �۱��θ�

	int		RemitNameFlag;				// �۱����Է��÷���

/////////////////////////////////////////////////////////////////////////////
	int		FICSelectAccountIndex;		// ����IC���ð����ε���	// 2004.02.03

/////////////////////////////////////////////////////////////////////////////
	int		SelStockIndex;				// ���������� �ε���
	char	StocksCode[20];				// �����ڵ�
	char	StocksReqNum[15];			// ��û����
	char	ByOnMargin[15];				// ���ű�
	char	LoanMoney[15];				// �����
	char	RefundAccountNum[16];		// ȯ�Ұ���
	char	NextInqCondition[20];		// ������ȸ����

	char	StocksDate[8];				// û����
	char	RefundAccountName[20];		// ȯ�Ұ��¸�
	char	RefundDate[8];				// ȯ������
	char	StocksListDate[8];			// ��������
	char	InAccountName[20];			// �Աݰ��¸�
	char	HostReqNum[12];				// HOST ���ŷ���ȣ(��Ұŷ��� ���)

	char	MuConfirmNum[2];			// �ؿ�ī�� ����
	int		MuTranFlag;
	int		MuBankTranBrandFlag;

	char	SMSCode[20];				// Phone SMS
	int		SMSCodeSize;				// SMS����

	int		AuthFlag;					// ���� Y,N 
	int		ProcessStep;				// ����ī��Ʈ 
	int		InComeCode[10];				// ���Աݾ�
	int		JobCode[10];				// �����ڵ�

	int		Index;

	int		LoanFlag;
	char	LoanCode[5];				// ���ڵ�
	char	LoanGubunCode[10];				// ���ڵ�
	char	LoanType[3];
	char	LoanMonth[3];
	char	LoanName[20];				//sj.Kim  2011.03.10

	char	CrmSrcFlag[4];
	char	CrmStartFlag[6];
	char	CrmMenuFlag[6];
	char	CrmCode[4];
	char	CrmDummy[100];

	char	InTelNum[16];

	int		EasyOneNextInqFlg;			//������ ���� ��ȸ ���� ����.#N0174
	char	EasyOneName[25];			//������ ������ ����
	char	EasyOneOrgName[35];			//������ ���� ���
	union	
	{
		struct {
			char	UserName[12];					// ����
			char	UserAge[3];						// ����
			char	UserGender[1];					// ����
			char	UserHeight[3];					// Ű
			char	UserWeight[3];					// ������
			char	Date[8];						// ��¥
			char	Time[6];						// �ð�
			char	TeleCom[3];						// ��Ż�
			char	ADGubunCode[2];					// ���ͷ�Ƽ�걤�� �����ڵ�
			char	ADName[20];						// ���ͷ�Ƽ�걤�� �̸�
		} InterAD;

		struct {
			char	GameName[20];					// ����ϰ��� �ٿ�ε� �̸�
			char	TeleCom[3];						// ��Ż�
			char	GameCompanyCode[3];				// ����ϰ��� �ٿ�ε� ȸ���ڵ�
			char	GameServiceCode[8];				// ����ϰ��� �ٿ�ε� �����ڵ�
		} MobileGame;

		struct {
			//  Hi-Pass������������	#N0266
			char	AvailLoadAmt[8];				// Hi-Pass �������ɱݾ�
			char	LoadAmt[8];						// Hi-Pass �����ݾ�
			char	CardNum[16];					// Hi-Pass ī���ȣ
		} HiPass;

	};

} Accept;
	
	// Operations - EMV �߰�
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
//	�������Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_AcceptOpPassword();
	int		fnAPP_AcceptManagerPassword();
	int		fnAPP_AcceptEveryDayManagerPassword();

/////////////////////////////////////////////////////////////////////////////
//	�ѵ��ݾ׼����Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_CalcMoneyMaxOut();							// �����ѵ����ϱ�
	int		fnAPP_CalcMoneyMaxOutATM();							// ATM���������ѵ����ϱ�
	int		fnAPP_CalcMoneyMaxOutCD();							// CD���������ѵ����ϱ�
	int		fnAPP_CalcCheckLowMaxOutCnt(int MoneyOut);			// ��ǥ�ּҸż����ϱ�
	int		fnAPP_CalcCheckHighMaxOutCnt(int MoneyOut);			// ��ǥ�ִ�ż����ϱ�
	int		fnAPP_CalcMoneyMaxTrans();							// ��ü�ѵ����ϱ�
	int		fnAPP_CalcMoneyMaxIn();								// �Ա��ѵ����ϱ�
	int		fnAPP_CalcCashMaxIn();								// �����Ա��ѵ����ϱ�
	int		fnAPP_CalcCashMaxInAmount();
	int		fnAPP_CalcCheckMaxIn();								// ��ǥ�Ա��ѵ����ϱ�

/////////////////////////////////////////////////////////////////////////////
//	�Է��Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_AcceptWith();									// ����Է�
	int		fnAPP_AcceptInq();									// ��ȸ�Է�
	int		fnAPP_AcceptInqDate();								// ��ȸ���Է�
	int		fnAPP_AcceptTrans();								// ��ü�Է�
	int		fnAPP_AcceptDep();									// �Ա��Է�
	int		fnAPP_AcceptDep_KB();								// �Ա��Է� ���� //#N0226

	int		fnAPP_AcceptMuAccountNum();							// ����ü���¹�ȣ�Է�
	int		fnAPP_AcceptMuConfirmNum();							// ����ü�ŷ����ι�ȣ�Է�
	int		fnAPP_AcceptLostAccountNum();						// ���Ű���¹�ȣ�Է�
	int		fnAPP_MuAcceptCardChoice();

	int		fnAPP_AcceptPassword();								// ��й�ȣ�Է�
	int		fnAPP_AcceptPbConfirmNum();							// ����ŷ����ι�ȣ�Է�
	int		fnAPP_AcceptTelPassword();							// ����ŷ��й�ȣ�Է�
	int		fnAPP_AcceptMuPassword();							// ����ü�ŷ���й�ȣ�Է�
	int		fnAPP_AcceptDesPassword();							// DES��й�ȣ�Է�
	int		fnAPP_AcceptMPINConfirmNum();						// ONEMONEY  #N0182
	
	int		fnAPP_AcceptMoney();								// ��ݱݾ��Է�
	int		fnAPP_AcceptTMoney();								// ��ü�ݾ��Է�
	int		fnAPP_AcceptMoneyInKind();							// �Աݱ�������
	int		fnAPP_AcceptMoneyInConfirm();						// �Աݱݾ�Ȯ��
	int		fnAPP_AcceptNhMoneyInConfirm();						// ����� �Աݱݾ�Ȯ��

	int		fnAPP_InputMoneyCase_Etc();			// �ݾ׼���(��ư) - Hnet Ư�� ��� 		
	int		fnAPP_SelectMoneyCase0(char * WithMsg);				// �ݾ׼���(��ư),			
	int		fnAPP_InputMoneyCase1(char * WithMsg);				// �ݾ��Է�,			
	int		fnAPP_SelectKindofMoneyCase2(int CheckValue);		// ��������,				//��ǥ���� parameter �߰�
	int		fnAPP_SelectCheckCase3(char * WithMsg);				// ��ǥ����, �Է�,		
	int		fnAPP_InputCheckCase4(int CheckValue);				// ��ǥ�Է�,				//��ǥ���� parameter �߰�
	int		fnAPP_Input50TCashMoney();
	int		fnAPP_Input50TCashMoney_HANA();						//#N0214
	
	int		fnAPP_AcceptRemitName();							// �۱��θ��Է�
	int		fnAPP_AcceptRMoney();								// ���׼۱ݱݾ��Է�
	int		fnAPP_AcceptRemitConfirm();							// �۱�Ȯ��

	int		fnAPP_AcceptBankNum();								// �����ȣ�Է�
	int		fnAPP_AcceptInAccountNum();							// ������¹�ȣ�Է�
	int		fnAPP_AcceptCmsCode();								// �����ι�ȣ�Է�

	int		fnAPP_AcceptInTransStock();							// ���������ڻ��ü�Է�
	int		fnAPP_AcceptInTransReqStock();						// û������Է�
	int		fnAPP_AcceptInTransReqStockAccount();				// ȯ�ұݰ���¹�ȣ�Է�

	int		fnAPP_AcceptChangePassword();						// ��й�ȣ�����Է�
	int		fnAPP_AcceptLoanAccountNum();						// ������¹�ȣ�Է�

	int		fnAPP_AcceptTelNum();								// ��ȭ��ȣ�Է�
	int		fnAPP_AcceptJuminNum();								// �ֹι�ȣ�Է�
	int		fnAPP_AcceptCompareJuminNum();						// �۱��ֹι�ȣ��
	CString fnDES_Cvt_CardNum(char* pVCardNum, int n_size);
	CString fnDES_Cvt_CardPan(char* pVCardNum, int n_size);      //#N0176

	int		fnAPP_AcceptAutoDetail();							// ��ü�ֱ�,��ü�����
	int		fnAPP_AcceptAutoConfirm();							// �ڵ���ü����Ȯ��

	int		fnAPP_AcceptInqDateEnd();							// �ŷ������ȸ���Է�

	int		fnAPP_AcceptTranChoice();							// ��ȭ��� ���� �߰� 

	int		fnAPP_AcceptSlipChoice();							// ��ǥ���ڿ��μ���
	int		fnAPP_AcceptContinuousPBChoice();					// �������� ���Ӱŷ� ���μ���
	int		fnAPP_AcceptTranPBProc();							// �������� ���Ӱŷ� ���μ���

	int		fnAPP_AcceptKCashPassword(int nReadDF, int ScrNum = 0);
																// K-CASH��й�ȣ�Է�
	int		fnAPP_AcceptKCashMoney();							// K-CASH�ݾ��Է�
	int		fnAPP_AcceptKCashChangePassword(int nReadDF = TRAN_IC_KCASH);
																// K-CASH��й�ȣ�����Է�
	int		fnAPP_FICSelectAccount();							// ����IC ���¼���

	int		fnAPP_TranWithChargeInqInputProc();

	
	int		fnAPP_UTILYBILLS_Sub01_InputProc();							// ���μ��ΰŷ��ڵ弱��
	int		fnAPP_UTILYBILLS_Sub02_InputProc();								// ���ι�ȣ�Է�
	int		fnAPP_UTILYBILLS_Sub03_InputProc();							// ���ڳ��ι�ȣ�Է�

	//#GIRO 
	int		fnAPP_LOCALTAX_Sub01_InputProc();							// ��ȸ��� ����
	int		fnAPP_LOCALTAX_Sub02_InputProc();							// �Һμ���
	int		fnAPP_LOCALTAX_AcceptPayMonth();							// �Һΰ��� ����
	int		fnAPP_LOCALTAX_AcceptSimpleNum();							// �����ι�ȣ 
	int		fnAPP_LOCALTAX_AcceptElecNum();								// ���ڳ��ι�ȣ

					

/////////////////////////////////////////////////////////////////////////////
#endif
