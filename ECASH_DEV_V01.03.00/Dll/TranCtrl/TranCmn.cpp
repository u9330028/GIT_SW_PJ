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
//  ��������
/////////////////////////////////////////////////////////////////////////////
	memset(&OpenInfo, 0, sizeof(OpenInfo));

	memcpy(OpenInfo.WithCashCount,	"100", 3);
	memcpy(OpenInfo.WithCheckCount,	"010", 3);
	memcpy(OpenInfo.DepCashCount,	"100", 3);
	memcpy(OpenInfo.DepCheckCount,	"010", 3);
	Strcpy(OpenInfo.TransMoney,  "006000000");
	Strcpy(OpenInfo.SvcTransMoney,  "099990000");
	sprintf((char *)OpenInfo.IdleMessage, "%8.8s173000", GetDate().GetBuffer(0));

	memcpy(MenuTbl, MenuTblConst, sizeof(MenuTbl));				// �ŷ���ư�ʱ�ȭ
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 1�ܰ�����)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 2�ܰ�����)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 3�ܰ�����)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 4�ܰ�����)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 5�ܰ�����)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 6�ܰ�����)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 7�ܰ�����)
	
/////////////////////////////////////////////////////////////////////////////
//	Nms����
/////////////////////////////////////////////////////////////////////////////
	ErrorSaveFlg = FALSE;										// �������SaveFlg
	ErrorInDirectFlg = FALSE;									// �������(��,�ڶ�,�ۼ��ŵ�)
	ErrorAutoOffSave = 0;										// �����ڵ�����

	RecycleBoxStatus = CST_NORMAL;								// ���ݻ���
	CheckCSTStatus = CST_NORMAL;								// ��ݼ�ǥ����
	CheckCSTStatusSensor = CST_NORMAL;							// ��ݼ�ǥ����(�ǹ�)
	CheckCSTStatusReg = CST_NORMAL;								// ��ݼ�ǥ����(���)
	CassetteStatus = CST_NORMAL;								// �Աݼ�ǥ����
	SlipStatus = SLIP_NORMAL;									// ��ǥ����
	JnlStatus = JNL_NORMAL;										// ���λ���
	RejectCSTStatus = CST_NORMAL;								// ����ȸ�����ǻ���
	A4pStatus = CST_NORMAL;										// A4�����ͻ���
	memset(&NmsDataSendData, 0, sizeof(NmsDataSendData));		// ���/������������
	memset(&NmsSaveDataSendData, 0, sizeof(NmsSaveDataSendData));
																// nms SendData Save
	
	NmsSendLength = 0;											// Nms�۽�Length
	NmsRecvLength = 0;											// Nms����Length
	memset(NmsRecvLengthTbl, 0, sizeof(NmsRecvLengthTbl));		// Nms����LengthTbl
	
	NmsRecvInTbl = 0;											// Nms����In
	NmsRecvOutTbl = 0;											// Nms����Out
	memset(NmsSendBuffer, 0, sizeof(NmsSendBuffer));			// Nms�۽�Buffer
	memset(NmsRecvBuffer, 0, sizeof(NmsRecvBuffer));			// Nms����Buffer
	memset(NmsRecvBufferTbl, 0, sizeof(NmsRecvBufferTbl));		// Nms����BufferTbl

	NmsErrorSendFlg = FALSE;									// ���Send����

	HotKeyFlag = FALSE;											// U8100 AP ���泻�� #10 - HotKey(����ư) �߰�

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
//	�ʱ�ȭ�Լ�
/////////////////////////////////////////////////////////////////////////////
// �����ʱ�ȭ
int CTranCmn::fnAPP_InitializeDS()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeDS()");

/////////////////////////////////////////////////////////////////////////////
//	ó�����
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->TranResult = FALSE;								// Host���
/////////////////////////////////////////////////////////////////////////////
//	�ŷ����º���
/////////////////////////////////////////////////////////////////////////////
	TranAvail = 0;												// ��ü�ŷ�����
	WithAvail = 0;												// ��ݰŷ�����
	DepAvail = 0;												// �Աݰŷ�����
	PbmAvail = 0;												// ����ŷ�����
	A4pAvail = 0;												// A4p�ŷ�����
	
/////////////////////////////////////////////////////////////////////////////
//	�޴�����
/////////////////////////////////////////////////////////////////////////////
	MenuIndex = 0;												// �޴�Index
	MenuIndexSub = 0;											// �޴�IndexSub
	MenuIndexNext = 0;											// �޴�IndexNext
	MenuAvail = MENU_NULL;										// �޴����ɻ���
	MenuMuTran = MENU_NULL;										// �޴�����ü�ŷ�����
	MenuBtnCnt = 0;												// �޴����ð����ѹ�ư��
	MenuBtnIndex = 0;											// �޴��ڵ����ð�����������ư��
	MenuOutGoingMode = 0;										// CREDIT(WITH):1,SAVING(WITH):2,CHECKING(WITH):3,CREDIT(INQ):4,SAVING(INQ):5,CHECKING(INQ):6
	MenuFingerMode = 0;											// FINGER:1
	MenuIrdaMode = 0;											// IRDA:1
	MenuKCashMode = 0;

	BrandSelectIndex = 0;										// ���Ӱŷ��� ���õ� Brand
	EMVReadFlg = FALSE;											// EMV READ FLAG
	CardBankFlag = 0;
	CardServiceFlag = 0;
	ExtraServiceFlag = 0;
	m_EMVCancelFlg = FALSE;

	TranProc = TRAN_NULL;										// �ŷ�ó��
	TranCode = 0;												// �ŷ��ڵ�
	TranCode2 = 0;												// �ŷ��ڵ�2

	TranProcSave = TRAN_NULL;									// Tran�ŷ�ó������
	TranCodeSave = 0;											// TranCode����
	TranCode2Save = 0;											// TranCode2����
	TranEncCode = 0;
/////////////////////////////////////////////////////////////////////////////
//	�ѵ��ݾ׺���
/////////////////////////////////////////////////////////////////////////////
	MoneyMaxOut = 0;											// ���Ⱑ���ѵ��ݾ�(����)
	MoneyMaxOutCnt = 0;											// ���Ⱑ�ɸż�
	CashMaxOutCnt = 0;											// ���ݹ��Ⱑ�ɸż�
	CheckMaxOutCnt = 0;											// ��ǥ���Ⱑ�ɸż�
	CheckLowMaxOutCnt = 0;										// ��ǥ�����ּҸż�
	CheckHighMaxOutCnt = 0;										// ��ǥ�����ִ�ż�

	TMoneyMaxOut = 0;											// ��ü�����ѵ��ݾ�(��)

	MoneyMaxIn = 0;												// �Աݰ����ѵ��ݾ�(����)
	CashMaxIn = 0;												// �����Աݰ��ɱݾ�
	CheckMaxInCnt = 0;											// ��ǥ�Աݰ��ɸż�

/////////////////////////////////////////////////////////////////////////////
//	�Էº���
/////////////////////////////////////////////////////////////////////////////
	memset(&Accept, 0, sizeof(Accept));							// �Էº���

	memset(Accept.PbConfirmNum, 0x20, sizeof(Accept.PbConfirmNum)); // ����ŷ����ι�ȣ
	Accept.PbConfirmNumSize = 0;								// ����ŷ����ι�ȣ����	

	Accept.MuAccountNumSize = 0;								// ����ü���¹�ȣ ����
	memset(Accept.MuAccountNum, ' ', sizeof(Accept.MuAccountNum));	
																// ����ü���¹�ȣ
	memset(Accept.CardReadTime, ' ', sizeof(Accept.CardReadTime)); //ī�� ���� �ð�	

	memset(Accept.PassWord, ' ', sizeof(Accept.PassWord));		// ��й�ȣ
	memset(Accept.PbPassWord, ' ', sizeof(Accept.PbPassWord));	// �����й�ȣ
	memset(Accept.MuPassWord, ' ', sizeof(Accept.MuPassWord));	// ����ü��й�ȣ
	memset(Accept.KCashPassWord, 0, sizeof(Accept.KCashPassWord));
	memset(Accept.SlockPassWord, 0, sizeof(Accept.SlockPassWord));	
	memset(Accept.UserId, ' ', sizeof(Accept.UserId));	
																// K-CASH��й�ȣ

	memset(Accept.DesPassWord, 0, sizeof(Accept.DesPassWord));	// Des��й�ȣ		// DES OutGoing�ŷ� 2004.05.31
	memset(Accept.DesEncryptPin, 0, sizeof(Accept.DesEncryptPin));	
																// Des Encrypt PIN	// DES OutGoing�ŷ� 2004.05.31
/////////////////////////////////////////////////////////////////////////////
//	�ݾװ��ú���
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.Money, '0', sizeof(Accept.Money));			// �ݾ�(����), �ݾ�(��õ��)
	memset(Accept.CashMoney, '0', sizeof(Accept.CashMoney));	// �ݾ�(������)/�����Ա�
	memset(Accept.CheckMoney, '0', sizeof(Accept.CheckMoney));	// �ݾ�(��ǥ��)/��ǥ�Ա�
	memset(Accept.Cash50T_Money, '0', sizeof(Accept.Cash50T_Money));	
	memset(Accept.Cash10T_Money, '0', sizeof(Accept.Cash10T_Money));	
	
	memset(Accept.MotherCheckMoneyM, '0', sizeof(Accept.MotherCheckMoneyM));	
																// �ݾ�(��ǥ��)/��ǥ�Ա������������ݾ�
	memset(Accept.MotherCheckMoneyO, '0', sizeof(Accept.MotherCheckMoneyO));	
																// �ݾ�(��ǥ��)/��ǥ�Ա�������Ÿ���ݾ�
	memset(Accept.OtherCheckMoney, '0', sizeof(Accept.OtherCheckMoney));	
																// �ݾ�(��ǥ��)/��ǥ�Ա���Ÿ��ݾ�
	Accept.MoneyInKind = TRAN_DEP_NOT;							// �Աݱ���

	memset(Accept.RemainRemMoney, '0', sizeof(Accept.RemainRemMoney));		
																// ���׼۱ݾ�
	memset(Accept.BalanceRemMoney, '0', sizeof(Accept.BalanceRemMoney));	
																// �����Աݾ�
/////////////////////////////////////////////////////////////////////////////

	memset(Accept.BankNum, ' ', sizeof(Accept.BankNum));		// ����
	Accept.InAccountNumSize = 0;								// ������¹�ȣ����
	memset(Accept.InAccountNum, ' ', sizeof(Accept.InAccountNum));	
																// �����Աݰ��¹�ȣ����
	Accept.RemitAccountNumSize = 0;								// ������¹�ȣ����
	memset(Accept.RemitAccountNum, ' ', sizeof(Accept.RemitAccountNum));	
																// �����Աݰ��¹�ȣ
	
	memset(Accept.ChangePwd, '0', sizeof(Accept.ChangePwd));	// ��й�ȣ(����)
	memset(Accept.ChangePwdConfirm, '0', sizeof(Accept.ChangePwdConfirm));
																// ��й�ȣ(����Ȯ��)
	memset(Accept.ChangeICPwd, 0, sizeof(Accept.ChangeICPwd));
																// K-CASH��й�ȣ(����)
	memset(Accept.ChangeICPwdConfirm, 0, sizeof(Accept.ChangeICPwdConfirm));
																// K-CASH��й�ȣ(����Ȯ��)
	
	Accept.LoanAccountNumSize = 0;								// ������¹�ȣ ����
	memset(Accept.LoanAccountNum, ' ', sizeof(Accept.LoanAccountNum));	
																// ������¹�ȣ
	Accept.JiroTranCode = 0;									// ���μ��ΰŷ��ڵ�
	memset(Accept.JiroTranOrgCode, 0x00, sizeof(Accept.JiroTranOrgCode));		// ���γ��α���ڵ�
	memset(Accept.JiroNum, ' ', sizeof(Accept.JiroNum));		// ���ι�ȣ
	Accept.JiroElecNumSize = 0;									// ���ڳ��ι�ȣ����
	memset(Accept.JiroElecNum, '0', sizeof(Accept.JiroElecNum));// ���ڳ��ι�ȣ
	memset(Accept.JiroSelItem, '0', sizeof(Accept.JiroSelItem));// ���ΰŷ��� ���ó���
	memset(Accept.JiroTranDetails, ' ', sizeof(Accept.JiroTranDetails));// ���ΰŷ��� ���ų��γ���
	memset(Accept.JiroMoney, '0', sizeof(Accept.JiroMoney));	// �ݾ�(����), �ݾ�(��õ��)

	Accept.SerialCount = 0;										// ���Ӱŷ�ī��Ʈ
//	Accept.SerialTranFlag = FALSE;								// ���Ӱŷ�����(���,��ü)
	Accept.SlipChoiceFlag = FALSE;								// ��ǥ���ڿ���
	Accept.A4ChoiceFlag = FALSE;								// A4���������ڿ���

	memset(Accept.RegCheckNum, '0', sizeof(Accept.RegCheckNum));// ��ϼ�ǥ��ȣ
	memset(Accept.RegCheckCount, '0', sizeof(Accept.RegCheckCount));
																// ��ϼ�ǥ�ż�

	memset(Accept.JuminNum, '0', sizeof(Accept.JuminNum));		// �ֹι�ȣ
	memset(Accept.TelNum, '0', sizeof(Accept.TelNum));			// ��ȭ��ȣ
	Accept.TelNumSize = 0;										// ��ȭ��ȣ����

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.CMSCode, ' ', sizeof(Accept.CMSCode));		// CMS(����/������ȣ)
	memset(Accept.CMSOrgName, ' ', sizeof(Accept.CMSOrgName));	// CMS(�����)

	memset(Accept.AutoDate, 0, sizeof(Accept.AutoDate));		// �ڵ���ü��
	memset(Accept.AutoCycle, 0, sizeof(Accept.AutoCycle));		// ��ü�ֱ�

	memset(Accept.InquiryDate, ' ', sizeof(Accept.InquiryDate));// �����ȸ��
	memset(Accept.RemitName, ' ', sizeof(Accept.RemitName));	// �۱��θ�
	Accept.RemitNameFlag = FALSE;								// �۱����Է��÷���
	Accept.MuTranFlag = FALSE;									// ����-����ü�÷�ũ
	Accept.InQuiryCycle = '0';									// ��ȸ�Ⱓ����(����,����)
	Accept.MuBankTranBrandFlag = FALSE;									// ����-����ü�÷�ũ	
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.StocksCode, ' ', sizeof(Accept.SelStockIndex));	// �����ڵ�
	memset(Accept.StocksReqNum, ' ', sizeof(Accept.StocksReqNum));	// ��û����
	memset(Accept.ByOnMargin, ' ', sizeof(Accept.ByOnMargin));		// ���ű�
	memset(Accept.LoanMoney, ' ', sizeof(Accept.LoanMoney));		// �����
	memset(Accept.RefundAccountNum, ' ', sizeof(Accept.RefundAccountNum));	// ȯ�Ұ���
	memset(Accept.NextInqCondition, ' ', sizeof(Accept.NextInqCondition));	// ������ȸ����
	memset(Accept.StocksDate, ' ', sizeof(Accept.StocksDate));					// û����
	memset(Accept.RefundAccountName, ' ', sizeof(Accept.RefundAccountName));	// ȯ�Ұ��¸�
	memset(Accept.RefundDate, ' ', sizeof(Accept.RefundDate));					// ȯ������
	memset(Accept.StocksListDate, ' ', sizeof(Accept.StocksListDate));			// ��������
	memset(Accept.InAccountName, ' ', sizeof(Accept.InAccountName));			// �Աݰ��¸�
	memset(Accept.AccountNum, ' ', sizeof(Accept.AccountNum));					// ���¹�ȣ
	memset(Accept.HostReqNum, ' ', sizeof(Accept.HostReqNum));					// HOST���ŷ���ȣ
	memset(Accept.MuConfirmNum, '0', sizeof(Accept.MuConfirmNum));				// �ؿ�ī�� ����
	memset(m_yymmddhhmmss,	0, sizeof(m_yymmddhhmmss));			// CARD Read Time

//	�ۼ��ź���
/////////////////////////////////////////////////////////////////////////////
	AddSerialFlg = FALSE;										// �Ϸù�ȣ����Flg
	TranSend = 0;												// N���۽ſ���

	SendLength = 0;												// �۽�Length
	SaveSendLength = 0;											// ����۽�Length
	RecvLength = 0;												// ����Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// �۽�Buffer
	memset(RecvBuffer, 0, sizeof(RecvBuffer));					// ����Buffer
	memset(SaveSendBuffer, 0, sizeof(SaveSendBuffer));			// ����۽�Buffer

	memset(&PSPRecvData, 0, sizeof(PSPRecvData));				// DATA��/������Ʈ(�ŷ�)
	memset(&PPPRecvData, 0, sizeof(PPPRecvData));				// DATA��/���������Ʈ(�ŷ�)
	memset(&PPPRecvDataNon, 0, sizeof(PPPRecvDataNon));				// DATA��/���������Ʈ(�ŷ�) - Non
	memset(&PPPRecvDataKb, 0, sizeof(PPPRecvDataKb));				// DATA��/���������Ʈ(�ŷ�) - KB #N0227
	memset(&PPWRecvData, 0, sizeof(PPWRecvData));				// DATA��/����M/S WRITE(�ŷ�)
	memset(&PCWRecvData, 0, sizeof(PCWRecvData));				// DATA��/ī��M/S WRITE(�ŷ�)
	memset(&PMERecvData, 0, sizeof(PMERecvData));				// DATA��/�޼�������ǥ��(�ŷ�������)
	memset(&PMDRecvData, 0, sizeof(PMDRecvData));				// DATA��/�޼���ǥ��(�ŷ�)
	
/////////////////////////////////////////////////////////////////////////////
//	�۽Ź���
/////////////////////////////////////////////////////////////////////////////
	
	memset(&m_TXECASH,	0, sizeof(TXRXECASH));					// �۽�
	memset(&m_RXECASH,	0, sizeof(TXRXECASH));					// ����
	memset(&m_RD,	    0, sizeof(RXDISPLAYDATA));				
	memset(&m_RecvEmvrcvData,	    0, sizeof(RECVEMVRCVDATA));	
	fnAPP_ClearArrDataField();
	m_pNetWork->ClearRemoteData();
	RegSetStr(_REGKEY_REMOTECTRL, "BID_RECV_RESULT", "NORMAL");	
/////////////////////////////////////////////////////////////////////////////
//	���Ź���
/////////////////////////////////////////////////////////////////////////////
	memset(&RecvComm,	0, sizeof(RECVCOMM));					// �������
	memset(HostReserved, NULL, sizeof(HostReserved));			// HOST����
	memset(SaveJiroNum, ' ', sizeof(SaveJiroNum));				// ���ι�ȣ����

/////////////////////////////////////////////////////////////////////////////
//	������ó������
/////////////////////////////////////////////////////////////////////////////
	ChoiceTran = TRAN_NULL;										// ��ȸ�ļ��õȰŷ�
	TranResultNGCode = 0;										// Tran�ŷ����NG�ڵ�
	PbNextTranFlag = FALSE;										// ���� �����ŷ� �뺸�÷���
	DepNextTranFlag = FALSE;									// ���Ӱŷ�_�Ա�_Flag
	PbNextPageFlag = FALSE;										// ���� ���������� �뺸�÷���
	InqNextTranFlag = FALSE;									// ��ȸ ������� �뺸 �÷���
	RetractFlag = FALSE;										// ��üȸ������Flag
	KCashHostICFlag = FALSE;									// K-CASH Host IC Flag
	UpdateKeyParam = FALSE;										// �Ķ���Ͱ��ſ���
	ContinueTranCount = 0;										// ���Ӱŷ�ī��Ʈ
	InqNextTranCount = 0;										// ��ȸȽ��
	DepNextTranCount = 0;

	SeedErrorCode = 0;											// �Ϻ�ȭ����
	PbPrintFlag	= FALSE;										// ������忩��
	GhostAccountFlag = FALSE;									// ������¿���
	PbPrintAfterMsgFlag = FALSE;
/////////////////////////////////////////////////////////////////////////////
//	��ü����
/////////////////////////////////////////////////////////////////////////////
	DevEnDisable = 0;											// ī������En/Disable
	ICCardTran = MENU_NULL;
	MSCardTran = MENU_NULL;
	CardTran = MENU_NULL;										// Card�ŷ����º���
	PbTran = MENU_NULL;											// Pb�ŷ����º���
	PbKind = 0;													// ������������
	CardBank = 0;												// ����ī������
	PbBank = 0;													// ���� Flag
	CardService = 0;											// �ſ�ī������
	CardNew = 0;												// �ű�ī������
	BranchKind = 0;												// ��Ÿ����
	MenuFICMode	= MENU_NULL;									// ����ICī��ŷ�����
	MenuEMVMode = MENU_NULL;									// EMVī��ŷ�����
	MenuEPBMode = MENU_NULL;									// ��������ŷ�����
	MenuRFDMode = MENU_NULL;									// RF�ŷ����º���
	FICMyBiFlg = FALSE;											// ����ICī�� MyBi����
	FallBackMenuFICMode = FALSE;								// ����IC ī��FALLBACK���º���
	FallBackEMVMode = FALSE;									// EMV IC ī��ŷ����º���
	EMVReadFlg = FALSE;											// EMV READ FLAG
	m_EMVCancelFlg = FALSE;
	FallBackCnt = 0x20;											// Default : Space
	BrandSelectIndex = 0;										// ���Ӱŷ��� ���õ� Brand
	KCashPingChangFlag = FALSE;									// PIN���/���溯��

	CardOnEMV = FALSE;											// EMV���� �ſ�ī��
	EMVAuthError = FALSE;										// EMV��������

	m_pDevCmn->FICAppletType = 0;	

	memset(&CardData, 0, sizeof(CardData));						// ī���ڷ� 
	pCardData2 = (MCISO2*)CardData.ISO2Buff;					// 2TR Pointer
	pCardData3 = (MCISO3*)CardData.ISO3Buff;					// 3TR Pointer
	memset(&PbMsData, 0, sizeof(PbMsData));						// �����ڷ�
	memset(&PbBarData, 0, sizeof(PbBarData));					// ���� Bar Info
	pPbMsData = (PBMS*)PbMsData.sBuf;							// Pb Pointer
	memset(&FingerData, 0, sizeof(FingerData));					// �����ڷ�
	memset(&BarData, 0, sizeof(BarData));						// BarCode�ڷ�

	CardEmbossScanFlg = FALSE;									// ī�忥������ĵ����
	CardImprintFlg = FALSE;										// ī��������Ʈ����
	CardPowerOnFlg = FALSE;										// ī����������(����ȭ��)
	PbDataCnt = 0;												// Pb��±���
	memset(PbData, 0, sizeof(PbData));							// Pb��¿���

	EjrIndexFlg = FALSE;										// ������������
	memset(&EjrData, ' ', sizeof(EjrData));						// ���������ڷ�
	memset(&EjrAmountData, 0, sizeof(EjrAmountData));			// ��������ݾװ����ڷ�

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���
	SlipPrintFlg = FALSE;										// ��ǥ��������
	JnlPrintFlg = FALSE;										// ������������
	
	MaterialOutFlg = 0;											// ��ü��������
	MaterialOutCheckFlg = 0;									// ��ü�����ļ�������
	MoneyOutFlg = 0;											// ���ݼ�ǥ��������
	MoneyOutCheckFlg = 0;										// ���ݼ�ǥ�����ļ�������

	SprOutRetryFlg = FALSE;										// ��ǥ������õ��÷���

	Accept.FICSelectAccountIndex = 0;							// ����IC ���ð����ε���

	A4PrintCnt = 0;												// ����� A4Print Line Count(PMERecvData.y��)
/////////////////////////////////////////////////////////////////////////////
//	������ú���
/////////////////////////////////////////////////////////////////////////////
	CashCountFlg = FALSE;										// ������ݰ������
	CheckCountFlg = FALSE;										// ��ݼ�ǥ�������

	CashInCountFlg = FALSE;										// �Ա����ݰ������
	CashRejectFlg = FALSE;										// �Ա����ݹ�ȯ����
	CashRejectAFlg = FALSE;										// �Ա������ʰ���ȯ����
	CheckInCountFlg = FALSE;									// �Աݼ�ǥ�������
	CheckRejectFlg = FALSE;										// �Աݼ�ǥ��ȯ����
	CheckRejectAFlg = FALSE;									// �Աݼ�ǥ������ȯ����
	CheckRejectKind = 0;										// �Աݼ�ǥ��ȯ����

	memset(&RejectData, 0, sizeof(RejectData));					// ȸ������
	memset(&CashData, 0, sizeof(CashData));						// �Ա����ݳ���
	memset(CheckInfoData, '0', sizeof(CheckInfoData));			// �Աݼ�ǥ����
	memset(Accept.PbPageInfo, 0, sizeof(Accept.PbPageInfo));	// ���� Bar Info //#0039
	CheckInfoStats = NORMAL;									// ��ǥ�Աݻ���
	CheckInfoCnt = 0;											// ��ǥ�Աݸż�
	CheckInfoAmt = 0;											// ��ǥ�Աݱݾ�
	memset(CheckPrintData, NULL, sizeof(CheckPrintData));		// ��ǥ�輭��������
	CheckInPrintFlg = FALSE;									// ��ǥ�Ա�������������
	SumClearFlag = FALSE;
/////////////////////////////////////////////////////////////////////////////
//	���ắ��
/////////////////////////////////////////////////////////////////////////////
	CancelProcNum = 0;											// ���ó��N�����ຯ��
	CancelProcCount = ' ';										// ���ó���������ȣ 
	CancelDepRejectFlg = FALSE;									// �Աݹ�ȯFlg
	CancelDepRejectProcFlg = FALSE;								// �Աݹ�ȯó��Flg
	CancelCheckClearFlg = FALSE;								// ��ǥ�����ʱ�ȭFlg
	CancelPrintFlg = FALSE;										// �������Flg
	CancelDepPrintFlg = TRUE;									// ����Ա�����Flg(�ʱ�:���๫)
	CancelWithSendFlg = FALSE;									// ����������Flg(�ʱ�:���๫) v01-00-00-#301
	CancelWithProcFlg = FALSE;									// ������ó��Flg
	CancelDepSendFlg = FALSE;									// ����Ա�����Flg(�ʱ�:���๫)
	CancelDepProcFlg = FALSE;									// ����Ա�ó��Flg(�ʱ�:���๫)
	CancelKCashSendFlg = TRUE;									// K-CASH����������Flg(�ʱ�:���๫)
	CancelKCashProcFlg = FALSE;									// K-CASH������ó��Flg


	CancelHIPassLoadFlg = FALSE;								// �����н� �ŷ����н� Complete ó��Flag	#N0266

	CancelHipassPrintFlg = TRUE;								// �����н� ��� FLG

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
	m_pDevCmn->Declined = FALSE;								// #NICCM01 ERROR ������ �� ó���� DECLINE ó���߰�	
	SourceAccount = S_OTHER;							    	// #NICCM01
	
	m_pDevCmn->nEmvStep = 0;
	m_pDevCmn->nEmvStepCode = 0;

	m_pDevCmn->m_nAtmsCommand = ATMS_CMD_IDLE;	
	m_pDevCmn->fnEMVLib_Initial();								// #NICCM01
	
/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeDS():return");
	return T_OK;
}

// ���Ӱŷ������ʱ�ȭ
int CTranCmn::fnAPP_InitializeSerialTranDS()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeSerialTranDS()");

/////////////////////////////////////////////////////////////////////////////
//	�ݾװ��ú���
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.Money, '0', sizeof(Accept.Money));			// �ݾ�(����), �ݾ�(��õ��)
	memset(Accept.CashMoney, '0', sizeof(Accept.CashMoney));	// �ݾ�(������)/�����Ա�
	memset(Accept.CheckMoney, '0', sizeof(Accept.CheckMoney));	// �ݾ�(��ǥ��)/��ǥ�Ա�
	memset(Accept.MotherCheckMoneyM, '0', sizeof(Accept.MotherCheckMoneyM));
																// �ݾ�(��ǥ��)/��ǥ�Ա������������ݾ�
	memset(Accept.MotherCheckMoneyO, '0', sizeof(Accept.MotherCheckMoneyO));
																// �ݾ�(��ǥ��)/��ǥ�Ա�������Ÿ���ݾ�
	memset(Accept.OtherCheckMoney, '0', sizeof(Accept.OtherCheckMoney));	
																// �ݾ�(��ǥ��)/��ǥ�Ա���Ÿ��ݾ�
	Accept.MoneyInKind = TRAN_DEP_NOT;							// �Աݱ���

	memset(Accept.RemainRemMoney, '0', sizeof(Accept.RemainRemMoney));		
																// ���׼۱ݾ�
	memset(Accept.BalanceRemMoney, '0', sizeof(Accept.BalanceRemMoney));	
																// �����Աݾ�
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PassWord, ' ', sizeof(Accept.PassWord));		// #N0223 ��ȸ�� �ŷ� �� ��й�ȣ �ʱ�ȭ
/////////////////////////////////////////////////////////////////////////////
//	������ú���
/////////////////////////////////////////////////////////////////////////////
	CashCountFlg = FALSE;										// ������ݰ������
	CheckCountFlg = FALSE;										// ��ݼ�ǥ�������

	CashInCountFlg = FALSE;										// �Ա����ݰ������
	CashRejectFlg = FALSE;										// �Ա����ݹ�ȯ����
	CashRejectAFlg = FALSE;										// �Ա������ʰ���ȯ����
	CheckInCountFlg = FALSE;									// �Աݼ�ǥ�������
	CheckRejectFlg = FALSE;										// �Աݼ�ǥ��ȯ����
	CheckRejectAFlg = FALSE;									// �Աݼ�ǥ������ȯ����
	CheckRejectKind = 0;										// �Աݼ�ǥ��ȯ����

	memset(&RejectData, 0, sizeof(RejectData));					// ȸ������
	memset(&CashData, 0, sizeof(CashData));						// �Ա����ݳ���
	memset(CheckInfoData, '0', sizeof(CheckInfoData));			// �Աݼ�ǥ����

	CheckInfoStats = NORMAL;									// ��ǥ�Աݻ���
	CheckInfoCnt = 0;											// ��ǥ�Աݸż�
	CheckInfoAmt = 0;											// ��ǥ�Աݱݾ�
	memset(CheckPrintData, NULL, sizeof(CheckPrintData));		// ��ǥ�輭��������
	CheckInPrintFlg = FALSE;									// ��ǥ�Ա�������������
	MenuDoingFlag = FALSE;										// Defualt FALSE
	MainTranDoingFlag = FALSE;									// ���Ӱŷ����� �ʱ�ȭ
	TranSend = 0;												// ���Ӱŷ��� 2TR�ŷ��б���ʱ�ȭ

	GhostAccountFlag = FALSE;									// ������¿���

	AddSerialFlg = FALSE;										// �Ϸù�ȣ����Flg

	CancelHipassPrintFlg = TRUE;								// �����н� ���� �ŷ� ��� #N0266
/////////////////////////////////////////////////////////////////////////////

	memset(m_pDevCmn->OrignalErrorCode, 0, sizeof(m_pDevCmn->OrignalErrorCode));
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeSerialTranDS():return");
	return T_OK;
}

