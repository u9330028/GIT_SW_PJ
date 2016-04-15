#include "stdafx.h"
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
/* Function Name : Data Format Make (send) gijin
/* 
/* Date			 : 2012.01.30
/* 
/* Input	     : TRCODE (��������)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/  
int	CTranCmn::fn_FTX_TRANID_0006(int nTranCode, int nSec)             
{
	int     nSendLength =0;
	int		nSlen = 0;
	int		nBcc = 0;
	BYTE	strDummy[1024];
	char	szTmp[1024];
	BYTE	byTmpBuff[1024];
	BYTE	bySendBuff[1024];

	BYTE	byTLen[4];
	BYTE	byTData[9216];
	BYTE	byBcc[2];

	memset(strDummy,   0x20, sizeof(strDummy));
	memset(&byTLen,   0x20, sizeof(byTLen));
	memset(&byTData,   0x20, sizeof(byTData));
	memset(&byBcc,   0x20, sizeof(byBcc));
	memset(szTmp,      0x00, sizeof(szTmp));
	memset(bySendBuff,      0x00, sizeof(bySendBuff));

	nSlen = 0;
	memcpy(&strDummy[nSlen], &m_pProfile->NETWORK.BranchNum, 6);
	nSlen = 6;
	memcpy(&strDummy[nSlen], &m_pProfile->NETWORK.AtmSerialNum, 8);
	nSlen += 8;
	memcpy(&strDummy[nSlen], m_pProfile->NETWORK.AtmIPAddress, 15);
	nSlen += 15;


	Int2Asc(nSlen+sizeof(byBcc), byTLen, sizeof(byTLen));
	memcpy(byTData, strDummy, nSlen);
	byTmpBuff[0] = MakeBcc(byTData, nSlen);
	MakeUnPack(byTmpBuff, byBcc, 1);

	memcpy(szTmp,                       byTLen, sizeof(byTLen));
	memcpy(&szTmp[sizeof(byTLen)],      byTData, nSlen);
	memcpy(&szTmp[sizeof(byTLen)+nSlen],byBcc, sizeof(byBcc));

	nSendLength = sizeof(byTLen) + nSlen + sizeof(byBcc);

	memcpy(&bySendBuff[0], szTmp, nSendLength);

	MsgDump(TRACE_CODE_MODE, "POLL", __FILE__, __LINE__, "fn_FTX_TRANID_0006 [%s] - [%d]", bySendBuff, nSendLength);
	int nRet = fnAPP_SendData(bySendBuff, nSendLength, LINE_SEND_TIME);

   return T_OK;
}
/**********************************************************************************************************/
/* Function Name : Data Format Make (send) gijin
/* 
/* Date			 : 2012.01.30
/* 
/* Input	     : TRCODE (��������)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/  
int	CTranCmn::fn_FTX_TRANID_A010(int nTranCode, int nSec)
{
	
	fnAPP_ClearArrDataField(TX_SEND_F);
	fnAPP_Init_031X();

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->


// 
// 	fnAPP_SetDataField_Item_044X(nTranCode, TX_SEND_F); //���ʾ�ȣȭKEY
// 	
// 
// 	//�����
// 	fnAPP_SetDataField_Item_034X(nTranCode, TX_SEND_F); //��ֱ���(����,���)
// 	fnAPP_SetDataField_Item_035X(nTranCode, TX_SEND_F); //������ �� ���� ��������
// 	fnAPP_SetDataField_Item_036X(nTranCode, TX_SEND_F); //�����Ժ� ���� ��������
 	fnAPP_SetDataField_Item_047X(nTranCode, TX_SEND_F); //SST Serial Number(SST-ID)
 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
 	fnAPP_SetDataField_Item_065X(nTranCode, TX_SEND_F); //����̽��� �� ����
// 	fnAPP_SetDataField_Item_083X(nTranCode, TX_SEND_F); //��Ÿ���������
// 	fnAPP_SetDataField_Item_084X(nTranCode, TX_SEND_F); //Shutdown�ð�
 	fnAPP_SetDataField_Item_088X(nTranCode, TX_SEND_F); //�ܸ����� IP Address(xxx.xxx.xxx.xxx)
 	fnAPP_SetDataField_Item_089X(nTranCode, TX_SEND_F); //�ܸ����� LanCard�� Physical Address('-'���� Data��)
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)
 	fnAPP_SetDataField_Item_094X(nTranCode, TX_SEND_F); //��ǥ ���� ��� ����


 	fnAPP_SetDataField_Item_171X(nTranCode, TX_SEND_F); //��ġ����� ������ü �ڵ� ����	
	fnAPP_SetDataField_Item_172X(nTranCode, TX_SEND_F); //��ġ����� �귣�屸�� �ڵ� ����
 	fnAPP_SetDataField_Item_173X(nTranCode, TX_SEND_F); //�ܸ����� ī��Ʈ�� ������ ��������


	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}        
int	CTranCmn::fn_FTX_TRANID_A020(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);
	
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_006X (%d)", nTranCode);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->


	//�����
	fnAPP_SetDataField_Item_034X(nTranCode, TX_SEND_F); //��ֱ���(����,���)
	fnAPP_SetDataField_Item_035X(nTranCode, TX_SEND_F); //������ �� ���� ��������
//	fnAPP_SetDataField_Item_036X(nTranCode, TX_SEND_F); //�����Ժ� ���� ��������
	fnAPP_SetDataField_Item_040X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������

//	fnAPP_SetDataField_Item_047X(nTranCode, TX_SEND_F); //SST Serial Number(SST-ID)

	if (m_pDevCmn->SCSTCntF == 2)
	{
 		fnAPP_SetDataField_Item_055X(nTranCode, TX_SEND_F); //�������Աݾ�
 		fnAPP_SetDataField_Item_056X(nTranCode, TX_SEND_F); //�������Խð�
	}

//  	fnAPP_SetDataField_Item_055X(nTranCode, TX_SEND_F); //�������Աݾ�
//  	fnAPP_SetDataField_Item_056X(nTranCode, TX_SEND_F); //�������Խð�

 	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //�Ϲ�����(��ݰ��� ��ǥ��), ��������(��⺸�� ��ǥ��)
 	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�

 	fnAPP_SetDataField_Item_087X(nTranCode, TX_SEND_F); //��ּ����ڵ�
	fnAPP_SetDataField_Item_088X(nTranCode, TX_SEND_F); //�ܸ����� IP Address(xxx.xxx.xxx.xxx)
	fnAPP_SetDataField_Item_100X(nTranCode, TX_SEND_F); //����OFF�Ⱓ
	fnAPP_SetDataField_Item_102X(nTranCode, TX_SEND_F); //H/W Module ��������
	fnAPP_SetDataField_Item_103X(nTranCode, TX_SEND_F); //�ڿ� ���� ��������

 	fnAPP_SetDataField_Item_173X(nTranCode, TX_SEND_F); //�ܸ����� ī��Ʈ�� ������ ��������
 	fnAPP_SetDataField_Item_464X(nTranCode, TX_SEND_F); //���ݺ���üũ

	fnAPP_TxECash_SetDataField_SendProc();	

	m_pTranCmn->ForceSendStatus         = STATUS_OFF_TIME;
	return T_OK;	
}   

/**********************************************************************************************************/
/* Function Name : Data Format Make (send) gijin
/* 
/* Date			 : 2012.01.30
/* 
/* Input	     : TRCODE (��������)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/  
int	CTranCmn::fn_FTX_TRANID_A040(int nTranCode, int nSec)
{
	
	fnAPP_ClearArrDataField(TX_SEND_F);

 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}

int	CTranCmn::fn_FTX_TRANID_2190(int nTranCode, int nSec, int nEncFlag)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	//��ȣȭ
	if(Asc2Int(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,2) == CIPHERTEXT_TMP)
		fnAPP_KeyInitStart(CardBank, 1);
	else
		fnAPP_KeyInitStart(CardBank, 0);



 	fnAPP_SetDataField_Item_043X(nTranCode, TX_SEND_F); //�־�ȣȭKey
	fnAPP_SetDataField_Item_044X(nTranCode, TX_SEND_F); //���ʾ�ȣȭKEY 

	fnAPP_SetDataField_Item_047X(nTranCode, TX_SEND_F); //SST Serial Number(SST-ID)
 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������

	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //������ ��û ������
	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)


	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}        

int	CTranCmn::fn_FTX_TRANID_A060(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_047X(nTranCode, TX_SEND_F); //SST Serial Number(SST-ID)
 	fnAPP_SetDataField_Item_055X(nTranCode, TX_SEND_F); //�������Աݾ�
 	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //�Ϲ�����(��ݰ��� ��ǥ��), ��������(��⺸�� ��ǥ��)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)


	//�����
	fnAPP_SetDataField_Item_105X(nTranCode, TX_SEND_F); //�ѿ�����ݰǼ�
	fnAPP_SetDataField_Item_106X(nTranCode, TX_SEND_F); //�ѿ�����ݱݾ�
	fnAPP_SetDataField_Item_107X(nTranCode, TX_SEND_F); //�ѿ������ �ѰǼ�
	fnAPP_SetDataField_Item_108X(nTranCode, TX_SEND_F); //�ѿ������ �ѱݾ�
	fnAPP_SetDataField_Item_109X(nTranCode, TX_SEND_F); //�Ѽ�����ݰǼ�
	fnAPP_SetDataField_Item_110X(nTranCode, TX_SEND_F); //�Ѽ�����ݱݾ�
	fnAPP_SetDataField_Item_111X(nTranCode, TX_SEND_F); //�Ѽ��������ҰǼ�
	fnAPP_SetDataField_Item_112X(nTranCode, TX_SEND_F); //�Ѽ��������ұݾ�
	fnAPP_SetDataField_Item_113X(nTranCode, TX_SEND_F); //����ü�Ǽ�
	fnAPP_SetDataField_Item_114X(nTranCode, TX_SEND_F); //����ü�ݾ�
	fnAPP_SetDataField_Item_117X(nTranCode, TX_SEND_F); //����ü�Ǽ�
	fnAPP_SetDataField_Item_118X(nTranCode, TX_SEND_F); //����ü�ݾ�

	fnAPP_SetDataField_Item_122X(nTranCode, TX_SEND_F); //�Ѽ�����ü�Ǽ�
	fnAPP_SetDataField_Item_123X(nTranCode, TX_SEND_F); //�Ѽ�����ü�ݾ�

	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //���ԱݰǼ�
	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //���Աݱݾ�

	fnAPP_SetDataField_Item_132X(nTranCode, TX_SEND_F); //���������հ�����
	fnAPP_SetDataField_Item_133X(nTranCode, TX_SEND_F); //���������հ�ð�
	fnAPP_SetDataField_Item_134X(nTranCode, TX_SEND_F); //��������հ�����
	fnAPP_SetDataField_Item_135X(nTranCode, TX_SEND_F); //��������հ�ð�


	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}        
  
int	CTranCmn::fn_FTX_TRANID_4310(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
//	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����
	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //��ȣȭ �������� 
	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 
	
	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)

	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //������, ����� ���

//	setData036x(); // �鼼�� ������ (�������� ������ݽ� ���)	-> �����												


	fnAPP_TxECash_SetDataField_SendProc();
	
	return T_OK;	
}    



int	CTranCmn::fn_FTX_TRANID_4370(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);
	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_004X(nTranCode, TX_SEND_F); //���޻� �Ϸù�ȣ 
	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
	
	if (TranEncCode == nEnCmd243_243)
		fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)

	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����

	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���

 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
    fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������

 	fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //����� 
	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //����� 
 	fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //�Ա���

//  fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //��ȣȭ �������� 

 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
//	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
	fnAPP_SetDataField_Item_062X(nTranCode, TX_SEND_F); //�������α����ڵ� 	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)

// 	fn_FTX_EMVSendMake_Lib01(nTranCode);
//  fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //�ſ�ī��IC��������
//  fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	
	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //�Ｚ����, ����� ���

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}        

int	CTranCmn::fn_FTX_TRANID_4390(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);
	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_004X(nTranCode, TX_SEND_F); //���޻� �Ϸù�ȣ 
	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ

	if (TranEncCode == nEnCmd243_243)
		fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)

	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����

	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���

 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
    fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������

 	fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //����� 
	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //����� 
 	fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //�Ա���

//  fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //��ȣȭ �������� 

 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
//	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
	fnAPP_SetDataField_Item_062X(nTranCode, TX_SEND_F); //�������α����ڵ� 	

 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)

// 	fn_FTX_EMVSendMake_Lib01(nTranCode);
//  fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //�ſ�ī��IC��������
//  fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	
	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //�Ｚ����, ����� ���

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}        

int	CTranCmn::fn_FTX_TRANID_4410(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);


	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
//	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����
	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������

	//����Ÿ���Ǹ� ���
	if (TranEncCode == nEnCmd209_379_PUBLIC)
	{
		fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //����� 
		fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //����� 
		fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //�Ա���
	}
	
	
//	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
//	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //��ȣȭ �������� 

	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)
 	fnAPP_SetDataField_Item_379X(nTranCode, TX_SEND_F); //�޴�����ȣ


	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}     

int	CTranCmn::fn_FTX_TRANID_7230(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //��ȣȭ �������� 

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 
	
	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)

 	fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //�ſ�ī��IC��������
 //	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	

	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //�ؿܽſ�
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //�ؿܽſ�
		}
	}

	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
} 

int	CTranCmn::fn_FTX_TRANID_7250(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);


	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����

	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ

 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������

 	fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //����� 
	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //����� 
 	fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //�Ա���

//  fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //��ȣȭ �������� 

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 

//	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
//	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
	fnAPP_SetDataField_Item_062X(nTranCode, TX_SEND_F); //�������α����ڵ� 	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)

	fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //�ſ�ī��IC��������
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	

	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //�ؿܽſ�
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //�ؿܽſ�
		}
	}

	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}        

int	CTranCmn::fn_FTX_TRANID_4350(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����

	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ

 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������

 	fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //����� 
	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //����� 
 	fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //�Ա���

//  fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //��ȣȭ �������� 

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 

//	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
//	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
	fnAPP_SetDataField_Item_062X(nTranCode, TX_SEND_F); //�������α����ڵ� 	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)


 	fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //�ſ�ī��IC��������
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	

	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //�ؿܽſ�
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //�ؿܽſ�
		}
	}

	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}   

int	CTranCmn::fn_FTX_TRANID_7200(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����
	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
//	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
//	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //��ȣȭ �������� 

	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)
 	fnAPP_SetDataField_Item_379X(nTranCode, TX_SEND_F); //�޴�����ȣ


	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}    

int	CTranCmn::fn_FTX_TRANID_7260(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����

	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ

 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������

//  fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //����� 
// 	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //����� 
//  fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //�Ա���

//  fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //��ȣȭ �������� 

 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
//	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
	fnAPP_SetDataField_Item_062X(nTranCode, TX_SEND_F); //�������α����ڵ� 	

 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)

	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //������, ����� ���


	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}     


//���
int	CTranCmn::fn_FTX_TRANID_4320(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����
	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
//	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_050X(nTranCode, TX_SEND_F); //���ŷ� �ܸ��� �Ϸù�ȣ (050X)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	
	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)

 	fnAPP_SetDataField_Item_445X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������

	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //�Ｚ����, ����� ���

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}  


int	CTranCmn::fn_FTX_TRANID_4340(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)

	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����
 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_050X(nTranCode, TX_SEND_F);
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 
	
	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)

// 	fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //�ſ�ī��IC��������
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	


	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //�ؿܽſ�
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //�ؿܽſ�
		}
	}


	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}  


//////////////////////////////////////////////////////////////////////////
// ���ݼ��� ��� ������ ��ȸ 
int	CTranCmn::fn_FTX_TRANID_7380(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);


	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����
 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������

	fnAPP_SetDataField_Item_459X(nTranCode, TX_SEND_F); //�ſ�ī��IC��������

    fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //�ſ�ī��IC��������
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC


	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //�ؿܽſ�
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //�ؿܽſ�
		}
	}

	fnAPP_TxECash_SetDataField_SendProc();	
		

	return T_OK;	
}                                    

int	CTranCmn::fn_FTX_TRANID_7530(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);


	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����
 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������

	fnAPP_SetDataField_Item_459X(nTranCode, TX_SEND_F); //�ſ�ī��IC��������

    fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //�ſ�ī��IC��������
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC



	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //�ؿܽſ�
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //�ؿܽſ�
		}
	}
	
	fnAPP_TxECash_SetDataField_SendProc();	
		

	return T_OK;	
}   

int	CTranCmn::fn_FTX_TRANID_7220(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //�߱ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1


// 	fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //����� 
// 	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //����� 
//	fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //�Ա���
	
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); ////��ȣȭ ��������



	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 
	
	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)


	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data

	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //������, ����� ���

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}        


int	CTranCmn::fn_FTX_TRANID_7210(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����
	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); ////��ȣȭ ��������

	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 
	
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)


	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ

	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //������, ����� ���

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
} 
       
int	CTranCmn::fn_FTX_TRANID_4330(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//�����
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //��ȣȭ����
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //��ȣȭ���� (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //�ŷ����ɱ�� ������ ������
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //��� MS Track ��ȣ
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //�������� (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ް��ڵ�
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //��ݸ�ü����
 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //���ʽ� ��ü ��ݰ��� ���� 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(�Ѿ�) -> �Ϲ�����(��ݰ����Ѿ�), ��������(��⺸�� ���ݾ�)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //�ŷ����¹�ȣ(�����)

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //ICī�� �Ϸù�ȣ
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //ICī�� ��ȣȭ ��й�ȣ 
	
	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //��ݰ��� �ݾ�(��ǥ)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //������ �ż�����	
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //�ܸ��� ���α׷��� ����(6�ڸ�����)


 	fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //�ſ�ī��IC��������
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC


	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //�ؿܽſ�
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //�ؿܽſ�
		}
	}


	fnAPP_TxECash_SetDataField_SendProc();	
		

	return T_OK;	
}


/**************************************************************************************************************************************************************/
//���� ���� �ϼ���. (2016.03.08) - �������� Kim.Gi.Jin [�����ʿ�ݵ���]
/**************************************************************************************************************************************************************/
// 
// int	CTranCmn::fn_FTX_TRANID_4210(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 
// 
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //��ȣȭ �������� 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_345X(nTranCode, TX_SEND_F); //������ ����Ʈ ���� ���� (��ȯ)  #N0174
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //�������������ȣ(ȸ��)
// 	}
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 
// 	return T_OK;	
// }   
// 
// //����ü ���
// int	CTranCmn::fn_FTX_TRANID_4116(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ֹι�ȣ
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_345X(nTranCode, TX_SEND_F); //������ ����Ʈ ���� ���� (��ȯ)  #N0174
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 
// 	return T_OK;	
// }   
//  
// 
// 
// int	CTranCmn::fn_FTX_TRANID_4215(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
// 	
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_345X(nTranCode, TX_SEND_F); //������ ����Ʈ ���� ���� (��ȯ)  #N0174
// 	
// 	if(m_pProfile->DEVICE.ETCDevice20 == HANA)
// 		fnAPP_SetDataField_Item_1721(nTranCode, TX_SEND_F); //�ϳ����� ��� ������ #N0214
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //�������������ȣ(ȸ��)
// 	}
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }
// 
// int	CTranCmn::fn_FTX_TRANID_4396(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ֹι�ȣ
// 	
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_345X(nTranCode, TX_SEND_F); //������ ����Ʈ ���� ���� (��ȯ)  #N0174
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == HANA)
// 		fnAPP_SetDataField_Item_1721(nTranCode, TX_SEND_F); //�ϳ����� ��� ������ #N0214
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }              
// int	CTranCmn::fn_FTX_TRANID_4393(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_345X(nTranCode, TX_SEND_F); //������ ����Ʈ ���� ���� (��ȯ)  #N0174
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == HANA)
// 		fnAPP_SetDataField_Item_1721(nTranCode, TX_SEND_F); //�ϳ����� ��� ������ #N0214
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// 
//    
// int	CTranCmn::fn_FTX_TRANID_4720(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�X
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0226
// 		fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 	
// 	//#0045
// 	fnAPP_SetDataField_Item_222X(nTranCode, TX_SEND_F); //���ڰŷ�������ȣ
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //�������������ȣ(ȸ��)
// 	}
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// } 
//    
// int	CTranCmn::fn_FTX_TRANID_4716(int nTranCode, int nSec) //-> ����ü �Ա�
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //����ü �Ա��ڸ� -> //#0101 --> #0107 -> #0111 ** (��������) 
// 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //�Ա��ڿ���ó -> //#0107 -> #0111 ** (��������)
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ֹι�ȣ
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// } 
// 
// int	CTranCmn::fn_FTX_TRANID_4713(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1 
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0226
// 		fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	//#0045
// 	fnAPP_SetDataField_Item_222X(nTranCode, TX_SEND_F); //���ڰŷ�������ȣ
// 	
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }        
// 

// int	CTranCmn::fn_FTX_TRANID_7101(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// //	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// //	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// //	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// //	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //�������������ȣ(ȸ��)
// 	}
// 
// //	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// //	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();
// 
// 	return T_OK;	
// } 
// 
// //����ü ��������ȸ
// int	CTranCmn::fn_FTX_TRANID_7106(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// //	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// //	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// //	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// //	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ֹι�ȣ
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// //	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// //	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }        
//        
// int	CTranCmn::fn_FTX_TRANID_7103(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ 
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// //	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// //	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// 
// 
//         
// 
// 
// 
// int	CTranCmn::fn_FTX_TRANID_7126(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
// 	
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //�������������ȣ(ȸ��)
// 	}
// 	
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }  
//   
// int	CTranCmn::fn_FTX_TRANID_7236(int nTranCode, int nSec) //����ü ���� ��ȸ 
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == HANA)	
// 		fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //����ü �Ա��ڸ� -> //#0101 -> ���ʿ�  = #N0214 �ϳ� �귣��
// 	
// 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //�Ա��ڿ���ó -> //#0107 -> #0111 ** (��������)
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ֹι�ȣ
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }
//   
// 
// 
// 
// 
// int	CTranCmn::fn_FTX_TRANID_7261(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// //	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// //	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0239
// 	{
// 		fnAPP_SetDataField_Item_087X(nTranCode, TX_SEND_F); //���� �ŷ� ����
// 	}
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //�������������ȣ(ȸ��)
// 	}
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }
// 
// int	CTranCmn::fn_FTX_TRANID_7276(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// //	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// //	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ֹι�ȣ
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }
//    
// 
// int	CTranCmn::fn_FTX_TRANID_4220(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// //	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// //	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// //	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// //	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// //	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //�������������ȣ(ȸ��)
// 	}
// 
// //	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// //	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// //	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// //	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// //	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 
// 	return T_OK;	
// }  
// 
// 

// int	CTranCmn::fn_FTX_TRANID_7265(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7266(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7267(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7268(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7269(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7280(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7281(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7290(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7300(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7310(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7311(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7312(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7313(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7314(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7315(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7316(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7317(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7318(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7319(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// 
// 
// //#GIRO �Ϲ����� ��������ȸ MS
// int	CTranCmn::fn_FTX_TRANID_7320(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7321(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;
// }        
// int	CTranCmn::fn_FTX_TRANID_7322(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 	
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// 	
// }   
// 
// //#GIRO �Ϲ����� ������ ��ȸ IC     
// int	CTranCmn::fn_FTX_TRANID_7323(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7324(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;
// }        
// int	CTranCmn::fn_FTX_TRANID_7325(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 	
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7326(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 	
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7327(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 	
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// 
// int	CTranCmn::fn_FTX_TRANID_7328(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// 
// int	CTranCmn::fn_FTX_TRANID_7329(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	//#SJ 1106
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	//fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	//fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	//fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	//#SJ 1106
// 	//fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	//fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 	//fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 	//fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }
// int	CTranCmn::fn_FTX_TRANID_7330(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// //	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 	
// 	//#0045 -> ��� �ش� ���� ����
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;		
// }        
// int	CTranCmn::fn_FTX_TRANID_7336(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7337(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7338(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7339(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7340(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7341(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7342(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7343(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7344(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7345(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7350(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7351(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7352(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7353(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7354(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7357(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7358(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7359(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7360(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7361(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7362(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7363(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7364(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7365(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7366(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7367(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7370(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7371(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7372(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7373(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//         
// int	CTranCmn::fn_FTX_TRANID_7390(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7400(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7410(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7411(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7412(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7413(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7420(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7421(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7425(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7426(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7427(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7428(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7430(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7431(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//      
//                         
// int	CTranCmn::fn_FTX_TRANID_4300(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4301(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	return T_OK;	
// }        
//      
// int	CTranCmn::fn_FTX_TRANID_4214(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	return T_OK;	
// }        
//         
// int	CTranCmn::fn_FTX_TRANID_4216(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	return T_OK;	
// }        
// 
// //#N0171 �ؿ�ī�� BC ���                                    
// int	CTranCmn::fn_FTX_TRANID_4316(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_346X(nTranCode, TX_SEND_F); //�ؿ�ī�� �귣�� ����           //#N0171
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// } 
// 
// //#N0171 �ؿ�ī�� BC ��� ���     
// int	CTranCmn::fn_FTX_TRANID_4325(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// //	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 
// 	fnAPP_SetDataField_Item_346X(nTranCode, TX_SEND_F); //�ؿ�ī�� �귣�� ����           //#N0171
// 	
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// /*
// 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// */
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }
//                                    
// //#N0171 �ؿ�ī�� SAVING                                    
// int	CTranCmn::fn_FTX_TRANID_4311(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //����������             //#N0161-1
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// } 
// 
// //#N0171  �ؿ�ī�� CHECKING     
// int	CTranCmn::fn_FTX_TRANID_4312(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //����������             //#N0161-1
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }  
// int	CTranCmn::fn_FTX_TRANID_4314(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4315(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//       
// int	CTranCmn::fn_FTX_TRANID_4321(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// //	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// /*
// 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// */
// 	fnAPP_TxECash_SetDataField_SendProc();
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4322(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// //	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// /*
// 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// */
// 	fnAPP_TxECash_SetDataField_SendProc();
// 
// 	return T_OK;	
// }        
//       
// int	CTranCmn::fn_FTX_TRANID_4324(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// //#N0171 �ؿ�ī�� CREIDT        
// int	CTranCmn::fn_FTX_TRANID_4331(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //����������             //#N0161-1
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4332(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	//fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// //	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //����������             //#N0161-1
// 
// 	fnAPP_SetDataField_Item_346X(nTranCode, TX_SEND_F); //�ؿ�ī�� �귣�� ����           //#N0171
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4336(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4339(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//       
// int	CTranCmn::fn_FTX_TRANID_4341(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// //	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// /*
// 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// */
// 	fnAPP_TxECash_SetDataField_SendProc();
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4342(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// //	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// /*
// 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// */
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4349(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//      
// int	CTranCmn::fn_FTX_TRANID_4360(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4361(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }              
//                                     
// int	CTranCmn::fn_FTX_TRANID_4391(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4392(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }                                     
//    
// int	CTranCmn::fn_FTX_TRANID_4394(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4395(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4399(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4520(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4530(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4540(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4550(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4560(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4570(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4571(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4572(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4580(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4610(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4620(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4611(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4621(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4631(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4630(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4640(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4650(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4660(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4670(int nTranCode, int nSec)		//#N0266 �����н� ���� ��û
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_068X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ
// 	fnAPP_SetDataField_Item_0691(nTranCode, TX_SEND_F); //������ �ܾ�
// // 	fnAPP_SetDataField_Item_070X(nTranCode, TX_SEND_F); //����ȭ�� ��������
// // 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //����ȭ�� ������
// // 	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //����ȭ�� ī������
// // 	fnAPP_SetDataField_Item_073X(nTranCode, TX_SEND_F); //����ȭ�� �˰���ID
// // 	fnAPP_SetDataField_Item_074X(nTranCode, TX_SEND_F); //����ȭ�� Ű�¹���
// // 	fnAPP_SetDataField_Item_075X(nTranCode, TX_SEND_F); //����ȭ�� ���� ID
// // 	fnAPP_SetDataField_Item_076X(nTranCode, TX_SEND_F); //����ȭ�� ������ID
// // 	fnAPP_SetDataField_Item_077X(nTranCode, TX_SEND_F); //����ȭ�� �̿���ī��ID
// // 	fnAPP_SetDataField_Item_078X(nTranCode, TX_SEND_F); //����ȭ�� PAN
// // 	fnAPP_SetDataField_Item_079X(nTranCode, TX_SEND_F); //����ȭ�� ������ID
// // 	fnAPP_SetDataField_Item_080X(nTranCode, TX_SEND_F); //���� �ſ�ī�� ��ȣ
// // 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //��ȭ��ȣ
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_310X(nTranCode, TX_SEND_F); //���� ����
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4671(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// // 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// // 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// // 	fnAPP_SetDataField_Item_068X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ
// // 	fnAPP_SetDataField_Item_0691(nTranCode, TX_SEND_F); //������ �ܾ�
// // 	fnAPP_SetDataField_Item_070X(nTranCode, TX_SEND_F); //����ȭ�� ��������
// // 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //����ȭ�� ������
// // 	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //����ȭ�� ī������
// // 	fnAPP_SetDataField_Item_073X(nTranCode, TX_SEND_F); //����ȭ�� �˰���ID
// // 	fnAPP_SetDataField_Item_074X(nTranCode, TX_SEND_F); //����ȭ�� Ű�¹���
// // 	fnAPP_SetDataField_Item_075X(nTranCode, TX_SEND_F); //����ȭ�� ���� ID
// // 	fnAPP_SetDataField_Item_076X(nTranCode, TX_SEND_F); //����ȭ�� ������ID
// // 	fnAPP_SetDataField_Item_077X(nTranCode, TX_SEND_F); //����ȭ�� �̿���ī��ID
// // 	fnAPP_SetDataField_Item_078X(nTranCode, TX_SEND_F); //����ȭ�� PAN
// // 	fnAPP_SetDataField_Item_079X(nTranCode, TX_SEND_F); //����ȭ�� ������ID
// // 	fnAPP_SetDataField_Item_080X(nTranCode, TX_SEND_F); //���� �ſ�ī�� ��ȣ
// // 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //��ȭ��ȣ
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_310X(nTranCode, TX_SEND_F); //���� ����
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4680(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// 
// // 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// // 	fnAPP_SetDataField_Item_068X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ
// // 	fnAPP_SetDataField_Item_0691(nTranCode, TX_SEND_F); //������ �ܾ�
// // 	fnAPP_SetDataField_Item_070X(nTranCode, TX_SEND_F); //����ȭ�� ��������
// // 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //����ȭ�� ������
// // 	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //����ȭ�� ī������
// // 	fnAPP_SetDataField_Item_073X(nTranCode, TX_SEND_F); //����ȭ�� �˰���ID
// // 	fnAPP_SetDataField_Item_074X(nTranCode, TX_SEND_F); //����ȭ�� Ű�¹���
// // 	fnAPP_SetDataField_Item_075X(nTranCode, TX_SEND_F); //����ȭ�� ���� ID
// // 	fnAPP_SetDataField_Item_076X(nTranCode, TX_SEND_F); //����ȭ�� ������ID
// // 	fnAPP_SetDataField_Item_077X(nTranCode, TX_SEND_F); //����ȭ�� �̿���ī��ID
// // 	fnAPP_SetDataField_Item_078X(nTranCode, TX_SEND_F); //����ȭ�� PAN
// // 	fnAPP_SetDataField_Item_079X(nTranCode, TX_SEND_F); //����ȭ�� ������ID
// // 	fnAPP_SetDataField_Item_080X(nTranCode, TX_SEND_F); //���� �ſ�ī�� ��ȣ
// // 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //��ȭ��ȣ
// // 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_310X(nTranCode, TX_SEND_F); //���� ����
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4681(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4682(int nTranCode, int nSec)
// {	
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// 
// // 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// // 	fnAPP_SetDataField_Item_068X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ
// // 	fnAPP_SetDataField_Item_0691(nTranCode, TX_SEND_F); //������ �ܾ�
// // 	fnAPP_SetDataField_Item_070X(nTranCode, TX_SEND_F); //����ȭ�� ��������
// // 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //����ȭ�� ������
// // 	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //����ȭ�� ī������
// // 	fnAPP_SetDataField_Item_073X(nTranCode, TX_SEND_F); //����ȭ�� �˰���ID
// // 	fnAPP_SetDataField_Item_074X(nTranCode, TX_SEND_F); //����ȭ�� Ű�¹���
// // 	fnAPP_SetDataField_Item_075X(nTranCode, TX_SEND_F); //����ȭ�� ���� ID
// // 	fnAPP_SetDataField_Item_076X(nTranCode, TX_SEND_F); //����ȭ�� ������ID
// // 	fnAPP_SetDataField_Item_077X(nTranCode, TX_SEND_F); //����ȭ�� �̿���ī��ID
// // 	fnAPP_SetDataField_Item_078X(nTranCode, TX_SEND_F); //����ȭ�� PAN
// // 	fnAPP_SetDataField_Item_079X(nTranCode, TX_SEND_F); //����ȭ�� ������ID
// // 	fnAPP_SetDataField_Item_080X(nTranCode, TX_SEND_F); //���� �ſ�ī�� ��ȣ
// // 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //��ȭ��ȣ
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	
// 	fnAPP_SetDataField_Item_310X(nTranCode, TX_SEND_F); //���� ����
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;	
// }                                         
// int	CTranCmn::fn_FTX_TRANID_4711(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4712(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }                                    
//                                     
// int	CTranCmn::fn_FTX_TRANID_4719(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//       
// int	CTranCmn::fn_FTX_TRANID_4730(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// //	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	//#0045 -> ��� �ش� ���� ����
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0227
// 	{
// 		fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 	}
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0277
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //�������������ȣ(ȸ��)
// 	}
// 	
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;	
// }      
// 
//  
//  //#GIRO �Ϲ����� ���� MS 
// int	CTranCmn::fn_FTX_TRANID_4810(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;
// }        
// int	CTranCmn::fn_FTX_TRANID_4811(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// //	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //���¼���
// 	
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4812(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }
// 
// //#GIRO �Ϲ����� ���� IC        
// int	CTranCmn::fn_FTX_TRANID_4813(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;
// }        
// int	CTranCmn::fn_FTX_TRANID_4814(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// //	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // ���ι�ȣ
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // ���ڳ��ι�ȣ
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // ���γ��γ���
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // �̿�����(�ŷ��� ��ȣ)
// 
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //���¼���
// 	
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;
// }        
// int	CTranCmn::fn_FTX_TRANID_4820(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4821(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4822(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4830(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4840(int nTranCode, int nSec) //#N0266 �����н� ���� ��û
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// //	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4850(int nTranCode, int nSec)  //#N0266 �����н� ���� ��û
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// //	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST�Ϸù�ȣ
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;	
// }
// 
// int	CTranCmn::fn_FTX_TRANID_4717(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //�Ա��ڿ���ó -> //#0107 -> #0111 ** (��������)
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_TxECash_SetDataField_SendProc();		
// 	
// 	return T_OK;
// } 
// 
//         
// int	CTranCmn::fn_FTX_TRANID_4841(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4851(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4842(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4852(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4860(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7000(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7004(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7005(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7006(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }                   
// 
// //#N0171 �ؿ�ī�� ������ ��ȸ                    
// int	CTranCmn::fn_FTX_TRANID_7104(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }  
// 
// 
// //#N0171 �ؿ�ī�� ������ ��ȸ                    
// int	CTranCmn::fn_FTX_TRANID_7105(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_346X(nTranCode, TX_SEND_F); //�ؿ�ī�� �귣�� ����           //#N0171
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7120(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7124(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7125(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//                                           
//         
// int	CTranCmn::fn_FTX_TRANID_7224(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7225(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//   
// int	CTranCmn::fn_FTX_TRANID_7228(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//        
// int	CTranCmn::fn_FTX_TRANID_7231(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //����������             //#N0161-1
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7232(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_346X(nTranCode, TX_SEND_F); //�ؿ�ī�� �귣�� ����           //#N0171
// 
// //	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// //	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7240(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7241(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//  
// int	CTranCmn::fn_FTX_TRANID_7221(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //����������             //#N0161-1
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7222(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //����������             //#N0161-1
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }                         
// 
// int	CTranCmn::fn_FTX_TRANID_7233(int nTranCode, int nSec)  //#N0235
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //����������             //#N0161-1
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }
//                                     
// int	CTranCmn::fn_FTX_TRANID_7235(int nTranCode, int nSec)  //#N0235
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// // //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// // //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //����������             //#N0161-1
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }
// 
// int	CTranCmn::fn_FTX_TRANID_7237(int nTranCode, int nSec)  //#N0235
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //����������             //#N0161-1
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }
//         
// int	CTranCmn::fn_FTX_TRANID_7262(int nTranCode, int nSec)
// {
// 
// 	return T_OK;	
// }        
//                                     
//                 
//                                                        
//                                    
// 
// 
// int	CTranCmn::fn_FTX_TRANID_7229(int nTranCode, int nSec)  //#N0239
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//�����
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //��¥
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //�ð�
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //�Ϸù�ȣ
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //��������
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //�ŷ����� - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //�ŷ���ü����
// //	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //�����ڵ�
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //�ܸ����ȣ
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2������ �ſ�ī�� ����(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //�������
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //��ް��ڵ�
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //��ȣȭ (���,����)
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //��й�ȣ
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //��ޱ���ڵ�
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)��ȣ1
// //	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //�������ڵ�
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //������¹�ȣ
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //������¼���
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //�ŷ��ݾ�
// //	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //�ŷ�������
// //	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //�̰���Ÿ���� �ݾ�
// 
// //	if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0239
// //		fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //�ŷ�����(ī��)�ܾ�
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //�ŷ������Ͻ�(ī�����)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //������¼���
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //��ǥ��Ͻð�
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //����ȭ�� �Ϸù�ȣ(ICī���Ϸù�ȣ)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST�۾� ������ ������
// 
// //	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //������ ��ݸż�
// //	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5������ ��ݸż�
// //	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10������ ��ݸż�
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //��õ���� �ż�
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //õ���� �ż�
// 
// //	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //��й�ȣ(4�ڸ�)
// //	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //��й�ȣ(����/�ϳ�/�Ｚ��/����)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// } 


int	CTranCmn::fn_FTX_EMVSendMake_Lib01(int nTranCode, int nSec)
{
	if (EMVReadFlg == FALSE)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fn_FTX_EMVSendMake_Lib[%x]", m_pDevCmn->nEmvStep);
		return T_OK;
	}

	if(nTranCode == TRANID_4330 || nTranCode == TRANID_7380 || nTranCode == TRANID_7230 ||  nTranCode == TRANID_4350 || nTranCode == TRANID_7530)
	{
		if (EMVReadFlg)												// EMV �۽ŵ����� ����
		{
			if(m_pDevCmn->IfmDirect)					
			{
				fnAPDLib_EMVSendMakeProc();                         
			}
			else
			{
				if ( fnAPD_EMVSendMakeProc() != T_OK )
				{
					FallBackCnt = 0x32;							// FallBack : GenerateAC Fail : 0
					EMVReadFlg = FALSE;
					CardPowerOnFlg = FALSE;						// ī�����ӹ�
					FallBackEMVMode = TRUE;
					
					if (!(CardTran & MENU_MCU2))
					{
						fnAPP_CancelProc(T_MSG, "EMV�۽����� �ۼ�����");
					}
				}
			}
		}
	}
	


	memset(&m_RecvEmvrcvData,	    ' ', sizeof(RECVEMVRCVDATA));	
	int		nOffset(0);
	int		nnOffsetIdx(0);
	int		nOffsetCvtPos(0);
	char	szBcCvtFormat[1024];

	switch(CardBank)
	{	
		case nEnCmd181_105:    //�ϳ�SKī��
		case nEnCmd111_111:    //����ī��
		case nEnCmd126_192:    //����ī��
		case nEnCmd194_194:    //BC ī��
		case nEnCmd107_107:    //����ī��	
		case nEnCmd120_120:    //�츮 ī��
//		case nEnCmd127_127:    //��Ƽ -> ����ó��
			//1. Terminal Capabilitise(9F33)(3)
			//2. Terminal Verification Result(95)(5)
			//3. Unpredictable Number(9F37)(4)
			//4. Application Cryptogram(9F26)(8)
			//5. Application Transaction Counter(9F36)(2)
			//6. Application Interchange Profile(82)(2)
			//7. Transaction Type(9C)(1)
			//8. Terminal Country Code(9F1A)(2)                  --> ������ (�����ʿ�)
			//9. Terminal Transaction Date(9A)(3)
			//10. Transaction Amount(9F02)(6)
			//11. Transaction Currency Code(5F2A)(2)
			//12. Another Transaction Amount(9F03)(6)
			//13. Cryptogram Information Data(9F27)(1)	
			//14. PAN Sequence Number(5F34)(1)
			//sum = 92;
			nnOffsetIdx = 92;
			memcpy(&szBcCvtFormat[nOffset], &m_pDevCmn->m_EMVszData[nOffset], nnOffsetIdx);

																									//1. Terminal Capabilitise(9F33)(3)
																									//2. Terminal Verification Result(95)(5)
																									//3. Unpredictable Number(9F37)(4)
																									//4. Application Cryptogram(9F26)(8)
																									//5. Application Transaction Counter(9F36)(2)
																									//6. Application Interchange Profile(82)(2)
																									//7. Transaction Type(9C)(1)
																									//8. Terminal Country Code(9F1A)(2)                  --> ������ (�����ʿ�)
																									//9. Terminal Transaction Date(9A)(3)
																									//10. Transaction Amount(9F02)(6)
																									//11. Transaction Currency Code(5F2A)(2)
																									//12. Another Transaction Amount(9F03)(6)
																									//13. Cryptogram Information Data(9F27)(1)	
																									//14. PAN Sequence Number(5F34)(1)
																									//15. TerminalType(9F35)(1)	
																									//16. Amount in Ref. Currency(9F3A)(4)
																									//17. Transaction Ref. Currency Code(9F3C) (2)
																									//18. Data Authentication Code(DAC) ( 9F45 ) (2)
																									//19. M/CHIP Version(9F09)(1)	
																									//20. Issuer Application Data(9F10)(Variables) (2) -> 20. Length Indicator[2]
			nOffset += nnOffsetIdx; //92
			nOffsetCvtPos = 113;
			memcpy(&szBcCvtFormat[nOffset], &m_pDevCmn->m_EMVszData[nOffsetCvtPos], 16);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData FI = [%16.16s]", &m_pDevCmn->m_EMVszData[nOffsetCvtPos]); /*����ó��*/	
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData BC = [%16.16s]", &szBcCvtFormat[nOffset]); /*����ó��*/	
			//15. Derivation Key Index[2]
			//16. Cryptogram Version Nr[2]
			//17. Card Verification Result[12]	
			nOffset += 2;
			nOffset += 2;
			nOffset += 12;
			//18. TerminalType(9F35)(1)	
			//19. Amount in Ref. Currency(9F3A)(4)
			//20. Transaction Ref. Currency Code(9F3C) (2)
			//21. Data Authentication Code(DAC) ( 9F45 ) (2)
			//22. M/CHIP Version(9F09)(1)
			nOffsetCvtPos = nnOffsetIdx; //92
			memcpy(&szBcCvtFormat[nOffset], &m_pDevCmn->m_EMVszData[nOffsetCvtPos], 19);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData FI = [%19.19s]", &m_pDevCmn->m_EMVszData[nOffsetCvtPos]); /*����ó��*/	
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData BC = [%19.19s]", &szBcCvtFormat[nOffset]); /*����ó��*/	

			nOffset += 19;
			nOffsetCvtPos += 19;
	

			//19. Issuer Application Data(9F10)(Variables) (2) -> 20. Length Indicator[2]  	
			memcpy(&szBcCvtFormat[nOffset], &m_pDevCmn->m_EMVszData[nOffsetCvtPos], 2);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData FI = [%2.2s]", &m_pDevCmn->m_EMVszData[nOffsetCvtPos]); /*����ó��*/	
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData BC = [%2.2s]", &szBcCvtFormat[nOffset]); /*����ó��*/	

			nOffset += 2;	
			nOffsetCvtPos += 2;


			nOffsetCvtPos += 2;
			nOffsetCvtPos += 2;
			nOffsetCvtPos += 12;
			//23. Issuer Discretionary Data[50] 
			memcpy(&szBcCvtFormat[nOffset], &m_pDevCmn->m_EMVszData[nOffsetCvtPos], 50);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData FI = [%50.50s]", &m_pDevCmn->m_EMVszData[nOffsetCvtPos]); /*����ó��*/	
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData BC = [%50.50s]", &szBcCvtFormat[nOffset]); /*����ó��*/	

			nOffset += 50;			

			HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVszData FI", m_pDevCmn->m_EMVszData, 200);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData FI = [%192.192s]", m_pDevCmn->m_EMVszData); /*����ó��*/	
			memset(m_pDevCmn->m_EMVszData, ' ', sizeof(m_pDevCmn->m_EMVszData));				// EMV Send RecvData ����
			memcpy(m_pDevCmn->m_EMVszData, szBcCvtFormat, nOffset);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData BC = [%192.192s]", m_pDevCmn->m_EMVszData); /*����ó��*/	
			HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVszData BC", m_pDevCmn->m_EMVszData, 200);

			break;
		default:
			//1. Terminal Capabilitise(9F33)(3)
			//2. Terminal Verification Result(95)(5)
			//3. Unpredictable Number(9F37)(4)
			//4. Application Cryptogram(9F26)(8)
			//5. Application Transaction Counter(9F36)(2)
			//6. Application Interchange Profile(82)(2)
			//7. Transaction Type(9C)(1)
			//8. Terminal Country Code(9F1A)(2)                  --> ������ (�����ʿ�)
			//9. Terminal Transaction Date(9A)(3)
			//10. Transaction Amount(9F02)(6)
			//11. Transaction Currency Code(5F2A)(2)
			//12. Another Transaction Amount(9F03)(6)
			//13. Cryptogram Information Data(9F27)(1)	
			//14. PAN Sequence Number(5F34)(1)
			//15. TerminalType(9F35)(1)	
			//16. Amount in Ref. Currency(9F3A)(4)
			//17. Transaction Ref. Currency Code(9F3C) (2)
			//18. Data Authentication Code(DAC) ( 9F45 ) (2)
			//19. M/CHIP Version(9F09)(1)	
			//20. Issuer Application Data(9F10)(Variables) (2) -> 20. Length Indicator[2]
			//sum = 113;
			//21. Derivation Key Index[2]
			//22. Cryptogram Version Nr[2]
			//23. Card Verification Result[12]
			//24. Issuer Discretionary Data[50]    	
// 
// 
// 	//**************************************************************************/
// 	/*	[IC CARD�� ��� IC �ſ�ī�� ����] -> Kim.Gi.Jin -> ���ΰ˻� �ҽ� 1�� �Ϸ�
// 		1. 001 - 006 : Terminal Capability Profile(9F33)
// 		2. 007 - 016 : Terminal Verification Result(95)
// 		3. 017 - 024 : Unpredictable Number(9F37)
// 		4. 025 - 040 : Cryptogram (ARQC, TC, AAC) (9F26)
// 		5. 041 - 044 : Application Transaction Counter(9F36)
// 		6. 045 - 048 : Application Interchange Profile(82)
// 		7. 049 - 050 : Cryptogram Transaction Type(9C)
// 		8. 051 - 054 : Terminal Country Code(9F1A)
// 		9. 055 - 060 : Terminal Transaction Date(9A)
// 		10.061 - 072 : Cryptogram Amount(9F02)
// 		11.073 - 076 : Cryptogram Currency Code(5F2A)
// 		12.077 - 088 : Cryptogram Cashback Amount(9F03)
// 		13.089 - 090 : Cryptogram Information Data(9F27)
// 		14.091 - 092 : Card Sequence Number(5F34)
// 		15.093 - 094 : Terminal Type(9F35)
// 		16.095 - 102 : Amount in Ref. Currency(9F3A)
// 		17.103 - 106 : Transaction Ref. Currency(9F3C)
// 		18.107 - 110 : Data Authentication Code(9F45)
// 		19.111       : M/chip version 
// 				       (1-M/Chip2 old, 2-M/Chip2 new, 4-M/chip4)
// 		20.112 - 179 : Issuer Application Data(9F10) 
// 		   1) 112 - 113 : Length Indicator ( 2byte ) 
// 		   2) 114 - 115 : Derivation Key Index (2byte)
// 		   3) 116 - 117 : Cryptogram Version Nr.(2byte)
// 		   4) 118 - 129 : Card Verification Result(12byte)
// 		   5) 130 - 179 : Issuer Discretionary Data
// 			  a) Length Indicator2 (1byte)
// 			  b) Data(var.)
// 		21.180 -192 : reserved */
// 	//**************************************************************************/
			break;
	}

            
             
// 
// 	//1. Terminal Capabilitise(9F33)(3)
// 	sprintf(szTmp, "%6.6s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 6;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Terminal Capabilitise", m_pDevCmn->EMVDOLListInfo.TerminalCapabilitise, sizeof(m_pDevCmn->EMVDOLListInfo.TerminalCapabilitise));
// 
// 	//2. Terminal Verification Result(95)(5)
// 	sprintf(szTmp, "%10.10s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 10;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Terminal Verification Result", m_pDevCmn->EMVDOLListInfo.TerminalVerifyResult, sizeof(m_pDevCmn->EMVDOLListInfo.TerminalVerifyResult));
// 
// 	//3. Unpredictable Number(9F37)(4)
// 	sprintf(szTmp, "%8.8s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 8;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Unpredictable Number", m_pDevCmn->EMVDOLListInfo.UnpredictableNumber, sizeof(m_pDevCmn->EMVDOLListInfo.UnpredictableNumber));
// 
// 	//4. Application Cryptogram(9F26)(8)
// 	sprintf(szTmp, "%16.16s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 16;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Application Cryptogram", m_pDevCmn->EMVDOLListInfo.AppCryptogram, sizeof(m_pDevCmn->EMVDOLListInfo.AppCryptogram));
// 
// 	//5. Application Transaction Counter(9F36)(2)
// 	sprintf(szTmp, "%4.4s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 4;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Application Cryptogram", m_pDevCmn->EMVDOLListInfo.AppTranCounter, sizeof(m_pDevCmn->EMVDOLListInfo.AppTranCounter));
// 	
// 	//6. Application Interchange Profile(82)(2)
// 	sprintf(szTmp, "%4.4s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 4;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Interchange Profile", m_pDevCmn->EMVDOLListInfo.AppInterchangeProfile, sizeof(m_pDevCmn->EMVDOLListInfo.AppInterchangeProfile));
// 
// 	//7. Transaction Type(9C)(1)
// 	sprintf(szTmp, "%2.2s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 2;
// 
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Transaction Type [%c]", m_pDevCmn->EMVDOLListInfo.TransactionType);
// 
// 	//8. Terminal Country Code(9F1A)(2)
// 	sprintf(szTmp, "%4.4s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 4;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Terminal Country Code", m_pDevCmn->EMVDOLListInfo.TerminalCountryCode, sizeof(m_pDevCmn->EMVDOLListInfo.TerminalCountryCode));
// 
// 	//9. Terminal Transaction Date(9A)(3)
// 	sprintf(szTmp, "%6.6s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 6;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Transaction Date", m_pDevCmn->EMVDOLListInfo.TransactionDate, sizeof(m_pDevCmn->EMVDOLListInfo.TransactionDate));
// 
// 	//10. Transaction Amount(9F02)(6)
// 	sprintf(szTmp, "%12.12s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 12;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Transaction Amount", m_pDevCmn->EMVDOLListInfo.TranAmountBCD, sizeof(m_pDevCmn->EMVDOLListInfo.TranAmountBCD));
// 
// 	//11. Transaction Currency Code(5F2A)(2)
// 	sprintf(szTmp, "%4.4s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 4;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Transaction Currency Code", m_pDevCmn->EMVDOLListInfo.TranCurrencyCode, sizeof(m_pDevCmn->EMVDOLListInfo.TranCurrencyCode));
// 
// 	//12. Another Transaction Amount(9F03)(6)
// 	sprintf(szTmp, "%12.12s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 12;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Another Transaction Amount", m_pDevCmn->EMVDOLListInfo.AnotherTranAmountBCD, sizeof(m_pDevCmn->EMVDOLListInfo.AnotherTranAmountBCD));
// 
// 	//13. Cryptogram Information Data(9F27)(1)
// 	sprintf(szTmp, "%2.2s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 2;
// 
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Cryptogram Information Data[%x]", m_pDevCmn->EMVDOLListInfo.CryptogramInfoData);
// 
// 	//14. PAN Sequence Number(5F34)(1)
// 	sprintf(szTmp, "%2.2s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 2;
// 
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PAN Sequence Number[%x]", m_pDevCmn->EMVDOLListInfo.CSN);
// 	//15. TerminalType(9F35)(1)
// 	sprintf(szTmp, "%2.2s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 2;
// 
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TerminalType[%x]", m_pDevCmn->EMVDOLListInfo.TerminalType);
// 	
// 	//16. Amount in Ref. Currency(9F3A)(4)
// 	sprintf(szTmp, "%8.8s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 8;
// 
// 	
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Amount in Ref. Currency[%x]", m_pDevCmn->EMVDOLListInfo.TerminalType);
// 	
// 	//17. Transaction Ref. Currency Code(9F3C) (2)
// 	//sprintf(szTmp, "%4.4s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	sprintf(szTmp, "%4.4s", "0410");
// 	nOffset += 4;
// 
// 	//18. Data Authentication Code(DAC) ( 9F45 ) (2)
// 	sprintf(szTmp, "%4.4s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 4;
// 
// 	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Data Authentication Code", &m_pDevCmn->EMVDOLListInfo.IssuerAppData[12], 2);
// 	
// 	//19. M/CHIP Version(9F09)(1)
// 	sprintf(szTmp, "%1.1s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 1;
// 
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Amount in Ref. Currency[%c]", m_pDevCmn->EMVDOLListInfo.TerminalType);
// 	
// 	ZeroMemory(szBcCvtFormat, 16);
// 	//20. Issuer Application Data(9F10)(Variables) (2) -> 20. Length Indicator[2]
// 	sprintf(szTmp, "%2.2s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 2;
// //-----------------------------------------------------------------------------------------------------------------------------------
// 	//21. Derivation Key Index[2]
// 	sprintf(szTmp, "%2.2s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	memcpy(&szBcCvtFormat[nSaveOffset], &m_pDevCmn->m_EMVszData[nOffset], 2)
// 	nOffset += 2;
// 
// 	//22. Cryptogram Version Nr[2]
// 	sprintf(szTmp, "%2.2s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	memcpy(&szBcCvtFormat[nSaveOffset], &m_pDevCmn->m_EMVszData[nOffset], 2)
// 	nOffset += 2;
// 
// 	//23. Card Verification Result[12]
// 	sprintf(szTmp, "%12.12s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	memcpy(&szBcCvtFormat[nSaveOffset], &m_pDevCmn->m_EMVszData[nOffset], 12)
// 	nOffset += 12;
// //-----------------------------------------------------------------------------------------------------------------------------------
// 
// 	//24. Issuer Discretionary Data[50]
// 	sprintf(szTmp, "%50.50s", &m_pDevCmn->m_EMVszData[nOffset]);
// 	nOffset += 50;


	
	
//	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo", (char *) &m_pDevCmn->EMVDOLListInfo.AppIDLen, sizeof(m_pDevCmn->EMVDOLListInfo));
//	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVszData", m_pDevCmn->m_EMVszData, sizeof(m_pDevCmn->m_EMVszData));

	return T_OK;
}

