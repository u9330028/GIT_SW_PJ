////////////////////////////////////////////////////////////////////////////////
// �� �� �� : TranFuncProc.cpp
// ��    �� : �ŷ�ȭ��� �Լ�����
// �� �� �� : SE����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
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



////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionInitialize()
// ��    �� : �ŷ��ʱ�ȭ
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInitialize()
{
	// To Do
	fnAPP_TranInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionMenuProcedure()
// ��    �� : �޴�����
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionMenuProcedure()
{
	// To Do
	fnAPP_TranMenuProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionGetTransaction()
// ��    �� : ���ðŷ���������
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionGetTransaction()
{
	// To Do
	fnAPP_TranGetTranProc();

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionGetMenuDoing()
// ��    �� : ���Ӱŷ�����
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionGetMenuDoing()
{
	// To Do
	fnAPP_TranGetMenuDoingProc();
	m_nRetCode = (MenuDoingFlag) ? RET_TRUE : RET_FALSE;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_SetProcedureCount1()
// ��    �� : ����ī��Ʈ����1
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount1()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('1');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_SetProcedureCount3()
// ��    �� : ����ī��Ʈ����3
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount3()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('3');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_SetProcedureCount5()
// ��    �� : ����ī��Ʈ����5
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount5()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('5');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_SetProcedureCount6()
// ��    �� : ����ī��Ʈ����6
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount6()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('6');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_SetProcedureCount9()
// ��    �� : ����ī��Ʈ����9
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount9()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('9');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_SendHost()
// ��    �� : ȣ��Ʈ�۽�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SendHost()
{
	// To Do
	fnAPP_SendHost();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_RecvHost()
// ��    �� : ȣ��Ʈ����
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_RecvHost()
{
	// To Do
	fnAPP_RecvHost();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_SendRecvSnod()
// ��    �� : ȣ��Ʈ�ۼ���
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SendRecvSnod()
{
	// To Do
	fnAPP_SendRecvHostSnod();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_GetHostResult()
// ��    �� : ȣ��Ʈ������
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_GetHostResult()
{
	// To Do
	m_nRetCode = (m_pDevCmn->TranResult) ? RET_HOST_OK : RET_HOST_NG;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_CardImprint()
// ��    �� : ī��������Ʈ
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_CardImprint()
{
	// To Do
	fnAPD_CardImprint();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionWithdrawInitialize()
// ��    �� : ��ݰŷ��ʱ�ȭ
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawInitialize()
{
	// To Do
	fnAPP_TranWithInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionWithdrawInput()
// ��    �� : ��ݼ۽����Է�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawInput()
{
	// To Do
	fnAPP_TranWithInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionWithdrawOutput()
// ��    �� : ��ݼ�����ó��
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawOutput()
{
	// To Do
	fnAPP_TranWithOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionWithdrawCheckDeviceAction()
// ��    �� : ��ݰŷ�����˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawCheckDeviceAction()
{
	// To Do
	fnAPP_TranWithCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionWithdrawCheckMenuContinue()
// ��    �� : ��ݿ��ӿ��ΰ˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawCheckMenuContinue()
{
	// To Do
	fnAPP_TranWithCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionInquiryInitialize()
// ��    �� : ��ȸ�ŷ��ʱ�ȭ
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryInitialize()
{
	// To Do
	fnAPP_TranInqInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionInquiryInput()
// ��    �� : ��ȸ�۽����Է�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryInput()
{
	// To Do
	fnAPP_TranInqInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionInquiryOutput()
// ��    �� : ��ȸ������ó��
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryOutput()
{
	// To Do
	fnAPP_TranInqOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionInquiryCheckDeviceAction()
// ��    �� : ��ȸ�ŷ�����˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryCheckDeviceAction()
{
	// To Do
	fnAPP_TranInqCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionInquiryCheckMenuContinue()
// ��    �� : ��ȸ���ӿ��ΰ˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryCheckMenuContinue()
{
	// To Do
	fnAPP_TranInqCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTransferInitialize()
// ��    �� : �۱ݰŷ��ʱ�ȭ
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferInitialize()
{
	// To Do
	fnAPP_TranTransInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTransferInput()
// ��    �� : �۱ݼ۽����Է�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferInput()
{
	// To Do
	fnAPP_TranTransInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTransferOutput()
// ��    �� : �۱ݼ�����ó��
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferOutput()
{
	// To Do
	fnAPP_TranTransOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTransferCheckDeviceAction()
// ��    �� : �۱ݰŷ�����˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferCheckDeviceAction()
{
	// To Do
	fnAPP_TranTransCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTransferCheckMenuContinue()
// ��    �� : �۱ݿ��ӿ��ΰ˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferCheckMenuContinue()
{
	// To Do
	fnAPP_TranTransCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionDepositInitialize()
// ��    �� : �Աݰŷ��ʱ�ȭ
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositInitialize()
{
	// To Do
	fnAPP_TranDepInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionDepositInput()
// ��    �� : �Աݼ۽����Է�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositInput()
{
	// To Do
	fnAPP_TranDepInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionDepositOutput()
// ��    �� : �Աݼ�����ó��
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositOutput()
{
	// To Do
	fnAPP_TranDepOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionDepositCheckDeviceAction()
// ��    �� : �Աݰŷ�����˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositCheckDeviceAction()
{
	// To Do
	fnAPP_TranDepCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionDepositCheckMenuContinue()
// ��    �� : �Աݿ��ӿ��ΰ˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositCheckMenuContinue()
{
	// To Do
	fnAPP_TranDepCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionPassbookInitialize()
// ��    �� : ���������ŷ��ʱ�ȭ
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookInitialize()
{
	// To Do
	fnAPP_TranPbInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionPassbookInput()
// ��    �� : ���������۽����Է�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookInput()
{
	// To Do
	fnAPP_TranPbInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionPassbookOutput()
// ��    �� : ��������������ó��
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookOutput()
{
	// To Do
	fnAPP_TranPbOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionPassbookCheckDeviceAction()
// ��    �� : ���������ŷ�����˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookCheckDeviceAction()
{
	// To Do
	fnAPP_TranPbCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionPassbookCheckMenuContinue()
// ��    �� : �����������ӿ��ΰ˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookCheckMenuContinue()
{
	// To Do
	fnAPP_TranPbCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTicketInitialize()
// ��    �� : Ƽ�Ϲ߱ǰŷ��ʱ�ȭ
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTicketInitialize()
{
	// To Do
	fnAPP_TranTicketInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTicketInput()
// ��    �� : Ƽ�Ϲ߱Ǽ۽����Է�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTicketInput()
{
	// To Do
	fnAPP_TranTicketInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTicketOutput()
// ��    �� : Ƽ�Ϲ߱Ǽ�����ó��
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTicketOutput()
{
	// To Do
	fnAPP_TranTicketOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTicketCheckDeviceAction()
// ��    �� : Ƽ�Ϲ߱ǰŷ�����˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTicketCheckDeviceAction()
{
	// To Do
	fnAPP_TranTicketCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTicketCheckMenuContinue()
// ��    �� : Ƽ�Ϲ߱ǿ��ӿ��ΰ˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTicketCheckMenuContinue()
{
	// To Do
	fnAPP_TranTicketCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionNoMediaInitialize()
// ��    �� : ����ü�ŷ��ʱ�ȭ
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionNoMediaInitialize()
{
	// To Do
	fnAPP_TranNoMediaInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionNoMediaInput()
// ��    �� : ����ü�۽����Է�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionNoMediaInput()
{
	// To Do
	fnAPP_TranNoMediaInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionNoMediaOutput()
// ��    �� : ����ü������ó��
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionNoMediaOutput()
{
	// To Do
	fnAPP_TranNoMediaOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionNoMediaCheckDeviceAction()
// ��    �� : ����ü�ŷ�����˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionNoMediaCheckDeviceAction()
{
	// To Do
	fnAPP_TranNoMediaCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionNoMediaCheckMenuContinue()
// ��    �� : ����ü���ӿ��ΰ˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionNoMediaCheckMenuContinue()
{
	// To Do
	fnAPP_TranNoMediaCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTranCommonInitialize()
// ��    �� : ����ŷ��ΰŷ��ʱ�ȭ
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTranCommonInitialize()
{
	// To Do
	fnAPP_TranCommonInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTranCommonInput()
// ��    �� : ����ŷ��μ۽����Է�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTranCommonInput()
{
	// To Do
	fnAPP_TranCommonInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTranCommonOutput()
// ��    �� : ����ŷ��μ�����ó��
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTranCommonOutput()
{
	// To Do
	fnAPP_TranCommonOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTranCommonCheckDeviceAction()
// ��    �� : ����ŷ��ΰŷ�����˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTranCommonCheckDeviceAction()
{
	// To Do
	fnAPP_TranCommonCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPPF_TransactionTranCommonCheckMenuContinue()
// ��    �� : ����ŷ��ο��ӿ��ΰ˻�
// �Ķ���� : ����
// �� �� �� : T_OK  - ����
//            T_NG  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTranCommonCheckMenuContinue()
{
	// To Do
	fnAPP_TranCommonCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}
