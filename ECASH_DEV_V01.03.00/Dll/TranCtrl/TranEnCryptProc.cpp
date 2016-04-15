#include "stdafx.h"
#include <direct.h>
#include "Winbase.h"
#include "EzCaDes.h"
#include "SeedDWS.h"
#include "Seedx.h"
//#CS0003
#include <MATH.H>

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Tran\WREncClt.h"
#include "..\..\H\Tran\HWEncClt.h"
#include "..\..\H\Tran\fnHDEncLib.h"
#include "..\..\H\Tran\DBEncClt.h"
#include "..\..\H\Tran\DSEncClt.h"
#include "..\..\H\Tran\DYEncClt.h"
#include "..\..\H\Tran\trust_connect.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\MacroDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\Dll\DevCtrl\DevDes.h"
#include "..\..\H\Dll\DevCmn.h"

#include "..\..\H\Tran\TranCmn.h"

UString usClientDecData;
int	 fnAPP_Encrypt_240_Step_Flow( char *_pszTransBuffer, UString *_pusDecData );


INI_K_INFO _INI_KEY[]={
	"C_R_YYMMDD"	,
	"C_V_YYMMDD"	,
	"K_R_YYMMDD"	,
	"K_V_YYMMDD"	,
	"K_S_FLAG"
};


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


ISA_HANDLE			m_handle_NON;
ISA_HANDLE			m_handle_SC;
XM_CLIENT_CTX		m_KIUP_XM_CTX;
KB_XM_CTX			m_KB_XM_CTX;
char				m_non_encdata[ISA_MAX_ENC_LEN(20)];
MSG_DATA			m_pMsgData;			

int CTranCmn::fnAPP_Encrypt_Flag_Init()
{

	HFILE fi;
	int ii = KB_ClientLib_Init (&m_KB_XM_CTX, KB_104_CERT);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Encrypt_Flag_Init SendLength:[%d]", ii);
	
	fi = _lopen(_ATMKEYTABELE_INI, OF_READWRITE);
	if (fi < 0) 
	{
		fnAPP_FileTable_Init(_KEYTABELE_INI);
	}
	else _lclose(fi);

//	DeleteFile(_ATMTRCODEENCRYPT_INI);

	fi = _lopen(_ATMTRCODEENCRYPT_INI, OF_READWRITE);
	if (fi < 0) 
	{
		fnAPP_FileTable_Init(_ORGENCRYPT_INI);
	}
	else _lclose(fi);


	return T_OK;
}


int CTranCmn::fnAPP_FileTable_Init(int type)
{
	switch (type) 
	{
		case _KEYTABELE_INI:
			fnAPP_KeyTable_Init();
			break;
		case _ORGENCRYPT_INI:
			fnAPP_TranBank_Flag_Init();
//			fnAPP_TranBank_Flag_Init_T();
			break;
		default:
			break;
	}
	return T_OK;
}

int CTranCmn::fnAPP_KeyTable_Init()
{

	HFILE fi;
	int	i=0;
	int j=0;
	char	szKey1[] = "MASTER";
	char	szKey2[] = "SLAVE0";
	CString cstZERO_M("");
	CString cstZERO_S("");
	cstZERO_M = "00000000000000000000000000000000";
	cstZERO_S = "0000000000000000|0000000";

	fi = _lopen(_ATMKEYTABELE_INI, OF_READWRITE);

	if (fi < 0)
	{
		EnterCriticalSection(&m_csLock);
			
		//해외
 		IniSetStr(_ATMKEYTABELE_INI,TVISACARD, szKey1, cstZERO_M );
 		IniSetStr(_ATMKEYTABELE_INI,TVISACARD, szKey2, cstZERO_S );

		//외환
		IniSetStr(_ATMKEYTABELE_INI, KOEBANKID, szKey1, cstZERO_M );
		IniSetStr(_ATMKEYTABELE_INI, KOEBANKID, szKey2, cstZERO_S );

		//우리
		IniSetStr(_ATMKEYTABELE_INI, WORINNERID1, szKey1, cstZERO_M );
		IniSetStr(_ATMKEYTABELE_INI, WORINNERID1, szKey2, cstZERO_S );

		//전북
		IniSetStr(_ATMKEYTABELE_INI, JEOINNERID, szKey1, cstZERO_M );
		IniSetStr(_ATMKEYTABELE_INI, JEOINNERID, szKey2, cstZERO_S );
	
		//청호기본
		IniSetStr(_ATMKEYTABELE_INI, DEFAULTID, szKey1, cstZERO_M );
		IniSetStr(_ATMKEYTABELE_INI, DEFAULTID, szKey2, cstZERO_S );
		
		//광주
		IniSetStr(_ATMKEYTABELE_INI, KAWINNERID, szKey1, cstZERO_M );
		IniSetStr(_ATMKEYTABELE_INI, KAWINNERID, szKey2, cstZERO_S );
		
		//하이투자증권
		IniSetStr(_ATMKEYTABELE_INI, HISBANKID, szKey1, cstZERO_M );
		IniSetStr(_ATMKEYTABELE_INI, HISBANKID, szKey2, cstZERO_S );
		
		//씨티
		IniSetStr(_ATMKEYTABELE_INI, CITINNERID, szKey1, cstZERO_M );
		IniSetStr(_ATMKEYTABELE_INI, CITINNERID, szKey2, cstZERO_S );

		//신한증권
		IniSetStr(_ATMKEYTABELE_INI, GOMSBANKID, szKey1, cstZERO_M );
		IniSetStr(_ATMKEYTABELE_INI, GOMSBANKID, szKey2, cstZERO_S );
		
		//동부증권
		IniSetStr(_ATMKEYTABELE_INI, DOBSBANKID, szKey1, cstZERO_M );
		IniSetStr(_ATMKEYTABELE_INI, DOBSBANKID, szKey2, cstZERO_S );

		LeaveCriticalSection(&m_csLock);
	}
	else
	{
		_lclose(fi);
	}
	return T_OK;

}

int CTranCmn::fnAPP_TranBank_Flag_Init()
{
	HFILE fi;
	int	i=0;
	int j=0;
	int nTranCode = 0; 
	char	szSection[1024];
	char	szKey[1024];
	char	szKeyValue[] = "01";

	fi = _lopen(_ATMTRCODEENCRYPT_INI, OF_READWRITE);
	if (fi < 0)
	{
		EnterCriticalSection(&m_csLock);

		for (int i = 0; i < itemof(BankCodeTbl); i++)
		{
//			if (memcmp(BankCodeTbl[i].BankCode, BankCodeTbl[i].OrgCode, strlen(BankCodeTbl[i].OrgCode)) == 0)
			{
				//1 출금,조회,이체,수취이체,입금,수수료조회
				sprintf(szSection, "%03d", Asc2Int(BankCodeTbl[i].BankCode, strlen(BankCodeTbl[i].BankCode)));
				sprintf(szKey, "%s|0000", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, fnAPP_Get_BankName(Asc2Int(BankCodeTbl[i].BankCode, strlen(BankCodeTbl[i].BankCode))));

				//==================================================================================
				sprintf(szKey, "%s|2190", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|7210", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|7220", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|7230", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|7250", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|7380", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|7530", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				//==================================================================================

				//새마을금고
				if(Asc2Int(BankCodeTbl[i].OrgEnc, strlen(szSection)) == nEnCmd045_45)
				{
					sprintf(szKey, "%s|7260", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
					sprintf(szKey, "%s|4370", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
					sprintf(szKey, "%s|4390", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
				}
				//대신증권
				else
				if(Asc2Int(BankCodeTbl[i].OrgEnc, strlen(szSection)) == nEnCmd267_267)
				{
					sprintf(szKey, "%s|7260", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
					sprintf(szKey, "%s|4370", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
					sprintf(szKey, "%s|4390", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
				}
				//신한금융
				else
				if(Asc2Int(BankCodeTbl[i].OrgEnc, strlen(szSection)) == nEnCmd278_278)
				{
					sprintf(szKey, "%s|7260", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
					sprintf(szKey, "%s|4370", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
					sprintf(szKey, "%s|4390", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
				}
				//삼성증권
				else
				if(Asc2Int(BankCodeTbl[i].OrgEnc, strlen(szSection)) == nEnCmd240_364)
				{
					sprintf(szKey, "%s|7260", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
					sprintf(szKey, "%s|4370", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
					sprintf(szKey, "%s|4390", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
				}
				//대우증권
				else
				if(Asc2Int(BankCodeTbl[i].OrgEnc, strlen(szSection)) == nEnCmd238_368)
				{
					sprintf(szKey, "%s|7260", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
					sprintf(szKey, "%s|4370", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
					sprintf(szKey, "%s|4390", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
				}

				else
				{
					sprintf(szKey, "%s|7260", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
					sprintf(szKey, "%s|4370", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
					sprintf(szKey, "%s|4390", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				}
				//==================================================================================
				sprintf(szKey, "%s|4350", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				//==================================================================================
				sprintf(szKey, "%s|4310", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|4330", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);

				//==================================================================================
				sprintf(szKey, "%s|4320", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|4340", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				//==================================================================================

				//현대공동망 (입금)
				if(Asc2Int(BankCodeTbl[i].OrgEnc, strlen(szSection)) == nEnCmd004_6)
				{
					sprintf(szKey, "%s|7200", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, KUMBANKID2);
					sprintf(szKey, "%s|4410", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, KUMBANKID2);
				}
// 				else
// 				if(Asc2Int(BankCodeTbl[i].OrgEnc, strlen(szSection)) == nEnCmd007_7)
// 				{
// 					sprintf(szKey, "%s|7200", szSection);
// 					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, SUHINNERID);
// 					sprintf(szKey, "%s|4410", szSection);
// 					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, SUHINNERID);
// 				}
				else
				{
					sprintf(szKey, "%s|7200", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
					sprintf(szKey, "%s|4410", szSection);
					IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, HYDSBANKID);
				}

			}
		}

		LeaveCriticalSection(&m_csLock);		
	}
	else
	{
		_lclose(fi);
	}

	return T_OK;

}



// SOFO, ININTECH, NON 
int CTranCmn::fnAPP_Bank_Key_Convert(int bc)
{
	int i = 0;
	int iGetKey = 0;
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_BankKey_Get_Flag:: [%d]" , bc);
	
	switch(bc)
	{
	case KBS_ORG_INT_004: //1
	case KBS_ORG_INT_006:
	case KBS_ORG_INT_029:
	case KBS_ORG_INT_030:
		for(i = 1; i < 6; i++) fnAPP_BankKey_Get_Flag(KBS_ORG_INT_006, i);
		fnAPP_CertV006(K_V);
		iGetKey = Asc2Int(m_B_E_INFO_INI.m_B_S_C[0].k_s_flag, 2);
		if(iGetKey == PLAINTEXT)
		{
			return K_V_F;
		}
		else
		if(iGetKey == CIPHERTEXT)
		{
			return K_V_S;
		}
		else return K_V_F;
		break;
	case KUB_ORG_INT_003: //2
		for(i = 1; i < 6; i++) fnAPP_BankKey_Get_Flag(KUB_ORG_INT_003, i);
		fnAPP_CertV003(K_V);
		iGetKey = Asc2Int(m_B_E_INFO_INI.m_B_S_C[1].k_s_flag,2);
		if(iGetKey == PLAINTEXT)
		{
			return K_V_F;
		}
		else
		if(iGetKey == CIPHERTEXT)
		{
			return K_V_S;
		}
		else return K_V_F;
		break;
	case NON_ORG_INT_011: //3 -> 미사용 
	case NON_ORG_INT_012:
	case NON_ORG_INT_016:
	case NON_ORG_INT_017:
		for(i = 1; i < 6; i++) fnAPP_BankKey_Get_Flag(NON_ORG_INT_011, i);
		iGetKey = Asc2Int(m_B_E_INFO_INI.m_B_S_C[2].k_s_flag,2);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Bank_Key_Convert_111: [%d]" , iGetKey);

		if(iGetKey == CIPHERTEXT)
		{
			if(fnAPP_CertV011(K_V) == K_V_F)	return K_V_F;
			sprintf(m_B_E_INFO_INI.m_B_S_C[2].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(NON_ORG_INT_011, K_S_F);
		}
		else
		if(iGetKey == PLAINTEXT)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[2].k_s_flag,"%s","01");
			fnAPP_BankKey_Set_Flag(NON_ORG_INT_011, K_S_F);
		}
		else
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[2].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(NON_ORG_INT_011, K_S_F);
		}

		break;
	case SCO_ORG_INT_023: //4 -> 미사용
		for(i = 1; i < 6; i++) fnAPP_BankKey_Get_Flag(SCO_ORG_INT_023, i);
		iGetKey = Asc2Int(m_B_E_INFO_INI.m_B_S_C[3].k_s_flag,2);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Bank_Key_Convert_023: [%d]" , iGetKey);
		if(iGetKey == CIPHERTEXT)
		{
			if(fnAPP_CertV023(K_V) == K_V_F)	return K_V_F;
			sprintf(m_B_E_INFO_INI.m_B_S_C[3].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(SCO_ORG_INT_023, K_S_F);
		}
		else
		if(iGetKey == PLAINTEXT)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[3].k_s_flag,"%s","01");
			fnAPP_BankKey_Set_Flag(SCO_ORG_INT_023, K_S_F);
		}
		else
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[3].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(SCO_ORG_INT_023, K_S_F);
		}		
		break;
	case WRT_ORG_INT_247: //5 NH투자
		for(i = 1; i < 6; i++) fnAPP_BankKey_Get_Flag(WRT_ORG_INT_247, i);
		iGetKey = Asc2Int(m_B_E_INFO_INI.m_B_S_C[4].k_s_flag,2);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_BankKey_Get_Flag: [%d]" , iGetKey);
		if(iGetKey == CIPHERTEXT)
		{
			return K_V_S;
		}
		else return K_V_F;
		break;
	case SST_ORG_INT_CVT_240://6 삼성증권
	case SST_ORG_INT_240:
		for(i = 1; i < 6; i++) fnAPP_BankKey_Get_Flag(SST_ORG_INT_CVT_240, i);
		iGetKey = Asc2Int(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,2);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_BankKey_Get_Flag: [%d]" , iGetKey);
		if(iGetKey == CIPHERTEXT)
		{
			if(TN_TCClientSessionDataStatus() < 0) return K_V_F;
			else 
				return K_V_S;
		}
		else
		if(iGetKey == CIPHERTEXT_TMP)
		{
			return CIPHERTEXT_TMP;
		}
		else return K_V_F;
		break;

	case HWT_ORG_INT_269: //7 한화증권
		for(i = 1; i < 6; i++) fnAPP_BankKey_Get_Flag(HWT_ORG_INT_269, i);
		iGetKey = Asc2Int(m_B_E_INFO_INI.m_B_S_C[6].k_s_flag,2);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_BankKey_Get_Flag: [%d]" , iGetKey);
		if(iGetKey == CIPHERTEXT)
		{
			return K_V_S;
		}
		else return K_V_F;
		break;

	case KJB_ORG_INT_034://8 광주
		for(i = 1; i < 6; i++) fnAPP_BankKey_Get_Flag(KJB_ORG_INT_034, i);
		iGetKey = Asc2Int(m_B_E_INFO_INI.m_B_S_C[7].k_s_flag,2);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_BankKey_Get_Flag: [%d]" , iGetKey);
		if(iGetKey == CIPHERTEXT)
		{
			return K_V_S;
		}
		else return K_V_F;
		break;
	case KEB_ORG_INT_005://9 외환
		for(i = 1; i < 6; i++) fnAPP_BankKey_Get_Flag(KEB_ORG_INT_005, i);
		iGetKey = Asc2Int(m_B_E_INFO_INI.m_B_S_C[7].k_s_flag,2);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_BankKey_Get_Flag: [%d]" , iGetKey);
		if(iGetKey == CIPHERTEXT)
		{
			return K_V_S;
		}
		else return K_V_F;
		break;
	case WRB_ORG_INT_020://10 우리
		for(i = 1; i < 6; i++) fnAPP_BankKey_Get_Flag(WRB_ORG_INT_020, i);
		iGetKey = Asc2Int(m_B_E_INFO_INI.m_B_S_C[7].k_s_flag,2);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_BankKey_Get_Flag: [%d]" , iGetKey);
		if(iGetKey == CIPHERTEXT)
		{
			return K_V_S;
		}
		else return K_V_F;
		break;



	case DOB_ORG_INT_279: //11 동부
		for(i = 1; i < 6; i++) fnAPP_BankKey_Get_Flag(DOB_ORG_INT_279, i);
		iGetKey = Asc2Int(m_B_E_INFO_INI.m_B_S_C[6].k_s_flag,2);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_BankKey_Get_Flag: [%d]" , iGetKey);
		if(iGetKey == CIPHERTEXT)
		{
			return K_V_S;
		}
		else return K_V_F;
		break;
	default:
		break;
	}
	return T_OK;
}
int CTranCmn::fnAPP_CertV006(int ikv)
{
	int iret = 0;

	if( ikv == K_V)
	{
		//안기삼과장님 Spec 정의 문제의 대한 인식 필요. 
		iret = KB_ClientLib_Init (&m_KB_XM_CTX, KB_104_CERT);
		
		if (iret < 0) {
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret Error = [%06d]", iret);
		}

		return K_V_S;
	}
	return T_OK;
}

int CTranCmn::fnAPP_CertV003(int ikv)
{
	int iret = 0;
	char  stroutputdata[10];
	
	if(ikv == K_R)
	{
		iret = XMC2_ClientLib_Init (&m_KIUP_XM_CTX, KUB_103_CERT);
		if (iret < 0) {
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret Error = [%06d]", iret);
			return	K_C_F;
		}
// 		iret = XMC2_Cert_Verify(&m_KIUP_XM_CTX, stroutputdata);
// 		if (iret <= 0)
// 		{
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret Error = [%06d]", iret);
// 			return K_C_F;
// 		}
		if(fnAPP_CertKey_Limit(stroutputdata,K_R) < 0)
		{
			return K_C_F;
		}
		if(fnAPP_CertKey_Limit(m_B_E_INFO_INI.m_B_S_C[1].c_v_yymmdd,K_R) < 0)
		{
			return K_C_F;
		}
		return K_C_S;
	}

	if( ikv == K_V)
	{
		//안기삼과장님 Spec 정의 문제의 대한 인식 필요. 
		iret = XMC2_ClientLib_Init (&m_KIUP_XM_CTX, KUB_103_CERT);
		
		if (iret < 0) {
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret Error = [%06d]", iret);
		}
// 		XMC2_Cert_Verify(&m_KIUP_XM_CTX, stroutputdata);

		return K_V_S;
	}
	
	return C_SUCCESS;
}

int CTranCmn::fnAPP_CertV011(int ikv)
{
	int iret = T_OK;
	if(ikv == K_R)
	{
		if(fnAPP_CertKey_Limit(m_B_E_INFO_INI.m_B_S_C[2].c_v_yymmdd, K_N_C) < 0)
		{
			return K_C_S;
		}
		return K_C_F;

	}

	if(ikv == K_V)
	{
		ISA_Init();
		if(m_handle_NON) {
			ISA_HANDLE_release(m_handle_NON);
			m_handle_NON=NULL;
		}
		ISA_HANDLE_acquire(&m_handle_NON, SL_FULL_SECURITY, NON_111_CERT);
		if(m_handle_NON) {
			ISA_HANDLE_release(m_handle_NON);
			m_handle_NON=NULL;
		}
		iret = ISA_HANDLE_load_encfile(&m_handle_NON, NON_ENCRIPT_DIR,   m_AtmCode, strlen(m_AtmCode));

		if (iret < 0) {
			return K_V_F;
		}
		return K_V_S;
	}
	
	return C_SUCCESS;

}

int CTranCmn::fnAPP_CertV023(int ikv)
{
	int iret = T_OK;
	if(ikv == K_R)
	{
		if(fnAPP_CertKey_Limit(m_B_E_INFO_INI.m_B_S_C[3].c_v_yymmdd, K_N_C) < 0)
		{
			return K_C_S;
		}
		return K_C_F;

	}

	if(ikv == K_V)
	{
		SC_ISA_Init();
		if(m_handle_SC) {
			SC_ISA_HANDLE_release(m_handle_SC);
			m_handle_SC=NULL;
		}
		SC_ISA_HANDLE_acquire(&m_handle_SC, SL_FULL_SECURITY, SC_123_CERT);
		if(m_handle_SC) {
			SC_ISA_HANDLE_release(m_handle_SC);
			m_handle_SC=NULL;
		}
		iret = SC_ISA_HANDLE_load_encfile(&m_handle_SC, SC_ENCRIPT_DIR,   m_AtmCode, strlen(m_AtmCode));

		if (iret < 0) {
			return K_V_F;
		}
		return K_V_S;
	}
	
	return C_SUCCESS;

}

int CTranCmn::fnAPP_CertKey_Limit(char *yymmdd,int type)
{
	int iret = T_OK;
	int	limit = T_OK;

	SYSTEMTIME st;
	GetLocalTime(&st);					// gets current time	
	if(type == K_R) 
	{
		CTime t1( Asc2Int(yymmdd, 4), Asc2Int(&yymmdd[4], 2), Asc2Int(&yymmdd[6], 2), Asc2Int(&yymmdd[8], 2), 0, 0 ); // set  current time
		CTime t2( st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,0 ); // gets current time
		CTimeSpan ts = t1 - t2;  // Subtract 2 CTimes	
		if(ts.GetDays() < 6)
		{
			return T_CANCEL;
		}
	}

	if(type == K_V)
	{
		if(yymmdd[0] == 0x00)
		{
			sprintf(yymmdd, "%04d%02d%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
		}
		CTime t1( Asc2Int(yymmdd, 4), Asc2Int(&yymmdd[4], 2), Asc2Int(&yymmdd[6], 2), Asc2Int(&yymmdd[8], 2), 0, 0 ); // set  current time
		CTime t2( st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,0 ); // gets current time
		CTimeSpan ts = t1 - t2;  // Subtract 2 CTimes	
		if(ts.GetDays() < 1)
		{
			return T_CANCEL;
		}
	}

	if(type == K_N_C)
	{
		CTime t1( Asc2Int(yymmdd, 4), Asc2Int(&yymmdd[4], 2), Asc2Int(&yymmdd[6], 2), 0, 0, 0 ); // set  current time
		CTime t2( st.wYear, st.wMonth, st.wDay, 0, 0,0 ); // gets current time
		CTimeSpan ts = t2 - t1;  // Subtract 2 CTimes
		if(ts.GetDays() == 0)
		{
			return T_CANCEL;
		}
	}
	
	return T_OK;
}


int	CTranCmn::fnAPP_File_Init(int type)
{
	HFILE fi;

	fi = _lopen(_B_E_INFO_INI, OF_READWRITE);
	if (fi < 0)
	{
		_mkdir(_INI);											
		fnAPP_FileMake_Init(_BK_E_INI);
	}
	else
	{
		_lclose(fi);
		for(int i = 1; i < 6; i++)
		{
			fnAPP_BankKey_Get_Flag(KBS_ORG_INT_006,i); //1
			fnAPP_BankKey_Get_Flag(KUB_ORG_INT_003,i); //2
			fnAPP_BankKey_Get_Flag(NON_ORG_INT_011,i); //3
			fnAPP_BankKey_Get_Flag(SCO_ORG_INT_023,i); //4
			fnAPP_BankKey_Get_Flag(WRT_ORG_INT_247,i); //5
			fnAPP_BankKey_Get_Flag(SST_ORG_INT_CVT_240,i); //6
			fnAPP_BankKey_Get_Flag(HWT_ORG_INT_269,i); //7
			fnAPP_BankKey_Get_Flag(KJB_ORG_INT_034,i); //8
			fnAPP_BankKey_Get_Flag(KEB_ORG_INT_005,i); //9
			fnAPP_BankKey_Get_Flag(WRB_ORG_INT_020,i); //10
			fnAPP_BankKey_Get_Flag(DOB_ORG_INT_279,i); //11


		}
	}

	return T_OK;
}
int	CTranCmn::fnAPP_FileMake_Init(int f_type)
{
	switch(f_type)
	{
	case _BK_E_INI:
		fnAPP_BankKey_Flag_Init();
		break;
	default:
		break;
	}
	return T_OK;
}
int	CTranCmn::fnAPP_BankKey_Flag_Init()
{
	int	i=0;
	memset(&m_B_E_INFO_INI,0x00,sizeof(m_B_E_INFO_INI));
	EnterCriticalSection(&m_csLock);
	memset(m_B_E_INFO_INI.m_B_S_C[0].b_c,0x00,sizeof(m_B_E_INFO_INI.m_B_S_C[0].b_c));
	memset(m_B_E_INFO_INI.m_B_S_C[1].b_c,0x00,sizeof(m_B_E_INFO_INI.m_B_S_C[1].b_c));
	memset(m_B_E_INFO_INI.m_B_S_C[2].b_c,0x00,sizeof(m_B_E_INFO_INI.m_B_S_C[2].b_c));
	memset(m_B_E_INFO_INI.m_B_S_C[3].b_c,0x00,sizeof(m_B_E_INFO_INI.m_B_S_C[3].b_c));
	memset(m_B_E_INFO_INI.m_B_S_C[4].b_c,0x00,sizeof(m_B_E_INFO_INI.m_B_S_C[4].b_c));
	memset(m_B_E_INFO_INI.m_B_S_C[5].b_c,0x00,sizeof(m_B_E_INFO_INI.m_B_S_C[5].b_c));
	memset(m_B_E_INFO_INI.m_B_S_C[6].b_c,0x00,sizeof(m_B_E_INFO_INI.m_B_S_C[6].b_c));
	memset(m_B_E_INFO_INI.m_B_S_C[7].b_c,0x00,sizeof(m_B_E_INFO_INI.m_B_S_C[7].b_c));   
	memset(m_B_E_INFO_INI.m_B_S_C[8].b_c,0x00,sizeof(m_B_E_INFO_INI.m_B_S_C[8].b_c));
	memset(m_B_E_INFO_INI.m_B_S_C[9].b_c,0x00,sizeof(m_B_E_INFO_INI.m_B_S_C[9].b_c));
	memset(m_B_E_INFO_INI.m_B_S_C[10].b_c,0x00,sizeof(m_B_E_INFO_INI.m_B_S_C[10].b_c));   
	
	sprintf(m_B_E_INFO_INI.m_B_S_C[0].b_c, "%3.3s", KBS_ORG_SEC);
	sprintf(m_B_E_INFO_INI.m_B_S_C[1].b_c, "%3.3s", KUB_ORG_SEC);
	sprintf(m_B_E_INFO_INI.m_B_S_C[2].b_c, "%3.3s", NON_ORG_SEC);
	sprintf(m_B_E_INFO_INI.m_B_S_C[3].b_c, "%3.3s", SCO_ORG_SEC);
	sprintf(m_B_E_INFO_INI.m_B_S_C[4].b_c, "%3.3s", WRT_ORG_SEC);
	sprintf(m_B_E_INFO_INI.m_B_S_C[5].b_c, "%3.3s", SST_ORG_SEC);
	sprintf(m_B_E_INFO_INI.m_B_S_C[6].b_c, "%3.3s", HWT_ORG_SEC);
	sprintf(m_B_E_INFO_INI.m_B_S_C[7].b_c, "%3.3s", KJB_ORG_SEC);   
	sprintf(m_B_E_INFO_INI.m_B_S_C[8].b_c, "%3.3s", KEB_ORG_SEC);
	sprintf(m_B_E_INFO_INI.m_B_S_C[9].b_c, "%3.3s", WRB_ORG_SEC);
	sprintf(m_B_E_INFO_INI.m_B_S_C[10].b_c, "%3.3s", DBS_ORG_SEC);   


	for(int j = 0; j < 11; j++)									
	{
		
		memset(m_B_E_INFO_INI.m_B_S_C[j].c_r_yymmdd, 0x30,sizeof(m_B_E_INFO_INI.m_B_S_C[j].c_r_yymmdd));
		m_B_E_INFO_INI.m_B_S_C[j].c_r_yymmdd[12]=0x00;
		memset(m_B_E_INFO_INI.m_B_S_C[j].c_v_yymmdd, 0x30,sizeof(m_B_E_INFO_INI.m_B_S_C[j].c_v_yymmdd));	
		m_B_E_INFO_INI.m_B_S_C[j].c_v_yymmdd[12]=0x00;
		memset(m_B_E_INFO_INI.m_B_S_C[j].k_r_yymmdd, 0x30,sizeof(m_B_E_INFO_INI.m_B_S_C[j].k_r_yymmdd));	
		m_B_E_INFO_INI.m_B_S_C[j].k_r_yymmdd[12]=0x00;
		memset(m_B_E_INFO_INI.m_B_S_C[j].k_v_yymmdd, 0x30,sizeof(m_B_E_INFO_INI.m_B_S_C[j].k_v_yymmdd));
		m_B_E_INFO_INI.m_B_S_C[j].k_v_yymmdd[12]=0x00;
	}

	IniSetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[0].c_r_yymmdd	);
	IniSetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[0].c_v_yymmdd	);
	IniSetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[0].k_r_yymmdd	);
	IniSetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[0].k_v_yymmdd	);
	IniSetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i++].n, "00"									);
	i = 0;
	IniSetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[1].c_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[1].c_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[1].k_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[1].k_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i++].n, "00" 								   );
	i = 0;
	IniSetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[2].c_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[2].c_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[2].k_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[2].k_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i++].n, "00"								   );
	i = 0;
	IniSetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[3].c_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[3].c_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[3].k_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[3].k_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i++].n, "00"								   );

	i = 0;
	IniSetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[4].c_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[4].c_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[4].k_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[4].k_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i++].n, "00"								   );

	i = 0;
	IniSetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[5].c_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[5].c_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[5].k_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[5].k_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i++].n, "00"								   );

	i = 0;
	IniSetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[6].c_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[6].c_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[6].k_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[6].k_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i++].n, "00"								   );

	i = 0;
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[7].c_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[7].c_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[7].k_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[7].k_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, "00"								   );

	i = 0;
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[8].c_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[8].c_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[8].k_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[8].k_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, "00"								   );

	i = 0;
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[9].c_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[9].c_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[9].k_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[9].k_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, "00"								   );

	i = 0;
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[10].c_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[10].c_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[10].k_r_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, m_B_E_INFO_INI.m_B_S_C[10].k_v_yymmdd);
	IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i++].n, "00"								   );

	LeaveCriticalSection(&m_csLock);

	return T_OK;
}

int	CTranCmn::fnAPP_BankKey_Set_Flag(int bc, int k_type)
{
	int i = 0;
	switch(bc)
	{
	case KBS_ORG_INT_006:
		if(k_type == C_R)
		{
			IniSetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[0].c_r_yymmdd);
		}
		i++;
		if(k_type == C_V)
		{
			IniSetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[0].c_v_yymmdd);
		}
		i++;
		if(k_type == K_R)
		{
			IniSetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[0].k_r_yymmdd);
		}
		i++;
		if(k_type == K_V)
		{
			IniSetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[0].k_v_yymmdd);
		}
		i++;
		if(k_type == K_S_F)
		{
			IniSetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[0].k_s_flag);
		}
		break;
	case KUB_ORG_INT_003:
		if(k_type == C_R)
		{
			IniSetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[1].c_r_yymmdd);
		}
		i++;
		if(k_type == C_V)
		{
			IniSetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[1].c_v_yymmdd);
		}
		i++;
		if(k_type == K_R)
		{
			IniSetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[1].k_r_yymmdd);
		}
		i++;
		if(k_type == K_V)
		{
			IniSetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[1].k_v_yymmdd);
		}
		i++;
		if(k_type == K_S_F)
		{
			IniSetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[1].k_s_flag);
		}
		break;
	case NON_ORG_INT_011:
		if(k_type == C_R)
		{
			IniSetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[2].c_r_yymmdd);
		}
		i++;
		if(k_type == C_V)
		{
			IniSetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[2].c_v_yymmdd);
		}
		i++;
		if(k_type == K_R)
		{
			IniSetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[2].k_r_yymmdd);
		}
		i++;
		if(k_type == K_V)
		{
			IniSetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[2].k_v_yymmdd);
		}
		i++;
		if(k_type == K_S_F)
		{
			IniSetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[2].k_s_flag);
		}
		break;
	case SCO_ORG_INT_023:
		if(k_type == C_R)
		{
			IniSetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[3].c_r_yymmdd);
		}
		i++;
		if(k_type == C_V)
		{
			IniSetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[3].c_v_yymmdd);
		}
		i++;
		if(k_type == K_R)
		{
			IniSetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[3].k_r_yymmdd);
		}
		i++;
		if(k_type == K_V)
		{
			IniSetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[3].k_v_yymmdd);
		}
		i++;
		if(k_type == K_S_F)
		{
			IniSetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[3].k_s_flag);
		}
		break;
	case WRT_ORG_INT_247:
		if(k_type == C_R)
		{
			IniSetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[4].c_r_yymmdd);
		}
		i++;
		if(k_type == C_V)
		{
			IniSetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[4].c_v_yymmdd);
		}
		i++;
		if(k_type == K_R)
		{
			IniSetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[4].k_r_yymmdd);
		}
		i++;
		if(k_type == K_V)
		{
			IniSetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[4].k_v_yymmdd);
		}
		i++;
		if(k_type == K_S_F)
		{
			IniSetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[4].k_s_flag);
		}
		break;
	case SST_ORG_INT_CVT_240:
	case SST_ORG_INT_240:
		if(k_type == C_R)
		{
			IniSetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[5].c_r_yymmdd);
		}
		i++;
		if(k_type == C_V)
		{
			IniSetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[5].c_v_yymmdd);
		}
		i++;
		if(k_type == K_R)
		{
			IniSetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[5].k_r_yymmdd);
		}
		i++;
		if(k_type == K_V)
		{
			IniSetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[5].k_v_yymmdd);
		}
		i++;
		if(k_type == K_S_F)
		{
			IniSetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[5].k_s_flag);
		}
		break;
	case HWT_ORG_INT_269:
		if(k_type == C_R)
		{
			IniSetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[6].c_r_yymmdd);
		}
		i++;
		if(k_type == C_V)
		{
			IniSetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[6].c_v_yymmdd);
		}
		i++;
		if(k_type == K_R)
		{
			IniSetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[6].k_r_yymmdd);
		}
		i++;
		if(k_type == K_V)
		{
			IniSetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[6].k_v_yymmdd);
		}
		i++;
		if(k_type == K_S_F)
		{
			IniSetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[6].k_s_flag);
		}
		break;
	case KJB_ORG_INT_034:   
		if(k_type == C_R)
		{
			IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[7].c_r_yymmdd);
		}
		i++;
		if(k_type == C_V)
		{
			IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[7].c_v_yymmdd);
		}
		i++;
		if(k_type == K_R)
		{
			IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[7].k_r_yymmdd);
		}
		i++;
		if(k_type == K_V)
		{
			IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[7].k_v_yymmdd);
		}
		i++;
		if(k_type == K_S_F)
		{
			IniSetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[7].k_s_flag);
		}
		break;

	case KEB_ORG_INT_005:   
		if(k_type == C_R)
		{
			IniSetStr(_B_E_INFO_INI, KEB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[8].c_r_yymmdd);
		}
		i++;
		if(k_type == C_V)
		{
			IniSetStr(_B_E_INFO_INI, KEB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[8].c_v_yymmdd);
		}
		i++;
		if(k_type == K_R)
		{
			IniSetStr(_B_E_INFO_INI, KEB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[8].k_r_yymmdd);
		}
		i++;
		if(k_type == K_V)
		{
			IniSetStr(_B_E_INFO_INI, KEB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[8].k_v_yymmdd);
		}
		i++;
		if(k_type == K_S_F)
		{
			IniSetStr(_B_E_INFO_INI, KEB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[8].k_s_flag);
		}
		break;

	case WRB_ORG_INT_020:   
		if(k_type == C_R)
		{
			IniSetStr(_B_E_INFO_INI, WRB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[9].c_r_yymmdd);
		}
		i++;
		if(k_type == C_V)
		{
			IniSetStr(_B_E_INFO_INI, WRB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[9].c_v_yymmdd);
		}
		i++;
		if(k_type == K_R)
		{
			IniSetStr(_B_E_INFO_INI, WRB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[9].k_r_yymmdd);
		}
		i++;
		if(k_type == K_V)
		{
			IniSetStr(_B_E_INFO_INI, WRB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[9].k_v_yymmdd);
		}
		i++;
		if(k_type == K_S_F)
		{
			IniSetStr(_B_E_INFO_INI, WRB_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[9].k_s_flag);
		}
		break;

	case DOB_ORG_INT_279:   
		if(k_type == C_R)
		{
			IniSetStr(_B_E_INFO_INI, DBS_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[10].c_r_yymmdd);
		}
		i++;
		if(k_type == C_V)
		{
			IniSetStr(_B_E_INFO_INI, DBS_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[10].c_v_yymmdd);
		}
		i++;
		if(k_type == K_R)
		{
			IniSetStr(_B_E_INFO_INI, DBS_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[10].k_r_yymmdd);
		}
		i++;
		if(k_type == K_V)
		{
			IniSetStr(_B_E_INFO_INI, DBS_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[10].k_v_yymmdd);
		}
		i++;
		if(k_type == K_S_F)
		{
			IniSetStr(_B_E_INFO_INI, DBS_ORG_SEC, _INI_KEY[i].n, m_B_E_INFO_INI.m_B_S_C[10].k_s_flag);
		}
		break;

	default:
		break;
	}

	return T_OK;
}
int	CTranCmn::fnAPP_BankKey_Get_Flag(int bc, int k_type)
{
	int i = 0;
	switch(bc)
	{
	case KBS_ORG_INT_006: //1
		i =0;
		sprintf(m_B_E_INFO_INI.m_B_S_C[0].b_c, "%3.3s", KBS_ORG_SEC);
		if(k_type == C_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[0].c_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == C_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[0].c_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[0].k_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[0].k_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_S_F)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[0].k_s_flag, "%s", IniGetStr(_B_E_INFO_INI, KBS_ORG_SEC, _INI_KEY[i].n, "00"));
		}
		break;
	case KUB_ORG_INT_003: ///2
		i =0;
		sprintf(m_B_E_INFO_INI.m_B_S_C[1].b_c, "%3.3s", KUB_ORG_SEC);
		if(k_type == C_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[1].c_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == C_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[1].c_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[1].k_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[1].k_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_S_F)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[1].k_s_flag, "%s", IniGetStr(_B_E_INFO_INI, KUB_ORG_SEC, _INI_KEY[i].n, "00"));
		}
		break;
	case NON_ORG_INT_011: //3
		i =0;
		sprintf(m_B_E_INFO_INI.m_B_S_C[2].b_c, "%3.3s", NON_ORG_SEC);
		if(k_type == C_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[2].c_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == C_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[2].c_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[2].k_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[2].k_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_S_F)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[2].k_s_flag, "%s", IniGetStr(_B_E_INFO_INI, NON_ORG_SEC, _INI_KEY[i].n, "00"));
		}
		break;
	case SCO_ORG_INT_023: //4
		i =0;
		sprintf(m_B_E_INFO_INI.m_B_S_C[3].b_c, "%3.3s", SCO_ORG_SEC);
		if(k_type == C_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[3].c_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == C_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[3].c_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[3].k_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[3].k_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_S_F)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[3].k_s_flag, "%s", IniGetStr(_B_E_INFO_INI, SCO_ORG_SEC, _INI_KEY[i].n, "00"));
		}
		break;
	case WRT_ORG_INT_247: //5
		i =0;
		sprintf(m_B_E_INFO_INI.m_B_S_C[4].b_c, "%3.3s", WRT_ORG_SEC);
		if(k_type == C_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[4].c_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == C_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[4].c_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[4].k_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[4].k_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_S_F)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[4].k_s_flag, "%s", IniGetStr(_B_E_INFO_INI, WRT_ORG_SEC, _INI_KEY[i].n, "00"));
		}
		break;
	case SST_ORG_INT_CVT_240: //6
	case SST_ORG_INT_240:
		i =0;
		sprintf(m_B_E_INFO_INI.m_B_S_C[5].b_c, "%3.3s", SST_ORG_SEC);
		if(k_type == C_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[5].c_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == C_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[5].c_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[5].k_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[5].k_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_S_F)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag, "%s", IniGetStr(_B_E_INFO_INI, SST_ORG_SEC, _INI_KEY[i].n, "00"));
		}
		break;
	case HWT_ORG_INT_269: //7
		i =0;
		sprintf(m_B_E_INFO_INI.m_B_S_C[6].b_c, "%3.3s", HWT_ORG_SEC);
		if(k_type == C_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[6].c_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == C_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[6].c_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[6].k_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[6].k_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_S_F)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[6].k_s_flag, "%s", IniGetStr(_B_E_INFO_INI, HWT_ORG_SEC, _INI_KEY[i].n, "00"));
		}
		break;
	case KJB_ORG_INT_034: //8
		i =0;
		sprintf(m_B_E_INFO_INI.m_B_S_C[7].b_c, "%3.3s", KJB_ORG_SEC);
		if(k_type == C_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[7].c_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == C_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[7].c_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[7].k_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[7].k_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_S_F)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[7].k_s_flag, "%s", IniGetStr(_B_E_INFO_INI, KJB_ORG_SEC, _INI_KEY[i].n, "00"));
		}
		break;
	case KEB_ORG_INT_005: //9
		i =0;
		sprintf(m_B_E_INFO_INI.m_B_S_C[8].b_c, "%3.3s", KEB_ORG_SEC);
		if(k_type == C_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[8].c_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KEB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == C_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[8].c_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KEB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[8].k_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KEB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[8].k_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, KEB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_S_F)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[8].k_s_flag, "%s", IniGetStr(_B_E_INFO_INI, KEB_ORG_SEC, _INI_KEY[i].n, "00"));
		}
		break;
	case WRB_ORG_INT_020: //10
		i =0;
		sprintf(m_B_E_INFO_INI.m_B_S_C[9].b_c, "%3.3s", WRB_ORG_SEC);
		if(k_type == C_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[9].c_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, WRB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == C_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[9].c_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, WRB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[9].k_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, WRB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[9].k_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, WRB_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_S_F)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[9].k_s_flag, "%s", IniGetStr(_B_E_INFO_INI, WRB_ORG_SEC, _INI_KEY[i].n, "00"));
		}
		break;
	case DOB_ORG_INT_279: //11
		i =0;
		sprintf(m_B_E_INFO_INI.m_B_S_C[10].b_c, "%3.3s", DBS_ORG_SEC);
		if(k_type == C_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[10].c_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, DBS_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == C_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[10].c_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, DBS_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_R)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[10].k_r_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, DBS_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_V)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[10].k_v_yymmdd, "%12.12s", IniGetStr(_B_E_INFO_INI, DBS_ORG_SEC, _INI_KEY[i].n, "000000000000"));
		}
		i++;
		if(k_type == K_S_F)
		{
			sprintf(m_B_E_INFO_INI.m_B_S_C[10].k_s_flag, "%s", IniGetStr(_B_E_INFO_INI, DBS_ORG_SEC, _INI_KEY[i].n, "00"));
		}
		break;
	default:
		break;
	}

	return T_OK;
}
/* Sofo, Initech Encrypt Key Registration, Certify*/ 
int CTranCmn::fnAPP_KeyInitStart(int bc, int nFlag)
{
	int iret = 0; 
	char strkeybuff[1024];
	char strkeylen[512];
	memset(strkeybuff,0x00, sizeof(strkeybuff));
	memset(strkeylen,0x00, sizeof(strkeylen));

	memset(m_TXENCKEY.k_len, 0x00, sizeof(m_TXENCKEY.k_len));	
	memset(m_TXENCKEY.k_data, 0x00, sizeof(m_TXENCKEY.k_data));		

	switch(bc)
	{
	case KBS_ORG_INT_004:
	case KBS_ORG_INT_006:
	case KBS_ORG_INT_029:
	case KBS_ORG_INT_030:
		iret = fnAPP_KeyInit004((BYTE *)strkeybuff, "C", strkeylen , SEND_F);
		if(iret == K_V_F) return T_OK;
		memcpy(m_TXENCKEY.k_len, strkeylen, sizeof(m_TXENCKEY.k_len));
		memcpy(m_TXENCKEY.k_data, strkeybuff, Asc2Int(strkeylen,5));
		break;
	case KUB_ORG_INT_003:
		iret = fnAPP_KeyInit003((BYTE *)strkeybuff, "C", strkeylen , SEND_F);
		if(iret == K_V_F) return T_OK;
		memcpy(m_TXENCKEY.k_len, strkeylen, sizeof(m_TXENCKEY.k_len));
		Int2Asc((Asc2Int(strkeylen,5) + 4), &m_TXENCKEY.k_len[0], 5, 10, '0');
		Int2Asc(Asc2Int(strkeylen,5), &m_TXENCKEY.k_data[0], 4, 10, '0');
		memcpy(&m_TXENCKEY.k_data[4], strkeybuff, Asc2Int(strkeylen,5));
		break;
	case NON_ORG_INT_011:
	case NON_ORG_INT_012:
	case NON_ORG_INT_016:
	case NON_ORG_INT_017:
		iret = fnAPP_KeyInit011((BYTE *)strkeybuff, "C", strkeylen , SEND_F);
		if(iret == K_V_F) return T_OK;
		memcpy(m_TXENCKEY.k_len, strkeylen, sizeof(m_TXENCKEY.k_len));
		memcpy(m_TXENCKEY.k_data, strkeybuff, Asc2Int(strkeylen,5));
		break;
	case SCO_ORG_INT_023:
		iret = fnAPP_KeyInit023((BYTE *)strkeybuff, "C", strkeylen , SEND_F);
		if(iret == K_V_F) return T_OK;
		memcpy(m_TXENCKEY.k_len, strkeylen, sizeof(m_TXENCKEY.k_len));
		memcpy(m_TXENCKEY.k_data, strkeybuff, Asc2Int(strkeylen,5));
		break;
	case WRT_ORG_INT_247:
		fnAPP_KeyInit247((BYTE *)strkeybuff, strkeylen, strkeylen , SEND_F);
		memset(m_TXENCKEY.k_r_yymmdd, 0x30, sizeof(m_TXENCKEY.k_r_yymmdd));
		memset(m_TXENCKEY.k_v_yymmdd, 0x30, sizeof(m_TXENCKEY.k_v_yymmdd));
		memcpy(m_TXENCKEY.k_len, strkeylen, sizeof(m_TXENCKEY.k_len));
		memcpy(m_TXENCKEY.k_data, strkeybuff, Asc2Int(strkeylen,5));
		break;
	case SST_ORG_INT_CVT_240:
	case SST_ORG_INT_240:
		memset(m_TXENCKEY.k_r_yymmdd, 0x30, sizeof(m_TXENCKEY.k_r_yymmdd));
		memset(m_TXENCKEY.k_v_yymmdd, 0x30, sizeof(m_TXENCKEY.k_v_yymmdd));
		if(nFlag == 0) 
		{
			TN_TCInitUString( &usClientDecData );
//			fnAPP_KeyInit305((BYTE *)strkeybuff, "P", strkeylen , SEND_F);
			memcpy(m_TXENCKEY.k_apart, "P", sizeof(m_TXENCKEY.k_apart));
			memcpy(m_TXENCKEY.k_len, "00400", sizeof(m_TXENCKEY.k_len));
			memset(m_TXENCKEY.k_data, 0x20, sizeof(m_TXENCKEY.k_data));

		}
		else
		if(nFlag == 1)
		{
			fnAPP_KeyInit240((BYTE *)strkeybuff, "S", strkeylen , SEND_F);
			memcpy(m_TXENCKEY.k_apart, "S", sizeof(m_TXENCKEY.k_apart));
			memcpy(m_TXENCKEY.k_len, strkeylen, sizeof(m_TXENCKEY.k_len));
			memcpy(m_TXENCKEY.k_data, strkeybuff, Asc2Int(strkeylen,5));
		}
		break;
	case HWT_ORG_INT_269:
		fnAPP_KeyInit269((BYTE *)strkeybuff, strkeylen, strkeylen , SEND_F);
		memset(m_TXENCKEY.k_r_yymmdd, 0x30, sizeof(m_TXENCKEY.k_r_yymmdd));
		memset(m_TXENCKEY.k_v_yymmdd, 0x30, sizeof(m_TXENCKEY.k_v_yymmdd));
		memcpy(m_TXENCKEY.k_len, strkeylen, sizeof(m_TXENCKEY.k_len));
		memcpy(m_TXENCKEY.k_data, strkeybuff, Asc2Int(strkeylen,5));
		break;
	case KJB_ORG_INT_034:
		memset(m_TXENCKEY.k_r_yymmdd, 0x30, sizeof(m_TXENCKEY.k_r_yymmdd));
		memset(m_TXENCKEY.k_v_yymmdd, 0x30, sizeof(m_TXENCKEY.k_v_yymmdd));
		memcpy(m_TXENCKEY.k_len, "16", strlen("16"));
		memcpy(m_TXENCKEY.k_data, "                ", 16);
		break;
	case KEB_ORG_INT_005:
		fnAPP_KeyInit005((BYTE *)strkeybuff, strkeylen, strkeylen , SEND_F);
		memset(m_TXENCKEY.k_r_yymmdd, 0x30, sizeof(m_TXENCKEY.k_r_yymmdd));
		memset(m_TXENCKEY.k_v_yymmdd, 0x30, sizeof(m_TXENCKEY.k_v_yymmdd));
		memcpy(m_TXENCKEY.k_len, strkeylen, sizeof(m_TXENCKEY.k_len));
		memcpy(m_TXENCKEY.k_data, strkeybuff, Asc2Int(strkeylen,5));
		break;
	case WRB_ORG_INT_020:
		memset(m_TXENCKEY.k_r_yymmdd, 0x30, sizeof(m_TXENCKEY.k_r_yymmdd));
		memset(m_TXENCKEY.k_v_yymmdd, 0x30, sizeof(m_TXENCKEY.k_v_yymmdd));
		memcpy(m_TXENCKEY.k_len, "16", strlen("16"));
		memcpy(m_TXENCKEY.k_data, "                ", 16);
		break;
	case DOB_ORG_INT_279:
		fnAPP_KeyInit279((BYTE *)strkeybuff, strkeylen, strkeylen , SEND_F);
		memset(m_TXENCKEY.k_r_yymmdd, 0x30, sizeof(m_TXENCKEY.k_r_yymmdd));
		memset(m_TXENCKEY.k_v_yymmdd, 0x30, sizeof(m_TXENCKEY.k_v_yymmdd));
		memcpy(m_TXENCKEY.k_len, strkeylen, sizeof(m_TXENCKEY.k_len));
		memcpy(m_TXENCKEY.k_data, strkeybuff, Asc2Int(strkeylen,5));
		break;
	default:
		break;
	}

	return T_OK;
}


int CTranCmn::fnAPP_KeyInit004(BYTE * out_b, char *out_i, char *out_v , int out_t)
{
	int		iret;
	char	strtmp[200];
	uint16  out_len;
	uint8   kbstrtmp[1024];
	memset(kbstrtmp, 0x00, sizeof(kbstrtmp));
	switch(out_t)
	{
	case SEND_F:
		iret = KB_Encode_KeyInit_Token (&m_KB_XM_CTX, (uint8 *)out_b, &out_len, sizeof(out_b));
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit004 SEND call KB_Encode_KeyInit_Token iret Error = [%04d]", iret);
		if (iret < 0) return  K_V_F;
		memcpy(kbstrtmp, &m_KB_XM_CTX, sizeof(m_KB_XM_CTX));
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit004 SEND",(char *)kbstrtmp, sizeof(m_KB_XM_CTX) );
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit004 SEND out_len = [%04d]", out_len);		
		sprintf(out_v,"%05d",out_len);
		break;
	case RECV_F:
		memset(strtmp, 0x00, sizeof(strtmp));
		sprintf(strtmp, "%5.5s", out_v);
		if(out_i[0] == 'C')
		{
			iret = KB_Decode_KeyFinal_Token (&m_KB_XM_CTX, out_b, Asc2Int(strtmp,5));
			if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit004 1. RECV call KB_Decode_KeyFinal_Token iret Error = [%04d]", iret);
			if (iret < 0) return  K_V_F;

			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit004 RECV out_v =[%s] : iret = [%04d]",strtmp, iret);		
			sprintf(m_B_E_INFO_INI.m_B_S_C[0].k_s_flag,"%s","01");
			fnAPP_BankKey_Set_Flag(KBS_ORG_INT_006, K_S_F);
			return K_V_F;
		}
		else
		{
			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit004 RECV Key",(char *)out_b, Asc2Int(strtmp,5) );
			iret = KB_Decode_KeyFinal_Token (&m_KB_XM_CTX, out_b, Asc2Int(strtmp,5));
			if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit004 2. RECV call KB_Decode_KeyFinal_Token iret Error = [%04d]", iret);
			if (iret < 0) return  K_V_F;
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit004 RECV iret = [%4d]", iret);		
			memcpy(kbstrtmp, &m_KB_XM_CTX, sizeof(m_KB_XM_CTX));
			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit004 RECV Res",(char *)kbstrtmp, sizeof(m_KB_XM_CTX));
			sprintf(m_B_E_INFO_INI.m_B_S_C[0].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(KBS_ORG_INT_006, K_S_F);
		}
		break;
	default:
		break;

	}
	return C_SUCCESS;
}
int CTranCmn::fnAPP_KeyInit003(BYTE * out_b, char *out_i, char *out_v , int out_t)
{
	int		iret;
	char	strtmp[200];
	uint16  out_len;
	uint8   kiupstrtmp[2048];
	memset(kiupstrtmp, 0x00, sizeof(kiupstrtmp));
	switch(out_t)
	{
	case SEND_F:
		iret = XMC2_Encode_KeyInit_Token_Base64 (&m_KIUP_XM_CTX, (uint8 *)out_b, &out_len, 2048);
		memcpy(kiupstrtmp, &m_KIUP_XM_CTX, sizeof(m_KIUP_XM_CTX));
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit003 SEND",(char *)kiupstrtmp, sizeof(m_KIUP_XM_CTX) );
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit003 SEND out_len = [%04d]", out_len);		
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit003 SEND Call XMC2_Encode_KeyInit_Token iret Error = [%04d]", iret);
		if (iret < 0) return  K_V_F;
		sprintf(out_v,"%05d",out_len);
		break;
	case RECV_F:
		memset(strtmp, 0x00, sizeof(strtmp));
		sprintf(strtmp, "%5.5s", out_v);
		if(out_i[0] == 'C')
		{
			iret = XMC2_Decode_KeyFinal_Token_Base64 (&m_KIUP_XM_CTX, out_b, Asc2Int(strtmp,5));
			if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit003 1. RECV call XMC2_Decode_KeyFinal_Token iret Error = [%04d]", iret);
			if (iret < 0) {
				return  K_V_F;
			}
			sprintf(m_B_E_INFO_INI.m_B_S_C[1].k_s_flag,"%s","01");
			fnAPP_BankKey_Set_Flag(KUB_ORG_INT_003, K_S_F);
		}
		else
		{
			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit003 RECV Key",(char *)out_b, Asc2Int(strtmp,5) );
			iret = XMC2_Decode_KeyFinal_Token_Base64 (&m_KIUP_XM_CTX, out_b,Asc2Int(strtmp,5));
			if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit003 2. RECV call XMC2_Decode_KeyFinal_Token iret Error = [%04d]", iret);
			if (iret < 0) {
				return  K_V_F;
			}	
			memcpy(kiupstrtmp, &m_KIUP_XM_CTX, sizeof(m_KIUP_XM_CTX));
			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit003 RECV Res",(char *)kiupstrtmp, sizeof(m_KIUP_XM_CTX));
			sprintf(m_B_E_INFO_INI.m_B_S_C[1].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(KUB_ORG_INT_003, K_S_F);
		}
		break;
	default:
		break;

	}

	return C_SUCCESS;

}

int CTranCmn::fnAPP_KeyInit011(BYTE * out_b, char *out_i, char *out_v , int out_t)
{
	char chmsg[ISA_MAX_CHELLO_LEN];
	int iret = -1;
	int outl;
	
	switch(out_t)
	{
	case SEND_F:
		ISA_Init();
		if(m_handle_NON) {
			ISA_HANDLE_release(m_handle_NON);
			m_handle_NON=NULL;
		}
		iret = ISA_HANDLE_acquire(&m_handle_NON, SL_FULL_SECURITY, NON_111_CERT);
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit011 SEND call ISA_HANDLE_acquire iret Error = [%06d]", iret);
		if (iret < 0) {
			return  K_V_F;
		}
		iret = ISA_chello(m_handle_NON, chmsg, sizeof(chmsg), &outl, NULL, 0);
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit011 SEND call ISA_chello iret Error = [%06d]", iret);
		if (iret < 0) {
			return  K_V_F;
		}
		memcpy(out_b, chmsg, outl);
		Int2Asc(outl, out_v, 5);

		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit011 SEND KEY", out_b, outl);
		break;
	case RECV_F:		
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit011 RECV len = [%5.5s]", out_v);
		outl = Asc2Int(out_v, 5);
		memcpy(chmsg, out_b, outl);
		iret = ISA_cfinish(m_handle_NON, chmsg, outl);
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit011 RECV call ISA_cfinish iret Error = [%06d]", iret);
		if (iret < 0) {
			return  K_V_F;
		}
		iret = ISA_HANDLE_store_encfile(m_handle_NON, NON_ENCRIPT_DIR,  m_AtmCode, strlen(m_AtmCode));
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit011 RECV call ISA_HANDLE_store_encfile iret Error = [%06d]", iret);
		if (iret < 0) {
			return K_V_F;
		}
		break;
	default:
		break;

	}




	return C_SUCCESS;

}

int CTranCmn::fnAPP_KeyInit023(BYTE * out_b, char *out_i, char *out_v , int out_t)
{
	char chmsg[ISA_MAX_CHELLO_LEN];
	int iret = -1;
	int outl;
	
	switch(out_t)
	{
	case SEND_F:
		SC_ISA_Init();
		if(m_handle_SC) {
			SC_ISA_HANDLE_release(m_handle_SC);
			m_handle_SC=NULL;
		}
		iret = SC_ISA_HANDLE_acquire(&m_handle_SC, SL_FULL_SECURITY, SC_123_CERT);
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit023 SEND call SC_ISA_HANDLE_acquire iret Error = [%06d]", iret);
		if (iret < 0) {
			return  K_V_F;
		}

		iret = SC_ISA_chello(m_handle_SC, chmsg, sizeof(chmsg), &outl, NULL, 0);
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit023 SEND call SC_ISA_chello iret Error = [%06d]", iret);
		if (iret < 0) {
			return  K_V_F;
		}
		memcpy(out_b, chmsg, outl);
		Int2Asc(outl, out_v, 5);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit023 SEND KEY", out_b, outl);
		break;
	case RECV_F:		
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit023 RECV len = [%5.5s]", out_v);
		outl = Asc2Int(out_v, 5);
		memcpy(chmsg, out_b, outl);
		iret = SC_ISA_cfinish(m_handle_SC, chmsg, outl);
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit023 RECV call SC_ISA_cfinish iret Error = [%06d]", iret);
		if (iret < 0) {
			return K_V_F;
		}		iret = SC_ISA_HANDLE_store_encfile(m_handle_SC, SC_ENCRIPT_DIR,  m_AtmCode, strlen(m_AtmCode));
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit023 RECV call SC_ISA_HANDLE_store_encfile iret Error = [%06d]", iret);
		if (iret < 0) {
			return K_V_F;
		}		break;
	default:
		break;

	}

	return C_SUCCESS;

}


//굿모닝신한증권투자증권
int CTranCmn::fnAPP_KeyInit278(BYTE *szOI_dummy, char *szI_ch, char *szOI_len , int nflag)
{

	return C_SUCCESS;

}


//동부증권 
int CTranCmn::fnAPP_KeyInit279(BYTE *szOI_dummy, char *szI_ch, char *szOI_len , int nflag)
{
	int s1Len=0, s2Len=0, iret;
	BYTE s1Data[49+1],s2Data[49+1];
	char szDummy[1024];
/********************************************TEST MASTER KEY ****************************************************************/
	memset(m_279MK_TABLE, 0x00, sizeof(m_279MK_TABLE));
//  Test Key 0728 Gi.jin (NH HYOSUNG)
//	strcpy((char *)m_306MK_TABLE, "675A3B56705705232C60514317D7B2B755DBC713514414B23A09E3C7038823FF61B2A00C140F8CFF61EBB6B5486BA3540935D6002360AAB829F6BBF843A08ABF5FAC6D41504E65A21208E35B6910F7E71012EF5D2E2454B76E5F444B58621A1B077816AF6819306D4DB5865858291BF83597AA2545BB60A06A6A0F111CF1E57C361265C416CA605434C998330BEE2CD7680E75076ED37BFA0F7650D649C11513");
//  Real Key 09.07.28
//	strcpy((char *)m_306MK_TABLE, "2F29C48AE772DCB458964FF7F7C1EFF1B9ACE1428D12449F2A6413220CBD4DE4B91853CE49C191AC250960830F3E2B2563DAD1029CFB4B4119138F1022E1B43826A056F4467E3C03184F38FCA4834FE2B3551AF84A466CD671CA36BD3841A728FC2897A3E79125E0B6D09F08BF78A34F328684CB9FDBEF86B8F0CFD259C56DDCC51ADC8433E2946C89F55D4F67056F9567D3D724A3A21C7878EC23F68A55507D");	

//  Test Key Gi.jin (NH NICE)
//	strcpy((char *)m_306MK_TABLE, "675A3B56705705232C60514317D7B2B755DBC713514414B23A09E3C7038823FF61B2A00C140F8CFF61EBB6B5486BA3540935D6002360AAB829F6BBF843A08ABF5FAC6D41504E65A21208E35B6910F7E71012EF5D2E2454B76E5F444B58621A1B077816AF6819306D4DB5865858291BF83597AA2545BB60A06A6A0F111CF1E57C361265C416CA605434C998330BEE2CD7680E75076ED37BFA0F7650D649C11513");
//  Real Key 
//	strcpy((char *)m_247MK_TABLE, "302AC58BE873DDB5599750F8F8C2F0F2BAADE2438E1345A02B6514230DBE4EE5BA1954CF4AC292AD260A6184103F2C2664DBD2039DFC4C421A14901123E2B53927A157F5477F3D04195039FDA58450E3B4561BF94B476DD772CB37BE3942A829FD2998A4E89226E1B7D1A009C079A450338785CCA0DCF087B9F1D0D35AC66EDDC61BDD8534E3956D8AF65E506806709668D4D825A4A31D7979ED24F78B56517E");	
	
	//*EasyCash*
	strcpy((char *)m_279MK_TABLE, "2923BE84E16CD6AE529049F1F1BBE9EBB3A6DB3C870C3E99245E0D1C06B747DEB3124DC843BB8BA61F035A7D0938251F5DD4CBFC96F5453B130D890A1CDBAE32209A50EE407836FD124932F69E7D49DCAD4F14F2444066D06BC430B7323BA122F622919DE18B1FDAB0CA9902B9729D492C807EC599D5E980B2EAC9CC53BF67D6BF14D67E2DDC8E6683EF574961FF698F61CDD11E9D9C167272E61DF0844F4A77");
								//"2923BE84E16CD6AE529049F1F1BBE9EBB3A6DB3C870C3E99245E0D1C06B747DEB3124DC843BB8BA61F035A7D0938251F5DD4CBFC96F5453B130D890A1CDBAE32209A50EE407836FD124932F69E7D49DCAD4F14F2444066D06BC430B7323BA122F622919DE18B1FDAB0CA9902B9729D492C807EC599D5E980B2EAC9CC53BF67D6BF14D67E2DDC8E6683EF574961FF698F61CDD11E9D9C167272E61DF0844F4A77
/*

	if(Asc2Int(m_pProfile->NETWORK.BpCurPortNo, 6) == 60001)	
		strcpy((char *)m_306MK_TABLE, "302AC58BE873DDB5599750F8F8C2F0F2BAADE2438E1345A02B6514230DBE4EE5BA1954CF4AC292AD260A6184103F2C2664DBD2039DFC4C421A14901123E2B53927A157F5477F3D04195039FDA58450E3B4561BF94B476DD772CB37BE3942A829FD2998A4E89226E1B7D1A009C079A450338785CCA0DCF087B9F1D0D35AC66EDDC61BDD8534E3956D8AF65E506806709668D4D825A4A31D7979ED24F78B56517E");	
	else
		strcpy((char *)m_306MK_TABLE, "675A3B56705705232C60514317D7B2B755DBC713514414B23A09E3C7038823FF61B2A00C140F8CFF61EBB6B5486BA3540935D6002360AAB829F6BBF843A08ABF5FAC6D41504E65A21208E35B6910F7E71012EF5D2E2454B76E5F444B58621A1B077816AF6819306D4DB5865858291BF83597AA2545BB60A06A6A0F111CF1E57C361265C416CA605434C998330BEE2CD7680E75076ED37BFA0F7650D649C11513");
*/

/********************************************TEST MASTER KEY ****************************************************************/
	switch(nflag)
	{
	case SEND_F:
		/* Client가 SessionKey1 생성 */
		memset(s1Data, 0x00, sizeof(s1Data));
		iret = DB_CKeyInit(s1Data, &s1Len, m_279MK_TABLE, m_279W1_KEY, &m_279Wlen); 
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit279 SEND call DB_CKeyInit iret Error = [%06d]", iret);
		if (iret < 0) {
			return  K_V_F;
		}
		memset(szDummy, ' ', sizeof(szDummy));
		memcpy(szDummy, s1Data, s1Len);
		memcpy(szOI_dummy, szDummy, 100);
		Int2Asc(100, szOI_len, 5);
		break;
	case RECV_F:		
		memset(s2Data, 0x00, sizeof(s2Data));
		memcpy(s2Data, szOI_dummy, sizeof(s2Data));
		/* s2Data 전송 (Server --> Client) 임시구현 */
		/* Client가 SessionKey1,SessionKey2 조합하여 WorkingKey 확보 */
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit279 RECV out_len = [%06d]", Asc2Int(szOI_len, 5));
		s2Len = Asc2Int(szOI_len, 5);
		iret = DB_SKeyFinal(s2Data, &s2Len, m_279MK_TABLE, m_279W1_KEY, &m_279Wlen); 
		if(iret != 0)
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit279 RECV call DB_SKeyFinal errCode = [X%01d]", iret);
			return  K_V_F;
		}
		else
		{

			sprintf(m_B_E_INFO_INI.m_B_S_C[10].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(DOB_ORG_INT_279, K_S_F);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[%.46s]", m_279W1_KEY);
			/***************************************************************************/
			/* Client는 활성메모리에 암호화키값 w1_Key(w1Len = 46 Byte)를 보관함       */
			/***************************************************************************/
		}
		break;
	default:
		break;

	}
	return C_SUCCESS;

}

//NH투자증권 2009.07.14 Gi.Jin(우리투자증권)
int CTranCmn::fnAPP_KeyInit247(BYTE *szOI_dummy, char *szI_ch, char *szOI_len , int nflag)
{
	int s1Len=0, s2Len=0, iret;
	BYTE s1Data[46+1],s2Data[46+1];
	char szDummy[1024];
	int nIdx = 0;
/********************************************TEST MASTER KEY ****************************************************************/
	memset(m_247MK_TABLE, 0x00, sizeof(m_247MK_TABLE));
//  Test Key 0728 Gi.jin (NH HYOSUNG)
//	strcpy((char *)m_306MK_TABLE, "675A3B56705705232C60514317D7B2B755DBC713514414B23A09E3C7038823FF61B2A00C140F8CFF61EBB6B5486BA3540935D6002360AAB829F6BBF843A08ABF5FAC6D41504E65A21208E35B6910F7E71012EF5D2E2454B76E5F444B58621A1B077816AF6819306D4DB5865858291BF83597AA2545BB60A06A6A0F111CF1E57C361265C416CA605434C998330BEE2CD7680E75076ED37BFA0F7650D649C11513");
//  Real Key 09.07.28
//	strcpy((char *)m_306MK_TABLE, "2F29C48AE772DCB458964FF7F7C1EFF1B9ACE1428D12449F2A6413220CBD4DE4B91853CE49C191AC250960830F3E2B2563DAD1029CFB4B4119138F1022E1B43826A056F4467E3C03184F38FCA4834FE2B3551AF84A466CD671CA36BD3841A728FC2897A3E79125E0B6D09F08BF78A34F328684CB9FDBEF86B8F0CFD259C56DDCC51ADC8433E2946C89F55D4F67056F9567D3D724A3A21C7878EC23F68A55507D");	

//  Test Key Gi.jin (NH NICE)
//	strcpy((char *)m_306MK_TABLE, "675A3B56705705232C60514317D7B2B755DBC713514414B23A09E3C7038823FF61B2A00C140F8CFF61EBB6B5486BA3540935D6002360AAB829F6BBF843A08ABF5FAC6D41504E65A21208E35B6910F7E71012EF5D2E2454B76E5F444B58621A1B077816AF6819306D4DB5865858291BF83597AA2545BB60A06A6A0F111CF1E57C361265C416CA605434C998330BEE2CD7680E75076ED37BFA0F7650D649C11513");
//  Real Key 

//	strcpy((char *)m_247MK_TABLE, "302AC58BE873DDB5599750F8F8C2F0F2BAADE2438E1345A02B6514230DBE4EE5BA1954CF4AC292AD260A6184103F2C2664DBD2039DFC4C421A14901123E2B53927A157F5477F3D04195039FDA58450E3B4561BF94B476DD772CB37BE3942A829FD2998A4E89226E1B7D1A009C079A450338785CCA0DCF087B9F1D0D35AC66EDDC61BDD8534E3956D8AF65E506806709668D4D825A4A31D7979ED24F78B56517E");	
	
	//*EasyCash*
	strcpy((char *)m_247MK_TABLE, "3C36D197F47FE9C165A35C0404CEFCFEC6B9EE4F9A1F51AC3771202F19CA5AF1C62560DB56CE9EB932166D901C4B383270E7DE0FA908584E26209C1D2FEEC14533AD6301538B4910255C4509B1905CEFC0622705575379E37ED743CA454EB4350935A4B0F49E32EDC3DDAC15CC85B05C3F9391D8ACE8FC93C5FDDCDF66D27AE9D227E99140EFA17996026A5C74127CA274E0E431B0AF298585F9300397625D8A");
/*

	if(Asc2Int(m_pProfile->NETWORK.BpCurPortNo, 6) == 60001)	
		strcpy((char *)m_306MK_TABLE, "302AC58BE873DDB5599750F8F8C2F0F2BAADE2438E1345A02B6514230DBE4EE5BA1954CF4AC292AD260A6184103F2C2664DBD2039DFC4C421A14901123E2B53927A157F5477F3D04195039FDA58450E3B4561BF94B476DD772CB37BE3942A829FD2998A4E89226E1B7D1A009C079A450338785CCA0DCF087B9F1D0D35AC66EDDC61BDD8534E3956D8AF65E506806709668D4D825A4A31D7979ED24F78B56517E");	
	else
		strcpy((char *)m_306MK_TABLE, "675A3B56705705232C60514317D7B2B755DBC713514414B23A09E3C7038823FF61B2A00C140F8CFF61EBB6B5486BA3540935D6002360AAB829F6BBF843A08ABF5FAC6D41504E65A21208E35B6910F7E71012EF5D2E2454B76E5F444B58621A1B077816AF6819306D4DB5865858291BF83597AA2545BB60A06A6A0F111CF1E57C361265C416CA605434C998330BEE2CD7680E75076ED37BFA0F7650D649C11513");
*/

/********************************************TEST MASTER KEY ****************************************************************/
	switch(nflag)
	{
	case SEND_F:
		/* Client가 SessionKey1 생성 */
		memset(s1Data, 0x00, sizeof(s1Data));
		iret = WR_CKeyInit(s1Data, &s1Len, m_247MK_TABLE, m_247W1_KEY, &m_247Wlen); 
		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit247 SEND call WR_CKeyInit iret Error = [%06d]", iret);
		if (iret < 0) {
			return  K_V_F;
		}
		memset(szDummy, ' ', sizeof(szDummy));
		Int2Asc(s1Len, &szDummy[nIdx], 3);
		nIdx += 3;
		memcpy(&szDummy[nIdx], s1Data, s1Len);
		nIdx += s1Len;
		memcpy(szOI_dummy, szDummy, nIdx);
		Int2Asc(nIdx, szOI_len, 5);
		break;
	case RECV_F:		
		memset(s2Data, 0x00, sizeof(s2Data));
		memcpy(s2Data, &szOI_dummy[3], sizeof(s2Data));
		/* s2Data 전송 (Server --> Client) 임시구현 */
		/* Client가 SessionKey1,SessionKey2 조합하여 WorkingKey 확보 */
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit247 RECV call WR_CKeyFinal [%46.46s]", &szOI_dummy[3]);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "out_len = [%04d]", Asc2Int(szOI_len, 5));

		iret = WR_CKeyFinal(s2Data, m_247MK_TABLE, m_247W1_KEY, &m_247Wlen); 
		if(iret != 0)
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit247 RECV call WR_CKeyFinal errCode = [X%01d]", iret);
			return  K_V_F;
		}
		else
		{

			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit247 RECV s2Data[%s], m_247MK_TABLE[%s], m_247Wlen[%d]", s2Data, m_247MK_TABLE, m_247Wlen);
			sprintf(m_B_E_INFO_INI.m_B_S_C[4].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(WRT_ORG_INT_247, K_S_F);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[%.46s]", m_247W1_KEY);
			/***************************************************************************/
			/* Client는 활성메모리에 암호화키값 w1_Key(w1Len = 46 Byte)를 보관함       */
			/***************************************************************************/
		}
		break;
	default:
		break;

	}
	return C_SUCCESS;

}
//한화투자증권 2009.10.23 Gi.Jin
int CTranCmn::fnAPP_KeyInit269(BYTE *szOI_dummy, char *szI_ch, char *szOI_len , int nflag)
{
	int s1Len=0, s2Len=0, iret;
	BYTE s1Data[46+1],s2Data[46+1];
	char szDummy[1024];
	int nIdx = 0;
/********************************************TEST MASTER KEY ****************************************************************/
	memset(m_269MK_TABLE, 0x00, sizeof(m_269MK_TABLE));

	//*EasyCash*
	strcpy((char *)m_269MK_TABLE, "2D27C288E570DAB256944DF5F5BFEDEFB7AADF408B10429D286211200ABB4BE2B71651CC47BF8FAA23075E810D3C292361D8CF009AF9493F17118D0E20DFB236249E54F2447C3A01164D36FAA2814DE0B15318F648446AD46FC834BB363FA526FA2695A1E58F23DEB4CE9D06BD76A14D308482C99DD9ED84B6EECDD057C36BDAC318DA8231E0926A87F35B4D65036D9365D1D522A1A01A7676EA21F488534E7B");

//  Real Key 09.07.28
//	strcpy((char *)m_306MK_TABLE, "2F29C48AE772DCB458964FF7F7C1EFF1B9ACE1428D12449F2A6413220CBD4DE4B91853CE49C191AC250960830F3E2B2563DAD1029CFB4B4119138F1022E1B43826A056F4467E3C03184F38FCA4834FE2B3551AF84A466CD671CA36BD3841A728FC2897A3E79125E0B6D09F08BF78A34F328684CB9FDBEF86B8F0CFD259C56DDCC51ADC8433E2946C89F55D4F67056F9567D3D724A3A21C7878EC23F68A55507D");	
/********************************************TEST MASTER KEY ****************************************************************/
//	strcpy((char *)MK_TABLE, "675A3B56705705232C60514317D7B2B755DBC713514414B23A09E3C7038823FF61B2A00C140F8CFF61EBB6B5486BA3540935D6002360AAB829F6BBF843A08ABF5FAC6D41504E65A21208E35B6910F7E71012EF5D2E2454B76E5F444B58621A1B077816AF6819306D4DB5865858291BF83597AA2545BB60A06A6A0F111CF1E57C361265C416CA605434C998330BEE2CD7680E75076ED37BFA0F7650D649C11513");

	switch(nflag)
	{
	case SEND_F:
		/* Client가 SessionKey1 생성 */
		memset(s1Data, 0x00, sizeof(s1Data));
		iret = HW_CKeyInit(s1Data, &s1Len, m_269MK_TABLE, m_269W1_KEY, &m_269Wlen); 

		if (iret < 0) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit269 SEND call HW_CKeyInit iret Error = [%06d]", iret);
		if (iret < 0) {
			return  K_V_F;
		}

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit269 SEND s1Data[%s], s1Len[%d], m_269MK_TABLE[%s], m_269Wlen[%d]", s1Data, s1Len, m_269MK_TABLE, m_269Wlen);

		memset(szDummy, ' ', sizeof(szDummy));
		Int2Asc(s1Len, &szDummy[nIdx], 3);
		nIdx += 3;
		memcpy(&szDummy[nIdx], s1Data, s1Len);
		nIdx += s1Len;
		memcpy(szOI_dummy, szDummy, nIdx);
		Int2Asc(nIdx, szOI_len, 5);
		break;
	case RECV_F:		
		memset(s2Data, 0x00, sizeof(s2Data));
		memcpy(s2Data, &szOI_dummy[3], sizeof(s2Data));
		/* s2Data 전송 (Server --> Client) 임시구현 */
		/* Client가 SessionKey1,SessionKey2 조합하여 WorkingKey 확보 */
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit269 RECV call HW_CKeyFinal [%46.46s]", &szOI_dummy[3]);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "out_len = [%04d]", Asc2Int(szOI_len, 5));

		iret = HW_CKeyFinal(s2Data, m_269MK_TABLE, m_269W1_KEY, &m_269Wlen); 
		if(iret != 0)
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit269 RECV call HW_CKeyFinal errCode = [X%01d]", iret);
			return  K_V_F;
		}
		else
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit269 RECV s2Data[%s], m_269MK_TABLE[%s], m_269MK_TABLE[%d]", s2Data, m_269MK_TABLE, m_269Wlen);
			sprintf(m_B_E_INFO_INI.m_B_S_C[6].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(HWT_ORG_INT_269, K_S_F);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[%.46s]", m_269MK_TABLE);
			/***************************************************************************/
			/* Client는 활성메모리에 암호화키값 w1_Key(w1Len = 46 Byte)를 보관함       */
			/***************************************************************************/
		}
		break;
	default:
		break;

	}
	return C_SUCCESS;

}

#define MASTER_KEY  "0785852607930784"	// 외환은행에서 제공한 마스터 키
int CTranCmn::fnAPP_KeyInit005(BYTE *szOI_dummy, char *szI_ch, char *szOI_len , int nflag)
{
//	int		iret;
//	char	szTmp[200];
//	int		nlen;
	char    szTmp_dummy[1024];

	int sLen;
	int i;	// 워킹키 생성(난수발생)
	BYTE sendData[23+1]; // 
	BYTE workData[8+1];
	BYTE workDataS[16+1];
	BYTE checkData[8+1];
	BYTE checkDataS[16+1];
	BYTE comp1[8+1];
	BYTE comp2[8+1];
	BYTE workKeyS[16+1];
	BYTE workKey[8+1];

	memset(szTmp_dummy, 0x00, sizeof(szTmp_dummy));
	switch(nflag)
	{
	case SEND_F:


		for(i=0; i<16; i++)
			sprintf((char*)&workKeyS[i], "%X", rand()%16);

		workKeyS[16] = 0x00;

		// WorkingKey를 Server에 전달하기 위해서 MasterKey로 DES암호화
		MakePack(&workKeyS[0], &workKey[0], 16);	// 
		memcpy(&comp1[0], &workKey[0], 8);		// Data Block
		MakePack((BYTE*)MASTER_KEY, &comp2[0], 16);	// Key Block
		

		m_pDevCmn->ENCRYPT(&workData[0], comp1, 8, comp2);

		MakeUnPack(&workData[0], (BYTE *)&workDataS[0], 8);
		sLen = 8*2; 	
		workDataS[sLen] = 0x00;

		// CheckDigit생성
		memset(&comp1[0], 0x00, 8);			// Data Block
		memcpy(&comp2[0], &workKey[0], 8);	// Key Block

		m_pDevCmn->ENCRYPT(&checkData[0], comp1, 8, comp2);
		MakeUnPack(&checkData[0], (BYTE *)&checkDataS[0], 8);
		sLen = 8*2; 
		checkDataS[sLen] = 0x00;

		// Make SendData(인증키)
		memcpy(&sendData[ 0], "020", 3);			// 인증키 길이
		memcpy(&sendData[ 3], &workDataS[0], 16);	// 워킹키 Data
		memcpy(&sendData[19], &checkDataS[0], 4);	// Check Digit
		sendData[23] = 0x00;

		memcpy(szOI_dummy, sendData, 23);
		Int2Asc(23, szOI_len, 5);			
		HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_KebBankKeyInit005_SEND",m_szTranBuffer, strlen(m_szTranBuffer) );
		break;

	case RECV_F:
		break;
	default:
		break;

	}
	return C_SUCCESS;

}



int CTranCmn::fnAPP_KeyInit240(BYTE *szOI_dummy, char *szI_ch, char *szOI_len , int nflag)
{
	int		iret;
	char	szTmp[200];
	int		nlen;
	char    szTmp_dummy[1024];
	memset(szTmp_dummy, 0x00, sizeof(szTmp_dummy));
	switch(nflag)
	{
	case SEND_F:
		if(szI_ch[0] == 'P')
		{
			;
		}
		else
		if(szI_ch[0] == 'S')
		{
			fnAPP_Encrypt_240_Step_Flow(m_szTranBuffer, &usClientDecData);
			memcpy(szOI_dummy, m_szTranBuffer, strlen(m_szTranBuffer));
			Int2Asc(strlen(m_szTranBuffer), szOI_len, 5);			
		}		
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit240_SEND",m_szTranBuffer, strlen(m_szTranBuffer) );
		break;
	case RECV_F:
		memset(szTmp, 0x00, sizeof(szTmp));
		sprintf(szTmp, "%5.5s", szOI_len);
		nlen = Asc2Int(szTmp, 5);
		memcpy(m_szTranBuffer, szOI_dummy, nlen);
		if(szI_ch[0] == 'P')
		{
			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit240_1 RECV",m_szTranBuffer, strlen(m_szTranBuffer) );
			iret = fnAPP_Encrypt_240_Step_Flow(m_szTranBuffer, &usClientDecData);

			if(iret !=0) 
				return K_V_F;

			sprintf(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,"%s","03");
			fnAPP_BankKey_Set_Flag(SST_ORG_INT_CVT_240, K_S_F);			
		}
		else
		if(szI_ch[0] == 'S')
		{
			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInit240_2 RECV",m_szTranBuffer, strlen(m_szTranBuffer) );
			iret = fnAPP_Encrypt_240_Step_Flow(m_szTranBuffer, &usClientDecData);

			if(iret !=0) 
				return K_V_F;

			sprintf(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(SST_ORG_INT_CVT_240, K_S_F);
		}
		break;
	default:
		break;

	}
	return C_SUCCESS;

}

int	fnAPP_Encrypt_240_Step_Flow( char *_pszTransBuffer, UString *_pusDecData )
{
	int nRet = 0, i = 0;
    int nType = _pszTransBuffer[0];
    char *pszMasterKey = "vK9GCbVdV4Mer5pmPKEdxIh3RGztbQdDo7+m6HhGNGWLDf6r+9Hr0P20g51xyL1i";

    switch ( nType )
    {
    case TN_TCRESPONSE_ENCPUBKEY:
        {
            UString usEncTNPubKey, usEncTNKey;
            TN_TCInitUString( &usEncTNPubKey );
            TN_TCInitUString( &usEncTNKey );
            
            nRet = TN_TCConvertTransBuffer2UString( _pszTransBuffer, 400, &usEncTNPubKey );
            if ( nRet != 0 )
            {
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Encrypt_240_Step_Flow_1 = [%d]", nRet); /*에러처리*/		
                break;
            }

            nRet = TN_TCClientKeyShareInit( pszMasterKey, &usEncTNPubKey, &usEncTNKey );
            if ( nRet != 0 )
            {
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Encrypt_240_Step_Flow_2 = [%d]", nRet); /*에러처리*/		
            }
            else
            {
                memset( _pszTransBuffer, 0x00, 4000 );
                nRet = TN_TCConvertUString2TransBuffer( TN_TCSEND_ENCTNKEY, &usEncTNKey, 400, _pszTransBuffer );
                if ( nRet != 0 )
                {
					MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Encrypt_240_Step_Flow_3 = [%d]", nRet); /*에러처리*/		
	            }
            }
            
            TN_TCDeleteMemoryUC( &usEncTNPubKey );
            TN_TCDeleteMemoryUC( &usEncTNKey );

            break;
        }

    case TN_TCSEND_ENCTNNONCE:
        {
            UString usEncTNNonce;
            TN_TCInitUString( &usEncTNNonce );
            
            nRet = TN_TCConvertTransBuffer2UString( _pszTransBuffer, 400, &usEncTNNonce );
            if ( nRet != 0 )
            {
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Encrypt_240_Step_Flow_4 = [%d]", nRet); /*에러처리*/		
                break;
            }
            
            nRet = TN_TCClientKeyShareFinal( &usEncTNNonce );
            if ( nRet != 0 )
            {
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Encrypt_240_Step_Flow_5 = [%d]", nRet); /*에러처리*/		
            }

            TN_TCDeleteMemoryUC( &usEncTNNonce );
            
            break;
        }

    case TN_TCENCDATA:
        {
            UString usEncTData;
            TN_TCInitUString( &usEncTData );
            
            nRet = TN_TCConvertTransBuffer2UString( _pszTransBuffer, 100, &usEncTData );
            if ( nRet != 0 )
            {
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Encrypt_240_Step_Flow_6 = [%d]", nRet); /*에러처리*/		
                break;
            }
            
            nRet = TN_TCClientDecipher( &usEncTData, _pusDecData );
            if ( nRet != 0 )
            {
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Encrypt_240_Step_Flow_7 = [%d]", nRet); /*에러처리*/		
            }
            
            TN_TCDeleteMemoryUC( &usEncTData );
            
            break;
        }
    }

    return nRet;
}


int CTranCmn::fnAPP_KeyInitStartRecv(int bc, int bc_type, int nEncFlag)
{
	int iret = 0;

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_KeyInitStartRecv(%03d) = nEncFlag (%d)", bc, nEncFlag);
	
	switch(bc)
	{
	case KBS_ORG_INT_004:
	case KBS_ORG_INT_006:
	case KBS_ORG_INT_029:
	case KBS_ORG_INT_030:
		if(bc_type == K_VERIFY_C)
		{
			memcpy(m_B_E_INFO_INI.m_B_S_C[0].k_r_yymmdd,  m_RXENCKEY.k_r_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			memcpy(m_B_E_INFO_INI.m_B_S_C[0].k_v_yymmdd,  m_RXENCKEY.k_v_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			int nFlag = Asc2Int(m_RXENCKEY.k_len, sizeof(m_RXENCKEY.k_len));	
			if(nFlag < 500)
			{
				m_RXENCKEY.k_apart[0] = 'S';
			}
			else
			{
				m_RXENCKEY.k_apart[0] = 'C';
			}
			if(fnAPP_KeyInit004(m_RXENCKEY.k_data, (char *)m_RXENCKEY.k_apart, (char *)m_RXENCKEY.k_len, RECV_F ) != C_SUCCESS)
			{
				return K_V_F;
			}
			fnAPP_BankKey_Set_Flag(bc, K_R);
			fnAPP_BankKey_Set_Flag(bc, K_V);
		}
		break;
	case KUB_ORG_INT_003:
		if(bc_type == K_VERIFY_C)
		{
			memcpy(m_B_E_INFO_INI.m_B_S_C[1].k_r_yymmdd,  m_RXENCKEY.k_r_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			memcpy(m_B_E_INFO_INI.m_B_S_C[1].k_v_yymmdd,  m_RXENCKEY.k_v_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			int nFlag = Asc2Int(m_RXENCKEY.k_len, sizeof(m_RXENCKEY.k_len));	
			if(nFlag < 500)
			{
				m_RXENCKEY.k_apart[0] = 'S';
			}
			else
			{
				m_RXENCKEY.k_apart[0] = 'C';
			}
			if(fnAPP_KeyInit003(m_RXENCKEY.k_data, (char *)m_RXENCKEY.k_apart, (char *)m_RXENCKEY.k_len, RECV_F  ) != C_SUCCESS)
			{
				return K_V_F;
			}
			fnAPP_BankKey_Set_Flag(bc, K_R);
			fnAPP_BankKey_Set_Flag(bc, K_V);
		}
		break;
	case NON_ORG_INT_011:
	case NON_ORG_INT_012:
	case NON_ORG_INT_016:
	case NON_ORG_INT_017:
		if(bc_type == K_VERIFY_C)
		{
			memcpy(m_B_E_INFO_INI.m_B_S_C[2].k_r_yymmdd,  m_RXENCKEY.k_r_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			memcpy(m_B_E_INFO_INI.m_B_S_C[2].k_v_yymmdd,  m_RXENCKEY.k_v_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			iret = fnAPP_KeyInit011(m_RXENCKEY.k_data, (char *)m_RXENCKEY.k_apart, (char *)m_RXENCKEY.k_len, RECV_F );
			if(iret == K_V_F)
			{
				return K_V_F;
			}
			fnAPP_BankKey_Set_Flag(bc, K_R);
			fnAPP_BankKey_Set_Flag(bc, K_V);
		}
		break;
	case SCO_ORG_INT_023:
		if(bc_type == K_VERIFY_C)
		{
			memcpy(m_B_E_INFO_INI.m_B_S_C[3].k_r_yymmdd,  m_RXENCKEY.k_r_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			memcpy(m_B_E_INFO_INI.m_B_S_C[3].k_v_yymmdd,  m_RXENCKEY.k_v_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			iret = fnAPP_KeyInit023(m_RXENCKEY.k_data, (char *)m_RXENCKEY.k_apart, (char *)m_RXENCKEY.k_len, RECV_F );
			if(iret == K_V_F)
			{
				return K_V_F;
			}
			fnAPP_BankKey_Set_Flag(bc, K_R);
			fnAPP_BankKey_Set_Flag(bc, K_V);
		}
		break;
	case WRT_ORG_INT_247:
		if(bc_type == K_VERIFY_C)
		{
			memcpy(m_B_E_INFO_INI.m_B_S_C[4].k_r_yymmdd, m_RXENCKEY.k_r_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			memcpy(m_B_E_INFO_INI.m_B_S_C[4].k_v_yymmdd, m_RXENCKEY.k_v_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			if(fnAPP_KeyInit247(m_RXENCKEY.k_data, (char *)m_RXENCKEY.k_apart, (char *)m_RXENCKEY.k_len, RECV_F ) != C_SUCCESS)
			{
				return K_V_F;
			}
			fnAPP_BankKey_Set_Flag(bc, K_R);
			fnAPP_BankKey_Set_Flag(bc, K_V);
		}
		break;
	case SST_ORG_INT_CVT_240:
	case SST_ORG_INT_240:
		if(bc_type == K_VERIFY_C)
		{

			if(nEncFlag == 0) 
			{
				memcpy(m_RXENCKEY.k_apart, "P", sizeof(m_RXENCKEY.k_apart));
			}
			else
			if(nEncFlag == 1)
			{
				memcpy(m_RXENCKEY.k_apart, "S", sizeof(m_RXENCKEY.k_apart));
			}

			memcpy(m_B_E_INFO_INI.m_B_S_C[5].k_r_yymmdd, m_RXENCKEY.k_r_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			memcpy(m_B_E_INFO_INI.m_B_S_C[5].k_v_yymmdd, m_RXENCKEY.k_v_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			if(fnAPP_KeyInit240(m_RXENCKEY.k_data, (char *)m_RXENCKEY.k_apart, (char *)m_RXENCKEY.k_len, RECV_F ) != C_SUCCESS)
			{
				return K_V_F;
			}
			fnAPP_BankKey_Set_Flag(bc, K_R);
			fnAPP_BankKey_Set_Flag(bc, K_V);
		}
		break;
	case HWT_ORG_INT_269:
		if(bc_type == K_VERIFY_C)
		{
			memcpy(m_B_E_INFO_INI.m_B_S_C[6].k_r_yymmdd, m_RXENCKEY.k_r_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			memcpy(m_B_E_INFO_INI.m_B_S_C[6].k_v_yymmdd, m_RXENCKEY.k_v_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			if(fnAPP_KeyInit269(m_RXENCKEY.k_data, (char *)m_RXENCKEY.k_apart, (char *)m_RXENCKEY.k_len, RECV_F ) != C_SUCCESS)
			{
				return K_V_F;
			}
			fnAPP_BankKey_Set_Flag(bc, K_R);
			fnAPP_BankKey_Set_Flag(bc, K_V);
		}
		break;
	case KJB_ORG_INT_034:  //8
		if(bc_type == K_VERIFY_C)
		{
			char strUnPack[200];
			char szTmp[200];
			
			memcpy(m_B_E_INFO_INI.m_B_S_C[7].k_r_yymmdd, m_RXENCKEY.k_r_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			memcpy(m_B_E_INFO_INI.m_B_S_C[7].k_v_yymmdd, m_RXENCKEY.k_v_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			

			sprintf(szTmp, "%16.16s", m_RXENCKEY.k_data);
			sprintf(strUnPack,"%s", MakeUnPack(szTmp, strlen(szTmp)));	
			IniSetStr(_ATMKEYTABELE_INI, KJB_ORG_SEC, "MASTER", strUnPack);
			sprintf(szTmp, "%2.2s", m_RXENCKEY.k_data_sub);
			IniSetStr(_ATMKEYTABELE_INI, KJB_ORG_SEC, "SLAVE0", szTmp);

			sprintf(m_B_E_INFO_INI.m_B_S_C[7].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(KJB_ORG_INT_034, K_S_F);

			fnAPP_BankKey_Set_Flag(bc, K_R);
			fnAPP_BankKey_Set_Flag(bc, K_V);
		}
		break;


	case KEB_ORG_INT_005://9 외환
		if(bc_type == K_VERIFY_C)
		{
			char strUnPack[200];
			char szTmp[200];
			
			memcpy(m_B_E_INFO_INI.m_B_S_C[8].k_r_yymmdd, m_RXENCKEY.k_r_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			memcpy(m_B_E_INFO_INI.m_B_S_C[8].k_v_yymmdd, m_RXENCKEY.k_v_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			

			sprintf(szTmp, "%16.16s", m_RXENCKEY.k_data);
			sprintf(strUnPack,"%s", MakeUnPack(szTmp, strlen(szTmp)));	
			IniSetStr(_ATMKEYTABELE_INI, KEB_ORG_SEC, "MASTER", strUnPack);
			sprintf(szTmp, "%4.4s", m_RXENCKEY.k_data_sub);
			IniSetStr(_ATMKEYTABELE_INI, KEB_ORG_SEC, "SLAVE0", szTmp);

			sprintf(m_B_E_INFO_INI.m_B_S_C[8].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(KEB_ORG_INT_005, K_S_F);

			fnAPP_BankKey_Set_Flag(bc, K_R);
			fnAPP_BankKey_Set_Flag(bc, K_V);
		}

		break;
	case WRB_ORG_INT_020://10 우리
		if(bc_type == K_VERIFY_C)
		{
			char strUnPack[200];
			char szTmp[200];
			
			memcpy(m_B_E_INFO_INI.m_B_S_C[9].k_r_yymmdd, m_RXENCKEY.k_r_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			memcpy(m_B_E_INFO_INI.m_B_S_C[9].k_v_yymmdd, m_RXENCKEY.k_v_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			

			sprintf(szTmp, "%16.16s", m_RXENCKEY.k_data);
			sprintf(strUnPack,"%s", MakeUnPack(szTmp, strlen(szTmp)));	
			IniSetStr(_ATMKEYTABELE_INI, WRB_ORG_SEC, "MASTER", strUnPack);
			sprintf(szTmp, "%2.2s", m_RXENCKEY.k_data_sub);
			IniSetStr(_ATMKEYTABELE_INI, WRB_ORG_SEC, "SLAVE0", szTmp);

			sprintf(m_B_E_INFO_INI.m_B_S_C[9].k_s_flag,"%s","02");
			fnAPP_BankKey_Set_Flag(WRB_ORG_INT_020, K_S_F);

			fnAPP_BankKey_Set_Flag(bc, K_R);
			fnAPP_BankKey_Set_Flag(bc, K_V);
		}


		break;



	case DOB_ORG_INT_279: //11 동부
		if(bc_type == K_VERIFY_C)
		{
			memcpy(m_B_E_INFO_INI.m_B_S_C[10].k_r_yymmdd, m_RXENCKEY.k_r_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			memcpy(m_B_E_INFO_INI.m_B_S_C[10].k_v_yymmdd, m_RXENCKEY.k_v_yymmdd, sizeof(m_RXENCKEY.k_r_yymmdd));
			if(fnAPP_KeyInit279(m_RXENCKEY.k_data, (char *)m_RXENCKEY.k_apart, (char *)m_RXENCKEY.k_len, RECV_F ) != C_SUCCESS)
			{
				return K_V_F;
			}
			fnAPP_BankKey_Set_Flag(bc, K_R);
			fnAPP_BankKey_Set_Flag(bc, K_V);
		}
		break;

	default:
		break;
	}


	return C_SUCCESS;

}



// 암호화 방법 (거래)
int CTranCmn::fnAPP_EncType(int nTranCode)
{
	int  nEncType =0;
	int  nOCarnBank = 0;
	int  nOEncBank = 0;
	char szEntry[1024];
	char szSection[1024];
	CString strReturn("");

	if (nTranCode == TRANID_A010 || nTranCode == TRANID_A020 || nTranCode == TRANID_A040 || nTranCode == TRANID_A060 || nTranCode == TRANID_2190)
		return nEncType;

	if (!(MenuAvail & (MENU_MCU2 | MENU_MCU3)))
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType Except1(%02d)", nEncType);
		return nEncType;
	}
	else
	if (!CardBank || CardBank == OTHBANK)
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType Except2(%02d)", nEncType);
		return nEncType;
	}

	int nIn = FALSE;
	for (int i = 0; i < itemof(m_F031X.F031XLIST); i++)
	{
		nOCarnBank = Asc2Int(m_F031X.F031XLIST[i].OrgNo, 3);
		nOEncBank  = fnAPP_Get_BankOrgEncInt(CardBank, nTranCode);
		if (nOCarnBank == nOEncBank)
		{
			nIn = TRUE;
			break;
		}
	}

	if ((MenuAvail & MENU_MCU3) && (!nIn))
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType:F031XLIST Idx(%03d) - CardBank[%03d] PUBLIC[%03d]" , nOCarnBank, CardBank, m_pProfile->TRANS.TranService);
		nEncType = m_pProfile->TRANS.TranService;
		return nEncType;
	}
	else
	if ((MenuAvail & MENU_MCU3) && (!nOEncBank))
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType:F031XLIST Idx(%03d) - CardBank[%03d] PUBLIC[%03d]" , nOCarnBank, CardBank, m_pProfile->TRANS.TranService);
		nEncType = m_pProfile->TRANS.TranService;
		return nEncType;
	}
	else
	if ((MenuAvail & MENU_MCU2) && (nOEncBank == 0))
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType:F031XLIST Idx(%03d) - CardBank[%03d] ETC[%03d]" , nOCarnBank, CardBank, nOEncBank);
		nEncType = nOEncBank;
		return nEncType;
	}

	CString cstCardInfo("000");
	CString cstAcceptInfo("000");
	switch(nTranCode)
	{
	case TRANID_7260:
		//대신증권
		if (fnAPP_Get_BankOrgCodeInt(CardBank) == nEnCmd267_267)
		{
			cstAcceptInfo = fnAPP_Get_BankOrgCode(Accept.BankNum);
			cstCardInfo = fnAPP_Get_BankOrgCode(CardBank);

			if (cstAcceptInfo == cstCardInfo)	
			{
				//#CS0004
				strReturn = DASSBANKID;
				sprintf(szEntry, "%s|4370", cstCardInfo);
				IniSetStr(_ATMTRCODEENCRYPT_INI, DASSBANKID, szEntry, DASSBANKID);
			}
			else
			{
				//#CS0004
				strReturn = HYDSBANKID_PUBLIC;
				sprintf(szEntry, "%s|4390", cstCardInfo);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szEntry, HYDSBANKID_PUBLIC);
			}
			nEncType = Asc2Int(strReturn.GetBuffer(0), strReturn.GetLength());
		}
		else
		{
			sprintf(szEntry, "%03d", fnAPP_Get_BankOrgCodeInt(CardBank));
			sprintf(szSection, "%3.3s|%04d",szEntry, nTranCode);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType(%s %s)", szEntry, szSection);
			strReturn = IniGetStr(_ATMTRCODEENCRYPT_INI, szEntry, szSection, "000");
			nEncType = Asc2Int(strReturn.GetBuffer(0), strReturn.GetLength());	
		}
		break;
	case TRANID_4390:
	case TRANID_4370:
		sprintf(szEntry, "%03d", fnAPP_Get_BankOrgCodeInt(CardBank));
		sprintf(szSection, "%3.3s|%04d",szEntry, nTranCode);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType(%s %s)", szEntry, szSection);
		strReturn = IniGetStr(_ATMTRCODEENCRYPT_INI, szEntry, szSection, "000");
		nEncType = Asc2Int(strReturn.GetBuffer(0), strReturn.GetLength());		
		break;
	case TRANID_4310: //출금,조회,이체수취,이체,입금가능,입금
	case TRANID_7220:
	case TRANID_4410:
	case TRANID_7200:
	case TRANID_7210:
		sprintf(szEntry, "%03d", fnAPP_Get_BankOrgCodeInt(CardBank));
		sprintf(szSection, "%3.3s|%04d",szEntry, nTranCode);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType(%s %s)", szEntry, szSection);
		strReturn = IniGetStr(_ATMTRCODEENCRYPT_INI, szEntry, szSection, "000");
		nEncType = Asc2Int(strReturn.GetBuffer(0), strReturn.GetLength());	
		break;
	case TRANID_4210: //통장-출금   
	case TRANID_7261: //통장-수취
	case TRANID_4215: //통장-이체
	case TRANID_7126: //통장-입금가능    
	case TRANID_4720: //통장-입금
	case TRANID_4730: //통장정리
	case TRANID_7330: //통장정리 (조회)
		sprintf(szEntry, "%03d", fnAPP_Get_BankOrgCodeInt(PbBank));
		sprintf(szSection, "%3.3s|%04d",szEntry, nTranCode);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType(%s %s)", szEntry, szSection);
		strReturn = IniGetStr(_ATMTRCODEENCRYPT_INI, szEntry, szSection, "000");
		nEncType = Asc2Int(strReturn.GetBuffer(0), strReturn.GetLength());	
		break;
	case TRANID_4330: //서비스-출금,조회,이체수취,이체 
	case TRANID_7230:
	case TRANID_7250:
	case TRANID_4350:
	case TRANID_7380:
	case TRANID_7530: //이자율조회(해외)
		sprintf(szEntry, "%03d", fnAPP_Get_BankOrgCodeInt(CardBank));
		sprintf(szSection, "%3.3s|%04d",szEntry, nTranCode);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType(%s %s)", szEntry, szSection);
		strReturn = IniGetStr(_ATMTRCODEENCRYPT_INI, szEntry, szSection, "000");
		nEncType = Asc2Int(strReturn.GetBuffer(0), strReturn.GetLength());	
		break;
	case TRANID_4320: //취소-출금,서비스,입금
	case TRANID_4340:
		sprintf(szEntry, "%03d", fnAPP_Get_BankOrgCodeInt(CardBank));
		sprintf(szSection, "%3.3s|%04d",szEntry, nTranCode);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType(%s %s)", szEntry, szSection);
		strReturn = IniGetStr(_ATMTRCODEENCRYPT_INI, szEntry, szSection, "000");
		nEncType = Asc2Int(strReturn.GetBuffer(0), strReturn.GetLength());	
		break;
	default:
		break;
	}


	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_EncType Return(%02d)",nEncType);
	return nEncType;
}

// 암호화 방법 (거래)
int CTranCmn::fnAPP_DecType(int nTranCode)
{

	int  nDecType =0;
	int  nOCarnBank = 0;
	int  nOEncBank = 0;
	char szEntry[1024];
	char szSection[1024];
	CString strReturn("");

	if (nTranCode == TRANID_A010 || nTranCode == TRANID_A020 || nTranCode == TRANID_A040 || nTranCode == TRANID_A060 || nTranCode == TRANID_2190)
		return nDecType;

	if (!(MenuAvail & (MENU_MCU2 | MENU_MCU3)))
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_DecType Except1(%02d)", nDecType);
		return nDecType;
	}
	else
	if (!CardBank || CardBank == OTHBANK)
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_DecType Except2(%02d)", nDecType);
		return nDecType;
	}


	int nIn = FALSE;
	for (int i = 0; i < itemof(m_F031X.F031XLIST); i++)
	{
		nOCarnBank = Asc2Int(m_F031X.F031XLIST[i].OrgNo, 3);
		nOEncBank  = fnAPP_Get_BankOrgEncInt(CardBank, nTranCode);
		if (nOCarnBank == nOEncBank)
		{
			nIn = TRUE;
			break;
		}
	}

	if ((MenuAvail & MENU_MCU3) && (nIn == 0))
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_DecType:F031XLIST Idx(%03d) - CardBank[%03d] PUBLIC[%03d]" , nOCarnBank, CardBank, m_pProfile->TRANS.TranService);
		nDecType = m_pProfile->TRANS.TranService;
		return nDecType;
	}
	else
	if ((MenuAvail & MENU_MCU3) && (nOEncBank == 0))
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_DecType:F031XLIST Idx(%03d) - CardBank[%03d] PUBLIC[%03d]" , nOCarnBank, CardBank, m_pProfile->TRANS.TranService);
		nDecType = m_pProfile->TRANS.TranService;
		return nDecType;
	}
	else
	if ((MenuAvail & MENU_MCU2) && (nOEncBank == 0))
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_DecType:F031XLIST Idx(%03d) - CardBank[%03d] ETC[%03d]" , nOCarnBank, CardBank, nOEncBank);
		nDecType = nOEncBank;
		return nDecType;
	}


	switch(nTranCode)
	{
	case TRANID_4310: //출금,조회,이체수취,이체,입금가능,입금
	case TRANID_7220:
	case TRANID_7260:
	case TRANID_4390:
	case TRANID_4370:
	case TRANID_4410:
	case TRANID_4713:
	case TRANID_7200:
	case TRANID_7210:
		sprintf(szEntry, "%03d", fnAPP_Get_BankOrgCodeInt(CardBank));
		sprintf(szSection, "%3.3s|%04d",szEntry, nTranCode);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_DecType(%s %s)", szEntry, szSection);
		strReturn = IniGetStr(_ATMTRCODEENCRYPT_INI, szEntry, szSection, "000");
		nDecType = Asc2Int(strReturn.GetBuffer(0), strReturn.GetLength());	
		break;
	case TRANID_7101: //통장-수수료조회
	case TRANID_4210: //통장-출금   
	case TRANID_7261: //통장-수취
	case TRANID_4215: //통장-이체
	case TRANID_7126: //통장-입금가능    
	case TRANID_4720: //통장-입금
	case TRANID_4730: //통장정리
	case TRANID_7330: //통장정리 (조회)
		sprintf(szEntry, "%03d", fnAPP_Get_BankOrgCodeInt(PbBank));
		sprintf(szSection, "%3.3s|%04d",szEntry, nTranCode);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_DecType(%s %s)", szEntry, szSection);
		strReturn = IniGetStr(_ATMTRCODEENCRYPT_INI, szEntry, szSection, "000");
		nDecType = Asc2Int(strReturn.GetBuffer(0), strReturn.GetLength());	
		break;
	case TRANID_4330: //서비스-출금,조회,이체수취,이체 
	case TRANID_7230:
	case TRANID_7250:
	case TRANID_4350:
		break;
	case TRANID_4320: //취소-출금,서비스,입금
	case TRANID_4340:
		break;
	default:
		break;
	}

	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_DecType Return(%02d)", nDecType);
	return nDecType;
}


int CTranCmn::fnAPP_EncCall(int nEncType, int nTranCode, int nEncrypt)
{
	if(nEncType > 999) return T_OK;
	if(nEncType < 0) return T_OK;
	int nEnc =0;

	switch(nEncType)
	{
	case nEnCmd000_0:		
	case nEnCmd001_1:		
	case nEnCmd002_2:		
		fnAPP_TranEnc000_000(nTranCode);
		break;
	case nEnCmd003_3:		
		fnAPP_TranEnc003_003(nTranCode);
		break;
	case nEnCmd005_5:		
		fnAPP_TranEnc005_005(nTranCode);
		break;
	case nEnCmd004_6:	
		fnAPP_TranEnc004_006(nTranCode);
		break;
	case nEnCmd007_7:		
		fnAPP_TranEnc007_007(nTranCode);
		break;
	case nEnCmd008_8:		
		fnAPP_TranEnc000_000(nTranCode);
		break;
	case nEnCmd011_11:	
// 		nEnc = Asc2Int(m_B_E_INFO_INI.m_B_S_C[2].k_s_flag,2);
// 		if(nEnc != ENABLE)
// 		{
// 			fnAPP_TranEnc011_011(nTranCode);
// 		}
		fnAPP_TranEnc000_000(nTranCode);
		break;
	case nEnCmd012_12:	
//		fnAPP_TranEnc012_012(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd020_20:	
		fnAPP_TranEnc020_020(nTranCode);
		break;
	case nEnCmd023_23:	
// 		nEnc = Asc2Int(m_B_E_INFO_INI.m_B_S_C[3].k_s_flag,2);
// 		if(nEnc != ENABLE)
// 		{
// 			fnAPP_TranEnc023_023(nTranCode);
// 		}
		fnAPP_TranEnc000_000(nTranCode);
		break;
	case nEnCmd031_31:	
//		fnAPP_TranEnc031_031(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd032_32:	
		fnAPP_TranEnc032_032(nTranCode);
		break;
	case nEnCmd034_34:	
		fnAPP_TranEnc034_034(nTranCode);
		break;
	case nEnCmd035_35:	
		fnAPP_TranEnc000_000(nTranCode);
		break;
	case nEnCmd037_37:	
		fnAPP_TranEnc037_037(nTranCode);
		break;
	case nEnCmd039_39:	
		//fnAPP_TranEnc039_039(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd045_45:	
		//fnAPP_TranEnc045_045(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd048_48:	
		fnAPP_TranEnc048_048(nTranCode);
		break;
	case nEnCmd050_50:	
		//fnAPP_TranEnc050_050(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd027_27:	
		fnAPP_TranEnc053_053(nTranCode);
		break;
	case nEnCmd054_54:	
		//fnAPP_TranEnc054_054(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd055_55:	
		fnAPP_TranEnc000_000(nTranCode);
		break;
	case nEnCmd056_56:	
		//fnAPP_TranEnc056_056(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd071_71:	
		fnAPP_TranEnc071_071(nTranCode);
		break;
	case nEnCmd025_81:	
		fnAPP_TranEnc025_081(nTranCode);
		break;
	case nEnCmd021_88:	
		fnAPP_TranEnc021_088(nTranCode);
		break;
	case nEnCmd105_105:
		fnAPP_TranEnc105_105(nTranCode);
		break;
	case nEnCmd105_115:
		fnAPP_TranEnc105_115(nTranCode);
		break;
	case nEnCmd106_106:
		fnAPP_TranEnc106_106(nTranCode);
		break;
	case nEnCmd107_107:
		fnAPP_TranEnc107_107(nTranCode);
		break;
	case nEnCmd111_111:
		fnAPP_TranEnc111_111(nTranCode);
		break;
	case nEnCmd120_120:
		fnAPP_TranEnc120_120(nTranCode);
		break;
	case nEnCmd121_121:
		fnAPP_TranEnc121_121(nTranCode);
		break;
	case nEnCmd126_192:
		fnAPP_TranEnc126_192(nTranCode);
		break;
	case nEnCmd127_127:
		fnAPP_TranEnc127_127(nTranCode);
		break;
	case nEnCmd134_134:
		fnAPP_TranEnc134_034(nTranCode);
		break;
	case nEnCmd135_135:
		fnAPP_TranEnc000_000(nTranCode);
		break;
	case nEnCmd137_137:
		fnAPP_TranEnc137_037(nTranCode);
		break;
	case nEnCmd184_184:
		fnAPP_TranEnc184_184(nTranCode);
		break;
	case nEnCmd193_193:
		fnAPP_TranEnc193_193(nTranCode);
		break;
	case nEnCmd194_194:
		fnAPP_TranEnc194_194(nTranCode);
		break;
	case nEnCmd195_195:
		fnAPP_TranEnc195_195(nTranCode);
		break;
	case nEnCmd209_379_PUBLIC:
	case nEnCmd209_369:
		fnAPP_TranEnc209_209(nTranCode);
		break;
	case nEnCmd218_218:
		fnAPP_TranEnc218_218(nTranCode);
		break;
	case nEnCmd230_230:
//		fnAPP_TranEnc230_230(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd238_368:
		fnAPP_TranEnc238_238(nTranCode);
		break;
	case nEnCmd240_364:
		fnAPP_TranEnc240_240(nTranCode);
		break;
	case nEnCmd243_243:
		fnAPP_TranEnc243_243(nTranCode);
		break;
	case nEnCmd247_247:
		fnAPP_TranEnc247_247(nTranCode);
		break;
	case nEnCmd261_261:
//		fnAPP_TranEnc261_261(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd262_262:
//		fnAPP_TranEnc262_262(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd263_263:
//		fnAPP_TranEnc263_263(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd264_264:
//		fnAPP_TranEnc264_264(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd265_265:
//		fnAPP_TranEnc265_265(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd266_266:
//		fnAPP_TranEnc266_266(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd267_267:
		fnAPP_TranEnc267_267(nTranCode);
		break;
	case nEnCmd268_268:
//		fnAPP_TranEnc268_268(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd269_269:
		fnAPP_TranEnc269_269(nTranCode);
		break;
	case nEnCmd270_270:
		fnAPP_TranEnc270_270(nTranCode);
		break;
	case nEnCmd278_278:
		fnAPP_TranEnc278_278(nTranCode);
		break;
	case nEnCmd279_279:
		fnAPP_TranEnc279_279(nTranCode);
		break;
	case nEnCmd280_280:
//		fnAPP_TranEnc280_280(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd287_287:
//		fnAPP_TranEnc287_287(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd289_289:
//		fnAPP_TranEnc289_289(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd290_290:
//		fnAPP_TranEnc290_290(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd291_291:
//		fnAPP_TranEnc291_291(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd292_292:
//		fnAPP_TranEnc292_292(nTranCode);
		fnAPP_TranEnc000_000();
		break;
	case nEnCmd294_294_BC:
//		fnAPP_TranEnc292_292(nTranCode);
		fnAPP_TranEnc294_294(nTranCode);
		break;
//	case nEnCmd384_364:
	case nEnCmd484_484:
	case nEnCmd601_601:
	case nEnCmd701_701:
	case nEnCmd702_702:
	case nEnCmd703_703:
		fnAPP_TranEnc000_000(nTranCode);
		break;
	default:
		fnAPP_TranEnc000_000(nTranCode);
		break;
	}
	

	return T_OK;
	
}

int CTranCmn::fnAPP_DecCall(int nEncType, int nTranCode, int nEncrypt)
{
	if(nEncType > 999) return T_OK;
	if(nEncType < 0) return T_OK;
	int nEnc =0;

	switch(nEncType)
	{
	case nEnCmd000_0:		
	case nEnCmd001_1:		
	case nEnCmd002_2:		
		fnAPP_TranDec000_000(nTranCode);
		break;
	case nEnCmd003_3:		
		fnAPP_TranDec003_003(nTranCode);
		break;
	case nEnCmd005_5:		
		fnAPP_TranDec005_005(nTranCode);
		break;
	case nEnCmd004_6:	
		fnAPP_TranDec004_006(nTranCode);
		break;
	case nEnCmd007_7:		
		fnAPP_TranDec007_007(nTranCode);
		break;
	case nEnCmd008_8:		
		fnAPP_TranDec000_000(nTranCode);
		break;
	case nEnCmd011_11:	
//		fnAPP_TranDec011_011(nTranCode);
		fnAPP_TranDec000_000(nTranCode);
		break;
	case nEnCmd012_12:	
//		fnAPP_TranDec012_012(nTranCode);
		fnAPP_TranDec000_000(nTranCode);
		break;
	case nEnCmd020_20:	
		fnAPP_TranDec020_020(nTranCode);
		break;
	case nEnCmd023_23:	
		fnAPP_TranDec023_023(nTranCode);
		break;
	case nEnCmd027_27:	
		fnAPP_TranDec053_053(nTranCode);
		break;
	case nEnCmd031_31:	
		fnAPP_TranDec031_031(nTranCode);
		break;
	case nEnCmd032_32:	
		fnAPP_TranDec032_032(nTranCode);
		break;
	case nEnCmd034_34:	
		fnAPP_TranDec034_034(nTranCode);
		break;
	case nEnCmd035_35:	
		fnAPP_TranDec035_035(nTranCode);
		break;
	case nEnCmd037_37:	
		fnAPP_TranDec037_037(nTranCode);
		break;
	case nEnCmd039_39:	
		fnAPP_TranDec039_039(nTranCode);
		break;
	case nEnCmd045_45:	
		fnAPP_TranDec045_045(nTranCode);
		break;
	case nEnCmd048_48:	
		fnAPP_TranDec048_048(nTranCode);
		break;
	case nEnCmd050_50:	
		fnAPP_TranDec050_050(nTranCode);
		break;
	case nEnCmd054_54:	
		fnAPP_TranDec054_054(nTranCode);
		break;
	case nEnCmd055_55:	
		fnAPP_TranDec000_000(nTranCode);
		break;
	case nEnCmd056_56:	
		fnAPP_TranDec056_056(nTranCode);
		break;
	case nEnCmd071_71:	
		fnAPP_TranDec071_071(nTranCode);
		break;
	case nEnCmd025_81:	
		fnAPP_TranDec025_081(nTranCode);
		break;
	case nEnCmd021_88:	
		fnAPP_TranDec021_088(nTranCode);
		break;
	case nEnCmd105_105:
		fnAPP_TranDec105_105(nTranCode);
		break;
	case nEnCmd106_106:
		fnAPP_TranDec106_106(nTranCode);
		break;
	case nEnCmd107_107:
		fnAPP_TranDec107_107(nTranCode);
		break;
	case nEnCmd111_111:
		fnAPP_TranDec111_111(nTranCode);
		break;
	case nEnCmd120_120:
		fnAPP_TranDec120_120(nTranCode);
		break;
	case nEnCmd121_121:
		fnAPP_TranDec121_121(nTranCode);
		break;
	case nEnCmd126_192:
		fnAPP_TranDec126_192(nTranCode);
		break;
	case nEnCmd127_127:
		fnAPP_TranDec127_127(nTranCode);
		break;
	case nEnCmd134_134:
		fnAPP_TranDec134_034(nTranCode);
		break;
	case nEnCmd135_135:
		fnAPP_TranDec135_135(nTranCode);
		break;
	case nEnCmd137_137:
		fnAPP_TranDec137_037(nTranCode);
		break;
	case nEnCmd184_184:
		fnAPP_TranDec184_184(nTranCode);
		break;
	case nEnCmd193_193:
		fnAPP_TranDec193_193(nTranCode);
		break;
	case nEnCmd194_194:
		fnAPP_TranDec194_194(nTranCode);
		break;
	case nEnCmd195_195:
		fnAPP_TranDec195_195(nTranCode);
		break;
	case nEnCmd209_379_PUBLIC:
	case nEnCmd209_369:
		fnAPP_TranDec209_209(nTranCode);
		break;
	case nEnCmd218_218:
		fnAPP_TranDec218_218(nTranCode);
		break;
	case nEnCmd230_230:
		fnAPP_TranDec230_230(nTranCode);
		break;
	case nEnCmd238_368:
		fnAPP_TranDec238_238(nTranCode);
		break;
	case nEnCmd240_364:
		fnAPP_TranDec240_240(nTranCode);
		break;
	case nEnCmd243_243:
		fnAPP_TranDec243_243(nTranCode);
		break;
	case nEnCmd247_247:
		fnAPP_TranDec247_247(nTranCode);
		break;
	case nEnCmd261_261:
		fnAPP_TranDec261_261(nTranCode);
		break;
	case nEnCmd262_262:
		fnAPP_TranDec262_262(nTranCode);
		break;
	case nEnCmd263_263:
		fnAPP_TranDec263_263(nTranCode);
		break;
	case nEnCmd264_264:
		fnAPP_TranDec264_264(nTranCode);
		break;
	case nEnCmd265_265:
		fnAPP_TranDec265_265(nTranCode);
		break;
	case nEnCmd266_266:
		fnAPP_TranDec266_266(nTranCode);
		break;
	case nEnCmd267_267:
		fnAPP_TranDec267_267(nTranCode);
		break;
	case nEnCmd268_268:
		fnAPP_TranDec268_268(nTranCode);
		break;
	case nEnCmd269_269:
		fnAPP_TranDec269_269(nTranCode);
		break;
	case nEnCmd270_270:
		fnAPP_TranDec270_270(nTranCode);
		break;
	case nEnCmd278_278:
		fnAPP_TranDec278_278(nTranCode);
		break;
	case nEnCmd279_279:
		fnAPP_TranDec279_279(nTranCode);
		break;
	case nEnCmd280_280:
		fnAPP_TranDec280_280(nTranCode);
		break;
	case nEnCmd287_287:
		fnAPP_TranDec287_287(nTranCode);
		break;
	case nEnCmd289_289:
		fnAPP_TranDec289_289(nTranCode);
		break;
	case nEnCmd290_290:
		fnAPP_TranDec290_290(nTranCode);
		break;
	case nEnCmd291_291:
		fnAPP_TranDec291_291(nTranCode);
		break;
	case nEnCmd292_292:
		fnAPP_TranDec292_292(nTranCode);
		break;
	case nEnCmd294_294_BC:
//		fnAPP_TranEnc292_292(nTranCode);
		fnAPP_TranDec294_294();
		break;
//	case nEnCmd384_364:
	case nEnCmd484_484:
	case nEnCmd601_601:
	case nEnCmd701_701:
	case nEnCmd702_702:
	case nEnCmd703_703:
		fnAPP_TranDec000_000(nTranCode);
		break;
	default:
		fnAPP_TranDec000_000(nTranCode);
		break;
	}

	return T_OK;
}


//ms(104) + passwd(16) = 120 //Dump (TRACE_CODE_MODE -> TRACE_DUMP_MODE)
int	CTranCmn::fnAPP_TranEnc004_006(int nTranCode)
{

	uint8   stroutbuff[9999];
 	char	strinbuff[9999];
	uint16  out_len;
	int		iret;
	memset(strinbuff, 0x00, sizeof(strinbuff));
	memset(stroutbuff, 0x00, sizeof(stroutbuff));
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_104 = [%04d]", nTranCode);	
	if (PbTran & MENU_PB) 
	{
		if (TranProc == TRAN_DEP || TranProc == TRAN_PB)
		{
			memset(&m_STXEncBuff.byPlaintMsdata[0], 0x20, 120);
			memcpy(&m_STXEncBuff.byPlaintMsdata[0], PbMsData.sBuf, PbMsData.Len);
		//	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_006_strinbuff_before",m_STXEncBuff.byPlaintMsdata, strlen(m_STXEncBuff.byPlaintMsdata) );
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d][%d]", PbMsData.Len, strlen(m_STXEncBuff.byPlaintMsdata));		
		}
		else
		{
			memset(&m_STXEncBuff.byPlaintMsdata[0], 0x20, 120);
			memcpy(&m_STXEncBuff.byPlaintMsdata[0], PbMsData.sBuf, PbMsData.Len);
			memset(&m_STXEncBuff.byPlaintMsdata[44], 0x20, 16);
			memcpy(&m_STXEncBuff.byPlaintMsdata[44], Accept.PbConfirmNum, sizeof(Accept.PbConfirmNum));
			memset(&m_STXEncBuff.byPlaintMsdata[104], 0x20, 16);
			memcpy(&m_STXEncBuff.byPlaintMsdata[104], Accept.PassWord, sizeof(Accept.PassWord));
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d][%d]", PbMsData.Len, strlen(m_STXEncBuff.byPlaintMsdata));	
		}	
	}
	else
	{
		memcpy(&m_STXEncBuff.byPlaintMsdata[0],   CardData.ISO3Buff, CardData.ISO3size);
		memset(&m_STXEncBuff.byPlaintMsdata[104], 0x20, 16);
		memcpy(&m_STXEncBuff.byPlaintMsdata[104], Accept.PassWord, sizeof(Accept.PassWord));
	}
	
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_006_strinbuff_before",m_STXEncBuff.byPlaintMsdata, strlen(m_STXEncBuff.byPlaintMsdata) );
	MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "iret = [%04d][%d]", CardData.ISO3size, strlen(m_STXEncBuff.byPlaintMsdata));		
	// 암화화 장애시 해결방법 (안기삼과장님 0426) -> 김기진 요청 (안기삼 과장님 기업은행과 명확한 문서형식 요청할것 )
	iret = KB_Encrypt_Message (&m_KB_XM_CTX, (BYTE *)m_STXEncBuff.byChiperMsdata, &out_len, sizeof(m_STXEncBuff.byChiperMsdata), (uint8 *)m_STXEncBuff.byPlaintMsdata, strlen(m_STXEncBuff.byPlaintMsdata));
	if(iret < 0)
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);		
	}
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_006_m_KB_XM_CTX",(char *)&m_KB_XM_CTX, sizeof(m_KB_XM_CTX) );
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_006_strinbuff_After",(char *)m_STXEncBuff.byChiperMsdata, out_len);
	memset(&m_STXEncBuff.byChiperMsdata[out_len], 0x20, 168 - out_len);
	Int2Asc(168, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	return T_OK;
}
//Kangsu (신용카드 암호화) ms(104) + passwd(16) = 120 - 미사용
int	CTranCmn::fnAPP_TranEnc034_034(int nTranCode)
{


	char   strprimary_key[60];
	char   stroutbuff[1024];
 	char   strinbuff[1024];
	int	   sub_key;
	memset(&m_G_KEY_T, 0x00, sizeof(GETKEYTABLE));
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));

	memset(strprimary_key,0x00,sizeof(strprimary_key));
	memset(strinbuff,0x00,sizeof(strinbuff));
	memset(stroutbuff,0x00,sizeof(stroutbuff));
	
	memset(&m_STXEncBuff.byPlaintMsdata[0], 0x00, sizeof(m_STXEncBuff.byPlaintMsdata));	  //#광주 신용카드 암호화
	memset(&m_STXEncBuff.byPlaintMsdata[0], 0x20, 120);	  //광주 신용카드 암호화
	if(MenuAvail & MENU_MCU2)
	{
		memcpy(&m_STXEncBuff.byPlaintMsdata[0], CardData.ISO2Buff, CardData.ISO2size);		
		memcpy(&m_STXEncBuff.byPlaintMsdata[104], Accept.PassWord, 4);
	}
	else
	if(MenuAvail & MENU_MCU3)
	{
		memcpy(&m_STXEncBuff.byPlaintMsdata[0], CardData.ISO3Buff, CardData.ISO3size);		
		memcpy(&m_STXEncBuff.byPlaintMsdata[104], Accept.PassWord, 4);
	}
	else
	if(MenuAvail & MENU_PB)
	{
		memcpy(&m_STXEncBuff.byPlaintMsdata[0], &PbMsData.sBuf[0], PbMsData.Len);		
		memcpy(&m_STXEncBuff.byPlaintMsdata[104], Accept.PassWord, 4);
	}

	sprintf(m_G_KEY_T.m_key, "%s", IniGetStr(_ATMKEYTABELE_INI, KJB_ORG_SEC, "MASTER", "4155544f4d414348494e45434f4f5250"));
	sprintf(m_G_KEY_T.s_key, "%s", IniGetStr(_ATMKEYTABELE_INI, KJB_ORG_SEC, "SLAVE0", "00"));
	sub_key = Asc2Int(m_G_KEY_T.s_key, 2);
	MakePack(m_G_KEY_T.m_key, strprimary_key, 32);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc034_034",m_STXEncBuff.byPlaintMsdata, 120 );
	SeedKJB(strprimary_key, sub_key, m_STXEncBuff.byPlaintMsdata, m_STXEncBuff.byChiperMsdata, SEND_ENCRYPT);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc034_034",m_STXEncBuff.byChiperMsdata, 120 );

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc034_034() nTranCode = [%04d]", nTranCode);	

	return T_OK;
}

//Chb = passwd(4) -> 신한
int	CTranCmn::fnAPP_TranEnc021_088(int nTranCode)
{

	const char *Returnsrc;
	char Packsrc[33];
	char Packdst[17];
	unsigned char strhexa[17] = "0123456789abcdef";
	int slen =0;
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));
	memset(m_pMsgData.cs_acctno,NULL,sizeof(MSG_DATA));
	memcpy(m_pMsgData.cs_acctno, &CardData.ISO3Buff[2],23);
	memcpy(m_pMsgData.cs_txamt, &Accept.Money[3],12);
	memset(m_pMsgData.cs_serialno,0x30,3);
	m_pMsgData.cs_serialno[2] = CardData.ISO3Buff[65];
	memcpy(m_pMsgData.cs_custno,Accept.PassWord,4);
	memcpy(&m_pMsgData.cs_custno[4],&CardData.ISO3Buff[61],4);
	Returnsrc = chb_convert(m_pMsgData);
	memcpy(Packsrc,Returnsrc,sizeof(Packsrc));
	for ( int j = 0 ; j < 33 ; j++)
	{
		Packsrc[j] =strhexa[Packsrc[j] - 0x30];
	}
	MakePack(&Packsrc[1], Packdst, 32);
	memcpy(&m_STXEncBuff.byChiperMsdata, Packdst, 16);
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc021_088",Packdst, 16 );

	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc021_088(%d)", nTranCode);		


	return T_OK;
}

//hana Passwd + Ms (계좌번호)
int	CTranCmn::fnAPP_TranEnc025_081(int nTranCode)
{

	char strinbuff[34];
	char stroutbuff[120];
	int  ioutl;

	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));
	memset(m_STXEncBuff.byPlaintMsdata,0x20,sizeof(m_STXEncBuff.byPlaintMsdata));
	memset(stroutbuff,0x00,sizeof(stroutbuff));

	if (PbTran & MENU_PB)
	{
		memcpy(&m_STXEncBuff.byPlaintMsdata[0], Accept.PassWord, 4);		
		memcpy(&m_STXEncBuff.byPlaintMsdata[4], Accept.PbConfirmNum, 5);

		ISEED_encrypt(HANA_ENCRIPT_DIR, (BYTE *)m_STXEncBuff.byPlaintMsdata, 4, (BYTE *)stroutbuff, 120, &ioutl);
		ISEED_encrypt(HANA_ENCRIPT_DIR, (BYTE *)&m_STXEncBuff.byPlaintMsdata[4], 5, (BYTE *)&stroutbuff[4], 120, &ioutl);

		MakeUnPack(stroutbuff, m_STXEncBuff.byChiperMsdata, 9);
	}
	else
	if (MenuAvail & MENU_MU)
	{
		memcpy(&m_STXEncBuff.byPlaintMsdata[0], Accept.PassWord, 4);		
		memcpy(&m_STXEncBuff.byPlaintMsdata[4], Accept.PbConfirmNum, 5);
		memcpy(&m_STXEncBuff.byPlaintMsdata[9], Accept.JuminNum, 13);

		ISEED_encrypt(HANA_ENCRIPT_DIR, (BYTE *)m_STXEncBuff.byPlaintMsdata, 4, (BYTE *)stroutbuff, 120, &ioutl);
		ISEED_encrypt(HANA_ENCRIPT_DIR, (BYTE *)&m_STXEncBuff.byPlaintMsdata[4], 5, (BYTE *)&stroutbuff[4], 120, &ioutl);
		ISEED_encrypt(HANA_ENCRIPT_DIR, (BYTE *)&m_STXEncBuff.byPlaintMsdata[9], 13, (BYTE *)&stroutbuff[9], 120, &ioutl);

		MakeUnPack(stroutbuff, m_STXEncBuff.byChiperMsdata, 22);
	}
	else
	{
		memcpy(&m_STXEncBuff.byPlaintMsdata[0], &CardData.ISO3Buff[8], 16);		
		memcpy(&m_STXEncBuff.byPlaintMsdata[16], Accept.PassWord, 4);
		memcpy(&m_STXEncBuff.byPlaintMsdata[20], &CardData.ISO3Buff[61], 4);
		memcpy(&m_STXEncBuff.byPlaintMsdata[24], &CardData.ISO3Buff[48], 6);
		HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_025_1",strinbuff, sizeof(strinbuff) );
		
		ISEED_encrypt(HANA_ENCRIPT_DIR, (BYTE *)m_STXEncBuff.byPlaintMsdata, 16, (BYTE *)m_STXEncBuff.byChiperMsdata, 120, &ioutl);
		HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_025_1",m_STXEncBuff.byChiperMsdata, ioutl );
		ISEED_encrypt(HANA_ENCRIPT_DIR, (BYTE *)&m_STXEncBuff.byPlaintMsdata[16], 16, (BYTE *)&m_STXEncBuff.byChiperMsdata[16], 120, &ioutl);
		HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_025_2",&m_STXEncBuff.byChiperMsdata[16], ioutl );
	}
	
	

	Int2Asc(ioutl, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_025 nTranCode = [%04d]", nTranCode);		

	
	return T_OK;
}
//busan
int	CTranCmn::fnAPP_TranEnc032_032(int nTranCode)
{

	int iret = -1;
	char strBpassword[120];
	char strBxprday[120];
	char strBacctno[120];
	char strBepin[120];

	memset(strBepin, 0x00, sizeof(strBepin));
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));

	sprintf(strBpassword, "%4.4s", Accept.PassWord);
	sprintf(strBacctno, "%12.12s", &CardData.ISO3Buff[9]);
	sprintf(strBxprday, "%4.4s"  , &CardData.ISO3Buff[61]);

	
	iret = PEMencrypt ( strBpassword, strBxprday, strBacctno, strBepin );

	if( iret < 0 ) MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc032_032() BUSAN ENCRYPT ERR");		
	memcpy(&m_STXEncBuff.byChiperMsdata, strBepin, 16);

	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc032_032() nTranCode = [%04d]", nTranCode);		


	return T_OK;
}
//kiup -> 0131(ms - 104 byte), 0151(pwd - 24 byte)
int	CTranCmn::fnAPP_TranEnc003_003(int nTranCode)
{

	int iret;
	char	strinspace[14];
	char	szTmp[8];
    uint8   stroutbuff[9048];
 	uint8	strinbuffv[9048];
	uint16  out_len;
	uint16 slen = 0;
	int    nIndex = 0;

 	memset(strinbuffv, 0x00, sizeof(strinbuffv));
	memset(stroutbuff, 0x00, sizeof(stroutbuff));
	memset(szTmp, 0x20, sizeof(szTmp));
	memset(strinspace, 0x20, sizeof(strinspace));

	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));

	if (PbTran & MENU_PB)
	{

		memcpy(szTmp, &PbMsData.sBuf[15], PbMsData.Len - 15);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "PbMsData sBuf = [%s]", &PbMsData.sBuf);
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "strTmp = [%s]", &szTmp);
		
		sprintf(m_STXEncBuff.byPlaintMsdata, "0002359003=0%14.14s00%4.4s%12.12s009%9.9s007%7.7s", \
				 Accept.AccountNum, Accept.PassWord, SPACE12, SPACE9, szTmp);
	}
	else
	{
// 		sprintf(m_STXEncBuff.byPlaintMsdata, "00%1.1s23%23.23s%4.4s%12.12s009%6.6s%1.1s00007%7.7s", &CardData.ISO3Buff[65], \
// 				&CardData.ISO3Buff[2], Accept.PassWord, strinspace,&CardData.ISO3Buff[48],  \
// 				&CardData.ISO3Buff[47],&CardData.ISO3Buff[88] );


		memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], pCardData3->AccountNo, 16);
		nIndex += 16;

		memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], CardData.ISO3Buff, 104);
		nIndex += 104;

		memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], ZERO13, 13);
		memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], &Accept.Money[2],13);
		nIndex += 13;

		memset(&m_STXEncBuff.byPlaintMsdata[nIndex], '0', 18);
		nIndex += 18;

		switch(nTranCode)
		{
		case TRANID_4310:
		case TRANID_7220:
		case TRANID_7210:
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], ZERO3, 3);   //입금은행코드
			nIndex += 3;
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], SPACE16, 16); //계좌
			nIndex += 16;
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], Accept.PassWord, 4);
			nIndex += 4;
			memset(&m_STXEncBuff.byPlaintMsdata[nIndex], '0', 14);
			nIndex += 14;
			memset(&m_STXEncBuff.byPlaintMsdata[nIndex], ' ', 32);
			nIndex += 32;

			break;
		case TRANID_7260:
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], Accept.BankNum, 3);   //입금은행코드
			nIndex += 3;
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], SPACE16, 16); //계좌
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], Accept.InAccountNum, Accept.InAccountNumSize);
			nIndex += 16;
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], Accept.PassWord, 4);
			nIndex += 4;
			memset(&m_STXEncBuff.byPlaintMsdata[nIndex], '0', 14);
			nIndex += 14;
			memset(&m_STXEncBuff.byPlaintMsdata[nIndex], ' ', 32);
			nIndex += 32;
			break;
		case TRANID_4370:
		case TRANID_4390:
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], Accept.BankNum, 3);   //입금은행코드
			nIndex += 3;
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], SPACE16, 16); //계좌
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], Accept.InAccountNum, Accept.InAccountNumSize);
			nIndex += 16;
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], Accept.PassWord, 4);
			nIndex += 4;
			memset(&m_STXEncBuff.byPlaintMsdata[nIndex], '0', 14);
			nIndex += 14;
			memset(&m_STXEncBuff.byPlaintMsdata[nIndex], ' ', 20);
			nIndex += 20;
			memset(&m_STXEncBuff.byPlaintMsdata[nIndex], '0', 2);
			nIndex += 2;
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], m_RD.byDataField_004x, 10);
			nIndex += 10;
			break;

		case TRANID_4320:
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], ZERO3, 3);   //입금은행코드
			nIndex += 3;
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], SPACE16, 16); //계좌
			nIndex += 16;
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], Accept.PassWord, 4);
			nIndex += 4;
			memset(&m_STXEncBuff.byPlaintMsdata[nIndex], '0', 14);
			nIndex += 14;
			memset(&m_STXEncBuff.byPlaintMsdata[nIndex], ' ', 32);
			nIndex += 32;
			break;
		default:
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], ZERO3, 3);   //입금은행코드
			nIndex += 3;
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], SPACE16, 16); //계좌
			nIndex += 16;
			memcpy(&m_STXEncBuff.byPlaintMsdata[nIndex], Accept.PassWord, 4);
			nIndex += 4;
			memset(&m_STXEncBuff.byPlaintMsdata[nIndex], '0', 14);
			nIndex += 14;
			memset(&m_STXEncBuff.byPlaintMsdata[nIndex], ' ', 32);
			nIndex += 32;
			break;
		}

	}

	memset(&m_STXEncBuff.byPlaintMsdata[nIndex], ' ', 35);
	nIndex += 35;
			
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_encrypt103",m_STXEncBuff.byPlaintMsdata, nIndex );
	iret = XMC2_Encrypt_Message_Base64 (&m_KIUP_XM_CTX, stroutbuff, &out_len, sizeof(stroutbuff), (uint8 *)m_STXEncBuff.byPlaintMsdata, nIndex);
	if(iret < 0)
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);		
	}
		
	Int2Asc(out_len, &m_STXEncBuff.byChiperMsdata[0], sizeof(m_STXEncBuff.byOutLen), 10, '0');
	memcpy(&m_STXEncBuff.byChiperMsdata[4], stroutbuff, out_len);

//	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_encrypt103_m_KIUP_XM_CTX",(char *)&m_KIUP_XM_CTX, sizeof(m_KIUP_XM_CTX) );
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_003_strinbuff_After",(char *)m_STXEncBuff.byChiperMsdata, out_len);

	Int2Asc((out_len + 4), m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_003 Return");
	return T_OK;
}
//non (passwd + ms)
int	CTranCmn::fnAPP_TranEnc011_011(int nTranCode)
{

	int  ioutl = 0;
	int  iret = -1;
	int  datalen = 4;
	memset(m_non_encdata, 0x00, sizeof(m_non_encdata));
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));

	if(m_handle_NON) {
		ISA_HANDLE_release(m_handle_NON);
		m_handle_NON=NULL;	
	}
	iret = ISA_HANDLE_load_encfile(&m_handle_NON, NON_ENCRIPT_DIR,  m_AtmCode, strlen(m_AtmCode));
	if (iret < 0) {
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);	
		return T_OK;		
	}


	if (TranProc == TRAN_DEP || TranProc == TRAN_PB)
	{
		memset(Accept.PassWord, '9', sizeof(Accept.PassWord));
	}

	/* 데이터 암호화 */
	iret = ISA_cencrypt(m_handle_NON, m_non_encdata, sizeof(m_non_encdata), &ioutl, Accept.PassWord, datalen, ALG_SEED_CBC);
	if (iret < 0) {
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);		
		return T_OK;
	}
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_011 Res",(char *)m_non_encdata, ioutl);

	Int2Asc(ioutl, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	memcpy(&m_STXEncBuff.byChiperMsdata, &m_non_encdata[0], ioutl);
		

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_011 Return");

	return T_OK;
}

//non (passwd + ms)
int	CTranCmn::fnAPP_TranEnc012_012(int nTranCode)
{

	int  ioutl = 0;
	int  iret = -1;
	int  datalen = 4;
	memset(m_non_encdata, 0x00, sizeof(m_non_encdata));
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));

	if(m_handle_NON) {
		ISA_HANDLE_release(m_handle_NON);
		m_handle_NON=NULL;	
	}
	iret = ISA_HANDLE_load_encfile(&m_handle_NON, NON_ENCRIPT_DIR,  m_AtmCode, strlen(m_AtmCode));
	if (iret < 0) {
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);	
		return T_OK;		
	}


	if (TranProc == TRAN_DEP || TranProc == TRAN_PB)
	{
		memset(Accept.PassWord, '9', sizeof(Accept.PassWord));
	}

	/* 데이터 암호화 */
	iret = ISA_cencrypt(m_handle_NON, m_non_encdata, sizeof(m_non_encdata), &ioutl, Accept.PassWord, datalen, ALG_SEED_CBC);
	if (iret < 0) {
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);		
		return T_OK;
	}
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_011 Res",(char *)m_non_encdata, ioutl);

	Int2Asc(ioutl, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	memcpy(&m_STXEncBuff.byChiperMsdata, &m_non_encdata[0], ioutl);
		

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_011 Return");

	return T_OK;
}

//sc (passwd + ms)
int	CTranCmn::fnAPP_TranEnc023_023(int nTranCode)
{
	int  ioutl = 0;
	int  iret = -1;
	int datalen = 4;
	memset(m_non_encdata, 0x00, sizeof(m_non_encdata));
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));


	if(m_handle_SC) {
		SC_ISA_HANDLE_release(m_handle_SC);
		m_handle_SC=NULL;
	}
	iret = SC_ISA_HANDLE_load_encfile(&m_handle_SC, SC_ENCRIPT_DIR,  m_AtmCode, strlen(m_AtmCode));
	if (iret < 0) {
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);	
		return T_OK;		
	}

	/* 데이터 암호화 */
	iret = SC_ISA_cencrypt(m_handle_SC, m_non_encdata, sizeof(m_non_encdata), &ioutl, Accept.PassWord, datalen, ALG_SEED_CBC);
	if (iret < 0) {
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);
		return T_OK;	
	}
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_023 Res",(char *)m_non_encdata, ioutl);

	Int2Asc(ioutl, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	memcpy(&m_STXEncBuff.byChiperMsdata, &m_non_encdata[0], ioutl);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_023 Return");

	return T_OK;
}




/**********************************************************************************************
[암호화 데이터]
 - 증권사 암호화전문의 암호화영역 192byte부분에 아래 항목을 통째로 암호화하여 100byte 
 - 비밀번호 : 밀번호(4) + space(2)
 - 계좌번호 : 카드번호(8) + space(8)
 - 암호화   : filler 78 space 사용안함.
**********************************************************************************************/ 
/*
	30 31 35-39 30 32 35 3d 31 32 32				0159025=122
	35 30 31 31 37 35 39 36-33 30 37 30 30 36 3d 34 50117596307006=4
  	31 30 34 31 30 33 30 30-30 30 30 30 30 30 30 30 1041030000000000
	30 30 30 30 30 32 35 30-30 30 30 31 35 30 30 30 0000025000015000
	30 30 39 39 39 39 31 3d-30 30 30 30 30 30 30 30 0099991=00000000
	30 30 30 30 30 30 30 30-30 3d 3d 30 3d 31 36 33 000000000==0=163
	33 35 35 31 33 34 30 30-30 30 30 30 30			3551340000000
*/
//삼성증권암호화 2009.08.25 Gi.Jin
int	CTranCmn::fnAPP_TranEnc240_240(int nTranCode)
{

	int nRet;
	char pszClientPlainText[100];
    char szTranBuffer[4000 + 1];
	UString usPlainText, usEncTData;
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));

    TN_TCInitUString( &usPlainText );
    TN_TCInitUString( &usEncTData );

	memset(pszClientPlainText, 0x00, sizeof(pszClientPlainText));
	memset(pszClientPlainText, 0x20, 24);
 

	if((memcmp(&CardData.ISO3Buff[4], SASSBANKID, strlen(SASSBANKID)) == 0))
	{
		
		if((CardData.ISO3Buff[8] == '0') && (CardData.ISO3Buff[9] != '0'))
		{
			memcpy(pszClientPlainText, Accept.PassWord, 4);
			memcpy(&pszClientPlainText[6], &CardData.ISO3Buff[8], 14);
			memcpy(&pszClientPlainText[20], "  ", 2);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_240-1 = [%22.22s]", pszClientPlainText); /*에러처리*/		
		}
		else
		{
			if(memcmp(&CardData.ISO3Buff[88], "0306", 4) == 0)
			{
				memcpy(pszClientPlainText, Accept.PassWord, 4);
				memcpy(&pszClientPlainText[6], &CardData.ISO3Buff[94], 8);
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_240-2 = [%22.22s]", pszClientPlainText); /*에러처리*/		
			}
			else
			if(memcmp(&CardData.ISO3Buff[88], "0307", 4) == 0)
			{
				if(Asc2Int(&CardData.ISO3Buff[67], 1) >= 7) //t - OK (GI.JIN) 04/30
				{
					memcpy(pszClientPlainText, Accept.PassWord, 4);
					memcpy(&pszClientPlainText[6], &CardData.ISO3Buff[67], 12);
					MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_240-3 = [%22.22s]", pszClientPlainText); /*에러처리*/		
				}
				else
				{
					memcpy(pszClientPlainText, Accept.PassWord, 4);
					memcpy(&pszClientPlainText[6], &CardData.ISO3Buff[67], 10);
					MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_240-4 = [%22.22s]", pszClientPlainText); /*에러처리*/
				}
			}
		}		
	}
	else	// 삼성증권 실계좌 또는 기존 타은행 가상계좌(8자리체계)  
	{
		if(memcmp(&CardData.ISO3Buff[88], "0306", 4) == 0)
		{
			memcpy(pszClientPlainText, Accept.PassWord, 4);
			memcpy(&pszClientPlainText[6], &CardData.ISO3Buff[94], 8);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_240-5 = [%22.22s]", pszClientPlainText); /*에러처리*/		
		}
		else
		if(memcmp(&CardData.ISO3Buff[88], "0307", 4) == 0)
		{
			if(Asc2Int(&CardData.ISO3Buff[67], 1) >= 7) //- OK (GI.JIN) 04/30
			{
				memcpy(pszClientPlainText, Accept.PassWord, 4);
				memcpy(&pszClientPlainText[6], &CardData.ISO3Buff[67], 12);
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_240-6 = [%22.22s]", pszClientPlainText); /*에러처리*/		
			}
			else
			{
				memcpy(pszClientPlainText, Accept.PassWord, 4);
				memcpy(&pszClientPlainText[6], &CardData.ISO3Buff[67], 10);
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_240-7 = [%22.22s]", pszClientPlainText); /*에러처리*/
			}
		}	
	}

		
	TN_TCSetUString( &usPlainText, (unsigned char*)pszClientPlainText, 22 );

    nRet = TN_TCClientEncipher( &usPlainText, &usEncTData );
    if ( nRet != 0 )
    {
        TN_TCDeleteMemoryUC( &usPlainText );
        TN_TCDeleteMemoryUC( &usEncTData );
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_240-3 = [%x]", nRet); /*에러처리*/		
    }
    else
    {
        memset( szTranBuffer, 0x20, 4000 );
        nRet = TN_TCConvertUString2TransBuffer( TN_TCENCDATA, &usEncTData, 100, szTranBuffer );
        if ( nRet != 0 )
        {
            TN_TCDeleteMemoryUC( &usPlainText );
            TN_TCDeleteMemoryUC( &usEncTData );
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_240-4 = [%x]", nRet); /*에러처리*/		
        }
    }

    TN_TCDeleteMemoryUC( &usPlainText );
    TN_TCDeleteMemoryUC( &usEncTData );
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_240 ", szTranBuffer, 100);
	
	Int2Asc(100, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	memcpy(&m_STXEncBuff.byChiperMsdata, szTranBuffer, 100);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_240 Return");

	return T_OK;
}


//NH투자증권 2009.07.14 Gi.Jin (AnKiSam Request Nextflow : 2009.08.27) = 향후 문서로 전달 예정임.
int	CTranCmn::fnAPP_TranEnc247_247(int nTranCode)
{

	int iret = 0, sLen = 0, dLen = 0;
	long amnt;
	BYTE pwd[4+1],  hData[20+1];
	BYTE datetime[10+1], sData[143+1], dData[143+1]; 
	memset(datetime, 0x00, sizeof(datetime));

	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));
	amnt = Asc2Int(Accept.Money, 15);
	memcpy(pwd, Accept.PassWord, 4);
	memcpy(datetime, Accept.CardReadTime, 10);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "Dummy Data[%d, %15.15s, %10.10s]", sizeof(Accept.PassWord), Accept.Money, &m_pProfile->TRANS.YYYYMMDDHHMMSS[4]);		

	/* Hash Code Create (전문전송일시(10)+거래금액(12)==>HashCode(20) ) */
	memset(hData, 0x00, sizeof(hData));
	iret = WR_GetHashCode(datetime, amnt, hData);
	if(iret != 0)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "errCode = [X%01d]", iret); /*에러처리*/		

	/* 암호화 대상정보 조합(32 Byte) */
	memset(sData, 0x00, sizeof(sData));
	sLen = 0;
	sData[sLen] = 0x31; sLen += 1;							/* 구분자(1) */
	memcpy(&sData[sLen], hData, 20); sLen += 20;			/* HashCode(20) */
	memcpy(&sData[sLen], pwd, 4); sLen += 4;				/* Password(4) */
	memset(&sData[sLen], 0x20, 7); sLen += 7;				/* Space(7) */
	
	/* 암호화 대상정보 암호화 (Out=100 Byte) */
	memset(dData, 0x00, sizeof(dData));
	iret = WR_EncSeed(m_247W1_KEY, sData, sLen, dData, &dLen);
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[m_247W1_KEY]", dData, dLen);
	if(iret != 0)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "errCode = [X%01d]", iret); /*에러처리*/		

	dData[dLen] = 0x00;


	Int2Asc(dLen, m_STXEncBuff.byChiperMsdata, 3, 10, '0');
	memcpy(&m_STXEncBuff.byChiperMsdata[3], dData, dLen);

	Int2Asc(dLen+3, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc247_247 Return");

	return T_OK;
}

//한화증권 2009.10.23 Gi.Jin (AnKiSam Request Nextflow : 2009.08.27) = 향후 문서로 전달 예정임.
int	CTranCmn::fnAPP_TranEnc269_269(int nTranCode)
{

	int iret, sLen, dLen;
	long amnt;
	BYTE pwd[4+1],  hData[20+1];
	BYTE datetime[10+1], sData[143+1], dData[143+1]; 
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));
	amnt = Asc2Int(Accept.Money, 15);
	m_pProfile->GetTransProfile();
	memcpy(pwd, Accept.PassWord, 4);
	memcpy(datetime, &m_pProfile->TRANS.YYYYMMDDHHMMSS[4], 10);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "Dummy Data[%d, %15.15s, %10.10s, %4.4s]", sizeof(Accept.PassWord), Accept.Money, &m_pProfile->TRANS.YYYYMMDDHHMMSS[4], pwd);		

	/* Hash Code Create (전문전송일시(10)+거래금액(12)==>HashCode(20) ) */
	memset(hData, 0x00, sizeof(hData));
	iret = HW_GetHashCode(datetime, amnt, hData);
	if(iret != 0)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "errCode = [X%01d]", iret); /*에러처리*/		

	/* 암호화 대상정보 조합(64 Byte) */
	memset(sData, 0x00, sizeof(sData));
	sLen = 0;
	sData[sLen] = 0x31; sLen += 1;							/* 구분자(1) */
	memcpy(&sData[sLen], hData, 20); sLen += 20;			/* HashCode(20) */
	memcpy(&sData[sLen], pwd, 4); sLen += 4;				/* Password(4) */
	memset(&sData[sLen], 0x20, 2); sLen += 2;				/* Space(2) */
	memcpy(&sData[sLen], &CardData.ISO3Buff[2], 23); sLen += 23;/* card acount no(23) */
	memset(&sData[sLen], 0x20, 14); sLen += 14;				/* Space(14) */
		
	
	/* 암호화 대상정보 암호화 (Out=128 Byte) */
	memset(dData, 0x00, sizeof(dData));
	iret = HW_EncSeed(m_269W1_KEY, sData, sLen, dData, &dLen);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[m_269W1_KEY]", dData, dLen);
	if(iret != 0)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "errCode = [X%01d]", iret); /*에러처리*/		

	dData[dLen] = 0x00;
	Int2Asc(dLen, m_STXEncBuff.byChiperMsdata, 3, 10, '0');
	memcpy(&m_STXEncBuff.byChiperMsdata[3], dData, dLen);

	Int2Asc(dLen+3, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_269 Return");

	return T_OK;
}

int	CTranCmn::fnAPP_TranEnc218_218(int nTranCode) //현대증권
{

	int iret, sLen, dLen;
	long amnt;
	BYTE pwd[4+1],  hData[20+1];
	BYTE datetime[10+1], sData[143+1], dData[143+1]; 
	BYTE s1_Key[33], w1_Key[33], w2_Key[33];
	BYTE w1_Idx[3];
	int	w1Len, s1Len;
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));
	amnt = Asc2Int(Accept.Money, 15);
	
	m_pProfile->GetTransProfile();
	memcpy(pwd, Accept.PassWord, 4);
	memcpy(datetime, &Accept.CardReadTime[4], 10);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "Dummy Data[%d, %15.15s, %10.10s]", sizeof(Accept.PassWord), Accept.Money, &Accept.CardReadTime[4]);		


	//*EasyCash*
	BYTE MK_TABLE[] = "1A2B3C4D5E6F7F8B2B3C91A1FF09AA953C4D82B2E018BB064D5E73C3D127CCA75E6F64D4C236DDB86F7055E6B345EEC9708146F7A454F0DA81922708957309EC92A31819868218FD03B4092A7791272EFA2B3C485E6F7F81EB3C91A9FF09AA92DC4D82BAE018BB03CD5E73CBD127CCA4BE6F64DCC236DDB5AF7055EDB345EEC6908146FEA454F0D77192270F957309E862A31810868218F963B409217791272A";
//                    "1A2B3C4D5E6F7F8B2B3C91A1FF09AA953C4D82B2E018BB064D5E73C3D127CCA75E6F64D4C236DDB86F7055E6B345EEC9708146F7A454F0DA81922708957309EC92A31819868218FD03B4092A7791272EFA2B3C485E6F7F81EB3C91A9FF09AA92DC4D82BAE018BB03CD5E73CBD127CCA4BE6F64DCC236DDB5AF7055EDB345EEC6908146FEA454F0D77192270F957309E862A31810868218F963B409217791272A";
	memset(s1_Key,                0x00,            sizeof(s1_Key));
	memset(w1_Key,                0x00,            sizeof(w1_Key));
	memset(w2_Key,                0x00,            sizeof(w2_Key));

	memcpy(s1_Key,                datetime,                    10);
	memcpy(s1_Key+10,             &CardData.ISO3Buff[8],		6);

	s1Len=strlen((char *)s1_Key);
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[fnAPP_encrypt218_s1_Key]", s1_Key, s1Len);
	/*----------------------------------------------------------*/
	/* 세션키를 가져온다                                        */
	/*----------------------------------------------------------*/
	fnHD_CKeyInit(w1_Key, &w1Len, w1_Idx, MK_TABLE, s1_Key, s1Len);

	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[fnAPP_encrypt218_s1_Key]", s1_Key, s1Len);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[fnAPP_encrypt218_w1_Key]", w1_Key, w1Len);
	/* Hash Code Create (전문전송일시(10)+거래금액(12)==>HashCode(20) ) */
	memset(hData,                  0x00,             sizeof(hData));
	
	iret = fnHD_GetHashCode(datetime, amnt, hData);
	if(iret != 0)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "errCode = [X%01d]", iret); /*에러처리*/		
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[fnAPP_encrypt218_hData]", hData, sizeof(hData));

	/* 암호화 대상정보 조합(64 Byte) */
	memset(sData, 0x00, sizeof(sData));
	sLen = 0;
	sData[sLen] = 0xAC; sLen += 1;							/* 구분자(1) */
	memcpy(&sData[sLen], hData, 20); sLen += 20;			/* HashCode(20) */
	memcpy(&sData[sLen], pwd, 4); sLen += 4;				/* Password(4) */
	memcpy(&sData[sLen], &CardData.ISO3Buff[8], 7); sLen += 7;/* acount no(7) */
		
	
	/* 암호화 대상정보 암호화 (Out=128 Byte) */
	memset(dData, 0x00, sizeof(dData));
	iret = fnHD_EncSeed(w1_Key, w1_Idx, sData, sLen, dData, &dLen);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc218_218", dData, dLen);
	if(iret != 0)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "errCode = [X%01d]", iret); /*에러처리*/		

	dData[dLen] = 0x00;
	Int2Asc(dLen, m_STXEncBuff.byChiperMsdata, 3, 10, '0');
	memcpy(&m_STXEncBuff.byChiperMsdata[3], dData, dLen);	

	Int2Asc(dLen+3, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc218_218 Return");

	return T_OK;
}

//하나대투증권 2010.12.13 Kim.Gi.Jin 2010.12.21 - Ms + passwd
int	CTranCmn::fnAPP_TranEnc270_270(int nTranCode)
{

	typedef struct _CryptKey
	{
		BYTE	send_length[4];	 // 요청 DATA길이
		BYTE	tran_date[14];	 // 거래시각
		BYTE	trace_no[6];	 // 전문추적번호
		BYTE	tran_type[4];	 // 전문종류
		BYTE	tran_id[4];		 // 전문ID
		BYTE	ms_track_no[1];	 // MS트랙NO
		BYTE	return_code[4];	 // 응답코드
		BYTE	user_org_no[8];	 // 이용자기관번호 
		BYTE	close_flag[1];	 // 마감전후구분
		BYTE	encrypt_flag[1]; // 암호화구분
		BYTE	seq_frefix[3];	 // 거래고유번호 Frefix
		BYTE	seq_cdcode[4];	 // 거래고유번호 CD코드
		BYTE	seq_no[6];		 // 거래고유번호 Sequence NO
		BYTE	cd_code[4];		 // CD코드
		BYTE	reserve[16];	 // 예비
	} CryptKey;

	CryptKey key;
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));

	m_pProfile->GetTransProfile();
	memset(&key, 0x20, sizeof(CryptKey));
	memcpy(key.send_length, "0500", sizeof(key.send_length));
	memcpy(key.tran_date, m_pProfile->TRANS.YYYYMMDDHHMMSS, sizeof(key.tran_date));
	memcpy(key.trace_no, "000000", sizeof(key.trace_no));
	memcpy(key.tran_type, "0200", sizeof(key.tran_type));
	Int2Asc(nTranCode, key.tran_id, sizeof(key.tran_id));
	memcpy(key.ms_track_no, "3", sizeof(key.ms_track_no));
	memcpy(key.return_code, "0000", sizeof(key.return_code));
	memcpy(key.user_org_no, "HYO_MCZ ", sizeof(key.user_org_no));
	memcpy(key.close_flag, "0", sizeof(key.close_flag));
	memcpy(key.encrypt_flag, "1", sizeof(key.encrypt_flag));
	memcpy(key.seq_frefix, "097", sizeof(key.seq_frefix));
	memcpy(key.seq_cdcode, &m_pProfile->NETWORK.AtmSerialNum[2], sizeof(key.seq_cdcode));
	memcpy(key.seq_no, m_pProfile->TRANS.SerialNo, sizeof(key.seq_no));
	memcpy(key.cd_code, &m_pProfile->NETWORK.AtmSerialNum[2], sizeof(key.cd_code));
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[fnAPP_TRAN_ENC_270_Key]", (BYTE *)&key, sizeof(key));


	int nRet = 0;
	int nMsLen = 0;
	char szTmp_In[120];
	char szTmp_Out[1024];

	memset(szTmp_In, 0x00, sizeof(szTmp_In));
	memset(szTmp_Out, 0x00, sizeof(szTmp_Out));
	memcpy(szTmp_In, Accept.PassWord, sizeof(Accept.PassWord));
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[fnAPP_TRAN_ENC_270_passwd_szTmp_In]", szTmp_In, sizeof(Accept.PassWord));
	nRet = ax_encrypt(szTmp_In, sizeof(m_STXEncBuff.byChiperMsdata)*2, (char *)&key, sizeof(key), szTmp_Out);
	if (nRet < 0)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_270 errCode = [X%01d]", nRet); /*에러처리*/		
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[fnAPP_TRAN_ENC_270_passwd_szTmp_Out]", szTmp_Out, sizeof(Accept.PassWord));
	memcpy(m_STXEncBuff.byChiperMsdata, szTmp_Out, sizeof(m_STXEncBuff.byChiperMsdata));

	memset(szTmp_In, 0x00, sizeof(szTmp_In));
	memset(szTmp_Out, 0x00, sizeof(szTmp_Out));
	memcpy(szTmp_In, CardData.ISO3Buff, CardData.ISO3size);
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[fnAPP_TRAN_ENC_270_msdata_szTmp_In]", szTmp_In, nMsLen);
	nRet = ax_encrypt(szTmp_In, nMsLen, (char *)&key, sizeof(key), szTmp_Out);
	if (nRet < 0)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_270 errCode = [X%01d]", nRet); /*에러처리*/		
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[fnAPP_TRAN_ENC_270_msdata_szTmp_Out]", szTmp_Out, nMsLen);
	memcpy(m_STXEncBuff.byChiperMsdata, szTmp_Out, nMsLen);
	Int2Asc(nMsLen, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	return T_OK;
}

//우체국
int	CTranCmn::fnAPP_TranEnc071_071(int nTranCode )
{
	char szLocalKey[] = {"1234567890ABCDEF"};
	//char szRealKey[]  = {"FC58B343A19DE56C"};
	char szRealKey[]  = {"F671DC5817A9630A"};
	char szGetEncData[16];

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_071()");		

	CEzCaDes EzCaDesPBank;

	SUCARDDES s_Enc071;
	memset(&s_Enc071, 0x00, sizeof(s_Enc071));
	memset(&m_STXEncBuff, 0x00, sizeof(m_STXEncBuff));

	memset(&szGetEncData, 0x00, sizeof(szGetEncData));
	
	s_Enc071.sType[0] = 'E';
	memcpy(s_Enc071.sInpData, Accept.PassWord, sizeof(Accept.PassWord));
	
	if(Asc2Int(m_pProfile->NETWORK.BpCurPortNo, 6) == 60001)	
		memcpy(s_Enc071.sKeyData, szRealKey, sizeof(s_Enc071.sKeyData));
	else
		memcpy(s_Enc071.sKeyData, szLocalKey, sizeof(s_Enc071.sKeyData)); //20015
		
	memcpy(s_Enc071.sOutData, szGetEncData, sizeof(s_Enc071.sOutData));

	EzCaDesPBank.EzCaDes(&s_Enc071);

	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[fnAPP_TranEnc071_071]", s_Enc071.sOutData, 16);
	
	memcpy(m_STXEncBuff.byChiperMsdata, s_Enc071.sOutData, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

    MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc071_071() Return");		
	return T_OK;
}

//대우증권 - 별도개발 필요 
int	CTranCmn::fnAPP_TranEnc238_238(int nTranCode )
{
	char szTmp1[16];
	int	 nOutlen = 0;					
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_238()");		

	CSeedDws SeedDws;
	memset(&m_STXEncBuff, 0x00, sizeof(m_STXEncBuff));

	memset(&szTmp1, 0x00, sizeof(szTmp1));

	memcpy(szTmp1, Accept.PassWord, sizeof(Accept.PassWord));
	SeedDws.SeedEncryptData(szTmp1, 4, m_STXEncBuff.byChiperMsdata, &nOutlen );
	Int2Asc(nOutlen, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));	
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[fnAPP_TRAN_ENC_238]", m_STXEncBuff.byChiperMsdata, nOutlen);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_ENC_238() Return");		
	return T_OK;
}


int	CTranCmn::fnAPP_TranDec004_006(int nTranCode )
{

	uint8   stroutbuff[9999];
 	char	strinbuff[9999];
	uint16  out_len;
	int		iret;
	memset(strinbuff, 0x00, sizeof(strinbuff));
	memset(stroutbuff, 0x00, sizeof(stroutbuff));
	if (TranCode2 == TRANID_4730) 
	{
		memcpy(&strinbuff[0], m_SRXEncBuff.byEnDummy, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_DEC_006", (char *)&strinbuff, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
		iret = KB_Decrypt_Message (&m_KB_XM_CTX, (BYTE *)m_SRXEncBuff.byDeDummy, &out_len, sizeof(m_SRXEncBuff.byDeDummy), (uint8 *)strinbuff, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
//		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_DEC_006", (char *)&stroutbuff, out_len ); 
		if(iret < 0)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[복호화장애]"));
			memset(m_SRXEncBuff.byPlaintMsdata, 0x00, sizeof(m_SRXEncBuff.byPlaintMsdata));
			memset(m_SRXEncBuff.byPlaintMsdata, 'X', 14);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);	
			return T_OK;
		}	

		if(m_SRXEncBuff.byDeDummy[3600] == '1')
			sprintf(m_SRXEncBuff.byPlaintMsdata, "-%13.13s", &m_SRXEncBuff.byDeDummy[3600+3]);
		else
			sprintf(m_SRXEncBuff.byPlaintMsdata, " %13.13s", &m_SRXEncBuff.byDeDummy[3600+3]);

		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "m_SRXEncBuff.byPlaintMsdata", (char *)m_SRXEncBuff.byPlaintMsdata, 14 );

	}
	else
	if (TranCode2 == TRANID_4720) 
	{

		if (PbTran & MENU_PB)
		{
			memcpy(&strinbuff[0], m_SRXEncBuff.byEnDummy, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_DEC_006", (char *)&strinbuff, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
			iret = KB_Decrypt_Message (&m_KB_XM_CTX, (BYTE *)m_SRXEncBuff.byDeDummy, &out_len, sizeof(m_SRXEncBuff.byDeDummy), (uint8 *)strinbuff, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_DEC_006", (char *)&m_SRXEncBuff.byDeDummy, out_len ); 
			if(iret < 0)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[복호화장애]"));
				memset(m_SRXEncBuff.byPlaintMsdata, 0x00, sizeof(m_SRXEncBuff.byPlaintMsdata));
				memset(m_SRXEncBuff.byPlaintMsdata, 'X', 14);
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);	
				return T_OK;				
			}	

			if(m_SRXEncBuff.byDeDummy[3600] == '1')
				sprintf(m_SRXEncBuff.byPlaintMsdata, "-%13.13s", &m_SRXEncBuff.byDeDummy[3600+3]);
			else
				sprintf(m_SRXEncBuff.byPlaintMsdata, " %13.13s", &m_SRXEncBuff.byDeDummy[3600+3]);

			Int2Asc(out_len, m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen));	
			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "m_SRXEncBuff.byPlaintMsdata", (char *)m_SRXEncBuff.byPlaintMsdata, 14 );

		}
	}
	else
	{
		memcpy(&strinbuff[0], m_SRXEncBuff.byChiperMsdata, 128);
		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_DEC_006", (char *)&strinbuff, 128);
		iret = KB_Decrypt_Message (&m_KB_XM_CTX, (BYTE *)m_SRXEncBuff.byPlaintMsdata, &out_len, sizeof(m_SRXEncBuff.byPlaintMsdata), (uint8 *)strinbuff, 128);
		if(iret < 0)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[복호화장애]"));
			memset(m_SRXEncBuff.byPlaintMsdata, 0x00, sizeof(m_SRXEncBuff.byPlaintMsdata));
			memset(m_SRXEncBuff.byPlaintMsdata, 'X', 14);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);	
			return T_OK;
		}	


		if(m_SRXEncBuff.byPlaintMsdata[0] == '0') 
			m_SRXEncBuff.byPlaintMsdata[0] =  ' ';
		else 
		if(m_SRXEncBuff.byPlaintMsdata[0] == '-') 
			m_SRXEncBuff.byPlaintMsdata[0] =  '-';
		else 
			m_SRXEncBuff.byPlaintMsdata[0] =  ' ';

		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "m_SRXEncBuff.byPlaintMsdata", (char *)m_SRXEncBuff.byPlaintMsdata, out_len );

	}

/*

	memcpy(&strinbuff[0], m_SRXEncBuff.byChiperMsdata, 128);
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_DEC_006", (char *)&strinbuff, 128);
	iret = KB_Decrypt_Message (&m_KB_XM_CTX, (BYTE *)m_SRXEncBuff.byPlaintMsdata, &out_len, sizeof(m_SRXEncBuff.byPlaintMsdata), (uint8 *)strinbuff, 128);


//---------------------------------------------------------------------------------

	if (m_SRXEncBuff.byPlaintMsdata[0] == '1')
		m_SRXEncBuff.byPlaintMsdata[0] = '-';
	else
	if (m_SRXEncBuff.byPlaintMsdata[0] == '0')
		m_SRXEncBuff.byPlaintMsdata[0] = ' ';
	else
		m_SRXEncBuff.byPlaintMsdata[0] = ' ';
//---------------------------------------------------------------------------------

	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_DEC_006", (char *)&stroutbuff, out_len );
	if(iret < 0)
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);		
	}	
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "m_SRXEncBuff.byPlaintMsdata", (char *)m_SRXEncBuff.byPlaintMsdata, 16 );
*/
	return T_OK;
}


int	CTranCmn::fnAPP_TranDec011_011(int nTranCode )
{

	int  ioutl = 0;
	int  iret = -1;
	char stroutbuff[1024];
 	char strinbuff[1024];

	memset(strinbuff, 0x00, sizeof(strinbuff));
	memset(stroutbuff, 0x00, sizeof(stroutbuff));
	/* 데이터 암호화 */ 
	iret = ISA_cdecrypt(m_handle_NON, m_SRXEncBuff.byDeDummy, sizeof( m_SRXEncBuff.byDeDummy), &ioutl, m_SRXEncBuff.byEnDummy, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
	if (iret < 0) {
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);	
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendRecvHostSnod() 통장부 거래 TIMEOUT!! 불능 Write!!");
//		fnAPD_PbMSWrite();	
		fnAPP_CancelProc(T_RECVERROR);			
		return T_OK;
	}
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_DEC_011 Res",m_SRXEncBuff.byDeDummy, ioutl);

	Int2Asc(ioutl, m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen));

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_encrypt111() nTranCode = [%04d]", nTranCode);		

	return T_OK;
}

int	CTranCmn::fnAPP_TranDec012_012(int nTranCode )
{

	int  ioutl = 0;
	int  iret = -1;
	char stroutbuff[1024];
 	char strinbuff[1024];

	memset(strinbuff, 0x00, sizeof(strinbuff));
	memset(stroutbuff, 0x00, sizeof(stroutbuff));
	/* 데이터 암호화 */ 
	iret = ISA_cdecrypt(m_handle_NON, m_SRXEncBuff.byDeDummy, sizeof( m_SRXEncBuff.byDeDummy), &ioutl, m_SRXEncBuff.byEnDummy, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
	if (iret < 0) {
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "iret = [%04d]", iret);	
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendRecvHostSnod() 통장부 거래 TIMEOUT!! 불능 Write!!");
//		fnAPD_PbMSWrite();	
		fnAPP_CancelProc(T_RECVERROR);			
		return T_OK;
	}
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_DEC_011 Res",m_SRXEncBuff.byDeDummy, ioutl);

	Int2Asc(ioutl, m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen));

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_encrypt111() nTranCode = [%04d]", nTranCode);		

	return T_OK;
}


int	CTranCmn::fnAPP_TranDec240_240(int nTranCode )
{
	 char szTranBuffer[4000 + 1];

	memcpy(szTranBuffer, &m_SRXEncBuff.byChiperMsdata, 100);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_DEC_240_1");		

	fnAPP_Encrypt_240_Step_Flow(szTranBuffer, &usClientDecData);
    TN_TCDeleteMemoryUC( &usClientDecData );
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TRAN_DEC_240_2");		

	return T_OK;
}




int CTranCmn::fnAPP_TranEnc000_000(int nTrCode)		//"????????????"
{

	return T_OK;
}

int CTranCmn::fnAPP_TranEnc001_001(int nTrCode)		//"한국은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc002_002(int nTrCode)		//"산업은행    "
{

	return T_OK;
}

int CTranCmn::fnAPP_TranEnc005_005(int nTrCode)		//"외환은행    "
{

	BYTE szDummy1[17] = "FFFFFFFFFFFFFFFF";
	BYTE szDummy2[17] = "0000";
	BYTE szPinSize[3] = "00";
	BYTE szValidData[13] = "            ";					// 암호화에 필요한 유효데이터 12 바이트
	BYTE szXorPindata[17] ="";
	BYTE szPinBlock[512] = {""};;
	BYTE szDummyWork[512] = {'7', 'E' , '0', 'E' , 'A', '6', '3', '2', '5', 'A', '6', '3', '4', '4', 'D', '8'} ;
	BYTE szMemberNum[512] = {""};
	BYTE o_MASETER_KEY[8] = {0x85, 0x26, 0x07, 0x84, 0x07, 0x85, 0x89, 0x51};
	BYTE o_WORKING_KEY[8] = {0x7E, 0x0E, 0xA6, 0x32, 0x5A, 0x63, 0x44, 0xD8};
	BYTE o_REAL_KEY[8] = {""};
	BYTE o_REAL_KEY_LEFT[8] =  {0x85, 0x26, 0x07, 0x84, 0x07, 0x85, 0x89, 0x51};
	BYTE o_REAL_KEY_RIGHT[8] = {0x85, 0x26, 0x07, 0x84, 0x07, 0x85, 0x89, 0x51};
	BYTE o_PINBLOCK[8] = {""};
	BYTE e_PINBLOCK[8] = {""};
	BYTE DesRPIN[17] = {""};
	
	
	int	 nPinSize = 0;
	int	 nReturn = FALSE;
	

	memset(DesRPIN, 0, sizeof(DesRPIN));						
	nPinSize = sizeof(Accept.PassWord);
	
	CString strTmp("");
	strTmp = IniGetStr(_ATMKEYTABELE_INI, KOEBANKID, "MASTER", "" );
	memcpy(szDummyWork, strTmp,__min(sizeof(o_WORKING_KEY)*2, strlen(strTmp)));

	MakePack(szDummyWork, o_WORKING_KEY, sizeof(o_WORKING_KEY)*2);	
	m_pDevCmn->DECRYPT(o_REAL_KEY, o_WORKING_KEY, 8, o_MASETER_KEY); 
	
	szPinSize[1] |= (UINT)nPinSize;				// SIZE
	memcpy((char *)szDummy1, (char *)szPinSize, 2);
	//memcpy((char *)&szDummy1[2], Accept.PassWord, nPinSize);
	memcpy((char *)&szDummy1[2], Accept.PassWord, nPinSize);
    memcpy((char *)&szDummy2[4],  pCardData3->AccountNo, 12);
	m_pDevCmn->fnUTL_PinDataXor(szDummy1, szDummy2, szXorPindata, 16);
	m_pDevCmn->fnUTL_JisPack(szXorPindata,o_PINBLOCK,16);

	m_pDevCmn->ENCRYPT(e_PINBLOCK, o_PINBLOCK, 8, o_REAL_KEY);
	
	MakeUnPack(e_PINBLOCK, DesRPIN, sizeof(e_PINBLOCK));
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc005(%s)", DesRPIN);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc005():return(%d)", nReturn);		
	
	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	return T_OK;
}

int CTranCmn::fnAPP_TranEnc007_007(int nTrCode)		//"수협중앙회  "
{

	BYTE bydummyEnc[33] = {0,};
	BYTE bysrcData[17];
	BYTE bykeyData[17];
	BYTE DesRPIN[21] = {""};
	BYTE byst;
	int  i= 0;

// Make 평문 대수치
	memcpy(&bydummyEnc[0], Accept.PassWord, 4);
	memcpy(&bydummyEnc[4], &CardData.ISO3Buff[61], 4);
	memcpy(&bydummyEnc[8], Accept.PassWord, 4);
	memcpy(&bydummyEnc[12],&CardData.ISO3Buff[61], 4);


	for(i=0; i<16; i++) // 대수치로 변환
	{
		if(bydummyEnc[i] >= '0' && bydummyEnc[i] <= '9')
		{
			byst = bydummyEnc[i] & 0x0f;
			bysrcData[i] = 9 - byst;
		}
		else if(bydummyEnc[i] >= 'A' && bydummyEnc[i] <= 'Z')
		{
			bysrcData[i] = 100 - bydummyEnc[i];
		}
		else
			bysrcData[i] = 0x00;
	}

	// Make 키 대수치
	memcpy(bydummyEnc, "07      CHC", strlen("07      CHC"));
	memcpy(&bydummyEnc[2], &Accept.CardReadTime[4], 6);
	memcpy(&bydummyEnc[11], &CardData.ISO3Buff[1], 5);


	for(i=0; i<16; i++) // 대수치로 변환
	{
		if(bydummyEnc[i] >= '0' && bydummyEnc[i] <= '9')
		{
			byst = bydummyEnc[i] & 0x0f;
			bykeyData[i] = 9 - byst;
		}
		else if(bydummyEnc[i] >= 'A' && bydummyEnc[i] <= 'Z')
		{
			bykeyData[i] = 100 - bydummyEnc[i];
		}
		else
			bykeyData[i] = 0x00;
	}


	// Make 암호문 대수치
	for(i=0; i<16; i++)
	{
		bydummyEnc[i] = (bysrcData[i] + bydummyEnc[i])%36;
	}

	// 암호문
	for(i=0; i<16; i++) // 대수치로 변환
	{
		if(bydummyEnc[i] >= 0 && bydummyEnc[i] <= 9)
			bydummyEnc[i] = (9 - bydummyEnc[i]) + 0x30;
		else
			bydummyEnc[i] = (100-bydummyEnc[i]) ;
	}

//	_Pack((BYTE *)encrypt, 32, (BYTE *)temp);
	memcpy(DesRPIN, bydummyEnc, 16);

// 송신 암호문 : "S0VXS1TT4A6ZS00T"

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc007_007(%s)", DesRPIN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc007_007():return");


	return T_OK;
}
int CTranCmn::fnAPP_TranEnc008_008(int nTrCode)		//"수출입은행  "
{

	return T_OK;
}
//Woori 
int CTranCmn::fnAPP_TranEnc020_020(int nTrCode)		//"우리은행    "
{


	char   strprimary_key[60];
	char   stroutbuff[1024];
 	char   strinbuff[1024];
	int	   sub_key;
	memset(&m_G_KEY_T, 0x00, sizeof(GETKEYTABLE));
	memset(&m_STXEncBuff, 0x00, sizeof(strinbuff));

	memset(strprimary_key,0x00,sizeof(strprimary_key));
	memset(strinbuff,0x00,sizeof(strinbuff));
	memset(stroutbuff,0x00,sizeof(stroutbuff));
	
	memset(&m_STXEncBuff.byPlaintMsdata[0], 0x00, sizeof(m_STXEncBuff.byPlaintMsdata));	  
	memset(&m_STXEncBuff.byPlaintMsdata[0], 0x20, 16);	  
//	memcpy(&m_STXEncBuff.byPlaintMsdata[0], &PbMsData.sBuf[0], PbMsData.Len);		
	memcpy(&m_STXEncBuff.byPlaintMsdata[0], Accept.PassWord, 4);
	

	switch(nTrCode)
	{
	case TRANID_4310:
	case TRANID_4320:
	case TRANID_7220:
		memcpy(&m_STXEncBuff.byPlaintMsdata[0], Accept.PassWord, 4);
		break;
	case TRANID_7260:
	case TRANID_4370:
	case TRANID_4390:
		memcpy(&m_STXEncBuff.byPlaintMsdata[0], Accept.PassWord, 4);		// 비밀번호(4)
		memcpy(&m_STXEncBuff.byPlaintMsdata[4], &CardData.ISO3Buff[61], 4);	// 유효기간(4)
		memcpy(&m_STXEncBuff.byPlaintMsdata[8], &CardData.ISO3Buff[48], 6);	// 카드비밀번호 6자리(6)
		memcpy(&m_STXEncBuff.byPlaintMsdata[14], ZERO2, 2);			// Space(2)
		break;
	}


	sprintf(m_G_KEY_T.m_key, "%s", IniGetStr(_ATMKEYTABELE_INI, WORINNERID1, "MASTER", "4155544f4d414348494e45434f4f5250"));	
	sprintf(m_G_KEY_T.s_key, "%s", IniGetStr(_ATMKEYTABELE_INI, WORINNERID1, "SLAVE0", "00"));	
	sub_key = Asc2Int(m_G_KEY_T.s_key, 2);
	MakePack(m_G_KEY_T.m_key, strprimary_key, 32);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc020_1",m_STXEncBuff.byPlaintMsdata, 120 );
	WooriSeed(strprimary_key, sub_key, m_STXEncBuff.byPlaintMsdata, m_STXEncBuff.byChiperMsdata);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc020_2",m_STXEncBuff.byChiperMsdata, 120 );

	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc020() nTranCode = [%04d]", nTrCode);	


	return T_OK;
}

int CTranCmn::fnAPP_TranEnc053_053(int nTrCode)		//"씨티은행    "
{

	BYTE szDummy1[17] = "FFFFFFFFFFFFFFFF";
	BYTE szDummy2[17] = "0000";
	BYTE szPinSize[3] = "00";
	BYTE szValidData[13] = "            ";					// 암호화에 필요한 유효데이터 12 바이트
	BYTE szXorPindata[17] ="";
	BYTE szPinBlock[512] = {""};
	/*
	BYTE szDummyWork[512] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86} ;
	BYTE szMemberNum[512] = {""};
	BYTE o_MASETER_KEY[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_WORKING_KEY[8] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86};
	BYTE o_REAL_KEY[8] = {""};
	BYTE o_REAL_KEY_LEFT[8] =  {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_REAL_KEY_RIGHT[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	*/
	BYTE szDummyWork[512] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86} ;
	BYTE szMemberNum[512] = {""};
	BYTE o_MASETER_KEY0[16] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_MASETER_KEY1[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_WORKING_KEY0[16] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86};
	BYTE o_WORKING_KEY1[8] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86};
	BYTE o_REAL_KEY[8] = {""};
	BYTE o_REAL_KEY_LEFT[8] =  {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_REAL_KEY_RIGHT[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_PINBLOCK[8] = {""};
	BYTE e_PINBLOCK[8] = {""};
	BYTE DesRPIN[17] = {""};

	int	 nPinSize = 0;
	int	 nReturn = FALSE;


	//ctkeyo.enc
	MakePack("9831C070E247B846C42DCD0C63A9F074", o_MASETER_KEY0, strlen("9831C070E247B846C42DCD0C63A9F074"));

	//ctkeyt.enc
	MakePack("022446688AACCEE0133557799BBDDFF1", o_WORKING_KEY0, strlen("022446688AACCEE0133557799BBDDFF1"));


	m_pDevCmn->DECRYPT(o_REAL_KEY_LEFT , &o_WORKING_KEY0[0], 8, &o_MASETER_KEY0[0]); 
	m_pDevCmn->DECRYPT(o_REAL_KEY_RIGHT, &o_WORKING_KEY0[8], 8, &o_MASETER_KEY0[8]); 
	


	memset(DesRPIN, 0, sizeof(DesRPIN));						
	nPinSize = sizeof(Accept.PassWord);
	


//	T_DES_DECRYPTOR(o_WORKING_KEY, DES_LEN, o_REAL_KEY_LEFT, o_REAL_KEY_RIGHT, o_REAL_KEY_LEFT, o_REAL_KEY);
	 
//	memcpy(o_REAL_KEY_LEFT, o_REAL_KEY, DES_LEN);

	szPinSize[1] |= (UINT)nPinSize;				// SIZE
	memcpy((char *)szDummy1, (char *)szPinSize, 2);
	memcpy((char *)&szDummy1[2], (char *)Accept.PassWord, nPinSize);
	memcpy(szXorPindata, szDummy1, sizeof(szDummy1));
	//memcpy(&szXorPindata[4], pCardData3->AccountNo, 12);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc053(%16.16s)", szXorPindata);

	MakePack(szXorPindata,o_PINBLOCK,16);

	m_pDevCmn->T_DES_ENCRYPTOR(o_PINBLOCK, DES_LEN, o_REAL_KEY_LEFT, o_REAL_KEY_RIGHT, o_REAL_KEY_LEFT, e_PINBLOCK);
	
	MakeUnPack(e_PINBLOCK, DesRPIN, sizeof(e_PINBLOCK));

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc053(%s)", DesRPIN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc053():return(%d)", nReturn);	

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc031_031(int nTrCode)		//"대구은행    "
{

	return T_OK;
}


int CTranCmn::fnAPP_TranEnc035_035(int nTrCode)		//"제주은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc037_037(int nTrCode)		//"전북은행    "
{

	BYTE szDummy1[17] = "FFFFFFFFFFFFFFFF";
	BYTE szDummy2[17] = "0000000000000000";
	BYTE szPinSize[3] = "00";
	BYTE szValidData[13] = "            ";					// 암호화에 필요한 유효데이터 12 바이트
	BYTE szXorPindata[17] ="";
	BYTE szPinBlock[9] = "        ";
	BYTE szDummyWork[16] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86} ;
	BYTE szMemberNum[512] = {""};;
	BYTE o_MASETER_KEY[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_WORKING_KEY[8] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86};
	BYTE o_REAL_KEY[8] = {""};;
	BYTE o_REAL_KEY_LEFT[8] =  {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_REAL_KEY_RIGHT[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_PINBLOCK[8] = {""};
	BYTE e_PINBLOCK[8] = {""};
	BYTE DesRPIN[21] = {""};


	int	 nPinSize = 0;
	int	 nReturn = FALSE;

//MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc053(%s %d %s %d %s)", vPinNum, PinSize, vMemberNum, MemberNumSize, vWorkingKey);


	memset(DesRPIN, 0, sizeof(DesRPIN));						

// 	CString strTmp("");
// 	strTmp = IniGetStr(_ATMKEYTABELE_INI, JEOBANKID, "MASTER", "CD7114BA459953EF" );
// 	memcpy(szDummyWork, strTmp,__min(sizeof(o_REAL_KEY)*2, strlen(strTmp)));

	memcpy(szDummyWork, "CD7114BA459953EF", strlen("CD7114BA459953EF"));

	m_pDevCmn->fnUTL_JisPack(szDummyWork, o_REAL_KEY, sizeof(o_REAL_KEY)*2);

	switch(nTrCode)
	{
	case TRANID_4350:
	case TRANID_7250:
	case TRANID_7230:
		nPinSize = 4;
		memcpy((char *)szDummy1, "04", 2);
		memcpy((char *)&szDummy1[2], Accept.PassWord, 4);
		memset((char *)&szDummy1[6], 'F', 10);

		memcpy((char *)&szDummy2, ZERO4, strlen(ZERO4));
		memcpy((char *)&szDummy2[4], &CardData.ISO2Buff[3], 12);
		break;
	case TRANID_7380:
	case TRANID_4330:
	case TRANID_4340:
		nPinSize = 4;
		memcpy((char *)szDummy1, "04", 2);
		memcpy((char *)&szDummy1[2], Accept.PassWord, 4);
		memset((char *)&szDummy1[6], 'F', 10);

		memcpy((char *)&szDummy2, ZERO4, strlen(ZERO4));
		memcpy((char *)&szDummy2[4], &CardData.ISO2Buff[3], 12);
		break;
	default:
		nPinSize = 4;
		memcpy((char *)szDummy1, "04", 2);
		memcpy((char *)&szDummy1[2], Accept.PassWord, 4);
		memset((char *)&szDummy1[6], 'F', 10);

		memcpy((char *)&szDummy2, ZERO4, strlen(ZERO4));
		memcpy((char *)&szDummy2[4], &CardData.ISO2Buff[3], 12);
		break;
	}
	
	m_pDevCmn->fnUTL_JisPack(szDummy1, o_MASETER_KEY,16);
	m_pDevCmn->fnUTL_JisPack(szDummy2, o_WORKING_KEY,16);

	for(int j=0; j<8 ; j++)
	{
		o_PINBLOCK[j] = o_MASETER_KEY[j] ^ o_WORKING_KEY[j];
	}


	
	m_pDevCmn->ENCRYPT(e_PINBLOCK, o_PINBLOCK, 8, o_REAL_KEY);
	
	MakeUnPack(e_PINBLOCK, DesRPIN, sizeof(e_PINBLOCK));
//	m_pDevCmn->UnpackPinData(e_PINBLOCK, DesRPIN, sizeof(e_PINBLOCK));

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc037(%s)", DesRPIN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc037():return(%d)", nReturn);		


	return T_OK;
}

int CTranCmn::fnAPP_TranEnc039_039(int nTrCode)		//"경남은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc045_045(int nTrCode)		//"새마을금고  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc048_048(int nTrCode)		//"신협중앙회  "
{
	BYTE szDummy1[17] = "FFFFFFFFFFFFFFFF";
	BYTE szDummy2[17] = "0000000000000000";
	BYTE szPinSize[3] = "00";
	BYTE szValidData[13] = "            ";					// 암호화에 필요한 유효데이터 12 바이트
	BYTE szXorPindata[17] ="";
	BYTE szPinBlock[9] = "        ";
	BYTE szDummyWork[16] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86} ;
	BYTE szMemberNum[512] = {""};;

	BYTE szTempCard[17];
	BYTE szTempPass[17];
	BYTE szcardNum[9];
	BYTE szpassWord[9];
	BYTE szEncrypt[9];

//	unsigned char comp1[9] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x00}; //test key
//	unsigned char comp1[9] = {0xEA, 0x1F, 0xA2, 0xCC, 0x89, 0x84, 0x7E, 0xEF, 0x00}; //Real key

#ifdef CERTI_TEST_KEY000
	BYTE T_MASETER_KEY[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}; //test key
	BYTE o_MASETER_KEY[8] = {0xEA, 0x1F, 0xA2, 0xCC, 0x89, 0x84, 0x7E, 0xEF}; //Real key
#else														//  
	BYTE T_MASETER_KEY[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}; //test key
	BYTE o_MASETER_KEY[8] = {0xEA, 0x1F, 0xA2, 0xCC, 0x89, 0x84, 0x7E, 0xEF}; //Real key
#endif
	
// 	BYTE T_MASETER_KEY[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}; //test key
// 	BYTE o_MASETER_KEY[8] = {0xEA, 0x1F, 0xA2, 0xCC, 0x89, 0x84, 0x7E, 0xEF}; //Real key
	BYTE o_REAL_KEY[8] = {""};
	BYTE o_PINBLOCK[8] = {""};
	BYTE e_PINBLOCK[8] = {""};
	BYTE DesRPIN[17]   = {""};

	memset(szTempPass, 0x00, sizeof(szTempPass));		// paded "F"
	memset(szTempPass, 0x46, sizeof(szTempPass)-1);		// paded "F"
	memset(szTempCard, 0x00, sizeof(szTempCard) );		// 0000 + 계좌번호
	memset(szTempCard, 0x30, sizeof(szTempCard)-1);		// 0000 + 계좌번호

	memset(szcardNum,  0x00, sizeof(szcardNum));
	memset(szpassWord, 0x00, sizeof(szpassWord));
	memset(o_REAL_KEY, 0x00, sizeof(o_REAL_KEY));
	memset(szEncrypt,  0x00, sizeof(szEncrypt) );


	//BLOCK1
	memcpy(szTempPass, "04", 2 );
	memcpy(&szTempPass[2],Accept.PassWord, 4);
	m_pDevCmn->fnUTL_JisPack(szTempPass, szpassWord, 16);
	
	//BLOCK2
	memcpy(&szTempCard[4], pCardData3->AccountNo,12);		// 계좌번호
	m_pDevCmn->fnUTL_JisPack(szTempCard, szcardNum, 16);

	for (int i = 0; i < 8; i++)
	{
		o_PINBLOCK[i] = szcardNum[i] ^ szpassWord[i];
	}

	m_pDevCmn->ENCRYPT(e_PINBLOCK, o_PINBLOCK, 8, o_MASETER_KEY);
	
	MakeUnPack(e_PINBLOCK, DesRPIN, sizeof(e_PINBLOCK));

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc048_048(%s)", DesRPIN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc048_048():return");

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc050_050(int nTrCode)		//"상호저축은행"
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc054_054(int nTrCode)		//"HSBC은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc055_055(int nTrCode)		//"도이치은행  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc056_056(int nTrCode)		//"ABN은행     "
{

	return T_OK;
}


int CTranCmn::fnAPP_TranEnc105_105(int nTrCode)		//"KEB하나카드 "
{
	BYTE byTempCard[17];
	BYTE byTempPass[17];
	BYTE byCardNum[9];
	BYTE byPassWord[9];
	BYTE byEncrypt[9];
	BYTE DesRPIN [21];

	BYTE byComp1[8] = {0xc4, 0x38, 0xad, 0x6b, 0x9d, 0xcd, 0x31, 0x8c}; //둘다 사용 
	BYTE byComp2[8] = {0x92, 0xf1, 0x04, 0xdc, 0x58, 0x1f, 0x0b, 0x4f}; //

	memset(DesRPIN  , 0x00, sizeof(DesRPIN  ));

	//BLOCK1
	memset(byTempPass, 0x46, sizeof(byTempPass));	// paded 'F'
	byTempPass[16] = 0x00;
	memset(byTempCard, 0x30, sizeof(byTempCard) );
	byTempCard[16] = 0x00;
	memset(byEncrypt, 0x00, sizeof(byEncrypt) );	
	
	memcpy(byTempPass, "04" , 2 ) ;
	memcpy(&byTempPass[2], Accept.PassWord, 4)  ;

	long sLen = MakePack((BYTE *)&byTempPass[0], (BYTE *)&byPassWord[0], 16);

	//BLOCK2
	memcpy(&byTempCard[4], &CardData.ISO2Buff[3] , 12 ) ;

	MakePack((BYTE *)&byTempCard[0], (BYTE *)&byCardNum[0],  16);
	
	for (int i = 0; i < 8; i++)
	{
		byCardNum[i] = byCardNum[i] ^ byPassWord[i];
		byComp1[i] = byComp1[i] ^ byComp2[i];
	}

	//DES
	m_pDevCmn->ENCRYPT(byEncrypt, byCardNum, 8, byComp1);

	MakeUnPack(&byEncrypt[0], (BYTE *)&DesRPIN[0], 8);

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	return T_OK;
}


int CTranCmn::fnAPP_TranEnc105_115(int nTrCode)		//"외환해외카드"
{
	int  nEncLen = 0;
	int  nTrack2AccLen = 0;
	int  result = -1;
	int  i;
	BYTE szWKey3[17];
	BYTE szWKey4[17];
	BYTE szDRKey1[17];
	BYTE szDRKey2[17];
	BYTE szTemp1[17];
	BYTE szTemp2[17];
	BYTE szPasswd[17];
	BYTE szTrack2Account[20];							// 2014.12.31 C KYS  17->20
	BYTE szPinBlock[17];
	BYTE szEncPinBlock[17];
	BYTE DesRPIN [21];

	// Test Key
	BYTE szERKey1[17] = {0x5B, 0x5E, 0xB1, 0x5B, 0x3E, 0x8A, 0xEA, 0x7E};																	
	BYTE szERKey2[17] = {0xA4, 0x84, 0x2B, 0x27, 0x06, 0x13, 0x7F, 0xD3};
	BYTE szWKey1 [17] = {0x86, 0xE0, 0xBA, 0x3D, 0x34, 0x1C, 0xEF, 0xA8, 0x98, 0x91, 0xDF, 0x85, 0x49, 0xA8, 0x67, 0x19};
	BYTE szWKey2 [17] = {0x2D, 0x8B, 0xBE, 0x77, 0x03, 0x69, 0xCF, 0x2D, 0x50, 0x24, 0xEE, 0x30, 0xD1, 0xD7, 0xAC, 0x3F};

	// Real Key
//	BYTE szERKey1[17] = {0xA0, 0x9D, 0xA9, 0x9A, 0x72, 0x19, 0x0A, 0x42};																	
//	BYTE szERKey2[17] = {0x35, 0x23, 0x62, 0x83, 0x9C, 0x07, 0xC1, 0x1D};
//	BYTE szWKey1 [17] = {0xB0, 0x3B, 0xD3, 0x3B, 0xB3, 0x5D, 0x61, 0xA8, 0xE6, 0x2F, 0xE6, 0x45, 0x5D, 0xEC, 0x68, 0xBC};
//	BYTE szWKey2 [17] = {0xFA, 0x8E, 0xB4, 0xFF, 0x63, 0x0F, 0xD7, 0x6C, 0xBD, 0xFC, 0x3C, 0x12, 0x2B, 0x8B, 0x22, 0x3F};


	memset(szWKey3		  , 0x00, sizeof(szWKey1		));
	memset(szWKey4		  , 0x00, sizeof(szWKey2		));
	memset(szDRKey1		  , 0x00, sizeof(szDRKey1		));
	memset(szDRKey2		  , 0x00, sizeof(szDRKey2		));
	memset(szPasswd		  , 0x00, sizeof(szPasswd		));
	memset(szTrack2Account, 0x00, sizeof(szTrack2Account));
	memset(szPinBlock	  , 0x00, sizeof(szPinBlock		));
	memset(szEncPinBlock  , 0x00, sizeof(szEncPinBlock	));
	memset(DesRPIN		  , 0x00, sizeof(DesRPIN		));


	for(i=0; i<8; i++)
		szWKey3[i] = szWKey1[i] ^ szWKey2[i];
	for(i=0; i<8; i++)
		szWKey4[i] = szWKey1[i+8] ^ szWKey2[i+8];

	memset(szTemp1, 0x00, sizeof(szTemp1));
	memset(szTemp2, 0x00, sizeof(szTemp2));


	m_pDevCmn->DECRYPT(szTemp1, szERKey1, 8, szWKey3);
	m_pDevCmn->ENCRYPT(szTemp2, szTemp1 , 8, szWKey4);
	m_pDevCmn->DECRYPT(szDRKey1, szTemp2, 8, szWKey3);


	memset(szTemp1, 0x00, sizeof(szTemp1));
	memset(szTemp2, 0x00, sizeof(szTemp2));

	m_pDevCmn->DECRYPT(szTemp1, szERKey2, 8, szWKey3);
	m_pDevCmn->ENCRYPT(szTemp2, szTemp1 , 8, szWKey4);
	m_pDevCmn->DECRYPT(szDRKey2, szTemp2, 8, szWKey3);


	memset(szPasswd, 'F', sizeof(szPasswd));					// 'F'
	sprintf((char *)szPasswd, "0%1d", strlen(Accept.DesPassWord));
	memcpy(&szPasswd[2], Accept.DesPassWord, strlen(Accept.DesPassWord));
	szPasswd[16] = 0x00;

	for (i=0 ; i<20 ; i++)									
	{													
		if(CardData.ISO2Buff[i] == 0x20 || CardData.ISO2Buff[i] == 0x00)
			break;
   	 }
	
	nTrack2AccLen = i;
	memset(szTrack2Account, 0x20, sizeof(szTrack2Account));
	memcpy(szTrack2Account, "0000", 4);
	if(nTrack2AccLen > 12)
		memcpy(&szTrack2Account[4], &CardData.ISO2Buff[nTrack2AccLen - 12 - 1], 12);
	else
		memcpy(&szTrack2Account[4], &CardData.ISO2Buff[0], 12);
	szTrack2Account[16] = 0x00;

	MakePack(szPasswd, szTemp1, 16);
	MakePack(szTrack2Account, szTemp2, 16);

	for(i=0; i<8; i++)
		szPinBlock[i] = szTemp1[i] ^ szTemp2[i];

	memset(szTemp1, 0x00, sizeof(szTemp1));
	memset(szTemp2, 0x00, sizeof(szTemp2));
	

	m_pDevCmn->ENCRYPT(szTemp1, szPinBlock, 8, szDRKey1);
	m_pDevCmn->DECRYPT(szTemp2, szTemp1 , 8, szDRKey2);
	m_pDevCmn->ENCRYPT(szEncPinBlock, szTemp2, 8, szDRKey1);


	MakeUnPack(szEncPinBlock, DesRPIN, 8);

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	return T_OK;
}


int CTranCmn::fnAPP_TranEnc106_106(int nTrCode)		//"국민카드    "
{
	BYTE szTempCard[17];
	BYTE byCardNum [ 9];
	BYTE byEncrypt [ 9];
	BYTE DesRPIN[21] = {""};

#ifdef CERTI_TEST_KEY000
	//Test
//	BYTE szKey[8] = {0x5e, 0xfd, 0x10, 0x1a, 0x34, 0x1f, 0xa2, 0xfe};		// Test Key
	BYTE szKey[8] = {0x1c, 0x46, 0x1a, 0xf8, 0x31, 0xb5, 0x6d, 0xf4};		// Real Key
#else														//  
	//Real
//	BYTE szKey[8] = {0x5e, 0xfd, 0x10, 0x1a, 0x34, 0x1f, 0xa2, 0xfe};		// Test Key
	BYTE szKey[8] = {0x1c, 0x46, 0x1a, 0xf8, 0x31, 0xb5, 0x6d, 0xf4};		// Real Key
#endif


//	BYTE szKey[8] = {0x5e, 0xfd, 0x10, 0x1a, 0x34, 0x1f, 0xa2, 0xfe};		// Test Key
//	BYTE szKey[8] = {0x1c, 0x46, 0x1a, 0xf8, 0x31, 0xb5, 0x6d, 0xf4};		// Real Key


	memset(szTempCard, 0x00, sizeof(szTempCard));
	memset(byCardNum , 0x00, sizeof(byCardNum ));
	memset(byEncrypt , 0x00, sizeof(byEncrypt ));
	memset(DesRPIN   , 0x00, sizeof(DesRPIN	  ));
	

	// Password 
	memcpy(szTempCard, Accept.PassWord, 4) ;

	// TODO : Amax flag										// 아멕스 카드이면
	if(CardService == AMEXCARD)
	{
		memcpy(&szTempCard[4], &CardData.ISO2Buff[3],12);
	}
	else												// VISA 국민카드이면
	{
		memcpy(&szTempCard[4], &CardData.ISO2Buff[4],12);
	}
 	

	MakePack((BYTE *)&szTempCard[0], (BYTE *)&byCardNum[0], 16);

	//DES
	m_pDevCmn->ENCRYPT(byEncrypt, byCardNum, 8, szKey);
	
	MakeUnPack(&byEncrypt[0], DesRPIN, 8 );

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc107_107(int nTrCode)		//"수협카드    "
{
	BYTE byTempCard[17];
	BYTE byTempKey [17];
	BYTE byEncrypt [ 9];
	BYTE byCardNum [ 9];
	BYTE byKeyNum  [ 9];

	BYTE DesRPIN   [21];
	BYTE szTemp1   [32];
	BYTE szTemp2   [32];
	BYTE szTemp3   [32];

	// Test Key
	BYTE szERKey1  [16];																	
	BYTE szERKey2  [16];


	BYTE szWKey1   [16];
	BYTE szWKey2   [16];

	BYTE szDRKey1  [ 9];
	BYTE szDRKey2  [ 9];


	BYTE wKey      [32];		// 작업키 저장 변수
	BYTE hex_wKey  [ 9];	// 작업키 저장변수(pack 이후)


	memset( wKey	 , 0x00, sizeof(wKey	  ));
	memset( hex_wKey , 0x00, sizeof(hex_wKey  ));
	memset( szERKey1 , 0x00, sizeof(szERKey1  ));
	memset( szERKey2 , 0x00, sizeof(szERKey2  ));
	memset( szWKey1	 , 0x00, sizeof(szWKey1	  ));
	memset( szWKey2	 , 0x00, sizeof(szWKey2	  ));
	memset(byTempCard, 0x00, sizeof(byTempCard));				// 0000 + 계좌번호
	memset(byKeyNum	 , 0x00, sizeof(byKeyNum  ));					// paded "F"
	memset(byCardNum , 0x00, sizeof(byCardNum ));
	memset(byKeyNum  , 0x00, sizeof(byKeyNum  ));
	memset(byEncrypt , 0x00, sizeof(byEncrypt ));
	memset(DesRPIN   , 0x00, sizeof(DesRPIN   ));
	memset(szTemp1	 , 0x00, sizeof(szTemp1   ));
	memset(szTemp2	 , 0x00, sizeof(szTemp2   ));
	memset(szTemp3	 , 0x00, sizeof(szTemp3   ));



#ifdef CERTI_TEST_KEY000
	//Test Key
	memcpy(wKey, "2D1005F8A344CF39", 16);					
	MakePack((void *)&wKey[0], (void *)&szERKey1[0], 16);	

	memcpy(wKey, "1CFC18A4B8C41D3B", 16);					
	MakePack((void *)&wKey[0], (void *)&szERKey2[0], 16);	
#else														//  

	//Real Key
	memcpy(wKey, "B289BDE1CB347914", 16);					
	MakePack((void *)&wKey[0], (void *)&szERKey1[0], 16);	

	memcpy(wKey, "ED22D61EAF53978B", 16);					
	MakePack((void *)&wKey[0], (void *)&szERKey2[0], 16);	

#endif

	//Test Key
// 	memcpy(wKey, "2D1005F8A344CF39", 16);					
// 	MakePack((void *)&wKey[0], (void *)&szERKey1[0], 16);	
// 
// 	memcpy(wKey, "1CFC18A4B8C41D3B", 16);					
// 	MakePack((void *)&wKey[0], (void *)&szERKey2[0], 16);	

	
	
	memcpy(wKey, "9182736455463728", 16);					
	MakePack((void *)&wKey[0], (void *)&szWKey1[0], 16);	

	memcpy(wKey, "A1B21122C3D43344", 16);					
	MakePack((void *)&wKey[0], (void *)&szWKey2[0], 16);	


	m_pDevCmn->DECRYPT(szDRKey1, szERKey1,  8, szWKey1);
	m_pDevCmn->DECRYPT(szDRKey2, szERKey2,  8, szWKey2);
						
	//BLOCK1
	memcpy(byTempCard, "0000", 4 );								// "0000"
	memcpy(&byTempCard[4], &CardData.ISO2Buff[3], 12);			// 카드번호 12자리
	MakePack((BYTE *)byTempCard, (BYTE *)byCardNum, 16);

	
	//BLOCK2
	memcpy(byTempKey, "04", 2 );
	memcpy(&byTempKey[2], Accept.PassWord, 4);					// "04"+비밀번호 + "FFFFFFFFFF"
	memset(&byTempKey[6], 'F', 10);								// Fixed "F" (10)
	MakePack((BYTE *)byTempKey, (BYTE *)byKeyNum, 16);


	// XOR
	for (int i = 0; i < 8; i++)
	{
		byCardNum[i] = byCardNum[i] ^ byKeyNum[i];
	}

	m_pDevCmn->ENCRYPT(szTemp1, byCardNum, 8, szDRKey1);
	m_pDevCmn->DECRYPT(szTemp2, szTemp1,  8, szDRKey2);
	m_pDevCmn->ENCRYPT(szTemp3, szTemp2,  8, szDRKey1);

  	MakeUnPack(szTemp3, (BYTE *)DesRPIN, 8);

	memcpy(m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	return T_OK;
}

int CTranCmn::fnAPP_TranEnc111_111(int nTrCode)		//"농협비자    "
{
	BYTE szSData      [32];
	BYTE szTempPw     [32];
	BYTE szTempAcc    [32];
	BYTE szTempPass   [32];
	BYTE szTempCardNum[32];
	BYTE szTempData   [32];
	BYTE byPackKey[512];
	BYTE o_KEY1[8] = {0x85, 0x26, 0x07, 0x84, 0x07, 0x85, 0x89, 0x51};
	BYTE o_KEY2[8] = {0x7E, 0x0E, 0xA6, 0x32, 0x5A, 0x63, 0x44, 0xD8};
	BYTE o_KEY3[8] = {0x7E, 0x0E, 0xA6, 0x32, 0x5A, 0x63, 0x44, 0xD8};
	BYTE o_REAL_KEY[8] = {""};

	// TODO : Key값 읽어오기
	BYTE szKey		  [17];
	BYTE DesRPIN      [21];

	int  i		= 0;
	int	 resLen = 0;
	int	 pwLen  = 0;

	memset(szTempPw     , 0x00, sizeof(szTempPw     ));
	memset(szTempAcc    , 0x00, sizeof(szTempAcc    ));
	memset(szTempData   , 0x00, sizeof(szTempData   ));
	memset(szTempPass   , 0x00, sizeof(szTempPass   ));
	memset(szTempCardNum, 0x30, sizeof(szTempCardNum));
	memset(szSData      , 0x00, sizeof(szSData      ));
	memset(szKey		, 0x00, sizeof(szKey		));
	memset(byPackKey	, 0x00, sizeof(byPackKey	));
	memset(DesRPIN		, 0x00, sizeof(DesRPIN		));
	

#ifdef CERTI_TEST_KEY000
	//Test
	MakePack("D9B9D63D0D0BE0F2A22C08831301B092B9294068831F348F", byPackKey, strlen("D9B9D63D0D0BE0F2A22C08831301B092B9294068831F348F"));
#else														//  
	//Real
	MakePack("D9B9D63D0D0BE0F2A22C08831301B092B9294068831F348F", byPackKey, strlen("D9B9D63D0D0BE0F2A22C08831301B092B9294068831F348F"));
#endif



	memcpy(o_KEY1, byPackKey, 8);
	memcpy(o_KEY2, &byPackKey[8], 8);
	memcpy(o_KEY3, &byPackKey[8+8], 8);


	for(i=0; i< DES_LEN; i++)																
	{
		o_KEY2[i] ^= o_KEY1[i];
		o_KEY3[i] ^= o_KEY2[i];
	}																				


	memcpy(szTempPass, "04", 2);								
	memcpy(&szTempPass[2], Accept.PassWord, 4);					
	memset(&szTempPass[6], 0x46, 10);
	szTempPass[16] = 0x00;

	memcpy(szTempCardNum, "0000", 4);
	memcpy(&szTempCardNum[4], &CardData.ISO2Buff[3], 12);		
	szTempCardNum[16] = 0x00;

	
	pwLen = MakePack((BYTE *)szTempCardNum, (BYTE *)szTempAcc, 16);
	pwLen = MakePack((BYTE *)szTempPass   , (BYTE *)szTempPw , 16);


	for(i=0; i<pwLen; i++)
	{
		szTempData[i] = szTempPw[i] ^ szTempAcc[i];
	}
    
	//DES
	m_pDevCmn->ENCRYPT(szSData, szTempData, 8, o_KEY3);
 
	MakeUnPack(szSData, DesRPIN, 8);

	memcpy(m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	return T_OK;
}

int CTranCmn::fnAPP_TranEnc120_120(int nTrCode)		//"우리카드    "
{
	long sLen;
	BYTE byTempCard[17];
	BYTE byTempKey[17];
	BYTE byEncrypt[9];
	BYTE byCardNum[9];
	BYTE byKeyNum[9];
	BYTE DesRPIN [21];



#ifdef CERTI_TEST_KEY000
	//Test
//	BYTE byComp1[9] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x00}; //test key
	BYTE byComp1[9] = {0x69, 0x72, 0xc6, 0xab, 0xe7, 0x3d, 0xe5, 0xd1, 0x00}; //Real key
#else														//  
	//Real
	//BYTE byComp1[9] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x00}; //test key
	BYTE byComp1[9] = {0x69, 0x72, 0xc6, 0xab, 0xe7, 0x3d, 0xe5, 0xd1, 0x00}; //Real key
#endif

	// 다시 셋팅해 줘야 한다..
//	BYTE byComp1[9] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x00}; //test key
//	BYTE byComp1[9] = {0x69, 0x72, 0xc6, 0xab, 0xe7, 0x3d, 0xe5, 0xd1, 0x00}; //Real key
	BYTE byComp2[9];


	//BLOCK1
	memset(byTempCard, 0x00, sizeof(byTempCard));				// 0000 + 계좌번호
	memset(byTempCard, 0x30, sizeof(byTempCard)-1);				// 0000 + 계좌번호

	memset(byTempKey , 0x00, sizeof(byTempKey));				// paded "F"
	memset(byTempKey , 0x46, sizeof(byTempKey)-1);				// paded "F"

	memset(byCardNum , 0x00, sizeof(byCardNum));
	memset(byKeyNum  , 0x00, sizeof(byKeyNum ));
	memset(byComp2   , 0x00, sizeof(byComp2  ));
	memset(byEncrypt , 0x00, sizeof(byEncrypt));
	memset(DesRPIN   , 0x00, sizeof(DesRPIN  ));


	//BLOCK1
	memcpy(byTempCard, "0000", 4 );								// "0000"
	memcpy(&byTempCard[4], &CardData.ISO2Buff[3], 12);			// 카드번호 12자리
	sLen = MakePack((BYTE *)byTempCard, (BYTE *)byCardNum, 16);
	

	//BLOCK2
	memcpy(byTempKey, "04", 2 );
	memcpy(&byTempKey[2], Accept.PassWord, 4);					// "04"+비밀번호 + "FFFFFFFFFF"
	memset(&byTempKey[6], 'F', 10);	
	sLen = MakePack((BYTE *)byTempKey, (BYTE *)byKeyNum, 16);


	for (int i = 0; i < 8; i++)
	{
		byComp2[i] = byCardNum[i] ^ byKeyNum[i];
	}

	//DES
	m_pDevCmn->ENCRYPT(byEncrypt, byComp2, 8, byComp1);	

	MakeUnPack(byEncrypt, (BYTE *)DesRPIN, 8);

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc121_121(int nTrCode)		//"조흥카드    "
{
	BYTE szSData [32];
	BYTE szTemp1 [32];
	BYTE szTemp2 [32];
	BYTE szTemp3 [32];
	BYTE szPasswd[32];
	BYTE DesRPIN [21];

	int  CipherDataLen = 0;
	
	unsigned char szDRKey1[9] = {0xAA,0x07,0xFC,0x41,0xD1,0xAA,0xEE,0xF6,0x00};
	unsigned char szDRKey2[9] = {0x13,0xF2,0x2F,0x88,0x36,0x79,0x85,0xCF,0x00};


	memset(szTemp1 , 0x00, sizeof(szTemp1 ));
	memset(szTemp2 , 0x00, sizeof(szTemp2 ));
	memset(szTemp3 , 0x00, sizeof(szTemp3 ));
	memset(szSData , 0x00, sizeof(szSData ));
	memset(szPasswd, 0x00, sizeof(szPasswd));
	memset(DesRPIN , 0x00, sizeof(DesRPIN ));

	memcpy(szPasswd, Accept.PassWord, 4);
	memset(&szPasswd[4], 0x46, 12);

	MakePack((BYTE *)szPasswd, (BYTE *)szSData, 16);

	//DES
	m_pDevCmn->ENCRYPT(szTemp1, szSData, 8, szDRKey1);
	m_pDevCmn->DECRYPT(szTemp2, szTemp1, 8, szDRKey2);
	m_pDevCmn->ENCRYPT(szTemp3, szTemp2, 8, szDRKey1);

	MakeUnPack(szTemp3, (BYTE *)DesRPIN, 8);

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc126_192(int nTrCode)		//"신한카드    "
{
	BYTE szSData   [32];
	BYTE szTemp1   [32];
	BYTE szTemp2   [32];
	BYTE szTemp3   [32];
	BYTE szPasswd  [32];
	BYTE DesRPIN   [21];
	BYTE szTCard   [17];
	BYTE szCardN   [32];
	BYTE szXCard   [32];

	int  CipherDataLen = 0;
	
	unsigned char szDRKey1[9] = {0xe6,0x17,0x8c,0xe9,0x89,0xb4,0x8a,0x9d,0x00};
	unsigned char szDRKey2[9] = {0x2c,0x55,0x42,0xa9,0xda,0x21,0x7a,0xfb,0x00};


	memset(szTemp1 , 0x00, sizeof(szTemp1 ));
	memset(szTemp2 , 0x00, sizeof(szTemp2 ));
	memset(szTemp3 , 0x00, sizeof(szTemp3 ));
	memset(szSData , 0x00, sizeof(szSData ));
	memset(szPasswd, 0x00, sizeof(szPasswd));
	memset(szTCard , 0x30, sizeof(szTCard ));
	memset(szCardN , 0x00, sizeof(szCardN ));
	memset(szXCard , 0x00, sizeof(szXCard ));
	memset(DesRPIN , 0x00, sizeof(DesRPIN ));

	memcpy(szPasswd, "04", 2 );
	memcpy(&szPasswd[2], Accept.PassWord, 4);
	memset(&szPasswd[6], 0x46, 12);


	// TODO : Amax flag										// 아멕스 카드이면
	if(CardService == AMEXCARD)
	{
		memcpy(&szTCard[4], &pCardData2->MembershipNo[2],12);
	}
	else												// VISA 국민카드이면
	{
		memcpy(&szTCard[4], &pCardData2->MembershipNo[3],12);
	}

	MakePack((BYTE *)szTCard , (BYTE *)szCardN, 16);
	MakePack((BYTE *)szPasswd, (BYTE *)szSData, 16);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc126_192 : [szPasswd %6.6s] [szTCard %16.16s]" , szPasswd, szTCard);

	for( int i = 0; i < 8; i++ )
	{
		szXCard[i] = szCardN[i] ^ szSData[i];
	}


	//DES
	m_pDevCmn->ENCRYPT(szTemp1, szXCard, 8, szDRKey1);
	m_pDevCmn->DECRYPT(szTemp2, szTemp1, 8, szDRKey2);
	m_pDevCmn->ENCRYPT(szTemp3, szTemp2, 8, szDRKey1);


	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "szTemp1" , szTemp1, 8);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "szTemp2" , szTemp2, 8);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "szTemp3" , szTemp3, 8);

	MakeUnPack(szTemp3, (BYTE *)DesRPIN, 8);

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc126_192 : [%4.4s] [%16.16s]" , m_STXEncBuff.byOutLen, DesRPIN);


	return T_OK;
}

int CTranCmn::fnAPP_TranEnc127_127(int nTrCode)		//"씨티카드    "
{
	BYTE szTempPw  [   32];
	BYTE szTempAcc [   32];
	BYTE szTempData[   32];
	BYTE szTemp1   [   32];
	BYTE szTemp2   [   32];
	BYTE szTemp3   [   32];
	BYTE szWKey    [100+1];
	BYTE szWTempKey[ 16+1];
	BYTE szERKey   [100+1];
	BYTE szDRKey1  [ 16+1];																	
	BYTE szDRKey2  [ 16+1];
	BYTE keyKey1   [ 16+1];																	
	BYTE keyKey2   [ 16+1];
	BYTE byTempCard[   17];
	BYTE byTempPass[   17];
	BYTE DesRPIN   [   21];
	BYTE byGetKey  [  512];


	int  i, j		= 0;
	int	 resLen		= 0;
	int  keyLen		= 0;
	int  blockcnt	= 0;
	int	 pwLen		= 0;


	memset(szTempPw   , 0x00, sizeof(szTempPw  ));
	memset(szTempAcc  , 0x00, sizeof(szTempAcc ));
	memset(szTempData , 0x00, sizeof(szTempData));
	memset(szTemp1    , 0x00, sizeof(szTemp1   ));
	memset(szTemp2    , 0x00, sizeof(szTemp2   ));
	memset(szTemp3    , 0x00, sizeof(szTemp3   ));
	memset(szWKey     , 0x00, sizeof(szWKey    ));
	memset(szWTempKey , 0x00, sizeof(szWTempKey));
	memset(szERKey    , 0x00, sizeof(szERKey   ));
	memset(szDRKey1   , 0x00, sizeof(szDRKey1  ));
	memset(szDRKey2   , 0x00, sizeof(szDRKey2  ));
	memset(keyKey1    , 0x00, sizeof(keyKey1   ));
	memset(keyKey2    , 0x00, sizeof(keyKey2   ));
	memset(byTempCard , 0x00, sizeof(byTempCard));
	memset(byTempPass , 0x00, sizeof(byTempPass));
	memset(byGetKey   , 0x00, sizeof(byGetKey));


#ifdef CERTI_TEST_KEY000
	//Test
	MakePack("EDBF3C558CD2106A49B45E4257A0B95FA207B203A2FFE25C", (BYTE *)szWKey, strlen("EDBF3C558CD2106A49B45E4257A0B95FA207B203A2FFE25C"));
	MakePack("29BEE1D65249F1E923846CAE90F1BBEB", (BYTE *)byGetKey, strlen("29BEE1D65249F1E923846CAE90F1BBEB"));
#else														//  
	//Real
	MakePack("033CE7B5AD8A19681E137E878D745A03A207B203A2FFE25C", (BYTE *)szWKey, strlen("033CE7B5AD8A19681E137E878D745A03A207B203A2FFE25C"));
	MakePack("29BEE1D65249F1E923846CAE90F1BBEB", (BYTE *)byGetKey, strlen("29BEE1D65249F1E923846CAE90F1BBEB"));
#endif

//______________Make Key_______________________________________________

	//Test
// 	MakePack("EDBF3C558CD2106A49B45E4257A0B95FA207B203A2FFE25C", (BYTE *)szWKey, strlen("EDBF3C558CD2106A49B45E4257A0B95FA207B203A2FFE25C"));
// 	MakePack("29BEE1D65249F1E923846CAE90F1BBEB", (BYTE *)byGetKey, strlen("29BEE1D65249F1E923846CAE90F1BBEB"));
	
	//Real
// 	MakePack("033CE7B5AD8A19681E137E878D745A03A207B203A2FFE25C", (BYTE *)szWKey, strlen("033CE7B5AD8A19681E137E878D745A03A207B203A2FFE25C"));
// 	MakePack("29BEE1D65249F1E923846CAE90F1BBEB", (BYTE *)byGetKey, strlen("29BEE1D65249F1E923846CAE90F1BBEB"));

	memcpy(keyKey1, &byGetKey[0], 8);
	memcpy(keyKey2, &byGetKey[8], 8);


	keyLen   = strlen((char *)szWKey);
	blockcnt = (int)keyLen/8 +1;

	for(i=0; i<blockcnt; i++)
	{
		for(j=0; j<8; j++)
		{
			szWTempKey[j] = szWKey[j+(i*8)];
		}

		m_pDevCmn->DECRYPT(szTemp1, szWTempKey, 8, keyKey1);		
		m_pDevCmn->ENCRYPT(szTemp2, szTemp1, 8, keyKey2);
		m_pDevCmn->DECRYPT(szTemp3, szTemp2, 8, keyKey1);


		for(j=0; j<8; j++)
		{
			szERKey[j+(i*8)] = szTemp3[j];
		}
	}
		
	for(i=0; i<(8*2); i++)
	{
		if(i<8)
			szDRKey1[i] = szERKey[i];
		else
			szDRKey2[i-8] = szERKey[i];
	}

	//BLOCK1
	memcpy(byTempCard, "0000", 4 );										// "0000"
	memcpy(&byTempCard[4], &CardData.ISO2Buff[3], 12);					// 카드번호 12자리
	MakePack((BYTE *)byTempCard, (BYTE *)szTempAcc, 16);
	

	//BLOCK2
	memcpy(byTempPass, "04", 2 );
	memcpy(&byTempPass[2], Accept.PassWord, 4);							// "04"+비밀번호 + "FFFFFFFFFF"
	memset(&byTempPass[6], 0x46, 10);
	pwLen = MakePack((BYTE *)byTempPass, (BYTE *)szTempPw, 16);


	for(i=0; i<8; i++)
	{
		szTempData[i] = szTempPw[i] ^ szTempAcc[i];
	}


	//DES
	m_pDevCmn->ENCRYPT(szTemp1, szTempData, 8, szDRKey1);
	m_pDevCmn->DECRYPT(szTemp2, szTemp1, 8, szDRKey2);
	m_pDevCmn->ENCRYPT(szTemp3, szTemp2, 8, szDRKey1);
    
	MakeUnPack(szTemp3, (BYTE *)DesRPIN, 8);

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));


	return T_OK;
}
int CTranCmn::fnAPP_TranEnc134_034(int nTrCode)		//"광주카드    "
{
	char   strprimary_key[60];
	char   stroutbuff[1024];
 	char   strinbuff[1024];
	int	   sub_key;
	memset(&m_G_KEY_T, 0x00, sizeof(GETKEYTABLE));
	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));

	memset(strprimary_key,0x00,sizeof(strprimary_key));
	memset(strinbuff,0x00,sizeof(strinbuff));
	memset(stroutbuff,0x00,sizeof(stroutbuff));
	
	memset(&m_STXEncBuff.byPlaintMsdata[0], 0x00, sizeof(m_STXEncBuff.byPlaintMsdata));	  
	memset(&m_STXEncBuff.byPlaintMsdata[0], 0x20, 104);	  
	if(MenuAvail & MENU_MCU2)
	{
		memcpy(&m_STXEncBuff.byPlaintMsdata[0], CardData.ISO2Buff, CardData.ISO2size);		
		memcpy(&m_STXEncBuff.byPlaintMsdata[104], Accept.PassWord, 4);
	}
	else
	if(MenuAvail & MENU_MCU3)
	{
		memcpy(&m_STXEncBuff.byPlaintMsdata[0], CardData.ISO3Buff, CardData.ISO3size);		
		memcpy(&m_STXEncBuff.byPlaintMsdata[104], Accept.PassWord, 4);
	}
	else
	if(MenuAvail & MENU_PB)
	{
		memcpy(&m_STXEncBuff.byPlaintMsdata[0], &PbMsData.sBuf[0], PbMsData.Len);		
		memcpy(&m_STXEncBuff.byPlaintMsdata[104], Accept.PassWord, 4);
	}
		
	
	sprintf(m_G_KEY_T.m_key, "%s", IniGetStr(_ATMKEYTABELE_INI, KJB_ORG_SEC, "MASTER", "4155544f4d414348494e45434f4f5250"));
	sprintf(m_G_KEY_T.s_key, "%s", IniGetStr(_ATMKEYTABELE_INI, KJB_ORG_SEC, "SLAVE0", "00"));
	sub_key = Asc2Int(m_G_KEY_T.s_key, 2);
	MakePack(m_G_KEY_T.m_key, strprimary_key, 32);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc134_034",m_STXEncBuff.byPlaintMsdata, 120 );
	SeedKJB(strprimary_key, sub_key, m_STXEncBuff.byPlaintMsdata, m_STXEncBuff.byChiperMsdata, SEND_ENCRYPT);
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc134_034",m_STXEncBuff.byChiperMsdata, 120 );

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc134_034() nTranCode = [%04d]", nTrCode);	

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc135_135(int nTrCode)		//"제주비자    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc137_037(int nTrCode)		//"전북카드    "
{
	BYTE szDummy1[17] = "FFFFFFFFFFFFFFFF";
	BYTE szDummy2[17] = "0000000000000000";
	BYTE szPinSize[3] = "00";
	BYTE szValidData[13] = "            ";					// 암호화에 필요한 유효데이터 12 바이트
	BYTE szXorPindata[17] ="";
	BYTE szPinBlock[9] = "        ";
	BYTE szDummyWork[16] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86} ;
	BYTE szMemberNum[512] = {""};;
	BYTE o_MASETER_KEY[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_WORKING_KEY[8] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86};
	BYTE o_REAL_KEY[8] = {""};;
	BYTE o_REAL_KEY_LEFT[8] =  {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_REAL_KEY_RIGHT[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_PINBLOCK[8] = {""};
	BYTE e_PINBLOCK[8] = {""};
	BYTE DesRPIN[21] = {""};


	int	 nPinSize = 0;
	int	 nReturn = FALSE;

//MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc053(%s %d %s %d %s)", vPinNum, PinSize, vMemberNum, MemberNumSize, vWorkingKey);


	memset(DesRPIN, 0, sizeof(DesRPIN));						

// 	CString strTmp("");
// 	strTmp = IniGetStr(_ATMKEYTABELE_INI, JEOBANKID, "MASTER", "CD7114BA459953EF" );
// 	memcpy(szDummyWork, strTmp,__min(sizeof(o_REAL_KEY)*2, strlen(strTmp)));

	memcpy(szDummyWork, "CD7114BA459953EF", strlen("CD7114BA459953EF"));

	m_pDevCmn->fnUTL_JisPack(szDummyWork, o_REAL_KEY, sizeof(o_REAL_KEY)*2);

	switch(nTrCode)
	{
	case TRANID_4350:
	case TRANID_7250:
	case TRANID_7230:
		nPinSize = 4;
		memcpy((char *)szDummy1, "04", 2);
		memcpy((char *)&szDummy1[2], Accept.PassWord, 4);
		memset((char *)&szDummy1[6], 'F', 10);

		memcpy((char *)&szDummy2, ZERO4, strlen(ZERO4));
		memcpy((char *)&szDummy2[4], &CardData.ISO2Buff[3], 12);
		break;
	case TRANID_7380:
	case TRANID_4330:
	case TRANID_4340:
		nPinSize = 4;
		memcpy((char *)szDummy1, "04", 2);
		memcpy((char *)&szDummy1[2], Accept.PassWord, 4);
		memset((char *)&szDummy1[6], 'F', 10);

		memcpy((char *)&szDummy2, ZERO4, strlen(ZERO4));
		memcpy((char *)&szDummy2[4], &CardData.ISO2Buff[3], 12);
		break;
	default:
		nPinSize = 4;
		memcpy((char *)szDummy1, "04", 2);
		memcpy((char *)&szDummy1[2], Accept.PassWord, 4);
		memset((char *)&szDummy1[6], 'F', 10);

		memcpy((char *)&szDummy2, ZERO4, strlen(ZERO4));
		memcpy((char *)&szDummy2[4], &CardData.ISO2Buff[3], 12);
		break;
	}
	
	m_pDevCmn->fnUTL_JisPack(szDummy1, o_MASETER_KEY,16);
	m_pDevCmn->fnUTL_JisPack(szDummy2, o_WORKING_KEY,16);

	for(int j=0; j<8 ; j++)
	{
		o_PINBLOCK[j] = o_MASETER_KEY[j] ^ o_WORKING_KEY[j];
	}

	
	m_pDevCmn->ENCRYPT(e_PINBLOCK, o_PINBLOCK, 8, o_REAL_KEY);
	
	MakeUnPack(e_PINBLOCK, DesRPIN, sizeof(e_PINBLOCK));
//	m_pDevCmn->UnpackPinData(e_PINBLOCK, DesRPIN, sizeof(e_PINBLOCK));

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc137_037(%s)", DesRPIN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc137_037():return(%d)", nReturn);		

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc181_105(int nTrCode)		//"KEB하나카드 "
{

	BYTE byTempCard[17];
	BYTE byTempPass[17];
	BYTE byCardNum[9];
	BYTE byPassWord[9];
	BYTE byEncrypt[9];
	BYTE DesRPIN [21];

	BYTE byComp1[8] = {0xc4, 0x38, 0xad, 0x6b, 0x9d, 0xcd, 0x31, 0x8c}; //둘다 사용 
	BYTE byComp2[8] = {0x92, 0xf1, 0x04, 0xdc, 0x58, 0x1f, 0x0b, 0x4f}; //

	memset(DesRPIN  , 0x00, sizeof(DesRPIN  ));

	//BLOCK1
	memset(byTempPass, 0x46, sizeof(byTempPass));	// paded 'F'
	byTempPass[16] = 0x00;
	memset(byTempCard, 0x30, sizeof(byTempCard) );
	byTempCard[16] = 0x00;
	memset(byEncrypt, 0x00, sizeof(byEncrypt) );	
	
	memcpy(byTempPass, "04" , 2 ) ;
	memcpy(&byTempPass[2], Accept.PassWord, 4)  ;
	long sLen = MakePack((BYTE *)&byTempPass[0], (BYTE *)&byPassWord[0], 16);

	//BLOCK2
	memcpy(&byTempCard[4], &CardData.ISO2Buff[3] , 12 ) ;

	sLen = MakePack((BYTE *)&byTempCard[0], (BYTE *)&byCardNum[0],  16);
	
	for (int i = 0; i < 8; i++)
	{
		byCardNum[i] = byCardNum[i] ^ byPassWord[i];
		byComp1[i] = byComp1[i] ^ byComp2[i];
	}

	//DES
	m_pDevCmn->ENCRYPT(byEncrypt, byCardNum, 8, byComp1);

	MakeUnPack(&byEncrypt[0], (BYTE *)&DesRPIN[0], 8);

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc181_105(%s)", DesRPIN);	

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc184_184(int nTrCode)		//"삼성카드    "
{	
	BYTE byPasswd[10];
	BYTE byKey   [17];

	//TODO
/*
	fp = fopen("c:\\samEnc.log", "a+");
	if(fp != NULL)
	{
		len = sprintf(temp, "\nPwd=[  %.4s]\nKey=[%.6s]", &sData[0], &mKey[0]);
		fwrite(temp, 1, len, fp);
	}
*/

	memset(byPasswd, 0x00, sizeof(byPasswd));
	memset(byKey   , 0x00, sizeof(byKey   ));

	// Send Time
	memcpy(byKey, &m_pProfile->TRANS.YYYYMMDDHHMMSS[8], 6);

	memcpy(byPasswd, Accept.PassWord, 4) ;

	// Make Encrypt Or Decrypt
	int d1,d2,d3,d4;

	d1 = ((byKey[2] & 0x0f) + (byPasswd[0] & 0x0f)) % 10;
	d2 = ((byKey[3] & 0x0f) + (byPasswd[1] & 0x0f)) % 10;
	d3 = ((byKey[4] & 0x0f) + (byPasswd[2] & 0x0f)) % 10;
	d4 = ((byKey[5] & 0x0f) + (byPasswd[3] & 0x0f)) % 10;

	sprintf((char *)&byPasswd[0], "%01d%01d%01d%01d", d1,d2,d3,d4);

	memcpy(&m_STXEncBuff.byChiperMsdata, byPasswd, 4);
	memcpy(&m_STXEncBuff.byChiperMsdata[4], SPACE12, 12);

	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc184_184 m_STXEncBuff.byChiperMsdata", m_STXEncBuff.byChiperMsdata,16);
	return T_OK;
}
int CTranCmn::fnAPP_TranEnc192_192(int nTrCode)		//"신한카드    "
{
	BYTE szSData [32];
	BYTE szTemp1 [32];
	BYTE szTemp2 [32];
	BYTE szTemp3 [32];
	BYTE szPasswd[32];
	BYTE DesRPIN [21];

	int  CipherDataLen = 0;
	
	unsigned char szDRKey1[9] = {0xAA,0x07,0xFC,0x41,0xD1,0xAA,0xEE,0xF6,0x00};
	unsigned char szDRKey2[9] = {0x13,0xF2,0x2F,0x88,0x36,0x79,0x85,0xCF,0x00};


	memset(szTemp1 , 0x00, sizeof(szTemp1 ));
	memset(szTemp2 , 0x00, sizeof(szTemp2 ));
	memset(szTemp3 , 0x00, sizeof(szTemp3 ));
	memset(szSData , 0x00, sizeof(szSData ));
	memset(szPasswd, 0x00, sizeof(szPasswd));
	memset(DesRPIN , 0x00, sizeof(DesRPIN ));

	memcpy(szPasswd, Accept.PassWord, 4);
	memset(&szPasswd[4], 0x46, 12);

	MakePack((BYTE *)szPasswd, (BYTE *)szSData, 16);

	//DES
	m_pDevCmn->ENCRYPT(szTemp1, szSData, 8, szDRKey1);
	m_pDevCmn->DECRYPT(szTemp2, szTemp1, 8, szDRKey2);
	m_pDevCmn->ENCRYPT(szTemp3, szTemp2, 8, szDRKey1);

	MakeUnPack(szTemp3, (BYTE *)DesRPIN, 8);

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc193_193(int nTrCode)		//"현대카드    "
{
	BYTE byTempCard[17];
	BYTE byTempPass[17];
	BYTE byCardNum[9];
	BYTE byPassWord[9];
	BYTE byEncrypt[9];
	BYTE DesRPIN [21];

	BYTE byComp1[8] = {0x61, 0xB0, 0x52, 0xC2, 0xD9, 0xB0, 0xD0, 0x19}; //real, test key

	memset(DesRPIN  , 0x00, sizeof(DesRPIN  ));

	//BLOCK1
	memset(byTempPass, 0x46, sizeof(byTempPass));	// paded "F"
	byTempPass[16] = 0x00;

	memset(byTempCard, 0x30, sizeof(byTempCard) );  // 0000 + 계좌번호
	byTempCard[16] = 0x00;

	memset(byEncrypt, 0x00, sizeof(byEncrypt) );


	memcpy(byTempPass, "04" , 2 ) ;
	memcpy(&byTempPass[2], Accept.PassWord, 4)  ;
	long sLen = MakePack((BYTE *)&byTempPass[0], (BYTE *)&byPassWord[0], 16);
	

	//BLOCK2
	//TODO : bDynasty
	if(CardService == DINERS)
		memcpy(&byTempCard[4], &CardData.ISO2Buff[1] , 12 );
	else
		memcpy(&byTempCard[4], &CardData.ISO2Buff[3], 12 );

	sLen = MakePack((BYTE *)&byTempCard[0], (BYTE *)&byCardNum[0], 16);

	for (int i = 0; i < 8; i++)
	{
		byCardNum[i] = byCardNum[i] ^ byPassWord[i];
	}
	

	//DES 
	m_pDevCmn->ENCRYPT(byEncrypt, byCardNum, 8, byComp1);
	
	MakeUnPack(&byEncrypt[0], (BYTE *)&DesRPIN[0], 8);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc193_193 ( DesRPIN %16.16s)", DesRPIN);	
	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc193_193( byChiperMsdata %16.16s)", m_STXEncBuff.byChiperMsdata);	

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc194_194(int nTrCode)		//"BC카드      "
{
	//#CS0003
	static struct WkPin_Auth 
	{

		   char   c_data_11     [32+1];

		   char   c_data_21     [12+1];
		   char   c_data_22     [12+1];
		   char   c_data_23     [ 8+1];

		   double   l_data_31; // 
		   double   l_data_32;
		   double   l_data_33;

		   double f_data_41;
		   double f_data_42;
		   double f_data_43;
		   double f_data_44;

		   long   l_data_51;
		   long   l_data_52;
		   long   l_data_53;
		   long   l_data_54;

		   long   l_data_61;
		   char   c_data_62    [   7];
		   char   c_data_63    [   8];

	} Pin;


	static char acq_id         [ 6+1];
	static char send_date_time [10+1];
	static char card_no        [16+1];
	static char temp_4         [ 4+1];
	static char mf_pin         [16+1];
	static char sys_date       [ 8+1];
	static char sys_time       [ 6+1];

	char temp[1000];
	int len;

	BYTE DesRPIN [21];
	memset(DesRPIN  , 0x00, sizeof(DesRPIN  ));

	int   i_auth_amt;
	char  c_card_no2[16+1];
	char  c_eff_period[4+1];
	char  c_d[4+1];
	char  c_t[6+1];

	//TODO : 카드번호, 유효기간, 날짜, 시간, 금액 확인 필요
	memcpy((BYTE *)&c_card_no2[0], &pCardData2->MembershipNo[0], 16);	// 카드번호 20030716
	c_card_no2[16] = 0x00;

	memcpy(temp_4, Accept.PassWord, 4);					// 비밀번호
	temp_4[4] = 0x00;

	memcpy((BYTE *)&c_eff_period[0], pCardData2->EndDay, 4);	// 유효기간 20030716
	c_eff_period[4] = 0x00;

	memcpy(sys_date, GetDate().GetBuffer(0), 8);					// 날짜
	sys_date[8] = 0x00;

	memcpy(sys_time, GetTime().GetBuffer(0), 6);					// 시간
	sys_time[6] = 0x00;


	//TODO
	i_auth_amt         =  Asc2Int(Accept.Money, 15);				// 금액

	strncpy(c_d,  &sys_date[4], 4);        c_d[4] = 0x00;	// MMDD
	strncpy(c_t,   sys_time,    6);        c_t[6] = 0x00;	// HHMMSS

	strcpy(acq_id, "940000");
	strncpy(send_date_time,       c_d, 4);
	strncpy(&send_date_time[4],   c_t, 6);
	send_date_time[10] = 0x00;
	strncpy(card_no, (char *)c_card_no2, 16);
	card_no[16] = 0x00;

	memset(&Pin, 0x00, sizeof(Pin));
	memcpy(&Pin.c_data_11[0],   acq_id, 6);					// 기관번호(6)
	memcpy(&Pin.c_data_11[6],   send_date_time, 10);		// MMDDHHMMSS(10)
	memcpy(&Pin.c_data_11[16],  card_no, 16);				// 카드번호(16)
	strncpy(Pin.c_data_21,  Pin.c_data_11, 12);			// 기관번호(6)+MMDDHH(6)
	strncpy(Pin.c_data_22, &Pin.c_data_11[12], 12);		// MMSS(4)+카드번호(8)
	strncpy(Pin.c_data_23, &Pin.c_data_11[24],  8);		// 카드번호(8)


	Pin.l_data_31  = atof(Pin.c_data_21);
	Pin.l_data_32  = atof(Pin.c_data_22);
	Pin.l_data_33  = atof(Pin.c_data_23);
	Pin.f_data_41   = Pin.l_data_31;
	Pin.f_data_42   = Pin.l_data_32;
	Pin.f_data_43   = sqrt(Pin.f_data_41);
	Pin.f_data_44   = sqrt(Pin.f_data_42);
	Pin.l_data_51   = (long)Pin.f_data_43;
	Pin.l_data_52   = (long)Pin.f_data_44;
	Pin.l_data_53   = (long)(Pin.f_data_41 - (Pin.l_data_51 * Pin.l_data_51));
	Pin.l_data_54   = (long)(Pin.f_data_42 - (Pin.l_data_52 * Pin.l_data_52));
	Pin.l_data_61   =  Pin.l_data_51 + Pin.l_data_52 - Pin.l_data_53 + Pin.l_data_54 + atoi(temp_4);

	if  (Pin.l_data_61   >=  0) 
	{

		len = sprintf(temp, "%ld", (long)Pin.l_data_61);	// 20030716
		if(len > sizeof(Pin.c_data_62))
			len = sizeof(Pin.c_data_62);
		memset((char *)&Pin.c_data_62[0], 0x30, sizeof(Pin.c_data_62));
		memcpy((char *)&Pin.c_data_62[sizeof(Pin.c_data_62)-len], temp, len);


		strncpy(Pin.c_data_63, Pin.c_data_62, 7);
		strncpy(&Pin.c_data_63[7], "C", 1);
	}
	else 
	{
		Pin.l_data_61    =  Pin.l_data_61 * -1;

		len = sprintf(temp, "%uld", (long)Pin.l_data_61);
		if(len > sizeof(Pin.c_data_62))
			len = sizeof(Pin.c_data_62);
		memset((char *)&Pin.c_data_62[0], 0x30, sizeof(Pin.c_data_62));
		memcpy((char *)&Pin.c_data_62[sizeof(Pin.c_data_62)-len], temp, len);

		strncpy(Pin.c_data_63, Pin.c_data_62, 7);
		strncpy(&Pin.c_data_63[7], "D", 1);
	}


	strncpy(mf_pin,  Pin.c_data_63,  8);

	mf_pin[ 8] = 0x30;
	mf_pin[ 9] = 0x30;
	mf_pin[10] = 0x30;
	mf_pin[11] = 0x30;
	mf_pin[12] = 0x30;
	mf_pin[13] = 0x30;
	mf_pin[14] = 0x30;
	mf_pin[15] = 0x30;
	mf_pin[16] = 0x00;
	
	// TODO
	memset(DesRPIN, 0x00, sizeof(DesRPIN) );
	memcpy(DesRPIN, &mf_pin[0], 17);

	memcpy(m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc195_195(int nTrCode)		//"롯데카드    "
{
	 BYTE byTempCard[17];
	 BYTE byTempPass[17];
	 BYTE byCardNum [ 9];
	 BYTE byPassWord[ 9];
	 BYTE byEncrypt [ 9];
	 BYTE byEncrypt2[ 9];
	 BYTE byEncrypt3[ 9];
	 BYTE WorkingKey[ 9];
	 BYTE DesRPIN   [21];

	 BYTE Keycomp1[9] = {0x6D,0xE5,0x9B,0x2F,0xAB,0x9B,0x54,0xFB,0x00};		// 암호화된 workingKey를 풀기위한 값1
	 BYTE Keycomp2[9] = {0x31,0xFB,0xAD,0xA1,0x29,0x4C,0xD5,0xB9,0x00};		// 암호화된 workingKey를 풀기위한 값2
	 BYTE Keycomp3[9] = {0xAD,0xC4,0x62,0xC2,0x3D,0x98,0xEC,0xE3,0x00};		// 암호화된 workingKey를 풀기위한 값3
	 BYTE Keycomp4[9] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};		// keycomp1,2을 XOR 한 값.
	 BYTE Keycomp5[9] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};		// keycomp2,3을 XOR 한 값.

	 BYTE EncWorkingKey[9] = {0xCB,0x09,0xF4,0xDE,0xD2,0x0C,0x7F,0xB3,0x00};// 암호화된 workingKey
//	 BYTE RealWorkingKey[9] = {0x63,0x48,0x51,0x4D,0xAE,0x92,0x16,0x51,0x00}; // 리얼키 넣을 위치
//	 BYTE RealWorkingKey[9] = {0x44,0x0A,0xDB,0x11,0x67,0x5F,0x22,0xB2,0x00}; // TEST 리얼키 넣을 위치 


#ifdef CERTI_TEST_KEY000
	//Test
	 BYTE RealWorkingKey[9] = {0x44,0x0A,0xDB,0x11,0x67,0x5F,0x22,0xB2,0x00}; // TEST 리얼키 넣을 위치 
#else														//  
	//Real
	 BYTE RealWorkingKey[9] = {0x63,0x48,0x51,0x4D,0xAE,0x92,0x16,0x51,0x00}; // 리얼키 넣을 위치
#endif

	// Working Key 키복호화
	for(int i=0; i<8; i++)
		Keycomp4[i] = Keycomp1[i] ^ Keycomp2[i];
	for(i=0; i<8; i++)
		Keycomp5[i] = Keycomp3[i] ^ Keycomp4[i];


	memset(WorkingKey, 0x00, sizeof(WorkingKey) );

	//DES
	m_pDevCmn->DECRYPT(WorkingKey, EncWorkingKey, 8, Keycomp5);


	//BLOCK1
	memset(byTempPass, 0x46, sizeof(byTempPass));	// paded "F"
	byTempPass[16] = 0x00;
	memset(byTempCard, 0x30, sizeof(byTempCard) );  // 0000 + 계좌번호
	byTempCard[16] = 0x00;
	memset(byCardNum, 0x00, sizeof(byCardNum) );
	memset(byPassWord, 0x00, sizeof(byPassWord) );
	memset(byEncrypt, 0x00, sizeof(byEncrypt) );
	memset(byEncrypt2, 0x00, sizeof(byEncrypt2) );
	memset(byEncrypt3, 0x00, sizeof(byEncrypt3) );


	memcpy(byTempPass, "04" , 2 ) ;
	memcpy(&byTempPass[2], Accept.PassWord, 4)  ;


	byTempPass[16]=0x00;
	long sLen = MakePack((BYTE *)&byTempPass[0], (BYTE *)&byPassWord[0], 16);
	

	//BLOCK2
	// TODO : Amax Flag
	if(CardService == AMEXCARD)
	{
		memcpy(&byTempCard[4], &CardData.ISO2Buff[2], 12);			// 카드번호 3th ~ 14th (12개)
	}
	else							// VISA 현대카드이면
	{
		memcpy(&byTempCard[4], &CardData.ISO2Buff[3], 12);			// 카드번호 4th ~ 15th (12개)
	}


	byTempCard[16]=0x00;
	sLen = MakePack((BYTE *)&byTempCard[0], (BYTE *)&byCardNum[0], 16); // 16자리


	for (i = 0; i < 8; i++)
	{
		byCardNum[i] = byCardNum[i] ^ byPassWord[i];
	}
	
	//DES
	m_pDevCmn->ENCRYPT(byEncrypt , byCardNum , 8, WorkingKey    );
	m_pDevCmn->DECRYPT(byEncrypt2, byEncrypt , 8, RealWorkingKey);
	m_pDevCmn->ENCRYPT(byEncrypt3, byEncrypt2, 8, WorkingKey    );

	MakeUnPack(&byEncrypt3[0], (BYTE *)&DesRPIN[0], 8);

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	return T_OK;
}

int CTranCmn::fnAPP_TranEnc209_209(int nTrCode)		//"유안타증권	 "
{

	BYTE szDesPWD[16];
	int	 nOutlen;
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc209_209()");		

	memset(&m_STXEncBuff, 0x00, sizeof(m_STXEncBuff));
	memset(szDesPWD,     0x00,  sizeof(szDesPWD));

	memcpy(szDesPWD, Accept.PassWord, sizeof(Accept.PassWord));
	DY_EncryptSeed(szDesPWD, 4, (BYTE *)m_STXEncBuff.byChiperMsdata, &nOutlen);
	Int2Asc(nOutlen, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));	
//	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[fnAPP_TranEnc209_209]", m_TXENCDATA.byChiperMsdata, nOutlen);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc209_209() Return");

	return T_OK;
}

int CTranCmn::fnAPP_TranEnc230_230(int nTrCode)		//"미래에증권  "
{

	return T_OK;
}


int CTranCmn::fnAPP_TranEnc243_243(int nTrCode)		//"한국투자    "
{
	BYTE szDummy1[17] = "FFFFFFFFFFFFFFFF";
	BYTE szDummy2[17] = "0000";
	BYTE szPinSize[3] = "00";
	BYTE szValidData[13] = "            ";					// 암호화에 필요한 유효데이터 12 바이트
	BYTE szXorPindata[17] ="";
	BYTE szPinBlock[512] = {""};
	/*
	BYTE szDummyWork[512] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86} ;
	BYTE szMemberNum[512] = {""};
	BYTE o_MASETER_KEY[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_WORKING_KEY[8] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86};
	BYTE o_REAL_KEY[8] = {""};
	BYTE o_REAL_KEY_LEFT[8] =  {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_REAL_KEY_RIGHT[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	*/
	BYTE szDummyWork[512] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86} ;
	BYTE szMemberNum[512] = {""};
	BYTE o_MASETER_KEY0[16] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_MASETER_KEY1[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_WORKING_KEY0[16] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86};
	BYTE o_WORKING_KEY1[8] = {0xB6, 0x14, 0x0B, 0x63, 0x6B, 0xBE, 0x7A, 0x86};
	BYTE o_REAL_KEY[8] = {""};
	BYTE o_REAL_KEY_LEFT[8] =  {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_REAL_KEY_RIGHT[8] = {0x85, 0x08, 0xC8, 0xAE, 0xF1, 0x9B, 0x92, 0xAE};
	BYTE o_PINBLOCK[8] = {""};
	BYTE e_PINBLOCK[8] = {""};
	BYTE DesRPIN[17] = {""};

	int	 nPinSize = 0;
	int	 nReturn = FALSE;


#ifdef CERTI_TEST_KEY000
	//Test
	//ctkeyo.enc
	MakePack("FABE163C8BE4CC4ABA52749A680F1DC1", o_WORKING_KEY0, strlen("FABE163C8BE4CC4ABA52749A680F1DC1"));

	//ctkey.enc
	MakePack("12345678901234560000000000000000", o_MASETER_KEY0, strlen("12345678901234560000000000000000"));
#else														//  
	//Real
	//ctkeyo.enc
	MakePack("1E7DD43AE9FAF14CE1A9AED967A3CFDD", o_WORKING_KEY0, strlen("1E7DD43AE9FAF14CE1A9AED967A3CFDD"));

	//ctkey.enc
	MakePack("12345678901234560000000000000000", o_MASETER_KEY0, strlen("12345678901234560000000000000000"));
#endif





	m_pDevCmn->DECRYPT(o_REAL_KEY_LEFT , &o_WORKING_KEY0[0], 8, &o_MASETER_KEY0[0]); 
	m_pDevCmn->DECRYPT(o_REAL_KEY_RIGHT, &o_WORKING_KEY0[8], 8, &o_WORKING_KEY0[8]); 
	


	memset(DesRPIN, 0, sizeof(DesRPIN));						
	nPinSize = sizeof(Accept.PassWord);
	


//	T_DES_DECRYPTOR(o_WORKING_KEY, DES_LEN, o_REAL_KEY_LEFT, o_REAL_KEY_RIGHT, o_REAL_KEY_LEFT, o_REAL_KEY);
	 
//	memcpy(o_REAL_KEY_LEFT, o_REAL_KEY, DES_LEN);

	szPinSize[1] |= (UINT)nPinSize;				// SIZE
	memcpy((char *)szDummy1, (char *)szPinSize, 2);
	memcpy((char *)&szDummy1[2], (char *)Accept.PassWord, nPinSize);
	memcpy(szXorPindata, szDummy1, sizeof(szDummy1));
	//memcpy(&szXorPindata[4], pCardData3->AccountNo, 12);
	MakePack(szXorPindata,o_PINBLOCK,16);


	m_pDevCmn->T_DES_ENCRYPTOR(o_PINBLOCK, DES_LEN, o_REAL_KEY_LEFT, o_REAL_KEY_RIGHT, o_REAL_KEY_LEFT, e_PINBLOCK);
	
	MakeUnPack(e_PINBLOCK, DesRPIN, sizeof(e_PINBLOCK));

	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc053(%s)", DesRPIN);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc053():return(%d)", nReturn);	


	return T_OK;
}

int CTranCmn::fnAPP_TranEnc261_261(int nTrCode)		//"교보증권    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc262_262(int nTrCode)		//"하이투자    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc263_263(int nTrCode)		//"HMC투자     "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc264_264(int nTrCode)		//"키움증권    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc265_265(int nTrCode)		//"이트레이드  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc266_266(int nTrCode)		//"에스케이    "
{

	return T_OK;
}

int CTranCmn::fnAPP_TranEnc267_267(int nTrCode)		//"대신증권    "
{

	BYTE szDesPWD[16];
	int	 nOutlen;

	memset(&m_STXEncBuff, 0x00, sizeof(m_STXEncBuff));
	memset(szDesPWD,     0x00,  sizeof(szDesPWD));

	memcpy(szDesPWD, Accept.PassWord, sizeof(Accept.PassWord));
	DS_EncryptSeed(szDesPWD, 4, (BYTE *)m_STXEncBuff.byChiperMsdata, &nOutlen);
	Int2Asc(nOutlen, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));	
//	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "[fnAPP_TranEnc267_267]", m_TXENCDATA.byChiperMsdata, nOutlen);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc267_267() Return");	
	
	return T_OK;
}
int CTranCmn::fnAPP_TranEnc268_268(int nTrCode)		//"솔로몬투자  "
{

	return T_OK;
}


int CTranCmn::fnAPP_TranEnc278_278(int nTrCode)		//"신한금융    "
{


	BYTE szDummy1[17] = "FFFFFFFFFFFFFFFF";
	BYTE szDummy2[17] = "0000";
	BYTE szPinSize[3] = "00";
	BYTE szValidData[13] = "            ";					// 암호화에 필요한 유효데이터 12 바이트
	BYTE szXorPindata[17] ="";
	BYTE szPinBlock[512] = {""};;
	BYTE szDummyWork[512] = {'7', 'E' , '0', 'E' , 'A', '6', '3', '2', '5', 'A', '6', '3', '4', '4', 'D', '8'} ;
	BYTE szMemberNum[512] = {""};
	BYTE o_MASETER_KEY[8] = {0x85, 0x26, 0x07, 0x84, 0x07, 0x85, 0x89, 0x51};
	BYTE o_WORKING_KEY[8] = {0x7E, 0x0E, 0xA6, 0x32, 0x5A, 0x63, 0x44, 0xD8};
	BYTE o_REAL_KEY[8] = {""};
	BYTE o_REAL_KEY_LEFT[8] =  {0x85, 0x26, 0x07, 0x84, 0x07, 0x85, 0x89, 0x51};
	BYTE o_REAL_KEY_RIGHT[8] = {0x85, 0x26, 0x07, 0x84, 0x07, 0x85, 0x89, 0x51};
	BYTE o_PINBLOCK[8] = {""};
	BYTE e_PINBLOCK[8] = {""};
	BYTE DesRPIN[17] = {""};
	
	//C66861007E0E3BC5 -> shStkeyo.enc : 리얼워킹키를 마스터키로 암호화(DES)한 워킹키 (8byte) 
	//0198237645546732 -> 마스터키 (8byte)  
	int	 nPinSize = 0;
	int	 nReturn = FALSE;
	

	memset(DesRPIN, 0, sizeof(DesRPIN));						
	nPinSize = sizeof(Accept.PassWord);
	

	MakePack("C66861007E0E3BC5", o_WORKING_KEY, strlen("C66861007E0E3BC5"));	
	MakePack("0198237645546732", o_MASETER_KEY, strlen("0198237645546732"));	

	m_pDevCmn->DECRYPT(o_REAL_KEY, o_WORKING_KEY, 8, o_MASETER_KEY); 
	
	szPinSize[1] |= (UINT)nPinSize;				// SIZE
	memcpy((char *)szDummy1, (char *)szPinSize, 2);
	//memcpy((char *)&szDummy1[2], Accept.PassWord, nPinSize);
	memcpy((char *)&szDummy1[2], Accept.PassWord, nPinSize);
	m_pDevCmn->fnUTL_JisPack(szDummy1,o_PINBLOCK,16);

	m_pDevCmn->ENCRYPT(e_PINBLOCK, o_PINBLOCK, 8, o_REAL_KEY);
	
	MakeUnPack(e_PINBLOCK, DesRPIN, sizeof(e_PINBLOCK));
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc278_278(%s)", DesRPIN);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranEnc278_278():return(%d)", nReturn);		
	
	memcpy(&m_STXEncBuff.byChiperMsdata, DesRPIN, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));


	return T_OK;
}

int CTranCmn::fnAPP_TranEnc279_279(int nTrCode)		//"동부증권    "
{
int iret = 0, sLen = 0, dLen = 0;
	long amnt;
	BYTE pwd[4+1],  hData[20+1];
	BYTE datetime[10+1], sData[143+1], dData[143+1]; 
	memset(datetime, 0x00, sizeof(datetime));

	memset(&m_STXEncBuff, 0x00, sizeof(SREncData));
	amnt = Asc2Int(Accept.Money, 15);
	memcpy(pwd, Accept.PassWord, 4);
	memcpy(datetime, Accept.CardReadTime, 10);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "Dummy Data[%d, %15.15s, %10.10s]", sizeof(Accept.PassWord), Accept.Money, &m_pProfile->TRANS.YYYYMMDDHHMMSS[4]);		

	/* Hash Code Create (전문전송일시(10)+거래금액(12)==>HashCode(20) ) */
	memset(hData, 0x00, sizeof(hData));
	iret = DB_GetHashCode(datetime, amnt, hData);
	if(iret != 0)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "errCode = [X%01d]", iret); /*에러처리*/		

	/* 암호화 대상정보 조합(32 Byte) */
	memset(sData, 0x00, sizeof(sData));
	sLen = 0;
	sData[sLen] = 0x31; sLen += 1;							/* 구분자(1) */
	memcpy(&sData[sLen], hData, 20); sLen += 20;			/* HashCode(20) */
	memcpy(&sData[sLen], pwd, 4); sLen += 4;				/* Password(4) */
	memset(&sData[sLen], 0x20, 7); sLen += 7;				/* Space(7) */
	
	/* 암호화 대상정보 암호화 (Out=100 Byte) */
	memset(dData, 0x00, sizeof(dData));
	iret = DB_EncSeed(m_279W1_KEY, sData, sLen, dData, &dLen);
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "[m_279W1_KEY]", dData, dLen);
	if(iret != 0)
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "errCode = [X%01d]", iret); /*에러처리*/		


	dData[dLen] = 0x00;
	Int2Asc(dLen, m_STXEncBuff.byChiperMsdata, 3, 10, '0');
	memcpy(&m_STXEncBuff.byChiperMsdata[3], dData, dLen);	

	Int2Asc(dLen+3, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc279_279 Return");


	return T_OK;
}

int CTranCmn::fnAPP_TranEnc294_294(int nTrCode)		//"해외카드 BC	   "
{
	if (CardServiceFlag == JCBCARD)	
		fnAPP_TranEnc294_294_BC_JCB_ENC_FOREGIN(nTrCode);
	else
		fnAPP_TranEnc294_294_BC_000_ENC_FOREGIN(nTrCode);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc294_294 Return");
	return T_OK;
}

int CTranCmn::fnAPP_TranEnc294_294_BC_000_ENC_FOREGIN(int nTrCode)
{
	BYTE workKey[8+1];
	BYTE workDataS[8+1];
	BYTE comp1[8+1];
	BYTE comp2[8+1];

	char temp[50];
	char temp1[50];
	char tempCard[17];
	char tempPass[17];

	char Card[20];							// 17->20 I KYS
	char Pass[17];
 	int  pinlen = 0, acctlen = 0;

	BYTE cardNum[9];
	BYTE passWord[9];
	BYTE workKeyS[16+1];
	BYTE encrypt[9];
	long sLen;
	int i;	// 워킹키 생성(난수발생)
	BYTE szMemberNum[512];
	BYTE szUnCardNum[512];

	//===============================================
	BYTE working[17];
	BYTE DesRPIN[21];	

	const char* SUBMASTER_KEY_BC_F	= "0785852607920785";	// 외환은행에서 제공한 마스터 키

#ifdef CERTI_TEST_KEY000
	const char* MASTER_KEY_BC_F		=  "4A1AFEE3A291761A";		// BC해외카드에서 제공한 마스터 키 TEST
#else														//  
	const char* MASTER_KEY_BC_F		=  "2CF8D6B3FBD59B5B";	// BC해외카드에서 제공한 마스터 키 real
#endif

	//================================================

	// Get WorkingKey
	memset(temp,	  0x00, sizeof(temp		));
	memset(temp1,	  0x00, sizeof(temp1	));
	memset(tempCard,  0x00, sizeof(tempCard	));
	memset(tempPass,  0x00, sizeof(tempPass	));
	memset(workKey,	  0x00, sizeof(workKey	));
	memset(workDataS, 0x00, sizeof(workDataS));
	memset(comp1,	  0x00, sizeof(comp1	));
	memset(comp2,	  0x00, sizeof(comp2	));
	memset(Pass,	  0x00, sizeof(Pass		));
	memset(Card,	  0x00, sizeof(Card		));
	memset(workKeyS,  0x00, sizeof(workKeyS	));
	memset(working,   0x00, sizeof(working	));
	memset(DesRPIN  , 0x00, sizeof(DesRPIN  ));
	memset(szMemberNum  , 0x00, sizeof(szMemberNum  ));
	memset(szUnCardNum  , 0x00, sizeof(szUnCardNum  ));

	sLen = MakePack((BYTE*)SUBMASTER_KEY_BC_F, (BYTE*)&comp1[0], 16);	// Data Block
	sLen = MakePack((BYTE*)MASTER_KEY_BC_F	 , (BYTE*)&comp2[0], 16);	// Key Block

	m_pDevCmn->ENCRYPT(&workDataS[0], comp1, 8, comp2);


	sLen = MakeUnPack(&workDataS[0], (BYTE *)&temp1[0], 8);
	temp1[sLen] = 0x00;

	memcpy(working,temp1, 16);
	working[16] = 0x00;


	sprintf((char *)szUnCardNum, "%s",fnDES_Cvt_CardNum((char *)CardData.ISO2Buff, CardData.ISO2size)); 
	int nCard = MakePack(szUnCardNum, szMemberNum, strlen((char *)szUnCardNum));	

	// Make cardNum Block
	// 20141231 I KYS
	memcpy(&Card[0], &szMemberNum[0], nCard);

	acctlen = strlen(Card);
	memset(tempCard, 0x30, 4);
	memcpy(&tempCard[4], &Card[acctlen - 13]  , 12);
	sLen = MakePack((BYTE *)&tempCard[0], (BYTE *)&cardNum[0], 16); 
	cardNum[sLen] = 0x00;

	// Make passWord Block
	memcpy(&Pass[0], &Accept.DesPassWord[0], 8);														// 비밀번호
	pinlen = strlen(Pass);
	tempPass[0] = 0x30;
	tempPass[1] = 0x30 | (BYTE)pinlen;
	memcpy(&tempPass[2], Pass, pinlen);
	pinlen += 2;
	memset(&tempPass[pinlen], 'F', 16 - pinlen);
	sLen = MakePack((BYTE *)&tempPass[0], (BYTE *)&passWord[0], 16);
	passWord[sLen] = 0x00;

	// XOR
	for (i = 0; i < 8; i++)
	{
		cardNum[i] = cardNum[i] ^ passWord[i];
	}

	m_pDevCmn->ENCRYPT(encrypt, cardNum, 8, comp1);
	//DES    ( CRYPTAGE, cardNum, comp1, encrypt );



	sLen = MakeUnPack(&encrypt[0], (BYTE *)&temp[0], 8);
	temp[sLen] = 0x00;

	memcpy(DesRPIN, temp, 16);
	DesRPIN[16] = 0x00;

	memcpy(m_STXEncBuff.byEncPasswd, DesRPIN, 16);
	memcpy(m_STXEncBuff.byChiperMsdata, working, 16);

	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc294_294_BC_000_ENC_FOREGIN EncPWD(%16.16s)", m_STXEncBuff.byEncPasswd);
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc294_294_BC_000_ENC_FOREGIN EncKey(%16.16s)", m_STXEncBuff.byChiperMsdata);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc294_294_BC_000_ENC_FOREGIN Return");
	return T_OK;
}

int CTranCmn::fnAPP_TranEnc294_294_BC_JCB_ENC_FOREGIN(int nTrCode)
{

	BYTE workKey[8+1];
	BYTE workDataS[8+1];
	BYTE comp1[8+1];
	BYTE comp2[8+1];

	char temp[50];
	char temp1[50];
	char tempCard[17];
	char tempPass[17];

	char Card[20];							// 17->20 I KYS
	char Pass[17];
 	int  pinlen = 0, acctlen = 0;

	BYTE cardNum[9];
	BYTE passWord[9];
	BYTE workKeyS[16+1];
	BYTE encrypt[9];
	long sLen;
	BYTE szMemberNum[512];
	BYTE szUnCardNum[512];

	//===============================================
	BYTE working[17];
	BYTE DesRPIN[21];	

	const char* SUBMASTER_KEY_BC_F	= "0785852607920785";	// 외환은행에서 제공한 마스터 키

#ifdef CERTI_TEST_KEY000
	const char* MASTER_KEY_BC_F		=  "4A1AFEE3A291761A";		// BC해외카드에서 제공한 마스터 키 TEST
#else														//  
	const char* MASTER_KEY_BC_F		=  "2CF8D6B3FBD59B5B";	// BC해외카드에서 제공한 마스터 키 real
#endif

	//================================================

	// Get WorkingKey
	memset(temp,	  0x00, sizeof(temp		));
	memset(temp1,	  0x00, sizeof(temp1	));
	memset(tempCard,  0x00, sizeof(tempCard	));
	memset(tempPass,  0x00, sizeof(tempPass	));
	memset(workKey,	  0x00, sizeof(workKey	));
	memset(workDataS, 0x00, sizeof(workDataS));
	memset(comp1,	  0x00, sizeof(comp1	));
	memset(comp2,	  0x00, sizeof(comp2	));
	memset(Pass,	  0x00, sizeof(Pass		));
	memset(Card,	  0x00, sizeof(Card		));
	memset(workKeyS,  0x00, sizeof(workKeyS	));
	memset(working,   0x00, sizeof(working	));
	memset(DesRPIN  , 0x00, sizeof(DesRPIN  ));
	memset(szMemberNum  , 0x00, sizeof(szMemberNum  ));
	memset(szUnCardNum  , 0x00, sizeof(szUnCardNum  ));

	sLen = MakePack((BYTE*)SUBMASTER_KEY_BC_F, (BYTE*)&comp1[0], 16);	// Data Block
	sLen = MakePack((BYTE*)MASTER_KEY_BC_F	 , (BYTE*)&comp2[0], 16);	// Key Block

	m_pDevCmn->DECRYPT(&workKey[0],   comp1, 8, comp2);
	m_pDevCmn->ENCRYPT(&workDataS[0], comp1, 8, comp2); 

	sLen = MakeUnPack(&workDataS[0], (BYTE *)&temp1[0], 8);
	temp1[sLen] = 0x00;

	memcpy(working,temp1, 16);
	working[16] = 0x00;

	sprintf((char *)szUnCardNum, "%s",fnDES_Cvt_CardNum((char *)CardData.ISO2Buff, CardData.ISO2size)); 
	int nCard = MakePack(szUnCardNum, szMemberNum, strlen((char *)szUnCardNum));	
	// Make cardNum Block
	// 20141231 I KYS
	memcpy(&Card[0], &szMemberNum[0], nCard);

	acctlen = strlen(Card);
	memset(tempCard, 0x30, 4);
	memcpy(&tempCard[4], &Card[acctlen - 13]  , 12);
	sLen = MakePack((BYTE *)&tempCard[0], (BYTE *)&cardNum[0], 16); 
	cardNum[sLen] = 0x00;

	// Make passWord Block
	memcpy(&Pass[0], &Accept.DesPassWord[0], 8);														// 비밀번호
	pinlen = strlen(Pass);
	memcpy(&tempPass[0], Pass, pinlen);
	memset(&tempPass[pinlen], 'F', 16 - pinlen);
	sLen = MakePack((BYTE *)&tempPass[0], (BYTE *)&passWord[0], 16);
	passWord[sLen] = 0x00;


	m_pDevCmn->ENCRYPT(encrypt, passWord, 8, workKey);
	//DES    ( CRYPTAGE, cardNum, comp1, encrypt );



	sLen = MakeUnPack(&encrypt[0], (BYTE *)&temp[0], 8);
	temp[sLen] = 0x00;

	memcpy(DesRPIN, temp, 16);
	DesRPIN[16] = 0x00;

	memcpy(m_STXEncBuff.byEncPasswd, DesRPIN, 16);
	memcpy(m_STXEncBuff.byChiperMsdata, working, 16);
	Int2Asc(16, m_STXEncBuff.byOutLen, sizeof(m_STXEncBuff.byOutLen));

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc294_294_BC_JCB_ENC_FOREGIN EncPWD(%16.16s)", m_STXEncBuff.byEncPasswd);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc294_294_BC_JCB_ENC_FOREGIN EncKey(%16.16s)", m_STXEncBuff.byChiperMsdata);

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_TranEnc294_294_BC_JCB_ENC_FOREGIN Return");
	return T_OK;
}

int CTranCmn::fnAPP_TranEnc280_280(int nTrCode)		//"유진투자    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc287_287(int nTrCode)		//"메리츠증권  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc289_289(int nTrCode)		//"NH증권      "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc290_290(int nTrCode)		//"부국증권    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc291_291(int nTrCode)		//"신영증권    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc292_292(int nTrCode)		//"LIG증권	   "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc384_364(int nTrCode)		//"삼성증권    "
{

	return T_OK;
}

int CTranCmn::fnAPP_TranEnc484_484(int nTrCode)		//"CJ시스템    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc601_601(int nTrCode)		//"발렉스      "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc701_701(int nTrCode)		//"KTF         "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc702_702(int nTrCode)		//"LGT         "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranEnc703_703(int nTrCode)		//"SKT         "
{

	return T_OK;
}



/********************************************************************/

int CTranCmn::fnAPP_TranDec000_000(int nTrCode)		//"????????????"
{

	return T_OK;
}

int CTranCmn::fnAPP_TranDec001_001(int nTrCode)		//"한국은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec002_002(int nTrCode)		//"산업은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec003_003(int nTrCode)		//"기업은행    "
{

	uint8   stroutbuffv[9048];
 	uint8	strinbuffv[9048];
	uint16  out_len;
	int		iret;
	uint16		slen =0;
	memset(strinbuffv, 0x00, sizeof(strinbuffv));
	memset(stroutbuffv, 0x00, sizeof(stroutbuffv));
/*
	slen = sizeof(m_TXATM_KIUP_TRAN_COM.card_no) + sizeof(m_TXATM_KIUP_TRAN_COM.ms_data) + sizeof(m_TXATM_KIUP_TRAN_COM.req_amt) + sizeof(m_TXATM_KIUP_TRAN_COM.svr_f_fee1);
	slen += sizeof(m_TXATM_KIUP_TRAN_COM.svr_f_fee2) + sizeof(m_TXATM_KIUP_TRAN_COM.svr_f_fee3) + sizeof(m_TXATM_KIUP_TRAN_COM.r_code) + sizeof(m_TXATM_KIUP_TRAN_COM.r_acc_no);
	slen += sizeof(m_TXATM_KIUP_TRAN_COM.pwd) + sizeof(m_TXATM_KIUP_TRAN_COM.svr_remain_f) + sizeof(m_TXATM_KIUP_TRAN_COM.remain) + sizeof(m_TXATM_KIUP_TRAN_COM.r_acc_name);
	slen += sizeof(m_TXATM_KIUP_TRAN_COM.svr_per_tranno) + sizeof(m_TXATM_KIUP_TRAN_COM.reserve);
*/
// 	memcpy(strinbuffv, m_RXATM_KIUP_TRAN_COM.card_no, Asc2Int(m_RXATM_KIUP_TRAN_COM.en_len, sizeof(m_RXATM_KIUP_TRAN_COM.en_len)));
// 	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_Decrypt103_strinbuff_After",(char *)strinbuffv, Asc2Int(m_RXATM_KIUP_TRAN_COM.en_len, sizeof(m_RXATM_KIUP_TRAN_COM.en_len)));

//	out_len = Asc2Int(m_RXATM_KIUP_TRAN_COM.en_len, sizeof(m_RXATM_KIUP_TRAN_COM.en_len));

	memcpy(&strinbuffv[0], m_SRXEncBuff.byEnDummy, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));	
	iret = XMC2_Decrypt_Message_Base64 (&m_KIUP_XM_CTX, stroutbuffv, &out_len, sizeof(stroutbuffv), strinbuffv, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));

	if(iret < 0)
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "nAPP_decrypt103 iret = [%04d]", iret);		
	}
		
//	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_encrypt103_m_KIUP_XM_CTX",(char *)&m_KIUP_XM_CTX, sizeof(m_KIUP_XM_CTX) );
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_Decrypt103_strinbuff_After",(char *)stroutbuffv, out_len);

// 	switch(nTranCode)
// 	{
// 	case TRANID_4310:
// 	case TRANID_6310:
// 	case TRANID_4320:
// 		memcpy(m_RXATM_KIUP_TRAN_COM.card_no, stroutbuffv, out_len);
// 		break;
// 	case TRANID_7220:
// 		memcpy(m_RXATM_KIUP_TRAN_COM.card_no, stroutbuffv, out_len);
// 		break;
// 	case TRANID_7260:
// 		memcpy(m_RXATM_KIUP_TRAN_COM.card_no, stroutbuffv, out_len);
// 		break;
// 	case TRANID_4370:
// 	case TRANID_4390:
// 		memcpy(m_RXATM_KIUP_TRAN_COM.card_no, stroutbuffv, out_len);
// 		break;
// 	default:
// 		memcpy(m_RXATM_KIUP_TRAN_COM.card_no, stroutbuffv, out_len);
// 		break;
// 	}
// 	
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_decrypt103() Return");		

	return T_OK;
}
int CTranCmn::fnAPP_TranDec005_005(int nTrCode)		//"외환은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec007_007(int nTrCode)		//"수협중앙회  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec008_008(int nTrCode)		//"수출입은행  "
{

	return T_OK;
}


int CTranCmn::fnAPP_TranDec020_020(int nTrCode)		//"우리은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec023_023(int nTrCode)		//"제일은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec053_053(int nTrCode)		//"씨티은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec031_031(int nTrCode)		//"대구은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec032_032(int nTrCode)		//"부산은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec034_034(int nTrCode)		//"광주은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec035_035(int nTrCode)		//"제주은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec037_037(int nTrCode)		//"전북은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec039_039(int nTrCode)		//"경남은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec045_045(int nTrCode)		//"새마을금고  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec048_048(int nTrCode)		//"신협중앙회  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec050_050(int nTrCode)		//"상호저축은행"
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec054_054(int nTrCode)		//"HSBC은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec055_055(int nTrCode)		//"도이치은행  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec056_056(int nTrCode)		//"ABN은행     "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec071_071(int nTrCode)		//"우체국      "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec025_081(int nTrCode)		//"하나은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec021_088(int nTrCode)		//"신한은행    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec105_105(int nTrCode)		//"KEB하나카드 "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec106_106(int nTrCode)		//"국민카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec107_107(int nTrCode)		//"수협카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec111_111(int nTrCode)		//"농협비자    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec120_120(int nTrCode)		//"우리카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec121_121(int nTrCode)		//"조흥카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec126_192(int nTrCode)		//"신한카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec127_127(int nTrCode)		//"씨티카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec134_034(int nTrCode)		//"광주카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec135_135(int nTrCode)		//"제주비자    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec137_037(int nTrCode)		//"전북카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec181_105(int nTrCode)		//"KEB하나카드 "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec184_184(int nTrCode)		//"삼성카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec192_192(int nTrCode)		//"신한카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec193_193(int nTrCode)		//"현대카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec194_194(int nTrCode)		//"BC카드      "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec195_195(int nTrCode)		//"롯데카드    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec209_209(int nTrCode)		//"유안타증권	 "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec218_218(int nTrCode)		//"현대증권    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec230_230(int nTrCode)		//"미래에증권  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec238_238(int nTrCode)		//"대우증권    "
{

	return T_OK;
}

int CTranCmn::fnAPP_TranDec243_243(int nTrCode)		//"한국투자    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec247_247(int nTrCode)		//"우리투자    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec261_261(int nTrCode)		//"교보증권    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec262_262(int nTrCode)		//"하이투자    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec263_263(int nTrCode)		//"HMC투자     "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec264_264(int nTrCode)		//"키움증권    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec265_265(int nTrCode)		//"이트레이드  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec266_266(int nTrCode)		//"에스케이    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec267_267(int nTrCode)		//"대신증권    "
{


	return T_OK;
}
int CTranCmn::fnAPP_TranDec268_268(int nTrCode)		//"솔로몬투자  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec269_269(int nTrCode)		//"한화증권    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec270_270(int nTrCode)		//"하나대투    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec278_278(int nTrCode)		//"신한금융    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec279_279(int nTrCode)		//"동부증권    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec280_280(int nTrCode)		//"유진투자    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec287_287(int nTrCode)		//"메리츠증권  "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec289_289(int nTrCode)		//"NH증권      "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec290_290(int nTrCode)		//"부국증권    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec291_291(int nTrCode)		//"신영증권    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec294_294(int nTrCode)		//"LIG증권	   "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec292_292(int nTrCode)		//"LIG증권	   "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec384_364(int nTrCode)		//"삼성증권    "
{

	return T_OK;
}

int CTranCmn::fnAPP_TranDec484_484(int nTrCode)		//"CJ시스템    "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec601_601(int nTrCode)		//"발렉스      "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec701_701(int nTrCode)		//"KTF         "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec702_702(int nTrCode)		//"LGT         "
{

	return T_OK;
}
int CTranCmn::fnAPP_TranDec703_703(int nTrCode)		//"SKT         "
{

	return T_OK;
}

