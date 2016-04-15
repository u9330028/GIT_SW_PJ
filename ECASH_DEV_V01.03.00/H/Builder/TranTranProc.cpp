////////////////////////////////////////////////////////////////////////////////
// �� �� �� : TranTranProc.cpp
// ��    �� : ���ΰŷ��б�
// �� �� �� : SE����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
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
// �� �� �� : fnAPPM_TranTranProc
// ��    �� : ���ΰŷ��б�
// �Ķ���� : ����
// �� �� �� : T_OK     - ����
//            T_ERROR  - ����
// ���ǻ��� : ����
// ���泻�� : 1. 2007-05-07  Cd-Van ver 01.00.00  AUTO - �űԻ���
//               �������� T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 ����
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 ����
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPM_TranTranProc()
{
	// �ʱ���� ����
	int nNextState = TRANBEGINPROC_STA;
	
	while (1)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__,
			"fnAPPM_TranTranProc:m_nRetCode[%d], nNextState[%d]", m_nRetCode, nNextState);

		// nNextState�� ���� ���� �����ܰ�� �б�
		switch (nNextState)
		{
			// �ŷ�����
			case TRANBEGINPROC_STA:
				fnAPPC_TranBeginProc();
				switch(m_nRetCode)
				{
					case RET_WITHDRAW:                               // ���
						nNextState = TRANWITHPROC_STA;
						break;
					case RET_TRANSFER:                               // �۱�
						nNextState = TRANTRANSPROC_STA;
						break;
					case RET_INQUIRY:                                // ��ȸ
						nNextState = TRANINQPROC_STA;
						break;
					case RET_DEPOSIT:                                // �Ա�
						nNextState = TRANDEPPROC_STA;
						break;
					case RET_PASSBOOK:                               // ��������
						nNextState = TRANPBPROC_STA;
						break;
					case RET_TICKET:                                 // Ƽ�Ϲ߱�
						nNextState = TRANTICKETPROC_STA;
						break;
					case RET_NOMEDIA:                                // ����ü
						nNextState = TRANNOMEDIAPROC_STA;
						break;
					case RET_COMMONSUB:                              // ����ŷ���
						nNextState = TRANCOMMONPROC_STA;
						break;
					default:                                         // �̻�����
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// ���
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
					default:                                         // �̻�����
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// �۱�
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
					default:                                         // �̻�����
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// ��ȸ
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
					default:                                         // �̻�����
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// �Ա�
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
					default:                                         // �̻�����
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// ��������
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
					default:                                         // �̻�����
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// Ƽ�Ϲ߱�
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
					default:                                         // �̻�����
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// ����ü
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
					default:                                         // �̻�����
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// ����ŷ���
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
					default:                                         // �̻�����
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// �ŷ���
			case TRANENDPROC_STA:
				fnAPPC_TranEndProc();

				switch(m_nRetCode)
				{
					case RET_TRUE:                                   // ReturnTrue
						nNextState = TRANBEGINPROC_STA;
						break;
					case RET_FALSE:                                  // ReturnFalse
						return T_OK;
					default:                                         // �̻�����
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			default:                                         // �̻�����
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
