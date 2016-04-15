////////////////////////////////////////////////////////////////////////////////
// �� �� �� : TranFuncProc_STA.h
// ��    �� : �ŷ�ȭ��� �Լ�����
// �� �� �� : SE����
// ���泻�� : 1. 2007-05-07  ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#if !defined(_TranFuncProc_STA_H_INCLUDED_)
#define _TranFuncProc_STA_H_INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// �б��ڵ� ����
//////////////////////////////////////////////////////////////////////////////

#define TRANSACTIONINITIALIZE_STA                           3                  // �ŷ��ʱ�ȭ
#define TRANSACTIONMENUPROCEDURE_STA                        4                  // �޴�����
#define TRANSACTIONGETTRANSACTION_STA                       5                  // ���ðŷ���������
#define TRANSACTIONGETMENUDOING_STA                         6                  // ���Ӱŷ�����

#define SETPROCEDURECOUNT1_STA                              8                  // ����ī��Ʈ����1
#define SETPROCEDURECOUNT3_STA                              9                  // ����ī��Ʈ����3
#define SETPROCEDURECOUNT5_STA                              10                 // ����ī��Ʈ����5
#define SETPROCEDURECOUNT6_STA                              11                 // ����ī��Ʈ����6
#define SETPROCEDURECOUNT9_STA                              12                 // ����ī��Ʈ����9
#define SENDHOST_STA                                        13                 // ȣ��Ʈ�۽�
#define RECVHOST_STA                                        14                 // ȣ��Ʈ����
#define SENDRECVSNOD_STA                                    15                 // ȣ��Ʈ�ۼ���
#define GETHOSTRESULT_STA                                   16                 // ȣ��Ʈ������
#define CARDIMPRINT_STA                                     17                 // ī��������Ʈ

#define TRANSACTIONWITHDRAWINITIALIZE_STA                   19                 // ��ݰŷ��ʱ�ȭ
#define TRANSACTIONWITHDRAWINPUT_STA                        20                 // ��ݼ۽����Է�
#define TRANSACTIONWITHDRAWOUTPUT_STA                       21                 // ��ݼ�����ó��
#define TRANSACTIONWITHDRAWCHECKDEVICEACTION_STA            22                 // ��ݰŷ�����˻�
#define TRANSACTIONWITHDRAWCHECKMENUCONTINUE_STA            23                 // ��ݿ��ӿ��ΰ˻�

#define TRANSACTIONINQUIRYINITIALIZE_STA                    25                 // ��ȸ�ŷ��ʱ�ȭ
#define TRANSACTIONINQUIRYINPUT_STA                         26                 // ��ȸ�۽����Է�
#define TRANSACTIONINQUIRYOUTPUT_STA                        27                 // ��ȸ������ó��
#define TRANSACTIONINQUIRYCHECKDEVICEACTION_STA             28                 // ��ȸ�ŷ�����˻�
#define TRANSACTIONINQUIRYCHECKMENUCONTINUE_STA             29                 // ��ȸ���ӿ��ΰ˻�

#define TRANSACTIONTRANSFERINITIALIZE_STA                   31                 // �۱ݰŷ��ʱ�ȭ
#define TRANSACTIONTRANSFERINPUT_STA                        32                 // �۱ݼ۽����Է�
#define TRANSACTIONTRANSFEROUTPUT_STA                       33                 // �۱ݼ�����ó��
#define TRANSACTIONTRANSFERCHECKDEVICEACTION_STA            34                 // �۱ݰŷ�����˻�
#define TRANSACTIONTRANSFERCHECKMENUCONTINUE_STA            35                 // �۱ݿ��ӿ��ΰ˻�

#define TRANSACTIONDEPOSITINITIALIZE_STA                    37                 // �Աݰŷ��ʱ�ȭ
#define TRANSACTIONDEPOSITINPUT_STA                         38                 // �Աݼ۽����Է�
#define TRANSACTIONDEPOSITOUTPUT_STA                        39                 // �Աݼ�����ó��
#define TRANSACTIONDEPOSITCHECKDEVICEACTION_STA             40                 // �Աݰŷ�����˻�
#define TRANSACTIONDEPOSITCHECKMENUCONTINUE_STA             41                 // �Աݿ��ӿ��ΰ˻�

#define TRANSACTIONPASSBOOKINITIALIZE_STA                   43                 // ���������ŷ��ʱ�ȭ
#define TRANSACTIONPASSBOOKINPUT_STA                        44                 // ���������۽����Է�
#define TRANSACTIONPASSBOOKOUTPUT_STA                       45                 // ��������������ó��
#define TRANSACTIONPASSBOOKCHECKDEVICEACTION_STA            46                 // ���������ŷ�����˻�
#define TRANSACTIONPASSBOOKCHECKMENUCONTINUE_STA            47                 // �����������ӿ��ΰ˻�

#define TRANSACTIONTICKETINITIALIZE_STA                     49                 // Ƽ�Ϲ߱ǰŷ��ʱ�ȭ
#define TRANSACTIONTICKETINPUT_STA                          50                 // Ƽ�Ϲ߱Ǽ۽����Է�
#define TRANSACTIONTICKETOUTPUT_STA                         51                 // Ƽ�Ϲ߱Ǽ�����ó��
#define TRANSACTIONTICKETCHECKDEVICEACTION_STA              52                 // Ƽ�Ϲ߱ǰŷ�����˻�
#define TRANSACTIONTICKETCHECKMENUCONTINUE_STA              53                 // Ƽ�Ϲ߱ǿ��ӿ��ΰ˻�

#define TRANSACTIONNOMEDIAINITIALIZE_STA                    55                 // ����ü�ŷ��ʱ�ȭ
#define TRANSACTIONNOMEDIAINPUT_STA                         56                 // ����ü�۽����Է�
#define TRANSACTIONNOMEDIAOUTPUT_STA                        57                 // ����ü������ó��
#define TRANSACTIONNOMEDIACHECKDEVICEACTION_STA             58                 // ����ü�ŷ�����˻�
#define TRANSACTIONNOMEDIACHECKMENUCONTINUE_STA             59                 // ����ü���ӿ��ΰ˻�

#define TRANSACTIONTRANCOMMONINITIALIZE_STA                 61                 // ����ŷ��ΰŷ��ʱ�ȭ
#define TRANSACTIONTRANCOMMONINPUT_STA                      62                 // ����ŷ��μ۽����Է�
#define TRANSACTIONTRANCOMMONOUTPUT_STA                     63                 // ����ŷ��μ�����ó��
#define TRANSACTIONTRANCOMMONCHECKDEVICEACTION_STA          64                 // ����ŷ��ΰŷ�����˻�
#define TRANSACTIONTRANCOMMONCHECKMENUCONTINUE_STA          65                 // ����ŷ��ο��ӿ��ΰ˻�



/////////////////////////////////////////////////////////////////////////////
#endif
