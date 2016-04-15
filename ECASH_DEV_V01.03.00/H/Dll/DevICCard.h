/////////////////////////////////////////////////////////////////////////////
#if !defined(_DEVICCARD_H_INCLUDED_)
#define _DEVICCARD_H_INCLUDED_


/*==============================================================================
*  Work Definitions
* ============================================================================*/
#define EMV_TA_MODE						0			// EMV ���� ��� ����

/*==============================================================================
*  ISO-7816 Definition
* ============================================================================*/
#define TRAN_IC_PROTOCOL_T0				0			// T0 Protocol
#define TRAN_IC_PROTOCOL_T1				1			// T1 Protocol

#define TRAN_IC_PROTOCOL_T6				6			// T6 Protocol
#define TRAN_IC_PROTOCOL_T7				7			// T7 Protocol
#define TRAN_IC_PROTOCOL_T8				8			// T8 Protocol

#define IC_PIN_LEN						0x08		// Length of PIN
#define IC_SW_LEN						0x02		// Status Word Len
#define IC_APDU_LEN						256			// APDU Len
#define IC_DATA_LEN						1024		// ATM/CD IC Data Len(EP, SP�� ����)
#define IC_AID_LEN						0x10		// AID �ִ����
#define IC_RID_LEN						0x05		// RID �ִ����

//******************************************************************************
//  ISO-7816 Tag
//******************************************************************************
#define TAG_ISO7816_DIRFILE				0x61
#define TAG_ISO7816_AID					0x4F
#define TAG_ISO7816_APPLABEL			0x50

//******************************************************************************
//  ISO-7816 APDU Case
//******************************************************************************
#define ISO_7816_CASE1					0x31		// CLA, INS, P1, P2
#define ISO_7816_CASE2					0x32		// CLA, INS, P1, P2, LE
#define ISO_7816_CASE3					0x33		// CLA, INS, P1, P2, LC, Data
#define ISO_7816_CASE4					0x34		// CLA, INS, P1, P2, LC, Data, LE
#define ISO_7816_CASEFREE				0x35		// Non ISO format

//******************************************************************************
//  ISO-7816 Class Byte
//******************************************************************************
#define	CLA_7816_STANDARD				0x00
#define	CLA_7816_STD_AUTH				0x04
#define	CLA_7816_APP_SPEC				0x80
#define	CLA_7816_APP_AUTH				0x84
#define	CLA_7816_CONFIDENT				0xA0
#define	CLA_7816_CONF_AUTH				0xA4

//******************************************************************************
//  ISO-7816 & VOP(Visa Open Flatform) Instruction Byte
//******************************************************************************
#define	INS_7816_ERASE_BINARY			0x0E		// ERASE_BINARY
#define INS_7816_VERIRY					0x20		// VERIFY
#define INS_7816_MANAGE_CHANNEL			0x70		// MANAGE_CHANNEL
#define INS_7816_EXTERNAL_AUTHENTICATE	0x82		// EXTERNAL_AUTHENTICATE
#define INS_7816_GET_CHALLANGE			0x84		// GET_CHALLANGE
#define INS_7816_INTERNAL_AUTHENTICATE	0x88		// INTERNAL_AUTHENTICATE
#define INS_7816_SELECT_FILE			0xA4		// SELECT_FILE
#define INS_7816_READ_BINARY			0xB0		// READ_BINARY
#define INS_7816_READ_RECORD			0xB2		// READ_RECORD
#define INS_7816_GET_RESPONSE			0xC0		// GET_RESPONSE
#define INS_7816_ENVELOPE				0xC2		// ENVELOPE
#define INS_7816_GET_DATA				0xCA		// GET_DATA
#define INS_7816_WRITE_BINARY			0xD0		// WRITE_BINARY
#define INS_7816_WRITE_RECORD			0xD2		// WRITE_RECORD
#define INS_7816_UPDATE_DATA			0xD6		// UPDATE_DATA
#define INS_7816_PUT_DATA				0xDA		// PUT_DATA
#define INS_7816_UPDATE_RECORD			0xDC		// UPDATE_RECORD
#define INS_7816_APPEND_RECORD			0xE2		// APPEND_RECORD

//#define INS_VOP_DELETE					0xE4
//#define INS_VOP_GET_STATUS				0xF2
//#define INS_VOP_INITIALIZE_UPDATE		0x50
//#define INS_VOP_INSTALL					0xE6
//#define INS_VOP_LOAD					0xE8
//#define INS_VOP_PIN_CHANGE_UNBLOCK		0x24
//#define INS_VOP_PUT_KEY					0xD8
//#define INS_VOP_SET_STATUS				0xF0

//******************************************************************************
//  ISO-7816 Parameter 1, 2 Byte
//******************************************************************************
#define P1P2_7816_ZERO					0x00
#define P1_7816_SELECTION_BY_FIRSTFILE	0x01
#define P1_7816_SELECTION_BY_FILENAME	0x04
#define P1_7816_SELECTION_BY_PATH		0x08
#define P2_7816_SELECTION_BY_P1RECORD	0x04
#define P2_7816_SELECTION_BY_FCIRECORD	0x0C

#define P1_7816_SELECTION_BY_FIRSTFILE2	0x81

//******************************************************************************
//  ǥ�� ���� ����
//******************************************************************************
// ���� �Ǵ� Warning
static BYTE	ISO7816_SW_NO_ERROR[2]                        = { 0x90, 0x00 };	// ����
static BYTE ISO7816_SW_BYTES_REMAINING[2]                 = { 0x61, 0x00 }; // 0x61XX. GET_RESPONSE �ʿ�
static BYTE ISO7816_SW_COUNTER_PROVIDED[2]                = { 0x63, 0xC0 };
static BYTE ISO7816_SW_CORRECT_LENGTH[2]                  = { 0x6C, 0x00 }; // 0x6CXX. Le ��ġ���� ����

// ISO7816 Error
static BYTE ISO7816_SW_WARNING_NV_MEMORY_UNCHANGE[2]      = { 0x62, 0x00 };
static BYTE ISO7816_SW_PART_OF_RETURN_DATA_CORRUPTED[2]   = { 0x62, 0x81 };
static BYTE ISO7816_SW_END_FILE_REACHED_BEFORE_LE_BYTE[2] = { 0x62, 0x82 };
static BYTE ISO7816_SW_SELECTED_FILE_INVALID[2]           = { 0x62, 0x83 };
static BYTE ISO7816_SW_FCI_NOT_FORMATTED_TO_ISO[2]        = { 0x62, 0x84 };
static BYTE ISO7816_SW_WARNING_NV_MEMORY_CHANGED[2]       = { 0x63, 0x00 };
static BYTE ISO7816_SW_FILE_FILLED_BY_LAST_WRITE[2]       = { 0x63, 0x81 };
static BYTE ISO7816_SW_ERROR_NV_MEMORY_UNCHANGED[2]       = { 0x64, 0x00 }; // 0x64XX
static BYTE ISO7816_SW_ERROR_NV_MEMORY_CHANGED[2]         = { 0x65, 0x00 }; // 0x65XX
static BYTE ISO7816_SW_RESERVED[2]                        = { 0x66, 0x00 }; // 0x66XX
static BYTE ISO7816_SW_WRONG_LENGTH[2]                    = { 0x67, 0x00 };
static BYTE ISO7816_SW_FUNCTIONS_IN_CLA_NOT_SUPPORTED[2]  = { 0x68, 0x00 };
static BYTE ISO7816_SW_LOGICAL_CHANNEL_NOT_SUPPORTED[2]   = { 0x68, 0x81 };
static BYTE ISO7816_SW_SECURE_MESSAGING_NOT_SUPPORTED[2]  = { 0x68, 0x82 };
static BYTE ISO7816_SW_COMMAND_NOT_ALLOWED[2]             = { 0x69, 0x00 };
static BYTE ISO7816_SW_COMMAND_INCOMPATIBLE_WITH_FILE[2]  = { 0x69, 0x81 };
static BYTE ISO7816_SW_SECURITY_STATUS_NOT_SATISFIED[2]   = { 0x69, 0x82 };
static BYTE ISO7816_SW_FILE_INVALID[2]                    = { 0x69, 0x83 };
static BYTE ISO7816_SW_DATA_INVALID[2]                    = { 0x69, 0x84 };
static BYTE ISO7816_SW_CONDITIONS_NOT_SATISFIED[2]        = { 0x69, 0x85 };
static BYTE ISO7816_SW_COMMAND_NOT_ALLOWED_AGAIN[2]	      = { 0x69, 0x86 };
static BYTE ISO7816_SW_SM_DATA_OBJECTS_MISSING[2]         = { 0x69, 0x87 };
static BYTE ISO7816_SW_SM_DATA_OBJECTS_INCORRECT[2]       = { 0x69, 0x88 };
static BYTE ISO7816_SW_WRONG_PARAMS[2]                    = { 0x6A, 0x00 };
static BYTE ISO7816_SW_WRONG_DATA[2]                      = { 0x6A, 0x80 };
static BYTE ISO7816_SW_FUNC_NOT_SUPPORTED[2]              = { 0x6A, 0x81 };
static BYTE ISO7816_SW_FILE_NOT_FOUND[2]                  = { 0x6A, 0x82 };
static BYTE ISO7816_SW_RECORD_NOT_FOUND[2]                = { 0x6A, 0x83 };
static BYTE ISO7816_SW_NOT_ENOUGH_SPACE_IN_FILE[2]        = { 0x6A, 0x84 };
static BYTE ISO7816_SW_Lc_INCONSISTENT_WITH_TLV[2]        = { 0x6A, 0x85 };
static BYTE ISO7816_SW_INCORRECT_P1P2[2]                  = { 0x6A, 0x86 };
static BYTE ISO7816_SW_Lc_INCONSISTENT_WITH_P1P2[2]       = { 0x6A, 0x87 };
static BYTE ISO7816_SW_REFERENCED_DATA_NOT_FOUND[2]       = { 0x6A, 0x88 }; // ���� ������ ã�� �� ����
static BYTE ISO7816_SW_WRONG_P1P2[2]                      = { 0x6B, 0x00 };
static BYTE ISO7816_SW_INS_NOT_SUPPORTED[2]               = { 0x6D, 0x00 }; // ��ɾ� �ڵ� ������ �Ǵ� ��ȿ���� ����
static BYTE ISO7816_SW_CLA_NOT_SUPPORTED[2]               = { 0x6E, 0x00 }; // Ŭ���� ������
static BYTE ISO7816_SW_UNKNOWN[2]                         = { 0x6F, 0x00 }; // ��Ȯ�� ���� ����


/*==============================================================================
* Universal Definitions
* ============================================================================*/
#define SAFE_DELETE_ARR(pBuf) {			\
	if ( pBuf ) {						\
		delete [] pBuf;					\
		pBuf = NULL;					\
	}									\
}

#define SAFE_DELETE(pBuf) {				\
	if ( pBuf ) {						\
		delete pBuf;					\
		pBuf = NULL;					\
	}									\
}


/*==============================================================================
* Work Definitions
* ============================================================================*/
#define	IC_INIT_TOKEN					("INIT")

//******************************************************************************
//  ATM Mode
//******************************************************************************
#define MWI_BASE_MODE					1			// Kalignite + MWI Base
#define KAL_BASE_MODE					2			// Kalignite + Javascript Base
#define WIN_BASE_MODE					3			// WinAtm Base

//******************************************************************************
//  User Define & Mondex Class Byte
//******************************************************************************
#define	CLA_USER_STANDARD				0x90

//******************************************************************************
//  User Define & Mondex Instruction Byte
//******************************************************************************
#define INS_MONDEX_REGISTER				0x22		// REGISTER
#define INS_MONDEX_PURSE_REGISTER		0x24		// PURSE_REGISTER
#define INS_MONDEX_READ_PAYMENT_RECORD	0x28		// READ_PAYMENT_RECORD
#define INS_MONDEX_READ_POCKET_RECORD	0x2A		// READ_POCKET_RECORD
#define INS_MONDEX_AUTNTICATION_ENQUIRY	0x40		// AUTNTICATION_ENQUIRY
#define INS_MONDEX_CHANGE_PCN			0x48		// CHANGE_PCN
#define INS_MONDEX_SERVICE_CHECK		0x4E		// AUTNTICATION_ENQUIRY
#define INS_MONDEX_PAYMENT_START_PAYEE	0x52		// PAYMENT_START_PAYEE
#define INS_MONDEX_PAYMENT_REGISTER		0x54		// PAYMENT_REGISTER
#define INS_MONDEX_PAYMENT_REQUEST		0x70		// PAYMENT_REQUEST
#define INS_MONDEX_PAYMENT_VALUE		0x72		// PAYMENT_VALUE
#define INS_MONDEX_PAYMENT_ACK			0x74		// PAYMENT_ACK
#define INS_USER_GET_ENCDATA			0x8E		// GET_ENCDATA

//******************************************************************************
//  IC Card �����ڵ� : FC, M, S. �̷��� 3���� ������ �����ȴ�.
//******************************************************************************
static BYTE	IC_RECV_OK[3]	= { 0x31, 0x00, 0x00 };		// ����
static BYTE	IC_RECV_OK2[3]	= { 0x30, 0x30, 0x30 };		// ����2. Spec���� �������� Ȯ�ο�.
#define	IC_RECV_NG						0xD2			// ������

#define IC_FCMS_LEN						0x03		// IC �����ڵ� ����
#define IC_ERRORRESP_LEN				0x04		// IC Error Response Length

////////////////////////////////////////////////////////////////////////////////
//  FC : ���� �����ڵ�. ��� �����ڵ�� �⺻������ FC�� �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////
#define IC_FC_MAIN						0x30		// IC Transaction

////////////////////////////////////////////////////////////////////////////////
//  M  : �ŷ��� �����ڵ�. �ŷ��� ������ �׷��ڵ��̴�.                                           //
////////////////////////////////////////////////////////////////////////////////
#define IC_MFC_BASIC					0x01		// �⺻���
#define IC_MFC_STDEP					0x02		// �������� ���ðŷ� 1
#define IC_MFC_EMV						0x04		// EMV ���� ���	V03-00-26-#01
#define IC_MFC_VALUE					0x06		// �������� ���ðŷ� 2
#define IC_MFC_CC						0x07		// ���� ������
#define IC_MFC_PARAM					0x08		// ����ȭ�� �Ķ���� ����
#define IC_MFC_PIN						0x09		// PIN ����
#define IC_MFC_PBOOK					0x0A		// �������� ���ø��
#define IC_MFC_EPB						0x0B		// �������� ���			// V03-00-28-#01
#define IC_MFC_CEPS1					0x22		// CEPS �ŷ� ���� ���
#define IC_MFC_CEPS2					0x26		// CEPS �ŷ� ���� ���

////////////////////////////////////////////////////////////////////////////////
//  S  : �ŷ��� ���� �����ڵ�. �ŷ��� ������ ���ξ����� ���� �ڵ带 �����Ѵ�.                        //
////////////////////////////////////////////////////////////////////////////////
// IC_MFC_BASIC ���� ���� �����ڵ�
#define IC_SFC_POWERON					0x01		// ī�忡 ��������(����:ATR)
#define IC_SFC_ISOIN					0x02		// APDU Command Send.
#define IC_SFC_ISOOUT					0x03		// APDU Command Receive.
#define IC_SFC_POWEROFF					0x04		// ī�忡 ��������
#define	IC_SFC_SELECTAPP				0x30		// AID�� ���� Select Application ���
#define	IC_SFC_ICCONFIRMTYPE			0x31		// IC CHIP TYPE READ ���

// IC_MFC_STDEP ���� ���� �����ڵ�
#define IC_SFC_READJAN					0x01		// �������� �ܾ���ȸ
#define IC_SFC_PINCHECK					0x02		// ��������DF/����������DF�� PIN������û
#define IC_SFC_ATRINFO					0x03		// ATR �б�
#define	IC_SFC_TRANCMPLT				0x04		// �ŷ����� Bit ����
#define	IC_SFC_VERIFYTRANBIT			0x05		// �ŷ����� Bit Ȯ��
#define IC_SFC_TRAFFICINFO				0x06		// ��������(10����Ʈ) �б�
#define IC_SFC_OPENREADJAN				0x20		// �������� �ܾ���ȸ(������)		2004.02.16
#define IC_SFC_OPENTRAFFICINFO			0x21		// ��������(10����Ʈ) �б�(������)	2004.02.16

// IC_MFC_VALUE ���� ���� �����ڵ�
#define IC_SFC_PRELOAD					0x01		// ������ó��
#define IC_SFC_LOADIEP					0x02		// �����䱸
#define IC_SFC_PREUNLOAD				0x04		// ȯ����ó��
#define IC_SFC_UNLOAD					0x05		// ȯ�ҿ䱸
#define IC_SFC_READPLOG					0x08		// ���Űŷ�������ȸ
#define IC_SFC_READCLOG					0x09		// ��ġ�����ŷ� ������ȸ
#define IC_SFC_READPLOG2				0x0A		// ���Űŷ�������ȸ2
#define IC_SFC_READPAN					0x0B		// ��������(PAN����) ��ȸ
#define IC_SFC_PRERCVUNLOAD				0x0C		// ȯ�Һ�����ó��
#define IC_SFC_READEPID					0x0D		// ����ȭ���ȣ�о����
#define IC_SFC_OPENREADPAN				0x20		// PAN���� �б�(������)				2004.02.16
#define IC_SFC_OPENPRERCVUNLOAD			0x21		// ȯ�Һ�����ó��(������)			2004.02.16

// ICCMD_MFC_EMV ���� ���� �����ڵ�
#define IC_SFC_CALISTSET				0x01		// CA Key Set ����(������)
#define IC_SFC_TERMINALLISTSET			0x02		// �ܸ��� ���� AID List ����
#define IC_SFC_TERMINALCONSTSET			0x03		// �ܸ��� �ʼ� ��ü ��� ����
#define	IC_SFC_SELECTPSE				0x04		// PSE Select(��� EMV �ŷ� ���� Ȯ��)
#define	IC_SFC_REQCANDIDATELIST			0x10		// �ĺ� ��� ��û
#define IC_SFC_SELECTAPPEMV				0x11		// AID�� ���� EMV Application Select
#define IC_SFC_READAPPEMV				0x12		// EMV Application ���� ����
#define IC_SFC_OFFDATAAUTHEN			0x13		// Offline Data Authentication
#define IC_SFC_ONLINEPROCESS			0x14		// Online ���� �� ī�� ���� �䱸
#define IC_SFC_COMPLETPROCESS			0x15		// �ŷ� ���� ó��
#define IC_SFC_AFTERSCRIPT				0x16		// �ŷ� �� Issuer Script ó��
#define IC_SFC_GETDOLDATA				0x20		// �ʿ��� ��ü ��� �䱸
#define IC_SFC_EMVRESULTDATA			0x21		// EMV �ŷ� ��� ������ �䱸(������)

// IC_MFC_CC ���� ���� �����ڵ�
#define IC_SFC_READPERSONINFO			0x08		// ���� ������ �������� ��ȸ
#define IC_SFC_READBASICINFO			0x20		// ���� ������ �⺻���� ��ȸ
#define IC_SFC_READACCOUNTINFO			0x21		// ���� ������ �������� ��ȸ
#define IC_SFC_READDUALINFO				0x22		// ���� ������ �⺻���� + �������� ��ȸ
#define IC_SFC_GETENCIPHER				0x23		// ���� ������ Get Encipher
#define IC_SFC_VERIFYPIN				0x24		// ���� ������ PIN ����

// IC_MFC_PARAM ���� ���� �����ڵ�
#define IC_SFC_PREUPDATE				0x01		// prepare update : �Ķ���� ������ ���� �ʱ�ȭ
#define IC_SFC_UPDATEIEP				0x02		// Update IEP : ���������� �Ķ���͸� �����Ѵ�.

// IC_MFC_PIN ���� ���� �����ڵ�
#define IC_SFC_PINCHANGE				0x09		// PIN������ ��û�Ѵ�.(����ȭ��/�ݰ���/����DF)
#define IC_SFC_OPENPINCHANGE			0x20		// PIN������ ��û�Ѵ�.(������)		2004.02.17
#define IC_SFC_OPTIONALOPENPINCHANGE	0x21		// PIN������ ��û�Ѵ�.(������)		2004.11.03
#define IC_SFC_OPTIONALCLOSEPINCHANGE	0x22		// PIN������ ��û�Ѵ�.(�����)		2004.11.03

// IC_MFC_PBOOK ���� ���� �����ڵ�
#define IC_SFC_EPBREAD					0x11		// �������e-����������������ȸ V05-00-02-#01 
#define IC_SFC_MAINPASSBK				0x01		// �ְ������� 

/*==============================================================================
*  ���� IC
* ============================================================================*/
#define ICTYPE_NOCHIP					0x00		// IC �ƴ�
#define ICTYPE_CASHIC					0x01		// ���� IC
#define ICTYPE_KCASH					0x02		// K-CASH
#define ICTYPE_EMV						0x04		// EMV Card
#define ICTYPE_ICASIGN					0x08		// ������(�����) Card
#define ICTYPE_ICALOAD					0x10		// ������(Ű��п�) Card
#define ICTYPE_MONDEX					0x01		// Mondex
#define ICTYPE_ELECPB					0x02		// ��������

#define ICTYPE_OPENCLOSE				0x80		// ������ ����� ����

#define ICBANK_COMMON					0x00		// �Ϲ������(���� IC + K-Cash(��/��) + EMV )
#define ICBANK_KOOKMIN					0x10		// ���������(�Ϲ������ + Mondex + ��������)                       
#define ICBANK_KOOKMIN_NOCERT			0x11		// ���������(�Ϲ����� + Mondex + �������� - ������) //20040924 kh �߰�

#define FIC_AID_LEN						0x07		// ���� IC AID Length
#define FIC_CSN_LEN						0x08		// ���� IC CAN, CSN Length
#define FIC_RANDOM_LEN					0x10		// ���� IC Random Length
#define FIC_ISO3_LEN					0x36		// ���� IC Account Password Length
#define FIC_ACCOUNTINFO_LEN				0x3E		// ���� IC �������� ũ��
#define FIC_BANKCODE_LEN				0x03		// ���� IC Bank Code
#define FIC_ENCDATA_LEN					0x08		// ���� IC ENCData Length(MyBi)
#define FIC_FCI_LEN						0x28		// ���� IC FCI Length
#define FIC_ICTYPE_LEN					0x02		// ���� IC FCI Length

#define FIC_MAX_ACCOUNINFO				0x0A		// ���� IC Account �������� Max
#define FIC_ISO3_ACCSTART				0x08		// ���� IC ���� ���� ��ġ

//******************************************************************************
//  Application ID
//	V02-06-01-#9 ����������� DF AID �߰�ó��
//******************************************************************************
static BYTE FIC_AID[FIC_AID_LEN]		=		{ 0xD4, 0x10, 0x65, 0x09, 0x90, 0x00, 0x10 };
static BYTE FIC_KCASH_AID[FIC_AID_LEN]	=		{ 0xD4, 0x10, 0x65, 0x09, 0x90, 0x00, 0x20 };
static BYTE FIC_ICASIGN_AID[FIC_AID_LEN]=		{ 0xD4, 0x10, 0x65, 0x09, 0x90, 0x00, 0x30 };		// ������(�����)
static BYTE ELECPB_AID[FIC_AID_LEN]		=		{ 0xD4, 0x10, 0x65, 0x00, 0x41, 0x00, 0x02 };		// ��������
static BYTE FIC_MYBI_AID[FIC_AID_LEN]	=		{ 0xD4, 0x10, 0x65, 0x90, 0x10, 0x00, 0x20 };
static BYTE HIPASS_AID[FIC_AID_LEN]		=		{ 0xA0, 0x00, 0x00, 0x02, 0x45, 0x00, 0x01 };		// �����н�	V06-13-00-#01 Hi-Pass���˽��������� #0164


// EMV ����(�� 14��)
static BYTE VISA_AID[FIC_AID_LEN]		=		{ 0xA0, 0x00, 0x00, 0x00, 0x03, 0x10, 0x10 };		// VISA(�ſ� or Check)
static BYTE ELECTRON_AID[FIC_AID_LEN]	=		{ 0xA0, 0x00, 0x00, 0x00, 0x03, 0x20, 0x10 };		// VISA ELECTRON
static BYTE PLUS_AID[FIC_AID_LEN]		=		{ 0xA0, 0x00, 0x00, 0x00, 0x03, 0x80, 0x10 };		// PLUS(VSDC �迭)
static BYTE MASTER_AID[FIC_AID_LEN]		=		{ 0xA0, 0x00, 0x00, 0x00, 0x04, 0x10, 0x10 };		// MASTER CARD
static BYTE MAESTRO_AID[FIC_AID_LEN]	=		{ 0xA0, 0x00, 0x00, 0x00, 0x04, 0x30, 0x60 };		// MAESTRO(M-Chip �迭)
static BYTE CIRRUS_AID[FIC_AID_LEN]		=		{ 0xA0, 0x00, 0x00, 0x00, 0x04, 0x60, 0x00 };		// CIRRUS(MASTER �迭)
static BYTE JCB_AID[FIC_AID_LEN]		=		{ 0xA0, 0x00, 0x00, 0x00, 0x65, 0x10, 0x10 };		// JCB(Japan)
static BYTE LOCALEMV_AID[FIC_AID_LEN]	=		{ 0xD4, 0x10, 0x00, 0x00, 0x01, 0x10, 0x10 };		// �������� �ſ� or Check VSDC
static BYTE LOCAL_MC_AID[FIC_AID_LEN]	=		{ 0xD4, 0x10, 0x00, 0x00, 0x01, 0x20, 0x10 };		// �������� �ſ� or Check M/CHIP
static BYTE D_PAS_AID[7]				=		{ 0xA0, 0x00, 0x00, 0x01, 0x52, 0x30, 0x10 };		// D-PAS
static BYTE AMX_AID[8]					=		{ 0xA0, 0x00, 0x00, 0x00, 0x25, 0x01, 0x04, 0x02};	// �Ƹƽ�
static BYTE CUP_DEBIT_AID[8]			=		{ 0xA0, 0x00, 0x00, 0x03, 0x33, 0x01, 0x01, 0x01};	// CUP DEBIT 
static BYTE CUP_AID[8]					=		{ 0xA0, 0x00, 0x00, 0x03, 0x33, 0x01, 0x01, 0x02};	// CUP CREDIT A000000333010102
static BYTE CUP_QUASI_AID[8]			=		{ 0xA0, 0x00, 0x00, 0x03, 0x33, 0x01, 0x01, 0x03};	// CUP QUASI		
//******************************************************************************
//  Template Tag
//******************************************************************************
static BYTE FIC_TAG_FCI			 =		{0x84};			// FCI DF Name
static BYTE FIC_TAG_CHDATATEMP   =		{0x65};			// Cardholder Data Template Tag
static BYTE FIC_TAG_CHNAME[2]    =		{0x5F, 0x20};	// Cardholder Name Tag
static BYTE FIC_TAG_REALNAME     =		{0x4B};			// �Ǹ� ID Tag
static BYTE FIC_TAG_EXPIREDAY[2] =		{0x5F, 0x24};	// ������ Tag
static BYTE FIC_TAG_ALGOCODE     =		{0xC2};			// �˰��� �ĺ��ڵ� Tag
static BYTE FIC_TAG_ISSURECODE   =		{0xC3};			// ������ �ĺ��ڵ� Tag
static BYTE FIC_TAG_CSN          =		{0xC4};			// ī�� �Ϸù�ȣ Tag
static BYTE FIC_TAG_ACCOUNTINFO  =		{0xC1};			// ���� ���� Tag


/*==============================================================================
*  ICCM ���� ���
* ============================================================================*/
#define ICCM_SERVICE_CLASS_CMN			(1000)
#define ICCM_SERVICE_CLASS_FIC			(1100)
#define ICCM_SERVICE_CLASS_EMV			(1200)
#define ICCM_SERVICE_CLASS_KCS			(1300)
#define ICCM_SERVICE_CLASS_VCASH		(1400)
#define ICCM_SERVICE_CLASS_EPB			(1500)
#define ICCM_SERVICE_CLASS_IMB			(1600)
#define ICCM_SERVICE_CLASS_HPS			(1700)
#define ICCM_SERVICE_CLASS_TMNY			(1800)
#define ICCM_SERVICE_CLASS_UPAY			(1900)
#define ICCM_SERVICE_CLASS_UPASS		(2000)
#define ICCM_SERVICE_CLASS_EB			(2100)

//******************************************************************************
//  �⺻�����ڵ�(CMN) IC COMMAND
//******************************************************************************
#define CMN_READVERSION					ICCM_SERVICE_CLASS_CMN + 0
#define CMN_POWERONCARD					ICCM_SERVICE_CLASS_CMN + 1
#define CMN_ISOIN						ICCM_SERVICE_CLASS_CMN + 2
#define CMN_ISOOUT						ICCM_SERVICE_CLASS_CMN + 3
#define CMN_POWEROFFCARD				ICCM_SERVICE_CLASS_CMN + 4
#define CMN_ISOIN2						ICCM_SERVICE_CLASS_CMN + 5
#define CMN_READATR2					ICCM_SERVICE_CLASS_CMN + 6
#define CMN_T1PROTOCOL					ICCM_SERVICE_CLASS_CMN + 7
#define	CMN_SELECTAPPLET				ICCM_SERVICE_CLASS_CMN + 8
#define CMN_ICTYPECONFIRM				ICCM_SERVICE_CLASS_CMN + 9

//******************************************************************************
//  ���� IC COMMAND(����)
//******************************************************************************			
#define	FIC_SELECTAPPLET				ICCM_SERVICE_CLASS_FIC + 0
#define	FIC_READBASICINFO				ICCM_SERVICE_CLASS_FIC + 1
#define	FIC_READACCOUNTINFO				ICCM_SERVICE_CLASS_FIC + 2
#define	FIC_READDUALINFO				ICCM_SERVICE_CLASS_FIC + 3
#define	FIC_GETENCIPHER					ICCM_SERVICE_CLASS_FIC + 4
#define	FIC_CHECKPIN					ICCM_SERVICE_CLASS_FIC + 5

//******************************************************************************
//  EMV COMMAND(����)
//******************************************************************************
#define	EMV_SELECTPSE					ICCM_SERVICE_CLASS_EMV + 0
#define	EMV_REQCANDIDATELIST			ICCM_SERVICE_CLASS_EMV + 1
#define	EMV_SELECTAPPLICATION			ICCM_SERVICE_CLASS_EMV + 2
#define	EMV_READAPPLICATION				ICCM_SERVICE_CLASS_EMV + 3
#define	EMV_OFFLINEDATAAUTHENTICATION	ICCM_SERVICE_CLASS_EMV + 4
#define	EMV_ONLINEPROCESSING			ICCM_SERVICE_CLASS_EMV + 5
#define	EMV_COMPLETIONPROCESSING		ICCM_SERVICE_CLASS_EMV + 6
#define	EMV_AFTERSCRIPTPROCESSING		ICCM_SERVICE_CLASS_EMV + 7
#define	EMV_GETDOLDATA					ICCM_SERVICE_CLASS_EMV + 8
#define	EMV_RESULTDATA					ICCM_SERVICE_CLASS_EMV + 9

//******************************************************************************
//  K-CASH COMMAND(����)
//******************************************************************************				
#define KCS_SELECTAPPLET				ICCM_SERVICE_CLASS_KCS + 0
#define	KCS_READBAMOUNT					ICCM_SERVICE_CLASS_KCS + 1
#define	KCS_READATR						ICCM_SERVICE_CLASS_KCS + 2
#define	KCS_REMOVETRANBIT				ICCM_SERVICE_CLASS_KCS + 3
#define	KCS_VERIFYTRANBIT				ICCM_SERVICE_CLASS_KCS + 4
#define	KCS_CHECKPIN					ICCM_SERVICE_CLASS_KCS + 5
#define	KCS_PINCHECK2					ICCM_SERVICE_CLASS_KCS + 6
#define KCS_PINCHECK3					ICCM_SERVICE_CLASS_KCS + 7
#define KCS_OPENREADBAMOUNT				ICCM_SERVICE_CLASS_KCS + 8
#define	KCS_READTRAFFICINFO				ICCM_SERVICE_CLASS_KCS + 9
#define	KCS_OPENREADTRAFFICINFO			ICCM_SERVICE_CLASS_KCS + 10
#define	KCS_PREPARELOADIEP				ICCM_SERVICE_CLASS_KCS + 11
#define	KCS_LOADIEP						ICCM_SERVICE_CLASS_KCS + 12
#define	KCS_PREPAREUNLOADIEP			ICCM_SERVICE_CLASS_KCS + 13
#define	KCS_UNLOADIEP					ICCM_SERVICE_CLASS_KCS + 14
#define	KCS_PREPAREUNLOADRCVIEP			ICCM_SERVICE_CLASS_KCS + 15
#define	KCS_READPLOG1					ICCM_SERVICE_CLASS_KCS + 16
#define	KCS_READPLOG2					ICCM_SERVICE_CLASS_KCS + 17
#define	KCS_READCLOG					ICCM_SERVICE_CLASS_KCS + 18
#define	KCS_READPAN						ICCM_SERVICE_CLASS_KCS + 19
#define KCS_READPAN2					ICCM_SERVICE_CLASS_KCS + 20
#define KCS_READIEPID					ICCM_SERVICE_CLASS_KCS + 21
#define KCS_OPENREADPAN					ICCM_SERVICE_CLASS_KCS + 22
#define	KCS_OPENPREPAREUNLOADRCVIEP		ICCM_SERVICE_CLASS_KCS + 23
#define	KCS_PREPAREUPDATEIEP			ICCM_SERVICE_CLASS_KCS + 24
#define	KCS_UPDATEIEP					ICCM_SERVICE_CLASS_KCS + 25
#define KCS_PINCHANGE					ICCM_SERVICE_CLASS_KCS + 26
#define KCS_PINCHANGE2					ICCM_SERVICE_CLASS_KCS + 27
#define KCS_OPENPINCHANGE				ICCM_SERVICE_CLASS_KCS + 28
#define KCS_OPENPUTKEY					ICCM_SERVICE_CLASS_KCS + 29
#define KCS_CLOSEPUTKEY					ICCM_SERVICE_CLASS_KCS + 30
#define KCS_READPLOG_SH					ICCM_SERVICE_CLASS_KCS + 31	//�������� ���� �߰�

//******************************************************************************
//  V-CASH COMMAND(�ϳ�)
//******************************************************************************
#define	VCASH_SELECTAPPLET				ICCM_SERVICE_CLASS_VCASH + 0
#define	VCASH_SELECTDF					ICCM_SERVICE_CLASS_VCASH + 1
#define	VCASH_CHECKTSBIT				ICCM_SERVICE_CLASS_VCASH + 2
#define	VCASH_READSERIALNUM				ICCM_SERVICE_CLASS_VCASH + 3
#define	VCASH_SELECTINFOFILE			ICCM_SERVICE_CLASS_VCASH + 4
#define	VCASH_READINFOFILE				ICCM_SERVICE_CLASS_VCASH + 5
#define	VCASH_SELECTDATAFILE			ICCM_SERVICE_CLASS_VCASH + 6
#define	VCASH_READDATAFILE				ICCM_SERVICE_CLASS_VCASH + 7
#define	VCASH_READBALANCE				ICCM_SERVICE_CLASS_VCASH + 8
#define	VCASH_READICBALANCE				ICCM_SERVICE_CLASS_VCASH + 9
#define	VCASH_BITRELEASE				ICCM_SERVICE_CLASS_VCASH + 10
#define	VCASH_SELPK						ICCM_SERVICE_CLASS_VCASH + 11
#define	VCASH_INITCREDIT				ICCM_SERVICE_CLASS_VCASH + 12
#define	VCASH_INITREFUND				ICCM_SERVICE_CLASS_VCASH + 13
#define	VCASH_INITRESTORE				ICCM_SERVICE_CLASS_VCASH + 14
#define	VCASH_COMCREDIT					ICCM_SERVICE_CLASS_VCASH + 15
#define	VCASH_COMREFUND					ICCM_SERVICE_CLASS_VCASH + 16
#define	VCASH_COMRESTORE				ICCM_SERVICE_CLASS_VCASH + 17
#define	VCASH_SELECTLOGFILE				ICCM_SERVICE_CLASS_VCASH + 18
#define	VCASH_GETLOGINDEX				ICCM_SERVICE_CLASS_VCASH + 19
#define	VCASH_GETLOGFILE				ICCM_SERVICE_CLASS_VCASH + 20

//******************************************************************************
//  EPB COMMAND(����)
//******************************************************************************
#define EPB_GETPASSBOOK1				ICCM_SERVICE_CLASS_EPB + 0
#define EPB_GETPASSBOOK2				ICCM_SERVICE_CLASS_EPB + 1
#define EPB_GETPASSBOOK3				ICCM_SERVICE_CLASS_EPB + 2
#define EPB_GETPASSBOOK4				ICCM_SERVICE_CLASS_EPB + 3
#define EPB_READEPBWITHACCOUNTINFO		ICCM_SERVICE_CLASS_EPB + 4
#define EPB_READEPBDEPACCOUNTINFO		ICCM_SERVICE_CLASS_EPB + 5
#define EPB_READIBKEPBACCOUNTINFO		ICCM_SERVICE_CLASS_EPB + 6
#define	EPB_CHECKPIN					ICCM_SERVICE_CLASS_EPB + 7
#define	EPB_VERIFYPINCHANGE_NH			ICCM_SERVICE_CLASS_EPB + 8
#define EPB_VERIFYPINCHANGE_HN			ICCM_SERVICE_CLASS_EPB + 9
#define EPB_VERIFYPINCHANGE_KB			ICCM_SERVICE_CLASS_EPB + 10
#define	EPB_GETDATACSN_NH				ICCM_SERVICE_CLASS_EPB + 11
#define	EPB_GETDATACSN_KB				ICCM_SERVICE_CLASS_EPB + 12
#define	EPB_GETDATAKEYVERSION 			ICCM_SERVICE_CLASS_EPB + 13
#define	EPB_GETDATAAPPVERSION			ICCM_SERVICE_CLASS_EPB + 14
#define	EPB_READBASICINFO_NH			ICCM_SERVICE_CLASS_EPB + 15
#define	EPB_READBASICINFO_JK			ICCM_SERVICE_CLASS_EPB + 16
#define	EPB_READACCOUNTINFO_NH			ICCM_SERVICE_CLASS_EPB + 17
#define	EPB_READACCOUNTINFO_JK			ICCM_SERVICE_CLASS_EPB + 18
#define	EPB_INSURREADACCOUNTINFO		ICCM_SERVICE_CLASS_EPB + 19
#define	EPB_CHECKPINCOUNTER				ICCM_SERVICE_CLASS_EPB + 20
#define	EPB_READPRIVATEINFO				ICCM_SERVICE_CLASS_EPB + 21
#define	EPB_READFIRSTACCTINFO			ICCM_SERVICE_CLASS_EPB + 22
#define	EPB_READGENERALACCTINFO			ICCM_SERVICE_CLASS_EPB + 23
#define	EPB_UPDATEGENERALACCTINFO		ICCM_SERVICE_CLASS_EPB + 24
#define	EPB_UPDATEFIRSTACCTINFO			ICCM_SERVICE_CLASS_EPB + 25
#define	EPB_UPDATEPRIVATEINFO			ICCM_SERVICE_CLASS_EPB + 26
#define	EPB_PUTDATAACCTINFOVER			ICCM_SERVICE_CLASS_EPB + 27
#define	EPB_GETCHALLENGE 				ICCM_SERVICE_CLASS_EPB + 28
#define	EPB_GETLIST						ICCM_SERVICE_CLASS_EPB + 29
#define	EPB_CREATESESSION				ICCM_SERVICE_CLASS_EPB + 30
#define	EPB_VERIFYAWCHANGE				ICCM_SERVICE_CLASS_EPB + 31
#define EPB_GETENCIPHER					ICCM_SERVICE_CLASS_EPB + 32
#define EPB_GETDATAVER					ICCM_SERVICE_CLASS_EPB + 33
#define EPB_GETDATAPBINFO				ICCM_SERVICE_CLASS_EPB + 34
#define	EPB_SELECTAPPLET				ICCM_SERVICE_CLASS_EPB + 35

//******************************************************************************
//  MYBI COMMAND(�λ�)
//******************************************************************************
#define IMB_MYBI_GETVERSION				ICCM_SERVICE_CLASS_IMB + 0
#define	IMB_MYBI_CARDINFOREAD			ICCM_SERVICE_CLASS_IMB + 1
#define	IMB_MYBI_1ST_REQ				ICCM_SERVICE_CLASS_IMB + 2
#define	IMB_MYBI_2ND_CHARGE_REQ			ICCM_SERVICE_CLASS_IMB + 3
#define	IMB_MYBI_2ND_UNLOAD_REQ			ICCM_SERVICE_CLASS_IMB + 4
#define	IMB_MYBI_CHANGE_CARD_EFSTAT		ICCM_SERVICE_CLASS_IMB + 5
#define	IMB_MYBI_READ_CARD_LOG			ICCM_SERVICE_CLASS_IMB + 6

//******************************************************************************
//  HI-PASS COMMAND(����)
//******************************************************************************
#define HPS_SELECTAPPLET				ICCM_SERVICE_CLASS_HPS + 0
#define	HPS_READBALANCE					ICCM_SERVICE_CLASS_HPS + 1
#define	HPS_PREPARELOADIEP				ICCM_SERVICE_CLASS_HPS + 2
#define	HPS_LOADIEP						ICCM_SERVICE_CLASS_HPS + 3
#define	HPS_READLLOG					ICCM_SERVICE_CLASS_HPS + 4
#define	HPS_READPAN						ICCM_SERVICE_CLASS_HPS + 5
#define	HPS_COMPLETETRAN				ICCM_SERVICE_CLASS_HPS + 6
#define	HPS_READEFIEF					ICCM_SERVICE_CLASS_HPS + 7

//******************************************************************************
//  T-MONEY COMMAND(����)
//******************************************************************************
#define	TMNY_SELECTAPPLET				ICCM_SERVICE_CLASS_TMNY + 0
#define	TMNY_GETDATAREADBALANCE			ICCM_SERVICE_CLASS_TMNY + 1
#define	TMNY_PREPAREUNLOAD				ICCM_SERVICE_CLASS_TMNY + 2
#define	TMNY_UNLOAD						ICCM_SERVICE_CLASS_TMNY + 3
#define	TMNY_PREPARELOAD				ICCM_SERVICE_CLASS_TMNY + 4
#define	TMNY_LOAD						ICCM_SERVICE_CLASS_TMNY + 5

//******************************************************************************
//  UPAY COMMAND(����)
//******************************************************************************
#define	UPAY_SELECTAPPLET				ICCM_SERVICE_CLASS_UPAY + 0
#define	UPAY_GETDATAREADBALANCE			ICCM_SERVICE_CLASS_UPAY + 1
#define	UPAY_PREPAREUNLOAD				ICCM_SERVICE_CLASS_UPAY + 2
#define	UPAY_PREPARELOAD				ICCM_SERVICE_CLASS_UPAY + 3
#define	UPAY_LOAD						ICCM_SERVICE_CLASS_UPAY + 4

//******************************************************************************
//  UPASS COMMAND(�츮)
//******************************************************************************
#define	UPASS_SELECTAPPLET				ICCM_SERVICE_CLASS_UPASS + 0
#define UPASS_GETDATAREADBALANCE		ICCM_SERVICE_CLASS_UPASS + 1
#define UPASS_PREPAREUNLOAD				ICCM_SERVICE_CLASS_UPASS + 2
#define UPASS_UNLOAD					ICCM_SERVICE_CLASS_UPASS + 3
#define UPASS_PREPARELOAD				ICCM_SERVICE_CLASS_UPASS + 4
#define UPASS_LOAD						ICCM_SERVICE_CLASS_UPASS + 5

//******************************************************************************
//  EB COMMAND(����)
//******************************************************************************
#define	EB_CMD_SELECTAPPLET				ICCM_SERVICE_CLASS_EB + 0
#define	EB_CMD_GETDATAREADBALANCE		ICCM_SERVICE_CLASS_EB + 1
#define	EB_CMD_PREPAREUNLOAD			ICCM_SERVICE_CLASS_EB + 2
#define	EB_CMD_UNLOAD					ICCM_SERVICE_CLASS_EB + 3
#define	EB_CMD_PREPARELOAD				ICCM_SERVICE_CLASS_EB + 4
#define	EB_CMD_LOAD						ICCM_SERVICE_CLASS_EB + 5


//******************************************************************************
//  �Ǹ� ����
//******************************************************************************
#define FIC_REALNAME_JUMIN				0x01		// �ֹε�Ϲ�ȣ
#define FIC_REALNAME_BUSINESS			0x02		// ����� ��Ϲ�ȣ
#define FIC_REALNAME_PASSPORT			0x03		// ���ǹ�ȣ
#define FIC_REALNAME_FOREIGN			0x04		// �ܱ��� ��Ϲ�ȣ

//******************************************************************************
//  ���� ����
//******************************************************************************
#define FIC_ACCOUNT_NORMAL				0x01		// �������
#define FIC_ACCOUNT_CLOSE				0x02		// ��������
#define FIC_ACCOUNT_TEMPSTOP			0x04		// �ӽ���������
#define FIC_ACCOUNT_TRANSFER			0x08		// �̰�����
#define FIC_ACCOUNT_ICCARD				0x10		// ����ȭ������

//******************************************************************************
//  ����ȭ��DF(KCASH) Templete
//******************************************************************************
static BYTE	FIC_KCASH_TAG_FCI		=	{0x6F};			// FCI Templete		Tag
static BYTE	FIC_KCASH_TAG_CARDID	=	{0x45};			// �߱���ID			Tag
static BYTE	FIC_KCASH_TAG_MAKEDAY[2]=	{0x5F, 0x26};	// FCI Templete		Tag
static BYTE	FIC_KCASH_TAG_CARDINFO	=	{0x47};			// ����ȭ�� ����	Tag

//******************************************************************************
//  MYBi FCI Templete
//******************************************************************************
static BYTE	FIC_MYBI_TAG_FCI		=	{0x6F};			// FCI Templete		Tag
static BYTE	FIC_MYBI_TAG_DFNAME		=	{0x84};			// FCI DF NAme		Tag
static BYTE	FIC_MYBI_TAG_PROP		=	{0xA5};			// FCI Proprietary	Tag
static BYTE	FIC_MYBI_TAG_LABEL		=	{0x50};			// FCI APP LAbel	Tag
static BYTE	FIC_MYBI_TAG_FILEINFO[2]=	{0xBF, 0x0C};	// FCI File Inform	Tag

//******************************************************************************
//*  EMV
//******************************************************************************
#define ICEMV				(0x10)		// EMV �ϳ������//HNBV01-00-10 #1(EMV)
#define EMV_CANDIDATELIST_CNT			0x0E		// EMV �ĺ� ��� ���� (14)
#define EMV_CAPKLISTRID_CNT				0x01		// EMV CA List(Key Set)�� RID ����
#define EMV_CAPKLISTKEY_CNT				0x07		// EMV CA List(Key Set)�� RID �� �ִ� Key Set ����

#define EMV_AID_LEN						0x07		// EMV AID Length
#define EMV_ARC_LEN						0x02		// EMV ARC Length
#define EMV_ARC_TRAN_LEN				0x01		// KH0930 ��������
#define EMV_ARQC_LEN					0x08		// EMV ARQC, ARPC Length
#define EMV_ATC_LEN						0x02		// EMV ATC Length
// V05-00-05-#03 EMV �������� ���� --------------------------------------------
#define	EMV_LI_LEN						0x01
#define	EMV_DKI_LEN						0x01
#define	EMV_CVN_LEN						0x01
#define	EMV_M4_CVR_LEN					0x06
#define	EMV_M2_CVR_LEN					0x04
#define	EMV_VS_CVR_LEN					0x04
#define	EMV_DAC_LEN						0x02
#define	EMV_M4_DAC						0x08
#define	EMV_M2_DAC						0x06
#define	EMV_M4_IAD_LEN					18
#define	EMV_M2_NEW_DAC					0x07		
#define	EMV_M2_OLD_DAC					0x06		

#define	EMV_MCHIP_ARQC_LEN				0x0A
#define	EMV_M4_IAD_LEN					18
#define	EMV_M2_OLD_IAD_LEN				8
#define	EMV_M2_NEW_IAD_LEN				9

#define	EMV_DATA_SEND_LEN				127
#define	EMV_DATA_RECV_LEN				20
//-----------------------------------------------------------------------------

#define EMV_FLAG_SUCCESS				0x00		// EMV Success Flag
#define EMV_FLAG_WARNING				0x01		// EMV Warning Flag
#define EMV_FLAG_TERMINATE				0x02		// EMV Terminate Flag
#define EMV_FLAG_INITDATAERROR			0x03		// EMV �ʱⰪ ���� ���� Flag

// EMV ��� : EMV_RESULT_SUCCESS�� EMV_RESULT_FAIL�� ������ TRUE�� FALSE�� ó��
#define EMV_RESULT_TERMINATE			0x02		// ������ �ŷ� ���� Ȥ�� ��õ�
#define EMV_RESULT_REVERSAL				0x03		// ��� �ŷ� �䱸
#define EMV_RESULT_ONLINEDECLINE		0x04		// �߱޻� ���� ����
#define EMV_RESULT_INITDATAERROR		0x05		// EMV �ʱⰪ ���� ����
#define EMV_RESULT_NOTSUPPORT			0x06		// EMV �ŷ��� �������� �ʴ� ī��
#define EMV_MAKE_REVERSAL_DATA			0x07		// ��� ���� ���нø� ����� ���� �ڵ�

// ������ ���� �ڵ尪 �߰�
#define EMV_FBCODE_NOATR				"000001"	// PowerOn ������
#define EMV_FBCODE_ATRERROR				"000002"	// ATR �� �̻�
#define EMV_FBCODE_SELECTERROR			"000004"	// Select Application ����
#define EMV_FBCODE_READERROR			"000008"	// �͹̳� �ൿ �м� �� ��� �߻�
#define EMV_FBCODE_BEFORE_OFFAUTHERR	"000100"	// ī���ൿ �м� ��(offlin ���� �õ� ��)����
#define EMV_FBCODE_AFTER_OFFAUTHERR		"000200"	// �������� ���� �� �¶��� ���� �� ����
#define EMV_FBCODE_ONAUTHERR			"000400"	// ȣ��Ʈ���� �¶��� ���� ���� �� ī��� ���� �� ����
#define EMV_FBCODE_ETC					"010000"	// ��Ÿ

// ������ �����ϸ� �ȵǴ� ���
#define EMV_FBCODE_NOFALLBACK			"FFFFFF"	// ������ �����ϸ� �ȵǴ� ���

//******************************************************************************
//  Template Tag
//******************************************************************************
#define EMV_TAG_CNT						0x21		// EMV Tag ��� ����

static BYTE EMV_TAG_APPID           = {0x4F};		// Application Identifier Tag
static BYTE EMV_TAG_MSTR2IMAGE      = {0x57};		// MS Track 2 Image Tag
static BYTE EMV_TAG_PAN             = {0x5A};		// Primary Account Number Tag
static BYTE EMV_TAG_TRANCC[2]       = {0x5F, 0x2A};	// Transaction Currency Code Tag
static BYTE EMV_TAG_CSN[2]          = {0x5F, 0x34};	// PAN Sequence Number Tag
static BYTE EMV_TAG_DDOL[2]         = {0x5F, 0x53};	// Dynamic Data Authentication Data Object List Tag
static BYTE EMV_TAG_HEXAAMOUNT      = {0x81};		// Transaction Amount(Hexadecimal) Tag
static BYTE EMV_TAG_AIP             = {0x82};		// Application Interchange Profile Tag
static BYTE EMV_TAG_ARC             = {0x8A};		// Authorization Response Code Tag
static BYTE EMV_TAG_CVML            = {0x8E};		// Cardholder Verification Method List Tag
static BYTE EMV_TAG_ARPC            = {0x91};		// Authorization Response Cryptogram Tag
static BYTE EMV_TAG_TVR             = {0x95};		// Terminal Verification Result Tag
static BYTE EMV_TAG_TRANDATE        = {0x9A};		// Transaction Date Tag
static BYTE EMV_TAG_TSI             = {0x9B};		// Terminal Status Infomation Tag
static BYTE EMV_TAG_TRANTYPE        = {0x9C};		// Transaction Type Tag
static BYTE EMV_TAG_BCDAMOUNT[2]    = {0x9F, 0x02};	// Transaction Amount(BCD) Tag
static BYTE EMV_TAG_BCDANAMOUNT[2]  = {0x9F, 0x03};	// Another Transaction Amount(BCD) Tag
static BYTE EMV_TAG_HEXAANAMOUNT[2] = {0x9F, 0x04};	// Another Transaction Amount(Hexadecimal) Tag
static BYTE EMV_TAG_AVN[2]          = {0x9F, 0x09};	// Application Version Number Tag
static BYTE EMV_TAG_IAD[2]          = {0x9F, 0x10};	// Issuer Application Data Tag
static BYTE EMV_TAG_TERMINALCC[2]   = {0x9F, 0x1A};	// Terminal Country Code Tag
static BYTE EMV_TAG_TFL[2]          = {0x9F, 0x1B};	// Transaction Floor Limit Tag
static BYTE EMV_TAG_TRANTIME[2]     = {0x9F, 0x21};	// Transaction Time Tag
static BYTE EMV_TAG_APPCRYPTO[2]    = {0x9F, 0x26};	// Application Cryptogram Tag
static BYTE EMV_TAG_CID[2]          = {0x9F, 0x27};	// Cryptogram Information Data Tag
static BYTE EMV_TAG_TC[2]           = {0x9F, 0x33};	// Terminal Capabilitise Tag
static BYTE EMV_TAG_CVMR[2]         = {0x9F, 0x34};	// Cardholder Verification Method Result Tag
static BYTE EMV_TAG_TERMINALTYPE[2] = {0x9F, 0x35};	// Terminal Type Tag
static BYTE EMV_TAG_APPTRANCOUNT[2] = {0x9F, 0x36};	// Application Transaction Counter Tag
static BYTE EMV_TAG_RANDOMNO[2]     = {0x9F, 0x37};	// Unpredictable Number Tag
static BYTE EMV_TAG_ADDTC[2]        = {0x9F, 0x40};	// Additional Terminal Capabilitise Tag
static BYTE EMV_TAG_TSC[2]          = {0x9F, 0x41};	// Transaction Sequence Counter Tag
static BYTE EMV_TAG_ISR[2]          = {0x9F, 0x5B};	// Issuer Script Result Tag

// Nautilus Hyosung ��ü ����
static BYTE EMV_TAG_TAC_DENIAL[2]   = {0x5F, 0x50};	// Terminal Action Code - Denial Tag
static BYTE EMV_TAG_TAC_ONLINE[2]   = {0x5F, 0x51};	// Terminal Action Code - Online Tag
static BYTE EMV_TAG_TAC_DEFAULT[2]  = {0x5F, 0x52};	// Terminal Action Code - Default Tag
static BYTE EMV_TAG_TDOL[2]         = {0x5F, 0x54};	// Transaction Certificate Data Object List Tag
static BYTE EMV_TAG_PSE[2]          = {0x5F, 0x55};	// Payment System Enviroment Tag

//******************************************************************************
//  ������ ����
//******************************************************************************
#define EMV_CERT_AAC					0x00		// �������� �ź�(Denial)
#define EMV_CERT_TC						0x40		// �������� ����(Approve)
#define EMV_CERT_ARQC					0x80		// �¶��� ����(ARQC)
#define EMV_CERT_AAR					0xC0		// ��Ź(Not Accept)

//******************************************************************************
//  �۽� ���� Type
//******************************************************************************
#define EMV_SENDHOST_ARQC				0x01		// �¶��� ���� ��û ����
#define EMV_SENDHOST_REVERSAL			0x11		// ��� ��û ����

#define EMV_SENDHOST_BC_FORMAT			0x88		// ��� ��û ���� (BC ���� �и� -> ûȣ �����)

//******************************************************************************
//  EMV Default Data Object List
//******************************************************************************
static BYTE EMV_DDO_ATC[5]          = {0x88, 0x00, 0xB0, 0x10, 0x01};	// Additional Terminal Capabilitise
static BYTE EMV_DDO_AVN[2]          = {0x00, 0x8C};						// Application Version Number(EMV 2000)
static BYTE EMV_DDO_TAC_DENIAL[5]   = {0x00, 0x00, 0x00, 0x00, 0x00};	// Terminal Action Code - Denial
static BYTE EMV_DDO_TAC_ONLINE[5]   = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};	// Terminal Action Code - Online
static BYTE EMV_DDO_TAC_DEFAULT[5]  = {0x00, 0x00, 0x00, 0x00, 0x00};	// Terminal Action Code - Default
static BYTE EMV_DDO_TC_9F33[3]      = {0x60, 0x40, 0x20};				// Terminal Capabilitise
static BYTE EMV_DDO_TFL[4]          = {0x00, 0x00, 0x00, 0x00};			// Transaction Floor Limit
static BYTE EMV_DDO_TT_9F35         = {0x14};							// Terminal Type
static BYTE EMV_DDO_TCC_5F2A[2]     = {0x04, 0x10};						// Transaction Currency Code. �ѱ�(0410)
static BYTE EMV_DDO_TCC_9F1A[2]     = {0x04, 0x10};						// Terminal Country Code. �ѱ�(0410)
static BYTE EMV_DDO_TC_9C           = {0x01};							// Transaction Type(�Ϲݰŷ�)
static BYTE EMV_DDO_TC_PINCHANGE    = {0x70};							// Transaction Type(PIN Change)
static BYTE EMV_DDO_TC_PINUNBLOCK   = {0x72};							// Transaction Type(PIN Unblock)
static BYTE EMV_DDO_DDOL[9]         = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
																		// Dynamic Data Authentication Data Object List
static BYTE EMV_DDO_TDOL[8]         = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
																		// Transaction Certificate Data Object List
static char EMV_DDO_PSE[20]         = "1PAY.SYS.DDF01";					// Payment System Enviroment

//******************************************************************************
//  ARC Data Type
//******************************************************************************
static BYTE EMV_ARC_SUCCESS[2]      = {0x30, 0x30};                    // Success "00"
static BYTE EMV_ARC_REVERSAL[2]     = {0x30, 0x32};                    // Reversal "02"
static BYTE EMV_ARC_REFERRAL[2]     = {0x30, 0x31};                    // Referral "01"
static BYTE EMV_ARC_DECLINE[2]      = {0x30, 0x35};                    // Decline "05"

static BYTE EMV_ARC_TERMINALY1[2]   = {0x59, 0x31};                    // Offline Approved "Y1"
static BYTE EMV_ARC_TERMINALY2[2]   = {0x59, 0x32};                    // Approval(After a Card Initiated Referral) "Y2"
static BYTE EMV_ARC_TERMINALY3[2]   = {0x59, 0x33};                    // Unable to Go Online, Offline Approved "Y3"
static BYTE EMV_ARC_TERMINALZ1[2]   = {0x5A, 0x31};                    // Offline Decline "Z1"
static BYTE EMV_ARC_TERMINALZ2[2]   = {0x5A, 0x32};                    // Declined(After a Card Initiated Referral) "Z2"
static BYTE EMV_ARC_TERMINALZ3[2]   = {0x5A, 0x33};                    // Unable to Go Online, Offline Declined "Z3"

/*==============================================================================
* �������� ���� Definition.							// V04.10.01 #1
* =============================================================================*/
#define ELECPB_FCI_LEN					0x22		// �������� FCI ���� Length			
#define ELECPB_CAN_LEN					0x08		// �������� CAN Length				
#define ELECPB_KEY_LEN					0x01		// �������� KEY���� Length			
#define ELECPB_APP_LEN					0x02		// �������� APP���� Length			
#define ELECPB_BASICINFO_LEN			0x3C		// �������� �⺻���� ���� Length(60)
#define ELECPB_ACCINFO_LEN				0x3E		// �������� �������� ���� Length(62)
#define ELECPB_MAX_ACCOUNINFO			0x1E		// �������� Account �������� Max(30)
#define ELECPB_ENCIPHER_LEN				0x20		// �������� ENCIPHER Length(32)		// V01.01.09_#01

//K���λ����Ұ�

/*==============================================================================
*  ��������			// LDC_20040204_A
* ============================================================================*/
#define ELECPB_AID_LEN					0x07		// �������� AID Length
#define ELECPB_PRIVATE_LEN				0x60		// �������� ���� ����						
#define ELECPB_FIRSTACCTNUM_LEN			0x3a		// �켱���¹�ȣ ����
#define ELECPB_RETRYCOUNT_PIN_LEN		0x02		// Retry Counter ����
#define ELECPB_PIN_LEN					0x08		// PIN Length
#define ELECPB_CREATE_SESSION_LEN		0x14		// Create Session					// LDC_20040213_A
#define ELECPB_UPDATE_FIRSTINFO_LEN		0x54		// First Info Updatebinary Length	// LDC_20040226_A

#define GET_DATA_CSN_LEN				0x08		// GetData CSN Len
#define GET_DATA_VERSION_LEN			0x08		// GetData Version
#define GET_CHALLENGE_LEN				0x10		// Get Challege						// LDC_20040213_A
#define PUT_DATA_ACCTINFO_LEN			0x02		// Put Data Account Info Version	// LDC_20040213_A
#define	TR_MAC1_LEN						0x14		// TR + MAC1 

/*==============================================================================
*  �����������. 20040204-LDC-A. 20040503-HSW-M
* ============================================================================*/
#define EPB_GETDATA_LEN					0x08		// Get Data CSN, Get Data Version Len
#define EPB_MAX_PRI_ACC_COUNT			0x02		// Max. �켱���� ����(2)
#define EPB_MAX_NOR_ACC_COUNT			0x1C		// Max. �⺻���� ����(28)
#define EPB_ACCOUNT_DATA_LEN			0x2A		// �������� ���� ����
#define EPB_PERSION_DATA_LEN			0x60		// �������� ���� ����
#define EPB_MAX_ACC_ONETIME				0x05		// �� ���� ������ �� �ִ� �ִ� ���� ����

//******************************************************************************
//  Template Tag
//******************************************************************************
static BYTE EPB_TAG_DATATEMP          = {0x70};		// Data Template Tag
static BYTE ELECPB_FIRSTACCT_DATA_LEN	= {0x1d};	// �켱 �������� ����Ÿ ����

//******************************************************************************
//  User Define & ��������(Electronic PassBox Class Byte)
//******************************************************************************
#define	CLA_USER_READBINARY				0x00
#define CLA_USER_UPDATEBINARY			0x00									// LDC_20040226_A
//#define	CLA_USER_PINCHECK				0x20
#define CLA_USER_GETDATACSN				0xCA
#define CLA_USER_GETDATAVER				0xCA
#define CLA_USER_GETPINREMAIN			0x20
#define CLA_USER_CREATESESSION			0x90									// LDC_20040213_A
#define CLA_USER_PUTDATAACCTINFOVER		0xDA									// LDC_20040213_A

//******************************************************************************
//  User Define & ��������(Electronic PassBox INS Byte)
//******************************************************************************
//#define INS_PIN_VERIFY					0x20		// PIN VERIFY
#define INS_USER_READBINARY				0xB0		// Read Binary
#define INS_USER_PINRTYCOUNTER			0x20		// Pin Retry Counter
#define INS_USER_UPDATEBINARY			0xD6		// Update Binary			// LDC_20040226_A			
#define INS_GET_CHALLENGE				0x84		// Get Challenge			// LDC_20040213_A
#define	INS_CREATESESSION				0x8A		// Create Session			// LDC_20040213_A

//******************************************************************************
//  User Define & HiPass Instruction Byte
//******************************************************************************
#define INS_TMNY_READBALANCE			0x4C		// READ BALANCE
#define INS_TMNY_INITCOMMAND			0x40		// Initialize Command(Load,UnLoad Prepare)
#define INS_TMNY_UNLOAD					0x44		// UnLoad
#define INS_TMNY_LOAD					0x42		// Load

#define TMNY_READBAL_LEN				0x04		// �ܾ���ȸ ����
#define TMNY_AMOUNT_LEN					0x04		// �ݾױ���
#define TMNY_INITCMD_LEN				0x1E		// Initialize Command ����
#define TMNY_UNLOAD_LEN					0x10		// ȯ�Ұŷ� ����
#define TMNY_UNLOAD_RESP_LEN			0x08		// ȯ�Ұŷ� ���䵥��Ÿ ����
/////////////////////////////////////////////////////////////////////////////

//******************************************************************************
//  User Define General Info 
//******************************************************************************
#define GENERALACCTNUM_FIRST_LEN		0xDD		
#define GENERALACCTNUM_SECOND_LEN		0xDC
#define	GENERALACCTNUM_THIRD_LEN		0xB0
//******************************************************************************
//  User Define Parameter 1, 2 Byte
//******************************************************************************
#define P1P2_GENERALACCTNUM_ZERO		0x00		// �Ϲݰ��� ���� ZERO
#define P1_GENERALACCTNUM				0x81		// �Ϲݰ�������
#define P1_GENERALACCTNUM_NEXT			0x01		// �Ϲݰ�������
#define P1_FIRSTACCTNUM					0x82		// �켱���¹�ȣ
#define P1_REGISTERNUM					0x83		// �ֹι�ȣ

#define P2_PIN_VERIFY_USER_PIN			0x01		// �� �Է½� ����� Pin
#define P2_PIN_VERIFY_MANAGE_PIN		0x02		// �� �Է½� ���� Pin
#define P1_GET_DATA_CSN					0x01		// Get Data CSN
#define P2_GET_DATA_CSN					0x01		// Get Data CSN
#define P1_GET_DATA_VERSION				0x01		// Get Data Version
#define P2_GET_DATA_VERSION				0x02		// Get Data Version
#define P1_GET_CHALLENGE				0x00		// Get Challenge	// LDC_20040213_A
#define P2_GET_CHALLENGE				0x00		// Get Challenge	// LDC_20040213_A
#define P1_CREATE_SESSION				0x00		// Create Session	// LDC_20040213_A
#define P2_CREATE_SESSION				0x02		// Create Session	// LDC_20040213_A
#define P1_PUT_DATA_ACCTINFOVER			0x01		// Put Data Account Info Version	// LDC_20040213_A
#define P2_PUT_DATA_ACCTINFOVER			0x02		// Put Data Account Info Version	// LDC_20040213_A
#define P1P2_UPDATE_GENERAL				0xEF		// Update General AcctInfo  // LDC_20040226_A
/////////////////////////////////////////////////////////////////////////////
//  �����ڵ� ���� ����
/////////////////////////////////////////////////////////////////////////////
#define FC_MAX_ERRCNT					28
#define FC_ERROR_LEN					4

typedef struct tagFCErr {
	BYTE FC;
	BYTE Err1;
	BYTE Err2;
	BYTE Err3;
	char ICErrGuide[13]; 
} FCErr;

static FCErr FCErrMsg[] =
{
//							"123456789012"
	0xD2, 0x42, 0x30, 0x30,	"FILE  �̹߰�",			// 1	
	0xD2, 0x42, 0x30, 0x35,	"����    ����",			// 2
	0xD2, 0x42, 0x20, 0x20,	"POWER ON ERR",			// 3
	0xD2, 0x42, 0x21, 0x21,	"����    ����",			// 4
	0xD2, 0x42, 0x22, 0x22,	"INVALID FILE",			// 5
	0xD2, 0x42, 0x23, 0x23,	"����    �Ҹ�",			// 6
	0xD2, 0x42, 0x24, 0x24,	"������ǿ���",			// 7
	0xD2, 0x42, 0x33, 0x33,	"�߰��ڷ�䱸",			// 8
	0xD2, 0x42, 0x34, 0x34,	"������  ���",			// 9
	0xD2, 0x42, 0x45, 0x45,	"�̹߰�  ���",			// 10
	0xD2, 0x42, 0x46, 0x46,	"GET DATA ERR",			// 11
	0xD2, 0x42, 0x47, 0x47,	"INIT IEP ERR",			// 12
	0xD2, 0x42, 0x60, 0x60,	"WRITE ERROR ",			// 13
	0xD2, 0x42, 0x61, 0x61,	"������ǿ���",			// 14
	0xD2, 0x42, 0x62, 0x62,	"����    ����",			// 15
	0xD2, 0x42, 0x63, 0x63,	"LOG ��Ͻ���",			// 16
	0xD2, 0x42, 0x65, 0x65,	"��й�ȣƲ��",			// 17
	0xD2, 0x42, 0x67, 0x67,	"READ BIN ERR",			// 18
	0xD2, 0x42, 0x71, 0x71,	"NOT SUPPORT ",			// 19
	0xD2, 0x42, 0x80, 0x80,	"PIN ��Ͽ���",			// 20
	0xD2, 0x42, 0x82, 0x82,	"�ŷ�ȸ���ʰ�",			// 21
	0xD2, 0x42, 0x83, 0x83,	"�ѵ��ʰ�����",			// 22
	0xD2, 0x42, 0x84, 0x84,	"��� 5ȸƲ��",			// 23
	0xD2, 0x42, 0x85, 0x85,	"STOP ERROR  ",			// 24
	0xD2, 0x42, 0x09, 0x09,	"�ܾ���ȸ����",			// 25
	0xD2, 0x42, 0x0A, 0x0A,	"�ܾ���ȸ����",			// 26
	0xD2, 0x42, 0x90, 0x90,	"����    ����",			// 27
	0xD2, 0x42, 0x94, 0x94,	"�ܾ���ȸ����",			// 28
	0xD2, 0x42, 0x62, 0x85,	"STOP ERROR  "			// 29			2004.02.19
													// 9999 : ��Ÿ����
};

/////////////////////////////////////////////////////////////////////////////
//  RFD ��� ����
/////////////////////////////////////////////////////////////////////////////
#define	COM_BASICINF		1
#define	COM_CHARGE			2
#define COM_DEVSTAT			3
#define COM_DEVINIT			4
#define COM_SELMODE			5
#define COM_DRAWBACK		6

// V06-13-00-#01 Hi-Pass���˽��������� ����������������������������������������#0164 
//******************************************************************************
//  User Define & HiPass Instruction Byte
//******************************************************************************
#define	INS_HPS_READBALANCE				0x5C		// READ BALANCE
#define INS_HPS_PRELOADIEP				0x50		// PREPARE LOAD
#define INS_HPS_LOADIEP					0x52		// LOAD
#define INS_HPS_COMPLETE				0x5E		// COMPLETE

#define P1_7816_SELECTION_BY_HPSPAN		0x82
#define P2_7816_SELECTION_BY_HPSLLOG	0x34
// ����������������������������������������������������������������������������


// Hi-Pass���˽��������� ����������������������������������������#0164 
//==============================================================================
// HiPass ���� �����ڵ�
//==============================================================================
/* 2008.08.05 �系 ��å�������� �����ڵ� ������
// IC_MFC_VALUE ���� ���� �����ڵ�
#define IC_SFC_HPSREADJAN				0x02		// �ܾ���ȸ
#define IC_SFC_HPSPRELOAD				0x03		// ������ó��
#define IC_SFC_HPSLOAD					0x04		// ����
#define IC_SFC_HPSCOMPLETE				0x05		// �����Ϸ�(���)
#define IC_SFC_HPSREADLLOG				0x06		// ��ġ���� �ŷ�����
#define	IC_SFC_HPSREADPAN				0x07		// PAN�б�
*/
#define	HPS_READPAN_RECVLEN				0x0A		// READPAN ���ű���
#define HPS_REALBAL_RECVLEN				0x04		// �ܾ���ȸ ���ű���
#define HPS_INITLOAD_LEN				0x04		// ������ó�� ����
#define HPS_INITLOAD_RECVLEN			0x2C		// ������ó�� ���ű���
#define HPS_LOADIEP_LEN					0x0C		// ���� ����
#define	HPS_LOADIECP_RECVLEN			0x04		// ���� ���ű���
#define HPS_READLLOG_RECVLEN			0x24		// ������� ����
#define HPS_COMPLETE_LEN				0x00		// �����Ϸ� ����
// ����������������������������������������������������������������������������

// V06-13-00-#01 Hi-Pass���˽��������� ������������������������������������#0164
//******************************************************************************
//  HiPass FCI Templete
//******************************************************************************
static BYTE	HIC_HPSS_TAG_FCI		=	{0x6F};			// FCI Templete		Tag
static BYTE	HIC_HPSS_TAG_DFNAME		=	{0x84};			// FCI DF Name		Tag
static BYTE	HIC_HPSS_TAG_PROP		=	{0xA5};			// FCI Proprietary	Tag
static BYTE	HIC_HPSS_TAG_SFI		=	{0x88};			// SFI				Tag
static BYTE	HIC_HPSS_TAG_FILEINFO[2]=	{0xBF, 0x0C};	// FCI File Inform	Tag
// ����������������������������������������������������������������������������


/////////////////////////////////////////////////////////////////////////////
#endif
