/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranDevProc_H_INCLUDED_)
#define _TranDevProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	��ü����
/////////////////////////////////////////////////////////////////////////////
	int		DevEnDisable;				// ī������En/Disable
	int		CardTran;					// Card�ŷ����º���
	int		ICCardTran;					// Card Transaction
	int		MSCardTran;					// Card Transaction
	int		PbTran;						// Pb�ŷ����º���
	int		PbKind;						// Pb����
	int		PbBank;						// ������������
	int		CardBank;					// ����ī������
	int		CardService;				// �ſ�ī������
	int		CardNew;					// �ű�ī������
	int		CardBankFlag;				// Card Service
	int		CardServiceFlag;			// Card Service 
	int		ExtraServiceFlag;			// �ΰ����� ���� 
	int		BranchKind;					// ��Ÿ����
	int		MenuFICMode;				// ����IC ī��ŷ����º���	2004.02.03
	int		MenuEPBMode;				// ����IC �������� �ŷ����� ����
	int		MenuEMVMode;				// EMVī�� �ŷ����º���
	BYTE	FallBackCnt;				// FallBack ����ī��Ʈ 
	int		MenuKCashMode;				// ����ȭ�󺯼�
	int		MenuRFDMode;				// RF�ŷ����º���
	int		FICMyBiFlg;					// ����ICī�� MyBiī�忩��	2004.02.03
	int		FallBackMenuFICMode;		// ����IC ī��FALLBACK���º���	V03-00-26-#01
	int		FallBackEMVMode;			// EMV IC ī��ŷ����º���		V03-00-26-#01
	int		EMVReadFlg;					// EMV READ FLAG V03-00-25-#02
//	int		BrandSelectIndex;			// ���Ӱŷ��� ���õ� Brand
	int		KCashStopbit;				// ������ƮȮ��
	int		KCashPingChangFlag;			// PIN���/���溯��

	int		CardOnEMV;					// EMV���� �ſ�ī��
	int		EMVAuthError;				// EMV��������

	MCAP	CardData;					// ī���ڷ� 
	MCISO2*	pCardData2;					// 2TR Pointer
	MCISO3*	pCardData3;					// 3TR Pointer
	char	CardAccountNo[256];			// Card Account No			// V09-33-00-#01 ICCM ��� ����
	char	CardPan[256];				// Card Pan					// V09-33-00-#01 ICCM ��� ����

	MS		PbMsData;					// �����ڷ�
	BAR		PbBarData;					// ���� Bar Info
	PBMS*	pPbMsData;					// Pb Pointer
	FINGER	FingerData;					// �����ڷ�
	BAR2D	BarData;					// ���ڵ��ڷ�

	int		CardEmbossScanFlg;			// ī�忥������ĵ����
	int		CardImprintFlg;				// ī��������Ʈ����
	int		CardPowerOnFlg;				// ī����������(����ȭ��)
	int		PbDataCnt;					// Pb��±���
	char	PbData[10240];				// Pb��¿���		

	int				EjrIndexFlg;		// ������������
	EjrTbl			EjrData;			// ���������ڷ�
	EjrAmountTbl	EjrAmountData;		// ��������ݾװ����ڷ�

	char	ShuData[34][256];			// Shu��¿���
	int		SlipPrintFlg;				// ��ǥ��������
	int		JnlPrintFlg;				// ������������
	
	int		MaterialOutFlg;				// ��ü��������
	int		MaterialOutCheckFlg;		// ��ü�����ļ�������
	int		MoneyOutFlg;				// ���ݼ�ǥ��������
	int		MoneyOutCheckFlg;			// ���ݼ�ǥ�����ļ�������

	int		SprOutRetryFlg;				// ��ǥ������õ��÷���	
	int		BrandSelectIndex;			// ���Ӱŷ��� ���õ� Brand		V09-33-00-#01  ICCM ��� ����
	int		EMVServiceCodeChk;			// V07-08-00-#01 �����ڵ� Ȯ�� ����

/////////////////////////////////////////////////////////////////////////////
//	������ú���
/////////////////////////////////////////////////////////////////////////////
	int		CashCountFlg;				// ������ݰ������
	int		CheckCountFlg;				// ��ݼ�ǥ�������

	int		CashInCountFlg;				// �Ա����ݰ������
	int		CashRejectFlg;				// �Ա����ݹ�ȯ����
	int		CashRejectAFlg;				// �Ա������ʰ���ȯ����
	int		CheckInCountFlg;			// �Աݼ�ǥ�������
	int		CheckRejectFlg;				// �Աݼ�ǥ��ȯ����
	int		CheckRejectAFlg;			// �Աݼ�ǥ������ȯ����
	int		CheckRejectKind;			// �Աݼ�ǥ��ȯ����

	RejectMoney		RejectData;			// ȸ������
	DepositMoney	CashData;			// �����Աݳ���
	CheckInfo		CheckInfoData[UCM_MAXCHECKINCNT];	
										// ��ǥ�Աݳ���
	int		CheckInfoStats;				// ��ǥ�Աݻ���
	int		CheckInfoCnt;				// ��ǥ�Աݸż�
	int		CheckInfoAmt;				// ��ǥ�Աݱݾ�
	CheckPrint		CheckPrintData;		// ��ǥ�輭����
	int		CheckInPrintFlg;			// ��ǥ�Ա�������������
	int		SumClearFlag;				// Sum Clear Flag

	int		TranEncCode;				// Encrypt Code
	/////////////////////////////////////////////////////////////////////////////////
	int		KbGwamok;						// ������񺯼�(���� 1:���� 2:��Ź 3:�䱸�� 4:����)
	int		KbTongBank; 					// ��������(1:������,2:������,3:���ձ���)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//	EMV LIBRARY
//  V09-33-00-#01 ICCM ��� ����
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
	int		m_SelectIndex;										// ���Ӱŷ��� ���õ� Brand
	int		m_EMVCancelFlg;										// �������FLG

	//#0164
	int		HiPassLoadFailFlg;									// Hi-Pass������������	
	int		HiPassDeviceKind;									// Hi-Pass�ŷ����	

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	��ü�Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPD_CheckDeviceAction(int ChkID, int ErrChkFlg = TRUE);			
																// ���ۿϷ����&��ְ���

	int		fnAPD_CardEnDisable(int Action, int InitFlg = FALSE, int WaitFlg = FALSE, int ContinueFlg = FALSE);
																// ī�����Ը�弳��
	int		fnAPD_ICCardEnDisable(int Action, int InitFlg = FALSE, int WaitFlg = FALSE);
																// ICī�����Ը�弳��
	int		fnAPD_PbEnDisable(int Action, int InitFlg = FALSE, int WaitFlg = FALSE);			
																// �������Ը�弳��

	int		fnAPD_CardReadCheck(LPCTSTR strInCardData = NULL);	// ī���б�&üũ
	int		fnAPD_ICCardReadCheck();							// ICī���б�&üũ
	int		fnAPD_PbReadCheck();								// �����б�&üũ
	int		fnAPD_IBK_PbReadCheck();							// �����б�&üũ - IBK
	int		fnAPD_NONGHYUP_PbReadCheck();						// �����б�&üũ - NONGHYUP
	int		fnAPD_KEB_PbReadCheck();							// �����б�&üũ - KEB
	int		fnAPD_KJB_PbReadCheck();							// �����б�&üũ - KJB	
	int		fnAPD_HANA_PbReadCheck();							// �����б�&üũ - HANA  #N0214
	int		fnAPD_KB_PbReadCheck();								// �����б�&üũ - KB  #N0226
	int		fnAPD_CTB_PbReadCheck();							// �����б�&üũ - CITI	#N0252
	int		fnAPD_SC_PbReadCheck();								// �����б�&üũ - SC  #N0245
	int		fnAPD_KNB_PbReadCheck();							// �����б�&üũ - KNB  #N0277

	int		fnAPD_GwamokCheck_Kb(char * InGwamok);				// ���� ���� ���� üũ#N0226

	int		fnAPD_KEB_GetPbAccount(BYTE* pbSendAccount);		// ��������������ϱ�
	int		fnAPD_GetPbAccount(BYTE* pbSendAccount);			// ��������������ϱ�
	int		fnAPD_PbMSWrite();		

	int		fnAPD_CardEmbossScan(int WaitFlg = FALSE);			// ī�忥������ĵ
	int		fnAPD_CardImprint(int WaitFlg = FALSE);				// ī��������Ʈ
	int		fnAPD_CardWrite(MCAP* WriteCardData, int WaitFlg = FALSE);
																// ī����

	int		fnAPD_PbPrint(int WaitFlg = FALSE);					// ��������
	int		fnAPD_EjrIndexSet();								// ��������INDEX��������
	int		fnAPD_ShuPrint(int PrintDevice, int JnlType = JNL_TRAN, int WaitFlg = FALSE);	
																// ��ǥ&��������
	int		fnAPD_JnlPrint(int WaitFlg = FALSE);				// ��������(����ó��)

	int		fnAPD_MaterialOut(int Device, int WaitFlg = FALSE, int CheckTime = K_45_WAIT);	
																// ��ü����
	int		fnAPD_A4PMaterialOut(int nPrintEndFlag, int WaitFlg, int CheckTime = K_45_WAIT);
																// A4P��ü����
	int		fnAPD_MaterialOutCheckRetract(int Device, int WaitFlg = TRUE);
																// ��ü���������ȸ��
	int		fnAPD_MaterialRetract(int Device, int WaitFlg = TRUE);			
																// ��üȸ��

	int		fnAPD_MoneyCount(int WaitFlg = FALSE);				// ���ݼ�ǥ��ݰ��
	int		fnAPD_MoneyOutCheck(int ScrNum = 0, int CheckTime = K_60_WAIT);	//#0134
																// ���ݼ�ǥ���������
	int		fnAPD_MoneyRetract();								// ���ݼ�ǥȸ��

	int		fnAPD_CashDeposit();								// �Ա�����
	int		fnAPD_CashDepositCount();							// �Ա����ݰ��
	int		fnAPD_CashDepositStore(int WaitFlg = FALSE);		// �Ա����ݼ���
	int		fnAPD_CashDepositReject();							// �Ա����ݹ�ȯ

	int		fnAPD_CheckDeposit();								// �Աݼ�ǥ
	int		fnAPD_CheckDepositCount();							// �Աݼ�ǥ���
	int		fnAPD_CheckDepositCurOutCheck(int ScrNum, int CheckTime = K_45_WAIT);	
																// �Աݼ�ǥ������ȯ������
	int		fnAPP_CheckDepositPrintEdit();						// �Աݼ�ǥ�輭��������
	int		fnAPD_CheckDepositStore(int WaitFlg = FALSE);		// �Աݼ�ǥ����
	int		fnAPD_CheckDepositReject();							// �Աݼ�ǥ��ȯ
	int		fnAPD_CheckDepositPrint(int PrintDevice, int WaitFlg = FALSE);			
																// �Աݼ�ǥ��������
	int		fnAPD_CashDepositPrint(int WaitFlg = FALSE);		// ���ݰ����������

	int		fnAPD_MaterialOutRetryAfterError(int nDevice);		// ��ü��������ֽù�����õ�	V02-06-02-#05 21405��ǥ�����

	int		fnAPD_CardReadSerial();								// ī�忬�Ӱŷ�ó��
	int		IsBCCRight(BYTE* pbyBuf, DWORD dwBufSize);			// ����ī�� ���� (MS ����)
	BYTE	fnAuthGetDigit(CString strTemp);
	BYTE	fnAuthGetAccumlate(int nPreSum, CString strTemp);
	BYTE	fnAuthGetAccumlate(CString strTemp);
/////////////////////////////////////////////////////////////////////////////
//	���� IC �Լ� 2004.02.03
/////////////////////////////////////////////////////////////////////////////
	int		fnAPD_FICCardReadCheck();							// ����ICī���б�&üũ		2004.02.03
	int		fnAPD_FICDistinctCardType(int nReadDF = 0);			// ICī�� ����ó��(�����/NATIVE/��������) : IC�����ó������
	int		fnAPD_FICBasicInquiry();							// ����IC ���� ��ȸ(��� �ŷ��� ����)
	int		fnAPD_FICMyBiProc();								// MyBi ���� IC �ŷ�

	int		fnAPD_FICGetEncipher();								// ����IC ��й�ȣ��ȣȭó��2004.02.03	
																// ī�峭��(32) + ��ȣȭ��й�ȣ(32)
	int		fnAPD_FICGetHostTranData();							// ����IC �۽ŵ���Ÿ���	2004.02.03
																// ��������(32) + ī�峭��(32) + ��ȣȭ��й�ȣ(32)
	int		fnAPD_PbSpeedUpCheck();									// ���� ���ڵ� �б� �ӵ� ����
	int		fnAPD_PBBarCodeRead();									//#N0245
/////////////////////////////////////////////////////////////////////////////
//	EMV �Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_EMVInitProc();								// EMV ���� Buffer�� �ʱ�ȭ�ϴ� �Լ�(��� Start�� 1ȸ ȣ��)
	int		fnAPP_EMVOpenSetProc(int ConfigCode =1);			// ������(�Ǵ� ������ Configuratin Code Setting��) EMV ������ ����
	int		fnAPP_EMVReadProc();								// EMV Application Read �Լ�
	int		fnAPP_EMVAfterOnlineTranProc();						// EMV �¶��� �� �ŷ� ó�� �Լ�
	int		fnAPP_EMVReversalProc();							// EMV��Ұŷ� ó���Լ�
	int		fnAPD_EMVJaTaCheck();								// ��Ÿ�౸�� �ӽ��Լ�

	int		fnAPD_EMVTerminalConstant();						// #N0268 FB�� ���� EMV TerminalConstant ����
	
	int		fnAPD_EMVContinueTrans();							// ��ȸ �� �ŷ� ���� ���� �ŷ��� ������ ���� ���� ���ۼ�
	int		fnAPD_EMVSelectProc();								// EMV Application�� Ȯ���ϴ� �Լ�
	int		fnAPD_EMVBrandChoice();								// EMV Application �� �ŷ��� Brand�� �����ϴ� �Լ�
//D	int		fnAPD_EMVJaTaCheck();								// ��Ÿ�౸��
	int		fnAPD_EMVSelectBrand();								// �ŷ��� EMV Application�� ������ �� �� ���� ������ �޴� �Լ�
	int		fnAPD_EMVSendMakeProc();							// EMV�ŷ��� ���� �¶��� �۽� ��ó�� �Լ�
	int		fnAPD_EMVAfterRecvProc();							// �¶��ΰŷ� ��ó�� �Լ�
	int		fnAPD_EMVResultProc();								// EMV�ŷ� ����� ��ȸ�ϴ� �Լ�(����&�������)
	CString fnAPP_GetCaptureData();								// U8100 AP ���泻�� #01 - 3Camera, Preview����, �ؽ�Ʈ���	

/////////////////////////////////////////////////////////////////////////////
//	EMV LIBRARY
//  V09-33-00-#01 ICCM ��� ���� 
/////////////////////////////////////////////////////////////////////////////
	CString	fnAPDLib_WCStoMBS(LPBYTE pAIDName, int nLen);		// @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK
	CString fnAPDLib_MBStoWCS(LPCSTR pAIDName);					// @ADD:[EMVKERNEL5_001] 2013.06.11 SHPARK
		
	int		fnAPPLib_EMVReadProc();								// EMV Application Read �Լ�
	int 	fnAPDLib_EMVUserAIDLoad(int nAIDIndex, BYTE btUserConfirm, int nAIDNum, LPCTSTR strAID);	// @ADD:[EMVKERNEL5_001] 2013.06.12 SHPARK - AID DESC ��� ����
	int		fnAPDLib_EMVSelectAID();							// [#156] US Justin 2012.05.04 Implement EMV Transaction
	int		fnAPDLib_EMVUserAIDSelection(int nAIDNum, LPCTSTR strAID);		// [#156] US Justin 2012.06.06 Implement EMV Transaction
	int		fnAPDLib_EMVSendMakeProc(int nEmvType = 1);						// Default is TRANSACTION
	int		fnAPDLib_OnlineProcessing(LPSTR lpszHostData, int nHostResp, BOOL HostRecvFlag);		// �¶��� ���� ��û�� ���� ȣ��Ʈ ������ ī��� ����

	int		fnAPD_PBMPrintTest();								// ���������Ʈ�׽�Ʈ	#N0211

	// #0164 Hi-Pass���˽��������� #N0266 
/////////////////////////////////////////////////////////////////////////////
//  �����н� �Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPD_HPSVerifyTranBit();							// �����н�ī�� �ŷ�������Ʈ üũ
	int		fnAPD_HPSReadBalance(int nFlag = FALSE);			// �����н�ī�� �ܾ���ȸ		(���˽�)
	int		fnAPD_HPSPrepareLoad(char *pAmount);				// �����н�ī�� ����1�ܰ�		(���˽�)
	int		fnAPD_HPSLoad(BYTE *pRN, BYTE *pSign2);				// �����н�ī�� ����2�ܰ�		(���˽�)
	int		fnAPD_HPSComplete();								// �����н�ī�� �����Ϸ�(���)	(���˽�)
// ����������������������������������������������������������������������������

//////////////////////////////////////////////////////////////////////////
	
/////////////////////////////////////////////////////////////////////////////
#endif
