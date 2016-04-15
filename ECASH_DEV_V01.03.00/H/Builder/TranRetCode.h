////////////////////////////////////////////////////////////////////////////////
// 파 일 명 : TranRetCode.h
// 설    명 : 리턴코드정의
// 작 성 자 : SE본부
// 변경내역 : 1. 2007-05-07  ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#if !defined(_TranRetCode_H_INCLUDED_)
#define _TranRetCode_H_INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// 리턴코드 선언
//////////////////////////////////////////////////////////////////////////////

#define RET_ABNORMAL_STEP                                  0                   // AbnormalStep
#define RET_NEXT_STEP                                      1                   // NextStep
#define RET_BEFORE_STEP                                    2                   // BeforeStep
#define RET_LOOP_STEP                                      3                   // LoopStep

#define RET_TRUE                                           10                  // ReturnTrue
#define RET_FALSE                                          11                  // ReturnFalse

#define RET_WITHDRAW                                       20                  // 출금
#define RET_TRANSFER                                       21                  // 송금
#define RET_INQUIRY                                        22                  // 조회
#define RET_DEPOSIT                                        23                  // 입금
#define RET_PASSBOOK                                       24                  // 통장정리
#define RET_TICKET                                         101                 // 티켓발권
#define RET_NOMEDIA                                        102                 // 무매체
#define RET_COMMONSUB                                      103                 // 공통거래부

#define RET_HOST_OK                                        30                  // HostOk
#define RET_HOST_NG                                        31                  // HostNg



/////////////////////////////////////////////////////////////////////////////
#endif
