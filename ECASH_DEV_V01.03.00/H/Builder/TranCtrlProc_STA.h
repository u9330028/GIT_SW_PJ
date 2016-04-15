////////////////////////////////////////////////////////////////////////////////
// 파 일 명 : TranCtrlProc_STA.h
// 설    명 : 제어함수 선언
// 작 성 자 : SE본부
// 변경내역 : 1. 2007-05-07  ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCtrlProc_STA_H_INCLUDED_)
#define _TranCtrlProc_STA_H_INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// 분기코드 선언
//////////////////////////////////////////////////////////////////////////////

#define TRANBEGINPROC_STA                                   3                  // 거래시작
#define TRANENDPROC_STA                                     4                  // 거래끝
#define TRANWITHPROC_STA                                    5                  // 출금
#define TRANTRANSPROC_STA                                   6                  // 송금
#define TRANINQPROC_STA                                     7                  // 조회
#define TRANDEPPROC_STA                                     8                  // 입금
#define TRANPBPROC_STA                                      9                  // 통장정리
#define TRANTICKETPROC_STA                                  10                 // 티켓발권
#define TRANNOMEDIAPROC_STA                                 11                 // 무매체
#define TRANCOMMONPROC_STA                                  12                 // 공통거래부



/////////////////////////////////////////////////////////////////////////////
#endif
