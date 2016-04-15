////////////////////////////////////////////////////////////////////////////////
// 파 일 명 : TranFuncProc_STA.h
// 설    명 : 거래화면부 함수선언
// 작 성 자 : SE본부
// 변경내역 : 1. 2007-05-07  ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#if !defined(_TranFuncProc_STA_H_INCLUDED_)
#define _TranFuncProc_STA_H_INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// 분기코드 선언
//////////////////////////////////////////////////////////////////////////////

#define TRANSACTIONINITIALIZE_STA                           3                  // 거래초기화
#define TRANSACTIONMENUPROCEDURE_STA                        4                  // 메뉴선택
#define TRANSACTIONGETTRANSACTION_STA                       5                  // 선택거래가져오기
#define TRANSACTIONGETMENUDOING_STA                         6                  // 연속거래여부

#define SETPROCEDURECOUNT1_STA                              8                  // 진행카운트설정1
#define SETPROCEDURECOUNT3_STA                              9                  // 진행카운트설정3
#define SETPROCEDURECOUNT5_STA                              10                 // 진행카운트설정5
#define SETPROCEDURECOUNT6_STA                              11                 // 진행카운트설정6
#define SETPROCEDURECOUNT9_STA                              12                 // 진행카운트설정9
#define SENDHOST_STA                                        13                 // 호스트송신
#define RECVHOST_STA                                        14                 // 호스트수신
#define SENDRECVSNOD_STA                                    15                 // 호스트송수신
#define GETHOSTRESULT_STA                                   16                 // 호스트결과얻기
#define CARDIMPRINT_STA                                     17                 // 카드임프린트

#define TRANSACTIONWITHDRAWINITIALIZE_STA                   19                 // 출금거래초기화
#define TRANSACTIONWITHDRAWINPUT_STA                        20                 // 출금송신전입력
#define TRANSACTIONWITHDRAWOUTPUT_STA                       21                 // 출금수신후처리
#define TRANSACTIONWITHDRAWCHECKDEVICEACTION_STA            22                 // 출금거래종료검사
#define TRANSACTIONWITHDRAWCHECKMENUCONTINUE_STA            23                 // 출금연속여부검사

#define TRANSACTIONINQUIRYINITIALIZE_STA                    25                 // 조회거래초기화
#define TRANSACTIONINQUIRYINPUT_STA                         26                 // 조회송신전입력
#define TRANSACTIONINQUIRYOUTPUT_STA                        27                 // 조회수신후처리
#define TRANSACTIONINQUIRYCHECKDEVICEACTION_STA             28                 // 조회거래종료검사
#define TRANSACTIONINQUIRYCHECKMENUCONTINUE_STA             29                 // 조회연속여부검사

#define TRANSACTIONTRANSFERINITIALIZE_STA                   31                 // 송금거래초기화
#define TRANSACTIONTRANSFERINPUT_STA                        32                 // 송금송신전입력
#define TRANSACTIONTRANSFEROUTPUT_STA                       33                 // 송금수신후처리
#define TRANSACTIONTRANSFERCHECKDEVICEACTION_STA            34                 // 송금거래종료검사
#define TRANSACTIONTRANSFERCHECKMENUCONTINUE_STA            35                 // 송금연속여부검사

#define TRANSACTIONDEPOSITINITIALIZE_STA                    37                 // 입금거래초기화
#define TRANSACTIONDEPOSITINPUT_STA                         38                 // 입금송신전입력
#define TRANSACTIONDEPOSITOUTPUT_STA                        39                 // 입금수신후처리
#define TRANSACTIONDEPOSITCHECKDEVICEACTION_STA             40                 // 입금거래종료검사
#define TRANSACTIONDEPOSITCHECKMENUCONTINUE_STA             41                 // 입금연속여부검사

#define TRANSACTIONPASSBOOKINITIALIZE_STA                   43                 // 통장정리거래초기화
#define TRANSACTIONPASSBOOKINPUT_STA                        44                 // 통장정리송신전입력
#define TRANSACTIONPASSBOOKOUTPUT_STA                       45                 // 통장정리수신후처리
#define TRANSACTIONPASSBOOKCHECKDEVICEACTION_STA            46                 // 통장정리거래종료검사
#define TRANSACTIONPASSBOOKCHECKMENUCONTINUE_STA            47                 // 통장정리연속여부검사

#define TRANSACTIONTICKETINITIALIZE_STA                     49                 // 티켓발권거래초기화
#define TRANSACTIONTICKETINPUT_STA                          50                 // 티켓발권송신전입력
#define TRANSACTIONTICKETOUTPUT_STA                         51                 // 티켓발권수신후처리
#define TRANSACTIONTICKETCHECKDEVICEACTION_STA              52                 // 티켓발권거래종료검사
#define TRANSACTIONTICKETCHECKMENUCONTINUE_STA              53                 // 티켓발권연속여부검사

#define TRANSACTIONNOMEDIAINITIALIZE_STA                    55                 // 무매체거래초기화
#define TRANSACTIONNOMEDIAINPUT_STA                         56                 // 무매체송신전입력
#define TRANSACTIONNOMEDIAOUTPUT_STA                        57                 // 무매체수신후처리
#define TRANSACTIONNOMEDIACHECKDEVICEACTION_STA             58                 // 무매체거래종료검사
#define TRANSACTIONNOMEDIACHECKMENUCONTINUE_STA             59                 // 무매체연속여부검사

#define TRANSACTIONTRANCOMMONINITIALIZE_STA                 61                 // 공통거래부거래초기화
#define TRANSACTIONTRANCOMMONINPUT_STA                      62                 // 공통거래부송신전입력
#define TRANSACTIONTRANCOMMONOUTPUT_STA                     63                 // 공통거래부수신후처리
#define TRANSACTIONTRANCOMMONCHECKDEVICEACTION_STA          64                 // 공통거래부거래종료검사
#define TRANSACTIONTRANCOMMONCHECKMENUCONTINUE_STA          65                 // 공통거래부연속여부검사



/////////////////////////////////////////////////////////////////////////////
#endif
