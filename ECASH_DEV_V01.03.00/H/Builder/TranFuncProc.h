////////////////////////////////////////////////////////////////////////////////
// 파 일 명 : TranFuncProc.h
// 설    명 : 거래화면부 함수선언
// 작 성 자 : SE본부
// 변경내역 : 1. 2007-05-07  ver 01.00.00  AUTO - 신규생성
//               동양증권 T2
//            2. 2010-05-09  ver 03.00.07  T1,T2,CATM,NH2700 통합
//            3. 2010-10-14  ver 03.00.08  T1,T2,CATM,U8100,MX2800 통합
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#if !defined(_TranFuncProc_H_INCLUDED_)
#define _TranFuncProc_H_INCLUDED_

//////////////////////////////////////////////////////////////////////////////
// Attributes
public:

//////////////////////////////////////////////////////////////////////////////
// Operations
public:

	int	fnAPPF_TransactionInitialize();                                             // 거래초기화
	int	fnAPPF_TransactionMenuProcedure();                                          // 메뉴선택
	int	fnAPPF_TransactionGetTransaction();                                         // 선택거래가져오기
	int	fnAPPF_TransactionGetMenuDoing();                                           // 연속거래여부

	int	fnAPPF_SetProcedureCount1();                                                // 진행카운트설정1
	int	fnAPPF_SetProcedureCount3();                                                // 진행카운트설정3
	int	fnAPPF_SetProcedureCount5();                                                // 진행카운트설정5
	int	fnAPPF_SetProcedureCount6();                                                // 진행카운트설정6
	int	fnAPPF_SetProcedureCount9();                                                // 진행카운트설정9
	int	fnAPPF_SendHost();                                                          // 호스트송신
	int	fnAPPF_RecvHost();                                                          // 호스트수신
	int	fnAPPF_SendRecvSnod();                                                      // 호스트송수신
	int	fnAPPF_GetHostResult();                                                     // 호스트결과얻기
	int	fnAPPF_CardImprint();                                                       // 카드임프린트

	int	fnAPPF_TransactionWithdrawInitialize();                                     // 출금거래초기화
	int	fnAPPF_TransactionWithdrawInput();                                          // 출금송신전입력
	int	fnAPPF_TransactionWithdrawOutput();                                         // 출금수신후처리
	int	fnAPPF_TransactionWithdrawCheckDeviceAction();                              // 출금거래종료검사
	int	fnAPPF_TransactionWithdrawCheckMenuContinue();                              // 출금연속여부검사

	int	fnAPPF_TransactionInquiryInitialize();                                      // 조회거래초기화
	int	fnAPPF_TransactionInquiryInput();                                           // 조회송신전입력
	int	fnAPPF_TransactionInquiryOutput();                                          // 조회수신후처리
	int	fnAPPF_TransactionInquiryCheckDeviceAction();                               // 조회거래종료검사
	int	fnAPPF_TransactionInquiryCheckMenuContinue();                               // 조회연속여부검사

	int	fnAPPF_TransactionTransferInitialize();                                     // 송금거래초기화
	int	fnAPPF_TransactionTransferInput();                                          // 송금송신전입력
	int	fnAPPF_TransactionTransferOutput();                                         // 송금수신후처리
	int	fnAPPF_TransactionTransferCheckDeviceAction();                              // 송금거래종료검사
	int	fnAPPF_TransactionTransferCheckMenuContinue();                              // 송금연속여부검사

	int	fnAPPF_TransactionDepositInitialize();                                      // 입금거래초기화
	int	fnAPPF_TransactionDepositInput();                                           // 입금송신전입력
	int	fnAPPF_TransactionDepositOutput();                                          // 입금수신후처리
	int	fnAPPF_TransactionDepositCheckDeviceAction();                               // 입금거래종료검사
	int	fnAPPF_TransactionDepositCheckMenuContinue();                               // 입금연속여부검사

	int	fnAPPF_TransactionPassbookInitialize();                                     // 통장정리거래초기화
	int	fnAPPF_TransactionPassbookInput();                                          // 통장정리송신전입력
	int	fnAPPF_TransactionPassbookOutput();                                         // 통장정리수신후처리
	int	fnAPPF_TransactionPassbookCheckDeviceAction();                              // 통장정리거래종료검사
	int	fnAPPF_TransactionPassbookCheckMenuContinue();                              // 통장정리연속여부검사

	int	fnAPPF_TransactionTicketInitialize();                                       // 티켓발권거래초기화
	int	fnAPPF_TransactionTicketInput();                                            // 티켓발권송신전입력
	int	fnAPPF_TransactionTicketOutput();                                           // 티켓발권수신후처리
	int	fnAPPF_TransactionTicketCheckDeviceAction();                                // 티켓발권거래종료검사
	int	fnAPPF_TransactionTicketCheckMenuContinue();                                // 티켓발권연속여부검사

	int	fnAPPF_TransactionNoMediaInitialize();                                      // 무매체거래초기화
	int	fnAPPF_TransactionNoMediaInput();                                           // 무매체송신전입력
	int	fnAPPF_TransactionNoMediaOutput();                                          // 무매체수신후처리
	int	fnAPPF_TransactionNoMediaCheckDeviceAction();                               // 무매체거래종료검사
	int	fnAPPF_TransactionNoMediaCheckMenuContinue();                               // 무매체연속여부검사

	int	fnAPPF_TransactionTranCommonInitialize();                                   // 공통거래부거래초기화
	int	fnAPPF_TransactionTranCommonInput();                                        // 공통거래부송신전입력
	int	fnAPPF_TransactionTranCommonOutput();                                       // 공통거래부수신후처리
	int	fnAPPF_TransactionTranCommonCheckDeviceAction();                            // 공통거래부거래종료검사
	int	fnAPPF_TransactionTranCommonCheckMenuContinue();                            // 공통거래부연속여부검사



/////////////////////////////////////////////////////////////////////////////
#endif
