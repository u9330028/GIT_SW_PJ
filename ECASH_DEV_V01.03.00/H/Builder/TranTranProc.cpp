////////////////////////////////////////////////////////////////////////////////
// 파 일 명 : TranTranProc.cpp
// 설    명 : 메인거래분기
// 작 성 자 : SE본부
// 변경내역 : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TranCtrlProc_STA.h"
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
// 함 수 명 : fnAPPM_TranTranProc
// 설    명 : 메인거래분기
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
int CTranCmn::fnAPPM_TranTranProc()
{
	// 초기상태 지정
	int nNextState = TRANBEGINPROC_STA;
	
	while (1)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__,
			"fnAPPM_TranTranProc:m_nRetCode[%d], nNextState[%d]", m_nRetCode, nNextState);

		// nNextState의 값에 따라 다음단계로 분기
		switch (nNextState)
		{
			// 거래시작
			case TRANBEGINPROC_STA:
				fnAPPC_TranBeginProc();
				switch(m_nRetCode)
				{
					case RET_WITHDRAW:                               // 출금
						nNextState = TRANWITHPROC_STA;
						break;
					case RET_TRANSFER:                               // 송금
						nNextState = TRANTRANSPROC_STA;
						break;
					case RET_INQUIRY:                                // 조회
						nNextState = TRANINQPROC_STA;
						break;
					case RET_DEPOSIT:                                // 입금
						nNextState = TRANDEPPROC_STA;
						break;
					case RET_PASSBOOK:                               // 통장정리
						nNextState = TRANPBPROC_STA;
						break;
					case RET_TICKET:                                 // 티켓발권
						nNextState = TRANTICKETPROC_STA;
						break;
					case RET_NOMEDIA:                                // 무매체
						nNextState = TRANNOMEDIAPROC_STA;
						break;
					case RET_COMMONSUB:                              // 공통거래부
						nNextState = TRANCOMMONPROC_STA;
						break;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// 출금
			case TRANWITHPROC_STA:
				fnAPPC_TranWithProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANWITHPROC_STA;
						break;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// 송금
			case TRANTRANSPROC_STA:
				fnAPPC_TranTransProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANTRANSPROC_STA;
						break;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// 조회
			case TRANINQPROC_STA:
				fnAPPC_TranInqProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						if(ChoiceTran == TRAN_WITH)					//#N0223
							nNextState = TRANWITHPROC_STA;
						else
						if(ChoiceTran == TRAN_TRANS)
							nNextState = TRANTRANSPROC_STA;
						else
							nNextState = TRANENDPROC_STA;
						break;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// 입금
			case TRANDEPPROC_STA:
				fnAPPC_TranDepProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANDEPPROC_STA;
						break;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// 통장정리
			case TRANPBPROC_STA:
				fnAPPC_TranPbProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANPBPROC_STA;
						break;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// 티켓발권
			case TRANTICKETPROC_STA:
				fnAPPC_TranTicketProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANTICKETPROC_STA;
						break;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// 무매체
			case TRANNOMEDIAPROC_STA:
				fnAPPC_TranNoMediaProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANNOMEDIAPROC_STA;
						break;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// 공통거래부
			case TRANCOMMONPROC_STA:
				fnAPPC_TranCommonProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANCOMMONPROC_STA;
						break;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// 거래끝
			case TRANENDPROC_STA:
				fnAPPC_TranEndProc();

				switch(m_nRetCode)
				{
					case RET_TRUE:                                   // ReturnTrue
						nNextState = TRANBEGINPROC_STA;
						break;
					case RET_FALSE:                                  // ReturnFalse
						return T_OK;
					default:                                         // 이상종료
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			default:                                         // 이상종료
				fnAPP_CancelProc(T_PROGRAM);

		}     // End switch

	}     // End while


	if (   TranCode2 == TRANID_7210 || TranCode2 == TRANID_7220 || TranCode2 == TRANID_7260 
		|| TranCode2 == TRANID_7230 || TranCode2 == TRANID_7250 || TranCode2 == TRANID_7380 
		|| TranCode2 == TRANID_4370 || TranCode2 == TRANID_4390 || TranCode2 == TRANID_4310
		|| TranCode2 == TRANID_4330 || TranCode2 == TRANID_4350 || TranCode2 == TRANID_4320
		|| TranCode2 == TRANID_4340 || TranCode2 == TRANID_7200 || TranCode2 == TRANID_4410 || TranCode2 == TRANID_7530) 
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("END  :02OOOOOOOOOOOO<<=====<<=====<<=====<<<<"));
	}



	return T_OK;

}
