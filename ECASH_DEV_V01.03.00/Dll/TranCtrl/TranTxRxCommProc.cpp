// NetWork.cpp: implementation of the CNetWork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\MacroDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"
#include <direct.h>
#include < nb30.h>
#include <mbstring.h>
#include "winbase.h"
#include "cpu_info.h"
#include "EzCaDes.h"
#include "SeedDWS.h"
#include "shlwapi.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CTranCmn::fnAPP_Tran_Init(int nFlag)
{

	InitializeCriticalSection(&m_csLock);
	char	szKeyValue[] = "01";
	int nTranCode = 0; 

	m_TranMode		= FALSE;

	memset(&m_STXEncBuff, 0x00, sizeof(m_STXEncBuff));		
	memset(&m_SRXEncBuff, 0x00, sizeof(m_SRXEncBuff));		

	memset(&m_TXENCKEY, 0x00, sizeof(SREncKey));		
	memset(&m_RXENCKEY, 0x00, sizeof(SREncKey));		
	
	fnAPP_Encrypt_Flag_Init();
	fnAPP_File_Init();

	
	memset(m_szTranBuffer, 0x00, sizeof(m_szTranBuffer));
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn():return");
	return T_OK;		
}


int CTranCmn::fnAPP_Tran_TxSend2( int nTranCode, int nSendSec, int nSendFlag)
{
	
	if (!m_pDevCmn->fnAPL_CheckHostLine())						// ����üũ
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ��۽� : ���]"));

		if (nTranCode == TRANID_A020 || nTranCode == TRANID_2190 || nTranCode == TRANID_0006)
			return T_OK;

		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			fnAPP_CancelProc(T_MSG, "�ŷ��� �Ұ��մϴ�", "������ �̿� �ٶ��ϴ�");
		else
			fnAPP_CancelProc(T_MSG, "Transaction is not valid");

		return T_OK;
	}

	SendLength = 0;												// �۽�Length
	memset(SendBuffer, 0, sizeof(SendBuffer));					// �۽�Buffer

	switch(nTranCode)
	{
	case TRANID_A010: //��������
		if (!AddSerialFlg)											// �Ϸù�ȣ������
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Tran_TxSend2 AddSerialNo++");
			AddSerialFlg = TRUE;									// �Ϸù�ȣ������
			m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
		}
		fn_FTX_TRANID_A010(TRANID_A010);
		break;
	case TRANID_A020: //��������
		if (!AddSerialFlg)											// �Ϸù�ȣ������
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Tran_TxSend2 AddSerialNo++");
			AddSerialFlg = TRUE;									// �Ϸù�ȣ������
			m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
		}
		fn_FTX_TRANID_A020(TRANID_A020);
		break;
	case TRANID_2190: //����Ű��û
		if (!AddSerialFlg)											// �Ϸù�ȣ������
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Tran_TxSend2 AddSerialNo++");
			AddSerialFlg = TRUE;									// �Ϸù�ȣ������
			m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
		}
		fn_FTX_TRANID_2190(TRANID_2190, nSendSec, nSendFlag);
		break;
	default:
		break;
	}

	MsgDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "fnAPP_Tran_TxSend2 Trid(%d), SendLength(%d)", nTranCode, SendLength);

	if (fnAPP_SendData(SendBuffer, SendLength, LINE_SEND_TIME) != T_OK)		
	{
		if(nSendSec == LINE_2190_TIME || nTranCode == TRANID_2190 || nTranCode == TRANID_A020 || nTranCode == TRANID_0006)
			return T_OK;

		m_pDevCmn->fnAPL_StackError("7710891", "�۽źҰ�");
		{
			if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
				fnAPP_CancelProc(T_MSG, "�ŷ��� �Ұ��մϴ�", "������ �̿� �ٶ��ϴ�");
			else
				fnAPP_CancelProc(T_MSG, "Transaction is not valid");
		}
	}	

	return T_OK;
}
int CTranCmn::fnAPP_Tran_TxSend( int nTranCode, int nSendSec)
{
	int nreturn = 0;
	int	nGetKeyF = 0;
	int nOrgBank = 100;
	int nflag = 0;
	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Tran_TxSend nTrCode[%d] nSec[%d]" , nTranCode, nSendSec);
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[����ڵ�(%03d)->O(%03d)]", CardBank, fnAPP_Get_BankOrgEncInt(CardBank, nTranCode)));
	

	switch(nTranCode)
	{
// 	case TRANID_0006: //ȸ������
// 		nreturn = fn_FTX_TRANID_0006(nTranCode, nSendSec);
// 		break;
	case TRANID_A010: //��������
		nreturn = fn_FTX_TRANID_A010(nTranCode, nSendSec);
		break;
	case TRANID_A020: //��������
		nreturn = fn_FTX_TRANID_A020(nTranCode, nSendSec);
		break;
	case TRANID_A060: //��������
		nreturn = fn_FTX_TRANID_A060(nTranCode, nSendSec);
		break;
	case TRANID_2190: //����Ű��û
		nreturn = fn_FTX_TRANID_2190(nTranCode, nSendSec);
		break;
	case TRANID_4310: //��������
		nreturn = fn_FTX_TRANID_4310(nTranCode, nSendSec);
		break;
	case TRANID_7380: //������ ��ȸ (���ݼ��� ���)  
		nreturn = fn_FTX_TRANID_7380(nTranCode, nSendSec);
		break;

	case TRANID_7220: //�ܾ���ȸ
		nreturn = fn_FTX_TRANID_7220(nTranCode, nSendSec);
		break;
	case TRANID_7210: //���������
		nreturn = fn_FTX_TRANID_7210(nTranCode, nSendSec);
		break;
	case TRANID_7530: //������ ��ȸ (���ݼ��� ���)  
		nreturn = fn_FTX_TRANID_7530(nTranCode, nSendSec);
		break;
	case TRANID_4390: //������ü
		nreturn = fn_FTX_TRANID_4390(nTranCode, nSendSec);
		break;
	case TRANID_4370: //������ü
		nreturn = fn_FTX_TRANID_4370(nTranCode, nSendSec);
		break;
	case TRANID_7260: //���������ȸ
		nreturn = fn_FTX_TRANID_7260(nTranCode, nSendSec);
 		break;

	case TRANID_4320: //�����������
	case TRANID_4220: //�����������
	case TRANID_4326: //�����������
		nreturn = fn_FTX_TRANID_4320(nTranCode, nSendSec);
		break;
	case TRANID_7200: //�Ա�
		nreturn = fn_FTX_TRANID_7200(nTranCode, nSendSec);
		break;
	case TRANID_4410: //�Ա�
		nreturn = fn_FTX_TRANID_4410(nTranCode, nSendSec);
		break;
	case TRANID_4330: //��������
		nreturn = fn_FTX_TRANID_4330(nTranCode, nSendSec);
		break;
	case TRANID_7230: //ī����ȸ
		nreturn = fn_FTX_TRANID_7230(nTranCode, nSendSec);
		break;
	case TRANID_4340: //�����ּ�
		nreturn = fn_FTX_TRANID_4340(nTranCode, nSendSec);
		break;
	case TRANID_4350: //������ü
		nreturn = fn_FTX_TRANID_4350(nTranCode, nSendSec);
		break;
	case TRANID_7250: //������ü��ȸ
		nreturn = fn_FTX_TRANID_7250(nTranCode, nSendSec);
		break;

// 	case TRANID_4116: //����ü ���	
// 		nreturn = fn_FTX_TRANID_4116(nTranCode, nSendSec);
// 		break;
// 
// 	case TRANID_7101: //�������� ������ ��ȸ 
// 		nreturn = fn_FTX_TRANID_7101(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7106: //����ü ������ ��ȸ 
// 		nreturn = fn_FTX_TRANID_7106(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7103: 
// 		nreturn = fn_FTX_TRANID_7103(nTranCode, nSendSec);
// 		break;

// 	case TRANID_4215: //���������ü
// 		nreturn = fn_FTX_TRANID_4215(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4396: //����ü������ü
// 		nreturn = fn_FTX_TRANID_4396(nTranCode, nSendSec);
// 		break;


// 	case TRANID_7261: //������������ȸ
// 		nreturn = fn_FTX_TRANID_7261(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7276: //������������ȸ
// 		nreturn = fn_FTX_TRANID_7276(nTranCode, nSendSec);
// 		break;


// 	case TRANID_7126: //�����Ա���ȸ
// 		nreturn = fn_FTX_TRANID_7126(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7236: //����ü�Ա���ȸ
// 		nreturn = fn_FTX_TRANID_7236(nTranCode, nSendSec);
// 		break;


// 	case TRANID_4720: //�����Ա�
// 		nreturn = fn_FTX_TRANID_4720(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4716: //�����Ա�
// 		nreturn = fn_FTX_TRANID_4716(nTranCode, nSendSec);
// 		break;


// 	case TRANID_4210: //��������	
// 		nreturn = fn_FTX_TRANID_4210(nTranCode, nSendSec);
// 		break;

// 	case TRANID_4719:

// 		nreturn = fn_FTX_TRANID_4719(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4730: //��������
// 		nreturn = fn_FTX_TRANID_4730(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7330: //�������� (��ȸ)
// 		nreturn = fn_FTX_TRANID_7330(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7320: //�Ϲ����� ������ ��ȸ ms
// 		nreturn = fn_FTX_TRANID_7320(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7323: //�Ϲ����� ������ ��ȸ IC
// 		nreturn = fn_FTX_TRANID_7323(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4810: //�Ϲ����� ����MS
// 		nreturn = fn_FTX_TRANID_4810(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4813: //�Ϲ����� ����IC
// 		nreturn = fn_FTX_TRANID_4813(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7321: //�Ϲ����� ���ΰ����ȸ MS
// 		nreturn = fn_FTX_TRANID_7321(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7324: //�Ϲ����� ���ΰ����ȸ IC
// 		nreturn = fn_FTX_TRANID_7324(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7322: //���漼 ������ ��ȸ MS
// 		nreturn = fn_FTX_TRANID_7322(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7325: //���漼 ������ ��ȸ IC
// 		nreturn = fn_FTX_TRANID_7325(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7328: //���漼 ������ ��ȸ �ſ�
// 		nreturn = fn_FTX_TRANID_7328(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4811: //���漼 ����MS
// 		nreturn = fn_FTX_TRANID_4811(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4814: //���漼 ����IC
// 		nreturn = fn_FTX_TRANID_4814(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4812: //���漼 ���νſ�
// 		nreturn = fn_FTX_TRANID_4812(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7326: //���漼 ���� ��� ��ȸ MS
// 		nreturn = fn_FTX_TRANID_7326(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7327: //���漼 ���� ��� ��ȸ IC
// 		nreturn = fn_FTX_TRANID_7327(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7329: //���漼 ���� ��� ��ȸ �ſ�
// 		nreturn = fn_FTX_TRANID_7329(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7104: //�ؿ�ī�� ��������ȸ	 #N0171
// 		nreturn = fn_FTX_TRANID_7104(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7105: //�ؿ�ī�� BC ��������ȸ	 #N0171
// 		nreturn = fn_FTX_TRANID_7105(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4316: //�ؿ�ī�� BC ���	 #N0171
// 		nreturn = fn_FTX_TRANID_4316(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4325: //�ؿ�ī�� BC ��� ��� #N0171
// 		nreturn = fn_FTX_TRANID_4325(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4332: //�ؿ�ī�� ���� ���	 #N0171
// 		nreturn = fn_FTX_TRANID_4332(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4342: //�ؿ�ī�� ���� ��� ���	 #N0171
// 		nreturn = fn_FTX_TRANID_4342(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7232: //�ؿ�ī�� ���� ��ȸ	 #N0171
// 		nreturn = fn_FTX_TRANID_7232(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4311: //�ؿ�ī�� SAVING		 #N0171
// 		nreturn = fn_FTX_TRANID_4311(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4312: //�ؿ�ī�� CHECKING	 #N0171
// 		nreturn = fn_FTX_TRANID_4312(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4331: //�ؿ�ī�� SCREDIT	 #N0171
// 		nreturn = fn_FTX_TRANID_4331(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4321: //�ؿ�ī�� SAVING	���	 #N0235
// 		nreturn = fn_FTX_TRANID_4311(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4322: //�ؿ�ī�� CHECKING ���	 #N0235
// 		nreturn = fn_FTX_TRANID_4312(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4341: //�ؿ�ī�� SCREDIT ���	 #N0235
// 		nreturn = fn_FTX_TRANID_4331(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7221: //�ؿ�ī�� SAVING		 #N0235
// 		nreturn = fn_FTX_TRANID_7221(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7222: //�ؿ�ī�� CHECKING	 #N0235
// 		nreturn = fn_FTX_TRANID_7222(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7231: //�ؿ�ī�� SCREDIT	 #N0235
// 		nreturn = fn_FTX_TRANID_7231(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7233: //�ؿ�ī�� SAVING ��ȸ ���	 #N0235
// 		nreturn = fn_FTX_TRANID_7233(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7235: //�ؿ�ī�� CHECKING ��ȸ ���	 #N0235
// 		nreturn = fn_FTX_TRANID_7235(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7237: //�ؿ�ī�� SCREDIT ��ȸ ���	 #N0235
// 		nreturn = fn_FTX_TRANID_7237(nTranCode, nSendSec);
// 		break;
// 	case TRANID_7229: //���� �ܾ� ��ȸ #N0239
// 		nreturn = fn_FTX_TRANID_7229(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4840: //�����н� �ſ� ���� #N0266
// 		nreturn = fn_FTX_TRANID_4840(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4850: //�����н� �ſ� ����  ��� #N0266
// 		nreturn = fn_FTX_TRANID_4850(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4670: //�����н� ���� ��û #N0266
// 		nreturn = fn_FTX_TRANID_4670(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4671: //�����н� ���� Ȯ�� #N0266
// 		nreturn = fn_FTX_TRANID_4671(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4680: //�����н� ���� Ȯ�� #N0266
// 		nreturn = fn_FTX_TRANID_4680(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4681: //�����н� ���� Ȯ�� #N0266
// 		nreturn = fn_FTX_TRANID_4681(nTranCode, nSendSec);
// 		break;
// 	case TRANID_4717: //�����н� �Ա� #N0266
// 		nreturn = fn_FTX_TRANID_4717(nTranCode, nSendSec);
// 		break;
	default:
		break;
	}

	MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Tran_TxSend: nreturn[%d]" , nreturn);
	
	return nreturn;
}

int CTranCmn::fnAPP_Tran_RxRecv(int nTranCode, int nRecvSec)
{
	int nLen = 0;
	int nFlag = 0;
	int nGetEncFlag =0;
	int AnalFlag = 0;
	int nOrgBank = 100;
	int nReturn = 0;
	char	szAtmBuff[1024];
	CString strRxGetTime("");
	CString strGetRxDataField("");
	nOrgBank = CardBank;

	memset(szAtmBuff, 0x00, sizeof(szAtmBuff));
	strRxGetTime.Format("%8.8s%6.6s", GetDate(), GetTime());
	fnAPP_ClearArrDataField(RX_RECV_F);
	memcpy(&m_RXECASH.byTLen, RecvBuffer, RecvLength);
	fnAPP_RxECash_GetDataField_RecvProc();
	fnAPP_RxECash_GetDataField_AnalProc();


	if(!fnAPP_RXGetFindArryDataField(x006X, strlen(x006X), strGetRxDataField, PACK_F))
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Tran_RxRecv: DataField Not Found - x006X[%s]" , x006X);
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_Tran_RxRecv: DataField x006X[%s]" , strGetRxDataField);
	}

	switch(nTranCode)
	{
	case TRANID_A010: //��������
		nReturn = fn_FRX_TRANID_A010(nTranCode, nRecvSec);
		break;
	case TRANID_A020: //��������
		nReturn = fn_FRX_TRANID_A020(nTranCode, nRecvSec);
		break;
	case TRANID_A060: //��������
		nReturn = fn_FRX_TRANID_A060(nTranCode, nRecvSec);
		break;
	case TRANID_2190: //����Ű��û
		nReturn = fn_FRX_TRANID_2190(nTranCode, nRecvSec);
		break;
	case TRANID_4310: //��������	
		nReturn = fn_FRX_TRANID_4310(nTranCode, nRecvSec);
		break;
	case TRANID_4330: //��������
		nReturn = fn_FRX_TRANID_4330(nTranCode, nRecvSec);
		break;
	case TRANID_7220: //�ܾ���ȸ
		nReturn = fn_FRX_TRANID_7220(nTranCode, nRecvSec);
		break;
	case TRANID_7210: //���������
		nReturn = fn_FRX_TRANID_7210(nTranCode, nRecvSec);
		break;
	case TRANID_7380: //������ ��ȸ (���ݼ��� ���)
		nReturn = fn_FRX_TRANID_7380(nTranCode, nRecvSec);
		break;
	case TRANID_7530: //������ ��ȸ (���ݼ��� ��ü)
		nReturn = fn_FRX_TRANID_7530(nTranCode, nRecvSec);
		break;
	case TRANID_4390: //������ü
		nReturn = fn_FRX_TRANID_4390(nTranCode, nRecvSec);
		break;
	case TRANID_4370: //������ü
		nReturn = fn_FRX_TRANID_4370(nTranCode, nRecvSec);
		break;
	case TRANID_7260: //���������ȸ
		nReturn = fn_FRX_TRANID_7260(nTranCode, nRecvSec);
		break;
	case TRANID_4320: //�����������
		nReturn = fn_FRX_TRANID_4320(nTranCode, nRecvSec);
		break;
	case TRANID_4410: //�Ա�
		nReturn = fn_FRX_TRANID_4410(nTranCode, nRecvSec);
		break;
	case TRANID_7200: //�Ա�
		nReturn = fn_FRX_TRANID_7200(nTranCode, nRecvSec);
		break;
	case TRANID_7230: //ī����ȸ
		nReturn = fn_FRX_TRANID_7230(nTranCode, nRecvSec);
		break;
	case TRANID_4340: //�����ּ�
		nReturn = fn_FRX_TRANID_4340(nTranCode, nRecvSec);
		break;
	case TRANID_4350: //������ü
		nReturn = fn_FRX_TRANID_4350(nTranCode, nRecvSec);
		break;
	case TRANID_7250: //����ī�� ���ݼ��� ��ü ��ȸ 
		nReturn = fn_FRX_TRANID_7250(nTranCode, nRecvSec);
		break;

// 	case TRANID_4220: //����-�����������
// 		nReturn = fn_FRX_TRANID_4220(nTranCode, nRecvSec);
// 		break;

// 	case TRANID_4210: //������������	
// 		nReturn = fn_FRX_TRANID_4210(nTranCode, nRecvSec);
// 		break;

// 	case TRANID_4719:
// 		nReturn = fn_FRX_TRANID_4719(nTranCode, nRecvSec);
// 		break;

// 	case TRANID_4396: //����ü-������ü
// 		nReturn = fn_FRX_TRANID_4396(nTranCode, nRecvSec);
// 		break;

// 	case TRANID_4720: //����-�Ա�
// 		nReturn = fn_FRX_TRANID_4720(nTranCode, nRecvSec);
// 		break;

// 
// 	case TRANID_7261: //����-���������ȸ
// 		nReturn = fn_FRX_TRANID_7261(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4215: //����-������ü
// 		nReturn = fn_FRX_TRANID_4215(nTranCode, nRecvSec);
// 		break;

// 	case TRANID_4116: //����ü ����	
// 		nReturn = fn_FRX_TRANID_4116(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4716: //����-�Ա�
// 		nReturn = fn_FRX_TRANID_4716(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4713: //�Ա�
// 		nReturn = fn_FRX_TRANID_4713(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7126: //����-�Ա�
// 		nReturn = fn_FRX_TRANID_7126(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7236: //����ü-�Ա�
// 		nReturn = fn_FRX_TRANID_7236(nTranCode, nRecvSec);
// 		break;

// 	case TRANID_4730:
// 		nReturn = fn_FRX_TRANID_4730(nTranCode, nRecvSec);
// 		break;

// 	case TRANID_7330: //�������� (��ȸ)
// 		nReturn = fn_FRX_TRANID_7330(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7320: //�Ϲ����� ������ ��ȸ ms
// 		nReturn = fn_FRX_TRANID_7320(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7323: //�Ϲ����� ������ ��ȸ IC
// 		nReturn = fn_FRX_TRANID_7323(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4810: //�Ϲ����� ����MS
// 		nReturn = fn_FRX_TRANID_4810(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4813: //�Ϲ����� ����IC
// 		nReturn = fn_FRX_TRANID_4813(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7321: //�Ϲ����� ���ΰ����ȸ MS
// 		nReturn = fn_FRX_TRANID_7321(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7324: //�Ϲ����� ���ΰ����ȸ IC
// 		nReturn = fn_FRX_TRANID_7324(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7322: //���漼 ������ ��ȸ MS
// 		nReturn = fn_FRX_TRANID_7322(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7325: //���漼 ������ ��ȸ IC
// 		nReturn = fn_FRX_TRANID_7325(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7328: //���漼 ������ ��ȸ �ſ�
// 		nReturn = fn_FRX_TRANID_7328(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4811: //���漼 ����MS
// 		nReturn = fn_FRX_TRANID_4811(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4814: //���漼 ����IC
// 		nReturn = fn_FRX_TRANID_4814(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4812: //���漼 ���νſ�
// 		nReturn = fn_FRX_TRANID_4812(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7326: //���漼 ���� ��� ��ȸ MS
// 		nReturn = fn_FRX_TRANID_7326(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7327: //���漼 ���� ��� ��ȸ IC
// 		nReturn = fn_FRX_TRANID_7327(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7329: //���漼 ���� ��� ��ȸ �ſ�
// 		nReturn = fn_FRX_TRANID_7329(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7104: //�ؿ�ī�� ��������ȸ	 #N0171
// 		nReturn = fn_FRX_TRANID_7104(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_7229: //���� �ܾ� ��ȸ #N0239
// 		nReturn = fn_FRX_TRANID_7229(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4840: //�����н� �ſ� ���� #N0266
// 		nReturn = fn_FRX_TRANID_4840(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4850: //�����н� �ſ� ����  ��� #N0266
// 		nReturn = fn_FRX_TRANID_4850(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4670: //�����н� ���� ��û #N0266
// 		nReturn = fn_FRX_TRANID_4670(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4671: //�����н� ���� Ȯ�� #N0266
// 		nReturn = fn_FRX_TRANID_4671(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4680: //�����н� ���� Ȯ�� #N0266
// 		nReturn = fn_FRX_TRANID_4680(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4681: //�����н� ���� Ȯ�� #N0266
// 		nReturn = fn_FRX_TRANID_4681(nTranCode, nRecvSec);
// 		break;
// 	case TRANID_4717: //�����н� �Ա� #N0266
// 		nReturn = fn_FRX_TRANID_4717(nTranCode, nRecvSec);
// 		break;
	default:
		break;
	}
	
	return T_OK;
}

int CTranCmn::fnAPP_Tran_RxRecv2(int nTranCode, int nRecvSec, int nRecvFlag)
{

	int		Index  = 0;
	int		nOpenFlag = FALSE;
	int		i = 0, j = 0;
	int		nRecvWaitSec = LINE_RECV_TIME;						// �ۼ��Ŵ��ð�
	int		nOrgBank = 100;
	int		nLen = 0;
	int		nFlag = 0;
	int		AnalFlag = 0;
	CString strGetRxDataField("");
	nRecvWaitSec = nRecvSec;

	if (fnAPP_RecvData(RecvBuffer, &RecvLength, nRecvWaitSec) != T_OK)		
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ŷ����� : ���]"));
		if(nRecvSec == LINE_2190_TIME || nTranCode == TRANID_2190 || nTranCode == TRANID_A020)
			return T_OK;

		switch(m_pDevCmn->fnNET_GetDeviceStatus())
		{
		case LINE_CLOSE:
			m_pDevCmn->fnAPL_StackError("7710893", "Line Close");
			break;
		default:
			m_pDevCmn->fnAPL_StackError("8860293", "���Žð��ʰ�");
			break;
		}
		fnAPP_CancelProc(T_RECVERROR);
	}

	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
		return T_OK;											// HOST OK

	fnAPP_ClearArrDataField(RX_RECV_F);
	memcpy(&m_RXECASH.byTLen, RecvBuffer, RecvLength);
	fnAPP_RxECash_GetDataField_RecvProc();
	fnAPP_RxECash_GetDataField_AnalProc();

	fnAPP_RXGetFindArryDataField(x006X, strlen(x006X), strGetRxDataField);

	switch(Asc2Int(strGetRxDataField))
	{
	case TRANID_2190: //ȸ������
		fn_FRX_TRANID_2190(nTranCode, nRecvFlag);
		break;
	case TRANID_A020: //ȸ������
	    fn_FRX_TRANID_A020(nTranCode, nRecvSec);
		break;
	default:
		break;
	}
	return T_OK;
}



int CTranCmn::fnAPP_Set_SystemTime(LPCTSTR szDate)
{
	WORD year, month, day, hour=0, minutes=0, second=0;
	SYSTEMTIME	SysDateTime;

	year = Asc2Int(&szDate[0], 4);
	month = Asc2Int(&szDate[4], 2);
	day = Asc2Int(&szDate[6], 2);
	hour = Asc2Int(&szDate[8], 2);
	minutes = Asc2Int(&szDate[10], 2);
	second = Asc2Int(&szDate[12], 2);
	GetLocalTime(&SysDateTime);			
	SysDateTime.wYear   = year;
	SysDateTime.wMonth  = month;
	SysDateTime.wDay    = day;
	SysDateTime.wHour   = hour;			
	SysDateTime.wMinute = minutes;
	SysDateTime.wSecond = second;

	if(!SetLocalTime(&SysDateTime))
	{
		MsgDump(TRACE_FREE_MODE, "log", __FILE__, __LINE__, "fnAPP_setsystemtime fail (%d)", GetLastError()); 
	}
	
	return T_OK;
}

CString CTranCmn::fnAPP_Get_BankName(int nOrgCode)
{
	char szOrgCode[20];
	CString strBuff("");

	sprintf(szOrgCode, "%03d", nOrgCode);

	for (int i = 0; i < itemof(BankCodeTbl); i++)
	{
		if (memcmp(szOrgCode, BankCodeTbl[i].BankCode, 3) == 0)
		{
			strBuff = GetString(BankCodeTbl[i].BankName, strlen(BankCodeTbl[i].BankName), strlen(BankCodeTbl[i].BankName));
			break;
		}
	}
	
	return strBuff; 

}

CString CTranCmn::fnAPP_Get_BankOrgCode(int nCardBank)
{
	char szCardBank[20];
	CString strBuff("");
	strBuff = ZERO3;

	sprintf(szCardBank, "%03d", nCardBank);

	for (int i = 0; i < itemof(BankCodeTbl); i++)
	{
		if (memcmp(szCardBank, BankCodeTbl[i].BankCode, 3) == 0)
		{
			strBuff = GetString(BankCodeTbl[i].OrgCode, strlen(BankCodeTbl[i].OrgCode), strlen(BankCodeTbl[i].OrgCode));
			break;
		}
	}
	
	return strBuff; 

}
int CTranCmn::fnAPP_Get_BankOrgCodeInt(int nCardBank)
{
	char szCardBank[20];
	CString strBuff("");
	strBuff = ZERO3;

	sprintf(szCardBank, "%03d", nCardBank);

	for (int i = 0; i < itemof(BankCodeTbl); i++)
	{
		if (memcmp(szCardBank, BankCodeTbl[i].BankCode, 3) == 0)
		{
			strBuff = GetString(BankCodeTbl[i].OrgCode, strlen(BankCodeTbl[i].OrgCode), strlen(BankCodeTbl[i].OrgCode));
			break;
		}
	}
	
	return Asc2Int(strBuff); 

}

int CTranCmn::fnAPP_Get_BankOrgEncCvtInt(int nCardBank)
{
	char szCardBank[20];
	CString strBuff("");
	strBuff = ZERO3;

	sprintf(szCardBank, "%03d", nCardBank);

	for (int i = 0; i < itemof(BankCodeTbl); i++)
	{
		if (memcmp(szCardBank, BankCodeTbl[i].OrgEnc, 3) == 0)
		{
			strBuff = GetString(BankCodeTbl[i].OrgEnc, strlen(BankCodeTbl[i].OrgEnc), strlen(BankCodeTbl[i].OrgEnc));
			break;
		}
	}
	
	return Asc2Int(strBuff); 

}


int CTranCmn::fnAPP_Get_BankEncOrgCvtInt(int nCardBank)
{
	char szCardBank[20];
	CString strBuff("");
	strBuff = "999";

	sprintf(szCardBank, "%03d", nCardBank);

	for (int i = 0; i < itemof(BankCodeTbl); i++)
	{
		if (memcmp(szCardBank, BankCodeTbl[i].OrgEnc, 3) == 0)
		{
			strBuff = GetString(BankCodeTbl[i].OrgCode, strlen(BankCodeTbl[i].OrgCode), strlen(BankCodeTbl[i].OrgCode));
			break;
		}
	}
	
	return Asc2Int(strBuff); 

}


CString CTranCmn::fnAPP_Get_BankOrgEnc(int nCardBank, int nTranCode)
{
	char szCardBank[20];
	CString strBuff("");
	strBuff = ZERO3;

	sprintf(szCardBank, "%03d", nCardBank);

	for (int i = 0; i < itemof(BankCodeTbl); i++)
	{
		if (memcmp(szCardBank, BankCodeTbl[i].BankCode, 3) == 0)
		{
			strBuff = GetString(BankCodeTbl[i].OrgEnc, strlen(BankCodeTbl[i].OrgEnc), strlen(BankCodeTbl[i].OrgEnc));
			break;
		}
	}
	
// 	if (MenuAvail & MENU_MCU3)
// 	{
// 		if (strBuff.CompareNoCase(ZERO3) == 0)
// 		{
// 			strBuff.Format("%03d", m_pProfile->TRANS.TranService);
// 		}
// 
// 	}

	return strBuff; 

}
int CTranCmn::fnAPP_Get_BankOrgEncInt(int nCardBank, int nTranCode)
{
	char szCardBank[20];
	CString strBuff("");
	strBuff = ZERO3;

	sprintf(szCardBank, "%03d", nCardBank);

	for (int i = 0; i < itemof(BankCodeTbl); i++)
	{
		if (memcmp(szCardBank, BankCodeTbl[i].BankCode, 3) == 0)
		{
			strBuff = GetString(BankCodeTbl[i].OrgEnc, strlen(BankCodeTbl[i].OrgEnc), strlen(BankCodeTbl[i].OrgEnc));
			break;
		}
	}
	
	return Asc2Int(strBuff); 
}

CString CTranCmn::fnAPP_Get_BankOrgCode(char* szCardBank)
{
	CString strBuff("");
	strBuff = ZERO3;

	for (int i = 0; i < itemof(BankCodeTbl); i++)
	{
		if (memcmp(szCardBank, BankCodeTbl[i].BankCode, 3) == 0)
		{
			strBuff = GetString(BankCodeTbl[i].OrgCode, strlen(BankCodeTbl[i].OrgCode), strlen(BankCodeTbl[i].OrgCode));
			break;
		}
	}
	
	return strBuff; 

}

CString CTranCmn::fnAPP_Get_BankName(char* szOrgCode)
{
	CString strBuff("");
	strBuff = ZERO3;
	
	for (int i = 0; i < itemof(BankCodeTbl); i++)
	{
		if (memcmp(szOrgCode, BankCodeTbl[i].BankCode, 3) == 0)
		{
			strBuff = GetString(BankCodeTbl[i].BankName, strlen(BankCodeTbl[i].BankName), strlen(BankCodeTbl[i].BankName));
			break;
		}
	}
	
	if (strBuff == ZERO3)
	{
		strBuff = "�ſ�ī��";
	}

	return strBuff; 
}

//Kim.Gi.Jin -> #0100
CString CTranCmn::fnAPP_Get_CreditName(int nOrgCode)
{
	char szOrgCode[20];
	CString strBuff("");
	strBuff = ZERO3;

	sprintf(szOrgCode, "%06d", nOrgCode);

	for (int i = 0; i < itemof(CreditBinTbl); i++)
	{
		if (memcmp(szOrgCode, CreditBinTbl[i].BinCode, 6) == 0)
		{
			strBuff = GetString(CreditBinTbl[i].BankName, strlen((char *)CreditBinTbl[i].BankName), strlen((char *)CreditBinTbl[i].BankName));
			break;
		}
	}
	
	if (strBuff == ZERO3)
	{
		strBuff = "�ſ�ī��";
	}
	
	return strBuff; 

}

CString CTranCmn::fnAPP_Get_CreditOrgCode(BYTE* byBinNo)
{
	char szOrgCode[20];
	CString strBuff("");
	strBuff = ZERO3;

	sprintf(szOrgCode, "%6.6s", byBinNo);

	for (int i = 0; i < itemof(CreditBinTbl); i++)
	{
		if (memcmp(szOrgCode, CreditBinTbl[i].BinCode, 6) == 0)
		{
			strBuff = GetString(CreditBinTbl[i].OrgCode, strlen((char *)CreditBinTbl[i].OrgCode), strlen((char *)CreditBinTbl[i].OrgCode));
			break;
		}
	}
	
	if (strBuff == ZERO3)
	{
		strBuff = "000";
	}
	
	return strBuff; 

}

//Kim.Gi.Jin
CString CTranCmn::fnAPP_Get_CreditName(BYTE* byBinNo)
{
	CString strBuff("??????01");
	
	for (int i = 0; i < itemof(CreditBinTbl); i++)
	{
		if (memcmp(byBinNo, CreditBinTbl[i].BinCode, 6) == 0)
		{
			strBuff = GetString(CreditBinTbl[i].BankName, strlen((char *)CreditBinTbl[i].BankName), strlen((char *)CreditBinTbl[i].BankName));
			break;
		}
	}
	
	return strBuff; 
}


int	CTranCmn::fnAPP_CardBank_C(char * OrgCode)
{
	char szBankNo[20];
	int  CardBank = 0;
	sprintf(szBankNo, "%3.3s", OrgCode);
	if (memcmp(szBankNo, KDBBANKID,  3) == 0)
	{
		CardBank = KDBBANK;					
	}
												// �������
	if (memcmp(szBankNo, KIUBANKID,  3) == 0)
	{
		CardBank = KIUBANK;					
	}		
												// ��ȯ���� 
	if (memcmp(szBankNo, KOEBANKID,  3) == 0)
	{
		CardBank = KOEBANK;					
	}
												// ���� 
	if (memcmp(szBankNo, SUHBANKID,  3) == 0)
	{
		CardBank = SUHBANK;					
	}
												// �������� 
	if (memcmp(szBankNo, CHOBANKID,  3) == 0)
	{
		CardBank = CHOBANK;					
	}
												// ��������
	if (memcmp(szBankNo, KOFBANKID,  3) == 0)
	{
		CardBank = KOFBANK;					
	}

												// �뱸���� 
	if (memcmp(szBankNo, DEABANKID,  3) == 0)
	{
		CardBank = DEABANK;					
	}
												// �λ����� 
	if (memcmp(szBankNo, PUSBANKID,  3) == 0)
	{
		CardBank = PUSBANK;					
	}
												// �������� 
	if (memcmp(szBankNo, KAWBANKID,  3) == 0)
	{
		CardBank = KAWBANK;					
	}
												// �������� 
	if (memcmp(szBankNo, JEJBANKID,  3) == 0)
	{
		CardBank = JEJBANK;					
	}
												// �������� 
	if (memcmp(szBankNo, JEOBANKID,  3) == 0)
	{
		CardBank = JEOBANK;					
	}
												// �泲���� 
	if (memcmp(szBankNo, KYOBANKID,  3) == 0)
	{
		CardBank = KYOBANK;					
	}
												// �������ݰ�  
	if (memcmp(szBankNo, KFCBANKID,  3) == 0)
	{
		CardBank = KFCBANK;					
	}
												// ���� 
	if (memcmp(szBankNo, SINBANKID,  3) == 0)
	{
		CardBank = SINBANK;					
	}
												// ��ȣ���� 					
	if (memcmp(szBankNo, SANBANKID,  3) == 0)
	{
		CardBank = SANBANK;					
	}
												// ��Ƽ���� 
	if (memcmp(szBankNo, CITBANKID,  3) == 0)
	{
		CardBank = CITBANK;					
	}
												// H.S.B.C 
	if(memcmp(szBankNo, HSBABNKID,  3) == 0)
	{
		CardBank = HSBABNK;					
	}

	if ((memcmp(szBankNo, SHIBANKID,  3) == 0)	||
		(memcmp(szBankNo, SHIBANKID1, 3) == 0))
	{										
		CardBank = SHIBANK;					
	}

												 // ��������
	if ((memcmp(szBankNo, KUMBANKID1, 3) == 0)	||
		(memcmp(szBankNo, KUMBANKID2, 3) == 0)	||
		(memcmp(szBankNo, KUMBANKID3, 3) == 0)	||
		(memcmp(szBankNo, KUMBANKID4, 3) == 0)	||
		(memcmp(szBankNo, KUMBANKID5, 3) == 0))
	{										
		CardBank = KUMBANK;					
	}
												 // ���� 
	if ((memcmp(szBankNo, NONBANKID1, 3) == 0)	||
		(memcmp(szBankNo, NONBANKID2, 3) == 0)	||
		(memcmp(szBankNo, NONBANKID3, 3) == 0)	||
		(memcmp(szBankNo, NONBANKID4, 3) == 0))
	{										
		CardBank = NONBANK;					
	}
												 // �츮���� 
	if ((memcmp(szBankNo, WORBANKID1, 3) == 0)	||
		(memcmp(szBankNo, WORBANKID2, 3) == 0)	||
		(memcmp(szBankNo, WORBANKID3, 3) == 0)	||
		(memcmp(szBankNo, WORBANKID4, 3) == 0))
	{										
		CardBank = WORBANK;					
	}
												 // �ϳ����� 
	if ((memcmp(szBankNo, HANBANKID1, 3) == 0)	||
		(memcmp(szBankNo, HANBANKID2, 3) == 0)	||
		(memcmp(szBankNo, HANBANKID3, 3) == 0)	||
		(memcmp(szBankNo, HANBANKID4, 3) == 0))
	{										
		CardBank = HANBANK;					
	}
												 // �ѹ����� 
	if ((memcmp(szBankNo, GOOBANKID1, 3) == 0)	||
		(memcmp(szBankNo, GOOBANKID2, 3) == 0))
	{										
		CardBank = GOOBANK;					
	}
												// ��ü��
	if ((memcmp(szBankNo, POSBANKID1, 3) == 0)	||
		(memcmp(szBankNo, POSBANKID2, 3) == 0)    ||
		(memcmp(szBankNo, POSBANKID3, 3) == 0)    ||
		(memcmp(szBankNo, POSBANKID4, 3) == 0)    ||
		(memcmp(szBankNo, POSBANKID5, 3) == 0))
	{										
		CardBank = POSBANK;					
	}


												// �������ձ�������
	if(memcmp(szBankNo, TYJSBANKID,  3) == 0)
	{
		CardBank = TYJSBANK;					
	}
												// ��������
	if(memcmp(szBankNo, HYDSBANKID,  3) == 0)
	{
		CardBank = HYDSBANK;					
	}
												// �̷���������
	if(memcmp(szBankNo, MRASBANKID,  3) == 0)
	{
		CardBank = MRASBANK;					
	}
												// �������
	if(memcmp(szBankNo, DAWSBANKID,  3) == 0)
	{
		CardBank = DAWSBANK;					
	}
												// �Ｚ����
	if(memcmp(szBankNo, SASSBANKID,  3) == 0)
	{
		CardBank = SASSBANK;					
	}
												// �츮��������
	if(memcmp(szBankNo, WRTSBANKID,  3) == 0)
	{
		CardBank = WRTSBANK;					
	}
												// �ѱ���������
	if(memcmp(szBankNo, HKTSBANKID,  3) == 0)
	{
		CardBank = HKTSBANK;					
	}
												// ��������
	if(memcmp(szBankNo, KYBSBANKID,  3) == 0)
	{
		CardBank = KYBSBANK;					
	}
												// ������������
	if(memcmp(szBankNo, HISBANKID,  3) == 0)
	{
		CardBank = HISBANK;					
	}
												// ����ġ������������
	if(memcmp(szBankNo, HMCSBANKID,  3) == 0)
	{
			CardBank = HMCSBANK;					
	}
												// Ű������
	if(memcmp(szBankNo, KIWSBANKID,  3) == 0)
	{
		CardBank = KIWSBANK;					
	}
												// ��Ʈ���̵�����
	if(memcmp(szBankNo, ETRSBANKID,  3) == 0)
	{
		CardBank = ETRSBANK;					
	}
												// ������������
	if(memcmp(szBankNo, SKSBANKID,  3) == 0)
	{
		CardBank = SKSBANK;					
	}
												// �������
	if(memcmp(szBankNo, DASSBANKID,  3) == 0)
	{
		CardBank = DASSBANK;					
	}
												// �ַθ���������
	if(memcmp(szBankNo, SOLSBANKID,  3) == 0)
	{
		CardBank = SOLSBANK;					
	}
												// ��ȭ����
	if(memcmp(szBankNo, HAHSBANKID,  3) == 0)
	{
		CardBank = HAHSBANK;					
	}
												// �ϳ���������
	if(memcmp(szBankNo, HNDSBANKID,  3) == 0)
	{
		CardBank = HNDSBANK;					
	}
												// �¸�׽�������
	if(memcmp(szBankNo, GOMSBANKID,  3) == 0)
	{
		CardBank = GOMSBANK;					
	}
												// ��������
	if(memcmp(szBankNo, DOBSBANKID,  3) == 0)
	{
		CardBank = DOBSBANK;					
	}
												// ������������
	if(memcmp(szBankNo, EUGSBANKID,  3) == 0)
	{
		CardBank = EUGSBANK;					
	}
												// �޸�Ʈ�߱�
	if(memcmp(szBankNo, MRISBANKID,  3) == 0)
	{
		CardBank = MRISBANK;					
	}
												// ������ġ��������
	if(memcmp(szBankNo, NHTSBANKID,  3) == 0)
	{
		CardBank = NHTSBANK;					
	}
												// �α�����
	if(memcmp(szBankNo, BOKSBANKID,  3) == 0)
	{
		CardBank = BOKSBANK;					
	}
												// �ſ�����
	if(memcmp(szBankNo, SIYSBANKID,  3) == 0)
	{
		CardBank = SIYSBANK;					
	}
												// ����������������
	if(memcmp(szBankNo, LIGSBANKID,  3) == 0)
	{
		CardBank = LIGSBANK;					
	}
	
	return CardBank;
}



CString	CTranCmn::strCT(void *inbuff,int slen)
{
	char strtmp[NETBUFFSIZE];
	static CString strC("");
	char *instr = (char *)inbuff;
	memset(strtmp, 0x00, sizeof(strtmp));
	strTrim(instr, strtmp, slen);
	strC.Format("%s", strtmp);
	return strC;	
}

int CTranCmn::strLTrim(char *sBuff, char *dBuff, int slen)
{
	for (int i = slen - 1; i >= 0; i--)
	{
		if (sBuff[i] != 0x20)
			break;
	}

	if (i == (slen - 1))
	{
		memcpy(dBuff, sBuff, slen);
		dBuff[slen] = 0;
		return slen;
	}

	memcpy(dBuff, sBuff, i+1);

	dBuff[i+1] = 0;

	return i+1;
}


int CTranCmn::strRTrim(char *sBuff, char *dBuff, int slen)
{
	for (int i = 0; i < slen; i++)
	{
		if (sBuff[i] != 0x20)
			break;
	}

	memcpy(dBuff, &sBuff[i], slen - i);

	dBuff[slen-i+1] = 0;


	return i+1;
}

int CTranCmn::strTrim(char *sBuff, char *dBuff, int slen)
{
	char szTmpBuff[NETBUFFSIZE];
	memset(szTmpBuff, 0x00, sizeof(szTmpBuff));
	strRTrim(sBuff, dBuff, slen);
	Strcpy(szTmpBuff, dBuff);
	strLTrim(szTmpBuff, dBuff, slen);
	return T_OK;
}



BYTE CTranCmn::fnAPP_SetBCC(BYTE* pbyBuf, DWORD dwBufSize)
{
	BYTE	*byBasc = (BYTE *)pbyBuf;
	int i = 1;

	BYTE byBCC = byBasc[0];
	while (i < (int)dwBufSize)
		byBCC ^= byBasc[i++];
	
	return (byBCC);

}


int CTranCmn::fnAPP_TranHostStatus_Confirm(int n) 
{

	int nOrgBank = 0;
	char	szSection[1024];
	char	szEntry	 [1024];
	int     nIniGetCnt = 0;
	CString strIniGetAll("0");
	CStringArray strGetAllArray; 
	CString strSDate;				// �������� St01 (12)
	CString strEDate;				// �������� Ed01 (12)
	CString strCurrDate;

	int IsFind = FALSE;

	if(m_pDevCmn->HostLocalSvrMode)
		return T_OK;


	if(!PathFileExists(_HOST_STATUS_INI))
		return T_OK;

		
	return T_OK;

	strIniGetAll = IniGetStr(_HOST_STATUS_INI, "ALL", "LIST", NULL);
	
	strGetAllArray.RemoveAll();
	SplitString(strIniGetAll, "|", strGetAllArray);

	for (int i=0; i<strGetAllArray.GetSize(); i++)
	{
		if (Asc2Int(strGetAllArray[i]) == 0)
		{
			IsFind = TRUE;
			sprintf(szSection, "%03d", 0 );
			sprintf(szEntry,   "%s"  , "CNT0");
			nIniGetCnt = IniGetInt(_HOST_STATUS_INI, szSection, szEntry, 0);

			if (!IsFind) 
			{
				IsFind = TRUE;
				break;
			}
			

			if (!nIniGetCnt) 
			{
				IsFind = TRUE;
				break;
			}



			//YYYYMMDDHHMMSS
			strCurrDate = GetDate() + GetTime();
			for (int j=0; j<nIniGetCnt; j++)
			{

				IsFind = FALSE;
				//==================================================================================
				sprintf(szEntry, "St%02d", j + 1);								// ��������
				strSDate = IniGetStr(_HOST_STATUS_INI, szSection, szEntry, "FFFFFFFFFFFF");
				//==================================================================================
				sprintf(szEntry, "Et%02d", j + 1);								// ��������
				strEDate = IniGetStr(_HOST_STATUS_INI, szSection, szEntry, "FFFFFFFFFFFF");
				//==================================================================================

				if ((strCurrDate.CompareNoCase(strSDate.GetBuffer(0)) > 0)	&&
					(strCurrDate.CompareNoCase(strEDate.GetBuffer(0)) < 0)	)
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Each Use TranNet Current[%s] -> [%s ~ %s]",strCurrDate.GetBuffer(0), strSDate.GetBuffer(0), strEDate.GetBuffer(0));

					IsFind = TRUE;
					break;
				}
			}

			if(!IsFind) continue;
			else
			if(IsFind) break;

		}


		if (Asc2Int(strGetAllArray[i]) == CardBank)
		{
			if (IsFind)
				break;

			IsFind = TRUE;
			sprintf(szSection, "%03d", CardBank );
			sprintf(szEntry,   "%s"  , "CNT0");
			nIniGetCnt = IniGetInt(_HOST_STATUS_INI, szSection, szEntry, 0);


			if (!IsFind) 
			{
				IsFind = TRUE;
				break;
			}
			

			if (!nIniGetCnt) 
			{
				IsFind = TRUE;
				break;
			}


			IsFind = FALSE;
			//YYYYMMDDHHMMSS
			strCurrDate = GetDate() + GetTime();
			for (int j=0; j<nIniGetCnt; j++)
			{

				//==================================================================================
				sprintf(szEntry, "St%02d", j + 1);								// ��������
				strSDate = IniGetStr(_HOST_STATUS_INI, szSection, szEntry, "FFFFFFFFFFFF");
				//==================================================================================
				sprintf(szEntry, "Et%02d", j + 1);								// ��������
				strEDate = IniGetStr(_HOST_STATUS_INI, szSection, szEntry, "FFFFFFFFFFFF");
				//==================================================================================

				if ((strCurrDate.CompareNoCase(strSDate.GetBuffer(0)) > 0)	&&
					(strCurrDate.CompareNoCase(strEDate.GetBuffer(0)) < 0)	)
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Each Use TranNet Current[%s] -> [%s ~ %s]",strCurrDate.GetBuffer(0), strSDate.GetBuffer(0), strEDate.GetBuffer(0));

					IsFind = TRUE;
					break;
				}
			}

			if(!IsFind) continue;
			else
			if(IsFind) break;

		}
	}

	if (!IsFind) 
		return T_OK;
	

	if (!nIniGetCnt) 
		return T_OK;


	if (!IsFind)
		return FALSE;
	else
		return TRUE;
}



int CTranCmn::fnAPP_CreditUnUsed_Check(int n) 
{
	int nRet = T_OK;
	int nOrgCredit = 0;

	
	if (!(CardTran & MENU_J2T2))
		return T_OK;

	if(TranCode == TC_SVCWITH || TranCode == TC_SVCINQ ) 
	{
		//BankCodeTbl -> TBL ���� (000) �Ұ� ó��
		nOrgCredit= fnAPP_Get_BankOrgEncInt(CardBank);
		if (nOrgCredit <= 0)
		{
			nRet =T_ERROR;
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ſ�ī��(�Ұ�) : %s(%s) ]", fnAPP_Get_CreditName(pCardData2->MembershipNo), fnAPP_Get_CreditOrgCode(pCardData2->MembershipNo)));
			fnAPP_CancelProc(T_DISMSG, TranMsgTbl[4][m_pDevCmn->fnSCR_GetCurrentLangMode()]);	
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ſ�ī��(����) : %s(%s) ]", fnAPP_Get_CreditName(pCardData2->MembershipNo), fnAPP_Get_CreditOrgCode(pCardData2->MembershipNo)));
			nRet = T_OK;
		}
	}

	return nRet;
}


int CTranCmn::fnAPP_Certificate_Confirm(int n) 
{

	int nOrgBank = 0;

	if(m_pDevCmn->HostLocalSvrMode)
		return T_OK;


	TranEncCode = fnAPP_EncType(TranCode2);
	if (TranEncCode == TRAN_ENC_000_00)      //������ �ŷ��ϰ�� Ű��ȯ x #0120
		return T_OK;


	if (TranEncCode == nEnCmd218_218_PUBLIC)
		return T_OK;

	if (TranEncCode == nEnCmd209_379_PUBLIC)
		return T_OK;


	if(TranCode == TC_SVCWITH || TranCode == TC_SVCINQ ) 
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Certificate_Confirm Abnormal-1:TranCode[%04d], TranCode2[%04d]", TranCode, TranCode2);
		return T_OK;
	}
	else
	if(TranCode == TC_DEPOSIT || TranCode == TC_WITHDRAW || TranCode == TC_INQUIRY || TranCode == TC_TRANSFER || TranCode == TC_PBOOK) 
	{
		if(CardTran & MENU_J3T3)
		{
			nOrgBank = CardBank;
		}
		else
		if(PbTran & MENU_PB)
		{
			nOrgBank = PbBank;
		}
		else
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Certificate_Confirm Abnormal:CardTran[%x], PbTran[%x]", CardTran, PbTran);
			return T_OK;
		}
		
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Certificate_Confirm (%03d)", nOrgBank);
	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_Certificate_Confirm Abnormal-2:TranCode[%04d], TranCode2[%04d]", TranCode, TranCode2);
		return T_OK;
	}
	
	int nKeyFlag = fnAPP_Bank_Key_Convert(nOrgBank);
	if(nKeyFlag == K_V_F)
	{


		fnAPD_MaterialOut(DEV_MCU | DEV_PBM);				// ��ü����	
		fnAPD_MaterialOutCheckRetract(DEV_MCU | DEV_PBM);

		m_pDevCmn->fnSCR_DisplayString(2, "����� �ٽ� �ŷ��� �ֽʽÿ�.");   //#0120 ���� �߰�
		m_pDevCmn->fnSCR_DisplayScreen(701);			
		Delay_Msg(3000);
		
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("Ű����1  : %d-����", TRANID_2190));
		fnAPP_Tran_TxSend2( TRANID_2190, LINE_OPCL_TIME);
		fnAPP_Tran_RxRecv2( TRANID_2190, LINE_OPCL_TIME);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("Ű����1  :  %d-�Ϸ�", TRANID_2190));
		
		nKeyFlag = fnAPP_Bank_Key_Convert(nOrgBank);
		if(nKeyFlag == CIPHERTEXT_TMP)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("Ű����2  :  %d-����", TRANID_2190));
			fnAPP_Tran_TxSend2( TRANID_2190, LINE_OPCL_TIME, 1);
			fnAPP_Tran_RxRecv2( TRANID_2190, LINE_OPCL_TIME);
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("Ű����2  :  %d-���ۿϷ�", TRANID_2190));
		}

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����: %s - %s]", fnAPP_Get_BankName(KBS_ORG_INT_006), (Asc2Int(m_B_E_INFO_INI.m_B_S_C[0].k_s_flag,2) == CIPHERTEXT) ? "OK" : "NG"));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����: %s - %s]", fnAPP_Get_BankName(KUB_ORG_INT_003), (Asc2Int(m_B_E_INFO_INI.m_B_S_C[1].k_s_flag,2) == CIPHERTEXT) ? "OK" : "NG"));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����: %s - %s]", fnAPP_Get_BankName(NON_ORG_INT_011), (Asc2Int(m_B_E_INFO_INI.m_B_S_C[2].k_s_flag,2) == CIPHERTEXT) ? "OK" : "NG"));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����: %s - %s]", fnAPP_Get_BankName(SCO_ORG_INT_023), (Asc2Int(m_B_E_INFO_INI.m_B_S_C[3].k_s_flag,2) == CIPHERTEXT) ? "OK" : "NG"));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����: %s - %s]", fnAPP_Get_BankName(WRT_ORG_INT_247), (Asc2Int(m_B_E_INFO_INI.m_B_S_C[4].k_s_flag,2) == CIPHERTEXT) ? "OK" : "NG"));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����: %s - %s]", fnAPP_Get_BankName(SST_ORG_INT_240), (Asc2Int(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,2) == CIPHERTEXT) ? "OK" : "NG"));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����: %s - %s]", fnAPP_Get_BankName(HWT_ORG_INT_269), (Asc2Int(m_B_E_INFO_INI.m_B_S_C[6].k_s_flag,2) == CIPHERTEXT) ? "OK" : "NG"));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����: %s - %s]", fnAPP_Get_BankName(KJB_ORG_INT_034), (Asc2Int(m_B_E_INFO_INI.m_B_S_C[7].k_s_flag,2) == CIPHERTEXT) ? "OK" : "NG"));  
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����: %s - %s]", fnAPP_Get_BankName(KEB_ORG_INT_005), (Asc2Int(m_B_E_INFO_INI.m_B_S_C[8].k_s_flag,2) == CIPHERTEXT) ? "OK" : "NG"));  
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[Ű����: %s - %s]", fnAPP_Get_BankName(WRB_ORG_INT_020), (Asc2Int(m_B_E_INFO_INI.m_B_S_C[9].k_s_flag,2) == CIPHERTEXT) ? "OK" : "NG"));  

		fnAPP_CancelProc(T_MSG, "�ش� ������ �ŷ��� �Ϸ� �Ǿ����ϴ�", "�ٽ� �ŷ��Ͽ� �ֽñ� �ٶ��ϴ�");
	}
	
	fnAPP_File_Init();
	return T_OK;
}


int CTranCmn::Om_GetCurrentActionRemain(int nFlag)
{
	BOOL	bRet = TRUE;

	//�ܾ� ���
	int nCurRB2Cnt = 0;
	int nCurRB1Cnt = 0;

	m_pDevCmn->SCSTCntF = 0;
	nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����
	nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
	nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
	nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

	if (nFlag)
	{
		IniSetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_T0GAP",   "FFFF");

		IniSetInt(_SP_BRM_CSTINFO_INI, "CURR_R_A", "50000",   0);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ܾ�: 5���� (%05d)]", 0));
		IniSetInt(_SP_BRM_CSTINFO_INI, "CURR_R_B", "10000",   0);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ܾ�: 1���� (%05d)]", 0));
	}
	else
	{
		IniSetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_A0GAP",   ZERO10);
		IniSetInt(_SP_BRM_CSTINFO_INI, "CURR_R_A", "50000",   nCurRB1Cnt);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ܾ�: 5���� (%05d)]", nCurRB1Cnt));
		IniSetInt(_SP_BRM_CSTINFO_INI, "CURR_R_B", "10000",   nCurRB2Cnt);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�����ܾ�: 1���� (%05d)]", nCurRB2Cnt));
	}

	
	return bRet;
}


int CTranCmn::Om_SetActionGapRemainAsycSend(int nFlag)
{
	BOOL	bRet = TRUE;
	CString strTemp("");
	CString IniGetAmt("");
	CString strTemp1("");

	int nCurRB2Cnt = 0;
	int nCurRB1Cnt = 0;
	int nSaveCurRB2Cnt = 0;
	int nSaveCurRB1Cnt = 0;
	int nSetCurRB2Cnt = 0;
	int nSetCurRB1Cnt = 0;
	int nSetCurTotCnt = 0;
	int nGetTotCnt = 0;

	nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����
	nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
	nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
	nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);


	if(m_pDevCmn->SCSTCntF == 2)
	{

		IniGetAmt      = IniGetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_T0GAP",   ZERO10);
		nSaveCurRB1Cnt = IniGetInt(_SP_BRM_CSTINFO_INI, "CURR_R_A", "50000",  0);
		nSaveCurRB2Cnt = IniGetInt(_SP_BRM_CSTINFO_INI, "CURR_R_B", "10000",  0);

		nGetTotCnt     = Asc2Int(IniGetAmt);
		nSetCurRB1Cnt  = (nCurRB1Cnt - nSaveCurRB1Cnt) * CASH_KOREA_50000;
		nSetCurRB2Cnt  = (nCurRB2Cnt - nSaveCurRB2Cnt) * CASH_KOREA_10000;
	
		nSetCurTotCnt  = (nSetCurRB1Cnt + nSetCurRB2Cnt);
		nGetTotCnt     = (nGetTotCnt + nSetCurTotCnt);

		strTemp.Empty();
		if (nSetCurTotCnt < 0)
		{
			strTemp.Format("-%09d", abs(nSetCurTotCnt));
		}
		else
		{
			strTemp.Format("0%09d", abs(nSetCurTotCnt));
		}

		//Cst Remain Money
		IniSetInt(_SP_BRM_CSTINFO_INI, "CURR_R_A", "50000",   nCurRB1Cnt);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ܾ�: 5���� (%05d)]", nCurRB1Cnt));	
		IniSetInt(_SP_BRM_CSTINFO_INI, "CURR_R_B", "10000",   nCurRB2Cnt);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[��ܾ�: 1���� (%05d)]", nCurRB2Cnt));

		if (nFlag)
		{
			IniSetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_T0GAP",   strTemp);
			IniSetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_A0GAP",   strTemp);
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���Աݾ�: ���� (%s)]",strTemp));
		}
		else
		{
			
			if (nGetTotCnt < 0)
			{
				strTemp1.Format("-%09d", abs(nGetTotCnt));
			}
			else
			{
				strTemp1.Format("0%09d", abs(nGetTotCnt));
			}

			IniSetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_T0GAP",   strTemp1);
			IniSetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_A0GAP",   strTemp);
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���Աݾ�: ���� (%s)]",strTemp1));
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[���Աݾ�: �߰� (%s)]",strTemp));
		}


		if (!m_pDevCmn->fnAPL_CheckHostOpen())						
			return T_OK;

		try 
		{
			TranCode2 = TRANID_A020;
			TranCode = TC_STATUS;
			m_pDevCmn->TranResult = FALSE;						
			m_pDevCmn->TranStatus = TRAN_STATUS;				
			m_pDevCmn->TranStatusSave = m_pDevCmn->TranStatus;
			AddSerialFlg = FALSE;
			fnAPP_Tran_TxSend2(TRANID_A020, LINE_SEND_5_TIME);
		}
		catch (CTranCmnExit& e)
		{	
			e.TranExitResult = e.TranExitResult;
		}

		m_pDevCmn->SCSTCntF = 0;

	}
	
	return bRet;
}





