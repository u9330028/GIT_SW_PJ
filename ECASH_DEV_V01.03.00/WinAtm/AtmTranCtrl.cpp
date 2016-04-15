/////////////////////////////////////////////////////////////////////////////
// AtmTranCtrl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\H\Common\CmnLibIn.h"
#include "..\H\Common\Define.h"
#include "..\H\Common\ConstDef.h"
#include "..\H\Common\ClassInclude.h"
#include "..\H\Dll\DevCmn.h"
#include "..\H\Tran\TranCmn.h"

#include "WinAtm.h"
#include "WinAtmCtl.h"
#include "WinAtmPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// 거래처리
int	CWinAtmCtrl::TranProc(int TranValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranProc(%d)", TranValue);

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->Om.OperationStatus = "OFF";						// 조작금지
	m_pDevCmn->fnAPL_ClerkInformation();						// 계원정보
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->AtmStatusReEntryFlag = FALSE;					// 재진입설정
	m_pDevCmn->TranResult = FALSE;								// 거래결과초기화(FALSE)
	m_pDevCmn->TranStatus = TranValue;							// 거래처리


	switch (m_pDevCmn->TranStatus) 
	{
		case TRAN_IDLE:											// 거래대기
			m_pDevCmn->TranStatusSave = TRAN_IDLE;				// 이전상태보관
			break;
		case TRAN_READY:										// 준비완처리
		case TRAN_STATUS:										// 상태/장애처리
		case TRAN_OPEN:											// 개국처리
		case TRAN_TRAN:											// 거래처리
		case TRAN_TOTAL:										// 합계인자
		case TRAN_SUBTOTAL:										// 소계인자
		case TRAN_CASH_REG:										// 현금등록
		case TRAN_CHECK_REG:									// 수표등록
		case TRAN_CHECK_UNREG:									// 수표해제
		case TRAN_MSFINGER_CONFIRM:								// 지문인증 Flow 진행
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranStatus[%d]", m_pDevCmn->TranStatus);
			m_pDevCmn->TranStatusSave = m_pDevCmn->TranStatus;	// 이전상태보관

			m_pTranCmn->fnAPP_TranVirtualMainProc();			// 거래가상메인
			break;

		default:
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranProc():return(%d)", m_pDevCmn->TranResult);
	return m_pDevCmn->TranResult;								// Tran거래결과
}

// 개국처리
int	CWinAtmCtrl::OpenProc()
{
	if (m_pDevCmn->fnAPL_CheckHostOpen())						// 호스트개국검지
		return TRUE;

	if (m_pDevCmn->Timer.HostOpenRetryTime)						// 개국재시도타임
		return FALSE;

	if (m_pDevCmn->HostStatus == ATM_CLERK)						// 개국재시도타임
		return FALSE;

	if (m_pDevCmn->HostStatus == HOST_WAITREADY)				// 준비완대기
	{
		m_pDevCmn->SeedTranStatus = FALSE;						// 

		if (m_pDevCmn->fnAPL_CheckHostLine())					// 준비완처리 (암호화 키처리)
		{														// Host Ok
			m_pDevCmn->HostStatus = HOST_READY;					// 준비완
			Delay_Msg();										// 개국대기진입
		}
		else 
		{														// Host Ng
			if (m_pDevCmn->HostOpenRetryCnt++ < 6)				// 개국시도횟수(준비완포함)
				m_pDevCmn->Timer.HostOpenRetryTime = 10;			// 개국재시도타임(5번이하)
			else 
				m_pDevCmn->Timer.HostOpenRetryTime = OPEN_RETRY_TIME;		
																// 개국재시도타임(6번이상)
		}
	}

	if (m_pDevCmn->HostStatus == HOST_WAITONLINE)					// 개국대기
	{
		if(m_pDevCmn->HostFirstOpenFlag)
		{
			if(TranProc(TRAN_STATUS))
			{
				m_pDevCmn->HostOpenFlag = TRUE;
				m_pDevCmn->HostStatus = HOST_ONLINE;				// 개국완료			
			}
			else
			{
				m_pDevCmn->HostStatus = HOST_WAITREADY;				// 준비완대기
				if (m_pDevCmn->HostOpenRetryCnt++ > 30)				// 개국시도횟수(준비완포함)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅 - RECV NG Cnt(%d)]", m_pDevCmn->HostOpenRetryCnt));
					m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
				}
				else 
					m_pDevCmn->Timer.HostOpenRetryTime = OPEN_RETRY_TIME;		
			}
		}
		else
		if (TranProc(TRAN_OPEN))								// 개국처리
		{														// Host Ok
			m_pDevCmn->HostStatus = HOST_ONLINE;				// 개국완료
			m_pDevCmn->HostFirstOpenFlag = TRUE;				// Host최초개국Flag
			m_pDevCmn->HostFirstReadyFlag = TRUE;
			m_pDevCmn->HostOpenFlag = TRUE;						// Host개국Flag
		}
		else 
		{														// Host Ng
			m_pDevCmn->HostStatus = HOST_WAITREADY;				// 준비완대기
			
			if (m_pDevCmn->HostOpenRetryCnt++ > 30)				// 개국시도횟수(준비완포함)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅 - RECV NG Cnt(%d)]", m_pDevCmn->HostOpenRetryCnt));
				m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			}
			else 
				m_pDevCmn->Timer.HostOpenRetryTime = OPEN_RETRY_TIME;		
																// 개국재시도타임(6번이상)
		}
	}

	if (!m_pDevCmn->fnAPL_CheckHostOpen())						// 호스트개국검지
		return FALSE;

	m_pDevCmn->HostOpenRetryCnt = 0;							// 개국시도횟수(준비완포함)
	m_pDevCmn->Timer.HostOpenRetryTime = 0;						// 개국재시도타임
	return TRUE;
}	

int	CWinAtmCtrl::ReOpenUploadProc()
{	
	CString strTmp("");
	char tmpYYYYMMDD[9];
	char tmpHHSS[5];
	static int sendFlg = FALSE;

	memcpy(tmpYYYYMMDD, GetDate().GetBuffer(0), GetDate().GetLength());
	memcpy(tmpHHSS, GetTime().GetBuffer(0), 4);
	strTmp.Format("%4.4s", &m_pProfile->NETWORK.AtmSerialNum[4]);

	if(memcmp(m_pProfile->TRANS.YYYYMMDD, GetDate().GetBuffer(0), GetDate().GetLength()) != 0)
	{
		//m_pTranCmn->DeleteJnlFiles(_EJR_DIR, 30);		//30일전 파일 삭제


		sendFlg = FALSE;
		
		//#N0280
		m_pDevCmn->fnAPL_SetDate(tmpYYYYMMDD);
		m_pDevCmn->fnAPL_ClearSerialNo();

		if (!m_pDevCmn->fnAPL_OpenLine() || !m_pDevCmn->fnAPL_CheckHostOpen())
			return T_OK;
	}

	// #N0274
	if (!sendFlg)
	{
		int nRebootTime = Asc2Int(strTmp, 1) / 2 * 10;	// 기번으로 리부팅시간 계산

		if (nRebootTime != Asc2Int(tmpHHSS, 4))
			return T_OK;

		sendFlg = TRUE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetDate[%s]", GetDate().GetBuffer(0));
		if (TranProc(TRAN_OPEN))								// 개국처리
		{														// Host Ok
			m_pDevCmn->HostStatus = HOST_ONLINE;				// 개국완료
			m_pDevCmn->HostFirstOpenFlag = TRUE;				// Host최초개국Flag
			m_pDevCmn->HostFirstReadyFlag = TRUE;
			m_pDevCmn->HostOpenFlag = TRUE;						// Host개국Flag
		}
		else 
		{														// Host Ng
			m_pDevCmn->HostStatus = HOST_WAITREADY;			// 준비완대기
			if (m_pDevCmn->HostOpenRetryCnt++ > 30)				// 개국시도횟수(준비완포함)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅 - RECV NG Cnt(%d)]", m_pDevCmn->HostOpenRetryCnt));
				m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT2;
			}
			else 
				m_pDevCmn->Timer.HostOpenRetryTime = OPEN_RETRY_TIME;		
		}
	}

	return T_OK;
}

