////////////////////////////////////////////////////////////////////////////////
// �� �� �� : TranRetCode.h
// ��    �� : �����ڵ�����
// �� �� �� : SE����
// ���泻�� : 1. 2007-05-07  ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#if !defined(_TranRetCode_H_INCLUDED_)
#define _TranRetCode_H_INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// �����ڵ� ����
//////////////////////////////////////////////////////////////////////////////

#define RET_ABNORMAL_STEP                                  0                   // AbnormalStep
#define RET_NEXT_STEP                                      1                   // NextStep
#define RET_BEFORE_STEP                                    2                   // BeforeStep
#define RET_LOOP_STEP                                      3                   // LoopStep

#define RET_TRUE                                           10                  // ReturnTrue
#define RET_FALSE                                          11                  // ReturnFalse

#define RET_WITHDRAW                                       20                  // ���
#define RET_TRANSFER                                       21                  // �۱�
#define RET_INQUIRY                                        22                  // ��ȸ
#define RET_DEPOSIT                                        23                  // �Ա�
#define RET_PASSBOOK                                       24                  // ��������
#define RET_TICKET                                         101                 // Ƽ�Ϲ߱�
#define RET_NOMEDIA                                        102                 // ����ü
#define RET_COMMONSUB                                      103                 // ����ŷ���

#define RET_HOST_OK                                        30                  // HostOk
#define RET_HOST_NG                                        31                  // HostNg



/////////////////////////////////////////////////////////////////////////////
#endif
