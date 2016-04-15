/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCmnMenu_H_)
#define _TranCmnMenu_H_

/////////////////////////////////////////////////////////////////////////////
//	ī��(����,Ÿ��,ISO2,ISO3)
//	����
//	����ü(ī��+�Էµ���)
/////////////////////////////////////////////////////////////////////////////
#define	MENU_ALL			(0xffffffff)
#define	MENU_NULL			(0x00000000)
#define	MENU_J2				(0x00000001)						// ����ſ�(2TR)
#define	MENU_T2				(0x00000002)						// Ÿ��ſ�(2TR)
#define	MENU_J3				(0x00000004)						// ��������(3TR)
#define	MENU_T3				(0x00000008)						// Ÿ������(3TR)
#define	MENU_S3				(0x00000010)						// ��������(3TR)
#define	MENU_D3				(0x00000020)						// Ÿ������(3TR)
#define	MENU_I3				(0x00000040)						// ���ຸ��(3TR)
#define	MENU_P3				(0x00000080)						// Ÿ�ຸ��(3TR)
#define	MENU_PB				(0x00010000)						// ����
#define	MENU_IC				(0x00020000)						// ����ȭ��
#define	MENU_MU				(0x10000000)						// ����ü
#define MENU_RF				(0x20000000)						// RFD
#define	MENU_INFO_1			(0x00040000)						// ����ȭ�� _ fnAPD_FICDistinctCardType
#define	MENU_INFO_2			(0x00080000)						// ����ȭ�� - fnAPD_FICBasicInquiry
#define	MENU_INFO_3			(0x00100000)						// ����ȭ�� - IC Button ���� Flag
#define	MENU_T3_NOT			(0x00000100)						// ����ī��(3TR)
#define	MENU_T2_NOT			(0x00000200)						// üũī��(3TR)

#define MENU_ONLY2			(0x40000000)						// (�������)

#define	MENU_J2T2			(MENU_J2 | MENU_T2)					// �ſ�(2TR)
#define	MENU_J3T3			(MENU_J3 | MENU_T3)					// ����(3TR)
#define	MENU_S3D3			(MENU_S3 | MENU_D3)					// ����(3TR)

#define MENU_MCU2			(MENU_J2T2)							// ī��(2TR)
#define MENU_MCU3			(MENU_J3T3 | MENU_IC)				// ī��(3TR)
#define MENU_MSMCU3			(MENU_J3T3)			             	// ī��(3TR) - MS-Only - #0127
#define MENU_MCUJ			(MENU_J3 | MENU_IC)					// ī��(3TR)
#define	MENU_MCU3PB			(MENU_MCU3 | MENU_PB)
#define	MENU_MCUJPB			(MENU_MCUJ | MENU_PB)
 
#define MENU_MCU			(MENU_MCU2 | MENU_MCU3 | MENU_IC)	// ī��
#define MENU_MCUJJ			(MENU_J2 | MENU_J3 | MENU_S3 | MENU_IC) 
																// ����ī��
#define MENU_MCUTT			(MENU_MCU & (~MENU_MCUJJ))			// Ÿ��ī��

#define MENU_JJ				(MENU_MCUJJ | MENU_PB | MENU_IC | MENU_MU) 
																// ����
#define MENU_TT				(MENU_MCUTT)						// Ÿ��

																// ��Ÿ(�޴����üӼ�)
#define	MENU_J3T3S3PBMU		(MENU_J3T3 | MENU_S3   | MENU_PB | MENU_MU)
																// ��������,������ü
#define	MENU_J3T3S3PB		(MENU_J3T3 | MENU_S3   | MENU_PB)
																// ��������,������ü(�ܱ���)
#define	MENU_J3T3S3MU		(MENU_J3T3 | MENU_S3   | MENU_MU)
																// ������ȸ
#define	MENU_J3T3S3			(MENU_J3T3 | MENU_S3)
																// ������ȸ(�ܱ���)
#define MENU_J3PBMU			(MENU_J3   | MENU_PB   | MENU_MU)	// ��й�ȣ,�۱�,�������
#define	MENU_J3S3PBMU		(MENU_J3   | MENU_S3   | MENU_PB | MENU_MU)
																// �Ա�
#define	MENU_J3S3PB			(MENU_J3   | MENU_S3   | MENU_PB)
																// �Ա�(�ܱ���)
#define	MENU_J3MU			(MENU_J3 | MENU_MU)
#define	MENU_J3PB			(MENU_J3 | MENU_PB)
#define	MENU_J3T3PB			(MENU_J3 | MENU_T3 | MENU_PB)
#define	MENU_J3T3PBMU		(MENU_J3T3PB | MENU_MU)
#define	MENU_J3S3			(MENU_J3 | MENU_S3)
																
/////////////////////////////////////////////////////////////////////////////
//  �����ŷ��� ���� ������ ������ ����.
//  ���� : �츮����(20)
//  ���� : T1ATM
/////////////////////////////////////////////////////////////////////////////
// �ŷ�����������
////�޴�����(0)			ī�弱���(1)
////						���弱���(2)
////						�ڵ��Է¼����(2)(1��ư��)
////							�Է¼����/�ŷ������(3)
////								�ŷ�ó���ڵ�(4)
////											�ŷ��ڵ�(5)
////														�ŷ��ڵ�2(6)
/////////////////////////////////////////////////////////////////////////////
//	MENU_NULL,			0,	0,	0,	TRAN_READY,	TC_READY,	0,	// 901:�غ�� 
//	MENU_NULL,			0,	0,	0,	TRAN_OPEN,	TC_OPEN,	0,	// 902:���� 
//	MENU_NULL,			0,	0,	0,	TRAN_TATUS,	TC_STATUS,	0,	// 903:���� 
//	MENU_NULL,			0,	0,	0,	TRAN_TOTAL,	TC_TOTAL,	0,	// 904:�հ� 
//
//
//
/////////////////////////////////////////////////////////////////////////////
////�޴�����(0)			ī�弱���(1)
////						���弱���(2)
////							�Է¼����/�ŷ������(3)
////								�ŷ�ó���ڵ�(4)
////											�ŷ��ڵ�(5)
////														�ŷ��ڵ�2(6)
/////////////////////////////////////////////////////////////////////////////
//	MENU_J3T3PB,		0,	0,	0,	TRAN_WITH,	TC_WITHDRAW,0,	// 1:����/��ǥ����
//	MENU_J3T3,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,	0,	// 2:�ܾ���ȸ
//	MENU_J3T3,			0,	0, ON,	TRAN_TRANS,	TC_TRANSFER,0,	// 3:������ü
//	MENU_J3,			0,	0,	0,	TRAN_TRANS,	TC_TFRCMS,	0,	// 4:CMS��ü
//	MENU_J3T3PB,		0,	0,	0,	TRAN_DEP,	TC_DEPOSIT,	0,	// 5:�Ա�
//	MENU_PB,			0,	0,	0,	TRAN_PB,	TC_PBOOK,	0,	// 6:��������
//	MENU_PB,			0,	0,	0,	TRAN_PB,	TC_PBOOKZIP,0,	// 7:��������
//	MENU_J2T2,			0,	0,	0,	TRAN_WITH,	TC_SVCWITH,	0,	// 8:��������
//	MENU_J2T2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,	0,	// 9:������ȸ
//	MENU_J2T2,			0,	0,	0,	TRAN_TRANS,	TC_SVCTRAN,	0,	//10:������ü
//	MENU_MU,			0,	0,  0,	TRAN_INQ,   TC_INQUIRYA,0,	//11:���ݰŷ������ȸ
//	MENU_MU, 			0,	0,	0,	TRAN_INQ,	TC_LOSTPB,	0,	//12:������Ű��� 
//	MENU_MU,			0,	0,	0,	TRAN_INQ,	TC_LOSTCARD,0,	//13:ī����Ű���
//	MENU_J3T3,			0,	0,  0,	TRAN_TRANS,	TC_JRTRAN,	0,	//14:���ΰ����ݳ���
//	MENU_J3T3,			0,	0,	0,	TRAN_INQ,	TC_JRINQ,	0,	//15:���γ��ΰ����ȸ
//	MENU_MU,			0,	0,	0,	TRAN_DEP,	TC_REMIT,	0,	//16:�۱�(�Ա�) 
//	MENU_J3PB,			0,	0,	0,	TRAN_INQ,	TC_AUTO,	0,	//17:�ڵ�������ü����

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Transaction Code Definition
#define		TC_WITHDRAW		1									// 1:�������
#define		TC_INQUIRY		2									// 2:������ȸ
#define		TC_TRANSFER		3									// 3:������ü
#define		TC_TFRCMS		4									// 4:CMS��ü
#define		TC_DEPOSIT		5									// 5:�Ա�
#define		TC_PBOOK		6									// 6:��������(�ŷ���������)
#define		TC_PBOOKZIP		7									// 7:��������(�ܰ�����)
#define		TC_SVCWITH		8									// 8:��������
#define		TC_SVCINQ		9									// 9:������ȸ
#define		TC_SVCTRAN		10									//10:������ü
#define		TC_INQUIRYA		11									//11:���ݱ����ȸ
#define		TC_LOSTPB		12									//12:������Ű�
#define		TC_LOSTCARD		13									//13:ī����Ű�
#define		TC_JRTRAN		14									//14:���ΰ����ݳ���
#define		TC_JRINQ		15									//15:���γ��ΰ����ȸ
#define		TC_REMIT		16									//16:�۱�(�������Ա�)
#define		TC_AUTO			17									//17:������Ա�
#define		TC_PBOOKTRM		18									//18:�����������(�Ⱓ����)
#define		TC_PBOOKRMNDR	19									//19:��������-�ܰ�����

#define		TC_TRANSTOC		21									//21:�������Ǵ�ü
#define		TC_REQSTOCK		22									//22:û���û
#define		TC_INQSTOCK		23									//23:û�೻����ȸ
#define		TC_CANSTOCK		24									//24:û�����

#define		TC_INQDEP		25									//25:��������ȸ
#define		TC_INQSECUR		26									//26:����������ȸ
#define		TC_INQWIDTH		27									//27:��ݰ��ɱݾ���ȸ
#define		TC_INQPRINT		28									//28:�ܰ����

#define		TC_KCLOAD		33									//33:����ȭ������
#define		TC_KCUNLOAD		34									//34:�ְ��·�ȯ��
#define		TC_KCPASSCH		35									//35:��й�ȣ����
#define		TC_KCREINQ		36									//36:K-CASH�ܾ���ȸ
#define		TC_KCLUINQ		37									//37:����/ȯ�ҳ���
#define		TC_KCBUYLST		38									//38:���ų�����ȸ
#define		TC_KCLOADRCV	39									//39:K-CASH��������
#define		TC_KCUNLOADRCV	40									//40:K-CASHȯ�Һ���
#define		TC_KCPARAKEY	41									//41:�Ķ���Ͱ���(Key)
#define		TC_KCPARALIMIT	42									//42:�Ķ���Ͱ���(�ִ������ѵ�)
#define		TC_KCPASSRE		43									//43:��й�ȣ���

#define		TC_FOREGIN		44									//44:�ؿ�ī�� �ŷ�
#define		TC_FIC			45									//45:IC����

#define		TC_TICKET		46									//46:Ƽ�Ϲ߱�
#define		TC_NOMEDIA		47									//47:����ü
#define		TC_NOMEDIA_HYUNCAPTIAL		48						//48:����ĳ��Ż(����ü)
#define		TC_UTILYBILLS	49		            				//50:�����ݾ���
#define		TC_LOAN_HYUNCARD	        50		            	//51:����ī���
#define		TC_LOAN_SHINCARD	        51		            	//52:����ī���
#define		TC_LOAN_LOTTCARD	        52		            	//53:�Ե�ī���
#define		TC_LOAN_SAMSCARD	        53		            	//54:�Ｚī���
#define		TC_CHECK_LOAN	55									//55:�ſ�����(��������)
#define		TC_OKCASHBAG	56									//56:OKĳ�������
#define		TC_GSNPOINT		57									//57:GS&POINT
#define		TC_CARDLOAN		58									//58:ī���
#define		TC_SVCINQ1		59									//59:��������ȸ
#define		TC_SVCTRAN1		60									//60:�������ü
#define		TC_INTERAD		62									//62:���ͷ�Ƽ�� ����
#define		TC_UTILYBILLS_01	63								//63:���漼 ����
#define		TC_UTILYBILLS_02	64								//64:���漼 Ÿ��
#define		TC_UTILYBILLS_03	65								//65:���漼 �Ǻ�

#define		TC_IBK_MANAGEMENT 	66								//66:IBK ȯ�����

#define		TC_E_TICKET_PAY 	67								//67:���ڻ�ǰ�� ����
#define		TC_E_TICKET_CANCEL 	68								//68:���ڻ�ǰ�� ���

#define		TC_EASYONE			69								//69:��ȯ ������ ����  //#N0174

#define		TC_MOBILEWITHDRAW	70								//70:���Ӵ� ���  //#N0182

#define		TC_HIPASS_BALANCE	71								//71:�����н� �ܾ���ȸ
#define		TC_HIPASS_CASH		72								//72:�����н� ��������
#define		TC_HIPASS_CREDIT	73								//73:�����н� �ſ�����
/***************************************************************************************/
#define		TC_CREDITITEM				100									//57:GS&POINT

//************************************************************************************************/
//* �Աݾ��� 
//************************************************************************************************/
#define		TC_MU_ALLINCE_SINHAN	101							//101:������
#define		TC_MU_ALLINCE_NH		102							//102:EveryDay Only Option , Option is setted by mannagemode

																// JiroTranCode
#define		TC_JRPHONE		201									//201:KT��ȭ���
#define		TC_JRELEC		202									//202:������
#define		TC_JRINSURE		203									//203:�ǰ������
#define		TC_JRPANS		204									//204:���ο���
#define		TC_JRJIRO		205									//205:���δ��
#define		TC_JRNTAX		206									//206:����/����
#define		TC_JRLTAX		207									//207:���漼/���ܼ���
#define		TC_JRWATER		208									//208:���ϼ������
#define		TC_JRTRAFF		209									//209:�����Ģ��
#define		TC_ELECINQ		210									// ���漼 �Ǻ�����

#define		TC_READY		901									//901:�غ��
#define		TC_OPEN			902									//902:����
#define		TC_STATUS		903									//903:����
#define		TC_ERROR		904									//904:���
#define		TC_TOTAL		905									//904:����
#define		TC_TOTALCANCEL	906									//905:�������
#define		TC_SUBTOTAL		907									//906:������ȸ
#define		TC_CASH_REG		908									//907:���ݵ��
#define		TC_CASH_UNREG	909									//908:��������
#define		TC_CHECK_REG	910									//909:��ǥ���
#define		TC_CHECK_UNREG	911									//910:��ǥ����



																// DES : 2003.12.12
#define		TC_TMKLOAD		1001								//1001:DES�ŷ� 1��-TMK
#define		TC_TPKLOAD		1002								//1002:DES�ŷ� 2��-TPK

																// DES OutGoing�ŷ� 2004.05.31
#define		TC_WITHCASHAD	1013								//1013:Credit Card Cash Advance
#define		TC_WITHSAVEAC	1014								//1014:Cash Withdrawal Saving Account
#define		TC_WITHCHKAC	1015								//1015:Cash Withdrawal Checking Account
#define		TC_INQCREDAC	1016								//1016:Balance Inquiry Credit Account
#define		TC_INQSAVEAC	1017								//1017:Balance Inquiry Saving Account
#define		TC_INQCHKAC		1018								//1018:Balance Inquiry Checking Account

/////////////////////////////////////////////////////////////////////////////
static MENUHeader MenuTblConst[] = 
{
////�޴�����(0)			ī�弱���(1)
////						���弱���(2)
////						�ڵ��Է¼����(2)(1��ư��)
////							�Է¼����/�ŷ������(3)
////								�ŷ�ó���ڵ�(4)
////											�ŷ��ڵ�(5)
////														�ŷ��ڵ�2(6)
/////////////////////////////////////////////////////////////////////////////
//// 0:����ȭ��
	"�ŷ�����",
	1,														// ȭ���ȣ
	"",
	"�Ա�",
	"���",
	"��ȸ",
	"��ü",
	"�ܱ�ī�����",									
	"���ݼ�����ȸ",
	"���ݼ�����ü",
	"",
	"",
	"�ٱ���",
	"",
	"",
	"",
	"�ؿ�ī��",
	"",
	"",
	"ȭ��Ȯ��",
	"",
	"",
	"",
	"",

	MENU_NULL,			1,	1,	0,	TRAN_NULL,	0,						0,				// 0:ó������
	MENU_MCU3,  		0,	0,	0,	TRAN_DEP,	TC_DEPOSIT,				TRANID_7200,	// 1:�Ա�
	MENU_MCU3,	    	0,	0,  0,	TRAN_WITH,	TC_WITHDRAW,			TRANID_4310,	// 2:���
	MENU_MCU3,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,				TRANID_7220,	// 3:��ȸ
	MENU_MCU3,  		0,	0,  0,	TRAN_TRANS,	TC_TRANSFER,			TRANID_7260,	// 4:��ü
	MENU_MCU2,			0,	0,	10,	TRAN_NULL,	0,						0,				// 5:���ݼ�������
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 6:���ݼ�����ȸ
//	MENU_MCU2,			0,	0,	0,	TRAN_TRANS,	TC_SVCTRAN1,			TRANID_7111,	// 7:���ݼ�����ü 
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0, 
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0,          	// 8:
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0,				// 09:
	MENU_MCU,			0,	0,	2,	TRAN_NULL,  0,						0,              // 10:�ٱ���
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,				// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,              // 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,				// 13:
	MENU_NULL,			0,	0,	6,	TRAN_NULL,	0,					    0,				// 14:�ؿ�ī��  
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,					    0,				// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,           	// 16:
	MENU_NULL,			0,	0,	11, TRAN_NULL,	0,						0,				// 17:ȭ��Ȯ�� (���÷���/�����)  
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,			    // 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,  0,				        0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:


//// 1:��ü ������ �ŷ����� ����ȭ��
	"�ŷ�����",
	2,														// ȭ���ȣ
	"",
	"�Ա�",
	"���",
	"��ȸ",
	"��ü",
	"�ܱ�ī�����",											
	"���ݼ�����ȸ",
	"���ݼ�����ü",
	"",
	"",
	"�ٱ���",
	"",
	"",
	"",
	"�ؿ�ī��",
	"",
	"",
	"ȭ��Ȯ��",
	"",
	"",
	"",
	"",

	MENU_NULL,			1,	1,	0,	TRAN_NULL,	0,						0,				// 0:ó������
	MENU_MCU3,			0,	0,	0,	TRAN_DEP,	TC_DEPOSIT,				TRANID_7200,	// 1:�Ա�
	MENU_MCU3,			0,	0,  0,	TRAN_WITH,	TC_WITHDRAW,			TRANID_4310,	// 2:���
	MENU_MCU3,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,				TRANID_7220,	// 3:��ȸ
	MENU_MCU3,			0,	0,  0,	TRAN_TRANS,	TC_TRANSFER,			TRANID_7260,	// 4:��ü
	MENU_MCU2,			0,	0,	10,	TRAN_NULL,	0,						0,				// 5:���ݼ�������
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 6:���ݼ�����ȸ
//	MENU_MCU2,			0,	0,	0,	TRAN_TRANS,	TC_SVCTRAN1,			TRANID_7250,	// 7:���ݼ�����ü  
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0, 
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0,          	// 8:
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0,				// 09:
	MENU_MCU,			0,	0,	2,	TRAN_NULL,  0,						0,              // 10:�ٱ���
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,				// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,              // 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,				// 13:
	MENU_NULL,			0,	0,	6,	TRAN_NULL,	0,					    0,				// 14:�ؿ�ī��  
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,					    0,				// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,           	// 16:
	MENU_NULL,			0,	0,	11,	TRAN_NULL,	0,						0,				// 17:ȭ��Ȯ�� (���÷���/�����)  
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,			    // 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,  0,				        0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:


// �ܱ������ ȭ�����.
//// 2:English/������/����(Global/Domestic����)
	"�ܱ���",
	103,																		// ȭ���ȣ	83 -> 38
	"",
	"��������",
	"�����Ϲ�",
	"�����߹�",
	"�ؿܿ���",
	"�ؿ��Ϲ�",
	"�ؿ��߹�",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_MCU,			0,	0,	4,	TRAN_NULL,	0,						0,	// 1:��������
	MENU_MCU,			0,	0,	4,	TRAN_NULL,	0,						0,	// 2:�����Ϻ���
	MENU_MCU,			0,	0,	4,	TRAN_NULL,	0,						0,	// 3:�����߱���
	MENU_MCU2,			0,	0,	3,	TRAN_NULL,	0,						0,	// 4:�ؿܿ���
	MENU_MCU2,			0,	0,	3,	TRAN_NULL,	0,						0,	// 5:�ؿ��Ϻ���
	MENU_MCU2,			0,	0,	3,	TRAN_NULL,	0,						0,	// 6:�ؿ��߱���
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:


/// 3:Global(English/������/����)
	"CARD_INSERT",
	801,														// ȭ���ȣ
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	MENU_NULL,			6,	0,	0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 1:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 2:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 3:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:


//// 4:Domestic(English/������/����)
	"�ܱ���",
	102,																		// ȭ���ȣ
	"",
	"�Ա�",
	"���",
	"��ȸ",
	"��ü",
	"���ݼ�������",
	"���ݼ�����ȸ",
	"��������",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_MCU3,	    	0,	0,	0,	TRAN_DEP,	TC_DEPOSIT,				TRANID_7200,	// 1:�Ա�
	MENU_MCU3,	    	0,	0,  0,	TRAN_WITH,	TC_WITHDRAW,			TRANID_4310,	// 2:���
	MENU_MCU3,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,				TRANID_7220,	// 3:��ȸ
	MENU_MCU3,	    	0,	0,  0,	TRAN_TRANS,	TC_TRANSFER,			TRANID_7260,	// 4:��ü
	MENU_MCU2,			0,	0,	0,	TRAN_WITH,	TC_SVCWITH,				TRANID_4330,	// 5:���ݼ�������
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 6:���ݼ�����ȸ
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:��������
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:



//// 5:����ȭ��
	"�ŷ�����",
	101,														// ȭ���ȣ
	"",
	"���ݼ�������",
	"���ݼ�����ȸ",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_MCU2,			0,	0,	0,	TRAN_WITH,	TC_SVCWITH,				TRANID_4330,	// 1:���ݼ�������
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 2:���ݼ�����ȸ
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 3:��ȸ
	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 4:��ü
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 5:���ݼ�������
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 6:���ݼ�����ȸ
	MENU_NULL,			0,	0,	0,	TRAN_NULL,  0,						0,  // 7:�ٱ���
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:


/// 6:Global(English/������/����)
////6:�ؿ�ī�弱�ý� ī�� ó��	
	"�ؿ�ī��",
	121,														// ȭ���ȣ
	"",
	"VISA_���",														// VISA				
	"MASTER_���",                                                     	// MASTER
	"JCB_���",                                                      	// JCP
	"CUP_���",                                                      	// CUP
	"CUP_��ȸ",                                                      	// CUP 
	"Other_���",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,	        TRANID_4330,	// 1:VISA ���
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,         	TRANID_4330,	// 2:MASTER ���
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,         	TRANID_4330,	// 3:JVB ���
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,         	TRANID_4330,	// 4:CUP ���
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN_INQ,  TC_FOREGIN,	 	TRANID_7230,	// 5:CUP ��ȸ
	MENU_MCU2,			0,	0,	7,	TRAN_NULL,	0,						0,				// 6:OTHERS
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:
	
//// 7:�ؿ�ī�� ���θ޴� #N0235
	"�ؿ�ī��",
	122,														// ȭ���ȣ
	"",
	"SAVINGACCOUNT",											// SAVINGSACCOUNT #N0171
	"CHECKINGACCOUNT",                                          // CHECKINGACCOUNT
	"CREDITCARD",											    // CREDITCARD
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_MCU2,			0,	0,	9,	TRAN_NULL,  0,					 	0,	// 1:SAVINGSACCOUNT #N0171
	MENU_MCU2,			0,	0,	9,	TRAN_NULL,  0,					  	0,	// 2:CHECKINGACCOUNT
	MENU_MCU2,			0,	0,	9,	TRAN_NULL,  0,					    0,	// 3:CREDITCARD
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:

	
//// 8:�ؿ�ī�� ���θ޴� #N0235
	"�ؿ�ī��",
	123,														// ȭ���ȣ
	"",
	"CASHADVANCE",											
	"BALANCEINQUIRY",                                          
	"",                                           
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,			TRANID_4330,	// 1:����
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN_INQ,   TC_FOREGIN,		TRANID_7230,	// 2:��ȸ
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 3:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:


//// 9:�ؿ�ī�� ���θ޴� #N0235
	"�ؿ�ī��",
	123,														// ȭ���ȣ
	"",
	"CASHWITHDRAWAL",
	"BALANCE",  
	"",                                           
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,			TRANID_4330,	// 1:����
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN_INQ, TC_FOREGIN,		TRANID_7230,	// 2:��ȸ
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 3:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:




	//// 10:�ſ�ī�� ���� �Ŵ�
	"�ſ�ī��",
	10,																// ȭ���ȣ
	"",
	"�ܱ�ī��������",											
	"�ܱ�ī�������ü",
	"�ܱ�ī�������ȸ",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_MCU2,			0,	0,	0,	TRAN_WITH,	TC_SVCWITH,				TRANID_4330,	// 1:���ݼ�������
//	MENU_MCU2,			0,	0,	0,	TRAN_TRANS,	TC_SVCTRAN1,			TRANID_7111,	// 2:���ݼ�����ü  #N0161
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 3:���ݼ�����ȸ
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:


//// 11:�ſ�ī�� ���� �Ŵ�
	"ȭ��Ȯ��",
	2,																// ȭ���ȣ
	"",
	"�������",
	"�Ա�",
	"������ü",
	"��������",
	"������ȸ",
	"�ܱ�ī�����",													//#N0247
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_MCU3,			0,	0,  0,	TRAN_WITH,	TC_WITHDRAW,			TRANID_4310,	// 1:���
	MENU_MCU3,			0,	0,	0,	TRAN_DEP,   TC_DEPOSIT,				TRANID_7200,    // 2:�Ա�
	MENU_MCU3,			0,	0,  0,	TRAN_TRANS,	TC_TRANSFER,			TRANID_7260,	// 3:��ü
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 4:��������
	MENU_MCU3,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,				TRANID_7220,	// 5:��ȸ
	MENU_NULL,			0,	0,	12,	TRAN_NULL,	0,						0,	// 6:�ſ�ī��
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:


	//// 12:���÷� �ſ�ī�� ���� �Ŵ�
	"�ſ�ī��",
	10,																// ȭ���ȣ
	"",
	"�ܱ�ī��������",
	"�ܱ�ī�������ü",
	"�ܱ�ī�������ȸ",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_MCU2,			0,	0,	0,	TRAN_WITH,	TC_SVCWITH,				TRANID_4330,	// 1:���ݼ�������
//	MENU_MCU2,			0,	0,	0,	TRAN_TRANS,	TC_SVCTRAN1,			TRANID_7250,	// 2:���ݼ�����ü  #N0161
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 3:���ݼ�����ȸ
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:


/// 13:Global(English/������/����) - �Ｚī��
////13:�ؿ�ī�弱�ý� ī�� ó��	#N0278
	"�ؿ�ī��",
	120,														// ȭ���ȣ
	"",
	"SAVINGACCOUNT",
	"CHECKINGACCOUNT",
	"CREDITCARD",
	"CUP_���",
	"CUP_��ȸ",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_MCU2,			0,	0,	9,	TRAN_NULL,  0,					 	0,	// 1:SAVINGSACCOUNT
	MENU_MCU2,			0,	0,	9,	TRAN_NULL,  0,					  	0,	// 2:CHECKINGACCOUNT
	MENU_MCU2,			0,	0,	9,	TRAN_NULL,  0,					    0,	// 3:CREDITCARD
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,         	TRANID_4330,	// 4:CUP ���
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN_INQ,  TC_FOREGIN,	 	TRANID_7230,	// 5:CUP ��ȸ
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:

//// N:������
	"",
	0,															// ȭ���ȣ
	"",
	"",
	"",  
	"",                                           
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 0:ó������
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 1:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 2:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 3:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 16:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 17:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:

};

/////////////////////////////////////////////////////////////////////////////
#endif

