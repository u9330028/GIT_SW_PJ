/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCmnDefine_H_)
#define _TranCmnDefine_H_

#if !defined(KB_SOFO)
#define KB_SOFO
#endif

/* **************************************************************************
//
// Nautilus Hyosung Inc. 2005 All Rights Reserved
//
// ���ϸ� : KtxCardIntf.h
// ��  �� : Dll �������
// ��  �� : 2005/08/29 �ű��ۼ� (gijin)
//  
// *************************************************************************/
#ifndef XDllExport
	#ifdef __cplusplus
		#define XDllExport   extern "C" __declspec( dllexport )
	#else 
		#define XDllExport __declspec( dllexport )
	#endif
#endif


// CHB (0411) Encrypt  (bank encrypt)
/*******************************************************************************************************************/
typedef struct MsgData
{
	char cs_acctno[24];   /*  ���¹�ȣ 23�ڸ�  */
	char cs_txamt[13];    /*  �ŷ��ݾ� 12�ڸ�  */
	char cs_serialno[4];  /*  ī��ȸ��  3�ڸ�  */
	char cs_custno[9];    /*  ��й�ȣ �� ��ȿ�Ⱓ 8�ڸ�  */
}MSG_DATA, *PMSG_DATA;

XDllExport void   WINAPI WooriSeed( char *key, int index, char *pin, char *epin );
XDllExport LPCSTR chb_convert(MSG_DATA m_pMsgData);
XDllExport void	  HouseSeed(char *mk,int index,char *sour,char *dest,int opt);
XDllExport void   HanaSeed(char *key,int option, int length, char *sour, char *dest,int check);
XDllExport int    PEMencrypt( char *pin, char *xprday, char *acno, char *epin ); 
XDllExport int __stdcall ax_encrypt(char* ibuf, int l_ibuf, char* keyhdr, int l_keyhdr, char* obuf);


/******************************************************************************************************************/

#define		LINE_CLEAR				0
#define		LINE_OPEN				'O'
#define		LINE_CLOSE				'C'

#define		UDP_SYNC				0
#define		UDP_ASYNC				1

#define		TCP_TSEC				3

#define		_BK_E_INI				1
	
#define		_KEYTABELE_INI			0	
#define		_ORGENCRYPT_INI			1	
#define		_CARDBINNO_NO			2
#define		_ATM_CFG_INI_T 			3
#define		_ATM_MOBILE_INI_T 		4

#define		ATM_SEND_ASYSC			2
#define		SEND_ENCRYPT			1
#define		RECV_DECRYPT			2

#define		LOGTRACE				10
#define		JOURNALTRACE			20 

#define		RET_OK					0 
#define RET_EMV                                            473                 // EMV
#define RET_EMVREADERROR                                   474                 // EMVREAD����

#define		_JOURNAL_ROOT				_T("D:\\T1ATM")
#define		_DIR_JOURNAL				_T("D:\\T1ATM\\Journal")
#define		_DIR_EJOURNAL				_T("D:\\T1ATM\\EJournal")
#define		_DIR_MOBILEGAME				_T("D:\\MobileGame")
#define		_B_E_INFO_INI				_T("D:\\INI\\B_E_INFO.INI")
#define		_ATMKEYTABELE_INI			_T("D:\\INI\\KEYTABLE.INI")
#define		_ATMTRCODEENCRYPT_INI		_T("D:\\INI\\TRCODEENCRYPT.INI")
#define		_ECASH_SVR_RES_INI			_T("D:\\INI\\ECASH_SVR_RESPONSE.INI")
#define		_HOST_STATUS_INI			_T("C:\\T1ATM\\INI\\HOST_STATUS.INI")

#define		KB_104_CERT					_T("C:\\T1ATM\\App\\Main\\qbank_cert.scb")
#define		KUB_103_CERT				_T("C:\\T1ATM\\App\\Main\\KUB103CERT.der")
#define		NON_111_CERT				_T("C:\\T1ATM\\App\\Main\\nh_cli.key")
#define		SC_123_CERT				    _T("C:\\T1ATM\\App\\Main\\SC_CLI.KEY")
#define		LOCALDIREXE					_T("C:\\T1ATM\\App\\Main\\ECash_Simulator.exe")
#define		HANA_ENCRIPT_DIR			_T("C:\\T1ATM\\App\\Main\\key.dat")
#define		NON_ENCRIPT_DIR				_T("C:\\T1ATM\\App\\Main\\ctx.bin")
#define		SC_ENCRIPT_DIR				_T("C:\\T1ATM\\App\\Main\\SC.bin")

#define		_ENCRYPT_ERROR_MSG1			_T("ATM_(������/��ȣȭ) ��ȿ�Ⱓ EXFIRE!!| ����� �����Ͽ� �ֽʽÿ� ???")
#define		_ENCRYPT_ERROR_MSG2			_T("ATM_(������/��ȣȭ) ��ȿ�Ⱓ EXFIRE!!| ����� �����Ͽ� �ֽʽÿ� ???")
#define		_COMM_ERROR_MSG3			_T("��������ġ(88501)|��������ų����� Ȯ�� �Ͻʽÿ� ???")


// EN/DISABLE 
#define		ENABLE			1
#define		DISABLE			2

#define		PLAINTEXT		1
#define		CIPHERTEXT		2
#define		CIPHERTEXT_TMP	3

#define		SEND_F			0			//	C_SUCCESS
#define		RECV_F			1			//	C_SUCCESS

// KEY SUCCESS (SOFO, INITECH)
#define		C_SUCCESS		0			//	C_SUCCESS

#define		K_CERTIFY_C		1			//	C_SUCCESS
#define		K_VERIFY_C		2			//	C_SUCCESS
#define		C_F				0			//	CHECK FLAG
#define		D_E				1			//	DATA_ENCRYPT
#define		D_D				2			//	DATA_DECRYPT
#define		C_R				1			// ��������ϳ���
#define		C_V				2			// ��������ȿ����
#define		K_R				3			// ����Ű��ϳ���
#define		K_V				4			// ����Ű��ȿ����	
#define		K_S_F			5			//	KEY_SETUP_FLAG
#define		K_N_C			4			//	KEY_NON_CHECK
#define		K_C_S			10			//	KEY_CERTIFY_SUCCESS
#define		K_C_F			11			//	KEY_CERTIFY_FAIL
#define		K_V_S			20			//	KEY_VERIFY_SUCCESS
#define		K_V_F			21			//	KEY_VERIFY_FAIL

#define		TX_SEND_F		1
#define		RX_RECV_F		2

#define		PACK_F	    	1
#define		UNPACK_F		2

#define		NH_ATM_TYPE			_T("S0") //ECASH ���� ���� �ڵ�


#define		ATM_CFG_SEC			_T("ATMINFO")
#define		ATM_MOBILE_SEC		_T("MOBILE")
#define		KBS_ORG_SEC			_T("006") //����
#define		KUB_ORG_SEC			_T("003") //���
#define		NON_ORG_SEC			_T("011") //����
#define		SCO_ORG_SEC			_T("023") //����
#define		WRT_ORG_SEC			_T("247") //NH��������
#define		SST_ORG_SEC			_T("240") //�Ｚ����
#define		HWT_ORG_SEC			_T("269") //��ȭ����
#define		KJB_ORG_SEC			_T("034") //��������
#define		KEB_ORG_SEC			_T("005") //��ȯ����
#define		WRB_ORG_SEC			_T("020") //�츮����
#define		DBS_ORG_SEC			_T("279") //����


// ATM ORG CODE encrypt 
#define		x00_ORG_INT_000		0  
#define		KBS_ORG_INT_004		4  
#define		KBS_ORG_INT_006		6  
#define		KBS_ORG_INT_029		29 
#define		KBS_ORG_INT_030		30 
#define		KUB_ORG_INT_003		3 
#define		NON_ORG_INT_011		11 
#define		NON_ORG_INT_012		12 
#define		NON_ORG_INT_016		16 
#define		NON_ORG_INT_017		17 
#define		SCO_ORG_INT_023		23 
#define		WRT_ORG_INT_247		247
#define		SST_ORG_INT_240		240
#define		SST_ORG_INT_CVT_240	364
#define		HWT_ORG_INT_269		269
#define		WRT_ORG_INT_278		278
#define		KJB_ORG_INT_034		34   
#define 	KEB_ORG_INT_005		5	//��ȯ����
#define 	WRB_ORG_INT_020		20	//�츮
#define		DOB_ORG_INT_279		279	  

// ATM SEND ENCRYT METHOD
#define		TRAN_ENC_000_00    0			//��Ÿ���� 
#define 	TRAN_ENC_006_01	   1			//��������	(30,29,04,06,09)
#define		TRAN_ENC_034_02    2			//���� (34)  
#define 	TRAN_ENC_026_03	   3			//��������,���� (21,26)     
#define 	TRAN_ENC_025_04	   4			//�ϳ�����  (25,81,82,33)   			
#define 	TRAN_ENC_032_05	   5			//�λ�����  (32)    
#define 	TRAN_ENC_003_06	   6			//�������  (03)	
#define 	TRAN_ENC_011_07	   7			//����      (17,16,12,11)
#define 	TRAN_ENC_023_08	   8			//����      (23)
#define 	TRAN_ENC_247_09	   9			//NH�������� (207)
#define 	TRAN_ENC_240_10	   10			//�Ｚ���� (240)
#define 	TRAN_ENC_269_11	   11			//��ȭ�������� (269)
#define 	TRAN_ENC_218_12	   12			//�������� (218)
#define 	TRAN_ENC_270_13	   13			//�ϳ��������� (270)
#define 	TRAN_ENC_071_14	   14			//��ü�� (071)
#define 	TRAN_ENC_238_15	   15			//�������(238)
#define 	TRAN_ENC_267_16	   16			//�������(267)
#define 	TRAN_ENC_278_17	   17			//�¸�׽�������
#define 	TRAN_ENC_027_18	   18			//��Ƽ�귣��

#define nEnCmd000_0			0		//"????????????"      
#define nEnCmd001_1			1		//"�ѱ�����    "      
#define nEnCmd002_2			2		//"�������    "      
#define nEnCmd003_3			3		//"�������    "      
#define nEnCmd005_5			5		//"��ȯ����    "      
#define nEnCmd004_6			6		//"��������    "      
#define nEnCmd006_6			6		//"��������    "      
#define nEnCmd009_6			6		//"��������    "      
#define nEnCmd029_6			6		//"��������    "      
#define nEnCmd030_6			6		//"��������    "      
#define nEnCmd007_7			7		//"�����߾�ȸ  "      
#define nEnCmd008_8			8		//"����������  "      
#define nEnCmd011_11		11		//"�����߾�ȸ  "    
#define nEnCmd016_11		11		//"�����߾�ȸ  "    
#define nEnCmd012_12		12		//"��������    "    
#define nEnCmd013_12		12		//"��������    "    
#define nEnCmd014_12		12		//"��������    "    
#define nEnCmd015_12		12		//"��������    "    
#define nEnCmd017_12		12		//"��������    "    
#define nEnCmd020_20		20		//"�츮����    "    
#define nEnCmd022_20		20		//"�츮����    "    
#define nEnCmd024_20		20		//"�츮����    "    
#define nEnCmd083_20		20		//"�츮����    "    
#define nEnCmd023_23		23		//"��������    "    
#define nEnCmd027_27		27		//"��Ƽ����    "    
#define nEnCmd053_53		53		//"��Ƽ����    "    
#define nEnCmd031_31		31		//"�뱸����    "    
#define nEnCmd032_32		32		//"�λ�����    "    
#define nEnCmd034_34		34		//"��������    "    
#define nEnCmd035_35		35		//"��������    "    
#define nEnCmd037_37		37		//"��������    "    
#define nEnCmd039_39		39		//"�泲����    "    
#define nEnCmd045_45		45		//"�������ݰ�  "    
#define nEnCmd048_48		48		//"�����߾�ȸ  "    
#define nEnCmd050_50		50		//"��ȣ��������"    
#define nEnCmd054_54		54		//"HSBC����    "    
#define nEnCmd055_55		55		//"����ġ����  "    
#define nEnCmd056_56		56		//"ABN����     "    
#define nEnCmd071_71		71		//"��ü��      "    
#define nEnCmd072_71		71		//"��ü��      "    
#define nEnCmd073_71		71		//"��ü��      "    
#define nEnCmd074_71		71		//"��ü��      "    
#define nEnCmd075_71		71		//"��ü��      "    
#define nEnCmd025_81		81		//"�ϳ�����    "    
#define nEnCmd033_81		81		//"�ϳ�����    "    
#define nEnCmd081_81		81		//"�ϳ�����    "    
#define nEnCmd082_81		81		//"�ϳ�����    "    
#define nEnCmd021_88		88		//"��������    "    
#define nEnCmd026_88		88		//"��������    "    
#define nEnCmd028_88		88		//"��������    "    
#define nEnCmd038_88		88		//"��������    "    
#define nEnCmd040_88		88		//"��������    "    
#define nEnCmd088_88		88		//"��������    "    
#define nEnCmd105_105 		105		//"KEB�ϳ�ī�� " 
#define nEnCmd105_115 		115		//"KEB�ϳ�ī�� " 
#define nEnCmd105_115 		115		//"��ȯī��    "    
#define nEnCmd106_106 		106		//"����ī��    "    
#define nEnCmd107_107 		107		//"����ī��    "    
#define nEnCmd111_111 		111		//"��������    "    
#define nEnCmd120_120 		120		//"�츮ī��    "    
#define nEnCmd121_121 		121		//"����ī��    "    
#define nEnCmd126_192 		192		//"����ī��    "    
#define nEnCmd127_127 		127		//"��Ƽī��    "    
#define nEnCmd134_134 		134		//"����ī��    "    
#define nEnCmd135_135 		135		//"���ֺ���    "    
#define nEnCmd137_137 		137		//"����ī��    "    
#define nEnCmd181_105 		105		//"KEB�ϳ�ī�� "    
#define nEnCmd184_184 		184		//"�Ｚī��    "    
#define nEnCmd192_192 		192		//"����ī��    "    
#define nEnCmd193_193 		193		//"����ī��    "    
#define nEnCmd194_194 		194		//"BCī��      "    
#define nEnCmd195_195 		195		//"�Ե�ī��    "    
#define nEnCmd209_369 				369		//"����Ÿ����  "    
#define nEnCmd209_379_PUBLIC 		379		//"����Ÿ����  "    
#define nEnCmd218_218_PUBLIC 		218		//"��������    "
#define nEnCmd218_218 		218		//"��������    "    
#define nEnCmd230_230 		230		//"�̷�������  "    
#define nEnCmd238_368 		368		//"�������    "    
#define nEnCmd240_364 		364		//"�Ｚ����    "    
#define nEnCmd243_243 		243		//"�ѱ�����    "    
#define nEnCmd247_247 		247		//"�츮����    "    
#define nEnCmd261_261 		261		//"��������    "    
#define nEnCmd262_262 		262		//"��������    "    
#define nEnCmd263_263 		263		//"HMC����     "    
#define nEnCmd264_264 		264		//"Ű������    "    
#define nEnCmd265_265 		265		//"��Ʈ���̵�  "    
#define nEnCmd266_266 		266		//"��������    "    
#define nEnCmd267_267 		267		//"�������    "    
#define nEnCmd268_268 		268		//"�ַθ�����  "    
#define nEnCmd269_269 		269		//"��ȭ����    "    
#define nEnCmd270_270 		270		//"�ϳ�����    "    
#define nEnCmd278_278 		278		//"���ѱ���    "    
#define nEnCmd279_279 		279		//"��������    "    
#define nEnCmd280_280 		280		//"��������    "    
#define nEnCmd287_287 		287		//"�޸�������  "    
#define nEnCmd289_289 		289		//"NH����      "    
#define nEnCmd290_290 		290		//"�α�����    "    
#define nEnCmd291_291 		291		//"�ſ�����    "    
#define nEnCmd292_292 		292		//"LIG����	   "    
#define nEnCmd294_294_BC	294		//"�ؿ� BC	   "    
#define nEnCmd384_364 		364		//"�Ｚ����    "    
#define nEnCmd484_484 		484		//"CJ�ý���    "    
#define nEnCmd601_601 		601		//"�߷���      "    
#define nEnCmd701_701 		701		//"KTF         "    
#define nEnCmd702_702 		702		//"LGT         "    
#define nEnCmd703_703 		703		//"SKT         "    



#define		ASC2INT(x,y)	(Strlen(x) == y ? Asc2Int(x,Strlen(x)) : 0)

#define 	TRAN_MSG_EXCEPTION_700	   700	//������ ������   - 700 ����ó��
#define 	TRAN_MSG_EXCEPTION_707	   707	//��������ȸ �Ұ� - 707 ����ó�� 
#define 	TRAN_MSG_EXCEPTION_2190	   2190	//�������� ����ó��
#define 	TRAN_MSG_EXCEPTION_A010	   10	//�������� ����ó��
#define 	TRAN_MSG_EXCEPTION_A020	   20	//�������� ����ó��
#define 	TRAN_MSG_EXCEPTION_845	   845	
#define 	TRAN_MSG_EXCEPTION_798	   798	
#define 	TRAN_MSG_EXCEPTION_094	   94	


#define TRAN_ORG_INIT_002_N		_T("�������")	  
#define TRAN_ORG_INIT_003_N		_T("�������")	  
#define TRAN_ORG_INIT_004_N		_T("��������")	  
#define TRAN_ORG_INIT_005_N		_T("��ȯ����")	  
#define TRAN_ORG_INIT_006_N		_T("��������")	  
#define TRAN_ORG_INIT_007_N		_T("��������")	  
#define TRAN_ORG_INIT_009_N		_T("��������")	  
#define TRAN_ORG_INIT_011_N		_T("����(�߾�)")  
#define TRAN_ORG_INIT_012_N		_T("����(����)")  
#define TRAN_ORG_INIT_016_N		_T("����(�߾�)")  
#define TRAN_ORG_INIT_017_N		_T("����(����)")  
#define TRAN_ORG_INIT_020_N		_T("�츮����")	  
#define TRAN_ORG_INIT_021_N		_T("��������")	  
#define TRAN_ORG_INIT_022_N		_T("�츮����")	  
#define TRAN_ORG_INIT_023_N		_T("SC����")	  
#define TRAN_ORG_INIT_024_N		_T("�츮����")	  
#define TRAN_ORG_INIT_025_N		_T("�ϳ�����")	  
#define TRAN_ORG_INIT_026_N		_T("��������")	  
#define TRAN_ORG_INIT_027_N		_T("��Ƽ����")	  
#define TRAN_ORG_INIT_029_N		_T("��������")	  
#define TRAN_ORG_INIT_030_N		_T("��������")	  
#define TRAN_ORG_INIT_031_N		_T("�뱸����")	  
#define TRAN_ORG_INIT_032_N		_T("�λ�����")	  
#define TRAN_ORG_INIT_033_N		_T("�ϳ�����")	  
#define TRAN_ORG_INIT_034_N		_T("��������")	  
#define TRAN_ORG_INIT_035_N		_T("��������")	  
#define TRAN_ORG_INIT_036_N		_T("��Ƽ����")	  
#define TRAN_ORG_INIT_037_N		_T("��������")	  
#define TRAN_ORG_INIT_039_N		_T("�泲����")	  
#define TRAN_ORG_INIT_045_N		_T("�������ݰ�")  
#define TRAN_ORG_INIT_048_N		_T("����")	  
#define TRAN_ORG_INIT_050_N		_T("��ȣ��������")
#define TRAN_ORG_INIT_053_N		_T("��Ƽ����")	  
#define TRAN_ORG_INIT_054_N		_T("H.S.B.C")     
#define TRAN_ORG_INIT_071_N		_T("��ü��")      
#define TRAN_ORG_INIT_072_N		_T("��ü��")      
#define TRAN_ORG_INIT_081_N		_T("�ϳ�����")	  
#define TRAN_ORG_INIT_082_N		_T("�ϳ�����")	  
#define TRAN_ORG_INIT_083_N		_T("�츮����")	  
#define TRAN_ORG_INIT_401_N		_T("����ī��")	  
#define TRAN_ORG_INIT_402_N		_T("����ī��")	  
#define TRAN_ORG_INIT_403_N		_T("MASTER  ")    
#define TRAN_ORG_INIT_404_N		_T("��ī��")	  
#define TRAN_ORG_INIT_405_N		_T("�Ｚī��")	  
#define TRAN_ORG_INIT_406_N		_T("�Ե�ī��")	  
#define TRAN_ORG_INIT_407_N		_T("����ī��")	  
#define TRAN_ORG_INIT_408_N		_T("��ȯī��")	  
#define TRAN_ORG_INIT_409_N		_T("VISA   ")     
#define TRAN_ORG_INIT_410_N		_T("JCB   ")      
#define TRAN_ORG_INIT_411_N		_T("CUP	   ")     
#define TRAN_ORG_INIT_412_N		_T("AMEX   ")     
#define TRAN_ORG_INIT_421_N		_T("����ī��")	  	  
#define TRAN_ORG_INIT_430_N		_T("����ī��")	  
#define TRAN_ORG_INIT_432_N		_T("�ϳ�ī��")	  
#define TRAN_ORG_INIT_433_N		_T("��Ƽī��")	  
#define TRAN_ORG_INIT_436_N		_T("��Ƽī��")	  
#define TRAN_ORG_INIT_437_N		_T("�츮ī��")	 

#define TRAN_ORG_SHINCARD	_T("����ī��")
#define TRAN_ORG_KJBCARD	_T("���ֺ���")				//#N0181

#define	SAMSCARDLOAN_RAINBOW	1						// �Ｚī�� ������ī���
#define	SAMSCARDLOAN_HANARO		2						// �Ｚī�� �ϳ���ī���
#define	HYUNCARDLOAN_CDLOAN		3						// ����ī�� CD��
#define	LOTTECARDLOAN			4						// �Ե�ī���
#define	SHINHANCARDLOAN			5						// ����ī���



#define SAMSUNG_LIFE  _T("403") //�Ｚ����
#define SAMSUNG_FIRE  _T("402") //�Ｚȭ��
#define DONGBU_FIRE   _T("401") //����ȭ��
#define SAMSUNG_LOAN  _T("205") //�Ｚī���
#define HYUNDAI_LOAN  _T("202") //����ī���
      
                 
#define MASTER_ORG    _T("203") //MASTER
#define VISA_ORG      _T("209") //VISA
#define JCB_ORG       _T("210") //JCB
#define CUP_ORG       _T("211") //CUP
#define AMEX_ORG      _T("212") //AMEX
#define DINERS_ORG    _T("213") //DINERS
#define DISCOVER_ORG  _T("214") //DISCOVER

#define OK_CASHBAG_ORG _T("501") //OKCASH
#define GSNPOINT_ORG   _T("507")
#define GIRO_TAX_APT  _T("601")
#define TRUST_ORG     _T("502")
#define KMERCE_ORG    _T("503")
#define KOREAN_AIR    _T("701")
#define XCASH_ORG     _T("504")
#define TMONEY_ORG		_T("505")	//T-Money
#define CGV_ORG			_T("703")	//CGV
#define WIRETRANSFER_ORG _T("105")	//�ؿܼ۱�
#define DOTONG_ORG     _T("506")
#define DAMON_ORG      _T("704")
#define PHOTO_ORG      _T("705")
#define CGV_ORG_PRIMUS _T("706")	//CGV
#define CJ_ORG_PIFF	   _T("707")	//CGV
#define FC_ORG_GAME    _T("801")	//CGV
#define EVERLAND_ORG_TICKET _T("802")	//CGV
#define FM_SPORTS_TICKET _T("803")	//CGV
#define GSTICKET_ORG    _T("801")	//CGV
#define FMTICKET_ORG    _T("802")	//CGV
#define GAME_MONEY_ORG	_T("504")

#define ORG_LOAN_SAMSCARD  _T("205") //�Ｚī���
#define ORG_LOAN_HYUNCARD  _T("202") //����ī���
#define ORG_LOAN_LOTTCARD  _T("206") //�Ե�ī���
#define ORG_LOAN_SHINCARD  _T("228") //����ī���




// ���ະ �ڵ����� 
// ����� �ڵ�����
#define		BANK_IB			_T("0159")
#define     KDBBANKID		_T("002")	//�������	                
#define 	KIUBANKID		_T("003")	//�������
	                
#define 	KUMBANKID1		_T("004")	//��������	
#define 	KUMBANKID2		_T("006")	//��������	
#define 	KUMBANKID3		_T("009")	//��������	
#define 	KUMBANKID4		_T("029")	//��������	
#define 	KUMBANKID5		_T("030")	//��������	

#define 	KOEBANKID		_T("005")	//��ȯ����	                
#define 	SUHBANKID		_T("007")	//��������
	         
#define 	NONBANKID1		_T("011")	//����      
#define 	NONBANKID2		_T("012")	//����      
#define 	NONBANKID3		_T("016")	//����      
#define 	NONBANKID4		_T("017")	//����
 
#define 	WORBANKID1		_T("020")	//�츮����  
#define 	WORBANKID2		_T("022")	//�츮����  
#define 	WORBANKID3		_T("024")	//�츮����  
#define 	WORBANKID4		_T("083")	//�츮����  
  
#define 	CHOBANKID		_T("021")	//��������                      
#define 	KOFBANKID		_T("023")	//��������
#define 	KOFBANKIDC		_T("0J2")	//��������

#define 	HANBANKID1		_T("025")	//�ϳ�����  
#define 	HANBANKID2		_T("033")	//�ϳ�����  
#define 	HANBANKID3		_T("081")	//�ϳ�����  
#define 	HANBANKID4		_T("082")	//�ϳ�����
#define 	HANBANKIDC		_T("080")	//�ϳ����� �귣�� ATM ûȣ����ĳ�� ���α�� �ڵ� #N0214 
 
#define 	SHIBANKID		_T("026")	//��������  
#define 	SHIBANKID1		_T("088")	//��������  

#define 	GOOBANKID1		_T("027")	//�ѹ�����  
#define 	GOOBANKID2		_T("036")	//�ѹ�����  

#define 	DEABANKID		_T("031")	//�뱸����                      
#define 	PUSBANKID		_T("032")	//�λ�����                      
#define 	KAWBANKID		_T("034")	//��������                      
#define 	JEJBANKID		_T("035")	//��������                      
#define 	JEOBANKID		_T("037")	//��������                      
#define 	KYOBANKID		_T("039")	//�泲����                      
#define 	KFCBANKID		_T("045")	//�������ݰ�                       
#define 	SINBANKID		_T("048")	//����	                        
#define 	SANBANKID		_T("050")	//��ȣ����                      
#define 	CITBANKID		_T("053")	//��Ƽ����                      
#define 	HSBABNKID		_T("054")	//H.S.B.C

#define 	POSBANKID1		_T("071")	//��ü��  
#define 	POSBANKID2		_T("072")	//��ü��  
#define 	POSBANKID3		_T("073")	//��ü��  
#define 	POSBANKID4		_T("074")	//��ü��  
#define 	POSBANKID5		_T("075")	//��ü�� 

#define     IKDBBANKID		2	//�������	                
#define 	IKIUBANKID		3	//�������
	                
#define 	IKUMBANKID1		4	//��������	
#define 	IKUMBANKID2		6	//��������	
#define 	IKUMBANKID3		9	//��������	
#define 	IKUMBANKID4		29	//��������	
#define 	IKUMBANKID5		30	//��������	

#define 	IKOEBANKID		5	//��ȯ����	                
#define 	ISUHBANKID		7	//��������
	                
#define 	INONBANKID1		11	//����      
#define 	INONBANKID2		12	//����      
#define 	INONBANKID3		16	//����      
#define 	INONBANKID4		17	//����
          
#define 	IWORBANKID1		20	//�츮����  
#define 	IWORBANKID2		22	//�츮����  
#define 	IWORBANKID3		24	//�츮����  
#define 	IWORBANKID4		83	//�츮����  
      
#define 	ICHOBANKID		21	//��������                      
#define 	IKOFBANKID		23	//��������
                      
#define 	IHANBANKID1		25	//�ϳ�����  
#define 	IHANBANKID2		33	//�ϳ�����  
#define 	IHANBANKID3		81	//�ϳ�����  
#define 	IHANBANKID4		82	//�ϳ�����
       
#define 	ISHIBANKID		26	//��������  
#define 	ISHIBANKID1		88	//��������  
                    
#define 	IGOOBANKID1		27	//�ѹ�����  
#define 	IGOOBANKID2		36	//�ѹ�����  
#define 	ICITBANKID		53	//��Ƽ����                      
            
#define 	IDEABANKID		31	//�뱸����                      
#define 	IPUSBANKID		32	//�λ�����                      
#define 	IKAWBANKID		34	//��������                      
#define 	IJEJBANKID		35	//��������                      
#define 	IJEOBANKID		37	//��������                      
#define 	IKYOBANKID		39	//�泲����                      
#define 	IKFCBANKID		45	//�������ݰ�                       
#define 	ISINBANKID		48	//����	                        
#define 	ISANBANKID		50	//��ȣ����                      
#define 	IHSBABNKID		54	//H.S.B.C
	                
#define 	IPOSBANKID1		71	//��ü��  
#define 	IPOSBANKID2		72	//��ü��  
#define 	IPOSBANKID3		73	//��ü��  
#define 	IPOSBANKID4		74	//��ü��  
#define 	IPOSBANKID5		75	//��ü��  

#define 	NHMUINPUT		100	//������Ա�  



#define     KDBINNERID		_T("002")	//�������	                
#define 	KIUINNERID		_T("003")	//�������
#define 	KUMINNERID1		_T("004")	//��������	
#define 	KUMINNERID2		_T("006")	//��������	
#define 	KUMINNERID3		_T("009")	//��������	
#define 	KUMINNERID4		_T("029")	//��������	
#define 	KUMINNERID5		_T("030")	//��������	
#define 	KOEINNERID		_T("005")	//��ȯ����	                
#define 	SUHINNERID		_T("007")	//��������
#define 	NONINNERID1		_T("011")	//����      
#define 	NONINNERID2		_T("012")	//����      
#define 	NONINNERID3		_T("016")	//����      
#define 	NONINNERID4		_T("017")	//����
#define 	WORINNERID1		_T("020")	//�츮����  
#define 	WORINNERID2		_T("022")	//�츮����  
#define 	WORINNERID3		_T("024")	//�츮����  
#define 	WORINNERID4		_T("083")	//�츮����  
#define 	CHOINNERID		_T("021")	//��������                      
#define 	KOFINNERID		_T("023")	//��������
#define 	HANINNERID1		_T("025")	//�ϳ�����  
#define 	HANINNERID2		_T("033")	//�ϳ�����  
#define 	HANINNERID3		_T("081")	//�ϳ�����  
#define 	HANINNERID4		_T("082")	//�ϳ����� 
#define 	SHIINNERID		_T("026")	//��������  
#define 	SHIINNERID1		_T("088")	//��������  
#define 	GOOINNERID1		_T("027")	//�ѹ�����  
#define 	GOOINNERID2		_T("036")	//�ѹ�����  
#define 	DEAINNERID		_T("031")	//�뱸����                      
#define 	PUSINNERID		_T("032")	//�λ�����                      
#define 	KAWINNERID		_T("034")	//��������                      
#define 	JEJINNERID		_T("035")	//��������                      
#define 	JEOINNERID		_T("037")	//��������                      
#define 	KYOINNERID		_T("039")	//�泲����                      
#define 	KFCINNERID		_T("045")	//�������ݰ�                       
#define 	SININNERID		_T("048")	//����	                        
#define 	SANINNERID		_T("050")	//��ȣ����                      
#define 	CITINNERID		_T("053")	//��Ƽ����                      
#define 	HSBINNERID		_T("054")	//H.S.B.C
#define 	POSINNERID1		_T("071")	//��ü��  
#define 	POSINNERID2		_T("072")	//��ü��  
#define 	POSINNERID3		_T("073")	//��ü��  
#define 	POSINNERID4		_T("074")	//��ü��  
#define 	POSINNERID5		_T("075")	//��ü��  


#define 	TMASTERCARD		_T("162")	//CUP	
#define 	TVISACARD		_T("161")	//AMEX	
#define 	TJCBCARD		_T("163")	//JCB	
#define 	TCUPCARD		_T("165")	//CUP	
#define 	TAMEXCARD		_T("160")	//AMEX	
#define 	TDINERSCARD		_T("164")	//DINERS
#define 	TDISCOVERCARD	_T("214")	//DISCOVER


#define 	HYDSBANKID_PUBLIC		_T("218")	//�������� ������
#define 	TYJSBANKID_PUBLIC		_T("379")	//����Ÿ ������ 


#define 	TSAMSUNGCARD	_T("205")	//�Ｚī��

#define 	HYDCAPITALID	_T("261")	//����ĳ��Ż

#define 	TYJSBANKID		_T("209")	//�������ձ�������
#define 	TYJSBANKIDC		_T("902")	//�������ձ�������


#define 	HYDSBANKID		_T("218")	//��������

#define 	MRASBANKID		_T("230")	//�̷���������
#define 	DAWSBANKID		_T("238")	//�������
#define 	SASSBANKID		_T("240")	//�Ｚ����

#define 	MRASBANKIDC		_T("9MR")	//�̷���������
#define 	DAWSBANKIDC		_T("9DW")	//�������
#define 	SASSBANKIDC		_T("9SG")	//�Ｚ����

#define 	WRTSBANKID		_T("247")	//NH��������
#define 	WRTSBANKIDC		_T("9WM")	//NH��������
#define 	HKTSBANKID		_T("243")	//�ѱ���������
#define 	KYBSBANKID		_T("261")	//��������
#define 	HISBANKID		_T("262")	//������������
#define 	HMCSBANKID		_T("263")	//����ġ������������
#define 	KIWSBANKID		_T("264")	//Ű������
#define 	ETRSBANKID		_T("265")	//��Ʈ���̵�����
#define 	SKSBANKID		_T("266")	//������������
#define 	DASSBANKID		_T("267")	//�������
#define 	DASSBANKIDC		_T("9DS")	//�������
#define 	SOLSBANKID		_T("268")	//�ַθ���������
#define 	HAHSBANKID		_T("269")	//��ȭ����
#define 	HNDSBANKID		_T("270")	//�ϳ���������
#define 	GOMSBANKID		_T("278")	//�¸�׽�������
#define 	GOMSBANKIDC		_T("9GM")	//�¸�׽�������
#define 	DOBSBANKID		_T("279")	//��������
#define 	EUGSBANKID		_T("280")	//������������
#define 	MRISBANKID		_T("206")	//�޸�Ʈ�߱�
#define 	MRISBANKIDC		_T("9ME")	//�޸�Ʈ�߱�
#define 	NHTSBANKID		_T("289")	//������ġ��������
#define 	BOKSBANKID		_T("290")	//�α�����
#define 	SIYSBANKID		_T("291")	//�ſ�����
#define 	LIGSBANKID		_T("292")	//����������������

#define 	NHMUINPUTID		_T("100")	//������Ա�  
#define 	DEFAULTID		_T("902")	//                                     	


/*************�귣�屸���ڵ�************************/  
#define NICE					0           // ���ڱ���
#define KB						1           // KB Brand CD
#define KIUP					2			// KIUP Brand CD
#define HOMEPLUS				3			// HOMEPLUS Brand CD
#define JEJU					4           // JEJU Brand CD
#define JEJU2					5			// WOORI Brand CD
#define SHINHAN					6			// SHINHAN Brand CD
#define MYASSET					7			// TONGYANG Brand CD
#define KWANGJU					8			// KWANGJU Brand CD
#define DAEWOO					9			// DEAWOO Brand CD
#define SC  					10			// SC Brand CD
#define MERITZ 					11			// MERITZ Brand CD
#define WOORISTOCK 				12			// WOORI Investments & Securities Brand CD
#define HYDCPTL 				13			// Hyundai Capital Brand CD
#define NONGHYUP 				14			// NongHyup Brand CD (012)
#define HANASTOCK 				15			// Hana Daetoo Securities Brand CD
#define POST					16			// POST Brand CD
#define SAMSSTOCK 				17			// Samsung Stock Securities Brand CD
#define MRASSTOCK 				18			// �̷����� Stock Securities Brand CD
#define HKTSSTOCK 				19			// �ѱ����� Stock Securities Brand CD
#define HAHSSTOCK 				20			// ��ȭ Stock Securities Brand CD
#define SHHSSTOCK 				21			// ���ѱ������� Stock Securities Brand CD
#define KEB						22			// KEB Brand CD
#define SUHYUP					23			// SUHYUP Brand CD
#define CITI					24			// CITI Brand CD
#define HANA					25			// HANA Brand CD
#define DASSSTOCK				26			// ������� Stock Securities Brand CD
#define HYDSSTOCK 				27			// �������� Stock Securities Brand CD
#define KYONGNAM 				28			// KYONGNAM Brand CD  #N0192
#define BUSAN 					29			// BUSAN Brand CD
// 30���� �������� ��ȯ/�ϳ� �귣��� �������
#define DAEGU 					31			// DAEGU Brand ATM
#define JEONBUK 				32			// JEONBUK Brand CD
#define DAEGU_CD 				33			// JEONBUK Brand CD (�ްԼ� ��ġ��� ����, ������ �귣���ڵ� (������ü��)�� �ٸ�)
#define NONGHYUPDAN				34			// ��������(012)

#define KB_Brand				"01"			// KB Brand CD
#define KIUP_Brand				"02"			// KIUP Brand CD
#define HOMEPLUS_Brand			"03"			// HOMEPLUS Brand CD
#define JEJU_Brand				"04"			// JEJU Brand CD
#define JEJU2_Brand				"05"			// WOORI Brand CD
#define SHINHAN_Brand			"06"			// SHINHAN Brand CD
#define MYASSET_Brand			"07"			// TONGYANG Brand CD
#define KWANGJU_Brand			"08"			// KWANGJU Brand CD
#define DAEWOO_Brand			"09"			// DEAWOO Brand CD
#define SC_Brand  				"10"			// SC Brand CD
#define MERITZ_Brand 			"11"			// MERITZ Brand CD
#define WOORISTOCK_Brand 		"12"			// WOORI Investments & Securities Brand CD
#define HYDCPTL_Brand 			"13"			// Hyundai Capital Brand CD
#define NONGHYUP_Brand 			"14"			// NongHyup Brand CD
#define HANASTOCK_Brand 		"15"			// Hana Daetoo Securities Brand CD
#define POST_Brand				"16"			// POST Brand CD
#define SAMSSTOCK_Brand 		"17"			// Samsung Stock Securities Brand CD
#define MRASSTOCK_Brand 		"18"			// �̷����� Stock Securities Brand CD
#define HKTSSTOCK_Brand 		"19"			// �ѱ����� Stock Securities Brand CD
#define HAHSSTOCK_Brand 		"20"			// ��ȭ Stock Securities Brand CD
#define SHHSSTOCK_Brand 		"21"			// ���ѱ������� Stock Securities Brand CD
#define KEB_Brand				"22"			// KEB Brand CD
#define SUHYUP_Brand			"23"			// SUHYUP Brand CD
#define CITI_Brand				"24"			// CITI Brand CD
#define HANA_Brand				"25"			// HANA Brand CD
#define DASSSTOCK_Brand 		"26"			// ������� Stock Securities Brand CD
#define KYONGNAM_Brand			"28"			// KYONGNAM Brand CD #N0192
#define BUSAN_Brand				"29"			// BUSAN Brand CD
#define DAEGU_Brand				"31"			// DAEGU Brand ATM
#define JEONBUK_Brand			"32"			// JEONBUK Brand CD
#define DAEGU_Brand_CD			"33"			// DAEGU Brand CD  (�ްԼ� ��ġ��� ����, ������ �귣���ڵ� (������ü��)�� �ٸ�)
#define NONGHYUPDAN_Brand		"34"			// ��������

/*************�귣�屸���ڵ�************************/  

/*************���� �ڵ�************************/  
#define		OTHBANK		100 //��Ÿ����
#define     KDBBANK		2	//�������	
#define 	KIUBANK		3	//�������	
#define 	KUMBANK		4	//��������	(30,29,04,06,09)
#define 	KOEBANK		5	//��ȯ����	
#define 	SUHBANK		7	//��������	
#define 	NONBANK		11	//����      (17,16,12,11)
#define 	WORBANK		20	//�츮����  (20,22,24,83)  
#define 	CHOBANK		21	//��������     
#define 	KOFBANK		23	//��������     
#define 	HANBANK		25	//�ϳ�����  (25,81,82,33)   
#define 	SHIBANK		26	//��������     
#define 	GOOBANK		27	//�ѹ�����  (27,36)   
#define 	DEABANK		31	//�뱸����     
#define 	PUSBANK		32	//�λ�����     
#define 	KAWBANK		34	//��������     
#define 	JEJBANK		35	//��������          
#define 	JEOBANK		37	//��������     
#define 	KYOBANK		39	//�泲����     
#define 	KFCBANK		45	//��������     
#define 	SINBANK		48	//����	     
#define 	SANBANK		50	//��ȣ����     
#define 	CITBANK		53	//��Ƽ����     
#define 	HSBABNK		54	//H.S.B.C	     
#define 	POSBANK		71	//��ü��  (71,72)	     
   
#define		ETCCARD		400	//�ش���׹�
#define 	KUMINCARD	401	//����ī��     
#define 	HYUNDAICARD	402	//����ī��     
#define 	MASTERCARD	403	//MASTER	     
#define 	BC_CARD		404	//��ī��     
#define 	SAMSUNGCARD	405	//�Ｚī��     
#define 	LOTTECARD	406	//�Ե�ī��     
#define 	LG_CARD		407	//����ī��     
#define 	YESCARD		408	//��ȯī��     
#define 	VISA_CARD	409	//VISA	
#define 	JCBCARD		410	//JCB	
#define 	CUPCARD		411	//CUP	
#define 	AMEXCARD	412	//AMEX	
#define 	DINERS		413	//DINERS	
#define 	DISCOVER	414	//DISCOVER	     
#define 	KWANGSUCARD 421	//����ī��     
#define 	SHINHANCARD 428	//����ī��     
#define 	CHBCARD		430	//����ī��     
#define 	GOODCARD	433	//�ѹ�ī��     
#define 	WOORICARD	437	//�츮ī��
#define 	NHCARD	    411	//����ī��
#define 	ISUHCARD    407	//����ī��
#define 	JEJCARD		435	//����ī��   

#define 	HYDCAPITAL	261	//����ĳ��Ż

#define 	TYJSBANK	209 //�������ձ�������
#define 	HYDSBANK	218	//��������
#define 	MRASBANK	230	//�̷���������
#define 	DAWSBANK	238	//�������
#define 	SASSBANK	240	//�Ｚ����
#define 	WRTSBANK	247	//NH��������
#define 	HKTSBANK	243	//�ѱ���������
#define 	KYBSBANK	261	//��������
#define 	HISBANK		262	//������������
#define 	HMCSBANK	263	//����ġ������������
#define 	KIWSBANK	264	//Ű������
#define 	ETRSBANK	265	//��Ʈ���̵�����
#define 	SKSBANK		266	//������������
#define 	DASSBANK	267	//�������
#define 	SOLSBANK	268	//�ַθ���������
#define 	HAHSBANK	269	//��ȭ����
#define 	HNDSBANK	270	//�ϳ���������
#define 	GOMSBANK	278	//�¸�׽�������
#define 	DOBSBANK	279	//��������
#define 	EUGSBANK	280	//������������
/*
#define 	MRISBANK	206	//�޸�Ʈ�߱�
#define 	NHTSBANK	225	//������ġ��������
#define 	BOKSBANK	226	//�α�����
#define 	SIYSBANK	227	//�ſ�����
#define 	LIGSBANK	228	//����������������
*/
//#N0131
#define 	MRISBANK	287	//�޸�Ʈ�߱�
#define 	NHTSBANK	289	//������ġ��������
#define 	BOKSBANK	290	//�α�����
#define 	SIYSBANK	291	//�ſ�����
#define 	LIGSBANK	292	//����������������




#define 	UTICKET		804	//����Ƽ��
#define 	INTERAD		806	//���ͷ�Ƽ�걤��
#define 	MOBILEGAME	804	//����ϰ��Ӵٿ�ε�


// ATM EP_VERSION
#define REG_GETEP_VERSION		_T("SOFTWARE\\ATM\\EP_Version")
// ATM SP_VERSION
#define REG_GETSP_VERSION		_T("SOFTWARE\\ATM\\SP_Version")
				

// ATM SEND FLAG
#define		ATM_SENDFLAG				_T("CQ")
#define		ATM_RECVFLAG				_T("SR")
#define		ATM_RETURN_OK				_T("0000")

// ATM SEND FORMAT REG_
#define		REG_TXATM0300				_T("SOFTWARE\\ATM\\TXRXData\\TXATM0300")
#define		REG_TXATM0330				_T("SOFTWARE\\ATM\\TXRXData\\TXATM0330")

// ATM DEVICE UNIT STATUS_
#define		REG_STATUSCOM				_T("SOFTWARE\\ATM\\TXRSData\\STATUSCOM")


#define	    TXDATAFIELD		1									//�۽�
#define	    RXDATAFIELD		2									//����

#define     TRANID_0300		300									//0300:��������
#define     TRANID_0310		310									//0310:��ǥ���� ���� �뺸 ����
#define     TRANID_0311		311									//0311:��ǥ ���� ��û ����
#define     TRANID_0330		330									//0330:���������뺸
#define     TRANID_0410		410									//0410:��������û�뺸
#define     TRANID_0415		415									//0415:����Ű��û�뺸
#define     TRANID_0501		501									//0501:�����Ϸ��뺸

#define		SUBINDEX00		0
#define		SUBINDEX01		1
#define		SUBINDEX02		2
#define		SUBINDEX03		3
#define		SUBINDEX04		4
#define		SUBINDEX05		5
#define		SUBINDEX06		6

#define		ENCRYPTMETHOD0  0
#define		ENCRYPTMETHOD1  1
#define		ENCRYPTMETHOD2  2
#define		ENCRYPTMETHOD3  3
#define		ENCRYPTMETHOD4  4
#define		ENCRYPTMETHOD5  5
#define		ENCRYPTMETHOD6  6
#define		ENCRYPTMETHOD7  7
#define		ENCRYPTMETHOD8  8
#define		ENCRYPTMETHOD9  9

#define	    TRANID_0006			6									//0006:��������
#define		TRANID_0006_S		"0006"

typedef struct tag031XFormat{		
	char	OrgNo[3];				// OrgCode
	char	Dummy;					// NULL
} F031X;

typedef struct tag031XINFO{	
	F031X    F031XLIST[100];				// �ŷ����ɱ������																	
} F031XINFO;

/*********************************************************************************************************************************/  
/* �������� - Tran-I */
#define	    TRANID_A010         10      //0800	0810	�ܸ��� ��������                               
#define	    TRANID_A020         20      //0800	0810	�ܸ��� ��������      
#define	    TRANID_A040         40      //0800	0810	�ܸ��� ��������                                                           
#define	    TRANID_A060         60      //0800	0810	�ܸ��� ��������    
#define	    TRANID_2190         2190    //0800	0810	�ܸ��� ���ΰŷ�   

#define	    TRANID_7230         7230    //0200	0210	������ȸ 
#define	    TRANID_7250         7250    //0200	0210	������ü��ȸ 
#define	    TRANID_7380         7380    //0200	0210	��������ȸ              //define	    TRANID_7110         7110    //0200	0210	��������ȸ ���ݼ��� ����	
#define	    TRANID_4330         4330    //0200	0210	�������  
#define	    TRANID_4350         4350    //0200	0210	������ü    

#define	    TRANID_7210         7210    //0200	0210	��������ȸ              //#define	    TRANID_7100         7100    //0200	0210	��������ȸ   
#define	    TRANID_4310         4310    //0200	0210	�������  
#define	    TRANID_7260         7260    //0200	0210	���������ȸ     
#define	    TRANID_4370         4370    //0200	0210	������ü                //���� ���� 
#define	    TRANID_4390         4390    //0200	0210	Ÿ�簣��ü  
#define	    TRANID_7220         7220    //0200	0210	������ȸ

#define	    TRANID_7530         7530    //0200	0210	��������ȸ(�ؿ�)  

#define	    TRANID_4320         4320    //5400	5410	����������   
#define	    TRANID_4340         4340    //5400	5410	���������� 

#define	    TRANID_7200         7200    //0200	0210	ī���Ա�                 //#define	    TRANID_7226         7226    //0200	0210	�Աݰ��ɿ�����ȸ 
#define	    TRANID_4410         4410    //0200	0210	ī���Ա�                 //#define	    TRANID_4710         4710    //0200	0210	ī���Ա� 
#define	    TRANID_4420         4420    //0200	0210	ī���Ա�                 //���κ귣��


/*********************************************************************************************************************************/                   
#define	    TRANID_4311         4311    //0200	0210	�ؿܿ�������                          
#define	    TRANID_4312         4312    //0200	0210	�ؿܴ��°ŷ�����                                      
#define	    TRANID_4314         4314    //0200	0210	��ü���                              
#define	    TRANID_4315         4315    //0200	0210	������ü��� 
#define	    TRANID_4316         4316    //0200	0210	�ؿ� BC���   #N0171                        
                       
#define	    TRANID_4321         4321    //5400	5410	�ؿܿ����������                      
#define	    TRANID_4322         4322    //5400	5410	�ؿܴ��°ŷ��������                               
#define	    TRANID_4324         4324    //5400	5410	��ü��� ��� 
#define	    TRANID_4325         4325    //0200	0210	�ؿ� BC��� ���   #N0171       
#define	    TRANID_4326         4326    //5400	4326	����ü��� ���                    
                          
#define	    TRANID_4331         4331    //0200	0210	�ؿ����ݼ�������                    
#define	    TRANID_4332         4332    //0200	0210	�߱����ü��� ����                   
#define	    TRANID_4336         4336    //0200	0210	���Ÿŵ� ó��                         
#define	    TRANID_4339         4339    //0200	0210	����ȣ��ü �ޱ�                       
                   
#define	    TRANID_4341         4341    //5400	5410	�ؿܼ����������                    
#define	    TRANID_4342         4342    //5400	5410	�߱����ü��� �������               
#define	    TRANID_4349         4349    //5400	5410	����ȣ��ü �ޱ����                   
                          
#define	    TRANID_4360         4360    //0200	0210	�������������                      
#define	    TRANID_4361         4361    //0200	0210	ĳ��Ż������                                                    
#define	    TRANID_4391         4391    //0200	0210	������ü(�´�����ǰ�� ��ü�Ǹ�)       
#define	    TRANID_4392         4392    //0200	0210	e-���� ������ü                                
#define	    TRANID_4394         4394    //0200	0210	ICī�� ������ü(�´�����ǰ�� ��ü�Ǹ�)
#define	    TRANID_4395         4395    //0200	0210	e-���� ������ü                       
#define	    TRANID_4399         4399    //0200	0210	������ü(����ȣ/�ޱ�)

#define	    TRANID_4300         4300    //5400	5410	����������������                  
#define	    TRANID_4301         4301    //5400	5410	ĳ��Ż�� �������                     
#define	    TRANID_4210         4210    //0200	0210	�������(����)                        
#define	    TRANID_4116         4116    //0200	0210	����ü���                       
#define	    TRANID_4214         4214    //0200	0210	��ü���(����)                        
#define	    TRANID_4215         4215    //0200	0210	������ü���(����)                    
#define	    TRANID_4396         4396    //0200	0210	������ü���(����ü)                    
#define	    TRANID_4216         4216    //0200	0210	e-������ü                            
#define	    TRANID_4220         4220    //5400	5410	������(����)    
                 
#define	    TRANID_4520         4520    //0200	0210	���� �м��� ����                    
#define	    TRANID_4530         4530    //5400	5410	���� �м��� �������                
#define	    TRANID_4540         4540    //0200	0210	�Ե� �м��� ����                    
#define	    TRANID_4550         4550    //5400	5410	�Ե� �м��� �������                
#define	    TRANID_4560         4560    //0200	0210	��ȯ �ؿܼ۱�                         
#define	    TRANID_4570         4570    //0200	0210	����ī��� ����                       
#define	    TRANID_4571         4571    //0200	0210	����ī��� ��ü                       
#define	    TRANID_4572         4572    //0200	0210	����ī��� ����                       
#define	    TRANID_4580         4580    //0200	0210	����ī��� �������                   
#define	    TRANID_4610         4610    //0200	0210	T-Money ��������                      
#define	    TRANID_4620         4620    //5400	5410	T-Money �����������                  
#define	    TRANID_4611         4611    //0200	0210	T-Money ����������                    
#define	    TRANID_4621         4621    //5400	5410	T-Money �������������                
#define	    TRANID_4631         4631    //0200	0210	T-Money ���ϸ�����ȸ                  
#define	    TRANID_4630         4630    //0200	0210	T-Money ���ϸ�������                  
#define	    TRANID_4640         4640    //5400	5410	T-Money ���ϸ������                  
#define	    TRANID_4650         4650    //0200	0210	T-Money������ �������                
#define	    TRANID_4660         4660    //5400	5410	T-Money������ ����������            
#define	    TRANID_4670         4670    //0200	0210	�����н� ������û                     
#define	    TRANID_4671         4671    //0200	0210	�����н� ����Ȯ��                     
#define	    TRANID_4680         4680    //0200	0210	�����н� ��������                     
#define	    TRANID_4681         4681    //0200	0210	�����н� ����������ȸ                 
#define	    TRANID_4682         4682    //0200	0210	�����н� ��������Ȯ��                 
                            
#define	    TRANID_4711         4711    //0200	0210	����ü�Ա�                            
#define	    TRANID_4712         4712    //0200	0210	�̸�Ʈ���� ������Ա�                 
#define	    TRANID_4713         4713    //0200	0210	ICī���Ա�                            
#define	    TRANID_4719         4719    //5400	5410	�Ա����                              
#define	    TRANID_4720         4720    //0200	0210	�����Ա�                              
#define	    TRANID_4716         4716    //0200	0210	�Ա�(����ü)                              
#define	    TRANID_4730         4730    //0200	0210	��������                              
#define	    TRANID_4810         4810    //0200	0210	�����ݳ���                            
#define	    TRANID_4811         4811    //0200	0210	���漼 ������(MS)                   
#define	    TRANID_4812         4812    //0200	0210	���漼 ������(�ſ�ī��)             
#define	    TRANID_4813         4813    //0200	0210	ICī�� �����ݳ���                     
#define	    TRANID_4814         4814    //0200	0210	���漼 ������(IC)                   
#define	    TRANID_4820         4820    //0200	0210	�ڵ��¼�/�ֹ�����                     
#define	    TRANID_4821         4821    //0200	-	��º���                              
#define	    TRANID_4822         4822    //0200	0210	�߱����� ���                         
#define	    TRANID_4830         4830    //0200	0210	TC ����                               
#define	    TRANID_4840         4840    //0200	0210	�ſ�ī�� ����(���ΰ���)               
#define	    TRANID_4850         4850    //5400	5410	�ſ�ī�� ���(���ΰ���)               
#define	    TRANID_4841         4841    //0200	0210	�ſ�ī�� ����(��������)               
#define	    TRANID_4851         4851    //5400	5410	�ſ�ī�� ���(��������)               
#define	    TRANID_4842         4842    //0200	0210	�ſ�ī�� ����(ȿ��Ƽ��)               
#define	    TRANID_4852         4852    //5400	5410	�ſ�ī�� ���(ȿ��Ƽ��)               
#define	    TRANID_4860         4860    //0200	0210	��ǰ���Ǹ������뺸                    
#define	    TRANID_7000         7000    //0200	0210	�ٰ�����ȸ                                    
#define	    TRANID_7004         7004    //0200	0210	�ٰ�����ȸ(�Ա�)                              
#define	    TRANID_7005         7005    //0200	0210	�ٰ�����ȸ(����)                              
#define	    TRANID_7006         7006    //0200	0210	�ٰ�����ȸ(��ü)                              
                                    
#define	    TRANID_7101         7101    //0200	0210	��������ȸ(����)  
#define	    TRANID_7106         7106    //0200	0210	��������ȸ(����ü)                              
#define	    TRANID_7103         7103    //0200	0210	ICī�� ��������ȸ
#define	    TRANID_7104         7104    //0200	0210	�ؿ���޼�������ȸ                              
#define	    TRANID_7105         7105    //0200	0210	�ؿ�BC��޼�������ȸ  
#define	    TRANID_7110         7110    //0200	0210	��������ȸ ���ݼ��� ����	#N0161
#define	    TRANID_7111         7111    //0200	0210	��������ȸ ���ݼ��� ��ü	#N0161                           
#define	    TRANID_7120         7120    //0200	0210	������ȸ(����)                                
#define	    TRANID_7124         7124    //0200	0210	��ü��ݰ�����ȸ(����)                        
#define	    TRANID_7125         7125    //0200	0210	������ü��ݰ�����ȸ(����)                    
#define	    TRANID_7126         7126    //0200	0210	�Աݰ��ɿ�����ȸ(����)                        
                                      
#define	    TRANID_7221         7221    //0200	0210	�ؿܿ����ܾ���ȸ                              
#define	    TRANID_7222         7222    //0200	0210	�ؿܴ��°ŷ��ܾ���ȸ                          
#define	    TRANID_7224         7224    //0200	0210	��ü��ݰ�����ȸ                              
#define	    TRANID_7225         7225    //0200	0210	������ü��ݰ�����ȸ                                                     
#define	    TRANID_7236         7236    //0200	0210	�Աݰ��ɿ�����ȸ(����ü)                                                       
#define	    TRANID_7228         7228    //0200	0210	����������ȸ 
#define	    TRANID_7229         7229    //0200	0210	���� �ܾ� ��ȸ   #N0239                               
                                   
#define	    TRANID_7231         7231    //0200	0210	�ؿ����ݼ�����ȸ                            
#define	    TRANID_7232         7232    //0200	0210	�߱����ü��� �ܾ���ȸ  
#define	    TRANID_7233         7233    //5400	0210	�ؿܿ����ܾ���ȸ ���                              
#define	    TRANID_7235         7235    //5400	0210	�ؿܴ��°ŷ��ܾ���ȸ ���
#define	    TRANID_7237         7237    //5400	0210	�ؿܽſ���ȸ ���                      
#define	    TRANID_7240         7240    //0200	0210	�����������ȸ                              
#define	    TRANID_7241         7241    //0200	0210	ĳ��Ż���ܾ���ȸ                              
                               
                                
#define	    TRANID_7261         7261    //0200	0210	���������ȸ(����)                            
#define	    TRANID_7276         7276    //0200	0210	���������ȸ(����ü)                            
#define	    TRANID_7262         7262    //0200	0210	���¼�����ȸ(e-���� ������ȸ)                                     
#define	    TRANID_7265         7265    //0200	0210	e-���� ������ü ���µ��                      
#define	    TRANID_7266         7266    //0200	0210	�Ǹ�Ȯ��                                      
#define	    TRANID_7267         7267    //0200	0210	�����ݾ���ȸ                                  
#define	    TRANID_7268         7268    //0200	0210	������ȣ�ޱ�                                  
#define	    TRANID_7269         7269    //0200	0210	���������ȸ(����ȣ/�ޱ�)                     
#define	    TRANID_7280         7280    //0200	0210	�Ե� �м��� ��ȸ                            
#define	    TRANID_7281         7281    //0200	0210	�Ե� �м��� ������ȸ                        
#define	    TRANID_7290         7290    //0200	0210	��ȯ �ؿܼ۱� ���� ��ȸ                       
#define	    TRANID_7300         7300    //0200	0210	��ȯ �ؿܼ۱� ��� ��ȸ                       
#define	    TRANID_7310         7310    //0200	0210	�ſ�������ȸ                                  
#define	    TRANID_7311         7311    //0200	0210	����������ȸ                                  
#define	    TRANID_7312         7312    //0200	0210	CB��ü������ȸ                                
#define	    TRANID_7313         7313    //0200	0210	�ſ�ī�� ����(�ѽ���)                         
#define	    TRANID_7314         7314    //0200	0210	�ſ�ī�� ����                                 
#define	    TRANID_7315         7315    //0200	0210	�ſ����� ���ھ� ��ȸ                          
#define	    TRANID_7316         7316    //0200	0210	�ſ�ī�� ����(����ĳ��Ż)                     
#define	    TRANID_7317         7317    //0200	0210	���Ⱑ�ɱݾ���ȸ - ����                       
#define	    TRANID_7318         7318    //0200	0210	���Ⱑ�ɱݾ���ȸ - ����                       
#define	    TRANID_7319         7319    //0200	0210	������ũ��                                    
#define	    TRANID_7320         7320    //0200	0210	�����ݳ�����ȸ                                
#define	    TRANID_7321         7321    //0200	0210	�����ݳ���ó�������ȸ                        
#define	    TRANID_7322         7322    //0200	0210	���漼��������ȸ(MS)                        
#define	    TRANID_7323         7323    //0200	0210	ICī�� �����ݳ�����ȸ                         
#define	    TRANID_7324         7324    //0200	0210	ICī�� �����ݳ���ó�������ȸ                 
#define	    TRANID_7325         7325    //0200	0210	���漼��������ȸ(IC)                        
#define	    TRANID_7326         7326    //0200	0210	���漼�����ΰ����ȸ(MS)                    
#define	    TRANID_7327         7327    //0200	0210	���漼�����ΰ����ȸ(IC)                    
#define	    TRANID_7328         7328    //0200	0210	���漼��������ȸ(�ſ�ī��)                  
#define	    TRANID_7329         7329    //0200	0210	���漼�����ΰ����ȸ(�ſ�ī��)              
#define	    TRANID_7330         7330    //0200	0210	����������ȸ                                  
#define	    TRANID_7336         7336    //0200	0210	���Ÿŵ��ܾ���ȸ                              
#define	    TRANID_7337         7337    //0200	0210	�ֹ�ü�᳻�����                              
#define	    TRANID_7338         7338    //0200	0210	�ܰ���ȸ���                                  
#define	    TRANID_7339         7339    //0200	0210	�ŷ��������                                  
#define	    TRANID_7340         7340    //0200	0210	TC ���� ���� ���� ��ȸ                        
#define	    TRANID_7341         7341    //0200	0210	Ƽ������ ��ȸ                                 
#define	    TRANID_7342         7342    //0200	0210	�Ǹ�ó���� ��ȸ                               
#define	    TRANID_7343         7343    //0200	0210	����/�������� ��ȸ                            
#define	    TRANID_7344         7344    //0200	0210	�������� ��ȸ                                 
#define	    TRANID_7345         7345    //0200	0210	��߽ð� ��ȸ                                 
                      
#define	    TRANID_7351         7351    //0200	0210	����Ʈ�㺸���� - ����                         
#define	    TRANID_7352         7352    //0200	0210	�����ڱݴ��� - ����                           
#define	    TRANID_7353         7353    //0200	0210	�����ڱݴ��� - ����                           
#define	    TRANID_7354         7354    //0200	0210	�޴���ȭ ����(����ĳ��Ż)                     
#define	    TRANID_7357         7357    //0200	0210	������ó ���(���ν�����)                   
#define	    TRANID_7358         7358    //0200	0210	�ֹι�ȣ,��Ż籸��,�ڵ�����ȣ�Է�(�����)
#define	    TRANID_7359         7359    //0200	0210	������ȣ�Է�(�����)                      
                                 
#define	    TRANID_7361         7361    //0200	0210	�������ȸ                                  
#define	    TRANID_7362         7362    //0200	0210	�������ε��                                  
#define	    TRANID_7363         7363    //0200	0210	�����ѵ����׵��                              
#define	    TRANID_7364         7364    //0200	0210	�Ż�����ȸ                                  
#define	    TRANID_7365         7365    //0200	0210	�Ż������                                  
#define	    TRANID_7366         7366    //0200	0210	�ѵ�������                                  
#define	    TRANID_7367         7367    //0200	0210	�����                                      
#define	    TRANID_7370         7370    //0200	0210	���Ⱑ�ɻ�ǰ��ȸ                              
#define	    TRANID_7371         7371    //0200	0210	���޻���ȸ                                  
#define	    TRANID_7372         7372    //0200	0210	��ü����ȸ                                  
#define	    TRANID_7373         7373    //0200	0210	��������ȸ                                  
                                   
#define	    TRANID_7390         7390    //0200	0210	�캰�                                      
#define	    TRANID_7400         7400    //0200	0210	������Ű                                      
#define	    TRANID_7410         7410    //0200	0210	�������                                    
#define	    TRANID_7411         7411    //0200	0210	������-�米                                   
#define	    TRANID_7412         7412    //0200	0210	������-����                                   
#define	    TRANID_7413         7413    //0200	0210	������-��ũ���̼�                             
#define	    TRANID_7420         7420    //0200	0210	����Ʈ��������Ʈ��ȸ                          
#define	    TRANID_7421         7421    //0200	0210	�ü���ȸ                                      
#define	    TRANID_7425         7425    //0200	0210	������ȸ(����)                                
#define	    TRANID_7426         7426    //0200	0210	�ѵ����Ⱑ�ɱݾ���ȸ(����)                    
#define	    TRANID_7427         7427    //0200	0210	�ѵ������û(����)                            
#define	    TRANID_7428         7428    //0200	0210	ķ������ȸ(����)                              
#define	    TRANID_7430         7430    //0200	0210	���� �м��� ��ȸ                            
#define	    TRANID_7431         7431    //0200	0210	���� �м��� ������ȸ                        
#define	    TRANID_7440         7440    //0200	0210	������ĳ�� �ʰ��� ��û                        
#define	    TRANID_7441         7441    //0200	0210	������ĳ�� ��������                           
#define	    TRANID_7442         7442    //0200	0210	������ĳ�� �������                         
#define	    TRANID_7443         7443    //0200	0210	������ĳ�� �ſ�ī������                       
#define	    TRANID_7444         7444    //0200	0210	������ĳ�� ������ȣ �۽�                      
#define	    TRANID_7445         7445    //0200	0210	������ĳ�� �ڵ��� ���� �Է�                   
#define	    TRANID_7446         7446    //0200	0210	������ĳ�� �����Ǽ���ȸ                       
#define	    TRANID_7447         7447    //0200	0210	������ĳ�� ������������Է�                   
#define	    TRANID_7448         7448    //0200	0210	������ĳ�� �����ѵ� ��ȸ                      
#define	    TRANID_7449         7449    //0200	0210	������ĳ�� ��������                           
#define	    TRANID_7450         7450    //0200	0210	������ĳ�� �۱ݿ�û                           
#define	    TRANID_7451         7451    //0200	0210	���α����ѵ���ȸ                              
                   


/////////////////////////////////////////////////////////////////////////////
//#N0174  ��ȯ ������ TRANID
#define	    TRANID_7500		    7500    //0200	0210	��ȯ������ ��ȸ MS
#define	    TRANID_7503		    7503    //0200	0210	��ȯ������ ��ȸ IC
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//#N0182  ���Ӵ� TRANID
#define	    TRANID_7107		    7107    //0200	0210	���Ӵ� ��� ��ȸ
#define	    TRANID_4117		    4117    //0200	0210	���Ӵ� ���
#define	    TRANID_4327		    4327    //0200	0210	���Ӵ� ��� ���

/////////////////////////////////////////////////////////////////////////////
//#N0184  ����ī�� �ѵ� ����
#define	    TRANID_7374		    7374    //0200	0210	����ī�� �ѵ����� ��ȸ
#define	    TRANID_7375		    7375    //0200	0210	����ī�� �ѵ����� ���
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//#N0266  �����н� ����
#define	    TRANID_4670		    4670    //0200	0210	�����н� ������û
#define	    TRANID_4671		    4671    //0200	0210	�����н� ����Ȯ��
#define	    TRANID_4680		    4680    //0200	0210	�����н� ��������
#define	    TRANID_4681		    4681    //0200	0210	�����н� ��������Ȯ��

#define	    TRANID_4717		    4717    //0200	0210	�����н� �����Ա�

#define	    TRANID_4840		    4840    //0200	0210	�����н� �ſ�ī�� ����
#define	    TRANID_4850		    4850    //0200	0210	�����н� �ſ�ī�� ���� ���

/////////////////////////////////////////////////////////////////////////////
 
/* DataField */
#define	   x081X                "0810"//����Format����
#define	   x001X                "0010"//�ŷ���¥                                          
#define	   x002X                "0020"//�ŷ��ð�                            
#define	   x003X                "0030"//SST �Ϸù�ȣ                        
#define	   x004X                "0040"//���޻� �Ϸù�ȣ 
#define	   x005X                "0050"//��������(Msg-Type)                  
#define	   x006X                "0060"//���������ڵ�(Tran-ID)               
#define	   x007X                "0070"//�ŷ���ü����                        
#define	   x008X                "0080"//�����ڵ�(RspCode)   
#define	   x009X                "0090"//��������              
#define	   x011X                "0110"//SST����ȣ(MecaNum)                
#define	   x048X                "0480"//�ſ�ī����������                    
#define	   x049X                "0490"//SST��������  (14�ڸ�)                        
#define	   x010X                "0100"//��ȣȭ����                          
#define	   x0120                "0120"//��ޱ���ڵ�1                       
#define	   x0121                "0120"//��ޱ���ڵ�2                       
#define	   x013X                "0130"//ISO3 Data                           
#define	   x0131                "0131"//ISO3 Data                           
#define	   x014X                "0141"//ISO2 Data                           
#define	   x015X                "0150"//��й�ȣ                            
#define	   x0151                "0151"//��й�ȣ - ��ȣȭ  
#define	   x0152                "0152"//��й�ȣ - �ؿ�ī�� DES      //#N0172                       
#define	   x016X                "0160"//���� M/S Data
#define	   x0161                "0161"//���� M/S Data ���� ��� ��ü ��ȸ #N0239                          
#define	   x017X                "0170"//�ŷ�����(ī��)��ȣ1                 
#define	   x018X                "0180"//�ŷ�����(ī��)����                  
#define	   x019X                "0190"//Filler  
#define	   x0191                "0191"//���� �ſ�ī�� ��ȣȭ DATA 104����Ʈ #N0181                   
#define	   x020X                "0200"//������¹�ȣ                        
#define	   x021X                "0210"//������¼���                        
#define	   x022X                "0220"//�ŷ�����(ī��)�ܾ�                  
#define	   x0221                "0221"//�ŷ�����(ī��)�ܾ� - ��ȣȭ                  
#define	   x023X                "0230"//�ŷ��ݾ�                            
#define	   x024X                "0240"//�ŷ�������                          
#define	   x025X                "0250"//�̰���Ÿ���� �ݾ�                   
#define	   x026X                "0260"//HOST�Ϸù�ȣ     
#define	   x0262                "0262"//�ؿܼ۱� �������� ��ȯ ������ #N0174                        
#define	   x027X                "0270"//��ǥ�ŷ��ݾ�                        
#define	   x028X                "0280"//��ǥ�ż�                            
#define	   x029X                "0290"//��ǥ���� ����  
#define	   x031X                "0310"//�ŷ����ɱ������ 
#define	   x038X                "0380"                    
#define	   x041X                "0410"//��ȣȭKey���� (�Ｚ���Ǿ�ȣȭ)                     
#define	   x042X                "0420"                      
#define	   x043X                "0430"                     
#define	   x044X                "0440"//������ȣȭKey 
#define	   x0441                "0441"//������ȣȭKey 
                      
#define	   x045X                "0450"//���������㰡���� 
#define	   x0451                "0451"//���������㰡����
#define	   x050X                "0500"//�Ϸù�ȣ  
#define	   x0520                "0520"//�ŷ����¹�ȣ
#define	   x0521                "0521"
#define	   x0522                "0522"
#define	   x054X                "0540"
#define	   x055X                "0550"
#define	   x056X                "0560"//���������հ����� 
#define	   x057X                "0570"
#define	   x058X                "0580"
 
#define	   x0571                "0571"
#define	   x0581                "0581"
        
#define	   x059X                "0590"//��ݰ��� �ݾ�(��ǥ)       
#define	   x061X                "0610"//������ �ż�����
#define	   x062X                "0620"//������ �ż�����          
#define	   x063X                "0630"//�ŷ������Ͻ�                        
#define	   x064X                "0640"//HOST�Ϸù�ȣ                        
#define	   x065X                "0650"//����ȭ�� ���� �����ݾ�              
#define	   x066X                "0660"//ȭ������                            
#define	   x067X                "0670"//����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)   
#define	   x068X                "0680"//�ŷ�ī����                          
#define	   x0691                "0691"//������ī���ܾ�                      
#define	   x070X                "0700"//����                                
#define	   x071X                "0710"//������                              
#define	   x072X                "0720"//ī������                            
#define	   x073X                "0730"//�˰��� ID                         
#define	   x074X                "0740"//VK                                  
#define	   x075X                "0750"//����ID                              
#define	   x076X                "0760"//������ID                          
#define	   x077X                "0770"//�̿���ī��ID                        
#define	   x078X                "0780"//PAN                                 
#define	   x079X                "0790"//������ID                            
#define	   x080X                "0800"//���� �ſ�ī��(����)��ȣ             
#define	   x085X                "0850"//��Ż� ����                         
#define	   x086X                "0860"//������ ��û ������ (086X)                           
#define	   x087X                "0870"//���ʰŷ�����                        
#define	   x088X                "0880"//�������� ��� ����                  
#define	   x089X                "0890"//����ŷ����� ȭ��ȳ� �޽���        
#define	   x090X                "0900"//�����ܾ� ���ڿ���                   
#define	   x091X                "0910"//�������� ������                     
#define	   x092X                "0920"//�������� ������                     
#define	   x093X                "0930"//�������� ���Ǽ�                   
#define	   x094X                "0940"//������������                        
#define	   x0941                "0941"//������������  
#define	   x0942                "0942"//������������  
#define	   x0943                "0943"//������������                          
#define	   x095X                "0950"//���������ڷ� �߰� ���� ����         
#define	   x0951                "0951"//���������ڷ� �߰� ���� ����  (�泲) #N0277
#define	   x096X                "0960"//�������Ϲ�ȣ                        
#define	   x097X                "0970"//���弭������                        
#define	   x098X                "0980"//�߰� ����� KEY                     
#define	   x099X                "0990"//ȭ��ȳ�����                         
#define	   x100X                "1000"//���Ⱓ                            
#define	   x101X                "1010"//��ȸ����                            
#define	   x104X                "1040"//��ǥ ����Data                     
#define	   x127X                "1270"//������ ��ݸż�                     
#define	   x1271                "1271"//������ �Աݸż�                     
#define	   x128X                "1280"//5������ ��ݸż�                    
#define	   x1281                "1281"//5������ ��ݸż�                    
#define	   x129X                "1290"//10������ ��ݸż�                   
#define	   x130X                "1300"//�����������                        
#define	   x131X                "1310"//����ݾ�                          
#define	   x166X                "1660"//�����ǰ�ڵ�                        
#define	   x167X                "1670"//�����û������                      
#define	   x1680                "1680"//��������                            
#define	   x1810                "1810"//��ü����   
#define	   x458X                "4580"//��ü����   
                         
#define	   x169X                "1690"//�����ݳ�����                        
#define	   x170X                "1700"//������                              
#define	   x171X                "1710"//���Ⱑ�ɱݾ�1                       
#define	   x172X                "1720"//��޼�����
#define	   x1721                "1721"//�ϳ����� ��޼�����  #N0214                         
#define	   x189X                "1890"//��õ���� �ż�                       
#define	   x1891                "1891"//��õ���� �ż�                       
#define	   x190X                "1900"//õ���� �ż�                         
#define	   x1901                "1901"//õ���� �ż�                         
#define	   x191X                "1910"//���ι�ȣ                            
#define	   x192X                "1920"//���ڳ��ι�ȣ(�Ϸù�ȣ)              
#define	   x193X                "1930"//���γ��γ���
#define	   x1931                "1931"//���漼 ���γ���       
#define	   x194X                "1940"//�̿�����(�ŷ��� ��ȣ)             
#define	   x195X                "1950"//���ΰ����ȸ����                    
#define	   x196X                "1960"//�Һΰ�����                          
#define	   x201X                "2010"//�ſ�������ȸ���� ��û�����       
#define	   x202X                "2020"//�޴���ȭ SMS�߼� ����Ű             
#define	   x204X                "2040"//�����ڵ�                            
#define	   x205X                "2050"//����Ʈ��                            
#define	   x206X                "2060"//����Ʈ�ּ�                          
#define	   x207X                "2070"//�ε��� �ü���ȸ �󼼳���            
#define	   x208X                "2080"//�߰� �����ڵ�                       
#define	   x209X                "2090"//� ������                         
#define	   x210X                "2100"//�ſ�������ȸ���� ��������       
#define	   x211X                "2110"//�ſ�������ȸ ä��������(����) ����  
#define	   x811X                "8110"//�ſ�������ȸ ä��������(����) 2     
#define	   x212X                "2120"//�ſ�������ȸ ä��������(����) ����  
#define	   x812X                "8120"//�ſ�������ȸ ä��������(����) 2     
#define	   x213X                "2130"//�ſ�������ȸ �ſ밳������ ����      
#define	   x813X                "8130"//�ſ�������ȸ �ſ밳������ 2         
#define	   x214X                "2140"//�ſ�������ȸ ��ȸ������� ����      
#define	   x215X                "2150"//�������� ����                       
#define	   x815X                "8150"//�������� ���� 2                     
#define	   x216X                "2160"//���ݼ��� ���� ����                
#define	   x816X                "8160"//���ݼ��� ���� ���� 2              
#define	   x217X                "2170"//ä���������� ����                   
#define	   x817X                "8170"//ä���������� ���� 2                 
#define	   x218X                "2180"//CB��ü���� ����                     
#define	   x818X                "8180"//CB��ü���� ���� 2                   
#define	   x828X                "8280"//CB��ü���� ���� 3                   
#define	   x219X                "2190"//�������� ����                       
#define	   x220X                "2200"//�ֹε�Ϲ�ȣ(����:��ȣȭ�ؼ�)       
#define	   x221X                "2210"//���ι�ȣ(�ֹ���ȣ)                  
#define	   x222X                "2220"//���ܰ� �ŷ�������ȣ                 
#define	   x223X                "2230"//�������� �ۼ��� �����              
#define	   x224X                "2240"//�����ŷ� SST�Ϸù�ȣ                
#define	   x225X                "2250"//����� ������  
#define	   x226X                "2260"//�������� 1:�Ϲݱ���, 2:�������                    
#define	   x230X                "2300"//�������������� ����                 
#define	   x231X                "2310"//������������1                       
#define	   x232X                "2320"//������������2                       
#define	   x233X                "2330"//������������3                       
#define	   x234X                "2340"//������������4                       
#define	   x235X                "2350"//������������5                       
#define	   x236X                "2360"//������������6                       
#define	   x237X                "2370"//������������7                       
#define	   x238X                "2380"//������������8                       
#define	   x240X                "2400"//�Ǹ�ó/���� ���� ����(��������Ƽ��) 
#define	   x241X                "2410"//���� ���� ���䳻��(��������Ƽ��)    
#define	   x242X                "2420"//�ſ���ο�û����(��������Ƽ��)      
#define	   x243X                "2430"//�ſ�������䳻��(��������Ƽ��)      
#define	   x244X                "2440"//�ȳ��޼���                          
#define	   x245X                "2450"//��������1                           
#define	   x246X                "2460"//��������2                           
#define	   x247X                "2470"//Ƽ�ϳ��� ����(��������Ƽ��)         
#define	   x248X                "2480"//�Ǹ�ó ���䳻��(��������Ƽ��)       
#define	   x249X                "2490"//����/�������� ���䳻��(��������Ƽ��)
#define	   x250X                "2500"//��߽ð� ���䳻��                   
#define	   x251X                "2510"//�ڵ��¼� ���� ��û ����             
#define	   x252X                "2520"//�ڵ��¼� ���� ���� ����             
#define	   x253X                "2530"//�ֹ����� ���� ����                  
#define	   x254X                "2540"//��¸ż����� ����                   
#define	   x255X                "2550"//�߱� ���� ���� 1(��������Ƽ��)      
#define	   x256X                "2560"//�߱� ���� ���� 2(��������Ƽ��)      
#define	   x257X                "2570"//�߱� ���� ���� 3(��������Ƽ��)      
#define	   x258X                "2580"//�߱� ���� ���� 4(��������Ƽ��)      
#define	   x260X                "2600"//TC/TE���� ��ȸ/��û ���볻��        
#define	   x261X                "2610"//TC/TE���� ��û ����                 
#define	   x270X                "2700"//���ް��� �ݾ���ȸ                   
#define	   x271X                "2710"//������ ��ȸ                         
#define	   x272X                "2720"//���°��� ��Ÿ����                   
#define	   x273X                "2730"//��ǰ, ������� ��Ÿ����             
#define	   x280X                "2800"//�����ڵ�                            
#define	   x281X                "2810"//�����                              
#define	   x282X                "2820"//����                                
#define	   x283X                "2830"//û�����ű�                          
#define	   x284X                "2840"//����(����)�ݾ�                      
#define	   x285X                "2850"//û��(����)�ݾ�                      
#define	   x286X                "2860"//����ݾ�ȭ��                        
#define	   x287X                "2870"//�����ǰ�����з�                    
#define	   x288X                "2880"//û�����                            
#define	   x289X                "2890"//��ȯ���                            
#define	   x290X                "2900"//�����ǰȭ��                        
#define	   x291X                "2910"//����ī��� ��ǰ��                 
#define	   x299X                "2990"//�ý��� ��������                     
#define	   x300X                "3000"//������ ��󿩺�                     
#define	   x301X                "3010"//�ѵ����� ������                     
#define	   x302X                "3020"//����� ������                     
#define	   x303X                "3030"//�Ż��� ������                     
#define	   x304x                "3040"//������ȸ ������(����)               
#define	   x305x                "3050"//�ѵ����� ������(����)               
#define	   x306x                "3060"//ķ���� ������(����)                 
#define	   x310X                "3100"//������ü���� 
#define	   x3101                "3101"//������ü����                       
#define	   x311X                "3110"//������������ڵ�                    
#define	   x312X                "3120"//������¹�ȣ                        
#define	   x313X                "3130"//ù�����Ա���                        
#define	   x314X                "3140"//��������ID                          
#define	   x315X                "3150"//������                              
#define	   x316X                "3160"//�ǸŻ�ǰ���Ϸù�ȣ                  
#define	   x320X                "3200"//�Ž����� ��������                   
#define	   x915X                "9150"//��й�ȣ(����/�ϳ�/�Ｚ��/����)     
#define	   x9151                "9151"//��й�ȣ(����/�ϳ�/�Ｚ��/����)     
#define	   x034X                "0340"//ó���䱸����(��ֱ���)      
#define	   x035X                "0350"//������ �� ���� ��������     
#define	   x036X                "0360"//�����Ժ� ���� ��������      
#define	   x047X                "0470"//SST Serial Number(SST-ID)   
#define	   x046X                "0460"//����γ���
#define	   x051X                "0510"//�����ڵ�
#define	   x083X                "0830"//��Ÿ���������              
#define	   x084X                "0840"//Shutdown�ð�                
#define	   x102X                "1020"//H/W Module ��������         
#define	   x103X                "1030"//�ڿ� ���� ��������          
#define	   x105X                "1050"//�ѿ�����ݰǼ�              
#define	   x106X                "1060"//�ѿ�����ݱݾ�              
#define	   x107X                "1070"//��ǥ ��ݰǼ�               
#define	   x108X                "1080"//��ǥ ��ݱݾ�               
#define	   x109X                "1090"//�Ѽ�����ݰǼ�  
#define	   x110X                "1100"//�Ѽ�����ݱݾ�                 
#define	   x111X                "1110"//�Ѽ��������ҰǼ�      
#define	   x112X                "1120"//�Ѽ��������ұݾ�
#define	   x113X                "1130"//��ü�Ǽ�      
#define	   x114X                "1140"//��ü�ݾ�
#define	   x117X                "1170"//Ÿ����ü�Ǽ�      
#define	   x118X                "1180"//Ÿ��ݾ�
            
#define	   x122X                "1220"//���ԱݰǼ�                  
#define	   x123X                "1230"//���Աݱݾ�                  
#define	   x124X                "1240"//��ǥ�ԱݰǼ�                
#define	   x125X                "1250"//��ǥ�Աݱݾ�                
#define	   x126X                "1260"//������������                
#define	   x132X                "1320"//���������հ�����            
#define	   x133X                "1330"//���������հ�ð�            
#define	   x134X                "1340"//��������հ�����            
#define	   x135X                "1350"//��������հ�ð�            
#define	   x136X                "1360"//Cassette1 �ѹ���ż�        
#define	   x137X                "1370"//Cassette2 �ѹ���ż�        
#define	   x138X                "1380"//Cassette3 �ѹ���ż�        
#define	   x139X                "1390"//Cassette4 �ѹ���ż�        
#define	   x140X                "1400"//Cassette1 ��ȸ���ż�        
#define	   x141X                "1410"//Cassette2 ��ȸ���ż�        
#define	   x142X                "1420"//Cassette3 ��ȸ���ż�        
#define	   x143X                "1430"//Cassette4 ��ȸ���ż�        
#define	   x145X                "1450"//�ؿ�/���/ī����Ѱŷ��Ǽ�  
#define	   x146X                "1460"//�ؿ�/���/ī����Ѱŷ��ݾ�  
#define	   x147X                "1470"//���ݹ̼��� �ѰǼ�           
#define	   x148X                "1480"//���ݹ̼��� �ѱݾ�           
#define	   x149X                "1490"//��ݼ��ͺ� ���� �ѰǼ�      
#define	   x150X                "1500"//��ݼ��ͺ� ���� �ѱݾ�      
#define	   x151X                "1510"//������ �ѰǼ�             
#define	   x152X                "1520"//������ �ѱݾ�             
#define	   x153X                "1530"//��ݰ����� �ѰǼ�         
#define	   x154X                "1540"//��ݰ����� �ѱݾ�         
#define	   x155X                "1550"//�̿Ϸ���� �ѰǼ�(�������) 
#define	   x156X                "1560"//�̿Ϸ���� �ѱݾ�(�������) 
#define	   x157X                "1570"//���� ���� �� �Ǽ�           
#define	   x158X                "1580"//���� ���� �� �ݾ�           
#define	   x159X                "1590"//T-Money���������� �ѰǼ�    
#define	   x160X                "1600"//T-Money���������� �ѱݾ�    
#define	   x161X                "1610"//��Ÿ�ŷ� �ѰǼ�             
#define	   x162X                "1620"//��Ÿ�ŷ� �ѱݾ�             
#define	   x163X                "1630"//��Ÿ��� �ѰǼ�             
#define	   x164X                "1640"//��Ÿ��� �ѱݾ�             
#define	   x165X                "1650"//������α׷�����            
#define	   x173X                "1730"//�Ա������� �ѰǼ�         
#define	   x174X                "1740"//�Ա������� �ѱݾ�         
#define	   x175X                "1750"//�Աݰ����� �ѰǼ�         
#define	   x176X                "1760"//�Աݰ����� �ѱݾ�         
#define	   x177X                "1770"//�Աݼ������ �ѰǼ�         
#define	   x178X                "1780"//�Աݼ������ �ѱݾ�         
#define	   x179X                "1790"//�Աݹ̼��� �ѰǼ�           
#define	   x180X                "1800"//�Աݹ̼��� �ѱݾ�           
#define	   x182X                "1820"//������ �ѹ���ż�           
#define	   x183X                "1830"//5������ �ѹ���ż�          
#define	   x184X                "1840"//10������ �ѹ���ż�         
#define	   x185X                "1850"//������ ���Աݸż�           
#define	   x186X                "1860"//5������ ���Աݸż�          
#define	   x187X                "1870"//10������ ���Աݸż�         
#define	   x188X                "1880"//��Ÿ���� ���Աݱݾ�         
#define	   x317X                "3170"//5õ���� �ѹ���ż�          
#define	   x318X                "3180"//1õ���� �ѹ���ż�          
#define	   x319X                "3190"//��ȭ �ѹ���ݾ�             
#define	   x910x                "9100"//�귣������ ����             
#define	   x911x                "9110"//��⼳������                
#define	   x912X                "9120"//������ĳ�� �ŷ����ðǼ� ����
#define	   x463X                "4630"//������� �α����� �ȳ� 
#define	   x914x                "9140"//RPC ���� ����
#define	   x916x                "9160"//VPN IP

#define	   x337x                "3370"//���ȯ������ǥ             
#define	   x338x                "3380"//��ġ�ھ�ü����              
#define	   x339X                "3390"//�����ڸ�(or ������ȣ)

#define	   x2082                "2082"//Host NG(���)

////////////////////////////////////////////////////////////////////////////////
//#N0142 ���ڻ�ǰ��
#define	   x0132				"0132" //���ڻ�ǰ�� IC DATA
#define	   x0153				"0153" //���ڻ�ǰ�� IC DATA
#define	   x3161				"3161" //���ڻ�ǰ�� �Ϸù�ȣ
#define	   x340X				"3400" //���ڻ�ǰ�� ��з�����Ÿ
#define	   x341X				"3410" //���ڻ�ǰ�� �����»�� ��ȭ��ȣ
#define	   x342X				"3420" //���ڻ�ǰ�� ����������
#define	   x343X				"3430" //���ڻ�ǰ�� ��ȿ�Ⱓ
#define	   x0231				"0231" //���ڻ�ǰ�� �Ǹ� ��� �ݾ�


////////////////////////////////////////////////////////////////////////////////
//#N0161 ������ ��ȸ
#define	   x168X					"1680" //�ſ�ī�� ���ݼ��� ������

////////////////////////////////////////////////////////////////////////////////
//#N0176 �ؿ�ī��
#define	   x346X					"3460" //�ؿ�ī�� �귣�� ����

#define	   x345X					"3450" // ������ ����Ʈ ����

////////////////////////////////////////////////////////////////////////////////
//#N0184 ����ī�� �ѵ�����
#define	   x307X					"3070" //����ī�� �ѵ�����

#define	   x040x					"0400" //macadress  #N0164
////////////////////////////////////////////////////////////////////////////////
//#N9999 EMVSETINFO
#define	   x358X					"3580" //1. Terminal Capabilitise(9F33)(3)
#define	   x359X					"3590" //2. Terminal Verification Result(95)(5)
#define	   x360X					"3600" //3. Unpredictable Number(9F37)(4)
#define	   x361X					"3610" //4. Application Cryptogram(9F26)(8)
#define	   x362X					"3620" //5. Application Transaction Counter(9F36)(2)
#define	   x363X					"3630" //6. Application Interchange Profile(82)(2)
#define	   x364X					"3640" //7. Transaction Type(9C)(1)
#define	   x365X					"3650" //8. Terminal Transaction Date(9A)(3)
#define	   x366X					"3660" //9. Transaction Amount(9F02)(6)
#define	   x367X					"3670" //10. Transaction Currency Code(5F2A)(2)
#define	   x368X					"3680" //11. Another Transaction Amount(9F03)(6)
#define	   x369X					"3690" //12. Cryptogram Information Data(9F27)(1)	
#define	   x370X					"3700" //13. PAN Sequence Number(5F34)(1)
#define	   x371X					"3710" //14. TerminalType(9F35)(1)	
#define	   x372X					"3720" //15. Amount in Ref. Currency(9F3A)(4)
#define	   x373X					"3730" //16. Transaction Ref. Currency Code(9F3C) (2)
#define	   x374X					"3740" //17. Data Authentication Code(DAC) ( 9F45 ) (2)
#define	   x375X					"3750" //18. M/CHIP Version(9F09)(1)	
#define	   x376X					"3760" //19. Issuer Application Data(9F10)(Variables) (2) -> 20. Length Indicator[2]	
#define	   x377X					"3770" //20. Derivation Key Index[2]
#define	   x378X					"3780" //21. Cryptogram Version Nr[2]
#define	   x379X					"3790" //22. Card Verification Result[12]
#define	   x380X					"3800" //23. Issuer Discretionary Data[50]			           --> ������ (�����ʿ�)
#define	   x386X					"3860" //24. Terminal Country Code[4]		

#define	   x387X					"3870" //�������� �����ȣ(ȸ��)		

// �� #N0278	
#define	   x390X					"3900" //Rebooting ��������	
#define	   x388X					"3880" //������ ���� �ʵ�	
#define	   x389X					"3890" //�������� �ʵ�		

//ARPC
#define	   x381X					"3810" //ARPC (16)
#define	   x382X					"3820" //APRCResposeCode(4)
#define	   x383X					"3830" //IssuerScriptLen+IssuerScriptResult 

#define	   x353X					"3530" //�����н� SAM DATA					//#N0266
#define	   x354X					"3540" //�����н� �ŷ��α׵�����
#define	   x355X					"3550" //�����н� �ŷ��Ϸ���ȣ�α�
#define	   x356X					"3560" //�����н� SAM DATA �α�
#define	   x357X					"3570" //�����н� ID ���� ������

#define	   x401X					"4010" 
#define	   x4015					"4015" 

#define	   x464X					"4640" //���ݺ���üũ
#define	   x445X					"4450" //��ݻ��� �м� ���� �����һ��� (������缭 ����)
#define	   x459X					"4590" 
#define	   x470X					"4700" 
#define	   x471X					"4710" 
#define	   x4721					"4721" 
#define	   x4751					"4751" 

#endif
