//******************************************************************************
// File Name         : DevEMV.cpp
// Description       : IC Card APDU interface class
// Author            : Han Su Woong
// Initial Date      : 2004.07.01
// Revision History  : 
// Notes             : (C)Nautilus Hyosung Inc. 2004 All Rights Reserved.
//******************************************************************************


#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevICCard.h"
#include "..\..\H\Dll\DevCmn.h"

#include "..\..\H\EMVInc\\AppErrCode.h"	
#include "..\..\H\EMVInc\\SCSp_ATM_EMV.h"

#include "..\..\H\Tran\TranCmn.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////
//  Main Operation Function
////////////////////////////////////////////////////////////////////////////////

/*==============================================================================
* Function Name : fnEMV_Initialize()
* Description   : EMV 관련 Buffer를 초기화하는 함수(기기 Init시 1회 호출)
* Parameters    : (None)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_Initialize()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Initialize");

	memset(         &EMVInfo, NULL,         sizeof(EMVInfo) );
	memset( &EMVCAPKListInfo, NULL, sizeof(EMVCAPKListInfo) );
	memset(  &EMVDOLListInfo, NULL,  sizeof(EMVDOLListInfo) );
	memset(  &EMVAIDListInfo, NULL,  sizeof(EMVAIDListInfo) );


	memset(m_EMVszData, ' ', sizeof(m_EMVszData));				// EMV Send RecvData 버퍼
	memcpy(m_szEMVFBCode, EMV_FBCODE_NOATR,  _tcslen(EMV_FBCODE_NOATR));

	return TRUE;
}

/*==============================================================================
* Function Name : fnEMV_TerminalConstSet()
* Description   : EMV 거래가능한 단말기의 객체데이터 구성 함수
* Parameters    : (NONE)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2015.01.19
* ============================================================================*/

int CDevCmn::fnEMV_TerminalConstSet(LPBYTE m_Data, BYTE Len)		//#N0268
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_TerminalConstSet()");
	
	int		nOffset = 0;
	int		nReturn(FALSE);
	CString strICSendData("");
	
	memset( ICSendData, 0, sizeof(ICSendData) );	// IC전송자료
	memcpy(ICSendData, m_Data, Len);

	strICSendData = MakeUnPack( ICSendData, Len);
	nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );
	

MsgDump(TRACE_CODE_MODE, "EMV", __FILE__, __LINE__, "fnEMV_TerminalConstSet(...):return[%d]", nReturn);

	return nReturn;
}
// V09-47-00-#07 FB율 개선 End


/*==============================================================================
* Function Name : fnEMV_SelectPSE()
* Description   : EMV 거래 가능 여부를 PSE를 통해 확인하는 함수
* Parameters    : (NONE)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.01 Han Su Woong
* ============================================================================*/

int CDevCmn::fnEMV_SelectPSE()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelectPSE()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV SelectPSE");

	int		nReturn(FALSE);
	CString strICSendData("");

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_EMV;						// 0x04
	ICSendData[2] = IC_SFC_SELECTPSE;				// 0x04

	strICSendData = MakeUnPack( ICSendData, IC_FCMS_LEN );
	nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelectPSE(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnEMV_ReqCandidateList()
* Description   : 카드로부터 EMV 거래가 가능한 후보 목록을 요청하는 함수
* Parameters    : (NONE)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.01 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_ReqCandidateList()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_ReqCandidateList()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Request Candidate List");

	int		nReturn(FALSE);
	int		nSendLen, nDataLen;
	CString strICSendData("");
	BYTE	bData[IC_DATA_LEN];

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC 전송자료
	memset(      bData, 0,      sizeof(bData) );	// Parameter

	// Data Constructuon
	nDataLen = fnEMV_MakeReqCandidate(bData);
//	if ( nDataLen == T_ERROR )
//	{
//		// 데이터 생성 중 에러인 경우 select 실패로 폴백 처리
//		memcpy(m_szEMVFBCode, EMV_FBCODE_SELECTERROR, _tcslen(EMV_FBCODE_SELECTERROR));	// kh 20041015
//		return T_ERROR;
//	}

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_EMV;						// 0x04
	ICSendData[2] = IC_SFC_REQCANDIDATELIST;		// 0x10
	nSendLen = IC_FCMS_LEN;

	ICSendData[nSendLen] = (BYTE)nDataLen;			// Length of Data
	nSendLen++;

	memcpy( &ICSendData[nSendLen], bData, nDataLen );
	nSendLen += nDataLen;

	strICSendData = MakeUnPack( ICSendData, nSendLen );
	nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_ReqCandidateList(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnEMV_SelectApplication()
* Description   : 후보 목록 중 선택된 Application으로 거래를 시작하는 함수
* Parameters    : Index : 선택된 Application의 Index. 1부터 시작함을 주의.
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.01 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_SelectApplication(BYTE Index)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelectApplication()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Select Application");

	int		nReturn(FALSE);
	int		nSendLen;
	CString strICSendData("");

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC 전송자료

	EMVInfo.TranIndex = Index - 1;				// 현재 선택된 Index 저장

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_EMV;						// 0x04
	ICSendData[2] = IC_SFC_SELECTAPPEMV;			// 0x11
	nSendLen = IC_FCMS_LEN;

	// Length of AID
	if(EMVInfo.TranIndex >= 0)					
		ICSendData[nSendLen] = (BYTE)EMVCandidateList[EMVInfo.TranIndex].AIDLen;
	nSendLen++;

	// AID
	if(EMVInfo.TranIndex >= 0)						
	{
		memcpy( &ICSendData[nSendLen],
				EMVCandidateList[EMVInfo.TranIndex].AID,
				EMVCandidateList[EMVInfo.TranIndex].AIDLen );
		nSendLen += EMVCandidateList[EMVInfo.TranIndex].AIDLen;

	}	
	strICSendData = MakeUnPack( ICSendData, nSendLen );
	nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );
HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "EMVCandidateList[EMVInfo.TranIndex].AID", EMVCandidateList[EMVInfo.TranIndex].AID, sizeof(EMVCandidateList[EMVInfo.TranIndex].AID));
HexDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVCandidateList[EMVInfo.TranIndex].AID", EMVCandidateList[EMVInfo.TranIndex].AID, sizeof(EMVCandidateList[EMVInfo.TranIndex].AID));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_SelectApplication(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnEMV_ReadApplication()
* Description   : 거래를 위해 Application의 정보를 수집하는 함수
* Parameters    : (NONE)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.01 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_ReadApplication()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_ReadApplication()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Read Application");

	int		nReturn(FALSE);
	CString strICSendData("");

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_EMV;						// 0x04
	ICSendData[2] = IC_SFC_READAPPEMV;				// 0x12

	strICSendData = MakeUnPack( ICSendData, IC_FCMS_LEN );
	nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_ReadApplication(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnEMV_OfflineDataAuthentication()
* Description   : 오프라인 인증을 요구하는 함수. EP 단에서는 이후 위험 관리, CVM
*                 처리, 조치 분석, 1'st Gernate AC Command 등을 계속적으로 수행한다.
* Parameters    : szAmount         : 입력받은 금액(Ex : "0000010000")
*                 AmountLen        : Length of szAmount
*                 szAnotherAmount  : 입력받은 기타금액(로열티 등)(Ex : "0000010000")
*                 AnotherAmountLen : Length of szAnotherAmount
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.01 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_OfflineDataAuthentication(char* szAmount, int AmountLen,
								char* szAnotherAmount, int AnotherAmountLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_OfflineDataAuthentication()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Offline Data Authentication Application");

	int		nReturn(FALSE);
	int		nSendLen, nDataLen;
	CString strICSendData("");
	BYTE	bData[IC_DATA_LEN];

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC 전송자료
	memset(      bData, 0,      sizeof(bData) );	// Parameter

	// Data Constructuon
	nDataLen = fnEMV_MakeOfflineDataAuthen( bData, szAmount, AmountLen,
									szAnotherAmount, AnotherAmountLen );
//	if ( nDataLen == T_ERROR )
//	{
//		// 데이터 생성 중 에러인 경우 select 실패로 폴백 처리
//		memcpy(m_szEMVFBCode, EMV_FBCODE_BEFORE_OFFAUTHERR, _tcslen(EMV_FBCODE_BEFORE_OFFAUTHERR));	// kh 20041015
//		return T_ERROR;
//	}

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_EMV;						// 0x04
	ICSendData[2] = IC_SFC_OFFDATAAUTHEN;			// 0x13
	nSendLen = IC_FCMS_LEN;

	ICSendData[nSendLen] = (BYTE)nDataLen;			// Length of Data
	nSendLen++;

	memcpy( &ICSendData[nSendLen], bData, nDataLen );
	nSendLen += nDataLen;

	strICSendData = MakeUnPack( ICSendData, nSendLen );
	nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_OfflineDataAuthentication(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnEMV_OnlineProcessing()
* Description   : 온라인 승인 요청에 대한 호스트 응답을 카드로 전달하는 함수
* Parameters    : lpszHostData : Host Response Data
*                 nHostResp    : 거래에 대한 Host의 성공, 실패 응답
*                 HostRecvFlag : Host 응답을 받았는지 송수신 에러인지를 판단.
*                                Host 응답을 받았지만 ARC가 없는 경우에는
*                                nHostResp를 통해 ARC를 만들어준다.
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_OnlineProcessing(LPSTR lpszHostData, int nHostResp, BOOL HostRecvFlag, int Certificate)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_OnlineProcessing()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Online Processing");

	int		nReturn(FALSE);
	int		nSendLen, nDataLen;
	CString strICSendData("");
	BYTE	bData[IC_DATA_LEN];

MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "fnEMV_OnlineProcessing() lpszHostData = %s", lpszHostData);
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "fnEMV_OnlineProcessing() nHostResp = %d", nHostResp);
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "fnEMV_OnlineProcessing() HostRecvFlag = %d", HostRecvFlag);

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC 전송자료
	memset(      bData, 0,      sizeof(bData) );	// Parameter

	HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeOnlineProcessing() lpszHostData", lpszHostData, 300);
	// Data Constructuon
	nDataLen = fnEMV_MakeOnlineProcessing( bData, lpszHostData, nHostResp, HostRecvFlag );
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "fnEMV_OnlineProcessing() RecvLen = %d, RecvData=[%100.100s]", nDataLen, bData);

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_EMV;						// 0x04
	ICSendData[2] = IC_SFC_ONLINEPROCESS;			// 0x14
	nSendLen = IC_FCMS_LEN;

	memcpy( &ICSendData[nSendLen], bData, nDataLen );
	nSendLen += nDataLen;

	strICSendData = MakeUnPack( ICSendData, nSendLen );
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "fnEMV_OnlineProcessing() strICSendData=[%s]", strICSendData);
HexDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "fnEMV_OnlineProcessing() strICSendData", strICSendData.GetBuffer(0), 500);
	nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_OnlineProcessing(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnEMV_CompletionProcessing()
* Description   : 카드로 거래 종료를 요청하는 함수
* Parameters    : (NONE)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_CompletionProcessing()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_CompletionProcessing()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Completion Processing");

	int		nReturn(FALSE);
	CString strICSendData("");

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_EMV;						// 0x04
	ICSendData[2] = IC_SFC_COMPLETPROCESS;			// 0x15

	strICSendData = MakeUnPack( ICSendData, IC_FCMS_LEN );
	nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_CompletionProcessing(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnEMV_AfterScriptProcessing()
* Description   : 거래를 종료하기 위해 스크립트 처리(After Script)를 요청하는 함수
* Parameters    : (NONE)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_AfterScriptProcessing()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AfterScriptProcessing()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV After Script Processing");

	int		nReturn(FALSE);
	CString strICSendData("");

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_EMV;						// 0x04
	ICSendData[2] = IC_SFC_AFTERSCRIPT;				// 0x16

	strICSendData = MakeUnPack( ICSendData, IC_FCMS_LEN );
	nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AfterScriptProcessing(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnEMV_GetDOLData()
* Description   : AP가 거래 중 필요한 데이터 객체를 EMV Kernel에 요청하는 함수.
*                 이 명령은 실제로 카드로 보내는 명령은 아니다.
* Parameters    : lpTag   : 정보를 얻어올 객체의 Tag + 예상 리턴 길이
*                 nTagLen : Length of lpTag
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_GetDOLData(LPBYTE lpTag, int nTagLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_GetDOLData()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Get DOL Data");

	int		nReturn(FALSE);
	CString strICSendData("");
	int		nSendLen;

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_EMV;						// 0x04
	ICSendData[2] = IC_SFC_GETDOLDATA;				// 0x20
	nSendLen = IC_FCMS_LEN;

	// DOL Data
	memcpy( &ICSendData[nSendLen], lpTag, nTagLen );
	nSendLen += nTagLen;

	strICSendData = MakeUnPack( ICSendData, nSendLen );
	nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_GetDOLData(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnEMV_ResultData()
* Description   : 거래 종료 후 화면으로 보여줄 EMV 거래 결과 변수들을 요청하는
*                 함수(인증용 & 디버깅용)
* Parameters    : (None)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_ResultData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_ResultData()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Result Data");

	int		nReturn(FALSE);
	CString strICSendData("");

	memset( ICSendData, 0, sizeof(ICSendData) );	// IC전송자료

	// Message 조립
	ICSendData[0] = IC_FC_MAIN;						// 0x30
	ICSendData[1] = IC_MFC_EMV;						// 0x04
	ICSendData[2] = IC_SFC_EMVRESULTDATA;			// 0x21

	strICSendData = MakeUnPack( ICSendData, IC_FCMS_LEN );
	nReturn = fnMCU_ICSendData( TRAN_IC_PROTOCOL_T1, strICSendData );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_ResultData(...):return[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnEMV_AnalRecvData()
* Description   : EMV IC Chip Receive Data를 해석하고 버퍼에 저장하는 함수
* Parameters    : nCmdType : 분석할 응답 메시지의 종류
                  +-----------------------+------------------------------------+
				  | 7816_CHECK            | ISO 7816 표준 에러 Check           |
				  | CASHIC_PROC_XXXX      | 각 거래 Flow에 따른 처리           |
				  +-----------------------+------------------------------------+
* Return        : Success : T_OK				Fail : T_ERROR
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_AnalRecvData(int nCmdType)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData(CmdType[%d])", nCmdType);
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Receive Data");

	int		nReturn(T_ERROR);		// 초기값 T_ERROR로 설정
									// Case문 내의 조건과 맞지 않으면 초기값으로 결정
	CString	strICRecvData("");
	int		nRecvLen;

	nEmvStep = nCmdType;

	memset( ICRecvData, 0, sizeof(ICRecvData) );
	strICRecvData = fstrMCU_GetICData();
	nRecvLen = __min( sizeof(ICRecvData) * 2, strICRecvData.GetLength() );
	MakePack( strICRecvData.GetBuffer(0), ICRecvData, nRecvLen );
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData(ICRecvData[%s])", ICRecvData);

	if ( fnICA_CheckFCCode() != TRUE )
	{
		fnICA_SetFCError(ICRecvData);
		fnICA_AnalyzeFCError();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData(...) FCError Return!!!!");
		return nReturn;
	}

	int		nOffset(0), nLength(0), nCount(0);
	int		i, j;

	char	szLog[200];

	nOffset  += IC_FCMS_LEN;						// 업무 코드
	nRecvLen /= 2;									// Pack Data Len

	nEmvStepCode = Asc2Int(&ICRecvData[nOffset],1);
	switch (nCmdType)
	{
	case EMV_PROC_CALISTSET :						// Terminal List Set
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_CALISTSET");
		nReturn = T_OK;								// 현재 특별한 응답 없음
		break;

	case EMV_PROC_TERMINALLISTSET :					// Terminal List Set
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_TERMINALLISTSET");
		nReturn = T_OK;								// 현재 특별한 응답 없음
		break;

	case EMV_PROC_TERMINALCONSTSET :				// Terminal Constant Set
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_TERMINALCONSTSET");

		// 0x31000000 : EMV 거래 가능(설정 모두 성공) -> Return = 0
		// 0x31000003 : EMV 초기값 설정 실패          -> Return = 5
		if ( ICRecvData[nOffset] == EMV_FLAG_SUCCESS )	// 초기값 설정 성공
		{
			nReturn = T_OK;
		}
		else if ( ICRecvData[nOffset] == EMV_FLAG_INITDATAERROR )	// 초기값 설정에 실패하였음
		{
			nReturn = EMV_RESULT_INITDATAERROR;		// EMV 거래 불가
		}

		break;

	case EMV_PROC_SELECTPSE :						// Select PSE
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_SELECTPSE");

		// 0x31000000 : EMV 거래 가능(PSE 존재)   -> Return = 1
		// 0x31000002 : EMV 거래 불가능(PSE 없음) -> Return = 2
		// 0x31000003 : EMV 초기값 설정 실패      -> Return = 5
		if ( ICRecvData[nOffset] == EMV_FLAG_SUCCESS )	// 거래 가능한 Application이 있음
		{
			nReturn = T_OK;
		}
		else if ( ICRecvData[nOffset] == EMV_FLAG_INITDATAERROR )	// 초기값 설정에 실패하였음
		{
			nReturn = EMV_RESULT_INITDATAERROR;		// EMV 거래 불가
		}
		else
		{
			nReturn = EMV_RESULT_TERMINATE;			// 거래 가능한 Application이 없음
		}

		break;

	case EMV_PROC_REQCANDIDATELIST :				// Request Candidate List
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_REQCANDIDATELIST");

		// Return Value : 목록 갯수 + 반복( API + AID Length + AID )
		EMVInfo.CandidateCount = 0;					// 목록 갯수
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData(1) : [%d]", ICRecvData[nOffset]);
		if ( ICRecvData[nOffset] == EMV_FLAG_SUCCESS )	// 거래 가능한 Application이 있음
		{
			nOffset++;

			nCount = ICRecvData[nOffset];
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData(2) : [%d]", ICRecvData[nOffset]);
			if ( nCount == NULL )
			{
				nReturn = EMV_RESULT_NOTSUPPORT;	// 거래 가능한 Application이 없음
				// 폴백 거래를 위한 처리, 카드 블럭 등의 사양은 연구소와 협의 후 차후 처리
				memcpy(m_szEMVFBCode, EMV_FBCODE_SELECTERROR, _tcslen(EMV_FBCODE_SELECTERROR));
				break;
			}
			nOffset++;
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData(3) : [%d]", ICRecvData[nOffset]);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_REQCANDIDATELIST : nCount = %d", nCount);

			// Display 구조체에 정보 Setting
			for ( i = 0; i < nCount; i++ )
			{
				// Application Priority Indicator
				EMVCandidateList[i].API = ICRecvData[nOffset];
				nOffset++;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData(4) : [%d]", ICRecvData[nOffset]);
				// AID Length
				EMVCandidateList[i].AIDLen = ICRecvData[nOffset];
				nOffset++;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData(5) : [%d]", ICRecvData[nOffset]);
				if ( EMVCandidateList[i].AIDLen == NULL )
				{
					memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
					fnICA_SetFCError(ICRecvData);
					fnICA_AnalyzeFCError();
					return nReturn;
				}

				// AID
				memcpy( EMVCandidateList[i].AID, ICRecvData + nOffset,
						EMVCandidateList[i].AIDLen );
				nOffset += EMVCandidateList[i].AIDLen;

				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData(6) : [%d]", ICRecvData[nOffset]);
				// Brand Name
				for ( j = 0; j < EMV_CANDIDATELIST_CNT; j++ )
				{

					HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVCandidateList[i].AID", EMVCandidateList[i].AID, sizeof(EMVCandidateList[i].AID));
					HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVAIDListInfo[i].AID", EMVAIDListInfo[j].AID, EMVAIDListInfo[j].AIDLen);
					if ( memcmp( EMVCandidateList[i].AID, EMVAIDListInfo[j].AID,
								 EMVAIDListInfo[j].AIDLen ) == 0 )
					{
						strcpy( EMVCandidateList[i].Brand,
								EMVAIDListInfo[j].Brand );
						EMVInfo.CandidateCount++;	// 목록 갯수 증가

#if EMV_TA_MODE		// 인증 모드
						// Visa Partial 부분 추가
						if ( EMVCandidateList[i].AIDLen > EMV_AID_LEN )
						{
							strcat( EMVCandidateList[i].Brand, "(Partial)" );
						}
#endif	// #if EMV_TA_MODE

						// Index
						wsprintf( EMVCandidateList[i].Index,
								  "%2d", EMVInfo.CandidateCount );

						nReturn = T_OK;
						break;
					}
				}
			}
		}
		else if ( ICRecvData[nOffset] == EMV_FLAG_INITDATAERROR )	// 초기값 설정에 실패하였음
		{
			// 초기값 설정 실패시는 폴백 거래 지원하지 않음
			memcpy(m_szEMVFBCode, EMV_FBCODE_NOATR, _tcslen(EMV_FBCODE_NOATR));	// kh 20041015 
			nReturn = EMV_RESULT_INITDATAERROR;		// EMV 거래 불가
		}
		else
		{
			// 폴백 처리로 전환, 카드 블럭 등은 연구소와 협의 후 처리
			memcpy(m_szEMVFBCode, EMV_FBCODE_SELECTERROR, _tcslen(EMV_FBCODE_SELECTERROR));	// kh 20041015
			nReturn = EMV_RESULT_NOTSUPPORT;		// 거래 가능한 Application이 없음
		}

		// 반환된 List는 단말기 목록 List에 의존하므로 실제 여길 타면 안된다.
		if ( nReturn == TRUE && EMVInfo.CandidateCount == 0 )
		{
			// 현 상황에서는 List에 없는 EMV이므로 폴백 거래 지원하지 않는다.
			memcpy(m_szEMVFBCode, EMV_FBCODE_NOATR, _tcslen(EMV_FBCODE_NOATR));	// kh 20041015
			nReturn = EMV_RESULT_NOTSUPPORT;		// 거래 가능한 Application이 없음
		}

		break;

	case EMV_PROC_SELECTAPPLICATION :				// EMV Select Application
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_SELECTAPPLICATION");

		// 선택된 Application이 실제 EMV 거래를 할 수 없는 경우
		// 이 경우 카드는 단말기로 0x6985를 반환하며 단말기는 이 목록을 제외한 목록을 다시 표시
		if ( ICRecvData[nOffset] == EMV_FLAG_SUCCESS )
		{
			nReturn = T_OK;
		}
		else if ( ICRecvData[nOffset] == EMV_FLAG_WARNING )
		{
			nOffset++;

			// Index에 해당하는 항목을 삭제하고 목록을 재정리
			for ( i = EMVInfo.TranIndex + 1 ; i < EMVInfo.CandidateCount; i++ )
			{
				memcpy( &EMVCandidateList[i - 1], &EMVCandidateList[i],
						sizeof(tagEMVCandidateList) );
			}
			memset( &EMVCandidateList[EMVInfo.CandidateCount - 1],
					NULL, sizeof(tagEMVCandidateList) );

			EMVInfo.CandidateCount--;				// 목록 갯수 감소

			if ( EMVInfo.CandidateCount <= 0 )
			{
				// 해당 거래가 불가하므로 fallBack 거래로 전환
				memcpy(m_szEMVFBCode, EMV_FBCODE_SELECTERROR, _tcslen(EMV_FBCODE_SELECTERROR));	// kh 20041015
				nReturn = EMV_RESULT_NOTSUPPORT;	// 리스트가 없으므로 거래 종료
			}
			else
			{
				nReturn = EMV_RESULT_REVERSAL;		// 후보 리스트 재표시 요구
				
for ( i = 0; i < EMVInfo.CandidateCount; i++ )
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_SELECTAPPLICATION : EMVCandidateList[%d].Brand = %s", i, EMVCandidateList[i].Brand);
}
			}
		}
		else if ( ICRecvData[nOffset] == EMV_FLAG_TERMINATE )
		{
			// 해당 거래가 불가하므로 fallback으로 전환
			memcpy(m_szEMVFBCode, EMV_FBCODE_SELECTERROR, _tcslen(EMV_FBCODE_SELECTERROR));	// kh 20041015
			return EMV_RESULT_TERMINATE;
		}
		else
		{
			memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
			fnICA_SetFCError(ICRecvData);
			fnICA_AnalyzeFCError();
			// 장비 에러 발생 시 fallback 거래 지원.
			memcpy(m_szEMVFBCode, EMV_FBCODE_SELECTERROR, _tcslen(EMV_FBCODE_SELECTERROR));	// kh 20041015
			return nReturn;
		}

		break;

	case EMV_PROC_READAPPLICATION :					// EMV Read Application
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_READAPPLICATION");

		// Return Value(LV Format) : PAN(5A) + CSN(5F34) + MS Track2 Image(57)

		if ( ICRecvData[nOffset] == EMV_FLAG_SUCCESS )
		{
			nOffset++;

			EMVDOLListInfo.PANLen = ICRecvData[nOffset];	// PAN Length
			nOffset++;
			if ( EMVDOLListInfo.PANLen != 0x00 &&
				 EMVDOLListInfo.PANLen <= sizeof(EMVDOLListInfo.PAN) )	// PAN
			{
				memcpy( EMVDOLListInfo.PAN, ICRecvData + nOffset,
						EMVDOLListInfo.PANLen );
				nOffset += EMVDOLListInfo.PANLen;

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.PAN, szLog, EMVDOLListInfo.PANLen );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_READAPPLICATION : EMVDOLListInfo.PAN = %s", szLog);
			}

			nLength = ICRecvData[nOffset];			// CSN Length
			nOffset++;
			if ( nLength == sizeof(EMVDOLListInfo.CSN) )
			{
				EMVDOLListInfo.CSN = ICRecvData[nOffset];	// CSN
				nOffset++;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_READAPPLICATION : EMVDOLListInfo.CSN = %02X", EMVDOLListInfo.CSN);
			}

			EMVDOLListInfo.MSTrack2Len = ICRecvData[nOffset];	// MS Track2 Image Length
			nOffset++;
			if ( EMVDOLListInfo.MSTrack2Len != 0x00 &&
				 EMVDOLListInfo.MSTrack2Len <= sizeof(EMVDOLListInfo.MSTrack2Image) )	// MS Track2 Image
			{
				memcpy( EMVDOLListInfo.MSTrack2Image,
						ICRecvData + nOffset,
						EMVDOLListInfo.MSTrack2Len );
				nOffset += EMVDOLListInfo.MSTrack2Len;

				memset( szLog, NULL, sizeof(szLog) );
				MakeUnPack( EMVDOLListInfo.MSTrack2Image, szLog, EMVDOLListInfo.MSTrack2Len );
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_READAPPLICATION : EMVDOLListInfo.MSTrack2Image = %s", szLog);
			}
		}
		else if ( ICRecvData[nOffset] == EMV_FLAG_TERMINATE )
		{
			// 폴백 거래로 전환
			memcpy(m_szEMVFBCode, EMV_FBCODE_READERROR, _tcslen(EMV_FBCODE_READERROR));	// kh 20041015
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : ICRecvData[nOffset] = [%x]", EMV_FLAG_TERMINATE);
			return EMV_RESULT_TERMINATE;
		}
		else
		{
			memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
			fnICA_SetFCError(ICRecvData);
			fnICA_AnalyzeFCError();
			// 장비 및 기타 에러 발생 시 폴백 지원
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : ICRecvData[nOffset] = [%x]", nReturn);
			memcpy(m_szEMVFBCode, EMV_FBCODE_READERROR, _tcslen(EMV_FBCODE_READERROR));	// kh 20041015
			return nReturn;
		}

		nReturn = T_OK;
		break;

	case EMV_PROC_OFFLINEDATAAUTHEN :				// Offline Data Authentication
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_OFFLINEDATAAUTHEN");
HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_OFFLINEDATAAUTHEN -> ICRecvData", ICRecvData, 200);

		/***********************************************************************
		| 취소 거래와 정상 처리 구분                                           |
		+-------------+---------------------+----------------------------------+
		| 31 00 00 00 | Offline Success     | EMV_SetAppCryptogram()의 값      |
		| 31 00 00 02 | Terminate(거래종료) | EMV_RESULT_TERMINATE             |
		***********************************************************************/
		if ( ICRecvData[nOffset] == EMV_FLAG_SUCCESS )
		{
			nOffset++;

			//******************************************************************
			// Offline Success 시 Return Value(LV Format) :
			// TVR(95) + AC(9F26) + ATC(9F36) + AIP(82) + CID(9F27) + IAD(9F10)
			//******************************************************************
			nLength = ICRecvData[nOffset];			// TVR Length
			nOffset++;
			if ( nLength != 0x00 &&
				 nLength <= sizeof(EMVDOLListInfo.TerminalVerifyResult) )	// TVR
			{
				memcpy( EMVDOLListInfo.TerminalVerifyResult,
						ICRecvData + nOffset, nLength );
				nOffset += nLength;

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.TerminalVerifyResult, szLog, 5 );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_OFFLINEDATAAUTHEN : EMVDOLListInfo.TerminalVerifyResult = %s", szLog);
			}

			nLength = ICRecvData[nOffset];			// AC Length
			nOffset++;
			if ( nLength != 0x00 && nLength <= EMV_ARQC_LEN )	// AC(인증서)
			{
				memcpy( EMVDOLListInfo.AppCryptogram,
						ICRecvData + nOffset, nLength );
				nOffset += nLength;

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.AppCryptogram, szLog, EMV_ARQC_LEN );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_OFFLINEDATAAUTHEN : EMVDOLListInfo.AppCryptogram = %s", szLog);
			}

			nLength = ICRecvData[nOffset];			// ATC Length
			nOffset++;
			if ( nLength != 0x00 && nLength <= EMV_ATC_LEN )	// ATC
			{
				memcpy( EMVDOLListInfo.AppTranCounter,
						ICRecvData + nOffset, nLength );
				nOffset += nLength;

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.AppTranCounter, szLog, EMV_ATC_LEN );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_OFFLINEDATAAUTHEN : EMVDOLListInfo.AppTranCounter = %s", szLog);
			}

			nLength = ICRecvData[nOffset];			// AIP Length
			nOffset++;
			if ( nLength != 0x00 &&
				 nLength <= sizeof(EMVDOLListInfo.AppInterchangeProfile) )	// AIP
			{
				memcpy( EMVDOLListInfo.AppInterchangeProfile,
						ICRecvData + nOffset, nLength );
				nOffset += nLength;

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.AppInterchangeProfile, szLog, 2 );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_OFFLINEDATAAUTHEN : EMVDOLListInfo.AppInterchangeProfile = %s", szLog);
			}

			nLength = ICRecvData[nOffset];			// CID Length
			nOffset++;
			if ( nLength == sizeof(BYTE) )			// CID
			{
				EMVDOLListInfo.CryptogramInfoData = ICRecvData[nOffset];
				nOffset++;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_OFFLINEDATAAUTHEN : EMVDOLListInfo.CryptogramInfoData = %02X", EMVDOLListInfo.CryptogramInfoData);
			}
		
			EMVDOLListInfo.IADLen = ICRecvData[nOffset];	// IAD Length
			nOffset++;
			if ( EMVDOLListInfo.IADLen != 0x00 &&
				 EMVDOLListInfo.IADLen <= sizeof(EMVDOLListInfo.IssuerAppData) )	// IAD
			{
				memset( EMVDOLListInfo.IssuerAppData,0x00, sizeof(EMVDOLListInfo.IssuerAppData)); // #N0229
				
				memcpy( EMVDOLListInfo.IssuerAppData,
						ICRecvData + nOffset, EMVDOLListInfo.IADLen );
				nOffset += EMVDOLListInfo.IADLen;

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.IssuerAppData, szLog, EMVDOLListInfo.IADLen );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_OFFLINEDATAAUTHEN : EMVDOLListInfo.IssuerAppData = %s", szLog);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_OFFLINEDATAAUTHEN : EMVDOLListInfo.IssuerAppData", szLog, sizeof(szLog));
			}

			nReturn = fnEMV_SetAppCryptogram();		// 인증서 구분(TC, ARQC, AAC, AAR)
		}
		else if ( ICRecvData[nOffset] == EMV_FLAG_TERMINATE )
		{
			// 폴백 거래로 전환한다.
			memcpy(m_szEMVFBCode, EMV_FBCODE_AFTER_OFFAUTHERR, sizeof(EMV_FBCODE_AFTER_OFFAUTHERR));	// kh 20041015
			nReturn = EMV_RESULT_TERMINATE;
		}
		else
		{
			// 폴백 거래로 전환한다.
			// 하드웨어 장애 등도 폴백 거래로 전환
			memcpy(m_szEMVFBCode, EMV_FBCODE_AFTER_OFFAUTHERR, sizeof(EMV_FBCODE_AFTER_OFFAUTHERR));	// kh 20041015
			memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
			fnICA_SetFCError(ICRecvData);
			fnICA_AnalyzeFCError();
			return nReturn;
		}

		break;

	case EMV_PROC_ONLINE :							// Online Processing
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE");

		/***********************************************************************
		| 취소 거래와 정상 처리 구분                                           |
		+----------------------+---------------------+-------------------------+
		| 31 00 00 00 02 30 30 | Online Success      | TRUE                    |
		| 31 00 00 01 02 30 30 | Online Success      | TRUE                    |
		| 31 00 00 00 02 30 32 | Reversal(취소요청)  | EMV_RESULT_REVERSAL     |
		| 31 00 00 01 02 30 32 | Reversal(취소요청)  | EMV_RESULT_REVERSAL     |
		| 31 00 00 00 02 30 35 | Online Decline      | TRUE(After Script 처리) |
		| 31 00 00 01 02 30 35 | Online Decline      | TRUE(After Script 처리) |
		| 31 00 00 02          | Terminate(거래종료) | EMV_RESULT_TERMINATE    |
		***********************************************************************/
//
HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : ICRecvData[nOffset] 1",&ICRecvData[nOffset], 20);
		if ( ICRecvData[nOffset] == EMV_FLAG_SUCCESS ||	// 0x00
			 ICRecvData[nOffset] == EMV_FLAG_WARNING )	// 0x01
		{
			nOffset++;
HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : ICRecvData[nOffset] 2 ",&ICRecvData[nOffset], 20);

			if ( ICRecvData[nOffset] != EMV_ARC_LEN )	// ARC Length
			{
HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : ICRecvData[nOffset] 3",&ICRecvData[nOffset], 20);
				memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
				fnICA_SetFCError(ICRecvData);
				fnICA_AnalyzeFCError();
				return nReturn;
			}
			nOffset++;

			// ARC Data Check
			if ( memcmp( ICRecvData + nOffset, EMV_ARC_SUCCESS, IC_SW_LEN ) == 0 )
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : EMV_ARC_SUCCESS");
				nReturn = T_OK;
			}
			else if ( memcmp( ICRecvData + nOffset, EMV_ARC_REVERSAL, IC_SW_LEN ) == 0 )
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : EMV_ARC_REVERSAL");
				nReturn = EMV_RESULT_REVERSAL;
			}
			else if ( memcmp( ICRecvData + nOffset, EMV_ARC_DECLINE, IC_SW_LEN ) == 0 )
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : EMV_ARC_DECLINE");
				nReturn = T_OK;						// After Script 이후 처리
			}
			else
			{
				memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
				fnICA_SetFCError(ICRecvData);
				fnICA_AnalyzeFCError();
				return nReturn;
			}
			// ARC Copy
			memcpy( ICRecvData + nOffset,
					EMVDOLListInfo.AuthResponseCode, EMV_ARC_LEN );
			nOffset += IC_SW_LEN;

			//******************************************************************
			//  0x00(TC), 0x01(AAC) 시 추가 Data(LV Format)
			//  : ISR(N) + TVR(5) + TSI(2)
			//******************************************************************
			EMVDOLListInfo.IssuerScriptResultLen = ICRecvData[nOffset];	// ISR Length
			nOffset++;
			if ( EMVDOLListInfo.IssuerScriptResultLen == 0x00 )		// ISR
			{
				memset( EMVDOLListInfo.IssuerScriptResult,
						0x00,
						sizeof(EMVDOLListInfo.IssuerScriptResult) );
			}
			else if ( EMVDOLListInfo.IssuerScriptResultLen <=
					  sizeof(EMVDOLListInfo.IssuerScriptResult) )
			{
				memcpy( EMVDOLListInfo.IssuerScriptResult,
						ICRecvData + nOffset,
						EMVDOLListInfo.IssuerScriptResultLen );
				nOffset += EMVDOLListInfo.IssuerScriptResultLen;
			}

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.IssuerScriptResult, szLog, EMVDOLListInfo.IssuerScriptResultLen );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : EMVDOLListInfo.IssuerScriptResult = %s", szLog);
			
			nLength = ICRecvData[nOffset];			// TVR Length
			nOffset++;
			if ( nLength == 0x00 )					// TVR
			{
				memset( EMVDOLListInfo.TerminalVerifyResult,
						0x00,
						sizeof(EMVDOLListInfo.TerminalVerifyResult) );
			}
			else if ( nLength <= sizeof(EMVDOLListInfo.TerminalVerifyResult) )
			{
				memcpy( EMVDOLListInfo.TerminalVerifyResult,
						ICRecvData + nOffset, nLength );
				nOffset += nLength;
			}

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.TerminalVerifyResult, szLog, 5 );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : EMVDOLListInfo.TerminalVerifyResult = %s", szLog);

			nLength = ICRecvData[nOffset];			// TSI Length
			nOffset++;
			if ( nLength == 0x00 )					// TSI
			{
				memset( EMVDOLListInfo.TerminalStatusInfo,
						0x00,
						sizeof(EMVDOLListInfo.TerminalStatusInfo) );
			}
			else if ( nLength <= sizeof(EMVDOLListInfo.TerminalStatusInfo) )
			{
				memcpy( EMVDOLListInfo.TerminalStatusInfo,
						ICRecvData + nOffset, nLength );
				nOffset += nLength;
			}

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.TerminalStatusInfo, szLog, 2 );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : EMVDOLListInfo.TerminalStatusInfo = %s", szLog);
		}
		else if ( ICRecvData[nOffset] == EMV_FLAG_TERMINATE )
		{
			nReturn = EMV_RESULT_TERMINATE;
		}
		else
		{
			memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
			fnICA_SetFCError(ICRecvData);
			fnICA_AnalyzeFCError();
			return nReturn;
		}

		break;

	case EMV_PROC_COMPLET :							// Completion Processing
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_COMPLET");

		/***********************************************************************
		| 취소 거래와 정상 처리 구분                                           |
		+----------------------+-----------------+-----------------------------+
		| 31 00 00 00 02 30 30 | TC Success      | TRUE                        |
		| 31 00 00 01 02 30 30 | AAC Reversal    | EMV_RESULT_REVERSAL         |
		| 31 00 00 00 02 30 35 | TC FAIL         | TRUE(After Script에서 처리) |
		| 31 00 00 01 02 30 35 | AAC FAIL        | TRUE(After Script에서 처리) |
		| 31 00 00 02          | Terminate(종료) | EMV_RESULT_TERMINATE        |
		***********************************************************************/
		if ( ICRecvData[nOffset] == EMV_FLAG_SUCCESS ||	// 0x00
			 ICRecvData[nOffset] == EMV_FLAG_WARNING )	// 0x01
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_COMPLET : EMV_FLAG_SUCCESS or EMV_FLAG_WARNING");

			nCount = ICRecvData[nOffset];				// 임시 저장
			nOffset++;

			if ( ICRecvData[nOffset] != EMV_ARC_LEN )	// ARC Length
			{
				memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
				fnICA_SetFCError(ICRecvData);
				fnICA_AnalyzeFCError();
				return nReturn;
			}
			nOffset++;

			if ( memcmp( ICRecvData + nOffset, EMV_ARC_SUCCESS, IC_SW_LEN ) == 0 )
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_COMPLET : EMV_FLAG_SUCCESS");

				if ( nCount == EMV_FLAG_SUCCESS )
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_COMPLET : EMV_FLAG_SUCCESS");
					nReturn = T_OK;				
				}
				else
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_COMPLET : EMV_FLAG_WARNING");
					// 해당 사항에 대하여는 현재는 정상 거래로 처리함.
					// 차후 필히 확인 바람.
					// nReturn = EMV_RESULT_REVERSAL;
					nReturn = T_OK;
				}
			}
			else if ( memcmp( ICRecvData + nOffset, EMV_ARC_DECLINE, IC_SW_LEN ) == 0 )
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_COMPLET : EMV_ARC_DECLINE");
				nReturn = T_OK;
			}
			else if ( ICRecvData[nOffset] == EMV_ARC_TERMINALY1[0] ||	// "Y"(0x59)
					  ICRecvData[nOffset] == EMV_ARC_TERMINALZ1[0] )	// "Z"(0x5A)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_COMPLET : EMV_ARC_SUCCESS");
				nReturn = T_OK;
			}
			else
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_COMPLET : Data Error!");

				memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
				fnICA_SetFCError(ICRecvData);
				fnICA_AnalyzeFCError();
				return nReturn;
			}
			// ARC Copy
			memcpy( ICRecvData + nOffset,
					EMVDOLListInfo.AuthResponseCode, EMV_ARC_LEN );
			nOffset += IC_SW_LEN;
			
			//******************************************************************
			//  TC나 AAC 시 추가 Data(LV Format)
			//  : ISR(N) + TVR(5) + TSI(2)
			//******************************************************************
			EMVDOLListInfo.IssuerScriptResultLen = ICRecvData[nOffset];	// ISR Length
			nOffset++;
			if ( EMVDOLListInfo.IssuerScriptResultLen == 0x00 )		// ISR
			{
				memset( EMVDOLListInfo.IssuerScriptResult,
						0x00,
						sizeof(EMVDOLListInfo.IssuerScriptResult) );
			}
			else if ( EMVDOLListInfo.IssuerScriptResultLen <=
					  sizeof(EMVDOLListInfo.IssuerScriptResult) )
			{
				memcpy( EMVDOLListInfo.IssuerScriptResult,
						ICRecvData + nOffset,
						EMVDOLListInfo.IssuerScriptResultLen );
				nOffset += EMVDOLListInfo.IssuerScriptResultLen;
			}

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.IssuerScriptResult, szLog, EMVDOLListInfo.IssuerScriptResultLen );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : EMVDOLListInfo.IssuerScriptResult = %s", szLog);

			nLength = ICRecvData[nOffset];			// TVR Length
			nOffset++;
			if ( nLength == 0x00 )					// TVR
			{
				memset( EMVDOLListInfo.TerminalVerifyResult,
						0x00,
						sizeof(EMVDOLListInfo.TerminalVerifyResult) );
			}
			else if ( nLength <= sizeof(EMVDOLListInfo.TerminalVerifyResult) )
			{
				memcpy( EMVDOLListInfo.TerminalVerifyResult,
						ICRecvData + nOffset, nLength );
				nOffset += nLength;
			}

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.TerminalVerifyResult, szLog, 5 );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : EMVDOLListInfo.TerminalVerifyResult = %s", szLog);

			nLength = ICRecvData[nOffset];			// TSI Length
			nOffset++;
			if ( nLength == 0x00 )					// TSI
			{
				memset( EMVDOLListInfo.TerminalStatusInfo,
						0x00,
						sizeof(EMVDOLListInfo.TerminalStatusInfo) );
			}
			else if ( nLength <= sizeof(EMVDOLListInfo.TerminalStatusInfo) )
			{
				memcpy( EMVDOLListInfo.TerminalStatusInfo,
						ICRecvData + nOffset, nLength );
				nOffset += nLength;
			}

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.TerminalStatusInfo, szLog, 2 );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_ONLINE : EMVDOLListInfo.TerminalStatusInfo = %s", szLog);
		}
		else if ( ICRecvData[nOffset] == EMV_FLAG_TERMINATE )
		{
			nReturn = EMV_RESULT_TERMINATE;
		}
		else
		{
			memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
			fnICA_SetFCError(ICRecvData);
			fnICA_AnalyzeFCError();
			return nReturn;
		}

		break;

	case EMV_PROC_AFTERSCRIPT :						// After Script Processing
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_AFTERSCRIPT");

		/***********************************************************************
		| 취소 거래와 정상 처리 구분                                           |
		+----------------------+-----------------+-----------------------------+
		| 31 00 00 00 02 30 30 | TC Success      | TRUE                        |
		| 31 00 00 01 02 30 30 | AAC Success     | TRUE                        |
		| 31 00 00 00 02 30 35 | TC FAIL         | EMV_RESULT_ONLINEDECLINE    |
		| 31 00 00 01 02 30 35 | AAC FAIL        | EMV_RESULT_ONLINEDECLINE    |
		| 31 00 00 02          | Terminate(종료) | EMV_RESULT_TERMINATE        |
		***********************************************************************/
		if ( ICRecvData[nOffset] == EMV_FLAG_SUCCESS ||	// 0x00
			 ICRecvData[nOffset] == EMV_FLAG_WARNING )	// 0x01
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_AFTERSCRIPT : EMV_FLAG_SUCCESS or EMV_FLAG_WARNING");

			nOffset++;

			if ( ICRecvData[nOffset] != EMV_ARC_LEN )	// ARC Length
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_AFTERSCRIPT : Get Data Error!");

				memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
				fnICA_SetFCError(ICRecvData);
				fnICA_AnalyzeFCError();
				return nReturn;
			}
			nOffset++;

			if ( memcmp( ICRecvData + nOffset, EMV_ARC_SUCCESS, IC_SW_LEN ) == 0 )
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_AFTERSCRIPT : EMV_ARC_SUCCESS");
				nReturn = T_OK;
			}
			else if ( memcmp( ICRecvData + nOffset, EMV_ARC_DECLINE, IC_SW_LEN ) == 0 )
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_AFTERSCRIPT : EMV_ARC_DECLINE");
				nReturn = EMV_RESULT_ONLINEDECLINE;

#if EMV_TA_MODE		// 인증 모드
				// 인증시 화면에 Decline 표시
				strcpy( EMVInfo.CertificationType, "DECLINE" );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_AFTERSCRIPT : EMVInfo.CertificationType = %s", EMVInfo.CertificationType);
#endif	// #if EMV_TA_MODE
			}
			else if ( ICRecvData[nOffset] == EMV_ARC_TERMINALY1[0] ||	// "Y"(0x59)
					  ICRecvData[nOffset] == EMV_ARC_TERMINALZ1[0] )	// "Z"(0x5A)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_AFTERSCRIPT : EMV_FLAG_SUCCESS");
				nReturn = T_OK;
			}
			else
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_AFTERSCRIPT : Get Data Error!");

				memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
				fnICA_SetFCError(ICRecvData);
				fnICA_AnalyzeFCError();
				return nReturn;
			}

			// ARC Copy
			memcpy( ICRecvData + nOffset,
					EMVDOLListInfo.AuthResponseCode, EMV_ARC_LEN );
			nOffset += IC_SW_LEN;
			
			//******************************************************************
			//  TC나 AAC 시 추가 Data(LV Format)
			//  : ISR(N) + TVR(5) + TSI(2)
			//******************************************************************
			EMVDOLListInfo.IssuerScriptResultLen = ICRecvData[nOffset];	// ISR Length
			nOffset++;
			if ( EMVDOLListInfo.IssuerScriptResultLen == 0x00 )			// ISR
			{
				memset( EMVDOLListInfo.IssuerScriptResult,
						0x00,
						sizeof(EMVDOLListInfo.IssuerScriptResult) );
			}
			else if ( EMVDOLListInfo.IssuerScriptResultLen <=
					  sizeof(EMVDOLListInfo.IssuerScriptResult) )
			{
				memcpy( EMVDOLListInfo.IssuerScriptResult,
						ICRecvData + nOffset,
						EMVDOLListInfo.IssuerScriptResultLen );
				nOffset += EMVDOLListInfo.IssuerScriptResultLen;
			}

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.IssuerScriptResult, szLog, EMVDOLListInfo.IssuerScriptResultLen );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_AFTERSCRIPT : EMVDOLListInfo.IssuerScriptResult = %s", szLog);

			nLength = ICRecvData[nOffset];			// TVR Length
			nOffset++;
			if ( nLength == 0x00 )					// TVR
			{
				memset( EMVDOLListInfo.TerminalVerifyResult,
						0x00,
						sizeof(EMVDOLListInfo.TerminalVerifyResult) );
			}
			else if ( nLength <= sizeof(EMVDOLListInfo.TerminalVerifyResult) )
			{
				memcpy( EMVDOLListInfo.TerminalVerifyResult,
						ICRecvData + nOffset, nLength );
				nOffset += nLength;
			}

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.TerminalVerifyResult, szLog, 5, TRUE );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_AFTERSCRIPT : EMVDOLListInfo.TerminalVerifyResult = %s", szLog);

			nLength = ICRecvData[nOffset];			// TSI Length
			nOffset++;
			if ( nLength == 0x00 )					// TSI
			{
				memset( EMVDOLListInfo.TerminalStatusInfo,
						0x00,
						sizeof(EMVDOLListInfo.TerminalStatusInfo) );
			}
			else if ( nLength <= sizeof(EMVDOLListInfo.TerminalStatusInfo) )
			{
				memcpy( EMVDOLListInfo.TerminalStatusInfo,
						ICRecvData + nOffset, nLength );
				nOffset += nLength;
			}

memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.TerminalStatusInfo, szLog, 2 );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_AFTERSCRIPT : EMVDOLListInfo.TerminalStatusInfo = %s", szLog);
		}
		else if ( ICRecvData[nOffset] == EMV_FLAG_TERMINATE )
		{
			nReturn = EMV_RESULT_TERMINATE;
		}
		else
		{
			memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
			fnICA_SetFCError(ICRecvData);
			fnICA_AnalyzeFCError();
			return nReturn;
		}

		break;
	
	case EMV_PROC_GETDOLDATA :						// Get DOL Data
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_GETDOLDATA");

		if ( ICRecvData[nOffset] == EMV_FLAG_SUCCESS )
		{
			nReturn = T_OK;
		}
		else if ( ICRecvData[nOffset] == EMV_FLAG_TERMINATE )
		{
			nReturn = EMV_RESULT_TERMINATE;			// 해당 객체가 없음
		}
		else
		{
			memcpy( ICRecvData, "WxD2Wx42Wx44Wx44", IC_ERRORRESP_LEN );	// Get Data Error
			fnICA_SetFCError(ICRecvData);
			fnICA_AnalyzeFCError();
			return nReturn;
		}

		break;

	case EMV_PROC_RESULTDATA :						// EMV Result Data
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_RESULTDATA");

		nOffset++;									// 0x00

		/***********************************************************************
		| 반환 데이터 객체 리스트                                              |
		+----------+-------+----------+----------------------------------------+
		|  Length  | Value | 참조 Tag |               설    명                 |
		+----------+-------+----------+----------------------------------------+
		|     5    | TVR   |   95     | Terminal Verification Result           |
		|     2    | TSI   |   9B     | Terminal Status Infomation             |
		|     5    | ISR   |   9F58   | Issuer Script Result                   |
		|     2    | ARC   |   8A     | Authorization Response Code            |
		| Variable | CVML  |   9E     | Cardholder Verification Method List    |
		|          |       |          | Max. 252이나 화면 표시용에는 그 중     |
		|          |       |          | 30바이트의 정보만 담는다.              |
		|     3    | CVMR  |   9F34   | Cardholder Verification Method Result  |
		|  8 ~ 10  | PAN   |   5A     | Primary Account Number                 |
		| Variable | AID   |   4F     | Application Identifier. Max. 16        |
		|     2    | ATC   |   9F36   | Application Transaction Counter        |
		|     6    | TA_T  |   9F02   | Transaction Amount(BCD)                |
		***********************************************************************/

		// Terminal Verification Result(5)
		nLength = ICRecvData[nOffset];
		nOffset++;
		if ( nLength == 0x00 )
		{
			memset( EMVDOLListInfo.TerminalVerifyResult,
					NULL,
					sizeof(EMVDOLListInfo.TerminalVerifyResult) );
		}
		else
		{
			memcpy( EMVDOLListInfo.TerminalVerifyResult,
					ICRecvData + nOffset, nLength );
			nOffset += nLength;
		}

		memset( szLog, NULL, sizeof(szLog) );
		MakeUnPack( EMVDOLListInfo.TerminalVerifyResult, szLog, 5 );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_RESULTDATA : EMVDOLListInfo.TerminalVerifyResult = %s", szLog);

		// Terminal Status Infomation(2)
		nLength = ICRecvData[nOffset];
		nOffset++;
		if ( nLength == 0x00 )
		{
			memset( EMVDOLListInfo.TerminalStatusInfo,
					NULL,
					sizeof(EMVDOLListInfo.TerminalStatusInfo) );
		}
		else
		{
			memcpy( EMVDOLListInfo.TerminalStatusInfo,
					ICRecvData + nOffset, nLength );
			nOffset += nLength;
		}

		memset( szLog, NULL, sizeof(szLog) );
		MakeUnPack( EMVDOLListInfo.TerminalStatusInfo, szLog, 2 );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_RESULTDATA : EMVDOLListInfo.TerminalStatusInfo = %s", szLog);

		// Issuer Script Result(N)
		EMVDOLListInfo.IssuerScriptResultLen = ICRecvData[nOffset];
		nOffset++;
		if ( EMVDOLListInfo.IssuerScriptResultLen == 0x00 )
		{
			memset( EMVDOLListInfo.IssuerScriptResult,
					NULL,
					sizeof(EMVDOLListInfo.IssuerScriptResult) );
		}
		else
		{
			memcpy( EMVDOLListInfo.IssuerScriptResult,
					ICRecvData + nOffset,
					EMVDOLListInfo.IssuerScriptResultLen );
			nOffset += EMVDOLListInfo.IssuerScriptResultLen;
		}

		memset( szLog, NULL, sizeof(szLog) );
		MakeUnPack( EMVDOLListInfo.IssuerScriptResult, szLog, EMVDOLListInfo.IssuerScriptResultLen );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_RESULTDATA : EMVDOLListInfo.IssuerScriptResult = %s", szLog);

		// Authorization Response Code(2)
		nLength = ICRecvData[nOffset];
		nOffset++;
		if ( nLength == 0x00 )
		{
			memset( EMVDOLListInfo.AuthResponseCode,
					NULL,
					sizeof(EMVDOLListInfo.AuthResponseCode) );
		}
		else
		{
			memcpy( EMVDOLListInfo.AuthResponseCode,
					ICRecvData + nOffset, nLength );
			nOffset += nLength;
		}

		memset( szLog, NULL, sizeof(szLog) );
		MakeUnPack( EMVDOLListInfo.AuthResponseCode, szLog, 2 );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_RESULTDATA : EMVDOLListInfo.AuthResponseCode = %s", szLog);

		// Cardholder Verification Method List(Max. 252)
		EMVDOLListInfo.CVMLLen = ICRecvData[nOffset];
		nOffset++;
		if ( EMVDOLListInfo.CVMLLen == 0x00 )
		{
			memset( EMVDOLListInfo.CVML, NULL, sizeof(EMVDOLListInfo.CVML) );
		}
		else
		{
			memcpy( EMVDOLListInfo.CVML, ICRecvData + nOffset,
					EMVDOLListInfo.CVMLLen );
			nOffset += EMVDOLListInfo.CVMLLen;
		}

		memset( szLog, NULL, sizeof(szLog) );
		if ( EMVDOLListInfo.CVMLLen > 30 )
			MakeUnPack( EMVDOLListInfo.CVML, szLog, 30 );
		else
			MakeUnPack( EMVDOLListInfo.CVML, szLog, EMVDOLListInfo.CVMLLen );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_RESULTDATA : EMVDOLListInfo.CVML = %s", szLog);

		// Cardholder Verification Method Result(3)
		nLength = ICRecvData[nOffset];
		nOffset++;
		if ( nLength == 0x00 )
		{
			memset( EMVDOLListInfo.CVMR, NULL, sizeof(EMVDOLListInfo.CVMR) );
		}
		else
		{
			memcpy( EMVDOLListInfo.CVMR, ICRecvData + nOffset, nLength );
			nOffset += nLength;
		}

		memset( szLog, NULL, sizeof(szLog) );
		MakeUnPack( EMVDOLListInfo.CVMR, szLog, 3 );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_RESULTDATA : EMVDOLListInfo.CVMR = %s", szLog);

		// Primary Account Number(8 ~ 10)
		EMVDOLListInfo.PANLen = ICRecvData[nOffset];
		nOffset++;
		if ( EMVDOLListInfo.PANLen == 0x00 )
		{
			memset( EMVDOLListInfo.PAN, NULL, sizeof(EMVDOLListInfo.PAN) );
		}
		else
		{
			memcpy( EMVDOLListInfo.PAN, ICRecvData + nOffset,
					EMVDOLListInfo.PANLen );
			nOffset += EMVDOLListInfo.PANLen;
		}

		memset( szLog, NULL, sizeof(szLog) );
		MakeUnPack( EMVDOLListInfo.PAN, szLog, EMVDOLListInfo.PANLen );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_RESULTDATA : EMVDOLListInfo.PAN = %s", szLog);

		// Application Identifier(Max. 16)
		EMVDOLListInfo.AppIDLen = ICRecvData[nOffset];
		nOffset++;
		if ( EMVDOLListInfo.AppIDLen == 0x00 )
		{
			memset( EMVDOLListInfo.AppID, NULL, sizeof(EMVDOLListInfo.AppID) );
		}
		else
		{
			memcpy( EMVDOLListInfo.AppID, ICRecvData + nOffset,
					EMVDOLListInfo.AppIDLen );
			nOffset += EMVDOLListInfo.AppIDLen;
		}

		memset( szLog, NULL, sizeof(szLog) );
		MakeUnPack( EMVDOLListInfo.AppID, szLog, EMVDOLListInfo.AppIDLen );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMV_PROC_RESULTDATA : EMVDOLListInfo.AppID = %s", szLog);

		// Application Transaction Counter(2)
		nLength = ICRecvData[nOffset];
		nOffset++;
		if ( nLength == 0x00 )
		{
			memset( EMVDOLListInfo.AppTranCounter, NULL,
					sizeof(EMVDOLListInfo.AppTranCounter) );
		}
		else
		{
			memcpy( EMVDOLListInfo.AppTranCounter,
				ICRecvData + nOffset, nLength );
			nOffset += nLength;
		}

		memset( szLog, NULL, sizeof(szLog) );
		MakeUnPack( EMVDOLListInfo.AppTranCounter, szLog, 2 );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMVDOLListInfo.AppTranCounter = %s", szLog);

		// Transaction Amount(BCD)(6)
		nLength = ICRecvData[nOffset];
		nOffset++;
		if ( nLength == 0x00 )
		{
			memset( EMVDOLListInfo.TranAmountBCD,
					NULL, sizeof(EMVDOLListInfo.TranAmountBCD) );
		}
		else
		{
			memcpy( EMVDOLListInfo.TranAmountBCD,
					ICRecvData + nOffset, nLength );
			nOffset += nLength;
		}

		memset( szLog, NULL, sizeof(szLog) );
		MakeUnPack( EMVDOLListInfo.TranAmountBCD, szLog, 6 );
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : EMVDOLListInfo.TranAmountBCD = %s", szLog);

#if EMV_TA_MODE		// 인증 모드
		//**********************************************************************
		//  화면 출력 정보 구조체에 Setting. 정보가 없는 것은 보여주지 않는다.
		//**********************************************************************
		// Terminal Verification Result(95)
		MakeUnPack( EMVDOLListInfo.TerminalVerifyResult,
					EMVResultDisp.TVR,
					sizeof(EMVDOLListInfo.TerminalVerifyResult) );

		// Terminal Status Infomation(9B)
		MakeUnPack( EMVDOLListInfo.TerminalStatusInfo,
					EMVResultDisp.TSI,
					sizeof(EMVDOLListInfo.TerminalStatusInfo) );

		// Issuer Script Result(9F5B)
		nLength = ( EMVDOLListInfo.IssuerScriptResultLen >= sizeof(EMVResultDisp.ISR) / 2 ) ?
				  sizeof(EMVResultDisp.ISR) / 2 : EMVDOLListInfo.IssuerScriptResultLen;
		MakeUnPack( EMVDOLListInfo.IssuerScriptResult,
					EMVResultDisp.ISR, nLength );
		if ( EMVDOLListInfo.IssuerScriptResultLen == 0x00 )
		{
			memset( EMVResultDisp.ISR, NULL, sizeof(EMVResultDisp.ISR) );
		}

		// Authorization Response Code(8A)
		MakeUnPack( EMVDOLListInfo.AuthResponseCode,
					EMVResultDisp.ARC,
					sizeof(EMVDOLListInfo.AuthResponseCode) );

		// Cardholder Verification Method List(8E)
		nLength = ( EMVDOLListInfo.CVMLLen >= sizeof(EMVResultDisp.CVML) / 2 ) ?
				  sizeof(EMVResultDisp.CVML) / 2 : EMVDOLListInfo.CVMLLen;
		MakeUnPack( EMVDOLListInfo.CVML, EMVResultDisp.CVML, nLength );
		if ( EMVDOLListInfo.CVMLLen == 0x00 )
		{
			memset( EMVResultDisp.CVML,
					NULL, sizeof(EMVResultDisp.CVML) );
		}

		// Cardholder Verification Method Result(9F34)
		MakeUnPack( EMVDOLListInfo.CVMR, EMVResultDisp.CVMR,
					sizeof(EMVDOLListInfo.CVMR) );
		if ( IsZero( EMVResultDisp.CVMR, sizeof(EMVDOLListInfo.CVMR) * 2 ) == TRUE )
		{
			memset( EMVResultDisp.CVMR, NULL, sizeof(EMVResultDisp.CVMR) );
		}

		// Primary Account Number(5A)
		MakeUnPack( EMVDOLListInfo.PAN, EMVResultDisp.PAN, EMVDOLListInfo.PANLen );

		// Application Identifier(4F)
		MakeUnPack( EMVDOLListInfo.AppID, EMVResultDisp.AID,
					EMVDOLListInfo.AppIDLen );

		// Application Transaction Counter(9F36)
		MakeUnPack( EMVDOLListInfo.AppTranCounter, EMVResultDisp.ATC,
					sizeof(EMVDOLListInfo.AppTranCounter) );

		// Transaction Amount(BCD)(9F02)
		MakeUnPack( EMVDOLListInfo.TranAmountBCD, EMVResultDisp.TAT,
					sizeof(EMVDOLListInfo.TranAmountBCD) );
#endif	// #if EMV_TA_MODE

		nReturn = T_OK;
		break;

	default :
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_AnalRecvData() : default");
		break;
	}

	return nReturn;
}


////////////////////////////////////////////////////////////////////////////////
//  Sub Operation Function
////////////////////////////////////////////////////////////////////////////////

/*==============================================================================
* Function Name : fnEMV_InitProcess()
* Description   : 거래시마다 초기화 되어야 할 Buffer를 Clear하는 함수
* Parameters    : (None)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_InitProcess()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_InitProcess()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Initialize Process");

	memset( &EMVCandidateList, NULL, sizeof(EMVCandidateList) );
#if EMV_TA_MODE										// 인증 모드
	memset(    &EMVResultDisp, NULL,    sizeof(EMVResultDisp) );
#endif	// #if EMV_TA_MODE

	return TRUE;
}

/*==============================================================================
* Function Name : fnEMV_DefaultDOLSet()
* Description   : 단말기에서 Default로 사용할 Data Object List를 Set하는 함수
* Parameters    : (None)
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_DefaultDOLSet()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSet()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Default DOL Set");

	// Additional Terminal Capabilitise(9F40)
	memcpy( EMVDOLListInfo.AddTerminalCapabilitise, EMV_DDO_ATC, sizeof(EMV_DDO_ATC) );

	// Application Version Number(EMV 2000)(9F09)
	memcpy( EMVDOLListInfo.AppVersionNum, EMV_DDO_AVN, sizeof(EMV_DDO_AVN) );

	// Terminal Action Code - Denial(5F50)
	memcpy( EMVDOLListInfo.TACDenial, EMV_DDO_TAC_DENIAL, sizeof(EMV_DDO_TAC_DENIAL) );

	// Terminal Action Code - Online(5F51)
	memcpy( EMVDOLListInfo.TACOnline, EMV_DDO_TAC_ONLINE, sizeof(EMV_DDO_TAC_ONLINE) );

	// Terminal Action Code - Default(5F52)
	memcpy( EMVDOLListInfo.TACDefault, EMV_DDO_TAC_DEFAULT, sizeof(EMV_DDO_TAC_DEFAULT) );

	// Terminal Capabilitise(9F33)
	memcpy( EMVDOLListInfo.TerminalCapabilitise, EMV_DDO_TC_9F33, sizeof(EMV_DDO_TC_9F33) );

	// Transaction Floor Limit(9F1B)
	memcpy( EMVDOLListInfo.TranFloorLimit, EMV_DDO_TFL, sizeof(EMV_DDO_TFL) );

	// Terminal Type(9F35)
	EMVDOLListInfo.TerminalType = EMV_DDO_TT_9F35;

	// Transaction Currency Code(5F2A). 한국(0410)
	memcpy( EMVDOLListInfo.TranCurrencyCode, EMV_DDO_TCC_5F2A, sizeof(EMV_DDO_TCC_5F2A) );

	// Terminal Country Code(9F1A). 한국(0410)
	memcpy( EMVDOLListInfo.TerminalCountryCode, EMV_DDO_TCC_9F1A, sizeof(EMV_DDO_TCC_9F1A) );

	// Transaction Type(9C)
	EMVDOLListInfo.TransactionType = EMV_DDO_TC_9C;
	
	// Dynamic Data Authentication Data Object List(5F53)
	memcpy( EMVDOLListInfo.DDOL, EMV_DDO_DDOL, sizeof(EMV_DDO_DDOL) );
	EMVDOLListInfo.DDOLLen = 0x00;

	// Transaction Certificate Data Object List(5F54)
	memcpy( EMVDOLListInfo.TDOL, EMV_DDO_TDOL, sizeof(EMV_DDO_TDOL) );
	EMVDOLListInfo.TDOLLen = 0x00;

	// Payment System Enviroment(5F55)
	strcpy( EMVDOLListInfo.PaySysEnviroment, EMV_DDO_PSE );

#if EMV_TA_MODE										// 인증 모드

	//**************************************************************************
	// 인증 Data Object List들 Set
	//**************************************************************************
	BYTE	TAC_Denial[5], TAC_Online[5], TAC_Default[5];
	BYTE	Default_DDOL[9], Default_TDOL[8];
	int		nOffset;

	switch (EMVInfo.ConfigCode)
	{
	case 2 :
		memcpy(  TAC_Denial, EMV_TA_TAC_00MODE,  sizeof(TAC_Denial) );
		memcpy(  TAC_Online, EMV_TA_TAC_00MODE,  sizeof(TAC_Online) );
		memcpy( TAC_Default, EMV_TA_TAC_00MODE, sizeof(TAC_Default) );

		// Default_DDOL : Not Present
		memset( Default_DDOL, NULL, sizeof(Default_DDOL) );
		EMVDOLListInfo.DDOLLen = 0x00;

		// Default_TDOL : "9F 02 06 9F 03 06 8A 02"
		nOffset = 0;

		memcpy( Default_TDOL + nOffset,
				EMV_TAG_BCDAMOUNT, sizeof(EMV_TAG_BCDAMOUNT) );		// 0x9F02
		nOffset += sizeof(EMV_TAG_BCDAMOUNT);

		Default_TDOL[nOffset] = sizeof(EMVDOLListInfo.TranAmountBCD);	// 0x06
		nOffset++;

		memcpy( Default_TDOL + nOffset,
				EMV_TAG_BCDANAMOUNT, sizeof(EMV_TAG_BCDANAMOUNT) );	// 0x9F03
		nOffset += sizeof(EMV_TAG_BCDANAMOUNT);

		Default_TDOL[nOffset] = sizeof(EMVDOLListInfo.AnotherTranAmountBCD);	// 0x06
		nOffset++;

		Default_TDOL[nOffset] = EMV_TAG_ARC;		// 0x8A
		nOffset++;

		Default_TDOL[nOffset] = EMV_ARC_LEN;		// 0x02
		nOffset++;

		EMVDOLListInfo.TDOLLen = (BYTE)nOffset;

		break;

	case 3 :
		memcpy(  TAC_Denial, EMV_TA_TAC_00MODE,  sizeof(TAC_Denial) );
		memcpy(  TAC_Online, EMV_TA_TAC_00MODE,  sizeof(TAC_Online) );
		memcpy( TAC_Default, EMV_TA_TAC_00MODE, sizeof(TAC_Default) );

		// Default_DDOL : "9F 02 06 9F 03 06"
		nOffset = 0;

		memcpy( Default_DDOL + nOffset,
				EMV_TAG_BCDAMOUNT, sizeof(EMV_TAG_BCDAMOUNT) );		// 0x9F02
		nOffset += sizeof(EMV_TAG_BCDAMOUNT);

		Default_DDOL[nOffset] = sizeof(EMVDOLListInfo.TranAmountBCD);	// 0x06
		nOffset++;

		memcpy( Default_DDOL + nOffset,
				EMV_TAG_BCDANAMOUNT, sizeof(EMV_TAG_BCDANAMOUNT) );	// 0x9F03
		nOffset += sizeof(EMV_TAG_BCDANAMOUNT);

		Default_DDOL[nOffset] = sizeof(EMVDOLListInfo.AnotherTranAmountBCD);	// 0x06
		nOffset++;

		EMVDOLListInfo.DDOLLen = (BYTE)nOffset;

		// Default_TDOL : "9F 02 06 9F 03 06 8A 02"
		nOffset = 0;

		memcpy( Default_TDOL + nOffset,
				EMV_TAG_BCDAMOUNT, sizeof(EMV_TAG_BCDAMOUNT) );		// 0x9F02
		nOffset += sizeof(EMV_TAG_BCDAMOUNT);

		Default_TDOL[nOffset] = sizeof(EMVDOLListInfo.TranAmountBCD);	// 0x06
		nOffset++;

		memcpy( Default_TDOL + nOffset,
				EMV_TAG_BCDANAMOUNT, sizeof(EMV_TAG_BCDANAMOUNT) );	// 0x9F03
		nOffset += sizeof(EMV_TAG_BCDANAMOUNT);

		Default_TDOL[nOffset] = sizeof(EMVDOLListInfo.AnotherTranAmountBCD);	// 0x06
		nOffset++;

		Default_TDOL[nOffset] = EMV_TAG_ARC;		// 0x8A
		nOffset++;

		Default_TDOL[nOffset] = EMV_ARC_LEN;	// 0x02
		nOffset++;

		EMVDOLListInfo.TDOLLen = (BYTE)nOffset;

		break;

	case 4 :
		memcpy(  TAC_Denial, EMV_TA_TAC_00MODE,  sizeof(TAC_Denial) );
		memcpy(  TAC_Online, EMV_TA_TAC_00MODE,  sizeof(TAC_Online) );
		memcpy( TAC_Default, EMV_TA_TAC_00MODE, sizeof(TAC_Default) );

		// Default_DDOL : "9F 02 06 9F 03 06 9F 37 04"
		nOffset = 0;

		memcpy( Default_DDOL + nOffset,
				EMV_TAG_BCDAMOUNT, sizeof(EMV_TAG_BCDAMOUNT) );		// 0x9F02
		nOffset += sizeof(EMV_TAG_BCDAMOUNT);

		Default_DDOL[nOffset] = sizeof(EMVDOLListInfo.TranAmountBCD);	// 0x06
		nOffset++;

		memcpy( Default_DDOL + nOffset,
				EMV_TAG_BCDANAMOUNT, sizeof(EMV_TAG_BCDANAMOUNT) );	// 0x9F03
		nOffset += sizeof(EMV_TAG_BCDANAMOUNT);

		Default_DDOL[nOffset] = sizeof(EMVDOLListInfo.AnotherTranAmountBCD);	// 0x06
		nOffset++;

		memcpy( Default_DDOL + nOffset,
				EMV_TAG_RANDOMNO, sizeof(EMV_TAG_RANDOMNO) );		// 0x9F03
		nOffset += sizeof(EMV_TAG_RANDOMNO);

		Default_DDOL[nOffset] = sizeof(EMVDOLListInfo.UnpredictableNumber);	// 0x04
		nOffset++;

		EMVDOLListInfo.DDOLLen = (BYTE)nOffset;

		// Default_TDOL : Not Present
		memset( Default_TDOL, NULL, sizeof(Default_TDOL) );
		EMVDOLListInfo.TDOLLen = 0x00;

		break;

	case 5 :
	case 6 :
	case 7 :
	case 8 :
	case 9 :
		memcpy(  TAC_Denial, EMV_TA_TAC_00MODE,  sizeof(TAC_Denial) );
		memcpy(  TAC_Online, EMV_TA_TAC_00MODE,  sizeof(TAC_Online) );
		memcpy( TAC_Default, EMV_TA_TAC_00MODE, sizeof(TAC_Default) );

		if ( EMVInfo.ConfigCode == 5 )
		{
			memcpy(  TAC_Denial, EMV_TA_TAC_80MODE,  sizeof(TAC_Denial) );
		}
		else if ( EMVInfo.ConfigCode == 6 )
		{
			memcpy(  TAC_Online, EMV_TA_TAC_80MODE,  sizeof(TAC_Denial) );
		}
		else if ( EMVInfo.ConfigCode == 7 )
		{
			memcpy( TAC_Default, EMV_TA_TAC_80MODE, sizeof(TAC_Default) );
		}
		else if ( EMVInfo.ConfigCode == 8 )
		{
			memcpy(  TAC_Online, EMV_TA_TAC_FFMODE,  sizeof(TAC_Online) );
			memcpy( TAC_Default, EMV_TA_TAC_80MODE, sizeof(TAC_Default) );
		}
		else
		{
			memcpy(  TAC_Online, EMV_TA_TAC_FFMODE,  sizeof(TAC_Online) );
		}

		// Default_DDOL : "9F 02 06 9F 03 06 9F 37 04"
		nOffset = 0;

		memcpy( Default_DDOL + nOffset,
				EMV_TAG_BCDAMOUNT, sizeof(EMV_TAG_BCDAMOUNT) );		// 0x9F02
		nOffset += sizeof(EMV_TAG_BCDAMOUNT);

		Default_DDOL[nOffset] = sizeof(EMVDOLListInfo.TranAmountBCD);	// 0x06
		nOffset++;

		memcpy( Default_DDOL + nOffset,
				EMV_TAG_BCDANAMOUNT, sizeof(EMV_TAG_BCDANAMOUNT) );	// 0x9F03
		nOffset += sizeof(EMV_TAG_BCDANAMOUNT);

		Default_DDOL[nOffset] = sizeof(EMVDOLListInfo.AnotherTranAmountBCD);	// 0x06
		nOffset++;

		memcpy( Default_DDOL + nOffset,
				EMV_TAG_RANDOMNO, sizeof(EMV_TAG_RANDOMNO) );		// 0x9F03
		nOffset += sizeof(EMV_TAG_RANDOMNO);

		Default_DDOL[nOffset] = sizeof(EMVDOLListInfo.UnpredictableNumber);	// 0x04
		nOffset++;

		EMVDOLListInfo.DDOLLen = (BYTE)nOffset;

		// Default_TDOL : "9F 02 06 9F 03 06 8A 02"
		nOffset = 0;

		memcpy( Default_TDOL + nOffset,
				EMV_TAG_BCDAMOUNT, sizeof(EMV_TAG_BCDAMOUNT) );		// 0x9F02
		nOffset += sizeof(EMV_TAG_BCDAMOUNT);

		Default_TDOL[nOffset] = sizeof(EMVDOLListInfo.TranAmountBCD);	// 0x06
		nOffset++;

		memcpy( Default_TDOL + nOffset,
				EMV_TAG_BCDANAMOUNT, sizeof(EMV_TAG_BCDANAMOUNT) );	// 0x9F03
		nOffset += sizeof(EMV_TAG_BCDANAMOUNT);

		Default_TDOL[nOffset] = sizeof(EMVDOLListInfo.AnotherTranAmountBCD);	// 0x06
		nOffset++;

		Default_TDOL[nOffset] = EMV_TAG_ARC;		// 0x8A
		nOffset++;

		Default_TDOL[nOffset] = EMV_ARC_LEN;	// 0x02
		nOffset++;

		EMVDOLListInfo.TDOLLen = (BYTE)nOffset;

		break;

	case 0 :										// Default
	case 1 :
	default :										// Default
		memcpy(  TAC_Denial, EMV_TA_TAC_00MODE,  sizeof(TAC_Denial) );
		memcpy(  TAC_Online, EMV_TA_TAC_00MODE,  sizeof(TAC_Online) );
		memcpy( TAC_Default, EMV_TA_TAC_00MODE, sizeof(TAC_Default) );

		// Default_DDOL : "9F 02 06 9F 03 06 9F 37 04"
		nOffset = 0;

		memcpy( Default_DDOL + nOffset,
				EMV_TAG_BCDAMOUNT, sizeof(EMV_TAG_BCDAMOUNT) );		// 0x9F02
		nOffset += sizeof(EMV_TAG_BCDAMOUNT);

		Default_DDOL[nOffset] = sizeof(EMVDOLListInfo.TranAmountBCD);	// 0x06
		nOffset++;

		memcpy( Default_DDOL + nOffset,
				EMV_TAG_BCDANAMOUNT, sizeof(EMV_TAG_BCDANAMOUNT) );	// 0x9F03
		nOffset += sizeof(EMV_TAG_BCDANAMOUNT);

		Default_DDOL[nOffset] = sizeof(EMVDOLListInfo.AnotherTranAmountBCD);	// 0x06
		nOffset++;

		memcpy( Default_DDOL + nOffset,
				EMV_TAG_RANDOMNO, sizeof(EMV_TAG_RANDOMNO) );		// 0x9F03
		nOffset += sizeof(EMV_TAG_RANDOMNO);

		Default_DDOL[nOffset] = sizeof(EMVDOLListInfo.UnpredictableNumber);	// 0x04
		nOffset++;

		EMVDOLListInfo.DDOLLen = (BYTE)nOffset;

		// Default_TDOL : "9F 02 06 9F 03 06 8A 02"
		nOffset = 0;

		memcpy( Default_TDOL + nOffset,
				EMV_TAG_BCDAMOUNT, sizeof(EMV_TAG_BCDAMOUNT) );		// 0x9F02
		nOffset += sizeof(EMV_TAG_BCDAMOUNT);

		Default_TDOL[nOffset] = sizeof(EMVDOLListInfo.TranAmountBCD);	// 0x06
		nOffset++;

		memcpy( Default_TDOL + nOffset,
				EMV_TAG_BCDANAMOUNT, sizeof(EMV_TAG_BCDANAMOUNT) );	// 0x9F03
		nOffset += sizeof(EMV_TAG_BCDANAMOUNT);

		Default_TDOL[nOffset] = sizeof(EMVDOLListInfo.AnotherTranAmountBCD);	// 0x06
		nOffset++;

		Default_TDOL[nOffset] = EMV_TAG_ARC;		// 0x8A
		nOffset++;

		Default_TDOL[nOffset] = EMV_ARC_LEN;	// 0x02
		nOffset++;

		EMVDOLListInfo.TDOLLen = (BYTE)nOffset;

		break;
	}

	//**************************************************************************
	// 인증 Data Object List들 Copy
	//**************************************************************************

	// Application Version Number(EMV 2000)(9F09) : AVN은 인증과 운영이 같다.

	// Terminal Action Code - Denial(5F50)
	memcpy( EMVDOLListInfo.TACDenial, TAC_Denial, sizeof(TAC_Denial) );

	// Terminal Action Code - Online(5F51)
	memcpy( EMVDOLListInfo.TACOnline, TAC_Online, sizeof(TAC_Online) );

	// Terminal Action Code - Default(5F52)
	memcpy( EMVDOLListInfo.TACDefault, TAC_Default, sizeof(TAC_Default) );

	// Transaction Floor Limit(9F1B). 10000
	memcpy( EMVDOLListInfo.TranFloorLimit, EMV_TA_TFL, sizeof(EMV_TA_TFL) );

	// Transaction Currency Code(5F2A). 영국(0840)
	memcpy( EMVDOLListInfo.TranCurrencyCode, EMV_TA_TCC_5F2A, sizeof(EMV_TA_TCC_5F2A) );

	// Terminal Country Code(9F1A). 영국(0840)
	memcpy( EMVDOLListInfo.TerminalCountryCode, EMV_TA_TCC_9F1A, sizeof(EMV_TA_TCC_9F1A) );

	// Dynamic Data Authentication Data Object List(5F53)
	memcpy( EMVDOLListInfo.DDOL, Default_DDOL, EMVDOLListInfo.DDOLLen );

	// Transaction Certificate Data Object List(5F54)
	memcpy( EMVDOLListInfo.TDOL, Default_TDOL, EMVDOLListInfo.TDOLLen );

#endif	// #if EMV_TA_MODE

	return TRUE;
}

/*==============================================================================
* Function Name : fnEMV_DefaultDOLSetINI()
* Description   : 단말기에서 Default로 사용할 Data Object List를 INI 파일을
*                 통해서 Setting하는 함수
* Parameters    : (None)
* Return        : Success : TRUE				Fail : FALSE
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_DefaultDOLSetINI()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV_DefaultDOLSetINI()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Default DOL Set INI");
// kh 20041012 추가
	char	szPath[256];
	BYTE	bDOLData[1000];
	int		nDOLLen, nOffset(0);
	int		nReturn(FALSE);
	char		szIniData[1024] = {0x00, };

	memset( szIniData, NULL, sizeof(szIniData) );
	memset(   szPath, NULL,   sizeof(szPath) );
	memset( bDOLData, NULL, sizeof(bDOLData) );

	//#N0268
	m_nEMVTerminalConstantLen = 0;
	memset( m_btEMVTerminalConstant, NULL, sizeof(m_btEMVTerminalConstant));
	
	//**************************************************************************
	// INI File Path Read
	//**************************************************************************
	nReturn = fnEMV_GetINIPath(szPath);

	CString strLog;
	strLog.Format( "CDevCmn::fnEMV_DefaultDOLSetINI() INIPath = %s", szPath );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI():INIPath[%s]", strLog);

	//**************************************************************************
	// INI File Read
	//**************************************************************************
	if ( nReturn == TRUE )
	{
		if ( !GetPrivateProfileString( (LPCTSTR)"TerminalConstant",
									   (LPCTSTR)"LIST",
									   "",
									   szIniData,
									   (DWORD)sizeof(szIniData),
									   (LPCTSTR)szPath ) )
		{
			nReturn = FALSE;
		}
	}

	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() TerminalConstant = %s", szIniData );
	MsgDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI():TerminalConstant[%s]", strLog);

	//**************************************************************************
	// Data Setting
	//**************************************************************************
	if ( nReturn == TRUE )							// File에서 읽은 값 Setting
	{
		nDOLLen = strlen(szIniData);
		MakePack( szIniData, bDOLData, nDOLLen );
		nDOLLen = nDOLLen / 2;

		//#N0268
		memcpy(m_btEMVTerminalConstant, bDOLData, nDOLLen);
		m_nEMVTerminalConstantLen = nDOLLen;
		
		nOffset += IC_FCMS_LEN;						// 업무코드 길이
		nOffset++;									// Total Data Length

		BYTE	bTagFlag = 0x1F;
		LPBYTE	lpDOLData;
		int		nTagLen, nDataLen, nTempLen, nException;

		while ( nOffset < nDOLLen )
		{
			// Tag의 하위 5바이트가 1이면 2바이트의 Tag이다(Ex : 5FXX, 9FXX).
			if ( ( bTagFlag & bDOLData[nOffset] ) == bTagFlag )
				nTagLen = 2;
			else
				nTagLen = 1;

			nException = 0;						// Exception Case Flag

			// Tag에 따를 Data 분류
			// Additional Terminal Capabilitise(9F40)
			if ( memcmp( &bDOLData[nOffset], EMV_TAG_ADDTC, sizeof(EMV_TAG_ADDTC) ) == 0 )
			{
				lpDOLData = EMVDOLListInfo.AddTerminalCapabilitise;
				nDataLen  = sizeof(EMVDOLListInfo.AddTerminalCapabilitise);
			}
			// Application Version Number(EMV 2000)(9F09)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_AVN, sizeof(EMV_TAG_AVN) ) == 0 )
			{
				lpDOLData = EMVDOLListInfo.AppVersionNum;
				nDataLen  = sizeof(EMVDOLListInfo.AppVersionNum);
			}
			// Terminal Action Code - Denial(5F50)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_TAC_DENIAL, sizeof(EMV_TAG_TAC_DENIAL) ) == 0 )
			{
				lpDOLData = EMVDOLListInfo.TACDenial;
				nDataLen  = sizeof(EMVDOLListInfo.TACDenial);
			}
			// Terminal Action Code - Online(5F51)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_TAC_ONLINE, sizeof(EMV_TAG_TAC_ONLINE) ) == 0 )
			{
				lpDOLData = EMVDOLListInfo.TACOnline;
				nDataLen  = sizeof(EMVDOLListInfo.TACOnline);
			}
			// Terminal Action Code - Default(5F52)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_TAC_DEFAULT, sizeof(EMV_TAG_TAC_DEFAULT) ) == 0 )
			{
				lpDOLData = EMVDOLListInfo.TACDefault;
				nDataLen  = sizeof(EMVDOLListInfo.TACDefault);
			}
			// Terminal Capabilitise(9F33)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_TC, sizeof(EMV_TAG_TC) ) == 0 )
			{
				lpDOLData = EMVDOLListInfo.TerminalCapabilitise;
				nDataLen  = sizeof(EMVDOLListInfo.TerminalCapabilitise);
char szLog[200];
CString strLog;
memset( szLog, NULL, sizeof(szLog) );
MakeUnPack( EMVDOLListInfo.TerminalCapabilitise, szLog, sizeof(EMVDOLListInfo.TerminalCapabilitise), TRUE );
strLog.Format( "fnEMV_DefaultDOLSetINI = %s", szLog );
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
			}
			// Transaction Floor Limit(9F1B)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_TFL, sizeof(EMV_TAG_TFL) ) == 0 )
			{
				lpDOLData = EMVDOLListInfo.TranFloorLimit;
				nDataLen  = sizeof(EMVDOLListInfo.TranFloorLimit);
			}
			// Terminal Type(9F35)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_TERMINALTYPE, sizeof(EMV_TAG_TFL) ) == 0 )
			{
				lpDOLData = &EMVDOLListInfo.TerminalType;
				nDataLen  = sizeof(BYTE);
			}
			// Transaction Currency Code(5F2A)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_TRANCC, sizeof(EMV_TAG_TRANCC) ) == 0 )
			{
				lpDOLData = EMVDOLListInfo.TranCurrencyCode;
				nDataLen  = sizeof(EMVDOLListInfo.TranCurrencyCode);
			}
			// Terminal Country Code(9F1A)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_TERMINALCC, sizeof(EMV_TAG_TERMINALCC) ) == 0 )
			{
				lpDOLData = EMVDOLListInfo.TerminalCountryCode;
				nDataLen  = sizeof(EMVDOLListInfo.TerminalCountryCode);
			}
			// Transaction Type(9C)
			else if ( bDOLData[nOffset] == EMV_TAG_TRANTYPE )
			{
				lpDOLData = &EMVDOLListInfo.TransactionType;
				nDataLen  = sizeof(BYTE);
			}
			// Dynamic Data Authentication Data Object List(5F53)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_DDOL, sizeof(EMV_TAG_DDOL) ) == 0 )
			{
				lpDOLData = EMVDOLListInfo.DDOL;
				nDataLen  = sizeof(EMVDOLListInfo.DDOL);
				nException = 1;
			}
			// Transaction Certificate Data Object List(5F54)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_TDOL, sizeof(EMV_TAG_TDOL) ) == 0 )
			{
				lpDOLData = EMVDOLListInfo.TDOL;
				nDataLen  = sizeof(EMVDOLListInfo.TDOL);
				nException = 2;
			}
			// Payment System Enviroment(5F55)
			else if ( memcmp( &bDOLData[nOffset], EMV_TAG_PSE, sizeof(EMV_TAG_PSE) ) == 0 )
			{
				lpDOLData = (LPBYTE)EMVDOLListInfo.PaySysEnviroment;
				nDataLen  = sizeof(EMVDOLListInfo.PaySysEnviroment);
				nException = 3;
			}
			else
			{				
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI() return FALSE");
				return FALSE;
			}

			nOffset += nTagLen;						// Tag 길이 추가

			nTempLen = bDOLData[nOffset];			// File에 읽은 Data Length
			nOffset++;

			// Data Copy
			if ( nException == 1 )					// DDOL
			{
				memset( lpDOLData, NULL, nDataLen );
				EMVDOLListInfo.DDOLLen = nTempLen;
				if ( nTempLen != 0 )
					memcpy( lpDOLData, bDOLData + nOffset, nTempLen );
			}
			else if ( nException == 2 )				// TDOL
			{
				memset( lpDOLData, NULL, nDataLen );
				EMVDOLListInfo.TDOLLen = nTempLen;
				if ( nTempLen != 0 )
					memcpy( lpDOLData, bDOLData + nOffset, nTempLen );
			}
			else if ( nException == 3 )				// Payment System Enviroment
			{
				memset( lpDOLData, NULL, nDataLen );
				if ( nTempLen != 0 )
					memcpy( lpDOLData, bDOLData + nOffset, nTempLen );
			}
			else
			{
				if ( nTempLen == nDataLen )			// 실제 요소와 File의 Size가 같음
				{
					memcpy( lpDOLData, bDOLData + nOffset, nDataLen );
				}
				else if ( nTempLen == 0 )
				{
					memset( lpDOLData, NULL, nDataLen );		// V05-01-04-#06:codesonar 오류지적
				}
				else
				{
					
					return FALSE;
				}
			}
			nOffset += nTempLen;
		}
	}

	char szLog[100];

	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( EMVDOLListInfo.AddTerminalCapabilitise, szLog, sizeof(EMVDOLListInfo.AddTerminalCapabilitise), TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.AddTerminalCapabilitise = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.AddTerminalCapabilitise = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);	

	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( EMVDOLListInfo.AppVersionNum, szLog, sizeof(EMVDOLListInfo.AppVersionNum), TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.AppVersionNum = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.AppVersionNum = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);		

	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( EMVDOLListInfo.TACDenial, szLog, sizeof(EMVDOLListInfo.TACDenial), TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.TACDenial = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.TACDenial = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
	
	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( EMVDOLListInfo.TACOnline, szLog, sizeof(EMVDOLListInfo.TACOnline), TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.TACOnline = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.TACOnline = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
	
	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( EMVDOLListInfo.TACDefault, szLog, sizeof(EMVDOLListInfo.TACDefault), TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.TACDefault = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.TACDefault = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
	
	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( EMVDOLListInfo.TerminalCapabilitise, szLog, sizeof(EMVDOLListInfo.TerminalCapabilitise), TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.TerminalCapabilitise = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.TerminalCapabilitise = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
	
	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( EMVDOLListInfo.TranFloorLimit, szLog, sizeof(EMVDOLListInfo.TranFloorLimit), TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.TranFloorLimit = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.TranFloorLimit = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
	
	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( &EMVDOLListInfo.TerminalType, szLog, sizeof(EMVDOLListInfo.TerminalType), TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.TerminalType = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.TerminalType = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
	
	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( EMVDOLListInfo.TranCurrencyCode, szLog, sizeof(EMVDOLListInfo.TranCurrencyCode), TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.TranCurrencyCode = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.TranCurrencyCode = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
	
	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( &EMVDOLListInfo.TransactionType, szLog, sizeof(EMVDOLListInfo.TransactionType), TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.TransactionType = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.TransactionType = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
	
	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( EMVDOLListInfo.DDOL, szLog, EMVDOLListInfo.DDOLLen, TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.DDOL = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.DDOL = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
	
	memset( szLog, NULL, sizeof(szLog) );
	MakeUnPack( EMVDOLListInfo.TDOL, szLog, EMVDOLListInfo.TDOLLen, TRUE );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.TDOL = %s", szLog);
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.TDOL = %s", szLog );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
	
	strLog.Format( "CDevCmn::EMV_DefaultDOLSetINI() EMVDOLListInfo.PaySysEnviroment = %s", EMVDOLListInfo.PaySysEnviroment );
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVDOLListInfo.PaySysEnviroment = %s", EMVDOLListInfo.PaySysEnviroment);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultDOLSetINI()[%s]", strLog);
	
	return nReturn;
// kh end
}

/*==============================================================================
* Function Name : fnEMV_DefaultAIDListSet()
* Description   : 단말기에서 Default로 지원이 가능한 AID List를 Set하는 함수
* Parameters    : (None)
* Return        : AID List Count
* Release       : Ver 1.0
* Notes			: 2004.04.09 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_DefaultAIDListSet()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultAIDListSet()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Default AID List Set");

	LPBYTE	lpAID;
	LPSTR	lpszBrand;
	int		i;

#if EMV_TA_MODE										// 인증 모드
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultAIDListSet() 인증 모드");

	EMVInfo.AIDListCount = 2;

	BYTE	bAID[EMV_AID_LEN + 1];

	// 인증시 Configuration Code에 상관없이 AID List는 동일하다.
	for ( i = 0; i < EMVInfo.AIDListCount; i++ )
	{
		memset( bAID, NULL, EMV_AID_LEN + 1);
		lpAID = bAID;

		switch (i)
		{
		case 0 :
			memcpy( bAID, "\xA0\x00\x00\x00\x03\x10\x10", EMV_AID_LEN );
			lpszBrand = "Visa Credit or Debit";
			EMVAIDListInfo[i].AIDLen = EMV_AID_LEN;
			break;
		case 1 :
			memcpy( bAID, "\xA0\x00\x00\x00\x99\x90\x90", EMV_AID_LEN );
			lpszBrand = "Non EMV";
			EMVAIDListInfo[i].AIDLen = EMV_AID_LEN;
			break;
//		case 2 :									// 2004년 인증시 제외
//			memcpy( bAID, "\xA0\x00\x00\x00\x03\x10\x10\x00", EMV_AID_LEN + 1 );
//			lpszBrand = "Partial";
//			m_SEMVAIDListInfo[i].AIDLen = EMV_AID_LEN + 1;
//			break;
		default :
			lpszBrand = NULL;
			break;
		}

		if ( lpszBrand == NULL )
			break;

		memcpy( EMVAIDListInfo[i].AID, lpAID, EMVAIDListInfo[i].AIDLen );
		strcpy( EMVAIDListInfo[i].Brand, lpszBrand );
	}
#else
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::EMV_DefaultTerminalListSet() 운영 모드");

	EMVInfo.AIDListCount = EMV_CANDIDATELIST_CNT;

	for ( i = 0; i < EMVInfo.AIDListCount; i++ )
	{
		switch (i)
		{
		case 0 :
			lpAID     = VISA_AID;
			lpszBrand = "VISA";
			break;
		case 1 :
			lpAID     = MASTER_AID;
			lpszBrand = "MASTER CARD";
			break;
		case 2 :
			lpAID     = JCB_AID;
			lpszBrand = "JCB";
			break;
		case 3 :
			lpAID     = D_PAS_AID;
			lpszBrand = "D-PAS";
			break;
		case 4 :
			lpAID     = CUP_AID;
			lpszBrand = "CUP";
			break;	
		case 5 :
			lpAID     = LOCALEMV_AID;
			lpszBrand = "국내 전용 신용/Check";
			break;
		case 6 :
			lpAID     = LOCAL_MC_AID;
			lpszBrand = "LOCAL M/CHIP";
			break;
		case 7 :
			lpAID     = AMX_AID;
			lpszBrand = "AMX";
			break;
		case 8 :
			lpAID     = ELECTRON_AID;
			lpszBrand = "VISA ELECTRON";
			break;
		case 9 :
			lpAID     = PLUS_AID;
			lpszBrand = "PLUS";	
			break;
		case 10 :
			lpAID     = MAESTRO_AID;
			lpszBrand = "MAESTRO";			
			break;
		case 11 :		
			lpAID     = CIRRUS_AID;
			lpszBrand = "CIRRUS";
			break;
		case 12 :								
			lpAID     = CUP_DEBIT_AID;
			lpszBrand = "CUP_DEBIT";
			break;
		case 13 :									
			lpAID     = CUP_QUASI_AID;
			lpszBrand = "CUP_QUASI";
			break;
		default :
			lpszBrand = NULL;
			break;
		}

		if ( lpszBrand == NULL )
			break;

		if(memcmp(lpszBrand, "AMX", 3) == 0 || memcmp(lpszBrand, "CUP", 3) == 0)		//V02-02-00-#13		AMEX카드는 8자리
		{
			EMVAIDListInfo[i].AIDLen = (BYTE) 0x08;			
			memcpy( EMVAIDListInfo[i].AID, lpAID, 8 );
		}
		else
		{
			EMVAIDListInfo[i].AIDLen = (BYTE) EMV_AID_LEN;	// 현재까지 모두 7	
			memcpy( EMVAIDListInfo[i].AID, lpAID, 7 );
		}
		
		strcpy( EMVAIDListInfo[i].Brand, lpszBrand );
	}
#endif	// #if EMV_TA_MODE
	
char szBuff[50];	
for ( i = 0; i < EMVInfo.AIDListCount; i++ )
{
	memset( szBuff, NULL, sizeof(szBuff) );
	MakeUnPack( EMVAIDListInfo[i].AID, szBuff, EMVAIDListInfo[i].AIDLen, TRUE );				
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::EMV_DefaultTerminalListSet() m_pMcu->EMVAIDListInfo[%d].AID = %s", i, szBuff);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultAIDListSet() EMVAIDListInfo[%d].Brand = %s", i, EMVAIDListInfo[i].Brand);
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "CTranCmn::EMV_DefaultTerminalListSet() m_pMcu->EMVAIDListInfo[%d].AID = %s", i, szBuff);
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "fnEMV_DefaultAIDListSet() EMVAIDListInfo[%d].Brand = %s", i, EMVAIDListInfo[i].Brand);
}

	return i;
}

/*==============================================================================
* Function Name : fnEMV_DefaultAIDListSetINI()
* Description   : 단말기에서 Default로 지원이 가능한 AID List를 INI File INI
*                 파일을 통해서 Setting하는 함수
* Parameters    : (None)
* Return        : Success : TRUE				Fail : FALSE
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_DefaultAIDListSetINI()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultAIDListSetINI()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Default AID List Set INI");

// kh 20041012 제일은행 소스 긁어옴 수정 필요할지도
	char	szIniData[1024] = {0x00, };
	char	szPath[256];
	BYTE	bListData[1000];
	int		nListLen, nOffset(0);
	int		i;
	int		nReturn(FALSE);

	memset( szIniData, NULL,  sizeof(szIniData) );
	memset(    szPath, NULL,    sizeof(szPath) );
	memset( bListData, NULL, sizeof(bListData) );

	//**************************************************************************
	// INI File Path Read
	//**************************************************************************
	nReturn = fnEMV_GetINIPath(szPath);

	CString strLog;
	strLog.Format( "CDevCmn::fnEMV_DefaultAIDListSetINI() INIPath = %s", szPath );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultAIDListSetINI()[%s]", strLog);

	//**************************************************************************
	// INI File Read
	//**************************************************************************
	if ( nReturn == TRUE )
	{
		if ( !GetPrivateProfileString( (LPCTSTR)"TerminalList",
									   (LPCTSTR)"LIST",
									   "",
									   szIniData,
									   sizeof(szIniData),
									   (LPCTSTR)szPath ) )
		{
			nReturn = FALSE;
		}
	}

	strLog.Format( "CDevCmn::fnEMV_DefaultAIDListSetINI() TerminalList = %s", szIniData );
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultAIDListSetINI()[%s]", strLog);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultAIDListSetINI(): nReturn [%d]", nReturn);
	MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "TerminalList = %s", szIniData);
	//**************************************************************************
	// Data Setting
	//**************************************************************************
	if ( nReturn == TRUE )							// File에서 읽은 값 Setting
	{
		nListLen = strlen(szIniData);
		MakePack( szIniData, bListData, nListLen );
		nListLen = nListLen / 2;

		nOffset += IC_FCMS_LEN;						// 업무코드 길이

		EMVInfo.AIDListCount = bListData[nOffset];			// AID Count
		nOffset++;

		//**********************************************************************
		// EMV 거래를 지원하는 AID List는 인증시엔 3개이며 운영시에는 9개이다.
		// 운영시 List의 AID가 우리나라의 단말기가 지원해야 할 9개의 목록과
		// 부합하지 않으면 지원할 의무는 없다.
		//**********************************************************************
#if EMV_TA_MODE										// 인증 모드
		BYTE TA_AID1[EMV_AID_LEN]     = { 0xA0, 0x00, 0x00, 0x00, 0x03, 0x10, 0x10 };
		BYTE TA_AID2[EMV_AID_LEN]     = { 0xA0, 0x00, 0x00, 0x00, 0x99, 0x90, 0x90 };
//		BYTE TA_AID3[EMV_AID_LEN + 1] = { 0xA0, 0x00, 0x00, 0x00, 0x03, 0x10, 0x10, 0x00 };
#endif	// #if EMV_TA_MODE

		for ( i = 0; i < EMVInfo.AIDListCount; i++ )
		{
			EMVAIDListInfo[i].AIDLen = bListData[nOffset];	// AID Length
			nOffset++;
			if (           EMVAIDListInfo[i].AIDLen > IC_AID_LEN ||
				 nOffset + EMVAIDListInfo[i].AIDLen > nListLen   )
				return FALSE;						// Data Error

			memcpy( EMVAIDListInfo[i].AID,
					bListData + nOffset,
					EMVAIDListInfo[i].AIDLen );	// AID
			nOffset += EMVAIDListInfo[i].AIDLen;

			// Brand
#if EMV_TA_MODE										// 인증 모드
			if ( memcmp( EMVAIDListInfo[i].AID, TA_AID1, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "Visa Credit or Debit" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, TA_AID2, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "Non EMV" );
			}
//			else if ( memcmp( m_SEMVAIDListInfo[i].AID, TA_AID3, EMV_AID_LEN + 1 ) == 0 )
//			{
//				strcpy( m_SEMVAIDListInfo[i].Brand, "Partial" );
//			}
#else
			if ( memcmp( EMVAIDListInfo[i].AID, VISA_AID, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "VISA" );
			}
#endif	// #if EMV_TA_MODE
			else if ( memcmp( EMVAIDListInfo[i].AID, ELECTRON_AID, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "VISA ELECTRON" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, PLUS_AID, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "PLUS" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, MASTER_AID, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "MASTER CARD" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, MAESTRO_AID, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "MAESTRO" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, CIRRUS_AID, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "CIRRUS" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, JCB_AID, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "JCB" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, LOCALEMV_AID, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "국내 전용 신용/Check" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, LOCAL_MC_AID, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "LOCAL M/CHIP" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, FIC_AID, EMV_AID_LEN ) == 0 )
			{
				strcpy( EMVAIDListInfo[i].Brand, "국내 금융공동망" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, AMX_AID, 8 ) == 0 )			//V02-02-00-#13	AMEX AID추가
			{
				strcpy( EMVAIDListInfo[i].Brand, "AMX" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, D_PAS_AID, EMV_AID_LEN ) == 0 )			//D-PAS
			{
				strcpy( EMVAIDListInfo[i].Brand, "D-PAS" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, LOCAL_MC_AID, EMV_AID_LEN ) == 0 )			//D-PAS
			{
				strcpy( EMVAIDListInfo[i].Brand, "LOCAL M/CHIP" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, CUP_DEBIT_AID, EMV_AID_LEN ) == 0 )			//D-PAS
			{
				strcpy( EMVAIDListInfo[i].Brand, "CUP_DEBIT" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, CUP_AID, EMV_AID_LEN ) == 0 )			//D-PAS
			{
				strcpy( EMVAIDListInfo[i].Brand, "CUP" );
			}
			else if ( memcmp( EMVAIDListInfo[i].AID, CUP_QUASI_AID, EMV_AID_LEN ) == 0 )			//D-PAS
			{
				strcpy( EMVAIDListInfo[i].Brand, "CUP_QUASI" );
			}
			else
			{
				return FALSE;						// Data Error
			}
		}
	}

	for ( i = 0; i < EMVInfo.AIDListCount; i++ )
	{

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVAIDListInfo[i].AID", EMVAIDListInfo[i].AID, sizeof(EMVAIDListInfo[i].AID));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_DefaultAIDListSetINI() Brand [%s]", EMVAIDListInfo[i].Brand );
HexDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMVAIDListInfo[i].AID", EMVAIDListInfo[i].AID, sizeof(EMVAIDListInfo[i].AID));
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "fnEMV_DefaultAIDListSetINI() Brand [%s]", EMVAIDListInfo[i].Brand );
	}

	return nReturn;
// kh end
}

// 20041012 kh 추가
/*==============================================================================
* Function Name : fnEMV_GetINIPath()
* Description   : EMV 정보를 가진 INI File의 Path Get
* Parameters    : lpstrPath : INI File의 Path
* Return        : Success : TRUE				Fail : FALSE
* Release       : Ver 1.0
* Notes			: 2004.04.27 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_GetINIPath(LPSTR lpstrPath)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_GetINIPath");
	unsigned long	lQueryResult;
	HKEY			hRegKey = NULL;								// V08-04-01:CodeSonar
	int				nReturn(FALSE);

	DWORD	dwRegSZ    = REG_SZ;
	DWORD	dwPathSize = 255;

	lQueryResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
								 _T("SOFTWARE\\ATM\\DEVINFO\\IDC"),
								 0,
								 KEY_ALL_ACCESS,
								 &hRegKey );

	if ( lQueryResult == ERROR_SUCCESS )
	{										// Registry에 정보가 있다면 읽어서 Set
		lQueryResult = RegQueryValueEx( hRegKey, TEXT("EMVInitParam"), 0,
							  &dwRegSZ, (LPBYTE)lpstrPath, &dwPathSize );
		if ( lQueryResult != ERROR_SUCCESS )
			lQueryResult = RegQueryValueEx( hRegKey, TEXT("EMVInitParam"),
								  0, &dwRegSZ, (LPBYTE)lpstrPath, &dwPathSize );
	}

	if (hRegKey) RegCloseKey(hRegKey);							// V08-05-01:Codesonar

	if ( lQueryResult == ERROR_SUCCESS )
		nReturn = TRUE;

	return nReturn;
}
// 추가 end

/*==============================================================================
* Function Name : fnEMV_MakeReqCandidate()
* Description   : fnEMV_ReqCandidateList()에 사용할 데이터 목록 조립하는 함수
* Parameters    : lpList : EMV Kernel로 보낼 Data List
* Return        : Success : Length of lpList
* Release       : Ver 1.0
* Notes			: 2004.06.21 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_MakeReqCandidate(LPBYTE lpList)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeReqCandidate()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Make Request Candidate");

	char	szDate[20], szTime[20], szCount[20];
	int		nOffset(0), nLength, nCount;

	//**************************************************************************
	//  필요 Data 구하기
	//**************************************************************************
	// 시간과 날짜 얻어오기
	memset( szDate, 0x00, sizeof(szDate) );			// Ex : "20040412"
	memset( szTime, 0x00, sizeof(szTime) );			// Ex : "145636"
	GetDateTime( szDate, szTime );					// YYYYMMDD, HHMMSS
//	sprintf(m_pTranCmn->Accept.CardReadTime, "%4.4s%6.6s", &szDate[2], &szTime[0]);
	
	MakePack( &szDate[2],
			  EMVDOLListInfo.TransactionDate,
			  sizeof(EMVDOLListInfo.TransactionDate) * 2 );
	MakePack( &szTime[0],
			  EMVDOLListInfo.TransactionTime,
			  sizeof(EMVDOLListInfo.TransactionTime) * 2 );

	// Create Unpredictable Number
	fnFIC_MakeRandom( EMVDOLListInfo.UnpredictableNumber,
					  sizeof(EMVDOLListInfo.UnpredictableNumber) );

	// Transaction Sequence Counter.
	// TRANS.INI : [TRANS] : EMVSerialNo : 1 ~ 0x0FFFFFFF
	memset( szCount, 0x00, sizeof(szCount) );		// Ex : "00001234"
	m_pProfile->GetTransProfile();					// Profile Read
	nCount = m_pProfile->TRANS.EMVSerialNo;			// 정수값
//	nCount = 1;
	wsprintf( szCount, "08X", nCount++ );			// Little Endian -> Big Endian
	MakePack( &szCount[0],
			  EMVDOLListInfo.TranSequenceCounter,
			  sizeof(EMVDOLListInfo.TranSequenceCounter) * 2 );
	// 증가된 Count 값이 너무 크면 초기화
	m_pProfile->TRANS.EMVSerialNo = ( nCount >= 0x0FFFFFFF ) ? 1 : nCount;
	m_pProfile->PutTransProfile();					// Profile Write

	//**************************************************************************
	//  Construct Message
	//**************************************************************************
	// Transaction Date(9A)
	nLength = fnICA_ConstructTLV( lpList + nOffset,
						EMV_TAG_TRANDATE,
						EMVDOLListInfo.TransactionDate,
						sizeof(EMVDOLListInfo.TransactionDate) );
	nOffset += nLength;

	// Transaction Time(9F21)
	nLength = fnICA_ConstructTLV( lpList + nOffset,
						EMV_TAG_TRANTIME,
						sizeof(EMV_TAG_TRANTIME),
						EMVDOLListInfo.TransactionTime,
						sizeof(EMVDOLListInfo.TransactionTime) );
	nOffset += nLength;

	// Unpredictable Number(9F37)
	nLength = fnICA_ConstructTLV( lpList + nOffset,
						EMV_TAG_RANDOMNO,
						sizeof(EMV_TAG_RANDOMNO),
						EMVDOLListInfo.UnpredictableNumber,
						sizeof(EMVDOLListInfo.UnpredictableNumber) );
	nOffset += nLength;

	// Transaction Sequence Counter(9F41)
	nLength = fnICA_ConstructTLV( lpList + nOffset,
						EMV_TAG_TSC,
						sizeof(EMV_TAG_TSC),
						EMVDOLListInfo.TranSequenceCounter,
						sizeof(EMVDOLListInfo.TranSequenceCounter) );
	nOffset += nLength;

	return nOffset;
}

/*==============================================================================
* Function Name : EMV_MakeOfflineDataAuthen()
* Description   : EMV_OfflineDataAuthentication()에 사용할 데이터 목록을 조립하는 함수
* Parameters    : lpList           : EMV Kernel로 보낼 Data List
*                 szAmount         : 입력받은 금액(Ex : "0000010000")
*                 AmountLen        : Length of szAmount
*                 szAnotherAmount  : 입력받은 기타금액(로열티 등)(Ex : "0000010000")
*                 AnotherAmountLen : Length of szAnotherAmount
* Return        : Success : Length of lpList
* Release       : Ver 1.0
* Notes			: 2004.06.21 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_MakeOfflineDataAuthen(LPBYTE lpList, char* szAmount,
					int AmountLen, char* szAnotherAmount, int AnotherAmountLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeOfflineDataAuthen()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Make Offline Data Authen");

	char	szTemp[30];
	int		nAmount;
	BYTE	bTC;
	int		nOffset(0), nLength;

	//**************************************************************************
	//  필요 Data 구하기
	//**************************************************************************
	// szAmount 변환
	memset( szTemp, NULL, sizeof(szTemp) );
	memcpy( szTemp, szAmount, AmountLen );			// NULL String이 아닐 경우 대비

	nAmount = atoi(szTemp);							// 정수형으로 형변환(Ex : 10000)

	// Transaction Amount(BCD)
	memset( szTemp, NULL, sizeof(szTemp) );
	sprintf( szTemp, "%012d", nAmount );			// 문자열로 형변환(Ex : "000000010000")
	
	MakePack( szTemp, EMVDOLListInfo.TranAmountBCD, strlen(szTemp) );

	// Transaction Amount(Hexadecimal)
	memset( szTemp, NULL, sizeof(szTemp) );
	sprintf( szTemp, "%08x", nAmount );				// 문자열로 형변환(Ex : "00002710")

	MakePack( szTemp, EMVDOLListInfo.TranAmountHexa, strlen(szTemp) );

	// szAnotherAmount 변환
	memset( szTemp, NULL, sizeof(szTemp) );
	memcpy( szTemp, szAnotherAmount, AnotherAmountLen );	// NULL String이 아닐 경우 대비

	nAmount = atoi(szTemp);							// 정수형으로 형변환(Ex : 0)

	// Another Transaction Amount(BCD)
	memset( szTemp, NULL, sizeof(szTemp) );
	sprintf( szTemp, "%012d", nAmount );			// 문자열로 형변환(Ex : "000000000000")
	
	MakePack( szTemp, EMVDOLListInfo.AnotherTranAmountBCD, strlen(szTemp) );

	// Another Transaction Amount(Hexadecimal)
	memset( szTemp, NULL, sizeof(szTemp) );
	sprintf( szTemp, "%08x", nAmount );				// 문자열로 형변환(Ex : "00000000")

	MakePack( szTemp, EMVDOLListInfo.AnotherTranAmountHexa, strlen(szTemp) );

	//**************************************************************************
	// Transaction Code
	// +-----------------+--------------------------------+
	// | 서비스 일반거래 |   Transaction Code(9C) = 0x01  |
	// | PIN Change      |   Transaction Code(9C) = 0x70  |
	// | PIN Unblock     |   Transaction Code(9C) = 0x72  |
	// +-----------------+--------------------------------+
	//**************************************************************************
//	if ( TranCode == 29 )							// PIN Change
//	{
//		bTC = EMV_DDO_TC_PINCHANGE;					// 0x70
//	}
//	else if ( TranCode == 255 )						// PIN Unblock. 아직 거래 없음.
//	{
//		bTC = EMV_DDO_TC_PINUNBLOCK;				// 0x72
//	}
//	else
//	{
		bTC = EMVDOLListInfo.TransactionType;		// 0x01
//	}

	//**************************************************************************
	//  Construct Message
	//**************************************************************************
	// Transaction Amount(BCD)(9F02)
	nLength = fnICA_ConstructTLV( lpList + nOffset,
					EMV_TAG_BCDAMOUNT,
					sizeof(EMV_TAG_BCDAMOUNT),
					EMVDOLListInfo.TranAmountBCD,
					sizeof(EMVDOLListInfo.TranAmountBCD) );
	nOffset += nLength;

	// Transaction Amount(Hexadecimal)(81)
	nLength = fnICA_ConstructTLV( lpList + nOffset,
					EMV_TAG_HEXAAMOUNT,
					EMVDOLListInfo.TranAmountHexa,
					sizeof(EMVDOLListInfo.TranAmountHexa) );
	nOffset += nLength;

#if EMV_TA_MODE		// 인증 모드
////////////////////////////////////////////////////////////////////////////
//  기타 금액 부분은 인증시에만 사용한다. 실제 운영시에는 사용하지 않는다.
//  이는 카드의 Application Usage Control과 충돌하기 때문이다. 현재는 카드의
//  AUC가 FF00으로 세팅되어 있다. 만약 카드의 AUC가 FF80 또는 FFC0로 세팅되
//  있다면 내려도 상관없다. 20040618-HSW-A
////////////////////////////////////////////////////////////////////////////
	
	// Another Transaction Amount(BCD)(9F03)
	nLength = fnICA_ConstructTLV( lpList + nOffset,
					EMV_TAG_BCDANAMOUNT,
					sizeof(EMV_TAG_BCDANAMOUNT),
					EMVDOLListInfo.AnotherTranAmountBCD,
					sizeof(EMVDOLListInfo.AnotherTranAmountBCD) );
	nOffset += nLength;

	// Another Transaction Amount(Hexadecimal)(9F04)
	nLength = fnICA_ConstructTLV( lpList + nOffset,
					EMV_TAG_HEXAANAMOUNT,
					sizeof(EMV_TAG_HEXAANAMOUNT),
					EMVDOLListInfo.AnotherTranAmountHexa,
					sizeof(EMVDOLListInfo.AnotherTranAmountHexa) );
	nOffset += nLength;
#endif	// #if EMV_TA_MODE

	// Transaction Type(9C)
	nLength = fnICA_ConstructTLV( lpList + nOffset, EMV_TAG_TRANTYPE,
								  &bTC, sizeof(BYTE) );
	nOffset += nLength;

	return nOffset;
}

/*==============================================================================
* Function Name : EMV_MakeOfflineDataAuthen()
* Description   : EMV_OnlineProcessing()에 사용할 데이터 목록을 조립하는 함수
* Parameters    : lpEMVResp    : EMV Kernel로 보낼 Host 응답 Data
*                 lpszHostData : Host에서 응답받은 EMV Data
*                 nHostResp    : 거래에 대한 Host의 성공, 실패 응답
*                 HostRecvFlag : Host 응답을 받았는지 송수신 에러인지를 판단.
*                                Host 응답을 받았지만 ARC가 없는 경우에는
*                                nHostResp를 통해 ARC를 만들어준다.
* Return        : Success : Length of lpList
* Release       : Ver 1.0
* Notes			: 2004.06.21 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_MakeOnlineProcessing(LPBYTE lpEMVResp, LPSTR lpszHostData,
									  int nHostResp, BOOL HostRecvFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeOnlineProcessing()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Make Online Processing");

	//**************************************************************************
	//  Data Analysis
	//--------------------------------------------------------------------------
	//  1. lpszHostData의 구성
	//     ARPC(91)(16) + ARC(8A)(4) + ATC(9F36)(4) + Issuer Script(72)(Max. 168)
	//     각 요소들은 생략 가능.
	//  2. Field39의 Host 응답과 EMV 응답 데이터의 성공 실패는 같아야 한다.
	//     nHostResp = Success   : ARC = 0x30 0x30
	//     nHostResp = Error     : ARC = 0x30 0x35
	//     nHostResp = Incorrect : ARC = 0x30 0x32(이 Case의 존재 여부는 은행과 상의하여야 한다)
	//     nHostResp = Referral  : ARC = 0x30 0x31(이 Case의 존재 여부는 부정적임)
	//  3. 은행 사양에 따라 이 ARC의 값들이 달라질 수도 있다.
	//     이 때는 2를 참조하여 변환하여야 한다. EP는 이 값들만을 인식한다.
	//  4. EMV 응답 데이터가 없는 경우 호스트 성공과 실패 여부를 확인하여 ARC
	//     값을 만들어 EP로 던져준다. EP는 판단의 근거를 모르기 때문이다.
	//  5. 공동망 전문 사양에서 ARC의 값은 EBCDIC이다. 만약 ARC가 EBCDIC으로
	//     전달되면 EP에는 ASCII로 변환하여 전달한다.
	//  6. 만약 송수신 에러가 났다면 ARC를 만들어주지 않는다.
	//**************************************************************************
	
#if EMV_TA_MODE		// 인증 모드

	BYTE	bHostData[IC_APDU_LEN], bARC[10];
	int		nOffset(0), nLength;

	memset( bHostData, NULL,  IC_APDU_LEN );
	memset(     bARC, NULL, sizeof(bARC));

	// Host Return Data의 크기 구하기
	nLength = strlen(lpszHostData);
	MakePack(lpszHostData, bHostData, nLength);
	nLength = nLength / 2;

	// ARPC 확인(91)
	if (bHostData[nOffset] == EMV_TAG_ARPC)
	{
		nOffset = nOffset + 10;						// Tag(1) + Length(1) + Value(8)
		memcpy(lpEMVResp, bHostData, nOffset);	// 리턴 데이터에 복사
	}

	// ARC 확인(8A)
	if (bHostData[nOffset] == EMV_TAG_ARC)		// ARC 확인
	{
		// ARC Object Save
		memcpy(EMVDOLListInfo.AuthResponseCode,
				&bHostData[nOffset + 2], EMV_ARC_LEN);

		// EP Send Data에 나머지 Data Copy
		memcpy(&lpEMVResp[nOffset], &bHostData[nOffset], nLength - nOffset);
	}
	else
	{
		char szInput[20], szOutput[20];				// Temp Buffer for Data Convert
		memset( szInput, NULL,  sizeof(szInput));
		memset(szOutput, NULL, sizeof(szOutput));
		memcpy(szInput, &lpszHostData[nOffset * 2], 8);
		
		// EBCDIC 여부 확인
		CodeCvt(szInput, szOutput, strlen(szInput), EBCASC);
		MakePack(szOutput, bARC, strlen(szOutput));

		if (bARC[0] == EMV_TAG_ARC)				// ARC가 EBCDIC으로 내려왔음
		{
			memcpy(EMVDOLListInfo.AuthResponseCode, &bARC[2], EMV_ARC_LEN);

			memcpy(&bHostData[nOffset], bARC, 4);	// Host 응답 데이터 Change

			// 리턴 데이터에 나머지 데이터 복사
			memcpy(&lpEMVResp[nOffset], &bHostData[nOffset], nLength - nOffset);
		}
		else										// EBCDIC도 아니라면 없는 것
		{
			// Host의 응답 데이터에서 ARC가 없을 때에만 세팅. 송수신 에러에서는 만들어주지 않는다.
			if (HostRecvFlag == TRUE)
			{
				bARC[0] = EMV_TAG_ARC;				// 0x8A
				bARC[1] = EMV_ARC_LEN;				// 0x02

				switch (nHostResp)
				{
				case 0 :							// Fail("05")
					memcpy(&bARC[2], EMV_ARC_DECLINE, sizeof(EMV_ARC_DECLINE));
					break;

				case 1 :							// Success("00")
					memcpy(&bARC[2], EMV_ARC_SUCCESS, sizeof(EMV_ARC_SUCCESS));
					break;
	
				case 2 :							// Incorrect("02")
					memcpy(&bARC[2], EMV_ARC_REVERSAL, sizeof(EMV_ARC_REVERSAL));
					break;
	
				case 3 :							// Referral("01")
					memcpy(&bARC[2], EMV_ARC_REFERRAL, sizeof(EMV_ARC_REFERRAL));
					break;
	
				default :
					break;
				}

				// 새로 조립된 ARC Data 추가
				memcpy(&lpEMVResp[nOffset], bARC, 4);
				
				// 리턴 데이터에 나머지 데이터 복사
				memcpy(&lpEMVResp[nOffset + 4],
						&bHostData[nOffset], nLength - nOffset);
				nLength = nLength + 4;				// ARC Length 추가
			}
			else
			{
				// 리턴 데이터에 나머지 데이터 복사
				memcpy(&lpEMVResp[nOffset],
						&bHostData[nOffset], nLength - nOffset);
			}
		}
	}

	//**************************************************************************
	//  Construct Message : Message Length(2) + Message(N)
	//**************************************************************************
	memset(bHostData,      NULL, IC_APDU_LEN);
	memcpy(bHostData, lpEMVResp,     nLength);	// 임시 Buffer Copy

	char szMsgLen[10];
	BYTE bMsgLen[10];
	wsprintf(szMsgLen, "%04X", nLength);			// Message Length는 Big Endian

	MakePack(szMsgLen, bMsgLen, strlen(szMsgLen));
	nOffset = 2;

	memcpy(          lpEMVResp,   bMsgLen, nOffset);	// Message Length(2)
	memcpy(lpEMVResp + nOffset, bHostData, nLength);	// Message(N)
	nLength += nOffset;

#else	// #if EMV_TA_MODE

	// BMT Mode
	int nLength = 0;
	// 여기에는 온라인 에러시 호스트 응답 데이터가 오지 않았을 경우의 처리가
	// 제대로 되어 있지 않다. 이 부분은 은행과 BC사의 협의가 필요하다.

	if (HostRecvFlag != TRUE)
	{
		return nLength;
	}

	if ((nHostResp != TRUE)&&					// Host OK
		  (_tcslen(lpszHostData) < 21))
	{
		lpEMVResp[0] = EMV_TAG_ARC;				// ARC Tag(8A)
		lpEMVResp[1] = EMV_ARC_LEN;				// ARC Length

		switch (nHostResp)
		{
		case 0 :								// Fail("05")
			memcpy(&lpEMVResp[2], EMV_ARC_DECLINE, sizeof(EMV_ARC_DECLINE));
			break;

		case 1 :								// Success("00")
			memcpy(&lpEMVResp[2], EMV_ARC_SUCCESS, sizeof(EMV_ARC_SUCCESS));
			break;
	
		case 2 :								// Incorrect("02")
			memcpy(&lpEMVResp[2], EMV_ARC_REVERSAL, sizeof(EMV_ARC_REVERSAL));
			break;
	
		case 3 :								// Referral("01")
			memcpy(&lpEMVResp[2], EMV_ARC_REFERRAL, sizeof(EMV_ARC_REFERRAL));
			break;
	
		default :
			break;
		}

		nLength = 4;
		return nLength;
	}

	BYTE	bHostData[IC_APDU_LEN];
	BYTE	bARPC[10], bARC[10], bATC[10], bScript[100];
	int		nOffset(0), nScriptLen; //nLength
	char	szScriptLen[10];
	char	ARCUnpack[10] = {0,};
	int		nARPCLen = 0;

	memset(  bHostData, NULL,         IC_APDU_LEN);
	memset(      bARPC, NULL,       sizeof(bARPC));
	memset(       bARC, NULL,        sizeof(bARC));
	memset(       bATC, NULL,        sizeof(bATC));
	memset(    bScript, NULL,     sizeof(bScript));
	memset(szScriptLen, NULL, sizeof(szScriptLen));

	if ((memcmp(EMVCandidateList[EMVInfo.TranIndex].AID, D_PAS_AID,	 sizeof(D_PAS_AID)) == 0)   ||	// D-PAS
		(memcmp(EMVCandidateList[EMVInfo.TranIndex].AID, LOCAL_MC_AID, sizeof(LOCAL_MC_AID)) == 0))
	{																							// V08-03-01-#03:D-PAS 강제처리
		nLength = ( EMV_MCHIP_ARQC_LEN + EMV_ARC_LEN ) * 2;
		// ARC(4)를 위해 "3030" 편집
		strcpy(&lpszHostData[20], "3030");
		nARPCLen = EMV_MCHIP_ARQC_LEN;							// ARPC 길이 설정	
	}
	else
	if (memcmp(EMVCandidateList[EMVInfo.TranIndex].AID, MASTER_AID, sizeof(MASTER_AID)) == 0)
	{															// Master Card
		if (EMVDOLListInfo.IADLen == EMV_M4_IAD_LEN)			// M/Chip4
		{
			nLength = ( EMV_MCHIP_ARQC_LEN + EMV_ARC_LEN ) * 2;
			// ARC(4)를 위해 "3030" 편집
			memcpy(&lpszHostData[20],"3030",4);
			nARPCLen = EMV_MCHIP_ARQC_LEN;							// ARPC 길이 설정
		}
		else 
		{
			nLength = ( EMV_ARQC_LEN + EMV_ARC_LEN ) * 2;			
			nARPCLen = EMV_ARQC_LEN;								// ARPC 길이 설정	
		}
	}
	else														// Visa Card
	{
		nLength = ( EMV_ARQC_LEN + EMV_ARC_LEN ) * 2;	
		nARPCLen = EMV_ARQC_LEN;								// ARPC 길이 설정	
	}


	// Host Return Data의 크기 구하기
	memcpy( szScriptLen, &lpszHostData[nLength], 3);
	nScriptLen = Asc2Int(szScriptLen, 3);						// VAN - Spec (Kim.Gi.Jin)

	nScriptLen = __min(nScriptLen, 72*2); 	// VAN - Spec (Kim.Gi.Jin)

	//Data 보정 Issuer Script (Issuer Length -> 3Byte) 
	memcpy(&lpszHostData[nLength], &lpszHostData[nLength + 3], nScriptLen);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeOnlineProcessing() lpszHostData IusserScriptLen[nLength]=[%3.3s]", &lpszHostData[nLength]);

	MakePack( lpszHostData, bHostData, nLength );

	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeOnlineProcessing() bHostData", bHostData, nLength);


	if (nScriptLen == 0);
	else nScriptLen = nScriptLen / 2;


	// ARPC(91)
	memcpy( bARPC, bHostData + nOffset, nARPCLen);
	nOffset += nARPCLen;
	
	// ARC(8A)
	memcpy( bARC, bHostData + nOffset,  EMV_ARC_LEN );
	nOffset += EMV_ARC_LEN;

	// Issuer Script Length
//	nOffset++;

	// Issuer Script(72)										// V05-04-03-#06:codesonar
	memcpy( bScript, bHostData + nOffset, __min(sizeof(bScript), nScriptLen) );
	nOffset += nScriptLen;

	//**************************************************************************
	//  Construct Message : Message Length(2) + Message(N)
	//**************************************************************************
	nOffset = 2;								// Message Length

	// ARPC(91)
	nLength = fnICA_ConstructTLV( lpEMVResp + nOffset, EMV_TAG_ARPC,
								  bARPC, EMV_ARQC_LEN );
	nOffset += nLength;

	MsgDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeOnlineProcessing() bARPC = [%-8.8s], nOffset = [%d]", bARPC, nOffset);
	HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "lpEMVResp", lpEMVResp, 100);
	// ARC(8A)
	nLength = fnICA_ConstructTLV( lpEMVResp + nOffset, EMV_TAG_ARC,
								  bARC, EMV_ARC_LEN );
	nOffset += nLength;

	MsgDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeOnlineProcessing() bARC = [%-4.4s], nOffset = [%d]", bARC, nOffset);
	HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "lpEMVResp", lpEMVResp, 100);
	
/*
	// ATC(9F36)
	nLength = fnICA_ConstructTLV( lpEMVResp + nOffset, EMV_TAG_APPTRANCOUNT,
							sizeof(EMV_TAG_APPTRANCOUNT), bATC, EMV_ATC_LEN );
	nOffset += nLength;
*/
	MsgDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeOnlineProcessing() bATC = [%-4.4s], nOffset = [%d]", bATC, nOffset);
	HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "lpEMVResp", lpEMVResp, 100);

	// Issuer Script(72)										// V05-04-03-#06:CodeSonar
	memcpy( lpEMVResp + nOffset, bScript, __min(sizeof(bScript), nScriptLen) );
	nOffset += nScriptLen;

	// Message Length
	char szMsgLen[10] = {0x00,};
	nLength = nOffset - 2;
	wsprintf( szMsgLen, "%04X", nLength );			// Message Length는 Big Endian

	MakePack( szMsgLen, lpEMVResp, 4 );

	nLength = nOffset;

	MsgDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeOnlineProcessing() nOffset = [%d]", nOffset);
	HexDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "lpEMVResp", lpEMVResp, 100);
	HexDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "lpEMVResp", lpEMVResp, 100);

#endif	// #if EMV_TA_MODE

	return nLength;
}

#if EMV_TA_MODE		// 인증 모드
/*==============================================================================
* Function Name : fnEMV_MakeSecurePINPAN()
* Description   : PIN과 PAN 정보를 이용한 랜덤값을 생성하는 함수(인증용)
* Parameters    : strPIN  : 고객이 입력한 PIN 정보
*                 nPINLen : Length of strPIN. Default 4.
* Return        : TRUE
* Release       : Ver 1.0
* Notes			: 2004.06.24 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_MakeSecurePINPAN(char* strPIN, int nPINLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeSecurePINPAN()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Make Secure PINPAN");

	BYTE	bTemp[50];
	int		nLen;

	memset( EMVInfo.EncryptPIN, NULL, IC_PIN_LEN );
	memset( EMVInfo.GetDOLData, NULL, sizeof(EMVInfo.GetDOLData) );
	memset( bTemp, NULL, sizeof(bTemp) );

	memcpy( EMVInfo.EncryptPIN, strPIN, nPINLen );

	// Return Data : PIN Block(16) + PAN 정보(16)
	fnCIC_MakeRandom( EMVInfo.EncryptPIN, IC_PIN_LEN );

	memcpy( bTemp, EMVInfo.EncryptPIN, IC_PIN_LEN );
	nLen = IC_PIN_LEN;

	memcpy( bTemp + nLen, EMVDOLListInfo.PAN, EMVDOLListInfo.PANLen );
	nLen += EMVDOLListInfo.PANLen;

	MakeUnPack( bTemp, EMVInfo.GetDOLData, nLen );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeSecurePINPAN() EMVInfo.GetDOLData = %s", EMVInfo.GetDOLData);
MsgDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMV_MakeSecurePINPAN() EMVInfo.GetDOLData = %s", EMVInfo.GetDOLData);

	return T_OK;
}
#endif	// #if EMV_TA_MODE


////////////////////////////////////////////////////////////////////////////////
//  Set/Get Function
////////////////////////////////////////////////////////////////////////////////

/*==============================================================================
* Function Name : fnEMV_GetConfigCode()
* Description   : Configuration Code Get
* Parameters    : (None)
* Return        : EMVInfo.ConfigCode
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
BYTE CDevCmn::fnEMV_GetConfigCode()
{
	return EMVInfo.ConfigCode;
}

/*==============================================================================
* Function Name : fnEMV_SetConfigCode()
* Description   : Configuration Code Set
* Parameters    : ConfigCode
* Return        : (NONE)
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
void CDevCmn::fnEMV_SetConfigCode(BYTE ConfigCode)
{
	EMVInfo.ConfigCode = ConfigCode;
}

/*==============================================================================
* Function Name : fnEMV_GetAIDListCount()
* Description   : AID List Count Get
* Parameters    : (None)
* Return        : EMVInfo.ConfigCode
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
BYTE CDevCmn::fnEMV_GetAIDListCount()
{
	return EMVInfo.AIDListCount;
}

/*==============================================================================
* Function Name : fnEMV_GetCandidateCount()
* Description   : Candidate List Count Get
* Parameters    : (None)
* Return        : EMVInfo.CandidateCount
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
BYTE CDevCmn::fnEMV_GetCandidateCount()
{
	return EMVInfo.CandidateCount;
}

/*==============================================================================
* Function Name : fnEMV_SetCandidateCount()
* Description   : Candidate List Count Set
* Parameters    : CandidateCount
* Return        : (NONE)
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
void CDevCmn::fnEMV_SetCandidateCount(BYTE CandidateCount)
{
	EMVInfo.CandidateCount = CandidateCount;
}

/*==============================================================================
* Function Name : fnEMV_GetTranIndex()
* Description   : 선택된 Application의 Index Get
* Parameters    : (None)
* Return        : EMVInfo.TranIndex
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
BYTE CDevCmn::fnEMV_GetTranIndex()
{
	return EMVInfo.TranIndex;
}

/*==============================================================================
* Function Name : fnEMV_SetTranIndex()
* Description   : 선택된 Application의 Index Set
* Parameters    : TranIndex
* Return        : (NONE)
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
void CDevCmn::fnEMV_SetTranIndex(BYTE TranIndex)
{
	EMVInfo.TranIndex = TranIndex;
}

/*==============================================================================
* Function Name : fnEMV_GetCertificationType()
* Description   : 인증서 종류(AAC, TC, ARQC, AAR) Get
* Parameters    : (None)
* Return        : EMVInfo.CertificationType
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnEMV_GetCertificationType()
{
	return EMVInfo.CertificationType;
}

/*==============================================================================
* Function Name : fnEMV_SetCertificationType()
* Description   : 인증서 종류(AAC, TC, ARQC, AAR) Set
* Parameters    : szCertificationType
* Return        : (NONE)
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
void CDevCmn::fnEMV_SetCertificationType(char* szCertificationType)
{
	if ( sizeof(EMVInfo.CertificationType) >= strlen(szCertificationType) )
		strcpy( EMVInfo.CertificationType, szCertificationType );
}

/*==============================================================================
* Function Name : fnEMV_GetBrandCount()
* Description   : EMV 거래 가능 Application 목록의 갯수를 반환하는 함수
* Parameters    : (None)
* Return        : EMVInfo.CandidateCount
* Release       : Ver 1.0
* Notes			: 2004.06.18 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_GetBrandCount()
{
	return EMVInfo.CandidateCount;
}

/*==============================================================================
* Function Name : EMV_GetISO2()
* Description   : MS Track 2 Image(Tag 37) Get
* Parameters    : (None)
* Return        : Length of MS Track 2 Image(Unpack)
* Release       : Ver 1.0
* Notes			: 2004.06.25 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_GetISO2(char* ISO2)
{
	MakeUnPack( EMVDOLListInfo.MSTrack2Image, ISO2, EMVDOLListInfo.MSTrack2Len, FALSE);
	return EMVDOLListInfo.MSTrack2Len * 2;
}

/*==============================================================================
* Function Name : fnEMV_GetCID9F27()
* Description   : Cryptogram Information Data(Tag 9F27) Get
* Parameters    : (None)
* Return        : EMVDOLListInfo.CryptogramInfoData
* Release       : Ver 1.0
* Notes			: 2004.07.02 Han Su Woong
* ============================================================================*/
BYTE CDevCmn::fnEMV_GetCID9F27()
{
	return EMVDOLListInfo.CryptogramInfoData;
}

/*==============================================================================
* Function Name : fnEMV_SetAppCryptogram()
* Description   : 인증서 종류를 결정하는 함수
* Parameters    : (None)
* Return        : +--------------------+---------------------------------------+
*                 |      종    류      |                Return                 |
*                 +--------------------+---------------------------------------+
*                 | 오프라인 Error     | T_ERROR(-5)                           |
*                 | AAC(오프라인 거부) | EMV_RESULT_ONLINEDECLINE(4)           |
*                 | TC(오프라인 승인)  | EMV_RESULT_ONLINEDECLINE(4)           |
*                 | ARQC(온라인 승인)  | T_OK(0)                               |
*                 | AAR(위탁)          | EMV_RESULT_ONLINEDECLINE(4)           |
*                 +--------------------+---------------------------------------+
* Release       : Ver 1.0
* Notes			: 2004.06.25 Han Su Woong
* ============================================================================*/
int CDevCmn::fnEMV_SetAppCryptogram()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV_SetAppCryptogram()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV Set App Cryptogram");

	BYTE	bFlag;
	int		nReturn(T_ERROR);

//	bFlag = EMVDOLListInfo.CryptogramInfoData & 0x0F;	// CID Error Check			//V01-15-01
//	if ( bFlag != NULL )
//		return nReturn;

	memset( EMVInfo.CertificationType, NULL, sizeof(EMVInfo.CertificationType) );
	bFlag = EMVDOLListInfo.CryptogramInfoData & 0xC0;	// Cryptogram Type Check
	switch (bFlag)
	{		
	case EMV_CERT_AAC :							// 0x00. 오프라인 거부(Denial)
		strcpy( EMVInfo.CertificationType, "DECLINE" );
		nReturn = EMV_RESULT_ONLINEDECLINE;
		break;
	
	case EMV_CERT_TC :							// 0x40. 오프라인 승인(Approve)
		strcpy( EMVInfo.CertificationType, "APPROVE" );
		nReturn = EMV_RESULT_ONLINEDECLINE;
		break;
	
	case EMV_CERT_ARQC :						// 0x80. 온라인 승인(ARQC)
		strcpy( EMVInfo.CertificationType, "ARQC" );
		nReturn = T_OK;
		break;
	
	case EMV_CERT_AAR :							// 0xC0. 위탁(Not Accept)
	default :
		strcpy( EMVInfo.CertificationType, "NOT ACCEPT" );
		nReturn = EMV_RESULT_ONLINEDECLINE;
		break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV_SetAppCryptogram() EMVInfo.CertificationType = %s", EMVInfo.CertificationType);
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV_SetAppCryptogram() EMVInfo.CertificationType = %s", EMVInfo.CertificationType);
	return nReturn;
}


/*==============================================================================
* Function Name : fnEMV_GetHostTranData()
* Description   : Host로 전송할 EMV Data를 구하는 함수
* Parameters    : TranType : Host 전문 Type
* Return        : Host Send 전문
* Release       : Ver 1.0
* Notes			: 2004.06.25 Han Su Woong
* ============================================================================*/
LPSTR CDevCmn::fnEMV_GetHostTranData(BYTE TranType, int nEmvType)
{
	BYTE	bTemp[256]; 
//	BYTE    aTemp[256];
	int		nOffset(0), nLength, nMChipLen(0), n, nOffsetPos(0);
	BOOL    nMChipFlg = TRUE;
	BOOL    bFlag = TRUE;

	memset( m_EMVszData, ' ', sizeof(m_EMVszData) );
	memset(    bTemp, NULL,    sizeof(bTemp) );

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_GetHostTranData()");
MsgDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "EMV HOST Tran Data");

	if ((memcmp(EMVCandidateList[EMVInfo.TranIndex].AID, MASTER_AID, sizeof(MASTER_AID)) == 0)	    ||	// Master Card
		(memcmp(EMVCandidateList[EMVInfo.TranIndex].AID, LOCAL_MC_AID, sizeof(LOCAL_MC_AID)) == 0)	||	// Local M/Chip
		(memcmp(EMVCandidateList[EMVInfo.TranIndex].AID, D_PAS_AID,	 sizeof(D_PAS_AID)) == 0)		)	// D-PAS
	{
		nMChipFlg = TRUE;
	}
	else
	{
		nMChipFlg = FALSE;
	}


	//**************************************************************************/
	/*	[IC CARD인 경우 IC 신용카드 정보] -> Kim.Gi.Jin -> 내부검사 소스 1차 완료
		1. 001 - 006 : Terminal Capability Profile(9F33)
		2. 007 - 016 : Terminal Verification Result(95)
		3. 017 - 024 : Unpredictable Number(9F37)
		4. 025 - 040 : Cryptogram (ARQC, TC, AAC) (9F26)
		5. 041 - 044 : Application Transaction Counter(9F36)
		6. 045 - 048 : Application Interchange Profile(82)
		7. 049 - 050 : Cryptogram Transaction Type(9C)
		8. 051 - 054 : Terminal Country Code(9F1A)
		9. 055 - 060 : Terminal Transaction Date(9A)
		10.061 - 072 : Cryptogram Amount(9F02)
		11.073 - 076 : Cryptogram Currency Code(5F2A)
		12.077 - 088 : Cryptogram Cashback Amount(9F03)
		13.089 - 090 : Cryptogram Information Data(9F27)
		14.091 - 092 : Card Sequence Number(5F34)
		15.093 - 094 : Terminal Type(9F35)
		16.095 - 102 : Amount in Ref. Currency(9F3A)
		17.103 - 106 : Transaction Ref. Currency(9F3C)
		18.107 - 110 : Data Authentication Code(9F45)
		19.111       : M/chip version 
				       (1-M/Chip2 old, 2-M/Chip2 new, 4-M/chip4)
		20.112 - 179 : Issuer Application Data(9F10) 
		   1) 112 - 113 : Length Indicator ( 2byte ) 
		   2) 114 - 115 : Derivation Key Index (2byte)
		   3) 116 - 117 : Cryptogram Version Nr.(2byte)
		   4) 118 - 129 : Card Verification Result(12byte)
		   5) 130 - 179 : Issuer Discretionary Data
			  a) Length Indicator2 (1byte)
			  b) Data(var.)
		21.180 -192 : reserved */
	//**************************************************************************/
	if ( TranType == EMV_SENDHOST_ARQC )
	{
#if EMV_TA_MODE		// 인증 모드
		// PAN Length(1)
		char szBCDLen[10];
		BYTE bBCDLen;

		memset( szBCDLen, NULL, sizeof(szBCDLen) );
		sprintf( szBCDLen, "%02d", EMVDOLListInfo.PANLen * 2 );
		MakePack( szBCDLen, &bBCDLen, 2 );

		nLength = sizeof(BYTE);
		bTemp[nOffset] = bBCDLen;
		nOffset += nLength;

		// PAN(5A)(8 ~ 10)
		nLength = EMVDOLListInfo.PANLen;
		memcpy( bTemp + nOffset, EMVDOLListInfo.PAN, nLength );
		nOffset += nLength;
#endif	// #if EMV_TA_MODE

		//1. 001 - 006 : Terminal Capability Profile(9F33)
		nLength = sizeof(EMVDOLListInfo.TerminalCapabilitise);
		MakeUnPack(EMVDOLListInfo.TerminalCapabilitise, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//2. 007 - 016 : Terminal Verification Result(95)
		nLength = sizeof(EMVDOLListInfo.TerminalVerifyResult);
		MakeUnPack(EMVDOLListInfo.TerminalVerifyResult, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//3. 017 - 024 : Unpredictable Number(9F37)
		nLength = sizeof(EMVDOLListInfo.UnpredictableNumber);
		MakeUnPack(EMVDOLListInfo.UnpredictableNumber, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//4. 025 - 040 : Cryptogram (ARQC, TC, AAC) (9F26)
		nLength = sizeof(EMVDOLListInfo.AppCryptogram);
		MakeUnPack(EMVDOLListInfo.AppCryptogram, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//5. 041 - 044 : Application Transaction Counter(9F36)
		nLength = sizeof(EMVDOLListInfo.AppTranCounter);
		MakeUnPack(EMVDOLListInfo.AppTranCounter, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//6. 045 - 048 : Application Interchange Profile(82)
		nLength = sizeof(EMVDOLListInfo.AppInterchangeProfile);
		MakeUnPack(EMVDOLListInfo.AppInterchangeProfile, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//7. 049 - 050 : Cryptogram Transaction Type(9C)
		nLength = sizeof(EMVDOLListInfo.TransactionType);
		MakeUnPack(&EMVDOLListInfo.TransactionType, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//8. 051 - 054 : Terminal Country Code(9F1A)
		nLength = sizeof(EMVDOLListInfo.TerminalCountryCode);
		MakeUnPack(EMVDOLListInfo.TerminalCountryCode, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//9. 055 - 060 : Terminal Transaction Date(9A)
		nLength = sizeof(EMVDOLListInfo.TransactionDate);
		MakeUnPack(EMVDOLListInfo.TransactionDate, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//10.061 - 072 : Cryptogram Amount(9F02)
		nLength = sizeof(EMVDOLListInfo.TranAmountBCD);
		MakeUnPack(EMVDOLListInfo.TranAmountBCD, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//11.073 - 076 : Cryptogram Currency Code(5F2A)
		nLength = sizeof(EMVDOLListInfo.TranCurrencyCode);
		MakeUnPack(EMVDOLListInfo.TranCurrencyCode, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//12.077 - 088 : Cryptogram Cashback Amount(9F03)
		nLength = sizeof(EMVDOLListInfo.AnotherTranAmountBCD);
		MakeUnPack(EMVDOLListInfo.AnotherTranAmountBCD, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//13.089 - 090 : Cryptogram Information Data(9F27)
		nLength = sizeof(EMVDOLListInfo.CryptogramInfoData);
		MakeUnPack(&EMVDOLListInfo.CryptogramInfoData, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);


		//14.091 - 092 : Card Sequence Number(5F34)
		nLength = sizeof(EMVDOLListInfo.CSN);
		MakeUnPack(&EMVDOLListInfo.CSN, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

// 		if (nEmvType == EMV_SENDHOST_BC_FORMAT);
// 		{
// 			nOffsetPos = nOffset;
// 			nOffset += 16;
// 		}

		//15.093 - 094 : Terminal Type(9F35)
		nLength = sizeof(EMVDOLListInfo.TerminalType );
		MakeUnPack(&EMVDOLListInfo.TerminalType, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//16.095 - 102 : Amount in Ref. Currency(9F3A)
		BYTE byAmount[4] = {0x00, };
		nLength = sizeof(byAmount);
		MakeUnPack(byAmount, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//17.103 - 106 : Transaction Ref. Currency(9F3C)
		BYTE byCurCode[2] = {0x00, };
		nLength = sizeof(byCurCode);
		MakeUnPack(byCurCode, bTemp + nOffset, nLength, TRUE);
		nOffset += (nLength * 2);

		//18.107 - 110 : Data Authentication Code(9F45)
		if (nMChipFlg)											// Master Card
		{
			int nIndexDAC = EMV_M4_DAC;
			
			if (EMVDOLListInfo.IADLen == EMV_M4_IAD_LEN)
			{													// M/Chip4
				nIndexDAC = EMV_M4_DAC;
			}
			else
			if (EMVDOLListInfo.IADLen == EMV_M2_NEW_IAD_LEN)	// M/Chip2 New
			{
				nIndexDAC = EMV_M2_NEW_DAC;
			}
			else												// M/Chip2 Old
			{
				nIndexDAC = EMV_M2_OLD_DAC;
			}
			
			nLength = EMV_DAC_LEN;
			MakeUnPack( &EMVDOLListInfo.IssuerAppData[nIndexDAC], bTemp + nOffset, nLength );
			nOffset += nLength * 2;
		}
		else													// VISA/JCB Card
		{
			BYTE byDAC[2] = {0x00, };
			nLength = sizeof(byDAC);
			MakeUnPack( byDAC, bTemp + nOffset, nLength );
			nOffset += nLength * 2;
		}

		//19.111       : M/chip version  -> (1-M/Chip2 old, 2-M/Chip2 new, 4-M/chip4)
		BYTE	ChipVersion = 0x30;
		BYTE	ChipLength = 0x00;
		if (memcmp(EMVCandidateList[EMVInfo.TranIndex].AID, D_PAS_AID,	 sizeof(D_PAS_AID)) == 0)
			ChipVersion = 0x34;
		else
		if (nMChipFlg)
		{
		   if (EMVDOLListInfo.IADLen == EMV_M4_IAD_LEN)         // M/Chip4
		   {
				ChipVersion = 0x34;
		   }
     	   else													// M/Chip2
		   if (EMVDOLListInfo.IADLen == EMV_M2_OLD_IAD_LEN)
		   {
				ChipVersion = 0x31;
		   }
		   else 
		   if (EMVDOLListInfo.IADLen == EMV_M2_NEW_IAD_LEN)
		   { 
				ChipVersion = 0x32;
		   }
		   else
		   { 
				ChipVersion = 0x20;
		   }
		}
		else
		{
			ChipVersion = 0x20;
		}

		ChipLength = EMVDOLListInfo.IADLen;
		
		memcpy(bTemp + nOffset, &ChipVersion, 1 );
		nOffset += 1;

		//20.112 - 179 : Issuer Application Data(9F10) 
		//*********************************************************************
		//  VSDC - Issuer Application Data (7Byte)
		//---------------------------------------------------------------------
		//  Length Indicator (1)
		//  Derivation Key Index (1)
		//  Cryptogram Version No. (1)
		//  Card Verification Result (4)
		//*********************************************************************
		//  JCB - Issuer Application Data (8Byte)
		//---------------------------------------------------------------------
		//  Length Indicator (1)
		//  Derivation Key Index (1)
		//  Cryptogram Version No. (1)
		//  Card Verification Result (5)
		//*********************************************************************
		//  M/Chip2 Old - Issuer Application Data (8Byte)
		//---------------------------------------------------------------------
		//  Derivation Key Index (1)
		//  Cryptogram Version No. (1)
		//  Card Verification Result (4)
		//  Data Authentication Code(DAC) (2)
		//*********************************************************************
		//  M/Chip2 New - Issuer Application Data (9Byte)
		//---------------------------------------------------------------------
		//  Length Indicator (1)
		//  Derivation Key Index (1)
		//  Cryptogram Version No. (1)
		//  Card Verification Result (4)
		//  Data Authentication Code(DAC) (2)
		//*********************************************************************
		//  M/Chip4 - Issuer Application Data (18Byte)
		//---------------------------------------------------------------------
		//  Derivation Key Index (1)
		//  Cryptogram Version No. (1)
		//  Card Verification Result (6)
		//  DAC/ICC Dynamic No. (2)
		//  Plaintext/Encrypted Counters (8)
		//*********************************************************************

		// Issuer Application Data(9F10)(Varables)
		//  LI + DKI + CVN
		if (nMChipFlg)											// Master Card
		{
			if (EMVDOLListInfo.IADLen == EMV_M4_IAD_LEN)		// M/Chip4
			{
MsgDump(MAC_TRACE_CODE_FILE("Log"), "M/Chip4");
				bTemp[nOffset] = 0x20;
				nOffset += 1;
				bTemp[nOffset] = 0x20;
				nOffset += 1;
				
				nLength = EMV_DKI_LEN + EMV_CVN_LEN;
				MakeUnPack( EMVDOLListInfo.IssuerAppData, bTemp + nOffset, nLength, TRUE );
				nOffset += nLength * 2;
			}
			else
			if (EMVDOLListInfo.IADLen == EMV_M2_NEW_IAD_LEN)	// M/Chip2 New
			{
				
MsgDump(MAC_TRACE_CODE_FILE("Log"), "M/Chip2 New");
				nLength = EMV_LI_LEN + EMV_DKI_LEN + EMV_CVN_LEN;
				MakeUnPack( EMVDOLListInfo.IssuerAppData, bTemp + nOffset, nLength, TRUE );
				nOffset += nLength * 2;
			}
			else												// M/Chip2 Old
			{
MsgDump(MAC_TRACE_CODE_FILE("Log"), "M/Chip2 Old");
				bTemp[nOffset] = 0x20;
				nOffset += 1;
				bTemp[nOffset] = 0x20;
				nOffset += 1;
				
				nLength = EMV_DKI_LEN + EMV_CVN_LEN;
				MakeUnPack( EMVDOLListInfo.IssuerAppData, bTemp + nOffset, nLength, TRUE );
				nOffset += nLength * 2;
			}
		}
		else													// JCB Card   Visa Card
		{
			nLength = EMV_LI_LEN + EMV_DKI_LEN + EMV_CVN_LEN;
			MakeUnPack( EMVDOLListInfo.IssuerAppData, bTemp + nOffset, nLength, TRUE );
			nOffset += nLength * 2;
		}


// 		if (nEmvType == EMV_SENDHOST_BC_FORMAT)
// 		{
// 			memcpy( bTemp + nOffsetPos, &bTemp[nOffset - 4],  4);
// 			nOffsetPos += 4;
// 		}


		// Card Verification Result(12byte)
		BYTE bCVRN[12];
		memset(bCVRN, 0x20, sizeof(bCVRN));

		if (memcmp(EMVCandidateList[EMVInfo.TranIndex].AID, D_PAS_AID,	 sizeof(D_PAS_AID)) == 0)
			MakeUnPack( &EMVDOLListInfo.IssuerAppData[2], bCVRN, 6, TRUE );
		else
		if (nMChipFlg)											// Master Card
		{
			if (EMVDOLListInfo.IADLen == EMV_M4_IAD_LEN)         // M/Chip4
			{
				MakeUnPack( &EMVDOLListInfo.IssuerAppData[2], bCVRN, 6, TRUE );
			}
			else
			if (EMVDOLListInfo.IADLen == EMV_M2_NEW_IAD_LEN)	// M/Chip2 New
			{
				MakeUnPack( &EMVDOLListInfo.IssuerAppData[3], bCVRN, 4, TRUE );
			}
			else												// M/Chip2 Old
			{
				MakeUnPack( &EMVDOLListInfo.IssuerAppData[2], bCVRN, 4, TRUE );
			}
		}
		else													
		if (memcmp(EMVCandidateList[EMVInfo.TranIndex].AID, JCB_AID, sizeof(JCB_AID)) == 0)
		{														// JCB Card
			MakeUnPack( &EMVDOLListInfo.IssuerAppData[3], bCVRN, 5, TRUE );
		}
		else													// Visa Card
		{
			MakeUnPack( &EMVDOLListInfo.IssuerAppData[3], bCVRN, 4, TRUE );
		}

	    memcpy( bTemp + nOffset, bCVRN, 12 );
		nOffset += 12;

// 		if (nEmvType == EMV_SENDHOST_BC_FORMAT)
// 		{
// 			memcpy( bTemp + nOffsetPos, bCVRN,  12);
// 		}
// 		else
// 		{
// 			memcpy( bTemp + nOffset, bCVRN,     12);
// 			nOffset += 12;
// 		}

		n = 192 - nOffset;
		memset(bTemp + nOffset, 0x20, n);
		nOffset = 192;

		memcpy(m_EMVszData, bTemp, nOffset);


	}
	else 
	if (TranType == EMV_SENDHOST_REVERSAL)
	{
// KH0930 전문추가
		// PAN Sequence Number(5F34)(1)
		nLength = sizeof(EMVDOLListInfo.CSN);
		bTemp[nOffset] = EMVDOLListInfo.CSN;
		nOffset += nLength;		
// KH0930 end

		// Terminal Verification Result(95)(5)
		nLength = sizeof(EMVDOLListInfo.TerminalVerifyResult);
		memcpy( bTemp + nOffset, EMVDOLListInfo.TerminalVerifyResult, nLength );
		nOffset += nLength;

// KH0930 전문추가
		// Issuer Application Data(9F10)(Varables)   // 주의 (Length Indicator(1) 제외)
		nLength = EMVDOLListInfo.IADLen - 1;
		 
		if(nLength == 8)  // 마스타의 경우
		{
			memcpy( bTemp + nOffset, EMVDOLListInfo.IssuerAppData + 1 , nLength );
			nOffset += nLength;	
			bFlag = TRUE;
		} 		
		else 
		if(nLength < 8)  // 비자의 경우 
		{
			memcpy( bTemp + nOffset, EMVDOLListInfo.IssuerAppData + 1 , nLength );
			nOffset += nLength;
		    memset( bTemp + nOffset, 0x20 , 8- nLength);
			nOffset += (8 - nLength);
			bFlag = FALSE;
		}
		else
		{
			memcpy( bTemp + nOffset, EMVDOLListInfo.IssuerAppData + 1 , 8 );
			nOffset += 8;
			bFlag = TRUE;
		}		
		
		// Application Transaction Counter(9F36)(2)
		nLength = sizeof(EMVDOLListInfo.AppTranCounter);
		memcpy( bTemp + nOffset, EMVDOLListInfo.AppTranCounter, nLength );
		nOffset += nLength;
// KH0930 end

#if EMV_TA_MODE		// 인증 모드
		// Terminal Status Infomation(9B)(2)
		nLength = sizeof(EMVDOLListInfo.TerminalStatusInfo);
		memcpy( bTemp + nOffset, EMVDOLListInfo.TerminalStatusInfo, nLength );
		nOffset += nLength;

		// Transaction Time(9F21)(3)
		nLength = sizeof(EMVDOLListInfo.TransactionTime);
		memcpy( bTemp + nOffset, EMVDOLListInfo.TransactionTime, nLength );
		nOffset += nLength;

		// Authorization Response Code(8A)(2)
		nLength = sizeof(EMVDOLListInfo.AuthResponseCode);
		memcpy( bTemp + nOffset, EMVDOLListInfo.AuthResponseCode, nLength );
		nOffset += nLength;
#endif	// #if EMV_TA_MODE

		// Issuer Script Result(9F5B)(5)
		memcpy( bTemp + nOffset, EMVDOLListInfo.IssuerScriptResult,	EMVDOLListInfo.IssuerScriptResultLen );
		nOffset += EMVDOLListInfo.IssuerScriptResultLen;
		// Message Unpack
		MakeUnPack( bTemp, m_EMVszData, nOffset, TRUE );
	}

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMV_GetHostTranData [m_EMVszData]", m_EMVszData, 200);
HexDump(TRACE_PLUS_MODE, "EMV", __FILE__, __LINE__, "fnEMV_GetHostTranData EMV Data", m_EMVszData, 200);
	return m_EMVszData;
}


// #NICCM01, AP보드 단종대응 ------------------------------------------
// EMV Library 작업 - by lucas
/////////////////////////////////////////////////////////////////////////////
//	EMV 수출용 LIB작업 FUNCTION(NHMWIEMV) 
/////////////////////////////////////////////////////////////////////////////

void CDevCmn::fnEMVLib_Initial()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Initial()");
	if (fnAPL_GetDefineDevice(DEV_MCU))
		m_pMwiEmv->EMV_Initial();
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Initial() return");
}

long CDevCmn::fnEMVLib_SetKernelType(long nType)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_SetKernelType()" );
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_SetKernelType(nType);
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_SystemInitial(.);return(%d)", nRtn);
	return nRtn;	
}

long CDevCmn::fnEMVLib_SystemInitial()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_SystemInitial()" );
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_SystemInitial();
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_SystemInitial(.);return(%d)", nRtn);
	return nRtn;
}

long CDevCmn::fnEMVLib_Sel_BuildCandidateApp()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Sel_BuildCandidateApp()");
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_Sel_BuildCandidateApp();

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Sel_BuildCandidateApp():return(%d)", nRtn);
	return nRtn;
}

long CDevCmn::fnEMVLib_Sel_SetOnlyOneAID(CString strYN)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Sel_SetOnlyOneAID(%s)", strYN);
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_Sel_SetOnlyOneAID(strYN);
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Sel_SetOnlyOneAID():return(%d)", nRtn);
	return nRtn;
}


CString	CDevCmn::fnEMVLib_Sel_GetAppListEx()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Sel_GetAppListEx()");
	
	CString sRtn = _T("ERROR");
	if (fnAPL_GetDefineDevice(DEV_MCU))
	{
		BSTR sAPList;
		long nRtn = m_pMwiEmv->EMV_Sel_GetAppListEx(&sAPList);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pMwiEmv->EMV_Sel_GetAppList(..):return %ld", nRtn);
		if(nRtn == EMV_RSLT_OK)
		{
			sRtn = sAPList;
			SysFreeString(sAPList);
		}
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Sel_GetAppList():return(%s)", sRtn);
	return sRtn;
}

CString	CDevCmn::fnEMVLib_Sel_GetAppList()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Sel_GetAppList()");

	CString sRtn = _T("ERROR");
	if (fnAPL_GetDefineDevice(DEV_MCU))
	{
		BSTR sAPList;
		long nRtn = m_pMwiEmv->EMV_Sel_GetAppList(&sAPList);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pMwiEmv->EMV_Sel_GetAppList(..):return %ld", nRtn);
		if(nRtn == EMV_RSLT_OK)
		{
			sRtn = sAPList;
			SysFreeString(sAPList);
		}
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Sel_GetAppList():return(%s)", sRtn);
	return sRtn;
}

long CDevCmn::fnEMVLib_Sel_FinalAppSelection(long iSelApp)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Sel_FinalAppSelection(%d)", iSelApp);

	EMVInfo.TranIndex = iSelApp;				//sjcha		20131226

	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_Sel_FinalAppSelection(iSelApp);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_Sel_FinalAppSelection():return(%d)", nRtn);
	return nRtn;
}

long CDevCmn::fnEMVLib_InitAppProcess(LPCTSTR TrProcCode, LPCTSTR TrAmount, LPCTSTR TrDateTime, long TrSerialCnt)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_InitAppProcess(%s, %s, %s, %d)", TrProcCode, TrAmount, TrDateTime, TrSerialCnt);
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_InitAppProcess(TrProcCode, TrAmount, TrDateTime, TrSerialCnt);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_InitAppProcess(....):return(%d)", nRtn);
	return nRtn;
}

long CDevCmn::fnEMVLib_ReadAppData()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_ReadAppData()");
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_ReadAppData();

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_ReadAppData():return(%d)", nRtn);
	return nRtn;
}

long CDevCmn::fnEMVLib_OffDataAuth()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_OffDataAuth()");
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_OffDataAuth();

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_OffDataAuth():return(%d)", nRtn);
	return nRtn;
}

long CDevCmn::fnEMVLib_ProcRestriction()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_ProcRestriction()");
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_ProcRestriction();

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_ProcRestriction():return(%d)", nRtn);
	return nRtn;
}

CString CDevCmn::fnEMVLib_GetVal(long Val_Id)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_GetVal(%d)", Val_Id);
	CString sRtn = _T("");
	if (fnAPL_GetDefineDevice(DEV_MCU))
		sRtn = m_pMwiEmv->EMV_GetVal(Val_Id);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_GetVal(%d):return(%s)", Val_Id, sRtn);
	return sRtn;
}

long CDevCmn::fnEMVLib_TlvStoreVal(long nTid, LPCTSTR strVal)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_TlvStoreVal(%d, %s)", nTid, strVal);
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_TlvStoreVal(nTid, strVal);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV_TlvStoreVal(..):return(%d)", nRtn);
	return nRtn;
}

long CDevCmn::fnEMVLib_CardholderVerify1(LPCTSTR EncPinData)
{
	MsgDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_CardholderVerify1(%s)", EncPinData);
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_CardholderVerify1(EncPinData);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_CardholderVerify1(.):return(%d)", nRtn);
	return nRtn;
}

long CDevCmn::fnEMVLib_TerminalRiskMgmt()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV_TerminalRiskMgmt()");
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_TerminalRiskMgmt();

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMV_TerminalRiskMgmt():return(%d)", nRtn);
	return nRtn;
}
long CDevCmn::fnEMVLib_ActionAnalysis(BSTR* CardRslt)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_ActionAnalysis(.)");
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_ActionAnalysis(CardRslt);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_ActionAnalysis(.):return(%d)", nRtn);
	return nRtn;
}

long CDevCmn::fnEMVLib_CompleteProcess()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_CompleteProcess()");
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
		nRtn = m_pMwiEmv->EMV_CompleteProcess();

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_CompleteProcess():return(%d)", nRtn);
	return nRtn;
}

CString CDevCmn::fnEMVLib_MakeEMVTagData(LPCTSTR sTagType)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_MakeEMVTagData(%s)", sTagType);
	CString sRtn = _T("");
	CString strType = CString(sTagType);
	CString strSubVal = _T("");
	CString strTemp;

	// 9F33 03 604000

	if( (strType == "TRANSACTION")	|| (strType == "REVERSAL"))
	{
		// Tag "9F33" - Terminal Capability                               
		strSubVal = fnEMVLib_GetVal(EMVTid_TerminalCapa);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.TerminalCapabilitise, __min(sizeof(EMVDOLListInfo.TerminalCapabilitise)*2, strSubVal.GetLength()));
	
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.TerminalCapabilitise ", EMVDOLListInfo.TerminalCapabilitise, sizeof(EMVDOLListInfo.TerminalCapabilitise ));	

		// Tag "95" - Terminal Verification Result
		strSubVal = fnEMVLib_GetVal(EMVTid_TVR);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.TerminalVerifyResult, __min(sizeof(EMVDOLListInfo.TerminalVerifyResult)*2, strSubVal.GetLength()));

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.TerminalVerifyResult ", EMVDOLListInfo.TerminalVerifyResult, sizeof(EMVDOLListInfo.TerminalVerifyResult ));	
		
		// Tag "9F37" - Unpredictable Number
		strSubVal = fnEMVLib_GetVal(EMVTid_UnpredictNo);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.UnpredictableNumber, __min(sizeof(EMVDOLListInfo.UnpredictableNumber)*2, strSubVal.GetLength()));

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.UnpredictableNumber ", EMVDOLListInfo.UnpredictableNumber, sizeof(EMVDOLListInfo.UnpredictableNumber ));		
		// Tag "9F26" - Application Cryptogram                            
		strSubVal = fnEMVLib_GetVal(EMVTid_AC);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.AppCryptogram, __min(sizeof(EMVDOLListInfo.AppCryptogram)*2, strSubVal.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.AppCryptogram ", EMVDOLListInfo.AppCryptogram, sizeof(EMVDOLListInfo.AppCryptogram ));	

		// Tag "9F36" - Application Transaction Counter                   
		strSubVal = fnEMVLib_GetVal(EMVTid_ATC);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.AppTranCounter, __min(sizeof(EMVDOLListInfo.AppTranCounter)*2, strSubVal.GetLength()));
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.AppTranCounter ", EMVDOLListInfo.AppTranCounter, sizeof(EMVDOLListInfo.AppTranCounter ));	
		
		// Tag "82" - Application Interchange Profile
		strSubVal = fnEMVLib_GetVal(EMVTid_AIP);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.AppInterchangeProfile, __min(sizeof(EMVDOLListInfo.AppInterchangeProfile)*2, strSubVal.GetLength()));
		
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.AppInterchangeProfile ", EMVDOLListInfo.AppInterchangeProfile, sizeof(EMVDOLListInfo.AppInterchangeProfile ));	


		// Tag "9C" - Transaction Type
		strSubVal = fnEMVLib_GetVal(EMVTid_TrType);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), &EMVDOLListInfo.TransactionType, __min(sizeof(&EMVDOLListInfo.TransactionType)*2, strSubVal.GetLength()));


HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.TransactionType ", &EMVDOLListInfo.TransactionType, sizeof(EMVDOLListInfo.TransactionType ));	

		// Tag "9F1A" - Terminal Countery Code                            
		strSubVal = fnEMVLib_GetVal(EMVTid_TCountryCod);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.TerminalCountryCode, __min(sizeof(EMVDOLListInfo.TerminalCountryCode)*2, strSubVal.GetLength()));

		
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.TerminalCountryCode ", EMVDOLListInfo.TerminalCountryCode, sizeof(EMVDOLListInfo.TerminalCountryCode ));	

		// Tag "9A" - Transaction Date
		strSubVal = fnEMVLib_GetVal(EMVTid_TrDate);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.TransactionDate, __min(sizeof(EMVDOLListInfo.TransactionDate)*2, strSubVal.GetLength()));

		
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.TransactionDate ", EMVDOLListInfo.TransactionDate, sizeof(EMVDOLListInfo.TransactionDate ));	

		// Tag "9F02" - Amount Authorized (Numeric)
		strSubVal = fnEMVLib_GetVal(EMVTid_AmountAuthN);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.TranAmountBCD, __min(sizeof(EMVDOLListInfo.TranAmountBCD)*2, strSubVal.GetLength()));

		
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.TranAmountBCD ", EMVDOLListInfo.TranAmountBCD, sizeof(EMVDOLListInfo.TranAmountBCD ));	

		// Tag "5F2A" - Transaction Currency Code
		strSubVal = fnEMVLib_GetVal(EMVTid_TrCurCod);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.TranCurrencyCode, __min(sizeof(EMVDOLListInfo.TranCurrencyCode)*2, strSubVal.GetLength()));

		
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.TranCurrencyCode ", EMVDOLListInfo.TranCurrencyCode, sizeof(EMVDOLListInfo.TranCurrencyCode ));	

		// Tag "9F03" - Amount Other (Numeric)                         
		strSubVal = fnEMVLib_GetVal(EMVTid_AmountOtherN);
		if(strSubVal.GetLength()==6)	
			MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.AnotherTranAmountBCD, __min(sizeof(EMVDOLListInfo.AnotherTranAmountBCD)*2, strSubVal.GetLength()));
		
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.AnotherTranAmountBCD ", EMVDOLListInfo.AnotherTranAmountBCD, sizeof(EMVDOLListInfo.AnotherTranAmountBCD ));	

		// Tag "9F27" - Cryptogram Information Data                       
		strSubVal = fnEMVLib_GetVal(EMVTid_CryptInfData);

		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), &EMVDOLListInfo.CryptogramInfoData, __min(sizeof(&EMVDOLListInfo.CryptogramInfoData)*2, strSubVal.GetLength()));

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.CryptogramInfoData ", &EMVDOLListInfo.CryptogramInfoData, sizeof(&EMVDOLListInfo.CryptogramInfoData ));	

		// Tag "5F34" - Primary Account Sequence Number                   
		strSubVal = fnEMVLib_GetVal(EMVTid_PANSeqNo);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), &EMVDOLListInfo.CSN, __min(sizeof(&EMVDOLListInfo.CSN)*2, strSubVal.GetLength()));

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.CSN ", &EMVDOLListInfo.CSN, sizeof(EMVDOLListInfo.CSN ));	

		// Tag "9F10" - Issuer Application Date                           
		strSubVal = fnEMVLib_GetVal(EMVTid_IsuAppData);
		HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strSubVal(EMVTid_IsuAppData)", strSubVal.GetBuffer(0), strSubVal.GetLength());
		if(!strSubVal.IsEmpty())
			EMVDOLListInfo.IADLen = MakePack(strSubVal.GetBuffer(0), EMVDOLListInfo.IssuerAppData, __min(sizeof(EMVDOLListInfo.IssuerAppData)*2, strSubVal.GetLength()));
		
		memcpy(EMVDOLListInfo.CVMR, &EMVDOLListInfo.IssuerAppData[8], 3);					//sjcha		20131226	9F34(CVM R)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.IssuerAppData Length = %d", EMVDOLListInfo.IADLen);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.IssuerAppData ", EMVDOLListInfo.IssuerAppData, EMVDOLListInfo.IADLen);	

		// Tag "9F35" - Terminal Type
		strSubVal = fnEMVLib_GetVal(EMVTid_TerminalType);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), &EMVDOLListInfo.TerminalType, __min(sizeof(&EMVDOLListInfo.TerminalType)*2, strSubVal.GetLength()));

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVDOLListInfo.TerminalType ", &EMVDOLListInfo.TerminalType, sizeof(EMVDOLListInfo.TerminalType ));	

/*
		// Tag "BF0C" - Issuer Discretionary Data //#N9999
		strSubVal = fnEMVLib_GetVal(EMVTid_FciIsuDisData);
		if(!strSubVal.IsEmpty())
			MakePack(strSubVal.GetBuffer(0), &EMVDOLListInfo.TerminalType, __min(sizeof(&EMVDOLListInfo.TerminalType)*2, strSubVal.GetLength())); 
*/
  
	}
	else
	if(strType == "FALLBACK")
	{
		; 
// 내수에서는 FALLBACK관련하여 별도 Flag관리로 여기서 할 필요가 없음.	
	}


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_MakeEMVTagData():return");

	return sRtn;
}

long CDevCmn::fnEMVLib_OnlineApproval(int nOnlineMode, int nHostResponse, LPCTSTR ResponseCd, long ARPCLen, LPCTSTR ARPCData, long IsuScriptLen, LPCTSTR IsuScriptData)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_OnlineApproval(HostRes:%d, OnlineMode:%d, EMV_8A:%s, %d, %s, %d, %s)", nHostResponse, nOnlineMode, ResponseCd, ARPCLen, ARPCData, IsuScriptLen, IsuScriptData);
	long nRtn = 0;
	if (fnAPL_GetDefineDevice(DEV_MCU))
	{
		CString sNewCode = _T("");
		CString sEMVResCode = CString(ResponseCd);
		if(sEMVResCode.GetLength()!=4)
		{
//			sNewCode.Format("%d03035", nOnlineMode);			// if 8A Tag is not valid, AAC with 3035
			sNewCode.Format("%d05A33", nOnlineMode);			// if 8A Tag is not valid, AAC with 5A33(Z3) [#2188] 인증용 Denial Code
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "8A TAG[%s] => Length Incorrect,  AAC with 3035 : [%s]", sEMVResCode, sNewCode);
		}
		else
		{
			BYTE* sOut = new BYTE[sEMVResCode.GetLength() +1];
			memset(sOut, NULL, sEMVResCode.GetLength()+1);
			int nconverted = MakePack( (void*)(LPCTSTR)sEMVResCode, (void*)sOut, sEMVResCode.GetLength(), '^', 0);
			char chCK1 = sOut[0];	char chCK2 = sOut[1];
			delete [] sOut;
			if( (nconverted!=2)||(chCK1<0x30)||(chCK1>0x39)||(chCK2<0x30)||(chCK2>0x39) )
			{
//				sNewCode.Format("%d03035", nOnlineMode);			// if 8A Tag is not valid, AAC with 3035
				sNewCode.Format("%d05A33", nOnlineMode);			// if 8A Tag is not valid, AAC with 5A33(Z3) [#2188] 인증용 Denial Code
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "INVALID 8A TAG[%s],  AAC with 3035 : [%s]", sEMVResCode, sNewCode);
			}
			else
			{
				sNewCode.Format("%d%d%s", nOnlineMode, nHostResponse, ResponseCd);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "New TAG for Retail : [%s]", sNewCode);
			}
		}
		nRtn = m_pMwiEmv->EMV_OnlineApproval2(sNewCode, ARPCLen, ARPCData, IsuScriptLen, IsuScriptData);
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnEMVLib_OnlineApproval(.) => EMV_OnlineApproval2(....)  return(%d)", nRtn);
	return nRtn;
}
// --------------------------------------------------------------------------------------------




