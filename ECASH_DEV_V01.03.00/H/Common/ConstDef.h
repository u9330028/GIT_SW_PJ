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
// - �������� MWI ActiveX�� ���� ������ ��ġ���� ���� ��� Definition Header
//   file �Դϴ�.
// - ���ϳ����� ����� ����� ���� ����/����/����/�߰� ���� ������ �ʿ��� ���
//   �������� ���� �����ϴ� �� �̿ܿ� MWI����ڿ��� ������ �����Ͽ� �ֽʽÿ�.
// 
// - �������� ����Կ� �־� ������ ������ ������ ���� ����� �����մϴ�.
//   ���� : T1ATM With KALIGNITE
//                                                       [��ƿ����ȿ�� SE����]
//
// ----------------------------------------------------------------------------
// HISTORY OF [ConstDef.h] 
// ----------------------------------------------------------------------------
//
// - 2003.06.18	teo		MWI���� ����ϴ� ��(Return Value/Parameter)�� ��������
//						�ʱ� ���� �ۼ�

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
#define		DEV_NONE					0x00000000			// ��ġ����
#define		DEV_SPR						0x00000001			// ��ǥ��
#define		DEV_JPR						0x00000002			// ���������κ�
#define		DEV_MCU						0x00000004			// ī���
#define		DEV_BRM						0x00000008			// ��������ݺ�
#define		DEV_CSH						DEV_BRM			    // ��������ݺ�
#define		DEV_CAM						0x00000010			// ��ǥ����ݺ�
#define		DEV_UCM						DEV_CAM				// ��ǥ����ݺ�
#define		DEV_PBM						0x00000020			// �����
#define		DEV_SCR						0x00000040			// SCREEN : FLASH ACTIVEX
#define		DEV_CDU						0x00000080			// ��������ݺ�
#define		DEV_MCR						0x00000100			// MCR
#define		DEV_GRO						0x00000200			// GIRO
#define		DEV_A4P						0x00000400			// A4 Printer
#define		DEV_PIN						0x00000800			// PINPAD		: 20030930_1.1_0001 : Add Pinpad Activex
#define		DEV_DOR						0x00001000			// �޹�
#define		DEV_UPS						0x00002000			// ����������ġ
#define		DEV_SIU						0x00004000			// SENSOR & INDICATOR			
#define		DEV_SPL						0x00008000			// 7SEG/LED/INFORM/RPL
#define		DEV_LGT						0x00010000			// Guidelight
#define		DEV_CMR						0x00020000			// USB 2 CAMERA

#define		DEV_IRD						0x00040000			// ���ܼ�		// 20030930_1.0_1081
#define		DEV_FNG						0x00080000			// ����			// 20030930_1.0_1081

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
#define		DEVNM_NONE					("NONE")			// ��ġ����
#define		DEVNM_MCU					("MCU")				// ī���
#define		DEVNM_SPR					("SPR")				// ��ǥ��
#define		DEVNM_JPR					("JPR")				// ���������κ�
#define		DEVNM_PBM					("PBM")				// �����
//#define		DEVNM_A4P					("LASTPAGE")		// A4P
#define		DEVNM_BRM					("BRM")				// ��������ݺ�
#define		DEVNM_CDU					("CDU")				// ������ݺ�
#define		DEVNM_UCM					("UCM")				// ��ǥ����ݺ�
#define		DEVNM_DOR					("RDOOR")			// �޹�
#define		DEVNM_UPS					("UPS")				// ����������ġ
#define		DEVNM_SIU					("SIU")				// SENSOR & INDICATOR			
#define		DEVNM_SPL					("SPL")				// 7SEG/LED/INFORM/RPL
#define		DEVNM_NET					("NET")				// NETWORK
#define		DEVNM_CMR					("CAMERA")			// USB 2 CAMERA
#define		DEVNM_SCR					("SCR")				// SCREEN

#define		DEVNM_MCR					("MCR")				// MCR
#define		DEVNM_GRO					("GIRO")			// GIRO
#define		DEVNM_A4P					("A4PRINTER")		// A4 PRINTER
#define		DEVNM_PIN					("PINPAD")			// PINPAD	:	20030930_1.1_0001 : Add Pinpad Activex
#define		DEVNM_FNG					("FINGER")			// ������		20030930_1.0_1081
#define		DEVNM_IRD					("IRDA")			// ���ܼ���		20030930_1.0_1081
#define		DEVNM_DES					("DES")				// DES			2003.11.13
#define		DEVNM_BAR					("BARCODE")			// BarCode 		2003.11.20
#define		DEVNM_SNS					("SENSOR")			// Sensor		2006.04.12
#define		DEVNM_RFD					("RFD")				// RF Reader	2006.04.19

#define		DEVNM_MAIN					("MAIN")			// 
#define		DEVNM_ALL					("ALL")				// 
#define		MWI_BRMTYEP					("BRM_TYPE")		// T3ATM AP ���泻�� #04 - HBRM���� ���泻��
///////////////////////////////////////////////////////////////////////////////
// �Լ�ó����� RETURN VALUE
// ����̽� : -
///////////////////////////////////////////////////////////////////////////////
#define		R_NORMAL					0
#define		R_DOING						1
#define		R_ERROR						2
#define		R_TIMEOVER					3
#define		R_CANCEL					-1
#define		R_TIMEOUT					-2
#define		R_OVERACCEPT				-3


///////////////////////////////////////////////////////////////////////////////
// ��ġ ����̽� ���� ����
// ����̽� : JPR / SPR / MCU / PBM / BRM / UCM
///////////////////////////////////////////////////////////////////////////////
#define		NORMAL						0					// �������� 
#define		DOING						1					// ���� ������
#define		DOWN						2					// ��� ���
#define		NODEVICE					3					// ��� �̿���
#define		FATAL						4					// ��� FATAL

///////////////////////////////////////////////////////////////////////////////
// ���� ���� �ܷ�����
// ����̽� : JPR
///////////////////////////////////////////////////////////////////////////////
#define		JNL_NORMAL					0x00				// ���� ���� ����
#define		JNL_EMPTY_PAPER				0x01				// ���� ���� �亸��		// 20030925_1.0_1078
#define		JNL_LOW_END					0x02				// ���� ���� �ܷ���		// 20030925_1.0_1078


///////////////////////////////////////////////////////////////////////////////
// A4P ���� �ܷ�����
// ����̽� : A4P
///////////////////////////////////////////////////////////////////////////////
#define		A4P_NORMAL					0x00				// A4P ���� ����
#define		A4P_EMPTY_PAPER				0x01				// A4P ���� �亸��
#define		A4P_LOW_END					0x02				// A4P ���� �ܷ���

///////////////////////////////////////////////////////////////////////////////
// ��ü �ܷ�����
// ����̽� : JPR / SPR / MCU / PBM
///////////////////////////////////////////////////////////////////////////////
#define		ST_NOT_DETECT				0x00				// ��ü �̰���
#define		ST_SENSOR1					0x01				// �Ա���(���Ա�) ��ü����
#define		ST_SENSOR2					0x02				// ��λ� ��ü����


///////////////////////////////////////////////////////////////////////////////
// ��ǥ ���� �ܷ�����
// ����̽� : SPR
///////////////////////////////////////////////////////////////////////////////
#define		SLIP_NORMAL					0x00				// ��ǥ ���� ����
#define		SLIP_EMPTY_PAPER			0x01				// ��ǥ ���� �亸��
#define		SLIP_LOW_END				0x02				// ��ǥ ���� �ܷ���


/////////////////////////////////////////////////////////////////////////////
// ��ǥ MediaStatus����										V02-06-02-#04
/////////////////////////////////////////////////////////////////////////////
#define		SLIP_MEDIA_NOTPRESENT		0x00
#define 	SLIP_MEDIA_INJAWS			0x01
#define 	SLIP_MEDIA_PRESENT			0x02

// ������ 1�� ���� (T3ATM AP ���泻�� #13)
// PBM INK���¿� ���� ����� �߰�
#define		PBM_INK_FULL				("FULL")			// iPBM Ink���� : FULL 
#define		PBM_INK_LOW					("LOW")				// iPBM Ink���� : LOW
#define		PBM_INK_OUT					("OUT")				// iPBM Ink���� : OUT
#define		PBM_INK_NOTSET				("NOTSET")			// iPBM Ink���� : NOTSET

#define		PBMINK_NORMAL				0x00				// iPBM INK NORMAL : ��������üũ�� ���
#define		PBMINK_LOW					0x01				// iPBM INK NORMAL : ��������üũ�� ���
#define		PBMINK_OUT					0x02				// iPBM INK NORMAL : ��������üũ�� ���
///////////////////////////////////////////////////////////////////////////////
// ��ġ����̽� ����Ź ����
// ����̽� : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		NORMAL_POS					0					// ������ġ
#define		ABNORMAL_POS				1					// Ż��
#define		RECOVERING_POS				2					// 53301������	// V05-05-01-#01

///////////////////////////////////////////////////////////////////////////////
// ��ġ����̽� ī��Ʈ ��������
// ����̽� : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		CST_NORMAL					0					// ����
#define		CST_NEAR					1					// cassette near end
#define		CST_EMPTY					2					// cassette empty
#define		CST_SET_NG					3					// SET NG
#define		CST_FULL					4					// cassette full
#define		RB_ALREADY_FULL				5					// recycle box full


///////////////////////////////////////////////////////////////////////////////
// ��ǥ�� �κ���ġ ��������
// ����̽� : UCM
///////////////////////////////////////////////////////////////////////////////
#define		UCM_DEPOSIT					0x01				// UCM-��ǥ�Աݺ�
#define		UCM_WITHDRAW				0x02				// UCM-��ǥ��ݺ�


///////////////////////////////////////////////////////////////////////////////
// ��ݰ��� ���� 
// ����̽� : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		WITH_NOT_AVAILABLE			0					// ��� �Ұ���
#define		WITH_CASH_ONLY				1					// ������ ��� ����
#define		WITH_CHECK_ONLY				2					// ��ǥ�� ��� ����
#define		WITH_BOTH_AVAILABLE			3					// ���� + ��ǥ ��� ����
#define		WITH_TEN_TH					4					// ������ ����	// V06-01-01-#01
#define		WITH_FIFTY_TH				5					// 5������ ����	// V06-01-01-#01

///////////////////////////////////////////////////////////////////////////////
// ��ݰ��� ���� 
// ����̽� : CDU 
///////////////////////////////////////////////////////////////////////////////
#define		WITH_NOT_AVAILABLE			0						// ��� �Ұ���
#define		WITH_TYPE1_ONLY				0x0001					// 1������ ��� ����
#define		WITH_TYPE2_ONLY				0x0010					// 2������ ��� ����
#define		WITH_TYPE3_ONLY				0x0100					// 3������ ��� ����
#define		WITH_TYPE4_ONLY				0x1000					// 4������ ��� ����

///////////////////////////////////////////////////////////////////////////////
// �Աݰ��� ���� 
// ����̽� : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		DEP_NOT_AVAILABLE			0					// �Ա� �Ұ���
#define		DEP_CASH_ONLY				1					// ���ݸ� �Աݰ���
#define		DEP_CHECK_ONLY				2					// ��ǥ�� �Աݰ���
#define		DEP_BOTH_AVAILABLE			3					// ����+��ǥ �Աݰ���
#define		DEP_ONLY_TEN_TH				4					// �����Ǹ� �Աݰ���
#define		DEP_ONLY_FIFTY_TH			5					// 5������ �Աݰ���(RJ, ���� FULL)	// V06-01-01-#01
#define		DEP_TEN_TH					6					// 1����+õ,��õ(5���� �Ұ�)		// V06-01-01-#01
#define		DEP_FIFTY_TH				7					// 5����+õ,��õ(1���� �Ұ�)		// V06-01-01-#01

///////////////////////////////////////////////////////////////////////////////
// ��ġ ����̽� ����Ż�� ����
// ����̽� : BRM
///////////////////////////////////////////////////////////////////////////////
#define		NO_CHANGE					0					// ����Ż������
#define		CHANGE_POS					1					// ����Ż��


///////////////////////////////////////////////////////////////////////////////
// �ڵ�����/ȸ�� ����
// ����̽� : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		NEED_NONE					0					// �״��
#define		NEED_REFILL					1					// �����ʿ�
#define		NEED_TAKEUP					2					// ȸ���ʿ�
#define		NEED_TAKEUP_OLD				3					// ȸ���ʿ�
#define		NEED_TAKEUP_NEW				4					// ȸ���ʿ�
#define		NEED_REFILL50000			5					// V06-01-01-#01
#define		NEED_TAKEUP50000			6					// V06-01-01-#01

///////////////////////////////////////////////////////////////////////////////
// ���� �ܷ� ����
// ����̽� : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		BRM_CLEAR					0x00				// ���� ����
#define		BRM_IN_CSM					0x01				// ���� ���Ա��� ���� ����
#define		BRM_IN_RJSD					0x02				// ��ȯ ����Ŀ�� ���� ����
#define		BRM_IN_NSTK					0x04				// �Ͻ� ����Ŀ�� ���� ����
#define		BRM_ON_PATH					0x08				// ���� �̵� ��λ� ���� ����
#define		IN_CSM						BRM_IN_CSM
#define		IN_RJSD						BRM_IN_RJSD
#define		IN_NSTK						BRM_IN_NSTK
#define		IN_PATH						BRM_ON_PATH

#define		BRM_IN_CSM_STACK			0x02				// ���� ���Ա��� ���� ����
///////////////////////////////////////////////////////////////////////////////
// ��ǥ �ܷ� ����
// ����̽� : UCM
///////////////////////////////////////////////////////////////////////////////
#define		UCM_WITH_CLEAR				0x00				// ��� ���� ����
#define		UCM_WITH_IN_NSTK			0x01				// ��� �Ͻ� ����Ŀ�� ���� ����		: 2004.01.09 - ������
#define		UCM_WITH_IN_CSM				0x10				// ��� ��ǥ ���ⱸ�� ���� ����		: 2004.01.09 - ������
#define		UCM_WITH_ON_PATH			0x0e				// ��� ���� �̵� ��λ� ���� ����

#define		UCM_DEP_CLEAR				0x00				// �Ա� ���� ����
#define		UCM_DEP_IN_CSM				0x01				// �Ա� ��ǥ ���Ա��� ���� ����
#define		UCM_DEP_IN_NSTK				0x20				// �Ա� �Ͻ� ����Ŀ�� ���� ����
#define		UCM_DEP_ON_PATH				0x1e				// �Ա� ���� �̵� ��λ� ���� ����


///////////////////////////////////////////////////////////////////////////////
// ���� �Աݻ��� ����
// ����̽� : BRM 
// ���     : 0~2�� ���� [��ġ ����̽� ���� ����]�� ����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
#define		NO_MONEY					3					// ����� ���� ����
#define		SKEW_NOTE					4					// ����� SKEW �߻�
#define		MAX_OVER					5					// �Ա� �ִ� �ż� �ʰ�
#define		UNKNOWN_MONEY				6					// �������� ���� ���� ����


#define		ERR_MONEY					90					// ���ڻ�ǰ�� �Ա� �ŷ��� �ݾ� ����  #N0142
#define		OVER_MONEY					91					// ���ڻ�ǰ�� �Ա� �ŷ��� �ݾ� �ʰ� ����  #N0142
#define		UNDER_MONEY					92					// ���ڻ�ǰ�� �Ա� �ŷ��� �ݾ� �̴� ����  #N0142


///////////////////////////////////////////////////////////////////////////////
// ���� ������� ����
// ����̽� : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		GOT_MONEY					0					// ���� ����
#define		WAIT_USER					1					// ���� �̼���


///////////////////////////////////////////////////////////////////////////////
// �޹� �������� ����
// ����̽� : DOR [RearDoor]
///////////////////////////////////////////////////////////////////////////////
#define		DOOR_OPENED					FALSE				// �޹�����
#define		DOOR_CLOSED					TRUE				// �޹�����

///////////////////////////////////////////////////////////////////////////////
// ī�޶� ����
// ����̽� : CAMERA
///////////////////////////////////////////////////////////////////////////////
#define		CAMERA_OFF					0					// ī�޶� ������
#define		CAMERA_ON					1					// ī�޶� ����


///////////////////////////////////////////////////////////////////////////////
// FLICKER INDEX ����
// ����̽� : LGT [LIGNT-FLICKER]
///////////////////////////////////////////////////////////////////////////////
#define		BRM_FLICKER					0x01				// ���� �Ա�/���ⱸ
#define		PBM_FLICKER					0x02				// ���� ����/���ⱸ
#define		SPR_FLICKER					0x04				// ��ǥ ���ⱸ
#define		MCU_FLICKER					0x04				// ī�� ����/���ⱸ(��ǥ�͵���)
#define		IC_FLICKER					0x04				// IC ī�� ����/���ⱸ
#define		UCM_FLICKER					0x08				// ��ǥ �Աݱ�
#define		A4P_FLICKER					0x80				// A4Printer ���ⱸ
#define		CDU_FLICKER					0x11				// CDU ������ⱸ					�߱������û(09->11) : 2004.04.09
#define		PIN_FLICKER					0x20				// PIN �Է�(NC3000)					�߱������û(10->20) : 2004.04.09
#define		ALL_FLICKER					(BRM_FLICKER | PBM_FLICKER | SPR_FLICKER |	\
										 MCU_FLICKER | IC_FLICKER  | UCM_FLICKER |	\
										 A4P_FLICKER |  CDU_FLICKER | PIN_FLICKER)
															// ��� FLICKER


///////////////////////////////////////////////////////////////////////////////
// FLICKER INDEX ����
///////////////////////////////////////////////////////////////////////////////
// Kalignite, Ubiator (T1ATM, T2ATM, GIRO)
#define		KU_BRM_ATM_FLICKER					0x0001				// ���� �Ա�/���ⱸ
#define		KU_PBM_ATM_FLICKER					0x0002				// ���� ����/���ⱸ
#define		KU_SPR_ATM_FLICKER					0x0004				// ��ǥ(+ī��)���ⱸ

#ifdef T3ATM_MODE
	#define		KU_MCU_ATM_FLICKER				0x0020				// ī��(+��ǥ)���ⱸ(T3 Exception : Gi.Jin 2009.07.21) => Mwi : V10.03.02 (2010.02.11)
#else	// T3ATM_MODE
	#define		KU_MCU_ATM_FLICKER				0x0004				// ī��(+��ǥ)���ⱸ
#endif


//#define		KU_MCU_ATM_FLICKER					0x0004				// ī��(+��ǥ)���ⱸ
#define		KU_UCM_ATM_FLICKER					0x0008				// ��ǥ����ݺ�
#define		KU_PIN_ATM_FLICKER					0x1000              // #0143
 
#define		KU_ALL_FLICKER						(KU_BRM_ATM_FLICKER | KU_PBM_ATM_FLICKER | KU_SPR_ATM_FLICKER |	\
												 KU_MCU_ATM_FLICKER |KU_UCM_ATM_FLICKER | KU_PIN_ATM_FLICKER) //#0143
															// ��� FLICKER

#define		KU_SPR_GRO_FLICKER					0x0010				// ����SPR
#define		KU_MCU_GRO_FLICKER					0x0020				// ����MCU
#define		KU_A4P_GRO_FLICKER					0x0040				// A4Printer
#define		KU_ICS_GRO_FLICKER					0x0080				// ICS(�ź�����ĳ��) V04.03.01-#2

#define		KU_GROU_GRO_FLICKER					0x0100				// �������Ա� V04.03.01-#2
#define		KU_GROD_GRO_FLICKER					0x0200				// ���ι��ⱸ V04.03.01-#2
#define		KU_PBM_GRO_FLICKER					0x0400				// ��������� V04.03.01-#2
#define		KU_A4P_TOP_FLICKER					0x0800				// A4Printer(Topper Type) V04.03.04-#1

// Nextware (����ǥ����, ����Sidecar��)
#define		NX_BRM_FLICKER						0x0001				// ���� �Ա�/���ⱸ
#define		NX_PBM_FLICKER						0x0002				// ���� ����/���ⱸ
#define		NX_SPR_FLICKER						0x0004				// ��ǥ ���ⱸ
#define		NX_MCU_FLICKER						0x0008				// ī�� ����/���ⱸ(��ǥ�͵���)	�߱������û(04->08) : 2004.04.09
#define		NX_IC_FLICKER						0x0008				// IC ī�� ����/���ⱸ				�߱������û(04->08) : 2004.04.09
#define		NX_CDU_FLICKER						0x0011				// CDU ������ⱸ					�߱������û(09->11) : 2004.04.09
#define		NX_PIN_FLICKER						0x0020				// PIN �Է�(NC3000)					�߱������û(10->20) : 2004.04.09
#define		NX_BCD_FLICKER						0x0040				// BARCODE 
#define		NX_WBA_FLICKER		    			0x0080				// BILL ACCEPTOR
#define		NX_CDP_FLICKER						0x0100				// CARD DISPENSER					
#define		NX_CSM_FLICKER						0x0200				// CHECK SCANER
#define		NX_CDP1_FLICKER  					0x0400				// CARD1 DISPENSER    // 2007.06.04 KJK ADD CDP1 FLICKER
#define		NX_CDP2_FLICKER						0x0800				// CARD2 DISPENSER    // 2007.06.04 KJK ADD CDP1 FLICKER
#define		NX_CDP3_FLICKER						0x1000				// CARD3 DISPENSER    // 2007.06.04 KJK ADD CDP1 FLICKER					
#define		NX_DEP_FLICKER	


///////////////////////////////////////////////////////////////////////////////
// FLICKER ���� ����
// ����̽� : LGT [LIGNT-FLICKER]
///////////////////////////////////////////////////////////////////////////////
#define		FLICKER_OFF					0					// FLICKER OFF
#define		FLICKER_ON					1					// FLICKER ON


///////////////////////////////////////////////////////////////////////////////
// INDICATOR(�ŷ�ǥ�õ�) ���� ����
// ����̽� : LGT [LIGNT-INDICATOR]
///////////////////////////////////////////////////////////////////////////////
#define		IND_TRANENABLE				0x01				// �ŷ�����	��
#define		IND_TRANREADY				0x02				// �غ���(�ŷ��Ұ�)	��
#define		IND_CASH					0x04				// ����	��
#define		IND_CHECK		 			0x08				// ��ǥ	��
#define		IND_DEPOSIT		 			0x10				// �Ա�	��
#define		IND_WITHDRAW	 			0x20				// ���	��
#define		IND_TRANSFER	 			0x40				// ��ü	��
#define		IND_PASSBOOK	 			0x80				// ����	��
#define		IND_ALL						0xff				// ���ǥ�õ� ��δ�	��
#define		IND_TRANALL	 		 		(IND_ALL & ~IND_TRANENABLE & ~IND_TRANREADY)
															// �ŷ�����/�غ����� ������ ���ǥ�õ�	��
#define		IND_ALLOFF	 				0x00				// ���ǥ�õ� ��δ� ��
#define		IND_TRANALLOFF				(IND_TRANENABLE | IND_TRANREADY)
															// �ŷ�����/�غ����� ������ ���ǥ�õ� ��


///////////////////////////////////////////////////////////////////////////////
// SPL ��ġ ���� 
// ����̽� : SPL [7SEG/LED/RPL/INFORM]
// ���     : �ϴ��� ������ T1ATM�� �������� �ۼ��Ǿ���
///////////////////////////////////////////////////////////////////////////////
//       < SPL::LED >
//		��������������������������������������������������������������
//		��	  ���� ����   ��               ��� / �̻����          ��
//		��������������������������������������������������������������
//		��  �� �� ��[0]   ��    ī    ��[4]         �� �� ǥ[8]     ��
//		��  ��    ��[1]   ��    ��    ��[5]         ��    ��[9]     ��
//		��  ��    ��[2]   ��    ��    ��[6]         �Աݼ�ǥ[10]    ��
//		��  DUMMY   [3]   ��    ��ݼ�ǥ[7]         ȸ �� ��[11]    ��
//		��������������������������������������������������������������
//		
//       < SPL::RPL >
//		��������������������������������������������������������������
//		��	  INDEX       ��               CONTENTS                 ��
//		��������������������������������������������������������������
//		��       [0]      ��    NORMAL  - �����                    ��
//		��       [1]      ��    CASH    - ����                      �� 
//		��       [2]      ��    LOSS    - �н�                      ��
//		��       [3]      ��    ALARM   - �˶�                      ��
//		��       [4]      ��    RECEIPT - ��ǥ                    ��
//		��       [5]      ��    CALL    - ȣ��                      ��
//		��������������������������������������������������������������
//		  
//       < SPL::INFORM >
//		��������������������������������������������������������������
//		��	  INDEX       ��               CONTENTS                 ��
//		��������������������������������������������������������������
//		��       [0]      ��    LOSS    - �н�                      ��
//		��       [1]      ��    CASH    - ����                      �� 
//		��       [2]      ��    ALARM   - �˶�                      ��
//		��       [3]      ��    RECEIPT - ��ǥ                    ��
//		��������������������������������������������������������������
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// LED ���� ���� 
// ����̽� : LED
///////////////////////////////////////////////////////////////////////////////
#define		ON_USING_LED				1					// �����	(T1ATM��)
#define		ALERT_LED					2					// ���		(T1ATM��)
#define		ABNORMAL_LED				3					// �̻�		(T1ATM��)
#define		DUMMY_LED					4					// DUMMY	(T1ATM��)
#define		CARD_LED					5					// ī��		(T1ATM��)
#define		PASSBOOK_LED				6					// ����		(T1ATM��)
#define		CASH_LED					7					// ����		(T1ATM��)
#define		OUT_CHECK_LED				8					// ��ݼ�ǥ	(T1ATM��)
#define		SLIP_LED					9					// ��ǥ	(T1ATM��)
#define		JNL_LED						10					// ����		(T1ATM��)
#define		IN_CHECK_LED				11					// �Աݼ�ǥ	(T1ATM��)
#define		RJB_LED						12					// ȸ����	(T1ATM��)



///////////////////////////////////////////////////////////////////////////////
// LED ���� ���� 
// ����̽� : LED
///////////////////////////////////////////////////////////////////////////////
#define		LED_OFF						0					// LED OFF
#define		LED_ON						1					// LED ON


///////////////////////////////////////////////////////////////////////////////
// UPS ��ġ ���� ���� 
// ����̽� : UPS
///////////////////////////////////////////////////////////////////////////////
#define		POWER_NORMAL				0x00				// POWER����
#define		EXT_AC_OFF					0x01				// �����Ǵ�AC����
#define		INT_BATTERY_LOW				0x02				// �嵥��LOW
#define		CLERK_POWER_OFF				0x04				// �����������
#define		POWER_ABNORMAL				0x08				// UPS���ºҷ�
#define		POWER_DOWN_GOOD				0x10				// POWER DOWN ����ó��

////////////////////////////////////////////////////////////
//
// �Ʒ��� data�� 3 cassette�� PNC(7 segment, led)��
// RSPL(7 segment, led)�� �����ϱ� ����
// data���� define�� ���̴�.
//
////////////////////////////////////////////////////////////
#define		CHAR_NULL					32					// SEGMENT CHARACTER NULL
#define		CHAR_ERROR					33					// undefined character
#define		CHAR_DOT					34					// SEGMENT CHARACTER .

// �Ʒ��� ���ǵ� ��ũ�λ���� �̿��Ͽ�
// �����ϰ��� �ϴ� LED�� ��ȣ�� �����Ѵ�.
#define		POLL_LED					1					// ���� 		(CD��)
#define		WITHDRAW_LED				2					// ���			(CD��)
#define		DOING_TRAN_LED				3					// �ŷ���		(CD��)
#define		PSBK_TRAN_LED				4					// ���� �ŷ�	(CD��)

#define		SLIP_LOW_LED				5					// ��ǥ (�ܷ�)(CD��)
#define		JNL_LOW_LED					6					// ����   (�ܷ�)(CD��)
#define		CST1_LOW_LED				7					// ī��Ʈ1(�ܷ�)(CD��)
#define		CST2_LOW_LED				8					// ī��Ʈ2(�ܷ�)(CD��)
#define		CST3_LOW_LED				9					// ī��Ʈ3(�ܷ�)(CD��)

#define		CARD_MAT_LED				10					// ī��   (��ü)(CD��)
#define		SLIP_MAT_LED				11					// ��ǥ (��ü)(CD��)
#define		JNL_MAT_LED					12					// ����   (��ü)(CD��)
#define		PSBK_MAT_LED				13					// ����   (��ü)(CD��)
#define		NOTES_MAT_LED				14					// ����	  (��ü)(CD��)

#define		LOWER_KEY_LED				15					// �ϴ� KEY		(CD��)(���ݸż�)
#define		OPEN_CLOSE_KEY_LED			16					// ����		(CD��)(����)
#define		NOT_USE_KEY_LED				17					// ���Ұ�		(CD��)
#define		OVER_TIME_KEY_LED			18					// �ð��� ���	(CD��)
#define		CHECK_INFO_KEY_LED			19					// ��ǥ����		(CD��)(��ǥ�ż�)
#define		NUM_4_KEY_LED				20					// ���� KEY 4	(CD��)
#define		NUM_5_KEY_LED				21					// ���� KEY 5	(CD��)(����)
#define		NUM_6_KEY_LED				22					// ���� KEY 6	(CD��)(����)

#define		ON_USING_LED_R				23 					// �����		(RATM��)
#define		CARD_LED_R					24 					// ī��			(RATM��)
#define		PASSBOOK_LED_R				25					// ����			(RATM��)
#define		SLIP_LED_R					26 					// ��ǥ   	(RATM��)
#define		JNL_LED_R					27					// ����     	(RATM��)

#define		WITH_NOTES_LED				28					// ��� ����	(RATM��)(����)
#define		WITH_CHK_LED				29					// ��� ��ǥ	(RATM��)(��ݼ�ǥ)
#define		DEP_CHK_LED					30					// �Ա� ��ǥ	(RATM��)(�Աݼ�ǥ)
#define		DEP_NOTES_LED				31					// �Ա� ����	(RATM��)
#define		REJECT_LED					32					// ȸ����       (RATM��)

// �Ʒ��� ���ǵ� ��ũ�λ���� �̿��Ͽ�
// CD�� �����ϰ��� �ϴ� LED�� ���¸� �����Ѵ�.
#define		LED_OFF						0					// LED OFF
#define		LED_ON						1					// LED ON

// �Ʒ��� ���ǵ� ��ũ�λ���� �̿��Ͽ�
// RATM�� �����ϰ��� �ϴ� LED�� ���¸� �����Ѵ�.
#define		LED_NORMAL					0x10				// LED OFF
#define		LED_WARNING					0x20				// ���
#define		LED_ABNORMAL				0x40				// �̻�

// �Ʒ��� ���ǵ� ��ũ�λ���� �̿��Ͽ�
// CD, RATM�� �����ϰ��� �ϴ� DISPLAY LAMP�� ���¸� �����Ѵ�.
#define		LAMP_OFF					0					// LAMP OFF
#define		LAMP_ON						1					// LAMP ON


// �Ʒ��� ���ǵ� ��ũ�λ���� �̿��Ͽ�
// CD, RATM�� �����ϰ��� �ϴ� FLICKER�� ���¸� �����Ѵ�.
#define		FLICKER_OFF					0					// FLICKER OFF
#define		FLICKER_ON					1					// FLICKER ON

///////////////////////////////////////////////////////////////////////////////
// SCREEN ��ġ ��� ���� 
// ����̽� : SCR
///////////////////////////////////////////////////////////////////////////////
#define		SCR_SCREEN					("APCenterMovie")	// ȭ�����
#define		SCR_TOP						("APLocalMovie")	// ȭ���� ������
#define		SCR_TIME					("APMaxTime")		// �Է´��ð�
#define		SCR_STRING					("APValue")			// ���ڿ� ���
#define		SCR_BTNONOFF				("APState")			// ��ư ON-OFF�� ���
//#define		SCR_SCREEN					("tMovie")			// ȭ�����
//#define		SCR_TOP						("tTop")			// ȭ���� ������
//#define		SCR_TIME					("tMaxTime")		// �Է´��ð�
//#define		SCR_STRING					("tValue")			// ���ڿ� ���
//#define		SCR_BTNONOFF				("tState")			// ��ư ON-OFF�� ���
#define		SCR_PROGRESS				("APProgress")		// PROCESS=����,FINISH=����
#define		SCR_KIND					("kindATM")			// ��������

#define		PROG_PROCESS				("PROCESS")			// ���� ������
#define		PROG_FINISH					("FINISH")			// ���� ������

//Start
// #N0193 �����ATM Flash�� ������ ������ �׸� �߰�
#define		SCR_KINDMODE				("kindMode")		// ���÷��� ���� - ȭ��Ȯ��
#define		EAR_KINDMODE				("earphoneState")	// ������ ���� - �����θ�� (�ŷ�ȭ�鰨�߱� + PinPad���)
#define		REPLAY_MODE					("m_keyValue")		// ȭ�� �ٽ� Display - �����θ�忡�� TimeOut�� 
//End

#define		SCR_KINDBRAND				("KindBrand")		// �귣�庰 ī�� �̹��� ���� ����.#N0220
#define		SCR_KINDATM				("KindAtm")			// ��� Ÿ�� ����.#N0271

///////////////////////////////////////////////////////////////////////////////
// Ÿ�Ӿƿ� ������ �ð�����(�ʴ���)
// ����̽� : -
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
#define		CHK_NORMAL					("NORMAL")			// ����ó��
#define		CHK_TIMEOUT					("TIMEOUT")			// ��ü �̼���
#define		CHK_TIMEOVER				("TIMEOVER")		// ��ġ ������
#define		CHK_ERROR					("ERROR")			// ó���� ��ֹ߻�

#define		DEV_MCROMRON                1                   // Omron 				
#define		DEV_MCRSANKYO				0                   // SanKyo

///////////////////////////////////////////////////////////////////////////////
// SCANDEVICEACTION���� ������
///////////////////////////////////////////////////////////////////////////////
#define		EVENT_ALL					0					// ��ü�̺�Ʈ Ȯ��
#define		EVENT_IN					1					// ��ü�Է��̺�Ʈ Ȯ��
#define		EVENT_OUT					2					// ��ü�����̺�Ʈ Ȯ��
#define		EVENT_TAKEN					3					// ��ü�����̺�Ʈ Ȯ��	2004.07.13
#define		EVENT_ERROR					4					// ��ü����̺�Ʈ Ȯ��
#define		EVENT_CANCEL				5					// CancelRefill, CancelTakeup �̺�Ʈ Ȯ��
#define		EVENT_ICIN					6					// SanKyo Dip IC Ready #0010
///////////////////////////////////////////////////////////////////////////////
// SIU����
///////////////////////////////////////////////////////////////////////////////
#define		CLKEY_ON					1					// ���Ű����	
#define		CLKEY_OFF					0					// �Կ�Ű ����

///////////////////////////////////////////////////////////////////////////////
// ��ġ����̽� DISPENSER ��ġ����								
// ����̽� : CDU
// 2004.01.20_1.2_2003 
// 20040315 JUNXI CHINA CDU ����
///////////////////////////////////////////////////////////////////////////////
#define		DISPENSER_NOT_SET			0						// ������ݺξ���
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
// ��ġ����̽� ī��Ʈ ��������
// ����̽� : CDU
// 2004.01.19_1.2_2003 ��ġ�����߰�
///////////////////////////////////////////////////////////////////////////////
#define		CDU_CST_REJECT				0						// Reject CST
#define		CDU_CST_1					1						// 1st    CST
#define		CDU_CST_2					2						// 2nd    CST
#define		CDU_CST_3					3						// 3rd    CST
#define		CDU_CST_4					4						// 4th    CST		// 20040315 JUNXI CHINA CDU

///////////////////////////////////////////////////////////////////////////////
// ���� �ٱ��� ��������
// ����̽� : CDU
// �߱����� ����ȭ	2004.03.31 
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
#define		CDUDOOR_CABINET				0x0001					// ĳ�����(CD/CDP)
#define		CDUDOOR_SHIELD				0x0002					// �ݰ���(TTW)


///////////////////////////////////////////////////////////////////////////////
// PINPAD �Է¸�� ����
// ����̽� : PINPAD
// �߱����� ����ȭ	2004.04.02 
///////////////////////////////////////////////////////////////////////////////
#define		PINMODE_NOT_USE				0						// �̻��ó��
#define		PINMODE_NORMAL				1						// Normal �Է¸��
#define		PINMODE_EPP					2						// Epp �Է¸��

//////////////////////////////////////////////////////////////////////////
// for T2ATM : 2006.07.25
///////////////////////////////////////////////////////////////////////////////
// OPERATOR SWITCH���� ������
///////////////////////////////////////////////////////////////////////////////
#define		OPER_NONE					0					// NONE
#define		OPER_SUPERVISOR				1					// SUPERVISOR
#define		OPER_RUN					2					// RUN
#define		OPER_SUPERVISORCHANGED		11					// SUPERVISOR CHANGED
#define		OPER_RUNCHANGED				12					// RUN CHANGED

//////////////////////////////////////////////////////////////////////////
// for T2ATM : 2006.07.25
///////////////////////////////////////////////////////////////////////////////
// PROXIMITY(����) SENSOR���� ������
///////////////////////////////////////////////////////////////////////////////
#define		PROXIMITY_OFF				0
#define		PROXIMITY_ON				1

///////////////////////////////////////////////////////////////////////////////
// BRM ī��Ʈ ���� ������ : for T2ATM(OKI BRM) 2006.07.29
///////////////////////////////////////////////////////////////////////////////
#define		BRM_NOTEKIND_10000_OLD		0x30
#define		BRM_NOTEKIND_5000_OLD		0x31
#define		BRM_NOTEKIND_1000_OLD		0x32
#define		BRM_NOTEKIND_2000			0x34					// �űǿ�� ������ �����.(A����Ŀ���ش�, �������Ͻü���)
#define		BRM_NOTEKIND_10000_NEW		0x35
#define		BRM_NOTEKIND_5000_NEW		0x36
#define		BRM_NOTEKIND_1000_NEW		0x37
#define		BRM_NOTEKIND_INPUT_DEP_ONLY	0x4E
#define		BRM_NOTEKIND_DEP_OLD		0x4F					// V04-02-08-#02:COM07-14 : �Ա����� (����õ����, ��õ����)
#define		BRM_NOTEKIND_DEP_NEW		BRM_NOTEKIND_INPUT_DEP_ONLY			// V04-02-08-#02:COM07-14 : �Ա����� (�ſ�õ����, ��õ����)
#define		BRM_NOTEKIND_50000			BRM_NOTEKIND_2000					// V06-01-01-#01

#define		DEP_AVAIL_10000_OLD			0x0001
#define		DEP_AVAIL_5000_OLD			0x0002
#define		DEP_AVAIL_1000_OLD			0x0004
#define		DEP_AVAIL_10000_NEW			0x0010
#define		DEP_AVAIL_5000_NEW			0x0020
#define		DEP_AVAIL_1000_NEW			0x0040
#define		DEP_AVAIL_50000				0x0008					// V06-01-01-#01
//////////////////////////////////////////////////////////////////////////

#define		KOR_M10000_MAXOUT			300000					// �ִ���� 30����
#define		KOR_M10000_MAXOUT_30		300000					// �ִ���� 30����
#define		KOR_M10000_MAXOUT_50		500000					// �ִ���� 50����
#define		KOR_M10000_MAXOUT_70		700000					// �ִ���� 70����

#define		KOR_M10000_MAXOUTCNT_10		10						// �ִ���� 10����
#define		KOR_M10000_MAXOUTCNT_30		30						// �ִ���� 10����
#define		KOR_M10000_MAXOUTCNT_50		50						// �ִ���� 10����
#define		KOR_M10000_MAXOUTCNT_70		70						// �ִ���� 10����

#define		CASH_KOREA_50000			50000					// 10000����
#define		CASH_KOREA_10000			10000					// 10000����
#define		CASH_KOREA_5000				5000					// 5000����
#define		CASH_KOREA_1000				1000					// 1000����


#define CSH_MAXCASHOUTCNT	10						// MAX CASH OUT COUNT : 2005.10.28 
#define CSH_FIXCASHCNT		0						// FIX CASH COUNT : 2004.11.05 (KIM.G.J)
#define CSH_EMPTYCASHCNT	(CSH_MAXCASHOUTCNT + CSH_FIXCASHCNT)
//2006.03.06 (�������� Kim.G.J)						// EMPTY CASH COUNT : 2004.11.05
#define CSH_NEARCASHCNT		0						// NEAR CASH COUNT
#define CSH_FULLREJECTCNT	200						// FULL REJECT COUNT
#define CSH_FULLREJECTCNT2	100						// FULL REJECT COUNT(G-CDU:MCD4,TTW2)

#define	CARD_EXIST			1

#define		OS_WIN7						1						// Windows7

#define		WFS_ERR_HARDWARE_ERROR		-14						// V09-33-00-#01	ICCM ��� ����
#define		WFS_ERR_TIMEOUT				-48						// V09-33-00-#01	ICCM ��� ����


#endif // _CONST_DEF_H_
