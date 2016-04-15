////////////////////////////////////////////////////////////////////////////////
// 파 일 명 : TranCtrlProc.h
// 설    명 : 제어함수 선언
// 작 성 자 : SE본부
// 변경내역 : 1. 2007-05-07  ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
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

	int	fnAPPC_TranBeginProc();                                                     // 거래시작
	int	fnAPPC_TranEndProc();                                                       // 거래끝
	int	fnAPPC_TranWithProc();                                                      // 출금
	int	fnAPPC_TranTransProc();                                                     // 송금
	int	fnAPPC_TranInqProc();                                                       // 조회
	int	fnAPPC_TranDepProc();                                                       // 입금
	int	fnAPPC_TranPbProc();                                                        // 통장정리
	int	fnAPPC_TranTicketProc();                                                    // 티켓발권
	int	fnAPPC_TranNoMediaProc();                                                   // 무매체
	int	fnAPPC_TranCommonProc();                                                    // 공통거래부



/////////////////////////////////////////////////////////////////////////////
#endif
