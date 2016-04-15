#include "stdafx.h"
#include "shlwapi.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\MacroDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**********************************************************************************************************/
/* Function Name : Data Format Make (Recv) gijin
/* 
/* Date			 : 2005.07.14
/* 
/* Input	     : TRCODE
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/  
int	CTranCmn::fn_FRX_TRANID_A010(int nTranCode, int nSec)
{
	CString strGetRxDataField_Date("");
	CString strGetRxDataField_Time("");
	CString strGetRxDataField("");
	int		nCnt = 0;
	char szMsg[1024] = {0,};

	char	szSection[1024];
	char	szKey	 [1024];
	CString strCode ;				// ����ڵ� (3)
	CString strCodeAll;				// ����ڵ� (3)
	CString strSDate;				// �������� St01 (12)
	CString strEDate;				// �������� Ed01 (12)

	int nStartIdx	= 0;
	int nSdateIdx	= 0;	
	int nEdateIdx	= 0;	
	int nLine		= 0;
	int nGap		= 1;
	int nPublicL    = 0;

	if(TranResultNGCode == TRAN_MSG_EXCEPTION_A010)
		return T_OK;
	
	if(!fnAPP_RXGetFindArryDataField(x001X, strlen(x001X), strGetRxDataField_Date))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x001X[%s]" , x001X);
		return T_OK;
	}

	if(!fnAPP_RXGetFindArryDataField(x002X, strlen(x002X), strGetRxDataField_Time, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x002X[%s]" , x002X);
		return T_OK;
	}
	//�ŷ��ð�	
	sprintf(m_RD.byDataField_002x, "%8.8s%6.6s", strGetRxDataField_Date.GetBuffer(0), strGetRxDataField_Time.GetBuffer(0));

	if(fnAPP_RXGetFindArryDataField(x003X, strlen(x003X), strGetRxDataField, PACK_F))
	{	
		m_pProfile->GetTransProfile();
		if (strGetRxDataField.CompareNoCase(m_pProfile->TRANS.SerialNo) != 0)
		{

			memcpy(m_pProfile->TRANS.SerialNo, strGetRxDataField.GetBuffer(0), 6);
			m_pDevCmn->fnAPL_SetSerialNo();
		}
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x003X[%s]" , x003X);
	}


	if(!fnAPP_RXGetFindArryDataField(x011X, strlen(x011X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x011X[%s]" , x011X);
	}
	else
	{
		memset(m_AtmCode, 0x00, sizeof(m_AtmCode));
		memcpy(m_AtmCode, strGetRxDataField.Right(4), 4);
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x011X[%s]" , m_AtmCode, x011X);
	
	}

	if(!fnAPP_RXGetFindArryDataField(x031X, strlen(x031X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x031X[%s]" , x031X);
	}
	else
	{

		int nOrg = 0;
		int nIdx = 0;
		int i = 0;
		nCnt = strGetRxDataField.GetLength()/sizeof(m_F031X.F031XLIST[0].OrgNo);
		for (i = 0; i < nCnt; i++)
		{
			memcpy(&m_F031X.F031XLIST[i], strGetRxDataField.Mid(nIdx, 3), 3);
			m_F031X.F031XLIST[i].Dummy = 0x00;
			nIdx += 3;

			if(memcmp(m_F031X.F031XLIST[i].OrgNo, HYDSBANKID_PUBLIC, strlen(HYDSBANKID_PUBLIC)) == 0)
			{
				if (nPublicL == 0)
				{
					MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010:F031XLIST Idx(%d) - x031X[%3.3s]" , nIdx, &m_F031X.F031XLIST[i]);
					nPublicL = Asc2Int(HYDSBANKID_PUBLIC);
				}
			}
			else
			if(memcmp(m_F031X.F031XLIST[i].OrgNo, TYJSBANKID_PUBLIC, strlen(TYJSBANKID_PUBLIC)) == 0)
			{
				if (nPublicL == 0)
				{
					MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010:F031XLIST Idx(%d) - x031X[%3.3s]" , nIdx, &m_F031X.F031XLIST[i]);
					nPublicL = Asc2Int(TYJSBANKID_PUBLIC);
				}
			}

//			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010:F031XLIST Idx(%d) - x031X[%3.3s]" , nIdx, &m_F031X.F031XLIST[i]);
		}

		if (nPublicL == 0)	
		{
			m_pProfile->TRANS.TranService = Asc2Int(HYDSBANKID_PUBLIC);
		}
		else
		{
			m_pProfile->TRANS.TranService = nPublicL;
		}
	
		m_pProfile->PutTransProfile();
		m_pProfile->GetTransProfile();

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010:TRANS.TranService PUBLIC(%03d)" , m_pProfile->TRANS.TranService);
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		CString cstGetReg("000");
		cstGetReg = RegGetStr(_REGKEY_REMOTECTRL, "ECashRX091X", "000");
		cstGetReg = ZERO3 + cstGetReg;
		cstGetReg = cstGetReg.Right(3);

		if (strGetRxDataField.CompareNoCase(cstGetReg) != 0)
		{
			//����
			if(!fnAPP_RXGetFindArryDataField(x090X, strlen(x090X), strGetRxDataField, PACK_F))
			{
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x090X[%s]" , x090X);

				CString strUpdateFile = _HOST_STATUS_INI;

				if(PathFileExists(strUpdateFile))
				{
					//WinExecWaitEnd(strUpdateFile, SW_HIDE);
					DeleteFile(strUpdateFile);
					MsgDump(TRACE_CODE_MODE, "Log",__FILE__, __LINE__, "fn_FRX_TRANID_A010() :DeleteFile(%s)", strUpdateFile);
				}

			}
			else
			{



				CString strUpdateFile = _HOST_STATUS_INI;

				strGetRxDataField.TrimLeft();
				strGetRxDataField.TrimRight();

				if(PathFileExists(strUpdateFile))
				{
					//WinExecWaitEnd(strUpdateFile, SW_HIDE);
					DeleteFile(strUpdateFile);
					MsgDump(TRACE_CODE_MODE, "Log",__FILE__, __LINE__, "fn_FRX_TRANID_A010() :DeleteFile(%s)", strUpdateFile);
				}

				strCodeAll = "FFF";
				while( nStartIdx < strGetRxDataField.GetLength())
				{
					strCode = strGetRxDataField.Mid(nStartIdx, 3 );							// ����ڵ�

					//==================================================================================
					sprintf(szSection, "%03d", Asc2Int(strCode));
					sprintf(szKey, "%s", "CNT0");
					IniSetStr(_HOST_STATUS_INI, szSection, szKey, strGetRxDataField.Mid(nStartIdx + 3, 1));
					//==================================================================================
					if (strCodeAll.CompareNoCase("FFF") == 0)
					{
						strCodeAll.Empty();
						strCodeAll = strCode;
					}
					else
					{
						strCodeAll += "|" + strCode;
					}

					for ( int i = 0; i < Asc2Int(strGetRxDataField.Mid(nStartIdx + 3, 1)); i++ )
					{
						nSdateIdx	= (nGap * 4) + nLine  * 24;								// ��������
						nEdateIdx	= nSdateIdx + 12;										// ��������

						strSDate	= strGetRxDataField.Mid(nSdateIdx, 12);
						strEDate	= strGetRxDataField.Mid(nEdateIdx, 12);

						//==================================================================================
						sprintf(szKey, "St%02d", i + 1);								// ��������
						IniSetStr(_HOST_STATUS_INI, szSection, szKey, strSDate);
						//==================================================================================
						sprintf(szKey, "Et%02d", i + 1);								// ��������
						IniSetStr(_HOST_STATUS_INI, szSection, szKey, strEDate);
						//==================================================================================
						nLine++;
					}

					nStartIdx = nEdateIdx + 12;
					nGap++;
				}

				IniSetStr(_HOST_STATUS_INI, "ALL", "LIST", strCodeAll);

				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x090X[%s]" , strGetRxDataField, x090X);
			
			}
		}

		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  ECashRX091X(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}

	if(!fnAPP_RXGetFindArryDataField(x099X, strlen(x099X), strGetRxDataField, UNPACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x099X[%s]" , x099X);
	}
	else
	{
		int nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RXHOSTMSG.host_msg01, strGetRxDataField.GetLength());
		sprintf(szMsg, "%s", strCT(m_RXHOSTMSG.host_msg01, 50));
		IniSetStr(_B_E_INFO_INI, "HOSTMSG","msg01", szMsg);
		sprintf(szMsg, "%s", strCT(m_RXHOSTMSG.host_msg02, 50));
		IniSetStr(_B_E_INFO_INI, "HOSTMSG","msg02", szMsg);
		sprintf(szMsg, "%s", strCT(m_RXHOSTMSG.host_msg03, 50));
		IniSetStr(_B_E_INFO_INI, "HOSTMSG","msg03", szMsg);
		sprintf(szMsg, "%s", strCT(m_RXHOSTMSG.host_msg04, 50));
		IniSetStr(_B_E_INFO_INI, "HOSTMSG","msg04", szMsg);
		sprintf(szMsg, "%s", strCT(m_RXHOSTMSG.host_msg05, 50));
		IniSetStr(_B_E_INFO_INI, "HOSTMSG","msg05", szMsg);


		sprintf(m_RXHOSTMSG.host_Allmsg, "%s|%s|%s|%s|%s",    strCT(m_RXHOSTMSG.host_msg01, 50), strCT(m_RXHOSTMSG.host_msg02, 50) \
															, strCT(m_RXHOSTMSG.host_msg03, 50), strCT(m_RXHOSTMSG.host_msg04, 50) \
															, strCT(m_RXHOSTMSG.host_msg05, 50));
	}


	if(!fnAPP_RXGetFindArryDataField(x101X, strlen(x101X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x101X[%s]" , x101X);
	}
	else
	{
		memcpy(m_FRX101X, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010:m_FRX101X - x101X[%3.3s]" , m_FRX101X);
	}


	m_pDevCmn->fnAPL_SetDate(strGetRxDataField_Date);
	m_pDevCmn->fnAPL_SetCheckDate(strGetRxDataField_Date);	
	PutDateTime(strGetRxDataField_Date, strGetRxDataField_Time);

	return T_OK;	
}        
int	CTranCmn::fn_FRX_TRANID_A020(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	char szMsg[1024] = {0,};

	if(TranResultNGCode == TRAN_MSG_EXCEPTION_A020)
		return T_OK;

	if(!fnAPP_RXGetFindArryDataField(x099X, strlen(x099X), strGetRxDataField, UNPACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A020: DataField Not Found - x099X[%s]" , x099X);
		return T_OK;
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A020: DataField Found - x099X[%s]" , strGetRxDataField);
		int nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RXHOSTMSG.host_msg01, strGetRxDataField.GetLength());
		sprintf(szMsg, "%s", strCT(m_RXHOSTMSG.host_msg01, 50));
		IniSetStr(_B_E_INFO_INI, "HOSTMSG","msg01", szMsg);
		sprintf(szMsg, "%s", strCT(m_RXHOSTMSG.host_msg02, 50));
		IniSetStr(_B_E_INFO_INI, "HOSTMSG","msg02", szMsg);
		sprintf(szMsg, "%s", strCT(m_RXHOSTMSG.host_msg03, 50));
		IniSetStr(_B_E_INFO_INI, "HOSTMSG","msg03", szMsg);
		sprintf(szMsg, "%s", strCT(m_RXHOSTMSG.host_msg04, 50));
		IniSetStr(_B_E_INFO_INI, "HOSTMSG","msg04", szMsg);
		sprintf(szMsg, "%s", strCT(m_RXHOSTMSG.host_msg05, 50));
		IniSetStr(_B_E_INFO_INI, "HOSTMSG","msg05", szMsg);


		sprintf(m_RXHOSTMSG.host_Allmsg, "%s|%s|%s|%s|%s",    strCT(m_RXHOSTMSG.host_msg01, 50), strCT(m_RXHOSTMSG.host_msg02, 50) \
															, strCT(m_RXHOSTMSG.host_msg03, 50), strCT(m_RXHOSTMSG.host_msg04, 50) \
															, strCT(m_RXHOSTMSG.host_msg05, 50));

	}

	return T_OK;	
}        
int	CTranCmn::fn_FRX_TRANID_2190(int nTranCode, int nSec, int nEncFlag)
{
	CString strGetRxDataField("");
	char szKeylen[1024];
	memset(&m_RXENCKEY, 0x20, sizeof(m_RXENCKEY));
	memset(m_RXENCKEY.k_r_yymmdd, 0x30, sizeof(m_RXENCKEY.k_r_yymmdd));
	memset(m_RXENCKEY.k_v_yymmdd, 0x30, sizeof(m_RXENCKEY.k_v_yymmdd));

 
	if(TranResultNGCode == TRAN_MSG_EXCEPTION_2190)
		return T_OK;

	if(!fnAPP_RXGetFindArryDataField(x045X, strlen(x045X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x045X[%s]" , x045X);
	}
	else
	{
		memcpy(m_RXENCKEY.k_r_yymmdd, strGetRxDataField.GetBuffer(0), sizeof(m_RXENCKEY.k_r_yymmdd));
		memcpy(m_RXENCKEY.k_v_yymmdd, strGetRxDataField.GetBuffer(0), sizeof(m_RXENCKEY.k_v_yymmdd));
	}
	

	switch(CardBank)
	{
	case KBS_ORG_INT_004:
	case KBS_ORG_INT_006:
	case KBS_ORG_INT_029:
	case KBS_ORG_INT_030:
		//���� Key Data
		if(!fnAPP_RXGetFindArryDataField(x043X, strlen(x043X), strGetRxDataField, UNPACK_F))
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x043X[%s]" , x043X);
		}
		else
		{
			int nPacklen = MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_data, strGetRxDataField.GetLength());
			sprintf(szKeylen, "%05d", nPacklen);
			memcpy(m_RXENCKEY.k_len, szKeylen, sizeof(m_RXENCKEY.k_len));
		}
		break;
	case KUB_ORG_INT_003:
			//���� Key Data
			if(!fnAPP_RXGetFindArryDataField(x044X, strlen(x044X), strGetRxDataField, UNPACK_F))
			{
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x044X[%s]" , x044X);
			}
			else
			{
				int nPacklen = MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_data, strGetRxDataField.GetLength());
				
				sprintf(szKeylen, "%05d", Asc2Int(&m_RXENCKEY.k_data[0], 4));

				memcpy(&m_RXENCKEY.k_data[0], &m_RXENCKEY.k_data[4], Asc2Int(&m_RXENCKEY.k_data[0], 4));
				memcpy(m_RXENCKEY.k_len, szKeylen, sizeof(m_RXENCKEY.k_len));
			}

			break;
	case NON_ORG_INT_011:
	case NON_ORG_INT_012:
	case NON_ORG_INT_016:
	case NON_ORG_INT_017:
		break;
	case SCO_ORG_INT_023:
		break;
	case WRT_ORG_INT_247:
		if(!fnAPP_RXGetFindArryDataField(x044X, strlen(x044X), strGetRxDataField, UNPACK_F))   
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x044X[%s]" , x044X);
		}
		else
		{
			int nPacklen = MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_data, strGetRxDataField.GetLength());
			sprintf(szKeylen, "%05d", nPacklen);
			memcpy(m_RXENCKEY.k_len, szKeylen, sizeof(m_RXENCKEY.k_len));
		}
		break;
	case SST_ORG_INT_CVT_240:
	case SST_ORG_INT_240:
		if(!fnAPP_RXGetFindArryDataField(x044X, strlen(x044X), strGetRxDataField, UNPACK_F))   
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x044X[%s]" , x044X);
		}
		else
		{
			int nPacklen = MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_data, strGetRxDataField.GetLength());
			sprintf(szKeylen, "%05d", nPacklen);
			memcpy(m_RXENCKEY.k_len, szKeylen, sizeof(m_RXENCKEY.k_len));
		}
	break;
	case HWT_ORG_INT_269:
		if(!fnAPP_RXGetFindArryDataField(x044X, strlen(x044X), strGetRxDataField, UNPACK_F))   
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x044X[%s]" , x044X);
		}
		else
		{
			int nPacklen = MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_data, strGetRxDataField.GetLength());
			sprintf(szKeylen, "%05d", nPacklen);
			memcpy(m_RXENCKEY.k_len, szKeylen, sizeof(m_RXENCKEY.k_len));
		}
		break;
	case KJB_ORG_INT_034:  
		//���� Key Data
		if(!fnAPP_RXGetFindArryDataField(x043X, strlen(x043X), strGetRxDataField, UNPACK_F))
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x043X[%s]" , x043X);
		}
		else
		{
			int nPacklen = MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_data, strGetRxDataField.GetLength());
			sprintf(szKeylen, "%05d", nPacklen);
			memcpy(m_RXENCKEY.k_len, szKeylen, sizeof(m_RXENCKEY.k_len));
		}

		if(!fnAPP_RXGetFindArryDataField(x044X, strlen(x044X), strGetRxDataField, UNPACK_F))   
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x044X[%s]" , x044X);
		}
		else
		{
			int nPacklen = MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_data_sub, strGetRxDataField.GetLength());
			sprintf(szKeylen, "%05d", nPacklen);
			memcpy(m_RXENCKEY.k_sublen, szKeylen, sizeof(m_RXENCKEY.k_sublen));
		}
		break;
	case KEB_ORG_INT_005:
		//���� Key Data
		if(!fnAPP_RXGetFindArryDataField(x043X, strlen(x043X), strGetRxDataField, UNPACK_F))
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x043X[%s]" , x043X);
		}
		else
		{
			int nPacklen = MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_data, strGetRxDataField.GetLength());
			sprintf(szKeylen, "%05d", nPacklen);
			memcpy(m_RXENCKEY.k_len, szKeylen, sizeof(m_RXENCKEY.k_len));
		}
		break;
	case WRB_ORG_INT_020:
		//���� Key Data
		if(!fnAPP_RXGetFindArryDataField(x043X, strlen(x043X), strGetRxDataField, UNPACK_F))
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x043X[%s]" , x043X);
		}
		else
		{
			int nPacklen = MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_data, strGetRxDataField.GetLength());
			sprintf(szKeylen, "%05d", nPacklen);
			memcpy(m_RXENCKEY.k_len, szKeylen, sizeof(m_RXENCKEY.k_len));
		}

		if(!fnAPP_RXGetFindArryDataField(x044X, strlen(x044X), strGetRxDataField, UNPACK_F))   
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x044X[%s]" , x044X);
		}
		else
		{
			int nPacklen = MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_data_sub, strGetRxDataField.GetLength());
			sprintf(szKeylen, "%05d", nPacklen);
			memcpy(m_RXENCKEY.k_sublen, szKeylen, sizeof(m_RXENCKEY.k_sublen));
		}
		break;
	case DOB_ORG_INT_279: 
		if(!fnAPP_RXGetFindArryDataField(x044X, strlen(x044X), strGetRxDataField, UNPACK_F))   
		{
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x044X[%s]" , x044X);
		}
		else
		{
			int nPacklen = MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_data, strGetRxDataField.GetLength());
			sprintf(szKeylen, "%05d", nPacklen);
			memcpy(m_RXENCKEY.k_len, szKeylen, sizeof(m_RXENCKEY.k_len));
		}
		break;

	default:
		break;
	}



	
	if(!fnAPP_RXGetFindArryDataField(x045X, strlen(x045X), strGetRxDataField, UNPACK_F))   
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x045X[%s]" , x045X);
	}
	else
	{
		MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_v_yymmdd, strGetRxDataField.GetLength());
		MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_r_yymmdd, strGetRxDataField.GetLength());
	}

	if(!fnAPP_RXGetFindArryDataField(x0451, strlen(x0451), strGetRxDataField, UNPACK_F))   
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x0451[%s]" , x0451);
	}
	else
	{
		MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_v_yymmdd, strGetRxDataField.GetLength());
		MakePack( strGetRxDataField.GetBuffer(0), m_RXENCKEY.k_r_yymmdd, strGetRxDataField.GetLength());
	}

	if(!fnAPP_RXGetFindArryDataField(x101X, strlen(x101X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x101X[%s]" , x101X);
	}
	else
	{
		memcpy(m_FRX101X, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190:m_FRX101X - x101X[%3.3s]" , m_FRX101X);
	}


	int nRet = 0;
	//��ȣȭ
	if(Asc2Int(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,2) == CIPHERTEXT_TMP)
	  nRet = fnAPP_KeyInitStartRecv(CardBank, K_VERIFY_C, 1);
	else
	  nRet = fnAPP_KeyInitStartRecv(CardBank, K_VERIFY_C, 0);

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű�������: %03d-%s]", CardBank, (nRet == C_SUCCESS) ? "OK" : "NG"));
	if(nRet != C_SUCCESS)
	{
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: [%s]" , GetSprintf("[Ű����ó��=> %03d-%s]", CardBank, (nRet == C_SUCCESS) ? "����" : "������"));
		return T_OK;

		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "[��ȣȭ Ű ��ȯ]", "����� �Ͻ��� �������");
		else
			fnAPP_CancelProc(T_MSG, "Transaction is not valid");
	}
  
	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_2190: DataField Not Found - x026X[%s]" , x026X);
	}
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	return T_OK;	
}    
    
int	CTranCmn::fn_FRX_TRANID_A060(int nTranCode, int nSec)
{

	return T_OK;	
}  

//�������       
int	CTranCmn::fn_FRX_TRANID_4310(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}

	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
	nEncType = fnAPP_DecType(TRANID_4310);
	if(nEncType == nEnCmd004_6)
	{
		if(!fnAPP_RXGetFindArryDataField(x054X, strlen(x054X), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x054X[%s]" , x054X);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_4310);
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);

	}
	else
	if(nEncType == nEnCmd240_364)
	{
		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x0151[%s]" , x0151);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_4310);
		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}
		
	}
	else
	{
		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
	return T_OK;	
}   

//����������     
int	CTranCmn::fn_FRX_TRANID_4320(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4320: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4320: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4320: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4320: DataField Not Found - x022X[%s]" , x022X);
	else
	{
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4320: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4320: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4320: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}
	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4320: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}
	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4320: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4320: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}


	return T_OK;	
}      

//�������
int	CTranCmn::fn_FRX_TRANID_4330(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4330: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4330: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4330: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4330: DataField Not Found - x022X[%s]" , x022X);
	else
	{
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4330: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4330: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4330: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}
	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4330: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}
	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4330: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4330: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
	//������  
	if(!fnAPP_RXGetFindArryDataField(x458X, strlen(x458X), strGetRxDataField, PACK_F))
	{
		sprintf(m_RD.byDataField_458x, "%s", "????");
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4330: DataField Not Found - x458X[%s]" , x458X);
	}
	else
	{
		sprintf(m_RD.byDataField_458x, "%s", strGetRxDataField.GetBuffer(0));
	}



	fn_FRX_EMVRecv_Lib(nTranCode);

	return T_OK;	
}      


//�����ּ�
int	CTranCmn::fn_FRX_TRANID_4340(int nTranCode, int nSec)
{

	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4340: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4340: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4340: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4340: DataField Not Found - x022X[%s]" , x022X);
	else
	{
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4340: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4340: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4340: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}
	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4340: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}


	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}
	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4340: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4340: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
	return T_OK;	
}   

//������ü
int	CTranCmn::fn_FRX_TRANID_4350(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;
	
	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));

	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x022X[%s]" , x022X);
	else
	{
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������  #N0161-1
		int FindEqual = 0;

		FindEqual = FindChar(strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength() , '=');	
		if(FindEqual > 0 && FindEqual < 16)
			sprintf(m_RD.byDataField_020x, "%s", &strGetRxDataField.GetBuffer(0)[FindEqual+1]);
		else
			sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}
	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}	

	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}

	//������  #N0161
	if(!fnAPP_RXGetFindArryDataField(x458X, strlen(x458X), strGetRxDataField, PACK_F))
	{
		sprintf(m_RD.byDataField_458x, "%s", "????");
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x458X[%s]" , x458X);
	}
	else
	{
		sprintf(m_RD.byDataField_458x, "%s", strGetRxDataField.GetBuffer(0));
	}


	fn_FRX_EMVRecv_Lib(nTranCode);
	return T_OK;	
}             


//������ü
int	CTranCmn::fn_FRX_TRANID_4370(int nTranCode, int nSec)
{

	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);


		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}

	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
	nEncType = fnAPP_DecType(TRANID_4370);
	if(nEncType == nEnCmd004_6)
	{
		if(!fnAPP_RXGetFindArryDataField(x054X, strlen(x054X), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x054X[%s]" , x054X);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_4370);
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);

	}
	else
	if(nEncType == TRAN_ENC_240_10)
	{
		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x0151[%s]" , x0151);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_4370);

		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}

	}
	else
	{
		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}

	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);;
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}



	return T_OK;	
}             
//������ü
int	CTranCmn::fn_FRX_TRANID_4390(int nTranCode, int nSec)
{

	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);


		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
	nEncType = fnAPP_DecType(TRANID_4390);
	if(nEncType == nEnCmd004_6)
	{
		if(!fnAPP_RXGetFindArryDataField(x054X, strlen(x054X), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x054X[%s]" , x054X);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_4390);
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);

	}
	else
	if(nEncType == TRAN_ENC_240_10)
	{
		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x0151[%s]" , x0151);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_4390);

		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}

	}
	else
	{
		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	
	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}

	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);;
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4390: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}



	return T_OK;	
}      


//ī���Ա�    
int	CTranCmn::fn_FRX_TRANID_4410(int nTranCode, int nSec)
{

	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);


		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));

		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);



	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
	nEncType = fnAPP_DecType(TRANID_4410);
	if(nEncType == nEnCmd004_6)
	{
		if(!fnAPP_RXGetFindArryDataField(x054X, strlen(x054X), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4310: DataField Not Found - x054X[%s]" , x054X);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_4410);
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);

	}
	else
	if(nEncType == TRAN_ENC_240_10)
	{
		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x0151[%s]" , x0151);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_4410);

		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}

	}
	else
	{
		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
	{
		sprintf(m_RD.byDataField_026x, "%12.12s", "????????????");	
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x026X[%s]" , x026X);
	}
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}

	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}

	return T_OK;	
}  

//��������ȸ
int	CTranCmn::fn_FRX_TRANID_7210(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	if(!fnAPP_RXGetFindArryDataField(x004X, strlen(x004X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x004X[%s]" , x004X);
	else
	{
		sprintf(m_RD.byDataField_004x, "%10.10s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x009X, strlen(x009X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x009X[%s]" , x009X);
	else
	{
		sprintf(m_RD.byDataField_009x, "%1.1s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
	nEncType = fnAPP_DecType(TRANID_7210);
	if(nEncType == nEnCmd004_6)
	{
		if(!fnAPP_RXGetFindArryDataField(x054X, strlen(x054X), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x054X[%s]" , x054X);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_7210);
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);

	}
	else
	if(nEncType == TRAN_ENC_240_10)
	{
		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x0151[%s]" , x0151);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_7210);

		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4410: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}

	}
	else
	{
		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}

	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

	//#N0180
	if(!fnAPP_RXGetFindArryDataField(x345X, strlen(x345X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7210: DataField Not Found - x345X[%s]" , x345X);
	else
	{
		//����������Ʈ���� ����
		sprintf(m_RD.byDataField_345x, "%2.2s", strGetRxDataField.GetBuffer(0));
	}

	return T_OK;	
}   

//��ȸ   
int	CTranCmn::fn_FRX_TRANID_7220(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}

	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
	nEncType = fnAPP_DecType(TRANID_7220);
	if(nEncType == nEnCmd004_6)
	{
		if(!fnAPP_RXGetFindArryDataField(x054X, strlen(x054X), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x054X[%s]" , x054X);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_7220);
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);

	}
	else
	if(nEncType == nEnCmd240_364)
	{
		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x0151[%s]" , x0151);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_7220);

		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}

	}
	else
	{
		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}


	return T_OK;	
}  
//������ü ��ȸ
int	CTranCmn::fn_FRX_TRANID_7250(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);


		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7250: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7250: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7250: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7250: DataField Not Found - x022X[%s]" , x022X);
	else
	{
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7250: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7250: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7250: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7250: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}
	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7250: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7250: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}

	//������  #N0161
	if(!fnAPP_RXGetFindArryDataField(x458X, strlen(x458X), strGetRxDataField, PACK_F))
	{
		sprintf(m_RD.byDataField_458x, "%s", "????");
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7250: DataField Not Found - x458X[%s]" , x458X);
	}
	else
	{
		sprintf(m_RD.byDataField_458x, "%s", strGetRxDataField.GetBuffer(0));
	}

	fn_FRX_EMVRecv_Lib(nTranCode);

	return T_OK;	
}        
//���������ȸ
int	CTranCmn::fn_FRX_TRANID_7260(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString strGetRxDataField2("");
	CString strGetRxDataField3("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	if(!fnAPP_RXGetFindArryDataField(x004X, strlen(x004X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x004X[%s]" , x004X);
	else
	{
		sprintf(m_RD.byDataField_004x, "%10.10s", strGetRxDataField.GetBuffer(0));
	}


	if(!fnAPP_RXGetFindArryDataField(x009X, strlen(x009X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x009X[%s]" , x009X);
	else
	{
		sprintf(m_RD.byDataField_009x, "%1.1s", strGetRxDataField.GetBuffer(0));
	}


	memset(m_RD.byDataField_027x, '0', sizeof(m_RD.byDataField_027x));
	if(!fnAPP_RXGetFindArryDataField(x027X, strlen(x027X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x027X[%s]" , x027X);
	else
	{
		if(!fnAPP_RXGetFindArryDataField(x028X, strlen(x028X), strGetRxDataField1, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x027X[%s]" , x027X);
		else
		{
			if(!fnAPP_RXGetFindArryDataField(x029X, strlen(x029X), strGetRxDataField2, PACK_F))
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x027X[%s]" , x027X);
			else
			{
				sprintf(m_RD.byDataField_027x, "%2.2s%2.2s%2.2s", strGetRxDataField.GetBuffer(0), strGetRxDataField1.GetBuffer(0), strGetRxDataField2.GetBuffer(0));
			}
		}
	}
	
	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}

	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
	nEncType = fnAPP_DecType(TRANID_7260);
	if(nEncType == nEnCmd004_6)
	{
		if(!fnAPP_RXGetFindArryDataField(x054X, strlen(x054X), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x054X[%s]" , x054X);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_7260);
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);

	}
	else
	if(nEncType == TRAN_ENC_240_10)
	{
		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x0151[%s]" , x0151);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_7260);

		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}
	}
	else
	{
		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}

	//#N0180
	if(!fnAPP_RXGetFindArryDataField(x345X, strlen(x345X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x345X[%s]" , x345X);
	else
	{
		//����������Ʈ���� ����
		sprintf(m_RD.byDataField_345x, "%2.2s", strGetRxDataField.GetBuffer(0));
	}

	//#N0214
	if(!fnAPP_RXGetFindArryDataField(x1721, strlen(x1721), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x1721[%s]" , x1721);
	else
	{
		//�ϳ����� ��� ������
		sprintf(m_RD.byDataField_1721, "%s", strGetRxDataField.GetBuffer(0));
	}
	return T_OK;	
}  

//�Աݰ��ɿ�����ȸ        
int	CTranCmn::fn_FRX_TRANID_7200(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString strGetRxDataField2("");
	CString strGetRxDataField3("");

	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	//����Ÿ����
	memset(m_RD.byDataField_027x, '0', sizeof(m_RD.byDataField_027x));
	if(!fnAPP_RXGetFindArryDataField(x027X, strlen(x027X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x027X[%s]" , x027X);
	else
	{
		if(!fnAPP_RXGetFindArryDataField(x028X, strlen(x028X), strGetRxDataField1, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x027X[%s]" , x027X);
		else
		{
			if(!fnAPP_RXGetFindArryDataField(x029X, strlen(x029X), strGetRxDataField2, PACK_F))
				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x027X[%s]" , x027X);
			else
			{
				sprintf(m_RD.byDataField_027x, "%2.2s%2.2s%2.2s", strGetRxDataField.GetBuffer(0), strGetRxDataField1.GetBuffer(0), strGetRxDataField2.GetBuffer(0));
			}
		}
	}


	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}

	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
	nEncType = fnAPP_DecType(TRANID_7200);
	if(nEncType == nEnCmd004_6)
	{
		if(!fnAPP_RXGetFindArryDataField(x054X, strlen(x054X), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x054X[%s]" , x054X);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_7200);
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);

	}
	else
	if(nEncType == TRAN_ENC_240_10)
	{
		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x0151[%s]" , x0151);
		else
			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());

		fnAPP_DecCall(nEncType, TRANID_7200);
	}
	else
	{
		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x022X[%s]" , x022X);
		else
		{
			//�����ܾ�	
			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
		}
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	//���� �귣�� ���� ����
	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x018X[%s]" , x018X);
	else
	{
		//���� ����	
		sprintf(m_RD.byDataField_018x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 

	return T_OK;	
}   

//������ȸ
int	CTranCmn::fn_FRX_TRANID_7230(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7230: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7230: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7230: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7230: DataField Not Found - x022X[%s]" , x022X);
	else
	{
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7230: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7230: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7230: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}
	
	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7230: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}
	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7230: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7230: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}



	fn_FRX_EMVRecv_Lib(nTranCode);
	return T_OK;	
}

//////////////////////////////////////////////////////////////////////////
// ���ݼ��� ��� ������ ��ȸ  #N0161                                    
                                    
int	CTranCmn::fn_FRX_TRANID_7380(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x022X[%s]" , x022X);
	else
	{
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}
	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}

	//������  
	if(!fnAPP_RXGetFindArryDataField(x458X, strlen(x458X), strGetRxDataField, PACK_F))
	{
		sprintf(m_RD.byDataField_458x, "%s", "????");
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x458X[%s]" , x458X);
	}
	else
	{
		sprintf(m_RD.byDataField_458x, "%s", strGetRxDataField.GetBuffer(0));
	}

	fn_FRX_EMVRecv_Lib(nTranCode);
	return T_OK;	
}                                    
            
int	CTranCmn::fn_FRX_TRANID_7530(int nTranCode, int nSec)
{
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
	{
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x017X[%s]" , x017X);
	}
	else
	{
		//���¹�ȣ	
//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
		else
			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

	}

	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x023X[%s]" , x023X);
	else
	{
		//�ŷ��ݾ�	
		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x024X[%s]" , x024X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x022X[%s]" , x022X);
	else
	{
		//�����ܾ�	
		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x025X[%s]" , x025X);
	else
	{
		//Ÿ����	
		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x021X[%s]" , x021X);
	else
	{
		//������	
		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x020X[%s]" , x020X);
	else
	{
		//�������	
		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
	}

	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x026X[%s]" , x026X);
	else
	{
		m_pProfile->GetTransProfile();
		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
		//Host �Ϸù�ȣ	
		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
		m_pProfile->PutTransProfile();
	}

	if(!fnAPP_RXGetFindArryDataField(x091X, strlen(x091X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField Not Found - x091X[%s]" , x091X);
	}
	else
	{
		RegSetStr(_REGKEY_REMOTECTRL, "ECashRX091X", strGetRxDataField);	
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_A010: DataField  m_AtmCode(%s) - x091X[%s]" , strGetRxDataField, x091X);
	
	}
	
	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x0120[%s]" , x0120);
	else
	{
		//������
		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
	}

// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}

	//������  
	if(!fnAPP_RXGetFindArryDataField(x458X, strlen(x458X), strGetRxDataField, PACK_F))
	{
		sprintf(m_RD.byDataField_458x, "%s", "????");
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7380: DataField Not Found - x458X[%s]" , x458X);
	}
	else
	{
		sprintf(m_RD.byDataField_458x, "%s", strGetRxDataField.GetBuffer(0));
	}

	fn_FRX_EMVRecv_Lib(nTranCode);
	return T_OK;	
}              
 
int	CTranCmn::fn_FRX_EMVRecv_Lib(int nTranCode, int nSec)
{
	memset(&m_RecvEmvrcvData,	    0, sizeof(RECVEMVRCVDATA));	
	CString strGetRxDataField("");
	CString strGetRxDataField1("");
	CString cstrTmp("");
	int nEncType = 0;
	int nPacklen = 0;

	if (EMVReadFlg == FALSE)
		return T_OK;

	//1. ARPC
	if(!fnAPP_RXGetFindArryDataField(x471X, strlen(x471X), strGetRxDataField, PACK_F))
	{
		sprintf(m_RecvEmvrcvData.ARPC, "%s", "0000");
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_EMVRecv_Lib: DataField Not Found - x471X[%s]" , x471X);
	}
	else
	{
		sprintf(m_RecvEmvrcvData.ARPC, "%s", strGetRxDataField.GetBuffer(0));
	}

	//2. APRCResposeCode
	if(!fnAPP_RXGetFindArryDataField(x382X, strlen(x382X), strGetRxDataField, PACK_F))
	{
		sprintf(m_RecvEmvrcvData.APRCResposeCode, "%s", "0000");
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_EMVRecv_Lib: DataField Not Found - x382X[%s]" , x382X);
	}
	else
	{
		sprintf(m_RecvEmvrcvData.APRCResposeCode, "%s", strGetRxDataField.GetBuffer(0));
	}

	//3. IssuerScriptLen(3) + IssuerScript(169)
	if(!fnAPP_RXGetFindArryDataField(x383X, strlen(x383X), strGetRxDataField, PACK_F))
	{
		sprintf(m_RecvEmvrcvData.APRCResposeCode, "%s", "0000");
		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_EMVRecv_Lib: DataField Not Found - x383X[%s]" , x383X);
	}
	else
	{
		sprintf(m_RecvEmvrcvData.IssuerScriptLen, "%3.3s", strGetRxDataField.GetBuffer(0));
		sprintf(m_RecvEmvrcvData.IssuerScript, "%169.169s", &strGetRxDataField.GetBuffer(0)[3]);
	}


	return T_OK;
}



                    
// /**************************************************************************************************************************************************************/
// //���� ���� �ϼ���. (2016.03.08) - �������� Kim.Gi.Jin [�����ʿ�ݵ���]
// /**************************************************************************************************************************************************************/
// int	CTranCmn::fn_FRX_TRANID_4300(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4301(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//        
// int	CTranCmn::fn_FRX_TRANID_4214(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//       
// int	CTranCmn::fn_FRX_TRANID_4216(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// 
//    
// // ���� ���
// int	CTranCmn::fn_FRX_TRANID_4210(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_4210);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4310);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4310);
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 		
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4210: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// } 
//   
// //#N0171 �ؿ�ī�� SAVING
// int	CTranCmn::fn_FRX_TRANID_4311(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4311: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4311: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4311: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4311: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4311: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4311: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4311: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4311: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4311: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4311: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	//������  #N0161
// 	if(!fnAPP_RXGetFindArryDataField(x458X, strlen(x458X), strGetRxDataField, PACK_F))
// 	{
// 		sprintf(m_RD.byDataField_458x, "%s", "????");
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4311: DataField Not Found - x458X[%s]" , x458X);
// 	}
// 	else
// 	{
// 		sprintf(m_RD.byDataField_458x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }
// 
// //#N0171 �ؿ�ī�� CHECKING        
// int	CTranCmn::fn_FRX_TRANID_4312(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4312: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4312: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4312: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4312: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4312: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4312: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4312: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4312: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4312: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4312: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	//������  #N0161
// 	if(!fnAPP_RXGetFindArryDataField(x458X, strlen(x458X), strGetRxDataField, PACK_F))
// 	{
// 		sprintf(m_RD.byDataField_458x, "%s", "????");
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4312: DataField Not Found - x458X[%s]" , x458X);
// 	}
// 	else
// 	{
// 		sprintf(m_RD.byDataField_458x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4314(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4315(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }  
// 
// //#N0171 �ؿ�ī�� BC ���
// int	CTranCmn::fn_FRX_TRANID_4316(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4316: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4316: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4316: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4316: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4316: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4316: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4316: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4316: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4316: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4316: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// }
// 
// //#N0171 �ؿ�ī�� BC ��� ���
// int	CTranCmn::fn_FRX_TRANID_4325(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 	return T_OK;	
// }
// 
// 
// // ���� ��� ���
// int	CTranCmn::fn_FRX_TRANID_4220(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4220: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4220: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4220: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4220: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4220: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4220: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4220: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4220: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4220: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4220: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// } 
// // ����ü ��� ���
// int	CTranCmn::fn_FRX_TRANID_4326(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x004X, strlen(x004X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4326: DataField Not Found - x004X[%s]" , x004X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_004x, "%10.10s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.MuAccountNum, 13))
// 			EditString(&Accept.MuAccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&Accept.MuAccountNum, 13))
// 			EditString(&Accept.MuAccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4326: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4326: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4326: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4326: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4326: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4326: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4326: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4326: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4326: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4326: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// } 
// // ����ü ���
// int	CTranCmn::fn_FRX_TRANID_4116(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.MuAccountNum, 4))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.MuAccountNum, 4))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_4310);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4310);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4310);
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 		
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4116: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// } 
//   
// int	CTranCmn::fn_FRX_TRANID_4321(int nTranCode, int nSec) //#N0235
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// } 
//        
// int	CTranCmn::fn_FRX_TRANID_4322(int nTranCode, int nSec)		
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 	return T_OK;
// }        
// 
// int	CTranCmn::fn_FRX_TRANID_4324(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// 
// 
// //#N0171 �ؿ�ī�� CREDIT  
// int	CTranCmn::fn_FRX_TRANID_4331(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4331: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4331: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4331: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4331: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4331: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4331: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4331: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4331: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4331: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4331: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	//������  
// 	if(!fnAPP_RXGetFindArryDataField(x458X, strlen(x458X), strGetRxDataField, PACK_F))
// 	{
// 		sprintf(m_RD.byDataField_458x, "%s", "????");
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4331: DataField Not Found - x458X[%s]" , x458X);
// 	}
// 	else
// 	{
// 		sprintf(m_RD.byDataField_458x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4332(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4332: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4332: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4332: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4332: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4332: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4332: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4332: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4332: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4332: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4332: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4336(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4339(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// 
// int	CTranCmn::fn_FRX_TRANID_4341(int nTranCode, int nSec)  //#N0235
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 	return T_OK;	
// }        
// 
// //�ؿ�ī�� ���� ��� ��� //#N0171
// int	CTranCmn::fn_FRX_TRANID_4342(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4349(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// 
// int	CTranCmn::fn_FRX_TRANID_4360(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4361(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// } 
// 
// //����-��ü
// int	CTranCmn::fn_FRX_TRANID_4215(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.AccountNum, 10))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.AccountNum, 4))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.AccountNum, 14))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_4390);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4390);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4390);
// 
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);;
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4215: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// }  
// //����ü ��ü
// int	CTranCmn::fn_FRX_TRANID_4396(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.MuAccountNum, 4))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.MuAccountNum, 4))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_4390);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4390);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4390);
// 
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);;
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4396: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// } 
//   
// int	CTranCmn::fn_FRX_TRANID_4391(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4392(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }    
// //��ü-IC    
// // int	CTranCmn::fn_FRX_TRANID_4370(int nTranCode, int nSec)
// // {
// // 	CString strGetRxDataField("");
// // 	CString strGetRxDataField1("");
// // 	CString cstrTmp("");
// // 	int nEncType = 0;
// // 	int nPacklen = 0;
// // 
// // 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// // 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// // 	{
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 
// // 
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x017X[%s]" , x017X);
// // 	}
// // 	else
// // 	{
// // 		//���¹�ȣ	
// // //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x023X[%s]" , x023X);
// // 	else
// // 	{
// // 		//�ŷ��ݾ�	
// // 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x024X[%s]" , x024X);
// // 	else
// // 	{
// // 		//������	
// // 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// // 	nEncType = fnAPP_DecType(TRANID_4370);
// // 	if(nEncType == TRAN_ENC_006_01)
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x022X[%s]" , x022X);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// // 
// // 
// // 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x019X[%s]" , x019X);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// // 
// // 		fnAPP_DecCall(nEncType, TRANID_4370);
// // 		//�����ܾ�	
// // 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// // 
// // 	}
// // 	else
// // 	if(nEncType == TRAN_ENC_240_10)
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x0151[%s]" , x0151);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// // 
// // 		fnAPP_DecCall(nEncType, TRANID_4370);
// // 
// // 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x022X[%s]" , x022X);
// // 		else
// // 		{
// // 			//�����ܾ�	
// // 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// // 		}
// // 
// // 	}
// // 	else
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x022X[%s]" , x022X);
// // 		else
// // 		{
// // 			//�����ܾ�	
// // 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// // 		}
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x025X[%s]" , x025X);
// // 	else
// // 	{
// // 		//Ÿ����	
// // 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x021X[%s]" , x021X);
// // 	else
// // 	{
// // 		//������	
// // 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x020X[%s]" , x020X);
// // 	else
// // 	{
// // 		//�������	
// // 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x026X[%s]" , x026X);
// // 	else
// // 	{
// // 		m_pProfile->GetTransProfile();
// // 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// // 		//Host �Ϸù�ȣ	
// // 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// // 		m_pProfile->PutTransProfile();
// // 	}
// // 
// // 	
// // 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x0120[%s]" , x0120);
// // 	else
// // 	{
// // 		//������
// // 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// // 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// // 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// // 	}
// // 
// // 	//��ǥ ����Data
// // 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x104X[%s]" , x0120);
// // 	else
// // 	{
// // 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// // 	}
// // 
// // 
// // 	return T_OK;	
// // }        
// int	CTranCmn::fn_FRX_TRANID_4394(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4395(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4399(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4520(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4530(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4540(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4550(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4560(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4570(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4571(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4572(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4580(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4610(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4620(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4611(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4621(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4631(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4630(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4640(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4650(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4660(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4670(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4670: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x070X, strlen(x070X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4670: DataField Not Found - x070X[%s]" , x070X);
// 	else
// 	{
// 		//������
// 		sprintf(m_RXHIPASS.m_RXATM4670.random_num, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x071X, strlen(x071X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4670: DataField Not Found - x071X[%s]" , x071X);
// 	else
// 	{
// 		//������
// 		sprintf(m_RXHIPASS.m_RXATM4670.s2, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x221X, strlen(x221X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4670: DataField Not Found - x221X[%s]" , x221X);
// 	else
// 	{
// 		sprintf(Accept.ETicketConfirmNum, "%8.8s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4670: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4670: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 	{
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4670: DataField Not Found - x024X[%s]" , x024X);
// 			sprintf(m_RD.byDataField_024x, "%s", "0");
// 	}
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x065X, strlen(x065X), strGetRxDataField, PACK_F))
// 	{
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4670: DataField Not Found - x065X[%s]" , x065X);
// 			sprintf(m_RD.byDataField_065x, "%s", "0");
// 	}
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_065x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4671(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x070X, strlen(x070X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x070X[%s]" , x070X);
// 	else
// 	{
// 		//������
// 		sprintf(m_RXHIPASS.m_RXATM4670.random_num, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x071X, strlen(x071X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4370: DataField Not Found - x071X[%s]" , x071X);
// 	else
// 	{
// 		//������
// 		sprintf(m_RXHIPASS.m_RXATM4670.s2, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x221X, strlen(x221X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7491: DataField Not Found - x221X[%s]" , x221X);
// 	else
// 	{
// 		sprintf(Accept.ETicketConfirmNum, "%8.8s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4680(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4681(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4682(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }    
// 
// // ���� �Ա�
// int	CTranCmn::fn_FRX_TRANID_4720(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.AccountNum, 4))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.AccountNum, 4))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_4720);
// 	if(m_pDevCmn->TranResult && nEncType == TRAN_ENC_006_01)
// 	{
// 		if(MenuAvail & MENU_PB)
// 		{
// 			char szPbm_P_N[2+1]; //����������󿩺�
// 			char szPbm_P_F[4+1]; //���� �ڷ� �߰� ���� ����
// 			char szPbm_S_L[3+1]; //���� ���� ��
// 			char szPbm_S_C[2+1]; //���� ��� �Ǽ�
// 			char szPbm_P_M[2+1]; //�����̿� ��� �ȳ�
// 
// 			char szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3 + 2 + 3600 + 80 + 200]; //�������ʰŷ�(1)+������󿩺�(1)+���������߰�(1)+����ŷ��ȳ�(1)+�����ܾ�����(1)+������(2)+������(3)+���Ǽ�(2)+3600+72	
// 
// 			memset(szPbm_P_N, 0x00, sizeof(szPbm_P_N));
// 			memset(szPbm_P_F, 0x00, sizeof(szPbm_P_F));
// 			memset(szPbm_S_L, 0x00, sizeof(szPbm_S_L));
// 			memset(szPbm_S_C, 0x00, sizeof(szPbm_S_C));
// 			memset(szPbm_P_M, 0x00, sizeof(szPbm_P_M));
// 			memset(szPbmAreaKb, 0x00, sizeof(szPbmAreaKb));	
// 
// 			if(!fnAPP_RXGetFindArryDataField(x088X, strlen(x088X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x088X[%s]" , x088X);
// 			else
// 			{
// 				//���� ���� ���� ����
// 				sprintf(szPbm_P_N, "%1.1s", strGetRxDataField.GetBuffer(0));
// 				memcpy(&szPbmAreaKb[1],szPbm_P_N , 1);
// 				memcpy(&m_RD.byDataField_Dummy[1] ,szPbm_P_N , 1);
// 				
// 			}
// 
// 			if(!fnAPP_RXGetFindArryDataField(x089X, strlen(x089X), strGetRxDataField, PACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x089X[%s]" , x089X);
// 			else
// 			{
// 				//���� �̿� ��� ����
// 				sprintf(szPbm_P_F, "%1.1s", strGetRxDataField.GetBuffer(0));
// 				memcpy(&szPbmAreaKb[1 + 1], szPbm_P_F , 1);
// 				memcpy(&m_RD.byDataField_Dummy[1 + 1] ,szPbm_P_F , 1);
// 			}
// 			
// 			if(!fnAPP_RXGetFindArryDataField(x092X, strlen(x092X), strGetRxDataField, PACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 			else
// 			{
// 				//���� ���� ��
// 				sprintf(szPbm_S_L, "%3.3s", strGetRxDataField.GetBuffer(0));
// 				memcpy(&szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2], szPbm_S_L, 3);
// 				memcpy(&m_RD.byDataField_Dummy[1 + 1 + 1 + 1 + 1 + 2] ,szPbm_S_L , 3);
// 			}
// 
// 			if(!fnAPP_RXGetFindArryDataField(x093X, strlen(x093X), strGetRxDataField, PACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 			else
// 			{
// 				//���� ��� �Ǽ�  #SJ
// 				sprintf(szPbm_S_C, "%2.2s", strGetRxDataField.GetBuffer(0));
// 				memcpy(&szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3], szPbm_S_C, 2);
// 				memcpy(&m_RD.byDataField_Dummy[1 + 1 + 1 + 1 + 1 + 2 + 3] ,szPbm_S_C , 2);
// 			}
// 
// 			if(!fnAPP_RXGetFindArryDataField(x095X, strlen(x095X), strGetRxDataField, PACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 			else
// 			{
// 				//���� �ڷ� �߰� ���� ���� 
// 				sprintf(szPbm_P_M, "%s", strGetRxDataField.GetBuffer(0));
// 				memcpy(&szPbmAreaKb[1 + 1 + 1], szPbm_P_M , 1);
// 				memcpy(&m_RD.byDataField_Dummy[1 + 1 + 1] ,szPbm_P_M , 1);
// 			}
// 
// 			if(!fnAPP_RXGetFindArryDataField(x094X, strlen(x094X), strGetRxDataField, UNPACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x094X[%s]" , x094X);
// 			else
// 				cstrTmp = strGetRxDataField;
// 
// 			if(!fnAPP_RXGetFindArryDataField(x0941, strlen(x0941), strGetRxDataField, UNPACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x094X[%s]" , x0941);
// 			else
// 				cstrTmp += strGetRxDataField;
// 
// 			if(!fnAPP_RXGetFindArryDataField(x0942, strlen(x0942), strGetRxDataField, UNPACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x094X[%s]" , x0942);
// 			else
// 				cstrTmp += strGetRxDataField;
// 
// 			if(!fnAPP_RXGetFindArryDataField(x0943, strlen(x0943), strGetRxDataField, UNPACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x094X[%s]" , x0943);
// 			else
// 				cstrTmp += strGetRxDataField;
// 
// 
// 			if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x022X[%s]" , x022X);
// 			else
// 				cstrTmp += strGetRxDataField;
// 
// 			if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField, UNPACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x019X[%s]" , x019X);
// 			else
// 			{
// 				cstrTmp += strGetRxDataField;
// 				HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField - x019" , &strGetRxDataField, strGetRxDataField.GetLength());
// 			}
// 
// 			nPacklen = MakePack(cstrTmp.GetBuffer(0), &szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3 + 2 + 1], cstrTmp.GetLength());
// 			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szPbmAreaKb, nPacklen);
// 
// 			int nRecvEncData = 0;
// 			nRecvEncData = cstrTmp.GetLength()/2;
// 
// 			Int2Asc(nRecvEncData, m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen), 10, '0');
// 			memcpy(&m_SRXEncBuff.byEnDummy, &szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3 + 2 + 1], nRecvEncData);
// 
// 			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-1", m_SRXEncBuff.byEnDummy, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
// 			
// 			int nEncType = fnAPP_DecType(nTranCode);
// 			fnAPP_DecCall(nEncType, nTranCode);
// 
// 			if (memcmp(m_SRXEncBuff.byPlaintMsdata, "XXXXXXXXXXXXXX",14) == 0)
// 			{
// 				Accept.Index = 0;
// 				PbNextTranFlag = FALSE;
// 				return T_OK;
// 			}
// 
// 			memcpy(&szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3 + 2 +1], m_SRXEncBuff.byDeDummy, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));	
// 			HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szPbmAreaKb, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
// 				//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 			if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0226
// 				fnAPP_KB_RecvPPPGet(szPbmAreaKb, &PPPRecvDataKb, sizeof(szPbmAreaKb));
// 		}
// 		else
// 		{
// 			if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x022X[%s]" , x022X);
// 			else
// 				nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 			if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x019X[%s]" , x019X);
// 			else
// 				nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 			fnAPP_DecCall(nEncType, TRANID_4410); //�����ܾ�	
// 		
// 		}
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4410);
// 
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// }    
//    
// // ����ü �Ա�
// int	CTranCmn::fn_FRX_TRANID_4716(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.MuAccountNum, 4))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.MuAccountNum, 4))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_4410);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4410);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4410);
// 
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4716: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// } 
//    
// int	CTranCmn::fn_FRX_TRANID_4711(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4712(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// //ī���Ա�-IC
// int	CTranCmn::fn_FRX_TRANID_4713(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_4713);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[16], strGetRxDataField1.GetLength());  //#N0226 �Աݽÿ� 0221�ʵ� 16����Ʈ
// 
// 		fnAPP_DecCall(nEncType, TRANID_4713);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_4713);
// 
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4713: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4719(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// //���� ����    
// int	CTranCmn::fn_FRX_TRANID_4730(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrPbmDate1("");
// 	CString cstrPbmDate2("");
// 	CString cstrPbmDate3("");
// 	CString cstrPbmDate4("");
// 	CString cstrPbmDate5("");
// 	CString cstrPbmDate6("");
// 	CString cstrPbmDate7("");
// 	CString cstrPbmDate8("");
// 
// 	CString cstrTmp("");
// 	char szPbm_P_N[2+1]; //����������󿩺�
// 	char szPbm_P_F[4+1];
// 	char szPbm_P_L[2+1];
// 	char szPbm_S_L[3+1];
// 	char szPbm_S_C[2+1];
// 	char szPbm_P_M[2+1]; //�����̿� ��� �ȳ�
// 
// 	char szPbm_Ptn_N[3+1];
// 	char szPbm_sPtn_N[2+1];
// 	char szPbm_Data_Len[4+1];
// 
// 	char szPbmArea[4 + 2 + 2 + 2 + (120 * 76)];
// 	char szPbmAreaNonGhyup[4 + 2 + 2 + 2 + (120 * 76)];
// 	char szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3 + 2 + 3600 + 80 + 200]; //�������ʰŷ�(1)+������󿩺�(1)+���������߰�(1)+����ŷ��ȳ�(1)+�����ܾ�����(1)+������(2)+������(3)+���Ǽ�(2)+3600+72	
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 	int nTranModeReg = 0;
// 	memset(szPbm_P_F, 0x00, sizeof(szPbm_P_F));
// 	memset(szPbm_P_L, 0x00, sizeof(szPbm_P_L));
// 	memset(szPbm_P_N, 0x00, sizeof(szPbm_P_N));		
// 
// 	memset(szPbm_P_M, 0x00, sizeof(szPbm_P_M));	
// 	memset(szPbm_S_L, 0x00, sizeof(szPbm_S_L));
// 	memset(szPbm_S_C, 0x00, sizeof(szPbm_S_C));
// 
// 	memset(szPbm_Ptn_N, 0x00, sizeof(szPbm_Ptn_N));
// 	memset(szPbm_sPtn_N, 0x00, sizeof(szPbm_sPtn_N));
// 	memset(szPbm_Data_Len, 0x00, sizeof(szPbm_Data_Len));
// 
// 	PbPrintAfterMsgFlag = FALSE;
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.AccountNum, 10))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.AccountNum, 10))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 
// 	//NONGHYUP == x960x, x970, x230x,  == �����ʿ�!!
// 	
// 	if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x092X, strlen(x092X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� ���� ��
// 			sprintf(szPbm_S_L, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmArea[4+2] ,szPbm_S_L, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x093X, strlen(x093X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� ��� �Ǽ�  #SJ
// 			sprintf(szPbm_S_C, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmArea[4+2+2] ,szPbm_S_C, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x095X, strlen(x095X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			
// 			//���� �ڷ� �߰� ���� ���� 
// 			sprintf(szPbm_P_F, "%s", strGetRxDataField.GetBuffer(0));
// 			if(memcmp(szPbm_P_F, "1", 1) == 0)
// 			{
// 
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 					Delay_Msg(2000);
// 				}
// 				else
// 				{
// 					PbNextTranFlag = TRUE; //#0024
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 
// 			}
// 			else 
// 			if(memcmp(szPbm_P_F, "2", 1) == 0)		
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "[�����̿�]");
// 					m_pDevCmn->fnSCR_DisplayString(3, "â������ ������ ��߱� �����ʽÿ� ");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				else
// 				{
// 					PbPrintAfterMsgFlag = TRUE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				Delay_Msg(2000);
// 			}
// 			else
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 				}
// 				else
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ� ");
// 				}
// 				m_pDevCmn->fnSCR_DisplayScreen(701);
// 			}
// 			Delay_Msg(2000);
// 		}
// 
// 		//�������� ����
// 		if(!fnAPP_RXGetFindArryDataField(x231X, strlen(x231X), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x231X[%s]" , x231X);
// 		else
// 		{
// 			cstrTmp = strGetRxDataField;
// 			if(!fnAPP_RXGetFindArryDataField(x232X, strlen(x232X), strGetRxDataField, UNPACK_F))
// 				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x232X[%s]" , x232X);
// 			else
// 				cstrTmp = cstrTmp + strGetRxDataField;
// 			nPacklen = MakePack(cstrTmp.GetBuffer(0), &szPbmArea[4+2+2+2], cstrTmp.GetLength());
// 		}
// 
// 		HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730", szPbmArea, sizeof(szPbmArea));
// 
// 	}
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
// 	{
// 
// 		if(!fnAPP_RXGetFindArryDataField(x092X, strlen(x092X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x092X[%s]" , x092X);
// 		else
// 		{
// 			//���� ���� ��
// 			sprintf(szPbm_S_L, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmAreaNonGhyup[4+2] ,szPbm_S_L, 2);
// 			memcpy(&PPPRecvDataNon.SkipLineCnt[0], szPbm_S_L, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x093X, strlen(x093X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x093X[%s]" , x093X);
// 		else
// 		{
// 			//���� ��� �Ǽ�
// 			sprintf(szPbm_S_C, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmAreaNonGhyup[4+2+2] ,szPbm_S_C, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x096X, strlen(x096X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x096X[%s]" , x096X);
// 		else
// 		{
// 			//�������Ϲ�ȣ 
// 			Sprintf(szPbm_Ptn_N, 3, "%03d", Asc2Int(strGetRxDataField.GetBuffer(0)));
// 			memcpy(&szPbmAreaNonGhyup[0] ,szPbm_Ptn_N, 3);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x097X, strlen(x097X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x097X[%s]" , x097X);
// 		else
// 		{
// 			//���弭������
// 			Sprintf(szPbm_sPtn_N, 2, "%02d", Asc2Int(strGetRxDataField.GetBuffer(0)));
// 			memcpy(&szPbmAreaNonGhyup[2] ,szPbm_sPtn_N, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x230X, strlen(x230X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x230X[%s]" , x230X);
// 		else
// 		{
// 			//���嵥���� ����
// 			Sprintf(szPbm_Data_Len, 4, "%04d", Asc2Int(strGetRxDataField.GetBuffer(0)));
// 		}
// 
// 		//�������� ����
// 		if(!fnAPP_RXGetFindArryDataField(x231X, strlen(x231X), cstrPbmDate1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x231X[%s]" , x231X);
// 		else
// 			cstrTmp = cstrPbmDate1;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x232X, strlen(x232X), cstrPbmDate2, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x232X[%s]" , x232X);
// 		else
// 			cstrTmp += cstrPbmDate2;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x233X, strlen(x233X), cstrPbmDate3, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x233X[%s]" , x233X);
// 		else
// 			cstrTmp += cstrPbmDate3;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x234X, strlen(x234X), cstrPbmDate4, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x234X[%s]" , x234X);
// 		else
// 			cstrTmp += cstrPbmDate4;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x235X, strlen(x235X), cstrPbmDate5, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x235X[%s]" , x235X);
// 		else
// 			cstrTmp += cstrPbmDate5;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x236X, strlen(x236X), cstrPbmDate6, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x236X[%s]" , x236X);
// 		else
// 			cstrTmp += cstrPbmDate6;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x237X, strlen(x237X), cstrPbmDate7, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x237X[%s]" , x237X);
// 		else
// 			cstrTmp += cstrPbmDate7;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x238X, strlen(x238X), cstrPbmDate8, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x238X[%s]" , x238X);
// 		else
// 			cstrTmp += cstrPbmDate8;
// 
// 		nTranModeReg = RegGetInt(_REGKEY_DEVICE, "TRANMODE", 0);
// 		if(nTranModeReg != 1)
// 		{
// 			memcpy(m_SRXEncBuff.byOutLen, szPbm_Data_Len, sizeof(m_SRXEncBuff.byOutLen));
// 			nPacklen = MakePack(cstrTmp.GetBuffer(0), &m_SRXEncBuff.byEnDummy, cstrTmp.GetLength());
// 			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-1", m_SRXEncBuff.byEnDummy, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
// 			nEncType = fnAPP_DecType(TRANID_4730);
// 			fnAPP_DecCall(nEncType, TRANID_4730);
// 			//���� Data	
// 			memcpy(&szPbmAreaNonGhyup[4+2+2+2], m_SRXEncBuff.byDeDummy, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));	
// 			HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szPbmAreaNonGhyup, sizeof(szPbmAreaNonGhyup));
// 		}
// 		else
// 		{
// 			nPacklen = MakePack(cstrTmp.GetBuffer(0), &szPbmAreaNonGhyup[4+2+2+2], cstrTmp.GetLength());
// 			//memcpy(&szPbmAreaNonGhyup[4+2+2+2], cstrTmp.GetBuffer(0), cstrTmp.GetLength());	
// 			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szPbmAreaNonGhyup, sizeof(szPbmAreaNonGhyup));
// 		}
// 	}
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == KEB) //#0085
// 	{
// 
// 		if(!fnAPP_RXGetFindArryDataField(x092X, strlen(x092X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� ���� ��
// 			sprintf(szPbm_S_L, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmArea[4+2] ,szPbm_S_L, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x093X, strlen(x093X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� ��� �Ǽ�  #SJ
// 			sprintf(szPbm_S_C, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmArea[4+2+2] ,szPbm_S_C, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x095X, strlen(x095X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			
// 			//���� �ڷ� �߰� ���� ���� 
// 			sprintf(szPbm_P_F, "%s", strGetRxDataField.GetBuffer(0));
// 			if(memcmp(szPbm_P_F, "1", 1) == 0)
// 			{
// 
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 					Delay_Msg(2000);
// 				}
// 				else
// 				{
// 					PbNextTranFlag = TRUE; //#0024
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 
// 			}
// 			else 
// 			if(memcmp(szPbm_P_F, "2", 1) == 0)		
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "[�����̿�]");
// 					m_pDevCmn->fnSCR_DisplayString(3, "â������ ������ ��߱� �����ʽÿ� ");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				else
// 				{
// 					PbPrintAfterMsgFlag = TRUE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				Delay_Msg(2000);
// 			}
// 			else
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 				}
// 				else
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ� ");
// 				}
// 				m_pDevCmn->fnSCR_DisplayScreen(701);
// 			}
// 			Delay_Msg(2000);
// 		}
// 
// 
// 		//�������� ����
// 		if(!fnAPP_RXGetFindArryDataField(x231X, strlen(x231X), cstrPbmDate1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x231X[%s]" , x231X);
// 		else
// 			cstrTmp = cstrPbmDate1;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x232X, strlen(x232X), cstrPbmDate2, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x232X[%s]" , x232X);
// 		else
// 			cstrTmp += cstrPbmDate2;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x233X, strlen(x233X), cstrPbmDate3, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x233X[%s]" , x233X);
// 		else
// 			cstrTmp += cstrPbmDate3;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x234X, strlen(x234X), cstrPbmDate4, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x234X[%s]" , x234X);
// 		else
// 			cstrTmp += cstrPbmDate4;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x235X, strlen(x235X), cstrPbmDate5, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x235X[%s]" , x235X);
// 		else
// 			cstrTmp += cstrPbmDate5;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x236X, strlen(x236X), cstrPbmDate6, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x236X[%s]" , x236X);
// 		else
// 			cstrTmp += cstrPbmDate6;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x237X, strlen(x237X), cstrPbmDate7, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x237X[%s]" , x237X);
// 		else
// 			cstrTmp += cstrPbmDate7;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x238X, strlen(x238X), cstrPbmDate8, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x238X[%s]" , x238X);
// 		else
// 			cstrTmp += cstrPbmDate8;
// 
// 		nPacklen = MakePack(cstrTmp.GetBuffer(0), &szPbmArea[4+2+2+2], cstrTmp.GetLength());
// 		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szPbmArea, nPacklen);
// 	}
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU) //#N0181
// 	{
// 
// 		if(!fnAPP_RXGetFindArryDataField(x092X, strlen(x092X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� ���� ��
// 			sprintf(szPbm_S_L, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmArea[4+2] ,szPbm_S_L, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x093X, strlen(x093X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� ��� �Ǽ�  #SJ
// 			sprintf(szPbm_S_C, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmArea[4+2+2] ,szPbm_S_C, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x095X, strlen(x095X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� �ڷ� �߰� ���� ���� 
// 			sprintf(szPbm_P_F, "%s", strGetRxDataField.GetBuffer(0));
// 			if(memcmp(szPbm_P_F, "1", 1) == 0)
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 					Delay_Msg(2000);
// 				}
// 				else
// 				{
// 					PbNextTranFlag = TRUE; //#0024
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 			}
// 			else 
// 			if(memcmp(szPbm_P_F, "2", 1) == 0)		
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "[�����̿�]");
// 					m_pDevCmn->fnSCR_DisplayString(3, "â������ ������ ��߱� �����ʽÿ� ");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				else
// 				{
// 					PbPrintAfterMsgFlag = TRUE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				Delay_Msg(2000);
// 			}
// 			else
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 				}
// 				else
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ� ");
// 				}
// 				m_pDevCmn->fnSCR_DisplayScreen(701);
// 			}
// 			Delay_Msg(2000);
// 		}
// 
// 
// 		//�������� ����
// 		if(!fnAPP_RXGetFindArryDataField(x231X, strlen(x231X), cstrPbmDate1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x231X[%s]" , x231X);
// 		else
// 			cstrTmp = cstrPbmDate1;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x232X, strlen(x232X), cstrPbmDate2, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x232X[%s]" , x232X);
// 		else
// 			cstrTmp += cstrPbmDate2;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x233X, strlen(x233X), cstrPbmDate3, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x233X[%s]" , x233X);
// 		else
// 			cstrTmp += cstrPbmDate3;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x234X, strlen(x234X), cstrPbmDate4, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x234X[%s]" , x234X);
// 		else
// 			cstrTmp += cstrPbmDate4;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x235X, strlen(x235X), cstrPbmDate5, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x235X[%s]" , x235X);
// 		else
// 			cstrTmp += cstrPbmDate5;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x236X, strlen(x236X), cstrPbmDate6, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x236X[%s]" , x236X);
// 		else
// 			cstrTmp += cstrPbmDate6;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x237X, strlen(x237X), cstrPbmDate7, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x237X[%s]" , x237X);
// 		else
// 			cstrTmp += cstrPbmDate7;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x238X, strlen(x238X), cstrPbmDate8, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x238X[%s]" , x238X);
// 		else
// 			cstrTmp += cstrPbmDate8;
// 
// 		nPacklen = MakePack(cstrTmp.GetBuffer(0), &szPbmArea[4+2+2+2], cstrTmp.GetLength());
// 		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szPbmArea, nPacklen);
// 	}
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == HANA) //#N0214
// 	{
// 
// 		//�������� ����
// 		if(!fnAPP_RXGetFindArryDataField(x231X, strlen(x231X), cstrPbmDate1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x231X[%s]" , x231X);
// 		else
// 			cstrTmp = cstrPbmDate1;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x232X, strlen(x232X), cstrPbmDate2, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x232X[%s]" , x232X);
// 		else
// 			cstrTmp += cstrPbmDate2;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x233X, strlen(x233X), cstrPbmDate3, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x233X[%s]" , x233X);
// 		else
// 			cstrTmp += cstrPbmDate3;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x234X, strlen(x234X), cstrPbmDate4, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x234X[%s]" , x234X);
// 		else
// 			cstrTmp += cstrPbmDate4;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x235X, strlen(x235X), cstrPbmDate5, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x235X[%s]" , x235X);
// 		else
// 			cstrTmp += cstrPbmDate5;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x236X, strlen(x236X), cstrPbmDate6, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x236X[%s]" , x236X);
// 		else
// 			cstrTmp += cstrPbmDate6;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x237X, strlen(x237X), cstrPbmDate7, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x237X[%s]" , x237X);
// 		else
// 			cstrTmp += cstrPbmDate7;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x238X, strlen(x238X), cstrPbmDate8, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x238X[%s]" , x238X);
// 		else
// 			cstrTmp += cstrPbmDate8;
// 
// 		//cstrTmp.Empty();
// 		//cstrTmp= "D1620140530��ü    �ݵ忹Ź������            54    *10,000,054#981                 D1720140530��ü    �ݵ忹Ź������            54    *10,000,108#981                 D1820140530��ü    �ݵ忹Ź������            54    *10,000,162#981                 D1920140530���ԣ͡�       100,500����               *9,899,662#801                 D2020140530���ԣ͡�       500,000����۱�           *9,399,662#801                 D21         (�ϳ�  98191000117708   �α��� )                                       D2220140530���ԣ͡�����                 140,000     *9,539,662#801                 D2320140617��ü    �ֹ���                10,000     *9,549,662#036                 D2420140617��ü    �ֹ���                10,000     *9,559,662#036                 M25---------- �� �������� ���������� �Ϸ�Ǿ����ϴ� ----------                     M01���� ������ ����                                                                ";
// 		//strTmp += "D1620140530��ü    �ݵ忹Ź������            54    *10,000,054#981                 D1620140530��ü    �ݵ忹Ź������            54    *10,000,054#981                 D1620140530��ü    �ݵ忹Ź������            54    *10,000,054#981                 D1620140530��ü    �ݵ忹Ź������            54    *10,000,054#981                 D1620140530��ü    �ݵ忹Ź������            54    *10,000,054#981                 ";
// 		
// 		//nPacklen = MakePack(cstrTmp.GetBuffer(0), &szPbmArea[4+2+2+2], cstrTmp.GetLength());
// 		//HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szPbmArea, nPacklen);
// 
// 		char szTmpPbmArea [200][83];
// 		int LineCnt =0,  CopyCnt = 0;
// 		memset(szTmpPbmArea, 0x20, sizeof(szTmpPbmArea));
// 		CopyCnt = 4+2+2+2;
// 		
// 		nPacklen = MakePack(cstrTmp.GetBuffer(0), szTmpPbmArea, cstrTmp.GetLength());
// 		//memcpy(szTmpPbmArea, cstrTmp.GetBuffer(0), __min(sizeof(szTmpPbmArea), cstrTmp.GetLength()));
// 		//memcpy(szTmpPbmArea, &szPbmArea[4+2+2+2], sizeof(szTmpPbmArea));
// 		memset(szPbmArea, 0x00, sizeof(szPbmArea));
// 		
// 		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szTmpPbmArea, sizeof(szTmpPbmArea));
// 
// 		if(!fnAPP_RXGetFindArryDataField(x093X, strlen(x093X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� ��� �Ǽ�  #SJ
// 			sprintf(szPbm_S_C, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmArea[4+2+2] ,szPbm_S_C, 2);
// 		}
// 
// 		if(szTmpPbmArea[0][0] == 'D')
// 		{
// 			//���� ���� ��
// 			memcpy(szPbm_S_L, &szTmpPbmArea[0][1], 2);
// 			memcpy(&szPbmArea[4+2] ,szPbm_S_L, 2);
// 
// 			for(int i = 0 ; i < 30; i++)
// 			{
// 				if(szTmpPbmArea[i][0] == 'D')
// 				{
// 					LineCnt ++;
// 					memcpy(&szPbmArea[CopyCnt], &szTmpPbmArea[i][0], 83); // 1���� ����
// 					CopyCnt += 83;
// 				}
// 				else
// 				if(szTmpPbmArea[i][0] == 'M') //#0099
// 				{
// 					if (memcmp(&szTmpPbmArea[i][1], "00", 2) == 0) 
// 					{
// 						Accept.Index = 99;
// 						memcpy(Accept.CrmDummy, &szTmpPbmArea[i][3], 80);
// 						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPPGet PBM M00 -> Msg[%s]", strCT(Accept.CrmDummy,80));
// 					}
// 					else
// 					if (memcmp(&szTmpPbmArea[i][1], "01", 2) == 0)
// 					{
// 						PbNextTranFlag = TRUE;
// 						LineCnt ++;
// 						memcpy(&szPbmArea[CopyCnt], &szTmpPbmArea[i][0], 83); // 1���� ����
// 						CopyCnt += 83;
// 					}
// 					else
// 					if (memcmp(&szTmpPbmArea[i][1], "25", 2) == 0)
// 					{
// 						LineCnt ++;
// 						memcpy(&szPbmArea[CopyCnt], &szTmpPbmArea[i][0], 83); // 1���� ����
// 						CopyCnt += 83;
// 					}
// 				}
// 					
// 			}
// 		}
// 		else
// 		{
// 			sprintf(szPbm_S_C, "%2.2s", "00");  // ���峻�� ����.
// 		}
// 		
// 
// 		//���� ��� �Ǽ�  #SJ
// 		/*
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "cstrTmp == [%s]", cstrTmp);
// 		sprintf(szPbm_S_C, "%2d", LineCnt);
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "cstrTmp == [%d][%d]",cstrTmp.GetLength(),  LineCnt);
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "cstrTmp == [%2.2s]",szPbm_S_C);
// 		memcpy(&szPbmArea[4+2+2] ,"16", 2);
// 		*/
// 
// 
// 		
// 		if (memcmp(szPbm_S_C, "00", 2) == 0)
// 		{
// 			PbNextTranFlag = FALSE;
// 			m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 			m_pDevCmn->fnSCR_DisplayString(2, "�������� ������ �����ϴ�");
// 		}
// 		else
// 		if (memcmp(szPbm_S_C, "01", 2) == 0 && memcmp(cstrTmp, "M02", 3) == 0)
// 		{
// 			PbNextTranFlag = FALSE;
// 			m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 			m_pDevCmn->fnSCR_DisplayString(2, "�������� ������ �����ϴ�");
// 		}
// 		else
// 		{
// 			m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 			m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ� ");
// 		}
// 		m_pDevCmn->fnSCR_DisplayScreen(701);
// 
// 		Delay_Msg(2000);	
// 	}
// 	else
// 	if (m_pProfile->DEVICE.ETCDevice20 == KB)  //#N0226
// 	{
// 
// 		memset(szPbmAreaKb, 0x00, sizeof(szPbmAreaKb));
// 		
// 
// 		if(!fnAPP_RXGetFindArryDataField(x088X, strlen(x088X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x088X[%s]" , x088X);
// 		else
// 		{
// 			//���� ���� ���� ����
// 			sprintf(szPbm_P_N, "%1.1s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmAreaKb[1],szPbm_P_N , 1);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x089X, strlen(x089X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x089X[%s]" , x089X);
// 		else
// 		{
// 			//���� �̿� ��� ����
// 			sprintf(szPbm_P_F, "%1.1s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmAreaKb[1+1], szPbm_P_F , 1);
// 		}
// 		
// 		if(!fnAPP_RXGetFindArryDataField(x092X, strlen(x092X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� ���� ��
// 			sprintf(szPbm_S_L, "%3.3s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2], szPbm_S_L, 3);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x093X, strlen(x093X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� ��� �Ǽ�  #SJ
// 			sprintf(szPbm_S_C, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3], szPbm_S_C, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x095X, strlen(x095X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			
// 			//���� �ڷ� �߰� ���� ���� 
// 			sprintf(szPbm_P_M, "%s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmAreaKb[2], szPbm_P_M , 1);			
// 		}
// 
// 		if(memcmp(szPbm_P_N, "0", 1) == 0) //�������� ��� ���� -> "0" : �������� ����, "1" : �������� ����
// 		{
// 			Accept.Index = 0;
// 			PbNextTranFlag = FALSE;
// 			m_pDevCmn->fnSCR_DisplayString(1, "���� ������ ������ �����ϴ�");
// 			m_pDevCmn->fnSCR_DisplayString(3, "�̿��� �ּż� �����մϴ�.");
// 			m_pDevCmn->fnSCR_DisplayScreen(701);
// 			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�������� �������]"));
// 			Delay_Msg(3000);
// 		}
// 		else
// 		if (memcmp(szPbm_P_M, "1", 1) == 0) //����ŷ� ȭ�� �ȳ� �޽��� -> "1" : ���� �̿� ��� �ȳ�
// 		{
// 			Accept.Index = 0;
// 			PbNextTranFlag = FALSE;
// 			m_pDevCmn->fnSCR_DisplayString(1, "�� �������δ� ���̻� ���� ������ �� �� �����ϴ� ");
// 			m_pDevCmn->fnSCR_DisplayString(2, "������ ������ �湮�Ͻþ�");
// 			m_pDevCmn->fnSCR_DisplayString(3, "������ ���� �߱� �����ñ� �ٶ��ϴ�.");
// 			m_pDevCmn->fnSCR_DisplayScreen(701);
// 			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����̿����]"));
// 			Delay_Msg(3000);
// 		}
// 		else 
// 		{
// 			if (memcmp(szPbm_P_F, "1", 1) == 0) //�������� �߰� ���� ���� -> "0" : �߰��ڷ� ����, "1' : �߰��ڷ� ����
// 			{
// 				PbNextTranFlag = TRUE;			//#H0227 -> �������� �߰� ���� ����
// 				Accept.Index = 99;
// 			}
// 			else
// 			{
// 				PbNextTranFlag = FALSE;	
// 				Accept.Index = 0;
// 			}
// 
// 			if (memcmp(m_SRXEncBuff.byPlaintMsdata, "XXXXXXXXXXXXXX",14) == 0)
// 			{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "�˼��մϴ� ���������� �Ϸ���� �ʾҽ��ϴ�");
// 					m_pDevCmn->fnSCR_DisplayString(2, "������ ������ �湮�Ͻþ�");
// 					m_pDevCmn->fnSCR_DisplayString(3, "�������� ������ Ȯ���Ͻñ� �ٶ��ϴ�.");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 					Delay_Msg(2000);
// 					Accept.Index = 0;
// 			}
// 			else
// 			if (memcmp(szPbm_S_C, "00", 2) == 0) //�������� ��� �Ǽ� -> MAX : 45
// 			{
// 				PbNextTranFlag = FALSE;
// 				m_pDevCmn->fnSCR_DisplayString(1, "���� ������ ������ �����ϴ�");
// 				m_pDevCmn->fnSCR_DisplayString(3, "�̿��� �ּż� �����մϴ�.");
// 				m_pDevCmn->fnSCR_DisplayScreen(701);
// 				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�������� �������]"));
// 				Delay_Msg(2000);
// 			}
// 			else
// 			{
// 				m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 				m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 				m_pDevCmn->fnSCR_DisplayScreen(701);
// 			}					
// 		}
// 		
// 		//�������� ����
// 		/*
// 		if(!fnAPP_RXGetFindArryDataField(x231X, strlen(x231X), cstrPbmDate1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x231X[%s]" , x231X);
// 		else
// 			cstrTmp = cstrPbmDate1;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x232X, strlen(x232X), cstrPbmDate2, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x232X[%s]" , x232X);
// 		else
// 			cstrTmp += cstrPbmDate2;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x233X, strlen(x233X), cstrPbmDate3, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x233X[%s]" , x233X);
// 		else
// 			cstrTmp += cstrPbmDate3;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x234X, strlen(x234X), cstrPbmDate4, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x234X[%s]" , x234X);
// 		else
// 			cstrTmp += cstrPbmDate4;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x235X, strlen(x235X), cstrPbmDate5, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x235X[%s]" , x235X);
// 		else
// 			cstrTmp += cstrPbmDate5;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x236X, strlen(x236X), cstrPbmDate6, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x236X[%s]" , x236X);
// 		else
// 			cstrTmp += cstrPbmDate6;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x237X, strlen(x237X), cstrPbmDate7, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x237X[%s]" , x237X);
// 		else
// 			cstrTmp += cstrPbmDate7;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x238X, strlen(x238X), cstrPbmDate8, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x238X[%s]" , x238X);
// 		else
// 			cstrTmp += cstrPbmDate8;
// */
// 		if(!fnAPP_RXGetFindArryDataField(x094X, strlen(x094X), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x094X[%s]" , x094X);
// 		else
// 			cstrTmp = strGetRxDataField;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x0941, strlen(x0941), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x094X[%s]" , x0941);
// 		else
// 			cstrTmp += strGetRxDataField;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x0942, strlen(x0942), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x094X[%s]" , x0942);
// 		else
// 			cstrTmp += strGetRxDataField;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x0943, strlen(x0943), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x094X[%s]" , x0943);
// 		else
// 			cstrTmp += strGetRxDataField;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x238X[%s]" , x238X);
// 		else
// 			cstrTmp += strGetRxDataField;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 		{
// 			cstrTmp += strGetRxDataField;
// 			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4720: DataField - x019" , &strGetRxDataField, strGetRxDataField.GetLength());
// 		}
// 
// 		nPacklen = MakePack(cstrTmp.GetBuffer(0), &szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3 + 2 + 1], cstrTmp.GetLength());
// 		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szPbmAreaKb, nPacklen);
// 
// 
// 
// 		 
// 
// 		int nRecvEncData = 0;
// 		nRecvEncData = cstrTmp.GetLength()/2 ;
// 
// 		Int2Asc(nRecvEncData, m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen), 10, '0');
// 		memcpy(&m_SRXEncBuff.byEnDummy, &szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3 + 2 + 1], nRecvEncData);
// 
// 		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-1", m_SRXEncBuff.byEnDummy, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
// 		
// 		int nEncType = fnAPP_DecType(nTranCode);
// 		fnAPP_DecCall(nEncType, nTranCode);
// 
// 		if (memcmp(m_SRXEncBuff.byPlaintMsdata, "XXXXXXXXXXXXXX",14) == 0)
// 		{
// 			m_pDevCmn->fnSCR_DisplayString(1, "�˼��մϴ� ���������� �Ϸ���� �ʾҽ��ϴ�");
// 			m_pDevCmn->fnSCR_DisplayString(2, "������ ������ �湮�Ͻþ�");
// 			m_pDevCmn->fnSCR_DisplayString(3, "�������� ������ Ȯ���Ͻñ� �ٶ��ϴ�.");
// 			m_pDevCmn->fnSCR_DisplayScreen(701);
// 			Delay_Msg(2000);
// 			Accept.Index = 0;
// 			PbNextTranFlag = FALSE;
// 			return T_OK;
// 		}
// 
// 		memcpy(&szPbmAreaKb[1 + 1 + 1 + 1 + 1 + 2 + 3 + 2 +1], m_SRXEncBuff.byDeDummy, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));	
// 		HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szPbmAreaKb, Asc2Int(m_SRXEncBuff.byOutLen, sizeof(m_SRXEncBuff.byOutLen)));
// 			//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);	
// 	}
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == CITI) //#N0252
// 	{
// 
// 		if(!fnAPP_RXGetFindArryDataField(x093X, strlen(x093X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x093X[%s]" , x0120);
// 		else
// 		{
// 			//���� ��� �Ǽ�  #SJ
// 			sprintf(szPbm_S_C, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmArea[4+2+2] ,szPbm_S_C, 2);
// 		}
// 
// 		/* �ѱ����ڱ��� ��Ƽ����������  fnAPP_CTB_RecvPPPGet �Լ� ������ �߰����� ���� ���翩�� �ľ���.
// 		if(!fnAPP_RXGetFindArryDataField(x095X, strlen(x095X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x095X[%s]" , x0120);
// 		else
// 		{
// 			
// 			//���� �ڷ� �߰� ���� ���� 
// 			sprintf(szPbm_P_F, "%s", strGetRxDataField.GetBuffer(0));
// 			if(memcmp(szPbm_P_F, "1", 1) == 0)
// 			{
// 
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 					Delay_Msg(2000);
// 				}
// 				else
// 				{
// 					PbNextTranFlag = TRUE; //#0024
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 
// 			}
// 			else 
// 			if(memcmp(szPbm_P_F, "2", 1) == 0)		
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "[�����̿�]");
// 					m_pDevCmn->fnSCR_DisplayString(3, "â������ ������ ��߱� �����ʽÿ� ");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				else
// 				{
// 					PbPrintAfterMsgFlag = TRUE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				Delay_Msg(2000);
// 			}
// 			else
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 				}
// 				else
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ� ");
// 				}
// 				m_pDevCmn->fnSCR_DisplayScreen(701);
// 			}
// 			Delay_Msg(2000);
// 		}
// 		*/
// 
// 		//�������� ����
// 		if(!fnAPP_RXGetFindArryDataField(x231X, strlen(x231X), cstrPbmDate1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x231X[%s]" , x231X);
// 		else
// 			cstrTmp = cstrPbmDate1;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x232X, strlen(x232X), cstrPbmDate2, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x232X[%s]" , x232X);
// 		else
// 			cstrTmp += cstrPbmDate2;
// 
// 		
// 		nPacklen = MakePack(cstrTmp.GetBuffer(0), &szPbmArea[4+2+2+2], cstrTmp.GetLength());
// 		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szPbmArea, nPacklen);
// 	}
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == SC)	//#N0245
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x092X, strlen(x092X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� ���� ��
// 			sprintf(szPbm_S_L, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmArea[4+2] ,szPbm_S_L, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x093X, strlen(x093X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			//���� ��� �Ǽ�  #SJ
// 			sprintf(szPbm_S_C, "%2.2s", strGetRxDataField.GetBuffer(0));
// 			memcpy(&szPbmArea[4+2+2] ,szPbm_S_C, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x095X, strlen(x095X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0120[%s]" , x0120);
// 		else
// 		{
// 			
// 			//���� �ڷ� �߰� ���� ���� 
// 			sprintf(szPbm_P_F, "%s", strGetRxDataField.GetBuffer(0));
// 			if(memcmp(szPbm_P_F, "1", 1) == 0)
// 			{
// 
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 					Delay_Msg(2000);
// 				}
// 				else
// 				{
// 					PbNextTranFlag = TRUE; //#0024
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 
// 			}
// 			else 
// 			if(memcmp(szPbm_P_F, "2", 1) == 0)		
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "[�����̿�]");
// 					m_pDevCmn->fnSCR_DisplayString(3, "â������ ������ ��߱� �����ʽÿ� ");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				else
// 				{
// 					PbPrintAfterMsgFlag = TRUE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				Delay_Msg(2000);
// 			}
// 			else
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 				}
// 				else
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ� ");
// 				}
// 				m_pDevCmn->fnSCR_DisplayScreen(701);
// 			}
// 			Delay_Msg(2000);
// 		}
// 
// 		//�������� ����
// 		if(!fnAPP_RXGetFindArryDataField(x231X, strlen(x231X), cstrPbmDate1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x231X[%s]" , x231X);
// 		else
// 			cstrTmp = cstrPbmDate1;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x232X, strlen(x232X), cstrPbmDate2, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x232X[%s]" , x232X);
// 		else
// 			cstrTmp += cstrPbmDate2;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x233X, strlen(x233X), cstrPbmDate3, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x233X[%s]" , x233X);
// 		else
// 			cstrTmp += cstrPbmDate3;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x234X, strlen(x234X), cstrPbmDate4, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x234X[%s]" , x234X);
// 		else
// 			cstrTmp += cstrPbmDate4;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x235X, strlen(x235X), cstrPbmDate5, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x235X[%s]" , x235X);
// 		else
// 			cstrTmp += cstrPbmDate5;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x236X, strlen(x236X), cstrPbmDate6, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x236X[%s]" , x236X);
// 		else
// 			cstrTmp += cstrPbmDate6;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x237X, strlen(x237X), cstrPbmDate7, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x237X[%s]" , x237X);
// 		else
// 			cstrTmp += cstrPbmDate7;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x238X, strlen(x238X), cstrPbmDate8, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x238X[%s]" , x238X);
// 		else
// 			cstrTmp += cstrPbmDate8;
// 
// 		nPacklen = MakePack(cstrTmp.GetBuffer(0), &szPbmArea[4+2+2+2], cstrTmp.GetLength());
// 
// 		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730", szPbmArea, sizeof(szPbmArea));
// 
// 	}
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0277
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x092X, strlen(x092X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x092X[%s]" , x092X);
// 		else
// 		{
// 			//���� ���� ��
// 			cstrTmp = strGetRxDataField.GetBuffer(0);
// 			sprintf(szPbm_S_L, "%2.2s", cstrTmp.Right(2));
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "cstrTmp == [%2.2s]",szPbm_S_L);
// 			memcpy(&szPbmArea[4+2] ,szPbm_S_L, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x093X, strlen(x093X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x093X[%s]" , x093X);
// 		else
// 		{
// 			//���� ��� �Ǽ�  #SJ
// 			cstrTmp = strGetRxDataField.GetBuffer(0);
// 			sprintf(szPbm_S_C, "%2.2s", cstrTmp.Right(2));
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "cstrTmp == [%2.2s]",szPbm_S_C);
// 			memcpy(&szPbmArea[4+2+2] ,szPbm_S_C, 2);
// 		}
// 
// 		if(!fnAPP_RXGetFindArryDataField(x0951, strlen(x0951), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x0951[%s]" , x0951);
// 		else
// 		{
// 			
// 			//���� �ڷ� �߰� ���� ���� 
// 			sprintf(szPbm_P_F, "%s", strGetRxDataField.GetBuffer(0));
// 			if(memcmp(szPbm_P_F, "2", 1) == 0)
// 			{
// 
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 					Delay_Msg(2000);
// 				}
// 				else
// 				{
// 					PbNextTranFlag = TRUE; //#0024
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 
// 			}
// 			else 
// 			if(memcmp(szPbm_P_F, "3", 1) == 0)		
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "[�����̿�]");
// 					m_pDevCmn->fnSCR_DisplayString(3, "â������ ������ ��߱� �����ʽÿ� ");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				else
// 				{
// 					PbPrintAfterMsgFlag = TRUE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
// 					m_pDevCmn->fnSCR_DisplayScreen(701);
// 				}
// 				Delay_Msg(2000);
// 			}
// 			else
// 			{
// 				if (memcmp(szPbm_S_C, "00", 2) == 0)
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� �� ������ �����ϴ�");
// 				}
// 				else
// 				{
// 					PbNextTranFlag = FALSE;
// 					m_pDevCmn->fnSCR_DisplayString(2, "���� ������ �Դϴ� ");
// 					m_pDevCmn->fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ� ");
// 				}
// 				m_pDevCmn->fnSCR_DisplayScreen(701);
// 			}
// 			Delay_Msg(2000);
// 		}
// 
// 		//�������� ����
// 		if(!fnAPP_RXGetFindArryDataField(x231X, strlen(x231X), cstrPbmDate1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x231X[%s]" , x231X);
// 		else
// 			cstrTmp = cstrPbmDate1;
// 
// 		if(!fnAPP_RXGetFindArryDataField(x232X, strlen(x232X), cstrPbmDate2, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730: DataField Not Found - x232X[%s]" , x232X);
// 		else
// 			cstrTmp += cstrPbmDate2;
// 
// 		nPacklen = MakePack(cstrTmp.GetBuffer(0), &szPbmArea[4+2+2+2], cstrTmp.GetLength());
// 		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-2", szPbmArea, nPacklen);
// 	}
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
// 	{
// 		PPPRecvDataNon.p = 0;		
// 		PPPRecvDataNon.y = 0;		
// 		PPPRecvDataNon.x = 0;
// 		if (Asc2Int(&PPPRecvDataNon.SkipLineCnt[0], 2) > 0)	
// 		{														        	// ���ΰ���
// 			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SkipLineCnt ���ΰ��� = %d", Asc2Int(&PPPRecvDataNon.SkipLineCnt[0], 2));
// 			PPPRecvDataNon.y = Asc2Int(&PPPRecvDataNon.SkipLineCnt[0], 2);	// �����
// 			PPPRecvDataNon.x = 0;
// 		}
// 
// 		for (int i = 0; i < Asc2Int(szPbm_S_C,2); i++)
// 		{
// 			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4730-3", &szPbmAreaNonGhyup[4+2+2+2 + i*120], 120);
// 			fnAPP_NONGHYUP_RecvPPPGet1(&szPbmAreaNonGhyup[4+2+2+2 + i*120], &PPPRecvDataNon, Asc2Int(szPbm_sPtn_N,2), 120);
// 
// 			PPPRecvDataNon.y ++;										// �����
// 			if (PPPRecvDataNon.y >= itemof(PPPRecvDataNon.Data[0]))		// ����üũ
// 			{
// 				PPPRecvDataNon.y -= 24;
// 				PPPRecvDataNon.p++;
// 			}
// 			PPPRecvDataNon.x = 0;
// 		}
// 	}
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
// 		fnAPP_IBK_RecvPPPGet(szPbmArea, &PPPRecvData, sizeof(szPbmArea));
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == KEB) //#0085
// 		fnAPP_KEB_RecvPPPGet(szPbmArea, &PPPRecvData, sizeof(szPbmArea));
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU) //#0085
// 		fnAPP_KJB_RecvPPPGet(szPbmArea, &PPPRecvData, sizeof(szPbmArea));
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == HANA)		//#N0214
// 		fnAPP_HANA_RecvPPPGet(szPbmArea, &PPPRecvData, sizeof(szPbmArea));
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0226
// 		fnAPP_KB_RecvPPPGet(szPbmAreaKb, &PPPRecvDataKb, sizeof(szPbmAreaKb));
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == CITI)		//#N0252
// 		fnAPP_CTB_RecvPPPGet(szPbmArea, &PPPRecvData, sizeof(szPbmArea));
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == SC)		//#N0245
// 		fnAPP_SC_RecvPPPGet(szPbmArea, &PPPRecvData, sizeof(szPbmArea));
// 	else
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)	//#N0277
// 		fnAPP_KNB_RecvPPPGet(szPbmArea, &PPPRecvData, sizeof(szPbmArea));
// 	else
// 		fnAPP_RecvPPPGet(szPbmArea, &PPPRecvData, sizeof(szPbmArea));
// 
// 	return T_OK;	
// }     
//    
// int	CTranCmn::fn_FRX_TRANID_4810(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4810: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4810: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		sprintf(m_RXLOCALTAXDATA.CustomerName, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4810: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4810: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4810: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//���ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x191X, strlen(x191X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4810: DataField Not Found - x191X[%s]" , x191X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4810: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x193X, strlen(x193X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4810: DataField Not Found - x193X[%s]" , x193X);
// 	else
// 	{
// 		memset(m_RXJIRODATA.JiroPayData, 0, sizeof(m_RXJIRODATA.JiroPayData));
// 		sprintf(m_RXJIRODATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4810: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4811(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		memcpy(m_RXLOCALTAXDATA.CustomerName, strGetRxDataField.GetBuffer(0), 16 );
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811 [%s]" , strGetRxDataField.GetBuffer(0));
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811 [%s]" , strCT(m_RXLOCALTAXDATA.CustomerName, 16));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x1931, strlen(x1931), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811: DataField Not Found - x1931[%s]" , x1931);
// 	else
// 	{
// 		memset(m_RXLOCALTAXDATA.JiroPayData, 0 , sizeof(m_RXLOCALTAXDATA.JiroPayData));
// 		sprintf(m_RXLOCALTAXDATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4811: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4812(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4812: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4812: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		memcpy(m_RXLOCALTAXDATA.CustomerName, strGetRxDataField.GetBuffer(0), 16 );
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4812 [%s]" , strGetRxDataField.GetBuffer(0));
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4812 [%s]" , strCT(m_RXLOCALTAXDATA.CustomerName, 16));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4812: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4812: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4812: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4812: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4812: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x1931, strlen(x1931), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4812: DataField Not Found - x1931[%s]" , x1931);
// 	else
// 	{
// 		memset(m_RXLOCALTAXDATA.JiroPayData, 0, sizeof(m_RXLOCALTAXDATA.JiroPayData));
// 		sprintf(m_RXLOCALTAXDATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4812: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4813(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4813: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4813: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		sprintf(m_RXLOCALTAXDATA.CustomerName, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4813: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 	
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4813: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4813: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4813: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//���ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x191X, strlen(x191X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4813: DataField Not Found - x191X[%s]" , x191X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4813: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x193X, strlen(x193X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4813: DataField Not Found - x193X[%s]" , x193X);
// 	else
// 	{
// 		memset(m_RXJIRODATA.JiroPayData, 0, sizeof(m_RXJIRODATA.JiroPayData));
// 		sprintf(m_RXJIRODATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4813: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;
// }        
// int	CTranCmn::fn_FRX_TRANID_4814(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4814: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4814: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		memcpy(m_RXLOCALTAXDATA.CustomerName, strGetRxDataField.GetBuffer(0), 16 );
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4814 [%s]" , strGetRxDataField.GetBuffer(0));
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4814 [%s]" , strCT(m_RXLOCALTAXDATA.CustomerName, 16));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4814: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4814: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4814: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4814: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4814: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x1931, strlen(x1931), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4814: DataField Not Found - x1931[%s]" , x1931);
// 	else
// 	{
// 		memset(m_RXLOCALTAXDATA.JiroPayData, 0 , sizeof(m_RXLOCALTAXDATA.JiroPayData));
// 		sprintf(m_RXLOCALTAXDATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4814: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4820(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4821(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4822(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4830(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4840(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x221X, strlen(x221X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7491: DataField Not Found - x221X[%s]" , x221X);
// 	else
// 	{
// 		sprintf(Accept.ETicketConfirmNum, "%8.8s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4670: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4850(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4350: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x221X, strlen(x221X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7491: DataField Not Found - x221X[%s]" , x221X);
// 	else
// 	{
// 		sprintf(Accept.ETicketConfirmNum, "%8.8s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }
// 
// int	CTranCmn::fn_FRX_TRANID_4717(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4841(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4851(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4842(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4852(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_4860(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7000(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7004(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7005(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7006(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// 
// //���� ��������ȸ     
// int	CTranCmn::fn_FRX_TRANID_7101(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.AccountNum, 4))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.AccountNum, 4))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_7210);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7210);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7101);
// 
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//#N0180
// 	if(!fnAPP_RXGetFindArryDataField(x345X, strlen(x345X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7101: DataField Not Found - x345X[%s]" , x345X);
// 	else
// 	{
// 		//����������Ʈ���� ����
// 		sprintf(m_RD.byDataField_345x, "%2.2s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }      
// 
// //����ü ��������ȸ
// int	CTranCmn::fn_FRX_TRANID_7106(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.MuAccountNum, 4))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_7210);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7210);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7210);
// 
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//#N0180
// 	if(!fnAPP_RXGetFindArryDataField(x345X, strlen(x345X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7106: DataField Not Found - x345X[%s]" , x345X);
// 	else
// 	{
// 		//����������Ʈ���� ����
// 		sprintf(m_RD.byDataField_345x, "%2.2s", strGetRxDataField.GetBuffer(0));
// 	}
// 	return T_OK;	
// }        
//   
// //��������ȸ
// int	CTranCmn::fn_FRX_TRANID_7103(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_7103);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7103);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7103);
// 		
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	//#N0180
// 	if(!fnAPP_RXGetFindArryDataField(x345X, strlen(x345X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7103: DataField Not Found - x345X[%s]" , x345X);
// 	else
// 	{
// 		//����������Ʈ���� ����
// 		sprintf(m_RD.byDataField_345x, "%2.2s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }
// 
// 
// //#N0171 �ؿ�ī�� ��������ȸ         
// int	CTranCmn::fn_FRX_TRANID_7104(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_7210);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7210);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7210);
// 
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7104: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	return T_OK;
// }
// 
// 
// //#N0171 �ؿ�ī�� ��������ȸ         
// int	CTranCmn::fn_FRX_TRANID_7105(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_7210);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7210);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7210);
// 
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7105: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	return T_OK;
// }        
// int	CTranCmn::fn_FRX_TRANID_7120(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7124(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7125(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// 
// //���� ��ü ��ȸ      
// int	CTranCmn::fn_FRX_TRANID_7261(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.AccountNum, 4))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.AccountNum, 14))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_7260);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7260);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7260);
// 
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	//#N0180
// 	if(!fnAPP_RXGetFindArryDataField(x345X, strlen(x345X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7261: DataField Not Found - x345X[%s]" , x345X);
// 	else
// 	{
// 		//����������Ʈ���� ����
// 		sprintf(m_RD.byDataField_345x, "%2.2s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//#N0214
// 	if(!fnAPP_RXGetFindArryDataField(x1721, strlen(x1721), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x1721[%s]" , x1721);
// 	else
// 	{
// 		//�ϳ����� ��� ������
// 		sprintf(m_RD.byDataField_1721, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	return T_OK;	
// } 
// //����ü ��ü ��ȸ
// int	CTranCmn::fn_FRX_TRANID_7276(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.MuAccountNum, 4))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.MuAccountNum, 4))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_7260);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7260);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7260);
// 
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	//#N0180
// 	if(!fnAPP_RXGetFindArryDataField(x345X, strlen(x345X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7276: DataField Not Found - x345X[%s]" , x345X);
// 	else
// 	{
// 		//����������Ʈ���� ����
// 		sprintf(m_RD.byDataField_345x, "%2.2s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//#N0214
// 	if(!fnAPP_RXGetFindArryDataField(x1721, strlen(x1721), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x1721[%s]" , x1721);
// 	else
// 	{
// 		//�ϳ����� ��� ������
// 		sprintf(m_RD.byDataField_1721, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	return T_OK;	
// }        
//        
// int	CTranCmn::fn_FRX_TRANID_7262(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }       
// //���������ȸ-IC 
// // int	CTranCmn::fn_FRX_TRANID_7260(int nTranCode, int nSec)
// // {
// // 	CString strGetRxDataField("");
// // 	CString strGetRxDataField1("");
// // 	CString cstrTmp("");
// // 	int nEncType = 0;
// // 	int nPacklen = 0;
// // 
// // 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// // 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// // 	{
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x017X[%s]" , x017X);
// // 	}
// // 	else
// // 	{
// // 		//���¹�ȣ	
// // //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x023X[%s]" , x023X);
// // 	else
// // 	{
// // 		//�ŷ��ݾ�	
// // 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x024X[%s]" , x024X);
// // 	else
// // 	{
// // 		//������	
// // 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// // 	nEncType = fnAPP_DecType(TRANID_7260);
// // 	if(nEncType == TRAN_ENC_006_01)
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x022X[%s]" , x022X);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// // 
// // 
// // 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x019X[%s]" , x019X);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// // 
// // 		fnAPP_DecCall(nEncType, TRANID_7260);
// // 		//�����ܾ�	
// // 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// // 
// // 	}
// // 	else
// // 	if(nEncType == TRAN_ENC_240_10)
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x0151[%s]" , x0151);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// // 
// // 		fnAPP_DecCall(nEncType, TRANID_7260);
// // 
// // 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x022X[%s]" , x022X);
// // 		else
// // 		{
// // 			//�����ܾ�	
// // 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// // 		}
// // 
// // 	}
// // 	else
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x022X[%s]" , x022X);
// // 		else
// // 		{
// // 			//�����ܾ�	
// // 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// // 		}
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x025X[%s]" , x025X);
// // 	else
// // 	{
// // 		//Ÿ����	
// // 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x021X[%s]" , x021X);
// // 	else
// // 	{
// // 		//������	
// // 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x020X[%s]" , x020X);
// // 	else
// // 	{
// // 		//�������	
// // 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x026X[%s]" , x026X);
// // 	else
// // 	{
// // 		m_pProfile->GetTransProfile();
// // 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// // 		//Host �Ϸù�ȣ	
// // 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// // 		m_pProfile->PutTransProfile();
// // 	}
// // 
// // 	
// // 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x0120[%s]" , x0120);
// // 	else
// // 	{
// // 		//������
// // 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// // 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// // 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// // 	}
// // 
// // 	//��ǥ ����Data
// // 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x104X[%s]" , x0120);
// // 	else
// // 	{
// // 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// // 	}
// // 
// // 	//#N0180
// // 	if(!fnAPP_RXGetFindArryDataField(x345X, strlen(x345X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x345X[%s]" , x345X);
// // 	else
// // 	{
// // 		//����������Ʈ���� ����
// // 		sprintf(m_RD.byDataField_345x, "%2.2s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	//#N0214
// // 	if(!fnAPP_RXGetFindArryDataField(x1721, strlen(x1721), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x1721[%s]" , x1721);
// // 	else
// // 	{
// // 		//�ϳ����� ��� ������
// // 		sprintf(m_RD.byDataField_1721, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 	
// // 	return T_OK;	
// // }        
// 
// int	CTranCmn::fn_FRX_TRANID_7225(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }
// 
// //���� �Ա� ������ȸ
// int	CTranCmn::fn_FRX_TRANID_7126(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.AccountNum, 4))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.AccountNum, 14))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.AccountNum, 4))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.AccountNum, 14))
// 			EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_7200);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7200);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7200);
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//#N0181 ���� �귣�� ���� ����
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		//���� ����	
// 		sprintf(m_RD.byDataField_018x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7126: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// }  
// //�Աݰ��ɿ�����ȸ
// int	CTranCmn::fn_FRX_TRANID_7236(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.MuAccountNum, 4))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.MuAccountNum, 4))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsSpace(&Accept.MuAccountNum, 14))
// 			EditString(&Accept.MuAccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// 	nEncType = fnAPP_DecType(TRANID_7200);
// 	if(nEncType == TRAN_ENC_006_01)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 
// 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x019X[%s]" , x019X);
// 		else
// 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7200);
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// 
// 	}
// 	else
// 	if(nEncType == TRAN_ENC_240_10)
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x0151[%s]" , x0151);
// 		else
// 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// 
// 		fnAPP_DecCall(nEncType, TRANID_7200);
// 	}
// 	else
// 	{
// 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x022X[%s]" , x022X);
// 		else
// 		{
// 			//�����ܾ�	
// 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 		}
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//#N0181 ���� �귣�� ���� ����
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		//���� ����	
// 		sprintf(m_RD.byDataField_018x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7236: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// }
// // �Աݰ��ɿ�����ȸ(IC)        
// // int	CTranCmn::fn_FRX_TRANID_7200(int nTranCode, int nSec)
// // {
// // 	CString strGetRxDataField("");
// // 	CString strGetRxDataField1("");
// // 	CString cstrTmp("");
// // 	int nEncType = 0;
// // 	int nPacklen = 0;
// // 
// // 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// // 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// // 	{
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x017X[%s]" , x017X);
// // 	}
// // 	else
// // 	{
// // 		//���¹�ȣ	
// // 		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x023X[%s]" , x023X);
// // 	else
// // 	{
// // 		//�ŷ��ݾ�	
// // 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x024X[%s]" , x024X);
// // 	else
// // 	{
// // 		//������	
// // 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		
// // 	nEncType = fnAPP_DecType(TRANID_7200);
// // 	if(nEncType == TRAN_ENC_006_01)
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x022X[%s]" , x022X);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// // 
// // 
// // 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x019X[%s]" , x019X);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_SRXEncBuff.byChiperMsdata[12], strGetRxDataField1.GetLength());
// // 
// // 		fnAPP_DecCall(nEncType, TRANID_7200);
// // 		//�����ܾ�	
// // 		sprintf(m_RD.byDataField_022x, "%s", m_SRXEncBuff.byPlaintMsdata);
// // 
// // 	}
// // 	else
// // 	if(nEncType == TRAN_ENC_240_10)
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x0151[%s]" , x0151);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_SRXEncBuff.byChiperMsdata, strGetRxDataField.GetLength());
// // 
// // 		fnAPP_DecCall(nEncType, TRANID_7200);
// // 	}
// // 	else
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x022X[%s]" , x022X);
// // 		else
// // 		{
// // 			//�����ܾ�	
// // 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// // 		}
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x025X[%s]" , x025X);
// // 	else
// // 	{
// // 		//Ÿ����	
// // 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x021X[%s]" , x021X);
// // 	else
// // 	{
// // 		//������	
// // 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	//#N0181 ���� �귣�� ���� ����
// // 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7200: DataField Not Found - x018X[%s]" , x018X);
// // 	else
// // 	{
// // 		//���� ����	
// // 		sprintf(m_RD.byDataField_018x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 	
// // 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x020X[%s]" , x020X);
// // 	else
// // 	{
// // 		//�������	
// // 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x026X[%s]" , x026X);
// // 	else
// // 	{
// // 		m_pProfile->GetTransProfile();
// // 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// // 		//Host �Ϸù�ȣ	
// // 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// // 		m_pProfile->PutTransProfile();
// // 	}
// // 
// // 	
// // 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x0120[%s]" , x0120);
// // 	else
// // 	{
// // 		//������
// // 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// // 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// // 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// // 	}
// // 
// // 	//��ǥ ����Data
// // 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7227: DataField Not Found - x104X[%s]" , x0120);
// // 	else
// // 	{
// // 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// // 	}
// // 
// // 	return T_OK;	
// // }        
// int	CTranCmn::fn_FRX_TRANID_7228(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// 
// int	CTranCmn::fn_FRX_TRANID_7221(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7221: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7221: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7221: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7221: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7221: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7221: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7221: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7221: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7221: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7221: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7221: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7222(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// }        
//         
// int	CTranCmn::fn_FRX_TRANID_7231(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7222: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7231: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7231: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7231: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7231: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7231: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7231: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7231: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7231: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7231: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7231: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7232(int nTranCode, int nSec)
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7232: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7232: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7232: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7232: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7232: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7232: DataField Not Found - x021X[%s]" , x021X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7232: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7232: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7232: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7232: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	return T_OK;	
// }
// 
// int	CTranCmn::fn_FRX_TRANID_7233(int nTranCode, int nSec)		//#N0235
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 	return T_OK;
// }
//         
// 
// int	CTranCmn::fn_FRX_TRANID_7235(int nTranCode, int nSec)		//#N0235
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 	return T_OK;
// }
// 
// int	CTranCmn::fn_FRX_TRANID_7237(int nTranCode, int nSec)		//#N0235
// {
// 
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x022X[%s]" , x022X);
// 	else
// 	{
// 		//�����ܾ�	
// 		sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x025X[%s]" , x025X);
// 	else
// 	{
// 		//Ÿ����	
// 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_4325: DataField Not Found - x104X[%s]" , x0120);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 	return T_OK;
// }
// 
// 
// int	CTranCmn::fn_FRX_TRANID_7240(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7241(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// 
// int	CTranCmn::fn_FRX_TRANID_7265(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7266(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7267(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7268(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7269(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7280(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7281(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7290(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7300(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7310(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7311(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7312(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7313(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7314(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7315(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7316(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7317(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7318(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7319(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// 
// int	CTranCmn::fn_FRX_TRANID_7320(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		sprintf(m_RXLOCALTAXDATA.CustomerName, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//���ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x191X, strlen(x191X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x191X[%s]" , x191X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroNum, "%s", strGetRxDataField.GetBuffer(0));
// 		memcpy(Accept.JiroNum, &m_RXJIRODATA.JiroNum[2] , sizeof(Accept.JiroNum));  //#SJ 1115 ���ι�ȣ
// 		memcpy(Accept.JiroTranOrgCode, &m_RXJIRODATA.JiroNum[0], 2);   //#N0162-1
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x193X, strlen(x193X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x193X[%s]" , x193X);
// 	else
// 	{
// 		memset(m_RXJIRODATA.JiroPayData, 0, sizeof(m_RXJIRODATA.JiroPayData));
// 		sprintf(m_RXJIRODATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7321(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7321: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7321: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		sprintf(m_RXLOCALTAXDATA.CustomerName, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7321: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7321: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7321: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//���ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x191X, strlen(x191X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7321: DataField Not Found - x191X[%s]" , x191X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7321: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x195X, strlen(x195X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7321: DataField Not Found - x195X[%s]" , x195X);
// 	else
// 	{
// 		memset(m_RXJIRODATA.JiroPayData, 0, sizeof(m_RXJIRODATA.JiroPayData));
// 		sprintf(m_RXJIRODATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7321: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7322(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		memcpy(m_RXLOCALTAXDATA.CustomerName, strGetRxDataField.GetBuffer(0), 16 );
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325 [%s]" , strGetRxDataField.GetBuffer(0));
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325 [%s]" , strCT(m_RXLOCALTAXDATA.CustomerName, 16));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x1931, strlen(x1931), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x1931[%s]" , x1931);
// 	else
// 	{
// 		memset(m_RXLOCALTAXDATA.JiroPayData,0 , sizeof(m_RXLOCALTAXDATA.JiroPayData));
// 		sprintf(m_RXLOCALTAXDATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;
// }        
// 
// int	CTranCmn::fn_FRX_TRANID_7323(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		sprintf(m_RXJIRODATA.CustomerName, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7220: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//���ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x191X, strlen(x191X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x191X[%s]" , x191X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroNum, "%s", strGetRxDataField.GetBuffer(0));
// 		memcpy(Accept.JiroNum, &m_RXJIRODATA.JiroNum[2] , sizeof(Accept.JiroNum));  //#SJ 1115 ���ι�ȣ
// 		memcpy(Accept.JiroTranOrgCode, &m_RXJIRODATA.JiroNum[0], 2);   //#N0162-1
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x193X, strlen(x193X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x193X[%s]" , x193X);
// 	else
// 	{
// 		memset(m_RXJIRODATA.JiroPayData, 0, sizeof(m_RXJIRODATA.JiroPayData));
// 		sprintf(m_RXJIRODATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_RXJIRODATA.JiroPayData=%s", m_RXJIRODATA.JiroPayData);
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7320: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }
// 
// int	CTranCmn::fn_FRX_TRANID_7324(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7324: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7324: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		sprintf(m_RXLOCALTAXDATA.CustomerName, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7324: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7324: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7324: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//���ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x191X, strlen(x191X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7324: DataField Not Found - x191X[%s]" , x191X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7324: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x195X, strlen(x195X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7324: DataField Not Found - x195X[%s]" , x195X);
// 	else
// 	{
// 		memset(m_RXJIRODATA.JiroPayData, 0, sizeof(m_RXJIRODATA.JiroPayData));
// 		sprintf(m_RXJIRODATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7324: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXJIRODATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;
// }        
// int	CTranCmn::fn_FRX_TRANID_7325(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		memcpy(m_RXLOCALTAXDATA.CustomerName, strGetRxDataField.GetBuffer(0), 16 );
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325 [%s]" , strGetRxDataField.GetBuffer(0));
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325 [%s]" , strCT(m_RXLOCALTAXDATA.CustomerName, 16));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x1931, strlen(x1931), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x1931[%s]" , x1931);
// 	else
// 	{
// 		memset(m_RXLOCALTAXDATA.JiroPayData, 0 , sizeof(m_RXLOCALTAXDATA.JiroPayData));
// 		sprintf(m_RXLOCALTAXDATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7325: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;
// }        
// int	CTranCmn::fn_FRX_TRANID_7326(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7326: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7326: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		memcpy(m_RXLOCALTAXDATA.CustomerName, strGetRxDataField.GetBuffer(0), 16 );
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7326 [%s]" , strGetRxDataField.GetBuffer(0));
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7326 [%s]" , strCT(m_RXLOCALTAXDATA.CustomerName, 16));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7326: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7326: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7326: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7326: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7326: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x1931, strlen(x1931), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7326: DataField Not Found - x1931[%s]" , x1931);
// 	else
// 	{
// 		memset(m_RXLOCALTAXDATA.JiroPayData, 0 , sizeof(m_RXLOCALTAXDATA.JiroPayData));
// 		sprintf(m_RXLOCALTAXDATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7326: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7327(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7327: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7327: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		memcpy(m_RXLOCALTAXDATA.CustomerName, strGetRxDataField.GetBuffer(0), 16 );
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7327 [%s]" , strGetRxDataField.GetBuffer(0));
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7327 [%s]" , strCT(m_RXLOCALTAXDATA.CustomerName, 16));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7327: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7327: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7327: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7327: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7327: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x1931, strlen(x1931), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7327: DataField Not Found - x1931[%s]" , x1931);
// 	else
// 	{
// 		memset(m_RXLOCALTAXDATA.JiroPayData, 0 , sizeof(m_RXLOCALTAXDATA.JiroPayData));
// 		sprintf(m_RXLOCALTAXDATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7327: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7328(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		memcpy(m_RXLOCALTAXDATA.CustomerName, strGetRxDataField.GetBuffer(0), 16 );
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328 [%s]" , strGetRxDataField.GetBuffer(0));
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328 [%s]" , strCT(m_RXLOCALTAXDATA.CustomerName, 16));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//���ڳ��ι�ȣ
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data
// 	if(!fnAPP_RXGetFindArryDataField(x1931, strlen(x1931), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328: DataField Not Found - x1931[%s]" , x1931);
// 	else
// 	{
// 		memset(m_RXLOCALTAXDATA.JiroPayData, 0 , sizeof(m_RXLOCALTAXDATA.JiroPayData));
// 		sprintf(m_RXLOCALTAXDATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�Һΰ��� //#SJ 1106
// 	if(!fnAPP_RXGetFindArryDataField(x196X, strlen(x196X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7328: DataField Not Found - x194X[%s]" , x196X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.BillPayMonth, "%2.2s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;
// }
//  
// int	CTranCmn::fn_FRX_TRANID_7329(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7329: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
// 			EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7329: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 		memcpy(m_RXLOCALTAXDATA.CustomerName, strGetRxDataField.GetBuffer(0), 16 );
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7329 [%s]" , strGetRxDataField.GetBuffer(0));
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7329 [%s]" , strCT(m_RXLOCALTAXDATA.CustomerName, 16));
// 	}
// 
// 	//��ǥ ����Data
// 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7329: DataField Not Found - x104X[%s]" , x104X);
// 	else
// 	{
// 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7329: DataField Not Found - x023X[%s]" , x023X);
// 	else
// 	{
// 		//�ŷ��ݾ�	
// 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7329: DataField Not Found - x024X[%s]" , x024X);
// 	else
// 	{
// 		//������	
// 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7329: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	//���ڳ��ι�ȣ   
// 	if(!fnAPP_RXGetFindArryDataField(x192X, strlen(x192X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7329: DataField Not Found - x192X[%s]" , x192X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.ElecNum, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	//���γ��� Data		//#SJ 1115 ���γ��ε���Ÿ
// 	if(!fnAPP_RXGetFindArryDataField(x1931, strlen(x1931), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7329: DataField Not Found - x1931[%s]" , x1931);
// 	else
// 	{
// 		memset(m_RXLOCALTAXDATA.JiroPayData, 0 , sizeof(m_RXLOCALTAXDATA.JiroPayData));
// 		sprintf(m_RXLOCALTAXDATA.JiroPayData, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	//�̿�����
// 	if(!fnAPP_RXGetFindArryDataField(x194X, strlen(x194X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7329: DataField Not Found - x194X[%s]" , x194X);
// 	else
// 	{
// 		sprintf(m_RXLOCALTAXDATA.JiroBankCode, "%16.16s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	return T_OK;	
// }        
// //������ȸ 
// int	CTranCmn::fn_FRX_TRANID_7330(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// 	{
// 		if(!IsZero(&Accept.AccountNum, 10))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7330: DataField Not Found - x017X[%s]" , x017X);
// 	}
// 	else
// 	{
// 		//���¹�ȣ	
// //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// 		if(!IsZero(&Accept.AccountNum, 10))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 		if(!IsZero(&Accept.AccountNum, 13))
// 			EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 		else
// 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// 
// 	}
// 
// 	// �����ּ���
// 	if(!fnAPP_RXGetFindArryDataField(x018X, strlen(x018X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7330: DataField Not Found - x018X[%s]" , x018X);
// 	else
// 	{
// 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 
// 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7330: DataField Not Found - x020X[%s]" , x020X);
// 	else
// 	{
// 		//�������	
// 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// 	}
// 
// 	if(!fnAPP_RXGetFindArryDataField(x026X, strlen(x026X), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7330: DataField Not Found - x026X[%s]" , x026X);
// 	else
// 	{
// 		m_pProfile->GetTransProfile();
// 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// 		//Host �Ϸù�ȣ	
// 		sprintf(m_RD.byDataField_026x, "%12.12s", strGetRxDataField.GetBuffer(0));
// 		m_pProfile->PutTransProfile();
// 	}
// 
// 	
// 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7260: DataField Not Found - x0120[%s]" , x0120);
// 	else
// 	{
// 		//������
// 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// 		cstrTmp = fnAPP_Get_BankName(m_RD.byDataField_012x);
// 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// 	}
// 
// 
// 	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7330 Return");
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7336(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7337(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7338(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7339(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7340(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7341(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7342(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7343(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7344(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7345(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7350(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7351(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7352(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7353(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7354(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7357(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7358(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7359(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7360(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7361(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7362(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7363(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7364(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7365(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7366(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7367(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7370(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7371(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7372(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7373(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//     
// int	CTranCmn::fn_FRX_TRANID_7420(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7421(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7425(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7426(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7427(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7428(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7430(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FRX_TRANID_7431(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//        
// 
//                                     
// //��ȸ   
// int	CTranCmn::fn_FRX_TRANID_7229(int nTranCode, int nSec)
// {
// 	CString strGetRxDataField("");
// 	CString strGetRxDataField1("");
// 	CString cstrTmp("");
// 	int nEncType = 0;
// 	int nPacklen = 0;
// 
// // 	memset(m_RD.byDataField_104x, 0x20, sizeof(m_RD.byDataField_104x));
// // 	if(!fnAPP_RXGetFindArryDataField(x017X, strlen(x017X), strGetRxDataField, PACK_F))
// // 	{
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x017X[%s]" , x017X);
// // 	}
// // 	else
// // 	{
// // 		//���¹�ȣ	
// // //		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 		if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
// // 			EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 		else
// // 			EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
// // 
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x023X, strlen(x023X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x023X[%s]" , x023X);
// // 	else
// // 	{
// // 		//�ŷ��ݾ�	
// // 		sprintf(m_RD.byDataField_023x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x024X, strlen(x024X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x024X[%s]" , x024X);
// // 	else
// // 	{
// // 		//������	
// // 		sprintf(m_RD.byDataField_024x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	memset(&m_RXDECDATA, 0x00, sizeof(m_RXDECDATA));		
// // 	nEncType = fnAPP_GET_DEC_TYPE(TRANID_7220);
// // 	if(nEncType == TRAN_ENC_006_01)
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x0221, strlen(x0221), strGetRxDataField, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x022X[%s]" , x022X);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RXDECDATA.byChiperMsdata, strGetRxDataField.GetLength());
// // 
// // 
// // 		if(!fnAPP_RXGetFindArryDataField(x019X, strlen(x019X), strGetRxDataField1, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x019X[%s]" , x019X);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField1.GetBuffer(0), &m_RXDECDATA.byChiperMsdata[12], strGetRxDataField1.GetLength());
// // 
// // 		fnAPP_MAKE_DEC_CALL(nEncType, TRANID_7220);
// // 		//�����ܾ�	
// // 		sprintf(m_RD.byDataField_022x, "%s", m_RXDECDATA.byPlaintMsdata);
// // 
// // 	}
// // 	else
// // 	if(nEncType == TRAN_ENC_240_10)
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x0151, strlen(x0151), strGetRxDataField, UNPACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x0151[%s]" , x0151);
// // 		else
// // 			nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RXDECDATA.byChiperMsdata, strGetRxDataField.GetLength());
// // 
// // 		fnAPP_MAKE_DEC_CALL(nEncType, TRANID_7220);
// // 
// // 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x022X[%s]" , x022X);
// // 		else
// // 		{
// // 			//�����ܾ�	
// // 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// // 		}
// // 
// // 	}
// // 	else
// // 	{
// // 		if(!fnAPP_RXGetFindArryDataField(x022X, strlen(x022X), strGetRxDataField, PACK_F))
// // 			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x022X[%s]" , x022X);
// // 		else
// // 		{
// // 			//�����ܾ�	
// // 			sprintf(m_RD.byDataField_022x, "%s", strGetRxDataField.GetBuffer(0));
// // 		}
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x025X, strlen(x025X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x025X[%s]" , x025X);
// // 	else
// // 	{
// // 		//Ÿ����	
// // 		sprintf(m_RD.byDataField_025x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x021X, strlen(x021X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x021X[%s]" , x021X);
// // 	else
// // 	{
// // 		//������	
// // 		sprintf(m_RD.byDataField_021x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x020X, strlen(x020X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x020X[%s]" , x020X);
// // 	else
// // 	{
// // 		//�������	
// // 		sprintf(m_RD.byDataField_020x, "%s", strGetRxDataField.GetBuffer(0));
// // 	}
// // 
// // 	if(!fnAPP_RXGetFindArryDataField(x064X, strlen(x064X), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x064X[%s]" , x064X);
// // 	else
// // 	{
// // 		m_pProfile->GetTransProfile();
// // 		memcpy(m_pProfile->TRANS.HostSerialNo, strGetRxDataField.GetBuffer(0), strGetRxDataField.GetLength());
// // 		//Host �Ϸù�ȣ	
// // 		sprintf(m_RD.byDataField_064x, "%12.12s", strGetRxDataField.GetBuffer(0));
// // 		m_pProfile->PutTransProfile();
// // 	}
// // 
// // 	
// // 	if(!fnAPP_RXGetFindArryDataField(x0120, strlen(x0120), strGetRxDataField, PACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x0120[%s]" , x0120);
// // 	else
// // 	{
// // 		//������
// // 		sprintf(m_RD.byDataField_012x, "%3.3s", strGetRxDataField.GetBuffer(0));
// // 		cstrTmp = fnAPP_Get_BankName(Accept.BankNum);
// // 		sprintf(m_RD.byDataField_012x, "%s", cstrTmp.GetBuffer(0));
// // 	}
// // 
// // 	//��ǥ ����Data
// // 	if(!fnAPP_RXGetFindArryDataField(x104X, strlen(x104X), strGetRxDataField, UNPACK_F))
// // 		MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7229: DataField Not Found - x104X[%s]" , x0120);
// // 	else
// // 	{
// // 		nPacklen = MakePack(strGetRxDataField.GetBuffer(0), m_RD.byDataField_104x, strGetRxDataField.GetLength());
// // 	}
// 
// 
// 	return T_OK;	
// }


