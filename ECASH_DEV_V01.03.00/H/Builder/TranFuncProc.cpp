////////////////////////////////////////////////////////////////////////////////
// 파 일 명 : TranFuncProc.cpp
// 설    명 : 거래화면부 함수선언
// 작 성 자 : SE본부
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
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
// 함 수 명 : fnAPPF_TransactionInitialize()
// 설    명 : 거래초기화
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInitialize()
{
	// To Do
	fnAPP_TranInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionMenuProcedure()
// 설    명 : 메뉴선택
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionMenuProcedure()
{
	// To Do
	fnAPP_TranMenuProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionGetTransaction()
// 설    명 : 선택거래가져오기
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionGetTransaction()
{
	// To Do
	fnAPP_TranGetTranProc();

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionGetMenuDoing()
// 설    명 : 연속거래여부
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionGetMenuDoing()
{
	// To Do
	fnAPP_TranGetMenuDoingProc();
	m_nRetCode = (MenuDoingFlag) ? RET_TRUE : RET_FALSE;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_SetProcedureCount1()
// 설    명 : 진행카운트설정1
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount1()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('1');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_SetProcedureCount3()
// 설    명 : 진행카운트설정3
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount3()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('3');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_SetProcedureCount5()
// 설    명 : 진행카운트설정5
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount5()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('5');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_SetProcedureCount6()
// 설    명 : 진행카운트설정6
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount6()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('6');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_SetProcedureCount9()
// 설    명 : 진행카운트설정9
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount9()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('9');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_SendHost()
// 설    명 : 호스트송신
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SendHost()
{
	// To Do
	fnAPP_SendHost();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_RecvHost()
// 설    명 : 호스트수신
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_RecvHost()
{
	// To Do
	fnAPP_RecvHost();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_SendRecvSnod()
// 설    명 : 호스트송수신
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SendRecvSnod()
{
	// To Do
	fnAPP_SendRecvHostSnod();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_GetHostResult()
// 설    명 : 호스트결과얻기
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_GetHostResult()
{
	// To Do
	m_nRetCode = (m_pDevCmn->TranResult) ? RET_HOST_OK : RET_HOST_NG;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_CardImprint()
// 설    명 : 카드임프린트
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_CardImprint()
{
	// To Do
	fnAPD_CardImprint();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionWithdrawInitialize()
// 설    명 : 출금거래초기화
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawInitialize()
{
	// To Do
	fnAPP_TranWithInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionWithdrawInput()
// 설    명 : 출금송신전입력
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawInput()
{
	// To Do
	fnAPP_TranWithInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionWithdrawOutput()
// 설    명 : 출금수신후처리
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawOutput()
{
	// To Do
	fnAPP_TranWithOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionWithdrawCheckDeviceAction()
// 설    명 : 출금거래종료검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawCheckDeviceAction()
{
	// To Do
	fnAPP_TranWithCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionWithdrawCheckMenuContinue()
// 설    명 : 출금연속여부검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawCheckMenuContinue()
{
	// To Do
	fnAPP_TranWithCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionInquiryInitialize()
// 설    명 : 조회거래초기화
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryInitialize()
{
	// To Do
	fnAPP_TranInqInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionInquiryInput()
// 설    명 : 조회송신전입력
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryInput()
{
	// To Do
	fnAPP_TranInqInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionInquiryOutput()
// 설    명 : 조회수신후처리
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryOutput()
{
	// To Do
	fnAPP_TranInqOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionInquiryCheckDeviceAction()
// 설    명 : 조회거래종료검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryCheckDeviceAction()
{
	// To Do
	fnAPP_TranInqCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionInquiryCheckMenuContinue()
// 설    명 : 조회연속여부검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryCheckMenuContinue()
{
	// To Do
	fnAPP_TranInqCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTransferInitialize()
// 설    명 : 송금거래초기화
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferInitialize()
{
	// To Do
	fnAPP_TranTransInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTransferInput()
// 설    명 : 송금송신전입력
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferInput()
{
	// To Do
	fnAPP_TranTransInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTransferOutput()
// 설    명 : 송금수신후처리
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferOutput()
{
	// To Do
	fnAPP_TranTransOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTransferCheckDeviceAction()
// 설    명 : 송금거래종료검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferCheckDeviceAction()
{
	// To Do
	fnAPP_TranTransCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTransferCheckMenuContinue()
// 설    명 : 송금연속여부검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferCheckMenuContinue()
{
	// To Do
	fnAPP_TranTransCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionDepositInitialize()
// 설    명 : 입금거래초기화
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositInitialize()
{
	// To Do
	fnAPP_TranDepInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionDepositInput()
// 설    명 : 입금송신전입력
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositInput()
{
	// To Do
	fnAPP_TranDepInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionDepositOutput()
// 설    명 : 입금수신후처리
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositOutput()
{
	// To Do
	fnAPP_TranDepOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionDepositCheckDeviceAction()
// 설    명 : 입금거래종료검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositCheckDeviceAction()
{
	// To Do
	fnAPP_TranDepCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionDepositCheckMenuContinue()
// 설    명 : 입금연속여부검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositCheckMenuContinue()
{
	// To Do
	fnAPP_TranDepCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionPassbookInitialize()
// 설    명 : 통장정리거래초기화
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookInitialize()
{
	// To Do
	fnAPP_TranPbInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionPassbookInput()
// 설    명 : 통장정리송신전입력
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookInput()
{
	// To Do
	fnAPP_TranPbInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionPassbookOutput()
// 설    명 : 통장정리수신후처리
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookOutput()
{
	// To Do
	fnAPP_TranPbOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionPassbookCheckDeviceAction()
// 설    명 : 통장정리거래종료검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookCheckDeviceAction()
{
	// To Do
	fnAPP_TranPbCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionPassbookCheckMenuContinue()
// 설    명 : 통장정리연속여부검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookCheckMenuContinue()
{
	// To Do
	fnAPP_TranPbCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTicketInitialize()
// 설    명 : 티켓발권거래초기화
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTicketInitialize()
{
	// To Do
	fnAPP_TranTicketInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTicketInput()
// 설    명 : 티켓발권송신전입력
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTicketInput()
{
	// To Do
	fnAPP_TranTicketInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTicketOutput()
// 설    명 : 티켓발권수신후처리
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTicketOutput()
{
	// To Do
	fnAPP_TranTicketOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTicketCheckDeviceAction()
// 설    명 : 티켓발권거래종료검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTicketCheckDeviceAction()
{
	// To Do
	fnAPP_TranTicketCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTicketCheckMenuContinue()
// 설    명 : 티켓발권연속여부검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTicketCheckMenuContinue()
{
	// To Do
	fnAPP_TranTicketCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionNoMediaInitialize()
// 설    명 : 무매체거래초기화
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionNoMediaInitialize()
{
	// To Do
	fnAPP_TranNoMediaInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionNoMediaInput()
// 설    명 : 무매체송신전입력
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionNoMediaInput()
{
	// To Do
	fnAPP_TranNoMediaInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionNoMediaOutput()
// 설    명 : 무매체수신후처리
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionNoMediaOutput()
{
	// To Do
	fnAPP_TranNoMediaOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionNoMediaCheckDeviceAction()
// 설    명 : 무매체거래종료검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionNoMediaCheckDeviceAction()
{
	// To Do
	fnAPP_TranNoMediaCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionNoMediaCheckMenuContinue()
// 설    명 : 무매체연속여부검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionNoMediaCheckMenuContinue()
{
	// To Do
	fnAPP_TranNoMediaCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTranCommonInitialize()
// 설    명 : 공통거래부거래초기화
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTranCommonInitialize()
{
	// To Do
	fnAPP_TranCommonInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTranCommonInput()
// 설    명 : 공통거래부송신전입력
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTranCommonInput()
{
	// To Do
	fnAPP_TranCommonInputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTranCommonOutput()
// 설    명 : 공통거래부수신후처리
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTranCommonOutput()
{
	// To Do
	fnAPP_TranCommonOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTranCommonCheckDeviceAction()
// 설    명 : 공통거래부거래종료검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTranCommonCheckDeviceAction()
{
	// To Do
	fnAPP_TranCommonCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPPF_TransactionTranCommonCheckMenuContinue()
// 설    명 : 공통거래부연속여부검사
// 파라미터 : 없음
// 리 턴 값 : T_OK  - 성공
//            T_NG  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTranCommonCheckMenuContinue()
{
	// To Do
	fnAPP_TranCommonCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}
