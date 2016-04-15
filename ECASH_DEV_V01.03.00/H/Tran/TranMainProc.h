/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranMainProc_H_INCLUDED_)
#define _TranMainProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Main Variable
/////////////////////////////////////////////////////////////////////////////
	int		MainTranDoingFlag;			// Main Transaction Doing Flag
	int		MainTranDoingCount;			// Main Transaction Doing Count

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	�����Լ�(TranMain.cpp)
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranVirtualMainProc();						// �������ó��
	int		fnAPP_TranVirtualStatusProc();						// �������ó��
	int		fnAPP_TranVirtualTranReadyProc();					// �������ó��
	int		fnAPP_TranMainProc();								// ����ó��
	int		fnAPP_TranReadyProc();								// �غ��ó��
	int		fnAPP_TranStatusProc();								// ����/���ó��ó��
	int		fnAPP_TranStatusOpenProc();							// ����/���ó��ó��
	int		fnAPP_TranOpenProc();								// ����ó��
	int		fnAPP_TranTranProc();								// �ŷ�ó��
	int		fnAPP_TranTotalProc();								// �հ�ó��
	int		fnAPP_TranSubTotalProc(int SprPrintFlag = TRUE);	// �Ұ�ó��
	int		fnAPP_TranCloseProc();								// ����ó��
	int		fnAPP_TranCloseInqProc();							// ������ȸó��
	int		fnAPP_TranCheckProc();								// ��ݼ�ǥ���ó����
	int		fnAPP_TranCashRegProc();							// ���ݵ��ó��
	int		fnAPP_TranAliveProc();								// ��������ó��(����ó��)
	int		fnAPP_TranMsFngConfirm();							// �����ν�Ȯ��
	int		fnAPP_TranSetCashProc(int nClearFlag = FALSE);	// ���۱ݾ׼���

	int		fnAPP_TranDesProc();								// DES TMK/TPK

/////////////////////////////////////////////////////////////////////////////
//	�����Լ�(TranMain.cpp)
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranInitProc();								// Initialize Procedure
	int		fnAPP_TranMenuProc();								// Menu Procedure
	int		fnAPP_TranGetTranProc();							// Get Transaciton Procedure
	int		fnAPP_TranGetMenuDoingProc();						// Get Menu Doing Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Withdraw Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranWithInitProc();							// Withdraw Initialize Procedure
	int		fnAPP_TranWithInputProc();							// Withdraw Input Procedure
	int		fnAPP_TranWithOutputProc();							// Withdraw Output Procedure
	int		fnAPP_TranWithCheckDeviceActionProc();				// Withdraw Check Device Action Procedure
	int		fnAPP_TranWithCheckMenuContinueProc();				// Withdraw Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Inquiry Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranInqInitProc();							// Inquiry Initialize Procedure
	int		fnAPP_TranInqInputProc();							// Inquiry Input Procedure
	int		fnAPP_TranInqOutputProc();							// Inquiry Output Procedure
	int		fnAPP_TranInqCheckDeviceActionProc();				// Inquiry Check Device Action Procedure
	int		fnAPP_TranInqCheckMenuContinueProc();				// Inquiry Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Transfer Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranTransInitProc();							// Transfer Initialize Procedure
	int		fnAPP_TranTransInputProc();							// Transfer Input Procedure
	int		fnAPP_TranTransOutputProc();						// Transfer Output Procedure
	int		fnAPP_TranTransCheckDeviceActionProc();				// Transfer Check Device Action Procedure
	int		fnAPP_TranTransCheckMenuContinueProc();				// Transfer Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Deposit Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranDepInitProc();							// Deposit Initialize Procedure
	int		fnAPP_TranDepInputProc();							// Deposit Input Procedure
	int		fnAPP_TranDepOutputProc();							// Deposit Output Procedure
	int		fnAPP_TranDepCheckDeviceActionProc();				// Deposit Check Device Action Procedure
	int		fnAPP_TranDepCheckMenuContinueProc();				// Deposit Check Menu Continue Procedure
	int		fnAPP_TranDepCardMuAllinceProc();							// Deposit Output Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Passbook Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranPbInitProc();								// Passbook Initialize Procedure
	int		fnAPP_TranPbInputProc();							// Passbook Input Procedure
	int		fnAPP_TranPbOutputProc();							// Passbook Output Procedure
	int		fnAPP_TranPbCheckDeviceActionProc();				// Passbook Check Device Action Procedure
	int		fnAPP_TranPbCheckMenuContinueProc();				// Passbook Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Ic Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranIcInitProc();								// Ic Initialize Procedure
	int		fnAPP_TranIcInputProc();							// Ic Input Procedure
	int		fnAPP_TranIcOutputProc();							// Ic Output Procedure
	int		fnAPP_TranIcCheckDeviceActionProc();				// Ic Check Device Action Procedure
	int		fnAPP_TranIcCheckMenuContinueProc();				// Ic Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction RF Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranRfInitProc();								// RF Initialize Procedure
	int		fnAPP_TranRfInputProc();							// RF Input Procedure
	int		fnAPP_TranRfOutputProc();							// RF Output Procedure
	int		fnAPP_TranRfCheckDeviceActionProc();				// RF Check Device Action Procedure
	int		fnAPP_TranRfCheckMenuContinueProc();				// RF Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
//	Transaction Clerk Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_TranClerkInitProc();							// RF Initialize Procedure
	int		fnAPP_TranClerkInputProc();							// RF Input Procedure
	int		fnAPP_TranClerkOutputProc();						// RF Output Procedure
	int		fnAPP_TranClerkCheckDeviceActionProc();				// RF Check Device Action Procedure
	int		fnAPP_TranClerkCheckMenuContinueProc();				// RF Check Menu Continue Procedure

/////////////////////////////////////////////////////////////////////////////
#endif
