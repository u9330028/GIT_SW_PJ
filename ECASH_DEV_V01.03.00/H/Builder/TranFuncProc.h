////////////////////////////////////////////////////////////////////////////////
// �� �� �� : TranFuncProc.h
// ��    �� : �ŷ�ȭ��� �Լ�����
// �� �� �� : SE����
// ���泻�� : 1. 2007-05-07  ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#if !defined(_TranFuncProc_H_INCLUDED_)
#define _TranFuncProc_H_INCLUDED_

//////////////////////////////////////////////////////////////////////////////
// Attributes
public:

//////////////////////////////////////////////////////////////////////////////
// Operations
public:

	int	fnAPPF_TransactionInitialize();                                             // �ŷ��ʱ�ȭ
	int	fnAPPF_TransactionMenuProcedure();                                          // �޴�����
	int	fnAPPF_TransactionGetTransaction();                                         // ���ðŷ���������
	int	fnAPPF_TransactionGetMenuDoing();                                           // ���Ӱŷ�����

	int	fnAPPF_SetProcedureCount1();                                                // ����ī��Ʈ����1
	int	fnAPPF_SetProcedureCount3();                                                // ����ī��Ʈ����3
	int	fnAPPF_SetProcedureCount5();                                                // ����ī��Ʈ����5
	int	fnAPPF_SetProcedureCount6();                                                // ����ī��Ʈ����6
	int	fnAPPF_SetProcedureCount9();                                                // ����ī��Ʈ����9
	int	fnAPPF_SendHost();                                                          // ȣ��Ʈ�۽�
	int	fnAPPF_RecvHost();                                                          // ȣ��Ʈ����
	int	fnAPPF_SendRecvSnod();                                                      // ȣ��Ʈ�ۼ���
	int	fnAPPF_GetHostResult();                                                     // ȣ��Ʈ������
	int	fnAPPF_CardImprint();                                                       // ī��������Ʈ

	int	fnAPPF_TransactionWithdrawInitialize();                                     // ��ݰŷ��ʱ�ȭ
	int	fnAPPF_TransactionWithdrawInput();                                          // ��ݼ۽����Է�
	int	fnAPPF_TransactionWithdrawOutput();                                         // ��ݼ�����ó��
	int	fnAPPF_TransactionWithdrawCheckDeviceAction();                              // ��ݰŷ�����˻�
	int	fnAPPF_TransactionWithdrawCheckMenuContinue();                              // ��ݿ��ӿ��ΰ˻�

	int	fnAPPF_TransactionInquiryInitialize();                                      // ��ȸ�ŷ��ʱ�ȭ
	int	fnAPPF_TransactionInquiryInput();                                           // ��ȸ�۽����Է�
	int	fnAPPF_TransactionInquiryOutput();                                          // ��ȸ������ó��
	int	fnAPPF_TransactionInquiryCheckDeviceAction();                               // ��ȸ�ŷ�����˻�
	int	fnAPPF_TransactionInquiryCheckMenuContinue();                               // ��ȸ���ӿ��ΰ˻�

	int	fnAPPF_TransactionTransferInitialize();                                     // �۱ݰŷ��ʱ�ȭ
	int	fnAPPF_TransactionTransferInput();                                          // �۱ݼ۽����Է�
	int	fnAPPF_TransactionTransferOutput();                                         // �۱ݼ�����ó��
	int	fnAPPF_TransactionTransferCheckDeviceAction();                              // �۱ݰŷ�����˻�
	int	fnAPPF_TransactionTransferCheckMenuContinue();                              // �۱ݿ��ӿ��ΰ˻�

	int	fnAPPF_TransactionDepositInitialize();                                      // �Աݰŷ��ʱ�ȭ
	int	fnAPPF_TransactionDepositInput();                                           // �Աݼ۽����Է�
	int	fnAPPF_TransactionDepositOutput();                                          // �Աݼ�����ó��
	int	fnAPPF_TransactionDepositCheckDeviceAction();                               // �Աݰŷ�����˻�
	int	fnAPPF_TransactionDepositCheckMenuContinue();                               // �Աݿ��ӿ��ΰ˻�

	int	fnAPPF_TransactionPassbookInitialize();                                     // ���������ŷ��ʱ�ȭ
	int	fnAPPF_TransactionPassbookInput();                                          // ���������۽����Է�
	int	fnAPPF_TransactionPassbookOutput();                                         // ��������������ó��
	int	fnAPPF_TransactionPassbookCheckDeviceAction();                              // ���������ŷ�����˻�
	int	fnAPPF_TransactionPassbookCheckMenuContinue();                              // �����������ӿ��ΰ˻�

	int	fnAPPF_TransactionTicketInitialize();                                       // Ƽ�Ϲ߱ǰŷ��ʱ�ȭ
	int	fnAPPF_TransactionTicketInput();                                            // Ƽ�Ϲ߱Ǽ۽����Է�
	int	fnAPPF_TransactionTicketOutput();                                           // Ƽ�Ϲ߱Ǽ�����ó��
	int	fnAPPF_TransactionTicketCheckDeviceAction();                                // Ƽ�Ϲ߱ǰŷ�����˻�
	int	fnAPPF_TransactionTicketCheckMenuContinue();                                // Ƽ�Ϲ߱ǿ��ӿ��ΰ˻�

	int	fnAPPF_TransactionNoMediaInitialize();                                      // ����ü�ŷ��ʱ�ȭ
	int	fnAPPF_TransactionNoMediaInput();                                           // ����ü�۽����Է�
	int	fnAPPF_TransactionNoMediaOutput();                                          // ����ü������ó��
	int	fnAPPF_TransactionNoMediaCheckDeviceAction();                               // ����ü�ŷ�����˻�
	int	fnAPPF_TransactionNoMediaCheckMenuContinue();                               // ����ü���ӿ��ΰ˻�

	int	fnAPPF_TransactionTranCommonInitialize();                                   // ����ŷ��ΰŷ��ʱ�ȭ
	int	fnAPPF_TransactionTranCommonInput();                                        // ����ŷ��μ۽����Է�
	int	fnAPPF_TransactionTranCommonOutput();                                       // ����ŷ��μ�����ó��
	int	fnAPPF_TransactionTranCommonCheckDeviceAction();                            // ����ŷ��ΰŷ�����˻�
	int	fnAPPF_TransactionTranCommonCheckMenuContinue();                            // ����ŷ��ο��ӿ��ΰ˻�



/////////////////////////////////////////////////////////////////////////////
#endif
