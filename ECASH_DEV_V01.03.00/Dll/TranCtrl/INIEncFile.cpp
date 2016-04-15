#include "stdafx.h"
#include <direct.h>
#include "Winbase.h"
#include "EzCaDes.h"
#include "SeedDWS.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Tran\WREncClt.h"
#include "..\..\H\Tran\HWEncClt.h"
#include "..\..\H\Tran\fnHDEncLib.h"
#include "..\..\H\Tran\trust_connect.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\MacroDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"

#include "..\..\H\Tran\TranCmn.h"

//24hr -> 설정 (테스트용임)
int CTranCmn::fnAPP_TranBank_Flag_Init_T()
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
			if (memcmp(BankCodeTbl[i].BankCode, BankCodeTbl[i].OrgCode, strlen(BankCodeTbl[i].OrgCode)) == 0)
			{
				//1 출금,조회,이체,수취이체,입금,수수료조회
				sprintf(szSection, "%03d", fnAPP_Get_BankOrgCodeInt(Asc2Int(BankCodeTbl[i].BankCode, strlen(BankCodeTbl[i].BankCode))));
				sprintf(szKey, "%s|0000", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, fnAPP_Get_BankName(Asc2Int(BankCodeTbl[i].BankCode, strlen(BankCodeTbl[i].BankCode))));

				//==================================================================================
				sprintf(szKey, "%s|7210", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|7220", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|7260", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|7230", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|7250", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|7380", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);

				//==================================================================================
				sprintf(szKey, "%s|4370", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|4390", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
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
				sprintf(szKey, "%s|7200", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				sprintf(szKey, "%s|4410", szSection);
				IniSetStr(_ATMTRCODEENCRYPT_INI,szSection, szKey, BankCodeTbl[i].OrgEnc);
				break;
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

