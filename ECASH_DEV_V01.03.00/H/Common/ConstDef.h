#ifndef _CONST_DEF_H_
#define _CONST_DEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
//
//                                 CONSTDEF.H
//
// - 본파일은 MWI ActiveX로 부터 제공된 장치제어 관련 상수 Definition Header
//   file 입니다.
// - 파일내용중 선언된 상수에 대한 변경/수정/삭제/추가 등의 내용이 필요한 경우
//   본파일을 직접 수정하는 것 이외에 MWI담당자와의 협의후 변경하여 주십시오.
// 
// - 본파일을 사용함에 있어 문제는 없으나 다음과 같은 사양을 권장합니다.
//   기종 : T1ATM With KALIGNITE
//                                                       [노틸러스효성 SE본부]
//
// ----------------------------------------------------------------------------
// HISTORY OF [ConstDef.h] 
// ----------------------------------------------------------------------------
//
// - 2003.06.18	teo		MWI에서 사용하는 값(Return Value/Parameter)을 바탕으로
//						초기 파일 작성

// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// DEIVCE IDENTIFIER : ID
///////////////////////////////////////////////////////////////////////////////
#define		DEV_SPR_ID		0
#define		DEV_JPR_ID		1
#define		DEV_MCU_ID		2
#define		DEV_BRM_ID		3
#define		DEV_UCM_ID		4
#define		DEV_PBM_ID		5
#define		DEV_SCR_ID		6
#define		DEV_CDU_ID		7
#define		DEV_MCR_ID		8
#define		DEV_GRO_ID		9
#define		DEV_A4P_ID		10
#define		DEV_PIN_ID		11
#define		DEV_DOR_ID		12
#define		DEV_UPS_ID		13
#define		DEV_SIU_ID		14
#define		DEV_SPL_ID		15
#define		DEV_LGT_ID		16
#define		DEV_CMR_ID		17
#define		DEV_IRD_ID		18
#define		DEV_FNG_ID		19
#define		DEV_DES_ID		20
#define		DEV_BCD_ID		21
#define		DEV_VFD_ID		22
#define		DEV_SNS_ID		23
#define		DEV_DVR_ID		24
#define		DEV_RFD_ID		25
#define		DEV_MOP_ID		26
#define		DEV_DEP_ID		27
#define		DEV_CDP_ID		28
#define		DEV_CDP2_ID		29
#define		DEV_BILL_ID		30
#define		DEV_CSM_ID		31
#define		DEV_CDP3_ID		32
#define		DEV_TTU_ID		33
#define		DEV_ICS_ID		34
#define		DEV_ADA_ID		35
#define		DEV_CRD_ID		36
#define		DEV_COUNT		37
//
#define		DEV_NONE		0x00000000
#define		DEV_SPR			0x00000001
#define		DEV_JPR			0x00000002
#define		DEV_MCU			0x00000004
#define		DEV_BRM			0x00000008
#define		DEV_CSH			DEV_BRM
#define		DEV_UCM			0x00000010
#define		DEV_CAM			DEV_UCM
#define		DEV_PBM			0x00000020
#define		DEV_SCR			0x00000040
#define		DEV_CDU			0x00000080
#define		DEV_MCR			0x00000100
#define		DEV_GRO			0x00000200
#define		DEV_A4P			0x00000400
#define		DEV_PIN			0x00000800
#define		DEV_DOR			0x00001000
#define		DEV_UPS			0x00002000
#define		DEV_SIU			0x00004000
#define		DEV_SPL			0x00008000
#define		DEV_LGT			0x00010000
#define		DEV_CMR			0x00020000
#define		DEV_IRD			0x00040000
#define		DEV_FNG			0x00080000
#define		DEV_DES			0x00100000
#define		DEV_BCD			0x00200000
#define		DEV_BAR			DEV_BCD
#define		DEV_VFD			0x00400000
#define		DEV_SNS			0x00800000
#define		DEV_DVR			0x01000000
#define		DEV_RFD			0x02000000
//#define		DEV_MOP			0			#0067 
#define		DEV_ICM			0x04000000
#define		DEV_DEP			0
#define		DEV_CDP			0
#define		DEV_CDP2		0
#define		DEV_BILL		0
#define		DEV_CSM			0
#define		DEV_CDP3		0
#define		DEV_TTU			0
#define		DEV_ICS			0x08000000
#define		DEV_ADA			0
#define		DEV_CRD			0
#define		DEV_NITFNG		DEV_FNG
#define		DEV_1DBAR		DEV_BCD
#define		DEV_RDCTRL		DEV_IRD
#define		DEV_SPG			0x08000000
//
__declspec(selectany) long S_DEVICE_IDS[DEV_COUNT] = {
	/*	0.	DEV_SPR_ID*/	0x00000001,
	/*	1.	DEV_JPR_ID*/	0x00000002,
	/*	2.	DEV_MCU_ID*/	0x00000004,
	/*	3.	DEV_BRM_ID*/	0x00000008,
	/*	4.	DEV_UCM_ID*/	0x00000010,
	/*	5.	DEV_PBM_ID*/	0x00000020,
	/*	6.	DEV_SCR_ID*/	0x00000040,
	/*	7.	DEV_CDU_ID*/	0x00000080,
	/*	8.	DEV_MCR_ID*/	0x00000100,
	/*	9.	DEV_GRO_ID*/	0x00000200,
	/*	10.	DEV_A4P_ID*/	0x00000400,
	/*	11.	DEV_PIN_ID*/	0x00000800,
	/*	12.	DEV_DOR_ID*/	0x00001000,
	/*	13.	DEV_UPS_ID*/	0x00002000,
	/*	14.	DEV_SIU_ID*/	0x00004000,
	/*	15.	DEV_SPL_ID*/	0x00008000,
	/*	16.	DEV_LGT_ID*/	0x00010000,
	/*	17.	DEV_CMR_ID*/	0x00020000,
	/*	18.	DEV_IRD_ID*/	0x00040000,
	/*	19.	DEV_FNG_ID*/	0x00080000,
	/*	20.	DEV_DES_ID*/	0x00100000,
	/*	21.	DEV_BCD_ID*/	0x00200000,
	/*	22.	DEV_VFD_ID*/	0x00400000,
	/*	23.	DEV_SNS_ID*/	0x00800000,
	/*	24.	DEV_DVR_ID*/	0x01000000,
	/*	25.	DEV_RFD_ID*/	0x02000000,
//	/*	26.	DEV_ICM_ID*/	0,          #0067
	/*	26.	DEV_ICM_ID*/	0x04000000,
	/*	27.	DEV_DEP_ID*/	0,
	/*	28.	DEV_CDP_ID*/	0,
	/*	29.	DEV_CDP2_ID*/	0,
	/*	30.	DEV_BILL_ID*/	0,
	/*	31.	DEV_CSM_ID*/	0,
	/*	32.	DEV_CDP3_ID*/	0,
	/*	33.	DEV_TTU_ID*/	0,
	/*	34.	DEV_ICS_ID*/	0x08000000,
	/*	35.	DEV_ADA_ID*/	0,
	/*	36.	DEV_CRD_ID*/	0
	};


#define		DEV_MAIN					(DEV_MCU	| DEV_SPR	|  DEV_PBM	| DEV_CSH)

#define		DEV_ALL						(DEV_MCU	| DEV_SPR	| DEV_JPR	|	\
										 DEV_PBM	| DEV_BRM	| DEV_UCM	|	\
										 DEV_DOR	| DEV_UPS	| DEV_SIU	|	\
										 DEV_SPL	| DEV_CMR   | DEV_SCR	|	\
										 DEV_IRD	| DEV_FNG	| DEV_DES	|	\
										 DEV_BAR )


/*  #0067
#define		DEV_MAIN					(DEV_MCU	| DEV_SPR	|	\
										 DEV_PBM	| DEV_CSH	)

#define		DEV_ALL						(DEV_MCU	| DEV_SPR	| DEV_JPR	|	\
										 DEV_PBM	| DEV_BRM	| DEV_UCM	|	\
										 DEV_DOR	| DEV_UPS	| DEV_SIU	|	\
										 DEV_SPL	| DEV_CMR   | DEV_SCR	|	\
										 DEV_IRD	| DEV_FNG	| DEV_DES	|	\
										 DEV_BAR	)

*/
/*
#define		DEV_MAIN					(DEV_MCU	| DEV_SPR	| DEV_JPR	|	\
										 DEV_PBM	| DEV_CSH	| DEV_UCM	)

#define		DEV_ALL						(DEV_MCU	| DEV_SPR	| DEV_JPR	|	\
										 DEV_PBM	| DEV_BRM	| DEV_UCM	|	\
										 DEV_DOR	| DEV_UPS	| DEV_SIU	|	\
										 DEV_SPL	| DEV_CMR   | DEV_SCR	|	\
										 DEV_IRD	| DEV_FNG	| DEV_DES	|	\
										 DEV_BAR	)



*/

/*

///////////////////////////////////////////////////////////////////////////////
// DEIVCE IDENTIFIER : ID
///////////////////////////////////////////////////////////////////////////////
#define		DEV_NONE					0x00000000			// 장치없음
#define		DEV_SPR						0x00000001			// 명세표부
#define		DEV_JPR						0x00000002			// 페이퍼저널부
#define		DEV_MCU						0x00000004			// 카드부
#define		DEV_BRM						0x00000008			// 현금입출금부
#define		DEV_CSH						DEV_BRM			    // 현금입출금부
#define		DEV_CAM						0x00000010			// 수표입출금부
#define		DEV_UCM						DEV_CAM				// 수표입출금부
#define		DEV_PBM						0x00000020			// 통장부
#define		DEV_SCR						0x00000040			// SCREEN : FLASH ACTIVEX
#define		DEV_CDU						0x00000080			// 현금입출금부
#define		DEV_MCR						0x00000100			// MCR
#define		DEV_GRO						0x00000200			// GIRO
#define		DEV_A4P						0x00000400			// A4 Printer
#define		DEV_PIN						0x00000800			// PINPAD		: 20030930_1.1_0001 : Add Pinpad Activex
#define		DEV_DOR						0x00001000			// 뒷문
#define		DEV_UPS						0x00002000			// 보조전원장치
#define		DEV_SIU						0x00004000			// SENSOR & INDICATOR			
#define		DEV_SPL						0x00008000			// 7SEG/LED/INFORM/RPL
#define		DEV_LGT						0x00010000			// Guidelight
#define		DEV_CMR						0x00020000			// USB 2 CAMERA

#define		DEV_IRD						0x00040000			// 적외선		// 20030930_1.0_1081
#define		DEV_FNG						0x00080000			// 지문			// 20030930_1.0_1081

#define		DEV_DES						0x00100000			// DES			// 2003.11.13

#define		DEV_BAR						0x00200000			// BARCode Reader // 2003.11.20

#define		DEV_VFD						0x00400000			// VFD			// T2ATM 2006.5.24 yscho
#define		DEV_SNS						0x00800000			// Sensor		// T2ATM 2006.5.24 yscho
#define		DEV_DVR						0x01000000			// DVR 4 Camera	// T2ATM 2006.5.24 yscho
#define		DEV_RFD						0x02000000			// RF Reader	// T2ATM 2006.5.24 yscho

#define		DEV_MAIN					(DEV_MCU	| DEV_SPR	| DEV_JPR	|	\
										 DEV_PBM	| DEV_CSH	| DEV_UCM	|	\
										 DEV_A4P)

#define		DEV_ALL						(DEV_MCU	| DEV_SPR	| DEV_JPR	|	\
										 DEV_PBM	| DEV_BRM	| DEV_UCM	|	\
										 DEV_DOR	| DEV_UPS	| DEV_SIU	|	\
										 DEV_SPL	| DEV_CMR   | DEV_SCR	|	\
										 DEV_IRD	| DEV_FNG	| DEV_DES	|	\
										 DEV_BAR	| DEV_ALL)
*/
///////////////////////////////////////////////////////////////////////////////
// DEIVCE IDENTIFIER : NAME
///////////////////////////////////////////////////////////////////////////////
#define		DEVNM_NONE					("NONE")			// 장치없음
#define		DEVNM_MCU					("MCU")				// 카드부
#define		DEVNM_SPR					("SPR")				// 명세표부
#define		DEVNM_JPR					("JPR")				// 페이퍼저널부
#define		DEVNM_PBM					("PBM")				// 통장부
//#define		DEVNM_A4P					("LASTPAGE")		// A4P
#define		DEVNM_BRM					("BRM")				// 현금입출금부
#define		DEVNM_CDU					("CDU")				// 현금출금부
#define		DEVNM_UCM					("UCM")				// 수표입출금부
#define		DEVNM_DOR					("RDOOR")			// 뒷문
#define		DEVNM_UPS					("UPS")				// 보조전원장치
#define		DEVNM_SIU					("SIU")				// SENSOR & INDICATOR			
#define		DEVNM_SPL					("SPL")				// 7SEG/LED/INFORM/RPL
#define		DEVNM_NET					("NET")				// NETWORK
#define		DEVNM_CMR					("CAMERA")			// USB 2 CAMERA
#define		DEVNM_SCR					("SCR")				// SCREEN

#define		DEVNM_MCR					("MCR")				// MCR
#define		DEVNM_GRO					("GIRO")			// GIRO
#define		DEVNM_A4P					("A4PRINTER")		// A4 PRINTER
#define		DEVNM_PIN					("PINPAD")			// PINPAD	:	20030930_1.1_0001 : Add Pinpad Activex
#define		DEVNM_FNG					("FINGER")			// 지문부		20030930_1.0_1081
#define		DEVNM_IRD					("IRDA")			// 적외선부		20030930_1.0_1081
#define		DEVNM_DES					("DES")				// DES			2003.11.13
#define		DEVNM_BAR					("BARCODE")			// BarCode 		2003.11.20
#define		DEVNM_SNS					("SENSOR")			// Sensor		2006.04.12
#define		DEVNM_RFD					("RFD")				// RF Reader	2006.04.19

#define		DEVNM_MAIN					("MAIN")			// 
#define		DEVNM_ALL					("ALL")				// 
#define		MWI_BRMTYEP					("BRM_TYPE")		// T3ATM AP 변경내역 #04 - HBRM관련 변경내역
///////////////////////////////////////////////////////////////////////////////
// 함수처리결과 RETURN VALUE
// 디바이스 : -
///////////////////////////////////////////////////////////////////////////////
#define		R_NORMAL					0
#define		R_DOING						1
#define		R_ERROR						2
#define		R_TIMEOVER					3
#define		R_CANCEL					-1
#define		R_TIMEOUT					-2
#define		R_OVERACCEPT				-3


///////////////////////////////////////////////////////////////////////////////
// 장치 디바이스 상태 정보
// 디바이스 : JPR / SPR / MCU / PBM / BRM / UCM
///////////////////////////////////////////////////////////////////////////////
#define		NORMAL						0					// 정상종료 
#define		DOING						1					// 현재 동작중
#define		DOWN						2					// 기기 장애
#define		NODEVICE					3					// 기기 미연결
#define		FATAL						4					// 기기 FATAL

///////////////////////////////////////////////////////////////////////////////
// 저널 용지 잔량상태
// 디바이스 : JPR
///////////////////////////////////////////////////////////////////////////////
#define		JNL_NORMAL					0x00				// 저널 용지 정상
#define		JNL_EMPTY_PAPER				0x01				// 저널 용지 요보충		// 20030925_1.0_1078
#define		JNL_LOW_END					0x02				// 저널 용지 잔량소		// 20030925_1.0_1078


///////////////////////////////////////////////////////////////////////////////
// A4P 용지 잔량상태
// 디바이스 : A4P
///////////////////////////////////////////////////////////////////////////////
#define		A4P_NORMAL					0x00				// A4P 용지 정상
#define		A4P_EMPTY_PAPER				0x01				// A4P 용지 요보충
#define		A4P_LOW_END					0x02				// A4P 용지 잔량소

///////////////////////////////////////////////////////////////////////////////
// 매체 잔류상태
// 디바이스 : JPR / SPR / MCU / PBM
///////////////////////////////////////////////////////////////////////////////
#define		ST_NOT_DETECT				0x00				// 매체 미검지
#define		ST_SENSOR1					0x01				// 입구부(투입구) 매체검지
#define		ST_SENSOR2					0x02				// 경로상 매체검지


///////////////////////////////////////////////////////////////////////////////
// 명세표 용지 잔량상태
// 디바이스 : SPR
///////////////////////////////////////////////////////////////////////////////
#define		SLIP_NORMAL					0x00				// 명세표 용지 정상
#define		SLIP_EMPTY_PAPER			0x01				// 명세표 용지 요보충
#define		SLIP_LOW_END				0x02				// 명세표 용지 잔량소


/////////////////////////////////////////////////////////////////////////////
// 명세표 MediaStatus정보										V02-06-02-#04
/////////////////////////////////////////////////////////////////////////////
#define		SLIP_MEDIA_NOTPRESENT		0x00
#define 	SLIP_MEDIA_INJAWS			0x01
#define 	SLIP_MEDIA_PRESENT			0x02

// 적용후 1차 수정 (T3ATM AP 변경내역 #13)
// PBM INK상태에 대한 상수값 추가
#define		PBM_INK_FULL				("FULL")			// iPBM Ink상태 : FULL 
#define		PBM_INK_LOW					("LOW")				// iPBM Ink상태 : LOW
#define		PBM_INK_OUT					("OUT")				// iPBM Ink상태 : OUT
#define		PBM_INK_NOTSET				("NOTSET")			// iPBM Ink상태 : NOTSET

#define		PBMINK_NORMAL				0x00				// iPBM INK NORMAL : 상태전문체크시 사용
#define		PBMINK_LOW					0x01				// iPBM INK NORMAL : 상태전문체크시 사용
#define		PBMINK_OUT					0x02				// iPBM INK NORMAL : 상태전문체크시 사용
///////////////////////////////////////////////////////////////////////////////
// 장치디바이스 장찰탁 정보
// 디바이스 : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		NORMAL_POS					0					// 정상위치
#define		ABNORMAL_POS				1					// 탈착
#define		RECOVERING_POS				2					// 53301복구중	// V05-05-01-#01

///////////////////////////////////////////////////////////////////////////////
// 장치디바이스 카세트 상태정보
// 디바이스 : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		CST_NORMAL					0					// 정상
#define		CST_NEAR					1					// cassette near end
#define		CST_EMPTY					2					// cassette empty
#define		CST_SET_NG					3					// SET NG
#define		CST_FULL					4					// cassette full
#define		RB_ALREADY_FULL				5					// recycle box full


///////////////////////////////////////////////////////////////////////////////
// 수표부 부분장치 구분정보
// 디바이스 : UCM
///////////////////////////////////////////////////////////////////////////////
#define		UCM_DEPOSIT					0x01				// UCM-수표입금부
#define		UCM_WITHDRAW				0x02				// UCM-수표출금부


///////////////////////////////////////////////////////////////////////////////
// 출금가능 여부 
// 디바이스 : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		WITH_NOT_AVAILABLE			0					// 출금 불가능
#define		WITH_CASH_ONLY				1					// 만원만 출금 가능
#define		WITH_CHECK_ONLY				2					// 수표만 출금 가능
#define		WITH_BOTH_AVAILABLE			3					// 만원 + 수표 출금 가능
#define		WITH_TEN_TH					4					// 만원만 가능	// V06-01-01-#01
#define		WITH_FIFTY_TH				5					// 5만원만 가능	// V06-01-01-#01

///////////////////////////////////////////////////////////////////////////////
// 출금가능 여부 
// 디바이스 : CDU 
///////////////////////////////////////////////////////////////////////////////
#define		WITH_NOT_AVAILABLE			0						// 출금 불가능
#define		WITH_TYPE1_ONLY				0x0001					// 1권종만 출금 가능
#define		WITH_TYPE2_ONLY				0x0010					// 2권종만 출금 가능
#define		WITH_TYPE3_ONLY				0x0100					// 3권종만 출금 가능
#define		WITH_TYPE4_ONLY				0x1000					// 4권종만 출금 가능

///////////////////////////////////////////////////////////////////////////////
// 입금가능 여부 
// 디바이스 : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		DEP_NOT_AVAILABLE			0					// 입금 불가능
#define		DEP_CASH_ONLY				1					// 현금만 입금가능
#define		DEP_CHECK_ONLY				2					// 수표만 입금가능
#define		DEP_BOTH_AVAILABLE			3					// 현금+수표 입금가능
#define		DEP_ONLY_TEN_TH				4					// 만원권만 입금가능
#define		DEP_ONLY_FIFTY_TH			5					// 5만원만 입금가능(RJ, 만원 FULL)	// V06-01-01-#01
#define		DEP_TEN_TH					6					// 1만원+천,오천(5만원 불가)		// V06-01-01-#01
#define		DEP_FIFTY_TH				7					// 5만원+천,오천(1만원 불가)		// V06-01-01-#01

///////////////////////////////////////////////////////////////////////////////
// 장치 디바이스 이전탈착 정보
// 디바이스 : BRM
///////////////////////////////////////////////////////////////////////////////
#define		NO_CHANGE					0					// 이전탈착없음
#define		CHANGE_POS					1					// 이전탈착


///////////////////////////////////////////////////////////////////////////////
// 자동보충/회수 정보
// 디바이스 : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		NEED_NONE					0					// 그대로
#define		NEED_REFILL					1					// 보충필요
#define		NEED_TAKEUP					2					// 회수필요
#define		NEED_TAKEUP_OLD				3					// 회수필요
#define		NEED_TAKEUP_NEW				4					// 회수필요
#define		NEED_REFILL50000			5					// V06-01-01-#01
#define		NEED_TAKEUP50000			6					// V06-01-01-#01

///////////////////////////////////////////////////////////////////////////////
// 현금 잔류 정보
// 디바이스 : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		BRM_CLEAR					0x00				// 지폐 없음
#define		BRM_IN_CSM					0x01				// 현금 투입구에 지폐 있음
#define		BRM_IN_RJSD					0x02				// 반환 스태커에 지폐 있음
#define		BRM_IN_NSTK					0x04				// 일시 스태커에 지폐 있음
#define		BRM_ON_PATH					0x08				// 지폐 이동 경로상에 지폐 있음
#define		IN_CSM						BRM_IN_CSM
#define		IN_RJSD						BRM_IN_RJSD
#define		IN_NSTK						BRM_IN_NSTK
#define		IN_PATH						BRM_ON_PATH

#define		BRM_IN_CSM_STACK			0x02				// 현금 투입구에 지폐 있음
///////////////////////////////////////////////////////////////////////////////
// 수표 잔류 정보
// 디바이스 : UCM
///////////////////////////////////////////////////////////////////////////////
#define		UCM_WITH_CLEAR				0x00				// 출금 지폐 없음
#define		UCM_WITH_IN_NSTK			0x01				// 출금 일시 스태커에 지폐 있음		: 2004.01.09 - 값변경
#define		UCM_WITH_IN_CSM				0x10				// 출금 수표 방출구에 지폐 있음		: 2004.01.09 - 값변경
#define		UCM_WITH_ON_PATH			0x0e				// 출금 지폐 이동 경로상에 지폐 있음

#define		UCM_DEP_CLEAR				0x00				// 입금 지폐 없음
#define		UCM_DEP_IN_CSM				0x01				// 입금 수표 투입구에 지폐 있음
#define		UCM_DEP_IN_NSTK				0x20				// 입금 일시 스태커에 지폐 있음
#define		UCM_DEP_ON_PATH				0x1e				// 입금 지폐 이동 경로상에 지폐 있음


///////////////////////////////////////////////////////////////////////////////
// 현금 입금상태 정보
// 디바이스 : BRM 
// 비고     : 0~2의 값은 [장치 디바이스 상태 정보]를 사용한다.
///////////////////////////////////////////////////////////////////////////////
#define		NO_MONEY					3					// 계수할 지폐 없음
#define		SKEW_NOTE					4					// 계수중 SKEW 발생
#define		MAX_OVER					5					// 입금 최대 매수 초과
#define		UNKNOWN_MONEY				6					// 감별되지 않은 지폐 있음


#define		ERR_MONEY					90					// 전자상품권 입금 거래시 금액 오류  #N0142
#define		OVER_MONEY					91					// 전자상품권 입금 거래시 금액 초과 오류  #N0142
#define		UNDER_MONEY					92					// 전자상품권 입금 거래시 금액 미달 오류  #N0142


///////////////////////////////////////////////////////////////////////////////
// 현금 수취상태 정보
// 디바이스 : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		GOT_MONEY					0					// 현금 수취
#define		WAIT_USER					1					// 현금 미수취


///////////////////////////////////////////////////////////////////////////////
// 뒷문 열림상태 정보
// 디바이스 : DOR [RearDoor]
///////////////////////////////////////////////////////////////////////////////
#define		DOOR_OPENED					FALSE				// 뒷문열림
#define		DOOR_CLOSED					TRUE				// 뒷문닫힘

///////////////////////////////////////////////////////////////////////////////
// 카메라 정보
// 디바이스 : CAMERA
///////////////////////////////////////////////////////////////////////////////
#define		CAMERA_OFF					0					// 카메라 비정상
#define		CAMERA_ON					1					// 카메라 정상


///////////////////////////////////////////////////////////////////////////////
// FLICKER INDEX 정보
// 디바이스 : LGT [LIGNT-FLICKER]
///////////////////////////////////////////////////////////////////////////////
#define		BRM_FLICKER					0x01				// 지폐 입금/방출구
#define		PBM_FLICKER					0x02				// 통장 투입/방출구
#define		SPR_FLICKER					0x04				// 명세표 방출구
#define		MCU_FLICKER					0x04				// 카드 투입/방출구(명세표와동일)
#define		IC_FLICKER					0x04				// IC 카드 투입/방출구
#define		UCM_FLICKER					0x08				// 수표 입금구
#define		A4P_FLICKER					0x80				// A4Printer 방출구
#define		CDU_FLICKER					0x11				// CDU 지폐방출구					중국나노요청(09->11) : 2004.04.09
#define		PIN_FLICKER					0x20				// PIN 입력(NC3000)					중국나노요청(10->20) : 2004.04.09
#define		ALL_FLICKER					(BRM_FLICKER | PBM_FLICKER | SPR_FLICKER |	\
										 MCU_FLICKER | IC_FLICKER  | UCM_FLICKER |	\
										 A4P_FLICKER |  CDU_FLICKER | PIN_FLICKER)
															// 모든 FLICKER


///////////////////////////////////////////////////////////////////////////////
// FLICKER INDEX 정보
///////////////////////////////////////////////////////////////////////////////
// Kalignite, Ubiator (T1ATM, T2ATM, GIRO)
#define		KU_BRM_ATM_FLICKER					0x0001				// 지폐 입금/방출구
#define		KU_PBM_ATM_FLICKER					0x0002				// 통장 투입/방출구
#define		KU_SPR_ATM_FLICKER					0x0004				// 명세표(+카드)방출구

#ifdef T3ATM_MODE
	#define		KU_MCU_ATM_FLICKER				0x0020				// 카드(+명세표)방출구(T3 Exception : Gi.Jin 2009.07.21) => Mwi : V10.03.02 (2010.02.11)
#else	// T3ATM_MODE
	#define		KU_MCU_ATM_FLICKER				0x0004				// 카드(+명세표)방출구
#endif


//#define		KU_MCU_ATM_FLICKER					0x0004				// 카드(+명세표)방출구
#define		KU_UCM_ATM_FLICKER					0x0008				// 수표입출금부
#define		KU_PIN_ATM_FLICKER					0x1000              // #0143
 
#define		KU_ALL_FLICKER						(KU_BRM_ATM_FLICKER | KU_PBM_ATM_FLICKER | KU_SPR_ATM_FLICKER |	\
												 KU_MCU_ATM_FLICKER |KU_UCM_ATM_FLICKER | KU_PIN_ATM_FLICKER) //#0143
															// 모든 FLICKER

#define		KU_SPR_GRO_FLICKER					0x0010				// 지로SPR
#define		KU_MCU_GRO_FLICKER					0x0020				// 지로MCU
#define		KU_A4P_GRO_FLICKER					0x0040				// A4Printer
#define		KU_ICS_GRO_FLICKER					0x0080				// ICS(신분증스캐너) V04.03.01-#2

#define		KU_GROU_GRO_FLICKER					0x0100				// 지로투입구 V04.03.01-#2
#define		KU_GROD_GRO_FLICKER					0x0200				// 지로배출구 V04.03.01-#2
#define		KU_PBM_GRO_FLICKER					0x0400				// 지로통장부 V04.03.01-#2
#define		KU_A4P_TOP_FLICKER					0x0800				// A4Printer(Topper Type) V04.03.04-#1

// Nextware (수출표준판, 수출Sidecar판)
#define		NX_BRM_FLICKER						0x0001				// 지폐 입금/방출구
#define		NX_PBM_FLICKER						0x0002				// 통장 투입/방출구
#define		NX_SPR_FLICKER						0x0004				// 명세표 방출구
#define		NX_MCU_FLICKER						0x0008				// 카드 투입/방출구(명세표와동일)	중국나노요청(04->08) : 2004.04.09
#define		NX_IC_FLICKER						0x0008				// IC 카드 투입/방출구				중국나노요청(04->08) : 2004.04.09
#define		NX_CDU_FLICKER						0x0011				// CDU 지폐방출구					중국나노요청(09->11) : 2004.04.09
#define		NX_PIN_FLICKER						0x0020				// PIN 입력(NC3000)					중국나노요청(10->20) : 2004.04.09
#define		NX_BCD_FLICKER						0x0040				// BARCODE 
#define		NX_WBA_FLICKER		    			0x0080				// BILL ACCEPTOR
#define		NX_CDP_FLICKER						0x0100				// CARD DISPENSER					
#define		NX_CSM_FLICKER						0x0200				// CHECK SCANER
#define		NX_CDP1_FLICKER  					0x0400				// CARD1 DISPENSER    // 2007.06.04 KJK ADD CDP1 FLICKER
#define		NX_CDP2_FLICKER						0x0800				// CARD2 DISPENSER    // 2007.06.04 KJK ADD CDP1 FLICKER
#define		NX_CDP3_FLICKER						0x1000				// CARD3 DISPENSER    // 2007.06.04 KJK ADD CDP1 FLICKER					
#define		NX_DEP_FLICKER	


///////////////////////////////////////////////////////////////////////////////
// FLICKER 조작 정보
// 디바이스 : LGT [LIGNT-FLICKER]
///////////////////////////////////////////////////////////////////////////////
#define		FLICKER_OFF					0					// FLICKER OFF
#define		FLICKER_ON					1					// FLICKER ON


///////////////////////////////////////////////////////////////////////////////
// INDICATOR(거래표시등) 조작 정보
// 디바이스 : LGT [LIGNT-INDICATOR]
///////////////////////////////////////////////////////////////////////////////
#define		IND_TRANENABLE				0x01				// 거래가능	켬
#define		IND_TRANREADY				0x02				// 준비중(거래불가)	켬
#define		IND_CASH					0x04				// 현금	켬
#define		IND_CHECK		 			0x08				// 수표	켬
#define		IND_DEPOSIT		 			0x10				// 입금	켬
#define		IND_WITHDRAW	 			0x20				// 출금	켬
#define		IND_TRANSFER	 			0x40				// 이체	켬
#define		IND_PASSBOOK	 			0x80				// 통장	켬
#define		IND_ALL						0xff				// 취급표시등 모두다	켬
#define		IND_TRANALL	 		 		(IND_ALL & ~IND_TRANENABLE & ~IND_TRANREADY)
															// 거래가능/준비중을 제외한 취급표시등	켬
#define		IND_ALLOFF	 				0x00				// 취급표시등 모두다 끔
#define		IND_TRANALLOFF				(IND_TRANENABLE | IND_TRANREADY)
															// 거래가능/준비중을 제외한 취급표시등 끔


///////////////////////////////////////////////////////////////////////////////
// SPL 장치 정보 
// 디바이스 : SPL [7SEG/LED/RPL/INFORM]
// 비고     : 하단의 스펙은 T1ATM을 기준으로 작성되었음
///////////////////////////////////////////////////////////////////////////////
//       < SPL::LED >
//		┌────────┬────────────────────┐
//		│	  동작 상태   │               경고 / 이상부위          │
//		├────────┼────────────────────┤
//		│  취 급 중[0]   │    카    드[4]         명 세 표[8]     │
//		│  경    고[1]   │    통    장[5]         저    널[9]     │
//		│  이    상[2]   │    지    폐[6]         입금수표[10]    │
//		│  DUMMY   [3]   │    출금수표[7]         회 수 함[11]    │
//		└────────┴────────────────────┘
//		
//       < SPL::RPL >
//		┌────────┬────────────────────┐
//		│	  INDEX       │               CONTENTS                 │
//		├────────┼────────────────────┤
//		│       [0]      │    NORMAL  - 취급중                    │
//		│       [1]      │    CASH    - 현금                      │ 
//		│       [2]      │    LOSS    - 분실                      │
//		│       [3]      │    ALARM   - 알람                      │
//		│       [4]      │    RECEIPT - 명세표                    │
//		│       [5]      │    CALL    - 호출                      │
//		└────────┴────────────────────┘
//		  
//       < SPL::INFORM >
//		┌────────┬────────────────────┐
//		│	  INDEX       │               CONTENTS                 │
//		├────────┼────────────────────┤
//		│       [0]      │    LOSS    - 분실                      │
//		│       [1]      │    CASH    - 현금                      │ 
//		│       [2]      │    ALARM   - 알람                      │
//		│       [3]      │    RECEIPT - 명세표                    │
//		└────────┴────────────────────┘
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// LED 조작 정보 
// 디바이스 : LED
///////////////////////////////////////////////////////////////////////////////
#define		ON_USING_LED				1					// 취급중	(T1ATM용)
#define		ALERT_LED					2					// 경고		(T1ATM용)
#define		ABNORMAL_LED				3					// 이상		(T1ATM용)
#define		DUMMY_LED					4					// DUMMY	(T1ATM용)
#define		CARD_LED					5					// 카드		(T1ATM용)
#define		PASSBOOK_LED				6					// 통장		(T1ATM용)
#define		CASH_LED					7					// 지폐		(T1ATM용)
#define		OUT_CHECK_LED				8					// 출금수표	(T1ATM용)
#define		SLIP_LED					9					// 명세표	(T1ATM용)
#define		JNL_LED						10					// 저널		(T1ATM용)
#define		IN_CHECK_LED				11					// 입금수표	(T1ATM용)
#define		RJB_LED						12					// 회수함	(T1ATM용)



///////////////////////////////////////////////////////////////////////////////
// LED 조작 정보 
// 디바이스 : LED
///////////////////////////////////////////////////////////////////////////////
#define		LED_OFF						0					// LED OFF
#define		LED_ON						1					// LED ON


///////////////////////////////////////////////////////////////////////////////
// UPS 장치 상태 정보 
// 디바이스 : UPS
///////////////////////////////////////////////////////////////////////////////
#define		POWER_NORMAL				0x00				// POWER정상
#define		EXT_AC_OFF					0x01				// 정전또는AC오프
#define		INT_BATTERY_LOW				0x02				// 밧데리LOW
#define		CLERK_POWER_OFF				0x04				// 계원전원오프
#define		POWER_ABNORMAL				0x08				// UPS상태불량
#define		POWER_DOWN_GOOD				0x10				// POWER DOWN 정상처리

////////////////////////////////////////////////////////////
//
// 아래의 data는 3 cassette용 PNC(7 segment, led)와
// RSPL(7 segment, led)를 제어하기 위한
// data들을 define한 것이다.
//
////////////////////////////////////////////////////////////
#define		CHAR_NULL					32					// SEGMENT CHARACTER NULL
#define		CHAR_ERROR					33					// undefined character
#define		CHAR_DOT					34					// SEGMENT CHARACTER .

// 아래에 정의된 매크로상수를 이용하여
// 제어하고자 하는 LED의 번호를 지정한다.
#define		POLL_LED					1					// 교신 		(CD용)
#define		WITHDRAW_LED				2					// 출금			(CD용)
#define		DOING_TRAN_LED				3					// 거래중		(CD용)
#define		PSBK_TRAN_LED				4					// 통장 거래	(CD용)

#define		SLIP_LOW_LED				5					// 명세표 (잔량)(CD용)
#define		JNL_LOW_LED					6					// 저널   (잔량)(CD용)
#define		CST1_LOW_LED				7					// 카세트1(잔량)(CD용)
#define		CST2_LOW_LED				8					// 카세트2(잔량)(CD용)
#define		CST3_LOW_LED				9					// 카세트3(잔량)(CD용)

#define		CARD_MAT_LED				10					// 카드   (매체)(CD용)
#define		SLIP_MAT_LED				11					// 명세표 (매체)(CD용)
#define		JNL_MAT_LED					12					// 저널   (매체)(CD용)
#define		PSBK_MAT_LED				13					// 통장   (매체)(CD용)
#define		NOTES_MAT_LED				14					// 지폐	  (매체)(CD용)

#define		LOWER_KEY_LED				15					// 하단 KEY		(CD용)(현금매수)
#define		OPEN_CLOSE_KEY_LED			16					// 개폐국		(CD용)(개폐국)
#define		NOT_USE_KEY_LED				17					// 사용불가		(CD용)
#define		OVER_TIME_KEY_LED			18					// 시간외 사용	(CD용)
#define		CHECK_INFO_KEY_LED			19					// 수표정보		(CD용)(수표매수)
#define		NUM_4_KEY_LED				20					// 숫자 KEY 4	(CD용)
#define		NUM_5_KEY_LED				21					// 숫자 KEY 5	(CD용)(통장)
#define		NUM_6_KEY_LED				22					// 숫자 KEY 6	(CD용)(음성)

#define		ON_USING_LED_R				23 					// 취급중		(RATM용)
#define		CARD_LED_R					24 					// 카드			(RATM용)
#define		PASSBOOK_LED_R				25					// 통장			(RATM용)
#define		SLIP_LED_R					26 					// 명세표   	(RATM용)
#define		JNL_LED_R					27					// 저널     	(RATM용)

#define		WITH_NOTES_LED				28					// 출금 지폐	(RATM용)(지폐)
#define		WITH_CHK_LED				29					// 출금 수표	(RATM용)(출금수표)
#define		DEP_CHK_LED					30					// 입금 수표	(RATM용)(입금수표)
#define		DEP_NOTES_LED				31					// 입금 지폐	(RATM용)
#define		REJECT_LED					32					// 회수함       (RATM용)

// 아래에 정의된 매크로상수를 이용하여
// CD의 제어하고자 하는 LED의 상태를 지정한다.
#define		LED_OFF						0					// LED OFF
#define		LED_ON						1					// LED ON

// 아래에 정의된 매크로상수를 이용하여
// RATM의 제어하고자 하는 LED의 상태를 지정한다.
#define		LED_NORMAL					0x10				// LED OFF
#define		LED_WARNING					0x20				// 경고
#define		LED_ABNORMAL				0x40				// 이상

// 아래에 정의된 매크로상수를 이용하여
// CD, RATM의 제어하고자 하는 DISPLAY LAMP의 상태를 지정한다.
#define		LAMP_OFF					0					// LAMP OFF
#define		LAMP_ON						1					// LAMP ON


// 아래에 정의된 매크로상수를 이용하여
// CD, RATM의 제어하고자 하는 FLICKER의 상태를 지정한다.
#define		FLICKER_OFF					0					// FLICKER OFF
#define		FLICKER_ON					1					// FLICKER ON

///////////////////////////////////////////////////////////////////////////////
// SCREEN 장치 출력 정보 
// 디바이스 : SCR
///////////////////////////////////////////////////////////////////////////////
#define		SCR_SCREEN					("APCenterMovie")	// 화면출력
#define		SCR_TOP						("APLocalMovie")	// 화면상단 배너출력
#define		SCR_TIME					("APMaxTime")		// 입력대기시간
#define		SCR_STRING					("APValue")			// 문자열 출력
#define		SCR_BTNONOFF				("APState")			// 버튼 ON-OFF값 출력
//#define		SCR_SCREEN					("tMovie")			// 화면출력
//#define		SCR_TOP						("tTop")			// 화면상단 배너출력
//#define		SCR_TIME					("tMaxTime")		// 입력대기시간
//#define		SCR_STRING					("tValue")			// 문자열 출력
//#define		SCR_BTNONOFF				("tState")			// 버튼 ON-OFF값 출력
#define		SCR_PROGRESS				("APProgress")		// PROCESS=진행,FINISH=종료
#define		SCR_KIND					("kindATM")			// 기종구분

#define		PROG_PROCESS				("PROCESS")			// 진행 깜빡임
#define		PROG_FINISH					("FINISH")			// 종료 깜빡임

//Start
// #N0193 장애인ATM Flash에 전달할 데이터 항목 추가
#define		SCR_KINDMODE				("kindMode")		// 저시력자 세팅 - 화면확대
#define		EAR_KINDMODE				("earphoneState")	// 전맹인 세팅 - 전맹인모드 (거래화면감추기 + PinPad사용)
#define		REPLAY_MODE					("m_keyValue")		// 화면 다시 Display - 전맹인모드에서 TimeOut시 
//End

#define		SCR_KINDBRAND				("KindBrand")		// 브랜드별 카드 이미지 변경 변수.#N0220
#define		SCR_KINDATM				("KindAtm")			// 기기 타입 변경.#N0271

///////////////////////////////////////////////////////////////////////////////
// 타임아웃 설정용 시간정보(초단위)
// 디바이스 : -
///////////////////////////////////////////////////////////////////////////////
#define		K_NO_WAIT					0
#define		K_1_WAIT					1
#define		K_2_WAIT					2
#define		K_3_WAIT					3
#define		K_4_WAIT					4
#define		K_5_WAIT					5
#define		K_6_WAIT					6
#define		K_10_WAIT					10
#define		K_15_WAIT					15
#define		K_20_WAIT					20
#define		K_25_WAIT					25
#define		K_30_WAIT					30
#define		K_35_WAIT					35
#define		K_40_WAIT					40
#define		K_45_WAIT					45
#define		K_60_WAIT					60
#define		K_65_WAIT					65
#define		K_90_WAIT					90
#define		K_120_WAIT					120
#define		K_WAIT						-1

#define		IMG_1_CNT					1
#define		IMG_2_CNT					2
#define		IMG_3_CNT					3
#define		IMG_4_CNT					4
#define		IMG_5_CNT					5
#define		IMG_6_CNT					6
#define		IMG_7_CNT					7
#define		IMG_8_CNT					8
#define		IMG_9_CNT					9
#define		IMG_10_CNT					10
#define		IMG_11_CNT					11
#define		IMG_12_CNT					12
#define		IMG_13_CNT					13
#define		IMG_14_CNT					14
#define		IMG_15_CNT					15
#define		IMG_16_CNT					16
#define		IMG_17_CNT					17
#define		IMG_18_CNT					18
#define		IMG_19_CNT					19
#define		IMG_20_CNT					20
#define		IMG_21_CNT					21
#define		IMG_22_CNT					22
#define		IMG_23_CNT					23
#define		IMG_24_CNT					24
#define		IMG_25_CNT					25
#define		IMG_26_CNT					26
#define		IMG_27_CNT					27
#define		IMG_28_CNT					28
#define		IMG_29_CNT					29
#define		IMG_30_CNT					30
#define		IMG_31_CNT					31
#define		IMG_32_CNT					32
#define		IMG_33_CNT					33
#define		IMG_34_CNT					34
#define		IMG_35_CNT					35
#define		IMG_36_CNT					36
#define		IMG_37_CNT					37
#define		IMG_38_CNT					38
#define		IMG_39_CNT					39
#define		IMG_40_CNT					40
#define		IMG_41_CNT					41
#define		IMG_42_CNT					42
#define		IMG_43_CNT					43
#define		IMG_44_CNT					44
#define		IMG_45_CNT					45
#define		IMG_46_CNT					46
#define		IMG_47_CNT					47
#define		IMG_48_CNT					48
#define		IMG_49_CNT					49
#define		IMG_50_CNT					50
#define		IMG_51_CNT					51
#define		IMG_52_CNT					52
#define		IMG_53_CNT					53
#define		IMG_54_CNT					54
#define		IMG_55_CNT					55
#define		IMG_56_CNT					56
#define		IMG_57_CNT					57
#define		IMG_58_CNT					58
#define		IMG_59_CNT					59
#define		IMG_60_CNT					60

///////////////////////////////////////////////////////////////////////////////
// CHECKDEVICEACTION FUNCTION RETURN VALUE
///////////////////////////////////////////////////////////////////////////////
#define		CHK_NORMAL					("NORMAL")			// 정상처리
#define		CHK_TIMEOUT					("TIMEOUT")			// 매체 미수취
#define		CHK_TIMEOVER				("TIMEOVER")		// 장치 무응답
#define		CHK_ERROR					("ERROR")			// 처리중 장애발생

#define		DEV_MCROMRON                1                   // Omron 				
#define		DEV_MCRSANKYO				0                   // SanKyo

///////////////////////////////////////////////////////////////////////////////
// SCANDEVICEACTION설정 값관련
///////////////////////////////////////////////////////////////////////////////
#define		EVENT_ALL					0					// 전체이벤트 확인
#define		EVENT_IN					1					// 매체입력이벤트 확인
#define		EVENT_OUT					2					// 매체방출이벤트 확인
#define		EVENT_TAKEN					3					// 매체수취이벤트 확인	2004.07.13
#define		EVENT_ERROR					4					// 매체장애이벤트 확인
#define		EVENT_CANCEL				5					// CancelRefill, CancelTakeup 이벤트 확인
#define		EVENT_ICIN					6					// SanKyo Dip IC Ready #0010
///////////////////////////////////////////////////////////////////////////////
// SIU관련
///////////////////////////////////////////////////////////////////////////////
#define		CLKEY_ON					1					// 계원키누름	
#define		CLKEY_OFF					0					// 게원키 정상

///////////////////////////////////////////////////////////////////////////////
// 장치디바이스 DISPENSER 장치구분								
// 디바이스 : CDU
// 2004.01.20_1.2_2003 
// 20040315 JUNXI CHINA CDU 수정
///////////////////////////////////////////////////////////////////////////////
#define		DISPENSER_NOT_SET			0						// 현금출금부없음
#define		DISPENSER_4CASH				1						// 4 CST
#define		DISPENSER_3CASH				2						// 3 CST
#define		DISPENSER_2CASH				3						// 2 CST 
#define		DISPENSER_1CASH				4						// 1 CST 

#define		DISPENSER_3CASH1CHECK		5						// 4 CST
#define		DISPENSER_2CASH1CHECK		6						// 3 CST
#define		DISPENSER_1CASH1CHECK		7						// 2 CST 
#define		DISPENSER_1CHECK			8						// 1 CST 

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// 장치디바이스 카세트 상태정보
// 디바이스 : CDU
// 2004.01.19_1.2_2003 장치정보추가
///////////////////////////////////////////////////////////////////////////////
#define		CDU_CST_REJECT				0						// Reject CST
#define		CDU_CST_1					1						// 1st    CST
#define		CDU_CST_2					2						// 2nd    CST
#define		CDU_CST_3					3						// 3rd    CST
#define		CDU_CST_4					4						// 4th    CST		// 20040315 JUNXI CHINA CDU

///////////////////////////////////////////////////////////////////////////////
// 현금 다권종 권종정보
// 디바이스 : CDU
// 중국기종 현지화	2004.03.31 
///////////////////////////////////////////////////////////////////////////////
#define		CASH_TYPE1					0x0001
#define		CASH_TYPE2					0x0010
#define		CASH_TYPE3					0x0100
#define		CASH_TYPE4					0x1000

///////////////////////////////////////////////////////////////////////////////
// CDU DOOR TYPE
// V03-03-01
///////////////////////////////////////////////////////////////////////////////
#define		CDUDOOR_NOT_SET				0x0000
#define		CDUDOOR_CABINET				0x0001					// 캐비넷형(CD/CDP)
#define		CDUDOOR_SHIELD				0x0002					// 금고형(TTW)


///////////////////////////////////////////////////////////////////////////////
// PINPAD 입력모드 설정
// 디바이스 : PINPAD
// 중국기종 현지화	2004.04.02 
///////////////////////////////////////////////////////////////////////////////
#define		PINMODE_NOT_USE				0						// 미사용처리
#define		PINMODE_NORMAL				1						// Normal 입력모드
#define		PINMODE_EPP					2						// Epp 입력모드

//////////////////////////////////////////////////////////////////////////
// for T2ATM : 2006.07.25
///////////////////////////////////////////////////////////////////////////////
// OPERATOR SWITCH설정 값관련
///////////////////////////////////////////////////////////////////////////////
#define		OPER_NONE					0					// NONE
#define		OPER_SUPERVISOR				1					// SUPERVISOR
#define		OPER_RUN					2					// RUN
#define		OPER_SUPERVISORCHANGED		11					// SUPERVISOR CHANGED
#define		OPER_RUNCHANGED				12					// RUN CHANGED

//////////////////////////////////////////////////////////////////////////
// for T2ATM : 2006.07.25
///////////////////////////////////////////////////////////////////////////////
// PROXIMITY(근접) SENSOR설정 값관련
///////////////////////////////////////////////////////////////////////////////
#define		PROXIMITY_OFF				0
#define		PROXIMITY_ON				1

///////////////////////////////////////////////////////////////////////////////
// BRM 카세트 설정 값관련 : for T2ATM(OKI BRM) 2006.07.29
///////////////////////////////////////////////////////////////////////////////
#define		BRM_NOTEKIND_10000_OLD		0x30
#define		BRM_NOTEKIND_5000_OLD		0x31
#define		BRM_NOTEKIND_1000_OLD		0x32
#define		BRM_NOTEKIND_2000			0x34					// 신권운용 전까지 사용함.(A스태커만해당, 공장출하시설정)
#define		BRM_NOTEKIND_10000_NEW		0x35
#define		BRM_NOTEKIND_5000_NEW		0x36
#define		BRM_NOTEKIND_1000_NEW		0x37
#define		BRM_NOTEKIND_INPUT_DEP_ONLY	0x4E
#define		BRM_NOTEKIND_DEP_OLD		0x4F					// V04-02-08-#02:COM07-14 : 입금전용 (구오천원권, 구천원권)
#define		BRM_NOTEKIND_DEP_NEW		BRM_NOTEKIND_INPUT_DEP_ONLY			// V04-02-08-#02:COM07-14 : 입금전용 (신오천원권, 신천원권)
#define		BRM_NOTEKIND_50000			BRM_NOTEKIND_2000					// V06-01-01-#01

#define		DEP_AVAIL_10000_OLD			0x0001
#define		DEP_AVAIL_5000_OLD			0x0002
#define		DEP_AVAIL_1000_OLD			0x0004
#define		DEP_AVAIL_10000_NEW			0x0010
#define		DEP_AVAIL_5000_NEW			0x0020
#define		DEP_AVAIL_1000_NEW			0x0040
#define		DEP_AVAIL_50000				0x0008					// V06-01-01-#01
//////////////////////////////////////////////////////////////////////////

#define		KOR_M10000_MAXOUT			300000					// 최대출금 30만원
#define		KOR_M10000_MAXOUT_30		300000					// 최대출금 30만원
#define		KOR_M10000_MAXOUT_50		500000					// 최대출금 50만원
#define		KOR_M10000_MAXOUT_70		700000					// 최대출금 70만원

#define		KOR_M10000_MAXOUTCNT_10		10						// 최대출금 10만원
#define		KOR_M10000_MAXOUTCNT_30		30						// 최대출금 10만원
#define		KOR_M10000_MAXOUTCNT_50		50						// 최대출금 10만원
#define		KOR_M10000_MAXOUTCNT_70		70						// 최대출금 10만원

#define		CASH_KOREA_50000			50000					// 10000원권
#define		CASH_KOREA_10000			10000					// 10000원권
#define		CASH_KOREA_5000				5000					// 5000원권
#define		CASH_KOREA_1000				1000					// 1000원권


#define CSH_MAXCASHOUTCNT	10						// MAX CASH OUT COUNT : 2005.10.28 
#define CSH_FIXCASHCNT		0						// FIX CASH COUNT : 2004.11.05 (KIM.G.J)
#define CSH_EMPTYCASHCNT	(CSH_MAXCASHOUTCNT + CSH_FIXCASHCNT)
//2006.03.06 (순차방출 Kim.G.J)						// EMPTY CASH COUNT : 2004.11.05
#define CSH_NEARCASHCNT		0						// NEAR CASH COUNT
#define CSH_FULLREJECTCNT	200						// FULL REJECT COUNT
#define CSH_FULLREJECTCNT2	100						// FULL REJECT COUNT(G-CDU:MCD4,TTW2)

#define	CARD_EXIST			1

#define		OS_WIN7						1						// Windows7

#define		WFS_ERR_HARDWARE_ERROR		-14						// V09-33-00-#01	ICCM 모듈 적용
#define		WFS_ERR_TIMEOUT				-48						// V09-33-00-#01	ICCM 모듈 적용


#endif // _CONST_DEF_H_
