/////////////////////////////////////////////////////////////////////////////
// AtmClerkCtrl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
/////////////////////////////////////////////////////////////////////////////
/* ──────────────────────────────────────
   ▷ 파일변경이력
  ┌─────┬─────┬────┬────────────────────┐
  │A P 버 전 │ 변경일자 │항목번호│  변경내용(사유)                        │
  └─────┴─────┴────┴────────────────────┘
    V03-01-01   2005.07.12  SPL처리지원
									  SCD(P)의 SPL처리를 지원
   ──────────────────────────────────────*/
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
// 계원처리
int	CWinAtmCtrl::ClerkProc()
{
	// OM_로그파일받기
	// OM_장치리셋
	// OM_출금수표등록||출금수표번호||출금수표매수
	// OM_출금수표해제
	// OM_준비
	// OM_운영
	// OM_설치정보||설치날짜||점번||기번||...
	// OM_부가장치설정||전자화폐(가능,불가)||...
	// OM_LAN설정정보_CD||BP IP||BP Port||기기번호 (VANIII)
	// OM_LAN설정정보||IP주소||서브넷마스크||...   (C-ATM, T1,T2)
	// OM_회선정보||통신종류||DA||...
	// OM_ATMS설정정보
	// OM_거래정보설정||현금출금(가능,불가)||..
	// OM_입금권종선택||오천원권/천원권 
	// OM_카세트권종선택||구만(신만)||구만(신만)||구만(신만)||구만(신만)
	// OM_합계
	// OM_소계
	// OM_BRM_금액설정 (C-ATM, VANIII)
	// OM_회수금등록 (C-ATM, VANIII)
	// OM_전체초기화
	// OM_BRM초기화
	// OM_전체보충
	// OM_전체회수
	// OM_일부보충||보충매수
	// OM_일부회수||회수매수
	// OM_입출금모드선택
	// OM_카세트권종선택
	// OM_출금우선권종
	// OM_입금권종선택
	// OM_구만원권일부회수
	// OM_구천원권일부회수
	// OM_신만원권일부회수
	// OM_신천원권일부회수
	// OM_구만원권전체회수
	// OM_구천원권전체회수
	// OM_신만원권전체회수
	// OM_신천원권전체회수
	// OM_명세표인자||라인1||라인2||...||라인19
	// OM_저널인자||라인1||라인2||...||라인19
	// OM_명세표저널인자||라인1||라인2||...||라인19
	// OM_단체테스트
	// OM_기기설정||기기타입||명세표부종류
	// OM_수표권종선택||수표종류(13,14,15,16)
	// OM_거래일련번호변경||일련번호
	// OM_광고동영상
	// OM_종료
	// OM_카메라테스트||CAM1||CAM2||CAM3||CAM4 (C-ATM, VANIII)
	// OM_패스워드||패스워드 (VANIII)
	// OM_브랜드설정||편의점설정||브랜드설정 (VANIII)
	// OM_장치포트설정||SPACK 포트 (VANIII)
	// OM_모뎀정보설정||회선종류||폰번호||무선모뎀포트||ADSLID||ADSL PWD (VANIII)
	// OM_명세표설정||0,1
	// OM_버전인자||0,1
	// OM_거래일련번호변경||일련번호
	// OM_날짜시간설정||20120322122300
	// OM_거래설정정보||서비스거래설정(1,2,3)||수수료조회설정(0,1)||최대금액설정자행(30,50,70)||최대금액설정타행(30,50,70)	
	// OM_자동기리부팅
	// OM_슈퍼비밀번호변경||000000
	// OM_유지보수비밀번호변경||000000
	// OM_리부팅시간설정||000000
	// OM_DEVICECHECK||000000
	// OM_DEVICEREGULARCHECK||000000
	// OM_VOLUME||OM_VOLUME=설정값
	// OM_PINMODE||PINTYPE (VANIII)
	// OM_POWEROFFSTATUS (VANIII)
	// OM_CLOSESHUTTER (VANIII)
	// OM_OPENSHUTTER (VANIII)
	
	static	int		ClerkProcInCnt = 0;							// 재진입횟수조정
	static	CString	strOmResult("");							// OM결과
	static	int		SetScreen = OFF;
	static	int		nOperatorSwitchStatus = OPER_NONE;
	static	int		nOperatorSwitchStatusSave = OPER_NONE;

	CString			strOmCommand("");
	CStringArray	strOmCommandArray;
	CString			strTemp("");
	CString			strTemp2("");
	CStringArray	strTempArray;
	int				nTemp = 0;
	int				i = 0;
	int				nErrorFlag = FALSE;
	static	BOOL	bOperChange = FALSE;
/////////////////////////////////////////////////////////////////////////////
// SPL처리지원													// V03-01-01
	nOperatorSwitchStatus = m_pDevCmn->fnSNS_GetOperatorSwitch();
//////////////////////////////////////////////////////////////////////////
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		if(SetScreen)
		{
			SetScreen = OFF;								// Next Clerk Procedure Stop
			m_pDevCmn->fnUTL_SetClerkScreen_T3_Van();										// 화면교대
		}
		else
		if (nOperatorSwitchStatus == OPER_SUPERVISORCHANGED ||
			nOperatorSwitchStatus == OPER_RUNCHANGED )
		{
			if ((nOperatorSwitchStatusSave != nOperatorSwitchStatus) && !SetScreen )
			{
				nOperatorSwitchStatusSave = nOperatorSwitchStatus;
				nTemp = m_pTranCmn->fnAPP_AcceptOpPassword();
				if (nTemp == T_CANCEL) 
				{
					m_pDevCmn->AtmStatusReEntryFlag = FALSE;
					return TRUE; 
				}
				m_pDevCmn->OpenKey = FALSE;
				strOmResult = OM_NORMAL;
				m_pDevCmn->fnAPL_ClerkInformation();				// 계원정보
				SetShareData("OM_RESULT", strOmResult);				// 응답처리
				//////////////////////////////////////////////////////////////////////////
				SetScreen = ON;
				m_pDevCmn->fnAPL_UnUsed();							// Un Used Mode Setting
				//////////////////////////////////////////////////////////////////////////
			}

		}
	}
/*  //#0048
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		if ((OPER_SUPERVISORCHANGED == nOperatorSwitchStatus) || (OPER_RUNCHANGED == nOperatorSwitchStatus))
		{
			m_pDevCmn->fnSNS_ClearOperatorSwitch();				// Clear Sensor
			if (FALSE == SetScreen)
			{
				m_pDevCmn->fnCMR_PreviewToOM(FALSE);			// Preview시 OFF를 안하면 화면에 잔상이 남네요...
				m_pDevCmn->fnCMR_PreviewToFront(FALSE);
				SetScreen = TRUE;
				SetClerkScreen(TRUE);							// Front OM
				SetShareData("OM_화면상태", "전면");
				SetShareData("OM_COMMAND","OM_준비");
				SetShareData("OM_RESULT","DO");
			}
			else
			{
				m_pDevCmn->fnCMR_PreviewToOM(FALSE);
				m_pDevCmn->fnCMR_PreviewToFront(FALSE);
				SetScreen = FALSE;
				SetClerkScreen(FALSE);							// Rear OM
				SetShareData("OM_COMMAND","OM_운영");
				SetShareData("OM_RESULT","DO");
				m_pDevCmn->fnSCR_DisplayScreen(774);			// Black Screen
			}
		}
	}
*/
/////////////////////////////////////////////////////////////////////////////
	if (GetShareData("OM_RESULT") != OM_DO)						// 실행요청무
		return TRUE;

/////////////////////////////////////////////////////////////////////////////
	if (ClerkProcInCnt > 0)										// 재진입횟수조정
	{
		ClerkProcInCnt--;
		if (ClerkProcInCnt == 0)
		{
			m_pDevCmn->fnAPL_ClerkInformation();				// 계원정보
			SetShareData("OM_RESULT", strOmResult);				// 응답처리
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult[%s]", strOmResult);
			return TRUE;
		}
	}

/////////////////////////////////////////////////////////////////////////////
	strOmCommand = GetShareData("OM_COMMAND");					// 커맨드,파라메터...
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmCommand[%s]", strOmCommand);
	SplitString(strOmCommand, "||", strOmCommandArray);
	
/////////////////////////////////////////////////////////////////////////////
	strOmResult = OM_ERROR;										// 기본장애지정
// Get Log Start ───────────────────────
	if (strOmCommandArray[0] == "OM_로그파일받기")
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[로그파일받기]"));

		if (strOmCommandArray.GetSize() < 3)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||로그파일받기 오류입니다";
		}
		else
		{
			if (m_pDevCmn->fnAPL_GetLogFileStart(Asc2Int(strOmCommandArray[1]), Asc2Int(strOmCommandArray[2])))
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||로그파일 받기가 정상처리되었습니다.";
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||USB 장치를 확인하여 주십시오.";
			}
		}
	}	
	else
	if (strOmCommandArray[0] == "OM_장치리셋")
	{
		m_pDevCmn->RecoveryType = 1;							// 0:자동복구,1:수동복구
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[장치리셋]"), JNL_OPER);
		m_pDevCmn->fnAPL_SetSensorInfo();							// 센서정보설정
//t		if (m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetErrorDevice(m_pDevCmn->DeviceStatus)))
//		if (m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetErrorDevice(m_pDevCmn->DeviceStatus | m_pDevCmn->DeviceNoDevice)))	// 2003.11.11
		if (m_pDevCmn->RejectCSTLoc != NORMAL_POS)
		{
			strOmResult = OM_ERROR;
			strOmResult += "|| [ REJECT BOX 위치점검 ]";
		}
		else
		if (m_pDevCmn->RetractCSTLoc != NORMAL_POS)
		{
			strOmResult = OM_ERROR;
			strOmResult += "|| [ RETRACT BOX 정위치 점검]";
		}
		else
		if (m_pDevCmn->BrmLoc != NORMAL_POS)
		{
			strOmResult = OM_ERROR;
			strOmResult += "|| [ BRM 정위치 점검 ]";
		}
		else
		if(m_pDevCmn->FstCSTStatus == CST_SET_NG)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||1카세트 탈착";
		}
		else
		if(m_pDevCmn->SndCSTStatus == CST_SET_NG)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||2카세트 탈착";
		}
		else
		if(m_pDevCmn->TrdCSTStatus == CST_SET_NG)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||3카세트 탈착";
		}
		else //#0063
		{
			m_pDevCmn->RecoveryType = 1;							// 0:자동복구,1:수동복구
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[장치리셋]"), JNL_OPER);
			m_pDevCmn->fnAPL_ClearError();							// 장애정보초기화
			m_pDevCmn->DeviceAutoOff = 0;							// 자동축퇴가 난 Device = Set
			m_pDevCmn->DeviceLoss = DEV_NONE;						// 분실정보초기화
			m_pDevCmn->ForceDownFlag = FALSE;						// 강제Down변수
			m_pDevCmn->RemoteDownFlag = FALSE;						// 강제Down변수 
			m_pDevCmn->fnAPL_CshRetactCntInfo(0);								// 회수횟수 Clear;
			if (m_pDevCmn->fnAPL_ResetDevice(DEV_MAIN))
			{
				m_pDevCmn->fnAPL_ClearError();							// 장애정보초기화
				strOmResult = OM_NORMAL;
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||장치리셋 FAIL!";
			}
		}
		m_pDevCmn->RecoveryType = 0;							// 0:자동복구,1:수동복구
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[장치리셋 %s]", strOmResult));
	}
	else
	if (strOmCommandArray[0] == "OM_출금수표합계")
	{
 		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[출금수표합계]"));

		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == (DEV_SPR | DEV_JPR)) 
		{

			TranProc(TRAN_CHECK_UNREG);
			if (m_pDevCmn->TranResult)
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||수표합계가 처리되었습니다";
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||수표합계중 장애가 발생했습니다";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표,저널 장애 상태입니다";
		}

	}
	else
	if (strOmCommandArray[0] == "OM_출금수표등록")
	{

 		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[출금수표등록]"));

		if (strOmCommandArray.GetSize() < 7)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||수표등록 입력 오류입니다";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == (DEV_SPR | DEV_JPR)) 
		{
			// 은행코드설정
			strTemp = ZERO2 + strOmCommandArray[1];
			strTemp = strTemp.Right(2);
			strTemp2 = ZERO2 + strOmCommandArray[4];
			strTemp2 = strTemp2.Right(2);
			m_pDevCmn->fnAPL_SetOutCheckBankNo(strTemp, strTemp2);

			strTemp.Empty();	strTemp2.Empty();

			// 수표번호설정
			strTemp = ZERO8 + strOmCommandArray[2];
			strTemp = strTemp.Right(8);
			strTemp2 = ZERO8 + strOmCommandArray[5];
			strTemp2 = strTemp2.Right(8);
			m_pDevCmn->fnAPL_SetNumberOfOutCheckNo(strTemp, strTemp2);

			// 수표매수설정
			m_pDevCmn->fnAPL_SetNumberOfOutCheck(Asc2Int(strOmCommandArray[3]), Asc2Int(strOmCommandArray[6]));

			TranProc(TRAN_CHECK_REG);
			if (m_pDevCmn->TranResult)
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||수표등록이 처리되었습니다";
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||수표등록중 장애가 발생했습니다";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표,저널 장애 상태입니다";
		}

	}
	else
	if (strOmCommandArray[0] == "OM_준비")
	{
		m_pTranCmn->Om_GetCurrentActionRemain();
		m_pDevCmn->OpenKey = FALSE;
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[준비]"));
		strOmResult = OM_NORMAL;

	}
	else
	if (strOmCommandArray[0] == "OM_운영")
	{

		int nCurRB2Cnt = 0;
		int nCurRB1Cnt = 0;
		int nSaveCurRB2Cnt = 0;
		int nSaveCurRB1Cnt = 0;
		int nSetCurRB2Cnt = 0;
		int nSetCurRB1Cnt = 0;
		int nSetCurTotCnt = 0;

		switch (m_pProfile->DEVICE.MachineType)
		{
			case U3100K:
				if(m_pDevCmn->FstCSTStatus == CST_SET_NG)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||1카세트 탈착";
				}
				else
				if(m_pDevCmn->SndCSTStatus == CST_SET_NG)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||2카세트 탈착";
				}
				else
				if(m_pDevCmn->TrdCSTStatus == CST_SET_NG)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||3카세트 탈착";
				}
				else
				if(m_pDevCmn->RejectCSTStatus == CST_SET_NG || m_pDevCmn->RejectCSTStatus == CST_FULL) //#H0020
				{
					strOmResult = OM_ERROR;
					strOmResult += "||REJECX NG,FULL";
				}
				else
				if(m_pDevCmn->RetractCSTStatus == CST_SET_NG || m_pDevCmn->RetractCSTStatus == CST_FULL) //#H0020
				{
					strOmResult = OM_ERROR;
					strOmResult += "||RETRACT NG,FULL";
				}
				else
				if(m_pDevCmn->BrmLoc != NORMAL_POS)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||BRM 탈착";
				}
				else
				if(m_pDevCmn->DoorStatus == DOOR_OPENED)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||문열림";
				}
				else
				if(m_pDevCmn->CabinetStatus == DOOR_OPENED)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||상단부문열림";
				}
				else
				if(m_pDevCmn->RejectCSTStatus == CST_SET_NG || m_pDevCmn->RetractCSTStatus == CST_SET_NG)
				{
					strOmResult = OM_ERROR;
					strOmResult += "||회수함,리젝트박스 탈착";
				}
				else
				if (!m_pDevCmn->fnAPL_ResetDevice(DEV_CSH))
				{
					m_pDevCmn->OpenKey = TRUE;
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[운영-FAIL!!]"));
					strOmResult = OM_NORMAL;
					m_pDevCmn->fnAPL_ClerkInformation();									// 계원정보
					m_pDevCmn->fnUTL_SetClerkScreen_T3_Van();										// 화면교대
					SetScreen = OFF;
					if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT4)
					{
						m_pDevCmn->AtmStatus = ATM_DOWN;
					}							
				}
				else
				{
					int nRB1NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKindUp");
					int nRB2NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteKindUp");
			//		int nRB3NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteKindUp");
					int nNoteKind1     = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKind");
					int nNoteKind2     = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteKind");
			//		int nNoteKind3     = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteKind");
	
					int nRB1NoteCnt = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt");
					int nRB2NoteCnt = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt");
					int nRB3NoteCnt = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt");
					CString strST1  = RegGetStr(_REGKEY_BRMSTATUS, "RB1Status");
					CString strST2  = RegGetStr(_REGKEY_BRMSTATUS, "RB2Status");
					CString strST3  = RegGetStr(_REGKEY_BRMSTATUS, "RB3Status");
	
					if(nRB1NoteKindUp != nNoteKind1)
					{
						strOmResult = OM_ERROR;
						strOmResult += "||1카세트 점퍼 설정 오류(5만원)"; //#0063
					}
					else
					if(nRB2NoteKindUp != nNoteKind2)
					{
						strOmResult = OM_ERROR;
						strOmResult += "||2카세트 점퍼 설정 오류(1만원)"; //#0063
					}
/*
					else
					if((0 < nRB1NoteCnt) && (strST1.CompareNoCase("EMPTY") == 0))
					{
						strOmResult = OM_ERROR;
						strOmResult += "||1카세트 금액 설정 오류"; //#0063
					}
					else
					if((0 < nRB2NoteCnt) && (strST2.CompareNoCase("EMPTY") == 0))
					{
						strOmResult = OM_ERROR;
						strOmResult += "||2카세트 금액 설정 오류"; //#0063
					}
					else
					if((0 < nRB3NoteCnt) && (strST3.CompareNoCase("EMPTY") == 0))
					{
						strOmResult = OM_ERROR;
						strOmResult += "||3카세트 금액 설정 오류"; //#0063
					}
*/
					else
					{
						m_pDevCmn->OpenKey = TRUE;
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[운영]"));
						strOmResult = OM_NORMAL;
						m_pDevCmn->fnAPL_ClerkInformation();									// 계원정보
						m_pDevCmn->fnUTL_SetClerkScreen_T3_Van();										// 화면교대
						SetScreen = OFF;

						//현재 잔액
						if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT4)
						{
							m_pDevCmn->AtmStatus = ATM_DOWN;
						}
					}
				}
				break;
			case U8100:
				m_pDevCmn->OpenKey = TRUE;
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[운영]"));
				m_pDevCmn->fnCMR_PreviewToOM(FALSE);
				m_pDevCmn->fnCMR_PreviewToFront(FALSE);
				strOmResult = OM_NORMAL;
				SetClerkScreen(FALSE);
				SetScreen = OFF;

				strTemp = IniGetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_T0GAP",   "FFFF");
				if (strTemp.CompareNoCase("FFFF") ==0)
				{
					m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);
				}
				else
				{
//					m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);
					m_pTranCmn->Om_SetActionGapRemainAsycSend();
				}

				if(m_pDevCmn->m_nAtmsCommand == ATMS_CMD_REBOOT4)
				{
					m_pDevCmn->AtmStatus = ATM_DOWN;
				}
				break;
			default:
				break;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_설치정보")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[설치정보]"));
		if (strOmCommandArray.GetSize() < 17)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||설치정보 입력 오류입니다";
		}
		else
		{
			m_pProfile->GetNetworkProfile();

			// 0:설치날짜(YYYYMMDD)
			strTemp = ZERO8 + strOmCommandArray[1];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.MachineMakeDate, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.MachineMakeDate[8] = NULL;

			// 1:소속지점1(NNNNNNNN)
			strTemp = ZERO8 + strOmCommandArray[2];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.BranchNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.BranchNum[8] = NULL;
			
			// 2:기번(NNNNNNNN)
			strTemp = ZERO8 + strOmCommandArray[3];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.AtmSerialNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.AtmSerialNum[8] = NULL;
			
			// 3:소속지점2(NNNNNNNN)
			strTemp = ZERO8 + strOmCommandArray[4];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.SubBranchNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.SubBranchNum[8] = NULL;
			
			// 4:점내(ON,OFF)
			if (!strOmCommandArray[5].CompareNoCase("ON"))
				m_pProfile->NETWORK.BranchInOut = TRUE;
			else
				m_pProfile->NETWORK.BranchInOut = FALSE;

			// 5:처리지점(NNNNNNNN)
			strTemp = ZERO8 + strOmCommandArray[6];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.RoomNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.RoomNum[8] = NULL;
			
			// 6:무인점포(ON,OFF)
			if (!strOmCommandArray[7].CompareNoCase("ON"))
				m_pProfile->NETWORK.AutoBranchOnOff = TRUE;
			else
				m_pProfile->NETWORK.AutoBranchOnOff = FALSE;
			
			// 7:24시간운영(ON,OFF)
			if (!strOmCommandArray[8].CompareNoCase("ON"))
				m_pProfile->NETWORK.All24OnOff = TRUE;
			else
				m_pProfile->NETWORK.All24OnOff = FALSE;
			
			// 8:365운영(ON,OFF)
			if (!strOmCommandArray[9].CompareNoCase("ON"))
				m_pProfile->NETWORK.All365OnOff = TRUE;
			else
				m_pProfile->NETWORK.All365OnOff = FALSE;
			
			// 9:CE사무소코드(NNNN)
			strTemp = strOmCommandArray[10] + SPACE4;
			strTemp = strTemp.Left(4);
			memcpy(m_pProfile->NETWORK.CeOfficeCode, strTemp.GetBuffer(0), 4);
			m_pProfile->NETWORK.CeOfficeCode[4] = NULL;
			
			// 10:기기관리번호(NNNNNNNNNNNNNNNN)
			strTemp = strOmCommandArray[11] + SPACE12;
			strTemp = strTemp.Left(16);
			memcpy(m_pProfile->NETWORK.MachineMakeNum, strTemp.GetBuffer(0), 16);
			m_pProfile->NETWORK.MachineMakeNum[16] = NULL;
			
			// 11:출장소(NNNNNNNN)
			strTemp = ZERO8 + strOmCommandArray[12];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.OutBranchNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.OutBranchNum[8] = NULL;
			
			// 12:설치자(NNNNNNNNNN)
			strTemp = strOmCommandArray[13] + SPACE10;
			strTemp = strTemp.Left(10);
			memcpy(m_pProfile->NETWORK.InstName, strTemp.GetBuffer(0), 10);
			m_pProfile->NETWORK.InstName[10] = NULL;
			
			// 13:설치확인자(NNNNNNNNNN)
			strTemp = strOmCommandArray[14] + SPACE10;
			strTemp = strTemp.Left(10);
			memcpy(m_pProfile->NETWORK.InstConfirmName, strTemp.GetBuffer(0), 10);
			m_pProfile->NETWORK.InstConfirmName[10] = NULL;
			
			// 14:보수담당자(NNNNNNNNNN)
			strTemp = strOmCommandArray[15] + SPACE10;
			strTemp = strTemp.Left(10);
			memcpy(m_pProfile->NETWORK.CeName, strTemp.GetBuffer(0), 10);
			m_pProfile->NETWORK.CeName[10] = NULL;
			
			// 15:보수담당자연락처(NNNNNNNNNNNNNNNN)
			strTemp = strOmCommandArray[16] + SPACE16;
			strTemp = strTemp.Left(16);
			memcpy(m_pProfile->NETWORK.CeTel, strTemp.GetBuffer(0), 16);
			m_pProfile->NETWORK.CeTel[16] = NULL;

			m_pProfile->PutNetworkProfile();
			m_pDevCmn->fnNET_Initialize(LINE_OPCL_TIME);					// Ip setting initialize
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_부가장치설정")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[부가장치설정]"));
		if (strOmCommandArray.GetSize() < 7)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||부가장치설정 입력 오류입니다";
		}
		else
		{

			m_pProfile->GetDeviceProfile();

			// 0:IC거래(가능,불가)
			if (!strOmCommandArray[1].CompareNoCase("가능"))
				m_pProfile->DEVICE.ICCardDevice = ICCARD_SET;
			else
				m_pProfile->DEVICE.ICCardDevice = NOT_SET;

			// 1:해외거래(가능,불가)
			if (!strOmCommandArray[2].CompareNoCase("가능"))
				m_pProfile->DEVICE.DesHandler = T_DES_SET;
			else
				m_pProfile->DEVICE.DesHandler = NOT_SET;
			
			// 2:지문거래(가능,불가)
			if (!strOmCommandArray[3].CompareNoCase("가능"))
				m_pProfile->DEVICE.FingerHandler = FINGER_SET;
			else
				m_pProfile->DEVICE.FingerHandler = NOT_SET;

			// 3:적외선거래(가능,불가)
			if (!strOmCommandArray[4].CompareNoCase("가능"))
				m_pProfile->DEVICE.IrdaHandler = IRDA_SET;
			else
				m_pProfile->DEVICE.IrdaHandler = NOT_SET;
			
			// 4:바코드거래(가능,불가)
			if (!strOmCommandArray[5].CompareNoCase("가능"))
				m_pProfile->DEVICE.BarHandler = BAR_SET;
			else
				m_pProfile->DEVICE.BarHandler = NOT_SET;

			// 5:암호화거래(가능,불가)
			if (!strOmCommandArray[6].CompareNoCase("가능"))
				m_pProfile->DEVICE.SeedHandler = SEED_SET;
			else
				m_pProfile->DEVICE.SeedHandler = NOT_SET;

			m_pProfile->PutDeviceProfile();
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_LAN설정정보_CD")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LAN설정정보]"));
		if (strOmCommandArray.GetSize() < 4)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||LAN설정정보 입력 오류입니다";
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LAN %s %s %s]", strOmCommandArray[1], strOmCommandArray[2], strOmCommandArray[3]));
			m_pProfile->GetNetworkProfile();					// Get Network Profile

			strTemp.Empty();
			strTemp = ZERO12 + strOmCommandArray[1];
			strTemp = strTemp.Right(12);
			strTemp.Format("%3.3s.%3.3s.%3.3s.%3.3s", 
							strTemp.Mid(0,3), 
							strTemp.Mid(3,3), 
							strTemp.Mid(6,3),
							strTemp.Mid(9,3));
			memcpy(m_pProfile->NETWORK.BpIpAddress, strTemp.GetBuffer(0), 15);
			m_pProfile->NETWORK.BpIpAddress[15] = NULL;

			strTemp.Empty();
			strTemp = ZERO5 + strOmCommandArray[2];
			strTemp = strTemp.Right(5);
			memcpy(m_pProfile->NETWORK.BpPortNo, strTemp.GetBuffer(0), 5);
			m_pProfile->NETWORK.BpPortNo[5] = NULL;

			strTemp.Empty();
			strTemp = ZERO14 + strOmCommandArray[3];
			strTemp = strTemp.Right(14);

			memcpy(m_pProfile->NETWORK.BranchNum, strTemp.GetBuffer(0), 6);
			m_pProfile->NETWORK.BranchNum[6] = '0';
			m_pProfile->NETWORK.BranchNum[8] = '0';
			m_pProfile->NETWORK.BranchNum[8] = NULL;

			memcpy(m_pProfile->NETWORK.AtmSerialNum, &strTemp.GetBuffer(0)[6], 8);

			strTemp2.Format("%6.6s", &m_pProfile->NETWORK.AtmSerialNum[2]);

			IniSetStr(_CH_SSLVPN_INI, "ATM", "VPN_IP",  "211.55.3.31");
			IniSetStr(_CH_SSLVPN_INI, "ATM", "VPN_ID",  strTemp2);


			m_pProfile->NETWORK.AtmSerialNum[8] = NULL;
// 			DWORD dwAtmPos = Asc2Int(strTemp.Left(5));
// 
// 			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OM_LAN설정정보_CD: dwAtmPos %d", dwAtmPos);
// 			switch (dwAtmPos)	
// 			{
// 				case 0:
// 				case 1:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "010000", 6);
// 					break;
// 				case 2:
// 				case 3:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "013000", 6);
// 					break;
// 				case 4:
// 				case 5:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "020000", 6);
// 					break;
// 				case 6:
// 				case 7:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "023000", 6);
// 					break;
// 				case 8:
// 				case 9:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "030000", 6);
// 					break;
// 				default:
// 					memcpy(m_pProfile->TRANS.RebootHHMMSS, "010000", 6);
// 					break;
// 
// 			}
			
			m_pProfile->PutTransProfile();



// 			char	szStartRebootTime[256];
// 			char	szEndRebootTime[256];
// 			memset(szStartRebootTime, 0x00, sizeof(szStartRebootTime));
// 			memset(szEndRebootTime,   0x00, sizeof(szEndRebootTime));
// 			memcpy(szStartRebootTime,  m_pProfile->TRANS.RebootHHMMSS, 6);
// 			memcpy(szEndRebootTime,    m_pProfile->TRANS.RebootHHMMSS, 6);
// 			AddString(szEndRebootTime, "010000", 6);

// 			if(memcmp(m_pProfile->TRANS.RebootHHMMSS, ZERO6, 6) != 0)
// 			{
// 				RegSetInt(_REGKEY_ATMRESTART, "UseRestart"    , 1);
// 				RegSetStr(_REGKEY_ATMRESTART, "CheckStartTime", szStartRebootTime);
// 				RegSetStr(_REGKEY_ATMRESTART, "CheckEndTime"  , szEndRebootTime);
// 				RegSetInt(_REGKEY_ATMRESTART, "ReadyRestart"  , 0);
// 			}
// 
// 			m_pDevCmn->AtmRestartInfo.bUseRestart		= RegGetInt(_REGKEY_ATMRESTART, "UseRestart", 1);
// 			m_pDevCmn->AtmRestartInfo.strCheckStartTime= RegGetStr(_REGKEY_ATMRESTART, "CheckStartTime", "040000");
// 			m_pDevCmn->AtmRestartInfo.strCheckEndTime	= RegGetStr(_REGKEY_ATMRESTART, "CheckEndTime", "050000");
// 			m_pDevCmn->AtmRestartInfo.bReadyRestart	= RegGetInt(_REGKEY_ATMRESTART, "ReadyRestart", 0);
// 			
// 			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OM_LAN설정정보_CD: CheckStartTime[%s] ~ CheckEndTime[%s]", szStartRebootTime, szEndRebootTime);
// 	
			
			m_pProfile->PutNetworkProfile();
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT4;
//			m_pDevCmn->AtmStatus = ATM_DOWN;
			strOmResult = OM_NORMAL;
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LAN %s]", strOmResult));
		}

	}
	else
	if (strOmCommandArray[0] == "OM_LAN설정정보")
	{
		if (strOmCommandArray.GetSize() < 8)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||LAN설정정보 입력 오류입니다";
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LAN설정정보]"));

			m_pProfile->GetNetworkProfile();

			// 0:IP주소(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[1], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				memcpy(m_pProfile->NETWORK.AtmIPAddress, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmIPAddress[15] = NULL;
			}
			else
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", 0, 0, 0, 0);
				memcpy(m_pProfile->NETWORK.AtmIPAddress, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmIPAddress[15] = NULL;
			}

			// 1:서브넷마스크(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[2], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				memcpy(m_pProfile->NETWORK.AtmSubnetMask, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmSubnetMask[15] = NULL;
			}
			else
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", 0, 0, 0, 0);
				memcpy(m_pProfile->NETWORK.AtmSubnetMask, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmSubnetMask[15] = NULL;
			}
			
			// 2:게이트웨이(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[3], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				memcpy(m_pProfile->NETWORK.AtmGateWay, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmGateWay[15] = NULL;
			}
			else
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", 0, 0, 0, 0);
				memcpy(m_pProfile->NETWORK.AtmGateWay, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.AtmGateWay[15] = NULL;
			}

			// 3:BP1IP주소(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[4], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				memcpy(m_pProfile->NETWORK.BpIpAddress, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.BpIpAddress[15] = NULL;
			}
			else
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", 0, 0, 0, 0);
				memcpy(m_pProfile->NETWORK.BpIpAddress, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.BpIpAddress[15] = NULL;
			}

			// 4:BP1PORT(NNNNN)
			strTemp.Format("%5.5d", Asc2Int(strOmCommandArray[5]));
			memcpy(m_pProfile->NETWORK.BpPortNo, strTemp.GetBuffer(0), 5);
			m_pProfile->NETWORK.BpPortNo[5] = NULL;
			
			// 5:BP2IP주소(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[6], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				memcpy(m_pProfile->NETWORK.BpIpAddress2, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.BpIpAddress2[15] = NULL;
			}
			else
			{
				strTemp.Format("%3.3d.%3.3d.%3.3d.%3.3d", 0, 0, 0, 0);
				memcpy(m_pProfile->NETWORK.BpIpAddress2, strTemp.GetBuffer(0), 15);
				m_pProfile->NETWORK.BpIpAddress2[15] = NULL;
			}

			// 6:BP2PORT(NNNNN)
			strTemp.Format("%5.5d", Asc2Int(strOmCommandArray[7]));
			memcpy(m_pProfile->NETWORK.BpPortNo2, strTemp.GetBuffer(0), 5);
			m_pProfile->NETWORK.BpPortNo2[5] = NULL;
			
			m_pProfile->PutNetworkProfile();
			// IP Address 변경 Library 적용		V01.03.01 #2
			m_pDevCmn->fnAPL_NetworkInfoSet();
//			m_pDevCmn->AtmInfoINI();						// atmInfo setting for BMGuard
			strOmResult = OM_NORMAL;
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LAN설정정보 %s]", strOmResult));
		}
	}
	else
	if (strOmCommandArray[0] == "OM_회선정보")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[회선정보]"));
		if (strOmCommandArray.GetSize() < 13)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||회선정보 입력 오류입니다";
		}
		else
		{
			m_pProfile->GetNetworkProfile();

			// 0:통신종류(IOC,HDLC,SWP,SDLC,TCP/IP,CCL,BAS)
			if (strOmCommandArray[1] == "IOC")
				m_pProfile->NETWORK.Interface = NET_IOC;
			else
			if (strOmCommandArray[1] == "HDLC")
				m_pProfile->NETWORK.Interface = NET_HDLC;
			else
			if (strOmCommandArray[1] == "SWP")
				m_pProfile->NETWORK.Interface = NET_SWP;
			else
			if (strOmCommandArray[1] == "SDLC")
				m_pProfile->NETWORK.Interface = NET_SDLC;
			else
			if (strOmCommandArray[1] == "TCP/IP")
				m_pProfile->NETWORK.Interface = NET_TCPIP;
			else
			if (strOmCommandArray[1] == "CCL")
				m_pProfile->NETWORK.Interface = NET_CCL;
			else
				m_pProfile->NETWORK.Interface = NET_BAS;

			// 1:DA(XX)
			strTemp = ZERO2 + strOmCommandArray[2];
			strTemp = strTemp.Right(2);
			Asc2Byte(strTemp.GetBuffer(0), &m_pProfile->NETWORK.DeviceAddr);
			
			// 2:SA(XX)
			strTemp = ZERO2 + strOmCommandArray[3];
			strTemp = strTemp.Right(2);
			Asc2Byte(strTemp.GetBuffer(0), &m_pProfile->NETWORK.SelectAddr);
			
			// 3:송수신사이즈
			strTemp = ZERO5 + strOmCommandArray[4];
			strTemp = strTemp.Right(5);
			m_pProfile->NETWORK.SendRecvSize = Asc2Int(strTemp);
			
			// 4:회선속도(1200,2400,4800,9600,19200)
			nTemp = Asc2Int(strOmCommandArray[5]);
			if (nTemp == 1200)
				m_pProfile->NETWORK.LineSpeed = L1200;
			else
			if (nTemp == 2400)
				m_pProfile->NETWORK.LineSpeed = L2400;
			else
			if (nTemp == 4800)
				m_pProfile->NETWORK.LineSpeed = L4800;
			else
			if (nTemp == 9600)
				m_pProfile->NETWORK.LineSpeed = L9600;
			else
				m_pProfile->NETWORK.LineSpeed = L19200;
			
			// 5:POLLCHECK(ON,OFF)
			if (!strOmCommandArray[6].CompareNoCase("ON"))
				m_pProfile->NETWORK.PollCheck = POLL_ON;
			else
				m_pProfile->NETWORK.PollCheck = POLL_OFF;
			
			// 6:POINT(PTP,MTP)
			if (!strOmCommandArray[7].CompareNoCase("PTP"))
				m_pProfile->NETWORK.Point = PTP;
			else
				m_pProfile->NETWORK.Point = MTP;
			
			// 7:DUPLEX(HALF,FULL)
			if (!strOmCommandArray[8].CompareNoCase("HALF"))
				m_pProfile->NETWORK.Duplex = HALF_DUPLEX;
			else
				m_pProfile->NETWORK.Duplex = FULL_DUPLEX;
			
			// 8:TIMEFILLER(MTF,FTF)
			if (!strOmCommandArray[9].CompareNoCase("MTF"))
				m_pProfile->NETWORK.TimeFiller = MTF;
			else
				m_pProfile->NETWORK.TimeFiller = FTF;
			
			// 9:RSTYPE(RS-422,RS-232C)
			if (!strOmCommandArray[10].CompareNoCase("RS-422"))
				m_pProfile->NETWORK.RsType = RS422;
			else
				m_pProfile->NETWORK.RsType = RS232C;
			
			// 10:NRZ(NRZ,NRZI)
			if (!strOmCommandArray[11].CompareNoCase("NRZ"))
				m_pProfile->NETWORK.Nrz = NRZ;
			else
				m_pProfile->NETWORK.Nrz = NRZI;
			
			// 11:CLOCK(INCLK,EXCLK)
			if (!strOmCommandArray[12].CompareNoCase("INCLK"))
				m_pProfile->NETWORK.Clk = INCLK;
			else
				m_pProfile->NETWORK.Clk = EXCLK;

			m_pProfile->PutNetworkProfile();
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_ATMS설정정보")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[ATMS설정정보]"));
		if (strOmCommandArray.GetSize() < 6)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||ATMS설정정보 입력 오류입니다";
		}
		else
		{
			// 1:SERVER IP주소(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[1], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%d.%d.%d.%d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP", strTemp);
				RegSetStr(_REGKEY_REMOTECTRL, "TCP_IP",   strTemp);
			}
			else
			{
				strTemp.Format("%d.%d.%d.%d", 0, 0, 0, 0);
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP", strTemp);
				RegSetStr(_REGKEY_REMOTECTRL, "TCP_IP",   strTemp);
			}

			// 2:SERVER PORT(NNNNN)
			strTemp.Format("%d", Asc2Int(strOmCommandArray[2]));
			IniSetStr(_ATMS_FTP_INI, "TCP_IP", "Port", strTemp);
			RegGetInt(_REGKEY_REMOTECTRL, "TCP_PORT", Asc2Int(strOmCommandArray[2]));
			
			// 3:예비SERVER1 IP주소(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[3], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%d.%d.%d.%d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTONE", strTemp);
			}
			else
			{
				strTemp.Format("%d.%d.%d.%d", 0, 0, 0, 0);
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTONE", strTemp);
			}

			// 4:예비SERVER2 IP주소(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[4], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%d.%d.%d.%d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTWO", strTemp);
			}
			else
			{
				strTemp.Format("%d.%d.%d.%d", 0, 0, 0, 0);
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTWO", strTemp);
			}

			// 5:예비SERVER3 IP주소(NNN.NNN.NNN.NNN)
			SplitString(strOmCommandArray[5], ".", strTempArray);
			if (strTempArray.GetSize() >= 4)
			{
				strTemp.Format("%d.%d.%d.%d", Asc2Int(strTempArray[0]),	
														  Asc2Int(strTempArray[1]), 
														  Asc2Int(strTempArray[2]),	
														  Asc2Int(strTempArray[3]));
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTHREE", strTemp);
			}
			else
			{
				strTemp.Format("%d.%d.%d.%d", 0, 0, 0, 0);
				IniSetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTHREE", strTemp);
			}
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_거래정보설정")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래정보설정]"));
		if (strOmCommandArray.GetSize() < 6)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||거래정보설정 입력 오류입니다";
		}
		else
		{
			m_pProfile->GetTransProfile();

			// 0:현금출금(가능,불가)
			if (strOmCommandArray[1] == "가능")
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_WITH_CASH;
			else
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_WITH_CASH;

			// 1:현금입금(가능,불가)
			if (strOmCommandArray[2] == "가능")
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_DEP_CASH;
			else
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_DEP_CASH;
			
			// 2:수표출금(가능,불가)
			if (strOmCommandArray[3] == "가능")
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_WITH_CHECK;
			else
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_WITH_CHECK;
			
			// 3:수표입금(가능,불가)
			if (strOmCommandArray[4] == "가능")
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_DEP_CHECK;
			else
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_DEP_CHECK;
			
			// 4:통장거래(가능,불가)
			if (strOmCommandArray[5] == "가능")
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_PBM_OK;
			else
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_PBM_OK;

			// 5:5만원권(가능,불가)
			if ((m_pDevCmn->ChangeBCFlag == FALSE)	||
				(strOmCommandArray[6] == "불가"))				
			{
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_WITH_50TCASH;
//				m_pDevCmn->Withdraw50TFlag = FALSE;			
			}
			else
			{
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_WITH_50TCASH;
				m_pDevCmn->Withdraw50TFlag = TRUE;			
			}

			m_pProfile->PutDeviceProfile();
			m_pProfile->PutTransProfile();
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_합계")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[합계]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR | DEV_JPR)) 
		{
			m_pDevCmn->fnAPL_SetSensorInfo();							// 센서정보설정
			// 현송시 BRM정위치 점검 
			if (m_pDevCmn->RejectCSTLoc != NORMAL_POS)
			{
				strOmResult = OM_ERROR;
				strOmResult += "|| [ REJECT BOX 위치점검 ]";
			}
			else
			if (m_pDevCmn->RetractCSTLoc != NORMAL_POS)
			{
				strOmResult = OM_ERROR;
				strOmResult += "|| [ RETRACT BOX 정위치 점검]";
			}
			else
			if (m_pDevCmn->BrmLoc != NORMAL_POS)
			{
				strOmResult = OM_ERROR;
				strOmResult += "|| [ BRM 정위치 점검 ]";
			}
			else
			if(m_pDevCmn->FstCSTStatus == CST_SET_NG)
			{
				strOmResult = OM_ERROR;
				strOmResult += "||1카세트 탈착";
			}
			else
			if(m_pDevCmn->SndCSTStatus == CST_SET_NG)
			{
				strOmResult = OM_ERROR;
				strOmResult += "||2카세트 탈착";
			}
			else
			if(m_pDevCmn->TrdCSTStatus == CST_SET_NG)
			{
				strOmResult = OM_ERROR;
				strOmResult += "||3카세트 탈착";
			}
			else
			{
				if(m_pProfile->DEVICE.MachineType == U3100K)
				{
				
					m_pDevCmn->FstCSTSetCash = 10000;
					m_pDevCmn->SndCSTSetCash = 10000;
					m_pDevCmn->TrdCSTSetCash = 10000;
					int nNoteKind1 = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKind", 0);

					if (NOTE_KIND_50000_USE == nNoteKind1)
						IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K", m_pDevCmn->FstCSTSetCash);
					else
						IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K", m_pDevCmn->FstCSTSetCash);										
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);

					//init cst2
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
					//init cst3
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K", m_pDevCmn->TrdCSTSetCash);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
					//init cst4
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

					
					strTemp.Empty();
					strTemp.Format("%8.8s%6.6s", GetDate(), GetTime());
					m_pDevCmn->fnUTL_IniSetInt(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedCstInfo", TRUE);
					m_pDevCmn->fnUTL_IniSetStr(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedDateTime", strTemp);
	//				m_pDevCmn->fnCSH_Initialize();						
				}
				else
				if(m_pProfile->DEVICE.MachineType == U8100)
				{
					strTemp.Empty();
					strTemp.Format("%8.8s%6.6s", GetDate(), GetTime());
					m_pDevCmn->fnUTL_IniSetStr(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedDateTime", strTemp);
					
				}

				TranProc(TRAN_TOTAL);								// 합계처리
				if (m_pDevCmn->TranResult)							// HOST OK
				{
					strOmResult = OM_NORMAL;
					strOmResult += "||합계 완료";
				}
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||합계처리 실패";
				}
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표,저널부 장애 상태입니다";
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[합계 %s]", strOmResult));
	}
	else
	if (strOmCommandArray[0] == "OM_소계")
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[소계]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR | DEV_JPR)) 
		{
			TranProc(TRAN_SUBTOTAL);							// 소계처리
			strOmResult = OM_NORMAL;
			strOmResult += "||소계 완료";
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||장애 상태입니다";
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[소계 %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_BRM_금액설정")
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[BRM_금액설정]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_JPR) == m_pDevCmn->fnAPL_GetDefineDevice(DEV_JPR)) 
		{
			if (strOmCommandArray.GetSize() >= 5)
			{
				//C-ATM Set Amt
				if (Asc2Int(strOmCommandArray[1]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[2]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[3]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[4]) > 10000)
					nErrorFlag = TRUE;
			}
			else
			if (strOmCommandArray.GetSize() >= 4)
			{
				//C-ATM Set Amt
				if (Asc2Int(strOmCommandArray[1]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[2]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[3]) > 10000)
					nErrorFlag = TRUE;
			}
			else
			if(strOmCommandArray.GetSize() >= 3)
			{
				if (Asc2Int(strOmCommandArray[1]) > 10000)
					nErrorFlag = TRUE;

				if (Asc2Int(strOmCommandArray[2]) > 10000)
					nErrorFlag = TRUE;

			}

			if (nErrorFlag)
			{
				strOmResult = OM_ERROR;
				strOmResult += "||입력매수가 카세트 최대 장착 매수보다 큼";
			}
			else
			{
				m_pDevCmn->FstCSTSetCash = 0;
				m_pDevCmn->SndCSTSetCash = 0;
				m_pDevCmn->TrdCSTSetCash = 0;
				m_pDevCmn->FthCSTSetCash = 0;

				if(m_pProfile->DEVICE.MachineType == U3100K)
				{
					m_pDevCmn->FstCSTSetCash = Asc2Int(strOmCommandArray[1]);
					m_pDevCmn->SndCSTSetCash = Asc2Int(strOmCommandArray[2]);
					m_pDevCmn->TrdCSTSetCash = Asc2Int(strOmCommandArray[3]);

					int nNoteKind1 = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKind", 0);

					if (NOTE_KIND_50000_USE == nNoteKind1)
						IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K", m_pDevCmn->FstCSTSetCash);
					else
						IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K", m_pDevCmn->FstCSTSetCash);										
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);

					//init cst2
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
					//init cst3
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K", m_pDevCmn->TrdCSTSetCash);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
					//init cst4
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
					IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

					
					strTemp.Empty();
					strTemp.Format("%8.8s%6.6s", GetDate(), GetTime());
					m_pDevCmn->fnUTL_IniSetInt(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedCstInfo", TRUE);
					m_pDevCmn->fnUTL_IniSetStr(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedDateTime", strTemp);

					TranProc(TRAN_CASH_REG); // 현금등록처리
					if (m_pDevCmn->TranResult)
					{
					
						strOmResult = OM_NORMAL;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->FstCSTSetCash,8,10,'0') ;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->SndCSTSetCash,8,10,'0') ;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->TrdCSTSetCash,8,10,'0') ;
						strOmResult += "||0||0||현금등록 완료";
					}
					else
					{
						strOmResult = OM_ERROR;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->FstCSTSetCash,8,10,'0') ;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->SndCSTSetCash,8,10,'0') ;
						strOmResult += "||";
						strOmResult += 	Int2Asc(m_pDevCmn->TrdCSTSetCash,8,10,'0') ;
						strOmResult += "||0||현금등록은 완료되었습니다, 서버등록 실패,[장치리셋요청]";
					}
						
				}			

			}

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표,저널부 장애 상태입니다";
		}

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[BRM_금액설정 %s]", strOmResult));
	}
	else
	if (strOmCommandArray[0] == "OM_마감조회")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[마감조회]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR | DEV_JPR)) 
		{
			TranProc(TRAN_SUBTOTAL);							// 마감조회
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||마감조회가 처리되었습니다";
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||마감조회처리가 실패 하였습니다";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표,저널부 장애 상태입니다";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_마감")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[마감]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR) == m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR | DEV_JPR)) 
		{
			TranProc(TRAN_TOTAL);								// 합계처리
			if (m_pDevCmn->TranResult)							// HOST OK
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||마감이 처리되었습니다";
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||마감처리가 실패 하였습니다";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표,저널부 장애 상태입니다";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_전체초기화")
	{
		m_pDevCmn->RecoveryType = 1;							// 0:자동복구,1:수동복구
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[전체초기화]"));
		m_pDevCmn->fnAPL_CloseLine();							// 호스트라인폐국
		m_pDevCmn->fnAPL_ClearError();							// 장애정보초기화
		m_pDevCmn->DeviceAutoOff = 0;							// 자동축퇴가 난 Device = Set
		m_pDevCmn->DeviceLoss = DEV_NONE;						// 분실정보초기화
		m_pDevCmn->ForceDownFlag = FALSE;						// 강제Down변수 
		m_pDevCmn->RemoteDownFlag = FALSE;						// 강제Down변수 
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);
		if (m_pDevCmn->fnAPL_ResetDevice(DEV_MAIN))
			strOmResult = OM_NORMAL;
		else
			strOmResult = OM_ERROR;
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[전체초기화 %s]", strOmResult));
		m_pDevCmn->fnSPACK_Deinitialize();
		m_pDevCmn->fnSPACK_Initialize();

		m_pDevCmn->RecoveryType = 0;							// 0:자동복구,1:수동복구
	}
	else
	if (strOmCommandArray[0] == "OM_BRM초기화")
	{
		m_pDevCmn->RecoveryType = 1;							// 0:자동복구,1:수동복구
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[BRM초기화]"));
		m_pDevCmn->fnAPL_ClearError();							// 장애정보초기화
		m_pDevCmn->DeviceAutoOff = 0;							// 자동축퇴가 난 Device = Set
		m_pDevCmn->DeviceLoss = DEV_NONE;						// 분실정보초기화
		m_pDevCmn->ForceDownFlag = FALSE;						// 강제Down변수 2006-12-19-#01
		m_pDevCmn->RemoteDownFlag = FALSE;						// 강제Down변수 2006-12-19-#01

		if (m_pDevCmn->fnAPL_ResetDevice(DEV_BRM))
			strOmResult = OM_NORMAL;
		else
			strOmResult = OM_ERROR;
		m_pDevCmn->RecoveryType = 0;							// 0:자동복구,1:수동복구
	}
	else
	if (strOmCommandArray[0] == "OM_전체보충")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[전체보충]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{

//			m_pTranCmn->Om_GetCurrentActionRemain();

			m_pDevCmn->fnCSH_RefillMoney(0xffff);				// 보충(현금매수)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// 동작대기및장애검지

		
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// 정상Segment출력
			SplitString(m_pDevCmn->fstrCSH_GetRefillResult(), ",", strTempArray);

			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	//
			{
				if(0 < Asc2Int(strTempArray[8]))				// 불명권 안내문구 개선 
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | REFILL_MOVE,
									Asc2Int(strTempArray[1]),	// 보충매수
									Asc2Int(strTempArray[3]),	// 만원보충매수
									Asc2Int(strTempArray[4]),	// 오천원보충매수
									Asc2Int(strTempArray[5]),	// 천원보충매수
									Asc2Int(strTempArray[7])	// 수표보충매수
								+	Asc2Int(strTempArray[8]),	// 불명권보충매수
									Asc2Int(strTempArray[6]),	// 오만원보충매수	
									Asc2Int(strTempArray[9]),	// 만원권   정상보충매수
									Asc2Int(strTempArray[10]));	// 오만원권 정상보충매수

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	

				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1만원
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);
				
				m_pDevCmn->SCSTCntF = 2;

			}
			else
			{													
				if(0 < Asc2Int(strTempArray[7]))				//  불명권 안내문구 개선 
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | REFILL_MOVE,
									Asc2Int(strTempArray[1]),	// 보충매수
									Asc2Int(strTempArray[3]),	// 만원보충매수
									Asc2Int(strTempArray[4]),	// 오천원보충매수
									Asc2Int(strTempArray[5]),	// 천원보충매수
									Asc2Int(strTempArray[6])	// 수표보충매수
								+	Asc2Int(strTempArray[7]));	// 불명권보충매수

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));
			}


			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{

// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애 상태입니다";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_전체회수")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[전체회수]"));

		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{

//			m_pTranCmn->Om_GetCurrentActionRemain();

			RegSetInt(_REGKEY_BRMINFO, "회수권종", 0x00);		// 0x00 : 모든 권종
			m_pDevCmn->fnCSH_TakeupMoney(0xffff);				// 회수(현금매수)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// 동작대기및장애검지
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// 정상Segment출력
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// 회수결과구하기
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	
			{
				if(0 < Asc2Int(strTempArray[8]))					// 불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[7])	// 수표회수매수
								+	Asc2Int(strTempArray[8]),	// 불명권회수매수
									Asc2Int(strTempArray[6]),	// 오만원회수매수		
									Asc2Int(strTempArray[9]),	// 만원권 정상회수매수	
									Asc2Int(strTempArray[10]));	// 오만원권 정상회수매수
				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	
				//#0042
				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1만원
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135	
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);
				
				m_pDevCmn->SCSTCntF = 2;
			}
			else
			{													
				if(0 < Asc2Int(strTempArray[7]))				// 불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[6])	// 수표회수매수
								+	Asc2Int(strTempArray[7]));	// 불명권회수매수

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));
			}

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else										
					strOmResult += "||" + strTemp + "||불가";

			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애 상태입니다";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_일부보충")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OM_일부보충]"));
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||일부보충 입력 오류입니다";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{
			if (((m_pProfile->DEVICE.MachineType == U8100))	&&
				 (strOmCommandArray.GetSize() > 2))
			{
				// 권종별 일부 보충 
				if (strOmCommandArray[2] == "구만")
					RegSetInt(_REGKEY_BRMINFO, "보충권종", BRM_NOTEKIND_10000_OLD);	// 0x30 구만원
				else if (strOmCommandArray[2] == "구천")
					RegSetInt(_REGKEY_BRMINFO, "보충권종", BRM_NOTEKIND_1000_OLD);	// 0x32 구천원
				else if (strOmCommandArray[2] == "신만")
					RegSetInt(_REGKEY_BRMINFO, "보충권종", BRM_NOTEKIND_10000_NEW);	// 0x35 신만원
				else if (strOmCommandArray[2] == "신천")
					RegSetInt(_REGKEY_BRMINFO, "보충권종", BRM_NOTEKIND_1000_NEW);	// 0x37 신천원
				else if (strOmCommandArray[2] == "오만")							
					RegSetInt(_REGKEY_BRMINFO, "보충권종", BRM_NOTEKIND_50000);		
				else
					RegSetInt(_REGKEY_BRMINFO, "보충권종", 0x00);	// 0x00 ALL
			}

//			m_pTranCmn->Om_GetCurrentActionRemain();

			nTemp = Asc2Int(strOmCommandArray[1]);
			m_pDevCmn->fnCSH_RefillMoney(nTemp);				// 보충(현금매수)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// 동작대기및장애검지
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// 정상Segment출력
			SplitString(m_pDevCmn->fstrCSH_GetRefillResult(), ",", strTempArray);
																// 보충결과구하기
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	// 
			{
				if(0 < Asc2Int(strTempArray[8]))				// 불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | REFILL_MOVE,
									Asc2Int(strTempArray[1]),	// 보충매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[7])	// 수표회수매수
								+	Asc2Int(strTempArray[8]),	// 불명권회수매수
									Asc2Int(strTempArray[6]),	// 오만원회수매수			
									Asc2Int(strTempArray[9]),	// 만원권 정상보충매수	
									Asc2Int(strTempArray[10]));	// 오만원권 정상보충매수
				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d",
									Asc2Int(strTempArray[1]),
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	

				//#0042
				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1만원
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);
				
				m_pDevCmn->SCSTCntF = 2;
			}
			else
			{												
				if(0 < Asc2Int(strTempArray[7]))				// 불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | REFILL_MOVE,
									Asc2Int(strTempArray[1]),		// 보충매수
									Asc2Int(strTempArray[3]),		// 만원회수매수
									Asc2Int(strTempArray[4]),		// 오천원회수매수
									Asc2Int(strTempArray[5]),		// 천원회수매수
									Asc2Int(strTempArray[6])		// 수표회수매수
								+	Asc2Int(strTempArray[7]));		// 불명권회수매수

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));
			}
			
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애 상태입니다";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_일부회수")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[일부회수]",
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4])), JNL_OPER);
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||일부회수 입력 오류입니다";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{
//			m_pTranCmn->Om_GetCurrentActionRemain();

			nTemp = Asc2Int(strOmCommandArray[1]);
			m_pDevCmn->fnCSH_TakeupMoney(nTemp);				// 회수(현금매수)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// 동작대기및장애검지
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// 정상Segment출력
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// 회수결과구하기
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	
			{
				if(0 < Asc2Int(strTempArray[8]))				//불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[7])	// 수표회수매수
								+	Asc2Int(strTempArray[8]),	// 불명권회수매수
									Asc2Int(strTempArray[6]),	// 오만원회수매수		
									Asc2Int(strTempArray[9]),	// 만원권 정상회수매수	
									Asc2Int(strTempArray[10]));	// 오만원권 정상회수매수

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	

				//#0042
				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1만원
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

				m_pDevCmn->SCSTCntF = 2;

			}
			else
			{
				if(0 < Asc2Int(strTempArray[7]))				// 불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,	
									Asc2Int(strTempArray[1]),	// 보충매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[6])	// 수표회수매수
								+	Asc2Int(strTempArray[7]));	// 불명권회수매수

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));
			}

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)				
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애 상태입니다";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_입출금모드선택")	
	{
		if (0 == strOmCommandArray[1].CompareNoCase("휴일"))	// 휴일모드=저속
		{
			m_pDevCmn->fnBRM_SetCarrySpeed(FALSE);
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				SetShareData("OM_입출금모드선택", "휴일");
				strOmResult = OM_NORMAL;
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||현금부 장애가 발생하였습니다";
			}
		}
		else													// 평일모드=고속
		{
			m_pDevCmn->fnBRM_SetCarrySpeed(TRUE);
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				SetShareData("OM_입출금모드선택", "평일");
				strOmResult = OM_NORMAL;
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||현금부 장애가 발생하였습니다";
			}
		}
	}
	else													
	if (strOmCommandArray[0] == "OM_자동계산설정")			
	{
		if (strOmCommandArray.GetSize() < 2)				
		{
			strOmResult = OM_ERROR;
			strOmResult += "||자동계산설정 입력 오류입니다";	
		}
		else
		if ((HBRM_SET != m_pDevCmn->AtmDefine.CashHandler)	||
			(!m_pDevCmn->ChangeBCFlag)					)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||5만원권 미지원 기종입니다";
		}
		else												
		{
			m_pProfile->GetBRMProfile();
			m_pProfile->BRM.PriorityNote = Asc2Int(strOmCommandArray[1]);
			strOmResult = OM_NORMAL;
			m_pProfile->PutBRMProfile();
		}
	}														
	else
	if (strOmCommandArray[0] == "OM_카세트권종선택")			
	{
		int nNoteKind[4] = {BRM_NOTEKIND_10000_OLD, };
		strTemp.Empty();

		for (int i = 0; i < 4; i++)
		{
			strTemp += strOmCommandArray[i + 1] + ",";
			if (0 == strOmCommandArray[i + 1].CompareNoCase("구만"))
				nNoteKind[i] = BRM_NOTEKIND_10000_OLD;			// 구만원권
			else
			if (0 == strOmCommandArray[i + 1].CompareNoCase("구천"))
				nNoteKind[i] = BRM_NOTEKIND_1000_OLD;			// 구천원권 : 2006.10.11
			else
			if (0 == strOmCommandArray[i + 1].CompareNoCase("신만"))
				nNoteKind[i] = BRM_NOTEKIND_10000_NEW;			// 신만원권 : 2006.11.03
			else
			if (0 == strOmCommandArray[i + 1].CompareNoCase("신천"))
				nNoteKind[i] = BRM_NOTEKIND_1000_NEW;			// 신천원권 : 2006.11.03
			else											
			if (0 == strOmCommandArray[i + 1].CompareNoCase("오만"))
				nNoteKind[i] = BRM_NOTEKIND_50000;
			else
			if (0 == strOmCommandArray[i + 1].CompareNoCase("입금구"))
				nNoteKind[i] = BRM_NOTEKIND_DEP_OLD;			// 입금전용(구천,구오천) : 2007.06.29
			else
			if (0 == strOmCommandArray[i + 1].CompareNoCase("입금신"))
				nNoteKind[i] = BRM_NOTEKIND_DEP_NEW;			// 입금전용(신천,신오천) : 2007.06.29
			else
				nNoteKind[i] = BRM_NOTEKIND_10000_OLD;			// 구만원권(Default)
		}
		strTemp.Empty();
		
		m_pDevCmn->fnBRM_SetNoteInfo(nNoteKind[0], nNoteKind[1], nNoteKind[2], nNoteKind[3]);
		if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
		{
			strTemp.Format("%s||%s||%s||%s", strOmCommandArray[1], 
											 strOmCommandArray[2], 
											 strOmCommandArray[3], 
											 strOmCommandArray[4]);

			if (m_pDevCmn->ChangeBCFlag)						
				strTemp += "||가능";
			else
				strTemp += "||불가";

			SetShareData("OM_카세트권종선택", strTemp);
			strOmResult = OM_NORMAL;

			if (strTemp.Find("오만") >= 0)						
			{												
				m_pProfile->GetTransProfile();
				m_pProfile->TRANS.ClerkTransOnOff |= TRAN_WITH_50TCASH;	
				m_pDevCmn->Withdraw50TFlag = TRUE;				
				m_pProfile->PutTransProfile();
			}												
			else												
			{
				m_pProfile->GetTransProfile();
				m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_WITH_50TCASH; 
				m_pDevCmn->Withdraw50TFlag = FALSE;						 
				m_pProfile->PutTransProfile();
			}													
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애가 발생하였습니다";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_출금우선권종")				
	{
		if (0 == strOmCommandArray[1].CompareNoCase("구만"))
		{
			m_pDevCmn->fnBRM_Set10000NoteUsePriority(BRM_NOTEKIND_10000_OLD);
			SetShareData("OM_출금우선권종", "구만");
		}
		else
		{
			m_pDevCmn->fnBRM_Set10000NoteUsePriority(BRM_NOTEKIND_10000_NEW);
			SetShareData("OM_출금우선권종", "신만");
		}

		strOmResult = OM_NORMAL;
	}
	else
	if (strOmCommandArray[0] == "OM_입금권종선택")				
	{
		m_pProfile->GetBRMProfile();
		if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)
		{
			m_pProfile->GetBRMProfile();

			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)
			{
				int nEnabledNoteKind = RegGetInt(_REGKEY_BRMINFO, "입금가능권종",
											 DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000);	// V06-01-01-#01

				if (strOmCommandArray[1] == "가능")					// 구만원권
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_10000_OLD, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_10000_OLD;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_10000_OLD, FALSE);
					nEnabledNoteKind &= ~ DEP_AVAIL_10000_OLD;
				}

				if (strOmCommandArray[2] == "가능")					// 구오천원권
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_5000_OLD, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_5000_OLD;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_5000_OLD, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_5000_OLD;
				}

				if (strOmCommandArray[3] == "가능")					// 구천원권
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_1000_OLD, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_1000_OLD;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_1000_OLD, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_1000_OLD;
				}

				if (strOmCommandArray[4] == "가능")					// 신만원권
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_10000_NEW, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_10000_NEW;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_10000_NEW, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_10000_NEW;
				}

				if (strOmCommandArray[5] == "가능")					// 신오천원권
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_5000_NEW, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_5000_NEW;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_5000_NEW, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_5000_NEW;
				}

				if (strOmCommandArray[6] == "가능")					// 신천원권
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_1000_NEW, TRUE);
					nEnabledNoteKind |= DEP_AVAIL_1000_NEW;
				}
				else
				{
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_1000_NEW, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_1000_NEW;
				}


				if (m_pDevCmn->ChangeBCFlag)						
				{
					if (strOmCommandArray[7] == "가능")
					{
						m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_50000, TRUE);
						nEnabledNoteKind |= DEP_AVAIL_50000;
					}
					else
					{
						m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_50000, FALSE);
						nEnabledNoteKind &= ~DEP_AVAIL_50000;
					}
				}
				else											
				{												
					m_pDevCmn->fnBRM_EnableDepositNote(BRM_NOTEKIND_50000, FALSE);
					nEnabledNoteKind &= ~DEP_AVAIL_50000;		
				}												

				m_pProfile->BRM.DepositCashKind = nEnabledNoteKind;
				//////////////////////////////////////////////////////////////////////////
				if (!(nEnabledNoteKind & (DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD)))
					m_pDevCmn->OldCashDepMode = FALSE;				// 구권(만/오천/천원권) 입금불가
				else
					m_pDevCmn->OldCashDepMode = TRUE;				// 구권(만/오천/천원권) 입금가능
				//////////////////////////////////////////////////////////////////////////

				RegSetInt(_REGKEY_BRMINFO, "입금가능권종", nEnabledNoteKind);
			
				if (HBRM_SET == m_pProfile->DEVICE.CashHandler)
				{
					strTemp.Format("%s||%s||%s||%s||%s||%s||%s", strOmCommandArray[1],
															 strOmCommandArray[2],
															 strOmCommandArray[3],
															 strOmCommandArray[4],
															 strOmCommandArray[5],
															 strOmCommandArray[6],
															 strOmCommandArray[7]);

					if (m_pDevCmn->ChangeBCFlag)				
						strTemp += "||가능";
					else
						strTemp += "||불가";
				}
				else											
					strTemp.Format("%s||%s||%s||%s||%s||%s", strOmCommandArray[1],
															 strOmCommandArray[2],
															 strOmCommandArray[3],
															 strOmCommandArray[4],
															 strOmCommandArray[5],
															 strOmCommandArray[6]);


			}
			else
			{
				// 0:오천원권/천원권(가능,불가)
				if (strOmCommandArray[1] == "가능")						// V200
				{
					m_pProfile->BRM.DepositCashKind = 0;

					RegSetInt(_REGKEY_BRMINFO, "만원권만입금가능",	0);	
					IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2만원권만입금가능매수", BRM_MAXRJB2DEPCNT);
				}
				else
				{
					m_pProfile->BRM.DepositCashKind = 1;

					RegSetInt(_REGKEY_BRMINFO, "만원권만입금가능",	1);
					IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2만원권만입금가능매수", 0);
				}
			}
		}
		
		m_pProfile->PutBRMProfile();
		strOmResult = OM_NORMAL;
	}
	else
	if (strOmCommandArray[0] == "OM_신만원권일부회수")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[신만원일부회수]"));
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||일부회수 입력 오류입니다";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{
			RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_10000_NEW);			// 0x35 신권만원

//			m_pTranCmn->Om_GetCurrentActionRemain();

			nTemp = Asc2Int(strOmCommandArray[1]);
			m_pDevCmn->fnBRM_TakeupMoney(nTemp);				// 회수(현금매수)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// 동작대기및장애검지
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// 정상Segment출력
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// 회수결과구하기
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	
			{
				if(0 < Asc2Int(strTempArray[8]))				//불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[7])	// 수표회수매수
								+	Asc2Int(strTempArray[8]),	// 불명권회수매수
									Asc2Int(strTempArray[6]),	// 오만원회수매수		
									Asc2Int(strTempArray[9]),	// 만원권 정상회수매수	
									Asc2Int(strTempArray[10]));	// 오만원권 정상회수매수

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),		
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	
				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1만원
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);
				m_pDevCmn->SCSTCntF = 2;
			}
			else
			{													
				if(0 < Asc2Int(strTempArray[7]))				// 불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[6])	// 수표회수매수
								+	Asc2Int(strTempArray[7]));	// 불명권회수매수

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));

			}
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{

// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애 상태입니다";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_신천원권일부회수")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[신천원일부회수]",
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4])), JNL_OPER);
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||일부회수 입력 오류입니다";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{

			RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_1000_NEW);			// 0x37 신권천원

			nTemp = Asc2Int(strOmCommandArray[1]);
			m_pDevCmn->fnBRM_TakeupMoney(nTemp);				// 회수(현금매수)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// 동작대기및장애검지
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// 정상Segment출력
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// 회수결과구하기
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	// 
			{
				if(0 < Asc2Int(strTempArray[8]))				// 불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[7])	// 수표회수매수
								+	Asc2Int(strTempArray[8]),	// 불명권회수매수
									Asc2Int(strTempArray[6]),	// 오만원회수매수	
									Asc2Int(strTempArray[9]),	// 만원권 정상회수매수	
									Asc2Int(strTempArray[10]));	// 오만원권 정상회수매수

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));
			}
			else
			{												
				if(0 < Asc2Int(strTempArray[7]))				//불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),		// 회수매수
									Asc2Int(strTempArray[3]),		// 만원회수매수
									Asc2Int(strTempArray[4]),		// 오천원회수매수
									Asc2Int(strTempArray[5]),		// 천원회수매수
									Asc2Int(strTempArray[6])		// 수표회수매수
								+	Asc2Int(strTempArray[7]));		// 불명권회수매수

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),	
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6])
												+	 Asc2Int(strTempArray[7]));

			}
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)				
					strOmResult += "||" + strTemp + "||가능";
				else										
					strOmResult += "||" + strTemp + "||불가";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애 상태입니다";
		}

	}
	else														
	if (strOmCommandArray[0] == "OM_오만원권일부회수")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[오만원일부회수]"));
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||일부회수 입력 오류입니다";
		}
		else
		if ((HBRM_SET != m_pDevCmn->AtmDefine.CashHandler)	||	
			(!m_pDevCmn->ChangeBCFlag)					)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||5만원권 미지원 기종입니다";
		}
		else												
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{

			RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_50000);			// 0x34 오만원
			
//			m_pTranCmn->Om_GetCurrentActionRemain();

			nTemp = Asc2Int(strOmCommandArray[1]);
			m_pDevCmn->fnBRM_TakeupMoney(nTemp);				// 회수(현금매수)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// 동작대기및장애j검지
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// 정상Segment출력
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// 회수결과구하기

			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)
			{
				if(0 < Asc2Int(strTempArray[8]))				//불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[7])	// 수표회수매수
								+	Asc2Int(strTempArray[8]),	// 불명권회수매수
									Asc2Int(strTempArray[6]),	// 오만원회수매수		
									Asc2Int(strTempArray[9]),	// 만원권 정상회수매수	
									Asc2Int(strTempArray[10]));	// 오만원권 정상회수매수

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));

				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1만원
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

				m_pDevCmn->SCSTCntF = 2;
			}

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend();
// 				m_pTranCmn->Om_SetActionGapRemainAsycSend(TRUE);

				strOmResult = OM_NORMAL;
				strOmResult += "||" + strTemp + "||가능";	
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||" + strTemp + "||가능";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애 상태입니다";
		}
	}			
	else
	if ((strOmCommandArray[0] == "OM_신만원권전체회수") &&
		 (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))			// 신천구구업무지원 2006.10.26
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[신권 만원 전체회수]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{
			RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_10000_NEW);			// 0x35 신권만원

			m_pDevCmn->fnBRM_TakeupMoney(0xffff);				// 회수(현금매수)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// 동작대기및장애검지
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// 정상Segment출력
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// 회수결과구하기
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	//
			{
				if(0 < Asc2Int(strTempArray[8]))				//불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[7])	// 수표회수매수
								+	Asc2Int(strTempArray[8]),	// 불명권회수매수
									Asc2Int(strTempArray[6]),	// 오만원회수매수		
									Asc2Int(strTempArray[9]),	// 만원권 정상회수매수	
									Asc2Int(strTempArray[10]));	// 오만원권 정상회수매수

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	



				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1만원
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

			}
			else
			{													
				if(0 < Asc2Int(strTempArray[7]))				// 불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[6])	// 수표회수매수
								+	Asc2Int(strTempArray[7]));	// 불명권회수매수

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6]) + Asc2Int(strTempArray[7]));

			}
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)				
					strOmResult += "||" + strTemp + "||가능";
				else										
					strOmResult += "||" + strTemp + "||불가";
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애 상태입니다";
		}

		RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_10000_OLD);			// 0x30 구권만원
	}
	else
	if ((strOmCommandArray[0] == "OM_신천원권전체회수") &&
		 (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))			// 신천구구업무지원 2006.10.26
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[신천원권전체회수]"));
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{

			RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_1000_NEW);			// 0x37 신권천원

			m_pDevCmn->fnBRM_TakeupMoney(0xffff);				// 회수(현금매수)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// 동작대기및장애검지
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// 정상Segment출력
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// 회수결과구하기
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)	
			{
				if(0 < Asc2Int(strTempArray[8]))				//불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[7])	// 수표회수매수
								+	Asc2Int(strTempArray[8]),	// 불명권회수매수
									Asc2Int(strTempArray[6]),	// 오만원회수매수		
									Asc2Int(strTempArray[9]),	// 만원권 정상회수매수	
									Asc2Int(strTempArray[10]));	// 오만원권 정상회수매수

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	
			}
			else
			{													
				if(0 < Asc2Int(strTempArray[7]))				// 불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[6])	// 수표회수매수
								+	Asc2Int(strTempArray[7]));	// 불명권회수매수

				strTemp.Format("%d||%d||%d||%d||%d", Asc2Int(strTempArray[1]),
													 Asc2Int(strTempArray[3]),
													 Asc2Int(strTempArray[4]),
													 Asc2Int(strTempArray[5]),
													 Asc2Int(strTempArray[6]) + Asc2Int(strTempArray[7]));
			}
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)				
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";
				else											
					strOmResult += "||" + strTemp + "||불가";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애 상태입니다";
		}
		RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_10000_OLD);			// 0x30 구권만원
	}
	else														
	if ((strOmCommandArray[0] == "OM_오만원권전체회수") &&
		 (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))			// 신천구구업무지원 2006.10.26
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[오만원 전체회수]"));
		if ((HBRM_SET != m_pDevCmn->AtmDefine.CashHandler)	||
			(!m_pDevCmn->ChangeBCFlag)					)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||5만원권 미지원 기종입니다";
		}
		else													
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{


			RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_50000);			// 0x37 신권천원

			m_pDevCmn->fnBRM_TakeupMoney(0xffff);				// 회수(현금매수)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// 동작대기및장애검지
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// 정상Segment출력
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// 회수결과구하기
			if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)
			{
				if(0 < Asc2Int(strTempArray[8]))				// 불명권 안내문구 개선 : COM07-17
				{
					m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
				}
				m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
									Asc2Int(strTempArray[1]),	// 회수매수
									Asc2Int(strTempArray[3]),	// 만원회수매수
									Asc2Int(strTempArray[4]),	// 오천원회수매수
									Asc2Int(strTempArray[5]),	// 천원회수매수
									Asc2Int(strTempArray[7])	// 수표회수매수
								+	Asc2Int(strTempArray[8]),	// 불명권회수매수
									Asc2Int(strTempArray[6]),	// 오만원회수매수		
									Asc2Int(strTempArray[9]),	// 만원권 정상회수매수	
									Asc2Int(strTempArray[10]));	// 오만원권 정상회수매수

				strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
									Asc2Int(strTempArray[1]),	
									Asc2Int(strTempArray[3]),
									Asc2Int(strTempArray[4]),
									Asc2Int(strTempArray[5]),
									Asc2Int(strTempArray[7])
								+	Asc2Int(strTempArray[8]),
									Asc2Int(strTempArray[6]),
									Asc2Int(strTempArray[9]),	
									Asc2Int(strTempArray[10]));	


				m_pDevCmn->FstCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원;
//				m_pDevCmn->FstCSTSetCash += Asc2Int(strTempArray[10]);

				m_pDevCmn->SndCSTSetCash =  RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0); //1만원
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
				m_pDevCmn->SndCSTSetCash += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);
//				m_pDevCmn->SndCSTSetCash += Asc2Int(strTempArray[9]);

				////#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K",   m_pDevCmn->FstCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW1K", 0);
				//init cst2
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", m_pDevCmn->SndCSTSetCash);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW1K", 0);
				//init cst3
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW1K", 0);
				//init cst4
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "50K",   0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K",0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW5K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW1K", 0);

			}

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||" + strTemp + "||가능";	
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||" + strTemp + "||가능";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애 상태입니다";
		}

	}															
	else
	if ((strOmCommandArray[0] == "OM_신권입금함전체회수") &&	// 입금전용 카세트 회수지원
		 (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))	
	{

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[신권입금함전체회수]"), JNL_OPER);
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_BRM))
		{
			RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_DEP_NEW);

			m_pDevCmn->fnBRM_TakeupMoney(0xffff);				// 회수(현금매수)
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM);		// 동작대기및장애검지
			m_pDevCmn->fnAPL_DisplayNormalSegment();			// 정상Segment출력
			SplitString(m_pDevCmn->fstrCSH_GetTakeupResult(), ",", strTempArray);
																// 회수결과구하기

			if(0 < Asc2Int(strTempArray[8]))					// 불명권 안내문구 개선
			{
				m_pDevCmn->fnSND_KeySound(_SOUND_OM_REJECT);
			}

			m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_MANUALLOAD | TAKEUP_MOVE,
								Asc2Int(strTempArray[1]),		// 회수매수
								Asc2Int(strTempArray[3]),		// 만원회수매수
								Asc2Int(strTempArray[4]),		// 오천원회수매수
								Asc2Int(strTempArray[5]),		// 천원회수매수
								Asc2Int(strTempArray[7])		// 수표회수매수
							+	Asc2Int(strTempArray[8]),		// 불명권회수매수
								Asc2Int(strTempArray[6]),		// 오만원회수매수		
								Asc2Int(strTempArray[9]),		// 만원권 정상회수매수	
								Asc2Int(strTempArray[10]));		// 오만원권 정상회수매수

			strTemp.Format("%d||%d||%d||%d||%d||%d||%d||%d", 
								Asc2Int(strTempArray[1]),		
								Asc2Int(strTempArray[3]),
								Asc2Int(strTempArray[4]),
								Asc2Int(strTempArray[5]),
								Asc2Int(strTempArray[7])
							+	Asc2Int(strTempArray[8]),
								Asc2Int(strTempArray[6]),
								Asc2Int(strTempArray[9]),		
								Asc2Int(strTempArray[10]));		
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_BRM))
			{
				strOmResult = OM_NORMAL;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
			else
			{
				strOmResult = OM_ERROR;
				if (m_pDevCmn->ChangeBCFlag)					
					strOmResult += "||" + strTemp + "||가능";	
				else											
					strOmResult += "||" + strTemp + "||불가";	
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||현금부 장애 상태입니다";
		}

	}
	else
	if (strOmCommandArray[0] == "OM_명세표인자")
	{
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표인자 입력 오류입니다";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		{

			m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_SPR);
			for (i = 2; i <= strOmCommandArray.GetSize();  i++)
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, i - 2, 0, strOmCommandArray[i - 1]);
			m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR, JNL_OPER);
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
			{
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);
				if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
					strOmResult = OM_NORMAL;
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||명세표인자중 장애가 발생하였습니다";
				}
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||명세표인자중 장애가 발생하였습니다";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표부 장애 상태입니다";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_저널인자")
	{
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||저널인자 입력 오류입니다";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_JPR))
		{

			m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_JPR);
			for (i = 2; i <= strOmCommandArray.GetSize();  i++)
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, i - 2, 0, strOmCommandArray[i - 1]);
			m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_JPR, JNL_OPER);
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_JPR))
				strOmResult = OM_NORMAL;
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||저널인자중 장애가 발생하였습니다";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||저널부 장애 상태입니다";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_명세표저널인자")
	{
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표저널인자 입력 오류입니다";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR | DEV_JPR))
		{

			m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_SPR | DEV_JPR);
			for (i = 2; i <= strOmCommandArray.GetSize();  i++)
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR | DEV_JPR, i - 2, 0, strOmCommandArray[i - 1]);
			m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR | DEV_JPR, JNL_OPER);
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR | DEV_JPR))
			{
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);
				if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
					strOmResult = OM_NORMAL;
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||명세표,저널인자중 장애가 발생하였습니다";
				}
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||명세표,저널인자중 장애가 발생하였습니다";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표,저널부 장애 상태입니다";
		}
	}
	else
	if (strOmCommandArray[0] == "OM_단체테스트")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[단체테스트]"));
		m_pDevCmn->fnLDCtrl_DisableWatchdogRpc();				
		ReadyDevTest();											// 단체테스트준비
		m_pDevCmn->fnAPL_ClerkInformation();					// 계원정보
		SetShareData("OM_RESULT", OM_NORMAL);					// 응답처리
		return FALSE;											// 종료처리
	}
	else
	if (strOmCommandArray[0] == "OM_기기설정")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[기기설정]"));
		if (strOmCommandArray.GetSize() < 3)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||기기설정 입력 오류입니다";
		}
		else
		{
			m_pProfile->GetDeviceProfile();

			// 0:기기종류(T2ATM,T1ATM,SATM,RATM,CDP,CD)
			m_pProfile->DEVICE.MachineType = U8500;

			m_pProfile->InitDeviceProfile();

			// 1:명세표부종류(T1SHU,NTSHU,HSHU,경덕SHU1,경덕SHU2)
			if (!strOmCommandArray[2].CompareNoCase("T1SHU"))
			{
				m_pProfile->DEVICE.SHUHandler = SHU_MECHA_A2;
				m_pProfile->DEVICE.CameraDevice = USB_CAMERA_SET;
			}
			else
			if (!strOmCommandArray[2].CompareNoCase("NTSHU"))
			{
				m_pProfile->DEVICE.SHUHandler = SHU_MECHA_NT;
				m_pProfile->DEVICE.CameraDevice = USB_CAMERA_SET;
				RegSetInt(_REGKEY_DEVINFO, "ShuType", 3);
			}
			m_pProfile->PutDeviceProfile();
			strOmResult = OM_NORMAL;
		}
	}
	else
	if (strOmCommandArray[0] == "OM_거래일련번호변경")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래일련번호변경]"));
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||일련번호변경 오류";
		}
		else
		{

			m_pProfile->GetTransProfile();
			// 0:시작일련번호(NNNN)
			strTemp = SPACE6 + strOmCommandArray[1];
			strTemp = strTemp.Right(6);
			memcpy(m_pProfile->TRANS.StartSerialNo, strTemp.GetBuffer(0), 6);
			m_pProfile->TRANS.StartSerialNo[6] = NULL;
			m_pProfile->PutTransProfile();
			m_pDevCmn->fnAPL_SetSerialNo();

			strOmResult = OM_NORMAL;
			strOmResult += "||거래일련번호변경 완료";
			m_pDevCmn->fnAPL_ClerkInformation();					// 계원정보
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[거래일련번호변경] %s", strOmResult));				

	}
	else
	if (strOmCommandArray[0] == "OM_광고동영상")
	{

		HWND hAviWnd;
		if (0 == strOmCommandArray[1].CompareNoCase("ON"))
		{
			SetShareData("OM_광고동영상", "ON");
//			hAviWnd = ::FindWindow(NULL, "AviPlayer");
			hAviWnd = _FIND_CFMON;
			if (hAviWnd)
				::PostMessage(hAviWnd, WUM_SOUND_ON, 0, 0);
		}
		else
		{
			SetShareData("OM_광고동영상", "OFF");
//			hAviWnd = ::FindWindow(NULL, "AviPlayer");
			hAviWnd = _FIND_CFMON;
			if (hAviWnd)
				::PostMessage(hAviWnd, WUM_SOUND_OFF, 0, 0);
		}

		strOmResult = OM_NORMAL;
	}
	else
	if (strOmCommandArray[0] == "OM_종료")
	{

		if ((m_pProfile->DEVICE.MachineType == U3100K)	||
			(m_pProfile->DEVICE.MachineType == U8100)	)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[종료]"));
		}
		m_pDevCmn->fnAPL_ClerkInformation();					// 계원정보
		SetShareData("OM_RESULT", OM_NORMAL);					// 응답처리
		return FALSE;											// 종료처리
	}
	else
	if (strOmCommandArray[0] == "OM_카메라테스트")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[카메라테스트]"));
		if(m_pProfile->DEVICE.MachineType == U3100K || m_pProfile->DEVICE.MachineType == U8100) 
		{
			if (strOmCommandArray.GetSize() < 5)
			{
				strOmResult = OM_ERROR;
				strOmResult += "||카메라테스트 입력 오류입니다";
			}
			else
			{
				DeleteFile(strOmCommandArray[1]+".Jpg");
				DeleteFile(strOmCommandArray[2]+".Jpg");
				DeleteFile(strOmCommandArray[3]+".Jpg");
				DeleteFile(strOmCommandArray[4]+".Jpg");


				CFileFind *m_file;
				ULONG nWaitTime;

				if (m_pDevCmn->fnCMR_GetFaceCameraDeviceStatus() == NORMAL)
				{
					nErrorFlag = m_pDevCmn->fnCMR_CaptureFace(strOmCommandArray[1]);
					m_file = new CFileFind;
					nWaitTime = TimerSet(5);
					while (!CheckTimer(nWaitTime))
					{
					
						Delay_Msg(400);
						if(m_file->FindFile(strOmCommandArray[1]+".Jpg"))
						{
							strOmResult = OM_NORMAL;
							strOmResult += "||" + strOmCommandArray[1]+".Jpg";
							break;
						}
						else
						{
							strOmResult = OM_NORMAL;
							strOmResult += "||NG";
						}

					}

					if (m_file!=NULL)
					{
						delete m_file;	
						m_file = NULL;
					}

				}
				else
				{
						strOmResult = OM_NORMAL;
						strOmResult += "||NG";
				}

				if (m_pDevCmn->fnCMR_GetHandCameraDeviceStatus() == NORMAL)
				{
					nErrorFlag = m_pDevCmn->fnCMR_CaptureHand(strOmCommandArray[2]);
					m_file = new CFileFind;
					nWaitTime = TimerSet(5);
					while (!CheckTimer(nWaitTime))
					{
						Delay_Msg(400);
						if(m_file->FindFile(strOmCommandArray[2]+".Jpg"))
						{
							strOmResult += "||" + strOmCommandArray[2]+".Jpg";
							break;
						}
						else
						{
							strOmResult += "||NG";
						}
					}

					if (m_file!=NULL)
					{
						delete m_file;	
						m_file = NULL;
					}

				}
				else
				{
						strOmResult += "||NG";
				}

				strOmResult += "||NG";
				strOmResult += "||NG";
					
			}
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[카메라테스트] %s", strOmResult));				
		m_pDevCmn->fnAPL_ClerkInformation();					// 계원정보
		SetShareData("OM_RESULT", strOmResult);					// 응답처리
		return FALSE;											// 종료처리
	}
	else
	if (strOmCommandArray[0] == "OM_POWEROFFSTATUS")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[POWEROFFSTATUS]"));
		{

			m_pDevCmn->AtmStatus = ATM_DOWN;
			strOmResult = OM_NORMAL;
			strOmResult += "||기기 종료 중 입니다!";
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[POWEROFFSTATUS] %s", strOmResult));
		m_pDevCmn->fnAPL_ClerkInformation();					// 계원정보
		SetShareData("OM_RESULT", strOmResult);					// 응답처리
		return FALSE;											// 종료처리
	}
	else
	if (strOmCommandArray[0] == "OM_CLOSESHUTTER")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[CLOSESHUTTER]"));
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
		if (m_pDevCmn->fnCSH_GetDeviceStatus() != NODEVICE)		// 2005.03.30
			m_pDevCmn->fnCSH_CloseShutter();					// Close Shutter
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
		
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
		{
			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";								// Null Message
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||[SHUTTER 장애]";								// Null Message
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[CLOSESHUTTER] %s", strOmResult));
		m_pDevCmn->fnAPL_ClerkInformation();					// 계원정보
		SetShareData("OM_RESULT", strOmResult);					// 응답처리
		return FALSE;											// 종료처리
	}
	else
	if (strOmCommandArray[0] == "OM_OPENSHUTTER")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OPENSHUTTER]"));
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
		if (m_pDevCmn->fnCSH_GetDeviceStatus() != NODEVICE)		// 2005.03.30
			m_pDevCmn->fnCSH_OpenShutter();					// Close Shutter
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
		
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
		{
			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";								// Null Message
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||[SHUTTER 장애]";								// Null Message
		}

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OPENSHUTTER] %s", strOmResult));

		m_pDevCmn->fnAPL_ClerkInformation();					// 계원정보
		SetShareData("OM_RESULT", strOmResult);					// 응답처리
		return FALSE;											// 종료처리
	}
	else
	if (strOmCommandArray[0] == "OM_LOCKOPEN")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LOCKOPEN]"));

		if (m_pDevCmn->fnLDCtrl_GetDeviceStatus() == NORMAL)		// 2005.03.30
		{
			int nFlag = FALSE;
			
			if((strOmCommandArray[1].GetLength() != 8) || 
			   (!IsNum(strOmCommandArray[1].GetBuffer(0), strOmCommandArray[1].GetLength())) || 
			   (IsZero(strOmCommandArray[1].GetBuffer(0), strOmCommandArray[1].GetLength())))
			{
				strOmResult = OM_ERROR;
				strOmResult += "||[키 입력 오류 !!]";								// Null Message				
			}
			else
			{
				if(m_pDevCmn->fnLDCtrl_GetLoadKeyCheckFlag(strOmCommandArray[1]) == LOCK_STAT_CHECK_OK)
				{
					if(m_pDevCmn->fnLDCtrl_OpenRpcLock("", 2) == T_ERROR)
					{
						strOmResult = OM_ERROR;
						strOmResult += "||[RPC LOCK 장애(연결실패)]";								// Null Message				
					}
					else
					{
						strOmResult = OM_NORMAL;
						strOmResult += "||[조작 성공] ☞ 5초 후 자동 잠김 ☜";								// Null Message
					}			
				}
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||[입력 키 불 일치!!]";								// Null Message				
				}
			}

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||[RPC LOCK 장애]";								// Null Message
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[LOCKOPEN] %s", strOmResult));
		m_pDevCmn->fnAPL_ClerkInformation();					// 계원정보
		SetShareData("OM_RESULT", strOmResult);					// 응답처리
		return FALSE;											// 종료처리
	}
	else
	if (strOmCommandArray[0] == "OM_FNGREAD")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[지문인식테스트]"));
		int nRet = FALSE;
		if (m_pDevCmn->fnAPL_GetDefineDevice(DEV_FNG))
		{
			if(m_pDevCmn->fnFNG_GetDeviceStatus(1) == NORMAL)
			{
				m_pDevCmn->fnFNG_Initialize();							// 연결시작및초기화
				m_pDevCmn->fnFNG_Acquire();								// 지문읽기
				nRet = m_pDevCmn->fnCMN_ScanDeviceAction(DEV_FNG | DEV_SCR, K_15_WAIT, EVENT_IN);
				if (nRet == R_TIMEOUT)
				{
					m_pDevCmn->fnFNG_CancelAcquire();					// 지문읽기취소
					strOmResult = OM_ERROR;
					strOmResult += "||NG||[조작(지문인식) 시간 초과]";								// Null Message
				}
				else
				if (nRet == DEV_FNG)
				{
					Delay_Msg(1000);									// 지문확인대기								
					strOmResult = OM_NORMAL;
					strOmResult += "||FNGDATA="+m_pDevCmn->fstrFNG_GetData()+"||[조작 성공]";								// Null Message
					m_pDevCmn->fnFNG_CancelAcquire();					// 지문읽기취소
				}
				else
				{
					m_pDevCmn->fnFNG_CancelAcquire();					// 지문읽기취소
					strOmResult = OM_ERROR;
					strOmResult += "||NG||[조작(지문인식) 시간 초과]";								// Null Message
				}
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||NG||[지문인식 연결실패]";								// Null Message				
			}
			

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||NG||[지문인식 미사용]";								// Null Message
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[지문인식테스트 %s]", strOmResult));
		m_pDevCmn->fnAPL_ClerkInformation();					// 계원정보
		SetShareData("OM_RESULT", strOmResult);					// 응답처리
		return FALSE;											// 종료처리
	}
	else
	if (strOmCommandArray[0] == "OM_PBMPRINT")  //#N0211
	{
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류입니다";
		}
		else
		{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[통장인자 테스트]"));


		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		{
			int	nReturn = 0;
				m_pTranCmn->PPPRecvData.y = Asc2Int(strOmCommandArray[1]);
			nReturn = m_pTranCmn->fnAPD_PBMPrintTest();
			if (nReturn == T_OK)
			{
				if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))
				{
					strOmResult = OM_NORMAL;
					strOmResult += "||통장테스트가 완료 되었습니다";
				}
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||통장테스트중 장애가 발생하였습니다";
				}
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||통장테스트중 장애가 발생하였습니다";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||통장부 장애 상태입니다";
			}
		}
	}
	else
	if (strOmCommandArray[0] == "OM_출하")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[출하버튼적용]"));
		char szTmp[1024];
		sprintf(szTmp, "00%6.6s", GetTime().GetBuffer(0));
		if(m_pDevCmn->fnAPL_GetDefineDevice(DEV_RDCTRL) && (m_pProfile->DEVICE.MachineType == U3100K))
		{
			m_pDevCmn->fnLDCtrl_SetKeytableFile(0, szTmp);
			m_pDevCmn->fnLDCtrl_SetKeytableFile(0, szTmp);
			m_pDevCmn->fnLDCtrl_SetKeytableFile(0, szTmp);
			m_pDevCmn->fnLDCtrl_SetKeytableFile(0, szTmp);
			m_pDevCmn->fnLDCtrl_SetKeytableFile(0, szTmp);			
			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";								// Null Message
		}
		else
		{
			//#0063
			if (m_pProfile->DEVICE.MachineType == U8100)
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||[조작 성공]";								// Null Message
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||RPC LOCK 장애(연결실패)";								// Null Message
			}
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[출하버튼적용 %s]", strOmResult));
		m_pDevCmn->fnAPL_ClerkInformation();					// 계원정보
		SetShareData("OM_RESULT", strOmResult);					// 응답처리
		return FALSE;											// 종료처리
	}
	else 
	if (strOmCommandArray[0] == "OM_SPACKSETUP") //OM_SPACKSETUP||충격장치설정(use  =1)||외부센서(0, 1 = #1, 1 = #2)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[충격장치설정 %s %s]", strOmCommandArray[1], strOmCommandArray[2]));
		if (strOmCommandArray.GetSize() >= 2)
		{

			if(Asc2Int(strOmCommandArray[1]) == 1)
			{
				nErrorFlag = FALSE;
				if(Asc2Int(strOmCommandArray[2]) == 0)
				{
					if(m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_OFF) == TRUE)
						nErrorFlag = FALSE;
					else
						nErrorFlag = TRUE;
				}
				else
				if(Asc2Int(strOmCommandArray[2]) == 1)
				{
					if(m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_ON, CLKEY_OFF) == TRUE)
						nErrorFlag = FALSE;
					else
						nErrorFlag = TRUE;
				}
				else
				if(Asc2Int(strOmCommandArray[2]) == 2)
				{
					if(m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_ON) == TRUE)
						nErrorFlag = FALSE;
					else
						nErrorFlag = TRUE;
				}
				else
				{
					if(m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_OFF) == TRUE)
						nErrorFlag = FALSE;
					else
						nErrorFlag = TRUE;
				}

				if(nErrorFlag)
				{
						strOmResult = OM_ERROR;
						strOmResult += "||[충격장치 장애]";											
				}
				else
				{
					
					Delay_Msg(5000);
					int nGetExternalS1EnSt = m_pDevCmn->fnSPACK_GetExternalSensorEnableStatus_1();
					int nGetExternalS2EnSt = m_pDevCmn->fnSPACK_GetExternalSensorEnableStatus_2();

					if(nGetExternalS1EnSt)
						IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 1);

					if(nGetExternalS2EnSt)
						IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 2);

					if(nGetExternalS1EnSt && nGetExternalS2EnSt)
						IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 3);

					if(!nGetExternalS1EnSt && !nGetExternalS2EnSt)
						IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 0);

					strOmResult = OM_NORMAL;
					strOmResult += "||[조작 성공]";							
				}
			}
			else
			{
				strOmResult = OM_NORMAL;
				strOmResult += "||[충격장치 미사용]";						// Null Message
			}
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[충격장치설정 %s]", strOmResult));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류입니다";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);

		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[모뎀정보설정 %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_모뎀정보설정") //OM_모뎀정보설정|회선종류|폰번호|무선모뎀포트|ADSLID|ADSL비밀번호
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[모뎀정보설정 %s %s %s %s %s]", strOmCommandArray[1], strOmCommandArray[2], strOmCommandArray[3], strOmCommandArray[4], strOmCommandArray[5]));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		if (strOmCommandArray.GetSize() >= 7)
		{
			//회선종류
			IniSetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice9", strOmCommandArray[1]);
			//폰번호
			IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "PHONENUMBER",strOmCommandArray[2]);
			//폰포트
			IniSetInt("D:\\INI\\MOBILECFG.ini","MOBILE", "COM", Asc2Int(strOmCommandArray[3]));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);
			int nModemType = Asc2Int(strOmCommandArray[1]);
			if(nModemType == MODEM_LGT)
			{
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "PPP", "777");
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "CMSSERVER", "0190001259");
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "ID", strOmCommandArray[2]+"@lgt.co.kr");
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "PASS", 	strOmCommandArray[2].Right(4));
			}
			else
			if(nModemType == MODEM_KTF)
			{
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "PPP", "*98#");
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "ID",  "WCDMA");
				IniSetStr("D:\\INI\\MOBILECFG.ini", "MOBILE", "PASS","");
			}
			else
			if(nModemType == MODEM_KT_ADSL)
			{
				CString strTmp = "";
				strTmp = strOmCommandArray[4];
				strTmp.MakeLower();
				//ADSL ID
				IniSetStr("D:\\INI\\ATM_CFG.ini", "ATMINFO", "ATM_MOVE_ID",  strTmp);
				strTmp = "";
				strTmp = strOmCommandArray[5];
				strTmp.MakeLower();
				//ADSL PWD
				IniSetStr("D:\\INI\\ATM_CFG.ini", "ATMINFO", "ATM_MOVE_PWD", strTmp);
			}
			else
			if(nModemType == MODEM_LGT_UPlus || nModemType == MODEM_LGT_U_GPS) //#0137
			{
				AtmSetEnableDHCP(FALSE);
			}
			else
			{
				AtmSetEnableDHCP(TRUE);
			}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);

			//기기고유번호(10 자리)
			strTemp = "";
			strTemp = strOmCommandArray[6] + SPACE16;
			strTemp = strTemp.Left(16);
			memcpy(m_pProfile->NETWORK.MachineMakeNum, strTemp.GetBuffer(0), strTemp.GetLength()); 
			m_pProfile->PutNetworkProfile();
			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";								// Null Message
		}
		else
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult4[%s]", strOmCommandArray[0]);

			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류입니다";
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[모뎀정보설정 %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_브랜드설정") //OM_브랜드설정|브랜드설정
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[브랜드설정1 %s]", strOmCommandArray[1]));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[브랜드설정2 %s]", strOmCommandArray[2]));

		if(m_pDevCmn->AtmDefine.MachineType == U3100K && 
			strOmCommandArray.GetSize() >= 2 && 
			(Asc2Int(strOmCommandArray[2]) != KIUP && Asc2Int(strOmCommandArray[2]) != SHINHAN))
		{
			strOmResult = OM_ERROR;
			strOmResult += "||지원하지 않는 브랜드 입니다.";
		}
		else
		if (strOmCommandArray.GetSize() >= 2)
		{
//			IniSetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice11", strOmCommandArray[1]);
			IniSetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice20", strOmCommandArray[2]);
			IniSetInt(_TRANS_INI, TRANS_SEC, "BrandJehyu", 0);

			int nApKind = IniGetInt(_DEVICE_INI,DEV_SEC, "ETCDevice20", 0);
			
			if(nApKind == KIUP)   //#N0190  기업 브랜드 설정시 최대 방출금액 셋팅
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//타행 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//자행 100

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//장애인ATM 지원
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if((nApKind == NONGHYUP) || (nApKind == NONGHYUPDAN))   //#N0190  농협 브랜드 설정시 최대 방출금액 셋팅
			{	// 계원에서 NH농협 설정시 브랜드 34로 값이 오지만 이미지 변경 외 거래  Flow 는 같아 브랜드 14로 값 다시 셋팅함.
				// 이미지 변경은 "BrandJehyu" 값으로 변경함.
				IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice20", NONGHYUP);
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//타행 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 70);					//자행 70

				//#N0284
				if(nApKind == NONGHYUPDAN)
				{
					IniSetInt(_TRANS_INI, TRANS_SEC, "BrandJehyu", NONGHYUPDAN);					
				}
				else
				{
					IniSetInt(_TRANS_INI, TRANS_SEC, "BrandJehyu", 0);					
				}
			}
			else
			if(nApKind == KEB)       //#N0190  외환 브랜드 설정시 최대 방출금액 셋팅
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//타행 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//자행 100	#N0276

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//장애인ATM 지원
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == KWANGJU)   //#N0181  광주 브랜드 설정시 최대 방출금액 셋팅
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//타행 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//자행 100

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//장애인ATM 지원
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == KYONGNAM)   //#N0192  경남 브랜드 설정시 최대 방출금액 셋팅
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//타행 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 50);					//자행 50

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  장애인ATM 지원
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == HANA)   //#N0214  하나 브랜드 설정시 최대 방출금액 셋팅
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//타행 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//자행 100

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  장애인ATM 지원
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == SHINHAN)   //#N0215  신한 브랜드 설정시 최대 방출금액 셋팅
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//타행 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//자행 100

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  장애인ATM 지원
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == SC)	 //#N0219  SC 제일은행
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//타행 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//자행 100  #N0241

				//제일은행 장애인 atm지원 => 브랜드 변경후 장애인 설정하면 사용 가능 했으나 자동 off 기능 제거
			}
			else
			if(nApKind == KB)	 //#N0226  국민은행
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//타행 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 70);					//자행 70

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  장애인ATM 지원
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == CITI)   //#N0238  씨티 브랜드 설정시 최대 방출금액 셋팅
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//타행 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 70);					//자행 70

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  장애인ATM 지원
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			else
			if(nApKind == NICE)   //#N0219  전자금융
			{
				IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//타행 30
				IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 30);					//자행 100

				IniSetInt(_DEVICE_INI,DEV_SEC, "EarHandler", NOT_SET);					//  장애인ATM 지원
				IniSetInt(_DEVICE_INI,DEV_SEC, "Proximity", NOT_SET);
				IniSetInt(_DEVICE_INI,DEV_SEC, "SightHandicapped", NOT_SET);
			}
			
			m_pProfile->GetDeviceProfile();

			RestoreBrandFileCopy(); //#0091
			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";								
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT4; //#0091
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류입니다";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);

		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[브랜드설정 %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_부가서비스설정") //OM_부가서비스설정|편의점설정
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[부가서비스설정 %s]", strOmCommandArray[1]));

		if (strOmCommandArray.GetSize() >= 2)
		{
			IniSetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice11", strOmCommandArray[1]);
			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";								// Null Message
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류입니다";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);

		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[부가서비스설정 %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_VOLUME") //OM_VOLUME|OM_VOLUME=설정값
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[VOLUME 설정 %s]", strOmCommandArray[1]));

		if (strOmCommandArray.GetSize() >= 2)
		{
			int		nSoundVaule = 0;
			nSoundVaule = Asc2Int(strOmCommandArray[1]);		
			m_pDevCmn->fnSND_KeySound(_SOUND_DDING);		// DDING

			if ((nSoundVaule >= 1)	&&
				(nSoundVaule <= 5)	)
			{
				m_pProfile->DEVICE.SoundVolume = nSoundVaule;
				m_pProfile->PutDeviceProfile();
				nSoundVaule = (DWORD)(nSoundVaule * 11000);
				m_pDevCmn->fnSND_SetCurrentVolume(nSoundVaule);
			}			
			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";							
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류입니다";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);

		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[VOLUME 설정 %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_명세표설정") //OM_명세표설정|OM_명세표설정=설정값(1 : 사용, 0 : 미사용)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[명세표설정 %s]", strOmCommandArray[1]));

		if (strOmCommandArray.GetSize() >= 2)
		{
			m_pProfile->DEVICE.SHUHandler = Asc2Int(strOmCommandArray[1]);
			m_pProfile->PutDeviceProfile();
			m_pProfile->GetDeviceProfile();
			if(m_pProfile->DEVICE.SHUHandler == NOT_SET)
			{
				m_pDevCmn->DeviceAutoOff |= DEV_SPR;
				m_pDevCmn->DeviceDefine = m_pDevCmn->DeviceDefine & (~DEV_SPR); 
			}
			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";				
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT4;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류입니다";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[명세표설정 %s]", strOmResult));
	}
	else
	if (strOmCommandArray[0] == "OM_버전인자")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[버전인자]"));
		if (strOmCommandArray.GetSize() < 2)
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표인자 입력 오류입니다";
		}
		else
		if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		{
			m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_SPR);
			strTemp = GetShareData("OM_Main_CD_VanIII_ApSpEpVersion");
			SplitString(strTemp, "||", strTempArray);
			if (m_pProfile->DEVICE.MachineType == U3100K)
			{
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 0, 0, "      [SP Version]    [EP Version]");
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 1, 0, "  BRM :"+strTempArray[2]+"    "+strTempArray[2+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 2, 0, "  JPR :"+strTempArray[3]+"    "+strTempArray[3+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 3, 0, "  SPR :"+strTempArray[4]+"    "+strTempArray[4+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 4, 0, "  MCU :"+strTempArray[5]+"    "+strTempArray[5+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 5, 0, "  PIN :"+strTempArray[6]+"    "+strTempArray[6+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 6, 0, "  CAM :"+strTempArray[7]+"    "+strTempArray[7+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 7, 0, "  SIU :"+strTempArray[8]+"    "+strTempArray[8+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 8, 0, "  UPS :"+strTempArray[9]+"    "+strTempArray[9+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 9, 0, "  LOK :"+strTempArray[10]+"    "+strTempArray[10+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 10, 0, "  FNG :"+strTempArray[11]+"    "+strTempArray[11+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 11, 0, "  PBM :"+strTempArray[23]+"    "+strTempArray[24]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 12, 0, "  MWI :"+strTempArray[12]);
				strTemp.Format("%9.9s", m_pProfile->TRANS.ApVersion);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 13, 0, "  AP :"+strTemp);
				m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR, JNL_OPER);
			}
			else
			if (m_pProfile->DEVICE.MachineType == U8100)
			{
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 0, 0, "[SP Version]");
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 1, 0, "BRM:"+strTempArray[2]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 2, 0, "JPR:"+strTempArray[3]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 3, 0, "SPR:"+strTempArray[4]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 4, 0, "MCU:"+strTempArray[5]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 5, 0, "PIN:"+strTempArray[6]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 6, 0, "CAM:"+strTempArray[7]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 7, 0, "SIU:"+strTempArray[8]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 8, 0, "UPS:"+strTempArray[9]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 9, 0, "LOK:"+strTempArray[10]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 10, 0, "FNG:"+strTempArray[11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 11, 0, "PBM:"+strTempArray[23]);
				m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR, JNL_OPER);
				m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR);
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);
				
				m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_SPR); ////#0095				
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 0, 0, "[EP Version]");
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 1, 0, "BRM:"+strTempArray[2+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 2, 0, "JPR:"+strTempArray[3+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 3, 0, "SPR:"+strTempArray[4+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 4, 0, "MCU:"+strTempArray[5+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 5, 0, "PIN:"+strTempArray[6+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 6, 0, "CAM:"+strTempArray[7+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 7, 0, "SIU:"+strTempArray[8+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 8, 0, "UPS:"+strTempArray[9+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 9, 0, "LOK:"+strTempArray[10+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 10, 0, "FNG:"+strTempArray[11+11]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 11, 0, "PBM:"+strTempArray[24]);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 12, 0, "MWI:"+strTempArray[12]);
				strTemp.Format("%9.9s", m_pProfile->TRANS.ApVersion);
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, 13, 0, "AP  :"+strTemp);
				m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR, JNL_OPER);
			}

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
			{
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);
				if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
				{
					strOmResult = OM_NORMAL;
					strOmResult += "||[조작 성공]";	
				}
				else
				{
					strOmResult = OM_ERROR;
					strOmResult += "||명세표인자중 장애가 발생하였습니다";
				}
			}
			else
			{
				strOmResult = OM_ERROR;
				strOmResult += "||명세표인자중 장애가 발생하였습니다";
			}
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||명세표부 장애 상태입니다";
		}
	}
	else 
	if (strOmCommandArray[0] == "OM_날짜시간설정") //OM_날짜시간설정||201207230123
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[날짜시간설정 %s]", strOmCommandArray[1]));
		if (strOmCommandArray.GetSize() >= 2)
		{
			m_pDevCmn->fnAPL_SetDate(strOmCommandArray[1]);
			m_pDevCmn->fnAPL_SetCheckDate(strOmCommandArray[1]);
			m_pTranCmn->fnAPP_Set_SystemTime(strOmCommandArray[1]);
			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";							
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류입니다";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[날짜시간설정 %s]", strOmResult));

	}
	else 
	if (strOmCommandArray[0] == "OM_슈퍼비밀번호변경") //OM_슈퍼비밀번호변경||123456
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[슈퍼비밀번호변경 %s]", strOmCommandArray[1]));
		if (strOmCommandArray.GetSize() >= 2)
		{
			strTemp = "";
			strTemp = strOmCommandArray[1] + SPACE6;
			strTemp = strTemp.Left(6);
			memcpy(m_pProfile->TRANS.OpPassWord, strTemp.GetBuffer(0), strTemp.GetLength()); 
			m_pProfile->PutTransProfile();
			m_pProfile->GetTransProfile();
			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";							
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류입니다";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[슈퍼비밀번호변경 %s]", strOmResult));

	}
	else 
	if (strOmCommandArray[0] == "OM_거래설정정보") //OM_거래설정정보||서비스거래설정(1,2,3)||수수료조회설정(0,1)||최대금액설정자행(30,50,70)||최대금액설정타행(30,50,70)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OM_거래설정정보 %s %s %s %s %s %s %s]", strOmCommandArray[1], strOmCommandArray[2], strOmCommandArray[3], strOmCommandArray[4], strOmCommandArray[5], strOmCommandArray[6], strOmCommandArray[7])); //#N0278
		if (strOmCommandArray.GetSize() >= 8)
		{
			m_pProfile->TRANS.TranService = 0;
//			m_pProfile->TRANS.TranService = Asc2Int(strOmCommandArray[1]);
			m_pProfile->TRANS.TranInqFee = Asc2Int(strOmCommandArray[2]);
			m_pProfile->TRANS.JaMaxDispAmt = Asc2Int(strOmCommandArray[3]);	
			m_pProfile->TRANS.TaMaxDispAmt = Asc2Int(strOmCommandArray[4]);	
		
			if(m_pProfile->DEVICE.EarHandler != Asc2Int(strOmCommandArray[5]))
			{
				m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT4; //#0091
			}
			m_pProfile->DEVICE.Proximity = Asc2Int(strOmCommandArray[5]);			//  장애인ATM 지원
			m_pProfile->DEVICE.EarHandler = Asc2Int(strOmCommandArray[5]);
			m_pProfile->DEVICE.SightHandicapped = Asc2Int(strOmCommandArray[5]);
			m_pProfile->PutDeviceProfile();											//  장애인ATM 지원
			m_pProfile->GetDeviceProfile();
			m_pProfile->PutTransProfile();
			m_pProfile->GetTransProfile();

			IniSetStr("D:\\INI\\TRANS.INI", "TRANS", "foreginBrand", strOmCommandArray[6]);
			IniSetStr("D:\\INI\\TRANS.INI", "TRANS", "viRobotCtrl", strOmCommandArray[7]);

			strOmResult = OM_NORMAL;
			strOmResult += "||조작 성공";							
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OM_거래설정정보 %s]", strOmResult));

	}
	else 
	if (strOmCommandArray[0] == "OM_자동기리부팅") //OM_자동기리부팅
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[OM_자동기리부팅 %s]", strOmCommandArray[0]));
		m_pDevCmn->AtmStatus = ATM_DOWN;
		//#N0278
		m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT3;
		strOmResult = OM_NORMAL;
		strOmResult += "||조작 성공";	
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[자동기리부팅 %s]", strOmResult));

	}
	else 
	if (strOmCommandArray[0] == "OM_리부팅시간설정") //OM_리부팅시간설정||000000
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅요일시간설정 %s %s]", strOmCommandArray[1], strOmCommandArray[2]));
		if (strOmCommandArray.GetSize() >= 3)
		{
			strTemp = "";
			strTemp = strOmCommandArray[1] + SPACE6;
			strTemp = strTemp.Left(6);
			memcpy(m_pProfile->TRANS.RebootHHMMSS, strTemp.GetBuffer(0), strTemp.GetLength()); 
			m_pProfile->PutTransProfile();
			m_pProfile->GetTransProfile();

			char	szStartRebootTime[256];
			char	szEndRebootTime[256];
			memset(szStartRebootTime, 0x00, sizeof(szStartRebootTime));
			memset(szEndRebootTime,   0x00, sizeof(szEndRebootTime));
			memcpy(szStartRebootTime,  m_pProfile->TRANS.RebootHHMMSS, 6);
			memcpy(szEndRebootTime,    m_pProfile->TRANS.RebootHHMMSS, 6);
			AddString(szEndRebootTime, "010000", 6);

			if(memcmp(m_pProfile->TRANS.RebootHHMMSS, ZERO6, 6) != 0)
			{
				RegSetInt(_REGKEY_ATMRESTART, "UseRestart"    , 1);
				RegSetStr(_REGKEY_ATMRESTART, "CheckStartTime", szStartRebootTime);
				RegSetStr(_REGKEY_ATMRESTART, "CheckEndTime"  , szEndRebootTime);
				RegSetInt(_REGKEY_ATMRESTART, "ReadyRestart"  , 0);
			}

			m_pDevCmn->AtmRestartInfo.bUseRestart		= RegGetInt(_REGKEY_ATMRESTART, "UseRestart", 1);
			m_pDevCmn->AtmRestartInfo.strCheckStartTime= RegGetStr(_REGKEY_ATMRESTART, "CheckStartTime", "040000");
			m_pDevCmn->AtmRestartInfo.strCheckEndTime	= RegGetStr(_REGKEY_ATMRESTART, "CheckEndTime", "050000");
			m_pDevCmn->AtmRestartInfo.bReadyRestart	= RegGetInt(_REGKEY_ATMRESTART, "ReadyRestart", 0);
			
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation: CheckStartTime[%s] ~ CheckEndTime[%s]", szStartRebootTime, szEndRebootTime);


			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";							
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류입니다";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[리부팅시간설정 %s]", strOmResult));

	}
	else
	if (strOmCommandArray[0] == "OM_ALARMONOFF") //OM_ALARMONOFF||ON/OFF
	{
		if (strOmCommandArray.GetSize() >= 2)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[충격장치 %s]", strOmCommandArray[1]));
			int nGetIni = 0;
			nGetIni = IniGetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19");

		if( strOmCommandArray[1] == "ON")
			{
				if(nGetIni == 1)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_ON, CLKEY_ON, CLKEY_OFF);
				}
				else
				if(nGetIni == 2)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_ON, CLKEY_OFF, CLKEY_ON);
				}
				else
				if(nGetIni == 3)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_ON, CLKEY_ON, CLKEY_ON);
				}
				else
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_ON, CLKEY_OFF, CLKEY_OFF);

				m_pDevCmn->fnSPACK_AlarmOnOff(FALSE);
			}
			else
			{
				if(nGetIni == 1)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_ON, CLKEY_OFF);
				}
				else
				if(nGetIni == 2)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_ON);
				}
				else
				if(nGetIni == 3)
				{
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_ON, CLKEY_ON);
				}
				else
					m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_OFF);

	
				m_pDevCmn->fnSPACK_AlarmOnOff(TRUE);
			}

			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";	

		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류!";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[충격장치 %s]", strOmResult));
	}
	else 
	if (strOmCommandArray[0] == "OM_PBM_SETUP") //OM_PBM_SETUP|OM_PBM_SETUP=설정값(1 : 사용, 0 : 미사용)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult1[%s]", strOmCommandArray[0]);
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[통장부설정 %s]", strOmCommandArray[1]));

		if (strOmCommandArray.GetSize() >= 2)
		{
			m_pProfile->DEVICE.PBHandler = Asc2Int(strOmCommandArray[1]);
			m_pProfile->PutDeviceProfile();
			strOmResult = OM_NORMAL;
			strOmResult += "||[조작 성공]";				
			m_pDevCmn->m_nAtmsCommand = ATMS_CMD_REBOOT4;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult2[%s]", strOmCommandArray[0]);
		}
		else
		{
			strOmResult = OM_ERROR;
			strOmResult += "||입력 오류입니다";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation:strOmResult3[%s]", strOmCommandArray[0]);
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[통장부설정 %s]", strOmResult));
	}
	else														
	{
		strOmResult = OM_ERROR;
	}
/////////////////////////////////////////////////////////////////////////////
	ClerkProcInCnt = 1;											// 재호출시응답처리
	return TRUE;
}





CStringArray strAdapterId;	
int CWinAtmCtrl::AtmGetAdapterId()
{
	// The following code is designed for Windows 2K.
	// If you want it to work on Windows NT, 
	//   you should read HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\NetworkCards

	HKEY hKey, hSubKey, hNdiIntKey;
	
	
	strAdapterId.RemoveAll();									// Clear

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"System\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}",
		0,
		KEY_READ,
		&hKey) != ERROR_SUCCESS)
		return FALSE;											// Register Open Failed
	
	DWORD			dwIndex		= 0;
	DWORD			dwBufSize	= 256;
	DWORD			dwDataType;
	char			szSubKey[256];
	unsigned char	szData[256];

	memset(szData, 0x00, sizeof(szData));					
	
	while(RegEnumKeyEx(hKey, dwIndex++, szSubKey, &dwBufSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		if(RegOpenKeyEx(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
		{		
			if(RegOpenKeyEx(hSubKey, "Ndi\\Interfaces", 0, KEY_READ, &hNdiIntKey) == ERROR_SUCCESS)
			{
				dwBufSize = 256;
				if(RegQueryValueEx(hNdiIntKey, "LowerRange", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
				{
					if(strcmp((char*)szData, "ethernet") == 0)	// Identify netcard
					{
						dwBufSize = 256;
						if(RegQueryValueEx(hSubKey, "DriverDesc", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
						{
							dwBufSize = 256;
							if(RegQueryValueEx(hSubKey, "NetCfgInstanceID", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
								strAdapterId.Add((LPCTSTR)szData);
						}
					}
				}
				RegCloseKey(hNdiIntKey);
			}
			RegCloseKey(hSubKey);
		}
		dwBufSize = 256;
	}	/* end of while */
	
	RegCloseKey(hKey);

	if(!strAdapterId.GetSize())									// No Netcard
		return FALSE;
	else
		return TRUE;
}

int CWinAtmCtrl::AtmSetEnableDHCP(int nFlag)
{
	BOOL	bRet = TRUE;

	CString	strIpAddress("192.168.1.10");
	CString strSubnetMask("255.255.255.0");
	CString strGateway("192.168.1.1");

MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "AtmSetEnableDHCP(%d)", nFlag);

	if(nFlag == FALSE)
	{
		ShellExecute( NULL , NULL , "netsh.exe", "interface ip set address \"로컬 영역 연결\" static " + strIpAddress + " " + strSubnetMask + " " + strGateway + " 1", NULL, SW_SHOW);			
	}
	else
	if(nFlag == TRUE)
	{
		ShellExecute( NULL , NULL , "netsh.exe", "interface ip set address \"로컬 영역 연결\" dhcp", NULL, SW_SHOW);
	}
	
	
	return bRet;
}




