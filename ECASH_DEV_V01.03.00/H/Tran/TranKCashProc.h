/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranKCashProc_H_INCLUDED_)
#define _TranKCashProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	K-CASH�Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_KCashCheckPin();								// ����ȭ�� ��й�ȣ�˻�
	int		fnAPP_KCashReadPan();								// ����ȭ�� PAN�����б�
	int		fnAPP_KCashCheckMainAccount();						// ����ȭ�� ���������м�(PAN����)
	int		fnAPP_KCashReadBAmount();							// ����ȭ�� JAN�����б�
	int		fnAPP_KCashReadTrafficInfo();						// ����ȭ�� ���������б�
	int		fnAPP_KCashPrepareUnloadRcvIEP();					// ����ȭ�� ȯ�Һ�����ó��

	int		fnAPP_KCashVerifyTranBit();							// ����ȭ�� �ŷ�����BitȮ��
	int		fnAPP_KCashRemoveTranBit();							// ����ȭ�� �ŷ�����Bit����

	int		fnAPP_KCashLoad();									// K-CASH����
	int		fnAPP_KCashUnload();								// K-CASHȯ��
	int		fnAPP_KCashChangePin();								// ��й�ȣ����
	
	int		fnAPP_KCashInquiry();								// K-CASH�ܾ���ȸ
	int		fnAPP_KCashLoadUnloadInquiry();						// ����/ȯ�ҳ���
	int		fnAPP_KCashBuyListInquiry();						// ���ų�����ȸ

	int		fnAPP_KCashUpdate();								// �Ķ���Ͱ���

	int		fnAPP_KCashReadEID();								// ����ȭ���ȣ �б�

/////////////////////////////////////////////////////////////////////////////
// �Է� �����Լ�
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_KCashCheckChangePin();						// PIN����� ������PIN Check	2004.02.27

/////////////////////////////////////////////////////////////////////////////
#endif
