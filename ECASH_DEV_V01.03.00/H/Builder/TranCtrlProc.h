////////////////////////////////////////////////////////////////////////////////
// �� �� �� : TranCtrlProc.h
// ��    �� : �����Լ� ����
// �� �� �� : SE����
// ���泻�� : 1. 2007-05-07  ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCtrlProc_H_INCLUDED_)
#define _TranCtrlProc_H_INCLUDED_

//////////////////////////////////////////////////////////////////////////////
// Attributes
public:

//////////////////////////////////////////////////////////////////////////////
// Operations
public:

	int	fnAPPC_TranBeginProc();                                                     // �ŷ�����
	int	fnAPPC_TranEndProc();                                                       // �ŷ���
	int	fnAPPC_TranWithProc();                                                      // ���
	int	fnAPPC_TranTransProc();                                                     // �۱�
	int	fnAPPC_TranInqProc();                                                       // ��ȸ
	int	fnAPPC_TranDepProc();                                                       // �Ա�
	int	fnAPPC_TranPbProc();                                                        // ��������
	int	fnAPPC_TranTicketProc();                                                    // Ƽ�Ϲ߱�
	int	fnAPPC_TranNoMediaProc();                                                   // ����ü
	int	fnAPPC_TranCommonProc();                                                    // ����ŷ���



/////////////////////////////////////////////////////////////////////////////
#endif
