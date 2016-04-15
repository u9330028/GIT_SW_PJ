/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCmnMenu_H_)
#define _TranCmnMenu_H_

/////////////////////////////////////////////////////////////////////////////
//	카드(자행,타행,ISO2,ISO3)
//	통장
//	무매체(카드+입력동시)
/////////////////////////////////////////////////////////////////////////////
#define	MENU_ALL			(0xffffffff)
#define	MENU_NULL			(0x00000000)
#define	MENU_J2				(0x00000001)						// 자행신용(2TR)
#define	MENU_T2				(0x00000002)						// 타행신용(2TR)
#define	MENU_J3				(0x00000004)						// 자행은행(3TR)
#define	MENU_T3				(0x00000008)						// 타행은행(3TR)
#define	MENU_S3				(0x00000010)						// 자행증권(3TR)
#define	MENU_D3				(0x00000020)						// 타행증권(3TR)
#define	MENU_I3				(0x00000040)						// 자행보험(3TR)
#define	MENU_P3				(0x00000080)						// 타행보험(3TR)
#define	MENU_PB				(0x00010000)						// 통장
#define	MENU_IC				(0x00020000)						// 전자화폐
#define	MENU_MU				(0x10000000)						// 무매체
#define MENU_RF				(0x20000000)						// RFD
#define	MENU_INFO_1			(0x00040000)						// 전자화폐 _ fnAPD_FICDistinctCardType
#define	MENU_INFO_2			(0x00080000)						// 전자화폐 - fnAPD_FICBasicInquiry
#define	MENU_INFO_3			(0x00100000)						// 전자화폐 - IC Button 선행 Flag
#define	MENU_T3_NOT			(0x00000100)						// 현금카드(3TR)
#define	MENU_T2_NOT			(0x00000200)						// 체크카드(3TR)

#define MENU_ONLY2			(0x40000000)						// (사원인증)

#define	MENU_J2T2			(MENU_J2 | MENU_T2)					// 신용(2TR)
#define	MENU_J3T3			(MENU_J3 | MENU_T3)					// 은행(3TR)
#define	MENU_S3D3			(MENU_S3 | MENU_D3)					// 증권(3TR)

#define MENU_MCU2			(MENU_J2T2)							// 카드(2TR)
#define MENU_MCU3			(MENU_J3T3 | MENU_IC)				// 카드(3TR)
#define MENU_MSMCU3			(MENU_J3T3)			             	// 카드(3TR) - MS-Only - #0127
#define MENU_MCUJ			(MENU_J3 | MENU_IC)					// 카드(3TR)
#define	MENU_MCU3PB			(MENU_MCU3 | MENU_PB)
#define	MENU_MCUJPB			(MENU_MCUJ | MENU_PB)
 
#define MENU_MCU			(MENU_MCU2 | MENU_MCU3 | MENU_IC)	// 카드
#define MENU_MCUJJ			(MENU_J2 | MENU_J3 | MENU_S3 | MENU_IC) 
																// 자행카드
#define MENU_MCUTT			(MENU_MCU & (~MENU_MCUJJ))			// 타행카드

#define MENU_JJ				(MENU_MCUJJ | MENU_PB | MENU_IC | MENU_MU) 
																// 자행
#define MENU_TT				(MENU_MCUTT)						// 타행

																// 기타(메뉴관련속성)
#define	MENU_J3T3S3PBMU		(MENU_J3T3 | MENU_S3   | MENU_PB | MENU_MU)
																// 예금지금,예금이체
#define	MENU_J3T3S3PB		(MENU_J3T3 | MENU_S3   | MENU_PB)
																// 예금지금,예금이체(외국어)
#define	MENU_J3T3S3MU		(MENU_J3T3 | MENU_S3   | MENU_MU)
																// 예금조회
#define	MENU_J3T3S3			(MENU_J3T3 | MENU_S3)
																// 예금조회(외국어)
#define MENU_J3PBMU			(MENU_J3   | MENU_PB   | MENU_MU)	// 비밀번호,송금,대출업무
#define	MENU_J3S3PBMU		(MENU_J3   | MENU_S3   | MENU_PB | MENU_MU)
																// 입금
#define	MENU_J3S3PB			(MENU_J3   | MENU_S3   | MENU_PB)
																// 입금(외국어)
#define	MENU_J3MU			(MENU_J3 | MENU_MU)
#define	MENU_J3PB			(MENU_J3 | MENU_PB)
#define	MENU_J3T3PB			(MENU_J3 | MENU_T3 | MENU_PB)
#define	MENU_J3T3PBMU		(MENU_J3T3PB | MENU_MU)
#define	MENU_J3S3			(MENU_J3 | MENU_S3)
																
/////////////////////////////////////////////////////////////////////////////
//  개별거래에 대한 정보는 다음과 같다.
//  유저 : 우리은행(20)
//  기종 : T1ATM
/////////////////////////////////////////////////////////////////////////////
// 거래종류별정보
////메뉴정보(0)			카드선행시(1)
////						통장선행시(2)
////						자동입력선행시(2)(1버튼시)
////							입력선행시/거래선행시(3)
////								거래처리코드(4)
////											거래코드(5)
////														거래코드2(6)
/////////////////////////////////////////////////////////////////////////////
//	MENU_NULL,			0,	0,	0,	TRAN_READY,	TC_READY,	0,	// 901:준비완 
//	MENU_NULL,			0,	0,	0,	TRAN_OPEN,	TC_OPEN,	0,	// 902:개국 
//	MENU_NULL,			0,	0,	0,	TRAN_TATUS,	TC_STATUS,	0,	// 903:상태 
//	MENU_NULL,			0,	0,	0,	TRAN_TOTAL,	TC_TOTAL,	0,	// 904:합계 
//
//
//
/////////////////////////////////////////////////////////////////////////////
////메뉴정보(0)			카드선행시(1)
////						통장선행시(2)
////							입력선행시/거래선행시(3)
////								거래처리코드(4)
////											거래코드(5)
////														거래코드2(6)
/////////////////////////////////////////////////////////////////////////////
//	MENU_J3T3PB,		0,	0,	0,	TRAN_WITH,	TC_WITHDRAW,0,	// 1:현금/수표지급
//	MENU_J3T3,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,	0,	// 2:잔액조회
//	MENU_J3T3,			0,	0, ON,	TRAN_TRANS,	TC_TRANSFER,0,	// 3:계좌이체
//	MENU_J3,			0,	0,	0,	TRAN_TRANS,	TC_TFRCMS,	0,	// 4:CMS이체
//	MENU_J3T3PB,		0,	0,	0,	TRAN_DEP,	TC_DEPOSIT,	0,	// 5:입금
//	MENU_PB,			0,	0,	0,	TRAN_PB,	TC_PBOOK,	0,	// 6:통장정리
//	MENU_PB,			0,	0,	0,	TRAN_PB,	TC_PBOOKZIP,0,	// 7:압축정리
//	MENU_J2T2,			0,	0,	0,	TRAN_WITH,	TC_SVCWITH,	0,	// 8:서비스지급
//	MENU_J2T2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,	0,	// 9:서비스조회
//	MENU_J2T2,			0,	0,	0,	TRAN_TRANS,	TC_SVCTRAN,	0,	//10:서비스이체
//	MENU_MU,			0,	0,  0,	TRAN_INQ,   TC_INQUIRYA,0,	//11:예금거래기록조회
//	MENU_MU, 			0,	0,	0,	TRAN_INQ,	TC_LOSTPB,	0,	//12:통장사고신고등록 
//	MENU_MU,			0,	0,	0,	TRAN_INQ,	TC_LOSTCARD,0,	//13:카드사고신고등록
//	MENU_J3T3,			0,	0,  0,	TRAN_TRANS,	TC_JRTRAN,	0,	//14:지로공과금납부
//	MENU_J3T3,			0,	0,	0,	TRAN_INQ,	TC_JRINQ,	0,	//15:지로납부결과조회
//	MENU_MU,			0,	0,	0,	TRAN_DEP,	TC_REMIT,	0,	//16:송금(입금) 
//	MENU_J3PB,			0,	0,	0,	TRAN_INQ,	TC_AUTO,	0,	//17:자동계좌이체설정

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Transaction Code Definition
#define		TC_WITHDRAW		1									// 1:예금출금
#define		TC_INQUIRY		2									// 2:예금조회
#define		TC_TRANSFER		3									// 3:계좌이체
#define		TC_TFRCMS		4									// 4:CMS이체
#define		TC_DEPOSIT		5									// 5:입금
#define		TC_PBOOK		6									// 6:통장정리(거래내역정리)
#define		TC_PBOOKZIP		7									// 7:압축정리(잔고정리)
#define		TC_SVCWITH		8									// 8:서비스지급
#define		TC_SVCINQ		9									// 9:서비스조회
#define		TC_SVCTRAN		10									//10:서비스이체
#define		TC_INQUIRYA		11									//11:예금기록조회
#define		TC_LOSTPB		12									//12:통장사고신고
#define		TC_LOSTCARD		13									//13:카드사고신고
#define		TC_JRTRAN		14									//14:지로공과금납부
#define		TC_JRINQ		15									//15:지로납부결과조회
#define		TC_REMIT		16									//16:송금(무통장입금)
#define		TC_AUTO			17									//17:매출금입금
#define		TC_PBOOKTRM		18									//18:통장단축정리(기간지정)
#define		TC_PBOOKRMNDR	19									//19:통장정리-잔고정리

#define		TC_TRANSTOC		21									//21:유가증권대체
#define		TC_REQSTOCK		22									//22:청약신청
#define		TC_INQSTOCK		23									//23:청약내역조회
#define		TC_CANSTOCK		24									//24:청약취소

#define		TC_INQDEP		25									//25:예수금조회
#define		TC_INQSECUR		26									//26:유가증권조회
#define		TC_INQWIDTH		27									//27:출금가능금액조회
#define		TC_INQPRINT		28									//28:잔고출력

#define		TC_KCLOAD		33									//33:전자화폐충전
#define		TC_KCUNLOAD		34									//34:주계좌로환불
#define		TC_KCPASSCH		35									//35:비밀번호변경
#define		TC_KCREINQ		36									//36:K-CASH잔액조회
#define		TC_KCLUINQ		37									//37:충전/환불내역
#define		TC_KCBUYLST		38									//38:구매내역조회
#define		TC_KCLOADRCV	39									//39:K-CASH충전복구
#define		TC_KCUNLOADRCV	40									//40:K-CASH환불복구
#define		TC_KCPARAKEY	41									//41:파라미터갱신(Key)
#define		TC_KCPARALIMIT	42									//42:파라미터갱신(최대저장한도)
#define		TC_KCPASSRE		43									//43:비밀번호등록

#define		TC_FOREGIN		44									//44:해외카드 거래
#define		TC_FIC			45									//45:IC업무

#define		TC_TICKET		46									//46:티켓발권
#define		TC_NOMEDIA		47									//47:무매체
#define		TC_NOMEDIA_HYUNCAPTIAL		48						//48:현대캐피탈(무매체)
#define		TC_UTILYBILLS	49		            				//50:공과금업무
#define		TC_LOAN_HYUNCARD	        50		            	//51:현대카드론
#define		TC_LOAN_SHINCARD	        51		            	//52:신한카드론
#define		TC_LOAN_LOTTCARD	        52		            	//53:롯데카드론
#define		TC_LOAN_SAMSCARD	        53		            	//54:삼성카드론
#define		TC_CHECK_LOAN	55									//55:신용정보(대출진단)
#define		TC_OKCASHBAG	56									//56:OK캐쉬백업무
#define		TC_GSNPOINT		57									//57:GS&POINT
#define		TC_CARDLOAN		58									//58:카드론
#define		TC_SVCINQ1		59									//59:결재대금조회
#define		TC_SVCTRAN1		60									//60:결대대금이체
#define		TC_INTERAD		62									//62:인터렉티브 광고
#define		TC_UTILYBILLS_01	63								//63:지방세 본인
#define		TC_UTILYBILLS_02	64								//64:지방세 타인
#define		TC_UTILYBILLS_03	65								//65:지방세 건별

#define		TC_IBK_MANAGEMENT 	66								//66:IBK 환경관리

#define		TC_E_TICKET_PAY 	67								//67:전자상품권 구매
#define		TC_E_TICKET_CANCEL 	68								//68:전자상품권 취소

#define		TC_EASYONE			69								//69:외환 이지원 서비스  //#N0174

#define		TC_MOBILEWITHDRAW	70								//70:원머니 출금  //#N0182

#define		TC_HIPASS_BALANCE	71								//71:하이패스 잔액조회
#define		TC_HIPASS_CASH		72								//72:하이패스 현금충전
#define		TC_HIPASS_CREDIT	73								//73:하이패스 신용충전
/***************************************************************************************/
#define		TC_CREDITITEM				100									//57:GS&POINT

//************************************************************************************************/
//* 입금업무 
//************************************************************************************************/
#define		TC_MU_ALLINCE_SINHAN	101							//101:편의점
#define		TC_MU_ALLINCE_NH		102							//102:EveryDay Only Option , Option is setted by mannagemode

																// JiroTranCode
#define		TC_JRPHONE		201									//201:KT전화요금
#define		TC_JRELEC		202									//202:전기요금
#define		TC_JRINSURE		203									//203:건강보험료
#define		TC_JRPANS		204									//204:국민연금
#define		TC_JRJIRO		205									//205:지로대금
#define		TC_JRNTAX		206									//206:국고/국세
#define		TC_JRLTAX		207									//207:지방세/세외수입
#define		TC_JRWATER		208									//208:상하수도요금
#define		TC_JRTRAFF		209									//209:교통범칙금
#define		TC_ELECINQ		210									// 지방세 건별납부

#define		TC_READY		901									//901:준비완
#define		TC_OPEN			902									//902:개국
#define		TC_STATUS		903									//903:상태
#define		TC_ERROR		904									//904:장애
#define		TC_TOTAL		905									//904:마감
#define		TC_TOTALCANCEL	906									//905:마감취소
#define		TC_SUBTOTAL		907									//906:마감조회
#define		TC_CASH_REG		908									//907:현금등록
#define		TC_CASH_UNREG	909									//908:현금해제
#define		TC_CHECK_REG	910									//909:수표등록
#define		TC_CHECK_UNREG	911									//910:수표해제



																// DES : 2003.12.12
#define		TC_TMKLOAD		1001								//1001:DES거래 1차-TMK
#define		TC_TPKLOAD		1002								//1002:DES거래 2차-TPK

																// DES OutGoing거래 2004.05.31
#define		TC_WITHCASHAD	1013								//1013:Credit Card Cash Advance
#define		TC_WITHSAVEAC	1014								//1014:Cash Withdrawal Saving Account
#define		TC_WITHCHKAC	1015								//1015:Cash Withdrawal Checking Account
#define		TC_INQCREDAC	1016								//1016:Balance Inquiry Credit Account
#define		TC_INQSAVEAC	1017								//1017:Balance Inquiry Saving Account
#define		TC_INQCHKAC		1018								//1018:Balance Inquiry Checking Account

/////////////////////////////////////////////////////////////////////////////
static MENUHeader MenuTblConst[] = 
{
////메뉴정보(0)			카드선행시(1)
////						통장선행시(2)
////						자동입력선행시(2)(1버튼시)
////							입력선행시/거래선행시(3)
////								거래처리코드(4)
////											거래코드(5)
////														거래코드2(6)
/////////////////////////////////////////////////////////////////////////////
//// 0:메인화면
	"거래선택",
	1,														// 화면번호
	"",
	"입금",
	"출금",
	"조회",
	"이체",
	"단기카드대출",									
	"현금서비스조회",
	"현금서비스이체",
	"",
	"",
	"다국어",
	"",
	"",
	"",
	"해외카드",
	"",
	"",
	"화면확대",
	"",
	"",
	"",
	"",

	MENU_NULL,			1,	1,	0,	TRAN_NULL,	0,						0,				// 0:처리관련
	MENU_MCU3,  		0,	0,	0,	TRAN_DEP,	TC_DEPOSIT,				TRANID_7200,	// 1:입금
	MENU_MCU3,	    	0,	0,  0,	TRAN_WITH,	TC_WITHDRAW,			TRANID_4310,	// 2:출금
	MENU_MCU3,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,				TRANID_7220,	// 3:조회
	MENU_MCU3,  		0,	0,  0,	TRAN_TRANS,	TC_TRANSFER,			TRANID_7260,	// 4:이체
	MENU_MCU2,			0,	0,	10,	TRAN_NULL,	0,						0,				// 5:현금서비스지급
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 6:현금서비스조회
//	MENU_MCU2,			0,	0,	0,	TRAN_TRANS,	TC_SVCTRAN1,			TRANID_7111,	// 7:현금서비스이체 
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0, 
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0,          	// 8:
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0,				// 09:
	MENU_MCU,			0,	0,	2,	TRAN_NULL,  0,						0,              // 10:다국어
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,				// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,              // 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,				// 13:
	MENU_NULL,			0,	0,	6,	TRAN_NULL,	0,					    0,				// 14:해외카드  
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,					    0,				// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,           	// 16:
	MENU_NULL,			0,	0,	11, TRAN_NULL,	0,						0,				// 17:화면확대 (저시력자/장애인)  
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,			    // 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,  0,				        0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:


//// 1:메체 투입후 거래선택 메인화면
	"거래선택",
	2,														// 화면번호
	"",
	"입금",
	"출금",
	"조회",
	"이체",
	"단기카드대출",											
	"현금서비스조회",
	"현금서비스이체",
	"",
	"",
	"다국어",
	"",
	"",
	"",
	"해외카드",
	"",
	"",
	"화면확대",
	"",
	"",
	"",
	"",

	MENU_NULL,			1,	1,	0,	TRAN_NULL,	0,						0,				// 0:처리관련
	MENU_MCU3,			0,	0,	0,	TRAN_DEP,	TC_DEPOSIT,				TRANID_7200,	// 1:입금
	MENU_MCU3,			0,	0,  0,	TRAN_WITH,	TC_WITHDRAW,			TRANID_4310,	// 2:출금
	MENU_MCU3,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,				TRANID_7220,	// 3:조회
	MENU_MCU3,			0,	0,  0,	TRAN_TRANS,	TC_TRANSFER,			TRANID_7260,	// 4:이체
	MENU_MCU2,			0,	0,	10,	TRAN_NULL,	0,						0,				// 5:현금서비스지급
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 6:현금서비스조회
//	MENU_MCU2,			0,	0,	0,	TRAN_TRANS,	TC_SVCTRAN1,			TRANID_7250,	// 7:현금서비스이체  
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0, 
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0,          	// 8:
	MENU_NULL,		    0,	0,	0,	TRAN_NULL,  0,						0,				// 09:
	MENU_MCU,			0,	0,	2,	TRAN_NULL,  0,						0,              // 10:다국어
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,				// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,              // 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,				// 13:
	MENU_NULL,			0,	0,	6,	TRAN_NULL,	0,					    0,				// 14:해외카드  
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,					    0,				// 15:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,           	// 16:
	MENU_NULL,			0,	0,	11,	TRAN_NULL,	0,						0,				// 17:화면확대 (저시력자/장애인)  
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,			    // 18:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 19:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,  0,				        0,	// 20:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 21:


// 외국어관련 화면없음.
//// 2:English/日本語/漢語(Global/Domestic포함)
	"외국어",
	103,																		// 화면번호	83 -> 38
	"",
	"국내영문",
	"국내일문",
	"국내중문",
	"해외영문",
	"해외일문",
	"해외중문",
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

	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 0:처리관련
	MENU_MCU,			0,	0,	4,	TRAN_NULL,	0,						0,	// 1:국내영어
	MENU_MCU,			0,	0,	4,	TRAN_NULL,	0,						0,	// 2:국내일본어
	MENU_MCU,			0,	0,	4,	TRAN_NULL,	0,						0,	// 3:국내중국어
	MENU_MCU2,			0,	0,	3,	TRAN_NULL,	0,						0,	// 4:해외영어
	MENU_MCU2,			0,	0,	3,	TRAN_NULL,	0,						0,	// 5:해외일본어
	MENU_MCU2,			0,	0,	3,	TRAN_NULL,	0,						0,	// 6:해외중국어
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


/// 3:Global(English/日本語/漢語)
	"CARD_INSERT",
	801,														// 화면번호
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

	MENU_NULL,			6,	0,	0,	TRAN_NULL,	0,						0,	// 0:처리관련
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


//// 4:Domestic(English/日本語/漢語)
	"외국어",
	102,																		// 화면번호
	"",
	"입금",
	"출금",
	"조회",
	"이체",
	"현금서비스지급",
	"현금서비스조회",
	"통장정리",
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

	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 0:처리관련
	MENU_MCU3,	    	0,	0,	0,	TRAN_DEP,	TC_DEPOSIT,				TRANID_7200,	// 1:입금
	MENU_MCU3,	    	0,	0,  0,	TRAN_WITH,	TC_WITHDRAW,			TRANID_4310,	// 2:출금
	MENU_MCU3,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,				TRANID_7220,	// 3:조회
	MENU_MCU3,	    	0,	0,  0,	TRAN_TRANS,	TC_TRANSFER,			TRANID_7260,	// 4:이체
	MENU_MCU2,			0,	0,	0,	TRAN_WITH,	TC_SVCWITH,				TRANID_4330,	// 5:현금서비스지급
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 6:현금서비스조회
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 7:통장정리
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



//// 5:메인화면
	"거래선택",
	101,														// 화면번호
	"",
	"현금서비스지급",
	"현금서비스조회",
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

	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 0:처리관련
	MENU_MCU2,			0,	0,	0,	TRAN_WITH,	TC_SVCWITH,				TRANID_4330,	// 1:현금서비스지급
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 2:현금서비스조회
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 3:조회
	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 4:이체
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 5:현금서비스지급
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 6:현금서비스조회
	MENU_NULL,			0,	0,	0,	TRAN_NULL,  0,						0,  // 7:다국어
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


/// 6:Global(English/日本語/漢語)
////6:해외카드선택시 카드 처리	
	"해외카드",
	121,														// 화면번호
	"",
	"VISA_출금",														// VISA				
	"MASTER_출금",                                                     	// MASTER
	"JCB_출금",                                                      	// JCP
	"CUP_출금",                                                      	// CUP
	"CUP_조회",                                                      	// CUP 
	"Other_출금",
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

	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:처리관련
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,	        TRANID_4330,	// 1:VISA 출금
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,         	TRANID_4330,	// 2:MASTER 출금
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,         	TRANID_4330,	// 3:JVB 출금
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,         	TRANID_4330,	// 4:CUP 출금
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN_INQ,  TC_FOREGIN,	 	TRANID_7230,	// 5:CUP 조회
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
	
//// 7:해외카드 세부메뉴 #N0235
	"해외카드",
	122,														// 화면번호
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

	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:처리관련
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

	
//// 8:해외카드 세부메뉴 #N0235
	"해외카드",
	123,														// 화면번호
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

	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:처리관련
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,			TRANID_4330,	// 1:지급
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN_INQ,   TC_FOREGIN,		TRANID_7230,	// 2:조회
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


//// 9:해외카드 세부메뉴 #N0235
	"해외카드",
	123,														// 화면번호
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

	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:처리관련
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,			TRANID_4330,	// 1:지급
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN_INQ, TC_FOREGIN,		TRANID_7230,	// 2:조회
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




	//// 10:신용카드 세부 매뉴
	"신용카드",
	10,																// 화면번호
	"",
	"단기카드대출출금",											
	"단기카드대출이체",
	"단기카드대출조회",
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
	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:처리관련
	MENU_MCU2,			0,	0,	0,	TRAN_WITH,	TC_SVCWITH,				TRANID_4330,	// 1:현금서비스지급
//	MENU_MCU2,			0,	0,	0,	TRAN_TRANS,	TC_SVCTRAN1,			TRANID_7111,	// 2:현금서비스이체  #N0161
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 3:현금서비스조회
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


//// 11:신용카드 세부 매뉴
	"화면확대",
	2,																// 화면번호
	"",
	"예금출금",
	"입금",
	"계좌이체",
	"통장정리",
	"예금조회",
	"단기카드대출",													//#N0247
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
	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:처리관련
	MENU_MCU3,			0,	0,  0,	TRAN_WITH,	TC_WITHDRAW,			TRANID_4310,	// 1:출금
	MENU_MCU3,			0,	0,	0,	TRAN_DEP,   TC_DEPOSIT,				TRANID_7200,    // 2:입금
	MENU_MCU3,			0,	0,  0,	TRAN_TRANS,	TC_TRANSFER,			TRANID_7260,	// 3:이체
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 4:통장정리
	MENU_MCU3,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,				TRANID_7220,	// 5:조회
	MENU_NULL,			0,	0,	12,	TRAN_NULL,	0,						0,	// 6:신용카드
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


	//// 12:저시력 신용카드 세부 매뉴
	"신용카드",
	10,																// 화면번호
	"",
	"단기카드대출출금",
	"단기카드대출이체",
	"단기카드대출조회",
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
	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:처리관련
	MENU_MCU2,			0,	0,	0,	TRAN_WITH,	TC_SVCWITH,				TRANID_4330,	// 1:현금서비스지급
//	MENU_MCU2,			0,	0,	0,	TRAN_TRANS,	TC_SVCTRAN1,			TRANID_7250,	// 2:현금서비스이체  #N0161
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,
	MENU_MCU2,			0,	0,	0,	TRAN_INQ,	TC_SVCINQ,				TRANID_7230,	// 3:현금서비스조회
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


/// 13:Global(English/日本語/漢語) - 삼성카드
////13:해외카드선택시 카드 처리	#N0278
	"해외카드",
	120,														// 화면번호
	"",
	"SAVINGACCOUNT",
	"CHECKINGACCOUNT",
	"CREDITCARD",
	"CUP_출금",
	"CUP_조회",
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

	MENU_NULL,			0,	0,  0,	TRAN_NULL,	0,						0,	// 0:처리관련
	MENU_MCU2,			0,	0,	9,	TRAN_NULL,  0,					 	0,	// 1:SAVINGSACCOUNT
	MENU_MCU2,			0,	0,	9,	TRAN_NULL,  0,					  	0,	// 2:CHECKINGACCOUNT
	MENU_MCU2,			0,	0,	9,	TRAN_NULL,  0,					    0,	// 3:CREDITCARD
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN,  TC_FOREGIN,         	TRANID_4330,	// 4:CUP 출금
	MENU_MCU2,			0,	0,	0,	TRAN_FOREGIN_INQ,  TC_FOREGIN,	 	TRANID_7230,	// 5:CUP 조회
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

//// N:마지막
	"",
	0,															// 화면번호
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

	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,						0,	// 0:처리관련
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

