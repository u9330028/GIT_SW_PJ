////////////////////////////////////////////////////////////////////////////////
// 파 일 명 : TranBeginProc.cpp
// 설    명 : 거래시작
// 작 성 자 : SE본부
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TranFuncProc_STA.h"
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
// 함 수 명 : fnAPPC_TranBeginProc
// 설    명 : 거래시작
// 파라미터 : 없음
// 리 턴 값 : T_OK     - 성공
//            T_ERROR  - 실패
// 주의사항 : 없음
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPC_TranBeginProc()
{
	// 초기상태 지정
	int nNextState = TRANSACTIONINITIALIZE_STA;
	m_pDevCmn->fnAPL_SetProcCount('1');                              // 진행설정
	
	while (1)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__,
			"fnAPPC_TranBeginProc:m_nRetCode[%d], nNextState[%d]", m_nRetCode, nNextState);

		// nNextState의 값에 따라 다음단계로 분기
		switch (nNextState)
		{
			// 거래초기화
			case TRANSACTIONINITIALIZE_STA:
				fnAPPF_TransactionInitialize();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANSACTIONMENUPROCEDURE_STA;
						break;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// 메뉴선택
			case TRANSACTIONMENUPROCEDURE_STA:
				fnAPPF_TransactionMenuProcedure();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANSACTIONGETTRANSACTION_STA;
						break;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// 선택거래가져오기
			case TRANSACTIONGETTRANSACTION_STA:
				fnAPPF_TransactionGetTransaction();
				switch(m_nRetCode)
				{
					case RET_WITHDRAW:                               // 출금
						return T_OK;
					case RET_TRANSFER:                               // 송금
						return T_OK;
					case RET_INQUIRY:                                // 조회
						return T_OK;
					case RET_DEPOSIT:                                // 입금
						return T_OK;
					case RET_PASSBOOK:                               // 통장정리
						return T_OK;
					case RET_TICKET:                                 // 티켓발권
						return T_OK;
					case RET_NOMEDIA:                                // 무매체
						return T_OK;
					case RET_COMMONSUB:                              // 공통거래부
						return T_OK;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			default:                                         // 이상종료
				fnAPP_CancelProc(T_PROGRAM);

		}     // End switch

	}     // End while

	return T_OK;

}
