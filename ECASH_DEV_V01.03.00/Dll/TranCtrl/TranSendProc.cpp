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
/* Input	     : TRCODE (라인전문)
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
/* Input	     : TRCODE (개국전문)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/  
int	CTranCmn::fn_FTX_TRANID_A010(int nTranCode, int nSec)
{
	
	fnAPP_ClearArrDataField(TX_SEND_F);
	fnAPP_Init_031X();

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->


// 
// 	fnAPP_SetDataField_Item_044X(nTranCode, TX_SEND_F); //보초암호화KEY
// 	
// 
// 	//운영관련
// 	fnAPP_SetDataField_Item_034X(nTranCode, TX_SEND_F); //장애구분(정상,계원)
// 	fnAPP_SetDataField_Item_035X(nTranCode, TX_SEND_F); //지폐함 및 용지 상태정보
// 	fnAPP_SetDataField_Item_036X(nTranCode, TX_SEND_F); //지폐함별 권종 설정정보
 	fnAPP_SetDataField_Item_047X(nTranCode, TX_SEND_F); //SST Serial Number(SST-ID)
 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
 	fnAPP_SetDataField_Item_065X(nTranCode, TX_SEND_F); //디바이스별 모델 정보
// 	fnAPP_SetDataField_Item_083X(nTranCode, TX_SEND_F); //통신망관리정보
// 	fnAPP_SetDataField_Item_084X(nTranCode, TX_SEND_F); //Shutdown시각
 	fnAPP_SetDataField_Item_088X(nTranCode, TX_SEND_F); //단말기의 IP Address(xxx.xxx.xxx.xxx)
 	fnAPP_SetDataField_Item_089X(nTranCode, TX_SEND_F); //단말기의 LanCard의 Physical Address('-'없이 Data만)
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)
 	fnAPP_SetDataField_Item_094X(nTranCode, TX_SEND_F); //명세표 광고 출력 버전


 	fnAPP_SetDataField_Item_171X(nTranCode, TX_SEND_F); //설치장소의 영업업체 코드 정보	
	fnAPP_SetDataField_Item_172X(nTranCode, TX_SEND_F); //설치장소의 브랜드구분 코드 정보
 	fnAPP_SetDataField_Item_173X(nTranCode, TX_SEND_F); //단말기의 카세트별 설정된 권종정보


	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}        
int	CTranCmn::fn_FTX_TRANID_A020(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);
	
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_SetDataField_Item_006X (%d)", nTranCode);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->


	//운영관련
	fnAPP_SetDataField_Item_034X(nTranCode, TX_SEND_F); //장애구분(정상,계원)
	fnAPP_SetDataField_Item_035X(nTranCode, TX_SEND_F); //지폐함 및 용지 상태정보
//	fnAPP_SetDataField_Item_036X(nTranCode, TX_SEND_F); //지폐함별 권종 설정정보
	fnAPP_SetDataField_Item_040X(nTranCode, TX_SEND_F); //전달정보
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류

//	fnAPP_SetDataField_Item_047X(nTranCode, TX_SEND_F); //SST Serial Number(SST-ID)

	if (m_pDevCmn->SCSTCntF == 2)
	{
 		fnAPP_SetDataField_Item_055X(nTranCode, TX_SEND_F); //현금장입금액
 		fnAPP_SetDataField_Item_056X(nTranCode, TX_SEND_F); //현금장입시각
	}

//  	fnAPP_SetDataField_Item_055X(nTranCode, TX_SEND_F); //현금장입금액
//  	fnAPP_SetDataField_Item_056X(nTranCode, TX_SEND_F); //현금장입시각

 	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //일반전문(출금가능 수표액), 시재전문(기기보유 수표액)
 	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간

 	fnAPP_SetDataField_Item_087X(nTranCode, TX_SEND_F); //장애세부코드
	fnAPP_SetDataField_Item_088X(nTranCode, TX_SEND_F); //단말기의 IP Address(xxx.xxx.xxx.xxx)
	fnAPP_SetDataField_Item_100X(nTranCode, TX_SEND_F); //전원OFF기간
	fnAPP_SetDataField_Item_102X(nTranCode, TX_SEND_F); //H/W Module 상태정보
	fnAPP_SetDataField_Item_103X(nTranCode, TX_SEND_F); //자원 감시 상태정보

 	fnAPP_SetDataField_Item_173X(nTranCode, TX_SEND_F); //단말기의 카세트별 설정된 권종정보
 	fnAPP_SetDataField_Item_464X(nTranCode, TX_SEND_F); //현금부족체크

	fnAPP_TxECash_SetDataField_SendProc();	

	m_pTranCmn->ForceSendStatus         = STATUS_OFF_TIME;
	return T_OK;	
}   

/**********************************************************************************************************/
/* Function Name : Data Format Make (send) gijin
/* 
/* Date			 : 2012.01.30
/* 
/* Input	     : TRCODE (제어전문)
/*
/* Output	     : T_OK, T_FAIL
/**********************************************************************************************************/  
int	CTranCmn::fn_FTX_TRANID_A040(int nTranCode, int nSec)
{
	
	fnAPP_ClearArrDataField(TX_SEND_F);

 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}

int	CTranCmn::fn_FTX_TRANID_2190(int nTranCode, int nSec, int nEncFlag)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	//암호화
	if(Asc2Int(m_B_E_INFO_INI.m_B_S_C[5].k_s_flag,2) == CIPHERTEXT_TMP)
		fnAPP_KeyInitStart(CardBank, 1);
	else
		fnAPP_KeyInitStart(CardBank, 0);



 	fnAPP_SetDataField_Item_043X(nTranCode, TX_SEND_F); //주암호화Key
	fnAPP_SetDataField_Item_044X(nTranCode, TX_SEND_F); //보초암호화KEY 

	fnAPP_SetDataField_Item_047X(nTranCode, TX_SEND_F); //SST Serial Number(SST-ID)
 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류

	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //인증서 요청 데이터
	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)


	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}        

int	CTranCmn::fn_FTX_TRANID_A060(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_047X(nTranCode, TX_SEND_F); //SST Serial Number(SST-ID)
 	fnAPP_SetDataField_Item_055X(nTranCode, TX_SEND_F); //현금장입금액
 	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //일반전문(출금가능 수표액), 시재전문(기기보유 수표액)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)


	//운영관련
	fnAPP_SetDataField_Item_105X(nTranCode, TX_SEND_F); //총예금출금건수
	fnAPP_SetDataField_Item_106X(nTranCode, TX_SEND_F); //총예금출금금액
	fnAPP_SetDataField_Item_107X(nTranCode, TX_SEND_F); //총예금취소 총건수
	fnAPP_SetDataField_Item_108X(nTranCode, TX_SEND_F); //총예금취소 총금액
	fnAPP_SetDataField_Item_109X(nTranCode, TX_SEND_F); //총서비스출금건수
	fnAPP_SetDataField_Item_110X(nTranCode, TX_SEND_F); //총서비스출금금액
	fnAPP_SetDataField_Item_111X(nTranCode, TX_SEND_F); //총서비스출금취소건수
	fnAPP_SetDataField_Item_112X(nTranCode, TX_SEND_F); //총서비스출금취소금액
	fnAPP_SetDataField_Item_113X(nTranCode, TX_SEND_F); //총이체건수
	fnAPP_SetDataField_Item_114X(nTranCode, TX_SEND_F); //총이체금액
	fnAPP_SetDataField_Item_117X(nTranCode, TX_SEND_F); //총이체건수
	fnAPP_SetDataField_Item_118X(nTranCode, TX_SEND_F); //총이체금액

	fnAPP_SetDataField_Item_122X(nTranCode, TX_SEND_F); //총서비스이체건수
	fnAPP_SetDataField_Item_123X(nTranCode, TX_SEND_F); //총서비스이체금액

	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //총입금건수
	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //총입금금액

	fnAPP_SetDataField_Item_132X(nTranCode, TX_SEND_F); //이전시제합계일자
	fnAPP_SetDataField_Item_133X(nTranCode, TX_SEND_F); //이전시제합계시각
	fnAPP_SetDataField_Item_134X(nTranCode, TX_SEND_F); //현재시제합계일자
	fnAPP_SetDataField_Item_135X(nTranCode, TX_SEND_F); //현재시제합계시각


	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}        
  
int	CTranCmn::fn_FTX_TRANID_4310(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
//	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분
	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //암호화 블럭데이터 
	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 
	
	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)

	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //상성증권, 기업만 사용

//	setData036x(); // 면세전 수수료 (광주은행 예금출금시 사용)	-> 물어보기												


	fnAPP_TxECash_SetDataField_SendProc();
	
	return T_OK;	
}    



int	CTranCmn::fn_FTX_TRANID_4370(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);
	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_004X(nTranCode, TX_SEND_F); //제휴사 일련번호 
	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
	
	if (TranEncCode == nEnCmd243_243)
		fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)

	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분

	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명

 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
    fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료

 	fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //출금점 
	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //취급점 
 	fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //입금점

//  fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //암호화 블럭데이터 

 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
//	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
	fnAPP_SetDataField_Item_062X(nTranCode, TX_SEND_F); //업무세부구분코드 	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)

// 	fn_FTX_EMVSendMake_Lib01(nTranCode);
//  fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //신용카드IC관련정보
//  fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	
	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //삼성증권, 기업만 사용

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}        

int	CTranCmn::fn_FTX_TRANID_4390(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);
	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_004X(nTranCode, TX_SEND_F); //제휴사 일련번호 
	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호

	if (TranEncCode == nEnCmd243_243)
		fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)

	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분

	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명

 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
    fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료

 	fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //출금점 
	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //취급점 
 	fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //입금점

//  fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //암호화 블럭데이터 

 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
//	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
	fnAPP_SetDataField_Item_062X(nTranCode, TX_SEND_F); //업무세부구분코드 	

 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)

// 	fn_FTX_EMVSendMake_Lib01(nTranCode);
//  fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //신용카드IC관련정보
//  fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	
	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //삼성증권, 기업만 사용

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}        

int	CTranCmn::fn_FTX_TRANID_4410(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);


	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
//	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분
	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료

	//유안타증권만 사용
	if (TranEncCode == nEnCmd209_379_PUBLIC)
	{
		fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //출금점 
		fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //취급점 
		fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //입금점
	}
	
	
//	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
//	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //암호화 블럭데이터 

	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)
 	fnAPP_SetDataField_Item_379X(nTranCode, TX_SEND_F); //휴대폰번호


	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}     

int	CTranCmn::fn_FTX_TRANID_7230(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //암호화 블럭데이터 

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 
	
	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)

 	fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //신용카드IC관련정보
 //	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	

	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //해외신용
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //해외신용
		}
	}

	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
} 

int	CTranCmn::fn_FTX_TRANID_7250(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);


	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분

	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호

 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료

 	fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //출금점 
	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //취급점 
 	fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //입금점

//  fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //암호화 블럭데이터 

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 

//	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
//	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
	fnAPP_SetDataField_Item_062X(nTranCode, TX_SEND_F); //업무세부구분코드 	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)

	fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //신용카드IC관련정보
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	

	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //해외신용
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //해외신용
		}
	}

	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}        

int	CTranCmn::fn_FTX_TRANID_4350(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분

	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호

 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료

 	fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //출금점 
	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //취급점 
 	fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //입금점

//  fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //암호화 블럭데이터 

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 

//	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
//	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
	fnAPP_SetDataField_Item_062X(nTranCode, TX_SEND_F); //업무세부구분코드 	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)


 	fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //신용카드IC관련정보
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	

	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //해외신용
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //해외신용
		}
	}

	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}   

int	CTranCmn::fn_FTX_TRANID_7200(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분
	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
//	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
//	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //암호화 블럭데이터 

	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)
 	fnAPP_SetDataField_Item_379X(nTranCode, TX_SEND_F); //휴대폰번호


	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}    

int	CTranCmn::fn_FTX_TRANID_7260(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분

	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호

 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료

//  fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //출금점 
// 	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //취급점 
//  fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //입금점

//  fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //암호화 블럭데이터 

 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
//	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
	fnAPP_SetDataField_Item_062X(nTranCode, TX_SEND_F); //업무세부구분코드 	

 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)

	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //상성증권, 기업만 사용


	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}     


//취소
int	CTranCmn::fn_FTX_TRANID_4320(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분
	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
//	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_050X(nTranCode, TX_SEND_F); //원거래 단말기 일련번호 (050X)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	
	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)

 	fnAPP_SetDataField_Item_445X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값

	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //삼성증권, 기업만 사용

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}  


int	CTranCmn::fn_FTX_TRANID_4340(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)

	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분
 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_050X(nTranCode, TX_SEND_F);
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 
	
	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)

// 	fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //신용카드IC관련정보
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC
	


	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //해외신용
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //해외신용
		}
	}


	fnAPP_TxECash_SetDataField_SendProc();	

	return T_OK;	
}  


//////////////////////////////////////////////////////////////////////////
// 현금서비스 출금 이자율 조회 
int	CTranCmn::fn_FTX_TRANID_7380(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);


	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분
 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값

	fnAPP_SetDataField_Item_459X(nTranCode, TX_SEND_F); //신용카드IC관련정보

    fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //신용카드IC관련정보
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC


	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //해외신용
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //해외신용
		}
	}

	fnAPP_TxECash_SetDataField_SendProc();	
		

	return T_OK;	
}                                    

int	CTranCmn::fn_FTX_TRANID_7530(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);


	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분
 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 

	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값

	fnAPP_SetDataField_Item_459X(nTranCode, TX_SEND_F); //신용카드IC관련정보

    fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //신용카드IC관련정보
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC



	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //해외신용
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //해외신용
		}
	}
	
	fnAPP_TxECash_SetDataField_SendProc();	
		

	return T_OK;	
}   

int	CTranCmn::fn_FTX_TRANID_7220(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //발급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1


// 	fnAPP_SetDataField_Item_027X(nTranCode, TX_SEND_F); //출금점 
// 	fnAPP_SetDataField_Item_028X(nTranCode, TX_SEND_F); //취급점 
//	fnAPP_SetDataField_Item_029X(nTranCode, TX_SEND_F); //입금점
	
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); ////암호화 블럭데이터



	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 
	
	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)


	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data

	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //상성증권, 기업만 사용

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
}        


int	CTranCmn::fn_FTX_TRANID_7210(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분
	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)
	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); ////암호화 블럭데이터

	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 
	
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)


	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호

	if (CardBank == KUB_ORG_INT_003 || CardBank == SST_ORG_INT_240)
		fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //상성증권, 기업만 사용

	fnAPP_TxECash_SetDataField_SendProc();	
	return T_OK;	
} 
       
int	CTranCmn::fn_FTX_TRANID_4330(int nTranCode, int nSec)
{
	fnAPP_ClearArrDataField(TX_SEND_F);

	//공통부
	fnAPP_SetDataField_Item_081X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID

//	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_010X(nTranCode, TX_SEND_F); //암호화구분
	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //암호화종류 (2 Byte)
 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_101X(nTranCode, TX_SEND_F); //거래가능기관 정보의 버젼값
	//--->

	TranEncCode = fnAPP_EncType(TranCode2);
	fnAPP_EncCall(TranEncCode, TranCode2);

	fnAPP_SetDataField_Item_007X(nTranCode, TX_SEND_F); //사용 MS Track 번호
//	fnAPP_SetDataField_Item_009X(nTranCode, TX_SEND_F); //마감구분 (009X)
	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //ISO2 Data
	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급관코드
	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
//	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //출금매체구분
 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
//  fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
 	fnAPP_SetDataField_Item_038X(nTranCode, TX_SEND_F); //보너스 매체 출금가능 정보 
	fnAPP_SetDataField_Item_042X(nTranCode, TX_SEND_F); //출금가능 금액(총액) -> 일반전문(출금가능총액), 시재전문(기기보유 현금액)
 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
	fnAPP_SetDataField_Item_052X(nTranCode, TX_SEND_F); //거래계좌번호(기관용)

// 	fnAPP_SetDataField_Item_057X(nTranCode, TX_SEND_F); //IC카드 일련번호
// 	fnAPP_SetDataField_Item_058X(nTranCode, TX_SEND_F); //IC카드 암호화 비밀번호 
	
	fnAPP_SetDataField_Item_059X(nTranCode, TX_SEND_F); //출금가능 금액(수표)
	fnAPP_SetDataField_Item_061X(nTranCode, TX_SEND_F); //권종별 매수정보	
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
 	fnAPP_SetDataField_Item_092X(nTranCode, TX_SEND_F); //단말기 프로그램의 버전(6자리숫자)


 	fn_FTX_EMVSendMake_Lib01(nTranCode);
 	fnAPP_SetDataField_Item_470X(nTranCode, TX_SEND_F); //신용카드IC관련정보
 	fnAPP_SetDataField_Item_471X(nTranCode, TX_SEND_F); //ARPC


	if (MenuAvail & MENU_MCU2)
	{
		if((m_pDevCmn->fnSCR_GetCurrentLangMode() == FOR_MODE) && (TranCode == TC_FOREGIN))
		{
			fnAPP_SetDataField_Item_4721(nTranCode, TX_SEND_F); //해외신용
			fnAPP_SetDataField_Item_4751(nTranCode, TX_SEND_F); //해외신용
		}
	}


	fnAPP_TxECash_SetDataField_SendProc();	
		

	return T_OK;	
}


/**************************************************************************************************************************************************************/
//이후 정리 하세요. (2016.03.08) - 전문정리 Kim.Gi.Jin [정리필요반듯이]
/**************************************************************************************************************************************************************/
// 
// int	CTranCmn::fn_FTX_TRANID_4210(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 
// 
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 	fnAPP_SetDataField_Item_054X(nTranCode, TX_SEND_F); //암호화 블럭데이터 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_345X(nTranCode, TX_SEND_F); //수수료 포인트 결제 여부 (외환)  #N0174
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //통장증서발행번호(회차)
// 	}
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 
// 	return T_OK;	
// }   
// 
// //무매체 출금
// int	CTranCmn::fn_FTX_TRANID_4116(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //주민번호
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_345X(nTranCode, TX_SEND_F); //수수료 포인트 결제 여부 (외환)  #N0174
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
// 	
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_345X(nTranCode, TX_SEND_F); //수수료 포인트 결제 여부 (외환)  #N0174
// 	
// 	if(m_pProfile->DEVICE.ETCDevice20 == HANA)
// 		fnAPP_SetDataField_Item_1721(nTranCode, TX_SEND_F); //하나은행 취급 수수료 #N0214
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //통장증서발행번호(회차)
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //주민번호
// 	
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_345X(nTranCode, TX_SEND_F); //수수료 포인트 결제 여부 (외환)  #N0174
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == HANA)
// 		fnAPP_SetDataField_Item_1721(nTranCode, TX_SEND_F); //하나은행 취급 수수료 #N0214
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }              
// int	CTranCmn::fn_FTX_TRANID_4393(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_345X(nTranCode, TX_SEND_F); //수수료 포인트 결제 여부 (외환)  #N0174
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == HANA)
// 		fnAPP_SetDataField_Item_1721(nTranCode, TX_SEND_F); //하나은행 취급 수수료 #N0214
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// 
//    
// int	CTranCmn::fn_FTX_TRANID_4720(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드X
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0226
// 		fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 	
// 	//#0045
// 	fnAPP_SetDataField_Item_222X(nTranCode, TX_SEND_F); //전자거래고유번호
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //통장증서발행번호(회차)
// 	}
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// } 
//    
// int	CTranCmn::fn_FTX_TRANID_4716(int nTranCode, int nSec) //-> 무매체 입금
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //무매체 입금자명 -> //#0101 --> #0107 -> #0111 ** (공장출하) 
// 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //입금자연락처 -> //#0107 -> #0111 ** (공장출하)
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //주민번호
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1 
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0226
// 		fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	//#0045
// 	fnAPP_SetDataField_Item_222X(nTranCode, TX_SEND_F); //전자거래고유번호
// 	
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }        
// 

// int	CTranCmn::fn_FTX_TRANID_7101(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// //	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// //	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// //	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// //	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //통장증서발행번호(회차)
// 	}
// 
// //	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// //	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();
// 
// 	return T_OK;	
// } 
// 
// //무매체 수수료조회
// int	CTranCmn::fn_FTX_TRANID_7106(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// //	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// //	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// //	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// //	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //주민번호
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// //	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// //	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }        
//        
// int	CTranCmn::fn_FTX_TRANID_7103(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// //	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// //	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
// 	
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //통장증서발행번호(회차)
// 	}
// 	
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }  
//   
// int	CTranCmn::fn_FTX_TRANID_7236(int nTranCode, int nSec) //무매체 가능 조회 
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == HANA)	
// 		fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //무매체 입금자명 -> //#0101 -> 불필요  = #N0214 하나 브랜드
// 	
// 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //입금자연락처 -> //#0107 -> #0111 ** (공장출하)
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //주민번호
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// //	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// //	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0239
// 	{
// 		fnAPP_SetDataField_Item_087X(nTranCode, TX_SEND_F); //최초 거래 여부
// 	}
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //통장증서발행번호(회차)
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// //	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// //	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //주민번호
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }
//    
// 
// int	CTranCmn::fn_FTX_TRANID_4220(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// //	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// //	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// //	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// //	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// //	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0282
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //통장증서발행번호(회차)
// 	}
// 
// //	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// //	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// //	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// //	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// //	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
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
// //#GIRO 일반지로 납부전조회 MS
// int	CTranCmn::fn_FTX_TRANID_7320(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7321(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;
// }        
// int	CTranCmn::fn_FTX_TRANID_7322(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 	
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// 	
// }   
// 
// //#GIRO 일반지로 납부전 조회 IC     
// int	CTranCmn::fn_FTX_TRANID_7323(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7324(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;
// }        
// int	CTranCmn::fn_FTX_TRANID_7325(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 	
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7326(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 	
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_7327(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 	
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// 
// int	CTranCmn::fn_FTX_TRANID_7328(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// 
// int	CTranCmn::fn_FTX_TRANID_7329(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	//#SJ 1106
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	//fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	//fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	//fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	//#SJ 1106
// 	//fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // 지로납부내역
// 	//fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 	//fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 	//fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }
// int	CTranCmn::fn_FTX_TRANID_7330(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// //	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 	
// 	//#0045 -> 기업 해당 사항 없음
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
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
// //#N0171 해외카드 BC 출금                                    
// int	CTranCmn::fn_FTX_TRANID_4316(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_346X(nTranCode, TX_SEND_F); //해외카드 브랜드 구분           //#N0171
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// } 
// 
// //#N0171 해외카드 BC 출금 취소     
// int	CTranCmn::fn_FTX_TRANID_4325(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// //	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST일련번호
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 
// 	fnAPP_SetDataField_Item_346X(nTranCode, TX_SEND_F); //해외카드 브랜드 구분           //#N0171
// 	
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// /*
// 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// */
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 
// 	return T_OK;	
// }
//                                    
// //#N0171 해외카드 SAVING                                    
// int	CTranCmn::fn_FTX_TRANID_4311(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //대출이자율             //#N0161-1
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// } 
// 
// //#N0171  해외카드 CHECKING     
// int	CTranCmn::fn_FTX_TRANID_4312(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //대출이자율             //#N0161-1
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// //	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST일련번호
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// /*
// 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// */
// 	fnAPP_TxECash_SetDataField_SendProc();
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4322(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// //	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST일련번호
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// /*
// 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
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
// //#N0171 해외카드 CREIDT        
// int	CTranCmn::fn_FTX_TRANID_4331(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //대출이자율             //#N0161-1
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	//fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// //	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //대출이자율             //#N0161-1
// 
// 	fnAPP_SetDataField_Item_346X(nTranCode, TX_SEND_F); //해외카드 브랜드 구분           //#N0171
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// //	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST일련번호
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// /*
// 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// */
// 	fnAPP_TxECash_SetDataField_SendProc();
// 
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4342(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// //	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST일련번호
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// /*
// 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
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
// int	CTranCmn::fn_FTX_TRANID_4670(int nTranCode, int nSec)		//#N0266 하이패스 충전 요청
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST일련번호
// 	fnAPP_SetDataField_Item_068X(nTranCode, TX_SEND_F); //전자화폐 일련번호
// 	fnAPP_SetDataField_Item_0691(nTranCode, TX_SEND_F); //충전전 잔액
// // 	fnAPP_SetDataField_Item_070X(nTranCode, TX_SEND_F); //전자화폐 생성난수
// // 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //전자화폐 인증자
// // 	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //전자화폐 카드종류
// // 	fnAPP_SetDataField_Item_073X(nTranCode, TX_SEND_F); //전자화폐 알고리즘ID
// // 	fnAPP_SetDataField_Item_074X(nTranCode, TX_SEND_F); //전자화폐 키셋버전
// // 	fnAPP_SetDataField_Item_075X(nTranCode, TX_SEND_F); //전자화폐 센터 ID
// // 	fnAPP_SetDataField_Item_076X(nTranCode, TX_SEND_F); //전자화폐 발행기관ID
// // 	fnAPP_SetDataField_Item_077X(nTranCode, TX_SEND_F); //전자화폐 이용자카드ID
// // 	fnAPP_SetDataField_Item_078X(nTranCode, TX_SEND_F); //전자화폐 PAN
// // 	fnAPP_SetDataField_Item_079X(nTranCode, TX_SEND_F); //전자화폐 발행자ID
// // 	fnAPP_SetDataField_Item_080X(nTranCode, TX_SEND_F); //승인 신용카드 번호
// // 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //전화번호
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_310X(nTranCode, TX_SEND_F); //충전 종류
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4671(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// // 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// // 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST일련번호
// // 	fnAPP_SetDataField_Item_068X(nTranCode, TX_SEND_F); //전자화폐 일련번호
// // 	fnAPP_SetDataField_Item_0691(nTranCode, TX_SEND_F); //충전전 잔액
// // 	fnAPP_SetDataField_Item_070X(nTranCode, TX_SEND_F); //전자화폐 생성난수
// // 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //전자화폐 인증자
// // 	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //전자화폐 카드종류
// // 	fnAPP_SetDataField_Item_073X(nTranCode, TX_SEND_F); //전자화폐 알고리즘ID
// // 	fnAPP_SetDataField_Item_074X(nTranCode, TX_SEND_F); //전자화폐 키셋버전
// // 	fnAPP_SetDataField_Item_075X(nTranCode, TX_SEND_F); //전자화폐 센터 ID
// // 	fnAPP_SetDataField_Item_076X(nTranCode, TX_SEND_F); //전자화폐 발행기관ID
// // 	fnAPP_SetDataField_Item_077X(nTranCode, TX_SEND_F); //전자화폐 이용자카드ID
// // 	fnAPP_SetDataField_Item_078X(nTranCode, TX_SEND_F); //전자화폐 PAN
// // 	fnAPP_SetDataField_Item_079X(nTranCode, TX_SEND_F); //전자화폐 발행자ID
// // 	fnAPP_SetDataField_Item_080X(nTranCode, TX_SEND_F); //승인 신용카드 번호
// // 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //전화번호
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_310X(nTranCode, TX_SEND_F); //충전 종류
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4680(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST일련번호
// 
// // 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //HOST일련번호
// // 	fnAPP_SetDataField_Item_068X(nTranCode, TX_SEND_F); //전자화폐 일련번호
// // 	fnAPP_SetDataField_Item_0691(nTranCode, TX_SEND_F); //충전전 잔액
// // 	fnAPP_SetDataField_Item_070X(nTranCode, TX_SEND_F); //전자화폐 생성난수
// // 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //전자화폐 인증자
// // 	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //전자화폐 카드종류
// // 	fnAPP_SetDataField_Item_073X(nTranCode, TX_SEND_F); //전자화폐 알고리즘ID
// // 	fnAPP_SetDataField_Item_074X(nTranCode, TX_SEND_F); //전자화폐 키셋버전
// // 	fnAPP_SetDataField_Item_075X(nTranCode, TX_SEND_F); //전자화폐 센터 ID
// // 	fnAPP_SetDataField_Item_076X(nTranCode, TX_SEND_F); //전자화폐 발행기관ID
// // 	fnAPP_SetDataField_Item_077X(nTranCode, TX_SEND_F); //전자화폐 이용자카드ID
// // 	fnAPP_SetDataField_Item_078X(nTranCode, TX_SEND_F); //전자화폐 PAN
// // 	fnAPP_SetDataField_Item_079X(nTranCode, TX_SEND_F); //전자화폐 발행자ID
// // 	fnAPP_SetDataField_Item_080X(nTranCode, TX_SEND_F); //승인 신용카드 번호
// // 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //전화번호
// // 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_310X(nTranCode, TX_SEND_F); //충전 종류
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST일련번호
// 
// // 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //HOST일련번호
// // 	fnAPP_SetDataField_Item_068X(nTranCode, TX_SEND_F); //전자화폐 일련번호
// // 	fnAPP_SetDataField_Item_0691(nTranCode, TX_SEND_F); //충전전 잔액
// // 	fnAPP_SetDataField_Item_070X(nTranCode, TX_SEND_F); //전자화폐 생성난수
// // 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //전자화폐 인증자
// // 	fnAPP_SetDataField_Item_072X(nTranCode, TX_SEND_F); //전자화폐 카드종류
// // 	fnAPP_SetDataField_Item_073X(nTranCode, TX_SEND_F); //전자화폐 알고리즘ID
// // 	fnAPP_SetDataField_Item_074X(nTranCode, TX_SEND_F); //전자화폐 키셋버전
// // 	fnAPP_SetDataField_Item_075X(nTranCode, TX_SEND_F); //전자화폐 센터 ID
// // 	fnAPP_SetDataField_Item_076X(nTranCode, TX_SEND_F); //전자화폐 발행기관ID
// // 	fnAPP_SetDataField_Item_077X(nTranCode, TX_SEND_F); //전자화폐 이용자카드ID
// // 	fnAPP_SetDataField_Item_078X(nTranCode, TX_SEND_F); //전자화폐 PAN
// // 	fnAPP_SetDataField_Item_079X(nTranCode, TX_SEND_F); //전자화폐 발행자ID
// // 	fnAPP_SetDataField_Item_080X(nTranCode, TX_SEND_F); //승인 신용카드 번호
// // 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //전화번호
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //거래금액
// 	
// 	fnAPP_SetDataField_Item_310X(nTranCode, TX_SEND_F); //충전 종류
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// //	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	//#0045 -> 기업 해당 사항 없음
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0227
// 	{
// 		fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 	}
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM) //#N0277
// 	{
// 		fnAPP_SetDataField_Item_387X(nTranCode, TX_SEND_F); //통장증서발행번호(회차)
// 	}
// 	
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;	
// }      
// 
//  
//  //#GIRO 일반지로 납부 MS 
// int	CTranCmn::fn_FTX_TRANID_4810(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;
// }        
// int	CTranCmn::fn_FTX_TRANID_4811(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// //	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //계좌성명
// 	
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4812(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;	
// }
// 
// //#GIRO 일반지로 납부 IC        
// int	CTranCmn::fn_FTX_TRANID_4813(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_193X(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 	return T_OK;
// }        
// int	CTranCmn::fn_FTX_TRANID_4814(int nTranCode, int nSec)
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// 	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// //	fnAPP_SetDataField_Item_191X(nTranCode, TX_SEND_F);  // 지로번호
// 	fnAPP_SetDataField_Item_192X(nTranCode, TX_SEND_F);  // 전자납부번호
// 	fnAPP_SetDataField_Item_1931(nTranCode, TX_SEND_F);  // 지로납부내역
// 	fnAPP_SetDataField_Item_194X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_196X(nTranCode, TX_SEND_F);  // 이용기관명(거래점 번호)
// 
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_018X(nTranCode, TX_SEND_F); //계좌성명
// 	
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
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
// int	CTranCmn::fn_FTX_TRANID_4840(int nTranCode, int nSec) //#N0266 하이패스 충전 요청
// {
// 
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// //	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST일련번호
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //거래금액
// 
// 
// 	fnAPP_TxECash_SetDataField_SendProc();
// 	return T_OK;	
// }        
// int	CTranCmn::fn_FTX_TRANID_4850(int nTranCode, int nSec)  //#N0266 하이패스 충전 요청
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// //	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //HOST일련번호
// 
// 	fnAPP_SetDataField_Item_220X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_221X(nTranCode, TX_SEND_F); //거래금액
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_086X(nTranCode, TX_SEND_F); //입금자연락처 -> //#0107 -> #0111 ** (공장출하)
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
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
// //#N0171 해외카드 수수료 조회                    
// int	CTranCmn::fn_FTX_TRANID_7104(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_TxECash_SetDataField_SendProc();	
// 		
// 
// 	return T_OK;	
// }  
// 
// 
// //#N0171 해외카드 수수료 조회                    
// int	CTranCmn::fn_FTX_TRANID_7105(int nTranCode, int nSec)
// {
// 	fnAPP_ClearArrDataField(TX_SEND_F);
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_346X(nTranCode, TX_SEND_F); //해외카드 브랜드 구분           //#N0171
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //대출이자율             //#N0161-1
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // 	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // 	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_346X(nTranCode, TX_SEND_F); //해외카드 브랜드 구분           //#N0171
// 
// //	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// //	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //대출이자율             //#N0161-1
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// // 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //대출이자율             //#N0161-1
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //대출이자율             //#N0161-1
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// // //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// // //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //대출이자율             //#N0161-1
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// 	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// 	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// 	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// 	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// 	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// 	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// 	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 
// // 	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// // 	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// // 	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// 	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// 	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
// 
// 	fnAPP_SetDataField_Item_168X(nTranCode, TX_SEND_F); //대출이자율             //#N0161-1
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
// 	//공통부
// 	fnAPP_SetDataField_Item_001X(nTranCode, TX_SEND_F); //날짜
// 	fnAPP_SetDataField_Item_002X(nTranCode, TX_SEND_F); //시각
// 	fnAPP_SetDataField_Item_003X(nTranCode, TX_SEND_F); //일련번호
// 	fnAPP_SetDataField_Item_005X(nTranCode, TX_SEND_F); //전문종류
// 	fnAPP_SetDataField_Item_006X(nTranCode, TX_SEND_F); //거래구분 - TrID
// 	fnAPP_SetDataField_Item_071X(nTranCode, TX_SEND_F); //거래매체구분
// //	fnAPP_SetDataField_Item_008X(nTranCode, TX_SEND_F); //응답코드
// 
// 	TranEncCode = fnAPP_EncType(TranCode2);
// 	fnAPP_EncCall(TranEncCode, TranCode2);
// 	
// 	fnAPP_SetDataField_Item_011X(nTranCode, TX_SEND_F); //단말기번호
// 	fnAPP_SetDataField_Item_048X(nTranCode, TX_SEND_F); //ISO2정보의 신용카드 여부(1/0)
// 	fnAPP_SetDataField_Item_049X(nTranCode, TX_SEND_F); //기기종류
// 	fnAPP_SetDataField_Item_0120(nTranCode, TX_SEND_F); //취급관코드
// 
// 	fnAPP_SetDataField_Item_013X014X015X915X(nTranCode, TX_SEND_F);
// 
// //	fnAPP_SetDataField_Item_013X(nTranCode, TX_SEND_F); //Track 3 MS Data
// 	fnAPP_SetDataField_Item_0131(nTranCode, TX_SEND_F); //암호화 (기업,국민)
// //	fnAPP_SetDataField_Item_014X(nTranCode, TX_SEND_F); //Track 2 MS Data
// 	fnAPP_SetDataField_Item_015X(nTranCode, TX_SEND_F); //비밀번호
// //	fnAPP_SetDataField_Item_0151(nTranCode, TX_SEND_F); //IC Track
// 	fnAPP_SetDataField_Item_016X(nTranCode, TX_SEND_F); //취급기관코드
// 	fnAPP_SetDataField_Item_017X(nTranCode, TX_SEND_F); //거래계좌(카드)번호1
// //	fnAPP_SetDataField_Item_019X(nTranCode, TX_SEND_F); //수취기관코드
// 
// 
// //	fnAPP_SetDataField_Item_020X(nTranCode, TX_SEND_F); //수취계좌번호
// //	fnAPP_SetDataField_Item_021X(nTranCode, TX_SEND_F); //수취계좌성명
// //	fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 	fnAPP_SetDataField_Item_023X(nTranCode, TX_SEND_F); //거래금액
// //	fnAPP_SetDataField_Item_024X(nTranCode, TX_SEND_F); //거래수수료
// //	fnAPP_SetDataField_Item_025X(nTranCode, TX_SEND_F); //미결제타점권 금액
// 
// //	if(m_pProfile->DEVICE.ETCDevice20 == KB)		//#N0239
// //		fnAPP_SetDataField_Item_022X(nTranCode, TX_SEND_F); //거래계좌(카드)잔액
// 
// 	fnAPP_SetDataField_Item_063X(nTranCode, TX_SEND_F); //거래시작일시(카드삽입)
// 	fnAPP_SetDataField_Item_064X(nTranCode, TX_SEND_F); //수취계좌성명
// 	fnAPP_SetDataField_Item_066X(nTranCode, TX_SEND_F); //수표등록시간
// //	fnAPP_SetDataField_Item_067X(nTranCode, TX_SEND_F); //전자화폐 일련번호(IC카드일련번호)
// 
// 	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP || m_pProfile->DEVICE.ETCDevice20 == KWANGJU)	   //#N0181
// 		fnAPP_SetDataField_Item_091X(nTranCode, TX_SEND_F); //HOST작업 정보의 버전값
// 
// //	fnAPP_SetDataField_Item_127X(nTranCode, TX_SEND_F); //만원권 출금매수
// //	fnAPP_SetDataField_Item_128X(nTranCode, TX_SEND_F); //5만원권 출금매수
// //	fnAPP_SetDataField_Item_129X(nTranCode, TX_SEND_F); //10만원권 출금매수
// //	fnAPP_SetDataField_Item_189X(nTranCode, TX_SEND_F); //오천원권 매수
// //	fnAPP_SetDataField_Item_190X(nTranCode, TX_SEND_F); //천원권 매수
// 
// //	fnAPP_SetDataField_Item_915X(nTranCode, TX_SEND_F); //비밀번호(4자리)
// //	fnAPP_SetDataField_Item_9151(nTranCode, TX_SEND_F); //비밀번호(농협/하나/삼성증/광주)
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
		if (EMVReadFlg)												// EMV 송신데이터 편집
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
					CardPowerOnFlg = FALSE;						// 카드접속무
					FallBackEMVMode = TRUE;
					
					if (!(CardTran & MENU_MCU2))
					{
						fnAPP_CancelProc(T_MSG, "EMV송신전문 작성실패");
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
		case nEnCmd181_105:    //하나SK카드
		case nEnCmd111_111:    //농협카드
		case nEnCmd126_192:    //신한카드
		case nEnCmd194_194:    //BC 카드
		case nEnCmd107_107:    //수협카드	
		case nEnCmd120_120:    //우리 카드
//		case nEnCmd127_127:    //씨티 -> 예외처리
			//1. Terminal Capabilitise(9F33)(3)
			//2. Terminal Verification Result(95)(5)
			//3. Unpredictable Number(9F37)(4)
			//4. Application Cryptogram(9F26)(8)
			//5. Application Transaction Counter(9F36)(2)
			//6. Application Interchange Profile(82)(2)
			//7. Transaction Type(9C)(1)
			//8. Terminal Country Code(9F1A)(2)                  --> 사용안함 (협의필요)
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
																									//8. Terminal Country Code(9F1A)(2)                  --> 사용안함 (협의필요)
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
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData FI = [%16.16s]", &m_pDevCmn->m_EMVszData[nOffsetCvtPos]); /*에러처리*/	
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData BC = [%16.16s]", &szBcCvtFormat[nOffset]); /*에러처리*/	
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
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData FI = [%19.19s]", &m_pDevCmn->m_EMVszData[nOffsetCvtPos]); /*에러처리*/	
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData BC = [%19.19s]", &szBcCvtFormat[nOffset]); /*에러처리*/	

			nOffset += 19;
			nOffsetCvtPos += 19;
	

			//19. Issuer Application Data(9F10)(Variables) (2) -> 20. Length Indicator[2]  	
			memcpy(&szBcCvtFormat[nOffset], &m_pDevCmn->m_EMVszData[nOffsetCvtPos], 2);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData FI = [%2.2s]", &m_pDevCmn->m_EMVszData[nOffsetCvtPos]); /*에러처리*/	
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData BC = [%2.2s]", &szBcCvtFormat[nOffset]); /*에러처리*/	

			nOffset += 2;	
			nOffsetCvtPos += 2;


			nOffsetCvtPos += 2;
			nOffsetCvtPos += 2;
			nOffsetCvtPos += 12;
			//23. Issuer Discretionary Data[50] 
			memcpy(&szBcCvtFormat[nOffset], &m_pDevCmn->m_EMVszData[nOffsetCvtPos], 50);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData FI = [%50.50s]", &m_pDevCmn->m_EMVszData[nOffsetCvtPos]); /*에러처리*/	
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData BC = [%50.50s]", &szBcCvtFormat[nOffset]); /*에러처리*/	

			nOffset += 50;			

			HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVszData FI", m_pDevCmn->m_EMVszData, 200);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData FI = [%192.192s]", m_pDevCmn->m_EMVszData); /*에러처리*/	
			memset(m_pDevCmn->m_EMVszData, ' ', sizeof(m_pDevCmn->m_EMVszData));				// EMV Send RecvData 버퍼
			memcpy(m_pDevCmn->m_EMVszData, szBcCvtFormat, nOffset);
			MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "EMVszData BC = [%192.192s]", m_pDevCmn->m_EMVszData); /*에러처리*/	
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
			//8. Terminal Country Code(9F1A)(2)                  --> 사용안함 (협의필요)
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
// 	/*	[IC CARD인 경우 IC 신용카드 정보] -> Kim.Gi.Jin -> 내부검사 소스 1차 완료
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

