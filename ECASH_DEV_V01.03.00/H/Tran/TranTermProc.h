/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranTermProc_H_INCLUDED_)
#define _TranTermProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	���ắ��
/////////////////////////////////////////////////////////////////////////////
	int		CancelProcNum;				// ���ó��N�����ຯ��
	char	CancelProcCount;			// ���ó���������ȣ 
	int		CancelDepRejectFlg;			// �Աݹ�ȯFlg
	int		CancelDepRejectProcFlg;		// �Աݹ�ȯó��Flg
	int		CancelCheckClearFlg;		// ��ǥ�����ʱ�ȭFlg
	int		CancelPrintFlg;				// �������Flg
	int		CancelDepPrintFlg;			// ����Ա�����Flg(�ʱ�:���๫)
	int		CancelWithSendFlg;			// ����������Flg(�ʱ�:���๫)
	int		CancelWithProcFlg;			// ������ó��Flg
	int		CancelDepSendFlg;			// ����Ա�����Flg(�ʱ�:���๫)
	int		CancelDepProcFlg;			// ����Ա�ó��Flg
	int		CancelKCashSendFlg;			// K-CASH����������Flg(�ʱ�:���๫)
	int		CancelKCashProcFlg;			// K-CASH������ó��Flg

	int		CancelEMVFlg;				// �������FLG

	int		CancelHIPassLoadFlg;		// �����н� �ŷ����н� Complete ó��Flag	#N0266


	int		CancelHipassPrintFlg;			// ��������н�����Flg(�ʱ�:���๫) #N0266
 
/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	�����Լ�(TranCmn.cpp)
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_CancelDepRejectProc(int CancelCode, char *pCancelMsg = NULL, char *pCancelMsg2 = NULL, char *pCancelMsg3 = NULL, int CancelScrNum = 0);
																// ����Աݹ�ȯ�Լ�(�������ó��)
	int		fnAPP_CancelProc(int CancelCode, char *pCancelMsg = NULL, char *pCancelMsg2 = NULL, char *pCancelMsg3 = NULL, int CancelScrNum = 0);
																// ����Լ�
	int		fnAPP_CancelProc2(int CancelCode, char *pCancelMsg = NULL, char *pCancelMsg2 = NULL, char *pCancelMsg3 = NULL, int CancelScrNum = 0);
																// ����Լ�2
	int		fnAPP_CancelProc3(int CancelCode, char *pCancelMsg = NULL, char *pCancelMsg2 = NULL, char *pCancelMsg3 = NULL, int CancelScrNum = 0);
																// ����Լ�3
	int		fnAPP_CancelProc4(int CancelCode, char *pCancelMsg = NULL, char *pCancelMsg2 = NULL, char *pCancelMsg3 = NULL, int CancelScrNum = 0);
																// ����Լ�4
	int		fnAPP_TerminateProc(int TerminateCode);				// �����Լ�
	int		fnAPP_TerminateDummyProc(int TerminateCode);		// �����Լ�

/////////////////////////////////////////////////////////////////////////////
#endif
